#include <std_include.hpp>
#include "rumble.hpp"

#include <zone/zone_memory.hpp>

namespace zonetool::iw8::neutral
{
	namespace
	{
		// zone_memory is a byte allocator: rounding every allocation to 8 keeps each later record aligned.
		constexpr std::size_t rounded(const std::size_t size)
		{
			return (size + 7) & ~std::size_t(7);
		}

		const char* copy_text(zone_memory& memory, const std::string& text)
		{
			auto* copy = memory.manual_allocate<char>(rounded(text.size() + 1));
			std::ranges::copy(text, copy);
			return copy;
		}

		std::string native_name(const char* name, const char* what)
		{
			if (!name)
			{
				throw std::runtime_error(std::string("native ") + what + " has no name");
			}

			std::string result = name;
			asset_data::detail::name(result, "rumble");
			return result;
		}

		// A curve of a rumble: the name of its graph, or none.
		std::optional<std::string> curve(const RumbleGraph* header)
		{
			return header ? std::optional(native_name(header->name, "rumble graph reference")) : std::nullopt;
		}
	}

	asset_data::rumble_graph to_neutral(const RumbleGraph& native)
	{
		asset_data::rumble_graph data;
		data.name = native_name(native.name, "RumbleGraph");
		data.knot_count = native.knotCount;
		for (std::size_t i = 0; i < 16; ++i)
		{
			data.x[i] = native.knots[i].v[0];
			data.y[i] = native.knots[i].v[1];
		}

		return data;
	}

	asset_data::rumble_info to_neutral(const RumbleInfo& native)
	{
		asset_data::rumble_info data;
		data.name = native_name(native.name, "RumbleInfo");
		data.duration_milliseconds = native.duration;
		data.range = native.range;
		data.high = curve(native.highRumbleGraph);
		data.low = curve(native.lowRumbleGraph);
		data.left_trigger = curve(native.leftTriggerRumbleGraph);
		data.right_trigger = curve(native.rightTriggerRumbleGraph);
		data.fade_with_distance = native.fadeWithDistance;
		data.broadcast = native.broadcast;
		data.swap_trigger_rumbles_on_lefty = native.swapTriggerRumblesOnLefty;
		data.pan_trigger_rumbles = native.panTriggerRumbles;
		return data;
	}

	RumbleGraph* from_neutral(const asset_data::rumble_graph& data, zone_memory& memory)
	{
		auto* native = memory.allocate<RumbleGraph>();
		native->knotCount = static_cast<std::uint16_t>(data.knot_count);
		for (std::size_t knot = 0; knot < 16; ++knot)
		{
			native->knots[knot].v[0] = data.x[knot];
			native->knots[knot].v[1] = data.y[knot];
		}

		native->name = copy_text(memory, data.name);
		return native;
	}

	RumbleInfo* from_neutral(const asset_data::rumble_info& data, zone_memory& memory,
		const std::function<RumbleGraph*(const std::string&)>& resolve)
	{
		const auto graph = [&resolve](const std::optional<std::string>& name) -> RumbleGraph*
		{
			if (!name)
			{
				return nullptr;
			}

			auto* header = resolve(*name);
			if (!header)
			{
				throw std::runtime_error("rumble graph dependency is absent: " + *name);
			}

			return header;
		};

		auto* native = memory.allocate<RumbleInfo>();
		native->duration = data.duration_milliseconds;
		native->range = data.range;
		native->highRumbleGraph = graph(data.high);
		native->lowRumbleGraph = graph(data.low);
		native->leftTriggerRumbleGraph = graph(data.left_trigger);
		native->rightTriggerRumbleGraph = graph(data.right_trigger);
		native->fadeWithDistance = data.fade_with_distance;
		native->broadcast = data.broadcast;
		native->swapTriggerRumblesOnLefty = data.swap_trigger_rumbles_on_lefty;
		native->panTriggerRumbles = data.pan_trigger_rumbles;
		native->name = copy_text(memory, data.name);
		return native;
	}

	std::size_t native_size(const asset_data::rumble_graph& data)
	{
		return rounded(sizeof(RumbleGraph)) + rounded(data.name.size() + 1);
	}

	std::size_t native_size(const asset_data::rumble_info& data)
	{
		return rounded(sizeof(RumbleInfo)) + rounded(data.name.size() + 1);
	}
}
