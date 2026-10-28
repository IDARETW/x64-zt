// Emitted by tools/gen_iw8_neutral.py from tools/neutral_families/scriptable.json: do not edit.
#include <std_include.hpp>
#include "scriptable.hpp"
#include "plain.hpp"

namespace zonetool::iw8::neutral
{
	namespace
	{
		asset_data::scriptable::scriptable read_scriptable(const ScriptableDef& native);
		void write_scriptable(ScriptableDef& native, const asset_data::scriptable::scriptable& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable(const asset_data::scriptable::scriptable& data);
		asset_data::scriptable::scriptable_part_def read_scriptable_part_def(const ScriptablePartDef& native);
		void write_scriptable_part_def(ScriptablePartDef& native, const asset_data::scriptable::scriptable_part_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_part_def(const asset_data::scriptable::scriptable_part_def& data);
		asset_data::scriptable::scriptable_state_def read_scriptable_state_def(const ScriptableStateDef& native);
		void write_scriptable_state_def(ScriptableStateDef& native, const asset_data::scriptable::scriptable_state_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_state_def(const asset_data::scriptable::scriptable_state_def& data);
		asset_data::scriptable::scriptable_state_base_def read_scriptable_state_base_def(const ScriptableStateBaseDef& native);
		void write_scriptable_state_base_def(ScriptableStateBaseDef& native, const asset_data::scriptable::scriptable_state_base_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_state_base_def(const asset_data::scriptable::scriptable_state_base_def& data);
		asset_data::scriptable::scriptable_event_def read_scriptable_event_def(const ScriptableEventDef& native);
		void write_scriptable_event_def(ScriptableEventDef& native, const asset_data::scriptable::scriptable_event_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_def(const asset_data::scriptable::scriptable_event_def& data);
		asset_data::scriptable::scriptable_event_base_def read_scriptable_event_base_def(const ScriptableEventBaseDef& native);
		void write_scriptable_event_base_def(ScriptableEventBaseDef& native, const asset_data::scriptable::scriptable_event_base_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_base_def(const asset_data::scriptable::scriptable_event_base_def& data);
		asset_data::scriptable::scriptable_event_state_change_def read_scriptable_event_state_change_def(const ScriptableEventStateChangeDef& native);
		void write_scriptable_event_state_change_def(ScriptableEventStateChangeDef& native, const asset_data::scriptable::scriptable_event_state_change_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_state_change_def(const asset_data::scriptable::scriptable_event_state_change_def& data);
		asset_data::scriptable::scriptable_event_wait_def read_scriptable_event_wait_def(const ScriptableEventWaitDef& native);
		void write_scriptable_event_wait_def(ScriptableEventWaitDef& native, const asset_data::scriptable::scriptable_event_wait_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_wait_def(const asset_data::scriptable::scriptable_event_wait_def& data);
		asset_data::scriptable::scriptable_event_random_def read_scriptable_event_random_def(const ScriptableEventRandomDef& native);
		void write_scriptable_event_random_def(ScriptableEventRandomDef& native, const asset_data::scriptable::scriptable_event_random_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_random_def(const asset_data::scriptable::scriptable_event_random_def& data);
		asset_data::scriptable::scriptable_event_script_def read_scriptable_event_script_def(const ScriptableEventScriptDef& native);
		void write_scriptable_event_script_def(ScriptableEventScriptDef& native, const asset_data::scriptable::scriptable_event_script_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_script_def(const asset_data::scriptable::scriptable_event_script_def& data);
		asset_data::scriptable::scriptable_event_model_def read_scriptable_event_model_def(const ScriptableEventModelDef& native);
		void write_scriptable_event_model_def(ScriptableEventModelDef& native, const asset_data::scriptable::scriptable_event_model_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_model_def(const asset_data::scriptable::scriptable_event_model_def& data);
		asset_data::scriptable::scriptable_event_model_def_model read_scriptable_event_model_def_model(const ScriptableModelUnion& native);
		void write_scriptable_event_model_def_model(ScriptableModelUnion& native, const asset_data::scriptable::scriptable_event_model_def_model& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_model_def_model(const asset_data::scriptable::scriptable_event_model_def_model& data);
		asset_data::scriptable::scriptable_event_model_def_composite_model read_scriptable_event_model_def_composite_model(const ScriptableModelUnion& native);
		void write_scriptable_event_model_def_composite_model(ScriptableModelUnion& native, const asset_data::scriptable::scriptable_event_model_def_composite_model& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_model_def_composite_model(const asset_data::scriptable::scriptable_event_model_def_composite_model& data);
		asset_data::scriptable::scriptable_event_collision_def read_scriptable_event_collision_def(const ScriptableEventCollisionDef& native);
		void write_scriptable_event_collision_def(ScriptableEventCollisionDef& native, const asset_data::scriptable::scriptable_event_collision_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_collision_def(const asset_data::scriptable::scriptable_event_collision_def& data);
		asset_data::scriptable::scriptable_event_animation_def read_scriptable_event_animation_def(const ScriptableEventAnimationDef& native);
		void write_scriptable_event_animation_def(ScriptableEventAnimationDef& native, const asset_data::scriptable::scriptable_event_animation_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_animation_def(const asset_data::scriptable::scriptable_event_animation_def& data);
		asset_data::scriptable::scr_anim_t read_scr_anim_t(const scr_anim_t& native);
		void write_scr_anim_t(scr_anim_t& native, const asset_data::scriptable::scr_anim_t& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scr_anim_t(const asset_data::scriptable::scr_anim_t& data);
		asset_data::scriptable::scriptable_event_hide_show_bone_def read_scriptable_event_hide_show_bone_def(const ScriptableEventHideShowBoneDef& native);
		void write_scriptable_event_hide_show_bone_def(ScriptableEventHideShowBoneDef& native, const asset_data::scriptable::scriptable_event_hide_show_bone_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_hide_show_bone_def(const asset_data::scriptable::scriptable_event_hide_show_bone_def& data);
		asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def read_scriptable_event_disable_physics_sub_shape_def(const ScriptableEventDisablePhysicsSubShapeDef& native);
		void write_scriptable_event_disable_physics_sub_shape_def(ScriptableEventDisablePhysicsSubShapeDef& native, const asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_disable_physics_sub_shape_def(const asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def& data);
		asset_data::scriptable::scriptable_event_note_track_def read_scriptable_event_note_track_def(const ScriptableEventNoteTrackDef& native);
		void write_scriptable_event_note_track_def(ScriptableEventNoteTrackDef& native, const asset_data::scriptable::scriptable_event_note_track_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_note_track_def(const asset_data::scriptable::scriptable_event_note_track_def& data);
		asset_data::scriptable::scriptable_note_track_def read_scriptable_note_track_def(const ScriptableNoteTrackDef& native);
		void write_scriptable_note_track_def(ScriptableNoteTrackDef& native, const asset_data::scriptable::scriptable_note_track_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_note_track_def(const asset_data::scriptable::scriptable_note_track_def& data);
		asset_data::scriptable::scriptable_event_chunk_dynent_def read_scriptable_event_chunk_dynent_def(const ScriptableEventChunkDynentDef& native);
		void write_scriptable_event_chunk_dynent_def(ScriptableEventChunkDynentDef& native, const asset_data::scriptable::scriptable_event_chunk_dynent_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_chunk_dynent_def(const asset_data::scriptable::scriptable_event_chunk_dynent_def& data);
		asset_data::scriptable::scriptable_event_spawn_dynent_def read_scriptable_event_spawn_dynent_def(const ScriptableEventSpawnDynentDef& native);
		void write_scriptable_event_spawn_dynent_def(ScriptableEventSpawnDynentDef& native, const asset_data::scriptable::scriptable_event_spawn_dynent_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_spawn_dynent_def(const asset_data::scriptable::scriptable_event_spawn_dynent_def& data);
		asset_data::scriptable::fx_combined_def read_fx_combined_def(const FxCombinedDef& native);
		void write_fx_combined_def(FxCombinedDef& native, const asset_data::scriptable::fx_combined_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_fx_combined_def(const asset_data::scriptable::fx_combined_def& data);
		asset_data::scriptable::scriptable_event_pfx_def read_scriptable_event_pfx_def(const ScriptableEventPFXDef& native);
		void write_scriptable_event_pfx_def(ScriptableEventPFXDef& native, const asset_data::scriptable::scriptable_event_pfx_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_pfx_def(const asset_data::scriptable::scriptable_event_pfx_def& data);
		asset_data::scriptable::scriptable_event_sound_def read_scriptable_event_sound_def(const ScriptableEventSoundDef& native);
		void write_scriptable_event_sound_def(ScriptableEventSoundDef& native, const asset_data::scriptable::scriptable_event_sound_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_sound_def(const asset_data::scriptable::scriptable_event_sound_def& data);
		asset_data::scriptable::scriptable_event_explosion_def read_scriptable_event_explosion_def(const ScriptableEventExplosionDef& native);
		void write_scriptable_event_explosion_def(ScriptableEventExplosionDef& native, const asset_data::scriptable::scriptable_event_explosion_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_explosion_def(const asset_data::scriptable::scriptable_event_explosion_def& data);
		asset_data::scriptable::scriptable_event_light_def read_scriptable_event_light_def(const ScriptableEventLightDef& native);
		void write_scriptable_event_light_def(ScriptableEventLightDef& native, const asset_data::scriptable::scriptable_event_light_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_light_def(const asset_data::scriptable::scriptable_event_light_def& data);
		asset_data::scriptable::scriptable_event_sun_def read_scriptable_event_sun_def(const ScriptableEventSunDef& native);
		void write_scriptable_event_sun_def(ScriptableEventSunDef& native, const asset_data::scriptable::scriptable_event_sun_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_sun_def(const asset_data::scriptable::scriptable_event_sun_def& data);
		asset_data::scriptable::scriptable_event_rumble_def read_scriptable_event_rumble_def(const ScriptableEventRumbleDef& native);
		void write_scriptable_event_rumble_def(ScriptableEventRumbleDef& native, const asset_data::scriptable::scriptable_event_rumble_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_rumble_def(const asset_data::scriptable::scriptable_event_rumble_def& data);
		asset_data::scriptable::scriptable_event_screenshake_def read_scriptable_event_screenshake_def(const ScriptableEventScreenshakeDef& native);
		void write_scriptable_event_screenshake_def(ScriptableEventScreenshakeDef& native, const asset_data::scriptable::scriptable_event_screenshake_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_screenshake_def(const asset_data::scriptable::scriptable_event_screenshake_def& data);
		asset_data::scriptable::scriptable_event_part_damage_def read_scriptable_event_part_damage_def(const ScriptableEventPartDamageDef& native);
		void write_scriptable_event_part_damage_def(ScriptableEventPartDamageDef& native, const asset_data::scriptable::scriptable_event_part_damage_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_part_damage_def(const asset_data::scriptable::scriptable_event_part_damage_def& data);
		asset_data::scriptable::scriptable_event_set_mayhem_def read_scriptable_event_set_mayhem_def(const ScriptableEventSetMayhemDef& native);
		void write_scriptable_event_set_mayhem_def(ScriptableEventSetMayhemDef& native, const asset_data::scriptable::scriptable_event_set_mayhem_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_set_mayhem_def(const asset_data::scriptable::scriptable_event_set_mayhem_def& data);
		asset_data::scriptable::scriptable_event_play_mayhem_def read_scriptable_event_play_mayhem_def(const ScriptableEventPlayMayhemDef& native);
		void write_scriptable_event_play_mayhem_def(ScriptableEventPlayMayhemDef& native, const asset_data::scriptable::scriptable_event_play_mayhem_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_play_mayhem_def(const asset_data::scriptable::scriptable_event_play_mayhem_def& data);
		asset_data::scriptable::scriptable_event_viewmodel_shader_param_def read_scriptable_event_viewmodel_shader_param_def(const ScriptableEventViewmodelShaderParamDef& native);
		void write_scriptable_event_viewmodel_shader_param_def(ScriptableEventViewmodelShaderParamDef& native, const asset_data::scriptable::scriptable_event_viewmodel_shader_param_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_viewmodel_shader_param_def(const asset_data::scriptable::scriptable_event_viewmodel_shader_param_def& data);
		asset_data::scriptable::scriptable_event_viewmodel_change_image_def read_scriptable_event_viewmodel_change_image_def(const ScriptableEventViewmodelChangeImageDef& native);
		void write_scriptable_event_viewmodel_change_image_def(ScriptableEventViewmodelChangeImageDef& native, const asset_data::scriptable::scriptable_event_viewmodel_change_image_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_viewmodel_change_image_def(const asset_data::scriptable::scriptable_event_viewmodel_change_image_def& data);
		asset_data::scriptable::scriptable_event_client_view_selector_def read_scriptable_event_client_view_selector_def(const ScriptableEventClientViewSelectorDef& native);
		void write_scriptable_event_client_view_selector_def(ScriptableEventClientViewSelectorDef& native, const asset_data::scriptable::scriptable_event_client_view_selector_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_client_view_selector_def(const asset_data::scriptable::scriptable_event_client_view_selector_def& data);
		asset_data::scriptable::scriptable_event_team_selector_def read_scriptable_event_team_selector_def(const ScriptableEventTeamSelectorDef& native);
		void write_scriptable_event_team_selector_def(ScriptableEventTeamSelectorDef& native, const asset_data::scriptable::scriptable_event_team_selector_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_team_selector_def(const asset_data::scriptable::scriptable_event_team_selector_def& data);
		asset_data::scriptable::scriptable_event_add_model_def read_scriptable_event_add_model_def(const ScriptableEventAddModelDef& native);
		void write_scriptable_event_add_model_def(ScriptableEventAddModelDef& native, const asset_data::scriptable::scriptable_event_add_model_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_add_model_def(const asset_data::scriptable::scriptable_event_add_model_def& data);
		asset_data::scriptable::scriptable_event_apply_force_def read_scriptable_event_apply_force_def(const ScriptableEventApplyForceDef& native);
		void write_scriptable_event_apply_force_def(ScriptableEventApplyForceDef& native, const asset_data::scriptable::scriptable_event_apply_force_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_apply_force_def(const asset_data::scriptable::scriptable_event_apply_force_def& data);
		asset_data::scriptable::scriptable_event_apply_angular_force_def read_scriptable_event_apply_angular_force_def(const ScriptableEventApplyAngularForceDef& native);
		void write_scriptable_event_apply_angular_force_def(ScriptableEventApplyAngularForceDef& native, const asset_data::scriptable::scriptable_event_apply_angular_force_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_apply_angular_force_def(const asset_data::scriptable::scriptable_event_apply_angular_force_def& data);
		asset_data::scriptable::scriptable_event_apply_constant_force_def read_scriptable_event_apply_constant_force_def(const ScriptableEventApplyConstantForceDef& native);
		void write_scriptable_event_apply_constant_force_def(ScriptableEventApplyConstantForceDef& native, const asset_data::scriptable::scriptable_event_apply_constant_force_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_apply_constant_force_def(const asset_data::scriptable::scriptable_event_apply_constant_force_def& data);
		asset_data::scriptable::scriptable_event_apply_constant_angular_force_def read_scriptable_event_apply_constant_angular_force_def(const ScriptableEventApplyConstantAngularForceDef& native);
		void write_scriptable_event_apply_constant_angular_force_def(ScriptableEventApplyConstantAngularForceDef& native, const asset_data::scriptable::scriptable_event_apply_constant_angular_force_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_apply_constant_angular_force_def(const asset_data::scriptable::scriptable_event_apply_constant_angular_force_def& data);
		asset_data::scriptable::scriptable_event_compass_icon_def read_scriptable_event_compass_icon_def(const ScriptableEventCompassIconDef& native);
		void write_scriptable_event_compass_icon_def(ScriptableEventCompassIconDef& native, const asset_data::scriptable::scriptable_event_compass_icon_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_compass_icon_def(const asset_data::scriptable::scriptable_event_compass_icon_def& data);
		asset_data::scriptable::scriptable_event_material_override_def read_scriptable_event_material_override_def(const ScriptableEventMaterialOverrideDef& native);
		void write_scriptable_event_material_override_def(ScriptableEventMaterialOverrideDef& native, const asset_data::scriptable::scriptable_event_material_override_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_material_override_def(const asset_data::scriptable::scriptable_event_material_override_def& data);
		asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def read_scriptable_event_dynamic_bone_noise_curve_def(const ScriptableEventDynamicBoneNoiseCurveDef& native);
		void write_scriptable_event_dynamic_bone_noise_curve_def(ScriptableEventDynamicBoneNoiseCurveDef& native, const asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_dynamic_bone_noise_curve_def(const asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def& data);
		asset_data::scriptable::scriptable_event_move_def read_scriptable_event_move_def(const ScriptableEventMoveDef& native);
		void write_scriptable_event_move_def(ScriptableEventMoveDef& native, const asset_data::scriptable::scriptable_event_move_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_move_def(const asset_data::scriptable::scriptable_event_move_def& data);
		asset_data::scriptable::scriptable_event_footstep_def read_scriptable_event_footstep_def(const ScriptableEventFootstepDef& native);
		void write_scriptable_event_footstep_def(ScriptableEventFootstepDef& native, const asset_data::scriptable::scriptable_event_footstep_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_footstep_def(const asset_data::scriptable::scriptable_event_footstep_def& data);
		asset_data::scriptable::scriptable_event_gravity_arc_def read_scriptable_event_gravity_arc_def(const ScriptableEventGravityArcDef& native);
		void write_scriptable_event_gravity_arc_def(ScriptableEventGravityArcDef& native, const asset_data::scriptable::scriptable_event_gravity_arc_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_gravity_arc_def(const asset_data::scriptable::scriptable_event_gravity_arc_def& data);
		asset_data::scriptable::scriptable_event_view_trigger_def read_scriptable_event_view_trigger_def(const ScriptableEventViewTriggerDef& native);
		void write_scriptable_event_view_trigger_def(ScriptableEventViewTriggerDef& native, const asset_data::scriptable::scriptable_event_view_trigger_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_view_trigger_def(const asset_data::scriptable::scriptable_event_view_trigger_def& data);
		asset_data::scriptable::scriptable_event_objective_def read_scriptable_event_objective_def(const ScriptableEventObjectiveDef& native);
		void write_scriptable_event_objective_def(ScriptableEventObjectiveDef& native, const asset_data::scriptable::scriptable_event_objective_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_objective_def(const asset_data::scriptable::scriptable_event_objective_def& data);
		asset_data::scriptable::objective_settings read_objective_settings(const ObjectiveSettings& native);
		void write_objective_settings(ObjectiveSettings& native, const asset_data::scriptable::objective_settings& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_objective_settings(const asset_data::scriptable::objective_settings& data);
		asset_data::scriptable::scriptable_event_spatial_disable_def read_scriptable_event_spatial_disable_def(const ScriptableEventSpatialDisableDef& native);
		void write_scriptable_event_spatial_disable_def(ScriptableEventSpatialDisableDef& native, const asset_data::scriptable::scriptable_event_spatial_disable_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_spatial_disable_def(const asset_data::scriptable::scriptable_event_spatial_disable_def& data);
		asset_data::scriptable::scriptable_event_hover_def read_scriptable_event_hover_def(const ScriptableEventHoverDef& native);
		void write_scriptable_event_hover_def(ScriptableEventHoverDef& native, const asset_data::scriptable::scriptable_event_hover_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_hover_def(const asset_data::scriptable::scriptable_event_hover_def& data);
		asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def read_scriptable_event_vehicle_blow_up_tire_def(const ScriptableEventVehicleBlowUpTireDef& native);
		void write_scriptable_event_vehicle_blow_up_tire_def(ScriptableEventVehicleBlowUpTireDef& native, const asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_vehicle_blow_up_tire_def(const asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def& data);
		asset_data::scriptable::scriptable_event_dlc1_def read_scriptable_event_dlc1_def(const ScriptableEventDlc1Def& native);
		void write_scriptable_event_dlc1_def(ScriptableEventDlc1Def& native, const asset_data::scriptable::scriptable_event_dlc1_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_dlc1_def(const asset_data::scriptable::scriptable_event_dlc1_def& data);
		asset_data::scriptable::scriptable_event_script_damage_def read_scriptable_event_script_damage_def(const ScriptableEventScriptDamageDef& native);
		void write_scriptable_event_script_damage_def(ScriptableEventScriptDamageDef& native, const asset_data::scriptable::scriptable_event_script_damage_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_event_script_damage_def(const asset_data::scriptable::scriptable_event_script_damage_def& data);
		asset_data::scriptable::scriptable_state_simple_def read_scriptable_state_simple_def(const ScriptableStateSimpleDef& native);
		void write_scriptable_state_simple_def(ScriptableStateSimpleDef& native, const asset_data::scriptable::scriptable_state_simple_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_state_simple_def(const asset_data::scriptable::scriptable_state_simple_def& data);
		asset_data::scriptable::scriptable_state_health_def read_scriptable_state_health_def(const ScriptableStateHealthDef& native);
		void write_scriptable_state_health_def(ScriptableStateHealthDef& native, const asset_data::scriptable::scriptable_state_health_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_state_health_def(const asset_data::scriptable::scriptable_state_health_def& data);
		asset_data::scriptable::scriptable_state_scripted_def read_scriptable_state_scripted_def(const ScriptableStateScriptedDef& native);
		void write_scriptable_state_scripted_def(ScriptableStateScriptedDef& native, const asset_data::scriptable::scriptable_state_scripted_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_state_scripted_def(const asset_data::scriptable::scriptable_state_scripted_def& data);
		asset_data::scriptable::scriptable_state_usable_def read_scriptable_state_usable_def(const ScriptableStateUsableDef& native);
		void write_scriptable_state_usable_def(ScriptableStateUsableDef& native, const asset_data::scriptable::scriptable_state_usable_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_state_usable_def(const asset_data::scriptable::scriptable_state_usable_def& data);
		asset_data::scriptable::scriptable_damage_tag_override read_scriptable_damage_tag_override(const ScriptableDamageTagOverride& native);
		void write_scriptable_damage_tag_override(ScriptableDamageTagOverride& native, const asset_data::scriptable::scriptable_damage_tag_override& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scriptable_damage_tag_override(const asset_data::scriptable::scriptable_damage_tag_override& data);

		asset_data::scriptable::scriptable read_scriptable(const ScriptableDef& native)
		{
			plain::trace("read_scriptable");
			asset_data::scriptable::scriptable data;
			data.name = plain::read_string(native.name);
			data.next_scriptable_def = plain::read_asset(ASSET_TYPE_SCRIPTABLE, native.nextScriptableDef);
			data.flags = native.flags;
			data.parts = plain::read_array(native.parts, native.numParts, "ScriptableDef.parts", [&](const auto& item) { return read_scriptable_part_def(item); });
			data.max_num_dyn_ents_required = native.maxNumDynEntsRequired;
			data.part_count = native.partCount;
			data.server_instanced_part_count = native.serverInstancedPartCount;
			data.server_controlled_part_count = native.serverControlledPartCount;
			data.max_num_dyn_ent_parts_base = native.maxNumDynEntPartsBase;
			data.max_num_dyn_ent_parts_for_spawning = native.maxNumDynEntPartsForSpawning;
			data.event_stream_size_required_server = native.eventStreamSizeRequiredServer;
			data.event_stream_size_required_client = native.eventStreamSizeRequiredClient;
			data.usable_part_count = native.usablePartCount;
			data.ff_mem_cost = native.ffMemCost;
			data.animation_tree_name = plain::read_script_string(native.animationTreeName);
			plain::require_zero(native.animationTreeDef, "ScriptableDef.animationTreeDef");
			data.view_radius = native.viewRadius;
			data.network_lod_range_override = native.networkLODRangeOverride;
			data.network_lod_range_override_distance = native.networkLODRangeOverrideDistance;
			data.models = plain::read_array(native.models, native.numXModels, "ScriptableDef.models", [&](const auto& item) { return plain::read_asset(ASSET_TYPE_XMODEL, item); });
			data.spatial_activation_mode = plain::enum_name(asset_data::scriptable::dyn_entity_spatial_activation_mode_names, static_cast<std::int64_t>(native.spatialActivationMode), "ScriptableDef.spatialActivationMode");
			return data;
		}

		void write_scriptable(ScriptableDef& native, const asset_data::scriptable::scriptable& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.nextScriptableDef = plain::resolve_asset<ScriptableDef>(resolve, ASSET_TYPE_SCRIPTABLE, data.next_scriptable_def);
			native.flags = data.flags;
			native.parts = plain::write_array<ScriptablePartDef>(data.parts, memory, [&](auto& out, const auto& in) { write_scriptable_part_def(out, in, resolve, memory); });
			native.maxNumDynEntsRequired = data.max_num_dyn_ents_required;
			native.partCount = data.part_count;
			native.serverInstancedPartCount = data.server_instanced_part_count;
			native.serverControlledPartCount = data.server_controlled_part_count;
			native.maxNumDynEntPartsBase = data.max_num_dyn_ent_parts_base;
			native.maxNumDynEntPartsForSpawning = data.max_num_dyn_ent_parts_for_spawning;
			native.eventStreamSizeRequiredServer = data.event_stream_size_required_server;
			native.eventStreamSizeRequiredClient = data.event_stream_size_required_client;
			native.usablePartCount = data.usable_part_count;
			native.ffMemCost = data.ff_mem_cost;
			native.animationTreeName = static_cast<scr_string_t>(plain::write_script_string(data.animation_tree_name));
			native.viewRadius = data.view_radius;
			native.networkLODRangeOverride = data.network_lod_range_override;
			native.networkLODRangeOverrideDistance = data.network_lod_range_override_distance;
			native.models = plain::write_array<XModel*>(data.models, memory, [&](auto& out, const auto& in) { out = plain::resolve_asset<XModel>(resolve, ASSET_TYPE_XMODEL, in); });
			native.spatialActivationMode = static_cast<DynEntitySpatialActivationMode>(plain::enum_value(asset_data::scriptable::dyn_entity_spatial_activation_mode_names, data.spatial_activation_mode, "ScriptableDef.spatialActivationMode"));
			native.numParts = plain::count_of<std::uint32_t>(data.parts);
			native.numXModels = plain::count_of<std::uint32_t>(data.models);
		}

		std::size_t size_scriptable([[maybe_unused]] const asset_data::scriptable::scriptable& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.parts)
			{
				size += plain::sized(std::max<std::size_t>(data.parts->size(), 1) * sizeof(ScriptablePartDef));
				for (const auto& item : *data.parts)
				{
					size += size_scriptable_part_def(item);
				}
			}
			if (data.models)
			{
				size += plain::sized(std::max<std::size_t>(data.models->size(), 1) * sizeof(XModel*));
			}
			return size;
		}

		asset_data::scriptable::scriptable_part_def read_scriptable_part_def(const ScriptablePartDef& native)
		{
			plain::trace("read_scriptable_part_def");
			asset_data::scriptable::scriptable_part_def data;
			data.name = plain::read_string(native.name);
			data.scr_name = plain::read_script_string(native.scrName);
			data.flags = native.flags;
			data.flat_id = native.flatId;
			data.server_instance_flat_id = native.serverInstanceFlatId;
			data.server_controlled_flat_id = native.serverControlledFlatId;
			data.tag_name = plain::read_string(native.tagName);
			data.scr_tag_name = plain::read_script_string(native.scrTagName);
			data.states = plain::read_array(native.states, native.numStates, "ScriptablePartDef.states", [&](const auto& item) { return read_scriptable_state_def(item); });
			plain::require_zero(native.parentPart, "ScriptablePartDef.parentPart");
			data.child_parts = plain::read_array(native.childParts, native.numChildParts, "ScriptablePartDef.childParts", [&](const auto& item) { return read_scriptable_part_def(item); });
			data.damage_tag_overrides = plain::read_array(native.damageTagOverrides, native.numDamageTagOverrides, "ScriptablePartDef.damageTagOverrides", [&](const auto& item) { return read_scriptable_damage_tag_override(item); });
			data.event_stream_buffer_offset_server = native.eventStreamBufferOffsetServer;
			data.event_stream_buffer_offset_client = native.eventStreamBufferOffsetClient;
			return data;
		}

		void write_scriptable_part_def(ScriptablePartDef& native, const asset_data::scriptable::scriptable_part_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.scrName = static_cast<scr_string_t>(plain::write_script_string(data.scr_name));
			native.flags = data.flags;
			native.flatId = data.flat_id;
			native.serverInstanceFlatId = data.server_instance_flat_id;
			native.serverControlledFlatId = data.server_controlled_flat_id;
			native.tagName = plain::write_string(memory, data.tag_name);
			native.scrTagName = static_cast<scr_string_t>(plain::write_script_string(data.scr_tag_name));
			native.states = plain::write_array<ScriptableStateDef>(data.states, memory, [&](auto& out, const auto& in) { write_scriptable_state_def(out, in, resolve, memory); });
			native.childParts = plain::write_array<ScriptablePartDef>(data.child_parts, memory, [&](auto& out, const auto& in) { write_scriptable_part_def(out, in, resolve, memory); });
			native.damageTagOverrides = plain::write_array<ScriptableDamageTagOverride>(data.damage_tag_overrides, memory, [&](auto& out, const auto& in) { write_scriptable_damage_tag_override(out, in, resolve, memory); });
			native.eventStreamBufferOffsetServer = data.event_stream_buffer_offset_server;
			native.eventStreamBufferOffsetClient = data.event_stream_buffer_offset_client;
			native.numStates = plain::count_of<std::uint32_t>(data.states);
			native.numChildParts = plain::count_of<std::uint32_t>(data.child_parts);
			native.numDamageTagOverrides = plain::count_of<std::uint32_t>(data.damage_tag_overrides);
		}

		std::size_t size_scriptable_part_def([[maybe_unused]] const asset_data::scriptable::scriptable_part_def& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			size += plain::string_size(data.tag_name);
			if (data.states)
			{
				size += plain::sized(std::max<std::size_t>(data.states->size(), 1) * sizeof(ScriptableStateDef));
				for (const auto& item : *data.states)
				{
					size += size_scriptable_state_def(item);
				}
			}
			if (data.child_parts)
			{
				size += plain::sized(std::max<std::size_t>(data.child_parts->size(), 1) * sizeof(ScriptablePartDef));
				for (const auto& item : *data.child_parts)
				{
					size += size_scriptable_part_def(item);
				}
			}
			if (data.damage_tag_overrides)
			{
				size += plain::sized(std::max<std::size_t>(data.damage_tag_overrides->size(), 1) * sizeof(ScriptableDamageTagOverride));
				for (const auto& item : *data.damage_tag_overrides)
				{
					size += size_scriptable_damage_tag_override(item);
				}
			}
			return size;
		}

		asset_data::scriptable::scriptable_state_base_def read_scriptable_state_base_def(const ScriptableStateBaseDef& native)
		{
			plain::trace("read_scriptable_state_base_def");
			asset_data::scriptable::scriptable_state_base_def data;
			data.name = plain::read_string(native.name);
			data.flags = native.flags;
			data.events = plain::read_array(native.events, native.numEvents, "ScriptableStateBaseDef.events", [&](const auto& item) { return read_scriptable_event_def(item); });
			data.events_id = plain::pointer_ids::of(native.events);
			return data;
		}

		void write_scriptable_state_base_def(ScriptableStateBaseDef& native, const asset_data::scriptable::scriptable_state_base_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.flags = data.flags;
			native.events = plain::write_shared_array<ScriptableEventDef>(data.events_id, data.events, memory, [&](auto& out, const auto& in) { write_scriptable_event_def(out, in, resolve, memory); });
			native.numEvents = plain::count_of<std::uint32_t>(data.events);
		}

		std::size_t size_scriptable_state_base_def([[maybe_unused]] const asset_data::scriptable::scriptable_state_base_def& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.events)
			{
				size += plain::sized(std::max<std::size_t>(data.events->size(), 1) * sizeof(ScriptableEventDef));
				for (const auto& item : *data.events)
				{
					size += size_scriptable_event_def(item);
				}
			}
			return size;
		}

		asset_data::scriptable::scriptable_state_simple_def read_scriptable_state_simple_def(const ScriptableStateSimpleDef& native)
		{
			plain::trace("read_scriptable_state_simple_def");
			asset_data::scriptable::scriptable_state_simple_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_state_base_def(item); });
			return data;
		}

		void write_scriptable_state_simple_def(ScriptableStateSimpleDef& native, const asset_data::scriptable::scriptable_state_simple_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableStateBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_state_base_def(out, in, resolve, memory); });
		}

		std::size_t size_scriptable_state_simple_def([[maybe_unused]] const asset_data::scriptable::scriptable_state_simple_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableStateBaseDef));
				size += size_scriptable_state_base_def(*data.base);
			}
			return size;
		}

		asset_data::scriptable::scriptable_state_health_def read_scriptable_state_health_def(const ScriptableStateHealthDef& native)
		{
			plain::trace("read_scriptable_state_health_def");
			asset_data::scriptable::scriptable_state_health_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_state_base_def(item); });
			data.health = native.health;
			data.valid_damage_cause = plain::enum_name(asset_data::scriptable::scriptable_damage_cause_names, static_cast<std::int64_t>(native.validDamageCause), "ScriptableStateHealthDef.validDamageCause");
			data.valid_attackers = plain::enum_name(asset_data::scriptable::scriptable_attackers_names, static_cast<std::int64_t>(native.validAttackers), "ScriptableStateHealthDef.validAttackers");
			data.valid_attacker_teams = plain::enum_name(asset_data::scriptable::scriptable_team_filter_names, static_cast<std::int64_t>(native.validAttackerTeams), "ScriptableStateHealthDef.validAttackerTeams");
			data.weapon_classes = plain::read_array(native.weaponClasses, native.numWeaponClasses, "ScriptableStateHealthDef.weaponClasses", [&](const auto& item) { return static_cast<std::uint32_t>(item); });
			data.weapons = plain::read_array(native.weapons, native.numWeapons, "ScriptableStateHealthDef.weapons", [&](const auto& item) { return plain::read_asset(ASSET_TYPE_WEAPON, item); });
			data.minimum_damage = native.minimumDamage;
			data.damage_propagation_to_parent = native.damagePropagationToParent;
			data.damage_propagation_to_child = native.damagePropagationToChild;
			data.damage_propagation_from_parent = native.damagePropagationFromParent;
			data.damage_propagation_from_child = native.damagePropagationFromChild;
			data.script_id = plain::read_string(native.script_id);
			data.scr_script_id = plain::read_script_string(native.scrScript_id);
			data.localize_splash_damage = native.localizeSplashDamage;
			return data;
		}

		void write_scriptable_state_health_def(ScriptableStateHealthDef& native, const asset_data::scriptable::scriptable_state_health_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableStateBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_state_base_def(out, in, resolve, memory); });
			native.health = data.health;
			native.validDamageCause = static_cast<Scriptable_DamageCause>(plain::enum_value(asset_data::scriptable::scriptable_damage_cause_names, data.valid_damage_cause, "ScriptableStateHealthDef.validDamageCause"));
			native.validAttackers = static_cast<Scriptable_Attackers>(plain::enum_value(asset_data::scriptable::scriptable_attackers_names, data.valid_attackers, "ScriptableStateHealthDef.validAttackers"));
			native.validAttackerTeams = static_cast<Scriptable_TeamFilter>(plain::enum_value(asset_data::scriptable::scriptable_team_filter_names, data.valid_attacker_teams, "ScriptableStateHealthDef.validAttackerTeams"));
			native.weaponClasses = plain::write_array<weapClass_t>(data.weapon_classes, memory, [&](auto& out, const auto& in) { out = static_cast<weapClass_t>(in); });
			native.weapons = plain::write_array<WeaponCompleteDef*>(data.weapons, memory, [&](auto& out, const auto& in) { out = plain::resolve_asset<WeaponCompleteDef>(resolve, ASSET_TYPE_WEAPON, in); });
			native.minimumDamage = data.minimum_damage;
			native.damagePropagationToParent = data.damage_propagation_to_parent;
			native.damagePropagationToChild = data.damage_propagation_to_child;
			native.damagePropagationFromParent = data.damage_propagation_from_parent;
			native.damagePropagationFromChild = data.damage_propagation_from_child;
			native.script_id = plain::write_string(memory, data.script_id);
			native.scrScript_id = static_cast<scr_string_t>(plain::write_script_string(data.scr_script_id));
			native.localizeSplashDamage = data.localize_splash_damage;
			native.numWeaponClasses = plain::count_of<std::uint32_t>(data.weapon_classes);
			native.numWeapons = plain::count_of<std::uint32_t>(data.weapons);
		}

		std::size_t size_scriptable_state_health_def([[maybe_unused]] const asset_data::scriptable::scriptable_state_health_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableStateBaseDef));
				size += size_scriptable_state_base_def(*data.base);
			}
			if (data.weapon_classes)
			{
				size += plain::sized(std::max<std::size_t>(data.weapon_classes->size(), 1) * sizeof(weapClass_t));
			}
			if (data.weapons)
			{
				size += plain::sized(std::max<std::size_t>(data.weapons->size(), 1) * sizeof(WeaponCompleteDef*));
			}
			size += plain::string_size(data.script_id);
			return size;
		}

		asset_data::scriptable::scriptable_state_scripted_def read_scriptable_state_scripted_def(const ScriptableStateScriptedDef& native)
		{
			plain::trace("read_scriptable_state_scripted_def");
			asset_data::scriptable::scriptable_state_scripted_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_state_base_def(item); });
			data.script_id = plain::read_string(native.script_id);
			data.scr_script_id = plain::read_script_string(native.scrScript_id);
			return data;
		}

		void write_scriptable_state_scripted_def(ScriptableStateScriptedDef& native, const asset_data::scriptable::scriptable_state_scripted_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableStateBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_state_base_def(out, in, resolve, memory); });
			native.script_id = plain::write_string(memory, data.script_id);
			native.scrScript_id = static_cast<scr_string_t>(plain::write_script_string(data.scr_script_id));
		}

		std::size_t size_scriptable_state_scripted_def([[maybe_unused]] const asset_data::scriptable::scriptable_state_scripted_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableStateBaseDef));
				size += size_scriptable_state_base_def(*data.base);
			}
			size += plain::string_size(data.script_id);
			return size;
		}

		asset_data::scriptable::scriptable_state_usable_def read_scriptable_state_usable_def(const ScriptableStateUsableDef& native)
		{
			plain::trace("read_scriptable_state_usable_def");
			asset_data::scriptable::scriptable_state_usable_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_state_base_def(item); });
			data.script_id = plain::read_string(native.script_id);
			data.scr_script_id = plain::read_script_string(native.scrScript_id);
			data.face_fov = native.faceFOV;
			data.use_fov = native.useFOV;
			data.use_radius = native.useRadius;
			data.use_radius_auto = native.useRadiusAuto;
			data.use_radius_display = native.useRadiusDisplay;
			data.use_radius_minimized = native.useRadiusMinimized;
			data.goto_next_state_on_use = native.gotoNextStateOnUse;
			data.allow_missing_tag = native.allowMissingTag;
			data.usable_when_obstructed = native.usableWhenObstructed;
			data.use_offset = plain::read_scalars<float, 3>(&native.useOffset);
			data.angle_offset = plain::read_scalars<float, 3>(&native.angleOffset);
			data.hint_hold_duration = native.hintHoldDuration;
			data.priority = native.priority;
			data.use2_dfov = native.use2DFOV;
			data.hint_binding = native.hintBinding;
			data.hint_tag = plain::read_script_string(native.hintTag);
			data.hint_string = plain::read_string(native.hintString);
			data.hint_string_disabled = plain::read_string(native.hintStringDisabled);
			data.hint_icon = plain::read_string(native.hintIcon);
			data.hint_icon_image = plain::read_asset(ASSET_TYPE_IMAGE, native.hintIconImage);
			data.hint_string_index = native.hintStringIndex;
			data.hint_string_disabled_index = native.hintStringDisabledIndex;
			data.hint_icon_index = native.hintIconIndex;
			return data;
		}

		void write_scriptable_state_usable_def(ScriptableStateUsableDef& native, const asset_data::scriptable::scriptable_state_usable_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableStateBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_state_base_def(out, in, resolve, memory); });
			native.script_id = plain::write_string(memory, data.script_id);
			native.scrScript_id = static_cast<scr_string_t>(plain::write_script_string(data.scr_script_id));
			native.faceFOV = data.face_fov;
			native.useFOV = data.use_fov;
			native.useRadius = data.use_radius;
			native.useRadiusAuto = data.use_radius_auto;
			native.useRadiusDisplay = data.use_radius_display;
			native.useRadiusMinimized = data.use_radius_minimized;
			native.gotoNextStateOnUse = data.goto_next_state_on_use;
			native.allowMissingTag = data.allow_missing_tag;
			native.usableWhenObstructed = data.usable_when_obstructed;
			plain::write_scalars(&native.useOffset, data.use_offset);
			plain::write_scalars(&native.angleOffset, data.angle_offset);
			native.hintHoldDuration = data.hint_hold_duration;
			native.priority = data.priority;
			native.use2DFOV = data.use2_dfov;
			native.hintBinding = data.hint_binding;
			native.hintTag = static_cast<scr_string_t>(plain::write_script_string(data.hint_tag));
			native.hintString = plain::write_string(memory, data.hint_string);
			native.hintStringDisabled = plain::write_string(memory, data.hint_string_disabled);
			native.hintIcon = plain::write_string(memory, data.hint_icon);
			native.hintIconImage = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, data.hint_icon_image);
			native.hintStringIndex = data.hint_string_index;
			native.hintStringDisabledIndex = data.hint_string_disabled_index;
			native.hintIconIndex = data.hint_icon_index;
		}

		std::size_t size_scriptable_state_usable_def([[maybe_unused]] const asset_data::scriptable::scriptable_state_usable_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableStateBaseDef));
				size += size_scriptable_state_base_def(*data.base);
			}
			size += plain::string_size(data.script_id);
			size += plain::string_size(data.hint_string);
			size += plain::string_size(data.hint_string_disabled);
			size += plain::string_size(data.hint_icon);
			return size;
		}

		asset_data::scriptable::scriptable_state_def read_scriptable_state_def(const ScriptableStateDef& native)
		{
			plain::trace("read_scriptable_state_def");
			asset_data::scriptable::scriptable_state_def data;
			data.base = read_scriptable_state_base_def(native.base);
			data.data = [&]() -> std::variant<std::monostate, asset_data::scriptable::scriptable_state_simple_def, asset_data::scriptable::scriptable_state_health_def, asset_data::scriptable::scriptable_state_scripted_def, asset_data::scriptable::scriptable_state_usable_def> { switch (static_cast<std::uint32_t>(native.type)) { case 0: { plain::module_scope scope(0); auto item = read_scriptable_state_simple_def(native.data.simple); if (native.data.simple.base == &native.base) { item.base.reset(); item.base_is_state_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_state_simple_def, asset_data::scriptable::scriptable_state_health_def, asset_data::scriptable::scriptable_state_scripted_def, asset_data::scriptable::scriptable_state_usable_def>(std::move(item)); } case 1: { plain::module_scope scope(1); auto item = read_scriptable_state_health_def(native.data.health); if (native.data.health.base == &native.base) { item.base.reset(); item.base_is_state_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_state_simple_def, asset_data::scriptable::scriptable_state_health_def, asset_data::scriptable::scriptable_state_scripted_def, asset_data::scriptable::scriptable_state_usable_def>(std::move(item)); } case 2: { plain::module_scope scope(2); auto item = read_scriptable_state_scripted_def(native.data.scripted); if (native.data.scripted.base == &native.base) { item.base.reset(); item.base_is_state_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_state_simple_def, asset_data::scriptable::scriptable_state_health_def, asset_data::scriptable::scriptable_state_scripted_def, asset_data::scriptable::scriptable_state_usable_def>(std::move(item)); } case 3: { plain::module_scope scope(3); auto item = read_scriptable_state_usable_def(native.data.usable); if (native.data.usable.base == &native.base) { item.base.reset(); item.base_is_state_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_state_simple_def, asset_data::scriptable::scriptable_state_health_def, asset_data::scriptable::scriptable_state_scripted_def, asset_data::scriptable::scriptable_state_usable_def>(std::move(item)); } default: plain::fail("ScriptableStateDef.data", "has a tag value without a case"); } }();
			return data;
		}

		void write_scriptable_state_def(ScriptableStateDef& native, const asset_data::scriptable::scriptable_state_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_scriptable_state_base_def(native.base, data.base, resolve, memory);
			std::visit([&](const auto& value)
			{
				using value_type = std::decay_t<decltype(value)>;
				if constexpr (std::is_same_v<value_type, std::monostate>)
				{
					plain::fail("ScriptableStateDef.data", "holds no module");
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_state_simple_def>)
				{
					native.type = static_cast<Scriptable_StateType>(0);
					plain::module_scope scope(0);
					write_scriptable_state_simple_def(native.data.simple, value, resolve, memory);
					if (value.base_is_state_base)
					{
						native.data.simple.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_state_health_def>)
				{
					native.type = static_cast<Scriptable_StateType>(1);
					plain::module_scope scope(1);
					write_scriptable_state_health_def(native.data.health, value, resolve, memory);
					if (value.base_is_state_base)
					{
						native.data.health.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_state_scripted_def>)
				{
					native.type = static_cast<Scriptable_StateType>(2);
					plain::module_scope scope(2);
					write_scriptable_state_scripted_def(native.data.scripted, value, resolve, memory);
					if (value.base_is_state_base)
					{
						native.data.scripted.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_state_usable_def>)
				{
					native.type = static_cast<Scriptable_StateType>(3);
					plain::module_scope scope(3);
					write_scriptable_state_usable_def(native.data.usable, value, resolve, memory);
					if (value.base_is_state_base)
					{
						native.data.usable.base = &native.base;
					}
				}
			}, data.data);
		}

		std::size_t size_scriptable_state_def([[maybe_unused]] const asset_data::scriptable::scriptable_state_def& data)
		{
			std::size_t size = 0;
			size += size_scriptable_state_base_def(data.base);
			std::visit([&](const auto& value)
			{
				using value_type = std::decay_t<decltype(value)>;
				if constexpr (std::is_same_v<value_type, std::monostate>)
				{
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_state_simple_def>)
				{
					size += size_scriptable_state_simple_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_state_health_def>)
				{
					size += size_scriptable_state_health_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_state_scripted_def>)
				{
					size += size_scriptable_state_scripted_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_state_usable_def>)
				{
					size += size_scriptable_state_usable_def(value);
				}
			}, data.data);
			return size;
		}

		asset_data::scriptable::scriptable_event_base_def read_scriptable_event_base_def(const ScriptableEventBaseDef& native)
		{
			plain::trace("read_scriptable_event_base_def");
			asset_data::scriptable::scriptable_event_base_def data;
			data.name = plain::read_string(native.name);
			data.flags = native.flags;
			return data;
		}

		void write_scriptable_event_base_def(ScriptableEventBaseDef& native, const asset_data::scriptable::scriptable_event_base_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.flags = data.flags;
		}

		std::size_t size_scriptable_event_base_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_base_def& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			return size;
		}

		asset_data::scriptable::scriptable_event_state_change_def read_scriptable_event_state_change_def(const ScriptableEventStateChangeDef& native)
		{
			plain::trace("read_scriptable_event_state_change_def");
			asset_data::scriptable::scriptable_event_state_change_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.part_reference = plain::read_scalars<std::uint32_t, 1>(&native.partReference)[0];
			data.state_idx = native.stateIdx;
			plain::require_zero(native.part, "ScriptableEventStateChangeDef.part");
			return data;
		}

		void write_scriptable_event_state_change_def(ScriptableEventStateChangeDef& native, const asset_data::scriptable::scriptable_event_state_change_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			plain::write_scalars(&native.partReference, std::array<std::uint32_t, 1>{data.part_reference});
			native.stateIdx = data.state_idx;
		}

		std::size_t size_scriptable_event_state_change_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_state_change_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			return size;
		}

		asset_data::scriptable::scriptable_event_wait_def read_scriptable_event_wait_def(const ScriptableEventWaitDef& native)
		{
			plain::trace("read_scriptable_event_wait_def");
			asset_data::scriptable::scriptable_event_wait_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.delay_min = native.delayMin;
			data.delay_max = native.delayMax;
			data.event_stream_buffer_offset_server = native.eventStreamBufferOffsetServer;
			data.event_stream_buffer_offset_client = native.eventStreamBufferOffsetClient;
			return data;
		}

		void write_scriptable_event_wait_def(ScriptableEventWaitDef& native, const asset_data::scriptable::scriptable_event_wait_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			native.delayMin = data.delay_min;
			native.delayMax = data.delay_max;
			native.eventStreamBufferOffsetServer = data.event_stream_buffer_offset_server;
			native.eventStreamBufferOffsetClient = data.event_stream_buffer_offset_client;
		}

		std::size_t size_scriptable_event_wait_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_wait_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			return size;
		}

		asset_data::scriptable::scriptable_event_random_def read_scriptable_event_random_def(const ScriptableEventRandomDef& native)
		{
			plain::trace("read_scriptable_event_random_def");
			asset_data::scriptable::scriptable_event_random_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.threshold = native.threshold;
			data.events_a = plain::read_array(native.eventsA, native.eventACount, "ScriptableEventRandomDef.eventsA", [&](const auto& item) { return read_scriptable_event_def(item); });
			data.events_b = plain::read_array(native.eventsB, native.eventBCount, "ScriptableEventRandomDef.eventsB", [&](const auto& item) { return read_scriptable_event_def(item); });
			data.event_stream_buffer_offset_server = native.eventStreamBufferOffsetServer;
			data.event_stream_buffer_offset_client = native.eventStreamBufferOffsetClient;
			return data;
		}

		void write_scriptable_event_random_def(ScriptableEventRandomDef& native, const asset_data::scriptable::scriptable_event_random_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			native.threshold = data.threshold;
			native.eventsA = plain::write_array<ScriptableEventDef>(data.events_a, memory, [&](auto& out, const auto& in) { write_scriptable_event_def(out, in, resolve, memory); });
			native.eventsB = plain::write_array<ScriptableEventDef>(data.events_b, memory, [&](auto& out, const auto& in) { write_scriptable_event_def(out, in, resolve, memory); });
			native.eventStreamBufferOffsetServer = data.event_stream_buffer_offset_server;
			native.eventStreamBufferOffsetClient = data.event_stream_buffer_offset_client;
			native.eventACount = plain::count_of<std::uint32_t>(data.events_a);
			native.eventBCount = plain::count_of<std::uint32_t>(data.events_b);
		}

		std::size_t size_scriptable_event_random_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_random_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			if (data.events_a)
			{
				size += plain::sized(std::max<std::size_t>(data.events_a->size(), 1) * sizeof(ScriptableEventDef));
				for (const auto& item : *data.events_a)
				{
					size += size_scriptable_event_def(item);
				}
			}
			if (data.events_b)
			{
				size += plain::sized(std::max<std::size_t>(data.events_b->size(), 1) * sizeof(ScriptableEventDef));
				for (const auto& item : *data.events_b)
				{
					size += size_scriptable_event_def(item);
				}
			}
			return size;
		}

		asset_data::scriptable::scriptable_event_script_def read_scriptable_event_script_def(const ScriptableEventScriptDef& native)
		{
			plain::trace("read_scriptable_event_script_def");
			asset_data::scriptable::scriptable_event_script_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.notification = plain::read_string(native.notification);
			data.scr_notification = plain::read_script_string(native.scrNotification);
			data.param = native.param;
			data.do_notify_callback = native.doNotifyCallback;
			return data;
		}

		void write_scriptable_event_script_def(ScriptableEventScriptDef& native, const asset_data::scriptable::scriptable_event_script_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			native.notification = plain::write_string(memory, data.notification);
			native.scrNotification = static_cast<scr_string_t>(plain::write_script_string(data.scr_notification));
			native.param = data.param;
			native.doNotifyCallback = data.do_notify_callback;
		}

		std::size_t size_scriptable_event_script_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_script_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			size += plain::string_size(data.notification);
			return size;
		}

		asset_data::scriptable::scriptable_event_model_def_model read_scriptable_event_model_def_model(const ScriptableModelUnion& native)
		{
			plain::trace("read_scriptable_event_model_def_model");
			asset_data::scriptable::scriptable_event_model_def_model data;
			data.model = plain::read_asset(ASSET_TYPE_XMODEL, native.model);
			return data;
		}

		void write_scriptable_event_model_def_model(ScriptableModelUnion& native, const asset_data::scriptable::scriptable_event_model_def_model& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.model = plain::resolve_asset<XModel>(resolve, ASSET_TYPE_XMODEL, data.model);
		}

		std::size_t size_scriptable_event_model_def_model([[maybe_unused]] const asset_data::scriptable::scriptable_event_model_def_model& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::scriptable::scriptable_event_model_def_composite_model read_scriptable_event_model_def_composite_model(const ScriptableModelUnion& native)
		{
			plain::trace("read_scriptable_event_model_def_composite_model");
			asset_data::scriptable::scriptable_event_model_def_composite_model data;
			data.composite_model = plain::read_asset(ASSET_TYPE_XCOMPOSITEMODEL, native.compositeModel);
			return data;
		}

		void write_scriptable_event_model_def_composite_model(ScriptableModelUnion& native, const asset_data::scriptable::scriptable_event_model_def_composite_model& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.compositeModel = plain::resolve_asset<XCompositeModelDef>(resolve, ASSET_TYPE_XCOMPOSITEMODEL, data.composite_model);
		}

		std::size_t size_scriptable_event_model_def_composite_model([[maybe_unused]] const asset_data::scriptable::scriptable_event_model_def_composite_model& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::scriptable::scriptable_event_model_def read_scriptable_event_model_def(const ScriptableEventModelDef& native)
		{
			plain::trace("read_scriptable_event_model_def");
			asset_data::scriptable::scriptable_event_model_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.data = [&]() -> std::variant<std::monostate, asset_data::scriptable::scriptable_event_model_def_model, asset_data::scriptable::scriptable_event_model_def_composite_model> { switch (static_cast<std::uint32_t>(native.dataType)) { case 1: { plain::module_scope scope(1); auto item = read_scriptable_event_model_def_model(native.data); return std::variant<std::monostate, asset_data::scriptable::scriptable_event_model_def_model, asset_data::scriptable::scriptable_event_model_def_composite_model>(std::move(item)); } case 2: { plain::module_scope scope(2); auto item = read_scriptable_event_model_def_composite_model(native.data); return std::variant<std::monostate, asset_data::scriptable::scriptable_event_model_def_model, asset_data::scriptable::scriptable_event_model_def_composite_model>(std::move(item)); } default: plain::fail("ScriptableEventModelDef.data", "has a tag value without a case"); } }();
			data.hud_outline_color = native.hudOutlineColor;
			data.hud_outline_active = native.hudOutlineActive;
			data.hud_outline_fill = native.hudOutlineFill;
			data.never_moves = native.neverMoves;
			data.dynamic_simulation = native.dynamicSimulation;
			data.activate_physics = native.activatePhysics;
			return data;
		}

		void write_scriptable_event_model_def(ScriptableEventModelDef& native, const asset_data::scriptable::scriptable_event_model_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			std::visit([&](const auto& value)
			{
				using value_type = std::decay_t<decltype(value)>;
				if constexpr (std::is_same_v<value_type, std::monostate>)
				{
					plain::fail("ScriptableEventModelDef.data", "holds no module");
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_model_def_model>)
				{
					native.dataType = static_cast<ScriptableDataType>(1);
					plain::module_scope scope(1);
					write_scriptable_event_model_def_model(native.data, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_model_def_composite_model>)
				{
					native.dataType = static_cast<ScriptableDataType>(2);
					plain::module_scope scope(2);
					write_scriptable_event_model_def_composite_model(native.data, value, resolve, memory);
				}
			}, data.data);
			native.hudOutlineColor = data.hud_outline_color;
			native.hudOutlineActive = data.hud_outline_active;
			native.hudOutlineFill = data.hud_outline_fill;
			native.neverMoves = data.never_moves;
			native.dynamicSimulation = data.dynamic_simulation;
			native.activatePhysics = data.activate_physics;
		}

		std::size_t size_scriptable_event_model_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_model_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			std::visit([&](const auto& value)
			{
				using value_type = std::decay_t<decltype(value)>;
				if constexpr (std::is_same_v<value_type, std::monostate>)
				{
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_model_def_model>)
				{
					size += size_scriptable_event_model_def_model(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_model_def_composite_model>)
				{
					size += size_scriptable_event_model_def_composite_model(value);
				}
			}, data.data);
			return size;
		}

		asset_data::scriptable::scriptable_event_collision_def read_scriptable_event_collision_def(const ScriptableEventCollisionDef& native)
		{
			plain::trace("read_scriptable_event_collision_def");
			asset_data::scriptable::scriptable_event_collision_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.collmap_name = plain::read_string(native.collmapName);
			data.ai_obstacle = native.aiObstacle;
			data.clipmap_c_model_index = native.clipmapCModelIndex;
			data.never_moves = native.neverMoves;
			data.can_push = native.canPush;
			data.can_touch = native.canTouch;
			data.destroy_on_exit = native.destroyOnExit;
			data.model = plain::read_asset(ASSET_TYPE_XMODEL, native.model);
			return data;
		}

		void write_scriptable_event_collision_def(ScriptableEventCollisionDef& native, const asset_data::scriptable::scriptable_event_collision_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			native.collmapName = plain::write_string(memory, data.collmap_name);
			native.aiObstacle = data.ai_obstacle;
			native.clipmapCModelIndex = data.clipmap_c_model_index;
			native.neverMoves = data.never_moves;
			native.canPush = data.can_push;
			native.canTouch = data.can_touch;
			native.destroyOnExit = data.destroy_on_exit;
			native.model = plain::resolve_asset<XModel>(resolve, ASSET_TYPE_XMODEL, data.model);
		}

		std::size_t size_scriptable_event_collision_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_collision_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			size += plain::string_size(data.collmap_name);
			return size;
		}

		asset_data::scriptable::scr_anim_t read_scr_anim_t(const scr_anim_t& native)
		{
			plain::trace("read_scr_anim_t");
			asset_data::scriptable::scr_anim_t data;
			data.link_pointer_or_s0 = plain::read_scalars<std::uint64_t, 1>(&native.___u0)[0];
			return data;
		}

		void write_scr_anim_t(scr_anim_t& native, const asset_data::scriptable::scr_anim_t& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.___u0, std::array<std::uint64_t, 1>{data.link_pointer_or_s0});
		}

		std::size_t size_scr_anim_t([[maybe_unused]] const asset_data::scriptable::scr_anim_t& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::scriptable::scriptable_event_animation_def read_scriptable_event_animation_def(const ScriptableEventAnimationDef& native)
		{
			plain::trace("read_scriptable_event_animation_def");
			asset_data::scriptable::scriptable_event_animation_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.animation = plain::read_asset(ASSET_TYPE_XANIMPARTS, native.animation);
			data.stateful = native.stateful;
			data.override_animation = native.overrideAnimation;
			data.pause_on_exit = native.pauseOnExit;
			data.client_only_in_mp = native.clientOnlyInMP;
			data.start_time_min = native.startTimeMin;
			data.start_time_max = native.startTimeMax;
			data.start_time_normalized = native.startTimeNormalized;
			data.playback_rate_min = native.playbackRateMin;
			data.playback_rate_max = native.playbackRateMax;
			data.blend_time = native.blendTime;
			data.events_at_end = plain::read_array(native.eventsAtEnd, native.eventAtEndCount, "ScriptableEventAnimationDef.eventsAtEnd", [&](const auto& item) { return read_scriptable_event_def(item); });
			data.event_stream_buffer_offset_client = native.eventStreamBufferOffsetClient;
			data.event_stream_buffer_offset_server = native.eventStreamBufferOffsetServer;
			data.animation_index = [&] { std::array<asset_data::scriptable::scr_anim_t, 2> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_scr_anim_t(native.animationIndex[i]); } return out; }();
			return data;
		}

		void write_scriptable_event_animation_def(ScriptableEventAnimationDef& native, const asset_data::scriptable::scriptable_event_animation_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			native.animation = plain::resolve_asset<XAnimParts>(resolve, ASSET_TYPE_XANIMPARTS, data.animation);
			native.stateful = data.stateful;
			native.overrideAnimation = data.override_animation;
			native.pauseOnExit = data.pause_on_exit;
			native.clientOnlyInMP = data.client_only_in_mp;
			native.startTimeMin = data.start_time_min;
			native.startTimeMax = data.start_time_max;
			native.startTimeNormalized = data.start_time_normalized;
			native.playbackRateMin = data.playback_rate_min;
			native.playbackRateMax = data.playback_rate_max;
			native.blendTime = data.blend_time;
			native.eventsAtEnd = plain::write_array<ScriptableEventDef>(data.events_at_end, memory, [&](auto& out, const auto& in) { write_scriptable_event_def(out, in, resolve, memory); });
			native.eventStreamBufferOffsetClient = data.event_stream_buffer_offset_client;
			native.eventStreamBufferOffsetServer = data.event_stream_buffer_offset_server;
			for (std::size_t i = 0; i < data.animation_index.size(); ++i)
			{
				write_scr_anim_t(native.animationIndex[i], data.animation_index[i], resolve, memory);
			}
			native.eventAtEndCount = plain::count_of<std::uint32_t>(data.events_at_end);
		}

		std::size_t size_scriptable_event_animation_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_animation_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			if (data.events_at_end)
			{
				size += plain::sized(std::max<std::size_t>(data.events_at_end->size(), 1) * sizeof(ScriptableEventDef));
				for (const auto& item : *data.events_at_end)
				{
					size += size_scriptable_event_def(item);
				}
			}
			for (const auto& item : data.animation_index)
			{
				size += size_scr_anim_t(item);
			}
			return size;
		}

		asset_data::scriptable::scriptable_event_hide_show_bone_def read_scriptable_event_hide_show_bone_def(const ScriptableEventHideShowBoneDef& native)
		{
			plain::trace("read_scriptable_event_hide_show_bone_def");
			asset_data::scriptable::scriptable_event_hide_show_bone_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.tag_name = plain::read_string(native.tagName);
			data.scr_tag_name = plain::read_script_string(native.scrTagName);
			data.hide = native.hide;
			data.allow_missing_tag = native.allowMissingTag;
			data.hide_show_children = native.hideShowChildren;
			return data;
		}

		void write_scriptable_event_hide_show_bone_def(ScriptableEventHideShowBoneDef& native, const asset_data::scriptable::scriptable_event_hide_show_bone_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			native.tagName = plain::write_string(memory, data.tag_name);
			native.scrTagName = static_cast<scr_string_t>(plain::write_script_string(data.scr_tag_name));
			native.hide = data.hide;
			native.allowMissingTag = data.allow_missing_tag;
			native.hideShowChildren = data.hide_show_children;
		}

		std::size_t size_scriptable_event_hide_show_bone_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_hide_show_bone_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			size += plain::string_size(data.tag_name);
			return size;
		}

		asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def read_scriptable_event_disable_physics_sub_shape_def(const ScriptableEventDisablePhysicsSubShapeDef& native)
		{
			plain::trace("read_scriptable_event_disable_physics_sub_shape_def");
			asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.mutable_shape_name = plain::read_string(native.mutableShapeName);
			data.mutable_shape_hash = native.mutableShapeHash;
			data.allow_missing_shape = native.allowMissingShape;
			return data;
		}

		void write_scriptable_event_disable_physics_sub_shape_def(ScriptableEventDisablePhysicsSubShapeDef& native, const asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			native.mutableShapeName = plain::write_string(memory, data.mutable_shape_name);
			native.mutableShapeHash = data.mutable_shape_hash;
			native.allowMissingShape = data.allow_missing_shape;
		}

		std::size_t size_scriptable_event_disable_physics_sub_shape_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			size += plain::string_size(data.mutable_shape_name);
			return size;
		}

		asset_data::scriptable::scriptable_event_note_track_def read_scriptable_event_note_track_def(const ScriptableEventNoteTrackDef& native)
		{
			plain::trace("read_scriptable_event_note_track_def");
			asset_data::scriptable::scriptable_event_note_track_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.note_tracks = plain::read_array(native.noteTracks, native.noteTrackCount, "ScriptableEventNoteTrackDef.noteTracks", [&](const auto& item) { return read_scriptable_note_track_def(item); });
			return data;
		}

		void write_scriptable_event_note_track_def(ScriptableEventNoteTrackDef& native, const asset_data::scriptable::scriptable_event_note_track_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			native.noteTracks = plain::write_array<ScriptableNoteTrackDef>(data.note_tracks, memory, [&](auto& out, const auto& in) { write_scriptable_note_track_def(out, in, resolve, memory); });
			native.noteTrackCount = plain::count_of<std::uint32_t>(data.note_tracks);
		}

		std::size_t size_scriptable_event_note_track_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_note_track_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			if (data.note_tracks)
			{
				size += plain::sized(std::max<std::size_t>(data.note_tracks->size(), 1) * sizeof(ScriptableNoteTrackDef));
				for (const auto& item : *data.note_tracks)
				{
					size += size_scriptable_note_track_def(item);
				}
			}
			return size;
		}

		asset_data::scriptable::scriptable_event_chunk_dynent_def read_scriptable_event_chunk_dynent_def(const ScriptableEventChunkDynentDef& native)
		{
			plain::trace("read_scriptable_event_chunk_dynent_def");
			asset_data::scriptable::scriptable_event_chunk_dynent_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.part_reference = plain::read_scalars<std::uint32_t, 1>(&native.partReference)[0];
			data.stateful = native.stateful;
			data.allow_cause_velocity = native.allowCauseVelocity;
			data.launch_lin_vel = plain::read_scalars<float, 3>(&native.launchLinVel);
			data.launch_ang_vel = plain::read_scalars<float, 3>(&native.launchAngVel);
			plain::require_zero(native.part, "ScriptableEventChunkDynentDef.part");
			return data;
		}

		void write_scriptable_event_chunk_dynent_def(ScriptableEventChunkDynentDef& native, const asset_data::scriptable::scriptable_event_chunk_dynent_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			plain::write_scalars(&native.partReference, std::array<std::uint32_t, 1>{data.part_reference});
			native.stateful = data.stateful;
			native.allowCauseVelocity = data.allow_cause_velocity;
			plain::write_scalars(&native.launchLinVel, data.launch_lin_vel);
			plain::write_scalars(&native.launchAngVel, data.launch_ang_vel);
		}

		std::size_t size_scriptable_event_chunk_dynent_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_chunk_dynent_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			return size;
		}

		asset_data::scriptable::fx_combined_def read_fx_combined_def(const FxCombinedDef& native)
		{
			plain::trace("read_fx_combined_def");
			asset_data::scriptable::fx_combined_def data;
			data.particle_system_def = plain::read_asset(ASSET_TYPE_VFX, native.particleSystemDef);
			return data;
		}

		void write_fx_combined_def(FxCombinedDef& native, const asset_data::scriptable::fx_combined_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.particleSystemDef = plain::resolve_asset<ParticleSystemDef>(resolve, ASSET_TYPE_VFX, data.particle_system_def);
		}

		std::size_t size_fx_combined_def([[maybe_unused]] const asset_data::scriptable::fx_combined_def& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::scriptable::scriptable_event_spawn_dynent_def read_scriptable_event_spawn_dynent_def(const ScriptableEventSpawnDynentDef& native)
		{
			plain::trace("read_scriptable_event_spawn_dynent_def");
			asset_data::scriptable::scriptable_event_spawn_dynent_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.stateful = native.stateful;
			data.model = plain::read_asset(ASSET_TYPE_XMODEL, native.model);
			data.tag_name = plain::read_string(native.tagName);
			data.scr_tag_name = plain::read_script_string(native.scrTagName);
			data.allow_cause_velocity = native.allowCauseVelocity;
			data.allow_nearest_vehicle_velocity = native.allowNearestVehicleVelocity;
			data.world_space_lin_vel = native.worldSpaceLinVel;
			data.world_space_ang_vel = native.worldSpaceAngVel;
			data.random_range_lin_vel = native.randomRangeLinVel;
			data.random_range_ang_vel = native.randomRangeAngVel;
			data.launch_lin_vel = plain::read_scalars<float, 3>(&native.launchLinVel);
			data.launch_ang_vel = plain::read_scalars<float, 3>(&native.launchAngVel);
			data.launch_lin_vel2 = plain::read_scalars<float, 3>(&native.launchLinVel2);
			data.launch_ang_vel2 = plain::read_scalars<float, 3>(&native.launchAngVel2);
			data.event_stream_buffer_offset_client = native.eventStreamBufferOffsetClient;
			data.allow_missing_tag = native.allowMissingTag;
			data.use_root_on_missing_tag = native.useRootOnMissingTag;
			data.effect_alias = plain::read_string(native.effectAlias);
			data.effect_def = read_fx_combined_def(native.effectDef);
			return data;
		}

		void write_scriptable_event_spawn_dynent_def(ScriptableEventSpawnDynentDef& native, const asset_data::scriptable::scriptable_event_spawn_dynent_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			native.stateful = data.stateful;
			native.model = plain::resolve_asset<XModel>(resolve, ASSET_TYPE_XMODEL, data.model);
			native.tagName = plain::write_string(memory, data.tag_name);
			native.scrTagName = static_cast<scr_string_t>(plain::write_script_string(data.scr_tag_name));
			native.allowCauseVelocity = data.allow_cause_velocity;
			native.allowNearestVehicleVelocity = data.allow_nearest_vehicle_velocity;
			native.worldSpaceLinVel = data.world_space_lin_vel;
			native.worldSpaceAngVel = data.world_space_ang_vel;
			native.randomRangeLinVel = data.random_range_lin_vel;
			native.randomRangeAngVel = data.random_range_ang_vel;
			plain::write_scalars(&native.launchLinVel, data.launch_lin_vel);
			plain::write_scalars(&native.launchAngVel, data.launch_ang_vel);
			plain::write_scalars(&native.launchLinVel2, data.launch_lin_vel2);
			plain::write_scalars(&native.launchAngVel2, data.launch_ang_vel2);
			native.eventStreamBufferOffsetClient = data.event_stream_buffer_offset_client;
			native.allowMissingTag = data.allow_missing_tag;
			native.useRootOnMissingTag = data.use_root_on_missing_tag;
			native.effectAlias = plain::write_string(memory, data.effect_alias);
			write_fx_combined_def(native.effectDef, data.effect_def, resolve, memory);
		}

		std::size_t size_scriptable_event_spawn_dynent_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_spawn_dynent_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			size += plain::string_size(data.tag_name);
			size += plain::string_size(data.effect_alias);
			size += size_fx_combined_def(data.effect_def);
			return size;
		}

		asset_data::scriptable::scriptable_event_pfx_def read_scriptable_event_pfx_def(const ScriptableEventPFXDef& native)
		{
			plain::trace("read_scriptable_event_pfx_def");
			asset_data::scriptable::scriptable_event_pfx_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.stateful = native.stateful;
			data.scr_tag_names = plain::read_array(native.scrTagNames, native.scrTagCount, "ScriptableEventPFXDef.scrTagNames", [&](const auto& item) { return plain::read_script_string(item); });
			data.scr_end_tag_names = plain::read_array(native.scrEndTagNames, native.scrEndTagCount, "ScriptableEventPFXDef.scrEndTagNames", [&](const auto& item) { return plain::read_script_string(item); });
			data.use_tag_angles = native.useTagAngles;
			data.effect_alias = plain::read_string(native.effectAlias);
			data.effect_def = read_fx_combined_def(native.effectDef);
			data.loop_rate = native.loopRate;
			data.oneshot_looping = native.oneshotLooping;
			data.event_stream_buffer_offset_client = native.eventStreamBufferOffsetClient;
			data.allow_missing_tag = native.allowMissingTag;
			data.use_root_on_missing_tag = native.useRootOnMissingTag;
			data.allow_nearest_vehicle_velocity = native.allowNearestVehicleVelocity;
			data.is_viewmodel = native.isViewmodel;
			data.kill_on_exit = native.killOnExit;
			data.use_dynamic_scripted_beam_length = native.useDynamicScriptedBeamLength;
			data.beam_bone_axis = native.beamBoneAxis;
			data.beam_length = native.beamLength;
			return data;
		}

		void write_scriptable_event_pfx_def(ScriptableEventPFXDef& native, const asset_data::scriptable::scriptable_event_pfx_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			native.stateful = data.stateful;
			native.scrTagNames = plain::write_array<scr_string_t>(data.scr_tag_names, memory, [&](auto& out, const auto& in) { out = static_cast<scr_string_t>(plain::write_script_string(in)); });
			native.scrEndTagNames = plain::write_array<scr_string_t>(data.scr_end_tag_names, memory, [&](auto& out, const auto& in) { out = static_cast<scr_string_t>(plain::write_script_string(in)); });
			native.useTagAngles = data.use_tag_angles;
			native.effectAlias = plain::write_string(memory, data.effect_alias);
			write_fx_combined_def(native.effectDef, data.effect_def, resolve, memory);
			native.loopRate = data.loop_rate;
			native.oneshotLooping = data.oneshot_looping;
			native.eventStreamBufferOffsetClient = data.event_stream_buffer_offset_client;
			native.allowMissingTag = data.allow_missing_tag;
			native.useRootOnMissingTag = data.use_root_on_missing_tag;
			native.allowNearestVehicleVelocity = data.allow_nearest_vehicle_velocity;
			native.isViewmodel = data.is_viewmodel;
			native.killOnExit = data.kill_on_exit;
			native.useDynamicScriptedBeamLength = data.use_dynamic_scripted_beam_length;
			native.beamBoneAxis = data.beam_bone_axis;
			native.beamLength = data.beam_length;
			native.scrTagCount = plain::count_of<std::uint32_t>(data.scr_tag_names);
			native.scrEndTagCount = plain::count_of<std::uint32_t>(data.scr_end_tag_names);
		}

		std::size_t size_scriptable_event_pfx_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_pfx_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			if (data.scr_tag_names)
			{
				size += plain::sized(std::max<std::size_t>(data.scr_tag_names->size(), 1) * sizeof(scr_string_t));
			}
			if (data.scr_end_tag_names)
			{
				size += plain::sized(std::max<std::size_t>(data.scr_end_tag_names->size(), 1) * sizeof(scr_string_t));
			}
			size += plain::string_size(data.effect_alias);
			size += size_fx_combined_def(data.effect_def);
			return size;
		}

		asset_data::scriptable::scriptable_event_sound_def read_scriptable_event_sound_def(const ScriptableEventSoundDef& native)
		{
			plain::trace("read_scriptable_event_sound_def");
			asset_data::scriptable::scriptable_event_sound_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.stateful = native.stateful;
			data.tag_name = plain::read_string(native.tagName);
			data.scr_tag_name = plain::read_script_string(native.scrTagName);
			data.sound_alias = plain::read_string(native.soundAlias);
			data.looping = native.looping;
			data.external_play = native.externalPlay;
			data.allow_missing_tag = native.allowMissingTag;
			data.use_root_on_missing_tag = native.useRootOnMissingTag;
			data.explosion_refl_class = plain::read_string(native.explosionReflClass);
			data.do_ground_trace = native.doGroundTrace;
			data.ground_trace_length = native.groundTraceLength;
			data.world_offset = plain::read_scalars<float, 3>(&native.worldOffset);
			plain::require_zero(native.soundAliasCache, "ScriptableEventSoundDef.soundAliasCache");
			return data;
		}

		void write_scriptable_event_sound_def(ScriptableEventSoundDef& native, const asset_data::scriptable::scriptable_event_sound_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			native.stateful = data.stateful;
			native.tagName = plain::write_string(memory, data.tag_name);
			native.scrTagName = static_cast<scr_string_t>(plain::write_script_string(data.scr_tag_name));
			native.soundAlias = plain::write_string(memory, data.sound_alias);
			native.looping = data.looping;
			native.externalPlay = data.external_play;
			native.allowMissingTag = data.allow_missing_tag;
			native.useRootOnMissingTag = data.use_root_on_missing_tag;
			native.explosionReflClass = plain::write_string(memory, data.explosion_refl_class);
			native.doGroundTrace = data.do_ground_trace;
			native.groundTraceLength = data.ground_trace_length;
			plain::write_scalars(&native.worldOffset, data.world_offset);
		}

		std::size_t size_scriptable_event_sound_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_sound_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			size += plain::string_size(data.tag_name);
			size += plain::string_size(data.sound_alias);
			size += plain::string_size(data.explosion_refl_class);
			return size;
		}

		asset_data::scriptable::scriptable_event_explosion_def read_scriptable_event_explosion_def(const ScriptableEventExplosionDef& native)
		{
			plain::trace("read_scriptable_event_explosion_def");
			asset_data::scriptable::scriptable_event_explosion_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.tag_name = plain::read_string(native.tagName);
			data.scr_tag_name = plain::read_script_string(native.scrTagName);
			data.weapon = plain::read_asset(ASSET_TYPE_WEAPON, native.weapon);
			data.radius = native.radius;
			data.dmg_outer = native.dmgOuter;
			data.dmg_inner = native.dmgInner;
			data.stateful = native.stateful;
			data.allow_missing_tag = native.allowMissingTag;
			data.use_root_on_missing_tag = native.useRootOnMissingTag;
			return data;
		}

		void write_scriptable_event_explosion_def(ScriptableEventExplosionDef& native, const asset_data::scriptable::scriptable_event_explosion_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			native.tagName = plain::write_string(memory, data.tag_name);
			native.scrTagName = static_cast<scr_string_t>(plain::write_script_string(data.scr_tag_name));
			native.weapon = plain::resolve_asset<WeaponCompleteDef>(resolve, ASSET_TYPE_WEAPON, data.weapon);
			native.radius = data.radius;
			native.dmgOuter = data.dmg_outer;
			native.dmgInner = data.dmg_inner;
			native.stateful = data.stateful;
			native.allowMissingTag = data.allow_missing_tag;
			native.useRootOnMissingTag = data.use_root_on_missing_tag;
		}

		std::size_t size_scriptable_event_explosion_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_explosion_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			size += plain::string_size(data.tag_name);
			return size;
		}

		asset_data::scriptable::scriptable_event_light_def read_scriptable_event_light_def(const ScriptableEventLightDef& native)
		{
			plain::trace("read_scriptable_event_light_def");
			asset_data::scriptable::scriptable_event_light_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.intensity_scale_min = native.intensityScaleMin;
			data.intensity_scale_max = native.intensityScaleMax;
			data.radius_min = native.radiusMin;
			data.radius_max = native.radiusMax;
			data.use_color = native.useColor;
			data.color_linear_srgb = plain::read_scalars<float, 4>(&native.colorLinearSrgb);
			data.use_state_transition_time = native.useStateTransitionTime;
			data.transition_min = native.transitionMin;
			data.transition_max = native.transitionMax;
			data.event_stream_buffer_offset_client = native.eventStreamBufferOffsetClient;
			return data;
		}

		void write_scriptable_event_light_def(ScriptableEventLightDef& native, const asset_data::scriptable::scriptable_event_light_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			native.intensityScaleMin = data.intensity_scale_min;
			native.intensityScaleMax = data.intensity_scale_max;
			native.radiusMin = data.radius_min;
			native.radiusMax = data.radius_max;
			native.useColor = data.use_color;
			plain::write_scalars(&native.colorLinearSrgb, data.color_linear_srgb);
			native.useStateTransitionTime = data.use_state_transition_time;
			native.transitionMin = data.transition_min;
			native.transitionMax = data.transition_max;
			native.eventStreamBufferOffsetClient = data.event_stream_buffer_offset_client;
		}

		std::size_t size_scriptable_event_light_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_light_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			return size;
		}

		asset_data::scriptable::scriptable_event_sun_def read_scriptable_event_sun_def(const ScriptableEventSunDef& native)
		{
			plain::trace("read_scriptable_event_sun_def");
			asset_data::scriptable::scriptable_event_sun_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.use_intensity = native.useIntensity;
			data.intensity_override_min = native.intensityOverrideMin;
			data.intensity_override_max = native.intensityOverrideMax;
			data.use_color = native.useColor;
			data.color_linear_srgb = plain::read_scalars<float, 4>(&native.colorLinearSrgb);
			data.use_state_transition_time = native.useStateTransitionTime;
			data.transition_min = native.transitionMin;
			data.transition_max = native.transitionMax;
			data.pitch_min = native.pitchMin;
			data.pitch_max = native.pitchMax;
			data.heading_min = native.headingMin;
			data.heading_max = native.headingMax;
			data.use_direction = native.useDirection;
			return data;
		}

		void write_scriptable_event_sun_def(ScriptableEventSunDef& native, const asset_data::scriptable::scriptable_event_sun_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			native.useIntensity = data.use_intensity;
			native.intensityOverrideMin = data.intensity_override_min;
			native.intensityOverrideMax = data.intensity_override_max;
			native.useColor = data.use_color;
			plain::write_scalars(&native.colorLinearSrgb, data.color_linear_srgb);
			native.useStateTransitionTime = data.use_state_transition_time;
			native.transitionMin = data.transition_min;
			native.transitionMax = data.transition_max;
			native.pitchMin = data.pitch_min;
			native.pitchMax = data.pitch_max;
			native.headingMin = data.heading_min;
			native.headingMax = data.heading_max;
			native.useDirection = data.use_direction;
		}

		std::size_t size_scriptable_event_sun_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_sun_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			return size;
		}

		asset_data::scriptable::scriptable_event_rumble_def read_scriptable_event_rumble_def(const ScriptableEventRumbleDef& native)
		{
			plain::trace("read_scriptable_event_rumble_def");
			asset_data::scriptable::scriptable_event_rumble_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.stateful = native.stateful;
			data.tag_name = plain::read_string(native.tagName);
			data.scr_tag_name = plain::read_script_string(native.scrTagName);
			data.rumble = plain::read_string(native.rumble);
			data.rumble_asset = plain::read_asset(ASSET_TYPE_RUMBLE, native.rumbleAsset);
			data.allow_missing_tag = native.allowMissingTag;
			data.use_root_on_missing_tag = native.useRootOnMissingTag;
			return data;
		}

		void write_scriptable_event_rumble_def(ScriptableEventRumbleDef& native, const asset_data::scriptable::scriptable_event_rumble_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			native.stateful = data.stateful;
			native.tagName = plain::write_string(memory, data.tag_name);
			native.scrTagName = static_cast<scr_string_t>(plain::write_script_string(data.scr_tag_name));
			native.rumble = plain::write_string(memory, data.rumble);
			native.rumbleAsset = plain::resolve_asset<RumbleInfo>(resolve, ASSET_TYPE_RUMBLE, data.rumble_asset);
			native.allowMissingTag = data.allow_missing_tag;
			native.useRootOnMissingTag = data.use_root_on_missing_tag;
		}

		std::size_t size_scriptable_event_rumble_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_rumble_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			size += plain::string_size(data.tag_name);
			size += plain::string_size(data.rumble);
			return size;
		}

		asset_data::scriptable::scriptable_event_screenshake_def read_scriptable_event_screenshake_def(const ScriptableEventScreenshakeDef& native)
		{
			plain::trace("read_scriptable_event_screenshake_def");
			asset_data::scriptable::scriptable_event_screenshake_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.stateful = native.stateful;
			data.is_earthquake = native.isEarthquake;
			data.is_screen_shake = native.isScreenShake;
			data.tag_name = plain::read_string(native.tagName);
			data.scr_tag_name = plain::read_script_string(native.scrTagName);
			data.duration = native.duration;
			data.duration_fade_up = native.durationFadeUp;
			data.duration_fade_down = native.durationFadeDown;
			data.radius = native.radius;
			data.scale_earthquake = native.scaleEarthquake;
			data.scale_pitch = native.scalePitch;
			data.scale_yaw = native.scaleYaw;
			data.scale_roll = native.scaleRoll;
			data.frequency_pitch = native.frequencyPitch;
			data.frequency_yaw = native.frequencyYaw;
			data.frequency_roll = native.frequencyRoll;
			data.allow_missing_tag = native.allowMissingTag;
			data.use_root_on_missing_tag = native.useRootOnMissingTag;
			return data;
		}

		void write_scriptable_event_screenshake_def(ScriptableEventScreenshakeDef& native, const asset_data::scriptable::scriptable_event_screenshake_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			native.stateful = data.stateful;
			native.isEarthquake = data.is_earthquake;
			native.isScreenShake = data.is_screen_shake;
			native.tagName = plain::write_string(memory, data.tag_name);
			native.scrTagName = static_cast<scr_string_t>(plain::write_script_string(data.scr_tag_name));
			native.duration = data.duration;
			native.durationFadeUp = data.duration_fade_up;
			native.durationFadeDown = data.duration_fade_down;
			native.radius = data.radius;
			native.scaleEarthquake = data.scale_earthquake;
			native.scalePitch = data.scale_pitch;
			native.scaleYaw = data.scale_yaw;
			native.scaleRoll = data.scale_roll;
			native.frequencyPitch = data.frequency_pitch;
			native.frequencyYaw = data.frequency_yaw;
			native.frequencyRoll = data.frequency_roll;
			native.allowMissingTag = data.allow_missing_tag;
			native.useRootOnMissingTag = data.use_root_on_missing_tag;
		}

		std::size_t size_scriptable_event_screenshake_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_screenshake_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			size += plain::string_size(data.tag_name);
			return size;
		}

		asset_data::scriptable::scriptable_event_part_damage_def read_scriptable_event_part_damage_def(const ScriptableEventPartDamageDef& native)
		{
			plain::trace("read_scriptable_event_part_damage_def");
			asset_data::scriptable::scriptable_event_part_damage_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.part_reference = plain::read_scalars<std::uint32_t, 1>(&native.partReference)[0];
			data.amount = native.amount;
			data.destroy = native.destroy;
			data.amount_do_t = native.amountDoT;
			data.interval_do_t = native.intervalDoT;
			data.event_stream_buffer_offset_server = native.eventStreamBufferOffsetServer;
			data.event_stream_buffer_offset_client = native.eventStreamBufferOffsetClient;
			plain::require_zero(native.part, "ScriptableEventPartDamageDef.part");
			return data;
		}

		void write_scriptable_event_part_damage_def(ScriptableEventPartDamageDef& native, const asset_data::scriptable::scriptable_event_part_damage_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			plain::write_scalars(&native.partReference, std::array<std::uint32_t, 1>{data.part_reference});
			native.amount = data.amount;
			native.destroy = data.destroy;
			native.amountDoT = data.amount_do_t;
			native.intervalDoT = data.interval_do_t;
			native.eventStreamBufferOffsetServer = data.event_stream_buffer_offset_server;
			native.eventStreamBufferOffsetClient = data.event_stream_buffer_offset_client;
		}

		std::size_t size_scriptable_event_part_damage_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_part_damage_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			return size;
		}

		asset_data::scriptable::scriptable_event_set_mayhem_def read_scriptable_event_set_mayhem_def(const ScriptableEventSetMayhemDef& native)
		{
			plain::trace("read_scriptable_event_set_mayhem_def");
			asset_data::scriptable::scriptable_event_set_mayhem_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.mayhem = plain::read_asset(ASSET_TYPE_MAYHEM, native.mayhem);
			data.event_stream_buffer_offset_client = native.eventStreamBufferOffsetClient;
			return data;
		}

		void write_scriptable_event_set_mayhem_def(ScriptableEventSetMayhemDef& native, const asset_data::scriptable::scriptable_event_set_mayhem_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			native.mayhem = plain::resolve_asset<MayhemData>(resolve, ASSET_TYPE_MAYHEM, data.mayhem);
			native.eventStreamBufferOffsetClient = data.event_stream_buffer_offset_client;
		}

		std::size_t size_scriptable_event_set_mayhem_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_set_mayhem_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			return size;
		}

		asset_data::scriptable::scriptable_event_play_mayhem_def read_scriptable_event_play_mayhem_def(const ScriptableEventPlayMayhemDef& native)
		{
			plain::trace("read_scriptable_event_play_mayhem_def");
			asset_data::scriptable::scriptable_event_play_mayhem_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.action = plain::enum_name(asset_data::scriptable::scriptable_mayhem_action_names, static_cast<std::int64_t>(native.action), "ScriptableEventPlayMayhemDef.action");
			data.event_stream_buffer_offset_client = native.eventStreamBufferOffsetClient;
			return data;
		}

		void write_scriptable_event_play_mayhem_def(ScriptableEventPlayMayhemDef& native, const asset_data::scriptable::scriptable_event_play_mayhem_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			native.action = static_cast<Scriptable_MayhemAction>(plain::enum_value(asset_data::scriptable::scriptable_mayhem_action_names, data.action, "ScriptableEventPlayMayhemDef.action"));
			native.eventStreamBufferOffsetClient = data.event_stream_buffer_offset_client;
		}

		std::size_t size_scriptable_event_play_mayhem_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_play_mayhem_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			return size;
		}

		asset_data::scriptable::scriptable_event_viewmodel_shader_param_def read_scriptable_event_viewmodel_shader_param_def(const ScriptableEventViewmodelShaderParamDef& native)
		{
			plain::trace("read_scriptable_event_viewmodel_shader_param_def");
			asset_data::scriptable::scriptable_event_viewmodel_shader_param_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.shader_param = plain::enum_name(asset_data::scriptable::scriptable_shader_param_type_names, static_cast<std::int64_t>(native.shaderParam), "ScriptableEventViewmodelShaderParamDef.shaderParam");
			data.float_value = native.floatValue;
			data.place_holder_value1 = native.placeHolderValue1;
			data.place_holder_value2 = native.placeHolderValue2;
			data.place_holder_value3 = native.placeHolderValue3;
			data.place_holder_value4 = native.placeHolderValue4;
			data.transition_time = native.transitionTime;
			data.event_stream_buffer_offset_client = native.eventStreamBufferOffsetClient;
			return data;
		}

		void write_scriptable_event_viewmodel_shader_param_def(ScriptableEventViewmodelShaderParamDef& native, const asset_data::scriptable::scriptable_event_viewmodel_shader_param_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			native.shaderParam = static_cast<Scriptable_ShaderParamType>(plain::enum_value(asset_data::scriptable::scriptable_shader_param_type_names, data.shader_param, "ScriptableEventViewmodelShaderParamDef.shaderParam"));
			native.floatValue = data.float_value;
			native.placeHolderValue1 = data.place_holder_value1;
			native.placeHolderValue2 = data.place_holder_value2;
			native.placeHolderValue3 = data.place_holder_value3;
			native.placeHolderValue4 = data.place_holder_value4;
			native.transitionTime = data.transition_time;
			native.eventStreamBufferOffsetClient = data.event_stream_buffer_offset_client;
		}

		std::size_t size_scriptable_event_viewmodel_shader_param_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_viewmodel_shader_param_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			return size;
		}

		asset_data::scriptable::scriptable_event_viewmodel_change_image_def read_scriptable_event_viewmodel_change_image_def(const ScriptableEventViewmodelChangeImageDef& native)
		{
			plain::trace("read_scriptable_event_viewmodel_change_image_def");
			asset_data::scriptable::scriptable_event_viewmodel_change_image_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.image_id = plain::enum_name(asset_data::scriptable::scriptable_image_id_names, static_cast<std::int64_t>(native.imageId), "ScriptableEventViewmodelChangeImageDef.imageId");
			data.image_value = native.imageValue;
			return data;
		}

		void write_scriptable_event_viewmodel_change_image_def(ScriptableEventViewmodelChangeImageDef& native, const asset_data::scriptable::scriptable_event_viewmodel_change_image_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			native.imageId = static_cast<Scriptable_ImageId>(plain::enum_value(asset_data::scriptable::scriptable_image_id_names, data.image_id, "ScriptableEventViewmodelChangeImageDef.imageId"));
			native.imageValue = data.image_value;
		}

		std::size_t size_scriptable_event_viewmodel_change_image_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_viewmodel_change_image_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			return size;
		}

		asset_data::scriptable::scriptable_event_client_view_selector_def read_scriptable_event_client_view_selector_def(const ScriptableEventClientViewSelectorDef& native)
		{
			plain::trace("read_scriptable_event_client_view_selector_def");
			asset_data::scriptable::scriptable_event_client_view_selector_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.events1p = plain::read_array(native.events1p, native.event1pCount, "ScriptableEventClientViewSelectorDef.events1p", [&](const auto& item) { return read_scriptable_event_def(item); });
			data.events3p = plain::read_array(native.events3p, native.event3pCount, "ScriptableEventClientViewSelectorDef.events3p", [&](const auto& item) { return read_scriptable_event_def(item); });
			data.event_stream_buffer_offset_client = native.eventStreamBufferOffsetClient;
			return data;
		}

		void write_scriptable_event_client_view_selector_def(ScriptableEventClientViewSelectorDef& native, const asset_data::scriptable::scriptable_event_client_view_selector_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			native.events1p = plain::write_array<ScriptableEventDef>(data.events1p, memory, [&](auto& out, const auto& in) { write_scriptable_event_def(out, in, resolve, memory); });
			native.events3p = plain::write_array<ScriptableEventDef>(data.events3p, memory, [&](auto& out, const auto& in) { write_scriptable_event_def(out, in, resolve, memory); });
			native.eventStreamBufferOffsetClient = data.event_stream_buffer_offset_client;
			native.event1pCount = plain::count_of<std::uint32_t>(data.events1p);
			native.event3pCount = plain::count_of<std::uint32_t>(data.events3p);
		}

		std::size_t size_scriptable_event_client_view_selector_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_client_view_selector_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			if (data.events1p)
			{
				size += plain::sized(std::max<std::size_t>(data.events1p->size(), 1) * sizeof(ScriptableEventDef));
				for (const auto& item : *data.events1p)
				{
					size += size_scriptable_event_def(item);
				}
			}
			if (data.events3p)
			{
				size += plain::sized(std::max<std::size_t>(data.events3p->size(), 1) * sizeof(ScriptableEventDef));
				for (const auto& item : *data.events3p)
				{
					size += size_scriptable_event_def(item);
				}
			}
			return size;
		}

		asset_data::scriptable::scriptable_event_team_selector_def read_scriptable_event_team_selector_def(const ScriptableEventTeamSelectorDef& native)
		{
			plain::trace("read_scriptable_event_team_selector_def");
			asset_data::scriptable::scriptable_event_team_selector_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.team_filter = plain::enum_name(asset_data::scriptable::scriptable_team_filter_names, static_cast<std::int64_t>(native.teamFilter), "ScriptableEventTeamSelectorDef.teamFilter");
			data.test_local_player = native.testLocalPlayer;
			data.events_pass = plain::read_array(native.eventsPass, native.eventPassCount, "ScriptableEventTeamSelectorDef.eventsPass", [&](const auto& item) { return read_scriptable_event_def(item); });
			data.events_fail = plain::read_array(native.eventsFail, native.eventFailCount, "ScriptableEventTeamSelectorDef.eventsFail", [&](const auto& item) { return read_scriptable_event_def(item); });
			data.event_stream_buffer_offset_client = native.eventStreamBufferOffsetClient;
			data.event_stream_buffer_offset_server = native.eventStreamBufferOffsetServer;
			return data;
		}

		void write_scriptable_event_team_selector_def(ScriptableEventTeamSelectorDef& native, const asset_data::scriptable::scriptable_event_team_selector_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			native.teamFilter = static_cast<Scriptable_TeamFilter>(plain::enum_value(asset_data::scriptable::scriptable_team_filter_names, data.team_filter, "ScriptableEventTeamSelectorDef.teamFilter"));
			native.testLocalPlayer = data.test_local_player;
			native.eventsPass = plain::write_array<ScriptableEventDef>(data.events_pass, memory, [&](auto& out, const auto& in) { write_scriptable_event_def(out, in, resolve, memory); });
			native.eventsFail = plain::write_array<ScriptableEventDef>(data.events_fail, memory, [&](auto& out, const auto& in) { write_scriptable_event_def(out, in, resolve, memory); });
			native.eventStreamBufferOffsetClient = data.event_stream_buffer_offset_client;
			native.eventStreamBufferOffsetServer = data.event_stream_buffer_offset_server;
			native.eventPassCount = plain::count_of<std::uint32_t>(data.events_pass);
			native.eventFailCount = plain::count_of<std::uint32_t>(data.events_fail);
		}

		std::size_t size_scriptable_event_team_selector_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_team_selector_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			if (data.events_pass)
			{
				size += plain::sized(std::max<std::size_t>(data.events_pass->size(), 1) * sizeof(ScriptableEventDef));
				for (const auto& item : *data.events_pass)
				{
					size += size_scriptable_event_def(item);
				}
			}
			if (data.events_fail)
			{
				size += plain::sized(std::max<std::size_t>(data.events_fail->size(), 1) * sizeof(ScriptableEventDef));
				for (const auto& item : *data.events_fail)
				{
					size += size_scriptable_event_def(item);
				}
			}
			return size;
		}

		asset_data::scriptable::scriptable_event_add_model_def read_scriptable_event_add_model_def(const ScriptableEventAddModelDef& native)
		{
			plain::trace("read_scriptable_event_add_model_def");
			asset_data::scriptable::scriptable_event_add_model_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.tag_name = plain::read_string(native.tagName);
			data.scr_tag_name = plain::read_script_string(native.scrTagName);
			data.model = plain::read_asset(ASSET_TYPE_XMODEL, native.model);
			return data;
		}

		void write_scriptable_event_add_model_def(ScriptableEventAddModelDef& native, const asset_data::scriptable::scriptable_event_add_model_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			native.tagName = plain::write_string(memory, data.tag_name);
			native.scrTagName = static_cast<scr_string_t>(plain::write_script_string(data.scr_tag_name));
			native.model = plain::resolve_asset<XModel>(resolve, ASSET_TYPE_XMODEL, data.model);
		}

		std::size_t size_scriptable_event_add_model_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_add_model_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			size += plain::string_size(data.tag_name);
			return size;
		}

		asset_data::scriptable::scriptable_event_apply_force_def read_scriptable_event_apply_force_def(const ScriptableEventApplyForceDef& native)
		{
			plain::trace("read_scriptable_event_apply_force_def");
			asset_data::scriptable::scriptable_event_apply_force_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.world_space = native.worldSpace;
			data.random_range = native.randomRange;
			data.force_pos = plain::read_scalars<float, 3>(&native.forcePos);
			data.force_vector = plain::read_scalars<float, 3>(&native.forceVector);
			data.force_pos2 = plain::read_scalars<float, 3>(&native.forcePos2);
			data.force_vector2 = plain::read_scalars<float, 3>(&native.forceVector2);
			return data;
		}

		void write_scriptable_event_apply_force_def(ScriptableEventApplyForceDef& native, const asset_data::scriptable::scriptable_event_apply_force_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			native.worldSpace = data.world_space;
			native.randomRange = data.random_range;
			plain::write_scalars(&native.forcePos, data.force_pos);
			plain::write_scalars(&native.forceVector, data.force_vector);
			plain::write_scalars(&native.forcePos2, data.force_pos2);
			plain::write_scalars(&native.forceVector2, data.force_vector2);
		}

		std::size_t size_scriptable_event_apply_force_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_apply_force_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			return size;
		}

		asset_data::scriptable::scriptable_event_apply_angular_force_def read_scriptable_event_apply_angular_force_def(const ScriptableEventApplyAngularForceDef& native)
		{
			plain::trace("read_scriptable_event_apply_angular_force_def");
			asset_data::scriptable::scriptable_event_apply_angular_force_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.world_space = native.worldSpace;
			data.random_range = native.randomRange;
			data.force_vector = plain::read_scalars<float, 3>(&native.forceVector);
			data.force_vector2 = plain::read_scalars<float, 3>(&native.forceVector2);
			return data;
		}

		void write_scriptable_event_apply_angular_force_def(ScriptableEventApplyAngularForceDef& native, const asset_data::scriptable::scriptable_event_apply_angular_force_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			native.worldSpace = data.world_space;
			native.randomRange = data.random_range;
			plain::write_scalars(&native.forceVector, data.force_vector);
			plain::write_scalars(&native.forceVector2, data.force_vector2);
		}

		std::size_t size_scriptable_event_apply_angular_force_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_apply_angular_force_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			return size;
		}

		asset_data::scriptable::scriptable_event_apply_constant_force_def read_scriptable_event_apply_constant_force_def(const ScriptableEventApplyConstantForceDef& native)
		{
			plain::trace("read_scriptable_event_apply_constant_force_def");
			asset_data::scriptable::scriptable_event_apply_constant_force_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.world_space = native.worldSpace;
			data.random_range = native.randomRange;
			data.force_pos = plain::read_scalars<float, 3>(&native.forcePos);
			data.force_vector = plain::read_scalars<float, 3>(&native.forceVector);
			data.force_pos2 = plain::read_scalars<float, 3>(&native.forcePos2);
			data.force_vector2 = plain::read_scalars<float, 3>(&native.forceVector2);
			data.event_stream_buffer_offset_server = native.eventStreamBufferOffsetServer;
			data.event_stream_buffer_offset_client = native.eventStreamBufferOffsetClient;
			return data;
		}

		void write_scriptable_event_apply_constant_force_def(ScriptableEventApplyConstantForceDef& native, const asset_data::scriptable::scriptable_event_apply_constant_force_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			native.worldSpace = data.world_space;
			native.randomRange = data.random_range;
			plain::write_scalars(&native.forcePos, data.force_pos);
			plain::write_scalars(&native.forceVector, data.force_vector);
			plain::write_scalars(&native.forcePos2, data.force_pos2);
			plain::write_scalars(&native.forceVector2, data.force_vector2);
			native.eventStreamBufferOffsetServer = data.event_stream_buffer_offset_server;
			native.eventStreamBufferOffsetClient = data.event_stream_buffer_offset_client;
		}

		std::size_t size_scriptable_event_apply_constant_force_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_apply_constant_force_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			return size;
		}

		asset_data::scriptable::scriptable_event_apply_constant_angular_force_def read_scriptable_event_apply_constant_angular_force_def(const ScriptableEventApplyConstantAngularForceDef& native)
		{
			plain::trace("read_scriptable_event_apply_constant_angular_force_def");
			asset_data::scriptable::scriptable_event_apply_constant_angular_force_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.world_space = native.worldSpace;
			data.random_range = native.randomRange;
			data.force_vector = plain::read_scalars<float, 3>(&native.forceVector);
			data.force_vector2 = plain::read_scalars<float, 3>(&native.forceVector2);
			data.event_stream_buffer_offset_server = native.eventStreamBufferOffsetServer;
			data.event_stream_buffer_offset_client = native.eventStreamBufferOffsetClient;
			return data;
		}

		void write_scriptable_event_apply_constant_angular_force_def(ScriptableEventApplyConstantAngularForceDef& native, const asset_data::scriptable::scriptable_event_apply_constant_angular_force_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			native.worldSpace = data.world_space;
			native.randomRange = data.random_range;
			plain::write_scalars(&native.forceVector, data.force_vector);
			plain::write_scalars(&native.forceVector2, data.force_vector2);
			native.eventStreamBufferOffsetServer = data.event_stream_buffer_offset_server;
			native.eventStreamBufferOffsetClient = data.event_stream_buffer_offset_client;
		}

		std::size_t size_scriptable_event_apply_constant_angular_force_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_apply_constant_angular_force_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			return size;
		}

		asset_data::scriptable::scriptable_event_compass_icon_def read_scriptable_event_compass_icon_def(const ScriptableEventCompassIconDef& native)
		{
			plain::trace("read_scriptable_event_compass_icon_def");
			asset_data::scriptable::scriptable_event_compass_icon_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.stateful = native.stateful;
			data.friendly_compass_icon_always_visible = native.friendlyCompassIconAlwaysVisible;
			data.enemy_compass_icon_always_visible = native.enemyCompassIconAlwaysVisible;
			data.friendly_arrow = plain::read_asset(ASSET_TYPE_MATERIAL, native.friendlyArrow);
			data.friendly_firing = plain::read_asset(ASSET_TYPE_MATERIAL, native.friendlyFiring);
			data.friendly_chatting = plain::read_asset(ASSET_TYPE_MATERIAL, native.friendlyChatting);
			data.friendly_yelling = plain::read_asset(ASSET_TYPE_MATERIAL, native.friendlyYelling);
			data.party_arrow = plain::read_asset(ASSET_TYPE_MATERIAL, native.partyArrow);
			data.party_firing = plain::read_asset(ASSET_TYPE_MATERIAL, native.partyFiring);
			data.party_chatting = plain::read_asset(ASSET_TYPE_MATERIAL, native.partyChatting);
			data.party_yelling = plain::read_asset(ASSET_TYPE_MATERIAL, native.partyYelling);
			data.squad_arrow = plain::read_asset(ASSET_TYPE_MATERIAL, native.squadArrow);
			data.squad_firing = plain::read_asset(ASSET_TYPE_MATERIAL, native.squadFiring);
			plain::require_zero(native.enemyCompassIconQuiet, "ScriptableEventCompassIconDef.enemyCompassIconQuiet");
			plain::require_zero(native.enemyCompassIconFiring, "ScriptableEventCompassIconDef.enemyCompassIconFiring");
			data.enemy_compass_icon_directional = plain::read_asset(ASSET_TYPE_MATERIAL, native.enemyCompassIconDirectional);
			return data;
		}

		void write_scriptable_event_compass_icon_def(ScriptableEventCompassIconDef& native, const asset_data::scriptable::scriptable_event_compass_icon_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			native.stateful = data.stateful;
			native.friendlyCompassIconAlwaysVisible = data.friendly_compass_icon_always_visible;
			native.enemyCompassIconAlwaysVisible = data.enemy_compass_icon_always_visible;
			native.friendlyArrow = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, data.friendly_arrow);
			native.friendlyFiring = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, data.friendly_firing);
			native.friendlyChatting = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, data.friendly_chatting);
			native.friendlyYelling = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, data.friendly_yelling);
			native.partyArrow = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, data.party_arrow);
			native.partyFiring = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, data.party_firing);
			native.partyChatting = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, data.party_chatting);
			native.partyYelling = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, data.party_yelling);
			native.squadArrow = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, data.squad_arrow);
			native.squadFiring = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, data.squad_firing);
			native.enemyCompassIconDirectional = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, data.enemy_compass_icon_directional);
		}

		std::size_t size_scriptable_event_compass_icon_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_compass_icon_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			return size;
		}

		asset_data::scriptable::scriptable_event_material_override_def read_scriptable_event_material_override_def(const ScriptableEventMaterialOverrideDef& native)
		{
			plain::trace("read_scriptable_event_material_override_def");
			asset_data::scriptable::scriptable_event_material_override_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.material_override_type = plain::enum_name(asset_data::scriptable::scriptable_material_override_type_names, static_cast<std::int64_t>(native.materialOverrideType), "ScriptableEventMaterialOverrideDef.materialOverrideType");
			data.transition_time = native.transitionTime;
			data.final_value = native.finalValue;
			data.event_stream_buffer_offset_client = native.eventStreamBufferOffsetClient;
			return data;
		}

		void write_scriptable_event_material_override_def(ScriptableEventMaterialOverrideDef& native, const asset_data::scriptable::scriptable_event_material_override_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			native.materialOverrideType = static_cast<Scriptable_MaterialOverrideType>(plain::enum_value(asset_data::scriptable::scriptable_material_override_type_names, data.material_override_type, "ScriptableEventMaterialOverrideDef.materialOverrideType"));
			native.transitionTime = data.transition_time;
			native.finalValue = data.final_value;
			native.eventStreamBufferOffsetClient = data.event_stream_buffer_offset_client;
		}

		std::size_t size_scriptable_event_material_override_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_material_override_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			return size;
		}

		asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def read_scriptable_event_dynamic_bone_noise_curve_def(const ScriptableEventDynamicBoneNoiseCurveDef& native)
		{
			plain::trace("read_scriptable_event_dynamic_bone_noise_curve_def");
			asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.stateful = native.stateful;
			data.curve = plain::read_asset(ASSET_TYPE_XANIMCURVE, native.curve);
			data.strength_multiplier = native.strengthMultiplier;
			data.rate_multiplier = native.rateMultiplier;
			data.is_viewmodel = native.isViewmodel;
			return data;
		}

		void write_scriptable_event_dynamic_bone_noise_curve_def(ScriptableEventDynamicBoneNoiseCurveDef& native, const asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			native.stateful = data.stateful;
			native.curve = plain::resolve_asset<XAnimCurve>(resolve, ASSET_TYPE_XANIMCURVE, data.curve);
			native.strengthMultiplier = data.strength_multiplier;
			native.rateMultiplier = data.rate_multiplier;
			native.isViewmodel = data.is_viewmodel;
		}

		std::size_t size_scriptable_event_dynamic_bone_noise_curve_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			return size;
		}

		asset_data::scriptable::scriptable_event_move_def read_scriptable_event_move_def(const ScriptableEventMoveDef& native)
		{
			plain::trace("read_scriptable_event_move_def");
			asset_data::scriptable::scriptable_event_move_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.origin_offset = plain::read_scalars<float, 3>(&native.originOffset);
			data.angle_offset = plain::read_scalars<float, 3>(&native.angleOffset);
			data.seconds = native.seconds;
			data.seconds_accel = native.secondsAccel;
			data.seconds_decel = native.secondsDecel;
			data.event_stream_buffer_offset_server = native.eventStreamBufferOffsetServer;
			data.event_stream_buffer_offset_client = native.eventStreamBufferOffsetClient;
			return data;
		}

		void write_scriptable_event_move_def(ScriptableEventMoveDef& native, const asset_data::scriptable::scriptable_event_move_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			plain::write_scalars(&native.originOffset, data.origin_offset);
			plain::write_scalars(&native.angleOffset, data.angle_offset);
			native.seconds = data.seconds;
			native.secondsAccel = data.seconds_accel;
			native.secondsDecel = data.seconds_decel;
			native.eventStreamBufferOffsetServer = data.event_stream_buffer_offset_server;
			native.eventStreamBufferOffsetClient = data.event_stream_buffer_offset_client;
		}

		std::size_t size_scriptable_event_move_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_move_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			return size;
		}

		asset_data::scriptable::scriptable_event_footstep_def read_scriptable_event_footstep_def(const ScriptableEventFootstepDef& native)
		{
			plain::trace("read_scriptable_event_footstep_def");
			asset_data::scriptable::scriptable_event_footstep_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.tag_name = plain::read_string(native.tagName);
			data.scr_tag_name = plain::read_script_string(native.scrTagName);
			data.sound_alias = plain::read_string(native.soundAlias);
			data.allow_missing_tag = native.allowMissingTag;
			data.use_root_on_missing_tag = native.useRootOnMissingTag;
			data.flags = plain::enum_name(asset_data::scriptable::scriptable_footstep_flag_names, static_cast<std::int64_t>(native.flags), "ScriptableEventFootstepDef.flags");
			data.right_footstep_vfx = plain::read_asset(ASSET_TYPE_FOOTSTEP_VFX, native.rightFootstepVFX);
			data.left_footstep_vfx = plain::read_asset(ASSET_TYPE_FOOTSTEP_VFX, native.leftFootstepVFX);
			return data;
		}

		void write_scriptable_event_footstep_def(ScriptableEventFootstepDef& native, const asset_data::scriptable::scriptable_event_footstep_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			native.tagName = plain::write_string(memory, data.tag_name);
			native.scrTagName = static_cast<scr_string_t>(plain::write_script_string(data.scr_tag_name));
			native.soundAlias = plain::write_string(memory, data.sound_alias);
			native.allowMissingTag = data.allow_missing_tag;
			native.useRootOnMissingTag = data.use_root_on_missing_tag;
			native.flags = static_cast<ScriptableFootstepFlag>(plain::enum_value(asset_data::scriptable::scriptable_footstep_flag_names, data.flags, "ScriptableEventFootstepDef.flags"));
			native.rightFootstepVFX = plain::resolve_asset<FootstepVFX>(resolve, ASSET_TYPE_FOOTSTEP_VFX, data.right_footstep_vfx);
			native.leftFootstepVFX = plain::resolve_asset<FootstepVFX>(resolve, ASSET_TYPE_FOOTSTEP_VFX, data.left_footstep_vfx);
		}

		std::size_t size_scriptable_event_footstep_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_footstep_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			size += plain::string_size(data.tag_name);
			size += plain::string_size(data.sound_alias);
			return size;
		}

		asset_data::scriptable::scriptable_event_gravity_arc_def read_scriptable_event_gravity_arc_def(const ScriptableEventGravityArcDef& native)
		{
			plain::trace("read_scriptable_event_gravity_arc_def");
			asset_data::scriptable::scriptable_event_gravity_arc_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.angle_offset = plain::read_scalars<float, 3>(&native.angleOffset);
			data.gravity_scale = native.gravityScale;
			data.upward_velocity = native.upwardVelocity;
			data.yaw_offset_for_xy_direction = native.yawOffsetForXYDirection;
			data.end_at_initial_pose = native.endAtInitialPose;
			data.random_spin = native.randomSpin;
			data.play_loot_trail_fx = native.playLootTrailFX;
			data.events_at_end = plain::read_array(native.eventsAtEnd, native.eventAtEndCount, "ScriptableEventGravityArcDef.eventsAtEnd", [&](const auto& item) { return read_scriptable_event_def(item); });
			data.event_stream_buffer_offset_server = native.eventStreamBufferOffsetServer;
			data.event_stream_buffer_offset_client = native.eventStreamBufferOffsetClient;
			return data;
		}

		void write_scriptable_event_gravity_arc_def(ScriptableEventGravityArcDef& native, const asset_data::scriptable::scriptable_event_gravity_arc_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			plain::write_scalars(&native.angleOffset, data.angle_offset);
			native.gravityScale = data.gravity_scale;
			native.upwardVelocity = data.upward_velocity;
			native.yawOffsetForXYDirection = data.yaw_offset_for_xy_direction;
			native.endAtInitialPose = data.end_at_initial_pose;
			native.randomSpin = data.random_spin;
			native.playLootTrailFX = data.play_loot_trail_fx;
			native.eventsAtEnd = plain::write_array<ScriptableEventDef>(data.events_at_end, memory, [&](auto& out, const auto& in) { write_scriptable_event_def(out, in, resolve, memory); });
			native.eventStreamBufferOffsetServer = data.event_stream_buffer_offset_server;
			native.eventStreamBufferOffsetClient = data.event_stream_buffer_offset_client;
			native.eventAtEndCount = plain::count_of<std::uint32_t>(data.events_at_end);
		}

		std::size_t size_scriptable_event_gravity_arc_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_gravity_arc_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			if (data.events_at_end)
			{
				size += plain::sized(std::max<std::size_t>(data.events_at_end->size(), 1) * sizeof(ScriptableEventDef));
				for (const auto& item : *data.events_at_end)
				{
					size += size_scriptable_event_def(item);
				}
			}
			return size;
		}

		asset_data::scriptable::scriptable_event_view_trigger_def read_scriptable_event_view_trigger_def(const ScriptableEventViewTriggerDef& native)
		{
			plain::trace("read_scriptable_event_view_trigger_def");
			asset_data::scriptable::scriptable_event_view_trigger_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.trigger_distance_sq = native.triggerDistanceSq;
			data.trigger_fov_cos_half_angle = native.triggerFOVCosHalfAngle;
			data.event_stream_buffer_offset_client = native.eventStreamBufferOffsetClient;
			return data;
		}

		void write_scriptable_event_view_trigger_def(ScriptableEventViewTriggerDef& native, const asset_data::scriptable::scriptable_event_view_trigger_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			native.triggerDistanceSq = data.trigger_distance_sq;
			native.triggerFOVCosHalfAngle = data.trigger_fov_cos_half_angle;
			native.eventStreamBufferOffsetClient = data.event_stream_buffer_offset_client;
		}

		std::size_t size_scriptable_event_view_trigger_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_view_trigger_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			return size;
		}

		asset_data::scriptable::objective_settings read_objective_settings(const ObjectiveSettings& native)
		{
			plain::trace("read_objective_settings");
			asset_data::scriptable::objective_settings data;
			data.flags = std::to_array(native.flags);
			data.state = plain::enum_name(asset_data::scriptable::objective_state_t_names, static_cast<std::int64_t>(native.state), "ObjectiveSettings.state");
			data.background = static_cast<std::uint8_t>(native.background);
			data.z_offset = native.zOffset;
			data.cached = native.cached;
			data.always_show_in_world = native.alwaysShowInWorld;
			data.always_snap_to_screen_edge = native.alwaysSnapToScreenEdge;
			data.size = static_cast<std::uint8_t>(native.size);
			data.label = native.label;
			data.description = native.description;
			data.icon = native.icon;
			return data;
		}

		void write_objective_settings(ObjectiveSettings& native, const asset_data::scriptable::objective_settings& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			std::ranges::copy(data.flags, native.flags);
			native.state = static_cast<objectiveState_t>(plain::enum_value(asset_data::scriptable::objective_state_t_names, data.state, "ObjectiveSettings.state"));
			native.background = static_cast<ObjectiveBackground>(data.background);
			native.zOffset = data.z_offset;
			native.cached = data.cached;
			native.alwaysShowInWorld = data.always_show_in_world;
			native.alwaysSnapToScreenEdge = data.always_snap_to_screen_edge;
			native.size = static_cast<ObjectiveIconSize>(data.size);
			native.label = data.label;
			native.description = data.description;
			native.icon = data.icon;
		}

		std::size_t size_objective_settings([[maybe_unused]] const asset_data::scriptable::objective_settings& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::scriptable::scriptable_event_objective_def read_scriptable_event_objective_def(const ScriptableEventObjectiveDef& native)
		{
			plain::trace("read_scriptable_event_objective_def");
			asset_data::scriptable::scriptable_event_objective_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.settings = read_objective_settings(native.settings);
			data.description = plain::read_string(native.description);
			data.icon = plain::read_string(native.icon);
			data.label = plain::read_string(native.label);
			data.icon_image = plain::read_asset(ASSET_TYPE_IMAGE, native.iconImage);
			return data;
		}

		void write_scriptable_event_objective_def(ScriptableEventObjectiveDef& native, const asset_data::scriptable::scriptable_event_objective_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			write_objective_settings(native.settings, data.settings, resolve, memory);
			native.description = plain::write_string(memory, data.description);
			native.icon = plain::write_string(memory, data.icon);
			native.label = plain::write_string(memory, data.label);
			native.iconImage = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, data.icon_image);
		}

		std::size_t size_scriptable_event_objective_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_objective_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			size += size_objective_settings(data.settings);
			size += plain::string_size(data.description);
			size += plain::string_size(data.icon);
			size += plain::string_size(data.label);
			return size;
		}

		asset_data::scriptable::scriptable_event_spatial_disable_def read_scriptable_event_spatial_disable_def(const ScriptableEventSpatialDisableDef& native)
		{
			plain::trace("read_scriptable_event_spatial_disable_def");
			asset_data::scriptable::scriptable_event_spatial_disable_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.disabled = native.disabled;
			return data;
		}

		void write_scriptable_event_spatial_disable_def(ScriptableEventSpatialDisableDef& native, const asset_data::scriptable::scriptable_event_spatial_disable_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			native.disabled = data.disabled;
		}

		std::size_t size_scriptable_event_spatial_disable_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_spatial_disable_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			return size;
		}

		asset_data::scriptable::scriptable_event_hover_def read_scriptable_event_hover_def(const ScriptableEventHoverDef& native)
		{
			plain::trace("read_scriptable_event_hover_def");
			asset_data::scriptable::scriptable_event_hover_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.amplitude_up_down = native.amplitudeUpDown;
			data.frequency_up_down = native.frequencyUpDown;
			data.frequency_pitch = native.frequencyPitch;
			data.frequency_yaw = native.frequencyYaw;
			return data;
		}

		void write_scriptable_event_hover_def(ScriptableEventHoverDef& native, const asset_data::scriptable::scriptable_event_hover_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			native.amplitudeUpDown = data.amplitude_up_down;
			native.frequencyUpDown = data.frequency_up_down;
			native.frequencyPitch = data.frequency_pitch;
			native.frequencyYaw = data.frequency_yaw;
		}

		std::size_t size_scriptable_event_hover_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_hover_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			return size;
		}

		asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def read_scriptable_event_vehicle_blow_up_tire_def(const ScriptableEventVehicleBlowUpTireDef& native)
		{
			plain::trace("read_scriptable_event_vehicle_blow_up_tire_def");
			asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.tire_index = native.tireIndex;
			return data;
		}

		void write_scriptable_event_vehicle_blow_up_tire_def(ScriptableEventVehicleBlowUpTireDef& native, const asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			native.tireIndex = data.tire_index;
		}

		std::size_t size_scriptable_event_vehicle_blow_up_tire_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			return size;
		}

		asset_data::scriptable::scriptable_event_dlc1_def read_scriptable_event_dlc1_def(const ScriptableEventDlc1Def& native)
		{
			plain::trace("read_scriptable_event_dlc1_def");
			asset_data::scriptable::scriptable_event_dlc1_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.dlc_float0 = native.dlcFloat0;
			data.dlc_float1 = native.dlcFloat1;
			data.dlc_float2 = native.dlcFloat2;
			data.dlc_float3 = native.dlcFloat3;
			return data;
		}

		void write_scriptable_event_dlc1_def(ScriptableEventDlc1Def& native, const asset_data::scriptable::scriptable_event_dlc1_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			native.dlcFloat0 = data.dlc_float0;
			native.dlcFloat1 = data.dlc_float1;
			native.dlcFloat2 = data.dlc_float2;
			native.dlcFloat3 = data.dlc_float3;
		}

		std::size_t size_scriptable_event_dlc1_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_dlc1_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			return size;
		}

		asset_data::scriptable::scriptable_event_script_damage_def read_scriptable_event_script_damage_def(const ScriptableEventScriptDamageDef& native)
		{
			plain::trace("read_scriptable_event_script_damage_def");
			asset_data::scriptable::scriptable_event_script_damage_def data;
			data.base = plain::read_single(native.base, [&](const auto& item) { return read_scriptable_event_base_def(item); });
			data.notification = plain::read_string(native.notification);
			data.scr_notification = plain::read_script_string(native.scrNotification);
			data.param = native.param;
			return data;
		}

		void write_scriptable_event_script_damage_def(ScriptableEventScriptDamageDef& native, const asset_data::scriptable::scriptable_event_script_damage_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.base = plain::write_single<ScriptableEventBaseDef>(data.base, memory, [&](auto& out, const auto& in) { write_scriptable_event_base_def(out, in, resolve, memory); });
			native.notification = plain::write_string(memory, data.notification);
			native.scrNotification = static_cast<scr_string_t>(plain::write_script_string(data.scr_notification));
			native.param = data.param;
		}

		std::size_t size_scriptable_event_script_damage_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_script_damage_def& data)
		{
			std::size_t size = 0;
			if (data.base)
			{
				size += plain::sized(sizeof(ScriptableEventBaseDef));
				size += size_scriptable_event_base_def(*data.base);
			}
			size += plain::string_size(data.notification);
			return size;
		}

		asset_data::scriptable::scriptable_event_def read_scriptable_event_def(const ScriptableEventDef& native)
		{
			plain::trace("read_scriptable_event_def");
			asset_data::scriptable::scriptable_event_def data;
			data.base = read_scriptable_event_base_def(native.base);
			data.data = [&]() -> std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def> { switch (static_cast<std::uint32_t>(native.type)) { case 0: { plain::module_scope scope(0); auto item = read_scriptable_event_state_change_def(native.data.stateChange); if (native.data.stateChange.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 1: { plain::module_scope scope(1); auto item = read_scriptable_event_wait_def(native.data.wait); if (native.data.wait.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 2: { plain::module_scope scope(2); auto item = read_scriptable_event_random_def(native.data.random); if (native.data.random.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 3: { plain::module_scope scope(3); auto item = read_scriptable_event_script_def(native.data.script); if (native.data.script.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 4: { plain::module_scope scope(4); auto item = read_scriptable_event_model_def(native.data.model); if (native.data.model.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 5: { plain::module_scope scope(5); auto item = read_scriptable_event_collision_def(native.data.collision); if (native.data.collision.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 6: { plain::module_scope scope(6); auto item = read_scriptable_event_animation_def(native.data.animation); if (native.data.animation.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 7: { plain::module_scope scope(7); auto item = read_scriptable_event_hide_show_bone_def(native.data.hideShowBone); if (native.data.hideShowBone.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 8: { plain::module_scope scope(8); auto item = read_scriptable_event_disable_physics_sub_shape_def(native.data.disablePhysicsSubShape); if (native.data.disablePhysicsSubShape.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 9: { plain::module_scope scope(9); auto item = read_scriptable_event_note_track_def(native.data.noteTrack); if (native.data.noteTrack.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 10: { plain::module_scope scope(10); auto item = read_scriptable_event_chunk_dynent_def(native.data.chunkDynent); if (native.data.chunkDynent.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 11: { plain::module_scope scope(11); auto item = read_scriptable_event_spawn_dynent_def(native.data.spawnDynent); if (native.data.spawnDynent.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 12: { plain::module_scope scope(12); auto item = read_scriptable_event_pfx_def(native.data.particleFX); if (native.data.particleFX.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 13: { plain::module_scope scope(13); auto item = read_scriptable_event_sound_def(native.data.sound); if (native.data.sound.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 14: { plain::module_scope scope(14); auto item = read_scriptable_event_explosion_def(native.data.explosion); if (native.data.explosion.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 15: { plain::module_scope scope(15); auto item = read_scriptable_event_light_def(native.data.light); if (native.data.light.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 16: { plain::module_scope scope(16); auto item = read_scriptable_event_sun_def(native.data.sun); if (native.data.sun.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 17: { plain::module_scope scope(17); auto item = read_scriptable_event_rumble_def(native.data.rumble); if (native.data.rumble.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 18: { plain::module_scope scope(18); auto item = read_scriptable_event_screenshake_def(native.data.screenshake); if (native.data.screenshake.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 19: { plain::module_scope scope(19); auto item = read_scriptable_event_part_damage_def(native.data.partDamage); if (native.data.partDamage.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 20: { plain::module_scope scope(20); auto item = read_scriptable_event_set_mayhem_def(native.data.setMayhem); if (native.data.setMayhem.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 21: { plain::module_scope scope(21); auto item = read_scriptable_event_play_mayhem_def(native.data.playMayhem); if (native.data.playMayhem.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 22: { plain::module_scope scope(22); auto item = read_scriptable_event_viewmodel_shader_param_def(native.data.viewmodelShaderParam); if (native.data.viewmodelShaderParam.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 23: { plain::module_scope scope(23); auto item = read_scriptable_event_viewmodel_change_image_def(native.data.viewmodelChangeImage); if (native.data.viewmodelChangeImage.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 24: { plain::module_scope scope(24); auto item = read_scriptable_event_client_view_selector_def(native.data.clientViewSelector); if (native.data.clientViewSelector.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 25: { plain::module_scope scope(25); auto item = read_scriptable_event_team_selector_def(native.data.teamSelector); if (native.data.teamSelector.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 26: { plain::module_scope scope(26); auto item = read_scriptable_event_add_model_def(native.data.addModel); if (native.data.addModel.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 27: { plain::module_scope scope(27); auto item = read_scriptable_event_apply_force_def(native.data.applyForce); if (native.data.applyForce.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 28: { plain::module_scope scope(28); auto item = read_scriptable_event_apply_angular_force_def(native.data.applyAngularForce); if (native.data.applyAngularForce.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 29: { plain::module_scope scope(29); auto item = read_scriptable_event_apply_constant_force_def(native.data.applyConstantForce); if (native.data.applyConstantForce.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 30: { plain::module_scope scope(30); auto item = read_scriptable_event_apply_constant_angular_force_def(native.data.applyConstantAngularForce); if (native.data.applyConstantAngularForce.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 31: { plain::module_scope scope(31); auto item = read_scriptable_event_compass_icon_def(native.data.compassIcon); if (native.data.compassIcon.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 32: { plain::module_scope scope(32); auto item = read_scriptable_event_material_override_def(native.data.materialOverride); if (native.data.materialOverride.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 33: { plain::module_scope scope(33); auto item = read_scriptable_event_dynamic_bone_noise_curve_def(native.data.dynamicBoneNoiseCurve); if (native.data.dynamicBoneNoiseCurve.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 34: { plain::module_scope scope(34); auto item = read_scriptable_event_move_def(native.data.move); if (native.data.move.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 35: { plain::module_scope scope(35); auto item = read_scriptable_event_footstep_def(native.data.footstep); if (native.data.footstep.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 36: { plain::module_scope scope(36); auto item = read_scriptable_event_gravity_arc_def(native.data.gravityArc); if (native.data.gravityArc.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 37: { plain::module_scope scope(37); auto item = read_scriptable_event_view_trigger_def(native.data.viewTrigger); if (native.data.viewTrigger.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 38: { plain::module_scope scope(38); auto item = read_scriptable_event_objective_def(native.data.objective); if (native.data.objective.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 39: { plain::module_scope scope(39); auto item = read_scriptable_event_spatial_disable_def(native.data.spatialDisable); if (native.data.spatialDisable.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 40: { plain::module_scope scope(40); auto item = read_scriptable_event_hover_def(native.data.hover); if (native.data.hover.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 41: { plain::module_scope scope(41); auto item = read_scriptable_event_vehicle_blow_up_tire_def(native.data.vehicleBlowUpTire); if (native.data.vehicleBlowUpTire.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 42: { plain::module_scope scope(42); auto item = read_scriptable_event_dlc1_def(native.data.dlc1); if (native.data.dlc1.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } case 43: { plain::module_scope scope(43); auto item = read_scriptable_event_script_damage_def(native.data.scriptDamage); if (native.data.scriptDamage.base == &native.base) { item.base.reset(); item.base_is_event_base = true; } return std::variant<std::monostate, asset_data::scriptable::scriptable_event_state_change_def, asset_data::scriptable::scriptable_event_wait_def, asset_data::scriptable::scriptable_event_random_def, asset_data::scriptable::scriptable_event_script_def, asset_data::scriptable::scriptable_event_model_def, asset_data::scriptable::scriptable_event_collision_def, asset_data::scriptable::scriptable_event_animation_def, asset_data::scriptable::scriptable_event_hide_show_bone_def, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def, asset_data::scriptable::scriptable_event_note_track_def, asset_data::scriptable::scriptable_event_chunk_dynent_def, asset_data::scriptable::scriptable_event_spawn_dynent_def, asset_data::scriptable::scriptable_event_pfx_def, asset_data::scriptable::scriptable_event_sound_def, asset_data::scriptable::scriptable_event_explosion_def, asset_data::scriptable::scriptable_event_light_def, asset_data::scriptable::scriptable_event_sun_def, asset_data::scriptable::scriptable_event_rumble_def, asset_data::scriptable::scriptable_event_screenshake_def, asset_data::scriptable::scriptable_event_part_damage_def, asset_data::scriptable::scriptable_event_set_mayhem_def, asset_data::scriptable::scriptable_event_play_mayhem_def, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def, asset_data::scriptable::scriptable_event_viewmodel_change_image_def, asset_data::scriptable::scriptable_event_client_view_selector_def, asset_data::scriptable::scriptable_event_team_selector_def, asset_data::scriptable::scriptable_event_add_model_def, asset_data::scriptable::scriptable_event_apply_force_def, asset_data::scriptable::scriptable_event_apply_angular_force_def, asset_data::scriptable::scriptable_event_apply_constant_force_def, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def, asset_data::scriptable::scriptable_event_compass_icon_def, asset_data::scriptable::scriptable_event_material_override_def, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def, asset_data::scriptable::scriptable_event_move_def, asset_data::scriptable::scriptable_event_footstep_def, asset_data::scriptable::scriptable_event_gravity_arc_def, asset_data::scriptable::scriptable_event_view_trigger_def, asset_data::scriptable::scriptable_event_objective_def, asset_data::scriptable::scriptable_event_spatial_disable_def, asset_data::scriptable::scriptable_event_hover_def, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def, asset_data::scriptable::scriptable_event_dlc1_def, asset_data::scriptable::scriptable_event_script_damage_def>(std::move(item)); } default: plain::fail("ScriptableEventDef.data", "has a tag value without a case"); } }();
			return data;
		}

		void write_scriptable_event_def(ScriptableEventDef& native, const asset_data::scriptable::scriptable_event_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_scriptable_event_base_def(native.base, data.base, resolve, memory);
			std::visit([&](const auto& value)
			{
				using value_type = std::decay_t<decltype(value)>;
				if constexpr (std::is_same_v<value_type, std::monostate>)
				{
					plain::fail("ScriptableEventDef.data", "holds no module");
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_state_change_def>)
				{
					native.type = static_cast<Scriptable_EventType>(0);
					plain::module_scope scope(0);
					write_scriptable_event_state_change_def(native.data.stateChange, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.stateChange.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_wait_def>)
				{
					native.type = static_cast<Scriptable_EventType>(1);
					plain::module_scope scope(1);
					write_scriptable_event_wait_def(native.data.wait, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.wait.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_random_def>)
				{
					native.type = static_cast<Scriptable_EventType>(2);
					plain::module_scope scope(2);
					write_scriptable_event_random_def(native.data.random, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.random.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_script_def>)
				{
					native.type = static_cast<Scriptable_EventType>(3);
					plain::module_scope scope(3);
					write_scriptable_event_script_def(native.data.script, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.script.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_model_def>)
				{
					native.type = static_cast<Scriptable_EventType>(4);
					plain::module_scope scope(4);
					write_scriptable_event_model_def(native.data.model, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.model.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_collision_def>)
				{
					native.type = static_cast<Scriptable_EventType>(5);
					plain::module_scope scope(5);
					write_scriptable_event_collision_def(native.data.collision, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.collision.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_animation_def>)
				{
					native.type = static_cast<Scriptable_EventType>(6);
					plain::module_scope scope(6);
					write_scriptable_event_animation_def(native.data.animation, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.animation.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_hide_show_bone_def>)
				{
					native.type = static_cast<Scriptable_EventType>(7);
					plain::module_scope scope(7);
					write_scriptable_event_hide_show_bone_def(native.data.hideShowBone, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.hideShowBone.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def>)
				{
					native.type = static_cast<Scriptable_EventType>(8);
					plain::module_scope scope(8);
					write_scriptable_event_disable_physics_sub_shape_def(native.data.disablePhysicsSubShape, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.disablePhysicsSubShape.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_note_track_def>)
				{
					native.type = static_cast<Scriptable_EventType>(9);
					plain::module_scope scope(9);
					write_scriptable_event_note_track_def(native.data.noteTrack, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.noteTrack.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_chunk_dynent_def>)
				{
					native.type = static_cast<Scriptable_EventType>(10);
					plain::module_scope scope(10);
					write_scriptable_event_chunk_dynent_def(native.data.chunkDynent, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.chunkDynent.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_spawn_dynent_def>)
				{
					native.type = static_cast<Scriptable_EventType>(11);
					plain::module_scope scope(11);
					write_scriptable_event_spawn_dynent_def(native.data.spawnDynent, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.spawnDynent.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_pfx_def>)
				{
					native.type = static_cast<Scriptable_EventType>(12);
					plain::module_scope scope(12);
					write_scriptable_event_pfx_def(native.data.particleFX, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.particleFX.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_sound_def>)
				{
					native.type = static_cast<Scriptable_EventType>(13);
					plain::module_scope scope(13);
					write_scriptable_event_sound_def(native.data.sound, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.sound.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_explosion_def>)
				{
					native.type = static_cast<Scriptable_EventType>(14);
					plain::module_scope scope(14);
					write_scriptable_event_explosion_def(native.data.explosion, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.explosion.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_light_def>)
				{
					native.type = static_cast<Scriptable_EventType>(15);
					plain::module_scope scope(15);
					write_scriptable_event_light_def(native.data.light, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.light.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_sun_def>)
				{
					native.type = static_cast<Scriptable_EventType>(16);
					plain::module_scope scope(16);
					write_scriptable_event_sun_def(native.data.sun, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.sun.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_rumble_def>)
				{
					native.type = static_cast<Scriptable_EventType>(17);
					plain::module_scope scope(17);
					write_scriptable_event_rumble_def(native.data.rumble, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.rumble.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_screenshake_def>)
				{
					native.type = static_cast<Scriptable_EventType>(18);
					plain::module_scope scope(18);
					write_scriptable_event_screenshake_def(native.data.screenshake, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.screenshake.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_part_damage_def>)
				{
					native.type = static_cast<Scriptable_EventType>(19);
					plain::module_scope scope(19);
					write_scriptable_event_part_damage_def(native.data.partDamage, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.partDamage.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_set_mayhem_def>)
				{
					native.type = static_cast<Scriptable_EventType>(20);
					plain::module_scope scope(20);
					write_scriptable_event_set_mayhem_def(native.data.setMayhem, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.setMayhem.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_play_mayhem_def>)
				{
					native.type = static_cast<Scriptable_EventType>(21);
					plain::module_scope scope(21);
					write_scriptable_event_play_mayhem_def(native.data.playMayhem, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.playMayhem.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def>)
				{
					native.type = static_cast<Scriptable_EventType>(22);
					plain::module_scope scope(22);
					write_scriptable_event_viewmodel_shader_param_def(native.data.viewmodelShaderParam, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.viewmodelShaderParam.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_viewmodel_change_image_def>)
				{
					native.type = static_cast<Scriptable_EventType>(23);
					plain::module_scope scope(23);
					write_scriptable_event_viewmodel_change_image_def(native.data.viewmodelChangeImage, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.viewmodelChangeImage.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_client_view_selector_def>)
				{
					native.type = static_cast<Scriptable_EventType>(24);
					plain::module_scope scope(24);
					write_scriptable_event_client_view_selector_def(native.data.clientViewSelector, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.clientViewSelector.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_team_selector_def>)
				{
					native.type = static_cast<Scriptable_EventType>(25);
					plain::module_scope scope(25);
					write_scriptable_event_team_selector_def(native.data.teamSelector, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.teamSelector.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_add_model_def>)
				{
					native.type = static_cast<Scriptable_EventType>(26);
					plain::module_scope scope(26);
					write_scriptable_event_add_model_def(native.data.addModel, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.addModel.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_apply_force_def>)
				{
					native.type = static_cast<Scriptable_EventType>(27);
					plain::module_scope scope(27);
					write_scriptable_event_apply_force_def(native.data.applyForce, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.applyForce.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_apply_angular_force_def>)
				{
					native.type = static_cast<Scriptable_EventType>(28);
					plain::module_scope scope(28);
					write_scriptable_event_apply_angular_force_def(native.data.applyAngularForce, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.applyAngularForce.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_apply_constant_force_def>)
				{
					native.type = static_cast<Scriptable_EventType>(29);
					plain::module_scope scope(29);
					write_scriptable_event_apply_constant_force_def(native.data.applyConstantForce, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.applyConstantForce.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def>)
				{
					native.type = static_cast<Scriptable_EventType>(30);
					plain::module_scope scope(30);
					write_scriptable_event_apply_constant_angular_force_def(native.data.applyConstantAngularForce, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.applyConstantAngularForce.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_compass_icon_def>)
				{
					native.type = static_cast<Scriptable_EventType>(31);
					plain::module_scope scope(31);
					write_scriptable_event_compass_icon_def(native.data.compassIcon, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.compassIcon.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_material_override_def>)
				{
					native.type = static_cast<Scriptable_EventType>(32);
					plain::module_scope scope(32);
					write_scriptable_event_material_override_def(native.data.materialOverride, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.materialOverride.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def>)
				{
					native.type = static_cast<Scriptable_EventType>(33);
					plain::module_scope scope(33);
					write_scriptable_event_dynamic_bone_noise_curve_def(native.data.dynamicBoneNoiseCurve, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.dynamicBoneNoiseCurve.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_move_def>)
				{
					native.type = static_cast<Scriptable_EventType>(34);
					plain::module_scope scope(34);
					write_scriptable_event_move_def(native.data.move, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.move.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_footstep_def>)
				{
					native.type = static_cast<Scriptable_EventType>(35);
					plain::module_scope scope(35);
					write_scriptable_event_footstep_def(native.data.footstep, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.footstep.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_gravity_arc_def>)
				{
					native.type = static_cast<Scriptable_EventType>(36);
					plain::module_scope scope(36);
					write_scriptable_event_gravity_arc_def(native.data.gravityArc, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.gravityArc.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_view_trigger_def>)
				{
					native.type = static_cast<Scriptable_EventType>(37);
					plain::module_scope scope(37);
					write_scriptable_event_view_trigger_def(native.data.viewTrigger, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.viewTrigger.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_objective_def>)
				{
					native.type = static_cast<Scriptable_EventType>(38);
					plain::module_scope scope(38);
					write_scriptable_event_objective_def(native.data.objective, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.objective.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_spatial_disable_def>)
				{
					native.type = static_cast<Scriptable_EventType>(39);
					plain::module_scope scope(39);
					write_scriptable_event_spatial_disable_def(native.data.spatialDisable, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.spatialDisable.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_hover_def>)
				{
					native.type = static_cast<Scriptable_EventType>(40);
					plain::module_scope scope(40);
					write_scriptable_event_hover_def(native.data.hover, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.hover.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def>)
				{
					native.type = static_cast<Scriptable_EventType>(41);
					plain::module_scope scope(41);
					write_scriptable_event_vehicle_blow_up_tire_def(native.data.vehicleBlowUpTire, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.vehicleBlowUpTire.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_dlc1_def>)
				{
					native.type = static_cast<Scriptable_EventType>(42);
					plain::module_scope scope(42);
					write_scriptable_event_dlc1_def(native.data.dlc1, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.dlc1.base = &native.base;
					}
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_script_damage_def>)
				{
					native.type = static_cast<Scriptable_EventType>(43);
					plain::module_scope scope(43);
					write_scriptable_event_script_damage_def(native.data.scriptDamage, value, resolve, memory);
					if (value.base_is_event_base)
					{
						native.data.scriptDamage.base = &native.base;
					}
				}
			}, data.data);
		}

		std::size_t size_scriptable_event_def([[maybe_unused]] const asset_data::scriptable::scriptable_event_def& data)
		{
			std::size_t size = 0;
			size += size_scriptable_event_base_def(data.base);
			std::visit([&](const auto& value)
			{
				using value_type = std::decay_t<decltype(value)>;
				if constexpr (std::is_same_v<value_type, std::monostate>)
				{
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_state_change_def>)
				{
					size += size_scriptable_event_state_change_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_wait_def>)
				{
					size += size_scriptable_event_wait_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_random_def>)
				{
					size += size_scriptable_event_random_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_script_def>)
				{
					size += size_scriptable_event_script_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_model_def>)
				{
					size += size_scriptable_event_model_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_collision_def>)
				{
					size += size_scriptable_event_collision_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_animation_def>)
				{
					size += size_scriptable_event_animation_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_hide_show_bone_def>)
				{
					size += size_scriptable_event_hide_show_bone_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_disable_physics_sub_shape_def>)
				{
					size += size_scriptable_event_disable_physics_sub_shape_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_note_track_def>)
				{
					size += size_scriptable_event_note_track_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_chunk_dynent_def>)
				{
					size += size_scriptable_event_chunk_dynent_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_spawn_dynent_def>)
				{
					size += size_scriptable_event_spawn_dynent_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_pfx_def>)
				{
					size += size_scriptable_event_pfx_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_sound_def>)
				{
					size += size_scriptable_event_sound_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_explosion_def>)
				{
					size += size_scriptable_event_explosion_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_light_def>)
				{
					size += size_scriptable_event_light_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_sun_def>)
				{
					size += size_scriptable_event_sun_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_rumble_def>)
				{
					size += size_scriptable_event_rumble_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_screenshake_def>)
				{
					size += size_scriptable_event_screenshake_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_part_damage_def>)
				{
					size += size_scriptable_event_part_damage_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_set_mayhem_def>)
				{
					size += size_scriptable_event_set_mayhem_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_play_mayhem_def>)
				{
					size += size_scriptable_event_play_mayhem_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_viewmodel_shader_param_def>)
				{
					size += size_scriptable_event_viewmodel_shader_param_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_viewmodel_change_image_def>)
				{
					size += size_scriptable_event_viewmodel_change_image_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_client_view_selector_def>)
				{
					size += size_scriptable_event_client_view_selector_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_team_selector_def>)
				{
					size += size_scriptable_event_team_selector_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_add_model_def>)
				{
					size += size_scriptable_event_add_model_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_apply_force_def>)
				{
					size += size_scriptable_event_apply_force_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_apply_angular_force_def>)
				{
					size += size_scriptable_event_apply_angular_force_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_apply_constant_force_def>)
				{
					size += size_scriptable_event_apply_constant_force_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_apply_constant_angular_force_def>)
				{
					size += size_scriptable_event_apply_constant_angular_force_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_compass_icon_def>)
				{
					size += size_scriptable_event_compass_icon_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_material_override_def>)
				{
					size += size_scriptable_event_material_override_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_dynamic_bone_noise_curve_def>)
				{
					size += size_scriptable_event_dynamic_bone_noise_curve_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_move_def>)
				{
					size += size_scriptable_event_move_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_footstep_def>)
				{
					size += size_scriptable_event_footstep_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_gravity_arc_def>)
				{
					size += size_scriptable_event_gravity_arc_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_view_trigger_def>)
				{
					size += size_scriptable_event_view_trigger_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_objective_def>)
				{
					size += size_scriptable_event_objective_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_spatial_disable_def>)
				{
					size += size_scriptable_event_spatial_disable_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_hover_def>)
				{
					size += size_scriptable_event_hover_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_vehicle_blow_up_tire_def>)
				{
					size += size_scriptable_event_vehicle_blow_up_tire_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_dlc1_def>)
				{
					size += size_scriptable_event_dlc1_def(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::scriptable::scriptable_event_script_damage_def>)
				{
					size += size_scriptable_event_script_damage_def(value);
				}
			}, data.data);
			return size;
		}

		asset_data::scriptable::scriptable_note_track_def read_scriptable_note_track_def(const ScriptableNoteTrackDef& native)
		{
			plain::trace("read_scriptable_note_track_def");
			asset_data::scriptable::scriptable_note_track_def data;
			data.flags = native.flags;
			data.note_track_name = plain::read_string(native.noteTrackName);
			data.scr_note_track_name = plain::read_script_string(native.scrNoteTrackName);
			data.events = plain::read_array(native.events, native.numEvents, "ScriptableNoteTrackDef.events", [&](const auto& item) { return read_scriptable_event_def(item); });
			return data;
		}

		void write_scriptable_note_track_def(ScriptableNoteTrackDef& native, const asset_data::scriptable::scriptable_note_track_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.flags = data.flags;
			native.noteTrackName = plain::write_string(memory, data.note_track_name);
			native.scrNoteTrackName = static_cast<scr_string_t>(plain::write_script_string(data.scr_note_track_name));
			native.events = plain::write_array<ScriptableEventDef>(data.events, memory, [&](auto& out, const auto& in) { write_scriptable_event_def(out, in, resolve, memory); });
			native.numEvents = plain::count_of<std::uint32_t>(data.events);
		}

		std::size_t size_scriptable_note_track_def([[maybe_unused]] const asset_data::scriptable::scriptable_note_track_def& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.note_track_name);
			if (data.events)
			{
				size += plain::sized(std::max<std::size_t>(data.events->size(), 1) * sizeof(ScriptableEventDef));
				for (const auto& item : *data.events)
				{
					size += size_scriptable_event_def(item);
				}
			}
			return size;
		}

		asset_data::scriptable::scriptable_damage_tag_override read_scriptable_damage_tag_override(const ScriptableDamageTagOverride& native)
		{
			plain::trace("read_scriptable_damage_tag_override");
			asset_data::scriptable::scriptable_damage_tag_override data;
			data.tag = plain::read_string(native.tag);
			data.scr_tag = plain::read_script_string(native.scrTag);
			return data;
		}

		void write_scriptable_damage_tag_override(ScriptableDamageTagOverride& native, const asset_data::scriptable::scriptable_damage_tag_override& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.tag = plain::write_string(memory, data.tag);
			native.scrTag = static_cast<scr_string_t>(plain::write_script_string(data.scr_tag));
		}

		std::size_t size_scriptable_damage_tag_override([[maybe_unused]] const asset_data::scriptable::scriptable_damage_tag_override& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.tag);
			return size;
		}

	}

	std::expected<asset_data::scriptable::scriptable, std::string> to_neutral(const ScriptableDef& native)
	{
		try
		{
			plain::pointer_ids ids;
			auto data = read_scriptable(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	ScriptableDef* from_neutral(const asset_data::scriptable::scriptable& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<ScriptableDef>();
		write_scriptable(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::scriptable::scriptable& data)
	{
		return plain::rounded(sizeof(ScriptableDef)) + size_scriptable(data) + 64;
	}

}
