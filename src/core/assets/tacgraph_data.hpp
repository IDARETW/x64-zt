#pragma once

#include "asset_data.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

// Neutral tacgraph assets (docs/NEUTRAL_ASSETS.md), emitted by tools/gen_iw8_neutral.py from the IW8 structs and stream
// traversals: do not edit; change the spec (tools/neutral_families/tacgraph.json) or the generator.
namespace zonetool::asset_data
{
namespace tacgraph
{
struct tac_graph;
struct tactical_graph;
struct tacpoint_t;
struct bfx_area_handle;
struct tac_vis_graph_row;
struct tacpoint_search_node_t;

inline void validate(const tac_graph& data);
inline void validate(const tactical_graph& data);
inline void validate(const tacpoint_t& data);
inline void validate(const bfx_area_handle& data);
inline void validate(const tac_vis_graph_row& data);
inline void validate(const tacpoint_search_node_t& data);

struct tac_graph
{
	std::optional<std::string> m_name{};
	std::optional<std::vector<tactical_graph>> m_tac_graphs{};

	bool operator==(const tac_graph&) const = default;
};

struct tacpoint_search_node_t
{
	std::array<float, 2> m_mid_point{};
	std::array<float, 2> m_half_size{};
	std::uint16_t m_point_list_root{};
	std::optional<std::vector<std::optional<tacpoint_search_node_t>>> m_child_nodes{};

	bool operator==(const tacpoint_search_node_t&) const = default;
};

struct tactical_graph
{
	std::optional<std::vector<tacpoint_t>> m_points{};
	std::int32_t m_num_vis_bytes{};
	std::optional<std::vector<tac_vis_graph_row>> m_vis_graph{};
	tacpoint_search_node_t m_search_root{};

	bool operator==(const tactical_graph&) const = default;
};

struct bfx_area_handle
{

	bool operator==(const bfx_area_handle&) const = default;
};

struct tacpoint_t
{
	std::array<float, 3> m_pos{};
	std::int16_t m_vertical_clearance{};
	std::uint8_t m_graph_idx{};
	std::uint8_t m_type{};
	bfx_area_handle m_h_area{};
	std::uint16_t m_search_next{};
	std::uint16_t m_search_prev{};
	std::int32_t m_padding{};

	bool operator==(const tacpoint_t&) const = default;
};

struct tac_vis_graph_row
{
	std::uint16_t m_start{};
	std::uint16_t m_end{};
	std::optional<std::vector<std::uint8_t>> m_vis{};

	bool operator==(const tac_vis_graph_row&) const = default;
};

inline void validate(const tac_graph& data)
{
	if (!data.m_name)
	{
		throw std::runtime_error("tac_graph has no name");
	}

	detail::name(*data.m_name, "tac_graph");

	if (data.m_tac_graphs)
	{
		for (const auto& item : *data.m_tac_graphs)
		{
			validate(item);
		}
	}

	if ((data.m_tac_graphs && data.m_tac_graphs->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("tac_graph has arrays that its count does not fit");
	}

}

inline void validate(const tacpoint_search_node_t& data)
{
	if (data.m_child_nodes)
	{
		for (const auto& item : *data.m_child_nodes)
		{
			if (item)
			{
				validate(*item);
			}
		}
	}

}

inline void validate(const tactical_graph& data)
{
	if (data.m_points)
	{
		for (const auto& item : *data.m_points)
		{
			validate(item);
		}
	}

	if (data.m_vis_graph)
	{
		for (const auto& item : *data.m_vis_graph)
		{
			validate(item);
		}
	}

	validate(data.m_search_root);

	if (static_cast<std::int64_t>(((-1) + (data.m_points ? data.m_points->size() : std::size_t{}))) != (data.m_vis_graph ? static_cast<std::int64_t>(data.m_vis_graph->size()) : 0))
	{
		throw std::runtime_error("tactical_graph.m_vis_graph has a length that its count does not give");
	}

	if ((data.m_points && data.m_points->size() > 0xFFFFull))
	{
		throw std::runtime_error("tactical_graph has arrays that its count does not fit");
	}

}

inline void validate(const bfx_area_handle& data)
{
	(void) data;
}

inline void validate(const tacpoint_t& data)
{
	validate(data.m_h_area);

}

inline void validate(const tac_vis_graph_row& data)
{
	(void) data;
}

} // namespace tacgraph
} // namespace zonetool::asset_data
