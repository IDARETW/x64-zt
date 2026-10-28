#pragma once

// Emitted by tools/gen_iw8_neutral.py from tools/neutral_families/gfxworld.json: do not edit.
#include <core/assets/gfxworld_data.hpp>
#include "resolver.hpp"
#include "../structs.hpp"

#include <expected>
#include <string>

namespace zonetool
{
	class zone_memory;
}

// Replay 1.20 gfxworld assets to and from neutral data (docs/NEUTRAL_ASSETS.md).
namespace zonetool::iw8::neutral
{
	std::expected<asset_data::gfxworld::gfx_world, std::string> to_neutral(const GfxWorld& native);
	GfxWorld* from_neutral(const asset_data::gfxworld::gfx_world& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::gfxworld::gfx_world& data);

	std::expected<asset_data::gfxworld::gfx_world_transient_zone, std::string> to_neutral(const GfxWorldTransientZone& native);
	GfxWorldTransientZone* from_neutral(const asset_data::gfxworld::gfx_world_transient_zone& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::gfxworld::gfx_world_transient_zone& data);

}
