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

// Neutral content assets (docs/NEUTRAL_ASSETS.md), emitted by tools/gen_iw8_neutral.py from the IW8 structs and stream
// traversals: do not edit; change the spec (tools/neutral_families/content.json) or the generator.
namespace zonetool::asset_data
{
namespace content
{
inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 4> attachment_type_names{{
	{0, "scope"},
	{1, "underbarrel"},
	{2, "other"},
	{3, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 5> alt_mode_player_anim_type_t_names{{
	{0, "altmodeplayeranimtype_none"},
	{1, "altmodeplayeranimtype_launcher"},
	{2, "altmodeplayeranimtype_shotgun"},
	{3, "altmodeplayeranimtype_fireselector"},
	{4, "num_altmodeplayeranimtypes"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 12> weap_type_t_names{{
	{0, "none"},
	{1, "model_only"},
	{2, "bullet"},
	{3, "grenade"},
	{4, "projectile"},
	{5, "riotshield"},
	{6, "script"},
	{7, "shield"},
	{8, "charge_shield"},
	{9, "location_select"},
	{10, "equip_deploy"},
	{11, "num"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 16> weap_class_t_names{{
	{0, "rifle"},
	{1, "sniper"},
	{2, "mg"},
	{3, "smg"},
	{4, "spread"},
	{5, "pistol"},
	{6, "grenade"},
	{7, "rocketlauncher"},
	{8, "turret"},
	{9, "throwingknife"},
	{10, "non_player"},
	{11, "item"},
	{12, "beam"},
	{13, "ball"},
	{14, "none"},
	{15, "num"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 7> weap_fire_type_t_names{{
	{0, "firetype_fullauto"},
	{1, "firetype_singleshot"},
	{2, "firetype_burst"},
	{3, "firetype_doublebarrel"},
	{4, "firetype_doublebarrel_dualtrigger"},
	{5, "firetype_beam"},
	{6, "firetypecount"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 12> dynamic_attachment_type_names{{
	{0, "muzzle"},
	{1, "underbarrel"},
	{2, "laser"},
	{3, "guard"},
	{4, "magazine"},
	{5, "receiver"},
	{6, "scope"},
	{7, "stock"},
	{8, "firetype"},
	{9, "barrel"},
	{10, "other"},
	{11, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 4> weapon_flag_override_names{{
	{0, "no_override"},
	{1, "false"},
	{2, "true"},
	{3, "num"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 3> tracer_style_t_names{{
	{0, "tracer"},
	{1, "beam"},
	{2, "num"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 7> penetrate_type_names{{
	{0, "none"},
	{1, "small"},
	{2, "medium"},
	{3, "large"},
	{4, "maximum"},
	{5, "ricochet"},
	{6, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 3> reticle_type_names{{
	{0, "four_sided"},
	{1, "one_piece"},
	{2, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 3> target_assist_type_t_names{{
	{0, "default"},
	{1, "cone"},
	{2, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 6> target_assist_behavior_t_names{{
	{0, "default"},
	{1, "direct_damage"},
	{2, "target_only"},
	{3, "beam"},
	{4, "smart_pellet"},
	{5, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 13> anim_damage_type_names{{
	{0, "none"},
	{1, "explosion_light"},
	{2, "explosion"},
	{3, "melee"},
	{4, "melee_dog"},
	{5, "melee_alien"},
	{6, "energy_bullet"},
	{7, "fire_explosion"},
	{8, "bullet_sm"},
	{9, "bullet_md"},
	{10, "bullet_lg"},
	{11, "fall"},
	{12, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 10> ads_offset_interpolation_type_names{{
	{0, "linear"},
	{1, "easeinoutsine"},
	{2, "easeinoutsmootherstep"},
	{3, "easeincubic"},
	{4, "easeoutcubic"},
	{5, "easeinquartic"},
	{6, "easeoutquartic"},
	{7, "easeinexpo"},
	{8, "easeoutexpo"},
	{9, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 3> weap_overlay_reticle_t_names{{
	{0, "none"},
	{1, "crosshair"},
	{2, "num"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 8> ammo_counter_clip_type_t_names{{
	{0, "none"},
	{1, "magazine"},
	{2, "shortmagazine"},
	{3, "shotgun"},
	{4, "rocket"},
	{5, "beltfed"},
	{6, "altweapon"},
	{7, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 9> weap_proj_exposion_t_names{{
	{0, "grenade"},
	{1, "rocket"},
	{2, "flashbang"},
	{3, "molotov"},
	{4, "none"},
	{5, "dud"},
	{6, "smoke"},
	{7, "heavy"},
	{8, "num"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 8> weap_stickiness_type_names{{
	{0, "none"},
	{1, "all"},
	{2, "all_orient"},
	{3, "ground"},
	{4, "ground_with_yaw"},
	{5, "ground_orient"},
	{6, "knife"},
	{7, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 4> ads_alt_switch_interp_type_names{{
	{0, "linear"},
	{1, "cosine"},
	{2, "sine"},
	{3, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 4> hitmarker_type_names{{
	{0, "small"},
	{1, "medium"},
	{2, "large"},
	{3, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 4> animation_controller_names{{
	{0, "none"},
	{1, "player"},
	{2, "dog"},
	{3, "num"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 8> lb_col_type_names{{
	{0, "number"},
	{1, "time"},
	{2, "levelxp"},
	{3, "prestige"},
	{4, "bignumber"},
	{5, "percent"},
	{6, "time_full"},
	{7, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 5> lb_agg_type_names{{
	{0, "min"},
	{1, "max"},
	{2, "sum"},
	{3, "last"},
	{4, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 5> lb_update_type_names{{
	{0, "normal"},
	{1, "rank"},
	{2, "combine"},
	{3, "virtual"},
	{4, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 4> gfx_lightmap_type_names{{
	{0, "regular"},
	{1, "regular_uncompressed"},
	{2, "ab_lighting"},
	{3, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 15> vehicle_type_names{{
	{0, "wheels_4"},
	{1, "plane"},
	{2, "boat"},
	{3, "artillery"},
	{4, "helicopter"},
	{5, "snowmobile"},
	{6, "submarine"},
	{7, "ugv"},
	{8, "spaceship"},
	{9, "treaded"},
	{10, "armoured"},
	{11, "car"},
	{12, "aircraft"},
	{13, "seacraft"},
	{14, "type_count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 4> vehicle_physics_netcode_type_names{{
	{0, "client_pred"},
	{1, "client_auth"},
	{2, "server_auth"},
	{3, "client_pred_sb"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 12> vehicle_physics_game_profile_names{{
	{0, "bradley"},
	{1, "wheelson"},
	{2, "jeep"},
	{3, "atvquad"},
	{4, "atvquadold"},
	{5, "technical"},
	{6, "fixedwing"},
	{7, "fixedwing_rcplane"},
	{8, "rotarywing"},
	{9, "boat"},
	{10, "twowheeled"},
	{11, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 28> vehicle_physics_anim_profile_names{{
	{0, "atango"},
	{1, "tromeo"},
	{2, "coscar"},
	{3, "stango"},
	{4, "vindia"},
	{5, "bromeo"},
	{6, "buniform"},
	{7, "skilo"},
	{8, "foscar"},
	{9, "lbravo"},
	{10, "blima"},
	{11, "decho"},
	{12, "techo"},
	{13, "pindia"},
	{14, "hindia"},
	{15, "asierra"},
	{16, "tromeotrac"},
	{17, "umike"},
	{18, "zuniform"},
	{19, "mkilo"},
	{20, "palfa"},
	{21, "whotel"},
	{22, "foxtrot"},
	{23, "zoscar"},
	{24, "scooter"},
	{25, "expansion3"},
	{26, "expansion4"},
	{27, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 4> vehicle_wheel_spin_bone_type_names{{
	{0, "none"},
	{1, "childbone"},
	{2, "bone"},
	{3, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 4> veh_cam_z_offset_mode_names{{
	{0, "world"},
	{1, "vehicle"},
	{2, "view"},
	{3, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 4> vehicle_turret_fire_type_names{{
	{0, "single_fire"},
	{1, "dual_fire"},
	{2, "alt_fire"},
	{3, "fire_type_count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 3> x_cam_aperture_type_names{{
	{0, "xcam_fov_aperture"},
	{1, "xcam_focal_length_aperture"},
	{2, "count_xcam_aperture"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 3> dyn_entity_basis_names{{
	{0, "model"},
	{1, "brush"},
	{2, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 8> dyn_entity_type_names{{
	{0, "invalid"},
	{1, "clutter"},
	{2, "clutter_noshadow"},
	{3, "scriptableinst"},
	{4, "scriptablephysics"},
	{5, "linked"},
	{6, "linked_noshadow"},
	{7, "count"},
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

struct attachment;
struct att_ammo_general;
struct att_ammo_type;
struct att_impact;
struct att_ammo_tracer;
struct att_penetration;
struct att_sight;
struct att_hold_breath;
struct att_reload;
struct att_add_ons;
struct att_laser;
struct laser_settings;
struct att_post;
struct att_general;
struct att_aim_assist;
struct att_target_assist;
struct att_ammunition;
struct att_damage;
struct weapon_damage_info;
struct weapon_damage_common;
struct att_melee;
struct att_idle_settings;
struct advanced_idle_settings;
struct att_ads_settings;
struct zoom_settings;
struct scene_zoom_settings;
struct weapon_zoom_settings;
struct att_ads_gun_bob_settings;
struct att_ads_reload_settings;
struct att_scope_drift_settings;
struct att_ads_stance_scales;
struct weapon_offset_pattern_scale_info;
struct weapon_offset_pattern_scale;
struct att_hip_spread;
struct att_slide_spread;
struct att_gun_kick;
struct att_gun_tilt;
struct att_kick_scaling;
struct att_view_kick;
struct att_ads_overlay;
struct ads_overlay;
struct att_outline;
struct att_ui;
struct att_rumbles;
struct att_projectile;
struct fx_combined_def;
struct snd_alias_lookup;
struct att_charged;
struct ads_alt_switch;
struct att_regeneration;
struct att_heat;
struct att_movement;
struct att_burst;
struct att_hitmarker;
struct att_ads_movement;
struct ballistic_info;
struct ballistic_info_calculated;
struct sway_settings;
struct hip_sway_settings;
struct common_sway_settings;
struct ads_sway_settings;
struct advanced_hip_sway_settings;
struct att_dlc_overrides;
struct att_dlc_scalars;
struct animclass;
struct animation_state_machine;
struct animation_state;
struct animation_entry;
struct anim_alias;
struct anim_alias_info;
struct animation_aim_set;
struct luafile;
struct leaderboarddef;
struct lb_column_def;
struct virtualleaderboarddef;
struct lb_vr_column_def;
struct clothasset;
struct cloth_asset_bone_mapping_set;
struct cloth_asset_bone_mapping;
struct cloth_asset_simulation_lod;
struct cloth_asset_sound;
struct stterrain;
struct st_disk_terrain_surface;
struct gfx_index_buffer;
struct st_terrain_grid;
struct st_terrain_grid_level;
struct gfx_placement;
struct bounds;
struct st_terrain_material_layer;
struct hlsl_terrain_layer_data_code;
struct st_terrain_displacement_scale;
struct st_terrain_clutter_layer;
struct gfx_wrapped_buffer;
struct gfx_wrapped_base_buffer;
struct gfx_buffer_resource;
struct gfx_shader_buffer_view;
struct st_clutter_sample_bitmask;
struct st_terrain_maps;
struct st_clutter_sample_points;
struct st_lightmap;
struct collisiontile;
struct static_model_collision_compressed_model_list;
struct static_model_collision_compressed_model;
struct static_model_collision_compressed_instance;
struct vehicle;
struct vehicle_physics_def;
struct vehicle_physics_sound_group;
struct vehicle_physics_vfx_group;
struct spaceship_anim_parts;
struct vehicle_physics_rev_modifier;
struct xcam;
struct x_cam_target_model;
struct x_cam_target_model_frame;
struct x_cam_camera;
struct x_cam_frame;
struct x_cam_camera_switch_params;
struct x_cam_frame_name;
struct x_anim_notify_info;
struct dynentitylist;
struct dyn_entity_def;
struct client_entity_link_to_def;
struct spatial_partition_population_node;
struct spatial_partition_transient_population;

inline void validate(const attachment& data);
inline void validate(const att_ammo_general& data);
inline void validate(const att_ammo_type& data);
inline void validate(const att_impact& data);
inline void validate(const att_ammo_tracer& data);
inline void validate(const att_penetration& data);
inline void validate(const att_sight& data);
inline void validate(const att_hold_breath& data);
inline void validate(const att_reload& data);
inline void validate(const att_add_ons& data);
inline void validate(const att_laser& data);
inline void validate(const laser_settings& data);
inline void validate(const att_post& data);
inline void validate(const att_general& data);
inline void validate(const att_aim_assist& data);
inline void validate(const att_target_assist& data);
inline void validate(const att_ammunition& data);
inline void validate(const att_damage& data);
inline void validate(const weapon_damage_info& data);
inline void validate(const weapon_damage_common& data);
inline void validate(const att_melee& data);
inline void validate(const att_idle_settings& data);
inline void validate(const advanced_idle_settings& data);
inline void validate(const att_ads_settings& data);
inline void validate(const zoom_settings& data);
inline void validate(const scene_zoom_settings& data);
inline void validate(const weapon_zoom_settings& data);
inline void validate(const att_ads_gun_bob_settings& data);
inline void validate(const att_ads_reload_settings& data);
inline void validate(const att_scope_drift_settings& data);
inline void validate(const att_ads_stance_scales& data);
inline void validate(const weapon_offset_pattern_scale_info& data);
inline void validate(const weapon_offset_pattern_scale& data);
inline void validate(const att_hip_spread& data);
inline void validate(const att_slide_spread& data);
inline void validate(const att_gun_kick& data);
inline void validate(const att_gun_tilt& data);
inline void validate(const att_kick_scaling& data);
inline void validate(const att_view_kick& data);
inline void validate(const att_ads_overlay& data);
inline void validate(const ads_overlay& data);
inline void validate(const att_outline& data);
inline void validate(const att_ui& data);
inline void validate(const att_rumbles& data);
inline void validate(const att_projectile& data);
inline void validate(const fx_combined_def& data);
inline void validate(const snd_alias_lookup& data);
inline void validate(const att_charged& data);
inline void validate(const ads_alt_switch& data);
inline void validate(const att_regeneration& data);
inline void validate(const att_heat& data);
inline void validate(const att_movement& data);
inline void validate(const att_burst& data);
inline void validate(const att_hitmarker& data);
inline void validate(const att_ads_movement& data);
inline void validate(const ballistic_info& data);
inline void validate(const ballistic_info_calculated& data);
inline void validate(const sway_settings& data);
inline void validate(const hip_sway_settings& data);
inline void validate(const common_sway_settings& data);
inline void validate(const ads_sway_settings& data);
inline void validate(const advanced_hip_sway_settings& data);
inline void validate(const att_dlc_overrides& data);
inline void validate(const att_dlc_scalars& data);
inline void validate(const animclass& data);
inline void validate(const animation_state_machine& data);
inline void validate(const animation_state& data);
inline void validate(const animation_entry& data);
inline void validate(const anim_alias& data);
inline void validate(const anim_alias_info& data);
inline void validate(const animation_aim_set& data);
inline void validate(const luafile& data);
inline void validate(const leaderboarddef& data);
inline void validate(const lb_column_def& data);
inline void validate(const virtualleaderboarddef& data);
inline void validate(const lb_vr_column_def& data);
inline void validate(const clothasset& data);
inline void validate(const cloth_asset_bone_mapping_set& data);
inline void validate(const cloth_asset_bone_mapping& data);
inline void validate(const cloth_asset_simulation_lod& data);
inline void validate(const cloth_asset_sound& data);
inline void validate(const stterrain& data);
inline void validate(const st_disk_terrain_surface& data);
inline void validate(const gfx_index_buffer& data);
inline void validate(const st_terrain_grid& data);
inline void validate(const st_terrain_grid_level& data);
inline void validate(const gfx_placement& data);
inline void validate(const bounds& data);
inline void validate(const st_terrain_material_layer& data);
inline void validate(const hlsl_terrain_layer_data_code& data);
inline void validate(const st_terrain_displacement_scale& data);
inline void validate(const st_terrain_clutter_layer& data);
inline void validate(const gfx_wrapped_buffer& data);
inline void validate(const gfx_wrapped_base_buffer& data);
inline void validate(const gfx_buffer_resource& data);
inline void validate(const gfx_shader_buffer_view& data);
inline void validate(const st_clutter_sample_bitmask& data);
inline void validate(const st_terrain_maps& data);
inline void validate(const st_clutter_sample_points& data);
inline void validate(const st_lightmap& data);
inline void validate(const collisiontile& data);
inline void validate(const static_model_collision_compressed_model_list& data);
inline void validate(const static_model_collision_compressed_model& data);
inline void validate(const static_model_collision_compressed_instance& data);
inline void validate(const vehicle& data);
inline void validate(const vehicle_physics_def& data);
inline void validate(const vehicle_physics_sound_group& data);
inline void validate(const vehicle_physics_vfx_group& data);
inline void validate(const spaceship_anim_parts& data);
inline void validate(const vehicle_physics_rev_modifier& data);
inline void validate(const xcam& data);
inline void validate(const x_cam_target_model& data);
inline void validate(const x_cam_target_model_frame& data);
inline void validate(const x_cam_camera& data);
inline void validate(const x_cam_frame& data);
inline void validate(const x_cam_camera_switch_params& data);
inline void validate(const x_cam_frame_name& data);
inline void validate(const x_anim_notify_info& data);
inline void validate(const dynentitylist& data);
inline void validate(const dyn_entity_def& data);
inline void validate(const client_entity_link_to_def& data);
inline void validate(const spatial_partition_population_node& data);
inline void validate(const spatial_partition_transient_population& data);

struct att_ammo_general
{
	std::string fire_type{};
	std::int32_t burst_count{};

	bool operator==(const att_ammo_general&) const = default;
};

struct att_ammo_type
{
	std::optional<std::string> sz_ammo_name{};

	bool operator==(const att_ammo_type&) const = default;
};

struct att_impact
{
	std::int32_t vfx_impact_type{};

	bool operator==(const att_impact&) const = default;
};

struct att_ammo_tracer
{
	std::optional<std::string> tracer_type{};
	std::string tracer_style{};

	bool operator==(const att_ammo_tracer&) const = default;
};

struct att_penetration
{
	std::string penetrate_type{};
	float penetrate_multiplier{};
	bool rifle_bullet{};
	bool armor_piercing{};
	bool impaling{};

	bool operator==(const att_penetration&) const = default;
};

struct att_sight
{
	float blur_scene_ads_in_fraction{};
	float blur_scene_ads_out_fraction{};
	bool ads_smooth_fade{};
	float ads_smooth_fade_in_begin{};
	float ads_smooth_fade_in_end{};
	float ads_smooth_fade_out_begin{};
	float ads_smooth_fade_out_end{};
	bool aim_down_sight{};
	bool ads_fire{};
	bool rechamber_while_ads{};
	bool no_ads_when_mag_empty{};
	bool hide_rail_with_this_scope{};
	bool force_iron_sight_down{};
	bool use_scope_drift{};
	bool use_dual_fov{};
	bool show_view_model_in_dual_fov{};
	bool synced_fov_in_dual_fov{};
	bool block_nvgads{};
	float scope_lens_inner_disk{};
	float scope_lens_inner_disk_mag{};
	float scope_lens_outer_ring{};
	float scope_lens_outer_ring_mag{};
	float scope_lens_radius{};
	float scope_lens_color_red{};
	float scope_lens_color_green{};
	float scope_lens_color_blue{};
	float scope_lens_brightness{};
	float scope_lens_fade_start{};
	float scope_lens_fade_end{};
	float scope_eye_relief_focus_distance{};
	float scope_eye_relief_focuse_uv_scale{};
	float scope_eye_relief_out_of_focus_distance{};
	float scope_eye_relief_out_of_focuse_uv_scale{};
	float scope_eye_relief_idle_movement_scale{};
	float scope_eye_relief_full_speed_movement_scale{};
	float scope_eye_relief_max_movement{};
	float scope_eye_relief_hip_movement_scale{};
	float scope_eye_relief_hip_max_movement{};
	float scope_eye_relief_idle_sway_freq{};
	float scope_eye_relief_idle_sway_movement{};
	float scope_eye_relief_full_speed_sway_freq{};
	float scope_eye_relief_full_speed_sway_movement{};

	bool operator==(const att_sight&) const = default;
};

struct att_hold_breath
{
	bool can_hold_breath{};

	bool operator==(const att_hold_breath&) const = default;
};

struct att_reload
{
	bool no_partial_reload{};
	bool segmented_reload{};
	bool reload_disabled{};

	bool operator==(const att_reload&) const = default;
};

struct att_add_ons
{
	bool silenced{};
	bool riot_shield{};

	bool operator==(const att_add_ons&) const = default;
};

struct laser_settings
{
	bool force_laser_on{};
	bool local_player_ads_laser_enabled{};
	bool local_player_hip_laser_enabled{};
	bool local_player_nvgads_laser_enabled{};
	bool local_player_nvg_hip_laser_enabled{};
	bool remote_player_ads_laser_enabled{};
	bool remote_player_hip_laser_enabled{};
	bool remote_player_nvgads_laser_enabled{};
	bool remote_player_nvg_hip_laser_enabled{};
	bool laser_view_center_in_ads{};

	bool operator==(const laser_settings&) const = default;
};

struct att_laser
{
	std::optional<std::string> laser_type_view_model{};
	std::optional<std::string> laser_type_view_model_alt{};
	std::optional<std::string> laser_type_friendly{};
	std::optional<std::string> laser_type_enemy{};
	laser_settings laser_settings{};

	bool operator==(const att_laser&) const = default;
};

struct att_post
{
	float ads_fire_motion_blur{};
	std::int32_t ads_fire_motion_blur_duration_ms{};
	float ads_fire_motion_blur_decay_exponent{};
	float hip_fire_motion_blur{};
	std::int32_t hip_fire_motion_blur_duration_ms{};
	float hip_fire_motion_blur_decay_exponent{};
	float dof_view_model_physical_fstop{};
	float dof_view_model_physical_focus_distance{};
	float dof_physical_fstop{};
	float dof_physical_focus_distance{};
	float ads_camera_shake_rotation_scale{};
	float ads_camera_shake_translation_scale{};
	bool dof_view_model_physical_focal_tag{};
	bool dof_scene_is_overridden{};

	bool operator==(const att_post&) const = default;
};

struct att_general
{
	bool bolt_action{};
	bool inherits_perks{};
	bool reticle_spin45{};
	bool ads_should_show_crosshair{};
	float enemy_crosshair_range{};
	std::string reticle_type{};
	std::optional<std::string> reticle_center{};
	std::optional<std::string> reticle_side{};
	std::optional<std::string> reticle_one_piece{};
	std::int32_t reticle_center_size{};
	std::int32_t reticle_side_size{};
	std::optional<std::string> sz_lui_crosshair_widget{};

	bool operator==(const att_general&) const = default;
};

struct att_aim_assist
{
	float auto_aim_range{};
	float aim_assist_range{};
	float aim_assist_range_ads{};
	float aim_assist_pitch_slowdown{};
	float aim_assist_pitch_slowdown_ads{};
	float aim_assist_yaw_slowdown{};
	float aim_assist_yaw_slowdown_ads{};
	float aim_assist_lockon_strength{};

	bool operator==(const att_aim_assist&) const = default;
};

struct att_target_assist
{
	std::string target_assist_type{};
	std::string target_assist_behavior{};
	float target_assist_range{};
	float target_assist_angle{};
	float target_assist_los_offset_forward{};
	float target_assist_los_offset_right{};
	float target_assist_los_offset_up{};
	bool target_assist_only_available_in_ads{};

	bool operator==(const att_target_assist&) const = default;
};

struct att_ammunition
{
	std::int32_t max_ammo{};
	std::int32_t start_ammo{};
	std::int32_t clip_size{};
	std::int32_t shot_count{};
	std::int32_t reload_ammo_add{};
	std::int32_t reload_start_add{};
	std::int32_t low_ammo_warning_count{};
	bool require_ammo_used_per_shot{};

	bool operator==(const att_ammunition&) const = default;
};

struct weapon_damage_common
{
	std::int32_t min_damage{};
	std::int32_t mid1_damage{};
	std::int32_t mid2_damage{};
	std::int32_t mid3_damage{};
	std::int32_t damage{};
	float max_damage_range{};
	float mid1_damage_range{};
	float mid2_damage_range{};
	float mid3_damage_range{};
	float min_damage_range{};

	bool operator==(const weapon_damage_common&) const = default;
};

struct weapon_damage_info
{
	std::array<weapon_damage_common, 3> damage_data{};
	std::string death_anim_damage_type{};
	float terminate_at_range{};

	bool operator==(const weapon_damage_info&) const = default;
};

struct att_damage
{
	weapon_damage_info damage_info{};
	std::string death_anim_damage_type{};
	float terminate_at_range{};

	bool operator==(const att_damage&) const = default;
};

struct att_melee
{
	std::int32_t melee_damage{};
	bool has_melee_finisher{};
	std::int32_t melee_count_to_finisher{};

	bool operator==(const att_melee&) const = default;
};

struct advanced_idle_settings
{
	bool use_advanced_idle_settings{};
	bool use_random_points_algorithm{};
	float breath_gasp_scale_override{};
	float idle_sway_setting1_hip_bullet_dir_scale{};
	float idle_sway_setting1_hip_idle_speed{};
	float idle_sway_setting1_hip_weapon_magnitude_x{};
	float idle_sway_setting1_hip_weapon_magnitude_y{};
	float idle_sway_setting1_hip_weapon_magnitude_z{};
	float idle_sway_setting1_hip_weapon_magnitude_f{};
	float idle_sway_setting1_hip_weapon_rotation_offset{};
	float idle_sway_setting1_hip_view_magnitude_x{};
	float idle_sway_setting1_hip_view_magnitude_y{};
	float idle_sway_setting1_ads_bullet_dir_scale{};
	float idle_sway_setting1_ads_idle_speed{};
	float idle_sway_setting1_ads_weapon_magnitude_x{};
	float idle_sway_setting1_ads_weapon_magnitude_y{};
	float idle_sway_setting1_ads_weapon_magnitude_z{};
	float idle_sway_setting1_ads_weapon_magnitude_f{};
	float idle_sway_setting1_ads_weapon_rotation_offset{};
	float idle_sway_setting1_ads_view_magnitude_x{};
	float idle_sway_setting1_ads_view_magnitude_y{};
	float idle_sway_setting2_hip_bullet_dir_scale{};
	float idle_sway_setting2_hip_idle_speed{};
	float idle_sway_setting2_hip_weapon_magnitude_x{};
	float idle_sway_setting2_hip_weapon_magnitude_y{};
	float idle_sway_setting2_hip_weapon_magnitude_z{};
	float idle_sway_setting2_hip_weapon_magnitude_f{};
	float idle_sway_setting2_hip_weapon_rotation_offset{};
	float idle_sway_setting2_hip_view_magnitude_x{};
	float idle_sway_setting2_hip_view_magnitude_y{};
	float idle_sway_setting2_ads_bullet_dir_scale{};
	float idle_sway_setting2_ads_idle_speed{};
	float idle_sway_setting2_ads_weapon_magnitude_x{};
	float idle_sway_setting2_ads_weapon_magnitude_y{};
	float idle_sway_setting2_ads_weapon_magnitude_z{};
	float idle_sway_setting2_ads_weapon_magnitude_f{};
	float idle_sway_setting2_ads_weapon_rotation_offset{};
	float idle_sway_setting2_ads_view_magnitude_x{};
	float idle_sway_setting2_ads_view_magnitude_y{};

	bool operator==(const advanced_idle_settings&) const = default;
};

struct att_idle_settings
{
	float hip_idle_amount{};
	float hip_idle_speed{};
	float idle_crouch_factor{};
	float idle_prone_factor{};
	float ads_idle_lerp_start_time{};
	float ads_idle_lerp_time{};
	bool use_uninterrupted_ads_idle_motion{};
	advanced_idle_settings advanced_idle_settings{};

	bool operator==(const att_idle_settings&) const = default;
};

struct scene_zoom_settings
{
	std::int32_t ads_zoom_count{};
	std::array<float, 3> ads_zoom_fov{};
	std::array<float, 3> ads_reload_fov{};
	std::array<std::int32_t, 3> ads_zoom_fov_transition_time{};
	std::string ads_zoom_fov_lerp_type{};
	float ads_zoom_in_frac{};
	float ads_zoom_out_frac{};
	float ads_fovnvg{};
	float ads_reload_fov_nvg{};
	bool ads_zoom_level_index_reset_on_toggle{};

	bool operator==(const scene_zoom_settings&) const = default;
};

struct weapon_zoom_settings
{
	float ads_zoom_fov{};
	std::string ads_zoom_fov_lerp_type{};
	float ads_zoom_fov_x_offset{};
	float ads_reload_zoom_fov{};
	float ads_reload_zoom_fov_x_offset{};
	float ads_nvg_zoom_fov{};
	float ads_nvg_zoom_fov_x_offset{};
	float ads_nvg_reload_zoom_fov{};
	float ads_nvg_reload_zoom_fov_x_offset{};
	float ads_zoom_in_frac{};
	float ads_zoom_out_frac{};
	bool ads_zoom_fov_x_offset_override{};
	bool ads_reload_fov_x_offset_override{};

	bool operator==(const weapon_zoom_settings&) const = default;
};

struct zoom_settings
{
	scene_zoom_settings scene{};
	weapon_zoom_settings weapon{};

	bool operator==(const zoom_settings&) const = default;
};

struct att_ads_settings
{
	float ads_spread{};
	float ads_spread_start_frac{};
	float ads_spread_end_frac{};
	float ads_aim_pitch{};
	float ads_trans_in_time{};
	float ads_trans_out_time{};
	float ads_trans_in_to_out_time{};
	float ads_trans_out_to_in_time{};
	std::int32_t ads_reload_trans_time{};
	float ads_in_crosshair_alpha_start{};
	float ads_in_crosshair_alpha_end{};
	float ads_out_crosshair_alpha_start{};
	float ads_out_crosshair_alpha_end{};
	zoom_settings zoom_settings{};
	float ads_fov_lerp_time{};
	float ads_bob_factor{};
	float ads_view_bob_mult{};
	float ads_fire_rate_scale{};
	float ads_damage_range_scale{};
	float ads_fire_anim_frac{};
	float fire_timer_lerp_to_ads_scale{};
	bool always_fire_at_max_range_in_ads{};
	bool ads_align_enabled{};
	bool disable_tag_align_x{};
	std::array<float, 3> ads_align_offset{};
	std::string ads_align_offset_lerp_type{};
	std::string ads_align_lerp_type{};

	bool operator==(const att_ads_settings&) const = default;
};

struct att_ads_gun_bob_settings
{
	float f_ads_gun_bob_pitch_scale{};
	float f_ads_gun_bob_yaw_scale{};
	float f_ads_gun_bob_tilt_pitch_scale{};
	float f_ads_gun_bob_tilt_yaw_scale{};
	float f_ads_gun_bob_tilt_roll_scale{};
	float f_ads_gun_bob_tilt_bullet_dir_scale{};
	float f_ads_gun_bob_tilt_offset{};

	bool operator==(const att_ads_gun_bob_settings&) const = default;
};

struct att_ads_reload_settings
{
	bool reload_while_ads{};

	bool operator==(const att_ads_reload_settings&) const = default;
};

struct att_scope_drift_settings
{
	float f_scope_drift_delay{};
	float f_scope_drift_lerp_in_time{};
	float f_scope_drift_steady_time{};
	float f_scope_drift_lerp_out_time{};
	float f_scope_drift_steady_factor{};
	float f_scope_drift_unsteady_factor{};

	bool operator==(const att_scope_drift_settings&) const = default;
};

struct weapon_offset_pattern_scale_info
{
	std::int32_t num_pattern_scales{};
	std::optional<std::vector<weapon_offset_pattern_scale>> pattern_scales{};

	bool operator==(const weapon_offset_pattern_scale_info&) const = default;
};

struct att_ads_stance_scales
{
	float ads_crouch_view_kick_factor_scale{};
	float ads_crouch_gun_kick_factor_scale{};
	float ads_prone_view_kick_factor_scale{};
	float ads_prone_gun_kick_factor_scale{};
	float ads_crouch_gun_bob_factor_scale{};
	weapon_offset_pattern_scale_info crouched_weapon_offset_pattern_scales{};
	weapon_offset_pattern_scale_info prone_weapon_offset_pattern_scales{};

	bool operator==(const att_ads_stance_scales&) const = default;
};

struct att_hip_spread
{
	float hip_spread_stand_min{};
	float hip_spread_ducked_min{};
	float hip_spread_prone_min{};
	float hip_spread_sprint_min{};
	float hip_spread_in_air_min{};
	float hip_spread_max{};
	float hip_spread_ducked_max{};
	float hip_spread_prone_max{};
	float hip_spread_sprint_max{};
	float hip_spread_in_air_max{};
	float hip_spread_fire_add{};
	float hip_spread_turn_add{};
	float hip_spread_move_add{};
	float hip_spread_decay_rate{};
	float hip_spread_ducked_decay{};
	float hip_spread_prone_decay{};
	float hip_spread_sprint_decay{};
	float hip_spread_in_air_decay{};
	float hip_spread_stand_move_max{};
	float hip_spread_ducked_move_max{};
	float hip_spread_prone_move_max{};
	float hip_spread_sprint_move_max{};
	float hip_spread_in_air_move_max{};

	bool operator==(const att_hip_spread&) const = default;
};

struct att_slide_spread
{
	float slide_spread_min{};
	float slide_spread_max{};
	float slide_spread_decay_rate{};
	float slide_spread_fire_add{};
	float slide_spread_turn_add{};
	float slide_spread_move_max{};

	bool operator==(const att_slide_spread&) const = default;
};

struct att_gun_kick
{
	float hip_gun_kick_pitch_min{};
	float hip_gun_kick_pitch_max{};
	float hip_gun_kick_yaw_min{};
	float hip_gun_kick_yaw_max{};
	float hip_gun_kick_mag_min{};
	float ads_gun_kick_pitch_min{};
	float ads_gun_kick_pitch_max{};
	float ads_gun_kick_yaw_min{};
	float ads_gun_kick_yaw_max{};
	float ads_gun_kick_mag_min{};
	bool use_angular_gun_kick{};
	std::array<float, 6> hip_angular_gun_kick_dir{};
	std::array<float, 6> hip_angular_gun_kick_dev{};
	std::array<float, 6> hip_angular_gun_kick_strength_min{};
	std::array<float, 6> hip_angular_gun_kick_strength_max{};
	std::array<float, 6> hip_angular_gun_kick_pitch_scale{};
	std::array<float, 6> ads_angular_gun_kick_dir{};
	std::array<float, 6> ads_angular_gun_kick_dev{};
	std::array<float, 6> ads_angular_gun_kick_strength_min{};
	std::array<float, 6> ads_angular_gun_kick_strength_max{};
	std::array<float, 6> ads_angular_gun_kick_pitch_scale{};
	std::array<std::int32_t, 6> ads_angular_gun_kick_bullet{};
	std::array<bool, 6> ads_angular_gun_kick_use_set{};
	bool use_new_gun_kick{};
	float hip_gun_kick_accel{};
	float ads_gun_kick_accel{};
	float hip_gun_kick_speed_max{};
	float ads_gun_kick_speed_max{};
	float hip_gun_kick_speed_decay{};
	float ads_gun_kick_speed_decay{};
	float hip_gun_kick_static_decay{};
	float ads_gun_kick_static_decay{};
	float hip_gun_kick_return_accel_scale{};
	float ads_gun_kick_return_accel_scale{};
	float hip_gun_kick_return_speed_curve_scale{};
	float ads_gun_kick_return_speed_curve_scale{};
	float gun_max_pitch{};
	float gun_max_yaw{};
	float bullet_dir_gun_angle_modifier_gun_kick_hip{};
	float bullet_dir_gun_angle_modifier_gun_kick_ads{};

	bool operator==(const att_gun_kick&) const = default;
};

struct att_gun_tilt
{
	float ads_gun_tilt_pitch_factor{};
	float ads_gun_tilt_yaw_factor{};
	float ads_gun_tilt_roll_factor{};
	float ads_gun_tilt_offset{};
	float hip_gun_tilt_pitch_factor{};
	float hip_gun_tilt_yaw_factor{};
	float hip_gun_tilt_roll_factor{};
	float hip_gun_tilt_offset{};

	bool operator==(const att_gun_tilt&) const = default;
};

struct att_kick_scaling
{
	float hip_starting_gun_kick_percent{};
	float hip_starting_view_kick_percent{};
	std::int32_t ads_starting_kick_bullets{};
	float ads_starting_gun_kick_percent{};
	float ads_starting_view_kick_percent{};
	float hip_ending_gun_kick_percent{};
	float hip_ending_view_kick_percent{};
	std::int32_t ads_ending_kick_bullets{};
	float ads_ending_gun_kick_percent{};
	float ads_ending_view_kick_percent{};

	bool operator==(const att_kick_scaling&) const = default;
};

struct att_view_kick
{
	float hip_view_kick_pitch_min{};
	float hip_view_kick_pitch_max{};
	float hip_view_kick_yaw_min{};
	float hip_view_kick_yaw_max{};
	float hip_view_kick_mag_min{};
	float ads_view_kick_pitch_min{};
	float ads_view_kick_pitch_max{};
	float ads_view_kick_yaw_min{};
	float ads_view_kick_yaw_max{};
	float ads_view_kick_mag_min{};
	bool use_angular_view_kick{};
	std::array<float, 6> hip_angular_view_kick_dir{};
	std::array<float, 6> hip_angular_view_kick_dev{};
	std::array<float, 6> hip_angular_view_kick_strength_min{};
	std::array<float, 6> hip_angular_view_kick_strength_max{};
	std::array<float, 6> hip_angular_view_kick_pitch_scale{};
	std::array<float, 6> ads_angular_view_kick_dir{};
	std::array<float, 6> ads_angular_view_kick_dev{};
	std::array<float, 6> ads_angular_view_kick_strength_min{};
	std::array<float, 6> ads_angular_view_kick_strength_max{};
	std::array<float, 6> ads_angular_view_kick_pitch_scale{};
	std::array<std::int32_t, 6> ads_angular_view_kick_bullet{};
	std::array<bool, 6> ads_angular_view_kick_use_set{};
	bool use_new_view_kick{};
	float hip_view_kick_center_speed{};
	float ads_view_kick_center_speed{};
	float hip_view_kick_return_accel_scale{};
	float ads_view_kick_return_accel_scale{};
	float hip_view_kick_return_speed_curve_scale{};
	float ads_view_kick_return_speed_curve_scale{};
	float kick_aligned_input_scalar{};
	float kick_opposed_input_scalar{};
	float view_kick_maintain_fraction{};
	float view_max_pitch{};
	float view_max_yaw{};

	bool operator==(const att_view_kick&) const = default;
};

struct ads_overlay
{
	std::optional<std::string> shader_mat{};
	std::optional<std::string> shader_low_res_mat{};
	std::optional<std::string> shader_emp_mat{};
	std::optional<std::string> shader_emp_low_res_mat{};
	std::optional<std::string> shader_img{};
	std::optional<std::string> shader_low_res_img{};
	std::optional<std::string> shader_emp_img{};
	std::optional<std::string> shader_emp_low_res_img{};
	bool apply_vision_set{};
	std::string reticle{};
	float width{};
	float height{};
	float width_splitscreen{};
	float height_splitscreen{};
	std::optional<std::string> vision_set_name{};
	float vision_set_ads_fraction{};
	std::int32_t vision_set_blend_in_time_ms{};
	std::int32_t vision_set_blend_out_time_ms{};

	bool operator==(const ads_overlay&) const = default;
};

struct att_ads_overlay
{
	ads_overlay overlay{};
	std::optional<std::string> hybrid_scope_attachment{};
	float hybrid_scope1_to2_fov_trans_begin{};
	float hybrid_scope1_to2_fov_trans_end{};
	float hybrid_scope2_to1_fov_trans_begin{};
	float hybrid_scope2_to1_fov_trans_end{};
	float hybrid_scope1_to2_scope_fade_trans_begin{};
	float hybrid_scope1_to2_scope_fade_trans_end{};
	float hybrid_scope2_to1_scope_fade_trans_begin{};
	float hybrid_scope2_to1_scope_fade_trans_end{};
	bool hybrid_scope1_show_hip_glass_when_inactive{};
	bool hybrid_scope2_show_hip_glass_when_inactive{};
	bool hybrid_toggle{};
	bool thermal_scope{};
	bool thermal_toggle{};

	bool operator==(const att_ads_overlay&) const = default;
};

struct att_outline
{
	std::array<float, 4> outline_color{};
	std::array<float, 4> outline_alt_color{};
	std::array<float, 4> outline_fill_color0{};
	std::array<float, 4> outline_fill_color1{};
	std::array<float, 4> outline_occluded_outline_color{};
	std::array<float, 4> outline_occluded_inline_color{};
	std::array<float, 4> outline_occluded_interior_color{};
	float outline_width{};
	bool outline_enemies{};
	bool outline_depth_test{};
	bool outline_fill{};
	bool outline_script_controlled{};
	bool outline_vehicles{};
	bool outline_turrets{};
	bool outline_characters{};
	bool outline_agents{};
	bool outline_script_movers{};
	bool outline_lock_on{};

	bool operator==(const att_outline&) const = default;
};

struct att_ui
{
	std::optional<std::string> hud_icon{};
	std::optional<std::string> pickup_icon{};
	std::optional<std::string> danger_icon_img{};
	std::optional<std::string> throw_back_icon_img{};
	std::optional<std::string> danger_icon_mat{};
	std::optional<std::string> throw_back_icon_mat{};
	bool hide_warning_icons{};
	float warning_icons_delay{};
	std::optional<std::string> dpad_icon_img{};
	std::optional<std::string> dpad_icon_inactive_img{};
	std::optional<std::string> dpad_icon_mat{};
	std::optional<std::string> dpad_icon_inactive_mat{};
	std::string ammo_counter_clip{};
	float grenade_danger_icon_distance{};

	bool operator==(const att_ui&) const = default;
};

struct att_rumbles
{
	std::optional<std::string> fire_rumble{};
	std::optional<std::string> melee_impact_rumble{};

	bool operator==(const att_rumbles&) const = default;
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

struct att_projectile
{
	std::int32_t explosion_radius{};
	std::int32_t explosion_force_radius{};
	std::int32_t explosion_inner_damage{};
	std::int32_t explosion_outer_damage{};
	float damage_cone_angle{};
	std::int32_t projectile_speed{};
	std::int32_t projectile_speed_up{};
	std::int32_t projectile_activate_dist{};
	float projectile_lifetime{};
	float projectile_lifetime_std_deviation{};
	std::optional<std::string> projectile_model{};
	std::string proj_explosion_type{};
	fx_combined_def proj_explosion_effect{};
	bool proj_explosion_effect_force_normal_up{};
	snd_alias_lookup proj_explosion_sound{};
	std::uint32_t proj_explosion_refl_class{};
	fx_combined_def proj_dud_effect{};
	fx_combined_def proj_timeout_effect{};
	snd_alias_lookup proj_dud_sound{};
	snd_alias_lookup proj_timeout_sound{};
	bool disable_fx_on_timeout{};
	bool proj_impact_explode{};
	bool disable_projectile_crumple_check{};
	float destabilization_rate_time{};
	float destabilization_curvature_max{};
	std::int32_t destabilize_distance{};
	fx_combined_def vm_proj_body_effect{};
	fx_combined_def proj_body_effect{};
	fx_combined_def proj_trail_effect{};
	fx_combined_def proj_beacon_effect{};
	std::int32_t proj_ignition_delay{};
	fx_combined_def proj_ignition_effect{};
	snd_alias_lookup proj_ignition_sound{};
	std::string stickiness{};
	std::int32_t fuse_time{};
	std::int32_t ai_fuse_time{};
	bool stick_to_players{};
	bool stick_to_vehicles{};
	bool stick_to_turrets{};
	bool stick_to_non_stick{};
	bool timed_detonation{};
	bool is_predicted_projectile{};
	bool use_stepped_explosion_damage{};
	std::int32_t client_trajectory_blend_out_time{};
	std::int32_t server_trajectory_blend_in_time{};
	float reactive_motion_radius_scale{};
	float reactive_motion_frequency_scale{};
	float reactive_motion_amplitude_scale{};
	float reactive_motion_falloff{};
	float reactive_motion_lifetime{};

	bool operator==(const att_projectile&) const = default;
};

struct att_charged
{
	std::uint32_t charge_type{};
	float charge_gain{};
	float charge_cost_per_shot{};
	float charge_loss_when_idle{};
	float charge_empty_cooldown{};
	float charge_fire_at_max_damage_multiplier{};
	std::int32_t charge_meter_effect_bone_count{};
	fx_combined_def charge_meter_effect{};
	snd_alias_lookup charge_up_sound{};
	snd_alias_lookup charge_down_sound{};
	snd_alias_lookup charge_down_to_up_sound{};
	snd_alias_lookup charge_up_to_down_sound{};
	snd_alias_lookup charge_max_sound{};
	snd_alias_lookup charge_up_sound_player{};
	snd_alias_lookup charge_down_sound_player{};
	snd_alias_lookup charge_down_to_up_sound_player{};
	snd_alias_lookup charge_up_to_down_sound_player{};
	snd_alias_lookup charge_max_sound_player{};
	bool charge_hud_reveal{};
	std::optional<std::string> charge_rumble{};

	bool operator==(const att_charged&) const = default;
};

struct ads_alt_switch
{
	float fov_interp_time_sec_main_to_alt{};
	float fov_interp_delay_sec_main_to_alt{};
	float dual_fov_switch_rate_main_to_alt{};
	std::string fov_interp_type_main_to_alt{};
	float fov_interp_time_sec_alt_to_main{};
	float fov_interp_delay_sec_alt_to_main{};
	float dual_fov_switch_rate_alt_to_main{};
	std::string fov_interp_type_alt_to_main{};

	bool operator==(const ads_alt_switch&) const = default;
};

struct att_regeneration
{
	bool regeneration_enabled{};
	std::int32_t regeneration_time_ms{};
	std::int32_t regeneration_add_time_ms{};
	std::int32_t regeneration_amount{};
	bool regeneration_consume_stock{};
	bool regeneration_disable_while_firing{};

	bool operator==(const att_regeneration&) const = default;
};

struct att_heat
{
	float heat_accumulation_per_shot{};
	float heat_dissipation_per_second{};
	float heat_smoke_start_threshold{};
	float heat_smoke_stop_threshold{};

	bool operator==(const att_heat&) const = default;
};

struct att_movement
{
	float move_speed_scale{};
	float ads_move_speed_scale{};
	bool fast_ads3_p_anims{};

	bool operator==(const att_movement&) const = default;
};

struct att_burst
{
	std::int32_t burst_fire_cooldown{};
	bool burst_fire_auto{};

	bool operator==(const att_burst&) const = default;
};

struct att_hitmarker
{
	std::string hitmarker_type{};

	bool operator==(const att_hitmarker&) const = default;
};

struct att_ads_movement
{
	float forward_offset{};
	float roll{};
	bool is_override{};

	bool operator==(const att_ads_movement&) const = default;
};

struct ballistic_info_calculated
{
	std::optional<std::vector<float>> distances{};
	float zeroing_angle{};

	bool operator==(const ballistic_info_calculated&) const = default;
};

struct ballistic_info
{
	float muzzle_velocity{};
	float ballistic_coefficient{};
	float diameter{};
	float mass{};
	float gravity_factor{};
	float zeroing_distance{};
	std::optional<ballistic_info_calculated> calculated{};
	bool enable_ballistic_trajectory{};
	std::int32_t life_time_ms{};

	bool operator==(const ballistic_info&) const = default;
};

struct common_sway_settings
{
	float max_angle{};
	float lerp_speed{};
	float pitch_scale{};
	float yaw_scale{};
	float horiz_scale{};
	float vert_scale{};
	float gun_angle_scale{};

	bool operator==(const common_sway_settings&) const = default;
};

struct hip_sway_settings
{
	common_sway_settings common{};
	float max_angle_steady_aim{};

	bool operator==(const hip_sway_settings&) const = default;
};

struct ads_sway_settings
{
	common_sway_settings common{};
	float sway_transition_lerp_speed{};
	std::array<float, 3> ads_sway_scale{};

	bool operator==(const ads_sway_settings&) const = default;
};

struct advanced_hip_sway_settings
{
	bool enabled{};
	float torso_goal_smooth_speed{};
	std::int32_t torso_goal_view_smooth_duration_ms{};
	std::array<float, 2> torso_goal_deadzone_adjust_speed{};
	std::optional<std::string> torso_goal_view_speed_to_max_deadzone_graph{};
	std::array<float, 2> torso_goal_view_speed_to_max_deadzone_viewspeed{};
	std::array<float, 2> torso_goal_view_speed_to_max_deadzone_max_deadzone{};
	std::array<float, 2> torso_mass{};
	std::array<float, 2> torso_spring{};
	std::array<float, 2> torso_damper{};
	std::int32_t gun_goal_view_smooth_duration_ms{};
	std::optional<std::string> gun_goal_view_speed_to_offset_graph{};
	std::array<float, 2> gun_goal_view_speed_to_offset_viewspeed{};
	std::array<float, 2> gun_goal_view_speed_to_offset_offset{};
	std::array<float, 2> gun_mass{};
	std::array<float, 2> gun_spring{};
	std::array<float, 2> gun_damper{};
	std::array<float, 3> gun_pivot_point{};
	float gun_yaw_to_roll_scale{};
	std::int32_t fire_duration_ms{};
	std::int32_t fire_start_blend_duration_ms{};
	std::int32_t fire_finish_blend_duration_ms{};
	float fire_torso_goal_smooth_speed{};
	float fire_torso_deadzone_scale{};
	float fire_torso_to_gun_dir_scale{};

	bool operator==(const advanced_hip_sway_settings&) const = default;
};

struct sway_settings
{
	hip_sway_settings hip{};
	ads_sway_settings ads{};
	advanced_hip_sway_settings adv{};
	float shell_shock_scale{};
	bool override_hip{};
	bool override_ads{};

	bool operator==(const sway_settings&) const = default;
};

struct att_dlc_overrides
{
	std::array<bool, 8> use_float_override{};
	std::array<float, 8> float_override{};

	bool operator==(const att_dlc_overrides&) const = default;
};

struct att_dlc_scalars
{
	std::array<float, 8> float_scalar{};

	bool operator==(const att_dlc_scalars&) const = default;
};

struct attachment
{
	std::optional<std::string> sz_internal_name{};
	std::optional<std::string> sz_display_name{};
	std::optional<std::string> internal_name{};
	std::optional<std::string> override_category_name{};
	bool override_display_name{};
	bool disable_weapon_inspect{};
	std::string type{};
	std::string alt_mode_player_anim_type{};
	std::string weapon_type{};
	std::string weap_class{};
	std::optional<std::string> fallback_world_model{};
	std::optional<std::string> fallback_view_model{};
	std::optional<std::vector<std::optional<std::string>>> world_model_variations{};
	std::optional<std::vector<std::optional<std::string>>> view_model_variations{};
	std::optional<std::string> attach_point{};
	std::optional<std::vector<std::optional<std::string>>> hide_tags{};
	std::optional<std::vector<std::optional<std::string>>> hide_tags_non_view_model{};
	std::optional<std::vector<std::optional<std::string>>> show_tags{};
	std::optional<std::vector<std::optional<std::string>>> reticles{};
	std::optional<std::vector<std::optional<std::string>>> alt_reticles{};
	std::optional<att_ammo_general> ammogeneral{};
	std::optional<att_ammo_type> ammotype{};
	std::optional<att_impact> impact{};
	std::string dynamic_attachment_type{};
	std::optional<std::string> dynamic_attachment_animate_seq{};
	std::optional<std::string> dynamic_attachment_icon{};
	std::string disable_input_driven_view_return_dampening{};
	std::optional<att_ammo_tracer> tracer{};
	std::optional<att_penetration> penetration{};
	std::optional<att_sight> sight{};
	std::optional<att_hold_breath> hold_breath{};
	std::optional<att_reload> reload{};
	std::optional<att_add_ons> add_ons{};
	std::optional<att_laser> laser{};
	std::optional<att_post> post{};
	std::optional<att_general> general{};
	std::optional<att_aim_assist> aim_assist{};
	std::optional<att_target_assist> target_assist{};
	std::optional<std::array<att_ammunition, 28>> ammunition{};
	std::optional<att_damage> damage{};
	std::optional<att_melee> melee{};
	std::optional<att_idle_settings> idle_settings{};
	std::optional<att_ads_settings> ads_settings{};
	std::optional<att_ads_settings> ads_settings_main{};
	std::optional<att_ads_gun_bob_settings> ads_gun_bob_settings{};
	std::optional<att_ads_gun_bob_settings> ads_gun_bob_settings_main{};
	std::optional<att_ads_reload_settings> ads_reload_settings{};
	std::optional<att_scope_drift_settings> scope_drift_settings{};
	std::optional<att_scope_drift_settings> scope_drift_settings_main{};
	std::optional<att_ads_stance_scales> ads_stance_scales{};
	std::optional<att_hip_spread> hip_spread{};
	std::optional<att_slide_spread> slide_spread{};
	std::optional<att_gun_kick> gun_kick{};
	std::optional<att_gun_tilt> gun_tilt{};
	std::optional<att_kick_scaling> kick_scaling{};
	std::optional<att_view_kick> view_kick{};
	std::optional<att_ads_overlay> ads_overlay{};
	std::optional<att_outline> outline{};
	std::optional<att_ui> ui{};
	std::optional<att_rumbles> rumbles{};
	std::optional<att_projectile> projectile{};
	std::optional<att_charged> charged{};
	std::optional<ads_alt_switch> ads_alt_switch{};
	std::optional<att_regeneration> regeneration{};
	std::optional<att_heat> heat{};
	std::optional<att_movement> movement{};
	std::optional<att_movement> movement_scale{};
	std::optional<att_movement> movement_scale_main{};
	std::optional<att_burst> burst{};
	std::optional<att_hitmarker> hitmarker{};
	std::optional<att_ads_movement> ads_movement{};
	std::optional<ballistic_info> ballistic_info{};
	std::optional<sway_settings> sway_settings{};
	std::optional<weapon_offset_pattern_scale_info> weapon_offset_pattern_scales{};
	std::optional<std::array<float, 22>> location_damage{};
	float ammunition_scale{};
	float ballistic_muzzle_velocity_scale{};
	float damage_scale{};
	bool override_stand_ofs{};
	std::array<float, 3> stand_ofs{};
	std::array<float, 3> stand_ofs_rot{};
	std::array<float, 3> stand_ofs_rot_pivot{};
	std::array<float, 3> stand_ofs_add{};
	std::array<float, 3> stand_ofs_rot_add{};
	bool overridehip_offset_lerp_type{};
	std::string hip_offset_lerp_type{};
	float state_timers_scale{};
	float reload_timers_scale{};
	std::int32_t fire_time_override{};
	std::int32_t fire_time_akimbo_override{};
	float fire_timers_scale{};
	float burst_fire_cooldown_scale{};
	float idle_hip_motion_scale{};
	float idle_ads_motion_scale{};
	float idle_ads_motion_time_scale{};
	float ads_settings_scale{};
	float ads_settings_scale_main{};
	float ads_spread_scale{};
	float ads_spread_scale_main{};
	float hip_spread_scale{};
	float slide_spread_scale{};
	float gun_kick_pitch_scale{};
	float gun_kick_yaw_scale{};
	float gun_center_scale{};
	float gun_tilt_scale{};
	float view_kick_pitch_scale{};
	float view_kick_yaw_scale{};
	float view_center_scale{};
	float view_kick_maintain_fraction_scale{};
	float hip_sway_scale{};
	float ads_sway_scale{};
	float ads_sway_scale2{};
	float ads_sway_scale3{};
	float ads_sway_max_angle_scale{};
	float ads_sway_lerp_speed_scale{};
	float ads_sway_scale_scale{};
	float sprint_out_timer_scale{};
	std::int32_t i_ammo_index{};
	float heat_accumulation_scale{};
	float heat_dissipation_scale{};
	float heat_smoke_threshold_scale{};
	float ads_fire_motion_blur_scale{};
	float ads_fire_motion_blur_duration_scale{};
	float hip_fire_motion_blur_scale{};
	float hip_fire_motion_blur_duration_scale{};
	float ads_camera_shake_rotation_scale_scale{};
	float ads_camera_shake_translation_scale_scale{};
	float ads_gun_bob_scale_scale{};
	float ads_gun_bob_scale_scale_main{};
	float ads_gun_bob_tilt_scale_scale{};
	float ads_gun_bob_tilt_scale_scale_main{};
	float ads_gun_bob_tilt_bullet_dir_scale_scale{};
	float ads_gun_bob_tilt_bullet_dir_scale_scale_main{};
	float ads_view_bob_scale_scale{};
	float ads_bob_rate_scale{};
	float melee_damage_scale{};
	std::int32_t melee_count_to_finish_adjust{};
	std::int32_t load_index{};
	std::int32_t ammo_used_per_shot_override{};
	std::uint16_t transient_view_flags{};
	std::uint16_t transient_world_flags{};
	bool use_alternate_view_model_laser{};
	bool ignore_iron_sights{};
	bool hide_iron_sights_with_this_attachment{};
	bool share_ammo_with_alt{};
	bool use_left_trigger_alt_fire{};
	bool force_foregrip_ik{};
	bool contains_ik_tag{};
	bool is_akimbo_attachment{};
	bool attach_left_weapon_akimbo{};
	bool attach_right_weapon_akimbo{};
	bool lui_weapon_info_widget_uses_scope_stencil{};
	std::optional<std::string> sz_alt_mode_name{};
	std::optional<std::string> sz_alt_mode_inactive_name{};
	std::optional<std::string> sz_lui_weapon_info_widget_name{};
	std::optional<std::string> sz_lui_weapon_info_widget_tag{};
	std::optional<std::string> alt_mode_icon{};
	std::optional<att_dlc_overrides> dlc_overrides{};
	std::optional<att_dlc_scalars> dlc_scalars{};

	bool operator==(const attachment&) const = default;
};

struct weapon_offset_pattern_scale
{
	std::optional<std::string> pattern_key{};
	std::array<float, 3> scale{};

	bool operator==(const weapon_offset_pattern_scale&) const = default;
};

struct animation_state_machine
{
	std::optional<std::string> name{};
	std::optional<std::vector<animation_state>> states{};
	std::optional<std::vector<animation_aim_set>> aim_sets{};

	bool operator==(const animation_state_machine&) const = default;
};

struct animclass
{
	std::optional<std::string> class_name{};
	std::optional<animation_state_machine> state_machine{};
	std::string anim_ctrl{};
	std::optional<std::string> anim_tree{};
	std::optional<std::vector<std::optional<std::string>>> sound_notes{};
	std::optional<std::vector<std::optional<std::string>>> sound_names{};
	std::optional<std::vector<std::optional<std::string>>> sound_options{};
	std::optional<std::vector<std::optional<std::string>>> effect_notes{};
	std::optional<std::vector<fx_combined_def>> effect_defs{};
	std::optional<std::vector<std::optional<std::string>>> effect_tags{};

	bool operator==(const animclass&) const = default;
};

struct animation_state
{
	std::optional<std::string> name{};
	std::optional<std::string> notify{};
	float blend_time{};
	float blend_out_time{};
	std::uint8_t flags{};
	std::int32_t aim_set_index{};
	std::optional<std::vector<animation_entry>> anim_entries{};
	std::optional<std::vector<std::uint64_t>> anim_indices{};
	std::optional<std::vector<anim_alias>> alias_list{};

	bool operator==(const animation_state&) const = default;
};

struct animation_entry
{
	std::optional<std::string> anim_name{};
	std::int32_t aim_set_index{};

	bool operator==(const animation_entry&) const = default;
};

struct anim_alias
{
	std::optional<std::string> alias_name{};
	std::optional<std::vector<anim_alias_info>> alias_info{};

	bool operator==(const anim_alias&) const = default;
};

struct anim_alias_info
{
	std::uint8_t anim_index{};
	float anim_weight{};

	bool operator==(const anim_alias_info&) const = default;
};

struct animation_aim_set
{
	std::optional<std::string> name{};
	std::optional<std::string> root_name{};
	std::optional<std::vector<std::optional<std::string>>> anim_name{};
	std::uint64_t root_index{};
	std::optional<std::vector<std::uint64_t>> anim_indices{};
	std::optional<std::vector<std::uint64_t>> aim_node_indices{};

	bool operator==(const animation_aim_set&) const = default;
};

struct luafile
{
	std::optional<std::string> name{};
	std::uint8_t stripping_type{};
	std::optional<std::vector<std::uint8_t>> buffer{};

	bool operator==(const luafile&) const = default;
};

struct leaderboarddef
{
	std::optional<std::string> name{};
	std::int32_t id{};
	std::int32_t source_lb_id{};
	std::int32_t source_lb_width{};
	std::int32_t xp_col_id{};
	std::int32_t prestige_col_id{};
	std::optional<std::vector<lb_column_def>> columns{};
	std::string update_type{};
	std::int32_t track_types{};
	std::int32_t rank_col_id_x{};
	std::int32_t rank_col_id_y{};

	bool operator==(const leaderboarddef&) const = default;
};

struct lb_column_def
{
	std::optional<std::string> name{};
	std::int32_t id{};
	std::int32_t property_id{};
	bool hidden{};
	std::int32_t stats_group{};
	std::optional<std::string> stat_name{};
	std::string type{};
	std::int32_t precision{};
	std::string agg{};
	std::int32_t ui_cal_col_x{};
	std::int32_t ui_cal_col_y{};

	bool operator==(const lb_column_def&) const = default;
};

struct virtualleaderboarddef
{
	std::optional<std::string> name{};
	std::optional<std::string> source_name{};
	std::int32_t id{};
	std::int32_t source_id{};
	std::optional<std::vector<lb_vr_column_def>> columns{};
	std::int32_t rank_col_id_x{};
	std::int32_t rank_col_id_y{};

	bool operator==(const virtualleaderboarddef&) const = default;
};

struct lb_vr_column_def
{
	std::optional<std::string> name{};
	std::int32_t id{};
	std::int32_t ui_cal_col_x{};
	std::int32_t ui_cal_col_y{};

	bool operator==(const lb_vr_column_def&) const = default;
};

struct clothasset
{
	std::optional<std::string> name{};
	bool stand_alone{};
	std::optional<std::vector<char>> havok_data{};
	std::optional<std::vector<cloth_asset_bone_mapping_set>> bone_mapping_sets{};
	std::optional<std::string> animation_state_name{};
	std::uint32_t animation_state_idx{};
	std::optional<std::vector<cloth_asset_simulation_lod>> simulation_lods{};
	std::int32_t surface_type_index{};
	bool vector_field_support{};
	float vector_field_drag{};
	bool bullet_support{};
	std::int32_t bullet_proxy_update_rate{};
	float bullet_force{};
	float default_speed{};
	float speed_blend_speed{};
	float sound_iw_max_distance_sq{};
	float sound_hk_max_distance_sq{};
	std::optional<std::vector<cloth_asset_sound>> sounds{};

	bool operator==(const clothasset&) const = default;
};

struct cloth_asset_bone_mapping_set
{
	std::optional<std::vector<cloth_asset_bone_mapping>> bone_mappings{};

	bool operator==(const cloth_asset_bone_mapping_set&) const = default;
};

struct cloth_asset_bone_mapping
{
	std::optional<std::vector<std::optional<std::string>>> scr_tag_names{};

	bool operator==(const cloth_asset_bone_mapping&) const = default;
};

struct cloth_asset_simulation_lod
{
	float max_distance_sq{};
	float hk_max_distance_sq{};
	std::optional<std::string> state_name{};
	std::uint32_t state_idx{};

	bool operator==(const cloth_asset_simulation_lod&) const = default;
};

struct cloth_asset_sound
{
	float min_speed{};
	float max_speed{};
	float pitch_min_speed{};
	float pitch_max_speed{};
	float volume_min_speed{};
	float volume_max_speed{};
	std::optional<std::string> sound_alias{};

	bool operator==(const cloth_asset_sound&) const = default;
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

struct st_clutter_sample_points
{
	std::uint32_t sample_count{};
	std::optional<std::vector<std::array<float, 2>>> sample_points{};
	gfx_wrapped_buffer sample_point_buffer{};

	bool operator==(const st_clutter_sample_points&) const = default;
};

struct stterrain
{
	std::optional<std::string> name{};
	std::optional<std::vector<st_disk_terrain_surface>> surfaces{};
	std::optional<std::vector<st_terrain_maps>> maps{};
	std::optional<std::vector<std::optional<std::string>>> flattened_images{};
	st_clutter_sample_points clutter_sample_points{};
	std::string lightmap_type{};
	std::optional<std::vector<st_lightmap>> lightmaps{};

	bool operator==(const stterrain&) const = default;
};

struct gfx_index_buffer
{
	std::uint64_t gpu_address{};
	std::uint64_t allocation{};
	std::uint32_t size{};

	bool operator==(const gfx_index_buffer&) const = default;
};

struct st_terrain_grid
{
	std::uint32_t num_levels{};
	std::optional<std::vector<st_terrain_grid_level>> levels{};

	bool operator==(const st_terrain_grid&) const = default;
};

struct gfx_placement
{
	std::array<float, 4> quat{};
	std::array<float, 3> origin{};

	bool operator==(const gfx_placement&) const = default;
};

struct bounds
{
	std::array<float, 3> mid_point{};
	std::array<float, 3> half_size{};

	bool operator==(const bounds&) const = default;
};

struct st_clutter_sample_bitmask
{
	std::optional<std::string> stream_key{};
	std::optional<std::vector<std::uint32_t>> bitmask{};
	gfx_wrapped_buffer bitmask_buffer{};
	gfx_wrapped_buffer relocated_bitmask_buffer{};

	bool operator==(const st_clutter_sample_bitmask&) const = default;
};

struct st_disk_terrain_surface
{
	std::uint32_t high_lod_index_count{};
	std::optional<std::vector<std::uint16_t>> high_lod_indices{};
	gfx_index_buffer high_lod_index_buffer{};
	st_terrain_grid grid{};
	gfx_placement obj_to_wld{};
	bounds wld_bounds{};
	std::uint32_t layer_count{};
	std::uint32_t surface_map_set_index{};
	std::uint32_t surface_map_x_count{};
	std::uint32_t surface_map_y_count{};
	std::uint32_t tile_vert_x_count{};
	std::array<float, 2> surface_map_origin{};
	float surface_map_world_size{};
	float height_offset{};
	float height_scale{};
	float half_wld_size{};
	std::optional<std::vector<st_terrain_material_layer>> layer_materials{};
	std::uint32_t layer_displacement_mask{};
	std::optional<std::vector<st_terrain_displacement_scale>> displacement_scales{};
	std::optional<std::vector<st_terrain_clutter_layer>> clutter_layers{};
	gfx_wrapped_buffer clutter_layers_buffer{};
	st_clutter_sample_bitmask clutter_sample_bitmask{};
	std::optional<std::vector<std::uint32_t>> clutter_tile_instance_counts{};
	std::optional<std::vector<std::uint16_t>> layer_mask_map_idxs{};
	std::optional<std::string> height_map{};
	std::optional<std::string> cutout_map{};
	std::optional<std::string> combined_albedo_map{};
	std::optional<std::string> combined_nog_map{};
	float combined_map_stream_radius_inv_sq{};
	std::uint32_t lightmap_index{};

	bool operator==(const st_disk_terrain_surface&) const = default;
};

struct st_terrain_grid_level
{
	std::uint32_t width{};
	std::uint32_t height{};

	bool operator==(const st_terrain_grid_level&) const = default;
};

struct hlsl_terrain_layer_data_code
{
	std::array<std::array<float, 4>, 2> texture_transform{};
	std::uint32_t flags{};
	std::uint32_t alpha_layer{};
	std::array<float, 2> texture_scale{};
	std::array<float, 2> detail_scale{};
	std::array<float, 3> color_tint{};
	float alpha_reveal_soft_edge{};
	float detail_scale_height{};
	std::uint32_t normal_map_packed_scale_bias{};
	std::uint32_t albedo_map_packed_scale_bias{};
	std::uint32_t color_sampler{};
	std::uint32_t color_map{};
	std::uint32_t tint_mask{};
	std::uint32_t normal_sampler{};
	std::uint32_t normal_map{};
	std::uint32_t reveal_sampler{};
	std::uint32_t reveal_map{};
	std::uint32_t emissive_map{};
	std::uint32_t transmittance_map{};
	std::uint32_t thickness_map{};
	std::uint32_t displacement_map{};
	float displacement_scale{};
	float grass_height{};
	float grass_height_variance{};
	float grass_width{};
	float grass_width_variance{};
	float grass_tilt{};
	float grass_tilt_variance{};
	float grass_bend{};

	bool operator==(const hlsl_terrain_layer_data_code&) const = default;
};

struct st_terrain_material_layer
{
	std::optional<std::string> material{};
	std::optional<std::array<std::uint16_t, 4>> material_texture_idxs{};
	std::array<std::array<float, 4>, 2> st_from_xyz{};
	float himip_radius_inv_sq{};
	std::int32_t layer{};
	std::int32_t decal_sort{};
	bool dynamic_uvs{};
	hlsl_terrain_layer_data_code layer_data{};

	bool operator==(const st_terrain_material_layer&) const = default;
};

struct st_terrain_displacement_scale
{
	std::uint32_t mask{};
	float scale{};

	bool operator==(const st_terrain_displacement_scale&) const = default;
};

struct st_terrain_clutter_layer
{
	std::uint32_t sample_offset{};
	std::uint32_t bitmask_offset{};
	std::array<std::uint32_t, 2> tile_count{};
	std::array<float, 2> tile_size_uv{};
	std::array<float, 2> layer_size{};
	std::array<float, 3> angle_min{};
	std::array<float, 3> angle_max{};
	std::array<float, 2> z_offset_min_max{};
	std::array<float, 2> scale_min_max{};
	std::uint32_t max_instance_count{};
	std::uint32_t tile_instance_count_offset{};
	std::uint32_t use_surface_normal{};

	bool operator==(const st_terrain_clutter_layer&) const = default;
};

struct st_terrain_maps
{
	std::int32_t map_resolution{};
	std::int32_t padded_resolution{};

	bool operator==(const st_terrain_maps&) const = default;
};

struct st_lightmap
{

	bool operator==(const st_lightmap&) const = default;
};

struct static_model_collision_compressed_model_list
{
	std::uint32_t num_models{};
	std::optional<std::vector<static_model_collision_compressed_model>> models{};
	std::uint32_t structure_memory_cost{};
	std::uint32_t num_instances{};
	std::uint32_t asset_cost{};

	bool operator==(const static_model_collision_compressed_model_list&) const = default;
};

struct collisiontile
{
	std::optional<std::string> name{};
	std::uint32_t index{};
	std::array<std::uint8_t, 2> grid_id{};
	std::array<float, 3> origin{};
	std::optional<static_model_collision_compressed_model_list> static_model_collision{};
	std::optional<std::vector<char>> havok_tile_shape_data{};
	std::optional<char> havok_tile_shape_pointer{};
	std::uint32_t world_collision_shape_idx{};

	bool operator==(const collisiontile&) const = default;
};

struct static_model_collision_compressed_model
{
	std::optional<std::string> name{};
	std::optional<std::string> physics_asset{};
	std::optional<std::string> detail_collision{};
	std::array<std::uint16_t, 2> shape_tags{};
	std::optional<std::vector<static_model_collision_compressed_instance>> instances{};

	bool operator==(const static_model_collision_compressed_model&) const = default;
};

struct static_model_collision_compressed_instance
{
	std::array<std::uint16_t, 3> origin{};
	std::array<std::uint16_t, 3> angles{};
	std::uint16_t scale{};

	bool operator==(const static_model_collision_compressed_instance&) const = default;
};

struct vehicle_physics_sound_group
{
	snd_alias_lookup alias{};
	float threshold{};
	float vmin{};
	float vmax{};
	float pmin{};
	float pmax{};

	bool operator==(const vehicle_physics_sound_group&) const = default;
};

struct vehicle_physics_vfx_group
{
	std::optional<std::string> surface_effects{};
	float threshold{};
	float range0{};
	float range1{};

	bool operator==(const vehicle_physics_vfx_group&) const = default;
};

struct vehicle_physics_def
{
	std::int32_t physics_enabled{};
	std::string physics_netcode_type{};
	std::string physics_game_profile{};
	std::string physics_anim_profile{};
	std::int32_t physics_num_wheels{};
	bool physics_axle_rigid_bodies{};
	float physics_axle_body_padding{};
	std::int32_t physics_axle_body_sides{};
	std::array<std::optional<std::string>, 12> physics_wheel_bones{};
	float physics_wheel_radius{};
	float physics_susp_stiffness{};
	float physics_susp_damping{};
	float physics_susp_noise_amp{};
	float physics_frontal_suspension{};
	float physics_ackermann_ratio{};
	std::array<float, 3> physics_hard_point_offset{};
	std::array<float, 3> physics_com_offset{};
	std::array<float, 4> physics_mass_factor_contact{};
	std::array<float, 4> physics_inertia_diagonal{};
	float physics_extra_gravity_factor{};
	float physics_extra_gravity_factor_air{};
	float physics_cam_fov_delta{};
	float physics_cam_pitch_base{};
	float physics_cam_pitch_dynamic{};
	float physics_cam_range_add{};
	float physics_steering_factor{};
	float physics_steering_force{};
	float physics_steering_max_yaw_speed{};
	float physics_steering_offset_point{};
	float physics_steering_offset_point_up{};
	float physics_steering_return_speed{};
	float physics_steering_change_dir_speed{};
	float physics_steering_handbrake{};
	float physics_steering_speed{};
	float physics_steering_speed_increase{};
	float physics_engine_offset_point{};
	float physics_engine_reduction_on_hb{};
	float physics_rolling_friction{};
	float physics_lateral_friction{};
	float physics_friction_base{};
	float physics_friction_recover_speed_tgt{};
	float physics_friction_fw_redirect{};
	float physics_friction_lost{};
	float physics_friction_handbrake{};
	float physics_friction_speed_to_block{};
	float physics_decel{};
	float physics_min_speed{};
	bool physics_stabilize_roll{};
	bool physics_stabilize_pitch{};
	bool physics_contents_as_missile{};
	bool physics_pitch_inversion{};
	std::uint32_t physics_control_mode{};
	float physics_time_after_coll{};
	std::int32_t physics_cycle_cam_button{};
	std::int32_t physics_boost_button{};
	float physics_boost_speed{};
	float physics_rcp_boost_accel{};
	float physics_rcp_boost_decel{};
	bool physics_hold_to_boost{};
	snd_alias_lookup physics_rcp_boost_sound{};
	std::array<float, 3> physics_rcp_boost_shake_cam{};
	bool physics_rcp_use_change_dir_logic{};
	bool physics_rcp_use_change_dir_logic_pitch{};
	bool physics_rcp_use_roll_for_yaw_speed{};
	float physics_rcp_overshoot_protection{};
	float physics_rcp_pitch_turn_speed{};
	float physics_rcp_pitch_change_dir_factor{};
	float physics_rcp_pitch_lerp_speed{};
	float physics_rcp_pitch_lerp_speed_change_dir{};
	float physics_rcp_pitch_lerp_speed_returning{};
	float physics_rcp_pitch_max_angle{};
	float physics_rcp_yaw_turn_speed{};
	float physics_rcp_yaw_change_dir_factor{};
	float physics_rcp_yaw_lerp_speed{};
	float physics_rcp_yaw_lerp_speed_change_dir{};
	float physics_rcp_yaw_lerp_speed_returning{};
	float physics_rcp_roll_turn_speed{};
	float physics_rcp_roll_lerp_speed_steering{};
	float physics_rcp_roll_lerp_speed_change_dir{};
	float physics_rcp_roll_lerp_speed_returning{};
	float physics_rcp_roll_max_angle{};
	float physics_rcp_accel_going_down{};
	float physics_rcp_decel_going_up{};
	float physics_rcp_trace_ahead{};
	bool physics_rcp_cam1st_attach_to_tag_player{};
	std::array<float, 2> physics_rcp_cam1st_noise_scale{};
	float physics_rcp_cam1st_roll_speed{};
	float physics_rcp_cam1st_roll_percentage{};
	float physics_com_adjust_threshold{};
	float physics_com_adjust_vert_disp{};
	float physics_players_weight_factor{};
	float physics_time_to_accept_input_on_start{};
	float physics_view_direction_help{};
	float physics_stabilize_pitch_speed{};
	float physics_stabilize_roll_speed{};
	std::string spin_wheel_bones{};
	float spin_wheel_angle_offset{};
	std::int32_t treads_enabled{};
	std::int32_t treads_bone_count{};
	std::array<std::optional<std::string>, 2> treads_first_bones{};
	float friction_braking{};
	float suspension_travel{};
	float max_steering_angle{};
	float steering_lerp{};
	float min_steering_scale{};
	float min_steering_speed{};
	std::int32_t disable_wheels_turning{};
	std::int32_t disable_wheels_spinning{};
	float minimum_wheel_cast_fraction{};
	float handbrake_threshold{};
	float minimum_jolt_for_notify{};
	float path_constraint_strength_fwd{};
	float path_constraint_strength_side{};
	float path_constraint_damp_fwd{};
	float path_constraint_damp_side{};
	float crash_point_offset_fwd{};
	float crash_point_offset_up{};
	float crash_impulse_side{};
	float crash_impulse_up{};
	float wrecked_mass_scale{};
	vehicle_physics_sound_group snd_impact_hard{};
	float snd_impact_exp{};
	vehicle_physics_sound_group snd_impact_light{};
	vehicle_physics_sound_group snd_driving_fast{};
	vehicle_physics_sound_group snd_driving_slow{};
	vehicle_physics_sound_group snd_suspension{};
	vehicle_physics_sound_group snd_braking_hard{};
	vehicle_physics_sound_group snd_braking_light{};
	vehicle_physics_sound_group snd_drifting{};
	vehicle_physics_sound_group snd_skidding{};
	vehicle_physics_sound_group snd_dlc1{};
	vehicle_physics_sound_group snd_dlc2{};
	vehicle_physics_sound_group snd_dlc3{};
	vehicle_physics_sound_group snd_dlc4{};
	vehicle_physics_vfx_group vfx_impact_hard{};
	vehicle_physics_vfx_group vfx_impact_light{};
	vehicle_physics_vfx_group vfx_driving_fast{};
	vehicle_physics_vfx_group vfx_driving_slow{};
	vehicle_physics_vfx_group vfx_braking_hard{};
	vehicle_physics_vfx_group vfx_braking_light{};
	vehicle_physics_vfx_group vfx_drifting{};
	vehicle_physics_vfx_group vfx_skidding{};
	vehicle_physics_vfx_group vfx_driving_flat_fast{};
	vehicle_physics_vfx_group vfx_driving_flat_slow{};
	vehicle_physics_vfx_group vfx_dlc3{};
	vehicle_physics_vfx_group vfx_dlc4{};

	bool operator==(const vehicle_physics_def&) const = default;
};

struct spaceship_anim_parts
{
	std::array<std::optional<std::string>, 2> anim{};

	bool operator==(const spaceship_anim_parts&) const = default;
};

struct vehicle_physics_rev_modifier
{
	float skid_snd_contrib{};
	float noise_ampl{};
	float sinewave_freq{};
	float sinewave_ampl{};
	float rpm_max{};
	float rpm_scale{};
	float rpm_speed_ema_samples{};
	float inclined_threshold{};
	std::int32_t gear_count{};
	float incl_contrib{};
	float max_spin_speed_rpm_fw{};
	float max_spin_speed_rpm_bw{};

	bool operator==(const vehicle_physics_rev_modifier&) const = default;
};

struct vehicle
{
	std::optional<std::string> name{};
	std::string type{};
	std::optional<std::string> use_hint_string{};
	std::int32_t health{};
	std::int32_t input_binding_set{};
	std::int32_t hit_client_scriptables{};
	std::int32_t hit_remote_controllers{};
	std::int32_t accurate_shape_overlap{};
	std::int32_t num_doors{};
	float max_door_angle{};
	std::array<std::optional<std::string>, 4> door_bones{};
	std::optional<std::string> camera_def{};
	std::optional<std::string> kill_cam_def{};
	float tex_scroll_scale{};
	float top_speed{};
	float accel{};
	float rot_rate{};
	float rot_accel{};
	float max_body_pitch{};
	float max_body_roll{};
	float range_for_bone_controllers{};
	float fake_body_accel_pitch{};
	float fake_body_accel_roll{};
	float fake_body_vel_pitch{};
	float fake_body_vel_roll{};
	float fake_body_side_vel_pitch{};
	float fake_body_pitch_strength{};
	float fake_body_roll_strength{};
	float fake_body_pitch_dampening{};
	float fake_body_roll_dampening{};
	float fake_body_boat_rocking_amplitude{};
	float fake_body_boat_rocking_period{};
	float fake_body_boat_rocking_rotation_period{};
	float fake_body_boat_rocking_fadeout_speed{};
	float boat_bouncing_min_force{};
	float boat_bouncing_max_force{};
	float boat_bouncing_rate{};
	float boat_bouncing_fadein_speed{};
	float boat_bouncing_fadeout_steering_angle{};
	float collision_damage{};
	float collision_speed{};
	bool collision_damage_ignores_impact_angle{};
	std::array<float, 3> killcam_offset{};
	std::int32_t player_protected{};
	std::int32_t bullet_damage{};
	std::int32_t armor_piercing_damage{};
	std::int32_t grenade_damage{};
	std::int32_t projectile_damage{};
	std::int32_t projectile_splash_damage{};
	std::int32_t heavy_explosive_damage{};
	vehicle_physics_def vehicle_physics_def{};
	std::int32_t veh_helicopter_lock_altitude{};
	std::int32_t veh_helicopter_offset_from_mesh{};
	float veh_helicopter_altitude_offset{};
	float veh_helicopter_pitch_offset{};
	float veh_helicopter_bounds_radius{};
	float veh_helicopter_max_speed{};
	float veh_helicopter_max_speed_vertical{};
	float veh_helicopter_max_accel{};
	float veh_helicopter_max_accel_vertical{};
	float veh_helicopter_deceleration_fwd{};
	float veh_helicopter_deceleration_side{};
	float veh_helicopter_deceleration_up{};
	float veh_helicopter_max_yaw_rate{};
	float veh_helicopter_max_yaw_accel{};
	float veh_helicopter_tilt_from_velocity{};
	float veh_helicopter_tilt_from_controller_axes{};
	float veh_helicopter_tilt_from_acceleration{};
	float veh_helicopter_tilt_from_deceleration{};
	float veh_helicopter_tilt_from_fwd_and_yaw_vel_at_max_tilt{};
	float veh_helicopter_tilt_from_fwd_and_yaw{};
	float veh_helicopter_tilt_momentum{};
	float veh_helicopter_tilt_speed{};
	float veh_helicopter_max_pitch{};
	float veh_helicopter_max_roll{};
	float veh_helicopter_hover_speed_threshold{};
	float veh_helicopter_jitter_jerkyness{};
	float veh_helicopter_lookahead_time{};
	std::int32_t veh_helicopter_soft_collisions{};
	std::int32_t veh_helicopter_use_ground_fx{};
	fx_combined_def veh_helicopter_ground_fx{};
	fx_combined_def veh_helicopter_ground_water_fx{};
	float veh_helicopter_ground_fx_default_repeat_rate{};
	float veh_helicopter_ground_fx_slowest_repeat_rate{};
	float veh_helicopter_ground_fx_fastest_repeat_rate{};
	float veh_helicopter_ground_fx_min_ground_dist{};
	float veh_helicopter_ground_fx_max_ground_dist{};
	float veh_rotor_max_velocity{};
	float veh_rotor_max_accel{};
	float veh_rotor_accel_tilt_blend{};
	float veh_rotor_max_vehicle_spin{};
	float veh_rotor_max_angle{};
	float veh_rotor_spin_speed{};
	float veh_rotor_spin_vertical_speed_threshold{};
	float veh_rotor_max_spin_addition{};
	float ss_weapon_switch_delay_seconds{};
	float ss_weapon_raise_delay_seconds{};
	std::array<float, 3> ss_fly_max_speed_mph{};
	std::array<float, 3> ss_fly_max_acceleration_mphps{};
	std::array<float, 3> ss_fly_max_counter_acceleration_mphps{};
	std::array<float, 3> ss_fly_friction_mphps{};
	std::array<float, 3> ss_fly_juke_speed_mph{};
	std::array<float, 3> ss_hover_max_speed_mph{};
	std::array<float, 3> ss_hover_max_acceleration_mphps{};
	std::array<float, 3> ss_hover_max_counter_acceleration_mphps{};
	std::array<float, 3> ss_hover_friction_mphps{};
	std::array<float, 3> ss_hover_juke_speed_mph{};
	float ss_fly_juke_ramp_time{};
	float ss_hover_juke_ramp_time{};
	float ss_braking_friction_mphps{};
	float ss_fly_redirect_velocity_scale{};
	float ss_fly_slow_redirect_velocity_scale{};
	float ss_hover_redirect_velocity_scale{};
	float ss_fly_min_speed_ratio{};
	bool ss_project_velocity_desires_to_xy{};
	bool ss_enforce_min_ground_clearance{};
	float ss_min_ground_clearance_height{};
	float ss_min_ground_clearance_lift_force{};
	bool ss_terrain_mode_movement{};
	float ss_terrain_mode_gravity{};
	float ss_terrain_mode_fly_time{};
	std::array<float, 2> ss_aim_offset_clamp_deg{};
	std::array<float, 2> ss_pitch_up_soft_limit_deg{};
	std::array<float, 2> ss_pitch_down_soft_limit_deg{};
	float ss_clutch_pitch_speed_scale{};
	std::array<float, 2> ss_fly_aim_speed_dps{};
	std::array<float, 2> ss_fly_slow_aim_speed_dps{};
	std::array<float, 2> ss_hover_aim_speed_dps{};
	std::array<float, 2> ss_fly_aim_drift_scale{};
	std::array<float, 2> ss_fly_slow_aim_drift_scale{};
	std::array<float, 2> ss_hover_aim_drift_scale{};
	float ss_min_spring_oscillation{};
	float ss_max_spring_oscillation{};
	float ss_min_spring_strength{};
	float ss_max_spring_strength{};
	float ss_min_spring_ang_speed{};
	float ss_max_spring_ang_speed{};
	float ss_spring_controller_contribution{};
	float ss_tilt_spring_oscillation{};
	float ss_tilt_spring_strength{};
	float ss_tilt_spring_angle_contribution{};
	std::int32_t ss_ads_to_lock_on{};
	float ss_lock_on_min_angle{};
	float ss_lock_on_max_angle{};
	float ss_pilot_assist_no_lock_aim_scale{};
	float ss_pilot_assist_no_lock_min_angle{};
	float ss_pilot_assist_no_lock_max_angle{};
	float ss_pilot_assist_aim_scale{};
	float ss_pilot_assist_min_angle{};
	float ss_pilot_assist_max_angle{};
	std::array<float, 2> ss_auto_level_strength_hover{};
	std::array<float, 2> ss_auto_level_strength_fly{};
	bool ss_is_roll_right_stick{};
	float ss_roll_max_speed_dps{};
	float ss_boost_speed_scale{};
	float ss_boost_thrust_scale{};
	bool ss_enable_boost_to_fly{};
	float ss_boost_time{};
	float ss_boost_regen_time{};
	float ss_boost_regen_stall_time{};
	float ss_boost_min_threshold_ratio{};
	float ss_collision_radius_i{};
	float ss_cylinder_height_i{};
	bool ss_use_cylinder{};
	bool ss_use_player_clip{};
	float ss_collision_offset_i{};
	float ss_collision_hard_threshold_mph{};
	float ss_fov_fly_offset_deg{};
	float ss_fov_fly_boost_offset_deg{};
	float ss_fov_hover_offset_deg{};
	float ss_zoom_blend_speed_dps{};
	float ss_zoom_max_angle{};
	float ss_zoom_min_dist_i{};
	float ss_zoom_ideal_dist_i{};
	float ss_zoom_max_dist_i{};
	bool ss_show_pilot_viewmodel{};
	float ss_camera_anim_scale{};
	std::array<float, 2> ss_fly_tilt_ship_angles{};
	float ss_fly_tilt_camera_scale{};
	std::array<float, 2> ss_hover_tilt_ship_angles{};
	float ss_hover_tilt_camera_scale{};
	float ss_motion_idle_rate_min_speed{};
	float ss_motion_idle_rate_max_speed{};
	float ss_motion_idle_min_rate{};
	float ss_motion_idle_max_rate{};
	std::optional<std::string> ss_anim_tree{};
	std::array<spaceship_anim_parts, 33> ss_anim_parts{};
	fx_combined_def ss_thrust_fx_loop{};
	fx_combined_def ss_juke_fx{};
	std::optional<std::string> ss_idle_rumble{};
	std::optional<std::string> ss_small_rumble{};
	std::optional<std::string> ss_med_rumble{};
	std::optional<std::string> ss_large_rumble{};
	snd_alias_lookup rattle_loop{};
	float rattle_loop_min_volume{};
	float rattle_loop_max_volume{};
	snd_alias_lookup air_loop{};
	float air_loop_min_volume{};
	float air_loop_max_volume{};
	snd_alias_lookup engine_loop{};
	float engine_loop_min_volume{};
	float engine_loop_max_volume{};
	float engine_loop_min_pitch{};
	float engine_loop_max_pitch{};
	snd_alias_lookup hover_loop{};
	float hover_loop_min_volume{};
	float hover_loop_max_volume{};
	float hover_loop_min_pitch{};
	float hover_loop_max_pitch{};
	snd_alias_lookup boost_loop{};
	float boost_loop_max_volume{};
	float boost_loop_max_pitch{};
	snd_alias_lookup ss_thrust_loop{};
	float ss_thrust_loop_max_volume{};
	float ss_thrust_loop_max_pitch{};
	snd_alias_lookup boost_start{};
	snd_alias_lookup boost_stop{};
	snd_alias_lookup boost_depleted{};
	snd_alias_lookup boost_unavailable{};
	snd_alias_lookup juke_left{};
	snd_alias_lookup juke_right{};
	snd_alias_lookup juke_up_down{};
	snd_alias_lookup juke_back{};
	snd_alias_lookup juke_front{};
	snd_alias_lookup flight_on{};
	snd_alias_lookup flight_off{};
	float flight_on_timer{};
	float flight_off_timer{};
	snd_alias_lookup hard_collision{};
	snd_alias_lookup soft_collision{};
	std::int32_t cam_look_enabled{};
	std::int32_t cam_relative_control{};
	std::int32_t cam_remote_drive{};
	float cam_lerp{};
	float cam_height{};
	float cam_radius{};
	float cam_pitch_influence{};
	float cam_yaw_influence{};
	float cam_roll_influence{};
	float cam_fov_increase{};
	float cam_fov_offset{};
	float cam_fov_speed{};
	float cam_return_speed{};
	float cam_return_lerp{};
	float cam_vehicle_angle_pitch_rate{};
	float cam_vehicle_angle_yaw_rate{};
	float cam_vehicle_angle_roll_rate{};
	std::int32_t veh_cam_use_gdt{};
	float veh_cam_angles_pitch{};
	float veh_cam_angles_yaw{};
	float veh_cam_angles_roll{};
	float veh_cam_offset_x{};
	float veh_cam_offset_y{};
	float veh_cam_offset_z{};
	float veh_cam_radius{};
	float veh_cam_speed_influence{};
	float veh_cam_pitch_turn_rate{};
	float veh_cam_pitch_clamp{};
	float veh_cam_yaw_turn_rate{};
	float veh_cam_yaw_clamp{};
	std::string veh_cam_z_offset_mode{};
	float veh_cam_angles_pitch3_p{};
	float veh_cam_angles_yaw3_p{};
	float veh_cam_angles_roll3_p{};
	float veh_cam_offset_x3_p{};
	float veh_cam_offset_y3_p{};
	float veh_cam_offset_z3_p{};
	float veh_cam_radius3_p{};
	float veh_cam_speed_influence3_p{};
	float veh_cam_pitch_turn_rate3_p{};
	float veh_cam_pitch_clamp3_p{};
	float veh_cam_yaw_turn_rate3_p{};
	float veh_cam_yaw_clamp3_p{};
	std::string veh_cam_z_offset_mode3_p{};
	std::optional<std::string> turret_weapon_name{};
	std::optional<std::string> turret_weapon{};
	float turret_horiz_span_left{};
	float turret_horiz_span_right{};
	float turret_vert_span_up{};
	float turret_vert_span_down{};
	float turret_horiz_resist_left{};
	float turret_horiz_resist_right{};
	float turret_vert_resist_up{};
	float turret_vert_resist_down{};
	float turret_rot_rate{};
	std::string turret_fire_type{};
	snd_alias_lookup turret_spin_snd{};
	snd_alias_lookup turret_stop_snd{};
	std::int32_t trophy_enabled{};
	float trophy_radius{};
	float trophy_inactive_radius{};
	std::int32_t trophy_ammo_count{};
	float trophy_reload_time{};
	std::array<std::optional<std::string>, 4> trophy_tags{};
	fx_combined_def trophy_explode_fx{};
	fx_combined_def trophy_flash_fx{};
	std::optional<std::string> compass_friendly_icon{};
	std::optional<std::string> compass_enemy_icon{};
	std::optional<std::string> compass_friendly_alt_icon{};
	std::optional<std::string> compass_enemy_alt_icon{};
	std::int32_t compass_icon_width{};
	std::int32_t compass_icon_height{};
	std::optional<std::string> detail_sound_bank_npc{};
	std::optional<std::string> detail_sound_bank_player{};
	snd_alias_lookup idle_low_snd{};
	snd_alias_lookup idle_high_snd{};
	snd_alias_lookup engine_low_snd{};
	snd_alias_lookup engine_high_snd{};
	snd_alias_lookup fallback_idle_low_snd{};
	snd_alias_lookup fallback_idle_high_snd{};
	snd_alias_lookup fallback_engine_low_snd{};
	snd_alias_lookup fallback_engine_high_snd{};
	float engine_snd_speed{};
	std::optional<std::string> audio_origin_tag{};
	snd_alias_lookup main_rotor_low_rpm_snd{};
	snd_alias_lookup main_rotor_operating_rpm_snd{};
	snd_alias_lookup main_rotor_operating_effort_snd{};
	snd_alias_lookup tail_rotor_low_rpm_snd{};
	snd_alias_lookup tail_rotor_operating_rpm_snd{};
	snd_alias_lookup tail_rotor_operating_effort_snd_left{};
	snd_alias_lookup tail_rotor_operating_effort_snd_right{};
	snd_alias_lookup physics_heli_startup{};
	snd_alias_lookup physics_heli_shutdown{};
	snd_alias_lookup turbine_low_rpm_snd{};
	snd_alias_lookup turbine_operating_rpm_snd{};
	bool use_rev_audio_settings{};
	snd_alias_lookup rev_low_fidelity_mod{};
	snd_alias_lookup rev_high_fidelity_mod{};
	float rev_player_priority{};
	vehicle_physics_rev_modifier rev_modifier{};
	snd_alias_lookup engine_start_up_snd{};
	std::int32_t engine_start_up_length{};
	snd_alias_lookup engine_shutdown_snd{};
	snd_alias_lookup engine_idle_snd{};
	snd_alias_lookup engine_sustain_snd{};
	snd_alias_lookup engine_ramp_up_snd{};
	std::int32_t engine_ramp_up_length{};
	snd_alias_lookup engine_ramp_down_snd{};
	std::int32_t engine_ramp_down_length{};
	snd_alias_lookup suspension_soft_snd{};
	float suspension_soft_compression{};
	snd_alias_lookup suspension_hard_snd{};
	float suspension_hard_compression{};
	snd_alias_lookup collision_snd{};
	float collision_blend_speed{};
	snd_alias_lookup speed_snd{};
	float speed_snd_blend_speed{};
	std::optional<std::string> surface_snd_name{};
	float surface_snd_blend_speed{};
	float slide_volume{};
	float slide_blend_speed{};
	float in_air_pitch{};
	std::optional<std::string> sound_trigger_override_zone{};
	bool sound_trigger_override_reverb{};
	bool sound_trigger_override_mix{};
	bool sound_trigger_override_filter{};
	bool sound_trigger_override_occlusion{};
	bool sound_trigger_override_ambient{};
	bool sound_trigger_override_ambient_events{};
	bool sound_trigger_override_adsr{};
	std::int32_t vision_blend_time{};
	std::optional<std::string> global_vision_settings{};
	std::optional<std::string> map_vision_settings{};
	std::optional<std::string> lui_crosshair_widget{};
	std::array<float, 8> dlc_float{};

	bool operator==(const vehicle&) const = default;
};

struct x_cam_target_model
{
	std::optional<std::string> name{};
	std::optional<std::vector<x_cam_target_model_frame>> animation_frames{};

	bool operator==(const x_cam_target_model&) const = default;
};

struct xcam
{
	std::optional<std::string> name{};
	std::optional<std::string> parent_scene{};
	bool hide_hud{};
	bool looping{};
	bool hide_local_player_model{};
	bool use_first_person_player_model{};
	bool disable_near_dof{};
	bool auto_motion_blur{};
	bool ease_animations_out{};
	bool show_non_animated_players{};
	std::int32_t camera_count{};
	std::int32_t frames{};
	std::int32_t frame_rate{};
	std::int32_t frame_name_count{};
	std::array<float, 4> rotation_offset_inv{};
	std::array<float, 3> offset{};
	std::optional<x_cam_target_model> target_model_bone_roots{};
	std::optional<std::vector<x_cam_camera>> cameras{};
	std::optional<std::vector<x_cam_camera_switch_params>> camera_switch_params{};
	std::optional<std::vector<x_cam_frame_name>> frame_names{};
	std::optional<std::vector<x_anim_notify_info>> notify{};
	std::uint8_t notify_count{};
	std::optional<std::vector<x_anim_notify_info>> startup_notify{};
	std::uint8_t startup_notify_count{};
	std::optional<std::vector<x_anim_notify_info>> shutdown_notify{};
	std::uint8_t shutdown_notify_count{};
	std::array<float, 3> right_stick_rotate_offset{};
	std::array<float, 2> right_stick_rotate_max_degrees{};
	float free_cursor_parallax{};

	bool operator==(const xcam&) const = default;
};

struct x_cam_target_model_frame
{
	std::array<float, 3> offset{};
	std::array<float, 4> axis_quat{};

	bool operator==(const x_cam_target_model_frame&) const = default;
};

struct x_cam_camera
{
	std::optional<std::string> name{};
	std::int32_t index{};
	std::string aperture_type{};
	std::optional<std::vector<x_cam_frame>> animation_frames{};
	float far_z{};
	float focus_shift_multiplier{};

	bool operator==(const x_cam_camera&) const = default;
};

struct x_cam_frame
{
	std::int32_t frame_num{};
	std::array<float, 3> origin{};
	std::array<float, 4> angle_quat{};
	float fov{};
	float focal_length{};
	float focal_distance{};
	float f_stop{};

	bool operator==(const x_cam_frame&) const = default;
};

struct x_cam_camera_switch_params
{
	std::int32_t frame_num{};
	std::int32_t camera1_index{};
	std::int32_t camera2_index{};
	float dissolve{};

	bool operator==(const x_cam_camera_switch_params&) const = default;
};

struct x_cam_frame_name
{
	std::optional<std::string> frame_name{};
	std::int32_t frame_num{};

	bool operator==(const x_cam_frame_name&) const = default;
};

struct x_anim_notify_info
{
	std::optional<std::string> name{};
	float time{};

	bool operator==(const x_anim_notify_info&) const = default;
};

struct spatial_partition_transient_population
{
	std::optional<std::vector<spatial_partition_population_node>> buckets{};
	std::optional<std::vector<spatial_partition_population_node>> bucket_tails{};
	std::optional<std::vector<std::uint32_t>> partition_ids{};
	std::uint32_t current_population{};

	bool operator==(const spatial_partition_transient_population&) const = default;
};

struct spatial_partition_population_node
{
	std::uint32_t spatial_node_next{};

	bool operator==(const spatial_partition_population_node&) const = default;
};

struct dynentitylist
{
	std::optional<std::string> name{};
	std::uint16_t index{};
	std::array<std::uint32_t, 2> dyn_ent_count{};
	std::uint32_t dyn_ent_count_total{};
	std::optional<std::vector<dyn_entity_def>> dyn_ent_def_list_0{};
	std::optional<std::vector<dyn_entity_def>> dyn_ent_def_list_1{};
	std::optional<spatial_partition_transient_population> dyn_ent_spatial_population_0{};
	std::optional<spatial_partition_transient_population> dyn_ent_spatial_population_1{};
	spatial_partition_population_node dyn_ent_no_spatial_list{};
	spatial_partition_population_node dyn_ent_no_spatial_list_tail{};

	bool operator==(const dynentitylist&) const = default;
};

struct client_entity_link_to_def
{
	std::int32_t anchor_index{};
	std::array<float, 3> origin_offset{};
	std::array<float, 3> angle_offset{};

	bool operator==(const client_entity_link_to_def&) const = default;
};

struct dyn_entity_def
{
	std::optional<std::string> base_model{};
	std::optional<client_entity_link_to_def> link_to{};
	gfx_placement initial_pose{};
	spatial_partition_population_node spatial_node{};
	std::uint8_t client_active_mask{};
	std::string basis{};
	std::string type{};
	std::string priority{};
	std::uint32_t spatial_partition_index{};
	std::array<std::uint16_t, 2> client_id{};
	std::uint32_t scriptable_map_index{};
	std::uint16_t dyn_entity_list_id{};
	std::uint16_t brush_model{};
	std::uint16_t scriptable_sub_index{};
	std::uint16_t transient_index_stored{};
	std::uint8_t scriptable_max_part_count_or_physics_asset_rigid_bodies_count{}; // the bits of scriptableMaxPartCount or physicsAssetRigidBodiesCount
	bool spawn_active{};
	bool no_physics{};
	bool distant_shadows{};
	bool no_spot_shadows{};
	std::uint8_t collision_flags{};
	bool is_transient{};
	bool transient_zone_loaded{};

	bool operator==(const dyn_entity_def&) const = default;
};

inline void validate(const att_ammo_general& data)
{
	if (std::ranges::none_of(weap_fire_type_t_names, [&](const auto& entry) { return entry.second == data.fire_type; }))
	{
		throw std::runtime_error("att_ammo_general.fire_type names no value");
	}

}

inline void validate(const att_ammo_type& data)
{
	(void) data;
}

inline void validate(const att_impact& data)
{
	(void) data;
}

inline void validate(const att_ammo_tracer& data)
{
	if (std::ranges::none_of(tracer_style_t_names, [&](const auto& entry) { return entry.second == data.tracer_style; }))
	{
		throw std::runtime_error("att_ammo_tracer.tracer_style names no value");
	}

}

inline void validate(const att_penetration& data)
{
	if (std::ranges::none_of(penetrate_type_names, [&](const auto& entry) { return entry.second == data.penetrate_type; }))
	{
		throw std::runtime_error("att_penetration.penetrate_type names no value");
	}

}

inline void validate(const att_sight& data)
{
	(void) data;
}

inline void validate(const att_hold_breath& data)
{
	(void) data;
}

inline void validate(const att_reload& data)
{
	(void) data;
}

inline void validate(const att_add_ons& data)
{
	(void) data;
}

inline void validate(const laser_settings& data)
{
	(void) data;
}

inline void validate(const att_laser& data)
{
	validate(data.laser_settings);

}

inline void validate(const att_post& data)
{
	(void) data;
}

inline void validate(const att_general& data)
{
	if (std::ranges::none_of(reticle_type_names, [&](const auto& entry) { return entry.second == data.reticle_type; }))
	{
		throw std::runtime_error("att_general.reticle_type names no value");
	}

}

inline void validate(const att_aim_assist& data)
{
	(void) data;
}

inline void validate(const att_target_assist& data)
{
	if (std::ranges::none_of(target_assist_type_t_names, [&](const auto& entry) { return entry.second == data.target_assist_type; }))
	{
		throw std::runtime_error("att_target_assist.target_assist_type names no value");
	}

	if (std::ranges::none_of(target_assist_behavior_t_names, [&](const auto& entry) { return entry.second == data.target_assist_behavior; }))
	{
		throw std::runtime_error("att_target_assist.target_assist_behavior names no value");
	}

}

inline void validate(const att_ammunition& data)
{
	(void) data;
}

inline void validate(const weapon_damage_common& data)
{
	(void) data;
}

inline void validate(const weapon_damage_info& data)
{
	for (const auto& item : data.damage_data)
	{
		validate(item);
	}

	if (std::ranges::none_of(anim_damage_type_names, [&](const auto& entry) { return entry.second == data.death_anim_damage_type; }))
	{
		throw std::runtime_error("weapon_damage_info.death_anim_damage_type names no value");
	}

}

inline void validate(const att_damage& data)
{
	validate(data.damage_info);

	if (std::ranges::none_of(anim_damage_type_names, [&](const auto& entry) { return entry.second == data.death_anim_damage_type; }))
	{
		throw std::runtime_error("att_damage.death_anim_damage_type names no value");
	}

}

inline void validate(const att_melee& data)
{
	(void) data;
}

inline void validate(const advanced_idle_settings& data)
{
	(void) data;
}

inline void validate(const att_idle_settings& data)
{
	validate(data.advanced_idle_settings);

}

inline void validate(const scene_zoom_settings& data)
{
	if (std::ranges::none_of(ads_offset_interpolation_type_names, [&](const auto& entry) { return entry.second == data.ads_zoom_fov_lerp_type; }))
	{
		throw std::runtime_error("scene_zoom_settings.ads_zoom_fov_lerp_type names no value");
	}

}

inline void validate(const weapon_zoom_settings& data)
{
	if (std::ranges::none_of(ads_offset_interpolation_type_names, [&](const auto& entry) { return entry.second == data.ads_zoom_fov_lerp_type; }))
	{
		throw std::runtime_error("weapon_zoom_settings.ads_zoom_fov_lerp_type names no value");
	}

}

inline void validate(const zoom_settings& data)
{
	validate(data.scene);

	validate(data.weapon);

}

inline void validate(const att_ads_settings& data)
{
	validate(data.zoom_settings);

	if (std::ranges::none_of(ads_offset_interpolation_type_names, [&](const auto& entry) { return entry.second == data.ads_align_offset_lerp_type; }))
	{
		throw std::runtime_error("att_ads_settings.ads_align_offset_lerp_type names no value");
	}

	if (std::ranges::none_of(ads_offset_interpolation_type_names, [&](const auto& entry) { return entry.second == data.ads_align_lerp_type; }))
	{
		throw std::runtime_error("att_ads_settings.ads_align_lerp_type names no value");
	}

}

inline void validate(const att_ads_gun_bob_settings& data)
{
	(void) data;
}

inline void validate(const att_ads_reload_settings& data)
{
	(void) data;
}

inline void validate(const att_scope_drift_settings& data)
{
	(void) data;
}

inline void validate(const weapon_offset_pattern_scale_info& data)
{
	if (data.pattern_scales)
	{
		for (const auto& item : *data.pattern_scales)
		{
			validate(item);
		}
	}

	if (static_cast<std::int64_t>(data.num_pattern_scales) != (data.pattern_scales ? static_cast<std::int64_t>(data.pattern_scales->size()) : 0))
	{
		throw std::runtime_error("weapon_offset_pattern_scale_info.pattern_scales has a length that its count does not give");
	}

}

inline void validate(const att_ads_stance_scales& data)
{
	validate(data.crouched_weapon_offset_pattern_scales);

	validate(data.prone_weapon_offset_pattern_scales);

}

inline void validate(const att_hip_spread& data)
{
	(void) data;
}

inline void validate(const att_slide_spread& data)
{
	(void) data;
}

inline void validate(const att_gun_kick& data)
{
	(void) data;
}

inline void validate(const att_gun_tilt& data)
{
	(void) data;
}

inline void validate(const att_kick_scaling& data)
{
	(void) data;
}

inline void validate(const att_view_kick& data)
{
	(void) data;
}

inline void validate(const ads_overlay& data)
{
	if (std::ranges::none_of(weap_overlay_reticle_t_names, [&](const auto& entry) { return entry.second == data.reticle; }))
	{
		throw std::runtime_error("ads_overlay.reticle names no value");
	}

}

inline void validate(const att_ads_overlay& data)
{
	validate(data.overlay);

}

inline void validate(const att_outline& data)
{
	(void) data;
}

inline void validate(const att_ui& data)
{
	if (std::ranges::none_of(ammo_counter_clip_type_t_names, [&](const auto& entry) { return entry.second == data.ammo_counter_clip; }))
	{
		throw std::runtime_error("att_ui.ammo_counter_clip names no value");
	}

}

inline void validate(const att_rumbles& data)
{
	(void) data;
}

inline void validate(const fx_combined_def& data)
{
	(void) data;
}

inline void validate(const snd_alias_lookup& data)
{
	(void) data;
}

inline void validate(const att_projectile& data)
{
	if (std::ranges::none_of(weap_proj_exposion_t_names, [&](const auto& entry) { return entry.second == data.proj_explosion_type; }))
	{
		throw std::runtime_error("att_projectile.proj_explosion_type names no value");
	}

	validate(data.proj_explosion_effect);

	validate(data.proj_explosion_sound);

	validate(data.proj_dud_effect);

	validate(data.proj_timeout_effect);

	validate(data.proj_dud_sound);

	validate(data.proj_timeout_sound);

	validate(data.vm_proj_body_effect);

	validate(data.proj_body_effect);

	validate(data.proj_trail_effect);

	validate(data.proj_beacon_effect);

	validate(data.proj_ignition_effect);

	validate(data.proj_ignition_sound);

	if (std::ranges::none_of(weap_stickiness_type_names, [&](const auto& entry) { return entry.second == data.stickiness; }))
	{
		throw std::runtime_error("att_projectile.stickiness names no value");
	}

}

inline void validate(const att_charged& data)
{
	validate(data.charge_meter_effect);

	validate(data.charge_up_sound);

	validate(data.charge_down_sound);

	validate(data.charge_down_to_up_sound);

	validate(data.charge_up_to_down_sound);

	validate(data.charge_max_sound);

	validate(data.charge_up_sound_player);

	validate(data.charge_down_sound_player);

	validate(data.charge_down_to_up_sound_player);

	validate(data.charge_up_to_down_sound_player);

	validate(data.charge_max_sound_player);

}

inline void validate(const ads_alt_switch& data)
{
	if (std::ranges::none_of(ads_alt_switch_interp_type_names, [&](const auto& entry) { return entry.second == data.fov_interp_type_main_to_alt; }))
	{
		throw std::runtime_error("ads_alt_switch.fov_interp_type_main_to_alt names no value");
	}

	if (std::ranges::none_of(ads_alt_switch_interp_type_names, [&](const auto& entry) { return entry.second == data.fov_interp_type_alt_to_main; }))
	{
		throw std::runtime_error("ads_alt_switch.fov_interp_type_alt_to_main names no value");
	}

}

inline void validate(const att_regeneration& data)
{
	(void) data;
}

inline void validate(const att_heat& data)
{
	(void) data;
}

inline void validate(const att_movement& data)
{
	(void) data;
}

inline void validate(const att_burst& data)
{
	(void) data;
}

inline void validate(const att_hitmarker& data)
{
	if (std::ranges::none_of(hitmarker_type_names, [&](const auto& entry) { return entry.second == data.hitmarker_type; }))
	{
		throw std::runtime_error("att_hitmarker.hitmarker_type names no value");
	}

}

inline void validate(const att_ads_movement& data)
{
	(void) data;
}

inline void validate(const ballistic_info_calculated& data)
{
	if ((data.distances && data.distances->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("ballistic_info_calculated has arrays that its count does not fit");
	}

}

inline void validate(const ballistic_info& data)
{
	if (data.calculated)
	{
		validate(*data.calculated);
	}

}

inline void validate(const common_sway_settings& data)
{
	(void) data;
}

inline void validate(const hip_sway_settings& data)
{
	validate(data.common);

}

inline void validate(const ads_sway_settings& data)
{
	validate(data.common);

}

inline void validate(const advanced_hip_sway_settings& data)
{
	(void) data;
}

inline void validate(const sway_settings& data)
{
	validate(data.hip);

	validate(data.ads);

	validate(data.adv);

}

inline void validate(const att_dlc_overrides& data)
{
	(void) data;
}

inline void validate(const att_dlc_scalars& data)
{
	(void) data;
}

inline void validate(const attachment& data)
{
	if (!data.sz_internal_name)
	{
		throw std::runtime_error("attachment has no name");
	}

	detail::name(*data.sz_internal_name, "attachment");

	if (std::ranges::none_of(attachment_type_names, [&](const auto& entry) { return entry.second == data.type; }))
	{
		throw std::runtime_error("attachment.type names no value");
	}

	if (std::ranges::none_of(alt_mode_player_anim_type_t_names, [&](const auto& entry) { return entry.second == data.alt_mode_player_anim_type; }))
	{
		throw std::runtime_error("attachment.alt_mode_player_anim_type names no value");
	}

	if (std::ranges::none_of(weap_type_t_names, [&](const auto& entry) { return entry.second == data.weapon_type; }))
	{
		throw std::runtime_error("attachment.weapon_type names no value");
	}

	if (std::ranges::none_of(weap_class_t_names, [&](const auto& entry) { return entry.second == data.weap_class; }))
	{
		throw std::runtime_error("attachment.weap_class names no value");
	}

	if (data.ammogeneral)
	{
		validate(*data.ammogeneral);
	}

	if (data.ammotype)
	{
		validate(*data.ammotype);
	}

	if (data.impact)
	{
		validate(*data.impact);
	}

	if (std::ranges::none_of(dynamic_attachment_type_names, [&](const auto& entry) { return entry.second == data.dynamic_attachment_type; }))
	{
		throw std::runtime_error("attachment.dynamic_attachment_type names no value");
	}

	if (std::ranges::none_of(weapon_flag_override_names, [&](const auto& entry) { return entry.second == data.disable_input_driven_view_return_dampening; }))
	{
		throw std::runtime_error("attachment.disable_input_driven_view_return_dampening names no value");
	}

	if (data.tracer)
	{
		validate(*data.tracer);
	}

	if (data.penetration)
	{
		validate(*data.penetration);
	}

	if (data.sight)
	{
		validate(*data.sight);
	}

	if (data.hold_breath)
	{
		validate(*data.hold_breath);
	}

	if (data.reload)
	{
		validate(*data.reload);
	}

	if (data.add_ons)
	{
		validate(*data.add_ons);
	}

	if (data.laser)
	{
		validate(*data.laser);
	}

	if (data.post)
	{
		validate(*data.post);
	}

	if (data.general)
	{
		validate(*data.general);
	}

	if (data.aim_assist)
	{
		validate(*data.aim_assist);
	}

	if (data.target_assist)
	{
		validate(*data.target_assist);
	}

	if (data.ammunition)
	{
		for (const auto& item : *data.ammunition)
		{
			validate(item);
		}
	}

	if (data.damage)
	{
		validate(*data.damage);
	}

	if (data.melee)
	{
		validate(*data.melee);
	}

	if (data.idle_settings)
	{
		validate(*data.idle_settings);
	}

	if (data.ads_settings)
	{
		validate(*data.ads_settings);
	}

	if (data.ads_settings_main)
	{
		validate(*data.ads_settings_main);
	}

	if (data.ads_gun_bob_settings)
	{
		validate(*data.ads_gun_bob_settings);
	}

	if (data.ads_gun_bob_settings_main)
	{
		validate(*data.ads_gun_bob_settings_main);
	}

	if (data.ads_reload_settings)
	{
		validate(*data.ads_reload_settings);
	}

	if (data.scope_drift_settings)
	{
		validate(*data.scope_drift_settings);
	}

	if (data.scope_drift_settings_main)
	{
		validate(*data.scope_drift_settings_main);
	}

	if (data.ads_stance_scales)
	{
		validate(*data.ads_stance_scales);
	}

	if (data.hip_spread)
	{
		validate(*data.hip_spread);
	}

	if (data.slide_spread)
	{
		validate(*data.slide_spread);
	}

	if (data.gun_kick)
	{
		validate(*data.gun_kick);
	}

	if (data.gun_tilt)
	{
		validate(*data.gun_tilt);
	}

	if (data.kick_scaling)
	{
		validate(*data.kick_scaling);
	}

	if (data.view_kick)
	{
		validate(*data.view_kick);
	}

	if (data.ads_overlay)
	{
		validate(*data.ads_overlay);
	}

	if (data.outline)
	{
		validate(*data.outline);
	}

	if (data.ui)
	{
		validate(*data.ui);
	}

	if (data.rumbles)
	{
		validate(*data.rumbles);
	}

	if (data.projectile)
	{
		validate(*data.projectile);
	}

	if (data.charged)
	{
		validate(*data.charged);
	}

	if (data.ads_alt_switch)
	{
		validate(*data.ads_alt_switch);
	}

	if (data.regeneration)
	{
		validate(*data.regeneration);
	}

	if (data.heat)
	{
		validate(*data.heat);
	}

	if (data.movement)
	{
		validate(*data.movement);
	}

	if (data.movement_scale)
	{
		validate(*data.movement_scale);
	}

	if (data.movement_scale_main)
	{
		validate(*data.movement_scale_main);
	}

	if (data.burst)
	{
		validate(*data.burst);
	}

	if (data.hitmarker)
	{
		validate(*data.hitmarker);
	}

	if (data.ads_movement)
	{
		validate(*data.ads_movement);
	}

	if (data.ballistic_info)
	{
		validate(*data.ballistic_info);
	}

	if (data.sway_settings)
	{
		validate(*data.sway_settings);
	}

	if (data.weapon_offset_pattern_scales)
	{
		validate(*data.weapon_offset_pattern_scales);
	}

	if (std::ranges::none_of(ads_offset_interpolation_type_names, [&](const auto& entry) { return entry.second == data.hip_offset_lerp_type; }))
	{
		throw std::runtime_error("attachment.hip_offset_lerp_type names no value");
	}

	if (data.dlc_overrides)
	{
		validate(*data.dlc_overrides);
	}

	if (data.dlc_scalars)
	{
		validate(*data.dlc_scalars);
	}

	if ((data.world_model_variations && data.world_model_variations->size() > 0xFFFFFFFFull) || (data.view_model_variations && data.view_model_variations->size() > 0xFFFFFFFFull) || (!data.world_model_variations != !data.view_model_variations) || (data.world_model_variations && data.view_model_variations && data.world_model_variations->size() != data.view_model_variations->size()))
	{
		throw std::runtime_error("attachment has arrays that its count does not fit");
	}

	if ((data.hide_tags && data.hide_tags->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("attachment has arrays that its count does not fit");
	}

	if ((data.hide_tags_non_view_model && data.hide_tags_non_view_model->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("attachment has arrays that its count does not fit");
	}

	if ((data.show_tags && data.show_tags->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("attachment has arrays that its count does not fit");
	}

	if ((data.reticles && data.reticles->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("attachment has arrays that its count does not fit");
	}

	if ((data.alt_reticles && data.alt_reticles->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("attachment has arrays that its count does not fit");
	}

}

inline void validate(const weapon_offset_pattern_scale& data)
{
	(void) data;
}

inline void validate(const animation_state_machine& data)
{
	if (data.states)
	{
		for (const auto& item : *data.states)
		{
			validate(item);
		}
	}

	if (data.aim_sets)
	{
		for (const auto& item : *data.aim_sets)
		{
			validate(item);
		}
	}

	if ((data.states && data.states->size() > 0xFFFFull))
	{
		throw std::runtime_error("animation_state_machine has arrays that its count does not fit");
	}

	if ((data.aim_sets && data.aim_sets->size() > 0xFFFFull))
	{
		throw std::runtime_error("animation_state_machine has arrays that its count does not fit");
	}

}

inline void validate(const animclass& data)
{
	if (!data.class_name)
	{
		throw std::runtime_error("animclass has no name");
	}

	detail::name(*data.class_name, "animclass");

	if (data.state_machine)
	{
		validate(*data.state_machine);
	}

	if (std::ranges::none_of(animation_controller_names, [&](const auto& entry) { return entry.second == data.anim_ctrl; }))
	{
		throw std::runtime_error("animclass.anim_ctrl names no value");
	}

	if (data.effect_defs)
	{
		for (const auto& item : *data.effect_defs)
		{
			validate(item);
		}
	}

	if ((data.sound_notes && data.sound_notes->size() > 0xFFFFull) || (data.sound_names && data.sound_names->size() > 0xFFFFull) || (data.sound_options && data.sound_options->size() > 0xFFFFull) || (!data.sound_notes != !data.sound_names) || (data.sound_notes && data.sound_names && data.sound_notes->size() != data.sound_names->size()) || (!data.sound_notes != !data.sound_options) || (data.sound_notes && data.sound_options && data.sound_notes->size() != data.sound_options->size()))
	{
		throw std::runtime_error("animclass has arrays that its count does not fit");
	}

	if ((data.effect_notes && data.effect_notes->size() > 0xFFFFull) || (data.effect_defs && data.effect_defs->size() > 0xFFFFull) || (data.effect_tags && data.effect_tags->size() > 0xFFFFull) || (!data.effect_notes != !data.effect_defs) || (data.effect_notes && data.effect_defs && data.effect_notes->size() != data.effect_defs->size()) || (!data.effect_notes != !data.effect_tags) || (data.effect_notes && data.effect_tags && data.effect_notes->size() != data.effect_tags->size()))
	{
		throw std::runtime_error("animclass has arrays that its count does not fit");
	}

}

inline void validate(const animation_state& data)
{
	if (data.anim_entries)
	{
		for (const auto& item : *data.anim_entries)
		{
			validate(item);
		}
	}

	if (data.alias_list)
	{
		for (const auto& item : *data.alias_list)
		{
			validate(item);
		}
	}

	if ((data.anim_entries && data.anim_entries->size() > 0xFFull) || (data.anim_indices && data.anim_indices->size() > 0xFFull) || (!data.anim_entries != !data.anim_indices) || (data.anim_entries && data.anim_indices && data.anim_entries->size() != data.anim_indices->size()))
	{
		throw std::runtime_error("animation_state has arrays that its count does not fit");
	}

	if ((data.alias_list && data.alias_list->size() > 0xFFull))
	{
		throw std::runtime_error("animation_state has arrays that its count does not fit");
	}

}

inline void validate(const animation_entry& data)
{
	(void) data;
}

inline void validate(const anim_alias& data)
{
	if (data.alias_info)
	{
		for (const auto& item : *data.alias_info)
		{
			validate(item);
		}
	}

	if ((data.alias_info && data.alias_info->size() > 0xFFull))
	{
		throw std::runtime_error("anim_alias has arrays that its count does not fit");
	}

}

inline void validate(const anim_alias_info& data)
{
	(void) data;
}

inline void validate(const animation_aim_set& data)
{
	if ((data.anim_name && data.anim_name->size() > 0x7FFFFFFFull) || (data.anim_indices && data.anim_indices->size() > 0x7FFFFFFFull) || (data.aim_node_indices && data.aim_node_indices->size() > 0x7FFFFFFFull) || (!data.anim_name != !data.anim_indices) || (data.anim_name && data.anim_indices && data.anim_name->size() != data.anim_indices->size()) || (!data.anim_name != !data.aim_node_indices) || (data.anim_name && data.aim_node_indices && data.anim_name->size() != data.aim_node_indices->size()))
	{
		throw std::runtime_error("animation_aim_set has arrays that its count does not fit");
	}

}

inline void validate(const luafile& data)
{
	if (!data.name)
	{
		throw std::runtime_error("luafile has no name");
	}

	detail::name(*data.name, "luafile");

	if ((data.buffer && data.buffer->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("luafile has arrays that its count does not fit");
	}

}

inline void validate(const leaderboarddef& data)
{
	if (!data.name)
	{
		throw std::runtime_error("leaderboarddef has no name");
	}

	detail::name(*data.name, "leaderboarddef");

	if (data.columns)
	{
		for (const auto& item : *data.columns)
		{
			validate(item);
		}
	}

	if (std::ranges::none_of(lb_update_type_names, [&](const auto& entry) { return entry.second == data.update_type; }))
	{
		throw std::runtime_error("leaderboarddef.update_type names no value");
	}

	if ((data.columns && data.columns->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("leaderboarddef has arrays that its count does not fit");
	}

}

inline void validate(const lb_column_def& data)
{
	if (std::ranges::none_of(lb_col_type_names, [&](const auto& entry) { return entry.second == data.type; }))
	{
		throw std::runtime_error("lb_column_def.type names no value");
	}

	if (std::ranges::none_of(lb_agg_type_names, [&](const auto& entry) { return entry.second == data.agg; }))
	{
		throw std::runtime_error("lb_column_def.agg names no value");
	}

}

inline void validate(const virtualleaderboarddef& data)
{
	if (!data.name)
	{
		throw std::runtime_error("virtualleaderboarddef has no name");
	}

	detail::name(*data.name, "virtualleaderboarddef");

	if (data.columns)
	{
		for (const auto& item : *data.columns)
		{
			validate(item);
		}
	}

	if ((data.columns && data.columns->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("virtualleaderboarddef has arrays that its count does not fit");
	}

}

inline void validate(const lb_vr_column_def& data)
{
	(void) data;
}

inline void validate(const clothasset& data)
{
	if (!data.name)
	{
		throw std::runtime_error("clothasset has no name");
	}

	detail::name(*data.name, "clothasset");

	if (data.bone_mapping_sets)
	{
		for (const auto& item : *data.bone_mapping_sets)
		{
			validate(item);
		}
	}

	if (data.simulation_lods)
	{
		for (const auto& item : *data.simulation_lods)
		{
			validate(item);
		}
	}

	if (data.sounds)
	{
		for (const auto& item : *data.sounds)
		{
			validate(item);
		}
	}

	if ((data.havok_data && data.havok_data->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("clothasset has arrays that its count does not fit");
	}

	if ((data.bone_mapping_sets && data.bone_mapping_sets->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("clothasset has arrays that its count does not fit");
	}

	if ((data.simulation_lods && data.simulation_lods->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("clothasset has arrays that its count does not fit");
	}

	if ((data.sounds && data.sounds->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("clothasset has arrays that its count does not fit");
	}

}

inline void validate(const cloth_asset_bone_mapping_set& data)
{
	if (data.bone_mappings)
	{
		for (const auto& item : *data.bone_mappings)
		{
			validate(item);
		}
	}

	if ((data.bone_mappings && data.bone_mappings->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("cloth_asset_bone_mapping_set has arrays that its count does not fit");
	}

}

inline void validate(const cloth_asset_bone_mapping& data)
{
	if ((data.scr_tag_names && data.scr_tag_names->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("cloth_asset_bone_mapping has arrays that its count does not fit");
	}

}

inline void validate(const cloth_asset_simulation_lod& data)
{
	(void) data;
}

inline void validate(const cloth_asset_sound& data)
{
	(void) data;
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

inline void validate(const st_clutter_sample_points& data)
{
	validate(data.sample_point_buffer);

	if (static_cast<std::int64_t>(data.sample_count) != (data.sample_points ? static_cast<std::int64_t>(data.sample_points->size()) : 0))
	{
		throw std::runtime_error("st_clutter_sample_points.sample_points has a length that its count does not give");
	}

}

inline void validate(const stterrain& data)
{
	if (!data.name)
	{
		throw std::runtime_error("stterrain has no name");
	}

	detail::name(*data.name, "stterrain");

	if (data.surfaces)
	{
		for (const auto& item : *data.surfaces)
		{
			validate(item);
		}
	}

	if (data.maps)
	{
		for (const auto& item : *data.maps)
		{
			validate(item);
		}
	}

	validate(data.clutter_sample_points);

	if (std::ranges::none_of(gfx_lightmap_type_names, [&](const auto& entry) { return entry.second == data.lightmap_type; }))
	{
		throw std::runtime_error("stterrain.lightmap_type names no value");
	}

	if (data.lightmaps)
	{
		for (const auto& item : *data.lightmaps)
		{
			validate(item);
		}
	}

	if ((data.surfaces && data.surfaces->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("stterrain has arrays that its count does not fit");
	}

	if ((data.maps && data.maps->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("stterrain has arrays that its count does not fit");
	}

	if ((data.flattened_images && data.flattened_images->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("stterrain has arrays that its count does not fit");
	}

	if ((data.lightmaps && data.lightmaps->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("stterrain has arrays that its count does not fit");
	}

}

inline void validate(const gfx_index_buffer& data)
{
	(void) data;
}

inline void validate(const st_terrain_grid& data)
{
	if (data.levels)
	{
		for (const auto& item : *data.levels)
		{
			validate(item);
		}
	}

	if (static_cast<std::int64_t>(data.num_levels) != (data.levels ? static_cast<std::int64_t>(data.levels->size()) : 0))
	{
		throw std::runtime_error("st_terrain_grid.levels has a length that its count does not give");
	}

}

inline void validate(const gfx_placement& data)
{
	(void) data;
}

inline void validate(const bounds& data)
{
	(void) data;
}

inline void validate(const st_clutter_sample_bitmask& data)
{
	validate(data.bitmask_buffer);

	validate(data.relocated_bitmask_buffer);

	if ((data.bitmask && data.bitmask->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("st_clutter_sample_bitmask has arrays that its count does not fit");
	}

}

inline void validate(const st_disk_terrain_surface& data)
{
	validate(data.high_lod_index_buffer);

	validate(data.grid);

	validate(data.obj_to_wld);

	validate(data.wld_bounds);

	if (data.layer_materials)
	{
		for (const auto& item : *data.layer_materials)
		{
			validate(item);
		}
	}

	if (data.displacement_scales)
	{
		for (const auto& item : *data.displacement_scales)
		{
			validate(item);
		}
	}

	if (data.clutter_layers)
	{
		for (const auto& item : *data.clutter_layers)
		{
			validate(item);
		}
	}

	validate(data.clutter_layers_buffer);

	validate(data.clutter_sample_bitmask);

	if (static_cast<std::int64_t>(data.high_lod_index_count) != (data.high_lod_indices ? static_cast<std::int64_t>(data.high_lod_indices->size()) : 0))
	{
		throw std::runtime_error("st_disk_terrain_surface.high_lod_indices has a length that its count does not give");
	}

	if ((data.layer_materials && data.layer_materials->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("st_disk_terrain_surface has arrays that its count does not fit");
	}

	if ((data.displacement_scales && data.displacement_scales->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("st_disk_terrain_surface has arrays that its count does not fit");
	}

	if ((data.clutter_layers && data.clutter_layers->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("st_disk_terrain_surface has arrays that its count does not fit");
	}

	if ((data.clutter_tile_instance_counts && data.clutter_tile_instance_counts->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("st_disk_terrain_surface has arrays that its count does not fit");
	}

	if ((data.layer_mask_map_idxs && data.layer_mask_map_idxs->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("st_disk_terrain_surface has arrays that its count does not fit");
	}

}

inline void validate(const st_terrain_grid_level& data)
{
	(void) data;
}

inline void validate(const hlsl_terrain_layer_data_code& data)
{
	(void) data;
}

inline void validate(const st_terrain_material_layer& data)
{
	validate(data.layer_data);

}

inline void validate(const st_terrain_displacement_scale& data)
{
	(void) data;
}

inline void validate(const st_terrain_clutter_layer& data)
{
	(void) data;
}

inline void validate(const st_terrain_maps& data)
{
	(void) data;
}

inline void validate(const st_lightmap& data)
{
	(void) data;
}

inline void validate(const static_model_collision_compressed_model_list& data)
{
	if (data.models)
	{
		for (const auto& item : *data.models)
		{
			validate(item);
		}
	}

	if (static_cast<std::int64_t>(data.num_models) != (data.models ? static_cast<std::int64_t>(data.models->size()) : 0))
	{
		throw std::runtime_error("static_model_collision_compressed_model_list.models has a length that its count does not give");
	}

}

inline void validate(const collisiontile& data)
{
	if (!data.name)
	{
		throw std::runtime_error("collisiontile has no name");
	}

	detail::name(*data.name, "collisiontile");

	if (data.static_model_collision)
	{
		validate(*data.static_model_collision);
	}

	if ((data.havok_tile_shape_data && data.havok_tile_shape_data->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("collisiontile has arrays that its count does not fit");
	}

}

inline void validate(const static_model_collision_compressed_model& data)
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
		throw std::runtime_error("static_model_collision_compressed_model has arrays that its count does not fit");
	}

}

inline void validate(const static_model_collision_compressed_instance& data)
{
	(void) data;
}

inline void validate(const vehicle_physics_sound_group& data)
{
	validate(data.alias);

}

inline void validate(const vehicle_physics_vfx_group& data)
{
	(void) data;
}

inline void validate(const vehicle_physics_def& data)
{
	if (std::ranges::none_of(vehicle_physics_netcode_type_names, [&](const auto& entry) { return entry.second == data.physics_netcode_type; }))
	{
		throw std::runtime_error("vehicle_physics_def.physics_netcode_type names no value");
	}

	if (std::ranges::none_of(vehicle_physics_game_profile_names, [&](const auto& entry) { return entry.second == data.physics_game_profile; }))
	{
		throw std::runtime_error("vehicle_physics_def.physics_game_profile names no value");
	}

	if (std::ranges::none_of(vehicle_physics_anim_profile_names, [&](const auto& entry) { return entry.second == data.physics_anim_profile; }))
	{
		throw std::runtime_error("vehicle_physics_def.physics_anim_profile names no value");
	}

	validate(data.physics_rcp_boost_sound);

	if (std::ranges::none_of(vehicle_wheel_spin_bone_type_names, [&](const auto& entry) { return entry.second == data.spin_wheel_bones; }))
	{
		throw std::runtime_error("vehicle_physics_def.spin_wheel_bones names no value");
	}

	validate(data.snd_impact_hard);

	validate(data.snd_impact_light);

	validate(data.snd_driving_fast);

	validate(data.snd_driving_slow);

	validate(data.snd_suspension);

	validate(data.snd_braking_hard);

	validate(data.snd_braking_light);

	validate(data.snd_drifting);

	validate(data.snd_skidding);

	validate(data.snd_dlc1);

	validate(data.snd_dlc2);

	validate(data.snd_dlc3);

	validate(data.snd_dlc4);

	validate(data.vfx_impact_hard);

	validate(data.vfx_impact_light);

	validate(data.vfx_driving_fast);

	validate(data.vfx_driving_slow);

	validate(data.vfx_braking_hard);

	validate(data.vfx_braking_light);

	validate(data.vfx_drifting);

	validate(data.vfx_skidding);

	validate(data.vfx_driving_flat_fast);

	validate(data.vfx_driving_flat_slow);

	validate(data.vfx_dlc3);

	validate(data.vfx_dlc4);

}

inline void validate(const spaceship_anim_parts& data)
{
	(void) data;
}

inline void validate(const vehicle_physics_rev_modifier& data)
{
	(void) data;
}

inline void validate(const vehicle& data)
{
	if (!data.name)
	{
		throw std::runtime_error("vehicle has no name");
	}

	detail::name(*data.name, "vehicle");

	if (std::ranges::none_of(vehicle_type_names, [&](const auto& entry) { return entry.second == data.type; }))
	{
		throw std::runtime_error("vehicle.type names no value");
	}

	validate(data.vehicle_physics_def);

	validate(data.veh_helicopter_ground_fx);

	validate(data.veh_helicopter_ground_water_fx);

	for (const auto& item : data.ss_anim_parts)
	{
		validate(item);
	}

	validate(data.ss_thrust_fx_loop);

	validate(data.ss_juke_fx);

	validate(data.rattle_loop);

	validate(data.air_loop);

	validate(data.engine_loop);

	validate(data.hover_loop);

	validate(data.boost_loop);

	validate(data.ss_thrust_loop);

	validate(data.boost_start);

	validate(data.boost_stop);

	validate(data.boost_depleted);

	validate(data.boost_unavailable);

	validate(data.juke_left);

	validate(data.juke_right);

	validate(data.juke_up_down);

	validate(data.juke_back);

	validate(data.juke_front);

	validate(data.flight_on);

	validate(data.flight_off);

	validate(data.hard_collision);

	validate(data.soft_collision);

	if (std::ranges::none_of(veh_cam_z_offset_mode_names, [&](const auto& entry) { return entry.second == data.veh_cam_z_offset_mode; }))
	{
		throw std::runtime_error("vehicle.veh_cam_z_offset_mode names no value");
	}

	if (std::ranges::none_of(veh_cam_z_offset_mode_names, [&](const auto& entry) { return entry.second == data.veh_cam_z_offset_mode3_p; }))
	{
		throw std::runtime_error("vehicle.veh_cam_z_offset_mode3_p names no value");
	}

	if (std::ranges::none_of(vehicle_turret_fire_type_names, [&](const auto& entry) { return entry.second == data.turret_fire_type; }))
	{
		throw std::runtime_error("vehicle.turret_fire_type names no value");
	}

	validate(data.turret_spin_snd);

	validate(data.turret_stop_snd);

	validate(data.trophy_explode_fx);

	validate(data.trophy_flash_fx);

	validate(data.idle_low_snd);

	validate(data.idle_high_snd);

	validate(data.engine_low_snd);

	validate(data.engine_high_snd);

	validate(data.fallback_idle_low_snd);

	validate(data.fallback_idle_high_snd);

	validate(data.fallback_engine_low_snd);

	validate(data.fallback_engine_high_snd);

	validate(data.main_rotor_low_rpm_snd);

	validate(data.main_rotor_operating_rpm_snd);

	validate(data.main_rotor_operating_effort_snd);

	validate(data.tail_rotor_low_rpm_snd);

	validate(data.tail_rotor_operating_rpm_snd);

	validate(data.tail_rotor_operating_effort_snd_left);

	validate(data.tail_rotor_operating_effort_snd_right);

	validate(data.physics_heli_startup);

	validate(data.physics_heli_shutdown);

	validate(data.turbine_low_rpm_snd);

	validate(data.turbine_operating_rpm_snd);

	validate(data.rev_low_fidelity_mod);

	validate(data.rev_high_fidelity_mod);

	validate(data.rev_modifier);

	validate(data.engine_start_up_snd);

	validate(data.engine_shutdown_snd);

	validate(data.engine_idle_snd);

	validate(data.engine_sustain_snd);

	validate(data.engine_ramp_up_snd);

	validate(data.engine_ramp_down_snd);

	validate(data.suspension_soft_snd);

	validate(data.suspension_hard_snd);

	validate(data.collision_snd);

	validate(data.speed_snd);

}

inline void validate(const x_cam_target_model& data)
{
	if (data.animation_frames)
	{
		for (const auto& item : *data.animation_frames)
		{
			validate(item);
		}
	}

	if ((data.animation_frames && data.animation_frames->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("x_cam_target_model has arrays that its count does not fit");
	}

}

inline void validate(const xcam& data)
{
	if (!data.name)
	{
		throw std::runtime_error("xcam has no name");
	}

	detail::name(*data.name, "xcam");

	if (data.target_model_bone_roots)
	{
		validate(*data.target_model_bone_roots);
	}

	if (data.cameras)
	{
		for (const auto& item : *data.cameras)
		{
			validate(item);
		}
	}

	if (data.camera_switch_params)
	{
		for (const auto& item : *data.camera_switch_params)
		{
			validate(item);
		}
	}

	if (data.frame_names)
	{
		for (const auto& item : *data.frame_names)
		{
			validate(item);
		}
	}

	if (data.notify)
	{
		for (const auto& item : *data.notify)
		{
			validate(item);
		}
	}

	if (data.startup_notify)
	{
		for (const auto& item : *data.startup_notify)
		{
			validate(item);
		}
	}

	if (data.shutdown_notify)
	{
		for (const auto& item : *data.shutdown_notify)
		{
			validate(item);
		}
	}

	if (data.cameras && data.cameras->size() != data.camera_count)
	{
		throw std::runtime_error("xcam.cameras has a length that its count does not give");
	}

	if (data.camera_switch_params && data.camera_switch_params->size() != data.frames)
	{
		throw std::runtime_error("xcam.camera_switch_params has a length that its count does not give");
	}

	if (data.frame_names && data.frame_names->size() != data.frame_name_count)
	{
		throw std::runtime_error("xcam.frame_names has a length that its count does not give");
	}

	if (data.notify && data.notify->size() != data.notify_count)
	{
		throw std::runtime_error("xcam.notify has a length that its count does not give");
	}

	if (data.startup_notify && data.startup_notify->size() != data.startup_notify_count)
	{
		throw std::runtime_error("xcam.startup_notify has a length that its count does not give");
	}

	if (data.shutdown_notify && data.shutdown_notify->size() != data.shutdown_notify_count)
	{
		throw std::runtime_error("xcam.shutdown_notify has a length that its count does not give");
	}

}

inline void validate(const x_cam_target_model_frame& data)
{
	(void) data;
}

inline void validate(const x_cam_camera& data)
{
	if (std::ranges::none_of(x_cam_aperture_type_names, [&](const auto& entry) { return entry.second == data.aperture_type; }))
	{
		throw std::runtime_error("x_cam_camera.aperture_type names no value");
	}

	if (data.animation_frames)
	{
		for (const auto& item : *data.animation_frames)
		{
			validate(item);
		}
	}

	if ((data.animation_frames && data.animation_frames->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("x_cam_camera has arrays that its count does not fit");
	}

}

inline void validate(const x_cam_frame& data)
{
	(void) data;
}

inline void validate(const x_cam_camera_switch_params& data)
{
	(void) data;
}

inline void validate(const x_cam_frame_name& data)
{
	(void) data;
}

inline void validate(const x_anim_notify_info& data)
{
	(void) data;
}

inline void validate(const spatial_partition_transient_population& data)
{
	if (data.buckets)
	{
		for (const auto& item : *data.buckets)
		{
			validate(item);
		}
	}

	if (data.bucket_tails)
	{
		for (const auto& item : *data.bucket_tails)
		{
			validate(item);
		}
	}

	if ((data.buckets && data.buckets->size() > 0xFFFFFFFFull) || (data.bucket_tails && data.bucket_tails->size() > 0xFFFFFFFFull) || (data.partition_ids && data.partition_ids->size() > 0xFFFFFFFFull) || (!data.buckets != !data.bucket_tails) || (data.buckets && data.bucket_tails && data.buckets->size() != data.bucket_tails->size()) || (!data.buckets != !data.partition_ids) || (data.buckets && data.partition_ids && data.buckets->size() != data.partition_ids->size()))
	{
		throw std::runtime_error("spatial_partition_transient_population has arrays that its count does not fit");
	}

}

inline void validate(const spatial_partition_population_node& data)
{
	(void) data;
}

inline void validate(const dynentitylist& data)
{
	if (!data.name)
	{
		throw std::runtime_error("dynentitylist has no name");
	}

	detail::name(*data.name, "dynentitylist");

	if (data.dyn_ent_def_list_0)
	{
		for (const auto& item : *data.dyn_ent_def_list_0)
		{
			validate(item);
		}
	}

	if (data.dyn_ent_def_list_1)
	{
		for (const auto& item : *data.dyn_ent_def_list_1)
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

	validate(data.dyn_ent_no_spatial_list);

	validate(data.dyn_ent_no_spatial_list_tail);

	if (static_cast<std::int64_t>(data.dyn_ent_count[0]) != (data.dyn_ent_def_list_0 ? static_cast<std::int64_t>(data.dyn_ent_def_list_0->size()) : 0))
	{
		throw std::runtime_error("dynentitylist.dyn_ent_def_list_0 has a length that its count does not give");
	}

	if (static_cast<std::int64_t>(data.dyn_ent_count[1]) != (data.dyn_ent_def_list_1 ? static_cast<std::int64_t>(data.dyn_ent_def_list_1->size()) : 0))
	{
		throw std::runtime_error("dynentitylist.dyn_ent_def_list_1 has a length that its count does not give");
	}

}

inline void validate(const client_entity_link_to_def& data)
{
	(void) data;
}

inline void validate(const dyn_entity_def& data)
{
	if (data.link_to)
	{
		validate(*data.link_to);
	}

	validate(data.initial_pose);

	validate(data.spatial_node);

	if (std::ranges::none_of(dyn_entity_basis_names, [&](const auto& entry) { return entry.second == data.basis; }))
	{
		throw std::runtime_error("dyn_entity_def.basis names no value");
	}

	if (std::ranges::none_of(dyn_entity_type_names, [&](const auto& entry) { return entry.second == data.type; }))
	{
		throw std::runtime_error("dyn_entity_def.type names no value");
	}

	if (std::ranges::none_of(dyn_entity_spatial_activation_mode_names, [&](const auto& entry) { return entry.second == data.priority; }))
	{
		throw std::runtime_error("dyn_entity_def.priority names no value");
	}

}

} // namespace content
} // namespace zonetool::asset_data
