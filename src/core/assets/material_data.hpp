#pragma once

#include "asset_data.hpp"
#include <algorithm>
#include <array>
#include <bit>
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

// What a shader argument binds: a code constant, texture, buffer, material texture or material constant (the profile's
// argument kinds), the shader stage that reads it, and where it goes. The numbers are those of the profile.
struct technique_argument
{
	std::uint32_t kind{};
	std::uint32_t stage{};
	std::uint32_t destination{};
	std::uint32_t index{};

	bool operator==(const technique_argument&) const = default;
};

// One way of drawing a material (a pass of the frame): its shaders by name, the arguments they read, and the state of
// the pipeline. The numbers (type, flags, formats, state bits) are those of the profile that wrote the technique.
struct technique
{
	std::string name;
	std::uint32_t type{};
	std::uint32_t flags{};
	std::uint32_t input_layout_count{};
	std::array<std::uint8_t, 25> pipeline_state_map{};
	// One hash per pipeline state and input layout.
	std::vector<std::uint64_t> pipeline_state_hashes;
	std::array<std::uint64_t, 4> custom_buffer_flags{};
	std::optional<std::array<std::uint8_t, 16>> root_parameter_map;
	std::optional<std::string> vertex_shader;
	std::optional<std::string> hull_shader;
	std::optional<std::string> domain_shader;
	std::optional<std::string> pixel_shader;
	// The arguments in the profile's order, and how many of them are never, per-object, per-primitive and stable.
	std::vector<technique_argument> arguments;
	std::array<std::uint32_t, 4> argument_counts{};
	std::array<std::uint32_t, 3> argument_sizes{};
	std::uint32_t color_target_count{};
	std::vector<std::array<std::uint8_t, 5>> state_bits;
	std::array<std::uint8_t, 5> target_formats{};
	std::uint32_t custom_sampler_flags{};
	std::uint32_t constant_layout{};
	std::uint32_t stage_config{};
	std::array<std::uint64_t, 2> default_state_bits{};

	bool operator==(const technique&) const = default;
};

// A technique set: the techniques of a kind of surface, one for each bit of its mask (in the order of the bits). A
// technique can be shared with other sets.
struct technique_set
{
	std::string name;
	std::uint64_t flags{};
	std::uint32_t geometry_type{};
	std::array<std::uint64_t, 4> mask{};
	std::vector<std::optional<technique>> techniques;

	bool operator==(const technique_set&) const = default;
};

inline void validate(const technique_set& data)
{
	detail::name(data.name, "techset");
	std::size_t bits = 0;
	for (const auto word : data.mask)
	{
		bits += static_cast<std::size_t>(std::popcount(word));
	}

	if (data.techniques.size() != bits)
	{
		throw std::runtime_error("techset " + data.name + " has a technique count that is not the bits of its mask");
	}

	for (const auto& value : data.techniques)
	{
		if (!value)
		{
			continue;
		}

		detail::name(value->name, "technique");
		const auto states = value->state_bits.size();
		const auto total = static_cast<std::size_t>(value->argument_counts[0]) + value->argument_counts[1] + value->argument_counts[2] +
			value->argument_counts[3];
		if (value->pipeline_state_hashes.size() != states * value->input_layout_count || value->arguments.size() != total ||
			states > 0xFF || value->input_layout_count > 0xFF || value->color_target_count > 0xFF || value->argument_counts[0] > 0xFF ||
			value->argument_counts[1] > 0xFF || value->argument_counts[2] > 0xFF || value->argument_counts[3] > 0xFF)
		{
			throw std::runtime_error("technique " + value->name + " has counts that do not agree");
		}

		for (const auto& shader : {value->vertex_shader, value->hull_shader, value->domain_shader, value->pixel_shader})
		{
			if (shader)
			{
				detail::name(*shader, "technique shader");
			}
		}
	}
}

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
