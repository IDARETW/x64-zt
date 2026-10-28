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
#include <variant>
#include <vector>

// Neutral animations (family F5 of docs/NEUTRAL_ASSETS.md): the channels of each bone, the motion of the root, the notes
// and the blend shape weights. Quantized channels keep their authored codes, so that an encoder of the same quantization
// stores the same samples: a quaternion is z and w (a turn about z) or x, y, z and w, in signed codes of 1/32767, and a
// keyed translation is origin + scale * code on each axis.
namespace zonetool::asset_data
{
struct quaternion_key
{
	std::uint32_t frame{};
	std::vector<std::int16_t> codes;

	bool operator==(const quaternion_key&) const = default;
};

struct translation_key
{
	std::uint32_t frame{};
	std::array<std::uint16_t, 3> codes{};

	bool operator==(const translation_key&) const = default;
};

struct identity_rotation
{
	bool operator==(const identity_rotation&) const = default;
};

struct constant_rotation
{
	std::vector<std::int16_t> codes;

	bool operator==(const constant_rotation&) const = default;
};

struct keyed_rotation
{
	std::vector<quaternion_key> keys;

	bool operator==(const keyed_rotation&) const = default;
};

struct zero_translation
{
	bool operator==(const zero_translation&) const = default;
};

struct constant_translation
{
	std::array<float, 3> value{};

	bool operator==(const constant_translation&) const = default;
};

struct keyed_translation
{
	std::array<float, 3> origin{};
	std::array<float, 3> scale{};
	// 8 or 16.
	std::uint32_t code_bits{};
	std::vector<translation_key> keys;

	bool operator==(const keyed_translation&) const = default;
};

using bone_rotation = std::variant<identity_rotation, constant_rotation, keyed_rotation>;
using bone_translation = std::variant<zero_translation, constant_translation, keyed_translation>;
using delta_rotation = std::variant<constant_rotation, keyed_rotation>;
using delta_translation = std::variant<constant_translation, keyed_translation>;

struct xanim_bone
{
	std::string name;
	bone_rotation rotation;
	bone_translation translation;

	bool operator==(const xanim_bone&) const = default;
};

// A present motion may have no channel: it differs from an absent one.
struct xanim_delta
{
	std::optional<delta_translation> translation;
	// z and w codes.
	std::optional<delta_rotation> rotation_2d;
	// x, y, z and w codes.
	std::optional<delta_rotation> rotation_3d;

	bool operator==(const xanim_delta&) const = default;
};

struct xanim_note
{
	std::string name;
	float time{};

	bool operator==(const xanim_note&) const = default;
};

struct xanim
{
	std::string name;
	// The frame intervals: keys lie on frames 0 to frames.
	std::uint32_t frames{};
	float framerate{};
	float frequency{};
	// Sorted: delta_2d, delta_3d, looping.
	std::vector<std::string> flags;
	std::vector<xanim_bone> bones;
	std::optional<xanim_delta> delta;
	// In authored order: equal names stay separate notes.
	std::vector<xanim_note> notes;
	std::vector<std::string> blend_shapes;
	// frames + 1 rows of one weight per blend shape.
	std::vector<float> blend_shape_weights;

