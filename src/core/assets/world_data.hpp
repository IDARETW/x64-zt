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

// Neutral world assets (docs/NEUTRAL_ASSETS.md), emitted by tools/gen_iw8_neutral.py from the IW8 structs and stream
// traversals: do not edit; change the spec (tools/neutral_families/world.json) or the generator.
namespace zonetool::asset_data
{
namespace world
{
inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 3> pmr_octree_flags_names{{
	{1, "empty_node"},
	{2, "leaf_node"},
	{4, "root_node"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 5> edge_query_bucket_t_names{{
	{0, "mount"},
	{1, "mantle"},
	{2, "ladder_centerline"},
	{3, "interaction"},
	{4, "edge_query_bucket_count"},
}};

struct com_primary_light
{
	std::uint8_t spawn_flags{};
	std::uint8_t type{};
	std::uint8_t can_use_shadow_map{};
	std::uint8_t needs_dynamic_shadows{};
	std::uint8_t is_volumetric{};
	std::uint32_t transient_zone_list{};
	std::uint32_t entity_id{};
	float intensity{};
	float uv_intensity{};
	float ir_intensity{};
	float heat_intensity{};
	std::array<float, 3> color_linear_srgb{};
	std::array<float, 3> dir{};
	std::array<float, 3> up{};
	std::array<float, 3> origin{};
	float radius{};
	std::array<float, 2> fade_offset{};
	float bulb_radius{};
	std::array<float, 3> bulb_length{};
	float cos_half_fov_outer{};
	float cos_half_fov_inner{};
	float shadow_near_plane_bias{};
	float shadow_softness{};
	float shadow_bias{};
	float shadow_area{};
	float distance_falloff{};
	float cos_half_fov_collimation{};
	float rotation_limit{};
	float translation_limit{};
	std::optional<std::string> def_name{};

	bool operator==(const com_primary_light&) const = default;
};

struct com_change_list_info
{
	std::uint32_t change_list_number{};
	std::int32_t time{};
	std::optional<std::string> user_name{};

	bool operator==(const com_change_list_info&) const = default;
};

struct bitarray_384
{
	std::array<std::uint32_t, 12> array{};

	bool operator==(const bitarray_384&) const = default;
};

struct com_world
{
	std::optional<std::string> name{};
	std::int32_t is_in_use{};
	std::int32_t use_forward_plus{};
	std::uint32_t bake_quality{};
	std::int32_t proxy_lod_quality{};
	std::array<float, 5> tr_vis_radii{};
	float tr_vis_facing_dist_add{};
	std::optional<std::vector<com_primary_light>> primary_lights{};
	std::uint32_t scriptable_primary_light_count{};
	std::uint32_t first_scriptable_primary_light{};
	std::optional<std::vector<std::uint16_t>> transient_table{};
	com_change_list_info change_list_info{};
	std::optional<std::vector<std::optional<std::string>>> umbra_gate_names{};
	bitarray_384 umbra_gate_initial_states{};

	bool operator==(const com_world&) const = default;
};

struct glass_piece
{
	std::uint16_t damage_taken{};
	std::uint16_t collapse_time{};
	std::int32_t last_state_change_time{};
	std::uint8_t impact_dir{};
	std::array<std::uint8_t, 2> impact_pos{};

	bool operator==(const glass_piece&) const = default;
};

struct glass_name
{
	std::optional<std::string> name_str{};
	std::optional<std::string> name{};
	std::optional<std::vector<std::uint16_t>> piece_indices{};

	bool operator==(const glass_name&) const = default;
};

struct spatial_partition_tree
{
	std::optional<std::vector<std::uint8_t>> tree{};
	std::optional<std::vector<std::uint32_t>> always_list{};

	bool operator==(const spatial_partition_tree&) const = default;
};

struct glass_data
{
	std::optional<std::vector<glass_piece>> glass_pieces{};
	std::uint16_t damage_to_weaken{};
	std::uint16_t damage_to_destroy{};
	std::optional<std::vector<glass_name>> glass_names{};
	std::optional<spatial_partition_tree> spatial_tree{};

	bool operator==(const glass_data&) const = default;
};

struct glass_world
{
	std::optional<std::string> name{};
	std::optional<glass_data> g_glass_data{};

	bool operator==(const glass_world&) const = default;
};

struct flagged_edge_metadata
{
	float packed_adjacent_normal{};
	std::uint16_t packed_open_angle{};
	std::array<std::uint8_t, 1> flags{};

	bool operator==(const flagged_edge_metadata&) const = default;
};

struct pmr_octree_node
{
	std::array<std::uint16_t, 3> uniform_grid_index{};
	std::uint8_t depth{};
	std::vector<std::string> flags{};
	std::uint32_t parent_node_index_or_q_edge_table_index{}; // the bits of parentNodeIndex or qEdgeTableIndex
	std::uint32_t child_node_set_index_or_q_edge_table_count{}; // the bits of childNodeSetIndex or qEdgeTableCount

	bool operator==(const pmr_octree_node&) const = default;
};

struct pmr_octree_metadata
{
	std::array<std::array<float, 4>, 16> node_half_width{};
	std::array<float, 4> min_extent{};
	pmr_octree_node root_node{};
	std::uint32_t node_set_count{};
	std::array<float, 16> node_bounding_radius_sq{};
	std::string edge_bucket{};
	std::array<std::uint8_t, 1> flags{};

	bool operator==(const pmr_octree_metadata&) const = default;
};

struct pmr_octree_node_set
{
	std::array<pmr_octree_node, 8> node{};

	bool operator==(const pmr_octree_node_set&) const = default;
};

struct edge_adjacency_metadata
{
	std::uint32_t start_index{};
	std::uint32_t mount_count{};
	std::uint32_t interaction_count{};

	bool operator==(const edge_adjacency_metadata&) const = default;
};

struct edge_adjacency
{
	std::uint16_t start{};
	std::uint16_t end{};
	std::uint32_t adj_edge{};
	std::uint16_t adj_start{};
	std::uint16_t adj_end{};

	bool operator==(const edge_adjacency&) const = default;
};

struct map_edge_list
{
	std::optional<std::string> name{};
	std::array<float, 3> mins{};
	std::array<float, 3> maxs{};
	std::uint32_t transient_index{};
	std::uint32_t dynamic_query_types{};
	std::uint32_t static_query_types{};
	std::uint32_t num_dynamic_edges{};
	std::uint32_t num_static_edges{};
	bool valid{};
	std::optional<std::vector<std::array<std::array<float, 4>, 2>>> edges{};
	std::optional<std::vector<flagged_edge_metadata>> edge_metadata{};
	std::optional<std::vector<pmr_octree_metadata>> edge_octrees{};
	std::optional<std::vector<pmr_octree_node_set>> edge_octree_node_sets{};
	std::optional<std::vector<std::uint32_t>> edge_indices{};
	std::optional<std::vector<edge_adjacency_metadata>> edge_adjacency_metadata{};
	std::optional<std::vector<edge_adjacency>> edge_adjacency{};

	bool operator==(const map_edge_list&) const = default;
};

struct nav_resource_s
{
	std::optional<std::string> target_name{};
	std::int32_t model_idx{};
	std::int32_t target_ent_num{};
	std::array<float, 3> local_offset_pos{};
	std::array<float, 4> local_offset_rot{};
	std::int32_t offset_layer{};
	std::int32_t build_error{};
	std::uint32_t layer_flags{};
	bool b_dockable{};
	bool b_is_volume{};
	std::optional<std::vector<std::uint8_t>> p_graph_buffer{};
	std::optional<std::vector<std::uint8_t>> p_working_graph{};

	bool operator==(const nav_resource_s&) const = default;
};

struct nav_boundary_plane_s
{
	std::array<float, 3> m_normal{};
	float m_dist_from_center{};

	bool operator==(const nav_boundary_plane_s&) const = default;
};

struct nav_obstacle_hull_s
{
	std::optional<std::vector<nav_boundary_plane_s>> m_boundaries{};
	float m_bounds_x{};
	float m_bounds_y{};
	float m_bounds_z{};
	std::array<float, 3> m_offset{};
	bool m_b_use_bounds{};

	bool operator==(const nav_obstacle_hull_s&) const = default;
};

struct nav_obstacle_bounds_s
{
	std::optional<std::vector<nav_obstacle_hull_s>> m_hulls{};

	bool operator==(const nav_obstacle_bounds_s&) const = default;
};

struct nav_glass_bounds_s
{
	std::optional<std::vector<nav_boundary_plane_s>> m_boundaries{};
	std::array<float, 3> m_pos{};
	std::uint32_t m_glass_piece_index{};

	bool operator==(const nav_glass_bounds_s&) const = default;
};

struct nav_modifier_s
{
	std::optional<std::vector<nav_boundary_plane_s>> m_boundaries{};
	std::array<float, 3> m_pos{};
	std::optional<std::string> m_target_name{};
	std::optional<std::string> m_target{};
	std::optional<std::string> m_script_noteworthy{};
	std::optional<std::string> m_script_link_name{};
	std::uint32_t m_user_flags{};
	std::uint32_t m_layer_flags{};
	float m_weight{};
	bool m_b_active_on_load{};

	bool operator==(const nav_modifier_s&) const = default;
};

struct nav_link_point_s
{
	std::array<float, 3> m_pt1{};
	std::array<float, 3> m_pt2{};

	bool operator==(const nav_link_point_s&) const = default;
};

struct nav_link_creation_data_s
{
	nav_link_point_s m_start{};
	nav_link_point_s m_end{};
	std::optional<std::string> m_animscript{};
	std::optional<std::string> m_target{};
	std::optional<std::string> m_parent{};
	std::uint32_t m_usage_flags{};
	float m_penalty_mult{};
	bool m_b_bidirectional{};

	bool operator==(const nav_link_creation_data_s&) const = default;
};

struct nav_raw_volume_s
{
	std::array<float, 3> m_mid_point{};
	std::array<float, 3> m_half_size{};
	float m_min_voxel_size{};
	float m_clearance_xy{};
	float m_clearance_z{};

	bool operator==(const nav_raw_volume_s&) const = default;
};

struct nav_raw_custom_volume_tri_s
{
	std::array<std::array<float, 3>, 3> m_verts{};

	bool operator==(const nav_raw_custom_volume_tri_s&) const = default;
};

struct nav_raw_custom_volume_s
{
	std::optional<std::vector<nav_raw_custom_volume_tri_s>> m_tris{};

	bool operator==(const nav_raw_custom_volume_s&) const = default;
};

struct nav_misc_s
{
	std::array<float, 3> m_static_bounds_half_size{};
	std::array<float, 3> m_static_bounds_mid_point{};

	bool operator==(const nav_misc_s&) const = default;
};

struct nav_mesh
{
	std::optional<std::string> name{};
	std::int32_t version{};
	std::int32_t has_exposure_data{};
	std::optional<std::vector<nav_resource_s>> nav_resources{};
	std::optional<std::vector<nav_obstacle_bounds_s>> obstacle_bounds{};
	std::optional<std::vector<nav_glass_bounds_s>> glass_bounds{};
	std::optional<std::vector<nav_modifier_s>> modifiers{};
	std::optional<std::vector<nav_link_creation_data_s>> link_creation_data{};
	std::optional<std::vector<std::array<float, 3>>> volume_seeds{};
	std::optional<std::vector<nav_raw_volume_s>> raw_volumes{};
	std::optional<std::vector<nav_raw_custom_volume_s>> raw_custom_volumes{};
	nav_misc_s misc_data{};

	bool operator==(const nav_mesh&) const = default;
};

struct static_model_collision_instance
{
	std::array<float, 3> origin{};
	std::array<float, 3> angles{};
	float scale{};

	bool operator==(const static_model_collision_instance&) const = default;
};

struct static_model_collision_model
{
	std::optional<std::string> name{};
	std::optional<std::string> physics_asset{};
	std::optional<std::string> detail_collision{};
	std::optional<std::vector<static_model_collision_instance>> instances{};

	bool operator==(const static_model_collision_model&) const = default;
};

struct static_model_collision_model_list
{
	std::optional<std::vector<static_model_collision_model>> models{};
	std::uint32_t structure_memory_cost{};

	bool operator==(const static_model_collision_model_list&) const = default;
};

struct stage
{
	std::optional<std::string> name{};
	std::array<float, 3> origin{};
	std::uint16_t trigger_index{};
	std::uint8_t sun_primary_light_index{};
	std::uint32_t entity_uid{};
	std::array<float, 3> sky_rotation_angles{};

	bool operator==(const stage&) const = default;
};

struct trigger_model
{
	std::int32_t contents{};
	std::uint16_t hull_count{};
	std::uint16_t first_hull{};
	std::uint16_t winding_count{};
	std::uint16_t first_winding{};
	std::uint32_t flags{};
	std::optional<std::string> physics_asset{};
	std::uint16_t physics_shape_override_idx{};

	bool operator==(const trigger_model&) const = default;
};

struct bounds
{
	std::array<float, 3> mid_point{};
	std::array<float, 3> half_size{};

	bool operator==(const bounds&) const = default;
};

struct trigger_hull
{
	bounds bounds{};
	std::int32_t contents{};
	std::uint16_t slab_count{};
	std::uint16_t first_slab{};

	bool operator==(const trigger_hull&) const = default;
};

struct trigger_slab
{
	std::array<float, 3> dir{};
	float mid_point{};
	float half_size{};

	bool operator==(const trigger_slab&) const = default;
};

struct trigger_winding
{
	std::array<float, 3> dir{};
	std::uint16_t point_count{};
	std::uint16_t first_point{};
	std::uint32_t flags{};

	bool operator==(const trigger_winding&) const = default;
};

struct trigger_winding_point
{
	std::array<float, 3> loc{};

	bool operator==(const trigger_winding_point&) const = default;
};

struct map_triggers
{
	std::uint32_t count{};
	std::optional<std::vector<trigger_model>> models{};
	std::optional<std::vector<trigger_hull>> hulls{};
	std::optional<std::vector<trigger_slab>> slabs{};
	std::optional<std::vector<trigger_winding>> windings{};
	std::optional<std::vector<trigger_winding_point>> winding_points{};

	bool operator==(const map_triggers&) const = default;
};

struct physics_capacities
{
	std::int32_t max_num_rigid_bodies_server{};
	std::int32_t max_num_detail_rigid_bodies_server{};
	std::int32_t max_num_constraints_server{};
	std::int32_t max_num_motions_server{};
	std::int32_t max_num_rigid_bodies_client{};
	std::int32_t max_num_detail_rigid_bodies_client{};
	std::int32_t max_num_constraints_client{};
	std::int32_t max_num_motions_client{};

	bool operator==(const physics_capacities&) const = default;
};

struct collision_heatmap_entry
{
	std::int32_t vertex_count{};
	float vertex_density{};
	std::array<float, 3> min_extent{};
	std::array<float, 3> max_extent{};

	bool operator==(const collision_heatmap_entry&) const = default;
};

struct top_down_map_data_header
{
	std::int32_t version{};
	std::int32_t samples_per_axis{};
	std::array<float, 2> bottom_left{};
	std::array<float, 2> top_right{};

	bool operator==(const top_down_map_data_header&) const = default;
};

struct top_down_map_data_content
{
	std::array<std::uint8_t, 8193> nav_mesh_visible{};

	bool operator==(const top_down_map_data_content&) const = default;
};

struct top_down_map_data
{
	top_down_map_data_header header{};
	top_down_map_data_content content{};

	bool operator==(const top_down_map_data&) const = default;
};

struct clip_map
{
	std::optional<std::string> name{};
	std::int32_t is_in_use{};
	std::optional<std::vector<static_model_collision_model_list>> static_model_collision_model_lists{};
	std::optional<std::string> map_ents{};
	std::optional<std::vector<stage>> stages{};
	map_triggers stage_trigger{};
	std::array<float, 3> broadphase_min{};
	std::array<float, 3> broadphase_max{};
	physics_capacities physics_capacities{};
	std::optional<std::vector<char>> havok_world_shape_data{};
	std::optional<std::vector<collision_heatmap_entry>> collision_heatmap{};
	std::uint32_t total_glass_init_piece_count{};
	std::uint32_t total_glass_piece_limit{};
	std::optional<top_down_map_data> top_down_map_data{};
	std::optional<std::string> vision_set_name{};
	std::uint32_t checksum{};

	bool operator==(const clip_map&) const = default;
};

struct gfx_fog_spline_data
{
	float fog_start_dist{};
	float fog_half_plane_distance{};
	float fog_max_distance{};

	bool operator==(const gfx_fog_spline_data&) const = default;
};

struct fog_spline
{
	std::optional<std::string> name{};
	std::optional<std::string> image{};
	gfx_fog_spline_data data{};

	bool operator==(const fog_spline&) const = default;
};

inline void validate(const com_primary_light& data)
{
	(void) data;
}

inline void validate(const com_change_list_info& data)
{
	(void) data;
}

inline void validate(const bitarray_384& data)
{
	(void) data;
}

inline void validate(const com_world& data)
{
	if (!data.name)
	{
		throw std::runtime_error("com_world has no name");
	}

	detail::name(*data.name, "com_world");

	if (data.primary_lights)
	{
		for (const auto& item : *data.primary_lights)
		{
			validate(item);
		}
	}

	validate(data.change_list_info);

	validate(data.umbra_gate_initial_states);

	if ((data.primary_lights && data.primary_lights->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("com_world has arrays that its count does not fit");
	}

	if ((data.transient_table && data.transient_table->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("com_world has arrays that its count does not fit");
	}

	if ((data.umbra_gate_names && data.umbra_gate_names->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("com_world has arrays that its count does not fit");
	}

}

inline void validate(const glass_piece& data)
{
	(void) data;
}

inline void validate(const glass_name& data)
{
	if ((data.piece_indices && data.piece_indices->size() > 0xFFFFull))
	{
		throw std::runtime_error("glass_name has arrays that its count does not fit");
	}

}

inline void validate(const spatial_partition_tree& data)
{
	if ((data.tree && data.tree->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("spatial_partition_tree has arrays that its count does not fit");
	}

	if ((data.always_list && data.always_list->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("spatial_partition_tree has arrays that its count does not fit");
	}

}

inline void validate(const glass_data& data)
{
	if (data.glass_pieces)
	{
		for (const auto& item : *data.glass_pieces)
		{
			validate(item);
		}
	}

	if (data.glass_names)
	{
		for (const auto& item : *data.glass_names)
		{
			validate(item);
		}
	}

	if (data.spatial_tree)
	{
		validate(*data.spatial_tree);
	}

	if ((data.glass_pieces && data.glass_pieces->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("glass_data has arrays that its count does not fit");
	}

	if ((data.glass_names && data.glass_names->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("glass_data has arrays that its count does not fit");
	}

}

inline void validate(const glass_world& data)
{
	if (!data.name)
	{
		throw std::runtime_error("glass_world has no name");
	}

	detail::name(*data.name, "glass_world");

	if (data.g_glass_data)
	{
		validate(*data.g_glass_data);
	}

}

inline void validate(const flagged_edge_metadata& data)
{
	(void) data;
}

inline void validate(const pmr_octree_node& data)
{
	for (const auto& name : data.flags)
	{
		if (std::ranges::none_of(pmr_octree_flags_names, [&](const auto& entry) { return entry.second == name; }))
		{
			throw std::runtime_error("pmr_octree_node.flags names no flag");
		}
	}

}

inline void validate(const pmr_octree_metadata& data)
{
	validate(data.root_node);

	if (std::ranges::none_of(edge_query_bucket_t_names, [&](const auto& entry) { return entry.second == data.edge_bucket; }))
	{
		throw std::runtime_error("pmr_octree_metadata.edge_bucket names no value");
	}

}

inline void validate(const pmr_octree_node_set& data)
{
	for (const auto& item : data.node)
	{
		validate(item);
	}

}

inline void validate(const edge_adjacency_metadata& data)
{
	(void) data;
}

inline void validate(const edge_adjacency& data)
{
	(void) data;
}

inline void validate(const map_edge_list& data)
{
	if (!data.name)
	{
		throw std::runtime_error("map_edge_list has no name");
	}

	detail::name(*data.name, "map_edge_list");

	if (data.edge_metadata)
	{
		for (const auto& item : *data.edge_metadata)
		{
			validate(item);
		}
	}

	if (data.edge_octrees)
	{
		for (const auto& item : *data.edge_octrees)
		{
			validate(item);
		}
	}

	if (data.edge_octree_node_sets)
	{
		for (const auto& item : *data.edge_octree_node_sets)
		{
			validate(item);
		}
	}

	if (data.edge_adjacency_metadata)
	{
		for (const auto& item : *data.edge_adjacency_metadata)
		{
			validate(item);
		}
	}

	if (data.edge_adjacency)
	{
		for (const auto& item : *data.edge_adjacency)
		{
			validate(item);
		}
	}

	if ((data.edges && data.edges->size() > 0xFFFFFFFFull) || (data.edge_metadata && data.edge_metadata->size() > 0xFFFFFFFFull) || (!data.edges != !data.edge_metadata) || (data.edges && data.edge_metadata && data.edges->size() != data.edge_metadata->size()))
	{
		throw std::runtime_error("map_edge_list has arrays that its count does not fit");
	}

	if ((data.edge_octrees && data.edge_octrees->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("map_edge_list has arrays that its count does not fit");
	}

	if ((data.edge_octree_node_sets && data.edge_octree_node_sets->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("map_edge_list has arrays that its count does not fit");
	}

	if ((data.edge_indices && data.edge_indices->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("map_edge_list has arrays that its count does not fit");
	}

	if ((data.edge_adjacency_metadata && data.edge_adjacency_metadata->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("map_edge_list has arrays that its count does not fit");
	}

	if ((data.edge_adjacency && data.edge_adjacency->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("map_edge_list has arrays that its count does not fit");
	}

}

inline void validate(const nav_resource_s& data)
{
	if (static_cast<std::int64_t>((static_cast<std::uint8_t>(data.b_dockable) == 0 ? 0 : (data.p_graph_buffer ? data.p_graph_buffer->size() : std::size_t{}))) != (data.p_working_graph ? static_cast<std::int64_t>(data.p_working_graph->size()) : 0))
	{
		throw std::runtime_error("nav_resource_s.p_working_graph has a length that its count does not give");
	}

	if ((data.p_graph_buffer && data.p_graph_buffer->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("nav_resource_s has arrays that its count does not fit");
	}

}

inline void validate(const nav_boundary_plane_s& data)
{
	(void) data;
}

inline void validate(const nav_obstacle_hull_s& data)
{
	if (data.m_boundaries)
	{
		for (const auto& item : *data.m_boundaries)
		{
			validate(item);
		}
	}

	if ((data.m_boundaries && data.m_boundaries->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("nav_obstacle_hull_s has arrays that its count does not fit");
	}

}

inline void validate(const nav_obstacle_bounds_s& data)
{
	if (data.m_hulls)
	{
		for (const auto& item : *data.m_hulls)
		{
			validate(item);
		}
	}

	if ((data.m_hulls && data.m_hulls->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("nav_obstacle_bounds_s has arrays that its count does not fit");
	}

}

inline void validate(const nav_glass_bounds_s& data)
{
	if (data.m_boundaries)
	{
		for (const auto& item : *data.m_boundaries)
		{
			validate(item);
		}
	}

	if ((data.m_boundaries && data.m_boundaries->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("nav_glass_bounds_s has arrays that its count does not fit");
	}

}

inline void validate(const nav_modifier_s& data)
{
	if (data.m_boundaries)
	{
		for (const auto& item : *data.m_boundaries)
		{
			validate(item);
		}
	}

	if ((data.m_boundaries && data.m_boundaries->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("nav_modifier_s has arrays that its count does not fit");
	}

}

inline void validate(const nav_link_point_s& data)
{
	(void) data;
}

inline void validate(const nav_link_creation_data_s& data)
{
	validate(data.m_start);

	validate(data.m_end);

}

inline void validate(const nav_raw_volume_s& data)
{
	(void) data;
}

inline void validate(const nav_raw_custom_volume_tri_s& data)
{
	(void) data;
}

inline void validate(const nav_raw_custom_volume_s& data)
{
	if (data.m_tris)
	{
		for (const auto& item : *data.m_tris)
		{
			validate(item);
		}
	}

	if ((data.m_tris && data.m_tris->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("nav_raw_custom_volume_s has arrays that its count does not fit");
	}

}

inline void validate(const nav_misc_s& data)
{
	(void) data;
}

inline void validate(const nav_mesh& data)
{
	if (!data.name)
	{
		throw std::runtime_error("nav_mesh has no name");
	}

	detail::name(*data.name, "nav_mesh");

	if (data.nav_resources)
	{
		for (const auto& item : *data.nav_resources)
		{
			validate(item);
		}
	}

	if (data.obstacle_bounds)
	{
		for (const auto& item : *data.obstacle_bounds)
		{
			validate(item);
		}
	}

	if (data.glass_bounds)
	{
		for (const auto& item : *data.glass_bounds)
		{
			validate(item);
		}
	}

	if (data.modifiers)
	{
		for (const auto& item : *data.modifiers)
		{
			validate(item);
		}
	}

	if (data.link_creation_data)
	{
		for (const auto& item : *data.link_creation_data)
		{
			validate(item);
		}
	}

	if (data.raw_volumes)
	{
		for (const auto& item : *data.raw_volumes)
		{
			validate(item);
		}
	}

	if (data.raw_custom_volumes)
	{
		for (const auto& item : *data.raw_custom_volumes)
		{
			validate(item);
		}
	}

	validate(data.misc_data);

	if ((data.nav_resources && data.nav_resources->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("nav_mesh has arrays that its count does not fit");
	}

	if ((data.obstacle_bounds && data.obstacle_bounds->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("nav_mesh has arrays that its count does not fit");
	}

	if ((data.glass_bounds && data.glass_bounds->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("nav_mesh has arrays that its count does not fit");
	}

	if ((data.modifiers && data.modifiers->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("nav_mesh has arrays that its count does not fit");
	}

	if ((data.link_creation_data && data.link_creation_data->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("nav_mesh has arrays that its count does not fit");
	}

	if ((data.volume_seeds && data.volume_seeds->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("nav_mesh has arrays that its count does not fit");
	}

	if ((data.raw_volumes && data.raw_volumes->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("nav_mesh has arrays that its count does not fit");
	}

	if ((data.raw_custom_volumes && data.raw_custom_volumes->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("nav_mesh has arrays that its count does not fit");
	}

}

inline void validate(const static_model_collision_instance& data)
{
	(void) data;
}

inline void validate(const static_model_collision_model& data)
{
	if (data.instances)
	{
		for (const auto& item : *data.instances)
		{
			validate(item);
		}
	}

	if ((data.instances && data.instances->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("static_model_collision_model has arrays that its count does not fit");
	}

}

inline void validate(const static_model_collision_model_list& data)
{
	if (data.models)
	{
		for (const auto& item : *data.models)
		{
			validate(item);
		}
	}

	if ((data.models && data.models->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("static_model_collision_model_list has arrays that its count does not fit");
	}

}

inline void validate(const stage& data)
{
	(void) data;
}

inline void validate(const trigger_model& data)
{
	(void) data;
}

inline void validate(const bounds& data)
{
	(void) data;
}

inline void validate(const trigger_hull& data)
{
	validate(data.bounds);

}

inline void validate(const trigger_slab& data)
{
	(void) data;
}

inline void validate(const trigger_winding& data)
{
	(void) data;
}

inline void validate(const trigger_winding_point& data)
{
	(void) data;
}

inline void validate(const map_triggers& data)
{
	if (data.models)
	{
		for (const auto& item : *data.models)
		{
			validate(item);
		}
	}

	if (data.hulls)
	{
		for (const auto& item : *data.hulls)
		{
			validate(item);
		}
	}

	if (data.slabs)
	{
		for (const auto& item : *data.slabs)
		{
			validate(item);
		}
	}

	if (data.windings)
	{
		for (const auto& item : *data.windings)
		{
			validate(item);
		}
	}

	if (data.winding_points)
	{
		for (const auto& item : *data.winding_points)
		{
			validate(item);
		}
	}

	if (static_cast<std::int64_t>(data.count) != (data.models ? static_cast<std::int64_t>(data.models->size()) : 0))
	{
		throw std::runtime_error("map_triggers.models has a length that its count does not give");
	}

	if ((data.hulls && data.hulls->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("map_triggers has arrays that its count does not fit");
	}

	if ((data.slabs && data.slabs->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("map_triggers has arrays that its count does not fit");
	}

	if ((data.windings && data.windings->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("map_triggers has arrays that its count does not fit");
	}

	if ((data.winding_points && data.winding_points->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("map_triggers has arrays that its count does not fit");
	}

}

inline void validate(const physics_capacities& data)
{
	(void) data;
}

inline void validate(const collision_heatmap_entry& data)
{
	(void) data;
}

inline void validate(const top_down_map_data_header& data)
{
	(void) data;
}

inline void validate(const top_down_map_data_content& data)
{
	(void) data;
}

inline void validate(const top_down_map_data& data)
{
	validate(data.header);

	validate(data.content);

}

inline void validate(const clip_map& data)
{
	if (!data.name)
	{
		throw std::runtime_error("clip_map has no name");
	}

	detail::name(*data.name, "clip_map");

	if (data.static_model_collision_model_lists)
	{
		for (const auto& item : *data.static_model_collision_model_lists)
		{
			validate(item);
		}
	}

	if (data.stages)
	{
		for (const auto& item : *data.stages)
		{
			validate(item);
		}
	}

	validate(data.stage_trigger);

	validate(data.physics_capacities);

	if (data.collision_heatmap)
	{
		for (const auto& item : *data.collision_heatmap)
		{
			validate(item);
		}
	}

	if (data.top_down_map_data)
	{
		validate(*data.top_down_map_data);
	}

	if ((data.static_model_collision_model_lists && data.static_model_collision_model_lists->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("clip_map has arrays that its count does not fit");
	}

	if ((data.stages && data.stages->size() > 0xFFull))
	{
		throw std::runtime_error("clip_map has arrays that its count does not fit");
	}

	if ((data.havok_world_shape_data && data.havok_world_shape_data->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("clip_map has arrays that its count does not fit");
	}

	if ((data.collision_heatmap && data.collision_heatmap->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("clip_map has arrays that its count does not fit");
	}

}

inline void validate(const gfx_fog_spline_data& data)
{
	(void) data;
}

inline void validate(const fog_spline& data)
{
	if (!data.name)
	{
		throw std::runtime_error("fog_spline has no name");
	}

	detail::name(*data.name, "fog_spline");

	validate(data.data);

}

} // namespace world
} // namespace zonetool::asset_data
