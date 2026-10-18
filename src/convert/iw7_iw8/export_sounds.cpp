#include <std_include.hpp>
#include "export.hpp"

#include <io/game_files.hpp>

namespace zonetool::convert::iw7_iw8
{
	namespace
	{
		// The bits of a FLAC stream, most significant first.
		class bit_reader
		{
		public:
			bit_reader(const std::uint8_t* data, const std::size_t size)
				: data_(data), size_(size)
			{
			}

			std::uint32_t read(unsigned bits)
			{
				std::uint32_t value{};
				while (bits)
				{
					if ((this->position_ >> 3) >= this->size_)
					{
						throw std::runtime_error("FLAC frame ends early");
					}

					const auto offset = static_cast<unsigned>(this->position_ & 7);
					const auto take = std::min(bits, 8 - offset);
					const auto byte = this->data_[this->position_ >> 3];
					value = (value << take) | ((byte >> (8 - offset - take)) & ((1u << take) - 1));
					this->position_ += take;
					bits -= take;
				}

				return value;
			}

			std::int32_t read_signed(const unsigned bits)
			{
				if (!bits)
				{
					return 0;
				}

				const auto value = this->read(bits);
				return bits >= 32 ? static_cast<std::int32_t>(value)
					: static_cast<std::int32_t>(value << (32 - bits)) >> (32 - bits);
			}

			std::uint32_t read_unary()
			{
				std::uint32_t count{};
				while (!this->read(1))
				{
					count++;
				}

				return count;
			}

			void align()
			{
				this->position_ = (this->position_ + 7) & ~static_cast<std::size_t>(7);
			}

			std::size_t byte_position() const
			{
				return this->position_ >> 3;
			}

		private:
			const std::uint8_t* data_;
			std::size_t size_;
			std::size_t position_{};
		};

		// The Rice-coded residual of a subframe, after its warm-up samples.
		void decode_residual(bit_reader& reader, const std::size_t block, const unsigned order, std::vector<std::int32_t>& out)
		{
			const auto method = reader.read(2);
			if (method > 1)
			{
				throw std::runtime_error("FLAC residual coding method is reserved");
			}

			const auto parameter_bits = method ? 5u : 4u;
			const auto escape = method ? 31u : 15u;
			const auto partition_order = reader.read(4);
			const auto partitions = std::size_t{1} << partition_order;
			auto at = static_cast<std::size_t>(order);
			for (std::size_t partition = 0; partition < partitions; partition++)
			{
				const auto samples = (block >> partition_order) - (partition == 0 ? order : 0);
				const auto parameter = reader.read(parameter_bits);
				if (parameter == escape)
				{
					// An escaped partition holds its samples unencoded.
					const auto bits = reader.read(5);
					for (std::size_t i = 0; i < samples && at < block; i++, at++)
					{
						out[at] = reader.read_signed(bits);
					}

					continue;
				}

				for (std::size_t i = 0; i < samples && at < block; i++, at++)
				{
					const auto value = (reader.read_unary() << parameter) | (parameter ? reader.read(parameter) : 0u);
					out[at] = static_cast<std::int32_t>(value >> 1) ^ -static_cast<std::int32_t>(value & 1);
				}
			}
		}

