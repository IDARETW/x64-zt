#pragma once

#include <core/assets/asset_data.hpp>
#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

// Neutral images (family F2 of docs/NEUTRAL_ASSETS.md): the authored texels of every mip level, as the source holds
// them (block-compressed formats keep their blocks).
namespace zonetool::asset_data
{
// One mip level: every slice of it (array element or cube face; the depth slices of a 3D level are part of its slice),
// slice after slice, without padding.
struct image_level
{
	std::uint32_t width{};
	std::uint32_t height{};
	std::uint32_t depth{};
	std::vector<std::uint8_t> bytes;

	bool operator==(const image_level&) const = default;
};

struct image
{
	std::string name;
	// The DXGI format name without its DXGI_FORMAT_ prefix, or L8_UNORM, L8A8_UNORM, R4G4_UNORM, R5G6B5_UNORM or
	// R10G10B10A2_SNORM for a format that DXGI does not name.
	std::string format;
	// 1d, 2d, 3d, cube, 2d_array or cube_array.
	std::string shape;
	std::uint32_t width{};
	std::uint32_t height{};
	std::uint32_t depth{};
	std::uint32_t elements{};
	// What the texels mean: 2d, function, color, gray, signed_velocity, normal, metalness, normal_occlusion_gloss,
	// signed_distance_field or card_imposter_normal.
	std::string usage;
	// Authoring flags, sorted: clamp_u, clamp_v, no_picmip, no_mipmaps, premultiplied_alpha, ...
	std::vector<std::string> flags;
	std::vector<image_level> levels;

	bool operator==(const image&) const = default;
};

inline constexpr const char* image_shapes[] = {"1d", "2d", "3d", "cube", "2d_array", "cube_array"};

inline void validate(const image& data)
{
	detail::name(data.name, "image");
	if (data.format.empty() || std::ranges::find(image_shapes, data.shape) == std::end(image_shapes) || !data.width ||
		!data.height || !data.depth || !data.elements || data.usage.empty() || !std::ranges::is_sorted(data.flags))
	{
		throw std::runtime_error("image " + data.name + " has an invalid format, shape, size, usage or flag order");
	}

	for (std::size_t level = 0; level < data.levels.size(); ++level)
	{
		const auto& value = data.levels[level];
		if (value.width != std::max(1u, data.width >> level) || value.height != std::max(1u, data.height >> level) ||
			value.depth != (data.shape == "3d" ? std::max(1u, data.depth >> level) : 1u))
		{
			throw std::runtime_error("image " + data.name + " has a mip level of the wrong size");
		}
	}
}
} // namespace zonetool::asset_data
