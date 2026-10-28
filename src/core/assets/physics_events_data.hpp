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

// Neutral physics_events assets (docs/NEUTRAL_ASSETS.md), emitted by tools/gen_iw8_neutral.py from the IW8 structs and stream
// traversals: do not edit; change the spec (tools/neutral_families/physics_events.json) or the generator.
namespace zonetool::asset_data
{
namespace physics_events
{
struct physics_sfx_event;
struct physics_sfx_event_asset_rules;
struct physics_sfx_event_asset_rule;
struct physics_sfx_event_asset_sound_rule;
struct physics_sfx_event_asset_material_rule;
struct physics_sfx_event_asset_material_entry;
struct physics_sfx_event_asset_angle_rule;
struct physics_sfx_event_asset_angle_entry;
struct physics_sfx_event_asset_speed_rule;
struct physics_sfx_event_asset_speed_entry;
struct physics_sfx_event_asset_mass_rule;
struct physics_sfx_event_asset_mass_entry;
struct physics_sfx_event_asset_gravity_rule;
struct physics_sfx_event_asset_gravity_entry;
struct physics_sfx_event_asset_atmosphere_rule;
struct physics_sfx_event_asset_atmosphere_entry;
struct physics_sfx_event_asset_camera_distance_rule;
struct physics_sfx_event_asset_camera_distance_entry;
struct physics_sfx_event_asset_frequency_rule;
struct physics_sfx_event_asset_frequency_entry;
struct physics_vfx_event;
struct physics_vfx_event_asset_rules;
struct physics_vfx_event_asset_rule;
struct physics_vfx_event_asset_particle_effect_rule;
struct physics_vfx_event_asset_material_rule;
struct physics_vfx_event_asset_material_entry;
struct physics_vfx_event_asset_angle_rule;
struct physics_vfx_event_asset_angle_entry;
struct physics_vfx_event_asset_speed_rule;
struct physics_vfx_event_asset_speed_entry;
struct physics_vfx_event_asset_mass_rule;
struct physics_vfx_event_asset_mass_entry;
struct physics_vfx_event_asset_gravity_rule;
struct physics_vfx_event_asset_gravity_entry;
struct physics_vfx_event_asset_atmosphere_rule;
struct physics_vfx_event_asset_atmosphere_entry;
struct physics_vfx_event_asset_camera_distance_rule;
struct physics_vfx_event_asset_camera_distance_entry;
struct physics_vfx_event_asset_frequency_rule;
struct physics_vfx_event_asset_frequency_entry;

inline void validate(const physics_sfx_event& data);
inline void validate(const physics_sfx_event_asset_rules& data);
inline void validate(const physics_sfx_event_asset_rule& data);
inline void validate(const physics_sfx_event_asset_sound_rule& data);
inline void validate(const physics_sfx_event_asset_material_rule& data);
inline void validate(const physics_sfx_event_asset_material_entry& data);
inline void validate(const physics_sfx_event_asset_angle_rule& data);
inline void validate(const physics_sfx_event_asset_angle_entry& data);
inline void validate(const physics_sfx_event_asset_speed_rule& data);
inline void validate(const physics_sfx_event_asset_speed_entry& data);
inline void validate(const physics_sfx_event_asset_mass_rule& data);
inline void validate(const physics_sfx_event_asset_mass_entry& data);
inline void validate(const physics_sfx_event_asset_gravity_rule& data);
inline void validate(const physics_sfx_event_asset_gravity_entry& data);
inline void validate(const physics_sfx_event_asset_atmosphere_rule& data);
inline void validate(const physics_sfx_event_asset_atmosphere_entry& data);
inline void validate(const physics_sfx_event_asset_camera_distance_rule& data);
inline void validate(const physics_sfx_event_asset_camera_distance_entry& data);
inline void validate(const physics_sfx_event_asset_frequency_rule& data);
inline void validate(const physics_sfx_event_asset_frequency_entry& data);
inline void validate(const physics_vfx_event& data);
inline void validate(const physics_vfx_event_asset_rules& data);
inline void validate(const physics_vfx_event_asset_rule& data);
inline void validate(const physics_vfx_event_asset_particle_effect_rule& data);
inline void validate(const physics_vfx_event_asset_material_rule& data);
inline void validate(const physics_vfx_event_asset_material_entry& data);
inline void validate(const physics_vfx_event_asset_angle_rule& data);
inline void validate(const physics_vfx_event_asset_angle_entry& data);
inline void validate(const physics_vfx_event_asset_speed_rule& data);
inline void validate(const physics_vfx_event_asset_speed_entry& data);
inline void validate(const physics_vfx_event_asset_mass_rule& data);
inline void validate(const physics_vfx_event_asset_mass_entry& data);
inline void validate(const physics_vfx_event_asset_gravity_rule& data);
inline void validate(const physics_vfx_event_asset_gravity_entry& data);
inline void validate(const physics_vfx_event_asset_atmosphere_rule& data);
inline void validate(const physics_vfx_event_asset_atmosphere_entry& data);
inline void validate(const physics_vfx_event_asset_camera_distance_rule& data);
inline void validate(const physics_vfx_event_asset_camera_distance_entry& data);
inline void validate(const physics_vfx_event_asset_frequency_rule& data);
inline void validate(const physics_vfx_event_asset_frequency_entry& data);

struct physics_sfx_event_asset_rules
{
	std::int32_t numrules{};
	std::optional<std::vector<physics_sfx_event_asset_rule>> rules{};

