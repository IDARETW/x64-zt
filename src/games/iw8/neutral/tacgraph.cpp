// Emitted by tools/gen_iw8_neutral.py from tools/neutral_families/tacgraph.json: do not edit.
#include <std_include.hpp>
#include "tacgraph.hpp"
#include "plain.hpp"

namespace zonetool::iw8::neutral
{
	namespace
	{
		asset_data::tacgraph::tac_graph read_tac_graph(const TacticalGraphData& native);
		void write_tac_graph(TacticalGraphData& native, const asset_data::tacgraph::tac_graph& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_tac_graph(const asset_data::tacgraph::tac_graph& data);
		asset_data::tacgraph::tactical_graph read_tactical_graph(const TacticalGraph& native);
		void write_tactical_graph(TacticalGraph& native, const asset_data::tacgraph::tactical_graph& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_tactical_graph(const asset_data::tacgraph::tactical_graph& data);
		asset_data::tacgraph::tacpoint_t read_tacpoint_t(const tacpoint_t& native);
		void write_tacpoint_t(tacpoint_t& native, const asset_data::tacgraph::tacpoint_t& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_tacpoint_t(const asset_data::tacgraph::tacpoint_t& data);
		asset_data::tacgraph::bfx_area_handle read_bfx_area_handle(const bfx_AreaHandle& native);
		void write_bfx_area_handle(bfx_AreaHandle& native, const asset_data::tacgraph::bfx_area_handle& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_bfx_area_handle(const asset_data::tacgraph::bfx_area_handle& data);
		asset_data::tacgraph::tac_vis_graph_row read_tac_vis_graph_row(const TacVisGraphRow& native);
		void write_tac_vis_graph_row(TacVisGraphRow& native, const asset_data::tacgraph::tac_vis_graph_row& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_tac_vis_graph_row(const asset_data::tacgraph::tac_vis_graph_row& data);
		asset_data::tacgraph::tacpoint_search_node_t read_tacpoint_search_node_t(const tacpoint_search_node_t& native);
		void write_tacpoint_search_node_t(tacpoint_search_node_t& native, const asset_data::tacgraph::tacpoint_search_node_t& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_tacpoint_search_node_t(const asset_data::tacgraph::tacpoint_search_node_t& data);

		asset_data::tacgraph::tac_graph read_tac_graph(const TacticalGraphData& native)
		{
			plain::trace("read_tac_graph");
			asset_data::tacgraph::tac_graph data;
			data.m_name = plain::read_string(native.m_Name);
			data.m_tac_graphs = plain::read_array(native.m_TacGraphs, native.m_NumTacGraphs, "TacticalGraphData.m_TacGraphs", [&](const auto& item) { return read_tactical_graph(item); });
			return data;
		}

		void write_tac_graph(TacticalGraphData& native, const asset_data::tacgraph::tac_graph& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.m_Name = plain::write_string(memory, data.m_name);
			native.m_TacGraphs = plain::write_array<TacticalGraph>(data.m_tac_graphs, memory, [&](auto& out, const auto& in) { write_tactical_graph(out, in, resolve, memory); });
			native.m_NumTacGraphs = plain::count_of<std::int32_t>(data.m_tac_graphs);
		}

		std::size_t size_tac_graph([[maybe_unused]] const asset_data::tacgraph::tac_graph& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.m_name);
			if (data.m_tac_graphs)
			{
				size += plain::sized(std::max<std::size_t>(data.m_tac_graphs->size(), 1) * sizeof(TacticalGraph));
				for (const auto& item : *data.m_tac_graphs)
				{
					size += size_tactical_graph(item);
				}
			}
			return size;
		}

		asset_data::tacgraph::tacpoint_search_node_t read_tacpoint_search_node_t(const tacpoint_search_node_t& native)
		{
			plain::trace("read_tacpoint_search_node_t");
			asset_data::tacgraph::tacpoint_search_node_t data;
			data.m_mid_point = plain::read_scalars<float, 2>(&native.m_MidPoint);
			data.m_half_size = plain::read_scalars<float, 2>(&native.m_HalfSize);
			data.m_point_list_root = native.m_PointListRoot;
			data.m_child_nodes = plain::read_fixed_with<4, true>(native.m_ChildNodes, [&](const auto& item) { return plain::read_pointee(item, [&](const auto& pointee) { return read_tacpoint_search_node_t(pointee); }); });
			return data;
		}

		void write_tacpoint_search_node_t(tacpoint_search_node_t& native, const asset_data::tacgraph::tacpoint_search_node_t& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.m_MidPoint, data.m_mid_point);
			plain::write_scalars(&native.m_HalfSize, data.m_half_size);
			native.m_PointListRoot = data.m_point_list_root;
			if (data.m_child_nodes)
			{
				native.m_ChildNodes = memory.manual_allocate<tacpoint_search_node_t*>(plain::rounded(4 * sizeof(tacpoint_search_node_t*)));
				for (std::size_t i = 0; i < 4; ++i)
				{
					native.m_ChildNodes[i] = plain::write_pointee<tacpoint_search_node_t>((*data.m_child_nodes)[i], memory, [&](auto& pointee, const auto& value) { write_tacpoint_search_node_t(pointee, value, resolve, memory); });
				}
			}
		}

		std::size_t size_tacpoint_search_node_t([[maybe_unused]] const asset_data::tacgraph::tacpoint_search_node_t& data)
		{
			std::size_t size = 0;
			if (data.m_child_nodes)
			{
				size += plain::sized(4 * sizeof(tacpoint_search_node_t*));
			}
			return size;
		}

		asset_data::tacgraph::tactical_graph read_tactical_graph(const TacticalGraph& native)
		{
			plain::trace("read_tactical_graph");
			asset_data::tacgraph::tactical_graph data;
			data.m_points = plain::read_array(native.m_Points, native.m_NumPoints, "TacticalGraph.m_Points", [&](const auto& item) { return read_tacpoint_t(item); });
			data.m_num_vis_bytes = native.m_NumVisBytes;
			data.m_vis_graph = plain::read_array(native.m_VisGraph, ((-1) + native.m_NumPoints), "TacticalGraph.m_VisGraph", [&](const auto& item) { return read_tac_vis_graph_row(item); });
			data.m_search_root = read_tacpoint_search_node_t(native.m_SearchRoot);
			return data;
		}

		void write_tactical_graph(TacticalGraph& native, const asset_data::tacgraph::tactical_graph& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.m_Points = plain::write_array<tacpoint_t>(data.m_points, memory, [&](auto& out, const auto& in) { write_tacpoint_t(out, in, resolve, memory); });
			native.m_NumVisBytes = data.m_num_vis_bytes;
			native.m_VisGraph = plain::write_array<TacVisGraphRow>(data.m_vis_graph, memory, [&](auto& out, const auto& in) { write_tac_vis_graph_row(out, in, resolve, memory); });
			write_tacpoint_search_node_t(native.m_SearchRoot, data.m_search_root, resolve, memory);
			native.m_NumPoints = plain::count_of<std::uint16_t>(data.m_points);
		}

		std::size_t size_tactical_graph([[maybe_unused]] const asset_data::tacgraph::tactical_graph& data)
		{
			std::size_t size = 0;
			if (data.m_points)
			{
				size += plain::sized(std::max<std::size_t>(data.m_points->size(), 1) * sizeof(tacpoint_t));
				for (const auto& item : *data.m_points)
				{
					size += size_tacpoint_t(item);
				}
			}
			if (data.m_vis_graph)
			{
				size += plain::sized(std::max<std::size_t>(data.m_vis_graph->size(), 1) * sizeof(TacVisGraphRow));
				for (const auto& item : *data.m_vis_graph)
				{
					size += size_tac_vis_graph_row(item);
				}
			}
			size += size_tacpoint_search_node_t(data.m_search_root);
			return size;
		}

		asset_data::tacgraph::bfx_area_handle read_bfx_area_handle(const bfx_AreaHandle& native)
		{
			plain::trace("read_bfx_area_handle");
			asset_data::tacgraph::bfx_area_handle data;
			plain::require_zero(native.m_handleImpl, "bfx_AreaHandle.m_handleImpl");
			plain::require_zero(native.m_pSpace, "bfx_AreaHandle.m_pSpace");
			return data;
		}

		void write_bfx_area_handle(bfx_AreaHandle& native, const asset_data::tacgraph::bfx_area_handle& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
		}

		std::size_t size_bfx_area_handle([[maybe_unused]] const asset_data::tacgraph::bfx_area_handle& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::tacgraph::tacpoint_t read_tacpoint_t(const tacpoint_t& native)
		{
			plain::trace("read_tacpoint_t");
			asset_data::tacgraph::tacpoint_t data;
			data.m_pos = plain::read_scalars<float, 3>(&native.m_Pos);
			data.m_vertical_clearance = native.m_VerticalClearance;
			data.m_graph_idx = native.m_GraphIdx;
			data.m_type = native.m_Type;
			data.m_h_area = read_bfx_area_handle(native.m_hArea);
			data.m_search_next = native.m_SearchNext;
			data.m_search_prev = native.m_SearchPrev;
			data.m_padding = native.m_Padding;
			return data;
		}

		void write_tacpoint_t(tacpoint_t& native, const asset_data::tacgraph::tacpoint_t& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.m_Pos, data.m_pos);
			native.m_VerticalClearance = data.m_vertical_clearance;
			native.m_GraphIdx = data.m_graph_idx;
			native.m_Type = data.m_type;
			write_bfx_area_handle(native.m_hArea, data.m_h_area, resolve, memory);
			native.m_SearchNext = data.m_search_next;
			native.m_SearchPrev = data.m_search_prev;
			native.m_Padding = data.m_padding;
		}

		std::size_t size_tacpoint_t([[maybe_unused]] const asset_data::tacgraph::tacpoint_t& data)
		{
			std::size_t size = 0;
			size += size_bfx_area_handle(data.m_h_area);
			return size;
		}

		asset_data::tacgraph::tac_vis_graph_row read_tac_vis_graph_row(const TacVisGraphRow& native)
		{
			plain::trace("read_tac_vis_graph_row");
			asset_data::tacgraph::tac_vis_graph_row data;
			data.m_start = native.m_Start;
			data.m_end = native.m_End;
			data.m_vis = plain::read_array(native.m_Vis, ((static_cast<std::int64_t>(((native.m_End - native.m_Start) + ((static_cast<std::int64_t>((native.m_End - native.m_Start)) < 0 ? -1 : 0) & 7))) >> 3) + 1), "TacVisGraphRow.m_Vis", [&](const auto& item) { return item; });
			return data;
		}

		void write_tac_vis_graph_row(TacVisGraphRow& native, const asset_data::tacgraph::tac_vis_graph_row& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.m_Start = data.m_start;
			native.m_End = data.m_end;
			native.m_Vis = plain::write_array<std::uint8_t>(data.m_vis, memory, [&](auto& out, const auto& in) { out = in; });
		}

		std::size_t size_tac_vis_graph_row([[maybe_unused]] const asset_data::tacgraph::tac_vis_graph_row& data)
		{
			std::size_t size = 0;
			if (data.m_vis)
			{
				size += plain::sized(std::max<std::size_t>(data.m_vis->size(), 1) * sizeof(std::uint8_t));
			}
			return size;
		}

	}

	std::expected<asset_data::tacgraph::tac_graph, std::string> to_neutral(const TacticalGraphData& native)
	{
		try
		{
			auto data = read_tac_graph(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	TacticalGraphData* from_neutral(const asset_data::tacgraph::tac_graph& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<TacticalGraphData>();
		write_tac_graph(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::tacgraph::tac_graph& data)
	{
		return plain::rounded(sizeof(TacticalGraphData)) + size_tac_graph(data) + 64;
	}

}
