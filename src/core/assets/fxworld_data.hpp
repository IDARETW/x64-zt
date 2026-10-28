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

// Neutral fxworld assets (docs/NEUTRAL_ASSETS.md), emitted by tools/gen_iw8_neutral.py from the IW8 structs and stream
// traversals: do not edit; change the spec (tools/neutral_families/fxworld.json) or the generator.
namespace zonetool::asset_data
{
namespace fxworld
{
struct fx_world;
struct fx_glass_globals;
struct fx_glass_def;
struct fx_combined_def;
struct snd_alias_lookup;
struct fx_glass_transient_lookup;
struct fx_glass_system;
struct fx_glass_piece_state;
struct fx_glass_piece_dynamics;
struct fx_glass_init_piece_state;
struct fx_spatial_frame;
struct fx_world_transient_zone;

inline void validate(const fx_world& data);
inline void validate(const fx_glass_globals& data);
inline void validate(const fx_glass_def& data);
inline void validate(const fx_combined_def& data);
inline void validate(const snd_alias_lookup& data);
inline void validate(const fx_glass_transient_lookup& data);
inline void validate(const fx_glass_system& data);
inline void validate(const fx_glass_piece_state& data);
inline void validate(const fx_glass_piece_dynamics& data);
inline void validate(const fx_glass_init_piece_state& data);
inline void validate(const fx_spatial_frame& data);
inline void validate(const fx_world_transient_zone& data);

struct fx_glass_globals
{
	std::uint32_t def_count{};
	std::optional<std::vector<fx_glass_def>> defs{};
	std::optional<std::vector<fx_glass_transient_lookup>> glass_transient_lookup{};
	std::optional<std::vector<float>> distances{};

	bool operator==(const fx_glass_globals&) const = default;
};

struct fx_glass_system
{
	std::int32_t time{};
	std::int32_t prev_time{};
	std::uint32_t cell_count{};
	std::uint32_t active_piece_count{};
	std::uint32_t first_free_piece{};
	std::uint32_t geo_data_count{};
	std::optional<std::vector<std::uint32_t>> piece_places{};
	std::optional<std::vector<fx_glass_piece_state>> piece_states{};
	std::optional<std::vector<fx_glass_piece_dynamics>> piece_dynamics{};
	std::optional<std::vector<std::array<std::uint8_t, 4>>> geo_data{};
	std::optional<std::vector<std::uint32_t>> is_in_use{};
	std::optional<std::vector<std::uint32_t>> cell_bits{};
	std::optional<std::vector<std::uint8_t>> vis_data{};
	std::optional<std::vector<std::array<float, 3>>> link_org{};
	std::optional<std::vector<float>> half_thickness{};
	std::optional<std::vector<std::array<std::uint8_t, 4>>> init_geo_data{};
	bool need_to_compact_data{};
	std::uint8_t init_count{};
	float effect_chance_accum{};
	std::int32_t last_piece_deletion_time{};
	std::optional<std::vector<fx_glass_init_piece_state>> init_piece_states{};

	bool operator==(const fx_glass_system&) const = default;
};

struct fx_world
{
	std::optional<std::string> name{};
	fx_glass_globals glass_glob{};
	fx_glass_system glass_sys{};
	std::optional<std::vector<std::optional<std::string>>> transient_zones{};
	std::array<std::uint16_t, 1536> active_transient_zones{};
	std::uint16_t active_transient_zone_count{};

	bool operator==(const fx_world&) const = default;
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

struct fx_glass_def
{
	float half_thickness{};
	std::array<std::array<float, 2>, 2> tex_vecs{};
	std::array<std::uint8_t, 4> color{};
	std::optional<std::string> material{};
	std::optional<std::string> material_shattered{};
	std::optional<std::string> physics_asset{};
	fx_combined_def piece_break_effect{};
	fx_combined_def shatter_effect{};
	fx_combined_def shatter_small_effect{};
	fx_combined_def crack_decal_effect{};
	snd_alias_lookup damaged_sound{};
	snd_alias_lookup destroyed_sound{};
	snd_alias_lookup destroyed_quiet_sound{};
	float high_mip_radius_inv_sq{};
	float shattered_high_mip_radius_inv_sq{};
	std::int32_t num_crack_rings{};
	std::uint16_t transient_index{};

	bool operator==(const fx_glass_def&) const = default;
};

struct fx_glass_transient_lookup
{
	std::uint16_t global_piece_start_index{};
	std::uint16_t transient_index{};

	bool operator==(const fx_glass_transient_lookup&) const = default;
};

struct fx_glass_piece_state
{
	std::array<float, 2> tex_coord_origin{};
	std::uint32_t support_mask{};
	std::uint32_t geo_data_start{};
	std::uint16_t init_index{};
	std::uint8_t def_index{};
	std::uint8_t vert_count{};
	std::uint8_t hole_data_count{};
	std::uint8_t crack_data_count{};
	std::uint8_t fan_data_count{};
	std::uint16_t flags{};
	float area_x2{};

	bool operator==(const fx_glass_piece_state&) const = default;
};

struct fx_glass_piece_dynamics
{
	std::int32_t fall_time{};
	std::uint32_t physics_instance{};
	std::uint32_t physics_detail_instance{};
	std::array<float, 3> vel{};
	std::array<float, 3> avel{};
	bool pending_creation{};

	bool operator==(const fx_glass_piece_dynamics&) const = default;
};

struct fx_spatial_frame
{
	std::array<float, 4> quat{};
	std::array<float, 3> origin{};

