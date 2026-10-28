#pragma once

#include "asset_data.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <optional>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace zonetool::asset_data
{
using rumble_json = detail::data_json;
// Engine-neutral values. Source ABI, padding and ownership belong to separate provenance records.
struct rumble_graph
{
	std::string name;
	std::uint32_t knot_count{};
	std::array<float, 16> x{}, y{};

	bool operator==(const rumble_graph&) const = default;
};
struct rumble_info
{
	std::string name;
	std::int32_t duration_milliseconds{};
	float range{};
	std::optional<std::string> high, low, left_trigger, right_trigger;
	bool fade_with_distance{}, broadcast{}, swap_trigger_rumbles_on_lefty{}, pan_trigger_rumbles{};

	bool operator==(const rumble_info&) const = default;
};
struct rumble_collection
{
	std::vector<rumble_info> infos;
	std::vector<rumble_graph> graphs;
};


inline void validate(const rumble_collection& data)
{
	std::set<std::string> graphs, infos;
	for (const auto& graph : data.graphs)
	{
		detail::name(graph.name, "rumble");
		if (!graphs.insert(graph.name).second || graph.knot_count > 16)
		{
			throw std::runtime_error("rumble graph has a duplicate identity or invalid knot count");
		}
		for (std::size_t i = 0; i < 16; ++i)
		{
			if (!std::isfinite(graph.x[i]) || !std::isfinite(graph.y[i]))
			{
				throw std::runtime_error("rumble graph has a nonfinite component");
			}
		}
	}
	for (const auto& info : data.infos)
	{
		detail::name(info.name, "rumble");
		if (!infos.insert(info.name).second || !std::isfinite(info.range))
		{
			throw std::runtime_error("rumble info has a duplicate identity or nonfinite range");
		}
		for (const auto* curve : {&info.high, &info.low, &info.left_trigger, &info.right_trigger})
		{
			if (*curve)
			{
				detail::name(**curve, "rumble");
				if (!graphs.contains(**curve))
				{
					throw std::runtime_error("rumble graph dependency is absent: " + **curve);
				}
			}
		}
	}
}

inline rumble_collection read_rumbles(const nlohmann::ordered_json& value)
{
	detail::fields(value, {"schema", "infos", "graphs"}, "rumble");
	if (detail::integer(value.at("schema"), "rumble") != 1 || !value.at("infos").is_array() || !value.at("graphs").is_array())
	{
		throw std::runtime_error("unsupported rumble collection schema");
	}
	rumble_collection result;
	for (const auto& item : value.at("graphs"))
	{
		detail::fields(item, {"name", "knotCount", "x", "y"}, "rumble");
		rumble_graph graph;
		graph.name = item.at("name").get<std::string>();
		const auto count = detail::integer(item.at("knotCount"), "rumble");
		if (count < 0 || count > 16 || !item.at("x").is_array() || !item.at("y").is_array() || item.at("x").size() != 16 ||
		    item.at("y").size() != 16)
		{
			throw std::runtime_error("rumble graph requires all sixteen slots and a count in 0..16");
		}
		graph.knot_count = static_cast<std::uint32_t>(count);
		for (std::size_t i = 0; i < 16; ++i)
		{
			graph.x[i] = detail::number(item.at("x").at(i), "rumble");
			graph.y[i] = detail::number(item.at("y").at(i), "rumble");
		}
		result.graphs.push_back(std::move(graph));
	}
	for (const auto& item : value.at("infos"))
	{
		detail::fields(item, {"name", "durationMilliseconds", "range", "curves", "flags"}, "rumble");
		const auto& curves = item.at("curves");
		const auto& flags = item.at("flags");
		detail::fields(curves, {"high", "low", "leftTrigger", "rightTrigger"}, "rumble");
		detail::fields(flags, {"fadeWithDistance", "broadcast", "swapTriggerRumblesOnLefty", "panTriggerRumbles"}, "rumble");
		rumble_info info;
		info.name = item.at("name").get<std::string>();
		info.duration_milliseconds = detail::integer(item.at("durationMilliseconds"), "rumble");
		info.range = detail::number(item.at("range"), "rumble");
		info.high = detail::optional_text(curves.at("high"), "rumble");
		info.low = detail::optional_text(curves.at("low"), "rumble");
		info.left_trigger = detail::optional_text(curves.at("leftTrigger"), "rumble");
		info.right_trigger = detail::optional_text(curves.at("rightTrigger"), "rumble");
		info.fade_with_distance = flags.at("fadeWithDistance").get<bool>();
		info.broadcast = flags.at("broadcast").get<bool>();
		info.swap_trigger_rumbles_on_lefty = flags.at("swapTriggerRumblesOnLefty").get<bool>();
		info.pan_trigger_rumbles = flags.at("panTriggerRumbles").get<bool>();
		result.infos.push_back(std::move(info));
	}
	validate(result);
	return result;
}

inline nlohmann::ordered_json write_rumbles(const rumble_collection& data)
{
	validate(data);
	auto sorted = data;
	std::ranges::sort(sorted.infos, {}, &rumble_info::name);
	std::ranges::sort(sorted.graphs, {}, &rumble_graph::name);
	rumble_json result{{"schema", 1}, {"infos", rumble_json::array()}, {"graphs", rumble_json::array()}};
	for (const auto& info : sorted.infos)
	{
		result["infos"].push_back({{"name", info.name},
		                           {"durationMilliseconds", info.duration_milliseconds},
		                           {"range", info.range},
		                           {"curves",
		                            {{"high", detail::optional_json(info.high)},
		                             {"low", detail::optional_json(info.low)},
		                             {"leftTrigger", detail::optional_json(info.left_trigger)},
		                             {"rightTrigger", detail::optional_json(info.right_trigger)}}},
		                           {"flags",
		                            {{"fadeWithDistance", info.fade_with_distance},
		                             {"broadcast", info.broadcast},
		                             {"swapTriggerRumblesOnLefty", info.swap_trigger_rumbles_on_lefty},
		                             {"panTriggerRumbles", info.pan_trigger_rumbles}}}});
	}
	for (const auto& graph : sorted.graphs)
	{
		result["graphs"].push_back({{"name", graph.name}, {"knotCount", graph.knot_count}, {"x", graph.x}, {"y", graph.y}});
	}
	return result;
}
} // namespace zonetool::asset_data
