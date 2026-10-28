#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <stdexcept>
#include <vector>

namespace zonetool::asset_data
{
	// A node of the tree that divides the path nodes of a map by space. The nodes of one tree are an array; an inner node splits
	// along an axis at a distance and points at two children of that array (each may be absent), given as the distance in
	// nodes from this one; a leaf (a negative axis) lists the path nodes it holds.
	struct path_tree
	{
		std::int32_t axis{};
		float dist{};
		std::vector<std::uint16_t> nodes{};
		std::array<std::optional<std::int32_t>, 2> children{};

		bool operator==(const path_tree&) const = default;
	};

	inline void validate(const path_tree& data)
	{
		if (data.axis < 0)
		{
			if (data.children[0] || data.children[1])
			{
				throw std::runtime_error("path_tree is a leaf and has children");
			}
		}
		else if (!data.nodes.empty())
		{
			throw std::runtime_error("path_tree is an inner node and lists path nodes");
		}
	}
}