	bool operator==(const physics_sfx_event_asset_rules&) const = default;
};

struct physics_sfx_event
{
	std::optional<std::string> name{};
	std::int32_t priority{};
	physics_sfx_event_asset_rules rules{};

	bool operator==(const physics_sfx_event&) const = default;
};

struct physics_sfx_event_asset_sound_rule
{
	std::optional<std::string> hit_sound_alias{};
	std::optional<std::string> scrape_sound_alias{};
	float scrape_sound_volume_speed_threshold_min{};
	float scrape_sound_volume_speed_threshold_max{};
	float scrape_sound_volume_blend_speed{};

	bool operator==(const physics_sfx_event_asset_sound_rule&) const = default;
};

struct physics_sfx_event_asset_material_rule
{
	std::int32_t numentries{};
	std::optional<std::vector<physics_sfx_event_asset_material_entry>> entries{};

	bool operator==(const physics_sfx_event_asset_material_rule&) const = default;
};

struct physics_sfx_event_asset_angle_rule
{
	std::int32_t numentries{};
	std::optional<std::vector<physics_sfx_event_asset_angle_entry>> entries{};

	bool operator==(const physics_sfx_event_asset_angle_rule&) const = default;
};

struct physics_sfx_event_asset_speed_rule
{
	std::int32_t numentries{};
	std::optional<std::vector<physics_sfx_event_asset_speed_entry>> entries{};

	bool operator==(const physics_sfx_event_asset_speed_rule&) const = default;
};

struct physics_sfx_event_asset_mass_rule
{
	std::int32_t numentries{};
	std::optional<std::vector<physics_sfx_event_asset_mass_entry>> entries{};

	bool operator==(const physics_sfx_event_asset_mass_rule&) const = default;
};

struct physics_sfx_event_asset_gravity_rule
{
	std::int32_t numentries{};
	std::optional<std::vector<physics_sfx_event_asset_gravity_entry>> entries{};

	bool operator==(const physics_sfx_event_asset_gravity_rule&) const = default;
};

struct physics_sfx_event_asset_atmosphere_rule
{
	std::int32_t numentries{};
	std::optional<std::vector<physics_sfx_event_asset_atmosphere_entry>> entries{};

	bool operator==(const physics_sfx_event_asset_atmosphere_rule&) const = default;
};

struct physics_sfx_event_asset_camera_distance_rule
{
	std::int32_t numentries{};
	std::optional<std::vector<physics_sfx_event_asset_camera_distance_entry>> entries{};

