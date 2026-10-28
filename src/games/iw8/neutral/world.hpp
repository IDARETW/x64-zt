#pragma once

// Emitted by tools/gen_iw8_neutral.py from tools/neutral_families/world.json: do not edit.
#include <core/assets/world_data.hpp>
#include "resolver.hpp"
#include "../structs.hpp"

#include <expected>
#include <string>

namespace zonetool
{
	class zone_memory;
}

// Replay 1.20 world assets to and from neutral data (docs/NEUTRAL_ASSETS.md).
namespace zonetool::iw8::neutral
{
	std::expected<asset_data::com_world, std::string> to_neutral(const ComWorld& native);
	ComWorld* from_neutral(const asset_data::com_world& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::com_world& data);

	std::expected<asset_data::glass_world, std::string> to_neutral(const GlassWorld& native);
	GlassWorld* from_neutral(const asset_data::glass_world& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::glass_world& data);

	std::expected<asset_data::map_edge_list, std::string> to_neutral(const MapEdgeList& native);
	MapEdgeList* from_neutral(const asset_data::map_edge_list& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::map_edge_list& data);

	std::expected<asset_data::nav_mesh, std::string> to_neutral(const NavMeshData& native);
	NavMeshData* from_neutral(const asset_data::nav_mesh& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::nav_mesh& data);

	std::expected<asset_data::clip_map, std::string> to_neutral(const clipMap_t& native);
	clipMap_t* from_neutral(const asset_data::clip_map& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::clip_map& data);

	std::expected<asset_data::fog_spline, std::string> to_neutral(const GfxFogSpline& native);
	GfxFogSpline* from_neutral(const asset_data::fog_spline& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::fog_spline& data);

}
