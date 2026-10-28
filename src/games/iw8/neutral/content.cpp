// Emitted by tools/gen_iw8_neutral.py from tools/neutral_families/content.json: do not edit.
#include <std_include.hpp>
#include "content.hpp"
#include "plain.hpp"

namespace zonetool::iw8::neutral
{
	namespace
	{
		asset_data::content::attachment read_attachment(const WeaponAttachment& native);
		void write_attachment(WeaponAttachment& native, const asset_data::content::attachment& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_attachment(const asset_data::content::attachment& data);
		asset_data::content::att_ammo_general read_att_ammo_general(const AttAmmoGeneral& native);
		void write_att_ammo_general(AttAmmoGeneral& native, const asset_data::content::att_ammo_general& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_ammo_general(const asset_data::content::att_ammo_general& data);
		asset_data::content::att_ammo_type read_att_ammo_type(const AttAmmoType& native);
		void write_att_ammo_type(AttAmmoType& native, const asset_data::content::att_ammo_type& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_ammo_type(const asset_data::content::att_ammo_type& data);
		asset_data::content::att_impact read_att_impact(const AttImpact& native);
		void write_att_impact(AttImpact& native, const asset_data::content::att_impact& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_impact(const asset_data::content::att_impact& data);
		asset_data::content::att_ammo_tracer read_att_ammo_tracer(const AttAmmoTracer& native);
		void write_att_ammo_tracer(AttAmmoTracer& native, const asset_data::content::att_ammo_tracer& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_ammo_tracer(const asset_data::content::att_ammo_tracer& data);
		asset_data::content::att_penetration read_att_penetration(const AttPenetration& native);
		void write_att_penetration(AttPenetration& native, const asset_data::content::att_penetration& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_penetration(const asset_data::content::att_penetration& data);
		asset_data::content::att_sight read_att_sight(const AttSight& native);
		void write_att_sight(AttSight& native, const asset_data::content::att_sight& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_sight(const asset_data::content::att_sight& data);
		asset_data::content::att_hold_breath read_att_hold_breath(const AttHoldBreath& native);
		void write_att_hold_breath(AttHoldBreath& native, const asset_data::content::att_hold_breath& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_hold_breath(const asset_data::content::att_hold_breath& data);
		asset_data::content::att_reload read_att_reload(const AttReload& native);
		void write_att_reload(AttReload& native, const asset_data::content::att_reload& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_reload(const asset_data::content::att_reload& data);
		asset_data::content::att_add_ons read_att_add_ons(const AttAddOns& native);
		void write_att_add_ons(AttAddOns& native, const asset_data::content::att_add_ons& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_add_ons(const asset_data::content::att_add_ons& data);
		asset_data::content::att_laser read_att_laser(const AttLaser& native);
		void write_att_laser(AttLaser& native, const asset_data::content::att_laser& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_laser(const asset_data::content::att_laser& data);
		asset_data::content::laser_settings read_laser_settings(const LaserSettings& native);
		void write_laser_settings(LaserSettings& native, const asset_data::content::laser_settings& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_laser_settings(const asset_data::content::laser_settings& data);
		asset_data::content::att_post read_att_post(const AttPost& native);
		void write_att_post(AttPost& native, const asset_data::content::att_post& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_post(const asset_data::content::att_post& data);
		asset_data::content::att_general read_att_general(const AttGeneral& native);
		void write_att_general(AttGeneral& native, const asset_data::content::att_general& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_general(const asset_data::content::att_general& data);
		asset_data::content::att_aim_assist read_att_aim_assist(const AttAimAssist& native);
		void write_att_aim_assist(AttAimAssist& native, const asset_data::content::att_aim_assist& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_aim_assist(const asset_data::content::att_aim_assist& data);
		asset_data::content::att_target_assist read_att_target_assist(const AttTargetAssist& native);
		void write_att_target_assist(AttTargetAssist& native, const asset_data::content::att_target_assist& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_target_assist(const asset_data::content::att_target_assist& data);
		asset_data::content::att_ammunition read_att_ammunition(const AttAmmunition& native);
		void write_att_ammunition(AttAmmunition& native, const asset_data::content::att_ammunition& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_ammunition(const asset_data::content::att_ammunition& data);
		asset_data::content::att_damage read_att_damage(const AttDamage& native);
		void write_att_damage(AttDamage& native, const asset_data::content::att_damage& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_damage(const asset_data::content::att_damage& data);
		asset_data::content::weapon_damage_info read_weapon_damage_info(const WeaponDamageInfo& native);
		void write_weapon_damage_info(WeaponDamageInfo& native, const asset_data::content::weapon_damage_info& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_weapon_damage_info(const asset_data::content::weapon_damage_info& data);
		asset_data::content::weapon_damage_common read_weapon_damage_common(const WeaponDamageCommon& native);
		void write_weapon_damage_common(WeaponDamageCommon& native, const asset_data::content::weapon_damage_common& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_weapon_damage_common(const asset_data::content::weapon_damage_common& data);
		asset_data::content::att_melee read_att_melee(const AttMelee& native);
		void write_att_melee(AttMelee& native, const asset_data::content::att_melee& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_melee(const asset_data::content::att_melee& data);
		asset_data::content::att_idle_settings read_att_idle_settings(const AttIdleSettings& native);
		void write_att_idle_settings(AttIdleSettings& native, const asset_data::content::att_idle_settings& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_idle_settings(const asset_data::content::att_idle_settings& data);
		asset_data::content::advanced_idle_settings read_advanced_idle_settings(const AdvancedIdleSettings& native);
		void write_advanced_idle_settings(AdvancedIdleSettings& native, const asset_data::content::advanced_idle_settings& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_advanced_idle_settings(const asset_data::content::advanced_idle_settings& data);
		asset_data::content::att_ads_settings read_att_ads_settings(const AttADSSettings& native);
		void write_att_ads_settings(AttADSSettings& native, const asset_data::content::att_ads_settings& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_ads_settings(const asset_data::content::att_ads_settings& data);
		asset_data::content::zoom_settings read_zoom_settings(const ZoomSettings& native);
		void write_zoom_settings(ZoomSettings& native, const asset_data::content::zoom_settings& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_zoom_settings(const asset_data::content::zoom_settings& data);
		asset_data::content::scene_zoom_settings read_scene_zoom_settings(const SceneZoomSettings& native);
		void write_scene_zoom_settings(SceneZoomSettings& native, const asset_data::content::scene_zoom_settings& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_scene_zoom_settings(const asset_data::content::scene_zoom_settings& data);
		asset_data::content::weapon_zoom_settings read_weapon_zoom_settings(const WeaponZoomSettings& native);
		void write_weapon_zoom_settings(WeaponZoomSettings& native, const asset_data::content::weapon_zoom_settings& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_weapon_zoom_settings(const asset_data::content::weapon_zoom_settings& data);
		asset_data::content::att_ads_gun_bob_settings read_att_ads_gun_bob_settings(const AttADSGunBobSettings& native);
		void write_att_ads_gun_bob_settings(AttADSGunBobSettings& native, const asset_data::content::att_ads_gun_bob_settings& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_ads_gun_bob_settings(const asset_data::content::att_ads_gun_bob_settings& data);
		asset_data::content::att_ads_reload_settings read_att_ads_reload_settings(const AttADSReloadSettings& native);
		void write_att_ads_reload_settings(AttADSReloadSettings& native, const asset_data::content::att_ads_reload_settings& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_ads_reload_settings(const asset_data::content::att_ads_reload_settings& data);
		asset_data::content::att_scope_drift_settings read_att_scope_drift_settings(const AttScopeDriftSettings& native);
		void write_att_scope_drift_settings(AttScopeDriftSettings& native, const asset_data::content::att_scope_drift_settings& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_scope_drift_settings(const asset_data::content::att_scope_drift_settings& data);
		asset_data::content::att_ads_stance_scales read_att_ads_stance_scales(const AttADSStanceScales& native);
		void write_att_ads_stance_scales(AttADSStanceScales& native, const asset_data::content::att_ads_stance_scales& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_ads_stance_scales(const asset_data::content::att_ads_stance_scales& data);
		asset_data::content::weapon_offset_pattern_scale_info read_weapon_offset_pattern_scale_info(const WeaponOffsetPatternScaleInfo& native);
		void write_weapon_offset_pattern_scale_info(WeaponOffsetPatternScaleInfo& native, const asset_data::content::weapon_offset_pattern_scale_info& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_weapon_offset_pattern_scale_info(const asset_data::content::weapon_offset_pattern_scale_info& data);
		asset_data::content::weapon_offset_pattern_scale read_weapon_offset_pattern_scale(const WeaponOffsetPatternScale& native);
		void write_weapon_offset_pattern_scale(WeaponOffsetPatternScale& native, const asset_data::content::weapon_offset_pattern_scale& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_weapon_offset_pattern_scale(const asset_data::content::weapon_offset_pattern_scale& data);
		asset_data::content::att_hip_spread read_att_hip_spread(const AttHipSpread& native);
		void write_att_hip_spread(AttHipSpread& native, const asset_data::content::att_hip_spread& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_hip_spread(const asset_data::content::att_hip_spread& data);
		asset_data::content::att_slide_spread read_att_slide_spread(const AttSlideSpread& native);
		void write_att_slide_spread(AttSlideSpread& native, const asset_data::content::att_slide_spread& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_slide_spread(const asset_data::content::att_slide_spread& data);
		asset_data::content::att_gun_kick read_att_gun_kick(const AttGunKick& native);
		void write_att_gun_kick(AttGunKick& native, const asset_data::content::att_gun_kick& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_gun_kick(const asset_data::content::att_gun_kick& data);
		asset_data::content::att_gun_tilt read_att_gun_tilt(const AttGunTilt& native);
		void write_att_gun_tilt(AttGunTilt& native, const asset_data::content::att_gun_tilt& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_gun_tilt(const asset_data::content::att_gun_tilt& data);
		asset_data::content::att_kick_scaling read_att_kick_scaling(const AttKickScaling& native);
		void write_att_kick_scaling(AttKickScaling& native, const asset_data::content::att_kick_scaling& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_kick_scaling(const asset_data::content::att_kick_scaling& data);
		asset_data::content::att_view_kick read_att_view_kick(const AttViewKick& native);
		void write_att_view_kick(AttViewKick& native, const asset_data::content::att_view_kick& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_view_kick(const asset_data::content::att_view_kick& data);
		asset_data::content::att_ads_overlay read_att_ads_overlay(const AttADSOverlay& native);
		void write_att_ads_overlay(AttADSOverlay& native, const asset_data::content::att_ads_overlay& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_ads_overlay(const asset_data::content::att_ads_overlay& data);
		asset_data::content::ads_overlay read_ads_overlay(const ADSOverlay& native);
		void write_ads_overlay(ADSOverlay& native, const asset_data::content::ads_overlay& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_ads_overlay(const asset_data::content::ads_overlay& data);
		asset_data::content::att_outline read_att_outline(const AttOutline& native);
		void write_att_outline(AttOutline& native, const asset_data::content::att_outline& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_outline(const asset_data::content::att_outline& data);
		asset_data::content::att_ui read_att_ui(const AttUI& native);
		void write_att_ui(AttUI& native, const asset_data::content::att_ui& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_ui(const asset_data::content::att_ui& data);
		asset_data::content::att_rumbles read_att_rumbles(const AttRumbles& native);
		void write_att_rumbles(AttRumbles& native, const asset_data::content::att_rumbles& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_rumbles(const asset_data::content::att_rumbles& data);
		asset_data::content::att_projectile read_att_projectile(const AttProjectile& native);
		void write_att_projectile(AttProjectile& native, const asset_data::content::att_projectile& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_projectile(const asset_data::content::att_projectile& data);
		asset_data::content::fx_combined_def read_fx_combined_def(const FxCombinedDef& native);
		void write_fx_combined_def(FxCombinedDef& native, const asset_data::content::fx_combined_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_fx_combined_def(const asset_data::content::fx_combined_def& data);
		asset_data::content::snd_alias_lookup read_snd_alias_lookup(const SndAliasLookup& native);
		void write_snd_alias_lookup(SndAliasLookup& native, const asset_data::content::snd_alias_lookup& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_snd_alias_lookup(const asset_data::content::snd_alias_lookup& data);
		asset_data::content::att_charged read_att_charged(const AttCharged& native);
		void write_att_charged(AttCharged& native, const asset_data::content::att_charged& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_charged(const asset_data::content::att_charged& data);
		asset_data::content::ads_alt_switch read_ads_alt_switch(const AdsAltSwitch& native);
		void write_ads_alt_switch(AdsAltSwitch& native, const asset_data::content::ads_alt_switch& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_ads_alt_switch(const asset_data::content::ads_alt_switch& data);
		asset_data::content::att_regeneration read_att_regeneration(const AttRegeneration& native);
		void write_att_regeneration(AttRegeneration& native, const asset_data::content::att_regeneration& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_regeneration(const asset_data::content::att_regeneration& data);
		asset_data::content::att_heat read_att_heat(const AttHeat& native);
		void write_att_heat(AttHeat& native, const asset_data::content::att_heat& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_heat(const asset_data::content::att_heat& data);
		asset_data::content::att_movement read_att_movement(const AttMovement& native);
		void write_att_movement(AttMovement& native, const asset_data::content::att_movement& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_movement(const asset_data::content::att_movement& data);
		asset_data::content::att_burst read_att_burst(const AttBurst& native);
		void write_att_burst(AttBurst& native, const asset_data::content::att_burst& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_burst(const asset_data::content::att_burst& data);
		asset_data::content::att_hitmarker read_att_hitmarker(const AttHitmarker& native);
		void write_att_hitmarker(AttHitmarker& native, const asset_data::content::att_hitmarker& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_hitmarker(const asset_data::content::att_hitmarker& data);
		asset_data::content::att_ads_movement read_att_ads_movement(const AttAdsMovement& native);
		void write_att_ads_movement(AttAdsMovement& native, const asset_data::content::att_ads_movement& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_ads_movement(const asset_data::content::att_ads_movement& data);
		asset_data::content::ballistic_info read_ballistic_info(const BallisticInfo& native);
		void write_ballistic_info(BallisticInfo& native, const asset_data::content::ballistic_info& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_ballistic_info(const asset_data::content::ballistic_info& data);
		asset_data::content::ballistic_info_calculated read_ballistic_info_calculated(const BallisticInfoCalculated& native);
		void write_ballistic_info_calculated(BallisticInfoCalculated& native, const asset_data::content::ballistic_info_calculated& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_ballistic_info_calculated(const asset_data::content::ballistic_info_calculated& data);
		asset_data::content::sway_settings read_sway_settings(const SwaySettings& native);
		void write_sway_settings(SwaySettings& native, const asset_data::content::sway_settings& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_sway_settings(const asset_data::content::sway_settings& data);
		asset_data::content::hip_sway_settings read_hip_sway_settings(const HipSwaySettings& native);
		void write_hip_sway_settings(HipSwaySettings& native, const asset_data::content::hip_sway_settings& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_hip_sway_settings(const asset_data::content::hip_sway_settings& data);
		asset_data::content::common_sway_settings read_common_sway_settings(const CommonSwaySettings& native);
		void write_common_sway_settings(CommonSwaySettings& native, const asset_data::content::common_sway_settings& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_common_sway_settings(const asset_data::content::common_sway_settings& data);
		asset_data::content::ads_sway_settings read_ads_sway_settings(const AdsSwaySettings& native);
		void write_ads_sway_settings(AdsSwaySettings& native, const asset_data::content::ads_sway_settings& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_ads_sway_settings(const asset_data::content::ads_sway_settings& data);
		asset_data::content::advanced_hip_sway_settings read_advanced_hip_sway_settings(const AdvancedHipSwaySettings& native);
		void write_advanced_hip_sway_settings(AdvancedHipSwaySettings& native, const asset_data::content::advanced_hip_sway_settings& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_advanced_hip_sway_settings(const asset_data::content::advanced_hip_sway_settings& data);
		asset_data::content::att_dlc_overrides read_att_dlc_overrides(const AttDlcOverrides& native);
		void write_att_dlc_overrides(AttDlcOverrides& native, const asset_data::content::att_dlc_overrides& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_dlc_overrides(const asset_data::content::att_dlc_overrides& data);
		asset_data::content::att_dlc_scalars read_att_dlc_scalars(const AttDlcScalars& native);
		void write_att_dlc_scalars(AttDlcScalars& native, const asset_data::content::att_dlc_scalars& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_att_dlc_scalars(const asset_data::content::att_dlc_scalars& data);
		asset_data::content::animclass read_animclass(const AnimationClass& native);
		void write_animclass(AnimationClass& native, const asset_data::content::animclass& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_animclass(const asset_data::content::animclass& data);
		asset_data::content::animation_state_machine read_animation_state_machine(const AnimationStateMachine& native);
		void write_animation_state_machine(AnimationStateMachine& native, const asset_data::content::animation_state_machine& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_animation_state_machine(const asset_data::content::animation_state_machine& data);
		asset_data::content::animation_state read_animation_state(const AnimationState& native);
		void write_animation_state(AnimationState& native, const asset_data::content::animation_state& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_animation_state(const asset_data::content::animation_state& data);
		asset_data::content::animation_entry read_animation_entry(const AnimationEntry& native);
		void write_animation_entry(AnimationEntry& native, const asset_data::content::animation_entry& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_animation_entry(const asset_data::content::animation_entry& data);
		asset_data::content::anim_alias read_anim_alias(const AnimAlias& native);
		void write_anim_alias(AnimAlias& native, const asset_data::content::anim_alias& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_anim_alias(const asset_data::content::anim_alias& data);
		asset_data::content::anim_alias_info read_anim_alias_info(const AnimAliasInfo& native);
		void write_anim_alias_info(AnimAliasInfo& native, const asset_data::content::anim_alias_info& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_anim_alias_info(const asset_data::content::anim_alias_info& data);
		asset_data::content::animation_aim_set read_animation_aim_set(const AnimationAimSet& native);
		void write_animation_aim_set(AnimationAimSet& native, const asset_data::content::animation_aim_set& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_animation_aim_set(const asset_data::content::animation_aim_set& data);
		asset_data::content::luafile read_luafile(const LuaFile& native);
		void write_luafile(LuaFile& native, const asset_data::content::luafile& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_luafile(const asset_data::content::luafile& data);
		asset_data::content::leaderboarddef read_leaderboarddef(const LeaderboardDef& native);
		void write_leaderboarddef(LeaderboardDef& native, const asset_data::content::leaderboarddef& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_leaderboarddef(const asset_data::content::leaderboarddef& data);
		asset_data::content::lb_column_def read_lb_column_def(const LbColumnDef& native);
		void write_lb_column_def(LbColumnDef& native, const asset_data::content::lb_column_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_lb_column_def(const asset_data::content::lb_column_def& data);
		asset_data::content::virtualleaderboarddef read_virtualleaderboarddef(const VirtualLeaderboardDef& native);
		void write_virtualleaderboarddef(VirtualLeaderboardDef& native, const asset_data::content::virtualleaderboarddef& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_virtualleaderboarddef(const asset_data::content::virtualleaderboarddef& data);
		asset_data::content::lb_vr_column_def read_lb_vr_column_def(const LbVrColumnDef& native);
		void write_lb_vr_column_def(LbVrColumnDef& native, const asset_data::content::lb_vr_column_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_lb_vr_column_def(const asset_data::content::lb_vr_column_def& data);
		asset_data::content::clothasset read_clothasset(const ClothAsset& native);
		void write_clothasset(ClothAsset& native, const asset_data::content::clothasset& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_clothasset(const asset_data::content::clothasset& data);
		asset_data::content::cloth_asset_bone_mapping_set read_cloth_asset_bone_mapping_set(const ClothAssetBoneMappingSet& native);
		void write_cloth_asset_bone_mapping_set(ClothAssetBoneMappingSet& native, const asset_data::content::cloth_asset_bone_mapping_set& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_cloth_asset_bone_mapping_set(const asset_data::content::cloth_asset_bone_mapping_set& data);
		asset_data::content::cloth_asset_bone_mapping read_cloth_asset_bone_mapping(const ClothAssetBoneMapping& native);
		void write_cloth_asset_bone_mapping(ClothAssetBoneMapping& native, const asset_data::content::cloth_asset_bone_mapping& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_cloth_asset_bone_mapping(const asset_data::content::cloth_asset_bone_mapping& data);
		asset_data::content::cloth_asset_simulation_lod read_cloth_asset_simulation_lod(const ClothAssetSimulationLOD& native);
		void write_cloth_asset_simulation_lod(ClothAssetSimulationLOD& native, const asset_data::content::cloth_asset_simulation_lod& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_cloth_asset_simulation_lod(const asset_data::content::cloth_asset_simulation_lod& data);
		asset_data::content::cloth_asset_sound read_cloth_asset_sound(const ClothAssetSound& native);
		void write_cloth_asset_sound(ClothAssetSound& native, const asset_data::content::cloth_asset_sound& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_cloth_asset_sound(const asset_data::content::cloth_asset_sound& data);
		asset_data::content::stterrain read_stterrain(const StTerrain& native);
		void write_stterrain(StTerrain& native, const asset_data::content::stterrain& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_stterrain(const asset_data::content::stterrain& data);
		asset_data::content::st_disk_terrain_surface read_st_disk_terrain_surface(const StDiskTerrainSurface& native);
		void write_st_disk_terrain_surface(StDiskTerrainSurface& native, const asset_data::content::st_disk_terrain_surface& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_st_disk_terrain_surface(const asset_data::content::st_disk_terrain_surface& data);
		asset_data::content::gfx_index_buffer read_gfx_index_buffer(const GfxIndexBuffer& native);
		void write_gfx_index_buffer(GfxIndexBuffer& native, const asset_data::content::gfx_index_buffer& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_index_buffer(const asset_data::content::gfx_index_buffer& data);
		asset_data::content::st_terrain_grid read_st_terrain_grid(const StTerrainGrid& native);
		void write_st_terrain_grid(StTerrainGrid& native, const asset_data::content::st_terrain_grid& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_st_terrain_grid(const asset_data::content::st_terrain_grid& data);
		asset_data::content::st_terrain_grid_level read_st_terrain_grid_level(const StTerrainGridLevel& native);
		void write_st_terrain_grid_level(StTerrainGridLevel& native, const asset_data::content::st_terrain_grid_level& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_st_terrain_grid_level(const asset_data::content::st_terrain_grid_level& data);
		asset_data::content::gfx_placement read_gfx_placement(const GfxPlacement& native);
		void write_gfx_placement(GfxPlacement& native, const asset_data::content::gfx_placement& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_placement(const asset_data::content::gfx_placement& data);
		asset_data::content::bounds read_bounds(const Bounds& native);
		void write_bounds(Bounds& native, const asset_data::content::bounds& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_bounds(const asset_data::content::bounds& data);
		asset_data::content::st_terrain_material_layer read_st_terrain_material_layer(const StTerrainMaterialLayer& native);
		void write_st_terrain_material_layer(StTerrainMaterialLayer& native, const asset_data::content::st_terrain_material_layer& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_st_terrain_material_layer(const asset_data::content::st_terrain_material_layer& data);
		asset_data::content::hlsl_terrain_layer_data_code read_hlsl_terrain_layer_data_code(const HLSL_TerrainLayerDataCode& native);
		void write_hlsl_terrain_layer_data_code(HLSL_TerrainLayerDataCode& native, const asset_data::content::hlsl_terrain_layer_data_code& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_hlsl_terrain_layer_data_code(const asset_data::content::hlsl_terrain_layer_data_code& data);
		asset_data::content::st_terrain_displacement_scale read_st_terrain_displacement_scale(const StTerrainDisplacementScale& native);
		void write_st_terrain_displacement_scale(StTerrainDisplacementScale& native, const asset_data::content::st_terrain_displacement_scale& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_st_terrain_displacement_scale(const asset_data::content::st_terrain_displacement_scale& data);
		asset_data::content::st_terrain_clutter_layer read_st_terrain_clutter_layer(const StTerrainClutterLayer& native);
		void write_st_terrain_clutter_layer(StTerrainClutterLayer& native, const asset_data::content::st_terrain_clutter_layer& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_st_terrain_clutter_layer(const asset_data::content::st_terrain_clutter_layer& data);
		asset_data::content::gfx_wrapped_buffer read_gfx_wrapped_buffer(const GfxWrappedBuffer& native);
		void write_gfx_wrapped_buffer(GfxWrappedBuffer& native, const asset_data::content::gfx_wrapped_buffer& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_wrapped_buffer(const asset_data::content::gfx_wrapped_buffer& data);
		asset_data::content::gfx_wrapped_base_buffer read_gfx_wrapped_base_buffer(const GfxWrappedBaseBuffer& native);
		void write_gfx_wrapped_base_buffer(GfxWrappedBaseBuffer& native, const asset_data::content::gfx_wrapped_base_buffer& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_wrapped_base_buffer(const asset_data::content::gfx_wrapped_base_buffer& data);
		asset_data::content::gfx_buffer_resource read_gfx_buffer_resource(const GfxBufferResource& native);
		void write_gfx_buffer_resource(GfxBufferResource& native, const asset_data::content::gfx_buffer_resource& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_buffer_resource(const asset_data::content::gfx_buffer_resource& data);
		asset_data::content::gfx_shader_buffer_view read_gfx_shader_buffer_view(const GfxShaderBufferView& native);
		void write_gfx_shader_buffer_view(GfxShaderBufferView& native, const asset_data::content::gfx_shader_buffer_view& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_shader_buffer_view(const asset_data::content::gfx_shader_buffer_view& data);
		asset_data::content::st_clutter_sample_bitmask read_st_clutter_sample_bitmask(const StClutterSampleBitmask& native);
		void write_st_clutter_sample_bitmask(StClutterSampleBitmask& native, const asset_data::content::st_clutter_sample_bitmask& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_st_clutter_sample_bitmask(const asset_data::content::st_clutter_sample_bitmask& data);
		asset_data::content::st_terrain_maps read_st_terrain_maps(const StTerrainMaps& native);
		void write_st_terrain_maps(StTerrainMaps& native, const asset_data::content::st_terrain_maps& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_st_terrain_maps(const asset_data::content::st_terrain_maps& data);
		asset_data::content::st_clutter_sample_points read_st_clutter_sample_points(const StClutterSamplePoints& native);
		void write_st_clutter_sample_points(StClutterSamplePoints& native, const asset_data::content::st_clutter_sample_points& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_st_clutter_sample_points(const asset_data::content::st_clutter_sample_points& data);
		asset_data::content::st_lightmap read_st_lightmap(const StLightmap& native);
		void write_st_lightmap(StLightmap& native, const asset_data::content::st_lightmap& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_st_lightmap(const asset_data::content::st_lightmap& data);
		asset_data::content::collisiontile read_collisiontile(const CollisionTile& native);
		void write_collisiontile(CollisionTile& native, const asset_data::content::collisiontile& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_collisiontile(const asset_data::content::collisiontile& data);
		asset_data::content::static_model_collision_compressed_model_list read_static_model_collision_compressed_model_list(const StaticModelCollisionCompressedModelList& native);
		void write_static_model_collision_compressed_model_list(StaticModelCollisionCompressedModelList& native, const asset_data::content::static_model_collision_compressed_model_list& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_static_model_collision_compressed_model_list(const asset_data::content::static_model_collision_compressed_model_list& data);
		asset_data::content::static_model_collision_compressed_model read_static_model_collision_compressed_model(const StaticModelCollisionCompressedModel& native);
		void write_static_model_collision_compressed_model(StaticModelCollisionCompressedModel& native, const asset_data::content::static_model_collision_compressed_model& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_static_model_collision_compressed_model(const asset_data::content::static_model_collision_compressed_model& data);
		asset_data::content::static_model_collision_compressed_instance read_static_model_collision_compressed_instance(const StaticModelCollisionCompressedInstance& native);
		void write_static_model_collision_compressed_instance(StaticModelCollisionCompressedInstance& native, const asset_data::content::static_model_collision_compressed_instance& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_static_model_collision_compressed_instance(const asset_data::content::static_model_collision_compressed_instance& data);
		asset_data::content::vehicle read_vehicle(const VehicleDef& native);
		void write_vehicle(VehicleDef& native, const asset_data::content::vehicle& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_vehicle(const asset_data::content::vehicle& data);
		asset_data::content::vehicle_physics_def read_vehicle_physics_def(const VehiclePhysicsDef& native);
		void write_vehicle_physics_def(VehiclePhysicsDef& native, const asset_data::content::vehicle_physics_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_vehicle_physics_def(const asset_data::content::vehicle_physics_def& data);
		asset_data::content::vehicle_physics_sound_group read_vehicle_physics_sound_group(const VehiclePhysicsSoundGroup& native);
		void write_vehicle_physics_sound_group(VehiclePhysicsSoundGroup& native, const asset_data::content::vehicle_physics_sound_group& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_vehicle_physics_sound_group(const asset_data::content::vehicle_physics_sound_group& data);
		asset_data::content::vehicle_physics_vfx_group read_vehicle_physics_vfx_group(const VehiclePhysicsVfxGroup& native);
		void write_vehicle_physics_vfx_group(VehiclePhysicsVfxGroup& native, const asset_data::content::vehicle_physics_vfx_group& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_vehicle_physics_vfx_group(const asset_data::content::vehicle_physics_vfx_group& data);
		asset_data::content::spaceship_anim_parts read_spaceship_anim_parts(const SpaceshipAnimParts& native);
		void write_spaceship_anim_parts(SpaceshipAnimParts& native, const asset_data::content::spaceship_anim_parts& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_spaceship_anim_parts(const asset_data::content::spaceship_anim_parts& data);
		asset_data::content::vehicle_physics_rev_modifier read_vehicle_physics_rev_modifier(const VehiclePhysicsRevModifier& native);
		void write_vehicle_physics_rev_modifier(VehiclePhysicsRevModifier& native, const asset_data::content::vehicle_physics_rev_modifier& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_vehicle_physics_rev_modifier(const asset_data::content::vehicle_physics_rev_modifier& data);
		asset_data::content::xcam read_xcam(const XCam& native);
		void write_xcam(XCam& native, const asset_data::content::xcam& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_xcam(const asset_data::content::xcam& data);
		asset_data::content::x_cam_target_model read_x_cam_target_model(const XCamTargetModel& native);
		void write_x_cam_target_model(XCamTargetModel& native, const asset_data::content::x_cam_target_model& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_x_cam_target_model(const asset_data::content::x_cam_target_model& data);
		asset_data::content::x_cam_target_model_frame read_x_cam_target_model_frame(const XCamTargetModelFrame& native);
		void write_x_cam_target_model_frame(XCamTargetModelFrame& native, const asset_data::content::x_cam_target_model_frame& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_x_cam_target_model_frame(const asset_data::content::x_cam_target_model_frame& data);
		asset_data::content::x_cam_camera read_x_cam_camera(const XCamCamera& native);
		void write_x_cam_camera(XCamCamera& native, const asset_data::content::x_cam_camera& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_x_cam_camera(const asset_data::content::x_cam_camera& data);
		asset_data::content::x_cam_frame read_x_cam_frame(const XCamFrame& native);
		void write_x_cam_frame(XCamFrame& native, const asset_data::content::x_cam_frame& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_x_cam_frame(const asset_data::content::x_cam_frame& data);
		asset_data::content::x_cam_camera_switch_params read_x_cam_camera_switch_params(const XCamCameraSwitchParams& native);
		void write_x_cam_camera_switch_params(XCamCameraSwitchParams& native, const asset_data::content::x_cam_camera_switch_params& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_x_cam_camera_switch_params(const asset_data::content::x_cam_camera_switch_params& data);
		asset_data::content::x_cam_frame_name read_x_cam_frame_name(const XCamFrameName& native);
		void write_x_cam_frame_name(XCamFrameName& native, const asset_data::content::x_cam_frame_name& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_x_cam_frame_name(const asset_data::content::x_cam_frame_name& data);
		asset_data::content::x_anim_notify_info read_x_anim_notify_info(const XAnimNotifyInfo& native);
		void write_x_anim_notify_info(XAnimNotifyInfo& native, const asset_data::content::x_anim_notify_info& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_x_anim_notify_info(const asset_data::content::x_anim_notify_info& data);
		asset_data::content::dynentitylist read_dynentitylist(const DynEntityList& native);
		void write_dynentitylist(DynEntityList& native, const asset_data::content::dynentitylist& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_dynentitylist(const asset_data::content::dynentitylist& data);
		asset_data::content::dyn_entity_def read_dyn_entity_def(const DynEntityDef& native);
		void write_dyn_entity_def(DynEntityDef& native, const asset_data::content::dyn_entity_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_dyn_entity_def(const asset_data::content::dyn_entity_def& data);
		asset_data::content::client_entity_link_to_def read_client_entity_link_to_def(const ClientEntityLinkToDef& native);
		void write_client_entity_link_to_def(ClientEntityLinkToDef& native, const asset_data::content::client_entity_link_to_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_client_entity_link_to_def(const asset_data::content::client_entity_link_to_def& data);
		asset_data::content::spatial_partition_population_node read_spatial_partition_population_node(const SpatialPartition_Population_Node& native);
		void write_spatial_partition_population_node(SpatialPartition_Population_Node& native, const asset_data::content::spatial_partition_population_node& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_spatial_partition_population_node(const asset_data::content::spatial_partition_population_node& data);
		asset_data::content::spatial_partition_transient_population read_spatial_partition_transient_population(const SpatialPartition_TransientPopulation& native);
		void write_spatial_partition_transient_population(SpatialPartition_TransientPopulation& native, const asset_data::content::spatial_partition_transient_population& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_spatial_partition_transient_population(const asset_data::content::spatial_partition_transient_population& data);

		asset_data::content::att_ammo_general read_att_ammo_general(const AttAmmoGeneral& native)
		{
			plain::trace("read_att_ammo_general");
			asset_data::content::att_ammo_general data;
			data.fire_type = plain::enum_name(asset_data::content::weap_fire_type_t_names, static_cast<std::int64_t>(native.fireType), "AttAmmoGeneral.fireType");
			data.burst_count = native.burstCount;
			return data;
		}

		void write_att_ammo_general(AttAmmoGeneral& native, const asset_data::content::att_ammo_general& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.fireType = static_cast<weapFireType_t>(plain::enum_value(asset_data::content::weap_fire_type_t_names, data.fire_type, "AttAmmoGeneral.fireType"));
			native.burstCount = data.burst_count;
		}

		std::size_t size_att_ammo_general([[maybe_unused]] const asset_data::content::att_ammo_general& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::att_ammo_type read_att_ammo_type(const AttAmmoType& native)
		{
			plain::trace("read_att_ammo_type");
			asset_data::content::att_ammo_type data;
			data.sz_ammo_name = plain::read_string(native.szAmmoName);
			return data;
		}

		void write_att_ammo_type(AttAmmoType& native, const asset_data::content::att_ammo_type& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.szAmmoName = plain::write_string(memory, data.sz_ammo_name);
		}

		std::size_t size_att_ammo_type([[maybe_unused]] const asset_data::content::att_ammo_type& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.sz_ammo_name);
			return size;
		}

		asset_data::content::att_impact read_att_impact(const AttImpact& native)
		{
			plain::trace("read_att_impact");
			asset_data::content::att_impact data;
			data.vfx_impact_type = native.vfxImpactType;
			return data;
		}

		void write_att_impact(AttImpact& native, const asset_data::content::att_impact& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.vfxImpactType = data.vfx_impact_type;
		}

		std::size_t size_att_impact([[maybe_unused]] const asset_data::content::att_impact& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::att_ammo_tracer read_att_ammo_tracer(const AttAmmoTracer& native)
		{
			plain::trace("read_att_ammo_tracer");
			asset_data::content::att_ammo_tracer data;
			data.tracer_type = plain::read_asset(ASSET_TYPE_TRACER, native.tracerType);
			data.tracer_style = plain::enum_name(asset_data::content::tracer_style_t_names, static_cast<std::int64_t>(native.tracerStyle), "AttAmmoTracer.tracerStyle");
			return data;
		}

		void write_att_ammo_tracer(AttAmmoTracer& native, const asset_data::content::att_ammo_tracer& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.tracerType = plain::resolve_asset<TracerDef>(resolve, ASSET_TYPE_TRACER, data.tracer_type);
			native.tracerStyle = static_cast<tracerStyle_t>(plain::enum_value(asset_data::content::tracer_style_t_names, data.tracer_style, "AttAmmoTracer.tracerStyle"));
		}

		std::size_t size_att_ammo_tracer([[maybe_unused]] const asset_data::content::att_ammo_tracer& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::att_penetration read_att_penetration(const AttPenetration& native)
		{
			plain::trace("read_att_penetration");
			asset_data::content::att_penetration data;
			data.penetrate_type = plain::enum_name(asset_data::content::penetrate_type_names, static_cast<std::int64_t>(native.penetrateType), "AttPenetration.penetrateType");
			data.penetrate_multiplier = native.penetrateMultiplier;
			data.rifle_bullet = native.rifleBullet;
			data.armor_piercing = native.armorPiercing;
			data.impaling = native.impaling;
			return data;
		}

		void write_att_penetration(AttPenetration& native, const asset_data::content::att_penetration& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.penetrateType = static_cast<PenetrateType>(plain::enum_value(asset_data::content::penetrate_type_names, data.penetrate_type, "AttPenetration.penetrateType"));
			native.penetrateMultiplier = data.penetrate_multiplier;
			native.rifleBullet = data.rifle_bullet;
			native.armorPiercing = data.armor_piercing;
			native.impaling = data.impaling;
		}

		std::size_t size_att_penetration([[maybe_unused]] const asset_data::content::att_penetration& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::att_sight read_att_sight(const AttSight& native)
		{
			plain::trace("read_att_sight");
			asset_data::content::att_sight data;
			data.blur_scene_ads_in_fraction = native.blurSceneAdsInFraction;
			data.blur_scene_ads_out_fraction = native.blurSceneAdsOutFraction;
			data.ads_smooth_fade = native.adsSmoothFade;
			data.ads_smooth_fade_in_begin = native.adsSmoothFadeInBegin;
			data.ads_smooth_fade_in_end = native.adsSmoothFadeInEnd;
			data.ads_smooth_fade_out_begin = native.adsSmoothFadeOutBegin;
			data.ads_smooth_fade_out_end = native.adsSmoothFadeOutEnd;
			data.aim_down_sight = native.aimDownSight;
			data.ads_fire = native.adsFire;
			data.rechamber_while_ads = native.rechamberWhileAds;
			data.no_ads_when_mag_empty = native.noAdsWhenMagEmpty;
			data.hide_rail_with_this_scope = native.hideRailWithThisScope;
			data.force_iron_sight_down = native.forceIronSightDown;
			data.use_scope_drift = native.useScopeDrift;
			data.use_dual_fov = native.useDualFOV;
			data.show_view_model_in_dual_fov = native.showViewModelInDualFOV;
			data.synced_fov_in_dual_fov = native.syncedFOVInDualFOV;
			data.block_nvgads = native.blockNVGADS;
			data.scope_lens_inner_disk = native.scopeLensInnerDisk;
			data.scope_lens_inner_disk_mag = native.scopeLensInnerDiskMag;
			data.scope_lens_outer_ring = native.scopeLensOuterRing;
			data.scope_lens_outer_ring_mag = native.scopeLensOuterRingMag;
			data.scope_lens_radius = native.scopeLensRadius;
			data.scope_lens_color_red = native.scopeLensColorRed;
			data.scope_lens_color_green = native.scopeLensColorGreen;
			data.scope_lens_color_blue = native.scopeLensColorBlue;
			data.scope_lens_brightness = native.scopeLensBrightness;
			data.scope_lens_fade_start = native.scopeLensFadeStart;
			data.scope_lens_fade_end = native.scopeLensFadeEnd;
			data.scope_eye_relief_focus_distance = native.scopeEyeRelief_focusDistance;
			data.scope_eye_relief_focuse_uv_scale = native.scopeEyeRelief_focuseUVScale;
			data.scope_eye_relief_out_of_focus_distance = native.scopeEyeRelief_outOfFocusDistance;
			data.scope_eye_relief_out_of_focuse_uv_scale = native.scopeEyeRelief_outOfFocuseUVScale;
			data.scope_eye_relief_idle_movement_scale = native.scopeEyeRelief_idleMovementScale;
			data.scope_eye_relief_full_speed_movement_scale = native.scopeEyeRelief_fullSpeedMovementScale;
			data.scope_eye_relief_max_movement = native.scopeEyeRelief_maxMovement;
			data.scope_eye_relief_hip_movement_scale = native.scopeEyeRelief_hipMovementScale;
			data.scope_eye_relief_hip_max_movement = native.scopeEyeRelief_hipMaxMovement;
			data.scope_eye_relief_idle_sway_freq = native.scopeEyeRelief_idleSway_freq;
			data.scope_eye_relief_idle_sway_movement = native.scopeEyeRelief_idleSway_movement;
			data.scope_eye_relief_full_speed_sway_freq = native.scopeEyeRelief_fullSpeedSway_freq;
			data.scope_eye_relief_full_speed_sway_movement = native.scopeEyeRelief_fullSpeedSway_movement;
			return data;
		}

		void write_att_sight(AttSight& native, const asset_data::content::att_sight& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.blurSceneAdsInFraction = data.blur_scene_ads_in_fraction;
			native.blurSceneAdsOutFraction = data.blur_scene_ads_out_fraction;
			native.adsSmoothFade = data.ads_smooth_fade;
			native.adsSmoothFadeInBegin = data.ads_smooth_fade_in_begin;
			native.adsSmoothFadeInEnd = data.ads_smooth_fade_in_end;
			native.adsSmoothFadeOutBegin = data.ads_smooth_fade_out_begin;
			native.adsSmoothFadeOutEnd = data.ads_smooth_fade_out_end;
			native.aimDownSight = data.aim_down_sight;
			native.adsFire = data.ads_fire;
			native.rechamberWhileAds = data.rechamber_while_ads;
			native.noAdsWhenMagEmpty = data.no_ads_when_mag_empty;
			native.hideRailWithThisScope = data.hide_rail_with_this_scope;
			native.forceIronSightDown = data.force_iron_sight_down;
			native.useScopeDrift = data.use_scope_drift;
			native.useDualFOV = data.use_dual_fov;
			native.showViewModelInDualFOV = data.show_view_model_in_dual_fov;
			native.syncedFOVInDualFOV = data.synced_fov_in_dual_fov;
			native.blockNVGADS = data.block_nvgads;
			native.scopeLensInnerDisk = data.scope_lens_inner_disk;
			native.scopeLensInnerDiskMag = data.scope_lens_inner_disk_mag;
			native.scopeLensOuterRing = data.scope_lens_outer_ring;
			native.scopeLensOuterRingMag = data.scope_lens_outer_ring_mag;
			native.scopeLensRadius = data.scope_lens_radius;
			native.scopeLensColorRed = data.scope_lens_color_red;
			native.scopeLensColorGreen = data.scope_lens_color_green;
			native.scopeLensColorBlue = data.scope_lens_color_blue;
			native.scopeLensBrightness = data.scope_lens_brightness;
			native.scopeLensFadeStart = data.scope_lens_fade_start;
			native.scopeLensFadeEnd = data.scope_lens_fade_end;
			native.scopeEyeRelief_focusDistance = data.scope_eye_relief_focus_distance;
			native.scopeEyeRelief_focuseUVScale = data.scope_eye_relief_focuse_uv_scale;
			native.scopeEyeRelief_outOfFocusDistance = data.scope_eye_relief_out_of_focus_distance;
			native.scopeEyeRelief_outOfFocuseUVScale = data.scope_eye_relief_out_of_focuse_uv_scale;
			native.scopeEyeRelief_idleMovementScale = data.scope_eye_relief_idle_movement_scale;
			native.scopeEyeRelief_fullSpeedMovementScale = data.scope_eye_relief_full_speed_movement_scale;
			native.scopeEyeRelief_maxMovement = data.scope_eye_relief_max_movement;
			native.scopeEyeRelief_hipMovementScale = data.scope_eye_relief_hip_movement_scale;
			native.scopeEyeRelief_hipMaxMovement = data.scope_eye_relief_hip_max_movement;
			native.scopeEyeRelief_idleSway_freq = data.scope_eye_relief_idle_sway_freq;
			native.scopeEyeRelief_idleSway_movement = data.scope_eye_relief_idle_sway_movement;
			native.scopeEyeRelief_fullSpeedSway_freq = data.scope_eye_relief_full_speed_sway_freq;
			native.scopeEyeRelief_fullSpeedSway_movement = data.scope_eye_relief_full_speed_sway_movement;
		}

		std::size_t size_att_sight([[maybe_unused]] const asset_data::content::att_sight& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::att_hold_breath read_att_hold_breath(const AttHoldBreath& native)
		{
			plain::trace("read_att_hold_breath");
			asset_data::content::att_hold_breath data;
			data.can_hold_breath = native.canHoldBreath;
			return data;
		}

		void write_att_hold_breath(AttHoldBreath& native, const asset_data::content::att_hold_breath& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.canHoldBreath = data.can_hold_breath;
		}

		std::size_t size_att_hold_breath([[maybe_unused]] const asset_data::content::att_hold_breath& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::att_reload read_att_reload(const AttReload& native)
		{
			plain::trace("read_att_reload");
			asset_data::content::att_reload data;
			data.no_partial_reload = native.noPartialReload;
			data.segmented_reload = native.segmentedReload;
			data.reload_disabled = native.reloadDisabled;
			return data;
		}

		void write_att_reload(AttReload& native, const asset_data::content::att_reload& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.noPartialReload = data.no_partial_reload;
			native.segmentedReload = data.segmented_reload;
			native.reloadDisabled = data.reload_disabled;
		}

		std::size_t size_att_reload([[maybe_unused]] const asset_data::content::att_reload& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::att_add_ons read_att_add_ons(const AttAddOns& native)
		{
			plain::trace("read_att_add_ons");
			asset_data::content::att_add_ons data;
			data.silenced = native.silenced;
			data.riot_shield = native.riotShield;
			return data;
		}

		void write_att_add_ons(AttAddOns& native, const asset_data::content::att_add_ons& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.silenced = data.silenced;
			native.riotShield = data.riot_shield;
		}

		std::size_t size_att_add_ons([[maybe_unused]] const asset_data::content::att_add_ons& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::laser_settings read_laser_settings(const LaserSettings& native)
		{
			plain::trace("read_laser_settings");
			asset_data::content::laser_settings data;
			data.force_laser_on = native.forceLaserOn;
			data.local_player_ads_laser_enabled = native.localPlayerADSLaserEnabled;
			data.local_player_hip_laser_enabled = native.localPlayerHipLaserEnabled;
			data.local_player_nvgads_laser_enabled = native.localPlayerNVGADSLaserEnabled;
			data.local_player_nvg_hip_laser_enabled = native.localPlayerNVGHipLaserEnabled;
			data.remote_player_ads_laser_enabled = native.remotePlayerADSLaserEnabled;
			data.remote_player_hip_laser_enabled = native.remotePlayerHipLaserEnabled;
			data.remote_player_nvgads_laser_enabled = native.remotePlayerNVGADSLaserEnabled;
			data.remote_player_nvg_hip_laser_enabled = native.remotePlayerNVGHipLaserEnabled;
			data.laser_view_center_in_ads = native.laserViewCenterInAds;
			return data;
		}

		void write_laser_settings(LaserSettings& native, const asset_data::content::laser_settings& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.forceLaserOn = data.force_laser_on;
			native.localPlayerADSLaserEnabled = data.local_player_ads_laser_enabled;
			native.localPlayerHipLaserEnabled = data.local_player_hip_laser_enabled;
			native.localPlayerNVGADSLaserEnabled = data.local_player_nvgads_laser_enabled;
			native.localPlayerNVGHipLaserEnabled = data.local_player_nvg_hip_laser_enabled;
			native.remotePlayerADSLaserEnabled = data.remote_player_ads_laser_enabled;
			native.remotePlayerHipLaserEnabled = data.remote_player_hip_laser_enabled;
			native.remotePlayerNVGADSLaserEnabled = data.remote_player_nvgads_laser_enabled;
			native.remotePlayerNVGHipLaserEnabled = data.remote_player_nvg_hip_laser_enabled;
			native.laserViewCenterInAds = data.laser_view_center_in_ads;
		}

		std::size_t size_laser_settings([[maybe_unused]] const asset_data::content::laser_settings& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::att_laser read_att_laser(const AttLaser& native)
		{
			plain::trace("read_att_laser");
			asset_data::content::att_laser data;
			data.laser_type_view_model = plain::read_asset(ASSET_TYPE_LASER, native.laserTypeViewModel);
			data.laser_type_view_model_alt = plain::read_asset(ASSET_TYPE_LASER, native.laserTypeViewModelAlt);
			data.laser_type_friendly = plain::read_asset(ASSET_TYPE_LASER, native.laserTypeFriendly);
			data.laser_type_enemy = plain::read_asset(ASSET_TYPE_LASER, native.laserTypeEnemy);
			data.laser_settings = read_laser_settings(native.laserSettings);
			return data;
		}

		void write_att_laser(AttLaser& native, const asset_data::content::att_laser& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.laserTypeViewModel = plain::resolve_asset<LaserDef>(resolve, ASSET_TYPE_LASER, data.laser_type_view_model);
			native.laserTypeViewModelAlt = plain::resolve_asset<LaserDef>(resolve, ASSET_TYPE_LASER, data.laser_type_view_model_alt);
			native.laserTypeFriendly = plain::resolve_asset<LaserDef>(resolve, ASSET_TYPE_LASER, data.laser_type_friendly);
			native.laserTypeEnemy = plain::resolve_asset<LaserDef>(resolve, ASSET_TYPE_LASER, data.laser_type_enemy);
			write_laser_settings(native.laserSettings, data.laser_settings, resolve, memory);
		}

		std::size_t size_att_laser([[maybe_unused]] const asset_data::content::att_laser& data)
		{
			std::size_t size = 0;
			size += size_laser_settings(data.laser_settings);
			return size;
		}

		asset_data::content::att_post read_att_post(const AttPost& native)
		{
			plain::trace("read_att_post");
			asset_data::content::att_post data;
			data.ads_fire_motion_blur = native.adsFireMotionBlur;
			data.ads_fire_motion_blur_duration_ms = native.adsFireMotionBlurDurationMs;
			data.ads_fire_motion_blur_decay_exponent = native.adsFireMotionBlurDecayExponent;
			data.hip_fire_motion_blur = native.hipFireMotionBlur;
			data.hip_fire_motion_blur_duration_ms = native.hipFireMotionBlurDurationMs;
			data.hip_fire_motion_blur_decay_exponent = native.hipFireMotionBlurDecayExponent;
			data.dof_view_model_physical_fstop = native.dofViewModelPhysicalFstop;
			data.dof_view_model_physical_focus_distance = native.dofViewModelPhysicalFocusDistance;
			data.dof_physical_fstop = native.dofPhysicalFstop;
			data.dof_physical_focus_distance = native.dofPhysicalFocusDistance;
			data.ads_camera_shake_rotation_scale = native.adsCameraShakeRotationScale;
			data.ads_camera_shake_translation_scale = native.adsCameraShakeTranslationScale;
			data.dof_view_model_physical_focal_tag = native.dofViewModelPhysicalFocalTag;
			data.dof_scene_is_overridden = native.dofSceneIsOverridden;
			return data;
		}

		void write_att_post(AttPost& native, const asset_data::content::att_post& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.adsFireMotionBlur = data.ads_fire_motion_blur;
			native.adsFireMotionBlurDurationMs = data.ads_fire_motion_blur_duration_ms;
			native.adsFireMotionBlurDecayExponent = data.ads_fire_motion_blur_decay_exponent;
			native.hipFireMotionBlur = data.hip_fire_motion_blur;
			native.hipFireMotionBlurDurationMs = data.hip_fire_motion_blur_duration_ms;
			native.hipFireMotionBlurDecayExponent = data.hip_fire_motion_blur_decay_exponent;
			native.dofViewModelPhysicalFstop = data.dof_view_model_physical_fstop;
			native.dofViewModelPhysicalFocusDistance = data.dof_view_model_physical_focus_distance;
			native.dofPhysicalFstop = data.dof_physical_fstop;
			native.dofPhysicalFocusDistance = data.dof_physical_focus_distance;
			native.adsCameraShakeRotationScale = data.ads_camera_shake_rotation_scale;
			native.adsCameraShakeTranslationScale = data.ads_camera_shake_translation_scale;
			native.dofViewModelPhysicalFocalTag = data.dof_view_model_physical_focal_tag;
			native.dofSceneIsOverridden = data.dof_scene_is_overridden;
		}

		std::size_t size_att_post([[maybe_unused]] const asset_data::content::att_post& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::att_general read_att_general(const AttGeneral& native)
		{
			plain::trace("read_att_general");
			asset_data::content::att_general data;
			data.bolt_action = native.boltAction;
			data.inherits_perks = native.inheritsPerks;
			data.reticle_spin45 = native.reticleSpin45;
			data.ads_should_show_crosshair = native.adsShouldShowCrosshair;
			data.enemy_crosshair_range = native.enemyCrosshairRange;
			data.reticle_type = plain::enum_name(asset_data::content::reticle_type_names, static_cast<std::int64_t>(native.reticleType), "AttGeneral.reticleType");
			data.reticle_center = plain::read_asset(ASSET_TYPE_MATERIAL, native.reticleCenter);
			data.reticle_side = plain::read_asset(ASSET_TYPE_MATERIAL, native.reticleSide);
			data.reticle_one_piece = plain::read_asset(ASSET_TYPE_MATERIAL, native.reticleOnePiece);
			data.reticle_center_size = native.reticleCenterSize;
			data.reticle_side_size = native.reticleSideSize;
			data.sz_lui_crosshair_widget = plain::read_string(native.szLUICrosshairWidget);
			return data;
		}

		void write_att_general(AttGeneral& native, const asset_data::content::att_general& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.boltAction = data.bolt_action;
			native.inheritsPerks = data.inherits_perks;
			native.reticleSpin45 = data.reticle_spin45;
			native.adsShouldShowCrosshair = data.ads_should_show_crosshair;
			native.enemyCrosshairRange = data.enemy_crosshair_range;
			native.reticleType = static_cast<ReticleType>(plain::enum_value(asset_data::content::reticle_type_names, data.reticle_type, "AttGeneral.reticleType"));
			native.reticleCenter = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, data.reticle_center);
			native.reticleSide = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, data.reticle_side);
			native.reticleOnePiece = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, data.reticle_one_piece);
			native.reticleCenterSize = data.reticle_center_size;
			native.reticleSideSize = data.reticle_side_size;
			native.szLUICrosshairWidget = plain::write_string(memory, data.sz_lui_crosshair_widget);
		}

		std::size_t size_att_general([[maybe_unused]] const asset_data::content::att_general& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.sz_lui_crosshair_widget);
			return size;
		}

		asset_data::content::att_aim_assist read_att_aim_assist(const AttAimAssist& native)
		{
			plain::trace("read_att_aim_assist");
			asset_data::content::att_aim_assist data;
			data.auto_aim_range = native.autoAimRange;
			data.aim_assist_range = native.aimAssistRange;
			data.aim_assist_range_ads = native.aimAssistRangeAds;
			data.aim_assist_pitch_slowdown = native.aimAssistPitchSlowdown;
			data.aim_assist_pitch_slowdown_ads = native.aimAssistPitchSlowdownAds;
			data.aim_assist_yaw_slowdown = native.aimAssistYawSlowdown;
			data.aim_assist_yaw_slowdown_ads = native.aimAssistYawSlowdownAds;
			data.aim_assist_lockon_strength = native.aimAssistLockonStrength;
			return data;
		}

		void write_att_aim_assist(AttAimAssist& native, const asset_data::content::att_aim_assist& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.autoAimRange = data.auto_aim_range;
			native.aimAssistRange = data.aim_assist_range;
			native.aimAssistRangeAds = data.aim_assist_range_ads;
			native.aimAssistPitchSlowdown = data.aim_assist_pitch_slowdown;
			native.aimAssistPitchSlowdownAds = data.aim_assist_pitch_slowdown_ads;
			native.aimAssistYawSlowdown = data.aim_assist_yaw_slowdown;
			native.aimAssistYawSlowdownAds = data.aim_assist_yaw_slowdown_ads;
			native.aimAssistLockonStrength = data.aim_assist_lockon_strength;
		}

		std::size_t size_att_aim_assist([[maybe_unused]] const asset_data::content::att_aim_assist& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::att_target_assist read_att_target_assist(const AttTargetAssist& native)
		{
			plain::trace("read_att_target_assist");
			asset_data::content::att_target_assist data;
			data.target_assist_type = plain::enum_name(asset_data::content::target_assist_type_t_names, static_cast<std::int64_t>(native.targetAssistType), "AttTargetAssist.targetAssistType");
			data.target_assist_behavior = plain::enum_name(asset_data::content::target_assist_behavior_t_names, static_cast<std::int64_t>(native.targetAssistBehavior), "AttTargetAssist.targetAssistBehavior");
			data.target_assist_range = native.targetAssistRange;
			data.target_assist_angle = native.targetAssistAngle;
			data.target_assist_los_offset_forward = native.targetAssistLosOffsetForward;
			data.target_assist_los_offset_right = native.targetAssistLosOffsetRight;
			data.target_assist_los_offset_up = native.targetAssistLosOffsetUp;
			data.target_assist_only_available_in_ads = native.targetAssistOnlyAvailableInAds;
			return data;
		}

		void write_att_target_assist(AttTargetAssist& native, const asset_data::content::att_target_assist& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.targetAssistType = static_cast<targetAssistType_t>(plain::enum_value(asset_data::content::target_assist_type_t_names, data.target_assist_type, "AttTargetAssist.targetAssistType"));
			native.targetAssistBehavior = static_cast<targetAssistBehavior_t>(plain::enum_value(asset_data::content::target_assist_behavior_t_names, data.target_assist_behavior, "AttTargetAssist.targetAssistBehavior"));
			native.targetAssistRange = data.target_assist_range;
			native.targetAssistAngle = data.target_assist_angle;
			native.targetAssistLosOffsetForward = data.target_assist_los_offset_forward;
			native.targetAssistLosOffsetRight = data.target_assist_los_offset_right;
			native.targetAssistLosOffsetUp = data.target_assist_los_offset_up;
			native.targetAssistOnlyAvailableInAds = data.target_assist_only_available_in_ads;
		}

		std::size_t size_att_target_assist([[maybe_unused]] const asset_data::content::att_target_assist& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::att_ammunition read_att_ammunition(const AttAmmunition& native)
		{
			plain::trace("read_att_ammunition");
			asset_data::content::att_ammunition data;
			data.max_ammo = native.maxAmmo;
			data.start_ammo = native.startAmmo;
			data.clip_size = native.clipSize;
			data.shot_count = native.shotCount;
			data.reload_ammo_add = native.reloadAmmoAdd;
			data.reload_start_add = native.reloadStartAdd;
			data.low_ammo_warning_count = native.lowAmmoWarningCount;
			data.require_ammo_used_per_shot = native.requireAmmoUsedPerShot;
			return data;
		}

		void write_att_ammunition(AttAmmunition& native, const asset_data::content::att_ammunition& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.maxAmmo = data.max_ammo;
			native.startAmmo = data.start_ammo;
			native.clipSize = data.clip_size;
			native.shotCount = data.shot_count;
			native.reloadAmmoAdd = data.reload_ammo_add;
			native.reloadStartAdd = data.reload_start_add;
			native.lowAmmoWarningCount = data.low_ammo_warning_count;
			native.requireAmmoUsedPerShot = data.require_ammo_used_per_shot;
		}

		std::size_t size_att_ammunition([[maybe_unused]] const asset_data::content::att_ammunition& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::weapon_damage_common read_weapon_damage_common(const WeaponDamageCommon& native)
		{
			plain::trace("read_weapon_damage_common");
			asset_data::content::weapon_damage_common data;
			data.min_damage = native.minDamage;
			data.mid1_damage = native.mid1Damage;
			data.mid2_damage = native.mid2Damage;
			data.mid3_damage = native.mid3Damage;
			data.damage = native.damage;
			data.max_damage_range = native.maxDamageRange;
			data.mid1_damage_range = native.mid1DamageRange;
			data.mid2_damage_range = native.mid2DamageRange;
			data.mid3_damage_range = native.mid3DamageRange;
			data.min_damage_range = native.minDamageRange;
			return data;
		}

		void write_weapon_damage_common(WeaponDamageCommon& native, const asset_data::content::weapon_damage_common& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.minDamage = data.min_damage;
			native.mid1Damage = data.mid1_damage;
			native.mid2Damage = data.mid2_damage;
			native.mid3Damage = data.mid3_damage;
			native.damage = data.damage;
			native.maxDamageRange = data.max_damage_range;
			native.mid1DamageRange = data.mid1_damage_range;
			native.mid2DamageRange = data.mid2_damage_range;
			native.mid3DamageRange = data.mid3_damage_range;
			native.minDamageRange = data.min_damage_range;
		}

		std::size_t size_weapon_damage_common([[maybe_unused]] const asset_data::content::weapon_damage_common& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::weapon_damage_info read_weapon_damage_info(const WeaponDamageInfo& native)
		{
			plain::trace("read_weapon_damage_info");
			asset_data::content::weapon_damage_info data;
			data.damage_data = [&] { std::array<asset_data::content::weapon_damage_common, 3> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_weapon_damage_common(native.damageData[i]); } return out; }();
			data.death_anim_damage_type = plain::enum_name(asset_data::content::anim_damage_type_names, static_cast<std::int64_t>(native.deathAnimDamageType), "WeaponDamageInfo.deathAnimDamageType");
			data.terminate_at_range = native.terminateAtRange;
			return data;
		}

		void write_weapon_damage_info(WeaponDamageInfo& native, const asset_data::content::weapon_damage_info& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			for (std::size_t i = 0; i < data.damage_data.size(); ++i)
			{
				write_weapon_damage_common(native.damageData[i], data.damage_data[i], resolve, memory);
			}
			native.deathAnimDamageType = static_cast<AnimDamageType>(plain::enum_value(asset_data::content::anim_damage_type_names, data.death_anim_damage_type, "WeaponDamageInfo.deathAnimDamageType"));
			native.terminateAtRange = data.terminate_at_range;
		}

		std::size_t size_weapon_damage_info([[maybe_unused]] const asset_data::content::weapon_damage_info& data)
		{
			std::size_t size = 0;
			for (const auto& item : data.damage_data)
			{
				size += size_weapon_damage_common(item);
			}
			return size;
		}

		asset_data::content::att_damage read_att_damage(const AttDamage& native)
		{
			plain::trace("read_att_damage");
			asset_data::content::att_damage data;
			data.damage_info = read_weapon_damage_info(native.damageInfo);
			data.death_anim_damage_type = plain::enum_name(asset_data::content::anim_damage_type_names, static_cast<std::int64_t>(native.deathAnimDamageType), "AttDamage.deathAnimDamageType");
			data.terminate_at_range = native.terminateAtRange;
			return data;
		}

		void write_att_damage(AttDamage& native, const asset_data::content::att_damage& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_weapon_damage_info(native.damageInfo, data.damage_info, resolve, memory);
			native.deathAnimDamageType = static_cast<AnimDamageType>(plain::enum_value(asset_data::content::anim_damage_type_names, data.death_anim_damage_type, "AttDamage.deathAnimDamageType"));
			native.terminateAtRange = data.terminate_at_range;
		}

		std::size_t size_att_damage([[maybe_unused]] const asset_data::content::att_damage& data)
		{
			std::size_t size = 0;
			size += size_weapon_damage_info(data.damage_info);
			return size;
		}

		asset_data::content::att_melee read_att_melee(const AttMelee& native)
		{
			plain::trace("read_att_melee");
			asset_data::content::att_melee data;
			data.melee_damage = native.meleeDamage;
			data.has_melee_finisher = native.hasMeleeFinisher;
			data.melee_count_to_finisher = native.meleeCountToFinisher;
			return data;
		}

		void write_att_melee(AttMelee& native, const asset_data::content::att_melee& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.meleeDamage = data.melee_damage;
			native.hasMeleeFinisher = data.has_melee_finisher;
			native.meleeCountToFinisher = data.melee_count_to_finisher;
		}

		std::size_t size_att_melee([[maybe_unused]] const asset_data::content::att_melee& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::advanced_idle_settings read_advanced_idle_settings(const AdvancedIdleSettings& native)
		{
			plain::trace("read_advanced_idle_settings");
			asset_data::content::advanced_idle_settings data;
			data.use_advanced_idle_settings = native.useAdvancedIdleSettings;
			data.use_random_points_algorithm = native.useRandomPointsAlgorithm;
			data.breath_gasp_scale_override = native.breathGaspScaleOverride;
			data.idle_sway_setting1_hip_bullet_dir_scale = native.idleSwaySetting1_HipBulletDirScale;
			data.idle_sway_setting1_hip_idle_speed = native.idleSwaySetting1_HipIdleSpeed;
			data.idle_sway_setting1_hip_weapon_magnitude_x = native.idleSwaySetting1_HipWeaponMagnitudeX;
			data.idle_sway_setting1_hip_weapon_magnitude_y = native.idleSwaySetting1_HipWeaponMagnitudeY;
			data.idle_sway_setting1_hip_weapon_magnitude_z = native.idleSwaySetting1_HipWeaponMagnitudeZ;
			data.idle_sway_setting1_hip_weapon_magnitude_f = native.idleSwaySetting1_HipWeaponMagnitudeF;
			data.idle_sway_setting1_hip_weapon_rotation_offset = native.idleSwaySetting1_HipWeaponRotationOffset;
			data.idle_sway_setting1_hip_view_magnitude_x = native.idleSwaySetting1_HipViewMagnitudeX;
			data.idle_sway_setting1_hip_view_magnitude_y = native.idleSwaySetting1_HipViewMagnitudeY;
			data.idle_sway_setting1_ads_bullet_dir_scale = native.idleSwaySetting1_AdsBulletDirScale;
			data.idle_sway_setting1_ads_idle_speed = native.idleSwaySetting1_AdsIdleSpeed;
			data.idle_sway_setting1_ads_weapon_magnitude_x = native.idleSwaySetting1_AdsWeaponMagnitudeX;
			data.idle_sway_setting1_ads_weapon_magnitude_y = native.idleSwaySetting1_AdsWeaponMagnitudeY;
			data.idle_sway_setting1_ads_weapon_magnitude_z = native.idleSwaySetting1_AdsWeaponMagnitudeZ;
			data.idle_sway_setting1_ads_weapon_magnitude_f = native.idleSwaySetting1_AdsWeaponMagnitudeF;
			data.idle_sway_setting1_ads_weapon_rotation_offset = native.idleSwaySetting1_AdsWeaponRotationOffset;
			data.idle_sway_setting1_ads_view_magnitude_x = native.idleSwaySetting1_AdsViewMagnitudeX;
			data.idle_sway_setting1_ads_view_magnitude_y = native.idleSwaySetting1_AdsViewMagnitudeY;
			data.idle_sway_setting2_hip_bullet_dir_scale = native.idleSwaySetting2_HipBulletDirScale;
			data.idle_sway_setting2_hip_idle_speed = native.idleSwaySetting2_HipIdleSpeed;
			data.idle_sway_setting2_hip_weapon_magnitude_x = native.idleSwaySetting2_HipWeaponMagnitudeX;
			data.idle_sway_setting2_hip_weapon_magnitude_y = native.idleSwaySetting2_HipWeaponMagnitudeY;
			data.idle_sway_setting2_hip_weapon_magnitude_z = native.idleSwaySetting2_HipWeaponMagnitudeZ;
			data.idle_sway_setting2_hip_weapon_magnitude_f = native.idleSwaySetting2_HipWeaponMagnitudeF;
			data.idle_sway_setting2_hip_weapon_rotation_offset = native.idleSwaySetting2_HipWeaponRotationOffset;
			data.idle_sway_setting2_hip_view_magnitude_x = native.idleSwaySetting2_HipViewMagnitudeX;
			data.idle_sway_setting2_hip_view_magnitude_y = native.idleSwaySetting2_HipViewMagnitudeY;
			data.idle_sway_setting2_ads_bullet_dir_scale = native.idleSwaySetting2_AdsBulletDirScale;
			data.idle_sway_setting2_ads_idle_speed = native.idleSwaySetting2_AdsIdleSpeed;
			data.idle_sway_setting2_ads_weapon_magnitude_x = native.idleSwaySetting2_AdsWeaponMagnitudeX;
			data.idle_sway_setting2_ads_weapon_magnitude_y = native.idleSwaySetting2_AdsWeaponMagnitudeY;
			data.idle_sway_setting2_ads_weapon_magnitude_z = native.idleSwaySetting2_AdsWeaponMagnitudeZ;
			data.idle_sway_setting2_ads_weapon_magnitude_f = native.idleSwaySetting2_AdsWeaponMagnitudeF;
			data.idle_sway_setting2_ads_weapon_rotation_offset = native.idleSwaySetting2_AdsWeaponRotationOffset;
			data.idle_sway_setting2_ads_view_magnitude_x = native.idleSwaySetting2_AdsViewMagnitudeX;
			data.idle_sway_setting2_ads_view_magnitude_y = native.idleSwaySetting2_AdsViewMagnitudeY;
			return data;
		}

		void write_advanced_idle_settings(AdvancedIdleSettings& native, const asset_data::content::advanced_idle_settings& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.useAdvancedIdleSettings = data.use_advanced_idle_settings;
			native.useRandomPointsAlgorithm = data.use_random_points_algorithm;
			native.breathGaspScaleOverride = data.breath_gasp_scale_override;
			native.idleSwaySetting1_HipBulletDirScale = data.idle_sway_setting1_hip_bullet_dir_scale;
			native.idleSwaySetting1_HipIdleSpeed = data.idle_sway_setting1_hip_idle_speed;
			native.idleSwaySetting1_HipWeaponMagnitudeX = data.idle_sway_setting1_hip_weapon_magnitude_x;
			native.idleSwaySetting1_HipWeaponMagnitudeY = data.idle_sway_setting1_hip_weapon_magnitude_y;
			native.idleSwaySetting1_HipWeaponMagnitudeZ = data.idle_sway_setting1_hip_weapon_magnitude_z;
			native.idleSwaySetting1_HipWeaponMagnitudeF = data.idle_sway_setting1_hip_weapon_magnitude_f;
			native.idleSwaySetting1_HipWeaponRotationOffset = data.idle_sway_setting1_hip_weapon_rotation_offset;
			native.idleSwaySetting1_HipViewMagnitudeX = data.idle_sway_setting1_hip_view_magnitude_x;
			native.idleSwaySetting1_HipViewMagnitudeY = data.idle_sway_setting1_hip_view_magnitude_y;
			native.idleSwaySetting1_AdsBulletDirScale = data.idle_sway_setting1_ads_bullet_dir_scale;
			native.idleSwaySetting1_AdsIdleSpeed = data.idle_sway_setting1_ads_idle_speed;
			native.idleSwaySetting1_AdsWeaponMagnitudeX = data.idle_sway_setting1_ads_weapon_magnitude_x;
			native.idleSwaySetting1_AdsWeaponMagnitudeY = data.idle_sway_setting1_ads_weapon_magnitude_y;
			native.idleSwaySetting1_AdsWeaponMagnitudeZ = data.idle_sway_setting1_ads_weapon_magnitude_z;
			native.idleSwaySetting1_AdsWeaponMagnitudeF = data.idle_sway_setting1_ads_weapon_magnitude_f;
			native.idleSwaySetting1_AdsWeaponRotationOffset = data.idle_sway_setting1_ads_weapon_rotation_offset;
			native.idleSwaySetting1_AdsViewMagnitudeX = data.idle_sway_setting1_ads_view_magnitude_x;
			native.idleSwaySetting1_AdsViewMagnitudeY = data.idle_sway_setting1_ads_view_magnitude_y;
			native.idleSwaySetting2_HipBulletDirScale = data.idle_sway_setting2_hip_bullet_dir_scale;
			native.idleSwaySetting2_HipIdleSpeed = data.idle_sway_setting2_hip_idle_speed;
			native.idleSwaySetting2_HipWeaponMagnitudeX = data.idle_sway_setting2_hip_weapon_magnitude_x;
			native.idleSwaySetting2_HipWeaponMagnitudeY = data.idle_sway_setting2_hip_weapon_magnitude_y;
			native.idleSwaySetting2_HipWeaponMagnitudeZ = data.idle_sway_setting2_hip_weapon_magnitude_z;
			native.idleSwaySetting2_HipWeaponMagnitudeF = data.idle_sway_setting2_hip_weapon_magnitude_f;
			native.idleSwaySetting2_HipWeaponRotationOffset = data.idle_sway_setting2_hip_weapon_rotation_offset;
			native.idleSwaySetting2_HipViewMagnitudeX = data.idle_sway_setting2_hip_view_magnitude_x;
			native.idleSwaySetting2_HipViewMagnitudeY = data.idle_sway_setting2_hip_view_magnitude_y;
			native.idleSwaySetting2_AdsBulletDirScale = data.idle_sway_setting2_ads_bullet_dir_scale;
			native.idleSwaySetting2_AdsIdleSpeed = data.idle_sway_setting2_ads_idle_speed;
			native.idleSwaySetting2_AdsWeaponMagnitudeX = data.idle_sway_setting2_ads_weapon_magnitude_x;
			native.idleSwaySetting2_AdsWeaponMagnitudeY = data.idle_sway_setting2_ads_weapon_magnitude_y;
			native.idleSwaySetting2_AdsWeaponMagnitudeZ = data.idle_sway_setting2_ads_weapon_magnitude_z;
			native.idleSwaySetting2_AdsWeaponMagnitudeF = data.idle_sway_setting2_ads_weapon_magnitude_f;
			native.idleSwaySetting2_AdsWeaponRotationOffset = data.idle_sway_setting2_ads_weapon_rotation_offset;
			native.idleSwaySetting2_AdsViewMagnitudeX = data.idle_sway_setting2_ads_view_magnitude_x;
			native.idleSwaySetting2_AdsViewMagnitudeY = data.idle_sway_setting2_ads_view_magnitude_y;
		}

		std::size_t size_advanced_idle_settings([[maybe_unused]] const asset_data::content::advanced_idle_settings& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::att_idle_settings read_att_idle_settings(const AttIdleSettings& native)
		{
			plain::trace("read_att_idle_settings");
			asset_data::content::att_idle_settings data;
			data.hip_idle_amount = native.hipIdleAmount;
			data.hip_idle_speed = native.hipIdleSpeed;
			data.idle_crouch_factor = native.idleCrouchFactor;
			data.idle_prone_factor = native.idleProneFactor;
			data.ads_idle_lerp_start_time = native.adsIdleLerpStartTime;
			data.ads_idle_lerp_time = native.adsIdleLerpTime;
			data.use_uninterrupted_ads_idle_motion = native.useUninterruptedAdsIdleMotion;
			data.advanced_idle_settings = read_advanced_idle_settings(native.advancedIdleSettings);
			return data;
		}

		void write_att_idle_settings(AttIdleSettings& native, const asset_data::content::att_idle_settings& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.hipIdleAmount = data.hip_idle_amount;
			native.hipIdleSpeed = data.hip_idle_speed;
			native.idleCrouchFactor = data.idle_crouch_factor;
			native.idleProneFactor = data.idle_prone_factor;
			native.adsIdleLerpStartTime = data.ads_idle_lerp_start_time;
			native.adsIdleLerpTime = data.ads_idle_lerp_time;
			native.useUninterruptedAdsIdleMotion = data.use_uninterrupted_ads_idle_motion;
			write_advanced_idle_settings(native.advancedIdleSettings, data.advanced_idle_settings, resolve, memory);
		}

		std::size_t size_att_idle_settings([[maybe_unused]] const asset_data::content::att_idle_settings& data)
		{
			std::size_t size = 0;
			size += size_advanced_idle_settings(data.advanced_idle_settings);
			return size;
		}

		asset_data::content::scene_zoom_settings read_scene_zoom_settings(const SceneZoomSettings& native)
		{
			plain::trace("read_scene_zoom_settings");
			asset_data::content::scene_zoom_settings data;
			data.ads_zoom_count = native.adsZoomCount;
			data.ads_zoom_fov = std::to_array(native.adsZoomFov);
			data.ads_reload_fov = std::to_array(native.adsReloadFov);
			data.ads_zoom_fov_transition_time = std::to_array(native.adsZoomFovTransitionTime);
			data.ads_zoom_fov_lerp_type = plain::enum_name(asset_data::content::ads_offset_interpolation_type_names, static_cast<std::int64_t>(native.adsZoomFovLerpType), "SceneZoomSettings.adsZoomFovLerpType");
			data.ads_zoom_in_frac = native.adsZoomInFrac;
			data.ads_zoom_out_frac = native.adsZoomOutFrac;
			data.ads_fovnvg = native.adsFOVNVG;
			data.ads_reload_fov_nvg = native.adsReloadFovNVG;
			data.ads_zoom_level_index_reset_on_toggle = native.adsZoomLevelIndexResetOnToggle;
			return data;
		}

		void write_scene_zoom_settings(SceneZoomSettings& native, const asset_data::content::scene_zoom_settings& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.adsZoomCount = data.ads_zoom_count;
			std::ranges::copy(data.ads_zoom_fov, native.adsZoomFov);
			std::ranges::copy(data.ads_reload_fov, native.adsReloadFov);
			std::ranges::copy(data.ads_zoom_fov_transition_time, native.adsZoomFovTransitionTime);
			native.adsZoomFovLerpType = static_cast<AdsOffsetInterpolationType>(plain::enum_value(asset_data::content::ads_offset_interpolation_type_names, data.ads_zoom_fov_lerp_type, "SceneZoomSettings.adsZoomFovLerpType"));
			native.adsZoomInFrac = data.ads_zoom_in_frac;
			native.adsZoomOutFrac = data.ads_zoom_out_frac;
			native.adsFOVNVG = data.ads_fovnvg;
			native.adsReloadFovNVG = data.ads_reload_fov_nvg;
			native.adsZoomLevelIndexResetOnToggle = data.ads_zoom_level_index_reset_on_toggle;
		}

		std::size_t size_scene_zoom_settings([[maybe_unused]] const asset_data::content::scene_zoom_settings& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::weapon_zoom_settings read_weapon_zoom_settings(const WeaponZoomSettings& native)
		{
			plain::trace("read_weapon_zoom_settings");
			asset_data::content::weapon_zoom_settings data;
			data.ads_zoom_fov = native.adsZoomFov;
			data.ads_zoom_fov_lerp_type = plain::enum_name(asset_data::content::ads_offset_interpolation_type_names, static_cast<std::int64_t>(native.adsZoomFovLerpType), "WeaponZoomSettings.adsZoomFovLerpType");
			data.ads_zoom_fov_x_offset = native.adsZoomFovXOffset;
			data.ads_reload_zoom_fov = native.adsReloadZoomFov;
			data.ads_reload_zoom_fov_x_offset = native.adsReloadZoomFovXOffset;
			data.ads_nvg_zoom_fov = native.adsNVGZoomFov;
			data.ads_nvg_zoom_fov_x_offset = native.adsNVGZoomFovXOffset;
			data.ads_nvg_reload_zoom_fov = native.adsNVGReloadZoomFov;
			data.ads_nvg_reload_zoom_fov_x_offset = native.adsNVGReloadZoomFovXOffset;
			data.ads_zoom_in_frac = native.adsZoomInFrac;
			data.ads_zoom_out_frac = native.adsZoomOutFrac;
			data.ads_zoom_fov_x_offset_override = native.adsZoomFovXOffsetOverride;
			data.ads_reload_fov_x_offset_override = native.adsReloadFovXOffsetOverride;
			return data;
		}

		void write_weapon_zoom_settings(WeaponZoomSettings& native, const asset_data::content::weapon_zoom_settings& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.adsZoomFov = data.ads_zoom_fov;
			native.adsZoomFovLerpType = static_cast<AdsOffsetInterpolationType>(plain::enum_value(asset_data::content::ads_offset_interpolation_type_names, data.ads_zoom_fov_lerp_type, "WeaponZoomSettings.adsZoomFovLerpType"));
			native.adsZoomFovXOffset = data.ads_zoom_fov_x_offset;
			native.adsReloadZoomFov = data.ads_reload_zoom_fov;
			native.adsReloadZoomFovXOffset = data.ads_reload_zoom_fov_x_offset;
			native.adsNVGZoomFov = data.ads_nvg_zoom_fov;
			native.adsNVGZoomFovXOffset = data.ads_nvg_zoom_fov_x_offset;
			native.adsNVGReloadZoomFov = data.ads_nvg_reload_zoom_fov;
			native.adsNVGReloadZoomFovXOffset = data.ads_nvg_reload_zoom_fov_x_offset;
			native.adsZoomInFrac = data.ads_zoom_in_frac;
			native.adsZoomOutFrac = data.ads_zoom_out_frac;
			native.adsZoomFovXOffsetOverride = data.ads_zoom_fov_x_offset_override;
			native.adsReloadFovXOffsetOverride = data.ads_reload_fov_x_offset_override;
		}

		std::size_t size_weapon_zoom_settings([[maybe_unused]] const asset_data::content::weapon_zoom_settings& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::zoom_settings read_zoom_settings(const ZoomSettings& native)
		{
			plain::trace("read_zoom_settings");
			asset_data::content::zoom_settings data;
			data.scene = read_scene_zoom_settings(native.scene);
			data.weapon = read_weapon_zoom_settings(native.weapon);
			return data;
		}

		void write_zoom_settings(ZoomSettings& native, const asset_data::content::zoom_settings& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_scene_zoom_settings(native.scene, data.scene, resolve, memory);
			write_weapon_zoom_settings(native.weapon, data.weapon, resolve, memory);
		}

		std::size_t size_zoom_settings([[maybe_unused]] const asset_data::content::zoom_settings& data)
		{
			std::size_t size = 0;
			size += size_scene_zoom_settings(data.scene);
			size += size_weapon_zoom_settings(data.weapon);
			return size;
		}

		asset_data::content::att_ads_settings read_att_ads_settings(const AttADSSettings& native)
		{
			plain::trace("read_att_ads_settings");
			asset_data::content::att_ads_settings data;
			data.ads_spread = native.adsSpread;
			data.ads_spread_start_frac = native.adsSpreadStartFrac;
			data.ads_spread_end_frac = native.adsSpreadEndFrac;
			data.ads_aim_pitch = native.adsAimPitch;
			data.ads_trans_in_time = native.adsTransInTime;
			data.ads_trans_out_time = native.adsTransOutTime;
			data.ads_trans_in_to_out_time = native.adsTransInToOutTime;
			data.ads_trans_out_to_in_time = native.adsTransOutToInTime;
			data.ads_reload_trans_time = native.adsReloadTransTime;
			data.ads_in_crosshair_alpha_start = native.adsInCrosshairAlphaStart;
			data.ads_in_crosshair_alpha_end = native.adsInCrosshairAlphaEnd;
			data.ads_out_crosshair_alpha_start = native.adsOutCrosshairAlphaStart;
			data.ads_out_crosshair_alpha_end = native.adsOutCrosshairAlphaEnd;
			data.zoom_settings = read_zoom_settings(native.zoomSettings);
			data.ads_fov_lerp_time = native.adsFovLerpTime;
			data.ads_bob_factor = native.adsBobFactor;
			data.ads_view_bob_mult = native.adsViewBobMult;
			data.ads_fire_rate_scale = native.adsFireRateScale;
			data.ads_damage_range_scale = native.adsDamageRangeScale;
			data.ads_fire_anim_frac = native.adsFireAnimFrac;
			data.fire_timer_lerp_to_ads_scale = native.fireTimerLerpToAdsScale;
			data.always_fire_at_max_range_in_ads = native.alwaysFireAtMaxRangeInAds;
			data.ads_align_enabled = native.adsAlignEnabled;
			data.disable_tag_align_x = native.disableTagAlignX;
			data.ads_align_offset = plain::read_scalars<float, 3>(&native.adsAlignOffset);
			data.ads_align_offset_lerp_type = plain::enum_name(asset_data::content::ads_offset_interpolation_type_names, static_cast<std::int64_t>(native.adsAlignOffsetLerpType), "AttADSSettings.adsAlignOffsetLerpType");
			data.ads_align_lerp_type = plain::enum_name(asset_data::content::ads_offset_interpolation_type_names, static_cast<std::int64_t>(native.adsAlignLerpType), "AttADSSettings.adsAlignLerpType");
			return data;
		}

		void write_att_ads_settings(AttADSSettings& native, const asset_data::content::att_ads_settings& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.adsSpread = data.ads_spread;
			native.adsSpreadStartFrac = data.ads_spread_start_frac;
			native.adsSpreadEndFrac = data.ads_spread_end_frac;
			native.adsAimPitch = data.ads_aim_pitch;
			native.adsTransInTime = data.ads_trans_in_time;
			native.adsTransOutTime = data.ads_trans_out_time;
			native.adsTransInToOutTime = data.ads_trans_in_to_out_time;
			native.adsTransOutToInTime = data.ads_trans_out_to_in_time;
			native.adsReloadTransTime = data.ads_reload_trans_time;
			native.adsInCrosshairAlphaStart = data.ads_in_crosshair_alpha_start;
			native.adsInCrosshairAlphaEnd = data.ads_in_crosshair_alpha_end;
			native.adsOutCrosshairAlphaStart = data.ads_out_crosshair_alpha_start;
			native.adsOutCrosshairAlphaEnd = data.ads_out_crosshair_alpha_end;
			write_zoom_settings(native.zoomSettings, data.zoom_settings, resolve, memory);
			native.adsFovLerpTime = data.ads_fov_lerp_time;
			native.adsBobFactor = data.ads_bob_factor;
			native.adsViewBobMult = data.ads_view_bob_mult;
			native.adsFireRateScale = data.ads_fire_rate_scale;
			native.adsDamageRangeScale = data.ads_damage_range_scale;
			native.adsFireAnimFrac = data.ads_fire_anim_frac;
			native.fireTimerLerpToAdsScale = data.fire_timer_lerp_to_ads_scale;
			native.alwaysFireAtMaxRangeInAds = data.always_fire_at_max_range_in_ads;
			native.adsAlignEnabled = data.ads_align_enabled;
			native.disableTagAlignX = data.disable_tag_align_x;
			plain::write_scalars(&native.adsAlignOffset, data.ads_align_offset);
			native.adsAlignOffsetLerpType = static_cast<AdsOffsetInterpolationType>(plain::enum_value(asset_data::content::ads_offset_interpolation_type_names, data.ads_align_offset_lerp_type, "AttADSSettings.adsAlignOffsetLerpType"));
			native.adsAlignLerpType = static_cast<AdsOffsetInterpolationType>(plain::enum_value(asset_data::content::ads_offset_interpolation_type_names, data.ads_align_lerp_type, "AttADSSettings.adsAlignLerpType"));
		}

		std::size_t size_att_ads_settings([[maybe_unused]] const asset_data::content::att_ads_settings& data)
		{
			std::size_t size = 0;
			size += size_zoom_settings(data.zoom_settings);
			return size;
		}

		asset_data::content::att_ads_gun_bob_settings read_att_ads_gun_bob_settings(const AttADSGunBobSettings& native)
		{
			plain::trace("read_att_ads_gun_bob_settings");
			asset_data::content::att_ads_gun_bob_settings data;
			data.f_ads_gun_bob_pitch_scale = native.fAdsGunBobPitchScale;
			data.f_ads_gun_bob_yaw_scale = native.fAdsGunBobYawScale;
			data.f_ads_gun_bob_tilt_pitch_scale = native.fAdsGunBobTiltPitchScale;
			data.f_ads_gun_bob_tilt_yaw_scale = native.fAdsGunBobTiltYawScale;
			data.f_ads_gun_bob_tilt_roll_scale = native.fAdsGunBobTiltRollScale;
			data.f_ads_gun_bob_tilt_bullet_dir_scale = native.fAdsGunBobTiltBulletDirScale;
			data.f_ads_gun_bob_tilt_offset = native.fAdsGunBobTiltOffset;
			return data;
		}

		void write_att_ads_gun_bob_settings(AttADSGunBobSettings& native, const asset_data::content::att_ads_gun_bob_settings& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.fAdsGunBobPitchScale = data.f_ads_gun_bob_pitch_scale;
			native.fAdsGunBobYawScale = data.f_ads_gun_bob_yaw_scale;
			native.fAdsGunBobTiltPitchScale = data.f_ads_gun_bob_tilt_pitch_scale;
			native.fAdsGunBobTiltYawScale = data.f_ads_gun_bob_tilt_yaw_scale;
			native.fAdsGunBobTiltRollScale = data.f_ads_gun_bob_tilt_roll_scale;
			native.fAdsGunBobTiltBulletDirScale = data.f_ads_gun_bob_tilt_bullet_dir_scale;
			native.fAdsGunBobTiltOffset = data.f_ads_gun_bob_tilt_offset;
		}

		std::size_t size_att_ads_gun_bob_settings([[maybe_unused]] const asset_data::content::att_ads_gun_bob_settings& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::att_ads_reload_settings read_att_ads_reload_settings(const AttADSReloadSettings& native)
		{
			plain::trace("read_att_ads_reload_settings");
			asset_data::content::att_ads_reload_settings data;
			data.reload_while_ads = native.reloadWhileADS;
			return data;
		}

		void write_att_ads_reload_settings(AttADSReloadSettings& native, const asset_data::content::att_ads_reload_settings& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.reloadWhileADS = data.reload_while_ads;
		}

		std::size_t size_att_ads_reload_settings([[maybe_unused]] const asset_data::content::att_ads_reload_settings& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::att_scope_drift_settings read_att_scope_drift_settings(const AttScopeDriftSettings& native)
		{
			plain::trace("read_att_scope_drift_settings");
			asset_data::content::att_scope_drift_settings data;
			data.f_scope_drift_delay = native.fScopeDriftDelay;
			data.f_scope_drift_lerp_in_time = native.fScopeDriftLerpInTime;
			data.f_scope_drift_steady_time = native.fScopeDriftSteadyTime;
			data.f_scope_drift_lerp_out_time = native.fScopeDriftLerpOutTime;
			data.f_scope_drift_steady_factor = native.fScopeDriftSteadyFactor;
			data.f_scope_drift_unsteady_factor = native.fScopeDriftUnsteadyFactor;
			return data;
		}

		void write_att_scope_drift_settings(AttScopeDriftSettings& native, const asset_data::content::att_scope_drift_settings& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.fScopeDriftDelay = data.f_scope_drift_delay;
			native.fScopeDriftLerpInTime = data.f_scope_drift_lerp_in_time;
			native.fScopeDriftSteadyTime = data.f_scope_drift_steady_time;
			native.fScopeDriftLerpOutTime = data.f_scope_drift_lerp_out_time;
			native.fScopeDriftSteadyFactor = data.f_scope_drift_steady_factor;
			native.fScopeDriftUnsteadyFactor = data.f_scope_drift_unsteady_factor;
		}

		std::size_t size_att_scope_drift_settings([[maybe_unused]] const asset_data::content::att_scope_drift_settings& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::weapon_offset_pattern_scale_info read_weapon_offset_pattern_scale_info(const WeaponOffsetPatternScaleInfo& native)
		{
			plain::trace("read_weapon_offset_pattern_scale_info");
			asset_data::content::weapon_offset_pattern_scale_info data;
			data.num_pattern_scales = native.numPatternScales;
			data.pattern_scales = plain::read_array(native.patternScales, native.numPatternScales, "WeaponOffsetPatternScaleInfo.patternScales", [&](const auto& item) { return read_weapon_offset_pattern_scale(item); });
			return data;
		}

		void write_weapon_offset_pattern_scale_info(WeaponOffsetPatternScaleInfo& native, const asset_data::content::weapon_offset_pattern_scale_info& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.numPatternScales = data.num_pattern_scales;
			native.patternScales = plain::write_array<WeaponOffsetPatternScale>(data.pattern_scales, memory, [&](auto& out, const auto& in) { write_weapon_offset_pattern_scale(out, in, resolve, memory); });
		}

		std::size_t size_weapon_offset_pattern_scale_info([[maybe_unused]] const asset_data::content::weapon_offset_pattern_scale_info& data)
		{
			std::size_t size = 0;
			if (data.pattern_scales)
			{
				size += plain::sized(std::max<std::size_t>(data.pattern_scales->size(), 1) * sizeof(WeaponOffsetPatternScale));
				for (const auto& item : *data.pattern_scales)
				{
					size += size_weapon_offset_pattern_scale(item);
				}
			}
			return size;
		}

		asset_data::content::att_ads_stance_scales read_att_ads_stance_scales(const AttADSStanceScales& native)
		{
			plain::trace("read_att_ads_stance_scales");
			asset_data::content::att_ads_stance_scales data;
			data.ads_crouch_view_kick_factor_scale = native.adsCrouchViewKickFactorScale;
			data.ads_crouch_gun_kick_factor_scale = native.adsCrouchGunKickFactorScale;
			data.ads_prone_view_kick_factor_scale = native.adsProneViewKickFactorScale;
			data.ads_prone_gun_kick_factor_scale = native.adsProneGunKickFactorScale;
			data.ads_crouch_gun_bob_factor_scale = native.adsCrouchGunBobFactorScale;
			data.crouched_weapon_offset_pattern_scales = read_weapon_offset_pattern_scale_info(native.crouchedWeaponOffsetPatternScales);
			data.prone_weapon_offset_pattern_scales = read_weapon_offset_pattern_scale_info(native.proneWeaponOffsetPatternScales);
			return data;
		}

		void write_att_ads_stance_scales(AttADSStanceScales& native, const asset_data::content::att_ads_stance_scales& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.adsCrouchViewKickFactorScale = data.ads_crouch_view_kick_factor_scale;
			native.adsCrouchGunKickFactorScale = data.ads_crouch_gun_kick_factor_scale;
			native.adsProneViewKickFactorScale = data.ads_prone_view_kick_factor_scale;
			native.adsProneGunKickFactorScale = data.ads_prone_gun_kick_factor_scale;
			native.adsCrouchGunBobFactorScale = data.ads_crouch_gun_bob_factor_scale;
			write_weapon_offset_pattern_scale_info(native.crouchedWeaponOffsetPatternScales, data.crouched_weapon_offset_pattern_scales, resolve, memory);
			write_weapon_offset_pattern_scale_info(native.proneWeaponOffsetPatternScales, data.prone_weapon_offset_pattern_scales, resolve, memory);
		}

		std::size_t size_att_ads_stance_scales([[maybe_unused]] const asset_data::content::att_ads_stance_scales& data)
		{
			std::size_t size = 0;
			size += size_weapon_offset_pattern_scale_info(data.crouched_weapon_offset_pattern_scales);
			size += size_weapon_offset_pattern_scale_info(data.prone_weapon_offset_pattern_scales);
			return size;
		}

		asset_data::content::att_hip_spread read_att_hip_spread(const AttHipSpread& native)
		{
			plain::trace("read_att_hip_spread");
			asset_data::content::att_hip_spread data;
			data.hip_spread_stand_min = native.hipSpreadStandMin;
			data.hip_spread_ducked_min = native.hipSpreadDuckedMin;
			data.hip_spread_prone_min = native.hipSpreadProneMin;
			data.hip_spread_sprint_min = native.hipSpreadSprintMin;
			data.hip_spread_in_air_min = native.hipSpreadInAirMin;
			data.hip_spread_max = native.hipSpreadMax;
			data.hip_spread_ducked_max = native.hipSpreadDuckedMax;
			data.hip_spread_prone_max = native.hipSpreadProneMax;
			data.hip_spread_sprint_max = native.hipSpreadSprintMax;
			data.hip_spread_in_air_max = native.hipSpreadInAirMax;
			data.hip_spread_fire_add = native.hipSpreadFireAdd;
			data.hip_spread_turn_add = native.hipSpreadTurnAdd;
			data.hip_spread_move_add = native.hipSpreadMoveAdd;
			data.hip_spread_decay_rate = native.hipSpreadDecayRate;
			data.hip_spread_ducked_decay = native.hipSpreadDuckedDecay;
			data.hip_spread_prone_decay = native.hipSpreadProneDecay;
			data.hip_spread_sprint_decay = native.hipSpreadSprintDecay;
			data.hip_spread_in_air_decay = native.hipSpreadInAirDecay;
			data.hip_spread_stand_move_max = native.hipSpreadStandMoveMax;
			data.hip_spread_ducked_move_max = native.hipSpreadDuckedMoveMax;
			data.hip_spread_prone_move_max = native.hipSpreadProneMoveMax;
			data.hip_spread_sprint_move_max = native.hipSpreadSprintMoveMax;
			data.hip_spread_in_air_move_max = native.hipSpreadInAirMoveMax;
			return data;
		}

		void write_att_hip_spread(AttHipSpread& native, const asset_data::content::att_hip_spread& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.hipSpreadStandMin = data.hip_spread_stand_min;
			native.hipSpreadDuckedMin = data.hip_spread_ducked_min;
			native.hipSpreadProneMin = data.hip_spread_prone_min;
			native.hipSpreadSprintMin = data.hip_spread_sprint_min;
			native.hipSpreadInAirMin = data.hip_spread_in_air_min;
			native.hipSpreadMax = data.hip_spread_max;
			native.hipSpreadDuckedMax = data.hip_spread_ducked_max;
			native.hipSpreadProneMax = data.hip_spread_prone_max;
			native.hipSpreadSprintMax = data.hip_spread_sprint_max;
			native.hipSpreadInAirMax = data.hip_spread_in_air_max;
			native.hipSpreadFireAdd = data.hip_spread_fire_add;
			native.hipSpreadTurnAdd = data.hip_spread_turn_add;
			native.hipSpreadMoveAdd = data.hip_spread_move_add;
			native.hipSpreadDecayRate = data.hip_spread_decay_rate;
			native.hipSpreadDuckedDecay = data.hip_spread_ducked_decay;
			native.hipSpreadProneDecay = data.hip_spread_prone_decay;
			native.hipSpreadSprintDecay = data.hip_spread_sprint_decay;
			native.hipSpreadInAirDecay = data.hip_spread_in_air_decay;
			native.hipSpreadStandMoveMax = data.hip_spread_stand_move_max;
			native.hipSpreadDuckedMoveMax = data.hip_spread_ducked_move_max;
			native.hipSpreadProneMoveMax = data.hip_spread_prone_move_max;
			native.hipSpreadSprintMoveMax = data.hip_spread_sprint_move_max;
			native.hipSpreadInAirMoveMax = data.hip_spread_in_air_move_max;
		}

		std::size_t size_att_hip_spread([[maybe_unused]] const asset_data::content::att_hip_spread& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::att_slide_spread read_att_slide_spread(const AttSlideSpread& native)
		{
			plain::trace("read_att_slide_spread");
			asset_data::content::att_slide_spread data;
			data.slide_spread_min = native.slideSpreadMin;
			data.slide_spread_max = native.slideSpreadMax;
			data.slide_spread_decay_rate = native.slideSpreadDecayRate;
			data.slide_spread_fire_add = native.slideSpreadFireAdd;
			data.slide_spread_turn_add = native.slideSpreadTurnAdd;
			data.slide_spread_move_max = native.slideSpreadMoveMax;
			return data;
		}

		void write_att_slide_spread(AttSlideSpread& native, const asset_data::content::att_slide_spread& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.slideSpreadMin = data.slide_spread_min;
			native.slideSpreadMax = data.slide_spread_max;
			native.slideSpreadDecayRate = data.slide_spread_decay_rate;
			native.slideSpreadFireAdd = data.slide_spread_fire_add;
			native.slideSpreadTurnAdd = data.slide_spread_turn_add;
			native.slideSpreadMoveMax = data.slide_spread_move_max;
		}

		std::size_t size_att_slide_spread([[maybe_unused]] const asset_data::content::att_slide_spread& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::att_gun_kick read_att_gun_kick(const AttGunKick& native)
		{
			plain::trace("read_att_gun_kick");
			asset_data::content::att_gun_kick data;
			data.hip_gun_kick_pitch_min = native.hipGunKickPitchMin;
			data.hip_gun_kick_pitch_max = native.hipGunKickPitchMax;
			data.hip_gun_kick_yaw_min = native.hipGunKickYawMin;
			data.hip_gun_kick_yaw_max = native.hipGunKickYawMax;
			data.hip_gun_kick_mag_min = native.hipGunKickMagMin;
			data.ads_gun_kick_pitch_min = native.adsGunKickPitchMin;
			data.ads_gun_kick_pitch_max = native.adsGunKickPitchMax;
			data.ads_gun_kick_yaw_min = native.adsGunKickYawMin;
			data.ads_gun_kick_yaw_max = native.adsGunKickYawMax;
			data.ads_gun_kick_mag_min = native.adsGunKickMagMin;
			data.use_angular_gun_kick = native.useAngularGunKick;
			data.hip_angular_gun_kick_dir = std::to_array(native.hipAngularGunKickDir);
			data.hip_angular_gun_kick_dev = std::to_array(native.hipAngularGunKickDev);
			data.hip_angular_gun_kick_strength_min = std::to_array(native.hipAngularGunKickStrengthMin);
			data.hip_angular_gun_kick_strength_max = std::to_array(native.hipAngularGunKickStrengthMax);
			data.hip_angular_gun_kick_pitch_scale = std::to_array(native.hipAngularGunKickPitchScale);
			data.ads_angular_gun_kick_dir = std::to_array(native.adsAngularGunKickDir);
			data.ads_angular_gun_kick_dev = std::to_array(native.adsAngularGunKickDev);
			data.ads_angular_gun_kick_strength_min = std::to_array(native.adsAngularGunKickStrengthMin);
			data.ads_angular_gun_kick_strength_max = std::to_array(native.adsAngularGunKickStrengthMax);
			data.ads_angular_gun_kick_pitch_scale = std::to_array(native.adsAngularGunKickPitchScale);
			data.ads_angular_gun_kick_bullet = std::to_array(native.adsAngularGunKickBullet);
			data.ads_angular_gun_kick_use_set = std::to_array(native.adsAngularGunKickUseSet);
			data.use_new_gun_kick = native.useNewGunKick;
			data.hip_gun_kick_accel = native.hipGunKickAccel;
			data.ads_gun_kick_accel = native.adsGunKickAccel;
			data.hip_gun_kick_speed_max = native.hipGunKickSpeedMax;
			data.ads_gun_kick_speed_max = native.adsGunKickSpeedMax;
			data.hip_gun_kick_speed_decay = native.hipGunKickSpeedDecay;
			data.ads_gun_kick_speed_decay = native.adsGunKickSpeedDecay;
			data.hip_gun_kick_static_decay = native.hipGunKickStaticDecay;
			data.ads_gun_kick_static_decay = native.adsGunKickStaticDecay;
			data.hip_gun_kick_return_accel_scale = native.hipGunKickReturnAccelScale;
			data.ads_gun_kick_return_accel_scale = native.adsGunKickReturnAccelScale;
			data.hip_gun_kick_return_speed_curve_scale = native.hipGunKickReturnSpeedCurveScale;
			data.ads_gun_kick_return_speed_curve_scale = native.adsGunKickReturnSpeedCurveScale;
			data.gun_max_pitch = native.gunMaxPitch;
			data.gun_max_yaw = native.gunMaxYaw;
			data.bullet_dir_gun_angle_modifier_gun_kick_hip = native.bulletDirGunAngleModifierGunKickHip;
			data.bullet_dir_gun_angle_modifier_gun_kick_ads = native.bulletDirGunAngleModifierGunKickAds;
			return data;
		}

		void write_att_gun_kick(AttGunKick& native, const asset_data::content::att_gun_kick& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.hipGunKickPitchMin = data.hip_gun_kick_pitch_min;
			native.hipGunKickPitchMax = data.hip_gun_kick_pitch_max;
			native.hipGunKickYawMin = data.hip_gun_kick_yaw_min;
			native.hipGunKickYawMax = data.hip_gun_kick_yaw_max;
			native.hipGunKickMagMin = data.hip_gun_kick_mag_min;
			native.adsGunKickPitchMin = data.ads_gun_kick_pitch_min;
			native.adsGunKickPitchMax = data.ads_gun_kick_pitch_max;
			native.adsGunKickYawMin = data.ads_gun_kick_yaw_min;
			native.adsGunKickYawMax = data.ads_gun_kick_yaw_max;
			native.adsGunKickMagMin = data.ads_gun_kick_mag_min;
			native.useAngularGunKick = data.use_angular_gun_kick;
			std::ranges::copy(data.hip_angular_gun_kick_dir, native.hipAngularGunKickDir);
			std::ranges::copy(data.hip_angular_gun_kick_dev, native.hipAngularGunKickDev);
			std::ranges::copy(data.hip_angular_gun_kick_strength_min, native.hipAngularGunKickStrengthMin);
			std::ranges::copy(data.hip_angular_gun_kick_strength_max, native.hipAngularGunKickStrengthMax);
			std::ranges::copy(data.hip_angular_gun_kick_pitch_scale, native.hipAngularGunKickPitchScale);
			std::ranges::copy(data.ads_angular_gun_kick_dir, native.adsAngularGunKickDir);
			std::ranges::copy(data.ads_angular_gun_kick_dev, native.adsAngularGunKickDev);
			std::ranges::copy(data.ads_angular_gun_kick_strength_min, native.adsAngularGunKickStrengthMin);
			std::ranges::copy(data.ads_angular_gun_kick_strength_max, native.adsAngularGunKickStrengthMax);
			std::ranges::copy(data.ads_angular_gun_kick_pitch_scale, native.adsAngularGunKickPitchScale);
			std::ranges::copy(data.ads_angular_gun_kick_bullet, native.adsAngularGunKickBullet);
			std::ranges::copy(data.ads_angular_gun_kick_use_set, native.adsAngularGunKickUseSet);
			native.useNewGunKick = data.use_new_gun_kick;
			native.hipGunKickAccel = data.hip_gun_kick_accel;
			native.adsGunKickAccel = data.ads_gun_kick_accel;
			native.hipGunKickSpeedMax = data.hip_gun_kick_speed_max;
			native.adsGunKickSpeedMax = data.ads_gun_kick_speed_max;
			native.hipGunKickSpeedDecay = data.hip_gun_kick_speed_decay;
			native.adsGunKickSpeedDecay = data.ads_gun_kick_speed_decay;
			native.hipGunKickStaticDecay = data.hip_gun_kick_static_decay;
			native.adsGunKickStaticDecay = data.ads_gun_kick_static_decay;
			native.hipGunKickReturnAccelScale = data.hip_gun_kick_return_accel_scale;
			native.adsGunKickReturnAccelScale = data.ads_gun_kick_return_accel_scale;
			native.hipGunKickReturnSpeedCurveScale = data.hip_gun_kick_return_speed_curve_scale;
			native.adsGunKickReturnSpeedCurveScale = data.ads_gun_kick_return_speed_curve_scale;
			native.gunMaxPitch = data.gun_max_pitch;
			native.gunMaxYaw = data.gun_max_yaw;
			native.bulletDirGunAngleModifierGunKickHip = data.bullet_dir_gun_angle_modifier_gun_kick_hip;
			native.bulletDirGunAngleModifierGunKickAds = data.bullet_dir_gun_angle_modifier_gun_kick_ads;
		}

		std::size_t size_att_gun_kick([[maybe_unused]] const asset_data::content::att_gun_kick& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::att_gun_tilt read_att_gun_tilt(const AttGunTilt& native)
		{
			plain::trace("read_att_gun_tilt");
			asset_data::content::att_gun_tilt data;
			data.ads_gun_tilt_pitch_factor = native.adsGunTiltPitchFactor;
			data.ads_gun_tilt_yaw_factor = native.adsGunTiltYawFactor;
			data.ads_gun_tilt_roll_factor = native.adsGunTiltRollFactor;
			data.ads_gun_tilt_offset = native.adsGunTiltOffset;
			data.hip_gun_tilt_pitch_factor = native.hipGunTiltPitchFactor;
			data.hip_gun_tilt_yaw_factor = native.hipGunTiltYawFactor;
			data.hip_gun_tilt_roll_factor = native.hipGunTiltRollFactor;
			data.hip_gun_tilt_offset = native.hipGunTiltOffset;
			return data;
		}

		void write_att_gun_tilt(AttGunTilt& native, const asset_data::content::att_gun_tilt& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.adsGunTiltPitchFactor = data.ads_gun_tilt_pitch_factor;
			native.adsGunTiltYawFactor = data.ads_gun_tilt_yaw_factor;
			native.adsGunTiltRollFactor = data.ads_gun_tilt_roll_factor;
			native.adsGunTiltOffset = data.ads_gun_tilt_offset;
			native.hipGunTiltPitchFactor = data.hip_gun_tilt_pitch_factor;
			native.hipGunTiltYawFactor = data.hip_gun_tilt_yaw_factor;
			native.hipGunTiltRollFactor = data.hip_gun_tilt_roll_factor;
			native.hipGunTiltOffset = data.hip_gun_tilt_offset;
		}

		std::size_t size_att_gun_tilt([[maybe_unused]] const asset_data::content::att_gun_tilt& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::att_kick_scaling read_att_kick_scaling(const AttKickScaling& native)
		{
			plain::trace("read_att_kick_scaling");
			asset_data::content::att_kick_scaling data;
			data.hip_starting_gun_kick_percent = native.hipStartingGunKickPercent;
			data.hip_starting_view_kick_percent = native.hipStartingViewKickPercent;
			data.ads_starting_kick_bullets = native.adsStartingKickBullets;
			data.ads_starting_gun_kick_percent = native.adsStartingGunKickPercent;
			data.ads_starting_view_kick_percent = native.adsStartingViewKickPercent;
			data.hip_ending_gun_kick_percent = native.hipEndingGunKickPercent;
			data.hip_ending_view_kick_percent = native.hipEndingViewKickPercent;
			data.ads_ending_kick_bullets = native.adsEndingKickBullets;
			data.ads_ending_gun_kick_percent = native.adsEndingGunKickPercent;
			data.ads_ending_view_kick_percent = native.adsEndingViewKickPercent;
			return data;
		}

		void write_att_kick_scaling(AttKickScaling& native, const asset_data::content::att_kick_scaling& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.hipStartingGunKickPercent = data.hip_starting_gun_kick_percent;
			native.hipStartingViewKickPercent = data.hip_starting_view_kick_percent;
			native.adsStartingKickBullets = data.ads_starting_kick_bullets;
			native.adsStartingGunKickPercent = data.ads_starting_gun_kick_percent;
			native.adsStartingViewKickPercent = data.ads_starting_view_kick_percent;
			native.hipEndingGunKickPercent = data.hip_ending_gun_kick_percent;
			native.hipEndingViewKickPercent = data.hip_ending_view_kick_percent;
			native.adsEndingKickBullets = data.ads_ending_kick_bullets;
			native.adsEndingGunKickPercent = data.ads_ending_gun_kick_percent;
			native.adsEndingViewKickPercent = data.ads_ending_view_kick_percent;
		}

		std::size_t size_att_kick_scaling([[maybe_unused]] const asset_data::content::att_kick_scaling& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::att_view_kick read_att_view_kick(const AttViewKick& native)
		{
			plain::trace("read_att_view_kick");
			asset_data::content::att_view_kick data;
			data.hip_view_kick_pitch_min = native.hipViewKickPitchMin;
			data.hip_view_kick_pitch_max = native.hipViewKickPitchMax;
			data.hip_view_kick_yaw_min = native.hipViewKickYawMin;
			data.hip_view_kick_yaw_max = native.hipViewKickYawMax;
			data.hip_view_kick_mag_min = native.hipViewKickMagMin;
			data.ads_view_kick_pitch_min = native.adsViewKickPitchMin;
			data.ads_view_kick_pitch_max = native.adsViewKickPitchMax;
			data.ads_view_kick_yaw_min = native.adsViewKickYawMin;
			data.ads_view_kick_yaw_max = native.adsViewKickYawMax;
			data.ads_view_kick_mag_min = native.adsViewKickMagMin;
			data.use_angular_view_kick = native.useAngularViewKick;
			data.hip_angular_view_kick_dir = std::to_array(native.hipAngularViewKickDir);
			data.hip_angular_view_kick_dev = std::to_array(native.hipAngularViewKickDev);
			data.hip_angular_view_kick_strength_min = std::to_array(native.hipAngularViewKickStrengthMin);
			data.hip_angular_view_kick_strength_max = std::to_array(native.hipAngularViewKickStrengthMax);
			data.hip_angular_view_kick_pitch_scale = std::to_array(native.hipAngularViewKickPitchScale);
			data.ads_angular_view_kick_dir = std::to_array(native.adsAngularViewKickDir);
			data.ads_angular_view_kick_dev = std::to_array(native.adsAngularViewKickDev);
			data.ads_angular_view_kick_strength_min = std::to_array(native.adsAngularViewKickStrengthMin);
			data.ads_angular_view_kick_strength_max = std::to_array(native.adsAngularViewKickStrengthMax);
			data.ads_angular_view_kick_pitch_scale = std::to_array(native.adsAngularViewKickPitchScale);
			data.ads_angular_view_kick_bullet = std::to_array(native.adsAngularViewKickBullet);
			data.ads_angular_view_kick_use_set = std::to_array(native.adsAngularViewKickUseSet);
			data.use_new_view_kick = native.useNewViewKick;
			data.hip_view_kick_center_speed = native.hipViewKickCenterSpeed;
			data.ads_view_kick_center_speed = native.adsViewKickCenterSpeed;
			data.hip_view_kick_return_accel_scale = native.hipViewKickReturnAccelScale;
			data.ads_view_kick_return_accel_scale = native.adsViewKickReturnAccelScale;
			data.hip_view_kick_return_speed_curve_scale = native.hipViewKickReturnSpeedCurveScale;
			data.ads_view_kick_return_speed_curve_scale = native.adsViewKickReturnSpeedCurveScale;
			data.kick_aligned_input_scalar = native.kickAlignedInputScalar;
			data.kick_opposed_input_scalar = native.kickOpposedInputScalar;
			data.view_kick_maintain_fraction = native.viewKickMaintainFraction;
			data.view_max_pitch = native.viewMaxPitch;
			data.view_max_yaw = native.viewMaxYaw;
			return data;
		}

		void write_att_view_kick(AttViewKick& native, const asset_data::content::att_view_kick& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.hipViewKickPitchMin = data.hip_view_kick_pitch_min;
			native.hipViewKickPitchMax = data.hip_view_kick_pitch_max;
			native.hipViewKickYawMin = data.hip_view_kick_yaw_min;
			native.hipViewKickYawMax = data.hip_view_kick_yaw_max;
			native.hipViewKickMagMin = data.hip_view_kick_mag_min;
			native.adsViewKickPitchMin = data.ads_view_kick_pitch_min;
			native.adsViewKickPitchMax = data.ads_view_kick_pitch_max;
			native.adsViewKickYawMin = data.ads_view_kick_yaw_min;
			native.adsViewKickYawMax = data.ads_view_kick_yaw_max;
			native.adsViewKickMagMin = data.ads_view_kick_mag_min;
			native.useAngularViewKick = data.use_angular_view_kick;
			std::ranges::copy(data.hip_angular_view_kick_dir, native.hipAngularViewKickDir);
			std::ranges::copy(data.hip_angular_view_kick_dev, native.hipAngularViewKickDev);
			std::ranges::copy(data.hip_angular_view_kick_strength_min, native.hipAngularViewKickStrengthMin);
			std::ranges::copy(data.hip_angular_view_kick_strength_max, native.hipAngularViewKickStrengthMax);
			std::ranges::copy(data.hip_angular_view_kick_pitch_scale, native.hipAngularViewKickPitchScale);
			std::ranges::copy(data.ads_angular_view_kick_dir, native.adsAngularViewKickDir);
			std::ranges::copy(data.ads_angular_view_kick_dev, native.adsAngularViewKickDev);
			std::ranges::copy(data.ads_angular_view_kick_strength_min, native.adsAngularViewKickStrengthMin);
			std::ranges::copy(data.ads_angular_view_kick_strength_max, native.adsAngularViewKickStrengthMax);
			std::ranges::copy(data.ads_angular_view_kick_pitch_scale, native.adsAngularViewKickPitchScale);
			std::ranges::copy(data.ads_angular_view_kick_bullet, native.adsAngularViewKickBullet);
			std::ranges::copy(data.ads_angular_view_kick_use_set, native.adsAngularViewKickUseSet);
			native.useNewViewKick = data.use_new_view_kick;
			native.hipViewKickCenterSpeed = data.hip_view_kick_center_speed;
			native.adsViewKickCenterSpeed = data.ads_view_kick_center_speed;
			native.hipViewKickReturnAccelScale = data.hip_view_kick_return_accel_scale;
			native.adsViewKickReturnAccelScale = data.ads_view_kick_return_accel_scale;
			native.hipViewKickReturnSpeedCurveScale = data.hip_view_kick_return_speed_curve_scale;
			native.adsViewKickReturnSpeedCurveScale = data.ads_view_kick_return_speed_curve_scale;
			native.kickAlignedInputScalar = data.kick_aligned_input_scalar;
			native.kickOpposedInputScalar = data.kick_opposed_input_scalar;
			native.viewKickMaintainFraction = data.view_kick_maintain_fraction;
			native.viewMaxPitch = data.view_max_pitch;
			native.viewMaxYaw = data.view_max_yaw;
		}

		std::size_t size_att_view_kick([[maybe_unused]] const asset_data::content::att_view_kick& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::ads_overlay read_ads_overlay(const ADSOverlay& native)
		{
			plain::trace("read_ads_overlay");
			asset_data::content::ads_overlay data;
			data.shader_mat = plain::read_asset(ASSET_TYPE_MATERIAL, native.shaderMat);
			data.shader_low_res_mat = plain::read_asset(ASSET_TYPE_MATERIAL, native.shaderLowResMat);
			data.shader_emp_mat = plain::read_asset(ASSET_TYPE_MATERIAL, native.shaderEMPMat);
			data.shader_emp_low_res_mat = plain::read_asset(ASSET_TYPE_MATERIAL, native.shaderEMPLowResMat);
			data.shader_img = plain::read_asset(ASSET_TYPE_IMAGE, native.shaderImg);
			data.shader_low_res_img = plain::read_asset(ASSET_TYPE_IMAGE, native.shaderLowResImg);
			data.shader_emp_img = plain::read_asset(ASSET_TYPE_IMAGE, native.shaderEMPImg);
			data.shader_emp_low_res_img = plain::read_asset(ASSET_TYPE_IMAGE, native.shaderEMPLowResImg);
			data.apply_vision_set = native.applyVisionSet;
			data.reticle = plain::enum_name(asset_data::content::weap_overlay_reticle_t_names, static_cast<std::int64_t>(native.reticle), "ADSOverlay.reticle");
			data.width = native.width;
			data.height = native.height;
			data.width_splitscreen = native.widthSplitscreen;
			data.height_splitscreen = native.heightSplitscreen;
			data.vision_set_name = plain::read_string(native.visionSetName);
			data.vision_set_ads_fraction = native.visionSetADSFraction;
			data.vision_set_blend_in_time_ms = native.visionSetBlendInTimeMs;
			data.vision_set_blend_out_time_ms = native.visionSetBlendOutTimeMs;
			return data;
		}

		void write_ads_overlay(ADSOverlay& native, const asset_data::content::ads_overlay& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.shaderMat = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, data.shader_mat);
			native.shaderLowResMat = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, data.shader_low_res_mat);
			native.shaderEMPMat = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, data.shader_emp_mat);
			native.shaderEMPLowResMat = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, data.shader_emp_low_res_mat);
			native.shaderImg = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, data.shader_img);
			native.shaderLowResImg = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, data.shader_low_res_img);
			native.shaderEMPImg = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, data.shader_emp_img);
			native.shaderEMPLowResImg = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, data.shader_emp_low_res_img);
			native.applyVisionSet = data.apply_vision_set;
			native.reticle = static_cast<weapOverlayReticle_t>(plain::enum_value(asset_data::content::weap_overlay_reticle_t_names, data.reticle, "ADSOverlay.reticle"));
			native.width = data.width;
			native.height = data.height;
			native.widthSplitscreen = data.width_splitscreen;
			native.heightSplitscreen = data.height_splitscreen;
			native.visionSetName = plain::write_string(memory, data.vision_set_name);
			native.visionSetADSFraction = data.vision_set_ads_fraction;
			native.visionSetBlendInTimeMs = data.vision_set_blend_in_time_ms;
			native.visionSetBlendOutTimeMs = data.vision_set_blend_out_time_ms;
		}

		std::size_t size_ads_overlay([[maybe_unused]] const asset_data::content::ads_overlay& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.vision_set_name);
			return size;
		}

		asset_data::content::att_ads_overlay read_att_ads_overlay(const AttADSOverlay& native)
		{
			plain::trace("read_att_ads_overlay");
			asset_data::content::att_ads_overlay data;
			data.overlay = read_ads_overlay(native.overlay);
			data.hybrid_scope_attachment = plain::read_asset(ASSET_TYPE_ATTACHMENT, native.hybridScopeAttachment);
			data.hybrid_scope1_to2_fov_trans_begin = native.hybridScope1To2FovTransBegin;
			data.hybrid_scope1_to2_fov_trans_end = native.hybridScope1To2FovTransEnd;
			data.hybrid_scope2_to1_fov_trans_begin = native.hybridScope2To1FovTransBegin;
			data.hybrid_scope2_to1_fov_trans_end = native.hybridScope2To1FovTransEnd;
			data.hybrid_scope1_to2_scope_fade_trans_begin = native.hybridScope1To2ScopeFadeTransBegin;
			data.hybrid_scope1_to2_scope_fade_trans_end = native.hybridScope1To2ScopeFadeTransEnd;
			data.hybrid_scope2_to1_scope_fade_trans_begin = native.hybridScope2To1ScopeFadeTransBegin;
			data.hybrid_scope2_to1_scope_fade_trans_end = native.hybridScope2To1ScopeFadeTransEnd;
			data.hybrid_scope1_show_hip_glass_when_inactive = native.hybridScope1ShowHipGlassWhenInactive;
			data.hybrid_scope2_show_hip_glass_when_inactive = native.hybridScope2ShowHipGlassWhenInactive;
			data.hybrid_toggle = native.hybridToggle;
			data.thermal_scope = native.thermalScope;
			data.thermal_toggle = native.thermalToggle;
			return data;
		}

		void write_att_ads_overlay(AttADSOverlay& native, const asset_data::content::att_ads_overlay& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_ads_overlay(native.overlay, data.overlay, resolve, memory);
			native.hybridScopeAttachment = plain::resolve_asset<WeaponAttachment>(resolve, ASSET_TYPE_ATTACHMENT, data.hybrid_scope_attachment);
			native.hybridScope1To2FovTransBegin = data.hybrid_scope1_to2_fov_trans_begin;
			native.hybridScope1To2FovTransEnd = data.hybrid_scope1_to2_fov_trans_end;
			native.hybridScope2To1FovTransBegin = data.hybrid_scope2_to1_fov_trans_begin;
			native.hybridScope2To1FovTransEnd = data.hybrid_scope2_to1_fov_trans_end;
			native.hybridScope1To2ScopeFadeTransBegin = data.hybrid_scope1_to2_scope_fade_trans_begin;
			native.hybridScope1To2ScopeFadeTransEnd = data.hybrid_scope1_to2_scope_fade_trans_end;
			native.hybridScope2To1ScopeFadeTransBegin = data.hybrid_scope2_to1_scope_fade_trans_begin;
			native.hybridScope2To1ScopeFadeTransEnd = data.hybrid_scope2_to1_scope_fade_trans_end;
			native.hybridScope1ShowHipGlassWhenInactive = data.hybrid_scope1_show_hip_glass_when_inactive;
			native.hybridScope2ShowHipGlassWhenInactive = data.hybrid_scope2_show_hip_glass_when_inactive;
			native.hybridToggle = data.hybrid_toggle;
			native.thermalScope = data.thermal_scope;
			native.thermalToggle = data.thermal_toggle;
		}

		std::size_t size_att_ads_overlay([[maybe_unused]] const asset_data::content::att_ads_overlay& data)
		{
			std::size_t size = 0;
			size += size_ads_overlay(data.overlay);
			return size;
		}

		asset_data::content::att_outline read_att_outline(const AttOutline& native)
		{
			plain::trace("read_att_outline");
			asset_data::content::att_outline data;
			data.outline_color = plain::read_scalars<float, 4>(&native.outlineColor);
			data.outline_alt_color = plain::read_scalars<float, 4>(&native.outlineAltColor);
			data.outline_fill_color0 = plain::read_scalars<float, 4>(&native.outlineFillColor0);
			data.outline_fill_color1 = plain::read_scalars<float, 4>(&native.outlineFillColor1);
			data.outline_occluded_outline_color = plain::read_scalars<float, 4>(&native.outlineOccludedOutlineColor);
			data.outline_occluded_inline_color = plain::read_scalars<float, 4>(&native.outlineOccludedInlineColor);
			data.outline_occluded_interior_color = plain::read_scalars<float, 4>(&native.outlineOccludedInteriorColor);
			data.outline_width = native.outlineWidth;
			data.outline_enemies = native.outlineEnemies;
			data.outline_depth_test = native.outlineDepthTest;
			data.outline_fill = native.outlineFill;
			data.outline_script_controlled = native.outlineScriptControlled;
			data.outline_vehicles = native.outlineVehicles;
			data.outline_turrets = native.outlineTurrets;
			data.outline_characters = native.outlineCharacters;
			data.outline_agents = native.outlineAgents;
			data.outline_script_movers = native.outlineScriptMovers;
			data.outline_lock_on = native.outlineLockOn;
			return data;
		}

		void write_att_outline(AttOutline& native, const asset_data::content::att_outline& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.outlineColor, data.outline_color);
			plain::write_scalars(&native.outlineAltColor, data.outline_alt_color);
			plain::write_scalars(&native.outlineFillColor0, data.outline_fill_color0);
			plain::write_scalars(&native.outlineFillColor1, data.outline_fill_color1);
			plain::write_scalars(&native.outlineOccludedOutlineColor, data.outline_occluded_outline_color);
			plain::write_scalars(&native.outlineOccludedInlineColor, data.outline_occluded_inline_color);
			plain::write_scalars(&native.outlineOccludedInteriorColor, data.outline_occluded_interior_color);
			native.outlineWidth = data.outline_width;
			native.outlineEnemies = data.outline_enemies;
			native.outlineDepthTest = data.outline_depth_test;
			native.outlineFill = data.outline_fill;
			native.outlineScriptControlled = data.outline_script_controlled;
			native.outlineVehicles = data.outline_vehicles;
			native.outlineTurrets = data.outline_turrets;
			native.outlineCharacters = data.outline_characters;
			native.outlineAgents = data.outline_agents;
			native.outlineScriptMovers = data.outline_script_movers;
			native.outlineLockOn = data.outline_lock_on;
		}

		std::size_t size_att_outline([[maybe_unused]] const asset_data::content::att_outline& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::att_ui read_att_ui(const AttUI& native)
		{
			plain::trace("read_att_ui");
			asset_data::content::att_ui data;
			data.hud_icon = plain::read_asset(ASSET_TYPE_IMAGE, native.hudIcon);
			data.pickup_icon = plain::read_asset(ASSET_TYPE_IMAGE, native.pickupIcon);
			data.danger_icon_img = plain::read_asset(ASSET_TYPE_IMAGE, native.dangerIconImg);
			data.throw_back_icon_img = plain::read_asset(ASSET_TYPE_IMAGE, native.throwBackIconImg);
			data.danger_icon_mat = plain::read_asset(ASSET_TYPE_MATERIAL, native.dangerIconMat);
			data.throw_back_icon_mat = plain::read_asset(ASSET_TYPE_MATERIAL, native.throwBackIconMat);
			data.hide_warning_icons = native.hideWarningIcons;
			data.warning_icons_delay = native.warningIconsDelay;
			data.dpad_icon_img = plain::read_asset(ASSET_TYPE_IMAGE, native.dpadIconImg);
			data.dpad_icon_inactive_img = plain::read_asset(ASSET_TYPE_IMAGE, native.dpadIconInactiveImg);
			data.dpad_icon_mat = plain::read_asset(ASSET_TYPE_MATERIAL, native.dpadIconMat);
			data.dpad_icon_inactive_mat = plain::read_asset(ASSET_TYPE_MATERIAL, native.dpadIconInactiveMat);
			data.ammo_counter_clip = plain::enum_name(asset_data::content::ammo_counter_clip_type_t_names, static_cast<std::int64_t>(native.ammoCounterClip), "AttUI.ammoCounterClip");
			data.grenade_danger_icon_distance = native.grenadeDangerIconDistance;
			return data;
		}

		void write_att_ui(AttUI& native, const asset_data::content::att_ui& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.hudIcon = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, data.hud_icon);
			native.pickupIcon = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, data.pickup_icon);
			native.dangerIconImg = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, data.danger_icon_img);
			native.throwBackIconImg = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, data.throw_back_icon_img);
			native.dangerIconMat = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, data.danger_icon_mat);
			native.throwBackIconMat = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, data.throw_back_icon_mat);
			native.hideWarningIcons = data.hide_warning_icons;
			native.warningIconsDelay = data.warning_icons_delay;
			native.dpadIconImg = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, data.dpad_icon_img);
			native.dpadIconInactiveImg = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, data.dpad_icon_inactive_img);
			native.dpadIconMat = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, data.dpad_icon_mat);
			native.dpadIconInactiveMat = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, data.dpad_icon_inactive_mat);
			native.ammoCounterClip = static_cast<ammoCounterClipType_t>(plain::enum_value(asset_data::content::ammo_counter_clip_type_t_names, data.ammo_counter_clip, "AttUI.ammoCounterClip"));
			native.grenadeDangerIconDistance = data.grenade_danger_icon_distance;
		}

		std::size_t size_att_ui([[maybe_unused]] const asset_data::content::att_ui& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::att_rumbles read_att_rumbles(const AttRumbles& native)
		{
			plain::trace("read_att_rumbles");
			asset_data::content::att_rumbles data;
			data.fire_rumble = plain::read_asset(ASSET_TYPE_RUMBLE, native.fireRumble);
			data.melee_impact_rumble = plain::read_asset(ASSET_TYPE_RUMBLE, native.meleeImpactRumble);
			return data;
		}

		void write_att_rumbles(AttRumbles& native, const asset_data::content::att_rumbles& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.fireRumble = plain::resolve_asset<RumbleInfo>(resolve, ASSET_TYPE_RUMBLE, data.fire_rumble);
			native.meleeImpactRumble = plain::resolve_asset<RumbleInfo>(resolve, ASSET_TYPE_RUMBLE, data.melee_impact_rumble);
		}

		std::size_t size_att_rumbles([[maybe_unused]] const asset_data::content::att_rumbles& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::fx_combined_def read_fx_combined_def(const FxCombinedDef& native)
		{
			plain::trace("read_fx_combined_def");
			asset_data::content::fx_combined_def data;
			data.particle_system_def = plain::read_asset(ASSET_TYPE_VFX, native.particleSystemDef);
			return data;
		}

		void write_fx_combined_def(FxCombinedDef& native, const asset_data::content::fx_combined_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.particleSystemDef = plain::resolve_asset<ParticleSystemDef>(resolve, ASSET_TYPE_VFX, data.particle_system_def);
		}

		std::size_t size_fx_combined_def([[maybe_unused]] const asset_data::content::fx_combined_def& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::snd_alias_lookup read_snd_alias_lookup(const SndAliasLookup& native)
		{
			plain::trace("read_snd_alias_lookup");
			asset_data::content::snd_alias_lookup data;
			data.name = plain::read_string(native.name);
			return data;
		}

		void write_snd_alias_lookup(SndAliasLookup& native, const asset_data::content::snd_alias_lookup& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
		}

		std::size_t size_snd_alias_lookup([[maybe_unused]] const asset_data::content::snd_alias_lookup& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			return size;
		}

		asset_data::content::att_projectile read_att_projectile(const AttProjectile& native)
		{
			plain::trace("read_att_projectile");
			asset_data::content::att_projectile data;
			data.explosion_radius = native.explosionRadius;
			data.explosion_force_radius = native.explosionForceRadius;
			data.explosion_inner_damage = native.explosionInnerDamage;
			data.explosion_outer_damage = native.explosionOuterDamage;
			data.damage_cone_angle = native.damageConeAngle;
			data.projectile_speed = native.projectileSpeed;
			data.projectile_speed_up = native.projectileSpeedUp;
			data.projectile_activate_dist = native.projectileActivateDist;
			data.projectile_lifetime = native.projectileLifetime;
			data.projectile_lifetime_std_deviation = native.projectileLifetimeStdDeviation;
			data.projectile_model = plain::read_asset(ASSET_TYPE_XMODEL, native.projectileModel);
			data.proj_explosion_type = plain::enum_name(asset_data::content::weap_proj_exposion_t_names, static_cast<std::int64_t>(native.projExplosionType), "AttProjectile.projExplosionType");
			data.proj_explosion_effect = read_fx_combined_def(native.projExplosionEffect);
			data.proj_explosion_effect_force_normal_up = native.projExplosionEffectForceNormalUp;
			data.proj_explosion_sound = read_snd_alias_lookup(native.projExplosionSound);
			data.proj_explosion_refl_class = native.projExplosionReflClass;
			data.proj_dud_effect = read_fx_combined_def(native.projDudEffect);
			data.proj_timeout_effect = read_fx_combined_def(native.projTimeoutEffect);
			data.proj_dud_sound = read_snd_alias_lookup(native.projDudSound);
			data.proj_timeout_sound = read_snd_alias_lookup(native.projTimeoutSound);
			data.disable_fx_on_timeout = native.disableFxOnTimeout;
			data.proj_impact_explode = native.projImpactExplode;
			data.disable_projectile_crumple_check = native.disableProjectileCrumpleCheck;
			data.destabilization_rate_time = native.destabilizationRateTime;
			data.destabilization_curvature_max = native.destabilizationCurvatureMax;
			data.destabilize_distance = native.destabilizeDistance;
			data.vm_proj_body_effect = read_fx_combined_def(native.vmProjBodyEffect);
			data.proj_body_effect = read_fx_combined_def(native.projBodyEffect);
			data.proj_trail_effect = read_fx_combined_def(native.projTrailEffect);
			data.proj_beacon_effect = read_fx_combined_def(native.projBeaconEffect);
			data.proj_ignition_delay = native.projIgnitionDelay;
			data.proj_ignition_effect = read_fx_combined_def(native.projIgnitionEffect);
			data.proj_ignition_sound = read_snd_alias_lookup(native.projIgnitionSound);
			data.stickiness = plain::enum_name(asset_data::content::weap_stickiness_type_names, static_cast<std::int64_t>(native.stickiness), "AttProjectile.stickiness");
			data.fuse_time = native.fuseTime;
			data.ai_fuse_time = native.aiFuseTime;
			data.stick_to_players = native.stickToPlayers;
			data.stick_to_vehicles = native.stickToVehicles;
			data.stick_to_turrets = native.stickToTurrets;
			data.stick_to_non_stick = native.stickToNonStick;
			data.timed_detonation = native.timedDetonation;
			data.is_predicted_projectile = native.isPredictedProjectile;
			data.use_stepped_explosion_damage = native.useSteppedExplosionDamage;
			data.client_trajectory_blend_out_time = native.clientTrajectoryBlendOutTime;
			data.server_trajectory_blend_in_time = native.serverTrajectoryBlendInTime;
			data.reactive_motion_radius_scale = native.reactiveMotionRadiusScale;
			data.reactive_motion_frequency_scale = native.reactiveMotionFrequencyScale;
			data.reactive_motion_amplitude_scale = native.reactiveMotionAmplitudeScale;
			data.reactive_motion_falloff = native.reactiveMotionFalloff;
			data.reactive_motion_lifetime = native.reactiveMotionLifetime;
			return data;
		}

		void write_att_projectile(AttProjectile& native, const asset_data::content::att_projectile& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.explosionRadius = data.explosion_radius;
			native.explosionForceRadius = data.explosion_force_radius;
			native.explosionInnerDamage = data.explosion_inner_damage;
			native.explosionOuterDamage = data.explosion_outer_damage;
			native.damageConeAngle = data.damage_cone_angle;
			native.projectileSpeed = data.projectile_speed;
			native.projectileSpeedUp = data.projectile_speed_up;
			native.projectileActivateDist = data.projectile_activate_dist;
			native.projectileLifetime = data.projectile_lifetime;
			native.projectileLifetimeStdDeviation = data.projectile_lifetime_std_deviation;
			native.projectileModel = plain::resolve_asset<XModel>(resolve, ASSET_TYPE_XMODEL, data.projectile_model);
			native.projExplosionType = static_cast<weapProjExposion_t>(plain::enum_value(asset_data::content::weap_proj_exposion_t_names, data.proj_explosion_type, "AttProjectile.projExplosionType"));
			write_fx_combined_def(native.projExplosionEffect, data.proj_explosion_effect, resolve, memory);
			native.projExplosionEffectForceNormalUp = data.proj_explosion_effect_force_normal_up;
			write_snd_alias_lookup(native.projExplosionSound, data.proj_explosion_sound, resolve, memory);
			native.projExplosionReflClass = data.proj_explosion_refl_class;
			write_fx_combined_def(native.projDudEffect, data.proj_dud_effect, resolve, memory);
			write_fx_combined_def(native.projTimeoutEffect, data.proj_timeout_effect, resolve, memory);
			write_snd_alias_lookup(native.projDudSound, data.proj_dud_sound, resolve, memory);
			write_snd_alias_lookup(native.projTimeoutSound, data.proj_timeout_sound, resolve, memory);
			native.disableFxOnTimeout = data.disable_fx_on_timeout;
			native.projImpactExplode = data.proj_impact_explode;
			native.disableProjectileCrumpleCheck = data.disable_projectile_crumple_check;
			native.destabilizationRateTime = data.destabilization_rate_time;
			native.destabilizationCurvatureMax = data.destabilization_curvature_max;
			native.destabilizeDistance = data.destabilize_distance;
			write_fx_combined_def(native.vmProjBodyEffect, data.vm_proj_body_effect, resolve, memory);
			write_fx_combined_def(native.projBodyEffect, data.proj_body_effect, resolve, memory);
			write_fx_combined_def(native.projTrailEffect, data.proj_trail_effect, resolve, memory);
			write_fx_combined_def(native.projBeaconEffect, data.proj_beacon_effect, resolve, memory);
			native.projIgnitionDelay = data.proj_ignition_delay;
			write_fx_combined_def(native.projIgnitionEffect, data.proj_ignition_effect, resolve, memory);
			write_snd_alias_lookup(native.projIgnitionSound, data.proj_ignition_sound, resolve, memory);
			native.stickiness = static_cast<WeapStickinessType>(plain::enum_value(asset_data::content::weap_stickiness_type_names, data.stickiness, "AttProjectile.stickiness"));
			native.fuseTime = data.fuse_time;
			native.aiFuseTime = data.ai_fuse_time;
			native.stickToPlayers = data.stick_to_players;
			native.stickToVehicles = data.stick_to_vehicles;
			native.stickToTurrets = data.stick_to_turrets;
			native.stickToNonStick = data.stick_to_non_stick;
			native.timedDetonation = data.timed_detonation;
			native.isPredictedProjectile = data.is_predicted_projectile;
			native.useSteppedExplosionDamage = data.use_stepped_explosion_damage;
			native.clientTrajectoryBlendOutTime = data.client_trajectory_blend_out_time;
			native.serverTrajectoryBlendInTime = data.server_trajectory_blend_in_time;
			native.reactiveMotionRadiusScale = data.reactive_motion_radius_scale;
			native.reactiveMotionFrequencyScale = data.reactive_motion_frequency_scale;
			native.reactiveMotionAmplitudeScale = data.reactive_motion_amplitude_scale;
			native.reactiveMotionFalloff = data.reactive_motion_falloff;
			native.reactiveMotionLifetime = data.reactive_motion_lifetime;
		}

		std::size_t size_att_projectile([[maybe_unused]] const asset_data::content::att_projectile& data)
		{
			std::size_t size = 0;
			size += size_fx_combined_def(data.proj_explosion_effect);
			size += size_snd_alias_lookup(data.proj_explosion_sound);
			size += size_fx_combined_def(data.proj_dud_effect);
			size += size_fx_combined_def(data.proj_timeout_effect);
			size += size_snd_alias_lookup(data.proj_dud_sound);
			size += size_snd_alias_lookup(data.proj_timeout_sound);
			size += size_fx_combined_def(data.vm_proj_body_effect);
			size += size_fx_combined_def(data.proj_body_effect);
			size += size_fx_combined_def(data.proj_trail_effect);
			size += size_fx_combined_def(data.proj_beacon_effect);
			size += size_fx_combined_def(data.proj_ignition_effect);
			size += size_snd_alias_lookup(data.proj_ignition_sound);
			return size;
		}

		asset_data::content::att_charged read_att_charged(const AttCharged& native)
		{
			plain::trace("read_att_charged");
			asset_data::content::att_charged data;
			data.charge_type = static_cast<std::uint32_t>(native.chargeType);
			data.charge_gain = native.chargeGain;
			data.charge_cost_per_shot = native.chargeCostPerShot;
			data.charge_loss_when_idle = native.chargeLossWhenIdle;
			data.charge_empty_cooldown = native.chargeEmptyCooldown;
			data.charge_fire_at_max_damage_multiplier = native.chargeFireAtMaxDamageMultiplier;
			data.charge_meter_effect_bone_count = native.chargeMeterEffectBoneCount;
			data.charge_meter_effect = read_fx_combined_def(native.chargeMeterEffect);
			data.charge_up_sound = read_snd_alias_lookup(native.chargeUpSound);
			data.charge_down_sound = read_snd_alias_lookup(native.chargeDownSound);
			data.charge_down_to_up_sound = read_snd_alias_lookup(native.chargeDownToUpSound);
			data.charge_up_to_down_sound = read_snd_alias_lookup(native.chargeUpToDownSound);
			data.charge_max_sound = read_snd_alias_lookup(native.chargeMaxSound);
			data.charge_up_sound_player = read_snd_alias_lookup(native.chargeUpSoundPlayer);
			data.charge_down_sound_player = read_snd_alias_lookup(native.chargeDownSoundPlayer);
			data.charge_down_to_up_sound_player = read_snd_alias_lookup(native.chargeDownToUpSoundPlayer);
			data.charge_up_to_down_sound_player = read_snd_alias_lookup(native.chargeUpToDownSoundPlayer);
			data.charge_max_sound_player = read_snd_alias_lookup(native.chargeMaxSoundPlayer);
			data.charge_hud_reveal = native.chargeHudReveal;
			data.charge_rumble = plain::read_asset(ASSET_TYPE_RUMBLE, native.chargeRumble);
			return data;
		}

		void write_att_charged(AttCharged& native, const asset_data::content::att_charged& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.chargeType = static_cast<WeaponChargeType>(data.charge_type);
			native.chargeGain = data.charge_gain;
			native.chargeCostPerShot = data.charge_cost_per_shot;
			native.chargeLossWhenIdle = data.charge_loss_when_idle;
			native.chargeEmptyCooldown = data.charge_empty_cooldown;
			native.chargeFireAtMaxDamageMultiplier = data.charge_fire_at_max_damage_multiplier;
			native.chargeMeterEffectBoneCount = data.charge_meter_effect_bone_count;
			write_fx_combined_def(native.chargeMeterEffect, data.charge_meter_effect, resolve, memory);
			write_snd_alias_lookup(native.chargeUpSound, data.charge_up_sound, resolve, memory);
			write_snd_alias_lookup(native.chargeDownSound, data.charge_down_sound, resolve, memory);
			write_snd_alias_lookup(native.chargeDownToUpSound, data.charge_down_to_up_sound, resolve, memory);
			write_snd_alias_lookup(native.chargeUpToDownSound, data.charge_up_to_down_sound, resolve, memory);
			write_snd_alias_lookup(native.chargeMaxSound, data.charge_max_sound, resolve, memory);
			write_snd_alias_lookup(native.chargeUpSoundPlayer, data.charge_up_sound_player, resolve, memory);
			write_snd_alias_lookup(native.chargeDownSoundPlayer, data.charge_down_sound_player, resolve, memory);
			write_snd_alias_lookup(native.chargeDownToUpSoundPlayer, data.charge_down_to_up_sound_player, resolve, memory);
			write_snd_alias_lookup(native.chargeUpToDownSoundPlayer, data.charge_up_to_down_sound_player, resolve, memory);
			write_snd_alias_lookup(native.chargeMaxSoundPlayer, data.charge_max_sound_player, resolve, memory);
			native.chargeHudReveal = data.charge_hud_reveal;
			native.chargeRumble = plain::resolve_asset<RumbleInfo>(resolve, ASSET_TYPE_RUMBLE, data.charge_rumble);
		}

		std::size_t size_att_charged([[maybe_unused]] const asset_data::content::att_charged& data)
		{
			std::size_t size = 0;
			size += size_fx_combined_def(data.charge_meter_effect);
			size += size_snd_alias_lookup(data.charge_up_sound);
			size += size_snd_alias_lookup(data.charge_down_sound);
			size += size_snd_alias_lookup(data.charge_down_to_up_sound);
			size += size_snd_alias_lookup(data.charge_up_to_down_sound);
			size += size_snd_alias_lookup(data.charge_max_sound);
			size += size_snd_alias_lookup(data.charge_up_sound_player);
			size += size_snd_alias_lookup(data.charge_down_sound_player);
			size += size_snd_alias_lookup(data.charge_down_to_up_sound_player);
			size += size_snd_alias_lookup(data.charge_up_to_down_sound_player);
			size += size_snd_alias_lookup(data.charge_max_sound_player);
			return size;
		}

		asset_data::content::ads_alt_switch read_ads_alt_switch(const AdsAltSwitch& native)
		{
			plain::trace("read_ads_alt_switch");
			asset_data::content::ads_alt_switch data;
			data.fov_interp_time_sec_main_to_alt = native.fovInterpTimeSecMainToAlt;
			data.fov_interp_delay_sec_main_to_alt = native.fovInterpDelaySecMainToAlt;
			data.dual_fov_switch_rate_main_to_alt = native.dualFovSwitchRateMainToAlt;
			data.fov_interp_type_main_to_alt = plain::enum_name(asset_data::content::ads_alt_switch_interp_type_names, static_cast<std::int64_t>(native.fovInterpTypeMainToAlt), "AdsAltSwitch.fovInterpTypeMainToAlt");
			data.fov_interp_time_sec_alt_to_main = native.fovInterpTimeSecAltToMain;
			data.fov_interp_delay_sec_alt_to_main = native.fovInterpDelaySecAltToMain;
			data.dual_fov_switch_rate_alt_to_main = native.dualFovSwitchRateAltToMain;
			data.fov_interp_type_alt_to_main = plain::enum_name(asset_data::content::ads_alt_switch_interp_type_names, static_cast<std::int64_t>(native.fovInterpTypeAltToMain), "AdsAltSwitch.fovInterpTypeAltToMain");
			return data;
		}

		void write_ads_alt_switch(AdsAltSwitch& native, const asset_data::content::ads_alt_switch& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.fovInterpTimeSecMainToAlt = data.fov_interp_time_sec_main_to_alt;
			native.fovInterpDelaySecMainToAlt = data.fov_interp_delay_sec_main_to_alt;
			native.dualFovSwitchRateMainToAlt = data.dual_fov_switch_rate_main_to_alt;
			native.fovInterpTypeMainToAlt = static_cast<AdsAltSwitchInterpType>(plain::enum_value(asset_data::content::ads_alt_switch_interp_type_names, data.fov_interp_type_main_to_alt, "AdsAltSwitch.fovInterpTypeMainToAlt"));
			native.fovInterpTimeSecAltToMain = data.fov_interp_time_sec_alt_to_main;
			native.fovInterpDelaySecAltToMain = data.fov_interp_delay_sec_alt_to_main;
			native.dualFovSwitchRateAltToMain = data.dual_fov_switch_rate_alt_to_main;
			native.fovInterpTypeAltToMain = static_cast<AdsAltSwitchInterpType>(plain::enum_value(asset_data::content::ads_alt_switch_interp_type_names, data.fov_interp_type_alt_to_main, "AdsAltSwitch.fovInterpTypeAltToMain"));
		}

		std::size_t size_ads_alt_switch([[maybe_unused]] const asset_data::content::ads_alt_switch& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::att_regeneration read_att_regeneration(const AttRegeneration& native)
		{
			plain::trace("read_att_regeneration");
			asset_data::content::att_regeneration data;
			data.regeneration_enabled = native.regenerationEnabled;
			data.regeneration_time_ms = native.regenerationTimeMs;
			data.regeneration_add_time_ms = native.regenerationAddTimeMs;
			data.regeneration_amount = native.regenerationAmount;
			data.regeneration_consume_stock = native.regenerationConsumeStock;
			data.regeneration_disable_while_firing = native.regenerationDisableWhileFiring;
			return data;
		}

		void write_att_regeneration(AttRegeneration& native, const asset_data::content::att_regeneration& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.regenerationEnabled = data.regeneration_enabled;
			native.regenerationTimeMs = data.regeneration_time_ms;
			native.regenerationAddTimeMs = data.regeneration_add_time_ms;
			native.regenerationAmount = data.regeneration_amount;
			native.regenerationConsumeStock = data.regeneration_consume_stock;
			native.regenerationDisableWhileFiring = data.regeneration_disable_while_firing;
		}

		std::size_t size_att_regeneration([[maybe_unused]] const asset_data::content::att_regeneration& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::att_heat read_att_heat(const AttHeat& native)
		{
			plain::trace("read_att_heat");
			asset_data::content::att_heat data;
			data.heat_accumulation_per_shot = native.heatAccumulationPerShot;
			data.heat_dissipation_per_second = native.heatDissipationPerSecond;
			data.heat_smoke_start_threshold = native.heatSmokeStartThreshold;
			data.heat_smoke_stop_threshold = native.heatSmokeStopThreshold;
			return data;
		}

		void write_att_heat(AttHeat& native, const asset_data::content::att_heat& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.heatAccumulationPerShot = data.heat_accumulation_per_shot;
			native.heatDissipationPerSecond = data.heat_dissipation_per_second;
			native.heatSmokeStartThreshold = data.heat_smoke_start_threshold;
			native.heatSmokeStopThreshold = data.heat_smoke_stop_threshold;
		}

		std::size_t size_att_heat([[maybe_unused]] const asset_data::content::att_heat& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::att_movement read_att_movement(const AttMovement& native)
		{
			plain::trace("read_att_movement");
			asset_data::content::att_movement data;
			data.move_speed_scale = native.moveSpeedScale;
			data.ads_move_speed_scale = native.adsMoveSpeedScale;
			data.fast_ads3_p_anims = native.fastADS3PAnims;
			return data;
		}

		void write_att_movement(AttMovement& native, const asset_data::content::att_movement& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.moveSpeedScale = data.move_speed_scale;
			native.adsMoveSpeedScale = data.ads_move_speed_scale;
			native.fastADS3PAnims = data.fast_ads3_p_anims;
		}

		std::size_t size_att_movement([[maybe_unused]] const asset_data::content::att_movement& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::att_burst read_att_burst(const AttBurst& native)
		{
			plain::trace("read_att_burst");
			asset_data::content::att_burst data;
			data.burst_fire_cooldown = native.burstFireCooldown;
			data.burst_fire_auto = native.burstFireAuto;
			return data;
		}

		void write_att_burst(AttBurst& native, const asset_data::content::att_burst& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.burstFireCooldown = data.burst_fire_cooldown;
			native.burstFireAuto = data.burst_fire_auto;
		}

		std::size_t size_att_burst([[maybe_unused]] const asset_data::content::att_burst& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::att_hitmarker read_att_hitmarker(const AttHitmarker& native)
		{
			plain::trace("read_att_hitmarker");
			asset_data::content::att_hitmarker data;
			data.hitmarker_type = plain::enum_name(asset_data::content::hitmarker_type_names, static_cast<std::int64_t>(native.hitmarkerType), "AttHitmarker.hitmarkerType");
			return data;
		}

		void write_att_hitmarker(AttHitmarker& native, const asset_data::content::att_hitmarker& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.hitmarkerType = static_cast<HitmarkerType>(plain::enum_value(asset_data::content::hitmarker_type_names, data.hitmarker_type, "AttHitmarker.hitmarkerType"));
		}

		std::size_t size_att_hitmarker([[maybe_unused]] const asset_data::content::att_hitmarker& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::att_ads_movement read_att_ads_movement(const AttAdsMovement& native)
		{
			plain::trace("read_att_ads_movement");
			asset_data::content::att_ads_movement data;
			data.forward_offset = native.forwardOffset;
			data.roll = native.roll;
			data.is_override = native.isOverride;
			return data;
		}

		void write_att_ads_movement(AttAdsMovement& native, const asset_data::content::att_ads_movement& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.forwardOffset = data.forward_offset;
			native.roll = data.roll;
			native.isOverride = data.is_override;
		}

		std::size_t size_att_ads_movement([[maybe_unused]] const asset_data::content::att_ads_movement& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::ballistic_info_calculated read_ballistic_info_calculated(const BallisticInfoCalculated& native)
		{
			plain::trace("read_ballistic_info_calculated");
			asset_data::content::ballistic_info_calculated data;
			data.distances = plain::read_array(native.distances, native.numDistanceEntries, "BallisticInfoCalculated.distances", [&](const auto& item) { return item; });
			data.zeroing_angle = native.zeroingAngle;
			return data;
		}

		void write_ballistic_info_calculated(BallisticInfoCalculated& native, const asset_data::content::ballistic_info_calculated& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.distances = plain::write_array<float>(data.distances, memory, [&](auto& out, const auto& in) { out = in; });
			native.zeroingAngle = data.zeroing_angle;
			native.numDistanceEntries = plain::count_of<std::int32_t>(data.distances);
		}

		std::size_t size_ballistic_info_calculated([[maybe_unused]] const asset_data::content::ballistic_info_calculated& data)
		{
			std::size_t size = 0;
			if (data.distances)
			{
				size += plain::sized(std::max<std::size_t>(data.distances->size(), 1) * sizeof(float));
			}
			return size;
		}

		asset_data::content::ballistic_info read_ballistic_info(const BallisticInfo& native)
		{
			plain::trace("read_ballistic_info");
			asset_data::content::ballistic_info data;
			data.muzzle_velocity = native.muzzleVelocity;
			data.ballistic_coefficient = native.ballisticCoefficient;
			data.diameter = native.diameter;
			data.mass = native.mass;
			data.gravity_factor = native.gravityFactor;
			data.zeroing_distance = native.zeroingDistance;
			data.calculated = plain::read_single(native.calculated, [&](const auto& item) { return read_ballistic_info_calculated(item); });
			data.enable_ballistic_trajectory = native.enableBallisticTrajectory;
			data.life_time_ms = native.lifeTimeMs;
			return data;
		}

		void write_ballistic_info(BallisticInfo& native, const asset_data::content::ballistic_info& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.muzzleVelocity = data.muzzle_velocity;
			native.ballisticCoefficient = data.ballistic_coefficient;
			native.diameter = data.diameter;
			native.mass = data.mass;
			native.gravityFactor = data.gravity_factor;
			native.zeroingDistance = data.zeroing_distance;
			native.calculated = plain::write_single<BallisticInfoCalculated>(data.calculated, memory, [&](auto& out, const auto& in) { write_ballistic_info_calculated(out, in, resolve, memory); });
			native.enableBallisticTrajectory = data.enable_ballistic_trajectory;
			native.lifeTimeMs = data.life_time_ms;
		}

		std::size_t size_ballistic_info([[maybe_unused]] const asset_data::content::ballistic_info& data)
		{
			std::size_t size = 0;
			if (data.calculated)
			{
				size += plain::sized(sizeof(BallisticInfoCalculated));
				size += size_ballistic_info_calculated(*data.calculated);
			}
			return size;
		}

		asset_data::content::common_sway_settings read_common_sway_settings(const CommonSwaySettings& native)
		{
			plain::trace("read_common_sway_settings");
			asset_data::content::common_sway_settings data;
			data.max_angle = native.maxAngle;
			data.lerp_speed = native.lerpSpeed;
			data.pitch_scale = native.pitchScale;
			data.yaw_scale = native.yawScale;
			data.horiz_scale = native.horizScale;
			data.vert_scale = native.vertScale;
			data.gun_angle_scale = native.gunAngleScale;
			return data;
		}

		void write_common_sway_settings(CommonSwaySettings& native, const asset_data::content::common_sway_settings& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.maxAngle = data.max_angle;
			native.lerpSpeed = data.lerp_speed;
			native.pitchScale = data.pitch_scale;
			native.yawScale = data.yaw_scale;
			native.horizScale = data.horiz_scale;
			native.vertScale = data.vert_scale;
			native.gunAngleScale = data.gun_angle_scale;
		}

		std::size_t size_common_sway_settings([[maybe_unused]] const asset_data::content::common_sway_settings& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::hip_sway_settings read_hip_sway_settings(const HipSwaySettings& native)
		{
			plain::trace("read_hip_sway_settings");
			asset_data::content::hip_sway_settings data;
			data.common = read_common_sway_settings(native.common);
			data.max_angle_steady_aim = native.maxAngleSteadyAim;
			return data;
		}

		void write_hip_sway_settings(HipSwaySettings& native, const asset_data::content::hip_sway_settings& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_common_sway_settings(native.common, data.common, resolve, memory);
			native.maxAngleSteadyAim = data.max_angle_steady_aim;
		}

		std::size_t size_hip_sway_settings([[maybe_unused]] const asset_data::content::hip_sway_settings& data)
		{
			std::size_t size = 0;
			size += size_common_sway_settings(data.common);
			return size;
		}

		asset_data::content::ads_sway_settings read_ads_sway_settings(const AdsSwaySettings& native)
		{
			plain::trace("read_ads_sway_settings");
			asset_data::content::ads_sway_settings data;
			data.common = read_common_sway_settings(native.common);
			data.sway_transition_lerp_speed = native.swayTransitionLerpSpeed;
			data.ads_sway_scale = std::to_array(native.adsSwayScale);
			return data;
		}

		void write_ads_sway_settings(AdsSwaySettings& native, const asset_data::content::ads_sway_settings& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_common_sway_settings(native.common, data.common, resolve, memory);
			native.swayTransitionLerpSpeed = data.sway_transition_lerp_speed;
			std::ranges::copy(data.ads_sway_scale, native.adsSwayScale);
		}

		std::size_t size_ads_sway_settings([[maybe_unused]] const asset_data::content::ads_sway_settings& data)
		{
			std::size_t size = 0;
			size += size_common_sway_settings(data.common);
			return size;
		}

		asset_data::content::advanced_hip_sway_settings read_advanced_hip_sway_settings(const AdvancedHipSwaySettings& native)
		{
			plain::trace("read_advanced_hip_sway_settings");
			asset_data::content::advanced_hip_sway_settings data;
			data.enabled = native.enabled;
			data.torso_goal_smooth_speed = native.torsoGoalSmoothSpeed;
			data.torso_goal_view_smooth_duration_ms = native.torsoGoalViewSmoothDurationMs;
			data.torso_goal_deadzone_adjust_speed = plain::read_scalars<float, 2>(&native.torsoGoalDeadzoneAdjustSpeed);
			data.torso_goal_view_speed_to_max_deadzone_graph = plain::read_asset(ASSET_TYPE_RUMBLE_GRAPH, native.torsoGoalViewSpeedToMaxDeadzone_graph);
			data.torso_goal_view_speed_to_max_deadzone_viewspeed = plain::read_scalars<float, 2>(&native.torsoGoalViewSpeedToMaxDeadzone_viewspeed);
			data.torso_goal_view_speed_to_max_deadzone_max_deadzone = plain::read_scalars<float, 2>(&native.torsoGoalViewSpeedToMaxDeadzone_maxDeadzone);
			data.torso_mass = plain::read_scalars<float, 2>(&native.torsoMass);
			data.torso_spring = plain::read_scalars<float, 2>(&native.torsoSpring);
			data.torso_damper = plain::read_scalars<float, 2>(&native.torsoDamper);
			data.gun_goal_view_smooth_duration_ms = native.gunGoalViewSmoothDurationMs;
			data.gun_goal_view_speed_to_offset_graph = plain::read_asset(ASSET_TYPE_RUMBLE_GRAPH, native.gunGoalViewSpeedToOffset_graph);
			data.gun_goal_view_speed_to_offset_viewspeed = plain::read_scalars<float, 2>(&native.gunGoalViewSpeedToOffset_viewspeed);
			data.gun_goal_view_speed_to_offset_offset = plain::read_scalars<float, 2>(&native.gunGoalViewSpeedToOffset_offset);
			data.gun_mass = plain::read_scalars<float, 2>(&native.gunMass);
			data.gun_spring = plain::read_scalars<float, 2>(&native.gunSpring);
			data.gun_damper = plain::read_scalars<float, 2>(&native.gunDamper);
			data.gun_pivot_point = plain::read_scalars<float, 3>(&native.gunPivotPoint);
			data.gun_yaw_to_roll_scale = native.gunYawToRollScale;
			data.fire_duration_ms = native.fireDurationMs;
			data.fire_start_blend_duration_ms = native.fireStartBlendDurationMs;
			data.fire_finish_blend_duration_ms = native.fireFinishBlendDurationMs;
			data.fire_torso_goal_smooth_speed = native.fireTorsoGoalSmoothSpeed;
			data.fire_torso_deadzone_scale = native.fireTorsoDeadzoneScale;
			data.fire_torso_to_gun_dir_scale = native.fireTorsoToGunDirScale;
			return data;
		}

		void write_advanced_hip_sway_settings(AdvancedHipSwaySettings& native, const asset_data::content::advanced_hip_sway_settings& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.enabled = data.enabled;
			native.torsoGoalSmoothSpeed = data.torso_goal_smooth_speed;
			native.torsoGoalViewSmoothDurationMs = data.torso_goal_view_smooth_duration_ms;
			plain::write_scalars(&native.torsoGoalDeadzoneAdjustSpeed, data.torso_goal_deadzone_adjust_speed);
			native.torsoGoalViewSpeedToMaxDeadzone_graph = plain::resolve_asset<RumbleGraph>(resolve, ASSET_TYPE_RUMBLE_GRAPH, data.torso_goal_view_speed_to_max_deadzone_graph);
			plain::write_scalars(&native.torsoGoalViewSpeedToMaxDeadzone_viewspeed, data.torso_goal_view_speed_to_max_deadzone_viewspeed);
			plain::write_scalars(&native.torsoGoalViewSpeedToMaxDeadzone_maxDeadzone, data.torso_goal_view_speed_to_max_deadzone_max_deadzone);
			plain::write_scalars(&native.torsoMass, data.torso_mass);
			plain::write_scalars(&native.torsoSpring, data.torso_spring);
			plain::write_scalars(&native.torsoDamper, data.torso_damper);
			native.gunGoalViewSmoothDurationMs = data.gun_goal_view_smooth_duration_ms;
			native.gunGoalViewSpeedToOffset_graph = plain::resolve_asset<RumbleGraph>(resolve, ASSET_TYPE_RUMBLE_GRAPH, data.gun_goal_view_speed_to_offset_graph);
			plain::write_scalars(&native.gunGoalViewSpeedToOffset_viewspeed, data.gun_goal_view_speed_to_offset_viewspeed);
			plain::write_scalars(&native.gunGoalViewSpeedToOffset_offset, data.gun_goal_view_speed_to_offset_offset);
			plain::write_scalars(&native.gunMass, data.gun_mass);
			plain::write_scalars(&native.gunSpring, data.gun_spring);
			plain::write_scalars(&native.gunDamper, data.gun_damper);
			plain::write_scalars(&native.gunPivotPoint, data.gun_pivot_point);
			native.gunYawToRollScale = data.gun_yaw_to_roll_scale;
			native.fireDurationMs = data.fire_duration_ms;
			native.fireStartBlendDurationMs = data.fire_start_blend_duration_ms;
			native.fireFinishBlendDurationMs = data.fire_finish_blend_duration_ms;
			native.fireTorsoGoalSmoothSpeed = data.fire_torso_goal_smooth_speed;
			native.fireTorsoDeadzoneScale = data.fire_torso_deadzone_scale;
			native.fireTorsoToGunDirScale = data.fire_torso_to_gun_dir_scale;
		}

		std::size_t size_advanced_hip_sway_settings([[maybe_unused]] const asset_data::content::advanced_hip_sway_settings& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::sway_settings read_sway_settings(const SwaySettings& native)
		{
			plain::trace("read_sway_settings");
			asset_data::content::sway_settings data;
			data.hip = read_hip_sway_settings(native.hip);
			data.ads = read_ads_sway_settings(native.ads);
			data.adv = read_advanced_hip_sway_settings(native.adv);
			data.shell_shock_scale = native.shellShockScale;
			data.override_hip = native.overrideHip;
			data.override_ads = native.overrideAds;
			return data;
		}

		void write_sway_settings(SwaySettings& native, const asset_data::content::sway_settings& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_hip_sway_settings(native.hip, data.hip, resolve, memory);
			write_ads_sway_settings(native.ads, data.ads, resolve, memory);
			write_advanced_hip_sway_settings(native.adv, data.adv, resolve, memory);
			native.shellShockScale = data.shell_shock_scale;
			native.overrideHip = data.override_hip;
			native.overrideAds = data.override_ads;
		}

		std::size_t size_sway_settings([[maybe_unused]] const asset_data::content::sway_settings& data)
		{
			std::size_t size = 0;
			size += size_hip_sway_settings(data.hip);
			size += size_ads_sway_settings(data.ads);
			size += size_advanced_hip_sway_settings(data.adv);
			return size;
		}

		asset_data::content::att_dlc_overrides read_att_dlc_overrides(const AttDlcOverrides& native)
		{
			plain::trace("read_att_dlc_overrides");
			asset_data::content::att_dlc_overrides data;
			data.use_float_override = std::to_array(native.useFloatOverride);
			data.float_override = std::to_array(native.floatOverride);
			return data;
		}

		void write_att_dlc_overrides(AttDlcOverrides& native, const asset_data::content::att_dlc_overrides& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			std::ranges::copy(data.use_float_override, native.useFloatOverride);
			std::ranges::copy(data.float_override, native.floatOverride);
		}

		std::size_t size_att_dlc_overrides([[maybe_unused]] const asset_data::content::att_dlc_overrides& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::att_dlc_scalars read_att_dlc_scalars(const AttDlcScalars& native)
		{
			plain::trace("read_att_dlc_scalars");
			asset_data::content::att_dlc_scalars data;
			data.float_scalar = std::to_array(native.floatScalar);
			return data;
		}

		void write_att_dlc_scalars(AttDlcScalars& native, const asset_data::content::att_dlc_scalars& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			std::ranges::copy(data.float_scalar, native.floatScalar);
		}

		std::size_t size_att_dlc_scalars([[maybe_unused]] const asset_data::content::att_dlc_scalars& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::attachment read_attachment(const WeaponAttachment& native)
		{
			plain::trace("read_attachment");
			asset_data::content::attachment data;
			data.sz_internal_name = plain::read_string(native.szInternalName);
			data.sz_display_name = plain::read_string(native.szDisplayName);
			data.internal_name = plain::read_script_string(native.internalName);
			data.override_category_name = plain::read_script_string(native.overrideCategoryName);
			data.override_display_name = native.overrideDisplayName;
			data.disable_weapon_inspect = native.disableWeaponInspect;
			data.type = plain::enum_name(asset_data::content::attachment_type_names, static_cast<std::int64_t>(native.type), "WeaponAttachment.type");
			data.alt_mode_player_anim_type = plain::enum_name(asset_data::content::alt_mode_player_anim_type_t_names, static_cast<std::int64_t>(native.altModePlayerAnimType), "WeaponAttachment.altModePlayerAnimType");
			data.weapon_type = plain::enum_name(asset_data::content::weap_type_t_names, static_cast<std::int64_t>(native.weaponType), "WeaponAttachment.weaponType");
			data.weap_class = plain::enum_name(asset_data::content::weap_class_t_names, static_cast<std::int64_t>(native.weapClass), "WeaponAttachment.weapClass");
			data.fallback_world_model = plain::read_asset(ASSET_TYPE_XMODEL, native.fallbackWorldModel);
			data.fallback_view_model = plain::read_asset(ASSET_TYPE_XMODEL, native.fallbackViewModel);
			data.world_model_variations = plain::read_array(native.worldModelVariations, native.numModelVariations, "WeaponAttachment.worldModelVariations", [&](const auto& item) { return plain::read_asset(ASSET_TYPE_XMODEL, item); });
			data.view_model_variations = plain::read_array(native.viewModelVariations, native.numModelVariations, "WeaponAttachment.viewModelVariations", [&](const auto& item) { return plain::read_asset(ASSET_TYPE_XMODEL, item); });
			data.attach_point = plain::read_script_string(native.attachPoint);
			data.hide_tags = plain::read_array(native.hideTags, native.numHideTags, "WeaponAttachment.hideTags", [&](const auto& item) { return plain::read_script_string(item); });
			data.hide_tags_non_view_model = plain::read_array(native.hideTagsNonViewModel, native.numHideTagsNonViewModel, "WeaponAttachment.hideTagsNonViewModel", [&](const auto& item) { return plain::read_script_string(item); });
			data.show_tags = plain::read_array(native.showTags, native.numShowTags, "WeaponAttachment.showTags", [&](const auto& item) { return plain::read_script_string(item); });
			data.reticles = plain::read_array(native.reticles, native.numReticles, "WeaponAttachment.reticles", [&](const auto& item) { return plain::read_asset(ASSET_TYPE_RETICLE, item); });
			data.alt_reticles = plain::read_array(native.altReticles, native.numAltReticles, "WeaponAttachment.altReticles", [&](const auto& item) { return plain::read_asset(ASSET_TYPE_RETICLE, item); });
			data.ammogeneral = plain::read_single(native.ammogeneral, [&](const auto& item) { return read_att_ammo_general(item); });
			data.ammotype = plain::read_single(native.ammotype, [&](const auto& item) { return read_att_ammo_type(item); });
			data.impact = plain::read_single(native.impact, [&](const auto& item) { return read_att_impact(item); });
			data.dynamic_attachment_type = plain::enum_name(asset_data::content::dynamic_attachment_type_names, static_cast<std::int64_t>(native.dynamicAttachmentType), "WeaponAttachment.dynamicAttachmentType");
			data.dynamic_attachment_animate_seq = plain::read_string(native.dynamicAttachmentAnimateSeq);
			data.dynamic_attachment_icon = plain::read_string(native.dynamicAttachmentIcon);
			data.disable_input_driven_view_return_dampening = plain::enum_name(asset_data::content::weapon_flag_override_names, static_cast<std::int64_t>(native.disableInputDrivenViewReturnDampening), "WeaponAttachment.disableInputDrivenViewReturnDampening");
			data.tracer = plain::read_single(native.tracer, [&](const auto& item) { return read_att_ammo_tracer(item); });
			data.penetration = plain::read_single(native.penetration, [&](const auto& item) { return read_att_penetration(item); });
			data.sight = plain::read_single(native.sight, [&](const auto& item) { return read_att_sight(item); });
			data.hold_breath = plain::read_single(native.holdBreath, [&](const auto& item) { return read_att_hold_breath(item); });
			data.reload = plain::read_single(native.reload, [&](const auto& item) { return read_att_reload(item); });
			data.add_ons = plain::read_single(native.addOns, [&](const auto& item) { return read_att_add_ons(item); });
			data.laser = plain::read_single(native.laser, [&](const auto& item) { return read_att_laser(item); });
			data.post = plain::read_single(native.post, [&](const auto& item) { return read_att_post(item); });
			data.general = plain::read_single(native.general, [&](const auto& item) { return read_att_general(item); });
			data.aim_assist = plain::read_single(native.aimAssist, [&](const auto& item) { return read_att_aim_assist(item); });
			data.target_assist = plain::read_single(native.targetAssist, [&](const auto& item) { return read_att_target_assist(item); });
			data.ammunition = plain::read_fixed_with<28>(native.ammunition, [&](const auto& item) { return read_att_ammunition(item); });
			data.damage = plain::read_single(native.damage, [&](const auto& item) { return read_att_damage(item); });
			data.melee = plain::read_single(native.melee, [&](const auto& item) { return read_att_melee(item); });
			data.idle_settings = plain::read_single(native.idleSettings, [&](const auto& item) { return read_att_idle_settings(item); });
			data.ads_settings = plain::read_single(native.adsSettings, [&](const auto& item) { return read_att_ads_settings(item); });
			data.ads_settings_main = plain::read_single(native.adsSettingsMain, [&](const auto& item) { return read_att_ads_settings(item); });
			data.ads_gun_bob_settings = plain::read_single(native.adsGunBobSettings, [&](const auto& item) { return read_att_ads_gun_bob_settings(item); });
			data.ads_gun_bob_settings_main = plain::read_single(native.adsGunBobSettingsMain, [&](const auto& item) { return read_att_ads_gun_bob_settings(item); });
			data.ads_reload_settings = plain::read_single(native.adsReloadSettings, [&](const auto& item) { return read_att_ads_reload_settings(item); });
			data.scope_drift_settings = plain::read_single(native.scopeDriftSettings, [&](const auto& item) { return read_att_scope_drift_settings(item); });
			data.scope_drift_settings_main = plain::read_single(native.scopeDriftSettingsMain, [&](const auto& item) { return read_att_scope_drift_settings(item); });
			data.ads_stance_scales = plain::read_single(native.adsStanceScales, [&](const auto& item) { return read_att_ads_stance_scales(item); });
			data.hip_spread = plain::read_single(native.hipSpread, [&](const auto& item) { return read_att_hip_spread(item); });
			data.slide_spread = plain::read_single(native.slideSpread, [&](const auto& item) { return read_att_slide_spread(item); });
			data.gun_kick = plain::read_single(native.gunKick, [&](const auto& item) { return read_att_gun_kick(item); });
			data.gun_tilt = plain::read_single(native.gunTilt, [&](const auto& item) { return read_att_gun_tilt(item); });
			data.kick_scaling = plain::read_single(native.kickScaling, [&](const auto& item) { return read_att_kick_scaling(item); });
			data.view_kick = plain::read_single(native.viewKick, [&](const auto& item) { return read_att_view_kick(item); });
			data.ads_overlay = plain::read_single(native.adsOverlay, [&](const auto& item) { return read_att_ads_overlay(item); });
			data.outline = plain::read_single(native.outline, [&](const auto& item) { return read_att_outline(item); });
			data.ui = plain::read_single(native.ui, [&](const auto& item) { return read_att_ui(item); });
			data.rumbles = plain::read_single(native.rumbles, [&](const auto& item) { return read_att_rumbles(item); });
			data.projectile = plain::read_single(native.projectile, [&](const auto& item) { return read_att_projectile(item); });
			data.charged = plain::read_single(native.charged, [&](const auto& item) { return read_att_charged(item); });
			data.ads_alt_switch = plain::read_single(native.adsAltSwitch, [&](const auto& item) { return read_ads_alt_switch(item); });
			data.regeneration = plain::read_single(native.regeneration, [&](const auto& item) { return read_att_regeneration(item); });
			data.heat = plain::read_single(native.heat, [&](const auto& item) { return read_att_heat(item); });
			data.movement = plain::read_single(native.movement, [&](const auto& item) { return read_att_movement(item); });
			data.movement_scale = plain::read_single(native.movementScale, [&](const auto& item) { return read_att_movement(item); });
			data.movement_scale_main = plain::read_single(native.movementScaleMain, [&](const auto& item) { return read_att_movement(item); });
			data.burst = plain::read_single(native.burst, [&](const auto& item) { return read_att_burst(item); });
			data.hitmarker = plain::read_single(native.hitmarker, [&](const auto& item) { return read_att_hitmarker(item); });
			data.ads_movement = plain::read_single(native.adsMovement, [&](const auto& item) { return read_att_ads_movement(item); });
			data.ballistic_info = plain::read_single(native.ballisticInfo, [&](const auto& item) { return read_ballistic_info(item); });
			data.sway_settings = plain::read_single(native.swaySettings, [&](const auto& item) { return read_sway_settings(item); });
			data.weapon_offset_pattern_scales = plain::read_single(native.weaponOffsetPatternScales, [&](const auto& item) { return read_weapon_offset_pattern_scale_info(item); });
			data.location_damage = plain::read_fixed_with<22>(native.locationDamage, [&](const auto& item) { return item; });
			data.ammunition_scale = native.ammunitionScale;
			data.ballistic_muzzle_velocity_scale = native.ballisticMuzzleVelocityScale;
			data.damage_scale = native.damageScale;
			data.override_stand_ofs = native.overrideStandOfs;
			data.stand_ofs = plain::read_scalars<float, 3>(&native.standOfs);
			data.stand_ofs_rot = plain::read_scalars<float, 3>(&native.standOfsRot);
			data.stand_ofs_rot_pivot = plain::read_scalars<float, 3>(&native.standOfsRotPivot);
			data.stand_ofs_add = plain::read_scalars<float, 3>(&native.standOfsAdd);
			data.stand_ofs_rot_add = plain::read_scalars<float, 3>(&native.standOfsRotAdd);
			data.overridehip_offset_lerp_type = native.overridehipOffsetLerpType;
			data.hip_offset_lerp_type = plain::enum_name(asset_data::content::ads_offset_interpolation_type_names, static_cast<std::int64_t>(native.hipOffsetLerpType), "WeaponAttachment.hipOffsetLerpType");
			data.state_timers_scale = native.stateTimersScale;
			data.reload_timers_scale = native.reloadTimersScale;
			data.fire_time_override = native.fireTimeOverride;
			data.fire_time_akimbo_override = native.fireTimeAkimboOverride;
			data.fire_timers_scale = native.fireTimersScale;
			data.burst_fire_cooldown_scale = native.burstFireCooldownScale;
			data.idle_hip_motion_scale = native.idleHipMotionScale;
			data.idle_ads_motion_scale = native.idleADSMotionScale;
			data.idle_ads_motion_time_scale = native.idleADSMotionTimeScale;
			data.ads_settings_scale = native.adsSettingsScale;
			data.ads_settings_scale_main = native.adsSettingsScaleMain;
			data.ads_spread_scale = native.adsSpreadScale;
			data.ads_spread_scale_main = native.adsSpreadScaleMain;
			data.hip_spread_scale = native.hipSpreadScale;
			data.slide_spread_scale = native.slideSpreadScale;
			data.gun_kick_pitch_scale = native.gunKickPitchScale;
			data.gun_kick_yaw_scale = native.gunKickYawScale;
			data.gun_center_scale = native.gunCenterScale;
			data.gun_tilt_scale = native.gunTiltScale;
			data.view_kick_pitch_scale = native.viewKickPitchScale;
			data.view_kick_yaw_scale = native.viewKickYawScale;
			data.view_center_scale = native.viewCenterScale;
			data.view_kick_maintain_fraction_scale = native.viewKickMaintainFractionScale;
			data.hip_sway_scale = native.hipSwayScale;
			data.ads_sway_scale = native.adsSwayScale;
			data.ads_sway_scale2 = native.adsSwayScale2;
			data.ads_sway_scale3 = native.adsSwayScale3;
			data.ads_sway_max_angle_scale = native.adsSwayMaxAngleScale;
			data.ads_sway_lerp_speed_scale = native.adsSwayLerpSpeedScale;
			data.ads_sway_scale_scale = native.adsSwayScaleScale;
			data.sprint_out_timer_scale = native.sprintOutTimerScale;
			data.i_ammo_index = native.iAmmoIndex;
			data.heat_accumulation_scale = native.heatAccumulationScale;
			data.heat_dissipation_scale = native.heatDissipationScale;
			data.heat_smoke_threshold_scale = native.heatSmokeThresholdScale;
			data.ads_fire_motion_blur_scale = native.adsFireMotionBlurScale;
			data.ads_fire_motion_blur_duration_scale = native.adsFireMotionBlurDurationScale;
			data.hip_fire_motion_blur_scale = native.hipFireMotionBlurScale;
			data.hip_fire_motion_blur_duration_scale = native.hipFireMotionBlurDurationScale;
			data.ads_camera_shake_rotation_scale_scale = native.adsCameraShakeRotationScaleScale;
			data.ads_camera_shake_translation_scale_scale = native.adsCameraShakeTranslationScaleScale;
			data.ads_gun_bob_scale_scale = native.adsGunBobScaleScale;
			data.ads_gun_bob_scale_scale_main = native.adsGunBobScaleScaleMain;
			data.ads_gun_bob_tilt_scale_scale = native.adsGunBobTiltScaleScale;
			data.ads_gun_bob_tilt_scale_scale_main = native.adsGunBobTiltScaleScaleMain;
			data.ads_gun_bob_tilt_bullet_dir_scale_scale = native.adsGunBobTiltBulletDirScaleScale;
			data.ads_gun_bob_tilt_bullet_dir_scale_scale_main = native.adsGunBobTiltBulletDirScaleScaleMain;
			data.ads_view_bob_scale_scale = native.adsViewBobScaleScale;
			data.ads_bob_rate_scale = native.adsBobRateScale;
			data.melee_damage_scale = native.meleeDamageScale;
			data.melee_count_to_finish_adjust = native.meleeCountToFinishAdjust;
			data.load_index = native.loadIndex;
			data.ammo_used_per_shot_override = native.ammoUsedPerShotOverride;
			data.transient_view_flags = native.transientViewFlags;
			data.transient_world_flags = native.transientWorldFlags;
			data.use_alternate_view_model_laser = native.useAlternateViewModelLaser;
			data.ignore_iron_sights = native.ignoreIronSights;
			data.hide_iron_sights_with_this_attachment = native.hideIronSightsWithThisAttachment;
			data.share_ammo_with_alt = native.shareAmmoWithAlt;
			data.use_left_trigger_alt_fire = native.useLeftTriggerAltFire;
			data.force_foregrip_ik = native.forceForegripIK;
			data.contains_ik_tag = native.containsIKTag;
			data.is_akimbo_attachment = native.isAkimboAttachment;
			data.attach_left_weapon_akimbo = native.attachLeftWeaponAkimbo;
			data.attach_right_weapon_akimbo = native.attachRightWeaponAkimbo;
			data.lui_weapon_info_widget_uses_scope_stencil = native.luiWeaponInfoWidgetUsesScopeStencil;
			data.sz_alt_mode_name = plain::read_string(native.szAltModeName);
			data.sz_alt_mode_inactive_name = plain::read_string(native.szAltModeInactiveName);
			data.sz_lui_weapon_info_widget_name = plain::read_string(native.szLUIWeaponInfoWidgetName);
			data.sz_lui_weapon_info_widget_tag = plain::read_string(native.szLUIWeaponInfoWidgetTag);
			data.alt_mode_icon = plain::read_asset(ASSET_TYPE_IMAGE, native.altModeIcon);
			data.dlc_overrides = plain::read_single(native.dlcOverrides, [&](const auto& item) { return read_att_dlc_overrides(item); });
			data.dlc_scalars = plain::read_single(native.dlcScalars, [&](const auto& item) { return read_att_dlc_scalars(item); });
			return data;
		}

		void write_attachment(WeaponAttachment& native, const asset_data::content::attachment& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.szInternalName = plain::write_string(memory, data.sz_internal_name);
			native.szDisplayName = plain::write_string(memory, data.sz_display_name);
			native.internalName = static_cast<scr_string_t>(plain::write_script_string(data.internal_name));
			native.overrideCategoryName = static_cast<scr_string_t>(plain::write_script_string(data.override_category_name));
			native.overrideDisplayName = data.override_display_name;
			native.disableWeaponInspect = data.disable_weapon_inspect;
			native.type = static_cast<AttachmentType>(plain::enum_value(asset_data::content::attachment_type_names, data.type, "WeaponAttachment.type"));
			native.altModePlayerAnimType = static_cast<altModePlayerAnimType_t>(plain::enum_value(asset_data::content::alt_mode_player_anim_type_t_names, data.alt_mode_player_anim_type, "WeaponAttachment.altModePlayerAnimType"));
			native.weaponType = static_cast<weapType_t>(plain::enum_value(asset_data::content::weap_type_t_names, data.weapon_type, "WeaponAttachment.weaponType"));
			native.weapClass = static_cast<weapClass_t>(plain::enum_value(asset_data::content::weap_class_t_names, data.weap_class, "WeaponAttachment.weapClass"));
			native.fallbackWorldModel = plain::resolve_asset<XModel>(resolve, ASSET_TYPE_XMODEL, data.fallback_world_model);
			native.fallbackViewModel = plain::resolve_asset<XModel>(resolve, ASSET_TYPE_XMODEL, data.fallback_view_model);
			native.worldModelVariations = plain::write_array<XModel*>(data.world_model_variations, memory, [&](auto& out, const auto& in) { out = plain::resolve_asset<XModel>(resolve, ASSET_TYPE_XMODEL, in); });
			native.viewModelVariations = plain::write_array<XModel*>(data.view_model_variations, memory, [&](auto& out, const auto& in) { out = plain::resolve_asset<XModel>(resolve, ASSET_TYPE_XMODEL, in); });
			native.attachPoint = static_cast<scr_string_t>(plain::write_script_string(data.attach_point));
			native.hideTags = plain::write_array<scr_string_t>(data.hide_tags, memory, [&](auto& out, const auto& in) { out = static_cast<scr_string_t>(plain::write_script_string(in)); });
			native.hideTagsNonViewModel = plain::write_array<scr_string_t>(data.hide_tags_non_view_model, memory, [&](auto& out, const auto& in) { out = static_cast<scr_string_t>(plain::write_script_string(in)); });
			native.showTags = plain::write_array<scr_string_t>(data.show_tags, memory, [&](auto& out, const auto& in) { out = static_cast<scr_string_t>(plain::write_script_string(in)); });
			native.reticles = plain::write_array<ReticleDef*>(data.reticles, memory, [&](auto& out, const auto& in) { out = plain::resolve_asset<ReticleDef>(resolve, ASSET_TYPE_RETICLE, in); });
			native.altReticles = plain::write_array<ReticleDef*>(data.alt_reticles, memory, [&](auto& out, const auto& in) { out = plain::resolve_asset<ReticleDef>(resolve, ASSET_TYPE_RETICLE, in); });
			native.ammogeneral = plain::write_single<AttAmmoGeneral>(data.ammogeneral, memory, [&](auto& out, const auto& in) { write_att_ammo_general(out, in, resolve, memory); });
			native.ammotype = plain::write_single<AttAmmoType>(data.ammotype, memory, [&](auto& out, const auto& in) { write_att_ammo_type(out, in, resolve, memory); });
			native.impact = plain::write_single<AttImpact>(data.impact, memory, [&](auto& out, const auto& in) { write_att_impact(out, in, resolve, memory); });
			native.dynamicAttachmentType = static_cast<DynamicAttachmentType>(plain::enum_value(asset_data::content::dynamic_attachment_type_names, data.dynamic_attachment_type, "WeaponAttachment.dynamicAttachmentType"));
			native.dynamicAttachmentAnimateSeq = plain::write_string(memory, data.dynamic_attachment_animate_seq);
			native.dynamicAttachmentIcon = plain::write_string(memory, data.dynamic_attachment_icon);
			native.disableInputDrivenViewReturnDampening = static_cast<WeaponFlagOverride>(plain::enum_value(asset_data::content::weapon_flag_override_names, data.disable_input_driven_view_return_dampening, "WeaponAttachment.disableInputDrivenViewReturnDampening"));
			native.tracer = plain::write_single<AttAmmoTracer>(data.tracer, memory, [&](auto& out, const auto& in) { write_att_ammo_tracer(out, in, resolve, memory); });
			native.penetration = plain::write_single<AttPenetration>(data.penetration, memory, [&](auto& out, const auto& in) { write_att_penetration(out, in, resolve, memory); });
			native.sight = plain::write_single<AttSight>(data.sight, memory, [&](auto& out, const auto& in) { write_att_sight(out, in, resolve, memory); });
			native.holdBreath = plain::write_single<AttHoldBreath>(data.hold_breath, memory, [&](auto& out, const auto& in) { write_att_hold_breath(out, in, resolve, memory); });
			native.reload = plain::write_single<AttReload>(data.reload, memory, [&](auto& out, const auto& in) { write_att_reload(out, in, resolve, memory); });
			native.addOns = plain::write_single<AttAddOns>(data.add_ons, memory, [&](auto& out, const auto& in) { write_att_add_ons(out, in, resolve, memory); });
			native.laser = plain::write_single<AttLaser>(data.laser, memory, [&](auto& out, const auto& in) { write_att_laser(out, in, resolve, memory); });
			native.post = plain::write_single<AttPost>(data.post, memory, [&](auto& out, const auto& in) { write_att_post(out, in, resolve, memory); });
			native.general = plain::write_single<AttGeneral>(data.general, memory, [&](auto& out, const auto& in) { write_att_general(out, in, resolve, memory); });
			native.aimAssist = plain::write_single<AttAimAssist>(data.aim_assist, memory, [&](auto& out, const auto& in) { write_att_aim_assist(out, in, resolve, memory); });
			native.targetAssist = plain::write_single<AttTargetAssist>(data.target_assist, memory, [&](auto& out, const auto& in) { write_att_target_assist(out, in, resolve, memory); });
			if (data.ammunition)
			{
				native.ammunition = memory.manual_allocate<AttAmmunition>(plain::rounded(28 * sizeof(AttAmmunition)));
				for (std::size_t i = 0; i < 28; ++i)
				{
					write_att_ammunition(native.ammunition[i], (*data.ammunition)[i], resolve, memory);
				}
			}
			native.damage = plain::write_single<AttDamage>(data.damage, memory, [&](auto& out, const auto& in) { write_att_damage(out, in, resolve, memory); });
			native.melee = plain::write_single<AttMelee>(data.melee, memory, [&](auto& out, const auto& in) { write_att_melee(out, in, resolve, memory); });
			native.idleSettings = plain::write_single<AttIdleSettings>(data.idle_settings, memory, [&](auto& out, const auto& in) { write_att_idle_settings(out, in, resolve, memory); });
			native.adsSettings = plain::write_single<AttADSSettings>(data.ads_settings, memory, [&](auto& out, const auto& in) { write_att_ads_settings(out, in, resolve, memory); });
			native.adsSettingsMain = plain::write_single<AttADSSettings>(data.ads_settings_main, memory, [&](auto& out, const auto& in) { write_att_ads_settings(out, in, resolve, memory); });
			native.adsGunBobSettings = plain::write_single<AttADSGunBobSettings>(data.ads_gun_bob_settings, memory, [&](auto& out, const auto& in) { write_att_ads_gun_bob_settings(out, in, resolve, memory); });
			native.adsGunBobSettingsMain = plain::write_single<AttADSGunBobSettings>(data.ads_gun_bob_settings_main, memory, [&](auto& out, const auto& in) { write_att_ads_gun_bob_settings(out, in, resolve, memory); });
			native.adsReloadSettings = plain::write_single<AttADSReloadSettings>(data.ads_reload_settings, memory, [&](auto& out, const auto& in) { write_att_ads_reload_settings(out, in, resolve, memory); });
			native.scopeDriftSettings = plain::write_single<AttScopeDriftSettings>(data.scope_drift_settings, memory, [&](auto& out, const auto& in) { write_att_scope_drift_settings(out, in, resolve, memory); });
			native.scopeDriftSettingsMain = plain::write_single<AttScopeDriftSettings>(data.scope_drift_settings_main, memory, [&](auto& out, const auto& in) { write_att_scope_drift_settings(out, in, resolve, memory); });
			native.adsStanceScales = plain::write_single<AttADSStanceScales>(data.ads_stance_scales, memory, [&](auto& out, const auto& in) { write_att_ads_stance_scales(out, in, resolve, memory); });
			native.hipSpread = plain::write_single<AttHipSpread>(data.hip_spread, memory, [&](auto& out, const auto& in) { write_att_hip_spread(out, in, resolve, memory); });
			native.slideSpread = plain::write_single<AttSlideSpread>(data.slide_spread, memory, [&](auto& out, const auto& in) { write_att_slide_spread(out, in, resolve, memory); });
			native.gunKick = plain::write_single<AttGunKick>(data.gun_kick, memory, [&](auto& out, const auto& in) { write_att_gun_kick(out, in, resolve, memory); });
			native.gunTilt = plain::write_single<AttGunTilt>(data.gun_tilt, memory, [&](auto& out, const auto& in) { write_att_gun_tilt(out, in, resolve, memory); });
			native.kickScaling = plain::write_single<AttKickScaling>(data.kick_scaling, memory, [&](auto& out, const auto& in) { write_att_kick_scaling(out, in, resolve, memory); });
			native.viewKick = plain::write_single<AttViewKick>(data.view_kick, memory, [&](auto& out, const auto& in) { write_att_view_kick(out, in, resolve, memory); });
			native.adsOverlay = plain::write_single<AttADSOverlay>(data.ads_overlay, memory, [&](auto& out, const auto& in) { write_att_ads_overlay(out, in, resolve, memory); });
			native.outline = plain::write_single<AttOutline>(data.outline, memory, [&](auto& out, const auto& in) { write_att_outline(out, in, resolve, memory); });
			native.ui = plain::write_single<AttUI>(data.ui, memory, [&](auto& out, const auto& in) { write_att_ui(out, in, resolve, memory); });
			native.rumbles = plain::write_single<AttRumbles>(data.rumbles, memory, [&](auto& out, const auto& in) { write_att_rumbles(out, in, resolve, memory); });
			native.projectile = plain::write_single<AttProjectile>(data.projectile, memory, [&](auto& out, const auto& in) { write_att_projectile(out, in, resolve, memory); });
			native.charged = plain::write_single<AttCharged>(data.charged, memory, [&](auto& out, const auto& in) { write_att_charged(out, in, resolve, memory); });
			native.adsAltSwitch = plain::write_single<AdsAltSwitch>(data.ads_alt_switch, memory, [&](auto& out, const auto& in) { write_ads_alt_switch(out, in, resolve, memory); });
			native.regeneration = plain::write_single<AttRegeneration>(data.regeneration, memory, [&](auto& out, const auto& in) { write_att_regeneration(out, in, resolve, memory); });
			native.heat = plain::write_single<AttHeat>(data.heat, memory, [&](auto& out, const auto& in) { write_att_heat(out, in, resolve, memory); });
			native.movement = plain::write_single<AttMovement>(data.movement, memory, [&](auto& out, const auto& in) { write_att_movement(out, in, resolve, memory); });
			native.movementScale = plain::write_single<AttMovement>(data.movement_scale, memory, [&](auto& out, const auto& in) { write_att_movement(out, in, resolve, memory); });
			native.movementScaleMain = plain::write_single<AttMovement>(data.movement_scale_main, memory, [&](auto& out, const auto& in) { write_att_movement(out, in, resolve, memory); });
			native.burst = plain::write_single<AttBurst>(data.burst, memory, [&](auto& out, const auto& in) { write_att_burst(out, in, resolve, memory); });
			native.hitmarker = plain::write_single<AttHitmarker>(data.hitmarker, memory, [&](auto& out, const auto& in) { write_att_hitmarker(out, in, resolve, memory); });
			native.adsMovement = plain::write_single<AttAdsMovement>(data.ads_movement, memory, [&](auto& out, const auto& in) { write_att_ads_movement(out, in, resolve, memory); });
			native.ballisticInfo = plain::write_single<BallisticInfo>(data.ballistic_info, memory, [&](auto& out, const auto& in) { write_ballistic_info(out, in, resolve, memory); });
			native.swaySettings = plain::write_single<SwaySettings>(data.sway_settings, memory, [&](auto& out, const auto& in) { write_sway_settings(out, in, resolve, memory); });
			native.weaponOffsetPatternScales = plain::write_single<WeaponOffsetPatternScaleInfo>(data.weapon_offset_pattern_scales, memory, [&](auto& out, const auto& in) { write_weapon_offset_pattern_scale_info(out, in, resolve, memory); });
			if (data.location_damage)
			{
				native.locationDamage = memory.manual_allocate<float>(plain::rounded(22 * sizeof(float)));
				for (std::size_t i = 0; i < 22; ++i)
				{
					native.locationDamage[i] = (*data.location_damage)[i];
				}
			}
			native.ammunitionScale = data.ammunition_scale;
			native.ballisticMuzzleVelocityScale = data.ballistic_muzzle_velocity_scale;
			native.damageScale = data.damage_scale;
			native.overrideStandOfs = data.override_stand_ofs;
			plain::write_scalars(&native.standOfs, data.stand_ofs);
			plain::write_scalars(&native.standOfsRot, data.stand_ofs_rot);
			plain::write_scalars(&native.standOfsRotPivot, data.stand_ofs_rot_pivot);
			plain::write_scalars(&native.standOfsAdd, data.stand_ofs_add);
			plain::write_scalars(&native.standOfsRotAdd, data.stand_ofs_rot_add);
			native.overridehipOffsetLerpType = data.overridehip_offset_lerp_type;
			native.hipOffsetLerpType = static_cast<AdsOffsetInterpolationType>(plain::enum_value(asset_data::content::ads_offset_interpolation_type_names, data.hip_offset_lerp_type, "WeaponAttachment.hipOffsetLerpType"));
			native.stateTimersScale = data.state_timers_scale;
			native.reloadTimersScale = data.reload_timers_scale;
			native.fireTimeOverride = data.fire_time_override;
			native.fireTimeAkimboOverride = data.fire_time_akimbo_override;
			native.fireTimersScale = data.fire_timers_scale;
			native.burstFireCooldownScale = data.burst_fire_cooldown_scale;
			native.idleHipMotionScale = data.idle_hip_motion_scale;
			native.idleADSMotionScale = data.idle_ads_motion_scale;
			native.idleADSMotionTimeScale = data.idle_ads_motion_time_scale;
			native.adsSettingsScale = data.ads_settings_scale;
			native.adsSettingsScaleMain = data.ads_settings_scale_main;
			native.adsSpreadScale = data.ads_spread_scale;
			native.adsSpreadScaleMain = data.ads_spread_scale_main;
			native.hipSpreadScale = data.hip_spread_scale;
			native.slideSpreadScale = data.slide_spread_scale;
			native.gunKickPitchScale = data.gun_kick_pitch_scale;
			native.gunKickYawScale = data.gun_kick_yaw_scale;
			native.gunCenterScale = data.gun_center_scale;
			native.gunTiltScale = data.gun_tilt_scale;
			native.viewKickPitchScale = data.view_kick_pitch_scale;
			native.viewKickYawScale = data.view_kick_yaw_scale;
			native.viewCenterScale = data.view_center_scale;
			native.viewKickMaintainFractionScale = data.view_kick_maintain_fraction_scale;
			native.hipSwayScale = data.hip_sway_scale;
			native.adsSwayScale = data.ads_sway_scale;
			native.adsSwayScale2 = data.ads_sway_scale2;
			native.adsSwayScale3 = data.ads_sway_scale3;
			native.adsSwayMaxAngleScale = data.ads_sway_max_angle_scale;
			native.adsSwayLerpSpeedScale = data.ads_sway_lerp_speed_scale;
			native.adsSwayScaleScale = data.ads_sway_scale_scale;
			native.sprintOutTimerScale = data.sprint_out_timer_scale;
			native.iAmmoIndex = data.i_ammo_index;
			native.heatAccumulationScale = data.heat_accumulation_scale;
			native.heatDissipationScale = data.heat_dissipation_scale;
			native.heatSmokeThresholdScale = data.heat_smoke_threshold_scale;
			native.adsFireMotionBlurScale = data.ads_fire_motion_blur_scale;
			native.adsFireMotionBlurDurationScale = data.ads_fire_motion_blur_duration_scale;
			native.hipFireMotionBlurScale = data.hip_fire_motion_blur_scale;
			native.hipFireMotionBlurDurationScale = data.hip_fire_motion_blur_duration_scale;
			native.adsCameraShakeRotationScaleScale = data.ads_camera_shake_rotation_scale_scale;
			native.adsCameraShakeTranslationScaleScale = data.ads_camera_shake_translation_scale_scale;
			native.adsGunBobScaleScale = data.ads_gun_bob_scale_scale;
			native.adsGunBobScaleScaleMain = data.ads_gun_bob_scale_scale_main;
			native.adsGunBobTiltScaleScale = data.ads_gun_bob_tilt_scale_scale;
			native.adsGunBobTiltScaleScaleMain = data.ads_gun_bob_tilt_scale_scale_main;
			native.adsGunBobTiltBulletDirScaleScale = data.ads_gun_bob_tilt_bullet_dir_scale_scale;
			native.adsGunBobTiltBulletDirScaleScaleMain = data.ads_gun_bob_tilt_bullet_dir_scale_scale_main;
			native.adsViewBobScaleScale = data.ads_view_bob_scale_scale;
			native.adsBobRateScale = data.ads_bob_rate_scale;
			native.meleeDamageScale = data.melee_damage_scale;
			native.meleeCountToFinishAdjust = data.melee_count_to_finish_adjust;
			native.loadIndex = data.load_index;
			native.ammoUsedPerShotOverride = data.ammo_used_per_shot_override;
			native.transientViewFlags = data.transient_view_flags;
			native.transientWorldFlags = data.transient_world_flags;
			native.useAlternateViewModelLaser = data.use_alternate_view_model_laser;
			native.ignoreIronSights = data.ignore_iron_sights;
			native.hideIronSightsWithThisAttachment = data.hide_iron_sights_with_this_attachment;
			native.shareAmmoWithAlt = data.share_ammo_with_alt;
			native.useLeftTriggerAltFire = data.use_left_trigger_alt_fire;
			native.forceForegripIK = data.force_foregrip_ik;
			native.containsIKTag = data.contains_ik_tag;
			native.isAkimboAttachment = data.is_akimbo_attachment;
			native.attachLeftWeaponAkimbo = data.attach_left_weapon_akimbo;
			native.attachRightWeaponAkimbo = data.attach_right_weapon_akimbo;
			native.luiWeaponInfoWidgetUsesScopeStencil = data.lui_weapon_info_widget_uses_scope_stencil;
			native.szAltModeName = plain::write_string(memory, data.sz_alt_mode_name);
			native.szAltModeInactiveName = plain::write_string(memory, data.sz_alt_mode_inactive_name);
			native.szLUIWeaponInfoWidgetName = plain::write_string(memory, data.sz_lui_weapon_info_widget_name);
			native.szLUIWeaponInfoWidgetTag = plain::write_string(memory, data.sz_lui_weapon_info_widget_tag);
			native.altModeIcon = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, data.alt_mode_icon);
			native.dlcOverrides = plain::write_single<AttDlcOverrides>(data.dlc_overrides, memory, [&](auto& out, const auto& in) { write_att_dlc_overrides(out, in, resolve, memory); });
			native.dlcScalars = plain::write_single<AttDlcScalars>(data.dlc_scalars, memory, [&](auto& out, const auto& in) { write_att_dlc_scalars(out, in, resolve, memory); });
			native.numModelVariations = plain::count_of<std::uint32_t>(data.world_model_variations);
			native.numHideTags = plain::count_of<std::uint32_t>(data.hide_tags);
			native.numHideTagsNonViewModel = plain::count_of<std::uint32_t>(data.hide_tags_non_view_model);
			native.numShowTags = plain::count_of<std::uint32_t>(data.show_tags);
			native.numReticles = plain::count_of<std::uint32_t>(data.reticles);
			native.numAltReticles = plain::count_of<std::uint32_t>(data.alt_reticles);
		}

		std::size_t size_attachment([[maybe_unused]] const asset_data::content::attachment& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.sz_internal_name);
			size += plain::string_size(data.sz_display_name);
			if (data.world_model_variations)
			{
				size += plain::sized(std::max<std::size_t>(data.world_model_variations->size(), 1) * sizeof(XModel*));
			}
			if (data.view_model_variations)
			{
				size += plain::sized(std::max<std::size_t>(data.view_model_variations->size(), 1) * sizeof(XModel*));
			}
			if (data.hide_tags)
			{
				size += plain::sized(std::max<std::size_t>(data.hide_tags->size(), 1) * sizeof(scr_string_t));
			}
			if (data.hide_tags_non_view_model)
			{
				size += plain::sized(std::max<std::size_t>(data.hide_tags_non_view_model->size(), 1) * sizeof(scr_string_t));
			}
			if (data.show_tags)
			{
				size += plain::sized(std::max<std::size_t>(data.show_tags->size(), 1) * sizeof(scr_string_t));
			}
			if (data.reticles)
			{
				size += plain::sized(std::max<std::size_t>(data.reticles->size(), 1) * sizeof(ReticleDef*));
			}
			if (data.alt_reticles)
			{
				size += plain::sized(std::max<std::size_t>(data.alt_reticles->size(), 1) * sizeof(ReticleDef*));
			}
			if (data.ammogeneral)
			{
				size += plain::sized(sizeof(AttAmmoGeneral));
				size += size_att_ammo_general(*data.ammogeneral);
			}
			if (data.ammotype)
			{
				size += plain::sized(sizeof(AttAmmoType));
				size += size_att_ammo_type(*data.ammotype);
			}
			if (data.impact)
			{
				size += plain::sized(sizeof(AttImpact));
				size += size_att_impact(*data.impact);
			}
			size += plain::string_size(data.dynamic_attachment_animate_seq);
			size += plain::string_size(data.dynamic_attachment_icon);
			if (data.tracer)
			{
				size += plain::sized(sizeof(AttAmmoTracer));
				size += size_att_ammo_tracer(*data.tracer);
			}
			if (data.penetration)
			{
				size += plain::sized(sizeof(AttPenetration));
				size += size_att_penetration(*data.penetration);
			}
			if (data.sight)
			{
				size += plain::sized(sizeof(AttSight));
				size += size_att_sight(*data.sight);
			}
			if (data.hold_breath)
			{
				size += plain::sized(sizeof(AttHoldBreath));
				size += size_att_hold_breath(*data.hold_breath);
			}
			if (data.reload)
			{
				size += plain::sized(sizeof(AttReload));
				size += size_att_reload(*data.reload);
			}
			if (data.add_ons)
			{
				size += plain::sized(sizeof(AttAddOns));
				size += size_att_add_ons(*data.add_ons);
			}
			if (data.laser)
			{
				size += plain::sized(sizeof(AttLaser));
				size += size_att_laser(*data.laser);
			}
			if (data.post)
			{
				size += plain::sized(sizeof(AttPost));
				size += size_att_post(*data.post);
			}
			if (data.general)
			{
				size += plain::sized(sizeof(AttGeneral));
				size += size_att_general(*data.general);
			}
			if (data.aim_assist)
			{
				size += plain::sized(sizeof(AttAimAssist));
				size += size_att_aim_assist(*data.aim_assist);
			}
			if (data.target_assist)
			{
				size += plain::sized(sizeof(AttTargetAssist));
				size += size_att_target_assist(*data.target_assist);
			}
			if (data.ammunition)
			{
				size += plain::sized(28 * sizeof(AttAmmunition));
				for (const auto& item : *data.ammunition)
				{
					size += size_att_ammunition(item);
				}
			}
			if (data.damage)
			{
				size += plain::sized(sizeof(AttDamage));
				size += size_att_damage(*data.damage);
			}
			if (data.melee)
			{
				size += plain::sized(sizeof(AttMelee));
				size += size_att_melee(*data.melee);
			}
			if (data.idle_settings)
			{
				size += plain::sized(sizeof(AttIdleSettings));
				size += size_att_idle_settings(*data.idle_settings);
			}
			if (data.ads_settings)
			{
				size += plain::sized(sizeof(AttADSSettings));
				size += size_att_ads_settings(*data.ads_settings);
			}
			if (data.ads_settings_main)
			{
				size += plain::sized(sizeof(AttADSSettings));
				size += size_att_ads_settings(*data.ads_settings_main);
			}
			if (data.ads_gun_bob_settings)
			{
				size += plain::sized(sizeof(AttADSGunBobSettings));
				size += size_att_ads_gun_bob_settings(*data.ads_gun_bob_settings);
			}
			if (data.ads_gun_bob_settings_main)
			{
				size += plain::sized(sizeof(AttADSGunBobSettings));
				size += size_att_ads_gun_bob_settings(*data.ads_gun_bob_settings_main);
			}
			if (data.ads_reload_settings)
			{
				size += plain::sized(sizeof(AttADSReloadSettings));
				size += size_att_ads_reload_settings(*data.ads_reload_settings);
			}
			if (data.scope_drift_settings)
			{
				size += plain::sized(sizeof(AttScopeDriftSettings));
				size += size_att_scope_drift_settings(*data.scope_drift_settings);
			}
			if (data.scope_drift_settings_main)
			{
				size += plain::sized(sizeof(AttScopeDriftSettings));
				size += size_att_scope_drift_settings(*data.scope_drift_settings_main);
			}
			if (data.ads_stance_scales)
			{
				size += plain::sized(sizeof(AttADSStanceScales));
				size += size_att_ads_stance_scales(*data.ads_stance_scales);
			}
			if (data.hip_spread)
			{
				size += plain::sized(sizeof(AttHipSpread));
				size += size_att_hip_spread(*data.hip_spread);
			}
			if (data.slide_spread)
			{
				size += plain::sized(sizeof(AttSlideSpread));
				size += size_att_slide_spread(*data.slide_spread);
			}
			if (data.gun_kick)
			{
				size += plain::sized(sizeof(AttGunKick));
				size += size_att_gun_kick(*data.gun_kick);
			}
			if (data.gun_tilt)
			{
				size += plain::sized(sizeof(AttGunTilt));
				size += size_att_gun_tilt(*data.gun_tilt);
			}
			if (data.kick_scaling)
			{
				size += plain::sized(sizeof(AttKickScaling));
				size += size_att_kick_scaling(*data.kick_scaling);
			}
			if (data.view_kick)
			{
				size += plain::sized(sizeof(AttViewKick));
				size += size_att_view_kick(*data.view_kick);
			}
			if (data.ads_overlay)
			{
				size += plain::sized(sizeof(AttADSOverlay));
				size += size_att_ads_overlay(*data.ads_overlay);
			}
			if (data.outline)
			{
				size += plain::sized(sizeof(AttOutline));
				size += size_att_outline(*data.outline);
			}
			if (data.ui)
			{
				size += plain::sized(sizeof(AttUI));
				size += size_att_ui(*data.ui);
			}
			if (data.rumbles)
			{
				size += plain::sized(sizeof(AttRumbles));
				size += size_att_rumbles(*data.rumbles);
			}
			if (data.projectile)
			{
				size += plain::sized(sizeof(AttProjectile));
				size += size_att_projectile(*data.projectile);
			}
			if (data.charged)
			{
				size += plain::sized(sizeof(AttCharged));
				size += size_att_charged(*data.charged);
			}
			if (data.ads_alt_switch)
			{
				size += plain::sized(sizeof(AdsAltSwitch));
				size += size_ads_alt_switch(*data.ads_alt_switch);
			}
			if (data.regeneration)
			{
				size += plain::sized(sizeof(AttRegeneration));
				size += size_att_regeneration(*data.regeneration);
			}
			if (data.heat)
			{
				size += plain::sized(sizeof(AttHeat));
				size += size_att_heat(*data.heat);
			}
			if (data.movement)
			{
				size += plain::sized(sizeof(AttMovement));
				size += size_att_movement(*data.movement);
			}
			if (data.movement_scale)
			{
				size += plain::sized(sizeof(AttMovement));
				size += size_att_movement(*data.movement_scale);
			}
			if (data.movement_scale_main)
			{
				size += plain::sized(sizeof(AttMovement));
				size += size_att_movement(*data.movement_scale_main);
			}
			if (data.burst)
			{
				size += plain::sized(sizeof(AttBurst));
				size += size_att_burst(*data.burst);
			}
			if (data.hitmarker)
			{
				size += plain::sized(sizeof(AttHitmarker));
				size += size_att_hitmarker(*data.hitmarker);
			}
			if (data.ads_movement)
			{
				size += plain::sized(sizeof(AttAdsMovement));
				size += size_att_ads_movement(*data.ads_movement);
			}
			if (data.ballistic_info)
			{
				size += plain::sized(sizeof(BallisticInfo));
				size += size_ballistic_info(*data.ballistic_info);
			}
			if (data.sway_settings)
			{
				size += plain::sized(sizeof(SwaySettings));
				size += size_sway_settings(*data.sway_settings);
			}
			if (data.weapon_offset_pattern_scales)
			{
				size += plain::sized(sizeof(WeaponOffsetPatternScaleInfo));
				size += size_weapon_offset_pattern_scale_info(*data.weapon_offset_pattern_scales);
			}
			if (data.location_damage)
			{
				size += plain::sized(22 * sizeof(float));
			}
			size += plain::string_size(data.sz_alt_mode_name);
			size += plain::string_size(data.sz_alt_mode_inactive_name);
			size += plain::string_size(data.sz_lui_weapon_info_widget_name);
			size += plain::string_size(data.sz_lui_weapon_info_widget_tag);
			if (data.dlc_overrides)
			{
				size += plain::sized(sizeof(AttDlcOverrides));
				size += size_att_dlc_overrides(*data.dlc_overrides);
			}
			if (data.dlc_scalars)
			{
				size += plain::sized(sizeof(AttDlcScalars));
				size += size_att_dlc_scalars(*data.dlc_scalars);
			}
			return size;
		}

		asset_data::content::weapon_offset_pattern_scale read_weapon_offset_pattern_scale(const WeaponOffsetPatternScale& native)
		{
			plain::trace("read_weapon_offset_pattern_scale");
			asset_data::content::weapon_offset_pattern_scale data;
			data.pattern_key = plain::read_script_string(native.patternKey);
			data.scale = plain::read_scalars<float, 3>(&native.scale);
			return data;
		}

		void write_weapon_offset_pattern_scale(WeaponOffsetPatternScale& native, const asset_data::content::weapon_offset_pattern_scale& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.patternKey = static_cast<scr_string_t>(plain::write_script_string(data.pattern_key));
			plain::write_scalars(&native.scale, data.scale);
		}

		std::size_t size_weapon_offset_pattern_scale([[maybe_unused]] const asset_data::content::weapon_offset_pattern_scale& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::animation_state_machine read_animation_state_machine(const AnimationStateMachine& native)
		{
			plain::trace("read_animation_state_machine");
			asset_data::content::animation_state_machine data;
			data.name = plain::read_script_string(native.name);
			data.states = plain::read_array(native.states, native.stateCount, "AnimationStateMachine.states", [&](const auto& item) { return read_animation_state(item); });
			data.aim_sets = plain::read_array(native.aimSets, native.aimSetCount, "AnimationStateMachine.aimSets", [&](const auto& item) { return read_animation_aim_set(item); });
			return data;
		}

		void write_animation_state_machine(AnimationStateMachine& native, const asset_data::content::animation_state_machine& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = static_cast<scr_string_t>(plain::write_script_string(data.name));
			native.states = plain::write_array<AnimationState>(data.states, memory, [&](auto& out, const auto& in) { write_animation_state(out, in, resolve, memory); });
			native.aimSets = plain::write_array<AnimationAimSet>(data.aim_sets, memory, [&](auto& out, const auto& in) { write_animation_aim_set(out, in, resolve, memory); });
			native.stateCount = plain::count_of<std::uint16_t>(data.states);
			native.aimSetCount = plain::count_of<std::uint16_t>(data.aim_sets);
		}

		std::size_t size_animation_state_machine([[maybe_unused]] const asset_data::content::animation_state_machine& data)
		{
			std::size_t size = 0;
			if (data.states)
			{
				size += plain::sized(std::max<std::size_t>(data.states->size(), 1) * sizeof(AnimationState));
				for (const auto& item : *data.states)
				{
					size += size_animation_state(item);
				}
			}
			if (data.aim_sets)
			{
				size += plain::sized(std::max<std::size_t>(data.aim_sets->size(), 1) * sizeof(AnimationAimSet));
				for (const auto& item : *data.aim_sets)
				{
					size += size_animation_aim_set(item);
				}
			}
			return size;
		}

		asset_data::content::animclass read_animclass(const AnimationClass& native)
		{
			plain::trace("read_animclass");
			asset_data::content::animclass data;
			data.class_name = plain::read_string(native.className);
			data.state_machine = plain::read_single(native.stateMachine, [&](const auto& item) { return read_animation_state_machine(item); });
			data.anim_ctrl = plain::enum_name(asset_data::content::animation_controller_names, static_cast<std::int64_t>(native.animCtrl), "AnimationClass.animCtrl");
			data.anim_tree = plain::read_script_string(native.animTree);
			data.sound_notes = plain::read_array(native.soundNotes, native.soundCount, "AnimationClass.soundNotes", [&](const auto& item) { return plain::read_script_string(item); });
			data.sound_names = plain::read_array(native.soundNames, native.soundCount, "AnimationClass.soundNames", [&](const auto& item) { return plain::read_script_string(item); });
			data.sound_options = plain::read_array(native.soundOptions, native.soundCount, "AnimationClass.soundOptions", [&](const auto& item) { return plain::read_script_string(item); });
			data.effect_notes = plain::read_array(native.effectNotes, native.effectCount, "AnimationClass.effectNotes", [&](const auto& item) { return plain::read_script_string(item); });
			data.effect_defs = plain::read_array(native.effectDefs, native.effectCount, "AnimationClass.effectDefs", [&](const auto& item) { return read_fx_combined_def(item); });
			data.effect_tags = plain::read_array(native.effectTags, native.effectCount, "AnimationClass.effectTags", [&](const auto& item) { return plain::read_script_string(item); });
			return data;
		}

		void write_animclass(AnimationClass& native, const asset_data::content::animclass& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.className = plain::write_string(memory, data.class_name);
			native.stateMachine = plain::write_single<AnimationStateMachine>(data.state_machine, memory, [&](auto& out, const auto& in) { write_animation_state_machine(out, in, resolve, memory); });
			native.animCtrl = static_cast<AnimationController>(plain::enum_value(asset_data::content::animation_controller_names, data.anim_ctrl, "AnimationClass.animCtrl"));
			native.animTree = static_cast<scr_string_t>(plain::write_script_string(data.anim_tree));
			native.soundNotes = plain::write_array<scr_string_t>(data.sound_notes, memory, [&](auto& out, const auto& in) { out = static_cast<scr_string_t>(plain::write_script_string(in)); });
			native.soundNames = plain::write_array<scr_string_t>(data.sound_names, memory, [&](auto& out, const auto& in) { out = static_cast<scr_string_t>(plain::write_script_string(in)); });
			native.soundOptions = plain::write_array<scr_string_t>(data.sound_options, memory, [&](auto& out, const auto& in) { out = static_cast<scr_string_t>(plain::write_script_string(in)); });
			native.effectNotes = plain::write_array<scr_string_t>(data.effect_notes, memory, [&](auto& out, const auto& in) { out = static_cast<scr_string_t>(plain::write_script_string(in)); });
			native.effectDefs = plain::write_array<FxCombinedDef>(data.effect_defs, memory, [&](auto& out, const auto& in) { write_fx_combined_def(out, in, resolve, memory); });
			native.effectTags = plain::write_array<scr_string_t>(data.effect_tags, memory, [&](auto& out, const auto& in) { out = static_cast<scr_string_t>(plain::write_script_string(in)); });
			native.soundCount = plain::count_of<std::uint16_t>(data.sound_notes);
			native.effectCount = plain::count_of<std::uint16_t>(data.effect_notes);
		}

		std::size_t size_animclass([[maybe_unused]] const asset_data::content::animclass& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.class_name);
			if (data.state_machine)
			{
				size += plain::sized(sizeof(AnimationStateMachine));
				size += size_animation_state_machine(*data.state_machine);
			}
			if (data.sound_notes)
			{
				size += plain::sized(std::max<std::size_t>(data.sound_notes->size(), 1) * sizeof(scr_string_t));
			}
			if (data.sound_names)
			{
				size += plain::sized(std::max<std::size_t>(data.sound_names->size(), 1) * sizeof(scr_string_t));
			}
			if (data.sound_options)
			{
				size += plain::sized(std::max<std::size_t>(data.sound_options->size(), 1) * sizeof(scr_string_t));
			}
			if (data.effect_notes)
			{
				size += plain::sized(std::max<std::size_t>(data.effect_notes->size(), 1) * sizeof(scr_string_t));
			}
			if (data.effect_defs)
			{
				size += plain::sized(std::max<std::size_t>(data.effect_defs->size(), 1) * sizeof(FxCombinedDef));
				for (const auto& item : *data.effect_defs)
				{
					size += size_fx_combined_def(item);
				}
			}
			if (data.effect_tags)
			{
				size += plain::sized(std::max<std::size_t>(data.effect_tags->size(), 1) * sizeof(scr_string_t));
			}
			return size;
		}

		asset_data::content::animation_state read_animation_state(const AnimationState& native)
		{
			plain::trace("read_animation_state");
			asset_data::content::animation_state data;
			data.name = plain::read_script_string(native.name);
			data.notify = plain::read_script_string(native.notify);
			data.blend_time = native.blendTime;
			data.blend_out_time = native.blendOutTime;
			data.flags = native.flags;
			data.aim_set_index = native.aimSetIndex;
			data.anim_entries = plain::read_array(native.animEntries, native.entryCount, "AnimationState.animEntries", [&](const auto& item) { return read_animation_entry(item); });
			data.anim_indices = plain::read_array(native.animIndices, native.entryCount, "AnimationState.animIndices", [&](const auto& item) { return item; });
			data.alias_list = plain::read_array(native.aliasList, native.aliasCount, "AnimationState.aliasList", [&](const auto& item) { return read_anim_alias(item); });
			return data;
		}

		void write_animation_state(AnimationState& native, const asset_data::content::animation_state& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = static_cast<scr_string_t>(plain::write_script_string(data.name));
			native.notify = static_cast<scr_string_t>(plain::write_script_string(data.notify));
			native.blendTime = data.blend_time;
			native.blendOutTime = data.blend_out_time;
			native.flags = data.flags;
			native.aimSetIndex = data.aim_set_index;
			native.animEntries = plain::write_array<AnimationEntry>(data.anim_entries, memory, [&](auto& out, const auto& in) { write_animation_entry(out, in, resolve, memory); });
			native.animIndices = plain::write_array<std::uint64_t>(data.anim_indices, memory, [&](auto& out, const auto& in) { out = in; });
			native.aliasList = plain::write_array<AnimAlias>(data.alias_list, memory, [&](auto& out, const auto& in) { write_anim_alias(out, in, resolve, memory); });
			native.entryCount = plain::count_of<std::uint8_t>(data.anim_entries);
			native.aliasCount = plain::count_of<std::uint8_t>(data.alias_list);
		}

		std::size_t size_animation_state([[maybe_unused]] const asset_data::content::animation_state& data)
		{
			std::size_t size = 0;
			if (data.anim_entries)
			{
				size += plain::sized(std::max<std::size_t>(data.anim_entries->size(), 1) * sizeof(AnimationEntry));
				for (const auto& item : *data.anim_entries)
				{
					size += size_animation_entry(item);
				}
			}
			if (data.anim_indices)
			{
				size += plain::sized(std::max<std::size_t>(data.anim_indices->size(), 1) * sizeof(std::uint64_t));
			}
			if (data.alias_list)
			{
				size += plain::sized(std::max<std::size_t>(data.alias_list->size(), 1) * sizeof(AnimAlias));
				for (const auto& item : *data.alias_list)
				{
					size += size_anim_alias(item);
				}
			}
			return size;
		}

		asset_data::content::animation_entry read_animation_entry(const AnimationEntry& native)
		{
			plain::trace("read_animation_entry");
			asset_data::content::animation_entry data;
			data.anim_name = plain::read_script_string(native.animName);
			data.aim_set_index = native.aimSetIndex;
			return data;
		}

		void write_animation_entry(AnimationEntry& native, const asset_data::content::animation_entry& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.animName = static_cast<scr_string_t>(plain::write_script_string(data.anim_name));
			native.aimSetIndex = data.aim_set_index;
		}

		std::size_t size_animation_entry([[maybe_unused]] const asset_data::content::animation_entry& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::anim_alias read_anim_alias(const AnimAlias& native)
		{
			plain::trace("read_anim_alias");
			asset_data::content::anim_alias data;
			data.alias_name = plain::read_script_string(native.aliasName);
			data.alias_info = plain::read_array(native.aliasInfo, native.animCount, "AnimAlias.aliasInfo", [&](const auto& item) { return read_anim_alias_info(item); });
			return data;
		}

		void write_anim_alias(AnimAlias& native, const asset_data::content::anim_alias& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.aliasName = static_cast<scr_string_t>(plain::write_script_string(data.alias_name));
			native.aliasInfo = plain::write_array<AnimAliasInfo>(data.alias_info, memory, [&](auto& out, const auto& in) { write_anim_alias_info(out, in, resolve, memory); });
			native.animCount = plain::count_of<std::uint8_t>(data.alias_info);
		}

		std::size_t size_anim_alias([[maybe_unused]] const asset_data::content::anim_alias& data)
		{
			std::size_t size = 0;
			if (data.alias_info)
			{
				size += plain::sized(std::max<std::size_t>(data.alias_info->size(), 1) * sizeof(AnimAliasInfo));
				for (const auto& item : *data.alias_info)
				{
					size += size_anim_alias_info(item);
				}
			}
			return size;
		}

		asset_data::content::anim_alias_info read_anim_alias_info(const AnimAliasInfo& native)
		{
			plain::trace("read_anim_alias_info");
			asset_data::content::anim_alias_info data;
			data.anim_index = native.animIndex;
			data.anim_weight = native.animWeight;
			return data;
		}

		void write_anim_alias_info(AnimAliasInfo& native, const asset_data::content::anim_alias_info& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.animIndex = data.anim_index;
			native.animWeight = data.anim_weight;
		}

		std::size_t size_anim_alias_info([[maybe_unused]] const asset_data::content::anim_alias_info& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::animation_aim_set read_animation_aim_set(const AnimationAimSet& native)
		{
			plain::trace("read_animation_aim_set");
			asset_data::content::animation_aim_set data;
			data.name = plain::read_script_string(native.name);
			data.root_name = plain::read_script_string(native.rootName);
			data.anim_name = plain::read_array(native.animName, native.animCount, "AnimationAimSet.animName", [&](const auto& item) { return plain::read_script_string(item); });
			data.root_index = native.rootIndex;
			data.anim_indices = plain::read_array(native.animIndices, native.animCount, "AnimationAimSet.animIndices", [&](const auto& item) { return item; });
			data.aim_node_indices = plain::read_array(native.aimNodeIndices, native.animCount, "AnimationAimSet.aimNodeIndices", [&](const auto& item) { return item; });
			return data;
		}

		void write_animation_aim_set(AnimationAimSet& native, const asset_data::content::animation_aim_set& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = static_cast<scr_string_t>(plain::write_script_string(data.name));
			native.rootName = static_cast<scr_string_t>(plain::write_script_string(data.root_name));
			native.animName = plain::write_array<scr_string_t>(data.anim_name, memory, [&](auto& out, const auto& in) { out = static_cast<scr_string_t>(plain::write_script_string(in)); });
			native.rootIndex = data.root_index;
			native.animIndices = plain::write_array<std::uint64_t>(data.anim_indices, memory, [&](auto& out, const auto& in) { out = in; });
			native.aimNodeIndices = plain::write_array<std::uint64_t>(data.aim_node_indices, memory, [&](auto& out, const auto& in) { out = in; });
			native.animCount = plain::count_of<std::int32_t>(data.anim_name);
		}

		std::size_t size_animation_aim_set([[maybe_unused]] const asset_data::content::animation_aim_set& data)
		{
			std::size_t size = 0;
			if (data.anim_name)
			{
				size += plain::sized(std::max<std::size_t>(data.anim_name->size(), 1) * sizeof(scr_string_t));
			}
			if (data.anim_indices)
			{
				size += plain::sized(std::max<std::size_t>(data.anim_indices->size(), 1) * sizeof(std::uint64_t));
			}
			if (data.aim_node_indices)
			{
				size += plain::sized(std::max<std::size_t>(data.aim_node_indices->size(), 1) * sizeof(std::uint64_t));
			}
			return size;
		}

		asset_data::content::luafile read_luafile(const LuaFile& native)
		{
			plain::trace("read_luafile");
			asset_data::content::luafile data;
			data.name = plain::read_string(native.name);
			data.stripping_type = native.strippingType;
			data.buffer = plain::read_array(native.buffer, native.len, "LuaFile.buffer", [&](const auto& item) { return item; });
			return data;
		}

		void write_luafile(LuaFile& native, const asset_data::content::luafile& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.strippingType = data.stripping_type;
			native.buffer = plain::write_array<std::uint8_t>(data.buffer, memory, [&](auto& out, const auto& in) { out = in; });
			native.len = plain::count_of<std::int32_t>(data.buffer);
		}

		std::size_t size_luafile([[maybe_unused]] const asset_data::content::luafile& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.buffer)
			{
				size += plain::sized(std::max<std::size_t>(data.buffer->size(), 1) * sizeof(std::uint8_t));
			}
			return size;
		}

		asset_data::content::leaderboarddef read_leaderboarddef(const LeaderboardDef& native)
		{
			plain::trace("read_leaderboarddef");
			asset_data::content::leaderboarddef data;
			data.name = plain::read_string(native.name);
			data.id = native.id;
			data.source_lb_id = native.sourceLbId;
			data.source_lb_width = native.sourceLbWidth;
			data.xp_col_id = native.xpColId;
			data.prestige_col_id = native.prestigeColId;
			data.columns = plain::read_array(native.columns, native.columnCount, "LeaderboardDef.columns", [&](const auto& item) { return read_lb_column_def(item); });
			data.update_type = plain::enum_name(asset_data::content::lb_update_type_names, static_cast<std::int64_t>(native.updateType), "LeaderboardDef.updateType");
			data.track_types = native.trackTypes;
			data.rank_col_id_x = native.rankColIdX;
			data.rank_col_id_y = native.rankColIdY;
			return data;
		}

		void write_leaderboarddef(LeaderboardDef& native, const asset_data::content::leaderboarddef& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.id = data.id;
			native.sourceLbId = data.source_lb_id;
			native.sourceLbWidth = data.source_lb_width;
			native.xpColId = data.xp_col_id;
			native.prestigeColId = data.prestige_col_id;
			native.columns = plain::write_array<LbColumnDef>(data.columns, memory, [&](auto& out, const auto& in) { write_lb_column_def(out, in, resolve, memory); });
			native.updateType = static_cast<LbUpdateType>(plain::enum_value(asset_data::content::lb_update_type_names, data.update_type, "LeaderboardDef.updateType"));
			native.trackTypes = data.track_types;
			native.rankColIdX = data.rank_col_id_x;
			native.rankColIdY = data.rank_col_id_y;
			native.columnCount = plain::count_of<std::int32_t>(data.columns);
		}

		std::size_t size_leaderboarddef([[maybe_unused]] const asset_data::content::leaderboarddef& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.columns)
			{
				size += plain::sized(std::max<std::size_t>(data.columns->size(), 1) * sizeof(LbColumnDef));
				for (const auto& item : *data.columns)
				{
					size += size_lb_column_def(item);
				}
			}
			return size;
		}

		asset_data::content::lb_column_def read_lb_column_def(const LbColumnDef& native)
		{
			plain::trace("read_lb_column_def");
			asset_data::content::lb_column_def data;
			data.name = plain::read_string(native.name);
			data.id = native.id;
			data.property_id = native.propertyId;
			data.hidden = native.hidden;
			data.stats_group = native.statsGroup;
			data.stat_name = plain::read_string(native.statName);
			data.type = plain::enum_name(asset_data::content::lb_col_type_names, static_cast<std::int64_t>(native.type), "LbColumnDef.type");
			data.precision = native.precision;
			data.agg = plain::enum_name(asset_data::content::lb_agg_type_names, static_cast<std::int64_t>(native.agg), "LbColumnDef.agg");
			data.ui_cal_col_x = native.uiCalColX;
			data.ui_cal_col_y = native.uiCalColY;
			return data;
		}

		void write_lb_column_def(LbColumnDef& native, const asset_data::content::lb_column_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.id = data.id;
			native.propertyId = data.property_id;
			native.hidden = data.hidden;
			native.statsGroup = data.stats_group;
			native.statName = plain::write_string(memory, data.stat_name);
			native.type = static_cast<LbColType>(plain::enum_value(asset_data::content::lb_col_type_names, data.type, "LbColumnDef.type"));
			native.precision = data.precision;
			native.agg = static_cast<LbAggType>(plain::enum_value(asset_data::content::lb_agg_type_names, data.agg, "LbColumnDef.agg"));
			native.uiCalColX = data.ui_cal_col_x;
			native.uiCalColY = data.ui_cal_col_y;
		}

		std::size_t size_lb_column_def([[maybe_unused]] const asset_data::content::lb_column_def& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			size += plain::string_size(data.stat_name);
			return size;
		}

		asset_data::content::virtualleaderboarddef read_virtualleaderboarddef(const VirtualLeaderboardDef& native)
		{
			plain::trace("read_virtualleaderboarddef");
			asset_data::content::virtualleaderboarddef data;
			data.name = plain::read_string(native.name);
			data.source_name = plain::read_string(native.sourceName);
			data.id = native.id;
			data.source_id = native.sourceId;
			data.columns = plain::read_array(native.columns, native.columnCount, "VirtualLeaderboardDef.columns", [&](const auto& item) { return read_lb_vr_column_def(item); });
			data.rank_col_id_x = native.rankColIdX;
			data.rank_col_id_y = native.rankColIdY;
			return data;
		}

		void write_virtualleaderboarddef(VirtualLeaderboardDef& native, const asset_data::content::virtualleaderboarddef& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.sourceName = plain::write_string(memory, data.source_name);
			native.id = data.id;
			native.sourceId = data.source_id;
			native.columns = plain::write_array<LbVrColumnDef>(data.columns, memory, [&](auto& out, const auto& in) { write_lb_vr_column_def(out, in, resolve, memory); });
			native.rankColIdX = data.rank_col_id_x;
			native.rankColIdY = data.rank_col_id_y;
			native.columnCount = plain::count_of<std::int32_t>(data.columns);
		}

		std::size_t size_virtualleaderboarddef([[maybe_unused]] const asset_data::content::virtualleaderboarddef& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			size += plain::string_size(data.source_name);
			if (data.columns)
			{
				size += plain::sized(std::max<std::size_t>(data.columns->size(), 1) * sizeof(LbVrColumnDef));
				for (const auto& item : *data.columns)
				{
					size += size_lb_vr_column_def(item);
				}
			}
			return size;
		}

		asset_data::content::lb_vr_column_def read_lb_vr_column_def(const LbVrColumnDef& native)
		{
			plain::trace("read_lb_vr_column_def");
			asset_data::content::lb_vr_column_def data;
			data.name = plain::read_string(native.name);
			data.id = native.id;
			data.ui_cal_col_x = native.uiCalColX;
			data.ui_cal_col_y = native.uiCalColY;
			return data;
		}

		void write_lb_vr_column_def(LbVrColumnDef& native, const asset_data::content::lb_vr_column_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.id = data.id;
			native.uiCalColX = data.ui_cal_col_x;
			native.uiCalColY = data.ui_cal_col_y;
		}

		std::size_t size_lb_vr_column_def([[maybe_unused]] const asset_data::content::lb_vr_column_def& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			return size;
		}

		asset_data::content::clothasset read_clothasset(const ClothAsset& native)
		{
			plain::trace("read_clothasset");
			asset_data::content::clothasset data;
			data.name = plain::read_string(native.name);
			data.stand_alone = native.standAlone;
			data.havok_data = plain::read_array(native.havokData, native.havokDataSize, "ClothAsset.havokData", [&](const auto& item) { return item; });
			data.bone_mapping_sets = plain::read_array(native.boneMappingSets, native.boneMappingSetCount, "ClothAsset.boneMappingSets", [&](const auto& item) { return read_cloth_asset_bone_mapping_set(item); });
			data.animation_state_name = plain::read_string(native.animationStateName);
			data.animation_state_idx = native.animationStateIdx;
			data.simulation_lods = plain::read_array(native.simulationLods, native.simulationLodCount, "ClothAsset.simulationLods", [&](const auto& item) { return read_cloth_asset_simulation_lod(item); });
			data.surface_type_index = native.surfaceTypeIndex;
			data.vector_field_support = native.vectorFieldSupport;
			data.vector_field_drag = native.vectorFieldDrag;
			data.bullet_support = native.bulletSupport;
			data.bullet_proxy_update_rate = native.bulletProxyUpdateRate;
			data.bullet_force = native.bulletForce;
			data.default_speed = native.defaultSpeed;
			data.speed_blend_speed = native.speedBlendSpeed;
			data.sound_iw_max_distance_sq = native.soundIWMaxDistanceSq;
			data.sound_hk_max_distance_sq = native.soundHKMaxDistanceSq;
			data.sounds = plain::read_array(native.sounds, native.soundCount, "ClothAsset.sounds", [&](const auto& item) { return read_cloth_asset_sound(item); });
			return data;
		}

		void write_clothasset(ClothAsset& native, const asset_data::content::clothasset& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.standAlone = data.stand_alone;
			native.havokData = plain::write_array<char>(data.havok_data, memory, [&](auto& out, const auto& in) { out = in; });
			native.boneMappingSets = plain::write_array<ClothAssetBoneMappingSet>(data.bone_mapping_sets, memory, [&](auto& out, const auto& in) { write_cloth_asset_bone_mapping_set(out, in, resolve, memory); });
			native.animationStateName = plain::write_string(memory, data.animation_state_name);
			native.animationStateIdx = data.animation_state_idx;
			native.simulationLods = plain::write_array<ClothAssetSimulationLOD>(data.simulation_lods, memory, [&](auto& out, const auto& in) { write_cloth_asset_simulation_lod(out, in, resolve, memory); });
			native.surfaceTypeIndex = data.surface_type_index;
			native.vectorFieldSupport = data.vector_field_support;
			native.vectorFieldDrag = data.vector_field_drag;
			native.bulletSupport = data.bullet_support;
			native.bulletProxyUpdateRate = data.bullet_proxy_update_rate;
			native.bulletForce = data.bullet_force;
			native.defaultSpeed = data.default_speed;
			native.speedBlendSpeed = data.speed_blend_speed;
			native.soundIWMaxDistanceSq = data.sound_iw_max_distance_sq;
			native.soundHKMaxDistanceSq = data.sound_hk_max_distance_sq;
			native.sounds = plain::write_array<ClothAssetSound>(data.sounds, memory, [&](auto& out, const auto& in) { write_cloth_asset_sound(out, in, resolve, memory); });
			native.havokDataSize = plain::count_of<std::uint32_t>(data.havok_data);
			native.boneMappingSetCount = plain::count_of<std::uint32_t>(data.bone_mapping_sets);
			native.simulationLodCount = plain::count_of<std::uint32_t>(data.simulation_lods);
			native.soundCount = plain::count_of<std::uint32_t>(data.sounds);
		}

		std::size_t size_clothasset([[maybe_unused]] const asset_data::content::clothasset& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.havok_data)
			{
				size += plain::sized(std::max<std::size_t>(data.havok_data->size(), 1) * sizeof(char));
			}
			if (data.bone_mapping_sets)
			{
				size += plain::sized(std::max<std::size_t>(data.bone_mapping_sets->size(), 1) * sizeof(ClothAssetBoneMappingSet));
				for (const auto& item : *data.bone_mapping_sets)
				{
					size += size_cloth_asset_bone_mapping_set(item);
				}
			}
			size += plain::string_size(data.animation_state_name);
			if (data.simulation_lods)
			{
				size += plain::sized(std::max<std::size_t>(data.simulation_lods->size(), 1) * sizeof(ClothAssetSimulationLOD));
				for (const auto& item : *data.simulation_lods)
				{
					size += size_cloth_asset_simulation_lod(item);
				}
			}
			if (data.sounds)
			{
				size += plain::sized(std::max<std::size_t>(data.sounds->size(), 1) * sizeof(ClothAssetSound));
				for (const auto& item : *data.sounds)
				{
					size += size_cloth_asset_sound(item);
				}
			}
			return size;
		}

		asset_data::content::cloth_asset_bone_mapping_set read_cloth_asset_bone_mapping_set(const ClothAssetBoneMappingSet& native)
		{
			plain::trace("read_cloth_asset_bone_mapping_set");
			asset_data::content::cloth_asset_bone_mapping_set data;
			data.bone_mappings = plain::read_array(native.boneMappings, native.boneMappingCount, "ClothAssetBoneMappingSet.boneMappings", [&](const auto& item) { return read_cloth_asset_bone_mapping(item); });
			return data;
		}

		void write_cloth_asset_bone_mapping_set(ClothAssetBoneMappingSet& native, const asset_data::content::cloth_asset_bone_mapping_set& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.boneMappings = plain::write_array<ClothAssetBoneMapping>(data.bone_mappings, memory, [&](auto& out, const auto& in) { write_cloth_asset_bone_mapping(out, in, resolve, memory); });
			native.boneMappingCount = plain::count_of<std::uint32_t>(data.bone_mappings);
		}

		std::size_t size_cloth_asset_bone_mapping_set([[maybe_unused]] const asset_data::content::cloth_asset_bone_mapping_set& data)
		{
			std::size_t size = 0;
			if (data.bone_mappings)
			{
				size += plain::sized(std::max<std::size_t>(data.bone_mappings->size(), 1) * sizeof(ClothAssetBoneMapping));
				for (const auto& item : *data.bone_mappings)
				{
					size += size_cloth_asset_bone_mapping(item);
				}
			}
			return size;
		}

		asset_data::content::cloth_asset_bone_mapping read_cloth_asset_bone_mapping(const ClothAssetBoneMapping& native)
		{
			plain::trace("read_cloth_asset_bone_mapping");
			asset_data::content::cloth_asset_bone_mapping data;
			data.scr_tag_names = plain::read_array(native.scrTagNames, native.scrTagCount, "ClothAssetBoneMapping.scrTagNames", [&](const auto& item) { return plain::read_script_string(item); });
			return data;
		}

		void write_cloth_asset_bone_mapping(ClothAssetBoneMapping& native, const asset_data::content::cloth_asset_bone_mapping& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.scrTagNames = plain::write_array<scr_string_t>(data.scr_tag_names, memory, [&](auto& out, const auto& in) { out = static_cast<scr_string_t>(plain::write_script_string(in)); });
			native.scrTagCount = plain::count_of<std::uint32_t>(data.scr_tag_names);
		}

		std::size_t size_cloth_asset_bone_mapping([[maybe_unused]] const asset_data::content::cloth_asset_bone_mapping& data)
		{
			std::size_t size = 0;
			if (data.scr_tag_names)
			{
				size += plain::sized(std::max<std::size_t>(data.scr_tag_names->size(), 1) * sizeof(scr_string_t));
			}
			return size;
		}

		asset_data::content::cloth_asset_simulation_lod read_cloth_asset_simulation_lod(const ClothAssetSimulationLOD& native)
		{
			plain::trace("read_cloth_asset_simulation_lod");
			asset_data::content::cloth_asset_simulation_lod data;
			data.max_distance_sq = native.maxDistanceSq;
			data.hk_max_distance_sq = native.hkMaxDistanceSq;
			data.state_name = plain::read_string(native.stateName);
			data.state_idx = native.stateIdx;
			return data;
		}

		void write_cloth_asset_simulation_lod(ClothAssetSimulationLOD& native, const asset_data::content::cloth_asset_simulation_lod& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.maxDistanceSq = data.max_distance_sq;
			native.hkMaxDistanceSq = data.hk_max_distance_sq;
			native.stateName = plain::write_string(memory, data.state_name);
			native.stateIdx = data.state_idx;
		}

		std::size_t size_cloth_asset_simulation_lod([[maybe_unused]] const asset_data::content::cloth_asset_simulation_lod& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.state_name);
			return size;
		}

		asset_data::content::cloth_asset_sound read_cloth_asset_sound(const ClothAssetSound& native)
		{
			plain::trace("read_cloth_asset_sound");
			asset_data::content::cloth_asset_sound data;
			data.min_speed = native.minSpeed;
			data.max_speed = native.maxSpeed;
			data.pitch_min_speed = native.pitchMinSpeed;
			data.pitch_max_speed = native.pitchMaxSpeed;
			data.volume_min_speed = native.volumeMinSpeed;
			data.volume_max_speed = native.volumeMaxSpeed;
			data.sound_alias = plain::read_string(native.soundAlias);
			return data;
		}

		void write_cloth_asset_sound(ClothAssetSound& native, const asset_data::content::cloth_asset_sound& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.minSpeed = data.min_speed;
			native.maxSpeed = data.max_speed;
			native.pitchMinSpeed = data.pitch_min_speed;
			native.pitchMaxSpeed = data.pitch_max_speed;
			native.volumeMinSpeed = data.volume_min_speed;
			native.volumeMaxSpeed = data.volume_max_speed;
			native.soundAlias = plain::write_string(memory, data.sound_alias);
		}

		std::size_t size_cloth_asset_sound([[maybe_unused]] const asset_data::content::cloth_asset_sound& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.sound_alias);
			return size;
		}

		asset_data::content::gfx_buffer_resource read_gfx_buffer_resource(const GfxBufferResource& native)
		{
			plain::trace("read_gfx_buffer_resource");
			asset_data::content::gfx_buffer_resource data;
			plain::require_zero(native.resource, "GfxBufferResource.resource");
			data.gpu_address = native.gpuAddress;
			data.allocation = native.allocation;
			data.size = native.size;
			return data;
		}

		void write_gfx_buffer_resource(GfxBufferResource& native, const asset_data::content::gfx_buffer_resource& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.gpuAddress = data.gpu_address;
			native.allocation = data.allocation;
			native.size = data.size;
		}

		std::size_t size_gfx_buffer_resource([[maybe_unused]] const asset_data::content::gfx_buffer_resource& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::gfx_shader_buffer_view read_gfx_shader_buffer_view(const GfxShaderBufferView& native)
		{
			plain::trace("read_gfx_shader_buffer_view");
			asset_data::content::gfx_shader_buffer_view data;
			plain::require_zero(native.resource, "GfxShaderBufferView.resource");
			data.view = native.view;
			return data;
		}

		void write_gfx_shader_buffer_view(GfxShaderBufferView& native, const asset_data::content::gfx_shader_buffer_view& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.view = data.view;
		}

		std::size_t size_gfx_shader_buffer_view([[maybe_unused]] const asset_data::content::gfx_shader_buffer_view& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::gfx_wrapped_base_buffer read_gfx_wrapped_base_buffer(const GfxWrappedBaseBuffer& native)
		{
			plain::trace("read_gfx_wrapped_base_buffer");
			asset_data::content::gfx_wrapped_base_buffer data;
			data.buffer = read_gfx_buffer_resource(native.buffer);
			data.view = read_gfx_shader_buffer_view(native.view);
			return data;
		}

		void write_gfx_wrapped_base_buffer(GfxWrappedBaseBuffer& native, const asset_data::content::gfx_wrapped_base_buffer& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_gfx_buffer_resource(native.buffer, data.buffer, resolve, memory);
			write_gfx_shader_buffer_view(native.view, data.view, resolve, memory);
		}

		std::size_t size_gfx_wrapped_base_buffer([[maybe_unused]] const asset_data::content::gfx_wrapped_base_buffer& data)
		{
			std::size_t size = 0;
			size += size_gfx_buffer_resource(data.buffer);
			size += size_gfx_shader_buffer_view(data.view);
			return size;
		}

		asset_data::content::gfx_wrapped_buffer read_gfx_wrapped_buffer(const GfxWrappedBuffer& native)
		{
			plain::trace("read_gfx_wrapped_buffer");
			asset_data::content::gfx_wrapped_buffer data;
			data.base = read_gfx_wrapped_base_buffer(native.base);
			plain::require_zero(native.data, "GfxWrappedBuffer.data");
			data.flags = native.flags;
			return data;
		}

		void write_gfx_wrapped_buffer(GfxWrappedBuffer& native, const asset_data::content::gfx_wrapped_buffer& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_gfx_wrapped_base_buffer(native.base, data.base, resolve, memory);
			native.flags = data.flags;
		}

		std::size_t size_gfx_wrapped_buffer([[maybe_unused]] const asset_data::content::gfx_wrapped_buffer& data)
		{
			std::size_t size = 0;
			size += size_gfx_wrapped_base_buffer(data.base);
			return size;
		}

		asset_data::content::st_clutter_sample_points read_st_clutter_sample_points(const StClutterSamplePoints& native)
		{
			plain::trace("read_st_clutter_sample_points");
			asset_data::content::st_clutter_sample_points data;
			data.sample_count = native.sampleCount;
			data.sample_points = plain::read_array(native.samplePoints, native.sampleCount, "StClutterSamplePoints.samplePoints", [&](const auto& item) { return plain::read_scalars<float, 2>(&item); });
			data.sample_point_buffer = read_gfx_wrapped_buffer(native.samplePointBuffer);
			return data;
		}

		void write_st_clutter_sample_points(StClutterSamplePoints& native, const asset_data::content::st_clutter_sample_points& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.sampleCount = data.sample_count;
			native.samplePoints = plain::write_array<vec2_t>(data.sample_points, memory, [&](auto& out, const auto& in) { plain::write_scalars(&out, in); });
			write_gfx_wrapped_buffer(native.samplePointBuffer, data.sample_point_buffer, resolve, memory);
		}

		std::size_t size_st_clutter_sample_points([[maybe_unused]] const asset_data::content::st_clutter_sample_points& data)
		{
			std::size_t size = 0;
			if (data.sample_points)
			{
				size += plain::sized(std::max<std::size_t>(data.sample_points->size(), 1) * sizeof(vec2_t));
			}
			size += size_gfx_wrapped_buffer(data.sample_point_buffer);
			return size;
		}

		asset_data::content::stterrain read_stterrain(const StTerrain& native)
		{
			plain::trace("read_stterrain");
			asset_data::content::stterrain data;
			data.name = plain::read_string(native.name);
			data.surfaces = plain::read_array(native.surfaces, native.surfaceCount, "StTerrain.surfaces", [&](const auto& item) { return read_st_disk_terrain_surface(item); });
			data.maps = plain::read_array(native.maps, native.mapsCount, "StTerrain.maps", [&](const auto& item) { return read_st_terrain_maps(item); });
			data.flattened_images = plain::read_array(native.flattenedImages, native.flattenedImagesCount, "StTerrain.flattenedImages", [&](const auto& item) { return plain::read_asset(ASSET_TYPE_IMAGE, item); });
			data.clutter_sample_points = read_st_clutter_sample_points(native.clutterSamplePoints);
			data.lightmap_type = plain::enum_name(asset_data::content::gfx_lightmap_type_names, static_cast<std::int64_t>(native.lightmapType), "StTerrain.lightmapType");
			data.lightmaps = plain::read_array(native.lightmaps, native.lightmapCount, "StTerrain.lightmaps", [&](const auto& item) { return read_st_lightmap(item); });
			return data;
		}

		void write_stterrain(StTerrain& native, const asset_data::content::stterrain& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.surfaces = plain::write_array<StDiskTerrainSurface>(data.surfaces, memory, [&](auto& out, const auto& in) { write_st_disk_terrain_surface(out, in, resolve, memory); });
			native.maps = plain::write_array<StTerrainMaps>(data.maps, memory, [&](auto& out, const auto& in) { write_st_terrain_maps(out, in, resolve, memory); });
			native.flattenedImages = plain::write_array<GfxImage*>(data.flattened_images, memory, [&](auto& out, const auto& in) { out = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, in); });
			write_st_clutter_sample_points(native.clutterSamplePoints, data.clutter_sample_points, resolve, memory);
			native.lightmapType = static_cast<GfxLightmapType>(plain::enum_value(asset_data::content::gfx_lightmap_type_names, data.lightmap_type, "StTerrain.lightmapType"));
			native.lightmaps = plain::write_array<StLightmap>(data.lightmaps, memory, [&](auto& out, const auto& in) { write_st_lightmap(out, in, resolve, memory); });
			native.surfaceCount = plain::count_of<std::uint32_t>(data.surfaces);
			native.mapsCount = plain::count_of<std::uint32_t>(data.maps);
			native.flattenedImagesCount = plain::count_of<std::uint32_t>(data.flattened_images);
			native.lightmapCount = plain::count_of<std::uint32_t>(data.lightmaps);
		}

		std::size_t size_stterrain([[maybe_unused]] const asset_data::content::stterrain& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.surfaces)
			{
				size += plain::sized(std::max<std::size_t>(data.surfaces->size(), 1) * sizeof(StDiskTerrainSurface));
				for (const auto& item : *data.surfaces)
				{
					size += size_st_disk_terrain_surface(item);
				}
			}
			if (data.maps)
			{
				size += plain::sized(std::max<std::size_t>(data.maps->size(), 1) * sizeof(StTerrainMaps));
				for (const auto& item : *data.maps)
				{
					size += size_st_terrain_maps(item);
				}
			}
			if (data.flattened_images)
			{
				size += plain::sized(std::max<std::size_t>(data.flattened_images->size(), 1) * sizeof(GfxImage*));
			}
			size += size_st_clutter_sample_points(data.clutter_sample_points);
			if (data.lightmaps)
			{
				size += plain::sized(std::max<std::size_t>(data.lightmaps->size(), 1) * sizeof(StLightmap));
				for (const auto& item : *data.lightmaps)
				{
					size += size_st_lightmap(item);
				}
			}
			return size;
		}

		asset_data::content::gfx_index_buffer read_gfx_index_buffer(const GfxIndexBuffer& native)
		{
			plain::trace("read_gfx_index_buffer");
			asset_data::content::gfx_index_buffer data;
			plain::require_zero(native.resource, "GfxIndexBuffer.resource");
			data.gpu_address = native.gpuAddress;
			data.allocation = native.allocation;
			data.size = native.size;
			return data;
		}

		void write_gfx_index_buffer(GfxIndexBuffer& native, const asset_data::content::gfx_index_buffer& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.gpuAddress = data.gpu_address;
			native.allocation = data.allocation;
			native.size = data.size;
		}

		std::size_t size_gfx_index_buffer([[maybe_unused]] const asset_data::content::gfx_index_buffer& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::st_terrain_grid read_st_terrain_grid(const StTerrainGrid& native)
		{
			plain::trace("read_st_terrain_grid");
			asset_data::content::st_terrain_grid data;
			data.num_levels = native.numLevels;
			data.levels = plain::read_array(native.levels, native.numLevels, "StTerrainGrid.levels", [&](const auto& item) { return read_st_terrain_grid_level(item); });
			return data;
		}

		void write_st_terrain_grid(StTerrainGrid& native, const asset_data::content::st_terrain_grid& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.numLevels = data.num_levels;
			native.levels = plain::write_array<StTerrainGridLevel>(data.levels, memory, [&](auto& out, const auto& in) { write_st_terrain_grid_level(out, in, resolve, memory); });
		}

		std::size_t size_st_terrain_grid([[maybe_unused]] const asset_data::content::st_terrain_grid& data)
		{
			std::size_t size = 0;
			if (data.levels)
			{
				size += plain::sized(std::max<std::size_t>(data.levels->size(), 1) * sizeof(StTerrainGridLevel));
				for (const auto& item : *data.levels)
				{
					size += size_st_terrain_grid_level(item);
				}
			}
			return size;
		}

		asset_data::content::gfx_placement read_gfx_placement(const GfxPlacement& native)
		{
			plain::trace("read_gfx_placement");
			asset_data::content::gfx_placement data;
			data.quat = plain::read_scalars<float, 4>(&native.quat);
			data.origin = plain::read_scalars<float, 3>(&native.origin);
			return data;
		}

		void write_gfx_placement(GfxPlacement& native, const asset_data::content::gfx_placement& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.quat, data.quat);
			plain::write_scalars(&native.origin, data.origin);
		}

		std::size_t size_gfx_placement([[maybe_unused]] const asset_data::content::gfx_placement& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::bounds read_bounds(const Bounds& native)
		{
			plain::trace("read_bounds");
			asset_data::content::bounds data;
			data.mid_point = plain::read_scalars<float, 3>(&native.midPoint);
			data.half_size = plain::read_scalars<float, 3>(&native.halfSize);
			return data;
		}

		void write_bounds(Bounds& native, const asset_data::content::bounds& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.midPoint, data.mid_point);
			plain::write_scalars(&native.halfSize, data.half_size);
		}

		std::size_t size_bounds([[maybe_unused]] const asset_data::content::bounds& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::st_clutter_sample_bitmask read_st_clutter_sample_bitmask(const StClutterSampleBitmask& native)
		{
			plain::trace("read_st_clutter_sample_bitmask");
			asset_data::content::st_clutter_sample_bitmask data;
			data.stream_key = plain::read_asset(ASSET_TYPE_STREAM_KEY, native.streamKey);
			data.bitmask = plain::read_array(native.bitmask, native.wordCount, "StClutterSampleBitmask.bitmask", [&](const auto& item) { return item; });
			data.bitmask_buffer = read_gfx_wrapped_buffer(native.bitmaskBuffer);
			data.relocated_bitmask_buffer = read_gfx_wrapped_buffer(native.relocatedBitmaskBuffer);
			return data;
		}

		void write_st_clutter_sample_bitmask(StClutterSampleBitmask& native, const asset_data::content::st_clutter_sample_bitmask& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.streamKey = plain::resolve_asset<StreamKey>(resolve, ASSET_TYPE_STREAM_KEY, data.stream_key);
			native.bitmask = plain::write_array<std::uint32_t>(data.bitmask, memory, [&](auto& out, const auto& in) { out = in; });
			write_gfx_wrapped_buffer(native.bitmaskBuffer, data.bitmask_buffer, resolve, memory);
			write_gfx_wrapped_buffer(native.relocatedBitmaskBuffer, data.relocated_bitmask_buffer, resolve, memory);
			native.wordCount = plain::count_of<std::uint32_t>(data.bitmask);
		}

		std::size_t size_st_clutter_sample_bitmask([[maybe_unused]] const asset_data::content::st_clutter_sample_bitmask& data)
		{
			std::size_t size = 0;
			if (data.bitmask)
			{
				size += plain::sized(std::max<std::size_t>(data.bitmask->size(), 1) * sizeof(std::uint32_t));
			}
			size += size_gfx_wrapped_buffer(data.bitmask_buffer);
			size += size_gfx_wrapped_buffer(data.relocated_bitmask_buffer);
			return size;
		}

		asset_data::content::st_disk_terrain_surface read_st_disk_terrain_surface(const StDiskTerrainSurface& native)
		{
			plain::trace("read_st_disk_terrain_surface");
			asset_data::content::st_disk_terrain_surface data;
			data.high_lod_index_count = native.highLODIndexCount;
			data.high_lod_indices = plain::read_array(native.highLODIndices, native.highLODIndexCount, "StDiskTerrainSurface.highLODIndices", [&](const auto& item) { return item; });
			data.high_lod_index_buffer = read_gfx_index_buffer(native.highLODIndexBuffer);
			data.grid = read_st_terrain_grid(native.grid);
			data.obj_to_wld = read_gfx_placement(native.objToWld);
			data.wld_bounds = read_bounds(native.wldBounds);
			data.layer_count = native.layerCount;
			data.surface_map_set_index = native.surfaceMapSetIndex;
			data.surface_map_x_count = native.surfaceMapXCount;
			data.surface_map_y_count = native.surfaceMapYCount;
			data.tile_vert_x_count = native.tileVertXCount;
			data.surface_map_origin = plain::read_scalars<float, 2>(&native.surfaceMapOrigin);
			data.surface_map_world_size = native.surfaceMapWorldSize;
			data.height_offset = native.heightOffset;
			data.height_scale = native.heightScale;
			data.half_wld_size = native.halfWldSize;
			data.layer_materials = plain::read_array(native.layerMaterials, native.layerMaterialsCount, "StDiskTerrainSurface.layerMaterials", [&](const auto& item) { return read_st_terrain_material_layer(item); });
			data.layer_displacement_mask = native.layerDisplacementMask;
			data.displacement_scales = plain::read_array(native.displacementScales, native.displacementScalesCount, "StDiskTerrainSurface.displacementScales", [&](const auto& item) { return read_st_terrain_displacement_scale(item); });
			data.clutter_layers = plain::read_array(native.clutterLayers, native.clutterLayerCount, "StDiskTerrainSurface.clutterLayers", [&](const auto& item) { return read_st_terrain_clutter_layer(item); });
			data.clutter_layers_buffer = read_gfx_wrapped_buffer(native.clutterLayersBuffer);
			data.clutter_sample_bitmask = read_st_clutter_sample_bitmask(native.clutterSampleBitmask);
			data.clutter_tile_instance_counts = plain::read_array(native.clutterTileInstanceCounts, native.clutterTileCount, "StDiskTerrainSurface.clutterTileInstanceCounts", [&](const auto& item) { return item; });
			data.layer_mask_map_idxs = plain::read_array(native.layerMaskMapIdxs, native.layerMaskMapIdxsCount, "StDiskTerrainSurface.layerMaskMapIdxs", [&](const auto& item) { return item; });
			data.height_map = plain::read_asset(ASSET_TYPE_IMAGE, native.heightMap);
			data.cutout_map = plain::read_asset(ASSET_TYPE_IMAGE, native.cutoutMap);
			data.combined_albedo_map = plain::read_asset(ASSET_TYPE_IMAGE, native.combinedAlbedoMap);
			data.combined_nog_map = plain::read_asset(ASSET_TYPE_IMAGE, native.combinedNogMap);
			data.combined_map_stream_radius_inv_sq = native.combinedMapStreamRadiusInvSq;
			data.lightmap_index = native.lightmapIndex;
			return data;
		}

		void write_st_disk_terrain_surface(StDiskTerrainSurface& native, const asset_data::content::st_disk_terrain_surface& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.highLODIndexCount = data.high_lod_index_count;
			native.highLODIndices = plain::write_array<std::uint16_t>(data.high_lod_indices, memory, [&](auto& out, const auto& in) { out = in; });
			write_gfx_index_buffer(native.highLODIndexBuffer, data.high_lod_index_buffer, resolve, memory);
			write_st_terrain_grid(native.grid, data.grid, resolve, memory);
			write_gfx_placement(native.objToWld, data.obj_to_wld, resolve, memory);
			write_bounds(native.wldBounds, data.wld_bounds, resolve, memory);
			native.layerCount = data.layer_count;
			native.surfaceMapSetIndex = data.surface_map_set_index;
			native.surfaceMapXCount = data.surface_map_x_count;
			native.surfaceMapYCount = data.surface_map_y_count;
			native.tileVertXCount = data.tile_vert_x_count;
			plain::write_scalars(&native.surfaceMapOrigin, data.surface_map_origin);
			native.surfaceMapWorldSize = data.surface_map_world_size;
			native.heightOffset = data.height_offset;
			native.heightScale = data.height_scale;
			native.halfWldSize = data.half_wld_size;
			native.layerMaterials = plain::write_array<StTerrainMaterialLayer>(data.layer_materials, memory, [&](auto& out, const auto& in) { write_st_terrain_material_layer(out, in, resolve, memory); });
			native.layerDisplacementMask = data.layer_displacement_mask;
			native.displacementScales = plain::write_array<StTerrainDisplacementScale>(data.displacement_scales, memory, [&](auto& out, const auto& in) { write_st_terrain_displacement_scale(out, in, resolve, memory); });
			native.clutterLayers = plain::write_array<StTerrainClutterLayer>(data.clutter_layers, memory, [&](auto& out, const auto& in) { write_st_terrain_clutter_layer(out, in, resolve, memory); });
			write_gfx_wrapped_buffer(native.clutterLayersBuffer, data.clutter_layers_buffer, resolve, memory);
			write_st_clutter_sample_bitmask(native.clutterSampleBitmask, data.clutter_sample_bitmask, resolve, memory);
			native.clutterTileInstanceCounts = plain::write_array<std::uint32_t>(data.clutter_tile_instance_counts, memory, [&](auto& out, const auto& in) { out = in; });
			native.layerMaskMapIdxs = plain::write_array<std::uint16_t>(data.layer_mask_map_idxs, memory, [&](auto& out, const auto& in) { out = in; });
			native.heightMap = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, data.height_map);
			native.cutoutMap = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, data.cutout_map);
			native.combinedAlbedoMap = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, data.combined_albedo_map);
			native.combinedNogMap = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, data.combined_nog_map);
			native.combinedMapStreamRadiusInvSq = data.combined_map_stream_radius_inv_sq;
			native.lightmapIndex = data.lightmap_index;
			native.layerMaterialsCount = plain::count_of<std::uint32_t>(data.layer_materials);
			native.displacementScalesCount = plain::count_of<std::uint32_t>(data.displacement_scales);
			native.clutterLayerCount = plain::count_of<std::uint32_t>(data.clutter_layers);
			native.clutterTileCount = plain::count_of<std::uint32_t>(data.clutter_tile_instance_counts);
			native.layerMaskMapIdxsCount = plain::count_of<std::uint32_t>(data.layer_mask_map_idxs);
		}

		std::size_t size_st_disk_terrain_surface([[maybe_unused]] const asset_data::content::st_disk_terrain_surface& data)
		{
			std::size_t size = 0;
			if (data.high_lod_indices)
			{
				size += plain::sized(std::max<std::size_t>(data.high_lod_indices->size(), 1) * sizeof(std::uint16_t));
			}
			size += size_gfx_index_buffer(data.high_lod_index_buffer);
			size += size_st_terrain_grid(data.grid);
			size += size_gfx_placement(data.obj_to_wld);
			size += size_bounds(data.wld_bounds);
			if (data.layer_materials)
			{
				size += plain::sized(std::max<std::size_t>(data.layer_materials->size(), 1) * sizeof(StTerrainMaterialLayer));
				for (const auto& item : *data.layer_materials)
				{
					size += size_st_terrain_material_layer(item);
				}
			}
			if (data.displacement_scales)
			{
				size += plain::sized(std::max<std::size_t>(data.displacement_scales->size(), 1) * sizeof(StTerrainDisplacementScale));
				for (const auto& item : *data.displacement_scales)
				{
					size += size_st_terrain_displacement_scale(item);
				}
			}
			if (data.clutter_layers)
			{
				size += plain::sized(std::max<std::size_t>(data.clutter_layers->size(), 1) * sizeof(StTerrainClutterLayer));
				for (const auto& item : *data.clutter_layers)
				{
					size += size_st_terrain_clutter_layer(item);
				}
			}
			size += size_gfx_wrapped_buffer(data.clutter_layers_buffer);
			size += size_st_clutter_sample_bitmask(data.clutter_sample_bitmask);
			if (data.clutter_tile_instance_counts)
			{
				size += plain::sized(std::max<std::size_t>(data.clutter_tile_instance_counts->size(), 1) * sizeof(std::uint32_t));
			}
			if (data.layer_mask_map_idxs)
			{
				size += plain::sized(std::max<std::size_t>(data.layer_mask_map_idxs->size(), 1) * sizeof(std::uint16_t));
			}
			return size;
		}

		asset_data::content::st_terrain_grid_level read_st_terrain_grid_level(const StTerrainGridLevel& native)
		{
			plain::trace("read_st_terrain_grid_level");
			asset_data::content::st_terrain_grid_level data;
			data.width = native.width;
			data.height = native.height;
			plain::require_zero(native.nodes, "StTerrainGridLevel.nodes");
			return data;
		}

		void write_st_terrain_grid_level(StTerrainGridLevel& native, const asset_data::content::st_terrain_grid_level& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.width = data.width;
			native.height = data.height;
		}

		std::size_t size_st_terrain_grid_level([[maybe_unused]] const asset_data::content::st_terrain_grid_level& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::hlsl_terrain_layer_data_code read_hlsl_terrain_layer_data_code(const HLSL_TerrainLayerDataCode& native)
		{
			plain::trace("read_hlsl_terrain_layer_data_code");
			asset_data::content::hlsl_terrain_layer_data_code data;
			data.texture_transform = [&] { std::array<std::array<float, 4>, 2> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = plain::read_scalars<float, 4>(&native.textureTransform[i]); } return out; }();
			data.flags = native.flags;
			data.alpha_layer = native.alphaLayer;
			data.texture_scale = plain::read_scalars<float, 2>(&native.textureScale);
			data.detail_scale = plain::read_scalars<float, 2>(&native.detailScale);
			data.color_tint = plain::read_scalars<float, 3>(&native.colorTint);
			data.alpha_reveal_soft_edge = native.alphaRevealSoftEdge;
			data.detail_scale_height = native.detailScaleHeight;
			data.normal_map_packed_scale_bias = native.normalMapPackedScaleBias;
			data.albedo_map_packed_scale_bias = native.albedoMapPackedScaleBias;
			data.color_sampler = native.colorSampler;
			data.color_map = native.colorMap;
			data.tint_mask = native.tintMask;
			data.normal_sampler = native.normalSampler;
			data.normal_map = native.normalMap;
			data.reveal_sampler = native.revealSampler;
			data.reveal_map = native.revealMap;
			data.emissive_map = native.emissiveMap;
			data.transmittance_map = native.transmittanceMap;
			data.thickness_map = native.thicknessMap;
			data.displacement_map = native.displacementMap;
			data.displacement_scale = native.displacementScale;
			data.grass_height = native.grassHeight;
			data.grass_height_variance = native.grassHeightVariance;
			data.grass_width = native.grassWidth;
			data.grass_width_variance = native.grassWidthVariance;
			data.grass_tilt = native.grassTilt;
			data.grass_tilt_variance = native.grassTiltVariance;
			data.grass_bend = native.grassBend;
			return data;
		}

		void write_hlsl_terrain_layer_data_code(HLSL_TerrainLayerDataCode& native, const asset_data::content::hlsl_terrain_layer_data_code& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			for (std::size_t i = 0; i < data.texture_transform.size(); ++i)
			{
				plain::write_scalars(&native.textureTransform[i], data.texture_transform[i]);
			}
			native.flags = data.flags;
			native.alphaLayer = data.alpha_layer;
			plain::write_scalars(&native.textureScale, data.texture_scale);
			plain::write_scalars(&native.detailScale, data.detail_scale);
			plain::write_scalars(&native.colorTint, data.color_tint);
			native.alphaRevealSoftEdge = data.alpha_reveal_soft_edge;
			native.detailScaleHeight = data.detail_scale_height;
			native.normalMapPackedScaleBias = data.normal_map_packed_scale_bias;
			native.albedoMapPackedScaleBias = data.albedo_map_packed_scale_bias;
			native.colorSampler = data.color_sampler;
			native.colorMap = data.color_map;
			native.tintMask = data.tint_mask;
			native.normalSampler = data.normal_sampler;
			native.normalMap = data.normal_map;
			native.revealSampler = data.reveal_sampler;
			native.revealMap = data.reveal_map;
			native.emissiveMap = data.emissive_map;
			native.transmittanceMap = data.transmittance_map;
			native.thicknessMap = data.thickness_map;
			native.displacementMap = data.displacement_map;
			native.displacementScale = data.displacement_scale;
			native.grassHeight = data.grass_height;
			native.grassHeightVariance = data.grass_height_variance;
			native.grassWidth = data.grass_width;
			native.grassWidthVariance = data.grass_width_variance;
			native.grassTilt = data.grass_tilt;
			native.grassTiltVariance = data.grass_tilt_variance;
			native.grassBend = data.grass_bend;
		}

		std::size_t size_hlsl_terrain_layer_data_code([[maybe_unused]] const asset_data::content::hlsl_terrain_layer_data_code& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::st_terrain_material_layer read_st_terrain_material_layer(const StTerrainMaterialLayer& native)
		{
			plain::trace("read_st_terrain_material_layer");
			asset_data::content::st_terrain_material_layer data;
			data.material = plain::read_asset(ASSET_TYPE_MATERIAL, native.material);
			data.material_texture_idxs = plain::read_fixed_with<4>(native.materialTextureIdxs, [&](const auto& item) { return item; });
			data.st_from_xyz = [&] { std::array<std::array<float, 4>, 2> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = plain::read_scalars<float, 4>(&native.stFromXYZ[i]); } return out; }();
			data.himip_radius_inv_sq = native.himipRadiusInvSq;
			data.layer = native.layer;
			data.decal_sort = native.decalSort;
			data.dynamic_uvs = native.dynamicUvs;
			data.layer_data = read_hlsl_terrain_layer_data_code(native.layerData);
			return data;
		}

		void write_st_terrain_material_layer(StTerrainMaterialLayer& native, const asset_data::content::st_terrain_material_layer& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.material = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, data.material);
			if (data.material_texture_idxs)
			{
				native.materialTextureIdxs = memory.manual_allocate<std::uint16_t>(plain::rounded(4 * sizeof(std::uint16_t)));
				for (std::size_t i = 0; i < 4; ++i)
				{
					native.materialTextureIdxs[i] = (*data.material_texture_idxs)[i];
				}
			}
			for (std::size_t i = 0; i < data.st_from_xyz.size(); ++i)
			{
				plain::write_scalars(&native.stFromXYZ[i], data.st_from_xyz[i]);
			}
			native.himipRadiusInvSq = data.himip_radius_inv_sq;
			native.layer = data.layer;
			native.decalSort = data.decal_sort;
			native.dynamicUvs = data.dynamic_uvs;
			write_hlsl_terrain_layer_data_code(native.layerData, data.layer_data, resolve, memory);
		}

		std::size_t size_st_terrain_material_layer([[maybe_unused]] const asset_data::content::st_terrain_material_layer& data)
		{
			std::size_t size = 0;
			if (data.material_texture_idxs)
			{
				size += plain::sized(4 * sizeof(std::uint16_t));
			}
			size += size_hlsl_terrain_layer_data_code(data.layer_data);
			return size;
		}

		asset_data::content::st_terrain_displacement_scale read_st_terrain_displacement_scale(const StTerrainDisplacementScale& native)
		{
			plain::trace("read_st_terrain_displacement_scale");
			asset_data::content::st_terrain_displacement_scale data;
			data.mask = native.mask;
			data.scale = native.scale;
			return data;
		}

		void write_st_terrain_displacement_scale(StTerrainDisplacementScale& native, const asset_data::content::st_terrain_displacement_scale& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.mask = data.mask;
			native.scale = data.scale;
		}

		std::size_t size_st_terrain_displacement_scale([[maybe_unused]] const asset_data::content::st_terrain_displacement_scale& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::st_terrain_clutter_layer read_st_terrain_clutter_layer(const StTerrainClutterLayer& native)
		{
			plain::trace("read_st_terrain_clutter_layer");
			asset_data::content::st_terrain_clutter_layer data;
			data.sample_offset = native.sampleOffset;
			data.bitmask_offset = native.bitmaskOffset;
			data.tile_count = plain::read_scalars<std::uint32_t, 2>(&native.tileCount);
			data.tile_size_uv = plain::read_scalars<float, 2>(&native.tileSizeUV);
			data.layer_size = plain::read_scalars<float, 2>(&native.layerSize);
			data.angle_min = plain::read_scalars<float, 3>(&native.angleMin);
			data.angle_max = plain::read_scalars<float, 3>(&native.angleMax);
			data.z_offset_min_max = plain::read_scalars<float, 2>(&native.zOffsetMinMax);
			data.scale_min_max = plain::read_scalars<float, 2>(&native.scaleMinMax);
			data.max_instance_count = native.maxInstanceCount;
			data.tile_instance_count_offset = native.tileInstanceCountOffset;
			data.use_surface_normal = native.useSurfaceNormal;
			return data;
		}

		void write_st_terrain_clutter_layer(StTerrainClutterLayer& native, const asset_data::content::st_terrain_clutter_layer& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.sampleOffset = data.sample_offset;
			native.bitmaskOffset = data.bitmask_offset;
			plain::write_scalars(&native.tileCount, data.tile_count);
			plain::write_scalars(&native.tileSizeUV, data.tile_size_uv);
			plain::write_scalars(&native.layerSize, data.layer_size);
			plain::write_scalars(&native.angleMin, data.angle_min);
			plain::write_scalars(&native.angleMax, data.angle_max);
			plain::write_scalars(&native.zOffsetMinMax, data.z_offset_min_max);
			plain::write_scalars(&native.scaleMinMax, data.scale_min_max);
			native.maxInstanceCount = data.max_instance_count;
			native.tileInstanceCountOffset = data.tile_instance_count_offset;
			native.useSurfaceNormal = data.use_surface_normal;
		}

		std::size_t size_st_terrain_clutter_layer([[maybe_unused]] const asset_data::content::st_terrain_clutter_layer& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::st_terrain_maps read_st_terrain_maps(const StTerrainMaps& native)
		{
			plain::trace("read_st_terrain_maps");
			asset_data::content::st_terrain_maps data;
			data.map_resolution = native.mapResolution;
			data.padded_resolution = native.paddedResolution;
			return data;
		}

		void write_st_terrain_maps(StTerrainMaps& native, const asset_data::content::st_terrain_maps& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.mapResolution = data.map_resolution;
			native.paddedResolution = data.padded_resolution;
		}

		std::size_t size_st_terrain_maps([[maybe_unused]] const asset_data::content::st_terrain_maps& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::st_lightmap read_st_lightmap(const StLightmap& native)
		{
			plain::trace("read_st_lightmap");
			asset_data::content::st_lightmap data;
			plain::require_zero(native.images, "StLightmap.images");
			return data;
		}

		void write_st_lightmap(StLightmap& native, const asset_data::content::st_lightmap& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
		}

		std::size_t size_st_lightmap([[maybe_unused]] const asset_data::content::st_lightmap& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::static_model_collision_compressed_model_list read_static_model_collision_compressed_model_list(const StaticModelCollisionCompressedModelList& native)
		{
			plain::trace("read_static_model_collision_compressed_model_list");
			asset_data::content::static_model_collision_compressed_model_list data;
			data.num_models = native.numModels;
			data.models = plain::read_array(native.models, native.numModels, "StaticModelCollisionCompressedModelList.models", [&](const auto& item) { return read_static_model_collision_compressed_model(item); });
			data.structure_memory_cost = native.structureMemoryCost;
			data.num_instances = native.numInstances;
			data.asset_cost = native.assetCost;
			return data;
		}

		void write_static_model_collision_compressed_model_list(StaticModelCollisionCompressedModelList& native, const asset_data::content::static_model_collision_compressed_model_list& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.numModels = data.num_models;
			native.models = plain::write_array<StaticModelCollisionCompressedModel>(data.models, memory, [&](auto& out, const auto& in) { write_static_model_collision_compressed_model(out, in, resolve, memory); });
			native.structureMemoryCost = data.structure_memory_cost;
			native.numInstances = data.num_instances;
			native.assetCost = data.asset_cost;
		}

		std::size_t size_static_model_collision_compressed_model_list([[maybe_unused]] const asset_data::content::static_model_collision_compressed_model_list& data)
		{
			std::size_t size = 0;
			if (data.models)
			{
				size += plain::sized(std::max<std::size_t>(data.models->size(), 1) * sizeof(StaticModelCollisionCompressedModel));
				for (const auto& item : *data.models)
				{
					size += size_static_model_collision_compressed_model(item);
				}
			}
			return size;
		}

		asset_data::content::collisiontile read_collisiontile(const CollisionTile& native)
		{
			plain::trace("read_collisiontile");
			asset_data::content::collisiontile data;
			data.name = plain::read_string(native.name);
			data.index = native.index;
			data.grid_id = std::to_array(native.gridId);
			data.origin = plain::read_scalars<float, 3>(&native.origin);
			data.static_model_collision = plain::read_single(native.staticModelCollision, [&](const auto& item) { return read_static_model_collision_compressed_model_list(item); });
			data.havok_tile_shape_data = plain::read_array(native.havokTileShapeData, native.havokTileShapeDataSize, "CollisionTile.havokTileShapeData", [&](const auto& item) { return item; });
			data.havok_tile_shape_pointer = plain::read_single(native.havokTileShapePointer, [&](const auto& item) { return item; });
			data.world_collision_shape_idx = native.worldCollisionShapeIdx;
			return data;
		}

		void write_collisiontile(CollisionTile& native, const asset_data::content::collisiontile& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.index = data.index;
			std::ranges::copy(data.grid_id, native.gridId);
			plain::write_scalars(&native.origin, data.origin);
			native.staticModelCollision = plain::write_single<StaticModelCollisionCompressedModelList>(data.static_model_collision, memory, [&](auto& out, const auto& in) { write_static_model_collision_compressed_model_list(out, in, resolve, memory); });
			native.havokTileShapeData = plain::write_array<char>(data.havok_tile_shape_data, memory, [&](auto& out, const auto& in) { out = in; });
			native.havokTileShapePointer = plain::write_single<char>(data.havok_tile_shape_pointer, memory, [&](auto& out, const auto& in) { out = in; });
			native.worldCollisionShapeIdx = data.world_collision_shape_idx;
			native.havokTileShapeDataSize = plain::count_of<std::uint32_t>(data.havok_tile_shape_data);
		}

		std::size_t size_collisiontile([[maybe_unused]] const asset_data::content::collisiontile& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.static_model_collision)
			{
				size += plain::sized(sizeof(StaticModelCollisionCompressedModelList));
				size += size_static_model_collision_compressed_model_list(*data.static_model_collision);
			}
			if (data.havok_tile_shape_data)
			{
				size += plain::sized(std::max<std::size_t>(data.havok_tile_shape_data->size(), 1) * sizeof(char));
			}
			if (data.havok_tile_shape_pointer)
			{
				size += plain::sized(sizeof(char));
			}
			return size;
		}

		asset_data::content::static_model_collision_compressed_model read_static_model_collision_compressed_model(const StaticModelCollisionCompressedModel& native)
		{
			plain::trace("read_static_model_collision_compressed_model");
			asset_data::content::static_model_collision_compressed_model data;
			data.name = plain::read_script_string(native.name);
			data.physics_asset = plain::read_asset(ASSET_TYPE_PHYSICSASSET, native.physicsAsset);
			data.detail_collision = plain::read_asset(ASSET_TYPE_XMODEL_DETAIL_COLLISION, native.detailCollision);
			plain::require_zero(native.shapes, "StaticModelCollisionCompressedModel.shapes");
			data.shape_tags = std::to_array(native.shapeTags);
			data.instances = plain::read_array(native.instances, native.numInstances, "StaticModelCollisionCompressedModel.instances", [&](const auto& item) { return read_static_model_collision_compressed_instance(item); });
			return data;
		}

		void write_static_model_collision_compressed_model(StaticModelCollisionCompressedModel& native, const asset_data::content::static_model_collision_compressed_model& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = static_cast<scr_string_t>(plain::write_script_string(data.name));
			native.physicsAsset = plain::resolve_asset<PhysicsAsset>(resolve, ASSET_TYPE_PHYSICSASSET, data.physics_asset);
			native.detailCollision = plain::resolve_asset<XModelDetailCollision>(resolve, ASSET_TYPE_XMODEL_DETAIL_COLLISION, data.detail_collision);
			std::ranges::copy(data.shape_tags, native.shapeTags);
			native.instances = plain::write_array<StaticModelCollisionCompressedInstance>(data.instances, memory, [&](auto& out, const auto& in) { write_static_model_collision_compressed_instance(out, in, resolve, memory); });
			native.numInstances = plain::count_of<std::uint32_t>(data.instances);
		}

		std::size_t size_static_model_collision_compressed_model([[maybe_unused]] const asset_data::content::static_model_collision_compressed_model& data)
		{
			std::size_t size = 0;
			if (data.instances)
			{
				size += plain::sized(std::max<std::size_t>(data.instances->size(), 1) * sizeof(StaticModelCollisionCompressedInstance));
				for (const auto& item : *data.instances)
				{
					size += size_static_model_collision_compressed_instance(item);
				}
			}
			return size;
		}

		asset_data::content::static_model_collision_compressed_instance read_static_model_collision_compressed_instance(const StaticModelCollisionCompressedInstance& native)
		{
			plain::trace("read_static_model_collision_compressed_instance");
			asset_data::content::static_model_collision_compressed_instance data;
			data.origin = std::to_array(native.origin);
			data.angles = std::to_array(native.angles);
			data.scale = native.scale;
			return data;
		}

		void write_static_model_collision_compressed_instance(StaticModelCollisionCompressedInstance& native, const asset_data::content::static_model_collision_compressed_instance& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			std::ranges::copy(data.origin, native.origin);
			std::ranges::copy(data.angles, native.angles);
			native.scale = data.scale;
		}

		std::size_t size_static_model_collision_compressed_instance([[maybe_unused]] const asset_data::content::static_model_collision_compressed_instance& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::vehicle_physics_sound_group read_vehicle_physics_sound_group(const VehiclePhysicsSoundGroup& native)
		{
			plain::trace("read_vehicle_physics_sound_group");
			asset_data::content::vehicle_physics_sound_group data;
			data.alias = read_snd_alias_lookup(native.alias);
			data.threshold = native.threshold;
			data.vmin = native.vmin;
			data.vmax = native.vmax;
			data.pmin = native.pmin;
			data.pmax = native.pmax;
			return data;
		}

		void write_vehicle_physics_sound_group(VehiclePhysicsSoundGroup& native, const asset_data::content::vehicle_physics_sound_group& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_snd_alias_lookup(native.alias, data.alias, resolve, memory);
			native.threshold = data.threshold;
			native.vmin = data.vmin;
			native.vmax = data.vmax;
			native.pmin = data.pmin;
			native.pmax = data.pmax;
		}

		std::size_t size_vehicle_physics_sound_group([[maybe_unused]] const asset_data::content::vehicle_physics_sound_group& data)
		{
			std::size_t size = 0;
			size += size_snd_alias_lookup(data.alias);
			return size;
		}

		asset_data::content::vehicle_physics_vfx_group read_vehicle_physics_vfx_group(const VehiclePhysicsVfxGroup& native)
		{
			plain::trace("read_vehicle_physics_vfx_group");
			asset_data::content::vehicle_physics_vfx_group data;
			data.surface_effects = plain::read_asset(ASSET_TYPE_FOOTSTEP_VFX, native.surfaceEffects);
			data.threshold = native.threshold;
			data.range0 = native.range0;
			data.range1 = native.range1;
			return data;
		}

		void write_vehicle_physics_vfx_group(VehiclePhysicsVfxGroup& native, const asset_data::content::vehicle_physics_vfx_group& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.surfaceEffects = plain::resolve_asset<FootstepVFX>(resolve, ASSET_TYPE_FOOTSTEP_VFX, data.surface_effects);
			native.threshold = data.threshold;
			native.range0 = data.range0;
			native.range1 = data.range1;
		}

		std::size_t size_vehicle_physics_vfx_group([[maybe_unused]] const asset_data::content::vehicle_physics_vfx_group& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::vehicle_physics_def read_vehicle_physics_def(const VehiclePhysicsDef& native)
		{
			plain::trace("read_vehicle_physics_def");
			asset_data::content::vehicle_physics_def data;
			data.physics_enabled = native.physicsEnabled;
			data.physics_netcode_type = plain::enum_name(asset_data::content::vehicle_physics_netcode_type_names, static_cast<std::int64_t>(native.physics_netcodeType), "VehiclePhysicsDef.physics_netcodeType");
			data.physics_game_profile = plain::enum_name(asset_data::content::vehicle_physics_game_profile_names, static_cast<std::int64_t>(native.physics_gameProfile), "VehiclePhysicsDef.physics_gameProfile");
			data.physics_anim_profile = plain::enum_name(asset_data::content::vehicle_physics_anim_profile_names, static_cast<std::int64_t>(native.physics_animProfile), "VehiclePhysicsDef.physics_animProfile");
			data.physics_num_wheels = native.physics_numWheels;
			data.physics_axle_rigid_bodies = native.physics_axleRigidBodies;
			data.physics_axle_body_padding = native.physics_axleBodyPadding;
			data.physics_axle_body_sides = native.physics_axleBodySides;
			data.physics_wheel_bones = [&] { std::array<std::optional<std::string>, 12> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = plain::read_script_string(native.physics_wheelBones[i]); } return out; }();
			data.physics_wheel_radius = native.physics_wheelRadius;
			data.physics_susp_stiffness = native.physics_suspStiffness;
			data.physics_susp_damping = native.physics_suspDamping;
			data.physics_susp_noise_amp = native.physics_suspNoiseAmp;
			data.physics_frontal_suspension = native.physics_frontalSuspension;
			data.physics_ackermann_ratio = native.physics_ackermannRatio;
			data.physics_hard_point_offset = plain::read_scalars<float, 3>(&native.physics_hardPointOffset);
			data.physics_com_offset = plain::read_scalars<float, 3>(&native.physics_comOffset);
			data.physics_mass_factor_contact = plain::read_scalars<float, 4>(&native.physics_massFactorContact);
			data.physics_inertia_diagonal = plain::read_scalars<float, 4>(&native.physics_inertiaDiagonal);
			data.physics_extra_gravity_factor = native.physics_extraGravityFactor;
			data.physics_extra_gravity_factor_air = native.physics_extraGravityFactorAir;
			data.physics_cam_fov_delta = native.physics_camFovDelta;
			data.physics_cam_pitch_base = native.physics_camPitchBase;
			data.physics_cam_pitch_dynamic = native.physics_camPitchDynamic;
			data.physics_cam_range_add = native.physics_camRangeAdd;
			data.physics_steering_factor = native.physics_steeringFactor;
			data.physics_steering_force = native.physics_steeringForce;
			data.physics_steering_max_yaw_speed = native.physics_steeringMaxYawSpeed;
			data.physics_steering_offset_point = native.physics_steeringOffsetPoint;
			data.physics_steering_offset_point_up = native.physics_steeringOffsetPointUp;
			data.physics_steering_return_speed = native.physics_steeringReturnSpeed;
			data.physics_steering_change_dir_speed = native.physics_steeringChangeDirSpeed;
			data.physics_steering_handbrake = native.physics_steeringHandbrake;
			data.physics_steering_speed = native.physics_steeringSpeed;
			data.physics_steering_speed_increase = native.physics_steeringSpeedIncrease;
			data.physics_engine_offset_point = native.physics_engineOffsetPoint;
			data.physics_engine_reduction_on_hb = native.physics_engineReductionOnHB;
			data.physics_rolling_friction = native.physics_rollingFriction;
			data.physics_lateral_friction = native.physics_lateralFriction;
			data.physics_friction_base = native.physics_frictionBase;
			data.physics_friction_recover_speed_tgt = native.physics_frictionRecoverSpeedTgt;
			data.physics_friction_fw_redirect = native.physics_frictionFwRedirect;
			data.physics_friction_lost = native.physics_frictionLost;
			data.physics_friction_handbrake = native.physics_frictionHandbrake;
			data.physics_friction_speed_to_block = native.physics_frictionSpeedToBlock;
			data.physics_decel = native.physics_decel;
			data.physics_min_speed = native.physics_minSpeed;
			data.physics_stabilize_roll = native.physics_stabilizeRoll;
			data.physics_stabilize_pitch = native.physics_stabilizePitch;
			data.physics_contents_as_missile = native.physics_contentsAsMissile;
			data.physics_pitch_inversion = native.physics_pitchInversion;
			data.physics_control_mode = native.physics_controlMode;
			data.physics_time_after_coll = native.physics_timeAfterColl;
			data.physics_cycle_cam_button = native.physics_cycleCamButton;
			data.physics_boost_button = native.physics_boostButton;
			data.physics_boost_speed = native.physics_boostSpeed;
			data.physics_rcp_boost_accel = native.physics_rcpBoostAccel;
			data.physics_rcp_boost_decel = native.physics_rcpBoostDecel;
			data.physics_hold_to_boost = native.physics_holdToBoost;
			data.physics_rcp_boost_sound = read_snd_alias_lookup(native.physics_rcpBoostSound);
			data.physics_rcp_boost_shake_cam = plain::read_scalars<float, 3>(&native.physics_rcpBoostShakeCam);
			data.physics_rcp_use_change_dir_logic = native.physics_rcpUseChangeDirLogic;
			data.physics_rcp_use_change_dir_logic_pitch = native.physics_rcpUseChangeDirLogicPitch;
			data.physics_rcp_use_roll_for_yaw_speed = native.physics_rcpUseRollForYawSpeed;
			data.physics_rcp_overshoot_protection = native.physics_rcpOvershootProtection;
			data.physics_rcp_pitch_turn_speed = native.physics_rcpPitchTurnSpeed;
			data.physics_rcp_pitch_change_dir_factor = native.physics_rcpPitchChangeDirFactor;
			data.physics_rcp_pitch_lerp_speed = native.physics_rcpPitchLerpSpeed;
			data.physics_rcp_pitch_lerp_speed_change_dir = native.physics_rcpPitchLerpSpeedChangeDir;
			data.physics_rcp_pitch_lerp_speed_returning = native.physics_rcpPitchLerpSpeedReturning;
			data.physics_rcp_pitch_max_angle = native.physics_rcpPitchMaxAngle;
			data.physics_rcp_yaw_turn_speed = native.physics_rcpYawTurnSpeed;
			data.physics_rcp_yaw_change_dir_factor = native.physics_rcpYawChangeDirFactor;
			data.physics_rcp_yaw_lerp_speed = native.physics_rcpYawLerpSpeed;
			data.physics_rcp_yaw_lerp_speed_change_dir = native.physics_rcpYawLerpSpeedChangeDir;
			data.physics_rcp_yaw_lerp_speed_returning = native.physics_rcpYawLerpSpeedReturning;
			data.physics_rcp_roll_turn_speed = native.physics_rcpRollTurnSpeed;
			data.physics_rcp_roll_lerp_speed_steering = native.physics_rcpRollLerpSpeedSteering;
			data.physics_rcp_roll_lerp_speed_change_dir = native.physics_rcpRollLerpSpeedChangeDir;
			data.physics_rcp_roll_lerp_speed_returning = native.physics_rcpRollLerpSpeedReturning;
			data.physics_rcp_roll_max_angle = native.physics_rcpRollMaxAngle;
			data.physics_rcp_accel_going_down = native.physics_rcpAccelGoingDown;
			data.physics_rcp_decel_going_up = native.physics_rcpDecelGoingUp;
			data.physics_rcp_trace_ahead = native.physics_rcpTraceAhead;
			data.physics_rcp_cam1st_attach_to_tag_player = native.physics_rcpCam1stAttachToTagPlayer;
			data.physics_rcp_cam1st_noise_scale = plain::read_scalars<float, 2>(&native.physics_rcpCam1stNoiseScale);
			data.physics_rcp_cam1st_roll_speed = native.physics_rcpCam1stRollSpeed;
			data.physics_rcp_cam1st_roll_percentage = native.physics_rcpCam1stRollPercentage;
			data.physics_com_adjust_threshold = native.physics_comAdjustThreshold;
			data.physics_com_adjust_vert_disp = native.physics_comAdjustVertDisp;
			data.physics_players_weight_factor = native.physics_playersWeightFactor;
			data.physics_time_to_accept_input_on_start = native.physics_timeToAcceptInputOnStart;
			data.physics_view_direction_help = native.physics_viewDirectionHelp;
			data.physics_stabilize_pitch_speed = native.physics_stabilizePitchSpeed;
			data.physics_stabilize_roll_speed = native.physics_stabilizeRollSpeed;
			data.spin_wheel_bones = plain::enum_name(asset_data::content::vehicle_wheel_spin_bone_type_names, static_cast<std::int64_t>(native.spinWheelBones), "VehiclePhysicsDef.spinWheelBones");
			data.spin_wheel_angle_offset = native.spinWheelAngleOffset;
			data.treads_enabled = native.treads_enabled;
			data.treads_bone_count = native.treads_boneCount;
			data.treads_first_bones = [&] { std::array<std::optional<std::string>, 2> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = plain::read_script_string(native.treads_firstBones[i]); } return out; }();
			data.friction_braking = native.frictionBraking;
			data.suspension_travel = native.suspensionTravel;
			data.max_steering_angle = native.maxSteeringAngle;
			data.steering_lerp = native.steeringLerp;
			data.min_steering_scale = native.minSteeringScale;
			data.min_steering_speed = native.minSteeringSpeed;
			data.disable_wheels_turning = native.disableWheelsTurning;
			data.disable_wheels_spinning = native.disableWheelsSpinning;
			data.minimum_wheel_cast_fraction = native.minimumWheelCastFraction;
			data.handbrake_threshold = native.handbrakeThreshold;
			data.minimum_jolt_for_notify = native.minimumJoltForNotify;
			data.path_constraint_strength_fwd = native.pathConstraintStrengthFwd;
			data.path_constraint_strength_side = native.pathConstraintStrengthSide;
			data.path_constraint_damp_fwd = native.pathConstraintDampFwd;
			data.path_constraint_damp_side = native.pathConstraintDampSide;
			data.crash_point_offset_fwd = native.crashPointOffsetFwd;
			data.crash_point_offset_up = native.crashPointOffsetUp;
			data.crash_impulse_side = native.crashImpulseSide;
			data.crash_impulse_up = native.crashImpulseUp;
			data.wrecked_mass_scale = native.wreckedMassScale;
			data.snd_impact_hard = read_vehicle_physics_sound_group(native.sndImpactHard);
			data.snd_impact_exp = native.sndImpactExp;
			data.snd_impact_light = read_vehicle_physics_sound_group(native.sndImpactLight);
			data.snd_driving_fast = read_vehicle_physics_sound_group(native.sndDrivingFast);
			data.snd_driving_slow = read_vehicle_physics_sound_group(native.sndDrivingSlow);
			data.snd_suspension = read_vehicle_physics_sound_group(native.sndSuspension);
			data.snd_braking_hard = read_vehicle_physics_sound_group(native.sndBrakingHard);
			data.snd_braking_light = read_vehicle_physics_sound_group(native.sndBrakingLight);
			data.snd_drifting = read_vehicle_physics_sound_group(native.sndDrifting);
			data.snd_skidding = read_vehicle_physics_sound_group(native.sndSkidding);
			data.snd_dlc1 = read_vehicle_physics_sound_group(native.sndDlc1);
			data.snd_dlc2 = read_vehicle_physics_sound_group(native.sndDlc2);
			data.snd_dlc3 = read_vehicle_physics_sound_group(native.sndDlc3);
			data.snd_dlc4 = read_vehicle_physics_sound_group(native.sndDlc4);
			data.vfx_impact_hard = read_vehicle_physics_vfx_group(native.vfxImpactHard);
			data.vfx_impact_light = read_vehicle_physics_vfx_group(native.vfxImpactLight);
			data.vfx_driving_fast = read_vehicle_physics_vfx_group(native.vfxDrivingFast);
			data.vfx_driving_slow = read_vehicle_physics_vfx_group(native.vfxDrivingSlow);
			data.vfx_braking_hard = read_vehicle_physics_vfx_group(native.vfxBrakingHard);
			data.vfx_braking_light = read_vehicle_physics_vfx_group(native.vfxBrakingLight);
			data.vfx_drifting = read_vehicle_physics_vfx_group(native.vfxDrifting);
			data.vfx_skidding = read_vehicle_physics_vfx_group(native.vfxSkidding);
			data.vfx_driving_flat_fast = read_vehicle_physics_vfx_group(native.vfxDrivingFlatFast);
			data.vfx_driving_flat_slow = read_vehicle_physics_vfx_group(native.vfxDrivingFlatSlow);
			data.vfx_dlc3 = read_vehicle_physics_vfx_group(native.vfxDlc3);
			data.vfx_dlc4 = read_vehicle_physics_vfx_group(native.vfxDlc4);
			return data;
		}

		void write_vehicle_physics_def(VehiclePhysicsDef& native, const asset_data::content::vehicle_physics_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.physicsEnabled = data.physics_enabled;
			native.physics_netcodeType = static_cast<VehiclePhysicsNetcodeType>(plain::enum_value(asset_data::content::vehicle_physics_netcode_type_names, data.physics_netcode_type, "VehiclePhysicsDef.physics_netcodeType"));
			native.physics_gameProfile = static_cast<VehiclePhysicsGameProfile>(plain::enum_value(asset_data::content::vehicle_physics_game_profile_names, data.physics_game_profile, "VehiclePhysicsDef.physics_gameProfile"));
			native.physics_animProfile = static_cast<VehiclePhysicsAnimProfile>(plain::enum_value(asset_data::content::vehicle_physics_anim_profile_names, data.physics_anim_profile, "VehiclePhysicsDef.physics_animProfile"));
			native.physics_numWheels = data.physics_num_wheels;
			native.physics_axleRigidBodies = data.physics_axle_rigid_bodies;
			native.physics_axleBodyPadding = data.physics_axle_body_padding;
			native.physics_axleBodySides = data.physics_axle_body_sides;
			for (std::size_t i = 0; i < data.physics_wheel_bones.size(); ++i)
			{
				native.physics_wheelBones[i] = static_cast<scr_string_t>(plain::write_script_string(data.physics_wheel_bones[i]));
			}
			native.physics_wheelRadius = data.physics_wheel_radius;
			native.physics_suspStiffness = data.physics_susp_stiffness;
			native.physics_suspDamping = data.physics_susp_damping;
			native.physics_suspNoiseAmp = data.physics_susp_noise_amp;
			native.physics_frontalSuspension = data.physics_frontal_suspension;
			native.physics_ackermannRatio = data.physics_ackermann_ratio;
			plain::write_scalars(&native.physics_hardPointOffset, data.physics_hard_point_offset);
			plain::write_scalars(&native.physics_comOffset, data.physics_com_offset);
			plain::write_scalars(&native.physics_massFactorContact, data.physics_mass_factor_contact);
			plain::write_scalars(&native.physics_inertiaDiagonal, data.physics_inertia_diagonal);
			native.physics_extraGravityFactor = data.physics_extra_gravity_factor;
			native.physics_extraGravityFactorAir = data.physics_extra_gravity_factor_air;
			native.physics_camFovDelta = data.physics_cam_fov_delta;
			native.physics_camPitchBase = data.physics_cam_pitch_base;
			native.physics_camPitchDynamic = data.physics_cam_pitch_dynamic;
			native.physics_camRangeAdd = data.physics_cam_range_add;
			native.physics_steeringFactor = data.physics_steering_factor;
			native.physics_steeringForce = data.physics_steering_force;
			native.physics_steeringMaxYawSpeed = data.physics_steering_max_yaw_speed;
			native.physics_steeringOffsetPoint = data.physics_steering_offset_point;
			native.physics_steeringOffsetPointUp = data.physics_steering_offset_point_up;
			native.physics_steeringReturnSpeed = data.physics_steering_return_speed;
			native.physics_steeringChangeDirSpeed = data.physics_steering_change_dir_speed;
			native.physics_steeringHandbrake = data.physics_steering_handbrake;
			native.physics_steeringSpeed = data.physics_steering_speed;
			native.physics_steeringSpeedIncrease = data.physics_steering_speed_increase;
			native.physics_engineOffsetPoint = data.physics_engine_offset_point;
			native.physics_engineReductionOnHB = data.physics_engine_reduction_on_hb;
			native.physics_rollingFriction = data.physics_rolling_friction;
			native.physics_lateralFriction = data.physics_lateral_friction;
			native.physics_frictionBase = data.physics_friction_base;
			native.physics_frictionRecoverSpeedTgt = data.physics_friction_recover_speed_tgt;
			native.physics_frictionFwRedirect = data.physics_friction_fw_redirect;
			native.physics_frictionLost = data.physics_friction_lost;
			native.physics_frictionHandbrake = data.physics_friction_handbrake;
			native.physics_frictionSpeedToBlock = data.physics_friction_speed_to_block;
			native.physics_decel = data.physics_decel;
			native.physics_minSpeed = data.physics_min_speed;
			native.physics_stabilizeRoll = data.physics_stabilize_roll;
			native.physics_stabilizePitch = data.physics_stabilize_pitch;
			native.physics_contentsAsMissile = data.physics_contents_as_missile;
			native.physics_pitchInversion = data.physics_pitch_inversion;
			native.physics_controlMode = data.physics_control_mode;
			native.physics_timeAfterColl = data.physics_time_after_coll;
			native.physics_cycleCamButton = data.physics_cycle_cam_button;
			native.physics_boostButton = data.physics_boost_button;
			native.physics_boostSpeed = data.physics_boost_speed;
			native.physics_rcpBoostAccel = data.physics_rcp_boost_accel;
			native.physics_rcpBoostDecel = data.physics_rcp_boost_decel;
			native.physics_holdToBoost = data.physics_hold_to_boost;
			write_snd_alias_lookup(native.physics_rcpBoostSound, data.physics_rcp_boost_sound, resolve, memory);
			plain::write_scalars(&native.physics_rcpBoostShakeCam, data.physics_rcp_boost_shake_cam);
			native.physics_rcpUseChangeDirLogic = data.physics_rcp_use_change_dir_logic;
			native.physics_rcpUseChangeDirLogicPitch = data.physics_rcp_use_change_dir_logic_pitch;
			native.physics_rcpUseRollForYawSpeed = data.physics_rcp_use_roll_for_yaw_speed;
			native.physics_rcpOvershootProtection = data.physics_rcp_overshoot_protection;
			native.physics_rcpPitchTurnSpeed = data.physics_rcp_pitch_turn_speed;
			native.physics_rcpPitchChangeDirFactor = data.physics_rcp_pitch_change_dir_factor;
			native.physics_rcpPitchLerpSpeed = data.physics_rcp_pitch_lerp_speed;
			native.physics_rcpPitchLerpSpeedChangeDir = data.physics_rcp_pitch_lerp_speed_change_dir;
			native.physics_rcpPitchLerpSpeedReturning = data.physics_rcp_pitch_lerp_speed_returning;
			native.physics_rcpPitchMaxAngle = data.physics_rcp_pitch_max_angle;
			native.physics_rcpYawTurnSpeed = data.physics_rcp_yaw_turn_speed;
			native.physics_rcpYawChangeDirFactor = data.physics_rcp_yaw_change_dir_factor;
			native.physics_rcpYawLerpSpeed = data.physics_rcp_yaw_lerp_speed;
			native.physics_rcpYawLerpSpeedChangeDir = data.physics_rcp_yaw_lerp_speed_change_dir;
			native.physics_rcpYawLerpSpeedReturning = data.physics_rcp_yaw_lerp_speed_returning;
			native.physics_rcpRollTurnSpeed = data.physics_rcp_roll_turn_speed;
			native.physics_rcpRollLerpSpeedSteering = data.physics_rcp_roll_lerp_speed_steering;
			native.physics_rcpRollLerpSpeedChangeDir = data.physics_rcp_roll_lerp_speed_change_dir;
			native.physics_rcpRollLerpSpeedReturning = data.physics_rcp_roll_lerp_speed_returning;
			native.physics_rcpRollMaxAngle = data.physics_rcp_roll_max_angle;
			native.physics_rcpAccelGoingDown = data.physics_rcp_accel_going_down;
			native.physics_rcpDecelGoingUp = data.physics_rcp_decel_going_up;
			native.physics_rcpTraceAhead = data.physics_rcp_trace_ahead;
			native.physics_rcpCam1stAttachToTagPlayer = data.physics_rcp_cam1st_attach_to_tag_player;
			plain::write_scalars(&native.physics_rcpCam1stNoiseScale, data.physics_rcp_cam1st_noise_scale);
			native.physics_rcpCam1stRollSpeed = data.physics_rcp_cam1st_roll_speed;
			native.physics_rcpCam1stRollPercentage = data.physics_rcp_cam1st_roll_percentage;
			native.physics_comAdjustThreshold = data.physics_com_adjust_threshold;
			native.physics_comAdjustVertDisp = data.physics_com_adjust_vert_disp;
			native.physics_playersWeightFactor = data.physics_players_weight_factor;
			native.physics_timeToAcceptInputOnStart = data.physics_time_to_accept_input_on_start;
			native.physics_viewDirectionHelp = data.physics_view_direction_help;
			native.physics_stabilizePitchSpeed = data.physics_stabilize_pitch_speed;
			native.physics_stabilizeRollSpeed = data.physics_stabilize_roll_speed;
			native.spinWheelBones = static_cast<VehicleWheelSpinBoneType>(plain::enum_value(asset_data::content::vehicle_wheel_spin_bone_type_names, data.spin_wheel_bones, "VehiclePhysicsDef.spinWheelBones"));
			native.spinWheelAngleOffset = data.spin_wheel_angle_offset;
			native.treads_enabled = data.treads_enabled;
			native.treads_boneCount = data.treads_bone_count;
			for (std::size_t i = 0; i < data.treads_first_bones.size(); ++i)
			{
				native.treads_firstBones[i] = static_cast<scr_string_t>(plain::write_script_string(data.treads_first_bones[i]));
			}
			native.frictionBraking = data.friction_braking;
			native.suspensionTravel = data.suspension_travel;
			native.maxSteeringAngle = data.max_steering_angle;
			native.steeringLerp = data.steering_lerp;
			native.minSteeringScale = data.min_steering_scale;
			native.minSteeringSpeed = data.min_steering_speed;
			native.disableWheelsTurning = data.disable_wheels_turning;
			native.disableWheelsSpinning = data.disable_wheels_spinning;
			native.minimumWheelCastFraction = data.minimum_wheel_cast_fraction;
			native.handbrakeThreshold = data.handbrake_threshold;
			native.minimumJoltForNotify = data.minimum_jolt_for_notify;
			native.pathConstraintStrengthFwd = data.path_constraint_strength_fwd;
			native.pathConstraintStrengthSide = data.path_constraint_strength_side;
			native.pathConstraintDampFwd = data.path_constraint_damp_fwd;
			native.pathConstraintDampSide = data.path_constraint_damp_side;
			native.crashPointOffsetFwd = data.crash_point_offset_fwd;
			native.crashPointOffsetUp = data.crash_point_offset_up;
			native.crashImpulseSide = data.crash_impulse_side;
			native.crashImpulseUp = data.crash_impulse_up;
			native.wreckedMassScale = data.wrecked_mass_scale;
			write_vehicle_physics_sound_group(native.sndImpactHard, data.snd_impact_hard, resolve, memory);
			native.sndImpactExp = data.snd_impact_exp;
			write_vehicle_physics_sound_group(native.sndImpactLight, data.snd_impact_light, resolve, memory);
			write_vehicle_physics_sound_group(native.sndDrivingFast, data.snd_driving_fast, resolve, memory);
			write_vehicle_physics_sound_group(native.sndDrivingSlow, data.snd_driving_slow, resolve, memory);
			write_vehicle_physics_sound_group(native.sndSuspension, data.snd_suspension, resolve, memory);
			write_vehicle_physics_sound_group(native.sndBrakingHard, data.snd_braking_hard, resolve, memory);
			write_vehicle_physics_sound_group(native.sndBrakingLight, data.snd_braking_light, resolve, memory);
			write_vehicle_physics_sound_group(native.sndDrifting, data.snd_drifting, resolve, memory);
			write_vehicle_physics_sound_group(native.sndSkidding, data.snd_skidding, resolve, memory);
			write_vehicle_physics_sound_group(native.sndDlc1, data.snd_dlc1, resolve, memory);
			write_vehicle_physics_sound_group(native.sndDlc2, data.snd_dlc2, resolve, memory);
			write_vehicle_physics_sound_group(native.sndDlc3, data.snd_dlc3, resolve, memory);
			write_vehicle_physics_sound_group(native.sndDlc4, data.snd_dlc4, resolve, memory);
			write_vehicle_physics_vfx_group(native.vfxImpactHard, data.vfx_impact_hard, resolve, memory);
			write_vehicle_physics_vfx_group(native.vfxImpactLight, data.vfx_impact_light, resolve, memory);
			write_vehicle_physics_vfx_group(native.vfxDrivingFast, data.vfx_driving_fast, resolve, memory);
			write_vehicle_physics_vfx_group(native.vfxDrivingSlow, data.vfx_driving_slow, resolve, memory);
			write_vehicle_physics_vfx_group(native.vfxBrakingHard, data.vfx_braking_hard, resolve, memory);
			write_vehicle_physics_vfx_group(native.vfxBrakingLight, data.vfx_braking_light, resolve, memory);
			write_vehicle_physics_vfx_group(native.vfxDrifting, data.vfx_drifting, resolve, memory);
			write_vehicle_physics_vfx_group(native.vfxSkidding, data.vfx_skidding, resolve, memory);
			write_vehicle_physics_vfx_group(native.vfxDrivingFlatFast, data.vfx_driving_flat_fast, resolve, memory);
			write_vehicle_physics_vfx_group(native.vfxDrivingFlatSlow, data.vfx_driving_flat_slow, resolve, memory);
			write_vehicle_physics_vfx_group(native.vfxDlc3, data.vfx_dlc3, resolve, memory);
			write_vehicle_physics_vfx_group(native.vfxDlc4, data.vfx_dlc4, resolve, memory);
		}

		std::size_t size_vehicle_physics_def([[maybe_unused]] const asset_data::content::vehicle_physics_def& data)
		{
			std::size_t size = 0;
			size += size_snd_alias_lookup(data.physics_rcp_boost_sound);
			size += size_vehicle_physics_sound_group(data.snd_impact_hard);
			size += size_vehicle_physics_sound_group(data.snd_impact_light);
			size += size_vehicle_physics_sound_group(data.snd_driving_fast);
			size += size_vehicle_physics_sound_group(data.snd_driving_slow);
			size += size_vehicle_physics_sound_group(data.snd_suspension);
			size += size_vehicle_physics_sound_group(data.snd_braking_hard);
			size += size_vehicle_physics_sound_group(data.snd_braking_light);
			size += size_vehicle_physics_sound_group(data.snd_drifting);
			size += size_vehicle_physics_sound_group(data.snd_skidding);
			size += size_vehicle_physics_sound_group(data.snd_dlc1);
			size += size_vehicle_physics_sound_group(data.snd_dlc2);
			size += size_vehicle_physics_sound_group(data.snd_dlc3);
			size += size_vehicle_physics_sound_group(data.snd_dlc4);
			size += size_vehicle_physics_vfx_group(data.vfx_impact_hard);
			size += size_vehicle_physics_vfx_group(data.vfx_impact_light);
			size += size_vehicle_physics_vfx_group(data.vfx_driving_fast);
			size += size_vehicle_physics_vfx_group(data.vfx_driving_slow);
			size += size_vehicle_physics_vfx_group(data.vfx_braking_hard);
			size += size_vehicle_physics_vfx_group(data.vfx_braking_light);
			size += size_vehicle_physics_vfx_group(data.vfx_drifting);
			size += size_vehicle_physics_vfx_group(data.vfx_skidding);
			size += size_vehicle_physics_vfx_group(data.vfx_driving_flat_fast);
			size += size_vehicle_physics_vfx_group(data.vfx_driving_flat_slow);
			size += size_vehicle_physics_vfx_group(data.vfx_dlc3);
			size += size_vehicle_physics_vfx_group(data.vfx_dlc4);
			return size;
		}

		asset_data::content::spaceship_anim_parts read_spaceship_anim_parts(const SpaceshipAnimParts& native)
		{
			plain::trace("read_spaceship_anim_parts");
			asset_data::content::spaceship_anim_parts data;
			data.anim = [&] { std::array<std::optional<std::string>, 2> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = plain::read_asset(ASSET_TYPE_XANIMPARTS, native.anim[i]); } return out; }();
			return data;
		}

		void write_spaceship_anim_parts(SpaceshipAnimParts& native, const asset_data::content::spaceship_anim_parts& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			for (std::size_t i = 0; i < data.anim.size(); ++i)
			{
				native.anim[i] = plain::resolve_asset<XAnimParts>(resolve, ASSET_TYPE_XANIMPARTS, data.anim[i]);
			}
		}

		std::size_t size_spaceship_anim_parts([[maybe_unused]] const asset_data::content::spaceship_anim_parts& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::vehicle_physics_rev_modifier read_vehicle_physics_rev_modifier(const VehiclePhysicsRevModifier& native)
		{
			plain::trace("read_vehicle_physics_rev_modifier");
			asset_data::content::vehicle_physics_rev_modifier data;
			data.skid_snd_contrib = native.skidSndContrib;
			data.noise_ampl = native.noiseAmpl;
			data.sinewave_freq = native.sinewaveFreq;
			data.sinewave_ampl = native.sinewaveAmpl;
			data.rpm_max = native.rpmMax;
			data.rpm_scale = native.rpmScale;
			data.rpm_speed_ema_samples = native.rpmSpeedEmaSamples;
			data.inclined_threshold = native.inclinedThreshold;
			data.gear_count = native.gearCount;
			data.incl_contrib = native.inclContrib;
			data.max_spin_speed_rpm_fw = native.maxSpinSpeedRPMFw;
			data.max_spin_speed_rpm_bw = native.maxSpinSpeedRPMBw;
			return data;
		}

		void write_vehicle_physics_rev_modifier(VehiclePhysicsRevModifier& native, const asset_data::content::vehicle_physics_rev_modifier& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.skidSndContrib = data.skid_snd_contrib;
			native.noiseAmpl = data.noise_ampl;
			native.sinewaveFreq = data.sinewave_freq;
			native.sinewaveAmpl = data.sinewave_ampl;
			native.rpmMax = data.rpm_max;
			native.rpmScale = data.rpm_scale;
			native.rpmSpeedEmaSamples = data.rpm_speed_ema_samples;
			native.inclinedThreshold = data.inclined_threshold;
			native.gearCount = data.gear_count;
			native.inclContrib = data.incl_contrib;
			native.maxSpinSpeedRPMFw = data.max_spin_speed_rpm_fw;
			native.maxSpinSpeedRPMBw = data.max_spin_speed_rpm_bw;
		}

		std::size_t size_vehicle_physics_rev_modifier([[maybe_unused]] const asset_data::content::vehicle_physics_rev_modifier& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::vehicle read_vehicle(const VehicleDef& native)
		{
			plain::trace("read_vehicle");
			asset_data::content::vehicle data;
			data.name = plain::read_string(native.name);
			data.type = plain::enum_name(asset_data::content::vehicle_type_names, static_cast<std::int64_t>(native.type), "VehicleDef.type");
			data.use_hint_string = plain::read_string(native.useHintString);
			data.health = native.health;
			data.input_binding_set = native.inputBindingSet;
			data.hit_client_scriptables = native.hitClientScriptables;
			data.hit_remote_controllers = native.hitRemoteControllers;
			data.accurate_shape_overlap = native.accurateShapeOverlap;
			data.num_doors = native.numDoors;
			data.max_door_angle = native.maxDoorAngle;
			data.door_bones = [&] { std::array<std::optional<std::string>, 4> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = plain::read_script_string(native.doorBones[i]); } return out; }();
			data.camera_def = plain::read_asset(ASSET_TYPE_CAMERA, native.cameraDef);
			data.kill_cam_def = plain::read_asset(ASSET_TYPE_CAMERA, native.killCamDef);
			data.tex_scroll_scale = native.texScrollScale;
			data.top_speed = native.topSpeed;
			data.accel = native.accel;
			data.rot_rate = native.rotRate;
			data.rot_accel = native.rotAccel;
			data.max_body_pitch = native.maxBodyPitch;
			data.max_body_roll = native.maxBodyRoll;
			data.range_for_bone_controllers = native.rangeForBoneControllers;
			data.fake_body_accel_pitch = native.fakeBodyAccelPitch;
			data.fake_body_accel_roll = native.fakeBodyAccelRoll;
			data.fake_body_vel_pitch = native.fakeBodyVelPitch;
			data.fake_body_vel_roll = native.fakeBodyVelRoll;
			data.fake_body_side_vel_pitch = native.fakeBodySideVelPitch;
			data.fake_body_pitch_strength = native.fakeBodyPitchStrength;
			data.fake_body_roll_strength = native.fakeBodyRollStrength;
			data.fake_body_pitch_dampening = native.fakeBodyPitchDampening;
			data.fake_body_roll_dampening = native.fakeBodyRollDampening;
			data.fake_body_boat_rocking_amplitude = native.fakeBodyBoatRockingAmplitude;
			data.fake_body_boat_rocking_period = native.fakeBodyBoatRockingPeriod;
			data.fake_body_boat_rocking_rotation_period = native.fakeBodyBoatRockingRotationPeriod;
			data.fake_body_boat_rocking_fadeout_speed = native.fakeBodyBoatRockingFadeoutSpeed;
			data.boat_bouncing_min_force = native.boatBouncingMinForce;
			data.boat_bouncing_max_force = native.boatBouncingMaxForce;
			data.boat_bouncing_rate = native.boatBouncingRate;
			data.boat_bouncing_fadein_speed = native.boatBouncingFadeinSpeed;
			data.boat_bouncing_fadeout_steering_angle = native.boatBouncingFadeoutSteeringAngle;
			data.collision_damage = native.collisionDamage;
			data.collision_speed = native.collisionSpeed;
			data.collision_damage_ignores_impact_angle = native.collisionDamageIgnoresImpactAngle;
			data.killcam_offset = plain::read_scalars<float, 3>(&native.killcamOffset);
			data.player_protected = native.playerProtected;
			data.bullet_damage = native.bulletDamage;
			data.armor_piercing_damage = native.armorPiercingDamage;
			data.grenade_damage = native.grenadeDamage;
			data.projectile_damage = native.projectileDamage;
			data.projectile_splash_damage = native.projectileSplashDamage;
			data.heavy_explosive_damage = native.heavyExplosiveDamage;
			data.vehicle_physics_def = read_vehicle_physics_def(native.vehiclePhysicsDef);
			data.veh_helicopter_lock_altitude = native.vehHelicopterLockAltitude;
			data.veh_helicopter_offset_from_mesh = native.vehHelicopterOffsetFromMesh;
			data.veh_helicopter_altitude_offset = native.vehHelicopterAltitudeOffset;
			data.veh_helicopter_pitch_offset = native.vehHelicopterPitchOffset;
			data.veh_helicopter_bounds_radius = native.vehHelicopterBoundsRadius;
			data.veh_helicopter_max_speed = native.vehHelicopterMaxSpeed;
			data.veh_helicopter_max_speed_vertical = native.vehHelicopterMaxSpeedVertical;
			data.veh_helicopter_max_accel = native.vehHelicopterMaxAccel;
			data.veh_helicopter_max_accel_vertical = native.vehHelicopterMaxAccelVertical;
			data.veh_helicopter_deceleration_fwd = native.vehHelicopterDecelerationFwd;
			data.veh_helicopter_deceleration_side = native.vehHelicopterDecelerationSide;
			data.veh_helicopter_deceleration_up = native.vehHelicopterDecelerationUp;
			data.veh_helicopter_max_yaw_rate = native.vehHelicopterMaxYawRate;
			data.veh_helicopter_max_yaw_accel = native.vehHelicopterMaxYawAccel;
			data.veh_helicopter_tilt_from_velocity = native.vehHelicopterTiltFromVelocity;
			data.veh_helicopter_tilt_from_controller_axes = native.vehHelicopterTiltFromControllerAxes;
			data.veh_helicopter_tilt_from_acceleration = native.vehHelicopterTiltFromAcceleration;
			data.veh_helicopter_tilt_from_deceleration = native.vehHelicopterTiltFromDeceleration;
			data.veh_helicopter_tilt_from_fwd_and_yaw_vel_at_max_tilt = native.vehHelicopterTiltFromFwdAndYaw_VelAtMaxTilt;
			data.veh_helicopter_tilt_from_fwd_and_yaw = native.vehHelicopterTiltFromFwdAndYaw;
			data.veh_helicopter_tilt_momentum = native.vehHelicopterTiltMomentum;
			data.veh_helicopter_tilt_speed = native.vehHelicopterTiltSpeed;
			data.veh_helicopter_max_pitch = native.vehHelicopterMaxPitch;
			data.veh_helicopter_max_roll = native.vehHelicopterMaxRoll;
			data.veh_helicopter_hover_speed_threshold = native.vehHelicopterHoverSpeedThreshold;
			data.veh_helicopter_jitter_jerkyness = native.vehHelicopterJitterJerkyness;
			data.veh_helicopter_lookahead_time = native.vehHelicopterLookaheadTime;
			data.veh_helicopter_soft_collisions = native.vehHelicopterSoftCollisions;
			data.veh_helicopter_use_ground_fx = native.vehHelicopterUseGroundFX;
			data.veh_helicopter_ground_fx = read_fx_combined_def(native.vehHelicopterGroundFx);
			data.veh_helicopter_ground_water_fx = read_fx_combined_def(native.vehHelicopterGroundWaterFx);
			data.veh_helicopter_ground_fx_default_repeat_rate = native.vehHelicopterGroundFxDefaultRepeatRate;
			data.veh_helicopter_ground_fx_slowest_repeat_rate = native.vehHelicopterGroundFxSlowestRepeatRate;
			data.veh_helicopter_ground_fx_fastest_repeat_rate = native.vehHelicopterGroundFxFastestRepeatRate;
			data.veh_helicopter_ground_fx_min_ground_dist = native.vehHelicopterGroundFxMinGroundDist;
			data.veh_helicopter_ground_fx_max_ground_dist = native.vehHelicopterGroundFxMaxGroundDist;
			data.veh_rotor_max_velocity = native.vehRotorMaxVelocity;
			data.veh_rotor_max_accel = native.vehRotorMaxAccel;
			data.veh_rotor_accel_tilt_blend = native.vehRotorAccelTiltBlend;
			data.veh_rotor_max_vehicle_spin = native.vehRotorMaxVehicleSpin;
			data.veh_rotor_max_angle = native.vehRotorMaxAngle;
			data.veh_rotor_spin_speed = native.vehRotorSpinSpeed;
			data.veh_rotor_spin_vertical_speed_threshold = native.vehRotorSpinVerticalSpeedThreshold;
			data.veh_rotor_max_spin_addition = native.vehRotorMaxSpinAddition;
			data.ss_weapon_switch_delay_seconds = native.ssWeaponSwitchDelaySeconds;
			data.ss_weapon_raise_delay_seconds = native.ssWeaponRaiseDelaySeconds;
			data.ss_fly_max_speed_mph = plain::read_scalars<float, 3>(&native.ssFlyMaxSpeedMph);
			data.ss_fly_max_acceleration_mphps = plain::read_scalars<float, 3>(&native.ssFlyMaxAccelerationMphps);
			data.ss_fly_max_counter_acceleration_mphps = plain::read_scalars<float, 3>(&native.ssFlyMaxCounterAccelerationMphps);
			data.ss_fly_friction_mphps = plain::read_scalars<float, 3>(&native.ssFlyFrictionMphps);
			data.ss_fly_juke_speed_mph = plain::read_scalars<float, 3>(&native.ssFlyJukeSpeedMph);
			data.ss_hover_max_speed_mph = plain::read_scalars<float, 3>(&native.ssHoverMaxSpeedMph);
			data.ss_hover_max_acceleration_mphps = plain::read_scalars<float, 3>(&native.ssHoverMaxAccelerationMphps);
			data.ss_hover_max_counter_acceleration_mphps = plain::read_scalars<float, 3>(&native.ssHoverMaxCounterAccelerationMphps);
			data.ss_hover_friction_mphps = plain::read_scalars<float, 3>(&native.ssHoverFrictionMphps);
			data.ss_hover_juke_speed_mph = plain::read_scalars<float, 3>(&native.ssHoverJukeSpeedMph);
			data.ss_fly_juke_ramp_time = native.ssFlyJukeRampTime;
			data.ss_hover_juke_ramp_time = native.ssHoverJukeRampTime;
			data.ss_braking_friction_mphps = native.ssBrakingFrictionMphps;
			data.ss_fly_redirect_velocity_scale = native.ssFlyRedirectVelocityScale;
			data.ss_fly_slow_redirect_velocity_scale = native.ssFlySlowRedirectVelocityScale;
			data.ss_hover_redirect_velocity_scale = native.ssHoverRedirectVelocityScale;
			data.ss_fly_min_speed_ratio = native.ssFlyMinSpeedRatio;
			data.ss_project_velocity_desires_to_xy = native.ssProjectVelocityDesiresToXY;
			data.ss_enforce_min_ground_clearance = native.ssEnforceMinGroundClearance;
			data.ss_min_ground_clearance_height = native.ssMinGroundClearanceHeight;
			data.ss_min_ground_clearance_lift_force = native.ssMinGroundClearanceLiftForce;
			data.ss_terrain_mode_movement = native.ssTerrainModeMovement;
			data.ss_terrain_mode_gravity = native.ssTerrainModeGravity;
			data.ss_terrain_mode_fly_time = native.ssTerrainModeFlyTime;
			data.ss_aim_offset_clamp_deg = plain::read_scalars<float, 2>(&native.ssAimOffsetClampDeg);
			data.ss_pitch_up_soft_limit_deg = plain::read_scalars<float, 2>(&native.ssPitchUpSoftLimitDeg);
			data.ss_pitch_down_soft_limit_deg = plain::read_scalars<float, 2>(&native.ssPitchDownSoftLimitDeg);
			data.ss_clutch_pitch_speed_scale = native.ssClutchPitchSpeedScale;
			data.ss_fly_aim_speed_dps = plain::read_scalars<float, 2>(&native.ssFlyAimSpeedDps);
			data.ss_fly_slow_aim_speed_dps = plain::read_scalars<float, 2>(&native.ssFlySlowAimSpeedDps);
			data.ss_hover_aim_speed_dps = plain::read_scalars<float, 2>(&native.ssHoverAimSpeedDps);
			data.ss_fly_aim_drift_scale = plain::read_scalars<float, 2>(&native.ssFlyAimDriftScale);
			data.ss_fly_slow_aim_drift_scale = plain::read_scalars<float, 2>(&native.ssFlySlowAimDriftScale);
			data.ss_hover_aim_drift_scale = plain::read_scalars<float, 2>(&native.ssHoverAimDriftScale);
			data.ss_min_spring_oscillation = native.ssMinSpringOscillation;
			data.ss_max_spring_oscillation = native.ssMaxSpringOscillation;
			data.ss_min_spring_strength = native.ssMinSpringStrength;
			data.ss_max_spring_strength = native.ssMaxSpringStrength;
			data.ss_min_spring_ang_speed = native.ssMinSpringAngSpeed;
			data.ss_max_spring_ang_speed = native.ssMaxSpringAngSpeed;
			data.ss_spring_controller_contribution = native.ssSpringControllerContribution;
			data.ss_tilt_spring_oscillation = native.ssTiltSpringOscillation;
			data.ss_tilt_spring_strength = native.ssTiltSpringStrength;
			data.ss_tilt_spring_angle_contribution = native.ssTiltSpringAngleContribution;
			data.ss_ads_to_lock_on = native.ssADSToLockOn;
			data.ss_lock_on_min_angle = native.ssLockOnMinAngle;
			data.ss_lock_on_max_angle = native.ssLockOnMaxAngle;
			data.ss_pilot_assist_no_lock_aim_scale = native.ssPilotAssistNoLockAimScale;
			data.ss_pilot_assist_no_lock_min_angle = native.ssPilotAssistNoLockMinAngle;
			data.ss_pilot_assist_no_lock_max_angle = native.ssPilotAssistNoLockMaxAngle;
			data.ss_pilot_assist_aim_scale = native.ssPilotAssistAimScale;
			data.ss_pilot_assist_min_angle = native.ssPilotAssistMinAngle;
			data.ss_pilot_assist_max_angle = native.ssPilotAssistMaxAngle;
			data.ss_auto_level_strength_hover = plain::read_scalars<float, 2>(&native.ssAutoLevelStrengthHover);
			data.ss_auto_level_strength_fly = plain::read_scalars<float, 2>(&native.ssAutoLevelStrengthFly);
			data.ss_is_roll_right_stick = native.ssIsRollRightStick;
			data.ss_roll_max_speed_dps = native.ssRollMaxSpeedDps;
			data.ss_boost_speed_scale = native.ssBoostSpeedScale;
			data.ss_boost_thrust_scale = native.ssBoostThrustScale;
			data.ss_enable_boost_to_fly = native.ssEnableBoostToFly;
			data.ss_boost_time = native.ssBoostTime;
			data.ss_boost_regen_time = native.ssBoostRegenTime;
			data.ss_boost_regen_stall_time = native.ssBoostRegenStallTime;
			data.ss_boost_min_threshold_ratio = native.ssBoostMinThresholdRatio;
			data.ss_collision_radius_i = native.ssCollisionRadiusI;
			data.ss_cylinder_height_i = native.ssCylinderHeightI;
			data.ss_use_cylinder = native.ssUseCylinder;
			data.ss_use_player_clip = native.ssUsePlayerClip;
			data.ss_collision_offset_i = native.ssCollisionOffsetI;
			data.ss_collision_hard_threshold_mph = native.ssCollisionHardThresholdMph;
			data.ss_fov_fly_offset_deg = native.ssFOVFlyOffsetDeg;
			data.ss_fov_fly_boost_offset_deg = native.ssFOVFlyBoostOffsetDeg;
			data.ss_fov_hover_offset_deg = native.ssFOVHoverOffsetDeg;
			data.ss_zoom_blend_speed_dps = native.ssZoomBlendSpeedDps;
			data.ss_zoom_max_angle = native.ssZoomMaxAngle;
			data.ss_zoom_min_dist_i = native.ssZoomMinDistI;
			data.ss_zoom_ideal_dist_i = native.ssZoomIdealDistI;
			data.ss_zoom_max_dist_i = native.ssZoomMaxDistI;
			data.ss_show_pilot_viewmodel = native.ssShowPilotViewmodel;
			data.ss_camera_anim_scale = native.ssCameraAnimScale;
			data.ss_fly_tilt_ship_angles = plain::read_scalars<float, 2>(&native.ssFlyTiltShipAngles);
			data.ss_fly_tilt_camera_scale = native.ssFlyTiltCameraScale;
			data.ss_hover_tilt_ship_angles = plain::read_scalars<float, 2>(&native.ssHoverTiltShipAngles);
			data.ss_hover_tilt_camera_scale = native.ssHoverTiltCameraScale;
			data.ss_motion_idle_rate_min_speed = native.ssMotionIdleRateMinSpeed;
			data.ss_motion_idle_rate_max_speed = native.ssMotionIdleRateMaxSpeed;
			data.ss_motion_idle_min_rate = native.ssMotionIdleMinRate;
			data.ss_motion_idle_max_rate = native.ssMotionIdleMaxRate;
			data.ss_anim_tree = plain::read_script_string(native.ssAnimTree);
			data.ss_anim_parts = [&] { std::array<asset_data::content::spaceship_anim_parts, 33> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_spaceship_anim_parts(native.ssAnimParts[i]); } return out; }();
			data.ss_thrust_fx_loop = read_fx_combined_def(native.ssThrustFxLoop);
			data.ss_juke_fx = read_fx_combined_def(native.ssJukeFx);
			data.ss_idle_rumble = plain::read_asset(ASSET_TYPE_RUMBLE, native.ssIdleRumble);
			data.ss_small_rumble = plain::read_asset(ASSET_TYPE_RUMBLE, native.ssSmallRumble);
			data.ss_med_rumble = plain::read_asset(ASSET_TYPE_RUMBLE, native.ssMedRumble);
			data.ss_large_rumble = plain::read_asset(ASSET_TYPE_RUMBLE, native.ssLargeRumble);
			data.rattle_loop = read_snd_alias_lookup(native.rattleLoop);
			data.rattle_loop_min_volume = native.rattleLoopMinVolume;
			data.rattle_loop_max_volume = native.rattleLoopMaxVolume;
			data.air_loop = read_snd_alias_lookup(native.airLoop);
			data.air_loop_min_volume = native.airLoopMinVolume;
			data.air_loop_max_volume = native.airLoopMaxVolume;
			data.engine_loop = read_snd_alias_lookup(native.engineLoop);
			data.engine_loop_min_volume = native.engineLoopMinVolume;
			data.engine_loop_max_volume = native.engineLoopMaxVolume;
			data.engine_loop_min_pitch = native.engineLoopMinPitch;
			data.engine_loop_max_pitch = native.engineLoopMaxPitch;
			data.hover_loop = read_snd_alias_lookup(native.hoverLoop);
			data.hover_loop_min_volume = native.hoverLoopMinVolume;
			data.hover_loop_max_volume = native.hoverLoopMaxVolume;
			data.hover_loop_min_pitch = native.hoverLoopMinPitch;
			data.hover_loop_max_pitch = native.hoverLoopMaxPitch;
			data.boost_loop = read_snd_alias_lookup(native.boostLoop);
			data.boost_loop_max_volume = native.boostLoopMaxVolume;
			data.boost_loop_max_pitch = native.boostLoopMaxPitch;
			data.ss_thrust_loop = read_snd_alias_lookup(native.ssThrustLoop);
			data.ss_thrust_loop_max_volume = native.ssThrustLoopMaxVolume;
			data.ss_thrust_loop_max_pitch = native.ssThrustLoopMaxPitch;
			data.boost_start = read_snd_alias_lookup(native.boostStart);
			data.boost_stop = read_snd_alias_lookup(native.boostStop);
			data.boost_depleted = read_snd_alias_lookup(native.boostDepleted);
			data.boost_unavailable = read_snd_alias_lookup(native.boostUnavailable);
			data.juke_left = read_snd_alias_lookup(native.jukeLeft);
			data.juke_right = read_snd_alias_lookup(native.jukeRight);
			data.juke_up_down = read_snd_alias_lookup(native.jukeUpDown);
			data.juke_back = read_snd_alias_lookup(native.jukeBack);
			data.juke_front = read_snd_alias_lookup(native.jukeFront);
			data.flight_on = read_snd_alias_lookup(native.flightOn);
			data.flight_off = read_snd_alias_lookup(native.flightOff);
			data.flight_on_timer = native.flightOnTimer;
			data.flight_off_timer = native.flightOffTimer;
			data.hard_collision = read_snd_alias_lookup(native.hardCollision);
			data.soft_collision = read_snd_alias_lookup(native.softCollision);
			data.cam_look_enabled = native.camLookEnabled;
			data.cam_relative_control = native.camRelativeControl;
			data.cam_remote_drive = native.camRemoteDrive;
			data.cam_lerp = native.camLerp;
			data.cam_height = native.camHeight;
			data.cam_radius = native.camRadius;
			data.cam_pitch_influence = native.camPitchInfluence;
			data.cam_yaw_influence = native.camYawInfluence;
			data.cam_roll_influence = native.camRollInfluence;
			data.cam_fov_increase = native.camFovIncrease;
			data.cam_fov_offset = native.camFovOffset;
			data.cam_fov_speed = native.camFovSpeed;
			data.cam_return_speed = native.camReturnSpeed;
			data.cam_return_lerp = native.camReturnLerp;
			data.cam_vehicle_angle_pitch_rate = native.camVehicleAnglePitchRate;
			data.cam_vehicle_angle_yaw_rate = native.camVehicleAngleYawRate;
			data.cam_vehicle_angle_roll_rate = native.camVehicleAngleRollRate;
			data.veh_cam_use_gdt = native.vehCam_UseGDT;
			data.veh_cam_angles_pitch = native.vehCam_anglesPitch;
			data.veh_cam_angles_yaw = native.vehCam_anglesYaw;
			data.veh_cam_angles_roll = native.vehCam_anglesRoll;
			data.veh_cam_offset_x = native.vehCam_offsetX;
			data.veh_cam_offset_y = native.vehCam_offsetY;
			data.veh_cam_offset_z = native.vehCam_offsetZ;
			data.veh_cam_radius = native.vehCam_radius;
			data.veh_cam_speed_influence = native.vehCam_speedInfluence;
			data.veh_cam_pitch_turn_rate = native.vehCam_pitchTurnRate;
			data.veh_cam_pitch_clamp = native.vehCam_pitchClamp;
			data.veh_cam_yaw_turn_rate = native.vehCam_yawTurnRate;
			data.veh_cam_yaw_clamp = native.vehCam_yawClamp;
			data.veh_cam_z_offset_mode = plain::enum_name(asset_data::content::veh_cam_z_offset_mode_names, static_cast<std::int64_t>(native.vehCam_zOffsetMode), "VehicleDef.vehCam_zOffsetMode");
			data.veh_cam_angles_pitch3_p = native.vehCam_anglesPitch3P;
			data.veh_cam_angles_yaw3_p = native.vehCam_anglesYaw3P;
			data.veh_cam_angles_roll3_p = native.vehCam_anglesRoll3P;
			data.veh_cam_offset_x3_p = native.vehCam_offsetX3P;
			data.veh_cam_offset_y3_p = native.vehCam_offsetY3P;
			data.veh_cam_offset_z3_p = native.vehCam_offsetZ3P;
			data.veh_cam_radius3_p = native.vehCam_radius3P;
			data.veh_cam_speed_influence3_p = native.vehCam_speedInfluence3P;
			data.veh_cam_pitch_turn_rate3_p = native.vehCam_pitchTurnRate3P;
			data.veh_cam_pitch_clamp3_p = native.vehCam_pitchClamp3P;
			data.veh_cam_yaw_turn_rate3_p = native.vehCam_yawTurnRate3P;
			data.veh_cam_yaw_clamp3_p = native.vehCam_yawClamp3P;
			data.veh_cam_z_offset_mode3_p = plain::enum_name(asset_data::content::veh_cam_z_offset_mode_names, static_cast<std::int64_t>(native.vehCam_zOffsetMode3P), "VehicleDef.vehCam_zOffsetMode3P");
			data.turret_weapon_name = plain::read_string(native.turretWeaponName);
			data.turret_weapon = plain::read_asset(ASSET_TYPE_WEAPON, native.turretWeapon);
			data.turret_horiz_span_left = native.turretHorizSpanLeft;
			data.turret_horiz_span_right = native.turretHorizSpanRight;
			data.turret_vert_span_up = native.turretVertSpanUp;
			data.turret_vert_span_down = native.turretVertSpanDown;
			data.turret_horiz_resist_left = native.turretHorizResistLeft;
			data.turret_horiz_resist_right = native.turretHorizResistRight;
			data.turret_vert_resist_up = native.turretVertResistUp;
			data.turret_vert_resist_down = native.turretVertResistDown;
			data.turret_rot_rate = native.turretRotRate;
			data.turret_fire_type = plain::enum_name(asset_data::content::vehicle_turret_fire_type_names, static_cast<std::int64_t>(native.turretFireType), "VehicleDef.turretFireType");
			data.turret_spin_snd = read_snd_alias_lookup(native.turretSpinSnd);
			data.turret_stop_snd = read_snd_alias_lookup(native.turretStopSnd);
			data.trophy_enabled = native.trophyEnabled;
			data.trophy_radius = native.trophyRadius;
			data.trophy_inactive_radius = native.trophyInactiveRadius;
			data.trophy_ammo_count = native.trophyAmmoCount;
			data.trophy_reload_time = native.trophyReloadTime;
			data.trophy_tags = [&] { std::array<std::optional<std::string>, 4> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = plain::read_script_string(native.trophyTags[i]); } return out; }();
			data.trophy_explode_fx = read_fx_combined_def(native.trophyExplodeFx);
			data.trophy_flash_fx = read_fx_combined_def(native.trophyFlashFx);
			data.compass_friendly_icon = plain::read_asset(ASSET_TYPE_MATERIAL, native.compassFriendlyIcon);
			data.compass_enemy_icon = plain::read_asset(ASSET_TYPE_MATERIAL, native.compassEnemyIcon);
			data.compass_friendly_alt_icon = plain::read_asset(ASSET_TYPE_MATERIAL, native.compassFriendlyAltIcon);
			data.compass_enemy_alt_icon = plain::read_asset(ASSET_TYPE_MATERIAL, native.compassEnemyAltIcon);
			data.compass_icon_width = native.compassIconWidth;
			data.compass_icon_height = native.compassIconHeight;
			data.detail_sound_bank_npc = plain::read_asset(ASSET_TYPE_SOUND_BANK_TRANSIENT, native.detailSoundBankNPC);
			data.detail_sound_bank_player = plain::read_asset(ASSET_TYPE_SOUND_BANK_TRANSIENT, native.detailSoundBankPlayer);
			data.idle_low_snd = read_snd_alias_lookup(native.idleLowSnd);
			data.idle_high_snd = read_snd_alias_lookup(native.idleHighSnd);
			data.engine_low_snd = read_snd_alias_lookup(native.engineLowSnd);
			data.engine_high_snd = read_snd_alias_lookup(native.engineHighSnd);
			data.fallback_idle_low_snd = read_snd_alias_lookup(native.fallbackIdleLowSnd);
			data.fallback_idle_high_snd = read_snd_alias_lookup(native.fallbackIdleHighSnd);
			data.fallback_engine_low_snd = read_snd_alias_lookup(native.fallbackEngineLowSnd);
			data.fallback_engine_high_snd = read_snd_alias_lookup(native.fallbackEngineHighSnd);
			data.engine_snd_speed = native.engineSndSpeed;
			data.audio_origin_tag = plain::read_script_string(native.audioOriginTag);
			data.main_rotor_low_rpm_snd = read_snd_alias_lookup(native.mainRotorLowRpmSnd);
			data.main_rotor_operating_rpm_snd = read_snd_alias_lookup(native.mainRotorOperatingRpmSnd);
			data.main_rotor_operating_effort_snd = read_snd_alias_lookup(native.mainRotorOperatingEffortSnd);
			data.tail_rotor_low_rpm_snd = read_snd_alias_lookup(native.tailRotorLowRpmSnd);
			data.tail_rotor_operating_rpm_snd = read_snd_alias_lookup(native.tailRotorOperatingRpmSnd);
			data.tail_rotor_operating_effort_snd_left = read_snd_alias_lookup(native.tailRotorOperatingEffortSndLeft);
			data.tail_rotor_operating_effort_snd_right = read_snd_alias_lookup(native.tailRotorOperatingEffortSndRight);
			data.physics_heli_startup = read_snd_alias_lookup(native.physicsHeliStartup);
			data.physics_heli_shutdown = read_snd_alias_lookup(native.physicsHeliShutdown);
			data.turbine_low_rpm_snd = read_snd_alias_lookup(native.turbineLowRpmSnd);
			data.turbine_operating_rpm_snd = read_snd_alias_lookup(native.turbineOperatingRpmSnd);
			data.use_rev_audio_settings = native.useRevAudioSettings;
			data.rev_low_fidelity_mod = read_snd_alias_lookup(native.revLowFidelityMod);
			data.rev_high_fidelity_mod = read_snd_alias_lookup(native.revHighFidelityMod);
			data.rev_player_priority = native.revPlayerPriority;
			data.rev_modifier = read_vehicle_physics_rev_modifier(native.revModifier);
			data.engine_start_up_snd = read_snd_alias_lookup(native.engineStartUpSnd);
			data.engine_start_up_length = native.engineStartUpLength;
			data.engine_shutdown_snd = read_snd_alias_lookup(native.engineShutdownSnd);
			data.engine_idle_snd = read_snd_alias_lookup(native.engineIdleSnd);
			data.engine_sustain_snd = read_snd_alias_lookup(native.engineSustainSnd);
			data.engine_ramp_up_snd = read_snd_alias_lookup(native.engineRampUpSnd);
			data.engine_ramp_up_length = native.engineRampUpLength;
			data.engine_ramp_down_snd = read_snd_alias_lookup(native.engineRampDownSnd);
			data.engine_ramp_down_length = native.engineRampDownLength;
			data.suspension_soft_snd = read_snd_alias_lookup(native.suspensionSoftSnd);
			data.suspension_soft_compression = native.suspensionSoftCompression;
			data.suspension_hard_snd = read_snd_alias_lookup(native.suspensionHardSnd);
			data.suspension_hard_compression = native.suspensionHardCompression;
			data.collision_snd = read_snd_alias_lookup(native.collisionSnd);
			data.collision_blend_speed = native.collisionBlendSpeed;
			data.speed_snd = read_snd_alias_lookup(native.speedSnd);
			data.speed_snd_blend_speed = native.speedSndBlendSpeed;
			data.surface_snd_name = plain::read_string(native.surfaceSndName);
			data.surface_snd_blend_speed = native.surfaceSndBlendSpeed;
			data.slide_volume = native.slideVolume;
			data.slide_blend_speed = native.slideBlendSpeed;
			data.in_air_pitch = native.inAirPitch;
			data.sound_trigger_override_zone = plain::read_string(native.soundTriggerOverrideZone);
			data.sound_trigger_override_reverb = native.soundTriggerOverrideReverb;
			data.sound_trigger_override_mix = native.soundTriggerOverrideMix;
			data.sound_trigger_override_filter = native.soundTriggerOverrideFilter;
			data.sound_trigger_override_occlusion = native.soundTriggerOverrideOcclusion;
			data.sound_trigger_override_ambient = native.soundTriggerOverrideAmbient;
			data.sound_trigger_override_ambient_events = native.soundTriggerOverrideAmbientEvents;
			data.sound_trigger_override_adsr = native.soundTriggerOverrideADSR;
			data.vision_blend_time = native.visionBlendTime;
			data.global_vision_settings = plain::read_string(native.globalVisionSettings);
			data.map_vision_settings = plain::read_string(native.mapVisionSettings);
			data.lui_crosshair_widget = plain::read_string(native.luiCrosshairWidget);
			data.dlc_float = std::to_array(native.dlcFloat);
			return data;
		}

		void write_vehicle(VehicleDef& native, const asset_data::content::vehicle& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.type = static_cast<VehicleType>(plain::enum_value(asset_data::content::vehicle_type_names, data.type, "VehicleDef.type"));
			native.useHintString = plain::write_string(memory, data.use_hint_string);
			native.health = data.health;
			native.inputBindingSet = data.input_binding_set;
			native.hitClientScriptables = data.hit_client_scriptables;
			native.hitRemoteControllers = data.hit_remote_controllers;
			native.accurateShapeOverlap = data.accurate_shape_overlap;
			native.numDoors = data.num_doors;
			native.maxDoorAngle = data.max_door_angle;
			for (std::size_t i = 0; i < data.door_bones.size(); ++i)
			{
				native.doorBones[i] = static_cast<scr_string_t>(plain::write_script_string(data.door_bones[i]));
			}
			native.cameraDef = plain::resolve_asset<CameraDef>(resolve, ASSET_TYPE_CAMERA, data.camera_def);
			native.killCamDef = plain::resolve_asset<CameraDef>(resolve, ASSET_TYPE_CAMERA, data.kill_cam_def);
			native.texScrollScale = data.tex_scroll_scale;
			native.topSpeed = data.top_speed;
			native.accel = data.accel;
			native.rotRate = data.rot_rate;
			native.rotAccel = data.rot_accel;
			native.maxBodyPitch = data.max_body_pitch;
			native.maxBodyRoll = data.max_body_roll;
			native.rangeForBoneControllers = data.range_for_bone_controllers;
			native.fakeBodyAccelPitch = data.fake_body_accel_pitch;
			native.fakeBodyAccelRoll = data.fake_body_accel_roll;
			native.fakeBodyVelPitch = data.fake_body_vel_pitch;
			native.fakeBodyVelRoll = data.fake_body_vel_roll;
			native.fakeBodySideVelPitch = data.fake_body_side_vel_pitch;
			native.fakeBodyPitchStrength = data.fake_body_pitch_strength;
			native.fakeBodyRollStrength = data.fake_body_roll_strength;
			native.fakeBodyPitchDampening = data.fake_body_pitch_dampening;
			native.fakeBodyRollDampening = data.fake_body_roll_dampening;
			native.fakeBodyBoatRockingAmplitude = data.fake_body_boat_rocking_amplitude;
			native.fakeBodyBoatRockingPeriod = data.fake_body_boat_rocking_period;
			native.fakeBodyBoatRockingRotationPeriod = data.fake_body_boat_rocking_rotation_period;
			native.fakeBodyBoatRockingFadeoutSpeed = data.fake_body_boat_rocking_fadeout_speed;
			native.boatBouncingMinForce = data.boat_bouncing_min_force;
			native.boatBouncingMaxForce = data.boat_bouncing_max_force;
			native.boatBouncingRate = data.boat_bouncing_rate;
			native.boatBouncingFadeinSpeed = data.boat_bouncing_fadein_speed;
			native.boatBouncingFadeoutSteeringAngle = data.boat_bouncing_fadeout_steering_angle;
			native.collisionDamage = data.collision_damage;
			native.collisionSpeed = data.collision_speed;
			native.collisionDamageIgnoresImpactAngle = data.collision_damage_ignores_impact_angle;
			plain::write_scalars(&native.killcamOffset, data.killcam_offset);
			native.playerProtected = data.player_protected;
			native.bulletDamage = data.bullet_damage;
			native.armorPiercingDamage = data.armor_piercing_damage;
			native.grenadeDamage = data.grenade_damage;
			native.projectileDamage = data.projectile_damage;
			native.projectileSplashDamage = data.projectile_splash_damage;
			native.heavyExplosiveDamage = data.heavy_explosive_damage;
			write_vehicle_physics_def(native.vehiclePhysicsDef, data.vehicle_physics_def, resolve, memory);
			native.vehHelicopterLockAltitude = data.veh_helicopter_lock_altitude;
			native.vehHelicopterOffsetFromMesh = data.veh_helicopter_offset_from_mesh;
			native.vehHelicopterAltitudeOffset = data.veh_helicopter_altitude_offset;
			native.vehHelicopterPitchOffset = data.veh_helicopter_pitch_offset;
			native.vehHelicopterBoundsRadius = data.veh_helicopter_bounds_radius;
			native.vehHelicopterMaxSpeed = data.veh_helicopter_max_speed;
			native.vehHelicopterMaxSpeedVertical = data.veh_helicopter_max_speed_vertical;
			native.vehHelicopterMaxAccel = data.veh_helicopter_max_accel;
			native.vehHelicopterMaxAccelVertical = data.veh_helicopter_max_accel_vertical;
			native.vehHelicopterDecelerationFwd = data.veh_helicopter_deceleration_fwd;
			native.vehHelicopterDecelerationSide = data.veh_helicopter_deceleration_side;
			native.vehHelicopterDecelerationUp = data.veh_helicopter_deceleration_up;
			native.vehHelicopterMaxYawRate = data.veh_helicopter_max_yaw_rate;
			native.vehHelicopterMaxYawAccel = data.veh_helicopter_max_yaw_accel;
			native.vehHelicopterTiltFromVelocity = data.veh_helicopter_tilt_from_velocity;
			native.vehHelicopterTiltFromControllerAxes = data.veh_helicopter_tilt_from_controller_axes;
			native.vehHelicopterTiltFromAcceleration = data.veh_helicopter_tilt_from_acceleration;
			native.vehHelicopterTiltFromDeceleration = data.veh_helicopter_tilt_from_deceleration;
			native.vehHelicopterTiltFromFwdAndYaw_VelAtMaxTilt = data.veh_helicopter_tilt_from_fwd_and_yaw_vel_at_max_tilt;
			native.vehHelicopterTiltFromFwdAndYaw = data.veh_helicopter_tilt_from_fwd_and_yaw;
			native.vehHelicopterTiltMomentum = data.veh_helicopter_tilt_momentum;
			native.vehHelicopterTiltSpeed = data.veh_helicopter_tilt_speed;
			native.vehHelicopterMaxPitch = data.veh_helicopter_max_pitch;
			native.vehHelicopterMaxRoll = data.veh_helicopter_max_roll;
			native.vehHelicopterHoverSpeedThreshold = data.veh_helicopter_hover_speed_threshold;
			native.vehHelicopterJitterJerkyness = data.veh_helicopter_jitter_jerkyness;
			native.vehHelicopterLookaheadTime = data.veh_helicopter_lookahead_time;
			native.vehHelicopterSoftCollisions = data.veh_helicopter_soft_collisions;
			native.vehHelicopterUseGroundFX = data.veh_helicopter_use_ground_fx;
			write_fx_combined_def(native.vehHelicopterGroundFx, data.veh_helicopter_ground_fx, resolve, memory);
			write_fx_combined_def(native.vehHelicopterGroundWaterFx, data.veh_helicopter_ground_water_fx, resolve, memory);
			native.vehHelicopterGroundFxDefaultRepeatRate = data.veh_helicopter_ground_fx_default_repeat_rate;
			native.vehHelicopterGroundFxSlowestRepeatRate = data.veh_helicopter_ground_fx_slowest_repeat_rate;
			native.vehHelicopterGroundFxFastestRepeatRate = data.veh_helicopter_ground_fx_fastest_repeat_rate;
			native.vehHelicopterGroundFxMinGroundDist = data.veh_helicopter_ground_fx_min_ground_dist;
			native.vehHelicopterGroundFxMaxGroundDist = data.veh_helicopter_ground_fx_max_ground_dist;
			native.vehRotorMaxVelocity = data.veh_rotor_max_velocity;
			native.vehRotorMaxAccel = data.veh_rotor_max_accel;
			native.vehRotorAccelTiltBlend = data.veh_rotor_accel_tilt_blend;
			native.vehRotorMaxVehicleSpin = data.veh_rotor_max_vehicle_spin;
			native.vehRotorMaxAngle = data.veh_rotor_max_angle;
			native.vehRotorSpinSpeed = data.veh_rotor_spin_speed;
			native.vehRotorSpinVerticalSpeedThreshold = data.veh_rotor_spin_vertical_speed_threshold;
			native.vehRotorMaxSpinAddition = data.veh_rotor_max_spin_addition;
			native.ssWeaponSwitchDelaySeconds = data.ss_weapon_switch_delay_seconds;
			native.ssWeaponRaiseDelaySeconds = data.ss_weapon_raise_delay_seconds;
			plain::write_scalars(&native.ssFlyMaxSpeedMph, data.ss_fly_max_speed_mph);
			plain::write_scalars(&native.ssFlyMaxAccelerationMphps, data.ss_fly_max_acceleration_mphps);
			plain::write_scalars(&native.ssFlyMaxCounterAccelerationMphps, data.ss_fly_max_counter_acceleration_mphps);
			plain::write_scalars(&native.ssFlyFrictionMphps, data.ss_fly_friction_mphps);
			plain::write_scalars(&native.ssFlyJukeSpeedMph, data.ss_fly_juke_speed_mph);
			plain::write_scalars(&native.ssHoverMaxSpeedMph, data.ss_hover_max_speed_mph);
			plain::write_scalars(&native.ssHoverMaxAccelerationMphps, data.ss_hover_max_acceleration_mphps);
			plain::write_scalars(&native.ssHoverMaxCounterAccelerationMphps, data.ss_hover_max_counter_acceleration_mphps);
			plain::write_scalars(&native.ssHoverFrictionMphps, data.ss_hover_friction_mphps);
			plain::write_scalars(&native.ssHoverJukeSpeedMph, data.ss_hover_juke_speed_mph);
			native.ssFlyJukeRampTime = data.ss_fly_juke_ramp_time;
			native.ssHoverJukeRampTime = data.ss_hover_juke_ramp_time;
			native.ssBrakingFrictionMphps = data.ss_braking_friction_mphps;
			native.ssFlyRedirectVelocityScale = data.ss_fly_redirect_velocity_scale;
			native.ssFlySlowRedirectVelocityScale = data.ss_fly_slow_redirect_velocity_scale;
			native.ssHoverRedirectVelocityScale = data.ss_hover_redirect_velocity_scale;
			native.ssFlyMinSpeedRatio = data.ss_fly_min_speed_ratio;
			native.ssProjectVelocityDesiresToXY = data.ss_project_velocity_desires_to_xy;
			native.ssEnforceMinGroundClearance = data.ss_enforce_min_ground_clearance;
			native.ssMinGroundClearanceHeight = data.ss_min_ground_clearance_height;
			native.ssMinGroundClearanceLiftForce = data.ss_min_ground_clearance_lift_force;
			native.ssTerrainModeMovement = data.ss_terrain_mode_movement;
			native.ssTerrainModeGravity = data.ss_terrain_mode_gravity;
			native.ssTerrainModeFlyTime = data.ss_terrain_mode_fly_time;
			plain::write_scalars(&native.ssAimOffsetClampDeg, data.ss_aim_offset_clamp_deg);
			plain::write_scalars(&native.ssPitchUpSoftLimitDeg, data.ss_pitch_up_soft_limit_deg);
			plain::write_scalars(&native.ssPitchDownSoftLimitDeg, data.ss_pitch_down_soft_limit_deg);
			native.ssClutchPitchSpeedScale = data.ss_clutch_pitch_speed_scale;
			plain::write_scalars(&native.ssFlyAimSpeedDps, data.ss_fly_aim_speed_dps);
			plain::write_scalars(&native.ssFlySlowAimSpeedDps, data.ss_fly_slow_aim_speed_dps);
			plain::write_scalars(&native.ssHoverAimSpeedDps, data.ss_hover_aim_speed_dps);
			plain::write_scalars(&native.ssFlyAimDriftScale, data.ss_fly_aim_drift_scale);
			plain::write_scalars(&native.ssFlySlowAimDriftScale, data.ss_fly_slow_aim_drift_scale);
			plain::write_scalars(&native.ssHoverAimDriftScale, data.ss_hover_aim_drift_scale);
			native.ssMinSpringOscillation = data.ss_min_spring_oscillation;
			native.ssMaxSpringOscillation = data.ss_max_spring_oscillation;
			native.ssMinSpringStrength = data.ss_min_spring_strength;
			native.ssMaxSpringStrength = data.ss_max_spring_strength;
			native.ssMinSpringAngSpeed = data.ss_min_spring_ang_speed;
			native.ssMaxSpringAngSpeed = data.ss_max_spring_ang_speed;
			native.ssSpringControllerContribution = data.ss_spring_controller_contribution;
			native.ssTiltSpringOscillation = data.ss_tilt_spring_oscillation;
			native.ssTiltSpringStrength = data.ss_tilt_spring_strength;
			native.ssTiltSpringAngleContribution = data.ss_tilt_spring_angle_contribution;
			native.ssADSToLockOn = data.ss_ads_to_lock_on;
			native.ssLockOnMinAngle = data.ss_lock_on_min_angle;
			native.ssLockOnMaxAngle = data.ss_lock_on_max_angle;
			native.ssPilotAssistNoLockAimScale = data.ss_pilot_assist_no_lock_aim_scale;
			native.ssPilotAssistNoLockMinAngle = data.ss_pilot_assist_no_lock_min_angle;
			native.ssPilotAssistNoLockMaxAngle = data.ss_pilot_assist_no_lock_max_angle;
			native.ssPilotAssistAimScale = data.ss_pilot_assist_aim_scale;
			native.ssPilotAssistMinAngle = data.ss_pilot_assist_min_angle;
			native.ssPilotAssistMaxAngle = data.ss_pilot_assist_max_angle;
			plain::write_scalars(&native.ssAutoLevelStrengthHover, data.ss_auto_level_strength_hover);
			plain::write_scalars(&native.ssAutoLevelStrengthFly, data.ss_auto_level_strength_fly);
			native.ssIsRollRightStick = data.ss_is_roll_right_stick;
			native.ssRollMaxSpeedDps = data.ss_roll_max_speed_dps;
			native.ssBoostSpeedScale = data.ss_boost_speed_scale;
			native.ssBoostThrustScale = data.ss_boost_thrust_scale;
			native.ssEnableBoostToFly = data.ss_enable_boost_to_fly;
			native.ssBoostTime = data.ss_boost_time;
			native.ssBoostRegenTime = data.ss_boost_regen_time;
			native.ssBoostRegenStallTime = data.ss_boost_regen_stall_time;
			native.ssBoostMinThresholdRatio = data.ss_boost_min_threshold_ratio;
			native.ssCollisionRadiusI = data.ss_collision_radius_i;
			native.ssCylinderHeightI = data.ss_cylinder_height_i;
			native.ssUseCylinder = data.ss_use_cylinder;
			native.ssUsePlayerClip = data.ss_use_player_clip;
			native.ssCollisionOffsetI = data.ss_collision_offset_i;
			native.ssCollisionHardThresholdMph = data.ss_collision_hard_threshold_mph;
			native.ssFOVFlyOffsetDeg = data.ss_fov_fly_offset_deg;
			native.ssFOVFlyBoostOffsetDeg = data.ss_fov_fly_boost_offset_deg;
			native.ssFOVHoverOffsetDeg = data.ss_fov_hover_offset_deg;
			native.ssZoomBlendSpeedDps = data.ss_zoom_blend_speed_dps;
			native.ssZoomMaxAngle = data.ss_zoom_max_angle;
			native.ssZoomMinDistI = data.ss_zoom_min_dist_i;
			native.ssZoomIdealDistI = data.ss_zoom_ideal_dist_i;
			native.ssZoomMaxDistI = data.ss_zoom_max_dist_i;
			native.ssShowPilotViewmodel = data.ss_show_pilot_viewmodel;
			native.ssCameraAnimScale = data.ss_camera_anim_scale;
			plain::write_scalars(&native.ssFlyTiltShipAngles, data.ss_fly_tilt_ship_angles);
			native.ssFlyTiltCameraScale = data.ss_fly_tilt_camera_scale;
			plain::write_scalars(&native.ssHoverTiltShipAngles, data.ss_hover_tilt_ship_angles);
			native.ssHoverTiltCameraScale = data.ss_hover_tilt_camera_scale;
			native.ssMotionIdleRateMinSpeed = data.ss_motion_idle_rate_min_speed;
			native.ssMotionIdleRateMaxSpeed = data.ss_motion_idle_rate_max_speed;
			native.ssMotionIdleMinRate = data.ss_motion_idle_min_rate;
			native.ssMotionIdleMaxRate = data.ss_motion_idle_max_rate;
			native.ssAnimTree = static_cast<scr_string_t>(plain::write_script_string(data.ss_anim_tree));
			for (std::size_t i = 0; i < data.ss_anim_parts.size(); ++i)
			{
				write_spaceship_anim_parts(native.ssAnimParts[i], data.ss_anim_parts[i], resolve, memory);
			}
			write_fx_combined_def(native.ssThrustFxLoop, data.ss_thrust_fx_loop, resolve, memory);
			write_fx_combined_def(native.ssJukeFx, data.ss_juke_fx, resolve, memory);
			native.ssIdleRumble = plain::resolve_asset<RumbleInfo>(resolve, ASSET_TYPE_RUMBLE, data.ss_idle_rumble);
			native.ssSmallRumble = plain::resolve_asset<RumbleInfo>(resolve, ASSET_TYPE_RUMBLE, data.ss_small_rumble);
			native.ssMedRumble = plain::resolve_asset<RumbleInfo>(resolve, ASSET_TYPE_RUMBLE, data.ss_med_rumble);
			native.ssLargeRumble = plain::resolve_asset<RumbleInfo>(resolve, ASSET_TYPE_RUMBLE, data.ss_large_rumble);
			write_snd_alias_lookup(native.rattleLoop, data.rattle_loop, resolve, memory);
			native.rattleLoopMinVolume = data.rattle_loop_min_volume;
			native.rattleLoopMaxVolume = data.rattle_loop_max_volume;
			write_snd_alias_lookup(native.airLoop, data.air_loop, resolve, memory);
			native.airLoopMinVolume = data.air_loop_min_volume;
			native.airLoopMaxVolume = data.air_loop_max_volume;
			write_snd_alias_lookup(native.engineLoop, data.engine_loop, resolve, memory);
			native.engineLoopMinVolume = data.engine_loop_min_volume;
			native.engineLoopMaxVolume = data.engine_loop_max_volume;
			native.engineLoopMinPitch = data.engine_loop_min_pitch;
			native.engineLoopMaxPitch = data.engine_loop_max_pitch;
			write_snd_alias_lookup(native.hoverLoop, data.hover_loop, resolve, memory);
			native.hoverLoopMinVolume = data.hover_loop_min_volume;
			native.hoverLoopMaxVolume = data.hover_loop_max_volume;
			native.hoverLoopMinPitch = data.hover_loop_min_pitch;
			native.hoverLoopMaxPitch = data.hover_loop_max_pitch;
			write_snd_alias_lookup(native.boostLoop, data.boost_loop, resolve, memory);
			native.boostLoopMaxVolume = data.boost_loop_max_volume;
			native.boostLoopMaxPitch = data.boost_loop_max_pitch;
			write_snd_alias_lookup(native.ssThrustLoop, data.ss_thrust_loop, resolve, memory);
			native.ssThrustLoopMaxVolume = data.ss_thrust_loop_max_volume;
			native.ssThrustLoopMaxPitch = data.ss_thrust_loop_max_pitch;
			write_snd_alias_lookup(native.boostStart, data.boost_start, resolve, memory);
			write_snd_alias_lookup(native.boostStop, data.boost_stop, resolve, memory);
			write_snd_alias_lookup(native.boostDepleted, data.boost_depleted, resolve, memory);
			write_snd_alias_lookup(native.boostUnavailable, data.boost_unavailable, resolve, memory);
			write_snd_alias_lookup(native.jukeLeft, data.juke_left, resolve, memory);
			write_snd_alias_lookup(native.jukeRight, data.juke_right, resolve, memory);
			write_snd_alias_lookup(native.jukeUpDown, data.juke_up_down, resolve, memory);
			write_snd_alias_lookup(native.jukeBack, data.juke_back, resolve, memory);
			write_snd_alias_lookup(native.jukeFront, data.juke_front, resolve, memory);
			write_snd_alias_lookup(native.flightOn, data.flight_on, resolve, memory);
			write_snd_alias_lookup(native.flightOff, data.flight_off, resolve, memory);
			native.flightOnTimer = data.flight_on_timer;
			native.flightOffTimer = data.flight_off_timer;
			write_snd_alias_lookup(native.hardCollision, data.hard_collision, resolve, memory);
			write_snd_alias_lookup(native.softCollision, data.soft_collision, resolve, memory);
			native.camLookEnabled = data.cam_look_enabled;
			native.camRelativeControl = data.cam_relative_control;
			native.camRemoteDrive = data.cam_remote_drive;
			native.camLerp = data.cam_lerp;
			native.camHeight = data.cam_height;
			native.camRadius = data.cam_radius;
			native.camPitchInfluence = data.cam_pitch_influence;
			native.camYawInfluence = data.cam_yaw_influence;
			native.camRollInfluence = data.cam_roll_influence;
			native.camFovIncrease = data.cam_fov_increase;
			native.camFovOffset = data.cam_fov_offset;
			native.camFovSpeed = data.cam_fov_speed;
			native.camReturnSpeed = data.cam_return_speed;
			native.camReturnLerp = data.cam_return_lerp;
			native.camVehicleAnglePitchRate = data.cam_vehicle_angle_pitch_rate;
			native.camVehicleAngleYawRate = data.cam_vehicle_angle_yaw_rate;
			native.camVehicleAngleRollRate = data.cam_vehicle_angle_roll_rate;
			native.vehCam_UseGDT = data.veh_cam_use_gdt;
			native.vehCam_anglesPitch = data.veh_cam_angles_pitch;
			native.vehCam_anglesYaw = data.veh_cam_angles_yaw;
			native.vehCam_anglesRoll = data.veh_cam_angles_roll;
			native.vehCam_offsetX = data.veh_cam_offset_x;
			native.vehCam_offsetY = data.veh_cam_offset_y;
			native.vehCam_offsetZ = data.veh_cam_offset_z;
			native.vehCam_radius = data.veh_cam_radius;
			native.vehCam_speedInfluence = data.veh_cam_speed_influence;
			native.vehCam_pitchTurnRate = data.veh_cam_pitch_turn_rate;
			native.vehCam_pitchClamp = data.veh_cam_pitch_clamp;
			native.vehCam_yawTurnRate = data.veh_cam_yaw_turn_rate;
			native.vehCam_yawClamp = data.veh_cam_yaw_clamp;
			native.vehCam_zOffsetMode = static_cast<VehCamZOffsetMode>(plain::enum_value(asset_data::content::veh_cam_z_offset_mode_names, data.veh_cam_z_offset_mode, "VehicleDef.vehCam_zOffsetMode"));
			native.vehCam_anglesPitch3P = data.veh_cam_angles_pitch3_p;
			native.vehCam_anglesYaw3P = data.veh_cam_angles_yaw3_p;
			native.vehCam_anglesRoll3P = data.veh_cam_angles_roll3_p;
			native.vehCam_offsetX3P = data.veh_cam_offset_x3_p;
			native.vehCam_offsetY3P = data.veh_cam_offset_y3_p;
			native.vehCam_offsetZ3P = data.veh_cam_offset_z3_p;
			native.vehCam_radius3P = data.veh_cam_radius3_p;
			native.vehCam_speedInfluence3P = data.veh_cam_speed_influence3_p;
			native.vehCam_pitchTurnRate3P = data.veh_cam_pitch_turn_rate3_p;
			native.vehCam_pitchClamp3P = data.veh_cam_pitch_clamp3_p;
			native.vehCam_yawTurnRate3P = data.veh_cam_yaw_turn_rate3_p;
			native.vehCam_yawClamp3P = data.veh_cam_yaw_clamp3_p;
			native.vehCam_zOffsetMode3P = static_cast<VehCamZOffsetMode>(plain::enum_value(asset_data::content::veh_cam_z_offset_mode_names, data.veh_cam_z_offset_mode3_p, "VehicleDef.vehCam_zOffsetMode3P"));
			native.turretWeaponName = plain::write_string(memory, data.turret_weapon_name);
			native.turretWeapon = plain::resolve_asset<WeaponCompleteDef>(resolve, ASSET_TYPE_WEAPON, data.turret_weapon);
			native.turretHorizSpanLeft = data.turret_horiz_span_left;
			native.turretHorizSpanRight = data.turret_horiz_span_right;
			native.turretVertSpanUp = data.turret_vert_span_up;
			native.turretVertSpanDown = data.turret_vert_span_down;
			native.turretHorizResistLeft = data.turret_horiz_resist_left;
			native.turretHorizResistRight = data.turret_horiz_resist_right;
			native.turretVertResistUp = data.turret_vert_resist_up;
			native.turretVertResistDown = data.turret_vert_resist_down;
			native.turretRotRate = data.turret_rot_rate;
			native.turretFireType = static_cast<VehicleTurretFireType>(plain::enum_value(asset_data::content::vehicle_turret_fire_type_names, data.turret_fire_type, "VehicleDef.turretFireType"));
			write_snd_alias_lookup(native.turretSpinSnd, data.turret_spin_snd, resolve, memory);
			write_snd_alias_lookup(native.turretStopSnd, data.turret_stop_snd, resolve, memory);
			native.trophyEnabled = data.trophy_enabled;
			native.trophyRadius = data.trophy_radius;
			native.trophyInactiveRadius = data.trophy_inactive_radius;
			native.trophyAmmoCount = data.trophy_ammo_count;
			native.trophyReloadTime = data.trophy_reload_time;
			for (std::size_t i = 0; i < data.trophy_tags.size(); ++i)
			{
				native.trophyTags[i] = static_cast<scr_string_t>(plain::write_script_string(data.trophy_tags[i]));
			}
			write_fx_combined_def(native.trophyExplodeFx, data.trophy_explode_fx, resolve, memory);
			write_fx_combined_def(native.trophyFlashFx, data.trophy_flash_fx, resolve, memory);
			native.compassFriendlyIcon = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, data.compass_friendly_icon);
			native.compassEnemyIcon = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, data.compass_enemy_icon);
			native.compassFriendlyAltIcon = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, data.compass_friendly_alt_icon);
			native.compassEnemyAltIcon = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, data.compass_enemy_alt_icon);
			native.compassIconWidth = data.compass_icon_width;
			native.compassIconHeight = data.compass_icon_height;
			native.detailSoundBankNPC = plain::resolve_asset<SndBankTransient>(resolve, ASSET_TYPE_SOUND_BANK_TRANSIENT, data.detail_sound_bank_npc);
			native.detailSoundBankPlayer = plain::resolve_asset<SndBankTransient>(resolve, ASSET_TYPE_SOUND_BANK_TRANSIENT, data.detail_sound_bank_player);
			write_snd_alias_lookup(native.idleLowSnd, data.idle_low_snd, resolve, memory);
			write_snd_alias_lookup(native.idleHighSnd, data.idle_high_snd, resolve, memory);
			write_snd_alias_lookup(native.engineLowSnd, data.engine_low_snd, resolve, memory);
			write_snd_alias_lookup(native.engineHighSnd, data.engine_high_snd, resolve, memory);
			write_snd_alias_lookup(native.fallbackIdleLowSnd, data.fallback_idle_low_snd, resolve, memory);
			write_snd_alias_lookup(native.fallbackIdleHighSnd, data.fallback_idle_high_snd, resolve, memory);
			write_snd_alias_lookup(native.fallbackEngineLowSnd, data.fallback_engine_low_snd, resolve, memory);
			write_snd_alias_lookup(native.fallbackEngineHighSnd, data.fallback_engine_high_snd, resolve, memory);
			native.engineSndSpeed = data.engine_snd_speed;
			native.audioOriginTag = static_cast<scr_string_t>(plain::write_script_string(data.audio_origin_tag));
			write_snd_alias_lookup(native.mainRotorLowRpmSnd, data.main_rotor_low_rpm_snd, resolve, memory);
			write_snd_alias_lookup(native.mainRotorOperatingRpmSnd, data.main_rotor_operating_rpm_snd, resolve, memory);
			write_snd_alias_lookup(native.mainRotorOperatingEffortSnd, data.main_rotor_operating_effort_snd, resolve, memory);
			write_snd_alias_lookup(native.tailRotorLowRpmSnd, data.tail_rotor_low_rpm_snd, resolve, memory);
			write_snd_alias_lookup(native.tailRotorOperatingRpmSnd, data.tail_rotor_operating_rpm_snd, resolve, memory);
			write_snd_alias_lookup(native.tailRotorOperatingEffortSndLeft, data.tail_rotor_operating_effort_snd_left, resolve, memory);
			write_snd_alias_lookup(native.tailRotorOperatingEffortSndRight, data.tail_rotor_operating_effort_snd_right, resolve, memory);
			write_snd_alias_lookup(native.physicsHeliStartup, data.physics_heli_startup, resolve, memory);
			write_snd_alias_lookup(native.physicsHeliShutdown, data.physics_heli_shutdown, resolve, memory);
			write_snd_alias_lookup(native.turbineLowRpmSnd, data.turbine_low_rpm_snd, resolve, memory);
			write_snd_alias_lookup(native.turbineOperatingRpmSnd, data.turbine_operating_rpm_snd, resolve, memory);
			native.useRevAudioSettings = data.use_rev_audio_settings;
			write_snd_alias_lookup(native.revLowFidelityMod, data.rev_low_fidelity_mod, resolve, memory);
			write_snd_alias_lookup(native.revHighFidelityMod, data.rev_high_fidelity_mod, resolve, memory);
			native.revPlayerPriority = data.rev_player_priority;
			write_vehicle_physics_rev_modifier(native.revModifier, data.rev_modifier, resolve, memory);
			write_snd_alias_lookup(native.engineStartUpSnd, data.engine_start_up_snd, resolve, memory);
			native.engineStartUpLength = data.engine_start_up_length;
			write_snd_alias_lookup(native.engineShutdownSnd, data.engine_shutdown_snd, resolve, memory);
			write_snd_alias_lookup(native.engineIdleSnd, data.engine_idle_snd, resolve, memory);
			write_snd_alias_lookup(native.engineSustainSnd, data.engine_sustain_snd, resolve, memory);
			write_snd_alias_lookup(native.engineRampUpSnd, data.engine_ramp_up_snd, resolve, memory);
			native.engineRampUpLength = data.engine_ramp_up_length;
			write_snd_alias_lookup(native.engineRampDownSnd, data.engine_ramp_down_snd, resolve, memory);
			native.engineRampDownLength = data.engine_ramp_down_length;
			write_snd_alias_lookup(native.suspensionSoftSnd, data.suspension_soft_snd, resolve, memory);
			native.suspensionSoftCompression = data.suspension_soft_compression;
			write_snd_alias_lookup(native.suspensionHardSnd, data.suspension_hard_snd, resolve, memory);
			native.suspensionHardCompression = data.suspension_hard_compression;
			write_snd_alias_lookup(native.collisionSnd, data.collision_snd, resolve, memory);
			native.collisionBlendSpeed = data.collision_blend_speed;
			write_snd_alias_lookup(native.speedSnd, data.speed_snd, resolve, memory);
			native.speedSndBlendSpeed = data.speed_snd_blend_speed;
			native.surfaceSndName = plain::write_string(memory, data.surface_snd_name);
			native.surfaceSndBlendSpeed = data.surface_snd_blend_speed;
			native.slideVolume = data.slide_volume;
			native.slideBlendSpeed = data.slide_blend_speed;
			native.inAirPitch = data.in_air_pitch;
			native.soundTriggerOverrideZone = plain::write_string(memory, data.sound_trigger_override_zone);
			native.soundTriggerOverrideReverb = data.sound_trigger_override_reverb;
			native.soundTriggerOverrideMix = data.sound_trigger_override_mix;
			native.soundTriggerOverrideFilter = data.sound_trigger_override_filter;
			native.soundTriggerOverrideOcclusion = data.sound_trigger_override_occlusion;
			native.soundTriggerOverrideAmbient = data.sound_trigger_override_ambient;
			native.soundTriggerOverrideAmbientEvents = data.sound_trigger_override_ambient_events;
			native.soundTriggerOverrideADSR = data.sound_trigger_override_adsr;
			native.visionBlendTime = data.vision_blend_time;
			native.globalVisionSettings = plain::write_string(memory, data.global_vision_settings);
			native.mapVisionSettings = plain::write_string(memory, data.map_vision_settings);
			native.luiCrosshairWidget = plain::write_string(memory, data.lui_crosshair_widget);
			std::ranges::copy(data.dlc_float, native.dlcFloat);
		}

		std::size_t size_vehicle([[maybe_unused]] const asset_data::content::vehicle& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			size += plain::string_size(data.use_hint_string);
			size += size_vehicle_physics_def(data.vehicle_physics_def);
			size += size_fx_combined_def(data.veh_helicopter_ground_fx);
			size += size_fx_combined_def(data.veh_helicopter_ground_water_fx);
			for (const auto& item : data.ss_anim_parts)
			{
				size += size_spaceship_anim_parts(item);
			}
			size += size_fx_combined_def(data.ss_thrust_fx_loop);
			size += size_fx_combined_def(data.ss_juke_fx);
			size += size_snd_alias_lookup(data.rattle_loop);
			size += size_snd_alias_lookup(data.air_loop);
			size += size_snd_alias_lookup(data.engine_loop);
			size += size_snd_alias_lookup(data.hover_loop);
			size += size_snd_alias_lookup(data.boost_loop);
			size += size_snd_alias_lookup(data.ss_thrust_loop);
			size += size_snd_alias_lookup(data.boost_start);
			size += size_snd_alias_lookup(data.boost_stop);
			size += size_snd_alias_lookup(data.boost_depleted);
			size += size_snd_alias_lookup(data.boost_unavailable);
			size += size_snd_alias_lookup(data.juke_left);
			size += size_snd_alias_lookup(data.juke_right);
			size += size_snd_alias_lookup(data.juke_up_down);
			size += size_snd_alias_lookup(data.juke_back);
			size += size_snd_alias_lookup(data.juke_front);
			size += size_snd_alias_lookup(data.flight_on);
			size += size_snd_alias_lookup(data.flight_off);
			size += size_snd_alias_lookup(data.hard_collision);
			size += size_snd_alias_lookup(data.soft_collision);
			size += plain::string_size(data.turret_weapon_name);
			size += size_snd_alias_lookup(data.turret_spin_snd);
			size += size_snd_alias_lookup(data.turret_stop_snd);
			size += size_fx_combined_def(data.trophy_explode_fx);
			size += size_fx_combined_def(data.trophy_flash_fx);
			size += size_snd_alias_lookup(data.idle_low_snd);
			size += size_snd_alias_lookup(data.idle_high_snd);
			size += size_snd_alias_lookup(data.engine_low_snd);
			size += size_snd_alias_lookup(data.engine_high_snd);
			size += size_snd_alias_lookup(data.fallback_idle_low_snd);
			size += size_snd_alias_lookup(data.fallback_idle_high_snd);
			size += size_snd_alias_lookup(data.fallback_engine_low_snd);
			size += size_snd_alias_lookup(data.fallback_engine_high_snd);
			size += size_snd_alias_lookup(data.main_rotor_low_rpm_snd);
			size += size_snd_alias_lookup(data.main_rotor_operating_rpm_snd);
			size += size_snd_alias_lookup(data.main_rotor_operating_effort_snd);
			size += size_snd_alias_lookup(data.tail_rotor_low_rpm_snd);
			size += size_snd_alias_lookup(data.tail_rotor_operating_rpm_snd);
			size += size_snd_alias_lookup(data.tail_rotor_operating_effort_snd_left);
			size += size_snd_alias_lookup(data.tail_rotor_operating_effort_snd_right);
			size += size_snd_alias_lookup(data.physics_heli_startup);
			size += size_snd_alias_lookup(data.physics_heli_shutdown);
			size += size_snd_alias_lookup(data.turbine_low_rpm_snd);
			size += size_snd_alias_lookup(data.turbine_operating_rpm_snd);
			size += size_snd_alias_lookup(data.rev_low_fidelity_mod);
			size += size_snd_alias_lookup(data.rev_high_fidelity_mod);
			size += size_vehicle_physics_rev_modifier(data.rev_modifier);
			size += size_snd_alias_lookup(data.engine_start_up_snd);
			size += size_snd_alias_lookup(data.engine_shutdown_snd);
			size += size_snd_alias_lookup(data.engine_idle_snd);
			size += size_snd_alias_lookup(data.engine_sustain_snd);
			size += size_snd_alias_lookup(data.engine_ramp_up_snd);
			size += size_snd_alias_lookup(data.engine_ramp_down_snd);
			size += size_snd_alias_lookup(data.suspension_soft_snd);
			size += size_snd_alias_lookup(data.suspension_hard_snd);
			size += size_snd_alias_lookup(data.collision_snd);
			size += size_snd_alias_lookup(data.speed_snd);
			size += plain::string_size(data.surface_snd_name);
			size += plain::string_size(data.sound_trigger_override_zone);
			size += plain::string_size(data.global_vision_settings);
			size += plain::string_size(data.map_vision_settings);
			size += plain::string_size(data.lui_crosshair_widget);
			return size;
		}

		asset_data::content::x_cam_target_model read_x_cam_target_model(const XCamTargetModel& native)
		{
			plain::trace("read_x_cam_target_model");
			asset_data::content::x_cam_target_model data;
			data.name = plain::read_string(native.name);
			data.animation_frames = plain::read_array(native.animationFrames, native.frames, "XCamTargetModel.animationFrames", [&](const auto& item) { return read_x_cam_target_model_frame(item); });
			return data;
		}

		void write_x_cam_target_model(XCamTargetModel& native, const asset_data::content::x_cam_target_model& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.animationFrames = plain::write_array<XCamTargetModelFrame>(data.animation_frames, memory, [&](auto& out, const auto& in) { write_x_cam_target_model_frame(out, in, resolve, memory); });
			native.frames = plain::count_of<std::int32_t>(data.animation_frames);
		}

		std::size_t size_x_cam_target_model([[maybe_unused]] const asset_data::content::x_cam_target_model& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.animation_frames)
			{
				size += plain::sized(std::max<std::size_t>(data.animation_frames->size(), 1) * sizeof(XCamTargetModelFrame));
				for (const auto& item : *data.animation_frames)
				{
					size += size_x_cam_target_model_frame(item);
				}
			}
			return size;
		}

		asset_data::content::xcam read_xcam(const XCam& native)
		{
			plain::trace("read_xcam");
			asset_data::content::xcam data;
			data.name = plain::read_string(native.name);
			data.parent_scene = plain::read_string(native.parent_scene);
			data.hide_hud = native.hideHud;
			data.looping = native.looping;
			data.hide_local_player_model = native.hideLocalPlayerModel;
			data.use_first_person_player_model = native.useFirstPersonPlayerModel;
			data.disable_near_dof = native.disableNearDof;
			data.auto_motion_blur = native.autoMotionBlur;
			data.ease_animations_out = native.easeAnimationsOut;
			data.show_non_animated_players = native.showNonAnimatedPlayers;
			data.camera_count = native.cameraCount;
			data.frames = native.frames;
			data.frame_rate = native.frameRate;
			data.frame_name_count = native.frameNameCount;
			data.rotation_offset_inv = plain::read_scalars<float, 4>(&native.rotationOffsetInv);
			data.offset = plain::read_scalars<float, 3>(&native.offset);
			data.target_model_bone_roots = plain::read_single(native.targetModelBoneRoots, [&](const auto& item) { return read_x_cam_target_model(item); });
			data.cameras = plain::read_array_if_present(native.cameras, native.cameraCount, "XCam.cameras", [&](const auto& item) { return read_x_cam_camera(item); });
			data.camera_switch_params = plain::read_array_if_present(native.cameraSwitchParams, native.frames, "XCam.cameraSwitchParams", [&](const auto& item) { return read_x_cam_camera_switch_params(item); });
			data.frame_names = plain::read_array_if_present(native.frameNames, native.frameNameCount, "XCam.frameNames", [&](const auto& item) { return read_x_cam_frame_name(item); });
			data.notify = plain::read_array_if_present(native.notify, native.notifyCount, "XCam.notify", [&](const auto& item) { return read_x_anim_notify_info(item); });
			data.notify_count = native.notifyCount;
			data.startup_notify = plain::read_array_if_present(native.startupNotify, native.startupNotifyCount, "XCam.startupNotify", [&](const auto& item) { return read_x_anim_notify_info(item); });
			data.startup_notify_count = native.startupNotifyCount;
			data.shutdown_notify = plain::read_array_if_present(native.shutdownNotify, native.shutdownNotifyCount, "XCam.shutdownNotify", [&](const auto& item) { return read_x_anim_notify_info(item); });
			data.shutdown_notify_count = native.shutdownNotifyCount;
			data.right_stick_rotate_offset = plain::read_scalars<float, 3>(&native.rightStickRotateOffset);
			data.right_stick_rotate_max_degrees = plain::read_scalars<float, 2>(&native.rightStickRotateMaxDegrees);
			data.free_cursor_parallax = native.freeCursorParallax;
			return data;
		}

		void write_xcam(XCam& native, const asset_data::content::xcam& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.parent_scene = plain::write_string(memory, data.parent_scene);
			native.hideHud = data.hide_hud;
			native.looping = data.looping;
			native.hideLocalPlayerModel = data.hide_local_player_model;
			native.useFirstPersonPlayerModel = data.use_first_person_player_model;
			native.disableNearDof = data.disable_near_dof;
			native.autoMotionBlur = data.auto_motion_blur;
			native.easeAnimationsOut = data.ease_animations_out;
			native.showNonAnimatedPlayers = data.show_non_animated_players;
			native.cameraCount = data.camera_count;
			native.frames = data.frames;
			native.frameRate = data.frame_rate;
			native.frameNameCount = data.frame_name_count;
			plain::write_scalars(&native.rotationOffsetInv, data.rotation_offset_inv);
			plain::write_scalars(&native.offset, data.offset);
			native.targetModelBoneRoots = plain::write_single<XCamTargetModel>(data.target_model_bone_roots, memory, [&](auto& out, const auto& in) { write_x_cam_target_model(out, in, resolve, memory); });
			native.cameras = plain::write_array<XCamCamera>(data.cameras, memory, [&](auto& out, const auto& in) { write_x_cam_camera(out, in, resolve, memory); });
			native.cameraSwitchParams = plain::write_array<XCamCameraSwitchParams>(data.camera_switch_params, memory, [&](auto& out, const auto& in) { write_x_cam_camera_switch_params(out, in, resolve, memory); });
			native.frameNames = plain::write_array<XCamFrameName>(data.frame_names, memory, [&](auto& out, const auto& in) { write_x_cam_frame_name(out, in, resolve, memory); });
			native.notify = plain::write_array<XAnimNotifyInfo>(data.notify, memory, [&](auto& out, const auto& in) { write_x_anim_notify_info(out, in, resolve, memory); });
			native.notifyCount = data.notify_count;
			native.startupNotify = plain::write_array<XAnimNotifyInfo>(data.startup_notify, memory, [&](auto& out, const auto& in) { write_x_anim_notify_info(out, in, resolve, memory); });
			native.startupNotifyCount = data.startup_notify_count;
			native.shutdownNotify = plain::write_array<XAnimNotifyInfo>(data.shutdown_notify, memory, [&](auto& out, const auto& in) { write_x_anim_notify_info(out, in, resolve, memory); });
			native.shutdownNotifyCount = data.shutdown_notify_count;
			plain::write_scalars(&native.rightStickRotateOffset, data.right_stick_rotate_offset);
			plain::write_scalars(&native.rightStickRotateMaxDegrees, data.right_stick_rotate_max_degrees);
			native.freeCursorParallax = data.free_cursor_parallax;
		}

		std::size_t size_xcam([[maybe_unused]] const asset_data::content::xcam& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			size += plain::string_size(data.parent_scene);
			if (data.target_model_bone_roots)
			{
				size += plain::sized(sizeof(XCamTargetModel));
				size += size_x_cam_target_model(*data.target_model_bone_roots);
			}
			if (data.cameras)
			{
				size += plain::sized(std::max<std::size_t>(data.cameras->size(), 1) * sizeof(XCamCamera));
				for (const auto& item : *data.cameras)
				{
					size += size_x_cam_camera(item);
				}
			}
			if (data.camera_switch_params)
			{
				size += plain::sized(std::max<std::size_t>(data.camera_switch_params->size(), 1) * sizeof(XCamCameraSwitchParams));
				for (const auto& item : *data.camera_switch_params)
				{
					size += size_x_cam_camera_switch_params(item);
				}
			}
			if (data.frame_names)
			{
				size += plain::sized(std::max<std::size_t>(data.frame_names->size(), 1) * sizeof(XCamFrameName));
				for (const auto& item : *data.frame_names)
				{
					size += size_x_cam_frame_name(item);
				}
			}
			if (data.notify)
			{
				size += plain::sized(std::max<std::size_t>(data.notify->size(), 1) * sizeof(XAnimNotifyInfo));
				for (const auto& item : *data.notify)
				{
					size += size_x_anim_notify_info(item);
				}
			}
			if (data.startup_notify)
			{
				size += plain::sized(std::max<std::size_t>(data.startup_notify->size(), 1) * sizeof(XAnimNotifyInfo));
				for (const auto& item : *data.startup_notify)
				{
					size += size_x_anim_notify_info(item);
				}
			}
			if (data.shutdown_notify)
			{
				size += plain::sized(std::max<std::size_t>(data.shutdown_notify->size(), 1) * sizeof(XAnimNotifyInfo));
				for (const auto& item : *data.shutdown_notify)
				{
					size += size_x_anim_notify_info(item);
				}
			}
			return size;
		}

		asset_data::content::x_cam_target_model_frame read_x_cam_target_model_frame(const XCamTargetModelFrame& native)
		{
			plain::trace("read_x_cam_target_model_frame");
			asset_data::content::x_cam_target_model_frame data;
			data.offset = plain::read_scalars<float, 3>(&native.offset);
			data.axis_quat = plain::read_scalars<float, 4>(&native.axisQuat);
			return data;
		}

		void write_x_cam_target_model_frame(XCamTargetModelFrame& native, const asset_data::content::x_cam_target_model_frame& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.offset, data.offset);
			plain::write_scalars(&native.axisQuat, data.axis_quat);
		}

		std::size_t size_x_cam_target_model_frame([[maybe_unused]] const asset_data::content::x_cam_target_model_frame& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::x_cam_camera read_x_cam_camera(const XCamCamera& native)
		{
			plain::trace("read_x_cam_camera");
			asset_data::content::x_cam_camera data;
			data.name = plain::read_string(native.name);
			data.index = native.index;
			data.aperture_type = plain::enum_name(asset_data::content::x_cam_aperture_type_names, static_cast<std::int64_t>(native.apertureType), "XCamCamera.apertureType");
			data.animation_frames = plain::read_array(native.animationFrames, native.frames, "XCamCamera.animationFrames", [&](const auto& item) { return read_x_cam_frame(item); });
			data.far_z = native.farZ;
			data.focus_shift_multiplier = native.focusShiftMultiplier;
			return data;
		}

		void write_x_cam_camera(XCamCamera& native, const asset_data::content::x_cam_camera& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.index = data.index;
			native.apertureType = static_cast<XCamApertureType>(plain::enum_value(asset_data::content::x_cam_aperture_type_names, data.aperture_type, "XCamCamera.apertureType"));
			native.animationFrames = plain::write_array<XCamFrame>(data.animation_frames, memory, [&](auto& out, const auto& in) { write_x_cam_frame(out, in, resolve, memory); });
			native.farZ = data.far_z;
			native.focusShiftMultiplier = data.focus_shift_multiplier;
			native.frames = plain::count_of<std::int32_t>(data.animation_frames);
		}

		std::size_t size_x_cam_camera([[maybe_unused]] const asset_data::content::x_cam_camera& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.animation_frames)
			{
				size += plain::sized(std::max<std::size_t>(data.animation_frames->size(), 1) * sizeof(XCamFrame));
				for (const auto& item : *data.animation_frames)
				{
					size += size_x_cam_frame(item);
				}
			}
			return size;
		}

		asset_data::content::x_cam_frame read_x_cam_frame(const XCamFrame& native)
		{
			plain::trace("read_x_cam_frame");
			asset_data::content::x_cam_frame data;
			data.frame_num = native.frameNum;
			data.origin = plain::read_scalars<float, 3>(&native.origin);
			data.angle_quat = plain::read_scalars<float, 4>(&native.angleQuat);
			data.fov = native.fov;
			data.focal_length = native.focalLength;
			data.focal_distance = native.focalDistance;
			data.f_stop = native.fStop;
			return data;
		}

		void write_x_cam_frame(XCamFrame& native, const asset_data::content::x_cam_frame& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.frameNum = data.frame_num;
			plain::write_scalars(&native.origin, data.origin);
			plain::write_scalars(&native.angleQuat, data.angle_quat);
			native.fov = data.fov;
			native.focalLength = data.focal_length;
			native.focalDistance = data.focal_distance;
			native.fStop = data.f_stop;
		}

		std::size_t size_x_cam_frame([[maybe_unused]] const asset_data::content::x_cam_frame& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::x_cam_camera_switch_params read_x_cam_camera_switch_params(const XCamCameraSwitchParams& native)
		{
			plain::trace("read_x_cam_camera_switch_params");
			asset_data::content::x_cam_camera_switch_params data;
			data.frame_num = native.frameNum;
			data.camera1_index = native.camera1Index;
			data.camera2_index = native.camera2Index;
			data.dissolve = native.dissolve;
			return data;
		}

		void write_x_cam_camera_switch_params(XCamCameraSwitchParams& native, const asset_data::content::x_cam_camera_switch_params& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.frameNum = data.frame_num;
			native.camera1Index = data.camera1_index;
			native.camera2Index = data.camera2_index;
			native.dissolve = data.dissolve;
		}

		std::size_t size_x_cam_camera_switch_params([[maybe_unused]] const asset_data::content::x_cam_camera_switch_params& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::x_cam_frame_name read_x_cam_frame_name(const XCamFrameName& native)
		{
			plain::trace("read_x_cam_frame_name");
			asset_data::content::x_cam_frame_name data;
			data.frame_name = plain::read_string(native.frameName);
			data.frame_num = native.frameNum;
			return data;
		}

		void write_x_cam_frame_name(XCamFrameName& native, const asset_data::content::x_cam_frame_name& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.frameName = plain::write_string(memory, data.frame_name);
			native.frameNum = data.frame_num;
		}

		std::size_t size_x_cam_frame_name([[maybe_unused]] const asset_data::content::x_cam_frame_name& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.frame_name);
			return size;
		}

		asset_data::content::x_anim_notify_info read_x_anim_notify_info(const XAnimNotifyInfo& native)
		{
			plain::trace("read_x_anim_notify_info");
			asset_data::content::x_anim_notify_info data;
			data.name = plain::read_script_string(native.name);
			data.time = native.time;
			return data;
		}

		void write_x_anim_notify_info(XAnimNotifyInfo& native, const asset_data::content::x_anim_notify_info& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = static_cast<scr_string_t>(plain::write_script_string(data.name));
			native.time = data.time;
		}

		std::size_t size_x_anim_notify_info([[maybe_unused]] const asset_data::content::x_anim_notify_info& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::spatial_partition_transient_population read_spatial_partition_transient_population(const SpatialPartition_TransientPopulation& native)
		{
			plain::trace("read_spatial_partition_transient_population");
			asset_data::content::spatial_partition_transient_population data;
			data.buckets = plain::read_array(native.buckets, native.partitionCount, "SpatialPartition_TransientPopulation.buckets", [&](const auto& item) { return read_spatial_partition_population_node(item); });
			data.bucket_tails = plain::read_array(native.bucketTails, native.partitionCount, "SpatialPartition_TransientPopulation.bucketTails", [&](const auto& item) { return read_spatial_partition_population_node(item); });
			data.partition_ids = plain::read_array(native.partitionIds, native.partitionCount, "SpatialPartition_TransientPopulation.partitionIds", [&](const auto& item) { return item; });
			data.current_population = native.currentPopulation;
			return data;
		}

		void write_spatial_partition_transient_population(SpatialPartition_TransientPopulation& native, const asset_data::content::spatial_partition_transient_population& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.buckets = plain::write_array<SpatialPartition_Population_Node>(data.buckets, memory, [&](auto& out, const auto& in) { write_spatial_partition_population_node(out, in, resolve, memory); });
			native.bucketTails = plain::write_array<SpatialPartition_Population_Node>(data.bucket_tails, memory, [&](auto& out, const auto& in) { write_spatial_partition_population_node(out, in, resolve, memory); });
			native.partitionIds = plain::write_array<std::uint32_t>(data.partition_ids, memory, [&](auto& out, const auto& in) { out = in; });
			native.currentPopulation = data.current_population;
			native.partitionCount = plain::count_of<std::uint32_t>(data.buckets);
		}

		std::size_t size_spatial_partition_transient_population([[maybe_unused]] const asset_data::content::spatial_partition_transient_population& data)
		{
			std::size_t size = 0;
			if (data.buckets)
			{
				size += plain::sized(std::max<std::size_t>(data.buckets->size(), 1) * sizeof(SpatialPartition_Population_Node));
				for (const auto& item : *data.buckets)
				{
					size += size_spatial_partition_population_node(item);
				}
			}
			if (data.bucket_tails)
			{
				size += plain::sized(std::max<std::size_t>(data.bucket_tails->size(), 1) * sizeof(SpatialPartition_Population_Node));
				for (const auto& item : *data.bucket_tails)
				{
					size += size_spatial_partition_population_node(item);
				}
			}
			if (data.partition_ids)
			{
				size += plain::sized(std::max<std::size_t>(data.partition_ids->size(), 1) * sizeof(std::uint32_t));
			}
			return size;
		}

		asset_data::content::spatial_partition_population_node read_spatial_partition_population_node(const SpatialPartition_Population_Node& native)
		{
			plain::trace("read_spatial_partition_population_node");
			asset_data::content::spatial_partition_population_node data;
			data.spatial_node_next = native.spatialNodeNext;
			return data;
		}

		void write_spatial_partition_population_node(SpatialPartition_Population_Node& native, const asset_data::content::spatial_partition_population_node& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.spatialNodeNext = data.spatial_node_next;
		}

		std::size_t size_spatial_partition_population_node([[maybe_unused]] const asset_data::content::spatial_partition_population_node& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::dynentitylist read_dynentitylist(const DynEntityList& native)
		{
			plain::trace("read_dynentitylist");
			asset_data::content::dynentitylist data;
			data.name = plain::read_string(native.name);
			data.index = static_cast<std::uint16_t>(native.index);
			data.dyn_ent_count = std::to_array(native.dynEntCount);
			data.dyn_ent_count_total = native.dynEntCountTotal;
			data.dyn_ent_def_list_0 = plain::read_array(native.dynEntDefList[0], native.dynEntCount[0], "DynEntityList.dynEntDefList[0]", [&](const auto& item) { return read_dyn_entity_def(item); });
			data.dyn_ent_def_list_1 = plain::read_array(native.dynEntDefList[1], native.dynEntCount[1], "DynEntityList.dynEntDefList[1]", [&](const auto& item) { return read_dyn_entity_def(item); });
			data.dyn_ent_spatial_population_0 = plain::read_single(native.dynEntSpatialPopulation[0], [&](const auto& item) { return read_spatial_partition_transient_population(item); });
			data.dyn_ent_spatial_population_1 = plain::read_single(native.dynEntSpatialPopulation[1], [&](const auto& item) { return read_spatial_partition_transient_population(item); });
			data.dyn_ent_no_spatial_list = read_spatial_partition_population_node(native.dynEntNoSpatialList);
			data.dyn_ent_no_spatial_list_tail = read_spatial_partition_population_node(native.dynEntNoSpatialListTail);
			return data;
		}

		void write_dynentitylist(DynEntityList& native, const asset_data::content::dynentitylist& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.index = static_cast<DynEntityListId>(data.index);
			std::ranges::copy(data.dyn_ent_count, native.dynEntCount);
			native.dynEntCountTotal = data.dyn_ent_count_total;
			native.dynEntDefList[0] = plain::write_array<DynEntityDef>(data.dyn_ent_def_list_0, memory, [&](auto& out, const auto& in) { write_dyn_entity_def(out, in, resolve, memory); });
			native.dynEntDefList[1] = plain::write_array<DynEntityDef>(data.dyn_ent_def_list_1, memory, [&](auto& out, const auto& in) { write_dyn_entity_def(out, in, resolve, memory); });
			native.dynEntSpatialPopulation[0] = plain::write_single<SpatialPartition_TransientPopulation>(data.dyn_ent_spatial_population_0, memory, [&](auto& out, const auto& in) { write_spatial_partition_transient_population(out, in, resolve, memory); });
			native.dynEntSpatialPopulation[1] = plain::write_single<SpatialPartition_TransientPopulation>(data.dyn_ent_spatial_population_1, memory, [&](auto& out, const auto& in) { write_spatial_partition_transient_population(out, in, resolve, memory); });
			write_spatial_partition_population_node(native.dynEntNoSpatialList, data.dyn_ent_no_spatial_list, resolve, memory);
			write_spatial_partition_population_node(native.dynEntNoSpatialListTail, data.dyn_ent_no_spatial_list_tail, resolve, memory);
		}

		std::size_t size_dynentitylist([[maybe_unused]] const asset_data::content::dynentitylist& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.dyn_ent_def_list_0)
			{
				size += plain::sized(std::max<std::size_t>(data.dyn_ent_def_list_0->size(), 1) * sizeof(DynEntityDef));
				for (const auto& item : *data.dyn_ent_def_list_0)
				{
					size += size_dyn_entity_def(item);
				}
			}
			if (data.dyn_ent_def_list_1)
			{
				size += plain::sized(std::max<std::size_t>(data.dyn_ent_def_list_1->size(), 1) * sizeof(DynEntityDef));
				for (const auto& item : *data.dyn_ent_def_list_1)
				{
					size += size_dyn_entity_def(item);
				}
			}
			if (data.dyn_ent_spatial_population_0)
			{
				size += plain::sized(sizeof(SpatialPartition_TransientPopulation));
				size += size_spatial_partition_transient_population(*data.dyn_ent_spatial_population_0);
			}
			if (data.dyn_ent_spatial_population_1)
			{
				size += plain::sized(sizeof(SpatialPartition_TransientPopulation));
				size += size_spatial_partition_transient_population(*data.dyn_ent_spatial_population_1);
			}
			size += size_spatial_partition_population_node(data.dyn_ent_no_spatial_list);
			size += size_spatial_partition_population_node(data.dyn_ent_no_spatial_list_tail);
			return size;
		}

		asset_data::content::client_entity_link_to_def read_client_entity_link_to_def(const ClientEntityLinkToDef& native)
		{
			plain::trace("read_client_entity_link_to_def");
			asset_data::content::client_entity_link_to_def data;
			data.anchor_index = native.anchorIndex;
			data.origin_offset = plain::read_scalars<float, 3>(&native.originOffset);
			data.angle_offset = plain::read_scalars<float, 3>(&native.angleOffset);
			return data;
		}

		void write_client_entity_link_to_def(ClientEntityLinkToDef& native, const asset_data::content::client_entity_link_to_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.anchorIndex = data.anchor_index;
			plain::write_scalars(&native.originOffset, data.origin_offset);
			plain::write_scalars(&native.angleOffset, data.angle_offset);
		}

		std::size_t size_client_entity_link_to_def([[maybe_unused]] const asset_data::content::client_entity_link_to_def& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::content::dyn_entity_def read_dyn_entity_def(const DynEntityDef& native)
		{
			plain::trace("read_dyn_entity_def");
			asset_data::content::dyn_entity_def data;
			data.base_model = plain::read_asset(ASSET_TYPE_XMODEL, native.baseModel);
			data.link_to = plain::read_single(native.linkTo, [&](const auto& item) { return read_client_entity_link_to_def(item); });
			data.initial_pose = read_gfx_placement(native.initialPose);
			data.spatial_node = read_spatial_partition_population_node(native.spatialNode);
			data.client_active_mask = native.clientActiveMask;
			data.basis = plain::enum_name(asset_data::content::dyn_entity_basis_names, static_cast<std::int64_t>(native.basis), "DynEntityDef.basis");
			data.type = plain::enum_name(asset_data::content::dyn_entity_type_names, static_cast<std::int64_t>(native.type), "DynEntityDef.type");
			data.priority = plain::enum_name(asset_data::content::dyn_entity_spatial_activation_mode_names, static_cast<std::int64_t>(native.priority), "DynEntityDef.priority");
			data.spatial_partition_index = native.spatialPartitionIndex;
			data.client_id = std::to_array(native.clientId);
			data.scriptable_map_index = native.scriptableMapIndex;
			data.dyn_entity_list_id = static_cast<std::uint16_t>(native.dynEntityListId);
			data.brush_model = native.brushModel;
			data.scriptable_sub_index = native.scriptableSubIndex;
			data.transient_index_stored = native.transientIndexStored;
			data.scriptable_max_part_count_or_physics_asset_rigid_bodies_count = plain::read_scalars<std::uint8_t, 1>(&native.___u15)[0];
			data.spawn_active = native.spawnActive;
			data.no_physics = native.noPhysics;
			data.distant_shadows = native.distantShadows;
			data.no_spot_shadows = native.noSpotShadows;
			data.collision_flags = native.collisionFlags;
			data.is_transient = native.isTransient;
			data.transient_zone_loaded = native.transientZoneLoaded;
			return data;
		}

		void write_dyn_entity_def(DynEntityDef& native, const asset_data::content::dyn_entity_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.baseModel = plain::resolve_asset<XModel>(resolve, ASSET_TYPE_XMODEL, data.base_model);
			native.linkTo = plain::write_single<ClientEntityLinkToDef>(data.link_to, memory, [&](auto& out, const auto& in) { write_client_entity_link_to_def(out, in, resolve, memory); });
			write_gfx_placement(native.initialPose, data.initial_pose, resolve, memory);
			write_spatial_partition_population_node(native.spatialNode, data.spatial_node, resolve, memory);
			native.clientActiveMask = data.client_active_mask;
			native.basis = static_cast<DynEntityBasis>(plain::enum_value(asset_data::content::dyn_entity_basis_names, data.basis, "DynEntityDef.basis"));
			native.type = static_cast<DynEntityType>(plain::enum_value(asset_data::content::dyn_entity_type_names, data.type, "DynEntityDef.type"));
			native.priority = static_cast<DynEntitySpatialActivationMode>(plain::enum_value(asset_data::content::dyn_entity_spatial_activation_mode_names, data.priority, "DynEntityDef.priority"));
			native.spatialPartitionIndex = data.spatial_partition_index;
			std::ranges::copy(data.client_id, native.clientId);
			native.scriptableMapIndex = data.scriptable_map_index;
			native.dynEntityListId = static_cast<DynEntityListId>(data.dyn_entity_list_id);
			native.brushModel = data.brush_model;
			native.scriptableSubIndex = data.scriptable_sub_index;
			native.transientIndexStored = data.transient_index_stored;
			plain::write_scalars(&native.___u15, std::array<std::uint8_t, 1>{data.scriptable_max_part_count_or_physics_asset_rigid_bodies_count});
			native.spawnActive = data.spawn_active;
			native.noPhysics = data.no_physics;
			native.distantShadows = data.distant_shadows;
			native.noSpotShadows = data.no_spot_shadows;
			native.collisionFlags = data.collision_flags;
			native.isTransient = data.is_transient;
			native.transientZoneLoaded = data.transient_zone_loaded;
		}

		std::size_t size_dyn_entity_def([[maybe_unused]] const asset_data::content::dyn_entity_def& data)
		{
			std::size_t size = 0;
			if (data.link_to)
			{
				size += plain::sized(sizeof(ClientEntityLinkToDef));
				size += size_client_entity_link_to_def(*data.link_to);
			}
			size += size_gfx_placement(data.initial_pose);
			size += size_spatial_partition_population_node(data.spatial_node);
			return size;
		}

	}

	std::expected<asset_data::content::attachment, std::string> to_neutral(const WeaponAttachment& native)
	{
		try
		{
			auto data = read_attachment(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	WeaponAttachment* from_neutral(const asset_data::content::attachment& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<WeaponAttachment>();
		write_attachment(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::content::attachment& data)
	{
		return plain::rounded(sizeof(WeaponAttachment)) + size_attachment(data) + 64;
	}

	std::expected<asset_data::content::animclass, std::string> to_neutral(const AnimationClass& native)
	{
		try
		{
			auto data = read_animclass(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	AnimationClass* from_neutral(const asset_data::content::animclass& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<AnimationClass>();
		write_animclass(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::content::animclass& data)
	{
		return plain::rounded(sizeof(AnimationClass)) + size_animclass(data) + 64;
	}

	std::expected<asset_data::content::luafile, std::string> to_neutral(const LuaFile& native)
	{
		try
		{
			auto data = read_luafile(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	LuaFile* from_neutral(const asset_data::content::luafile& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<LuaFile>();
		write_luafile(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::content::luafile& data)
	{
		return plain::rounded(sizeof(LuaFile)) + size_luafile(data) + 64;
	}

	std::expected<asset_data::content::leaderboarddef, std::string> to_neutral(const LeaderboardDef& native)
	{
		try
		{
			auto data = read_leaderboarddef(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	LeaderboardDef* from_neutral(const asset_data::content::leaderboarddef& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<LeaderboardDef>();
		write_leaderboarddef(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::content::leaderboarddef& data)
	{
		return plain::rounded(sizeof(LeaderboardDef)) + size_leaderboarddef(data) + 64;
	}

	std::expected<asset_data::content::virtualleaderboarddef, std::string> to_neutral(const VirtualLeaderboardDef& native)
	{
		try
		{
			auto data = read_virtualleaderboarddef(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	VirtualLeaderboardDef* from_neutral(const asset_data::content::virtualleaderboarddef& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<VirtualLeaderboardDef>();
		write_virtualleaderboarddef(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::content::virtualleaderboarddef& data)
	{
		return plain::rounded(sizeof(VirtualLeaderboardDef)) + size_virtualleaderboarddef(data) + 64;
	}

	std::expected<asset_data::content::clothasset, std::string> to_neutral(const ClothAsset& native)
	{
		try
		{
			auto data = read_clothasset(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	ClothAsset* from_neutral(const asset_data::content::clothasset& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<ClothAsset>();
		write_clothasset(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::content::clothasset& data)
	{
		return plain::rounded(sizeof(ClothAsset)) + size_clothasset(data) + 64;
	}

	std::expected<asset_data::content::stterrain, std::string> to_neutral(const StTerrain& native)
	{
		try
		{
			auto data = read_stterrain(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	StTerrain* from_neutral(const asset_data::content::stterrain& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<StTerrain>();
		write_stterrain(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::content::stterrain& data)
	{
		return plain::rounded(sizeof(StTerrain)) + size_stterrain(data) + 64;
	}

	std::expected<asset_data::content::collisiontile, std::string> to_neutral(const CollisionTile& native)
	{
		try
		{
			auto data = read_collisiontile(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	CollisionTile* from_neutral(const asset_data::content::collisiontile& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<CollisionTile>();
		write_collisiontile(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::content::collisiontile& data)
	{
		return plain::rounded(sizeof(CollisionTile)) + size_collisiontile(data) + 64;
	}

	std::expected<asset_data::content::vehicle, std::string> to_neutral(const VehicleDef& native)
	{
		try
		{
			auto data = read_vehicle(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	VehicleDef* from_neutral(const asset_data::content::vehicle& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<VehicleDef>();
		write_vehicle(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::content::vehicle& data)
	{
		return plain::rounded(sizeof(VehicleDef)) + size_vehicle(data) + 64;
	}

	std::expected<asset_data::content::xcam, std::string> to_neutral(const XCam& native)
	{
		try
		{
			auto data = read_xcam(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	XCam* from_neutral(const asset_data::content::xcam& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<XCam>();
		write_xcam(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::content::xcam& data)
	{
		return plain::rounded(sizeof(XCam)) + size_xcam(data) + 64;
	}

	std::expected<asset_data::content::dynentitylist, std::string> to_neutral(const DynEntityList& native)
	{
		try
		{
			auto data = read_dynentitylist(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	DynEntityList* from_neutral(const asset_data::content::dynentitylist& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<DynEntityList>();
		write_dynentitylist(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::content::dynentitylist& data)
	{
		return plain::rounded(sizeof(DynEntityList)) + size_dynentitylist(data) + 64;
	}

}
