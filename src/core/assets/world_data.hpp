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

} // namespace zonetool::asset_data
