#pragma once

#include <core/assets/impact_data.hpp>
#include "resolver.hpp"
#include "../structs.hpp"

#include <expected>
#include <string>

namespace zonetool
{
	class zone_memory;
}

// Replay 1.20 impact and surface effect tables to and from neutral data (docs/NEUTRAL_ASSETS.md, family F7). Effects
// are referenced by the name of their vfx asset.
namespace zonetool::iw8::neutral
{
	std::expected<asset_data::impact_table, std::string> to_neutral(const FxImpactTable& native);
	FxImpactTable* from_neutral(const asset_data::impact_table& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::impact_table& data);

	std::expected<asset_data::surface_effect_table, std::string> to_neutral(const SurfaceFxTable& native);
	SurfaceFxTable* from_neutral(const asset_data::surface_effect_table& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::surface_effect_table& data);
}
