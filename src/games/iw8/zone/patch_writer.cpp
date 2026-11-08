#include <std_include.hpp>
#include "patch_writer.h"

#include "../iw8.hpp"

#include <game.hpp>
#include <xfile/xfile.hpp>

#include <xxhash.h>

#include <algorithm>
#include <array>
#include <cstring>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>

// The Replay patch writer, the inverse of xfile.cpp's apply_iw8_patch / decode_vcdiff / copy_vcdiff. The layout follows
// the stock patches of the Replay install (37 surveyed .fp files, all accepted by that reader with their checksums):
//
//   0x000  "IWffd100", version 6 (u32), diff version 4 (u32 at 0x00C, required by Replay's DB_ReadFastfileHeaderData)
//   0x010  BDiff window sizes (u64 x3): largest target window, largest source segment end, largest window record
//          (allocation hints for Replay's temporary buffers; the reader ignores them)
//   0x028  frame size (u64), delta size (u64)
//   0x038  base zone header (0x88 bytes): the deployed file's header, checked byte for byte by the reader
//   0x0C0  patched zone header (0x88 bytes): the header the patched zone takes
//   0x148  frame: "\x01IWC" + delta (stored) or Replay's compressed IWC frame (xfile::compress_iw8_frame)
//
// The delta is RFC 3284 VCDIFF in the Replay form the reader accepts: delta header D6 C3 C4 00 08, and one window per
// 2 MiB of target (every non-final stock window is exactly 2 MiB). Each window has indicator 0x09 (source segment and
// checksum), the whole base payload as its source segment, and an XXH64 checksum seeded with the previous one. The
// window is encoded greedily: RUN for runs of identical bytes, COPY for 16-byte blocks found in the base payload,
// ADD for the rest. Copy addresses use the RFC address cache exactly as the reader updates it.
namespace zonetool::iw8
{
	const game& definition();

	namespace
	{
		constexpr std::size_t file_header_size = 0x88;
		constexpr std::size_t patch_header_size = 0x148;
		constexpr std::size_t base_header_offset = 0x38;
		constexpr std::size_t patched_header_offset = 0xC0;
		constexpr std::size_t size_field_offset = 32;
		constexpr std::size_t frame_size_offset = 0x28;
		constexpr std::uint32_t patch_version = 6;
		constexpr std::uint32_t patch_diff_version = 4;

		constexpr std::size_t window_size = 2 * 1024 * 1024;
		// Slack on the largest window record in the window size hint. The stock hints equal the largest records; the
		// margin only keeps a writer difference from short-sizing one of Replay's temporary buffers.
		constexpr std::uint64_t window_record_margin = 64;
		constexpr std::uint8_t delta_header_indicator = 0x08;
		constexpr std::uint8_t window_indicator = 0x09;

		// Code table entries used by the writer (see xfile.cpp vcdiff_code_table): code 0 is RUN with its size read from
		// the instructions, code 1 is ADD with its size read, code 1 + n is ADD n (n 1..17), code 19 + 16 m is COPY mode
		// m with its size read and code 19 + 16 m + (n - 3) is COPY mode m size n (n 4..18).
		constexpr std::uint8_t code_run = 0;
		constexpr std::uint8_t code_add = 1;
		constexpr std::uint8_t code_copy = 19;
		constexpr std::size_t code_copy_stride = 16;

		constexpr std::size_t near_size = 4;
		constexpr std::size_t same_size = 3 * 256;

		constexpr std::size_t block_size = 16;
		constexpr std::size_t run_min = 32;
		constexpr std::size_t probe_limit = 8;
		constexpr std::size_t max_table_slots = std::size_t{1} << 27;
		constexpr std::uint64_t hash_prime = 0x100000001B3ull;

		// hash_prime^(block_size - 1): the weight of the byte that leaves a rolling block.
		constexpr std::uint64_t hash_top = []
		{
			std::uint64_t power = 1;
			for (std::size_t i = 1; i < block_size; i++)
			{
				power *= hash_prime;
			}

			return power;
		}();

