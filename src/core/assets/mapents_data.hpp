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

// Neutral mapents assets (docs/NEUTRAL_ASSETS.md), emitted by tools/gen_iw8_neutral.py from the IW8 structs and stream
// traversals: do not edit; change the spec (tools/neutral_families/mapents.json) or the generator.
namespace zonetool::asset_data
{
namespace mapents
{
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

struct map_ents;
struct map_triggers;
struct trigger_model;
struct trigger_hull;
struct bounds;
struct trigger_slab;
struct trigger_winding;
struct trigger_winding_point;
struct client_triggers;
struct client_map_triggers;
struct client_trigger_model;
struct client_trigger_hull;
struct ct_aud_rvb_pan_info;
struct client_entity_link_to_def;
struct spatial_partition_tree;
struct client_trigger_blend;
struct client_trigger_blend_node;
struct spawn_point_record_list;
struct spawn_point_entity_record;
struct spline_record_list;
struct spline_point_record_list;
struct spline_point_entity_record;
struct cmodel_t;
struct cm_client_model;
struct spatial_partition_population;
struct spatial_partition_population_tree;
struct spatial_partition_population_partition;
struct extent_bounds;
struct spatial_partition_population_node;
struct spatial_partition_population_transient_map;
struct bitarray_dynamic;
struct client_ent_anchor;
struct scriptable_map_ents;
struct scriptable_physics_limits;
struct scriptable_instance;
struct scriptable_event_sun_data;
struct scriptable_reserved_dynents;
struct scriptable_reserved_dynent;
struct spawn_group;
struct spawn_group_point;
struct spawn_group_cluster;
struct spawn_group_zone;
struct client_side_effects;
struct client_oneshot_effect_def;
struct fx_combined_def;
struct snd_alias_lookup;
struct snd_alias_list;
struct snd_alias;
struct client_exploder_def;
struct client_interval_sound_def;
struct client_loop_sound_def;
struct client_reactive_ent_def;
struct create_fx_asset_data;
struct create_fx_palette_def;
struct server_side_effects;
struct server_exploder_def;
struct mayhem_instance;
struct gfx_placement;
struct spawner_list;
struct spawner;
struct spawner_field;
struct audio_pa_speaker;
struct audio_propagation_node;
struct audio_propagation_edge;
struct coll_map_lookup_entry;

inline void validate(const map_ents& data);
inline void validate(const map_triggers& data);
inline void validate(const trigger_model& data);
inline void validate(const trigger_hull& data);
inline void validate(const bounds& data);
inline void validate(const trigger_slab& data);
inline void validate(const trigger_winding& data);
inline void validate(const trigger_winding_point& data);
inline void validate(const client_triggers& data);
inline void validate(const client_map_triggers& data);
inline void validate(const client_trigger_model& data);
inline void validate(const client_trigger_hull& data);
inline void validate(const ct_aud_rvb_pan_info& data);
inline void validate(const client_entity_link_to_def& data);
inline void validate(const spatial_partition_tree& data);
inline void validate(const client_trigger_blend& data);
inline void validate(const client_trigger_blend_node& data);
inline void validate(const spawn_point_record_list& data);
inline void validate(const spawn_point_entity_record& data);
inline void validate(const spline_record_list& data);
inline void validate(const spline_point_record_list& data);
inline void validate(const spline_point_entity_record& data);
inline void validate(const cmodel_t& data);
inline void validate(const cm_client_model& data);
inline void validate(const spatial_partition_population& data);
inline void validate(const spatial_partition_population_tree& data);
inline void validate(const spatial_partition_population_partition& data);
inline void validate(const extent_bounds& data);
inline void validate(const spatial_partition_population_node& data);
inline void validate(const spatial_partition_population_transient_map& data);
inline void validate(const bitarray_dynamic& data);
inline void validate(const client_ent_anchor& data);
inline void validate(const scriptable_map_ents& data);
inline void validate(const scriptable_physics_limits& data);
inline void validate(const scriptable_instance& data);
inline void validate(const scriptable_event_sun_data& data);
inline void validate(const scriptable_reserved_dynents& data);
inline void validate(const scriptable_reserved_dynent& data);
inline void validate(const spawn_group& data);
inline void validate(const spawn_group_point& data);
inline void validate(const spawn_group_cluster& data);
inline void validate(const spawn_group_zone& data);
inline void validate(const client_side_effects& data);
inline void validate(const client_oneshot_effect_def& data);
inline void validate(const fx_combined_def& data);
inline void validate(const snd_alias_lookup& data);
inline void validate(const snd_alias_list& data);
inline void validate(const snd_alias& data);
inline void validate(const client_exploder_def& data);
inline void validate(const client_interval_sound_def& data);
inline void validate(const client_loop_sound_def& data);
inline void validate(const client_reactive_ent_def& data);
inline void validate(const create_fx_asset_data& data);
inline void validate(const create_fx_palette_def& data);
inline void validate(const server_side_effects& data);
inline void validate(const server_exploder_def& data);
inline void validate(const mayhem_instance& data);
inline void validate(const gfx_placement& data);
inline void validate(const spawner_list& data);
inline void validate(const spawner& data);
inline void validate(const spawner_field& data);
inline void validate(const audio_pa_speaker& data);
inline void validate(const audio_propagation_node& data);
inline void validate(const audio_propagation_edge& data);
inline void validate(const coll_map_lookup_entry& data);

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

struct client_map_triggers
{
	std::uint32_t count{};
	std::optional<std::vector<client_trigger_model>> models{};
	std::optional<std::vector<client_trigger_hull>> hulls{};
	std::optional<std::vector<trigger_slab>> slabs{};

