// Emitted by tools/gen_iw8_neutral.py from tools/neutral_families/mapents.json: do not edit.
#include <std_include.hpp>
#include "mapents.hpp"
#include "plain.hpp"

namespace zonetool::iw8::neutral
{
	namespace
	{
		asset_data::mapents::map_ents read_map_ents(const MapEnts& native);
		void write_map_ents(MapEnts& native, const asset_data::mapents::map_ents& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_map_ents(const asset_data::mapents::map_ents& data);
		asset_data::mapents::map_triggers read_map_triggers(const MapTriggers& native);
		void write_map_triggers(MapTriggers& native, const asset_data::mapents::map_triggers& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_map_triggers(const asset_data::mapents::map_triggers& data);
		asset_data::mapents::trigger_model read_trigger_model(const TriggerModel& native);
		void write_trigger_model(TriggerModel& native, const asset_data::mapents::trigger_model& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_trigger_model(const asset_data::mapents::trigger_model& data);
		asset_data::mapents::trigger_hull read_trigger_hull(const TriggerHull& native);
		void write_trigger_hull(TriggerHull& native, const asset_data::mapents::trigger_hull& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_trigger_hull(const asset_data::mapents::trigger_hull& data);
		asset_data::mapents::bounds read_bounds(const Bounds& native);
		void write_bounds(Bounds& native, const asset_data::mapents::bounds& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_bounds(const asset_data::mapents::bounds& data);
		asset_data::mapents::trigger_slab read_trigger_slab(const TriggerSlab& native);
		void write_trigger_slab(TriggerSlab& native, const asset_data::mapents::trigger_slab& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_trigger_slab(const asset_data::mapents::trigger_slab& data);
		asset_data::mapents::trigger_winding read_trigger_winding(const TriggerWinding& native);
		void write_trigger_winding(TriggerWinding& native, const asset_data::mapents::trigger_winding& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_trigger_winding(const asset_data::mapents::trigger_winding& data);
		asset_data::mapents::trigger_winding_point read_trigger_winding_point(const TriggerWindingPoint& native);
		void write_trigger_winding_point(TriggerWindingPoint& native, const asset_data::mapents::trigger_winding_point& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_trigger_winding_point(const asset_data::mapents::trigger_winding_point& data);
		asset_data::mapents::client_triggers read_client_triggers(const ClientTriggers& native);
		void write_client_triggers(ClientTriggers& native, const asset_data::mapents::client_triggers& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_client_triggers(const asset_data::mapents::client_triggers& data);
		asset_data::mapents::client_map_triggers read_client_map_triggers(const ClientMapTriggers& native);
		void write_client_map_triggers(ClientMapTriggers& native, const asset_data::mapents::client_map_triggers& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_client_map_triggers(const asset_data::mapents::client_map_triggers& data);
		asset_data::mapents::client_trigger_model read_client_trigger_model(const ClientTriggerModel& native);
		void write_client_trigger_model(ClientTriggerModel& native, const asset_data::mapents::client_trigger_model& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_client_trigger_model(const asset_data::mapents::client_trigger_model& data);
		asset_data::mapents::client_trigger_hull read_client_trigger_hull(const ClientTriggerHull& native);
		void write_client_trigger_hull(ClientTriggerHull& native, const asset_data::mapents::client_trigger_hull& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_client_trigger_hull(const asset_data::mapents::client_trigger_hull& data);
		asset_data::mapents::ct_aud_rvb_pan_info read_ct_aud_rvb_pan_info(const CTAudRvbPanInfo& native);
		void write_ct_aud_rvb_pan_info(CTAudRvbPanInfo& native, const asset_data::mapents::ct_aud_rvb_pan_info& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_ct_aud_rvb_pan_info(const asset_data::mapents::ct_aud_rvb_pan_info& data);
		asset_data::mapents::client_entity_link_to_def read_client_entity_link_to_def(const ClientEntityLinkToDef& native);
		void write_client_entity_link_to_def(ClientEntityLinkToDef& native, const asset_data::mapents::client_entity_link_to_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_client_entity_link_to_def(const asset_data::mapents::client_entity_link_to_def& data);
		asset_data::mapents::spatial_partition_tree read_spatial_partition_tree(const SpatialPartition_Tree& native);
		void write_spatial_partition_tree(SpatialPartition_Tree& native, const asset_data::mapents::spatial_partition_tree& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_spatial_partition_tree(const asset_data::mapents::spatial_partition_tree& data);
		asset_data::mapents::client_trigger_blend read_client_trigger_blend(const ClientTriggerBlend& native);
		void write_client_trigger_blend(ClientTriggerBlend& native, const asset_data::mapents::client_trigger_blend& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_client_trigger_blend(const asset_data::mapents::client_trigger_blend& data);
		asset_data::mapents::client_trigger_blend_node read_client_trigger_blend_node(const ClientTriggerBlendNode& native);
		void write_client_trigger_blend_node(ClientTriggerBlendNode& native, const asset_data::mapents::client_trigger_blend_node& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_client_trigger_blend_node(const asset_data::mapents::client_trigger_blend_node& data);
		asset_data::mapents::spawn_point_record_list read_spawn_point_record_list(const SpawnPointRecordList& native);
		void write_spawn_point_record_list(SpawnPointRecordList& native, const asset_data::mapents::spawn_point_record_list& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_spawn_point_record_list(const asset_data::mapents::spawn_point_record_list& data);
		asset_data::mapents::spawn_point_entity_record read_spawn_point_entity_record(const SpawnPointEntityRecord& native);
		void write_spawn_point_entity_record(SpawnPointEntityRecord& native, const asset_data::mapents::spawn_point_entity_record& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_spawn_point_entity_record(const asset_data::mapents::spawn_point_entity_record& data);
		asset_data::mapents::spline_record_list read_spline_record_list(const SplineRecordList& native);
		void write_spline_record_list(SplineRecordList& native, const asset_data::mapents::spline_record_list& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_spline_record_list(const asset_data::mapents::spline_record_list& data);
		asset_data::mapents::spline_point_record_list read_spline_point_record_list(const SplinePointRecordList& native);
		void write_spline_point_record_list(SplinePointRecordList& native, const asset_data::mapents::spline_point_record_list& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_spline_point_record_list(const asset_data::mapents::spline_point_record_list& data);
		asset_data::mapents::spline_point_entity_record read_spline_point_entity_record(const SplinePointEntityRecord& native);
		void write_spline_point_entity_record(SplinePointEntityRecord& native, const asset_data::mapents::spline_point_entity_record& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_spline_point_entity_record(const asset_data::mapents::spline_point_entity_record& data);
		asset_data::mapents::cmodel_t read_cmodel_t(const cmodel_t& native);
		void write_cmodel_t(cmodel_t& native, const asset_data::mapents::cmodel_t& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_cmodel_t(const asset_data::mapents::cmodel_t& data);
		asset_data::mapents::cm_client_model read_cm_client_model(const CM_ClientModel& native);
		void write_cm_client_model(CM_ClientModel& native, const asset_data::mapents::cm_client_model& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_cm_client_model(const asset_data::mapents::cm_client_model& data);
		asset_data::mapents::spatial_partition_population read_spatial_partition_population(const SpatialPartition_Population& native);
		void write_spatial_partition_population(SpatialPartition_Population& native, const asset_data::mapents::spatial_partition_population& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_spatial_partition_population(const asset_data::mapents::spatial_partition_population& data);
		asset_data::mapents::spatial_partition_population_tree read_spatial_partition_population_tree(const SpatialPartition_Population_Tree& native);
		void write_spatial_partition_population_tree(SpatialPartition_Population_Tree& native, const asset_data::mapents::spatial_partition_population_tree& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_spatial_partition_population_tree(const asset_data::mapents::spatial_partition_population_tree& data);
		asset_data::mapents::spatial_partition_population_partition read_spatial_partition_population_partition(const SpatialPartition_Population_Partition& native);
		void write_spatial_partition_population_partition(SpatialPartition_Population_Partition& native, const asset_data::mapents::spatial_partition_population_partition& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_spatial_partition_population_partition(const asset_data::mapents::spatial_partition_population_partition& data);
		asset_data::mapents::extent_bounds read_extent_bounds(const ExtentBounds& native);
		void write_extent_bounds(ExtentBounds& native, const asset_data::mapents::extent_bounds& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_extent_bounds(const asset_data::mapents::extent_bounds& data);
		asset_data::mapents::spatial_partition_population_node read_spatial_partition_population_node(const SpatialPartition_Population_Node& native);
		void write_spatial_partition_population_node(SpatialPartition_Population_Node& native, const asset_data::mapents::spatial_partition_population_node& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_spatial_partition_population_node(const asset_data::mapents::spatial_partition_population_node& data);
		asset_data::mapents::spatial_partition_population_transient_map read_spatial_partition_population_transient_map(const SpatialPartition_Population_TransientMap& native);
		void write_spatial_partition_population_transient_map(SpatialPartition_Population_TransientMap& native, const asset_data::mapents::spatial_partition_population_transient_map& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_spatial_partition_population_transient_map(const asset_data::mapents::spatial_partition_population_transient_map& data);
		asset_data::mapents::bitarray_dynamic read_bitarray_dynamic(const bitarray_dynamic& native);
		void write_bitarray_dynamic(bitarray_dynamic& native, const asset_data::mapents::bitarray_dynamic& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_bitarray_dynamic(const asset_data::mapents::bitarray_dynamic& data);
		asset_data::mapents::client_ent_anchor read_client_ent_anchor(const ClientEntAnchor& native);
		void write_client_ent_anchor(ClientEntAnchor& native, const asset_data::mapents::client_ent_anchor& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_client_ent_anchor(const asset_data::mapents::client_ent_anchor& data);
		asset_data::mapents::scriptable_map_ents read_scriptable_map_ents(const ScriptableMapEnts& native);
		void write_scriptable_map_ents(ScriptableMapEnts& native, const asset_data::mapents::scriptable_map_ents& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_map_ents(const asset_data::mapents::scriptable_map_ents& data);
		asset_data::mapents::scriptable_physics_limits read_scriptable_physics_limits(const ScriptablePhysicsLimits& native);
		void write_scriptable_physics_limits(ScriptablePhysicsLimits& native, const asset_data::mapents::scriptable_physics_limits& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_physics_limits(const asset_data::mapents::scriptable_physics_limits& data);
		asset_data::mapents::scriptable_instance read_scriptable_instance(const ScriptableInstance& native);
		void write_scriptable_instance(ScriptableInstance& native, const asset_data::mapents::scriptable_instance& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_instance(const asset_data::mapents::scriptable_instance& data);
		asset_data::mapents::scriptable_event_sun_data read_scriptable_event_sun_data(const Scriptable_EventSun_Data& native);
		void write_scriptable_event_sun_data(Scriptable_EventSun_Data& native, const asset_data::mapents::scriptable_event_sun_data& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_sun_data(const asset_data::mapents::scriptable_event_sun_data& data);
		asset_data::mapents::scriptable_reserved_dynents read_scriptable_reserved_dynents(const ScriptableReservedDynents& native);
		void write_scriptable_reserved_dynents(ScriptableReservedDynents& native, const asset_data::mapents::scriptable_reserved_dynents& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_reserved_dynents(const asset_data::mapents::scriptable_reserved_dynents& data);
		asset_data::mapents::scriptable_reserved_dynent read_scriptable_reserved_dynent(const ScriptableReservedDynent& native);
		void write_scriptable_reserved_dynent(ScriptableReservedDynent& native, const asset_data::mapents::scriptable_reserved_dynent& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_reserved_dynent(const asset_data::mapents::scriptable_reserved_dynent& data);
		asset_data::mapents::spawn_group read_spawn_group(const SpawnGroup& native);
		void write_spawn_group(SpawnGroup& native, const asset_data::mapents::spawn_group& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_spawn_group(const asset_data::mapents::spawn_group& data);
		asset_data::mapents::spawn_group_point read_spawn_group_point(const SpawnGroupPoint& native);
		void write_spawn_group_point(SpawnGroupPoint& native, const asset_data::mapents::spawn_group_point& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_spawn_group_point(const asset_data::mapents::spawn_group_point& data);
		asset_data::mapents::spawn_group_cluster read_spawn_group_cluster(const SpawnGroupCluster& native);
		void write_spawn_group_cluster(SpawnGroupCluster& native, const asset_data::mapents::spawn_group_cluster& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_spawn_group_cluster(const asset_data::mapents::spawn_group_cluster& data);
		asset_data::mapents::spawn_group_zone read_spawn_group_zone(const SpawnGroupZone& native);
		void write_spawn_group_zone(SpawnGroupZone& native, const asset_data::mapents::spawn_group_zone& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_spawn_group_zone(const asset_data::mapents::spawn_group_zone& data);
		asset_data::mapents::client_side_effects read_client_side_effects(const ClientSideEffects& native);
		void write_client_side_effects(ClientSideEffects& native, const asset_data::mapents::client_side_effects& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_client_side_effects(const asset_data::mapents::client_side_effects& data);
		asset_data::mapents::client_oneshot_effect_def read_client_oneshot_effect_def(const ClientOneshotEffectDef& native);
		void write_client_oneshot_effect_def(ClientOneshotEffectDef& native, const asset_data::mapents::client_oneshot_effect_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_client_oneshot_effect_def(const asset_data::mapents::client_oneshot_effect_def& data);
		asset_data::mapents::fx_combined_def read_fx_combined_def(const FxCombinedDef& native);
		void write_fx_combined_def(FxCombinedDef& native, const asset_data::mapents::fx_combined_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_fx_combined_def(const asset_data::mapents::fx_combined_def& data);
		asset_data::mapents::snd_alias_lookup read_snd_alias_lookup(const SndAliasLookup& native);
		void write_snd_alias_lookup(SndAliasLookup& native, const asset_data::mapents::snd_alias_lookup& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_snd_alias_lookup(const asset_data::mapents::snd_alias_lookup& data);
		asset_data::mapents::snd_alias_list read_snd_alias_list(const SndAliasList& native);
		void write_snd_alias_list(SndAliasList& native, const asset_data::mapents::snd_alias_list& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_snd_alias_list(const asset_data::mapents::snd_alias_list& data);
		asset_data::mapents::snd_alias read_snd_alias(const SndAlias& native);
		void write_snd_alias(SndAlias& native, const asset_data::mapents::snd_alias& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_snd_alias(const asset_data::mapents::snd_alias& data);
		asset_data::mapents::client_exploder_def read_client_exploder_def(const ClientExploderDef& native);
		void write_client_exploder_def(ClientExploderDef& native, const asset_data::mapents::client_exploder_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_client_exploder_def(const asset_data::mapents::client_exploder_def& data);
		asset_data::mapents::client_interval_sound_def read_client_interval_sound_def(const ClientIntervalSoundDef& native);
		void write_client_interval_sound_def(ClientIntervalSoundDef& native, const asset_data::mapents::client_interval_sound_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_client_interval_sound_def(const asset_data::mapents::client_interval_sound_def& data);
		asset_data::mapents::client_loop_sound_def read_client_loop_sound_def(const ClientLoopSoundDef& native);
		void write_client_loop_sound_def(ClientLoopSoundDef& native, const asset_data::mapents::client_loop_sound_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_client_loop_sound_def(const asset_data::mapents::client_loop_sound_def& data);
		asset_data::mapents::client_reactive_ent_def read_client_reactive_ent_def(const ClientReactiveEntDef& native);
		void write_client_reactive_ent_def(ClientReactiveEntDef& native, const asset_data::mapents::client_reactive_ent_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_client_reactive_ent_def(const asset_data::mapents::client_reactive_ent_def& data);
		asset_data::mapents::create_fx_asset_data read_create_fx_asset_data(const CreateFxAssetData& native);
		void write_create_fx_asset_data(CreateFxAssetData& native, const asset_data::mapents::create_fx_asset_data& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_create_fx_asset_data(const asset_data::mapents::create_fx_asset_data& data);
		asset_data::mapents::create_fx_palette_def read_create_fx_palette_def(const CreateFxPaletteDef& native);
		void write_create_fx_palette_def(CreateFxPaletteDef& native, const asset_data::mapents::create_fx_palette_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_create_fx_palette_def(const asset_data::mapents::create_fx_palette_def& data);
		asset_data::mapents::server_side_effects read_server_side_effects(const ServerSideEffects& native);
		void write_server_side_effects(ServerSideEffects& native, const asset_data::mapents::server_side_effects& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_server_side_effects(const asset_data::mapents::server_side_effects& data);
		asset_data::mapents::server_exploder_def read_server_exploder_def(const ServerExploderDef& native);
		void write_server_exploder_def(ServerExploderDef& native, const asset_data::mapents::server_exploder_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_server_exploder_def(const asset_data::mapents::server_exploder_def& data);
		asset_data::mapents::mayhem_instance read_mayhem_instance(const MayhemInstance& native);
		void write_mayhem_instance(MayhemInstance& native, const asset_data::mapents::mayhem_instance& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_mayhem_instance(const asset_data::mapents::mayhem_instance& data);
		asset_data::mapents::gfx_placement read_gfx_placement(const GfxPlacement& native);
		void write_gfx_placement(GfxPlacement& native, const asset_data::mapents::gfx_placement& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_placement(const asset_data::mapents::gfx_placement& data);
		asset_data::mapents::spawner_list read_spawner_list(const SpawnerList& native);
		void write_spawner_list(SpawnerList& native, const asset_data::mapents::spawner_list& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_spawner_list(const asset_data::mapents::spawner_list& data);
		asset_data::mapents::spawner read_spawner(const Spawner& native);
		void write_spawner(Spawner& native, const asset_data::mapents::spawner& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_spawner(const asset_data::mapents::spawner& data);
		asset_data::mapents::spawner_field read_spawner_field(const SpawnerField& native);
		void write_spawner_field(SpawnerField& native, const asset_data::mapents::spawner_field& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_spawner_field(const asset_data::mapents::spawner_field& data);
		asset_data::mapents::audio_pa_speaker read_audio_pa_speaker(const AudioPASpeaker& native);
		void write_audio_pa_speaker(AudioPASpeaker& native, const asset_data::mapents::audio_pa_speaker& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_audio_pa_speaker(const asset_data::mapents::audio_pa_speaker& data);
		asset_data::mapents::audio_propagation_node read_audio_propagation_node(const AudioPropagationNode& native);
		void write_audio_propagation_node(AudioPropagationNode& native, const asset_data::mapents::audio_propagation_node& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_audio_propagation_node(const asset_data::mapents::audio_propagation_node& data);
		asset_data::mapents::audio_propagation_edge read_audio_propagation_edge(const AudioPropagationEdge& native);
		void write_audio_propagation_edge(AudioPropagationEdge& native, const asset_data::mapents::audio_propagation_edge& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_audio_propagation_edge(const asset_data::mapents::audio_propagation_edge& data);
		asset_data::mapents::coll_map_lookup_entry read_coll_map_lookup_entry(const CollMapLookupEntry& native);
		void write_coll_map_lookup_entry(CollMapLookupEntry& native, const asset_data::mapents::coll_map_lookup_entry& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_coll_map_lookup_entry(const asset_data::mapents::coll_map_lookup_entry& data);

		asset_data::mapents::map_triggers read_map_triggers(const MapTriggers& native)
		{
			plain::trace("read_map_triggers");
			asset_data::mapents::map_triggers data;
			data.count = native.count;
			data.models = plain::read_array(native.models, native.count, "MapTriggers.models", [&](const auto& item) { return read_trigger_model(item); });
			data.hulls = plain::read_array(native.hulls, native.hullCount, "MapTriggers.hulls", [&](const auto& item) { return read_trigger_hull(item); });
			data.slabs = plain::read_array(native.slabs, native.slabCount, "MapTriggers.slabs", [&](const auto& item) { return read_trigger_slab(item); });
			data.windings = plain::read_array(native.windings, native.windingCount, "MapTriggers.windings", [&](const auto& item) { return read_trigger_winding(item); });
			data.winding_points = plain::read_array(native.windingPoints, native.windingPointCount, "MapTriggers.windingPoints", [&](const auto& item) { return read_trigger_winding_point(item); });
			return data;
		}

		void write_map_triggers(MapTriggers& native, const asset_data::mapents::map_triggers& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
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

		std::size_t size_map_triggers([[maybe_unused]] const asset_data::mapents::map_triggers& data)
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

		asset_data::mapents::client_map_triggers read_client_map_triggers(const ClientMapTriggers& native)
		{
			plain::trace("read_client_map_triggers");
			asset_data::mapents::client_map_triggers data;
			data.count = native.count;
			data.models = plain::read_array(native.models, native.count, "ClientMapTriggers.models", [&](const auto& item) { return read_client_trigger_model(item); });
			data.hulls = plain::read_array(native.hulls, native.hullCount, "ClientMapTriggers.hulls", [&](const auto& item) { return read_client_trigger_hull(item); });
			data.slabs = plain::read_array(native.slabs, native.slabCount, "ClientMapTriggers.slabs", [&](const auto& item) { return read_trigger_slab(item); });
			return data;
		}

		void write_client_map_triggers(ClientMapTriggers& native, const asset_data::mapents::client_map_triggers& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.count = data.count;
			native.models = plain::write_array<ClientTriggerModel>(data.models, memory, [&](auto& out, const auto& in) { write_client_trigger_model(out, in, resolve, memory); });
			native.hulls = plain::write_array<ClientTriggerHull>(data.hulls, memory, [&](auto& out, const auto& in) { write_client_trigger_hull(out, in, resolve, memory); });
			native.slabs = plain::write_array<TriggerSlab>(data.slabs, memory, [&](auto& out, const auto& in) { write_trigger_slab(out, in, resolve, memory); });
			native.hullCount = plain::count_of<std::uint32_t>(data.hulls);
			native.slabCount = plain::count_of<std::uint32_t>(data.slabs);
		}

		std::size_t size_client_map_triggers([[maybe_unused]] const asset_data::mapents::client_map_triggers& data)
		{
			std::size_t size = 0;
			if (data.models)
			{
				size += plain::sized(std::max<std::size_t>(data.models->size(), 1) * sizeof(ClientTriggerModel));
				for (const auto& item : *data.models)
				{
					size += size_client_trigger_model(item);
				}
			}
			if (data.hulls)
			{
				size += plain::sized(std::max<std::size_t>(data.hulls->size(), 1) * sizeof(ClientTriggerHull));
				for (const auto& item : *data.hulls)
				{
					size += size_client_trigger_hull(item);
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
			return size;
		}

		asset_data::mapents::spatial_partition_tree read_spatial_partition_tree(const SpatialPartition_Tree& native)
		{
			plain::trace("read_spatial_partition_tree");
			asset_data::mapents::spatial_partition_tree data;
			data.tree = plain::read_array(native.tree, native.treeSize, "SpatialPartition_Tree.tree", [&](const auto& item) { return item; });
			data.always_list = plain::read_array(native.alwaysList, native.alwaysListLength, "SpatialPartition_Tree.alwaysList", [&](const auto& item) { return item; });
			return data;
		}

		void write_spatial_partition_tree(SpatialPartition_Tree& native, const asset_data::mapents::spatial_partition_tree& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.tree = plain::write_array<std::uint8_t>(data.tree, memory, [&](auto& out, const auto& in) { out = in; });
			native.alwaysList = plain::write_array<std::uint32_t>(data.always_list, memory, [&](auto& out, const auto& in) { out = in; });
			native.treeSize = plain::count_of<std::uint32_t>(data.tree);
			native.alwaysListLength = plain::count_of<std::uint32_t>(data.always_list);
		}

		std::size_t size_spatial_partition_tree([[maybe_unused]] const asset_data::mapents::spatial_partition_tree& data)
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

		asset_data::mapents::client_triggers read_client_triggers(const ClientTriggers& native)
		{
			plain::trace("read_client_triggers");
			asset_data::mapents::client_triggers data;
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
			data.detail_sound_bank = plain::read_array(native.detailSoundBank, native.trigger.count, "ClientTriggers.detailSoundBank", [&](const auto& item) { return plain::read_asset(ASSET_TYPE_SOUND_BANK_TRANSIENT, item); });
			data.audio_rvb_pan_info = plain::read_array(native.audioRvbPanInfo, native.trigger.count, "ClientTriggers.audioRvbPanInfo", [&](const auto& item) { return read_ct_aud_rvb_pan_info(item); });
			data.parent = plain::read_array(native.parent, native.trigger.count, "ClientTriggers.parent", [&](const auto& item) { return item; });
			data.link_to = plain::read_array(native.linkTo, native.trigger.count, "ClientTriggers.linkTo", [&](const auto& item) { return plain::read_pointee(item, [&](const auto& pointee) { return read_client_entity_link_to_def(pointee); }); });
			data.spatial_tree = plain::read_single(native.spatialTree, [&](const auto& item) { return read_spatial_partition_tree(item); });
			return data;
		}

		void write_client_triggers(ClientTriggers& native, const asset_data::mapents::client_triggers& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
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
			native.detailSoundBank = plain::write_array<SndBankTransient*>(data.detail_sound_bank, memory, [&](auto& out, const auto& in) { out = plain::resolve_asset<SndBankTransient>(resolve, ASSET_TYPE_SOUND_BANK_TRANSIENT, in); });
			native.audioRvbPanInfo = plain::write_array<CTAudRvbPanInfo>(data.audio_rvb_pan_info, memory, [&](auto& out, const auto& in) { write_ct_aud_rvb_pan_info(out, in, resolve, memory); });
			native.parent = plain::write_array<std::int16_t>(data.parent, memory, [&](auto& out, const auto& in) { out = in; });
			native.linkTo = plain::write_array<ClientEntityLinkToDef*>(data.link_to, memory, [&](auto& out, const auto& in) { out = plain::write_pointee<ClientEntityLinkToDef>(in, memory, [&](auto& pointee, const auto& value) { write_client_entity_link_to_def(pointee, value, resolve, memory); }); });
			native.spatialTree = plain::write_single<SpatialPartition_Tree>(data.spatial_tree, memory, [&](auto& out, const auto& in) { write_spatial_partition_tree(out, in, resolve, memory); });
			native.triggerStringLength = plain::count_of<std::uint32_t>(data.trigger_string);
		}

		std::size_t size_client_triggers([[maybe_unused]] const asset_data::mapents::client_triggers& data)
		{
			std::size_t size = 0;
			size += size_client_map_triggers(data.trigger);
			if (data.trigger_string)
			{
				size += plain::sized(std::max<std::size_t>(data.trigger_string->size(), 1) * sizeof(char));
			}
			if (data.vision_set_triggers)
			{
				size += plain::sized(std::max<std::size_t>(data.vision_set_triggers->size(), 1) * sizeof(std::int16_t));
			}
			if (data.trigger_type)
			{
				size += plain::sized(std::max<std::size_t>(data.trigger_type->size(), 1) * sizeof(std::uint16_t));
			}
			if (data.origins)
			{
				size += plain::sized(std::max<std::size_t>(data.origins->size(), 1) * sizeof(vec3_t));
			}
			if (data.script_delay)
			{
				size += plain::sized(std::max<std::size_t>(data.script_delay->size(), 1) * sizeof(float));
			}
			if (data.priority)
			{
				size += plain::sized(std::max<std::size_t>(data.priority->size(), 1) * sizeof(float));
			}
			if (data.audio_triggers)
			{
				size += plain::sized(std::max<std::size_t>(data.audio_triggers->size(), 1) * sizeof(std::int16_t));
			}
			if (data.blend_lookup)
			{
				size += plain::sized(std::max<std::size_t>(data.blend_lookup->size(), 1) * sizeof(std::int16_t));
			}
			if (data.npc_triggers)
			{
				size += plain::sized(std::max<std::size_t>(data.npc_triggers->size(), 1) * sizeof(std::int16_t));
			}
			if (data.audio_state_ids)
			{
				size += plain::sized(std::max<std::size_t>(data.audio_state_ids->size(), 1) * sizeof(std::int16_t));
			}
			if (data.detail_sound_bank)
			{
				size += plain::sized(std::max<std::size_t>(data.detail_sound_bank->size(), 1) * sizeof(SndBankTransient*));
			}
			if (data.audio_rvb_pan_info)
			{
				size += plain::sized(std::max<std::size_t>(data.audio_rvb_pan_info->size(), 1) * sizeof(CTAudRvbPanInfo));
				for (const auto& item : *data.audio_rvb_pan_info)
				{
					size += size_ct_aud_rvb_pan_info(item);
				}
			}
			if (data.parent)
			{
				size += plain::sized(std::max<std::size_t>(data.parent->size(), 1) * sizeof(std::int16_t));
			}
			if (data.link_to)
			{
				size += plain::sized(std::max<std::size_t>(data.link_to->size(), 1) * sizeof(ClientEntityLinkToDef*));
				for (const auto& item : *data.link_to)
				{
					if (item)
					{
						size += plain::sized(sizeof(ClientEntityLinkToDef)) + size_client_entity_link_to_def(*item);
					}
				}
			}
			if (data.spatial_tree)
			{
				size += plain::sized(sizeof(SpatialPartition_Tree));
				size += size_spatial_partition_tree(*data.spatial_tree);
			}
			return size;
		}

		asset_data::mapents::client_trigger_blend read_client_trigger_blend(const ClientTriggerBlend& native)
		{
			plain::trace("read_client_trigger_blend");
			asset_data::mapents::client_trigger_blend data;
			data.num_client_trigger_blend_nodes = native.numClientTriggerBlendNodes;
			data.blend_nodes = plain::read_array(native.blendNodes, native.numClientTriggerBlendNodes, "ClientTriggerBlend.blendNodes", [&](const auto& item) { return read_client_trigger_blend_node(item); });
			return data;
		}

		void write_client_trigger_blend(ClientTriggerBlend& native, const asset_data::mapents::client_trigger_blend& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.numClientTriggerBlendNodes = data.num_client_trigger_blend_nodes;
			native.blendNodes = plain::write_array<ClientTriggerBlendNode>(data.blend_nodes, memory, [&](auto& out, const auto& in) { write_client_trigger_blend_node(out, in, resolve, memory); });
		}

		std::size_t size_client_trigger_blend([[maybe_unused]] const asset_data::mapents::client_trigger_blend& data)
		{
			std::size_t size = 0;
			if (data.blend_nodes)
			{
				size += plain::sized(std::max<std::size_t>(data.blend_nodes->size(), 1) * sizeof(ClientTriggerBlendNode));
				for (const auto& item : *data.blend_nodes)
				{
					size += size_client_trigger_blend_node(item);
				}
			}
			return size;
		}

		asset_data::mapents::spawn_point_record_list read_spawn_point_record_list(const SpawnPointRecordList& native)
		{
			plain::trace("read_spawn_point_record_list");
			asset_data::mapents::spawn_point_record_list data;
			data.spawns_count = native.spawnsCount;
			data.spawns = plain::read_array(native.spawns, native.spawnsCount, "SpawnPointRecordList.spawns", [&](const auto& item) { return read_spawn_point_entity_record(item); });
			return data;
		}

		void write_spawn_point_record_list(SpawnPointRecordList& native, const asset_data::mapents::spawn_point_record_list& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.spawnsCount = data.spawns_count;
			native.spawns = plain::write_array<SpawnPointEntityRecord>(data.spawns, memory, [&](auto& out, const auto& in) { write_spawn_point_entity_record(out, in, resolve, memory); });
		}

		std::size_t size_spawn_point_record_list([[maybe_unused]] const asset_data::mapents::spawn_point_record_list& data)
		{
			std::size_t size = 0;
			if (data.spawns)
			{
				size += plain::sized(std::max<std::size_t>(data.spawns->size(), 1) * sizeof(SpawnPointEntityRecord));
				for (const auto& item : *data.spawns)
				{
					size += size_spawn_point_entity_record(item);
				}
			}
			return size;
		}

		asset_data::mapents::spline_record_list read_spline_record_list(const SplineRecordList& native)
		{
			plain::trace("read_spline_record_list");
			asset_data::mapents::spline_record_list data;
			data.spline_count = native.splineCount;
			data.splines = plain::read_array(native.splines, native.splineCount, "SplineRecordList.splines", [&](const auto& item) { return read_spline_point_record_list(item); });
			return data;
		}

		void write_spline_record_list(SplineRecordList& native, const asset_data::mapents::spline_record_list& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.splineCount = data.spline_count;
			native.splines = plain::write_array<SplinePointRecordList>(data.splines, memory, [&](auto& out, const auto& in) { write_spline_point_record_list(out, in, resolve, memory); });
		}

		std::size_t size_spline_record_list([[maybe_unused]] const asset_data::mapents::spline_record_list& data)
		{
			std::size_t size = 0;
			if (data.splines)
			{
				size += plain::sized(std::max<std::size_t>(data.splines->size(), 1) * sizeof(SplinePointRecordList));
				for (const auto& item : *data.splines)
				{
					size += size_spline_point_record_list(item);
				}
			}
			return size;
		}

		asset_data::mapents::spatial_partition_population_tree read_spatial_partition_population_tree(const SpatialPartition_Population_Tree& native)
		{
			plain::trace("read_spatial_partition_population_tree");
			asset_data::mapents::spatial_partition_population_tree data;
			data.partitions = plain::read_array(native.partitions, native.partitionCount, "SpatialPartition_Population_Tree.partitions", [&](const auto& item) { return read_spatial_partition_population_partition(item); });
			data.extents = plain::read_array(native.extents, native.partitionCount, "SpatialPartition_Population_Tree.extents", [&](const auto& item) { return read_extent_bounds(item); });
			return data;
		}

		void write_spatial_partition_population_tree(SpatialPartition_Population_Tree& native, const asset_data::mapents::spatial_partition_population_tree& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.partitions = plain::write_array<SpatialPartition_Population_Partition>(data.partitions, memory, [&](auto& out, const auto& in) { write_spatial_partition_population_partition(out, in, resolve, memory); });
			native.extents = plain::write_array<ExtentBounds>(data.extents, memory, [&](auto& out, const auto& in) { write_extent_bounds(out, in, resolve, memory); });
			native.partitionCount = plain::count_of<std::uint32_t>(data.partitions);
		}

		std::size_t size_spatial_partition_population_tree([[maybe_unused]] const asset_data::mapents::spatial_partition_population_tree& data)
		{
			std::size_t size = 0;
			if (data.partitions)
			{
				size += plain::sized(std::max<std::size_t>(data.partitions->size(), 1) * sizeof(SpatialPartition_Population_Partition));
				for (const auto& item : *data.partitions)
				{
					size += size_spatial_partition_population_partition(item);
				}
			}
			if (data.extents)
			{
				size += plain::sized(std::max<std::size_t>(data.extents->size(), 1) * sizeof(ExtentBounds));
				for (const auto& item : *data.extents)
				{
					size += size_extent_bounds(item);
				}
			}
			return size;
		}

		asset_data::mapents::spatial_partition_population read_spatial_partition_population(const SpatialPartition_Population& native)
		{
			plain::trace("read_spatial_partition_population");
			asset_data::mapents::spatial_partition_population data;
			data.tree = plain::read_single(native.tree, [&](const auto& item) { return read_spatial_partition_population_tree(item); });
			data.buckets = plain::read_array(native.buckets, native.partitionCount, "SpatialPartition_Population.buckets", [&](const auto& item) { return read_spatial_partition_population_node(item); });
			data.current_population = native.currentPopulation;
			data.buckets_are_changing = native.bucketsAreChanging;
			return data;
		}

		void write_spatial_partition_population(SpatialPartition_Population& native, const asset_data::mapents::spatial_partition_population& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.tree = plain::write_single<SpatialPartition_Population_Tree>(data.tree, memory, [&](auto& out, const auto& in) { write_spatial_partition_population_tree(out, in, resolve, memory); });
			native.buckets = plain::write_array<SpatialPartition_Population_Node>(data.buckets, memory, [&](auto& out, const auto& in) { write_spatial_partition_population_node(out, in, resolve, memory); });
			native.currentPopulation = data.current_population;
			native.bucketsAreChanging = data.buckets_are_changing;
			native.partitionCount = plain::count_of<std::uint32_t>(data.buckets);
		}

		std::size_t size_spatial_partition_population([[maybe_unused]] const asset_data::mapents::spatial_partition_population& data)
		{
			std::size_t size = 0;
			if (data.tree)
			{
				size += plain::sized(sizeof(SpatialPartition_Population_Tree));
				size += size_spatial_partition_population_tree(*data.tree);
			}
			if (data.buckets)
			{
				size += plain::sized(std::max<std::size_t>(data.buckets->size(), 1) * sizeof(SpatialPartition_Population_Node));
				for (const auto& item : *data.buckets)
				{
					size += size_spatial_partition_population_node(item);
				}
			}
			return size;
		}

		asset_data::mapents::spatial_partition_population_transient_map read_spatial_partition_population_transient_map(const SpatialPartition_Population_TransientMap& native)
		{
			plain::trace("read_spatial_partition_population_transient_map");
			asset_data::mapents::spatial_partition_population_transient_map data;
			data.transient_count = native.transientCount;
			data.partitions_for_transient = plain::read_array(native.partitionsForTransient, native.transientCount, "SpatialPartition_Population_TransientMap.partitionsForTransient", [&](const auto& item) { return read_bitarray_dynamic(item); });
			return data;
		}

		void write_spatial_partition_population_transient_map(SpatialPartition_Population_TransientMap& native, const asset_data::mapents::spatial_partition_population_transient_map& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.transientCount = data.transient_count;
			native.partitionsForTransient = plain::write_array<bitarray_dynamic>(data.partitions_for_transient, memory, [&](auto& out, const auto& in) { write_bitarray_dynamic(out, in, resolve, memory); });
		}

		std::size_t size_spatial_partition_population_transient_map([[maybe_unused]] const asset_data::mapents::spatial_partition_population_transient_map& data)
		{
			std::size_t size = 0;
			if (data.partitions_for_transient)
			{
				size += plain::sized(std::max<std::size_t>(data.partitions_for_transient->size(), 1) * sizeof(bitarray_dynamic));
				for (const auto& item : *data.partitions_for_transient)
				{
					size += size_bitarray_dynamic(item);
				}
			}
			return size;
		}

		asset_data::mapents::spatial_partition_population_node read_spatial_partition_population_node(const SpatialPartition_Population_Node& native)
		{
			plain::trace("read_spatial_partition_population_node");
			asset_data::mapents::spatial_partition_population_node data;
			data.spatial_node_next = native.spatialNodeNext;
			return data;
		}

		void write_spatial_partition_population_node(SpatialPartition_Population_Node& native, const asset_data::mapents::spatial_partition_population_node& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.spatialNodeNext = data.spatial_node_next;
		}

		std::size_t size_spatial_partition_population_node([[maybe_unused]] const asset_data::mapents::spatial_partition_population_node& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::mapents::scriptable_physics_limits read_scriptable_physics_limits(const ScriptablePhysicsLimits& native)
		{
			plain::trace("read_scriptable_physics_limits");
			asset_data::mapents::scriptable_physics_limits data;
			data.client_standalone_main_rigid_body_count = native.clientStandaloneMainRigidBodyCount;
			data.client_standalone_detailed_rigid_body_count = native.clientStandaloneDetailedRigidBodyCount;
			data.client_standalone_motion_count = native.clientStandaloneMotionCount;
			data.server_standalone_main_rigid_body_count = native.serverStandaloneMainRigidBodyCount;
			data.server_standalone_detailed_rigid_body_count = native.serverStandaloneDetailedRigidBodyCount;
			data.server_standalone_motion_count = native.serverStandaloneMotionCount;
			return data;
		}

		void write_scriptable_physics_limits(ScriptablePhysicsLimits& native, const asset_data::mapents::scriptable_physics_limits& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.clientStandaloneMainRigidBodyCount = data.client_standalone_main_rigid_body_count;
			native.clientStandaloneDetailedRigidBodyCount = data.client_standalone_detailed_rigid_body_count;
			native.clientStandaloneMotionCount = data.client_standalone_motion_count;
			native.serverStandaloneMainRigidBodyCount = data.server_standalone_main_rigid_body_count;
			native.serverStandaloneDetailedRigidBodyCount = data.server_standalone_detailed_rigid_body_count;
			native.serverStandaloneMotionCount = data.server_standalone_motion_count;
		}

		std::size_t size_scriptable_physics_limits([[maybe_unused]] const asset_data::mapents::scriptable_physics_limits& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::mapents::scriptable_event_sun_data read_scriptable_event_sun_data(const Scriptable_EventSun_Data& native)
		{
			plain::trace("read_scriptable_event_sun_data");
			asset_data::mapents::scriptable_event_sun_data data;
			data.start_time = native.startTime;
			data.trans_time = native.transTime;
			data.start_color_linear_srgb = plain::read_scalars<float, 3>(&native.startColorLinearSrgb);
			data.start_intensity = native.startIntensity;
			data.target_intensity = native.targetIntensity;
			data.start_angles = plain::read_scalars<float, 3>(&native.startAngles);
			data.target_angles = plain::read_scalars<float, 3>(&native.targetAngles);
			return data;
		}

		void write_scriptable_event_sun_data(Scriptable_EventSun_Data& native, const asset_data::mapents::scriptable_event_sun_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.startTime = data.start_time;
			native.transTime = data.trans_time;
			plain::write_scalars(&native.startColorLinearSrgb, data.start_color_linear_srgb);
			native.startIntensity = data.start_intensity;
			native.targetIntensity = data.target_intensity;
			plain::write_scalars(&native.startAngles, data.start_angles);
			plain::write_scalars(&native.targetAngles, data.target_angles);
		}

		std::size_t size_scriptable_event_sun_data([[maybe_unused]] const asset_data::mapents::scriptable_event_sun_data& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::mapents::scriptable_reserved_dynents read_scriptable_reserved_dynents(const ScriptableReservedDynents& native)
		{
			plain::trace("read_scriptable_reserved_dynents");
			asset_data::mapents::scriptable_reserved_dynents data;
			data.free_head = native.freeHead;
			data.reserved_dynents = plain::read_array(native.reservedDynents, native.numReservedDynents, "ScriptableReservedDynents.reservedDynents", [&](const auto& item) { return read_scriptable_reserved_dynent(item); });
			return data;
		}

		void write_scriptable_reserved_dynents(ScriptableReservedDynents& native, const asset_data::mapents::scriptable_reserved_dynents& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.freeHead = data.free_head;
			native.reservedDynents = plain::write_array<ScriptableReservedDynent>(data.reserved_dynents, memory, [&](auto& out, const auto& in) { write_scriptable_reserved_dynent(out, in, resolve, memory); });
			native.numReservedDynents = plain::count_of<std::uint32_t>(data.reserved_dynents);
		}

		std::size_t size_scriptable_reserved_dynents([[maybe_unused]] const asset_data::mapents::scriptable_reserved_dynents& data)
		{
			std::size_t size = 0;
			if (data.reserved_dynents)
			{
				size += plain::sized(std::max<std::size_t>(data.reserved_dynents->size(), 1) * sizeof(ScriptableReservedDynent));
				for (const auto& item : *data.reserved_dynents)
				{
					size += size_scriptable_reserved_dynent(item);
				}
			}
			return size;
		}

		asset_data::mapents::scriptable_map_ents read_scriptable_map_ents(const ScriptableMapEnts& native)
		{
			plain::trace("read_scriptable_map_ents");
			asset_data::mapents::scriptable_map_ents data;
			data.total_instance_count = native.totalInstanceCount;
			data.reserved_instance_count = native.reservedInstanceCount;
			data.loot_instance_count = native.lootInstanceCount;
			data.server_instance_count = native.serverInstanceCount;
			data.server_fixed_part_count = native.serverFixedPartCount;
			data.server_usable_part_count = native.serverUsablePartCount;
			data.physics_limits = read_scriptable_physics_limits(native.physicsLimits);
			data.map_instances = plain::read_array(native.mapInstances, native.mapInstanceCount, "ScriptableMapEnts.mapInstances", [&](const auto& item) { return read_scriptable_instance(item); });
			data.sun_client_datas = [&] { std::array<asset_data::mapents::scriptable_event_sun_data, 2> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_scriptable_event_sun_data(native.sunClientDatas[i]); } return out; }();
			data.reserved_dynents = [&] { std::array<asset_data::mapents::scriptable_reserved_dynents, 2> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_scriptable_reserved_dynents(native.reservedDynents[i]); } return out; }();
			data.spatial_population_tree = plain::read_single(native.spatialPopulationTree, [&](const auto& item) { return read_spatial_partition_population_tree(item); });
			data.ff_mem_cost = native.ffMemCost;
			return data;
		}

		void write_scriptable_map_ents(ScriptableMapEnts& native, const asset_data::mapents::scriptable_map_ents& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
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

		std::size_t size_scriptable_map_ents([[maybe_unused]] const asset_data::mapents::scriptable_map_ents& data)
		{
			std::size_t size = 0;
			size += size_scriptable_physics_limits(data.physics_limits);
			if (data.map_instances)
			{
				size += plain::sized(std::max<std::size_t>(data.map_instances->size(), 1) * sizeof(ScriptableInstance));
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
				size += plain::sized(sizeof(SpatialPartition_Population_Tree));
				size += size_spatial_partition_population_tree(*data.spatial_population_tree);
			}
			return size;
		}

		asset_data::mapents::spawn_group read_spawn_group(const SpawnGroup& native)
		{
			plain::trace("read_spawn_group");
			asset_data::mapents::spawn_group data;
			data.point_count = native.pointCount;
			data.points = plain::read_array(native.points, native.pointCount, "SpawnGroup.points", [&](const auto& item) { return read_spawn_group_point(item); });
			data.clusters = plain::read_array(native.clusters, native.clusterCount, "SpawnGroup.clusters", [&](const auto& item) { return read_spawn_group_cluster(item); });
			data.zones = plain::read_array(native.zones, native.zoneCount, "SpawnGroup.zones", [&](const auto& item) { return read_spawn_group_zone(item); });
			return data;
		}

		void write_spawn_group(SpawnGroup& native, const asset_data::mapents::spawn_group& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.pointCount = data.point_count;
			native.points = plain::write_array<SpawnGroupPoint>(data.points, memory, [&](auto& out, const auto& in) { write_spawn_group_point(out, in, resolve, memory); });
			native.clusters = plain::write_array<SpawnGroupCluster>(data.clusters, memory, [&](auto& out, const auto& in) { write_spawn_group_cluster(out, in, resolve, memory); });
			native.zones = plain::write_array<SpawnGroupZone>(data.zones, memory, [&](auto& out, const auto& in) { write_spawn_group_zone(out, in, resolve, memory); });
			native.clusterCount = plain::count_of<std::uint16_t>(data.clusters);
			native.zoneCount = plain::count_of<std::uint16_t>(data.zones);
		}

		std::size_t size_spawn_group([[maybe_unused]] const asset_data::mapents::spawn_group& data)
		{
			std::size_t size = 0;
			if (data.points)
			{
				size += plain::sized(std::max<std::size_t>(data.points->size(), 1) * sizeof(SpawnGroupPoint));
				for (const auto& item : *data.points)
				{
					size += size_spawn_group_point(item);
				}
			}
			if (data.clusters)
			{
				size += plain::sized(std::max<std::size_t>(data.clusters->size(), 1) * sizeof(SpawnGroupCluster));
				for (const auto& item : *data.clusters)
				{
					size += size_spawn_group_cluster(item);
				}
			}
			if (data.zones)
			{
				size += plain::sized(std::max<std::size_t>(data.zones->size(), 1) * sizeof(SpawnGroupZone));
				for (const auto& item : *data.zones)
				{
					size += size_spawn_group_zone(item);
				}
			}
			return size;
		}

		asset_data::mapents::client_side_effects read_client_side_effects(const ClientSideEffects& native)
		{
			plain::trace("read_client_side_effects");
			asset_data::mapents::client_side_effects data;
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

		void write_client_side_effects(ClientSideEffects& native, const asset_data::mapents::client_side_effects& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
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

		std::size_t size_client_side_effects([[maybe_unused]] const asset_data::mapents::client_side_effects& data)
		{
			std::size_t size = 0;
			if (data.oneshot_effects)
			{
				size += plain::sized(std::max<std::size_t>(data.oneshot_effects->size(), 1) * sizeof(ClientOneshotEffectDef));
				for (const auto& item : *data.oneshot_effects)
				{
					size += size_client_oneshot_effect_def(item);
				}
			}
			if (data.oneshot_effects_tree)
			{
				size += plain::sized(sizeof(SpatialPartition_Tree));
				size += size_spatial_partition_tree(*data.oneshot_effects_tree);
			}
			if (data.oneshot_sound_indices)
			{
				size += plain::sized(std::max<std::size_t>(data.oneshot_sound_indices->size(), 1) * sizeof(std::uint32_t));
			}
			if (data.exploders)
			{
				size += plain::sized(std::max<std::size_t>(data.exploders->size(), 1) * sizeof(ClientExploderDef));
				for (const auto& item : *data.exploders)
				{
					size += size_client_exploder_def(item);
				}
			}
			if (data.interval_sounds)
			{
				size += plain::sized(std::max<std::size_t>(data.interval_sounds->size(), 1) * sizeof(ClientIntervalSoundDef));
				for (const auto& item : *data.interval_sounds)
				{
					size += size_client_interval_sound_def(item);
				}
			}
			if (data.loop_sounds)
			{
				size += plain::sized(std::max<std::size_t>(data.loop_sounds->size(), 1) * sizeof(ClientLoopSoundDef));
				for (const auto& item : *data.loop_sounds)
				{
					size += size_client_loop_sound_def(item);
				}
			}
			if (data.sounds_spatial_tree)
			{
				size += plain::sized(sizeof(SpatialPartition_Tree));
				size += size_spatial_partition_tree(*data.sounds_spatial_tree);
			}
			if (data.reactive_ents)
			{
				size += plain::sized(std::max<std::size_t>(data.reactive_ents->size(), 1) * sizeof(ClientReactiveEntDef));
				for (const auto& item : *data.reactive_ents)
				{
					size += size_client_reactive_ent_def(item);
				}
			}
			if (data.reactive_spatial_tree)
			{
				size += plain::sized(sizeof(SpatialPartition_Tree));
				size += size_spatial_partition_tree(*data.reactive_spatial_tree);
			}
			return size;
		}

		asset_data::mapents::create_fx_asset_data read_create_fx_asset_data(const CreateFxAssetData& native)
		{
			plain::trace("read_create_fx_asset_data");
			asset_data::mapents::create_fx_asset_data data;
			data.fx_palette_count = native.fxPaletteCount;
			data.fx_palette = plain::read_array(native.fxPalette, native.fxPaletteCount, "CreateFxAssetData.fxPalette", [&](const auto& item) { return read_create_fx_palette_def(item); });
			return data;
		}

		void write_create_fx_asset_data(CreateFxAssetData& native, const asset_data::mapents::create_fx_asset_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.fxPaletteCount = data.fx_palette_count;
			native.fxPalette = plain::write_array<CreateFxPaletteDef>(data.fx_palette, memory, [&](auto& out, const auto& in) { write_create_fx_palette_def(out, in, resolve, memory); });
		}

		std::size_t size_create_fx_asset_data([[maybe_unused]] const asset_data::mapents::create_fx_asset_data& data)
		{
			std::size_t size = 0;
			if (data.fx_palette)
			{
				size += plain::sized(std::max<std::size_t>(data.fx_palette->size(), 1) * sizeof(CreateFxPaletteDef));
				for (const auto& item : *data.fx_palette)
				{
					size += size_create_fx_palette_def(item);
				}
			}
			return size;
		}

		asset_data::mapents::server_side_effects read_server_side_effects(const ServerSideEffects& native)
		{
			plain::trace("read_server_side_effects");
			asset_data::mapents::server_side_effects data;
			data.exploder_count = native.exploderCount;
			data.exploders = plain::read_array(native.exploders, native.exploderCount, "ServerSideEffects.exploders", [&](const auto& item) { return read_server_exploder_def(item); });
			return data;
		}

		void write_server_side_effects(ServerSideEffects& native, const asset_data::mapents::server_side_effects& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.exploderCount = data.exploder_count;
			native.exploders = plain::write_array<ServerExploderDef>(data.exploders, memory, [&](auto& out, const auto& in) { write_server_exploder_def(out, in, resolve, memory); });
		}

		std::size_t size_server_side_effects([[maybe_unused]] const asset_data::mapents::server_side_effects& data)
		{
			std::size_t size = 0;
			if (data.exploders)
			{
				size += plain::sized(std::max<std::size_t>(data.exploders->size(), 1) * sizeof(ServerExploderDef));
				for (const auto& item : *data.exploders)
				{
					size += size_server_exploder_def(item);
				}
			}
			return size;
		}

		asset_data::mapents::spawner_list read_spawner_list(const SpawnerList& native)
		{
			plain::trace("read_spawner_list");
			asset_data::mapents::spawner_list data;
			data.spawner_count = native.spawnerCount;
			data.spanwer_list = plain::read_array(native.spanwerList, native.spawnerCount, "SpawnerList.spanwerList", [&](const auto& item) { return read_spawner(item); });
			return data;
		}

		void write_spawner_list(SpawnerList& native, const asset_data::mapents::spawner_list& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.spawnerCount = data.spawner_count;
			native.spanwerList = plain::write_array<Spawner>(data.spanwer_list, memory, [&](auto& out, const auto& in) { write_spawner(out, in, resolve, memory); });
		}

		std::size_t size_spawner_list([[maybe_unused]] const asset_data::mapents::spawner_list& data)
		{
			std::size_t size = 0;
			if (data.spanwer_list)
			{
				size += plain::sized(std::max<std::size_t>(data.spanwer_list->size(), 1) * sizeof(Spawner));
				for (const auto& item : *data.spanwer_list)
				{
					size += size_spawner(item);
				}
			}
			return size;
		}

		asset_data::mapents::map_ents read_map_ents(const MapEnts& native)
		{
			plain::trace("read_map_ents");
			asset_data::mapents::map_ents data;
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
			data.edge_lists = plain::read_array(native.edgeLists, native.numEdgeLists, "MapEnts.edgeLists", [&](const auto& item) { return plain::read_asset(ASSET_TYPE_EDGE_LIST, item); });
			data.edge_list_spatial_tree = plain::read_single(native.edgeListSpatialTree, [&](const auto& item) { return read_spatial_partition_tree(item); });
			data.client_models = plain::read_array(native.clientModels, native.numClientModels, "MapEnts.clientModels", [&](const auto& item) { return read_cm_client_model(item); });
			data.dyn_ent_count = std::to_array(native.dynEntCount);
			data.dyn_ent_count_total = native.dynEntCountTotal;
			data.dyn_entity_lists_count = static_cast<std::uint16_t>(native.dynEntityListsCount);
			data.dyn_ent_list_ids = plain::read_array(native.dynEntListIds, static_cast<std::uint16_t>(native.dynEntityListsCount), "MapEnts.dynEntListIds", [&](const auto& item) { return static_cast<std::uint16_t>(item); });
			data.dyn_ent_physics_setup_head = plain::read_block<std::array<std::array<std::uint16_t, 2>, 2>>(&native.dynEntPhysicsSetupHead);
			data.dyn_ent_physics_setup_tail = plain::read_block<std::array<std::array<std::uint16_t, 2>, 2>>(&native.dynEntPhysicsSetupTail);
			data.dyn_ent_no_spatial_count = std::to_array(native.dynEntNoSpatialCount);
			data.dyn_ent_max_client_history_count = native.dynEntMaxClientHistoryCount;
			data.dyn_ent_max_pose_part_count = native.dynEntMaxPosePartCount;
			data.dyn_ents_with_extra_pose_parts_count = native.dynEntsWithExtraPosePartsCount;
			data.dyn_ent_spatial_population_0 = plain::read_single(native.dynEntSpatialPopulation[0], [&](const auto& item) { return read_spatial_partition_population(item); });
			data.dyn_ent_spatial_population_1 = plain::read_single(native.dynEntSpatialPopulation[1], [&](const auto& item) { return read_spatial_partition_population(item); });
			data.dyn_ent_spatial_transient_map_0 = plain::read_single(native.dynEntSpatialTransientMap[0], [&](const auto& item) { return read_spatial_partition_population_transient_map(item); });
			data.dyn_ent_spatial_transient_map_1 = plain::read_single(native.dynEntSpatialTransientMap[1], [&](const auto& item) { return read_spatial_partition_population_transient_map(item); });
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

		void write_map_ents(MapEnts& native, const asset_data::mapents::map_ents& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
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
			native.edgeLists = plain::write_array<MapEdgeList*>(data.edge_lists, memory, [&](auto& out, const auto& in) { out = plain::resolve_asset<MapEdgeList>(resolve, ASSET_TYPE_EDGE_LIST, in); });
			native.edgeListSpatialTree = plain::write_single<SpatialPartition_Tree>(data.edge_list_spatial_tree, memory, [&](auto& out, const auto& in) { write_spatial_partition_tree(out, in, resolve, memory); });
			native.clientModels = plain::write_array<CM_ClientModel>(data.client_models, memory, [&](auto& out, const auto& in) { write_cm_client_model(out, in, resolve, memory); });
			std::ranges::copy(data.dyn_ent_count, native.dynEntCount);
			native.dynEntCountTotal = data.dyn_ent_count_total;
			native.dynEntityListsCount = static_cast<DynEntityListId>(data.dyn_entity_lists_count);
			native.dynEntListIds = plain::write_array<DynEntityListId>(data.dyn_ent_list_ids, memory, [&](auto& out, const auto& in) { out = static_cast<DynEntityListId>(in); });
			plain::write_block(&native.dynEntPhysicsSetupHead, data.dyn_ent_physics_setup_head);
			plain::write_block(&native.dynEntPhysicsSetupTail, data.dyn_ent_physics_setup_tail);
			std::ranges::copy(data.dyn_ent_no_spatial_count, native.dynEntNoSpatialCount);
			native.dynEntMaxClientHistoryCount = data.dyn_ent_max_client_history_count;
			native.dynEntMaxPosePartCount = data.dyn_ent_max_pose_part_count;
			native.dynEntsWithExtraPosePartsCount = data.dyn_ents_with_extra_pose_parts_count;
			native.dynEntSpatialPopulation[0] = plain::write_single<SpatialPartition_Population>(data.dyn_ent_spatial_population_0, memory, [&](auto& out, const auto& in) { write_spatial_partition_population(out, in, resolve, memory); });
			native.dynEntSpatialPopulation[1] = plain::write_single<SpatialPartition_Population>(data.dyn_ent_spatial_population_1, memory, [&](auto& out, const auto& in) { write_spatial_partition_population(out, in, resolve, memory); });
			native.dynEntSpatialTransientMap[0] = plain::write_single<SpatialPartition_Population_TransientMap>(data.dyn_ent_spatial_transient_map_0, memory, [&](auto& out, const auto& in) { write_spatial_partition_population_transient_map(out, in, resolve, memory); });
			native.dynEntSpatialTransientMap[1] = plain::write_single<SpatialPartition_Population_TransientMap>(data.dyn_ent_spatial_transient_map_1, memory, [&](auto& out, const auto& in) { write_spatial_partition_population_transient_map(out, in, resolve, memory); });
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
			native.numEdgeLists = plain::count_of<std::uint32_t>(data.edge_lists);
			native.numClientModels = plain::count_of<std::uint32_t>(data.client_models);
			native.clientEntAnchorCount = plain::count_of<std::uint32_t>(data.client_ent_anchors);
			native.exploderNameTotal = plain::count_of<std::uint32_t>(data.exploder_names);
			native.numMayhemScenes = plain::count_of<std::uint32_t>(data.mayhem_scenes);
			native.audioPASpeakerCount = plain::count_of<std::uint32_t>(data.audio_pa_speakers);
			native.numAudioPropNodes = plain::count_of<std::uint32_t>(data.audio_prop_nodes);
			native.numAudioPropEdges = plain::count_of<std::uint32_t>(data.audio_prop_edges);
			native.numCollmaps = plain::count_of<std::uint32_t>(data.collmap_lookups);
		}

		std::size_t size_map_ents([[maybe_unused]] const asset_data::mapents::map_ents& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.entity_string)
			{
				size += plain::sized(std::max<std::size_t>(data.entity_string->size(), 1) * sizeof(char));
			}
			size += size_map_triggers(data.trigger);
			size += size_client_triggers(data.client_trigger);
			size += size_client_trigger_blend(data.client_trigger_blend);
			size += size_spawn_point_record_list(data.spawn_list);
			size += size_spline_record_list(data.spline_list);
			if (data.havok_ents_shape_data)
			{
				size += plain::sized(std::max<std::size_t>(data.havok_ents_shape_data->size(), 1) * sizeof(char));
			}
			if (data.cmodels)
			{
				size += plain::sized(std::max<std::size_t>(data.cmodels->size(), 1) * sizeof(cmodel_t));
				for (const auto& item : *data.cmodels)
				{
					size += size_cmodel_t(item);
				}
			}
			if (data.edge_lists)
			{
				size += plain::sized(std::max<std::size_t>(data.edge_lists->size(), 1) * sizeof(MapEdgeList*));
			}
			if (data.edge_list_spatial_tree)
			{
				size += plain::sized(sizeof(SpatialPartition_Tree));
				size += size_spatial_partition_tree(*data.edge_list_spatial_tree);
			}
			if (data.client_models)
			{
				size += plain::sized(std::max<std::size_t>(data.client_models->size(), 1) * sizeof(CM_ClientModel));
				for (const auto& item : *data.client_models)
				{
					size += size_cm_client_model(item);
				}
			}
			if (data.dyn_ent_list_ids)
			{
				size += plain::sized(std::max<std::size_t>(data.dyn_ent_list_ids->size(), 1) * sizeof(DynEntityListId));
			}
			if (data.dyn_ent_spatial_population_0)
			{
				size += plain::sized(sizeof(SpatialPartition_Population));
				size += size_spatial_partition_population(*data.dyn_ent_spatial_population_0);
			}
			if (data.dyn_ent_spatial_population_1)
			{
				size += plain::sized(sizeof(SpatialPartition_Population));
				size += size_spatial_partition_population(*data.dyn_ent_spatial_population_1);
			}
			if (data.dyn_ent_spatial_transient_map_0)
			{
				size += plain::sized(sizeof(SpatialPartition_Population_TransientMap));
				size += size_spatial_partition_population_transient_map(*data.dyn_ent_spatial_transient_map_0);
			}
			if (data.dyn_ent_spatial_transient_map_1)
			{
				size += plain::sized(sizeof(SpatialPartition_Population_TransientMap));
				size += size_spatial_partition_population_transient_map(*data.dyn_ent_spatial_transient_map_1);
			}
			size += size_spatial_partition_population_node(data.dyn_ent_no_spatial_list);
			if (data.client_ent_anchors)
			{
				size += plain::sized(std::max<std::size_t>(data.client_ent_anchors->size(), 1) * sizeof(ClientEntAnchor));
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
				size += plain::sized(std::max<std::size_t>(data.exploder_names->size(), 1) * sizeof(scr_string_t));
			}
			size += size_server_side_effects(data.server_side_effects);
			if (data.mayhem_scenes)
			{
				size += plain::sized(std::max<std::size_t>(data.mayhem_scenes->size(), 1) * sizeof(MayhemInstance));
				for (const auto& item : *data.mayhem_scenes)
				{
					size += size_mayhem_instance(item);
				}
			}
			size += size_spawner_list(data.spawners);
			if (data.audio_pa_speakers)
			{
				size += plain::sized(std::max<std::size_t>(data.audio_pa_speakers->size(), 1) * sizeof(AudioPASpeaker));
				for (const auto& item : *data.audio_pa_speakers)
				{
					size += size_audio_pa_speaker(item);
				}
			}
			if (data.audio_prop_nodes)
			{
				size += plain::sized(std::max<std::size_t>(data.audio_prop_nodes->size(), 1) * sizeof(AudioPropagationNode));
				for (const auto& item : *data.audio_prop_nodes)
				{
					size += size_audio_propagation_node(item);
				}
			}
			if (data.audio_prop_edges)
			{
				size += plain::sized(std::max<std::size_t>(data.audio_prop_edges->size(), 1) * sizeof(AudioPropagationEdge));
				for (const auto& item : *data.audio_prop_edges)
				{
					size += size_audio_propagation_edge(item);
				}
			}
			if (data.collmap_lookups)
			{
				size += plain::sized(std::max<std::size_t>(data.collmap_lookups->size(), 1) * sizeof(CollMapLookupEntry));
				for (const auto& item : *data.collmap_lookups)
				{
					size += size_coll_map_lookup_entry(item);
				}
			}
			return size;
		}

		asset_data::mapents::trigger_model read_trigger_model(const TriggerModel& native)
		{
			plain::trace("read_trigger_model");
			asset_data::mapents::trigger_model data;
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

		void write_trigger_model(TriggerModel& native, const asset_data::mapents::trigger_model& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
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

		std::size_t size_trigger_model([[maybe_unused]] const asset_data::mapents::trigger_model& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::mapents::bounds read_bounds(const Bounds& native)
		{
			plain::trace("read_bounds");
			asset_data::mapents::bounds data;
			data.mid_point = plain::read_scalars<float, 3>(&native.midPoint);
			data.half_size = plain::read_scalars<float, 3>(&native.halfSize);
			return data;
		}

		void write_bounds(Bounds& native, const asset_data::mapents::bounds& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.midPoint, data.mid_point);
			plain::write_scalars(&native.halfSize, data.half_size);
		}

		std::size_t size_bounds([[maybe_unused]] const asset_data::mapents::bounds& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::mapents::trigger_hull read_trigger_hull(const TriggerHull& native)
		{
			plain::trace("read_trigger_hull");
			asset_data::mapents::trigger_hull data;
			data.bounds = read_bounds(native.bounds);
			data.contents = native.contents;
			data.slab_count = native.slabCount;
			data.first_slab = native.firstSlab;
			return data;
		}

		void write_trigger_hull(TriggerHull& native, const asset_data::mapents::trigger_hull& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_bounds(native.bounds, data.bounds, resolve, memory);
			native.contents = data.contents;
			native.slabCount = data.slab_count;
			native.firstSlab = data.first_slab;
		}

		std::size_t size_trigger_hull([[maybe_unused]] const asset_data::mapents::trigger_hull& data)
		{
			std::size_t size = 0;
			size += size_bounds(data.bounds);
			return size;
		}

		asset_data::mapents::trigger_slab read_trigger_slab(const TriggerSlab& native)
		{
			plain::trace("read_trigger_slab");
			asset_data::mapents::trigger_slab data;
			data.dir = plain::read_scalars<float, 3>(&native.dir);
			data.mid_point = native.midPoint;
			data.half_size = native.halfSize;
			return data;
		}

		void write_trigger_slab(TriggerSlab& native, const asset_data::mapents::trigger_slab& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.dir, data.dir);
			native.midPoint = data.mid_point;
			native.halfSize = data.half_size;
		}

		std::size_t size_trigger_slab([[maybe_unused]] const asset_data::mapents::trigger_slab& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::mapents::trigger_winding read_trigger_winding(const TriggerWinding& native)
		{
			plain::trace("read_trigger_winding");
			asset_data::mapents::trigger_winding data;
			data.dir = plain::read_scalars<float, 3>(&native.dir);
			data.point_count = native.pointCount;
			data.first_point = native.firstPoint;
			data.flags = native.flags;
			return data;
		}

		void write_trigger_winding(TriggerWinding& native, const asset_data::mapents::trigger_winding& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.dir, data.dir);
			native.pointCount = data.point_count;
			native.firstPoint = data.first_point;
			native.flags = data.flags;
		}

		std::size_t size_trigger_winding([[maybe_unused]] const asset_data::mapents::trigger_winding& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::mapents::trigger_winding_point read_trigger_winding_point(const TriggerWindingPoint& native)
		{
			plain::trace("read_trigger_winding_point");
			asset_data::mapents::trigger_winding_point data;
			data.loc = plain::read_scalars<float, 3>(&native.loc);
			return data;
		}

		void write_trigger_winding_point(TriggerWindingPoint& native, const asset_data::mapents::trigger_winding_point& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.loc, data.loc);
		}

		std::size_t size_trigger_winding_point([[maybe_unused]] const asset_data::mapents::trigger_winding_point& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::mapents::client_trigger_model read_client_trigger_model(const ClientTriggerModel& native)
		{
			plain::trace("read_client_trigger_model");
			asset_data::mapents::client_trigger_model data;
			data.hull_count = native.hullCount;
			data.first_hull = native.firstHull;
			data.flags = native.flags;
			return data;
		}

		void write_client_trigger_model(ClientTriggerModel& native, const asset_data::mapents::client_trigger_model& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.hullCount = data.hull_count;
			native.firstHull = data.first_hull;
			native.flags = data.flags;
		}

		std::size_t size_client_trigger_model([[maybe_unused]] const asset_data::mapents::client_trigger_model& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::mapents::client_trigger_hull read_client_trigger_hull(const ClientTriggerHull& native)
		{
			plain::trace("read_client_trigger_hull");
			asset_data::mapents::client_trigger_hull data;
			data.trigger_space_bounds = read_bounds(native.triggerSpaceBounds);
			data.slab_count = native.slabCount;
			data.first_slab = native.firstSlab;
			return data;
		}

		void write_client_trigger_hull(ClientTriggerHull& native, const asset_data::mapents::client_trigger_hull& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_bounds(native.triggerSpaceBounds, data.trigger_space_bounds, resolve, memory);
			native.slabCount = data.slab_count;
			native.firstSlab = data.first_slab;
		}

		std::size_t size_client_trigger_hull([[maybe_unused]] const asset_data::mapents::client_trigger_hull& data)
		{
			std::size_t size = 0;
			size += size_bounds(data.trigger_space_bounds);
			return size;
		}

		asset_data::mapents::ct_aud_rvb_pan_info read_ct_aud_rvb_pan_info(const CTAudRvbPanInfo& native)
		{
			plain::trace("read_ct_aud_rvb_pan_info");
			asset_data::mapents::ct_aud_rvb_pan_info data;
			data.has_custom_position = native.hasCustomPosition;
			data.position = plain::read_scalars<float, 3>(&native.position);
			data.max_wall_distance = native.maxWallDistance;
			data.min_reverb_volume = native.minReverbVolume;
			return data;
		}

		void write_ct_aud_rvb_pan_info(CTAudRvbPanInfo& native, const asset_data::mapents::ct_aud_rvb_pan_info& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.hasCustomPosition = data.has_custom_position;
			plain::write_scalars(&native.position, data.position);
			native.maxWallDistance = data.max_wall_distance;
			native.minReverbVolume = data.min_reverb_volume;
		}

		std::size_t size_ct_aud_rvb_pan_info([[maybe_unused]] const asset_data::mapents::ct_aud_rvb_pan_info& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::mapents::client_entity_link_to_def read_client_entity_link_to_def(const ClientEntityLinkToDef& native)
		{
			plain::trace("read_client_entity_link_to_def");
			asset_data::mapents::client_entity_link_to_def data;
			data.anchor_index = native.anchorIndex;
			data.origin_offset = plain::read_scalars<float, 3>(&native.originOffset);
			data.angle_offset = plain::read_scalars<float, 3>(&native.angleOffset);
			return data;
		}

		void write_client_entity_link_to_def(ClientEntityLinkToDef& native, const asset_data::mapents::client_entity_link_to_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.anchorIndex = data.anchor_index;
			plain::write_scalars(&native.originOffset, data.origin_offset);
			plain::write_scalars(&native.angleOffset, data.angle_offset);
		}

		std::size_t size_client_entity_link_to_def([[maybe_unused]] const asset_data::mapents::client_entity_link_to_def& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::mapents::client_trigger_blend_node read_client_trigger_blend_node(const ClientTriggerBlendNode& native)
		{
			plain::trace("read_client_trigger_blend_node");
			asset_data::mapents::client_trigger_blend_node data;
			data.point_a = plain::read_scalars<float, 3>(&native.pointA);
			data.point_b = plain::read_scalars<float, 3>(&native.pointB);
			data.trigger_a = native.triggerA;
			data.trigger_b = native.triggerB;
			return data;
		}

		void write_client_trigger_blend_node(ClientTriggerBlendNode& native, const asset_data::mapents::client_trigger_blend_node& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.pointA, data.point_a);
			plain::write_scalars(&native.pointB, data.point_b);
			native.triggerA = data.trigger_a;
			native.triggerB = data.trigger_b;
		}

		std::size_t size_client_trigger_blend_node([[maybe_unused]] const asset_data::mapents::client_trigger_blend_node& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::mapents::spawn_point_entity_record read_spawn_point_entity_record(const SpawnPointEntityRecord& native)
		{
			plain::trace("read_spawn_point_entity_record");
			asset_data::mapents::spawn_point_entity_record data;
			data.index = native.index;
			data.name = plain::read_script_string(native.name);
			data.target = plain::read_script_string(native.target);
			data.script_noteworthy = plain::read_script_string(native.script_noteworthy);
			data.origin = plain::read_scalars<float, 3>(&native.origin);
			data.angles = plain::read_scalars<float, 3>(&native.angles);
			return data;
		}

		void write_spawn_point_entity_record(SpawnPointEntityRecord& native, const asset_data::mapents::spawn_point_entity_record& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.index = data.index;
			native.name = static_cast<scr_string_t>(plain::write_script_string(data.name));
			native.target = static_cast<scr_string_t>(plain::write_script_string(data.target));
			native.script_noteworthy = static_cast<scr_string_t>(plain::write_script_string(data.script_noteworthy));
			plain::write_scalars(&native.origin, data.origin);
			plain::write_scalars(&native.angles, data.angles);
		}

		std::size_t size_spawn_point_entity_record([[maybe_unused]] const asset_data::mapents::spawn_point_entity_record& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::mapents::spline_point_record_list read_spline_point_record_list(const SplinePointRecordList& native)
		{
			plain::trace("read_spline_point_record_list");
			asset_data::mapents::spline_point_record_list data;
			data.spline_point_count = native.splinePointCount;
			data.spline_length = native.splineLength;
			data.spline_points = plain::read_array(native.splinePoints, native.splinePointCount, "SplinePointRecordList.splinePoints", [&](const auto& item) { return read_spline_point_entity_record(item); });
			return data;
		}

		void write_spline_point_record_list(SplinePointRecordList& native, const asset_data::mapents::spline_point_record_list& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.splinePointCount = data.spline_point_count;
			native.splineLength = data.spline_length;
			native.splinePoints = plain::write_array<SplinePointEntityRecord>(data.spline_points, memory, [&](auto& out, const auto& in) { write_spline_point_entity_record(out, in, resolve, memory); });
		}

		std::size_t size_spline_point_record_list([[maybe_unused]] const asset_data::mapents::spline_point_record_list& data)
		{
			std::size_t size = 0;
			if (data.spline_points)
			{
				size += plain::sized(std::max<std::size_t>(data.spline_points->size(), 1) * sizeof(SplinePointEntityRecord));
				for (const auto& item : *data.spline_points)
				{
					size += size_spline_point_entity_record(item);
				}
			}
			return size;
		}

		asset_data::mapents::spline_point_entity_record read_spline_point_entity_record(const SplinePointEntityRecord& native)
		{
			plain::trace("read_spline_point_entity_record");
			asset_data::mapents::spline_point_entity_record data;
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

		void write_spline_point_entity_record(SplinePointEntityRecord& native, const asset_data::mapents::spline_point_entity_record& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
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

		std::size_t size_spline_point_entity_record([[maybe_unused]] const asset_data::mapents::spline_point_entity_record& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::mapents::cmodel_t read_cmodel_t(const cmodel_t& native)
		{
			plain::trace("read_cmodel_t");
			asset_data::mapents::cmodel_t data;
			data.bounds = read_bounds(native.bounds);
			data.radius = native.radius;
			data.physics_asset = plain::read_asset(ASSET_TYPE_PHYSICSASSET, native.physicsAsset);
			data.physics_shape_override_idx = native.physicsShapeOverrideIdx;
			data.nav_obstacle_idx = native.navObstacleIdx;
			data.edge_first_index = native.edgeFirstIndex;
			data.edge_total_count = native.edgeTotalCount;
			return data;
		}

		void write_cmodel_t(cmodel_t& native, const asset_data::mapents::cmodel_t& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_bounds(native.bounds, data.bounds, resolve, memory);
			native.radius = data.radius;
			native.physicsAsset = plain::resolve_asset<PhysicsAsset>(resolve, ASSET_TYPE_PHYSICSASSET, data.physics_asset);
			native.physicsShapeOverrideIdx = data.physics_shape_override_idx;
			native.navObstacleIdx = data.nav_obstacle_idx;
			native.edgeFirstIndex = data.edge_first_index;
			native.edgeTotalCount = data.edge_total_count;
		}

		std::size_t size_cmodel_t([[maybe_unused]] const asset_data::mapents::cmodel_t& data)
		{
			std::size_t size = 0;
			size += size_bounds(data.bounds);
			return size;
		}

		asset_data::mapents::cm_client_model read_cm_client_model(const CM_ClientModel& native)
		{
			plain::trace("read_cm_client_model");
			asset_data::mapents::cm_client_model data;
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

		void write_cm_client_model(CM_ClientModel& native, const asset_data::mapents::cm_client_model& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
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

		std::size_t size_cm_client_model([[maybe_unused]] const asset_data::mapents::cm_client_model& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::mapents::spatial_partition_population_partition read_spatial_partition_population_partition(const SpatialPartition_Population_Partition& native)
		{
			plain::trace("read_spatial_partition_population_partition");
			asset_data::mapents::spatial_partition_population_partition data;
			data.dist = native.dist;
			data.axis = native.axis;
			data.right_child = native.rightChild;
			return data;
		}

		void write_spatial_partition_population_partition(SpatialPartition_Population_Partition& native, const asset_data::mapents::spatial_partition_population_partition& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.dist = data.dist;
			native.axis = data.axis;
			native.rightChild = data.right_child;
		}

		std::size_t size_spatial_partition_population_partition([[maybe_unused]] const asset_data::mapents::spatial_partition_population_partition& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::mapents::extent_bounds read_extent_bounds(const ExtentBounds& native)
		{
			plain::trace("read_extent_bounds");
			asset_data::mapents::extent_bounds data;
			data.mins = plain::read_scalars<float, 3>(&native.mins);
			data.maxs = plain::read_scalars<float, 3>(&native.maxs);
			return data;
		}

		void write_extent_bounds(ExtentBounds& native, const asset_data::mapents::extent_bounds& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.mins, data.mins);
			plain::write_scalars(&native.maxs, data.maxs);
		}

		std::size_t size_extent_bounds([[maybe_unused]] const asset_data::mapents::extent_bounds& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::mapents::bitarray_dynamic read_bitarray_dynamic(const bitarray_dynamic& native)
		{
			plain::trace("read_bitarray_dynamic");
			asset_data::mapents::bitarray_dynamic data;
			data.array = plain::read_array(native.array, native.wordCount, "bitarray_dynamic.array", [&](const auto& item) { return item; });
			data.bit_count = native.bitCount;
			return data;
		}

		void write_bitarray_dynamic(bitarray_dynamic& native, const asset_data::mapents::bitarray_dynamic& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.array = plain::write_array<std::uint32_t>(data.array, memory, [&](auto& out, const auto& in) { out = in; });
			native.bitCount = data.bit_count;
			native.wordCount = plain::count_of<std::uint32_t>(data.array);
		}

		std::size_t size_bitarray_dynamic([[maybe_unused]] const asset_data::mapents::bitarray_dynamic& data)
		{
			std::size_t size = 0;
			if (data.array)
			{
				size += plain::sized(std::max<std::size_t>(data.array->size(), 1) * sizeof(std::uint32_t));
			}
			return size;
		}

		asset_data::mapents::client_ent_anchor read_client_ent_anchor(const ClientEntAnchor& native)
		{
			plain::trace("read_client_ent_anchor");
			asset_data::mapents::client_ent_anchor data;
			data.name = plain::read_script_string(native.name);
			data.ent_num = native.entNum;
			return data;
		}

		void write_client_ent_anchor(ClientEntAnchor& native, const asset_data::mapents::client_ent_anchor& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = static_cast<scr_string_t>(plain::write_script_string(data.name));
			native.entNum = data.ent_num;
		}

		std::size_t size_client_ent_anchor([[maybe_unused]] const asset_data::mapents::client_ent_anchor& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::mapents::scriptable_instance read_scriptable_instance(const ScriptableInstance& native)
		{
			plain::trace("read_scriptable_instance");
			asset_data::mapents::scriptable_instance data;
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

		void write_scriptable_instance(ScriptableInstance& native, const asset_data::mapents::scriptable_instance& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
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

		std::size_t size_scriptable_instance([[maybe_unused]] const asset_data::mapents::scriptable_instance& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::mapents::scriptable_reserved_dynent read_scriptable_reserved_dynent(const ScriptableReservedDynent& native)
		{
			plain::trace("read_scriptable_reserved_dynent");
			asset_data::mapents::scriptable_reserved_dynent data;
			data.dynent_id = native.dynentId;
			data.next = native.next;
			return data;
		}

		void write_scriptable_reserved_dynent(ScriptableReservedDynent& native, const asset_data::mapents::scriptable_reserved_dynent& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.dynentId = data.dynent_id;
			native.next = data.next;
		}

		std::size_t size_scriptable_reserved_dynent([[maybe_unused]] const asset_data::mapents::scriptable_reserved_dynent& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::mapents::spawn_group_point read_spawn_group_point(const SpawnGroupPoint& native)
		{
			plain::trace("read_spawn_group_point");
			asset_data::mapents::spawn_group_point data;
			data.origin = plain::read_scalars<float, 3>(&native.origin);
			data.angles = plain::read_scalars<float, 3>(&native.angles);
			data.cluster = native.cluster;
			data.zone = native.zone;
			return data;
		}

		void write_spawn_group_point(SpawnGroupPoint& native, const asset_data::mapents::spawn_group_point& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.origin, data.origin);
			plain::write_scalars(&native.angles, data.angles);
			native.cluster = data.cluster;
			native.zone = data.zone;
		}

		std::size_t size_spawn_group_point([[maybe_unused]] const asset_data::mapents::spawn_group_point& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::mapents::spawn_group_cluster read_spawn_group_cluster(const SpawnGroupCluster& native)
		{
			plain::trace("read_spawn_group_cluster");
			asset_data::mapents::spawn_group_cluster data;
			data.zone = native.zone;
			data.points = plain::read_array(native.points, native.pointCount, "SpawnGroupCluster.points", [&](const auto& item) { return item; });
			data.bounds = read_bounds(native.bounds);
			return data;
		}

		void write_spawn_group_cluster(SpawnGroupCluster& native, const asset_data::mapents::spawn_group_cluster& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.zone = data.zone;
			native.points = plain::write_array<std::uint16_t>(data.points, memory, [&](auto& out, const auto& in) { out = in; });
			write_bounds(native.bounds, data.bounds, resolve, memory);
			native.pointCount = plain::count_of<std::uint16_t>(data.points);
		}

		std::size_t size_spawn_group_cluster([[maybe_unused]] const asset_data::mapents::spawn_group_cluster& data)
		{
			std::size_t size = 0;
			if (data.points)
			{
				size += plain::sized(std::max<std::size_t>(data.points->size(), 1) * sizeof(std::uint16_t));
			}
			size += size_bounds(data.bounds);
			return size;
		}

		asset_data::mapents::spawn_group_zone read_spawn_group_zone(const SpawnGroupZone& native)
		{
			plain::trace("read_spawn_group_zone");
			asset_data::mapents::spawn_group_zone data;
			data.name = plain::read_script_string(native.name);
			data.points = plain::read_array(native.points, native.pointCount, "SpawnGroupZone.points", [&](const auto& item) { return item; });
			data.clusters = plain::read_array(native.clusters, native.clusterCount, "SpawnGroupZone.clusters", [&](const auto& item) { return item; });
			data.bounds = read_bounds(native.bounds);
			return data;
		}

		void write_spawn_group_zone(SpawnGroupZone& native, const asset_data::mapents::spawn_group_zone& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = static_cast<scr_string_t>(plain::write_script_string(data.name));
			native.points = plain::write_array<std::uint16_t>(data.points, memory, [&](auto& out, const auto& in) { out = in; });
			native.clusters = plain::write_array<std::uint16_t>(data.clusters, memory, [&](auto& out, const auto& in) { out = in; });
			write_bounds(native.bounds, data.bounds, resolve, memory);
			native.pointCount = plain::count_of<std::uint16_t>(data.points);
			native.clusterCount = plain::count_of<std::uint16_t>(data.clusters);
		}

		std::size_t size_spawn_group_zone([[maybe_unused]] const asset_data::mapents::spawn_group_zone& data)
		{
			std::size_t size = 0;
			if (data.points)
			{
				size += plain::sized(std::max<std::size_t>(data.points->size(), 1) * sizeof(std::uint16_t));
			}
			if (data.clusters)
			{
				size += plain::sized(std::max<std::size_t>(data.clusters->size(), 1) * sizeof(std::uint16_t));
			}
			size += size_bounds(data.bounds);
			return size;
		}

		asset_data::mapents::fx_combined_def read_fx_combined_def(const FxCombinedDef& native)
		{
			plain::trace("read_fx_combined_def");
			asset_data::mapents::fx_combined_def data;
			data.particle_system_def = plain::read_asset(ASSET_TYPE_VFX, native.particleSystemDef);
			return data;
		}

		void write_fx_combined_def(FxCombinedDef& native, const asset_data::mapents::fx_combined_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.particleSystemDef = plain::resolve_asset<ParticleSystemDef>(resolve, ASSET_TYPE_VFX, data.particle_system_def);
		}

		std::size_t size_fx_combined_def([[maybe_unused]] const asset_data::mapents::fx_combined_def& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::mapents::snd_alias_lookup read_snd_alias_lookup(const SndAliasLookup& native)
		{
			plain::trace("read_snd_alias_lookup");
			asset_data::mapents::snd_alias_lookup data;
			data.name = plain::read_string(native.name);
			return data;
		}

		void write_snd_alias_lookup(SndAliasLookup& native, const asset_data::mapents::snd_alias_lookup& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
		}

		std::size_t size_snd_alias_lookup([[maybe_unused]] const asset_data::mapents::snd_alias_lookup& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			return size;
		}

		asset_data::mapents::snd_alias_list read_snd_alias_list(const SndAliasList& native)
		{
			plain::trace("read_snd_alias_list");
			asset_data::mapents::snd_alias_list data;
			data.alias_name = plain::read_string(native.aliasName);
			data.id = native.id;
			data.head = plain::read_array(native.head, native.count, "SndAliasList.head", [&](const auto& item) { return read_snd_alias(item); });
			data.sequence = native.sequence;
			return data;
		}

		void write_snd_alias_list(SndAliasList& native, const asset_data::mapents::snd_alias_list& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.aliasName = plain::write_string(memory, data.alias_name);
			native.id = data.id;
			native.head = plain::write_array<SndAlias>(data.head, memory, [&](auto& out, const auto& in) { write_snd_alias(out, in, resolve, memory); });
			native.sequence = data.sequence;
			native.count = plain::count_of<std::int32_t>(data.head);
		}

		std::size_t size_snd_alias_list([[maybe_unused]] const asset_data::mapents::snd_alias_list& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.alias_name);
			if (data.head)
			{
				size += plain::sized(std::max<std::size_t>(data.head->size(), 1) * sizeof(SndAlias));
				for (const auto& item : *data.head)
				{
					size += size_snd_alias(item);
				}
			}
			return size;
		}

		asset_data::mapents::client_oneshot_effect_def read_client_oneshot_effect_def(const ClientOneshotEffectDef& native)
		{
			plain::trace("read_client_oneshot_effect_def");
			asset_data::mapents::client_oneshot_effect_def data;
			data.origin = plain::read_scalars<float, 3>(&native.origin);
			data.angles = plain::read_scalars<float, 3>(&native.angles);
			data.radius = native.radius;
			data.delay_msec = native.delayMsec;
			data.effect = read_fx_combined_def(native.effect);
			data.effect_sound = read_snd_alias_lookup(native.effectSound);
			data.alias_list = plain::read_single(native.aliasList, [&](const auto& item) { return read_snd_alias_list(item); });
			return data;
		}

		void write_client_oneshot_effect_def(ClientOneshotEffectDef& native, const asset_data::mapents::client_oneshot_effect_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.origin, data.origin);
			plain::write_scalars(&native.angles, data.angles);
			native.radius = data.radius;
			native.delayMsec = data.delay_msec;
			write_fx_combined_def(native.effect, data.effect, resolve, memory);
			write_snd_alias_lookup(native.effectSound, data.effect_sound, resolve, memory);
			native.aliasList = plain::write_single<SndAliasList>(data.alias_list, memory, [&](auto& out, const auto& in) { write_snd_alias_list(out, in, resolve, memory); });
		}

		std::size_t size_client_oneshot_effect_def([[maybe_unused]] const asset_data::mapents::client_oneshot_effect_def& data)
		{
			std::size_t size = 0;
			size += size_fx_combined_def(data.effect);
			size += size_snd_alias_lookup(data.effect_sound);
			if (data.alias_list)
			{
				size += plain::sized(sizeof(SndAliasList));
				size += size_snd_alias_list(*data.alias_list);
			}
			return size;
		}

		asset_data::mapents::snd_alias read_snd_alias(const SndAlias& native)
		{
			plain::trace("read_snd_alias");
			asset_data::mapents::snd_alias data;
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

		void write_snd_alias(SndAlias& native, const asset_data::mapents::snd_alias& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
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

		std::size_t size_snd_alias([[maybe_unused]] const asset_data::mapents::snd_alias& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.alias_name);
			size += plain::string_size(data.subtitle);
			size += plain::string_size(data.secondary_alias_name);
			size += plain::string_size(data.asset_file_name);
			return size;
		}

		asset_data::mapents::client_exploder_def read_client_exploder_def(const ClientExploderDef& native)
		{
			plain::trace("read_client_exploder_def");
			asset_data::mapents::client_exploder_def data;
			data.origin = plain::read_scalars<float, 3>(&native.origin);
			data.angles = plain::read_scalars<float, 3>(&native.angles);
			data.delay_msec = native.delayMsec;
			data.effect = read_fx_combined_def(native.effect);
			data.effect_sound = read_snd_alias_lookup(native.effectSound);
			data.alias_list = plain::read_single(native.aliasList, [&](const auto& item) { return read_snd_alias_list(item); });
			data.name = plain::read_script_string(native.name);
			return data;
		}

		void write_client_exploder_def(ClientExploderDef& native, const asset_data::mapents::client_exploder_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.origin, data.origin);
			plain::write_scalars(&native.angles, data.angles);
			native.delayMsec = data.delay_msec;
			write_fx_combined_def(native.effect, data.effect, resolve, memory);
			write_snd_alias_lookup(native.effectSound, data.effect_sound, resolve, memory);
			native.aliasList = plain::write_single<SndAliasList>(data.alias_list, memory, [&](auto& out, const auto& in) { write_snd_alias_list(out, in, resolve, memory); });
			native.name = static_cast<scr_string_t>(plain::write_script_string(data.name));
		}

		std::size_t size_client_exploder_def([[maybe_unused]] const asset_data::mapents::client_exploder_def& data)
		{
			std::size_t size = 0;
			size += size_fx_combined_def(data.effect);
			size += size_snd_alias_lookup(data.effect_sound);
			if (data.alias_list)
			{
				size += plain::sized(sizeof(SndAliasList));
				size += size_snd_alias_list(*data.alias_list);
			}
			return size;
		}

		asset_data::mapents::client_interval_sound_def read_client_interval_sound_def(const ClientIntervalSoundDef& native)
		{
			plain::trace("read_client_interval_sound_def");
			asset_data::mapents::client_interval_sound_def data;
			data.origin = plain::read_scalars<float, 3>(&native.origin);
			data.angles = plain::read_scalars<float, 3>(&native.angles);
			data.effect_sound = read_snd_alias_lookup(native.effectSound);
			data.alias_list = plain::read_single(native.aliasList, [&](const auto& item) { return read_snd_alias_list(item); });
			data.delay_min_msec = native.delayMinMsec;
			data.delay_max_msec = native.delayMaxMsec;
			return data;
		}

		void write_client_interval_sound_def(ClientIntervalSoundDef& native, const asset_data::mapents::client_interval_sound_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.origin, data.origin);
			plain::write_scalars(&native.angles, data.angles);
			write_snd_alias_lookup(native.effectSound, data.effect_sound, resolve, memory);
			native.aliasList = plain::write_single<SndAliasList>(data.alias_list, memory, [&](auto& out, const auto& in) { write_snd_alias_list(out, in, resolve, memory); });
			native.delayMinMsec = data.delay_min_msec;
			native.delayMaxMsec = data.delay_max_msec;
		}

		std::size_t size_client_interval_sound_def([[maybe_unused]] const asset_data::mapents::client_interval_sound_def& data)
		{
			std::size_t size = 0;
			size += size_snd_alias_lookup(data.effect_sound);
			if (data.alias_list)
			{
				size += plain::sized(sizeof(SndAliasList));
				size += size_snd_alias_list(*data.alias_list);
			}
			return size;
		}

		asset_data::mapents::client_loop_sound_def read_client_loop_sound_def(const ClientLoopSoundDef& native)
		{
			plain::trace("read_client_loop_sound_def");
			asset_data::mapents::client_loop_sound_def data;
			data.origin = plain::read_scalars<float, 3>(&native.origin);
			data.angles = plain::read_scalars<float, 3>(&native.angles);
			data.effect_sound = read_snd_alias_lookup(native.effectSound);
			data.alias_list = plain::read_single(native.aliasList, [&](const auto& item) { return read_snd_alias_list(item); });
			return data;
		}

		void write_client_loop_sound_def(ClientLoopSoundDef& native, const asset_data::mapents::client_loop_sound_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.origin, data.origin);
			plain::write_scalars(&native.angles, data.angles);
			write_snd_alias_lookup(native.effectSound, data.effect_sound, resolve, memory);
			native.aliasList = plain::write_single<SndAliasList>(data.alias_list, memory, [&](auto& out, const auto& in) { write_snd_alias_list(out, in, resolve, memory); });
		}

		std::size_t size_client_loop_sound_def([[maybe_unused]] const asset_data::mapents::client_loop_sound_def& data)
		{
			std::size_t size = 0;
			size += size_snd_alias_lookup(data.effect_sound);
			if (data.alias_list)
			{
				size += plain::sized(sizeof(SndAliasList));
				size += size_snd_alias_list(*data.alias_list);
			}
			return size;
		}

		asset_data::mapents::client_reactive_ent_def read_client_reactive_ent_def(const ClientReactiveEntDef& native)
		{
			plain::trace("read_client_reactive_ent_def");
			asset_data::mapents::client_reactive_ent_def data;
			data.origin = plain::read_scalars<float, 3>(&native.origin);
			data.angles = plain::read_scalars<float, 3>(&native.angles);
			data.effect = read_fx_combined_def(native.effect);
			data.effect_sound = read_snd_alias_lookup(native.effectSound);
			data.alias_list = plain::read_single(native.aliasList, [&](const auto& item) { return read_snd_alias_list(item); });
			data.radius = native.radius;
			return data;
		}

		void write_client_reactive_ent_def(ClientReactiveEntDef& native, const asset_data::mapents::client_reactive_ent_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.origin, data.origin);
			plain::write_scalars(&native.angles, data.angles);
			write_fx_combined_def(native.effect, data.effect, resolve, memory);
			write_snd_alias_lookup(native.effectSound, data.effect_sound, resolve, memory);
			native.aliasList = plain::write_single<SndAliasList>(data.alias_list, memory, [&](auto& out, const auto& in) { write_snd_alias_list(out, in, resolve, memory); });
			native.radius = data.radius;
		}

		std::size_t size_client_reactive_ent_def([[maybe_unused]] const asset_data::mapents::client_reactive_ent_def& data)
		{
			std::size_t size = 0;
			size += size_fx_combined_def(data.effect);
			size += size_snd_alias_lookup(data.effect_sound);
			if (data.alias_list)
			{
				size += plain::sized(sizeof(SndAliasList));
				size += size_snd_alias_list(*data.alias_list);
			}
			return size;
		}

		asset_data::mapents::create_fx_palette_def read_create_fx_palette_def(const CreateFxPaletteDef& native)
		{
			plain::trace("read_create_fx_palette_def");
			asset_data::mapents::create_fx_palette_def data;
			data.alias_name = plain::read_script_string(native.aliasName);
			data.effect_path = plain::read_script_string(native.effectPath);
			data.ref_count = native.refCount;
			return data;
		}

		void write_create_fx_palette_def(CreateFxPaletteDef& native, const asset_data::mapents::create_fx_palette_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.aliasName = static_cast<scr_string_t>(plain::write_script_string(data.alias_name));
			native.effectPath = static_cast<scr_string_t>(plain::write_script_string(data.effect_path));
			native.refCount = data.ref_count;
		}

		std::size_t size_create_fx_palette_def([[maybe_unused]] const asset_data::mapents::create_fx_palette_def& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::mapents::server_exploder_def read_server_exploder_def(const ServerExploderDef& native)
		{
			plain::trace("read_server_exploder_def");
			asset_data::mapents::server_exploder_def data;
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

		void write_server_exploder_def(ServerExploderDef& native, const asset_data::mapents::server_exploder_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
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

		std::size_t size_server_exploder_def([[maybe_unused]] const asset_data::mapents::server_exploder_def& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::mapents::gfx_placement read_gfx_placement(const GfxPlacement& native)
		{
			plain::trace("read_gfx_placement");
			asset_data::mapents::gfx_placement data;
			data.quat = plain::read_scalars<float, 4>(&native.quat);
			data.origin = plain::read_scalars<float, 3>(&native.origin);
			return data;
		}

		void write_gfx_placement(GfxPlacement& native, const asset_data::mapents::gfx_placement& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.quat, data.quat);
			plain::write_scalars(&native.origin, data.origin);
		}

		std::size_t size_gfx_placement([[maybe_unused]] const asset_data::mapents::gfx_placement& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::mapents::mayhem_instance read_mayhem_instance(const MayhemInstance& native)
		{
			plain::trace("read_mayhem_instance");
			asset_data::mapents::mayhem_instance data;
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

		void write_mayhem_instance(MayhemInstance& native, const asset_data::mapents::mayhem_instance& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
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

		std::size_t size_mayhem_instance([[maybe_unused]] const asset_data::mapents::mayhem_instance& data)
		{
			std::size_t size = 0;
			size += size_gfx_placement(data.initial_pose);
			if (data.link_to)
			{
				size += plain::sized(sizeof(ClientEntityLinkToDef));
				size += size_client_entity_link_to_def(*data.link_to);
			}
			return size;
		}

		asset_data::mapents::spawner read_spawner(const Spawner& native)
		{
			plain::trace("read_spawner");
			asset_data::mapents::spawner data;
			data.origin = plain::read_scalars<float, 3>(&native.origin);
			data.angles = plain::read_scalars<float, 3>(&native.angles);
			data.fields = plain::read_array(native.fields, native.numFields, "Spawner.fields", [&](const auto& item) { return read_spawner_field(item); });
			data.flags = native.flags;
			data.count = native.count;
			data.targetname = plain::read_script_string(native.targetname);
			data.classname = plain::read_script_string(native.classname);
			return data;
		}

		void write_spawner(Spawner& native, const asset_data::mapents::spawner& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
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

		std::size_t size_spawner([[maybe_unused]] const asset_data::mapents::spawner& data)
		{
			std::size_t size = 0;
			if (data.fields)
			{
				size += plain::sized(std::max<std::size_t>(data.fields->size(), 1) * sizeof(SpawnerField));
				for (const auto& item : *data.fields)
				{
					size += size_spawner_field(item);
				}
			}
			return size;
		}

		asset_data::mapents::spawner_field read_spawner_field(const SpawnerField& native)
		{
			plain::trace("read_spawner_field");
			asset_data::mapents::spawner_field data;
			data.key = plain::read_script_string(native.key);
			data.key_canonical = native.keyCanonical;
			data.value = plain::read_script_string(native.value);
			data.type = plain::enum_name(asset_data::mapents::variable_type_names, static_cast<std::int64_t>(native.type), "SpawnerField.type");
			return data;
		}

		void write_spawner_field(SpawnerField& native, const asset_data::mapents::spawner_field& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.key = static_cast<scr_string_t>(plain::write_script_string(data.key));
			native.keyCanonical = data.key_canonical;
			native.value = static_cast<scr_string_t>(plain::write_script_string(data.value));
			native.type = static_cast<VariableType>(plain::enum_value(asset_data::mapents::variable_type_names, data.type, "SpawnerField.type"));
		}

		std::size_t size_spawner_field([[maybe_unused]] const asset_data::mapents::spawner_field& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::mapents::audio_pa_speaker read_audio_pa_speaker(const AudioPASpeaker& native)
		{
			plain::trace("read_audio_pa_speaker");
			asset_data::mapents::audio_pa_speaker data;
			data.origin = plain::read_scalars<float, 3>(&native.origin);
			data.name_hash = native.nameHash;
			data.ent_channel_hash = native.entChannelHash;
			return data;
		}

		void write_audio_pa_speaker(AudioPASpeaker& native, const asset_data::mapents::audio_pa_speaker& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.origin, data.origin);
			native.nameHash = data.name_hash;
			native.entChannelHash = data.ent_channel_hash;
		}

		std::size_t size_audio_pa_speaker([[maybe_unused]] const asset_data::mapents::audio_pa_speaker& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::mapents::audio_propagation_node read_audio_propagation_node(const AudioPropagationNode& native)
		{
			plain::trace("read_audio_propagation_node");
			asset_data::mapents::audio_propagation_node data;
			data.position = plain::read_scalars<float, 3>(&native.position);
			data.num_edges = native.numEdges;
			data.edge_start_index = native.edgeStartIndex;
			return data;
		}

		void write_audio_propagation_node(AudioPropagationNode& native, const asset_data::mapents::audio_propagation_node& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.position, data.position);
			native.numEdges = data.num_edges;
			native.edgeStartIndex = data.edge_start_index;
		}

		std::size_t size_audio_propagation_node([[maybe_unused]] const asset_data::mapents::audio_propagation_node& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::mapents::audio_propagation_edge read_audio_propagation_edge(const AudioPropagationEdge& native)
		{
			plain::trace("read_audio_propagation_edge");
			asset_data::mapents::audio_propagation_edge data;
			data.neighbor = native.neighbor;
			data.portal_index = native.portalIndex;
			return data;
		}

		void write_audio_propagation_edge(AudioPropagationEdge& native, const asset_data::mapents::audio_propagation_edge& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.neighbor = data.neighbor;
			native.portalIndex = data.portal_index;
		}

		std::size_t size_audio_propagation_edge([[maybe_unused]] const asset_data::mapents::audio_propagation_edge& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::mapents::coll_map_lookup_entry read_coll_map_lookup_entry(const CollMapLookupEntry& native)
		{
			plain::trace("read_coll_map_lookup_entry");
			asset_data::mapents::coll_map_lookup_entry data;
			data.target_name = plain::read_string(native.targetName);
			data.brush_index = native.brushIndex;
			return data;
		}

		void write_coll_map_lookup_entry(CollMapLookupEntry& native, const asset_data::mapents::coll_map_lookup_entry& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.targetName = plain::write_string(memory, data.target_name);
			native.brushIndex = data.brush_index;
		}

		std::size_t size_coll_map_lookup_entry([[maybe_unused]] const asset_data::mapents::coll_map_lookup_entry& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.target_name);
			return size;
		}

	}

	std::expected<asset_data::mapents::map_ents, std::string> to_neutral(const MapEnts& native)
	{
		try
		{
			auto data = read_map_ents(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	MapEnts* from_neutral(const asset_data::mapents::map_ents& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<MapEnts>();
		write_map_ents(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::mapents::map_ents& data)
	{
		return plain::rounded(sizeof(MapEnts)) + size_map_ents(data) + 64;
	}

}
