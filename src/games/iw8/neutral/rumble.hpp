#pragma once

#include <core/assets/rumble_data.hpp>
#include "../structs.hpp"

#include <functional>

namespace zonetool
{
	class zone_memory;
}

// Replay 1.20 rumble assets to and from neutral data (docs/NEUTRAL_ASSETS.md, family F9). A rumble names its curves;
// from_neutral takes the native graph of each name from resolve.
namespace zonetool::iw8::neutral
{
	asset_data::rumble_graph to_neutral(const RumbleGraph& native);
	asset_data::rumble_info to_neutral(const RumbleInfo& native);

	RumbleGraph* from_neutral(const asset_data::rumble_graph& data, zone_memory& memory);
	RumbleInfo* from_neutral(const asset_data::rumble_info& data, zone_memory& memory,
		const std::function<RumbleGraph*(const std::string&)>& resolve);

	std::size_t native_size(const asset_data::rumble_graph& data);
	std::size_t native_size(const asset_data::rumble_info& data);
}
