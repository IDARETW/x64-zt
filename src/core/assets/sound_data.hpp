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

// Neutral sound assets (docs/NEUTRAL_ASSETS.md), emitted by tools/gen_iw8_neutral.py from the IW8 structs and stream
// traversals: do not edit; change the spec (tools/neutral_families/sound.json) or the generator.
namespace zonetool::asset_data
{
inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 5> adsr_curve_names{{
	{0, "linear"},
	{1, "log_fast"},
	{2, "log_slow"},
	{3, "flat_ended"},
	{4, "flat_middle"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 6> sound_eqtype_names{{
	{0, "first"},
	{1, "highpass"},
	{2, "lowshelf"},
	{3, "highshelf"},
	{4, "bell"},
	{5, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 2> weapon_reflection_room_type_names{{
	{0, "exterior"},
	{1, "interior"},
}};

struct sound_alias
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

	bool operator==(const sound_alias&) const = default;
};

struct sound_alias_list
{
	std::optional<std::string> alias_name{};
	std::uint32_t id{};
	std::optional<std::vector<sound_alias>> head{};
	std::int32_t sequence{};

	bool operator==(const sound_alias_list&) const = default;
};

struct sound_index_entry
{
	std::uint16_t value{};
	std::uint16_t next{};

	bool operator==(const sound_index_entry&) const = default;
};

struct sound_send_effect_params
{
	std::string name{};
	std::uint32_t id{};
	float smoothing{};
	float early_time{};
	float late_time{};
	float early_gain{};
	float late_gain{};
	float late_gain_prox{};
	float return_gain{};
	float early_lpf{};
	float late_lpf{};
	float input_lpf{};
	float damp_lpf{};
	float wall_reflect{};
	float dry_gain{};
	float early_size{};
	float late_size{};
	float diffusion{};
	float rear_level{};
	bool delay1enabled{};
	float delay1delay_time_ms{};
	float delay1_level{};
	float delay1_lpf_cutoff{};
	float delay1_hpf_cutoff{};
	bool delay2enabled{};
	float delay2delay_time_ms{};
	float delay2_level{};
	float delay2_lpf_cutoff{};
	float delay2_hpf_cutoff{};
	bool delay3enabled{};
	float delay3delay_time_ms{};
	float delay3_level{};
	float delay3_lpf_cutoff{};
	float delay3_hpf_cutoff{};
	bool delay4enabled{};
	float delay4delay_time_ms{};
	float delay4_level{};
	float delay4_lpf_cutoff{};
	float delay4_hpf_cutoff{};
	bool delay5enabled{};
	float delay5delay_time_ms{};
	float delay5_level{};
	float delay5_lpf_cutoff{};
	float delay5_hpf_cutoff{};

	bool operator==(const sound_send_effect_params&) const = default;
};

struct adsr_setting
{
	std::optional<std::string> name{};
	std::uint32_t id{};
	float attack_length{};
	std::string attack_curve{};
	float decay_length{};
	std::string decay_curve{};
	float release_length{};
	std::string release_curve{};
	float sustain_level{};
	float sustain_length{};

	bool operator==(const adsr_setting&) const = default;
};

struct ambient_def
{
	std::optional<std::string> name{};
	std::uint32_t id{};
	std::uint16_t ambient_event_index{};
	std::uint16_t num_events{};
	float time_min{};
	float time_max{};

	bool operator==(const ambient_def&) const = default;
};

struct ambient_event
{
	std::uint32_t id{};
	std::uint16_t ambient_element_index{};
	float weight{};

	bool operator==(const ambient_event&) const = default;
};

struct ambient_element
{
	std::uint32_t id{};
	std::optional<std::string> alias_name{};
	float range_min{};
	float range_max{};
	float cone_min{};
	float cone_max{};

	bool operator==(const ambient_element&) const = default;
};

struct filter_def
{
	std::uint32_t id{};
	std::uint8_t ent_channel_idx{};
	std::uint8_t band{};
	std::string type{};
	float freq{};
	float gain{};
	float q{};

	bool operator==(const filter_def&) const = default;
};

struct mix_def
{
	std::uint32_t id{};
	std::uint16_t vol_mod_index{};
	float volume{};
	float fade{};

	bool operator==(const mix_def&) const = default;
};

struct adsr_zone_entry
{
	std::optional<std::string> name{};
	std::uint32_t id{};
	std::optional<std::string> weapon_name{};
	std::int32_t adsr_idx{};
	std::uint32_t weapon_idx{};

	bool operator==(const adsr_zone_entry&) const = default;
};

struct occlude_def
{
	std::uint32_t id{};
	std::uint16_t ent_channel_idx{};
	float cutoff_level1{};
	float cutoff_level2{};
	float cutoff_level3{};
	float cutoff_level4{};
	float attn_level1{};
	float attn_level2{};
	float attn_level3{};
	float attn_level4{};

	bool operator==(const occlude_def&) const = default;
};

struct reverb_def
{
	std::uint32_t id{};
	std::uint32_t room_type_id{};
	std::string room_type{};
	float dry_level{};
	float wet_level{};
	float fade_time{};

	bool operator==(const reverb_def&) const = default;
};

struct timescale_entry
{
	std::uint32_t id{};
	std::uint32_t preset_name{};
	std::int32_t ent_channel_idx{};
	float scale{};

	bool operator==(const timescale_entry&) const = default;
};

struct zone_def
{
	std::uint32_t id{};
	std::uint32_t state_id{};
	std::int16_t reverb_index{};
	std::int16_t start_occlusion_index{};
	std::int16_t num_occlusion{};
	std::int16_t start_filter_index{};
	std::int16_t num_filter{};
	std::int16_t start_mix_index{};
	std::int16_t num_mix{};
	std::optional<std::string> ambient_stream{};
	std::int16_t start_adsr_zone_index{};
	std::int16_t num_adsr_zone{};
	std::int16_t start_npc_adsr_zone_index{};
	std::int16_t num_npc_adsr_zone{};
	std::int16_t ambient_def_index{};
	std::int16_t start_full_occ_index{};
	std::int16_t num_disable_full_occ{};
	std::int16_t start_player_breath_state_index{};
	std::int16_t num_player_breath_states{};
	std::uint32_t weapon_reflection_id{};
	bool exterior{};
	std::optional<std::string> zone_name{};
	std::optional<std::string> state_name{};
	std::optional<std::string> ambient_name{};
	std::optional<std::string> mix_name{};
	std::optional<std::string> reverb_name{};
	std::optional<std::string> filter_name{};
	std::optional<std::string> occlusion_name{};
	std::optional<std::string> player_adsr_name{};
	std::optional<std::string> npc_adsr_name{};
	std::optional<std::string> weapon_reflection_name{};
	std::optional<std::string> full_occ_name{};
	std::optional<std::string> player_breath_state_name{};
	std::uint32_t context_type1{};
	std::uint32_t context_value1{};
	std::uint32_t context_type2{};
	std::uint32_t context_value2{};
	std::uint32_t context_type3{};
	std::uint32_t context_value3{};
	std::uint32_t context_type4{};
	std::uint32_t context_value4{};
	std::uint32_t duck{};

	bool operator==(const zone_def&) const = default;
};

struct full_occlusion_def
{
	std::uint32_t id{};
	std::uint32_t preset_name{};
	std::int32_t ent_channel_idx{};

	bool operator==(const full_occlusion_def&) const = default;
};

struct player_breath_state_def
{
	std::uint32_t state_type{};
	std::optional<std::string> inhale_alias{};
	std::optional<std::string> exhale_alias{};
	std::int32_t min_breaths{};

	bool operator==(const player_breath_state_def&) const = default;
};

struct sound_table
{
	std::uint32_t id{};
	std::optional<std::vector<adsr_setting>> adsr_settings{};
	std::optional<std::vector<ambient_def>> ambient_defs{};
	std::optional<std::vector<ambient_event>> ambient_events{};
	std::optional<std::vector<ambient_element>> ambient_elements{};
	std::optional<std::vector<filter_def>> filters{};
	std::optional<std::vector<mix_def>> mixes{};
	std::optional<std::vector<adsr_zone_entry>> npc_adsr_zones{};
	std::optional<std::vector<occlude_def>> occlusion_filters{};
	std::optional<std::vector<adsr_zone_entry>> player_adsr_zones{};
	std::optional<std::vector<reverb_def>> reverbs{};
	std::optional<std::vector<timescale_entry>> time_scale_settings{};
	std::optional<std::vector<zone_def>> zones{};
	std::optional<std::vector<full_occlusion_def>> full_occlusion_defs{};
	std::optional<std::vector<player_breath_state_def>> player_breath_state_defs{};

	bool operator==(const sound_table&) const = default;
};

struct sound_duck
{
	std::string name{};
	std::uint32_t id{};
	float fade_in{};
	float fade_out{};
	float start_delay{};
	float min_distance{};
	float distance{};
	float length{};
	std::uint32_t fade_in_curve{};
	std::uint32_t fade_out_curve{};
	std::optional<std::array<float, 128>> attenuation{};
	std::optional<std::array<float, 128>> hpf{};
	std::optional<std::array<float, 128>> lpf{};
	std::optional<std::array<float, 128>> focus_amount{};
	std::optional<std::array<std::uint32_t, 128>> focus_cone{};
	bool update_while_paused{};
	bool track_amplitude{};
	bool disable_in_splitscreen{};
	float alias_attenuation{};
	float alias_hpf{};
	float alias_lpf{};
	std::uint32_t duck_alias{};

	bool operator==(const sound_duck&) const = default;
};

struct sound_music_asset
{
	std::string alias{};
	std::uint32_t alias_id{};
	bool inactive{};
	bool complete_loop{};
	bool remove_after_play{};
	bool first_random{};
	bool start_sync{};
	bool stop_sync{};
	bool complete_on_stop{};
	std::uint32_t loop_start_offset{};
	std::int32_t bpm{};
	std::int32_t asset_type{};
	std::int32_t loop_number{};
	std::uint32_t sample_count{};
	std::int32_t start_delay_beats{};
	std::int32_t start_fade_beats{};
	std::int32_t stop_delay_beats{};
	std::int32_t stop_fade_beats{};
	std::int32_t start_offset_frames{};
	std::int32_t meter{};

	bool operator==(const sound_music_asset&) const = default;
};

struct sound_music_state
{
	std::string name{};
	std::uint32_t id{};
	sound_music_asset intro{};
	sound_music_asset exit{};
	std::optional<std::vector<sound_music_asset>> loops{};
	std::int32_t order{};
	std::int32_t interrupt_priority{};
	std::int32_t persist_priority{};
	bool is_random{};
	bool is_sequential{};
	bool skip_previous_exit{};
	bool wall_clock_sync{};

	bool operator==(const sound_music_state&) const = default;
};

struct sound_music_set
{
	std::string name{};
	std::uint32_t id{};
	std::optional<std::vector<sound_music_state>> states{};

	bool operator==(const sound_music_set&) const = default;
};

struct weapon_reflection_def
{
	std::optional<std::string> name{};
	std::uint32_t id{};
	std::optional<std::string> weapon_class{};
	std::uint32_t weapon_class_id{};
	std::optional<std::string> type{};
	std::string zone_type{};
	float zone_type_scalar{};
	float close_med_threshold{};
	float med_far_threshold{};
	float far_threshold{};
	float npc_close_med_threshold{};
	float npc_med_far_threshold{};
	float npc_far_threshold{};
	float speed_of_sound{};
	float max_pitch_close{};
	float pitch_dist_close{};
	float max_pitch_med{};
	float pitch_dist_med{};
	float max_pitch_far{};
	float pitch_dist_far{};
	float npc_max_pitch_med{};
	float npc_pitch_dist_med{};
	float npc_max_pitch_far{};
	float npc_pitch_dist_far{};
	float front_vol{};
	float front_eq{};
	float side_vol{};
	float side_eq{};
	float rear_vol{};
	float rear_eq{};

	bool operator==(const weapon_reflection_def&) const = default;
};

struct doppler_preset
{
	std::string name{};
	std::uint32_t id{};
	float speed_of_sound{};
	float player_velocity_scale{};
	float min_pitch{};
	float max_pitch{};
	float smoothing{};

	bool operator==(const doppler_preset&) const = default;
};

struct occlusion_shape
{
	std::string name{};
	std::uint32_t id{};
	float inner_angle_volume{};
	float outer_angle_volume{};
	float occluded_volume{};
	float inner_angle_lpf{};
	float outer_angle_lpf{};
	float occluded_lpf{};
	float yaw_offset_angle{};
	float pitch_offset_angle{};

	bool operator==(const occlusion_shape&) const = default;
};

struct focus_cone
{
	std::string name{};
	std::uint32_t id{};
	float inner_angle{};
	float outer_angle{};
	float inner_dist{};
	float outer_dist{};
	float attenuation{};

	bool operator==(const focus_cone&) const = default;
};

struct sound_curve
{
	std::string name{};
	std::uint32_t id{};
	std::int32_t num_points{};
	std::array<std::array<float, 2>, 16> points{};

	bool operator==(const sound_curve&) const = default;
};

struct speaker_map
{
	std::string name{};
	std::uint32_t id{};
	std::array<float, 2> mono_to_stereo{};
	std::array<float, 4> stereo_to_stereo{};
	std::array<float, 6> mono_to51{};
	std::array<float, 12> stereo_to51{};
	std::array<float, 8> mono_to71{};
	std::array<float, 16> stereo_to71{};
	std::array<float, 16> mono_to7144{};
	std::array<float, 32> stereo_to7144{};

	bool operator==(const speaker_map&) const = default;
};

struct sound_futz
{
	std::string name{};
	std::uint32_t id{};
	bool enable{};
	float bpf_f{};
	float bpf_q{};
	float ls_g{};
	float ls_f{};
	float ls_q{};
	float dist{};
	float pre_g{};
	float post_g{};
	float th{};
	float tg{};
	float clippre{};
	float clippost{};
	float blend{};
	std::uint32_t start_alias_id{};
	std::uint32_t stop_alias_id{};
	std::uint32_t loop_alias_id{};

	bool operator==(const sound_futz&) const = default;
};

struct sound_bank_stream_info
{
	std::optional<std::string> loaded_stream_key{};
	bool always_loaded{};

	bool operator==(const sound_bank_stream_info&) const = default;
};

struct sound_bank
{
	std::optional<std::string> name{};
	std::optional<std::string> zone{};
	std::optional<std::string> game_language{};
	std::optional<std::string> sound_language{};
	std::optional<std::vector<sound_alias_list>> alias{};
	std::optional<std::vector<sound_index_entry>> alias_index{};
	std::optional<std::vector<sound_send_effect_params>> send_effects{};
	sound_table tables{};
	std::optional<std::vector<sound_duck>> ducks{};
	std::optional<std::vector<sound_music_set>> music_sets{};
	std::optional<std::vector<weapon_reflection_def>> weapon_reflections{};
	std::optional<std::vector<doppler_preset>> doppler_presets{};
	std::optional<std::vector<occlusion_shape>> occlusion_shapes{};
	std::optional<std::vector<focus_cone>> focus_cones{};
	std::optional<std::vector<sound_curve>> volume_falloff_curves{};
	std::optional<std::vector<sound_curve>> lpf_curves{};
	std::optional<std::vector<sound_curve>> hpf_curves{};
	std::optional<std::vector<sound_curve>> reverb_curves{};
	std::optional<std::vector<speaker_map>> speaker_maps{};
	std::optional<std::vector<sound_futz>> futzs{};
	sound_bank_stream_info stream_info{};

	bool operator==(const sound_bank&) const = default;
};

struct sound_bank_resident
{
	sound_bank bank{};

	bool operator==(const sound_bank_resident&) const = default;
};

struct sound_bank_transient
{
	sound_bank bank{};

	bool operator==(const sound_bank_transient&) const = default;
};

inline void validate(const sound_alias& data)
{
	(void) data;
}

inline void validate(const sound_alias_list& data)
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
		throw std::runtime_error("sound_alias_list has arrays that its count does not fit");
	}

}

inline void validate(const sound_index_entry& data)
{
	(void) data;
}

inline void validate(const sound_send_effect_params& data)
{
	if (data.name.size() >= 64)
	{
		throw std::runtime_error("sound_send_effect_params.name does not fit its 64 character buffer");
	}

}

inline void validate(const adsr_setting& data)
{
	if (std::ranges::none_of(adsr_curve_names, [&](const auto& entry) { return entry.second == data.attack_curve; }))
	{
		throw std::runtime_error("adsr_setting.attack_curve names no value");
	}

	if (std::ranges::none_of(adsr_curve_names, [&](const auto& entry) { return entry.second == data.decay_curve; }))
	{
		throw std::runtime_error("adsr_setting.decay_curve names no value");
	}

	if (std::ranges::none_of(adsr_curve_names, [&](const auto& entry) { return entry.second == data.release_curve; }))
	{
		throw std::runtime_error("adsr_setting.release_curve names no value");
	}

}

inline void validate(const ambient_def& data)
{
	(void) data;
}

inline void validate(const ambient_event& data)
{
	(void) data;
}

inline void validate(const ambient_element& data)
{
	(void) data;
}

inline void validate(const filter_def& data)
{
	if (std::ranges::none_of(sound_eqtype_names, [&](const auto& entry) { return entry.second == data.type; }))
	{
		throw std::runtime_error("filter_def.type names no value");
	}

}

inline void validate(const mix_def& data)
{
	(void) data;
}

inline void validate(const adsr_zone_entry& data)
{
	(void) data;
}

inline void validate(const occlude_def& data)
{
	(void) data;
}

inline void validate(const reverb_def& data)
{
	if (data.room_type.size() >= 64)
	{
		throw std::runtime_error("reverb_def.room_type does not fit its 64 character buffer");
	}

}

inline void validate(const timescale_entry& data)
{
	(void) data;
}

inline void validate(const zone_def& data)
{
	(void) data;
}

inline void validate(const full_occlusion_def& data)
{
	(void) data;
}

inline void validate(const player_breath_state_def& data)
{
	(void) data;
}

inline void validate(const sound_table& data)
{
	if (data.adsr_settings)
	{
		for (const auto& item : *data.adsr_settings)
		{
			validate(item);
		}
	}

	if (data.ambient_defs)
	{
		for (const auto& item : *data.ambient_defs)
		{
			validate(item);
		}
	}

	if (data.ambient_events)
	{
		for (const auto& item : *data.ambient_events)
		{
			validate(item);
		}
	}

	if (data.ambient_elements)
	{
		for (const auto& item : *data.ambient_elements)
		{
			validate(item);
		}
	}

	if (data.filters)
	{
		for (const auto& item : *data.filters)
		{
			validate(item);
		}
	}

	if (data.mixes)
	{
		for (const auto& item : *data.mixes)
		{
			validate(item);
		}
	}

	if (data.npc_adsr_zones)
	{
		for (const auto& item : *data.npc_adsr_zones)
		{
			validate(item);
		}
	}

	if (data.occlusion_filters)
	{
		for (const auto& item : *data.occlusion_filters)
		{
			validate(item);
		}
	}

	if (data.player_adsr_zones)
	{
		for (const auto& item : *data.player_adsr_zones)
		{
			validate(item);
		}
	}

	if (data.reverbs)
	{
		for (const auto& item : *data.reverbs)
		{
			validate(item);
		}
	}

	if (data.time_scale_settings)
	{
		for (const auto& item : *data.time_scale_settings)
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

	if (data.full_occlusion_defs)
	{
		for (const auto& item : *data.full_occlusion_defs)
		{
			validate(item);
		}
	}

	if (data.player_breath_state_defs)
	{
		for (const auto& item : *data.player_breath_state_defs)
		{
			validate(item);
		}
	}

	if ((data.adsr_settings && data.adsr_settings->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("sound_table has arrays that its count does not fit");
	}

	if ((data.ambient_defs && data.ambient_defs->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("sound_table has arrays that its count does not fit");
	}

	if ((data.ambient_events && data.ambient_events->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("sound_table has arrays that its count does not fit");
	}

	if ((data.ambient_elements && data.ambient_elements->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("sound_table has arrays that its count does not fit");
	}

	if ((data.filters && data.filters->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("sound_table has arrays that its count does not fit");
	}

	if ((data.mixes && data.mixes->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("sound_table has arrays that its count does not fit");
	}

	if ((data.npc_adsr_zones && data.npc_adsr_zones->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("sound_table has arrays that its count does not fit");
	}

	if ((data.occlusion_filters && data.occlusion_filters->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("sound_table has arrays that its count does not fit");
	}

	if ((data.player_adsr_zones && data.player_adsr_zones->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("sound_table has arrays that its count does not fit");
	}

	if ((data.reverbs && data.reverbs->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("sound_table has arrays that its count does not fit");
	}

	if ((data.time_scale_settings && data.time_scale_settings->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("sound_table has arrays that its count does not fit");
	}

	if ((data.zones && data.zones->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("sound_table has arrays that its count does not fit");
	}

	if ((data.full_occlusion_defs && data.full_occlusion_defs->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("sound_table has arrays that its count does not fit");
	}

	if ((data.player_breath_state_defs && data.player_breath_state_defs->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("sound_table has arrays that its count does not fit");
	}

}

inline void validate(const sound_duck& data)
{
	if (data.name.size() >= 64)
	{
		throw std::runtime_error("sound_duck.name does not fit its 64 character buffer");
	}

}

inline void validate(const sound_music_asset& data)
{
	if (data.alias.size() >= 64)
	{
		throw std::runtime_error("sound_music_asset.alias does not fit its 64 character buffer");
	}

}

inline void validate(const sound_music_state& data)
{
	if (data.name.size() >= 64)
	{
		throw std::runtime_error("sound_music_state.name does not fit its 64 character buffer");
	}

	validate(data.intro);

	validate(data.exit);

	if (data.loops)
	{
		for (const auto& item : *data.loops)
		{
			validate(item);
		}
	}

	if ((data.loops && data.loops->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("sound_music_state has arrays that its count does not fit");
	}

}

inline void validate(const sound_music_set& data)
{
	if (data.name.size() >= 64)
	{
		throw std::runtime_error("sound_music_set.name does not fit its 64 character buffer");
	}

	if (data.states)
	{
		for (const auto& item : *data.states)
		{
			validate(item);
		}
	}

	if ((data.states && data.states->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("sound_music_set has arrays that its count does not fit");
	}

}

inline void validate(const weapon_reflection_def& data)
{
	if (std::ranges::none_of(weapon_reflection_room_type_names, [&](const auto& entry) { return entry.second == data.zone_type; }))
	{
		throw std::runtime_error("weapon_reflection_def.zone_type names no value");
	}

}

inline void validate(const doppler_preset& data)
{
	if (data.name.size() >= 64)
	{
		throw std::runtime_error("doppler_preset.name does not fit its 64 character buffer");
	}

}

inline void validate(const occlusion_shape& data)
{
	if (data.name.size() >= 64)
	{
		throw std::runtime_error("occlusion_shape.name does not fit its 64 character buffer");
	}

}

inline void validate(const focus_cone& data)
{
	if (data.name.size() >= 64)
	{
		throw std::runtime_error("focus_cone.name does not fit its 64 character buffer");
	}

}

inline void validate(const sound_curve& data)
{
	if (data.name.size() >= 64)
	{
		throw std::runtime_error("sound_curve.name does not fit its 64 character buffer");
	}

}

inline void validate(const speaker_map& data)
{
	if (data.name.size() >= 64)
	{
		throw std::runtime_error("speaker_map.name does not fit its 64 character buffer");
	}

}

inline void validate(const sound_futz& data)
{
	if (data.name.size() >= 64)
	{
		throw std::runtime_error("sound_futz.name does not fit its 64 character buffer");
	}

}

inline void validate(const sound_bank_stream_info& data)
{
	(void) data;
}

inline void validate(const sound_bank& data)
{
	if (data.alias)
	{
		for (const auto& item : *data.alias)
		{
			validate(item);
		}
	}

	if (data.alias_index)
	{
		for (const auto& item : *data.alias_index)
		{
			validate(item);
		}
	}

	if (data.send_effects)
	{
		for (const auto& item : *data.send_effects)
		{
			validate(item);
		}
	}

	validate(data.tables);

	if (data.ducks)
	{
		for (const auto& item : *data.ducks)
		{
			validate(item);
		}
	}

	if (data.music_sets)
	{
		for (const auto& item : *data.music_sets)
		{
			validate(item);
		}
	}

	if (data.weapon_reflections)
	{
		for (const auto& item : *data.weapon_reflections)
		{
			validate(item);
		}
	}

	if (data.doppler_presets)
	{
		for (const auto& item : *data.doppler_presets)
		{
			validate(item);
		}
	}

	if (data.occlusion_shapes)
	{
		for (const auto& item : *data.occlusion_shapes)
		{
			validate(item);
		}
	}

	if (data.focus_cones)
	{
		for (const auto& item : *data.focus_cones)
		{
			validate(item);
		}
	}

	if (data.volume_falloff_curves)
	{
		for (const auto& item : *data.volume_falloff_curves)
		{
			validate(item);
		}
	}

	if (data.lpf_curves)
	{
		for (const auto& item : *data.lpf_curves)
		{
			validate(item);
		}
	}

	if (data.hpf_curves)
	{
		for (const auto& item : *data.hpf_curves)
		{
			validate(item);
		}
	}

	if (data.reverb_curves)
	{
		for (const auto& item : *data.reverb_curves)
		{
			validate(item);
		}
	}

	if (data.speaker_maps)
	{
		for (const auto& item : *data.speaker_maps)
		{
			validate(item);
		}
	}

	if (data.futzs)
	{
		for (const auto& item : *data.futzs)
		{
			validate(item);
		}
	}

	validate(data.stream_info);

	if ((data.alias && data.alias->size() > 0xFFFFFFFFull) || (data.alias_index && data.alias_index->size() > 0xFFFFFFFFull) || (!data.alias != !data.alias_index) || (data.alias && data.alias_index && data.alias->size() != data.alias_index->size()))
	{
		throw std::runtime_error("sound_bank has arrays that its count does not fit");
	}

	if ((data.send_effects && data.send_effects->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("sound_bank has arrays that its count does not fit");
	}

	if ((data.ducks && data.ducks->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("sound_bank has arrays that its count does not fit");
	}

	if ((data.music_sets && data.music_sets->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("sound_bank has arrays that its count does not fit");
	}

	if ((data.weapon_reflections && data.weapon_reflections->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("sound_bank has arrays that its count does not fit");
	}

	if ((data.doppler_presets && data.doppler_presets->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("sound_bank has arrays that its count does not fit");
	}

	if ((data.occlusion_shapes && data.occlusion_shapes->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("sound_bank has arrays that its count does not fit");
	}

	if ((data.focus_cones && data.focus_cones->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("sound_bank has arrays that its count does not fit");
	}

	if ((data.volume_falloff_curves && data.volume_falloff_curves->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("sound_bank has arrays that its count does not fit");
	}

	if ((data.lpf_curves && data.lpf_curves->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("sound_bank has arrays that its count does not fit");
	}

	if ((data.hpf_curves && data.hpf_curves->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("sound_bank has arrays that its count does not fit");
	}

	if ((data.reverb_curves && data.reverb_curves->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("sound_bank has arrays that its count does not fit");
	}

	if ((data.speaker_maps && data.speaker_maps->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("sound_bank has arrays that its count does not fit");
	}

	if ((data.futzs && data.futzs->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("sound_bank has arrays that its count does not fit");
	}

}

inline void validate(const sound_bank_resident& data)
{
	if (!data.bank.name)
	{
		throw std::runtime_error("sound_bank_resident has no name");
	}

	detail::name(*data.bank.name, "sound_bank_resident");

	validate(data.bank);

}

inline void validate(const sound_bank_transient& data)
{
	if (!data.bank.name)
	{
		throw std::runtime_error("sound_bank_transient has no name");
	}

	detail::name(*data.bank.name, "sound_bank_transient");

	validate(data.bank);

}

} // namespace zonetool::asset_data