	bool operator==(const fx_spatial_frame&) const = default;
};

struct fx_glass_init_piece_state
{
	fx_spatial_frame frame{};
	float radius{};
	std::array<float, 2> tex_coord_origin{};
	std::uint32_t support_mask{};
	float area_x2{};
	std::uint8_t def_index{};
	std::uint8_t vert_count{};
	std::uint8_t fan_data_count{};

	bool operator==(const fx_glass_init_piece_state&) const = default;
};

struct fx_world_transient_zone
{
	std::optional<std::string> name{};
	std::uint32_t transient_zone_index{};
	fx_glass_system glass_sys{};

	bool operator==(const fx_world_transient_zone&) const = default;
};

inline void validate(const fx_glass_globals& data)
{
	if (data.defs)
	{
		for (const auto& item : *data.defs)
		{
			validate(item);
		}
	}

	if (data.glass_transient_lookup)
	{
		for (const auto& item : *data.glass_transient_lookup)
		{
			validate(item);
		}
	}

	if (static_cast<std::int64_t>(data.def_count) != (data.defs ? static_cast<std::int64_t>(data.defs->size()) : 0))
	{
		throw std::runtime_error("fx_glass_globals.defs has a length that its count does not give");
	}

	if (static_cast<std::int64_t>(data.def_count) != (data.distances ? static_cast<std::int64_t>(data.distances->size()) : 0))
	{
		throw std::runtime_error("fx_glass_globals.distances has a length that its count does not give");
	}

	if ((data.glass_transient_lookup && data.glass_transient_lookup->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("fx_glass_globals has arrays that its count does not fit");
	}

}

inline void validate(const fx_glass_system& data)
{
	if (data.piece_states)
	{
		for (const auto& item : *data.piece_states)
		{
			validate(item);
		}
	}

	if (data.piece_dynamics)
	{
		for (const auto& item : *data.piece_dynamics)
		{
			validate(item);
		}
	}

	if (data.init_piece_states)
	{
		for (const auto& item : *data.init_piece_states)
		{
			validate(item);
		}
	}

	if (static_cast<std::int64_t>((data.cell_count * (data.is_in_use ? data.is_in_use->size() : std::size_t{}))) != (data.cell_bits ? static_cast<std::int64_t>(data.cell_bits->size()) : 0))
	{
		throw std::runtime_error("fx_glass_system.cell_bits has a length that its count does not give");
	}

	if (static_cast<std::int64_t>((((data.piece_places ? data.piece_places->size() : std::size_t{}) + 15) & 0xFFFFFFF0ull)) != (data.vis_data ? static_cast<std::int64_t>(data.vis_data->size()) : 0))
	{
		throw std::runtime_error("fx_glass_system.vis_data has a length that its count does not give");
	}

	if (static_cast<std::int64_t>((((data.piece_places ? data.piece_places->size() : std::size_t{}) + 3) & 0xFFFFFFFCull)) != (data.half_thickness ? static_cast<std::int64_t>(data.half_thickness->size()) : 0))
	{
		throw std::runtime_error("fx_glass_system.half_thickness has a length that its count does not give");
	}

	if ((data.piece_places && data.piece_places->size() > 0xFFFFFFFFull) || (data.piece_states && data.piece_states->size() > 0xFFFFFFFFull) || (data.piece_dynamics && data.piece_dynamics->size() > 0xFFFFFFFFull) || (data.link_org && data.link_org->size() > 0xFFFFFFFFull) || (!data.piece_places != !data.piece_states) || (data.piece_places && data.piece_states && data.piece_places->size() != data.piece_states->size()) || (!data.piece_places != !data.piece_dynamics) || (data.piece_places && data.piece_dynamics && data.piece_places->size() != data.piece_dynamics->size()) || (!data.piece_places != !data.link_org) || (data.piece_places && data.link_org && data.piece_places->size() != data.link_org->size()))
	{
		throw std::runtime_error("fx_glass_system has arrays that its count does not fit");
	}

	if ((data.geo_data && data.geo_data->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("fx_glass_system has arrays that its count does not fit");
	}

	if ((data.is_in_use && data.is_in_use->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("fx_glass_system has arrays that its count does not fit");
	}

	if ((data.init_geo_data && data.init_geo_data->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("fx_glass_system has arrays that its count does not fit");
	}

	if ((data.init_piece_states && data.init_piece_states->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("fx_glass_system has arrays that its count does not fit");
	}

}

inline void validate(const fx_world& data)
{
	if (!data.name)
	{
		throw std::runtime_error("fx_world has no name");
	}

	detail::name(*data.name, "fx_world");

	validate(data.glass_glob);

	validate(data.glass_sys);

	if ((data.transient_zones && data.transient_zones->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("fx_world has arrays that its count does not fit");
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

inline void validate(const fx_glass_def& data)
{
	validate(data.piece_break_effect);

	validate(data.shatter_effect);

	validate(data.shatter_small_effect);

	validate(data.crack_decal_effect);

	validate(data.damaged_sound);

	validate(data.destroyed_sound);

	validate(data.destroyed_quiet_sound);

}

inline void validate(const fx_glass_transient_lookup& data)
{
	(void) data;
}

inline void validate(const fx_glass_piece_state& data)
{
	(void) data;
}

inline void validate(const fx_glass_piece_dynamics& data)
{
	(void) data;
}

inline void validate(const fx_spatial_frame& data)
{
	(void) data;
}

inline void validate(const fx_glass_init_piece_state& data)
{
	validate(data.frame);

}

inline void validate(const fx_world_transient_zone& data)
{
	if (!data.name)
	{
		throw std::runtime_error("fx_world_transient_zone has no name");
	}

	detail::name(*data.name, "fx_world_transient_zone");

	validate(data.glass_sys);

}

} // namespace fxworld
} // namespace zonetool::asset_data
