#pragma once

#include <core/assets/path_tree_data.hpp>
#include "resolver.hpp"
#include "plain.hpp"
#include "../structs.hpp"

// The tree of path nodes of a map, which the generated path data adapter (tools/gen_iw8_neutral.py, spec path_data.json) hands to
// code written by hand: what a node holds follows from the sign of its axis, which the structs do not state.
namespace zonetool::iw8::neutral
{
	asset_data::path_tree read_path_tree(const pathnode_tree_t& native);
	void write_path_tree(pathnode_tree_t& native, const asset_data::path_tree& data, const asset_resolver& resolve, plain::context& memory);
	std::size_t size_path_tree(const asset_data::path_tree& data);
}