	bool operator==(const client_map_triggers&) const = default;
};

struct spatial_partition_tree
{
	std::optional<std::vector<std::uint8_t>> tree{};
	std::optional<std::vector<std::uint32_t>> always_list{};

	bool operator==(const spatial_partition_tree&) const = default;
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
	std::optional<std::vector<std::optional<std::string>>> detail_sound_bank{};
	std::optional<std::vector<ct_aud_rvb_pan_info>> audio_rvb_pan_info{};
	std::optional<std::vector<std::int16_t>> parent{};
	std::optional<std::vector<std::optional<client_entity_link_to_def>>> link_to{};
	std::optional<spatial_partition_tree> spatial_tree{};

	bool operator==(const client_triggers&) const = default;
};

struct client_trigger_blend
{
	std::uint16_t num_client_trigger_blend_nodes{};
	std::optional<std::vector<client_trigger_blend_node>> blend_nodes{};

	bool operator==(const client_trigger_blend&) const = default;
};

struct spawn_point_record_list
{
	std::uint16_t spawns_count{};
	std::optional<std::vector<spawn_point_entity_record>> spawns{};

	bool operator==(const spawn_point_record_list&) const = default;
};

struct spline_record_list
{
	std::uint16_t spline_count{};
	std::optional<std::vector<spline_point_record_list>> splines{};

	bool operator==(const spline_record_list&) const = default;
};

struct spatial_partition_population_tree
{
	std::optional<std::vector<spatial_partition_population_partition>> partitions{};
	std::optional<std::vector<extent_bounds>> extents{};

	bool operator==(const spatial_partition_population_tree&) const = default;
};

struct spatial_partition_population
{
	std::optional<spatial_partition_population_tree> tree{};
	std::optional<std::vector<spatial_partition_population_node>> buckets{};
	std::uint32_t current_population{};
	bool buckets_are_changing{};

	bool operator==(const spatial_partition_population&) const = default;
};

struct spatial_partition_population_transient_map
{
	std::uint32_t transient_count{};
	std::optional<std::vector<bitarray_dynamic>> partitions_for_transient{};

	bool operator==(const spatial_partition_population_transient_map&) const = default;
};

struct spatial_partition_population_node
{
	std::uint32_t spatial_node_next{};

	bool operator==(const spatial_partition_population_node&) const = default;
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

struct scriptable_reserved_dynents
{
	std::int16_t free_head{};
	std::optional<std::vector<scriptable_reserved_dynent>> reserved_dynents{};

	bool operator==(const scriptable_reserved_dynents&) const = default;
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

struct spawn_group
{
	std::uint16_t point_count{};
	std::optional<std::vector<spawn_group_point>> points{};
	std::optional<std::vector<spawn_group_cluster>> clusters{};
	std::optional<std::vector<spawn_group_zone>> zones{};

	bool operator==(const spawn_group&) const = default;
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

struct create_fx_asset_data
{
	std::uint16_t fx_palette_count{};
	std::optional<std::vector<create_fx_palette_def>> fx_palette{};

	bool operator==(const create_fx_asset_data&) const = default;
};

struct server_side_effects
{
	std::uint16_t exploder_count{};
	std::optional<std::vector<server_exploder_def>> exploders{};

	bool operator==(const server_side_effects&) const = default;
};

struct spawner_list
{
	std::uint32_t spawner_count{};
	std::optional<std::vector<spawner>> spanwer_list{};

