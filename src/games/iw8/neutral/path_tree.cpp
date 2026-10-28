#include <std_include.hpp>
#include "path_tree.hpp"

namespace zonetool::iw8::neutral
{
	asset_data::path_tree read_path_tree(const pathnode_tree_t& native)
	{
		plain::trace("read_path_tree");
		asset_data::path_tree data;
		data.axis = native.axis;
		data.dist = native.dist;
		if (native.axis < 0)
		{
			// A leaf: the count takes the first four bytes of the union and the list of path nodes its second pointer.
			plain::require_zero(reinterpret_cast<const std::uint32_t*>(&native.u)[1], "pathnode_tree_t.leaf padding");
			if (auto nodes = plain::read_array(native.u.s.nodes, native.u.s.nodeCount, "pathnode_tree_t.nodes", [](const auto& item) { return item; }))
			{
				data.nodes = std::move(*nodes);
			}

			return data;
		}

		for (std::size_t index = 0; index < 2; ++index)
		{
			if (native.u.child[index])
			{
				if (!plain::element_range::contains(native.u.child[index]))
				{
					plain::fail("pathnode_tree_t.child", "points outside the array of its tree");
				}

				data.children[index] = static_cast<std::int32_t>(native.u.child[index] - &native);
			}
		}

		return data;
	}

	void write_path_tree(pathnode_tree_t& native, const asset_data::path_tree& data, [[maybe_unused]] const asset_resolver& resolve,
		plain::context& memory)
	{
		native.axis = data.axis;
		native.dist = data.dist;
		if (data.axis < 0)
		{
			native.u.s.nodeCount = static_cast<std::int32_t>(data.nodes.size());
			if (!data.nodes.empty())
			{
				native.u.s.nodes = memory.manual_allocate<std::uint16_t>(plain::rounded(data.nodes.size() * sizeof(std::uint16_t)));
				std::ranges::copy(data.nodes, native.u.s.nodes);
			}

			return;
		}

		for (std::size_t index = 0; index < 2; ++index)
		{
			if (data.children[index])
			{
				native.u.child[index] = &native + *data.children[index];
			}
		}
	}

	std::size_t size_path_tree(const asset_data::path_tree& data)
	{
		return data.axis < 0 ? plain::sized(data.nodes.size() * sizeof(std::uint16_t)) : 0;
	}
}
