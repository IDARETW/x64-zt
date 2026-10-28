#pragma once

// Emitted by tools/gen_iw8_neutral.py from tools/neutral_families/mapents.json: do not edit.
#include <core/assets/mapents_data.hpp>
#include "resolver.hpp"
#include "../structs.hpp"

#include <expected>
#include <string>

namespace zonetool
{
	class zone_memory;
}

// Replay 1.20 mapents assets to and from neutral data (docs/NEUTRAL_ASSETS.md).
namespace zonetool::iw8::neutral
{
	std::expected<asset_data::mapents::map_ents, std::string> to_neutral(const MapEnts& native);
	MapEnts* from_neutral(const asset_data::mapents::map_ents& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::mapents::map_ents& data);

}