		enum class vcdiff_op
		{
			run,
			add,
			copy,
		};

		void append_u32(std::string& out, const std::uint32_t value)
		{
			for (int shift = 0; shift < 32; shift += 8)
			{
				out.push_back(static_cast<char>(value >> shift));
			}
		}

		void put_u32(std::string& out, const std::size_t offset, const std::uint32_t value)
		{
			for (std::size_t i = 0; i < 4; i++)
			{
				out[offset + i] = static_cast<char>(value >> (8 * i));
			}
		}

		void put_u64(std::string& out, const std::size_t offset, const std::uint64_t value)
		{
			for (std::size_t i = 0; i < 8; i++)
			{
				out[offset + i] = static_cast<char>(value >> (8 * i));
			}
		}

		// The base-128 integer of RFC 3284 section 2: most significant group first, bit 7 set on every byte but the last.
		void write_integer(std::string& out, std::uint64_t value)
		{
			std::array<char, 10> chunks{};
			std::size_t count = 0;
			chunks[count++] = static_cast<char>(value & 0x7F);
			for (value >>= 7; value; value >>= 7)
			{
				chunks[count++] = static_cast<char>(0x80 | (value & 0x7F));
			}

			while (count)
			{
				out.push_back(chunks[--count]);
			}
		}

		std::size_t integer_size(std::uint64_t value)
		{
			std::size_t size = 1;
			for (value >>= 7; value; value >>= 7)
			{
				size++;
			}

			return size;
		}

		std::uint64_t block_hash(const char* data)
		{
			std::uint64_t hash = 0;
			for (std::size_t i = 0; i < block_size; i++)
			{
				hash = hash * hash_prime + static_cast<std::uint8_t>(data[i]);
			}

			return hash;
		}

		// The reader's window checksum (decode_vcdiff): XXH64 seeded with the previous checksum, folded to 32 bits.
		std::uint32_t window_checksum(const std::string_view data, const std::uint32_t seed)
		{
			const auto hash = XXH64(data.data(), data.size(), seed);
			return static_cast<std::uint32_t>(hash ^ (hash >> 32));
		}

		// The address cache of vcdiff_address_cache: the reader updates it with every copy address, in every mode.
		// (The members avoid the names near/same, which the Windows headers may define.)
		struct address_cache
		{
			std::array<std::size_t, near_size> near_addresses{};
			std::size_t next_near{};
			std::array<std::size_t, same_size> same_addresses{};

			void update(const std::size_t address)
			{
				near_addresses[next_near] = address;
				next_near = (next_near + 1) % near_size;
				same_addresses[address % same_size] = address;
			}
		};

		// The instruction, address and data streams of one window.
		class window_streams
		{
		public:
			std::string data;
			std::string instructions;
			std::string addresses;

			void add(const std::string_view literal)
			{
				instruction(vcdiff_op::add, literal.size(), 0);
				data.append(literal);
			}

			void run(const char byte, const std::size_t count)
			{
				instruction(vcdiff_op::run, count, 0);
				data.push_back(byte);
			}

			// `here` is the reader's address of the copy's first byte: source size + target bytes already in the window.
			void copy(const std::size_t address, const std::size_t count, const std::size_t here)
			{
				instruction(vcdiff_op::copy, count, encode_address(address, here));
			}

		private:
			address_cache cache_;

			void instruction(const vcdiff_op op, const std::size_t count, const std::uint8_t mode)
			{
				std::uint8_t code = code_run;
				bool sized = false;
				switch (op)
				{
				case vcdiff_op::run:
					break;
				case vcdiff_op::add:
					if (count >= 1 && count <= 17)
					{
						code = static_cast<std::uint8_t>(code_add + count);
						sized = true;
					}
					else
					{
						code = code_add;
					}
					break;
				case vcdiff_op::copy:
				{
					const auto base = static_cast<std::uint8_t>(code_copy + code_copy_stride * mode);
					if (count >= 4 && count <= 18)
					{
						code = static_cast<std::uint8_t>(base + (count - 3));
						sized = true;
					}
					else
					{
						code = base;
					}
					break;
				}
				}

				instructions.push_back(static_cast<char>(code));
				if (!sized)
				{
					write_integer(instructions, count);
				}
			}

