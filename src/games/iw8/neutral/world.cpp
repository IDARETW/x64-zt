// Emitted by tools/gen_iw8_neutral.py from tools/neutral_families/world.json: do not edit.
#include <std_include.hpp>
#include "world.hpp"
#include "plain.hpp"

namespace zonetool::iw8::neutral
{
	namespace
	{
		asset_data::com_primary_light read_com_primary_light(const ComPrimaryLight& native)
		{
			asset_data::com_primary_light data;
			data.spawn_flags = native.spawnFlags;
			data.type = native.type;
			data.can_use_shadow_map = native.canUseShadowMap;
			data.needs_dynamic_shadows = native.needsDynamicShadows;
			data.is_volumetric = native.isVolumetric;
			data.transient_zone_list = native.transientZoneList;
			data.entity_id = native.entityId;
			data.intensity = native.intensity;
			data.uv_intensity = native.uvIntensity;
			data.ir_intensity = native.irIntensity;
			data.heat_intensity = native.heatIntensity;
			data.color_linear_srgb = plain::read_scalars<float, 3>(&native.colorLinearSrgb);
			data.dir = plain::read_scalars<float, 3>(&native.dir);
			data.up = plain::read_scalars<float, 3>(&native.up);
			data.origin = plain::read_scalars<float, 3>(&native.origin);
			data.radius = native.radius;
			data.fade_offset = plain::read_scalars<float, 2>(&native.fadeOffset);
			data.bulb_radius = native.bulbRadius;
			data.bulb_length = plain::read_scalars<float, 3>(&native.bulbLength);
			data.cos_half_fov_outer = native.cosHalfFovOuter;
			data.cos_half_fov_inner = native.cosHalfFovInner;
			data.shadow_near_plane_bias = native.shadowNearPlaneBias;
			data.shadow_softness = native.shadowSoftness;
			data.shadow_bias = native.shadowBias;
			data.shadow_area = native.shadowArea;
			data.distance_falloff = native.distanceFalloff;
			data.cos_half_fov_collimation = native.cosHalfFovCollimation;
			data.rotation_limit = native.rotationLimit;
			data.translation_limit = native.translationLimit;
			data.def_name = plain::read_string(native.defName);
			return data;
		}

		void write_com_primary_light(ComPrimaryLight& native, const asset_data::com_primary_light& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.spawnFlags = data.spawn_flags;
			native.type = data.type;
			native.canUseShadowMap = data.can_use_shadow_map;
			native.needsDynamicShadows = data.needs_dynamic_shadows;
			native.isVolumetric = data.is_volumetric;
			native.transientZoneList = data.transient_zone_list;
			native.entityId = data.entity_id;
			native.intensity = data.intensity;
			native.uvIntensity = data.uv_intensity;
			native.irIntensity = data.ir_intensity;
			native.heatIntensity = data.heat_intensity;
			plain::write_scalars(&native.colorLinearSrgb, data.color_linear_srgb);
			plain::write_scalars(&native.dir, data.dir);
			plain::write_scalars(&native.up, data.up);
			plain::write_scalars(&native.origin, data.origin);
			native.radius = data.radius;
			plain::write_scalars(&native.fadeOffset, data.fade_offset);
			native.bulbRadius = data.bulb_radius;
			plain::write_scalars(&native.bulbLength, data.bulb_length);
			native.cosHalfFovOuter = data.cos_half_fov_outer;
			native.cosHalfFovInner = data.cos_half_fov_inner;
			native.shadowNearPlaneBias = data.shadow_near_plane_bias;
			native.shadowSoftness = data.shadow_softness;
			native.shadowBias = data.shadow_bias;
			native.shadowArea = data.shadow_area;
			native.distanceFalloff = data.distance_falloff;
			native.cosHalfFovCollimation = data.cos_half_fov_collimation;
			native.rotationLimit = data.rotation_limit;
			native.translationLimit = data.translation_limit;
			native.defName = plain::write_string(memory, data.def_name);
		}

		std::size_t size_com_primary_light([[maybe_unused]] const asset_data::com_primary_light& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.def_name);
			return size;
		}

		asset_data::com_change_list_info read_com_change_list_info(const ComChangeListInfo& native)
		{
			asset_data::com_change_list_info data;
			data.change_list_number = native.changeListNumber;
			data.time = native.time;
			data.user_name = plain::read_string(native.userName);
			return data;
		}

		void write_com_change_list_info(ComChangeListInfo& native, const asset_data::com_change_list_info& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.changeListNumber = data.change_list_number;
			native.time = data.time;
			native.userName = plain::write_string(memory, data.user_name);
		}