	bool operator==(const xanim&) const = default;
};

inline constexpr const char* xanim_flags[] = {"delta_2d", "delta_3d", "looping"};

namespace detail
{
inline void finite(const float value, const xanim& data)
{
	if (!std::isfinite(value))
	{
		throw std::runtime_error("animation " + data.name + " has a value that is not finite");
	}
}

inline void codes(const std::size_t count, const std::size_t components, const xanim& data)
{
	if (components ? count != components : count != 2 && count != 4)
	{
		throw std::runtime_error("animation " + data.name + " has a quaternion of the wrong component count");
	}
}

// components: 2 or 4, or 0 for either (the same for every key).
inline void validate(const keyed_rotation& value, const std::size_t components, const xanim& data)
{
	if (value.keys.empty())
	{
		throw std::runtime_error("animation " + data.name + " has a keyed rotation without keys");
	}

	codes(value.keys.front().codes.size(), components, data);
	for (std::size_t key = 0; key < value.keys.size(); ++key)
	{
		const auto& item = value.keys[key];
		codes(item.codes.size(), value.keys.front().codes.size(), data);
		if (item.frame > data.frames || (key && item.frame < value.keys[key - 1].frame))
		{
			throw std::runtime_error("animation " + data.name + " has rotation keys out of frame order");
		}
	}
}

inline void validate(const keyed_translation& value, const xanim& data)
{
	if (value.keys.empty() || (value.code_bits != 8 && value.code_bits != 16))
	{
		throw std::runtime_error("animation " + data.name + " has a keyed translation without keys or code size");
	}

	for (std::size_t axis = 0; axis < 3; ++axis)
	{
		finite(value.origin[axis], data);
		finite(value.scale[axis], data);
	}

	for (std::size_t key = 0; key < value.keys.size(); ++key)
	{
		const auto& item = value.keys[key];
		if (item.frame > data.frames || (key && item.frame < value.keys[key - 1].frame) ||
			std::ranges::any_of(item.codes, [&value](const std::uint16_t code) { return code >> value.code_bits; }))
		{
			throw std::runtime_error("animation " + data.name + " has translation keys out of frame order or code range");
		}
	}
}

inline void validate(const constant_translation& value, const xanim& data)
{
	std::ranges::for_each(value.value, [&data](const float item) { finite(item, data); });
}

inline void validate(const constant_rotation& value, const std::size_t components, const xanim& data)
{
	codes(value.codes.size(), components, data);
}

inline void validate(const identity_rotation&, const std::size_t, const xanim&)
{
}

inline void validate(const zero_translation&, const xanim&)
{
}
}

inline void validate(const xanim& data)
{
	detail::name(data.name, "animation");
	detail::finite(data.framerate, data);
	detail::finite(data.frequency, data);
	if (!std::ranges::is_sorted(data.flags) || std::ranges::adjacent_find(data.flags) != data.flags.end() ||
		std::ranges::any_of(data.flags, [](const std::string& flag) { return std::ranges::find(xanim_flags, flag) == std::end(xanim_flags); }))
	{
		throw std::runtime_error("animation " + data.name + " has unknown, repeated or unsorted flags");
	}

	std::set<std::string> names;
	for (const auto& bone : data.bones)
	{
		if (bone.name.empty() || !names.insert(bone.name).second)
		{
			throw std::runtime_error("animation " + data.name + " has an empty or repeated bone name");
		}

		std::visit([&data](const auto& value) { detail::validate(value, 0, data); }, bone.rotation);
		std::visit([&data](const auto& value) { detail::validate(value, data); }, bone.translation);
	}

	if (data.delta)
	{
		if (data.delta->translation)
		{
			std::visit([&data](const auto& value) { detail::validate(value, data); }, *data.delta->translation);
		}

		for (const auto& [rotation, components] : {std::pair{&data.delta->rotation_2d, 2}, {&data.delta->rotation_3d, 4}})
		{
			if (*rotation)
			{
				std::visit([&](const auto& value) { detail::validate(value, components, data); }, **rotation);
			}
		}
	}

	std::ranges::for_each(data.notes, [&data](const xanim_note& note) { detail::finite(note.time, data); });
	std::ranges::for_each(data.blend_shape_weights, [&data](const float weight) { detail::finite(weight, data); });
	if (std::ranges::any_of(data.blend_shapes, &std::string::empty) ||
		data.blend_shape_weights.size() != (static_cast<std::size_t>(data.frames) + 1) * data.blend_shapes.size())
	{
		throw std::runtime_error("animation " + data.name + " has an empty blend shape name or a weight count that is not "
			"frames + 1 rows of its blend shapes");
	}
}
namespace detail
{
inline data_json write_channel(const identity_rotation&)
{
	return {{"kind", "identity"}};
}

inline data_json write_channel(const zero_translation&)
{
	return {{"kind", "zero"}};
}

inline data_json write_channel(const constant_rotation& value)
{
	return {{"kind", "constant"}, {"codes", value.codes}};
}

inline data_json write_channel(const constant_translation& value)
{
	return {{"kind", "constant"}, {"value", value.value}};
}

inline data_json write_channel(const keyed_rotation& value)
{
	auto keys = data_json::array();
	for (const auto& key : value.keys)
	{
		keys.push_back({{"frame", key.frame}, {"codes", key.codes}});
	}

	return {{"kind", "keyed"}, {"keys", std::move(keys)}};
}

inline data_json write_channel(const keyed_translation& value)
{
	auto keys = data_json::array();
	for (const auto& key : value.keys)
	{
		keys.push_back({{"frame", key.frame}, {"codes", key.codes}});
	}

	return {{"kind", "keyed"}, {"origin", value.origin}, {"scale", value.scale}, {"code_bits", value.code_bits},
		{"keys", std::move(keys)}};
}

template <typename Channel>
data_json write_optional_channel(const std::optional<Channel>& channel)
{
	return channel ? std::visit([](const auto& value) { return write_channel(value); }, *channel) : data_json(nullptr);
}
}

inline detail::data_json write_xanim(const xanim& data)
{
	validate(data);
	auto bones = detail::data_json::array();
	for (const auto& bone : data.bones)
	{
		bones.push_back({{"name", bone.name}, {"rotation", std::visit([](const auto& value) { return detail::write_channel(value); }, bone.rotation)},
			{"translation", std::visit([](const auto& value) { return detail::write_channel(value); }, bone.translation)}});
	}

	auto delta = detail::data_json(nullptr);
	if (data.delta)
	{
		delta = {{"translation", detail::write_optional_channel(data.delta->translation)},
			{"rotation_2d", detail::write_optional_channel(data.delta->rotation_2d)},
			{"rotation_3d", detail::write_optional_channel(data.delta->rotation_3d)}};
	}

	auto notes = detail::data_json::array();
	for (const auto& note : data.notes)
	{
		notes.push_back({{"name", note.name}, {"time", note.time}});
	}

	return {{"format", "canonical-xanim"}, {"schema_version", 1}, {"name", data.name}, {"frames", data.frames},
		{"framerate", data.framerate}, {"frequency", data.frequency}, {"flags", data.flags}, {"bones", std::move(bones)},
		{"delta", std::move(delta)}, {"notes", std::move(notes)}, {"blend_shapes", data.blend_shapes},
		{"blend_shape_weights", data.blend_shape_weights}};
}
} // namespace zonetool::asset_data
