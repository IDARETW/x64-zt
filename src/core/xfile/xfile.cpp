#include <std_include.hpp>
#include "xfile.hpp"

#include <io/game_files.hpp>
#include <utils/cryptography.hpp>
#include <utils/io.hpp>

#include <zlib.h>
#include <lz4.h>
#include <lz4hc.h>
#include <xxhash.h>
#include <zstd.h>

namespace zonetool::xfile
{
	namespace
	{
		constexpr std::size_t auth_block_size = 0x4000;
		constexpr std::size_t auth_blocks_per_group = 512;

		enum class s1_compression : std::uint8_t
		{
			iw6_zlib = 0,
			zlib = 1,
			none = 3,
			block_container = 4,
		};

		constexpr std::size_t header_size_before_stream_files = offsetof(XFileHeader, baseFileLen);
		constexpr std::size_t file_length_fields_size = sizeof(XFileHeader) - header_size_before_stream_files;

		class byte_reader
		{
		public:
			byte_reader(const std::string_view data)
				: data_(data)
			{
			}

			template <typename T>
			T read()
			{
				T value{};
				this->read(&value, sizeof(T));
				return value;
			}

			void read(void* destination, const std::size_t size)
			{
				if (this->remaining() < size)
				{
					throw std::runtime_error("unexpected end of file");
				}

				std::memcpy(destination, this->data_.data() + this->offset_, size);
				this->offset_ += size;
			}

			std::string_view read_view(const std::size_t size)
			{
				if (this->remaining() < size)
				{
					throw std::runtime_error("unexpected end of file");
				}

				const auto view = this->data_.substr(this->offset_, size);
				this->offset_ += size;
				return view;
			}

			void skip(const std::size_t size)
			{
				this->read_view(size);
			}

			std::string_view rest()
			{
				return this->read_view(this->remaining());
			}

			std::size_t remaining() const
			{
				return this->data_.size() - this->offset_;
			}

		private:
			std::string_view data_;
			std::size_t offset_{};
		};

		std::string strip_auth_blocks(const std::string_view data, const format& format)
		{
			std::string result;
			result.reserve(data.size());

			byte_reader reader(data);
			reader.skip(std::min(format.auth_block_size, reader.remaining()));

			while (reader.remaining())
			{
				reader.skip(std::min(format.auth_block_size, reader.remaining()));

				for (std::size_t block = 0; block < format.auth_blocks_per_group && reader.remaining(); block++)
				{
					result.append(reader.read_view(std::min(format.auth_block_size, reader.remaining())));
				}
			}

			return result;
		}

		std::string inflate_zlib(const std::string_view data, const int window_bits, const std::size_t size_hint)
		{
			z_stream stream{};
			if (inflateInit2(&stream, window_bits) != Z_OK)
			{
				throw std::runtime_error("inflateInit2 failed");
			}

			const auto _ = gsl::finally([&]
			{
				inflateEnd(&stream);
			});

			std::string result;
			result.resize(std::max(size_hint, data.size() * 4));

			stream.next_in = reinterpret_cast<const Bytef*>(data.data());
			stream.avail_in = static_cast<uInt>(data.size());

			while (true)
			{
				if (stream.total_out == result.size())
				{
					result.resize(result.size() * 2);
				}

				stream.next_out = reinterpret_cast<Bytef*>(result.data() + stream.total_out);
				stream.avail_out = static_cast<uInt>(std::min<std::size_t>(result.size() - stream.total_out, UINT32_MAX));

				const auto status = inflate(&stream, Z_NO_FLUSH);
				if (status == Z_STREAM_END)
				{
					break;
				}

				if (status != Z_OK && status != Z_BUF_ERROR)
				{
					throw std::runtime_error("zlib inflate failed: "s + (stream.msg ? stream.msg : "unknown error"));
				}

				if (status == Z_BUF_ERROR && stream.avail_in == 0)
				{
					throw std::runtime_error("zlib stream is truncated");
				}
			}

			result.resize(stream.total_out);
			return result;
		}

		enum block_codec : std::uint8_t
		{
			codec_zlib_size = 1,
			codec_zlib_speed = 2,
			codec_lz4hc = 3,
			codec_lz4 = 4,
			codec_none = 5,
		};

		void decompress_block(const std::uint8_t codec, const std::string_view block, char* destination, const std::size_t size)
		{
			switch (codec)
			{
			case codec_zlib_size:
			case codec_zlib_speed:
			{
				const auto inflated = inflate_zlib(block, MAX_WBITS, size);
				if (inflated.size() != size)
				{
					throw std::runtime_error("zlib block has the wrong size");
				}

				std::memcpy(destination, inflated.data(), size);
				return;
			}
			case codec_lz4hc:
			case codec_lz4:
				if (LZ4_decompress_safe(block.data(), destination, static_cast<int>(block.size()), static_cast<int>(size)) != static_cast<int>(size))
				{
					throw std::runtime_error("lz4 block failed");
				}
				return;
			case codec_none:
				if (block.size() != size)
				{
					throw std::runtime_error("uncompressed block has the wrong size");
				}

				std::memcpy(destination, block.data(), size);
				return;
			}

			throw std::runtime_error(std::format("unknown block codec {}", codec));
		}

		template <typename DecompressedSize>
		std::string decompress_blocks(byte_reader& reader, const std::uint64_t total_size, const std::uint8_t codec)
		{
			std::string result;
			result.resize(total_size);

			std::size_t written = 0;
			while (written < total_size)
			{
				const auto compressed_size = reader.read<std::uint32_t>();
				const auto decompressed_size = static_cast<std::size_t>(reader.read<DecompressedSize>());
				const auto stored_size = codec == codec_none ? decompressed_size : compressed_size;
				const auto block = reader.read_view(stored_size);
				reader.skip(((stored_size + 3) & ~std::size_t(3)) - stored_size);

				if (written + decompressed_size > total_size)
				{
					throw std::runtime_error("block container exceeds its declared size");
				}

				try
				{
					decompress_block(codec, block, result.data() + written, decompressed_size);
				}
				catch (const std::exception& e)
				{
					throw std::runtime_error(std::format("{} at output offset 0x{:X}", e.what(), written));
				}

				written += decompressed_size;
			}

			return result;
		}