	bool operator==(const physics_sfx_event_asset_camera_distance_rule&) const = default;
};

struct physics_sfx_event_asset_frequency_rule
{
	std::int32_t numentries{};
	std::optional<std::vector<physics_sfx_event_asset_frequency_entry>> entries{};

	bool operator==(const physics_sfx_event_asset_frequency_rule&) const = default;
};

struct physics_sfx_event_asset_rule
{
	std::variant<std::monostate, physics_sfx_event_asset_sound_rule, physics_sfx_event_asset_material_rule, physics_sfx_event_asset_angle_rule, physics_sfx_event_asset_speed_rule, physics_sfx_event_asset_mass_rule, physics_sfx_event_asset_gravity_rule, physics_sfx_event_asset_atmosphere_rule, physics_sfx_event_asset_camera_distance_rule, physics_sfx_event_asset_frequency_rule> u{};

	bool operator==(const physics_sfx_event_asset_rule&) const = default;
};

struct physics_sfx_event_asset_material_entry
{
	std::int32_t surface_type{};
	physics_sfx_event_asset_rules rules{};

	bool operator==(const physics_sfx_event_asset_material_entry&) const = default;
};

struct physics_sfx_event_asset_angle_entry
{
	float angle{};
	float hit_sound_volume_modifier_min{};
	float hit_sound_volume_modifier_max{};
	float hit_sound_pitch_modifier_min{};
	float hit_sound_pitch_modifier_max{};
	physics_sfx_event_asset_rules rules{};

	bool operator==(const physics_sfx_event_asset_angle_entry&) const = default;
};

struct physics_sfx_event_asset_speed_entry
{
	float speed_threshold{};
	float hit_sound_volume_modifier_min{};
	float hit_sound_volume_modifier_max{};
	float hit_sound_pitch_modifier_min{};
	float hit_sound_pitch_modifier_max{};
	physics_sfx_event_asset_rules rules{};

	bool operator==(const physics_sfx_event_asset_speed_entry&) const = default;
};

struct physics_sfx_event_asset_mass_entry
{
	float mass_threshold{};
	float hit_sound_volume_modifier_min{};
	float hit_sound_volume_modifier_max{};
	float hit_sound_pitch_modifier_min{};
	float hit_sound_pitch_modifier_max{};
	physics_sfx_event_asset_rules rules{};

	bool operator==(const physics_sfx_event_asset_mass_entry&) const = default;
};

struct physics_sfx_event_asset_gravity_entry
{
	bool gravity{};
	physics_sfx_event_asset_rules rules{};

	bool operator==(const physics_sfx_event_asset_gravity_entry&) const = default;
};

struct physics_sfx_event_asset_atmosphere_entry
{
	bool atmosphere{};
	physics_sfx_event_asset_rules rules{};

	bool operator==(const physics_sfx_event_asset_atmosphere_entry&) const = default;
};

struct physics_sfx_event_asset_camera_distance_entry
{
	float distance{};
	physics_sfx_event_asset_rules rules{};

	bool operator==(const physics_sfx_event_asset_camera_distance_entry&) const = default;
};

struct physics_sfx_event_asset_frequency_entry
{
	std::int32_t max_events{};
	physics_sfx_event_asset_rules rules{};

	bool operator==(const physics_sfx_event_asset_frequency_entry&) const = default;
};

struct physics_vfx_event_asset_rules
{
	std::int32_t numrules{};
	std::optional<std::vector<physics_vfx_event_asset_rule>> rules{};

	bool operator==(const physics_vfx_event_asset_rules&) const = default;
};

struct physics_vfx_event
{
	std::optional<std::string> name{};
	std::int32_t priority{};
	physics_vfx_event_asset_rules rules{};

	bool operator==(const physics_vfx_event&) const = default;
};

struct physics_vfx_event_asset_particle_effect_rule
{
	std::optional<std::string> hit_particle_effect_alias{};
	std::optional<std::string> scrape_particle_effect_alias{};

