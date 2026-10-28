#pragma once

#include "asset_data.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

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

// An image bound to a numbered argument slot of the material's technique set. The slot numbers are those of the
// profile that the material comes from.
struct material_texture
{
	std::uint32_t slot{};
	std::optional<std::string> image;

	bool operator==(const material_texture&) const = default;
};

// A literal four-component value bound to a numbered argument slot. Shader constants can hold infinities and NaNs as
// markers, so values are compared by their bits.
struct material_constant
{
	std::uint32_t slot{};
	std::array<float, 4> value{};

	bool operator==(const material_constant& other) const
	{
		return this->slot == other.slot && std::memcmp(this->value.data(), other.value.data(), sizeof(this->value)) == 0;
	}
};

// The packed shader constants of one buffer, by shader stage.
struct material_constant_buffer
{
	std::vector<std::uint8_t> vertex;
	std::vector<std::uint8_t> hull;
	std::vector<std::uint8_t> domain;
	std::vector<std::uint8_t> pixel;

	bool operator==(const material_constant_buffer&) const = default;
};

// A material: the technique set that draws it, its images and constants by argument slot, the constants packed for the
// shaders, and the layers it blends. The meaning of a slot belongs to the technique set of the profile that wrote it.
struct material
{
	std::string name;
	std::uint32_t contents{};
	std::uint32_t surface_flags{};
	float max_displacement{};
	// The draw order among materials of one kind.
	std::uint32_t sort_key{};
	std::uint32_t atlas_rows{};
	std::uint32_t atlas_columns{};
	std::optional<std::vector<std::uint8_t>> atlas_data;
	std::optional<std::string> techset;
	std::vector<material_texture> textures;
	std::vector<material_constant> constants;
	std::optional<std::string> decal_volume;
	// For each argument slot, the constant buffer that holds it.
	std::optional<std::vector<std::uint8_t>> constant_buffer_slots;
	std::vector<material_constant_buffer> constant_buffers;
	// The materials of the layers, in order.
	std::vector<std::string> layers;

	bool operator==(const material&) const = default;
};

inline void validate(const material& data)
{
	detail::name(data.name, "material");
	const auto optional_name = [](const std::optional<std::string>& value, const char* what)
	{
		if (value)
		{
			detail::name(*value, what);
		}
	};

	optional_name(data.techset, "material techset");
	optional_name(data.decal_volume, "material decal volume");
	for (const auto& texture : data.textures)
	{
		optional_name(texture.image, "material image");
	}

	for (const auto& layer : data.layers)
	{
		detail::name(layer, "material layer");
	}

	const auto fail = [&data](const std::string& what) { throw std::runtime_error("material " + data.name + " has " + what); };
	if (data.textures.size() > 0xFF || data.constants.size() > 0xFF || data.constant_buffers.size() > 0xFF || data.layers.size() > 0xFF)
	{
		fail("more than 255 textures, constants, constant buffers or layers");
	}

	if (data.sort_key > 0xFF || data.atlas_rows > 0xFF || data.atlas_columns > 0xFF || (data.atlas_data && data.atlas_data->size() > 0xFFFF))
	{
		fail("a sort key or atlas beyond its size");
	}

	if (std::ranges::any_of(data.textures, [](const material_texture& value) { return value.slot > 0xFF; }) ||
		std::ranges::any_of(data.constants, [](const material_constant& value) { return value.slot > 0xFF; }))
	{
		fail("a slot beyond 255");
	}

	if (!std::isfinite(data.max_displacement))
	{
		fail("a maximum displacement that is not finite");
	}
}

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
