#pragma once

#include <core/assets/rumble_data.hpp>
#include <iw8/structs.hpp>

namespace zonetool::iw8
{
// Decode a complete owned Replay record. Reference-only headers must be resolved by the caller first.
inline asset_data::rumble_graph decode_rumble_graph(const RumbleGraph& native)
{
	if (!native.name)
	{
		throw std::runtime_error("native RumbleGraph has no name");
	}
	asset_data::rumble_graph value;
	value.name = native.name;
	asset_data::rumble_detail::name(value.name);
	value.knot_count = native.knotCount;
	for (std::size_t i = 0; i < 16; ++i)
	{
		value.x[i] = native.knots[i].v[0];
		value.y[i] = native.knots[i].v[1];
	}
	return value;
}

inline asset_data::rumble_info decode_rumble(const RumbleInfo& native)
{
	if (!native.name)
	{
		throw std::runtime_error("native RumbleInfo has no name");
	}
	const auto graph = [](const RumbleGraph* header) -> std::optional<std::string>
	{
		if (!header)
		{
			return std::nullopt;
		}
		if (!header->name)
		{
			throw std::runtime_error("native rumble graph reference has no name");
		}
		std::string name = header->name;
		asset_data::rumble_detail::name(name);
		return name;
	};
	asset_data::rumble_info value;
	value.name = native.name;
	asset_data::rumble_detail::name(value.name);
	value.duration_milliseconds = native.duration;
	value.range = native.range;
	value.high = graph(native.highRumbleGraph);
	value.low = graph(native.lowRumbleGraph);
	value.left_trigger = graph(native.leftTriggerRumbleGraph);
	value.right_trigger = graph(native.rightTriggerRumbleGraph);
	value.fade_with_distance = native.fadeWithDistance;
	value.broadcast = native.broadcast;
	value.swap_trigger_rumbles_on_lefty = native.swapTriggerRumblesOnLefty;
	value.pan_trigger_rumbles = native.panTriggerRumbles;
	return value;
}
} // namespace zonetool::iw8
