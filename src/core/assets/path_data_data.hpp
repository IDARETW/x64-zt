#pragma once

#include "asset_data.hpp"
#include "path_tree_data.hpp"
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

// Neutral path_data assets (docs/NEUTRAL_ASSETS.md), emitted by tools/gen_iw8_neutral.py from the IW8 structs and stream
// traversals: do not edit; change the spec (tools/neutral_families/path_data.json) or the generator.
namespace zonetool::asset_data
{
namespace path_data
{
struct path_data;
struct pathnode_t;
struct pathnode_constant_t;
struct tacpoint_ref_t;
struct pathlink_s;
struct pathnode_dynamic_t;
struct sentient_handle;
struct pathnode_transient_t;
struct path_dynamic_node_group;

inline void validate(const path_data& data);
inline void validate(const pathnode_t& data);
inline void validate(const pathnode_constant_t& data);
inline void validate(const tacpoint_ref_t& data);
inline void validate(const pathlink_s& data);
inline void validate(const pathnode_dynamic_t& data);
inline void validate(const sentient_handle& data);
inline void validate(const pathnode_transient_t& data);
inline void validate(const path_dynamic_node_group& data);

struct path_data
{
	std::optional<std::string> name{};
	std::optional<std::vector<pathnode_t>> nodes{};
	bool parent_index_resolved{};
	std::uint16_t version{};
	std::optional<std::vector<std::uint8_t>> path_vis{};
	std::optional<std::vector<path_tree>> node_tree{};
	std::optional<std::vector<path_dynamic_node_group>> dynamic_node_groups{};
	bool use_path_extra_data{};
	std::optional<std::vector<std::uint8_t>> path_exposure{};
	std::optional<std::vector<std::uint8_t>> path_no_peek_vis{};
	std::int32_t zone_count{};
	std::optional<std::vector<std::uint8_t>> path_zones{};
	std::int32_t fixed_node_count{};
	std::int32_t max_dynamic_spawned_node_count{};
	std::optional<std::vector<std::uint8_t>> path_dyn_states{};
	std::int32_t vis_dist_data_size{};

	bool operator==(const path_data&) const = default;
};

struct tacpoint_ref_t
{
	std::uint16_t m_point_id{};
	std::uint8_t m_graph_id_plus_one{};

	bool operator==(const tacpoint_ref_t&) const = default;
};

struct pathnode_constant_t
{
	std::uint32_t spawnflags{};
	std::optional<std::string> targetname{};
	std::optional<std::string> script_link_name{};
	std::optional<std::string> script_noteworthy{};
	std::optional<std::string> target{};
	std::optional<std::string> animscript{};
	std::int32_t animscriptfunc{};
	std::optional<std::string> interactable{};
	std::array<float, 3> v_local_origin{};
	std::array<std::uint8_t, 12> u9{};
	std::uint32_t parent{}; // the bits of name or index
	std::uint32_t min_use_dist_sq_or_error{}; // the bits of minUseDistSq or error
	std::array<std::uint16_t, 2> w_overlap_node{};
	tacpoint_ref_t tacpoint_ref{};
	std::optional<std::vector<pathlink_s>> links{};
	std::uint16_t type{};

	bool operator==(const pathnode_constant_t&) const = default;
};

struct sentient_handle
{
	std::uint16_t number{};
	std::uint16_t info_index{};

	bool operator==(const sentient_handle&) const = default;
};

struct pathnode_dynamic_t
{
	std::array<sentient_handle, 2> p_owners{};
	std::array<std::int32_t, 2> i_free_time{};
	std::array<std::int32_t, 3> i_valid_time{};
	std::int16_t w_link_count{};
	std::uint16_t turret_ent_number_or_cover_multi_type{}; // the bits of turretEntNumber or coverMultiType
	std::uint8_t b_inactive{};
	std::array<std::uint8_t, 2> w_overlap_count{};
	std::array<std::uint8_t, 16> u7{};

