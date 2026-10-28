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

// Neutral behavior assets (docs/NEUTRAL_ASSETS.md), emitted by tools/gen_iw8_neutral.py from the IW8 structs and stream
// traversals: do not edit; change the spec (tools/neutral_families/behavior.json) or the generator.
namespace zonetool::asset_data
{
namespace behavior
{
inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 6> behavior_tree_result_names{{
	{0, "failure"},
	{1, "success"},
	{2, "running"},
	{3, "invalid"},
	{4, "no_override"},
	{5, "count"},
}};

struct behaviortree;
struct behavior_tree_node;
struct selector_children;
struct sequence_children;
struct active_selector_children;
struct active_sequence_children;
struct parallel_children;
struct bsm_children;
struct behavior_tree_action_data;
struct negate_children;
struct prefab_children;
struct behavior_tree_decorator_children;
struct behavior_tree_decorator;
struct behavior_tree_decorator_terminate;
struct behavior_tree_decorator_init;
struct behavior_tree_decorator_root_init;

inline void validate(const behaviortree& data);
inline void validate(const behavior_tree_node& data);
inline void validate(const selector_children& data);
inline void validate(const sequence_children& data);
inline void validate(const active_selector_children& data);
inline void validate(const active_sequence_children& data);
inline void validate(const parallel_children& data);
inline void validate(const bsm_children& data);
inline void validate(const behavior_tree_action_data& data);
inline void validate(const negate_children& data);
inline void validate(const prefab_children& data);
inline void validate(const behavior_tree_decorator_children& data);
inline void validate(const behavior_tree_decorator& data);
inline void validate(const behavior_tree_decorator_terminate& data);
inline void validate(const behavior_tree_decorator_init& data);
inline void validate(const behavior_tree_decorator_root_init& data);

struct behaviortree
{
	std::optional<std::string> name{};
	std::optional<std::vector<behavior_tree_node>> nodes{};
	std::optional<std::vector<std::int32_t>> func_i_ds{};
	std::optional<std::vector<behavior_tree_decorator>> decorators{};

	bool operator==(const behaviortree&) const = default;
};

struct selector_children
{
	std::uint16_t first_child_offset{};
	std::uint16_t child_count{};

	bool operator==(const selector_children&) const = default;
};

struct sequence_children
{
	std::uint16_t first_child_offset{};
	std::uint16_t child_count{};

	bool operator==(const sequence_children&) const = default;
};

struct active_selector_children
{
	std::uint16_t first_child_offset{};
	std::uint16_t child_count{};

	bool operator==(const active_selector_children&) const = default;
};

struct active_sequence_children
{
	std::uint16_t first_child_offset{};
	std::uint16_t child_count{};

	bool operator==(const active_sequence_children&) const = default;
};

struct parallel_children
{
	std::uint16_t first_child_offset{};
	std::uint16_t child_count{};

	bool operator==(const parallel_children&) const = default;
};

struct bsm_children
{
	std::uint16_t first_child_offset{};
	std::uint16_t child_count{};

	bool operator==(const bsm_children&) const = default;
};

struct behavior_tree_action_data
{
	std::uint16_t init_fn{};
	std::uint16_t process_fn{};
	std::uint16_t terminate_fn{};
	std::uint16_t process_args_fn{};
	bool b_process_builtin{};
	bool b_init_builtin{};
	bool b_terminate_builtin{};

	bool operator==(const behavior_tree_action_data&) const = default;
};

struct negate_children
{
	std::uint16_t first_child_offset{};
	std::uint16_t child_count{};

	bool operator==(const negate_children&) const = default;
};

struct prefab_children
{
	std::uint16_t first_child_offset{};
	std::uint16_t child_count{};

	bool operator==(const prefab_children&) const = default;
};

struct behavior_tree_decorator_children
{
	std::uint16_t first_decorator{};
	std::uint16_t decorator_count{};