		std::string decompress_container(const std::string_view data)
		{
			byte_reader reader(data);

			const auto total_size = reader.read<std::uint64_t>();
			const auto flags = reader.read<std::array<std::uint8_t, 4>>();
			return decompress_blocks<std::uint32_t>(reader, total_size, flags[3]);
		}

		std::string decompress(const XFileHeader& header, const std::string_view data)
		{
			if (!header.compress)
			{
				return std::string(data);
			}

			switch (static_cast<s1_compression>(header.compressType))
			{
			case s1_compression::iw6_zlib:
			case s1_compression::zlib:
				return inflate_zlib(data, MAX_WBITS, 0);
			case s1_compression::none:
				return std::string(data);
			case s1_compression::block_container:
				return decompress_container(data);
			}

			throw std::runtime_error(std::format("unknown compression type {}", header.compressType));
		}

		std::string read_signed_data(byte_reader& reader, const format& format, const bool is_signed)
		{
			return is_signed ? strip_auth_blocks(reader.rest(), format) : std::string(reader.rest());
		}

		void read_s1(byte_reader& reader, const format& format, fastfile& file)
		{
			XFileHeader header{};
			reader.read(&header, header_size_before_stream_files);

			file.stream_files.resize(header.imageCount);
			reader.read(file.stream_files.data(), file.stream_files.size() * sizeof(XStreamFile));
			reader.read(&header.baseFileLen, file_length_fields_size);

			const auto zone = decompress(header, read_signed_data(reader, format, file.is_signed));

			byte_reader zone_reader(zone);
			const auto size = zone_reader.read<std::uint64_t>();
			zone_reader.skip(sizeof(std::uint64_t));

			file.block_sizes.resize(format.block_count);
			zone_reader.read(file.block_sizes.data(), file.block_sizes.size() * sizeof(std::uint64_t));

			if (size != zone_reader.remaining())
			{
				throw std::runtime_error(std::format("zone size 0x{:X} does not match data size 0x{:X}", size, zone_reader.remaining()));
			}

			file.payload = zone_reader.rest();
		}

		constexpr std::size_t iw7_block_count = 10;

#pragma pack(push, 1)
		struct iw7_file_header
		{
			char magic[8];
			std::uint32_t version;
			std::uint8_t unused;
			std::uint8_t has_no_image_fastfile;
			std::uint8_t has_no_shared_fastfile;
			std::uint8_t unknown;
			std::uint32_t file_time_high;
			std::uint32_t file_time_low;
			std::uint64_t size;
			std::uint64_t unknown_sizes[2];
			std::uint64_t block_sizes[iw7_block_count];
			std::uint64_t unknown_array[8];
			std::uint32_t shared_stream_hash;
			std::uint32_t shared_stream_count;
			std::uint32_t image_stream_hash;
			std::uint32_t image_stream_count;
		};

		struct iw7_stream_file
		{
			std::uint64_t offset;
			std::uint64_t offset_end;
			std::uint16_t file_index;
			std::uint8_t is_localized;
			std::uint8_t pad[5];
		};
#pragma pack(pop)

		static_assert(sizeof(iw7_file_header) == 0xD0);
		static_assert(sizeof(iw7_stream_file) == 0x18);

		std::vector<XStreamFile> read_iw7_stream_files(byte_reader& reader, const std::size_t count)
		{
			std::vector<XStreamFile> files(count);
			for (auto& file : files)
			{
				const auto stream_file = reader.read<iw7_stream_file>();
				file.isLocalized = stream_file.is_localized;
				file.fileIndex = stream_file.file_index;
				file.offset = stream_file.offset;
				file.offsetEnd = stream_file.offset_end;
			}

			return files;
		}

		std::string decompress_iwc(const std::string_view data)
		{
			enum compressor : std::uint8_t
			{
				compressor_passthrough = 1,
				compressor_block = 2,
			};

			byte_reader reader(data);

			const auto type = reader.read<std::uint8_t>();
			if (reader.read_view(3) != "IWC")
			{
				throw std::runtime_error("compressor header has no IWC magic");
			}

			if (type == compressor_passthrough)
			{
				return std::string(reader.rest());
			}

			if (type != compressor_block)
			{
				throw std::runtime_error(std::format("unknown compressor {}", type));
			}

			const auto total_size = reader.read<std::uint64_t>();
			const auto block_size_and_codec = reader.read<std::uint32_t>();
			return decompress_blocks<std::uint64_t>(reader, total_size, static_cast<std::uint8_t>(block_size_and_codec >> 24));
		}

		void read_iw7(byte_reader& reader, const format& format, fastfile& file)
		{
			if (format.block_count != iw7_block_count)
			{
				throw std::runtime_error(std::format("IW7 zones have {} blocks", iw7_block_count));
			}

			const auto header = reader.read<iw7_file_header>();
			file.block_sizes.assign(std::begin(header.block_sizes), std::end(header.block_sizes));
			file.shared_stream_files = read_iw7_stream_files(reader, header.shared_stream_count);
			file.stream_files = read_iw7_stream_files(reader, header.image_stream_count);
			reader.skip(3 * sizeof(std::uint64_t));

			file.payload = decompress_iwc(read_signed_data(reader, format, file.is_signed));
		}

		constexpr std::size_t s2_header_version = 5;
		constexpr std::size_t s2_rsa_block_size = 256;
		constexpr std::size_t s2_aes_key_size = 24;
		constexpr std::size_t s2_aes_iv_size = 16;
		constexpr std::size_t s2_master_hash_offset = 0x150;
		constexpr std::string_view s2_master_magic = "S1ffs100";

#pragma pack(push, 1)
		struct s2_file_header
		{
			char magic[8];
			std::uint32_t header_version;
			std::uint32_t version;
			std::uint8_t compress;
			std::uint8_t compress_type;
			std::uint8_t stream_compress_type;
			std::uint8_t unknown;
			std::uint32_t unknown2;
			std::uint32_t file_time_high;
			std::uint32_t file_time_low;
			std::uint32_t stream_key_count;
		};
#pragma pack(pop)

