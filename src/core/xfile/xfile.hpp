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
	// apply_patch = false returns the zone's own payload, before the Replay patch (.fp) beside it. A patch is built
	// from that base payload, so patchzone reads both of its zones this way.
	fastfile read(const std::filesystem::path& path, const format& format, bool apply_patch);
	// The same from the file's data, already read.
	fastfile read(const std::filesystem::path& path, std::string data, const format& format, bool apply_patch = true);
	// Writes an unsigned zone. An IW8 payload is compressed with Oodle Kraken in Replay's IWC frame when an Oodle library
	// is set (set_oodle_directory), as stock fastfiles are; it is stored without one, or with MW120R_FF_STORED=1.
	void write(const std::filesystem::path& path, const format& format, const fastfile& file);
	// The game install whose oo2core_*_win64.dll compresses IW8 payloads (Oodle is not redistributable).
	void set_oodle_directory(const std::filesystem::path& directory);
	// Replay's compressed IWC frame of an IW8 payload; empty when no Oodle compressor is available or MW120R_FF_STORED=1.
	std::string compress_iw8_frame(std::string_view payload);

	// Reads one stream of an IW7 shared stream file (assetfile<N>.pak): signed blocks that hold a compressed payload.
	std::string read_iw7_shared_stream(std::string_view data, const format& format);

	void write_s2(const std::filesystem::path& path, const format& format, const fastfile& file, std::string_view payload);

	// compresses every stream like the fastfile streams and writes them after header, in order
	std::vector<stream_location> write_s2_pak(const std::filesystem::path& path, std::string_view header,
		std::span<const std::filesystem::path> streams);

	std::string decompress_s2_stream(std::string_view data);
}
