// Emitted by tools/gen_iw8_neutral.py from tools/neutral_families/world.json: do not edit.
#include <std_include.hpp>
#include "world.hpp"
#include "plain.hpp"

namespace zonetool::iw8::neutral
{
	namespace
	{
		asset_data::world::com_world read_com_world(const ComWorld& native);
		void write_com_world(ComWorld& native, const asset_data::world::com_world& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_com_world(const asset_data::world::com_world& data);
		asset_data::world::com_primary_light read_com_primary_light(const ComPrimaryLight& native);
		void write_com_primary_light(ComPrimaryLight& native, const asset_data::world::com_primary_light& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_com_primary_light(const asset_data::world::com_primary_light& data);
		asset_data::world::com_change_list_info read_com_change_list_info(const ComChangeListInfo& native);
		void write_com_change_list_info(ComChangeListInfo& native, const asset_data::world::com_change_list_info& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_com_change_list_info(const asset_data::world::com_change_list_info& data);
		asset_data::world::bitarray_384 read_bitarray_384(const bitarray_384& native);
		void write_bitarray_384(bitarray_384& native, const asset_data::world::bitarray_384& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_bitarray_384(const asset_data::world::bitarray_384& data);
		asset_data::world::glass_world read_glass_world(const GlassWorld& native);
		void write_glass_world(GlassWorld& native, const asset_data::world::glass_world& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_glass_world(const asset_data::world::glass_world& data);
		asset_data::world::glass_data read_glass_data(const G_GlassData& native);
		void write_glass_data(G_GlassData& native, const asset_data::world::glass_data& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_glass_data(const asset_data::world::glass_data& data);
		asset_data::world::glass_piece read_glass_piece(const G_GlassPiece& native);
		void write_glass_piece(G_GlassPiece& native, const asset_data::world::glass_piece& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_glass_piece(const asset_data::world::glass_piece& data);
		asset_data::world::glass_name read_glass_name(const G_GlassName& native);
		void write_glass_name(G_GlassName& native, const asset_data::world::glass_name& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_glass_name(const asset_data::world::glass_name& data);
		asset_data::world::spatial_partition_tree read_spatial_partition_tree(const SpatialPartition_Tree& native);
		void write_spatial_partition_tree(SpatialPartition_Tree& native, const asset_data::world::spatial_partition_tree& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_spatial_partition_tree(const asset_data::world::spatial_partition_tree& data);
		asset_data::world::map_edge_list read_map_edge_list(const MapEdgeList& native);
		void write_map_edge_list(MapEdgeList& native, const asset_data::world::map_edge_list& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_map_edge_list(const asset_data::world::map_edge_list& data);
		asset_data::world::flagged_edge_metadata read_flagged_edge_metadata(const FlaggedEdgeMetadata& native);
		void write_flagged_edge_metadata(FlaggedEdgeMetadata& native, const asset_data::world::flagged_edge_metadata& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_flagged_edge_metadata(const asset_data::world::flagged_edge_metadata& data);
		asset_data::world::pmr_octree_metadata read_pmr_octree_metadata(const PMROctreeMetadata& native);
		void write_pmr_octree_metadata(PMROctreeMetadata& native, const asset_data::world::pmr_octree_metadata& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_pmr_octree_metadata(const asset_data::world::pmr_octree_metadata& data);
		asset_data::world::pmr_octree_node read_pmr_octree_node(const PMROctreeNode& native);
		void write_pmr_octree_node(PMROctreeNode& native, const asset_data::world::pmr_octree_node& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_pmr_octree_node(const asset_data::world::pmr_octree_node& data);
		asset_data::world::pmr_octree_node_set read_pmr_octree_node_set(const PMROctreeNodeSet& native);
		void write_pmr_octree_node_set(PMROctreeNodeSet& native, const asset_data::world::pmr_octree_node_set& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_pmr_octree_node_set(const asset_data::world::pmr_octree_node_set& data);
		asset_data::world::edge_adjacency_metadata read_edge_adjacency_metadata(const EdgeAdjacencyMetadata& native);
		void write_edge_adjacency_metadata(EdgeAdjacencyMetadata& native, const asset_data::world::edge_adjacency_metadata& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_edge_adjacency_metadata(const asset_data::world::edge_adjacency_metadata& data);
		asset_data::world::edge_adjacency read_edge_adjacency(const EdgeAdjacency& native);
		void write_edge_adjacency(EdgeAdjacency& native, const asset_data::world::edge_adjacency& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_edge_adjacency(const asset_data::world::edge_adjacency& data);
		asset_data::world::nav_mesh read_nav_mesh(const NavMeshData& native);
		void write_nav_mesh(NavMeshData& native, const asset_data::world::nav_mesh& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_nav_mesh(const asset_data::world::nav_mesh& data);
		asset_data::world::nav_resource_s read_nav_resource_s(const nav_resource_s& native);
		void write_nav_resource_s(nav_resource_s& native, const asset_data::world::nav_resource_s& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_nav_resource_s(const asset_data::world::nav_resource_s& data);
		asset_data::world::nav_obstacle_bounds_s read_nav_obstacle_bounds_s(const nav_obstacle_bounds_s& native);
		void write_nav_obstacle_bounds_s(nav_obstacle_bounds_s& native, const asset_data::world::nav_obstacle_bounds_s& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_nav_obstacle_bounds_s(const asset_data::world::nav_obstacle_bounds_s& data);
		asset_data::world::nav_obstacle_hull_s read_nav_obstacle_hull_s(const nav_obstacle_hull_s& native);
		void write_nav_obstacle_hull_s(nav_obstacle_hull_s& native, const asset_data::world::nav_obstacle_hull_s& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_nav_obstacle_hull_s(const asset_data::world::nav_obstacle_hull_s& data);
		asset_data::world::nav_boundary_plane_s read_nav_boundary_plane_s(const nav_boundary_plane_s& native);
		void write_nav_boundary_plane_s(nav_boundary_plane_s& native, const asset_data::world::nav_boundary_plane_s& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_nav_boundary_plane_s(const asset_data::world::nav_boundary_plane_s& data);
		asset_data::world::nav_glass_bounds_s read_nav_glass_bounds_s(const nav_glass_bounds_s& native);
		void write_nav_glass_bounds_s(nav_glass_bounds_s& native, const asset_data::world::nav_glass_bounds_s& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_nav_glass_bounds_s(const asset_data::world::nav_glass_bounds_s& data);
		asset_data::world::nav_modifier_s read_nav_modifier_s(const nav_modifier_s& native);
		void write_nav_modifier_s(nav_modifier_s& native, const asset_data::world::nav_modifier_s& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_nav_modifier_s(const asset_data::world::nav_modifier_s& data);
		asset_data::world::nav_link_creation_data_s read_nav_link_creation_data_s(const nav_link_creation_data_s& native);
		void write_nav_link_creation_data_s(nav_link_creation_data_s& native, const asset_data::world::nav_link_creation_data_s& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_nav_link_creation_data_s(const asset_data::world::nav_link_creation_data_s& data);
		asset_data::world::nav_link_point_s read_nav_link_point_s(const nav_link_point_s& native);
		void write_nav_link_point_s(nav_link_point_s& native, const asset_data::world::nav_link_point_s& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_nav_link_point_s(const asset_data::world::nav_link_point_s& data);
		asset_data::world::nav_raw_volume_s read_nav_raw_volume_s(const nav_raw_volume_s& native);
		void write_nav_raw_volume_s(nav_raw_volume_s& native, const asset_data::world::nav_raw_volume_s& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_nav_raw_volume_s(const asset_data::world::nav_raw_volume_s& data);
		asset_data::world::nav_raw_custom_volume_s read_nav_raw_custom_volume_s(const nav_raw_custom_volume_s& native);
		void write_nav_raw_custom_volume_s(nav_raw_custom_volume_s& native, const asset_data::world::nav_raw_custom_volume_s& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_nav_raw_custom_volume_s(const asset_data::world::nav_raw_custom_volume_s& data);
		asset_data::world::nav_raw_custom_volume_tri_s read_nav_raw_custom_volume_tri_s(const nav_raw_custom_volume_tri_s& native);
		void write_nav_raw_custom_volume_tri_s(nav_raw_custom_volume_tri_s& native, const asset_data::world::nav_raw_custom_volume_tri_s& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_nav_raw_custom_volume_tri_s(const asset_data::world::nav_raw_custom_volume_tri_s& data);
		asset_data::world::nav_misc_s read_nav_misc_s(const nav_misc_s& native);
		void write_nav_misc_s(nav_misc_s& native, const asset_data::world::nav_misc_s& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_nav_misc_s(const asset_data::world::nav_misc_s& data);
		asset_data::world::clip_map read_clip_map(const clipMap_t& native);
		void write_clip_map(clipMap_t& native, const asset_data::world::clip_map& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_clip_map(const asset_data::world::clip_map& data);
		asset_data::world::static_model_collision_model_list read_static_model_collision_model_list(const StaticModelCollisionModelList& native);
		void write_static_model_collision_model_list(StaticModelCollisionModelList& native, const asset_data::world::static_model_collision_model_list& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_static_model_collision_model_list(const asset_data::world::static_model_collision_model_list& data);
		asset_data::world::static_model_collision_model read_static_model_collision_model(const StaticModelCollisionModel& native);
		void write_static_model_collision_model(StaticModelCollisionModel& native, const asset_data::world::static_model_collision_model& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_static_model_collision_model(const asset_data::world::static_model_collision_model& data);
		asset_data::world::static_model_collision_instance read_static_model_collision_instance(const StaticModelCollisionInstance& native);
		void write_static_model_collision_instance(StaticModelCollisionInstance& native, const asset_data::world::static_model_collision_instance& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_static_model_collision_instance(const asset_data::world::static_model_collision_instance& data);
		asset_data::world::stage read_stage(const Stage& native);
		void write_stage(Stage& native, const asset_data::world::stage& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_stage(const asset_data::world::stage& data);
		asset_data::world::map_triggers read_map_triggers(const MapTriggers& native);
		void write_map_triggers(MapTriggers& native, const asset_data::world::map_triggers& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_map_triggers(const asset_data::world::map_triggers& data);
		asset_data::world::trigger_model read_trigger_model(const TriggerModel& native);
		void write_trigger_model(TriggerModel& native, const asset_data::world::trigger_model& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_trigger_model(const asset_data::world::trigger_model& data);
		asset_data::world::trigger_hull read_trigger_hull(const TriggerHull& native);
		void write_trigger_hull(TriggerHull& native, const asset_data::world::trigger_hull& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_trigger_hull(const asset_data::world::trigger_hull& data);
		asset_data::world::bounds read_bounds(const Bounds& native);
		void write_bounds(Bounds& native, const asset_data::world::bounds& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_bounds(const asset_data::world::bounds& data);
		asset_data::world::trigger_slab read_trigger_slab(const TriggerSlab& native);
		void write_trigger_slab(TriggerSlab& native, const asset_data::world::trigger_slab& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_trigger_slab(const asset_data::world::trigger_slab& data);
		asset_data::world::trigger_winding read_trigger_winding(const TriggerWinding& native);
		void write_trigger_winding(TriggerWinding& native, const asset_data::world::trigger_winding& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_trigger_winding(const asset_data::world::trigger_winding& data);
		asset_data::world::trigger_winding_point read_trigger_winding_point(const TriggerWindingPoint& native);
		void write_trigger_winding_point(TriggerWindingPoint& native, const asset_data::world::trigger_winding_point& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_trigger_winding_point(const asset_data::world::trigger_winding_point& data);
		asset_data::world::physics_capacities read_physics_capacities(const PhysicsCapacities& native);
		void write_physics_capacities(PhysicsCapacities& native, const asset_data::world::physics_capacities& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_capacities(const asset_data::world::physics_capacities& data);
		asset_data::world::collision_heatmap_entry read_collision_heatmap_entry(const CollisionHeatmapEntry& native);
		void write_collision_heatmap_entry(CollisionHeatmapEntry& native, const asset_data::world::collision_heatmap_entry& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_collision_heatmap_entry(const asset_data::world::collision_heatmap_entry& data);
		asset_data::world::top_down_map_data read_top_down_map_data(const TopDownMapData& native);
		void write_top_down_map_data(TopDownMapData& native, const asset_data::world::top_down_map_data& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_top_down_map_data(const asset_data::world::top_down_map_data& data);
		asset_data::world::top_down_map_data_header read_top_down_map_data_header(const TopDownMapDataHeader& native);
		void write_top_down_map_data_header(TopDownMapDataHeader& native, const asset_data::world::top_down_map_data_header& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_top_down_map_data_header(const asset_data::world::top_down_map_data_header& data);
		asset_data::world::top_down_map_data_content read_top_down_map_data_content(const TopDownMapDataContent& native);
		void write_top_down_map_data_content(TopDownMapDataContent& native, const asset_data::world::top_down_map_data_content& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_top_down_map_data_content(const asset_data::world::top_down_map_data_content& data);
		asset_data::world::fog_spline read_fog_spline(const GfxFogSpline& native);
		void write_fog_spline(GfxFogSpline& native, const asset_data::world::fog_spline& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_fog_spline(const asset_data::world::fog_spline& data);
		asset_data::world::gfx_fog_spline_data read_gfx_fog_spline_data(const GfxFogSplineData& native);
		void write_gfx_fog_spline_data(GfxFogSplineData& native, const asset_data::world::gfx_fog_spline_data& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_fog_spline_data(const asset_data::world::gfx_fog_spline_data& data);

		asset_data::world::com_change_list_info read_com_change_list_info(const ComChangeListInfo& native)
		{
			plain::trace("read_com_change_list_info");
			asset_data::world::com_change_list_info data;
			data.change_list_number = native.changeListNumber;
			data.time = native.time;
			data.user_name = plain::read_string(native.userName);
			return data;
		}

		void write_com_change_list_info(ComChangeListInfo& native, const asset_data::world::com_change_list_info& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.changeListNumber = data.change_list_number;
			native.time = data.time;
			native.userName = plain::write_string(memory, data.user_name);
		}

		std::size_t size_com_change_list_info([[maybe_unused]] const asset_data::world::com_change_list_info& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.user_name);
			return size;
		}

		asset_data::world::bitarray_384 read_bitarray_384(const bitarray_384& native)
		{
			plain::trace("read_bitarray_384");
			asset_data::world::bitarray_384 data;
			data.array = std::to_array(native.array);
			return data;
		}

		void write_bitarray_384(bitarray_384& native, const asset_data::world::bitarray_384& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			std::ranges::copy(data.array, native.array);
		}

		std::size_t size_bitarray_384([[maybe_unused]] const asset_data::world::bitarray_384& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::world::com_world read_com_world(const ComWorld& native)
		{
			plain::trace("read_com_world");
			asset_data::world::com_world data;
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

		void write_com_world(ComWorld& native, const asset_data::world::com_world& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
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

		std::size_t size_com_world([[maybe_unused]] const asset_data::world::com_world& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.primary_lights)
			{
				size += plain::sized(std::max<std::size_t>(data.primary_lights->size(), 1) * sizeof(ComPrimaryLight));
				for (const auto& item : *data.primary_lights)
				{
					size += size_com_primary_light(item);
				}
			}
			if (data.transient_table)
			{
				size += plain::sized(std::max<std::size_t>(data.transient_table->size(), 1) * sizeof(std::uint16_t));
			}
			size += size_com_change_list_info(data.change_list_info);
			if (data.umbra_gate_names)
			{
				size += plain::sized(std::max<std::size_t>(data.umbra_gate_names->size(), 1) * sizeof(const char*));
				for (const auto& item : *data.umbra_gate_names)
				{
					size += plain::string_size(item);
				}
			}
			size += size_bitarray_384(data.umbra_gate_initial_states);
			return size;
		}

		asset_data::world::com_primary_light read_com_primary_light(const ComPrimaryLight& native)
		{
			plain::trace("read_com_primary_light");
			asset_data::world::com_primary_light data;
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

		void write_com_primary_light(ComPrimaryLight& native, const asset_data::world::com_primary_light& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
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

		std::size_t size_com_primary_light([[maybe_unused]] const asset_data::world::com_primary_light& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.def_name);
			return size;
		}

		asset_data::world::spatial_partition_tree read_spatial_partition_tree(const SpatialPartition_Tree& native)
		{
			plain::trace("read_spatial_partition_tree");
			asset_data::world::spatial_partition_tree data;
			data.tree = plain::read_array(native.tree, native.treeSize, "SpatialPartition_Tree.tree", [&](const auto& item) { return item; });
			data.always_list = plain::read_array(native.alwaysList, native.alwaysListLength, "SpatialPartition_Tree.alwaysList", [&](const auto& item) { return item; });
			return data;
		}

		void write_spatial_partition_tree(SpatialPartition_Tree& native, const asset_data::world::spatial_partition_tree& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.tree = plain::write_array<std::uint8_t>(data.tree, memory, [&](auto& out, const auto& in) { out = in; });
			native.alwaysList = plain::write_array<std::uint32_t>(data.always_list, memory, [&](auto& out, const auto& in) { out = in; });
			native.treeSize = plain::count_of<std::uint32_t>(data.tree);
			native.alwaysListLength = plain::count_of<std::uint32_t>(data.always_list);
		}

		std::size_t size_spatial_partition_tree([[maybe_unused]] const asset_data::world::spatial_partition_tree& data)
		{
			std::size_t size = 0;
			if (data.tree)
			{
				size += plain::sized(std::max<std::size_t>(data.tree->size(), 1) * sizeof(std::uint8_t));
			}
			if (data.always_list)
			{
				size += plain::sized(std::max<std::size_t>(data.always_list->size(), 1) * sizeof(std::uint32_t));
			}
			return size;
		}

		asset_data::world::glass_data read_glass_data(const G_GlassData& native)
		{
			plain::trace("read_glass_data");
			asset_data::world::glass_data data;
			data.glass_pieces = plain::read_array(native.glassPieces, native.pieceCount, "G_GlassData.glassPieces", [&](const auto& item) { return read_glass_piece(item); });
			data.damage_to_weaken = native.damageToWeaken;
			data.damage_to_destroy = native.damageToDestroy;
			data.glass_names = plain::read_array(native.glassNames, native.glassNameCount, "G_GlassData.glassNames", [&](const auto& item) { return read_glass_name(item); });
			data.spatial_tree = plain::read_single(native.spatialTree, [&](const auto& item) { return read_spatial_partition_tree(item); });
			return data;
		}

		void write_glass_data(G_GlassData& native, const asset_data::world::glass_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.glassPieces = plain::write_array<G_GlassPiece>(data.glass_pieces, memory, [&](auto& out, const auto& in) { write_glass_piece(out, in, resolve, memory); });
			native.damageToWeaken = data.damage_to_weaken;
			native.damageToDestroy = data.damage_to_destroy;
			native.glassNames = plain::write_array<G_GlassName>(data.glass_names, memory, [&](auto& out, const auto& in) { write_glass_name(out, in, resolve, memory); });
			native.spatialTree = plain::write_single<SpatialPartition_Tree>(data.spatial_tree, memory, [&](auto& out, const auto& in) { write_spatial_partition_tree(out, in, resolve, memory); });
			native.pieceCount = plain::count_of<std::uint32_t>(data.glass_pieces);
			native.glassNameCount = plain::count_of<std::uint32_t>(data.glass_names);
		}

		std::size_t size_glass_data([[maybe_unused]] const asset_data::world::glass_data& data)
		{
			std::size_t size = 0;
			if (data.glass_pieces)
			{
				size += plain::sized(std::max<std::size_t>(data.glass_pieces->size(), 1) * sizeof(G_GlassPiece));
				for (const auto& item : *data.glass_pieces)
				{
					size += size_glass_piece(item);
				}
			}
			if (data.glass_names)
			{
				size += plain::sized(std::max<std::size_t>(data.glass_names->size(), 1) * sizeof(G_GlassName));
				for (const auto& item : *data.glass_names)
				{
					size += size_glass_name(item);
				}
			}
			if (data.spatial_tree)
			{
				size += plain::sized(sizeof(SpatialPartition_Tree));
				size += size_spatial_partition_tree(*data.spatial_tree);
			}
			return size;
		}

		asset_data::world::glass_world read_glass_world(const GlassWorld& native)
		{
			plain::trace("read_glass_world");
			asset_data::world::glass_world data;
			data.name = plain::read_string(native.name);
			data.g_glass_data = plain::read_single(native.g_glassData, [&](const auto& item) { return read_glass_data(item); });
			return data;
		}

		void write_glass_world(GlassWorld& native, const asset_data::world::glass_world& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.g_glassData = plain::write_single<G_GlassData>(data.g_glass_data, memory, [&](auto& out, const auto& in) { write_glass_data(out, in, resolve, memory); });
		}

		std::size_t size_glass_world([[maybe_unused]] const asset_data::world::glass_world& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.g_glass_data)
			{
				size += plain::sized(sizeof(G_GlassData));
				size += size_glass_data(*data.g_glass_data);
			}
			return size;
		}

		asset_data::world::glass_piece read_glass_piece(const G_GlassPiece& native)
		{
			plain::trace("read_glass_piece");
			asset_data::world::glass_piece data;
			data.damage_taken = native.damageTaken;
			data.collapse_time = native.collapseTime;
			data.last_state_change_time = native.lastStateChangeTime;
			data.impact_dir = native.impactDir;
			data.impact_pos = std::to_array(native.impactPos);
			return data;
		}

		void write_glass_piece(G_GlassPiece& native, const asset_data::world::glass_piece& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.damageTaken = data.damage_taken;
			native.collapseTime = data.collapse_time;
			native.lastStateChangeTime = data.last_state_change_time;
			native.impactDir = data.impact_dir;
			std::ranges::copy(data.impact_pos, native.impactPos);
		}

		std::size_t size_glass_piece([[maybe_unused]] const asset_data::world::glass_piece& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::world::glass_name read_glass_name(const G_GlassName& native)
		{
			plain::trace("read_glass_name");
			asset_data::world::glass_name data;
			data.name_str = plain::read_string(native.nameStr);
			data.name = plain::read_script_string(native.name);
			data.piece_indices = plain::read_array(native.pieceIndices, native.pieceCount, "G_GlassName.pieceIndices", [&](const auto& item) { return item; });
			return data;
		}

		void write_glass_name(G_GlassName& native, const asset_data::world::glass_name& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.nameStr = plain::write_string(memory, data.name_str);
			native.name = static_cast<scr_string_t>(plain::write_script_string(data.name));
			native.pieceIndices = plain::write_array<std::uint16_t>(data.piece_indices, memory, [&](auto& out, const auto& in) { out = in; });
			native.pieceCount = plain::count_of<std::uint16_t>(data.piece_indices);
		}

		std::size_t size_glass_name([[maybe_unused]] const asset_data::world::glass_name& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name_str);
			if (data.piece_indices)
			{
				size += plain::sized(std::max<std::size_t>(data.piece_indices->size(), 1) * sizeof(std::uint16_t));
			}
			return size;
		}

		asset_data::world::map_edge_list read_map_edge_list(const MapEdgeList& native)
		{
			plain::trace("read_map_edge_list");
			asset_data::world::map_edge_list data;
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

		void write_map_edge_list(MapEdgeList& native, const asset_data::world::map_edge_list& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
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

		std::size_t size_map_edge_list([[maybe_unused]] const asset_data::world::map_edge_list& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.edges)
			{
				size += plain::sized(std::max<std::size_t>(data.edges->size(), 1) * 2 * sizeof(float4));
			}
			if (data.edge_metadata)
			{
				size += plain::sized(std::max<std::size_t>(data.edge_metadata->size(), 1) * sizeof(FlaggedEdgeMetadata));
				for (const auto& item : *data.edge_metadata)
				{
					size += size_flagged_edge_metadata(item);
				}
			}
			if (data.edge_octrees)
			{
				size += plain::sized(std::max<std::size_t>(data.edge_octrees->size(), 1) * sizeof(PMROctreeMetadata));
				for (const auto& item : *data.edge_octrees)
				{
					size += size_pmr_octree_metadata(item);
				}
			}
			if (data.edge_octree_node_sets)
			{
				size += plain::sized(std::max<std::size_t>(data.edge_octree_node_sets->size(), 1) * sizeof(PMROctreeNodeSet));
				for (const auto& item : *data.edge_octree_node_sets)
				{
					size += size_pmr_octree_node_set(item);
				}
			}
			if (data.edge_indices)
			{
				size += plain::sized(std::max<std::size_t>(data.edge_indices->size(), 1) * sizeof(std::uint32_t));
			}
			if (data.edge_adjacency_metadata)
			{
				size += plain::sized(std::max<std::size_t>(data.edge_adjacency_metadata->size(), 1) * sizeof(EdgeAdjacencyMetadata));
				for (const auto& item : *data.edge_adjacency_metadata)
				{
					size += size_edge_adjacency_metadata(item);
				}
			}
			if (data.edge_adjacency)
			{
				size += plain::sized(std::max<std::size_t>(data.edge_adjacency->size(), 1) * sizeof(EdgeAdjacency));
				for (const auto& item : *data.edge_adjacency)
				{
					size += size_edge_adjacency(item);
				}
			}
			return size;
		}

		asset_data::world::flagged_edge_metadata read_flagged_edge_metadata(const FlaggedEdgeMetadata& native)
		{
			plain::trace("read_flagged_edge_metadata");
			asset_data::world::flagged_edge_metadata data;
			data.packed_adjacent_normal = native.packedAdjacentNormal;
			data.packed_open_angle = native.packedOpenAngle;
			data.flags = std::to_array(native.flags);
			return data;
		}

		void write_flagged_edge_metadata(FlaggedEdgeMetadata& native, const asset_data::world::flagged_edge_metadata& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.packedAdjacentNormal = data.packed_adjacent_normal;
			native.packedOpenAngle = data.packed_open_angle;
			std::ranges::copy(data.flags, native.flags);
		}

		std::size_t size_flagged_edge_metadata([[maybe_unused]] const asset_data::world::flagged_edge_metadata& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::world::pmr_octree_node read_pmr_octree_node(const PMROctreeNode& native)
		{
			plain::trace("read_pmr_octree_node");
			asset_data::world::pmr_octree_node data;
			data.uniform_grid_index = std::to_array(native.uniformGridIndex);
			data.depth = native.depth;
			data.flags = plain::flag_names(asset_data::world::pmr_octree_flags_names, static_cast<std::uint64_t>(native.flags), "PMROctreeNode.flags");
			data.parent_node_index_or_q_edge_table_index = plain::read_scalars<std::uint32_t, 1>(&native.___u3)[0];
			data.child_node_set_index_or_q_edge_table_count = plain::read_scalars<std::uint32_t, 1>(&native.___u4)[0];
			return data;
		}

		void write_pmr_octree_node(PMROctreeNode& native, const asset_data::world::pmr_octree_node& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			std::ranges::copy(data.uniform_grid_index, native.uniformGridIndex);
			native.depth = data.depth;
			native.flags = static_cast<PMROctreeFlags>(plain::flag_value(asset_data::world::pmr_octree_flags_names, data.flags, "PMROctreeNode.flags"));
			plain::write_scalars(&native.___u3, std::array<std::uint32_t, 1>{data.parent_node_index_or_q_edge_table_index});
			plain::write_scalars(&native.___u4, std::array<std::uint32_t, 1>{data.child_node_set_index_or_q_edge_table_count});
		}

		std::size_t size_pmr_octree_node([[maybe_unused]] const asset_data::world::pmr_octree_node& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::world::pmr_octree_metadata read_pmr_octree_metadata(const PMROctreeMetadata& native)
		{
			plain::trace("read_pmr_octree_metadata");
			asset_data::world::pmr_octree_metadata data;
			data.node_half_width = [&] { std::array<std::array<float, 4>, 16> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = plain::read_scalars<float, 4>(&native.nodeHalfWidth[i]); } return out; }();
			data.min_extent = plain::read_scalars<float, 4>(&native.minExtent);
			data.root_node = read_pmr_octree_node(native.rootNode);
			data.node_set_count = native.nodeSetCount;
			data.node_bounding_radius_sq = std::to_array(native.nodeBoundingRadiusSq);
			data.edge_bucket = plain::enum_name(asset_data::world::edge_query_bucket_t_names, static_cast<std::int64_t>(native.edgeBucket), "PMROctreeMetadata.edgeBucket");
			data.flags = std::to_array(native.flags);
			plain::require_zero(native.pad, "PMROctreeMetadata.pad");
			return data;
		}

		void write_pmr_octree_metadata(PMROctreeMetadata& native, const asset_data::world::pmr_octree_metadata& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			for (std::size_t i = 0; i < data.node_half_width.size(); ++i)
			{
				plain::write_scalars(&native.nodeHalfWidth[i], data.node_half_width[i]);
			}
			plain::write_scalars(&native.minExtent, data.min_extent);
			write_pmr_octree_node(native.rootNode, data.root_node, resolve, memory);
			native.nodeSetCount = data.node_set_count;
			std::ranges::copy(data.node_bounding_radius_sq, native.nodeBoundingRadiusSq);
			native.edgeBucket = static_cast<edgeQueryBucket_t>(plain::enum_value(asset_data::world::edge_query_bucket_t_names, data.edge_bucket, "PMROctreeMetadata.edgeBucket"));
			std::ranges::copy(data.flags, native.flags);
		}

		std::size_t size_pmr_octree_metadata([[maybe_unused]] const asset_data::world::pmr_octree_metadata& data)
		{
			std::size_t size = 0;
			size += size_pmr_octree_node(data.root_node);
			return size;
		}

		asset_data::world::pmr_octree_node_set read_pmr_octree_node_set(const PMROctreeNodeSet& native)
		{
			plain::trace("read_pmr_octree_node_set");
			asset_data::world::pmr_octree_node_set data;
			data.node = [&] { std::array<asset_data::world::pmr_octree_node, 8> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_pmr_octree_node(native.node[i]); } return out; }();
			return data;
		}

		void write_pmr_octree_node_set(PMROctreeNodeSet& native, const asset_data::world::pmr_octree_node_set& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			for (std::size_t i = 0; i < data.node.size(); ++i)
			{
				write_pmr_octree_node(native.node[i], data.node[i], resolve, memory);
			}
		}

		std::size_t size_pmr_octree_node_set([[maybe_unused]] const asset_data::world::pmr_octree_node_set& data)
		{
			std::size_t size = 0;
			for (const auto& item : data.node)
			{
				size += size_pmr_octree_node(item);
			}
			return size;
		}

		asset_data::world::edge_adjacency_metadata read_edge_adjacency_metadata(const EdgeAdjacencyMetadata& native)
		{
			plain::trace("read_edge_adjacency_metadata");
			asset_data::world::edge_adjacency_metadata data;
			data.start_index = native.startIndex;
			data.mount_count = native.mountCount;
			data.interaction_count = native.interactionCount;
			return data;
		}

		void write_edge_adjacency_metadata(EdgeAdjacencyMetadata& native, const asset_data::world::edge_adjacency_metadata& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.startIndex = data.start_index;
			native.mountCount = data.mount_count;
			native.interactionCount = data.interaction_count;
		}

		std::size_t size_edge_adjacency_metadata([[maybe_unused]] const asset_data::world::edge_adjacency_metadata& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::world::edge_adjacency read_edge_adjacency(const EdgeAdjacency& native)
		{
			plain::trace("read_edge_adjacency");
			asset_data::world::edge_adjacency data;
			data.start = native.start;
			data.end = native.end;
			data.adj_edge = native.adjEdge;
			data.adj_start = native.adjStart;
			data.adj_end = native.adjEnd;
			return data;
		}

		void write_edge_adjacency(EdgeAdjacency& native, const asset_data::world::edge_adjacency& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.start = data.start;
			native.end = data.end;
			native.adjEdge = data.adj_edge;
			native.adjStart = data.adj_start;
			native.adjEnd = data.adj_end;
		}

		std::size_t size_edge_adjacency([[maybe_unused]] const asset_data::world::edge_adjacency& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::world::nav_misc_s read_nav_misc_s(const nav_misc_s& native)
		{
			plain::trace("read_nav_misc_s");
			asset_data::world::nav_misc_s data;
			data.m_static_bounds_half_size = plain::read_scalars<float, 3>(&native.m_StaticBoundsHalfSize);
			data.m_static_bounds_mid_point = plain::read_scalars<float, 3>(&native.m_StaticBoundsMidPoint);
			return data;
		}

		void write_nav_misc_s(nav_misc_s& native, const asset_data::world::nav_misc_s& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.m_StaticBoundsHalfSize, data.m_static_bounds_half_size);
			plain::write_scalars(&native.m_StaticBoundsMidPoint, data.m_static_bounds_mid_point);
		}

		std::size_t size_nav_misc_s([[maybe_unused]] const asset_data::world::nav_misc_s& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::world::nav_mesh read_nav_mesh(const NavMeshData& native)
		{
			plain::trace("read_nav_mesh");
			asset_data::world::nav_mesh data;
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

		void write_nav_mesh(NavMeshData& native, const asset_data::world::nav_mesh& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
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

		std::size_t size_nav_mesh([[maybe_unused]] const asset_data::world::nav_mesh& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.nav_resources)
			{
				size += plain::sized(std::max<std::size_t>(data.nav_resources->size(), 1) * sizeof(nav_resource_s));
				for (const auto& item : *data.nav_resources)
				{
					size += size_nav_resource_s(item);
				}
			}
			if (data.obstacle_bounds)
			{
				size += plain::sized(std::max<std::size_t>(data.obstacle_bounds->size(), 1) * sizeof(nav_obstacle_bounds_s));
				for (const auto& item : *data.obstacle_bounds)
				{
					size += size_nav_obstacle_bounds_s(item);
				}
			}
			if (data.glass_bounds)
			{
				size += plain::sized(std::max<std::size_t>(data.glass_bounds->size(), 1) * sizeof(nav_glass_bounds_s));
				for (const auto& item : *data.glass_bounds)
				{
					size += size_nav_glass_bounds_s(item);
				}
			}
			if (data.modifiers)
			{
				size += plain::sized(std::max<std::size_t>(data.modifiers->size(), 1) * sizeof(nav_modifier_s));
				for (const auto& item : *data.modifiers)
				{
					size += size_nav_modifier_s(item);
				}
			}
			if (data.link_creation_data)
			{
				size += plain::sized(std::max<std::size_t>(data.link_creation_data->size(), 1) * sizeof(nav_link_creation_data_s));
				for (const auto& item : *data.link_creation_data)
				{
					size += size_nav_link_creation_data_s(item);
				}
			}
			if (data.volume_seeds)
			{
				size += plain::sized(std::max<std::size_t>(data.volume_seeds->size(), 1) * sizeof(vec3_t));
			}
			if (data.raw_volumes)
			{
				size += plain::sized(std::max<std::size_t>(data.raw_volumes->size(), 1) * sizeof(nav_raw_volume_s));
				for (const auto& item : *data.raw_volumes)
				{
					size += size_nav_raw_volume_s(item);
				}
			}
			if (data.raw_custom_volumes)
			{
				size += plain::sized(std::max<std::size_t>(data.raw_custom_volumes->size(), 1) * sizeof(nav_raw_custom_volume_s));
				for (const auto& item : *data.raw_custom_volumes)
				{
					size += size_nav_raw_custom_volume_s(item);
				}
			}
			size += size_nav_misc_s(data.misc_data);
			return size;
		}

		asset_data::world::nav_resource_s read_nav_resource_s(const nav_resource_s& native)
		{
			plain::trace("read_nav_resource_s");
			asset_data::world::nav_resource_s data;
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

		void write_nav_resource_s(nav_resource_s& native, const asset_data::world::nav_resource_s& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
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

		std::size_t size_nav_resource_s([[maybe_unused]] const asset_data::world::nav_resource_s& data)
		{
			std::size_t size = 0;
			if (data.p_graph_buffer)
			{
				size += plain::sized(std::max<std::size_t>(data.p_graph_buffer->size(), 1) * sizeof(std::uint8_t));
			}
			if (data.p_working_graph)
			{
				size += plain::sized(std::max<std::size_t>(data.p_working_graph->size(), 1) * sizeof(std::uint8_t));
			}
			return size;
		}

		asset_data::world::nav_obstacle_bounds_s read_nav_obstacle_bounds_s(const nav_obstacle_bounds_s& native)
		{
			plain::trace("read_nav_obstacle_bounds_s");
			asset_data::world::nav_obstacle_bounds_s data;
			data.m_hulls = plain::read_array(native.m_Hulls, native.m_NumHulls, "nav_obstacle_bounds_s.m_Hulls", [&](const auto& item) { return read_nav_obstacle_hull_s(item); });
			return data;
		}

		void write_nav_obstacle_bounds_s(nav_obstacle_bounds_s& native, const asset_data::world::nav_obstacle_bounds_s& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.m_Hulls = plain::write_array<nav_obstacle_hull_s>(data.m_hulls, memory, [&](auto& out, const auto& in) { write_nav_obstacle_hull_s(out, in, resolve, memory); });
			native.m_NumHulls = plain::count_of<std::int32_t>(data.m_hulls);
		}

		std::size_t size_nav_obstacle_bounds_s([[maybe_unused]] const asset_data::world::nav_obstacle_bounds_s& data)
		{
			std::size_t size = 0;
			if (data.m_hulls)
			{
				size += plain::sized(std::max<std::size_t>(data.m_hulls->size(), 1) * sizeof(nav_obstacle_hull_s));
				for (const auto& item : *data.m_hulls)
				{
					size += size_nav_obstacle_hull_s(item);
				}
			}
			return size;
		}

		asset_data::world::nav_obstacle_hull_s read_nav_obstacle_hull_s(const nav_obstacle_hull_s& native)
		{
			plain::trace("read_nav_obstacle_hull_s");
			asset_data::world::nav_obstacle_hull_s data;
			data.m_boundaries = plain::read_array(native.m_Boundaries, native.m_NumBoundaries, "nav_obstacle_hull_s.m_Boundaries", [&](const auto& item) { return read_nav_boundary_plane_s(item); });
			data.m_bounds_x = native.m_BoundsX;
			data.m_bounds_y = native.m_BoundsY;
			data.m_bounds_z = native.m_BoundsZ;
			data.m_offset = plain::read_scalars<float, 3>(&native.m_Offset);
			data.m_b_use_bounds = native.m_bUseBounds;
			return data;
		}

		void write_nav_obstacle_hull_s(nav_obstacle_hull_s& native, const asset_data::world::nav_obstacle_hull_s& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.m_Boundaries = plain::write_array<nav_boundary_plane_s>(data.m_boundaries, memory, [&](auto& out, const auto& in) { write_nav_boundary_plane_s(out, in, resolve, memory); });
			native.m_BoundsX = data.m_bounds_x;
			native.m_BoundsY = data.m_bounds_y;
			native.m_BoundsZ = data.m_bounds_z;
			plain::write_scalars(&native.m_Offset, data.m_offset);
			native.m_bUseBounds = data.m_b_use_bounds;
			native.m_NumBoundaries = plain::count_of<std::int32_t>(data.m_boundaries);
		}

		std::size_t size_nav_obstacle_hull_s([[maybe_unused]] const asset_data::world::nav_obstacle_hull_s& data)
		{
			std::size_t size = 0;
			if (data.m_boundaries)
			{
				size += plain::sized(std::max<std::size_t>(data.m_boundaries->size(), 1) * sizeof(nav_boundary_plane_s));
				for (const auto& item : *data.m_boundaries)
				{
					size += size_nav_boundary_plane_s(item);
				}
			}
			return size;
		}

		asset_data::world::nav_boundary_plane_s read_nav_boundary_plane_s(const nav_boundary_plane_s& native)
		{
			plain::trace("read_nav_boundary_plane_s");
			asset_data::world::nav_boundary_plane_s data;
			data.m_normal = plain::read_scalars<float, 3>(&native.m_Normal);
			data.m_dist_from_center = native.m_DistFromCenter;
			return data;
		}

		void write_nav_boundary_plane_s(nav_boundary_plane_s& native, const asset_data::world::nav_boundary_plane_s& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.m_Normal, data.m_normal);
			native.m_DistFromCenter = data.m_dist_from_center;
		}

		std::size_t size_nav_boundary_plane_s([[maybe_unused]] const asset_data::world::nav_boundary_plane_s& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::world::nav_glass_bounds_s read_nav_glass_bounds_s(const nav_glass_bounds_s& native)
		{
			plain::trace("read_nav_glass_bounds_s");
			asset_data::world::nav_glass_bounds_s data;
			data.m_boundaries = plain::read_array(native.m_Boundaries, native.m_NumBoundaries, "nav_glass_bounds_s.m_Boundaries", [&](const auto& item) { return read_nav_boundary_plane_s(item); });
			data.m_pos = plain::read_scalars<float, 3>(&native.m_Pos);
			data.m_glass_piece_index = native.m_GlassPieceIndex;
			return data;
		}

		void write_nav_glass_bounds_s(nav_glass_bounds_s& native, const asset_data::world::nav_glass_bounds_s& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.m_Boundaries = plain::write_array<nav_boundary_plane_s>(data.m_boundaries, memory, [&](auto& out, const auto& in) { write_nav_boundary_plane_s(out, in, resolve, memory); });
			plain::write_scalars(&native.m_Pos, data.m_pos);
			native.m_GlassPieceIndex = data.m_glass_piece_index;
			native.m_NumBoundaries = plain::count_of<std::int32_t>(data.m_boundaries);
		}

		std::size_t size_nav_glass_bounds_s([[maybe_unused]] const asset_data::world::nav_glass_bounds_s& data)
		{
			std::size_t size = 0;
			if (data.m_boundaries)
			{
				size += plain::sized(std::max<std::size_t>(data.m_boundaries->size(), 1) * sizeof(nav_boundary_plane_s));
				for (const auto& item : *data.m_boundaries)
				{
					size += size_nav_boundary_plane_s(item);
				}
			}
			return size;
		}

		asset_data::world::nav_modifier_s read_nav_modifier_s(const nav_modifier_s& native)
		{
			plain::trace("read_nav_modifier_s");
			asset_data::world::nav_modifier_s data;
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

		void write_nav_modifier_s(nav_modifier_s& native, const asset_data::world::nav_modifier_s& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
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

		std::size_t size_nav_modifier_s([[maybe_unused]] const asset_data::world::nav_modifier_s& data)
		{
			std::size_t size = 0;
			if (data.m_boundaries)
			{
				size += plain::sized(std::max<std::size_t>(data.m_boundaries->size(), 1) * sizeof(nav_boundary_plane_s));
				for (const auto& item : *data.m_boundaries)
				{
					size += size_nav_boundary_plane_s(item);
				}
			}
			return size;
		}

		asset_data::world::nav_link_point_s read_nav_link_point_s(const nav_link_point_s& native)
		{
			plain::trace("read_nav_link_point_s");
			asset_data::world::nav_link_point_s data;
			data.m_pt1 = plain::read_scalars<float, 3>(&native.m_Pt1);
			data.m_pt2 = plain::read_scalars<float, 3>(&native.m_Pt2);
			return data;
		}

		void write_nav_link_point_s(nav_link_point_s& native, const asset_data::world::nav_link_point_s& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.m_Pt1, data.m_pt1);
			plain::write_scalars(&native.m_Pt2, data.m_pt2);
		}

		std::size_t size_nav_link_point_s([[maybe_unused]] const asset_data::world::nav_link_point_s& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::world::nav_link_creation_data_s read_nav_link_creation_data_s(const nav_link_creation_data_s& native)
		{
			plain::trace("read_nav_link_creation_data_s");
			asset_data::world::nav_link_creation_data_s data;
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

		void write_nav_link_creation_data_s(nav_link_creation_data_s& native, const asset_data::world::nav_link_creation_data_s& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
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

		std::size_t size_nav_link_creation_data_s([[maybe_unused]] const asset_data::world::nav_link_creation_data_s& data)
		{
			std::size_t size = 0;
			size += size_nav_link_point_s(data.m_start);
			size += size_nav_link_point_s(data.m_end);
			return size;
		}

		asset_data::world::nav_raw_volume_s read_nav_raw_volume_s(const nav_raw_volume_s& native)
		{
			plain::trace("read_nav_raw_volume_s");
			asset_data::world::nav_raw_volume_s data;
			data.m_mid_point = plain::read_scalars<float, 3>(&native.m_MidPoint);
			data.m_half_size = plain::read_scalars<float, 3>(&native.m_HalfSize);
			data.m_min_voxel_size = native.m_MinVoxelSize;
			data.m_clearance_xy = native.m_ClearanceXY;
			data.m_clearance_z = native.m_ClearanceZ;
			return data;
		}

		void write_nav_raw_volume_s(nav_raw_volume_s& native, const asset_data::world::nav_raw_volume_s& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.m_MidPoint, data.m_mid_point);
			plain::write_scalars(&native.m_HalfSize, data.m_half_size);
			native.m_MinVoxelSize = data.m_min_voxel_size;
			native.m_ClearanceXY = data.m_clearance_xy;
			native.m_ClearanceZ = data.m_clearance_z;
		}

		std::size_t size_nav_raw_volume_s([[maybe_unused]] const asset_data::world::nav_raw_volume_s& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::world::nav_raw_custom_volume_s read_nav_raw_custom_volume_s(const nav_raw_custom_volume_s& native)
		{
			plain::trace("read_nav_raw_custom_volume_s");
			asset_data::world::nav_raw_custom_volume_s data;
			data.m_tris = plain::read_array(native.m_Tris, native.m_NumTris, "nav_raw_custom_volume_s.m_Tris", [&](const auto& item) { return read_nav_raw_custom_volume_tri_s(item); });
			return data;
		}

		void write_nav_raw_custom_volume_s(nav_raw_custom_volume_s& native, const asset_data::world::nav_raw_custom_volume_s& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.m_Tris = plain::write_array<nav_raw_custom_volume_tri_s>(data.m_tris, memory, [&](auto& out, const auto& in) { write_nav_raw_custom_volume_tri_s(out, in, resolve, memory); });
			native.m_NumTris = plain::count_of<std::int32_t>(data.m_tris);
		}

		std::size_t size_nav_raw_custom_volume_s([[maybe_unused]] const asset_data::world::nav_raw_custom_volume_s& data)
		{
			std::size_t size = 0;
			if (data.m_tris)
			{
				size += plain::sized(std::max<std::size_t>(data.m_tris->size(), 1) * sizeof(nav_raw_custom_volume_tri_s));
				for (const auto& item : *data.m_tris)
				{
					size += size_nav_raw_custom_volume_tri_s(item);
				}
			}
			return size;
		}

		asset_data::world::nav_raw_custom_volume_tri_s read_nav_raw_custom_volume_tri_s(const nav_raw_custom_volume_tri_s& native)
		{
			plain::trace("read_nav_raw_custom_volume_tri_s");
			asset_data::world::nav_raw_custom_volume_tri_s data;
			data.m_verts = [&] { std::array<std::array<float, 3>, 3> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = plain::read_scalars<float, 3>(&native.m_Verts[i]); } return out; }();
			return data;
		}

		void write_nav_raw_custom_volume_tri_s(nav_raw_custom_volume_tri_s& native, const asset_data::world::nav_raw_custom_volume_tri_s& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			for (std::size_t i = 0; i < data.m_verts.size(); ++i)
			{
				plain::write_scalars(&native.m_Verts[i], data.m_verts[i]);
			}
		}

		std::size_t size_nav_raw_custom_volume_tri_s([[maybe_unused]] const asset_data::world::nav_raw_custom_volume_tri_s& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::world::map_triggers read_map_triggers(const MapTriggers& native)
		{
			plain::trace("read_map_triggers");
			asset_data::world::map_triggers data;
			data.count = native.count;
			data.models = plain::read_array(native.models, native.count, "MapTriggers.models", [&](const auto& item) { return read_trigger_model(item); });
			data.hulls = plain::read_array(native.hulls, native.hullCount, "MapTriggers.hulls", [&](const auto& item) { return read_trigger_hull(item); });
			data.slabs = plain::read_array(native.slabs, native.slabCount, "MapTriggers.slabs", [&](const auto& item) { return read_trigger_slab(item); });
			data.windings = plain::read_array(native.windings, native.windingCount, "MapTriggers.windings", [&](const auto& item) { return read_trigger_winding(item); });
			data.winding_points = plain::read_array(native.windingPoints, native.windingPointCount, "MapTriggers.windingPoints", [&](const auto& item) { return read_trigger_winding_point(item); });
			return data;
		}

		void write_map_triggers(MapTriggers& native, const asset_data::world::map_triggers& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
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

		std::size_t size_map_triggers([[maybe_unused]] const asset_data::world::map_triggers& data)
		{
			std::size_t size = 0;
			if (data.models)
			{
				size += plain::sized(std::max<std::size_t>(data.models->size(), 1) * sizeof(TriggerModel));
				for (const auto& item : *data.models)
				{
					size += size_trigger_model(item);
				}
			}
			if (data.hulls)
			{
				size += plain::sized(std::max<std::size_t>(data.hulls->size(), 1) * sizeof(TriggerHull));
				for (const auto& item : *data.hulls)
				{
					size += size_trigger_hull(item);
				}
			}
			if (data.slabs)
			{
				size += plain::sized(std::max<std::size_t>(data.slabs->size(), 1) * sizeof(TriggerSlab));
				for (const auto& item : *data.slabs)
				{
					size += size_trigger_slab(item);
				}
			}
			if (data.windings)
			{
				size += plain::sized(std::max<std::size_t>(data.windings->size(), 1) * sizeof(TriggerWinding));
				for (const auto& item : *data.windings)
				{
					size += size_trigger_winding(item);
				}
			}
			if (data.winding_points)
			{
				size += plain::sized(std::max<std::size_t>(data.winding_points->size(), 1) * sizeof(TriggerWindingPoint));
				for (const auto& item : *data.winding_points)
				{
					size += size_trigger_winding_point(item);
				}
			}
			return size;
		}

		asset_data::world::physics_capacities read_physics_capacities(const PhysicsCapacities& native)
		{
			plain::trace("read_physics_capacities");
			asset_data::world::physics_capacities data;
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

		void write_physics_capacities(PhysicsCapacities& native, const asset_data::world::physics_capacities& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
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

		std::size_t size_physics_capacities([[maybe_unused]] const asset_data::world::physics_capacities& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::world::top_down_map_data_header read_top_down_map_data_header(const TopDownMapDataHeader& native)
		{
			plain::trace("read_top_down_map_data_header");
			asset_data::world::top_down_map_data_header data;
			data.version = native.version;
			data.samples_per_axis = native.samplesPerAxis;
			data.bottom_left = plain::read_scalars<float, 2>(&native.bottomLeft);
			data.top_right = plain::read_scalars<float, 2>(&native.topRight);
			return data;
		}

		void write_top_down_map_data_header(TopDownMapDataHeader& native, const asset_data::world::top_down_map_data_header& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.version = data.version;
			native.samplesPerAxis = data.samples_per_axis;
			plain::write_scalars(&native.bottomLeft, data.bottom_left);
			plain::write_scalars(&native.topRight, data.top_right);
		}

		std::size_t size_top_down_map_data_header([[maybe_unused]] const asset_data::world::top_down_map_data_header& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::world::top_down_map_data_content read_top_down_map_data_content(const TopDownMapDataContent& native)
		{
			plain::trace("read_top_down_map_data_content");
			asset_data::world::top_down_map_data_content data;
			data.nav_mesh_visible = std::to_array(native.navMeshVisible);
			return data;
		}

		void write_top_down_map_data_content(TopDownMapDataContent& native, const asset_data::world::top_down_map_data_content& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			std::ranges::copy(data.nav_mesh_visible, native.navMeshVisible);
		}

		std::size_t size_top_down_map_data_content([[maybe_unused]] const asset_data::world::top_down_map_data_content& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::world::top_down_map_data read_top_down_map_data(const TopDownMapData& native)
		{
			plain::trace("read_top_down_map_data");
			asset_data::world::top_down_map_data data;
			data.header = read_top_down_map_data_header(native.header);
			data.content = read_top_down_map_data_content(native.content);
			return data;
		}

		void write_top_down_map_data(TopDownMapData& native, const asset_data::world::top_down_map_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_top_down_map_data_header(native.header, data.header, resolve, memory);
			write_top_down_map_data_content(native.content, data.content, resolve, memory);
		}

		std::size_t size_top_down_map_data([[maybe_unused]] const asset_data::world::top_down_map_data& data)
		{
			std::size_t size = 0;
			size += size_top_down_map_data_header(data.header);
			size += size_top_down_map_data_content(data.content);
			return size;
		}

		asset_data::world::clip_map read_clip_map(const clipMap_t& native)
		{
			plain::trace("read_clip_map");
			asset_data::world::clip_map data;
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

		void write_clip_map(clipMap_t& native, const asset_data::world::clip_map& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
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

		std::size_t size_clip_map([[maybe_unused]] const asset_data::world::clip_map& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.static_model_collision_model_lists)
			{
				size += plain::sized(std::max<std::size_t>(data.static_model_collision_model_lists->size(), 1) * sizeof(StaticModelCollisionModelList));
				for (const auto& item : *data.static_model_collision_model_lists)
				{
					size += size_static_model_collision_model_list(item);
				}
			}
			if (data.stages)
			{
				size += plain::sized(std::max<std::size_t>(data.stages->size(), 1) * sizeof(Stage));
				for (const auto& item : *data.stages)
				{
					size += size_stage(item);
				}
			}
			size += size_map_triggers(data.stage_trigger);
			size += size_physics_capacities(data.physics_capacities);
			if (data.havok_world_shape_data)
			{
				size += plain::sized(std::max<std::size_t>(data.havok_world_shape_data->size(), 1) * sizeof(char));
			}
			if (data.collision_heatmap)
			{
				size += plain::sized(std::max<std::size_t>(data.collision_heatmap->size(), 1) * sizeof(CollisionHeatmapEntry));
				for (const auto& item : *data.collision_heatmap)
				{
					size += size_collision_heatmap_entry(item);
				}
			}
			if (data.top_down_map_data)
			{
				size += plain::sized(sizeof(TopDownMapData));
				size += size_top_down_map_data(*data.top_down_map_data);
			}
			size += plain::string_size(data.vision_set_name);
			return size;
		}

		asset_data::world::static_model_collision_model_list read_static_model_collision_model_list(const StaticModelCollisionModelList& native)
		{
			plain::trace("read_static_model_collision_model_list");
			asset_data::world::static_model_collision_model_list data;
			data.num_models = native.numModels;
			data.models = plain::read_array(native.models, native.numModels, "StaticModelCollisionModelList.models", [&](const auto& item) { return read_static_model_collision_model(item); });
			data.structure_memory_cost = native.structureMemoryCost;
			return data;
		}

		void write_static_model_collision_model_list(StaticModelCollisionModelList& native, const asset_data::world::static_model_collision_model_list& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.numModels = data.num_models;
			native.models = plain::write_array<StaticModelCollisionModel>(data.models, memory, [&](auto& out, const auto& in) { write_static_model_collision_model(out, in, resolve, memory); });
			native.structureMemoryCost = data.structure_memory_cost;
		}

		std::size_t size_static_model_collision_model_list([[maybe_unused]] const asset_data::world::static_model_collision_model_list& data)
		{
			std::size_t size = 0;
			if (data.models)
			{
				size += plain::sized(std::max<std::size_t>(data.models->size(), 1) * sizeof(StaticModelCollisionModel));
				for (const auto& item : *data.models)
				{
					size += size_static_model_collision_model(item);
				}
			}
			return size;
		}

		asset_data::world::static_model_collision_model read_static_model_collision_model(const StaticModelCollisionModel& native)
		{
			plain::trace("read_static_model_collision_model");
			asset_data::world::static_model_collision_model data;
			data.name = plain::read_script_string(native.name);
			data.physics_asset = plain::read_asset(ASSET_TYPE_PHYSICSASSET, native.physicsAsset);
			data.detail_collision = plain::read_asset(ASSET_TYPE_XMODEL_DETAIL_COLLISION, native.detailCollision);
			data.instances = plain::read_array(native.instances, native.numInstances, "StaticModelCollisionModel.instances", [&](const auto& item) { return read_static_model_collision_instance(item); });
			return data;
		}

		void write_static_model_collision_model(StaticModelCollisionModel& native, const asset_data::world::static_model_collision_model& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = static_cast<scr_string_t>(plain::write_script_string(data.name));
			native.physicsAsset = plain::resolve_asset<PhysicsAsset>(resolve, ASSET_TYPE_PHYSICSASSET, data.physics_asset);
			native.detailCollision = plain::resolve_asset<XModelDetailCollision>(resolve, ASSET_TYPE_XMODEL_DETAIL_COLLISION, data.detail_collision);
			native.instances = plain::write_array<StaticModelCollisionInstance>(data.instances, memory, [&](auto& out, const auto& in) { write_static_model_collision_instance(out, in, resolve, memory); });
			native.numInstances = plain::count_of<std::uint32_t>(data.instances);
		}

		std::size_t size_static_model_collision_model([[maybe_unused]] const asset_data::world::static_model_collision_model& data)
		{
			std::size_t size = 0;
			if (data.instances)
			{
				size += plain::sized(std::max<std::size_t>(data.instances->size(), 1) * sizeof(StaticModelCollisionInstance));
				for (const auto& item : *data.instances)
				{
					size += size_static_model_collision_instance(item);
				}
			}
			return size;
		}

		asset_data::world::static_model_collision_instance read_static_model_collision_instance(const StaticModelCollisionInstance& native)
		{
			plain::trace("read_static_model_collision_instance");
			asset_data::world::static_model_collision_instance data;
			data.origin = plain::read_scalars<float, 3>(&native.origin);
			data.angles = plain::read_scalars<float, 3>(&native.angles);
			data.scale = native.scale;
			return data;
		}

		void write_static_model_collision_instance(StaticModelCollisionInstance& native, const asset_data::world::static_model_collision_instance& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.origin, data.origin);
			plain::write_scalars(&native.angles, data.angles);
			native.scale = data.scale;
		}

		std::size_t size_static_model_collision_instance([[maybe_unused]] const asset_data::world::static_model_collision_instance& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::world::stage read_stage(const Stage& native)
		{
			plain::trace("read_stage");
			asset_data::world::stage data;
			data.name = plain::read_string(native.name);
			data.origin = plain::read_scalars<float, 3>(&native.origin);
			data.trigger_index = native.triggerIndex;
			data.sun_primary_light_index = native.sunPrimaryLightIndex;
			data.entity_uid = native.entityUID;
			data.sky_rotation_angles = plain::read_scalars<float, 3>(&native.skyRotationAngles);
			return data;
		}

		void write_stage(Stage& native, const asset_data::world::stage& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			plain::write_scalars(&native.origin, data.origin);
			native.triggerIndex = data.trigger_index;
			native.sunPrimaryLightIndex = data.sun_primary_light_index;
			native.entityUID = data.entity_uid;
			plain::write_scalars(&native.skyRotationAngles, data.sky_rotation_angles);
		}

		std::size_t size_stage([[maybe_unused]] const asset_data::world::stage& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			return size;
		}

		asset_data::world::trigger_model read_trigger_model(const TriggerModel& native)
		{
			plain::trace("read_trigger_model");
			asset_data::world::trigger_model data;
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

		void write_trigger_model(TriggerModel& native, const asset_data::world::trigger_model& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
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

		std::size_t size_trigger_model([[maybe_unused]] const asset_data::world::trigger_model& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::world::bounds read_bounds(const Bounds& native)
		{
			plain::trace("read_bounds");
			asset_data::world::bounds data;
			data.mid_point = plain::read_scalars<float, 3>(&native.midPoint);
			data.half_size = plain::read_scalars<float, 3>(&native.halfSize);
			return data;
		}

		void write_bounds(Bounds& native, const asset_data::world::bounds& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.midPoint, data.mid_point);
			plain::write_scalars(&native.halfSize, data.half_size);
		}

		std::size_t size_bounds([[maybe_unused]] const asset_data::world::bounds& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::world::trigger_hull read_trigger_hull(const TriggerHull& native)
		{
			plain::trace("read_trigger_hull");
			asset_data::world::trigger_hull data;
			data.bounds = read_bounds(native.bounds);
			data.contents = native.contents;
			data.slab_count = native.slabCount;
			data.first_slab = native.firstSlab;
			return data;
		}

		void write_trigger_hull(TriggerHull& native, const asset_data::world::trigger_hull& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_bounds(native.bounds, data.bounds, resolve, memory);
			native.contents = data.contents;
			native.slabCount = data.slab_count;
			native.firstSlab = data.first_slab;
		}

		std::size_t size_trigger_hull([[maybe_unused]] const asset_data::world::trigger_hull& data)
		{
			std::size_t size = 0;
			size += size_bounds(data.bounds);
			return size;
		}

		asset_data::world::trigger_slab read_trigger_slab(const TriggerSlab& native)
		{
			plain::trace("read_trigger_slab");
			asset_data::world::trigger_slab data;
			data.dir = plain::read_scalars<float, 3>(&native.dir);
			data.mid_point = native.midPoint;
			data.half_size = native.halfSize;
			return data;
		}

		void write_trigger_slab(TriggerSlab& native, const asset_data::world::trigger_slab& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.dir, data.dir);
			native.midPoint = data.mid_point;
			native.halfSize = data.half_size;
		}

		std::size_t size_trigger_slab([[maybe_unused]] const asset_data::world::trigger_slab& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::world::trigger_winding read_trigger_winding(const TriggerWinding& native)
		{
			plain::trace("read_trigger_winding");
			asset_data::world::trigger_winding data;
			data.dir = plain::read_scalars<float, 3>(&native.dir);
			data.point_count = native.pointCount;
			data.first_point = native.firstPoint;
			data.flags = native.flags;
			return data;
		}

		void write_trigger_winding(TriggerWinding& native, const asset_data::world::trigger_winding& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.dir, data.dir);
			native.pointCount = data.point_count;
			native.firstPoint = data.first_point;
			native.flags = data.flags;
		}

		std::size_t size_trigger_winding([[maybe_unused]] const asset_data::world::trigger_winding& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::world::trigger_winding_point read_trigger_winding_point(const TriggerWindingPoint& native)
		{
			plain::trace("read_trigger_winding_point");
			asset_data::world::trigger_winding_point data;
			data.loc = plain::read_scalars<float, 3>(&native.loc);
			return data;
		}

		void write_trigger_winding_point(TriggerWindingPoint& native, const asset_data::world::trigger_winding_point& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.loc, data.loc);
		}

		std::size_t size_trigger_winding_point([[maybe_unused]] const asset_data::world::trigger_winding_point& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::world::collision_heatmap_entry read_collision_heatmap_entry(const CollisionHeatmapEntry& native)
		{
			plain::trace("read_collision_heatmap_entry");
			asset_data::world::collision_heatmap_entry data;
			data.vertex_count = native.vertexCount;
			data.vertex_density = native.vertexDensity;
			data.min_extent = plain::read_scalars<float, 3>(&native.minExtent);
			data.max_extent = plain::read_scalars<float, 3>(&native.maxExtent);
			return data;
		}

		void write_collision_heatmap_entry(CollisionHeatmapEntry& native, const asset_data::world::collision_heatmap_entry& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.vertexCount = data.vertex_count;
			native.vertexDensity = data.vertex_density;
			plain::write_scalars(&native.minExtent, data.min_extent);
			plain::write_scalars(&native.maxExtent, data.max_extent);
		}

		std::size_t size_collision_heatmap_entry([[maybe_unused]] const asset_data::world::collision_heatmap_entry& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::world::gfx_fog_spline_data read_gfx_fog_spline_data(const GfxFogSplineData& native)
		{
			plain::trace("read_gfx_fog_spline_data");
			asset_data::world::gfx_fog_spline_data data;
			data.fog_start_dist = native.fogStartDist;
			data.fog_half_plane_distance = native.fogHalfPlaneDistance;
			data.fog_max_distance = native.fogMaxDistance;
			return data;
		}

		void write_gfx_fog_spline_data(GfxFogSplineData& native, const asset_data::world::gfx_fog_spline_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.fogStartDist = data.fog_start_dist;
			native.fogHalfPlaneDistance = data.fog_half_plane_distance;
			native.fogMaxDistance = data.fog_max_distance;
		}

		std::size_t size_gfx_fog_spline_data([[maybe_unused]] const asset_data::world::gfx_fog_spline_data& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::world::fog_spline read_fog_spline(const GfxFogSpline& native)
		{
			plain::trace("read_fog_spline");
			asset_data::world::fog_spline data;
			data.name = plain::read_string(native.name);
			data.image = plain::read_asset(ASSET_TYPE_IMAGE, native.image);
			data.data = read_gfx_fog_spline_data(native.data);
			return data;
		}

		void write_fog_spline(GfxFogSpline& native, const asset_data::world::fog_spline& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.image = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, data.image);
			write_gfx_fog_spline_data(native.data, data.data, resolve, memory);
		}

		std::size_t size_fog_spline([[maybe_unused]] const asset_data::world::fog_spline& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			size += size_gfx_fog_spline_data(data.data);
			return size;
		}

	}

	std::expected<asset_data::world::com_world, std::string> to_neutral(const ComWorld& native)
	{
		try
		{
			auto data = read_com_world(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	ComWorld* from_neutral(const asset_data::world::com_world& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<ComWorld>();
		write_com_world(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::world::com_world& data)
	{
		return plain::rounded(sizeof(ComWorld)) + size_com_world(data) + 64;
	}

	std::expected<asset_data::world::glass_world, std::string> to_neutral(const GlassWorld& native)
	{
		try
		{
			auto data = read_glass_world(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	GlassWorld* from_neutral(const asset_data::world::glass_world& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<GlassWorld>();
		write_glass_world(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::world::glass_world& data)
	{
		return plain::rounded(sizeof(GlassWorld)) + size_glass_world(data) + 64;
	}

	std::expected<asset_data::world::map_edge_list, std::string> to_neutral(const MapEdgeList& native)
	{
		try
		{
			auto data = read_map_edge_list(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	MapEdgeList* from_neutral(const asset_data::world::map_edge_list& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<MapEdgeList>();
		write_map_edge_list(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::world::map_edge_list& data)
	{
		return plain::rounded(sizeof(MapEdgeList)) + size_map_edge_list(data) + 64;
	}

	std::expected<asset_data::world::nav_mesh, std::string> to_neutral(const NavMeshData& native)
	{
		try
		{
			auto data = read_nav_mesh(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	NavMeshData* from_neutral(const asset_data::world::nav_mesh& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<NavMeshData>();
		write_nav_mesh(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::world::nav_mesh& data)
	{
		return plain::rounded(sizeof(NavMeshData)) + size_nav_mesh(data) + 64;
	}

	std::expected<asset_data::world::clip_map, std::string> to_neutral(const clipMap_t& native)
	{
		try
		{
			auto data = read_clip_map(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	clipMap_t* from_neutral(const asset_data::world::clip_map& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<clipMap_t>();
		write_clip_map(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::world::clip_map& data)
	{
		return plain::rounded(sizeof(clipMap_t)) + size_clip_map(data) + 64;
	}

	std::expected<asset_data::world::fog_spline, std::string> to_neutral(const GfxFogSpline& native)
	{
		try
		{
			auto data = read_fog_spline(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	GfxFogSpline* from_neutral(const asset_data::world::fog_spline& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<GfxFogSpline>();
		write_fog_spline(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::world::fog_spline& data)
	{
		return plain::rounded(sizeof(GfxFogSpline)) + size_fog_spline(data) + 64;
	}

}