		void decode_subframe(bit_reader& reader, const std::size_t block, unsigned bits, std::vector<std::int32_t>& out)
		{
			out.assign(block, 0);
			reader.read(1);
			const auto type = reader.read(6);
			unsigned wasted{};
			if (reader.read(1))
			{
				wasted = reader.read_unary() + 1;
				bits -= wasted;
			}

			if (type == 0)
			{
				std::ranges::fill(out, reader.read_signed(bits));
			}
			else if (type == 1)
			{
				for (auto& sample : out)
				{
					sample = reader.read_signed(bits);
				}
			}
			else if (type >= 8 && type <= 12)
			{
				const auto order = type - 8;
				for (auto i = 0u; i < order; i++)
				{
					out[i] = reader.read_signed(bits);
				}

				decode_residual(reader, block, order, out);
				for (auto i = static_cast<std::size_t>(order); i < block; i++)
				{
					const auto* s = out.data() + i;
					switch (order)
					{
					case 1: out[i] += s[-1]; break;
					case 2: out[i] += 2 * s[-1] - s[-2]; break;
					case 3: out[i] += 3 * s[-1] - 3 * s[-2] + s[-3]; break;
					case 4: out[i] += 4 * s[-1] - 6 * s[-2] + 4 * s[-3] - s[-4]; break;
					default: break;
					}
				}
			}
			else if (type >= 32)
			{
				const auto order = type - 31;
				for (auto i = 0u; i < order; i++)
				{
					out[i] = reader.read_signed(bits);
				}

				const auto precision = reader.read(4) + 1;
				const auto shift = reader.read_signed(5);
				std::array<std::int32_t, 32> coefficients{};
				for (auto i = 0u; i < order; i++)
				{
					coefficients[i] = reader.read_signed(precision);
				}

				decode_residual(reader, block, order, out);
				for (auto i = static_cast<std::size_t>(order); i < block; i++)
				{
					std::int64_t sum{};
					for (auto j = 0u; j < order; j++)
					{
						sum += static_cast<std::int64_t>(coefficients[j]) * out[i - 1 - j];
					}

					out[i] += static_cast<std::int32_t>(sum >> std::max(shift, 0));
				}
			}
			else
			{
				throw std::runtime_error(std::format("FLAC subframe type {} is reserved", type));
			}

			if (wasted)
			{
				for (auto& sample : out)
				{
					sample <<= wasted;
				}
			}
		}

		// Decodes FLAC frames (IW7 sound asset banks keep them without the stream header) into interleaved 16-bit PCM.
		std::vector<std::int16_t> decode_flac(const std::uint8_t* data, const std::size_t size, const unsigned channels,
			const std::size_t frames)
		{
			std::vector<std::int16_t> pcm;
			pcm.reserve(frames * channels);
			bit_reader reader(data, size);
			std::array<std::vector<std::int32_t>, 2> decoded;
			while (reader.byte_position() + 4 < size && pcm.size() < frames * channels)
			{
				if (reader.read(14) != 0x3FFE)
				{
					throw std::runtime_error("FLAC frame has no sync code");
				}

				reader.read(2);
				const auto block_code = reader.read(4);
				const auto rate_code = reader.read(4);
				const auto assignment = reader.read(4);
				const auto size_code = reader.read(3);
				reader.read(1);
				const auto first = reader.read(8);
				auto ones = 0u;
				while (ones < 8 && (first & (0x80u >> ones)))
				{
					ones++;
				}

				for (auto i = 1u; i < ones; i++)
				{
					reader.read(8);
				}

				std::size_t block = block_code == 1 ? 192 : block_code >= 2 && block_code <= 5 ? std::size_t{576} << (block_code - 2)
					: block_code == 6 ? reader.read(8) + 1 : block_code == 7 ? reader.read(16) + 1
					: block_code >= 8 ? std::size_t{256} << (block_code - 8) : 0;
				if (rate_code == 12)
				{
					reader.read(8);
				}
				else if (rate_code == 13 || rate_code == 14)
				{
					reader.read(16);
				}

				reader.read(8);
				const auto bits = size_code == 1 ? 8u : size_code == 2 ? 12u : size_code == 5 ? 20u : size_code == 6 ? 24u : 16u;
				const auto frame_channels = assignment < 8 ? assignment + 1 : 2u;
				if (!block || frame_channels != channels)
				{
					throw std::runtime_error("FLAC frame does not match its sample");
				}

				for (auto c = 0u; c < frame_channels; c++)
				{
					const auto side = (assignment == 8 && c == 1) || (assignment == 9 && c == 0) || (assignment == 10 && c == 1);
					decode_subframe(reader, block, bits + (side ? 1 : 0), decoded[c]);
				}

				reader.align();
				reader.read(16);
				const auto clamp = [&](const std::int32_t value)
				{
					const auto scaled = bits == 16 ? value : bits > 16 ? value >> (bits - 16) : value << (16 - bits);
					return static_cast<std::int16_t>(std::clamp(scaled, -32768, 32767));
				};

				for (std::size_t i = 0; i < block; i++)
				{
					auto left = decoded[0][i];
					auto right = channels == 2 ? decoded[1][i] : 0;
					if (assignment == 8)
					{
						right = left - right;
					}
					else if (assignment == 9)
					{
						left += right;
					}
					else if (assignment == 10)
					{
						const auto mid = (static_cast<std::int64_t>(left) << 1) | (right & 1);
						left = static_cast<std::int32_t>((mid + right) >> 1);
						right = static_cast<std::int32_t>((mid - right) >> 1);
					}

					pcm.push_back(clamp(left));
					if (channels == 2)
					{
						pcm.push_back(clamp(right));
					}
				}
			}

			pcm.resize(std::min(pcm.size(), frames * channels));
			return pcm;
		}