	bool operator==(const physics_vfx_event_asset_particle_effect_rule&) const = default;
};

struct physics_vfx_event_asset_material_rule
{
	std::int32_t numentries{};
	std::optional<std::vector<physics_vfx_event_asset_material_entry>> entries{};

	bool operator==(const physics_vfx_event_asset_material_rule&) const = default;
};

struct physics_vfx_event_asset_angle_rule
{
	std::int32_t numentries{};
	std::optional<std::vector<physics_vfx_event_asset_angle_entry>> entries{};

	bool operator==(const physics_vfx_event_asset_angle_rule&) const = default;
};

struct physics_vfx_event_asset_speed_rule
{
	std::int32_t numentries{};
	std::optional<std::vector<physics_vfx_event_asset_speed_entry>> entries{};

	bool operator==(const physics_vfx_event_asset_speed_rule&) const = default;
};

struct physics_vfx_event_asset_mass_rule
{
	std::int32_t numentries{};
	std::optional<std::vector<physics_vfx_event_asset_mass_entry>> entries{};

	bool operator==(const physics_vfx_event_asset_mass_rule&) const = default;
};

struct physics_vfx_event_asset_gravity_rule
{
	std::int32_t numentries{};
	std::optional<std::vector<physics_vfx_event_asset_gravity_entry>> entries{};

	bool operator==(const physics_vfx_event_asset_gravity_rule&) const = default;
};

struct physics_vfx_event_asset_atmosphere_rule
{
	std::int32_t numentries{};
	std::optional<std::vector<physics_vfx_event_asset_atmosphere_entry>> entries{};

	bool operator==(const physics_vfx_event_asset_atmosphere_rule&) const = default;
};

struct physics_vfx_event_asset_camera_distance_rule
{
	std::int32_t numentries{};
	std::optional<std::vector<physics_vfx_event_asset_camera_distance_entry>> entries{};

	bool operator==(const physics_vfx_event_asset_camera_distance_rule&) const = default;
};

struct physics_vfx_event_asset_frequency_rule
{
	std::int32_t numentries{};
	std::optional<std::vector<physics_vfx_event_asset_frequency_entry>> entries{};

	bool operator==(const physics_vfx_event_asset_frequency_rule&) const = default;
};

struct physics_vfx_event_asset_rule
{
	std::variant<std::monostate, physics_vfx_event_asset_particle_effect_rule, physics_vfx_event_asset_material_rule, physics_vfx_event_asset_angle_rule, physics_vfx_event_asset_speed_rule, physics_vfx_event_asset_mass_rule, physics_vfx_event_asset_gravity_rule, physics_vfx_event_asset_atmosphere_rule, physics_vfx_event_asset_camera_distance_rule, physics_vfx_event_asset_frequency_rule> u{};

	bool operator==(const physics_vfx_event_asset_rule&) const = default;
};

struct physics_vfx_event_asset_material_entry
{
	std::int32_t surface_type{};
	physics_vfx_event_asset_rules rules{};

	bool operator==(const physics_vfx_event_asset_material_entry&) const = default;
};

struct physics_vfx_event_asset_angle_entry
{
	float angle{};
	physics_vfx_event_asset_rules rules{};

	bool operator==(const physics_vfx_event_asset_angle_entry&) const = default;
};

struct physics_vfx_event_asset_speed_entry
{
	float speed_threshold{};
	physics_vfx_event_asset_rules rules{};

	bool operator==(const physics_vfx_event_asset_speed_entry&) const = default;
};

struct physics_vfx_event_asset_mass_entry
{
	float mass_threshold{};
	physics_vfx_event_asset_rules rules{};

	bool operator==(const physics_vfx_event_asset_mass_entry&) const = default;
};

struct physics_vfx_event_asset_gravity_entry
{
	bool gravity{};
	physics_vfx_event_asset_rules rules{};

	bool operator==(const physics_vfx_event_asset_gravity_entry&) const = default;
};

struct physics_vfx_event_asset_atmosphere_entry
{
	bool atmosphere{};
	physics_vfx_event_asset_rules rules{};