		static_assert(sizeof(s2_file_header) == 0x24);

		std::string decrypt_s2_block(const std::string_view block, const std::string_view key, const std::string_view hash)
		{
			return utils::cryptography::aes::crypt_ctr_little_endian(block, hash.substr(0, s2_aes_iv_size), key);
		}

		std::string decrypt_s2_auth_blocks(byte_reader& reader, const format& format)
		{
			if (!format.zone_key)
			{
				throw std::runtime_error("signed zones need a zone key");
			}

			const auto rsa_key = format.zone_key();
			if (rsa_key.empty())
			{
				throw std::runtime_error("cannot find the zone key");
			}

			const auto key_data = utils::cryptography::rsa::decrypt(std::string(reader.read_view(s2_rsa_block_size)), "sha256", rsa_key);
			if (key_data.size() != s2_aes_key_size + s2_aes_iv_size)
			{
				throw std::runtime_error("cannot decrypt the zone key");
			}

			const std::string_view key(key_data.data(), s2_aes_key_size);
			const auto master = decrypt_s2_block(reader.read_view(std::min(auth_block_size, reader.remaining())), key, std::string_view(key_data).substr(s2_aes_key_size));
			if (!master.starts_with(s2_master_magic))
			{
				throw std::runtime_error("auth header has the wrong magic");
			}

			std::string result;
			result.reserve(reader.remaining());

			for (std::size_t group = 0; reader.remaining(); group++)
			{
				const auto group_hash_offset = s2_master_hash_offset + group * sizeof(DB_AuthHash);
				if (group_hash_offset + sizeof(DB_AuthHash) > master.size())
				{
					throw std::runtime_error("zone has too many auth groups");
				}

				const auto hashes = decrypt_s2_block(reader.read_view(std::min(auth_block_size, reader.remaining())), key, std::string_view(master).substr(group_hash_offset));
				for (std::size_t block = 0; block < auth_blocks_per_group && reader.remaining(); block++)
				{
					const auto data = reader.read_view(std::min(auth_block_size, reader.remaining()));
					result.append(decrypt_s2_block(data, key, std::string_view(hashes).substr(block * sizeof(DB_AuthHash))));
				}
			}

			return result;
		}

		std::uint8_t s2_block_codec(const std::uint8_t codec)
		{
			switch (codec)
			{
			case 1:
				return codec_zlib_size;
			case 2:
			case 3:
				return codec_lz4;
			}

			throw std::runtime_error(std::format("block codec {} is Oodle, which is not supported", codec));
		}

		void read_s2(byte_reader& reader, const format& format, fastfile& file)
		{
			const auto header = reader.read<s2_file_header>();
			if (header.header_version != s2_header_version)
			{
				throw std::runtime_error(std::format("header version {} is not {}", header.header_version, s2_header_version));
			}

			file.stream_keys.resize(header.stream_key_count);
			reader.read(file.stream_keys.data(), file.stream_keys.size() * sizeof(XStreamFileKey));
			file.shared_stream_keys.resize(reader.read<std::uint32_t>());
			reader.read(file.shared_stream_keys.data(), file.shared_stream_keys.size() * sizeof(XStreamFileKey));
			reader.skip(2 * sizeof(std::uint64_t));

			const auto size = reader.read<std::uint64_t>();
			reader.skip(sizeof(std::uint64_t));
			const auto block_count = reader.read<std::uint64_t>();
			if (block_count != format.block_count)
			{
				throw std::runtime_error(std::format("zone has {} blocks, not {}", block_count, format.block_count));
			}

			file.block_sizes.resize(block_count);
			reader.read(file.block_sizes.data(), file.block_sizes.size() * sizeof(std::uint64_t));

			const auto data = file.is_signed ? decrypt_s2_auth_blocks(reader, format) : std::string(reader.rest());
			byte_reader container(data);
			const auto total_size = container.read<std::uint64_t>();
			const auto flags = container.read<std::array<std::uint8_t, 4>>();
			file.payload = decompress_blocks<std::uint32_t>(container, total_size, s2_block_codec(flags[3]));

			if (size != file.payload.size())
			{
				throw std::runtime_error(std::format("zone size 0x{:X} does not match data size 0x{:X}", size, file.payload.size()));
			}
		}
	}

	std::string decompress_s2_stream(const std::string_view data)
	{
		byte_reader reader(data);
		const auto total_size = reader.read<std::uint64_t>();
		const auto flags = reader.read<std::array<std::uint8_t, 4>>();
		return decompress_blocks<std::uint32_t>(reader, total_size, s2_block_codec(flags[3]));
	}

	namespace
	{
		constexpr std::size_t iw8_block_count = 11;
		// The Replay 1.20 install also holds zones from earlier revisions, with the same asset data.
		constexpr std::array<std::uint32_t, 12> iw8_xfile_versions{0xFCD, 0xFCF, 0xFD0, 0xFD1, 0xFD5, 0xFDA, 0xFE1, 0xFE3, 0xFEE,
			0xFF3, 0xFF5, 0xFF7};
		constexpr std::size_t iw8_signature_size = 0x8000;
		constexpr std::size_t iw8_signed_chunk_size = 0x800000;
		constexpr std::size_t iw8_rsa_block_size = 0x4000;
		constexpr std::uint32_t iw8_patch_version = 6;

#pragma pack(push, 1)
		struct iw8_file_header
		{
			char magic[8];
			std::uint32_t version;
			std::uint32_t xfile_version;
			std::uint8_t compressed;
			std::uint8_t encrypted;
			std::uint8_t transient;
			std::uint8_t pad;
			std::uint32_t resident_size;
			std::uint32_t resident_hash;
			std::uint32_t always_loaded_size;
			std::uint64_t size;
			std::uint64_t preload_walk_size;
			std::uint64_t block_sizes[iw8_block_count];
		};

		struct iw8_compressed_block
		{
			std::uint32_t compressed_size;
			std::uint32_t decompressed_size;
			std::uint32_t encryption_counter;
		};