		void write_wav(const std::filesystem::path& path, const std::vector<std::int16_t>& pcm, const unsigned rate,
			const unsigned channels)
		{
			std::filesystem::create_directories(path.parent_path());
			std::ofstream file(path, std::ios::binary | std::ios::trunc);
			const auto data_bytes = static_cast<std::uint32_t>(pcm.size() * 2);
			const auto put32 = [&](const std::uint32_t value) { file.write(reinterpret_cast<const char*>(&value), 4); };
			const auto put16 = [&](const std::uint16_t value) { file.write(reinterpret_cast<const char*>(&value), 2); };
			file.write("RIFF", 4);
			put32(36 + data_bytes);
			file.write("WAVEfmt ", 8);
			put32(16);
			put16(1);
			put16(static_cast<std::uint16_t>(channels));
			put32(rate);
			put32(rate * channels * 2);
			put16(static_cast<std::uint16_t>(channels * 2));
			put16(16);
			file.write("data", 4);
			put32(data_bytes);
			file.write(reinterpret_cast<const char*>(pcm.data()), data_bytes);
		}

		// A sound asset bank (.sabl) of the zone folder: its entries by id, and the bytes of each.
		class asset_bank
		{
		public:
			explicit asset_bank(const std::string& name)
				: name_(name)
			{
				iw7::SndAssetBankHeader header{};
				if (!game_files::get().read(name, 0, sizeof(header), &header) || header.entrySize != sizeof(iw7::SndAssetBankEntry))
				{
					return;
				}

				std::vector<iw7::SndAssetBankEntry> entries(header.entryCount);
				if (!game_files::get().read(name, header.entryOffset, entries.size() * sizeof(iw7::SndAssetBankEntry), entries.data()))
				{
					return;
				}

				for (const auto& entry : entries)
				{
					this->entries_.emplace(entry.id, entry);
				}
			}

			const iw7::SndAssetBankEntry* find(const std::uint32_t id) const
			{
				const auto found = this->entries_.find(id);
				return found != this->entries_.end() ? &found->second : nullptr;
			}

			std::optional<std::vector<std::uint8_t>> read(const iw7::SndAssetBankEntry& entry) const
			{
				std::vector<std::uint8_t> bytes(entry.size);
				if (!game_files::get().read(this->name_, entry.offset + entry.seekTableSize, bytes.size(), bytes.data()))
				{
					return {};
				}

				return bytes;
			}

			std::size_t size() const
			{
				return this->entries_.size();
			}

		private:
			std::string name_;
			std::unordered_map<std::uint32_t, iw7::SndAssetBankEntry> entries_;
		};
	}

