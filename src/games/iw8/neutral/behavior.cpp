// Emitted by tools/gen_iw8_neutral.py from tools/neutral_families/behavior.json: do not edit.
#include <std_include.hpp>
#include "behavior.hpp"
#include "plain.hpp"

namespace zonetool::iw8::neutral
{
	namespace
	{
		asset_data::behavior::behaviortree read_behaviortree(const BehaviorTree& native);
		void write_behaviortree(BehaviorTree& native, const asset_data::behavior::behaviortree& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_behaviortree(const asset_data::behavior::behaviortree& data);
		asset_data::behavior::behavior_tree_node read_behavior_tree_node(const BehaviorTreeNode& native);
		void write_behavior_tree_node(BehaviorTreeNode& native, const asset_data::behavior::behavior_tree_node& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_behavior_tree_node(const asset_data::behavior::behavior_tree_node& data);
		asset_data::behavior::selector_children read_selector_children(const BehaviorTreeNodeChildren& native);
		void write_selector_children(BehaviorTreeNodeChildren& native, const asset_data::behavior::selector_children& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_selector_children(const asset_data::behavior::selector_children& data);
		asset_data::behavior::sequence_children read_sequence_children(const BehaviorTreeNodeChildren& native);
		void write_sequence_children(BehaviorTreeNodeChildren& native, const asset_data::behavior::sequence_children& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_sequence_children(const asset_data::behavior::sequence_children& data);
		asset_data::behavior::active_selector_children read_active_selector_children(const BehaviorTreeNodeChildren& native);
		void write_active_selector_children(BehaviorTreeNodeChildren& native, const asset_data::behavior::active_selector_children& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_active_selector_children(const asset_data::behavior::active_selector_children& data);
		asset_data::behavior::active_sequence_children read_active_sequence_children(const BehaviorTreeNodeChildren& native);
		void write_active_sequence_children(BehaviorTreeNodeChildren& native, const asset_data::behavior::active_sequence_children& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_active_sequence_children(const asset_data::behavior::active_sequence_children& data);
		asset_data::behavior::parallel_children read_parallel_children(const BehaviorTreeNodeChildren& native);
		void write_parallel_children(BehaviorTreeNodeChildren& native, const asset_data::behavior::parallel_children& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_parallel_children(const asset_data::behavior::parallel_children& data);
		asset_data::behavior::bsm_children read_bsm_children(const BehaviorTreeNodeChildren& native);
		void write_bsm_children(BehaviorTreeNodeChildren& native, const asset_data::behavior::bsm_children& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_bsm_children(const asset_data::behavior::bsm_children& data);
		asset_data::behavior::behavior_tree_action_data read_behavior_tree_action_data(const BehaviorTreeActionData& native);
		void write_behavior_tree_action_data(BehaviorTreeActionData& native, const asset_data::behavior::behavior_tree_action_data& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_behavior_tree_action_data(const asset_data::behavior::behavior_tree_action_data& data);
		asset_data::behavior::negate_children read_negate_children(const BehaviorTreeNodeChildren& native);
		void write_negate_children(BehaviorTreeNodeChildren& native, const asset_data::behavior::negate_children& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_negate_children(const asset_data::behavior::negate_children& data);
		asset_data::behavior::prefab_children read_prefab_children(const BehaviorTreeNodeChildren& native);
		void write_prefab_children(BehaviorTreeNodeChildren& native, const asset_data::behavior::prefab_children& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_prefab_children(const asset_data::behavior::prefab_children& data);
		asset_data::behavior::behavior_tree_decorator_children read_behavior_tree_decorator_children(const BehaviorTreeDecoratorChildren& native);
		void write_behavior_tree_decorator_children(BehaviorTreeDecoratorChildren& native, const asset_data::behavior::behavior_tree_decorator_children& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_behavior_tree_decorator_children(const asset_data::behavior::behavior_tree_decorator_children& data);
		asset_data::behavior::behavior_tree_decorator read_behavior_tree_decorator(const BehaviorTreeDecorator& native);
		void write_behavior_tree_decorator(BehaviorTreeDecorator& native, const asset_data::behavior::behavior_tree_decorator& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_behavior_tree_decorator(const asset_data::behavior::behavior_tree_decorator& data);
		asset_data::behavior::behavior_tree_decorator_terminate read_behavior_tree_decorator_terminate(const BehaviorTreeDecoratorTerminate& native);
		void write_behavior_tree_decorator_terminate(BehaviorTreeDecoratorTerminate& native, const asset_data::behavior::behavior_tree_decorator_terminate& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_behavior_tree_decorator_terminate(const asset_data::behavior::behavior_tree_decorator_terminate& data);
		asset_data::behavior::behavior_tree_decorator_init read_behavior_tree_decorator_init(const BehaviorTreeDecoratorInit& native);
		void write_behavior_tree_decorator_init(BehaviorTreeDecoratorInit& native, const asset_data::behavior::behavior_tree_decorator_init& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_behavior_tree_decorator_init(const asset_data::behavior::behavior_tree_decorator_init& data);
		asset_data::behavior::behavior_tree_decorator_root_init read_behavior_tree_decorator_root_init(const BehaviorTreeDecoratorRootInit& native);
		void write_behavior_tree_decorator_root_init(BehaviorTreeDecoratorRootInit& native, const asset_data::behavior::behavior_tree_decorator_root_init& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_behavior_tree_decorator_root_init(const asset_data::behavior::behavior_tree_decorator_root_init& data);

		asset_data::behavior::behaviortree read_behaviortree(const BehaviorTree& native)
		{
			plain::trace("read_behaviortree");
			asset_data::behavior::behaviortree data;
			data.name = plain::read_string(native.name);
			data.nodes = plain::read_array(native.nodes, native.nodeCount, "BehaviorTree.nodes", [&](const auto& item) { return read_behavior_tree_node(item); });
			data.func_i_ds = plain::read_array(native.funcIDs, native.numFuncIDs, "BehaviorTree.funcIDs", [&](const auto& item) { return item; });
			data.decorators = plain::read_array(native.decorators, native.decoratorCount, "BehaviorTree.decorators", [&](const auto& item) { return read_behavior_tree_decorator(item); });
			return data;
		}

		void write_behaviortree(BehaviorTree& native, const asset_data::behavior::behaviortree& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.nodes = plain::write_array<BehaviorTreeNode>(data.nodes, memory, [&](auto& out, const auto& in) { write_behavior_tree_node(out, in, resolve, memory); });
			native.funcIDs = plain::write_array<std::int32_t>(data.func_i_ds, memory, [&](auto& out, const auto& in) { out = in; });
			native.decorators = plain::write_array<BehaviorTreeDecorator>(data.decorators, memory, [&](auto& out, const auto& in) { write_behavior_tree_decorator(out, in, resolve, memory); });
			native.nodeCount = plain::count_of<std::uint16_t>(data.nodes);
			native.numFuncIDs = plain::count_of<std::uint16_t>(data.func_i_ds);
			native.decoratorCount = plain::count_of<std::uint16_t>(data.decorators);
		}

		std::size_t size_behaviortree([[maybe_unused]] const asset_data::behavior::behaviortree& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.nodes)
			{
				size += plain::sized(std::max<std::size_t>(data.nodes->size(), 1) * sizeof(BehaviorTreeNode));
				for (const auto& item : *data.nodes)
				{
					size += size_behavior_tree_node(item);
				}
			}
			if (data.func_i_ds)
			{
				size += plain::sized(std::max<std::size_t>(data.func_i_ds->size(), 1) * sizeof(std::int32_t));
			}
			if (data.decorators)
			{
				size += plain::sized(std::max<std::size_t>(data.decorators->size(), 1) * sizeof(BehaviorTreeDecorator));
				for (const auto& item : *data.decorators)
				{
					size += size_behavior_tree_decorator(item);
				}
			}
			return size;
		}

		asset_data::behavior::selector_children read_selector_children(const BehaviorTreeNodeChildren& native)
		{
			plain::trace("read_selector_children");
			asset_data::behavior::selector_children data;
			data.first_child_offset = native.firstChildOffset;
			data.child_count = native.childCount;
			return data;
		}

		void write_selector_children(BehaviorTreeNodeChildren& native, const asset_data::behavior::selector_children& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.firstChildOffset = data.first_child_offset;
			native.childCount = data.child_count;
		}

		std::size_t size_selector_children([[maybe_unused]] const asset_data::behavior::selector_children& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::behavior::sequence_children read_sequence_children(const BehaviorTreeNodeChildren& native)
		{
			plain::trace("read_sequence_children");
			asset_data::behavior::sequence_children data;
			data.first_child_offset = native.firstChildOffset;
			data.child_count = native.childCount;
			return data;
		}

		void write_sequence_children(BehaviorTreeNodeChildren& native, const asset_data::behavior::sequence_children& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.firstChildOffset = data.first_child_offset;
			native.childCount = data.child_count;
		}

		std::size_t size_sequence_children([[maybe_unused]] const asset_data::behavior::sequence_children& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::behavior::active_selector_children read_active_selector_children(const BehaviorTreeNodeChildren& native)
		{
			plain::trace("read_active_selector_children");
			asset_data::behavior::active_selector_children data;
			data.first_child_offset = native.firstChildOffset;
			data.child_count = native.childCount;
			return data;
		}

		void write_active_selector_children(BehaviorTreeNodeChildren& native, const asset_data::behavior::active_selector_children& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.firstChildOffset = data.first_child_offset;
			native.childCount = data.child_count;
		}

		std::size_t size_active_selector_children([[maybe_unused]] const asset_data::behavior::active_selector_children& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::behavior::active_sequence_children read_active_sequence_children(const BehaviorTreeNodeChildren& native)
		{
			plain::trace("read_active_sequence_children");
			asset_data::behavior::active_sequence_children data;
			data.first_child_offset = native.firstChildOffset;
			data.child_count = native.childCount;
			return data;
		}

		void write_active_sequence_children(BehaviorTreeNodeChildren& native, const asset_data::behavior::active_sequence_children& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.firstChildOffset = data.first_child_offset;
			native.childCount = data.child_count;
		}

		std::size_t size_active_sequence_children([[maybe_unused]] const asset_data::behavior::active_sequence_children& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::behavior::parallel_children read_parallel_children(const BehaviorTreeNodeChildren& native)
		{
			plain::trace("read_parallel_children");
			asset_data::behavior::parallel_children data;
			data.first_child_offset = native.firstChildOffset;
			data.child_count = native.childCount;
			return data;
		}

		void write_parallel_children(BehaviorTreeNodeChildren& native, const asset_data::behavior::parallel_children& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.firstChildOffset = data.first_child_offset;
			native.childCount = data.child_count;
		}

		std::size_t size_parallel_children([[maybe_unused]] const asset_data::behavior::parallel_children& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::behavior::bsm_children read_bsm_children(const BehaviorTreeNodeChildren& native)
		{
			plain::trace("read_bsm_children");
			asset_data::behavior::bsm_children data;
			data.first_child_offset = native.firstChildOffset;
			data.child_count = native.childCount;
			return data;
		}

		void write_bsm_children(BehaviorTreeNodeChildren& native, const asset_data::behavior::bsm_children& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.firstChildOffset = data.first_child_offset;
			native.childCount = data.child_count;
		}

		std::size_t size_bsm_children([[maybe_unused]] const asset_data::behavior::bsm_children& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::behavior::behavior_tree_action_data read_behavior_tree_action_data(const BehaviorTreeActionData& native)
		{
			plain::trace("read_behavior_tree_action_data");
			asset_data::behavior::behavior_tree_action_data data;
			data.init_fn = native.initFn;
			data.process_fn = native.processFn;
			data.terminate_fn = native.terminateFn;
			data.process_args_fn = native.processArgsFn;
			data.b_process_builtin = native.bProcessBuiltin;
			data.b_init_builtin = native.bInitBuiltin;
			data.b_terminate_builtin = native.bTerminateBuiltin;
			return data;
		}

		void write_behavior_tree_action_data(BehaviorTreeActionData& native, const asset_data::behavior::behavior_tree_action_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.initFn = data.init_fn;
			native.processFn = data.process_fn;
			native.terminateFn = data.terminate_fn;
			native.processArgsFn = data.process_args_fn;
			native.bProcessBuiltin = data.b_process_builtin;
			native.bInitBuiltin = data.b_init_builtin;
			native.bTerminateBuiltin = data.b_terminate_builtin;
		}

		std::size_t size_behavior_tree_action_data([[maybe_unused]] const asset_data::behavior::behavior_tree_action_data& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::behavior::negate_children read_negate_children(const BehaviorTreeNodeChildren& native)
		{
			plain::trace("read_negate_children");
			asset_data::behavior::negate_children data;
			data.first_child_offset = native.firstChildOffset;
			data.child_count = native.childCount;
			return data;
		}

		void write_negate_children(BehaviorTreeNodeChildren& native, const asset_data::behavior::negate_children& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.firstChildOffset = data.first_child_offset;
			native.childCount = data.child_count;
		}

		std::size_t size_negate_children([[maybe_unused]] const asset_data::behavior::negate_children& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::behavior::prefab_children read_prefab_children(const BehaviorTreeNodeChildren& native)
		{
			plain::trace("read_prefab_children");
			asset_data::behavior::prefab_children data;
			data.first_child_offset = native.firstChildOffset;
			data.child_count = native.childCount;
			return data;
		}

		void write_prefab_children(BehaviorTreeNodeChildren& native, const asset_data::behavior::prefab_children& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.firstChildOffset = data.first_child_offset;
			native.childCount = data.child_count;
		}

		std::size_t size_prefab_children([[maybe_unused]] const asset_data::behavior::prefab_children& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::behavior::behavior_tree_decorator_children read_behavior_tree_decorator_children(const BehaviorTreeDecoratorChildren& native)
		{
			plain::trace("read_behavior_tree_decorator_children");
			asset_data::behavior::behavior_tree_decorator_children data;
			data.first_decorator = native.firstDecorator;
			data.decorator_count = native.decoratorCount;
			return data;
		}

		void write_behavior_tree_decorator_children(BehaviorTreeDecoratorChildren& native, const asset_data::behavior::behavior_tree_decorator_children& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.firstDecorator = data.first_decorator;
			native.decoratorCount = data.decorator_count;
		}

		std::size_t size_behavior_tree_decorator_children([[maybe_unused]] const asset_data::behavior::behavior_tree_decorator_children& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::behavior::behavior_tree_node read_behavior_tree_node(const BehaviorTreeNode& native)
		{
			plain::trace("read_behavior_tree_node");
			asset_data::behavior::behavior_tree_node data;
			data.name = plain::read_string(native.name);
			data.result_override = plain::enum_name(asset_data::behavior::behavior_tree_result_names, static_cast<std::int64_t>(native.resultOverride), "BehaviorTreeNode.resultOverride");
			data.schedule = native.schedule;
			data.data = [&]() -> std::variant<std::monostate, asset_data::behavior::selector_children, asset_data::behavior::sequence_children, asset_data::behavior::active_selector_children, asset_data::behavior::active_sequence_children, asset_data::behavior::parallel_children, asset_data::behavior::bsm_children, asset_data::behavior::behavior_tree_action_data, asset_data::behavior::negate_children, asset_data::behavior::prefab_children> { switch (static_cast<std::uint32_t>(native.type)) { case 1: { plain::module_scope scope(1); auto item = read_selector_children(native.data.children); return std::variant<std::monostate, asset_data::behavior::selector_children, asset_data::behavior::sequence_children, asset_data::behavior::active_selector_children, asset_data::behavior::active_sequence_children, asset_data::behavior::parallel_children, asset_data::behavior::bsm_children, asset_data::behavior::behavior_tree_action_data, asset_data::behavior::negate_children, asset_data::behavior::prefab_children>(std::move(item)); } case 2: { plain::module_scope scope(2); auto item = read_sequence_children(native.data.children); return std::variant<std::monostate, asset_data::behavior::selector_children, asset_data::behavior::sequence_children, asset_data::behavior::active_selector_children, asset_data::behavior::active_sequence_children, asset_data::behavior::parallel_children, asset_data::behavior::bsm_children, asset_data::behavior::behavior_tree_action_data, asset_data::behavior::negate_children, asset_data::behavior::prefab_children>(std::move(item)); } case 3: { plain::module_scope scope(3); auto item = read_active_selector_children(native.data.children); return std::variant<std::monostate, asset_data::behavior::selector_children, asset_data::behavior::sequence_children, asset_data::behavior::active_selector_children, asset_data::behavior::active_sequence_children, asset_data::behavior::parallel_children, asset_data::behavior::bsm_children, asset_data::behavior::behavior_tree_action_data, asset_data::behavior::negate_children, asset_data::behavior::prefab_children>(std::move(item)); } case 4: { plain::module_scope scope(4); auto item = read_active_sequence_children(native.data.children); return std::variant<std::monostate, asset_data::behavior::selector_children, asset_data::behavior::sequence_children, asset_data::behavior::active_selector_children, asset_data::behavior::active_sequence_children, asset_data::behavior::parallel_children, asset_data::behavior::bsm_children, asset_data::behavior::behavior_tree_action_data, asset_data::behavior::negate_children, asset_data::behavior::prefab_children>(std::move(item)); } case 5: { plain::module_scope scope(5); auto item = read_parallel_children(native.data.children); return std::variant<std::monostate, asset_data::behavior::selector_children, asset_data::behavior::sequence_children, asset_data::behavior::active_selector_children, asset_data::behavior::active_sequence_children, asset_data::behavior::parallel_children, asset_data::behavior::bsm_children, asset_data::behavior::behavior_tree_action_data, asset_data::behavior::negate_children, asset_data::behavior::prefab_children>(std::move(item)); } case 6: { plain::module_scope scope(6); auto item = read_bsm_children(native.data.children); return std::variant<std::monostate, asset_data::behavior::selector_children, asset_data::behavior::sequence_children, asset_data::behavior::active_selector_children, asset_data::behavior::active_sequence_children, asset_data::behavior::parallel_children, asset_data::behavior::bsm_children, asset_data::behavior::behavior_tree_action_data, asset_data::behavior::negate_children, asset_data::behavior::prefab_children>(std::move(item)); } case 7: { plain::module_scope scope(7); auto item = read_behavior_tree_action_data(native.data.actionData); return std::variant<std::monostate, asset_data::behavior::selector_children, asset_data::behavior::sequence_children, asset_data::behavior::active_selector_children, asset_data::behavior::active_sequence_children, asset_data::behavior::parallel_children, asset_data::behavior::bsm_children, asset_data::behavior::behavior_tree_action_data, asset_data::behavior::negate_children, asset_data::behavior::prefab_children>(std::move(item)); } case 8: { plain::module_scope scope(8); auto item = read_negate_children(native.data.children); return std::variant<std::monostate, asset_data::behavior::selector_children, asset_data::behavior::sequence_children, asset_data::behavior::active_selector_children, asset_data::behavior::active_sequence_children, asset_data::behavior::parallel_children, asset_data::behavior::bsm_children, asset_data::behavior::behavior_tree_action_data, asset_data::behavior::negate_children, asset_data::behavior::prefab_children>(std::move(item)); } case 9: { plain::module_scope scope(9); auto item = read_prefab_children(native.data.children); return std::variant<std::monostate, asset_data::behavior::selector_children, asset_data::behavior::sequence_children, asset_data::behavior::active_selector_children, asset_data::behavior::active_sequence_children, asset_data::behavior::parallel_children, asset_data::behavior::bsm_children, asset_data::behavior::behavior_tree_action_data, asset_data::behavior::negate_children, asset_data::behavior::prefab_children>(std::move(item)); } default: plain::fail("BehaviorTreeNode.data", "has a tag value without a case"); } }();
			data.decorators = read_behavior_tree_decorator_children(native.decorators);
			return data;
		}

		void write_behavior_tree_node(BehaviorTreeNode& native, const asset_data::behavior::behavior_tree_node& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.resultOverride = static_cast<BehaviorTreeResult>(plain::enum_value(asset_data::behavior::behavior_tree_result_names, data.result_override, "BehaviorTreeNode.resultOverride"));
			native.schedule = data.schedule;
			std::visit([&](const auto& value)
			{
				using value_type = std::decay_t<decltype(value)>;
				if constexpr (std::is_same_v<value_type, std::monostate>)
				{
					plain::fail("BehaviorTreeNode.data", "holds no module");
				}
				else if constexpr (std::is_same_v<value_type, asset_data::behavior::selector_children>)
				{
					native.type = static_cast<BehaviorTreeNodeType>(1);
					plain::module_scope scope(1);
					write_selector_children(native.data.children, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::behavior::sequence_children>)
				{
					native.type = static_cast<BehaviorTreeNodeType>(2);
					plain::module_scope scope(2);
					write_sequence_children(native.data.children, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::behavior::active_selector_children>)
				{
					native.type = static_cast<BehaviorTreeNodeType>(3);
					plain::module_scope scope(3);
					write_active_selector_children(native.data.children, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::behavior::active_sequence_children>)
				{
					native.type = static_cast<BehaviorTreeNodeType>(4);
					plain::module_scope scope(4);
					write_active_sequence_children(native.data.children, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::behavior::parallel_children>)
				{
					native.type = static_cast<BehaviorTreeNodeType>(5);
					plain::module_scope scope(5);
					write_parallel_children(native.data.children, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::behavior::bsm_children>)
				{
					native.type = static_cast<BehaviorTreeNodeType>(6);
					plain::module_scope scope(6);
					write_bsm_children(native.data.children, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::behavior::behavior_tree_action_data>)
				{
					native.type = static_cast<BehaviorTreeNodeType>(7);
					plain::module_scope scope(7);
					write_behavior_tree_action_data(native.data.actionData, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::behavior::negate_children>)
				{
					native.type = static_cast<BehaviorTreeNodeType>(8);
					plain::module_scope scope(8);
					write_negate_children(native.data.children, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::behavior::prefab_children>)
				{
					native.type = static_cast<BehaviorTreeNodeType>(9);
					plain::module_scope scope(9);
					write_prefab_children(native.data.children, value, resolve, memory);
				}
			}, data.data);
			write_behavior_tree_decorator_children(native.decorators, data.decorators, resolve, memory);
		}

		std::size_t size_behavior_tree_node([[maybe_unused]] const asset_data::behavior::behavior_tree_node& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			std::visit([&](const auto& value)
			{
				using value_type = std::decay_t<decltype(value)>;
				if constexpr (std::is_same_v<value_type, std::monostate>)
				{
				}
				else if constexpr (std::is_same_v<value_type, asset_data::behavior::selector_children>)
				{
					size += size_selector_children(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::behavior::sequence_children>)
				{
					size += size_sequence_children(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::behavior::active_selector_children>)
				{
					size += size_active_selector_children(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::behavior::active_sequence_children>)
				{
					size += size_active_sequence_children(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::behavior::parallel_children>)
				{
					size += size_parallel_children(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::behavior::bsm_children>)
				{
					size += size_bsm_children(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::behavior::behavior_tree_action_data>)
				{
					size += size_behavior_tree_action_data(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::behavior::negate_children>)
				{
					size += size_negate_children(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::behavior::prefab_children>)
				{
					size += size_prefab_children(value);
				}
			}, data.data);
			size += size_behavior_tree_decorator_children(data.decorators);
			return size;
		}

		asset_data::behavior::behavior_tree_decorator_terminate read_behavior_tree_decorator_terminate(const BehaviorTreeDecoratorTerminate& native)
		{
			plain::trace("read_behavior_tree_decorator_terminate");
			asset_data::behavior::behavior_tree_decorator_terminate data;
			data.terminate_fn = native.terminateFn;
			data.b_terminate_builtin = native.bTerminateBuiltin;
			return data;
		}

		void write_behavior_tree_decorator_terminate(BehaviorTreeDecoratorTerminate& native, const asset_data::behavior::behavior_tree_decorator_terminate& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.terminateFn = data.terminate_fn;
			native.bTerminateBuiltin = data.b_terminate_builtin;
		}

		std::size_t size_behavior_tree_decorator_terminate([[maybe_unused]] const asset_data::behavior::behavior_tree_decorator_terminate& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::behavior::behavior_tree_decorator_init read_behavior_tree_decorator_init(const BehaviorTreeDecoratorInit& native)
		{
			plain::trace("read_behavior_tree_decorator_init");
			asset_data::behavior::behavior_tree_decorator_init data;
			data.init_fn = native.initFn;
			data.b_init_builtin = native.bInitBuiltin;
			return data;
		}

		void write_behavior_tree_decorator_init(BehaviorTreeDecoratorInit& native, const asset_data::behavior::behavior_tree_decorator_init& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.initFn = data.init_fn;
			native.bInitBuiltin = data.b_init_builtin;
		}

		std::size_t size_behavior_tree_decorator_init([[maybe_unused]] const asset_data::behavior::behavior_tree_decorator_init& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::behavior::behavior_tree_decorator_root_init read_behavior_tree_decorator_root_init(const BehaviorTreeDecoratorRootInit& native)
		{
			plain::trace("read_behavior_tree_decorator_root_init");
			asset_data::behavior::behavior_tree_decorator_root_init data;
			data.init_fn = native.initFn;
			data.b_init_builtin = native.bInitBuiltin;
			return data;
		}

		void write_behavior_tree_decorator_root_init(BehaviorTreeDecoratorRootInit& native, const asset_data::behavior::behavior_tree_decorator_root_init& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.initFn = data.init_fn;
			native.bInitBuiltin = data.b_init_builtin;
		}

		std::size_t size_behavior_tree_decorator_root_init([[maybe_unused]] const asset_data::behavior::behavior_tree_decorator_root_init& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::behavior::behavior_tree_decorator read_behavior_tree_decorator(const BehaviorTreeDecorator& native)
		{
			plain::trace("read_behavior_tree_decorator");
			asset_data::behavior::behavior_tree_decorator data;
			data.data = [&]() -> std::variant<std::monostate, asset_data::behavior::behavior_tree_decorator_terminate, asset_data::behavior::behavior_tree_decorator_init, asset_data::behavior::behavior_tree_decorator_root_init> { switch (static_cast<std::uint32_t>(native.type)) { case 0: { plain::module_scope scope(0); auto item = read_behavior_tree_decorator_terminate(native.data.terminate); return std::variant<std::monostate, asset_data::behavior::behavior_tree_decorator_terminate, asset_data::behavior::behavior_tree_decorator_init, asset_data::behavior::behavior_tree_decorator_root_init>(std::move(item)); } case 1: { plain::module_scope scope(1); auto item = read_behavior_tree_decorator_init(native.data.init); return std::variant<std::monostate, asset_data::behavior::behavior_tree_decorator_terminate, asset_data::behavior::behavior_tree_decorator_init, asset_data::behavior::behavior_tree_decorator_root_init>(std::move(item)); } case 2: { plain::module_scope scope(2); auto item = read_behavior_tree_decorator_root_init(native.data.rootInit); return std::variant<std::monostate, asset_data::behavior::behavior_tree_decorator_terminate, asset_data::behavior::behavior_tree_decorator_init, asset_data::behavior::behavior_tree_decorator_root_init>(std::move(item)); } default: plain::fail("BehaviorTreeDecorator.data", "has a tag value without a case"); } }();
			return data;
		}

		void write_behavior_tree_decorator(BehaviorTreeDecorator& native, const asset_data::behavior::behavior_tree_decorator& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			std::visit([&](const auto& value)
			{
				using value_type = std::decay_t<decltype(value)>;
				if constexpr (std::is_same_v<value_type, std::monostate>)
				{
					plain::fail("BehaviorTreeDecorator.data", "holds no module");
				}
				else if constexpr (std::is_same_v<value_type, asset_data::behavior::behavior_tree_decorator_terminate>)
				{
					native.type = static_cast<BehaviorTreeDecoratorType>(0);
					plain::module_scope scope(0);
					write_behavior_tree_decorator_terminate(native.data.terminate, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::behavior::behavior_tree_decorator_init>)
				{
					native.type = static_cast<BehaviorTreeDecoratorType>(1);
					plain::module_scope scope(1);
					write_behavior_tree_decorator_init(native.data.init, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::behavior::behavior_tree_decorator_root_init>)
				{
					native.type = static_cast<BehaviorTreeDecoratorType>(2);
					plain::module_scope scope(2);
					write_behavior_tree_decorator_root_init(native.data.rootInit, value, resolve, memory);
				}
			}, data.data);
		}

		std::size_t size_behavior_tree_decorator([[maybe_unused]] const asset_data::behavior::behavior_tree_decorator& data)
		{
			std::size_t size = 0;
			std::visit([&](const auto& value)
			{
				using value_type = std::decay_t<decltype(value)>;
				if constexpr (std::is_same_v<value_type, std::monostate>)
				{
				}
				else if constexpr (std::is_same_v<value_type, asset_data::behavior::behavior_tree_decorator_terminate>)
				{
					size += size_behavior_tree_decorator_terminate(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::behavior::behavior_tree_decorator_init>)
				{
					size += size_behavior_tree_decorator_init(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::behavior::behavior_tree_decorator_root_init>)
				{
					size += size_behavior_tree_decorator_root_init(value);
				}
			}, data.data);
			return size;
		}

	}

	std::expected<asset_data::behavior::behaviortree, std::string> to_neutral(const BehaviorTree& native)
	{
		try
		{
			auto data = read_behaviortree(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	BehaviorTree* from_neutral(const asset_data::behavior::behaviortree& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<BehaviorTree>();
		write_behaviortree(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::behavior::behaviortree& data)
	{
		return plain::rounded(sizeof(BehaviorTree)) + size_behaviortree(data) + 64;
	}

}