		struct iw8_patch_header
		{
			char magic[8];
			std::uint32_t version;
			std::uint8_t unknown[0x1C];
			std::uint64_t data_size;
			std::uint64_t delta_size;
			iw8_file_header base;
			iw8_file_header patched;
		};
#pragma pack(pop)

		static_assert(sizeof(iw8_file_header) == 0x88);
		static_assert(sizeof(iw8_compressed_block) == 0xC);
		static_assert(sizeof(iw8_patch_header) == 0x148);

		enum iw8_codec : std::uint8_t
		{
			iw8_codec_none = 1,
			iw8_codec_zlib_size = 2,
			iw8_codec_zlib_speed = 3,
			iw8_codec_lz4hc = 4,
			iw8_codec_lz4 = 5,
			iw8_codec_kraken_size = 6,
			iw8_codec_kraken_speed = 7,
			iw8_codec_leviathan_size = 0xC,
			iw8_codec_leviathan_speed = 0xD,
			iw8_codec_mermaid_size = 0xE,
			iw8_codec_mermaid_speed = 0xF,
			iw8_codec_selkie_size = 0x10,
			iw8_codec_selkie_speed = 0x11,
			iw8_codec_zstd_size = 0x12,
			iw8_codec_zstd_speed = 0x13,
		};

		using oodle_decompress_t = std::intptr_t(*)(const void*, std::intptr_t, void*, std::intptr_t, int, int, int, void*,
			std::intptr_t, void*, void*, void*, std::intptr_t, int);

		// Oodle is not redistributable: use the oo2core library of the game install that holds the zone.
		oodle_decompress_t load_oodle(const std::filesystem::path& zone_path)
		{
			static std::mutex mutex;
			static oodle_decompress_t decompress{};

			std::lock_guard _(mutex);
			if (decompress)
			{
				return decompress;
			}

			auto folder = std::filesystem::absolute(zone_path).parent_path();
			for (auto depth = 0; depth < 4 && !folder.empty(); depth++, folder = folder.parent_path())
			{
				std::error_code error;
				for (const auto& entry : std::filesystem::directory_iterator(folder, error))
				{
					const auto name = entry.path().filename().string();
					if (!name.starts_with("oo2core_") || !name.ends_with("_win64.dll"))
					{
						continue;
					}

					if (const auto library = LoadLibraryW(entry.path().c_str()))
					{
						decompress = reinterpret_cast<oodle_decompress_t>(GetProcAddress(library, "OodleLZ_Decompress"));
						if (decompress)
						{
							return decompress;
						}
					}
				}

				if (folder == folder.root_path())
				{
					break;
				}
			}

			throw std::runtime_error("the zone needs Oodle: no oo2core_*_win64.dll in the game folder");
		}

		void decompress_iw8_block(const std::filesystem::path& zone_path, const std::uint8_t codec, const std::string_view block,
			char* destination, const std::size_t size)
		{
			switch (codec)
			{
			case iw8_codec_none:
				if (block.size() != size)
				{
					throw std::runtime_error("uncompressed block has the wrong size");
				}

				std::memcpy(destination, block.data(), size);
				return;
			case iw8_codec_zlib_size:
			case iw8_codec_zlib_speed:
			{
				const auto inflated = inflate_zlib(block, MAX_WBITS, size);
				if (inflated.size() != size)
				{
					throw std::runtime_error("zlib block has the wrong size");
				}

				std::memcpy(destination, inflated.data(), size);
				return;
			}
			case iw8_codec_lz4hc:
			case iw8_codec_lz4:
				if (LZ4_decompress_safe(block.data(), destination, static_cast<int>(block.size()), static_cast<int>(size)) != static_cast<int>(size))
				{
					throw std::runtime_error("lz4 block failed");
				}
				return;
			case iw8_codec_zstd_size:
			case iw8_codec_zstd_speed:
				if (ZSTD_decompress(destination, size, block.data(), block.size()) != size)
				{
					throw std::runtime_error("zstd block failed");
				}
				return;
			case iw8_codec_kraken_size:
			case iw8_codec_kraken_speed:
			case iw8_codec_leviathan_size:
			case iw8_codec_leviathan_speed:
			case iw8_codec_mermaid_size:
			case iw8_codec_mermaid_speed:
			case iw8_codec_selkie_size:
			case iw8_codec_selkie_speed:
			{
				const auto decompress = load_oodle(zone_path);
				const auto written = decompress(block.data(), static_cast<std::intptr_t>(block.size()), destination,
					static_cast<std::intptr_t>(size), 1, 0, 0, nullptr, 0, nullptr, nullptr, nullptr, 0, 3);
				if (written != static_cast<std::intptr_t>(size))
				{
					throw std::runtime_error("oodle block failed");
				}
				return;
			}
			}

			throw std::runtime_error(std::format("unknown block codec {}", codec));
		}

		// Signed zones hold one RSA hash block after every 8 MiB of data, counted from the IWC frame.
		std::string strip_iw8_rsa_blocks(const std::string_view data)
		{
			std::string result;
			result.reserve(data.size());

			byte_reader reader(data);
			while (reader.remaining())
			{
				const auto chunk = std::min(iw8_signed_chunk_size, reader.remaining());
				result.append(reader.read_view(chunk));
				if (chunk == iw8_signed_chunk_size && reader.remaining())
				{
					reader.skip(std::min(iw8_rsa_block_size, reader.remaining()));
				}
			}

			return result;
		}