		std::size_t size_com_change_list_info([[maybe_unused]] const asset_data::com_change_list_info& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.user_name);
			return size;
		}

		asset_data::bitarray_384 read_bitarray_384(const bitarray_384& native)
		{
			asset_data::bitarray_384 data;
			data.array = std::to_array(native.array);
			return data;
		}

		void write_bitarray_384(bitarray_384& native, const asset_data::bitarray_384& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			std::ranges::copy(data.array, native.array);
		}

		std::size_t size_bitarray_384([[maybe_unused]] const asset_data::bitarray_384& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::com_world read_com_world(const ComWorld& native)
		{
			asset_data::com_world data;
			data.name = plain::read_string(native.name);
			data.is_in_use = native.isInUse;
			data.use_forward_plus = native.useForwardPlus;
			data.bake_quality = native.bakeQuality;
			data.proxy_lod_quality = native.proxyLODQuality;
			data.tr_vis_radii = std::to_array(native.trVisRadii);
			data.tr_vis_facing_dist_add = native.trVisFacingDistAdd;
			data.primary_lights = plain::read_array(native.primaryLights, native.primaryLightCount, "ComWorld.primaryLights", [&](const auto& item) { return read_com_primary_light(item); });
			data.scriptable_primary_light_count = native.scriptablePrimaryLightCount;
			data.first_scriptable_primary_light = native.firstScriptablePrimaryLight;
			data.transient_table = plain::read_array(native.transientTable, native.transientTableSize, "ComWorld.transientTable", [&](const auto& item) { return item; });
			data.change_list_info = read_com_change_list_info(native.changeListInfo);
			data.umbra_gate_names = plain::read_array(native.umbraGateNames, native.numUmbraGates, "ComWorld.umbraGateNames", [&](const auto& item) { return plain::read_string(item); });
			data.umbra_gate_initial_states = read_bitarray_384(native.umbraGateInitialStates);
			return data;
		}

		void write_com_world(ComWorld& native, const asset_data::com_world& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.isInUse = data.is_in_use;
			native.useForwardPlus = data.use_forward_plus;
			native.bakeQuality = data.bake_quality;
			native.proxyLODQuality = data.proxy_lod_quality;
			std::ranges::copy(data.tr_vis_radii, native.trVisRadii);
			native.trVisFacingDistAdd = data.tr_vis_facing_dist_add;
			native.primaryLights = plain::write_array<ComPrimaryLight>(data.primary_lights, memory, [&](auto& out, const auto& in) { write_com_primary_light(out, in, resolve, memory); });
			native.scriptablePrimaryLightCount = data.scriptable_primary_light_count;
			native.firstScriptablePrimaryLight = data.first_scriptable_primary_light;
			native.transientTable = plain::write_array<std::uint16_t>(data.transient_table, memory, [&](auto& out, const auto& in) { out = in; });
			write_com_change_list_info(native.changeListInfo, data.change_list_info, resolve, memory);
			native.umbraGateNames = plain::write_array<const char*>(data.umbra_gate_names, memory, [&](auto& out, const auto& in) { out = plain::write_string(memory, in); });
			write_bitarray_384(native.umbraGateInitialStates, data.umbra_gate_initial_states, resolve, memory);
			native.primaryLightCount = plain::count_of<std::uint32_t>(data.primary_lights);
			native.transientTableSize = plain::count_of<std::uint32_t>(data.transient_table);
			native.numUmbraGates = plain::count_of<std::uint32_t>(data.umbra_gate_names);
		}

		std::size_t size_com_world([[maybe_unused]] const asset_data::com_world& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.primary_lights)
			{
				size += plain::rounded(std::max<std::size_t>(data.primary_lights->size(), 1) * sizeof(ComPrimaryLight));
				for (const auto& item : *data.primary_lights)
				{
					size += size_com_primary_light(item);
				}
			}
			if (data.transient_table)
			{
				size += plain::rounded(std::max<std::size_t>(data.transient_table->size(), 1) * sizeof(std::uint16_t));
			}
			size += size_com_change_list_info(data.change_list_info);
			if (data.umbra_gate_names)
			{
				size += plain::rounded(std::max<std::size_t>(data.umbra_gate_names->size(), 1) * sizeof(const char*));
				for (const auto& item : *data.umbra_gate_names)
				{
					size += plain::string_size(item);
				}
			}
			size += size_bitarray_384(data.umbra_gate_initial_states);
			return size;
		}

		asset_data::glass_piece read_glass_piece(const G_GlassPiece& native)
		{
			asset_data::glass_piece data;
			data.damage_taken = native.damageTaken;
			data.collapse_time = native.collapseTime;
			data.last_state_change_time = native.lastStateChangeTime;
			data.impact_dir = native.impactDir;
			data.impact_pos = std::to_array(native.impactPos);
			return data;
		}

		void write_glass_piece(G_GlassPiece& native, const asset_data::glass_piece& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.damageTaken = data.damage_taken;
			native.collapseTime = data.collapse_time;
			native.lastStateChangeTime = data.last_state_change_time;
			native.impactDir = data.impact_dir;
			std::ranges::copy(data.impact_pos, native.impactPos);
		}

		std::size_t size_glass_piece([[maybe_unused]] const asset_data::glass_piece& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::glass_name read_glass_name(const G_GlassName& native)
		{
			asset_data::glass_name data;
			data.name_str = plain::read_string(native.nameStr);
			data.name = plain::read_script_string(native.name);
			data.piece_indices = plain::read_array(native.pieceIndices, native.pieceCount, "G_GlassName.pieceIndices", [&](const auto& item) { return item; });
			return data;
		}

		void write_glass_name(G_GlassName& native, const asset_data::glass_name& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.nameStr = plain::write_string(memory, data.name_str);
			native.name = static_cast<scr_string_t>(plain::write_script_string(data.name));
			native.pieceIndices = plain::write_array<std::uint16_t>(data.piece_indices, memory, [&](auto& out, const auto& in) { out = in; });
			native.pieceCount = plain::count_of<std::uint16_t>(data.piece_indices);
		}

		std::size_t size_glass_name([[maybe_unused]] const asset_data::glass_name& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name_str);
			if (data.piece_indices)
			{
				size += plain::rounded(std::max<std::size_t>(data.piece_indices->size(), 1) * sizeof(std::uint16_t));
			}
			return size;
		}

		asset_data::spatial_partition_tree read_spatial_partition_tree(const SpatialPartition_Tree& native)
		{
			asset_data::spatial_partition_tree data;
			data.tree = plain::read_array(native.tree, native.treeSize, "SpatialPartition_Tree.tree", [&](const auto& item) { return item; });
			data.always_list = plain::read_array(native.alwaysList, native.alwaysListLength, "SpatialPartition_Tree.alwaysList", [&](const auto& item) { return item; });
			return data;
		}

		void write_spatial_partition_tree(SpatialPartition_Tree& native, const asset_data::spatial_partition_tree& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.tree = plain::write_array<std::uint8_t>(data.tree, memory, [&](auto& out, const auto& in) { out = in; });
			native.alwaysList = plain::write_array<std::uint32_t>(data.always_list, memory, [&](auto& out, const auto& in) { out = in; });
			native.treeSize = plain::count_of<std::uint32_t>(data.tree);
			native.alwaysListLength = plain::count_of<std::uint32_t>(data.always_list);
		}

		std::size_t size_spatial_partition_tree([[maybe_unused]] const asset_data::spatial_partition_tree& data)
		{
			std::size_t size = 0;
			if (data.tree)
			{
				size += plain::rounded(std::max<std::size_t>(data.tree->size(), 1) * sizeof(std::uint8_t));
			}
			if (data.always_list)
			{
				size += plain::rounded(std::max<std::size_t>(data.always_list->size(), 1) * sizeof(std::uint32_t));
			}
			return size;
		}

		asset_data::glass_data read_glass_data(const G_GlassData& native)
		{
			asset_data::glass_data data;
			data.glass_pieces = plain::read_array(native.glassPieces, native.pieceCount, "G_GlassData.glassPieces", [&](const auto& item) { return read_glass_piece(item); });
			data.damage_to_weaken = native.damageToWeaken;
			data.damage_to_destroy = native.damageToDestroy;
			data.glass_names = plain::read_array(native.glassNames, native.glassNameCount, "G_GlassData.glassNames", [&](const auto& item) { return read_glass_name(item); });
			data.spatial_tree = plain::read_single(native.spatialTree, [&](const auto& item) { return read_spatial_partition_tree(item); });
			return data;
		}

		void write_glass_data(G_GlassData& native, const asset_data::glass_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.glassPieces = plain::write_array<G_GlassPiece>(data.glass_pieces, memory, [&](auto& out, const auto& in) { write_glass_piece(out, in, resolve, memory); });
			native.damageToWeaken = data.damage_to_weaken;
			native.damageToDestroy = data.damage_to_destroy;
			native.glassNames = plain::write_array<G_GlassName>(data.glass_names, memory, [&](auto& out, const auto& in) { write_glass_name(out, in, resolve, memory); });
			native.spatialTree = plain::write_single<SpatialPartition_Tree>(data.spatial_tree, memory, [&](auto& out, const auto& in) { write_spatial_partition_tree(out, in, resolve, memory); });
			native.pieceCount = plain::count_of<std::uint32_t>(data.glass_pieces);
			native.glassNameCount = plain::count_of<std::uint32_t>(data.glass_names);
		}

		std::size_t size_glass_data([[maybe_unused]] const asset_data::glass_data& data)
		{
			std::size_t size = 0;
			if (data.glass_pieces)
			{
				size += plain::rounded(std::max<std::size_t>(data.glass_pieces->size(), 1) * sizeof(G_GlassPiece));
				for (const auto& item : *data.glass_pieces)
				{
					size += size_glass_piece(item);
				}
			}
			if (data.glass_names)
			{
				size += plain::rounded(std::max<std::size_t>(data.glass_names->size(), 1) * sizeof(G_GlassName));
				for (const auto& item : *data.glass_names)
				{
					size += size_glass_name(item);
				}
			}
			if (data.spatial_tree)
			{
				size += plain::rounded(sizeof(SpatialPartition_Tree));
				size += size_spatial_partition_tree(*data.spatial_tree);
			}
			return size;
		}

		asset_data::glass_world read_glass_world(const GlassWorld& native)
		{
			asset_data::glass_world data;
			data.name = plain::read_string(native.name);
			data.g_glass_data = plain::read_single(native.g_glassData, [&](const auto& item) { return read_glass_data(item); });
			return data;
		}

		void write_glass_world(GlassWorld& native, const asset_data::glass_world& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.g_glassData = plain::write_single<G_GlassData>(data.g_glass_data, memory, [&](auto& out, const auto& in) { write_glass_data(out, in, resolve, memory); });
		}

		std::size_t size_glass_world([[maybe_unused]] const asset_data::glass_world& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.g_glass_data)
			{
				size += plain::rounded(sizeof(G_GlassData));
				size += size_glass_data(*data.g_glass_data);
			}
			return size;
		}

		asset_data::flagged_edge_metadata read_flagged_edge_metadata(const FlaggedEdgeMetadata& native)
		{
			asset_data::flagged_edge_metadata data;
			data.packed_adjacent_normal = native.packedAdjacentNormal;
			data.packed_open_angle = native.packedOpenAngle;
			data.flags = std::to_array(native.flags);
			return data;
		}

		void write_flagged_edge_metadata(FlaggedEdgeMetadata& native, const asset_data::flagged_edge_metadata& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.packedAdjacentNormal = data.packed_adjacent_normal;
			native.packedOpenAngle = data.packed_open_angle;
			std::ranges::copy(data.flags, native.flags);
		}

		std::size_t size_flagged_edge_metadata([[maybe_unused]] const asset_data::flagged_edge_metadata& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::pmr_octree_node read_pmr_octree_node(const PMROctreeNode& native)
		{
			asset_data::pmr_octree_node data;
			data.uniform_grid_index = std::to_array(native.uniformGridIndex);
			data.depth = native.depth;
			data.flags = plain::flag_names(asset_data::pmr_octree_flags_names, static_cast<std::uint64_t>(native.flags), "PMROctreeNode.flags");
			data.parent_node_index_or_q_edge_table_index = plain::read_scalars<std::uint32_t, 1>(&native.___u3)[0];
			data.child_node_set_index_or_q_edge_table_count = plain::read_scalars<std::uint32_t, 1>(&native.___u4)[0];
			return data;
		}

		void write_pmr_octree_node(PMROctreeNode& native, const asset_data::pmr_octree_node& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			std::ranges::copy(data.uniform_grid_index, native.uniformGridIndex);
			native.depth = data.depth;
			native.flags = static_cast<PMROctreeFlags>(plain::flag_value(asset_data::pmr_octree_flags_names, data.flags, "PMROctreeNode.flags"));
			plain::write_scalars(&native.___u3, std::array<std::uint32_t, 1>{data.parent_node_index_or_q_edge_table_index});
			plain::write_scalars(&native.___u4, std::array<std::uint32_t, 1>{data.child_node_set_index_or_q_edge_table_count});
		}

		std::size_t size_pmr_octree_node([[maybe_unused]] const asset_data::pmr_octree_node& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::pmr_octree_metadata read_pmr_octree_metadata(const PMROctreeMetadata& native)
		{
			asset_data::pmr_octree_metadata data;
			data.node_half_width = [&] { std::array<std::array<float, 4>, 16> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = plain::read_scalars<float, 4>(&native.nodeHalfWidth[i]); } return out; }();
			data.min_extent = plain::read_scalars<float, 4>(&native.minExtent);
			data.root_node = read_pmr_octree_node(native.rootNode);
			data.node_set_count = native.nodeSetCount;
			data.node_bounding_radius_sq = std::to_array(native.nodeBoundingRadiusSq);
			data.edge_bucket = plain::enum_name(asset_data::edge_query_bucket_t_names, static_cast<std::int64_t>(native.edgeBucket), "PMROctreeMetadata.edgeBucket");
			data.flags = std::to_array(native.flags);
			plain::require_zero(native.pad, "PMROctreeMetadata.pad");
			return data;
		}

		void write_pmr_octree_metadata(PMROctreeMetadata& native, const asset_data::pmr_octree_metadata& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			for (std::size_t i = 0; i < data.node_half_width.size(); ++i)
			{
				plain::write_scalars(&native.nodeHalfWidth[i], data.node_half_width[i]);
			}
			plain::write_scalars(&native.minExtent, data.min_extent);
			write_pmr_octree_node(native.rootNode, data.root_node, resolve, memory);
			native.nodeSetCount = data.node_set_count;
			std::ranges::copy(data.node_bounding_radius_sq, native.nodeBoundingRadiusSq);
			native.edgeBucket = static_cast<edgeQueryBucket_t>(plain::enum_value(asset_data::edge_query_bucket_t_names, data.edge_bucket, "PMROctreeMetadata.edgeBucket"));
			std::ranges::copy(data.flags, native.flags);
		}

		std::size_t size_pmr_octree_metadata([[maybe_unused]] const asset_data::pmr_octree_metadata& data)
		{
			std::size_t size = 0;
			size += size_pmr_octree_node(data.root_node);
			return size;
		}

		asset_data::pmr_octree_node_set read_pmr_octree_node_set(const PMROctreeNodeSet& native)
		{
			asset_data::pmr_octree_node_set data;
			data.node = [&] { std::array<asset_data::pmr_octree_node, 8> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_pmr_octree_node(native.node[i]); } return out; }();
			return data;
		}

		void write_pmr_octree_node_set(PMROctreeNodeSet& native, const asset_data::pmr_octree_node_set& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			for (std::size_t i = 0; i < data.node.size(); ++i)
			{
				write_pmr_octree_node(native.node[i], data.node[i], resolve, memory);
			}
		}

		std::size_t size_pmr_octree_node_set([[maybe_unused]] const asset_data::pmr_octree_node_set& data)
		{
			std::size_t size = 0;
			for (const auto& item : data.node)
			{
				size += size_pmr_octree_node(item);
			}
			return size;
		}

		asset_data::edge_adjacency_metadata read_edge_adjacency_metadata(const EdgeAdjacencyMetadata& native)
		{
			asset_data::edge_adjacency_metadata data;
			data.start_index = native.startIndex;
			data.mount_count = native.mountCount;
			data.interaction_count = native.interactionCount;
			return data;
		}

		void write_edge_adjacency_metadata(EdgeAdjacencyMetadata& native, const asset_data::edge_adjacency_metadata& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.startIndex = data.start_index;
			native.mountCount = data.mount_count;
			native.interactionCount = data.interaction_count;
		}

		std::size_t size_edge_adjacency_metadata([[maybe_unused]] const asset_data::edge_adjacency_metadata& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::edge_adjacency read_edge_adjacency(const EdgeAdjacency& native)
		{
			asset_data::edge_adjacency data;
			data.start = native.start;
			data.end = native.end;
			data.adj_edge = native.adjEdge;
			data.adj_start = native.adjStart;
			data.adj_end = native.adjEnd;
			return data;
		}

		void write_edge_adjacency(EdgeAdjacency& native, const asset_data::edge_adjacency& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.start = data.start;
			native.end = data.end;
			native.adjEdge = data.adj_edge;
			native.adjStart = data.adj_start;
			native.adjEnd = data.adj_end;
		}

		std::size_t size_edge_adjacency([[maybe_unused]] const asset_data::edge_adjacency& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::map_edge_list read_map_edge_list(const MapEdgeList& native)
		{
			asset_data::map_edge_list data;
			data.name = plain::read_string(native.name);
			data.mins = plain::read_scalars<float, 3>(&native.mins);
			data.maxs = plain::read_scalars<float, 3>(&native.maxs);
			data.transient_index = native.transientIndex;
			data.dynamic_query_types = native.dynamicQueryTypes;
			data.static_query_types = native.staticQueryTypes;
			data.num_dynamic_edges = native.numDynamicEdges;
			data.num_static_edges = native.numStaticEdges;
			data.valid = native.valid;
			data.edges = plain::read_array(reinterpret_cast<const std::array<float4, 2>*>(native.edges), native.numEdges, "MapEdgeList.edges", [&](const auto& item) { return plain::read_block<std::array<std::array<float, 4>, 2>>(&item); });
			data.edge_metadata = plain::read_array(native.edgeMetadata, native.numEdges, "MapEdgeList.edgeMetadata", [&](const auto& item) { return read_flagged_edge_metadata(item); });
			data.edge_octrees = plain::read_array(native.edgeOctrees, native.numEdgeOctrees, "MapEdgeList.edgeOctrees", [&](const auto& item) { return read_pmr_octree_metadata(item); });
			data.edge_octree_node_sets = plain::read_array(native.edgeOctreeNodeSets, native.numEdgeOctreeNodeSet, "MapEdgeList.edgeOctreeNodeSets", [&](const auto& item) { return read_pmr_octree_node_set(item); });
			data.edge_indices = plain::read_array(native.edgeIndices, native.numEdgeIndices, "MapEdgeList.edgeIndices", [&](const auto& item) { return item; });
			data.edge_adjacency_metadata = plain::read_array(native.edgeAdjacencyMetadata, native.numEdgeAdjacencyMetadata, "MapEdgeList.edgeAdjacencyMetadata", [&](const auto& item) { return read_edge_adjacency_metadata(item); });
			data.edge_adjacency = plain::read_array(native.edgeAdjacency, native.numEdgeAdjacency, "MapEdgeList.edgeAdjacency", [&](const auto& item) { return read_edge_adjacency(item); });
			return data;
		}

		void write_map_edge_list(MapEdgeList& native, const asset_data::map_edge_list& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			plain::write_scalars(&native.mins, data.mins);
			plain::write_scalars(&native.maxs, data.maxs);
			native.transientIndex = data.transient_index;
			native.dynamicQueryTypes = data.dynamic_query_types;
			native.staticQueryTypes = data.static_query_types;
			native.numDynamicEdges = data.num_dynamic_edges;
			native.numStaticEdges = data.num_static_edges;
			native.valid = data.valid;
			native.edges = reinterpret_cast<float4*>(plain::write_array<std::array<float4, 2>>(data.edges, memory, [&](auto& out, const auto& in) { plain::write_block(&out, in); }));
			native.edgeMetadata = plain::write_array<FlaggedEdgeMetadata>(data.edge_metadata, memory, [&](auto& out, const auto& in) { write_flagged_edge_metadata(out, in, resolve, memory); });
			native.edgeOctrees = plain::write_array<PMROctreeMetadata>(data.edge_octrees, memory, [&](auto& out, const auto& in) { write_pmr_octree_metadata(out, in, resolve, memory); });
			native.edgeOctreeNodeSets = plain::write_array<PMROctreeNodeSet>(data.edge_octree_node_sets, memory, [&](auto& out, const auto& in) { write_pmr_octree_node_set(out, in, resolve, memory); });
			native.edgeIndices = plain::write_array<std::uint32_t>(data.edge_indices, memory, [&](auto& out, const auto& in) { out = in; });
			native.edgeAdjacencyMetadata = plain::write_array<EdgeAdjacencyMetadata>(data.edge_adjacency_metadata, memory, [&](auto& out, const auto& in) { write_edge_adjacency_metadata(out, in, resolve, memory); });
			native.edgeAdjacency = plain::write_array<EdgeAdjacency>(data.edge_adjacency, memory, [&](auto& out, const auto& in) { write_edge_adjacency(out, in, resolve, memory); });
			native.numEdges = plain::count_of<std::uint32_t>(data.edges);
			native.numEdgeOctrees = plain::count_of<std::uint32_t>(data.edge_octrees);
			native.numEdgeOctreeNodeSet = plain::count_of<std::uint32_t>(data.edge_octree_node_sets);
			native.numEdgeIndices = plain::count_of<std::uint32_t>(data.edge_indices);
			native.numEdgeAdjacencyMetadata = plain::count_of<std::uint32_t>(data.edge_adjacency_metadata);
			native.numEdgeAdjacency = plain::count_of<std::uint32_t>(data.edge_adjacency);
		}

		std::size_t size_map_edge_list([[maybe_unused]] const asset_data::map_edge_list& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.edges)
			{
				size += plain::rounded(std::max<std::size_t>(data.edges->size(), 1) * 2 * sizeof(float4));
			}
			if (data.edge_metadata)
			{
				size += plain::rounded(std::max<std::size_t>(data.edge_metadata->size(), 1) * sizeof(FlaggedEdgeMetadata));
				for (const auto& item : *data.edge_metadata)
				{
					size += size_flagged_edge_metadata(item);
				}
			}
			if (data.edge_octrees)
			{
				size += plain::rounded(std::max<std::size_t>(data.edge_octrees->size(), 1) * sizeof(PMROctreeMetadata));
				for (const auto& item : *data.edge_octrees)
				{
					size += size_pmr_octree_metadata(item);
				}
			}
			if (data.edge_octree_node_sets)
			{
				size += plain::rounded(std::max<std::size_t>(data.edge_octree_node_sets->size(), 1) * sizeof(PMROctreeNodeSet));
				for (const auto& item : *data.edge_octree_node_sets)
				{
					size += size_pmr_octree_node_set(item);
				}
			}
			if (data.edge_indices)
			{
				size += plain::rounded(std::max<std::size_t>(data.edge_indices->size(), 1) * sizeof(std::uint32_t));
			}
			if (data.edge_adjacency_metadata)
			{
				size += plain::rounded(std::max<std::size_t>(data.edge_adjacency_metadata->size(), 1) * sizeof(EdgeAdjacencyMetadata));
				for (const auto& item : *data.edge_adjacency_metadata)
				{
					size += size_edge_adjacency_metadata(item);
				}
			}
			if (data.edge_adjacency)
			{
				size += plain::rounded(std::max<std::size_t>(data.edge_adjacency->size(), 1) * sizeof(EdgeAdjacency));
				for (const auto& item : *data.edge_adjacency)
				{
					size += size_edge_adjacency(item);
				}
			}
			return size;
		}

		asset_data::nav_resource_s read_nav_resource_s(const nav_resource_s& native)
		{
			asset_data::nav_resource_s data;
			data.target_name = plain::read_script_string(native.targetName);
			data.model_idx = native.modelIdx;
			data.target_ent_num = native.targetEntNum;
			plain::require_zero(native.pSpace, "nav_resource_s.pSpace");
			data.local_offset_pos = plain::read_scalars<float, 3>(&native.localOffsetPos);
			data.local_offset_rot = plain::read_scalars<float, 4>(&native.localOffsetRot);
			data.offset_layer = native.offsetLayer;
			plain::require_zero(native.pPrev, "nav_resource_s.pPrev");
			plain::require_zero(native.pNext, "nav_resource_s.pNext");
			data.build_error = native.buildError;
			data.layer_flags = native.layerFlags;
			data.b_dockable = native.bDockable;
			data.b_is_volume = native.bIsVolume;
			data.p_graph_buffer = plain::read_array(native.pGraphBuffer, native.graphSize, "nav_resource_s.pGraphBuffer", [&](const auto& item) { return item; });
			data.p_working_graph = plain::read_array(native.pWorkingGraph, (static_cast<std::uint8_t>(native.bDockable) == 0 ? 0 : native.graphSize), "nav_resource_s.pWorkingGraph", [&](const auto& item) { return item; });
			return data;
		}

		void write_nav_resource_s(nav_resource_s& native, const asset_data::nav_resource_s& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.targetName = static_cast<scr_string_t>(plain::write_script_string(data.target_name));
			native.modelIdx = data.model_idx;
			native.targetEntNum = data.target_ent_num;
			plain::write_scalars(&native.localOffsetPos, data.local_offset_pos);
			plain::write_scalars(&native.localOffsetRot, data.local_offset_rot);
			native.offsetLayer = data.offset_layer;
			native.buildError = data.build_error;
			native.layerFlags = data.layer_flags;
			native.bDockable = data.b_dockable;
			native.bIsVolume = data.b_is_volume;
			native.pGraphBuffer = plain::write_array<std::uint8_t>(data.p_graph_buffer, memory, [&](auto& out, const auto& in) { out = in; });
			native.pWorkingGraph = plain::write_array<std::uint8_t>(data.p_working_graph, memory, [&](auto& out, const auto& in) { out = in; });
			native.graphSize = plain::count_of<std::int32_t>(data.p_graph_buffer);
		}

		std::size_t size_nav_resource_s([[maybe_unused]] const asset_data::nav_resource_s& data)
		{
			std::size_t size = 0;
			if (data.p_graph_buffer)
			{
				size += plain::rounded(std::max<std::size_t>(data.p_graph_buffer->size(), 1) * sizeof(std::uint8_t));
			}
			if (data.p_working_graph)
			{
				size += plain::rounded(std::max<std::size_t>(data.p_working_graph->size(), 1) * sizeof(std::uint8_t));
			}
			return size;
		}

		asset_data::nav_boundary_plane_s read_nav_boundary_plane_s(const nav_boundary_plane_s& native)
		{
			asset_data::nav_boundary_plane_s data;
			data.m_normal = plain::read_scalars<float, 3>(&native.m_Normal);
			data.m_dist_from_center = native.m_DistFromCenter;
			return data;
		}

		void write_nav_boundary_plane_s(nav_boundary_plane_s& native, const asset_data::nav_boundary_plane_s& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.m_Normal, data.m_normal);
			native.m_DistFromCenter = data.m_dist_from_center;
		}

		std::size_t size_nav_boundary_plane_s([[maybe_unused]] const asset_data::nav_boundary_plane_s& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::nav_obstacle_hull_s read_nav_obstacle_hull_s(const nav_obstacle_hull_s& native)
		{
			asset_data::nav_obstacle_hull_s data;
			data.m_boundaries = plain::read_array(native.m_Boundaries, native.m_NumBoundaries, "nav_obstacle_hull_s.m_Boundaries", [&](const auto& item) { return read_nav_boundary_plane_s(item); });
			data.m_bounds_x = native.m_BoundsX;
			data.m_bounds_y = native.m_BoundsY;
			data.m_bounds_z = native.m_BoundsZ;
			data.m_offset = plain::read_scalars<float, 3>(&native.m_Offset);
			data.m_b_use_bounds = native.m_bUseBounds;
			return data;
		}

		void write_nav_obstacle_hull_s(nav_obstacle_hull_s& native, const asset_data::nav_obstacle_hull_s& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.m_Boundaries = plain::write_array<nav_boundary_plane_s>(data.m_boundaries, memory, [&](auto& out, const auto& in) { write_nav_boundary_plane_s(out, in, resolve, memory); });
			native.m_BoundsX = data.m_bounds_x;
			native.m_BoundsY = data.m_bounds_y;
			native.m_BoundsZ = data.m_bounds_z;
			plain::write_scalars(&native.m_Offset, data.m_offset);
			native.m_bUseBounds = data.m_b_use_bounds;
			native.m_NumBoundaries = plain::count_of<std::int32_t>(data.m_boundaries);
		}

		std::size_t size_nav_obstacle_hull_s([[maybe_unused]] const asset_data::nav_obstacle_hull_s& data)
		{
			std::size_t size = 0;
			if (data.m_boundaries)
			{
				size += plain::rounded(std::max<std::size_t>(data.m_boundaries->size(), 1) * sizeof(nav_boundary_plane_s));
				for (const auto& item : *data.m_boundaries)
				{
					size += size_nav_boundary_plane_s(item);
				}
			}
			return size;
		}

		asset_data::nav_obstacle_bounds_s read_nav_obstacle_bounds_s(const nav_obstacle_bounds_s& native)
		{
			asset_data::nav_obstacle_bounds_s data;
			data.m_hulls = plain::read_array(native.m_Hulls, native.m_NumHulls, "nav_obstacle_bounds_s.m_Hulls", [&](const auto& item) { return read_nav_obstacle_hull_s(item); });
			return data;
		}

		void write_nav_obstacle_bounds_s(nav_obstacle_bounds_s& native, const asset_data::nav_obstacle_bounds_s& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.m_Hulls = plain::write_array<nav_obstacle_hull_s>(data.m_hulls, memory, [&](auto& out, const auto& in) { write_nav_obstacle_hull_s(out, in, resolve, memory); });
			native.m_NumHulls = plain::count_of<std::int32_t>(data.m_hulls);
		}

		std::size_t size_nav_obstacle_bounds_s([[maybe_unused]] const asset_data::nav_obstacle_bounds_s& data)
		{
			std::size_t size = 0;
			if (data.m_hulls)
			{
				size += plain::rounded(std::max<std::size_t>(data.m_hulls->size(), 1) * sizeof(nav_obstacle_hull_s));
				for (const auto& item : *data.m_hulls)
				{
					size += size_nav_obstacle_hull_s(item);
				}
			}
			return size;
		}

		asset_data::nav_glass_bounds_s read_nav_glass_bounds_s(const nav_glass_bounds_s& native)
		{
			asset_data::nav_glass_bounds_s data;
			data.m_boundaries = plain::read_array(native.m_Boundaries, native.m_NumBoundaries, "nav_glass_bounds_s.m_Boundaries", [&](const auto& item) { return read_nav_boundary_plane_s(item); });
			data.m_pos = plain::read_scalars<float, 3>(&native.m_Pos);
			data.m_glass_piece_index = native.m_GlassPieceIndex;
			return data;
		}

		void write_nav_glass_bounds_s(nav_glass_bounds_s& native, const asset_data::nav_glass_bounds_s& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.m_Boundaries = plain::write_array<nav_boundary_plane_s>(data.m_boundaries, memory, [&](auto& out, const auto& in) { write_nav_boundary_plane_s(out, in, resolve, memory); });
			plain::write_scalars(&native.m_Pos, data.m_pos);
			native.m_GlassPieceIndex = data.m_glass_piece_index;
			native.m_NumBoundaries = plain::count_of<std::int32_t>(data.m_boundaries);
		}

		std::size_t size_nav_glass_bounds_s([[maybe_unused]] const asset_data::nav_glass_bounds_s& data)
		{
			std::size_t size = 0;
			if (data.m_boundaries)
			{
				size += plain::rounded(std::max<std::size_t>(data.m_boundaries->size(), 1) * sizeof(nav_boundary_plane_s));
				for (const auto& item : *data.m_boundaries)
				{
					size += size_nav_boundary_plane_s(item);
				}
			}
			return size;
		}

		asset_data::nav_modifier_s read_nav_modifier_s(const nav_modifier_s& native)
		{
			asset_data::nav_modifier_s data;
			data.m_boundaries = plain::read_array(native.m_Boundaries, native.m_NumBoundaries, "nav_modifier_s.m_Boundaries", [&](const auto& item) { return read_nav_boundary_plane_s(item); });
			data.m_pos = plain::read_scalars<float, 3>(&native.m_Pos);
			data.m_target_name = plain::read_script_string(native.m_TargetName);
			data.m_target = plain::read_script_string(native.m_Target);
			data.m_script_noteworthy = plain::read_script_string(native.m_ScriptNoteworthy);
			data.m_script_link_name = plain::read_script_string(native.m_ScriptLinkName);
			data.m_user_flags = native.m_UserFlags;
			data.m_layer_flags = native.m_LayerFlags;
			data.m_weight = native.m_Weight;
			data.m_b_active_on_load = native.m_bActiveOnLoad;
			return data;
		}

		void write_nav_modifier_s(nav_modifier_s& native, const asset_data::nav_modifier_s& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.m_Boundaries = plain::write_array<nav_boundary_plane_s>(data.m_boundaries, memory, [&](auto& out, const auto& in) { write_nav_boundary_plane_s(out, in, resolve, memory); });
			plain::write_scalars(&native.m_Pos, data.m_pos);
			native.m_TargetName = static_cast<scr_string_t>(plain::write_script_string(data.m_target_name));
			native.m_Target = static_cast<scr_string_t>(plain::write_script_string(data.m_target));
			native.m_ScriptNoteworthy = static_cast<scr_string_t>(plain::write_script_string(data.m_script_noteworthy));
			native.m_ScriptLinkName = static_cast<scr_string_t>(plain::write_script_string(data.m_script_link_name));
			native.m_UserFlags = data.m_user_flags;
			native.m_LayerFlags = data.m_layer_flags;
			native.m_Weight = data.m_weight;
			native.m_bActiveOnLoad = data.m_b_active_on_load;
			native.m_NumBoundaries = plain::count_of<std::int32_t>(data.m_boundaries);
		}

		std::size_t size_nav_modifier_s([[maybe_unused]] const asset_data::nav_modifier_s& data)
		{
			std::size_t size = 0;
			if (data.m_boundaries)
			{
				size += plain::rounded(std::max<std::size_t>(data.m_boundaries->size(), 1) * sizeof(nav_boundary_plane_s));
				for (const auto& item : *data.m_boundaries)
				{
					size += size_nav_boundary_plane_s(item);
				}
			}
			return size;
		}

		asset_data::nav_link_point_s read_nav_link_point_s(const nav_link_point_s& native)
		{
			asset_data::nav_link_point_s data;
			data.m_pt1 = plain::read_scalars<float, 3>(&native.m_Pt1);
			data.m_pt2 = plain::read_scalars<float, 3>(&native.m_Pt2);
			return data;
		}

		void write_nav_link_point_s(nav_link_point_s& native, const asset_data::nav_link_point_s& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.m_Pt1, data.m_pt1);
			plain::write_scalars(&native.m_Pt2, data.m_pt2);
		}

		std::size_t size_nav_link_point_s([[maybe_unused]] const asset_data::nav_link_point_s& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::nav_link_creation_data_s read_nav_link_creation_data_s(const nav_link_creation_data_s& native)
		{
			asset_data::nav_link_creation_data_s data;
			data.m_start = read_nav_link_point_s(native.m_Start);
			data.m_end = read_nav_link_point_s(native.m_End);
			data.m_animscript = plain::read_script_string(native.m_Animscript);
			data.m_target = plain::read_script_string(native.m_Target);
			data.m_parent = plain::read_script_string(native.m_Parent);
			data.m_usage_flags = native.m_UsageFlags;
			data.m_penalty_mult = native.m_PenaltyMult;
			data.m_b_bidirectional = native.m_bBidirectional;
			return data;
		}

		void write_nav_link_creation_data_s(nav_link_creation_data_s& native, const asset_data::nav_link_creation_data_s& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_nav_link_point_s(native.m_Start, data.m_start, resolve, memory);
			write_nav_link_point_s(native.m_End, data.m_end, resolve, memory);
			native.m_Animscript = static_cast<scr_string_t>(plain::write_script_string(data.m_animscript));
			native.m_Target = static_cast<scr_string_t>(plain::write_script_string(data.m_target));
			native.m_Parent = static_cast<scr_string_t>(plain::write_script_string(data.m_parent));
			native.m_UsageFlags = data.m_usage_flags;
			native.m_PenaltyMult = data.m_penalty_mult;
			native.m_bBidirectional = data.m_b_bidirectional;
		}

		std::size_t size_nav_link_creation_data_s([[maybe_unused]] const asset_data::nav_link_creation_data_s& data)
		{
			std::size_t size = 0;
			size += size_nav_link_point_s(data.m_start);
			size += size_nav_link_point_s(data.m_end);
			return size;
		}

		asset_data::nav_raw_volume_s read_nav_raw_volume_s(const nav_raw_volume_s& native)
		{
			asset_data::nav_raw_volume_s data;
			data.m_mid_point = plain::read_scalars<float, 3>(&native.m_MidPoint);
			data.m_half_size = plain::read_scalars<float, 3>(&native.m_HalfSize);
			data.m_min_voxel_size = native.m_MinVoxelSize;
			data.m_clearance_xy = native.m_ClearanceXY;
			data.m_clearance_z = native.m_ClearanceZ;
			return data;
		}

		void write_nav_raw_volume_s(nav_raw_volume_s& native, const asset_data::nav_raw_volume_s& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.m_MidPoint, data.m_mid_point);
			plain::write_scalars(&native.m_HalfSize, data.m_half_size);
			native.m_MinVoxelSize = data.m_min_voxel_size;
			native.m_ClearanceXY = data.m_clearance_xy;
			native.m_ClearanceZ = data.m_clearance_z;
		}

		std::size_t size_nav_raw_volume_s([[maybe_unused]] const asset_data::nav_raw_volume_s& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::nav_raw_custom_volume_tri_s read_nav_raw_custom_volume_tri_s(const nav_raw_custom_volume_tri_s& native)
		{
			asset_data::nav_raw_custom_volume_tri_s data;
			data.m_verts = [&] { std::array<std::array<float, 3>, 3> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = plain::read_scalars<float, 3>(&native.m_Verts[i]); } return out; }();
			return data;
		}

		void write_nav_raw_custom_volume_tri_s(nav_raw_custom_volume_tri_s& native, const asset_data::nav_raw_custom_volume_tri_s& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			for (std::size_t i = 0; i < data.m_verts.size(); ++i)
			{
				plain::write_scalars(&native.m_Verts[i], data.m_verts[i]);
			}
		}

		std::size_t size_nav_raw_custom_volume_tri_s([[maybe_unused]] const asset_data::nav_raw_custom_volume_tri_s& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::nav_raw_custom_volume_s read_nav_raw_custom_volume_s(const nav_raw_custom_volume_s& native)
		{
			asset_data::nav_raw_custom_volume_s data;
			data.m_tris = plain::read_array(native.m_Tris, native.m_NumTris, "nav_raw_custom_volume_s.m_Tris", [&](const auto& item) { return read_nav_raw_custom_volume_tri_s(item); });
			return data;
		}

		void write_nav_raw_custom_volume_s(nav_raw_custom_volume_s& native, const asset_data::nav_raw_custom_volume_s& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.m_Tris = plain::write_array<nav_raw_custom_volume_tri_s>(data.m_tris, memory, [&](auto& out, const auto& in) { write_nav_raw_custom_volume_tri_s(out, in, resolve, memory); });
			native.m_NumTris = plain::count_of<std::int32_t>(data.m_tris);
		}

		std::size_t size_nav_raw_custom_volume_s([[maybe_unused]] const asset_data::nav_raw_custom_volume_s& data)
		{
			std::size_t size = 0;
			if (data.m_tris)
			{
				size += plain::rounded(std::max<std::size_t>(data.m_tris->size(), 1) * sizeof(nav_raw_custom_volume_tri_s));
				for (const auto& item : *data.m_tris)
				{
					size += size_nav_raw_custom_volume_tri_s(item);
				}
			}
			return size;
		}

		asset_data::nav_misc_s read_nav_misc_s(const nav_misc_s& native)
		{
			asset_data::nav_misc_s data;
			data.m_static_bounds_half_size = plain::read_scalars<float, 3>(&native.m_StaticBoundsHalfSize);
			data.m_static_bounds_mid_point = plain::read_scalars<float, 3>(&native.m_StaticBoundsMidPoint);
			return data;
		}

		void write_nav_misc_s(nav_misc_s& native, const asset_data::nav_misc_s& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.m_StaticBoundsHalfSize, data.m_static_bounds_half_size);
			plain::write_scalars(&native.m_StaticBoundsMidPoint, data.m_static_bounds_mid_point);
		}

		std::size_t size_nav_misc_s([[maybe_unused]] const asset_data::nav_misc_s& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::nav_mesh read_nav_mesh(const NavMeshData& native)
		{
			asset_data::nav_mesh data;
			data.name = plain::read_string(native.name);
			data.version = native.version;
			data.has_exposure_data = native.hasExposureData;
			data.nav_resources = plain::read_array(native.navResources, native.numNavResources, "NavMeshData.navResources", [&](const auto& item) { return read_nav_resource_s(item); });
			data.obstacle_bounds = plain::read_array(native.obstacleBounds, native.numObstacleBounds, "NavMeshData.obstacleBounds", [&](const auto& item) { return read_nav_obstacle_bounds_s(item); });
			data.glass_bounds = plain::read_array(native.glassBounds, native.numGlassBounds, "NavMeshData.glassBounds", [&](const auto& item) { return read_nav_glass_bounds_s(item); });
			data.modifiers = plain::read_array(native.modifiers, native.numModifiers, "NavMeshData.modifiers", [&](const auto& item) { return read_nav_modifier_s(item); });
			data.link_creation_data = plain::read_array(native.linkCreationData, native.numLinkCreationData, "NavMeshData.linkCreationData", [&](const auto& item) { return read_nav_link_creation_data_s(item); });
			data.volume_seeds = plain::read_array(native.volumeSeeds, native.numVolumeSeeds, "NavMeshData.volumeSeeds", [&](const auto& item) { return plain::read_scalars<float, 3>(&item); });
			data.raw_volumes = plain::read_array(native.rawVolumes, native.numRawVolumes, "NavMeshData.rawVolumes", [&](const auto& item) { return read_nav_raw_volume_s(item); });
			data.raw_custom_volumes = plain::read_array(native.rawCustomVolumes, native.numRawCustomVolumes, "NavMeshData.rawCustomVolumes", [&](const auto& item) { return read_nav_raw_custom_volume_s(item); });
			data.misc_data = read_nav_misc_s(native.miscData);
			return data;
		}

		void write_nav_mesh(NavMeshData& native, const asset_data::nav_mesh& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.version = data.version;
			native.hasExposureData = data.has_exposure_data;
			native.navResources = plain::write_array<nav_resource_s>(data.nav_resources, memory, [&](auto& out, const auto& in) { write_nav_resource_s(out, in, resolve, memory); });
			native.obstacleBounds = plain::write_array<nav_obstacle_bounds_s>(data.obstacle_bounds, memory, [&](auto& out, const auto& in) { write_nav_obstacle_bounds_s(out, in, resolve, memory); });
			native.glassBounds = plain::write_array<nav_glass_bounds_s>(data.glass_bounds, memory, [&](auto& out, const auto& in) { write_nav_glass_bounds_s(out, in, resolve, memory); });
			native.modifiers = plain::write_array<nav_modifier_s>(data.modifiers, memory, [&](auto& out, const auto& in) { write_nav_modifier_s(out, in, resolve, memory); });
			native.linkCreationData = plain::write_array<nav_link_creation_data_s>(data.link_creation_data, memory, [&](auto& out, const auto& in) { write_nav_link_creation_data_s(out, in, resolve, memory); });
			native.volumeSeeds = plain::write_array<vec3_t>(data.volume_seeds, memory, [&](auto& out, const auto& in) { plain::write_scalars(&out, in); });
			native.rawVolumes = plain::write_array<nav_raw_volume_s>(data.raw_volumes, memory, [&](auto& out, const auto& in) { write_nav_raw_volume_s(out, in, resolve, memory); });
			native.rawCustomVolumes = plain::write_array<nav_raw_custom_volume_s>(data.raw_custom_volumes, memory, [&](auto& out, const auto& in) { write_nav_raw_custom_volume_s(out, in, resolve, memory); });
			write_nav_misc_s(native.miscData, data.misc_data, resolve, memory);
			native.numNavResources = plain::count_of<std::int32_t>(data.nav_resources);
			native.numObstacleBounds = plain::count_of<std::int32_t>(data.obstacle_bounds);
			native.numGlassBounds = plain::count_of<std::int32_t>(data.glass_bounds);
			native.numModifiers = plain::count_of<std::int32_t>(data.modifiers);
			native.numLinkCreationData = plain::count_of<std::int32_t>(data.link_creation_data);
			native.numVolumeSeeds = plain::count_of<std::int32_t>(data.volume_seeds);
			native.numRawVolumes = plain::count_of<std::int32_t>(data.raw_volumes);
			native.numRawCustomVolumes = plain::count_of<std::int32_t>(data.raw_custom_volumes);
		}

		std::size_t size_nav_mesh([[maybe_unused]] const asset_data::nav_mesh& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.nav_resources)
			{
				size += plain::rounded(std::max<std::size_t>(data.nav_resources->size(), 1) * sizeof(nav_resource_s));
				for (const auto& item : *data.nav_resources)
				{
					size += size_nav_resource_s(item);
				}
			}
			if (data.obstacle_bounds)
			{
				size += plain::rounded(std::max<std::size_t>(data.obstacle_bounds->size(), 1) * sizeof(nav_obstacle_bounds_s));
				for (const auto& item : *data.obstacle_bounds)
				{
					size += size_nav_obstacle_bounds_s(item);
				}
			}
			if (data.glass_bounds)
			{
				size += plain::rounded(std::max<std::size_t>(data.glass_bounds->size(), 1) * sizeof(nav_glass_bounds_s));
				for (const auto& item : *data.glass_bounds)
				{
					size += size_nav_glass_bounds_s(item);
				}
			}
			if (data.modifiers)
			{
				size += plain::rounded(std::max<std::size_t>(data.modifiers->size(), 1) * sizeof(nav_modifier_s));
				for (const auto& item : *data.modifiers)
				{
					size += size_nav_modifier_s(item);
				}
			}
			if (data.link_creation_data)
			{
				size += plain::rounded(std::max<std::size_t>(data.link_creation_data->size(), 1) * sizeof(nav_link_creation_data_s));
				for (const auto& item : *data.link_creation_data)
				{
					size += size_nav_link_creation_data_s(item);
				}
			}
			if (data.volume_seeds)
			{
				size += plain::rounded(std::max<std::size_t>(data.volume_seeds->size(), 1) * sizeof(vec3_t));
			}
			if (data.raw_volumes)
			{
				size += plain::rounded(std::max<std::size_t>(data.raw_volumes->size(), 1) * sizeof(nav_raw_volume_s));
				for (const auto& item : *data.raw_volumes)
				{
					size += size_nav_raw_volume_s(item);
				}
			}
			if (data.raw_custom_volumes)
			{
				size += plain::rounded(std::max<std::size_t>(data.raw_custom_volumes->size(), 1) * sizeof(nav_raw_custom_volume_s));
				for (const auto& item : *data.raw_custom_volumes)
				{
					size += size_nav_raw_custom_volume_s(item);
				}
			}
			size += size_nav_misc_s(data.misc_data);
			return size;
		}

		asset_data::bfx_area_handle read_bfx_area_handle(const bfx_AreaHandle& native)
		{
			asset_data::bfx_area_handle data;
			plain::require_zero(native.m_handleImpl, "bfx_AreaHandle.m_handleImpl");
			plain::require_zero(native.m_pSpace, "bfx_AreaHandle.m_pSpace");
			return data;
		}

		void write_bfx_area_handle(bfx_AreaHandle& native, const asset_data::bfx_area_handle& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
		}

		std::size_t size_bfx_area_handle([[maybe_unused]] const asset_data::bfx_area_handle& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::tacpoint_t read_tacpoint_t(const tacpoint_t& native)
		{
			asset_data::tacpoint_t data;
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

		void write_tacpoint_t(tacpoint_t& native, const asset_data::tacpoint_t& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
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

		std::size_t size_tacpoint_t([[maybe_unused]] const asset_data::tacpoint_t& data)
		{
			std::size_t size = 0;
			size += size_bfx_area_handle(data.m_h_area);
			return size;
		}

		asset_data::tac_vis_graph_row read_tac_vis_graph_row(const TacVisGraphRow& native)
		{
			asset_data::tac_vis_graph_row data;
			data.m_start = native.m_Start;
			data.m_end = native.m_End;
			plain::require_zero(native.m_Vis, "TacVisGraphRow.m_Vis");
			return data;
		}

		void write_tac_vis_graph_row(TacVisGraphRow& native, const asset_data::tac_vis_graph_row& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.m_Start = data.m_start;
			native.m_End = data.m_end;
		}

		std::size_t size_tac_vis_graph_row([[maybe_unused]] const asset_data::tac_vis_graph_row& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::tacpoint_search_node_t read_tacpoint_search_node_t(const tacpoint_search_node_t& native)
		{
			asset_data::tacpoint_search_node_t data;
			data.m_mid_point = plain::read_scalars<float, 2>(&native.m_MidPoint);
			data.m_half_size = plain::read_scalars<float, 2>(&native.m_HalfSize);
			data.m_point_list_root = native.m_PointListRoot;
			plain::require_zero(native.m_ChildNodes, "tacpoint_search_node_t.m_ChildNodes");
			return data;
		}

		void write_tacpoint_search_node_t(tacpoint_search_node_t& native, const asset_data::tacpoint_search_node_t& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.m_MidPoint, data.m_mid_point);
			plain::write_scalars(&native.m_HalfSize, data.m_half_size);
			native.m_PointListRoot = data.m_point_list_root;
		}

		std::size_t size_tacpoint_search_node_t([[maybe_unused]] const asset_data::tacpoint_search_node_t& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::tactical_graph read_tactical_graph(const TacticalGraph& native)
		{
			asset_data::tactical_graph data;
			data.m_points = plain::read_array(native.m_Points, native.m_NumPoints, "TacticalGraph.m_Points", [&](const auto& item) { return read_tacpoint_t(item); });
			data.m_num_vis_bytes = native.m_NumVisBytes;
			data.m_vis_graph = plain::read_array(native.m_VisGraph, ((-1) + native.m_NumPoints), "TacticalGraph.m_VisGraph", [&](const auto& item) { return read_tac_vis_graph_row(item); });
			data.m_search_root = read_tacpoint_search_node_t(native.m_SearchRoot);
			return data;
		}

		void write_tactical_graph(TacticalGraph& native, const asset_data::tactical_graph& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.m_Points = plain::write_array<tacpoint_t>(data.m_points, memory, [&](auto& out, const auto& in) { write_tacpoint_t(out, in, resolve, memory); });
			native.m_NumVisBytes = data.m_num_vis_bytes;
			native.m_VisGraph = plain::write_array<TacVisGraphRow>(data.m_vis_graph, memory, [&](auto& out, const auto& in) { write_tac_vis_graph_row(out, in, resolve, memory); });
			write_tacpoint_search_node_t(native.m_SearchRoot, data.m_search_root, resolve, memory);
			native.m_NumPoints = plain::count_of<std::uint16_t>(data.m_points);
		}

		std::size_t size_tactical_graph([[maybe_unused]] const asset_data::tactical_graph& data)
		{
			std::size_t size = 0;
			if (data.m_points)
			{
				size += plain::rounded(std::max<std::size_t>(data.m_points->size(), 1) * sizeof(tacpoint_t));
				for (const auto& item : *data.m_points)
				{
					size += size_tacpoint_t(item);
				}
			}
			if (data.m_vis_graph)
			{
				size += plain::rounded(std::max<std::size_t>(data.m_vis_graph->size(), 1) * sizeof(TacVisGraphRow));
				for (const auto& item : *data.m_vis_graph)
				{
					size += size_tac_vis_graph_row(item);
				}
			}
			size += size_tacpoint_search_node_t(data.m_search_root);
			return size;
		}

		asset_data::tactical_graph_data read_tactical_graph_data(const TacticalGraphData& native)
		{
			asset_data::tactical_graph_data data;
			data.m_name = plain::read_string(native.m_Name);
			data.m_tac_graphs = plain::read_array(native.m_TacGraphs, native.m_NumTacGraphs, "TacticalGraphData.m_TacGraphs", [&](const auto& item) { return read_tactical_graph(item); });
			return data;
		}

		void write_tactical_graph_data(TacticalGraphData& native, const asset_data::tactical_graph_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.m_Name = plain::write_string(memory, data.m_name);
			native.m_TacGraphs = plain::write_array<TacticalGraph>(data.m_tac_graphs, memory, [&](auto& out, const auto& in) { write_tactical_graph(out, in, resolve, memory); });
			native.m_NumTacGraphs = plain::count_of<std::int32_t>(data.m_tac_graphs);
		}

		std::size_t size_tactical_graph_data([[maybe_unused]] const asset_data::tactical_graph_data& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.m_name);
			if (data.m_tac_graphs)
			{
				size += plain::rounded(std::max<std::size_t>(data.m_tac_graphs->size(), 1) * sizeof(TacticalGraph));
				for (const auto& item : *data.m_tac_graphs)
				{
					size += size_tactical_graph(item);
				}
			}
			return size;
		}

		asset_data::static_model_collision_instance read_static_model_collision_instance(const StaticModelCollisionInstance& native)
		{
			asset_data::static_model_collision_instance data;
			data.origin = plain::read_scalars<float, 3>(&native.origin);
			data.angles = plain::read_scalars<float, 3>(&native.angles);
			data.scale = native.scale;
			return data;
		}

		void write_static_model_collision_instance(StaticModelCollisionInstance& native, const asset_data::static_model_collision_instance& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.origin, data.origin);
			plain::write_scalars(&native.angles, data.angles);
			native.scale = data.scale;
		}

		std::size_t size_static_model_collision_instance([[maybe_unused]] const asset_data::static_model_collision_instance& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::static_model_collision_model read_static_model_collision_model(const StaticModelCollisionModel& native)
		{
			asset_data::static_model_collision_model data;
			data.name = plain::read_script_string(native.name);
			data.physics_asset = plain::read_asset(ASSET_TYPE_PHYSICSASSET, native.physicsAsset);
			data.detail_collision = plain::read_asset(ASSET_TYPE_XMODEL_DETAIL_COLLISION, native.detailCollision);
			data.instances = plain::read_array(native.instances, native.numInstances, "StaticModelCollisionModel.instances", [&](const auto& item) { return read_static_model_collision_instance(item); });
			return data;
		}

		void write_static_model_collision_model(StaticModelCollisionModel& native, const asset_data::static_model_collision_model& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = static_cast<scr_string_t>(plain::write_script_string(data.name));
			native.physicsAsset = plain::resolve_asset<PhysicsAsset>(resolve, ASSET_TYPE_PHYSICSASSET, data.physics_asset);
			native.detailCollision = plain::resolve_asset<XModelDetailCollision>(resolve, ASSET_TYPE_XMODEL_DETAIL_COLLISION, data.detail_collision);
			native.instances = plain::write_array<StaticModelCollisionInstance>(data.instances, memory, [&](auto& out, const auto& in) { write_static_model_collision_instance(out, in, resolve, memory); });
			native.numInstances = plain::count_of<std::uint32_t>(data.instances);
		}

		std::size_t size_static_model_collision_model([[maybe_unused]] const asset_data::static_model_collision_model& data)
		{
			std::size_t size = 0;
			if (data.instances)
			{
				size += plain::rounded(std::max<std::size_t>(data.instances->size(), 1) * sizeof(StaticModelCollisionInstance));
				for (const auto& item : *data.instances)
				{
					size += size_static_model_collision_instance(item);
				}
			}
			return size;
		}

		asset_data::static_model_collision_model_list read_static_model_collision_model_list(const StaticModelCollisionModelList& native)
		{
			asset_data::static_model_collision_model_list data;
			data.models = plain::read_array(native.models, native.numModels, "StaticModelCollisionModelList.models", [&](const auto& item) { return read_static_model_collision_model(item); });
			data.structure_memory_cost = native.structureMemoryCost;
			return data;
		}

		void write_static_model_collision_model_list(StaticModelCollisionModelList& native, const asset_data::static_model_collision_model_list& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.models = plain::write_array<StaticModelCollisionModel>(data.models, memory, [&](auto& out, const auto& in) { write_static_model_collision_model(out, in, resolve, memory); });
			native.structureMemoryCost = data.structure_memory_cost;
			native.numModels = plain::count_of<std::uint32_t>(data.models);
		}

		std::size_t size_static_model_collision_model_list([[maybe_unused]] const asset_data::static_model_collision_model_list& data)
		{
			std::size_t size = 0;
			if (data.models)
			{
				size += plain::rounded(std::max<std::size_t>(data.models->size(), 1) * sizeof(StaticModelCollisionModel));
				for (const auto& item : *data.models)
				{
					size += size_static_model_collision_model(item);
				}
			}
			return size;
		}

		asset_data::stage read_stage(const Stage& native)
		{
			asset_data::stage data;
			data.name = plain::read_string(native.name);
			data.origin = plain::read_scalars<float, 3>(&native.origin);
			data.trigger_index = native.triggerIndex;
			data.sun_primary_light_index = native.sunPrimaryLightIndex;
			data.entity_uid = native.entityUID;
			data.sky_rotation_angles = plain::read_scalars<float, 3>(&native.skyRotationAngles);
			return data;
		}

		void write_stage(Stage& native, const asset_data::stage& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			plain::write_scalars(&native.origin, data.origin);
			native.triggerIndex = data.trigger_index;
			native.sunPrimaryLightIndex = data.sun_primary_light_index;
			native.entityUID = data.entity_uid;
			plain::write_scalars(&native.skyRotationAngles, data.sky_rotation_angles);
		}

		std::size_t size_stage([[maybe_unused]] const asset_data::stage& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			return size;
		}

		asset_data::trigger_model read_trigger_model(const TriggerModel& native)
		{
			asset_data::trigger_model data;
			data.contents = native.contents;
			data.hull_count = native.hullCount;
			data.first_hull = native.firstHull;
			data.winding_count = native.windingCount;
			data.first_winding = native.firstWinding;
			data.flags = native.flags;
			data.physics_asset = plain::read_asset(ASSET_TYPE_PHYSICSASSET, native.physicsAsset);
			data.physics_shape_override_idx = native.physicsShapeOverrideIdx;
			return data;
		}

		void write_trigger_model(TriggerModel& native, const asset_data::trigger_model& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.contents = data.contents;
			native.hullCount = data.hull_count;
			native.firstHull = data.first_hull;
			native.windingCount = data.winding_count;
			native.firstWinding = data.first_winding;
			native.flags = data.flags;
			native.physicsAsset = plain::resolve_asset<PhysicsAsset>(resolve, ASSET_TYPE_PHYSICSASSET, data.physics_asset);
			native.physicsShapeOverrideIdx = data.physics_shape_override_idx;
		}

		std::size_t size_trigger_model([[maybe_unused]] const asset_data::trigger_model& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::bounds read_bounds(const Bounds& native)
		{
			asset_data::bounds data;
			data.mid_point = plain::read_scalars<float, 3>(&native.midPoint);
			data.half_size = plain::read_scalars<float, 3>(&native.halfSize);
			return data;
		}

		void write_bounds(Bounds& native, const asset_data::bounds& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.midPoint, data.mid_point);
			plain::write_scalars(&native.halfSize, data.half_size);
		}

		std::size_t size_bounds([[maybe_unused]] const asset_data::bounds& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::trigger_hull read_trigger_hull(const TriggerHull& native)
		{
			asset_data::trigger_hull data;
			data.bounds = read_bounds(native.bounds);
			data.contents = native.contents;
			data.slab_count = native.slabCount;
			data.first_slab = native.firstSlab;
			return data;
		}

		void write_trigger_hull(TriggerHull& native, const asset_data::trigger_hull& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_bounds(native.bounds, data.bounds, resolve, memory);
			native.contents = data.contents;
			native.slabCount = data.slab_count;
			native.firstSlab = data.first_slab;
		}

		std::size_t size_trigger_hull([[maybe_unused]] const asset_data::trigger_hull& data)
		{
			std::size_t size = 0;
			size += size_bounds(data.bounds);
			return size;
		}

		asset_data::trigger_slab read_trigger_slab(const TriggerSlab& native)
		{
			asset_data::trigger_slab data;
			data.dir = plain::read_scalars<float, 3>(&native.dir);
			data.mid_point = native.midPoint;
			data.half_size = native.halfSize;
			return data;
		}

		void write_trigger_slab(TriggerSlab& native, const asset_data::trigger_slab& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.dir, data.dir);
			native.midPoint = data.mid_point;
			native.halfSize = data.half_size;
		}

		std::size_t size_trigger_slab([[maybe_unused]] const asset_data::trigger_slab& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::trigger_winding read_trigger_winding(const TriggerWinding& native)
		{
			asset_data::trigger_winding data;
			data.dir = plain::read_scalars<float, 3>(&native.dir);
			data.point_count = native.pointCount;
			data.first_point = native.firstPoint;
			data.flags = native.flags;
			return data;
		}

		void write_trigger_winding(TriggerWinding& native, const asset_data::trigger_winding& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.dir, data.dir);
			native.pointCount = data.point_count;
			native.firstPoint = data.first_point;
			native.flags = data.flags;
		}

		std::size_t size_trigger_winding([[maybe_unused]] const asset_data::trigger_winding& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::trigger_winding_point read_trigger_winding_point(const TriggerWindingPoint& native)
		{
			asset_data::trigger_winding_point data;
			data.loc = plain::read_scalars<float, 3>(&native.loc);
			return data;
		}

		void write_trigger_winding_point(TriggerWindingPoint& native, const asset_data::trigger_winding_point& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.loc, data.loc);
		}

		std::size_t size_trigger_winding_point([[maybe_unused]] const asset_data::trigger_winding_point& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::map_triggers read_map_triggers(const MapTriggers& native)
		{
			asset_data::map_triggers data;
			data.count = native.count;
			data.models = plain::read_array(native.models, native.count, "MapTriggers.models", [&](const auto& item) { return read_trigger_model(item); });
			data.hulls = plain::read_array(native.hulls, native.hullCount, "MapTriggers.hulls", [&](const auto& item) { return read_trigger_hull(item); });
			data.slabs = plain::read_array(native.slabs, native.slabCount, "MapTriggers.slabs", [&](const auto& item) { return read_trigger_slab(item); });
			data.windings = plain::read_array(native.windings, native.windingCount, "MapTriggers.windings", [&](const auto& item) { return read_trigger_winding(item); });
			data.winding_points = plain::read_array(native.windingPoints, native.windingPointCount, "MapTriggers.windingPoints", [&](const auto& item) { return read_trigger_winding_point(item); });
			return data;
		}

		void write_map_triggers(MapTriggers& native, const asset_data::map_triggers& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.count = data.count;
			native.models = plain::write_array<TriggerModel>(data.models, memory, [&](auto& out, const auto& in) { write_trigger_model(out, in, resolve, memory); });
			native.hulls = plain::write_array<TriggerHull>(data.hulls, memory, [&](auto& out, const auto& in) { write_trigger_hull(out, in, resolve, memory); });
			native.slabs = plain::write_array<TriggerSlab>(data.slabs, memory, [&](auto& out, const auto& in) { write_trigger_slab(out, in, resolve, memory); });
			native.windings = plain::write_array<TriggerWinding>(data.windings, memory, [&](auto& out, const auto& in) { write_trigger_winding(out, in, resolve, memory); });
			native.windingPoints = plain::write_array<TriggerWindingPoint>(data.winding_points, memory, [&](auto& out, const auto& in) { write_trigger_winding_point(out, in, resolve, memory); });
			native.hullCount = plain::count_of<std::uint32_t>(data.hulls);
			native.slabCount = plain::count_of<std::uint32_t>(data.slabs);
			native.windingCount = plain::count_of<std::uint32_t>(data.windings);
			native.windingPointCount = plain::count_of<std::uint32_t>(data.winding_points);
		}

		std::size_t size_map_triggers([[maybe_unused]] const asset_data::map_triggers& data)
		{
			std::size_t size = 0;
			if (data.models)
			{
				size += plain::rounded(std::max<std::size_t>(data.models->size(), 1) * sizeof(TriggerModel));
				for (const auto& item : *data.models)
				{
					size += size_trigger_model(item);
				}
			}
			if (data.hulls)
			{
				size += plain::rounded(std::max<std::size_t>(data.hulls->size(), 1) * sizeof(TriggerHull));
				for (const auto& item : *data.hulls)
				{
					size += size_trigger_hull(item);
				}
			}
			if (data.slabs)
			{
				size += plain::rounded(std::max<std::size_t>(data.slabs->size(), 1) * sizeof(TriggerSlab));
				for (const auto& item : *data.slabs)
				{
					size += size_trigger_slab(item);
				}
			}
			if (data.windings)
			{
				size += plain::rounded(std::max<std::size_t>(data.windings->size(), 1) * sizeof(TriggerWinding));
				for (const auto& item : *data.windings)
				{
					size += size_trigger_winding(item);
				}
			}
			if (data.winding_points)
			{
				size += plain::rounded(std::max<std::size_t>(data.winding_points->size(), 1) * sizeof(TriggerWindingPoint));
				for (const auto& item : *data.winding_points)
				{
					size += size_trigger_winding_point(item);
				}
			}
			return size;
		}

		asset_data::physics_capacities read_physics_capacities(const PhysicsCapacities& native)
		{
			asset_data::physics_capacities data;
			data.max_num_rigid_bodies_server = native.maxNumRigidBodiesServer;
			data.max_num_detail_rigid_bodies_server = native.maxNumDetailRigidBodiesServer;
			data.max_num_constraints_server = native.maxNumConstraintsServer;
			data.max_num_motions_server = native.maxNumMotionsServer;
			data.max_num_rigid_bodies_client = native.maxNumRigidBodiesClient;
			data.max_num_detail_rigid_bodies_client = native.maxNumDetailRigidBodiesClient;
			data.max_num_constraints_client = native.maxNumConstraintsClient;
			data.max_num_motions_client = native.maxNumMotionsClient;
			return data;
		}

		void write_physics_capacities(PhysicsCapacities& native, const asset_data::physics_capacities& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.maxNumRigidBodiesServer = data.max_num_rigid_bodies_server;
			native.maxNumDetailRigidBodiesServer = data.max_num_detail_rigid_bodies_server;
			native.maxNumConstraintsServer = data.max_num_constraints_server;
			native.maxNumMotionsServer = data.max_num_motions_server;
			native.maxNumRigidBodiesClient = data.max_num_rigid_bodies_client;
			native.maxNumDetailRigidBodiesClient = data.max_num_detail_rigid_bodies_client;
			native.maxNumConstraintsClient = data.max_num_constraints_client;
			native.maxNumMotionsClient = data.max_num_motions_client;
		}

		std::size_t size_physics_capacities([[maybe_unused]] const asset_data::physics_capacities& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::collision_heatmap_entry read_collision_heatmap_entry(const CollisionHeatmapEntry& native)
		{
			asset_data::collision_heatmap_entry data;
			data.vertex_count = native.vertexCount;
			data.vertex_density = native.vertexDensity;
			data.min_extent = plain::read_scalars<float, 3>(&native.minExtent);
			data.max_extent = plain::read_scalars<float, 3>(&native.maxExtent);
			return data;
		}

		void write_collision_heatmap_entry(CollisionHeatmapEntry& native, const asset_data::collision_heatmap_entry& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.vertexCount = data.vertex_count;
			native.vertexDensity = data.vertex_density;
			plain::write_scalars(&native.minExtent, data.min_extent);
			plain::write_scalars(&native.maxExtent, data.max_extent);
		}

		std::size_t size_collision_heatmap_entry([[maybe_unused]] const asset_data::collision_heatmap_entry& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::top_down_map_data_header read_top_down_map_data_header(const TopDownMapDataHeader& native)
		{
			asset_data::top_down_map_data_header data;
			data.version = native.version;
			data.samples_per_axis = native.samplesPerAxis;
			data.bottom_left = plain::read_scalars<float, 2>(&native.bottomLeft);
			data.top_right = plain::read_scalars<float, 2>(&native.topRight);
			return data;
		}

		void write_top_down_map_data_header(TopDownMapDataHeader& native, const asset_data::top_down_map_data_header& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.version = data.version;
			native.samplesPerAxis = data.samples_per_axis;
			plain::write_scalars(&native.bottomLeft, data.bottom_left);
			plain::write_scalars(&native.topRight, data.top_right);
		}

		std::size_t size_top_down_map_data_header([[maybe_unused]] const asset_data::top_down_map_data_header& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::top_down_map_data_content read_top_down_map_data_content(const TopDownMapDataContent& native)
		{
			asset_data::top_down_map_data_content data;
			data.nav_mesh_visible = std::to_array(native.navMeshVisible);
			return data;
		}

		void write_top_down_map_data_content(TopDownMapDataContent& native, const asset_data::top_down_map_data_content& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			std::ranges::copy(data.nav_mesh_visible, native.navMeshVisible);
		}

		std::size_t size_top_down_map_data_content([[maybe_unused]] const asset_data::top_down_map_data_content& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::top_down_map_data read_top_down_map_data(const TopDownMapData& native)
		{
			asset_data::top_down_map_data data;
			data.header = read_top_down_map_data_header(native.header);
			data.content = read_top_down_map_data_content(native.content);
			return data;
		}

		void write_top_down_map_data(TopDownMapData& native, const asset_data::top_down_map_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_top_down_map_data_header(native.header, data.header, resolve, memory);
			write_top_down_map_data_content(native.content, data.content, resolve, memory);
		}

		std::size_t size_top_down_map_data([[maybe_unused]] const asset_data::top_down_map_data& data)
		{
			std::size_t size = 0;
			size += size_top_down_map_data_header(data.header);
			size += size_top_down_map_data_content(data.content);
			return size;
		}

		asset_data::clip_map read_clip_map(const clipMap_t& native)
		{
			asset_data::clip_map data;
			data.name = plain::read_string(native.name);
			data.is_in_use = native.isInUse;
			data.static_model_collision_model_lists = plain::read_array(native.staticModelCollisionModelLists, native.numStaticModelCollisionModelLists, "clipMap_t.staticModelCollisionModelLists", [&](const auto& item) { return read_static_model_collision_model_list(item); });
			data.map_ents = plain::read_asset(ASSET_TYPE_MAP_ENTS, native.mapEnts);
			data.stages = plain::read_array(native.stages, native.stageCount, "clipMap_t.stages", [&](const auto& item) { return read_stage(item); });
			data.stage_trigger = read_map_triggers(native.stageTrigger);
			data.broadphase_min = plain::read_scalars<float, 3>(&native.broadphaseMin);
			data.broadphase_max = plain::read_scalars<float, 3>(&native.broadphaseMax);
			data.physics_capacities = read_physics_capacities(native.physicsCapacities);
			data.havok_world_shape_data = plain::read_array(native.havokWorldShapeData, native.havokWorldShapeDataSize, "clipMap_t.havokWorldShapeData", [&](const auto& item) { return item; });
			data.collision_heatmap = plain::read_array(native.collisionHeatmap, native.numCollisionHeatmapEntries, "clipMap_t.collisionHeatmap", [&](const auto& item) { return read_collision_heatmap_entry(item); });
			data.total_glass_init_piece_count = native.totalGlassInitPieceCount;
			data.total_glass_piece_limit = native.totalGlassPieceLimit;
			data.top_down_map_data = plain::read_single(native.topDownMapData, [&](const auto& item) { return read_top_down_map_data(item); });
			data.vision_set_name = plain::read_string(native.visionSetName);
			data.checksum = native.checksum;
			return data;
		}

		void write_clip_map(clipMap_t& native, const asset_data::clip_map& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.isInUse = data.is_in_use;
			native.staticModelCollisionModelLists = plain::write_array<StaticModelCollisionModelList>(data.static_model_collision_model_lists, memory, [&](auto& out, const auto& in) { write_static_model_collision_model_list(out, in, resolve, memory); });
			native.mapEnts = plain::resolve_asset<MapEnts>(resolve, ASSET_TYPE_MAP_ENTS, data.map_ents);
			native.stages = plain::write_array<Stage>(data.stages, memory, [&](auto& out, const auto& in) { write_stage(out, in, resolve, memory); });
			write_map_triggers(native.stageTrigger, data.stage_trigger, resolve, memory);
			plain::write_scalars(&native.broadphaseMin, data.broadphase_min);
			plain::write_scalars(&native.broadphaseMax, data.broadphase_max);
			write_physics_capacities(native.physicsCapacities, data.physics_capacities, resolve, memory);
			native.havokWorldShapeData = plain::write_array<char>(data.havok_world_shape_data, memory, [&](auto& out, const auto& in) { out = in; });
			native.collisionHeatmap = plain::write_array<CollisionHeatmapEntry>(data.collision_heatmap, memory, [&](auto& out, const auto& in) { write_collision_heatmap_entry(out, in, resolve, memory); });
			native.totalGlassInitPieceCount = data.total_glass_init_piece_count;
			native.totalGlassPieceLimit = data.total_glass_piece_limit;
			native.topDownMapData = plain::write_single<TopDownMapData>(data.top_down_map_data, memory, [&](auto& out, const auto& in) { write_top_down_map_data(out, in, resolve, memory); });
			native.visionSetName = plain::write_string(memory, data.vision_set_name);
			native.checksum = data.checksum;
			native.numStaticModelCollisionModelLists = plain::count_of<std::uint32_t>(data.static_model_collision_model_lists);
			native.stageCount = plain::count_of<std::uint8_t>(data.stages);
			native.havokWorldShapeDataSize = plain::count_of<std::uint32_t>(data.havok_world_shape_data);
			native.numCollisionHeatmapEntries = plain::count_of<std::uint32_t>(data.collision_heatmap);
		}

		std::size_t size_clip_map([[maybe_unused]] const asset_data::clip_map& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.static_model_collision_model_lists)
			{
				size += plain::rounded(std::max<std::size_t>(data.static_model_collision_model_lists->size(), 1) * sizeof(StaticModelCollisionModelList));
				for (const auto& item : *data.static_model_collision_model_lists)
				{
					size += size_static_model_collision_model_list(item);
				}
			}
			if (data.stages)
			{
				size += plain::rounded(std::max<std::size_t>(data.stages->size(), 1) * sizeof(Stage));
				for (const auto& item : *data.stages)
				{
					size += size_stage(item);
				}
			}
			size += size_map_triggers(data.stage_trigger);
			size += size_physics_capacities(data.physics_capacities);
			if (data.havok_world_shape_data)
			{
				size += plain::rounded(std::max<std::size_t>(data.havok_world_shape_data->size(), 1) * sizeof(char));
			}
			if (data.collision_heatmap)
			{
				size += plain::rounded(std::max<std::size_t>(data.collision_heatmap->size(), 1) * sizeof(CollisionHeatmapEntry));
				for (const auto& item : *data.collision_heatmap)
				{
					size += size_collision_heatmap_entry(item);
				}
			}
			if (data.top_down_map_data)
			{
				size += plain::rounded(sizeof(TopDownMapData));
				size += size_top_down_map_data(*data.top_down_map_data);
			}
			size += plain::string_size(data.vision_set_name);
			return size;
		}

		asset_data::client_trigger_model read_client_trigger_model(const ClientTriggerModel& native)
		{
			asset_data::client_trigger_model data;
			data.hull_count = native.hullCount;
			data.first_hull = native.firstHull;
			data.flags = native.flags;
			return data;
		}

		void write_client_trigger_model(ClientTriggerModel& native, const asset_data::client_trigger_model& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.hullCount = data.hull_count;
			native.firstHull = data.first_hull;
			native.flags = data.flags;
		}

		std::size_t size_client_trigger_model([[maybe_unused]] const asset_data::client_trigger_model& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::client_trigger_hull read_client_trigger_hull(const ClientTriggerHull& native)
		{
			asset_data::client_trigger_hull data;
			data.trigger_space_bounds = read_bounds(native.triggerSpaceBounds);
			data.slab_count = native.slabCount;
			data.first_slab = native.firstSlab;
			return data;
		}

		void write_client_trigger_hull(ClientTriggerHull& native, const asset_data::client_trigger_hull& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_bounds(native.triggerSpaceBounds, data.trigger_space_bounds, resolve, memory);
			native.slabCount = data.slab_count;
			native.firstSlab = data.first_slab;
		}

		std::size_t size_client_trigger_hull([[maybe_unused]] const asset_data::client_trigger_hull& data)
		{
			std::size_t size = 0;
			size += size_bounds(data.trigger_space_bounds);
			return size;
		}

		asset_data::client_map_triggers read_client_map_triggers(const ClientMapTriggers& native)
		{
			asset_data::client_map_triggers data;
			data.count = native.count;
			data.models = plain::read_array(native.models, native.count, "ClientMapTriggers.models", [&](const auto& item) { return read_client_trigger_model(item); });
			data.hulls = plain::read_array(native.hulls, native.hullCount, "ClientMapTriggers.hulls", [&](const auto& item) { return read_client_trigger_hull(item); });
			data.slabs = plain::read_array(native.slabs, native.slabCount, "ClientMapTriggers.slabs", [&](const auto& item) { return read_trigger_slab(item); });
			return data;
		}

		void write_client_map_triggers(ClientMapTriggers& native, const asset_data::client_map_triggers& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.count = data.count;
			native.models = plain::write_array<ClientTriggerModel>(data.models, memory, [&](auto& out, const auto& in) { write_client_trigger_model(out, in, resolve, memory); });
			native.hulls = plain::write_array<ClientTriggerHull>(data.hulls, memory, [&](auto& out, const auto& in) { write_client_trigger_hull(out, in, resolve, memory); });
			native.slabs = plain::write_array<TriggerSlab>(data.slabs, memory, [&](auto& out, const auto& in) { write_trigger_slab(out, in, resolve, memory); });
			native.hullCount = plain::count_of<std::uint32_t>(data.hulls);
			native.slabCount = plain::count_of<std::uint32_t>(data.slabs);
		}

		std::size_t size_client_map_triggers([[maybe_unused]] const asset_data::client_map_triggers& data)
		{
			std::size_t size = 0;
			if (data.models)
			{
				size += plain::rounded(std::max<std::size_t>(data.models->size(), 1) * sizeof(ClientTriggerModel));
				for (const auto& item : *data.models)
				{
					size += size_client_trigger_model(item);
				}
			}
			if (data.hulls)
			{
				size += plain::rounded(std::max<std::size_t>(data.hulls->size(), 1) * sizeof(ClientTriggerHull));
				for (const auto& item : *data.hulls)
				{
					size += size_client_trigger_hull(item);
				}
			}
			if (data.slabs)
			{
				size += plain::rounded(std::max<std::size_t>(data.slabs->size(), 1) * sizeof(TriggerSlab));
				for (const auto& item : *data.slabs)
				{
					size += size_trigger_slab(item);
				}
			}
			return size;
		}

		asset_data::ct_aud_rvb_pan_info read_ct_aud_rvb_pan_info(const CTAudRvbPanInfo& native)
		{
			asset_data::ct_aud_rvb_pan_info data;
			data.has_custom_position = native.hasCustomPosition;
			data.position = plain::read_scalars<float, 3>(&native.position);
			data.max_wall_distance = native.maxWallDistance;
			data.min_reverb_volume = native.minReverbVolume;
			return data;
		}

		void write_ct_aud_rvb_pan_info(CTAudRvbPanInfo& native, const asset_data::ct_aud_rvb_pan_info& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.hasCustomPosition = data.has_custom_position;
			plain::write_scalars(&native.position, data.position);
			native.maxWallDistance = data.max_wall_distance;
			native.minReverbVolume = data.min_reverb_volume;
		}

		std::size_t size_ct_aud_rvb_pan_info([[maybe_unused]] const asset_data::ct_aud_rvb_pan_info& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::client_triggers read_client_triggers(const ClientTriggers& native)
		{
			asset_data::client_triggers data;
			data.trigger = read_client_map_triggers(native.trigger);
			data.trigger_string = plain::read_array(native.triggerString, native.triggerStringLength, "ClientTriggers.triggerString", [&](const auto& item) { return item; });
			data.vision_set_triggers = plain::read_array(native.visionSetTriggers, native.trigger.count, "ClientTriggers.visionSetTriggers", [&](const auto& item) { return item; });
			data.trigger_type = plain::read_array(native.triggerType, native.trigger.count, "ClientTriggers.triggerType", [&](const auto& item) { return item; });
			data.origins = plain::read_array(native.origins, native.trigger.count, "ClientTriggers.origins", [&](const auto& item) { return plain::read_scalars<float, 3>(&item); });
			data.script_delay = plain::read_array(native.scriptDelay, native.trigger.count, "ClientTriggers.scriptDelay", [&](const auto& item) { return item; });
			data.priority = plain::read_array(native.priority, native.trigger.count, "ClientTriggers.priority", [&](const auto& item) { return item; });
			data.audio_triggers = plain::read_array(native.audioTriggers, native.trigger.count, "ClientTriggers.audioTriggers", [&](const auto& item) { return item; });
			data.blend_lookup = plain::read_array(native.blendLookup, native.trigger.count, "ClientTriggers.blendLookup", [&](const auto& item) { return item; });
			data.npc_triggers = plain::read_array(native.npcTriggers, native.trigger.count, "ClientTriggers.npcTriggers", [&](const auto& item) { return item; });
			data.audio_state_ids = plain::read_array(native.audioStateIds, native.trigger.count, "ClientTriggers.audioStateIds", [&](const auto& item) { return item; });
			plain::require_zero(native.detailSoundBank, "ClientTriggers.detailSoundBank");
			data.audio_rvb_pan_info = plain::read_array(native.audioRvbPanInfo, native.trigger.count, "ClientTriggers.audioRvbPanInfo", [&](const auto& item) { return read_ct_aud_rvb_pan_info(item); });
			data.parent = plain::read_array(native.parent, native.trigger.count, "ClientTriggers.parent", [&](const auto& item) { return item; });
			plain::require_zero(native.linkTo, "ClientTriggers.linkTo");
			data.spatial_tree = plain::read_single(native.spatialTree, [&](const auto& item) { return read_spatial_partition_tree(item); });
			return data;
		}

		void write_client_triggers(ClientTriggers& native, const asset_data::client_triggers& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_client_map_triggers(native.trigger, data.trigger, resolve, memory);
			native.triggerString = plain::write_array<char>(data.trigger_string, memory, [&](auto& out, const auto& in) { out = in; });
			native.visionSetTriggers = plain::write_array<std::int16_t>(data.vision_set_triggers, memory, [&](auto& out, const auto& in) { out = in; });
			native.triggerType = plain::write_array<std::uint16_t>(data.trigger_type, memory, [&](auto& out, const auto& in) { out = in; });
			native.origins = plain::write_array<vec3_t>(data.origins, memory, [&](auto& out, const auto& in) { plain::write_scalars(&out, in); });
			native.scriptDelay = plain::write_array<float>(data.script_delay, memory, [&](auto& out, const auto& in) { out = in; });
			native.priority = plain::write_array<float>(data.priority, memory, [&](auto& out, const auto& in) { out = in; });
			native.audioTriggers = plain::write_array<std::int16_t>(data.audio_triggers, memory, [&](auto& out, const auto& in) { out = in; });
			native.blendLookup = plain::write_array<std::int16_t>(data.blend_lookup, memory, [&](auto& out, const auto& in) { out = in; });
			native.npcTriggers = plain::write_array<std::int16_t>(data.npc_triggers, memory, [&](auto& out, const auto& in) { out = in; });
			native.audioStateIds = plain::write_array<std::int16_t>(data.audio_state_ids, memory, [&](auto& out, const auto& in) { out = in; });
			native.audioRvbPanInfo = plain::write_array<CTAudRvbPanInfo>(data.audio_rvb_pan_info, memory, [&](auto& out, const auto& in) { write_ct_aud_rvb_pan_info(out, in, resolve, memory); });
			native.parent = plain::write_array<std::int16_t>(data.parent, memory, [&](auto& out, const auto& in) { out = in; });
			native.spatialTree = plain::write_single<SpatialPartition_Tree>(data.spatial_tree, memory, [&](auto& out, const auto& in) { write_spatial_partition_tree(out, in, resolve, memory); });
			native.triggerStringLength = plain::count_of<std::uint32_t>(data.trigger_string);
		}

		std::size_t size_client_triggers([[maybe_unused]] const asset_data::client_triggers& data)
		{
			std::size_t size = 0;
			size += size_client_map_triggers(data.trigger);
			if (data.trigger_string)
			{
				size += plain::rounded(std::max<std::size_t>(data.trigger_string->size(), 1) * sizeof(char));
			}
			if (data.vision_set_triggers)
			{
				size += plain::rounded(std::max<std::size_t>(data.vision_set_triggers->size(), 1) * sizeof(std::int16_t));
			}
			if (data.trigger_type)
			{
				size += plain::rounded(std::max<std::size_t>(data.trigger_type->size(), 1) * sizeof(std::uint16_t));
			}
			if (data.origins)
			{
				size += plain::rounded(std::max<std::size_t>(data.origins->size(), 1) * sizeof(vec3_t));
			}
			if (data.script_delay)
			{
				size += plain::rounded(std::max<std::size_t>(data.script_delay->size(), 1) * sizeof(float));
			}
			if (data.priority)
			{
				size += plain::rounded(std::max<std::size_t>(data.priority->size(), 1) * sizeof(float));
			}
			if (data.audio_triggers)
			{
				size += plain::rounded(std::max<std::size_t>(data.audio_triggers->size(), 1) * sizeof(std::int16_t));
			}
			if (data.blend_lookup)
			{
				size += plain::rounded(std::max<std::size_t>(data.blend_lookup->size(), 1) * sizeof(std::int16_t));
			}
			if (data.npc_triggers)
			{
				size += plain::rounded(std::max<std::size_t>(data.npc_triggers->size(), 1) * sizeof(std::int16_t));
			}
			if (data.audio_state_ids)
			{
				size += plain::rounded(std::max<std::size_t>(data.audio_state_ids->size(), 1) * sizeof(std::int16_t));
			}
			if (data.audio_rvb_pan_info)
			{
				size += plain::rounded(std::max<std::size_t>(data.audio_rvb_pan_info->size(), 1) * sizeof(CTAudRvbPanInfo));
				for (const auto& item : *data.audio_rvb_pan_info)
				{
					size += size_ct_aud_rvb_pan_info(item);
				}
			}
			if (data.parent)
			{
				size += plain::rounded(std::max<std::size_t>(data.parent->size(), 1) * sizeof(std::int16_t));
			}
			if (data.spatial_tree)
			{
				size += plain::rounded(sizeof(SpatialPartition_Tree));
				size += size_spatial_partition_tree(*data.spatial_tree);
			}
			return size;
		}

		asset_data::client_trigger_blend_node read_client_trigger_blend_node(const ClientTriggerBlendNode& native)
		{
			asset_data::client_trigger_blend_node data;
			data.point_a = plain::read_scalars<float, 3>(&native.pointA);
			data.point_b = plain::read_scalars<float, 3>(&native.pointB);
			data.trigger_a = native.triggerA;
			data.trigger_b = native.triggerB;
			return data;
		}

		void write_client_trigger_blend_node(ClientTriggerBlendNode& native, const asset_data::client_trigger_blend_node& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.pointA, data.point_a);
			plain::write_scalars(&native.pointB, data.point_b);
			native.triggerA = data.trigger_a;
			native.triggerB = data.trigger_b;
		}

		std::size_t size_client_trigger_blend_node([[maybe_unused]] const asset_data::client_trigger_blend_node& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::client_trigger_blend read_client_trigger_blend(const ClientTriggerBlend& native)
		{
			asset_data::client_trigger_blend data;
			data.num_client_trigger_blend_nodes = native.numClientTriggerBlendNodes;
			data.blend_nodes = plain::read_array(native.blendNodes, native.numClientTriggerBlendNodes, "ClientTriggerBlend.blendNodes", [&](const auto& item) { return read_client_trigger_blend_node(item); });
			return data;
		}

		void write_client_trigger_blend(ClientTriggerBlend& native, const asset_data::client_trigger_blend& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.numClientTriggerBlendNodes = data.num_client_trigger_blend_nodes;
			native.blendNodes = plain::write_array<ClientTriggerBlendNode>(data.blend_nodes, memory, [&](auto& out, const auto& in) { write_client_trigger_blend_node(out, in, resolve, memory); });
		}

		std::size_t size_client_trigger_blend([[maybe_unused]] const asset_data::client_trigger_blend& data)
		{
			std::size_t size = 0;
			if (data.blend_nodes)
			{
				size += plain::rounded(std::max<std::size_t>(data.blend_nodes->size(), 1) * sizeof(ClientTriggerBlendNode));
				for (const auto& item : *data.blend_nodes)
				{
					size += size_client_trigger_blend_node(item);
				}
			}
			return size;
		}

		asset_data::spawn_point_entity_record read_spawn_point_entity_record(const SpawnPointEntityRecord& native)
		{
			asset_data::spawn_point_entity_record data;
			data.index = native.index;
			data.name = plain::read_script_string(native.name);
			data.target = plain::read_script_string(native.target);
			data.script_noteworthy = plain::read_script_string(native.script_noteworthy);
			data.origin = plain::read_scalars<float, 3>(&native.origin);
			data.angles = plain::read_scalars<float, 3>(&native.angles);
			return data;
		}

		void write_spawn_point_entity_record(SpawnPointEntityRecord& native, const asset_data::spawn_point_entity_record& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.index = data.index;
			native.name = static_cast<scr_string_t>(plain::write_script_string(data.name));
			native.target = static_cast<scr_string_t>(plain::write_script_string(data.target));
			native.script_noteworthy = static_cast<scr_string_t>(plain::write_script_string(data.script_noteworthy));
			plain::write_scalars(&native.origin, data.origin);
			plain::write_scalars(&native.angles, data.angles);
		}

		std::size_t size_spawn_point_entity_record([[maybe_unused]] const asset_data::spawn_point_entity_record& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::spawn_point_record_list read_spawn_point_record_list(const SpawnPointRecordList& native)
		{
			asset_data::spawn_point_record_list data;
			data.spawns_count = native.spawnsCount;
			data.spawns = plain::read_array(native.spawns, native.spawnsCount, "SpawnPointRecordList.spawns", [&](const auto& item) { return read_spawn_point_entity_record(item); });
			return data;
		}

		void write_spawn_point_record_list(SpawnPointRecordList& native, const asset_data::spawn_point_record_list& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.spawnsCount = data.spawns_count;
			native.spawns = plain::write_array<SpawnPointEntityRecord>(data.spawns, memory, [&](auto& out, const auto& in) { write_spawn_point_entity_record(out, in, resolve, memory); });
		}

		std::size_t size_spawn_point_record_list([[maybe_unused]] const asset_data::spawn_point_record_list& data)
		{
			std::size_t size = 0;
			if (data.spawns)
			{
				size += plain::rounded(std::max<std::size_t>(data.spawns->size(), 1) * sizeof(SpawnPointEntityRecord));
				for (const auto& item : *data.spawns)
				{
					size += size_spawn_point_entity_record(item);
				}
			}
			return size;
		}

		asset_data::spline_point_entity_record read_spline_point_entity_record(const SplinePointEntityRecord& native)
		{
			asset_data::spline_point_entity_record data;
			data.spline_id = native.splineId;
			data.spline_node_id = native.splineNodeId;
			data.spline_node_label = plain::read_script_string(native.splineNodeLabel);
			data.targetname = plain::read_script_string(native.targetname);
			data.target = plain::read_script_string(native.target);
			data.string = plain::read_script_string(native.string);
			data.speed = native.speed;
			data.spline_node_tension = native.splineNodeTension;
			data.origin = plain::read_scalars<float, 3>(&native.origin);
			data.angles = plain::read_scalars<float, 3>(&native.angles);
			data.throttle = native.throttle;
			data.corridor_dims = plain::read_scalars<float, 2>(&native.corridorDims);
			data.tangent = plain::read_scalars<float, 3>(&native.tangent);
			data.dist_to_next_node = native.distToNextNode;
			data.position_cubic = [&] { std::array<std::array<float, 3>, 4> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = plain::read_scalars<float, 3>(&native.positionCubic[i]); } return out; }();
			data.tangent_quadratic = [&] { std::array<std::array<float, 3>, 3> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = plain::read_scalars<float, 3>(&native.tangentQuadratic[i]); } return out; }();
			return data;
		}

		void write_spline_point_entity_record(SplinePointEntityRecord& native, const asset_data::spline_point_entity_record& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.splineId = data.spline_id;
			native.splineNodeId = data.spline_node_id;
			native.splineNodeLabel = static_cast<scr_string_t>(plain::write_script_string(data.spline_node_label));
			native.targetname = static_cast<scr_string_t>(plain::write_script_string(data.targetname));
			native.target = static_cast<scr_string_t>(plain::write_script_string(data.target));
			native.string = static_cast<scr_string_t>(plain::write_script_string(data.string));
			native.speed = data.speed;
			native.splineNodeTension = data.spline_node_tension;
			plain::write_scalars(&native.origin, data.origin);
			plain::write_scalars(&native.angles, data.angles);
			native.throttle = data.throttle;
			plain::write_scalars(&native.corridorDims, data.corridor_dims);
			plain::write_scalars(&native.tangent, data.tangent);
			native.distToNextNode = data.dist_to_next_node;
			for (std::size_t i = 0; i < data.position_cubic.size(); ++i)
			{
				plain::write_scalars(&native.positionCubic[i], data.position_cubic[i]);
			}
			for (std::size_t i = 0; i < data.tangent_quadratic.size(); ++i)
			{
				plain::write_scalars(&native.tangentQuadratic[i], data.tangent_quadratic[i]);
			}
		}

		std::size_t size_spline_point_entity_record([[maybe_unused]] const asset_data::spline_point_entity_record& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::spline_point_record_list read_spline_point_record_list(const SplinePointRecordList& native)
		{
			asset_data::spline_point_record_list data;
			data.spline_point_count = native.splinePointCount;
			data.spline_length = native.splineLength;
			data.spline_points = plain::read_array(native.splinePoints, native.splinePointCount, "SplinePointRecordList.splinePoints", [&](const auto& item) { return read_spline_point_entity_record(item); });
			return data;
		}

		void write_spline_point_record_list(SplinePointRecordList& native, const asset_data::spline_point_record_list& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.splinePointCount = data.spline_point_count;
			native.splineLength = data.spline_length;
			native.splinePoints = plain::write_array<SplinePointEntityRecord>(data.spline_points, memory, [&](auto& out, const auto& in) { write_spline_point_entity_record(out, in, resolve, memory); });
		}

		std::size_t size_spline_point_record_list([[maybe_unused]] const asset_data::spline_point_record_list& data)
		{
			std::size_t size = 0;
			if (data.spline_points)
			{
				size += plain::rounded(std::max<std::size_t>(data.spline_points->size(), 1) * sizeof(SplinePointEntityRecord));
				for (const auto& item : *data.spline_points)
				{
					size += size_spline_point_entity_record(item);
				}
			}
			return size;
		}

		asset_data::spline_record_list read_spline_record_list(const SplineRecordList& native)
		{
			asset_data::spline_record_list data;
			data.spline_count = native.splineCount;
			data.splines = plain::read_array(native.splines, native.splineCount, "SplineRecordList.splines", [&](const auto& item) { return read_spline_point_record_list(item); });
			return data;
		}

		void write_spline_record_list(SplineRecordList& native, const asset_data::spline_record_list& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.splineCount = data.spline_count;
			native.splines = plain::write_array<SplinePointRecordList>(data.splines, memory, [&](auto& out, const auto& in) { write_spline_point_record_list(out, in, resolve, memory); });
		}

		std::size_t size_spline_record_list([[maybe_unused]] const asset_data::spline_record_list& data)
		{
			std::size_t size = 0;
			if (data.splines)
			{
				size += plain::rounded(std::max<std::size_t>(data.splines->size(), 1) * sizeof(SplinePointRecordList));
				for (const auto& item : *data.splines)
				{
					size += size_spline_point_record_list(item);
				}
			}
			return size;
		}

		asset_data::cmodel_t read_cmodel_t(const cmodel_t& native)
		{
			asset_data::cmodel_t data;
			data.bounds = read_bounds(native.bounds);
			data.radius = native.radius;
			data.physics_asset = plain::read_asset(ASSET_TYPE_PHYSICSASSET, native.physicsAsset);
			data.physics_shape_override_idx = native.physicsShapeOverrideIdx;
			data.nav_obstacle_idx = native.navObstacleIdx;
			data.edge_first_index = native.edgeFirstIndex;
			data.edge_total_count = native.edgeTotalCount;
			return data;
		}

		void write_cmodel_t(cmodel_t& native, const asset_data::cmodel_t& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_bounds(native.bounds, data.bounds, resolve, memory);
			native.radius = data.radius;
			native.physicsAsset = plain::resolve_asset<PhysicsAsset>(resolve, ASSET_TYPE_PHYSICSASSET, data.physics_asset);
			native.physicsShapeOverrideIdx = data.physics_shape_override_idx;
			native.navObstacleIdx = data.nav_obstacle_idx;
			native.edgeFirstIndex = data.edge_first_index;
			native.edgeTotalCount = data.edge_total_count;
		}

		std::size_t size_cmodel_t([[maybe_unused]] const asset_data::cmodel_t& data)
		{
			std::size_t size = 0;
			size += size_bounds(data.bounds);
			return size;
		}

		asset_data::cm_client_model read_cm_client_model(const CM_ClientModel& native)
		{
			asset_data::cm_client_model data;
			data.spawn_origin = plain::read_scalars<float, 3>(&native.spawnOrigin);
			data.spawn_angles = plain::read_scalars<float, 3>(&native.spawnAngles);
			data.model = plain::read_asset(ASSET_TYPE_XMODEL, native.model);
			data.animation_tree_name = plain::read_script_string(native.animationTreeName);
			data.animation = plain::read_asset(ASSET_TYPE_XANIMPARTS, native.animation);
			data.name = plain::read_script_string(native.name);
			data.no_physics = native.noPhysics;
			data.no_cloth = native.noCloth;
			return data;
		}

		void write_cm_client_model(CM_ClientModel& native, const asset_data::cm_client_model& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.spawnOrigin, data.spawn_origin);
			plain::write_scalars(&native.spawnAngles, data.spawn_angles);
			native.model = plain::resolve_asset<XModel>(resolve, ASSET_TYPE_XMODEL, data.model);
			native.animationTreeName = static_cast<scr_string_t>(plain::write_script_string(data.animation_tree_name));
			native.animation = plain::resolve_asset<XAnimParts>(resolve, ASSET_TYPE_XANIMPARTS, data.animation);
			native.name = static_cast<scr_string_t>(plain::write_script_string(data.name));
			native.noPhysics = data.no_physics;
			native.noCloth = data.no_cloth;
		}

		std::size_t size_cm_client_model([[maybe_unused]] const asset_data::cm_client_model& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::spatial_partition_population_node read_spatial_partition_population_node(const SpatialPartition_Population_Node& native)
		{
			asset_data::spatial_partition_population_node data;
			data.spatial_node_next = native.spatialNodeNext;
			return data;
		}

		void write_spatial_partition_population_node(SpatialPartition_Population_Node& native, const asset_data::spatial_partition_population_node& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.spatialNodeNext = data.spatial_node_next;
		}

		std::size_t size_spatial_partition_population_node([[maybe_unused]] const asset_data::spatial_partition_population_node& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::client_ent_anchor read_client_ent_anchor(const ClientEntAnchor& native)
		{
			asset_data::client_ent_anchor data;
			data.name = plain::read_script_string(native.name);
			data.ent_num = native.entNum;
			return data;
		}

		void write_client_ent_anchor(ClientEntAnchor& native, const asset_data::client_ent_anchor& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = static_cast<scr_string_t>(plain::write_script_string(data.name));
			native.entNum = data.ent_num;
		}

		std::size_t size_client_ent_anchor([[maybe_unused]] const asset_data::client_ent_anchor& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::scriptable_physics_limits read_scriptable_physics_limits(const ScriptablePhysicsLimits& native)
		{
			asset_data::scriptable_physics_limits data;
			data.client_standalone_main_rigid_body_count = native.clientStandaloneMainRigidBodyCount;
			data.client_standalone_detailed_rigid_body_count = native.clientStandaloneDetailedRigidBodyCount;
			data.client_standalone_motion_count = native.clientStandaloneMotionCount;
			data.server_standalone_main_rigid_body_count = native.serverStandaloneMainRigidBodyCount;
			data.server_standalone_detailed_rigid_body_count = native.serverStandaloneDetailedRigidBodyCount;
			data.server_standalone_motion_count = native.serverStandaloneMotionCount;
			return data;
		}

		void write_scriptable_physics_limits(ScriptablePhysicsLimits& native, const asset_data::scriptable_physics_limits& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.clientStandaloneMainRigidBodyCount = data.client_standalone_main_rigid_body_count;
			native.clientStandaloneDetailedRigidBodyCount = data.client_standalone_detailed_rigid_body_count;
			native.clientStandaloneMotionCount = data.client_standalone_motion_count;
			native.serverStandaloneMainRigidBodyCount = data.server_standalone_main_rigid_body_count;
			native.serverStandaloneDetailedRigidBodyCount = data.server_standalone_detailed_rigid_body_count;
			native.serverStandaloneMotionCount = data.server_standalone_motion_count;
		}

		std::size_t size_scriptable_physics_limits([[maybe_unused]] const asset_data::scriptable_physics_limits& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::scriptable_instance read_scriptable_instance(const ScriptableInstance& native)
		{
			asset_data::scriptable_instance data;
			data.initial_def = plain::read_asset(ASSET_TYPE_SCRIPTABLE, native.initialDef);
			data.initial_origin = plain::read_scalars<float, 3>(&native.initialOrigin);
			data.initial_angles = plain::read_scalars<float, 3>(&native.initialAngles);
			data.classname = plain::read_script_string(native.classname);
			data.target = plain::read_script_string(native.target);
			data.targetname = plain::read_script_string(native.targetname);
			data.script_linkname = plain::read_script_string(native.script_linkname);
			data.script_linkto = plain::read_script_string(native.script_linkto);
			data.script_noteworthy = plain::read_script_string(native.script_noteworthy);
			data.light_indices = std::to_array(native.lightIndices);
			data.ff_mem_cost = native.ffMemCost;
			data.map_ent_lookup = native.mapEntLookup;
			data.flags = native.flags;
			plain::require_zero(native.padding, "ScriptableInstance.padding");
			return data;
		}

		void write_scriptable_instance(ScriptableInstance& native, const asset_data::scriptable_instance& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.initialDef = plain::resolve_asset<ScriptableDef>(resolve, ASSET_TYPE_SCRIPTABLE, data.initial_def);
			plain::write_scalars(&native.initialOrigin, data.initial_origin);
			plain::write_scalars(&native.initialAngles, data.initial_angles);
			native.classname = static_cast<scr_string_t>(plain::write_script_string(data.classname));
			native.target = static_cast<scr_string_t>(plain::write_script_string(data.target));
			native.targetname = static_cast<scr_string_t>(plain::write_script_string(data.targetname));
			native.script_linkname = static_cast<scr_string_t>(plain::write_script_string(data.script_linkname));
			native.script_linkto = static_cast<scr_string_t>(plain::write_script_string(data.script_linkto));
			native.script_noteworthy = static_cast<scr_string_t>(plain::write_script_string(data.script_noteworthy));
			std::ranges::copy(data.light_indices, native.lightIndices);
			native.ffMemCost = data.ff_mem_cost;
			native.mapEntLookup = data.map_ent_lookup;
			native.flags = data.flags;
		}

		std::size_t size_scriptable_instance([[maybe_unused]] const asset_data::scriptable_instance& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::scriptable_event_sun_data read_scriptable_event_sun_data(const Scriptable_EventSun_Data& native)
		{
			asset_data::scriptable_event_sun_data data;
			data.start_time = native.startTime;
			data.trans_time = native.transTime;
			data.start_color_linear_srgb = plain::read_scalars<float, 3>(&native.startColorLinearSrgb);
			data.start_intensity = native.startIntensity;
			data.target_intensity = native.targetIntensity;
			data.start_angles = plain::read_scalars<float, 3>(&native.startAngles);
			data.target_angles = plain::read_scalars<float, 3>(&native.targetAngles);
			return data;
		}

		void write_scriptable_event_sun_data(Scriptable_EventSun_Data& native, const asset_data::scriptable_event_sun_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.startTime = data.start_time;
			native.transTime = data.trans_time;
			plain::write_scalars(&native.startColorLinearSrgb, data.start_color_linear_srgb);
			native.startIntensity = data.start_intensity;
			native.targetIntensity = data.target_intensity;
			plain::write_scalars(&native.startAngles, data.start_angles);
			plain::write_scalars(&native.targetAngles, data.target_angles);
		}

		std::size_t size_scriptable_event_sun_data([[maybe_unused]] const asset_data::scriptable_event_sun_data& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::scriptable_reserved_dynent read_scriptable_reserved_dynent(const ScriptableReservedDynent& native)
		{
			asset_data::scriptable_reserved_dynent data;
			data.dynent_id = native.dynentId;
			data.next = native.next;
			return data;
		}

		void write_scriptable_reserved_dynent(ScriptableReservedDynent& native, const asset_data::scriptable_reserved_dynent& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.dynentId = data.dynent_id;
			native.next = data.next;
		}

		std::size_t size_scriptable_reserved_dynent([[maybe_unused]] const asset_data::scriptable_reserved_dynent& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::scriptable_reserved_dynents read_scriptable_reserved_dynents(const ScriptableReservedDynents& native)
		{
			asset_data::scriptable_reserved_dynents data;
			data.free_head = native.freeHead;
			data.reserved_dynents = plain::read_array(native.reservedDynents, native.numReservedDynents, "ScriptableReservedDynents.reservedDynents", [&](const auto& item) { return read_scriptable_reserved_dynent(item); });
			return data;
		}

		void write_scriptable_reserved_dynents(ScriptableReservedDynents& native, const asset_data::scriptable_reserved_dynents& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.freeHead = data.free_head;
			native.reservedDynents = plain::write_array<ScriptableReservedDynent>(data.reserved_dynents, memory, [&](auto& out, const auto& in) { write_scriptable_reserved_dynent(out, in, resolve, memory); });
			native.numReservedDynents = plain::count_of<std::uint32_t>(data.reserved_dynents);
		}

		std::size_t size_scriptable_reserved_dynents([[maybe_unused]] const asset_data::scriptable_reserved_dynents& data)
		{
			std::size_t size = 0;
			if (data.reserved_dynents)
			{
				size += plain::rounded(std::max<std::size_t>(data.reserved_dynents->size(), 1) * sizeof(ScriptableReservedDynent));
				for (const auto& item : *data.reserved_dynents)
				{
					size += size_scriptable_reserved_dynent(item);
				}
			}
			return size;
		}

		asset_data::spatial_partition_population_partition read_spatial_partition_population_partition(const SpatialPartition_Population_Partition& native)
		{
			asset_data::spatial_partition_population_partition data;
			data.dist = native.dist;
			data.axis = native.axis;
			data.right_child = native.rightChild;
			return data;
		}

		void write_spatial_partition_population_partition(SpatialPartition_Population_Partition& native, const asset_data::spatial_partition_population_partition& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.dist = data.dist;
			native.axis = data.axis;
			native.rightChild = data.right_child;
		}

		std::size_t size_spatial_partition_population_partition([[maybe_unused]] const asset_data::spatial_partition_population_partition& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::extent_bounds read_extent_bounds(const ExtentBounds& native)
		{
			asset_data::extent_bounds data;
			data.mins = plain::read_scalars<float, 3>(&native.mins);
			data.maxs = plain::read_scalars<float, 3>(&native.maxs);
			return data;
		}

		void write_extent_bounds(ExtentBounds& native, const asset_data::extent_bounds& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.mins, data.mins);
			plain::write_scalars(&native.maxs, data.maxs);
		}

		std::size_t size_extent_bounds([[maybe_unused]] const asset_data::extent_bounds& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::spatial_partition_population_tree read_spatial_partition_population_tree(const SpatialPartition_Population_Tree& native)
		{
			asset_data::spatial_partition_population_tree data;
			data.partitions = plain::read_array(native.partitions, native.partitionCount, "SpatialPartition_Population_Tree.partitions", [&](const auto& item) { return read_spatial_partition_population_partition(item); });
			data.extents = plain::read_array(native.extents, native.partitionCount, "SpatialPartition_Population_Tree.extents", [&](const auto& item) { return read_extent_bounds(item); });
			return data;
		}

		void write_spatial_partition_population_tree(SpatialPartition_Population_Tree& native, const asset_data::spatial_partition_population_tree& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.partitions = plain::write_array<SpatialPartition_Population_Partition>(data.partitions, memory, [&](auto& out, const auto& in) { write_spatial_partition_population_partition(out, in, resolve, memory); });
			native.extents = plain::write_array<ExtentBounds>(data.extents, memory, [&](auto& out, const auto& in) { write_extent_bounds(out, in, resolve, memory); });
			native.partitionCount = plain::count_of<std::uint32_t>(data.partitions);
		}

		std::size_t size_spatial_partition_population_tree([[maybe_unused]] const asset_data::spatial_partition_population_tree& data)
		{
			std::size_t size = 0;
			if (data.partitions)
			{
				size += plain::rounded(std::max<std::size_t>(data.partitions->size(), 1) * sizeof(SpatialPartition_Population_Partition));
				for (const auto& item : *data.partitions)
				{
					size += size_spatial_partition_population_partition(item);
				}
			}
			if (data.extents)
			{
				size += plain::rounded(std::max<std::size_t>(data.extents->size(), 1) * sizeof(ExtentBounds));
				for (const auto& item : *data.extents)
				{
					size += size_extent_bounds(item);
				}
			}
			return size;
		}

		asset_data::scriptable_map_ents read_scriptable_map_ents(const ScriptableMapEnts& native)
		{
			asset_data::scriptable_map_ents data;
			data.total_instance_count = native.totalInstanceCount;
			data.reserved_instance_count = native.reservedInstanceCount;
			data.loot_instance_count = native.lootInstanceCount;
			data.server_instance_count = native.serverInstanceCount;
			data.server_fixed_part_count = native.serverFixedPartCount;
			data.server_usable_part_count = native.serverUsablePartCount;
			data.physics_limits = read_scriptable_physics_limits(native.physicsLimits);
			data.map_instances = plain::read_array(native.mapInstances, native.mapInstanceCount, "ScriptableMapEnts.mapInstances", [&](const auto& item) { return read_scriptable_instance(item); });
			data.sun_client_datas = [&] { std::array<asset_data::scriptable_event_sun_data, 2> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_scriptable_event_sun_data(native.sunClientDatas[i]); } return out; }();
			data.reserved_dynents = [&] { std::array<asset_data::scriptable_reserved_dynents, 2> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_scriptable_reserved_dynents(native.reservedDynents[i]); } return out; }();
			data.spatial_population_tree = plain::read_single(native.spatialPopulationTree, [&](const auto& item) { return read_spatial_partition_population_tree(item); });
			data.ff_mem_cost = native.ffMemCost;
			return data;
		}

		void write_scriptable_map_ents(ScriptableMapEnts& native, const asset_data::scriptable_map_ents& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.totalInstanceCount = data.total_instance_count;
			native.reservedInstanceCount = data.reserved_instance_count;
			native.lootInstanceCount = data.loot_instance_count;
			native.serverInstanceCount = data.server_instance_count;
			native.serverFixedPartCount = data.server_fixed_part_count;
			native.serverUsablePartCount = data.server_usable_part_count;
			write_scriptable_physics_limits(native.physicsLimits, data.physics_limits, resolve, memory);
			native.mapInstances = plain::write_array<ScriptableInstance>(data.map_instances, memory, [&](auto& out, const auto& in) { write_scriptable_instance(out, in, resolve, memory); });
			for (std::size_t i = 0; i < data.sun_client_datas.size(); ++i)
			{
				write_scriptable_event_sun_data(native.sunClientDatas[i], data.sun_client_datas[i], resolve, memory);
			}
			for (std::size_t i = 0; i < data.reserved_dynents.size(); ++i)
			{
				write_scriptable_reserved_dynents(native.reservedDynents[i], data.reserved_dynents[i], resolve, memory);
			}
			native.spatialPopulationTree = plain::write_single<SpatialPartition_Population_Tree>(data.spatial_population_tree, memory, [&](auto& out, const auto& in) { write_spatial_partition_population_tree(out, in, resolve, memory); });
			native.ffMemCost = data.ff_mem_cost;
			native.mapInstanceCount = plain::count_of<std::uint32_t>(data.map_instances);
		}

		std::size_t size_scriptable_map_ents([[maybe_unused]] const asset_data::scriptable_map_ents& data)
		{
			std::size_t size = 0;
			size += size_scriptable_physics_limits(data.physics_limits);
			if (data.map_instances)
			{
				size += plain::rounded(std::max<std::size_t>(data.map_instances->size(), 1) * sizeof(ScriptableInstance));
				for (const auto& item : *data.map_instances)
				{
					size += size_scriptable_instance(item);
				}
			}
			for (const auto& item : data.sun_client_datas)
			{
				size += size_scriptable_event_sun_data(item);
			}
			for (const auto& item : data.reserved_dynents)
			{
				size += size_scriptable_reserved_dynents(item);
			}
			if (data.spatial_population_tree)
			{
				size += plain::rounded(sizeof(SpatialPartition_Population_Tree));
				size += size_spatial_partition_population_tree(*data.spatial_population_tree);
			}
			return size;
		}

		asset_data::spawn_group_point read_spawn_group_point(const SpawnGroupPoint& native)
		{
			asset_data::spawn_group_point data;
			data.origin = plain::read_scalars<float, 3>(&native.origin);
			data.angles = plain::read_scalars<float, 3>(&native.angles);
			data.cluster = native.cluster;
			data.zone = native.zone;
			return data;
		}

		void write_spawn_group_point(SpawnGroupPoint& native, const asset_data::spawn_group_point& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.origin, data.origin);
			plain::write_scalars(&native.angles, data.angles);
			native.cluster = data.cluster;
			native.zone = data.zone;
		}

		std::size_t size_spawn_group_point([[maybe_unused]] const asset_data::spawn_group_point& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::spawn_group_cluster read_spawn_group_cluster(const SpawnGroupCluster& native)
		{
			asset_data::spawn_group_cluster data;
			data.zone = native.zone;
			data.points = plain::read_array(native.points, native.pointCount, "SpawnGroupCluster.points", [&](const auto& item) { return item; });
			data.bounds = read_bounds(native.bounds);
			return data;
		}

		void write_spawn_group_cluster(SpawnGroupCluster& native, const asset_data::spawn_group_cluster& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.zone = data.zone;
			native.points = plain::write_array<std::uint16_t>(data.points, memory, [&](auto& out, const auto& in) { out = in; });
			write_bounds(native.bounds, data.bounds, resolve, memory);
			native.pointCount = plain::count_of<std::uint16_t>(data.points);
		}

		std::size_t size_spawn_group_cluster([[maybe_unused]] const asset_data::spawn_group_cluster& data)
		{
			std::size_t size = 0;
			if (data.points)
			{
				size += plain::rounded(std::max<std::size_t>(data.points->size(), 1) * sizeof(std::uint16_t));
			}
			size += size_bounds(data.bounds);
			return size;
		}

		asset_data::spawn_group_zone read_spawn_group_zone(const SpawnGroupZone& native)
		{
			asset_data::spawn_group_zone data;
			data.name = plain::read_script_string(native.name);
			data.points = plain::read_array(native.points, native.pointCount, "SpawnGroupZone.points", [&](const auto& item) { return item; });
			data.clusters = plain::read_array(native.clusters, native.clusterCount, "SpawnGroupZone.clusters", [&](const auto& item) { return item; });
			data.bounds = read_bounds(native.bounds);
			return data;
		}

		void write_spawn_group_zone(SpawnGroupZone& native, const asset_data::spawn_group_zone& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = static_cast<scr_string_t>(plain::write_script_string(data.name));
			native.points = plain::write_array<std::uint16_t>(data.points, memory, [&](auto& out, const auto& in) { out = in; });
			native.clusters = plain::write_array<std::uint16_t>(data.clusters, memory, [&](auto& out, const auto& in) { out = in; });
			write_bounds(native.bounds, data.bounds, resolve, memory);
			native.pointCount = plain::count_of<std::uint16_t>(data.points);
			native.clusterCount = plain::count_of<std::uint16_t>(data.clusters);
		}

		std::size_t size_spawn_group_zone([[maybe_unused]] const asset_data::spawn_group_zone& data)
		{
			std::size_t size = 0;
			if (data.points)
			{
				size += plain::rounded(std::max<std::size_t>(data.points->size(), 1) * sizeof(std::uint16_t));
			}
			if (data.clusters)
			{
				size += plain::rounded(std::max<std::size_t>(data.clusters->size(), 1) * sizeof(std::uint16_t));
			}
			size += size_bounds(data.bounds);
			return size;
		}

		asset_data::spawn_group read_spawn_group(const SpawnGroup& native)
		{
			asset_data::spawn_group data;
			data.points = plain::read_array(native.points, native.pointCount, "SpawnGroup.points", [&](const auto& item) { return read_spawn_group_point(item); });
			data.clusters = plain::read_array(native.clusters, native.clusterCount, "SpawnGroup.clusters", [&](const auto& item) { return read_spawn_group_cluster(item); });
			data.zones = plain::read_array(native.zones, native.zoneCount, "SpawnGroup.zones", [&](const auto& item) { return read_spawn_group_zone(item); });
			return data;
		}

		void write_spawn_group(SpawnGroup& native, const asset_data::spawn_group& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.points = plain::write_array<SpawnGroupPoint>(data.points, memory, [&](auto& out, const auto& in) { write_spawn_group_point(out, in, resolve, memory); });
			native.clusters = plain::write_array<SpawnGroupCluster>(data.clusters, memory, [&](auto& out, const auto& in) { write_spawn_group_cluster(out, in, resolve, memory); });
			native.zones = plain::write_array<SpawnGroupZone>(data.zones, memory, [&](auto& out, const auto& in) { write_spawn_group_zone(out, in, resolve, memory); });
			native.pointCount = plain::count_of<std::uint16_t>(data.points);
			native.clusterCount = plain::count_of<std::uint16_t>(data.clusters);
			native.zoneCount = plain::count_of<std::uint16_t>(data.zones);
		}

		std::size_t size_spawn_group([[maybe_unused]] const asset_data::spawn_group& data)
		{
			std::size_t size = 0;
			if (data.points)
			{
				size += plain::rounded(std::max<std::size_t>(data.points->size(), 1) * sizeof(SpawnGroupPoint));
				for (const auto& item : *data.points)
				{
					size += size_spawn_group_point(item);
				}
			}
			if (data.clusters)
			{
				size += plain::rounded(std::max<std::size_t>(data.clusters->size(), 1) * sizeof(SpawnGroupCluster));
				for (const auto& item : *data.clusters)
				{
					size += size_spawn_group_cluster(item);
				}
			}
			if (data.zones)
			{
				size += plain::rounded(std::max<std::size_t>(data.zones->size(), 1) * sizeof(SpawnGroupZone));
				for (const auto& item : *data.zones)
				{
					size += size_spawn_group_zone(item);
				}
			}
			return size;
		}

		asset_data::fx_combined_def read_fx_combined_def(const FxCombinedDef& native)
		{
			asset_data::fx_combined_def data;
			data.particle_system_def = plain::read_asset(ASSET_TYPE_VFX, native.particleSystemDef);
			return data;
		}

		void write_fx_combined_def(FxCombinedDef& native, const asset_data::fx_combined_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.particleSystemDef = plain::resolve_asset<ParticleSystemDef>(resolve, ASSET_TYPE_VFX, data.particle_system_def);
		}

		std::size_t size_fx_combined_def([[maybe_unused]] const asset_data::fx_combined_def& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::snd_alias_lookup read_snd_alias_lookup(const SndAliasLookup& native)
		{
			asset_data::snd_alias_lookup data;
			data.name = plain::read_string(native.name);
			return data;
		}

		void write_snd_alias_lookup(SndAliasLookup& native, const asset_data::snd_alias_lookup& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
		}

		std::size_t size_snd_alias_lookup([[maybe_unused]] const asset_data::snd_alias_lookup& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			return size;
		}

		asset_data::snd_alias read_snd_alias(const SndAlias& native)
		{
			asset_data::snd_alias data;
			data.alias_name = plain::read_string(native.aliasName);
			data.subtitle = plain::read_string(native.subtitle);
			data.secondary_alias_name = plain::read_string(native.secondaryAliasName);
			data.asset_file_name = plain::read_string(native.assetFileName);
			data.id = native.id;
			data.secondary_id = native.secondaryId;
			data.asset_id = native.assetId;
			data.context_type = native.contextType;
			data.context_value = native.contextValue;
			data.context2_type = native.context2Type;
			data.context2_value = native.context2Value;
			data.duck = native.duck;
			data.surface_flags = native.surfaceFlags;
			data.sequence = native.sequence;
			data.vol_min = native.volMin;
			data.vol_max = native.volMax;
			data.vol_mod_index = native.volModIndex;
			data.pitch_min = native.pitchMin;
			data.pitch_max = native.pitchMax;
			data.donut_fade_end = native.donutFadeEnd;
			data.dist_min = native.distMin;
			data.dist_max = native.distMax;
			data.velocity_min = native.velocityMin;
			data.flags = native.flags;
			data.probability = native.probability;
			data.lfe_percentage = native.lfePercentage;
			data.center_percentage = native.centerPercentage;
			data.start_delay = native.startDelay;
			data.start_delay_max = native.startDelayMax;
			data.volume_falloff_curve = native.volumeFalloffCurve;
			data.lpf_curve = native.lpfCurve;
			data.hpf_curve = native.hpfCurve;
			data.reverb_send_curve = native.reverbSendCurve;
			data.envelop_min = native.envelopMin;
			data.envelop_max = native.envelopMax;
			data.envelop_percentage = native.envelopPercentage;
			data.speaker_map = native.speakerMap;
			data.reverb_multiplier = native.reverbMultiplier;
			data.far_reverb_multiplier = native.farReverbMultiplier;
			data.occlusion_shape = native.occlusionShape;
			data.doppler_preset = native.dopplerPreset;
			data.smart_pan_distance2d = native.smartPanDistance2d;
			data.smart_pan_distance3d = native.smartPanDistance3d;
			data.smart_pan_attenuation2d = native.smartPanAttenuation2d;
			data.stereo_spread_min_dist = native.stereoSpreadMinDist;
			data.stereo_spread_max_dist = native.stereoSpreadMaxDist;
			data.stereo_spread_mid_point = native.stereoSpreadMidPoint;
			data.stereo_spread_max_angle = native.stereoSpreadMaxAngle;
			data.master_priority = native.masterPriority;
			data.master_percentage = native.masterPercentage;
			data.master_slave_percentage = native.masterSlavePercentage;
			data.master_threshold = native.masterThreshold;
			data.max_duration_dsec = native.maxDuration_dsec;
			return data;
		}

		void write_snd_alias(SndAlias& native, const asset_data::snd_alias& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.aliasName = plain::write_string(memory, data.alias_name);
			native.subtitle = plain::write_string(memory, data.subtitle);
			native.secondaryAliasName = plain::write_string(memory, data.secondary_alias_name);
			native.assetFileName = plain::write_string(memory, data.asset_file_name);
			native.id = data.id;
			native.secondaryId = data.secondary_id;
			native.assetId = data.asset_id;
			native.contextType = data.context_type;
			native.contextValue = data.context_value;
			native.context2Type = data.context2_type;
			native.context2Value = data.context2_value;
			native.duck = data.duck;
			native.surfaceFlags = data.surface_flags;
			native.sequence = data.sequence;
			native.volMin = data.vol_min;
			native.volMax = data.vol_max;
			native.volModIndex = data.vol_mod_index;
			native.pitchMin = data.pitch_min;
			native.pitchMax = data.pitch_max;
			native.donutFadeEnd = data.donut_fade_end;
			native.distMin = data.dist_min;
			native.distMax = data.dist_max;
			native.velocityMin = data.velocity_min;
			native.flags = data.flags;
			native.probability = data.probability;
			native.lfePercentage = data.lfe_percentage;
			native.centerPercentage = data.center_percentage;
			native.startDelay = data.start_delay;
			native.startDelayMax = data.start_delay_max;
			native.volumeFalloffCurve = data.volume_falloff_curve;
			native.lpfCurve = data.lpf_curve;
			native.hpfCurve = data.hpf_curve;
			native.reverbSendCurve = data.reverb_send_curve;
			native.envelopMin = data.envelop_min;
			native.envelopMax = data.envelop_max;
			native.envelopPercentage = data.envelop_percentage;
			native.speakerMap = data.speaker_map;
			native.reverbMultiplier = data.reverb_multiplier;
			native.farReverbMultiplier = data.far_reverb_multiplier;
			native.occlusionShape = data.occlusion_shape;
			native.dopplerPreset = data.doppler_preset;
			native.smartPanDistance2d = data.smart_pan_distance2d;
			native.smartPanDistance3d = data.smart_pan_distance3d;
			native.smartPanAttenuation2d = data.smart_pan_attenuation2d;
			native.stereoSpreadMinDist = data.stereo_spread_min_dist;
			native.stereoSpreadMaxDist = data.stereo_spread_max_dist;
			native.stereoSpreadMidPoint = data.stereo_spread_mid_point;
			native.stereoSpreadMaxAngle = data.stereo_spread_max_angle;
			native.masterPriority = data.master_priority;
			native.masterPercentage = data.master_percentage;
			native.masterSlavePercentage = data.master_slave_percentage;
			native.masterThreshold = data.master_threshold;
			native.maxDuration_dsec = data.max_duration_dsec;
		}

		std::size_t size_snd_alias([[maybe_unused]] const asset_data::snd_alias& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.alias_name);
			size += plain::string_size(data.subtitle);
			size += plain::string_size(data.secondary_alias_name);
			size += plain::string_size(data.asset_file_name);
			return size;
		}

		asset_data::snd_alias_list read_snd_alias_list(const SndAliasList& native)
		{
			asset_data::snd_alias_list data;
			data.alias_name = plain::read_string(native.aliasName);
			data.id = native.id;
			data.head = plain::read_array(native.head, native.count, "SndAliasList.head", [&](const auto& item) { return read_snd_alias(item); });
			data.sequence = native.sequence;
			return data;
		}

		void write_snd_alias_list(SndAliasList& native, const asset_data::snd_alias_list& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.aliasName = plain::write_string(memory, data.alias_name);
			native.id = data.id;
			native.head = plain::write_array<SndAlias>(data.head, memory, [&](auto& out, const auto& in) { write_snd_alias(out, in, resolve, memory); });
			native.sequence = data.sequence;
			native.count = plain::count_of<std::int32_t>(data.head);
		}

		std::size_t size_snd_alias_list([[maybe_unused]] const asset_data::snd_alias_list& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.alias_name);
			if (data.head)
			{
				size += plain::rounded(std::max<std::size_t>(data.head->size(), 1) * sizeof(SndAlias));
				for (const auto& item : *data.head)
				{
					size += size_snd_alias(item);
				}
			}
			return size;
		}

		asset_data::client_oneshot_effect_def read_client_oneshot_effect_def(const ClientOneshotEffectDef& native)
		{
			asset_data::client_oneshot_effect_def data;
			data.origin = plain::read_scalars<float, 3>(&native.origin);
			data.angles = plain::read_scalars<float, 3>(&native.angles);
			data.radius = native.radius;
			data.delay_msec = native.delayMsec;
			data.effect = read_fx_combined_def(native.effect);
			data.effect_sound = read_snd_alias_lookup(native.effectSound);
			data.alias_list = plain::read_single(native.aliasList, [&](const auto& item) { return read_snd_alias_list(item); });
			return data;
		}

		void write_client_oneshot_effect_def(ClientOneshotEffectDef& native, const asset_data::client_oneshot_effect_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.origin, data.origin);
			plain::write_scalars(&native.angles, data.angles);
			native.radius = data.radius;
			native.delayMsec = data.delay_msec;
			write_fx_combined_def(native.effect, data.effect, resolve, memory);
			write_snd_alias_lookup(native.effectSound, data.effect_sound, resolve, memory);
			native.aliasList = plain::write_single<SndAliasList>(data.alias_list, memory, [&](auto& out, const auto& in) { write_snd_alias_list(out, in, resolve, memory); });
		}

		std::size_t size_client_oneshot_effect_def([[maybe_unused]] const asset_data::client_oneshot_effect_def& data)
		{
			std::size_t size = 0;
			size += size_fx_combined_def(data.effect);
			size += size_snd_alias_lookup(data.effect_sound);
			if (data.alias_list)
			{
				size += plain::rounded(sizeof(SndAliasList));
				size += size_snd_alias_list(*data.alias_list);
			}
			return size;
		}

		asset_data::client_exploder_def read_client_exploder_def(const ClientExploderDef& native)
		{
			asset_data::client_exploder_def data;
			data.origin = plain::read_scalars<float, 3>(&native.origin);
			data.angles = plain::read_scalars<float, 3>(&native.angles);
			data.delay_msec = native.delayMsec;
			data.effect = read_fx_combined_def(native.effect);
			data.effect_sound = read_snd_alias_lookup(native.effectSound);
			data.alias_list = plain::read_single(native.aliasList, [&](const auto& item) { return read_snd_alias_list(item); });
			data.name = plain::read_script_string(native.name);
			return data;
		}

		void write_client_exploder_def(ClientExploderDef& native, const asset_data::client_exploder_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.origin, data.origin);
			plain::write_scalars(&native.angles, data.angles);
			native.delayMsec = data.delay_msec;
			write_fx_combined_def(native.effect, data.effect, resolve, memory);
			write_snd_alias_lookup(native.effectSound, data.effect_sound, resolve, memory);
			native.aliasList = plain::write_single<SndAliasList>(data.alias_list, memory, [&](auto& out, const auto& in) { write_snd_alias_list(out, in, resolve, memory); });
			native.name = static_cast<scr_string_t>(plain::write_script_string(data.name));
		}

		std::size_t size_client_exploder_def([[maybe_unused]] const asset_data::client_exploder_def& data)
		{
			std::size_t size = 0;
			size += size_fx_combined_def(data.effect);
			size += size_snd_alias_lookup(data.effect_sound);
			if (data.alias_list)
			{
				size += plain::rounded(sizeof(SndAliasList));
				size += size_snd_alias_list(*data.alias_list);
			}
			return size;
		}

		asset_data::client_interval_sound_def read_client_interval_sound_def(const ClientIntervalSoundDef& native)
		{
			asset_data::client_interval_sound_def data;
			data.origin = plain::read_scalars<float, 3>(&native.origin);
			data.angles = plain::read_scalars<float, 3>(&native.angles);
			data.effect_sound = read_snd_alias_lookup(native.effectSound);
			data.alias_list = plain::read_single(native.aliasList, [&](const auto& item) { return read_snd_alias_list(item); });
			data.delay_min_msec = native.delayMinMsec;
			data.delay_max_msec = native.delayMaxMsec;
			return data;
		}

		void write_client_interval_sound_def(ClientIntervalSoundDef& native, const asset_data::client_interval_sound_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.origin, data.origin);
			plain::write_scalars(&native.angles, data.angles);
			write_snd_alias_lookup(native.effectSound, data.effect_sound, resolve, memory);
			native.aliasList = plain::write_single<SndAliasList>(data.alias_list, memory, [&](auto& out, const auto& in) { write_snd_alias_list(out, in, resolve, memory); });
			native.delayMinMsec = data.delay_min_msec;
			native.delayMaxMsec = data.delay_max_msec;
		}

		std::size_t size_client_interval_sound_def([[maybe_unused]] const asset_data::client_interval_sound_def& data)
		{
			std::size_t size = 0;
			size += size_snd_alias_lookup(data.effect_sound);
			if (data.alias_list)
			{
				size += plain::rounded(sizeof(SndAliasList));
				size += size_snd_alias_list(*data.alias_list);
			}
			return size;
		}

		asset_data::client_loop_sound_def read_client_loop_sound_def(const ClientLoopSoundDef& native)
		{
			asset_data::client_loop_sound_def data;
			data.origin = plain::read_scalars<float, 3>(&native.origin);
			data.angles = plain::read_scalars<float, 3>(&native.angles);
			data.effect_sound = read_snd_alias_lookup(native.effectSound);
			data.alias_list = plain::read_single(native.aliasList, [&](const auto& item) { return read_snd_alias_list(item); });
			return data;
		}

		void write_client_loop_sound_def(ClientLoopSoundDef& native, const asset_data::client_loop_sound_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.origin, data.origin);
			plain::write_scalars(&native.angles, data.angles);
			write_snd_alias_lookup(native.effectSound, data.effect_sound, resolve, memory);
			native.aliasList = plain::write_single<SndAliasList>(data.alias_list, memory, [&](auto& out, const auto& in) { write_snd_alias_list(out, in, resolve, memory); });
		}

		std::size_t size_client_loop_sound_def([[maybe_unused]] const asset_data::client_loop_sound_def& data)
		{
			std::size_t size = 0;
			size += size_snd_alias_lookup(data.effect_sound);
			if (data.alias_list)
			{
				size += plain::rounded(sizeof(SndAliasList));
				size += size_snd_alias_list(*data.alias_list);
			}
			return size;
		}

		asset_data::client_reactive_ent_def read_client_reactive_ent_def(const ClientReactiveEntDef& native)
		{
			asset_data::client_reactive_ent_def data;
			data.origin = plain::read_scalars<float, 3>(&native.origin);
			data.angles = plain::read_scalars<float, 3>(&native.angles);
			data.effect = read_fx_combined_def(native.effect);
			data.effect_sound = read_snd_alias_lookup(native.effectSound);
			data.alias_list = plain::read_single(native.aliasList, [&](const auto& item) { return read_snd_alias_list(item); });
			data.radius = native.radius;
			return data;
		}

		void write_client_reactive_ent_def(ClientReactiveEntDef& native, const asset_data::client_reactive_ent_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.origin, data.origin);
			plain::write_scalars(&native.angles, data.angles);
			write_fx_combined_def(native.effect, data.effect, resolve, memory);
			write_snd_alias_lookup(native.effectSound, data.effect_sound, resolve, memory);
			native.aliasList = plain::write_single<SndAliasList>(data.alias_list, memory, [&](auto& out, const auto& in) { write_snd_alias_list(out, in, resolve, memory); });
			native.radius = data.radius;
		}

		std::size_t size_client_reactive_ent_def([[maybe_unused]] const asset_data::client_reactive_ent_def& data)
		{
			std::size_t size = 0;
			size += size_fx_combined_def(data.effect);
			size += size_snd_alias_lookup(data.effect_sound);
			if (data.alias_list)
			{
				size += plain::rounded(sizeof(SndAliasList));
				size += size_snd_alias_list(*data.alias_list);
			}
			return size;
		}

		asset_data::client_side_effects read_client_side_effects(const ClientSideEffects& native)
		{
			asset_data::client_side_effects data;
			data.oneshot_effect_count = native.oneshotEffectCount;
			data.oneshot_effects = plain::read_array(native.oneshotEffects, native.oneshotEffectCount, "ClientSideEffects.oneshotEffects", [&](const auto& item) { return read_client_oneshot_effect_def(item); });
			data.oneshot_effects_tree = plain::read_single(native.oneshotEffectsTree, [&](const auto& item) { return read_spatial_partition_tree(item); });
			data.oneshot_sound_indices = plain::read_array(native.oneshotSoundIndices, native.oneshotSoundCount, "ClientSideEffects.oneshotSoundIndices", [&](const auto& item) { return item; });
			data.exploders = plain::read_array(native.exploders, native.exploderCount, "ClientSideEffects.exploders", [&](const auto& item) { return read_client_exploder_def(item); });
			data.interval_sounds = plain::read_array(native.intervalSounds, native.intervalSoundCount, "ClientSideEffects.intervalSounds", [&](const auto& item) { return read_client_interval_sound_def(item); });
			data.loop_sounds = plain::read_array(native.loopSounds, native.loopSoundCount, "ClientSideEffects.loopSounds", [&](const auto& item) { return read_client_loop_sound_def(item); });
			data.sounds_spatial_tree = plain::read_single(native.soundsSpatialTree, [&](const auto& item) { return read_spatial_partition_tree(item); });
			data.reactive_ents = plain::read_array(native.reactiveEnts, native.reactiveEntCount, "ClientSideEffects.reactiveEnts", [&](const auto& item) { return read_client_reactive_ent_def(item); });
			data.reactive_spatial_tree = plain::read_single(native.reactiveSpatialTree, [&](const auto& item) { return read_spatial_partition_tree(item); });
			return data;
		}

		void write_client_side_effects(ClientSideEffects& native, const asset_data::client_side_effects& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.oneshotEffectCount = data.oneshot_effect_count;
			native.oneshotEffects = plain::write_array<ClientOneshotEffectDef>(data.oneshot_effects, memory, [&](auto& out, const auto& in) { write_client_oneshot_effect_def(out, in, resolve, memory); });
			native.oneshotEffectsTree = plain::write_single<SpatialPartition_Tree>(data.oneshot_effects_tree, memory, [&](auto& out, const auto& in) { write_spatial_partition_tree(out, in, resolve, memory); });
			native.oneshotSoundIndices = plain::write_array<std::uint32_t>(data.oneshot_sound_indices, memory, [&](auto& out, const auto& in) { out = in; });
			native.exploders = plain::write_array<ClientExploderDef>(data.exploders, memory, [&](auto& out, const auto& in) { write_client_exploder_def(out, in, resolve, memory); });
			native.intervalSounds = plain::write_array<ClientIntervalSoundDef>(data.interval_sounds, memory, [&](auto& out, const auto& in) { write_client_interval_sound_def(out, in, resolve, memory); });
			native.loopSounds = plain::write_array<ClientLoopSoundDef>(data.loop_sounds, memory, [&](auto& out, const auto& in) { write_client_loop_sound_def(out, in, resolve, memory); });
			native.soundsSpatialTree = plain::write_single<SpatialPartition_Tree>(data.sounds_spatial_tree, memory, [&](auto& out, const auto& in) { write_spatial_partition_tree(out, in, resolve, memory); });
			native.reactiveEnts = plain::write_array<ClientReactiveEntDef>(data.reactive_ents, memory, [&](auto& out, const auto& in) { write_client_reactive_ent_def(out, in, resolve, memory); });
			native.reactiveSpatialTree = plain::write_single<SpatialPartition_Tree>(data.reactive_spatial_tree, memory, [&](auto& out, const auto& in) { write_spatial_partition_tree(out, in, resolve, memory); });
			native.oneshotSoundCount = plain::count_of<std::uint32_t>(data.oneshot_sound_indices);
			native.exploderCount = plain::count_of<std::uint32_t>(data.exploders);
			native.intervalSoundCount = plain::count_of<std::uint32_t>(data.interval_sounds);
			native.loopSoundCount = plain::count_of<std::uint32_t>(data.loop_sounds);
			native.reactiveEntCount = plain::count_of<std::uint32_t>(data.reactive_ents);
		}

		std::size_t size_client_side_effects([[maybe_unused]] const asset_data::client_side_effects& data)
		{
			std::size_t size = 0;
			if (data.oneshot_effects)
			{
				size += plain::rounded(std::max<std::size_t>(data.oneshot_effects->size(), 1) * sizeof(ClientOneshotEffectDef));
				for (const auto& item : *data.oneshot_effects)
				{
					size += size_client_oneshot_effect_def(item);
				}
			}
			if (data.oneshot_effects_tree)
			{
				size += plain::rounded(sizeof(SpatialPartition_Tree));
				size += size_spatial_partition_tree(*data.oneshot_effects_tree);
			}
			if (data.oneshot_sound_indices)
			{
				size += plain::rounded(std::max<std::size_t>(data.oneshot_sound_indices->size(), 1) * sizeof(std::uint32_t));
			}
			if (data.exploders)
			{
				size += plain::rounded(std::max<std::size_t>(data.exploders->size(), 1) * sizeof(ClientExploderDef));
				for (const auto& item : *data.exploders)
				{
					size += size_client_exploder_def(item);
				}
			}
			if (data.interval_sounds)
			{
				size += plain::rounded(std::max<std::size_t>(data.interval_sounds->size(), 1) * sizeof(ClientIntervalSoundDef));
				for (const auto& item : *data.interval_sounds)
				{
					size += size_client_interval_sound_def(item);
				}
			}
			if (data.loop_sounds)
			{
				size += plain::rounded(std::max<std::size_t>(data.loop_sounds->size(), 1) * sizeof(ClientLoopSoundDef));
				for (const auto& item : *data.loop_sounds)
				{
					size += size_client_loop_sound_def(item);
				}
			}
			if (data.sounds_spatial_tree)
			{
				size += plain::rounded(sizeof(SpatialPartition_Tree));
				size += size_spatial_partition_tree(*data.sounds_spatial_tree);
			}
			if (data.reactive_ents)
			{
				size += plain::rounded(std::max<std::size_t>(data.reactive_ents->size(), 1) * sizeof(ClientReactiveEntDef));
				for (const auto& item : *data.reactive_ents)
				{
					size += size_client_reactive_ent_def(item);
				}
			}
			if (data.reactive_spatial_tree)
			{
				size += plain::rounded(sizeof(SpatialPartition_Tree));
				size += size_spatial_partition_tree(*data.reactive_spatial_tree);
			}
			return size;
		}

		asset_data::create_fx_palette_def read_create_fx_palette_def(const CreateFxPaletteDef& native)
		{
			asset_data::create_fx_palette_def data;
			data.alias_name = plain::read_script_string(native.aliasName);
			data.effect_path = plain::read_script_string(native.effectPath);
			data.ref_count = native.refCount;
			return data;
		}

		void write_create_fx_palette_def(CreateFxPaletteDef& native, const asset_data::create_fx_palette_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.aliasName = static_cast<scr_string_t>(plain::write_script_string(data.alias_name));
			native.effectPath = static_cast<scr_string_t>(plain::write_script_string(data.effect_path));
			native.refCount = data.ref_count;
		}

		std::size_t size_create_fx_palette_def([[maybe_unused]] const asset_data::create_fx_palette_def& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::create_fx_asset_data read_create_fx_asset_data(const CreateFxAssetData& native)
		{
			asset_data::create_fx_asset_data data;
			data.fx_palette_count = native.fxPaletteCount;
			data.fx_palette = plain::read_array(native.fxPalette, native.fxPaletteCount, "CreateFxAssetData.fxPalette", [&](const auto& item) { return read_create_fx_palette_def(item); });
			return data;
		}

		void write_create_fx_asset_data(CreateFxAssetData& native, const asset_data::create_fx_asset_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.fxPaletteCount = data.fx_palette_count;
			native.fxPalette = plain::write_array<CreateFxPaletteDef>(data.fx_palette, memory, [&](auto& out, const auto& in) { write_create_fx_palette_def(out, in, resolve, memory); });
		}

		std::size_t size_create_fx_asset_data([[maybe_unused]] const asset_data::create_fx_asset_data& data)
		{
			std::size_t size = 0;
			if (data.fx_palette)
			{
				size += plain::rounded(std::max<std::size_t>(data.fx_palette->size(), 1) * sizeof(CreateFxPaletteDef));
				for (const auto& item : *data.fx_palette)
				{
					size += size_create_fx_palette_def(item);
				}
			}
			return size;
		}

		asset_data::server_exploder_def read_server_exploder_def(const ServerExploderDef& native)
		{
			asset_data::server_exploder_def data;
			data.origin = plain::read_scalars<float, 3>(&native.origin);
			data.delay_msec = native.delayMsec;
			data.damage_amount = native.damageAmount;
			data.damage_radius = native.damageRadius;
			data.name = plain::read_script_string(native.name);
			data.earthquake_name = plain::read_script_string(native.earthquakeName);
			data.rumble_name = plain::read_script_string(native.rumbleName);
			data.damage_environment_only = native.damageEnvironmentOnly;
			data.damage_do_occlusion_traces = native.damageDoOcclusionTraces;
			return data;
		}

		void write_server_exploder_def(ServerExploderDef& native, const asset_data::server_exploder_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.origin, data.origin);
			native.delayMsec = data.delay_msec;
			native.damageAmount = data.damage_amount;
			native.damageRadius = data.damage_radius;
			native.name = static_cast<scr_string_t>(plain::write_script_string(data.name));
			native.earthquakeName = static_cast<scr_string_t>(plain::write_script_string(data.earthquake_name));
			native.rumbleName = static_cast<scr_string_t>(plain::write_script_string(data.rumble_name));
			native.damageEnvironmentOnly = data.damage_environment_only;
			native.damageDoOcclusionTraces = data.damage_do_occlusion_traces;
		}

		std::size_t size_server_exploder_def([[maybe_unused]] const asset_data::server_exploder_def& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::server_side_effects read_server_side_effects(const ServerSideEffects& native)
		{
			asset_data::server_side_effects data;
			data.exploders = plain::read_array(native.exploders, native.exploderCount, "ServerSideEffects.exploders", [&](const auto& item) { return read_server_exploder_def(item); });
			return data;
		}

		void write_server_side_effects(ServerSideEffects& native, const asset_data::server_side_effects& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.exploders = plain::write_array<ServerExploderDef>(data.exploders, memory, [&](auto& out, const auto& in) { write_server_exploder_def(out, in, resolve, memory); });
			native.exploderCount = plain::count_of<std::uint16_t>(data.exploders);
		}

		std::size_t size_server_side_effects([[maybe_unused]] const asset_data::server_side_effects& data)
		{
			std::size_t size = 0;
			if (data.exploders)
			{
				size += plain::rounded(std::max<std::size_t>(data.exploders->size(), 1) * sizeof(ServerExploderDef));
				for (const auto& item : *data.exploders)
				{
					size += size_server_exploder_def(item);
				}
			}
			return size;
		}

		asset_data::gfx_placement read_gfx_placement(const GfxPlacement& native)
		{
			asset_data::gfx_placement data;
			data.quat = plain::read_scalars<float, 4>(&native.quat);
			data.origin = plain::read_scalars<float, 3>(&native.origin);
			return data;
		}

		void write_gfx_placement(GfxPlacement& native, const asset_data::gfx_placement& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.quat, data.quat);
			plain::write_scalars(&native.origin, data.origin);
		}

		std::size_t size_gfx_placement([[maybe_unused]] const asset_data::gfx_placement& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::client_entity_link_to_def read_client_entity_link_to_def(const ClientEntityLinkToDef& native)
		{
			asset_data::client_entity_link_to_def data;
			data.anchor_index = native.anchorIndex;
			data.origin_offset = plain::read_scalars<float, 3>(&native.originOffset);
			data.angle_offset = plain::read_scalars<float, 3>(&native.angleOffset);
			return data;
		}

		void write_client_entity_link_to_def(ClientEntityLinkToDef& native, const asset_data::client_entity_link_to_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.anchorIndex = data.anchor_index;
			plain::write_scalars(&native.originOffset, data.origin_offset);
			plain::write_scalars(&native.angleOffset, data.angle_offset);
		}

		std::size_t size_client_entity_link_to_def([[maybe_unused]] const asset_data::client_entity_link_to_def& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::mayhem_instance read_mayhem_instance(const MayhemInstance& native)
		{
			asset_data::mayhem_instance data;
			data.initial_pose = read_gfx_placement(native.initialPose);
			data.mayhem = plain::read_asset(ASSET_TYPE_MAYHEM, native.mayhem);
			data.link_to = plain::read_single(native.linkTo, [&](const auto& item) { return read_client_entity_link_to_def(item); });
			data.script_name = plain::read_script_string(native.scriptName);
			data.cur_time = native.curTime;
			data.old_time = native.oldTime;
			data.initial_flags = native.initialFlags;
			data.flags = native.flags;
			data.map_ent_lookup = native.mapEntLookup;
			data.transient_index_stored = native.transientIndexStored;
			return data;
		}

		void write_mayhem_instance(MayhemInstance& native, const asset_data::mayhem_instance& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_gfx_placement(native.initialPose, data.initial_pose, resolve, memory);
			native.mayhem = plain::resolve_asset<MayhemData>(resolve, ASSET_TYPE_MAYHEM, data.mayhem);
			native.linkTo = plain::write_single<ClientEntityLinkToDef>(data.link_to, memory, [&](auto& out, const auto& in) { write_client_entity_link_to_def(out, in, resolve, memory); });
			native.scriptName = static_cast<scr_string_t>(plain::write_script_string(data.script_name));
			native.curTime = data.cur_time;
			native.oldTime = data.old_time;
			native.initialFlags = data.initial_flags;
			native.flags = data.flags;
			native.mapEntLookup = data.map_ent_lookup;
			native.transientIndexStored = data.transient_index_stored;
		}

		std::size_t size_mayhem_instance([[maybe_unused]] const asset_data::mayhem_instance& data)
		{
			std::size_t size = 0;
			size += size_gfx_placement(data.initial_pose);
			if (data.link_to)
			{
				size += plain::rounded(sizeof(ClientEntityLinkToDef));
				size += size_client_entity_link_to_def(*data.link_to);
			}
			return size;
		}

		asset_data::spawner_field read_spawner_field(const SpawnerField& native)
		{
			asset_data::spawner_field data;
			data.key = plain::read_script_string(native.key);
			data.key_canonical = native.keyCanonical;
			data.value = plain::read_script_string(native.value);
			data.type = plain::enum_name(asset_data::variable_type_names, static_cast<std::int64_t>(native.type), "SpawnerField.type");
			return data;
		}

		void write_spawner_field(SpawnerField& native, const asset_data::spawner_field& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.key = static_cast<scr_string_t>(plain::write_script_string(data.key));
			native.keyCanonical = data.key_canonical;
			native.value = static_cast<scr_string_t>(plain::write_script_string(data.value));
			native.type = static_cast<VariableType>(plain::enum_value(asset_data::variable_type_names, data.type, "SpawnerField.type"));
		}

		std::size_t size_spawner_field([[maybe_unused]] const asset_data::spawner_field& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::spawner read_spawner(const Spawner& native)
		{
			asset_data::spawner data;
			data.origin = plain::read_scalars<float, 3>(&native.origin);
			data.angles = plain::read_scalars<float, 3>(&native.angles);
			data.fields = plain::read_array(native.fields, native.numFields, "Spawner.fields", [&](const auto& item) { return read_spawner_field(item); });
			data.flags = native.flags;
			data.count = native.count;
			data.targetname = plain::read_script_string(native.targetname);
			data.classname = plain::read_script_string(native.classname);
			return data;
		}

		void write_spawner(Spawner& native, const asset_data::spawner& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.origin, data.origin);
			plain::write_scalars(&native.angles, data.angles);
			native.fields = plain::write_array<SpawnerField>(data.fields, memory, [&](auto& out, const auto& in) { write_spawner_field(out, in, resolve, memory); });
			native.flags = data.flags;
			native.count = data.count;
			native.targetname = static_cast<scr_string_t>(plain::write_script_string(data.targetname));
			native.classname = static_cast<scr_string_t>(plain::write_script_string(data.classname));
			native.numFields = plain::count_of<std::uint32_t>(data.fields);
		}

		std::size_t size_spawner([[maybe_unused]] const asset_data::spawner& data)
		{
			std::size_t size = 0;
			if (data.fields)
			{
				size += plain::rounded(std::max<std::size_t>(data.fields->size(), 1) * sizeof(SpawnerField));
				for (const auto& item : *data.fields)
				{
					size += size_spawner_field(item);
				}
			}
			return size;
		}

		asset_data::spawner_list read_spawner_list(const SpawnerList& native)
		{
			asset_data::spawner_list data;
			data.spawner_count = native.spawnerCount;
			data.spanwer_list = plain::read_array(native.spanwerList, native.spawnerCount, "SpawnerList.spanwerList", [&](const auto& item) { return read_spawner(item); });
			return data;
		}

		void write_spawner_list(SpawnerList& native, const asset_data::spawner_list& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.spawnerCount = data.spawner_count;
			native.spanwerList = plain::write_array<Spawner>(data.spanwer_list, memory, [&](auto& out, const auto& in) { write_spawner(out, in, resolve, memory); });
		}

		std::size_t size_spawner_list([[maybe_unused]] const asset_data::spawner_list& data)
		{
			std::size_t size = 0;
			if (data.spanwer_list)
			{
				size += plain::rounded(std::max<std::size_t>(data.spanwer_list->size(), 1) * sizeof(Spawner));
				for (const auto& item : *data.spanwer_list)
				{
					size += size_spawner(item);
				}
			}
			return size;
		}

		asset_data::audio_pa_speaker read_audio_pa_speaker(const AudioPASpeaker& native)
		{
			asset_data::audio_pa_speaker data;
			data.origin = plain::read_scalars<float, 3>(&native.origin);
			data.name_hash = native.nameHash;
			data.ent_channel_hash = native.entChannelHash;
			return data;
		}

		void write_audio_pa_speaker(AudioPASpeaker& native, const asset_data::audio_pa_speaker& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.origin, data.origin);
			native.nameHash = data.name_hash;
			native.entChannelHash = data.ent_channel_hash;
		}

		std::size_t size_audio_pa_speaker([[maybe_unused]] const asset_data::audio_pa_speaker& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::audio_propagation_node read_audio_propagation_node(const AudioPropagationNode& native)
		{
			asset_data::audio_propagation_node data;
			data.position = plain::read_scalars<float, 3>(&native.position);
			data.num_edges = native.numEdges;
			data.edge_start_index = native.edgeStartIndex;
			return data;
		}

		void write_audio_propagation_node(AudioPropagationNode& native, const asset_data::audio_propagation_node& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.position, data.position);
			native.numEdges = data.num_edges;
			native.edgeStartIndex = data.edge_start_index;
		}

		std::size_t size_audio_propagation_node([[maybe_unused]] const asset_data::audio_propagation_node& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::audio_propagation_edge read_audio_propagation_edge(const AudioPropagationEdge& native)
		{
			asset_data::audio_propagation_edge data;
			data.neighbor = native.neighbor;
			data.portal_index = native.portalIndex;
			return data;
		}

		void write_audio_propagation_edge(AudioPropagationEdge& native, const asset_data::audio_propagation_edge& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.neighbor = data.neighbor;
			native.portalIndex = data.portal_index;
		}

		std::size_t size_audio_propagation_edge([[maybe_unused]] const asset_data::audio_propagation_edge& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::coll_map_lookup_entry read_coll_map_lookup_entry(const CollMapLookupEntry& native)
		{
			asset_data::coll_map_lookup_entry data;
			data.target_name = plain::read_string(native.targetName);
			data.brush_index = native.brushIndex;
			return data;
		}

		void write_coll_map_lookup_entry(CollMapLookupEntry& native, const asset_data::coll_map_lookup_entry& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.targetName = plain::write_string(memory, data.target_name);
			native.brushIndex = data.brush_index;
		}

		std::size_t size_coll_map_lookup_entry([[maybe_unused]] const asset_data::coll_map_lookup_entry& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.target_name);
			return size;
		}

		asset_data::map_ents read_map_ents(const MapEnts& native)
		{
			asset_data::map_ents data;
			data.name = plain::read_string(native.name);
			data.entity_string = plain::read_array(native.entityString, native.numEntityChars, "MapEnts.entityString", [&](const auto& item) { return item; });
			data.trigger = read_map_triggers(native.trigger);
			data.client_trigger = read_client_triggers(native.clientTrigger);
			data.client_trigger_blend = read_client_trigger_blend(native.clientTriggerBlend);
			data.spawn_list = read_spawn_point_record_list(native.spawnList);
			data.spline_list = read_spline_record_list(native.splineList);
			data.havok_ents_shape_data = plain::read_array(native.havokEntsShapeData, native.havokEntsShapeDataSize, "MapEnts.havokEntsShapeData", [&](const auto& item) { return item; });
			data.cmodels = plain::read_array(native.cmodels, native.numSubModels, "MapEnts.cmodels", [&](const auto& item) { return read_cmodel_t(item); });
			data.edge_list_used_query_types = native.edgeListUsedQueryTypes;
			data.num_edge_lists = native.numEdgeLists;
			plain::require_zero(native.edgeLists, "MapEnts.edgeLists");
			data.edge_list_spatial_tree = plain::read_single(native.edgeListSpatialTree, [&](const auto& item) { return read_spatial_partition_tree(item); });
			data.client_models = plain::read_array(native.clientModels, native.numClientModels, "MapEnts.clientModels", [&](const auto& item) { return read_cm_client_model(item); });
			data.dyn_ent_count = std::to_array(native.dynEntCount);
			data.dyn_ent_count_total = native.dynEntCountTotal;
			data.dyn_entity_lists_count = plain::enum_name(asset_data::dyn_entity_list_id_names, static_cast<std::int64_t>(native.dynEntityListsCount), "MapEnts.dynEntityListsCount");
			plain::require_zero(native.dynEntListIds, "MapEnts.dynEntListIds");
			data.dyn_ent_physics_setup_head = plain::read_block<std::array<std::array<std::uint16_t, 2>, 2>>(&native.dynEntPhysicsSetupHead);
			data.dyn_ent_physics_setup_tail = plain::read_block<std::array<std::array<std::uint16_t, 2>, 2>>(&native.dynEntPhysicsSetupTail);
			data.dyn_ent_no_spatial_count = std::to_array(native.dynEntNoSpatialCount);
			data.dyn_ent_max_client_history_count = native.dynEntMaxClientHistoryCount;
			data.dyn_ent_max_pose_part_count = native.dynEntMaxPosePartCount;
			data.dyn_ents_with_extra_pose_parts_count = native.dynEntsWithExtraPosePartsCount;
			plain::require_zero(native.dynEntSpatialPopulation, "MapEnts.dynEntSpatialPopulation");
			plain::require_zero(native.dynEntSpatialTransientMap, "MapEnts.dynEntSpatialTransientMap");
			data.dyn_ent_no_spatial_list = read_spatial_partition_population_node(native.dynEntNoSpatialList);
			data.client_ent_anchors = plain::read_array(native.clientEntAnchors, native.clientEntAnchorCount, "MapEnts.clientEntAnchors", [&](const auto& item) { return read_client_ent_anchor(item); });
			data.scriptable_map_ents = read_scriptable_map_ents(native.scriptableMapEnts);
			data.spawn_group_loot = read_spawn_group(native.spawnGroupLoot);
			data.client_side_effects = read_client_side_effects(native.clientSideEffects);
			data.create_fx_asset_data = read_create_fx_asset_data(native.createFxAssetData);
			data.exploder_names = plain::read_array(native.exploderNames, native.exploderNameTotal, "MapEnts.exploderNames", [&](const auto& item) { return plain::read_script_string(item); });
			data.server_side_effects = read_server_side_effects(native.serverSideEffects);
			data.create_fx_effect_total = native.createFxEffectTotal;
			data.mayhem_scenes = plain::read_array(native.mayhemScenes, native.numMayhemScenes, "MapEnts.mayhemScenes", [&](const auto& item) { return read_mayhem_instance(item); });
			data.spawners = read_spawner_list(native.spawners);
			data.audio_pa_speakers = plain::read_array(native.audioPASpeakers, native.audioPASpeakerCount, "MapEnts.audioPASpeakers", [&](const auto& item) { return read_audio_pa_speaker(item); });
			data.audio_prop_nodes = plain::read_array(native.audioPropNodes, native.numAudioPropNodes, "MapEnts.audioPropNodes", [&](const auto& item) { return read_audio_propagation_node(item); });
			data.audio_prop_edges = plain::read_array(native.audioPropEdges, native.numAudioPropEdges, "MapEnts.audioPropEdges", [&](const auto& item) { return read_audio_propagation_edge(item); });
			data.collmap_lookups = plain::read_array(native.collmapLookups, native.numCollmaps, "MapEnts.collmapLookups", [&](const auto& item) { return read_coll_map_lookup_entry(item); });
			return data;
		}

		void write_map_ents(MapEnts& native, const asset_data::map_ents& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.entityString = plain::write_array<char>(data.entity_string, memory, [&](auto& out, const auto& in) { out = in; });
			write_map_triggers(native.trigger, data.trigger, resolve, memory);
			write_client_triggers(native.clientTrigger, data.client_trigger, resolve, memory);
			write_client_trigger_blend(native.clientTriggerBlend, data.client_trigger_blend, resolve, memory);
			write_spawn_point_record_list(native.spawnList, data.spawn_list, resolve, memory);
			write_spline_record_list(native.splineList, data.spline_list, resolve, memory);
			native.havokEntsShapeData = plain::write_array<char>(data.havok_ents_shape_data, memory, [&](auto& out, const auto& in) { out = in; });
			native.cmodels = plain::write_array<cmodel_t>(data.cmodels, memory, [&](auto& out, const auto& in) { write_cmodel_t(out, in, resolve, memory); });
			native.edgeListUsedQueryTypes = data.edge_list_used_query_types;
			native.numEdgeLists = data.num_edge_lists;
			native.edgeListSpatialTree = plain::write_single<SpatialPartition_Tree>(data.edge_list_spatial_tree, memory, [&](auto& out, const auto& in) { write_spatial_partition_tree(out, in, resolve, memory); });
			native.clientModels = plain::write_array<CM_ClientModel>(data.client_models, memory, [&](auto& out, const auto& in) { write_cm_client_model(out, in, resolve, memory); });
			std::ranges::copy(data.dyn_ent_count, native.dynEntCount);
			native.dynEntCountTotal = data.dyn_ent_count_total;
			native.dynEntityListsCount = static_cast<DynEntityListId>(plain::enum_value(asset_data::dyn_entity_list_id_names, data.dyn_entity_lists_count, "MapEnts.dynEntityListsCount"));
			plain::write_block(&native.dynEntPhysicsSetupHead, data.dyn_ent_physics_setup_head);
			plain::write_block(&native.dynEntPhysicsSetupTail, data.dyn_ent_physics_setup_tail);
			std::ranges::copy(data.dyn_ent_no_spatial_count, native.dynEntNoSpatialCount);
			native.dynEntMaxClientHistoryCount = data.dyn_ent_max_client_history_count;
			native.dynEntMaxPosePartCount = data.dyn_ent_max_pose_part_count;
			native.dynEntsWithExtraPosePartsCount = data.dyn_ents_with_extra_pose_parts_count;
			write_spatial_partition_population_node(native.dynEntNoSpatialList, data.dyn_ent_no_spatial_list, resolve, memory);
			native.clientEntAnchors = plain::write_array<ClientEntAnchor>(data.client_ent_anchors, memory, [&](auto& out, const auto& in) { write_client_ent_anchor(out, in, resolve, memory); });
			write_scriptable_map_ents(native.scriptableMapEnts, data.scriptable_map_ents, resolve, memory);
			write_spawn_group(native.spawnGroupLoot, data.spawn_group_loot, resolve, memory);
			write_client_side_effects(native.clientSideEffects, data.client_side_effects, resolve, memory);
			write_create_fx_asset_data(native.createFxAssetData, data.create_fx_asset_data, resolve, memory);
			native.exploderNames = plain::write_array<scr_string_t>(data.exploder_names, memory, [&](auto& out, const auto& in) { out = static_cast<scr_string_t>(plain::write_script_string(in)); });
			write_server_side_effects(native.serverSideEffects, data.server_side_effects, resolve, memory);
			native.createFxEffectTotal = data.create_fx_effect_total;
			native.mayhemScenes = plain::write_array<MayhemInstance>(data.mayhem_scenes, memory, [&](auto& out, const auto& in) { write_mayhem_instance(out, in, resolve, memory); });
			write_spawner_list(native.spawners, data.spawners, resolve, memory);
			native.audioPASpeakers = plain::write_array<AudioPASpeaker>(data.audio_pa_speakers, memory, [&](auto& out, const auto& in) { write_audio_pa_speaker(out, in, resolve, memory); });
			native.audioPropNodes = plain::write_array<AudioPropagationNode>(data.audio_prop_nodes, memory, [&](auto& out, const auto& in) { write_audio_propagation_node(out, in, resolve, memory); });
			native.audioPropEdges = plain::write_array<AudioPropagationEdge>(data.audio_prop_edges, memory, [&](auto& out, const auto& in) { write_audio_propagation_edge(out, in, resolve, memory); });
			native.collmapLookups = plain::write_array<CollMapLookupEntry>(data.collmap_lookups, memory, [&](auto& out, const auto& in) { write_coll_map_lookup_entry(out, in, resolve, memory); });
			native.numEntityChars = plain::count_of<std::int32_t>(data.entity_string);
			native.havokEntsShapeDataSize = plain::count_of<std::uint32_t>(data.havok_ents_shape_data);
			native.numSubModels = plain::count_of<std::uint32_t>(data.cmodels);
			native.numClientModels = plain::count_of<std::uint32_t>(data.client_models);
			native.clientEntAnchorCount = plain::count_of<std::uint32_t>(data.client_ent_anchors);
			native.exploderNameTotal = plain::count_of<std::uint32_t>(data.exploder_names);
			native.numMayhemScenes = plain::count_of<std::uint32_t>(data.mayhem_scenes);
			native.audioPASpeakerCount = plain::count_of<std::uint32_t>(data.audio_pa_speakers);
			native.numAudioPropNodes = plain::count_of<std::uint32_t>(data.audio_prop_nodes);
			native.numAudioPropEdges = plain::count_of<std::uint32_t>(data.audio_prop_edges);
			native.numCollmaps = plain::count_of<std::uint32_t>(data.collmap_lookups);
		}

		std::size_t size_map_ents([[maybe_unused]] const asset_data::map_ents& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.entity_string)
			{
				size += plain::rounded(std::max<std::size_t>(data.entity_string->size(), 1) * sizeof(char));
			}
			size += size_map_triggers(data.trigger);
			size += size_client_triggers(data.client_trigger);
			size += size_client_trigger_blend(data.client_trigger_blend);
			size += size_spawn_point_record_list(data.spawn_list);
			size += size_spline_record_list(data.spline_list);
			if (data.havok_ents_shape_data)
			{
				size += plain::rounded(std::max<std::size_t>(data.havok_ents_shape_data->size(), 1) * sizeof(char));
			}
			if (data.cmodels)
			{
				size += plain::rounded(std::max<std::size_t>(data.cmodels->size(), 1) * sizeof(cmodel_t));
				for (const auto& item : *data.cmodels)
				{
					size += size_cmodel_t(item);
				}
			}
			if (data.edge_list_spatial_tree)
			{
				size += plain::rounded(sizeof(SpatialPartition_Tree));
				size += size_spatial_partition_tree(*data.edge_list_spatial_tree);
			}
			if (data.client_models)
			{
				size += plain::rounded(std::max<std::size_t>(data.client_models->size(), 1) * sizeof(CM_ClientModel));
				for (const auto& item : *data.client_models)
				{
					size += size_cm_client_model(item);
				}
			}
			size += size_spatial_partition_population_node(data.dyn_ent_no_spatial_list);
			if (data.client_ent_anchors)
			{
				size += plain::rounded(std::max<std::size_t>(data.client_ent_anchors->size(), 1) * sizeof(ClientEntAnchor));
				for (const auto& item : *data.client_ent_anchors)
				{
					size += size_client_ent_anchor(item);
				}
			}
			size += size_scriptable_map_ents(data.scriptable_map_ents);
			size += size_spawn_group(data.spawn_group_loot);
			size += size_client_side_effects(data.client_side_effects);
			size += size_create_fx_asset_data(data.create_fx_asset_data);
			if (data.exploder_names)
			{
				size += plain::rounded(std::max<std::size_t>(data.exploder_names->size(), 1) * sizeof(scr_string_t));
			}
			size += size_server_side_effects(data.server_side_effects);
			if (data.mayhem_scenes)
			{
				size += plain::rounded(std::max<std::size_t>(data.mayhem_scenes->size(), 1) * sizeof(MayhemInstance));
				for (const auto& item : *data.mayhem_scenes)
				{
					size += size_mayhem_instance(item);
				}
			}
			size += size_spawner_list(data.spawners);
			if (data.audio_pa_speakers)
			{
				size += plain::rounded(std::max<std::size_t>(data.audio_pa_speakers->size(), 1) * sizeof(AudioPASpeaker));
				for (const auto& item : *data.audio_pa_speakers)
				{
					size += size_audio_pa_speaker(item);
				}
			}
			if (data.audio_prop_nodes)
			{
				size += plain::rounded(std::max<std::size_t>(data.audio_prop_nodes->size(), 1) * sizeof(AudioPropagationNode));
				for (const auto& item : *data.audio_prop_nodes)
				{
					size += size_audio_propagation_node(item);
				}
			}
			if (data.audio_prop_edges)
			{
				size += plain::rounded(std::max<std::size_t>(data.audio_prop_edges->size(), 1) * sizeof(AudioPropagationEdge));
				for (const auto& item : *data.audio_prop_edges)
				{
					size += size_audio_propagation_edge(item);
				}
			}
			if (data.collmap_lookups)
			{
				size += plain::rounded(std::max<std::size_t>(data.collmap_lookups->size(), 1) * sizeof(CollMapLookupEntry));
				for (const auto& item : *data.collmap_lookups)
				{
					size += size_coll_map_lookup_entry(item);
				}
			}
			return size;
		}

		asset_data::gfx_buffer_resource read_gfx_buffer_resource(const GfxBufferResource& native)
		{
			asset_data::gfx_buffer_resource data;
			plain::require_zero(native.resource, "GfxBufferResource.resource");
			data.gpu_address = native.gpuAddress;
			data.allocation = native.allocation;
			data.size = native.size;
			return data;
		}

		void write_gfx_buffer_resource(GfxBufferResource& native, const asset_data::gfx_buffer_resource& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.gpuAddress = data.gpu_address;
			native.allocation = data.allocation;
			native.size = data.size;
		}

		std::size_t size_gfx_buffer_resource([[maybe_unused]] const asset_data::gfx_buffer_resource& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfx_shader_buffer_view read_gfx_shader_buffer_view(const GfxShaderBufferView& native)
		{
			asset_data::gfx_shader_buffer_view data;
			plain::require_zero(native.resource, "GfxShaderBufferView.resource");
			data.view = native.view;
			return data;
		}

		void write_gfx_shader_buffer_view(GfxShaderBufferView& native, const asset_data::gfx_shader_buffer_view& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.view = data.view;
		}

		std::size_t size_gfx_shader_buffer_view([[maybe_unused]] const asset_data::gfx_shader_buffer_view& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfx_wrapped_base_buffer read_gfx_wrapped_base_buffer(const GfxWrappedBaseBuffer& native)
		{
			asset_data::gfx_wrapped_base_buffer data;
			data.buffer = read_gfx_buffer_resource(native.buffer);
			data.view = read_gfx_shader_buffer_view(native.view);
			return data;
		}

		void write_gfx_wrapped_base_buffer(GfxWrappedBaseBuffer& native, const asset_data::gfx_wrapped_base_buffer& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_gfx_buffer_resource(native.buffer, data.buffer, resolve, memory);
			write_gfx_shader_buffer_view(native.view, data.view, resolve, memory);
		}

		std::size_t size_gfx_wrapped_base_buffer([[maybe_unused]] const asset_data::gfx_wrapped_base_buffer& data)
		{
			std::size_t size = 0;
			size += size_gfx_buffer_resource(data.buffer);
			size += size_gfx_shader_buffer_view(data.view);
			return size;
		}

		asset_data::gfx_wrapped_buffer read_gfx_wrapped_buffer(const GfxWrappedBuffer& native)
		{
			asset_data::gfx_wrapped_buffer data;
			data.base = read_gfx_wrapped_base_buffer(native.base);
			plain::require_zero(native.data, "GfxWrappedBuffer.data");
			data.flags = native.flags;
			return data;
		}

		void write_gfx_wrapped_buffer(GfxWrappedBuffer& native, const asset_data::gfx_wrapped_buffer& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_gfx_wrapped_base_buffer(native.base, data.base, resolve, memory);
			native.flags = data.flags;
		}

		std::size_t size_gfx_wrapped_buffer([[maybe_unused]] const asset_data::gfx_wrapped_buffer& data)
		{
			std::size_t size = 0;
			size += size_gfx_wrapped_base_buffer(data.base);
			return size;
		}

		asset_data::gfx_index_buffer read_gfx_index_buffer(const GfxIndexBuffer& native)
		{
			asset_data::gfx_index_buffer data;
			plain::require_zero(native.resource, "GfxIndexBuffer.resource");
			data.gpu_address = native.gpuAddress;
			data.allocation = native.allocation;
			data.size = native.size;
			return data;
		}

		void write_gfx_index_buffer(GfxIndexBuffer& native, const asset_data::gfx_index_buffer& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.gpuAddress = data.gpu_address;
			native.allocation = data.allocation;
			native.size = data.size;
		}

		std::size_t size_gfx_index_buffer([[maybe_unused]] const asset_data::gfx_index_buffer& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfx_world_draw_verts read_gfx_world_draw_verts(const GfxWorldDrawVerts& native)
		{
			asset_data::gfx_world_draw_verts data;
			data.pos_data_size = native.posDataSize;
			data.pos_data = plain::read_array(native.posData, native.posDataSize, "GfxWorldDrawVerts.posData", [&](const auto& item) { return item; });
			data.aux_data = plain::read_array(native.auxData, native.auxDataSize, "GfxWorldDrawVerts.auxData", [&](const auto& item) { return item; });
			data.pos_buffer = read_gfx_wrapped_buffer(native.posBuffer);
			data.aux_buffer = read_gfx_wrapped_buffer(native.auxBuffer);
			data.indices = plain::read_array(native.indices, native.indexCount, "GfxWorldDrawVerts.indices", [&](const auto& item) { return item; });
			data.index_buffer = read_gfx_index_buffer(native.indexBuffer);
			return data;
		}

		void write_gfx_world_draw_verts(GfxWorldDrawVerts& native, const asset_data::gfx_world_draw_verts& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.posDataSize = data.pos_data_size;
			native.posData = plain::write_array<std::uint8_t>(data.pos_data, memory, [&](auto& out, const auto& in) { out = in; });
			native.auxData = plain::write_array<std::uint8_t>(data.aux_data, memory, [&](auto& out, const auto& in) { out = in; });
			write_gfx_wrapped_buffer(native.posBuffer, data.pos_buffer, resolve, memory);
			write_gfx_wrapped_buffer(native.auxBuffer, data.aux_buffer, resolve, memory);
			native.indices = plain::write_array<std::uint16_t>(data.indices, memory, [&](auto& out, const auto& in) { out = in; });
			write_gfx_index_buffer(native.indexBuffer, data.index_buffer, resolve, memory);
			native.auxDataSize = plain::count_of<std::uint32_t>(data.aux_data);
			native.indexCount = plain::count_of<std::uint32_t>(data.indices);
		}

		std::size_t size_gfx_world_draw_verts([[maybe_unused]] const asset_data::gfx_world_draw_verts& data)
		{
			std::size_t size = 0;
			if (data.pos_data)
			{
				size += plain::rounded(std::max<std::size_t>(data.pos_data->size(), 1) * sizeof(std::uint8_t));
			}
			if (data.aux_data)
			{
				size += plain::rounded(std::max<std::size_t>(data.aux_data->size(), 1) * sizeof(std::uint8_t));
			}
			size += size_gfx_wrapped_buffer(data.pos_buffer);
			size += size_gfx_wrapped_buffer(data.aux_buffer);
			if (data.indices)
			{
				size += plain::rounded(std::max<std::size_t>(data.indices->size(), 1) * sizeof(std::uint16_t));
			}
			size += size_gfx_index_buffer(data.index_buffer);
			return size;
		}

		asset_data::gfx_cell_tree_count read_gfx_cell_tree_count(const GfxCellTreeCount& native)
		{
			asset_data::gfx_cell_tree_count data;
			data.aabb_tree_count = native.aabbTreeCount;
			return data;
		}

		void write_gfx_cell_tree_count(GfxCellTreeCount& native, const asset_data::gfx_cell_tree_count& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.aabbTreeCount = data.aabb_tree_count;
		}

		std::size_t size_gfx_cell_tree_count([[maybe_unused]] const asset_data::gfx_cell_tree_count& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfx_cell_tree read_gfx_cell_tree(const GfxCellTree& native)
		{
			asset_data::gfx_cell_tree data;
			plain::require_zero(native.aabbTree, "GfxCellTree.aabbTree");
			return data;
		}

		void write_gfx_cell_tree(GfxCellTree& native, const asset_data::gfx_cell_tree& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
		}

		std::size_t size_gfx_cell_tree([[maybe_unused]] const asset_data::gfx_cell_tree& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfx_world_draw_cells read_gfx_world_draw_cells(const GfxWorldDrawCells& native)
		{
			asset_data::gfx_world_draw_cells data;
			data.cell_count = native.cellCount;
			data.aabb_tree_counts = plain::read_array(native.aabbTreeCounts, native.cellCount, "GfxWorldDrawCells.aabbTreeCounts", [&](const auto& item) { return read_gfx_cell_tree_count(item); });
			data.aabb_trees = plain::read_array(native.aabbTrees, native.cellCount, "GfxWorldDrawCells.aabbTrees", [&](const auto& item) { return read_gfx_cell_tree(item); });
			return data;
		}

		void write_gfx_world_draw_cells(GfxWorldDrawCells& native, const asset_data::gfx_world_draw_cells& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.cellCount = data.cell_count;
			native.aabbTreeCounts = plain::write_array<GfxCellTreeCount>(data.aabb_tree_counts, memory, [&](auto& out, const auto& in) { write_gfx_cell_tree_count(out, in, resolve, memory); });
			native.aabbTrees = plain::write_array<GfxCellTree>(data.aabb_trees, memory, [&](auto& out, const auto& in) { write_gfx_cell_tree(out, in, resolve, memory); });
		}

		std::size_t size_gfx_world_draw_cells([[maybe_unused]] const asset_data::gfx_world_draw_cells& data)
		{
			std::size_t size = 0;
			if (data.aabb_tree_counts)
			{
				size += plain::rounded(std::max<std::size_t>(data.aabb_tree_counts->size(), 1) * sizeof(GfxCellTreeCount));
				for (const auto& item : *data.aabb_tree_counts)
				{
					size += size_gfx_cell_tree_count(item);
				}
			}
			if (data.aabb_trees)
			{
				size += plain::rounded(std::max<std::size_t>(data.aabb_trees->size(), 1) * sizeof(GfxCellTree));
				for (const auto& item : *data.aabb_trees)
				{
					size += size_gfx_cell_tree(item);
				}
			}
			return size;
		}

		asset_data::gfx_sh_compressed_probe_data read_gfx_sh_compressed_probe_data(const GfxSHCompressedProbeData& native)
		{
			asset_data::gfx_sh_compressed_probe_data data;
			data.data = std::to_array(native.data);
			return data;
		}

		void write_gfx_sh_compressed_probe_data(GfxSHCompressedProbeData& native, const asset_data::gfx_sh_compressed_probe_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			std::ranges::copy(data.data, native.data);
		}

		std::size_t size_gfx_sh_compressed_probe_data([[maybe_unused]] const asset_data::gfx_sh_compressed_probe_data& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfx_shader_buffer_rw_view read_gfx_shader_buffer_rw_view(const GfxShaderBufferRWView& native)
		{
			asset_data::gfx_shader_buffer_rw_view data;
			plain::require_zero(native.resource, "GfxShaderBufferRWView.resource");
			data.gpu_address = native.gpuAddress;
			data.allocation = native.allocation;
			data.view = native.view;
			return data;
		}

		void write_gfx_shader_buffer_rw_view(GfxShaderBufferRWView& native, const asset_data::gfx_shader_buffer_rw_view& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.gpuAddress = data.gpu_address;
			native.allocation = data.allocation;
			native.view = data.view;
		}

		std::size_t size_gfx_shader_buffer_rw_view([[maybe_unused]] const asset_data::gfx_shader_buffer_rw_view& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfx_wrapped_rw_buffer read_gfx_wrapped_rw_buffer(const GfxWrappedRWBuffer& native)
		{
			asset_data::gfx_wrapped_rw_buffer data;
			data.buffer = read_gfx_wrapped_buffer(native.buffer);
			data.rw_view = read_gfx_shader_buffer_rw_view(native.rwView);
			return data;
		}

		void write_gfx_wrapped_rw_buffer(GfxWrappedRWBuffer& native, const asset_data::gfx_wrapped_rw_buffer& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_gfx_wrapped_buffer(native.buffer, data.buffer, resolve, memory);
			write_gfx_shader_buffer_rw_view(native.rwView, data.rw_view, resolve, memory);
		}

		std::size_t size_gfx_wrapped_rw_buffer([[maybe_unused]] const asset_data::gfx_wrapped_rw_buffer& data)
		{
			std::size_t size = 0;
			size += size_gfx_wrapped_buffer(data.buffer);
			size += size_gfx_shader_buffer_rw_view(data.rw_view);
			return size;
		}

		asset_data::gfx_sh_probe_data read_gfx_sh_probe_data(const GfxSHProbeData& native)
		{
			asset_data::gfx_sh_probe_data data;
			data.coeffs = std::to_array(native.coeffs);
			plain::require_zero(native.pad, "GfxSHProbeData.pad");
			return data;
		}

		void write_gfx_sh_probe_data(GfxSHProbeData& native, const asset_data::gfx_sh_probe_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			std::ranges::copy(data.coeffs, native.coeffs);
		}

		std::size_t size_gfx_sh_probe_data([[maybe_unused]] const asset_data::gfx_sh_probe_data& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfx_gpu_light_grid_zone read_gfx_gpu_light_grid_zone(const GfxGpuLightGridZone& native)
		{
			asset_data::gfx_gpu_light_grid_zone data;
			data.num_probes = native.numProbes;
			data.first_probe = native.firstProbe;
			data.num_tetrahedrons = native.numTetrahedrons;
			data.first_tetrahedron = native.firstTetrahedron;
			data.first_voxel_tetrahedron_index = native.firstVoxelTetrahedronIndex;
			data.voxel_tetrahedron_internal_node_shift = native.voxelTetrahedronInternalNodeShift;
			data.num_voxel_tetrahedron_indices = native.numVoxelTetrahedronIndices;
			data.fallback_probe_data = read_gfx_sh_probe_data(native.fallbackProbeData);
			return data;
		}

		void write_gfx_gpu_light_grid_zone(GfxGpuLightGridZone& native, const asset_data::gfx_gpu_light_grid_zone& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.numProbes = data.num_probes;
			native.firstProbe = data.first_probe;
			native.numTetrahedrons = data.num_tetrahedrons;
			native.firstTetrahedron = data.first_tetrahedron;
			native.firstVoxelTetrahedronIndex = data.first_voxel_tetrahedron_index;
			native.voxelTetrahedronInternalNodeShift = data.voxel_tetrahedron_internal_node_shift;
			native.numVoxelTetrahedronIndices = data.num_voxel_tetrahedron_indices;
			write_gfx_sh_probe_data(native.fallbackProbeData, data.fallback_probe_data, resolve, memory);
		}

		std::size_t size_gfx_gpu_light_grid_zone([[maybe_unused]] const asset_data::gfx_gpu_light_grid_zone& data)
		{
			std::size_t size = 0;
			size += size_gfx_sh_probe_data(data.fallback_probe_data);
			return size;
		}

		asset_data::gfx_voxel_tree_header read_gfx_voxel_tree_header(const GfxVoxelTreeHeader& native)
		{
			asset_data::gfx_voxel_tree_header data;
			data.root_node_dimension = std::to_array(native.rootNodeDimension);
			data.node_coord_bit_shift = std::to_array(native.nodeCoordBitShift);
			data.bound_min = plain::read_scalars<float, 4>(&native.boundMin);
			data.bound_max = plain::read_scalars<float, 4>(&native.boundMax);
			return data;
		}

		void write_gfx_voxel_tree_header(GfxVoxelTreeHeader& native, const asset_data::gfx_voxel_tree_header& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			std::ranges::copy(data.root_node_dimension, native.rootNodeDimension);
			std::ranges::copy(data.node_coord_bit_shift, native.nodeCoordBitShift);
			plain::write_scalars(&native.boundMin, data.bound_min);
			plain::write_scalars(&native.boundMax, data.bound_max);
		}

		std::size_t size_gfx_voxel_tree_header([[maybe_unused]] const asset_data::gfx_voxel_tree_header& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfx_voxel_top_down_view_node read_gfx_voxel_top_down_view_node(const GfxVoxelTopDownViewNode& native)
		{
			asset_data::gfx_voxel_top_down_view_node data;
			data.first_node_index = native.firstNodeIndex;
			data.z_min = native.zMin;
			data.z_max = native.zMax;
			return data;
		}

		void write_gfx_voxel_top_down_view_node(GfxVoxelTopDownViewNode& native, const asset_data::gfx_voxel_top_down_view_node& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.firstNodeIndex = data.first_node_index;
			native.zMin = data.z_min;
			native.zMax = data.z_max;
		}

		std::size_t size_gfx_voxel_top_down_view_node([[maybe_unused]] const asset_data::gfx_voxel_top_down_view_node& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfx_voxel_internal_node read_gfx_voxel_internal_node(const GfxVoxelInternalNode& native)
		{
			asset_data::gfx_voxel_internal_node data;
			data.first_node_index = std::to_array(native.firstNodeIndex);
			data.child_node_mask = std::to_array(native.childNodeMask);
			return data;
		}

		void write_gfx_voxel_internal_node(GfxVoxelInternalNode& native, const asset_data::gfx_voxel_internal_node& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			std::ranges::copy(data.first_node_index, native.firstNodeIndex);
			std::ranges::copy(data.child_node_mask, native.childNodeMask);
		}

		std::size_t size_gfx_voxel_internal_node([[maybe_unused]] const asset_data::gfx_voxel_internal_node& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfx_gpu_light_grid_voxel_tree read_gfx_gpu_light_grid_voxel_tree(const GfxGpuLightGridVoxelTree& native)
		{
			asset_data::gfx_gpu_light_grid_voxel_tree data;
			data.top_down_view_node_count = native.topDownViewNodeCount;
			data.tree_header = plain::read_single(native.treeHeader, [&](const auto& item) { return read_gfx_voxel_tree_header(item); });
			data.top_down_view_nodes = plain::read_array(native.topDownViewNodes, native.topDownViewNodeCount, "GfxGpuLightGridVoxelTree.topDownViewNodes", [&](const auto& item) { return read_gfx_voxel_top_down_view_node(item); });
			data.internal_nodes = plain::read_array(native.internalNodes, native.internalNodeCount, "GfxGpuLightGridVoxelTree.internalNodes", [&](const auto& item) { return read_gfx_voxel_internal_node(item); });
			data.tree_header_buffer = read_gfx_wrapped_buffer(native.treeHeaderBuffer);
			data.top_down_view_node_buffer = read_gfx_wrapped_buffer(native.topDownViewNodeBuffer);
			data.internal_node_buffer = read_gfx_wrapped_buffer(native.internalNodeBuffer);
			return data;
		}

		void write_gfx_gpu_light_grid_voxel_tree(GfxGpuLightGridVoxelTree& native, const asset_data::gfx_gpu_light_grid_voxel_tree& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.topDownViewNodeCount = data.top_down_view_node_count;
			native.treeHeader = plain::write_single<GfxVoxelTreeHeader>(data.tree_header, memory, [&](auto& out, const auto& in) { write_gfx_voxel_tree_header(out, in, resolve, memory); });
			native.topDownViewNodes = plain::write_array<GfxVoxelTopDownViewNode>(data.top_down_view_nodes, memory, [&](auto& out, const auto& in) { write_gfx_voxel_top_down_view_node(out, in, resolve, memory); });
			native.internalNodes = plain::write_array<GfxVoxelInternalNode>(data.internal_nodes, memory, [&](auto& out, const auto& in) { write_gfx_voxel_internal_node(out, in, resolve, memory); });
			write_gfx_wrapped_buffer(native.treeHeaderBuffer, data.tree_header_buffer, resolve, memory);
			write_gfx_wrapped_buffer(native.topDownViewNodeBuffer, data.top_down_view_node_buffer, resolve, memory);
			write_gfx_wrapped_buffer(native.internalNodeBuffer, data.internal_node_buffer, resolve, memory);
			native.internalNodeCount = plain::count_of<std::int32_t>(data.internal_nodes);
		}

		std::size_t size_gfx_gpu_light_grid_voxel_tree([[maybe_unused]] const asset_data::gfx_gpu_light_grid_voxel_tree& data)
		{
			std::size_t size = 0;
			if (data.tree_header)
			{
				size += plain::rounded(sizeof(GfxVoxelTreeHeader));
				size += size_gfx_voxel_tree_header(*data.tree_header);
			}
			if (data.top_down_view_nodes)
			{
				size += plain::rounded(std::max<std::size_t>(data.top_down_view_nodes->size(), 1) * sizeof(GfxVoxelTopDownViewNode));
				for (const auto& item : *data.top_down_view_nodes)
				{
					size += size_gfx_voxel_top_down_view_node(item);
				}
			}
			if (data.internal_nodes)
			{
				size += plain::rounded(std::max<std::size_t>(data.internal_nodes->size(), 1) * sizeof(GfxVoxelInternalNode));
				for (const auto& item : *data.internal_nodes)
				{
					size += size_gfx_voxel_internal_node(item);
				}
			}
			size += size_gfx_wrapped_buffer(data.tree_header_buffer);
			size += size_gfx_wrapped_buffer(data.top_down_view_node_buffer);
			size += size_gfx_wrapped_buffer(data.internal_node_buffer);
			return size;
		}

		asset_data::gfx_gpu_light_grid_tetrahedron read_gfx_gpu_light_grid_tetrahedron(const GfxGpuLightGridTetrahedron& native)
		{
			asset_data::gfx_gpu_light_grid_tetrahedron data;
			data.index_flags = std::to_array(native.indexFlags);
			return data;
		}

		void write_gfx_gpu_light_grid_tetrahedron(GfxGpuLightGridTetrahedron& native, const asset_data::gfx_gpu_light_grid_tetrahedron& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			std::ranges::copy(data.index_flags, native.indexFlags);
		}

		std::size_t size_gfx_gpu_light_grid_tetrahedron([[maybe_unused]] const asset_data::gfx_gpu_light_grid_tetrahedron& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfx_gpu_light_grid_tetrahedron_neighbors read_gfx_gpu_light_grid_tetrahedron_neighbors(const GfxGpuLightGridTetrahedronNeighbors& native)
		{
			asset_data::gfx_gpu_light_grid_tetrahedron_neighbors data;
			data.neighbors = std::to_array(native.neighbors);
			return data;
		}

		void write_gfx_gpu_light_grid_tetrahedron_neighbors(GfxGpuLightGridTetrahedronNeighbors& native, const asset_data::gfx_gpu_light_grid_tetrahedron_neighbors& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			std::ranges::copy(data.neighbors, native.neighbors);
		}

		std::size_t size_gfx_gpu_light_grid_tetrahedron_neighbors([[maybe_unused]] const asset_data::gfx_gpu_light_grid_tetrahedron_neighbors& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfx_gpu_light_grid_tetrahedron_visibility read_gfx_gpu_light_grid_tetrahedron_visibility(const GfxGpuLightGridTetrahedronVisibility& native)
		{
			asset_data::gfx_gpu_light_grid_tetrahedron_visibility data;
			data.visibility = std::to_array(native.visibility);
			return data;
		}

		void write_gfx_gpu_light_grid_tetrahedron_visibility(GfxGpuLightGridTetrahedronVisibility& native, const asset_data::gfx_gpu_light_grid_tetrahedron_visibility& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			std::ranges::copy(data.visibility, native.visibility);
		}

		std::size_t size_gfx_gpu_light_grid_tetrahedron_visibility([[maybe_unused]] const asset_data::gfx_gpu_light_grid_tetrahedron_visibility& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfx_gpu_light_grid_voxel_start_tetrahedron read_gfx_gpu_light_grid_voxel_start_tetrahedron(const GfxGpuLightGridVoxelStartTetrahedron& native)
		{
			asset_data::gfx_gpu_light_grid_voxel_start_tetrahedron data;
			data.index = native.index;
			return data;
		}

		void write_gfx_gpu_light_grid_voxel_start_tetrahedron(GfxGpuLightGridVoxelStartTetrahedron& native, const asset_data::gfx_gpu_light_grid_voxel_start_tetrahedron& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.index = data.index;
		}

		std::size_t size_gfx_gpu_light_grid_voxel_start_tetrahedron([[maybe_unused]] const asset_data::gfx_gpu_light_grid_voxel_start_tetrahedron& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfx_gpu_light_grid read_gfx_gpu_light_grid(const GfxGpuLightGrid& native)
		{
			asset_data::gfx_gpu_light_grid data;
			data.probe_count = native.probeCount;
			data.probes = plain::read_array(native.probes, native.probeCount, "GfxGpuLightGrid.probes", [&](const auto& item) { return read_gfx_sh_compressed_probe_data(item); });
			data.probes_buffer = read_gfx_wrapped_rw_buffer(native.probesBuffer);
			data.probe_positions = plain::read_array(native.probePositions, native.probeCount, "GfxGpuLightGrid.probePositions", [&](const auto& item) { return plain::read_scalars<float, 3>(&item); });
			data.probe_positions_buffer = read_gfx_wrapped_buffer(native.probePositionsBuffer);
			data.fallback_probe_use_custom_sample = native.fallbackProbeUseCustomSample;
			data.fallback_probe_custom_sample_pos = plain::read_scalars<float, 3>(&native.fallbackProbeCustomSamplePos);
			data.zones = plain::read_array(native.zones, native.zoneCount, "GfxGpuLightGrid.zones", [&](const auto& item) { return read_gfx_gpu_light_grid_zone(item); });
			data.zone_voxel_trees = plain::read_array(native.zoneVoxelTrees, native.zoneCount, "GfxGpuLightGrid.zoneVoxelTrees", [&](const auto& item) { return read_gfx_gpu_light_grid_voxel_tree(item); });
			data.tetrahedrons = plain::read_array(native.tetrahedrons, native.tetrahedronCount, "GfxGpuLightGrid.tetrahedrons", [&](const auto& item) { return read_gfx_gpu_light_grid_tetrahedron(item); });
			data.tetrahedron_buffer = read_gfx_wrapped_buffer(native.tetrahedronBuffer);
			data.tetrahedron_neighbors = plain::read_array(native.tetrahedronNeighbors, native.tetrahedronCount, "GfxGpuLightGrid.tetrahedronNeighbors", [&](const auto& item) { return read_gfx_gpu_light_grid_tetrahedron_neighbors(item); });
			data.tetrahedron_neighbors_buffer = read_gfx_wrapped_buffer(native.tetrahedronNeighborsBuffer);
			data.tetrahedron_visibility = plain::read_array(native.tetrahedronVisibility, native.tetrahedronCountVisible, "GfxGpuLightGrid.tetrahedronVisibility", [&](const auto& item) { return read_gfx_gpu_light_grid_tetrahedron_visibility(item); });
			data.tetrahedron_visibility_buffer = read_gfx_wrapped_buffer(native.tetrahedronVisibilityBuffer);
			data.voxel_start_tetrahedron = plain::read_array(native.voxelStartTetrahedron, native.voxelStartTetrahedronCount, "GfxGpuLightGrid.voxelStartTetrahedron", [&](const auto& item) { return read_gfx_gpu_light_grid_voxel_start_tetrahedron(item); });
			data.voxel_start_tetrahedron_buffer = read_gfx_wrapped_buffer(native.voxelStartTetrahedronBuffer);
			data.bounding_volume = [&] { std::array<std::array<float, 4>, 7> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = plain::read_scalars<float, 4>(&native.boundingVolume[i]); } return out; }();
			return data;
		}

		void write_gfx_gpu_light_grid(GfxGpuLightGrid& native, const asset_data::gfx_gpu_light_grid& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.probeCount = data.probe_count;
			native.probes = plain::write_array<GfxSHCompressedProbeData>(data.probes, memory, [&](auto& out, const auto& in) { write_gfx_sh_compressed_probe_data(out, in, resolve, memory); });
			write_gfx_wrapped_rw_buffer(native.probesBuffer, data.probes_buffer, resolve, memory);
			native.probePositions = plain::write_array<vec3_t>(data.probe_positions, memory, [&](auto& out, const auto& in) { plain::write_scalars(&out, in); });
			write_gfx_wrapped_buffer(native.probePositionsBuffer, data.probe_positions_buffer, resolve, memory);
			native.fallbackProbeUseCustomSample = data.fallback_probe_use_custom_sample;
			plain::write_scalars(&native.fallbackProbeCustomSamplePos, data.fallback_probe_custom_sample_pos);
			native.zones = plain::write_array<GfxGpuLightGridZone>(data.zones, memory, [&](auto& out, const auto& in) { write_gfx_gpu_light_grid_zone(out, in, resolve, memory); });
			native.zoneVoxelTrees = plain::write_array<GfxGpuLightGridVoxelTree>(data.zone_voxel_trees, memory, [&](auto& out, const auto& in) { write_gfx_gpu_light_grid_voxel_tree(out, in, resolve, memory); });
			native.tetrahedrons = plain::write_array<GfxGpuLightGridTetrahedron>(data.tetrahedrons, memory, [&](auto& out, const auto& in) { write_gfx_gpu_light_grid_tetrahedron(out, in, resolve, memory); });
			write_gfx_wrapped_buffer(native.tetrahedronBuffer, data.tetrahedron_buffer, resolve, memory);
			native.tetrahedronNeighbors = plain::write_array<GfxGpuLightGridTetrahedronNeighbors>(data.tetrahedron_neighbors, memory, [&](auto& out, const auto& in) { write_gfx_gpu_light_grid_tetrahedron_neighbors(out, in, resolve, memory); });
			write_gfx_wrapped_buffer(native.tetrahedronNeighborsBuffer, data.tetrahedron_neighbors_buffer, resolve, memory);
			native.tetrahedronVisibility = plain::write_array<GfxGpuLightGridTetrahedronVisibility>(data.tetrahedron_visibility, memory, [&](auto& out, const auto& in) { write_gfx_gpu_light_grid_tetrahedron_visibility(out, in, resolve, memory); });
			write_gfx_wrapped_buffer(native.tetrahedronVisibilityBuffer, data.tetrahedron_visibility_buffer, resolve, memory);
			native.voxelStartTetrahedron = plain::write_array<GfxGpuLightGridVoxelStartTetrahedron>(data.voxel_start_tetrahedron, memory, [&](auto& out, const auto& in) { write_gfx_gpu_light_grid_voxel_start_tetrahedron(out, in, resolve, memory); });
			write_gfx_wrapped_buffer(native.voxelStartTetrahedronBuffer, data.voxel_start_tetrahedron_buffer, resolve, memory);
			for (std::size_t i = 0; i < data.bounding_volume.size(); ++i)
			{
				plain::write_scalars(&native.boundingVolume[i], data.bounding_volume[i]);
			}
			native.zoneCount = plain::count_of<std::uint32_t>(data.zones);
			native.tetrahedronCount = plain::count_of<std::uint32_t>(data.tetrahedrons);
			native.tetrahedronCountVisible = plain::count_of<std::uint32_t>(data.tetrahedron_visibility);
			native.voxelStartTetrahedronCount = plain::count_of<std::uint32_t>(data.voxel_start_tetrahedron);
		}

		std::size_t size_gfx_gpu_light_grid([[maybe_unused]] const asset_data::gfx_gpu_light_grid& data)
		{
			std::size_t size = 0;
			if (data.probes)
			{
				size += plain::rounded(std::max<std::size_t>(data.probes->size(), 1) * sizeof(GfxSHCompressedProbeData));
				for (const auto& item : *data.probes)
				{
					size += size_gfx_sh_compressed_probe_data(item);
				}
			}
			size += size_gfx_wrapped_rw_buffer(data.probes_buffer);
			if (data.probe_positions)
			{
				size += plain::rounded(std::max<std::size_t>(data.probe_positions->size(), 1) * sizeof(vec3_t));
			}
			size += size_gfx_wrapped_buffer(data.probe_positions_buffer);
			if (data.zones)
			{
				size += plain::rounded(std::max<std::size_t>(data.zones->size(), 1) * sizeof(GfxGpuLightGridZone));
				for (const auto& item : *data.zones)
				{
					size += size_gfx_gpu_light_grid_zone(item);
				}
			}
			if (data.zone_voxel_trees)
			{
				size += plain::rounded(std::max<std::size_t>(data.zone_voxel_trees->size(), 1) * sizeof(GfxGpuLightGridVoxelTree));
				for (const auto& item : *data.zone_voxel_trees)
				{
					size += size_gfx_gpu_light_grid_voxel_tree(item);
				}
			}
			if (data.tetrahedrons)
			{
				size += plain::rounded(std::max<std::size_t>(data.tetrahedrons->size(), 1) * sizeof(GfxGpuLightGridTetrahedron));
				for (const auto& item : *data.tetrahedrons)
				{
					size += size_gfx_gpu_light_grid_tetrahedron(item);
				}
			}
			size += size_gfx_wrapped_buffer(data.tetrahedron_buffer);
			if (data.tetrahedron_neighbors)
			{
				size += plain::rounded(std::max<std::size_t>(data.tetrahedron_neighbors->size(), 1) * sizeof(GfxGpuLightGridTetrahedronNeighbors));
				for (const auto& item : *data.tetrahedron_neighbors)
				{
					size += size_gfx_gpu_light_grid_tetrahedron_neighbors(item);
				}
			}
			size += size_gfx_wrapped_buffer(data.tetrahedron_neighbors_buffer);
			if (data.tetrahedron_visibility)
			{
				size += plain::rounded(std::max<std::size_t>(data.tetrahedron_visibility->size(), 1) * sizeof(GfxGpuLightGridTetrahedronVisibility));
				for (const auto& item : *data.tetrahedron_visibility)
				{
					size += size_gfx_gpu_light_grid_tetrahedron_visibility(item);
				}
			}
			size += size_gfx_wrapped_buffer(data.tetrahedron_visibility_buffer);
			if (data.voxel_start_tetrahedron)
			{
				size += plain::rounded(std::max<std::size_t>(data.voxel_start_tetrahedron->size(), 1) * sizeof(GfxGpuLightGridVoxelStartTetrahedron));
				for (const auto& item : *data.voxel_start_tetrahedron)
				{
					size += size_gfx_gpu_light_grid_voxel_start_tetrahedron(item);
				}
			}
			size += size_gfx_wrapped_buffer(data.voxel_start_tetrahedron_buffer);
			return size;
		}

		asset_data::gfx_static_model_light_grid_volume read_gfx_static_model_light_grid_volume(const GfxStaticModelLightGridVolume& native)
		{
			asset_data::gfx_static_model_light_grid_volume data;
			data.first_probe = native.firstProbe;
			data.m_volume_size = std::to_array(native.m_volumeSize);
			data.flags = plain::enum_name(asset_data::gfx_static_model_light_grid_volume_flags_names, static_cast<std::int64_t>(native.flags), "GfxStaticModelLightGridVolume.flags");
			data.single_probe = native.singleProbe;
			data.volume_transform = [&] { std::array<std::array<float, 4>, 3> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = plain::read_scalars<float, 4>(&native.volumeTransform[i]); } return out; }();
			return data;
		}

		void write_gfx_static_model_light_grid_volume(GfxStaticModelLightGridVolume& native, const asset_data::gfx_static_model_light_grid_volume& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.firstProbe = data.first_probe;
			std::ranges::copy(data.m_volume_size, native.m_volumeSize);
			native.flags = static_cast<GfxStaticModelLightGridVolumeFlags>(plain::enum_value(asset_data::gfx_static_model_light_grid_volume_flags_names, data.flags, "GfxStaticModelLightGridVolume.flags"));
			native.singleProbe = data.single_probe;
			for (std::size_t i = 0; i < data.volume_transform.size(); ++i)
			{
				plain::write_scalars(&native.volumeTransform[i], data.volume_transform[i]);
			}
		}

		std::size_t size_gfx_static_model_light_grid_volume([[maybe_unused]] const asset_data::gfx_static_model_light_grid_volume& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfx_gpu_dyn_lightsets_lgv_reference read_gfx_gpu_dyn_lightsets_lgv_reference(const GfxGpuDynLightsetsLGVReference& native)
		{
			asset_data::gfx_gpu_dyn_lightsets_lgv_reference data;
			data.volume_index = native.volumeIndex;
			data.volume_probe_index = native.volumeProbeIndex;
			return data;
		}

		void write_gfx_gpu_dyn_lightsets_lgv_reference(GfxGpuDynLightsetsLGVReference& native, const asset_data::gfx_gpu_dyn_lightsets_lgv_reference& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.volumeIndex = data.volume_index;
			native.volumeProbeIndex = data.volume_probe_index;
		}

		std::size_t size_gfx_gpu_dyn_lightsets_lgv_reference([[maybe_unused]] const asset_data::gfx_gpu_dyn_lightsets_lgv_reference& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfx_dynamic_lightset_g_chunk_info read_gfx_dynamic_lightset_g_chunk_info(const GfxDynamicLightsetGChunkInfo& native)
		{
			asset_data::gfx_dynamic_lightset_g_chunk_info data;
			data.num_records = native.numRecords;
			data.bf_start = native.bfStart;
			data.org_start = native.orgStart;
			data.tmp_start = native.tmpStart;
			return data;
		}

		void write_gfx_dynamic_lightset_g_chunk_info(GfxDynamicLightsetGChunkInfo& native, const asset_data::gfx_dynamic_lightset_g_chunk_info& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.numRecords = data.num_records;
			native.bfStart = data.bf_start;
			native.orgStart = data.org_start;
			native.tmpStart = data.tmp_start;
		}

		std::size_t size_gfx_dynamic_lightset_g_chunk_info([[maybe_unused]] const asset_data::gfx_dynamic_lightset_g_chunk_info& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfx_dynamic_lightset_g_chunk read_gfx_dynamic_lightset_g_chunk(const GfxDynamicLightsetGChunk& native)
		{
			asset_data::gfx_dynamic_lightset_g_chunk data;
			data.chunk_info = read_gfx_dynamic_lightset_g_chunk_info(native.chunkInfo);
			data.flags = plain::enum_name(asset_data::gfx_dynamic_lightset_g_chunk_flags_names, static_cast<std::int64_t>(native.flags), "GfxDynamicLightsetGChunk.flags");
			data.which_lm = native.whichLM;
			data.bf_chunk_start = native.bfChunkStart;
			return data;
		}

		void write_gfx_dynamic_lightset_g_chunk(GfxDynamicLightsetGChunk& native, const asset_data::gfx_dynamic_lightset_g_chunk& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_gfx_dynamic_lightset_g_chunk_info(native.chunkInfo, data.chunk_info, resolve, memory);
			native.flags = static_cast<GfxDynamicLightsetGChunkFlags>(plain::enum_value(asset_data::gfx_dynamic_lightset_g_chunk_flags_names, data.flags, "GfxDynamicLightsetGChunk.flags"));
			native.whichLM = data.which_lm;
			native.bfChunkStart = data.bf_chunk_start;
		}

		std::size_t size_gfx_dynamic_lightset_g_chunk([[maybe_unused]] const asset_data::gfx_dynamic_lightset_g_chunk& data)
		{
			std::size_t size = 0;
			size += size_gfx_dynamic_lightset_g_chunk_info(data.chunk_info);
			return size;
		}

		asset_data::gfx_dynamic_lightset_probe_data read_gfx_dynamic_lightset_probe_data(const GfxDynamicLightsetProbeData& native)
		{
			asset_data::gfx_dynamic_lightset_probe_data data;
			data.num_g_chunk = native.numGChunk;
			data.chunk_g = plain::read_array(native.chunkG, native.numGChunk, "GfxDynamicLightsetProbeData.chunkG", [&](const auto& item) { return read_gfx_dynamic_lightset_g_chunk(item); });
			data.chunk_gbf = plain::read_array(native.chunkGBF, native.numGChunkBF, "GfxDynamicLightsetProbeData.chunkGBF", [&](const auto& item) { return item; });
			data.orig_start = native.origStart;
			data.orig_probes = plain::read_array(native.origProbes, native.numOrigProbes, "GfxDynamicLightsetProbeData.origProbes", [&](const auto& item) { return read_gfx_sh_compressed_probe_data(item); });
			data.orig_probes_buffer = read_gfx_wrapped_buffer(native.origProbesBuffer);
			data.bf_probes = plain::read_array(native.bfProbes, native.numBFProbes, "GfxDynamicLightsetProbeData.bfProbes", [&](const auto& item) { return read_gfx_sh_compressed_probe_data(item); });
			data.bf_probes_buffer = read_gfx_wrapped_buffer(native.bfProbesBuffer);
			data.skip_records = plain::read_array(native.skipRecords, native.numSkipRecords, "GfxDynamicLightsetProbeData.skipRecords", [&](const auto& item) { return item; });
			data.skip_records_buffer = read_gfx_wrapped_buffer(native.skipRecordsBuffer);
			data.pallette_records = plain::read_array(native.palletteRecords, native.numSkipRecords, "GfxDynamicLightsetProbeData.palletteRecords", [&](const auto& item) { return item; });
			data.pallette_records_buffer = read_gfx_wrapped_buffer(native.palletteRecordsBuffer);
			data.temp_remap = plain::read_array(native.tempRemap, native.numOrigProbes, "GfxDynamicLightsetProbeData.tempRemap", [&](const auto& item) { return item; });
			data.temp_remap_buffer = read_gfx_wrapped_buffer(native.tempRemapBuffer);
			return data;
		}

		void write_gfx_dynamic_lightset_probe_data(GfxDynamicLightsetProbeData& native, const asset_data::gfx_dynamic_lightset_probe_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.numGChunk = data.num_g_chunk;
			native.chunkG = plain::write_array<GfxDynamicLightsetGChunk>(data.chunk_g, memory, [&](auto& out, const auto& in) { write_gfx_dynamic_lightset_g_chunk(out, in, resolve, memory); });
			native.chunkGBF = plain::write_array<std::uint16_t>(data.chunk_gbf, memory, [&](auto& out, const auto& in) { out = in; });
			native.origStart = data.orig_start;
			native.origProbes = plain::write_array<GfxSHCompressedProbeData>(data.orig_probes, memory, [&](auto& out, const auto& in) { write_gfx_sh_compressed_probe_data(out, in, resolve, memory); });
			write_gfx_wrapped_buffer(native.origProbesBuffer, data.orig_probes_buffer, resolve, memory);
			native.bfProbes = plain::write_array<GfxSHCompressedProbeData>(data.bf_probes, memory, [&](auto& out, const auto& in) { write_gfx_sh_compressed_probe_data(out, in, resolve, memory); });
			write_gfx_wrapped_buffer(native.bfProbesBuffer, data.bf_probes_buffer, resolve, memory);
			native.skipRecords = plain::write_array<std::uint16_t>(data.skip_records, memory, [&](auto& out, const auto& in) { out = in; });
			write_gfx_wrapped_buffer(native.skipRecordsBuffer, data.skip_records_buffer, resolve, memory);
			native.palletteRecords = plain::write_array<std::uint32_t>(data.pallette_records, memory, [&](auto& out, const auto& in) { out = in; });
			write_gfx_wrapped_buffer(native.palletteRecordsBuffer, data.pallette_records_buffer, resolve, memory);
			native.tempRemap = plain::write_array<std::uint32_t>(data.temp_remap, memory, [&](auto& out, const auto& in) { out = in; });
			write_gfx_wrapped_buffer(native.tempRemapBuffer, data.temp_remap_buffer, resolve, memory);
			native.numGChunkBF = plain::count_of<std::uint32_t>(data.chunk_gbf);
			native.numOrigProbes = plain::count_of<std::uint32_t>(data.orig_probes);
			native.numBFProbes = plain::count_of<std::uint32_t>(data.bf_probes);
			native.numSkipRecords = plain::count_of<std::uint32_t>(data.skip_records);
		}

		std::size_t size_gfx_dynamic_lightset_probe_data([[maybe_unused]] const asset_data::gfx_dynamic_lightset_probe_data& data)
		{
			std::size_t size = 0;
			if (data.chunk_g)
			{
				size += plain::rounded(std::max<std::size_t>(data.chunk_g->size(), 1) * sizeof(GfxDynamicLightsetGChunk));
				for (const auto& item : *data.chunk_g)
				{
					size += size_gfx_dynamic_lightset_g_chunk(item);
				}
			}
			if (data.chunk_gbf)
			{
				size += plain::rounded(std::max<std::size_t>(data.chunk_gbf->size(), 1) * sizeof(std::uint16_t));
			}
			if (data.orig_probes)
			{
				size += plain::rounded(std::max<std::size_t>(data.orig_probes->size(), 1) * sizeof(GfxSHCompressedProbeData));
				for (const auto& item : *data.orig_probes)
				{
					size += size_gfx_sh_compressed_probe_data(item);
				}
			}
			size += size_gfx_wrapped_buffer(data.orig_probes_buffer);
			if (data.bf_probes)
			{
				size += plain::rounded(std::max<std::size_t>(data.bf_probes->size(), 1) * sizeof(GfxSHCompressedProbeData));
				for (const auto& item : *data.bf_probes)
				{
					size += size_gfx_sh_compressed_probe_data(item);
				}
			}
			size += size_gfx_wrapped_buffer(data.bf_probes_buffer);
			if (data.skip_records)
			{
				size += plain::rounded(std::max<std::size_t>(data.skip_records->size(), 1) * sizeof(std::uint16_t));
			}
			size += size_gfx_wrapped_buffer(data.skip_records_buffer);
			if (data.pallette_records)
			{
				size += plain::rounded(std::max<std::size_t>(data.pallette_records->size(), 1) * sizeof(std::uint32_t));
			}
			size += size_gfx_wrapped_buffer(data.pallette_records_buffer);
			if (data.temp_remap)
			{
				size += plain::rounded(std::max<std::size_t>(data.temp_remap->size(), 1) * sizeof(std::uint32_t));
			}
			size += size_gfx_wrapped_buffer(data.temp_remap_buffer);
			return size;
		}

		asset_data::gfx_dynamic_lightset_l_gpp_zone_info read_gfx_dynamic_lightset_l_gpp_zone_info(const GfxDynamicLightsetLGppZoneInfo& native)
		{
			asset_data::gfx_dynamic_lightset_l_gpp_zone_info data;
			data.num_orig_sml_gpp_probes = native.numOrigSMLGppProbes;
			data.lgv_references = plain::read_array(native.lgvReferences, native.numOrigSMLGppProbes, "GfxDynamicLightsetLGppZoneInfo.lgvReferences", [&](const auto& item) { return read_gfx_gpu_dyn_lightsets_lgv_reference(item); });
			data.lgv_references_buffer = read_gfx_wrapped_buffer(native.lgvReferencesBuffer);
			data.lgv_probe_data = read_gfx_dynamic_lightset_probe_data(native.lgvProbeData);
			return data;
		}

		void write_gfx_dynamic_lightset_l_gpp_zone_info(GfxDynamicLightsetLGppZoneInfo& native, const asset_data::gfx_dynamic_lightset_l_gpp_zone_info& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.numOrigSMLGppProbes = data.num_orig_sml_gpp_probes;
			native.lgvReferences = plain::write_array<GfxGpuDynLightsetsLGVReference>(data.lgv_references, memory, [&](auto& out, const auto& in) { write_gfx_gpu_dyn_lightsets_lgv_reference(out, in, resolve, memory); });
			write_gfx_wrapped_buffer(native.lgvReferencesBuffer, data.lgv_references_buffer, resolve, memory);
			write_gfx_dynamic_lightset_probe_data(native.lgvProbeData, data.lgv_probe_data, resolve, memory);
		}

		std::size_t size_gfx_dynamic_lightset_l_gpp_zone_info([[maybe_unused]] const asset_data::gfx_dynamic_lightset_l_gpp_zone_info& data)
		{
			std::size_t size = 0;
			if (data.lgv_references)
			{
				size += plain::rounded(std::max<std::size_t>(data.lgv_references->size(), 1) * sizeof(GfxGpuDynLightsetsLGVReference));
				for (const auto& item : *data.lgv_references)
				{
					size += size_gfx_gpu_dyn_lightsets_lgv_reference(item);
				}
			}
			size += size_gfx_wrapped_buffer(data.lgv_references_buffer);
			size += size_gfx_dynamic_lightset_probe_data(data.lgv_probe_data);
			return size;
		}

		asset_data::gfx_gpu_light_grid_transient_data read_gfx_gpu_light_grid_transient_data(const GfxGpuLightGridTransientData& native)
		{
			asset_data::gfx_gpu_light_grid_transient_data data;
			data.gpu_light_grid = plain::read_single(native.gpuLightGrid, [&](const auto& item) { return read_gfx_gpu_light_grid(item); });
			data.static_model_volumes = plain::read_array(native.staticModelVolumes, native.staticModelVolumeCount, "GfxGpuLightGridTransientData.staticModelVolumes", [&](const auto& item) { return read_gfx_static_model_light_grid_volume(item); });
			data.sml_gpp_zone = plain::read_single(native.SMLGppZone, [&](const auto& item) { return read_gfx_dynamic_lightset_l_gpp_zone_info(item); });
			plain::require_zero(native.tempLightGridVolumeData, "GfxGpuLightGridTransientData.tempLightGridVolumeData");
			return data;
		}

		void write_gfx_gpu_light_grid_transient_data(GfxGpuLightGridTransientData& native, const asset_data::gfx_gpu_light_grid_transient_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.gpuLightGrid = plain::write_single<GfxGpuLightGrid>(data.gpu_light_grid, memory, [&](auto& out, const auto& in) { write_gfx_gpu_light_grid(out, in, resolve, memory); });
			native.staticModelVolumes = plain::write_array<GfxStaticModelLightGridVolume>(data.static_model_volumes, memory, [&](auto& out, const auto& in) { write_gfx_static_model_light_grid_volume(out, in, resolve, memory); });
			native.SMLGppZone = plain::write_single<GfxDynamicLightsetLGppZoneInfo>(data.sml_gpp_zone, memory, [&](auto& out, const auto& in) { write_gfx_dynamic_lightset_l_gpp_zone_info(out, in, resolve, memory); });
			native.staticModelVolumeCount = plain::count_of<std::uint32_t>(data.static_model_volumes);
		}

		std::size_t size_gfx_gpu_light_grid_transient_data([[maybe_unused]] const asset_data::gfx_gpu_light_grid_transient_data& data)
		{
			std::size_t size = 0;
			if (data.gpu_light_grid)
			{
				size += plain::rounded(sizeof(GfxGpuLightGrid));
				size += size_gfx_gpu_light_grid(*data.gpu_light_grid);
			}
			if (data.static_model_volumes)
			{
				size += plain::rounded(std::max<std::size_t>(data.static_model_volumes->size(), 1) * sizeof(GfxStaticModelLightGridVolume));
				for (const auto& item : *data.static_model_volumes)
				{
					size += size_gfx_static_model_light_grid_volume(item);
				}
			}
			if (data.sml_gpp_zone)
			{
				size += plain::rounded(sizeof(GfxDynamicLightsetLGppZoneInfo));
				size += size_gfx_dynamic_lightset_l_gpp_zone_info(*data.sml_gpp_zone);
			}
			return size;
		}

		asset_data::gfx_image_reflection_probe read_gfx_image_reflection_probe(const GfxImageReflectionProbe& native)
		{
			asset_data::gfx_image_reflection_probe data;
			data.total_size = native.totalSize;
			data.element_count = native.elementCount;
			data.texture_id = plain::enum_name(asset_data::gfx_texture_id_names, static_cast<std::int64_t>(native.textureId), "GfxImageReflectionProbe.textureId");
			data.pixels = plain::read_array(native.pixels, native.totalSize, "GfxImageReflectionProbe.pixels", [&](const auto& item) { return item; });
			return data;
		}

		void write_gfx_image_reflection_probe(GfxImageReflectionProbe& native, const asset_data::gfx_image_reflection_probe& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.totalSize = data.total_size;
			native.elementCount = data.element_count;
			native.textureId = static_cast<GfxTextureId>(plain::enum_value(asset_data::gfx_texture_id_names, data.texture_id, "GfxImageReflectionProbe.textureId"));
			native.pixels = plain::write_array<std::uint8_t>(data.pixels, memory, [&](auto& out, const auto& in) { out = in; });
		}

		std::size_t size_gfx_image_reflection_probe([[maybe_unused]] const asset_data::gfx_image_reflection_probe& data)
		{
			std::size_t size = 0;
			if (data.pixels)
			{
				size += plain::rounded(std::max<std::size_t>(data.pixels->size(), 1) * sizeof(std::uint8_t));
			}
			return size;
		}

		asset_data::gfx_reflection_probe_transient_data read_gfx_reflection_probe_transient_data(const GfxReflectionProbeTransientData& native)
		{
			asset_data::gfx_reflection_probe_transient_data data;
			data.exclusive_probe_count = native.exclusiveProbeCount;
			data.reflection_probe_instances = plain::read_array(native.reflectionProbeInstances, native.reflectionProbeInstanceCount, "GfxReflectionProbeTransientData.reflectionProbeInstances", [&](const auto& item) { return item; });
			data.exclusive_probe_buckets = plain::read_array(native.exclusiveProbeBuckets, native.exclusiveProbeBucketCount, "GfxReflectionProbeTransientData.exclusiveProbeBuckets", [&](const auto& item) { return read_gfx_image_reflection_probe(item); });
			return data;
		}

		void write_gfx_reflection_probe_transient_data(GfxReflectionProbeTransientData& native, const asset_data::gfx_reflection_probe_transient_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.exclusiveProbeCount = data.exclusive_probe_count;
			native.reflectionProbeInstances = plain::write_array<std::uint16_t>(data.reflection_probe_instances, memory, [&](auto& out, const auto& in) { out = in; });
			native.exclusiveProbeBuckets = plain::write_array<GfxImageReflectionProbe>(data.exclusive_probe_buckets, memory, [&](auto& out, const auto& in) { write_gfx_image_reflection_probe(out, in, resolve, memory); });
			native.reflectionProbeInstanceCount = plain::count_of<std::uint16_t>(data.reflection_probe_instances);
			native.exclusiveProbeBucketCount = plain::count_of<std::uint16_t>(data.exclusive_probe_buckets);
		}

		std::size_t size_gfx_reflection_probe_transient_data([[maybe_unused]] const asset_data::gfx_reflection_probe_transient_data& data)
		{
			std::size_t size = 0;
			if (data.reflection_probe_instances)
			{
				size += plain::rounded(std::max<std::size_t>(data.reflection_probe_instances->size(), 1) * sizeof(std::uint16_t));
			}
			if (data.exclusive_probe_buckets)
			{
				size += plain::rounded(std::max<std::size_t>(data.exclusive_probe_buckets->size(), 1) * sizeof(GfxImageReflectionProbe));
				for (const auto& item : *data.exclusive_probe_buckets)
				{
					size += size_gfx_image_reflection_probe(item);
				}
			}
			return size;
		}

		asset_data::gfx_static_decal_volume_packed read_gfx_static_decal_volume_packed(const GfxStaticDecalVolumePacked& native)
		{
			asset_data::gfx_static_decal_volume_packed data;
			data.obb_center = plain::read_scalars<float, 3>(&native.obbCenter);
			data.obb_quat = native.obbQuat;
			data.obb_half_size = native.obbHalfSize;
			data.opacity_params = native.opacityParams;
			data.uv_matrix_and_offset = std::to_array(native.uvMatrixAndOffset);
			data.packed_material_info = native.packedMaterialInfo;
			plain::require_zero(native.reserved, "GfxStaticDecalVolumePacked.reserved");
			return data;
		}

		void write_gfx_static_decal_volume_packed(GfxStaticDecalVolumePacked& native, const asset_data::gfx_static_decal_volume_packed& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.obbCenter, data.obb_center);
			native.obbQuat = data.obb_quat;
			native.obbHalfSize = data.obb_half_size;
			native.opacityParams = data.opacity_params;
			std::ranges::copy(data.uv_matrix_and_offset, native.uvMatrixAndOffset);
			native.packedMaterialInfo = data.packed_material_info;
		}

		std::size_t size_gfx_static_decal_volume_packed([[maybe_unused]] const asset_data::gfx_static_decal_volume_packed& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfx_static_decal_volume_uid read_gfx_static_decal_volume_uid(const GfxStaticDecalVolumeUid& native)
		{
			asset_data::gfx_static_decal_volume_uid data;
			data.uid = native.uid;
			return data;
		}

		void write_gfx_static_decal_volume_uid(GfxStaticDecalVolumeUid& native, const asset_data::gfx_static_decal_volume_uid& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.uid = data.uid;
		}

		std::size_t size_gfx_static_decal_volume_uid([[maybe_unused]] const asset_data::gfx_static_decal_volume_uid& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfx_decal_volume_transient_data read_gfx_decal_volume_transient_data(const GfxDecalVolumeTransientData& native)
		{
			asset_data::gfx_decal_volume_transient_data data;
			data.decal_volume_count = native.decalVolumeCount;
			data.first_collection = native.firstCollection;
			data.decal_volumes = plain::read_array(native.decalVolumes, native.decalVolumeCount, "GfxDecalVolumeTransientData.decalVolumes", [&](const auto& item) { return read_gfx_static_decal_volume_packed(item); });
			data.decal_volumes_buffer = read_gfx_wrapped_buffer(native.decalVolumesBuffer);
			data.collection_bounds = plain::read_array(native.collectionBounds, native.collectionCount, "GfxDecalVolumeTransientData.collectionBounds", [&](const auto& item) { return read_bounds(item); });
			data.decal_volume_uids = plain::read_array(native.decalVolumeUids, native.decalVolumeCount, "GfxDecalVolumeTransientData.decalVolumeUids", [&](const auto& item) { return read_gfx_static_decal_volume_uid(item); });
			return data;
		}

		void write_gfx_decal_volume_transient_data(GfxDecalVolumeTransientData& native, const asset_data::gfx_decal_volume_transient_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.decalVolumeCount = data.decal_volume_count;
			native.firstCollection = data.first_collection;
			native.decalVolumes = plain::write_array<GfxStaticDecalVolumePacked>(data.decal_volumes, memory, [&](auto& out, const auto& in) { write_gfx_static_decal_volume_packed(out, in, resolve, memory); });
			write_gfx_wrapped_buffer(native.decalVolumesBuffer, data.decal_volumes_buffer, resolve, memory);
			native.collectionBounds = plain::write_array<Bounds>(data.collection_bounds, memory, [&](auto& out, const auto& in) { write_bounds(out, in, resolve, memory); });
			native.decalVolumeUids = plain::write_array<GfxStaticDecalVolumeUid>(data.decal_volume_uids, memory, [&](auto& out, const auto& in) { write_gfx_static_decal_volume_uid(out, in, resolve, memory); });
			native.collectionCount = plain::count_of<std::uint16_t>(data.collection_bounds);
		}

		std::size_t size_gfx_decal_volume_transient_data([[maybe_unused]] const asset_data::gfx_decal_volume_transient_data& data)
		{
			std::size_t size = 0;
			if (data.decal_volumes)
			{
				size += plain::rounded(std::max<std::size_t>(data.decal_volumes->size(), 1) * sizeof(GfxStaticDecalVolumePacked));
				for (const auto& item : *data.decal_volumes)
				{
					size += size_gfx_static_decal_volume_packed(item);
				}
			}
			size += size_gfx_wrapped_buffer(data.decal_volumes_buffer);
			if (data.collection_bounds)
			{
				size += plain::rounded(std::max<std::size_t>(data.collection_bounds->size(), 1) * sizeof(Bounds));
				for (const auto& item : *data.collection_bounds)
				{
					size += size_bounds(item);
				}
			}
			if (data.decal_volume_uids)
			{
				size += plain::rounded(std::max<std::size_t>(data.decal_volume_uids->size(), 1) * sizeof(GfxStaticDecalVolumeUid));
				for (const auto& item : *data.decal_volume_uids)
				{
					size += size_gfx_static_decal_volume_uid(item);
				}
			}
			return size;
		}

		asset_data::gfx_transient_zone read_gfx_transient_zone(const GfxWorldTransientZone& native)
		{
			asset_data::gfx_transient_zone data;
			data.name = plain::read_string(native.name);
			data.transient_zone_index = native.transientZoneIndex;
			data.draw_verts = read_gfx_world_draw_verts(native.drawVerts);
			data.draw_cells = read_gfx_world_draw_cells(native.drawCells);
			plain::require_zero(native.tempLightmapData, "GfxWorldTransientZone.tempLightmapData");
			data.gpu_light_grid = read_gfx_gpu_light_grid_transient_data(native.gpuLightGrid);
			data.reflection_probes = plain::read_single(native.reflectionProbes, [&](const auto& item) { return read_gfx_reflection_probe_transient_data(item); });
			data.collections = plain::read_array(native.collections, native.collectionCount, "GfxWorldTransientZone.collections", [&](const auto& item) { return item; });
			data.decal_volumes = plain::read_single(native.decalVolumes, [&](const auto& item) { return read_gfx_decal_volume_transient_data(item); });
			plain::require_zero(native.compressedSunShadow, "GfxWorldTransientZone.compressedSunShadow");
			return data;
		}

		void write_gfx_transient_zone(GfxWorldTransientZone& native, const asset_data::gfx_transient_zone& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.transientZoneIndex = data.transient_zone_index;
			write_gfx_world_draw_verts(native.drawVerts, data.draw_verts, resolve, memory);
			write_gfx_world_draw_cells(native.drawCells, data.draw_cells, resolve, memory);
			write_gfx_gpu_light_grid_transient_data(native.gpuLightGrid, data.gpu_light_grid, resolve, memory);
			native.reflectionProbes = plain::write_single<GfxReflectionProbeTransientData>(data.reflection_probes, memory, [&](auto& out, const auto& in) { write_gfx_reflection_probe_transient_data(out, in, resolve, memory); });
			native.collections = plain::write_array<std::uint16_t>(data.collections, memory, [&](auto& out, const auto& in) { out = in; });
			native.decalVolumes = plain::write_single<GfxDecalVolumeTransientData>(data.decal_volumes, memory, [&](auto& out, const auto& in) { write_gfx_decal_volume_transient_data(out, in, resolve, memory); });
			native.collectionCount = plain::count_of<std::uint32_t>(data.collections);
		}

		std::size_t size_gfx_transient_zone([[maybe_unused]] const asset_data::gfx_transient_zone& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			size += size_gfx_world_draw_verts(data.draw_verts);
			size += size_gfx_world_draw_cells(data.draw_cells);
			size += size_gfx_gpu_light_grid_transient_data(data.gpu_light_grid);
			if (data.reflection_probes)
			{
				size += plain::rounded(sizeof(GfxReflectionProbeTransientData));
				size += size_gfx_reflection_probe_transient_data(*data.reflection_probes);
			}
			if (data.collections)
			{
				size += plain::rounded(std::max<std::size_t>(data.collections->size(), 1) * sizeof(std::uint16_t));
			}
			if (data.decal_volumes)
			{
				size += plain::rounded(sizeof(GfxDecalVolumeTransientData));
				size += size_gfx_decal_volume_transient_data(*data.decal_volumes);
			}
			return size;
		}

		asset_data::gfx_fog_spline_data read_gfx_fog_spline_data(const GfxFogSplineData& native)
		{
			asset_data::gfx_fog_spline_data data;
			data.fog_start_dist = native.fogStartDist;
			data.fog_half_plane_distance = native.fogHalfPlaneDistance;
			data.fog_max_distance = native.fogMaxDistance;
			return data;
		}

		void write_gfx_fog_spline_data(GfxFogSplineData& native, const asset_data::gfx_fog_spline_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.fogStartDist = data.fog_start_dist;
			native.fogHalfPlaneDistance = data.fog_half_plane_distance;
			native.fogMaxDistance = data.fog_max_distance;
		}

		std::size_t size_gfx_fog_spline_data([[maybe_unused]] const asset_data::gfx_fog_spline_data& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::fog_spline read_fog_spline(const GfxFogSpline& native)
		{
			asset_data::fog_spline data;
			data.name = plain::read_string(native.name);
			data.image = plain::read_asset(ASSET_TYPE_IMAGE, native.image);
			data.data = read_gfx_fog_spline_data(native.data);
			return data;
		}

		void write_fog_spline(GfxFogSpline& native, const asset_data::fog_spline& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.image = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, data.image);
			write_gfx_fog_spline_data(native.data, data.data, resolve, memory);
		}

		std::size_t size_fog_spline([[maybe_unused]] const asset_data::fog_spline& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			size += size_gfx_fog_spline_data(data.data);
			return size;
		}

	}

	std::expected<asset_data::com_world, std::string> to_neutral(const ComWorld& native)
	{
		try
		{
			auto data = read_com_world(native);
			asset_data::validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	ComWorld* from_neutral(const asset_data::com_world& data, const asset_resolver& resolve, zone_memory& memory)
	{
		asset_data::validate(data);
		plain::context context(memory);
		auto* native = context.allocate<ComWorld>();
		write_com_world(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::com_world& data)
	{
		return plain::rounded(sizeof(ComWorld)) + size_com_world(data) + 64;
	}

	std::expected<asset_data::glass_world, std::string> to_neutral(const GlassWorld& native)
	{
		try
		{
			auto data = read_glass_world(native);
			asset_data::validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	GlassWorld* from_neutral(const asset_data::glass_world& data, const asset_resolver& resolve, zone_memory& memory)
	{
		asset_data::validate(data);
		plain::context context(memory);
		auto* native = context.allocate<GlassWorld>();
		write_glass_world(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::glass_world& data)
	{
		return plain::rounded(sizeof(GlassWorld)) + size_glass_world(data) + 64;
	}

	std::expected<asset_data::map_edge_list, std::string> to_neutral(const MapEdgeList& native)
	{
		try
		{
			auto data = read_map_edge_list(native);
			asset_data::validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	MapEdgeList* from_neutral(const asset_data::map_edge_list& data, const asset_resolver& resolve, zone_memory& memory)
	{
		asset_data::validate(data);
		plain::context context(memory);
		auto* native = context.allocate<MapEdgeList>();
		write_map_edge_list(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::map_edge_list& data)
	{
		return plain::rounded(sizeof(MapEdgeList)) + size_map_edge_list(data) + 64;
	}

	std::expected<asset_data::nav_mesh, std::string> to_neutral(const NavMeshData& native)
	{
		try
		{
			auto data = read_nav_mesh(native);
			asset_data::validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	NavMeshData* from_neutral(const asset_data::nav_mesh& data, const asset_resolver& resolve, zone_memory& memory)
	{
		asset_data::validate(data);
		plain::context context(memory);
		auto* native = context.allocate<NavMeshData>();
		write_nav_mesh(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::nav_mesh& data)
	{
		return plain::rounded(sizeof(NavMeshData)) + size_nav_mesh(data) + 64;
	}

	std::expected<asset_data::tactical_graph_data, std::string> to_neutral(const TacticalGraphData& native)
	{
		try
		{
			auto data = read_tactical_graph_data(native);
			asset_data::validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	TacticalGraphData* from_neutral(const asset_data::tactical_graph_data& data, const asset_resolver& resolve, zone_memory& memory)
	{
		asset_data::validate(data);
		plain::context context(memory);
		auto* native = context.allocate<TacticalGraphData>();
		write_tactical_graph_data(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::tactical_graph_data& data)
	{
		return plain::rounded(sizeof(TacticalGraphData)) + size_tactical_graph_data(data) + 64;
	}

	std::expected<asset_data::clip_map, std::string> to_neutral(const clipMap_t& native)
	{
		try
		{
			auto data = read_clip_map(native);
			asset_data::validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	clipMap_t* from_neutral(const asset_data::clip_map& data, const asset_resolver& resolve, zone_memory& memory)
	{
		asset_data::validate(data);
		plain::context context(memory);
		auto* native = context.allocate<clipMap_t>();
		write_clip_map(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::clip_map& data)
	{
		return plain::rounded(sizeof(clipMap_t)) + size_clip_map(data) + 64;
	}

	std::expected<asset_data::map_ents, std::string> to_neutral(const MapEnts& native)
	{
		try
		{
			auto data = read_map_ents(native);
			asset_data::validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	MapEnts* from_neutral(const asset_data::map_ents& data, const asset_resolver& resolve, zone_memory& memory)
	{
		asset_data::validate(data);
		plain::context context(memory);
		auto* native = context.allocate<MapEnts>();
		write_map_ents(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::map_ents& data)
	{
		return plain::rounded(sizeof(MapEnts)) + size_map_ents(data) + 64;
	}

	std::expected<asset_data::gfx_transient_zone, std::string> to_neutral(const GfxWorldTransientZone& native)
	{
		try
		{
			auto data = read_gfx_transient_zone(native);
			asset_data::validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	GfxWorldTransientZone* from_neutral(const asset_data::gfx_transient_zone& data, const asset_resolver& resolve, zone_memory& memory)
	{
		asset_data::validate(data);
		plain::context context(memory);
		auto* native = context.allocate<GfxWorldTransientZone>();
		write_gfx_transient_zone(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::gfx_transient_zone& data)
	{
		return plain::rounded(sizeof(GfxWorldTransientZone)) + size_gfx_transient_zone(data) + 64;
	}

	std::expected<asset_data::fog_spline, std::string> to_neutral(const GfxFogSpline& native)
	{
		try
		{
			auto data = read_fog_spline(native);
			asset_data::validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	GfxFogSpline* from_neutral(const asset_data::fog_spline& data, const asset_resolver& resolve, zone_memory& memory)
	{
		asset_data::validate(data);
		plain::context context(memory);
		auto* native = context.allocate<GfxFogSpline>();
		write_fog_spline(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::fog_spline& data)
	{
		return plain::rounded(sizeof(GfxFogSpline)) + size_fog_spline(data) + 64;
	}

}
