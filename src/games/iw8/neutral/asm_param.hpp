#pragma once

#include <core/assets/asm_param_data.hpp>
#include "resolver.hpp"
#include "plain.hpp"
#include "../structs.hpp"

// The parameter of a function call in a state machine or an animation set, which the generated adapter (tools/gen_iw8_neutral.py,
// spec aiasm.json) hands to code written by hand: its type says which member of its union holds the value.
namespace zonetool::iw8::neutral
{
	asset_data::asm_function_param read_asm_function_param(const ASM_Function_Param& native);
	void write_asm_function_param(ASM_Function_Param& native, const asset_data::asm_function_param& data, const asset_resolver& resolve,
		plain::context& memory);
	std::size_t size_asm_function_param(const asset_data::asm_function_param& data);
}
