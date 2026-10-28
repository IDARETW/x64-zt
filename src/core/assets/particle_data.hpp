#pragma once

#include "asset_data.hpp"
#include <array>
#include <cstdint>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

// Neutral particle system support (family F7 of docs/NEUTRAL_ASSETS.md): the two constructs of a vfx whose meaning depends
// on context, so that tools/gen_iw8_neutral.py cannot derive them from the structs alone.
namespace zonetool::asset_data
{
// What an event module of a particle system spawns or plays: the kind follows from the type of the module that holds the
// list (an init-model module lists models, a test module lists effects, and so on), and the names are those of the assets.
inline constexpr const char* particle_linked_kinds[] = {"material", "model", "light_def", "effect", "vector_field", "particle_sim",
	"sound", "decal", "physics_fx"};

// One entry of such a list. Most kinds name one asset; a decal names three materials; a physics effect names its
// pipeline (the first name) and its shape (the second).
struct particle_linked_asset
{
	std::array<std::optional<std::string>, 3> names;
	// For an effect: the selector data that follows the name in the entry (0x00010000 on the event modules that start child
	// effects, zero on the others). No other kind has been seen to use it.
	std::uint32_t selector{};

	bool operator==(const particle_linked_asset&) const = default;
};

struct particle_linked_asset_list
{
	std::string kind;
	std::optional<std::vector<particle_linked_asset>> assets;

	bool operator==(const particle_linked_asset_list&) const = default;
};

// A scripted input of a particle system: a switch for an emitter or a key value pair.
struct particle_scripted_input_node
{
	// "kvp" or "emitter_disable".
	std::string kind;
	std::uint32_t flags{};
	std::uint32_t emitter_index{};
	// For a key value pair only.
	std::uint32_t key{};
	bool dirty{};
	bool value{};

	bool operator==(const particle_scripted_input_node&) const = default;
};

inline void validate(const particle_linked_asset_list& data)
{
	if (std::ranges::find(particle_linked_kinds, data.kind) == std::end(particle_linked_kinds))
	{
		throw std::runtime_error("a particle linked asset list has an unknown kind");
	}

	if (data.assets)
	{
		for (const auto& asset : *data.assets)
		{
			for (const auto& name : asset.names)
			{
				if (name)
				{
					detail::name(*name, "particle linked asset");
				}
			}

			if (asset.selector && data.kind != "effect")
			{
				throw std::runtime_error("a particle linked asset other than an effect has selector data");
			}
		}
	}
}

inline void validate(const particle_scripted_input_node& data)
{
	if (data.kind != "kvp" && data.kind != "emitter_disable")
	{
		throw std::runtime_error("a scripted input node has an unknown kind");
	}

	if (data.kind == "emitter_disable" && (data.key || data.dirty || data.value))
	{
		throw std::runtime_error("an emitter disable node has a key value pair");
	}
}
} // namespace zonetool::asset_data
