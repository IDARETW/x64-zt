#pragma once

#include <json.hpp>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <limits>
#include <initializer_list>
#include <utility>
#include <optional>
#include <set>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace zonetool::asset_data
{
using rumble_json = nlohmann::ordered_json;
// Engine-neutral values. Source ABI, padding and ownership belong to separate provenance records.
struct rumble_graph
{
	std::string name;
	std::uint32_t knot_count{};
	std::array<float, 16> x{}, y{};
};
struct rumble_info
{
	std::string name;
	std::int32_t duration_milliseconds{};
	float range{};
	std::optional<std::string> high, low, left_trigger, right_trigger;
	bool fade_with_distance{}, broadcast{}, swap_trigger_rumbles_on_lefty{}, pan_trigger_rumbles{};
};
struct rumble_collection
{
	std::vector<rumble_info> infos;
	std::vector<rumble_graph> graphs;
};

namespace rumble_detail
{
inline void fields(const rumble_json& value, const std::initializer_list<std::string_view> names)
{
	if (!value.is_object() || value.size() != names.size())
	{
		throw std::runtime_error("rumble data has missing or unknown fields");
	}
	for (const auto name : names)
	{
		if (!value.contains(std::string(name)))
		{
			throw std::runtime_error("rumble data is missing " + std::string(name));
		}
	}
}
inline void name(const std::string& value)
{
	if (value.empty() || value.front() == ',' || value.find('\0') != std::string::npos)
	{
		throw std::runtime_error("rumble asset name must be a nonempty definition name");
	}
}
inline float number(const rumble_json& value)
{
	if (!value.is_number())
	{
		throw std::runtime_error("rumble value must be a finite float");
	}
	const double input = value.get<double>();
	if (!std::isfinite(input) || std::abs(input) > std::numeric_limits<float>::max())
	{
		throw std::runtime_error("rumble value is outside the finite float range");
	}
	const auto result = static_cast<float>(input);
	if (input != 0 && result == 0)
	{
		throw std::runtime_error("rumble value underflows the float range");
	}
	return result;
}
inline std::int32_t integer(const rumble_json& value)
{
	if (value.is_number_unsigned())
	{
		const auto number = value.get<std::uint64_t>();
		if (number > static_cast<std::uint64_t>(std::numeric_limits<std::int32_t>::max()))
		{
			throw std::runtime_error("rumble integer exceeds int32");
		}
		return static_cast<std::int32_t>(number);
	}
	if (!value.is_number_integer())
	{
		throw std::runtime_error("rumble integer must have an integer encoding");
	}
	const auto number = value.get<std::int64_t>();
	if (number < std::numeric_limits<std::int32_t>::min() || number > std::numeric_limits<std::int32_t>::max())
	{
		throw std::runtime_error("rumble integer exceeds int32");
	}
	return static_cast<std::int32_t>(number);
}
inline std::optional<std::string> curve(const rumble_json& value)
{
	return value.is_null() ? std::nullopt : std::optional(value.get<std::string>());
}
} // namespace rumble_detail

inline void validate(const rumble_collection& data)
{
	std::set<std::string> graphs, infos;
	for (const auto& graph : data.graphs)
	{
		rumble_detail::name(graph.name);
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
		rumble_detail::name(info.name);
		if (!infos.insert(info.name).second || !std::isfinite(info.range))
		{
			throw std::runtime_error("rumble info has a duplicate identity or nonfinite range");
		}
		for (const auto* curve : {&info.high, &info.low, &info.left_trigger, &info.right_trigger})
		{
			if (*curve)
			{
				rumble_detail::name(**curve);
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
	using namespace rumble_detail;
	fields(value, {"schema", "infos", "graphs"});
	if (integer(value.at("schema")) != 1 || !value.at("infos").is_array() || !value.at("graphs").is_array())
	{
		throw std::runtime_error("unsupported rumble collection schema");
	}
	rumble_collection result;
	for (const auto& item : value.at("graphs"))
	{
		fields(item, {"name", "knotCount", "x", "y"});
		rumble_graph graph;
		graph.name = item.at("name").get<std::string>();
		const auto count = integer(item.at("knotCount"));
		if (count < 0 || count > 16 || !item.at("x").is_array() || !item.at("y").is_array() || item.at("x").size() != 16 ||
		    item.at("y").size() != 16)
		{
			throw std::runtime_error("rumble graph requires all sixteen slots and a count in 0..16");
		}
		graph.knot_count = static_cast<std::uint32_t>(count);
		for (std::size_t i = 0; i < 16; ++i)
		{
			graph.x[i] = number(item.at("x").at(i));
			graph.y[i] = number(item.at("y").at(i));
		}
		result.graphs.push_back(std::move(graph));
	}
	for (const auto& item : value.at("infos"))
	{
		fields(item, {"name", "durationMilliseconds", "range", "curves", "flags"});
		const auto& curves = item.at("curves");
		const auto& flags = item.at("flags");
		fields(curves, {"high", "low", "leftTrigger", "rightTrigger"});
		fields(flags, {"fadeWithDistance", "broadcast", "swapTriggerRumblesOnLefty", "panTriggerRumbles"});
		rumble_info info;
		info.name = item.at("name").get<std::string>();
		info.duration_milliseconds = integer(item.at("durationMilliseconds"));
		info.range = number(item.at("range"));
		info.high = curve(curves.at("high"));
		info.low = curve(curves.at("low"));
		info.left_trigger = curve(curves.at("leftTrigger"));
		info.right_trigger = curve(curves.at("rightTrigger"));
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
	const auto reference = [](const std::optional<std::string>& name) { return name ? rumble_json(*name) : rumble_json(nullptr); };
	rumble_json result{{"schema", 1}, {"infos", rumble_json::array()}, {"graphs", rumble_json::array()}};
	for (const auto& info : sorted.infos)
	{
		result["infos"].push_back({{"name", info.name},
		                           {"durationMilliseconds", info.duration_milliseconds},
		                           {"range", info.range},
		                           {"curves",
		                            {{"high", reference(info.high)},
		                             {"low", reference(info.low)},
		                             {"leftTrigger", reference(info.left_trigger)},
		                             {"rightTrigger", reference(info.right_trigger)}}},
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