		std::string decompress_iw8_frame(const std::filesystem::path& zone_path, const std::string_view data, const std::uint64_t size)
		{
			byte_reader reader(data);

			const auto type = reader.read<std::uint8_t>();
			if (reader.read_view(3) != "IWC")
			{
				throw std::runtime_error("compressor header has no IWC magic");
			}

			if (type == 1)
			{
				if (reader.remaining() != size)
				{
					throw std::runtime_error(std::format("stored zone holds 0x{:X} bytes, not 0x{:X}", reader.remaining(), size));
				}

				return std::string(reader.rest());
			}

			if (type != 2)
			{
				throw std::runtime_error(std::format("unknown compressor {}", type));
			}

			reader.skip(sizeof(std::uint32_t) + 3);
			const auto codec = reader.read<std::uint8_t>();

			std::string result;
			result.resize(size);

			std::size_t written = 0;
			while (written < size)
			{
				const auto block = reader.read<iw8_compressed_block>();
				if (!block.compressed_size || written + block.decompressed_size > size)
				{
					throw std::runtime_error(std::format("compressed block at output offset 0x{:X} is not valid", written));
				}

				const auto stored = reader.read_view(block.compressed_size);
				reader.skip(std::min(((block.compressed_size + 3) & ~std::size_t(3)) - block.compressed_size, reader.remaining()));

				try
				{
					decompress_iw8_block(zone_path, codec, stored, result.data() + written, block.decompressed_size);
				}
				catch (const std::exception& e)
				{
					throw std::runtime_error(std::format("{} at output offset 0x{:X}", e.what(), written));
				}

				written += block.decompressed_size;
			}

			return result;
		}

		std::size_t read_vcdiff_integer(byte_reader& reader)
		{
			std::size_t value{};
			std::uint8_t byte{};
			do
			{
				if (value >> 57)
				{
					throw std::runtime_error("VCDIFF integer is too large");
				}

				byte = reader.read<std::uint8_t>();
				value = (value << 7) | (byte & 0x7F);
			} while (byte & 0x80);

			return value;
		}

		enum class vcdiff_op : std::uint8_t
		{
			none,
			add,
			run,
			copy,
		};

		struct vcdiff_instruction
		{
			vcdiff_op op;
			std::uint8_t mode;
			std::uint8_t size;
		};

		using vcdiff_code = std::array<vcdiff_instruction, 2>;

		// The default code table of RFC 3284 section 5.6.
		constexpr auto vcdiff_code_table = []
		{
			std::array<vcdiff_code, 256> table{};
			std::size_t code{};
			table[code++] = {{{vcdiff_op::run, 0, 0}}};
			for (std::uint8_t size = 0; size <= 17; size++)
			{
				table[code++] = {{{vcdiff_op::add, 0, size}}};
			}

			for (std::uint8_t mode = 0; mode <= 8; mode++)
			{
				table[code++] = {{{vcdiff_op::copy, mode, 0}}};
				for (std::uint8_t size = 4; size <= 18; size++)
				{
					table[code++] = {{{vcdiff_op::copy, mode, size}}};
				}
			}

			for (std::uint8_t mode = 0; mode <= 5; mode++)
			{
				for (std::uint8_t add_size = 1; add_size <= 4; add_size++)
				{
					for (std::uint8_t copy_size = 4; copy_size <= 6; copy_size++)
					{
						table[code++] = {{{vcdiff_op::add, 0, add_size}, {vcdiff_op::copy, mode, copy_size}}};
					}
				}
			}

			for (std::uint8_t mode = 6; mode <= 8; mode++)
			{
				for (std::uint8_t add_size = 1; add_size <= 4; add_size++)
				{
					table[code++] = {{{vcdiff_op::add, 0, add_size}, {vcdiff_op::copy, mode, 4}}};
				}
			}

			for (std::uint8_t mode = 0; mode <= 8; mode++)
			{
				table[code++] = {{{vcdiff_op::copy, mode, 4}, {vcdiff_op::add, 0, 1}}};
			}

			return table;
		}();

		class vcdiff_address_cache
		{
		public:
			std::size_t decode(byte_reader& addresses, const std::size_t here, const std::uint8_t mode)
			{
				std::size_t address{};
				if (mode == 0)
				{
					address = read_vcdiff_integer(addresses);
				}
				else if (mode == 1)
				{
					address = here - read_vcdiff_integer(addresses);
				}
				else if (mode < 2 + near_size)
				{
					address = this->near_[mode - 2] + read_vcdiff_integer(addresses);
				}
				else
				{
					address = this->same_[(mode - 2 - near_size) * 256 + addresses.read<std::uint8_t>()];
				}

				this->near_[this->next_near_] = address;
				this->next_near_ = (this->next_near_ + 1) % near_size;
				this->same_[address % this->same_.size()] = address;
				return address;
			}

		private:
			static constexpr std::size_t near_size = 4;

			std::array<std::size_t, near_size> near_{};
			std::array<std::size_t, 3 * 256> same_{};
			std::size_t next_near_{};
		};

		void copy_vcdiff(std::string& target, const std::size_t window_start, const std::string_view segment,
			std::size_t address, std::size_t count)
		{
			if (address >= segment.size() + target.size() - window_start)
			{
				throw std::runtime_error("VCDIFF copy reads data that is not decoded");
			}

			if (address < segment.size())
			{
				const auto part = std::min(count, segment.size() - address);
				target.append(segment.substr(address, part));
				address += part;
				count -= part;
				if (!count)
				{
					return;
				}
			}

			const auto position = window_start + address - segment.size();
			const auto end = target.size();
			if (position + count <= end)
			{
				target.resize(end + count);
				std::memcpy(target.data() + end, target.data() + position, count);
				return;
			}

			for (std::size_t i = 0; i < count; i++)
			{
				target.push_back(target[position + i]);
			}
		}