	bool operator==(const spawner_list&) const = default;
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
	std::optional<std::vector<std::optional<std::string>>> edge_lists{};
	std::optional<spatial_partition_tree> edge_list_spatial_tree{};
	std::optional<std::vector<cm_client_model>> client_models{};
	std::array<std::uint32_t, 2> dyn_ent_count{};
	std::uint32_t dyn_ent_count_total{};
	std::uint16_t dyn_entity_lists_count{};
	std::optional<std::vector<std::uint16_t>> dyn_ent_list_ids{};
	std::array<std::array<std::uint16_t, 2>, 2> dyn_ent_physics_setup_head{};
	std::array<std::array<std::uint16_t, 2>, 2> dyn_ent_physics_setup_tail{};
	std::array<std::uint16_t, 2> dyn_ent_no_spatial_count{};
	std::uint16_t dyn_ent_max_client_history_count{};
	std::uint8_t dyn_ent_max_pose_part_count{};
	std::uint32_t dyn_ents_with_extra_pose_parts_count{};
	std::optional<spatial_partition_population> dyn_ent_spatial_population_0{};
	std::optional<spatial_partition_population> dyn_ent_spatial_population_1{};
	std::optional<spatial_partition_population_transient_map> dyn_ent_spatial_transient_map_0{};
	std::optional<spatial_partition_population_transient_map> dyn_ent_spatial_transient_map_1{};
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

struct ct_aud_rvb_pan_info
{
	bool has_custom_position{};
	std::array<float, 3> position{};
	float max_wall_distance{};
	float min_reverb_volume{};

	bool operator==(const ct_aud_rvb_pan_info&) const = default;
};

struct client_entity_link_to_def
{
	std::int32_t anchor_index{};
	std::array<float, 3> origin_offset{};
	std::array<float, 3> angle_offset{};

	bool operator==(const client_entity_link_to_def&) const = default;
};

struct client_trigger_blend_node
{
	std::array<float, 3> point_a{};
	std::array<float, 3> point_b{};
	std::uint16_t trigger_a{};
	std::uint16_t trigger_b{};

	bool operator==(const client_trigger_blend_node&) const = default;
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

struct spline_point_record_list
{
	std::uint16_t spline_point_count{};
	float spline_length{};
	std::optional<std::vector<spline_point_entity_record>> spline_points{};

	bool operator==(const spline_point_record_list&) const = default;
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

struct bitarray_dynamic
{
	std::optional<std::vector<std::uint32_t>> array{};
	std::uint32_t bit_count{};

	bool operator==(const bitarray_dynamic&) const = default;
};

struct client_ent_anchor
{
	std::optional<std::string> name{};
	std::int32_t ent_num{};

	bool operator==(const client_ent_anchor&) const = default;
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

struct scriptable_reserved_dynent
{
	std::uint32_t dynent_id{};
	std::int16_t next{};

	bool operator==(const scriptable_reserved_dynent&) const = default;
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

struct create_fx_palette_def
{
	std::optional<std::string> alias_name{};
	std::optional<std::string> effect_path{};
	std::uint16_t ref_count{};

	bool operator==(const create_fx_palette_def&) const = default;
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

struct gfx_placement
{
	std::array<float, 4> quat{};
	std::array<float, 3> origin{};

	bool operator==(const gfx_placement&) const = default;
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

struct spawner_field
{
	std::optional<std::string> key{};
	std::uint32_t key_canonical{};
	std::optional<std::string> value{};
	std::string type{};

	bool operator==(const spawner_field&) const = default;
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