			// Chooses the cheapest mode the reader decodes: the same-cache entry (one byte), else self, here or a near
			// entry (integer). Ties keep the lower mode, as the cost comparison of the reference writer does.
			std::uint8_t encode_address(const std::size_t address, const std::size_t here)
			{
				std::uint8_t mode = 0;
				const auto index = address % same_size;
				if (cache_.same_addresses[index] == address)
				{
					mode = static_cast<std::uint8_t>(6 + index / 256);
					addresses.push_back(static_cast<char>(index % 256));
				}
				else
				{
					struct option
					{
						std::size_t cost;
						std::uint8_t mode;
						std::size_t value;
					};

					std::array<option, 2 + near_size> options{};
					std::size_t count = 0;
					options[count++] = {integer_size(address), 0, address};
					if (here >= address)
					{
						options[count++] = {integer_size(here - address), 1, here - address};
					}

					for (std::size_t j = 0; j < near_size; j++)
					{
						if (address >= cache_.near_addresses[j])
						{
							const auto difference = address - cache_.near_addresses[j];
							options[count++] = {integer_size(difference), static_cast<std::uint8_t>(2 + j), difference};
						}
					}

					auto best = options[0];
					for (std::size_t i = 1; i < count; i++)
					{
						if (options[i].cost < best.cost || (options[i].cost == best.cost && options[i].mode < best.mode))
						{
							best = options[i];
						}
					}

					mode = best.mode;
					write_integer(addresses, best.value);
				}

				cache_.update(address);
				return mode;
			}
		};

		// Hash table of aligned source blocks. Slots hold position + 1 (0 is empty) and fill linearly from the block's
		// home slot, at most probe_limit steps. A lookup stops at the first empty slot, which is safe because nothing is
		// removed. Candidates are verified byte for byte, so hash collisions only cost a comparison.
		class source_index
		{
		public:
			explicit source_index(const std::string_view source) : source_(source)
			{
				if (source.size() >= 0xFFFFFFFFull)
				{
					throw std::runtime_error("a patch base larger than 4 GiB needs a wider block index");
				}

				const auto blocks = source.size() / block_size;
				slots_ = 1024;
				while (slots_ < blocks * 2 && slots_ < max_table_slots)
				{
					slots_ <<= 1;
				}

				table_.assign(slots_, 0);
				for (std::size_t pos = 0; pos + block_size <= source.size(); pos += block_size)
				{
					insert(block_hash(source.data() + pos), pos);
				}
			}

			// Writes up to probe_limit source positions whose block equals `block`, returns how many.
			std::size_t find(const std::uint64_t hash, const char* block, std::array<std::size_t, probe_limit>& out) const
			{
				std::size_t count = 0;
				const auto home = hash & (slots_ - 1);
				for (std::size_t probe = 0; probe < probe_limit; probe++)
				{
					const auto slot = table_[(home + probe) & (slots_ - 1)];
					if (slot == 0)
					{
						break;
					}

					const auto pos = static_cast<std::size_t>(slot - 1);
					if (std::memcmp(source_.data() + pos, block, block_size) == 0)
					{
						out[count++] = pos;
					}
				}

				return count;
			}

		private:
			std::string_view source_;
			std::size_t slots_{};
			std::vector<std::uint32_t> table_;

			void insert(const std::uint64_t hash, const std::size_t pos)
			{
				const auto home = hash & (slots_ - 1);
				for (std::size_t probe = 0; probe < probe_limit; probe++)
				{
					auto& slot = table_[(home + probe) & (slots_ - 1)];
					if (slot == 0)
					{
						slot = static_cast<std::uint32_t>(pos + 1);
						return;
					}
				}
				// A full chain drops the block: it is only a missed copy, never a wrong one.
			}
		};

