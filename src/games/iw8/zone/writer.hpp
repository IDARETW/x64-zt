#pragma once

namespace zonetool::iw8
{
	struct loaded_zone;

	struct zone_asset
	{
		std::int32_t type;
		void* header;
		bool referenced;
		// For an entry of the asset list: the marker to write (-1, -2 or -3), or 0 for the default.
		std::uint64_t marker{};
	};

	struct write_options
	{
		// Insert (-3) the assets of the list, as iw8-zonetool does. Stock zones follow (-2) them.
		bool insert_list_assets{};
		// The markers of source fields to keep, for a zone that is written again (load_options::keep_markers).
		const pointer_markers* markers{};
		// The markers of the list header pointers, 0 for the follow marker.
		std::uint64_t strings_marker{};
		std::uint64_t assets_marker{};
		// The expected size of the zone data, which the writer reserves (0: none).
		std::size_t expected_size{};
	};

	struct written_zone
	{
		std::string data;
		std::vector<std::uint64_t> block_sizes;
		std::size_t preload_walk_size{};
		// The data offset where each asset starts, with its type and name.
		std::vector<std::tuple<std::size_t, std::int32_t, std::string>> asset_offsets;
	};

	// Writes a zone in the Replay 1.20 stream layout: the script strings, the asset list, and each asset at its
	// first use. A later use points at the insert slot of the asset. A referenced asset gets a name stub.
	// `assets` holds the data of the assets that the zone holds; a use of any other asset is a reference.
	// `script_strings` is the start of the zone string list (nullptr for a null entry), for example the list of
	// a zone that is written again. Other strings follow in the order of use.
	written_zone write_zone(std::span<const zone_asset> list, std::span<const zone_asset> assets,
		std::span<const char* const> script_strings, const write_options& options = {});

	// Writes the assets and script strings of a loaded zone again with the choices of this writer. The list entries
	// follow (-2), as in stock zones. The zone must load with keep_temp_data.
	written_zone write_zone(const loaded_zone& zone, const write_options& options = {});

	// Writes zone data that another writer made with the choices of this writer, for example a zone of the IW3
	// converter: the zone loads, keeping the data that the game drops, and is written again. The result keeps the xfile
	// version and the transient flag of `file`, and at least its preload walk size.
	xfile::fastfile rewrite_fastfile(const std::string& name, xfile::fastfile file);
}