	if (data.link_to)
	{
		for (const auto& item : *data.link_to)
		{
			if (item)
			{
				validate(*item);
			}
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

	if (static_cast<std::int64_t>(data.trigger.count) != (data.detail_sound_bank ? static_cast<std::int64_t>(data.detail_sound_bank->size()) : 0))
	{
		throw std::runtime_error("client_triggers.detail_sound_bank has a length that its count does not give");
	}

	if (static_cast<std::int64_t>(data.trigger.count) != (data.audio_rvb_pan_info ? static_cast<std::int64_t>(data.audio_rvb_pan_info->size()) : 0))
	{
		throw std::runtime_error("client_triggers.audio_rvb_pan_info has a length that its count does not give");
	}

	if (static_cast<std::int64_t>(data.trigger.count) != (data.parent ? static_cast<std::int64_t>(data.parent->size()) : 0))
	{
		throw std::runtime_error("client_triggers.parent has a length that its count does not give");
	}

	if (static_cast<std::int64_t>(data.trigger.count) != (data.link_to ? static_cast<std::int64_t>(data.link_to->size()) : 0))
	{
		throw std::runtime_error("client_triggers.link_to has a length that its count does not give");
	}

	if ((data.trigger_string && data.trigger_string->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("client_triggers has arrays that its count does not fit");
	}

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

inline void validate(const spatial_partition_population& data)
{
	if (data.tree)
	{
		validate(*data.tree);
	}

	if (data.buckets)
	{
		for (const auto& item : *data.buckets)
		{
			validate(item);
		}
	}

	if ((data.buckets && data.buckets->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("spatial_partition_population has arrays that its count does not fit");
	}

}

inline void validate(const spatial_partition_population_transient_map& data)
{
	if (data.partitions_for_transient)
	{
		for (const auto& item : *data.partitions_for_transient)
		{
			validate(item);
		}
	}

	if (static_cast<std::int64_t>(data.transient_count) != (data.partitions_for_transient ? static_cast<std::int64_t>(data.partitions_for_transient->size()) : 0))
	{
		throw std::runtime_error("spatial_partition_population_transient_map.partitions_for_transient has a length that its count does not give");
	}

}

inline void validate(const spatial_partition_population_node& data)
{
	(void) data;
}

inline void validate(const scriptable_physics_limits& data)
{
	(void) data;
}

inline void validate(const scriptable_event_sun_data& data)
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

	if (static_cast<std::int64_t>(data.point_count) != (data.points ? static_cast<std::int64_t>(data.points->size()) : 0))
	{
		throw std::runtime_error("spawn_group.points has a length that its count does not give");
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

inline void validate(const server_side_effects& data)
{
	if (data.exploders)
	{
		for (const auto& item : *data.exploders)
		{
			validate(item);
		}
	}

	if (static_cast<std::int64_t>(data.exploder_count) != (data.exploders ? static_cast<std::int64_t>(data.exploders->size()) : 0))
	{
		throw std::runtime_error("server_side_effects.exploders has a length that its count does not give");
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

	if (data.dyn_ent_spatial_population_0)
	{
		validate(*data.dyn_ent_spatial_population_0);
	}

	if (data.dyn_ent_spatial_population_1)
	{
		validate(*data.dyn_ent_spatial_population_1);
	}

	if (data.dyn_ent_spatial_transient_map_0)
	{
		validate(*data.dyn_ent_spatial_transient_map_0);
	}

	if (data.dyn_ent_spatial_transient_map_1)
	{
		validate(*data.dyn_ent_spatial_transient_map_1);
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

	if (static_cast<std::int64_t>(static_cast<std::uint16_t>(data.dyn_entity_lists_count)) != (data.dyn_ent_list_ids ? static_cast<std::int64_t>(data.dyn_ent_list_ids->size()) : 0))
	{
		throw std::runtime_error("map_ents.dyn_ent_list_ids has a length that its count does not give");
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

	if ((data.edge_lists && data.edge_lists->size() > 0xFFFFFFFFull))
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

inline void validate(const client_trigger_model& data)
{
	(void) data;
}

inline void validate(const client_trigger_hull& data)
{
	validate(data.trigger_space_bounds);

}

inline void validate(const ct_aud_rvb_pan_info& data)
{
	(void) data;
}

inline void validate(const client_entity_link_to_def& data)
{
	(void) data;
}

inline void validate(const client_trigger_blend_node& data)
{
	(void) data;
}

inline void validate(const spawn_point_entity_record& data)
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

inline void validate(const spline_point_entity_record& data)
{
	(void) data;
}

inline void validate(const cmodel_t& data)
{
	validate(data.bounds);

}

inline void validate(const cm_client_model& data)
{
	(void) data;
}

inline void validate(const spatial_partition_population_partition& data)
{
	(void) data;
}

inline void validate(const extent_bounds& data)
{
	(void) data;
}

inline void validate(const bitarray_dynamic& data)
{
	if ((data.array && data.array->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("bitarray_dynamic has arrays that its count does not fit");
	}

}

inline void validate(const client_ent_anchor& data)
{
	(void) data;
}

inline void validate(const scriptable_instance& data)
{
	(void) data;
}

inline void validate(const scriptable_reserved_dynent& data)
{
	(void) data;
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

inline void validate(const fx_combined_def& data)
{
	(void) data;
}

inline void validate(const snd_alias_lookup& data)
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

inline void validate(const snd_alias& data)
{
	(void) data;
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

inline void validate(const create_fx_palette_def& data)
{
	(void) data;
}

inline void validate(const server_exploder_def& data)
{
	(void) data;
}

inline void validate(const gfx_placement& data)
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

inline void validate(const spawner_field& data)
{
	if (std::ranges::none_of(variable_type_names, [&](const auto& entry) { return entry.second == data.type; }))
	{
		throw std::runtime_error("spawner_field.type names no value");
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

} // namespace mapents
} // namespace zonetool::asset_data
