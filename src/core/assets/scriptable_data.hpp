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

// Neutral scriptable assets (docs/NEUTRAL_ASSETS.md), emitted by tools/gen_iw8_neutral.py from the IW8 structs and stream
// traversals: do not edit; change the spec (tools/neutral_families/scriptable.json) or the generator.
namespace zonetool::asset_data
{
namespace scriptable
{
inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 3> scriptable_mayhem_action_names{{
	{0, "play"},
	{1, "pause"},
	{2, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 15> scriptable_shader_param_type_names{{
	{0, "unknown"},
	{1, "scroll_rate_x"},
	{2, "scroll_rate_y"},
	{3, "scroll_rate_r"},
	{4, "rotation"},
	{5, "tiling_x"},
	{6, "tiling_y"},
	{7, "alpha"},
	{8, "emissive"},
	{9, "atlas_frame"},
	{10, "placeholder1"},
	{11, "placeholder2"},
	{12, "placeholder3"},
	{13, "placeholder4"},
	{14, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 2> scriptable_image_id_names{{
	{0, "unknown"},
	{1, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 14> scriptable_team_filter_names{{
	{0, "all"},
	{1, "bad"},
	{2, "axis"},
	{3, "allies"},
	{4, "three"},
	{5, "neutral"},
	{6, "dead"},
	{7, "my_team"},
	{8, "enemy_team"},
	{9, "all_but_my_team"},
	{10, "all_but_enemy_team"},
	{11, "enemy_team_and_me"},
	{12, "just_me"},
	{13, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 5> scriptable_material_override_type_names{{
	{0, "off"},
	{1, "refraction"},
	{2, "dissolve"},
	{3, "flag_amplitude_scale"},
	{4, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 5> scriptable_footstep_flag_names{{
	{0, "none"},
	{1, "left"},
	{2, "large"},
	{4, "stair"},
	{128, "actor"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 7> objective_state_t_names{{
	{0, "empty"},
	{1, "active"},
	{2, "invisible"},
	{3, "done"},
	{4, "current"},
	{5, "failed"},
	{6, "numstates"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 10> scriptable_damage_cause_names{{
	{0, "all"},
	{1, "splash_only"},
	{2, "no_splash"},
	{3, "no_melee"},
	{4, "none"},
	{5, "melee_only"},
	{6, "melee_and_splash_only"},
	{7, "crush_only"},
	{8, "crush_and_splash_only"},
	{9, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 7> scriptable_attackers_names{{
	{0, "all"},
	{1, "no_player"},
	{2, "player_only"},
	{3, "no_ai"},
	{4, "ai_only"},
	{5, "none"},
	{6, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 7> dyn_entity_spatial_activation_mode_names{{
	{0, "radial_priority_level_low"},
	{1, "radial_priority_level_high"},
	{2, "radial_priority_level_count"},
	{3, "priority_sparse_occluder"},
	{4, "priority_no_spatial_deactivatable"},
	{254, "invalid_priority_level"},
	{255, "priority_level_always_active"},
}};

struct scriptable;
struct scriptable_part_def;
struct scriptable_state_def;
struct scriptable_state_base_def;
struct scriptable_event_def;
struct scriptable_event_base_def;
struct scriptable_event_state_change_def;
struct scriptable_event_wait_def;
struct scriptable_event_random_def;
struct scriptable_event_script_def;
struct scriptable_event_model_def;
struct scriptable_event_model_def_model;
struct scriptable_event_model_def_composite_model;
struct scriptable_event_collision_def;
struct scriptable_event_animation_def;
struct scr_anim_t;
struct scriptable_event_hide_show_bone_def;
struct scriptable_event_disable_physics_sub_shape_def;
struct scriptable_event_note_track_def;
struct scriptable_note_track_def;
struct scriptable_event_chunk_dynent_def;
struct scriptable_event_spawn_dynent_def;
struct fx_combined_def;
struct scriptable_event_pfx_def;
struct scriptable_event_sound_def;
struct scriptable_event_explosion_def;
struct scriptable_event_light_def;
struct scriptable_event_sun_def;
struct scriptable_event_rumble_def;
struct scriptable_event_screenshake_def;
struct scriptable_event_part_damage_def;
struct scriptable_event_set_mayhem_def;
struct scriptable_event_play_mayhem_def;
struct scriptable_event_viewmodel_shader_param_def;
struct scriptable_event_viewmodel_change_image_def;
struct scriptable_event_client_view_selector_def;
struct scriptable_event_team_selector_def;
struct scriptable_event_add_model_def;
struct scriptable_event_apply_force_def;
struct scriptable_event_apply_angular_force_def;
struct scriptable_event_apply_constant_force_def;
struct scriptable_event_apply_constant_angular_force_def;
struct scriptable_event_compass_icon_def;
struct scriptable_event_material_override_def;
struct scriptable_event_dynamic_bone_noise_curve_def;
struct scriptable_event_move_def;
struct scriptable_event_footstep_def;
struct scriptable_event_gravity_arc_def;
struct scriptable_event_view_trigger_def;
struct scriptable_event_objective_def;
struct objective_settings;
struct scriptable_event_spatial_disable_def;
struct scriptable_event_hover_def;
struct scriptable_event_vehicle_blow_up_tire_def;
struct scriptable_event_dlc1_def;
struct scriptable_event_script_damage_def;
struct scriptable_state_simple_def;
struct scriptable_state_health_def;
struct scriptable_state_scripted_def;
struct scriptable_state_usable_def;
struct scriptable_damage_tag_override;

inline void validate(const scriptable& data);
inline void validate(const scriptable_part_def& data);
inline void validate(const scriptable_state_def& data);
inline void validate(const scriptable_state_base_def& data);
inline void validate(const scriptable_event_def& data);
inline void validate(const scriptable_event_base_def& data);
inline void validate(const scriptable_event_state_change_def& data);
inline void validate(const scriptable_event_wait_def& data);
inline void validate(const scriptable_event_random_def& data);
inline void validate(const scriptable_event_script_def& data);
inline void validate(const scriptable_event_model_def& data);
inline void validate(const scriptable_event_model_def_model& data);
inline void validate(const scriptable_event_model_def_composite_model& data);
inline void validate(const scriptable_event_collision_def& data);
inline void validate(const scriptable_event_animation_def& data);
inline void validate(const scr_anim_t& data);
inline void validate(const scriptable_event_hide_show_bone_def& data);
inline void validate(const scriptable_event_disable_physics_sub_shape_def& data);
inline void validate(const scriptable_event_note_track_def& data);
inline void validate(const scriptable_note_track_def& data);
inline void validate(const scriptable_event_chunk_dynent_def& data);
inline void validate(const scriptable_event_spawn_dynent_def& data);
inline void validate(const fx_combined_def& data);
inline void validate(const scriptable_event_pfx_def& data);
inline void validate(const scriptable_event_sound_def& data);
inline void validate(const scriptable_event_explosion_def& data);
inline void validate(const scriptable_event_light_def& data);
inline void validate(const scriptable_event_sun_def& data);
inline void validate(const scriptable_event_rumble_def& data);
inline void validate(const scriptable_event_screenshake_def& data);
inline void validate(const scriptable_event_part_damage_def& data);
inline void validate(const scriptable_event_set_mayhem_def& data);
inline void validate(const scriptable_event_play_mayhem_def& data);
inline void validate(const scriptable_event_viewmodel_shader_param_def& data);
inline void validate(const scriptable_event_viewmodel_change_image_def& data);
inline void validate(const scriptable_event_client_view_selector_def& data);
inline void validate(const scriptable_event_team_selector_def& data);
inline void validate(const scriptable_event_add_model_def& data);
inline void validate(const scriptable_event_apply_force_def& data);
inline void validate(const scriptable_event_apply_angular_force_def& data);
inline void validate(const scriptable_event_apply_constant_force_def& data);
inline void validate(const scriptable_event_apply_constant_angular_force_def& data);
inline void validate(const scriptable_event_compass_icon_def& data);
inline void validate(const scriptable_event_material_override_def& data);
inline void validate(const scriptable_event_dynamic_bone_noise_curve_def& data);
inline void validate(const scriptable_event_move_def& data);
inline void validate(const scriptable_event_footstep_def& data);
inline void validate(const scriptable_event_gravity_arc_def& data);
inline void validate(const scriptable_event_view_trigger_def& data);
inline void validate(const scriptable_event_objective_def& data);
inline void validate(const objective_settings& data);
inline void validate(const scriptable_event_spatial_disable_def& data);
inline void validate(const scriptable_event_hover_def& data);
inline void validate(const scriptable_event_vehicle_blow_up_tire_def& data);
inline void validate(const scriptable_event_dlc1_def& data);
inline void validate(const scriptable_event_script_damage_def& data);
inline void validate(const scriptable_state_simple_def& data);
inline void validate(const scriptable_state_health_def& data);
inline void validate(const scriptable_state_scripted_def& data);
inline void validate(const scriptable_state_usable_def& data);
inline void validate(const scriptable_damage_tag_override& data);

struct scriptable
{
	std::optional<std::string> name{};
	std::optional<std::string> next_scriptable_def{};
	std::int32_t flags{};
	std::optional<std::vector<scriptable_part_def>> parts{};
	std::uint32_t max_num_dyn_ents_required{};
	std::uint32_t part_count{};
	std::uint32_t server_instanced_part_count{};
	std::uint32_t server_controlled_part_count{};
	std::uint32_t max_num_dyn_ent_parts_base{};
	std::uint32_t max_num_dyn_ent_parts_for_spawning{};
	std::uint16_t event_stream_size_required_server{};
	std::uint16_t event_stream_size_required_client{};
	std::uint32_t usable_part_count{};
	std::uint32_t ff_mem_cost{};
	std::optional<std::string> animation_tree_name{};
	float view_radius{};
	bool network_lod_range_override{};
	float network_lod_range_override_distance{};
	std::optional<std::vector<std::optional<std::string>>> models{};
	std::string spatial_activation_mode{};

	bool operator==(const scriptable&) const = default;
};

struct scriptable_part_def
{
	std::optional<std::string> name{};
	std::optional<std::string> scr_name{};
	std::int32_t flags{};
	std::uint32_t flat_id{};
	std::uint32_t server_instance_flat_id{};
	std::uint32_t server_controlled_flat_id{};
	std::optional<std::string> tag_name{};
	std::optional<std::string> scr_tag_name{};
	std::optional<std::vector<scriptable_state_def>> states{};
	std::optional<std::vector<scriptable_part_def>> child_parts{};
	std::optional<std::vector<scriptable_damage_tag_override>> damage_tag_overrides{};
	std::uint16_t event_stream_buffer_offset_server{};
	std::uint16_t event_stream_buffer_offset_client{};

	bool operator==(const scriptable_part_def&) const = default;
};

struct scriptable_state_base_def
{
	std::optional<std::string> name{};
	std::int32_t flags{};
	std::optional<std::vector<scriptable_event_def>> events{};
	std::uint32_t events_id{};

	bool operator==(const scriptable_state_base_def&) const = default;
};

struct scriptable_state_simple_def
{
	std::optional<scriptable_state_base_def> base{};
	bool base_is_state_base{};

	bool operator==(const scriptable_state_simple_def&) const = default;
};

struct scriptable_state_health_def
{
	std::optional<scriptable_state_base_def> base{};
	std::int32_t health{};
	std::string valid_damage_cause{};
	std::string valid_attackers{};
	std::string valid_attacker_teams{};
	std::optional<std::vector<std::uint32_t>> weapon_classes{};
	std::optional<std::vector<std::optional<std::string>>> weapons{};
	std::int32_t minimum_damage{};
	float damage_propagation_to_parent{};
	float damage_propagation_to_child{};
	float damage_propagation_from_parent{};
	float damage_propagation_from_child{};
	std::optional<std::string> script_id{};
	std::optional<std::string> scr_script_id{};
	bool localize_splash_damage{};
	bool base_is_state_base{};

	bool operator==(const scriptable_state_health_def&) const = default;
};

struct scriptable_state_scripted_def
{
	std::optional<scriptable_state_base_def> base{};
	std::optional<std::string> script_id{};
	std::optional<std::string> scr_script_id{};
	bool base_is_state_base{};

	bool operator==(const scriptable_state_scripted_def&) const = default;
};

struct scriptable_state_usable_def
{
	std::optional<scriptable_state_base_def> base{};
	std::optional<std::string> script_id{};
	std::optional<std::string> scr_script_id{};
	float face_fov{};
	float use_fov{};
	float use_radius{};
	float use_radius_auto{};
	float use_radius_display{};
	bool use_radius_minimized{};
	bool goto_next_state_on_use{};
	bool allow_missing_tag{};
	bool usable_when_obstructed{};
	std::array<float, 3> use_offset{};
	std::array<float, 3> angle_offset{};
	std::uint32_t hint_hold_duration{};
	char priority{};
	bool use2_dfov{};
	std::int16_t hint_binding{};
	std::optional<std::string> hint_tag{};
	std::optional<std::string> hint_string{};
	std::optional<std::string> hint_string_disabled{};
	std::optional<std::string> hint_icon{};
	std::optional<std::string> hint_icon_image{};
	std::uint32_t hint_string_index{};
	std::uint32_t hint_string_disabled_index{};
	std::uint32_t hint_icon_index{};
	bool base_is_state_base{};

	bool operator==(const scriptable_state_usable_def&) const = default;
};

struct scriptable_state_def
{
	scriptable_state_base_def base{};
	std::variant<std::monostate, scriptable_state_simple_def, scriptable_state_health_def, scriptable_state_scripted_def, scriptable_state_usable_def> data{};

	bool operator==(const scriptable_state_def&) const = default;
};

struct scriptable_event_base_def
{
	std::optional<std::string> name{};
	std::int32_t flags{};

	bool operator==(const scriptable_event_base_def&) const = default;
};

struct scriptable_event_state_change_def
{
	std::optional<scriptable_event_base_def> base{};
	std::uint32_t part_reference{}; // the bits of flatId or partStringHash
	std::uint32_t state_idx{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_state_change_def&) const = default;
};

struct scriptable_event_wait_def
{
	std::optional<scriptable_event_base_def> base{};
	float delay_min{};
	float delay_max{};
	std::uint16_t event_stream_buffer_offset_server{};
	std::uint16_t event_stream_buffer_offset_client{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_wait_def&) const = default;
};

struct scriptable_event_random_def
{
	std::optional<scriptable_event_base_def> base{};
	float threshold{};
	std::optional<std::vector<scriptable_event_def>> events_a{};
	std::optional<std::vector<scriptable_event_def>> events_b{};
	std::uint16_t event_stream_buffer_offset_server{};
	std::uint16_t event_stream_buffer_offset_client{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_random_def&) const = default;
};

struct scriptable_event_script_def
{
	std::optional<scriptable_event_base_def> base{};
	std::optional<std::string> notification{};
	std::optional<std::string> scr_notification{};
	std::int32_t param{};
	bool do_notify_callback{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_script_def&) const = default;
};

struct scriptable_event_model_def_model
{
	std::optional<std::string> model{};

	bool operator==(const scriptable_event_model_def_model&) const = default;
};

struct scriptable_event_model_def_composite_model
{
	std::optional<std::string> composite_model{};

	bool operator==(const scriptable_event_model_def_composite_model&) const = default;
};

struct scriptable_event_model_def
{
	std::optional<scriptable_event_base_def> base{};
	std::variant<std::monostate, scriptable_event_model_def_model, scriptable_event_model_def_composite_model> data{};
	std::uint8_t hud_outline_color{};
	bool hud_outline_active{};
	bool hud_outline_fill{};
	bool never_moves{};
	bool dynamic_simulation{};
	bool activate_physics{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_model_def&) const = default;
};

struct scriptable_event_collision_def
{
	std::optional<scriptable_event_base_def> base{};
	std::optional<std::string> collmap_name{};
	bool ai_obstacle{};
	std::int32_t clipmap_c_model_index{};
	bool never_moves{};
	bool can_push{};
	bool can_touch{};
	bool destroy_on_exit{};
	std::optional<std::string> model{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_collision_def&) const = default;
};

struct scr_anim_t
{
	std::uint64_t link_pointer_or_s0{}; // the bits of linkPointer or __s0

	bool operator==(const scr_anim_t&) const = default;
};

struct scriptable_event_animation_def
{
	std::optional<scriptable_event_base_def> base{};
	std::optional<std::string> animation{};
	bool stateful{};
	bool override_animation{};
	bool pause_on_exit{};
	bool client_only_in_mp{};
	float start_time_min{};
	float start_time_max{};
	bool start_time_normalized{};
	float playback_rate_min{};
	float playback_rate_max{};
	float blend_time{};
	std::optional<std::vector<scriptable_event_def>> events_at_end{};
	std::uint16_t event_stream_buffer_offset_client{};
	std::uint16_t event_stream_buffer_offset_server{};
	std::array<scr_anim_t, 2> animation_index{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_animation_def&) const = default;
};

struct scriptable_event_hide_show_bone_def
{
	std::optional<scriptable_event_base_def> base{};
	std::optional<std::string> tag_name{};
	std::optional<std::string> scr_tag_name{};
	bool hide{};
	bool allow_missing_tag{};
	bool hide_show_children{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_hide_show_bone_def&) const = default;
};

struct scriptable_event_disable_physics_sub_shape_def
{
	std::optional<scriptable_event_base_def> base{};
	std::optional<std::string> mutable_shape_name{};
	std::uint64_t mutable_shape_hash{};
	bool allow_missing_shape{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_disable_physics_sub_shape_def&) const = default;
};

struct scriptable_event_note_track_def
{
	std::optional<scriptable_event_base_def> base{};
	std::optional<std::vector<scriptable_note_track_def>> note_tracks{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_note_track_def&) const = default;
};

struct scriptable_event_chunk_dynent_def
{
	std::optional<scriptable_event_base_def> base{};
	std::uint32_t part_reference{}; // the bits of flatId or partStringHash
	bool stateful{};
	bool allow_cause_velocity{};
	std::array<float, 3> launch_lin_vel{};
	std::array<float, 3> launch_ang_vel{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_chunk_dynent_def&) const = default;
};

struct fx_combined_def
{
	std::optional<std::string> particle_system_def{};

	bool operator==(const fx_combined_def&) const = default;
};

struct scriptable_event_spawn_dynent_def
{
	std::optional<scriptable_event_base_def> base{};
	bool stateful{};
	std::optional<std::string> model{};
	std::optional<std::string> tag_name{};
	std::optional<std::string> scr_tag_name{};
	bool allow_cause_velocity{};
	bool allow_nearest_vehicle_velocity{};
	bool world_space_lin_vel{};
	bool world_space_ang_vel{};
	bool random_range_lin_vel{};
	bool random_range_ang_vel{};
	std::array<float, 3> launch_lin_vel{};
	std::array<float, 3> launch_ang_vel{};
	std::array<float, 3> launch_lin_vel2{};
	std::array<float, 3> launch_ang_vel2{};
	std::uint16_t event_stream_buffer_offset_client{};
	bool allow_missing_tag{};
	bool use_root_on_missing_tag{};
	std::optional<std::string> effect_alias{};
	fx_combined_def effect_def{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_spawn_dynent_def&) const = default;
};

struct scriptable_event_pfx_def
{
	std::optional<scriptable_event_base_def> base{};
	bool stateful{};
	std::optional<std::vector<std::optional<std::string>>> scr_tag_names{};
	std::optional<std::vector<std::optional<std::string>>> scr_end_tag_names{};
	bool use_tag_angles{};
	std::optional<std::string> effect_alias{};
	fx_combined_def effect_def{};
	float loop_rate{};
	bool oneshot_looping{};
	std::uint16_t event_stream_buffer_offset_client{};
	bool allow_missing_tag{};
	bool use_root_on_missing_tag{};
	bool allow_nearest_vehicle_velocity{};
	bool is_viewmodel{};
	bool kill_on_exit{};
	bool use_dynamic_scripted_beam_length{};
	std::int32_t beam_bone_axis{};
	float beam_length{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_pfx_def&) const = default;
};

struct scriptable_event_sound_def
{
	std::optional<scriptable_event_base_def> base{};
	bool stateful{};
	std::optional<std::string> tag_name{};
	std::optional<std::string> scr_tag_name{};
	std::optional<std::string> sound_alias{};
	bool looping{};
	bool external_play{};
	bool allow_missing_tag{};
	bool use_root_on_missing_tag{};
	std::optional<std::string> explosion_refl_class{};
	bool do_ground_trace{};
	float ground_trace_length{};
	std::array<float, 3> world_offset{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_sound_def&) const = default;
};

struct scriptable_event_explosion_def
{
	std::optional<scriptable_event_base_def> base{};
	std::optional<std::string> tag_name{};
	std::optional<std::string> scr_tag_name{};
	std::optional<std::string> weapon{};
	float radius{};
	float dmg_outer{};
	float dmg_inner{};
	bool stateful{};
	bool allow_missing_tag{};
	bool use_root_on_missing_tag{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_explosion_def&) const = default;
};

struct scriptable_event_light_def
{
	std::optional<scriptable_event_base_def> base{};
	float intensity_scale_min{};
	float intensity_scale_max{};
	float radius_min{};
	float radius_max{};
	bool use_color{};
	std::array<float, 4> color_linear_srgb{};
	bool use_state_transition_time{};
	float transition_min{};
	float transition_max{};
	std::uint16_t event_stream_buffer_offset_client{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_light_def&) const = default;
};

struct scriptable_event_sun_def
{
	std::optional<scriptable_event_base_def> base{};
	bool use_intensity{};
	float intensity_override_min{};
	float intensity_override_max{};
	bool use_color{};
	std::array<float, 4> color_linear_srgb{};
	bool use_state_transition_time{};
	float transition_min{};
	float transition_max{};
	float pitch_min{};
	float pitch_max{};
	float heading_min{};
	float heading_max{};
	bool use_direction{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_sun_def&) const = default;
};

struct scriptable_event_rumble_def
{
	std::optional<scriptable_event_base_def> base{};
	bool stateful{};
	std::optional<std::string> tag_name{};
	std::optional<std::string> scr_tag_name{};
	std::optional<std::string> rumble{};
	std::optional<std::string> rumble_asset{};
	bool allow_missing_tag{};
	bool use_root_on_missing_tag{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_rumble_def&) const = default;
};

struct scriptable_event_screenshake_def
{
	std::optional<scriptable_event_base_def> base{};
	bool stateful{};
	bool is_earthquake{};
	bool is_screen_shake{};
	std::optional<std::string> tag_name{};
	std::optional<std::string> scr_tag_name{};
	float duration{};
	float duration_fade_up{};
	float duration_fade_down{};
	std::int32_t radius{};
	float scale_earthquake{};
	std::int32_t scale_pitch{};
	std::int32_t scale_yaw{};
	std::int32_t scale_roll{};
	std::int32_t frequency_pitch{};
	std::int32_t frequency_yaw{};
	std::int32_t frequency_roll{};
	bool allow_missing_tag{};
	bool use_root_on_missing_tag{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_screenshake_def&) const = default;
};

struct scriptable_event_part_damage_def
{
	std::optional<scriptable_event_base_def> base{};
	std::uint32_t part_reference{}; // the bits of flatId or partStringHash
	float amount{};
	bool destroy{};
	float amount_do_t{};
	float interval_do_t{};
	std::uint16_t event_stream_buffer_offset_server{};
	std::uint16_t event_stream_buffer_offset_client{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_part_damage_def&) const = default;
};

struct scriptable_event_set_mayhem_def
{
	std::optional<scriptable_event_base_def> base{};
	std::optional<std::string> mayhem{};
	std::uint16_t event_stream_buffer_offset_client{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_set_mayhem_def&) const = default;
};

struct scriptable_event_play_mayhem_def
{
	std::optional<scriptable_event_base_def> base{};
	std::string action{};
	std::uint16_t event_stream_buffer_offset_client{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_play_mayhem_def&) const = default;
};

struct scriptable_event_viewmodel_shader_param_def
{
	std::optional<scriptable_event_base_def> base{};
	std::string shader_param{};
	float float_value{};
	float place_holder_value1{};
	float place_holder_value2{};
	float place_holder_value3{};
	float place_holder_value4{};
	float transition_time{};
	std::uint16_t event_stream_buffer_offset_client{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_viewmodel_shader_param_def&) const = default;
};

struct scriptable_event_viewmodel_change_image_def
{
	std::optional<scriptable_event_base_def> base{};
	std::string image_id{};
	std::int32_t image_value{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_viewmodel_change_image_def&) const = default;
};

struct scriptable_event_client_view_selector_def
{
	std::optional<scriptable_event_base_def> base{};
	std::optional<std::vector<scriptable_event_def>> events1p{};
	std::optional<std::vector<scriptable_event_def>> events3p{};
	std::uint16_t event_stream_buffer_offset_client{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_client_view_selector_def&) const = default;
};

struct scriptable_event_team_selector_def
{
	std::optional<scriptable_event_base_def> base{};
	std::string team_filter{};
	bool test_local_player{};
	std::optional<std::vector<scriptable_event_def>> events_pass{};
	std::optional<std::vector<scriptable_event_def>> events_fail{};
	std::uint16_t event_stream_buffer_offset_client{};
	std::uint16_t event_stream_buffer_offset_server{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_team_selector_def&) const = default;
};

struct scriptable_event_add_model_def
{
	std::optional<scriptable_event_base_def> base{};
	std::optional<std::string> tag_name{};
	std::optional<std::string> scr_tag_name{};
	std::optional<std::string> model{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_add_model_def&) const = default;
};

struct scriptable_event_apply_force_def
{
	std::optional<scriptable_event_base_def> base{};
	bool world_space{};
	bool random_range{};
	std::array<float, 3> force_pos{};
	std::array<float, 3> force_vector{};
	std::array<float, 3> force_pos2{};
	std::array<float, 3> force_vector2{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_apply_force_def&) const = default;
};

struct scriptable_event_apply_angular_force_def
{
	std::optional<scriptable_event_base_def> base{};
	bool world_space{};
	bool random_range{};
	std::array<float, 3> force_vector{};
	std::array<float, 3> force_vector2{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_apply_angular_force_def&) const = default;
};

struct scriptable_event_apply_constant_force_def
{
	std::optional<scriptable_event_base_def> base{};
	bool world_space{};
	bool random_range{};
	std::array<float, 3> force_pos{};
	std::array<float, 3> force_vector{};
	std::array<float, 3> force_pos2{};
	std::array<float, 3> force_vector2{};
	std::uint16_t event_stream_buffer_offset_server{};
	std::uint16_t event_stream_buffer_offset_client{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_apply_constant_force_def&) const = default;
};

struct scriptable_event_apply_constant_angular_force_def
{
	std::optional<scriptable_event_base_def> base{};
	bool world_space{};
	bool random_range{};
	std::array<float, 3> force_vector{};
	std::array<float, 3> force_vector2{};
	std::uint16_t event_stream_buffer_offset_server{};
	std::uint16_t event_stream_buffer_offset_client{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_apply_constant_angular_force_def&) const = default;
};

struct scriptable_event_compass_icon_def
{
	std::optional<scriptable_event_base_def> base{};
	bool stateful{};
	bool friendly_compass_icon_always_visible{};
	bool enemy_compass_icon_always_visible{};
	std::optional<std::string> friendly_arrow{};
	std::optional<std::string> friendly_firing{};
	std::optional<std::string> friendly_chatting{};
	std::optional<std::string> friendly_yelling{};
	std::optional<std::string> party_arrow{};
	std::optional<std::string> party_firing{};
	std::optional<std::string> party_chatting{};
	std::optional<std::string> party_yelling{};
	std::optional<std::string> squad_arrow{};
	std::optional<std::string> squad_firing{};
	std::optional<std::string> enemy_compass_icon_directional{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_compass_icon_def&) const = default;
};

struct scriptable_event_material_override_def
{
	std::optional<scriptable_event_base_def> base{};
	std::string material_override_type{};
	std::int32_t transition_time{};
	std::uint8_t final_value{};
	std::uint16_t event_stream_buffer_offset_client{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_material_override_def&) const = default;
};

struct scriptable_event_dynamic_bone_noise_curve_def
{
	std::optional<scriptable_event_base_def> base{};
	bool stateful{};
	std::optional<std::string> curve{};
	float strength_multiplier{};
	float rate_multiplier{};
	bool is_viewmodel{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_dynamic_bone_noise_curve_def&) const = default;
};

struct scriptable_event_move_def
{
	std::optional<scriptable_event_base_def> base{};
	std::array<float, 3> origin_offset{};
	std::array<float, 3> angle_offset{};
	float seconds{};
	float seconds_accel{};
	float seconds_decel{};
	std::uint16_t event_stream_buffer_offset_server{};
	std::uint16_t event_stream_buffer_offset_client{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_move_def&) const = default;
};

struct scriptable_event_footstep_def
{
	std::optional<scriptable_event_base_def> base{};
	std::optional<std::string> tag_name{};
	std::optional<std::string> scr_tag_name{};
	std::optional<std::string> sound_alias{};
	bool allow_missing_tag{};
	bool use_root_on_missing_tag{};
	std::string flags{};
	std::optional<std::string> right_footstep_vfx{};
	std::optional<std::string> left_footstep_vfx{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_footstep_def&) const = default;
};

struct scriptable_event_gravity_arc_def
{
	std::optional<scriptable_event_base_def> base{};
	std::array<float, 3> angle_offset{};
	float gravity_scale{};
	float upward_velocity{};
	float yaw_offset_for_xy_direction{};
	bool end_at_initial_pose{};
	bool random_spin{};
	bool play_loot_trail_fx{};
	std::optional<std::vector<scriptable_event_def>> events_at_end{};
	std::uint16_t event_stream_buffer_offset_server{};
	std::uint16_t event_stream_buffer_offset_client{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_gravity_arc_def&) const = default;
};

struct scriptable_event_view_trigger_def
{
	std::optional<scriptable_event_base_def> base{};
	float trigger_distance_sq{};
	float trigger_fov_cos_half_angle{};
	std::uint16_t event_stream_buffer_offset_client{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_view_trigger_def&) const = default;
};

struct objective_settings
{
	std::array<std::uint8_t, 2> flags{};
	std::string state{};
	std::uint8_t background{};
	std::int32_t z_offset{};
	bool cached{};
	bool always_show_in_world{};
	bool always_snap_to_screen_edge{};
	std::uint8_t size{};
	std::uint32_t label{};
	std::uint32_t description{};
	std::uint32_t icon{};

	bool operator==(const objective_settings&) const = default;
};

struct scriptable_event_objective_def
{
	std::optional<scriptable_event_base_def> base{};
	objective_settings settings{};
	std::optional<std::string> description{};
	std::optional<std::string> icon{};
	std::optional<std::string> label{};
	std::optional<std::string> icon_image{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_objective_def&) const = default;
};

struct scriptable_event_spatial_disable_def
{
	std::optional<scriptable_event_base_def> base{};
	bool disabled{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_spatial_disable_def&) const = default;
};

struct scriptable_event_hover_def
{
	std::optional<scriptable_event_base_def> base{};
	float amplitude_up_down{};
	float frequency_up_down{};
	float frequency_pitch{};
	float frequency_yaw{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_hover_def&) const = default;
};

struct scriptable_event_vehicle_blow_up_tire_def
{
	std::optional<scriptable_event_base_def> base{};
	std::int32_t tire_index{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_vehicle_blow_up_tire_def&) const = default;
};

struct scriptable_event_dlc1_def
{
	std::optional<scriptable_event_base_def> base{};
	float dlc_float0{};
	float dlc_float1{};
	float dlc_float2{};
	float dlc_float3{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_dlc1_def&) const = default;
};

struct scriptable_event_script_damage_def
{
	std::optional<scriptable_event_base_def> base{};
	std::optional<std::string> notification{};
	std::optional<std::string> scr_notification{};
	std::int32_t param{};
	bool base_is_event_base{};

	bool operator==(const scriptable_event_script_damage_def&) const = default;
};

struct scriptable_event_def
{
	scriptable_event_base_def base{};
	std::variant<std::monostate, scriptable_event_state_change_def, scriptable_event_wait_def, scriptable_event_random_def, scriptable_event_script_def, scriptable_event_model_def, scriptable_event_collision_def, scriptable_event_animation_def, scriptable_event_hide_show_bone_def, scriptable_event_disable_physics_sub_shape_def, scriptable_event_note_track_def, scriptable_event_chunk_dynent_def, scriptable_event_spawn_dynent_def, scriptable_event_pfx_def, scriptable_event_sound_def, scriptable_event_explosion_def, scriptable_event_light_def, scriptable_event_sun_def, scriptable_event_rumble_def, scriptable_event_screenshake_def, scriptable_event_part_damage_def, scriptable_event_set_mayhem_def, scriptable_event_play_mayhem_def, scriptable_event_viewmodel_shader_param_def, scriptable_event_viewmodel_change_image_def, scriptable_event_client_view_selector_def, scriptable_event_team_selector_def, scriptable_event_add_model_def, scriptable_event_apply_force_def, scriptable_event_apply_angular_force_def, scriptable_event_apply_constant_force_def, scriptable_event_apply_constant_angular_force_def, scriptable_event_compass_icon_def, scriptable_event_material_override_def, scriptable_event_dynamic_bone_noise_curve_def, scriptable_event_move_def, scriptable_event_footstep_def, scriptable_event_gravity_arc_def, scriptable_event_view_trigger_def, scriptable_event_objective_def, scriptable_event_spatial_disable_def, scriptable_event_hover_def, scriptable_event_vehicle_blow_up_tire_def, scriptable_event_dlc1_def, scriptable_event_script_damage_def> data{};

	bool operator==(const scriptable_event_def&) const = default;
};

struct scriptable_note_track_def
{
	std::int32_t flags{};
	std::optional<std::string> note_track_name{};
	std::optional<std::string> scr_note_track_name{};
	std::optional<std::vector<scriptable_event_def>> events{};

	bool operator==(const scriptable_note_track_def&) const = default;
};

struct scriptable_damage_tag_override
{
	std::optional<std::string> tag{};
	std::optional<std::string> scr_tag{};

	bool operator==(const scriptable_damage_tag_override&) const = default;
};

inline void validate(const scriptable& data)
{
	if (!data.name)
	{
		throw std::runtime_error("scriptable has no name");
	}

	detail::name(*data.name, "scriptable");

	if (data.parts)
	{
		for (const auto& item : *data.parts)
		{
			validate(item);
		}
	}

	if (std::ranges::none_of(dyn_entity_spatial_activation_mode_names, [&](const auto& entry) { return entry.second == data.spatial_activation_mode; }))
	{
		throw std::runtime_error("scriptable.spatial_activation_mode names no value");
	}

	if ((data.parts && data.parts->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("scriptable has arrays that its count does not fit");
	}

	if ((data.models && data.models->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("scriptable has arrays that its count does not fit");
	}

}

inline void validate(const scriptable_part_def& data)
{
	if (data.states)
	{
		for (const auto& item : *data.states)
		{
			validate(item);
		}
	}

	if (data.child_parts)
	{
		for (const auto& item : *data.child_parts)
		{
			validate(item);
		}
	}

	if (data.damage_tag_overrides)
	{
		for (const auto& item : *data.damage_tag_overrides)
		{
			validate(item);
		}
	}

	if ((data.states && data.states->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("scriptable_part_def has arrays that its count does not fit");
	}

	if ((data.child_parts && data.child_parts->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("scriptable_part_def has arrays that its count does not fit");
	}

	if ((data.damage_tag_overrides && data.damage_tag_overrides->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("scriptable_part_def has arrays that its count does not fit");
	}

}

inline void validate(const scriptable_state_base_def& data)
{
	if (data.events)
	{
		for (const auto& item : *data.events)
		{
			validate(item);
		}
	}

	if ((data.events_id == 0) != !data.events)
	{
		throw std::runtime_error("scriptable_state_base_def has an id without its events or the reverse");
	}

	if ((data.events && data.events->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("scriptable_state_base_def has arrays that its count does not fit");
	}

}

inline void validate(const scriptable_state_simple_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (data.base_is_state_base && data.base)
	{
		throw std::runtime_error("scriptable_state_simple_def has a base that is both the member of its owner and a separate object");
	}

}

inline void validate(const scriptable_state_health_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (std::ranges::none_of(scriptable_damage_cause_names, [&](const auto& entry) { return entry.second == data.valid_damage_cause; }))
	{
		throw std::runtime_error("scriptable_state_health_def.valid_damage_cause names no value");
	}

	if (std::ranges::none_of(scriptable_attackers_names, [&](const auto& entry) { return entry.second == data.valid_attackers; }))
	{
		throw std::runtime_error("scriptable_state_health_def.valid_attackers names no value");
	}

	if (std::ranges::none_of(scriptable_team_filter_names, [&](const auto& entry) { return entry.second == data.valid_attacker_teams; }))
	{
		throw std::runtime_error("scriptable_state_health_def.valid_attacker_teams names no value");
	}

	if (data.base_is_state_base && data.base)
	{
		throw std::runtime_error("scriptable_state_health_def has a base that is both the member of its owner and a separate object");
	}

	if ((data.weapon_classes && data.weapon_classes->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("scriptable_state_health_def has arrays that its count does not fit");
	}

	if ((data.weapons && data.weapons->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("scriptable_state_health_def has arrays that its count does not fit");
	}

}

inline void validate(const scriptable_state_scripted_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (data.base_is_state_base && data.base)
	{
		throw std::runtime_error("scriptable_state_scripted_def has a base that is both the member of its owner and a separate object");
	}

}

inline void validate(const scriptable_state_usable_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (data.base_is_state_base && data.base)
	{
		throw std::runtime_error("scriptable_state_usable_def has a base that is both the member of its owner and a separate object");
	}

}

inline void validate(const scriptable_state_def& data)
{
	validate(data.base);

	std::visit([](const auto& value)
	{
		if constexpr (std::is_same_v<std::decay_t<decltype(value)>, std::monostate>)
		{
			throw std::runtime_error("scriptable_state_def.data holds no module");
		}
		else
		{
			validate(value);
		}
	}, data.data);

}

inline void validate(const scriptable_event_base_def& data)
{
	(void) data;
}

inline void validate(const scriptable_event_state_change_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_state_change_def has a base that is both the member of its owner and a separate object");
	}

}

inline void validate(const scriptable_event_wait_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_wait_def has a base that is both the member of its owner and a separate object");
	}

}

inline void validate(const scriptable_event_random_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (data.events_a)
	{
		for (const auto& item : *data.events_a)
		{
			validate(item);
		}
	}

	if (data.events_b)
	{
		for (const auto& item : *data.events_b)
		{
			validate(item);
		}
	}

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_random_def has a base that is both the member of its owner and a separate object");
	}

	if ((data.events_a && data.events_a->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("scriptable_event_random_def has arrays that its count does not fit");
	}

	if ((data.events_b && data.events_b->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("scriptable_event_random_def has arrays that its count does not fit");
	}

}

inline void validate(const scriptable_event_script_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_script_def has a base that is both the member of its owner and a separate object");
	}

}

inline void validate(const scriptable_event_model_def_model& data)
{
	(void) data;
}

inline void validate(const scriptable_event_model_def_composite_model& data)
{
	(void) data;
}

inline void validate(const scriptable_event_model_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	std::visit([](const auto& value)
	{
		if constexpr (std::is_same_v<std::decay_t<decltype(value)>, std::monostate>)
		{
			throw std::runtime_error("scriptable_event_model_def.data holds no module");
		}
		else
		{
			validate(value);
		}
	}, data.data);

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_model_def has a base that is both the member of its owner and a separate object");
	}

}

inline void validate(const scriptable_event_collision_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_collision_def has a base that is both the member of its owner and a separate object");
	}

}

inline void validate(const scr_anim_t& data)
{
	(void) data;
}

inline void validate(const scriptable_event_animation_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (data.events_at_end)
	{
		for (const auto& item : *data.events_at_end)
		{
			validate(item);
		}
	}

	for (const auto& item : data.animation_index)
	{
		validate(item);
	}

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_animation_def has a base that is both the member of its owner and a separate object");
	}

	if ((data.events_at_end && data.events_at_end->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("scriptable_event_animation_def has arrays that its count does not fit");
	}

}

inline void validate(const scriptable_event_hide_show_bone_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_hide_show_bone_def has a base that is both the member of its owner and a separate object");
	}

}

inline void validate(const scriptable_event_disable_physics_sub_shape_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_disable_physics_sub_shape_def has a base that is both the member of its owner and a separate object");
	}

}

inline void validate(const scriptable_event_note_track_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (data.note_tracks)
	{
		for (const auto& item : *data.note_tracks)
		{
			validate(item);
		}
	}

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_note_track_def has a base that is both the member of its owner and a separate object");
	}

	if ((data.note_tracks && data.note_tracks->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("scriptable_event_note_track_def has arrays that its count does not fit");
	}

}

inline void validate(const scriptable_event_chunk_dynent_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_chunk_dynent_def has a base that is both the member of its owner and a separate object");
	}

}

inline void validate(const fx_combined_def& data)
{
	(void) data;
}

inline void validate(const scriptable_event_spawn_dynent_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	validate(data.effect_def);

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_spawn_dynent_def has a base that is both the member of its owner and a separate object");
	}

}

inline void validate(const scriptable_event_pfx_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	validate(data.effect_def);

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_pfx_def has a base that is both the member of its owner and a separate object");
	}

	if ((data.scr_tag_names && data.scr_tag_names->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("scriptable_event_pfx_def has arrays that its count does not fit");
	}

	if ((data.scr_end_tag_names && data.scr_end_tag_names->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("scriptable_event_pfx_def has arrays that its count does not fit");
	}

}

inline void validate(const scriptable_event_sound_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_sound_def has a base that is both the member of its owner and a separate object");
	}

}

inline void validate(const scriptable_event_explosion_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_explosion_def has a base that is both the member of its owner and a separate object");
	}

}

inline void validate(const scriptable_event_light_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_light_def has a base that is both the member of its owner and a separate object");
	}

}

inline void validate(const scriptable_event_sun_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_sun_def has a base that is both the member of its owner and a separate object");
	}

}

inline void validate(const scriptable_event_rumble_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_rumble_def has a base that is both the member of its owner and a separate object");
	}

}

inline void validate(const scriptable_event_screenshake_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_screenshake_def has a base that is both the member of its owner and a separate object");
	}

}

inline void validate(const scriptable_event_part_damage_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_part_damage_def has a base that is both the member of its owner and a separate object");
	}

}

inline void validate(const scriptable_event_set_mayhem_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_set_mayhem_def has a base that is both the member of its owner and a separate object");
	}

}

inline void validate(const scriptable_event_play_mayhem_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (std::ranges::none_of(scriptable_mayhem_action_names, [&](const auto& entry) { return entry.second == data.action; }))
	{
		throw std::runtime_error("scriptable_event_play_mayhem_def.action names no value");
	}

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_play_mayhem_def has a base that is both the member of its owner and a separate object");
	}

}

inline void validate(const scriptable_event_viewmodel_shader_param_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (std::ranges::none_of(scriptable_shader_param_type_names, [&](const auto& entry) { return entry.second == data.shader_param; }))
	{
		throw std::runtime_error("scriptable_event_viewmodel_shader_param_def.shader_param names no value");
	}

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_viewmodel_shader_param_def has a base that is both the member of its owner and a separate object");
	}

}

inline void validate(const scriptable_event_viewmodel_change_image_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (std::ranges::none_of(scriptable_image_id_names, [&](const auto& entry) { return entry.second == data.image_id; }))
	{
		throw std::runtime_error("scriptable_event_viewmodel_change_image_def.image_id names no value");
	}

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_viewmodel_change_image_def has a base that is both the member of its owner and a separate object");
	}

}

inline void validate(const scriptable_event_client_view_selector_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (data.events1p)
	{
		for (const auto& item : *data.events1p)
		{
			validate(item);
		}
	}

	if (data.events3p)
	{
		for (const auto& item : *data.events3p)
		{
			validate(item);
		}
	}

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_client_view_selector_def has a base that is both the member of its owner and a separate object");
	}

	if ((data.events1p && data.events1p->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("scriptable_event_client_view_selector_def has arrays that its count does not fit");
	}

	if ((data.events3p && data.events3p->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("scriptable_event_client_view_selector_def has arrays that its count does not fit");
	}

}

inline void validate(const scriptable_event_team_selector_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (std::ranges::none_of(scriptable_team_filter_names, [&](const auto& entry) { return entry.second == data.team_filter; }))
	{
		throw std::runtime_error("scriptable_event_team_selector_def.team_filter names no value");
	}

	if (data.events_pass)
	{
		for (const auto& item : *data.events_pass)
		{
			validate(item);
		}
	}

	if (data.events_fail)
	{
		for (const auto& item : *data.events_fail)
		{
			validate(item);
		}
	}

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_team_selector_def has a base that is both the member of its owner and a separate object");
	}

	if ((data.events_pass && data.events_pass->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("scriptable_event_team_selector_def has arrays that its count does not fit");
	}

	if ((data.events_fail && data.events_fail->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("scriptable_event_team_selector_def has arrays that its count does not fit");
	}

}

inline void validate(const scriptable_event_add_model_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_add_model_def has a base that is both the member of its owner and a separate object");
	}

}

inline void validate(const scriptable_event_apply_force_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_apply_force_def has a base that is both the member of its owner and a separate object");
	}

}

inline void validate(const scriptable_event_apply_angular_force_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_apply_angular_force_def has a base that is both the member of its owner and a separate object");
	}

}

inline void validate(const scriptable_event_apply_constant_force_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_apply_constant_force_def has a base that is both the member of its owner and a separate object");
	}

}

inline void validate(const scriptable_event_apply_constant_angular_force_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_apply_constant_angular_force_def has a base that is both the member of its owner and a separate object");
	}

}

inline void validate(const scriptable_event_compass_icon_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_compass_icon_def has a base that is both the member of its owner and a separate object");
	}

}

inline void validate(const scriptable_event_material_override_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (std::ranges::none_of(scriptable_material_override_type_names, [&](const auto& entry) { return entry.second == data.material_override_type; }))
	{
		throw std::runtime_error("scriptable_event_material_override_def.material_override_type names no value");
	}

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_material_override_def has a base that is both the member of its owner and a separate object");
	}

}

inline void validate(const scriptable_event_dynamic_bone_noise_curve_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_dynamic_bone_noise_curve_def has a base that is both the member of its owner and a separate object");
	}

}

inline void validate(const scriptable_event_move_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_move_def has a base that is both the member of its owner and a separate object");
	}

}

inline void validate(const scriptable_event_footstep_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (std::ranges::none_of(scriptable_footstep_flag_names, [&](const auto& entry) { return entry.second == data.flags; }))
	{
		throw std::runtime_error("scriptable_event_footstep_def.flags names no value");
	}

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_footstep_def has a base that is both the member of its owner and a separate object");
	}

}

inline void validate(const scriptable_event_gravity_arc_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (data.events_at_end)
	{
		for (const auto& item : *data.events_at_end)
		{
			validate(item);
		}
	}

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_gravity_arc_def has a base that is both the member of its owner and a separate object");
	}

	if ((data.events_at_end && data.events_at_end->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("scriptable_event_gravity_arc_def has arrays that its count does not fit");
	}

}

inline void validate(const scriptable_event_view_trigger_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_view_trigger_def has a base that is both the member of its owner and a separate object");
	}

}

inline void validate(const objective_settings& data)
{
	if (std::ranges::none_of(objective_state_t_names, [&](const auto& entry) { return entry.second == data.state; }))
	{
		throw std::runtime_error("objective_settings.state names no value");
	}

}

inline void validate(const scriptable_event_objective_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	validate(data.settings);

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_objective_def has a base that is both the member of its owner and a separate object");
	}

}

inline void validate(const scriptable_event_spatial_disable_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_spatial_disable_def has a base that is both the member of its owner and a separate object");
	}

}

inline void validate(const scriptable_event_hover_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_hover_def has a base that is both the member of its owner and a separate object");
	}

}

inline void validate(const scriptable_event_vehicle_blow_up_tire_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_vehicle_blow_up_tire_def has a base that is both the member of its owner and a separate object");
	}

}

inline void validate(const scriptable_event_dlc1_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_dlc1_def has a base that is both the member of its owner and a separate object");
	}

}

inline void validate(const scriptable_event_script_damage_def& data)
{
	if (data.base)
	{
		validate(*data.base);
	}

	if (data.base_is_event_base && data.base)
	{
		throw std::runtime_error("scriptable_event_script_damage_def has a base that is both the member of its owner and a separate object");
	}

}

inline void validate(const scriptable_event_def& data)
{
	validate(data.base);

	std::visit([](const auto& value)
	{
		if constexpr (std::is_same_v<std::decay_t<decltype(value)>, std::monostate>)
		{
			throw std::runtime_error("scriptable_event_def.data holds no module");
		}
		else
		{
			validate(value);
		}
	}, data.data);

}

inline void validate(const scriptable_note_track_def& data)
{
	if (data.events)
	{
		for (const auto& item : *data.events)
		{
			validate(item);
		}
	}

	if ((data.events && data.events->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("scriptable_note_track_def has arrays that its count does not fit");
	}

}

inline void validate(const scriptable_damage_tag_override& data)
{
	(void) data;
}

} // namespace scriptable
} // namespace zonetool::asset_data
