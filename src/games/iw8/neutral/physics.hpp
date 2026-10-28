#pragma once

#include <core/assets/physics_data.hpp>
#include "resolver.hpp"
#include "../structs.hpp"

#include <expected>
#include <string>

namespace zonetool
{
	class zone_memory;
}

// Replay 1.20 physics assets to and from neutral data (docs/NEUTRAL_ASSETS.md, families F4 and F7). Sound and effect
// events are referenced by the name of their asset.
namespace zonetool::iw8::neutral
{
	std::expected<asset_data::physics_asset, std::string> to_neutral(const PhysicsAsset& native);
	PhysicsAsset* from_neutral(const asset_data::physics_asset& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::physics_asset& data);

	std::expected<asset_data::physics_fx_shape, std::string> to_neutral(const PhysicsFXShape& native);
	PhysicsFXShape* from_neutral(const asset_data::physics_fx_shape& data, zone_memory& memory);
	std::size_t native_size(const asset_data::physics_fx_shape& data);

	std::expected<asset_data::detail_collision, std::string> to_neutral(const XModelDetailCollision& native);
	XModelDetailCollision* from_neutral(const asset_data::detail_collision& data, zone_memory& memory);
	std::size_t native_size(const asset_data::detail_collision& data);
}