	void export_sounds(const source_map& source, extraction& output)
	{
		if (output.sound_names.empty())
		{
			return;
		}

		const auto bank = static_cast<const iw7::SndBank*>(source.find(iw7::ASSET_TYPE_SOUND_BANK, source.name() + ".all"));
		if (!bank || !bank->alias)
		{
			ZONETOOL_WARNING("iw7: the map has no sound bank %s.all; it has no sounds", source.name().data());
			return;
		}

		const asset_bank loaded(std::string(bank->zone ? bank->zone : source.name().data()) + ".sabl");
		if (!loaded.size())
		{
			ZONETOOL_WARNING("iw7: the zone folder has no %s.sabl; the map has no sounds", bank->zone ? bank->zone : source.name().data());
			return;
		}

		std::unordered_map<std::string, const iw7::SndAliasList*> lists;
		for (auto i = 0u; i < bank->aliasCount; i++)
		{
			if (bank->alias[i].aliasName)
			{
				lists.emplace(asset_name(bank->alias[i].aliasName), &bank->alias[i]);
			}
		}

		// The aliases that the scripts name, with their secondary aliases.
		std::vector<std::string> pending(output.sound_names.begin(), output.sound_names.end());
		std::set<std::string> visited;
		std::set<std::uint32_t> written;
		std::size_t aliases{};
		std::size_t samples{};
		while (!pending.empty())
		{
			const auto name = pending.back();
			pending.pop_back();
			const auto found = lists.find(name);
			if (found == lists.end() || !visited.insert(name).second)
			{
				continue;
			}

			auto variations = json::array();
			for (auto v = 0; v < found->second->count; v++)
			{
				const auto& alias = found->second->head[v];
				const auto entry = loaded.find(alias.assetId);
				if (!entry || entry->channelCount < 1 || entry->channelCount > 2)
				{
					continue;
				}

				const auto file = std::format("iw7/{:08x}.wav", alias.assetId);
				if (written.insert(alias.assetId).second)
				{
					const auto bytes = loaded.read(*entry);
					try
					{
						if (!bytes)
						{
							throw std::runtime_error("cannot be read");
						}

						const auto pcm = decode_flac(bytes->data(), bytes->size(), entry->channelCount, entry->frameCount);
						write_wav(output.extracted / "sound" / file, pcm, static_cast<unsigned>(entry->frameRate), entry->channelCount);
						samples++;
					}
					catch (const std::exception& e)
					{
						ZONETOOL_WARNING("iw7: sound %08x of alias \"%s\" does not convert: %s", alias.assetId, name.data(), e.what());
						continue;
					}
				}

				// IW3's flags: bit 0 loops, bits 8-13 the channel (1 = auto).
				const auto looping = alias.flags.looping != 0;
				json variation =
				{
					{"file", {{"name", file}}},
					{"volume", {{"min", alias.volMin}, {"max", alias.volMax}}},
					{"pitch", {{"min", alias.pitchMin}, {"max", alias.pitchMax}}},
					{"distance", {{"min", alias.distMin}, {"max", alias.distMax}}},
					{"probability", alias.probability},
					{"lfe_percentage", alias.lfePercentage},
					{"center_percentage", alias.centerPercentage},
					{"start_delay", alias.startDelay},
					{"flags", (looping ? 1u : 0u) | (1u << 8)},
					{"envelope", {{"min", alias.envelopMin}, {"max", alias.envelopMax}, {"percentage", alias.envelopPercentage}}},
				};

				if (alias.secondaryAliasName && *alias.secondaryAliasName)
				{
					const auto secondary = std::string(asset_name(alias.secondaryAliasName));
					variation["secondary_alias"] = secondary;
					pending.push_back(secondary);
				}

				variations.push_back(std::move(variation));
			}

			if (variations.empty())
			{
				continue;
			}

			write_json(output.extracted / "soundaliases" / (file_name(name) + ".iw3.json"), {{"name", name}, {"aliases", std::move(variations)}});
			aliases++;
		}

		ZONETOOL_INFO("iw7: exported %zu of %zu sound aliases that the scripts name, with %zu sounds of %s", aliases,
			output.sound_names.size(), samples, bank->name ? bank->name : "?");
	}
}