	bool operator==(const pathnode_dynamic_t&) const = default;
};

struct pathnode_transient_t
{
	std::int32_t i_search_frame{};
	float f_cost{};
	float f_heuristic{};
	std::uint32_t node_cost_or_link_index{}; // the bits of nodeCost or linkIndex

	bool operator==(const pathnode_transient_t&) const = default;
};

struct pathnode_t
{
	pathnode_constant_t constant{};
	pathnode_dynamic_t dynamic{};
	pathnode_transient_t transient{};

	bool operator==(const pathnode_t&) const = default;
};

struct pathlink_s
{
	float f_dist{};
	std::uint16_t node_num{};
	std::uint8_t disconnect_count{};
	std::uint8_t negotiation_link{};
	std::uint8_t flags{};
	std::array<std::uint8_t, 3> ub_bad_place_count{};

	bool operator==(const pathlink_s&) const = default;
};

struct path_dynamic_node_group
{
	std::uint16_t parent_index{};
	std::optional<std::vector<path_tree>> node_tree{};

	bool operator==(const path_dynamic_node_group&) const = default;
};

inline void validate(const path_data& data)
{
	if (!data.name)
	{
		throw std::runtime_error("path_data has no name");
	}

	detail::name(*data.name, "path_data");

	if (data.nodes)
	{
		for (const auto& item : *data.nodes)
		{
			validate(item);
		}
	}

	if (data.node_tree)
	{
		for (const auto& item : *data.node_tree)
		{
			validate(item);
		}
	}

	if (data.dynamic_node_groups)
	{
		for (const auto& item : *data.dynamic_node_groups)
		{
			validate(item);
		}
	}

	if ((data.nodes && data.nodes->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("path_data has arrays that its count does not fit");
	}

	if ((data.path_vis && data.path_vis->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("path_data has arrays that its count does not fit");
	}

	if ((data.node_tree && data.node_tree->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("path_data has arrays that its count does not fit");
	}

	if ((data.dynamic_node_groups && data.dynamic_node_groups->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("path_data has arrays that its count does not fit");
	}

	if ((data.path_exposure && data.path_exposure->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("path_data has arrays that its count does not fit");
	}

	if ((data.path_no_peek_vis && data.path_no_peek_vis->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("path_data has arrays that its count does not fit");
	}

	if ((data.path_zones && data.path_zones->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("path_data has arrays that its count does not fit");
	}

	if ((data.path_dyn_states && data.path_dyn_states->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("path_data has arrays that its count does not fit");
	}

}

inline void validate(const tacpoint_ref_t& data)
{
	(void) data;
}

inline void validate(const pathnode_constant_t& data)
{
	validate(data.tacpoint_ref);

	if (data.links)
	{
		for (const auto& item : *data.links)
		{
			validate(item);
		}
	}

	if ((data.links && data.links->size() > 0xFFFFull))
	{
		throw std::runtime_error("pathnode_constant_t has arrays that its count does not fit");
	}

}

inline void validate(const sentient_handle& data)
{
	(void) data;
}

inline void validate(const pathnode_dynamic_t& data)
{
	for (const auto& item : data.p_owners)
	{
		validate(item);
	}

}

inline void validate(const pathnode_transient_t& data)
{
	(void) data;
}

inline void validate(const pathnode_t& data)
{
	validate(data.constant);

	validate(data.dynamic);

	validate(data.transient);

}

inline void validate(const pathlink_s& data)
{
	(void) data;
}

inline void validate(const path_dynamic_node_group& data)
{
	if (data.node_tree)
	{
		for (const auto& item : *data.node_tree)
		{
			validate(item);
		}
	}

	if ((data.node_tree && data.node_tree->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("path_dynamic_node_group has arrays that its count does not fit");
	}

}

} // namespace path_data
} // namespace zonetool::asset_data
