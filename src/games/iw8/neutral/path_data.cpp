// Emitted by tools/gen_iw8_neutral.py from tools/neutral_families/path_data.json: do not edit.
#include <std_include.hpp>
#include "path_data.hpp"
#include "plain.hpp"
#include "path_tree.hpp"

namespace zonetool::iw8::neutral
{
	namespace
	{
		asset_data::path_data::path_data read_path_data(const PathData& native);
		void write_path_data(PathData& native, const asset_data::path_data::path_data& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_path_data(const asset_data::path_data::path_data& data);
		asset_data::path_data::pathnode_t read_pathnode_t(const pathnode_t& native);
		void write_pathnode_t(pathnode_t& native, const asset_data::path_data::pathnode_t& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_pathnode_t(const asset_data::path_data::pathnode_t& data);
		asset_data::path_data::pathnode_constant_t read_pathnode_constant_t(const pathnode_constant_t& native);
		void write_pathnode_constant_t(pathnode_constant_t& native, const asset_data::path_data::pathnode_constant_t& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_pathnode_constant_t(const asset_data::path_data::pathnode_constant_t& data);
		asset_data::path_data::tacpoint_ref_t read_tacpoint_ref_t(const tacpoint_ref_t& native);
		void write_tacpoint_ref_t(tacpoint_ref_t& native, const asset_data::path_data::tacpoint_ref_t& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_tacpoint_ref_t(const asset_data::path_data::tacpoint_ref_t& data);
		asset_data::path_data::pathlink_s read_pathlink_s(const pathlink_s& native);
		void write_pathlink_s(pathlink_s& native, const asset_data::path_data::pathlink_s& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_pathlink_s(const asset_data::path_data::pathlink_s& data);
		asset_data::path_data::pathnode_dynamic_t read_pathnode_dynamic_t(const pathnode_dynamic_t& native);
		void write_pathnode_dynamic_t(pathnode_dynamic_t& native, const asset_data::path_data::pathnode_dynamic_t& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_pathnode_dynamic_t(const asset_data::path_data::pathnode_dynamic_t& data);
		asset_data::path_data::sentient_handle read_sentient_handle(const SentientHandle& native);
		void write_sentient_handle(SentientHandle& native, const asset_data::path_data::sentient_handle& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_sentient_handle(const asset_data::path_data::sentient_handle& data);
		asset_data::path_data::pathnode_transient_t read_pathnode_transient_t(const pathnode_transient_t& native);
		void write_pathnode_transient_t(pathnode_transient_t& native, const asset_data::path_data::pathnode_transient_t& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_pathnode_transient_t(const asset_data::path_data::pathnode_transient_t& data);
		asset_data::path_data::path_dynamic_node_group read_path_dynamic_node_group(const PathDynamicNodeGroup& native);
		void write_path_dynamic_node_group(PathDynamicNodeGroup& native, const asset_data::path_data::path_dynamic_node_group& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_path_dynamic_node_group(const asset_data::path_data::path_dynamic_node_group& data);

		asset_data::path_data::path_data read_path_data(const PathData& native)
		{
			plain::trace("read_path_data");
			asset_data::path_data::path_data data;
			data.name = plain::read_string(native.name);
			data.nodes = plain::read_array(native.nodes, native.nodeCount, "PathData.nodes", [&](const auto& item) { return read_pathnode_t(item); });
			data.parent_index_resolved = native.parentIndexResolved;
			data.version = native.version;
			data.path_vis = plain::read_array(native.pathVis, native.visBytes, "PathData.pathVis", [&](const auto& item) { return item; });
			data.node_tree = [&] { plain::element_range range(native.nodeTree, native.nodeTreeCount * sizeof(pathnode_tree_t)); return plain::read_array(native.nodeTree, native.nodeTreeCount, "PathData.nodeTree", [&](const auto& item) { return read_path_tree(item); }); }();
			data.dynamic_node_groups = plain::read_array(native.dynamicNodeGroups, native.dynamicNodeGroupCount, "PathData.dynamicNodeGroups", [&](const auto& item) { return read_path_dynamic_node_group(item); });
			data.use_path_extra_data = native.usePathExtraData;
			data.path_exposure = plain::read_array(native.pathExposure, native.exposureBytes, "PathData.pathExposure", [&](const auto& item) { return item; });
			data.path_no_peek_vis = plain::read_array(native.pathNoPeekVis, native.noPeekVisBytes, "PathData.pathNoPeekVis", [&](const auto& item) { return item; });
			data.zone_count = native.zoneCount;
			data.path_zones = plain::read_array(native.pathZones, native.zonesBytes, "PathData.pathZones", [&](const auto& item) { return item; });
			data.fixed_node_count = native.fixedNodeCount;
			data.max_dynamic_spawned_node_count = native.maxDynamicSpawnedNodeCount;
			data.path_dyn_states = plain::read_array(native.pathDynStates, native.dynStatesBytes, "PathData.pathDynStates", [&](const auto& item) { return item; });
			data.vis_dist_data_size = native.visDistDataSize;
			return data;
		}

		void write_path_data(PathData& native, const asset_data::path_data::path_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.nodes = plain::write_array<pathnode_t>(data.nodes, memory, [&](auto& out, const auto& in) { write_pathnode_t(out, in, resolve, memory); });
			native.parentIndexResolved = data.parent_index_resolved;
			native.version = data.version;
			native.pathVis = plain::write_array<std::uint8_t>(data.path_vis, memory, [&](auto& out, const auto& in) { out = in; });
			native.nodeTree = plain::write_array<pathnode_tree_t>(data.node_tree, memory, [&](auto& out, const auto& in) { write_path_tree(out, in, resolve, memory); });
			native.dynamicNodeGroups = plain::write_array<PathDynamicNodeGroup>(data.dynamic_node_groups, memory, [&](auto& out, const auto& in) { write_path_dynamic_node_group(out, in, resolve, memory); });
			native.usePathExtraData = data.use_path_extra_data;
			native.pathExposure = plain::write_array<std::uint8_t>(data.path_exposure, memory, [&](auto& out, const auto& in) { out = in; });
			native.pathNoPeekVis = plain::write_array<std::uint8_t>(data.path_no_peek_vis, memory, [&](auto& out, const auto& in) { out = in; });
			native.zoneCount = data.zone_count;
			native.pathZones = plain::write_array<std::uint8_t>(data.path_zones, memory, [&](auto& out, const auto& in) { out = in; });
			native.fixedNodeCount = data.fixed_node_count;
			native.maxDynamicSpawnedNodeCount = data.max_dynamic_spawned_node_count;
			native.pathDynStates = plain::write_array<std::uint8_t>(data.path_dyn_states, memory, [&](auto& out, const auto& in) { out = in; });
			native.visDistDataSize = data.vis_dist_data_size;
			native.nodeCount = plain::count_of<std::uint32_t>(data.nodes);
			native.visBytes = plain::count_of<std::int32_t>(data.path_vis);
			native.nodeTreeCount = plain::count_of<std::int32_t>(data.node_tree);
			native.dynamicNodeGroupCount = plain::count_of<std::int32_t>(data.dynamic_node_groups);
			native.exposureBytes = plain::count_of<std::int32_t>(data.path_exposure);
			native.noPeekVisBytes = plain::count_of<std::int32_t>(data.path_no_peek_vis);
			native.zonesBytes = plain::count_of<std::int32_t>(data.path_zones);
			native.dynStatesBytes = plain::count_of<std::int32_t>(data.path_dyn_states);
		}

		std::size_t size_path_data([[maybe_unused]] const asset_data::path_data::path_data& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.nodes)
			{
				size += plain::sized(std::max<std::size_t>(data.nodes->size(), 1) * sizeof(pathnode_t));
				for (const auto& item : *data.nodes)
				{
					size += size_pathnode_t(item);
				}
			}
			if (data.path_vis)
			{
				size += plain::sized(std::max<std::size_t>(data.path_vis->size(), 1) * sizeof(std::uint8_t));
			}
			if (data.node_tree)
			{
				size += plain::sized(std::max<std::size_t>(data.node_tree->size(), 1) * sizeof(pathnode_tree_t));
				for (const auto& item : *data.node_tree)
				{
					size += size_path_tree(item);
				}
			}
			if (data.dynamic_node_groups)
			{
				size += plain::sized(std::max<std::size_t>(data.dynamic_node_groups->size(), 1) * sizeof(PathDynamicNodeGroup));
				for (const auto& item : *data.dynamic_node_groups)
				{
					size += size_path_dynamic_node_group(item);
				}
			}
			if (data.path_exposure)
			{
				size += plain::sized(std::max<std::size_t>(data.path_exposure->size(), 1) * sizeof(std::uint8_t));
			}
			if (data.path_no_peek_vis)
			{
				size += plain::sized(std::max<std::size_t>(data.path_no_peek_vis->size(), 1) * sizeof(std::uint8_t));
			}
			if (data.path_zones)
			{
				size += plain::sized(std::max<std::size_t>(data.path_zones->size(), 1) * sizeof(std::uint8_t));
			}
			if (data.path_dyn_states)
			{
				size += plain::sized(std::max<std::size_t>(data.path_dyn_states->size(), 1) * sizeof(std::uint8_t));
			}
			return size;
		}

		asset_data::path_data::tacpoint_ref_t read_tacpoint_ref_t(const tacpoint_ref_t& native)
		{
			plain::trace("read_tacpoint_ref_t");
			asset_data::path_data::tacpoint_ref_t data;
			data.m_point_id = native.m_PointID;
			data.m_graph_id_plus_one = native.m_GraphIDPlusOne;
			return data;
		}

		void write_tacpoint_ref_t(tacpoint_ref_t& native, const asset_data::path_data::tacpoint_ref_t& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.m_PointID = data.m_point_id;
			native.m_GraphIDPlusOne = data.m_graph_id_plus_one;
		}

		std::size_t size_tacpoint_ref_t([[maybe_unused]] const asset_data::path_data::tacpoint_ref_t& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::path_data::pathnode_constant_t read_pathnode_constant_t(const pathnode_constant_t& native)
		{
			plain::trace("read_pathnode_constant_t");
			asset_data::path_data::pathnode_constant_t data;
			data.spawnflags = native.spawnflags;
			data.targetname = plain::read_script_string(native.targetname);
			data.script_link_name = plain::read_script_string(native.script_linkName);
			data.script_noteworthy = plain::read_script_string(native.script_noteworthy);
			data.target = plain::read_script_string(native.target);
			data.animscript = plain::read_script_string(native.animscript);
			data.animscriptfunc = native.animscriptfunc;
			data.interactable = plain::read_script_string(native.interactable);
			data.v_local_origin = plain::read_scalars<float, 3>(&native.vLocalOrigin);
			data.u9 = plain::read_block<std::array<std::uint8_t, 12>>(&native.___u9);
			data.parent = plain::read_scalars<std::uint32_t, 1>(&native.parent)[0];
			data.min_use_dist_sq_or_error = plain::read_scalars<std::uint32_t, 1>(&native.___u11)[0];
			data.w_overlap_node = std::to_array(native.wOverlapNode);
			data.tacpoint_ref = read_tacpoint_ref_t(native.tacpointRef);
			data.links = plain::read_array(native.Links, native.totalLinkCount, "pathnode_constant_t.Links", [&](const auto& item) { return read_pathlink_s(item); });
			data.type = native.type;
			return data;
		}

		void write_pathnode_constant_t(pathnode_constant_t& native, const asset_data::path_data::pathnode_constant_t& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.spawnflags = data.spawnflags;
			native.targetname = static_cast<scr_string_t>(plain::write_script_string(data.targetname));
			native.script_linkName = static_cast<scr_string_t>(plain::write_script_string(data.script_link_name));
			native.script_noteworthy = static_cast<scr_string_t>(plain::write_script_string(data.script_noteworthy));
			native.target = static_cast<scr_string_t>(plain::write_script_string(data.target));
			native.animscript = static_cast<scr_string_t>(plain::write_script_string(data.animscript));
			native.animscriptfunc = data.animscriptfunc;
			native.interactable = static_cast<scr_string_t>(plain::write_script_string(data.interactable));
			plain::write_scalars(&native.vLocalOrigin, data.v_local_origin);
			plain::write_block(&native.___u9, data.u9);
			plain::write_scalars(&native.parent, std::array<std::uint32_t, 1>{data.parent});
			plain::write_scalars(&native.___u11, std::array<std::uint32_t, 1>{data.min_use_dist_sq_or_error});
			std::ranges::copy(data.w_overlap_node, native.wOverlapNode);
			write_tacpoint_ref_t(native.tacpointRef, data.tacpoint_ref, resolve, memory);
			native.Links = plain::write_array<pathlink_s>(data.links, memory, [&](auto& out, const auto& in) { write_pathlink_s(out, in, resolve, memory); });
			native.type = data.type;
			native.totalLinkCount = plain::count_of<std::uint16_t>(data.links);
		}

		std::size_t size_pathnode_constant_t([[maybe_unused]] const asset_data::path_data::pathnode_constant_t& data)
		{
			std::size_t size = 0;
			size += size_tacpoint_ref_t(data.tacpoint_ref);
			if (data.links)
			{
				size += plain::sized(std::max<std::size_t>(data.links->size(), 1) * sizeof(pathlink_s));
				for (const auto& item : *data.links)
				{
					size += size_pathlink_s(item);
				}
			}
			return size;
		}

		asset_data::path_data::sentient_handle read_sentient_handle(const SentientHandle& native)
		{
			plain::trace("read_sentient_handle");
			asset_data::path_data::sentient_handle data;
			data.number = native.number;
			data.info_index = native.infoIndex;
			return data;
		}

		void write_sentient_handle(SentientHandle& native, const asset_data::path_data::sentient_handle& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.number = data.number;
			native.infoIndex = data.info_index;
		}

		std::size_t size_sentient_handle([[maybe_unused]] const asset_data::path_data::sentient_handle& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::path_data::pathnode_dynamic_t read_pathnode_dynamic_t(const pathnode_dynamic_t& native)
		{
			plain::trace("read_pathnode_dynamic_t");
			asset_data::path_data::pathnode_dynamic_t data;
			data.p_owners = [&] { std::array<asset_data::path_data::sentient_handle, 2> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_sentient_handle(native.pOwners[i]); } return out; }();
			data.i_free_time = std::to_array(native.iFreeTime);
			data.i_valid_time = std::to_array(native.iValidTime);
			data.w_link_count = native.wLinkCount;
			data.turret_ent_number_or_cover_multi_type = plain::read_scalars<std::uint16_t, 1>(&native.___u4)[0];
			data.b_inactive = native.bInactive;
			data.w_overlap_count = std::to_array(native.wOverlapCount);
			data.u7 = plain::read_block<std::array<std::uint8_t, 16>>(&native.___u7);
			return data;
		}

		void write_pathnode_dynamic_t(pathnode_dynamic_t& native, const asset_data::path_data::pathnode_dynamic_t& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			for (std::size_t i = 0; i < data.p_owners.size(); ++i)
			{
				write_sentient_handle(native.pOwners[i], data.p_owners[i], resolve, memory);
			}
			std::ranges::copy(data.i_free_time, native.iFreeTime);
			std::ranges::copy(data.i_valid_time, native.iValidTime);
			native.wLinkCount = data.w_link_count;
			plain::write_scalars(&native.___u4, std::array<std::uint16_t, 1>{data.turret_ent_number_or_cover_multi_type});
			native.bInactive = data.b_inactive;
			std::ranges::copy(data.w_overlap_count, native.wOverlapCount);
			plain::write_block(&native.___u7, data.u7);
		}

		std::size_t size_pathnode_dynamic_t([[maybe_unused]] const asset_data::path_data::pathnode_dynamic_t& data)
		{
			std::size_t size = 0;
			for (const auto& item : data.p_owners)
			{
				size += size_sentient_handle(item);
			}
			return size;
		}

		asset_data::path_data::pathnode_transient_t read_pathnode_transient_t(const pathnode_transient_t& native)
		{
			plain::trace("read_pathnode_transient_t");
			asset_data::path_data::pathnode_transient_t data;
			data.i_search_frame = native.iSearchFrame;
			plain::require_zero(native.pNextOpen, "pathnode_transient_t.pNextOpen");
			plain::require_zero(native.pPrevOpen, "pathnode_transient_t.pPrevOpen");
			plain::require_zero(native.pParent, "pathnode_transient_t.pParent");
			data.f_cost = native.fCost;
			data.f_heuristic = native.fHeuristic;
			data.node_cost_or_link_index = plain::read_scalars<std::uint32_t, 1>(&native.___u6)[0];
			return data;
		}

		void write_pathnode_transient_t(pathnode_transient_t& native, const asset_data::path_data::pathnode_transient_t& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.iSearchFrame = data.i_search_frame;
			native.fCost = data.f_cost;
			native.fHeuristic = data.f_heuristic;
			plain::write_scalars(&native.___u6, std::array<std::uint32_t, 1>{data.node_cost_or_link_index});
		}

		std::size_t size_pathnode_transient_t([[maybe_unused]] const asset_data::path_data::pathnode_transient_t& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::path_data::pathnode_t read_pathnode_t(const pathnode_t& native)
		{
			plain::trace("read_pathnode_t");
			asset_data::path_data::pathnode_t data;
			data.constant = read_pathnode_constant_t(native.constant);
			data.dynamic = read_pathnode_dynamic_t(native.dynamic);
			data.transient = read_pathnode_transient_t(native.transient);
			return data;
		}

		void write_pathnode_t(pathnode_t& native, const asset_data::path_data::pathnode_t& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_pathnode_constant_t(native.constant, data.constant, resolve, memory);
			write_pathnode_dynamic_t(native.dynamic, data.dynamic, resolve, memory);
			write_pathnode_transient_t(native.transient, data.transient, resolve, memory);
		}

		std::size_t size_pathnode_t([[maybe_unused]] const asset_data::path_data::pathnode_t& data)
		{
			std::size_t size = 0;
			size += size_pathnode_constant_t(data.constant);
			size += size_pathnode_dynamic_t(data.dynamic);
			size += size_pathnode_transient_t(data.transient);
			return size;
		}

		asset_data::path_data::pathlink_s read_pathlink_s(const pathlink_s& native)
		{
			plain::trace("read_pathlink_s");
			asset_data::path_data::pathlink_s data;
			data.f_dist = native.fDist;
			data.node_num = native.nodeNum;
			data.disconnect_count = native.disconnectCount;
			data.negotiation_link = native.negotiationLink;
			data.flags = native.flags;
			data.ub_bad_place_count = std::to_array(native.ubBadPlaceCount);
			return data;
		}

		void write_pathlink_s(pathlink_s& native, const asset_data::path_data::pathlink_s& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.fDist = data.f_dist;
			native.nodeNum = data.node_num;
			native.disconnectCount = data.disconnect_count;
			native.negotiationLink = data.negotiation_link;
			native.flags = data.flags;
			std::ranges::copy(data.ub_bad_place_count, native.ubBadPlaceCount);
		}

		std::size_t size_pathlink_s([[maybe_unused]] const asset_data::path_data::pathlink_s& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::path_data::path_dynamic_node_group read_path_dynamic_node_group(const PathDynamicNodeGroup& native)
		{
			plain::trace("read_path_dynamic_node_group");
			asset_data::path_data::path_dynamic_node_group data;
			data.parent_index = native.parentIndex;
			data.node_tree = [&] { plain::element_range range(native.nodeTree, native.nodeTreeCount * sizeof(pathnode_tree_t)); return plain::read_array(native.nodeTree, native.nodeTreeCount, "PathDynamicNodeGroup.nodeTree", [&](const auto& item) { return read_path_tree(item); }); }();
			return data;
		}

		void write_path_dynamic_node_group(PathDynamicNodeGroup& native, const asset_data::path_data::path_dynamic_node_group& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.parentIndex = data.parent_index;
			native.nodeTree = plain::write_array<pathnode_tree_t>(data.node_tree, memory, [&](auto& out, const auto& in) { write_path_tree(out, in, resolve, memory); });
			native.nodeTreeCount = plain::count_of<std::int32_t>(data.node_tree);
		}

		std::size_t size_path_dynamic_node_group([[maybe_unused]] const asset_data::path_data::path_dynamic_node_group& data)
		{
			std::size_t size = 0;
			if (data.node_tree)
			{
				size += plain::sized(std::max<std::size_t>(data.node_tree->size(), 1) * sizeof(pathnode_tree_t));
				for (const auto& item : *data.node_tree)
				{
					size += size_path_tree(item);
				}
			}
			return size;
		}

	}

	std::expected<asset_data::path_data::path_data, std::string> to_neutral(const PathData& native)
	{
		try
		{
			auto data = read_path_data(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	PathData* from_neutral(const asset_data::path_data::path_data& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<PathData>();
		write_path_data(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::path_data::path_data& data)
	{
		return plain::rounded(sizeof(PathData)) + size_path_data(data) + 64;
	}

}
