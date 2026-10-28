#pragma once

#include "asset_data.hpp"
#include "asm_param_data.hpp"
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

// Neutral aiasm assets (docs/NEUTRAL_ASSETS.md), emitted by tools/gen_iw8_neutral.py from the IW8 structs and stream
// traversals: do not edit; change the spec (tools/neutral_families/aiasm.json) or the generator.
namespace zonetool::asset_data
{
namespace aiasm
{
inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 7> animset_state_type_names{{
	{0, "default"},
	{1, "aimset"},
	{2, "gesture"},
	{3, "shoot"},
	{4, "lookset"},
	{5, "execution_attacker"},
	{6, "execution_victim"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 4> animation_controller_names{{
	{0, "none"},
	{1, "player"},
	{2, "dog"},
	{3, "num"},
}};

struct aiasm;
struct asm_state;
struct asm_transition;
struct asm_function;
struct asm_state_m_ai_state;
struct ai_asm_state;
struct asm_logic_group;
struct asm_m_aiasm;
struct ai_asm;
struct aianimset;
struct animset_state;
struct animset_alias;
struct animset_anim;
struct animset_alias_m_ai_animset_alias;
struct ai_animset_alias;
struct animset_blendtree_alias;
struct animset_state_entry_map;
struct animset_state_m_ai_animset_state;
struct ai_animset_state;
struct animset_m_ai_animset;
struct ai_animset;
struct ai_animset_node_offset;

inline void validate(const aiasm& data);
inline void validate(const asm_state& data);
inline void validate(const asm_transition& data);
inline void validate(const asm_function& data);
inline void validate(const asm_state_m_ai_state& data);
inline void validate(const ai_asm_state& data);
inline void validate(const asm_logic_group& data);
inline void validate(const asm_m_aiasm& data);
inline void validate(const ai_asm& data);
inline void validate(const aianimset& data);
inline void validate(const animset_state& data);
inline void validate(const animset_alias& data);
inline void validate(const animset_anim& data);
inline void validate(const animset_alias_m_ai_animset_alias& data);
inline void validate(const ai_animset_alias& data);
inline void validate(const animset_blendtree_alias& data);
inline void validate(const animset_state_entry_map& data);
inline void validate(const animset_state_m_ai_animset_state& data);
inline void validate(const ai_animset_state& data);
inline void validate(const animset_m_ai_animset& data);
inline void validate(const ai_animset& data);
inline void validate(const ai_animset_node_offset& data);

struct ai_asm
{
	std::optional<std::vector<std::int32_t>> services{};
	std::int32_t script_handler{};

	bool operator==(const ai_asm&) const = default;
};

struct asm_m_aiasm
{
	std::optional<ai_asm> aiasm{};

	bool operator==(const asm_m_aiasm&) const = default;
};

struct aiasm
{
	std::optional<std::string> sz_name{};
	std::optional<std::string> name{};
	std::optional<std::vector<asm_state>> states{};
	std::optional<std::vector<asm_logic_group>> logic_groups{};
	std::optional<std::vector<std::int32_t>> func_i_ds{};
	std::int32_t start_state{};
	std::variant<std::monostate, asm_m_aiasm> u{};

	bool operator==(const aiasm&) const = default;
};

struct asm_function
{
	std::int32_t func_id{};
	std::optional<std::vector<asm_function_param>> params{};
	bool b_builtin{};
	bool b_negate{};
	std::uint8_t flags{};
	std::uint16_t hash{};

	bool operator==(const asm_function&) const = default;
};

struct ai_asm_state
{
	std::int32_t power_down_state{};
	std::optional<std::vector<asm_function_param>> power_down_params{};
	std::optional<std::string> anim_mode{};
	std::optional<std::string> orient_mode{};
	std::optional<std::string> anim_mode_mp{};
	std::optional<std::string> orient_mode_mp{};

	bool operator==(const ai_asm_state&) const = default;
};

struct asm_state_m_ai_state
{
	std::optional<ai_asm_state> ai_state{};

	bool operator==(const asm_state_m_ai_state&) const = default;
};

struct asm_state
{
	std::optional<std::vector<asm_transition>> transitions{};
	std::optional<std::string> name{};
	std::int32_t id{};
	std::int32_t type{};
	std::int32_t stance{};
	std::int32_t flags{};
	std::int32_t logic_group_id{};
	std::optional<std::vector<std::optional<std::string>>> subtrees{};
	asm_function play_anim{};
	asm_function play_anim_note_handler{};
	asm_function choose_anim_func{};
	asm_function terminate_func{};
	std::optional<std::string> exit_notify{};
	std::optional<std::string> facial_state{};
	std::int32_t death_state{};
	std::optional<std::vector<asm_function_param>> death_params{};
	std::int32_t pain_state{};
	std::optional<std::vector<asm_function_param>> pain_params{};
	std::int32_t execution_state{};
	std::optional<std::vector<asm_function_param>> execution_params{};
	std::variant<std::monostate, asm_state_m_ai_state> u{};

	bool operator==(const asm_state&) const = default;
};

struct asm_transition
{
	std::int32_t to_state_id{};
	asm_function func_name{};
	std::int32_t priority{};

	bool operator==(const asm_transition&) const = default;
};

struct asm_logic_group
{
	std::int32_t id{};
	std::int32_t parent_id{};
	asm_function init_func{};
	asm_function terminate_func{};

	bool operator==(const asm_logic_group&) const = default;
};

struct ai_animset_node_offset
{
	std::int32_t hide_yaw{};
	std::int32_t snap_yaw{};
	std::int32_t lean_yaw{};
	std::int32_t lean_pitch{};

	bool operator==(const ai_animset_node_offset&) const = default;
};

struct ai_animset
{
	ai_animset_node_offset cover_left_offset{};
	ai_animset_node_offset cover_left_crouch_offset{};
	ai_animset_node_offset cover_right_offset{};
	ai_animset_node_offset cover_right_crouch_offset{};
	ai_animset_node_offset cover_crouch_offset{};
	ai_animset_node_offset cover_stand_offset{};

	bool operator==(const ai_animset&) const = default;
};

struct animset_m_ai_animset
{
	std::optional<ai_animset> ai_animset{};

	bool operator==(const animset_m_ai_animset&) const = default;
};

struct aianimset
{
	std::optional<std::string> name{};
	std::optional<std::string> const_name{};
	std::optional<std::string> anim_tree{};
	std::optional<std::vector<animset_state>> states{};
	std::string anim_ctrl{};
	std::optional<std::string> start_state_name{};
	std::optional<std::string> start_alias_name{};
	std::optional<std::string> clear_state_name{};
	std::optional<std::string> clear_alias_name{};
	std::optional<std::string> always_on_state_name{};
	std::variant<std::monostate, animset_m_ai_animset> u{};

	bool operator==(const aianimset&) const = default;
};

struct ai_animset_state
{

	bool operator==(const ai_animset_state&) const = default;
};

struct animset_state_m_ai_animset_state
{
	std::optional<ai_animset_state> ai_animset_state{};

	bool operator==(const animset_state_m_ai_animset_state&) const = default;
};

struct animset_state
{
	std::optional<std::string> name{};
	std::optional<std::vector<animset_alias>> anim_aliases{};
	std::optional<std::vector<animset_blendtree_alias>> blendtree_aliases{};
	std::optional<std::string> notify{};
	std::optional<std::string> aimset_target{};
	std::optional<std::string> aimset_root{};
	std::optional<std::vector<animset_state_entry_map>> anim_entry_map{};
	std::string state_type{};
	float look_at_min_yaw{};
	float look_at_max_yaw{};
	float look_at_min_pitch{};
	float look_at_max_pitch{};
	float look_at_neutral_pitch{};
	float look_at_neutral_yaw{};
	bool restart{};
	bool is_aimset{};
	bool clear_always_on{};
	std::variant<std::monostate, animset_state_m_ai_animset_state> u{};

	bool operator==(const animset_state&) const = default;
};

struct ai_animset_alias
{
	std::optional<std::vector<animset_anim>> red_anims{};

	bool operator==(const ai_animset_alias&) const = default;
};

struct animset_alias_m_ai_animset_alias
{
	std::optional<ai_animset_alias> ai_animset_alias{};

	bool operator==(const animset_alias_m_ai_animset_alias&) const = default;
};

struct animset_alias
{
	std::optional<std::string> name{};
	std::optional<std::vector<animset_anim>> anims{};
	std::optional<std::vector<asm_function>> funcs{};
	std::optional<std::vector<std::optional<std::string>>> addons{};
	std::optional<std::string> anim_alias{};
	std::int32_t blender_index{};
	float blendtime{};
	std::optional<std::string> curve_name{};
	std::variant<std::monostate, animset_alias_m_ai_animset_alias> u{};

	bool operator==(const animset_alias&) const = default;
};

struct animset_anim
{
	std::optional<std::string> name{};
	float weight{};
	std::optional<std::string> aimset_name{};

	bool operator==(const animset_anim&) const = default;
};

struct animset_blendtree_alias
{
	std::optional<std::string> name{};
	std::optional<std::string> graft_node{};
	std::optional<std::string> blendtree_name{};
	std::optional<std::vector<asm_function>> funcs{};
	bool b_notify{};

	bool operator==(const animset_blendtree_alias&) const = default;
};

struct animset_state_entry_map
{
	std::uint16_t alias_index{};
	std::uint16_t alias_anim_entry_index{};

	bool operator==(const animset_state_entry_map&) const = default;
};

inline void validate(const ai_asm& data)
{
	if ((data.services && data.services->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("ai_asm has arrays that its count does not fit");
	}

}

inline void validate(const asm_m_aiasm& data)
{
	if (data.aiasm)
	{
		validate(*data.aiasm);
	}

}

inline void validate(const aiasm& data)
{
	if (!data.sz_name)
	{
		throw std::runtime_error("aiasm has no name");
	}

	detail::name(*data.sz_name, "aiasm");

	if (data.states)
	{
		for (const auto& item : *data.states)
		{
			validate(item);
		}
	}

	if (data.logic_groups)
	{
		for (const auto& item : *data.logic_groups)
		{
			validate(item);
		}
	}

	std::visit([](const auto& value)
	{
		if constexpr (std::is_same_v<std::decay_t<decltype(value)>, std::monostate>)
		{
			throw std::runtime_error("aiasm.u holds no module");
		}
		else
		{
			validate(value);
		}
	}, data.u);

	if ((data.states && data.states->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("aiasm has arrays that its count does not fit");
	}

	if ((data.logic_groups && data.logic_groups->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("aiasm has arrays that its count does not fit");
	}

	if ((data.func_i_ds && data.func_i_ds->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("aiasm has arrays that its count does not fit");
	}

}

inline void validate(const asm_function& data)
{
	if (data.params)
	{
		for (const auto& item : *data.params)
		{
			validate(item);
		}
	}

	if ((data.params && data.params->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("asm_function has arrays that its count does not fit");
	}

}

inline void validate(const ai_asm_state& data)
{
	if (data.power_down_params)
	{
		for (const auto& item : *data.power_down_params)
		{
			validate(item);
		}
	}

	if ((data.power_down_params && data.power_down_params->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("ai_asm_state has arrays that its count does not fit");
	}

}

inline void validate(const asm_state_m_ai_state& data)
{
	if (data.ai_state)
	{
		validate(*data.ai_state);
	}

}

inline void validate(const asm_state& data)
{
	if (data.transitions)
	{
		for (const auto& item : *data.transitions)
		{
			validate(item);
		}
	}

	validate(data.play_anim);

	validate(data.play_anim_note_handler);

	validate(data.choose_anim_func);

	validate(data.terminate_func);

	if (data.death_params)
	{
		for (const auto& item : *data.death_params)
		{
			validate(item);
		}
	}

	if (data.pain_params)
	{
		for (const auto& item : *data.pain_params)
		{
			validate(item);
		}
	}

	if (data.execution_params)
	{
		for (const auto& item : *data.execution_params)
		{
			validate(item);
		}
	}

	std::visit([](const auto& value)
	{
		if constexpr (std::is_same_v<std::decay_t<decltype(value)>, std::monostate>)
		{
			throw std::runtime_error("asm_state.u holds no module");
		}
		else
		{
			validate(value);
		}
	}, data.u);

	if ((data.transitions && data.transitions->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("asm_state has arrays that its count does not fit");
	}

	if ((data.subtrees && data.subtrees->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("asm_state has arrays that its count does not fit");
	}

	if ((data.death_params && data.death_params->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("asm_state has arrays that its count does not fit");
	}

	if ((data.pain_params && data.pain_params->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("asm_state has arrays that its count does not fit");
	}

	if ((data.execution_params && data.execution_params->size() > 0x7FFFFFFFull))
	{
		throw std::runtime_error("asm_state has arrays that its count does not fit");
	}

}

inline void validate(const asm_transition& data)
{
	validate(data.func_name);

}

inline void validate(const asm_logic_group& data)
{
	validate(data.init_func);

	validate(data.terminate_func);

}

inline void validate(const ai_animset_node_offset& data)
{
	(void) data;
}

inline void validate(const ai_animset& data)
{
	validate(data.cover_left_offset);

	validate(data.cover_left_crouch_offset);

	validate(data.cover_right_offset);

	validate(data.cover_right_crouch_offset);

	validate(data.cover_crouch_offset);

	validate(data.cover_stand_offset);

}

inline void validate(const animset_m_ai_animset& data)
{
	if (data.ai_animset)
	{
		validate(*data.ai_animset);
	}

}

inline void validate(const aianimset& data)
{
	if (!data.name)
	{
		throw std::runtime_error("aianimset has no name");
	}

	detail::name(*data.name, "aianimset");

	if (data.states)
	{
		for (const auto& item : *data.states)
		{
			validate(item);
		}
	}

	if (std::ranges::none_of(animation_controller_names, [&](const auto& entry) { return entry.second == data.anim_ctrl; }))
	{
		throw std::runtime_error("aianimset.anim_ctrl names no value");
	}

	std::visit([](const auto& value)
	{
		if constexpr (std::is_same_v<std::decay_t<decltype(value)>, std::monostate>)
		{
			throw std::runtime_error("aianimset.u holds no module");
		}
		else
		{
			validate(value);
		}
	}, data.u);

	if ((data.states && data.states->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("aianimset has arrays that its count does not fit");
	}

}

inline void validate(const ai_animset_state& data)
{
	(void) data;
}

inline void validate(const animset_state_m_ai_animset_state& data)
{
	if (data.ai_animset_state)
	{
		validate(*data.ai_animset_state);
	}

}

inline void validate(const animset_state& data)
{
	if (data.anim_aliases)
	{
		for (const auto& item : *data.anim_aliases)
		{
			validate(item);
		}
	}

	if (data.blendtree_aliases)
	{
		for (const auto& item : *data.blendtree_aliases)
		{
			validate(item);
		}
	}

	if (data.anim_entry_map)
	{
		for (const auto& item : *data.anim_entry_map)
		{
			validate(item);
		}
	}

	if (std::ranges::none_of(animset_state_type_names, [&](const auto& entry) { return entry.second == data.state_type; }))
	{
		throw std::runtime_error("animset_state.state_type names no value");
	}

	std::visit([](const auto& value)
	{
		if constexpr (std::is_same_v<std::decay_t<decltype(value)>, std::monostate>)
		{
			throw std::runtime_error("animset_state.u holds no module");
		}
		else
		{
			validate(value);
		}
	}, data.u);

	if ((data.anim_aliases && data.anim_aliases->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("animset_state has arrays that its count does not fit");
	}

	if ((data.blendtree_aliases && data.blendtree_aliases->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("animset_state has arrays that its count does not fit");
	}

	if ((data.anim_entry_map && data.anim_entry_map->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("animset_state has arrays that its count does not fit");
	}

}

inline void validate(const ai_animset_alias& data)
{
	if (data.red_anims)
	{
		for (const auto& item : *data.red_anims)
		{
			validate(item);
		}
	}

	if ((data.red_anims && data.red_anims->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("ai_animset_alias has arrays that its count does not fit");
	}

}

inline void validate(const animset_alias_m_ai_animset_alias& data)
{
	if (data.ai_animset_alias)
	{
		validate(*data.ai_animset_alias);
	}

}

inline void validate(const animset_alias& data)
{
	if (data.anims)
	{
		for (const auto& item : *data.anims)
		{
			validate(item);
		}
	}

	if (data.funcs)
	{
		for (const auto& item : *data.funcs)
		{
			validate(item);
		}
	}

	std::visit([](const auto& value)
	{
		if constexpr (std::is_same_v<std::decay_t<decltype(value)>, std::monostate>)
		{
			throw std::runtime_error("animset_alias.u holds no module");
		}
		else
		{
			validate(value);
		}
	}, data.u);

	if ((data.anims && data.anims->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("animset_alias has arrays that its count does not fit");
	}

	if ((data.funcs && data.funcs->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("animset_alias has arrays that its count does not fit");
	}

	if ((data.addons && data.addons->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("animset_alias has arrays that its count does not fit");
	}

}

inline void validate(const animset_anim& data)
{
	(void) data;
}

inline void validate(const animset_blendtree_alias& data)
{
	if (data.funcs)
	{
		for (const auto& item : *data.funcs)
		{
			validate(item);
		}
	}

	if ((data.funcs && data.funcs->size() > 0xFFFFFFFFull))
	{
		throw std::runtime_error("animset_blendtree_alias has arrays that its count does not fit");
	}

}

inline void validate(const animset_state_entry_map& data)
{
	(void) data;
}

} // namespace aiasm
} // namespace zonetool::asset_data
