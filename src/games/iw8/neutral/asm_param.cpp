#include <std_include.hpp>
#include "asm_param.hpp"

namespace zonetool::iw8::neutral
{
	namespace
	{
		// The bytes of the union after the member that the type uses are padding.
		void require_unused_zero(const ASM_Function_Param& native, const std::size_t used)
		{
			const auto* bytes = reinterpret_cast<const std::uint8_t*>(&native.u);
			for (std::size_t index = used; index < sizeof(native.u); ++index)
			{
				if (bytes[index])
				{
					plain::fail("ASM_Function_Param.u", "holds data after the member its type uses");
				}
			}
		}
	}

	asset_data::asm_function_param read_asm_function_param(const ASM_Function_Param& native)
	{
		asset_data::asm_function_param data;
		switch (native.m_Type)
		{
		case ParamType_Undefined:
			require_unused_zero(native, 0);
			break;
		case ParamType_Bool:
		{
			const auto flag = *reinterpret_cast<const std::uint8_t*>(&native.u);
			if (flag > 1)
			{
				plain::fail("ASM_Function_Param.u", "holds a bool that is neither false nor true");
			}

			require_unused_zero(native, sizeof(bool));
			data.value = flag != 0;
			break;
		}
		case ParamType_String:
			data.value = plain::read_script_string(native.u.m_String);
			break;
		case ParamType_Int:
			data.value = native.u.m_Int;
			break;
		case ParamType_Float:
			data.value = native.u.m_Float;
			break;
		default:
			plain::fail("ASM_Function_Param.m_Type", "is not a parameter type");
		}

		return data;
	}

	void write_asm_function_param(ASM_Function_Param& native, const asset_data::asm_function_param& data, [[maybe_unused]] const asset_resolver& resolve,
		[[maybe_unused]] plain::context& memory)
	{
		std::memset(&native.u, 0, sizeof(native.u));
		native.m_Type = static_cast<ASM_Function_Param_Type>(data.value.index());
		std::visit([&](const auto& value)
		{
			using value_type = std::decay_t<decltype(value)>;
			if constexpr (std::is_same_v<value_type, bool>)
			{
				native.u.m_Bool = value;
			}
			else if constexpr (std::is_same_v<value_type, std::optional<std::string>>)
			{
				native.u.m_String = static_cast<scr_string_t>(plain::write_script_string(value));
			}
			else if constexpr (std::is_same_v<value_type, std::int32_t>)
			{
				native.u.m_Int = value;
			}
			else if constexpr (std::is_same_v<value_type, float>)
			{
				native.u.m_Float = value;
			}
		}, data.value);
	}

	std::size_t size_asm_function_param([[maybe_unused]] const asset_data::asm_function_param& data)
	{
		return 0;
	}
}
