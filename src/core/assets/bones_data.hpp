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

// Neutral bones assets (docs/NEUTRAL_ASSETS.md), emitted by tools/gen_iw8_neutral.py from the IW8 structs and stream
// traversals: do not edit; change the spec (tools/neutral_families/bones.json) or the generator.
namespace zonetool::asset_data
{
namespace bones
{
inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 3> x_anim_dynamic_bone_noise_axis_space_names{{
	{0, "world"},
	{1, "model"},
	{2, "joint"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 3> x_anim_up_vector_type_names{{
	{0, "world_space"},
	{1, "bone_relative"},
	{2, "aim_at_bone"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 2> x_anim_curve_type_names{{
	{0, "linear"},
	{1, "smoothstep"},
}};

struct dynamicbones;
struct x_anim_dynamic_bone_properties;
struct snd_alias_lookup;
struct x_anim_rotational_spring_properties;
struct x_anim_translational_spring_properties;
struct x_anim_dynamic_bone_noise;
struct x_anim_dynamic_bone_noise_properties;
struct x_anim_dynamic_bone_noise_axis;
struct x_anim_collidable_bone;
struct x_anim_collision_shape;
struct x_anim_sphere_collision_shape;
struct x_anim_capsule_collision_shape;
struct x_anim_box_collision_shape;
struct x_anim_custom_pin_constraint;
struct proceduralbones;
struct x_anim_constraint;
struct x_anim_position_constraint;
struct x_anim_rotation_constraint;
struct x_anim_aim_constraint;
struct distance_driven_translation_offset_constraint;
struct distance_driven_rotation_offset_constraint;

inline void validate(const dynamicbones& data);
inline void validate(const x_anim_dynamic_bone_properties& data);
inline void validate(const snd_alias_lookup& data);
inline void validate(const x_anim_rotational_spring_properties& data);
inline void validate(const x_anim_translational_spring_properties& data);
inline void validate(const x_anim_dynamic_bone_noise& data);
inline void validate(const x_anim_dynamic_bone_noise_properties& data);
inline void validate(const x_anim_dynamic_bone_noise_axis& data);
inline void validate(const x_anim_collidable_bone& data);
inline void validate(const x_anim_collision_shape& data);
inline void validate(const x_anim_sphere_collision_shape& data);
inline void validate(const x_anim_capsule_collision_shape& data);
inline void validate(const x_anim_box_collision_shape& data);
inline void validate(const x_anim_custom_pin_constraint& data);
inline void validate(const proceduralbones& data);
inline void validate(const x_anim_constraint& data);
inline void validate(const x_anim_position_constraint& data);
inline void validate(const x_anim_rotation_constraint& data);
inline void validate(const x_anim_aim_constraint& data);
inline void validate(const distance_driven_translation_offset_constraint& data);
inline void validate(const distance_driven_rotation_offset_constraint& data);

struct dynamicbones
{
	std::optional<std::string> name{};
	std::optional<std::vector<x_anim_dynamic_bone_properties>> dynamic_bone_properties{};
	std::optional<std::vector<x_anim_collidable_bone>> collidable_bones{};
	std::uint32_t num_collidable_bone_collision_shapes{};
	std::optional<std::vector<x_anim_collision_shape>> collision_shapes{};
	std::optional<std::vector<x_anim_custom_pin_constraint>> custom_pin_constraints{};
	std::optional<std::vector<std::optional<std::string>>> pin_constraint_bone_names{};

	bool operator==(const dynamicbones&) const = default;
};

struct snd_alias_lookup
{
	std::optional<std::string> name{};

	bool operator==(const snd_alias_lookup&) const = default;
};

struct x_anim_rotational_spring_properties
{
	float swing_strength{};
	float twist_strength{};

	bool operator==(const x_anim_rotational_spring_properties&) const = default;
};

struct x_anim_translational_spring_properties
{
	std::array<float, 3> strength{};
	std::array<float, 3> damping{};
	std::array<float, 3> min_limit{};
	std::array<float, 3> max_limit{};

	bool operator==(const x_anim_translational_spring_properties&) const = default;
};

struct x_anim_dynamic_bone_noise_axis
{
	std::optional<std::string> curve{};
	float strength{};
	float rate{};
	float start{};

	bool operator==(const x_anim_dynamic_bone_noise_axis&) const = default;
};

struct x_anim_dynamic_bone_noise_properties
{
	std::string relative{};
	std::array<x_anim_dynamic_bone_noise_axis, 3> axis{};

	bool operator==(const x_anim_dynamic_bone_noise_properties&) const = default;
};

struct x_anim_dynamic_bone_noise
{
	bool enable{};
	x_anim_dynamic_bone_noise_properties translation{};
	x_anim_dynamic_bone_noise_properties rotation{};

	bool operator==(const x_anim_dynamic_bone_noise&) const = default;
};

struct x_anim_dynamic_bone_properties
{
	std::optional<std::string> bone_name{};
	std::int32_t parent_dynamic_bone_index{};
	std::int32_t dynamic_bone_chain_root_index{};
	float chain_length{};
	float inv_mass{};
	std::array<float, 3> center_of_mass{};
	float inv_inertia_tensor{};
	std::array<float, 3> twist_axis{};
	float min_twist_angle{};
	float max_twist_angle{};
	float damping{};
	float gravity_scale{};
	float cos_max_swing_angle{};
	std::array<float, 4> local_base_pose_quat{};
	std::array<float, 3> local_base_pose_trans{};
	std::uint32_t first_collision_shape_index{};
	std::uint32_t num_collision_shapes{};
	snd_alias_lookup light_hit_sound{};
	snd_alias_lookup heavy_hit_sound{};
	float light_hit_threshold{};
	float heavy_hit_threshold{};
	bool enable_swing_limit{};
	bool enable_twist_limit{};
	bool enable_rotational_spring{};
	bool enable_translational_spring{};
	x_anim_rotational_spring_properties rotational_spring{};
	x_anim_translational_spring_properties translational_spring{};
	x_anim_dynamic_bone_noise noise{};

	bool operator==(const x_anim_dynamic_bone_properties&) const = default;
};

struct x_anim_collidable_bone
{
	std::optional<std::string> bone_name{};
	std::uint32_t first_collision_shape_index{};
	std::uint32_t num_collision_shapes{};

	bool operator==(const x_anim_collidable_bone&) const = default;
};

struct x_anim_sphere_collision_shape
{
	std::array<float, 3> center{};
	float radius{};

	bool operator==(const x_anim_sphere_collision_shape&) const = default;
};

struct x_anim_capsule_collision_shape
{
	std::array<float, 3> start{};
	std::array<float, 3> end{};
	float radius{};

	bool operator==(const x_anim_capsule_collision_shape&) const = default;
};

struct x_anim_box_collision_shape
{
	std::array<float, 3> center{};
	std::array<float, 3> extents{};

	bool operator==(const x_anim_box_collision_shape&) const = default;
};

struct x_anim_collision_shape
{
	std::variant<std::monostate, x_anim_sphere_collision_shape, x_anim_capsule_collision_shape, x_anim_box_collision_shape> u{};

	bool operator==(const x_anim_collision_shape&) const = default;
};

struct x_anim_custom_pin_constraint
{
	std::int32_t dynamic_bone_index_a{};
	std::int32_t dynamic_bone_index_b{};
	std::int32_t bone_name_index_a{};
	std::array<float, 3> local_translation_a{};
	std::array<float, 3> local_translation_b{};
	float distance{};
	float stiffness{};

	bool operator==(const x_anim_custom_pin_constraint&) const = default;
};

struct proceduralbones
{
	std::optional<std::string> name{};
	std::optional<std::vector<x_anim_constraint>> constraints{};
	std::optional<std::vector<std::optional<std::string>>> target_bone_names{};

	bool operator==(const proceduralbones&) const = default;
};

struct x_anim_position_constraint
{
	float blend_fraction{};
	std::array<float, 3> min_local_translation_limit{};
	std::array<float, 3> max_local_translation_limit{};
	std::array<float, 3> offset{};
	std::array<std::array<float, 3>, 2> source_bone_offsets{};
	std::array<float, 3> axis_mask{};

	bool operator==(const x_anim_position_constraint&) const = default;
};

struct x_anim_rotation_constraint
{
	std::array<float, 4> rotation_offset_quat{};
	std::array<float, 3> axis_mask{};
	float blend_fraction{};

	bool operator==(const x_anim_rotation_constraint&) const = default;
};

struct x_anim_aim_constraint
{
	std::array<float, 3> aim_target_local_offset{};
	std::array<float, 3> local_aim_vector{};
	std::array<float, 3> local_up_vector{};
	std::array<float, 3> axis_mask{};
	std::array<float, 3> up_vector{};
	std::string up_vector_type{};

	bool operator==(const x_anim_aim_constraint&) const = default;
};

struct distance_driven_translation_offset_constraint
{
	std::array<float, 4> min_local_offset{};
	std::array<float, 4> max_local_offset{};
	std::array<std::array<float, 3>, 2> source_bone_offsets{};
	float distance_scale{};
	float distance_bias{};
	std::string curve_type{};

	bool operator==(const distance_driven_translation_offset_constraint&) const = default;
};

struct distance_driven_rotation_offset_constraint
{
	std::array<float, 4> min_local_offset{};
	std::array<float, 4> max_local_offset{};
	std::array<std::array<float, 3>, 2> source_bone_offsets{};
	float distance_scale{};
	float distance_bias{};
	std::string curve_type{};

	bool operator==(const distance_driven_rotation_offset_constraint&) const = default;
};

struct x_anim_constraint
{
	std::uint8_t local_target_bone_index{};
	std::array<std::optional<std::string>, 2> source_bone_names{};
	std::variant<std::monostate, x_anim_position_constraint, x_anim_rotation_constraint, x_anim_aim_constraint, distance_driven_translation_offset_constraint, distance_driven_rotation_offset_constraint> u{};

	bool operator==(const x_anim_constraint&) const = default;
};

inline void validate(const dynamicbones& data)
{
	if (!data.name)
	{
		throw std::runtime_error("dynamicbones has no name");
	}

	detail::name(*data.name, "dynamicbones");

	if (data.dynamic_bone_properties)
	{
		for (const auto& item : *data.dynamic_bone_properties)
		{
			validate(item);
		}
	}

	if (data.collidable_bones)
	{
		for (const auto& item : *data.collidable_bones)
		{
			validate(item);
		}
	}

	if (data.collision_shapes)
	{
		for (const auto& item : *data.collision_shapes)
		{
			validate(item);
		}
	}

	if (data.custom_pin_constraints)
	{
		for (const auto& item : *data.custom_pin_constraints)
		{
			validate(item);
		}
	}

	if ((data.dynamic_bone_properties && data.dynamic_bone_properties->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("dynamicbones has arrays that its count does not fit");
	}

	if ((data.collidable_bones && data.collidable_bones->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("dynamicbones has arrays that its count does not fit");
	}

	if ((data.collision_shapes && data.collision_shapes->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("dynamicbones has arrays that its count does not fit");
	}

	if ((data.custom_pin_constraints && data.custom_pin_constraints->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("dynamicbones has arrays that its count does not fit");
	}

	if ((data.pin_constraint_bone_names && data.pin_constraint_bone_names->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("dynamicbones has arrays that its count does not fit");
	}

}

inline void validate(const snd_alias_lookup& data)
{
	(void) data;
}

inline void validate(const x_anim_rotational_spring_properties& data)
{
	(void) data;
}

inline void validate(const x_anim_translational_spring_properties& data)
{
	(void) data;
}

inline void validate(const x_anim_dynamic_bone_noise_axis& data)
{
	(void) data;
}

inline void validate(const x_anim_dynamic_bone_noise_properties& data)
{
	if (std::ranges::none_of(x_anim_dynamic_bone_noise_axis_space_names, [&](const auto& entry) { return entry.second == data.relative; }))
	{
		throw std::runtime_error("x_anim_dynamic_bone_noise_properties.relative names no value");
	}

	for (const auto& item : data.axis)
	{
		validate(item);
	}

}

inline void validate(const x_anim_dynamic_bone_noise& data)
{
	validate(data.translation);

	validate(data.rotation);

}

inline void validate(const x_anim_dynamic_bone_properties& data)
{
	validate(data.light_hit_sound);

	validate(data.heavy_hit_sound);

	validate(data.rotational_spring);

	validate(data.translational_spring);

	validate(data.noise);

}

inline void validate(const x_anim_collidable_bone& data)
{
	(void) data;
}

inline void validate(const x_anim_sphere_collision_shape& data)
{
	(void) data;
}

inline void validate(const x_anim_capsule_collision_shape& data)
{
	(void) data;
}

inline void validate(const x_anim_box_collision_shape& data)
{
	(void) data;
}

inline void validate(const x_anim_collision_shape& data)
{
	std::visit([](const auto& value)
	{
		if constexpr (std::is_same_v<std::decay_t<decltype(value)>, std::monostate>)
		{
			throw std::runtime_error("x_anim_collision_shape.u holds no module");
		}
		else
		{
			validate(value);
		}
	}, data.u);

}

inline void validate(const x_anim_custom_pin_constraint& data)
{
	(void) data;
}

inline void validate(const proceduralbones& data)
{
	if (!data.name)
	{
		throw std::runtime_error("proceduralbones has no name");
	}

	detail::name(*data.name, "proceduralbones");

	if (data.constraints)
	{
		for (const auto& item : *data.constraints)
		{
			validate(item);
		}
	}

	if ((data.constraints && data.constraints->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("proceduralbones has arrays that its count does not fit");
	}

	if ((data.target_bone_names && data.target_bone_names->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("proceduralbones has arrays that its count does not fit");
	}

}

inline void validate(const x_anim_position_constraint& data)
{
	(void) data;
}

inline void validate(const x_anim_rotation_constraint& data)
{
	(void) data;
}

inline void validate(const x_anim_aim_constraint& data)
{
	if (std::ranges::none_of(x_anim_up_vector_type_names, [&](const auto& entry) { return entry.second == data.up_vector_type; }))
	{
		throw std::runtime_error("x_anim_aim_constraint.up_vector_type names no value");
	}

}

inline void validate(const distance_driven_translation_offset_constraint& data)
{
	if (std::ranges::none_of(x_anim_curve_type_names, [&](const auto& entry) { return entry.second == data.curve_type; }))
	{
		throw std::runtime_error("distance_driven_translation_offset_constraint.curve_type names no value");
	}

}

inline void validate(const distance_driven_rotation_offset_constraint& data)
{
	if (std::ranges::none_of(x_anim_curve_type_names, [&](const auto& entry) { return entry.second == data.curve_type; }))
	{
		throw std::runtime_error("distance_driven_rotation_offset_constraint.curve_type names no value");
	}

}

inline void validate(const x_anim_constraint& data)
{
	std::visit([](const auto& value)
	{
		if constexpr (std::is_same_v<std::decay_t<decltype(value)>, std::monostate>)
		{
			throw std::runtime_error("x_anim_constraint.u holds no module");
		}
		else
		{
			validate(value);
		}
	}, data.u);

}

} // namespace bones
} // namespace zonetool::asset_data
