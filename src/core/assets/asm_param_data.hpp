#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <variant>

namespace zonetool::asset_data
{
	// A parameter of a function call in a state machine or an animation set. The alternatives follow the order of the parameter
	// types (undefined, bool, string, int, float); a string is the name of a script string.
	struct asm_function_param
	{
		std::variant<std::monostate, bool, std::optional<std::string>, std::int32_t, float> value{};

		bool operator==(const asm_function_param&) const = default;
	};

	inline void validate([[maybe_unused]] const asm_function_param& data)
	{
	}
}
