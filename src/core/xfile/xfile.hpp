#pragma once

#include "structs.hpp"

namespace zonetool::xfile
{
	enum class container
	{
		s1,
		iw7,
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
	};

	struct fastfile
	{
		std::string magic;
		std::uint32_t version;
		bool is_signed;
		std::vector<std::uint64_t> block_sizes;
		std::vector<XStreamFile> stream_files;
		std::vector<XStreamFile> shared_stream_files;
		std::string payload;
		// IW8: the xfile revision, the transient flag and the capacity of the map preload plan.
		std::uint32_t xfile_version{};
		bool transient{};
		std::uint64_t preload_walk_size{};
	};

	fastfile read(const std::filesystem::path& path, const format& format);
	// Writes an unsigned zone with the payload stored (not compressed).
	void write(const std::filesystem::path& path, const format& format, const fastfile& file);
}
