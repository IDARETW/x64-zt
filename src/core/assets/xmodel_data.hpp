#pragma once

#include <core/assets/asset_data.hpp>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <optional>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

// Neutral models (family F4 of docs/NEUTRAL_ASSETS.md): the skeleton, the levels of detail with their surfaces and
// materials, and the assets a model points to, by definition name. Rotations keep their authored codes: a bone's
// rest rotation is x, y, z and w in signed codes of 1/32767.
namespace zonetool::asset_data
{
struct bounds_box
{
	std::array<float, 3> midpoint{};
	std::array<float, 3> half_size{};

	bool operator==(const bounds_box&) const = default;
};

// The rest pose of a bone relative to its parent.
struct bone_link
{
	std::uint32_t parent{};
	std::array<std::int16_t, 4> rotation{};
	std::array<float, 3> translation{};

	bool operator==(const bone_link&) const = default;
};

struct xmodel_bone
{
	std::string name;
	// Absent for a root bone.
	std::optional<bone_link> link;

	bool operator==(const xmodel_bone&) const = default;
};

// The model-space pose of a bone and its translation weight.
struct bone_base
{
	std::array<float, 4> rotation{};
	std::array<float, 3> translation{};
	float translation_weight{};

	bool operator==(const bone_base&) const = default;
};

struct bone_info
{
	bounds_box bounds;
	float radius_squared{};

	bool operator==(const bone_info&) const = default;
};

struct xmodel_lod
{
	// The XModelSurfs asset of the level.
	std::optional<std::string> surfaces;
	float distance{};
	std::uint32_t first_surface{};
	std::uint32_t surface_count{};
	std::array<std::uint32_t, 8> part_bits{};
	// Sorted: subdiv, subdiv_non_adaptive.
	std::vector<std::string> flags;

	bool operator==(const xmodel_lod&) const = default;
};

// A piece of a model that bends in the wind and under pushes: its stiffness is a half float and its rest rotation a
// packed quaternion, as authored.
struct reactive_part
{
	std::array<float, 3> center{};
	std::uint16_t stiffness{};
	std::uint16_t parent{};
	std::array<float, 3> pivot{};
	std::uint32_t pivot_rotation{};

	bool operator==(const reactive_part&) const = default;
};

struct reactive_tweak
{
	std::array<float, 4> amplitude_scale{};
	std::array<float, 4> frequency_scale{};

	bool operator==(const reactive_tweak&) const = default;
};

struct reactive_motion_info
{
	// Area, amplitude and frequency scales of the wind, and a fourth value.
	std::array<float, 4> wind{};
	std::vector<reactive_part> parts;
	std::optional<std::vector<reactive_tweak>> tweaks;
	// Sorted: use_hierarchy, use_part_tweaks.
	std::vector<std::string> flags;

	bool operator==(const reactive_motion_info&) const = default;
};

struct blend_shape_map
{
	std::uint16_t weight{};
	std::uint16_t target{};
	float full_weight{};

	bool operator==(const blend_shape_map&) const = default;
};

struct xmodel_blend_shapes
{
	std::uint32_t targets{};
	std::vector<std::string> weights;
	std::vector<blend_shape_map> maps;

	bool operator==(const xmodel_blend_shapes&) const = default;
};

struct mdao_volume
{
	std::array<float, 3> mins{};
	std::array<float, 3> maxs{};
	std::array<std::uint16_t, 3> cells{};
	std::uint16_t parent_bone{};
	std::optional<std::string> image;

	bool operator==(const mdao_volume&) const = default;
};

struct xmodel_decal_volume
{
	std::array<float, 3> center{};
	std::array<float, 9> axes{};
	std::array<float, 3> half_size{};
	std::optional<std::string> material;
	std::optional<std::string> blend_mask;
	std::int32_t bone{};
	std::array<float, 2> uv_offset{};
	std::array<float, 4> uv_matrix{};

	bool operator==(const xmodel_decal_volume&) const = default;
};

struct xmodel
{
	std::string name;
	float scale{};
	float radius{};
	float edge_length{};
	bounds_box bounds;
	std::uint32_t collision_lod{};
	std::uint32_t shadow_cutoff_lod{};
	// no_physics, standard, no_collision_lod or collision_lod_only.
	std::string physics_use;
	// normal, alien, small_robot, large_robot or zombie.
	std::string impact_type;
	// none, hitbox_skeleton, base_skeleton or volume.
	std::string mdao_type;

	// The root bones first; the last client_bones bones are client-only.
	std::vector<xmodel_bone> bones;
	std::uint32_t client_bones{};
	// One per bone that is not client-only.
	std::optional<std::vector<std::uint8_t>> classifications;
	std::optional<std::vector<bone_base>> base_poses;
	std::optional<std::vector<bone_info>> bone_infos;
	std::vector<std::string> aim_assist_bones;
	std::optional<std::array<std::array<float, 3>, 4>> ik_hinge_axes;
	std::array<std::uint32_t, 8> unscaled_parts{};

	// One per surface.
	std::vector<std::optional<std::string>> materials;
	std::optional<std::vector<float>> high_mip_radius_inv_sq;
	std::vector<xmodel_lod> lods;

