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

// Neutral effects assets (docs/NEUTRAL_ASSETS.md), emitted by tools/gen_iw8_neutral.py from the IW8 structs and stream
// traversals: do not edit; change the spec (tools/neutral_families/effects.json) or the generator.
namespace zonetool::asset_data
{
namespace effects
{
struct particle_sim_animation;
struct fx_particle_sim_animation_header;
struct fx_particle_sim_animation_particle_data;
struct fx_particle_sim_animation_particle_data_orientation3_d;
struct fx_particle_sim_animation_particle_data_index;
struct fx_particle_sim_animation_frame;

inline void validate(const particle_sim_animation& data);
inline void validate(const fx_particle_sim_animation_header& data);
inline void validate(const fx_particle_sim_animation_particle_data& data);
inline void validate(const fx_particle_sim_animation_particle_data_orientation3_d& data);
inline void validate(const fx_particle_sim_animation_particle_data_index& data);
inline void validate(const fx_particle_sim_animation_frame& data);

struct fx_particle_sim_animation_header
{
	float playback_rate{};
	float duration{};
	std::uint32_t frame_count{};
	float min_x{};
	float min_y{};
	float min_z{};
	float bounds_x_delta{};
	float bounds_y_delta{};
	float bounds_z_delta{};
	float max_width{};
	float max_height{};
	std::uint32_t color_table_size{};
	std::uint32_t particle_data_count{};
	std::uint32_t max_active_particles{};
	bool eval_vis_state_per_particle{};
	bool sort_particles_at_runtime{};
	bool has_orientation3_d{};
	bool has_index{};

	bool operator==(const fx_particle_sim_animation_header&) const = default;
};

struct particle_sim_animation
{
	std::optional<std::string> name{};
	std::optional<std::string> material{};
	fx_particle_sim_animation_header header{};
	std::optional<std::vector<fx_particle_sim_animation_particle_data>> particle_data{};
	std::optional<std::vector<fx_particle_sim_animation_particle_data_orientation3_d>> particle_data_orientation3_d{};
	std::optional<std::vector<fx_particle_sim_animation_particle_data_index>> particle_data_index{};
	std::optional<std::vector<fx_particle_sim_animation_frame>> frames{};
	std::optional<std::vector<std::array<float, 4>>> color_table{};

	bool operator==(const particle_sim_animation&) const = default;
};

struct fx_particle_sim_animation_particle_data
{
	std::uint16_t x_normalized_pos{};
	std::uint16_t y_normalized_pos{};
	std::uint16_t z_normalized_pos{};
	std::uint16_t x_normalized_width{};
	std::uint16_t y_normalized_height{};
	std::uint16_t orientation{};
	std::uint16_t lifetime{};
	std::uint16_t particle_id{};
	std::uint16_t x_normalized_pos_next_frame{};
	std::uint16_t y_normalized_pos_next_frame{};
	std::uint16_t z_normalized_pos_next_frame{};
	std::uint16_t x_normalized_width_next_frame{};
	std::uint16_t y_normalized_height_next_frame{};
	std::int16_t orientation_delta{};
	std::uint16_t color_table_index{};
	std::uint16_t next_color_table_index{};

	bool operator==(const fx_particle_sim_animation_particle_data&) const = default;
};

struct fx_particle_sim_animation_particle_data_orientation3_d
{
	std::uint16_t y_orientation{};
	std::uint16_t z_orientation{};
	std::int16_t y_orientation_delta{};
	std::int16_t z_orientation_delta{};

	bool operator==(const fx_particle_sim_animation_particle_data_orientation3_d&) const = default;
};

struct fx_particle_sim_animation_particle_data_index
{
	std::uint8_t index{};
	std::uint8_t next_index{};

	bool operator==(const fx_particle_sim_animation_particle_data_index&) const = default;
};

struct fx_particle_sim_animation_frame
{
	std::uint32_t particle_data_offset{};
	std::uint32_t num_active_particles{};

	bool operator==(const fx_particle_sim_animation_frame&) const = default;
};

inline void validate(const fx_particle_sim_animation_header& data)
{
	(void) data;
}

inline void validate(const particle_sim_animation& data)
{
	if (!data.name)
	{
		throw std::runtime_error("particle_sim_animation has no name");
	}

	detail::name(*data.name, "particle_sim_animation");

	validate(data.header);

	if (data.particle_data)
	{
		for (const auto& item : *data.particle_data)
		{
			validate(item);
		}
	}

	if (data.particle_data_orientation3_d)
	{
		for (const auto& item : *data.particle_data_orientation3_d)
		{
			validate(item);
		}
	}

	if (data.particle_data_index)
	{
		for (const auto& item : *data.particle_data_index)
		{
			validate(item);
		}
	}

	if (data.frames)
	{
		for (const auto& item : *data.frames)
		{
			validate(item);
		}
	}

	if (static_cast<std::int64_t>(data.header.particle_data_count) != (data.particle_data ? static_cast<std::int64_t>(data.particle_data->size()) : 0))
	{
		throw std::runtime_error("particle_sim_animation.particle_data has a length that its count does not give");
	}

	if (static_cast<std::int64_t>((static_cast<std::uint8_t>(data.header.has_orientation3_d) == 0 ? 0 : data.header.particle_data_count)) != (data.particle_data_orientation3_d ? static_cast<std::int64_t>(data.particle_data_orientation3_d->size()) : 0))
	{
		throw std::runtime_error("particle_sim_animation.particle_data_orientation3_d has a length that its count does not give");
	}

	if (static_cast<std::int64_t>((static_cast<std::uint8_t>(data.header.has_index) == 0 ? 0 : data.header.particle_data_count)) != (data.particle_data_index ? static_cast<std::int64_t>(data.particle_data_index->size()) : 0))
	{
		throw std::runtime_error("particle_sim_animation.particle_data_index has a length that its count does not give");
	}

	if (static_cast<std::int64_t>(data.header.frame_count) != (data.frames ? static_cast<std::int64_t>(data.frames->size()) : 0))
	{
		throw std::runtime_error("particle_sim_animation.frames has a length that its count does not give");
	}

	if (static_cast<std::int64_t>(data.header.color_table_size) != (data.color_table ? static_cast<std::int64_t>(data.color_table->size()) : 0))
	{
		throw std::runtime_error("particle_sim_animation.color_table has a length that its count does not give");
	}

}

inline void validate(const fx_particle_sim_animation_particle_data& data)
{
	(void) data;
}

inline void validate(const fx_particle_sim_animation_particle_data_orientation3_d& data)
{
	(void) data;
}

inline void validate(const fx_particle_sim_animation_particle_data_index& data)
{
	(void) data;
}

inline void validate(const fx_particle_sim_animation_frame& data)
{
	(void) data;
}

} // namespace effects
} // namespace zonetool::asset_data
