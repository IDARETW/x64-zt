#pragma once

// Emitted by tools/gen_iw8_neutral.py from tools/neutral_families/scriptable.json: do not edit.
#include <core/assets/scriptable_data.hpp>
#include "resolver.hpp"
#include "../structs.hpp"

#include <expected>
#include <string>

namespace zonetool
{
	class zone_memory;
}

// Replay 1.20 scriptable assets to and from neutral data (docs/NEUTRAL_ASSETS.md).
namespace zonetool::iw8::neutral
{
	std::expected<asset_data::scriptable::scriptable, std::string> to_neutral(const ScriptableDef& native);
	ScriptableDef* from_neutral(const asset_data::scriptable::scriptable& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::scriptable::scriptable& data);

}
