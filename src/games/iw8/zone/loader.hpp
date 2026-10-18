#pragma once

namespace zonetool::iw8
{
	struct loaded_asset
	{
		std::int32_t type;
		void* header;
		bool referenced;
	};

	struct loaded_zone
	{
		std::string name;
		xfile::fastfile file;
		std::unique_ptr<zone_reader> reader;
		std::vector<loaded_asset> assets;
		std::size_t script_string_count{};
	};

	const char* get_asset_name(std::int32_t type, const void* header);

	std::unique_ptr<loaded_zone> load_zone(const std::filesystem::path& path);
}
