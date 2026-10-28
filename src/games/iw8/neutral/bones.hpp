#pragma once

// Emitted by tools/gen_iw8_neutral.py from tools/neutral_families/bones.json: do not edit.
#include <core/assets/bones_data.hpp>
#include "resolver.hpp"
#include "../structs.hpp"

#include <expected>
#include <string>

namespace zonetool
{
	class zone_memory;
}

// Replay 1.20 bones assets to and from neutral data (docs/NEUTRAL_ASSETS.md).
namespace zonetool::iw8::neutral
{
	std::expected<asset_data::bones::dynamicbones, std::string> to_neutral(const XAnimDynamicBones& native);
	XAnimDynamicBones* from_neutral(const asset_data::bones::dynamicbones& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::bones::dynamicbones& data);

	std::expected<asset_data::bones::proceduralbones, std::string> to_neutral(const XAnimProceduralBones& native);
	XAnimProceduralBones* from_neutral(const asset_data::bones::proceduralbones& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::bones::proceduralbones& data);

}