	bool operator==(const behavior_tree_decorator_children&) const = default;
};

struct behavior_tree_node
{
	std::optional<std::string> name{};
	std::string result_override{};
	std::int32_t schedule{};
	std::variant<std::monostate, selector_children, sequence_children, active_selector_children, active_sequence_children, parallel_children, bsm_children, behavior_tree_action_data, negate_children, prefab_children> data{};
	behavior_tree_decorator_children decorators{};

	bool operator==(const behavior_tree_node&) const = default;
};

struct behavior_tree_decorator_terminate
{
	std::uint16_t terminate_fn{};
	bool b_terminate_builtin{};

	bool operator==(const behavior_tree_decorator_terminate&) const = default;
};

struct behavior_tree_decorator_init
{
	std::uint16_t init_fn{};
	bool b_init_builtin{};

	bool operator==(const behavior_tree_decorator_init&) const = default;
};

struct behavior_tree_decorator_root_init
{
	std::uint16_t init_fn{};
	bool b_init_builtin{};

	bool operator==(const behavior_tree_decorator_root_init&) const = default;
};

struct behavior_tree_decorator
{
	std::variant<std::monostate, behavior_tree_decorator_terminate, behavior_tree_decorator_init, behavior_tree_decorator_root_init> data{};

	bool operator==(const behavior_tree_decorator&) const = default;
};

inline void validate(const behaviortree& data)
{
	if (!data.name)
	{
		throw std::runtime_error("behaviortree has no name");
	}

	detail::name(*data.name, "behaviortree");

	if (data.nodes)
	{
		for (const auto& item : *data.nodes)
		{
			validate(item);
		}
	}

	if (data.decorators)
	{
		for (const auto& item : *data.decorators)
		{
			validate(item);
		}
	}

	if ((data.nodes && data.nodes->size() > 0xFFFFull))
	{
		throw std::runtime_error("behaviortree has arrays that its count does not fit");
	}

	if ((data.func_i_ds && data.func_i_ds->size() > 0xFFFFull))
	{
		throw std::runtime_error("behaviortree has arrays that its count does not fit");
	}

	if ((data.decorators && data.decorators->size() > 0xFFFFull))
	{
		throw std::runtime_error("behaviortree has arrays that its count does not fit");
	}

}

inline void validate(const selector_children& data)
{
	(void) data;
}

inline void validate(const sequence_children& data)
{
	(void) data;
}

inline void validate(const active_selector_children& data)
{
	(void) data;
}

inline void validate(const active_sequence_children& data)
{
	(void) data;
}

inline void validate(const parallel_children& data)
{
	(void) data;
}

inline void validate(const bsm_children& data)
{
	(void) data;
}

inline void validate(const behavior_tree_action_data& data)
{
	(void) data;
}

inline void validate(const negate_children& data)
{
	(void) data;
}

inline void validate(const prefab_children& data)
{
	(void) data;
}

inline void validate(const behavior_tree_decorator_children& data)
{
	(void) data;
}

inline void validate(const behavior_tree_node& data)
{
	if (std::ranges::none_of(behavior_tree_result_names, [&](const auto& entry) { return entry.second == data.result_override; }))
	{
		throw std::runtime_error("behavior_tree_node.result_override names no value");
	}

	std::visit([](const auto& value)
	{
		if constexpr (std::is_same_v<std::decay_t<decltype(value)>, std::monostate>)
		{
			throw std::runtime_error("behavior_tree_node.data holds no module");
		}
		else
		{
			validate(value);
		}
	}, data.data);

	validate(data.decorators);

}

inline void validate(const behavior_tree_decorator_terminate& data)
{
	(void) data;
}

inline void validate(const behavior_tree_decorator_init& data)
{
	(void) data;
}

inline void validate(const behavior_tree_decorator_root_init& data)
{
	(void) data;
}

inline void validate(const behavior_tree_decorator& data)
{
	std::visit([](const auto& value)
	{
		if constexpr (std::is_same_v<std::decay_t<decltype(value)>, std::monostate>)
		{
			throw std::runtime_error("behavior_tree_decorator.data holds no module");
		}
		else
		{
			validate(value);
		}
	}, data.data);

}

} // namespace behavior
} // namespace zonetool::asset_data