	std::optional<std::string> physics_asset;
	std::optional<std::string> physics_fx_shape;
	std::optional<std::string> detail_collision;
	std::optional<std::string> scriptable_mover;
	std::optional<std::string> procedural_bones;
	std::optional<std::string> dynamic_bones;
	std::vector<std::optional<std::string>> cloth;
	std::optional<reactive_motion_info> reactive_motion;
	std::optional<xmodel_blend_shapes> blend_shapes;
	std::vector<mdao_volume> mdao_volumes;
	std::optional<std::vector<xmodel_decal_volume>> decal_volumes;

	bool operator==(const xmodel&) const = default;
};

inline constexpr const char* xmodel_physics_uses[] = {"no_physics", "standard", "no_collision_lod", "collision_lod_only"};
inline constexpr const char* xmodel_impact_types[] = {"normal", "alien", "small_robot", "large_robot", "zombie"};
inline constexpr const char* xmodel_mdao_types[] = {"none", "hitbox_skeleton", "base_skeleton", "volume"};
inline constexpr const char* xmodel_lod_flags[] = {"subdiv", "subdiv_non_adaptive"};
inline constexpr const char* reactive_motion_flags[] = {"use_hierarchy", "use_part_tweaks"};

namespace detail
{
template <typename Range>
void finite_values(const Range& values, const std::string& name)
{
	if (!std::ranges::all_of(values, [](const float value) { return std::isfinite(value); }))
	{
		throw std::runtime_error("model " + name + " has a value that is not finite");
	}
}

template <std::size_t Count>
void known_flags(const std::vector<std::string>& flags, const char* const (&names)[Count], const std::string& name)
{
	if (!std::ranges::is_sorted(flags) || std::ranges::adjacent_find(flags) != flags.end() ||
		std::ranges::any_of(flags, [&names](const std::string& flag) { return std::ranges::find(names, flag) == std::end(names); }))
	{
		throw std::runtime_error("model " + name + " has unknown, repeated or unsorted flags");
	}
}

template <std::size_t Count>
void known_name(const std::string& value, const char* const (&names)[Count], const std::string& name)
{
	if (std::ranges::find(names, value) == std::end(names))
	{
		throw std::runtime_error("model " + name + " has an unknown value: " + value);
	}
}
}

inline void validate(const xmodel& data)
{
	detail::name(data.name, "xmodel");
	const auto& name = data.name;
	detail::finite_values(std::array{data.scale, data.radius, data.edge_length}, name);
	detail::finite_values(data.bounds.midpoint, name);
	detail::finite_values(data.bounds.half_size, name);
	detail::known_name(data.physics_use, xmodel_physics_uses, name);
	detail::known_name(data.impact_type, xmodel_impact_types, name);
	detail::known_name(data.mdao_type, xmodel_mdao_types, name);

	std::set<std::string> names;
	bool roots = true;
	for (std::size_t bone = 0; bone < data.bones.size(); ++bone)
	{
		const auto& value = data.bones[bone];
		roots = roots && !value.link;
		if (value.name.empty() || (value.link && (value.link->parent >= bone)) || (!value.link && !roots))
		{
			throw std::runtime_error("model " + name + " has an empty bone name, a parent after its bone or a root after a child");
		}

		if (value.link)
		{
			detail::finite_values(value.link->translation, name);
		}
	}

	const auto bones = data.bones.size();
	if (data.client_bones > bones || (data.classifications && data.classifications->size() != bones - data.client_bones) ||
		(data.base_poses && data.base_poses->size() != bones) || (data.bone_infos && data.bone_infos->size() != bones) ||
		(data.high_mip_radius_inv_sq && data.high_mip_radius_inv_sq->size() != data.materials.size()))
	{
		throw std::runtime_error("model " + name + " has per-bone or per-surface data of the wrong count");
	}

	for (const auto& pose : data.base_poses.value_or(std::vector<bone_base>{}))
	{
		detail::finite_values(pose.rotation, name);
		detail::finite_values(pose.translation, name);
		detail::finite_values(std::array{pose.translation_weight}, name);
	}

	for (const auto& info : data.bone_infos.value_or(std::vector<bone_info>{}))
	{
		detail::finite_values(info.bounds.midpoint, name);
		detail::finite_values(info.bounds.half_size, name);
		detail::finite_values(std::array{info.radius_squared}, name);
	}

	for (const auto& lod : data.lods)
	{
		detail::finite_values(std::array{lod.distance}, name);
		detail::known_flags(lod.flags, xmodel_lod_flags, name);
	}

	if (data.reactive_motion)
	{
		detail::finite_values(data.reactive_motion->wind, name);
		detail::known_flags(data.reactive_motion->flags, reactive_motion_flags, name);
		if (data.reactive_motion->tweaks && data.reactive_motion->tweaks->size() != data.reactive_motion->parts.size())
		{
			throw std::runtime_error("model " + name + " has reactive tweaks of the wrong count");
		}
	}

	if (data.blend_shapes && std::ranges::any_of(data.blend_shapes->maps, [](const blend_shape_map& map) { return !std::isfinite(map.full_weight); }))
	{
		throw std::runtime_error("model " + name + " has a blend shape weight that is not finite");
	}
}
} // namespace zonetool::asset_data
