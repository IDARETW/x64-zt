#pragma once

#include "asset_data.hpp"
#include <array>
#include <cstdint>
#include <optional>
#include <span>
#include <stdexcept>
#include <string>
#include <vector>

// Neutral effect tables (family F7 of docs/NEUTRAL_ASSETS.md): which effect plays for a hit or a footstep on each kind
// of surface. An effect is named; a slot without an effect is empty.
namespace zonetool::asset_data
{
// The count of the surface kinds that an impact hits, of the flesh kinds, and of the registered impact types.
inline constexpr std::size_t impact_surface_kinds = 64;
inline constexpr std::size_t impact_flesh_kinds = 23;
inline constexpr std::size_t impact_registered_types = 33;
inline constexpr std::size_t surface_effect_kinds = 64;

// The effects of one impact on each kind of surface and each kind of flesh.
struct impact_effects
{
	std::array<std::optional<std::string>, impact_surface_kinds> surface;
	std::array<std::optional<std::string>, impact_flesh_kinds> flesh;

	bool operator==(const impact_effects&) const = default;
};

// An impact type: the effects of a hit that stays in the surface and of one that exits it, two sets of each.
struct impact_entry
{
	std::optional<std::array<impact_effects, 2>> normal;
	std::optional<std::array<impact_effects, 2>> exit;

	bool operator==(const impact_entry&) const = default;
};

// The impact effects of a weapon kind: one entry for each impact type, and the maps between the generic impact types and
// the registered ones.
struct impact_table
{
	std::string name;
	std::optional<std::vector<impact_entry>> entries;
	std::optional<std::vector<std::uint32_t>> generic_to_registered;
	std::optional<std::array<std::int32_t, impact_registered_types>> registered_to_generic;

	bool operator==(const impact_table&) const = default;
};

// The effects of a surface kind: the one that always plays and one for each kind of surface it meets.
struct surface_effect_entry
{
	std::optional<std::string> base;
	std::array<std::optional<std::string>, surface_effect_kinds> surface;

	bool operator==(const surface_effect_entry&) const = default;
};

// The surface effects of a map: an entry for each surface kind, and the size of the pool of the maps that the game
// builds for them at run time (the pool holds no data in the zone).
struct surface_effect_table
{
	std::string name;
	std::optional<std::vector<surface_effect_entry>> entries;
	std::int32_t map_pool_size{};
	bool has_map_pool{};

	bool operator==(const surface_effect_table&) const = default;
};

inline void validate(const impact_table& data)
{
	detail::name(data.name, "impactfx");
	const auto effect_names = [](const impact_effects& effects)
	{
		for (const auto& list : {std::span<const std::optional<std::string>>(effects.surface),
			std::span<const std::optional<std::string>>(effects.flesh)})
		{
			for (const auto& effect : list)
			{
				if (effect)
				{
					detail::name(*effect, "impactfx effect");
				}
			}
		}
	};

	std::size_t count = 0;
	if (data.entries)
	{
		count = data.entries->size();
		for (const auto& entry : *data.entries)
		{
			for (const auto* sets : {&entry.normal, &entry.exit})
			{
				if (*sets)
				{
					effect_names((**sets)[0]);
					effect_names((**sets)[1]);
				}
			}
		}
	}

	if (count > 0xFFFFFFFFull || (data.generic_to_registered && data.generic_to_registered->size() != count))
	{
		throw std::runtime_error("impactfx " + data.name + " has a generic map that is not one entry for each impact type");
	}
}

inline void validate(const surface_effect_table& data)
{
	detail::name(data.name, "surfacefx");
	if (data.map_pool_size < 0 || (data.map_pool_size != 0 && !data.has_map_pool) || (data.entries && data.entries->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("surfacefx " + data.name + " has a pool size without a pool or a size beyond its range");
	}

	if (data.entries)
	{
		for (const auto& entry : *data.entries)
		{
			if (entry.base)
			{
				detail::name(*entry.base, "surfacefx effect");
			}

			for (const auto& effect : entry.surface)
			{
				if (effect)
				{
					detail::name(*effect, "surfacefx effect");
				}
			}
		}
	}
}
} // namespace zonetool::asset_data
