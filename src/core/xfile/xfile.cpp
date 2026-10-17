#include <std_include.hpp>
#include "xfile.hpp"

#include <io/game_files.hpp>

#include <zlib.h>
#include <lz4.h>
#include <zstd.h>

namespace zonetool::xfile
{
	namespace
	{
		constexpr std::size_t auth_block_size = 0x4000;
		constexpr std::size_t auth_blocks_per_group = 512;

		enum class s1_compression : std::uint8_t
		{
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

		std::string strip_auth_blocks(const std::string_view data)
		{
			std::string result;
			result.reserve(data.size());

			byte_reader reader(data);
			reader.skip(std::min(auth_block_size, reader.remaining()));

			while (reader.remaining())
			{
				reader.skip(std::min(auth_block_size, reader.remaining()));

				for (std::size_t block = 0; block < auth_blocks_per_group && reader.remaining(); block++)
				{
					result.append(reader.read_view(std::min(auth_block_size, reader.remaining())));
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
			case s1_compression::zlib:
				return inflate_zlib(data, MAX_WBITS, 0);
			case s1_compression::none:
				return std::string(data);
			case s1_compression::block_container:
				return decompress_container(data);
			}

			throw std::runtime_error(std::format("unknown compression type {}", header.compressType));
		}

		std::string read_signed_data(byte_reader& reader, const bool is_signed)
		{
			return is_signed ? strip_auth_blocks(reader.rest()) : std::string(reader.rest());
		}

		void read_s1(byte_reader& reader, const format& format, fastfile& file)
		{
			XFileHeader header{};
			reader.read(&header, header_size_before_stream_files);

			file.stream_files.resize(header.imageCount);
			reader.read(file.stream_files.data(), file.stream_files.size() * sizeof(XStreamFile));
			reader.read(&header.baseFileLen, file_length_fields_size);

			const auto zone = decompress(header, read_signed_data(reader, file.is_signed));

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

			file.payload = decompress_iwc(read_signed_data(reader, file.is_signed));
		}
	}

	namespace
	{
		constexpr std::size_t iw8_block_count = 11;
		constexpr std::uint32_t iw8_xfile_version = 0xFF7;
		constexpr std::size_t iw8_signature_size = 0x8000;
		constexpr std::size_t iw8_signed_chunk_size = 0x800000;
		constexpr std::size_t iw8_rsa_block_size = 0x4000;

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
#pragma pack(pop)

		static_assert(sizeof(iw8_file_header) == 0x88);
		static_assert(sizeof(iw8_compressed_block) == 0xC);

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

		void read_iw8(byte_reader& reader, const std::filesystem::path& path, const format& format, fastfile& file)
		{
			if (format.block_count != iw8_block_count)
			{
				throw std::runtime_error(std::format("IW8 zones have {} blocks", iw8_block_count));
			}

			const auto header = reader.read<iw8_file_header>();
			if (header.xfile_version != iw8_xfile_version)
			{
				throw std::runtime_error(std::format("xfile version 0x{:X} is not 0x{:X}", header.xfile_version, iw8_xfile_version));
			}

			file.block_sizes.assign(std::begin(header.block_sizes), std::end(header.block_sizes));

			if (!file.is_signed)
			{
				file.payload = decompress_iw8_frame(path, reader.rest(), header.size);
				return;
			}

			if (reader.remaining() < iw8_signature_size || reader.read_view(8) != "IWffs100")
			{
				throw std::runtime_error("signed zone has no IWffs100 signature header");
			}

			reader.skip(iw8_signature_size - 8);
			file.payload = decompress_iw8_frame(path, strip_iw8_rsa_blocks(reader.rest()), header.size);
		}
	}

	fastfile read(const std::filesystem::path& path, const format& format)
	{
		std::string data;
		if (!game_files::read(path, data))
		{
			throw std::runtime_error("cannot read file");
		}

		struct
		{
			char magic[8];
			std::uint32_t version;
		} identity{};
		byte_reader(data).read(&identity, sizeof(identity));

		fastfile file{};
		file.magic = std::string(identity.magic, sizeof(identity.magic));
		file.version = identity.version;

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
		case container::iw8:
			read_iw8(reader, path, format, file);
			break;
		case container::unsupported:
			throw std::runtime_error(std::format("the {} container has no v2 reader", format.unsigned_magic));
		}

		return file;
	}
}
