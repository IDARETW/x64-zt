// Emitted by tools/gen_iw8_neutral.py from tools/neutral_families/sound_globals.json: do not edit.
#include <std_include.hpp>
#include "sound_globals.hpp"
#include "plain.hpp"

namespace zonetool::iw8::neutral
{
	namespace
	{
		asset_data::sound_globals::sound_volume_modifier read_sound_volume_modifier(const snd_volmod_info_t& native)
		{
			asset_data::sound_globals::sound_volume_modifier data;
			data.name = plain::read_text(native.name, "snd_volmod_info_t.name");
			data.id = native.id;
			data.value = native.value;
			return data;
		}

		void write_sound_volume_modifier(snd_volmod_info_t& native, const asset_data::sound_globals::sound_volume_modifier& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_text(native.name, data.name, "snd_volmod_info_t.name");
			native.id = data.id;
			native.value = data.value;
		}

		std::size_t size_sound_volume_modifier([[maybe_unused]] const asset_data::sound_globals::sound_volume_modifier& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::sound_globals::sound_entity_channel read_sound_entity_channel(const snd_entchannel_info_t& native)
		{
			asset_data::sound_globals::sound_entity_channel data;
			data.name = plain::read_text(native.name, "snd_entchannel_info_t.name");
			data.id = native.id;
			data.priority = native.priority;
			data.spatial_type = plain::enum_name(asset_data::sound_globals::sound_entity_channel_spaital_type_names, static_cast<std::int64_t>(native.spatialType), "snd_entchannel_info_t.spatialType");
			data.is_restricted = native.isRestricted;
			data.is_pausable = native.isPausable;
			data.max_voices = native.maxVoices;
			data.limit_behavior = plain::enum_name(asset_data::sound_globals::sound_limit_behavior_names, static_cast<std::int64_t>(native.limitBehavior), "snd_entchannel_info_t.limitBehavior");
			data.center_speaker_pan = native.centerSpeakerPan;
			data.mp_far_reverb_behavior = plain::enum_name(asset_data::sound_globals::sound_far_reverb_behavior_names, static_cast<std::int64_t>(native.mpFarReverbBehavior), "snd_entchannel_info_t.mpFarReverbBehavior");
			data.sp_far_reverb_behavior = plain::enum_name(asset_data::sound_globals::sound_far_reverb_behavior_names, static_cast<std::int64_t>(native.spFarReverbBehavior), "snd_entchannel_info_t.spFarReverbBehavior");
			data.cp_far_reverb_behavior = plain::enum_name(asset_data::sound_globals::sound_far_reverb_behavior_names, static_cast<std::int64_t>(native.cpFarReverbBehavior), "snd_entchannel_info_t.cpFarReverbBehavior");
			data.has71_pan = native.has71Pan;
			data.bus_id = native.busId;
			data.is_player_dialogue = native.isPlayerDialogue;
			data.send_type = plain::enum_name(asset_data::sound_globals::sound_send_type_names, static_cast<std::int64_t>(native.sendType), "snd_entchannel_info_t.sendType");
			data.speed_of_sound_mult = native.speedOfSoundMult;
			return data;
		}

		void write_sound_entity_channel(snd_entchannel_info_t& native, const asset_data::sound_globals::sound_entity_channel& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_text(native.name, data.name, "snd_entchannel_info_t.name");
			native.id = data.id;
			native.priority = data.priority;
			native.spatialType = static_cast<SndEntchannelSpaitalType>(plain::enum_value(asset_data::sound_globals::sound_entity_channel_spaital_type_names, data.spatial_type, "snd_entchannel_info_t.spatialType"));
			native.isRestricted = data.is_restricted;
			native.isPausable = data.is_pausable;
			native.maxVoices = data.max_voices;
			native.limitBehavior = static_cast<SndLimitBehavior>(plain::enum_value(asset_data::sound_globals::sound_limit_behavior_names, data.limit_behavior, "snd_entchannel_info_t.limitBehavior"));
			native.centerSpeakerPan = data.center_speaker_pan;
			native.mpFarReverbBehavior = static_cast<SndFarReverbBehavior>(plain::enum_value(asset_data::sound_globals::sound_far_reverb_behavior_names, data.mp_far_reverb_behavior, "snd_entchannel_info_t.mpFarReverbBehavior"));
			native.spFarReverbBehavior = static_cast<SndFarReverbBehavior>(plain::enum_value(asset_data::sound_globals::sound_far_reverb_behavior_names, data.sp_far_reverb_behavior, "snd_entchannel_info_t.spFarReverbBehavior"));
			native.cpFarReverbBehavior = static_cast<SndFarReverbBehavior>(plain::enum_value(asset_data::sound_globals::sound_far_reverb_behavior_names, data.cp_far_reverb_behavior, "snd_entchannel_info_t.cpFarReverbBehavior"));
			native.has71Pan = data.has71_pan;
			native.busId = data.bus_id;
			native.isPlayerDialogue = data.is_player_dialogue;
			native.sendType = static_cast<SndSendType>(plain::enum_value(asset_data::sound_globals::sound_send_type_names, data.send_type, "snd_entchannel_info_t.sendType"));
			native.speedOfSoundMult = data.speed_of_sound_mult;
		}

		std::size_t size_sound_entity_channel([[maybe_unused]] const asset_data::sound_globals::sound_entity_channel& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::sound_globals::sound_context read_sound_context(const SndContext& native)
		{
			asset_data::sound_globals::sound_context data;
			data.type = plain::read_string(native.type);
			data.type_id = native.typeId;
			data.num_values = native.numValues;
			data.start_index = native.startIndex;
			data.allow_defaults = native.allowDefaults;
			data.include_defaults_in_random = native.includeDefaultsInRandom;
			data.can_blend = native.canBlend;
			return data;
		}

		void write_sound_context(SndContext& native, const asset_data::sound_globals::sound_context& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.type = plain::write_string(memory, data.type);
			native.typeId = data.type_id;
			native.numValues = data.num_values;
			native.startIndex = data.start_index;
			native.allowDefaults = data.allow_defaults;
			native.includeDefaultsInRandom = data.include_defaults_in_random;
			native.canBlend = data.can_blend;
		}

		std::size_t size_sound_context([[maybe_unused]] const asset_data::sound_globals::sound_context& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.type);
			return size;
		}

		asset_data::sound_globals::sound_context_value read_sound_context_value(const SndContextValue& native)
		{
			asset_data::sound_globals::sound_context_value data;
			data.value = plain::read_string(native.value);
			data.value_id = native.valueId;
			return data;
		}

		void write_sound_context_value(SndContextValue& native, const asset_data::sound_globals::sound_context_value& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.value = plain::write_string(memory, data.value);
			native.valueId = data.value_id;
		}

		std::size_t size_sound_context_value([[maybe_unused]] const asset_data::sound_globals::sound_context_value& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.value);
			return size;
		}

