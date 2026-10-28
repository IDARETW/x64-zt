#pragma once

#include "asset_data.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

// Neutral effect support assets (family F7 of docs/NEUTRAL_ASSETS.md).
namespace zonetool::asset_data
{
// The damping of a physics FX pipeline's integrator; the basic integrator has none.
struct integrator_damping
{
	float linear{};
	float angular{};

	bool operator==(const integrator_damping&) const = default;
};

// How the particles of a physics effect move and collide.
struct physics_fx_pipeline
{
	std::string name;
	std::int32_t particles_per_instance{};
	float friction{};
	float restitution{};
	bool particle_collisions{};
	bool collision_callbacks{};
	float callback_impulse_threshold{};
	std::optional<integrator_damping> damping;

	bool operator==(const physics_fx_pipeline&) const = default;
};

// A regular grid of 4-component samples over a box: a force or a velocity.
struct vector_subfield
{
	std::array<float, 3> mins{};
	std::array<float, 3> maxs{};
	// The spacing of the grid cells.
	std::array<float, 3> cell_size{};
	std::array<std::uint32_t, 3> entries{};
	// none, force or velocity.
	std::string type;
	// entries[0] * entries[1] * entries[2] samples of 4 floats.
	std::vector<float> samples;

	bool operator==(const vector_subfield&) const = default;
};

struct vector_field
{
	std::string name;
	std::array<float, 3> mins{};
	std::array<float, 3> maxs{};
	std::array<float, 3> local_origin{};
	std::vector<vector_subfield> subfields;

	bool operator==(const vector_field&) const = default;
};

inline constexpr const char* vector_field_types[] = {"none", "force", "velocity"};

namespace detail
{
template <typename Range>
bool all_finite(const Range& values)
{
	return std::ranges::all_of(values, [](const float value) { return std::isfinite(value); });
}
}

inline void validate(const physics_fx_pipeline& data)
{
	detail::name(data.name, "physicsfxpipeline");
	const std::array values{data.friction, data.restitution, data.callback_impulse_threshold,
		data.damping ? data.damping->linear : 0.0f, data.damping ? data.damping->angular : 0.0f};
	if (!detail::all_finite(values) || data.particles_per_instance < 0)
	{
		throw std::runtime_error("physicsfxpipeline " + data.name + " has a value that is not finite or a negative particle count");
	}
}

inline void validate(const vector_field& data)
{
	detail::name(data.name, "vectorfield");
	if (!detail::all_finite(data.mins) || !detail::all_finite(data.maxs) || !detail::all_finite(data.local_origin))
	{
		throw std::runtime_error("vectorfield " + data.name + " has a bound that is not finite");
	}

	for (const auto& subfield : data.subfields)
	{
		const std::size_t cells = static_cast<std::size_t>(subfield.entries[0]) * subfield.entries[1] * subfield.entries[2];
		if (std::ranges::find(vector_field_types, subfield.type) == std::end(vector_field_types) ||
			subfield.samples.size() != cells * 4 || !detail::all_finite(subfield.samples) || !detail::all_finite(subfield.mins) ||
			!detail::all_finite(subfield.maxs) || !detail::all_finite(subfield.cell_size))
		{
			throw std::runtime_error("vectorfield " + data.name + " has a sub field with an unknown type, a sample count that is not "
				"4 per cell, or a value that is not finite");
		}
	}
}
} // namespace zonetool::asset_data