		bool is_run(const std::string_view target, const std::size_t pos, const std::size_t length)
		{
			return std::all_of(target.begin() + pos, target.begin() + pos + length,
				[first = target[pos]](const char c) { return c == first; });
		}

		// Greedy VCDIFF window over target[start, end): the reader's window starts at target size `start`.
		window_streams encode_window(const std::string_view source, const source_index& index, const std::string_view target,
			const std::size_t start, const std::size_t end)
		{
			window_streams out;
			std::size_t literal = start;
			std::size_t pos = start;
			bool rolling_valid = false;
			std::uint64_t rolling = 0;
			std::array<std::size_t, probe_limit> candidates{};

			while (pos < end)
			{
				if (pos + run_min <= end && is_run(target, pos, run_min))
				{
					auto run_end = pos + run_min;
					while (run_end < end && target[run_end] == target[pos])
					{
						run_end++;
					}

					if (pos > literal)
					{
						out.add(target.substr(literal, pos - literal));
					}

					out.run(target[pos], run_end - pos);
					pos = literal = run_end;
					rolling_valid = false;
					continue;
				}

				std::size_t best_length = 0;
				std::size_t best_address = 0;
				std::size_t best_start = pos;
				if (pos + block_size <= end)
				{
					if (!rolling_valid)
					{
						rolling = block_hash(target.data() + pos);
						rolling_valid = true;
					}

					const auto count = index.find(rolling, target.data() + pos, candidates);
					for (std::size_t i = 0; i < count; i++)
					{
						const auto candidate = candidates[i];
						std::size_t length = block_size;
						while (pos + length < end && candidate + length < source.size() &&
							source[candidate + length] == target[pos + length])
						{
							length++;
						}

						// Extend the copy back over literal bytes that are still pending.
						std::size_t back = 0;
						while (pos - back > literal && candidate - back > 0 &&
							source[candidate - back - 1] == target[pos - back - 1])
						{
							back++;
						}

						if (length + back > best_length)
						{
							best_length = length + back;
							best_address = candidate - back;
							best_start = pos - back;
						}
					}
				}

				if (best_length >= block_size)
				{
					if (best_start > literal)
					{
						out.add(target.substr(literal, best_start - literal));
					}

					out.copy(best_address, best_length, source.size() + (best_start - start));
					pos = literal = best_start + best_length;
					rolling_valid = false;
				}
				else
				{
					if (rolling_valid && pos + block_size < end)
					{
						rolling = (rolling - static_cast<std::uint8_t>(target[pos]) * hash_top) * hash_prime +
							static_cast<std::uint8_t>(target[pos + block_size]);
					}
					else
					{
						rolling_valid = false;
					}

					pos++;
				}
			}

			if (end > literal)
			{
				out.add(target.substr(literal, end - literal));
			}

			return out;
		}

		std::string encode_delta(const std::string_view source, const std::string_view target, std::uint64_t& max_record)
		{
			const source_index index(source);

			std::string delta("\xD6\xC3\xC4\x00", 4);
			delta.push_back(static_cast<char>(delta_header_indicator));

			std::uint32_t checksum = 0;
			for (std::size_t start = 0; start < target.size(); start += window_size)
			{
				const auto end = std::min(target.size(), start + window_size);
				const auto streams = encode_window(source, index, target, start, end);

				std::string body;
				write_integer(body, end - start);
				body.push_back('\0'); // delta indicator: no secondary compressor
				write_integer(body, streams.data.size());
				write_integer(body, streams.instructions.size());
				write_integer(body, streams.addresses.size());
				body += streams.data;
				body += streams.instructions;
				body += streams.addresses;

				// The record of this window as the reader sees it: indicator, source segment, position 0, body size and body.
				const auto record = 1 + integer_size(source.size()) + integer_size(0) + integer_size(body.size()) + body.size();
				max_record = std::max<std::uint64_t>(max_record, record);

				delta.push_back(static_cast<char>(window_indicator));
				write_integer(delta, source.size()); // source segment: the whole base payload
				write_integer(delta, 0);
				write_integer(delta, body.size());
				delta += body;

				checksum = window_checksum(target.substr(start, end - start), checksum);
				append_u32(delta, checksum);
			}

			return delta;
		}

