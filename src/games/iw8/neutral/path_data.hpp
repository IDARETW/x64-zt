#pragma once

// Emitted by tools/gen_iw8_neutral.py from tools/neutral_families/path_data.json: do not edit.
#include <core/assets/path_data_data.hpp>
#include "resolver.hpp"
#include "../structs.hpp"

#include <expected>
#include <string>

namespace zonetool
{
	class zone_memory;
}

// Replay 1.20 path_data assets to and from neutral data (docs/NEUTRAL_ASSETS.md).
namespace zonetool::iw8::neutral
{
	std::expected<asset_data::path_data::path_data, std::string> to_neutral(const PathData& native);
	PathData* from_neutral(const asset_data::path_data::path_data& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::path_data::path_data& data);

}