		// Replay patches are VCDIFF deltas (RFC 3284) with two window indicator bits added. Bit 2 adds an integer
		// after the source segment position. Bit 3 adds a checksum after the window: XXH64 of the target window,
		// seeded with the previous checksum and folded to 32 bits.
		std::string decode_vcdiff(const std::string_view source, const std::string_view delta, const std::size_t size)
		{
			byte_reader reader(delta);
			const auto header = reader.read_view(5);
			if (header.substr(0, 3) != "\xD6\xC3\xC4" || (header[4] & 0xF3) != 0)
			{
				throw std::runtime_error("patch delta has no VCDIFF header");
			}

			std::string target;
			target.reserve(size);

			std::uint32_t checksum{};
			while (reader.remaining())
			{
				const auto indicator = reader.read<std::uint8_t>();
				if (indicator & 0xF2)
				{
					throw std::runtime_error(std::format("VCDIFF window indicator 0x{:X} is not supported", indicator));
				}

				std::string_view segment;
				if (indicator & 1)
				{
					const auto segment_size = read_vcdiff_integer(reader);
					const auto segment_position = read_vcdiff_integer(reader);
					if (indicator & 4)
					{
						read_vcdiff_integer(reader);
					}

					if (segment_position > source.size() || segment_size > source.size() - segment_position)
					{
						throw std::runtime_error("VCDIFF source segment is outside the zone");
					}

					segment = source.substr(segment_position, segment_size);
				}

				byte_reader window(reader.read_view(read_vcdiff_integer(reader)));
				const auto window_size = read_vcdiff_integer(window);
				if (window.read<std::uint8_t>() != 0)
				{
					throw std::runtime_error("compressed VCDIFF sections are not supported");
				}

				const auto data_size = read_vcdiff_integer(window);
				const auto instructions_size = read_vcdiff_integer(window);
				const auto addresses_size = read_vcdiff_integer(window);
				byte_reader data(window.read_view(data_size));
				byte_reader instructions(window.read_view(instructions_size));
				byte_reader addresses(window.read_view(addresses_size));

				const auto window_start = target.size();
				vcdiff_address_cache cache;
				while (instructions.remaining())
				{
					for (const auto& instruction : vcdiff_code_table[instructions.read<std::uint8_t>()])
					{
						if (instruction.op == vcdiff_op::none)
						{
							continue;
						}

						const auto count = instruction.size ? instruction.size : read_vcdiff_integer(instructions);
						switch (instruction.op)
						{
						case vcdiff_op::add:
							target.append(data.read_view(count));
							break;
						case vcdiff_op::run:
							target.append(count, data.read<char>());
							break;
						case vcdiff_op::copy:
						{
							const auto here = segment.size() + target.size() - window_start;
							copy_vcdiff(target, window_start, segment, cache.decode(addresses, here, instruction.mode), count);
							break;
						}
						case vcdiff_op::none:
							break;
						}
					}
				}

				if (target.size() - window_start != window_size || window.remaining() || data.remaining() || addresses.remaining())
				{
					throw std::runtime_error(std::format("VCDIFF window at target offset 0x{:X} is not valid", window_start));
				}

				if (indicator & 8)
				{
					const auto expected = reader.read<std::uint32_t>();
					const auto hash = XXH64(target.data() + window_start, window_size, checksum);
					if (static_cast<std::uint32_t>(hash ^ (hash >> 32)) != expected)
					{
						throw std::runtime_error(std::format("VCDIFF window at target offset 0x{:X} has a bad checksum", window_start));
					}

					checksum = expected;
				}
			}

			return target;
		}

		std::string apply_iw8_patch(const std::filesystem::path& zone_path, iw8_file_header& header, std::string payload)
		{
			auto patch_path = zone_path;
			patch_path.replace_extension(".fp");

			std::string data;
			if (!game_files::read(patch_path, data))
			{
				return payload;
			}

			byte_reader reader(data);
			const auto patch = reader.read<iw8_patch_header>();
			if (std::string_view(patch.magic, sizeof(patch.magic)) != "IWffd100" || patch.version != iw8_patch_version)
			{
				throw std::runtime_error(std::format("{} is not a Replay patch", patch_path.filename().string()));
			}

			if (std::memcmp(&patch.base, &header, sizeof(header)))
			{
				throw std::runtime_error(std::format("{} patches another version of the zone", patch_path.filename().string()));
			}

			header = patch.patched;
			if (!reader.remaining())
			{
				if (header.size != payload.size())
				{
					throw std::runtime_error("patch without a delta changes the zone size");
				}

				return payload;
			}

			auto frame = reader.read_view(patch.data_size);
			std::string unsigned_frame;
			if (frame.starts_with("IWffs100"))
			{
				if (frame.size() < iw8_signature_size)
				{
					throw std::runtime_error("signed patch has no signature header");
				}

				unsigned_frame = strip_iw8_rsa_blocks(frame.substr(iw8_signature_size));
				frame = unsigned_frame;
			}

			const auto delta = decompress_iw8_frame(patch_path, frame, patch.delta_size);
			auto patched = decode_vcdiff(payload, delta, header.size);
			if (patched.size() != header.size)
			{
				throw std::runtime_error(std::format("patched zone holds 0x{:X} bytes, not 0x{:X}", patched.size(), header.size));
			}

			return patched;
		}

		void read_iw8(byte_reader& reader, std::string& data, const std::filesystem::path& path, const format& format,
			fastfile& file)
		{
			if (format.block_count != iw8_block_count)
			{
				throw std::runtime_error(std::format("IW8 zones have {} blocks", iw8_block_count));
			}

			auto header = reader.read<iw8_file_header>();

			std::string payload;
			if (!file.is_signed)
			{
				const auto rest = reader.rest();
				if (rest.size() >= 4 && rest[0] == 1 && rest.substr(1, 3) == "IWC")
				{
					// A stored payload is the rest of the file: it moves out of the data instead of being copied.
					const auto offset = static_cast<std::size_t>(rest.data() - data.data()) + 4;
					if (data.size() - offset != header.size)
					{
						throw std::runtime_error(std::format("stored zone holds 0x{:X} bytes, not 0x{:X}", data.size() - offset, header.size));
					}

					data.erase(0, offset);
					payload = std::move(data);
				}
				else
				{
					payload = decompress_iw8_frame(path, rest, header.size);
				}
			}
			else
			{
				if (reader.remaining() < iw8_signature_size || reader.read_view(8) != "IWffs100")
				{
					throw std::runtime_error("signed zone has no IWffs100 signature header");
				}

				reader.skip(iw8_signature_size - 8);
				payload = decompress_iw8_frame(path, strip_iw8_rsa_blocks(reader.rest()), header.size);
			}

			file.payload = apply_iw8_patch(path, header, std::move(payload));
			if (std::ranges::find(iw8_xfile_versions, header.xfile_version) == iw8_xfile_versions.end())
			{
				throw std::runtime_error(std::format("xfile version 0x{:X} is not a Replay 1.20 version", header.xfile_version));
			}

			file.block_sizes.assign(std::begin(header.block_sizes), std::end(header.block_sizes));
			file.xfile_version = header.xfile_version;
			file.transient = header.transient != 0;
			file.preload_walk_size = header.preload_walk_size;
		}

