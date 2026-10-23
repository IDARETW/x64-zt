#pragma once

namespace zonetool::iw8
{
	struct xasset_entry
	{
		std::uint32_t type;
		std::uint32_t pad;
		void* header;
	};

	struct xasset_list
	{
		std::uint32_t string_count;
		std::uint32_t strings_loaded;
		const char** strings;
		std::uint32_t asset_count;
		std::uint32_t assets_loaded;
		xasset_entry* assets;
	};

	static_assert(sizeof(xasset_entry) == 16);
	static_assert(sizeof(xasset_list) == 32);

	struct loaded_asset
	{
		std::int32_t type;
		void* header;
		bool referenced;
		// For an entry of the asset list: its marker in the zone (-1, -2 or -3), or 0 for an alias.
		std::uint64_t marker{};
	};

	struct loaded_zone
	{
		std::string name;
		xfile::fastfile file;
		std::unique_ptr<zone_reader> reader;
		// Every asset that the zone holds, in load order: an asset that another asset uses comes first.
		std::vector<loaded_asset> assets;
		// The asset list of the zone, in its order.
		std::vector<loaded_asset> list;
		// The script string list of the zone, with nullptr for its null entries.
		std::vector<const char*> script_strings;
		// The markers of the string list and asset list pointers in the list header.
		std::uint64_t strings_marker{};
		std::uint64_t assets_marker{};
		// The markers that a writer keeps, when the zone loads with keep_markers.
		pointer_markers markers;
	};

	struct load_options
	{
		// Keep the data that the game drops after each asset, for dumps and writers.
		bool keep_temp_data{};
		// Keep the pointer markers, to write the zone again as it is.
		bool keep_markers{};
	};

	const zone_layout& replay_layout();
	const char* get_asset_name(std::int32_t type, const void* header);

	std::unique_ptr<loaded_zone> load_zone(const std::filesystem::path& path, const load_options& options = {});
	// Loads zone data that is in memory, for example a zone that a writer made.
	std::unique_ptr<loaded_zone> load_zone(const std::string& name, xfile::fastfile file, const load_options& options = {});
	// Loads a zone file and reads each asset. Returns the type of each entry of the asset list.
	std::vector<std::int32_t> zone_list_types(const std::filesystem::path& path);
	// The same from the zone file's data, already read.
	std::vector<std::int32_t> zone_list_types(const std::filesystem::path& path, std::string data);
}