		asset_data::sound_globals::sound_master_eq_param read_sound_master_eq_param(const SndMasterEqParam& native)
		{
			asset_data::sound_globals::sound_master_eq_param data;
			data.enabled = native.enabled;
			data.gain = native.gain;
			data.frequency = native.frequency;
			data.q = native.q;
			return data;
		}

		void write_sound_master_eq_param(SndMasterEqParam& native, const asset_data::sound_globals::sound_master_eq_param& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.enabled = data.enabled;
			native.gain = data.gain;
			native.frequency = data.frequency;
			native.q = data.q;
		}

		std::size_t size_sound_master_eq_param([[maybe_unused]] const asset_data::sound_globals::sound_master_eq_param& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::sound_globals::sound_master_eq_param_bank read_sound_master_eq_param_bank(const SndMasterEqParamBank& native)
		{
			asset_data::sound_globals::sound_master_eq_param_bank data;
			data.low = read_sound_master_eq_param(native.low);
			data.peak1 = read_sound_master_eq_param(native.peak1);
			data.peak2 = read_sound_master_eq_param(native.peak2);
			data.high = read_sound_master_eq_param(native.high);
			return data;
		}

		void write_sound_master_eq_param_bank(SndMasterEqParamBank& native, const asset_data::sound_globals::sound_master_eq_param_bank& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_sound_master_eq_param(native.low, data.low, resolve, memory);
			write_sound_master_eq_param(native.peak1, data.peak1, resolve, memory);
			write_sound_master_eq_param(native.peak2, data.peak2, resolve, memory);
			write_sound_master_eq_param(native.high, data.high, resolve, memory);
		}

		std::size_t size_sound_master_eq_param_bank([[maybe_unused]] const asset_data::sound_globals::sound_master_eq_param_bank& data)
		{
			std::size_t size = 0;
			size += size_sound_master_eq_param(data.low);
			size += size_sound_master_eq_param(data.peak1);
			size += size_sound_master_eq_param(data.peak2);
			size += size_sound_master_eq_param(data.high);
			return size;
		}

		asset_data::sound_globals::sound_dynamics_params read_sound_dynamics_params(const SndDynamicsParams& native)
		{
			asset_data::sound_globals::sound_dynamics_params data;
			data.enable = native.enable;
			data.pre_gain = native.preGain;
			data.post_gain = native.postGain;
			data.threshold = native.threshold;
			data.ratio = native.ratio;
			data.time_attack = native.timeAttack;
			data.time_release = native.timeRelease;
			data.spread = native.spread;
			data.linear = native.linear;
			data.itu1770 = native.ITU1770;
			return data;
		}

		void write_sound_dynamics_params(SndDynamicsParams& native, const asset_data::sound_globals::sound_dynamics_params& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.enable = data.enable;
			native.preGain = data.pre_gain;
			native.postGain = data.post_gain;
			native.threshold = data.threshold;
			native.ratio = data.ratio;
			native.timeAttack = data.time_attack;
			native.timeRelease = data.time_release;
			native.spread = data.spread;
			native.linear = data.linear;
			native.ITU1770 = data.itu1770;
		}

		std::size_t size_sound_dynamics_params([[maybe_unused]] const asset_data::sound_globals::sound_dynamics_params& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::sound_globals::sound_peak_limiter_params read_sound_peak_limiter_params(const SndPeakLimiterParams& native)
		{
			asset_data::sound_globals::sound_peak_limiter_params data;
			data.enable = native.enable;
			data.input_gain = native.inputGain;
			data.output_gain = native.outputGain;
			data.threshold = native.threshold;
			data.release_min = native.releaseMin;
			data.release_max = native.releaseMax;
			return data;
		}

		void write_sound_peak_limiter_params(SndPeakLimiterParams& native, const asset_data::sound_globals::sound_peak_limiter_params& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.enable = data.enable;
			native.inputGain = data.input_gain;
			native.outputGain = data.output_gain;
			native.threshold = data.threshold;
			native.releaseMin = data.release_min;
			native.releaseMax = data.release_max;
		}

		std::size_t size_sound_peak_limiter_params([[maybe_unused]] const asset_data::sound_globals::sound_peak_limiter_params& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::sound_globals::sound_master read_sound_master(const SndMaster& native)
		{
			asset_data::sound_globals::sound_master data;
			data.name = plain::read_text(native.name, "SndMaster.name");
			data.id = native.id;
			data.dialog_eq = read_sound_master_eq_param_bank(native.dialogEq);
			data.weap_exp_scenes_eq = read_sound_master_eq_param_bank(native.weapExpScenesEq);
			data.sfx_eq = read_sound_master_eq_param_bank(native.sfxEq);
			data.main_eq = read_sound_master_eq_param_bank(native.mainEq);
			data.dialog_dyn = read_sound_dynamics_params(native.dialogDyn);
			data.sfx_dyn = read_sound_dynamics_params(native.sfxDyn);
			data.main_dyn = read_sound_dynamics_params(native.mainDyn);
			data.dialog_peak_limit = read_sound_peak_limiter_params(native.dialogPeakLimit);
			data.sfx_peak_limit = read_sound_peak_limiter_params(native.sfxPeakLimit);
			data.main_peak_limit = read_sound_peak_limiter_params(native.mainPeakLimit);
			data.bus_dialog_gain = native.busDialogGain;
			data.bus_weap_exp_scene_gain = native.busWeapExpSceneGain;
			data.bus_sfx_gain = native.busSfxGain;
			data.bus_reverb_gain = native.busReverbGain;
			data.bus_music_gain = native.busMusicGain;
			data.bus_movie_gain = native.busMovieGain;
			data.bus_reference_gain = native.busReferenceGain;
			data.bus_dialog_enable = native.busDialogEnable;
			data.bus_weap_exp_scene_enable = native.busWeapExpSceneEnable;
			data.bus_sfx_enable = native.busSfxEnable;
			data.bus_reverb_enable = native.busReverbEnable;
			data.bus_music_enable = native.busMusicEnable;
			data.bus_movie_enable = native.busMovieEnable;
			data.bus_reference_enable = native.busReferenceEnable;
			return data;
		}

		void write_sound_master(SndMaster& native, const asset_data::sound_globals::sound_master& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_text(native.name, data.name, "SndMaster.name");
			native.id = data.id;
			write_sound_master_eq_param_bank(native.dialogEq, data.dialog_eq, resolve, memory);
			write_sound_master_eq_param_bank(native.weapExpScenesEq, data.weap_exp_scenes_eq, resolve, memory);
			write_sound_master_eq_param_bank(native.sfxEq, data.sfx_eq, resolve, memory);
			write_sound_master_eq_param_bank(native.mainEq, data.main_eq, resolve, memory);
			write_sound_dynamics_params(native.dialogDyn, data.dialog_dyn, resolve, memory);
			write_sound_dynamics_params(native.sfxDyn, data.sfx_dyn, resolve, memory);
			write_sound_dynamics_params(native.mainDyn, data.main_dyn, resolve, memory);
			write_sound_peak_limiter_params(native.dialogPeakLimit, data.dialog_peak_limit, resolve, memory);
			write_sound_peak_limiter_params(native.sfxPeakLimit, data.sfx_peak_limit, resolve, memory);
			write_sound_peak_limiter_params(native.mainPeakLimit, data.main_peak_limit, resolve, memory);
			native.busDialogGain = data.bus_dialog_gain;
			native.busWeapExpSceneGain = data.bus_weap_exp_scene_gain;
			native.busSfxGain = data.bus_sfx_gain;
			native.busReverbGain = data.bus_reverb_gain;
			native.busMusicGain = data.bus_music_gain;
			native.busMovieGain = data.bus_movie_gain;
			native.busReferenceGain = data.bus_reference_gain;
			native.busDialogEnable = data.bus_dialog_enable;
			native.busWeapExpSceneEnable = data.bus_weap_exp_scene_enable;
			native.busSfxEnable = data.bus_sfx_enable;
			native.busReverbEnable = data.bus_reverb_enable;
			native.busMusicEnable = data.bus_music_enable;
			native.busMovieEnable = data.bus_movie_enable;
			native.busReferenceEnable = data.bus_reference_enable;
		}

		std::size_t size_sound_master([[maybe_unused]] const asset_data::sound_globals::sound_master& data)
		{
			std::size_t size = 0;
			size += size_sound_master_eq_param_bank(data.dialog_eq);
			size += size_sound_master_eq_param_bank(data.weap_exp_scenes_eq);
			size += size_sound_master_eq_param_bank(data.sfx_eq);
			size += size_sound_master_eq_param_bank(data.main_eq);
			size += size_sound_dynamics_params(data.dialog_dyn);
			size += size_sound_dynamics_params(data.sfx_dyn);
			size += size_sound_dynamics_params(data.main_dyn);
			size += size_sound_peak_limiter_params(data.dialog_peak_limit);
			size += size_sound_peak_limiter_params(data.sfx_peak_limit);
			size += size_sound_peak_limiter_params(data.main_peak_limit);
			return size;
		}

		asset_data::sound_globals::sound_whizby read_sound_whizby(const SndWhizby& native)
		{
			asset_data::sound_globals::sound_whizby data;
			data.name = plain::read_text(native.name, "SndWhizby.name");
			data.id = native.id;
			data.radius_near = native.radiusNear;
			data.radius_medium = native.radiusMedium;
			data.radius_far = native.radiusFar;
			data.spread_near = native.spreadNear;
			data.spread_medium = native.spreadMedium;
			data.spread_far = native.spreadFar;
			data.whizby_in_time_near = native.whizbyInTimeNear;
			data.whizby_in_time_medium = native.whizbyInTimeMedium;
			data.whizby_in_time_far = native.whizbyInTimeFar;
			data.shooter_dist_near = native.shooterDistNear;
			data.shooter_dist_medium = native.shooterDistMedium;
			data.shooter_dist_far = native.shooterDistFar;
			data.is_super_sonic = native.isSuperSonic;
			data.crack_max_angle = native.crackMaxAngle;
			data.bullet_speed = native.bulletSpeed;
			data.speed_of_sound = native.speedOfSound;
			return data;
		}

		void write_sound_whizby(SndWhizby& native, const asset_data::sound_globals::sound_whizby& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_text(native.name, data.name, "SndWhizby.name");
			native.id = data.id;
			native.radiusNear = data.radius_near;
			native.radiusMedium = data.radius_medium;
			native.radiusFar = data.radius_far;
			native.spreadNear = data.spread_near;
			native.spreadMedium = data.spread_medium;
			native.spreadFar = data.spread_far;
			native.whizbyInTimeNear = data.whizby_in_time_near;
			native.whizbyInTimeMedium = data.whizby_in_time_medium;
			native.whizbyInTimeFar = data.whizby_in_time_far;
			native.shooterDistNear = data.shooter_dist_near;
			native.shooterDistMedium = data.shooter_dist_medium;
			native.shooterDistFar = data.shooter_dist_far;
			native.isSuperSonic = data.is_super_sonic;
			native.crackMaxAngle = data.crack_max_angle;
			native.bulletSpeed = data.bullet_speed;
			native.speedOfSound = data.speed_of_sound;
		}

		std::size_t size_sound_whizby([[maybe_unused]] const asset_data::sound_globals::sound_whizby& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::sound_globals::sound_perk read_sound_perk(const snd_perk_info_t& native)
		{
			asset_data::sound_globals::sound_perk data;
			data.name = plain::read_text(native.name, "snd_perk_info_t.name");
			data.id = native.id;
			data.volume = native.volume;
			return data;
		}

		void write_sound_perk(snd_perk_info_t& native, const asset_data::sound_globals::sound_perk& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_text(native.name, data.name, "snd_perk_info_t.name");
			native.id = data.id;
			native.volume = data.volume;
		}

		std::size_t size_sound_perk([[maybe_unused]] const asset_data::sound_globals::sound_perk& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::sound_globals::sound_globals read_sound_globals(const SndGlobals& native)
		{
			asset_data::sound_globals::sound_globals data;
			data.name = plain::read_string(native.name);
			data.global_volume_modifier = native.globalVolumeModifier;
			data.bink_volume_modifier = native.binkVolumeModifier;
			data.volmodinfo = plain::read_array(native.volmodinfo, native.volmodinfoCount, "SndGlobals.volmodinfo", [&](const auto& item) { return read_sound_volume_modifier(item); });
			data.entity_channel_info = plain::read_array(native.entchannelInfo, native.entchannelCount, "SndGlobals.entchannelInfo", [&](const auto& item) { return read_sound_entity_channel(item); });
			data.contexts = plain::read_array(native.contexts, native.contextCount, "SndGlobals.contexts", [&](const auto& item) { return read_sound_context(item); });
			data.context_values = plain::read_array(native.contextValues, native.contextValueCount, "SndGlobals.contextValues", [&](const auto& item) { return read_sound_context_value(item); });
			data.masters = plain::read_array(native.masters, native.masterCount, "SndGlobals.masters", [&](const auto& item) { return read_sound_master(item); });
			data.whizbys = plain::read_array(native.whizbys, native.whizbyCount, "SndGlobals.whizbys", [&](const auto& item) { return read_sound_whizby(item); });
			data.perkinfo = plain::read_array(native.perkinfo, native.perkinfoCount, "SndGlobals.perkinfo", [&](const auto& item) { return read_sound_perk(item); });
			return data;
		}

		void write_sound_globals(SndGlobals& native, const asset_data::sound_globals::sound_globals& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.globalVolumeModifier = data.global_volume_modifier;
			native.binkVolumeModifier = data.bink_volume_modifier;
			native.volmodinfo = plain::write_array<snd_volmod_info_t>(data.volmodinfo, memory, [&](auto& out, const auto& in) { write_sound_volume_modifier(out, in, resolve, memory); });
			native.entchannelInfo = plain::write_array<snd_entchannel_info_t>(data.entity_channel_info, memory, [&](auto& out, const auto& in) { write_sound_entity_channel(out, in, resolve, memory); });
			native.contexts = plain::write_array<SndContext>(data.contexts, memory, [&](auto& out, const auto& in) { write_sound_context(out, in, resolve, memory); });
			native.contextValues = plain::write_array<SndContextValue>(data.context_values, memory, [&](auto& out, const auto& in) { write_sound_context_value(out, in, resolve, memory); });
			native.masters = plain::write_array<SndMaster>(data.masters, memory, [&](auto& out, const auto& in) { write_sound_master(out, in, resolve, memory); });
			native.whizbys = plain::write_array<SndWhizby>(data.whizbys, memory, [&](auto& out, const auto& in) { write_sound_whizby(out, in, resolve, memory); });
			native.perkinfo = plain::write_array<snd_perk_info_t>(data.perkinfo, memory, [&](auto& out, const auto& in) { write_sound_perk(out, in, resolve, memory); });
			native.volmodinfoCount = plain::count_of<std::uint32_t>(data.volmodinfo);
			native.entchannelCount = plain::count_of<std::uint32_t>(data.entity_channel_info);
			native.contextCount = plain::count_of<std::uint32_t>(data.contexts);
			native.contextValueCount = plain::count_of<std::uint32_t>(data.context_values);
			native.masterCount = plain::count_of<std::uint32_t>(data.masters);
			native.whizbyCount = plain::count_of<std::uint32_t>(data.whizbys);
			native.perkinfoCount = plain::count_of<std::uint32_t>(data.perkinfo);
		}

		std::size_t size_sound_globals([[maybe_unused]] const asset_data::sound_globals::sound_globals& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.volmodinfo)
			{
				size += plain::sized(std::max<std::size_t>(data.volmodinfo->size(), 1) * sizeof(snd_volmod_info_t));
				for (const auto& item : *data.volmodinfo)
				{
					size += size_sound_volume_modifier(item);
				}
			}
			if (data.entity_channel_info)
			{
				size += plain::sized(std::max<std::size_t>(data.entity_channel_info->size(), 1) * sizeof(snd_entchannel_info_t));
				for (const auto& item : *data.entity_channel_info)
				{
					size += size_sound_entity_channel(item);
				}
			}
			if (data.contexts)
			{
				size += plain::sized(std::max<std::size_t>(data.contexts->size(), 1) * sizeof(SndContext));
				for (const auto& item : *data.contexts)
				{
					size += size_sound_context(item);
				}
			}
			if (data.context_values)
			{
				size += plain::sized(std::max<std::size_t>(data.context_values->size(), 1) * sizeof(SndContextValue));
				for (const auto& item : *data.context_values)
				{
					size += size_sound_context_value(item);
				}
			}
			if (data.masters)
			{
				size += plain::sized(std::max<std::size_t>(data.masters->size(), 1) * sizeof(SndMaster));
				for (const auto& item : *data.masters)
				{
					size += size_sound_master(item);
				}
			}
			if (data.whizbys)
			{
				size += plain::sized(std::max<std::size_t>(data.whizbys->size(), 1) * sizeof(SndWhizby));
				for (const auto& item : *data.whizbys)
				{
					size += size_sound_whizby(item);
				}
			}
			if (data.perkinfo)
			{
				size += plain::sized(std::max<std::size_t>(data.perkinfo->size(), 1) * sizeof(snd_perk_info_t));
				for (const auto& item : *data.perkinfo)
				{
					size += size_sound_perk(item);
				}
			}
			return size;
		}

	}

	std::expected<asset_data::sound_globals::sound_globals, std::string> to_neutral(const SndGlobals& native)
	{
		try
		{
			auto data = read_sound_globals(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	SndGlobals* from_neutral(const asset_data::sound_globals::sound_globals& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<SndGlobals>();
		write_sound_globals(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::sound_globals::sound_globals& data)
	{
		return plain::rounded(sizeof(SndGlobals)) + size_sound_globals(data) + 64;
	}

}
