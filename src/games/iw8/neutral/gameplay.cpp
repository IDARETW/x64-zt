// Emitted by tools/gen_iw8_neutral.py from tools/neutral_families/gameplay.json: do not edit.
#include <std_include.hpp>
#include "gameplay.hpp"
#include "plain.hpp"

namespace zonetool::iw8::neutral
{
	namespace
	{
		asset_data::gameplay::animpkg read_animpkg(const WeaponAnimPackage& native);
		void write_animpkg(WeaponAnimPackage& native, const asset_data::gameplay::animpkg& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_animpkg(const asset_data::gameplay::animpkg& data);
		asset_data::gameplay::weapon_anim_package_state_timers read_weapon_anim_package_state_timers(const WeaponAnimPackageStateTimers& native);
		void write_weapon_anim_package_state_timers(WeaponAnimPackageStateTimers& native, const asset_data::gameplay::weapon_anim_package_state_timers& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_weapon_anim_package_state_timers(const asset_data::gameplay::weapon_anim_package_state_timers& data);
		asset_data::gameplay::footstep_anim read_footstep_anim(const FootstepAnim& native);
		void write_footstep_anim(FootstepAnim& native, const asset_data::gameplay::footstep_anim& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_footstep_anim(const asset_data::gameplay::footstep_anim& data);
		asset_data::gameplay::footstep_time read_footstep_time(const FootstepTime& native);
		void write_footstep_time(FootstepTime& native, const asset_data::gameplay::footstep_time& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_footstep_time(const asset_data::gameplay::footstep_time& data);
		asset_data::gameplay::movement_time read_movement_time(const MovementTime& native);
		void write_movement_time(MovementTime& native, const asset_data::gameplay::movement_time& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_movement_time(const asset_data::gameplay::movement_time& data);
		asset_data::gameplay::sfxpkg read_sfxpkg(const WeaponSFXPackage& native);
		void write_sfxpkg(WeaponSFXPackage& native, const asset_data::gameplay::sfxpkg& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_sfxpkg(const asset_data::gameplay::sfxpkg& data);
		asset_data::gameplay::weapon_sfx_package_sounds read_weapon_sfx_package_sounds(const WeaponSFXPackageSounds& native);
		void write_weapon_sfx_package_sounds(WeaponSFXPackageSounds& native, const asset_data::gameplay::weapon_sfx_package_sounds& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_weapon_sfx_package_sounds(const asset_data::gameplay::weapon_sfx_package_sounds& data);
		asset_data::gameplay::snd_alias_lookup read_snd_alias_lookup(const SndAliasLookup& native);
		void write_snd_alias_lookup(SndAliasLookup& native, const asset_data::gameplay::snd_alias_lookup& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_snd_alias_lookup(const asset_data::gameplay::snd_alias_lookup& data);
		asset_data::gameplay::trigger_disconnect_sound_data read_trigger_disconnect_sound_data(const TriggerDisconnectSoundData& native);
		void write_trigger_disconnect_sound_data(TriggerDisconnectSoundData& native, const asset_data::gameplay::trigger_disconnect_sound_data& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_trigger_disconnect_sound_data(const asset_data::gameplay::trigger_disconnect_sound_data& data);
		asset_data::gameplay::vfxpkg read_vfxpkg(const WeaponVFXPackage& native);
		void write_vfxpkg(WeaponVFXPackage& native, const asset_data::gameplay::vfxpkg& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_vfxpkg(const asset_data::gameplay::vfxpkg& data);
		asset_data::gameplay::weapon_vfx_package_defs read_weapon_vfx_package_defs(const WeaponVFXPackageDefs& native);
		void write_weapon_vfx_package_defs(WeaponVFXPackageDefs& native, const asset_data::gameplay::weapon_vfx_package_defs& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_weapon_vfx_package_defs(const asset_data::gameplay::weapon_vfx_package_defs& data);
		asset_data::gameplay::fx_combined_def read_fx_combined_def(const FxCombinedDef& native);
		void write_fx_combined_def(FxCombinedDef& native, const asset_data::gameplay::fx_combined_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_fx_combined_def(const asset_data::gameplay::fx_combined_def& data);
		asset_data::gameplay::tracer read_tracer(const TracerDef& native);
		void write_tracer(TracerDef& native, const asset_data::gameplay::tracer& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_tracer(const asset_data::gameplay::tracer& data);
		asset_data::gameplay::laser read_laser(const LaserDef& native);
		void write_laser(LaserDef& native, const asset_data::gameplay::laser& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_laser(const asset_data::gameplay::laser& data);
		asset_data::gameplay::camera read_camera(const CameraDef& native);
		void write_camera(CameraDef& native, const asset_data::gameplay::camera& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_camera(const asset_data::gameplay::camera& data);
		asset_data::gameplay::camera_first_person_profile_def read_camera_first_person_profile_def(const CameraFirstPersonProfileDef& native);
		void write_camera_first_person_profile_def(CameraFirstPersonProfileDef& native, const asset_data::gameplay::camera_first_person_profile_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_camera_first_person_profile_def(const asset_data::gameplay::camera_first_person_profile_def& data);
		asset_data::gameplay::camera_orbit_profile_def read_camera_orbit_profile_def(const CameraOrbitProfileDef& native);
		void write_camera_orbit_profile_def(CameraOrbitProfileDef& native, const asset_data::gameplay::camera_orbit_profile_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_camera_orbit_profile_def(const asset_data::gameplay::camera_orbit_profile_def& data);
		asset_data::gameplay::camera_sentry_drone_def read_camera_sentry_drone_def(const CameraSentryDroneDef& native);
		void write_camera_sentry_drone_def(CameraSentryDroneDef& native, const asset_data::gameplay::camera_sentry_drone_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_camera_sentry_drone_def(const asset_data::gameplay::camera_sentry_drone_def& data);
		asset_data::gameplay::camera_kill_cam_vehicle_def read_camera_kill_cam_vehicle_def(const CameraKillCamVehicleDef& native);
		void write_camera_kill_cam_vehicle_def(CameraKillCamVehicleDef& native, const asset_data::gameplay::camera_kill_cam_vehicle_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_camera_kill_cam_vehicle_def(const asset_data::gameplay::camera_kill_cam_vehicle_def& data);
		asset_data::gameplay::camera_transition_def read_camera_transition_def(const CameraTransitionDef& native);
		void write_camera_transition_def(CameraTransitionDef& native, const asset_data::gameplay::camera_transition_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_camera_transition_def(const asset_data::gameplay::camera_transition_def& data);
		asset_data::gameplay::hudoutline read_hudoutline(const HudOutlineDef& native);
		void write_hudoutline(HudOutlineDef& native, const asset_data::gameplay::hudoutline& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_hudoutline(const asset_data::gameplay::hudoutline& data);
		asset_data::gameplay::footstepvfx read_footstepvfx(const FootstepVFX& native);
		void write_footstepvfx(FootstepVFX& native, const asset_data::gameplay::footstepvfx& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_footstepvfx(const asset_data::gameplay::footstepvfx& data);
		asset_data::gameplay::xcompositemodel read_xcompositemodel(const XCompositeModelDef& native);
		void write_xcompositemodel(XCompositeModelDef& native, const asset_data::gameplay::xcompositemodel& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_xcompositemodel(const asset_data::gameplay::xcompositemodel& data);
		asset_data::gameplay::x_composite_submodel read_x_composite_submodel(const XCompositeSubmodel& native);
		void write_x_composite_submodel(XCompositeSubmodel& native, const asset_data::gameplay::x_composite_submodel& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_x_composite_submodel(const asset_data::gameplay::x_composite_submodel& data);
		asset_data::gameplay::x_composite_sub_composite_model read_x_composite_sub_composite_model(const XCompositeSubCompositeModel& native);
		void write_x_composite_sub_composite_model(XCompositeSubCompositeModel& native, const asset_data::gameplay::x_composite_sub_composite_model& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_x_composite_sub_composite_model(const asset_data::gameplay::x_composite_sub_composite_model& data);
		asset_data::gameplay::xanimcurve read_xanimcurve(const XAnimCurve& native);
		void write_xanimcurve(XAnimCurve& native, const asset_data::gameplay::xanimcurve& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_xanimcurve(const asset_data::gameplay::xanimcurve& data);
		asset_data::gameplay::x_anim_curve_control_point read_x_anim_curve_control_point(const XAnimCurveControlPoint& native);
		void write_x_anim_curve_control_point(XAnimCurveControlPoint& native, const asset_data::gameplay::x_anim_curve_control_point& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_x_anim_curve_control_point(const asset_data::gameplay::x_anim_curve_control_point& data);
		asset_data::gameplay::keyvaluepairs read_keyvaluepairs(const KeyValuePairs& native);
		void write_keyvaluepairs(KeyValuePairs& native, const asset_data::gameplay::keyvaluepairs& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_keyvaluepairs(const asset_data::gameplay::keyvaluepairs& data);
		asset_data::gameplay::key_value_pair read_key_value_pair(const KeyValuePair& native);
		void write_key_value_pair(KeyValuePair& native, const asset_data::gameplay::key_value_pair& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_key_value_pair(const asset_data::gameplay::key_value_pair& data);
		asset_data::gameplay::blendspace2d read_blendspace2d(const BlendSpace2DDef& native);
		void write_blendspace2d(BlendSpace2DDef& native, const asset_data::gameplay::blendspace2d& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_blendspace2d(const asset_data::gameplay::blendspace2d& data);
		asset_data::gameplay::blend_space2_d_anim_entry read_blend_space2_d_anim_entry(const BlendSpace2DAnimEntry& native);
		void write_blend_space2_d_anim_entry(BlendSpace2DAnimEntry& native, const asset_data::gameplay::blend_space2_d_anim_entry& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_blend_space2_d_anim_entry(const asset_data::gameplay::blend_space2_d_anim_entry& data);
		asset_data::gameplay::camo read_camo(const Camo& native);
		void write_camo(Camo& native, const asset_data::gameplay::camo& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_camo(const asset_data::gameplay::camo& data);
		asset_data::gameplay::material_texture_def read_material_texture_def(const MaterialTextureDef& native);
		void write_material_texture_def(MaterialTextureDef& native, const asset_data::gameplay::material_texture_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_material_texture_def(const asset_data::gameplay::material_texture_def& data);
		asset_data::gameplay::accessory read_accessory(const AccessoryDef& native);
		void write_accessory(AccessoryDef& native, const asset_data::gameplay::accessory& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_accessory(const asset_data::gameplay::accessory& data);
		asset_data::gameplay::locdmgtable read_locdmgtable(const LocDmgTable& native);
		void write_locdmgtable(LocDmgTable& native, const asset_data::gameplay::locdmgtable& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_locdmgtable(const asset_data::gameplay::locdmgtable& data);
		asset_data::gameplay::bulletpenetration read_bulletpenetration(const BulletPenetration& native);
		void write_bulletpenetration(BulletPenetration& native, const asset_data::gameplay::bulletpenetration& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_bulletpenetration(const asset_data::gameplay::bulletpenetration& data);
		asset_data::gameplay::cinematicmotion read_cinematicmotion(const CinematicMotionDef& native);
		void write_cinematicmotion(CinematicMotionDef& native, const asset_data::gameplay::cinematicmotion& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_cinematicmotion(const asset_data::gameplay::cinematicmotion& data);
		asset_data::gameplay::cinematic_shake_def read_cinematic_shake_def(const CinematicShakeDef& native);
		void write_cinematic_shake_def(CinematicShakeDef& native, const asset_data::gameplay::cinematic_shake_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_cinematic_shake_def(const asset_data::gameplay::cinematic_shake_def& data);
		asset_data::gameplay::cinematic_shake_modifier_def read_cinematic_shake_modifier_def(const CinematicShakeModifierDef& native);
		void write_cinematic_shake_modifier_def(CinematicShakeModifierDef& native, const asset_data::gameplay::cinematic_shake_modifier_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_cinematic_shake_modifier_def(const asset_data::gameplay::cinematic_shake_modifier_def& data);
		asset_data::gameplay::cinematic_impact_shake_def read_cinematic_impact_shake_def(const CinematicImpactShakeDef& native);
		void write_cinematic_impact_shake_def(CinematicImpactShakeDef& native, const asset_data::gameplay::cinematic_impact_shake_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_cinematic_impact_shake_def(const asset_data::gameplay::cinematic_impact_shake_def& data);
		asset_data::gameplay::decalvolumemask read_decalvolumemask(const GfxDecalVolumeMask& native);
		void write_decalvolumemask(GfxDecalVolumeMask& native, const asset_data::gameplay::decalvolumemask& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_decalvolumemask(const asset_data::gameplay::decalvolumemask& data);
		asset_data::gameplay::gesture read_gesture(const Gesture& native);
		void write_gesture(Gesture& native, const asset_data::gameplay::gesture& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gesture(const asset_data::gameplay::gesture& data);
		asset_data::gameplay::gesture_weapon_settings read_gesture_weapon_settings(const GestureWeaponSettings& native);
		void write_gesture_weapon_settings(GestureWeaponSettings& native, const asset_data::gameplay::gesture_weapon_settings& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gesture_weapon_settings(const asset_data::gameplay::gesture_weapon_settings& data);
		asset_data::gameplay::gesture_directional_settings read_gesture_directional_settings(const GestureDirectionalSettings& native);
		void write_gesture_directional_settings(GestureDirectionalSettings& native, const asset_data::gameplay::gesture_directional_settings& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gesture_directional_settings(const asset_data::gameplay::gesture_directional_settings& data);
		asset_data::gameplay::gesture_look_around_settings read_gesture_look_around_settings(const GestureLookAroundSettings& native);
		void write_gesture_look_around_settings(GestureLookAroundSettings& native, const asset_data::gameplay::gesture_look_around_settings& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gesture_look_around_settings(const asset_data::gameplay::gesture_look_around_settings& data);
		asset_data::gameplay::gesture_ik_target_settings read_gesture_ik_target_settings(const GestureIKTargetSettings& native);
		void write_gesture_ik_target_settings(GestureIKTargetSettings& native, const asset_data::gameplay::gesture_ik_target_settings& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gesture_ik_target_settings(const asset_data::gameplay::gesture_ik_target_settings& data);
		asset_data::gameplay::reticle read_reticle(const ReticleDef& native);
		void write_reticle(ReticleDef& native, const asset_data::gameplay::reticle& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_reticle(const asset_data::gameplay::reticle& data);
		asset_data::gameplay::streaminginfo read_streaminginfo(const StreamingInfo& native);
		void write_streaminginfo(StreamingInfo& native, const asset_data::gameplay::streaminginfo& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_streaminginfo(const asset_data::gameplay::streaminginfo& data);
		asset_data::gameplay::alwaysloaded_flag_set read_alwaysloaded_flag_set(const AlwaysloadedFlagSet& native);
		void write_alwaysloaded_flag_set(AlwaysloadedFlagSet& native, const asset_data::gameplay::alwaysloaded_flag_set& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_alwaysloaded_flag_set(const asset_data::gameplay::alwaysloaded_flag_set& data);
		asset_data::gameplay::alwaysloaded_flags read_alwaysloaded_flags(const AlwaysloadedFlags& native);
		void write_alwaysloaded_flags(AlwaysloadedFlags& native, const asset_data::gameplay::alwaysloaded_flags& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_alwaysloaded_flags(const asset_data::gameplay::alwaysloaded_flags& data);
		asset_data::gameplay::alwaysloaded_flags_per_model read_alwaysloaded_flags_per_model(const AlwaysloadedFlagsPerModel& native);
		void write_alwaysloaded_flags_per_model(AlwaysloadedFlagsPerModel& native, const asset_data::gameplay::alwaysloaded_flags_per_model& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_alwaysloaded_flags_per_model(const asset_data::gameplay::alwaysloaded_flags_per_model& data);
		asset_data::gameplay::transient_info read_transient_info(const TransientInfo& native);
		void write_transient_info(TransientInfo& native, const asset_data::gameplay::transient_info& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_transient_info(const asset_data::gameplay::transient_info& data);
		asset_data::gameplay::transient_sets read_transient_sets(const TransientSets& native);
		void write_transient_sets(TransientSets& native, const asset_data::gameplay::transient_sets& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_transient_sets(const asset_data::gameplay::transient_sets& data);
		asset_data::gameplay::bitarray_32 read_bitarray_32(const bitarray_32& native);
		void write_bitarray_32(bitarray_32& native, const asset_data::gameplay::bitarray_32& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_bitarray_32(const asset_data::gameplay::bitarray_32& data);
		asset_data::gameplay::transient_costs read_transient_costs(const TransientCosts& native);
		void write_transient_costs(TransientCosts& native, const asset_data::gameplay::transient_costs& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_transient_costs(const asset_data::gameplay::transient_costs& data);
		asset_data::gameplay::streaming_content_set read_streaming_content_set(const StreamingContentSet& native);
		void write_streaming_content_set(StreamingContentSet& native, const asset_data::gameplay::streaming_content_set& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_streaming_content_set(const asset_data::gameplay::streaming_content_set& data);
		asset_data::gameplay::streaming_item_cost read_streaming_item_cost(const StreamingItemCost& native);
		void write_streaming_item_cost(StreamingItemCost& native, const asset_data::gameplay::streaming_item_cost& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_streaming_item_cost(const asset_data::gameplay::streaming_item_cost& data);
		asset_data::gameplay::forced_stub_asset_reference read_forced_stub_asset_reference(const ForcedStubAssetReference& native);
		void write_forced_stub_asset_reference(ForcedStubAssetReference& native, const asset_data::gameplay::forced_stub_asset_reference& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_forced_stub_asset_reference(const asset_data::gameplay::forced_stub_asset_reference& data);
		asset_data::gameplay::transient_file_reference read_transient_file_reference(const TransientFileReference& native);
		void write_transient_file_reference(TransientFileReference& native, const asset_data::gameplay::transient_file_reference& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_transient_file_reference(const asset_data::gameplay::transient_file_reference& data);
		asset_data::gameplay::transient_asset_reference read_transient_asset_reference(const TransientAssetReference& native);
		void write_transient_asset_reference(TransientAssetReference& native, const asset_data::gameplay::transient_asset_reference& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_transient_asset_reference(const asset_data::gameplay::transient_asset_reference& data);
		asset_data::gameplay::transient_world_grid_tile_map read_transient_world_grid_tile_map(const TransientWorldGridTileMap& native);
		void write_transient_world_grid_tile_map(TransientWorldGridTileMap& native, const asset_data::gameplay::transient_world_grid_tile_map& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_transient_world_grid_tile_map(const asset_data::gameplay::transient_world_grid_tile_map& data);
		asset_data::gameplay::spatial_partition_tree read_spatial_partition_tree(const SpatialPartition_Tree& native);
		void write_spatial_partition_tree(SpatialPartition_Tree& native, const asset_data::gameplay::spatial_partition_tree& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_spatial_partition_tree(const asset_data::gameplay::spatial_partition_tree& data);
		asset_data::gameplay::bounds read_bounds(const Bounds& native);
		void write_bounds(Bounds& native, const asset_data::gameplay::bounds& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_bounds(const asset_data::gameplay::bounds& data);
		asset_data::gameplay::bitarray_dynamic read_bitarray_dynamic(const bitarray_dynamic& native);
		void write_bitarray_dynamic(bitarray_dynamic& native, const asset_data::gameplay::bitarray_dynamic& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_bitarray_dynamic(const asset_data::gameplay::bitarray_dynamic& data);
		asset_data::gameplay::suit read_suit(const SuitDef& native);
		void write_suit(SuitDef& native, const asset_data::gameplay::suit& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_suit(const asset_data::gameplay::suit& data);
		asset_data::gameplay::suitanimpackage read_suitanimpackage(const SuitAnimPackage& native);
		void write_suitanimpackage(SuitAnimPackage& native, const asset_data::gameplay::suitanimpackage& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_suitanimpackage(const asset_data::gameplay::suitanimpackage& data);
		asset_data::gameplay::suit_anim_per_weap_class read_suit_anim_per_weap_class(const SuitAnimPerWeapClass& native);
		void write_suit_anim_per_weap_class(SuitAnimPerWeapClass& native, const asset_data::gameplay::suit_anim_per_weap_class& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_suit_anim_per_weap_class(const asset_data::gameplay::suit_anim_per_weap_class& data);
		asset_data::gameplay::suit_anim_entry read_suit_anim_entry(const SuitAnimEntry& native);
		void write_suit_anim_entry(SuitAnimEntry& native, const asset_data::gameplay::suit_anim_entry& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_suit_anim_entry(const asset_data::gameplay::suit_anim_entry& data);
		asset_data::gameplay::suit_gesture_entry read_suit_gesture_entry(const SuitGestureEntry& native);
		void write_suit_gesture_entry(SuitGestureEntry& native, const asset_data::gameplay::suit_gesture_entry& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_suit_gesture_entry(const asset_data::gameplay::suit_gesture_entry& data);
		asset_data::gameplay::coverselector read_coverselector(const CoverSelector& native);
		void write_coverselector(CoverSelector& native, const asset_data::gameplay::coverselector& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_coverselector(const asset_data::gameplay::coverselector& data);
		asset_data::gameplay::enemyselector read_enemyselector(const EnemySelector& native);
		void write_enemyselector(EnemySelector& native, const asset_data::gameplay::enemyselector& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_enemyselector(const asset_data::gameplay::enemyselector& data);
		asset_data::gameplay::carryobject read_carryobject(const CarryObjectDef& native);
		void write_carryobject(CarryObjectDef& native, const asset_data::gameplay::carryobject& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_carryobject(const asset_data::gameplay::carryobject& data);
		asset_data::gameplay::execution read_execution(const ExecutionDef& native);
		void write_execution(ExecutionDef& native, const asset_data::gameplay::execution& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_execution(const asset_data::gameplay::execution& data);
		asset_data::gameplay::clientcharacter read_clientcharacter(const ClientCharacter& native);
		void write_clientcharacter(ClientCharacter& native, const asset_data::gameplay::clientcharacter& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_clientcharacter(const asset_data::gameplay::clientcharacter& data);
		asset_data::gameplay::physicslibrary read_physicslibrary(const PhysicsLibrary& native);
		void write_physicslibrary(PhysicsLibrary& native, const asset_data::gameplay::physicslibrary& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physicslibrary(const asset_data::gameplay::physicslibrary& data);
		asset_data::gameplay::ttf read_ttf(const TTFDef& native);
		void write_ttf(TTFDef& native, const asset_data::gameplay::ttf& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_ttf(const asset_data::gameplay::ttf& data);

		asset_data::gameplay::weapon_anim_package_state_timers read_weapon_anim_package_state_timers(const WeaponAnimPackageStateTimers& native)
		{
			plain::trace("read_weapon_anim_package_state_timers");
			asset_data::gameplay::weapon_anim_package_state_timers data;
			data.i_detonate_delay = native.iDetonateDelay;
			data.i_first_raise_time = native.iFirstRaiseTime;
			data.i_alt_raise_time = native.iAltRaiseTime;
			data.i_alt_raise_ads_time = native.iAltRaiseADSTime;
			data.i_alt_raise_akimbo_time = native.iAltRaiseAkimboTime;
			data.i_rechamber_time = native.iRechamberTime;
			data.rechamber_time_one_handed = native.rechamberTimeOneHanded;
			data.i_rechamber_bolt_time = native.iRechamberBoltTime;
			data.i_hold_fire_time = native.iHoldFireTime;
			data.i_hold_prime_time = native.iHoldPrimeTime;
			data.i_detonate_time = native.iDetonateTime;
			data.i_melee_time = native.iMeleeTime;
			data.melee_charge_time = native.meleeChargeTime;
			data.melee_charge_view_and_movement_lock_time = native.meleeChargeViewAndMovementLockTime;
			data.melee_slice_damage_time = native.meleeSliceDamageTime;
			data.melee_slice_interrupt_time = native.meleeSliceInterruptTime;
			data.melee_slice_view_and_movement_lock_time = native.meleeSliceViewAndMovementLockTime;
			data.melee_re_slice_interrupt_time = native.meleeReSliceInterruptTime;
			data.melee_slice_standard_interrupt_time = native.meleeSliceStandardInterruptTime;
			data.melee_charge_damage_time = native.meleeChargeDamageTime;
			data.melee_charge_interrupt_time = native.meleeChargeInterruptTime;
			data.melee_re_charge_interrupt_time = native.meleeReChargeInterruptTime;
			data.melee_charge_standard_interrupt_time = native.meleeChargeStandardInterruptTime;
			data.alt_melee_time = native.altMeleeTime;
			data.alt_melee_charge_time = native.altMeleeChargeTime;
			data.alt_melee_charge_view_and_movement_lock_time = native.altMeleeChargeViewAndMovementLockTime;
			data.alt_melee_slice_damage_time = native.altMeleeSliceDamageTime;
			data.alt_melee_slice_interrupt_time = native.altMeleeSliceInterruptTime;
			data.alt_melee_slice_view_and_movement_lock_time = native.altMeleeSliceViewAndMovementLockTime;
			data.alt_melee_re_slice_interrupt_time = native.altMeleeReSliceInterruptTime;
			data.alt_melee_slice_standard_interrupt_time = native.altMeleeSliceStandardInterruptTime;
			data.alt_melee_charge_damage_time = native.altMeleeChargeDamageTime;
			data.alt_melee_charge_interrupt_time = native.altMeleeChargeInterruptTime;
			data.alt_melee_re_charge_interrupt_time = native.altMeleeReChargeInterruptTime;
			data.alt_melee_charge_standard_interrupt_time = native.altMeleeChargeStandardInterruptTime;
			data.i_reload_time = native.iReloadTime;
			data.i_fast_reload_time = native.iFastReloadTime;
			data.reload_show_rocket_time = native.reloadShowRocketTime;
			data.i_reload_empty_time = native.iReloadEmptyTime;
			data.i_fast_reload_empty_time = native.iFastReloadEmptyTime;
			data.i_reload_add_time = native.iReloadAddTime;
			data.i_fast_reload_add_time = native.iFastReloadAddTime;
			data.i_reload_add_second_time = native.iReloadAddSecondTime;
			data.i_fast_reload_add_second_time = native.iFastReloadAddSecondTime;
			data.i_reload_empty_add_time = native.iReloadEmptyAddTime;
			data.i_fast_reload_empty_add_time = native.iFastReloadEmptyAddTime;
			data.i_alt_override3_p_in_time = native.iAltOverride3PInTime;
			data.i_alt_override3_p_out_time = native.iAltOverride3POutTime;
			data.i_reload_start_time = native.iReloadStartTime;
			data.i_fast_reload_start_time = native.iFastReloadStartTime;
			data.i_reload_start_add_time = native.iReloadStartAddTime;
			data.i_fast_reload_start_add_time = native.iFastReloadStartAddTime;
			data.i_reload_start_add_second_time = native.iReloadStartAddSecondTime;
			data.i_fast_reload_start_add_second_time = native.iFastReloadStartAddSecondTime;
			data.i_reload_end_time = native.iReloadEndTime;
			data.i_reload_end_interrupted_time = native.iReloadEndInterruptedTime;
			data.i_reload_end_rechamber_time = native.iReloadEndRechamberTime;
			data.i_reload_end_rechamber_shell_eject_time = native.iReloadEndRechamberShellEjectTime;
			data.i_reload_end_rechamber_interrupted_time = native.iReloadEndRechamberInterruptedTime;
			data.i_reload_end_rechamber_interrupted_shell_eject_time = native.iReloadEndRechamberInterruptedShellEjectTime;
			data.i_fast_reload_end_time = native.iFastReloadEndTime;
			data.i_fast_reload_end_interrupted_time = native.iFastReloadEndInterruptedTime;
			data.i_fast_reload_end_rechamber_time = native.iFastReloadEndRechamberTime;
			data.i_fast_reload_end_rechamber_shell_eject_time = native.iFastReloadEndRechamberShellEjectTime;
			data.i_fast_reload_end_rechamber_interrupted_time = native.iFastReloadEndRechamberInterruptedTime;
			data.i_fast_reload_end_rechamber_interrupted_shell_eject_time = native.iFastReloadEndRechamberInterruptedShellEjectTime;
			data.i_drop_time = native.iDropTime;
			data.i_raise_time = native.iRaiseTime;
			data.i_alt_drop_time = native.iAltDropTime;
			data.i_alt_drop_ads_time = native.iAltDropADSTime;
			data.i_alt_drop_akimbo_time = native.iAltDropAkimboTime;
			data.quick_drop_time = native.quickDropTime;
			data.quick_raise_time = native.quickRaiseTime;
			data.i_breach_raise_time = native.iBreachRaiseTime;
			data.i_empty_raise_time = native.iEmptyRaiseTime;
			data.i_empty_drop_time = native.iEmptyDropTime;
			data.i_ladder_vault_time = native.iLadderVaultTime;
			data.i_ladder_climb_raise_time = native.iLadderClimbRaiseTime;
			data.i_ladder_climb_drop_time = native.iLadderClimbDropTime;
			data.i_ladder_aim_raise_time = native.iLadderAimRaiseTime;
			data.i_ladder_aim_raise_interrupt_time = native.iLadderAimRaiseInterruptTime;
			data.i_ladder_aim_ads_raise_time = native.iLadderAimAdsRaiseTime;
			data.i_ladder_aim_ads_raise_interrupt_time = native.iLadderAimAdsRaiseInterruptTime;
			data.i_ladder_aim_drop_time = native.iLadderAimDropTime;
			data.i_ladder_aim_drop_interrupt_time = native.iLadderAimDropInterruptTime;
			data.sprint_in_time = native.sprintInTime;
			data.sprint_loop_time = native.sprintLoopTime;
			data.sprint_out_time = native.sprintOutTime;
			data.walk_time = native.walkTime;
			data.gung_ho_time = native.gungHoTime;
			data.crawl_time = native.crawlTime;
			data.stunned_time_begin = native.stunnedTimeBegin;
			data.stunned_time_loop = native.stunnedTimeLoop;
			data.stunned_time_end = native.stunnedTimeEnd;
			data.prime_time = native.primeTime;
			data.b_hold_full_prime = native.bHoldFullPrime;
			data.b_alt_override3_pads_only = native.bAltOverride3PADSOnly;
			data.blast_front_time = native.blastFrontTime;
			data.blast_right_time = native.blastRightTime;
			data.blast_back_time = native.blastBackTime;
			data.blast_left_time = native.blastLeftTime;
			data.slide_in_time = native.slideInTime;
			data.slide_loop_time = native.slideLoopTime;
			data.slide_out_time = native.slideOutTime;
			data.dodge_time = native.dodgeTime;
			data.wall_run_in_time = native.wallRunInTime;
			data.wall_run_out_time = native.wallRunOutTime;
			data.leap_in_time = native.leapInTime;
			data.leap_loop_time = native.leapLoopTime;
			data.leap_out_time = native.leapOutTime;
			data.leap_cancel_time = native.leapCancelTime;
			data.charge_in_time = native.chargeInTime;
			data.charge_loop_time = native.chargeLoopTime;
			data.charge_out_time = native.chargeOutTime;
			data.i_regeneration_time = native.iRegenerationTime;
			data.i_regeneration_add_time = native.iRegenerationAddTime;
			data.pre_fire_time = native.preFireTime;
			data.i_cycle_walk = native.iCycleWalk;
			data.i_cycle_jog = native.iCycleJog;
			data.i_cycle_sprint = native.iCycleSprint;
			data.i_cycle_super_sprint = native.iCycleSuperSprint;
			data.i_transition_to_walk = native.iTransitionToWalk;
			data.i_transition_to_jog = native.iTransitionToJog;
			data.i_transition_to_sprint = native.iTransitionToSprint;
			data.i_transition_to_super_sprint = native.iTransitionToSuperSprint;
			data.i_transition_down_from_sprint = native.iTransitionDownFromSprint;
			data.i_transition_down_from_super_sprint = native.iTransitionDownFromSuperSprint;
			data.skydive_freefall_raise_time = native.skydiveFreefallRaiseTime;
			data.scope_toggle_on_time = native.scopeToggleOnTime;
			data.scope_toggle_off_time = native.scopeToggleOffTime;
			data.i_reload_interrupt_time = native.iReloadInterruptTime;
			data.i_reload_empty_interrupt_time = native.iReloadEmptyInterruptTime;
			data.i_reload_start_interrupt_time = native.iReloadStartInterruptTime;
			data.i_reload_end_interrupt_time = native.iReloadEndInterruptTime;
			data.i_reload_end_interrupted_interrupt_time = native.iReloadEndInterruptedInterruptTime;
			data.i_reload_end_rechamber_interrupt_time = native.iReloadEndRechamberInterruptTime;
			data.i_reload_end_rechamber_interrupted_interrupt_time = native.iReloadEndRechamberInterruptedInterruptTime;
			data.i_reload_multiple1_interrupt_time = native.iReloadMultiple1InterruptTime;
			data.i_reload_multiple2_interrupt_time = native.iReloadMultiple2InterruptTime;
			data.i_reload_multiple3_interrupt_time = native.iReloadMultiple3InterruptTime;
			data.i_reload_multiple4_interrupt_time = native.iReloadMultiple4InterruptTime;
			data.i_reload_multiple5_interrupt_time = native.iReloadMultiple5InterruptTime;
			data.i_reload_multiple6_interrupt_time = native.iReloadMultiple6InterruptTime;
			data.i_reload_multiple7_interrupt_time = native.iReloadMultiple7InterruptTime;
			data.i_reload_multiple8_interrupt_time = native.iReloadMultiple8InterruptTime;
			data.i_fast_reload_interrupt_time = native.iFastReloadInterruptTime;
			data.i_fast_reload_empty_interrupt_time = native.iFastReloadEmptyInterruptTime;
			data.i_fast_reload_start_interrupt_time = native.iFastReloadStartInterruptTime;
			data.i_fast_reload_end_interrupt_time = native.iFastReloadEndInterruptTime;
			data.i_fast_reload_end_interrupted_interrupt_time = native.iFastReloadEndInterruptedInterruptTime;
			data.i_fast_reload_end_rechamber_interrupt_time = native.iFastReloadEndRechamberInterruptTime;
			data.i_fast_reload_end_rechamber_interrupted_interrupt_time = native.iFastReloadEndRechamberInterruptedInterruptTime;
			data.i_fast_reload_multiple1_interrupt_time = native.iFastReloadMultiple1InterruptTime;
			data.i_fast_reload_multiple2_interrupt_time = native.iFastReloadMultiple2InterruptTime;
			data.i_fast_reload_multiple3_interrupt_time = native.iFastReloadMultiple3InterruptTime;
			data.i_fast_reload_multiple4_interrupt_time = native.iFastReloadMultiple4InterruptTime;
			data.i_fast_reload_multiple5_interrupt_time = native.iFastReloadMultiple5InterruptTime;
			data.i_fast_reload_multiple6_interrupt_time = native.iFastReloadMultiple6InterruptTime;
			data.i_fast_reload_multiple7_interrupt_time = native.iFastReloadMultiple7InterruptTime;
			data.i_fast_reload_multiple8_interrupt_time = native.iFastReloadMultiple8InterruptTime;
			data.i_raise_start_disable_interrupt_time = native.iRaiseStartDisableInterruptTime;
			data.i_raise_interrupt_time = native.iRaiseInterruptTime;
			data.i_first_raise_start_disable_interrupt_time = native.iFirstRaiseStartDisableInterruptTime;
			data.i_first_raise_interrupt_time = native.iFirstRaiseInterruptTime;
			data.i_quick_raise_start_disable_interrupt_time = native.iQuickRaiseStartDisableInterruptTime;
			data.i_quick_raise_interrupt_time = native.iQuickRaiseInterruptTime;
			data.i_empty_raise_start_disable_interrupt_time = native.iEmptyRaiseStartDisableInterruptTime;
			data.i_empty_raise_interrupt_time = native.iEmptyRaiseInterruptTime;
			data.i_alt_raise_interrupt_time = native.iAltRaiseInterruptTime;
			data.i_alt_raise_ads_interrupt_time = native.iAltRaiseADSInterruptTime;
			data.i_alt_raise_akimbo_interrupt_time = native.iAltRaiseAkimboInterruptTime;
			data.i_sprint_out_interrupt_time = native.iSprintOutInterruptTime;
			data.hold_fire_interrupt_time = native.holdFireInterruptTime;
			data.max_blend_to_fire = native.maxBlendToFire;
			data.min_blend_to_fire = native.minBlendToFire;
			data.max_blend_alt_raise_to_fire = native.maxBlendAltRaiseToFire;
			data.min_blend_alt_raise_to_fire = native.minBlendAltRaiseToFire;
			data.i_transition_down_from_sprint_interrupt = native.iTransitionDownFromSprintInterrupt;
			data.i_transition_down_from_super_sprint_interrupt = native.iTransitionDownFromSuperSprintInterrupt;
			data.scope_toggle_on_interrupt_time = native.scopeToggleOnInterruptTime;
			data.scope_toggle_off_interrupt_time = native.scopeToggleOffInterruptTime;
			data.inspect_time = native.inspectTime;
			data.i_dlc_anim0_time = native.iDlcAnim0Time;
			data.i_dlc_anim1_time = native.iDlcAnim1Time;
			data.i_dlc_anim2_time = native.iDlcAnim2Time;
			data.i_dlc_anim3_time = native.iDlcAnim3Time;
			return data;
		}

		void write_weapon_anim_package_state_timers(WeaponAnimPackageStateTimers& native, const asset_data::gameplay::weapon_anim_package_state_timers& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.iDetonateDelay = data.i_detonate_delay;
			native.iFirstRaiseTime = data.i_first_raise_time;
			native.iAltRaiseTime = data.i_alt_raise_time;
			native.iAltRaiseADSTime = data.i_alt_raise_ads_time;
			native.iAltRaiseAkimboTime = data.i_alt_raise_akimbo_time;
			native.iRechamberTime = data.i_rechamber_time;
			native.rechamberTimeOneHanded = data.rechamber_time_one_handed;
			native.iRechamberBoltTime = data.i_rechamber_bolt_time;
			native.iHoldFireTime = data.i_hold_fire_time;
			native.iHoldPrimeTime = data.i_hold_prime_time;
			native.iDetonateTime = data.i_detonate_time;
			native.iMeleeTime = data.i_melee_time;
			native.meleeChargeTime = data.melee_charge_time;
			native.meleeChargeViewAndMovementLockTime = data.melee_charge_view_and_movement_lock_time;
			native.meleeSliceDamageTime = data.melee_slice_damage_time;
			native.meleeSliceInterruptTime = data.melee_slice_interrupt_time;
			native.meleeSliceViewAndMovementLockTime = data.melee_slice_view_and_movement_lock_time;
			native.meleeReSliceInterruptTime = data.melee_re_slice_interrupt_time;
			native.meleeSliceStandardInterruptTime = data.melee_slice_standard_interrupt_time;
			native.meleeChargeDamageTime = data.melee_charge_damage_time;
			native.meleeChargeInterruptTime = data.melee_charge_interrupt_time;
			native.meleeReChargeInterruptTime = data.melee_re_charge_interrupt_time;
			native.meleeChargeStandardInterruptTime = data.melee_charge_standard_interrupt_time;
			native.altMeleeTime = data.alt_melee_time;
			native.altMeleeChargeTime = data.alt_melee_charge_time;
			native.altMeleeChargeViewAndMovementLockTime = data.alt_melee_charge_view_and_movement_lock_time;
			native.altMeleeSliceDamageTime = data.alt_melee_slice_damage_time;
			native.altMeleeSliceInterruptTime = data.alt_melee_slice_interrupt_time;
			native.altMeleeSliceViewAndMovementLockTime = data.alt_melee_slice_view_and_movement_lock_time;
			native.altMeleeReSliceInterruptTime = data.alt_melee_re_slice_interrupt_time;
			native.altMeleeSliceStandardInterruptTime = data.alt_melee_slice_standard_interrupt_time;
			native.altMeleeChargeDamageTime = data.alt_melee_charge_damage_time;
			native.altMeleeChargeInterruptTime = data.alt_melee_charge_interrupt_time;
			native.altMeleeReChargeInterruptTime = data.alt_melee_re_charge_interrupt_time;
			native.altMeleeChargeStandardInterruptTime = data.alt_melee_charge_standard_interrupt_time;
			native.iReloadTime = data.i_reload_time;
			native.iFastReloadTime = data.i_fast_reload_time;
			native.reloadShowRocketTime = data.reload_show_rocket_time;
			native.iReloadEmptyTime = data.i_reload_empty_time;
			native.iFastReloadEmptyTime = data.i_fast_reload_empty_time;
			native.iReloadAddTime = data.i_reload_add_time;
			native.iFastReloadAddTime = data.i_fast_reload_add_time;
			native.iReloadAddSecondTime = data.i_reload_add_second_time;
			native.iFastReloadAddSecondTime = data.i_fast_reload_add_second_time;
			native.iReloadEmptyAddTime = data.i_reload_empty_add_time;
			native.iFastReloadEmptyAddTime = data.i_fast_reload_empty_add_time;
			native.iAltOverride3PInTime = data.i_alt_override3_p_in_time;
			native.iAltOverride3POutTime = data.i_alt_override3_p_out_time;
			native.iReloadStartTime = data.i_reload_start_time;
			native.iFastReloadStartTime = data.i_fast_reload_start_time;
			native.iReloadStartAddTime = data.i_reload_start_add_time;
			native.iFastReloadStartAddTime = data.i_fast_reload_start_add_time;
			native.iReloadStartAddSecondTime = data.i_reload_start_add_second_time;
			native.iFastReloadStartAddSecondTime = data.i_fast_reload_start_add_second_time;
			native.iReloadEndTime = data.i_reload_end_time;
			native.iReloadEndInterruptedTime = data.i_reload_end_interrupted_time;
			native.iReloadEndRechamberTime = data.i_reload_end_rechamber_time;
			native.iReloadEndRechamberShellEjectTime = data.i_reload_end_rechamber_shell_eject_time;
			native.iReloadEndRechamberInterruptedTime = data.i_reload_end_rechamber_interrupted_time;
			native.iReloadEndRechamberInterruptedShellEjectTime = data.i_reload_end_rechamber_interrupted_shell_eject_time;
			native.iFastReloadEndTime = data.i_fast_reload_end_time;
			native.iFastReloadEndInterruptedTime = data.i_fast_reload_end_interrupted_time;
			native.iFastReloadEndRechamberTime = data.i_fast_reload_end_rechamber_time;
			native.iFastReloadEndRechamberShellEjectTime = data.i_fast_reload_end_rechamber_shell_eject_time;
			native.iFastReloadEndRechamberInterruptedTime = data.i_fast_reload_end_rechamber_interrupted_time;
			native.iFastReloadEndRechamberInterruptedShellEjectTime = data.i_fast_reload_end_rechamber_interrupted_shell_eject_time;
			native.iDropTime = data.i_drop_time;
			native.iRaiseTime = data.i_raise_time;
			native.iAltDropTime = data.i_alt_drop_time;
			native.iAltDropADSTime = data.i_alt_drop_ads_time;
			native.iAltDropAkimboTime = data.i_alt_drop_akimbo_time;
			native.quickDropTime = data.quick_drop_time;
			native.quickRaiseTime = data.quick_raise_time;
			native.iBreachRaiseTime = data.i_breach_raise_time;
			native.iEmptyRaiseTime = data.i_empty_raise_time;
			native.iEmptyDropTime = data.i_empty_drop_time;
			native.iLadderVaultTime = data.i_ladder_vault_time;
			native.iLadderClimbRaiseTime = data.i_ladder_climb_raise_time;
			native.iLadderClimbDropTime = data.i_ladder_climb_drop_time;
			native.iLadderAimRaiseTime = data.i_ladder_aim_raise_time;
			native.iLadderAimRaiseInterruptTime = data.i_ladder_aim_raise_interrupt_time;
			native.iLadderAimAdsRaiseTime = data.i_ladder_aim_ads_raise_time;
			native.iLadderAimAdsRaiseInterruptTime = data.i_ladder_aim_ads_raise_interrupt_time;
			native.iLadderAimDropTime = data.i_ladder_aim_drop_time;
			native.iLadderAimDropInterruptTime = data.i_ladder_aim_drop_interrupt_time;
			native.sprintInTime = data.sprint_in_time;
			native.sprintLoopTime = data.sprint_loop_time;
			native.sprintOutTime = data.sprint_out_time;
			native.walkTime = data.walk_time;
			native.gungHoTime = data.gung_ho_time;
			native.crawlTime = data.crawl_time;
			native.stunnedTimeBegin = data.stunned_time_begin;
			native.stunnedTimeLoop = data.stunned_time_loop;
			native.stunnedTimeEnd = data.stunned_time_end;
			native.primeTime = data.prime_time;
			native.bHoldFullPrime = data.b_hold_full_prime;
			native.bAltOverride3PADSOnly = data.b_alt_override3_pads_only;
			native.blastFrontTime = data.blast_front_time;
			native.blastRightTime = data.blast_right_time;
			native.blastBackTime = data.blast_back_time;
			native.blastLeftTime = data.blast_left_time;
			native.slideInTime = data.slide_in_time;
			native.slideLoopTime = data.slide_loop_time;
			native.slideOutTime = data.slide_out_time;
			native.dodgeTime = data.dodge_time;
			native.wallRunInTime = data.wall_run_in_time;
			native.wallRunOutTime = data.wall_run_out_time;
			native.leapInTime = data.leap_in_time;
			native.leapLoopTime = data.leap_loop_time;
			native.leapOutTime = data.leap_out_time;
			native.leapCancelTime = data.leap_cancel_time;
			native.chargeInTime = data.charge_in_time;
			native.chargeLoopTime = data.charge_loop_time;
			native.chargeOutTime = data.charge_out_time;
			native.iRegenerationTime = data.i_regeneration_time;
			native.iRegenerationAddTime = data.i_regeneration_add_time;
			native.preFireTime = data.pre_fire_time;
			native.iCycleWalk = data.i_cycle_walk;
			native.iCycleJog = data.i_cycle_jog;
			native.iCycleSprint = data.i_cycle_sprint;
			native.iCycleSuperSprint = data.i_cycle_super_sprint;
			native.iTransitionToWalk = data.i_transition_to_walk;
			native.iTransitionToJog = data.i_transition_to_jog;
			native.iTransitionToSprint = data.i_transition_to_sprint;
			native.iTransitionToSuperSprint = data.i_transition_to_super_sprint;
			native.iTransitionDownFromSprint = data.i_transition_down_from_sprint;
			native.iTransitionDownFromSuperSprint = data.i_transition_down_from_super_sprint;
			native.skydiveFreefallRaiseTime = data.skydive_freefall_raise_time;
			native.scopeToggleOnTime = data.scope_toggle_on_time;
			native.scopeToggleOffTime = data.scope_toggle_off_time;
			native.iReloadInterruptTime = data.i_reload_interrupt_time;
			native.iReloadEmptyInterruptTime = data.i_reload_empty_interrupt_time;
			native.iReloadStartInterruptTime = data.i_reload_start_interrupt_time;
			native.iReloadEndInterruptTime = data.i_reload_end_interrupt_time;
			native.iReloadEndInterruptedInterruptTime = data.i_reload_end_interrupted_interrupt_time;
			native.iReloadEndRechamberInterruptTime = data.i_reload_end_rechamber_interrupt_time;
			native.iReloadEndRechamberInterruptedInterruptTime = data.i_reload_end_rechamber_interrupted_interrupt_time;
			native.iReloadMultiple1InterruptTime = data.i_reload_multiple1_interrupt_time;
			native.iReloadMultiple2InterruptTime = data.i_reload_multiple2_interrupt_time;
			native.iReloadMultiple3InterruptTime = data.i_reload_multiple3_interrupt_time;
			native.iReloadMultiple4InterruptTime = data.i_reload_multiple4_interrupt_time;
			native.iReloadMultiple5InterruptTime = data.i_reload_multiple5_interrupt_time;
			native.iReloadMultiple6InterruptTime = data.i_reload_multiple6_interrupt_time;
			native.iReloadMultiple7InterruptTime = data.i_reload_multiple7_interrupt_time;
			native.iReloadMultiple8InterruptTime = data.i_reload_multiple8_interrupt_time;
			native.iFastReloadInterruptTime = data.i_fast_reload_interrupt_time;
			native.iFastReloadEmptyInterruptTime = data.i_fast_reload_empty_interrupt_time;
			native.iFastReloadStartInterruptTime = data.i_fast_reload_start_interrupt_time;
			native.iFastReloadEndInterruptTime = data.i_fast_reload_end_interrupt_time;
			native.iFastReloadEndInterruptedInterruptTime = data.i_fast_reload_end_interrupted_interrupt_time;
			native.iFastReloadEndRechamberInterruptTime = data.i_fast_reload_end_rechamber_interrupt_time;
			native.iFastReloadEndRechamberInterruptedInterruptTime = data.i_fast_reload_end_rechamber_interrupted_interrupt_time;
			native.iFastReloadMultiple1InterruptTime = data.i_fast_reload_multiple1_interrupt_time;
			native.iFastReloadMultiple2InterruptTime = data.i_fast_reload_multiple2_interrupt_time;
			native.iFastReloadMultiple3InterruptTime = data.i_fast_reload_multiple3_interrupt_time;
			native.iFastReloadMultiple4InterruptTime = data.i_fast_reload_multiple4_interrupt_time;
			native.iFastReloadMultiple5InterruptTime = data.i_fast_reload_multiple5_interrupt_time;
			native.iFastReloadMultiple6InterruptTime = data.i_fast_reload_multiple6_interrupt_time;
			native.iFastReloadMultiple7InterruptTime = data.i_fast_reload_multiple7_interrupt_time;
			native.iFastReloadMultiple8InterruptTime = data.i_fast_reload_multiple8_interrupt_time;
			native.iRaiseStartDisableInterruptTime = data.i_raise_start_disable_interrupt_time;
			native.iRaiseInterruptTime = data.i_raise_interrupt_time;
			native.iFirstRaiseStartDisableInterruptTime = data.i_first_raise_start_disable_interrupt_time;
			native.iFirstRaiseInterruptTime = data.i_first_raise_interrupt_time;
			native.iQuickRaiseStartDisableInterruptTime = data.i_quick_raise_start_disable_interrupt_time;
			native.iQuickRaiseInterruptTime = data.i_quick_raise_interrupt_time;
			native.iEmptyRaiseStartDisableInterruptTime = data.i_empty_raise_start_disable_interrupt_time;
			native.iEmptyRaiseInterruptTime = data.i_empty_raise_interrupt_time;
			native.iAltRaiseInterruptTime = data.i_alt_raise_interrupt_time;
			native.iAltRaiseADSInterruptTime = data.i_alt_raise_ads_interrupt_time;
			native.iAltRaiseAkimboInterruptTime = data.i_alt_raise_akimbo_interrupt_time;
			native.iSprintOutInterruptTime = data.i_sprint_out_interrupt_time;
			native.holdFireInterruptTime = data.hold_fire_interrupt_time;
			native.maxBlendToFire = data.max_blend_to_fire;
			native.minBlendToFire = data.min_blend_to_fire;
			native.maxBlendAltRaiseToFire = data.max_blend_alt_raise_to_fire;
			native.minBlendAltRaiseToFire = data.min_blend_alt_raise_to_fire;
			native.iTransitionDownFromSprintInterrupt = data.i_transition_down_from_sprint_interrupt;
			native.iTransitionDownFromSuperSprintInterrupt = data.i_transition_down_from_super_sprint_interrupt;
			native.scopeToggleOnInterruptTime = data.scope_toggle_on_interrupt_time;
			native.scopeToggleOffInterruptTime = data.scope_toggle_off_interrupt_time;
			native.inspectTime = data.inspect_time;
			native.iDlcAnim0Time = data.i_dlc_anim0_time;
			native.iDlcAnim1Time = data.i_dlc_anim1_time;
			native.iDlcAnim2Time = data.i_dlc_anim2_time;
			native.iDlcAnim3Time = data.i_dlc_anim3_time;
		}

		std::size_t size_weapon_anim_package_state_timers([[maybe_unused]] const asset_data::gameplay::weapon_anim_package_state_timers& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gameplay::footstep_time read_footstep_time(const FootstepTime& native)
		{
			plain::trace("read_footstep_time");
			asset_data::gameplay::footstep_time data;
			data.time = native.time;
			data.is_left = native.isLeft;
			return data;
		}

		void write_footstep_time(FootstepTime& native, const asset_data::gameplay::footstep_time& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.time = data.time;
			native.isLeft = data.is_left;
		}

		std::size_t size_footstep_time([[maybe_unused]] const asset_data::gameplay::footstep_time& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gameplay::movement_time read_movement_time(const MovementTime& native)
		{
			plain::trace("read_movement_time");
			asset_data::gameplay::movement_time data;
			data.time = native.time;
			data.is_lead_in = native.isLeadIn;
			return data;
		}

		void write_movement_time(MovementTime& native, const asset_data::gameplay::movement_time& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.time = data.time;
			native.isLeadIn = data.is_lead_in;
		}

		std::size_t size_movement_time([[maybe_unused]] const asset_data::gameplay::movement_time& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gameplay::footstep_anim read_footstep_anim(const FootstepAnim& native)
		{
			plain::trace("read_footstep_anim");
			asset_data::gameplay::footstep_anim data;
			data.left_count = native.leftCount;
			data.step = [&] { std::array<asset_data::gameplay::footstep_time, 32> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_footstep_time(native.step[i]); } return out; }();
			data.movement = [&] { std::array<asset_data::gameplay::movement_time, 32> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_movement_time(native.movement[i]); } return out; }();
			return data;
		}

		void write_footstep_anim(FootstepAnim& native, const asset_data::gameplay::footstep_anim& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.leftCount = data.left_count;
			for (std::size_t i = 0; i < data.step.size(); ++i)
			{
				write_footstep_time(native.step[i], data.step[i], resolve, memory);
			}
			for (std::size_t i = 0; i < data.movement.size(); ++i)
			{
				write_movement_time(native.movement[i], data.movement[i], resolve, memory);
			}
		}

		std::size_t size_footstep_anim([[maybe_unused]] const asset_data::gameplay::footstep_anim& data)
		{
			std::size_t size = 0;
			for (const auto& item : data.step)
			{
				size += size_footstep_time(item);
			}
			for (const auto& item : data.movement)
			{
				size += size_movement_time(item);
			}
			return size;
		}

		asset_data::gameplay::animpkg read_animpkg(const WeaponAnimPackage& native)
		{
			plain::trace("read_animpkg");
			asset_data::gameplay::animpkg data;
			data.name = plain::read_string(native.name);
			data.anims = plain::read_fixed_with<614>(native.anims, [&](const auto& item) { return plain::read_asset(ASSET_TYPE_XANIMPARTS, item); });
			data.timers = plain::read_single(native.timers, [&](const auto& item) { return read_weapon_anim_package_state_timers(item); });
			data.melee_anim_primary_type = static_cast<std::uint32_t>(native.meleeAnimPrimaryType);
			data.melee_anim_primary_count = native.meleeAnimPrimaryCount;
			data.melee_anim_primary_set = native.meleeAnimPrimarySet;
			data.melee_randomize_anims = native.meleeRandomizeAnims;
			data.melee_anim_alt_type = static_cast<std::uint32_t>(native.meleeAnimAltType);
			data.melee_anim_alt_count = native.meleeAnimAltCount;
			data.melee_anim_alt_set = native.meleeAnimAltSet;
			data.alt_melee_randomize_anims = native.altMeleeRandomizeAnims;
			data.blend_spaces = plain::read_fixed_with<8>(native.blendSpaces, [&](const auto& item) { return plain::read_asset(ASSET_TYPE_BLENDSPACE2D, item); });
			data.footstep = plain::read_fixed_with<13>(native.footstep, [&](const auto& item) { return read_footstep_anim(item); });
			return data;
		}

		void write_animpkg(WeaponAnimPackage& native, const asset_data::gameplay::animpkg& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			if (data.anims)
			{
				native.anims = memory.manual_allocate<XAnimParts*>(plain::rounded(614 * sizeof(XAnimParts*)));
				for (std::size_t i = 0; i < 614; ++i)
				{
					native.anims[i] = plain::resolve_asset<XAnimParts>(resolve, ASSET_TYPE_XANIMPARTS, (*data.anims)[i]);
				}
			}
			native.timers = plain::write_single<WeaponAnimPackageStateTimers>(data.timers, memory, [&](auto& out, const auto& in) { write_weapon_anim_package_state_timers(out, in, resolve, memory); });
			native.meleeAnimPrimaryType = static_cast<MeleeAnimType>(data.melee_anim_primary_type);
			native.meleeAnimPrimaryCount = data.melee_anim_primary_count;
			native.meleeAnimPrimarySet = data.melee_anim_primary_set;
			native.meleeRandomizeAnims = data.melee_randomize_anims;
			native.meleeAnimAltType = static_cast<MeleeAnimType>(data.melee_anim_alt_type);
			native.meleeAnimAltCount = data.melee_anim_alt_count;
			native.meleeAnimAltSet = data.melee_anim_alt_set;
			native.altMeleeRandomizeAnims = data.alt_melee_randomize_anims;
			if (data.blend_spaces)
			{
				native.blendSpaces = memory.manual_allocate<BlendSpace2DDef*>(plain::rounded(8 * sizeof(BlendSpace2DDef*)));
				for (std::size_t i = 0; i < 8; ++i)
				{
					native.blendSpaces[i] = plain::resolve_asset<BlendSpace2DDef>(resolve, ASSET_TYPE_BLENDSPACE2D, (*data.blend_spaces)[i]);
				}
			}
			if (data.footstep)
			{
				native.footstep = memory.manual_allocate<FootstepAnim>(plain::rounded(13 * sizeof(FootstepAnim)));
				for (std::size_t i = 0; i < 13; ++i)
				{
					write_footstep_anim(native.footstep[i], (*data.footstep)[i], resolve, memory);
				}
			}
		}

		std::size_t size_animpkg([[maybe_unused]] const asset_data::gameplay::animpkg& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.anims)
			{
				size += plain::sized(614 * sizeof(XAnimParts*));
			}
			if (data.timers)
			{
				size += plain::sized(sizeof(WeaponAnimPackageStateTimers));
				size += size_weapon_anim_package_state_timers(*data.timers);
			}
			if (data.blend_spaces)
			{
				size += plain::sized(8 * sizeof(BlendSpace2DDef*));
			}
			if (data.footstep)
			{
				size += plain::sized(13 * sizeof(FootstepAnim));
				for (const auto& item : *data.footstep)
				{
					size += size_footstep_anim(item);
				}
			}
			return size;
		}

		asset_data::gameplay::snd_alias_lookup read_snd_alias_lookup(const SndAliasLookup& native)
		{
			plain::trace("read_snd_alias_lookup");
			asset_data::gameplay::snd_alias_lookup data;
			data.name = plain::read_string(native.name);
			return data;
		}

		void write_snd_alias_lookup(SndAliasLookup& native, const asset_data::gameplay::snd_alias_lookup& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
		}

		std::size_t size_snd_alias_lookup([[maybe_unused]] const asset_data::gameplay::snd_alias_lookup& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			return size;
		}

		asset_data::gameplay::weapon_sfx_package_sounds read_weapon_sfx_package_sounds(const WeaponSFXPackageSounds& native)
		{
			plain::trace("read_weapon_sfx_package_sounds");
			asset_data::gameplay::weapon_sfx_package_sounds data;
			data.name = plain::read_string(native.name);
			data.pickup_sound = read_snd_alias_lookup(native.pickupSound);
			data.pickup_sound_player = read_snd_alias_lookup(native.pickupSoundPlayer);
			data.ammo_pickup_sound = read_snd_alias_lookup(native.ammoPickupSound);
			data.ammo_pickup_sound_player = read_snd_alias_lookup(native.ammoPickupSoundPlayer);
			data.projectile_sound = read_snd_alias_lookup(native.projectileSound);
			data.pullback_sound = read_snd_alias_lookup(native.pullbackSound);
			data.pullback_sound_player = read_snd_alias_lookup(native.pullbackSoundPlayer);
			data.pullback_sound_quick = read_snd_alias_lookup(native.pullbackSoundQuick);
			data.pullback_sound_quick_player = read_snd_alias_lookup(native.pullbackSoundQuickPlayer);
			data.pre_fire_sound = read_snd_alias_lookup(native.preFireSound);
			data.pre_fire_sound_player = read_snd_alias_lookup(native.preFireSoundPlayer);
			data.fire_sound = read_snd_alias_lookup(native.fireSound);
			data.fire_sound_player = read_snd_alias_lookup(native.fireSoundPlayer);
			data.fire_sound_player_akimbo = read_snd_alias_lookup(native.fireSoundPlayerAkimbo);
			data.fire_sound_player_akimbo_right = read_snd_alias_lookup(native.fireSoundPlayerAkimboRight);
			data.fire_sound_atmosphere = read_snd_alias_lookup(native.fireSoundAtmosphere);
			data.fire_sound_atmosphere_player = read_snd_alias_lookup(native.fireSoundAtmospherePlayer);
			data.fire_sound_atmosphere_player_akimbo = read_snd_alias_lookup(native.fireSoundAtmospherePlayerAkimbo);
			data.fire_sound_atmosphere_player_akimbo_right = read_snd_alias_lookup(native.fireSoundAtmospherePlayerAkimboRight);
			data.fire_start_sound = read_snd_alias_lookup(native.fireStartSound);
			data.fire_start_sound_player = read_snd_alias_lookup(native.fireStartSoundPlayer);
			data.fire_loop_sound = read_snd_alias_lookup(native.fireLoopSound);
			data.fire_loop_sound_player = read_snd_alias_lookup(native.fireLoopSoundPlayer);
			data.fire_stop_sound = read_snd_alias_lookup(native.fireStopSound);
			data.fire_stop_sound_player = read_snd_alias_lookup(native.fireStopSoundPlayer);
			data.fire_first_sound = read_snd_alias_lookup(native.fireFirstSound);
			data.fire_first_sound_player = read_snd_alias_lookup(native.fireFirstSoundPlayer);
			data.fire_first_sound_player_akimbo = read_snd_alias_lookup(native.fireFirstSoundPlayerAkimbo);
			data.fire_first_sound_player_akimbo_right = read_snd_alias_lookup(native.fireFirstSoundPlayerAkimboRight);
			data.fire_hyper_burst_sound = read_snd_alias_lookup(native.fireHyperBurstSound);
			data.fire_hyper_burst_sound_player = read_snd_alias_lookup(native.fireHyperBurstSoundPlayer);
			data.fire_hyper_burst_sound_player_akimbo = read_snd_alias_lookup(native.fireHyperBurstSoundPlayerAkimbo);
			data.fire_hyper_burst_sound_player_akimbo_right = read_snd_alias_lookup(native.fireHyperBurstSoundPlayerAkimboRight);
			data.fire_last_sound = read_snd_alias_lookup(native.fireLastSound);
			data.fire_last_sound_player = read_snd_alias_lookup(native.fireLastSoundPlayer);
			data.fire_last_sound_player_akimbo = read_snd_alias_lookup(native.fireLastSoundPlayerAkimbo);
			data.fire_last_sound_player_akimbo_right = read_snd_alias_lookup(native.fireLastSoundPlayerAkimboRight);
			data.regenerate_sound = read_snd_alias_lookup(native.regenerateSound);
			data.regenerate_sound_player = read_snd_alias_lookup(native.regenerateSoundPlayer);
			data.regenerate_add_sound = read_snd_alias_lookup(native.regenerateAddSound);
			data.regenerate_add_sound_player = read_snd_alias_lookup(native.regenerateAddSoundPlayer);
			data.empty_fire_sound = read_snd_alias_lookup(native.emptyFireSound);
			data.empty_fire_sound_player = read_snd_alias_lookup(native.emptyFireSoundPlayer);
			data.empty_regen_fire_sound = read_snd_alias_lookup(native.emptyRegenFireSound);
			data.empty_regen_fire_sound_player = read_snd_alias_lookup(native.emptyRegenFireSoundPlayer);
			data.rechamber_sound = read_snd_alias_lookup(native.rechamberSound);
			data.rechamber_sound_player = read_snd_alias_lookup(native.rechamberSoundPlayer);
			data.reload_sound = read_snd_alias_lookup(native.reloadSound);
			data.reload_sound_player = read_snd_alias_lookup(native.reloadSoundPlayer);
			data.reload_empty_sound = read_snd_alias_lookup(native.reloadEmptySound);
			data.reload_empty_sound_player = read_snd_alias_lookup(native.reloadEmptySoundPlayer);
			data.reload_start_sound = read_snd_alias_lookup(native.reloadStartSound);
			data.reload_start_sound_player = read_snd_alias_lookup(native.reloadStartSoundPlayer);
			data.reload_end_sound = read_snd_alias_lookup(native.reloadEndSound);
			data.reload_end_sound_player = read_snd_alias_lookup(native.reloadEndSoundPlayer);
			data.reload_end_rechamber_sound = read_snd_alias_lookup(native.reloadEndRechamberSound);
			data.reload_end_rechamber_sound_player = read_snd_alias_lookup(native.reloadEndRechamberSoundPlayer);
			data.detonate_sound = read_snd_alias_lookup(native.detonateSound);
			data.detonate_sound_player = read_snd_alias_lookup(native.detonateSoundPlayer);
			data.night_vision_wear_sound = read_snd_alias_lookup(native.nightVisionWearSound);
			data.night_vision_wear_sound_player = read_snd_alias_lookup(native.nightVisionWearSoundPlayer);
			data.night_vision_remove_sound = read_snd_alias_lookup(native.nightVisionRemoveSound);
			data.night_vision_remove_sound_player = read_snd_alias_lookup(native.nightVisionRemoveSoundPlayer);
			data.alt_switch_sound = read_snd_alias_lookup(native.altSwitchSound);
			data.alt_switch_sound_player = read_snd_alias_lookup(native.altSwitchSoundPlayer);
			data.raise_sound = read_snd_alias_lookup(native.raiseSound);
			data.raise_sound_player = read_snd_alias_lookup(native.raiseSoundPlayer);
			data.first_raise_sound = read_snd_alias_lookup(native.firstRaiseSound);
			data.first_raise_sound_player = read_snd_alias_lookup(native.firstRaiseSoundPlayer);
			data.putaway_sound = read_snd_alias_lookup(native.putawaySound);
			data.putaway_sound_player = read_snd_alias_lookup(native.putawaySoundPlayer);
			data.enter_ads_sound = read_snd_alias_lookup(native.enterAdsSound);
			data.enter_ads_sound_player = read_snd_alias_lookup(native.enterAdsSoundPlayer);
			data.leave_ads_sound = read_snd_alias_lookup(native.leaveAdsSound);
			data.leave_ads_sound_player = read_snd_alias_lookup(native.leaveAdsSoundPlayer);
			data.scan_sound = read_snd_alias_lookup(native.scanSound);
			data.change_variable_zoom_sound = read_snd_alias_lookup(native.changeVariableZoomSound);
			data.bullet_ricochet_impact = read_snd_alias_lookup(native.bulletRicochetImpact);
			data.proj_explosion_sound = read_snd_alias_lookup(native.projExplosionSound);
			data.proj_dud_sound = read_snd_alias_lookup(native.projDudSound);
			data.proj_timeout_sound = read_snd_alias_lookup(native.projTimeoutSound);
			data.proj_ignition_sound = read_snd_alias_lookup(native.projIgnitionSound);
			data.bounce_sound = read_snd_alias_lookup(native.bounceSound);
			data.bounce_sound_heavy = read_snd_alias_lookup(native.bounceSoundHeavy);
			data.bounce_sound_medium = read_snd_alias_lookup(native.bounceSoundMedium);
			data.bounce_sound_soft = read_snd_alias_lookup(native.bounceSoundSoft);
			data.rolling_sound = read_snd_alias_lookup(native.rollingSound);
			data.turret_spin_snd = read_snd_alias_lookup(native.turretSpinSnd);
			data.turret_stop_snd = read_snd_alias_lookup(native.turretStopSnd);
			data.trigger_disconnect_sound = read_snd_alias_lookup(native.triggerDisconnectSound);
			data.trigger_disconnect_sound_player_akimbo = read_snd_alias_lookup(native.triggerDisconnectSoundPlayerAkimbo);
			data.trigger_disconnect_sound_player_akimbo_right = read_snd_alias_lookup(native.triggerDisconnectSoundPlayerAkimboRight);
			data.dlc_sound0 = read_snd_alias_lookup(native.dlcSound0);
			data.dlc_sound0_player = read_snd_alias_lookup(native.dlcSound0Player);
			data.dlc_sound1 = read_snd_alias_lookup(native.dlcSound1);
			data.dlc_sound1_player = read_snd_alias_lookup(native.dlcSound1Player);
			return data;
		}

		void write_weapon_sfx_package_sounds(WeaponSFXPackageSounds& native, const asset_data::gameplay::weapon_sfx_package_sounds& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			write_snd_alias_lookup(native.pickupSound, data.pickup_sound, resolve, memory);
			write_snd_alias_lookup(native.pickupSoundPlayer, data.pickup_sound_player, resolve, memory);
			write_snd_alias_lookup(native.ammoPickupSound, data.ammo_pickup_sound, resolve, memory);
			write_snd_alias_lookup(native.ammoPickupSoundPlayer, data.ammo_pickup_sound_player, resolve, memory);
			write_snd_alias_lookup(native.projectileSound, data.projectile_sound, resolve, memory);
			write_snd_alias_lookup(native.pullbackSound, data.pullback_sound, resolve, memory);
			write_snd_alias_lookup(native.pullbackSoundPlayer, data.pullback_sound_player, resolve, memory);
			write_snd_alias_lookup(native.pullbackSoundQuick, data.pullback_sound_quick, resolve, memory);
			write_snd_alias_lookup(native.pullbackSoundQuickPlayer, data.pullback_sound_quick_player, resolve, memory);
			write_snd_alias_lookup(native.preFireSound, data.pre_fire_sound, resolve, memory);
			write_snd_alias_lookup(native.preFireSoundPlayer, data.pre_fire_sound_player, resolve, memory);
			write_snd_alias_lookup(native.fireSound, data.fire_sound, resolve, memory);
			write_snd_alias_lookup(native.fireSoundPlayer, data.fire_sound_player, resolve, memory);
			write_snd_alias_lookup(native.fireSoundPlayerAkimbo, data.fire_sound_player_akimbo, resolve, memory);
			write_snd_alias_lookup(native.fireSoundPlayerAkimboRight, data.fire_sound_player_akimbo_right, resolve, memory);
			write_snd_alias_lookup(native.fireSoundAtmosphere, data.fire_sound_atmosphere, resolve, memory);
			write_snd_alias_lookup(native.fireSoundAtmospherePlayer, data.fire_sound_atmosphere_player, resolve, memory);
			write_snd_alias_lookup(native.fireSoundAtmospherePlayerAkimbo, data.fire_sound_atmosphere_player_akimbo, resolve, memory);
			write_snd_alias_lookup(native.fireSoundAtmospherePlayerAkimboRight, data.fire_sound_atmosphere_player_akimbo_right, resolve, memory);
			write_snd_alias_lookup(native.fireStartSound, data.fire_start_sound, resolve, memory);
			write_snd_alias_lookup(native.fireStartSoundPlayer, data.fire_start_sound_player, resolve, memory);
			write_snd_alias_lookup(native.fireLoopSound, data.fire_loop_sound, resolve, memory);
			write_snd_alias_lookup(native.fireLoopSoundPlayer, data.fire_loop_sound_player, resolve, memory);
			write_snd_alias_lookup(native.fireStopSound, data.fire_stop_sound, resolve, memory);
			write_snd_alias_lookup(native.fireStopSoundPlayer, data.fire_stop_sound_player, resolve, memory);
			write_snd_alias_lookup(native.fireFirstSound, data.fire_first_sound, resolve, memory);
			write_snd_alias_lookup(native.fireFirstSoundPlayer, data.fire_first_sound_player, resolve, memory);
			write_snd_alias_lookup(native.fireFirstSoundPlayerAkimbo, data.fire_first_sound_player_akimbo, resolve, memory);
			write_snd_alias_lookup(native.fireFirstSoundPlayerAkimboRight, data.fire_first_sound_player_akimbo_right, resolve, memory);
			write_snd_alias_lookup(native.fireHyperBurstSound, data.fire_hyper_burst_sound, resolve, memory);
			write_snd_alias_lookup(native.fireHyperBurstSoundPlayer, data.fire_hyper_burst_sound_player, resolve, memory);
			write_snd_alias_lookup(native.fireHyperBurstSoundPlayerAkimbo, data.fire_hyper_burst_sound_player_akimbo, resolve, memory);
			write_snd_alias_lookup(native.fireHyperBurstSoundPlayerAkimboRight, data.fire_hyper_burst_sound_player_akimbo_right, resolve, memory);
			write_snd_alias_lookup(native.fireLastSound, data.fire_last_sound, resolve, memory);
			write_snd_alias_lookup(native.fireLastSoundPlayer, data.fire_last_sound_player, resolve, memory);
			write_snd_alias_lookup(native.fireLastSoundPlayerAkimbo, data.fire_last_sound_player_akimbo, resolve, memory);
			write_snd_alias_lookup(native.fireLastSoundPlayerAkimboRight, data.fire_last_sound_player_akimbo_right, resolve, memory);
			write_snd_alias_lookup(native.regenerateSound, data.regenerate_sound, resolve, memory);
			write_snd_alias_lookup(native.regenerateSoundPlayer, data.regenerate_sound_player, resolve, memory);
			write_snd_alias_lookup(native.regenerateAddSound, data.regenerate_add_sound, resolve, memory);
			write_snd_alias_lookup(native.regenerateAddSoundPlayer, data.regenerate_add_sound_player, resolve, memory);
			write_snd_alias_lookup(native.emptyFireSound, data.empty_fire_sound, resolve, memory);
			write_snd_alias_lookup(native.emptyFireSoundPlayer, data.empty_fire_sound_player, resolve, memory);
			write_snd_alias_lookup(native.emptyRegenFireSound, data.empty_regen_fire_sound, resolve, memory);
			write_snd_alias_lookup(native.emptyRegenFireSoundPlayer, data.empty_regen_fire_sound_player, resolve, memory);
			write_snd_alias_lookup(native.rechamberSound, data.rechamber_sound, resolve, memory);
			write_snd_alias_lookup(native.rechamberSoundPlayer, data.rechamber_sound_player, resolve, memory);
			write_snd_alias_lookup(native.reloadSound, data.reload_sound, resolve, memory);
			write_snd_alias_lookup(native.reloadSoundPlayer, data.reload_sound_player, resolve, memory);
			write_snd_alias_lookup(native.reloadEmptySound, data.reload_empty_sound, resolve, memory);
			write_snd_alias_lookup(native.reloadEmptySoundPlayer, data.reload_empty_sound_player, resolve, memory);
			write_snd_alias_lookup(native.reloadStartSound, data.reload_start_sound, resolve, memory);
			write_snd_alias_lookup(native.reloadStartSoundPlayer, data.reload_start_sound_player, resolve, memory);
			write_snd_alias_lookup(native.reloadEndSound, data.reload_end_sound, resolve, memory);
			write_snd_alias_lookup(native.reloadEndSoundPlayer, data.reload_end_sound_player, resolve, memory);
			write_snd_alias_lookup(native.reloadEndRechamberSound, data.reload_end_rechamber_sound, resolve, memory);
			write_snd_alias_lookup(native.reloadEndRechamberSoundPlayer, data.reload_end_rechamber_sound_player, resolve, memory);
			write_snd_alias_lookup(native.detonateSound, data.detonate_sound, resolve, memory);
			write_snd_alias_lookup(native.detonateSoundPlayer, data.detonate_sound_player, resolve, memory);
			write_snd_alias_lookup(native.nightVisionWearSound, data.night_vision_wear_sound, resolve, memory);
			write_snd_alias_lookup(native.nightVisionWearSoundPlayer, data.night_vision_wear_sound_player, resolve, memory);
			write_snd_alias_lookup(native.nightVisionRemoveSound, data.night_vision_remove_sound, resolve, memory);
			write_snd_alias_lookup(native.nightVisionRemoveSoundPlayer, data.night_vision_remove_sound_player, resolve, memory);
			write_snd_alias_lookup(native.altSwitchSound, data.alt_switch_sound, resolve, memory);
			write_snd_alias_lookup(native.altSwitchSoundPlayer, data.alt_switch_sound_player, resolve, memory);
			write_snd_alias_lookup(native.raiseSound, data.raise_sound, resolve, memory);
			write_snd_alias_lookup(native.raiseSoundPlayer, data.raise_sound_player, resolve, memory);
			write_snd_alias_lookup(native.firstRaiseSound, data.first_raise_sound, resolve, memory);
			write_snd_alias_lookup(native.firstRaiseSoundPlayer, data.first_raise_sound_player, resolve, memory);
			write_snd_alias_lookup(native.putawaySound, data.putaway_sound, resolve, memory);
			write_snd_alias_lookup(native.putawaySoundPlayer, data.putaway_sound_player, resolve, memory);
			write_snd_alias_lookup(native.enterAdsSound, data.enter_ads_sound, resolve, memory);
			write_snd_alias_lookup(native.enterAdsSoundPlayer, data.enter_ads_sound_player, resolve, memory);
			write_snd_alias_lookup(native.leaveAdsSound, data.leave_ads_sound, resolve, memory);
			write_snd_alias_lookup(native.leaveAdsSoundPlayer, data.leave_ads_sound_player, resolve, memory);
			write_snd_alias_lookup(native.scanSound, data.scan_sound, resolve, memory);
			write_snd_alias_lookup(native.changeVariableZoomSound, data.change_variable_zoom_sound, resolve, memory);
			write_snd_alias_lookup(native.bulletRicochetImpact, data.bullet_ricochet_impact, resolve, memory);
			write_snd_alias_lookup(native.projExplosionSound, data.proj_explosion_sound, resolve, memory);
			write_snd_alias_lookup(native.projDudSound, data.proj_dud_sound, resolve, memory);
			write_snd_alias_lookup(native.projTimeoutSound, data.proj_timeout_sound, resolve, memory);
			write_snd_alias_lookup(native.projIgnitionSound, data.proj_ignition_sound, resolve, memory);
			write_snd_alias_lookup(native.bounceSound, data.bounce_sound, resolve, memory);
			write_snd_alias_lookup(native.bounceSoundHeavy, data.bounce_sound_heavy, resolve, memory);
			write_snd_alias_lookup(native.bounceSoundMedium, data.bounce_sound_medium, resolve, memory);
			write_snd_alias_lookup(native.bounceSoundSoft, data.bounce_sound_soft, resolve, memory);
			write_snd_alias_lookup(native.rollingSound, data.rolling_sound, resolve, memory);
			write_snd_alias_lookup(native.turretSpinSnd, data.turret_spin_snd, resolve, memory);
			write_snd_alias_lookup(native.turretStopSnd, data.turret_stop_snd, resolve, memory);
			write_snd_alias_lookup(native.triggerDisconnectSound, data.trigger_disconnect_sound, resolve, memory);
			write_snd_alias_lookup(native.triggerDisconnectSoundPlayerAkimbo, data.trigger_disconnect_sound_player_akimbo, resolve, memory);
			write_snd_alias_lookup(native.triggerDisconnectSoundPlayerAkimboRight, data.trigger_disconnect_sound_player_akimbo_right, resolve, memory);
			write_snd_alias_lookup(native.dlcSound0, data.dlc_sound0, resolve, memory);
			write_snd_alias_lookup(native.dlcSound0Player, data.dlc_sound0_player, resolve, memory);
			write_snd_alias_lookup(native.dlcSound1, data.dlc_sound1, resolve, memory);
			write_snd_alias_lookup(native.dlcSound1Player, data.dlc_sound1_player, resolve, memory);
		}

		std::size_t size_weapon_sfx_package_sounds([[maybe_unused]] const asset_data::gameplay::weapon_sfx_package_sounds& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			size += size_snd_alias_lookup(data.pickup_sound);
			size += size_snd_alias_lookup(data.pickup_sound_player);
			size += size_snd_alias_lookup(data.ammo_pickup_sound);
			size += size_snd_alias_lookup(data.ammo_pickup_sound_player);
			size += size_snd_alias_lookup(data.projectile_sound);
			size += size_snd_alias_lookup(data.pullback_sound);
			size += size_snd_alias_lookup(data.pullback_sound_player);
			size += size_snd_alias_lookup(data.pullback_sound_quick);
			size += size_snd_alias_lookup(data.pullback_sound_quick_player);
			size += size_snd_alias_lookup(data.pre_fire_sound);
			size += size_snd_alias_lookup(data.pre_fire_sound_player);
			size += size_snd_alias_lookup(data.fire_sound);
			size += size_snd_alias_lookup(data.fire_sound_player);
			size += size_snd_alias_lookup(data.fire_sound_player_akimbo);
			size += size_snd_alias_lookup(data.fire_sound_player_akimbo_right);
			size += size_snd_alias_lookup(data.fire_sound_atmosphere);
			size += size_snd_alias_lookup(data.fire_sound_atmosphere_player);
			size += size_snd_alias_lookup(data.fire_sound_atmosphere_player_akimbo);
			size += size_snd_alias_lookup(data.fire_sound_atmosphere_player_akimbo_right);
			size += size_snd_alias_lookup(data.fire_start_sound);
			size += size_snd_alias_lookup(data.fire_start_sound_player);
			size += size_snd_alias_lookup(data.fire_loop_sound);
			size += size_snd_alias_lookup(data.fire_loop_sound_player);
			size += size_snd_alias_lookup(data.fire_stop_sound);
			size += size_snd_alias_lookup(data.fire_stop_sound_player);
			size += size_snd_alias_lookup(data.fire_first_sound);
			size += size_snd_alias_lookup(data.fire_first_sound_player);
			size += size_snd_alias_lookup(data.fire_first_sound_player_akimbo);
			size += size_snd_alias_lookup(data.fire_first_sound_player_akimbo_right);
			size += size_snd_alias_lookup(data.fire_hyper_burst_sound);
			size += size_snd_alias_lookup(data.fire_hyper_burst_sound_player);
			size += size_snd_alias_lookup(data.fire_hyper_burst_sound_player_akimbo);
			size += size_snd_alias_lookup(data.fire_hyper_burst_sound_player_akimbo_right);
			size += size_snd_alias_lookup(data.fire_last_sound);
			size += size_snd_alias_lookup(data.fire_last_sound_player);
			size += size_snd_alias_lookup(data.fire_last_sound_player_akimbo);
			size += size_snd_alias_lookup(data.fire_last_sound_player_akimbo_right);
			size += size_snd_alias_lookup(data.regenerate_sound);
			size += size_snd_alias_lookup(data.regenerate_sound_player);
			size += size_snd_alias_lookup(data.regenerate_add_sound);
			size += size_snd_alias_lookup(data.regenerate_add_sound_player);
			size += size_snd_alias_lookup(data.empty_fire_sound);
			size += size_snd_alias_lookup(data.empty_fire_sound_player);
			size += size_snd_alias_lookup(data.empty_regen_fire_sound);
			size += size_snd_alias_lookup(data.empty_regen_fire_sound_player);
			size += size_snd_alias_lookup(data.rechamber_sound);
			size += size_snd_alias_lookup(data.rechamber_sound_player);
			size += size_snd_alias_lookup(data.reload_sound);
			size += size_snd_alias_lookup(data.reload_sound_player);
			size += size_snd_alias_lookup(data.reload_empty_sound);
			size += size_snd_alias_lookup(data.reload_empty_sound_player);
			size += size_snd_alias_lookup(data.reload_start_sound);
			size += size_snd_alias_lookup(data.reload_start_sound_player);
			size += size_snd_alias_lookup(data.reload_end_sound);
			size += size_snd_alias_lookup(data.reload_end_sound_player);
			size += size_snd_alias_lookup(data.reload_end_rechamber_sound);
			size += size_snd_alias_lookup(data.reload_end_rechamber_sound_player);
			size += size_snd_alias_lookup(data.detonate_sound);
			size += size_snd_alias_lookup(data.detonate_sound_player);
			size += size_snd_alias_lookup(data.night_vision_wear_sound);
			size += size_snd_alias_lookup(data.night_vision_wear_sound_player);
			size += size_snd_alias_lookup(data.night_vision_remove_sound);
			size += size_snd_alias_lookup(data.night_vision_remove_sound_player);
			size += size_snd_alias_lookup(data.alt_switch_sound);
			size += size_snd_alias_lookup(data.alt_switch_sound_player);
			size += size_snd_alias_lookup(data.raise_sound);
			size += size_snd_alias_lookup(data.raise_sound_player);
			size += size_snd_alias_lookup(data.first_raise_sound);
			size += size_snd_alias_lookup(data.first_raise_sound_player);
			size += size_snd_alias_lookup(data.putaway_sound);
			size += size_snd_alias_lookup(data.putaway_sound_player);
			size += size_snd_alias_lookup(data.enter_ads_sound);
			size += size_snd_alias_lookup(data.enter_ads_sound_player);
			size += size_snd_alias_lookup(data.leave_ads_sound);
			size += size_snd_alias_lookup(data.leave_ads_sound_player);
			size += size_snd_alias_lookup(data.scan_sound);
			size += size_snd_alias_lookup(data.change_variable_zoom_sound);
			size += size_snd_alias_lookup(data.bullet_ricochet_impact);
			size += size_snd_alias_lookup(data.proj_explosion_sound);
			size += size_snd_alias_lookup(data.proj_dud_sound);
			size += size_snd_alias_lookup(data.proj_timeout_sound);
			size += size_snd_alias_lookup(data.proj_ignition_sound);
			size += size_snd_alias_lookup(data.bounce_sound);
			size += size_snd_alias_lookup(data.bounce_sound_heavy);
			size += size_snd_alias_lookup(data.bounce_sound_medium);
			size += size_snd_alias_lookup(data.bounce_sound_soft);
			size += size_snd_alias_lookup(data.rolling_sound);
			size += size_snd_alias_lookup(data.turret_spin_snd);
			size += size_snd_alias_lookup(data.turret_stop_snd);
			size += size_snd_alias_lookup(data.trigger_disconnect_sound);
			size += size_snd_alias_lookup(data.trigger_disconnect_sound_player_akimbo);
			size += size_snd_alias_lookup(data.trigger_disconnect_sound_player_akimbo_right);
			size += size_snd_alias_lookup(data.dlc_sound0);
			size += size_snd_alias_lookup(data.dlc_sound0_player);
			size += size_snd_alias_lookup(data.dlc_sound1);
			size += size_snd_alias_lookup(data.dlc_sound1_player);
			return size;
		}

		asset_data::gameplay::trigger_disconnect_sound_data read_trigger_disconnect_sound_data(const TriggerDisconnectSoundData& native)
		{
			plain::trace("read_trigger_disconnect_sound_data");
			asset_data::gameplay::trigger_disconnect_sound_data data;
			data.analog_trigger_value = native.analogTriggerValue;
			data.enabled_for_fire_type = std::to_array(native.enabledForFireType);
			return data;
		}

		void write_trigger_disconnect_sound_data(TriggerDisconnectSoundData& native, const asset_data::gameplay::trigger_disconnect_sound_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.analogTriggerValue = data.analog_trigger_value;
			std::ranges::copy(data.enabled_for_fire_type, native.enabledForFireType);
		}

		std::size_t size_trigger_disconnect_sound_data([[maybe_unused]] const asset_data::gameplay::trigger_disconnect_sound_data& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gameplay::sfxpkg read_sfxpkg(const WeaponSFXPackage& native)
		{
			plain::trace("read_sfxpkg");
			asset_data::gameplay::sfxpkg data;
			data.name = plain::read_string(native.name);
			data.sounds = plain::read_single(native.sounds, [&](const auto& item) { return read_weapon_sfx_package_sounds(item); });
			data.transient_fallback_package = plain::read_asset(ASSET_TYPE_SFX_PACKAGE, native.transientFallbackPackage);
			data.detail_sound_bank_npc = plain::read_asset(ASSET_TYPE_SOUND_BANK_TRANSIENT, native.detailSoundBankNPC);
			data.detail_sound_bank_player = plain::read_asset(ASSET_TYPE_SOUND_BANK_TRANSIENT, native.detailSoundBankPlayer);
			data.rattle_sound_type = native.rattleSoundType;
			data.sz_adsr_base_setting = plain::read_string(native.szAdsrBaseSetting);
			data.sz_weap_snd_reflection_class = plain::read_string(native.szWeapSndReflectionClass);
			data.sz_weap_snd_refl_front_override = plain::read_string(native.szWeapSndReflFrontOverride);
			data.sz_weap_snd_refl_distant_override = plain::read_string(native.szWeapSndReflDistantOverride);
			data.weap_snd_fire_volume_shot1 = native.weapSndFireVolumeShot1;
			data.weap_snd_fire_volume_shot2 = native.weapSndFireVolumeShot2;
			data.weap_snd_fire_volume_shot3 = native.weapSndFireVolumeShot3;
			data.weap_snd_prone_fire_lfe_volume = native.weapSndProneFireLFEVolume;
			data.weap_snd_medium_range_start = native.weapSndMediumRangeStart;
			data.weap_snd_far_range_start = native.weapSndFarRangeStart;
			data.weap_snd_distant_range_start = native.weapSndDistantRangeStart;
			data.proj_explosion_refl_class = native.projExplosionReflClass;
			data.sfx_impact_type = native.sfxImpactType;
			data.sfx_melee_impact_type = plain::enum_name(asset_data::gameplay::melee_impact_type_names, static_cast<std::int64_t>(native.sfxMeleeImpactType), "WeaponSFXPackage.sfxMeleeImpactType");
			data.sfx_material_type = static_cast<std::uint32_t>(native.sfxMaterialType);
			data.sz_mount_enter_exit_string = plain::read_string(native.szMountEnterExitString);
			data.sz_mount_impact_string = plain::read_string(native.szMountImpactString);
			data.whizby_type = native.whizbyType;
			data.ads_context_type = native.adsContextType;
			data.ads_context_value = native.adsContextValue;
			data.speed_of_sound_mult = native.speedOfSoundMult;
			data.trigger_disconnect_sound_data = read_trigger_disconnect_sound_data(native.triggerDisconnectSoundData);
			return data;
		}

		void write_sfxpkg(WeaponSFXPackage& native, const asset_data::gameplay::sfxpkg& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.sounds = plain::write_single<WeaponSFXPackageSounds>(data.sounds, memory, [&](auto& out, const auto& in) { write_weapon_sfx_package_sounds(out, in, resolve, memory); });
			native.transientFallbackPackage = plain::resolve_asset<WeaponSFXPackage>(resolve, ASSET_TYPE_SFX_PACKAGE, data.transient_fallback_package);
			native.detailSoundBankNPC = plain::resolve_asset<SndBankTransient>(resolve, ASSET_TYPE_SOUND_BANK_TRANSIENT, data.detail_sound_bank_npc);
			native.detailSoundBankPlayer = plain::resolve_asset<SndBankTransient>(resolve, ASSET_TYPE_SOUND_BANK_TRANSIENT, data.detail_sound_bank_player);
			native.rattleSoundType = data.rattle_sound_type;
			native.szAdsrBaseSetting = plain::write_string(memory, data.sz_adsr_base_setting);
			native.szWeapSndReflectionClass = plain::write_string(memory, data.sz_weap_snd_reflection_class);
			native.szWeapSndReflFrontOverride = plain::write_string(memory, data.sz_weap_snd_refl_front_override);
			native.szWeapSndReflDistantOverride = plain::write_string(memory, data.sz_weap_snd_refl_distant_override);
			native.weapSndFireVolumeShot1 = data.weap_snd_fire_volume_shot1;
			native.weapSndFireVolumeShot2 = data.weap_snd_fire_volume_shot2;
			native.weapSndFireVolumeShot3 = data.weap_snd_fire_volume_shot3;
			native.weapSndProneFireLFEVolume = data.weap_snd_prone_fire_lfe_volume;
			native.weapSndMediumRangeStart = data.weap_snd_medium_range_start;
			native.weapSndFarRangeStart = data.weap_snd_far_range_start;
			native.weapSndDistantRangeStart = data.weap_snd_distant_range_start;
			native.projExplosionReflClass = data.proj_explosion_refl_class;
			native.sfxImpactType = data.sfx_impact_type;
			native.sfxMeleeImpactType = static_cast<MeleeImpactType>(plain::enum_value(asset_data::gameplay::melee_impact_type_names, data.sfx_melee_impact_type, "WeaponSFXPackage.sfxMeleeImpactType"));
			native.sfxMaterialType = static_cast<WeaponMaterialType>(data.sfx_material_type);
			native.szMountEnterExitString = plain::write_string(memory, data.sz_mount_enter_exit_string);
			native.szMountImpactString = plain::write_string(memory, data.sz_mount_impact_string);
			native.whizbyType = data.whizby_type;
			native.adsContextType = data.ads_context_type;
			native.adsContextValue = data.ads_context_value;
			native.speedOfSoundMult = data.speed_of_sound_mult;
			write_trigger_disconnect_sound_data(native.triggerDisconnectSoundData, data.trigger_disconnect_sound_data, resolve, memory);
		}

		std::size_t size_sfxpkg([[maybe_unused]] const asset_data::gameplay::sfxpkg& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.sounds)
			{
				size += plain::sized(sizeof(WeaponSFXPackageSounds));
				size += size_weapon_sfx_package_sounds(*data.sounds);
			}
			size += plain::string_size(data.sz_adsr_base_setting);
			size += plain::string_size(data.sz_weap_snd_reflection_class);
			size += plain::string_size(data.sz_weap_snd_refl_front_override);
			size += plain::string_size(data.sz_weap_snd_refl_distant_override);
			size += plain::string_size(data.sz_mount_enter_exit_string);
			size += plain::string_size(data.sz_mount_impact_string);
			size += size_trigger_disconnect_sound_data(data.trigger_disconnect_sound_data);
			return size;
		}

		asset_data::gameplay::fx_combined_def read_fx_combined_def(const FxCombinedDef& native)
		{
			plain::trace("read_fx_combined_def");
			asset_data::gameplay::fx_combined_def data;
			data.particle_system_def = plain::read_asset(ASSET_TYPE_VFX, native.particleSystemDef);
			return data;
		}

		void write_fx_combined_def(FxCombinedDef& native, const asset_data::gameplay::fx_combined_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.particleSystemDef = plain::resolve_asset<ParticleSystemDef>(resolve, ASSET_TYPE_VFX, data.particle_system_def);
		}

		std::size_t size_fx_combined_def([[maybe_unused]] const asset_data::gameplay::fx_combined_def& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gameplay::weapon_vfx_package_defs read_weapon_vfx_package_defs(const WeaponVFXPackageDefs& native)
		{
			plain::trace("read_weapon_vfx_package_defs");
			asset_data::gameplay::weapon_vfx_package_defs data;
			data.view_flash_effect = read_fx_combined_def(native.viewFlashEffect);
			data.world_flash_effect = read_fx_combined_def(native.worldFlashEffect);
			data.view_flash_ads_effect = read_fx_combined_def(native.viewFlashADSEffect);
			data.view_flash_first_effect = read_fx_combined_def(native.viewFlashFirstEffect);
			data.world_flash_first_effect = read_fx_combined_def(native.worldFlashFirstEffect);
			data.view_flash_first_ads_effect = read_fx_combined_def(native.viewFlashFirstADSEffect);
			data.view_flash_overcharge_effect = read_fx_combined_def(native.viewFlashOverchargeEffect);
			data.view_flash_overcharge_ads_effect = read_fx_combined_def(native.viewFlashOverchargeADSEffect);
			data.world_flash_overcharge_effect = read_fx_combined_def(native.worldFlashOverchargeEffect);
			data.view_shell_eject_effect = read_fx_combined_def(native.viewShellEjectEffect);
			data.world_shell_eject_effect = read_fx_combined_def(native.worldShellEjectEffect);
			data.view_last_shot_eject_effect = read_fx_combined_def(native.viewLastShotEjectEffect);
			data.world_last_shot_eject_effect = read_fx_combined_def(native.worldLastShotEjectEffect);
			data.view_mag_eject_effect = read_fx_combined_def(native.viewMagEjectEffect);
			data.view_muzzle_smoke_effect = read_fx_combined_def(native.viewMuzzleSmokeEffect);
			data.world_muzzle_smoke_effect = read_fx_combined_def(native.worldMuzzleSmokeEffect);
			data.view_barrel_smoke_effect = read_fx_combined_def(native.viewBarrelSmokeEffect);
			data.world_barrel_smoke_effect = read_fx_combined_def(native.worldBarrelSmokeEffect);
			data.view_ejection_port_smoke_effect = read_fx_combined_def(native.viewEjectionPortSmokeEffect);
			data.world_ejection_port_smoke_effect = read_fx_combined_def(native.worldEjectionPortSmokeEffect);
			data.view_dlc_effect0 = read_fx_combined_def(native.viewDlcEffect0);
			data.world_dlc_effect0 = read_fx_combined_def(native.worldDlcEffect0);
			data.view_dlc_effect1 = read_fx_combined_def(native.viewDlcEffect1);
			data.world_dlc_effect1 = read_fx_combined_def(native.worldDlcEffect1);
			data.view_dlc_effect2 = read_fx_combined_def(native.viewDlcEffect2);
			data.world_dlc_effect2 = read_fx_combined_def(native.worldDlcEffect2);
			data.view_dlc_effect3 = read_fx_combined_def(native.viewDlcEffect3);
			data.world_dlc_effect3 = read_fx_combined_def(native.worldDlcEffect3);
			return data;
		}

		void write_weapon_vfx_package_defs(WeaponVFXPackageDefs& native, const asset_data::gameplay::weapon_vfx_package_defs& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_fx_combined_def(native.viewFlashEffect, data.view_flash_effect, resolve, memory);
			write_fx_combined_def(native.worldFlashEffect, data.world_flash_effect, resolve, memory);
			write_fx_combined_def(native.viewFlashADSEffect, data.view_flash_ads_effect, resolve, memory);
			write_fx_combined_def(native.viewFlashFirstEffect, data.view_flash_first_effect, resolve, memory);
			write_fx_combined_def(native.worldFlashFirstEffect, data.world_flash_first_effect, resolve, memory);
			write_fx_combined_def(native.viewFlashFirstADSEffect, data.view_flash_first_ads_effect, resolve, memory);
			write_fx_combined_def(native.viewFlashOverchargeEffect, data.view_flash_overcharge_effect, resolve, memory);
			write_fx_combined_def(native.viewFlashOverchargeADSEffect, data.view_flash_overcharge_ads_effect, resolve, memory);
			write_fx_combined_def(native.worldFlashOverchargeEffect, data.world_flash_overcharge_effect, resolve, memory);
			write_fx_combined_def(native.viewShellEjectEffect, data.view_shell_eject_effect, resolve, memory);
			write_fx_combined_def(native.worldShellEjectEffect, data.world_shell_eject_effect, resolve, memory);
			write_fx_combined_def(native.viewLastShotEjectEffect, data.view_last_shot_eject_effect, resolve, memory);
			write_fx_combined_def(native.worldLastShotEjectEffect, data.world_last_shot_eject_effect, resolve, memory);
			write_fx_combined_def(native.viewMagEjectEffect, data.view_mag_eject_effect, resolve, memory);
			write_fx_combined_def(native.viewMuzzleSmokeEffect, data.view_muzzle_smoke_effect, resolve, memory);
			write_fx_combined_def(native.worldMuzzleSmokeEffect, data.world_muzzle_smoke_effect, resolve, memory);
			write_fx_combined_def(native.viewBarrelSmokeEffect, data.view_barrel_smoke_effect, resolve, memory);
			write_fx_combined_def(native.worldBarrelSmokeEffect, data.world_barrel_smoke_effect, resolve, memory);
			write_fx_combined_def(native.viewEjectionPortSmokeEffect, data.view_ejection_port_smoke_effect, resolve, memory);
			write_fx_combined_def(native.worldEjectionPortSmokeEffect, data.world_ejection_port_smoke_effect, resolve, memory);
			write_fx_combined_def(native.viewDlcEffect0, data.view_dlc_effect0, resolve, memory);
			write_fx_combined_def(native.worldDlcEffect0, data.world_dlc_effect0, resolve, memory);
			write_fx_combined_def(native.viewDlcEffect1, data.view_dlc_effect1, resolve, memory);
			write_fx_combined_def(native.worldDlcEffect1, data.world_dlc_effect1, resolve, memory);
			write_fx_combined_def(native.viewDlcEffect2, data.view_dlc_effect2, resolve, memory);
			write_fx_combined_def(native.worldDlcEffect2, data.world_dlc_effect2, resolve, memory);
			write_fx_combined_def(native.viewDlcEffect3, data.view_dlc_effect3, resolve, memory);
			write_fx_combined_def(native.worldDlcEffect3, data.world_dlc_effect3, resolve, memory);
		}

		std::size_t size_weapon_vfx_package_defs([[maybe_unused]] const asset_data::gameplay::weapon_vfx_package_defs& data)
		{
			std::size_t size = 0;
			size += size_fx_combined_def(data.view_flash_effect);
			size += size_fx_combined_def(data.world_flash_effect);
			size += size_fx_combined_def(data.view_flash_ads_effect);
			size += size_fx_combined_def(data.view_flash_first_effect);
			size += size_fx_combined_def(data.world_flash_first_effect);
			size += size_fx_combined_def(data.view_flash_first_ads_effect);
			size += size_fx_combined_def(data.view_flash_overcharge_effect);
			size += size_fx_combined_def(data.view_flash_overcharge_ads_effect);
			size += size_fx_combined_def(data.world_flash_overcharge_effect);
			size += size_fx_combined_def(data.view_shell_eject_effect);
			size += size_fx_combined_def(data.world_shell_eject_effect);
			size += size_fx_combined_def(data.view_last_shot_eject_effect);
			size += size_fx_combined_def(data.world_last_shot_eject_effect);
			size += size_fx_combined_def(data.view_mag_eject_effect);
			size += size_fx_combined_def(data.view_muzzle_smoke_effect);
			size += size_fx_combined_def(data.world_muzzle_smoke_effect);
			size += size_fx_combined_def(data.view_barrel_smoke_effect);
			size += size_fx_combined_def(data.world_barrel_smoke_effect);
			size += size_fx_combined_def(data.view_ejection_port_smoke_effect);
			size += size_fx_combined_def(data.world_ejection_port_smoke_effect);
			size += size_fx_combined_def(data.view_dlc_effect0);
			size += size_fx_combined_def(data.world_dlc_effect0);
			size += size_fx_combined_def(data.view_dlc_effect1);
			size += size_fx_combined_def(data.world_dlc_effect1);
			size += size_fx_combined_def(data.view_dlc_effect2);
			size += size_fx_combined_def(data.world_dlc_effect2);
			size += size_fx_combined_def(data.view_dlc_effect3);
			size += size_fx_combined_def(data.world_dlc_effect3);
			return size;
		}

		asset_data::gameplay::vfxpkg read_vfxpkg(const WeaponVFXPackage& native)
		{
			plain::trace("read_vfxpkg");
			asset_data::gameplay::vfxpkg data;
			data.name = plain::read_string(native.name);
			data.vfx = plain::read_single(native.vfx, [&](const auto& item) { return read_weapon_vfx_package_defs(item); });
			return data;
		}

		void write_vfxpkg(WeaponVFXPackage& native, const asset_data::gameplay::vfxpkg& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.vfx = plain::write_single<WeaponVFXPackageDefs>(data.vfx, memory, [&](auto& out, const auto& in) { write_weapon_vfx_package_defs(out, in, resolve, memory); });
		}

		std::size_t size_vfxpkg([[maybe_unused]] const asset_data::gameplay::vfxpkg& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.vfx)
			{
				size += plain::sized(sizeof(WeaponVFXPackageDefs));
				size += size_weapon_vfx_package_defs(*data.vfx);
			}
			return size;
		}

		asset_data::gameplay::tracer read_tracer(const TracerDef& native)
		{
			plain::trace("read_tracer");
			asset_data::gameplay::tracer data;
			data.name = plain::read_string(native.name);
			data.effect = read_fx_combined_def(native.effect);
			data.viewmodel_effect = read_fx_combined_def(native.viewmodelEffect);
			data.draw_interval = native.drawInterval;
			data.speed = native.speed;
			data.draw_legacy_tracer = native.drawLegacyTracer;
			data.fade_over_time = native.fadeOverTime;
			data.fade_time = native.fadeTime;
			data.material = plain::read_asset(ASSET_TYPE_MATERIAL, native.material);
			data.beam_length = native.beamLength;
			data.beam_width = native.beamWidth;
			data.screw_radius = native.screwRadius;
			data.screw_dist = native.screwDist;
			data.colors = [&] { std::array<std::array<float, 4>, 5> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = plain::read_scalars<float, 4>(&native.colors[i]); } return out; }();
			return data;
		}

		void write_tracer(TracerDef& native, const asset_data::gameplay::tracer& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			write_fx_combined_def(native.effect, data.effect, resolve, memory);
			write_fx_combined_def(native.viewmodelEffect, data.viewmodel_effect, resolve, memory);
			native.drawInterval = data.draw_interval;
			native.speed = data.speed;
			native.drawLegacyTracer = data.draw_legacy_tracer;
			native.fadeOverTime = data.fade_over_time;
			native.fadeTime = data.fade_time;
			native.material = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, data.material);
			native.beamLength = data.beam_length;
			native.beamWidth = data.beam_width;
			native.screwRadius = data.screw_radius;
			native.screwDist = data.screw_dist;
			for (std::size_t i = 0; i < data.colors.size(); ++i)
			{
				plain::write_scalars(&native.colors[i], data.colors[i]);
			}
		}

		std::size_t size_tracer([[maybe_unused]] const asset_data::gameplay::tracer& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			size += size_fx_combined_def(data.effect);
			size += size_fx_combined_def(data.viewmodel_effect);
			return size;
		}

		asset_data::gameplay::laser read_laser(const LaserDef& native)
		{
			plain::trace("read_laser");
			asset_data::gameplay::laser data;
			data.name = plain::read_string(native.name);
			data.laser_material = plain::read_asset(ASSET_TYPE_MATERIAL, native.laserMaterial);
			data.laser_light_material = plain::read_asset(ASSET_TYPE_MATERIAL, native.laserLightMaterial);
			data.owner_only = native.ownerOnly;
			data.nightvision_only = native.nightvisionOnly;
			data.range = native.range;
			data.radius = native.radius;
			data.end_offset_viewmodel = native.endOffsetViewmodel;
			data.end_offset_other = native.endOffsetOther;
			data.flare_pct = native.flarePct;
			data.beam_effect = read_fx_combined_def(native.beamEffect);
			data.laser_end_effect = read_fx_combined_def(native.laserEndEffect);
			data.client_collision = native.clientCollision;
			data.color = plain::read_scalars<float, 4>(&native.color);
			data.hdr_color_scale = plain::read_scalars<float, 4>(&native.hdrColorScale);
			data.laser_light = native.laserLight;
			data.laser_light_nvg_only = native.laserLightNvgOnly;
			data.laser_light_radius = native.laserLightRadius;
			data.laser_light_begin_offset = native.laserLightBeginOffset;
			data.laser_light_end_offset = native.laserLightEndOffset;
			data.laser_light_body_tweak = native.laserLightBodyTweak;
			data.laser_light_color = plain::read_scalars<float, 4>(&native.laserLightColor);
			data.laser_light_hdr_color_scale = plain::read_scalars<float, 4>(&native.laserLightHdrColorScale);
			data.range_alt = native.range_alt;
			data.radius_alt = native.radius_alt;
			data.laser_light_radius_alt = native.laserLightRadius_alt;
			data.flare_pct_alt = native.flarePct_alt;
			return data;
		}

		void write_laser(LaserDef& native, const asset_data::gameplay::laser& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.laserMaterial = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, data.laser_material);
			native.laserLightMaterial = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, data.laser_light_material);
			native.ownerOnly = data.owner_only;
			native.nightvisionOnly = data.nightvision_only;
			native.range = data.range;
			native.radius = data.radius;
			native.endOffsetViewmodel = data.end_offset_viewmodel;
			native.endOffsetOther = data.end_offset_other;
			native.flarePct = data.flare_pct;
			write_fx_combined_def(native.beamEffect, data.beam_effect, resolve, memory);
			write_fx_combined_def(native.laserEndEffect, data.laser_end_effect, resolve, memory);
			native.clientCollision = data.client_collision;
			plain::write_scalars(&native.color, data.color);
			plain::write_scalars(&native.hdrColorScale, data.hdr_color_scale);
			native.laserLight = data.laser_light;
			native.laserLightNvgOnly = data.laser_light_nvg_only;
			native.laserLightRadius = data.laser_light_radius;
			native.laserLightBeginOffset = data.laser_light_begin_offset;
			native.laserLightEndOffset = data.laser_light_end_offset;
			native.laserLightBodyTweak = data.laser_light_body_tweak;
			plain::write_scalars(&native.laserLightColor, data.laser_light_color);
			plain::write_scalars(&native.laserLightHdrColorScale, data.laser_light_hdr_color_scale);
			native.range_alt = data.range_alt;
			native.radius_alt = data.radius_alt;
			native.laserLightRadius_alt = data.laser_light_radius_alt;
			native.flarePct_alt = data.flare_pct_alt;
		}

		std::size_t size_laser([[maybe_unused]] const asset_data::gameplay::laser& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			size += size_fx_combined_def(data.beam_effect);
			size += size_fx_combined_def(data.laser_end_effect);
			return size;
		}

		asset_data::gameplay::camera_first_person_profile_def read_camera_first_person_profile_def(const CameraFirstPersonProfileDef& native)
		{
			plain::trace("read_camera_first_person_profile_def");
			asset_data::gameplay::camera_first_person_profile_def data;
			data.apply_offset = native.applyOffset;
			data.offset_translation = plain::read_scalars<float, 3>(&native.offsetTranslation);
			data.disable_fov_weapon_offset = native.disableFOVWeaponOffset;
			data.camera_animation_trans_scale = native.cameraAnimationTransScale;
			data.camera_animation_rot_scale = native.cameraAnimationRotScale;
			data.handheld_camera_trans_scale = native.handheldCameraTransScale;
			data.handheld_camera_rot_scale = native.handheldCameraRotScale;
			data.handheld_camera_impulse_trans_scale = native.handheldCameraImpulseTransScale;
			data.handheld_camera_impulse_rot_scale = native.handheldCameraImpulseRotScale;
			return data;
		}

		void write_camera_first_person_profile_def(CameraFirstPersonProfileDef& native, const asset_data::gameplay::camera_first_person_profile_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.applyOffset = data.apply_offset;
			plain::write_scalars(&native.offsetTranslation, data.offset_translation);
			native.disableFOVWeaponOffset = data.disable_fov_weapon_offset;
			native.cameraAnimationTransScale = data.camera_animation_trans_scale;
			native.cameraAnimationRotScale = data.camera_animation_rot_scale;
			native.handheldCameraTransScale = data.handheld_camera_trans_scale;
			native.handheldCameraRotScale = data.handheld_camera_rot_scale;
			native.handheldCameraImpulseTransScale = data.handheld_camera_impulse_trans_scale;
			native.handheldCameraImpulseRotScale = data.handheld_camera_impulse_rot_scale;
		}

		std::size_t size_camera_first_person_profile_def([[maybe_unused]] const asset_data::gameplay::camera_first_person_profile_def& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gameplay::camera_orbit_profile_def read_camera_orbit_profile_def(const CameraOrbitProfileDef& native)
		{
			plain::trace("read_camera_orbit_profile_def");
			asset_data::gameplay::camera_orbit_profile_def data;
			data.lerp_speed = native.lerpSpeed;
			data.pivot_offset = plain::read_scalars<float, 3>(&native.pivotOffset);
			data.initial_azimuth = native.initialAzimuth;
			data.initial_polar = native.initialPolar;
			data.polar_min = native.polarMin;
			data.polar_max = native.polarMax;
			data.range = native.range;
			data.return_azimuth_speed = native.returnAzimuthSpeed;
			data.return_polar_speed = native.returnPolarSpeed;
			data.ease_in_factor = native.easeInFactor;
			data.return_time_to = native.returnTimeTo;
			data.turning_speed_polar = native.turningSpeedPolar;
			data.turning_speed_azimuth = native.turningSpeedAzimuth;
			data.trace_coll_radius = native.traceCollRadius;
			data.mouse_input_multiplier = native.mouseInputMultiplier;
			data.roll_contrib = native.rollContrib;
			data.pitch_contrib = native.pitchContrib;
			data.button_toggle_return = native.buttonToggleReturn;
			data.button_cycle_perspective = native.buttonCyclePerspective;
			data.button_recenter = native.buttonRecenter;
			data.initial_return = native.initialReturn;
			data.return_azimuth = native.returnAzimuth;
			data.return_polar = native.returnPolar;
			data.align_to_movement = native.alignToMovement;
			data.interpolate_focus = native.interpolateFocus;
			data.camera_return_on_gas = native.cameraReturnOnGas;
			data.camera_return_on_speed = native.cameraReturnOnSpeed;
			data.yaw_turn_adjust = native.yawTurnAdjust;
			return data;
		}

		void write_camera_orbit_profile_def(CameraOrbitProfileDef& native, const asset_data::gameplay::camera_orbit_profile_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.lerpSpeed = data.lerp_speed;
			plain::write_scalars(&native.pivotOffset, data.pivot_offset);
			native.initialAzimuth = data.initial_azimuth;
			native.initialPolar = data.initial_polar;
			native.polarMin = data.polar_min;
			native.polarMax = data.polar_max;
			native.range = data.range;
			native.returnAzimuthSpeed = data.return_azimuth_speed;
			native.returnPolarSpeed = data.return_polar_speed;
			native.easeInFactor = data.ease_in_factor;
			native.returnTimeTo = data.return_time_to;
			native.turningSpeedPolar = data.turning_speed_polar;
			native.turningSpeedAzimuth = data.turning_speed_azimuth;
			native.traceCollRadius = data.trace_coll_radius;
			native.mouseInputMultiplier = data.mouse_input_multiplier;
			native.rollContrib = data.roll_contrib;
			native.pitchContrib = data.pitch_contrib;
			native.buttonToggleReturn = data.button_toggle_return;
			native.buttonCyclePerspective = data.button_cycle_perspective;
			native.buttonRecenter = data.button_recenter;
			native.initialReturn = data.initial_return;
			native.returnAzimuth = data.return_azimuth;
			native.returnPolar = data.return_polar;
			native.alignToMovement = data.align_to_movement;
			native.interpolateFocus = data.interpolate_focus;
			native.cameraReturnOnGas = data.camera_return_on_gas;
			native.cameraReturnOnSpeed = data.camera_return_on_speed;
			native.yawTurnAdjust = data.yaw_turn_adjust;
		}

		std::size_t size_camera_orbit_profile_def([[maybe_unused]] const asset_data::gameplay::camera_orbit_profile_def& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gameplay::camera_sentry_drone_def read_camera_sentry_drone_def(const CameraSentryDroneDef& native)
		{
			plain::trace("read_camera_sentry_drone_def");
			asset_data::gameplay::camera_sentry_drone_def data;
			data.range = native.range;
			data.thres_hit_to_speed_up = native.thresHitToSpeedUp;
			data.speed_up_factor = native.speedUpFactor;
			data.lerp_speed_eye = native.lerpSpeedEye;
			data.lerp_speed_focus = native.lerpSpeedFocus;
			data.lerp_speed_up = native.lerpSpeedUp;
			data.focus_trace_len = native.focusTraceLen;
			data.eye_height = native.eyeHeight;
			data.horizontal_offset = native.horizontalOffset;
			data.up_offset_factor_incl = native.upOffsetFactorIncl;
			return data;
		}

		void write_camera_sentry_drone_def(CameraSentryDroneDef& native, const asset_data::gameplay::camera_sentry_drone_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.range = data.range;
			native.thresHitToSpeedUp = data.thres_hit_to_speed_up;
			native.speedUpFactor = data.speed_up_factor;
			native.lerpSpeedEye = data.lerp_speed_eye;
			native.lerpSpeedFocus = data.lerp_speed_focus;
			native.lerpSpeedUp = data.lerp_speed_up;
			native.focusTraceLen = data.focus_trace_len;
			native.eyeHeight = data.eye_height;
			native.horizontalOffset = data.horizontal_offset;
			native.upOffsetFactorIncl = data.up_offset_factor_incl;
		}

		std::size_t size_camera_sentry_drone_def([[maybe_unused]] const asset_data::gameplay::camera_sentry_drone_def& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gameplay::camera_kill_cam_vehicle_def read_camera_kill_cam_vehicle_def(const CameraKillCamVehicleDef& native)
		{
			plain::trace("read_camera_kill_cam_vehicle_def");
			asset_data::gameplay::camera_kill_cam_vehicle_def data;
			data.look_at_radius = native.lookAtRadius;
			data.fov = native.fov;
			data.dist_back = native.distBack;
			data.dist_up = native.distUp;
			data.dist_back_radius_mod = native.distBackRadiusMod;
			data.dist_up_radius_mod = native.distUpRadiusMod;
			data.fov_radius_mod = native.fovRadiusMod;
			data.side_mag = native.sideMag;
			data.trace_coll_rad = native.traceCollRad;
			return data;
		}

		void write_camera_kill_cam_vehicle_def(CameraKillCamVehicleDef& native, const asset_data::gameplay::camera_kill_cam_vehicle_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.lookAtRadius = data.look_at_radius;
			native.fov = data.fov;
			native.distBack = data.dist_back;
			native.distUp = data.dist_up;
			native.distBackRadiusMod = data.dist_back_radius_mod;
			native.distUpRadiusMod = data.dist_up_radius_mod;
			native.fovRadiusMod = data.fov_radius_mod;
			native.sideMag = data.side_mag;
			native.traceCollRad = data.trace_coll_rad;
		}

		std::size_t size_camera_kill_cam_vehicle_def([[maybe_unused]] const asset_data::gameplay::camera_kill_cam_vehicle_def& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gameplay::camera_transition_def read_camera_transition_def(const CameraTransitionDef& native)
		{
			plain::trace("read_camera_transition_def");
			asset_data::gameplay::camera_transition_def data;
			data.active = native.active;
			data.time = native.time;
			data.timewait_eyepos = native.timewaitEyepos;
			data.timewait_axis = native.timewaitAxis;
			data.curve_eyepos = plain::enum_name(asset_data::gameplay::camera_transition_curve_names, static_cast<std::int64_t>(native.curveEyepos), "CameraTransitionDef.curveEyepos");
			data.curve_axis = plain::enum_name(asset_data::gameplay::camera_transition_curve_names, static_cast<std::int64_t>(native.curveAxis), "CameraTransitionDef.curveAxis");
			return data;
		}

		void write_camera_transition_def(CameraTransitionDef& native, const asset_data::gameplay::camera_transition_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.active = data.active;
			native.time = data.time;
			native.timewaitEyepos = data.timewait_eyepos;
			native.timewaitAxis = data.timewait_axis;
			native.curveEyepos = static_cast<CameraTransitionCurve>(plain::enum_value(asset_data::gameplay::camera_transition_curve_names, data.curve_eyepos, "CameraTransitionDef.curveEyepos"));
			native.curveAxis = static_cast<CameraTransitionCurve>(plain::enum_value(asset_data::gameplay::camera_transition_curve_names, data.curve_axis, "CameraTransitionDef.curveAxis"));
		}

		std::size_t size_camera_transition_def([[maybe_unused]] const asset_data::gameplay::camera_transition_def& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gameplay::camera read_camera(const CameraDef& native)
		{
			plain::trace("read_camera");
			asset_data::gameplay::camera data;
			data.name = plain::read_string(native.name);
			data.profile = plain::enum_name(asset_data::gameplay::camera_profile_names, static_cast<std::int64_t>(native.profile), "CameraDef.profile");
			data.keep_shake_cam = native.keepShakeCam;
			data.server_feedback = native.serverFeedback;
			data.behave_as_remote_cont = native.behaveAsRemoteCont;
			data.interpolate_view_transition = native.interpolateViewTransition;
			data.override_fov = native.overrideFOV;
			data.fov = native.FOV;
			data.override_lens_profile = plain::enum_name(asset_data::gameplay::lens_profile_mode_names, static_cast<std::int64_t>(native.overrideLensProfile), "CameraDef.overrideLensProfile");
			data.lens_focal_length = native.lensFocalLength;
			data.lens_f_stop = native.lensFStop;
			data.lens_scale = native.lensScale;
			data.lens_uv_scale = native.lensUVScale;
			data.override_radial_blur = native.overrideRadialBlur;
			data.radial_blur_radius = native.radialBlurRadius;
			data.radial_blur_strength = native.radialBlurStrength;
			data.override_z_planes = native.overrideZPlanes;
			data.z_plane_vm_near = native.zPlaneVMNear;
			data.z_plane_scene_near = native.zPlaneSceneNear;
			data.first_person = read_camera_first_person_profile_def(native.firstPerson);
			data.orbit = read_camera_orbit_profile_def(native.orbit);
			data.sentry_drone = read_camera_sentry_drone_def(native.sentryDrone);
			data.kcam_veh = read_camera_kill_cam_vehicle_def(native.kcamVeh);
			data.transition_in = read_camera_transition_def(native.transitionIn);
			data.transition_out = read_camera_transition_def(native.transitionOut);
			data.fpveh_offset_ls = plain::read_scalars<float, 3>(&native.fpveh_offsetLs);
			data.fpveh_offset_yaw = native.fpveh_offsetYaw;
			data.fpveh_offset_pitch = native.fpveh_offsetPitch;
			data.fpveh_roll_contrib = native.fpveh_rollContrib;
			data.fpveh_roll_max = native.fpveh_rollMax;
			data.fpveh_free_look = native.fpveh_freeLook;
			data.fpveh_free_look_return = native.fpveh_freeLookReturn;
			data.dlc_bool = std::to_array(native.dlcBool);
			data.fade_out_player_on_transition_in = native.fadeOutPlayerOnTransitionIn;
			return data;
		}

		void write_camera(CameraDef& native, const asset_data::gameplay::camera& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.profile = static_cast<CameraProfile>(plain::enum_value(asset_data::gameplay::camera_profile_names, data.profile, "CameraDef.profile"));
			native.keepShakeCam = data.keep_shake_cam;
			native.serverFeedback = data.server_feedback;
			native.behaveAsRemoteCont = data.behave_as_remote_cont;
			native.interpolateViewTransition = data.interpolate_view_transition;
			native.overrideFOV = data.override_fov;
			native.FOV = data.fov;
			native.overrideLensProfile = static_cast<LensProfileMode>(plain::enum_value(asset_data::gameplay::lens_profile_mode_names, data.override_lens_profile, "CameraDef.overrideLensProfile"));
			native.lensFocalLength = data.lens_focal_length;
			native.lensFStop = data.lens_f_stop;
			native.lensScale = data.lens_scale;
			native.lensUVScale = data.lens_uv_scale;
			native.overrideRadialBlur = data.override_radial_blur;
			native.radialBlurRadius = data.radial_blur_radius;
			native.radialBlurStrength = data.radial_blur_strength;
			native.overrideZPlanes = data.override_z_planes;
			native.zPlaneVMNear = data.z_plane_vm_near;
			native.zPlaneSceneNear = data.z_plane_scene_near;
			write_camera_first_person_profile_def(native.firstPerson, data.first_person, resolve, memory);
			write_camera_orbit_profile_def(native.orbit, data.orbit, resolve, memory);
			write_camera_sentry_drone_def(native.sentryDrone, data.sentry_drone, resolve, memory);
			write_camera_kill_cam_vehicle_def(native.kcamVeh, data.kcam_veh, resolve, memory);
			write_camera_transition_def(native.transitionIn, data.transition_in, resolve, memory);
			write_camera_transition_def(native.transitionOut, data.transition_out, resolve, memory);
			plain::write_scalars(&native.fpveh_offsetLs, data.fpveh_offset_ls);
			native.fpveh_offsetYaw = data.fpveh_offset_yaw;
			native.fpveh_offsetPitch = data.fpveh_offset_pitch;
			native.fpveh_rollContrib = data.fpveh_roll_contrib;
			native.fpveh_rollMax = data.fpveh_roll_max;
			native.fpveh_freeLook = data.fpveh_free_look;
			native.fpveh_freeLookReturn = data.fpveh_free_look_return;
			std::ranges::copy(data.dlc_bool, native.dlcBool);
			native.fadeOutPlayerOnTransitionIn = data.fade_out_player_on_transition_in;
		}

		std::size_t size_camera([[maybe_unused]] const asset_data::gameplay::camera& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			size += size_camera_first_person_profile_def(data.first_person);
			size += size_camera_orbit_profile_def(data.orbit);
			size += size_camera_sentry_drone_def(data.sentry_drone);
			size += size_camera_kill_cam_vehicle_def(data.kcam_veh);
			size += size_camera_transition_def(data.transition_in);
			size += size_camera_transition_def(data.transition_out);
			return size;
		}

		asset_data::gameplay::hudoutline read_hudoutline(const HudOutlineDef& native)
		{
			plain::trace("read_hudoutline");
			asset_data::gameplay::hudoutline data;
			data.name = plain::read_string(native.name);
			data.outline_type = native.outlineType;
			data.draw_in_stencil = native.drawInStencil;
			data.draw_non_occluded_pixels = native.drawNonOccludedPixels;
			data.draw_occluded_pixels = native.drawOccludedPixels;
			data.draw_fill = native.drawFill;
			data.fade_over_time_enable = native.fadeOverTimeEnable;
			data.fade_over_time_looping = native.fadeOverTimeLooping;
			data.fade_over_time_curve_type = native.fadeOverTimeCurveType;
			data.fade_over_time_min_alpha = native.fadeOverTimeMinAlpha;
			data.fade_over_time_max_alpha_time = native.fadeOverTimeMaxAlphaTime;
			data.fade_over_time_min_alpha_time = native.fadeOverTimeMinAlphaTime;
			data.fade_over_time_in_time = native.fadeOverTimeInTime;
			data.fade_over_time_out_time = native.fadeOverTimeOutTime;
			data.distance_fade_enable = native.distanceFadeEnable;
			data.distance_fade_min_alpha = native.distanceFadeMinAlpha;
			data.distance_fade_start_distance = native.distanceFadeStartDistance;
			data.distance_fade_end_distance = native.distanceFadeEndDistance;
			data.outline_color = native.outlineColor;
			data.outline_width = native.outlineWidth;
			data.draw_on_local_player_character = native.drawOnLocalPlayerCharacter;
			data.draw_on_vehicle_occupants = native.drawOnVehicleOccupants;
			data.draw_on_local_player_vehicle_occupants = native.drawOnLocalPlayerVehicleOccupants;
			data.draw_on_local_player_vehicle = native.drawOnLocalPlayerVehicle;
			data.dlc_bool0 = native.dlcBool0;
			data.dlc_bool1 = native.dlcBool1;
			data.dlc_bool2 = native.dlcBool2;
			data.dlc_bool3 = native.dlcBool3;
			data.dlc_bool4 = native.dlcBool4;
			data.dlc_bool5 = native.dlcBool5;
			data.dlc_float0 = native.dlcFloat0;
			data.dlc_float1 = native.dlcFloat1;
			data.dlc_float2 = native.dlcFloat2;
			data.dlc_float3 = native.dlcFloat3;
			data.dlc_float4 = native.dlcFloat4;
			data.dlc_float5 = native.dlcFloat5;
			return data;
		}

		void write_hudoutline(HudOutlineDef& native, const asset_data::gameplay::hudoutline& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.outlineType = data.outline_type;
			native.drawInStencil = data.draw_in_stencil;
			native.drawNonOccludedPixels = data.draw_non_occluded_pixels;
			native.drawOccludedPixels = data.draw_occluded_pixels;
			native.drawFill = data.draw_fill;
			native.fadeOverTimeEnable = data.fade_over_time_enable;
			native.fadeOverTimeLooping = data.fade_over_time_looping;
			native.fadeOverTimeCurveType = data.fade_over_time_curve_type;
			native.fadeOverTimeMinAlpha = data.fade_over_time_min_alpha;
			native.fadeOverTimeMaxAlphaTime = data.fade_over_time_max_alpha_time;
			native.fadeOverTimeMinAlphaTime = data.fade_over_time_min_alpha_time;
			native.fadeOverTimeInTime = data.fade_over_time_in_time;
			native.fadeOverTimeOutTime = data.fade_over_time_out_time;
			native.distanceFadeEnable = data.distance_fade_enable;
			native.distanceFadeMinAlpha = data.distance_fade_min_alpha;
			native.distanceFadeStartDistance = data.distance_fade_start_distance;
			native.distanceFadeEndDistance = data.distance_fade_end_distance;
			native.outlineColor = data.outline_color;
			native.outlineWidth = data.outline_width;
			native.drawOnLocalPlayerCharacter = data.draw_on_local_player_character;
			native.drawOnVehicleOccupants = data.draw_on_vehicle_occupants;
			native.drawOnLocalPlayerVehicleOccupants = data.draw_on_local_player_vehicle_occupants;
			native.drawOnLocalPlayerVehicle = data.draw_on_local_player_vehicle;
			native.dlcBool0 = data.dlc_bool0;
			native.dlcBool1 = data.dlc_bool1;
			native.dlcBool2 = data.dlc_bool2;
			native.dlcBool3 = data.dlc_bool3;
			native.dlcBool4 = data.dlc_bool4;
			native.dlcBool5 = data.dlc_bool5;
			native.dlcFloat0 = data.dlc_float0;
			native.dlcFloat1 = data.dlc_float1;
			native.dlcFloat2 = data.dlc_float2;
			native.dlcFloat3 = data.dlc_float3;
			native.dlcFloat4 = data.dlc_float4;
			native.dlcFloat5 = data.dlc_float5;
		}

		std::size_t size_hudoutline([[maybe_unused]] const asset_data::gameplay::hudoutline& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			return size;
		}

		asset_data::gameplay::footstepvfx read_footstepvfx(const FootstepVFX& native)
		{
			plain::trace("read_footstepvfx");
			asset_data::gameplay::footstepvfx data;
			data.name = plain::read_string(native.name);
			data.footstep_vfx = [&] { std::array<asset_data::gameplay::fx_combined_def, 64> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_fx_combined_def(native.footstepVFX[i]); } return out; }();
			data.small_footstep_vfx = [&] { std::array<asset_data::gameplay::fx_combined_def, 64> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_fx_combined_def(native.smallFootstepVFX[i]); } return out; }();
			return data;
		}

		void write_footstepvfx(FootstepVFX& native, const asset_data::gameplay::footstepvfx& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			for (std::size_t i = 0; i < data.footstep_vfx.size(); ++i)
			{
				write_fx_combined_def(native.footstepVFX[i], data.footstep_vfx[i], resolve, memory);
			}
			for (std::size_t i = 0; i < data.small_footstep_vfx.size(); ++i)
			{
				write_fx_combined_def(native.smallFootstepVFX[i], data.small_footstep_vfx[i], resolve, memory);
			}
		}

		std::size_t size_footstepvfx([[maybe_unused]] const asset_data::gameplay::footstepvfx& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			for (const auto& item : data.footstep_vfx)
			{
				size += size_fx_combined_def(item);
			}
			for (const auto& item : data.small_footstep_vfx)
			{
				size += size_fx_combined_def(item);
			}
			return size;
		}

		asset_data::gameplay::x_composite_submodel read_x_composite_submodel(const XCompositeSubmodel& native)
		{
			plain::trace("read_x_composite_submodel");
			asset_data::gameplay::x_composite_submodel data;
			data.model_name = plain::read_string(native.modelName);
			data.translation = plain::read_scalars<float, 3>(&native.translation);
			data.angles = plain::read_scalars<float, 3>(&native.angles);
			data.parent_bone_name = plain::read_script_string(native.parentBoneName);
			data.parent_submodel_slot = native.parentSubmodelSlot;
			data.model = plain::read_asset(ASSET_TYPE_XMODEL, native.model);
			return data;
		}

		void write_x_composite_submodel(XCompositeSubmodel& native, const asset_data::gameplay::x_composite_submodel& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.modelName = plain::write_string(memory, data.model_name);
			plain::write_scalars(&native.translation, data.translation);
			plain::write_scalars(&native.angles, data.angles);
			native.parentBoneName = static_cast<scr_string_t>(plain::write_script_string(data.parent_bone_name));
			native.parentSubmodelSlot = data.parent_submodel_slot;
			native.model = plain::resolve_asset<XModel>(resolve, ASSET_TYPE_XMODEL, data.model);
		}

		std::size_t size_x_composite_submodel([[maybe_unused]] const asset_data::gameplay::x_composite_submodel& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.model_name);
			return size;
		}

		asset_data::gameplay::x_composite_sub_composite_model read_x_composite_sub_composite_model(const XCompositeSubCompositeModel& native)
		{
			plain::trace("read_x_composite_sub_composite_model");
			asset_data::gameplay::x_composite_sub_composite_model data;
			data.composite_model_name = plain::read_string(native.compositeModelName);
			data.translation = plain::read_scalars<float, 3>(&native.translation);
			data.angles = plain::read_scalars<float, 3>(&native.angles);
			data.parent_bone_name = plain::read_script_string(native.parentBoneName);
			data.parent_submodel_slot = native.parentSubmodelSlot;
			data.composite_model = plain::read_asset(ASSET_TYPE_XCOMPOSITEMODEL, native.compositeModel);
			return data;
		}

		void write_x_composite_sub_composite_model(XCompositeSubCompositeModel& native, const asset_data::gameplay::x_composite_sub_composite_model& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.compositeModelName = plain::write_string(memory, data.composite_model_name);
			plain::write_scalars(&native.translation, data.translation);
			plain::write_scalars(&native.angles, data.angles);
			native.parentBoneName = static_cast<scr_string_t>(plain::write_script_string(data.parent_bone_name));
			native.parentSubmodelSlot = data.parent_submodel_slot;
			native.compositeModel = plain::resolve_asset<XCompositeModelDef>(resolve, ASSET_TYPE_XCOMPOSITEMODEL, data.composite_model);
		}

		std::size_t size_x_composite_sub_composite_model([[maybe_unused]] const asset_data::gameplay::x_composite_sub_composite_model& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.composite_model_name);
			return size;
		}

		asset_data::gameplay::xcompositemodel read_xcompositemodel(const XCompositeModelDef& native)
		{
			plain::trace("read_xcompositemodel");
			asset_data::gameplay::xcompositemodel data;
			data.name = plain::read_string(native.name);
			data.submodels = plain::read_fixed_with<10>(native.submodels, [&](const auto& item) { return read_x_composite_submodel(item); });
			data.sub_composite_models = plain::read_fixed_with<10>(native.subCompositeModels, [&](const auto& item) { return read_x_composite_sub_composite_model(item); });
			return data;
		}

		void write_xcompositemodel(XCompositeModelDef& native, const asset_data::gameplay::xcompositemodel& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			if (data.submodels)
			{
				native.submodels = memory.manual_allocate<XCompositeSubmodel>(plain::rounded(10 * sizeof(XCompositeSubmodel)));
				for (std::size_t i = 0; i < 10; ++i)
				{
					write_x_composite_submodel(native.submodels[i], (*data.submodels)[i], resolve, memory);
				}
			}
			if (data.sub_composite_models)
			{
				native.subCompositeModels = memory.manual_allocate<XCompositeSubCompositeModel>(plain::rounded(10 * sizeof(XCompositeSubCompositeModel)));
				for (std::size_t i = 0; i < 10; ++i)
				{
					write_x_composite_sub_composite_model(native.subCompositeModels[i], (*data.sub_composite_models)[i], resolve, memory);
				}
			}
		}

		std::size_t size_xcompositemodel([[maybe_unused]] const asset_data::gameplay::xcompositemodel& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.submodels)
			{
				size += plain::sized(10 * sizeof(XCompositeSubmodel));
				for (const auto& item : *data.submodels)
				{
					size += size_x_composite_submodel(item);
				}
			}
			if (data.sub_composite_models)
			{
				size += plain::sized(10 * sizeof(XCompositeSubCompositeModel));
				for (const auto& item : *data.sub_composite_models)
				{
					size += size_x_composite_sub_composite_model(item);
				}
			}
			return size;
		}

		asset_data::gameplay::xanimcurve read_xanimcurve(const XAnimCurve& native)
		{
			plain::trace("read_xanimcurve");
			asset_data::gameplay::xanimcurve data;
			data.name = plain::read_string(native.name);
			data.control_points = plain::read_array(native.controlPoints, native.numControlPoints, "XAnimCurve.controlPoints", [&](const auto& item) { return read_x_anim_curve_control_point(item); });
			data.curve_type = plain::enum_name(asset_data::gameplay::x_anim_curve_type_names, static_cast<std::int64_t>(native.curveType), "XAnimCurve.curveType");
			data.duration = native.duration;
			return data;
		}

		void write_xanimcurve(XAnimCurve& native, const asset_data::gameplay::xanimcurve& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.controlPoints = plain::write_array<XAnimCurveControlPoint>(data.control_points, memory, [&](auto& out, const auto& in) { write_x_anim_curve_control_point(out, in, resolve, memory); });
			native.curveType = static_cast<XAnimCurveType>(plain::enum_value(asset_data::gameplay::x_anim_curve_type_names, data.curve_type, "XAnimCurve.curveType"));
			native.duration = data.duration;
			native.numControlPoints = plain::count_of<std::int32_t>(data.control_points);
		}

		std::size_t size_xanimcurve([[maybe_unused]] const asset_data::gameplay::xanimcurve& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.control_points)
			{
				size += plain::sized(std::max<std::size_t>(data.control_points->size(), 1) * sizeof(XAnimCurveControlPoint));
				for (const auto& item : *data.control_points)
				{
					size += size_x_anim_curve_control_point(item);
				}
			}
			return size;
		}

		asset_data::gameplay::x_anim_curve_control_point read_x_anim_curve_control_point(const XAnimCurveControlPoint& native)
		{
			plain::trace("read_x_anim_curve_control_point");
			asset_data::gameplay::x_anim_curve_control_point data;
			data.time = native.time;
			data.value = native.value;
			data.inv_time_delta = native.invTimeDelta;
			return data;
		}

		void write_x_anim_curve_control_point(XAnimCurveControlPoint& native, const asset_data::gameplay::x_anim_curve_control_point& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.time = data.time;
			native.value = data.value;
			native.invTimeDelta = data.inv_time_delta;
		}

		std::size_t size_x_anim_curve_control_point([[maybe_unused]] const asset_data::gameplay::x_anim_curve_control_point& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gameplay::keyvaluepairs read_keyvaluepairs(const KeyValuePairs& native)
		{
			plain::trace("read_keyvaluepairs");
			asset_data::gameplay::keyvaluepairs data;
			data.name = plain::read_string(native.name);
			data.key_value_pairs = plain::read_array(native.keyValuePairs, native.numVariables, "KeyValuePairs.keyValuePairs", [&](const auto& item) { return read_key_value_pair(item); });
			return data;
		}

		void write_keyvaluepairs(KeyValuePairs& native, const asset_data::gameplay::keyvaluepairs& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.keyValuePairs = plain::write_array<KeyValuePair>(data.key_value_pairs, memory, [&](auto& out, const auto& in) { write_key_value_pair(out, in, resolve, memory); });
			native.numVariables = plain::count_of<std::int32_t>(data.key_value_pairs);
		}

		std::size_t size_keyvaluepairs([[maybe_unused]] const asset_data::gameplay::keyvaluepairs& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.key_value_pairs)
			{
				size += plain::sized(std::max<std::size_t>(data.key_value_pairs->size(), 1) * sizeof(KeyValuePair));
				for (const auto& item : *data.key_value_pairs)
				{
					size += size_key_value_pair(item);
				}
			}
			return size;
		}

		asset_data::gameplay::key_value_pair read_key_value_pair(const KeyValuePair& native)
		{
			plain::trace("read_key_value_pair");
			asset_data::gameplay::key_value_pair data;
			data.key_hash = native.keyHash;
			data.value = plain::read_string(native.value);
			return data;
		}

		void write_key_value_pair(KeyValuePair& native, const asset_data::gameplay::key_value_pair& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.keyHash = data.key_hash;
			native.value = plain::write_string(memory, data.value);
		}

		std::size_t size_key_value_pair([[maybe_unused]] const asset_data::gameplay::key_value_pair& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.value);
			return size;
		}

		asset_data::gameplay::blendspace2d read_blendspace2d(const BlendSpace2DDef& native)
		{
			plain::trace("read_blendspace2d");
			asset_data::gameplay::blendspace2d data;
			data.name = plain::read_string(native.name);
			data.range_x = plain::read_scalars<float, 2>(&native.rangeX);
			data.range_y = plain::read_scalars<float, 2>(&native.rangeY);
			data.anim_entries = plain::read_array(native.animEntries, native.numAnimEntries, "BlendSpace2DDef.animEntries", [&](const auto& item) { return read_blend_space2_d_anim_entry(item); });
			data.triangle_coords = plain::read_array(native.triangleCoords, native.numTriangleCoords, "BlendSpace2DDef.triangleCoords", [&](const auto& item) { return item; });
			data.blend_time_ms = native.blendTimeMs;
			data.all_anims_notify = native.allAnimsNotify;
			return data;
		}

		void write_blendspace2d(BlendSpace2DDef& native, const asset_data::gameplay::blendspace2d& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			plain::write_scalars(&native.rangeX, data.range_x);
			plain::write_scalars(&native.rangeY, data.range_y);
			native.animEntries = plain::write_array<BlendSpace2DAnimEntry>(data.anim_entries, memory, [&](auto& out, const auto& in) { write_blend_space2_d_anim_entry(out, in, resolve, memory); });
			native.triangleCoords = plain::write_array<std::uint32_t>(data.triangle_coords, memory, [&](auto& out, const auto& in) { out = in; });
			native.blendTimeMs = data.blend_time_ms;
			native.allAnimsNotify = data.all_anims_notify;
			native.numAnimEntries = plain::count_of<std::uint32_t>(data.anim_entries);
			native.numTriangleCoords = plain::count_of<std::uint32_t>(data.triangle_coords);
		}

		std::size_t size_blendspace2d([[maybe_unused]] const asset_data::gameplay::blendspace2d& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.anim_entries)
			{
				size += plain::sized(std::max<std::size_t>(data.anim_entries->size(), 1) * sizeof(BlendSpace2DAnimEntry));
				for (const auto& item : *data.anim_entries)
				{
					size += size_blend_space2_d_anim_entry(item);
				}
			}
			if (data.triangle_coords)
			{
				size += plain::sized(std::max<std::size_t>(data.triangle_coords->size(), 1) * sizeof(std::uint32_t));
			}
			return size;
		}

		asset_data::gameplay::blend_space2_d_anim_entry read_blend_space2_d_anim_entry(const BlendSpace2DAnimEntry& native)
		{
			plain::trace("read_blend_space2_d_anim_entry");
			asset_data::gameplay::blend_space2_d_anim_entry data;
			data.anim = plain::read_asset(ASSET_TYPE_XANIMPARTS, native.anim);
			data.anim_coords = plain::read_scalars<float, 2>(&native.animCoords);
			return data;
		}

		void write_blend_space2_d_anim_entry(BlendSpace2DAnimEntry& native, const asset_data::gameplay::blend_space2_d_anim_entry& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.anim = plain::resolve_asset<XAnimParts>(resolve, ASSET_TYPE_XANIMPARTS, data.anim);
			plain::write_scalars(&native.animCoords, data.anim_coords);
		}

		std::size_t size_blend_space2_d_anim_entry([[maybe_unused]] const asset_data::gameplay::blend_space2_d_anim_entry& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gameplay::camo read_camo(const Camo& native)
		{
			plain::trace("read_camo");
			asset_data::gameplay::camo data;
			data.name = plain::read_string(native.name);
			data.internal_name = plain::read_script_string(native.internalName);
			data.texture_defs = plain::read_array(native.textureDefs, native.textureDefCount, "Camo.textureDefs", [&](const auto& item) { return read_material_texture_def(item); });
			data.color_tint = std::to_array(native.colorTint);
			data.albedo_map_scale_bias = std::to_array(native.albedoMapScaleBias);
			data.normal_map_scale_bias = std::to_array(native.normalMapScaleBias);
			data.uv_scale_offset = [&] { std::array<std::array<float, 4>, 3> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = plain::read_scalars<float, 4>(&native.uvScaleOffset[i]); } return out; }();
			data.uv_rotate = [&] { std::array<std::array<float, 4>, 3> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = plain::read_scalars<float, 4>(&native.uvRotate[i]); } return out; }();
			data.blend_map_channels = native.blendMapChannels;
			return data;
		}

		void write_camo(Camo& native, const asset_data::gameplay::camo& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.internalName = static_cast<scr_string_t>(plain::write_script_string(data.internal_name));
			native.textureDefs = plain::write_array<MaterialTextureDef>(data.texture_defs, memory, [&](auto& out, const auto& in) { write_material_texture_def(out, in, resolve, memory); });
			std::ranges::copy(data.color_tint, native.colorTint);
			std::ranges::copy(data.albedo_map_scale_bias, native.albedoMapScaleBias);
			std::ranges::copy(data.normal_map_scale_bias, native.normalMapScaleBias);
			for (std::size_t i = 0; i < data.uv_scale_offset.size(); ++i)
			{
				plain::write_scalars(&native.uvScaleOffset[i], data.uv_scale_offset[i]);
			}
			for (std::size_t i = 0; i < data.uv_rotate.size(); ++i)
			{
				plain::write_scalars(&native.uvRotate[i], data.uv_rotate[i]);
			}
			native.blendMapChannels = data.blend_map_channels;
			native.textureDefCount = plain::count_of<std::uint32_t>(data.texture_defs);
		}

		std::size_t size_camo([[maybe_unused]] const asset_data::gameplay::camo& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.texture_defs)
			{
				size += plain::sized(std::max<std::size_t>(data.texture_defs->size(), 1) * sizeof(MaterialTextureDef));
				for (const auto& item : *data.texture_defs)
				{
					size += size_material_texture_def(item);
				}
			}
			return size;
		}

		asset_data::gameplay::material_texture_def read_material_texture_def(const MaterialTextureDef& native)
		{
			plain::trace("read_material_texture_def");
			asset_data::gameplay::material_texture_def data;
			data.index = native.index;
			data.image = plain::read_asset(ASSET_TYPE_IMAGE, native.image);
			return data;
		}

		void write_material_texture_def(MaterialTextureDef& native, const asset_data::gameplay::material_texture_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.index = data.index;
			native.image = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, data.image);
		}

		std::size_t size_material_texture_def([[maybe_unused]] const asset_data::gameplay::material_texture_def& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gameplay::accessory read_accessory(const AccessoryDef& native)
		{
			plain::trace("read_accessory");
			asset_data::gameplay::accessory data;
			data.name = plain::read_string(native.name);
			data.vm_attach_bone = plain::read_script_string(native.VMAttachBone);
			data.vm_attach_right_hand = native.VMAttachRightHand;
			data.interact_gesture = plain::read_asset(ASSET_TYPE_GESTURE, native.interactGesture);
			data.widget_vm = plain::read_string(native.widgetVM);
			data.widget_vmrtt_width = native.widgetVMRTTWidth;
			data.widget_vmrtt_height = native.widgetVMRTTHeight;
			data.widget_vmrtt_mip_count = native.widgetVMRTTMipCount;
			return data;
		}

		void write_accessory(AccessoryDef& native, const asset_data::gameplay::accessory& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.VMAttachBone = static_cast<scr_string_t>(plain::write_script_string(data.vm_attach_bone));
			native.VMAttachRightHand = data.vm_attach_right_hand;
			native.interactGesture = plain::resolve_asset<Gesture>(resolve, ASSET_TYPE_GESTURE, data.interact_gesture);
			native.widgetVM = plain::write_string(memory, data.widget_vm);
			native.widgetVMRTTWidth = data.widget_vmrtt_width;
			native.widgetVMRTTHeight = data.widget_vmrtt_height;
			native.widgetVMRTTMipCount = data.widget_vmrtt_mip_count;
		}

		std::size_t size_accessory([[maybe_unused]] const asset_data::gameplay::accessory& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			size += plain::string_size(data.widget_vm);
			return size;
		}

		asset_data::gameplay::locdmgtable read_locdmgtable(const LocDmgTable& native)
		{
			plain::trace("read_locdmgtable");
			asset_data::gameplay::locdmgtable data;
			data.name = plain::read_string(native.name);
			data.mults = std::to_array(native.mults);
			return data;
		}

		void write_locdmgtable(LocDmgTable& native, const asset_data::gameplay::locdmgtable& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			std::ranges::copy(data.mults, native.mults);
		}

		std::size_t size_locdmgtable([[maybe_unused]] const asset_data::gameplay::locdmgtable& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			return size;
		}

		asset_data::gameplay::bulletpenetration read_bulletpenetration(const BulletPenetration& native)
		{
			plain::trace("read_bulletpenetration");
			asset_data::gameplay::bulletpenetration data;
			data.name = plain::read_string(native.name);
			data.depth_table = plain::read_block<std::array<std::array<float, 64>, 4>>(&native.depthTable);
			return data;
		}

		void write_bulletpenetration(BulletPenetration& native, const asset_data::gameplay::bulletpenetration& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			plain::write_block(&native.depthTable, data.depth_table);
		}

		std::size_t size_bulletpenetration([[maybe_unused]] const asset_data::gameplay::bulletpenetration& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			return size;
		}

		asset_data::gameplay::cinematic_shake_modifier_def read_cinematic_shake_modifier_def(const CinematicShakeModifierDef& native)
		{
			plain::trace("read_cinematic_shake_modifier_def");
			asset_data::gameplay::cinematic_shake_modifier_def data;
			data.type = static_cast<std::uint32_t>(native.type);
			data.in_blend_in_time_ms = native.inBlendInTimeMs;
			data.in_blend_out_time_ms = native.inBlendOutTimeMs;
			data.in_speed_range = plain::read_scalars<float, 2>(&native.inSpeedRange);
			data.out_amplitude_scale = plain::read_scalars<float, 2>(&native.outAmplitudeScale);
			data.out_persistence_scale = plain::read_scalars<float, 2>(&native.outPersistenceScale);
			data.out_speed_scale = plain::read_scalars<float, 2>(&native.outSpeedScale);
			data.out_blend_in_time_ms = native.outBlendInTimeMs;
			data.out_blend_out_time_ms = native.outBlendOutTimeMs;
			data.non_linear_map = plain::read_asset(ASSET_TYPE_RUMBLE_GRAPH, native.nonLinearMap);
			return data;
		}

		void write_cinematic_shake_modifier_def(CinematicShakeModifierDef& native, const asset_data::gameplay::cinematic_shake_modifier_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.type = static_cast<CinematicShakeModifierType>(data.type);
			native.inBlendInTimeMs = data.in_blend_in_time_ms;
			native.inBlendOutTimeMs = data.in_blend_out_time_ms;
			plain::write_scalars(&native.inSpeedRange, data.in_speed_range);
			plain::write_scalars(&native.outAmplitudeScale, data.out_amplitude_scale);
			plain::write_scalars(&native.outPersistenceScale, data.out_persistence_scale);
			plain::write_scalars(&native.outSpeedScale, data.out_speed_scale);
			native.outBlendInTimeMs = data.out_blend_in_time_ms;
			native.outBlendOutTimeMs = data.out_blend_out_time_ms;
			native.nonLinearMap = plain::resolve_asset<RumbleGraph>(resolve, ASSET_TYPE_RUMBLE_GRAPH, data.non_linear_map);
		}

		std::size_t size_cinematic_shake_modifier_def([[maybe_unused]] const asset_data::gameplay::cinematic_shake_modifier_def& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gameplay::cinematic_shake_def read_cinematic_shake_def(const CinematicShakeDef& native)
		{
			plain::trace("read_cinematic_shake_def");
			asset_data::gameplay::cinematic_shake_def data;
			data.enabled = native.enabled;
			data.frequency = plain::read_scalars<float, 3>(&native.frequency);
			data.amplitude = plain::read_scalars<float, 3>(&native.amplitude);
			data.persistence = plain::read_scalars<float, 3>(&native.persistence);
			data.move_speed_modifiers = read_cinematic_shake_modifier_def(native.moveSpeedModifiers);
			data.view_speed_modifiers = read_cinematic_shake_modifier_def(native.viewSpeedModifiers);
			data.modifier_combine_type = static_cast<std::uint32_t>(native.modifierCombineType);
			return data;
		}

		void write_cinematic_shake_def(CinematicShakeDef& native, const asset_data::gameplay::cinematic_shake_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.enabled = data.enabled;
			plain::write_scalars(&native.frequency, data.frequency);
			plain::write_scalars(&native.amplitude, data.amplitude);
			plain::write_scalars(&native.persistence, data.persistence);
			write_cinematic_shake_modifier_def(native.moveSpeedModifiers, data.move_speed_modifiers, resolve, memory);
			write_cinematic_shake_modifier_def(native.viewSpeedModifiers, data.view_speed_modifiers, resolve, memory);
			native.modifierCombineType = static_cast<CinematicShakeModifierCombineType>(data.modifier_combine_type);
		}

		std::size_t size_cinematic_shake_def([[maybe_unused]] const asset_data::gameplay::cinematic_shake_def& data)
		{
			std::size_t size = 0;
			size += size_cinematic_shake_modifier_def(data.move_speed_modifiers);
			size += size_cinematic_shake_modifier_def(data.view_speed_modifiers);
			return size;
		}

		asset_data::gameplay::cinematic_impact_shake_def read_cinematic_impact_shake_def(const CinematicImpactShakeDef& native)
		{
			plain::trace("read_cinematic_impact_shake_def");
			asset_data::gameplay::cinematic_impact_shake_def data;
			data.frequency = plain::read_scalars<float, 3>(&native.frequency);
			data.amplitude = plain::read_scalars<float, 3>(&native.amplitude);
			data.persistence = plain::read_scalars<float, 3>(&native.persistence);
			return data;
		}

		void write_cinematic_impact_shake_def(CinematicImpactShakeDef& native, const asset_data::gameplay::cinematic_impact_shake_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.frequency, data.frequency);
			plain::write_scalars(&native.amplitude, data.amplitude);
			plain::write_scalars(&native.persistence, data.persistence);
		}

		std::size_t size_cinematic_impact_shake_def([[maybe_unused]] const asset_data::gameplay::cinematic_impact_shake_def& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gameplay::cinematicmotion read_cinematicmotion(const CinematicMotionDef& native)
		{
			plain::trace("read_cinematicmotion");
			asset_data::gameplay::cinematicmotion data;
			data.name = plain::read_string(native.name);
			data.camera_spring_velocity_smoothing_sec = plain::read_scalars<float, 3>(&native.camera_spring_velocitySmoothingSec);
			data.camera_spring_velocity_scale = plain::read_scalars<float, 3>(&native.camera_spring_velocityScale);
			data.camera_spring_spring_constant = plain::read_scalars<float, 3>(&native.camera_spring_springConstant);
			data.camera_spring_damper_constant = plain::read_scalars<float, 3>(&native.camera_spring_damperConstant);
			data.camera_spring_max_offset = plain::read_scalars<float, 3>(&native.camera_spring_maxOffset);
			data.viewmodel_spring_velocity_smoothing_sec = plain::read_scalars<float, 3>(&native.viewmodel_spring_velocitySmoothingSec);
			data.viewmodel_spring_velocity_scale = plain::read_scalars<float, 3>(&native.viewmodel_spring_velocityScale);
			data.viewmodel_spring_spring_constant = plain::read_scalars<float, 3>(&native.viewmodel_spring_springConstant);
			data.viewmodel_spring_damper_constant = plain::read_scalars<float, 3>(&native.viewmodel_spring_damperConstant);
			data.viewmodel_spring_max_offset = plain::read_scalars<float, 3>(&native.viewmodel_spring_maxOffset);
			data.viewmodel_spring_offset_to_angles_x = plain::read_scalars<float, 3>(&native.viewmodel_spring_offsetToAnglesX);
			data.viewmodel_spring_offset_to_angles_y = plain::read_scalars<float, 3>(&native.viewmodel_spring_offsetToAnglesY);
			data.viewmodel_spring_offset_to_angles_z = plain::read_scalars<float, 3>(&native.viewmodel_spring_offsetToAnglesZ);
			data.camera_shake_rotation = read_cinematic_shake_def(native.camera_shake_rotation);
			data.camera_shake_translation = read_cinematic_shake_def(native.camera_shake_translation);
			data.camera_shake_impact_rotation = read_cinematic_impact_shake_def(native.camera_shake_impact_rotation);
			data.camera_shake_impact_translation = read_cinematic_impact_shake_def(native.camera_shake_impact_translation);
			return data;
		}

		void write_cinematicmotion(CinematicMotionDef& native, const asset_data::gameplay::cinematicmotion& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			plain::write_scalars(&native.camera_spring_velocitySmoothingSec, data.camera_spring_velocity_smoothing_sec);
			plain::write_scalars(&native.camera_spring_velocityScale, data.camera_spring_velocity_scale);
			plain::write_scalars(&native.camera_spring_springConstant, data.camera_spring_spring_constant);
			plain::write_scalars(&native.camera_spring_damperConstant, data.camera_spring_damper_constant);
			plain::write_scalars(&native.camera_spring_maxOffset, data.camera_spring_max_offset);
			plain::write_scalars(&native.viewmodel_spring_velocitySmoothingSec, data.viewmodel_spring_velocity_smoothing_sec);
			plain::write_scalars(&native.viewmodel_spring_velocityScale, data.viewmodel_spring_velocity_scale);
			plain::write_scalars(&native.viewmodel_spring_springConstant, data.viewmodel_spring_spring_constant);
			plain::write_scalars(&native.viewmodel_spring_damperConstant, data.viewmodel_spring_damper_constant);
			plain::write_scalars(&native.viewmodel_spring_maxOffset, data.viewmodel_spring_max_offset);
			plain::write_scalars(&native.viewmodel_spring_offsetToAnglesX, data.viewmodel_spring_offset_to_angles_x);
			plain::write_scalars(&native.viewmodel_spring_offsetToAnglesY, data.viewmodel_spring_offset_to_angles_y);
			plain::write_scalars(&native.viewmodel_spring_offsetToAnglesZ, data.viewmodel_spring_offset_to_angles_z);
			write_cinematic_shake_def(native.camera_shake_rotation, data.camera_shake_rotation, resolve, memory);
			write_cinematic_shake_def(native.camera_shake_translation, data.camera_shake_translation, resolve, memory);
			write_cinematic_impact_shake_def(native.camera_shake_impact_rotation, data.camera_shake_impact_rotation, resolve, memory);
			write_cinematic_impact_shake_def(native.camera_shake_impact_translation, data.camera_shake_impact_translation, resolve, memory);
		}

		std::size_t size_cinematicmotion([[maybe_unused]] const asset_data::gameplay::cinematicmotion& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			size += size_cinematic_shake_def(data.camera_shake_rotation);
			size += size_cinematic_shake_def(data.camera_shake_translation);
			size += size_cinematic_impact_shake_def(data.camera_shake_impact_rotation);
			size += size_cinematic_impact_shake_def(data.camera_shake_impact_translation);
			return size;
		}

		asset_data::gameplay::decalvolumemask read_decalvolumemask(const GfxDecalVolumeMask& native)
		{
			plain::trace("read_decalvolumemask");
			asset_data::gameplay::decalvolumemask data;
			data.name = plain::read_string(native.name);
			data.blend_map = plain::read_asset(ASSET_TYPE_IMAGE, native.blendMap);
			return data;
		}

		void write_decalvolumemask(GfxDecalVolumeMask& native, const asset_data::gameplay::decalvolumemask& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.blendMap = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, data.blend_map);
		}

		std::size_t size_decalvolumemask([[maybe_unused]] const asset_data::gameplay::decalvolumemask& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			return size;
		}

		asset_data::gameplay::gesture_weapon_settings read_gesture_weapon_settings(const GestureWeaponSettings& native)
		{
			plain::trace("read_gesture_weapon_settings");
			asset_data::gameplay::gesture_weapon_settings data;
			data.blend_to_states = native.blendToStates;
			data.hide_reticle = native.hideReticle;
			data.fire_delay = native.fireDelay;
			data.sprint_delay = native.sprintDelay;
			data.use_left_idle_akimbo = native.useLeftIdleAkimbo;
			data.split_anims_akimbo = native.splitAnimsAkimbo;
			data.blend_to_demeanor_loop = native.blendToDemeanorLoop;
			data.blend_out_raise = native.blendOutRaise;
			data.blend_out_finger_pose = native.blendOutFingerPose;
			data.blend_out_additive_ads = native.blendOutAdditiveADS;
			return data;
		}

		void write_gesture_weapon_settings(GestureWeaponSettings& native, const asset_data::gameplay::gesture_weapon_settings& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.blendToStates = data.blend_to_states;
			native.hideReticle = data.hide_reticle;
			native.fireDelay = data.fire_delay;
			native.sprintDelay = data.sprint_delay;
			native.useLeftIdleAkimbo = data.use_left_idle_akimbo;
			native.splitAnimsAkimbo = data.split_anims_akimbo;
			native.blendToDemeanorLoop = data.blend_to_demeanor_loop;
			native.blendOutRaise = data.blend_out_raise;
			native.blendOutFingerPose = data.blend_out_finger_pose;
			native.blendOutAdditiveADS = data.blend_out_additive_ads;
		}

		std::size_t size_gesture_weapon_settings([[maybe_unused]] const asset_data::gameplay::gesture_weapon_settings& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gameplay::gesture_directional_settings read_gesture_directional_settings(const GestureDirectionalSettings& native)
		{
			plain::trace("read_gesture_directional_settings");
			asset_data::gameplay::gesture_directional_settings data;
			data.max_angle = native.maxAngle;
			data.lerp_at_max_angle = native.lerpAtMaxAngle;
			data.width_cushion_angle = native.widthCushionAngle;
			data.lerp_at_min_cushion_angle = native.lerpAtMinCushionAngle;
			data.lerp_at_max_cushion_angle = native.lerpAtMaxCushionAngle;
			data.limit_left = native.limitLeft;
			data.limit_right = native.limitRight;
			data.limit_up = native.limitUp;
			data.limit_down = native.limitDown;
			data.use_target_offset = native.useTargetOffset;
			data.target_offset_x = native.targetOffsetX;
			data.target_offset_y = native.targetOffsetY;
			data.target_offset_z = native.targetOffsetZ;
			data.target_offset_yaw = native.targetOffsetYaw;
			data.target_offset_pitch = native.targetOffsetPitch;
			data.target_offset_roll = native.targetOffsetRoll;
			data.ignore_view_pitch_for_target_offset = native.ignoreViewPitchForTargetOffset;
			data.ignore_view_yaw_for_target_offset = native.ignoreViewYawForTargetOffset;
			data.ignore_view_roll_for_target_offset = native.ignoreViewRollForTargetOffset;
			return data;
		}

		void write_gesture_directional_settings(GestureDirectionalSettings& native, const asset_data::gameplay::gesture_directional_settings& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.maxAngle = data.max_angle;
			native.lerpAtMaxAngle = data.lerp_at_max_angle;
			native.widthCushionAngle = data.width_cushion_angle;
			native.lerpAtMinCushionAngle = data.lerp_at_min_cushion_angle;
			native.lerpAtMaxCushionAngle = data.lerp_at_max_cushion_angle;
			native.limitLeft = data.limit_left;
			native.limitRight = data.limit_right;
			native.limitUp = data.limit_up;
			native.limitDown = data.limit_down;
			native.useTargetOffset = data.use_target_offset;
			native.targetOffsetX = data.target_offset_x;
			native.targetOffsetY = data.target_offset_y;
			native.targetOffsetZ = data.target_offset_z;
			native.targetOffsetYaw = data.target_offset_yaw;
			native.targetOffsetPitch = data.target_offset_pitch;
			native.targetOffsetRoll = data.target_offset_roll;
			native.ignoreViewPitchForTargetOffset = data.ignore_view_pitch_for_target_offset;
			native.ignoreViewYawForTargetOffset = data.ignore_view_yaw_for_target_offset;
			native.ignoreViewRollForTargetOffset = data.ignore_view_roll_for_target_offset;
		}

		std::size_t size_gesture_directional_settings([[maybe_unused]] const asset_data::gameplay::gesture_directional_settings& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gameplay::gesture_look_around_settings read_gesture_look_around_settings(const GestureLookAroundSettings& native)
		{
			plain::trace("read_gesture_look_around_settings");
			asset_data::gameplay::gesture_look_around_settings data;
			data.yaw_lerp_in = native.yawLerpIn;
			data.yaw_lerp_out = native.yawLerpOut;
			data.walk_time = native.walkTime;
			data.walk_foot_step_anim = read_footstep_anim(native.walkFootStepAnim);
			return data;
		}

		void write_gesture_look_around_settings(GestureLookAroundSettings& native, const asset_data::gameplay::gesture_look_around_settings& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.yawLerpIn = data.yaw_lerp_in;
			native.yawLerpOut = data.yaw_lerp_out;
			native.walkTime = data.walk_time;
			write_footstep_anim(native.walkFootStepAnim, data.walk_foot_step_anim, resolve, memory);
		}

		std::size_t size_gesture_look_around_settings([[maybe_unused]] const asset_data::gameplay::gesture_look_around_settings& data)
		{
			std::size_t size = 0;
			size += size_footstep_anim(data.walk_foot_step_anim);
			return size;
		}

		asset_data::gameplay::gesture_ik_target_settings read_gesture_ik_target_settings(const GestureIKTargetSettings& native)
		{
			plain::trace("read_gesture_ik_target_settings");
			asset_data::gameplay::gesture_ik_target_settings data;
			data.target_entity_bone_name = plain::read_script_string(native.targetEntityBoneName);
			return data;
		}

		void write_gesture_ik_target_settings(GestureIKTargetSettings& native, const asset_data::gameplay::gesture_ik_target_settings& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.targetEntityBoneName = static_cast<scr_string_t>(plain::write_script_string(data.target_entity_bone_name));
		}

		std::size_t size_gesture_ik_target_settings([[maybe_unused]] const asset_data::gameplay::gesture_ik_target_settings& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gameplay::gesture read_gesture(const Gesture& native)
		{
			plain::trace("read_gesture");
			asset_data::gameplay::gesture data;
			data.name = plain::read_string(native.name);
			data.type = plain::enum_name(asset_data::gameplay::gesture_type_names, static_cast<std::int64_t>(native.type), "Gesture.type");
			data.priority = plain::enum_name(asset_data::gameplay::gesture_priority_names, static_cast<std::int64_t>(native.priority), "Gesture.priority");
			data.looping = native.looping;
			data.anims = plain::read_fixed_with<22>(native.anims, [&](const auto& item) { return plain::read_asset(ASSET_TYPE_XANIMPARTS, item); });
			data.weapon_settings = read_gesture_weapon_settings(native.weaponSettings);
			data.directional_settings = read_gesture_directional_settings(native.directionalSettings);
			data.look_around_settings = read_gesture_look_around_settings(native.lookAroundSettings);
			data.ik_target_settings = read_gesture_ik_target_settings(native.ikTargetSettings);
			return data;
		}

		void write_gesture(Gesture& native, const asset_data::gameplay::gesture& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.type = static_cast<GestureType>(plain::enum_value(asset_data::gameplay::gesture_type_names, data.type, "Gesture.type"));
			native.priority = static_cast<GesturePriority>(plain::enum_value(asset_data::gameplay::gesture_priority_names, data.priority, "Gesture.priority"));
			native.looping = data.looping;
			if (data.anims)
			{
				native.anims = memory.manual_allocate<XAnimParts*>(plain::rounded(22 * sizeof(XAnimParts*)));
				for (std::size_t i = 0; i < 22; ++i)
				{
					native.anims[i] = plain::resolve_asset<XAnimParts>(resolve, ASSET_TYPE_XANIMPARTS, (*data.anims)[i]);
				}
			}
			write_gesture_weapon_settings(native.weaponSettings, data.weapon_settings, resolve, memory);
			write_gesture_directional_settings(native.directionalSettings, data.directional_settings, resolve, memory);
			write_gesture_look_around_settings(native.lookAroundSettings, data.look_around_settings, resolve, memory);
			write_gesture_ik_target_settings(native.ikTargetSettings, data.ik_target_settings, resolve, memory);
		}

		std::size_t size_gesture([[maybe_unused]] const asset_data::gameplay::gesture& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.anims)
			{
				size += plain::sized(22 * sizeof(XAnimParts*));
			}
			size += size_gesture_weapon_settings(data.weapon_settings);
			size += size_gesture_directional_settings(data.directional_settings);
			size += size_gesture_look_around_settings(data.look_around_settings);
			size += size_gesture_ik_target_settings(data.ik_target_settings);
			return size;
		}

		asset_data::gameplay::reticle read_reticle(const ReticleDef& native)
		{
			plain::trace("read_reticle");
			asset_data::gameplay::reticle data;
			data.name = plain::read_string(native.name);
			data.widget_name = plain::read_string(native.widgetName);
			data.lock_reticle_to_tag = native.lockReticleToTag;
			data.primary_affected_by_emp = native.primaryAffectedByEMP;
			data.secondary_affected_by_emp = native.secondaryAffectedByEMP;
			data.offset_from_tag = native.offsetFromTag;
			data.scale_with_fov = native.scaleWithFOV;
			data.primary_default_scale = native.primaryDefaultScale;
			data.secondary_default_scale = native.secondaryDefaultScale;
			data.secondary_widget_name = plain::read_string(native.secondaryWidgetName);
			data.secondary_lock_reticle_to_tag = native.secondaryLockReticleToTag;
			data.secondary_offset_from_tag = native.secondaryOffsetFromTag;
			data.natural_distance = native.naturalDistance;
			data.natural_fov = native.naturalFOV;
			data.min_reticle_scale = native.minReticleScale;
			data.max_reticle_scale = native.maxReticleScale;
			return data;
		}

		void write_reticle(ReticleDef& native, const asset_data::gameplay::reticle& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.widgetName = plain::write_string(memory, data.widget_name);
			native.lockReticleToTag = data.lock_reticle_to_tag;
			native.primaryAffectedByEMP = data.primary_affected_by_emp;
			native.secondaryAffectedByEMP = data.secondary_affected_by_emp;
			native.offsetFromTag = data.offset_from_tag;
			native.scaleWithFOV = data.scale_with_fov;
			native.primaryDefaultScale = data.primary_default_scale;
			native.secondaryDefaultScale = data.secondary_default_scale;
			native.secondaryWidgetName = plain::write_string(memory, data.secondary_widget_name);
			native.secondaryLockReticleToTag = data.secondary_lock_reticle_to_tag;
			native.secondaryOffsetFromTag = data.secondary_offset_from_tag;
			native.naturalDistance = data.natural_distance;
			native.naturalFOV = data.natural_fov;
			native.minReticleScale = data.min_reticle_scale;
			native.maxReticleScale = data.max_reticle_scale;
		}

		std::size_t size_reticle([[maybe_unused]] const asset_data::gameplay::reticle& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			size += plain::string_size(data.widget_name);
			size += plain::string_size(data.secondary_widget_name);
			return size;
		}

		asset_data::gameplay::alwaysloaded_flags read_alwaysloaded_flags(const AlwaysloadedFlags& native)
		{
			plain::trace("read_alwaysloaded_flags");
			asset_data::gameplay::alwaysloaded_flags data;
			data.images = plain::read_array(native.images, native.imageCount, "AlwaysloadedFlags.images", [&](const auto& item) { return plain::read_asset(ASSET_TYPE_IMAGE, item); });
			data.image_flags = plain::read_array(native.imageFlags, (((native.imageCount * 4) + 31) >> 5), "AlwaysloadedFlags.imageFlags", [&](const auto& item) { return item; });
			return data;
		}

		void write_alwaysloaded_flags(AlwaysloadedFlags& native, const asset_data::gameplay::alwaysloaded_flags& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.images = plain::write_array<GfxImage*>(data.images, memory, [&](auto& out, const auto& in) { out = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, in); });
			native.imageFlags = plain::write_array<std::uint32_t>(data.image_flags, memory, [&](auto& out, const auto& in) { out = in; });
			native.imageCount = plain::count_of<std::uint32_t>(data.images);
		}

		std::size_t size_alwaysloaded_flags([[maybe_unused]] const asset_data::gameplay::alwaysloaded_flags& data)
		{
			std::size_t size = 0;
			if (data.images)
			{
				size += plain::sized(std::max<std::size_t>(data.images->size(), 1) * sizeof(GfxImage*));
			}
			if (data.image_flags)
			{
				size += plain::sized(std::max<std::size_t>(data.image_flags->size(), 1) * sizeof(std::uint32_t));
			}
			return size;
		}

		asset_data::gameplay::alwaysloaded_flag_set read_alwaysloaded_flag_set(const AlwaysloadedFlagSet& native)
		{
			plain::trace("read_alwaysloaded_flag_set");
			asset_data::gameplay::alwaysloaded_flag_set data;
			data.global_flags = read_alwaysloaded_flags(native.globalFlags);
			data.per_model_flags = plain::read_array(native.perModelFlags, native.modelCount, "AlwaysloadedFlagSet.perModelFlags", [&](const auto& item) { return read_alwaysloaded_flags_per_model(item); });
			data.model_surfs = plain::read_array(native.modelSurfs, native.modelSurfCount, "AlwaysloadedFlagSet.modelSurfs", [&](const auto& item) { return plain::read_asset(ASSET_TYPE_XMODEL_SURFS, item); });
			return data;
		}

		void write_alwaysloaded_flag_set(AlwaysloadedFlagSet& native, const asset_data::gameplay::alwaysloaded_flag_set& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_alwaysloaded_flags(native.globalFlags, data.global_flags, resolve, memory);
			native.perModelFlags = plain::write_array<AlwaysloadedFlagsPerModel>(data.per_model_flags, memory, [&](auto& out, const auto& in) { write_alwaysloaded_flags_per_model(out, in, resolve, memory); });
			native.modelSurfs = plain::write_array<XModelSurfs*>(data.model_surfs, memory, [&](auto& out, const auto& in) { out = plain::resolve_asset<XModelSurfs>(resolve, ASSET_TYPE_XMODEL_SURFS, in); });
			native.modelCount = plain::count_of<std::uint32_t>(data.per_model_flags);
			native.modelSurfCount = plain::count_of<std::uint32_t>(data.model_surfs);
		}

		std::size_t size_alwaysloaded_flag_set([[maybe_unused]] const asset_data::gameplay::alwaysloaded_flag_set& data)
		{
			std::size_t size = 0;
			size += size_alwaysloaded_flags(data.global_flags);
			if (data.per_model_flags)
			{
				size += plain::sized(std::max<std::size_t>(data.per_model_flags->size(), 1) * sizeof(AlwaysloadedFlagsPerModel));
				for (const auto& item : *data.per_model_flags)
				{
					size += size_alwaysloaded_flags_per_model(item);
				}
			}
			if (data.model_surfs)
			{
				size += plain::sized(std::max<std::size_t>(data.model_surfs->size(), 1) * sizeof(XModelSurfs*));
			}
			return size;
		}

		asset_data::gameplay::transient_sets read_transient_sets(const TransientSets& native)
		{
			plain::trace("read_transient_sets");
			asset_data::gameplay::transient_sets data;
			data.count = native.count;
			data.names = plain::read_array(native.names, native.count, "TransientSets.names", [&](const auto& item) { return plain::read_string(item); });
			data.masks = plain::read_array(native.masks, native.count, "TransientSets.masks", [&](const auto& item) { return read_bitarray_32(item); });
			return data;
		}

		void write_transient_sets(TransientSets& native, const asset_data::gameplay::transient_sets& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.count = data.count;
			native.names = plain::write_array<const char*>(data.names, memory, [&](auto& out, const auto& in) { out = plain::write_string(memory, in); });
			native.masks = plain::write_array<bitarray_32>(data.masks, memory, [&](auto& out, const auto& in) { write_bitarray_32(out, in, resolve, memory); });
		}

		std::size_t size_transient_sets([[maybe_unused]] const asset_data::gameplay::transient_sets& data)
		{
			std::size_t size = 0;
			if (data.names)
			{
				size += plain::sized(std::max<std::size_t>(data.names->size(), 1) * sizeof(const char*));
				for (const auto& item : *data.names)
				{
					size += plain::string_size(item);
				}
			}
			if (data.masks)
			{
				size += plain::sized(std::max<std::size_t>(data.masks->size(), 1) * sizeof(bitarray_32));
				for (const auto& item : *data.masks)
				{
					size += size_bitarray_32(item);
				}
			}
			return size;
		}

		asset_data::gameplay::streaming_content_set read_streaming_content_set(const StreamingContentSet& native)
		{
			plain::trace("read_streaming_content_set");
			asset_data::gameplay::streaming_content_set data;
			plain::require_zero(native.contentCounts, "StreamingContentSet.contentCounts");
			plain::require_zero(native.contents, "StreamingContentSet.contents");
			return data;
		}

		void write_streaming_content_set(StreamingContentSet& native, const asset_data::gameplay::streaming_content_set& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
		}

		std::size_t size_streaming_content_set([[maybe_unused]] const asset_data::gameplay::streaming_content_set& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gameplay::transient_costs read_transient_costs(const TransientCosts& native)
		{
			plain::trace("read_transient_costs");
			asset_data::gameplay::transient_costs data;
			data.stream_contents = read_streaming_content_set(native.streamContents);
			data.streaming_costs = plain::read_array(native.streamingCosts, native.streamingItemCount, "TransientCosts.streamingCosts", [&](const auto& item) { return read_streaming_item_cost(item); });
			data.resident_page_sizes = plain::read_array(native.residentPageSizes, native.transientCostCount, "TransientCosts.residentPageSizes", [&](const auto& item) { return item; });
			data.temp_page_sizes = plain::read_array(native.tempPageSizes, native.transientCostCount, "TransientCosts.tempPageSizes", [&](const auto& item) { return item; });
			return data;
		}

		void write_transient_costs(TransientCosts& native, const asset_data::gameplay::transient_costs& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_streaming_content_set(native.streamContents, data.stream_contents, resolve, memory);
			native.streamingCosts = plain::write_array<StreamingItemCost>(data.streaming_costs, memory, [&](auto& out, const auto& in) { write_streaming_item_cost(out, in, resolve, memory); });
			native.residentPageSizes = plain::write_array<std::uint16_t>(data.resident_page_sizes, memory, [&](auto& out, const auto& in) { out = in; });
			native.tempPageSizes = plain::write_array<std::uint16_t>(data.temp_page_sizes, memory, [&](auto& out, const auto& in) { out = in; });
			native.streamingItemCount = plain::count_of<std::uint32_t>(data.streaming_costs);
			native.transientCostCount = plain::count_of<std::uint32_t>(data.resident_page_sizes);
		}

		std::size_t size_transient_costs([[maybe_unused]] const asset_data::gameplay::transient_costs& data)
		{
			std::size_t size = 0;
			size += size_streaming_content_set(data.stream_contents);
			if (data.streaming_costs)
			{
				size += plain::sized(std::max<std::size_t>(data.streaming_costs->size(), 1) * sizeof(StreamingItemCost));
				for (const auto& item : *data.streaming_costs)
				{
					size += size_streaming_item_cost(item);
				}
			}
			if (data.resident_page_sizes)
			{
				size += plain::sized(std::max<std::size_t>(data.resident_page_sizes->size(), 1) * sizeof(std::uint16_t));
			}
			if (data.temp_page_sizes)
			{
				size += plain::sized(std::max<std::size_t>(data.temp_page_sizes->size(), 1) * sizeof(std::uint16_t));
			}
			return size;
		}

		asset_data::gameplay::spatial_partition_tree read_spatial_partition_tree(const SpatialPartition_Tree& native)
		{
			plain::trace("read_spatial_partition_tree");
			asset_data::gameplay::spatial_partition_tree data;
			data.tree = plain::read_array(native.tree, native.treeSize, "SpatialPartition_Tree.tree", [&](const auto& item) { return item; });
			data.always_list = plain::read_array(native.alwaysList, native.alwaysListLength, "SpatialPartition_Tree.alwaysList", [&](const auto& item) { return item; });
			return data;
		}

		void write_spatial_partition_tree(SpatialPartition_Tree& native, const asset_data::gameplay::spatial_partition_tree& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.tree = plain::write_array<std::uint8_t>(data.tree, memory, [&](auto& out, const auto& in) { out = in; });
			native.alwaysList = plain::write_array<std::uint32_t>(data.always_list, memory, [&](auto& out, const auto& in) { out = in; });
			native.treeSize = plain::count_of<std::uint32_t>(data.tree);
			native.alwaysListLength = plain::count_of<std::uint32_t>(data.always_list);
		}

		std::size_t size_spatial_partition_tree([[maybe_unused]] const asset_data::gameplay::spatial_partition_tree& data)
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

		asset_data::gameplay::bitarray_dynamic read_bitarray_dynamic(const bitarray_dynamic& native)
		{
			plain::trace("read_bitarray_dynamic");
			asset_data::gameplay::bitarray_dynamic data;
			data.array = plain::read_array(native.array, native.wordCount, "bitarray_dynamic.array", [&](const auto& item) { return item; });
			data.bit_count = native.bitCount;
			return data;
		}

		void write_bitarray_dynamic(bitarray_dynamic& native, const asset_data::gameplay::bitarray_dynamic& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.array = plain::write_array<std::uint32_t>(data.array, memory, [&](auto& out, const auto& in) { out = in; });
			native.bitCount = data.bit_count;
			native.wordCount = plain::count_of<std::uint32_t>(data.array);
		}

		std::size_t size_bitarray_dynamic([[maybe_unused]] const asset_data::gameplay::bitarray_dynamic& data)
		{
			std::size_t size = 0;
			if (data.array)
			{
				size += plain::sized(std::max<std::size_t>(data.array->size(), 1) * sizeof(std::uint32_t));
			}
			return size;
		}

		asset_data::gameplay::transient_info read_transient_info(const TransientInfo& native)
		{
			plain::trace("read_transient_info");
			asset_data::gameplay::transient_info data;
			data.transient_sets = read_transient_sets(native.transientSets);
			data.transient_costs = read_transient_costs(native.transientCosts);
			data.forced_stub_assets = plain::read_array(native.forcedStubAssets, native.forcedStubAssetCount, "TransientInfo.forcedStubAssets", [&](const auto& item) { return read_forced_stub_asset_reference(item); });
			data.transient_files = plain::read_array(native.transientFiles, native.transientFileCount, "TransientInfo.transientFiles", [&](const auto& item) { return read_transient_file_reference(item); });
			data.transient_assets = plain::read_array(native.transientAssets, native.transientAssetCount, "TransientInfo.transientAssets", [&](const auto& item) { return read_transient_asset_reference(item); });
			data.transient_world_grid_tile_maps = plain::read_array(native.transientWorldGridTileMaps, native.transientWorldGridTileMapCount, "TransientInfo.transientWorldGridTileMaps", [&](const auto& item) { return read_transient_world_grid_tile_map(item); });
			data.transient_world_grid_spatial = plain::read_single(native.transientWorldGridSpatial, [&](const auto& item) { return read_spatial_partition_tree(item); });
			data.transient_world_grid_bounds = plain::read_array(native.transientWorldGridBounds, native.transientWorldGridBoundsCount, "TransientInfo.transientWorldGridBounds", [&](const auto& item) { return read_bounds(item); });
			data.transient_world_grid_high_lod_lookup = plain::read_array(native.transientWorldGridHighLodLookup, native.transientWorldGridHighLodLookupCount, "TransientInfo.transientWorldGridHighLodLookup", [&](const auto& item) { return item; });
			data.transient_world_grid_high_lod_count = native.transientWorldGridHighLodCount;
			data.transient_info_flags = plain::flag_names(asset_data::gameplay::transient_info_flags_names, static_cast<std::uint64_t>(native.transientInfoFlags), "TransientInfo.transientInfoFlags");
			data.transient_skip_loading = read_bitarray_dynamic(native.transientSkipLoading);
			data.transient_collision_tile_count = native.transientCollisionTileCount;
			data.transient_mp_common_count = native.transientMPCommonCount;
			return data;
		}

		void write_transient_info(TransientInfo& native, const asset_data::gameplay::transient_info& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_transient_sets(native.transientSets, data.transient_sets, resolve, memory);
			write_transient_costs(native.transientCosts, data.transient_costs, resolve, memory);
			native.forcedStubAssets = plain::write_array<ForcedStubAssetReference>(data.forced_stub_assets, memory, [&](auto& out, const auto& in) { write_forced_stub_asset_reference(out, in, resolve, memory); });
			native.transientFiles = plain::write_array<TransientFileReference>(data.transient_files, memory, [&](auto& out, const auto& in) { write_transient_file_reference(out, in, resolve, memory); });
			native.transientAssets = plain::write_array<TransientAssetReference>(data.transient_assets, memory, [&](auto& out, const auto& in) { write_transient_asset_reference(out, in, resolve, memory); });
			native.transientWorldGridTileMaps = plain::write_array<TransientWorldGridTileMap>(data.transient_world_grid_tile_maps, memory, [&](auto& out, const auto& in) { write_transient_world_grid_tile_map(out, in, resolve, memory); });
			native.transientWorldGridSpatial = plain::write_single<SpatialPartition_Tree>(data.transient_world_grid_spatial, memory, [&](auto& out, const auto& in) { write_spatial_partition_tree(out, in, resolve, memory); });
			native.transientWorldGridBounds = plain::write_array<Bounds>(data.transient_world_grid_bounds, memory, [&](auto& out, const auto& in) { write_bounds(out, in, resolve, memory); });
			native.transientWorldGridHighLodLookup = plain::write_array<std::uint16_t>(data.transient_world_grid_high_lod_lookup, memory, [&](auto& out, const auto& in) { out = in; });
			native.transientWorldGridHighLodCount = data.transient_world_grid_high_lod_count;
			native.transientInfoFlags = static_cast<TransientInfoFlags>(plain::flag_value(asset_data::gameplay::transient_info_flags_names, data.transient_info_flags, "TransientInfo.transientInfoFlags"));
			write_bitarray_dynamic(native.transientSkipLoading, data.transient_skip_loading, resolve, memory);
			native.transientCollisionTileCount = data.transient_collision_tile_count;
			native.transientMPCommonCount = data.transient_mp_common_count;
			native.forcedStubAssetCount = plain::count_of<std::uint32_t>(data.forced_stub_assets);
			native.transientFileCount = plain::count_of<std::uint32_t>(data.transient_files);
			native.transientAssetCount = plain::count_of<std::uint32_t>(data.transient_assets);
			native.transientWorldGridTileMapCount = plain::count_of<std::uint32_t>(data.transient_world_grid_tile_maps);
			native.transientWorldGridBoundsCount = plain::count_of<std::uint32_t>(data.transient_world_grid_bounds);
			native.transientWorldGridHighLodLookupCount = plain::count_of<std::uint32_t>(data.transient_world_grid_high_lod_lookup);
		}

		std::size_t size_transient_info([[maybe_unused]] const asset_data::gameplay::transient_info& data)
		{
			std::size_t size = 0;
			size += size_transient_sets(data.transient_sets);
			size += size_transient_costs(data.transient_costs);
			if (data.forced_stub_assets)
			{
				size += plain::sized(std::max<std::size_t>(data.forced_stub_assets->size(), 1) * sizeof(ForcedStubAssetReference));
				for (const auto& item : *data.forced_stub_assets)
				{
					size += size_forced_stub_asset_reference(item);
				}
			}
			if (data.transient_files)
			{
				size += plain::sized(std::max<std::size_t>(data.transient_files->size(), 1) * sizeof(TransientFileReference));
				for (const auto& item : *data.transient_files)
				{
					size += size_transient_file_reference(item);
				}
			}
			if (data.transient_assets)
			{
				size += plain::sized(std::max<std::size_t>(data.transient_assets->size(), 1) * sizeof(TransientAssetReference));
				for (const auto& item : *data.transient_assets)
				{
					size += size_transient_asset_reference(item);
				}
			}
			if (data.transient_world_grid_tile_maps)
			{
				size += plain::sized(std::max<std::size_t>(data.transient_world_grid_tile_maps->size(), 1) * sizeof(TransientWorldGridTileMap));
				for (const auto& item : *data.transient_world_grid_tile_maps)
				{
					size += size_transient_world_grid_tile_map(item);
				}
			}
			if (data.transient_world_grid_spatial)
			{
				size += plain::sized(sizeof(SpatialPartition_Tree));
				size += size_spatial_partition_tree(*data.transient_world_grid_spatial);
			}
			if (data.transient_world_grid_bounds)
			{
				size += plain::sized(std::max<std::size_t>(data.transient_world_grid_bounds->size(), 1) * sizeof(Bounds));
				for (const auto& item : *data.transient_world_grid_bounds)
				{
					size += size_bounds(item);
				}
			}
			if (data.transient_world_grid_high_lod_lookup)
			{
				size += plain::sized(std::max<std::size_t>(data.transient_world_grid_high_lod_lookup->size(), 1) * sizeof(std::uint16_t));
			}
			size += size_bitarray_dynamic(data.transient_skip_loading);
			return size;
		}

		asset_data::gameplay::streaminginfo read_streaminginfo(const StreamingInfo& native)
		{
			plain::trace("read_streaminginfo");
			asset_data::gameplay::streaminginfo data;
			data.name = plain::read_string(native.name);
			data.alwaysloaded_flag_sets = [&] { std::array<asset_data::gameplay::alwaysloaded_flag_set, 1> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_alwaysloaded_flag_set(native.alwaysloadedFlagSets[i]); } return out; }();
			data.transient_info = plain::read_single(native.transientInfo, [&](const auto& item) { return read_transient_info(item); });
			return data;
		}

		void write_streaminginfo(StreamingInfo& native, const asset_data::gameplay::streaminginfo& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			for (std::size_t i = 0; i < data.alwaysloaded_flag_sets.size(); ++i)
			{
				write_alwaysloaded_flag_set(native.alwaysloadedFlagSets[i], data.alwaysloaded_flag_sets[i], resolve, memory);
			}
			native.transientInfo = plain::write_single<TransientInfo>(data.transient_info, memory, [&](auto& out, const auto& in) { write_transient_info(out, in, resolve, memory); });
		}

		std::size_t size_streaminginfo([[maybe_unused]] const asset_data::gameplay::streaminginfo& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			for (const auto& item : data.alwaysloaded_flag_sets)
			{
				size += size_alwaysloaded_flag_set(item);
			}
			if (data.transient_info)
			{
				size += plain::sized(sizeof(TransientInfo));
				size += size_transient_info(*data.transient_info);
			}
			return size;
		}

		asset_data::gameplay::alwaysloaded_flags_per_model read_alwaysloaded_flags_per_model(const AlwaysloadedFlagsPerModel& native)
		{
			plain::trace("read_alwaysloaded_flags_per_model");
			asset_data::gameplay::alwaysloaded_flags_per_model data;
			data.model = plain::read_asset(ASSET_TYPE_XMODEL, native.model);
			data.flags = read_alwaysloaded_flags(native.flags);
			return data;
		}

		void write_alwaysloaded_flags_per_model(AlwaysloadedFlagsPerModel& native, const asset_data::gameplay::alwaysloaded_flags_per_model& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.model = plain::resolve_asset<XModel>(resolve, ASSET_TYPE_XMODEL, data.model);
			write_alwaysloaded_flags(native.flags, data.flags, resolve, memory);
		}

		std::size_t size_alwaysloaded_flags_per_model([[maybe_unused]] const asset_data::gameplay::alwaysloaded_flags_per_model& data)
		{
			std::size_t size = 0;
			size += size_alwaysloaded_flags(data.flags);
			return size;
		}

		asset_data::gameplay::bitarray_32 read_bitarray_32(const bitarray_32& native)
		{
			plain::trace("read_bitarray_32");
			asset_data::gameplay::bitarray_32 data;
			data.array = std::to_array(native.array);
			return data;
		}

		void write_bitarray_32(bitarray_32& native, const asset_data::gameplay::bitarray_32& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			std::ranges::copy(data.array, native.array);
		}

		std::size_t size_bitarray_32([[maybe_unused]] const asset_data::gameplay::bitarray_32& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gameplay::streaming_item_cost read_streaming_item_cost(const StreamingItemCost& native)
		{
			plain::trace("read_streaming_item_cost");
			asset_data::gameplay::streaming_item_cost data;
			data.cost_in_bytes = native.costInBytes;
			return data;
		}

		void write_streaming_item_cost(StreamingItemCost& native, const asset_data::gameplay::streaming_item_cost& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.costInBytes = data.cost_in_bytes;
		}

		std::size_t size_streaming_item_cost([[maybe_unused]] const asset_data::gameplay::streaming_item_cost& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gameplay::forced_stub_asset_reference read_forced_stub_asset_reference(const ForcedStubAssetReference& native)
		{
			plain::trace("read_forced_stub_asset_reference");
			asset_data::gameplay::forced_stub_asset_reference data;
			data.name = plain::read_string(native.name);
			data.type = native.type;
			return data;
		}

		void write_forced_stub_asset_reference(ForcedStubAssetReference& native, const asset_data::gameplay::forced_stub_asset_reference& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.type = data.type;
		}

		std::size_t size_forced_stub_asset_reference([[maybe_unused]] const asset_data::gameplay::forced_stub_asset_reference& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			return size;
		}

		asset_data::gameplay::transient_file_reference read_transient_file_reference(const TransientFileReference& native)
		{
			plain::trace("read_transient_file_reference");
			asset_data::gameplay::transient_file_reference data;
			data.name = plain::read_string(native.name);
			data.type = native.type;
			return data;
		}

		void write_transient_file_reference(TransientFileReference& native, const asset_data::gameplay::transient_file_reference& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.type = data.type;
		}

		std::size_t size_transient_file_reference([[maybe_unused]] const asset_data::gameplay::transient_file_reference& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			return size;
		}

		asset_data::gameplay::transient_asset_reference read_transient_asset_reference(const TransientAssetReference& native)
		{
			plain::trace("read_transient_asset_reference");
			asset_data::gameplay::transient_asset_reference data;
			data.name = plain::read_string(native.name);
			data.type = native.type;
			data.mp_parent_index = native.mpParentIndex;
			return data;
		}

		void write_transient_asset_reference(TransientAssetReference& native, const asset_data::gameplay::transient_asset_reference& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.type = data.type;
			native.mpParentIndex = data.mp_parent_index;
		}

		std::size_t size_transient_asset_reference([[maybe_unused]] const asset_data::gameplay::transient_asset_reference& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			return size;
		}

		asset_data::gameplay::transient_world_grid_tile_map read_transient_world_grid_tile_map(const TransientWorldGridTileMap& native)
		{
			plain::trace("read_transient_world_grid_tile_map");
			asset_data::gameplay::transient_world_grid_tile_map data;
			data.space_to_transient_index = plain::read_array(native.spaceToTransientIndex, native.indexCount, "TransientWorldGridTileMap.spaceToTransientIndex", [&](const auto& item) { return item; });
			data.level = native.level;
			data.lod_count = native.lodCount;
			data.grid_count_x = native.gridCountX;
			data.grid_count_y = native.gridCountY;
			data.cell_size = native.cellSize;
			data.min_x = native.minX;
			data.min_y = native.minY;
			return data;
		}

		void write_transient_world_grid_tile_map(TransientWorldGridTileMap& native, const asset_data::gameplay::transient_world_grid_tile_map& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.spaceToTransientIndex = plain::write_array<std::uint16_t>(data.space_to_transient_index, memory, [&](auto& out, const auto& in) { out = in; });
			native.level = data.level;
			native.lodCount = data.lod_count;
			native.gridCountX = data.grid_count_x;
			native.gridCountY = data.grid_count_y;
			native.cellSize = data.cell_size;
			native.minX = data.min_x;
			native.minY = data.min_y;
			native.indexCount = plain::count_of<std::uint32_t>(data.space_to_transient_index);
		}

		std::size_t size_transient_world_grid_tile_map([[maybe_unused]] const asset_data::gameplay::transient_world_grid_tile_map& data)
		{
			std::size_t size = 0;
			if (data.space_to_transient_index)
			{
				size += plain::sized(std::max<std::size_t>(data.space_to_transient_index->size(), 1) * sizeof(std::uint16_t));
			}
			return size;
		}

		asset_data::gameplay::bounds read_bounds(const Bounds& native)
		{
			plain::trace("read_bounds");
			asset_data::gameplay::bounds data;
			data.mid_point = plain::read_scalars<float, 3>(&native.midPoint);
			data.half_size = plain::read_scalars<float, 3>(&native.halfSize);
			return data;
		}

		void write_bounds(Bounds& native, const asset_data::gameplay::bounds& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.midPoint, data.mid_point);
			plain::write_scalars(&native.halfSize, data.half_size);
		}

		std::size_t size_bounds([[maybe_unused]] const asset_data::gameplay::bounds& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gameplay::suit read_suit(const SuitDef& native)
		{
			plain::trace("read_suit");
			asset_data::gameplay::suit data;
			data.name = plain::read_string(native.name);
			data.cam_min_velocity_for_fov_increase = native.cam_minVelocityForFovIncrease;
			data.cam_max_velocity_for_fov_increase = native.cam_maxVelocityForFovIncrease;
			data.cam_velocity_to_decrease_fov = native.cam_velocityToDecreaseFov;
			data.cam_fov_increase_at_max_velocity = native.cam_fovIncreaseAtMaxVelocity;
			data.cam_one_over_fov_ease_in_time_ms = native.cam_oneOverFovEaseInTimeMs;
			data.cam_one_over_fov_ease_out_time_ms = native.cam_oneOverFovEaseOutTimeMs;
			data.cam_helmet_cam = plain::read_asset(ASSET_TYPE_CAMERA, native.cam_helmetCam);
			data.cam_draw_helmet = native.cam_drawHelmet;
			data.cam_camto_helmet_offset_origin = plain::read_scalars<float, 3>(&native.cam_camtoHelmetOffsetOrigin);
			data.cam_camto_helmet_offset_angles = plain::read_scalars<float, 3>(&native.cam_camtoHelmetOffsetAngles);
			data.enable_ik_override = native.enableIKOverride;
			data.player_global_accel_scale = native.player_globalAccelScale;
			data.player_crouch_speed_scale = native.player_crouchSpeedScale;
			data.player_prone_speed_scale = native.player_proneSpeedScale;
			data.player_last_stand_crawl_speed_scale = native.player_lastStandCrawlSpeedScale;
			data.player_sprint_speed_scale = native.player_sprintSpeedScale;
			data.player_sprint_unlimited = native.player_sprintUnlimited;
			data.player_view_bob_scale = native.player_viewBobScale;
			data.jump_slowdown_enable = native.jump_slowdownEnable;
			data.jump_height = native.jump_height;
			data.sprint_leap_height = native.sprintLeap_height;
			data.sprint_leap_forward_velocity_scale = native.sprintLeap_forwardVelocityScale;
			data.sprint_leap_min_sprint_time_ms = native.sprintLeap_minSprintTimeMs;
			data.double_jump_accel = native.doubleJump_accel;
			data.double_jump_speed = native.doubleJump_speed;
			data.double_jump_speed_no_boost = native.doubleJump_speedNoBoost;
			data.double_jump_friction_min = native.doubleJump_frictionMin;
			data.double_jump_friction_max = native.doubleJump_frictionMax;
			data.double_jump_initial_up_boost_accel = native.doubleJump_initialUpBoostAccel;
			data.double_jump_standard_up_boost_accel = native.doubleJump_standardUpBoostAccel;
			data.double_jump_energy_needed_for_initial_up_boost = native.doubleJump_energyNeededForInitialUpBoost;
			data.double_jump_energy_needed_for_standard_up_boost = native.doubleJump_energyNeededForStandardUpBoost;
			data.double_jump_max_upwards_velocity = native.doubleJump_maxUpwardsVelocity;
			data.double_jump_enable_min_z_velocity = native.doubleJump_enableMinZVelocity;
			data.double_jump_min_z_velocity = native.doubleJump_minZVelocity;
			data.double_jump_energy_initial_cost = native.doubleJump_energyInitialCost;
			data.double_jump_boost_up_energy_use_rate = native.doubleJump_boostUpEnergyUseRate;
			data.double_jump_energy_use_per_button_press = native.doubleJump_energyUsePerButtonPress;
			data.double_jump_hover_only = native.doubleJump_hoverOnly;
			data.double_jump_max_view_pitch_dip = native.doubleJump_maxViewPitchDip;
			data.double_jump_max_view_backwards_pitch_dip = native.doubleJump_maxViewBackwardsPitchDip;
			data.double_jump_max_view_roll = native.doubleJump_maxViewRoll;
			data.double_jump_one_over_ease_in_time = native.doubleJump_oneOverEaseInTime;
			data.double_jump_one_over_ease_out_time_ms = native.doubleJump_oneOverEaseOutTimeMs;
			data.double_jump_always_dip_view = native.doubleJump_alwaysDipView;
			data.double_jump_sound = read_snd_alias_lookup(native.doubleJump_sound);
			data.double_jump_sound_player = read_snd_alias_lookup(native.doubleJump_soundPlayer);
			data.double_jump_release_sound = read_snd_alias_lookup(native.doubleJump_releaseSound);
			data.double_jump_release_sound_player = read_snd_alias_lookup(native.doubleJump_releaseSoundPlayer);
			data.slide_strafe_speed_scale = native.slide_strafe_speed_scale;
			data.slide_sprint_penalty_ms = native.slide_sprint_penalty_ms;
			data.slide_allow_firing = native.slide_allow_firing;
			data.slide_allow_ads = native.slide_allow_ads;
			data.slide_allow_weapon_switch = native.slide_allow_weapon_switch;
			data.slide_jump_speed_scale = native.slide_jump_speed_scale;
			data.slide_energy_cost_ratio = native.slide_energy_cost_ratio;
			data.slide_max_time_ms = native.slide_max_time_ms;
			data.slide_max_time_reduced_ms = native.slide_max_time_reduced_ms;
			data.slide_max_time_base_ms = native.slide_max_time_base_ms;
			data.slide_in_time_ms = native.slide_inTimeMs;
			data.slide_in_max_speed_scale = native.slide_inMaxSpeedScale;
			data.slide_in_acceleration = native.slide_inAcceleration;
			data.slide_friction_scale_normal = native.slide_frictionScaleNormal;
			data.slide_friction_scale_downhill = native.slide_frictionScaleDownhill;
			data.slide_friction_scale_uphill = native.slide_frictionScaleUphill;
			data.slide_friction_scale_blocked = native.slide_frictionScaleBlocked;
			data.slide_gesture_out_time_for_jump_ms = native.slide_gestureOutTimeForJumpMs;
			data.slide_sprint_delay_ms = native.slide_sprintDelayMs;
			data.slide_disable_prone_transition = native.slide_disableProneTransition;
			data.slide_prone_delay_ms = native.slide_proneDelayMs;
			data.slide_prone_button_check_time_ms = native.slide_proneButtonCheckTimeMs;
			data.slide_out_time_ms = native.slide_outTimeMs;
			data.slide_out_friction_scale_start = native.slide_outFrictionScaleStart;
			data.slide_out_friction_scale_finish = native.slide_outFrictionScaleFinish;
			data.slide_out_speed_scale_start = native.slide_outSpeedScaleStart;
			data.slide_out_speed_scale_finish = native.slide_outSpeedScaleFinish;
			data.slide_in_air_time_ms = native.slide_inAirTimeMs;
			data.slide_in_air_friction_scale_start = native.slide_inAirFrictionScaleStart;
			data.slide_in_air_friction_scale_finish = native.slide_inAirFrictionScaleFinish;
			data.slide_view_blend_in_time_ms = native.slide_viewBlendInTimeMs;
			data.slide_view_blend_out_time_ms = native.slide_viewBlendOutTimeMs;
			data.wall_run_min_z_vel = native.wallRun_minZVel;
			data.wall_run_min_trigger_speed = native.wallRun_minTriggerSpeed;
			data.wall_run_min_maintain_speed = native.wallRun_minMaintainSpeed;
			data.wall_run_delay_period_ms = native.wallRun_delayPeriodMs;
			data.wall_run_min_jump_height = native.wallRun_minJumpHeight;
			data.wall_run_max_time_ms = native.wallRun_maxTimeMs;
			data.wall_run_fall_stage_time_ms = native.wallRun_fallStageTimeMs;
			data.wall_run_max_height = native.wallRun_maxHeight;
			data.wall_run_jump_height = native.wallRun_jumpHeight;
			data.wall_run_jump_velocity = native.wallRun_jumpVelocity;
			data.wall_run_friction_scale = native.wallRun_frictionScale;
			data.wall_run_speed_scale = native.wallRun_speedScale;
			data.wall_run_speed_scale_ads = native.wallRun_speedScaleADS;
			data.wall_run_energy_initial_cost = native.wallRun_energyInitialCost;
			data.wall_run_energy_change_per_second = native.wallRun_energyChangePerSecond;
			data.ladder_anchor_offset = native.ladder_anchorOffset;
			data.ladder_anchor_offset_wm = native.ladder_anchorOffsetWM;
			data.ladder_hand_distance = native.ladder_handDistance;
			data.suit_anim_type = plain::enum_name(asset_data::gameplay::suit_anim_type_names, static_cast<std::int64_t>(native.suitAnimType), "SuitDef.suitAnimType");
			data.is_movement_camera_independent = native.isMovementCameraIndependent;
			data.anim_package = plain::read_asset(ASSET_TYPE_SUITANIMPACKAGE, native.animPackage);
			data.anim_package_l = plain::read_asset(ASSET_TYPE_SUITANIMPACKAGE, native.animPackageL);
			data.anim_package_r = plain::read_asset(ASSET_TYPE_SUITANIMPACKAGE, native.animPackageR);
			data.anim_package_relaxed = plain::read_asset(ASSET_TYPE_SUITANIMPACKAGE, native.animPackageRelaxed);
			data.anim_package_safe = plain::read_asset(ASSET_TYPE_SUITANIMPACKAGE, native.animPackageSafe);
			data.asm_asset = plain::read_asset(ASSET_TYPE_ASM, native.asmAsset);
			data.animset_asset = plain::read_asset(ASSET_TYPE_ANIMSET, native.animsetAsset);
			data.facial_anim_type = native.facialAnimType;
			data.body_anim_type = plain::enum_name(asset_data::gameplay::suit_body_anim_type_names, static_cast<std::int64_t>(native.bodyAnimType), "SuitDef.bodyAnimType");
			data.scriptable_def = plain::read_asset(ASSET_TYPE_SCRIPTABLE, native.scriptableDef);
			data.viewheight_stand = native.viewheight_stand;
			data.viewheight_crouch = native.viewheight_crouch;
			data.viewheight_prone = native.viewheight_prone;
			data.viewheight_laststand = native.viewheight_laststand;
			data.viewheight_dead = native.viewheight_dead;
			data.viewheight_swim = native.viewheight_swim;
			data.viewheight_slide = native.viewheight_slide;
			data.viewheight_sprint = native.viewheight_sprint;
			data.bounds_radius = native.bounds_radius;
			data.bounds_height_stand = native.bounds_height_stand;
			data.bounds_height_crouch = native.bounds_height_crouch;
			data.bounds_height_prone = native.bounds_height_prone;
			data.bounds_stand = read_bounds(native.bounds_stand);
			data.bounds_crouch = read_bounds(native.bounds_crouch);
			data.bounds_prone = read_bounds(native.bounds_prone);
			data.radial_motion_blur_interp_time_in = native.radialMotionBlur_interpTimeIn;
			data.radial_motion_blur_interp_time_out = native.radialMotionBlur_interpTimeOut;
			data.radial_motion_blur_sprint_min_radius = native.radialMotionBlur_sprintMinRadius;
			data.radial_motion_blur_sprint_max_radius = native.radialMotionBlur_sprintMaxRadius;
			data.radial_motion_blur_sprint_min_strength = native.radialMotionBlur_sprintMinStrength;
			data.radial_motion_blur_sprint_max_strength = native.radialMotionBlur_sprintMaxStrength;
			data.radial_motion_blur_slide_min_radius = native.radialMotionBlur_slideMinRadius;
			data.radial_motion_blur_slide_max_radius = native.radialMotionBlur_slideMaxRadius;
			data.radial_motion_blur_slide_min_strength = native.radialMotionBlur_slideMinStrength;
			data.radial_motion_blur_slide_max_strength = native.radialMotionBlur_slideMaxStrength;
			data.radial_motion_blur_double_jump_min_radius = native.radialMotionBlur_doubleJumpMinRadius;
			data.radial_motion_blur_double_jump_max_radius = native.radialMotionBlur_doubleJumpMaxRadius;
			data.radial_motion_blur_double_jump_min_strength = native.radialMotionBlur_doubleJumpMinStrength;
			data.radial_motion_blur_double_jump_max_strength = native.radialMotionBlur_doubleJumpMaxStrength;
			data.radial_motion_blur_wall_run_min_radius = native.radialMotionBlur_wallRunMinRadius;
			data.radial_motion_blur_wall_run_max_radius = native.radialMotionBlur_wallRunMaxRadius;
			data.radial_motion_blur_wall_run_min_strength = native.radialMotionBlur_wallRunMinStrength;
			data.radial_motion_blur_wall_run_max_strength = native.radialMotionBlur_wallRunMaxStrength;
			data.radial_motion_blur_ground_pound_min_radius = native.radialMotionBlur_groundPoundMinRadius;
			data.radial_motion_blur_ground_pound_max_radius = native.radialMotionBlur_groundPoundMaxRadius;
			data.radial_motion_blur_ground_pound_min_strength = native.radialMotionBlur_groundPoundMinStrength;
			data.radial_motion_blur_ground_pound_max_strength = native.radialMotionBlur_groundPoundMaxStrength;
			data.radial_motion_blur_rewind_min_radius = native.radialMotionBlur_rewindMinRadius;
			data.radial_motion_blur_rewind_max_radius = native.radialMotionBlur_rewindMaxRadius;
			data.radial_motion_blur_rewind_min_strength = native.radialMotionBlur_rewindMinStrength;
			data.radial_motion_blur_rewind_max_strength = native.radialMotionBlur_rewindMaxStrength;
			data.radial_motion_blur_dodge_min_radius = native.radialMotionBlur_dodgeMinRadius;
			data.radial_motion_blur_dodge_max_radius = native.radialMotionBlur_dodgeMaxRadius;
			data.radial_motion_blur_dodge_min_strength = native.radialMotionBlur_dodgeMinStrength;
			data.radial_motion_blur_dodge_max_strength = native.radialMotionBlur_dodgeMaxStrength;
			data.radial_motion_blur_skydive_min_radius = native.radialMotionBlur_skydiveMinRadius;
			data.radial_motion_blur_skydive_max_radius = native.radialMotionBlur_skydiveMaxRadius;
			data.radial_motion_blur_skydive_min_strength = native.radialMotionBlur_skydiveMinStrength;
			data.radial_motion_blur_skydive_max_strength = native.radialMotionBlur_skydiveMaxStrength;
			data.ground_pound_activation_rumble = plain::read_asset(ASSET_TYPE_RUMBLE, native.groundPound_activationRumble);
			data.ground_pound_activation_sound = read_snd_alias_lookup(native.groundPound_activationSound);
			data.ground_pound_activation_sound_player = read_snd_alias_lookup(native.groundPound_activationSoundPlayer);
			data.ground_pound_landing_rumble = plain::read_asset(ASSET_TYPE_RUMBLE, native.groundPound_landingRumble);
			data.ground_pound_landing_sound = read_snd_alias_lookup(native.groundPound_landingSound);
			data.ground_pound_landing_sound_player = read_snd_alias_lookup(native.groundPound_landingSoundPlayer);
			data.landing_rumble_low_height = plain::read_asset(ASSET_TYPE_RUMBLE, native.landing_rumbleLowHeight);
			data.landing_rumble_medium_height = plain::read_asset(ASSET_TYPE_RUMBLE, native.landing_rumbleMediumHeight);
			data.landing_rumble_high_height = plain::read_asset(ASSET_TYPE_RUMBLE, native.landing_rumbleHighHeight);
			data.landing_rumble_extreme_height = plain::read_asset(ASSET_TYPE_RUMBLE, native.landing_rumbleExtremeHeight);
			data.landing_speed_scale = native.landing_speedScale;
			data.footstep_shake_broadcast_radius_inches = native.footstep_shakeBroadcastRadiusInches;
			data.footstep_shake_duration_ms = native.footstep_shakeDurationMs;
			data.footstep_shake_amplitude = native.footstep_shakeAmplitude;
			data.footstep_rumble = plain::read_asset(ASSET_TYPE_RUMBLE, native.footstep_rumble);
			data.footstepvfx_slide = plain::read_asset(ASSET_TYPE_FOOTSTEP_VFX, native.footstepvfx_slide);
			data.footstepvfx_creep_left = plain::read_asset(ASSET_TYPE_FOOTSTEP_VFX, native.footstepvfx_creep_left);
			data.footstepvfx_creep_right = plain::read_asset(ASSET_TYPE_FOOTSTEP_VFX, native.footstepvfx_creep_right);
			data.footstepvfx_walk_left = plain::read_asset(ASSET_TYPE_FOOTSTEP_VFX, native.footstepvfx_walk_left);
			data.footstepvfx_walk_right = plain::read_asset(ASSET_TYPE_FOOTSTEP_VFX, native.footstepvfx_walk_right);
			data.footstepvfx_run_left = plain::read_asset(ASSET_TYPE_FOOTSTEP_VFX, native.footstepvfx_run_left);
			data.footstepvfx_run_right = plain::read_asset(ASSET_TYPE_FOOTSTEP_VFX, native.footstepvfx_run_right);
			data.footstepvfx_sprint_left = plain::read_asset(ASSET_TYPE_FOOTSTEP_VFX, native.footstepvfx_sprint_left);
			data.footstepvfx_sprint_right = plain::read_asset(ASSET_TYPE_FOOTSTEP_VFX, native.footstepvfx_sprint_right);
			data.skydive_mass = native.skydive_mass;
			data.skydive_gravitatational_acceleration = native.skydive_gravitatationalAcceleration;
			data.skydive_parachute_view_model = plain::read_asset(ASSET_TYPE_XMODEL, native.skydive_parachuteViewModel);
			data.skydive_parachute_world_model = plain::read_asset(ASSET_TYPE_XMODEL, native.skydive_parachuteWorldModel);
			data.skydive_base_jump_minimum_height = native.skydive_baseJumpMinimumHeight;
			data.skydive_base_jump_clearance_radius = native.skydive_baseJumpClearanceRadius;
			data.skydive_base_jump_auto_deploy_height = native.skydive_baseJumpAutoDeployHeight;
			data.skydive_base_jump_force_freefall_height = native.skydive_baseJumpForceFreefallHeight;
			data.skydive_freefall_super_dive_camera_pitch = native.skydive_freefallSuperDiveCameraPitch;
			data.skydive_freefall_super_dive_stick_input = native.skydive_freefallSuperDiveStickInput;
			data.skydive_freefall_super_dive_fov_adjust_degrees = native.skydive_freefallSuperDiveFovAdjustDegrees;
			data.skydive_freefall_super_dive_fov_adjust_in_time = native.skydive_freefallSuperDiveFovAdjustInTime;
			data.skydive_freefall_super_dive_fov_adjust_out_time = native.skydive_freefallSuperDiveFovAdjustOutTime;
			data.skydive_freefall_cinematic_motion = plain::read_asset(ASSET_TYPE_CINEMATICMOTION, native.skydive_freefallCinematicMotion);
			data.skydive_freefall_throttle_track_speed = native.skydive_freefallThrottleTrackSpeed;
			data.skydive_freefall_anim_track_speed_throttle = native.skydive_freefallAnimTrackSpeedThrottle;
			data.skydive_freefall_anim_track_speed_strafe = native.skydive_freefallAnimTrackSpeedStrafe;
			data.skydive_freefall_anim_track_speed_yaw = native.skydive_freefallAnimTrackSpeedYaw;
			data.skydive_freefall_anim_track_speed_pitch = native.skydive_freefallAnimTrackSpeedPitch;
			data.skydive_freefall_anim_min_weight = native.skydive_freefallAnimMinWeight;
			data.skydive_freefall_anim_max_weight = native.skydive_freefallAnimMaxWeight;
			data.skydive_freefall_anim_min_play_rate = native.skydive_freefallAnimMinPlayRate;
			data.skydive_freefall_anim_max_play_rate = native.skydive_freefallAnimMaxPlayRate;
			data.skydive_freefall_anim_speed_for_min_weight = native.skydive_freefallAnimSpeedForMinWeight;
			data.skydive_freefall_anim_speed_for_max_weight = native.skydive_freefallAnimSpeedForMaxWeight;
			data.skydive_freefall_top_ground_speed = native.skydive_freefallTopGroundSpeed;
			data.skydive_freefall_min_pitch = native.skydive_freefallMinPitch;
			data.skydive_freefall_max_pitch = native.skydive_freefallMaxPitch;
			data.skydive_freefall_pitch_speed = native.skydive_freefallPitchSpeed;
			data.skydive_freefall_yaw_speed = native.skydive_freefallYawSpeed;
			data.skydive_freefall_cross_sectional_area = native.skydive_freefallCrossSectionalArea;
			data.skydive_freefall_look_horiz_force_min = native.skydive_freefallLookHorizForceMin;
			data.skydive_freefall_look_horiz_force_max = native.skydive_freefallLookHorizForceMax;
			data.skydive_freefall_look_horiz_force_graph = plain::read_asset(ASSET_TYPE_RUMBLE_GRAPH, native.skydive_freefallLookHorizForceGraph);
			data.skydive_freefall_stick_horiz_force_min = native.skydive_freefallStickHorizForceMin;
			data.skydive_freefall_stick_horiz_force_max = native.skydive_freefallStickHorizForceMax;
			data.skydive_freefall_stick_horiz_force_graph = plain::read_asset(ASSET_TYPE_RUMBLE_GRAPH, native.skydive_freefallStickHorizForceGraph);
			data.skydive_freefall_drag_coef_horiz = native.skydive_freefallDragCoefHoriz;
			data.skydive_freefall_look_horiz_drag_coeff = native.skydive_freefallLookHorizDragCoeff;
			data.skydive_freefall_look_horiz_drag_graph = plain::read_asset(ASSET_TYPE_RUMBLE_GRAPH, native.skydive_freefallLookHorizDragGraph);
			data.skydive_freefall_stick_horiz_drag_coeff = native.skydive_freefallStickHorizDragCoeff;
			data.skydive_freefall_stick_horiz_drag_graph = plain::read_asset(ASSET_TYPE_RUMBLE_GRAPH, native.skydive_freefallStickHorizDragGraph);
			data.skydive_freefall_drag_coef_vert = native.skydive_freefallDragCoefVert;
			data.skydive_freefall_look_vert_drag_coeff = native.skydive_freefallLookVertDragCoeff;
			data.skydive_freefall_look_vert_drag_graph = plain::read_asset(ASSET_TYPE_RUMBLE_GRAPH, native.skydive_freefallLookVertDragGraph);
			data.skydive_freefall_stick_vert_drag_increase_perc = native.skydive_freefallStickVertDragIncreasePerc;
			data.skydive_freefall_stick_vert_drag_increase_graph = plain::read_asset(ASSET_TYPE_RUMBLE_GRAPH, native.skydive_freefallStickVertDragIncreaseGraph);
			data.skydive_freefall_stick_vert_drag_reduction_perc = native.skydive_freefallStickVertDragReductionPerc;
			data.skydive_freefall_stick_vert_drag_reduction_graph = plain::read_asset(ASSET_TYPE_RUMBLE_GRAPH, native.skydive_freefallStickVertDragReductionGraph);
			data.skydive_freefall_stick_sideways_force = native.skydive_freefallStickSidewaysForce;
			data.skydive_freefall_max_brake_force = native.skydive_freefallMaxBrakeForce;
			data.skydive_freefall_min_turn_multiplier = native.skydive_freefallMinTurnMultiplier;
			data.skydive_freefall_max_turn_multiplier = native.skydive_freefallMaxTurnMultiplier;
			data.skydive_freefall_turn_multiplier_graph = plain::read_asset(ASSET_TYPE_RUMBLE_GRAPH, native.skydive_freefallTurnMultiplierGraph);
			data.skydive_freefall_turn_multiplier_min_scale = native.skydive_freefallTurnMultiplierMinScale;
			data.skydive_freefall_turn_multiplier_max_scale = native.skydive_freefallTurnMultiplierMaxScale;
			data.skydive_freefall_turn_multiplier_pitch_graph = plain::read_asset(ASSET_TYPE_RUMBLE_GRAPH, native.skydive_freefallTurnMultiplierPitchGraph);
			data.skydive_freefall_ambient_sound = read_snd_alias_lookup(native.skydive_freefallAmbientSound);
			data.skydive_freefall_ambient_sound_ground_speed_weight = native.skydive_freefallAmbientSoundGroundSpeedWeight;
			data.skydive_freefall_ambient_sound_min_volume = native.skydive_freefallAmbientSoundMinVolume;
			data.skydive_freefall_ambient_sound_max_volume = native.skydive_freefallAmbientSoundMaxVolume;
			data.skydive_freefall_ambient_sound_min_pitch = native.skydive_freefallAmbientSoundMinPitch;
			data.skydive_freefall_ambient_sound_max_pitch = native.skydive_freefallAmbientSoundMaxPitch;
			data.skydive_freefall_ambient_sound_min_ground_speed = native.skydive_freefallAmbientSoundMinGroundSpeed;
			data.skydive_freefall_ambient_sound_max_ground_speed = native.skydive_freefallAmbientSoundMaxGroundSpeed;
			data.skydive_freefall_ambient_sound_min_fall_speed = native.skydive_freefallAmbientSoundMinFallSpeed;
			data.skydive_freefall_ambient_sound_max_fall_speed = native.skydive_freefallAmbientSoundMaxFallSpeed;
			data.skydive_freefall_high_speed_sound = read_snd_alias_lookup(native.skydive_freefallHighSpeedSound);
			data.skydive_freefall_high_speed_sound_ground_speed_weight = native.skydive_freefallHighSpeedSoundGroundSpeedWeight;
			data.skydive_freefall_high_speed_sound_min_volume = native.skydive_freefallHighSpeedSoundMinVolume;
			data.skydive_freefall_high_speed_sound_max_volume = native.skydive_freefallHighSpeedSoundMaxVolume;
			data.skydive_freefall_high_speed_sound_min_pitch = native.skydive_freefallHighSpeedSoundMinPitch;
			data.skydive_freefall_high_speed_sound_max_pitch = native.skydive_freefallHighSpeedSoundMaxPitch;
			data.skydive_freefall_high_speed_sound_min_ground_speed = native.skydive_freefallHighSpeedSoundMinGroundSpeed;
			data.skydive_freefall_high_speed_sound_max_ground_speed = native.skydive_freefallHighSpeedSoundMaxGroundSpeed;
			data.skydive_freefall_high_speed_sound_min_fall_speed = native.skydive_freefallHighSpeedSoundMinFallSpeed;
			data.skydive_freefall_high_speed_sound_max_fall_speed = native.skydive_freefallHighSpeedSoundMaxFallSpeed;
			data.skydive_deploy_horizontal_drag = native.skydive_deployHorizontalDrag;
			data.skydive_deploy_vertical_drag = native.skydive_deployVerticalDrag;
			data.skydive_canopy_cinematic_motion = plain::read_asset(ASSET_TYPE_CINEMATICMOTION, native.skydive_canopyCinematicMotion);
			data.skydive_canopy_throttle_track_speed = native.skydive_canopyThrottleTrackSpeed;
			data.skydive_canopy_anim_track_speed_throttle = native.skydive_canopyAnimTrackSpeedThrottle;
			data.skydive_canopy_anim_track_speed_strafe = native.skydive_canopyAnimTrackSpeedStrafe;
			data.skydive_canopy_anim_track_speed_yaw = native.skydive_canopyAnimTrackSpeedYaw;
			data.skydive_canopy_anim_track_speed_pitch = native.skydive_canopyAnimTrackSpeedPitch;
			data.skydive_canopy_anim_min_weight = native.skydive_canopyAnimMinWeight;
			data.skydive_canopy_anim_max_weight = native.skydive_canopyAnimMaxWeight;
			data.skydive_canopy_anim_min_play_rate = native.skydive_canopyAnimMinPlayRate;
			data.skydive_canopy_anim_max_play_rate = native.skydive_canopyAnimMaxPlayRate;
			data.skydive_canopy_anim_speed_for_min_weight = native.skydive_canopyAnimSpeedForMinWeight;
			data.skydive_canopy_anim_speed_for_max_weight = native.skydive_canopyAnimSpeedForMaxWeight;
			data.skydive_canopy_top_ground_speed = native.skydive_canopyTopGroundSpeed;
			data.skydive_canopy_min_pitch = native.skydive_canopyMinPitch;
			data.skydive_canopy_max_pitch = native.skydive_canopyMaxPitch;
			data.skydive_canopy_pitch_speed = native.skydive_canopyPitchSpeed;
			data.skydive_canopy_yaw_speed = native.skydive_canopyYawSpeed;
			data.skydive_canopy_cross_sectional_area = native.skydive_canopyCrossSectionalArea;
			data.skydive_canopy_look_horiz_force_min = native.skydive_canopyLookHorizForceMin;
			data.skydive_canopy_look_horiz_force_max = native.skydive_canopyLookHorizForceMax;
			data.skydive_canopy_look_horiz_force_graph = plain::read_asset(ASSET_TYPE_RUMBLE_GRAPH, native.skydive_canopyLookHorizForceGraph);
			data.skydive_canopy_stick_horiz_force_min = native.skydive_canopyStickHorizForceMin;
			data.skydive_canopy_stick_horiz_force_max = native.skydive_canopyStickHorizForceMax;
			data.skydive_canopy_stick_horiz_force_graph = plain::read_asset(ASSET_TYPE_RUMBLE_GRAPH, native.skydive_canopyStickHorizForceGraph);
			data.skydive_canopy_drag_coef_horiz = native.skydive_canopyDragCoefHoriz;
			data.skydive_canopy_look_horiz_drag_coeff = native.skydive_canopyLookHorizDragCoeff;
			data.skydive_canopy_look_horiz_drag_graph = plain::read_asset(ASSET_TYPE_RUMBLE_GRAPH, native.skydive_canopyLookHorizDragGraph);
			data.skydive_canopy_stick_horiz_drag_coeff = native.skydive_canopyStickHorizDragCoeff;
			data.skydive_canopy_stick_horiz_drag_graph = plain::read_asset(ASSET_TYPE_RUMBLE_GRAPH, native.skydive_canopyStickHorizDragGraph);
			data.skydive_canopy_drag_coef_vert = native.skydive_canopyDragCoefVert;
			data.skydive_canopy_look_vert_drag_coeff = native.skydive_canopyLookVertDragCoeff;
			data.skydive_canopy_look_vert_drag_graph = plain::read_asset(ASSET_TYPE_RUMBLE_GRAPH, native.skydive_canopyLookVertDragGraph);
			data.skydive_canopy_stick_vert_drag_increase_perc = native.skydive_canopyStickVertDragIncreasePerc;
			data.skydive_canopy_stick_vert_drag_increase_graph = plain::read_asset(ASSET_TYPE_RUMBLE_GRAPH, native.skydive_canopyStickVertDragIncreaseGraph);
			data.skydive_canopy_stick_vert_drag_reduction_perc = native.skydive_canopyStickVertDragReductionPerc;
			data.skydive_canopy_stick_vert_drag_reduction_graph = plain::read_asset(ASSET_TYPE_RUMBLE_GRAPH, native.skydive_canopyStickVertDragReductionGraph);
			data.skydive_canopy_stick_sideways_force = native.skydive_canopyStickSidewaysForce;
			data.skydive_canopy_max_brake_force = native.skydive_canopyMaxBrakeForce;
			data.skydive_canopy_min_turn_multiplier = native.skydive_canopyMinTurnMultiplier;
			data.skydive_canopy_max_turn_multiplier = native.skydive_canopyMaxTurnMultiplier;
			data.skydive_canopy_turn_multiplier_graph = plain::read_asset(ASSET_TYPE_RUMBLE_GRAPH, native.skydive_canopyTurnMultiplierGraph);
			data.skydive_canopy_turn_multiplier_min_scale = native.skydive_canopyTurnMultiplierMinScale;
			data.skydive_canopy_turn_multiplier_max_scale = native.skydive_canopyTurnMultiplierMaxScale;
			data.skydive_canopy_turn_multiplier_pitch_graph = plain::read_asset(ASSET_TYPE_RUMBLE_GRAPH, native.skydive_canopyTurnMultiplierPitchGraph);
			data.skydive_canopy_ambient_sound = read_snd_alias_lookup(native.skydive_canopyAmbientSound);
			data.skydive_canopy_ambient_sound_ground_speed_weight = native.skydive_canopyAmbientSoundGroundSpeedWeight;
			data.skydive_canopy_ambient_sound_min_volume = native.skydive_canopyAmbientSoundMinVolume;
			data.skydive_canopy_ambient_sound_max_volume = native.skydive_canopyAmbientSoundMaxVolume;
			data.skydive_canopy_ambient_sound_min_pitch = native.skydive_canopyAmbientSoundMinPitch;
			data.skydive_canopy_ambient_sound_max_pitch = native.skydive_canopyAmbientSoundMaxPitch;
			data.skydive_canopy_ambient_sound_min_ground_speed = native.skydive_canopyAmbientSoundMinGroundSpeed;
			data.skydive_canopy_ambient_sound_max_ground_speed = native.skydive_canopyAmbientSoundMaxGroundSpeed;
			data.skydive_canopy_ambient_sound_min_fall_speed = native.skydive_canopyAmbientSoundMinFallSpeed;
			data.skydive_canopy_ambient_sound_max_fall_speed = native.skydive_canopyAmbientSoundMaxFallSpeed;
			data.skydive_canopy_high_speed_sound = read_snd_alias_lookup(native.skydive_canopyHighSpeedSound);
			data.skydive_canopy_high_speed_sound_ground_speed_weight = native.skydive_canopyHighSpeedSoundGroundSpeedWeight;
			data.skydive_canopy_high_speed_sound_min_volume = native.skydive_canopyHighSpeedSoundMinVolume;
			data.skydive_canopy_high_speed_sound_max_volume = native.skydive_canopyHighSpeedSoundMaxVolume;
			data.skydive_canopy_high_speed_sound_min_pitch = native.skydive_canopyHighSpeedSoundMinPitch;
			data.skydive_canopy_high_speed_sound_max_pitch = native.skydive_canopyHighSpeedSoundMaxPitch;
			data.skydive_canopy_high_speed_sound_min_ground_speed = native.skydive_canopyHighSpeedSoundMinGroundSpeed;
			data.skydive_canopy_high_speed_sound_max_ground_speed = native.skydive_canopyHighSpeedSoundMaxGroundSpeed;
			data.skydive_canopy_high_speed_sound_min_fall_speed = native.skydive_canopyHighSpeedSoundMinFallSpeed;
			data.skydive_canopy_high_speed_sound_max_fall_speed = native.skydive_canopyHighSpeedSoundMaxFallSpeed;
			data.skydive_wm_anim_freefall_pitch_min = native.skydive_wmAnimFreefallPitchMin;
			data.skydive_wm_anim_freefall_pitch_max = native.skydive_wmAnimFreefallPitchMax;
			data.skydive_wm_anim_freefall_pitch_track_speed = native.skydive_wmAnimFreefallPitchTrackSpeed;
			data.skydive_wm_anim_freefall_roll_track_speed = native.skydive_wmAnimFreefallRollTrackSpeed;
			data.skydive_wm_anim_freefall_roll_return_speed = native.skydive_wmAnimFreefallRollReturnSpeed;
			data.skydive_wm_anim_freefall_left_stick_x_to_roll_graph = plain::read_asset(ASSET_TYPE_RUMBLE_GRAPH, native.skydive_wmAnimFreefallLeftStickXToRollGraph);
			data.skydive_wm_anim_freefall_right_stick_x_to_roll_graph = plain::read_asset(ASSET_TYPE_RUMBLE_GRAPH, native.skydive_wmAnimFreefallRightStickXToRollGraph);
			data.skydive_cam_freefall_height_offset = native.skydive_camFreefallHeightOffset;
			data.skydive_cam_freefall_pitch_to_distance_graph = plain::read_asset(ASSET_TYPE_RUMBLE_GRAPH, native.skydive_camFreefallPitchToDistanceGraph);
			data.skydive_cam_freefall_pitch_to_forward_offset_graph = plain::read_asset(ASSET_TYPE_RUMBLE_GRAPH, native.skydive_camFreefallPitchToForwardOffsetGraph);
			data.skydive_cam_parachute_height_offset = native.skydive_camParachuteHeightOffset;
			data.skydive_cam_parachute_pitch_to_distance_graph = plain::read_asset(ASSET_TYPE_RUMBLE_GRAPH, native.skydive_camParachutePitchToDistanceGraph);
			data.skydive_cam_parachute_pitch_to_forward_offset_graph = plain::read_asset(ASSET_TYPE_RUMBLE_GRAPH, native.skydive_camParachutePitchToForwardOffsetGraph);
			data.skydive_cam_parachute_pitch_to_model_pitch_forward_offset_graph = plain::read_asset(ASSET_TYPE_RUMBLE_GRAPH, native.skydive_camParachutePitchToModelPitchForwardOffsetGraph);
			data.skydive_cam_parachute_pitch_to_model_roll_side_offset_graph = plain::read_asset(ASSET_TYPE_RUMBLE_GRAPH, native.skydive_camParachutePitchToModelRollSideOffsetGraph);
			data.skydive_cam_parachute_pitch_to_model_roll_up_offset_graph = plain::read_asset(ASSET_TYPE_RUMBLE_GRAPH, native.skydive_camParachutePitchToModelRollUpOffsetGraph);
			data.skydive_cam_parachute_blend_in_time = native.skydive_camParachuteBlendInTime;
			data.nvg_blind_sound = read_snd_alias_lookup(native.nvg_blindSound);
			data.dlc_float = std::to_array(native.dlcFloat);
			return data;
		}

		void write_suit(SuitDef& native, const asset_data::gameplay::suit& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.cam_minVelocityForFovIncrease = data.cam_min_velocity_for_fov_increase;
			native.cam_maxVelocityForFovIncrease = data.cam_max_velocity_for_fov_increase;
			native.cam_velocityToDecreaseFov = data.cam_velocity_to_decrease_fov;
			native.cam_fovIncreaseAtMaxVelocity = data.cam_fov_increase_at_max_velocity;
			native.cam_oneOverFovEaseInTimeMs = data.cam_one_over_fov_ease_in_time_ms;
			native.cam_oneOverFovEaseOutTimeMs = data.cam_one_over_fov_ease_out_time_ms;
			native.cam_helmetCam = plain::resolve_asset<CameraDef>(resolve, ASSET_TYPE_CAMERA, data.cam_helmet_cam);
			native.cam_drawHelmet = data.cam_draw_helmet;
			plain::write_scalars(&native.cam_camtoHelmetOffsetOrigin, data.cam_camto_helmet_offset_origin);
			plain::write_scalars(&native.cam_camtoHelmetOffsetAngles, data.cam_camto_helmet_offset_angles);
			native.enableIKOverride = data.enable_ik_override;
			native.player_globalAccelScale = data.player_global_accel_scale;
			native.player_crouchSpeedScale = data.player_crouch_speed_scale;
			native.player_proneSpeedScale = data.player_prone_speed_scale;
			native.player_lastStandCrawlSpeedScale = data.player_last_stand_crawl_speed_scale;
			native.player_sprintSpeedScale = data.player_sprint_speed_scale;
			native.player_sprintUnlimited = data.player_sprint_unlimited;
			native.player_viewBobScale = data.player_view_bob_scale;
			native.jump_slowdownEnable = data.jump_slowdown_enable;
			native.jump_height = data.jump_height;
			native.sprintLeap_height = data.sprint_leap_height;
			native.sprintLeap_forwardVelocityScale = data.sprint_leap_forward_velocity_scale;
			native.sprintLeap_minSprintTimeMs = data.sprint_leap_min_sprint_time_ms;
			native.doubleJump_accel = data.double_jump_accel;
			native.doubleJump_speed = data.double_jump_speed;
			native.doubleJump_speedNoBoost = data.double_jump_speed_no_boost;
			native.doubleJump_frictionMin = data.double_jump_friction_min;
			native.doubleJump_frictionMax = data.double_jump_friction_max;
			native.doubleJump_initialUpBoostAccel = data.double_jump_initial_up_boost_accel;
			native.doubleJump_standardUpBoostAccel = data.double_jump_standard_up_boost_accel;
			native.doubleJump_energyNeededForInitialUpBoost = data.double_jump_energy_needed_for_initial_up_boost;
			native.doubleJump_energyNeededForStandardUpBoost = data.double_jump_energy_needed_for_standard_up_boost;
			native.doubleJump_maxUpwardsVelocity = data.double_jump_max_upwards_velocity;
			native.doubleJump_enableMinZVelocity = data.double_jump_enable_min_z_velocity;
			native.doubleJump_minZVelocity = data.double_jump_min_z_velocity;
			native.doubleJump_energyInitialCost = data.double_jump_energy_initial_cost;
			native.doubleJump_boostUpEnergyUseRate = data.double_jump_boost_up_energy_use_rate;
			native.doubleJump_energyUsePerButtonPress = data.double_jump_energy_use_per_button_press;
			native.doubleJump_hoverOnly = data.double_jump_hover_only;
			native.doubleJump_maxViewPitchDip = data.double_jump_max_view_pitch_dip;
			native.doubleJump_maxViewBackwardsPitchDip = data.double_jump_max_view_backwards_pitch_dip;
			native.doubleJump_maxViewRoll = data.double_jump_max_view_roll;
			native.doubleJump_oneOverEaseInTime = data.double_jump_one_over_ease_in_time;
			native.doubleJump_oneOverEaseOutTimeMs = data.double_jump_one_over_ease_out_time_ms;
			native.doubleJump_alwaysDipView = data.double_jump_always_dip_view;
			write_snd_alias_lookup(native.doubleJump_sound, data.double_jump_sound, resolve, memory);
			write_snd_alias_lookup(native.doubleJump_soundPlayer, data.double_jump_sound_player, resolve, memory);
			write_snd_alias_lookup(native.doubleJump_releaseSound, data.double_jump_release_sound, resolve, memory);
			write_snd_alias_lookup(native.doubleJump_releaseSoundPlayer, data.double_jump_release_sound_player, resolve, memory);
			native.slide_strafe_speed_scale = data.slide_strafe_speed_scale;
			native.slide_sprint_penalty_ms = data.slide_sprint_penalty_ms;
			native.slide_allow_firing = data.slide_allow_firing;
			native.slide_allow_ads = data.slide_allow_ads;
			native.slide_allow_weapon_switch = data.slide_allow_weapon_switch;
			native.slide_jump_speed_scale = data.slide_jump_speed_scale;
			native.slide_energy_cost_ratio = data.slide_energy_cost_ratio;
			native.slide_max_time_ms = data.slide_max_time_ms;
			native.slide_max_time_reduced_ms = data.slide_max_time_reduced_ms;
			native.slide_max_time_base_ms = data.slide_max_time_base_ms;
			native.slide_inTimeMs = data.slide_in_time_ms;
			native.slide_inMaxSpeedScale = data.slide_in_max_speed_scale;
			native.slide_inAcceleration = data.slide_in_acceleration;
			native.slide_frictionScaleNormal = data.slide_friction_scale_normal;
			native.slide_frictionScaleDownhill = data.slide_friction_scale_downhill;
			native.slide_frictionScaleUphill = data.slide_friction_scale_uphill;
			native.slide_frictionScaleBlocked = data.slide_friction_scale_blocked;
			native.slide_gestureOutTimeForJumpMs = data.slide_gesture_out_time_for_jump_ms;
			native.slide_sprintDelayMs = data.slide_sprint_delay_ms;
			native.slide_disableProneTransition = data.slide_disable_prone_transition;
			native.slide_proneDelayMs = data.slide_prone_delay_ms;
			native.slide_proneButtonCheckTimeMs = data.slide_prone_button_check_time_ms;
			native.slide_outTimeMs = data.slide_out_time_ms;
			native.slide_outFrictionScaleStart = data.slide_out_friction_scale_start;
			native.slide_outFrictionScaleFinish = data.slide_out_friction_scale_finish;
			native.slide_outSpeedScaleStart = data.slide_out_speed_scale_start;
			native.slide_outSpeedScaleFinish = data.slide_out_speed_scale_finish;
			native.slide_inAirTimeMs = data.slide_in_air_time_ms;
			native.slide_inAirFrictionScaleStart = data.slide_in_air_friction_scale_start;
			native.slide_inAirFrictionScaleFinish = data.slide_in_air_friction_scale_finish;
			native.slide_viewBlendInTimeMs = data.slide_view_blend_in_time_ms;
			native.slide_viewBlendOutTimeMs = data.slide_view_blend_out_time_ms;
			native.wallRun_minZVel = data.wall_run_min_z_vel;
			native.wallRun_minTriggerSpeed = data.wall_run_min_trigger_speed;
			native.wallRun_minMaintainSpeed = data.wall_run_min_maintain_speed;
			native.wallRun_delayPeriodMs = data.wall_run_delay_period_ms;
			native.wallRun_minJumpHeight = data.wall_run_min_jump_height;
			native.wallRun_maxTimeMs = data.wall_run_max_time_ms;
			native.wallRun_fallStageTimeMs = data.wall_run_fall_stage_time_ms;
			native.wallRun_maxHeight = data.wall_run_max_height;
			native.wallRun_jumpHeight = data.wall_run_jump_height;
			native.wallRun_jumpVelocity = data.wall_run_jump_velocity;
			native.wallRun_frictionScale = data.wall_run_friction_scale;
			native.wallRun_speedScale = data.wall_run_speed_scale;
			native.wallRun_speedScaleADS = data.wall_run_speed_scale_ads;
			native.wallRun_energyInitialCost = data.wall_run_energy_initial_cost;
			native.wallRun_energyChangePerSecond = data.wall_run_energy_change_per_second;
			native.ladder_anchorOffset = data.ladder_anchor_offset;
			native.ladder_anchorOffsetWM = data.ladder_anchor_offset_wm;
			native.ladder_handDistance = data.ladder_hand_distance;
			native.suitAnimType = static_cast<SuitAnimType>(plain::enum_value(asset_data::gameplay::suit_anim_type_names, data.suit_anim_type, "SuitDef.suitAnimType"));
			native.isMovementCameraIndependent = data.is_movement_camera_independent;
			native.animPackage = plain::resolve_asset<SuitAnimPackage>(resolve, ASSET_TYPE_SUITANIMPACKAGE, data.anim_package);
			native.animPackageL = plain::resolve_asset<SuitAnimPackage>(resolve, ASSET_TYPE_SUITANIMPACKAGE, data.anim_package_l);
			native.animPackageR = plain::resolve_asset<SuitAnimPackage>(resolve, ASSET_TYPE_SUITANIMPACKAGE, data.anim_package_r);
			native.animPackageRelaxed = plain::resolve_asset<SuitAnimPackage>(resolve, ASSET_TYPE_SUITANIMPACKAGE, data.anim_package_relaxed);
			native.animPackageSafe = plain::resolve_asset<SuitAnimPackage>(resolve, ASSET_TYPE_SUITANIMPACKAGE, data.anim_package_safe);
			native.asmAsset = plain::resolve_asset<ASM>(resolve, ASSET_TYPE_ASM, data.asm_asset);
			native.animsetAsset = plain::resolve_asset<Animset>(resolve, ASSET_TYPE_ANIMSET, data.animset_asset);
			native.facialAnimType = data.facial_anim_type;
			native.bodyAnimType = static_cast<SuitBodyAnimType>(plain::enum_value(asset_data::gameplay::suit_body_anim_type_names, data.body_anim_type, "SuitDef.bodyAnimType"));
			native.scriptableDef = plain::resolve_asset<ScriptableDef>(resolve, ASSET_TYPE_SCRIPTABLE, data.scriptable_def);
			native.viewheight_stand = data.viewheight_stand;
			native.viewheight_crouch = data.viewheight_crouch;
			native.viewheight_prone = data.viewheight_prone;
			native.viewheight_laststand = data.viewheight_laststand;
			native.viewheight_dead = data.viewheight_dead;
			native.viewheight_swim = data.viewheight_swim;
			native.viewheight_slide = data.viewheight_slide;
			native.viewheight_sprint = data.viewheight_sprint;
			native.bounds_radius = data.bounds_radius;
			native.bounds_height_stand = data.bounds_height_stand;
			native.bounds_height_crouch = data.bounds_height_crouch;
			native.bounds_height_prone = data.bounds_height_prone;
			write_bounds(native.bounds_stand, data.bounds_stand, resolve, memory);
			write_bounds(native.bounds_crouch, data.bounds_crouch, resolve, memory);
			write_bounds(native.bounds_prone, data.bounds_prone, resolve, memory);
			native.radialMotionBlur_interpTimeIn = data.radial_motion_blur_interp_time_in;
			native.radialMotionBlur_interpTimeOut = data.radial_motion_blur_interp_time_out;
			native.radialMotionBlur_sprintMinRadius = data.radial_motion_blur_sprint_min_radius;
			native.radialMotionBlur_sprintMaxRadius = data.radial_motion_blur_sprint_max_radius;
			native.radialMotionBlur_sprintMinStrength = data.radial_motion_blur_sprint_min_strength;
			native.radialMotionBlur_sprintMaxStrength = data.radial_motion_blur_sprint_max_strength;
			native.radialMotionBlur_slideMinRadius = data.radial_motion_blur_slide_min_radius;
			native.radialMotionBlur_slideMaxRadius = data.radial_motion_blur_slide_max_radius;
			native.radialMotionBlur_slideMinStrength = data.radial_motion_blur_slide_min_strength;
			native.radialMotionBlur_slideMaxStrength = data.radial_motion_blur_slide_max_strength;
			native.radialMotionBlur_doubleJumpMinRadius = data.radial_motion_blur_double_jump_min_radius;
			native.radialMotionBlur_doubleJumpMaxRadius = data.radial_motion_blur_double_jump_max_radius;
			native.radialMotionBlur_doubleJumpMinStrength = data.radial_motion_blur_double_jump_min_strength;
			native.radialMotionBlur_doubleJumpMaxStrength = data.radial_motion_blur_double_jump_max_strength;
			native.radialMotionBlur_wallRunMinRadius = data.radial_motion_blur_wall_run_min_radius;
			native.radialMotionBlur_wallRunMaxRadius = data.radial_motion_blur_wall_run_max_radius;
			native.radialMotionBlur_wallRunMinStrength = data.radial_motion_blur_wall_run_min_strength;
			native.radialMotionBlur_wallRunMaxStrength = data.radial_motion_blur_wall_run_max_strength;
			native.radialMotionBlur_groundPoundMinRadius = data.radial_motion_blur_ground_pound_min_radius;
			native.radialMotionBlur_groundPoundMaxRadius = data.radial_motion_blur_ground_pound_max_radius;
			native.radialMotionBlur_groundPoundMinStrength = data.radial_motion_blur_ground_pound_min_strength;
			native.radialMotionBlur_groundPoundMaxStrength = data.radial_motion_blur_ground_pound_max_strength;
			native.radialMotionBlur_rewindMinRadius = data.radial_motion_blur_rewind_min_radius;
			native.radialMotionBlur_rewindMaxRadius = data.radial_motion_blur_rewind_max_radius;
			native.radialMotionBlur_rewindMinStrength = data.radial_motion_blur_rewind_min_strength;
			native.radialMotionBlur_rewindMaxStrength = data.radial_motion_blur_rewind_max_strength;
			native.radialMotionBlur_dodgeMinRadius = data.radial_motion_blur_dodge_min_radius;
			native.radialMotionBlur_dodgeMaxRadius = data.radial_motion_blur_dodge_max_radius;
			native.radialMotionBlur_dodgeMinStrength = data.radial_motion_blur_dodge_min_strength;
			native.radialMotionBlur_dodgeMaxStrength = data.radial_motion_blur_dodge_max_strength;
			native.radialMotionBlur_skydiveMinRadius = data.radial_motion_blur_skydive_min_radius;
			native.radialMotionBlur_skydiveMaxRadius = data.radial_motion_blur_skydive_max_radius;
			native.radialMotionBlur_skydiveMinStrength = data.radial_motion_blur_skydive_min_strength;
			native.radialMotionBlur_skydiveMaxStrength = data.radial_motion_blur_skydive_max_strength;
			native.groundPound_activationRumble = plain::resolve_asset<RumbleInfo>(resolve, ASSET_TYPE_RUMBLE, data.ground_pound_activation_rumble);
			write_snd_alias_lookup(native.groundPound_activationSound, data.ground_pound_activation_sound, resolve, memory);
			write_snd_alias_lookup(native.groundPound_activationSoundPlayer, data.ground_pound_activation_sound_player, resolve, memory);
			native.groundPound_landingRumble = plain::resolve_asset<RumbleInfo>(resolve, ASSET_TYPE_RUMBLE, data.ground_pound_landing_rumble);
			write_snd_alias_lookup(native.groundPound_landingSound, data.ground_pound_landing_sound, resolve, memory);
			write_snd_alias_lookup(native.groundPound_landingSoundPlayer, data.ground_pound_landing_sound_player, resolve, memory);
			native.landing_rumbleLowHeight = plain::resolve_asset<RumbleInfo>(resolve, ASSET_TYPE_RUMBLE, data.landing_rumble_low_height);
			native.landing_rumbleMediumHeight = plain::resolve_asset<RumbleInfo>(resolve, ASSET_TYPE_RUMBLE, data.landing_rumble_medium_height);
			native.landing_rumbleHighHeight = plain::resolve_asset<RumbleInfo>(resolve, ASSET_TYPE_RUMBLE, data.landing_rumble_high_height);
			native.landing_rumbleExtremeHeight = plain::resolve_asset<RumbleInfo>(resolve, ASSET_TYPE_RUMBLE, data.landing_rumble_extreme_height);
			native.landing_speedScale = data.landing_speed_scale;
			native.footstep_shakeBroadcastRadiusInches = data.footstep_shake_broadcast_radius_inches;
			native.footstep_shakeDurationMs = data.footstep_shake_duration_ms;
			native.footstep_shakeAmplitude = data.footstep_shake_amplitude;
			native.footstep_rumble = plain::resolve_asset<RumbleInfo>(resolve, ASSET_TYPE_RUMBLE, data.footstep_rumble);
			native.footstepvfx_slide = plain::resolve_asset<FootstepVFX>(resolve, ASSET_TYPE_FOOTSTEP_VFX, data.footstepvfx_slide);
			native.footstepvfx_creep_left = plain::resolve_asset<FootstepVFX>(resolve, ASSET_TYPE_FOOTSTEP_VFX, data.footstepvfx_creep_left);
			native.footstepvfx_creep_right = plain::resolve_asset<FootstepVFX>(resolve, ASSET_TYPE_FOOTSTEP_VFX, data.footstepvfx_creep_right);
			native.footstepvfx_walk_left = plain::resolve_asset<FootstepVFX>(resolve, ASSET_TYPE_FOOTSTEP_VFX, data.footstepvfx_walk_left);
			native.footstepvfx_walk_right = plain::resolve_asset<FootstepVFX>(resolve, ASSET_TYPE_FOOTSTEP_VFX, data.footstepvfx_walk_right);
			native.footstepvfx_run_left = plain::resolve_asset<FootstepVFX>(resolve, ASSET_TYPE_FOOTSTEP_VFX, data.footstepvfx_run_left);
			native.footstepvfx_run_right = plain::resolve_asset<FootstepVFX>(resolve, ASSET_TYPE_FOOTSTEP_VFX, data.footstepvfx_run_right);
			native.footstepvfx_sprint_left = plain::resolve_asset<FootstepVFX>(resolve, ASSET_TYPE_FOOTSTEP_VFX, data.footstepvfx_sprint_left);
			native.footstepvfx_sprint_right = plain::resolve_asset<FootstepVFX>(resolve, ASSET_TYPE_FOOTSTEP_VFX, data.footstepvfx_sprint_right);
			native.skydive_mass = data.skydive_mass;
			native.skydive_gravitatationalAcceleration = data.skydive_gravitatational_acceleration;
			native.skydive_parachuteViewModel = plain::resolve_asset<XModel>(resolve, ASSET_TYPE_XMODEL, data.skydive_parachute_view_model);
			native.skydive_parachuteWorldModel = plain::resolve_asset<XModel>(resolve, ASSET_TYPE_XMODEL, data.skydive_parachute_world_model);
			native.skydive_baseJumpMinimumHeight = data.skydive_base_jump_minimum_height;
			native.skydive_baseJumpClearanceRadius = data.skydive_base_jump_clearance_radius;
			native.skydive_baseJumpAutoDeployHeight = data.skydive_base_jump_auto_deploy_height;
			native.skydive_baseJumpForceFreefallHeight = data.skydive_base_jump_force_freefall_height;
			native.skydive_freefallSuperDiveCameraPitch = data.skydive_freefall_super_dive_camera_pitch;
			native.skydive_freefallSuperDiveStickInput = data.skydive_freefall_super_dive_stick_input;
			native.skydive_freefallSuperDiveFovAdjustDegrees = data.skydive_freefall_super_dive_fov_adjust_degrees;
			native.skydive_freefallSuperDiveFovAdjustInTime = data.skydive_freefall_super_dive_fov_adjust_in_time;
			native.skydive_freefallSuperDiveFovAdjustOutTime = data.skydive_freefall_super_dive_fov_adjust_out_time;
			native.skydive_freefallCinematicMotion = plain::resolve_asset<CinematicMotionDef>(resolve, ASSET_TYPE_CINEMATICMOTION, data.skydive_freefall_cinematic_motion);
			native.skydive_freefallThrottleTrackSpeed = data.skydive_freefall_throttle_track_speed;
			native.skydive_freefallAnimTrackSpeedThrottle = data.skydive_freefall_anim_track_speed_throttle;
			native.skydive_freefallAnimTrackSpeedStrafe = data.skydive_freefall_anim_track_speed_strafe;
			native.skydive_freefallAnimTrackSpeedYaw = data.skydive_freefall_anim_track_speed_yaw;
			native.skydive_freefallAnimTrackSpeedPitch = data.skydive_freefall_anim_track_speed_pitch;
			native.skydive_freefallAnimMinWeight = data.skydive_freefall_anim_min_weight;
			native.skydive_freefallAnimMaxWeight = data.skydive_freefall_anim_max_weight;
			native.skydive_freefallAnimMinPlayRate = data.skydive_freefall_anim_min_play_rate;
			native.skydive_freefallAnimMaxPlayRate = data.skydive_freefall_anim_max_play_rate;
			native.skydive_freefallAnimSpeedForMinWeight = data.skydive_freefall_anim_speed_for_min_weight;
			native.skydive_freefallAnimSpeedForMaxWeight = data.skydive_freefall_anim_speed_for_max_weight;
			native.skydive_freefallTopGroundSpeed = data.skydive_freefall_top_ground_speed;
			native.skydive_freefallMinPitch = data.skydive_freefall_min_pitch;
			native.skydive_freefallMaxPitch = data.skydive_freefall_max_pitch;
			native.skydive_freefallPitchSpeed = data.skydive_freefall_pitch_speed;
			native.skydive_freefallYawSpeed = data.skydive_freefall_yaw_speed;
			native.skydive_freefallCrossSectionalArea = data.skydive_freefall_cross_sectional_area;
			native.skydive_freefallLookHorizForceMin = data.skydive_freefall_look_horiz_force_min;
			native.skydive_freefallLookHorizForceMax = data.skydive_freefall_look_horiz_force_max;
			native.skydive_freefallLookHorizForceGraph = plain::resolve_asset<RumbleGraph>(resolve, ASSET_TYPE_RUMBLE_GRAPH, data.skydive_freefall_look_horiz_force_graph);
			native.skydive_freefallStickHorizForceMin = data.skydive_freefall_stick_horiz_force_min;
			native.skydive_freefallStickHorizForceMax = data.skydive_freefall_stick_horiz_force_max;
			native.skydive_freefallStickHorizForceGraph = plain::resolve_asset<RumbleGraph>(resolve, ASSET_TYPE_RUMBLE_GRAPH, data.skydive_freefall_stick_horiz_force_graph);
			native.skydive_freefallDragCoefHoriz = data.skydive_freefall_drag_coef_horiz;
			native.skydive_freefallLookHorizDragCoeff = data.skydive_freefall_look_horiz_drag_coeff;
			native.skydive_freefallLookHorizDragGraph = plain::resolve_asset<RumbleGraph>(resolve, ASSET_TYPE_RUMBLE_GRAPH, data.skydive_freefall_look_horiz_drag_graph);
			native.skydive_freefallStickHorizDragCoeff = data.skydive_freefall_stick_horiz_drag_coeff;
			native.skydive_freefallStickHorizDragGraph = plain::resolve_asset<RumbleGraph>(resolve, ASSET_TYPE_RUMBLE_GRAPH, data.skydive_freefall_stick_horiz_drag_graph);
			native.skydive_freefallDragCoefVert = data.skydive_freefall_drag_coef_vert;
			native.skydive_freefallLookVertDragCoeff = data.skydive_freefall_look_vert_drag_coeff;
			native.skydive_freefallLookVertDragGraph = plain::resolve_asset<RumbleGraph>(resolve, ASSET_TYPE_RUMBLE_GRAPH, data.skydive_freefall_look_vert_drag_graph);
			native.skydive_freefallStickVertDragIncreasePerc = data.skydive_freefall_stick_vert_drag_increase_perc;
			native.skydive_freefallStickVertDragIncreaseGraph = plain::resolve_asset<RumbleGraph>(resolve, ASSET_TYPE_RUMBLE_GRAPH, data.skydive_freefall_stick_vert_drag_increase_graph);
			native.skydive_freefallStickVertDragReductionPerc = data.skydive_freefall_stick_vert_drag_reduction_perc;
			native.skydive_freefallStickVertDragReductionGraph = plain::resolve_asset<RumbleGraph>(resolve, ASSET_TYPE_RUMBLE_GRAPH, data.skydive_freefall_stick_vert_drag_reduction_graph);
			native.skydive_freefallStickSidewaysForce = data.skydive_freefall_stick_sideways_force;
			native.skydive_freefallMaxBrakeForce = data.skydive_freefall_max_brake_force;
			native.skydive_freefallMinTurnMultiplier = data.skydive_freefall_min_turn_multiplier;
			native.skydive_freefallMaxTurnMultiplier = data.skydive_freefall_max_turn_multiplier;
			native.skydive_freefallTurnMultiplierGraph = plain::resolve_asset<RumbleGraph>(resolve, ASSET_TYPE_RUMBLE_GRAPH, data.skydive_freefall_turn_multiplier_graph);
			native.skydive_freefallTurnMultiplierMinScale = data.skydive_freefall_turn_multiplier_min_scale;
			native.skydive_freefallTurnMultiplierMaxScale = data.skydive_freefall_turn_multiplier_max_scale;
			native.skydive_freefallTurnMultiplierPitchGraph = plain::resolve_asset<RumbleGraph>(resolve, ASSET_TYPE_RUMBLE_GRAPH, data.skydive_freefall_turn_multiplier_pitch_graph);
			write_snd_alias_lookup(native.skydive_freefallAmbientSound, data.skydive_freefall_ambient_sound, resolve, memory);
			native.skydive_freefallAmbientSoundGroundSpeedWeight = data.skydive_freefall_ambient_sound_ground_speed_weight;
			native.skydive_freefallAmbientSoundMinVolume = data.skydive_freefall_ambient_sound_min_volume;
			native.skydive_freefallAmbientSoundMaxVolume = data.skydive_freefall_ambient_sound_max_volume;
			native.skydive_freefallAmbientSoundMinPitch = data.skydive_freefall_ambient_sound_min_pitch;
			native.skydive_freefallAmbientSoundMaxPitch = data.skydive_freefall_ambient_sound_max_pitch;
			native.skydive_freefallAmbientSoundMinGroundSpeed = data.skydive_freefall_ambient_sound_min_ground_speed;
			native.skydive_freefallAmbientSoundMaxGroundSpeed = data.skydive_freefall_ambient_sound_max_ground_speed;
			native.skydive_freefallAmbientSoundMinFallSpeed = data.skydive_freefall_ambient_sound_min_fall_speed;
			native.skydive_freefallAmbientSoundMaxFallSpeed = data.skydive_freefall_ambient_sound_max_fall_speed;
			write_snd_alias_lookup(native.skydive_freefallHighSpeedSound, data.skydive_freefall_high_speed_sound, resolve, memory);
			native.skydive_freefallHighSpeedSoundGroundSpeedWeight = data.skydive_freefall_high_speed_sound_ground_speed_weight;
			native.skydive_freefallHighSpeedSoundMinVolume = data.skydive_freefall_high_speed_sound_min_volume;
			native.skydive_freefallHighSpeedSoundMaxVolume = data.skydive_freefall_high_speed_sound_max_volume;
			native.skydive_freefallHighSpeedSoundMinPitch = data.skydive_freefall_high_speed_sound_min_pitch;
			native.skydive_freefallHighSpeedSoundMaxPitch = data.skydive_freefall_high_speed_sound_max_pitch;
			native.skydive_freefallHighSpeedSoundMinGroundSpeed = data.skydive_freefall_high_speed_sound_min_ground_speed;
			native.skydive_freefallHighSpeedSoundMaxGroundSpeed = data.skydive_freefall_high_speed_sound_max_ground_speed;
			native.skydive_freefallHighSpeedSoundMinFallSpeed = data.skydive_freefall_high_speed_sound_min_fall_speed;
			native.skydive_freefallHighSpeedSoundMaxFallSpeed = data.skydive_freefall_high_speed_sound_max_fall_speed;
			native.skydive_deployHorizontalDrag = data.skydive_deploy_horizontal_drag;
			native.skydive_deployVerticalDrag = data.skydive_deploy_vertical_drag;
			native.skydive_canopyCinematicMotion = plain::resolve_asset<CinematicMotionDef>(resolve, ASSET_TYPE_CINEMATICMOTION, data.skydive_canopy_cinematic_motion);
			native.skydive_canopyThrottleTrackSpeed = data.skydive_canopy_throttle_track_speed;
			native.skydive_canopyAnimTrackSpeedThrottle = data.skydive_canopy_anim_track_speed_throttle;
			native.skydive_canopyAnimTrackSpeedStrafe = data.skydive_canopy_anim_track_speed_strafe;
			native.skydive_canopyAnimTrackSpeedYaw = data.skydive_canopy_anim_track_speed_yaw;
			native.skydive_canopyAnimTrackSpeedPitch = data.skydive_canopy_anim_track_speed_pitch;
			native.skydive_canopyAnimMinWeight = data.skydive_canopy_anim_min_weight;
			native.skydive_canopyAnimMaxWeight = data.skydive_canopy_anim_max_weight;
			native.skydive_canopyAnimMinPlayRate = data.skydive_canopy_anim_min_play_rate;
			native.skydive_canopyAnimMaxPlayRate = data.skydive_canopy_anim_max_play_rate;
			native.skydive_canopyAnimSpeedForMinWeight = data.skydive_canopy_anim_speed_for_min_weight;
			native.skydive_canopyAnimSpeedForMaxWeight = data.skydive_canopy_anim_speed_for_max_weight;
			native.skydive_canopyTopGroundSpeed = data.skydive_canopy_top_ground_speed;
			native.skydive_canopyMinPitch = data.skydive_canopy_min_pitch;
			native.skydive_canopyMaxPitch = data.skydive_canopy_max_pitch;
			native.skydive_canopyPitchSpeed = data.skydive_canopy_pitch_speed;
			native.skydive_canopyYawSpeed = data.skydive_canopy_yaw_speed;
			native.skydive_canopyCrossSectionalArea = data.skydive_canopy_cross_sectional_area;
			native.skydive_canopyLookHorizForceMin = data.skydive_canopy_look_horiz_force_min;
			native.skydive_canopyLookHorizForceMax = data.skydive_canopy_look_horiz_force_max;
			native.skydive_canopyLookHorizForceGraph = plain::resolve_asset<RumbleGraph>(resolve, ASSET_TYPE_RUMBLE_GRAPH, data.skydive_canopy_look_horiz_force_graph);
			native.skydive_canopyStickHorizForceMin = data.skydive_canopy_stick_horiz_force_min;
			native.skydive_canopyStickHorizForceMax = data.skydive_canopy_stick_horiz_force_max;
			native.skydive_canopyStickHorizForceGraph = plain::resolve_asset<RumbleGraph>(resolve, ASSET_TYPE_RUMBLE_GRAPH, data.skydive_canopy_stick_horiz_force_graph);
			native.skydive_canopyDragCoefHoriz = data.skydive_canopy_drag_coef_horiz;
			native.skydive_canopyLookHorizDragCoeff = data.skydive_canopy_look_horiz_drag_coeff;
			native.skydive_canopyLookHorizDragGraph = plain::resolve_asset<RumbleGraph>(resolve, ASSET_TYPE_RUMBLE_GRAPH, data.skydive_canopy_look_horiz_drag_graph);
			native.skydive_canopyStickHorizDragCoeff = data.skydive_canopy_stick_horiz_drag_coeff;
			native.skydive_canopyStickHorizDragGraph = plain::resolve_asset<RumbleGraph>(resolve, ASSET_TYPE_RUMBLE_GRAPH, data.skydive_canopy_stick_horiz_drag_graph);
			native.skydive_canopyDragCoefVert = data.skydive_canopy_drag_coef_vert;
			native.skydive_canopyLookVertDragCoeff = data.skydive_canopy_look_vert_drag_coeff;
			native.skydive_canopyLookVertDragGraph = plain::resolve_asset<RumbleGraph>(resolve, ASSET_TYPE_RUMBLE_GRAPH, data.skydive_canopy_look_vert_drag_graph);
			native.skydive_canopyStickVertDragIncreasePerc = data.skydive_canopy_stick_vert_drag_increase_perc;
			native.skydive_canopyStickVertDragIncreaseGraph = plain::resolve_asset<RumbleGraph>(resolve, ASSET_TYPE_RUMBLE_GRAPH, data.skydive_canopy_stick_vert_drag_increase_graph);
			native.skydive_canopyStickVertDragReductionPerc = data.skydive_canopy_stick_vert_drag_reduction_perc;
			native.skydive_canopyStickVertDragReductionGraph = plain::resolve_asset<RumbleGraph>(resolve, ASSET_TYPE_RUMBLE_GRAPH, data.skydive_canopy_stick_vert_drag_reduction_graph);
			native.skydive_canopyStickSidewaysForce = data.skydive_canopy_stick_sideways_force;
			native.skydive_canopyMaxBrakeForce = data.skydive_canopy_max_brake_force;
			native.skydive_canopyMinTurnMultiplier = data.skydive_canopy_min_turn_multiplier;
			native.skydive_canopyMaxTurnMultiplier = data.skydive_canopy_max_turn_multiplier;
			native.skydive_canopyTurnMultiplierGraph = plain::resolve_asset<RumbleGraph>(resolve, ASSET_TYPE_RUMBLE_GRAPH, data.skydive_canopy_turn_multiplier_graph);
			native.skydive_canopyTurnMultiplierMinScale = data.skydive_canopy_turn_multiplier_min_scale;
			native.skydive_canopyTurnMultiplierMaxScale = data.skydive_canopy_turn_multiplier_max_scale;
			native.skydive_canopyTurnMultiplierPitchGraph = plain::resolve_asset<RumbleGraph>(resolve, ASSET_TYPE_RUMBLE_GRAPH, data.skydive_canopy_turn_multiplier_pitch_graph);
			write_snd_alias_lookup(native.skydive_canopyAmbientSound, data.skydive_canopy_ambient_sound, resolve, memory);
			native.skydive_canopyAmbientSoundGroundSpeedWeight = data.skydive_canopy_ambient_sound_ground_speed_weight;
			native.skydive_canopyAmbientSoundMinVolume = data.skydive_canopy_ambient_sound_min_volume;
			native.skydive_canopyAmbientSoundMaxVolume = data.skydive_canopy_ambient_sound_max_volume;
			native.skydive_canopyAmbientSoundMinPitch = data.skydive_canopy_ambient_sound_min_pitch;
			native.skydive_canopyAmbientSoundMaxPitch = data.skydive_canopy_ambient_sound_max_pitch;
			native.skydive_canopyAmbientSoundMinGroundSpeed = data.skydive_canopy_ambient_sound_min_ground_speed;
			native.skydive_canopyAmbientSoundMaxGroundSpeed = data.skydive_canopy_ambient_sound_max_ground_speed;
			native.skydive_canopyAmbientSoundMinFallSpeed = data.skydive_canopy_ambient_sound_min_fall_speed;
			native.skydive_canopyAmbientSoundMaxFallSpeed = data.skydive_canopy_ambient_sound_max_fall_speed;
			write_snd_alias_lookup(native.skydive_canopyHighSpeedSound, data.skydive_canopy_high_speed_sound, resolve, memory);
			native.skydive_canopyHighSpeedSoundGroundSpeedWeight = data.skydive_canopy_high_speed_sound_ground_speed_weight;
			native.skydive_canopyHighSpeedSoundMinVolume = data.skydive_canopy_high_speed_sound_min_volume;
			native.skydive_canopyHighSpeedSoundMaxVolume = data.skydive_canopy_high_speed_sound_max_volume;
			native.skydive_canopyHighSpeedSoundMinPitch = data.skydive_canopy_high_speed_sound_min_pitch;
			native.skydive_canopyHighSpeedSoundMaxPitch = data.skydive_canopy_high_speed_sound_max_pitch;
			native.skydive_canopyHighSpeedSoundMinGroundSpeed = data.skydive_canopy_high_speed_sound_min_ground_speed;
			native.skydive_canopyHighSpeedSoundMaxGroundSpeed = data.skydive_canopy_high_speed_sound_max_ground_speed;
			native.skydive_canopyHighSpeedSoundMinFallSpeed = data.skydive_canopy_high_speed_sound_min_fall_speed;
			native.skydive_canopyHighSpeedSoundMaxFallSpeed = data.skydive_canopy_high_speed_sound_max_fall_speed;
			native.skydive_wmAnimFreefallPitchMin = data.skydive_wm_anim_freefall_pitch_min;
			native.skydive_wmAnimFreefallPitchMax = data.skydive_wm_anim_freefall_pitch_max;
			native.skydive_wmAnimFreefallPitchTrackSpeed = data.skydive_wm_anim_freefall_pitch_track_speed;
			native.skydive_wmAnimFreefallRollTrackSpeed = data.skydive_wm_anim_freefall_roll_track_speed;
			native.skydive_wmAnimFreefallRollReturnSpeed = data.skydive_wm_anim_freefall_roll_return_speed;
			native.skydive_wmAnimFreefallLeftStickXToRollGraph = plain::resolve_asset<RumbleGraph>(resolve, ASSET_TYPE_RUMBLE_GRAPH, data.skydive_wm_anim_freefall_left_stick_x_to_roll_graph);
			native.skydive_wmAnimFreefallRightStickXToRollGraph = plain::resolve_asset<RumbleGraph>(resolve, ASSET_TYPE_RUMBLE_GRAPH, data.skydive_wm_anim_freefall_right_stick_x_to_roll_graph);
			native.skydive_camFreefallHeightOffset = data.skydive_cam_freefall_height_offset;
			native.skydive_camFreefallPitchToDistanceGraph = plain::resolve_asset<RumbleGraph>(resolve, ASSET_TYPE_RUMBLE_GRAPH, data.skydive_cam_freefall_pitch_to_distance_graph);
			native.skydive_camFreefallPitchToForwardOffsetGraph = plain::resolve_asset<RumbleGraph>(resolve, ASSET_TYPE_RUMBLE_GRAPH, data.skydive_cam_freefall_pitch_to_forward_offset_graph);
			native.skydive_camParachuteHeightOffset = data.skydive_cam_parachute_height_offset;
			native.skydive_camParachutePitchToDistanceGraph = plain::resolve_asset<RumbleGraph>(resolve, ASSET_TYPE_RUMBLE_GRAPH, data.skydive_cam_parachute_pitch_to_distance_graph);
			native.skydive_camParachutePitchToForwardOffsetGraph = plain::resolve_asset<RumbleGraph>(resolve, ASSET_TYPE_RUMBLE_GRAPH, data.skydive_cam_parachute_pitch_to_forward_offset_graph);
			native.skydive_camParachutePitchToModelPitchForwardOffsetGraph = plain::resolve_asset<RumbleGraph>(resolve, ASSET_TYPE_RUMBLE_GRAPH, data.skydive_cam_parachute_pitch_to_model_pitch_forward_offset_graph);
			native.skydive_camParachutePitchToModelRollSideOffsetGraph = plain::resolve_asset<RumbleGraph>(resolve, ASSET_TYPE_RUMBLE_GRAPH, data.skydive_cam_parachute_pitch_to_model_roll_side_offset_graph);
			native.skydive_camParachutePitchToModelRollUpOffsetGraph = plain::resolve_asset<RumbleGraph>(resolve, ASSET_TYPE_RUMBLE_GRAPH, data.skydive_cam_parachute_pitch_to_model_roll_up_offset_graph);
			native.skydive_camParachuteBlendInTime = data.skydive_cam_parachute_blend_in_time;
			write_snd_alias_lookup(native.nvg_blindSound, data.nvg_blind_sound, resolve, memory);
			std::ranges::copy(data.dlc_float, native.dlcFloat);
		}

		std::size_t size_suit([[maybe_unused]] const asset_data::gameplay::suit& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			size += size_snd_alias_lookup(data.double_jump_sound);
			size += size_snd_alias_lookup(data.double_jump_sound_player);
			size += size_snd_alias_lookup(data.double_jump_release_sound);
			size += size_snd_alias_lookup(data.double_jump_release_sound_player);
			size += size_bounds(data.bounds_stand);
			size += size_bounds(data.bounds_crouch);
			size += size_bounds(data.bounds_prone);
			size += size_snd_alias_lookup(data.ground_pound_activation_sound);
			size += size_snd_alias_lookup(data.ground_pound_activation_sound_player);
			size += size_snd_alias_lookup(data.ground_pound_landing_sound);
			size += size_snd_alias_lookup(data.ground_pound_landing_sound_player);
			size += size_snd_alias_lookup(data.skydive_freefall_ambient_sound);
			size += size_snd_alias_lookup(data.skydive_freefall_high_speed_sound);
			size += size_snd_alias_lookup(data.skydive_canopy_ambient_sound);
			size += size_snd_alias_lookup(data.skydive_canopy_high_speed_sound);
			size += size_snd_alias_lookup(data.nvg_blind_sound);
			return size;
		}

		asset_data::gameplay::suit_anim_per_weap_class read_suit_anim_per_weap_class(const SuitAnimPerWeapClass& native)
		{
			plain::trace("read_suit_anim_per_weap_class");
			asset_data::gameplay::suit_anim_per_weap_class data;
			data.anims = plain::read_array(native.anims, native.numAnims, "SuitAnimPerWeapClass.anims", [&](const auto& item) { return read_suit_anim_entry(item); });
			data.weap_classes = native.weapClasses;
			data.gestures = plain::read_array(native.gestures, native.numGestures, "SuitAnimPerWeapClass.gestures", [&](const auto& item) { return read_suit_gesture_entry(item); });
			data.cinematic_motion = plain::read_asset(ASSET_TYPE_CINEMATICMOTION, native.cinematicMotion);
			data.footstep = [&] { std::array<asset_data::gameplay::footstep_anim, 13> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_footstep_anim(native.footstep[i]); } return out; }();
			data.skydive_parachute_slow_soft_land_velocity = native.skydiveParachuteSlowSoftLandVelocity;
			data.skydive_parachute_slow_hard_land_velocity = native.skydiveParachuteSlowHardLandVelocity;
			data.skydive_parachute_fast_land_velocity = native.skydiveParachuteFastLandVelocity;
			return data;
		}

		void write_suit_anim_per_weap_class(SuitAnimPerWeapClass& native, const asset_data::gameplay::suit_anim_per_weap_class& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.anims = plain::write_array<SuitAnimEntry>(data.anims, memory, [&](auto& out, const auto& in) { write_suit_anim_entry(out, in, resolve, memory); });
			native.weapClasses = data.weap_classes;
			native.gestures = plain::write_array<SuitGestureEntry>(data.gestures, memory, [&](auto& out, const auto& in) { write_suit_gesture_entry(out, in, resolve, memory); });
			native.cinematicMotion = plain::resolve_asset<CinematicMotionDef>(resolve, ASSET_TYPE_CINEMATICMOTION, data.cinematic_motion);
			for (std::size_t i = 0; i < data.footstep.size(); ++i)
			{
				write_footstep_anim(native.footstep[i], data.footstep[i], resolve, memory);
			}
			native.skydiveParachuteSlowSoftLandVelocity = data.skydive_parachute_slow_soft_land_velocity;
			native.skydiveParachuteSlowHardLandVelocity = data.skydive_parachute_slow_hard_land_velocity;
			native.skydiveParachuteFastLandVelocity = data.skydive_parachute_fast_land_velocity;
			native.numAnims = plain::count_of<std::uint32_t>(data.anims);
			native.numGestures = plain::count_of<std::uint32_t>(data.gestures);
		}

		std::size_t size_suit_anim_per_weap_class([[maybe_unused]] const asset_data::gameplay::suit_anim_per_weap_class& data)
		{
			std::size_t size = 0;
			if (data.anims)
			{
				size += plain::sized(std::max<std::size_t>(data.anims->size(), 1) * sizeof(SuitAnimEntry));
				for (const auto& item : *data.anims)
				{
					size += size_suit_anim_entry(item);
				}
			}
			if (data.gestures)
			{
				size += plain::sized(std::max<std::size_t>(data.gestures->size(), 1) * sizeof(SuitGestureEntry));
				for (const auto& item : *data.gestures)
				{
					size += size_suit_gesture_entry(item);
				}
			}
			for (const auto& item : data.footstep)
			{
				size += size_footstep_anim(item);
			}
			return size;
		}

		asset_data::gameplay::suitanimpackage read_suitanimpackage(const SuitAnimPackage& native)
		{
			plain::trace("read_suitanimpackage");
			asset_data::gameplay::suitanimpackage data;
			data.name = plain::read_string(native.name);
			data.anim_overrides = plain::read_fixed_with<6>(native.animOverrides, [&](const auto& item) { return read_suit_anim_per_weap_class(item); });
			return data;
		}

		void write_suitanimpackage(SuitAnimPackage& native, const asset_data::gameplay::suitanimpackage& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			if (data.anim_overrides)
			{
				native.animOverrides = memory.manual_allocate<SuitAnimPerWeapClass>(plain::rounded(6 * sizeof(SuitAnimPerWeapClass)));
				for (std::size_t i = 0; i < 6; ++i)
				{
					write_suit_anim_per_weap_class(native.animOverrides[i], (*data.anim_overrides)[i], resolve, memory);
				}
			}
		}

		std::size_t size_suitanimpackage([[maybe_unused]] const asset_data::gameplay::suitanimpackage& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.anim_overrides)
			{
				size += plain::sized(6 * sizeof(SuitAnimPerWeapClass));
				for (const auto& item : *data.anim_overrides)
				{
					size += size_suit_anim_per_weap_class(item);
				}
			}
			return size;
		}

		asset_data::gameplay::suit_anim_entry read_suit_anim_entry(const SuitAnimEntry& native)
		{
			plain::trace("read_suit_anim_entry");
			asset_data::gameplay::suit_anim_entry data;
			data.anim = plain::read_asset(ASSET_TYPE_XANIMPARTS, native.anim);
			data.time = native.time;
			data.interrupt_time = native.interruptTime;
			data.anim_type = native.animType;
			return data;
		}

		void write_suit_anim_entry(SuitAnimEntry& native, const asset_data::gameplay::suit_anim_entry& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.anim = plain::resolve_asset<XAnimParts>(resolve, ASSET_TYPE_XANIMPARTS, data.anim);
			native.time = data.time;
			native.interruptTime = data.interrupt_time;
			native.animType = data.anim_type;
		}

		std::size_t size_suit_anim_entry([[maybe_unused]] const asset_data::gameplay::suit_anim_entry& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gameplay::suit_gesture_entry read_suit_gesture_entry(const SuitGestureEntry& native)
		{
			plain::trace("read_suit_gesture_entry");
			asset_data::gameplay::suit_gesture_entry data;
			data.gesture = plain::read_asset(ASSET_TYPE_GESTURE, native.gesture);
			data.gesture_anim_type = plain::enum_name(asset_data::gameplay::gesture_anim_type_t_names, static_cast<std::int64_t>(native.gestureAnimType), "SuitGestureEntry.gestureAnimType");
			return data;
		}

		void write_suit_gesture_entry(SuitGestureEntry& native, const asset_data::gameplay::suit_gesture_entry& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.gesture = plain::resolve_asset<Gesture>(resolve, ASSET_TYPE_GESTURE, data.gesture);
			native.gestureAnimType = static_cast<gestureAnimType_t>(plain::enum_value(asset_data::gameplay::gesture_anim_type_t_names, data.gesture_anim_type, "SuitGestureEntry.gestureAnimType"));
		}

		std::size_t size_suit_gesture_entry([[maybe_unused]] const asset_data::gameplay::suit_gesture_entry& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gameplay::coverselector read_coverselector(const CoverSelector& native)
		{
			plain::trace("read_coverselector");
			asset_data::gameplay::coverselector data;
			data.name = plain::read_string(native.name);
			data.scr_name = plain::read_script_string(native.scrName);
			data.distance_weight = native.distanceWeight;
			data.use_distance_squared = native.useDistanceSquared;
			data.distance_min = native.distanceMin;
			data.distance_optimal = native.distanceOptimal;
			data.distance_max = native.distanceMax;
			data.engagement_weight = native.engagementWeight;
			data.dangerous_weight = native.dangerousWeight;
			data.target_dir_weight = native.targetDirWeight;
			data.move_towards_target_weight = native.moveTowardsTargetWeight;
			data.visible_weight = native.visibleWeight;
			data.not_visible_weight = native.notVisibleWeight;
			data.consider_cover_nodes = native.considerCoverNodes;
			data.consider_tac_points = native.considerTacPoints;
			data.strict_cover_node_weight = native.strictCoverNodeWeight;
			data.conceal_node_weight = native.concealNodeWeight;
			data.exposed_node_weight = native.exposedNodeWeight;
			data.ambush_node_weight = native.ambushNodeWeight;
			data.node_angle_weight = native.nodeAngleWeight;
			data.secondary_target_weight = native.secondaryTargetWeight;
			data.player_sight_weight = native.playerSightWeight;
			data.priority_weight = native.priorityWeight;
			data.lmg_mountable_weight = native.lmgMountableWeight;
			data.indoor_weight = native.indoorWeight;
			data.outdoor_weight = native.outdoorWeight;
			data.height_weight = native.heightWeight;
			data.target_height_weight = native.targetHeightWeight;
			data.stickiness_weight = native.stickinessWeight;
			return data;
		}

		void write_coverselector(CoverSelector& native, const asset_data::gameplay::coverselector& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.scrName = static_cast<scr_string_t>(plain::write_script_string(data.scr_name));
			native.distanceWeight = data.distance_weight;
			native.useDistanceSquared = data.use_distance_squared;
			native.distanceMin = data.distance_min;
			native.distanceOptimal = data.distance_optimal;
			native.distanceMax = data.distance_max;
			native.engagementWeight = data.engagement_weight;
			native.dangerousWeight = data.dangerous_weight;
			native.targetDirWeight = data.target_dir_weight;
			native.moveTowardsTargetWeight = data.move_towards_target_weight;
			native.visibleWeight = data.visible_weight;
			native.notVisibleWeight = data.not_visible_weight;
			native.considerCoverNodes = data.consider_cover_nodes;
			native.considerTacPoints = data.consider_tac_points;
			native.strictCoverNodeWeight = data.strict_cover_node_weight;
			native.concealNodeWeight = data.conceal_node_weight;
			native.exposedNodeWeight = data.exposed_node_weight;
			native.ambushNodeWeight = data.ambush_node_weight;
			native.nodeAngleWeight = data.node_angle_weight;
			native.secondaryTargetWeight = data.secondary_target_weight;
			native.playerSightWeight = data.player_sight_weight;
			native.priorityWeight = data.priority_weight;
			native.lmgMountableWeight = data.lmg_mountable_weight;
			native.indoorWeight = data.indoor_weight;
			native.outdoorWeight = data.outdoor_weight;
			native.heightWeight = data.height_weight;
			native.targetHeightWeight = data.target_height_weight;
			native.stickinessWeight = data.stickiness_weight;
		}

		std::size_t size_coverselector([[maybe_unused]] const asset_data::gameplay::coverselector& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			return size;
		}

		asset_data::gameplay::enemyselector read_enemyselector(const EnemySelector& native)
		{
			plain::trace("read_enemyselector");
			asset_data::gameplay::enemyselector data;
			data.name = plain::read_string(native.name);
			data.scr_name = plain::read_script_string(native.scrName);
			data.threat_visible = native.threatVisible;
			data.threat_fully_aware = native.threatFullyAware;
			data.threat_friendly_timing_out_on_enemy = native.threatFriendlyTimingOutOnEnemy;
			data.threat_scariness_scale = native.threatScarinessScale;
			data.threat_scariness_min = native.threatScarinessMin;
			data.threat_scariness_max = native.threatScarinessMax;
			data.threat_shared_enemy = native.threatSharedEnemy;
			data.threat_shared_enemy_cap = native.threatSharedEnemyCap;
			data.threat_shared_melee_enemy = native.threatSharedMeleeEnemy;
			data.threat_friendlies_current_enemy = native.threatFriendliesCurrentEnemy;
			data.threat_friendlies_timeout_enemy = native.threatFriendliesTimeoutEnemy;
			data.threat_current_enemy = native.threatCurrentEnemy;
			data.threat_current_enemy_player_damaged = native.threatCurrentEnemyPlayerDamaged;
			data.threat_flashed_enemy = native.threatFlashedEnemy;
			data.threat_no_cover = native.threatNoCover;
			data.threat_player_attacked_me_recently = native.threatPlayerAttackedMeRecently;
			data.threat_killed_teammate_recently = native.threatKilledTeammateRecently;
			data.threat_behind_enemy_lines_max = native.threatBehindEnemyLinesMax;
			data.threat_behind_enemy_lines_dist = native.threatBehindEnemyLinesDist;
			data.threat_covering_fire = native.threatCoveringFire;
			data.threat_cover_fire_cur_target = native.threatCoverFireCurTarget;
			data.threat_distance_max_range = native.threatDistanceMaxRange;
			data.threat_distance_max_threat = native.threatDistanceMaxThreat;
			data.threat_min_dist_to_ignore = native.threatMinDistToIgnore;
			data.threat_min_suppression = native.threatMinSuppression;
			return data;
		}

		void write_enemyselector(EnemySelector& native, const asset_data::gameplay::enemyselector& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.scrName = static_cast<scr_string_t>(plain::write_script_string(data.scr_name));
			native.threatVisible = data.threat_visible;
			native.threatFullyAware = data.threat_fully_aware;
			native.threatFriendlyTimingOutOnEnemy = data.threat_friendly_timing_out_on_enemy;
			native.threatScarinessScale = data.threat_scariness_scale;
			native.threatScarinessMin = data.threat_scariness_min;
			native.threatScarinessMax = data.threat_scariness_max;
			native.threatSharedEnemy = data.threat_shared_enemy;
			native.threatSharedEnemyCap = data.threat_shared_enemy_cap;
			native.threatSharedMeleeEnemy = data.threat_shared_melee_enemy;
			native.threatFriendliesCurrentEnemy = data.threat_friendlies_current_enemy;
			native.threatFriendliesTimeoutEnemy = data.threat_friendlies_timeout_enemy;
			native.threatCurrentEnemy = data.threat_current_enemy;
			native.threatCurrentEnemyPlayerDamaged = data.threat_current_enemy_player_damaged;
			native.threatFlashedEnemy = data.threat_flashed_enemy;
			native.threatNoCover = data.threat_no_cover;
			native.threatPlayerAttackedMeRecently = data.threat_player_attacked_me_recently;
			native.threatKilledTeammateRecently = data.threat_killed_teammate_recently;
			native.threatBehindEnemyLinesMax = data.threat_behind_enemy_lines_max;
			native.threatBehindEnemyLinesDist = data.threat_behind_enemy_lines_dist;
			native.threatCoveringFire = data.threat_covering_fire;
			native.threatCoverFireCurTarget = data.threat_cover_fire_cur_target;
			native.threatDistanceMaxRange = data.threat_distance_max_range;
			native.threatDistanceMaxThreat = data.threat_distance_max_threat;
			native.threatMinDistToIgnore = data.threat_min_dist_to_ignore;
			native.threatMinSuppression = data.threat_min_suppression;
		}

		std::size_t size_enemyselector([[maybe_unused]] const asset_data::gameplay::enemyselector& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			return size;
		}

		asset_data::gameplay::carryobject read_carryobject(const CarryObjectDef& native)
		{
			plain::trace("read_carryobject");
			asset_data::gameplay::carryobject data;
			data.name = plain::read_string(native.name);
			data.type = plain::enum_name(asset_data::gameplay::carry_object_type_names, static_cast<std::int64_t>(native.type), "CarryObjectDef.type");
			data.carry_view_model = plain::read_asset(ASSET_TYPE_XMODEL, native.carryViewModel);
			data.carry_world_model = plain::read_asset(ASSET_TYPE_XMODEL, native.carryWorldModel);
			data.gesture = plain::read_asset(ASSET_TYPE_GESTURE, native.gesture);
			data.attach_tag = plain::read_script_string(native.attachTag);
			data.allow_ads = native.allowADS;
			return data;
		}

		void write_carryobject(CarryObjectDef& native, const asset_data::gameplay::carryobject& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.type = static_cast<CarryObjectType>(plain::enum_value(asset_data::gameplay::carry_object_type_names, data.type, "CarryObjectDef.type"));
			native.carryViewModel = plain::resolve_asset<XModel>(resolve, ASSET_TYPE_XMODEL, data.carry_view_model);
			native.carryWorldModel = plain::resolve_asset<XModel>(resolve, ASSET_TYPE_XMODEL, data.carry_world_model);
			native.gesture = plain::resolve_asset<Gesture>(resolve, ASSET_TYPE_GESTURE, data.gesture);
			native.attachTag = static_cast<scr_string_t>(plain::write_script_string(data.attach_tag));
			native.allowADS = data.allow_ads;
		}

		std::size_t size_carryobject([[maybe_unused]] const asset_data::gameplay::carryobject& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			return size;
		}

		asset_data::gameplay::execution read_execution(const ExecutionDef& native)
		{
			plain::trace("read_execution");
			asset_data::gameplay::execution data;
			data.name = plain::read_string(native.name);
			data.anim_type = native.animType;
			data.use_prop_weapon = native.usePropWeapon;
			data.prop_weapon_attach_tag = plain::read_script_string(native.propWeaponAttachTag);
			data.prop_weapon_inventory_override_enabled = native.propWeaponInventoryOverrideEnabled;
			data.prop_weapon_enable_ik = native.propWeaponEnableIK;
			data.prop_weapon_inventory_override_class = plain::enum_name(asset_data::gameplay::weap_class_t_names, static_cast<std::int64_t>(native.propWeaponInventoryOverrideClass), "ExecutionDef.propWeaponInventoryOverrideClass");
			data.duration_victim_stand_ms = native.durationVictimStandMs;
			data.xcam_victim_stand = plain::read_asset(ASSET_TYPE_XCAM, native.xcamVictimStand);
			data.duration_victim_prone_ms = native.durationVictimProneMs;
			data.xcam_victim_prone = plain::read_asset(ASSET_TYPE_XCAM, native.xcamVictimProne);
			data.duration_victim_last_stand_ms = native.durationVictimLastStandMs;
			data.xcam_victim_last_stand = plain::read_asset(ASSET_TYPE_XCAM, native.xcamVictimLastStand);
			data.dlc_float = std::to_array(native.dlcFloat);
			return data;
		}

		void write_execution(ExecutionDef& native, const asset_data::gameplay::execution& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.animType = data.anim_type;
			native.usePropWeapon = data.use_prop_weapon;
			native.propWeaponAttachTag = static_cast<scr_string_t>(plain::write_script_string(data.prop_weapon_attach_tag));
			native.propWeaponInventoryOverrideEnabled = data.prop_weapon_inventory_override_enabled;
			native.propWeaponEnableIK = data.prop_weapon_enable_ik;
			native.propWeaponInventoryOverrideClass = static_cast<weapClass_t>(plain::enum_value(asset_data::gameplay::weap_class_t_names, data.prop_weapon_inventory_override_class, "ExecutionDef.propWeaponInventoryOverrideClass"));
			native.durationVictimStandMs = data.duration_victim_stand_ms;
			native.xcamVictimStand = plain::resolve_asset<XCam>(resolve, ASSET_TYPE_XCAM, data.xcam_victim_stand);
			native.durationVictimProneMs = data.duration_victim_prone_ms;
			native.xcamVictimProne = plain::resolve_asset<XCam>(resolve, ASSET_TYPE_XCAM, data.xcam_victim_prone);
			native.durationVictimLastStandMs = data.duration_victim_last_stand_ms;
			native.xcamVictimLastStand = plain::resolve_asset<XCam>(resolve, ASSET_TYPE_XCAM, data.xcam_victim_last_stand);
			std::ranges::copy(data.dlc_float, native.dlcFloat);
		}

		std::size_t size_execution([[maybe_unused]] const asset_data::gameplay::execution& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			return size;
		}

		asset_data::gameplay::clientcharacter read_clientcharacter(const ClientCharacter& native)
		{
			plain::trace("read_clientcharacter");
			asset_data::gameplay::clientcharacter data;
			data.name = plain::read_string(native.name);
			data.scriptable_def = plain::read_asset(ASSET_TYPE_SCRIPTABLE, native.scriptableDef);
			data.anim_file_name = plain::read_string(native.animFileName);
			data.anim_tree_name = plain::read_string(native.animTreeName);
			return data;
		}

		void write_clientcharacter(ClientCharacter& native, const asset_data::gameplay::clientcharacter& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.scriptableDef = plain::resolve_asset<ScriptableDef>(resolve, ASSET_TYPE_SCRIPTABLE, data.scriptable_def);
			native.animFileName = plain::write_string(memory, data.anim_file_name);
			native.animTreeName = plain::write_string(memory, data.anim_tree_name);
		}

		std::size_t size_clientcharacter([[maybe_unused]] const asset_data::gameplay::clientcharacter& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			size += plain::string_size(data.anim_file_name);
			size += plain::string_size(data.anim_tree_name);
			return size;
		}

		asset_data::gameplay::physicslibrary read_physicslibrary(const PhysicsLibrary& native)
		{
			plain::trace("read_physicslibrary");
			asset_data::gameplay::physicslibrary data;
			data.name = plain::read_string(native.name);
			data.is_material_list = native.isMaterialList;
			data.is_body_quality_list = native.isBodyQualityList;
			data.is_motion_properties_list = native.isMotionPropertiesList;
			data.is_global_type_compendium = native.isGlobalTypeCompendium;
			data.havok_data = plain::read_array(native.havokData, native.havokDataSize, "PhysicsLibrary.havokData", [&](const auto& item) { return item; });
			return data;
		}

		void write_physicslibrary(PhysicsLibrary& native, const asset_data::gameplay::physicslibrary& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.isMaterialList = data.is_material_list;
			native.isBodyQualityList = data.is_body_quality_list;
			native.isMotionPropertiesList = data.is_motion_properties_list;
			native.isGlobalTypeCompendium = data.is_global_type_compendium;
			native.havokData = plain::write_array<char>(data.havok_data, memory, [&](auto& out, const auto& in) { out = in; });
			native.havokDataSize = plain::count_of<std::uint32_t>(data.havok_data);
		}

		std::size_t size_physicslibrary([[maybe_unused]] const asset_data::gameplay::physicslibrary& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.havok_data)
			{
				size += plain::sized(std::max<std::size_t>(data.havok_data->size(), 1) * sizeof(char));
			}
			return size;
		}

		asset_data::gameplay::ttf read_ttf(const TTFDef& native)
		{
			plain::trace("read_ttf");
			asset_data::gameplay::ttf data;
			data.name = plain::read_string(native.name);
			data.file_len = native.fileLen;
			data.file = plain::read_array(native.file, (native.fileLen + 1), "TTFDef.file", [&](const auto& item) { return item; });
			plain::require_zero(native.ftFace, "TTFDef.ftFace");
			return data;
		}

		void write_ttf(TTFDef& native, const asset_data::gameplay::ttf& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.fileLen = data.file_len;
			native.file = plain::write_array<char>(data.file, memory, [&](auto& out, const auto& in) { out = in; });
		}

		std::size_t size_ttf([[maybe_unused]] const asset_data::gameplay::ttf& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.file)
			{
				size += plain::sized(std::max<std::size_t>(data.file->size(), 1) * sizeof(char));
			}
			return size;
		}

	}

	std::expected<asset_data::gameplay::animpkg, std::string> to_neutral(const WeaponAnimPackage& native)
	{
		try
		{
			auto data = read_animpkg(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	WeaponAnimPackage* from_neutral(const asset_data::gameplay::animpkg& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<WeaponAnimPackage>();
		write_animpkg(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::gameplay::animpkg& data)
	{
		return plain::rounded(sizeof(WeaponAnimPackage)) + size_animpkg(data) + 64;
	}

	std::expected<asset_data::gameplay::sfxpkg, std::string> to_neutral(const WeaponSFXPackage& native)
	{
		try
		{
			auto data = read_sfxpkg(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	WeaponSFXPackage* from_neutral(const asset_data::gameplay::sfxpkg& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<WeaponSFXPackage>();
		write_sfxpkg(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::gameplay::sfxpkg& data)
	{
		return plain::rounded(sizeof(WeaponSFXPackage)) + size_sfxpkg(data) + 64;
	}

	std::expected<asset_data::gameplay::vfxpkg, std::string> to_neutral(const WeaponVFXPackage& native)
	{
		try
		{
			auto data = read_vfxpkg(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	WeaponVFXPackage* from_neutral(const asset_data::gameplay::vfxpkg& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<WeaponVFXPackage>();
		write_vfxpkg(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::gameplay::vfxpkg& data)
	{
		return plain::rounded(sizeof(WeaponVFXPackage)) + size_vfxpkg(data) + 64;
	}

	std::expected<asset_data::gameplay::tracer, std::string> to_neutral(const TracerDef& native)
	{
		try
		{
			auto data = read_tracer(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	TracerDef* from_neutral(const asset_data::gameplay::tracer& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<TracerDef>();
		write_tracer(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::gameplay::tracer& data)
	{
		return plain::rounded(sizeof(TracerDef)) + size_tracer(data) + 64;
	}

	std::expected<asset_data::gameplay::laser, std::string> to_neutral(const LaserDef& native)
	{
		try
		{
			auto data = read_laser(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	LaserDef* from_neutral(const asset_data::gameplay::laser& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<LaserDef>();
		write_laser(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::gameplay::laser& data)
	{
		return plain::rounded(sizeof(LaserDef)) + size_laser(data) + 64;
	}

	std::expected<asset_data::gameplay::camera, std::string> to_neutral(const CameraDef& native)
	{
		try
		{
			auto data = read_camera(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	CameraDef* from_neutral(const asset_data::gameplay::camera& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<CameraDef>();
		write_camera(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::gameplay::camera& data)
	{
		return plain::rounded(sizeof(CameraDef)) + size_camera(data) + 64;
	}

	std::expected<asset_data::gameplay::hudoutline, std::string> to_neutral(const HudOutlineDef& native)
	{
		try
		{
			auto data = read_hudoutline(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	HudOutlineDef* from_neutral(const asset_data::gameplay::hudoutline& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<HudOutlineDef>();
		write_hudoutline(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::gameplay::hudoutline& data)
	{
		return plain::rounded(sizeof(HudOutlineDef)) + size_hudoutline(data) + 64;
	}

	std::expected<asset_data::gameplay::footstepvfx, std::string> to_neutral(const FootstepVFX& native)
	{
		try
		{
			auto data = read_footstepvfx(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	FootstepVFX* from_neutral(const asset_data::gameplay::footstepvfx& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<FootstepVFX>();
		write_footstepvfx(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::gameplay::footstepvfx& data)
	{
		return plain::rounded(sizeof(FootstepVFX)) + size_footstepvfx(data) + 64;
	}

	std::expected<asset_data::gameplay::xcompositemodel, std::string> to_neutral(const XCompositeModelDef& native)
	{
		try
		{
			auto data = read_xcompositemodel(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	XCompositeModelDef* from_neutral(const asset_data::gameplay::xcompositemodel& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<XCompositeModelDef>();
		write_xcompositemodel(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::gameplay::xcompositemodel& data)
	{
		return plain::rounded(sizeof(XCompositeModelDef)) + size_xcompositemodel(data) + 64;
	}

	std::expected<asset_data::gameplay::xanimcurve, std::string> to_neutral(const XAnimCurve& native)
	{
		try
		{
			auto data = read_xanimcurve(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	XAnimCurve* from_neutral(const asset_data::gameplay::xanimcurve& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<XAnimCurve>();
		write_xanimcurve(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::gameplay::xanimcurve& data)
	{
		return plain::rounded(sizeof(XAnimCurve)) + size_xanimcurve(data) + 64;
	}

	std::expected<asset_data::gameplay::keyvaluepairs, std::string> to_neutral(const KeyValuePairs& native)
	{
		try
		{
			auto data = read_keyvaluepairs(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	KeyValuePairs* from_neutral(const asset_data::gameplay::keyvaluepairs& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<KeyValuePairs>();
		write_keyvaluepairs(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::gameplay::keyvaluepairs& data)
	{
		return plain::rounded(sizeof(KeyValuePairs)) + size_keyvaluepairs(data) + 64;
	}

	std::expected<asset_data::gameplay::blendspace2d, std::string> to_neutral(const BlendSpace2DDef& native)
	{
		try
		{
			auto data = read_blendspace2d(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	BlendSpace2DDef* from_neutral(const asset_data::gameplay::blendspace2d& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<BlendSpace2DDef>();
		write_blendspace2d(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::gameplay::blendspace2d& data)
	{
		return plain::rounded(sizeof(BlendSpace2DDef)) + size_blendspace2d(data) + 64;
	}

	std::expected<asset_data::gameplay::camo, std::string> to_neutral(const Camo& native)
	{
		try
		{
			auto data = read_camo(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	Camo* from_neutral(const asset_data::gameplay::camo& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<Camo>();
		write_camo(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::gameplay::camo& data)
	{
		return plain::rounded(sizeof(Camo)) + size_camo(data) + 64;
	}

	std::expected<asset_data::gameplay::accessory, std::string> to_neutral(const AccessoryDef& native)
	{
		try
		{
			auto data = read_accessory(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	AccessoryDef* from_neutral(const asset_data::gameplay::accessory& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<AccessoryDef>();
		write_accessory(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::gameplay::accessory& data)
	{
		return plain::rounded(sizeof(AccessoryDef)) + size_accessory(data) + 64;
	}

	std::expected<asset_data::gameplay::locdmgtable, std::string> to_neutral(const LocDmgTable& native)
	{
		try
		{
			auto data = read_locdmgtable(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	LocDmgTable* from_neutral(const asset_data::gameplay::locdmgtable& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<LocDmgTable>();
		write_locdmgtable(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::gameplay::locdmgtable& data)
	{
		return plain::rounded(sizeof(LocDmgTable)) + size_locdmgtable(data) + 64;
	}

	std::expected<asset_data::gameplay::bulletpenetration, std::string> to_neutral(const BulletPenetration& native)
	{
		try
		{
			auto data = read_bulletpenetration(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	BulletPenetration* from_neutral(const asset_data::gameplay::bulletpenetration& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<BulletPenetration>();
		write_bulletpenetration(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::gameplay::bulletpenetration& data)
	{
		return plain::rounded(sizeof(BulletPenetration)) + size_bulletpenetration(data) + 64;
	}

	std::expected<asset_data::gameplay::cinematicmotion, std::string> to_neutral(const CinematicMotionDef& native)
	{
		try
		{
			auto data = read_cinematicmotion(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	CinematicMotionDef* from_neutral(const asset_data::gameplay::cinematicmotion& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<CinematicMotionDef>();
		write_cinematicmotion(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::gameplay::cinematicmotion& data)
	{
		return plain::rounded(sizeof(CinematicMotionDef)) + size_cinematicmotion(data) + 64;
	}

	std::expected<asset_data::gameplay::decalvolumemask, std::string> to_neutral(const GfxDecalVolumeMask& native)
	{
		try
		{
			auto data = read_decalvolumemask(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	GfxDecalVolumeMask* from_neutral(const asset_data::gameplay::decalvolumemask& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<GfxDecalVolumeMask>();
		write_decalvolumemask(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::gameplay::decalvolumemask& data)
	{
		return plain::rounded(sizeof(GfxDecalVolumeMask)) + size_decalvolumemask(data) + 64;
	}

	std::expected<asset_data::gameplay::gesture, std::string> to_neutral(const Gesture& native)
	{
		try
		{
			auto data = read_gesture(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	Gesture* from_neutral(const asset_data::gameplay::gesture& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<Gesture>();
		write_gesture(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::gameplay::gesture& data)
	{
		return plain::rounded(sizeof(Gesture)) + size_gesture(data) + 64;
	}

	std::expected<asset_data::gameplay::reticle, std::string> to_neutral(const ReticleDef& native)
	{
		try
		{
			auto data = read_reticle(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	ReticleDef* from_neutral(const asset_data::gameplay::reticle& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<ReticleDef>();
		write_reticle(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::gameplay::reticle& data)
	{
		return plain::rounded(sizeof(ReticleDef)) + size_reticle(data) + 64;
	}

	std::expected<asset_data::gameplay::streaminginfo, std::string> to_neutral(const StreamingInfo& native)
	{
		try
		{
			auto data = read_streaminginfo(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	StreamingInfo* from_neutral(const asset_data::gameplay::streaminginfo& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<StreamingInfo>();
		write_streaminginfo(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::gameplay::streaminginfo& data)
	{
		return plain::rounded(sizeof(StreamingInfo)) + size_streaminginfo(data) + 64;
	}

	std::expected<asset_data::gameplay::suit, std::string> to_neutral(const SuitDef& native)
	{
		try
		{
			auto data = read_suit(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	SuitDef* from_neutral(const asset_data::gameplay::suit& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<SuitDef>();
		write_suit(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::gameplay::suit& data)
	{
		return plain::rounded(sizeof(SuitDef)) + size_suit(data) + 64;
	}

	std::expected<asset_data::gameplay::suitanimpackage, std::string> to_neutral(const SuitAnimPackage& native)
	{
		try
		{
			auto data = read_suitanimpackage(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	SuitAnimPackage* from_neutral(const asset_data::gameplay::suitanimpackage& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<SuitAnimPackage>();
		write_suitanimpackage(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::gameplay::suitanimpackage& data)
	{
		return plain::rounded(sizeof(SuitAnimPackage)) + size_suitanimpackage(data) + 64;
	}

	std::expected<asset_data::gameplay::coverselector, std::string> to_neutral(const CoverSelector& native)
	{
		try
		{
			auto data = read_coverselector(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	CoverSelector* from_neutral(const asset_data::gameplay::coverselector& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<CoverSelector>();
		write_coverselector(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::gameplay::coverselector& data)
	{
		return plain::rounded(sizeof(CoverSelector)) + size_coverselector(data) + 64;
	}

	std::expected<asset_data::gameplay::enemyselector, std::string> to_neutral(const EnemySelector& native)
	{
		try
		{
			auto data = read_enemyselector(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	EnemySelector* from_neutral(const asset_data::gameplay::enemyselector& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<EnemySelector>();
		write_enemyselector(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::gameplay::enemyselector& data)
	{
		return plain::rounded(sizeof(EnemySelector)) + size_enemyselector(data) + 64;
	}

	std::expected<asset_data::gameplay::carryobject, std::string> to_neutral(const CarryObjectDef& native)
	{
		try
		{
			auto data = read_carryobject(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	CarryObjectDef* from_neutral(const asset_data::gameplay::carryobject& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<CarryObjectDef>();
		write_carryobject(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::gameplay::carryobject& data)
	{
		return plain::rounded(sizeof(CarryObjectDef)) + size_carryobject(data) + 64;
	}

	std::expected<asset_data::gameplay::execution, std::string> to_neutral(const ExecutionDef& native)
	{
		try
		{
			auto data = read_execution(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	ExecutionDef* from_neutral(const asset_data::gameplay::execution& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<ExecutionDef>();
		write_execution(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::gameplay::execution& data)
	{
		return plain::rounded(sizeof(ExecutionDef)) + size_execution(data) + 64;
	}

	std::expected<asset_data::gameplay::clientcharacter, std::string> to_neutral(const ClientCharacter& native)
	{
		try
		{
			auto data = read_clientcharacter(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	ClientCharacter* from_neutral(const asset_data::gameplay::clientcharacter& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<ClientCharacter>();
		write_clientcharacter(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::gameplay::clientcharacter& data)
	{
		return plain::rounded(sizeof(ClientCharacter)) + size_clientcharacter(data) + 64;
	}

	std::expected<asset_data::gameplay::physicslibrary, std::string> to_neutral(const PhysicsLibrary& native)
	{
		try
		{
			auto data = read_physicslibrary(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	PhysicsLibrary* from_neutral(const asset_data::gameplay::physicslibrary& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<PhysicsLibrary>();
		write_physicslibrary(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::gameplay::physicslibrary& data)
	{
		return plain::rounded(sizeof(PhysicsLibrary)) + size_physicslibrary(data) + 64;
	}

	std::expected<asset_data::gameplay::ttf, std::string> to_neutral(const TTFDef& native)
	{
		try
		{
			auto data = read_ttf(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	TTFDef* from_neutral(const asset_data::gameplay::ttf& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<TTFDef>();
		write_ttf(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::gameplay::ttf& data)
	{
		return plain::rounded(sizeof(TTFDef)) + size_ttf(data) + 64;
	}

}