	bool operator==(const physics_vfx_event_asset_atmosphere_entry&) const = default;
};

struct physics_vfx_event_asset_camera_distance_entry
{
	float distance{};
	physics_vfx_event_asset_rules rules{};

	bool operator==(const physics_vfx_event_asset_camera_distance_entry&) const = default;
};

struct physics_vfx_event_asset_frequency_entry
{
	std::int32_t max_events{};
	physics_vfx_event_asset_rules rules{};

	bool operator==(const physics_vfx_event_asset_frequency_entry&) const = default;
};

inline void validate(const physics_sfx_event_asset_rules& data)
{
	if (data.rules)
	{
		for (const auto& item : *data.rules)
		{
			validate(item);
		}
	}

	if (static_cast<std::int64_t>(data.numrules) != (data.rules ? static_cast<std::int64_t>(data.rules->size()) : 0))
	{
		throw std::runtime_error("physics_sfx_event_asset_rules.rules has a length that its count does not give");
	}

}

inline void validate(const physics_sfx_event& data)
{
	if (!data.name)
	{
		throw std::runtime_error("physics_sfx_event has no name");
	}

	detail::name(*data.name, "physics_sfx_event");

	validate(data.rules);

}

inline void validate(const physics_sfx_event_asset_sound_rule& data)
{
	(void) data;
}

inline void validate(const physics_sfx_event_asset_material_rule& data)
{
	if (data.entries)
	{
		for (const auto& item : *data.entries)
		{
			validate(item);
		}
	}

	if (static_cast<std::int64_t>(data.numentries) != (data.entries ? static_cast<std::int64_t>(data.entries->size()) : 0))
	{
		throw std::runtime_error("physics_sfx_event_asset_material_rule.entries has a length that its count does not give");
	}

}

inline void validate(const physics_sfx_event_asset_angle_rule& data)
{
	if (data.entries)
	{
		for (const auto& item : *data.entries)
		{
			validate(item);
		}
	}

	if (static_cast<std::int64_t>(data.numentries) != (data.entries ? static_cast<std::int64_t>(data.entries->size()) : 0))
	{
		throw std::runtime_error("physics_sfx_event_asset_angle_rule.entries has a length that its count does not give");
	}

}

inline void validate(const physics_sfx_event_asset_speed_rule& data)
{
	if (data.entries)
	{
		for (const auto& item : *data.entries)
		{
			validate(item);
		}
	}

	if (static_cast<std::int64_t>(data.numentries) != (data.entries ? static_cast<std::int64_t>(data.entries->size()) : 0))
	{
		throw std::runtime_error("physics_sfx_event_asset_speed_rule.entries has a length that its count does not give");
	}

}

inline void validate(const physics_sfx_event_asset_mass_rule& data)
{
	if (data.entries)
	{
		for (const auto& item : *data.entries)
		{
			validate(item);
		}
	}

	if (static_cast<std::int64_t>(data.numentries) != (data.entries ? static_cast<std::int64_t>(data.entries->size()) : 0))
	{
		throw std::runtime_error("physics_sfx_event_asset_mass_rule.entries has a length that its count does not give");
	}

}

inline void validate(const physics_sfx_event_asset_gravity_rule& data)
{
	if (data.entries)
	{
		for (const auto& item : *data.entries)
		{
			validate(item);
		}
	}

	if (static_cast<std::int64_t>(data.numentries) != (data.entries ? static_cast<std::int64_t>(data.entries->size()) : 0))
	{
		throw std::runtime_error("physics_sfx_event_asset_gravity_rule.entries has a length that its count does not give");
	}

}

inline void validate(const physics_sfx_event_asset_atmosphere_rule& data)
{
	if (data.entries)
	{
		for (const auto& item : *data.entries)
		{
			validate(item);
		}
	}

	if (static_cast<std::int64_t>(data.numentries) != (data.entries ? static_cast<std::int64_t>(data.entries->size()) : 0))
	{
		throw std::runtime_error("physics_sfx_event_asset_atmosphere_rule.entries has a length that its count does not give");
	}

}

inline void validate(const physics_sfx_event_asset_camera_distance_rule& data)
{
	if (data.entries)
	{
		for (const auto& item : *data.entries)
		{
			validate(item);
		}
	}

	if (static_cast<std::int64_t>(data.numentries) != (data.entries ? static_cast<std::int64_t>(data.entries->size()) : 0))
	{
		throw std::runtime_error("physics_sfx_event_asset_camera_distance_rule.entries has a length that its count does not give");
	}

}

inline void validate(const physics_sfx_event_asset_frequency_rule& data)
{
	if (data.entries)
	{
		for (const auto& item : *data.entries)
		{
			validate(item);
		}
	}

	if (static_cast<std::int64_t>(data.numentries) != (data.entries ? static_cast<std::int64_t>(data.entries->size()) : 0))
	{
		throw std::runtime_error("physics_sfx_event_asset_frequency_rule.entries has a length that its count does not give");
	}

}

inline void validate(const physics_sfx_event_asset_rule& data)
{
	std::visit([](const auto& value)
	{
		if constexpr (std::is_same_v<std::decay_t<decltype(value)>, std::monostate>)
		{
			throw std::runtime_error("physics_sfx_event_asset_rule.u holds no module");
		}
		else
		{
			validate(value);
		}
	}, data.u);

}

inline void validate(const physics_sfx_event_asset_material_entry& data)
{
	validate(data.rules);

}

inline void validate(const physics_sfx_event_asset_angle_entry& data)
{
	validate(data.rules);

}

inline void validate(const physics_sfx_event_asset_speed_entry& data)
{
	validate(data.rules);

}

inline void validate(const physics_sfx_event_asset_mass_entry& data)
{
	validate(data.rules);

}

inline void validate(const physics_sfx_event_asset_gravity_entry& data)
{
	validate(data.rules);

}

inline void validate(const physics_sfx_event_asset_atmosphere_entry& data)
{
	validate(data.rules);

}

inline void validate(const physics_sfx_event_asset_camera_distance_entry& data)
{
	validate(data.rules);

}

inline void validate(const physics_sfx_event_asset_frequency_entry& data)
{
	validate(data.rules);

}

inline void validate(const physics_vfx_event_asset_rules& data)
{
	if (data.rules)
	{
		for (const auto& item : *data.rules)
		{
			validate(item);
		}
	}

	if (static_cast<std::int64_t>(data.numrules) != (data.rules ? static_cast<std::int64_t>(data.rules->size()) : 0))
	{
		throw std::runtime_error("physics_vfx_event_asset_rules.rules has a length that its count does not give");
	}

}

inline void validate(const physics_vfx_event& data)
{
	if (!data.name)
	{
		throw std::runtime_error("physics_vfx_event has no name");
	}

	detail::name(*data.name, "physics_vfx_event");

	validate(data.rules);

}

inline void validate(const physics_vfx_event_asset_particle_effect_rule& data)
{
	(void) data;
}

inline void validate(const physics_vfx_event_asset_material_rule& data)
{
	if (data.entries)
	{
		for (const auto& item : *data.entries)
		{
			validate(item);
		}
	}

	if (static_cast<std::int64_t>(data.numentries) != (data.entries ? static_cast<std::int64_t>(data.entries->size()) : 0))
	{
		throw std::runtime_error("physics_vfx_event_asset_material_rule.entries has a length that its count does not give");
	}

}

inline void validate(const physics_vfx_event_asset_angle_rule& data)
{
	if (data.entries)
	{
		for (const auto& item : *data.entries)
		{
			validate(item);
		}
	}

	if (static_cast<std::int64_t>(data.numentries) != (data.entries ? static_cast<std::int64_t>(data.entries->size()) : 0))
	{
		throw std::runtime_error("physics_vfx_event_asset_angle_rule.entries has a length that its count does not give");
	}

}

inline void validate(const physics_vfx_event_asset_speed_rule& data)
{
	if (data.entries)
	{
		for (const auto& item : *data.entries)
		{
			validate(item);
		}
	}

	if (static_cast<std::int64_t>(data.numentries) != (data.entries ? static_cast<std::int64_t>(data.entries->size()) : 0))
	{
		throw std::runtime_error("physics_vfx_event_asset_speed_rule.entries has a length that its count does not give");
	}

}

inline void validate(const physics_vfx_event_asset_mass_rule& data)
{
	if (data.entries)
	{
		for (const auto& item : *data.entries)
		{
			validate(item);
		}
	}

	if (static_cast<std::int64_t>(data.numentries) != (data.entries ? static_cast<std::int64_t>(data.entries->size()) : 0))
	{
		throw std::runtime_error("physics_vfx_event_asset_mass_rule.entries has a length that its count does not give");
	}

}

inline void validate(const physics_vfx_event_asset_gravity_rule& data)
{
	if (data.entries)
	{
		for (const auto& item : *data.entries)
		{
			validate(item);
		}
	}

	if (static_cast<std::int64_t>(data.numentries) != (data.entries ? static_cast<std::int64_t>(data.entries->size()) : 0))
	{
		throw std::runtime_error("physics_vfx_event_asset_gravity_rule.entries has a length that its count does not give");
	}

}

inline void validate(const physics_vfx_event_asset_atmosphere_rule& data)
{
	if (data.entries)
	{
		for (const auto& item : *data.entries)
		{
			validate(item);
		}
	}

	if (static_cast<std::int64_t>(data.numentries) != (data.entries ? static_cast<std::int64_t>(data.entries->size()) : 0))
	{
		throw std::runtime_error("physics_vfx_event_asset_atmosphere_rule.entries has a length that its count does not give");
	}

}

inline void validate(const physics_vfx_event_asset_camera_distance_rule& data)
{
	if (data.entries)
	{
		for (const auto& item : *data.entries)
		{
			validate(item);
		}
	}

	if (static_cast<std::int64_t>(data.numentries) != (data.entries ? static_cast<std::int64_t>(data.entries->size()) : 0))
	{
		throw std::runtime_error("physics_vfx_event_asset_camera_distance_rule.entries has a length that its count does not give");
	}

}

inline void validate(const physics_vfx_event_asset_frequency_rule& data)
{
	if (data.entries)
	{
		for (const auto& item : *data.entries)
		{
			validate(item);
		}
	}

	if (static_cast<std::int64_t>(data.numentries) != (data.entries ? static_cast<std::int64_t>(data.entries->size()) : 0))
	{
		throw std::runtime_error("physics_vfx_event_asset_frequency_rule.entries has a length that its count does not give");
	}

}

inline void validate(const physics_vfx_event_asset_rule& data)
{
	std::visit([](const auto& value)
	{
		if constexpr (std::is_same_v<std::decay_t<decltype(value)>, std::monostate>)
		{
			throw std::runtime_error("physics_vfx_event_asset_rule.u holds no module");
		}
		else
		{
			validate(value);
		}
	}, data.u);

}

inline void validate(const physics_vfx_event_asset_material_entry& data)
{
	validate(data.rules);

}

inline void validate(const physics_vfx_event_asset_angle_entry& data)
{
	validate(data.rules);

}

inline void validate(const physics_vfx_event_asset_speed_entry& data)
{
	validate(data.rules);

}

inline void validate(const physics_vfx_event_asset_mass_entry& data)
{
	validate(data.rules);

}

inline void validate(const physics_vfx_event_asset_gravity_entry& data)
{
	validate(data.rules);

}

inline void validate(const physics_vfx_event_asset_atmosphere_entry& data)
{
	validate(data.rules);

}

inline void validate(const physics_vfx_event_asset_camera_distance_entry& data)
{
	validate(data.rules);

}

inline void validate(const physics_vfx_event_asset_frequency_entry& data)
{
	validate(data.rules);

}

} // namespace physics_events
} // namespace zonetool::asset_data
