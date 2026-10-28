// Emitted by tools/gen_iw8_neutral.py from tools/neutral_families/aiasm.json: do not edit.
#include <std_include.hpp>
#include "aiasm.hpp"
#include "plain.hpp"
#include "asm_param.hpp"

namespace zonetool::iw8::neutral
{
	namespace
	{
		asset_data::aiasm::aiasm read_aiasm(const ASM& native);
		void write_aiasm(ASM& native, const asset_data::aiasm::aiasm& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_aiasm(const asset_data::aiasm::aiasm& data);
		asset_data::aiasm::asm_state read_asm_state(const ASM_State& native, const ASM& outer);
		void write_asm_state(ASM_State& native, const asset_data::aiasm::asm_state& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_asm_state(const asset_data::aiasm::asm_state& data);
		asset_data::aiasm::asm_transition read_asm_transition(const ASM_Transition& native);
		void write_asm_transition(ASM_Transition& native, const asset_data::aiasm::asm_transition& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_asm_transition(const asset_data::aiasm::asm_transition& data);
		asset_data::aiasm::asm_function read_asm_function(const ASM_Function& native);
		void write_asm_function(ASM_Function& native, const asset_data::aiasm::asm_function& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_asm_function(const asset_data::aiasm::asm_function& data);
		asset_data::aiasm::asm_state_m_ai_state read_asm_state_m_ai_state(const ASM_State_Union& native);
		void write_asm_state_m_ai_state(ASM_State_Union& native, const asset_data::aiasm::asm_state_m_ai_state& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_asm_state_m_ai_state(const asset_data::aiasm::asm_state_m_ai_state& data);
		asset_data::aiasm::ai_asm_state read_ai_asm_state(const AI_ASM_State& native);
		void write_ai_asm_state(AI_ASM_State& native, const asset_data::aiasm::ai_asm_state& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_ai_asm_state(const asset_data::aiasm::ai_asm_state& data);
		asset_data::aiasm::asm_logic_group read_asm_logic_group(const ASM_LogicGroup& native);
		void write_asm_logic_group(ASM_LogicGroup& native, const asset_data::aiasm::asm_logic_group& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_asm_logic_group(const asset_data::aiasm::asm_logic_group& data);
		asset_data::aiasm::asm_m_aiasm read_asm_m_aiasm(const ASM_Union& native);
		void write_asm_m_aiasm(ASM_Union& native, const asset_data::aiasm::asm_m_aiasm& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_asm_m_aiasm(const asset_data::aiasm::asm_m_aiasm& data);
		asset_data::aiasm::ai_asm read_ai_asm(const AI_ASM& native);
		void write_ai_asm(AI_ASM& native, const asset_data::aiasm::ai_asm& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_ai_asm(const asset_data::aiasm::ai_asm& data);
		asset_data::aiasm::aianimset read_aianimset(const Animset& native);
		void write_aianimset(Animset& native, const asset_data::aiasm::aianimset& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_aianimset(const asset_data::aiasm::aianimset& data);
		asset_data::aiasm::animset_state read_animset_state(const AnimsetState& native, const Animset& outer);
		void write_animset_state(AnimsetState& native, const asset_data::aiasm::animset_state& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_animset_state(const asset_data::aiasm::animset_state& data);
		asset_data::aiasm::animset_alias read_animset_alias(const AnimsetAlias& native, const Animset& outer);
		void write_animset_alias(AnimsetAlias& native, const asset_data::aiasm::animset_alias& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_animset_alias(const asset_data::aiasm::animset_alias& data);
		asset_data::aiasm::animset_anim read_animset_anim(const AnimsetAnim& native);
		void write_animset_anim(AnimsetAnim& native, const asset_data::aiasm::animset_anim& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_animset_anim(const asset_data::aiasm::animset_anim& data);
		asset_data::aiasm::animset_alias_m_ai_animset_alias read_animset_alias_m_ai_animset_alias(const AnimsetAlias_Union& native);
		void write_animset_alias_m_ai_animset_alias(AnimsetAlias_Union& native, const asset_data::aiasm::animset_alias_m_ai_animset_alias& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_animset_alias_m_ai_animset_alias(const asset_data::aiasm::animset_alias_m_ai_animset_alias& data);
		asset_data::aiasm::ai_animset_alias read_ai_animset_alias(const AIAnimsetAlias& native);
		void write_ai_animset_alias(AIAnimsetAlias& native, const asset_data::aiasm::ai_animset_alias& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_ai_animset_alias(const asset_data::aiasm::ai_animset_alias& data);
		asset_data::aiasm::animset_blendtree_alias read_animset_blendtree_alias(const AnimsetBlendtreeAlias& native);
		void write_animset_blendtree_alias(AnimsetBlendtreeAlias& native, const asset_data::aiasm::animset_blendtree_alias& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_animset_blendtree_alias(const asset_data::aiasm::animset_blendtree_alias& data);
		asset_data::aiasm::animset_state_entry_map read_animset_state_entry_map(const AnimsetStateEntryMap& native);
		void write_animset_state_entry_map(AnimsetStateEntryMap& native, const asset_data::aiasm::animset_state_entry_map& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_animset_state_entry_map(const asset_data::aiasm::animset_state_entry_map& data);
		asset_data::aiasm::animset_state_m_ai_animset_state read_animset_state_m_ai_animset_state(const AnimsetState_Union& native);
		void write_animset_state_m_ai_animset_state(AnimsetState_Union& native, const asset_data::aiasm::animset_state_m_ai_animset_state& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_animset_state_m_ai_animset_state(const asset_data::aiasm::animset_state_m_ai_animset_state& data);
		asset_data::aiasm::ai_animset_state read_ai_animset_state(const AIAnimsetState& native);
		void write_ai_animset_state(AIAnimsetState& native, const asset_data::aiasm::ai_animset_state& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_ai_animset_state(const asset_data::aiasm::ai_animset_state& data);
		asset_data::aiasm::animset_m_ai_animset read_animset_m_ai_animset(const Animset_Union& native);
		void write_animset_m_ai_animset(Animset_Union& native, const asset_data::aiasm::animset_m_ai_animset& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_animset_m_ai_animset(const asset_data::aiasm::animset_m_ai_animset& data);
		asset_data::aiasm::ai_animset read_ai_animset(const AIAnimset& native);
		void write_ai_animset(AIAnimset& native, const asset_data::aiasm::ai_animset& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_ai_animset(const asset_data::aiasm::ai_animset& data);
		asset_data::aiasm::ai_animset_node_offset read_ai_animset_node_offset(const AIAnimsetNodeOffset& native);
		void write_ai_animset_node_offset(AIAnimsetNodeOffset& native, const asset_data::aiasm::ai_animset_node_offset& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_ai_animset_node_offset(const asset_data::aiasm::ai_animset_node_offset& data);

		asset_data::aiasm::ai_asm read_ai_asm(const AI_ASM& native)
		{
			plain::trace("read_ai_asm");
			asset_data::aiasm::ai_asm data;
			data.services = plain::read_array(native.m_Services, native.m_NumServices, "AI_ASM.m_Services", [&](const auto& item) { return item; });
			data.script_handler = native.m_ScriptHandler;
			return data;
		}

		void write_ai_asm(AI_ASM& native, const asset_data::aiasm::ai_asm& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.m_Services = plain::write_array<std::int32_t>(data.services, memory, [&](auto& out, const auto& in) { out = in; });
			native.m_ScriptHandler = data.script_handler;
			native.m_NumServices = plain::count_of<std::int32_t>(data.services);
		}

		std::size_t size_ai_asm([[maybe_unused]] const asset_data::aiasm::ai_asm& data)
		{
			std::size_t size = 0;
			if (data.services)
			{
				size += plain::sized(std::max<std::size_t>(data.services->size(), 1) * sizeof(std::int32_t));
			}
			return size;
		}

		asset_data::aiasm::asm_m_aiasm read_asm_m_aiasm(const ASM_Union& native)
		{
			plain::trace("read_asm_m_aiasm");
			asset_data::aiasm::asm_m_aiasm data;
			data.aiasm = plain::read_single(native.m_AIASM, [&](const auto& item) { return read_ai_asm(item); });
			return data;
		}

		void write_asm_m_aiasm(ASM_Union& native, const asset_data::aiasm::asm_m_aiasm& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.m_AIASM = plain::write_single<AI_ASM>(data.aiasm, memory, [&](auto& out, const auto& in) { write_ai_asm(out, in, resolve, memory); });
		}

		std::size_t size_asm_m_aiasm([[maybe_unused]] const asset_data::aiasm::asm_m_aiasm& data)
		{
			std::size_t size = 0;
			if (data.aiasm)
			{
				size += plain::sized(sizeof(AI_ASM));
				size += size_ai_asm(*data.aiasm);
			}
			return size;
		}

		asset_data::aiasm::aiasm read_aiasm(const ASM& native)
		{
			plain::trace("read_aiasm");
			asset_data::aiasm::aiasm data;
			data.sz_name = plain::read_string(native.m_szName);
			data.name = plain::read_script_string(native.m_Name);
			data.states = plain::read_array(native.m_States, native.m_NumStates, "ASM.m_States", [&](const auto& item) { return read_asm_state(item, native); });
			data.logic_groups = plain::read_array(native.m_LogicGroups, native.m_NumLogicGroups, "ASM.m_LogicGroups", [&](const auto& item) { return read_asm_logic_group(item); });
			data.func_i_ds = plain::read_array(native.m_FuncIDs, native.m_NumFuncIDs, "ASM.m_FuncIDs", [&](const auto& item) { return item; });
			data.start_state = native.m_StartState;
			data.u = [&]() -> std::variant<std::monostate, asset_data::aiasm::asm_m_aiasm> { switch (static_cast<std::uint32_t>(native.m_Mode)) { case 0: { plain::module_scope scope(0); auto item = read_asm_m_aiasm(native.u); return std::variant<std::monostate, asset_data::aiasm::asm_m_aiasm>(std::move(item)); } default: plain::fail("ASM.u", "has a tag value without a case"); } }();
			return data;
		}

		void write_aiasm(ASM& native, const asset_data::aiasm::aiasm& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.m_szName = plain::write_string(memory, data.sz_name);
			native.m_Name = static_cast<scr_string_t>(plain::write_script_string(data.name));
			native.m_States = plain::write_array<ASM_State>(data.states, memory, [&](auto& out, const auto& in) { write_asm_state(out, in, resolve, memory); });
			native.m_LogicGroups = plain::write_array<ASM_LogicGroup>(data.logic_groups, memory, [&](auto& out, const auto& in) { write_asm_logic_group(out, in, resolve, memory); });
			native.m_FuncIDs = plain::write_array<std::int32_t>(data.func_i_ds, memory, [&](auto& out, const auto& in) { out = in; });
			native.m_StartState = data.start_state;
			std::visit([&](const auto& value)
			{
				using value_type = std::decay_t<decltype(value)>;
				if constexpr (std::is_same_v<value_type, std::monostate>)
				{
					plain::fail("ASM.u", "holds no module");
				}
				else if constexpr (std::is_same_v<value_type, asset_data::aiasm::asm_m_aiasm>)
				{
					native.m_Mode = static_cast<ASM_Mode>(0);
					plain::module_scope scope(0);
					write_asm_m_aiasm(native.u, value, resolve, memory);
				}
			}, data.u);
			native.m_NumStates = plain::count_of<std::int32_t>(data.states);
			native.m_NumLogicGroups = plain::count_of<std::int32_t>(data.logic_groups);
			native.m_NumFuncIDs = plain::count_of<std::int32_t>(data.func_i_ds);
		}

		std::size_t size_aiasm([[maybe_unused]] const asset_data::aiasm::aiasm& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.sz_name);
			if (data.states)
			{
				size += plain::sized(std::max<std::size_t>(data.states->size(), 1) * sizeof(ASM_State));
				for (const auto& item : *data.states)
				{
					size += size_asm_state(item);
				}
			}
			if (data.logic_groups)
			{
				size += plain::sized(std::max<std::size_t>(data.logic_groups->size(), 1) * sizeof(ASM_LogicGroup));
				for (const auto& item : *data.logic_groups)
				{
					size += size_asm_logic_group(item);
				}
			}
			if (data.func_i_ds)
			{
				size += plain::sized(std::max<std::size_t>(data.func_i_ds->size(), 1) * sizeof(std::int32_t));
			}
			std::visit([&](const auto& value)
			{
				using value_type = std::decay_t<decltype(value)>;
				if constexpr (std::is_same_v<value_type, std::monostate>)
				{
				}
				else if constexpr (std::is_same_v<value_type, asset_data::aiasm::asm_m_aiasm>)
				{
					size += size_asm_m_aiasm(value);
				}
			}, data.u);
			return size;
		}

		asset_data::aiasm::asm_function read_asm_function(const ASM_Function& native)
		{
			plain::trace("read_asm_function");
			asset_data::aiasm::asm_function data;
			data.func_id = native.m_FuncID;
			data.params = plain::read_array(native.m_Params, native.m_NumParams, "ASM_Function.m_Params", [&](const auto& item) { return read_asm_function_param(item); });
			data.b_builtin = native.m_bBuiltin;
			data.b_negate = native.m_bNegate;
			data.flags = native.m_Flags;
			data.hash = native.m_Hash;
			plain::require_zero(native.padding, "ASM_Function.padding");
			return data;
		}

		void write_asm_function(ASM_Function& native, const asset_data::aiasm::asm_function& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.m_FuncID = data.func_id;
			native.m_Params = plain::write_array<ASM_Function_Param>(data.params, memory, [&](auto& out, const auto& in) { write_asm_function_param(out, in, resolve, memory); });
			native.m_bBuiltin = data.b_builtin;
			native.m_bNegate = data.b_negate;
			native.m_Flags = data.flags;
			native.m_Hash = data.hash;
			native.m_NumParams = plain::count_of<std::int32_t>(data.params);
		}

		std::size_t size_asm_function([[maybe_unused]] const asset_data::aiasm::asm_function& data)
		{
			std::size_t size = 0;
			if (data.params)
			{
				size += plain::sized(std::max<std::size_t>(data.params->size(), 1) * sizeof(ASM_Function_Param));
				for (const auto& item : *data.params)
				{
					size += size_asm_function_param(item);
				}
			}
			return size;
		}

		asset_data::aiasm::ai_asm_state read_ai_asm_state(const AI_ASM_State& native)
		{
			plain::trace("read_ai_asm_state");
			asset_data::aiasm::ai_asm_state data;
			data.power_down_state = native.m_PowerDownState;
			data.power_down_params = plain::read_array(native.m_PowerDownParams, native.m_PowerDownNumParams, "AI_ASM_State.m_PowerDownParams", [&](const auto& item) { return read_asm_function_param(item); });
			data.anim_mode = plain::read_script_string(native.m_AnimMode);
			data.orient_mode = plain::read_script_string(native.m_OrientMode);
			data.anim_mode_mp = plain::read_script_string(native.m_AnimModeMP);
			data.orient_mode_mp = plain::read_script_string(native.m_OrientModeMP);
			return data;
		}

		void write_ai_asm_state(AI_ASM_State& native, const asset_data::aiasm::ai_asm_state& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.m_PowerDownState = data.power_down_state;
			native.m_PowerDownParams = plain::write_array<ASM_Function_Param>(data.power_down_params, memory, [&](auto& out, const auto& in) { write_asm_function_param(out, in, resolve, memory); });
			native.m_AnimMode = static_cast<scr_string_t>(plain::write_script_string(data.anim_mode));
			native.m_OrientMode = static_cast<scr_string_t>(plain::write_script_string(data.orient_mode));
			native.m_AnimModeMP = static_cast<scr_string_t>(plain::write_script_string(data.anim_mode_mp));
			native.m_OrientModeMP = static_cast<scr_string_t>(plain::write_script_string(data.orient_mode_mp));
			native.m_PowerDownNumParams = plain::count_of<std::int32_t>(data.power_down_params);
		}

		std::size_t size_ai_asm_state([[maybe_unused]] const asset_data::aiasm::ai_asm_state& data)
		{
			std::size_t size = 0;
			if (data.power_down_params)
			{
				size += plain::sized(std::max<std::size_t>(data.power_down_params->size(), 1) * sizeof(ASM_Function_Param));
				for (const auto& item : *data.power_down_params)
				{
					size += size_asm_function_param(item);
				}
			}
			return size;
		}

		asset_data::aiasm::asm_state_m_ai_state read_asm_state_m_ai_state(const ASM_State_Union& native)
		{
			plain::trace("read_asm_state_m_ai_state");
			asset_data::aiasm::asm_state_m_ai_state data;
			data.ai_state = plain::read_single(native.m_AIState, [&](const auto& item) { return read_ai_asm_state(item); });
			return data;
		}

		void write_asm_state_m_ai_state(ASM_State_Union& native, const asset_data::aiasm::asm_state_m_ai_state& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.m_AIState = plain::write_single<AI_ASM_State>(data.ai_state, memory, [&](auto& out, const auto& in) { write_ai_asm_state(out, in, resolve, memory); });
		}

		std::size_t size_asm_state_m_ai_state([[maybe_unused]] const asset_data::aiasm::asm_state_m_ai_state& data)
		{
			std::size_t size = 0;
			if (data.ai_state)
			{
				size += plain::sized(sizeof(AI_ASM_State));
				size += size_ai_asm_state(*data.ai_state);
			}
			return size;
		}

		asset_data::aiasm::asm_state read_asm_state(const ASM_State& native, [[maybe_unused]] const ASM& outer)
		{
			plain::trace("read_asm_state");
			asset_data::aiasm::asm_state data;
			data.transitions = plain::read_array(native.m_Transitions, native.m_NumTransitions, "ASM_State.m_Transitions", [&](const auto& item) { return read_asm_transition(item); });
			data.name = plain::read_script_string(native.m_Name);
			data.id = native.m_ID;
			data.type = native.m_Type;
			data.stance = native.m_Stance;
			data.flags = native.m_Flags;
			data.logic_group_id = native.m_LogicGroupID;
			data.subtrees = plain::read_array(native.m_Subtrees, native.m_NumSubtrees, "ASM_State.m_Subtrees", [&](const auto& item) { return plain::read_script_string(item); });
			data.play_anim = read_asm_function(native.m_PlayAnim);
			data.play_anim_note_handler = read_asm_function(native.m_PlayAnimNoteHandler);
			data.choose_anim_func = read_asm_function(native.m_ChooseAnimFunc);
			data.terminate_func = read_asm_function(native.m_TerminateFunc);
			data.exit_notify = plain::read_script_string(native.m_ExitNotify);
			data.facial_state = plain::read_script_string(native.m_FacialState);
			data.death_state = native.m_DeathState;
			data.death_params = plain::read_array(native.m_DeathParams, native.m_DeathNumParams, "ASM_State.m_DeathParams", [&](const auto& item) { return read_asm_function_param(item); });
			data.pain_state = native.m_PainState;
			data.pain_params = plain::read_array(native.m_PainParams, native.m_PainNumParams, "ASM_State.m_PainParams", [&](const auto& item) { return read_asm_function_param(item); });
			data.execution_state = native.m_ExecutionState;
			data.execution_params = plain::read_array(native.m_ExecutionParams, native.m_ExecutionNumParams, "ASM_State.m_ExecutionParams", [&](const auto& item) { return read_asm_function_param(item); });
			data.u = [&]() -> std::variant<std::monostate, asset_data::aiasm::asm_state_m_ai_state> { switch (static_cast<std::uint32_t>(outer.m_Mode)) { case 0: { plain::module_scope scope(0); auto item = read_asm_state_m_ai_state(native.u); return std::variant<std::monostate, asset_data::aiasm::asm_state_m_ai_state>(std::move(item)); } default: plain::fail("ASM_State.u", "has a tag value without a case"); } }();
			return data;
		}

		void write_asm_state(ASM_State& native, const asset_data::aiasm::asm_state& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.m_Transitions = plain::write_array<ASM_Transition>(data.transitions, memory, [&](auto& out, const auto& in) { write_asm_transition(out, in, resolve, memory); });
			native.m_Name = static_cast<scr_string_t>(plain::write_script_string(data.name));
			native.m_ID = data.id;
			native.m_Type = data.type;
			native.m_Stance = data.stance;
			native.m_Flags = data.flags;
			native.m_LogicGroupID = data.logic_group_id;
			native.m_Subtrees = plain::write_array<scr_string_t>(data.subtrees, memory, [&](auto& out, const auto& in) { out = static_cast<scr_string_t>(plain::write_script_string(in)); });
			write_asm_function(native.m_PlayAnim, data.play_anim, resolve, memory);
			write_asm_function(native.m_PlayAnimNoteHandler, data.play_anim_note_handler, resolve, memory);
			write_asm_function(native.m_ChooseAnimFunc, data.choose_anim_func, resolve, memory);
			write_asm_function(native.m_TerminateFunc, data.terminate_func, resolve, memory);
			native.m_ExitNotify = static_cast<scr_string_t>(plain::write_script_string(data.exit_notify));
			native.m_FacialState = static_cast<scr_string_t>(plain::write_script_string(data.facial_state));
			native.m_DeathState = data.death_state;
			native.m_DeathParams = plain::write_array<ASM_Function_Param>(data.death_params, memory, [&](auto& out, const auto& in) { write_asm_function_param(out, in, resolve, memory); });
			native.m_PainState = data.pain_state;
			native.m_PainParams = plain::write_array<ASM_Function_Param>(data.pain_params, memory, [&](auto& out, const auto& in) { write_asm_function_param(out, in, resolve, memory); });
			native.m_ExecutionState = data.execution_state;
			native.m_ExecutionParams = plain::write_array<ASM_Function_Param>(data.execution_params, memory, [&](auto& out, const auto& in) { write_asm_function_param(out, in, resolve, memory); });
			std::visit([&](const auto& value)
			{
				using value_type = std::decay_t<decltype(value)>;
				if constexpr (std::is_same_v<value_type, std::monostate>)
				{
					plain::fail("ASM_State.u", "holds no module");
				}
				else if constexpr (std::is_same_v<value_type, asset_data::aiasm::asm_state_m_ai_state>)
				{
					plain::module_scope scope(0);
					write_asm_state_m_ai_state(native.u, value, resolve, memory);
				}
			}, data.u);
			native.m_NumTransitions = plain::count_of<std::int32_t>(data.transitions);
			native.m_NumSubtrees = plain::count_of<std::int32_t>(data.subtrees);
			native.m_DeathNumParams = plain::count_of<std::int32_t>(data.death_params);
			native.m_PainNumParams = plain::count_of<std::int32_t>(data.pain_params);
			native.m_ExecutionNumParams = plain::count_of<std::int32_t>(data.execution_params);
		}

		std::size_t size_asm_state([[maybe_unused]] const asset_data::aiasm::asm_state& data)
		{
			std::size_t size = 0;
			if (data.transitions)
			{
				size += plain::sized(std::max<std::size_t>(data.transitions->size(), 1) * sizeof(ASM_Transition));
				for (const auto& item : *data.transitions)
				{
					size += size_asm_transition(item);
				}
			}
			if (data.subtrees)
			{
				size += plain::sized(std::max<std::size_t>(data.subtrees->size(), 1) * sizeof(scr_string_t));
			}
			size += size_asm_function(data.play_anim);
			size += size_asm_function(data.play_anim_note_handler);
			size += size_asm_function(data.choose_anim_func);
			size += size_asm_function(data.terminate_func);
			if (data.death_params)
			{
				size += plain::sized(std::max<std::size_t>(data.death_params->size(), 1) * sizeof(ASM_Function_Param));
				for (const auto& item : *data.death_params)
				{
					size += size_asm_function_param(item);
				}
			}
			if (data.pain_params)
			{
				size += plain::sized(std::max<std::size_t>(data.pain_params->size(), 1) * sizeof(ASM_Function_Param));
				for (const auto& item : *data.pain_params)
				{
					size += size_asm_function_param(item);
				}
			}
			if (data.execution_params)
			{
				size += plain::sized(std::max<std::size_t>(data.execution_params->size(), 1) * sizeof(ASM_Function_Param));
				for (const auto& item : *data.execution_params)
				{
					size += size_asm_function_param(item);
				}
			}
			std::visit([&](const auto& value)
			{
				using value_type = std::decay_t<decltype(value)>;
				if constexpr (std::is_same_v<value_type, std::monostate>)
				{
				}
				else if constexpr (std::is_same_v<value_type, asset_data::aiasm::asm_state_m_ai_state>)
				{
					size += size_asm_state_m_ai_state(value);
				}
			}, data.u);
			return size;
		}

		asset_data::aiasm::asm_transition read_asm_transition(const ASM_Transition& native)
		{
			plain::trace("read_asm_transition");
			asset_data::aiasm::asm_transition data;
			data.to_state_id = native.m_ToStateID;
			data.func_name = read_asm_function(native.m_FuncName);
			data.priority = native.m_Priority;
			return data;
		}

		void write_asm_transition(ASM_Transition& native, const asset_data::aiasm::asm_transition& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.m_ToStateID = data.to_state_id;
			write_asm_function(native.m_FuncName, data.func_name, resolve, memory);
			native.m_Priority = data.priority;
		}

		std::size_t size_asm_transition([[maybe_unused]] const asset_data::aiasm::asm_transition& data)
		{
			std::size_t size = 0;
			size += size_asm_function(data.func_name);
			return size;
		}

		asset_data::aiasm::asm_logic_group read_asm_logic_group(const ASM_LogicGroup& native)
		{
			plain::trace("read_asm_logic_group");
			asset_data::aiasm::asm_logic_group data;
			data.id = native.m_ID;
			data.parent_id = native.m_ParentID;
			data.init_func = read_asm_function(native.m_InitFunc);
			data.terminate_func = read_asm_function(native.m_TerminateFunc);
			return data;
		}

		void write_asm_logic_group(ASM_LogicGroup& native, const asset_data::aiasm::asm_logic_group& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.m_ID = data.id;
			native.m_ParentID = data.parent_id;
			write_asm_function(native.m_InitFunc, data.init_func, resolve, memory);
			write_asm_function(native.m_TerminateFunc, data.terminate_func, resolve, memory);
		}

		std::size_t size_asm_logic_group([[maybe_unused]] const asset_data::aiasm::asm_logic_group& data)
		{
			std::size_t size = 0;
			size += size_asm_function(data.init_func);
			size += size_asm_function(data.terminate_func);
			return size;
		}

		asset_data::aiasm::ai_animset_node_offset read_ai_animset_node_offset(const AIAnimsetNodeOffset& native)
		{
			plain::trace("read_ai_animset_node_offset");
			asset_data::aiasm::ai_animset_node_offset data;
			data.hide_yaw = native.hideYaw;
			data.snap_yaw = native.snapYaw;
			data.lean_yaw = native.leanYaw;
			data.lean_pitch = native.leanPitch;
			return data;
		}

		void write_ai_animset_node_offset(AIAnimsetNodeOffset& native, const asset_data::aiasm::ai_animset_node_offset& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.hideYaw = data.hide_yaw;
			native.snapYaw = data.snap_yaw;
			native.leanYaw = data.lean_yaw;
			native.leanPitch = data.lean_pitch;
		}

		std::size_t size_ai_animset_node_offset([[maybe_unused]] const asset_data::aiasm::ai_animset_node_offset& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::aiasm::ai_animset read_ai_animset(const AIAnimset& native)
		{
			plain::trace("read_ai_animset");
			asset_data::aiasm::ai_animset data;
			data.cover_left_offset = read_ai_animset_node_offset(native.coverLeftOffset);
			data.cover_left_crouch_offset = read_ai_animset_node_offset(native.coverLeftCrouchOffset);
			data.cover_right_offset = read_ai_animset_node_offset(native.coverRightOffset);
			data.cover_right_crouch_offset = read_ai_animset_node_offset(native.coverRightCrouchOffset);
			data.cover_crouch_offset = read_ai_animset_node_offset(native.coverCrouchOffset);
			data.cover_stand_offset = read_ai_animset_node_offset(native.coverStandOffset);
			return data;
		}

		void write_ai_animset(AIAnimset& native, const asset_data::aiasm::ai_animset& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_ai_animset_node_offset(native.coverLeftOffset, data.cover_left_offset, resolve, memory);
			write_ai_animset_node_offset(native.coverLeftCrouchOffset, data.cover_left_crouch_offset, resolve, memory);
			write_ai_animset_node_offset(native.coverRightOffset, data.cover_right_offset, resolve, memory);
			write_ai_animset_node_offset(native.coverRightCrouchOffset, data.cover_right_crouch_offset, resolve, memory);
			write_ai_animset_node_offset(native.coverCrouchOffset, data.cover_crouch_offset, resolve, memory);
			write_ai_animset_node_offset(native.coverStandOffset, data.cover_stand_offset, resolve, memory);
		}

		std::size_t size_ai_animset([[maybe_unused]] const asset_data::aiasm::ai_animset& data)
		{
			std::size_t size = 0;
			size += size_ai_animset_node_offset(data.cover_left_offset);
			size += size_ai_animset_node_offset(data.cover_left_crouch_offset);
			size += size_ai_animset_node_offset(data.cover_right_offset);
			size += size_ai_animset_node_offset(data.cover_right_crouch_offset);
			size += size_ai_animset_node_offset(data.cover_crouch_offset);
			size += size_ai_animset_node_offset(data.cover_stand_offset);
			return size;
		}

		asset_data::aiasm::animset_m_ai_animset read_animset_m_ai_animset(const Animset_Union& native)
		{
			plain::trace("read_animset_m_ai_animset");
			asset_data::aiasm::animset_m_ai_animset data;
			data.ai_animset = plain::read_single(native.m_AIAnimset, [&](const auto& item) { return read_ai_animset(item); });
			return data;
		}

		void write_animset_m_ai_animset(Animset_Union& native, const asset_data::aiasm::animset_m_ai_animset& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.m_AIAnimset = plain::write_single<AIAnimset>(data.ai_animset, memory, [&](auto& out, const auto& in) { write_ai_animset(out, in, resolve, memory); });
		}

		std::size_t size_animset_m_ai_animset([[maybe_unused]] const asset_data::aiasm::animset_m_ai_animset& data)
		{
			std::size_t size = 0;
			if (data.ai_animset)
			{
				size += plain::sized(sizeof(AIAnimset));
				size += size_ai_animset(*data.ai_animset);
			}
			return size;
		}

		asset_data::aiasm::aianimset read_aianimset(const Animset& native)
		{
			plain::trace("read_aianimset");
			asset_data::aiasm::aianimset data;
			data.name = plain::read_string(native.name);
			data.const_name = plain::read_script_string(native.constName);
			data.anim_tree = plain::read_script_string(native.animTree);
			data.states = plain::read_array(native.states, native.numStates, "Animset.states", [&](const auto& item) { return read_animset_state(item, native); });
			data.anim_ctrl = plain::enum_name(asset_data::aiasm::animation_controller_names, static_cast<std::int64_t>(native.animCtrl), "Animset.animCtrl");
			data.start_state_name = plain::read_script_string(native.startStateName);
			data.start_alias_name = plain::read_script_string(native.startAliasName);
			data.clear_state_name = plain::read_script_string(native.clearStateName);
			data.clear_alias_name = plain::read_script_string(native.clearAliasName);
			data.always_on_state_name = plain::read_script_string(native.alwaysOnStateName);
			data.u = [&]() -> std::variant<std::monostate, asset_data::aiasm::animset_m_ai_animset> { switch (static_cast<std::uint32_t>(native.mode)) { case 0: { plain::module_scope scope(0); auto item = read_animset_m_ai_animset(native.u); return std::variant<std::monostate, asset_data::aiasm::animset_m_ai_animset>(std::move(item)); } default: plain::fail("Animset.u", "has a tag value without a case"); } }();
			return data;
		}

		void write_aianimset(Animset& native, const asset_data::aiasm::aianimset& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.constName = static_cast<scr_string_t>(plain::write_script_string(data.const_name));
			native.animTree = static_cast<scr_string_t>(plain::write_script_string(data.anim_tree));
			native.states = plain::write_array<AnimsetState>(data.states, memory, [&](auto& out, const auto& in) { write_animset_state(out, in, resolve, memory); });
			native.animCtrl = static_cast<AnimationController>(plain::enum_value(asset_data::aiasm::animation_controller_names, data.anim_ctrl, "Animset.animCtrl"));
			native.startStateName = static_cast<scr_string_t>(plain::write_script_string(data.start_state_name));
			native.startAliasName = static_cast<scr_string_t>(plain::write_script_string(data.start_alias_name));
			native.clearStateName = static_cast<scr_string_t>(plain::write_script_string(data.clear_state_name));
			native.clearAliasName = static_cast<scr_string_t>(plain::write_script_string(data.clear_alias_name));
			native.alwaysOnStateName = static_cast<scr_string_t>(plain::write_script_string(data.always_on_state_name));
			std::visit([&](const auto& value)
			{
				using value_type = std::decay_t<decltype(value)>;
				if constexpr (std::is_same_v<value_type, std::monostate>)
				{
					plain::fail("Animset.u", "holds no module");
				}
				else if constexpr (std::is_same_v<value_type, asset_data::aiasm::animset_m_ai_animset>)
				{
					native.mode = static_cast<ASM_Mode>(0);
					plain::module_scope scope(0);
					write_animset_m_ai_animset(native.u, value, resolve, memory);
				}
			}, data.u);
			native.numStates = plain::count_of<std::uint32_t>(data.states);
		}

		std::size_t size_aianimset([[maybe_unused]] const asset_data::aiasm::aianimset& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.states)
			{
				size += plain::sized(std::max<std::size_t>(data.states->size(), 1) * sizeof(AnimsetState));
				for (const auto& item : *data.states)
				{
					size += size_animset_state(item);
				}
			}
			std::visit([&](const auto& value)
			{
				using value_type = std::decay_t<decltype(value)>;
				if constexpr (std::is_same_v<value_type, std::monostate>)
				{
				}
				else if constexpr (std::is_same_v<value_type, asset_data::aiasm::animset_m_ai_animset>)
				{
					size += size_animset_m_ai_animset(value);
				}
			}, data.u);
			return size;
		}

		asset_data::aiasm::ai_animset_state read_ai_animset_state(const AIAnimsetState& native)
		{
			plain::trace("read_ai_animset_state");
			asset_data::aiasm::ai_animset_state data;
			plain::require_zero(native.pad_0, "AIAnimsetState.pad_0");
			return data;
		}

		void write_ai_animset_state(AIAnimsetState& native, const asset_data::aiasm::ai_animset_state& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
		}

		std::size_t size_ai_animset_state([[maybe_unused]] const asset_data::aiasm::ai_animset_state& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::aiasm::animset_state_m_ai_animset_state read_animset_state_m_ai_animset_state(const AnimsetState_Union& native)
		{
			plain::trace("read_animset_state_m_ai_animset_state");
			asset_data::aiasm::animset_state_m_ai_animset_state data;
			data.ai_animset_state = plain::read_single(native.m_AIAnimsetState, [&](const auto& item) { return read_ai_animset_state(item); });
			return data;
		}

		void write_animset_state_m_ai_animset_state(AnimsetState_Union& native, const asset_data::aiasm::animset_state_m_ai_animset_state& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.m_AIAnimsetState = plain::write_single<AIAnimsetState>(data.ai_animset_state, memory, [&](auto& out, const auto& in) { write_ai_animset_state(out, in, resolve, memory); });
		}

		std::size_t size_animset_state_m_ai_animset_state([[maybe_unused]] const asset_data::aiasm::animset_state_m_ai_animset_state& data)
		{
			std::size_t size = 0;
			if (data.ai_animset_state)
			{
				size += plain::sized(sizeof(AIAnimsetState));
				size += size_ai_animset_state(*data.ai_animset_state);
			}
			return size;
		}

		asset_data::aiasm::animset_state read_animset_state(const AnimsetState& native, [[maybe_unused]] const Animset& outer)
		{
			plain::trace("read_animset_state");
			asset_data::aiasm::animset_state data;
			data.name = plain::read_script_string(native.name);
			data.anim_aliases = plain::read_array(native.animAliases, native.numAnimAliases, "AnimsetState.animAliases", [&](const auto& item) { return read_animset_alias(item, outer); });
			data.blendtree_aliases = plain::read_array(native.blendtreeAliases, native.numBlendtreeAliases, "AnimsetState.blendtreeAliases", [&](const auto& item) { return read_animset_blendtree_alias(item); });
			data.notify = plain::read_script_string(native.notify);
			data.aimset_target = plain::read_script_string(native.aimsetTarget);
			data.aimset_root = plain::read_script_string(native.aimsetRoot);
			data.anim_entry_map = plain::read_array(native.animEntryMap, native.numAnims, "AnimsetState.animEntryMap", [&](const auto& item) { return read_animset_state_entry_map(item); });
			data.state_type = plain::enum_name(asset_data::aiasm::animset_state_type_names, static_cast<std::int64_t>(native.stateType), "AnimsetState.stateType");
			data.look_at_min_yaw = native.lookAtMinYaw;
			data.look_at_max_yaw = native.lookAtMaxYaw;
			data.look_at_min_pitch = native.lookAtMinPitch;
			data.look_at_max_pitch = native.lookAtMaxPitch;
			data.look_at_neutral_pitch = native.lookAtNeutralPitch;
			data.look_at_neutral_yaw = native.lookAtNeutralYaw;
			data.restart = native.restart;
			data.is_aimset = native.isAimset;
			data.clear_always_on = native.clearAlwaysOn;
			data.u = [&]() -> std::variant<std::monostate, asset_data::aiasm::animset_state_m_ai_animset_state> { switch (static_cast<std::uint32_t>(outer.mode)) { case 0: { plain::module_scope scope(0); auto item = read_animset_state_m_ai_animset_state(native.u); return std::variant<std::monostate, asset_data::aiasm::animset_state_m_ai_animset_state>(std::move(item)); } default: plain::fail("AnimsetState.u", "has a tag value without a case"); } }();
			return data;
		}

		void write_animset_state(AnimsetState& native, const asset_data::aiasm::animset_state& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = static_cast<scr_string_t>(plain::write_script_string(data.name));
			native.animAliases = plain::write_array<AnimsetAlias>(data.anim_aliases, memory, [&](auto& out, const auto& in) { write_animset_alias(out, in, resolve, memory); });
			native.blendtreeAliases = plain::write_array<AnimsetBlendtreeAlias>(data.blendtree_aliases, memory, [&](auto& out, const auto& in) { write_animset_blendtree_alias(out, in, resolve, memory); });
			native.notify = static_cast<scr_string_t>(plain::write_script_string(data.notify));
			native.aimsetTarget = static_cast<scr_string_t>(plain::write_script_string(data.aimset_target));
			native.aimsetRoot = static_cast<scr_string_t>(plain::write_script_string(data.aimset_root));
			native.animEntryMap = plain::write_array<AnimsetStateEntryMap>(data.anim_entry_map, memory, [&](auto& out, const auto& in) { write_animset_state_entry_map(out, in, resolve, memory); });
			native.stateType = static_cast<AnimsetStateType>(plain::enum_value(asset_data::aiasm::animset_state_type_names, data.state_type, "AnimsetState.stateType"));
			native.lookAtMinYaw = data.look_at_min_yaw;
			native.lookAtMaxYaw = data.look_at_max_yaw;
			native.lookAtMinPitch = data.look_at_min_pitch;
			native.lookAtMaxPitch = data.look_at_max_pitch;
			native.lookAtNeutralPitch = data.look_at_neutral_pitch;
			native.lookAtNeutralYaw = data.look_at_neutral_yaw;
			native.restart = data.restart;
			native.isAimset = data.is_aimset;
			native.clearAlwaysOn = data.clear_always_on;
			std::visit([&](const auto& value)
			{
				using value_type = std::decay_t<decltype(value)>;
				if constexpr (std::is_same_v<value_type, std::monostate>)
				{
					plain::fail("AnimsetState.u", "holds no module");
				}
				else if constexpr (std::is_same_v<value_type, asset_data::aiasm::animset_state_m_ai_animset_state>)
				{
					plain::module_scope scope(0);
					write_animset_state_m_ai_animset_state(native.u, value, resolve, memory);
				}
			}, data.u);
			native.numAnimAliases = plain::count_of<std::uint32_t>(data.anim_aliases);
			native.numBlendtreeAliases = plain::count_of<std::uint32_t>(data.blendtree_aliases);
			native.numAnims = plain::count_of<std::uint32_t>(data.anim_entry_map);
		}

		std::size_t size_animset_state([[maybe_unused]] const asset_data::aiasm::animset_state& data)
		{
			std::size_t size = 0;
			if (data.anim_aliases)
			{
				size += plain::sized(std::max<std::size_t>(data.anim_aliases->size(), 1) * sizeof(AnimsetAlias));
				for (const auto& item : *data.anim_aliases)
				{
					size += size_animset_alias(item);
				}
			}
			if (data.blendtree_aliases)
			{
				size += plain::sized(std::max<std::size_t>(data.blendtree_aliases->size(), 1) * sizeof(AnimsetBlendtreeAlias));
				for (const auto& item : *data.blendtree_aliases)
				{
					size += size_animset_blendtree_alias(item);
				}
			}
			if (data.anim_entry_map)
			{
				size += plain::sized(std::max<std::size_t>(data.anim_entry_map->size(), 1) * sizeof(AnimsetStateEntryMap));
				for (const auto& item : *data.anim_entry_map)
				{
					size += size_animset_state_entry_map(item);
				}
			}
			std::visit([&](const auto& value)
			{
				using value_type = std::decay_t<decltype(value)>;
				if constexpr (std::is_same_v<value_type, std::monostate>)
				{
				}
				else if constexpr (std::is_same_v<value_type, asset_data::aiasm::animset_state_m_ai_animset_state>)
				{
					size += size_animset_state_m_ai_animset_state(value);
				}
			}, data.u);
			return size;
		}

		asset_data::aiasm::ai_animset_alias read_ai_animset_alias(const AIAnimsetAlias& native)
		{
			plain::trace("read_ai_animset_alias");
			asset_data::aiasm::ai_animset_alias data;
			data.red_anims = plain::read_array(native.redAnims, native.numRedAnims, "AIAnimsetAlias.redAnims", [&](const auto& item) { return read_animset_anim(item); });
			return data;
		}

		void write_ai_animset_alias(AIAnimsetAlias& native, const asset_data::aiasm::ai_animset_alias& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.redAnims = plain::write_array<AnimsetAnim>(data.red_anims, memory, [&](auto& out, const auto& in) { write_animset_anim(out, in, resolve, memory); });
			native.numRedAnims = plain::count_of<std::uint32_t>(data.red_anims);
		}

		std::size_t size_ai_animset_alias([[maybe_unused]] const asset_data::aiasm::ai_animset_alias& data)
		{
			std::size_t size = 0;
			if (data.red_anims)
			{
				size += plain::sized(std::max<std::size_t>(data.red_anims->size(), 1) * sizeof(AnimsetAnim));
				for (const auto& item : *data.red_anims)
				{
					size += size_animset_anim(item);
				}
			}
			return size;
		}

		asset_data::aiasm::animset_alias_m_ai_animset_alias read_animset_alias_m_ai_animset_alias(const AnimsetAlias_Union& native)
		{
			plain::trace("read_animset_alias_m_ai_animset_alias");
			asset_data::aiasm::animset_alias_m_ai_animset_alias data;
			data.ai_animset_alias = plain::read_single(native.m_AIAnimsetAlias, [&](const auto& item) { return read_ai_animset_alias(item); });
			return data;
		}

		void write_animset_alias_m_ai_animset_alias(AnimsetAlias_Union& native, const asset_data::aiasm::animset_alias_m_ai_animset_alias& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.m_AIAnimsetAlias = plain::write_single<AIAnimsetAlias>(data.ai_animset_alias, memory, [&](auto& out, const auto& in) { write_ai_animset_alias(out, in, resolve, memory); });
		}

		std::size_t size_animset_alias_m_ai_animset_alias([[maybe_unused]] const asset_data::aiasm::animset_alias_m_ai_animset_alias& data)
		{
			std::size_t size = 0;
			if (data.ai_animset_alias)
			{
				size += plain::sized(sizeof(AIAnimsetAlias));
				size += size_ai_animset_alias(*data.ai_animset_alias);
			}
			return size;
		}

		asset_data::aiasm::animset_alias read_animset_alias(const AnimsetAlias& native, [[maybe_unused]] const Animset& outer)
		{
			plain::trace("read_animset_alias");
			asset_data::aiasm::animset_alias data;
			data.name = plain::read_script_string(native.name);
			data.anims = plain::read_array(native.anims, native.numAnims, "AnimsetAlias.anims", [&](const auto& item) { return read_animset_anim(item); });
			data.funcs = plain::read_array(native.funcs, native.numFuncs, "AnimsetAlias.funcs", [&](const auto& item) { return read_asm_function(item); });
			data.addons = plain::read_array(native.addons, native.numAddons, "AnimsetAlias.addons", [&](const auto& item) { return plain::read_script_string(item); });
			data.anim_alias = plain::read_script_string(native.animAlias);
			data.blender_index = native.blenderIndex;
			data.blendtime = native.blendtime;
			data.curve_name = plain::read_script_string(native.curveName);
			data.u = [&]() -> std::variant<std::monostate, asset_data::aiasm::animset_alias_m_ai_animset_alias> { switch (static_cast<std::uint32_t>(outer.mode)) { case 0: { plain::module_scope scope(0); auto item = read_animset_alias_m_ai_animset_alias(native.u); return std::variant<std::monostate, asset_data::aiasm::animset_alias_m_ai_animset_alias>(std::move(item)); } default: plain::fail("AnimsetAlias.u", "has a tag value without a case"); } }();
			return data;
		}

		void write_animset_alias(AnimsetAlias& native, const asset_data::aiasm::animset_alias& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = static_cast<scr_string_t>(plain::write_script_string(data.name));
			native.anims = plain::write_array<AnimsetAnim>(data.anims, memory, [&](auto& out, const auto& in) { write_animset_anim(out, in, resolve, memory); });
			native.funcs = plain::write_array<ASM_Function>(data.funcs, memory, [&](auto& out, const auto& in) { write_asm_function(out, in, resolve, memory); });
			native.addons = plain::write_array<scr_string_t>(data.addons, memory, [&](auto& out, const auto& in) { out = static_cast<scr_string_t>(plain::write_script_string(in)); });
			native.animAlias = static_cast<scr_string_t>(plain::write_script_string(data.anim_alias));
			native.blenderIndex = data.blender_index;
			native.blendtime = data.blendtime;
			native.curveName = static_cast<scr_string_t>(plain::write_script_string(data.curve_name));
			std::visit([&](const auto& value)
			{
				using value_type = std::decay_t<decltype(value)>;
				if constexpr (std::is_same_v<value_type, std::monostate>)
				{
					plain::fail("AnimsetAlias.u", "holds no module");
				}
				else if constexpr (std::is_same_v<value_type, asset_data::aiasm::animset_alias_m_ai_animset_alias>)
				{
					plain::module_scope scope(0);
					write_animset_alias_m_ai_animset_alias(native.u, value, resolve, memory);
				}
			}, data.u);
			native.numAnims = plain::count_of<std::uint32_t>(data.anims);
			native.numFuncs = plain::count_of<std::uint32_t>(data.funcs);
			native.numAddons = plain::count_of<std::uint32_t>(data.addons);
		}

		std::size_t size_animset_alias([[maybe_unused]] const asset_data::aiasm::animset_alias& data)
		{
			std::size_t size = 0;
			if (data.anims)
			{
				size += plain::sized(std::max<std::size_t>(data.anims->size(), 1) * sizeof(AnimsetAnim));
				for (const auto& item : *data.anims)
				{
					size += size_animset_anim(item);
				}
			}
			if (data.funcs)
			{
				size += plain::sized(std::max<std::size_t>(data.funcs->size(), 1) * sizeof(ASM_Function));
				for (const auto& item : *data.funcs)
				{
					size += size_asm_function(item);
				}
			}
			if (data.addons)
			{
				size += plain::sized(std::max<std::size_t>(data.addons->size(), 1) * sizeof(scr_string_t));
			}
			std::visit([&](const auto& value)
			{
				using value_type = std::decay_t<decltype(value)>;
				if constexpr (std::is_same_v<value_type, std::monostate>)
				{
				}
				else if constexpr (std::is_same_v<value_type, asset_data::aiasm::animset_alias_m_ai_animset_alias>)
				{
					size += size_animset_alias_m_ai_animset_alias(value);
				}
			}, data.u);
			return size;
		}

		asset_data::aiasm::animset_anim read_animset_anim(const AnimsetAnim& native)
		{
			plain::trace("read_animset_anim");
			asset_data::aiasm::animset_anim data;
			data.name = plain::read_script_string(native.name);
			data.weight = native.weight;
			data.aimset_name = plain::read_script_string(native.aimsetName);
			return data;
		}

		void write_animset_anim(AnimsetAnim& native, const asset_data::aiasm::animset_anim& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = static_cast<scr_string_t>(plain::write_script_string(data.name));
			native.weight = data.weight;
			native.aimsetName = static_cast<scr_string_t>(plain::write_script_string(data.aimset_name));
		}

		std::size_t size_animset_anim([[maybe_unused]] const asset_data::aiasm::animset_anim& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::aiasm::animset_blendtree_alias read_animset_blendtree_alias(const AnimsetBlendtreeAlias& native)
		{
			plain::trace("read_animset_blendtree_alias");
			asset_data::aiasm::animset_blendtree_alias data;
			data.name = plain::read_script_string(native.name);
			data.graft_node = plain::read_script_string(native.graftNode);
			data.blendtree_name = plain::read_script_string(native.blendtreeName);
			data.funcs = plain::read_array(native.funcs, native.numFuncs, "AnimsetBlendtreeAlias.funcs", [&](const auto& item) { return read_asm_function(item); });
			data.b_notify = native.bNotify;
			return data;
		}

		void write_animset_blendtree_alias(AnimsetBlendtreeAlias& native, const asset_data::aiasm::animset_blendtree_alias& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = static_cast<scr_string_t>(plain::write_script_string(data.name));
			native.graftNode = static_cast<scr_string_t>(plain::write_script_string(data.graft_node));
			native.blendtreeName = static_cast<scr_string_t>(plain::write_script_string(data.blendtree_name));
			native.funcs = plain::write_array<ASM_Function>(data.funcs, memory, [&](auto& out, const auto& in) { write_asm_function(out, in, resolve, memory); });
			native.bNotify = data.b_notify;
			native.numFuncs = plain::count_of<std::uint32_t>(data.funcs);
		}

		std::size_t size_animset_blendtree_alias([[maybe_unused]] const asset_data::aiasm::animset_blendtree_alias& data)
		{
			std::size_t size = 0;
			if (data.funcs)
			{
				size += plain::sized(std::max<std::size_t>(data.funcs->size(), 1) * sizeof(ASM_Function));
				for (const auto& item : *data.funcs)
				{
					size += size_asm_function(item);
				}
			}
			return size;
		}

		asset_data::aiasm::animset_state_entry_map read_animset_state_entry_map(const AnimsetStateEntryMap& native)
		{
			plain::trace("read_animset_state_entry_map");
			asset_data::aiasm::animset_state_entry_map data;
			data.alias_index = native.aliasIndex;
			data.alias_anim_entry_index = native.aliasAnimEntryIndex;
			return data;
		}

		void write_animset_state_entry_map(AnimsetStateEntryMap& native, const asset_data::aiasm::animset_state_entry_map& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.aliasIndex = data.alias_index;
			native.aliasAnimEntryIndex = data.alias_anim_entry_index;
		}

		std::size_t size_animset_state_entry_map([[maybe_unused]] const asset_data::aiasm::animset_state_entry_map& data)
		{
			std::size_t size = 0;
			return size;
		}

	}

	std::expected<asset_data::aiasm::aiasm, std::string> to_neutral(const ASM& native)
	{
		try
		{
			auto data = read_aiasm(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	ASM* from_neutral(const asset_data::aiasm::aiasm& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<ASM>();
		write_aiasm(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::aiasm::aiasm& data)
	{
		return plain::rounded(sizeof(ASM)) + size_aiasm(data) + 64;
	}

	std::expected<asset_data::aiasm::aianimset, std::string> to_neutral(const Animset& native)
	{
		try
		{
			auto data = read_aianimset(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	Animset* from_neutral(const asset_data::aiasm::aianimset& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<Animset>();
		write_aianimset(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::aiasm::aianimset& data)
	{
		return plain::rounded(sizeof(Animset)) + size_aianimset(data) + 64;
	}

}