		std::string read_header(const std::filesystem::path& path)
		{
			std::ifstream file(path, std::ios::binary);
			std::string header(file_header_size, '\0');
			if (!file.read(header.data(), static_cast<std::streamsize>(header.size())))
			{
				throw std::runtime_error(std::format("{} has no zone header", path.string()));
			}

			return header;
		}

		std::uint64_t header_size(const std::string& header)
		{
			std::uint64_t size{};
			std::memcpy(&size, header.data() + size_field_offset, sizeof(size));
			return size;
		}
	}

	void write_patch(const std::filesystem::path& base, const std::filesystem::path& patched,
		const std::filesystem::path& output)
	{
		const auto& format = definition().format;
		const auto base_header = read_header(base);
		const auto patched_header = read_header(patched);

		std::string head(patch_header_size, '\0');
		std::memcpy(head.data(), "IWffd100", 8);
		put_u32(head, 8, patch_version);
		put_u32(head, 12, patch_diff_version);
		std::memcpy(head.data() + base_header_offset, base_header.data(), file_header_size);
		std::memcpy(head.data() + patched_header_offset, patched_header.data(), file_header_size);

		std::size_t frame_size = 0;
		// BDiff window size hints (iw8_patch_header::window_sizes); zero when the patch has no delta.
		std::array<std::uint64_t, 3> window_sizes{};
		{
			// Both payloads are read without any .fp beside them; they go out of scope before the file is written.
			const auto base_zone = xfile::read(base, format, false);
			const auto patched_zone = xfile::read(patched, format, false);
			if (base_zone.payload.size() != header_size(base_header))
			{
				throw std::runtime_error(std::format("{} holds 0x{:X} payload bytes, not 0x{:X}", base.string(),
					base_zone.payload.size(), header_size(base_header)));
			}

			if (patched_zone.payload.size() != header_size(patched_header))
			{
				throw std::runtime_error(std::format("{} holds 0x{:X} payload bytes, not 0x{:X}", patched.string(),
					patched_zone.payload.size(), header_size(patched_header)));
			}

			// An unchanged payload needs no frame: the reader accepts a header-only patch of equal size.
			if (base_zone.payload != patched_zone.payload)
			{
				std::uint64_t max_record = 0;
				const auto delta = encode_delta(base_zone.payload, patched_zone.payload, max_record);
				// Replay sizes its temporary BDiff buffers from these: the largest target window, the whole base as the source
				// segment (segment size + position - extra) and the largest window record.
				window_sizes = {std::min<std::uint64_t>(patched_zone.payload.size(), window_size), base_zone.payload.size(),
					max_record + window_record_margin};
				auto frame = xfile::compress_iw8_frame(delta);
				if (frame.empty())
				{
					frame = std::string("\x01IWC", 4) + delta;
				}

				frame_size = frame.size();
				head.append(frame);
				put_u64(head, frame_size_offset, frame.size());
				put_u64(head, frame_size_offset + 8, delta.size());
				for (std::size_t i = 0; i < window_sizes.size(); i++)
				{
					put_u64(head, 16 + 8 * i, window_sizes[i]);
				}
			}
		}

		if (output.has_parent_path())
		{
			std::filesystem::create_directories(output.parent_path());
		}

		// Write beside the destination and replace it after the complete patch is on disk.
		auto partial = output;
		partial += ".partial";
		{
			std::ofstream file(partial, std::ios::binary | std::ios::trunc);
			if (!file.write(head.data(), static_cast<std::streamsize>(head.size())))
			{
				std::filesystem::remove(partial);
				throw std::runtime_error(std::format("failed to write {}", partial.string()));
			}
		}

		std::filesystem::rename(partial, output);
		ZONETOOL_INFO("patch %s: 0x%zX bytes (frame 0x%zX), base %s, patched %s", output.string().data(), head.size(),
			frame_size, base.string().data(), patched.string().data());
	}
}
