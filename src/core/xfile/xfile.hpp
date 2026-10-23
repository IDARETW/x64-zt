#pragma once

#include "structs.hpp"

namespace zonetool::xfile
{
	enum class container
	{
		s1,
		iw7,
		s2,
		iw8,
		unsupported,
	};

	struct format
	{
		container container;
		std::string_view unsigned_magic;
		std::string_view signed_magic;
		std::uint32_t version;
		std::size_t block_count;
		std::string (*zone_key)();
		std::size_t auth_block_size = 0x4000;
		std::size_t auth_blocks_per_group = 512;
	};

	struct fastfile
	{
		std::string magic;
		std::uint32_t version;
		bool is_signed;
		std::vector<std::uint64_t> block_sizes;
		std::vector<XStreamFile> stream_files;
		std::vector<XStreamFile> shared_stream_files;
		std::vector<XStreamFileKey> stream_keys;
		std::vector<XStreamFileKey> shared_stream_keys;
		std::string payload;
		// IW8: the xfile revision, the transient flag and the capacity of the map preload plan.
		std::uint32_t xfile_version{};
		bool transient{};
		std::uint64_t preload_walk_size{};
	};

	struct stream_location
	{
		std::uint64_t offset;
		std::uint32_t size;
	};

	fastfile read(const std::filesystem::path& path, const format& format);
	// The same from the file's data, already read.
	fastfile read(const std::filesystem::path& path, std::string data, const format& format);
	// Writes an unsigned zone with the payload stored (not compressed).
	void write(const std::filesystem::path& path, const format& format, const fastfile& file);

	// Reads one stream of an IW7 shared stream file (assetfile<N>.pak): signed blocks that hold a compressed payload.
	std::string read_iw7_shared_stream(std::string_view data, const format& format);

	void write_s2(const std::filesystem::path& path, const format& format, const fastfile& file, std::string_view payload);

	// compresses every stream like the fastfile streams and writes them after header, in order
	std::vector<stream_location> write_s2_pak(const std::filesystem::path& path, std::string_view header,
		std::span<const std::filesystem::path> streams);

	std::string decompress_s2_stream(std::string_view data);
}