		// The header and the stored frame that come before the payload.
		std::string write_iw8_header(const format& format, const fastfile& file)
		{
			if (format.block_count != iw8_block_count || file.block_sizes.size() != iw8_block_count)
			{
				throw std::runtime_error(std::format("IW8 zones have {} blocks", iw8_block_count));
			}

			// A stored frame: the compressor type 1 and "IWC", then the payload.
			constexpr std::string_view stored_frame = "\x01IWC";
			if (file.payload.size() > std::numeric_limits<std::uint32_t>::max() - stored_frame.size())
			{
				throw std::runtime_error("the zone is too large for an IW8 fastfile");
			}

			iw8_file_header header{};
			std::memcpy(header.magic, format.unsigned_magic.data(), sizeof(header.magic));
			header.version = format.version;
			header.xfile_version = file.xfile_version ? file.xfile_version : iw8_xfile_versions.back();
			header.transient = file.transient ? 1 : 0;
			header.resident_size = static_cast<std::uint32_t>(stored_frame.size() + file.payload.size());
			header.always_loaded_size = static_cast<std::uint32_t>(file.payload.size());
			header.size = file.payload.size();
			header.preload_walk_size = file.preload_walk_size;
			std::ranges::copy(file.block_sizes, std::begin(header.block_sizes));

			std::string data;
			data.append(reinterpret_cast<const char*>(&header), sizeof(header));
			data.append(stored_frame);
			return data;
		}
	}

	void write(const std::filesystem::path& path, const format& format, const fastfile& file)
	{
		if (format.container != container::iw8)
		{
			throw std::runtime_error(std::format("the {} container has no v2 writer", format.unsigned_magic));
		}

		const auto header = write_iw8_header(format, file);
		if (path.has_parent_path())
		{
			std::filesystem::create_directories(path.parent_path());
		}

		// Write beside the destination and replace it after the complete file is on disk.
		auto partial = path;
		partial += ".partial";
		{
			std::ofstream stream(partial, std::ios::binary | std::ios::trunc);
			stream.write(header.data(), static_cast<std::streamsize>(header.size()));
			stream.write(file.payload.data(), static_cast<std::streamsize>(file.payload.size()));
			stream.close();
			if (!stream)
			{
				// The stream gives no cause; errno holds the C runtime's (ENOSPC on a full disk). A 2.2 GB write in one call
				// works, so the size alone does not fail it (Scorch's 2+ GiB zone, 2026-10-21).
				const auto cause = std::generic_category().message(errno);
				std::error_code ignored;
				std::filesystem::remove(partial, ignored);
				const auto space = std::filesystem::space(partial.parent_path(), ignored);
				throw std::runtime_error(std::format("cannot write {} ({} bytes: {}; {} MB free)", partial.string(),
					header.size() + file.payload.size(), cause, ignored ? 0 : space.available >> 20));
			}
		}

		std::filesystem::rename(partial, path);
	}

	std::string read_iw7_shared_stream(const std::string_view data, const format& format)
	{
		return decompress_iwc(strip_auth_blocks(data, format));
	}

	fastfile read(const std::filesystem::path& path, const format& format)
	{
		std::string data;
		if (!game_files::read(path, data))
		{
			throw std::runtime_error("cannot read file");
		}

		return read(path, std::move(data), format);
	}

	fastfile read(const std::filesystem::path& path, std::string data, const format& format)
	{
		struct
		{
			char magic[8];
			std::uint32_t version;
			std::uint32_t s2_version;
		} identity{};
		byte_reader(data).read(&identity, sizeof(identity));

		fastfile file{};
		file.magic = std::string(identity.magic, sizeof(identity.magic));
		file.version = format.container == container::s2 ? identity.s2_version : identity.version;

		if (file.magic == format.signed_magic)
		{
			file.is_signed = true;
		}
		else if (file.magic != format.unsigned_magic)
		{
			throw std::runtime_error(std::format("unknown magic \"{}\"", file.magic));
		}

		if (file.version != format.version)
		{
			throw std::runtime_error(std::format("version {} is not {}", file.version, format.version));
		}

		byte_reader reader(data);
		switch (format.container)
		{
		case container::s1:
			read_s1(reader, format, file);
			break;
		case container::iw7:
			read_iw7(reader, format, file);
			break;
		case container::s2:
			read_s2(reader, format, file);
			break;
		case container::iw8:
			read_iw8(reader, data, path, format, file);
			break;
		case container::unsupported:
			throw std::runtime_error(std::format("the {} container has no v2 reader", format.unsigned_magic));
		}

		return file;
	}

	namespace
	{
		constexpr std::size_t s2_block_size = 0x10000;
		constexpr std::uint8_t s2_write_codec = 3;
		constexpr int s2_lz4_level = 9;
		constexpr std::uintmax_t s2_stream_batch_size = 0x10000000;

		template <typename T>
		void append(std::string& output, const T& value)
		{
			output.append(reinterpret_cast<const char*>(&value), sizeof(T));
		}

		void append_stream_keys(std::string& output, const std::vector<XStreamFileKey>& keys)
		{
			output.append(reinterpret_cast<const char*>(keys.data()), keys.size() * sizeof(XStreamFileKey));
		}

		template <typename Work>
		void run_parallel(const std::size_t count, const Work& work)
		{
			const auto worker_count = std::max(1u, std::min<unsigned>(std::thread::hardware_concurrency(), static_cast<unsigned>(count)));
			std::atomic_size_t next{};

			const auto run = [&]
			{
				for (auto index = next++; index < count; index = next++)
				{
					work(index);
				}
			};

			std::vector<std::thread> workers;
			for (auto i = 1u; i < worker_count; i++)
			{
				workers.emplace_back(run);
			}

			run();
			for (auto& worker : workers)
			{
				worker.join();
			}
		}

