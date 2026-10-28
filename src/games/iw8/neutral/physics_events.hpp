#pragma once

// Emitted by tools/gen_iw8_neutral.py from tools/neutral_families/physics_events.json: do not edit.
#include <core/assets/physics_events_data.hpp>
#include "resolver.hpp"
#include "../structs.hpp"

#include <expected>
#include <string>

namespace zonetool
{
	class zone_memory;
}

// Replay 1.20 physics_events assets to and from neutral data (docs/NEUTRAL_ASSETS.md).
namespace zonetool::iw8::neutral
{
	std::expected<asset_data::physics_events::physics_sfx_event, std::string> to_neutral(const PhysicsSFXEventAsset& native);
	PhysicsSFXEventAsset* from_neutral(const asset_data::physics_events::physics_sfx_event& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::physics_events::physics_sfx_event& data);

	std::expected<asset_data::physics_events::physics_vfx_event, std::string> to_neutral(const PhysicsVFXEventAsset& native);
	PhysicsVFXEventAsset* from_neutral(const asset_data::physics_events::physics_vfx_event& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::physics_events::physics_vfx_event& data);

}
