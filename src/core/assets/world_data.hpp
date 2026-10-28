#pragma once

#include "asset_data.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

// Neutral world assets (docs/NEUTRAL_ASSETS.md), emitted by tools/gen_iw8_neutral.py from the IW8 structs and stream
// traversals: do not edit; change the spec (tools/neutral_families/world.json) or the generator.
namespace zonetool::asset_data
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

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 2> dyn_entity_list_id_names{{
	{1536, "default_dynentity_list_id"},
	{1537, "dynentity_list_id_unloading"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 30> variable_type_names{{
	{0, "undefined"},
	{1, "pointer"},
	{2, "string"},
	{3, "istring"},
	{4, "vector"},
	{5, "float"},
	{6, "integer"},
	{7, "codepos"},
	{8, "precodepos"},
	{9, "function"},
	{10, "builtin_function"},
	{11, "builtin_method"},
	{12, "stack"},
	{13, "animation"},
	{14, "developer_codepos"},
	{15, "pre_animation"},
	{16, "anim_tree"},
	{17, "thread"},
	{18, "notify_thread"},
	{19, "time_thread"},
	{20, "child_thread"},
	{21, "object"},
	{22, "dead_entity"},
	{23, "entity"},
	{24, "array"},
	{25, "dead_thread"},
	{26, "count"},
	{27, "thread_list"},
	{28, "endon_list"},
	{29, "total_count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 3> gfx_static_model_light_grid_volume_flags_names{{
	{0, "emtpy"},
	{3, "axis_mask"},
	{4, "thinpush"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 7> gfx_dynamic_lightset_g_chunk_flags_names{{
	{0, "count_empty"},
	{255, "count_mask"},
	{1792, "type_blend_count_mask"},
	{2048, "type_specialize_fast"},
	{4096, "type_specialize_slow"},
	{8192, "type_mixed_small"},
	{16384, "type_mixed_large"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 1> gfx_texture_id_names{{
	{0, "nullid"},
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

	bool operator==(const tac_vis_graph_row&) const = default;
};

struct tacpoint_search_node_t
{
	std::array<float, 2> m_mid_point{};
	std::array<float, 2> m_half_size{};
	std::uint16_t m_point_list_root{};

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

struct tactical_graph_data
{
	std::optional<std::string> m_name{};
	std::optional<std::vector<tactical_graph>> m_tac_graphs{};

	bool operator==(const tactical_graph_data&) const = default;
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

struct client_trigger_model
{
	std::uint16_t hull_count{};
	std::uint32_t first_hull{};
	std::uint32_t flags{};

	bool operator==(const client_trigger_model&) const = default;
};

struct client_trigger_hull
{
	bounds trigger_space_bounds{};
	std::uint16_t slab_count{};
	std::uint32_t first_slab{};

	bool operator==(const client_trigger_hull&) const = default;
};

struct client_map_triggers
{
	std::uint32_t count{};
	std::optional<std::vector<client_trigger_model>> models{};
	std::optional<std::vector<client_trigger_hull>> hulls{};
	std::optional<std::vector<trigger_slab>> slabs{};

	bool operator==(const client_map_triggers&) const = default;
};

struct ct_aud_rvb_pan_info
{
	bool has_custom_position{};
	std::array<float, 3> position{};
	float max_wall_distance{};
	float min_reverb_volume{};

	bool operator==(const ct_aud_rvb_pan_info&) const = default;
};

struct client_triggers
{
	client_map_triggers trigger{};
	std::optional<std::vector<char>> trigger_string{};
	std::optional<std::vector<std::int16_t>> vision_set_triggers{};
	std::optional<std::vector<std::uint16_t>> trigger_type{};
	std::optional<std::vector<std::array<float, 3>>> origins{};
	std::optional<std::vector<float>> script_delay{};
	std::optional<std::vector<float>> priority{};
	std::optional<std::vector<std::int16_t>> audio_triggers{};
	std::optional<std::vector<std::int16_t>> blend_lookup{};
	std::optional<std::vector<std::int16_t>> npc_triggers{};
	std::optional<std::vector<std::int16_t>> audio_state_ids{};
	std::optional<std::vector<ct_aud_rvb_pan_info>> audio_rvb_pan_info{};
	std::optional<std::vector<std::int16_t>> parent{};
	std::optional<spatial_partition_tree> spatial_tree{};

	bool operator==(const client_triggers&) const = default;
};

struct client_trigger_blend_node
{
	std::array<float, 3> point_a{};
	std::array<float, 3> point_b{};
	std::uint16_t trigger_a{};
	std::uint16_t trigger_b{};

	bool operator==(const client_trigger_blend_node&) const = default;
};

struct client_trigger_blend
{
	std::uint16_t num_client_trigger_blend_nodes{};
	std::optional<std::vector<client_trigger_blend_node>> blend_nodes{};

	bool operator==(const client_trigger_blend&) const = default;
};

struct spawn_point_entity_record
{
	std::uint16_t index{};
	std::optional<std::string> name{};
	std::optional<std::string> target{};
	std::optional<std::string> script_noteworthy{};
	std::array<float, 3> origin{};
	std::array<float, 3> angles{};

	bool operator==(const spawn_point_entity_record&) const = default;
};

struct spawn_point_record_list
{
	std::uint16_t spawns_count{};
	std::optional<std::vector<spawn_point_entity_record>> spawns{};

	bool operator==(const spawn_point_record_list&) const = default;
};

struct spline_point_entity_record
{
	std::int32_t spline_id{};
	std::int32_t spline_node_id{};
	std::optional<std::string> spline_node_label{};
	std::optional<std::string> targetname{};
	std::optional<std::string> target{};
	std::optional<std::string> string{};
	float speed{};
	float spline_node_tension{};
	std::array<float, 3> origin{};
	std::array<float, 3> angles{};
	float throttle{};
	std::array<float, 2> corridor_dims{};
	std::array<float, 3> tangent{};
	float dist_to_next_node{};
	std::array<std::array<float, 3>, 4> position_cubic{};
	std::array<std::array<float, 3>, 3> tangent_quadratic{};

	bool operator==(const spline_point_entity_record&) const = default;
};

struct spline_point_record_list
{
	std::uint16_t spline_point_count{};
	float spline_length{};
	std::optional<std::vector<spline_point_entity_record>> spline_points{};

	bool operator==(const spline_point_record_list&) const = default;
};

struct spline_record_list
{
	std::uint16_t spline_count{};
	std::optional<std::vector<spline_point_record_list>> splines{};

	bool operator==(const spline_record_list&) const = default;
};

struct cmodel_t
{
	bounds bounds{};
	float radius{};
	std::optional<std::string> physics_asset{};
	std::uint16_t physics_shape_override_idx{};
	std::uint16_t nav_obstacle_idx{};
	std::uint32_t edge_first_index{};
	std::uint32_t edge_total_count{};

	bool operator==(const cmodel_t&) const = default;
};

struct cm_client_model
{
	std::array<float, 3> spawn_origin{};
	std::array<float, 3> spawn_angles{};
	std::optional<std::string> model{};
	std::optional<std::string> animation_tree_name{};
	std::optional<std::string> animation{};
	std::optional<std::string> name{};
	bool no_physics{};
	bool no_cloth{};

	bool operator==(const cm_client_model&) const = default;
};

struct spatial_partition_population_node
{
	std::uint32_t spatial_node_next{};

	bool operator==(const spatial_partition_population_node&) const = default;
};

struct client_ent_anchor
{
	std::optional<std::string> name{};
	std::int32_t ent_num{};

	bool operator==(const client_ent_anchor&) const = default;
};

struct scriptable_physics_limits
{
	std::uint32_t client_standalone_main_rigid_body_count{};
	std::uint32_t client_standalone_detailed_rigid_body_count{};
	std::uint32_t client_standalone_motion_count{};
	std::uint32_t server_standalone_main_rigid_body_count{};
	std::uint32_t server_standalone_detailed_rigid_body_count{};
	std::uint32_t server_standalone_motion_count{};

	bool operator==(const scriptable_physics_limits&) const = default;
};

struct scriptable_instance
{
	std::optional<std::string> initial_def{};
	std::array<float, 3> initial_origin{};
	std::array<float, 3> initial_angles{};
	std::optional<std::string> classname{};
	std::optional<std::string> target{};
	std::optional<std::string> targetname{};
	std::optional<std::string> script_linkname{};
	std::optional<std::string> script_linkto{};
	std::optional<std::string> script_noteworthy{};
	std::array<std::uint16_t, 5> light_indices{};
	std::uint16_t ff_mem_cost{};
	std::uint16_t map_ent_lookup{};
	std::uint8_t flags{};

	bool operator==(const scriptable_instance&) const = default;
};

struct scriptable_event_sun_data
{
	std::int32_t start_time{};
	std::int32_t trans_time{};
	std::array<float, 3> start_color_linear_srgb{};
	float start_intensity{};
	float target_intensity{};
	std::array<float, 3> start_angles{};
	std::array<float, 3> target_angles{};

	bool operator==(const scriptable_event_sun_data&) const = default;
};

struct scriptable_reserved_dynent
{
	std::uint32_t dynent_id{};
	std::int16_t next{};

	bool operator==(const scriptable_reserved_dynent&) const = default;
};

struct scriptable_reserved_dynents
{
	std::int16_t free_head{};
	std::optional<std::vector<scriptable_reserved_dynent>> reserved_dynents{};

	bool operator==(const scriptable_reserved_dynents&) const = default;
};

struct spatial_partition_population_partition
{
	float dist{};
	std::uint32_t axis{};
	std::uint32_t right_child{};

	bool operator==(const spatial_partition_population_partition&) const = default;
};

struct extent_bounds
{
	std::array<float, 3> mins{};
	std::array<float, 3> maxs{};

	bool operator==(const extent_bounds&) const = default;
};

struct spatial_partition_population_tree
{
	std::optional<std::vector<spatial_partition_population_partition>> partitions{};
	std::optional<std::vector<extent_bounds>> extents{};

	bool operator==(const spatial_partition_population_tree&) const = default;
};

struct scriptable_map_ents
{
	std::uint32_t total_instance_count{};
	std::uint32_t reserved_instance_count{};
	std::uint32_t loot_instance_count{};
	std::uint32_t server_instance_count{};
	std::uint32_t server_fixed_part_count{};
	std::uint32_t server_usable_part_count{};
	scriptable_physics_limits physics_limits{};
	std::optional<std::vector<scriptable_instance>> map_instances{};
	std::array<scriptable_event_sun_data, 2> sun_client_datas{};
	std::array<scriptable_reserved_dynents, 2> reserved_dynents{};
	std::optional<spatial_partition_population_tree> spatial_population_tree{};
	std::uint32_t ff_mem_cost{};

	bool operator==(const scriptable_map_ents&) const = default;
};

struct spawn_group_point
{
	std::array<float, 3> origin{};
	std::array<float, 3> angles{};
	std::uint16_t cluster{};
	std::uint16_t zone{};

	bool operator==(const spawn_group_point&) const = default;
};

struct spawn_group_cluster
{
	std::uint16_t zone{};
	std::optional<std::vector<std::uint16_t>> points{};
	bounds bounds{};

	bool operator==(const spawn_group_cluster&) const = default;
};

struct spawn_group_zone
{
	std::optional<std::string> name{};
	std::optional<std::vector<std::uint16_t>> points{};
	std::optional<std::vector<std::uint16_t>> clusters{};
	bounds bounds{};

	bool operator==(const spawn_group_zone&) const = default;
};

struct spawn_group
{
	std::optional<std::vector<spawn_group_point>> points{};
	std::optional<std::vector<spawn_group_cluster>> clusters{};
	std::optional<std::vector<spawn_group_zone>> zones{};

	bool operator==(const spawn_group&) const = default;
};

struct fx_combined_def
{
	std::optional<std::string> particle_system_def{};

	bool operator==(const fx_combined_def&) const = default;
};

struct snd_alias_lookup
{
	std::optional<std::string> name{};

	bool operator==(const snd_alias_lookup&) const = default;
};

struct snd_alias
{
	std::optional<std::string> alias_name{};
	std::optional<std::string> subtitle{};
	std::optional<std::string> secondary_alias_name{};
	std::optional<std::string> asset_file_name{};
	std::uint32_t id{};
	std::uint32_t secondary_id{};
	std::uint32_t asset_id{};
	std::uint32_t context_type{};
	std::uint32_t context_value{};
	std::uint32_t context2_type{};
	std::uint32_t context2_value{};
	std::uint32_t duck{};
	std::uint64_t surface_flags{};
	std::int32_t sequence{};
	float vol_min{};
	float vol_max{};
	std::int32_t vol_mod_index{};
	float pitch_min{};
	float pitch_max{};
	float donut_fade_end{};
	float dist_min{};
	float dist_max{};
	float velocity_min{};
	std::int32_t flags{};
	float probability{};
	float lfe_percentage{};
	float center_percentage{};
	std::int32_t start_delay{};
	std::int32_t start_delay_max{};
	std::uint32_t volume_falloff_curve{};
	std::uint32_t lpf_curve{};
	std::uint32_t hpf_curve{};
	std::uint32_t reverb_send_curve{};
	float envelop_min{};
	float envelop_max{};
	float envelop_percentage{};
	std::uint32_t speaker_map{};
	float reverb_multiplier{};
	float far_reverb_multiplier{};
	std::uint32_t occlusion_shape{};
	std::uint32_t doppler_preset{};
	float smart_pan_distance2d{};
	float smart_pan_distance3d{};
	float smart_pan_attenuation2d{};
	float stereo_spread_min_dist{};
	float stereo_spread_max_dist{};
	float stereo_spread_mid_point{};
	std::uint8_t stereo_spread_max_angle{};
	std::uint8_t master_priority{};
	float master_percentage{};
	float master_slave_percentage{};
	float master_threshold{};
	std::int16_t max_duration_dsec{};

	bool operator==(const snd_alias&) const = default;
};

struct snd_alias_list
{
	std::optional<std::string> alias_name{};
	std::uint32_t id{};
	std::optional<std::vector<snd_alias>> head{};
	std::int32_t sequence{};

	bool operator==(const snd_alias_list&) const = default;
};

struct client_oneshot_effect_def
{
	std::array<float, 3> origin{};
	std::array<float, 3> angles{};
	float radius{};
	std::int32_t delay_msec{};
	fx_combined_def effect{};
	snd_alias_lookup effect_sound{};
	std::optional<snd_alias_list> alias_list{};

	bool operator==(const client_oneshot_effect_def&) const = default;
};

struct client_exploder_def
{
	std::array<float, 3> origin{};
	std::array<float, 3> angles{};
	std::int32_t delay_msec{};
	fx_combined_def effect{};
	snd_alias_lookup effect_sound{};
	std::optional<snd_alias_list> alias_list{};
	std::optional<std::string> name{};

	bool operator==(const client_exploder_def&) const = default;
};

struct client_interval_sound_def
{
	std::array<float, 3> origin{};
	std::array<float, 3> angles{};
	snd_alias_lookup effect_sound{};
	std::optional<snd_alias_list> alias_list{};
	std::int32_t delay_min_msec{};
	std::int32_t delay_max_msec{};

	bool operator==(const client_interval_sound_def&) const = default;
};

struct client_loop_sound_def
{
	std::array<float, 3> origin{};
	std::array<float, 3> angles{};
	snd_alias_lookup effect_sound{};
	std::optional<snd_alias_list> alias_list{};

	bool operator==(const client_loop_sound_def&) const = default;
};

struct client_reactive_ent_def
{
	std::array<float, 3> origin{};
	std::array<float, 3> angles{};
	fx_combined_def effect{};
	snd_alias_lookup effect_sound{};
	std::optional<snd_alias_list> alias_list{};
	float radius{};

	bool operator==(const client_reactive_ent_def&) const = default;
};

struct client_side_effects
{
	std::uint32_t oneshot_effect_count{};
	std::optional<std::vector<client_oneshot_effect_def>> oneshot_effects{};
	std::optional<spatial_partition_tree> oneshot_effects_tree{};
	std::optional<std::vector<std::uint32_t>> oneshot_sound_indices{};
	std::optional<std::vector<client_exploder_def>> exploders{};
	std::optional<std::vector<client_interval_sound_def>> interval_sounds{};
	std::optional<std::vector<client_loop_sound_def>> loop_sounds{};
	std::optional<spatial_partition_tree> sounds_spatial_tree{};
	std::optional<std::vector<client_reactive_ent_def>> reactive_ents{};
	std::optional<spatial_partition_tree> reactive_spatial_tree{};

	bool operator==(const client_side_effects&) const = default;
};

struct create_fx_palette_def
{
	std::optional<std::string> alias_name{};
	std::optional<std::string> effect_path{};
	std::uint16_t ref_count{};

	bool operator==(const create_fx_palette_def&) const = default;
};

struct create_fx_asset_data
{
	std::uint16_t fx_palette_count{};
	std::optional<std::vector<create_fx_palette_def>> fx_palette{};

	bool operator==(const create_fx_asset_data&) const = default;
};

struct server_exploder_def
{
	std::array<float, 3> origin{};
	std::int32_t delay_msec{};
	float damage_amount{};
	float damage_radius{};
	std::optional<std::string> name{};
	std::optional<std::string> earthquake_name{};
	std::optional<std::string> rumble_name{};
	bool damage_environment_only{};
	bool damage_do_occlusion_traces{};

	bool operator==(const server_exploder_def&) const = default;
};

struct server_side_effects
{
	std::optional<std::vector<server_exploder_def>> exploders{};

	bool operator==(const server_side_effects&) const = default;
};

struct gfx_placement
{
	std::array<float, 4> quat{};
	std::array<float, 3> origin{};

	bool operator==(const gfx_placement&) const = default;
};

struct client_entity_link_to_def
{
	std::int32_t anchor_index{};
	std::array<float, 3> origin_offset{};
	std::array<float, 3> angle_offset{};

	bool operator==(const client_entity_link_to_def&) const = default;
};

struct mayhem_instance
{
	gfx_placement initial_pose{};
	std::optional<std::string> mayhem{};
	std::optional<client_entity_link_to_def> link_to{};
	std::optional<std::string> script_name{};
	float cur_time{};
	float old_time{};
	std::uint32_t initial_flags{};
	std::uint32_t flags{};
	std::uint32_t map_ent_lookup{};
	std::uint16_t transient_index_stored{};

	bool operator==(const mayhem_instance&) const = default;
};

struct spawner_field
{
	std::optional<std::string> key{};
	std::uint32_t key_canonical{};
	std::optional<std::string> value{};
	std::string type{};

	bool operator==(const spawner_field&) const = default;
};

struct spawner
{
	std::array<float, 3> origin{};
	std::array<float, 3> angles{};
	std::optional<std::vector<spawner_field>> fields{};
	std::int32_t flags{};
	std::int32_t count{};
	std::optional<std::string> targetname{};
	std::optional<std::string> classname{};

	bool operator==(const spawner&) const = default;
};

struct spawner_list
{
	std::uint32_t spawner_count{};
	std::optional<std::vector<spawner>> spanwer_list{};

	bool operator==(const spawner_list&) const = default;
};

struct audio_pa_speaker
{
	std::array<float, 3> origin{};
	std::uint32_t name_hash{};
	std::uint32_t ent_channel_hash{};

	bool operator==(const audio_pa_speaker&) const = default;
};

struct audio_propagation_node
{
	std::array<float, 3> position{};
	std::uint16_t num_edges{};
	std::uint32_t edge_start_index{};

	bool operator==(const audio_propagation_node&) const = default;
};

struct audio_propagation_edge
{
	std::int16_t neighbor{};
	std::int16_t portal_index{};

	bool operator==(const audio_propagation_edge&) const = default;
};

struct coll_map_lookup_entry
{
	std::optional<std::string> target_name{};
	std::int32_t brush_index{};

	bool operator==(const coll_map_lookup_entry&) const = default;
};

struct map_ents
{
	std::optional<std::string> name{};
	std::optional<std::vector<char>> entity_string{};
	map_triggers trigger{};
	client_triggers client_trigger{};
	client_trigger_blend client_trigger_blend{};
	spawn_point_record_list spawn_list{};
	spline_record_list spline_list{};
	std::optional<std::vector<char>> havok_ents_shape_data{};
	std::optional<std::vector<cmodel_t>> cmodels{};
	std::uint32_t edge_list_used_query_types{};
	std::uint32_t num_edge_lists{};
	std::optional<spatial_partition_tree> edge_list_spatial_tree{};
	std::optional<std::vector<cm_client_model>> client_models{};
	std::array<std::uint32_t, 2> dyn_ent_count{};
	std::uint32_t dyn_ent_count_total{};
	std::string dyn_entity_lists_count{};
	std::array<std::array<std::uint16_t, 2>, 2> dyn_ent_physics_setup_head{};
	std::array<std::array<std::uint16_t, 2>, 2> dyn_ent_physics_setup_tail{};
	std::array<std::uint16_t, 2> dyn_ent_no_spatial_count{};
	std::uint16_t dyn_ent_max_client_history_count{};
	std::uint8_t dyn_ent_max_pose_part_count{};
	std::uint32_t dyn_ents_with_extra_pose_parts_count{};
	spatial_partition_population_node dyn_ent_no_spatial_list{};
	std::optional<std::vector<client_ent_anchor>> client_ent_anchors{};
	scriptable_map_ents scriptable_map_ents{};
	spawn_group spawn_group_loot{};
	client_side_effects client_side_effects{};
	create_fx_asset_data create_fx_asset_data{};
	std::optional<std::vector<std::optional<std::string>>> exploder_names{};
	server_side_effects server_side_effects{};
	std::uint32_t create_fx_effect_total{};
	std::optional<std::vector<mayhem_instance>> mayhem_scenes{};
	spawner_list spawners{};
	std::optional<std::vector<audio_pa_speaker>> audio_pa_speakers{};
	std::optional<std::vector<audio_propagation_node>> audio_prop_nodes{};
	std::optional<std::vector<audio_propagation_edge>> audio_prop_edges{};
	std::optional<std::vector<coll_map_lookup_entry>> collmap_lookups{};

	bool operator==(const map_ents&) const = default;
};

struct gfx_buffer_resource
{
	std::uint64_t gpu_address{};
	std::uint64_t allocation{};
	std::uint32_t size{};

	bool operator==(const gfx_buffer_resource&) const = default;
};

struct gfx_shader_buffer_view
{
	std::uint32_t view{};

	bool operator==(const gfx_shader_buffer_view&) const = default;
};

struct gfx_wrapped_base_buffer
{
	gfx_buffer_resource buffer{};
	gfx_shader_buffer_view view{};

	bool operator==(const gfx_wrapped_base_buffer&) const = default;
};

struct gfx_wrapped_buffer
{
	gfx_wrapped_base_buffer base{};
	std::uint8_t flags{};

	bool operator==(const gfx_wrapped_buffer&) const = default;
};

struct gfx_index_buffer
{
	std::uint64_t gpu_address{};
	std::uint64_t allocation{};
	std::uint32_t size{};

	bool operator==(const gfx_index_buffer&) const = default;
};

struct gfx_world_draw_verts
{
	std::uint32_t pos_data_size{};
	std::optional<std::vector<std::uint8_t>> pos_data{};
	std::optional<std::vector<std::uint8_t>> aux_data{};
	gfx_wrapped_buffer pos_buffer{};
	gfx_wrapped_buffer aux_buffer{};
	std::optional<std::vector<std::uint16_t>> indices{};
	gfx_index_buffer index_buffer{};

	bool operator==(const gfx_world_draw_verts&) const = default;
};

struct gfx_cell_tree_count
{
	std::int32_t aabb_tree_count{};

	bool operator==(const gfx_cell_tree_count&) const = default;
};

struct gfx_cell_tree
{

	bool operator==(const gfx_cell_tree&) const = default;
};

struct gfx_world_draw_cells
{
	std::uint32_t cell_count{};
	std::optional<std::vector<gfx_cell_tree_count>> aabb_tree_counts{};
	std::optional<std::vector<gfx_cell_tree>> aabb_trees{};

	bool operator==(const gfx_world_draw_cells&) const = default;
};

struct gfx_sh_compressed_probe_data
{
	std::array<std::uint32_t, 8> data{};

	bool operator==(const gfx_sh_compressed_probe_data&) const = default;
};

struct gfx_shader_buffer_rw_view
{
	std::uint64_t gpu_address{};
	std::uint64_t allocation{};
	std::uint32_t view{};

	bool operator==(const gfx_shader_buffer_rw_view&) const = default;
};

struct gfx_wrapped_rw_buffer
{
	gfx_wrapped_buffer buffer{};
	gfx_shader_buffer_rw_view rw_view{};

	bool operator==(const gfx_wrapped_rw_buffer&) const = default;
};

struct gfx_sh_probe_data
{
	std::array<std::uint16_t, 29> coeffs{};

	bool operator==(const gfx_sh_probe_data&) const = default;
};

struct gfx_gpu_light_grid_zone
{
	std::uint32_t num_probes{};
	std::uint32_t first_probe{};
	std::uint32_t num_tetrahedrons{};
	std::uint32_t first_tetrahedron{};
	std::uint32_t first_voxel_tetrahedron_index{};
	std::uint32_t voxel_tetrahedron_internal_node_shift{};
	std::uint32_t num_voxel_tetrahedron_indices{};
	gfx_sh_probe_data fallback_probe_data{};

	bool operator==(const gfx_gpu_light_grid_zone&) const = default;
};

struct gfx_voxel_tree_header
{
	std::array<std::int32_t, 4> root_node_dimension{};
	std::array<std::int32_t, 4> node_coord_bit_shift{};
	std::array<float, 4> bound_min{};
	std::array<float, 4> bound_max{};

	bool operator==(const gfx_voxel_tree_header&) const = default;
};

struct gfx_voxel_top_down_view_node
{
	std::int32_t first_node_index{};
	std::int32_t z_min{};
	std::int32_t z_max{};

	bool operator==(const gfx_voxel_top_down_view_node&) const = default;
};

struct gfx_voxel_internal_node
{
	std::array<std::int32_t, 2> first_node_index{};
	std::array<std::int32_t, 2> child_node_mask{};

	bool operator==(const gfx_voxel_internal_node&) const = default;
};

struct gfx_gpu_light_grid_voxel_tree
{
	std::int32_t top_down_view_node_count{};
	std::optional<gfx_voxel_tree_header> tree_header{};
	std::optional<std::vector<gfx_voxel_top_down_view_node>> top_down_view_nodes{};
	std::optional<std::vector<gfx_voxel_internal_node>> internal_nodes{};
	gfx_wrapped_buffer tree_header_buffer{};
	gfx_wrapped_buffer top_down_view_node_buffer{};
	gfx_wrapped_buffer internal_node_buffer{};

	bool operator==(const gfx_gpu_light_grid_voxel_tree&) const = default;
};

struct gfx_gpu_light_grid_tetrahedron
{
	std::array<std::uint32_t, 4> index_flags{};

	bool operator==(const gfx_gpu_light_grid_tetrahedron&) const = default;
};

struct gfx_gpu_light_grid_tetrahedron_neighbors
{
	std::array<std::uint32_t, 4> neighbors{};

	bool operator==(const gfx_gpu_light_grid_tetrahedron_neighbors&) const = default;
};

struct gfx_gpu_light_grid_tetrahedron_visibility
{
	std::array<std::uint32_t, 16> visibility{};

	bool operator==(const gfx_gpu_light_grid_tetrahedron_visibility&) const = default;
};

struct gfx_gpu_light_grid_voxel_start_tetrahedron
{
	std::uint32_t index{};

	bool operator==(const gfx_gpu_light_grid_voxel_start_tetrahedron&) const = default;
};

struct gfx_gpu_light_grid
{
	std::uint32_t probe_count{};
	std::optional<std::vector<gfx_sh_compressed_probe_data>> probes{};
	gfx_wrapped_rw_buffer probes_buffer{};
	std::optional<std::vector<std::array<float, 3>>> probe_positions{};
	gfx_wrapped_buffer probe_positions_buffer{};
	bool fallback_probe_use_custom_sample{};
	std::array<float, 3> fallback_probe_custom_sample_pos{};
	std::optional<std::vector<gfx_gpu_light_grid_zone>> zones{};
	std::optional<std::vector<gfx_gpu_light_grid_voxel_tree>> zone_voxel_trees{};
	std::optional<std::vector<gfx_gpu_light_grid_tetrahedron>> tetrahedrons{};
	gfx_wrapped_buffer tetrahedron_buffer{};
	std::optional<std::vector<gfx_gpu_light_grid_tetrahedron_neighbors>> tetrahedron_neighbors{};
	gfx_wrapped_buffer tetrahedron_neighbors_buffer{};
	std::optional<std::vector<gfx_gpu_light_grid_tetrahedron_visibility>> tetrahedron_visibility{};
	gfx_wrapped_buffer tetrahedron_visibility_buffer{};
	std::optional<std::vector<gfx_gpu_light_grid_voxel_start_tetrahedron>> voxel_start_tetrahedron{};
	gfx_wrapped_buffer voxel_start_tetrahedron_buffer{};
	std::array<std::array<float, 4>, 7> bounding_volume{};

	bool operator==(const gfx_gpu_light_grid&) const = default;
};

struct gfx_static_model_light_grid_volume
{
	std::uint32_t first_probe{};
	std::array<std::uint16_t, 3> m_volume_size{};
	std::string flags{};
	std::uint32_t single_probe{};
	std::array<std::array<float, 4>, 3> volume_transform{};

	bool operator==(const gfx_static_model_light_grid_volume&) const = default;
};

struct gfx_gpu_dyn_lightsets_lgv_reference
{
	std::uint32_t volume_index{};
	std::uint32_t volume_probe_index{};

	bool operator==(const gfx_gpu_dyn_lightsets_lgv_reference&) const = default;
};

struct gfx_dynamic_lightset_g_chunk_info
{
	std::uint32_t num_records{};
	std::uint32_t bf_start{};
	std::uint32_t org_start{};
	std::uint32_t tmp_start{};

	bool operator==(const gfx_dynamic_lightset_g_chunk_info&) const = default;
};

struct gfx_dynamic_lightset_g_chunk
{
	gfx_dynamic_lightset_g_chunk_info chunk_info{};
	std::string flags{};
	std::uint16_t which_lm{};
	std::uint32_t bf_chunk_start{};

	bool operator==(const gfx_dynamic_lightset_g_chunk&) const = default;
};

struct gfx_dynamic_lightset_probe_data
{
	std::uint32_t num_g_chunk{};
	std::optional<std::vector<gfx_dynamic_lightset_g_chunk>> chunk_g{};
	std::optional<std::vector<std::uint16_t>> chunk_gbf{};
	std::uint32_t orig_start{};
	std::optional<std::vector<gfx_sh_compressed_probe_data>> orig_probes{};
	gfx_wrapped_buffer orig_probes_buffer{};
	std::optional<std::vector<gfx_sh_compressed_probe_data>> bf_probes{};
	gfx_wrapped_buffer bf_probes_buffer{};
	std::optional<std::vector<std::uint16_t>> skip_records{};
	gfx_wrapped_buffer skip_records_buffer{};
	std::optional<std::vector<std::uint32_t>> pallette_records{};
	gfx_wrapped_buffer pallette_records_buffer{};
	std::optional<std::vector<std::uint32_t>> temp_remap{};
	gfx_wrapped_buffer temp_remap_buffer{};

	bool operator==(const gfx_dynamic_lightset_probe_data&) const = default;
};

struct gfx_dynamic_lightset_l_gpp_zone_info
{
	std::uint32_t num_orig_sml_gpp_probes{};
	std::optional<std::vector<gfx_gpu_dyn_lightsets_lgv_reference>> lgv_references{};
	gfx_wrapped_buffer lgv_references_buffer{};
	gfx_dynamic_lightset_probe_data lgv_probe_data{};

	bool operator==(const gfx_dynamic_lightset_l_gpp_zone_info&) const = default;
};

struct gfx_gpu_light_grid_transient_data
{
	std::optional<gfx_gpu_light_grid> gpu_light_grid{};
	std::optional<std::vector<gfx_static_model_light_grid_volume>> static_model_volumes{};
	std::optional<gfx_dynamic_lightset_l_gpp_zone_info> sml_gpp_zone{};

	bool operator==(const gfx_gpu_light_grid_transient_data&) const = default;
};

struct gfx_image_reflection_probe
{
	std::uint32_t total_size{};
	std::uint32_t element_count{};
	std::string texture_id{};
	std::optional<std::vector<std::uint8_t>> pixels{};

	bool operator==(const gfx_image_reflection_probe&) const = default;
};

struct gfx_reflection_probe_transient_data
{
	std::uint32_t exclusive_probe_count{};
	std::optional<std::vector<std::uint16_t>> reflection_probe_instances{};
	std::optional<std::vector<gfx_image_reflection_probe>> exclusive_probe_buckets{};

	bool operator==(const gfx_reflection_probe_transient_data&) const = default;
};

struct gfx_static_decal_volume_packed
{
	std::array<float, 3> obb_center{};
	std::uint32_t obb_quat{};
	std::uint32_t obb_half_size{};
	std::uint32_t opacity_params{};
	std::array<std::uint32_t, 3> uv_matrix_and_offset{};
	std::uint32_t packed_material_info{};

	bool operator==(const gfx_static_decal_volume_packed&) const = default;
};

struct gfx_static_decal_volume_uid
{
	std::uint32_t uid{};

	bool operator==(const gfx_static_decal_volume_uid&) const = default;
};

struct gfx_decal_volume_transient_data
{
	std::uint32_t decal_volume_count{};
	std::uint16_t first_collection{};
	std::optional<std::vector<gfx_static_decal_volume_packed>> decal_volumes{};
	gfx_wrapped_buffer decal_volumes_buffer{};
	std::optional<std::vector<bounds>> collection_bounds{};
	std::optional<std::vector<gfx_static_decal_volume_uid>> decal_volume_uids{};

	bool operator==(const gfx_decal_volume_transient_data&) const = default;
};

struct gfx_transient_zone
{
	std::optional<std::string> name{};
	std::uint32_t transient_zone_index{};
	gfx_world_draw_verts draw_verts{};
	gfx_world_draw_cells draw_cells{};
	gfx_gpu_light_grid_transient_data gpu_light_grid{};
	std::optional<gfx_reflection_probe_transient_data> reflection_probes{};
	std::optional<std::vector<std::uint16_t>> collections{};
	std::optional<gfx_decal_volume_transient_data> decal_volumes{};

	bool operator==(const gfx_transient_zone&) const = default;
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

inline void validate(const tacpoint_search_node_t& data)
{
	(void) data;
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

inline void validate(const tactical_graph_data& data)
{
	if (!data.m_name)
	{
		throw std::runtime_error("tactical_graph_data has no name");
	}

	detail::name(*data.m_name, "tactical_graph_data");

	if (data.m_tac_graphs)
	{
		for (const auto& item : *data.m_tac_graphs)
		{
			validate(item);
		}
	}

	if ((data.m_tac_graphs && data.m_tac_graphs->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("tactical_graph_data has arrays that its count does not fit");
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

inline void validate(const client_trigger_model& data)
{
	(void) data;
}

inline void validate(const client_trigger_hull& data)
{
	validate(data.trigger_space_bounds);

}

inline void validate(const client_map_triggers& data)
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

	if (static_cast<std::int64_t>(data.count) != (data.models ? static_cast<std::int64_t>(data.models->size()) : 0))
	{
		throw std::runtime_error("client_map_triggers.models has a length that its count does not give");
	}

	if ((data.hulls && data.hulls->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("client_map_triggers has arrays that its count does not fit");
	}

	if ((data.slabs && data.slabs->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("client_map_triggers has arrays that its count does not fit");
	}

}

inline void validate(const ct_aud_rvb_pan_info& data)
{
	(void) data;
}

inline void validate(const client_triggers& data)
{
	validate(data.trigger);

	if (data.audio_rvb_pan_info)
	{
		for (const auto& item : *data.audio_rvb_pan_info)
		{
			validate(item);
		}
	}

	if (data.spatial_tree)
	{
		validate(*data.spatial_tree);
	}

	if (static_cast<std::int64_t>(data.trigger.count) != (data.vision_set_triggers ? static_cast<std::int64_t>(data.vision_set_triggers->size()) : 0))
	{
		throw std::runtime_error("client_triggers.vision_set_triggers has a length that its count does not give");
	}

	if (static_cast<std::int64_t>(data.trigger.count) != (data.trigger_type ? static_cast<std::int64_t>(data.trigger_type->size()) : 0))
	{
		throw std::runtime_error("client_triggers.trigger_type has a length that its count does not give");
	}

	if (static_cast<std::int64_t>(data.trigger.count) != (data.origins ? static_cast<std::int64_t>(data.origins->size()) : 0))
	{
		throw std::runtime_error("client_triggers.origins has a length that its count does not give");
	}

	if (static_cast<std::int64_t>(data.trigger.count) != (data.script_delay ? static_cast<std::int64_t>(data.script_delay->size()) : 0))
	{
		throw std::runtime_error("client_triggers.script_delay has a length that its count does not give");
	}

	if (static_cast<std::int64_t>(data.trigger.count) != (data.priority ? static_cast<std::int64_t>(data.priority->size()) : 0))
	{
		throw std::runtime_error("client_triggers.priority has a length that its count does not give");
	}

	if (static_cast<std::int64_t>(data.trigger.count) != (data.audio_triggers ? static_cast<std::int64_t>(data.audio_triggers->size()) : 0))
	{
		throw std::runtime_error("client_triggers.audio_triggers has a length that its count does not give");
	}

	if (static_cast<std::int64_t>(data.trigger.count) != (data.blend_lookup ? static_cast<std::int64_t>(data.blend_lookup->size()) : 0))
	{
		throw std::runtime_error("client_triggers.blend_lookup has a length that its count does not give");
	}

	if (static_cast<std::int64_t>(data.trigger.count) != (data.npc_triggers ? static_cast<std::int64_t>(data.npc_triggers->size()) : 0))
	{
		throw std::runtime_error("client_triggers.npc_triggers has a length that its count does not give");
	}

	if (static_cast<std::int64_t>(data.trigger.count) != (data.audio_state_ids ? static_cast<std::int64_t>(data.audio_state_ids->size()) : 0))
	{
		throw std::runtime_error("client_triggers.audio_state_ids has a length that its count does not give");
	}

	if (static_cast<std::int64_t>(data.trigger.count) != (data.audio_rvb_pan_info ? static_cast<std::int64_t>(data.audio_rvb_pan_info->size()) : 0))
	{
		throw std::runtime_error("client_triggers.audio_rvb_pan_info has a length that its count does not give");
	}

	if (static_cast<std::int64_t>(data.trigger.count) != (data.parent ? static_cast<std::int64_t>(data.parent->size()) : 0))
	{
		throw std::runtime_error("client_triggers.parent has a length that its count does not give");
	}

	if ((data.trigger_string && data.trigger_string->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("client_triggers has arrays that its count does not fit");
	}

}

inline void validate(const client_trigger_blend_node& data)
{
	(void) data;
}

inline void validate(const client_trigger_blend& data)
{
	if (data.blend_nodes)
	{
		for (const auto& item : *data.blend_nodes)
		{
			validate(item);
		}
	}

	if (static_cast<std::int64_t>(data.num_client_trigger_blend_nodes) != (data.blend_nodes ? static_cast<std::int64_t>(data.blend_nodes->size()) : 0))
	{
		throw std::runtime_error("client_trigger_blend.blend_nodes has a length that its count does not give");
	}

}

inline void validate(const spawn_point_entity_record& data)
{
	(void) data;
}

inline void validate(const spawn_point_record_list& data)
{
	if (data.spawns)
	{
		for (const auto& item : *data.spawns)
		{
			validate(item);
		}
	}

	if (static_cast<std::int64_t>(data.spawns_count) != (data.spawns ? static_cast<std::int64_t>(data.spawns->size()) : 0))
	{
		throw std::runtime_error("spawn_point_record_list.spawns has a length that its count does not give");
	}

}

inline void validate(const spline_point_entity_record& data)
{
	(void) data;
}

inline void validate(const spline_point_record_list& data)
{
	if (data.spline_points)
	{
		for (const auto& item : *data.spline_points)
		{
			validate(item);
		}
	}

	if (static_cast<std::int64_t>(data.spline_point_count) != (data.spline_points ? static_cast<std::int64_t>(data.spline_points->size()) : 0))
	{
		throw std::runtime_error("spline_point_record_list.spline_points has a length that its count does not give");
	}

}

inline void validate(const spline_record_list& data)
{
	if (data.splines)
	{
		for (const auto& item : *data.splines)
		{
			validate(item);
		}
	}

	if (static_cast<std::int64_t>(data.spline_count) != (data.splines ? static_cast<std::int64_t>(data.splines->size()) : 0))
	{
		throw std::runtime_error("spline_record_list.splines has a length that its count does not give");
	}

}

inline void validate(const cmodel_t& data)
{
	validate(data.bounds);

}

inline void validate(const cm_client_model& data)
{
	(void) data;
}

inline void validate(const spatial_partition_population_node& data)
{
	(void) data;
}

inline void validate(const client_ent_anchor& data)
{
	(void) data;
}

inline void validate(const scriptable_physics_limits& data)
{
	(void) data;
}

inline void validate(const scriptable_instance& data)
{
	(void) data;
}

inline void validate(const scriptable_event_sun_data& data)
{
	(void) data;
}

inline void validate(const scriptable_reserved_dynent& data)
{
	(void) data;
}

inline void validate(const scriptable_reserved_dynents& data)
{
	if (data.reserved_dynents)
	{
		for (const auto& item : *data.reserved_dynents)
		{
			validate(item);
		}
	}

	if ((data.reserved_dynents && data.reserved_dynents->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("scriptable_reserved_dynents has arrays that its count does not fit");
	}

}

inline void validate(const spatial_partition_population_partition& data)
{
	(void) data;
}

inline void validate(const extent_bounds& data)
{
	(void) data;
}

inline void validate(const spatial_partition_population_tree& data)
{
	if (data.partitions)
	{
		for (const auto& item : *data.partitions)
		{
			validate(item);
		}
	}

	if (data.extents)
	{
		for (const auto& item : *data.extents)
		{
			validate(item);
		}
	}

	if ((data.partitions && data.partitions->size() > 0xFFFFFFFFull) || (data.extents && data.extents->size() > 0xFFFFFFFFull) || (!data.partitions != !data.extents) || (data.partitions && data.extents && data.partitions->size() != data.extents->size()))
	{
		throw std::runtime_error("spatial_partition_population_tree has arrays that its count does not fit");
	}

}

inline void validate(const scriptable_map_ents& data)
{
	validate(data.physics_limits);

	if (data.map_instances)
	{
		for (const auto& item : *data.map_instances)
		{
			validate(item);
		}
	}

	for (const auto& item : data.sun_client_datas)
	{
		validate(item);
	}

	for (const auto& item : data.reserved_dynents)
	{
		validate(item);
	}

	if (data.spatial_population_tree)
	{
		validate(*data.spatial_population_tree);
	}

	if ((data.map_instances && data.map_instances->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("scriptable_map_ents has arrays that its count does not fit");
	}

}

inline void validate(const spawn_group_point& data)
{
	(void) data;
}

inline void validate(const spawn_group_cluster& data)
{
	validate(data.bounds);

	if ((data.points && data.points->size() > 0xFFFFull))
	{
		throw std::runtime_error("spawn_group_cluster has arrays that its count does not fit");
	}

}

inline void validate(const spawn_group_zone& data)
{
	validate(data.bounds);

	if ((data.points && data.points->size() > 0xFFFFull))
	{
		throw std::runtime_error("spawn_group_zone has arrays that its count does not fit");
	}

	if ((data.clusters && data.clusters->size() > 0xFFFFull))
	{
		throw std::runtime_error("spawn_group_zone has arrays that its count does not fit");
	}

}

inline void validate(const spawn_group& data)
{
	if (data.points)
	{
		for (const auto& item : *data.points)
		{
			validate(item);
		}
	}

	if (data.clusters)
	{
		for (const auto& item : *data.clusters)
		{
			validate(item);
		}
	}

	if (data.zones)
	{
		for (const auto& item : *data.zones)
		{
			validate(item);
		}
	}

	if ((data.points && data.points->size() > 0xFFFFull))
	{
		throw std::runtime_error("spawn_group has arrays that its count does not fit");
	}

	if ((data.clusters && data.clusters->size() > 0xFFFFull))
	{
		throw std::runtime_error("spawn_group has arrays that its count does not fit");
	}

	if ((data.zones && data.zones->size() > 0xFFFFull))
	{
		throw std::runtime_error("spawn_group has arrays that its count does not fit");
	}

}

inline void validate(const fx_combined_def& data)
{
	(void) data;
}

inline void validate(const snd_alias_lookup& data)
{
	(void) data;
}

inline void validate(const snd_alias& data)
{
	(void) data;
}

inline void validate(const snd_alias_list& data)
{
	if (data.head)
	{
		for (const auto& item : *data.head)
		{
			validate(item);
		}
	}

	if ((data.head && data.head->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("snd_alias_list has arrays that its count does not fit");
	}

}

inline void validate(const client_oneshot_effect_def& data)
{
	validate(data.effect);

	validate(data.effect_sound);

	if (data.alias_list)
	{
		validate(*data.alias_list);
	}

}

inline void validate(const client_exploder_def& data)
{
	validate(data.effect);

	validate(data.effect_sound);

	if (data.alias_list)
	{
		validate(*data.alias_list);
	}

}

inline void validate(const client_interval_sound_def& data)
{
	validate(data.effect_sound);

	if (data.alias_list)
	{
		validate(*data.alias_list);
	}

}

inline void validate(const client_loop_sound_def& data)
{
	validate(data.effect_sound);

	if (data.alias_list)
	{
		validate(*data.alias_list);
	}

}

inline void validate(const client_reactive_ent_def& data)
{
	validate(data.effect);

	validate(data.effect_sound);

	if (data.alias_list)
	{
		validate(*data.alias_list);
	}

}

inline void validate(const client_side_effects& data)
{
	if (data.oneshot_effects)
	{
		for (const auto& item : *data.oneshot_effects)
		{
			validate(item);
		}
	}

	if (data.oneshot_effects_tree)
	{
		validate(*data.oneshot_effects_tree);
	}

	if (data.exploders)
	{
		for (const auto& item : *data.exploders)
		{
			validate(item);
		}
	}

	if (data.interval_sounds)
	{
		for (const auto& item : *data.interval_sounds)
		{
			validate(item);
		}
	}

	if (data.loop_sounds)
	{
		for (const auto& item : *data.loop_sounds)
		{
			validate(item);
		}
	}

	if (data.sounds_spatial_tree)
	{
		validate(*data.sounds_spatial_tree);
	}

	if (data.reactive_ents)
	{
		for (const auto& item : *data.reactive_ents)
		{
			validate(item);
		}
	}

	if (data.reactive_spatial_tree)
	{
		validate(*data.reactive_spatial_tree);
	}

	if (static_cast<std::int64_t>(data.oneshot_effect_count) != (data.oneshot_effects ? static_cast<std::int64_t>(data.oneshot_effects->size()) : 0))
	{
		throw std::runtime_error("client_side_effects.oneshot_effects has a length that its count does not give");
	}

	if ((data.oneshot_sound_indices && data.oneshot_sound_indices->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("client_side_effects has arrays that its count does not fit");
	}

	if ((data.exploders && data.exploders->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("client_side_effects has arrays that its count does not fit");
	}

	if ((data.interval_sounds && data.interval_sounds->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("client_side_effects has arrays that its count does not fit");
	}

	if ((data.loop_sounds && data.loop_sounds->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("client_side_effects has arrays that its count does not fit");
	}

	if ((data.reactive_ents && data.reactive_ents->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("client_side_effects has arrays that its count does not fit");
	}

}

inline void validate(const create_fx_palette_def& data)
{
	(void) data;
}

inline void validate(const create_fx_asset_data& data)
{
	if (data.fx_palette)
	{
		for (const auto& item : *data.fx_palette)
		{
			validate(item);
		}
	}

	if (static_cast<std::int64_t>(data.fx_palette_count) != (data.fx_palette ? static_cast<std::int64_t>(data.fx_palette->size()) : 0))
	{
		throw std::runtime_error("create_fx_asset_data.fx_palette has a length that its count does not give");
	}

}

inline void validate(const server_exploder_def& data)
{
	(void) data;
}

inline void validate(const server_side_effects& data)
{
	if (data.exploders)
	{
		for (const auto& item : *data.exploders)
		{
			validate(item);
		}
	}

	if ((data.exploders && data.exploders->size() > 0xFFFFull))
	{
		throw std::runtime_error("server_side_effects has arrays that its count does not fit");
	}

}

inline void validate(const gfx_placement& data)
{
	(void) data;
}

inline void validate(const client_entity_link_to_def& data)
{
	(void) data;
}

inline void validate(const mayhem_instance& data)
{
	validate(data.initial_pose);

	if (data.link_to)
	{
		validate(*data.link_to);
	}

}

inline void validate(const spawner_field& data)
{
	if (std::ranges::none_of(variable_type_names, [&](const auto& entry) { return entry.second == data.type; }))
	{
		throw std::runtime_error("spawner_field.type names no value");
	}

}

inline void validate(const spawner& data)
{
	if (data.fields)
	{
		for (const auto& item : *data.fields)
		{
			validate(item);
		}
	}

	if ((data.fields && data.fields->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("spawner has arrays that its count does not fit");
	}

}

inline void validate(const spawner_list& data)
{
	if (data.spanwer_list)
	{
		for (const auto& item : *data.spanwer_list)
		{
			validate(item);
		}
	}

	if (static_cast<std::int64_t>(data.spawner_count) != (data.spanwer_list ? static_cast<std::int64_t>(data.spanwer_list->size()) : 0))
	{
		throw std::runtime_error("spawner_list.spanwer_list has a length that its count does not give");
	}

}

inline void validate(const audio_pa_speaker& data)
{
	(void) data;
}

inline void validate(const audio_propagation_node& data)
{
	(void) data;
}

inline void validate(const audio_propagation_edge& data)
{
	(void) data;
}

inline void validate(const coll_map_lookup_entry& data)
{
	(void) data;
}

inline void validate(const map_ents& data)
{
	if (!data.name)
	{
		throw std::runtime_error("map_ents has no name");
	}

	detail::name(*data.name, "map_ents");

	validate(data.trigger);

	validate(data.client_trigger);

	validate(data.client_trigger_blend);

	validate(data.spawn_list);

	validate(data.spline_list);

	if (data.cmodels)
	{
		for (const auto& item : *data.cmodels)
		{
			validate(item);
		}
	}

	if (data.edge_list_spatial_tree)
	{
		validate(*data.edge_list_spatial_tree);
	}

	if (data.client_models)
	{
		for (const auto& item : *data.client_models)
		{
			validate(item);
		}
	}

	if (std::ranges::none_of(dyn_entity_list_id_names, [&](const auto& entry) { return entry.second == data.dyn_entity_lists_count; }))
	{
		throw std::runtime_error("map_ents.dyn_entity_lists_count names no value");
	}

	validate(data.dyn_ent_no_spatial_list);

	if (data.client_ent_anchors)
	{
		for (const auto& item : *data.client_ent_anchors)
		{
			validate(item);
		}
	}

	validate(data.scriptable_map_ents);

	validate(data.spawn_group_loot);

	validate(data.client_side_effects);

	validate(data.create_fx_asset_data);

	validate(data.server_side_effects);

	if (data.mayhem_scenes)
	{
		for (const auto& item : *data.mayhem_scenes)
		{
			validate(item);
		}
	}

	validate(data.spawners);

	if (data.audio_pa_speakers)
	{
		for (const auto& item : *data.audio_pa_speakers)
		{
			validate(item);
		}
	}

	if (data.audio_prop_nodes)
	{
		for (const auto& item : *data.audio_prop_nodes)
		{
			validate(item);
		}
	}

	if (data.audio_prop_edges)
	{
		for (const auto& item : *data.audio_prop_edges)
		{
			validate(item);
		}
	}

	if (data.collmap_lookups)
	{
		for (const auto& item : *data.collmap_lookups)
		{
			validate(item);
		}
	}

	if ((data.entity_string && data.entity_string->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("map_ents has arrays that its count does not fit");
	}

	if ((data.havok_ents_shape_data && data.havok_ents_shape_data->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("map_ents has arrays that its count does not fit");
	}

	if ((data.cmodels && data.cmodels->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("map_ents has arrays that its count does not fit");
	}

	if ((data.client_models && data.client_models->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("map_ents has arrays that its count does not fit");
	}

	if ((data.client_ent_anchors && data.client_ent_anchors->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("map_ents has arrays that its count does not fit");
	}

	if ((data.exploder_names && data.exploder_names->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("map_ents has arrays that its count does not fit");
	}

	if ((data.mayhem_scenes && data.mayhem_scenes->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("map_ents has arrays that its count does not fit");
	}

	if ((data.audio_pa_speakers && data.audio_pa_speakers->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("map_ents has arrays that its count does not fit");
	}

	if ((data.audio_prop_nodes && data.audio_prop_nodes->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("map_ents has arrays that its count does not fit");
	}

	if ((data.audio_prop_edges && data.audio_prop_edges->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("map_ents has arrays that its count does not fit");
	}

	if ((data.collmap_lookups && data.collmap_lookups->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("map_ents has arrays that its count does not fit");
	}

}

inline void validate(const gfx_buffer_resource& data)
{
	(void) data;
}

inline void validate(const gfx_shader_buffer_view& data)
{
	(void) data;
}

inline void validate(const gfx_wrapped_base_buffer& data)
{
	validate(data.buffer);

	validate(data.view);

}

inline void validate(const gfx_wrapped_buffer& data)
{
	validate(data.base);

}

inline void validate(const gfx_index_buffer& data)
{
	(void) data;
}

inline void validate(const gfx_world_draw_verts& data)
{
	validate(data.pos_buffer);

	validate(data.aux_buffer);

	validate(data.index_buffer);

	if (static_cast<std::int64_t>(data.pos_data_size) != (data.pos_data ? static_cast<std::int64_t>(data.pos_data->size()) : 0))
	{
		throw std::runtime_error("gfx_world_draw_verts.pos_data has a length that its count does not give");
	}

	if ((data.aux_data && data.aux_data->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("gfx_world_draw_verts has arrays that its count does not fit");
	}

	if ((data.indices && data.indices->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("gfx_world_draw_verts has arrays that its count does not fit");
	}

}

inline void validate(const gfx_cell_tree_count& data)
{
	(void) data;
}

inline void validate(const gfx_cell_tree& data)
{
	(void) data;
}

inline void validate(const gfx_world_draw_cells& data)
{
	if (data.aabb_tree_counts)
	{
		for (const auto& item : *data.aabb_tree_counts)
		{
			validate(item);
		}
	}

	if (data.aabb_trees)
	{
		for (const auto& item : *data.aabb_trees)
		{
			validate(item);
		}
	}

	if (static_cast<std::int64_t>(data.cell_count) != (data.aabb_tree_counts ? static_cast<std::int64_t>(data.aabb_tree_counts->size()) : 0))
	{
		throw std::runtime_error("gfx_world_draw_cells.aabb_tree_counts has a length that its count does not give");
	}

	if (static_cast<std::int64_t>(data.cell_count) != (data.aabb_trees ? static_cast<std::int64_t>(data.aabb_trees->size()) : 0))
	{
		throw std::runtime_error("gfx_world_draw_cells.aabb_trees has a length that its count does not give");
	}

}

inline void validate(const gfx_sh_compressed_probe_data& data)
{
	(void) data;
}

inline void validate(const gfx_shader_buffer_rw_view& data)
{
	(void) data;
}

inline void validate(const gfx_wrapped_rw_buffer& data)
{
	validate(data.buffer);

	validate(data.rw_view);

}

inline void validate(const gfx_sh_probe_data& data)
{
	(void) data;
}

inline void validate(const gfx_gpu_light_grid_zone& data)
{
	validate(data.fallback_probe_data);

}

inline void validate(const gfx_voxel_tree_header& data)
{
	(void) data;
}

inline void validate(const gfx_voxel_top_down_view_node& data)
{
	(void) data;
}

inline void validate(const gfx_voxel_internal_node& data)
{
	(void) data;
}

inline void validate(const gfx_gpu_light_grid_voxel_tree& data)
{
	if (data.tree_header)
	{
		validate(*data.tree_header);
	}

	if (data.top_down_view_nodes)
	{
		for (const auto& item : *data.top_down_view_nodes)
		{
			validate(item);
		}
	}

	if (data.internal_nodes)
	{
		for (const auto& item : *data.internal_nodes)
		{
			validate(item);
		}
	}

	validate(data.tree_header_buffer);

	validate(data.top_down_view_node_buffer);

	validate(data.internal_node_buffer);

	if (static_cast<std::int64_t>(data.top_down_view_node_count) != (data.top_down_view_nodes ? static_cast<std::int64_t>(data.top_down_view_nodes->size()) : 0))
	{
		throw std::runtime_error("gfx_gpu_light_grid_voxel_tree.top_down_view_nodes has a length that its count does not give");
	}

	if ((data.internal_nodes && data.internal_nodes->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("gfx_gpu_light_grid_voxel_tree has arrays that its count does not fit");
	}

}

inline void validate(const gfx_gpu_light_grid_tetrahedron& data)
{
	(void) data;
}

inline void validate(const gfx_gpu_light_grid_tetrahedron_neighbors& data)
{
	(void) data;
}

inline void validate(const gfx_gpu_light_grid_tetrahedron_visibility& data)
{
	(void) data;
}

inline void validate(const gfx_gpu_light_grid_voxel_start_tetrahedron& data)
{
	(void) data;
}

inline void validate(const gfx_gpu_light_grid& data)
{
	if (data.probes)
	{
		for (const auto& item : *data.probes)
		{
			validate(item);
		}
	}

	validate(data.probes_buffer);

	validate(data.probe_positions_buffer);

	if (data.zones)
	{
		for (const auto& item : *data.zones)
		{
			validate(item);
		}
	}

	if (data.zone_voxel_trees)
	{
		for (const auto& item : *data.zone_voxel_trees)
		{
			validate(item);
		}
	}

	if (data.tetrahedrons)
	{
		for (const auto& item : *data.tetrahedrons)
		{
			validate(item);
		}
	}

	validate(data.tetrahedron_buffer);

	if (data.tetrahedron_neighbors)
	{
		for (const auto& item : *data.tetrahedron_neighbors)
		{
			validate(item);
		}
	}

	validate(data.tetrahedron_neighbors_buffer);

	if (data.tetrahedron_visibility)
	{
		for (const auto& item : *data.tetrahedron_visibility)
		{
			validate(item);
		}
	}

	validate(data.tetrahedron_visibility_buffer);

	if (data.voxel_start_tetrahedron)
	{
		for (const auto& item : *data.voxel_start_tetrahedron)
		{
			validate(item);
		}
	}

	validate(data.voxel_start_tetrahedron_buffer);

	if (static_cast<std::int64_t>(data.probe_count) != (data.probes ? static_cast<std::int64_t>(data.probes->size()) : 0))
	{
		throw std::runtime_error("gfx_gpu_light_grid.probes has a length that its count does not give");
	}

	if (static_cast<std::int64_t>(data.probe_count) != (data.probe_positions ? static_cast<std::int64_t>(data.probe_positions->size()) : 0))
	{
		throw std::runtime_error("gfx_gpu_light_grid.probe_positions has a length that its count does not give");
	}

	if ((data.zones && data.zones->size() > 0xFFFFFFFFull) || (data.zone_voxel_trees && data.zone_voxel_trees->size() > 0xFFFFFFFFull) || (!data.zones != !data.zone_voxel_trees) || (data.zones && data.zone_voxel_trees && data.zones->size() != data.zone_voxel_trees->size()))
	{
		throw std::runtime_error("gfx_gpu_light_grid has arrays that its count does not fit");
	}

	if ((data.tetrahedrons && data.tetrahedrons->size() > 0xFFFFFFFFull) || (data.tetrahedron_neighbors && data.tetrahedron_neighbors->size() > 0xFFFFFFFFull) || (!data.tetrahedrons != !data.tetrahedron_neighbors) || (data.tetrahedrons && data.tetrahedron_neighbors && data.tetrahedrons->size() != data.tetrahedron_neighbors->size()))
	{
		throw std::runtime_error("gfx_gpu_light_grid has arrays that its count does not fit");
	}

	if ((data.tetrahedron_visibility && data.tetrahedron_visibility->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("gfx_gpu_light_grid has arrays that its count does not fit");
	}

	if ((data.voxel_start_tetrahedron && data.voxel_start_tetrahedron->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("gfx_gpu_light_grid has arrays that its count does not fit");
	}

}

inline void validate(const gfx_static_model_light_grid_volume& data)
{
	if (std::ranges::none_of(gfx_static_model_light_grid_volume_flags_names, [&](const auto& entry) { return entry.second == data.flags; }))
	{
		throw std::runtime_error("gfx_static_model_light_grid_volume.flags names no value");
	}

}

inline void validate(const gfx_gpu_dyn_lightsets_lgv_reference& data)
{
	(void) data;
}

inline void validate(const gfx_dynamic_lightset_g_chunk_info& data)
{
	(void) data;
}

inline void validate(const gfx_dynamic_lightset_g_chunk& data)
{
	validate(data.chunk_info);

	if (std::ranges::none_of(gfx_dynamic_lightset_g_chunk_flags_names, [&](const auto& entry) { return entry.second == data.flags; }))
	{
		throw std::runtime_error("gfx_dynamic_lightset_g_chunk.flags names no value");
	}

}

inline void validate(const gfx_dynamic_lightset_probe_data& data)
{
	if (data.chunk_g)
	{
		for (const auto& item : *data.chunk_g)
		{
			validate(item);
		}
	}

	if (data.orig_probes)
	{
		for (const auto& item : *data.orig_probes)
		{
			validate(item);
		}
	}

	validate(data.orig_probes_buffer);

	if (data.bf_probes)
	{
		for (const auto& item : *data.bf_probes)
		{
			validate(item);
		}
	}

	validate(data.bf_probes_buffer);

	validate(data.skip_records_buffer);

	validate(data.pallette_records_buffer);

	validate(data.temp_remap_buffer);

	if (static_cast<std::int64_t>(data.num_g_chunk) != (data.chunk_g ? static_cast<std::int64_t>(data.chunk_g->size()) : 0))
	{
		throw std::runtime_error("gfx_dynamic_lightset_probe_data.chunk_g has a length that its count does not give");
	}

	if ((data.chunk_gbf && data.chunk_gbf->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("gfx_dynamic_lightset_probe_data has arrays that its count does not fit");
	}

	if ((data.orig_probes && data.orig_probes->size() > 0xFFFFFFFFull) || (data.temp_remap && data.temp_remap->size() > 0xFFFFFFFFull) || (!data.orig_probes != !data.temp_remap) || (data.orig_probes && data.temp_remap && data.orig_probes->size() != data.temp_remap->size()))
	{
		throw std::runtime_error("gfx_dynamic_lightset_probe_data has arrays that its count does not fit");
	}

	if ((data.bf_probes && data.bf_probes->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("gfx_dynamic_lightset_probe_data has arrays that its count does not fit");
	}

	if ((data.skip_records && data.skip_records->size() > 0xFFFFFFFFull) || (data.pallette_records && data.pallette_records->size() > 0xFFFFFFFFull) || (!data.skip_records != !data.pallette_records) || (data.skip_records && data.pallette_records && data.skip_records->size() != data.pallette_records->size()))
	{
		throw std::runtime_error("gfx_dynamic_lightset_probe_data has arrays that its count does not fit");
	}

}

inline void validate(const gfx_dynamic_lightset_l_gpp_zone_info& data)
{
	if (data.lgv_references)
	{
		for (const auto& item : *data.lgv_references)
		{
			validate(item);
		}
	}

	validate(data.lgv_references_buffer);

	validate(data.lgv_probe_data);

	if (static_cast<std::int64_t>(data.num_orig_sml_gpp_probes) != (data.lgv_references ? static_cast<std::int64_t>(data.lgv_references->size()) : 0))
	{
		throw std::runtime_error("gfx_dynamic_lightset_l_gpp_zone_info.lgv_references has a length that its count does not give");
	}

}

inline void validate(const gfx_gpu_light_grid_transient_data& data)
{
	if (data.gpu_light_grid)
	{
		validate(*data.gpu_light_grid);
	}

	if (data.static_model_volumes)
	{
		for (const auto& item : *data.static_model_volumes)
		{
			validate(item);
		}
	}

	if (data.sml_gpp_zone)
	{
		validate(*data.sml_gpp_zone);
	}

	if ((data.static_model_volumes && data.static_model_volumes->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("gfx_gpu_light_grid_transient_data has arrays that its count does not fit");
	}

}

inline void validate(const gfx_image_reflection_probe& data)
{
	if (std::ranges::none_of(gfx_texture_id_names, [&](const auto& entry) { return entry.second == data.texture_id; }))
	{
		throw std::runtime_error("gfx_image_reflection_probe.texture_id names no value");
	}

	if (static_cast<std::int64_t>(data.total_size) != (data.pixels ? static_cast<std::int64_t>(data.pixels->size()) : 0))
	{
		throw std::runtime_error("gfx_image_reflection_probe.pixels has a length that its count does not give");
	}

}

inline void validate(const gfx_reflection_probe_transient_data& data)
{
	if (data.exclusive_probe_buckets)
	{
		for (const auto& item : *data.exclusive_probe_buckets)
		{
			validate(item);
		}
	}

	if ((data.reflection_probe_instances && data.reflection_probe_instances->size() > 0xFFFFull))
	{
		throw std::runtime_error("gfx_reflection_probe_transient_data has arrays that its count does not fit");
	}

	if ((data.exclusive_probe_buckets && data.exclusive_probe_buckets->size() > 0xFFFFull))
	{
		throw std::runtime_error("gfx_reflection_probe_transient_data has arrays that its count does not fit");
	}

}

inline void validate(const gfx_static_decal_volume_packed& data)
{
	(void) data;
}

inline void validate(const gfx_static_decal_volume_uid& data)
{
	(void) data;
}

inline void validate(const gfx_decal_volume_transient_data& data)
{
	if (data.decal_volumes)
	{
		for (const auto& item : *data.decal_volumes)
		{
			validate(item);
		}
	}

	validate(data.decal_volumes_buffer);

	if (data.collection_bounds)
	{
		for (const auto& item : *data.collection_bounds)
		{
			validate(item);
		}
	}

	if (data.decal_volume_uids)
	{
		for (const auto& item : *data.decal_volume_uids)
		{
			validate(item);
		}
	}

	if (static_cast<std::int64_t>(data.decal_volume_count) != (data.decal_volumes ? static_cast<std::int64_t>(data.decal_volumes->size()) : 0))
	{
		throw std::runtime_error("gfx_decal_volume_transient_data.decal_volumes has a length that its count does not give");
	}

	if (static_cast<std::int64_t>(data.decal_volume_count) != (data.decal_volume_uids ? static_cast<std::int64_t>(data.decal_volume_uids->size()) : 0))
	{
		throw std::runtime_error("gfx_decal_volume_transient_data.decal_volume_uids has a length that its count does not give");
	}

	if ((data.collection_bounds && data.collection_bounds->size() > 0xFFFFull))
	{
		throw std::runtime_error("gfx_decal_volume_transient_data has arrays that its count does not fit");
	}

}

inline void validate(const gfx_transient_zone& data)
{
	if (!data.name)
	{
		throw std::runtime_error("gfx_transient_zone has no name");
	}

	detail::name(*data.name, "gfx_transient_zone");

	validate(data.draw_verts);

	validate(data.draw_cells);

	validate(data.gpu_light_grid);

	if (data.reflection_probes)
	{
		validate(*data.reflection_probes);
	}

	if (data.decal_volumes)
	{
		validate(*data.decal_volumes);
	}

	if ((data.collections && data.collections->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("gfx_transient_zone has arrays that its count does not fit");
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

} // namespace zonetool::asset_data