		std::size_t s2_block_count(const std::string_view data)
		{
			return (data.size() + s2_block_size - 1) / s2_block_size;
		}

		std::string compress_s2_block(const std::string_view data, const std::size_t index)
		{
			const auto source = data.substr(index * s2_block_size, s2_block_size);
			std::string block(LZ4_compressBound(static_cast<int>(source.size())), '\0');

			const auto size = LZ4_compress_HC(source.data(), block.data(), static_cast<int>(source.size()),
				static_cast<int>(block.size()), s2_lz4_level);
			if (size <= 0)
			{
				throw std::runtime_error("lz4 compression failed");
			}

			block.resize(size);
			return block;
		}

		std::string make_s2_container(const std::string_view data, const std::vector<std::string>& blocks)
		{
			std::string container;
			append(container, static_cast<std::uint64_t>(data.size()));
			container.append({0, 0, 1, static_cast<char>(s2_write_codec)});
			for (std::size_t i = 0; i < blocks.size(); i++)
			{
				const auto decompressed_size = std::min(s2_block_size, data.size() - i * s2_block_size);
				append(container, static_cast<std::uint32_t>(blocks[i].size()));
				append(container, static_cast<std::uint32_t>(decompressed_size));
				container.append(blocks[i]);
				container.append(((blocks[i].size() + 3) & ~std::size_t(3)) - blocks[i].size(), '\0');
			}

			return container;
		}

		std::string compress_s2_container(const std::string_view data)
		{
			std::vector<std::string> blocks(s2_block_count(data));
			run_parallel(blocks.size(), [&](const std::size_t index)
			{
				blocks[index] = compress_s2_block(data, index);
			});

			return make_s2_container(data, blocks);
		}

		std::string compress_s2_stream(const std::string_view data)
		{
			std::vector<std::string> blocks(s2_block_count(data));
			for (std::size_t i = 0; i < blocks.size(); i++)
			{
				blocks[i] = compress_s2_block(data, i);
			}

			return make_s2_container(data, blocks);
		}

		std::string make_s2_zone(const format& format, const fastfile& file, const std::string_view payload)
		{
			const auto container = compress_s2_container(payload);

			s2_file_header header{};
			std::memcpy(header.magic, format.unsigned_magic.data(), sizeof(header.magic));
			header.header_version = s2_header_version;
			header.version = format.version;
			header.compress = 1;
			header.compress_type = 2;
			header.stream_compress_type = 2;
			header.unknown = 0;
			header.unknown2 = static_cast<std::uint32_t>(s2_block_size);
			header.stream_key_count = static_cast<std::uint32_t>(file.stream_keys.size());

			std::string output;
			append(output, header);
			append_stream_keys(output, file.stream_keys);
			append(output, static_cast<std::uint32_t>(file.shared_stream_keys.size()));
			append_stream_keys(output, file.shared_stream_keys);

			const auto file_size = output.size() + 2 * sizeof(std::uint64_t) + 3 * sizeof(std::uint64_t)
				+ format.block_count * sizeof(std::uint64_t) + container.size();
			append(output, static_cast<std::uint64_t>(file_size));
			append(output, static_cast<std::uint64_t>(file_size));
			append(output, static_cast<std::uint64_t>(payload.size()));
			append(output, std::uint64_t{});
			append(output, static_cast<std::uint64_t>(format.block_count));
			for (std::size_t i = 0; i < format.block_count; i++)
			{
				append(output, i < file.block_sizes.size() ? file.block_sizes[i] : std::uint64_t{});
			}

			output.append(container);
			return output;
		}

		std::size_t next_stream_batch(const std::span<const std::filesystem::path> streams, std::size_t index)
		{
			std::uintmax_t size{};
			while (index < streams.size() && size < s2_stream_batch_size)
			{
				size += std::filesystem::file_size(streams[index++]);
			}

			return index;
		}

		std::vector<std::string> read_stream_batch(const std::span<const std::filesystem::path> streams)
		{
			std::vector<std::string> data(streams.size());
			for (std::size_t i = 0; i < streams.size(); i++)
			{
				if (!utils::io::read_file(streams[i].string(), &data[i]))
				{
					throw std::runtime_error(std::format("cannot read \"{}\"", streams[i].string()));
				}
			}

			return data;
		}
	}

	void write_s2(const std::filesystem::path& path, const format& format, const fastfile& file, const std::string_view payload)
	{
		if (format.container != container::s2)
		{
			throw std::runtime_error("write_s2 needs the s2 container");
		}

		if (file.is_signed)
		{
			throw std::runtime_error("signed s2 zones are not supported");
		}

		std::filesystem::create_directories(path.parent_path());
		if (!utils::io::write_file(path.string(), make_s2_zone(format, file, payload)))
		{
			throw std::runtime_error(std::format("cannot write \"{}\"", path.string()));
		}
	}

	std::vector<stream_location> write_s2_pak(const std::filesystem::path& path, const std::string_view header,
		const std::span<const std::filesystem::path> streams)
	{
		std::filesystem::create_directories(path.parent_path());
		std::ofstream output(path, std::ios::binary | std::ios::trunc);
		output.write(header.data(), header.size());

		std::vector<stream_location> locations;
		locations.reserve(streams.size());

		std::uint64_t offset = header.size();
		for (std::size_t first = 0; first < streams.size();)
		{
			const auto batch = streams.subspan(first, next_stream_batch(streams, first) - first);
			const auto data = read_stream_batch(batch);

			std::vector<std::string> entries(batch.size());
			run_parallel(entries.size(), [&](const std::size_t index)
			{
				entries[index] = compress_s2_stream(data[index]);
			});

			for (const auto& entry : entries)
			{
				locations.emplace_back(offset, static_cast<std::uint32_t>(entry.size()));
				output.write(entry.data(), entry.size());
				offset += entry.size();
			}

			first += batch.size();
		}

		if (!output)
		{
			throw std::runtime_error(std::format("cannot write \"{}\"", path.string()));
		}

		return locations;
	}
}
