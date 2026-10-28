#pragma once

#include "asset_data.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <optional>
#include <stdexcept>
#include <string>

// Neutral materials (family F3 of docs/NEUTRAL_ASSETS.md). A reference names its image by definition name.
namespace zonetool::asset_data
{
// A decal volume material: up to six image channels (a slot with no image is empty), a colour tint, the dissolve and
// emission scales, the distance at which the displacement stops and how fast it falls off, and the atlas grid of its
// images.
struct decal_volume_material
{
	std::string name;
	std::array<std::optional<std::string>, 6> channels;
	std::array<float, 3> color_tint{};
	float alpha_dissolve{};
	float emissive_scale{};
	float displacement_cutoff_distance{};
	float displacement_cutoff_falloff{};
	std::uint32_t atlas_rows{};
	std::uint32_t atlas_columns{};

	bool operator==(const decal_volume_material&) const = default;
};

inline void validate(const decal_volume_material& data)
{
	detail::name(data.name, "decalvolumematerial");
	for (const auto& channel : data.channels)
	{
		if (channel)
		{
			detail::name(*channel, "decalvolumematerial image");
		}
	}

	const std::array values{data.color_tint[0], data.color_tint[1], data.color_tint[2], data.alpha_dissolve, data.emissive_scale,
		data.displacement_cutoff_distance, data.displacement_cutoff_falloff};
	if (!std::ranges::all_of(values, [](const float value) { return std::isfinite(value); }) || data.atlas_rows > 0xFF ||
		data.atlas_columns > 0xFF)
	{
		throw std::runtime_error("decalvolumematerial " + data.name + " has a value that is not finite or an atlas beyond 255");
	}
}
} // namespace zonetool::asset_data
