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

// Neutral sound_globals assets (docs/NEUTRAL_ASSETS.md), emitted by tools/gen_iw8_neutral.py from the IW8 structs and stream
// traversals: do not edit; change the spec (tools/neutral_families/sound_globals.json) or the generator.
namespace zonetool::asset_data
{
inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 3> sound_entity_channel_spaital_type_names{{
	{0, "2_d"},
	{1, "3_d"},
	{2, "pa_speaker"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 5> sound_limit_behavior_names{{
	{0, "no_new"},
	{1, "replace_quieter"},
	{2, "replace_oldest"},
	{3, "replace_farthest"},
	{4, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 3> sound_far_reverb_behavior_names{{
	{0, "none"},
	{1, "full"},
	{2, "wet_only"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 3> sound_send_type_names{{
	{0, "reverb"},
	{1, "delay"},
	{2, "none"},
}};

struct sound_volume_modifier
{
	std::string name{};
	std::uint32_t id{};
	float value{};

	bool operator==(const sound_volume_modifier&) const = default;
};

struct sound_entity_channel
{
	std::string name{};
	std::uint32_t id{};
	std::int32_t priority{};
	std::string spatial_type{};
	bool is_restricted{};
	bool is_pausable{};
	std::int32_t max_voices{};
	std::string limit_behavior{};
	bool center_speaker_pan{};
	std::string mp_far_reverb_behavior{};
	std::string sp_far_reverb_behavior{};
	std::string cp_far_reverb_behavior{};
	bool has71_pan{};
	std::int32_t bus_id{};
	bool is_player_dialogue{};
	std::string send_type{};
	float speed_of_sound_mult{};

	bool operator==(const sound_entity_channel&) const = default;
};

struct sound_context
{
	std::optional<std::string> type{};
	std::uint32_t type_id{};
	std::int32_t num_values{};
	std::int32_t start_index{};
	bool allow_defaults{};
	bool include_defaults_in_random{};
	bool can_blend{};

	bool operator==(const sound_context&) const = default;
};

struct sound_context_value
{
	std::optional<std::string> value{};
	std::uint32_t value_id{};

	bool operator==(const sound_context_value&) const = default;
};

struct sound_master_eq_param
{
	std::int32_t enabled{};
	float gain{};
	float frequency{};
	float q{};

	bool operator==(const sound_master_eq_param&) const = default;
};

struct sound_master_eq_param_bank
{
	sound_master_eq_param low{};
	sound_master_eq_param peak1{};
	sound_master_eq_param peak2{};
	sound_master_eq_param high{};

	bool operator==(const sound_master_eq_param_bank&) const = default;
};

struct sound_dynamics_params
{
	std::int32_t enable{};
	float pre_gain{};
	float post_gain{};
	float threshold{};
	float ratio{};
	float time_attack{};
	float time_release{};
	float spread{};
	std::int32_t linear{};
	std::int32_t itu1770{};

	bool operator==(const sound_dynamics_params&) const = default;
};

struct sound_peak_limiter_params
{
	std::int32_t enable{};
	float input_gain{};
	float output_gain{};
	float threshold{};
	float release_min{};
	float release_max{};

	bool operator==(const sound_peak_limiter_params&) const = default;
};

struct sound_master
{
	std::string name{};
	std::uint32_t id{};
	sound_master_eq_param_bank dialog_eq{};
	sound_master_eq_param_bank weap_exp_scenes_eq{};
	sound_master_eq_param_bank sfx_eq{};
	sound_master_eq_param_bank main_eq{};
	sound_dynamics_params dialog_dyn{};
	sound_dynamics_params sfx_dyn{};
	sound_dynamics_params main_dyn{};
	sound_peak_limiter_params dialog_peak_limit{};
	sound_peak_limiter_params sfx_peak_limit{};
	sound_peak_limiter_params main_peak_limit{};
	float bus_dialog_gain{};
	float bus_weap_exp_scene_gain{};
	float bus_sfx_gain{};
	float bus_reverb_gain{};
	float bus_music_gain{};
	float bus_movie_gain{};
	float bus_reference_gain{};
	std::int32_t bus_dialog_enable{};
	std::int32_t bus_weap_exp_scene_enable{};
	std::int32_t bus_sfx_enable{};
	std::int32_t bus_reverb_enable{};
	std::int32_t bus_music_enable{};
	std::int32_t bus_movie_enable{};
	std::int32_t bus_reference_enable{};

	bool operator==(const sound_master&) const = default;
};

struct sound_whizby
{
	std::string name{};
	std::uint32_t id{};
	float radius_near{};
	float radius_medium{};
	float radius_far{};
	float spread_near{};
	float spread_medium{};
	float spread_far{};
	float whizby_in_time_near{};
	float whizby_in_time_medium{};
	float whizby_in_time_far{};
	float shooter_dist_near{};
	float shooter_dist_medium{};
	float shooter_dist_far{};
	bool is_super_sonic{};
	float crack_max_angle{};
	float bullet_speed{};
	float speed_of_sound{};

	bool operator==(const sound_whizby&) const = default;
};

struct sound_perk
{
	std::string name{};
	std::uint32_t id{};
	float volume{};

	bool operator==(const sound_perk&) const = default;
};

struct sound_globals
{
	std::optional<std::string> name{};
	float global_volume_modifier{};
	float bink_volume_modifier{};
	std::optional<std::vector<sound_volume_modifier>> volmodinfo{};
	std::optional<std::vector<sound_entity_channel>> entity_channel_info{};
	std::optional<std::vector<sound_context>> contexts{};
	std::optional<std::vector<sound_context_value>> context_values{};
	std::optional<std::vector<sound_master>> masters{};
	std::optional<std::vector<sound_whizby>> whizbys{};
	std::optional<std::vector<sound_perk>> perkinfo{};

	bool operator==(const sound_globals&) const = default;
};

inline void validate(const sound_volume_modifier& data)
{
	if (data.name.size() >= 64)
	{
		throw std::runtime_error("sound_volume_modifier.name does not fit its 64 character buffer");
	}

}

inline void validate(const sound_entity_channel& data)
{
	if (data.name.size() >= 64)
	{
		throw std::runtime_error("sound_entity_channel.name does not fit its 64 character buffer");
	}

	if (std::ranges::none_of(sound_entity_channel_spaital_type_names, [&](const auto& entry) { return entry.second == data.spatial_type; }))
	{
		throw std::runtime_error("sound_entity_channel.spatial_type names no value");
	}

	if (std::ranges::none_of(sound_limit_behavior_names, [&](const auto& entry) { return entry.second == data.limit_behavior; }))
	{
		throw std::runtime_error("sound_entity_channel.limit_behavior names no value");
	}

	if (std::ranges::none_of(sound_far_reverb_behavior_names, [&](const auto& entry) { return entry.second == data.mp_far_reverb_behavior; }))
	{
		throw std::runtime_error("sound_entity_channel.mp_far_reverb_behavior names no value");
	}

	if (std::ranges::none_of(sound_far_reverb_behavior_names, [&](const auto& entry) { return entry.second == data.sp_far_reverb_behavior; }))
	{
		throw std::runtime_error("sound_entity_channel.sp_far_reverb_behavior names no value");
	}

	if (std::ranges::none_of(sound_far_reverb_behavior_names, [&](const auto& entry) { return entry.second == data.cp_far_reverb_behavior; }))
	{
		throw std::runtime_error("sound_entity_channel.cp_far_reverb_behavior names no value");
	}

	if (std::ranges::none_of(sound_send_type_names, [&](const auto& entry) { return entry.second == data.send_type; }))
	{
		throw std::runtime_error("sound_entity_channel.send_type names no value");
	}

}

inline void validate(const sound_context& data)
{
	(void) data;
}

inline void validate(const sound_context_value& data)
{
	(void) data;
}

inline void validate(const sound_master_eq_param& data)
{
	(void) data;
}

inline void validate(const sound_master_eq_param_bank& data)
{
	validate(data.low);

	validate(data.peak1);

	validate(data.peak2);

	validate(data.high);

}

inline void validate(const sound_dynamics_params& data)
{
	(void) data;
}

inline void validate(const sound_peak_limiter_params& data)
{
	(void) data;
}

inline void validate(const sound_master& data)
{
	if (data.name.size() >= 64)
	{
		throw std::runtime_error("sound_master.name does not fit its 64 character buffer");
	}

	validate(data.dialog_eq);

	validate(data.weap_exp_scenes_eq);

	validate(data.sfx_eq);

	validate(data.main_eq);

	validate(data.dialog_dyn);

	validate(data.sfx_dyn);

	validate(data.main_dyn);

	validate(data.dialog_peak_limit);

	validate(data.sfx_peak_limit);

	validate(data.main_peak_limit);

}

inline void validate(const sound_whizby& data)
{
	if (data.name.size() >= 64)
	{
		throw std::runtime_error("sound_whizby.name does not fit its 64 character buffer");
	}

}

inline void validate(const sound_perk& data)
{
	if (data.name.size() >= 64)
	{
		throw std::runtime_error("sound_perk.name does not fit its 64 character buffer");
	}

}

inline void validate(const sound_globals& data)
{
	if (!data.name)
	{
		throw std::runtime_error("sound_globals has no name");
	}

	detail::name(*data.name, "sound_globals");

	if (data.volmodinfo)
	{
		for (const auto& item : *data.volmodinfo)
		{
			validate(item);
		}
	}

	if (data.entity_channel_info)
	{
		for (const auto& item : *data.entity_channel_info)
		{
			validate(item);
		}
	}

	if (data.contexts)
	{
		for (const auto& item : *data.contexts)
		{
			validate(item);
		}
	}

	if (data.context_values)
	{
		for (const auto& item : *data.context_values)
		{
			validate(item);
		}
	}

	if (data.masters)
	{
		for (const auto& item : *data.masters)
		{
			validate(item);
		}
	}

	if (data.whizbys)
	{
		for (const auto& item : *data.whizbys)
		{
			validate(item);
		}
	}

	if (data.perkinfo)
	{
		for (const auto& item : *data.perkinfo)
		{
			validate(item);
		}
	}

	if ((data.volmodinfo && data.volmodinfo->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("sound_globals has arrays that its count does not fit");
	}

	if ((data.entity_channel_info && data.entity_channel_info->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("sound_globals has arrays that its count does not fit");
	}

	if ((data.contexts && data.contexts->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("sound_globals has arrays that its count does not fit");
	}

	if ((data.context_values && data.context_values->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("sound_globals has arrays that its count does not fit");
	}

	if ((data.masters && data.masters->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("sound_globals has arrays that its count does not fit");
	}

	if ((data.whizbys && data.whizbys->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("sound_globals has arrays that its count does not fit");
	}

	if ((data.perkinfo && data.perkinfo->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("sound_globals has arrays that its count does not fit");
	}

}

} // namespace zonetool::asset_data
