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

// Neutral gameplay assets (docs/NEUTRAL_ASSETS.md), emitted by tools/gen_iw8_neutral.py from the IW8 structs and stream
// traversals: do not edit; change the spec (tools/neutral_families/gameplay.json) or the generator.
namespace zonetool::asset_data
{
namespace gameplay
{
inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 15> melee_impact_type_names{{
	{0, "none"},
	{1, "gun_small"},
	{2, "gun_medium"},
	{3, "gun_large"},
	{4, "knife"},
	{5, "shield"},
	{6, "user1"},
	{7, "user2"},
	{8, "user3"},
	{9, "user4"},
	{10, "user5"},
	{11, "user6"},
	{12, "user7"},
	{13, "user8"},
	{14, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 6> camera_profile_names{{
	{0, "first_person_view"},
	{1, "orbit"},
	{2, "sentry_drone"},
	{3, "killcam_vehicle"},
	{4, "first_person_vehicle"},
	{5, "max"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 9> lens_profile_mode_names{{
	{0, "none"},
	{1, "compact_portable"},
	{2, "smartphone"},
	{3, "cinematic_prime"},
	{4, "actioncam_1"},
	{5, "actioncam_2"},
	{6, "dronecam"},
	{7, "telescopic"},
	{8, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 4> camera_transition_curve_names{{
	{0, "none"},
	{1, "linear"},
	{2, "smoothstep"},
	{3, "smootherstep"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 2> x_anim_curve_type_names{{
	{0, "linear"},
	{1, "smoothstep"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 5> gesture_type_names{{
	{0, "non_directional"},
	{1, "directional"},
	{2, "look_around"},
	{3, "ik_target"},
	{4, "num"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 14> gesture_priority_names{{
	{0, "offhand_shield"},
	{1, "offhand_thrown_weapon"},
	{2, "offhand_script_weapon"},
	{3, "space_jump"},
	{4, "script"},
	{5, "slide"},
	{6, "mantle"},
	{7, "ground_pound"},
	{8, "wall_run"},
	{9, "mount"},
	{10, "demeanor"},
	{11, "mount_reach"},
	{12, "carry"},
	{13, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 4> transient_info_flags_names{{
	{1, "collision_dedupe"},
	{2, "low_alwaysloaded_flagging"},
	{4, "br_map"},
	{8, "alwaysloaded_transient_world"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 5> suit_anim_type_names{{
	{0, "anim_suit_human"},
	{1, "anim_suit_robotic"},
	{2, "anim_suit_c8"},
	{3, "anim_suit_reaper"},
	{4, "num_anim_suit_state"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 3> suit_body_anim_type_names{{
	{0, "human"},
	{1, "juggernaut"},
	{2, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 55> gesture_anim_type_t_names{{
	{0, "none"},
	{1, "begin"},
	{2, "slide_akimbo"},
	{3, "mantle_first"},
	{4, "mantle_on_f_middle"},
	{5, "mantle_on_f_high"},
	{6, "mantle_on_f_extreme"},
	{7, "mantle_on_b_low"},
	{8, "mantle_on_b_middle"},
	{9, "mantle_on_b_high"},
	{10, "mantle_on_b_extreme"},
	{11, "mantle_on_l_low"},
	{12, "mantle_on_l_middle"},
	{13, "mantle_on_l_high"},
	{14, "mantle_on_l_extreme"},
	{15, "mantle_on_r_low"},
	{16, "mantle_on_r_middle"},
	{17, "mantle_on_r_high"},
	{18, "mantle_on_r_extreme"},
	{19, "mantle_over_f_low"},
	{20, "mantle_over_f_middle"},
	{21, "mantle_over_f_high"},
	{22, "mantle_over_f_extreme"},
	{23, "mantle_over_b_low"},
	{24, "mantle_over_b_middle"},
	{25, "mantle_over_b_high"},
	{26, "mantle_over_b_extreme"},
	{27, "mantle_over_l_low"},
	{28, "mantle_over_l_middle"},
	{29, "mantle_over_l_high"},
	{30, "mantle_over_l_extreme"},
	{31, "mantle_over_r_low"},
	{32, "mantle_over_r_middle"},
	{33, "mantle_over_r_high"},
	{34, "mantle_over_r_extreme"},
	{35, "mantle_ladder_low"},
	{36, "mantle_ladder_middle"},
	{37, "mantle_ladder_high"},
	{38, "wallrun_left"},
	{39, "wallrun_right"},
	{40, "wallrun_left_akimbo"},
	{41, "wallrun_right_akimbo"},
	{42, "mount_reach"},
	{43, "mount_top"},
	{44, "mount_left"},
	{45, "mount_right"},
	{46, "ground_pound"},
	{47, "nvg_on"},
	{48, "nvg_off"},
	{49, "suit_dlcgesture0"},
	{50, "suit_dlcgesture1"},
	{51, "suit_dlcgesture2"},
	{52, "suit_dlcgesture3"},
	{53, "suit_dlcgesture4"},
	{54, "num"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 3> carry_object_type_names{{
	{0, "generic"},
	{1, "human"},
	{2, "count"},
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

struct animpkg;
struct weapon_anim_package_state_timers;
struct footstep_anim;
struct footstep_time;
struct movement_time;
struct sfxpkg;
struct weapon_sfx_package_sounds;
struct snd_alias_lookup;
struct trigger_disconnect_sound_data;
struct vfxpkg;
struct weapon_vfx_package_defs;
struct fx_combined_def;
struct tracer;
struct laser;
struct camera;
struct camera_first_person_profile_def;
struct camera_orbit_profile_def;
struct camera_sentry_drone_def;
struct camera_kill_cam_vehicle_def;
struct camera_transition_def;
struct hudoutline;
struct footstepvfx;
struct xcompositemodel;
struct x_composite_submodel;
struct x_composite_sub_composite_model;
struct xanimcurve;
struct x_anim_curve_control_point;
struct keyvaluepairs;
struct key_value_pair;
struct blendspace2d;
struct blend_space2_d_anim_entry;
struct camo;
struct material_texture_def;
struct accessory;
struct locdmgtable;
struct bulletpenetration;
struct cinematicmotion;
struct cinematic_shake_def;
struct cinematic_shake_modifier_def;
struct cinematic_impact_shake_def;
struct decalvolumemask;
struct gesture;
struct gesture_weapon_settings;
struct gesture_directional_settings;
struct gesture_look_around_settings;
struct gesture_ik_target_settings;
struct reticle;
struct streaminginfo;
struct alwaysloaded_flag_set;
struct alwaysloaded_flags;
struct alwaysloaded_flags_per_model;
struct transient_info;
struct transient_sets;
struct bitarray_32;
struct transient_costs;
struct streaming_content_set;
struct streaming_item_cost;
struct forced_stub_asset_reference;
struct transient_file_reference;
struct transient_asset_reference;
struct transient_world_grid_tile_map;
struct spatial_partition_tree;
struct bounds;
struct bitarray_dynamic;
struct suit;
struct suitanimpackage;
struct suit_anim_per_weap_class;
struct suit_anim_entry;
struct suit_gesture_entry;
struct coverselector;
struct enemyselector;
struct carryobject;
struct execution;
struct clientcharacter;
struct physicslibrary;
struct ttf;

inline void validate(const animpkg& data);
inline void validate(const weapon_anim_package_state_timers& data);
inline void validate(const footstep_anim& data);
inline void validate(const footstep_time& data);
inline void validate(const movement_time& data);
inline void validate(const sfxpkg& data);
inline void validate(const weapon_sfx_package_sounds& data);
inline void validate(const snd_alias_lookup& data);
inline void validate(const trigger_disconnect_sound_data& data);
inline void validate(const vfxpkg& data);
inline void validate(const weapon_vfx_package_defs& data);
inline void validate(const fx_combined_def& data);
inline void validate(const tracer& data);
inline void validate(const laser& data);
inline void validate(const camera& data);
inline void validate(const camera_first_person_profile_def& data);
inline void validate(const camera_orbit_profile_def& data);
inline void validate(const camera_sentry_drone_def& data);
inline void validate(const camera_kill_cam_vehicle_def& data);
inline void validate(const camera_transition_def& data);
inline void validate(const hudoutline& data);
inline void validate(const footstepvfx& data);
inline void validate(const xcompositemodel& data);
inline void validate(const x_composite_submodel& data);
inline void validate(const x_composite_sub_composite_model& data);
inline void validate(const xanimcurve& data);
inline void validate(const x_anim_curve_control_point& data);
inline void validate(const keyvaluepairs& data);
inline void validate(const key_value_pair& data);
inline void validate(const blendspace2d& data);
inline void validate(const blend_space2_d_anim_entry& data);
inline void validate(const camo& data);
inline void validate(const material_texture_def& data);
inline void validate(const accessory& data);
inline void validate(const locdmgtable& data);
inline void validate(const bulletpenetration& data);
inline void validate(const cinematicmotion& data);
inline void validate(const cinematic_shake_def& data);
inline void validate(const cinematic_shake_modifier_def& data);
inline void validate(const cinematic_impact_shake_def& data);
inline void validate(const decalvolumemask& data);
inline void validate(const gesture& data);
inline void validate(const gesture_weapon_settings& data);
inline void validate(const gesture_directional_settings& data);
inline void validate(const gesture_look_around_settings& data);
inline void validate(const gesture_ik_target_settings& data);
inline void validate(const reticle& data);
inline void validate(const streaminginfo& data);
inline void validate(const alwaysloaded_flag_set& data);
inline void validate(const alwaysloaded_flags& data);
inline void validate(const alwaysloaded_flags_per_model& data);
inline void validate(const transient_info& data);
inline void validate(const transient_sets& data);
inline void validate(const bitarray_32& data);
inline void validate(const transient_costs& data);
inline void validate(const streaming_content_set& data);
inline void validate(const streaming_item_cost& data);
inline void validate(const forced_stub_asset_reference& data);
inline void validate(const transient_file_reference& data);
inline void validate(const transient_asset_reference& data);
inline void validate(const transient_world_grid_tile_map& data);
inline void validate(const spatial_partition_tree& data);
inline void validate(const bounds& data);
inline void validate(const bitarray_dynamic& data);
inline void validate(const suit& data);
inline void validate(const suitanimpackage& data);
inline void validate(const suit_anim_per_weap_class& data);
inline void validate(const suit_anim_entry& data);
inline void validate(const suit_gesture_entry& data);
inline void validate(const coverselector& data);
inline void validate(const enemyselector& data);
inline void validate(const carryobject& data);
inline void validate(const execution& data);
inline void validate(const clientcharacter& data);
inline void validate(const physicslibrary& data);
inline void validate(const ttf& data);

struct weapon_anim_package_state_timers
{
	std::uint16_t i_detonate_delay{};
	std::uint16_t i_first_raise_time{};
	std::uint16_t i_alt_raise_time{};
	std::uint16_t i_alt_raise_ads_time{};
	std::uint16_t i_alt_raise_akimbo_time{};
	std::uint16_t i_rechamber_time{};
	std::uint16_t rechamber_time_one_handed{};
	std::uint16_t i_rechamber_bolt_time{};
	std::uint16_t i_hold_fire_time{};
	std::uint16_t i_hold_prime_time{};
	std::uint16_t i_detonate_time{};
	std::uint16_t i_melee_time{};
	std::uint16_t melee_charge_time{};
	std::uint16_t melee_charge_view_and_movement_lock_time{};
	std::uint16_t melee_slice_damage_time{};
	std::uint16_t melee_slice_interrupt_time{};
	std::uint16_t melee_slice_view_and_movement_lock_time{};
	std::uint16_t melee_re_slice_interrupt_time{};
	std::uint16_t melee_slice_standard_interrupt_time{};
	std::uint16_t melee_charge_damage_time{};
	std::uint16_t melee_charge_interrupt_time{};
	std::uint16_t melee_re_charge_interrupt_time{};
	std::uint16_t melee_charge_standard_interrupt_time{};
	std::uint16_t alt_melee_time{};
	std::uint16_t alt_melee_charge_time{};
	std::uint16_t alt_melee_charge_view_and_movement_lock_time{};
	std::uint16_t alt_melee_slice_damage_time{};
	std::uint16_t alt_melee_slice_interrupt_time{};
	std::uint16_t alt_melee_slice_view_and_movement_lock_time{};
	std::uint16_t alt_melee_re_slice_interrupt_time{};
	std::uint16_t alt_melee_slice_standard_interrupt_time{};
	std::uint16_t alt_melee_charge_damage_time{};
	std::uint16_t alt_melee_charge_interrupt_time{};
	std::uint16_t alt_melee_re_charge_interrupt_time{};
	std::uint16_t alt_melee_charge_standard_interrupt_time{};
	std::uint16_t i_reload_time{};
	std::uint16_t i_fast_reload_time{};
	std::uint16_t reload_show_rocket_time{};
	std::uint16_t i_reload_empty_time{};
	std::uint16_t i_fast_reload_empty_time{};
	std::uint16_t i_reload_add_time{};
	std::uint16_t i_fast_reload_add_time{};
	std::uint16_t i_reload_add_second_time{};
	std::uint16_t i_fast_reload_add_second_time{};
	std::uint16_t i_reload_empty_add_time{};
	std::uint16_t i_fast_reload_empty_add_time{};
	std::uint16_t i_alt_override3_p_in_time{};
	std::uint16_t i_alt_override3_p_out_time{};
	std::uint16_t i_reload_start_time{};
	std::uint16_t i_fast_reload_start_time{};
	std::uint16_t i_reload_start_add_time{};
	std::uint16_t i_fast_reload_start_add_time{};
	std::uint16_t i_reload_start_add_second_time{};
	std::uint16_t i_fast_reload_start_add_second_time{};
	std::uint16_t i_reload_end_time{};
	std::uint16_t i_reload_end_interrupted_time{};
	std::uint16_t i_reload_end_rechamber_time{};
	std::uint16_t i_reload_end_rechamber_shell_eject_time{};
	std::uint16_t i_reload_end_rechamber_interrupted_time{};
	std::uint16_t i_reload_end_rechamber_interrupted_shell_eject_time{};
	std::uint16_t i_fast_reload_end_time{};
	std::uint16_t i_fast_reload_end_interrupted_time{};
	std::uint16_t i_fast_reload_end_rechamber_time{};
	std::uint16_t i_fast_reload_end_rechamber_shell_eject_time{};
	std::uint16_t i_fast_reload_end_rechamber_interrupted_time{};
	std::uint16_t i_fast_reload_end_rechamber_interrupted_shell_eject_time{};
	std::uint16_t i_drop_time{};
	std::uint16_t i_raise_time{};
	std::uint16_t i_alt_drop_time{};
	std::uint16_t i_alt_drop_ads_time{};
	std::uint16_t i_alt_drop_akimbo_time{};
	std::uint16_t quick_drop_time{};
	std::uint16_t quick_raise_time{};
	std::uint16_t i_breach_raise_time{};
	std::uint16_t i_empty_raise_time{};
	std::uint16_t i_empty_drop_time{};
	std::uint16_t i_ladder_vault_time{};
	std::uint16_t i_ladder_climb_raise_time{};
	std::uint16_t i_ladder_climb_drop_time{};
	std::uint16_t i_ladder_aim_raise_time{};
	std::uint16_t i_ladder_aim_raise_interrupt_time{};
	std::uint16_t i_ladder_aim_ads_raise_time{};
	std::uint16_t i_ladder_aim_ads_raise_interrupt_time{};
	std::uint16_t i_ladder_aim_drop_time{};
	std::uint16_t i_ladder_aim_drop_interrupt_time{};
	std::uint16_t sprint_in_time{};
	std::uint16_t sprint_loop_time{};
	std::uint16_t sprint_out_time{};
	std::uint16_t walk_time{};
	std::uint16_t gung_ho_time{};
	std::uint16_t crawl_time{};
	std::uint16_t stunned_time_begin{};
	std::uint16_t stunned_time_loop{};
	std::uint16_t stunned_time_end{};
	std::uint16_t prime_time{};
	bool b_hold_full_prime{};
	bool b_alt_override3_pads_only{};
	std::uint16_t blast_front_time{};
	std::uint16_t blast_right_time{};
	std::uint16_t blast_back_time{};
	std::uint16_t blast_left_time{};
	std::uint16_t slide_in_time{};
	std::uint16_t slide_loop_time{};
	std::uint16_t slide_out_time{};
	std::uint16_t dodge_time{};
	std::uint16_t wall_run_in_time{};
	std::uint16_t wall_run_out_time{};
	std::uint16_t leap_in_time{};
	std::uint16_t leap_loop_time{};
	std::uint16_t leap_out_time{};
	std::uint16_t leap_cancel_time{};
	std::uint16_t charge_in_time{};
	std::uint16_t charge_loop_time{};
	std::uint16_t charge_out_time{};
	std::uint16_t i_regeneration_time{};
	std::uint16_t i_regeneration_add_time{};
	std::uint16_t pre_fire_time{};
	std::uint16_t i_cycle_walk{};
	std::uint16_t i_cycle_jog{};
	std::uint16_t i_cycle_sprint{};
	std::uint16_t i_cycle_super_sprint{};
	std::uint16_t i_transition_to_walk{};
	std::uint16_t i_transition_to_jog{};
	std::uint16_t i_transition_to_sprint{};
	std::uint16_t i_transition_to_super_sprint{};
	std::uint16_t i_transition_down_from_sprint{};
	std::uint16_t i_transition_down_from_super_sprint{};
	std::uint16_t skydive_freefall_raise_time{};
	std::uint16_t scope_toggle_on_time{};
	std::uint16_t scope_toggle_off_time{};
	std::uint16_t i_reload_interrupt_time{};
	std::uint16_t i_reload_empty_interrupt_time{};
	std::uint16_t i_reload_start_interrupt_time{};
	std::uint16_t i_reload_end_interrupt_time{};
	std::uint16_t i_reload_end_interrupted_interrupt_time{};
	std::uint16_t i_reload_end_rechamber_interrupt_time{};
	std::uint16_t i_reload_end_rechamber_interrupted_interrupt_time{};
	std::uint16_t i_reload_multiple1_interrupt_time{};
	std::uint16_t i_reload_multiple2_interrupt_time{};
	std::uint16_t i_reload_multiple3_interrupt_time{};
	std::uint16_t i_reload_multiple4_interrupt_time{};
	std::uint16_t i_reload_multiple5_interrupt_time{};
	std::uint16_t i_reload_multiple6_interrupt_time{};
	std::uint16_t i_reload_multiple7_interrupt_time{};
	std::uint16_t i_reload_multiple8_interrupt_time{};
	std::uint16_t i_fast_reload_interrupt_time{};
	std::uint16_t i_fast_reload_empty_interrupt_time{};
	std::uint16_t i_fast_reload_start_interrupt_time{};
	std::uint16_t i_fast_reload_end_interrupt_time{};
	std::uint16_t i_fast_reload_end_interrupted_interrupt_time{};
	std::uint16_t i_fast_reload_end_rechamber_interrupt_time{};
	std::uint16_t i_fast_reload_end_rechamber_interrupted_interrupt_time{};
	std::uint16_t i_fast_reload_multiple1_interrupt_time{};
	std::uint16_t i_fast_reload_multiple2_interrupt_time{};
	std::uint16_t i_fast_reload_multiple3_interrupt_time{};
	std::uint16_t i_fast_reload_multiple4_interrupt_time{};
	std::uint16_t i_fast_reload_multiple5_interrupt_time{};
	std::uint16_t i_fast_reload_multiple6_interrupt_time{};
	std::uint16_t i_fast_reload_multiple7_interrupt_time{};
	std::uint16_t i_fast_reload_multiple8_interrupt_time{};
	std::uint16_t i_raise_start_disable_interrupt_time{};
	std::uint16_t i_raise_interrupt_time{};
	std::uint16_t i_first_raise_start_disable_interrupt_time{};
	std::uint16_t i_first_raise_interrupt_time{};
	std::uint16_t i_quick_raise_start_disable_interrupt_time{};
	std::uint16_t i_quick_raise_interrupt_time{};
	std::uint16_t i_empty_raise_start_disable_interrupt_time{};
	std::uint16_t i_empty_raise_interrupt_time{};
	std::uint16_t i_alt_raise_interrupt_time{};
	std::uint16_t i_alt_raise_ads_interrupt_time{};
	std::uint16_t i_alt_raise_akimbo_interrupt_time{};
	std::uint16_t i_sprint_out_interrupt_time{};
	std::uint16_t hold_fire_interrupt_time{};
	std::uint16_t max_blend_to_fire{};
	std::uint16_t min_blend_to_fire{};
	std::uint16_t max_blend_alt_raise_to_fire{};
	std::uint16_t min_blend_alt_raise_to_fire{};
	std::uint16_t i_transition_down_from_sprint_interrupt{};
	std::uint16_t i_transition_down_from_super_sprint_interrupt{};
	std::uint16_t scope_toggle_on_interrupt_time{};
	std::uint16_t scope_toggle_off_interrupt_time{};
	std::uint16_t inspect_time{};
	std::uint16_t i_dlc_anim0_time{};
	std::uint16_t i_dlc_anim1_time{};
	std::uint16_t i_dlc_anim2_time{};
	std::uint16_t i_dlc_anim3_time{};

	bool operator==(const weapon_anim_package_state_timers&) const = default;
};

struct footstep_time
{
	float time{};
	bool is_left{};

	bool operator==(const footstep_time&) const = default;
};

struct movement_time
{
	float time{};
	bool is_lead_in{};

	bool operator==(const movement_time&) const = default;
};

struct footstep_anim
{
	std::int32_t left_count{};
	std::array<footstep_time, 32> step{};
	std::array<movement_time, 32> movement{};

	bool operator==(const footstep_anim&) const = default;
};

struct animpkg
{
	std::optional<std::string> name{};
	std::optional<std::array<std::optional<std::string>, 614>> anims{};
	std::optional<weapon_anim_package_state_timers> timers{};
	std::uint32_t melee_anim_primary_type{};
	std::int32_t melee_anim_primary_count{};
	std::uint8_t melee_anim_primary_set{};
	bool melee_randomize_anims{};
	std::uint32_t melee_anim_alt_type{};
	std::int32_t melee_anim_alt_count{};
	std::uint8_t melee_anim_alt_set{};
	bool alt_melee_randomize_anims{};
	std::optional<std::array<std::optional<std::string>, 8>> blend_spaces{};
	std::optional<std::array<footstep_anim, 13>> footstep{};

	bool operator==(const animpkg&) const = default;
};

struct snd_alias_lookup
{
	std::optional<std::string> name{};

	bool operator==(const snd_alias_lookup&) const = default;
};

struct weapon_sfx_package_sounds
{
	std::optional<std::string> name{};
	snd_alias_lookup pickup_sound{};
	snd_alias_lookup pickup_sound_player{};
	snd_alias_lookup ammo_pickup_sound{};
	snd_alias_lookup ammo_pickup_sound_player{};
	snd_alias_lookup projectile_sound{};
	snd_alias_lookup pullback_sound{};
	snd_alias_lookup pullback_sound_player{};
	snd_alias_lookup pullback_sound_quick{};
	snd_alias_lookup pullback_sound_quick_player{};
	snd_alias_lookup pre_fire_sound{};
	snd_alias_lookup pre_fire_sound_player{};
	snd_alias_lookup fire_sound{};
	snd_alias_lookup fire_sound_player{};
	snd_alias_lookup fire_sound_player_akimbo{};
	snd_alias_lookup fire_sound_player_akimbo_right{};
	snd_alias_lookup fire_sound_atmosphere{};
	snd_alias_lookup fire_sound_atmosphere_player{};
	snd_alias_lookup fire_sound_atmosphere_player_akimbo{};
	snd_alias_lookup fire_sound_atmosphere_player_akimbo_right{};
	snd_alias_lookup fire_start_sound{};
	snd_alias_lookup fire_start_sound_player{};
	snd_alias_lookup fire_loop_sound{};
	snd_alias_lookup fire_loop_sound_player{};
	snd_alias_lookup fire_stop_sound{};
	snd_alias_lookup fire_stop_sound_player{};
	snd_alias_lookup fire_first_sound{};
	snd_alias_lookup fire_first_sound_player{};
	snd_alias_lookup fire_first_sound_player_akimbo{};
	snd_alias_lookup fire_first_sound_player_akimbo_right{};
	snd_alias_lookup fire_hyper_burst_sound{};
	snd_alias_lookup fire_hyper_burst_sound_player{};
	snd_alias_lookup fire_hyper_burst_sound_player_akimbo{};
	snd_alias_lookup fire_hyper_burst_sound_player_akimbo_right{};
	snd_alias_lookup fire_last_sound{};
	snd_alias_lookup fire_last_sound_player{};
	snd_alias_lookup fire_last_sound_player_akimbo{};
	snd_alias_lookup fire_last_sound_player_akimbo_right{};
	snd_alias_lookup regenerate_sound{};
	snd_alias_lookup regenerate_sound_player{};
	snd_alias_lookup regenerate_add_sound{};
	snd_alias_lookup regenerate_add_sound_player{};
	snd_alias_lookup empty_fire_sound{};
	snd_alias_lookup empty_fire_sound_player{};
	snd_alias_lookup empty_regen_fire_sound{};
	snd_alias_lookup empty_regen_fire_sound_player{};
	snd_alias_lookup rechamber_sound{};
	snd_alias_lookup rechamber_sound_player{};
	snd_alias_lookup reload_sound{};
	snd_alias_lookup reload_sound_player{};
	snd_alias_lookup reload_empty_sound{};
	snd_alias_lookup reload_empty_sound_player{};
	snd_alias_lookup reload_start_sound{};
	snd_alias_lookup reload_start_sound_player{};
	snd_alias_lookup reload_end_sound{};
	snd_alias_lookup reload_end_sound_player{};
	snd_alias_lookup reload_end_rechamber_sound{};
	snd_alias_lookup reload_end_rechamber_sound_player{};
	snd_alias_lookup detonate_sound{};
	snd_alias_lookup detonate_sound_player{};
	snd_alias_lookup night_vision_wear_sound{};
	snd_alias_lookup night_vision_wear_sound_player{};
	snd_alias_lookup night_vision_remove_sound{};
	snd_alias_lookup night_vision_remove_sound_player{};
	snd_alias_lookup alt_switch_sound{};
	snd_alias_lookup alt_switch_sound_player{};
	snd_alias_lookup raise_sound{};
	snd_alias_lookup raise_sound_player{};
	snd_alias_lookup first_raise_sound{};
	snd_alias_lookup first_raise_sound_player{};
	snd_alias_lookup putaway_sound{};
	snd_alias_lookup putaway_sound_player{};
	snd_alias_lookup enter_ads_sound{};
	snd_alias_lookup enter_ads_sound_player{};
	snd_alias_lookup leave_ads_sound{};
	snd_alias_lookup leave_ads_sound_player{};
	snd_alias_lookup scan_sound{};
	snd_alias_lookup change_variable_zoom_sound{};
	snd_alias_lookup bullet_ricochet_impact{};
	snd_alias_lookup proj_explosion_sound{};
	snd_alias_lookup proj_dud_sound{};
	snd_alias_lookup proj_timeout_sound{};
	snd_alias_lookup proj_ignition_sound{};
	snd_alias_lookup bounce_sound{};
	snd_alias_lookup bounce_sound_heavy{};
	snd_alias_lookup bounce_sound_medium{};
	snd_alias_lookup bounce_sound_soft{};
	snd_alias_lookup rolling_sound{};
	snd_alias_lookup turret_spin_snd{};
	snd_alias_lookup turret_stop_snd{};
	snd_alias_lookup trigger_disconnect_sound{};
	snd_alias_lookup trigger_disconnect_sound_player_akimbo{};
	snd_alias_lookup trigger_disconnect_sound_player_akimbo_right{};
	snd_alias_lookup dlc_sound0{};
	snd_alias_lookup dlc_sound0_player{};
	snd_alias_lookup dlc_sound1{};
	snd_alias_lookup dlc_sound1_player{};

	bool operator==(const weapon_sfx_package_sounds&) const = default;
};

struct trigger_disconnect_sound_data
{
	float analog_trigger_value{};
	std::array<bool, 6> enabled_for_fire_type{};

	bool operator==(const trigger_disconnect_sound_data&) const = default;
};

struct sfxpkg
{
	std::optional<std::string> name{};
	std::optional<weapon_sfx_package_sounds> sounds{};
	std::optional<std::string> transient_fallback_package{};
	std::optional<std::string> detail_sound_bank_npc{};
	std::optional<std::string> detail_sound_bank_player{};
	std::uint8_t rattle_sound_type{};
	std::optional<std::string> sz_adsr_base_setting{};
	std::optional<std::string> sz_weap_snd_reflection_class{};
	std::optional<std::string> sz_weap_snd_refl_front_override{};
	std::optional<std::string> sz_weap_snd_refl_distant_override{};
	float weap_snd_fire_volume_shot1{};
	float weap_snd_fire_volume_shot2{};
	float weap_snd_fire_volume_shot3{};
	float weap_snd_prone_fire_lfe_volume{};
	float weap_snd_medium_range_start{};
	float weap_snd_far_range_start{};
	float weap_snd_distant_range_start{};
	std::uint32_t proj_explosion_refl_class{};
	std::int32_t sfx_impact_type{};
	std::string sfx_melee_impact_type{};
	std::uint32_t sfx_material_type{};
	std::optional<std::string> sz_mount_enter_exit_string{};
	std::optional<std::string> sz_mount_impact_string{};
	std::uint32_t whizby_type{};
	std::uint32_t ads_context_type{};
	std::uint32_t ads_context_value{};
	float speed_of_sound_mult{};
	trigger_disconnect_sound_data trigger_disconnect_sound_data{};

	bool operator==(const sfxpkg&) const = default;
};

struct fx_combined_def
{
	std::optional<std::string> particle_system_def{};

	bool operator==(const fx_combined_def&) const = default;
};

struct weapon_vfx_package_defs
{
	fx_combined_def view_flash_effect{};
	fx_combined_def world_flash_effect{};
	fx_combined_def view_flash_ads_effect{};
	fx_combined_def view_flash_first_effect{};
	fx_combined_def world_flash_first_effect{};
	fx_combined_def view_flash_first_ads_effect{};
	fx_combined_def view_flash_overcharge_effect{};
	fx_combined_def view_flash_overcharge_ads_effect{};
	fx_combined_def world_flash_overcharge_effect{};
	fx_combined_def view_shell_eject_effect{};
	fx_combined_def world_shell_eject_effect{};
	fx_combined_def view_last_shot_eject_effect{};
	fx_combined_def world_last_shot_eject_effect{};
	fx_combined_def view_mag_eject_effect{};
	fx_combined_def view_muzzle_smoke_effect{};
	fx_combined_def world_muzzle_smoke_effect{};
	fx_combined_def view_barrel_smoke_effect{};
	fx_combined_def world_barrel_smoke_effect{};
	fx_combined_def view_ejection_port_smoke_effect{};
	fx_combined_def world_ejection_port_smoke_effect{};
	fx_combined_def view_dlc_effect0{};
	fx_combined_def world_dlc_effect0{};
	fx_combined_def view_dlc_effect1{};
	fx_combined_def world_dlc_effect1{};
	fx_combined_def view_dlc_effect2{};
	fx_combined_def world_dlc_effect2{};
	fx_combined_def view_dlc_effect3{};
	fx_combined_def world_dlc_effect3{};

	bool operator==(const weapon_vfx_package_defs&) const = default;
};

struct vfxpkg
{
	std::optional<std::string> name{};
	std::optional<weapon_vfx_package_defs> vfx{};

	bool operator==(const vfxpkg&) const = default;
};

struct tracer
{
	std::optional<std::string> name{};
	fx_combined_def effect{};
	fx_combined_def viewmodel_effect{};
	std::uint32_t draw_interval{};
	float speed{};
	bool draw_legacy_tracer{};
	bool fade_over_time{};
	float fade_time{};
	std::optional<std::string> material{};
	float beam_length{};
	float beam_width{};
	float screw_radius{};
	float screw_dist{};
	std::array<std::array<float, 4>, 5> colors{};

	bool operator==(const tracer&) const = default;
};

struct laser
{
	std::optional<std::string> name{};
	std::optional<std::string> laser_material{};
	std::optional<std::string> laser_light_material{};
	bool owner_only{};
	bool nightvision_only{};
	float range{};
	float radius{};
	float end_offset_viewmodel{};
	float end_offset_other{};
	float flare_pct{};
	fx_combined_def beam_effect{};
	fx_combined_def laser_end_effect{};
	bool client_collision{};
	std::array<float, 4> color{};
	std::array<float, 4> hdr_color_scale{};
	bool laser_light{};
	bool laser_light_nvg_only{};
	float laser_light_radius{};
	float laser_light_begin_offset{};
	float laser_light_end_offset{};
	float laser_light_body_tweak{};
	std::array<float, 4> laser_light_color{};
	std::array<float, 4> laser_light_hdr_color_scale{};
	float range_alt{};
	float radius_alt{};
	float laser_light_radius_alt{};
	float flare_pct_alt{};

	bool operator==(const laser&) const = default;
};

struct camera_first_person_profile_def
{
	bool apply_offset{};
	std::array<float, 3> offset_translation{};
	bool disable_fov_weapon_offset{};
	float camera_animation_trans_scale{};
	float camera_animation_rot_scale{};
	float handheld_camera_trans_scale{};
	float handheld_camera_rot_scale{};
	float handheld_camera_impulse_trans_scale{};
	float handheld_camera_impulse_rot_scale{};

	bool operator==(const camera_first_person_profile_def&) const = default;
};

struct camera_orbit_profile_def
{
	float lerp_speed{};
	std::array<float, 3> pivot_offset{};
	float initial_azimuth{};
	float initial_polar{};
	float polar_min{};
	float polar_max{};
	float range{};
	float return_azimuth_speed{};
	float return_polar_speed{};
	float ease_in_factor{};
	float return_time_to{};
	float turning_speed_polar{};
	float turning_speed_azimuth{};
	float trace_coll_radius{};
	float mouse_input_multiplier{};
	float roll_contrib{};
	float pitch_contrib{};
	std::int32_t button_toggle_return{};
	std::int32_t button_cycle_perspective{};
	std::int32_t button_recenter{};
	bool initial_return{};
	bool return_azimuth{};
	bool return_polar{};
	float align_to_movement{};
	bool interpolate_focus{};
	bool camera_return_on_gas{};
	float camera_return_on_speed{};
	float yaw_turn_adjust{};

	bool operator==(const camera_orbit_profile_def&) const = default;
};

struct camera_sentry_drone_def
{
	float range{};
	float thres_hit_to_speed_up{};
	float speed_up_factor{};
	float lerp_speed_eye{};
	float lerp_speed_focus{};
	float lerp_speed_up{};
	float focus_trace_len{};
	float eye_height{};
	float horizontal_offset{};
	float up_offset_factor_incl{};

	bool operator==(const camera_sentry_drone_def&) const = default;
};

struct camera_kill_cam_vehicle_def
{
	float look_at_radius{};
	float fov{};
	float dist_back{};
	float dist_up{};
	float dist_back_radius_mod{};
	float dist_up_radius_mod{};
	float fov_radius_mod{};
	float side_mag{};
	float trace_coll_rad{};

	bool operator==(const camera_kill_cam_vehicle_def&) const = default;
};

struct camera_transition_def
{
	bool active{};
	float time{};
	float timewait_eyepos{};
	float timewait_axis{};
	std::string curve_eyepos{};
	std::string curve_axis{};

	bool operator==(const camera_transition_def&) const = default;
};

struct camera
{
	std::optional<std::string> name{};
	std::string profile{};
	bool keep_shake_cam{};
	bool server_feedback{};
	bool behave_as_remote_cont{};
	bool interpolate_view_transition{};
	bool override_fov{};
	float fov{};
	std::string override_lens_profile{};
	float lens_focal_length{};
	float lens_f_stop{};
	float lens_scale{};
	float lens_uv_scale{};
	bool override_radial_blur{};
	float radial_blur_radius{};
	float radial_blur_strength{};
	bool override_z_planes{};
	float z_plane_vm_near{};
	float z_plane_scene_near{};
	camera_first_person_profile_def first_person{};
	camera_orbit_profile_def orbit{};
	camera_sentry_drone_def sentry_drone{};
	camera_kill_cam_vehicle_def kcam_veh{};
	camera_transition_def transition_in{};
	camera_transition_def transition_out{};
	std::array<float, 3> fpveh_offset_ls{};
	float fpveh_offset_yaw{};
	float fpveh_offset_pitch{};
	float fpveh_roll_contrib{};
	float fpveh_roll_max{};
	bool fpveh_free_look{};
	bool fpveh_free_look_return{};
	std::array<bool, 1> dlc_bool{};
	bool fade_out_player_on_transition_in{};

	bool operator==(const camera&) const = default;
};

struct hudoutline
{
	std::optional<std::string> name{};
	std::uint8_t outline_type{};
	bool draw_in_stencil{};
	bool draw_non_occluded_pixels{};
	bool draw_occluded_pixels{};
	bool draw_fill{};
	bool fade_over_time_enable{};
	bool fade_over_time_looping{};
	std::uint8_t fade_over_time_curve_type{};
	float fade_over_time_min_alpha{};
	float fade_over_time_max_alpha_time{};
	float fade_over_time_min_alpha_time{};
	float fade_over_time_in_time{};
	float fade_over_time_out_time{};
	bool distance_fade_enable{};
	float distance_fade_min_alpha{};
	float distance_fade_start_distance{};
	float distance_fade_end_distance{};
	std::uint32_t outline_color{};
	std::int32_t outline_width{};
	bool draw_on_local_player_character{};
	bool draw_on_vehicle_occupants{};
	bool draw_on_local_player_vehicle_occupants{};
	bool draw_on_local_player_vehicle{};
	bool dlc_bool0{};
	bool dlc_bool1{};
	bool dlc_bool2{};
	bool dlc_bool3{};
	bool dlc_bool4{};
	bool dlc_bool5{};
	float dlc_float0{};
	float dlc_float1{};
	float dlc_float2{};
	float dlc_float3{};
	float dlc_float4{};
	float dlc_float5{};

	bool operator==(const hudoutline&) const = default;
};

struct footstepvfx
{
	std::optional<std::string> name{};
	std::array<fx_combined_def, 64> footstep_vfx{};
	std::array<fx_combined_def, 64> small_footstep_vfx{};

	bool operator==(const footstepvfx&) const = default;
};

struct x_composite_submodel
{
	std::optional<std::string> model_name{};
	std::array<float, 3> translation{};
	std::array<float, 3> angles{};
	std::optional<std::string> parent_bone_name{};
	std::int32_t parent_submodel_slot{};
	std::optional<std::string> model{};

	bool operator==(const x_composite_submodel&) const = default;
};

struct x_composite_sub_composite_model
{
	std::optional<std::string> composite_model_name{};
	std::array<float, 3> translation{};
	std::array<float, 3> angles{};
	std::optional<std::string> parent_bone_name{};
	std::int32_t parent_submodel_slot{};
	std::optional<std::string> composite_model{};

	bool operator==(const x_composite_sub_composite_model&) const = default;
};

struct xcompositemodel
{
	std::optional<std::string> name{};
	std::optional<std::array<x_composite_submodel, 10>> submodels{};
	std::optional<std::array<x_composite_sub_composite_model, 10>> sub_composite_models{};

	bool operator==(const xcompositemodel&) const = default;
};

struct xanimcurve
{
	std::optional<std::string> name{};
	std::optional<std::vector<x_anim_curve_control_point>> control_points{};
	std::string curve_type{};
	float duration{};

	bool operator==(const xanimcurve&) const = default;
};

struct x_anim_curve_control_point
{
	float time{};
	float value{};
	float inv_time_delta{};

	bool operator==(const x_anim_curve_control_point&) const = default;
};

struct keyvaluepairs
{
	std::optional<std::string> name{};
	std::optional<std::vector<key_value_pair>> key_value_pairs{};

	bool operator==(const keyvaluepairs&) const = default;
};

struct key_value_pair
{
	std::uint32_t key_hash{};
	std::optional<std::string> value{};

	bool operator==(const key_value_pair&) const = default;
};

struct blendspace2d
{
	std::optional<std::string> name{};
	std::array<float, 2> range_x{};
	std::array<float, 2> range_y{};
	std::optional<std::vector<blend_space2_d_anim_entry>> anim_entries{};
	std::optional<std::vector<std::uint32_t>> triangle_coords{};
	std::int32_t blend_time_ms{};
	bool all_anims_notify{};

	bool operator==(const blendspace2d&) const = default;
};

struct blend_space2_d_anim_entry
{
	std::optional<std::string> anim{};
	std::array<float, 2> anim_coords{};

	bool operator==(const blend_space2_d_anim_entry&) const = default;
};

struct camo
{
	std::optional<std::string> name{};
	std::optional<std::string> internal_name{};
	std::optional<std::vector<material_texture_def>> texture_defs{};
	std::array<std::uint32_t, 3> color_tint{};
	std::array<std::uint32_t, 3> albedo_map_scale_bias{};
	std::array<std::uint32_t, 3> normal_map_scale_bias{};
	std::array<std::array<float, 4>, 3> uv_scale_offset{};
	std::array<std::array<float, 4>, 3> uv_rotate{};
	std::uint8_t blend_map_channels{};

	bool operator==(const camo&) const = default;
};

struct material_texture_def
{
	std::uint8_t index{};
	std::optional<std::string> image{};

	bool operator==(const material_texture_def&) const = default;
};

struct accessory
{
	std::optional<std::string> name{};
	std::optional<std::string> vm_attach_bone{};
	bool vm_attach_right_hand{};
	std::optional<std::string> interact_gesture{};
	std::optional<std::string> widget_vm{};
	std::int32_t widget_vmrtt_width{};
	std::int32_t widget_vmrtt_height{};
	std::int32_t widget_vmrtt_mip_count{};

	bool operator==(const accessory&) const = default;
};

struct locdmgtable
{
	std::optional<std::string> name{};
	std::array<float, 22> mults{};

	bool operator==(const locdmgtable&) const = default;
};

struct bulletpenetration
{
	std::optional<std::string> name{};
	std::array<std::array<float, 64>, 4> depth_table{};

	bool operator==(const bulletpenetration&) const = default;
};

struct cinematic_shake_modifier_def
{
	std::uint32_t type{};
	std::int32_t in_blend_in_time_ms{};
	std::int32_t in_blend_out_time_ms{};
	std::array<float, 2> in_speed_range{};
	std::array<float, 2> out_amplitude_scale{};
	std::array<float, 2> out_persistence_scale{};
	std::array<float, 2> out_speed_scale{};
	std::int32_t out_blend_in_time_ms{};
	std::int32_t out_blend_out_time_ms{};
	std::optional<std::string> non_linear_map{};

	bool operator==(const cinematic_shake_modifier_def&) const = default;
};

struct cinematic_shake_def
{
	bool enabled{};
	std::array<float, 3> frequency{};
	std::array<float, 3> amplitude{};
	std::array<float, 3> persistence{};
	cinematic_shake_modifier_def move_speed_modifiers{};
	cinematic_shake_modifier_def view_speed_modifiers{};
	std::uint32_t modifier_combine_type{};

	bool operator==(const cinematic_shake_def&) const = default;
};

struct cinematic_impact_shake_def
{
	std::array<float, 3> frequency{};
	std::array<float, 3> amplitude{};
	std::array<float, 3> persistence{};

	bool operator==(const cinematic_impact_shake_def&) const = default;
};

struct cinematicmotion
{
	std::optional<std::string> name{};
	std::array<float, 3> camera_spring_velocity_smoothing_sec{};
	std::array<float, 3> camera_spring_velocity_scale{};
	std::array<float, 3> camera_spring_spring_constant{};
	std::array<float, 3> camera_spring_damper_constant{};
	std::array<float, 3> camera_spring_max_offset{};
	std::array<float, 3> viewmodel_spring_velocity_smoothing_sec{};
	std::array<float, 3> viewmodel_spring_velocity_scale{};
	std::array<float, 3> viewmodel_spring_spring_constant{};
	std::array<float, 3> viewmodel_spring_damper_constant{};
	std::array<float, 3> viewmodel_spring_max_offset{};
	std::array<float, 3> viewmodel_spring_offset_to_angles_x{};
	std::array<float, 3> viewmodel_spring_offset_to_angles_y{};
	std::array<float, 3> viewmodel_spring_offset_to_angles_z{};
	cinematic_shake_def camera_shake_rotation{};
	cinematic_shake_def camera_shake_translation{};
	cinematic_impact_shake_def camera_shake_impact_rotation{};
	cinematic_impact_shake_def camera_shake_impact_translation{};

	bool operator==(const cinematicmotion&) const = default;
};

struct decalvolumemask
{
	std::optional<std::string> name{};
	std::optional<std::string> blend_map{};

	bool operator==(const decalvolumemask&) const = default;
};

struct gesture_weapon_settings
{
	std::uint16_t blend_to_states{};
	bool hide_reticle{};
	float fire_delay{};
	float sprint_delay{};
	bool use_left_idle_akimbo{};
	bool split_anims_akimbo{};
	bool blend_to_demeanor_loop{};
	bool blend_out_raise{};
	bool blend_out_finger_pose{};
	bool blend_out_additive_ads{};

	bool operator==(const gesture_weapon_settings&) const = default;
};

struct gesture_directional_settings
{
	float max_angle{};
	float lerp_at_max_angle{};
	float width_cushion_angle{};
	float lerp_at_min_cushion_angle{};
	float lerp_at_max_cushion_angle{};
	float limit_left{};
	float limit_right{};
	float limit_up{};
	float limit_down{};
	bool use_target_offset{};
	float target_offset_x{};
	float target_offset_y{};
	float target_offset_z{};
	float target_offset_yaw{};
	float target_offset_pitch{};
	float target_offset_roll{};
	bool ignore_view_pitch_for_target_offset{};
	bool ignore_view_yaw_for_target_offset{};
	bool ignore_view_roll_for_target_offset{};

	bool operator==(const gesture_directional_settings&) const = default;
};

struct gesture_look_around_settings
{
	float yaw_lerp_in{};
	float yaw_lerp_out{};
	std::uint16_t walk_time{};
	footstep_anim walk_foot_step_anim{};

	bool operator==(const gesture_look_around_settings&) const = default;
};

struct gesture_ik_target_settings
{
	std::optional<std::string> target_entity_bone_name{};

	bool operator==(const gesture_ik_target_settings&) const = default;
};

struct gesture
{
	std::optional<std::string> name{};
	std::string type{};
	std::string priority{};
	bool looping{};
	std::optional<std::array<std::optional<std::string>, 22>> anims{};
	gesture_weapon_settings weapon_settings{};
	gesture_directional_settings directional_settings{};
	gesture_look_around_settings look_around_settings{};
	gesture_ik_target_settings ik_target_settings{};

	bool operator==(const gesture&) const = default;
};

struct reticle
{
	std::optional<std::string> name{};
	std::optional<std::string> widget_name{};
	bool lock_reticle_to_tag{};
	bool primary_affected_by_emp{};
	bool secondary_affected_by_emp{};
	float offset_from_tag{};
	bool scale_with_fov{};
	float primary_default_scale{};
	float secondary_default_scale{};
	std::optional<std::string> secondary_widget_name{};
	bool secondary_lock_reticle_to_tag{};
	float secondary_offset_from_tag{};
	float natural_distance{};
	std::int32_t natural_fov{};
	float min_reticle_scale{};
	float max_reticle_scale{};

	bool operator==(const reticle&) const = default;
};

struct alwaysloaded_flags
{
	std::optional<std::vector<std::optional<std::string>>> images{};
	std::optional<std::vector<std::uint32_t>> image_flags{};

	bool operator==(const alwaysloaded_flags&) const = default;
};

struct alwaysloaded_flag_set
{
	alwaysloaded_flags global_flags{};
	std::optional<std::vector<alwaysloaded_flags_per_model>> per_model_flags{};
	std::optional<std::vector<std::optional<std::string>>> model_surfs{};

	bool operator==(const alwaysloaded_flag_set&) const = default;
};

struct transient_sets
{
	std::uint32_t count{};
	std::optional<std::vector<std::optional<std::string>>> names{};
	std::optional<std::vector<bitarray_32>> masks{};

	bool operator==(const transient_sets&) const = default;
};

struct streaming_content_set
{

	bool operator==(const streaming_content_set&) const = default;
};

struct transient_costs
{
	streaming_content_set stream_contents{};
	std::optional<std::vector<streaming_item_cost>> streaming_costs{};
	std::optional<std::vector<std::uint16_t>> resident_page_sizes{};
	std::optional<std::vector<std::uint16_t>> temp_page_sizes{};

	bool operator==(const transient_costs&) const = default;
};

struct spatial_partition_tree
{
	std::optional<std::vector<std::uint8_t>> tree{};
	std::optional<std::vector<std::uint32_t>> always_list{};

	bool operator==(const spatial_partition_tree&) const = default;
};

struct bitarray_dynamic
{
	std::optional<std::vector<std::uint32_t>> array{};
	std::uint32_t bit_count{};

	bool operator==(const bitarray_dynamic&) const = default;
};

struct transient_info
{
	transient_sets transient_sets{};
	transient_costs transient_costs{};
	std::optional<std::vector<forced_stub_asset_reference>> forced_stub_assets{};
	std::optional<std::vector<transient_file_reference>> transient_files{};
	std::optional<std::vector<transient_asset_reference>> transient_assets{};
	std::optional<std::vector<transient_world_grid_tile_map>> transient_world_grid_tile_maps{};
	std::optional<spatial_partition_tree> transient_world_grid_spatial{};
	std::optional<std::vector<bounds>> transient_world_grid_bounds{};
	std::optional<std::vector<std::uint16_t>> transient_world_grid_high_lod_lookup{};
	std::uint32_t transient_world_grid_high_lod_count{};
	std::vector<std::string> transient_info_flags{};
	bitarray_dynamic transient_skip_loading{};
	std::uint32_t transient_collision_tile_count{};
	std::uint32_t transient_mp_common_count{};

	bool operator==(const transient_info&) const = default;
};

struct streaminginfo
{
	std::optional<std::string> name{};
	std::array<alwaysloaded_flag_set, 1> alwaysloaded_flag_sets{};
	std::optional<transient_info> transient_info{};

	bool operator==(const streaminginfo&) const = default;
};

struct alwaysloaded_flags_per_model
{
	std::optional<std::string> model{};
	alwaysloaded_flags flags{};

	bool operator==(const alwaysloaded_flags_per_model&) const = default;
};

struct bitarray_32
{
	std::array<std::uint32_t, 1> array{};

	bool operator==(const bitarray_32&) const = default;
};

struct streaming_item_cost
{
	std::uint32_t cost_in_bytes{};

	bool operator==(const streaming_item_cost&) const = default;
};

struct forced_stub_asset_reference
{
	std::optional<std::string> name{};
	std::uint8_t type{};

	bool operator==(const forced_stub_asset_reference&) const = default;
};

struct transient_file_reference
{
	std::optional<std::string> name{};
	std::uint8_t type{};

	bool operator==(const transient_file_reference&) const = default;
};

struct transient_asset_reference
{
	std::optional<std::string> name{};
	std::uint8_t type{};
	std::uint32_t mp_parent_index{};

	bool operator==(const transient_asset_reference&) const = default;
};

struct transient_world_grid_tile_map
{
	std::optional<std::vector<std::uint16_t>> space_to_transient_index{};
	std::uint32_t level{};
	std::uint32_t lod_count{};
	std::uint32_t grid_count_x{};
	std::uint32_t grid_count_y{};
	float cell_size{};
	float min_x{};
	float min_y{};

	bool operator==(const transient_world_grid_tile_map&) const = default;
};

struct bounds
{
	std::array<float, 3> mid_point{};
	std::array<float, 3> half_size{};

	bool operator==(const bounds&) const = default;
};

struct suit
{
	std::optional<std::string> name{};
	float cam_min_velocity_for_fov_increase{};
	float cam_max_velocity_for_fov_increase{};
	float cam_velocity_to_decrease_fov{};
	float cam_fov_increase_at_max_velocity{};
	float cam_one_over_fov_ease_in_time_ms{};
	float cam_one_over_fov_ease_out_time_ms{};
	std::optional<std::string> cam_helmet_cam{};
	bool cam_draw_helmet{};
	std::array<float, 3> cam_camto_helmet_offset_origin{};
	std::array<float, 3> cam_camto_helmet_offset_angles{};
	bool enable_ik_override{};
	float player_global_accel_scale{};
	float player_crouch_speed_scale{};
	float player_prone_speed_scale{};
	float player_last_stand_crawl_speed_scale{};
	float player_sprint_speed_scale{};
	bool player_sprint_unlimited{};
	float player_view_bob_scale{};
	bool jump_slowdown_enable{};
	float jump_height{};
	float sprint_leap_height{};
	float sprint_leap_forward_velocity_scale{};
	std::int32_t sprint_leap_min_sprint_time_ms{};
	float double_jump_accel{};
	float double_jump_speed{};
	float double_jump_speed_no_boost{};
	float double_jump_friction_min{};
	float double_jump_friction_max{};
	float double_jump_initial_up_boost_accel{};
	float double_jump_standard_up_boost_accel{};
	float double_jump_energy_needed_for_initial_up_boost{};
	float double_jump_energy_needed_for_standard_up_boost{};
	float double_jump_max_upwards_velocity{};
	bool double_jump_enable_min_z_velocity{};
	float double_jump_min_z_velocity{};
	float double_jump_energy_initial_cost{};
	float double_jump_boost_up_energy_use_rate{};
	float double_jump_energy_use_per_button_press{};
	bool double_jump_hover_only{};
	float double_jump_max_view_pitch_dip{};
	float double_jump_max_view_backwards_pitch_dip{};
	float double_jump_max_view_roll{};
	float double_jump_one_over_ease_in_time{};
	float double_jump_one_over_ease_out_time_ms{};
	bool double_jump_always_dip_view{};
	snd_alias_lookup double_jump_sound{};
	snd_alias_lookup double_jump_sound_player{};
	snd_alias_lookup double_jump_release_sound{};
	snd_alias_lookup double_jump_release_sound_player{};
	float slide_strafe_speed_scale{};
	std::int32_t slide_sprint_penalty_ms{};
	bool slide_allow_firing{};
	bool slide_allow_ads{};
	bool slide_allow_weapon_switch{};
	float slide_jump_speed_scale{};
	float slide_energy_cost_ratio{};
	std::int32_t slide_max_time_ms{};
	std::int32_t slide_max_time_reduced_ms{};
	std::int32_t slide_max_time_base_ms{};
	std::int32_t slide_in_time_ms{};
	float slide_in_max_speed_scale{};
	float slide_in_acceleration{};
	float slide_friction_scale_normal{};
	float slide_friction_scale_downhill{};
	float slide_friction_scale_uphill{};
	float slide_friction_scale_blocked{};
	std::int32_t slide_gesture_out_time_for_jump_ms{};
	std::int32_t slide_sprint_delay_ms{};
	bool slide_disable_prone_transition{};
	std::int32_t slide_prone_delay_ms{};
	std::int32_t slide_prone_button_check_time_ms{};
	std::int32_t slide_out_time_ms{};
	float slide_out_friction_scale_start{};
	float slide_out_friction_scale_finish{};
	float slide_out_speed_scale_start{};
	float slide_out_speed_scale_finish{};
	std::int32_t slide_in_air_time_ms{};
	float slide_in_air_friction_scale_start{};
	float slide_in_air_friction_scale_finish{};
	std::int32_t slide_view_blend_in_time_ms{};
	std::int32_t slide_view_blend_out_time_ms{};
	float wall_run_min_z_vel{};
	float wall_run_min_trigger_speed{};
	float wall_run_min_maintain_speed{};
	std::int32_t wall_run_delay_period_ms{};
	float wall_run_min_jump_height{};
	std::int32_t wall_run_max_time_ms{};
	std::int32_t wall_run_fall_stage_time_ms{};
	float wall_run_max_height{};
	float wall_run_jump_height{};
	float wall_run_jump_velocity{};
	float wall_run_friction_scale{};
	float wall_run_speed_scale{};
	float wall_run_speed_scale_ads{};
	float wall_run_energy_initial_cost{};
	float wall_run_energy_change_per_second{};
	float ladder_anchor_offset{};
	float ladder_anchor_offset_wm{};
	float ladder_hand_distance{};
	std::string suit_anim_type{};
	bool is_movement_camera_independent{};
	std::optional<std::string> anim_package{};
	std::optional<std::string> anim_package_l{};
	std::optional<std::string> anim_package_r{};
	std::optional<std::string> anim_package_relaxed{};
	std::optional<std::string> anim_package_safe{};
	std::optional<std::string> asm_asset{};
	std::optional<std::string> animset_asset{};
	std::uint8_t facial_anim_type{};
	std::string body_anim_type{};
	std::optional<std::string> scriptable_def{};
	std::int32_t viewheight_stand{};
	std::int32_t viewheight_crouch{};
	std::int32_t viewheight_prone{};
	std::int32_t viewheight_laststand{};
	std::int32_t viewheight_dead{};
	std::int32_t viewheight_swim{};
	std::int32_t viewheight_slide{};
	std::int32_t viewheight_sprint{};
	std::int32_t bounds_radius{};
	std::int32_t bounds_height_stand{};
	std::int32_t bounds_height_crouch{};
	std::int32_t bounds_height_prone{};
	bounds bounds_stand{};
	bounds bounds_crouch{};
	bounds bounds_prone{};
	std::int32_t radial_motion_blur_interp_time_in{};
	std::int32_t radial_motion_blur_interp_time_out{};
	float radial_motion_blur_sprint_min_radius{};
	float radial_motion_blur_sprint_max_radius{};
	float radial_motion_blur_sprint_min_strength{};
	float radial_motion_blur_sprint_max_strength{};
	float radial_motion_blur_slide_min_radius{};
	float radial_motion_blur_slide_max_radius{};
	float radial_motion_blur_slide_min_strength{};
	float radial_motion_blur_slide_max_strength{};
	float radial_motion_blur_double_jump_min_radius{};
	float radial_motion_blur_double_jump_max_radius{};
	float radial_motion_blur_double_jump_min_strength{};
	float radial_motion_blur_double_jump_max_strength{};
	float radial_motion_blur_wall_run_min_radius{};
	float radial_motion_blur_wall_run_max_radius{};
	float radial_motion_blur_wall_run_min_strength{};
	float radial_motion_blur_wall_run_max_strength{};
	float radial_motion_blur_ground_pound_min_radius{};
	float radial_motion_blur_ground_pound_max_radius{};
	float radial_motion_blur_ground_pound_min_strength{};
	float radial_motion_blur_ground_pound_max_strength{};
	float radial_motion_blur_rewind_min_radius{};
	float radial_motion_blur_rewind_max_radius{};
	float radial_motion_blur_rewind_min_strength{};
	float radial_motion_blur_rewind_max_strength{};
	float radial_motion_blur_dodge_min_radius{};
	float radial_motion_blur_dodge_max_radius{};
	float radial_motion_blur_dodge_min_strength{};
	float radial_motion_blur_dodge_max_strength{};
	float radial_motion_blur_skydive_min_radius{};
	float radial_motion_blur_skydive_max_radius{};
	float radial_motion_blur_skydive_min_strength{};
	float radial_motion_blur_skydive_max_strength{};
	std::optional<std::string> ground_pound_activation_rumble{};
	snd_alias_lookup ground_pound_activation_sound{};
	snd_alias_lookup ground_pound_activation_sound_player{};
	std::optional<std::string> ground_pound_landing_rumble{};
	snd_alias_lookup ground_pound_landing_sound{};
	snd_alias_lookup ground_pound_landing_sound_player{};
	std::optional<std::string> landing_rumble_low_height{};
	std::optional<std::string> landing_rumble_medium_height{};
	std::optional<std::string> landing_rumble_high_height{};
	std::optional<std::string> landing_rumble_extreme_height{};
	float landing_speed_scale{};
	float footstep_shake_broadcast_radius_inches{};
	std::int32_t footstep_shake_duration_ms{};
	float footstep_shake_amplitude{};
	std::optional<std::string> footstep_rumble{};
	std::optional<std::string> footstepvfx_slide{};
	std::optional<std::string> footstepvfx_creep_left{};
	std::optional<std::string> footstepvfx_creep_right{};
	std::optional<std::string> footstepvfx_walk_left{};
	std::optional<std::string> footstepvfx_walk_right{};
	std::optional<std::string> footstepvfx_run_left{};
	std::optional<std::string> footstepvfx_run_right{};
	std::optional<std::string> footstepvfx_sprint_left{};
	std::optional<std::string> footstepvfx_sprint_right{};
	float skydive_mass{};
	float skydive_gravitatational_acceleration{};
	std::optional<std::string> skydive_parachute_view_model{};
	std::optional<std::string> skydive_parachute_world_model{};
	float skydive_base_jump_minimum_height{};
	float skydive_base_jump_clearance_radius{};
	float skydive_base_jump_auto_deploy_height{};
	float skydive_base_jump_force_freefall_height{};
	float skydive_freefall_super_dive_camera_pitch{};
	float skydive_freefall_super_dive_stick_input{};
	float skydive_freefall_super_dive_fov_adjust_degrees{};
	std::int32_t skydive_freefall_super_dive_fov_adjust_in_time{};
	std::int32_t skydive_freefall_super_dive_fov_adjust_out_time{};
	std::optional<std::string> skydive_freefall_cinematic_motion{};
	float skydive_freefall_throttle_track_speed{};
	float skydive_freefall_anim_track_speed_throttle{};
	float skydive_freefall_anim_track_speed_strafe{};
	float skydive_freefall_anim_track_speed_yaw{};
	float skydive_freefall_anim_track_speed_pitch{};
	float skydive_freefall_anim_min_weight{};
	float skydive_freefall_anim_max_weight{};
	float skydive_freefall_anim_min_play_rate{};
	float skydive_freefall_anim_max_play_rate{};
	float skydive_freefall_anim_speed_for_min_weight{};
	float skydive_freefall_anim_speed_for_max_weight{};
	float skydive_freefall_top_ground_speed{};
	float skydive_freefall_min_pitch{};
	float skydive_freefall_max_pitch{};
	float skydive_freefall_pitch_speed{};
	float skydive_freefall_yaw_speed{};
	float skydive_freefall_cross_sectional_area{};
	float skydive_freefall_look_horiz_force_min{};
	float skydive_freefall_look_horiz_force_max{};
	std::optional<std::string> skydive_freefall_look_horiz_force_graph{};
	float skydive_freefall_stick_horiz_force_min{};
	float skydive_freefall_stick_horiz_force_max{};
	std::optional<std::string> skydive_freefall_stick_horiz_force_graph{};
	float skydive_freefall_drag_coef_horiz{};
	float skydive_freefall_look_horiz_drag_coeff{};
	std::optional<std::string> skydive_freefall_look_horiz_drag_graph{};
	float skydive_freefall_stick_horiz_drag_coeff{};
	std::optional<std::string> skydive_freefall_stick_horiz_drag_graph{};
	float skydive_freefall_drag_coef_vert{};
	float skydive_freefall_look_vert_drag_coeff{};
	std::optional<std::string> skydive_freefall_look_vert_drag_graph{};
	float skydive_freefall_stick_vert_drag_increase_perc{};
	std::optional<std::string> skydive_freefall_stick_vert_drag_increase_graph{};
	float skydive_freefall_stick_vert_drag_reduction_perc{};
	std::optional<std::string> skydive_freefall_stick_vert_drag_reduction_graph{};
	float skydive_freefall_stick_sideways_force{};
	float skydive_freefall_max_brake_force{};
	float skydive_freefall_min_turn_multiplier{};
	float skydive_freefall_max_turn_multiplier{};
	std::optional<std::string> skydive_freefall_turn_multiplier_graph{};
	float skydive_freefall_turn_multiplier_min_scale{};
	float skydive_freefall_turn_multiplier_max_scale{};
	std::optional<std::string> skydive_freefall_turn_multiplier_pitch_graph{};
	snd_alias_lookup skydive_freefall_ambient_sound{};
	float skydive_freefall_ambient_sound_ground_speed_weight{};
	float skydive_freefall_ambient_sound_min_volume{};
	float skydive_freefall_ambient_sound_max_volume{};
	float skydive_freefall_ambient_sound_min_pitch{};
	float skydive_freefall_ambient_sound_max_pitch{};
	float skydive_freefall_ambient_sound_min_ground_speed{};
	float skydive_freefall_ambient_sound_max_ground_speed{};
	float skydive_freefall_ambient_sound_min_fall_speed{};
	float skydive_freefall_ambient_sound_max_fall_speed{};
	snd_alias_lookup skydive_freefall_high_speed_sound{};
	float skydive_freefall_high_speed_sound_ground_speed_weight{};
	float skydive_freefall_high_speed_sound_min_volume{};
	float skydive_freefall_high_speed_sound_max_volume{};
	float skydive_freefall_high_speed_sound_min_pitch{};
	float skydive_freefall_high_speed_sound_max_pitch{};
	float skydive_freefall_high_speed_sound_min_ground_speed{};
	float skydive_freefall_high_speed_sound_max_ground_speed{};
	float skydive_freefall_high_speed_sound_min_fall_speed{};
	float skydive_freefall_high_speed_sound_max_fall_speed{};
	float skydive_deploy_horizontal_drag{};
	float skydive_deploy_vertical_drag{};
	std::optional<std::string> skydive_canopy_cinematic_motion{};
	float skydive_canopy_throttle_track_speed{};
	float skydive_canopy_anim_track_speed_throttle{};
	float skydive_canopy_anim_track_speed_strafe{};
	float skydive_canopy_anim_track_speed_yaw{};
	float skydive_canopy_anim_track_speed_pitch{};
	float skydive_canopy_anim_min_weight{};
	float skydive_canopy_anim_max_weight{};
	float skydive_canopy_anim_min_play_rate{};
	float skydive_canopy_anim_max_play_rate{};
	float skydive_canopy_anim_speed_for_min_weight{};
	float skydive_canopy_anim_speed_for_max_weight{};
	float skydive_canopy_top_ground_speed{};
	float skydive_canopy_min_pitch{};
	float skydive_canopy_max_pitch{};
	float skydive_canopy_pitch_speed{};
	float skydive_canopy_yaw_speed{};
	float skydive_canopy_cross_sectional_area{};
	float skydive_canopy_look_horiz_force_min{};
	float skydive_canopy_look_horiz_force_max{};
	std::optional<std::string> skydive_canopy_look_horiz_force_graph{};
	float skydive_canopy_stick_horiz_force_min{};
	float skydive_canopy_stick_horiz_force_max{};
	std::optional<std::string> skydive_canopy_stick_horiz_force_graph{};
	float skydive_canopy_drag_coef_horiz{};
	float skydive_canopy_look_horiz_drag_coeff{};
	std::optional<std::string> skydive_canopy_look_horiz_drag_graph{};
	float skydive_canopy_stick_horiz_drag_coeff{};
	std::optional<std::string> skydive_canopy_stick_horiz_drag_graph{};
	float skydive_canopy_drag_coef_vert{};
	float skydive_canopy_look_vert_drag_coeff{};
	std::optional<std::string> skydive_canopy_look_vert_drag_graph{};
	float skydive_canopy_stick_vert_drag_increase_perc{};
	std::optional<std::string> skydive_canopy_stick_vert_drag_increase_graph{};
	float skydive_canopy_stick_vert_drag_reduction_perc{};
	std::optional<std::string> skydive_canopy_stick_vert_drag_reduction_graph{};
	float skydive_canopy_stick_sideways_force{};
	float skydive_canopy_max_brake_force{};
	float skydive_canopy_min_turn_multiplier{};
	float skydive_canopy_max_turn_multiplier{};
	std::optional<std::string> skydive_canopy_turn_multiplier_graph{};
	float skydive_canopy_turn_multiplier_min_scale{};
	float skydive_canopy_turn_multiplier_max_scale{};
	std::optional<std::string> skydive_canopy_turn_multiplier_pitch_graph{};
	snd_alias_lookup skydive_canopy_ambient_sound{};
	float skydive_canopy_ambient_sound_ground_speed_weight{};
	float skydive_canopy_ambient_sound_min_volume{};
	float skydive_canopy_ambient_sound_max_volume{};
	float skydive_canopy_ambient_sound_min_pitch{};
	float skydive_canopy_ambient_sound_max_pitch{};
	float skydive_canopy_ambient_sound_min_ground_speed{};
	float skydive_canopy_ambient_sound_max_ground_speed{};
	float skydive_canopy_ambient_sound_min_fall_speed{};
	float skydive_canopy_ambient_sound_max_fall_speed{};
	snd_alias_lookup skydive_canopy_high_speed_sound{};
	float skydive_canopy_high_speed_sound_ground_speed_weight{};
	float skydive_canopy_high_speed_sound_min_volume{};
	float skydive_canopy_high_speed_sound_max_volume{};
	float skydive_canopy_high_speed_sound_min_pitch{};
	float skydive_canopy_high_speed_sound_max_pitch{};
	float skydive_canopy_high_speed_sound_min_ground_speed{};
	float skydive_canopy_high_speed_sound_max_ground_speed{};
	float skydive_canopy_high_speed_sound_min_fall_speed{};
	float skydive_canopy_high_speed_sound_max_fall_speed{};
	float skydive_wm_anim_freefall_pitch_min{};
	float skydive_wm_anim_freefall_pitch_max{};
	float skydive_wm_anim_freefall_pitch_track_speed{};
	float skydive_wm_anim_freefall_roll_track_speed{};
	float skydive_wm_anim_freefall_roll_return_speed{};
	std::optional<std::string> skydive_wm_anim_freefall_left_stick_x_to_roll_graph{};
	std::optional<std::string> skydive_wm_anim_freefall_right_stick_x_to_roll_graph{};
	float skydive_cam_freefall_height_offset{};
	std::optional<std::string> skydive_cam_freefall_pitch_to_distance_graph{};
	std::optional<std::string> skydive_cam_freefall_pitch_to_forward_offset_graph{};
	float skydive_cam_parachute_height_offset{};
	std::optional<std::string> skydive_cam_parachute_pitch_to_distance_graph{};
	std::optional<std::string> skydive_cam_parachute_pitch_to_forward_offset_graph{};
	std::optional<std::string> skydive_cam_parachute_pitch_to_model_pitch_forward_offset_graph{};
	std::optional<std::string> skydive_cam_parachute_pitch_to_model_roll_side_offset_graph{};
	std::optional<std::string> skydive_cam_parachute_pitch_to_model_roll_up_offset_graph{};
	float skydive_cam_parachute_blend_in_time{};
	snd_alias_lookup nvg_blind_sound{};
	std::array<float, 8> dlc_float{};

	bool operator==(const suit&) const = default;
};

struct suit_anim_per_weap_class
{
	std::optional<std::vector<suit_anim_entry>> anims{};
	std::int32_t weap_classes{};
	std::optional<std::vector<suit_gesture_entry>> gestures{};
	std::optional<std::string> cinematic_motion{};
	std::array<footstep_anim, 13> footstep{};
	float skydive_parachute_slow_soft_land_velocity{};
	float skydive_parachute_slow_hard_land_velocity{};
	float skydive_parachute_fast_land_velocity{};

	bool operator==(const suit_anim_per_weap_class&) const = default;
};

struct suitanimpackage
{
	std::optional<std::string> name{};
	std::optional<std::array<suit_anim_per_weap_class, 6>> anim_overrides{};

	bool operator==(const suitanimpackage&) const = default;
};

struct suit_anim_entry
{
	std::optional<std::string> anim{};
	std::int32_t time{};
	std::int32_t interrupt_time{};
	std::uint32_t anim_type{};

	bool operator==(const suit_anim_entry&) const = default;
};

struct suit_gesture_entry
{
	std::optional<std::string> gesture{};
	std::string gesture_anim_type{};

	bool operator==(const suit_gesture_entry&) const = default;
};

struct coverselector
{
	std::optional<std::string> name{};
	std::optional<std::string> scr_name{};
	float distance_weight{};
	bool use_distance_squared{};
	float distance_min{};
	float distance_optimal{};
	float distance_max{};
	float engagement_weight{};
	float dangerous_weight{};
	float target_dir_weight{};
	float move_towards_target_weight{};
	float visible_weight{};
	float not_visible_weight{};
	bool consider_cover_nodes{};
	bool consider_tac_points{};
	float strict_cover_node_weight{};
	float conceal_node_weight{};
	float exposed_node_weight{};
	float ambush_node_weight{};
	float node_angle_weight{};
	float secondary_target_weight{};
	float player_sight_weight{};
	float priority_weight{};
	float lmg_mountable_weight{};
	float indoor_weight{};
	float outdoor_weight{};
	float height_weight{};
	float target_height_weight{};
	float stickiness_weight{};

	bool operator==(const coverselector&) const = default;
};

struct enemyselector
{
	std::optional<std::string> name{};
	std::optional<std::string> scr_name{};
	std::int32_t threat_visible{};
	std::int32_t threat_fully_aware{};
	std::int32_t threat_friendly_timing_out_on_enemy{};
	std::int32_t threat_scariness_scale{};
	std::int32_t threat_scariness_min{};
	std::int32_t threat_scariness_max{};
	std::int32_t threat_shared_enemy{};
	std::int32_t threat_shared_enemy_cap{};
	std::int32_t threat_shared_melee_enemy{};
	std::int32_t threat_friendlies_current_enemy{};
	std::int32_t threat_friendlies_timeout_enemy{};
	std::int32_t threat_current_enemy{};
	std::int32_t threat_current_enemy_player_damaged{};
	std::int32_t threat_flashed_enemy{};
	std::int32_t threat_no_cover{};
	std::int32_t threat_player_attacked_me_recently{};
	std::int32_t threat_killed_teammate_recently{};
	std::int32_t threat_behind_enemy_lines_max{};
	std::int32_t threat_behind_enemy_lines_dist{};
	std::int32_t threat_covering_fire{};
	std::int32_t threat_cover_fire_cur_target{};
	float threat_distance_max_range{};
	float threat_distance_max_threat{};
	float threat_min_dist_to_ignore{};
	float threat_min_suppression{};

	bool operator==(const enemyselector&) const = default;
};

struct carryobject
{
	std::optional<std::string> name{};
	std::string type{};
	std::optional<std::string> carry_view_model{};
	std::optional<std::string> carry_world_model{};
	std::optional<std::string> gesture{};
	std::optional<std::string> attach_tag{};
	bool allow_ads{};

	bool operator==(const carryobject&) const = default;
};

struct execution
{
	std::optional<std::string> name{};
	std::uint32_t anim_type{};
	bool use_prop_weapon{};
	std::optional<std::string> prop_weapon_attach_tag{};
	bool prop_weapon_inventory_override_enabled{};
	bool prop_weapon_enable_ik{};
	std::string prop_weapon_inventory_override_class{};
	std::int32_t duration_victim_stand_ms{};
	std::optional<std::string> xcam_victim_stand{};
	std::int32_t duration_victim_prone_ms{};
	std::optional<std::string> xcam_victim_prone{};
	std::int32_t duration_victim_last_stand_ms{};
	std::optional<std::string> xcam_victim_last_stand{};
	std::array<float, 8> dlc_float{};

	bool operator==(const execution&) const = default;
};

struct clientcharacter
{
	std::optional<std::string> name{};
	std::optional<std::string> scriptable_def{};
	std::optional<std::string> anim_file_name{};
	std::optional<std::string> anim_tree_name{};

	bool operator==(const clientcharacter&) const = default;
};

struct physicslibrary
{
	std::optional<std::string> name{};
	bool is_material_list{};
	bool is_body_quality_list{};
	bool is_motion_properties_list{};
	bool is_global_type_compendium{};
	std::optional<std::vector<char>> havok_data{};

	bool operator==(const physicslibrary&) const = default;
};

struct ttf
{
	std::optional<std::string> name{};
	std::int32_t file_len{};
	std::optional<std::vector<char>> file{};

	bool operator==(const ttf&) const = default;
};

inline void validate(const weapon_anim_package_state_timers& data)
{
	(void) data;
}

inline void validate(const footstep_time& data)
{
	(void) data;
}

inline void validate(const movement_time& data)
{
	(void) data;
}

inline void validate(const footstep_anim& data)
{
	for (const auto& item : data.step)
	{
		validate(item);
	}

	for (const auto& item : data.movement)
	{
		validate(item);
	}

}

inline void validate(const animpkg& data)
{
	if (!data.name)
	{
		throw std::runtime_error("animpkg has no name");
	}

	detail::name(*data.name, "animpkg");

	if (data.timers)
	{
		validate(*data.timers);
	}

	if (data.footstep)
	{
		for (const auto& item : *data.footstep)
		{
			validate(item);
		}
	}

}

inline void validate(const snd_alias_lookup& data)
{
	(void) data;
}

inline void validate(const weapon_sfx_package_sounds& data)
{
	validate(data.pickup_sound);

	validate(data.pickup_sound_player);

	validate(data.ammo_pickup_sound);

	validate(data.ammo_pickup_sound_player);

	validate(data.projectile_sound);

	validate(data.pullback_sound);

	validate(data.pullback_sound_player);

	validate(data.pullback_sound_quick);

	validate(data.pullback_sound_quick_player);

	validate(data.pre_fire_sound);

	validate(data.pre_fire_sound_player);

	validate(data.fire_sound);

	validate(data.fire_sound_player);

	validate(data.fire_sound_player_akimbo);

	validate(data.fire_sound_player_akimbo_right);

	validate(data.fire_sound_atmosphere);

	validate(data.fire_sound_atmosphere_player);

	validate(data.fire_sound_atmosphere_player_akimbo);

	validate(data.fire_sound_atmosphere_player_akimbo_right);

	validate(data.fire_start_sound);

	validate(data.fire_start_sound_player);

	validate(data.fire_loop_sound);

	validate(data.fire_loop_sound_player);

	validate(data.fire_stop_sound);

	validate(data.fire_stop_sound_player);

	validate(data.fire_first_sound);

	validate(data.fire_first_sound_player);

	validate(data.fire_first_sound_player_akimbo);

	validate(data.fire_first_sound_player_akimbo_right);

	validate(data.fire_hyper_burst_sound);

	validate(data.fire_hyper_burst_sound_player);

	validate(data.fire_hyper_burst_sound_player_akimbo);

	validate(data.fire_hyper_burst_sound_player_akimbo_right);

	validate(data.fire_last_sound);

	validate(data.fire_last_sound_player);

	validate(data.fire_last_sound_player_akimbo);

	validate(data.fire_last_sound_player_akimbo_right);

	validate(data.regenerate_sound);

	validate(data.regenerate_sound_player);

	validate(data.regenerate_add_sound);

	validate(data.regenerate_add_sound_player);

	validate(data.empty_fire_sound);

	validate(data.empty_fire_sound_player);

	validate(data.empty_regen_fire_sound);

	validate(data.empty_regen_fire_sound_player);

	validate(data.rechamber_sound);

	validate(data.rechamber_sound_player);

	validate(data.reload_sound);

	validate(data.reload_sound_player);

	validate(data.reload_empty_sound);

	validate(data.reload_empty_sound_player);

	validate(data.reload_start_sound);

	validate(data.reload_start_sound_player);

	validate(data.reload_end_sound);

	validate(data.reload_end_sound_player);

	validate(data.reload_end_rechamber_sound);

	validate(data.reload_end_rechamber_sound_player);

	validate(data.detonate_sound);

	validate(data.detonate_sound_player);

	validate(data.night_vision_wear_sound);

	validate(data.night_vision_wear_sound_player);

	validate(data.night_vision_remove_sound);

	validate(data.night_vision_remove_sound_player);

	validate(data.alt_switch_sound);

	validate(data.alt_switch_sound_player);

	validate(data.raise_sound);

	validate(data.raise_sound_player);

	validate(data.first_raise_sound);

	validate(data.first_raise_sound_player);

	validate(data.putaway_sound);

	validate(data.putaway_sound_player);

	validate(data.enter_ads_sound);

	validate(data.enter_ads_sound_player);

	validate(data.leave_ads_sound);

	validate(data.leave_ads_sound_player);

	validate(data.scan_sound);

	validate(data.change_variable_zoom_sound);

	validate(data.bullet_ricochet_impact);

	validate(data.proj_explosion_sound);

	validate(data.proj_dud_sound);

	validate(data.proj_timeout_sound);

	validate(data.proj_ignition_sound);

	validate(data.bounce_sound);

	validate(data.bounce_sound_heavy);

	validate(data.bounce_sound_medium);

	validate(data.bounce_sound_soft);

	validate(data.rolling_sound);

	validate(data.turret_spin_snd);

	validate(data.turret_stop_snd);

	validate(data.trigger_disconnect_sound);

	validate(data.trigger_disconnect_sound_player_akimbo);

	validate(data.trigger_disconnect_sound_player_akimbo_right);

	validate(data.dlc_sound0);

	validate(data.dlc_sound0_player);

	validate(data.dlc_sound1);

	validate(data.dlc_sound1_player);

}

inline void validate(const trigger_disconnect_sound_data& data)
{
	(void) data;
}

inline void validate(const sfxpkg& data)
{
	if (!data.name)
	{
		throw std::runtime_error("sfxpkg has no name");
	}

	detail::name(*data.name, "sfxpkg");

	if (data.sounds)
	{
		validate(*data.sounds);
	}

	if (std::ranges::none_of(melee_impact_type_names, [&](const auto& entry) { return entry.second == data.sfx_melee_impact_type; }))
	{
		throw std::runtime_error("sfxpkg.sfx_melee_impact_type names no value");
	}

	validate(data.trigger_disconnect_sound_data);

}

inline void validate(const fx_combined_def& data)
{
	(void) data;
}

inline void validate(const weapon_vfx_package_defs& data)
{
	validate(data.view_flash_effect);

	validate(data.world_flash_effect);

	validate(data.view_flash_ads_effect);

	validate(data.view_flash_first_effect);

	validate(data.world_flash_first_effect);

	validate(data.view_flash_first_ads_effect);

	validate(data.view_flash_overcharge_effect);

	validate(data.view_flash_overcharge_ads_effect);

	validate(data.world_flash_overcharge_effect);

	validate(data.view_shell_eject_effect);

	validate(data.world_shell_eject_effect);

	validate(data.view_last_shot_eject_effect);

	validate(data.world_last_shot_eject_effect);

	validate(data.view_mag_eject_effect);

	validate(data.view_muzzle_smoke_effect);

	validate(data.world_muzzle_smoke_effect);

	validate(data.view_barrel_smoke_effect);

	validate(data.world_barrel_smoke_effect);

	validate(data.view_ejection_port_smoke_effect);

	validate(data.world_ejection_port_smoke_effect);

	validate(data.view_dlc_effect0);

	validate(data.world_dlc_effect0);

	validate(data.view_dlc_effect1);

	validate(data.world_dlc_effect1);

	validate(data.view_dlc_effect2);

	validate(data.world_dlc_effect2);

	validate(data.view_dlc_effect3);

	validate(data.world_dlc_effect3);

}

inline void validate(const vfxpkg& data)
{
	if (!data.name)
	{
		throw std::runtime_error("vfxpkg has no name");
	}

	detail::name(*data.name, "vfxpkg");

	if (data.vfx)
	{
		validate(*data.vfx);
	}

}

inline void validate(const tracer& data)
{
	if (!data.name)
	{
		throw std::runtime_error("tracer has no name");
	}

	detail::name(*data.name, "tracer");

	validate(data.effect);

	validate(data.viewmodel_effect);

}

inline void validate(const laser& data)
{
	if (!data.name)
	{
		throw std::runtime_error("laser has no name");
	}

	detail::name(*data.name, "laser");

	validate(data.beam_effect);

	validate(data.laser_end_effect);

}

inline void validate(const camera_first_person_profile_def& data)
{
	(void) data;
}

inline void validate(const camera_orbit_profile_def& data)
{
	(void) data;
}

inline void validate(const camera_sentry_drone_def& data)
{
	(void) data;
}

inline void validate(const camera_kill_cam_vehicle_def& data)
{
	(void) data;
}

inline void validate(const camera_transition_def& data)
{
	if (std::ranges::none_of(camera_transition_curve_names, [&](const auto& entry) { return entry.second == data.curve_eyepos; }))
	{
		throw std::runtime_error("camera_transition_def.curve_eyepos names no value");
	}

	if (std::ranges::none_of(camera_transition_curve_names, [&](const auto& entry) { return entry.second == data.curve_axis; }))
	{
		throw std::runtime_error("camera_transition_def.curve_axis names no value");
	}

}

inline void validate(const camera& data)
{
	if (!data.name)
	{
		throw std::runtime_error("camera has no name");
	}

	detail::name(*data.name, "camera");

	if (std::ranges::none_of(camera_profile_names, [&](const auto& entry) { return entry.second == data.profile; }))
	{
		throw std::runtime_error("camera.profile names no value");
	}

	if (std::ranges::none_of(lens_profile_mode_names, [&](const auto& entry) { return entry.second == data.override_lens_profile; }))
	{
		throw std::runtime_error("camera.override_lens_profile names no value");
	}

	validate(data.first_person);

	validate(data.orbit);

	validate(data.sentry_drone);

	validate(data.kcam_veh);

	validate(data.transition_in);

	validate(data.transition_out);

}

inline void validate(const hudoutline& data)
{
	if (!data.name)
	{
		throw std::runtime_error("hudoutline has no name");
	}

	detail::name(*data.name, "hudoutline");

}

inline void validate(const footstepvfx& data)
{
	if (!data.name)
	{
		throw std::runtime_error("footstepvfx has no name");
	}

	detail::name(*data.name, "footstepvfx");

	for (const auto& item : data.footstep_vfx)
	{
		validate(item);
	}

	for (const auto& item : data.small_footstep_vfx)
	{
		validate(item);
	}

}

inline void validate(const x_composite_submodel& data)
{
	(void) data;
}

inline void validate(const x_composite_sub_composite_model& data)
{
	(void) data;
}

inline void validate(const xcompositemodel& data)
{
	if (!data.name)
	{
		throw std::runtime_error("xcompositemodel has no name");
	}

	detail::name(*data.name, "xcompositemodel");

	if (data.submodels)
	{
		for (const auto& item : *data.submodels)
		{
			validate(item);
		}
	}

	if (data.sub_composite_models)
	{
		for (const auto& item : *data.sub_composite_models)
		{
			validate(item);
		}
	}

}

inline void validate(const xanimcurve& data)
{
	if (!data.name)
	{
		throw std::runtime_error("xanimcurve has no name");
	}

	detail::name(*data.name, "xanimcurve");

	if (data.control_points)
	{
		for (const auto& item : *data.control_points)
		{
			validate(item);
		}
	}

	if (std::ranges::none_of(x_anim_curve_type_names, [&](const auto& entry) { return entry.second == data.curve_type; }))
	{
		throw std::runtime_error("xanimcurve.curve_type names no value");
	}

	if ((data.control_points && data.control_points->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("xanimcurve has arrays that its count does not fit");
	}

}

inline void validate(const x_anim_curve_control_point& data)
{
	(void) data;
}

inline void validate(const keyvaluepairs& data)
{
	if (!data.name)
	{
		throw std::runtime_error("keyvaluepairs has no name");
	}

	detail::name(*data.name, "keyvaluepairs");

	if (data.key_value_pairs)
	{
		for (const auto& item : *data.key_value_pairs)
		{
			validate(item);
		}
	}

	if ((data.key_value_pairs && data.key_value_pairs->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("keyvaluepairs has arrays that its count does not fit");
	}

}

inline void validate(const key_value_pair& data)
{
	(void) data;
}

inline void validate(const blendspace2d& data)
{
	if (!data.name)
	{
		throw std::runtime_error("blendspace2d has no name");
	}

	detail::name(*data.name, "blendspace2d");

	if (data.anim_entries)
	{
		for (const auto& item : *data.anim_entries)
		{
			validate(item);
		}
	}

	if ((data.anim_entries && data.anim_entries->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("blendspace2d has arrays that its count does not fit");
	}

	if ((data.triangle_coords && data.triangle_coords->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("blendspace2d has arrays that its count does not fit");
	}

}

inline void validate(const blend_space2_d_anim_entry& data)
{
	(void) data;
}

inline void validate(const camo& data)
{
	if (!data.name)
	{
		throw std::runtime_error("camo has no name");
	}

	detail::name(*data.name, "camo");

	if (data.texture_defs)
	{
		for (const auto& item : *data.texture_defs)
		{
			validate(item);
		}
	}

	if ((data.texture_defs && data.texture_defs->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("camo has arrays that its count does not fit");
	}

}

inline void validate(const material_texture_def& data)
{
	(void) data;
}

inline void validate(const accessory& data)
{
	if (!data.name)
	{
		throw std::runtime_error("accessory has no name");
	}

	detail::name(*data.name, "accessory");

}

inline void validate(const locdmgtable& data)
{
	if (!data.name)
	{
		throw std::runtime_error("locdmgtable has no name");
	}

	detail::name(*data.name, "locdmgtable");

}

inline void validate(const bulletpenetration& data)
{
	if (!data.name)
	{
		throw std::runtime_error("bulletpenetration has no name");
	}

	detail::name(*data.name, "bulletpenetration");

}

inline void validate(const cinematic_shake_modifier_def& data)
{
	(void) data;
}

inline void validate(const cinematic_shake_def& data)
{
	validate(data.move_speed_modifiers);

	validate(data.view_speed_modifiers);

}

inline void validate(const cinematic_impact_shake_def& data)
{
	(void) data;
}

inline void validate(const cinematicmotion& data)
{
	if (!data.name)
	{
		throw std::runtime_error("cinematicmotion has no name");
	}

	detail::name(*data.name, "cinematicmotion");

	validate(data.camera_shake_rotation);

	validate(data.camera_shake_translation);

	validate(data.camera_shake_impact_rotation);

	validate(data.camera_shake_impact_translation);

}

inline void validate(const decalvolumemask& data)
{
	if (!data.name)
	{
		throw std::runtime_error("decalvolumemask has no name");
	}

	detail::name(*data.name, "decalvolumemask");

}

inline void validate(const gesture_weapon_settings& data)
{
	(void) data;
}

inline void validate(const gesture_directional_settings& data)
{
	(void) data;
}

inline void validate(const gesture_look_around_settings& data)
{
	validate(data.walk_foot_step_anim);

}

inline void validate(const gesture_ik_target_settings& data)
{
	(void) data;
}

inline void validate(const gesture& data)
{
	if (!data.name)
	{
		throw std::runtime_error("gesture has no name");
	}

	detail::name(*data.name, "gesture");

	if (std::ranges::none_of(gesture_type_names, [&](const auto& entry) { return entry.second == data.type; }))
	{
		throw std::runtime_error("gesture.type names no value");
	}

	if (std::ranges::none_of(gesture_priority_names, [&](const auto& entry) { return entry.second == data.priority; }))
	{
		throw std::runtime_error("gesture.priority names no value");
	}

	validate(data.weapon_settings);

	validate(data.directional_settings);

	validate(data.look_around_settings);

	validate(data.ik_target_settings);

}

inline void validate(const reticle& data)
{
	if (!data.name)
	{
		throw std::runtime_error("reticle has no name");
	}

	detail::name(*data.name, "reticle");

}

inline void validate(const alwaysloaded_flags& data)
{
	if (static_cast<std::int64_t>(((((data.images ? data.images->size() : std::size_t{}) * 4) + 31) >> 5)) != (data.image_flags ? static_cast<std::int64_t>(data.image_flags->size()) : 0))
	{
		throw std::runtime_error("alwaysloaded_flags.image_flags has a length that its count does not give");
	}

	if ((data.images && data.images->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("alwaysloaded_flags has arrays that its count does not fit");
	}

}

inline void validate(const alwaysloaded_flag_set& data)
{
	validate(data.global_flags);

	if (data.per_model_flags)
	{
		for (const auto& item : *data.per_model_flags)
		{
			validate(item);
		}
	}

	if ((data.per_model_flags && data.per_model_flags->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("alwaysloaded_flag_set has arrays that its count does not fit");
	}

	if ((data.model_surfs && data.model_surfs->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("alwaysloaded_flag_set has arrays that its count does not fit");
	}

}

inline void validate(const transient_sets& data)
{
	if (data.masks)
	{
		for (const auto& item : *data.masks)
		{
			validate(item);
		}
	}

	if (static_cast<std::int64_t>(data.count) != (data.names ? static_cast<std::int64_t>(data.names->size()) : 0))
	{
		throw std::runtime_error("transient_sets.names has a length that its count does not give");
	}

	if (static_cast<std::int64_t>(data.count) != (data.masks ? static_cast<std::int64_t>(data.masks->size()) : 0))
	{
		throw std::runtime_error("transient_sets.masks has a length that its count does not give");
	}

}

inline void validate(const streaming_content_set& data)
{
	(void) data;
}

inline void validate(const transient_costs& data)
{
	validate(data.stream_contents);

	if (data.streaming_costs)
	{
		for (const auto& item : *data.streaming_costs)
		{
			validate(item);
		}
	}

	if ((data.streaming_costs && data.streaming_costs->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("transient_costs has arrays that its count does not fit");
	}

	if ((data.resident_page_sizes && data.resident_page_sizes->size() > 0xFFFFFFFFull) || (data.temp_page_sizes && data.temp_page_sizes->size() > 0xFFFFFFFFull) || (!data.resident_page_sizes != !data.temp_page_sizes) || (data.resident_page_sizes && data.temp_page_sizes && data.resident_page_sizes->size() != data.temp_page_sizes->size()))
	{
		throw std::runtime_error("transient_costs has arrays that its count does not fit");
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

inline void validate(const bitarray_dynamic& data)
{
	if ((data.array && data.array->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("bitarray_dynamic has arrays that its count does not fit");
	}

}

inline void validate(const transient_info& data)
{
	validate(data.transient_sets);

	validate(data.transient_costs);

	if (data.forced_stub_assets)
	{
		for (const auto& item : *data.forced_stub_assets)
		{
			validate(item);
		}
	}

	if (data.transient_files)
	{
		for (const auto& item : *data.transient_files)
		{
			validate(item);
		}
	}

	if (data.transient_assets)
	{
		for (const auto& item : *data.transient_assets)
		{
			validate(item);
		}
	}

	if (data.transient_world_grid_tile_maps)
	{
		for (const auto& item : *data.transient_world_grid_tile_maps)
		{
			validate(item);
		}
	}

	if (data.transient_world_grid_spatial)
	{
		validate(*data.transient_world_grid_spatial);
	}

	if (data.transient_world_grid_bounds)
	{
		for (const auto& item : *data.transient_world_grid_bounds)
		{
			validate(item);
		}
	}

	for (const auto& name : data.transient_info_flags)
	{
		if (std::ranges::none_of(transient_info_flags_names, [&](const auto& entry) { return entry.second == name; }))
		{
			throw std::runtime_error("transient_info.transient_info_flags names no flag");
		}
	}

	validate(data.transient_skip_loading);

	if ((data.forced_stub_assets && data.forced_stub_assets->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("transient_info has arrays that its count does not fit");
	}

	if ((data.transient_files && data.transient_files->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("transient_info has arrays that its count does not fit");
	}

	if ((data.transient_assets && data.transient_assets->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("transient_info has arrays that its count does not fit");
	}

	if ((data.transient_world_grid_tile_maps && data.transient_world_grid_tile_maps->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("transient_info has arrays that its count does not fit");
	}

	if ((data.transient_world_grid_bounds && data.transient_world_grid_bounds->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("transient_info has arrays that its count does not fit");
	}

	if ((data.transient_world_grid_high_lod_lookup && data.transient_world_grid_high_lod_lookup->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("transient_info has arrays that its count does not fit");
	}

}

inline void validate(const streaminginfo& data)
{
	if (!data.name)
	{
		throw std::runtime_error("streaminginfo has no name");
	}

	detail::name(*data.name, "streaminginfo");

	for (const auto& item : data.alwaysloaded_flag_sets)
	{
		validate(item);
	}

	if (data.transient_info)
	{
		validate(*data.transient_info);
	}

}

inline void validate(const alwaysloaded_flags_per_model& data)
{
	validate(data.flags);

}

inline void validate(const bitarray_32& data)
{
	(void) data;
}

inline void validate(const streaming_item_cost& data)
{
	(void) data;
}

inline void validate(const forced_stub_asset_reference& data)
{
	(void) data;
}

inline void validate(const transient_file_reference& data)
{
	(void) data;
}

inline void validate(const transient_asset_reference& data)
{
	(void) data;
}

inline void validate(const transient_world_grid_tile_map& data)
{
	if ((data.space_to_transient_index && data.space_to_transient_index->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("transient_world_grid_tile_map has arrays that its count does not fit");
	}

}

inline void validate(const bounds& data)
{
	(void) data;
}

inline void validate(const suit& data)
{
	if (!data.name)
	{
		throw std::runtime_error("suit has no name");
	}

	detail::name(*data.name, "suit");

	validate(data.double_jump_sound);

	validate(data.double_jump_sound_player);

	validate(data.double_jump_release_sound);

	validate(data.double_jump_release_sound_player);

	if (std::ranges::none_of(suit_anim_type_names, [&](const auto& entry) { return entry.second == data.suit_anim_type; }))
	{
		throw std::runtime_error("suit.suit_anim_type names no value");
	}

	if (std::ranges::none_of(suit_body_anim_type_names, [&](const auto& entry) { return entry.second == data.body_anim_type; }))
	{
		throw std::runtime_error("suit.body_anim_type names no value");
	}

	validate(data.bounds_stand);

	validate(data.bounds_crouch);

	validate(data.bounds_prone);

	validate(data.ground_pound_activation_sound);

	validate(data.ground_pound_activation_sound_player);

	validate(data.ground_pound_landing_sound);

	validate(data.ground_pound_landing_sound_player);

	validate(data.skydive_freefall_ambient_sound);

	validate(data.skydive_freefall_high_speed_sound);

	validate(data.skydive_canopy_ambient_sound);

	validate(data.skydive_canopy_high_speed_sound);

	validate(data.nvg_blind_sound);

}

inline void validate(const suit_anim_per_weap_class& data)
{
	if (data.anims)
	{
		for (const auto& item : *data.anims)
		{
			validate(item);
		}
	}

	if (data.gestures)
	{
		for (const auto& item : *data.gestures)
		{
			validate(item);
		}
	}

	for (const auto& item : data.footstep)
	{
		validate(item);
	}

	if ((data.anims && data.anims->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("suit_anim_per_weap_class has arrays that its count does not fit");
	}

	if ((data.gestures && data.gestures->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("suit_anim_per_weap_class has arrays that its count does not fit");
	}

}

inline void validate(const suitanimpackage& data)
{
	if (!data.name)
	{
		throw std::runtime_error("suitanimpackage has no name");
	}

	detail::name(*data.name, "suitanimpackage");

	if (data.anim_overrides)
	{
		for (const auto& item : *data.anim_overrides)
		{
			validate(item);
		}
	}

}

inline void validate(const suit_anim_entry& data)
{
	(void) data;
}

inline void validate(const suit_gesture_entry& data)
{
	if (std::ranges::none_of(gesture_anim_type_t_names, [&](const auto& entry) { return entry.second == data.gesture_anim_type; }))
	{
		throw std::runtime_error("suit_gesture_entry.gesture_anim_type names no value");
	}

}

inline void validate(const coverselector& data)
{
	if (!data.name)
	{
		throw std::runtime_error("coverselector has no name");
	}

	detail::name(*data.name, "coverselector");

}

inline void validate(const enemyselector& data)
{
	if (!data.name)
	{
		throw std::runtime_error("enemyselector has no name");
	}

	detail::name(*data.name, "enemyselector");

}

inline void validate(const carryobject& data)
{
	if (!data.name)
	{
		throw std::runtime_error("carryobject has no name");
	}

	detail::name(*data.name, "carryobject");

	if (std::ranges::none_of(carry_object_type_names, [&](const auto& entry) { return entry.second == data.type; }))
	{
		throw std::runtime_error("carryobject.type names no value");
	}

}

inline void validate(const execution& data)
{
	if (!data.name)
	{
		throw std::runtime_error("execution has no name");
	}

	detail::name(*data.name, "execution");

	if (std::ranges::none_of(weap_class_t_names, [&](const auto& entry) { return entry.second == data.prop_weapon_inventory_override_class; }))
	{
		throw std::runtime_error("execution.prop_weapon_inventory_override_class names no value");
	}

}

inline void validate(const clientcharacter& data)
{
	if (!data.name)
	{
		throw std::runtime_error("clientcharacter has no name");
	}

	detail::name(*data.name, "clientcharacter");

}

inline void validate(const physicslibrary& data)
{
	if (!data.name)
	{
		throw std::runtime_error("physicslibrary has no name");
	}

	detail::name(*data.name, "physicslibrary");

	if ((data.havok_data && data.havok_data->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("physicslibrary has arrays that its count does not fit");
	}

}

inline void validate(const ttf& data)
{
	if (!data.name)
	{
		throw std::runtime_error("ttf has no name");
	}

	detail::name(*data.name, "ttf");

	if (static_cast<std::int64_t>((data.file_len + 1)) != (data.file ? static_cast<std::int64_t>(data.file->size()) : 0))
	{
		throw std::runtime_error("ttf.file has a length that its count does not give");
	}

}

} // namespace gameplay
} // namespace zonetool::asset_data
