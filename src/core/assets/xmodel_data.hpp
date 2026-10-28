#pragma once

#include "asset_data.hpp"
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

// A vertex of a model surface, as authored: its position is three 21-bit codes over the surface bounds (midpoint +
// (2 * code / 2097151 - 1) * the largest half size), its tangent frame a packed quaternion (10, 10 and 9 bits, the
// handedness and the index of the largest component).
struct mesh_vertex
{
	std::array<std::uint32_t, 3> position{};
	std::uint32_t self_visibility{};
	std::array<float, 2> uv{};
	std::uint32_t tangent_frame{};

	bool operator==(const mesh_vertex&) const = default;
};

struct rigid_vertex_list
{
	std::uint32_t bone{};
	std::uint32_t vertex_count{};
	std::uint32_t first_triangle{};
	std::uint32_t triangle_count{};

	bool operator==(const rigid_vertex_list&) const = default;
};

// The bounds of up to 64 triangles (midpoint and half size as half floats) and their visibility cone, as authored.
struct triangle_cluster
{
	std::array<std::uint16_t, 6> bounds{};
	std::array<std::uint32_t, 3> cone{};

	bool operator==(const triangle_cluster&) const = default;
};

struct mesh_surface
{
	bounds_box bounds;
	std::vector<std::array<std::uint16_t, 3>> triangles;
	std::vector<mesh_vertex> vertices;
	std::optional<std::vector<std::uint32_t>> colors;
	std::vector<triangle_cluster> clusters;
	std::vector<rigid_vertex_list> rigid_lists;
	// One per rigid list.
	std::optional<std::vector<bounds_box>> rigid_bounds;
	// Vertices with 1 to 8 bone influences, and their bone and weight words.
	std::array<std::uint16_t, 8> blend_vertex_counts{};
	std::vector<std::uint16_t> blend_words;
	std::optional<std::vector<std::uint32_t>> lightmap_coords;
	// A 32-bit code a vertex each, as authored; their encodings are not mapped.
	std::optional<std::vector<std::uint32_t>> second_uv;
	std::optional<std::vector<std::uint32_t>> normal_transforms;
	std::array<std::uint32_t, 8> part_bits{};

	bool operator==(const mesh_surface&) const = default;
};

// The surfaces of one level of detail of a model.
struct xmodel_surfs
{
	std::string name;
	std::array<std::uint32_t, 8> part_bits{};
	std::vector<mesh_surface> surfaces;

	bool operator==(const xmodel_surfs&) const = default;
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
inline void validate(const xmodel_surfs& data)
{
	detail::name(data.name, "xmodelsurfs");
	for (const auto& surface : data.surfaces)
	{
		const auto vertices = surface.vertices.size();
		detail::finite_values(surface.bounds.midpoint, data.name);
		detail::finite_values(surface.bounds.half_size, data.name);
		if (vertices > 0xFFFF || surface.triangles.size() > 0xFFFF || surface.clusters.size() != (surface.triangles.size() + 63) / 64 ||
			(surface.colors && surface.colors->size() != vertices) || (surface.lightmap_coords && surface.lightmap_coords->size() != vertices) ||
			(surface.second_uv && surface.second_uv->size() != vertices) ||
			(surface.normal_transforms && surface.normal_transforms->size() != vertices) ||
			(surface.rigid_bounds && surface.rigid_bounds->size() != surface.rigid_lists.size()) || surface.rigid_lists.size() > 0xFF)
		{
			throw std::runtime_error("model surfaces " + data.name + " have streams of the wrong count");
		}

		for (const auto& triangle : surface.triangles)
		{
			if (std::ranges::any_of(triangle, [vertices](const std::uint16_t index) { return index >= vertices; }))
			{
				throw std::runtime_error("model surfaces " + data.name + " have a triangle with a missing vertex");
			}
		}

		for (const auto& vertex : surface.vertices)
		{
			detail::finite_values(vertex.uv, data.name);
			if (std::ranges::any_of(vertex.position, [](const std::uint32_t code) { return code >> 21; }))
			{
				throw std::runtime_error("model surfaces " + data.name + " have a position code beyond 21 bits");
			}
		}
	}
}
} // namespace zonetool::asset_data
