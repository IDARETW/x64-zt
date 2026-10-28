#pragma once

#include "asset_data.hpp"
#include "particle_data.hpp"
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

// Neutral vfx assets (docs/NEUTRAL_ASSETS.md), emitted by tools/gen_iw8_neutral.py from the IW8 structs and stream
// traversals: do not edit; change the spec (tools/neutral_families/vfx.json) or the generator.
namespace zonetool::asset_data
{
namespace vfx
{
inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 64> particle_module_type_names{{
	{0, "init_begin"},
	{1, "init_attributes"},
	{2, "init_beam"},
	{3, "init_camera_offset"},
	{4, "init_cloud"},
	{5, "init_decal"},
	{6, "init_flare"},
	{7, "init_geo_trail"},
	{8, "init_kill_wrap_box"},
	{9, "init_light_omni"},
	{10, "init_light_spot"},
	{11, "init_material"},
	{12, "init_mirror_texture"},
	{13, "init_model"},
	{14, "init_occlusion_query"},
	{15, "init_oriented_sprite"},
	{16, "init_particle_sim"},
	{17, "init_player_facing"},
	{18, "init_relative_velocity"},
	{19, "init_rotation"},
	{20, "init_rotation_3_d"},
	{21, "init_runner"},
	{22, "init_sound"},
	{23, "init_spawn"},
	{24, "init_spawn_shape_first"},
	{25, "init_spawn_shape_cylinder"},
	{26, "init_spawn_shape_ellipsoid"},
	{27, "init_spawn_shape_mesh"},
	{28, "init_spawn_shape_sphere"},
	{29, "init_tail"},
	{30, "init_vector_field"},
	{31, "init_volumetric"},
	{32, "init_dismember"},
	{33, "update_begin"},
	{34, "color_graph"},
	{35, "color_lerp"},
	{36, "emission_graph"},
	{37, "emissive_graph"},
	{38, "shader_graph"},
	{39, "force"},
	{40, "force_drag_graph"},
	{41, "gravity"},
	{42, "intensity_graph"},
	{43, "temperature_graph"},
	{44, "parent_velocity_graph"},
	{45, "physics_light"},
	{46, "physics_ray_cast"},
	{47, "position_graph"},
	{48, "rotation_graph"},
	{49, "rotation_graph_3_d"},
	{50, "scale_by_distance"},
	{51, "size_graph"},
	{52, "size_lerp"},
	{53, "velocity_graph"},
	{54, "vector_field_graph"},
	{55, "test_begin"},
	{56, "test_birth"},
	{57, "test_death"},
	{58, "test_gravity"},
	{59, "test_impact"},
	{60, "test_pos"},
	{61, "test_time_in_state"},
	{62, "count"},
	{65535, "invalid"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 3> particle_module_init_beam_flags_names{{
	{1, "tile"},
	{2, "use_curve_points"},
	{4, "camera_facing"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 6> fx_mark_projection_axis_names{{
	{0, "pos_x"},
	{1, "neg_x"},
	{2, "pos_y"},
	{3, "neg_y"},
	{4, "pos_z"},
	{5, "neg_z"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 12> particle_module_axes_flags_names{{
	{1, "pos_x"},
	{2, "neg_x"},
	{3, "all_x"},
	{4, "pos_y"},
	{8, "neg_y"},
	{12, "all_y"},
	{16, "pos_z"},
	{21, "all_pos"},
	{32, "neg_z"},
	{42, "all_neg"},
	{48, "all_z"},
	{63, "all"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 4> particle_render_options_names{{
	{0, "none"},
	{1, "thermal_view_only"},
	{2, "thermal_view_never"},
	{3, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 4> particle_shader_graph_options_names{{
	{0, "none"},
	{1, "depthscan"},
	{2, "sphericalz"},
	{3, "option_count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 4> particle_mirror_texture_type_names{{
	{0, "none"},
	{1, "standard"},
	{2, "random"},
	{3, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 6> particle_relative_velocity_type_names{{
	{0, "local"},
	{1, "world"},
	{2, "relative_to_effect_origin"},
	{3, "count"},
	{4, "world_with_bolt_info"},
	{5, "relative_to_effect_origin_with_bolt_info"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 8> particle_use_orientation_options_names{{
	{0, "identity"},
	{1, "rotation"},
	{2, "relative_rotation"},
	{3, "velocity_y"},
	{4, "velocity_x"},
	{5, "velocity_z"},
	{6, "relative"},
	{7, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 4> particle_use_scale_options_names{{
	{0, "none"},
	{1, "mult"},
	{2, "override"},
	{3, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 4> particle_use_velocity_options_names{{
	{0, "none"},
	{1, "mult"},
	{2, "override"},
	{3, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 5> particle_spawn_flags_names{{
	{1, "only_on_surface"},
	{2, "use_curve_values"},
	{4, "use_modifier_scale"},
	{8, "is_deterministic"},
	{16, "uniform"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 3> particle_module_axis_names{{
	{0, "x"},
	{1, "y"},
	{2, "z"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 5> particle_spawn_shape_type_names{{
	{0, "box"},
	{1, "cylinder"},
	{2, "ellipsoid"},
	{3, "mesh"},
	{4, "sphere"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 11> dismemberment_point_type_names{{
	{0, "default"},
	{1, "cryo"},
	{2, "gold"},
	{3, "shadow"},
	{4, "weed"},
	{5, "raygun"},
	{6, "soul"},
	{7, "generic0"},
	{8, "generic1"},
	{9, "generic2"},
	{10, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 4> particle_gravity_options_names{{
	{0, "none"},
	{1, "gravity_only"},
	{2, "gravity_never"},
	{3, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 3> particle_instance_pool_action_names{{
	{0, "no_spawn"},
	{1, "kill_oldest_and_spawn"},
	{2, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 33> particle_data_flags_names{{
	{0, "use_none"},
	{1, "use_position"},
	{2, "use_velocity"},
	{4, "use_color"},
	{8, "use_rotation_angle"},
	{16, "use_rotation_rate"},
	{32, "use_spawn_pos"},
	{64, "use_spawn_quat"},
	{128, "use_size"},
	{256, "use_emissive"},
	{512, "use_parent_velocity"},
	{1024, "use_child_system"},
	{2048, "use_spawn_time"},
	{4096, "use_life"},
	{8192, "use_state"},
	{16384, "use_random_seed"},
	{32768, "use_flags"},
	{65536, "use_module_tests"},
	{131072, "use_handle"},
	{262144, "use_intensity"},
	{524288, "use_temperature"},
	{1048576, "use_camera_distance"},
	{2097152, "use_inherit_percentage"},
	{4194304, "use_ray_cast_handle"},
	{8388608, "use_atlas_index"},
	{16777216, "use_vector_field_scale"},
	{26409215, "use_base"},
	{33554432, "use_lighting_frac"},
	{33816832, "use_emissive_curves"},
	{67108864, "use_prev_placement"},
	{134217728, "use_shader_params"},
	{134217729, "check_field_count"},
	{268435455, "use_all"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 4> particle_phase_options_names{{
	{0, "phase_never"},
	{1, "phase_only"},
	{2, "always"},
	{3, "count"},
}};

struct particle_system;
struct particle_emitter_def;
struct particle_state_def;
struct particle_module_group_def;
struct particle_module_def;
struct particle_module_init_atlas;
struct particle_module;
struct particle_curve_def;
struct particle_curve_control_point_def;
struct particle_module_init_attributes;
struct particle_module_init_beam;
struct particle_module_init_camera_offset;
struct particle_module_init_cloud;
struct particle_module_init_decal;
struct particle_module_init_flare;
struct particle_float_range;
struct particle_module_init_geo_trail;
struct particle_module_init_kill_wrap_box;
struct particle_module_init_light_omni;
struct particle_module_init_light_spot;
struct particle_module_init_material;
struct particle_material_data;
struct particle_module_init_mirror_texture;
struct particle_module_init_model;
struct particle_module_init_occlusion_query;
struct particle_module_init_oriented_sprite;
struct particle_module_init_particle_sim;
struct particle_module_init_player_facing;
struct particle_module_init_relative_velocity;
struct particle_module_init_rotation;
struct particle_module_init_rotation3_d;
struct particle_module_init_runner;
struct particle_modifier;
struct particle_module_init_sound;
struct particle_module_init_spawn;
struct particle_module_init_spawn_shape_box;
struct particle_module_init_spawn_shape;
struct particle_module_init_spawn_shape_cylinder;
struct particle_module_init_spawn_shape_ellipsoid;
struct particle_module_init_spawn_shape_mesh;
struct particle_module_init_spawn_shape_sphere;
struct particle_module_init_tail;
struct particle_module_init_vector_field;
struct particle_module_init_volumetric;
struct particle_module_init_dismember;
struct particle_module_attractor;
struct particle_module_color_graph;
struct particle_module_color_lerp;
struct particle_module_emission_graph;
struct particle_module_emissive_graph;
struct particle_module_shader_graph;
struct particle_module_force;
struct particle_module_force_drag_graph;
struct particle_module_gravity;
struct particle_module_intensity_graph;
struct particle_module_temperature_graph;
struct particle_module_parent_velocity_graph;
struct particle_module_physics_light;
struct particle_module_physics_ray_cast;
struct bounds;
struct particle_module_position_graph;
struct particle_module_rotation_graph;
struct particle_module_rotation_graph3_d;
struct particle_module_scale_by_distance;
struct particle_module_size_graph;
struct particle_module_size_lerp;
struct particle_module_velocity_graph;
struct particle_module_vector_field_graph;
struct particle_module_test_age;
struct particle_module_test;
struct particle_module_test_event_handler_data;
struct particle_module_test_birth;
struct particle_module_test_death;
struct particle_module_test_gravity;
struct particle_module_test_impact;
struct particle_module_test_pos;
struct particle_module_test_time_in_state;
struct particle_int_range;

inline void validate(const particle_system& data);
inline void validate(const particle_emitter_def& data);
inline void validate(const particle_state_def& data);
inline void validate(const particle_module_group_def& data);
inline void validate(const particle_module_def& data);
inline void validate(const particle_module_init_atlas& data);
inline void validate(const particle_module& data);
inline void validate(const particle_curve_def& data);
inline void validate(const particle_curve_control_point_def& data);
inline void validate(const particle_module_init_attributes& data);
inline void validate(const particle_module_init_beam& data);
inline void validate(const particle_module_init_camera_offset& data);
inline void validate(const particle_module_init_cloud& data);
inline void validate(const particle_module_init_decal& data);
inline void validate(const particle_module_init_flare& data);
inline void validate(const particle_float_range& data);
inline void validate(const particle_module_init_geo_trail& data);
inline void validate(const particle_module_init_kill_wrap_box& data);
inline void validate(const particle_module_init_light_omni& data);
inline void validate(const particle_module_init_light_spot& data);
inline void validate(const particle_module_init_material& data);
inline void validate(const particle_material_data& data);
inline void validate(const particle_module_init_mirror_texture& data);
inline void validate(const particle_module_init_model& data);
inline void validate(const particle_module_init_occlusion_query& data);
inline void validate(const particle_module_init_oriented_sprite& data);
inline void validate(const particle_module_init_particle_sim& data);
inline void validate(const particle_module_init_player_facing& data);
inline void validate(const particle_module_init_relative_velocity& data);
inline void validate(const particle_module_init_rotation& data);
inline void validate(const particle_module_init_rotation3_d& data);
inline void validate(const particle_module_init_runner& data);
inline void validate(const particle_modifier& data);
inline void validate(const particle_module_init_sound& data);
inline void validate(const particle_module_init_spawn& data);
inline void validate(const particle_module_init_spawn_shape_box& data);
inline void validate(const particle_module_init_spawn_shape& data);
inline void validate(const particle_module_init_spawn_shape_cylinder& data);
inline void validate(const particle_module_init_spawn_shape_ellipsoid& data);
inline void validate(const particle_module_init_spawn_shape_mesh& data);
inline void validate(const particle_module_init_spawn_shape_sphere& data);
inline void validate(const particle_module_init_tail& data);
inline void validate(const particle_module_init_vector_field& data);
inline void validate(const particle_module_init_volumetric& data);
inline void validate(const particle_module_init_dismember& data);
inline void validate(const particle_module_attractor& data);
inline void validate(const particle_module_color_graph& data);
inline void validate(const particle_module_color_lerp& data);
inline void validate(const particle_module_emission_graph& data);
inline void validate(const particle_module_emissive_graph& data);
inline void validate(const particle_module_shader_graph& data);
inline void validate(const particle_module_force& data);
inline void validate(const particle_module_force_drag_graph& data);
inline void validate(const particle_module_gravity& data);
inline void validate(const particle_module_intensity_graph& data);
inline void validate(const particle_module_temperature_graph& data);
inline void validate(const particle_module_parent_velocity_graph& data);
inline void validate(const particle_module_physics_light& data);
inline void validate(const particle_module_physics_ray_cast& data);
inline void validate(const bounds& data);
inline void validate(const particle_module_position_graph& data);
inline void validate(const particle_module_rotation_graph& data);
inline void validate(const particle_module_rotation_graph3_d& data);
inline void validate(const particle_module_scale_by_distance& data);
inline void validate(const particle_module_size_graph& data);
inline void validate(const particle_module_size_lerp& data);
inline void validate(const particle_module_velocity_graph& data);
inline void validate(const particle_module_vector_field_graph& data);
inline void validate(const particle_module_test_age& data);
inline void validate(const particle_module_test& data);
inline void validate(const particle_module_test_event_handler_data& data);
inline void validate(const particle_module_test_birth& data);
inline void validate(const particle_module_test_death& data);
inline void validate(const particle_module_test_gravity& data);
inline void validate(const particle_module_test_impact& data);
inline void validate(const particle_module_test_pos& data);
inline void validate(const particle_module_test_time_in_state& data);
inline void validate(const particle_int_range& data);

struct particle_system
{
	std::optional<std::string> name{};
	std::optional<std::vector<particle_emitter_def>> emitter_defs{};
	std::optional<std::vector<particle_scripted_input_node>> scripted_input_node_defs{};
	std::int32_t version{};
	std::uint32_t flags{};
	std::int32_t occlusion_override_emitter_index{};
	std::string phase_options{};
	float draw_frustum_cull_radius{};
	float update_frustum_cull_radius{};
	float sun_distance{};
	std::int32_t pre_roll_m_sec{};
	std::array<float, 4> editor_position{};
	std::array<float, 4> editor_rotation{};
	std::array<float, 4> game_tweak_position{};
	std::array<float, 4> game_tweak_rotation{};

	bool operator==(const particle_system&) const = default;
};

struct particle_float_range
{
	float min{};
	float max{};

	bool operator==(const particle_float_range&) const = default;
};

struct particle_int_range
{
	std::int32_t min{};
	std::int32_t max{};

	bool operator==(const particle_int_range&) const = default;
};

struct particle_curve_def
{
	std::optional<std::vector<particle_curve_control_point_def>> control_points{};
	float scale{};

	bool operator==(const particle_curve_def&) const = default;
};

struct particle_emitter_def
{
	std::optional<std::vector<particle_state_def>> state_defs{};
	particle_float_range particle_spawn_rate{};
	particle_float_range particle_life{};
	particle_float_range particle_delay{};
	std::uint32_t particle_count_max{};
	particle_int_range particle_burst_count{};
	particle_float_range emitter_life{};
	particle_float_range emitter_delay{};
	std::int32_t random_seed{};
	particle_float_range spawn_range_sq{};
	float fade_out_max_distance{};
	particle_curve_def fade_curve_def{};
	float spawn_frustum_cull_radius{};
	std::uint32_t flags{};
	std::string gravity_options{};
	std::array<std::uint32_t, 4> group_i_ds{};
	particle_float_range emit_by_distance_density{};
	std::uint32_t instance_pool{};
	std::uint32_t solo_instance_max{};
	std::string instance_action{};
	std::vector<std::string> m_data_flags{};
	particle_float_range particle_spawn_shape_range{};

	bool operator==(const particle_emitter_def&) const = default;
};

struct particle_module_group_def
{
	std::optional<std::vector<particle_module_def>> module_defs{};
	bool disabled{};

	bool operator==(const particle_module_group_def&) const = default;
};

struct particle_state_def
{
	std::optional<std::array<particle_module_group_def, 3>> module_group_defs{};
	std::uint32_t element_type{};
	std::uint64_t flags{};

	bool operator==(const particle_state_def&) const = default;
};

struct particle_module
{
	std::string m_type{};
	std::uint32_t m_flags{};

	bool operator==(const particle_module&) const = default;
};

struct particle_module_init_atlas
{
	particle_module unnamed{};
	std::int32_t m_start_frame{};
	std::int32_t m_loop_count{};
	bool m_random_index{};
	bool m_play_over_life{};
	std::array<particle_curve_def, 2> m_curves{};

	bool operator==(const particle_module_init_atlas&) const = default;
};

struct particle_module_init_attributes
{
	particle_module unnamed{};
	bool m_use_non_uniform_interpolation_for_color{};
	bool m_use_non_uniform_interpolation_for_size{};
	std::array<float, 4> m_size_min{};
	std::array<float, 4> m_size_max{};
	std::array<float, 4> m_color_min{};
	std::array<float, 4> m_color_max{};
	std::array<float, 4> m_velocity_min{};
	std::array<float, 4> m_velocity_max{};

	bool operator==(const particle_module_init_attributes&) const = default;
};

struct particle_module_init_beam
{
	particle_module unnamed{};
	std::string m_beam_flags{};
	float m_split_distance{};
	std::array<float, 4> m_start_pos{};
	std::array<float, 4> m_end_pos{};
	std::array<float, 4> m_offset{};
	std::array<float, 4> m_curve_point1{};
	std::array<float, 4> m_curve_point2{};
	particle_linked_asset_list m_linked_asset_list{};
	float m_scroll_rate_uv{};
	float m_scroll_rate_spiral_graph{};
	float m_spiral_repeat_distance{};

	bool operator==(const particle_module_init_beam&) const = default;
};

struct particle_module_init_camera_offset
{
	particle_module unnamed{};

	bool operator==(const particle_module_init_camera_offset&) const = default;
};

struct particle_module_init_cloud
{
	particle_module unnamed{};

	bool operator==(const particle_module_init_cloud&) const = default;
};

struct particle_module_init_decal
{
	particle_module unnamed{};
	std::uint16_t m_fade_in_time{};
	std::uint16_t m_fade_out_time{};
	std::uint16_t m_stoppable_fade_out_time{};
	std::uint16_t m_lerp_wait_time{};
	particle_linked_asset_list m_linked_asset_list{};
	std::array<float, 4> m_lerp_color{};
	std::uint16_t m_lerp_time{};
	bool m_dynamic_decal{};
	std::string m_projection_axis{};
	bool m_bypass_stacking_limiter{};

	bool operator==(const particle_module_init_decal&) const = default;
};

struct particle_module_init_flare
{
	particle_module unnamed{};
	float m_position{};
	std::array<float, 4> m_direction{};
	std::int32_t m_angular_rot_count{};
	std::uint32_t m_flare_flags{};
	particle_float_range m_depth_scale_range{};
	particle_float_range m_depth_scale_value{};
	particle_float_range m_radial_rot{};
	particle_float_range m_radial_scale_x{};
	particle_float_range m_radial_scale_y{};
	std::array<particle_curve_def, 4> m_curves{};

	bool operator==(const particle_module_init_flare&) const = default;
};

struct particle_module_init_geo_trail
{
	particle_module unnamed{};
	std::uint32_t m_num_points_max{};
	float m_split_distance{};
	float m_split_angle{};
	float m_center_offset{};
	std::uint32_t m_num_sheets{};
	float m_fade_in_distance{};
	float m_fade_out_distance{};
	float m_tile_distance{};
	particle_float_range m_tile_offset{};
	float m_scroll_time{};
	bool m_use_local_velocity{};
	bool m_use_vertical_texture{};
	bool m_camera_facing{};
	bool m_fix_leading_edge{};
	bool m_clamp_u_vs{};

	bool operator==(const particle_module_init_geo_trail&) const = default;
};

struct particle_module_init_kill_wrap_box
{
	particle_module unnamed{};
	std::vector<std::string> m_planes_kill{};
	std::array<float, 4> m_bounds{};
	std::array<float, 4> m_offset{};

	bool operator==(const particle_module_init_kill_wrap_box&) const = default;
};

struct particle_module_init_light_omni
{
	particle_module unnamed{};
	particle_linked_asset_list m_linked_asset_list{};
	float m_fov_outer{};
	float m_fov_inner{};
	float m_bulb_radius{};
	float m_bulb_length{};
	float m_distance_falloff{};
	float m_brightness{};
	float m_intensity_uv{};
	float m_intensity_ir{};
	float m_intensity_heat{};
	float m_shadow_softness{};
	float m_shadow_bias{};
	float m_shadow_area{};
	float m_tone_mapping_scale_factor{};
	bool m_disable_volumetric{};
	bool m_disable_shadow_map{};
	bool m_disable_dynamic_shadows{};
	bool m_script_scale{};

	bool operator==(const particle_module_init_light_omni&) const = default;
};

struct particle_module_init_light_spot
{
	particle_module unnamed{};
	particle_linked_asset_list m_linked_asset_list{};
	float m_fov_outer{};
	float m_fov_inner{};
	float m_bulb_radius{};
	float m_bulb_length{};
	float m_distance_falloff{};
	float m_fov_collimation{};
	float m_brightness{};
	float m_intensity_uv{};
	float m_intensity_ir{};
	float m_intensity_heat{};
	float m_shadow_softness{};
	float m_shadow_bias{};
	float m_shadow_area{};
	float m_shadow_near_plane{};
	float m_tone_mapping_scale_factor{};
	bool m_disable_volumetric{};
	bool m_disable_shadow_map{};
	bool m_disable_dynamic_shadows{};
	bool m_script_scale{};

	bool operator==(const particle_module_init_light_spot&) const = default;
};

struct particle_material_data
{
	std::array<float, 4> falloff_parms{};
	std::array<float, 4> opaque_color{};
	std::array<float, 4> transparent_color{};
	std::array<float, 2> translucent_shadow_params{};
	std::array<float, 2> beam_parms{};
	std::array<float, 4> eye_offset_parms{};
	std::array<float, 4> alpha_dissolve_parms{};
	std::array<float, 4> feather_parms0{};
	std::array<float, 4> feather_parms1{};
	std::array<float, 4> region_highlight_parms{};
	std::array<float, 4> depth_scan_color{};
	std::array<float, 4> depth_scan_outline_color{};
	std::array<float, 2> thermal_params{};
	float anisotropy{};
	std::uint32_t feature_enabled_flags{};

	bool operator==(const particle_material_data&) const = default;
};

struct particle_module_init_material
{
	particle_module unnamed{};
	std::string m_render_options{};
	std::string m_shader_graph_options{};
	particle_linked_asset_list m_linked_asset_list{};
	particle_material_data m_material_data{};

	bool operator==(const particle_module_init_material&) const = default;
};

struct particle_module_init_mirror_texture
{
	particle_module unnamed{};
	std::string m_mirror_horizontal{};
	std::string m_mirror_vertical{};

	bool operator==(const particle_module_init_mirror_texture&) const = default;
};

struct particle_module_init_model
{
	particle_module unnamed{};
	bool m_use_physics{};
	bool m_motion_blur_hq{};
	particle_linked_asset_list m_linked_asset_list{};

	bool operator==(const particle_module_init_model&) const = default;
};

struct particle_module_init_occlusion_query
{
	particle_module unnamed{};
	float m_depth_bias{};
	float m_fade_in_time{};
	float m_fade_out_time{};
	particle_float_range m_scale{};
	std::array<float, 2> m_world_size{};
	bool m_offscreen_raycast{};

	bool operator==(const particle_module_init_occlusion_query&) const = default;
};

struct particle_module_init_oriented_sprite
{
	particle_module unnamed{};
	std::array<float, 4> m_orientation_quat{};

	bool operator==(const particle_module_init_oriented_sprite&) const = default;
};

struct particle_module_init_particle_sim
{
	particle_module unnamed{};
	particle_linked_asset_list m_linked_asset_list{};
	particle_float_range m_scale_factor{};
	bool m_hold_last_frame{};

	bool operator==(const particle_module_init_particle_sim&) const = default;
};

struct particle_module_init_player_facing
{
	particle_module unnamed{};

	bool operator==(const particle_module_init_player_facing&) const = default;
};

struct particle_module_init_relative_velocity
{
	particle_module unnamed{};
	std::string m_velocity_type{};
	bool m_use_bolt_info{};

	bool operator==(const particle_module_init_relative_velocity&) const = default;
};

struct particle_module_init_rotation
{
	particle_module unnamed{};
	particle_float_range m_rotation_angle{};
	particle_float_range m_rotation_rate{};

	bool operator==(const particle_module_init_rotation&) const = default;
};

struct particle_module_init_rotation3_d
{
	particle_module unnamed{};
	std::array<float, 4> m_rotation_angle_min{};
	std::array<float, 4> m_rotation_angle_max{};
	std::array<float, 4> m_rotation_rate_min{};
	std::array<float, 4> m_rotation_rate_max{};

	bool operator==(const particle_module_init_rotation3_d&) const = default;
};

struct particle_modifier
{
	std::array<float, 4> min{};
	std::array<float, 4> max{};

	bool operator==(const particle_modifier&) const = default;
};

struct particle_module_init_runner
{
	particle_module unnamed{};
	particle_modifier m_scale_modifier{};
	particle_modifier m_velocity_modifier{};
	particle_linked_asset_list m_linked_asset_list{};
	std::string m_use_orientation_options{};
	std::string m_use_scale_options{};
	std::string m_use_velocity_options{};
	bool m_attach_to_parent{};
	bool m_stop_child_on_death{};
	bool m_kill_child_on_death{};
	bool m_legacy_orientation_velocity{};
	bool m_legacy_orientation_rotation{};

	bool operator==(const particle_module_init_runner&) const = default;
};

struct particle_module_init_sound
{
	particle_module unnamed{};
	particle_linked_asset_list m_linked_asset_list{};

	bool operator==(const particle_module_init_sound&) const = default;
};

struct particle_module_init_spawn
{
	particle_module unnamed{};
	std::array<particle_curve_def, 1> m_curves{};

	bool operator==(const particle_module_init_spawn&) const = default;
};

struct particle_module_init_spawn_shape
{
	particle_module unnamed{};
	std::vector<std::string> m_axis_flags{};
	std::vector<std::string> m_spawn_flags{};
	std::string m_normal_axis{};
	std::string m_spawn_type{};
	float m_volume_cube_root{};
	std::array<float, 4> m_calculation_offset{};
	std::array<float, 4> m_offset{};

	bool operator==(const particle_module_init_spawn_shape&) const = default;
};

struct particle_module_init_spawn_shape_box
{
	particle_module_init_spawn_shape unnamed{};
	bool m_use_beam_info{};
	std::array<float, 4> m_dimensions_min{};
	std::array<float, 4> m_dimensions_max{};
	std::array<particle_curve_def, 6> m_curves{};

	bool operator==(const particle_module_init_spawn_shape_box&) const = default;
};

struct particle_module_init_spawn_shape_cylinder
{
	particle_module_init_spawn_shape unnamed{};
	bool m_has_rotation{};
	bool m_rotate_calculated_offset{};
	float m_half_height{};
	particle_float_range m_radius{};
	std::array<float, 4> m_direction_quat{};
	std::array<particle_curve_def, 5> m_curves{};

	bool operator==(const particle_module_init_spawn_shape_cylinder&) const = default;
};

struct particle_module_init_spawn_shape_ellipsoid
{
	particle_module_init_spawn_shape unnamed{};
	std::array<float, 4> m_radius_min{};
	std::array<float, 4> m_radius_max{};
	std::array<particle_curve_def, 6> m_curves{};

	bool operator==(const particle_module_init_spawn_shape_ellipsoid&) const = default;
};

struct particle_module_init_spawn_shape_mesh
{
	particle_module_init_spawn_shape unnamed{};
	std::uint32_t m_num_mesh_assets{};
	bool m_use_faces{};
	particle_linked_asset_list m_linked_asset_list{};
	std::array<particle_curve_def, 3> m_curves{};

	bool operator==(const particle_module_init_spawn_shape_mesh&) const = default;
};

struct particle_module_init_spawn_shape_sphere
{
	particle_module_init_spawn_shape unnamed{};
	particle_float_range m_radius{};
	std::array<particle_curve_def, 4> m_curves{};

	bool operator==(const particle_module_init_spawn_shape_sphere&) const = default;
};

struct particle_module_init_tail
{
	particle_module unnamed{};
	std::uint16_t m_average_past_velocities{};
	std::uint16_t m_max_parent_speed{};
	bool m_tail_leading{};
	bool m_scale_with_velocity{};
	bool m_rotate_around_pivot{};

	bool operator==(const particle_module_init_tail&) const = default;
};

struct particle_module_init_vector_field
{
	particle_module unnamed{};
	particle_linked_asset_list m_linked_asset_list{};
	float m_area_scale{};
	float m_amplitude_scale{};
	float m_frequency_scale{};
	float m_noise_max_rotation{};
	float m_noise_frequency_scale{};
	float m_noise_scale{};

	bool operator==(const particle_module_init_vector_field&) const = default;
};

struct particle_module_init_volumetric
{
	particle_module unnamed{};
	float m_density{};
	float m_falloff{};
	float m_noise_amplitude{};
	std::uint32_t m_noise_mode{};

	bool operator==(const particle_module_init_volumetric&) const = default;
};

struct particle_module_init_dismember
{
	particle_module unnamed{};
	std::uint32_t m_chance{};
	std::string m_dismemberment_type{};

	bool operator==(const particle_module_init_dismember&) const = default;
};

struct particle_module_attractor
{
	particle_module unnamed{};
	bool m_kill_when_near{};
	float m_force_mag{};
	float m_near_distance_sq{};
	std::array<float, 4> m_attract_point{};

	bool operator==(const particle_module_attractor&) const = default;
};

struct particle_module_color_graph
{
	particle_module unnamed{};
	bool first_curve{};
	bool m_modulate_color_by_alpha{};
	std::array<particle_curve_def, 8> m_curves{};

	bool operator==(const particle_module_color_graph&) const = default;
};

struct particle_module_color_lerp
{
	particle_module unnamed{};
	std::array<float, 4> m_color_begin{};
	std::array<float, 4> m_color_end{};

	bool operator==(const particle_module_color_lerp&) const = default;
};

struct particle_module_emission_graph
{
	particle_module unnamed{};
	std::array<particle_curve_def, 2> m_curves{};

	bool operator==(const particle_module_emission_graph&) const = default;
};

struct particle_module_emissive_graph
{
	particle_module unnamed{};
	bool first_curve{};
	std::array<particle_curve_def, 10> m_curves{};

	bool operator==(const particle_module_emissive_graph&) const = default;
};

struct particle_module_shader_graph
{
	particle_module unnamed{};
	bool first_curve{};
	std::array<particle_curve_def, 12> m_curves{};

	bool operator==(const particle_module_shader_graph&) const = default;
};

struct particle_module_force
{
	particle_module unnamed{};
	std::array<float, 4> m_force_min{};
	std::array<float, 4> m_force_max{};

	bool operator==(const particle_module_force&) const = default;
};

struct particle_module_force_drag_graph
{
	particle_module unnamed{};
	std::array<particle_curve_def, 2> m_curves{};

	bool operator==(const particle_module_force_drag_graph&) const = default;
};

struct particle_module_gravity
{
	particle_module unnamed{};
	particle_float_range m_gravity_percentage{};

	bool operator==(const particle_module_gravity&) const = default;
};

struct particle_module_intensity_graph
{
	particle_module unnamed{};
	bool first_curve{};
	std::array<particle_curve_def, 2> m_curves{};

	bool operator==(const particle_module_intensity_graph&) const = default;
};

struct particle_module_temperature_graph
{
	particle_module unnamed{};
	bool first_curve{};
	std::array<particle_curve_def, 2> m_curves{};

	bool operator==(const particle_module_temperature_graph&) const = default;
};

struct particle_module_parent_velocity_graph
{
	particle_module unnamed{};
	std::array<particle_curve_def, 2> m_curves{};

	bool operator==(const particle_module_parent_velocity_graph&) const = default;
};

struct particle_module_physics_light
{
	particle_module unnamed{};
	bool m_ignore_emitter_orientation{};
	bool m_use_surface_type{};
	particle_linked_asset_list m_linked_asset_list{};

	bool operator==(const particle_module_physics_light&) const = default;
};

struct bounds
{
	std::array<float, 3> mid_point{};
	std::array<float, 3> half_size{};

	bool operator==(const bounds&) const = default;
};

struct particle_module_physics_ray_cast
{
	particle_module unnamed{};
	particle_float_range m_bounce{};
	bounds m_bounds{};
	bool m_use_item_clip{};
	bool m_use_surface_type{};
	bool m_collide_with_water{};
	bool m_ignore_content_item{};

	bool operator==(const particle_module_physics_ray_cast&) const = default;
};

struct particle_module_position_graph
{
	particle_module unnamed{};
	std::array<particle_curve_def, 6> m_curves{};

	bool operator==(const particle_module_position_graph&) const = default;
};

struct particle_module_rotation_graph
{
	particle_module unnamed{};
	bool m_use_rotation_rate{};
	std::array<particle_curve_def, 2> m_curves{};

	bool operator==(const particle_module_rotation_graph&) const = default;
};

struct particle_module_rotation_graph3_d
{
	particle_module unnamed{};
	bool m_use_rotation_rate{};
	std::array<particle_curve_def, 6> m_curves{};

	bool operator==(const particle_module_rotation_graph3_d&) const = default;
};

struct particle_module_scale_by_distance
{
	particle_module unnamed{};
	std::array<particle_curve_def, 2> m_curves{};
	float m_max_distance{};
	float m_scale_begin{};
	float m_scale_end{};

	bool operator==(const particle_module_scale_by_distance&) const = default;
};

struct particle_module_size_graph
{
	particle_module unnamed{};
	bool first_curve{};
	std::array<particle_curve_def, 6> m_curves{};
	std::array<float, 4> m_size_begin{};
	std::array<float, 4> m_size_end{};

	bool operator==(const particle_module_size_graph&) const = default;
};

struct particle_module_size_lerp
{
	particle_module unnamed{};
	std::array<float, 4> m_size_begin{};
	std::array<float, 4> m_size_end{};

	bool operator==(const particle_module_size_lerp&) const = default;
};

struct particle_module_velocity_graph
{
	particle_module unnamed{};
	std::array<particle_curve_def, 6> m_curves{};
	std::array<float, 4> m_velocity_begin{};
	std::array<float, 4> m_velocity_end{};

	bool operator==(const particle_module_velocity_graph&) const = default;
};

struct particle_module_vector_field_graph
{
	particle_module unnamed{};
	bool first_curve{};
	std::array<particle_curve_def, 2> m_curves{};

	bool operator==(const particle_module_vector_field_graph&) const = default;
};

struct particle_module_test_event_handler_data
{
	std::uint32_t m_next_state{};
	particle_linked_asset_list m_linked_asset_list{};
	bool m_kill{};

	bool operator==(const particle_module_test_event_handler_data&) const = default;
};

struct particle_module_test
{
	particle_module unnamed{};
	std::uint16_t m_module_index{};
	std::string m_use_orientation_options{};
	std::string m_use_scale_options{};
	std::string m_use_velocity_options{};
	particle_modifier m_scale_modifier{};
	particle_modifier m_velocity_modifier{};
	particle_module_test_event_handler_data m_event_handler_data{};

	bool operator==(const particle_module_test&) const = default;
};

struct particle_module_test_age
{
	particle_module_test unnamed{};
	float m_age{};

	bool operator==(const particle_module_test_age&) const = default;
};

struct particle_module_test_birth
{
	particle_module_test unnamed{};

	bool operator==(const particle_module_test_birth&) const = default;
};

struct particle_module_test_death
{
	particle_module_test unnamed{};

	bool operator==(const particle_module_test_death&) const = default;
};

struct particle_module_test_gravity
{
	particle_module_test unnamed{};
	std::uint32_t m_gravity_state{};

	bool operator==(const particle_module_test_gravity&) const = default;
};

struct particle_module_test_impact
{
	particle_module_test unnamed{};
	std::uint32_t m_impact_direction{};

	bool operator==(const particle_module_test_impact&) const = default;
};

struct particle_module_test_pos
{
	particle_module_test unnamed{};
	float m_pos_z{};

	bool operator==(const particle_module_test_pos&) const = default;
};

struct particle_module_test_time_in_state
{
	particle_module_test unnamed{};
	particle_float_range m_time_in_state{};

	bool operator==(const particle_module_test_time_in_state&) const = default;
};

struct particle_module_def
{
	std::variant<std::monostate, particle_module_init_atlas, particle_module_init_attributes, particle_module_init_beam, particle_module_init_camera_offset, particle_module_init_cloud, particle_module_init_decal, particle_module_init_flare, particle_module_init_geo_trail, particle_module_init_kill_wrap_box, particle_module_init_light_omni, particle_module_init_light_spot, particle_module_init_material, particle_module_init_mirror_texture, particle_module_init_model, particle_module_init_occlusion_query, particle_module_init_oriented_sprite, particle_module_init_particle_sim, particle_module_init_player_facing, particle_module_init_relative_velocity, particle_module_init_rotation, particle_module_init_rotation3_d, particle_module_init_runner, particle_module_init_sound, particle_module_init_spawn, particle_module_init_spawn_shape_box, particle_module_init_spawn_shape_cylinder, particle_module_init_spawn_shape_ellipsoid, particle_module_init_spawn_shape_mesh, particle_module_init_spawn_shape_sphere, particle_module_init_tail, particle_module_init_vector_field, particle_module_init_volumetric, particle_module_init_dismember, particle_module_attractor, particle_module_color_graph, particle_module_color_lerp, particle_module_emission_graph, particle_module_emissive_graph, particle_module_shader_graph, particle_module_force, particle_module_force_drag_graph, particle_module_gravity, particle_module_intensity_graph, particle_module_temperature_graph, particle_module_parent_velocity_graph, particle_module_physics_light, particle_module_physics_ray_cast, particle_module_position_graph, particle_module_rotation_graph, particle_module_rotation_graph3_d, particle_module_scale_by_distance, particle_module_size_graph, particle_module_size_lerp, particle_module_velocity_graph, particle_module_vector_field_graph, particle_module_test_age, particle_module_test_birth, particle_module_test_death, particle_module_test_gravity, particle_module_test_impact, particle_module_test_pos, particle_module_test_time_in_state> module_data{};

	bool operator==(const particle_module_def&) const = default;
};

struct particle_curve_control_point_def
{
	float time{};
	float value{};
	float inv_time_delta{};

	bool operator==(const particle_curve_control_point_def&) const = default;
};

inline void validate(const particle_system& data)
{
	if (!data.name)
	{
		throw std::runtime_error("particle_system has no name");
	}

	detail::name(*data.name, "particle_system");

	if (data.emitter_defs)
	{
		for (const auto& item : *data.emitter_defs)
		{
			validate(item);
		}
	}

	if (data.scripted_input_node_defs)
	{
		for (const auto& item : *data.scripted_input_node_defs)
		{
			validate(item);
		}
	}

	if (std::ranges::none_of(particle_phase_options_names, [&](const auto& entry) { return entry.second == data.phase_options; }))
	{
		throw std::runtime_error("particle_system.phase_options names no value");
	}

	if ((data.emitter_defs && data.emitter_defs->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("particle_system has arrays that its count does not fit");
	}

	if ((data.scripted_input_node_defs && data.scripted_input_node_defs->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("particle_system has arrays that its count does not fit");
	}

}

inline void validate(const particle_float_range& data)
{
	(void) data;
}

inline void validate(const particle_int_range& data)
{
	(void) data;
}

inline void validate(const particle_curve_def& data)
{
	if (data.control_points)
	{
		for (const auto& item : *data.control_points)
		{
			validate(item);
		}
	}

	if ((data.control_points && data.control_points->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("particle_curve_def has arrays that its count does not fit");
	}

}

inline void validate(const particle_emitter_def& data)
{
	if (data.state_defs)
	{
		for (const auto& item : *data.state_defs)
		{
			validate(item);
		}
	}

	validate(data.particle_spawn_rate);

	validate(data.particle_life);

	validate(data.particle_delay);

	validate(data.particle_burst_count);

	validate(data.emitter_life);

	validate(data.emitter_delay);

	validate(data.spawn_range_sq);

	validate(data.fade_curve_def);

	if (std::ranges::none_of(particle_gravity_options_names, [&](const auto& entry) { return entry.second == data.gravity_options; }))
	{
		throw std::runtime_error("particle_emitter_def.gravity_options names no value");
	}

	validate(data.emit_by_distance_density);

	if (std::ranges::none_of(particle_instance_pool_action_names, [&](const auto& entry) { return entry.second == data.instance_action; }))
	{
		throw std::runtime_error("particle_emitter_def.instance_action names no value");
	}

	for (const auto& name : data.m_data_flags)
	{
		if (std::ranges::none_of(particle_data_flags_names, [&](const auto& entry) { return entry.second == name; }))
		{
			throw std::runtime_error("particle_emitter_def.m_data_flags names no flag");
		}
	}

	validate(data.particle_spawn_shape_range);

	if ((data.state_defs && data.state_defs->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("particle_emitter_def has arrays that its count does not fit");
	}

}

inline void validate(const particle_module_group_def& data)
{
	if (data.module_defs)
	{
		for (const auto& item : *data.module_defs)
		{
			validate(item);
		}
	}

	if ((data.module_defs && data.module_defs->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("particle_module_group_def has arrays that its count does not fit");
	}

}

inline void validate(const particle_state_def& data)
{
	if (data.module_group_defs)
	{
		for (const auto& item : *data.module_group_defs)
		{
			validate(item);
		}
	}

}

inline void validate(const particle_module& data)
{
	if (std::ranges::none_of(particle_module_type_names, [&](const auto& entry) { return entry.second == data.m_type; }))
	{
		throw std::runtime_error("particle_module.m_type names no value");
	}

}

inline void validate(const particle_module_init_atlas& data)
{
	validate(data.unnamed);

	for (const auto& item : data.m_curves)
	{
		validate(item);
	}

}

inline void validate(const particle_module_init_attributes& data)
{
	validate(data.unnamed);

}

inline void validate(const particle_module_init_beam& data)
{
	validate(data.unnamed);

	if (std::ranges::none_of(particle_module_init_beam_flags_names, [&](const auto& entry) { return entry.second == data.m_beam_flags; }))
	{
		throw std::runtime_error("particle_module_init_beam.m_beam_flags names no value");
	}

	validate(data.m_linked_asset_list);

}

inline void validate(const particle_module_init_camera_offset& data)
{
	validate(data.unnamed);

}

inline void validate(const particle_module_init_cloud& data)
{
	validate(data.unnamed);

}

inline void validate(const particle_module_init_decal& data)
{
	validate(data.unnamed);

	validate(data.m_linked_asset_list);

	if (std::ranges::none_of(fx_mark_projection_axis_names, [&](const auto& entry) { return entry.second == data.m_projection_axis; }))
	{
		throw std::runtime_error("particle_module_init_decal.m_projection_axis names no value");
	}

}

inline void validate(const particle_module_init_flare& data)
{
	validate(data.unnamed);

	validate(data.m_depth_scale_range);

	validate(data.m_depth_scale_value);

	validate(data.m_radial_rot);

	validate(data.m_radial_scale_x);

	validate(data.m_radial_scale_y);

	for (const auto& item : data.m_curves)
	{
		validate(item);
	}

}

inline void validate(const particle_module_init_geo_trail& data)
{
	validate(data.unnamed);

	validate(data.m_tile_offset);

}

inline void validate(const particle_module_init_kill_wrap_box& data)
{
	validate(data.unnamed);

	for (const auto& name : data.m_planes_kill)
	{
		if (std::ranges::none_of(particle_module_axes_flags_names, [&](const auto& entry) { return entry.second == name; }))
		{
			throw std::runtime_error("particle_module_init_kill_wrap_box.m_planes_kill names no flag");
		}
	}

}

inline void validate(const particle_module_init_light_omni& data)
{
	validate(data.unnamed);

	validate(data.m_linked_asset_list);

}

inline void validate(const particle_module_init_light_spot& data)
{
	validate(data.unnamed);

	validate(data.m_linked_asset_list);

}

inline void validate(const particle_material_data& data)
{
	(void) data;
}

inline void validate(const particle_module_init_material& data)
{
	validate(data.unnamed);

	if (std::ranges::none_of(particle_render_options_names, [&](const auto& entry) { return entry.second == data.m_render_options; }))
	{
		throw std::runtime_error("particle_module_init_material.m_render_options names no value");
	}

	if (std::ranges::none_of(particle_shader_graph_options_names, [&](const auto& entry) { return entry.second == data.m_shader_graph_options; }))
	{
		throw std::runtime_error("particle_module_init_material.m_shader_graph_options names no value");
	}

	validate(data.m_linked_asset_list);

	validate(data.m_material_data);

}

inline void validate(const particle_module_init_mirror_texture& data)
{
	validate(data.unnamed);

	if (std::ranges::none_of(particle_mirror_texture_type_names, [&](const auto& entry) { return entry.second == data.m_mirror_horizontal; }))
	{
		throw std::runtime_error("particle_module_init_mirror_texture.m_mirror_horizontal names no value");
	}

	if (std::ranges::none_of(particle_mirror_texture_type_names, [&](const auto& entry) { return entry.second == data.m_mirror_vertical; }))
	{
		throw std::runtime_error("particle_module_init_mirror_texture.m_mirror_vertical names no value");
	}

}

inline void validate(const particle_module_init_model& data)
{
	validate(data.unnamed);

	validate(data.m_linked_asset_list);

}

inline void validate(const particle_module_init_occlusion_query& data)
{
	validate(data.unnamed);

	validate(data.m_scale);

}

inline void validate(const particle_module_init_oriented_sprite& data)
{
	validate(data.unnamed);

}

inline void validate(const particle_module_init_particle_sim& data)
{
	validate(data.unnamed);

	validate(data.m_linked_asset_list);

	validate(data.m_scale_factor);

}

inline void validate(const particle_module_init_player_facing& data)
{
	validate(data.unnamed);

}

inline void validate(const particle_module_init_relative_velocity& data)
{
	validate(data.unnamed);

	if (std::ranges::none_of(particle_relative_velocity_type_names, [&](const auto& entry) { return entry.second == data.m_velocity_type; }))
	{
		throw std::runtime_error("particle_module_init_relative_velocity.m_velocity_type names no value");
	}

}

inline void validate(const particle_module_init_rotation& data)
{
	validate(data.unnamed);

	validate(data.m_rotation_angle);

	validate(data.m_rotation_rate);

}

inline void validate(const particle_module_init_rotation3_d& data)
{
	validate(data.unnamed);

}

inline void validate(const particle_modifier& data)
{
	(void) data;
}

inline void validate(const particle_module_init_runner& data)
{
	validate(data.unnamed);

	validate(data.m_scale_modifier);

	validate(data.m_velocity_modifier);

	validate(data.m_linked_asset_list);

	if (std::ranges::none_of(particle_use_orientation_options_names, [&](const auto& entry) { return entry.second == data.m_use_orientation_options; }))
	{
		throw std::runtime_error("particle_module_init_runner.m_use_orientation_options names no value");
	}

	if (std::ranges::none_of(particle_use_scale_options_names, [&](const auto& entry) { return entry.second == data.m_use_scale_options; }))
	{
		throw std::runtime_error("particle_module_init_runner.m_use_scale_options names no value");
	}

	if (std::ranges::none_of(particle_use_velocity_options_names, [&](const auto& entry) { return entry.second == data.m_use_velocity_options; }))
	{
		throw std::runtime_error("particle_module_init_runner.m_use_velocity_options names no value");
	}

}

inline void validate(const particle_module_init_sound& data)
{
	validate(data.unnamed);

	validate(data.m_linked_asset_list);

}

inline void validate(const particle_module_init_spawn& data)
{
	validate(data.unnamed);

	for (const auto& item : data.m_curves)
	{
		validate(item);
	}

}

inline void validate(const particle_module_init_spawn_shape& data)
{
	validate(data.unnamed);

	for (const auto& name : data.m_axis_flags)
	{
		if (std::ranges::none_of(particle_module_axes_flags_names, [&](const auto& entry) { return entry.second == name; }))
		{
			throw std::runtime_error("particle_module_init_spawn_shape.m_axis_flags names no flag");
		}
	}

	for (const auto& name : data.m_spawn_flags)
	{
		if (std::ranges::none_of(particle_spawn_flags_names, [&](const auto& entry) { return entry.second == name; }))
		{
			throw std::runtime_error("particle_module_init_spawn_shape.m_spawn_flags names no flag");
		}
	}

	if (std::ranges::none_of(particle_module_axis_names, [&](const auto& entry) { return entry.second == data.m_normal_axis; }))
	{
		throw std::runtime_error("particle_module_init_spawn_shape.m_normal_axis names no value");
	}

	if (std::ranges::none_of(particle_spawn_shape_type_names, [&](const auto& entry) { return entry.second == data.m_spawn_type; }))
	{
		throw std::runtime_error("particle_module_init_spawn_shape.m_spawn_type names no value");
	}

}

inline void validate(const particle_module_init_spawn_shape_box& data)
{
	validate(data.unnamed);

	for (const auto& item : data.m_curves)
	{
		validate(item);
	}

}

inline void validate(const particle_module_init_spawn_shape_cylinder& data)
{
	validate(data.unnamed);

	validate(data.m_radius);

	for (const auto& item : data.m_curves)
	{
		validate(item);
	}

}

inline void validate(const particle_module_init_spawn_shape_ellipsoid& data)
{
	validate(data.unnamed);

	for (const auto& item : data.m_curves)
	{
		validate(item);
	}

}

inline void validate(const particle_module_init_spawn_shape_mesh& data)
{
	validate(data.unnamed);

	validate(data.m_linked_asset_list);

	for (const auto& item : data.m_curves)
	{
		validate(item);
	}

}

inline void validate(const particle_module_init_spawn_shape_sphere& data)
{
	validate(data.unnamed);

	validate(data.m_radius);

	for (const auto& item : data.m_curves)
	{
		validate(item);
	}

}

inline void validate(const particle_module_init_tail& data)
{
	validate(data.unnamed);

}

inline void validate(const particle_module_init_vector_field& data)
{
	validate(data.unnamed);

	validate(data.m_linked_asset_list);

}

inline void validate(const particle_module_init_volumetric& data)
{
	validate(data.unnamed);

}

inline void validate(const particle_module_init_dismember& data)
{
	validate(data.unnamed);

	if (std::ranges::none_of(dismemberment_point_type_names, [&](const auto& entry) { return entry.second == data.m_dismemberment_type; }))
	{
		throw std::runtime_error("particle_module_init_dismember.m_dismemberment_type names no value");
	}

}

inline void validate(const particle_module_attractor& data)
{
	validate(data.unnamed);

}

inline void validate(const particle_module_color_graph& data)
{
	validate(data.unnamed);

	for (const auto& item : data.m_curves)
	{
		validate(item);
	}

}

inline void validate(const particle_module_color_lerp& data)
{
	validate(data.unnamed);

}

inline void validate(const particle_module_emission_graph& data)
{
	validate(data.unnamed);

	for (const auto& item : data.m_curves)
	{
		validate(item);
	}

}

inline void validate(const particle_module_emissive_graph& data)
{
	validate(data.unnamed);

	for (const auto& item : data.m_curves)
	{
		validate(item);
	}

}

inline void validate(const particle_module_shader_graph& data)
{
	validate(data.unnamed);

	for (const auto& item : data.m_curves)
	{
		validate(item);
	}

}

inline void validate(const particle_module_force& data)
{
	validate(data.unnamed);

}

inline void validate(const particle_module_force_drag_graph& data)
{
	validate(data.unnamed);

	for (const auto& item : data.m_curves)
	{
		validate(item);
	}

}

inline void validate(const particle_module_gravity& data)
{
	validate(data.unnamed);

	validate(data.m_gravity_percentage);

}

inline void validate(const particle_module_intensity_graph& data)
{
	validate(data.unnamed);

	for (const auto& item : data.m_curves)
	{
		validate(item);
	}

}

inline void validate(const particle_module_temperature_graph& data)
{
	validate(data.unnamed);

	for (const auto& item : data.m_curves)
	{
		validate(item);
	}

}

inline void validate(const particle_module_parent_velocity_graph& data)
{
	validate(data.unnamed);

	for (const auto& item : data.m_curves)
	{
		validate(item);
	}

}

inline void validate(const particle_module_physics_light& data)
{
	validate(data.unnamed);

	validate(data.m_linked_asset_list);

}

inline void validate(const bounds& data)
{
	(void) data;
}

inline void validate(const particle_module_physics_ray_cast& data)
{
	validate(data.unnamed);

	validate(data.m_bounce);

	validate(data.m_bounds);

}

inline void validate(const particle_module_position_graph& data)
{
	validate(data.unnamed);

	for (const auto& item : data.m_curves)
	{
		validate(item);
	}

}

inline void validate(const particle_module_rotation_graph& data)
{
	validate(data.unnamed);

	for (const auto& item : data.m_curves)
	{
		validate(item);
	}

}

inline void validate(const particle_module_rotation_graph3_d& data)
{
	validate(data.unnamed);

	for (const auto& item : data.m_curves)
	{
		validate(item);
	}

}

inline void validate(const particle_module_scale_by_distance& data)
{
	validate(data.unnamed);

	for (const auto& item : data.m_curves)
	{
		validate(item);
	}

}

inline void validate(const particle_module_size_graph& data)
{
	validate(data.unnamed);

	for (const auto& item : data.m_curves)
	{
		validate(item);
	}

}

inline void validate(const particle_module_size_lerp& data)
{
	validate(data.unnamed);

}

inline void validate(const particle_module_velocity_graph& data)
{
	validate(data.unnamed);

	for (const auto& item : data.m_curves)
	{
		validate(item);
	}

}

inline void validate(const particle_module_vector_field_graph& data)
{
	validate(data.unnamed);

	for (const auto& item : data.m_curves)
	{
		validate(item);
	}

}

inline void validate(const particle_module_test_event_handler_data& data)
{
	validate(data.m_linked_asset_list);

}

inline void validate(const particle_module_test& data)
{
	validate(data.unnamed);

	if (std::ranges::none_of(particle_use_orientation_options_names, [&](const auto& entry) { return entry.second == data.m_use_orientation_options; }))
	{
		throw std::runtime_error("particle_module_test.m_use_orientation_options names no value");
	}

	if (std::ranges::none_of(particle_use_scale_options_names, [&](const auto& entry) { return entry.second == data.m_use_scale_options; }))
	{
		throw std::runtime_error("particle_module_test.m_use_scale_options names no value");
	}

	if (std::ranges::none_of(particle_use_velocity_options_names, [&](const auto& entry) { return entry.second == data.m_use_velocity_options; }))
	{
		throw std::runtime_error("particle_module_test.m_use_velocity_options names no value");
	}

	validate(data.m_scale_modifier);

	validate(data.m_velocity_modifier);

	validate(data.m_event_handler_data);

}

inline void validate(const particle_module_test_age& data)
{
	validate(data.unnamed);

}

inline void validate(const particle_module_test_birth& data)
{
	validate(data.unnamed);

}

inline void validate(const particle_module_test_death& data)
{
	validate(data.unnamed);

}

inline void validate(const particle_module_test_gravity& data)
{
	validate(data.unnamed);

}

inline void validate(const particle_module_test_impact& data)
{
	validate(data.unnamed);

}

inline void validate(const particle_module_test_pos& data)
{
	validate(data.unnamed);

}

inline void validate(const particle_module_test_time_in_state& data)
{
	validate(data.unnamed);

	validate(data.m_time_in_state);

}

inline void validate(const particle_module_def& data)
{
	std::visit([](const auto& value)
	{
		if constexpr (std::is_same_v<std::decay_t<decltype(value)>, std::monostate>)
		{
			throw std::runtime_error("particle_module_def.module_data holds no module");
		}
		else
		{
			validate(value);
		}
	}, data.module_data);

}

inline void validate(const particle_curve_control_point_def& data)
{
	(void) data;
}

} // namespace vfx
} // namespace zonetool::asset_data
