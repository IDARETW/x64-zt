#pragma once

#include "asset_data.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

// Neutral physics payload assets (families F4 and F7 of docs/NEUTRAL_ASSETS.md): the shape of a physics effect and the
// detail collision of a model. The shapes are Havok tagfiles; the neutral data keeps the file and the SDK version it names.
namespace zonetool::asset_data
{
// A Havok binary tagfile: a chunk "TAG0" whose "SDKV" chunk names the SDK that wrote it, e.g. "20180200". The bytes are
// the file; a writer for another SDK version has to convert the shape, so the version travels with them.
struct havok_tagfile
{
	std::string sdk_version;
	std::vector<std::uint8_t> bytes;

	bool operator==(const havok_tagfile&) const = default;
};

// The SDK version that a tagfile's SDKV chunk names, or an empty string when the bytes are not a tagfile.
inline std::string havok_tagfile_version(const std::vector<std::uint8_t>& bytes)
{
	// A chunk is a big-endian size (with flags in the top bits) and a four-character tag; the first chunk is TAG0.
	if (bytes.size() < 16 || std::string_view(reinterpret_cast<const char*>(bytes.data()) + 4, 4) != "TAG0")
	{
		return {};
	}

	// The first child of TAG0 is the SDKV chunk: its tag at byte 12 of the file, then the version text.
	if (std::string_view(reinterpret_cast<const char*>(bytes.data()) + 12, 4) != "SDKV" || bytes.size() < 24)
	{
		return {};
	}

	return std::string(reinterpret_cast<const char*>(bytes.data()) + 16, 8);
}

// What a physics asset is used for, and how each of its bodies is simulated, by name.
inline constexpr const char* physics_use_categories[] = {"static_model", "common_entity", "vehicle", "dynent", "entity_or_dynent",
	"ragdoll", "particle", "custom"};
inline constexpr const char* physics_simulation_categories[] = {"static", "keyframed_entity", "keyframed_animated",
	"keyframed_bone_offset", "dynamic_entity", "dynamic_debris", "ragdoll", "vehicle_chassis", "vehicle_chassis_no_force", "custom"};

// A rigid body of a physics asset: how it is simulated and the contents (collision classes) it has.
struct physics_body
{
	std::string simulation_category;
	std::uint32_t contents{};

	bool operator==(const physics_body&) const = default;
};

// How many entities of each kind use a physics asset; the game counts them at run time.
struct physics_usage_counter
{
	std::int32_t server_entities{};
	std::int32_t client_entities{};
	std::int32_t dynent_entities{};

	bool operator==(const physics_usage_counter&) const = default;
};

// The physics of a model: the Havok bodies and constraints, how they are used and simulated, and the sound and effect
// events that they trigger (by asset name).
struct physics_asset
{
	std::string name;
	havok_tagfile shape;
	std::string use_category;
	std::int32_t constraint_count{};
	// Absent when the asset has no body arrays.
	std::optional<std::vector<physics_body>> bodies;
	std::optional<std::vector<std::optional<std::string>>> sound_events;
	std::optional<std::vector<std::optional<std::string>>> effect_events;
	physics_usage_counter usage;
	bool contains_dynamic_bodies{};

	bool operator==(const physics_asset&) const = default;
};

// The shape of a physics effect and the sound it makes when it hits: the impulse range maps to the volume range.
struct physics_fx_shape
{
	std::string name;
	havok_tagfile shape;
	std::optional<std::string> sound_alias;
	float sound_min_impulse{};
	float sound_max_impulse{};
	float sound_min_impulse_volume{};
	float sound_max_impulse_volume{};

	bool operator==(const physics_fx_shape&) const = default;
};

// The collision of the details of a model (its LOD shapes) and the names of its parts, in the order of the shape.
struct detail_collision
{
	std::string name;
	havok_tagfile shape;
	std::vector<std::string> part_names;

	bool operator==(const detail_collision&) const = default;
};

inline void validate(const havok_tagfile& data, const std::string& what)
{
	if (data.bytes.empty())
	{
		if (!data.sdk_version.empty())
		{
			throw std::runtime_error(what + " names an SDK version for no tagfile");
		}

		return;
	}

	if (havok_tagfile_version(data.bytes) != data.sdk_version || data.sdk_version.empty())
	{
		throw std::runtime_error(what + " holds bytes that are not a tagfile of the SDK version it names");
	}

	if (data.bytes.size() > 0xFFFFFFFFull)
	{
		throw std::runtime_error(what + " holds a tagfile larger than 4 GiB");
	}
}

inline void validate(const physics_asset& data)
{
	detail::name(data.name, "physicsasset");
	validate(data.shape, "physicsasset " + data.name);
	const auto fail = [&data](const std::string& what) { throw std::runtime_error("physicsasset " + data.name + " has " + what); };
	if (std::ranges::find(physics_use_categories, data.use_category) == std::end(physics_use_categories))
	{
		fail("an unknown use category");
	}

	if (data.constraint_count < 0 || (data.bodies && data.bodies->size() > 0x7FFFFFFFull) ||
		(data.sound_events && data.sound_events->size() > 0x7FFFFFFFull) || (data.effect_events && data.effect_events->size() > 0x7FFFFFFFull))
	{
		fail("a count that is negative or beyond its range");
	}

	if (data.bodies)
	{
		for (const auto& body : *data.bodies)
		{
			if (std::ranges::find(physics_simulation_categories, body.simulation_category) == std::end(physics_simulation_categories))
			{
				fail("a body with an unknown simulation category");
			}
		}
	}

	for (const auto* events : {&data.sound_events, &data.effect_events})
	{
		if (*events)
		{
			for (const auto& event : **events)
			{
				if (event)
				{
					detail::name(*event, "physicsasset event");
				}
			}
		}
	}
}

inline void validate(const physics_fx_shape& data)
{
	detail::name(data.name, "physicsfxshape");
	validate(data.shape, "physicsfxshape " + data.name);
	if (data.sound_alias)
	{
		detail::name(*data.sound_alias, "physicsfxshape sound alias");
	}

	if (!std::isfinite(data.sound_min_impulse) || !std::isfinite(data.sound_max_impulse) ||
		!std::isfinite(data.sound_min_impulse_volume) || !std::isfinite(data.sound_max_impulse_volume))
	{
		throw std::runtime_error("physicsfxshape " + data.name + " has a sound range that is not finite");
	}
}

inline void validate(const detail_collision& data)
{
	detail::name(data.name, "xmodeldetailcollision");
	validate(data.shape, "xmodeldetailcollision " + data.name);
	if (data.part_names.size() > 0xFFFFFFFFull)
	{
		throw std::runtime_error("xmodeldetailcollision " + data.name + " has more part names than a count holds");
	}

	for (const auto& part : data.part_names)
	{
		if (part.empty())
		{
			throw std::runtime_error("xmodeldetailcollision " + data.name + " has an empty part name");
		}
	}
}
} // namespace zonetool::asset_data
