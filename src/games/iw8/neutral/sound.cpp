// Emitted by tools/gen_iw8_neutral.py from tools/neutral_families/sound.json: do not edit.
#include <std_include.hpp>
#include "sound.hpp"
#include "plain.hpp"

namespace zonetool::iw8::neutral
{
	namespace
	{
		asset_data::sound_alias read_sound_alias(const SndAlias& native)
		{
			asset_data::sound_alias data;
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

		void write_sound_alias(SndAlias& native, const asset_data::sound_alias& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
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

		std::size_t size_sound_alias([[maybe_unused]] const asset_data::sound_alias& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.alias_name);
			size += plain::string_size(data.subtitle);
			size += plain::string_size(data.secondary_alias_name);
			size += plain::string_size(data.asset_file_name);
			return size;
		}

		asset_data::sound_alias_list read_sound_alias_list(const SndAliasList& native)
		{
			asset_data::sound_alias_list data;
			data.alias_name = plain::read_string(native.aliasName);
			data.id = native.id;
			data.head = plain::read_array(native.head, native.count, "SndAliasList.head", [&](const auto& item) { return read_sound_alias(item); });
			data.sequence = native.sequence;
			return data;
		}

		void write_sound_alias_list(SndAliasList& native, const asset_data::sound_alias_list& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.aliasName = plain::write_string(memory, data.alias_name);
			native.id = data.id;
			native.head = plain::write_array<SndAlias>(data.head, memory, [&](auto& out, const auto& in) { write_sound_alias(out, in, resolve, memory); });
			native.sequence = data.sequence;
			native.count = plain::count_of<std::int32_t>(data.head);
		}

		std::size_t size_sound_alias_list([[maybe_unused]] const asset_data::sound_alias_list& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.alias_name);
			if (data.head)
			{
				size += plain::rounded(std::max<std::size_t>(data.head->size(), 1) * sizeof(SndAlias));
				for (const auto& item : *data.head)
				{
					size += size_sound_alias(item);
				}
			}
			return size;
		}

		asset_data::sound_index_entry read_sound_index_entry(const SndIndexEntry& native)
		{
			asset_data::sound_index_entry data;
			data.value = native.value;
			data.next = native.next;
			return data;
		}

		void write_sound_index_entry(SndIndexEntry& native, const asset_data::sound_index_entry& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.value = data.value;
			native.next = data.next;
		}

		std::size_t size_sound_index_entry([[maybe_unused]] const asset_data::sound_index_entry& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::sound_send_effect_params read_sound_send_effect_params(const SndSendEffectParams& native)
		{
			asset_data::sound_send_effect_params data;
			data.name = plain::read_text(native.name, "SndSendEffectParams.name");
			data.id = native.id;
			data.smoothing = native.smoothing;
			data.early_time = native.earlyTime;
			data.late_time = native.lateTime;
			data.early_gain = native.earlyGain;
			data.late_gain = native.lateGain;
			data.late_gain_prox = native.lateGainProx;
			data.return_gain = native.returnGain;
			data.early_lpf = native.earlyLpf;
			data.late_lpf = native.lateLpf;
			data.input_lpf = native.inputLpf;
			data.damp_lpf = native.dampLpf;
			data.wall_reflect = native.wallReflect;
			data.dry_gain = native.dryGain;
			data.early_size = native.earlySize;
			data.late_size = native.lateSize;
			data.diffusion = native.diffusion;
			data.rear_level = native.rearLevel;
			data.delay1enabled = native.delay1enabled;
			data.delay1delay_time_ms = native.delay1delayTimeMS;
			data.delay1_level = native.delay1Level;
			data.delay1_lpf_cutoff = native.delay1LpfCutoff;
			data.delay1_hpf_cutoff = native.delay1HpfCutoff;
			data.delay2enabled = native.delay2enabled;
			data.delay2delay_time_ms = native.delay2delayTimeMS;
			data.delay2_level = native.delay2Level;
			data.delay2_lpf_cutoff = native.delay2LpfCutoff;
			data.delay2_hpf_cutoff = native.delay2HpfCutoff;
			data.delay3enabled = native.delay3enabled;
			data.delay3delay_time_ms = native.delay3delayTimeMS;
			data.delay3_level = native.delay3Level;
			data.delay3_lpf_cutoff = native.delay3LpfCutoff;
			data.delay3_hpf_cutoff = native.delay3HpfCutoff;
			data.delay4enabled = native.delay4enabled;
			data.delay4delay_time_ms = native.delay4delayTimeMS;
			data.delay4_level = native.delay4Level;
			data.delay4_lpf_cutoff = native.delay4LpfCutoff;
			data.delay4_hpf_cutoff = native.delay4HpfCutoff;
			data.delay5enabled = native.delay5enabled;
			data.delay5delay_time_ms = native.delay5delayTimeMS;
			data.delay5_level = native.delay5Level;
			data.delay5_lpf_cutoff = native.delay5LpfCutoff;
			data.delay5_hpf_cutoff = native.delay5HpfCutoff;
			return data;
		}

		void write_sound_send_effect_params(SndSendEffectParams& native, const asset_data::sound_send_effect_params& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_text(native.name, data.name, "SndSendEffectParams.name");
			native.id = data.id;
			native.smoothing = data.smoothing;
			native.earlyTime = data.early_time;
			native.lateTime = data.late_time;
			native.earlyGain = data.early_gain;
			native.lateGain = data.late_gain;
			native.lateGainProx = data.late_gain_prox;
			native.returnGain = data.return_gain;
			native.earlyLpf = data.early_lpf;
			native.lateLpf = data.late_lpf;
			native.inputLpf = data.input_lpf;
			native.dampLpf = data.damp_lpf;
			native.wallReflect = data.wall_reflect;
			native.dryGain = data.dry_gain;
			native.earlySize = data.early_size;
			native.lateSize = data.late_size;
			native.diffusion = data.diffusion;
			native.rearLevel = data.rear_level;
			native.delay1enabled = data.delay1enabled;
			native.delay1delayTimeMS = data.delay1delay_time_ms;
			native.delay1Level = data.delay1_level;
			native.delay1LpfCutoff = data.delay1_lpf_cutoff;
			native.delay1HpfCutoff = data.delay1_hpf_cutoff;
			native.delay2enabled = data.delay2enabled;
			native.delay2delayTimeMS = data.delay2delay_time_ms;
			native.delay2Level = data.delay2_level;
			native.delay2LpfCutoff = data.delay2_lpf_cutoff;
			native.delay2HpfCutoff = data.delay2_hpf_cutoff;
			native.delay3enabled = data.delay3enabled;
			native.delay3delayTimeMS = data.delay3delay_time_ms;
			native.delay3Level = data.delay3_level;
			native.delay3LpfCutoff = data.delay3_lpf_cutoff;
			native.delay3HpfCutoff = data.delay3_hpf_cutoff;
			native.delay4enabled = data.delay4enabled;
			native.delay4delayTimeMS = data.delay4delay_time_ms;
			native.delay4Level = data.delay4_level;
			native.delay4LpfCutoff = data.delay4_lpf_cutoff;
			native.delay4HpfCutoff = data.delay4_hpf_cutoff;
			native.delay5enabled = data.delay5enabled;
			native.delay5delayTimeMS = data.delay5delay_time_ms;
			native.delay5Level = data.delay5_level;
			native.delay5LpfCutoff = data.delay5_lpf_cutoff;
			native.delay5HpfCutoff = data.delay5_hpf_cutoff;
		}

		std::size_t size_sound_send_effect_params([[maybe_unused]] const asset_data::sound_send_effect_params& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::adsr_setting read_adsr_setting(const ADSRSetting& native)
		{
			asset_data::adsr_setting data;
			data.name = plain::read_string(native.name);
			data.id = native.id;
			data.attack_length = native.attackLength;
			data.attack_curve = plain::enum_name(asset_data::adsr_curve_names, static_cast<std::int64_t>(native.attackCurve), "ADSRSetting.attackCurve");
			data.decay_length = native.decayLength;
			data.decay_curve = plain::enum_name(asset_data::adsr_curve_names, static_cast<std::int64_t>(native.decayCurve), "ADSRSetting.decayCurve");
			data.release_length = native.releaseLength;
			data.release_curve = plain::enum_name(asset_data::adsr_curve_names, static_cast<std::int64_t>(native.releaseCurve), "ADSRSetting.releaseCurve");
			data.sustain_level = native.sustainLevel;
			data.sustain_length = native.sustainLength;
			return data;
		}

		void write_adsr_setting(ADSRSetting& native, const asset_data::adsr_setting& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.id = data.id;
			native.attackLength = data.attack_length;
			native.attackCurve = static_cast<ADSRCurve>(plain::enum_value(asset_data::adsr_curve_names, data.attack_curve, "ADSRSetting.attackCurve"));
			native.decayLength = data.decay_length;
			native.decayCurve = static_cast<ADSRCurve>(plain::enum_value(asset_data::adsr_curve_names, data.decay_curve, "ADSRSetting.decayCurve"));
			native.releaseLength = data.release_length;
			native.releaseCurve = static_cast<ADSRCurve>(plain::enum_value(asset_data::adsr_curve_names, data.release_curve, "ADSRSetting.releaseCurve"));
			native.sustainLevel = data.sustain_level;
			native.sustainLength = data.sustain_length;
		}

		std::size_t size_adsr_setting([[maybe_unused]] const asset_data::adsr_setting& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			return size;
		}

		asset_data::ambient_def read_ambient_def(const AmbientDef& native)
		{
			asset_data::ambient_def data;
			data.name = plain::read_string(native.name);
			data.id = native.id;
			data.ambient_event_index = native.ambientEventIndex;
			data.num_events = native.numEvents;
			data.time_min = native.timeMin;
			data.time_max = native.timeMax;
			return data;
		}

		void write_ambient_def(AmbientDef& native, const asset_data::ambient_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.id = data.id;
			native.ambientEventIndex = data.ambient_event_index;
			native.numEvents = data.num_events;
			native.timeMin = data.time_min;
			native.timeMax = data.time_max;
		}

		std::size_t size_ambient_def([[maybe_unused]] const asset_data::ambient_def& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			return size;
		}

		asset_data::ambient_event read_ambient_event(const AmbientEvent& native)
		{
			asset_data::ambient_event data;
			data.id = native.id;
			data.ambient_element_index = native.ambientElementIndex;
			data.weight = native.weight;
			return data;
		}

		void write_ambient_event(AmbientEvent& native, const asset_data::ambient_event& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.id = data.id;
			native.ambientElementIndex = data.ambient_element_index;
			native.weight = data.weight;
		}

		std::size_t size_ambient_event([[maybe_unused]] const asset_data::ambient_event& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::ambient_element read_ambient_element(const AmbientElement& native)
		{
			asset_data::ambient_element data;
			data.id = native.id;
			plain::require_zero(native.unusedNameField, "AmbientElement.unusedNameField");
			data.alias_name = plain::read_string(native.aliasName);
			data.range_min = native.rangeMin;
			data.range_max = native.rangeMax;
			data.cone_min = native.coneMin;
			data.cone_max = native.coneMax;
			return data;
		}

		void write_ambient_element(AmbientElement& native, const asset_data::ambient_element& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.id = data.id;
			native.aliasName = plain::write_string(memory, data.alias_name);
			native.rangeMin = data.range_min;
			native.rangeMax = data.range_max;
			native.coneMin = data.cone_min;
			native.coneMax = data.cone_max;
		}

		std::size_t size_ambient_element([[maybe_unused]] const asset_data::ambient_element& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.alias_name);
			return size;
		}

		asset_data::filter_def read_filter_def(const FilterDef& native)
		{
			asset_data::filter_def data;
			data.id = native.id;
			data.ent_channel_idx = native.entChannelIdx;
			data.band = native.band;
			data.type = plain::enum_name(asset_data::sound_eqtype_names, static_cast<std::int64_t>(native.type), "FilterDef.type");
			data.freq = native.freq;
			data.gain = native.gain;
			data.q = native.q;
			return data;
		}

		void write_filter_def(FilterDef& native, const asset_data::filter_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.id = data.id;
			native.entChannelIdx = data.ent_channel_idx;
			native.band = data.band;
			native.type = static_cast<SND_EQTYPE>(plain::enum_value(asset_data::sound_eqtype_names, data.type, "FilterDef.type"));
			native.freq = data.freq;
			native.gain = data.gain;
			native.q = data.q;
		}

		std::size_t size_filter_def([[maybe_unused]] const asset_data::filter_def& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::mix_def read_mix_def(const MixDef& native)
		{
			asset_data::mix_def data;
			data.id = native.id;
			data.vol_mod_index = native.volModIndex;
			data.volume = native.volume;
			data.fade = native.fade;
			return data;
		}

		void write_mix_def(MixDef& native, const asset_data::mix_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.id = data.id;
			native.volModIndex = data.vol_mod_index;
			native.volume = data.volume;
			native.fade = data.fade;
		}

		std::size_t size_mix_def([[maybe_unused]] const asset_data::mix_def& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::adsr_zone_entry read_adsr_zone_entry(const AdsrZoneEntry& native)
		{
			asset_data::adsr_zone_entry data;
			data.name = plain::read_string(native.name);
			data.id = native.id;
			data.weapon_name = plain::read_string(native.weaponName);
			data.adsr_idx = native.adsrIdx;
			data.weapon_idx = native.weaponIdx;
			return data;
		}

		void write_adsr_zone_entry(AdsrZoneEntry& native, const asset_data::adsr_zone_entry& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.id = data.id;
			native.weaponName = plain::write_string(memory, data.weapon_name);
			native.adsrIdx = data.adsr_idx;
			native.weaponIdx = data.weapon_idx;
		}

		std::size_t size_adsr_zone_entry([[maybe_unused]] const asset_data::adsr_zone_entry& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			size += plain::string_size(data.weapon_name);
			return size;
		}

		asset_data::occlude_def read_occlude_def(const OccludeDef& native)
		{
			asset_data::occlude_def data;
			data.id = native.id;
			data.ent_channel_idx = native.entChannelIdx;
			data.cutoff_level1 = native.cutoffLevel1;
			data.cutoff_level2 = native.cutoffLevel2;
			data.cutoff_level3 = native.cutoffLevel3;
			data.cutoff_level4 = native.cutoffLevel4;
			data.attn_level1 = native.attnLevel1;
			data.attn_level2 = native.attnLevel2;
			data.attn_level3 = native.attnLevel3;
			data.attn_level4 = native.attnLevel4;
			return data;
		}

		void write_occlude_def(OccludeDef& native, const asset_data::occlude_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.id = data.id;
			native.entChannelIdx = data.ent_channel_idx;
			native.cutoffLevel1 = data.cutoff_level1;
			native.cutoffLevel2 = data.cutoff_level2;
			native.cutoffLevel3 = data.cutoff_level3;
			native.cutoffLevel4 = data.cutoff_level4;
			native.attnLevel1 = data.attn_level1;
			native.attnLevel2 = data.attn_level2;
			native.attnLevel3 = data.attn_level3;
			native.attnLevel4 = data.attn_level4;
		}

		std::size_t size_occlude_def([[maybe_unused]] const asset_data::occlude_def& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::reverb_def read_reverb_def(const ReverbDef& native)
		{
			asset_data::reverb_def data;
			data.id = native.id;
			data.room_type_id = native.roomTypeId;
			data.room_type = plain::read_text(native.roomType, "ReverbDef.roomType");
			data.dry_level = native.dryLevel;
			data.wet_level = native.wetLevel;
			data.fade_time = native.fadeTime;
			return data;
		}

		void write_reverb_def(ReverbDef& native, const asset_data::reverb_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.id = data.id;
			native.roomTypeId = data.room_type_id;
			plain::write_text(native.roomType, data.room_type, "ReverbDef.roomType");
			native.dryLevel = data.dry_level;
			native.wetLevel = data.wet_level;
			native.fadeTime = data.fade_time;
		}

		std::size_t size_reverb_def([[maybe_unused]] const asset_data::reverb_def& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::timescale_entry read_timescale_entry(const TimescaleEntry& native)
		{
			asset_data::timescale_entry data;
			data.id = native.id;
			data.preset_name = native.presetName;
			data.ent_channel_idx = native.entChannelIdx;
			data.scale = native.scale;
			return data;
		}

		void write_timescale_entry(TimescaleEntry& native, const asset_data::timescale_entry& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.id = data.id;
			native.presetName = data.preset_name;
			native.entChannelIdx = data.ent_channel_idx;
			native.scale = data.scale;
		}

		std::size_t size_timescale_entry([[maybe_unused]] const asset_data::timescale_entry& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::zone_def read_zone_def(const ZoneDef& native)
		{
			asset_data::zone_def data;
			data.id = native.id;
			data.state_id = native.stateId;
			data.reverb_index = native.reverbIndex;
			data.start_occlusion_index = native.startOcclusionIndex;
			data.num_occlusion = native.numOcclusion;
			data.start_filter_index = native.startFilterIndex;
			data.num_filter = native.numFilter;
			data.start_mix_index = native.startMixIndex;
			data.num_mix = native.numMix;
			data.ambient_stream = plain::read_string(native.ambientStream);
			data.start_adsr_zone_index = native.startAdsrZoneIndex;
			data.num_adsr_zone = native.numAdsrZone;
			data.start_npc_adsr_zone_index = native.startNPCAdsrZoneIndex;
			data.num_npc_adsr_zone = native.numNPCAdsrZone;
			data.ambient_def_index = native.ambientDefIndex;
			data.start_full_occ_index = native.startFullOccIndex;
			data.num_disable_full_occ = native.numDisableFullOcc;
			data.start_player_breath_state_index = native.startPlayerBreathStateIndex;
			data.num_player_breath_states = native.numPlayerBreathStates;
			data.weapon_reflection_id = native.weapReflId;
			data.exterior = native.exterior;
			data.zone_name = plain::read_string(native.zoneName);
			data.state_name = plain::read_string(native.stateName);
			data.ambient_name = plain::read_string(native.ambientName);
			data.mix_name = plain::read_string(native.mixName);
			data.reverb_name = plain::read_string(native.reverbName);
			data.filter_name = plain::read_string(native.filterName);
			data.occlusion_name = plain::read_string(native.occlusionName);
			data.player_adsr_name = plain::read_string(native.plrAdsrName);
			data.npc_adsr_name = plain::read_string(native.npcAdsrName);
			data.weapon_reflection_name = plain::read_string(native.weapReflName);
			data.full_occ_name = plain::read_string(native.fullOccName);
			data.player_breath_state_name = plain::read_string(native.playerBreathStateName);
			data.context_type1 = native.contextType1;
			data.context_value1 = native.contextValue1;
			data.context_type2 = native.contextType2;
			data.context_value2 = native.contextValue2;
			data.context_type3 = native.contextType3;
			data.context_value3 = native.contextValue3;
			data.context_type4 = native.contextType4;
			data.context_value4 = native.contextValue4;
			data.duck = native.duck;
			return data;
		}

		void write_zone_def(ZoneDef& native, const asset_data::zone_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.id = data.id;
			native.stateId = data.state_id;
			native.reverbIndex = data.reverb_index;
			native.startOcclusionIndex = data.start_occlusion_index;
			native.numOcclusion = data.num_occlusion;
			native.startFilterIndex = data.start_filter_index;
			native.numFilter = data.num_filter;
			native.startMixIndex = data.start_mix_index;
			native.numMix = data.num_mix;
			native.ambientStream = plain::write_string(memory, data.ambient_stream);
			native.startAdsrZoneIndex = data.start_adsr_zone_index;
			native.numAdsrZone = data.num_adsr_zone;
			native.startNPCAdsrZoneIndex = data.start_npc_adsr_zone_index;
			native.numNPCAdsrZone = data.num_npc_adsr_zone;
			native.ambientDefIndex = data.ambient_def_index;
			native.startFullOccIndex = data.start_full_occ_index;
			native.numDisableFullOcc = data.num_disable_full_occ;
			native.startPlayerBreathStateIndex = data.start_player_breath_state_index;
			native.numPlayerBreathStates = data.num_player_breath_states;
			native.weapReflId = data.weapon_reflection_id;
			native.exterior = data.exterior;
			native.zoneName = plain::write_string(memory, data.zone_name);
			native.stateName = plain::write_string(memory, data.state_name);
			native.ambientName = plain::write_string(memory, data.ambient_name);
			native.mixName = plain::write_string(memory, data.mix_name);
			native.reverbName = plain::write_string(memory, data.reverb_name);
			native.filterName = plain::write_string(memory, data.filter_name);
			native.occlusionName = plain::write_string(memory, data.occlusion_name);
			native.plrAdsrName = plain::write_string(memory, data.player_adsr_name);
			native.npcAdsrName = plain::write_string(memory, data.npc_adsr_name);
			native.weapReflName = plain::write_string(memory, data.weapon_reflection_name);
			native.fullOccName = plain::write_string(memory, data.full_occ_name);
			native.playerBreathStateName = plain::write_string(memory, data.player_breath_state_name);
			native.contextType1 = data.context_type1;
			native.contextValue1 = data.context_value1;
			native.contextType2 = data.context_type2;
			native.contextValue2 = data.context_value2;
			native.contextType3 = data.context_type3;
			native.contextValue3 = data.context_value3;
			native.contextType4 = data.context_type4;
			native.contextValue4 = data.context_value4;
			native.duck = data.duck;
		}

		std::size_t size_zone_def([[maybe_unused]] const asset_data::zone_def& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.ambient_stream);
			size += plain::string_size(data.zone_name);
			size += plain::string_size(data.state_name);
			size += plain::string_size(data.ambient_name);
			size += plain::string_size(data.mix_name);
			size += plain::string_size(data.reverb_name);
			size += plain::string_size(data.filter_name);
			size += plain::string_size(data.occlusion_name);
			size += plain::string_size(data.player_adsr_name);
			size += plain::string_size(data.npc_adsr_name);
			size += plain::string_size(data.weapon_reflection_name);
			size += plain::string_size(data.full_occ_name);
			size += plain::string_size(data.player_breath_state_name);
			return size;
		}

		asset_data::full_occlusion_def read_full_occlusion_def(const FullOcclusionDef& native)
		{
			asset_data::full_occlusion_def data;
			data.id = native.id;
			data.preset_name = native.presetName;
			data.ent_channel_idx = native.entChannelIdx;
			return data;
		}

		void write_full_occlusion_def(FullOcclusionDef& native, const asset_data::full_occlusion_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.id = data.id;
			native.presetName = data.preset_name;
			native.entChannelIdx = data.ent_channel_idx;
		}

		std::size_t size_full_occlusion_def([[maybe_unused]] const asset_data::full_occlusion_def& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::player_breath_state_def read_player_breath_state_def(const PlayerBreathStateDef& native)
		{
			asset_data::player_breath_state_def data;
			data.state_type = native.stateType;
			data.inhale_alias = plain::read_string(native.inhaleAlias);
			data.exhale_alias = plain::read_string(native.exhaleAlias);
			data.min_breaths = native.minBreaths;
			return data;
		}

		void write_player_breath_state_def(PlayerBreathStateDef& native, const asset_data::player_breath_state_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.stateType = data.state_type;
			native.inhaleAlias = plain::write_string(memory, data.inhale_alias);
			native.exhaleAlias = plain::write_string(memory, data.exhale_alias);
			native.minBreaths = data.min_breaths;
		}

		std::size_t size_player_breath_state_def([[maybe_unused]] const asset_data::player_breath_state_def& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.inhale_alias);
			size += plain::string_size(data.exhale_alias);
			return size;
		}

		asset_data::sound_table read_sound_table(const SoundTable& native)
		{
			asset_data::sound_table data;
			data.id = native.id;
			data.adsr_settings = plain::read_array(native.adsrSettings, native.adsrCount, "SoundTable.adsrSettings", [&](const auto& item) { return read_adsr_setting(item); });
			data.ambient_defs = plain::read_array(native.ambientDefs, native.ambientDefCount, "SoundTable.ambientDefs", [&](const auto& item) { return read_ambient_def(item); });
			data.ambient_events = plain::read_array(native.ambientEvents, native.ambientEventCount, "SoundTable.ambientEvents", [&](const auto& item) { return read_ambient_event(item); });
			data.ambient_elements = plain::read_array(native.ambientElements, native.ambientElementsCount, "SoundTable.ambientElements", [&](const auto& item) { return read_ambient_element(item); });
			data.filters = plain::read_array(native.filters, native.filterCount, "SoundTable.filters", [&](const auto& item) { return read_filter_def(item); });
			data.mixes = plain::read_array(native.mixes, native.mixCount, "SoundTable.mixes", [&](const auto& item) { return read_mix_def(item); });
			data.npc_adsr_zones = plain::read_array(native.npcADSRZones, native.npcADSRZoneCount, "SoundTable.npcADSRZones", [&](const auto& item) { return read_adsr_zone_entry(item); });
			data.occlusion_filters = plain::read_array(native.occlusionFilters, native.occlusionFilterCount, "SoundTable.occlusionFilters", [&](const auto& item) { return read_occlude_def(item); });
			data.player_adsr_zones = plain::read_array(native.playerADSRZones, native.playerADSRZoneCount, "SoundTable.playerADSRZones", [&](const auto& item) { return read_adsr_zone_entry(item); });
			data.reverbs = plain::read_array(native.reverbs, native.reverbCount, "SoundTable.reverbs", [&](const auto& item) { return read_reverb_def(item); });
			data.time_scale_settings = plain::read_array(native.timeScaleSettings, native.timeScaleSettingCount, "SoundTable.timeScaleSettings", [&](const auto& item) { return read_timescale_entry(item); });
			data.zones = plain::read_array(native.zones, native.zoneCount, "SoundTable.zones", [&](const auto& item) { return read_zone_def(item); });
			data.full_occlusion_defs = plain::read_array(native.fullOcclusionDefs, native.fullOcclusionDefCount, "SoundTable.fullOcclusionDefs", [&](const auto& item) { return read_full_occlusion_def(item); });
			data.player_breath_state_defs = plain::read_array(native.plrBreathStateDefs, native.plrBreathStateDefCount, "SoundTable.plrBreathStateDefs", [&](const auto& item) { return read_player_breath_state_def(item); });
			return data;
		}

		void write_sound_table(SoundTable& native, const asset_data::sound_table& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.id = data.id;
			native.adsrSettings = plain::write_array<ADSRSetting>(data.adsr_settings, memory, [&](auto& out, const auto& in) { write_adsr_setting(out, in, resolve, memory); });
			native.ambientDefs = plain::write_array<AmbientDef>(data.ambient_defs, memory, [&](auto& out, const auto& in) { write_ambient_def(out, in, resolve, memory); });
			native.ambientEvents = plain::write_array<AmbientEvent>(data.ambient_events, memory, [&](auto& out, const auto& in) { write_ambient_event(out, in, resolve, memory); });
			native.ambientElements = plain::write_array<AmbientElement>(data.ambient_elements, memory, [&](auto& out, const auto& in) { write_ambient_element(out, in, resolve, memory); });
			native.filters = plain::write_array<FilterDef>(data.filters, memory, [&](auto& out, const auto& in) { write_filter_def(out, in, resolve, memory); });
			native.mixes = plain::write_array<MixDef>(data.mixes, memory, [&](auto& out, const auto& in) { write_mix_def(out, in, resolve, memory); });
			native.npcADSRZones = plain::write_array<AdsrZoneEntry>(data.npc_adsr_zones, memory, [&](auto& out, const auto& in) { write_adsr_zone_entry(out, in, resolve, memory); });
			native.occlusionFilters = plain::write_array<OccludeDef>(data.occlusion_filters, memory, [&](auto& out, const auto& in) { write_occlude_def(out, in, resolve, memory); });
			native.playerADSRZones = plain::write_array<AdsrZoneEntry>(data.player_adsr_zones, memory, [&](auto& out, const auto& in) { write_adsr_zone_entry(out, in, resolve, memory); });
			native.reverbs = plain::write_array<ReverbDef>(data.reverbs, memory, [&](auto& out, const auto& in) { write_reverb_def(out, in, resolve, memory); });
			native.timeScaleSettings = plain::write_array<TimescaleEntry>(data.time_scale_settings, memory, [&](auto& out, const auto& in) { write_timescale_entry(out, in, resolve, memory); });
			native.zones = plain::write_array<ZoneDef>(data.zones, memory, [&](auto& out, const auto& in) { write_zone_def(out, in, resolve, memory); });
			native.fullOcclusionDefs = plain::write_array<FullOcclusionDef>(data.full_occlusion_defs, memory, [&](auto& out, const auto& in) { write_full_occlusion_def(out, in, resolve, memory); });
			native.plrBreathStateDefs = plain::write_array<PlayerBreathStateDef>(data.player_breath_state_defs, memory, [&](auto& out, const auto& in) { write_player_breath_state_def(out, in, resolve, memory); });
			native.adsrCount = plain::count_of<std::uint32_t>(data.adsr_settings);
			native.ambientDefCount = plain::count_of<std::uint32_t>(data.ambient_defs);
			native.ambientEventCount = plain::count_of<std::uint32_t>(data.ambient_events);
			native.ambientElementsCount = plain::count_of<std::uint32_t>(data.ambient_elements);
			native.filterCount = plain::count_of<std::uint32_t>(data.filters);
			native.mixCount = plain::count_of<std::uint32_t>(data.mixes);
			native.npcADSRZoneCount = plain::count_of<std::uint32_t>(data.npc_adsr_zones);
			native.occlusionFilterCount = plain::count_of<std::uint32_t>(data.occlusion_filters);
			native.playerADSRZoneCount = plain::count_of<std::uint32_t>(data.player_adsr_zones);
			native.reverbCount = plain::count_of<std::uint32_t>(data.reverbs);
			native.timeScaleSettingCount = plain::count_of<std::uint32_t>(data.time_scale_settings);
			native.zoneCount = plain::count_of<std::uint32_t>(data.zones);
			native.fullOcclusionDefCount = plain::count_of<std::uint32_t>(data.full_occlusion_defs);
			native.plrBreathStateDefCount = plain::count_of<std::uint32_t>(data.player_breath_state_defs);
		}

		std::size_t size_sound_table([[maybe_unused]] const asset_data::sound_table& data)
		{
			std::size_t size = 0;
			if (data.adsr_settings)
			{
				size += plain::rounded(std::max<std::size_t>(data.adsr_settings->size(), 1) * sizeof(ADSRSetting));
				for (const auto& item : *data.adsr_settings)
				{
					size += size_adsr_setting(item);
				}
			}
			if (data.ambient_defs)
			{
				size += plain::rounded(std::max<std::size_t>(data.ambient_defs->size(), 1) * sizeof(AmbientDef));
				for (const auto& item : *data.ambient_defs)
				{
					size += size_ambient_def(item);
				}
			}
			if (data.ambient_events)
			{
				size += plain::rounded(std::max<std::size_t>(data.ambient_events->size(), 1) * sizeof(AmbientEvent));
				for (const auto& item : *data.ambient_events)
				{
					size += size_ambient_event(item);
				}
			}
			if (data.ambient_elements)
			{
				size += plain::rounded(std::max<std::size_t>(data.ambient_elements->size(), 1) * sizeof(AmbientElement));
				for (const auto& item : *data.ambient_elements)
				{
					size += size_ambient_element(item);
				}
			}
			if (data.filters)
			{
				size += plain::rounded(std::max<std::size_t>(data.filters->size(), 1) * sizeof(FilterDef));
				for (const auto& item : *data.filters)
				{
					size += size_filter_def(item);
				}
			}
			if (data.mixes)
			{
				size += plain::rounded(std::max<std::size_t>(data.mixes->size(), 1) * sizeof(MixDef));
				for (const auto& item : *data.mixes)
				{
					size += size_mix_def(item);
				}
			}
			if (data.npc_adsr_zones)
			{
				size += plain::rounded(std::max<std::size_t>(data.npc_adsr_zones->size(), 1) * sizeof(AdsrZoneEntry));
				for (const auto& item : *data.npc_adsr_zones)
				{
					size += size_adsr_zone_entry(item);
				}
			}
			if (data.occlusion_filters)
			{
				size += plain::rounded(std::max<std::size_t>(data.occlusion_filters->size(), 1) * sizeof(OccludeDef));
				for (const auto& item : *data.occlusion_filters)
				{
					size += size_occlude_def(item);
				}
			}
			if (data.player_adsr_zones)
			{
				size += plain::rounded(std::max<std::size_t>(data.player_adsr_zones->size(), 1) * sizeof(AdsrZoneEntry));
				for (const auto& item : *data.player_adsr_zones)
				{
					size += size_adsr_zone_entry(item);
				}
			}
			if (data.reverbs)
			{
				size += plain::rounded(std::max<std::size_t>(data.reverbs->size(), 1) * sizeof(ReverbDef));
				for (const auto& item : *data.reverbs)
				{
					size += size_reverb_def(item);
				}
			}
			if (data.time_scale_settings)
			{
				size += plain::rounded(std::max<std::size_t>(data.time_scale_settings->size(), 1) * sizeof(TimescaleEntry));
				for (const auto& item : *data.time_scale_settings)
				{
					size += size_timescale_entry(item);
				}
			}
			if (data.zones)
			{
				size += plain::rounded(std::max<std::size_t>(data.zones->size(), 1) * sizeof(ZoneDef));
				for (const auto& item : *data.zones)
				{
					size += size_zone_def(item);
				}
			}
			if (data.full_occlusion_defs)
			{
				size += plain::rounded(std::max<std::size_t>(data.full_occlusion_defs->size(), 1) * sizeof(FullOcclusionDef));
				for (const auto& item : *data.full_occlusion_defs)
				{
					size += size_full_occlusion_def(item);
				}
			}
			if (data.player_breath_state_defs)
			{
				size += plain::rounded(std::max<std::size_t>(data.player_breath_state_defs->size(), 1) * sizeof(PlayerBreathStateDef));
				for (const auto& item : *data.player_breath_state_defs)
				{
					size += size_player_breath_state_def(item);
				}
			}
			return size;
		}

		asset_data::sound_duck read_sound_duck(const SndDuck& native)
		{
			asset_data::sound_duck data;
			data.name = plain::read_text(native.name, "SndDuck.name");
			data.id = native.id;
			data.fade_in = native.fadeIn;
			data.fade_out = native.fadeOut;
			data.start_delay = native.startDelay;
			data.min_distance = native.minDistance;
			data.distance = native.distance;
			data.length = native.length;
			data.fade_in_curve = native.fadeInCurve;
			data.fade_out_curve = native.fadeOutCurve;
			data.attenuation = plain::read_fixed<float, 128>(native.attenuation);
			data.hpf = plain::read_fixed<float, 128>(native.hpf);
			data.lpf = plain::read_fixed<float, 128>(native.lpf);
			data.focus_amount = plain::read_fixed<float, 128>(native.focusAmount);
			data.focus_cone = plain::read_fixed<std::uint32_t, 128>(native.focusCone);
			data.update_while_paused = native.updateWhilePaused;
			data.track_amplitude = native.trackAmplitude;
			data.disable_in_splitscreen = native.disableInSplitscreen;
			data.alias_attenuation = native.aliasAttenuation;
			data.alias_hpf = native.aliasHpf;
			data.alias_lpf = native.aliasLpf;
			data.duck_alias = native.duckAlias;
			return data;
		}

		void write_sound_duck(SndDuck& native, const asset_data::sound_duck& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_text(native.name, data.name, "SndDuck.name");
			native.id = data.id;
			native.fadeIn = data.fade_in;
			native.fadeOut = data.fade_out;
			native.startDelay = data.start_delay;
			native.minDistance = data.min_distance;
			native.distance = data.distance;
			native.length = data.length;
			native.fadeInCurve = data.fade_in_curve;
			native.fadeOutCurve = data.fade_out_curve;
			if (data.attenuation)
			{
				native.attenuation = memory.manual_allocate<float>(plain::rounded(128 * sizeof(float)));
				for (std::size_t i = 0; i < 128; ++i)
				{
					native.attenuation[i] = (*data.attenuation)[i];
				}
			}
			if (data.hpf)
			{
				native.hpf = memory.manual_allocate<float>(plain::rounded(128 * sizeof(float)));
				for (std::size_t i = 0; i < 128; ++i)
				{
					native.hpf[i] = (*data.hpf)[i];
				}
			}
			if (data.lpf)
			{
				native.lpf = memory.manual_allocate<float>(plain::rounded(128 * sizeof(float)));
				for (std::size_t i = 0; i < 128; ++i)
				{
					native.lpf[i] = (*data.lpf)[i];
				}
			}
			if (data.focus_amount)
			{
				native.focusAmount = memory.manual_allocate<float>(plain::rounded(128 * sizeof(float)));
				for (std::size_t i = 0; i < 128; ++i)
				{
					native.focusAmount[i] = (*data.focus_amount)[i];
				}
			}
			if (data.focus_cone)
			{
				native.focusCone = memory.manual_allocate<std::uint32_t>(plain::rounded(128 * sizeof(std::uint32_t)));
				for (std::size_t i = 0; i < 128; ++i)
				{
					native.focusCone[i] = (*data.focus_cone)[i];
				}
			}
			native.updateWhilePaused = data.update_while_paused;
			native.trackAmplitude = data.track_amplitude;
			native.disableInSplitscreen = data.disable_in_splitscreen;
			native.aliasAttenuation = data.alias_attenuation;
			native.aliasHpf = data.alias_hpf;
			native.aliasLpf = data.alias_lpf;
			native.duckAlias = data.duck_alias;
		}

		std::size_t size_sound_duck([[maybe_unused]] const asset_data::sound_duck& data)
		{
			std::size_t size = 0;
			size += data.attenuation ? plain::rounded(128 * sizeof(float)) : 0;
			size += data.hpf ? plain::rounded(128 * sizeof(float)) : 0;
			size += data.lpf ? plain::rounded(128 * sizeof(float)) : 0;
			size += data.focus_amount ? plain::rounded(128 * sizeof(float)) : 0;
			size += data.focus_cone ? plain::rounded(128 * sizeof(std::uint32_t)) : 0;
			return size;
		}

		asset_data::sound_music_asset read_sound_music_asset(const SndMusicAsset& native)
		{
			asset_data::sound_music_asset data;
			data.alias = plain::read_text(native.alias, "SndMusicAsset.alias");
			data.alias_id = native.aliasId;
			data.inactive = native.inactive;
			data.complete_loop = native.completeLoop;
			data.remove_after_play = native.removeAfterPlay;
			data.first_random = native.firstRandom;
			data.start_sync = native.startSync;
			data.stop_sync = native.stopSync;
			data.complete_on_stop = native.completeOnStop;
			data.loop_start_offset = native.loopStartOffset;
			data.bpm = native.bpm;
			data.asset_type = native.assetType;
			data.loop_number = native.loopNumber;
			data.sample_count = native.sampleCount;
			data.start_delay_beats = native.startDelayBeats;
			data.start_fade_beats = native.startFadeBeats;
			data.stop_delay_beats = native.stopDelayBeats;
			data.stop_fade_beats = native.stopFadeBeats;
			data.start_offset_frames = native.startOffsetFrames;
			data.meter = native.meter;
			return data;
		}

		void write_sound_music_asset(SndMusicAsset& native, const asset_data::sound_music_asset& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_text(native.alias, data.alias, "SndMusicAsset.alias");
			native.aliasId = data.alias_id;
			native.inactive = data.inactive;
			native.completeLoop = data.complete_loop;
			native.removeAfterPlay = data.remove_after_play;
			native.firstRandom = data.first_random;
			native.startSync = data.start_sync;
			native.stopSync = data.stop_sync;
			native.completeOnStop = data.complete_on_stop;
			native.loopStartOffset = data.loop_start_offset;
			native.bpm = data.bpm;
			native.assetType = data.asset_type;
			native.loopNumber = data.loop_number;
			native.sampleCount = data.sample_count;
			native.startDelayBeats = data.start_delay_beats;
			native.startFadeBeats = data.start_fade_beats;
			native.stopDelayBeats = data.stop_delay_beats;
			native.stopFadeBeats = data.stop_fade_beats;
			native.startOffsetFrames = data.start_offset_frames;
			native.meter = data.meter;
		}

		std::size_t size_sound_music_asset([[maybe_unused]] const asset_data::sound_music_asset& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::sound_music_state read_sound_music_state(const SndMusicState& native)
		{
			asset_data::sound_music_state data;
			data.name = plain::read_text(native.name, "SndMusicState.name");
			data.id = native.id;
			data.intro = read_sound_music_asset(native.intro);
			data.exit = read_sound_music_asset(native.exit);
			data.loops = plain::read_array(native.loops, native.loopCount, "SndMusicState.loops", [&](const auto& item) { return read_sound_music_asset(item); });
			data.order = native.order;
			data.interrupt_priority = native.interruptPriority;
			data.persist_priority = native.persistPriority;
			data.is_random = native.isRandom;
			data.is_sequential = native.isSequential;
			data.skip_previous_exit = native.skipPreviousExit;
			data.wall_clock_sync = native.wallClockSync;
			return data;
		}

		void write_sound_music_state(SndMusicState& native, const asset_data::sound_music_state& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_text(native.name, data.name, "SndMusicState.name");
			native.id = data.id;
			write_sound_music_asset(native.intro, data.intro, resolve, memory);
			write_sound_music_asset(native.exit, data.exit, resolve, memory);
			native.loops = plain::write_array<SndMusicAsset>(data.loops, memory, [&](auto& out, const auto& in) { write_sound_music_asset(out, in, resolve, memory); });
			native.order = data.order;
			native.interruptPriority = data.interrupt_priority;
			native.persistPriority = data.persist_priority;
			native.isRandom = data.is_random;
			native.isSequential = data.is_sequential;
			native.skipPreviousExit = data.skip_previous_exit;
			native.wallClockSync = data.wall_clock_sync;
			native.loopCount = plain::count_of<std::uint32_t>(data.loops);
		}

		std::size_t size_sound_music_state([[maybe_unused]] const asset_data::sound_music_state& data)
		{
			std::size_t size = 0;
			size += size_sound_music_asset(data.intro);
			size += size_sound_music_asset(data.exit);
			if (data.loops)
			{
				size += plain::rounded(std::max<std::size_t>(data.loops->size(), 1) * sizeof(SndMusicAsset));
				for (const auto& item : *data.loops)
				{
					size += size_sound_music_asset(item);
				}
			}
			return size;
		}

		asset_data::sound_music_set read_sound_music_set(const SndMusicSet& native)
		{
			asset_data::sound_music_set data;
			data.name = plain::read_text(native.name, "SndMusicSet.name");
			data.id = native.id;
			data.states = plain::read_array(native.states, native.stateCount, "SndMusicSet.states", [&](const auto& item) { return read_sound_music_state(item); });
			return data;
		}

		void write_sound_music_set(SndMusicSet& native, const asset_data::sound_music_set& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_text(native.name, data.name, "SndMusicSet.name");
			native.id = data.id;
			native.states = plain::write_array<SndMusicState>(data.states, memory, [&](auto& out, const auto& in) { write_sound_music_state(out, in, resolve, memory); });
			native.stateCount = plain::count_of<std::uint32_t>(data.states);
		}

		std::size_t size_sound_music_set([[maybe_unused]] const asset_data::sound_music_set& data)
		{
			std::size_t size = 0;
			if (data.states)
			{
				size += plain::rounded(std::max<std::size_t>(data.states->size(), 1) * sizeof(SndMusicState));
				for (const auto& item : *data.states)
				{
					size += size_sound_music_state(item);
				}
			}
			return size;
		}

		asset_data::weapon_reflection_def read_weapon_reflection_def(const WeaponReflectionDef& native)
		{
			asset_data::weapon_reflection_def data;
			data.name = plain::read_string(native.name);
			data.id = native.id;
			data.weapon_class = plain::read_string(native.weaponClass);
			data.weapon_class_id = native.weaponClassId;
			data.type = plain::read_string(native.type);
			data.zone_type = plain::enum_name(asset_data::weapon_reflection_room_type_names, static_cast<std::int64_t>(native.zoneType), "WeaponReflectionDef.zoneType");
			data.zone_type_scalar = native.zoneTypeScalar;
			data.close_med_threshold = native.closeMedThreshold;
			data.med_far_threshold = native.medFarThreshold;
			data.far_threshold = native.farThreshold;
			data.npc_close_med_threshold = native.npcCloseMedThreshold;
			data.npc_med_far_threshold = native.npcMedFarThreshold;
			data.npc_far_threshold = native.npcFarThreshold;
			data.speed_of_sound = native.speedOfSound;
			data.max_pitch_close = native.maxPitchClose;
			data.pitch_dist_close = native.pitchDistClose;
			data.max_pitch_med = native.maxPitchMed;
			data.pitch_dist_med = native.pitchDistMed;
			data.max_pitch_far = native.maxPitchFar;
			data.pitch_dist_far = native.pitchDistFar;
			data.npc_max_pitch_med = native.npcMaxPitchMed;
			data.npc_pitch_dist_med = native.npcPitchDistMed;
			data.npc_max_pitch_far = native.npcMaxPitchFar;
			data.npc_pitch_dist_far = native.npcPitchDistFar;
			data.front_vol = native.frontVol;
			data.front_eq = native.frontEq;
			data.side_vol = native.sideVol;
			data.side_eq = native.sideEq;
			data.rear_vol = native.rearVol;
			data.rear_eq = native.rearEq;
			return data;
		}

		void write_weapon_reflection_def(WeaponReflectionDef& native, const asset_data::weapon_reflection_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.id = data.id;
			native.weaponClass = plain::write_string(memory, data.weapon_class);
			native.weaponClassId = data.weapon_class_id;
			native.type = plain::write_string(memory, data.type);
			native.zoneType = static_cast<WeapReflRoomType>(plain::enum_value(asset_data::weapon_reflection_room_type_names, data.zone_type, "WeaponReflectionDef.zoneType"));
			native.zoneTypeScalar = data.zone_type_scalar;
			native.closeMedThreshold = data.close_med_threshold;
			native.medFarThreshold = data.med_far_threshold;
			native.farThreshold = data.far_threshold;
			native.npcCloseMedThreshold = data.npc_close_med_threshold;
			native.npcMedFarThreshold = data.npc_med_far_threshold;
			native.npcFarThreshold = data.npc_far_threshold;
			native.speedOfSound = data.speed_of_sound;
			native.maxPitchClose = data.max_pitch_close;
			native.pitchDistClose = data.pitch_dist_close;
			native.maxPitchMed = data.max_pitch_med;
			native.pitchDistMed = data.pitch_dist_med;
			native.maxPitchFar = data.max_pitch_far;
			native.pitchDistFar = data.pitch_dist_far;
			native.npcMaxPitchMed = data.npc_max_pitch_med;
			native.npcPitchDistMed = data.npc_pitch_dist_med;
			native.npcMaxPitchFar = data.npc_max_pitch_far;
			native.npcPitchDistFar = data.npc_pitch_dist_far;
			native.frontVol = data.front_vol;
			native.frontEq = data.front_eq;
			native.sideVol = data.side_vol;
			native.sideEq = data.side_eq;
			native.rearVol = data.rear_vol;
			native.rearEq = data.rear_eq;
		}

		std::size_t size_weapon_reflection_def([[maybe_unused]] const asset_data::weapon_reflection_def& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			size += plain::string_size(data.weapon_class);
			size += plain::string_size(data.type);
			return size;
		}

		asset_data::doppler_preset read_doppler_preset(const DopplerPreset& native)
		{
			asset_data::doppler_preset data;
			data.name = plain::read_text(native.name, "DopplerPreset.name");
			data.id = native.id;
			data.speed_of_sound = native.speedOfSound;
			data.player_velocity_scale = native.playerVelocityScale;
			data.min_pitch = native.minPitch;
			data.max_pitch = native.maxPitch;
			data.smoothing = native.smoothing;
			return data;
		}

		void write_doppler_preset(DopplerPreset& native, const asset_data::doppler_preset& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_text(native.name, data.name, "DopplerPreset.name");
			native.id = data.id;
			native.speedOfSound = data.speed_of_sound;
			native.playerVelocityScale = data.player_velocity_scale;
			native.minPitch = data.min_pitch;
			native.maxPitch = data.max_pitch;
			native.smoothing = data.smoothing;
		}

		std::size_t size_doppler_preset([[maybe_unused]] const asset_data::doppler_preset& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::occlusion_shape read_occlusion_shape(const OcclusionShape& native)
		{
			asset_data::occlusion_shape data;
			data.name = plain::read_text(native.name, "OcclusionShape.name");
			data.id = native.id;
			data.inner_angle_volume = native.innerAngleVolume;
			data.outer_angle_volume = native.outerAngleVolume;
			data.occluded_volume = native.occludedVolume;
			data.inner_angle_lpf = native.innerAngleLPF;
			data.outer_angle_lpf = native.outerAngleLPF;
			data.occluded_lpf = native.occludedLPF;
			data.yaw_offset_angle = native.yawOffsetAngle;
			data.pitch_offset_angle = native.pitchOffsetAngle;
			return data;
		}

		void write_occlusion_shape(OcclusionShape& native, const asset_data::occlusion_shape& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_text(native.name, data.name, "OcclusionShape.name");
			native.id = data.id;
			native.innerAngleVolume = data.inner_angle_volume;
			native.outerAngleVolume = data.outer_angle_volume;
			native.occludedVolume = data.occluded_volume;
			native.innerAngleLPF = data.inner_angle_lpf;
			native.outerAngleLPF = data.outer_angle_lpf;
			native.occludedLPF = data.occluded_lpf;
			native.yawOffsetAngle = data.yaw_offset_angle;
			native.pitchOffsetAngle = data.pitch_offset_angle;
		}

		std::size_t size_occlusion_shape([[maybe_unused]] const asset_data::occlusion_shape& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::focus_cone read_focus_cone(const FocusCone& native)
		{
			asset_data::focus_cone data;
			data.name = plain::read_text(native.name, "FocusCone.name");
			data.id = native.id;
			data.inner_angle = native.innerAngle;
			data.outer_angle = native.outerAngle;
			data.inner_dist = native.innerDist;
			data.outer_dist = native.outerDist;
			data.attenuation = native.attenuation;
			return data;
		}

		void write_focus_cone(FocusCone& native, const asset_data::focus_cone& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_text(native.name, data.name, "FocusCone.name");
			native.id = data.id;
			native.innerAngle = data.inner_angle;
			native.outerAngle = data.outer_angle;
			native.innerDist = data.inner_dist;
			native.outerDist = data.outer_dist;
			native.attenuation = data.attenuation;
		}

		std::size_t size_focus_cone([[maybe_unused]] const asset_data::focus_cone& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::sound_curve read_sound_curve(const SndCurve& native)
		{
			asset_data::sound_curve data;
			data.name = plain::read_text(native.name, "SndCurve.name");
			data.id = native.id;
			data.num_points = native.numPoints;
			data.points = [&] { std::array<std::array<float, 2>, 16> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = plain::read_scalars<float, 2>(&native.points[i]); } return out; }();
			return data;
		}

		void write_sound_curve(SndCurve& native, const asset_data::sound_curve& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_text(native.name, data.name, "SndCurve.name");
			native.id = data.id;
			native.numPoints = data.num_points;
			for (std::size_t i = 0; i < data.points.size(); ++i)
			{
				plain::write_scalars(&native.points[i], data.points[i]);
			}
		}

		std::size_t size_sound_curve([[maybe_unused]] const asset_data::sound_curve& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::speaker_map read_speaker_map(const SpeakerMap& native)
		{
			asset_data::speaker_map data;
			data.name = plain::read_text(native.name, "SpeakerMap.name");
			data.id = native.id;
			data.mono_to_stereo = std::to_array(native.monoToStereo);
			data.stereo_to_stereo = std::to_array(native.stereoToStereo);
			data.mono_to51 = std::to_array(native.monoTo51);
			data.stereo_to51 = std::to_array(native.stereoTo51);
			data.mono_to71 = std::to_array(native.monoTo71);
			data.stereo_to71 = std::to_array(native.stereoTo71);
			data.mono_to7144 = std::to_array(native.monoTo7144);
			data.stereo_to7144 = std::to_array(native.stereoTo7144);
			return data;
		}

		void write_speaker_map(SpeakerMap& native, const asset_data::speaker_map& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_text(native.name, data.name, "SpeakerMap.name");
			native.id = data.id;
			std::ranges::copy(data.mono_to_stereo, native.monoToStereo);
			std::ranges::copy(data.stereo_to_stereo, native.stereoToStereo);
			std::ranges::copy(data.mono_to51, native.monoTo51);
			std::ranges::copy(data.stereo_to51, native.stereoTo51);
			std::ranges::copy(data.mono_to71, native.monoTo71);
			std::ranges::copy(data.stereo_to71, native.stereoTo71);
			std::ranges::copy(data.mono_to7144, native.monoTo7144);
			std::ranges::copy(data.stereo_to7144, native.stereoTo7144);
		}

		std::size_t size_speaker_map([[maybe_unused]] const asset_data::speaker_map& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::sound_futz read_sound_futz(const SndFutz& native)
		{
			asset_data::sound_futz data;
			data.name = plain::read_text(native.name, "SndFutz.name");
			data.id = native.id;
			data.enable = native.enable;
			data.bpf_f = native.bpfF;
			data.bpf_q = native.bpfQ;
			data.ls_g = native.lsG;
			data.ls_f = native.lsF;
			data.ls_q = native.lsQ;
			data.dist = native.dist;
			data.pre_g = native.preG;
			data.post_g = native.postG;
			data.th = native.th;
			data.tg = native.tg;
			data.clippre = native.clippre;
			data.clippost = native.clippost;
			data.blend = native.blend;
			data.start_alias_id = native.startAliasId;
			data.stop_alias_id = native.stopAliasId;
			data.loop_alias_id = native.loopAliasId;
			return data;
		}

		void write_sound_futz(SndFutz& native, const asset_data::sound_futz& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_text(native.name, data.name, "SndFutz.name");
			native.id = data.id;
			native.enable = data.enable;
			native.bpfF = data.bpf_f;
			native.bpfQ = data.bpf_q;
			native.lsG = data.ls_g;
			native.lsF = data.ls_f;
			native.lsQ = data.ls_q;
			native.dist = data.dist;
			native.preG = data.pre_g;
			native.postG = data.post_g;
			native.th = data.th;
			native.tg = data.tg;
			native.clippre = data.clippre;
			native.clippost = data.clippost;
			native.blend = data.blend;
			native.startAliasId = data.start_alias_id;
			native.stopAliasId = data.stop_alias_id;
			native.loopAliasId = data.loop_alias_id;
		}

		std::size_t size_sound_futz([[maybe_unused]] const asset_data::sound_futz& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::sound_bank_stream_info read_sound_bank_stream_info(const SndBankStreamInfo& native)
		{
			asset_data::sound_bank_stream_info data;
			data.loaded_stream_key = plain::read_asset(ASSET_TYPE_STREAM_KEY, native.loadedStreamKey);
			data.always_loaded = native.alwaysLoaded;
			return data;
		}

		void write_sound_bank_stream_info(SndBankStreamInfo& native, const asset_data::sound_bank_stream_info& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.loadedStreamKey = plain::resolve_asset<StreamKey>(resolve, ASSET_TYPE_STREAM_KEY, data.loaded_stream_key);
			native.alwaysLoaded = data.always_loaded;
		}

		std::size_t size_sound_bank_stream_info([[maybe_unused]] const asset_data::sound_bank_stream_info& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::sound_bank read_sound_bank(const SndBank& native)
		{
			asset_data::sound_bank data;
			data.name = plain::read_string(native.name);
			data.zone = plain::read_string(native.zone);
			data.game_language = plain::read_string(native.gameLanguage);
			data.sound_language = plain::read_string(native.soundLanguage);
			data.alias = plain::read_array(native.alias, native.aliasCount, "SndBank.alias", [&](const auto& item) { return read_sound_alias_list(item); });
			data.alias_index = plain::read_array(native.aliasIndex, native.aliasCount, "SndBank.aliasIndex", [&](const auto& item) { return read_sound_index_entry(item); });
			data.send_effects = plain::read_array(native.sendEffects, native.sendEffectCount, "SndBank.sendEffects", [&](const auto& item) { return read_sound_send_effect_params(item); });
			data.tables = read_sound_table(native.soundTable);
			data.ducks = plain::read_array(native.ducks, native.duckCount, "SndBank.ducks", [&](const auto& item) { return read_sound_duck(item); });
			data.music_sets = plain::read_array(native.musicSets, native.musicSetCount, "SndBank.musicSets", [&](const auto& item) { return read_sound_music_set(item); });
			data.weapon_reflections = plain::read_array(native.weapReflections, native.weapReflectionCount, "SndBank.weapReflections", [&](const auto& item) { return read_weapon_reflection_def(item); });
			data.doppler_presets = plain::read_array(native.dopplerPresets, native.dopplerPresetCount, "SndBank.dopplerPresets", [&](const auto& item) { return read_doppler_preset(item); });
			data.occlusion_shapes = plain::read_array(native.occlusionShapes, native.occlusionShapeCount, "SndBank.occlusionShapes", [&](const auto& item) { return read_occlusion_shape(item); });
			data.focus_cones = plain::read_array(native.focusCones, native.focusConeCount, "SndBank.focusCones", [&](const auto& item) { return read_focus_cone(item); });
			data.volume_falloff_curves = plain::read_array(native.vfCurves, native.vfCurveCount, "SndBank.vfCurves", [&](const auto& item) { return read_sound_curve(item); });
			data.lpf_curves = plain::read_array(native.lpfCurves, native.lpfCurveCount, "SndBank.lpfCurves", [&](const auto& item) { return read_sound_curve(item); });
			data.hpf_curves = plain::read_array(native.hpfCurves, native.hpfCurveCount, "SndBank.hpfCurves", [&](const auto& item) { return read_sound_curve(item); });
			data.reverb_curves = plain::read_array(native.rvbCurves, native.rvbCurveCount, "SndBank.rvbCurves", [&](const auto& item) { return read_sound_curve(item); });
			data.speaker_maps = plain::read_array(native.speakerMaps, native.speakerMapCount, "SndBank.speakerMaps", [&](const auto& item) { return read_speaker_map(item); });
			data.futzs = plain::read_array(native.futzs, native.futzCount, "SndBank.futzs", [&](const auto& item) { return read_sound_futz(item); });
			data.stream_info = read_sound_bank_stream_info(native.streamInfo);
			return data;
		}

		void write_sound_bank(SndBank& native, const asset_data::sound_bank& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.zone = plain::write_string(memory, data.zone);
			native.gameLanguage = plain::write_string(memory, data.game_language);
			native.soundLanguage = plain::write_string(memory, data.sound_language);
			native.alias = plain::write_array<SndAliasList>(data.alias, memory, [&](auto& out, const auto& in) { write_sound_alias_list(out, in, resolve, memory); });
			native.aliasIndex = plain::write_array<SndIndexEntry>(data.alias_index, memory, [&](auto& out, const auto& in) { write_sound_index_entry(out, in, resolve, memory); });
			native.sendEffects = plain::write_array<SndSendEffectParams>(data.send_effects, memory, [&](auto& out, const auto& in) { write_sound_send_effect_params(out, in, resolve, memory); });
			write_sound_table(native.soundTable, data.tables, resolve, memory);
			native.ducks = plain::write_array<SndDuck>(data.ducks, memory, [&](auto& out, const auto& in) { write_sound_duck(out, in, resolve, memory); });
			native.musicSets = plain::write_array<SndMusicSet>(data.music_sets, memory, [&](auto& out, const auto& in) { write_sound_music_set(out, in, resolve, memory); });
			native.weapReflections = plain::write_array<WeaponReflectionDef>(data.weapon_reflections, memory, [&](auto& out, const auto& in) { write_weapon_reflection_def(out, in, resolve, memory); });
			native.dopplerPresets = plain::write_array<DopplerPreset>(data.doppler_presets, memory, [&](auto& out, const auto& in) { write_doppler_preset(out, in, resolve, memory); });
			native.occlusionShapes = plain::write_array<OcclusionShape>(data.occlusion_shapes, memory, [&](auto& out, const auto& in) { write_occlusion_shape(out, in, resolve, memory); });
			native.focusCones = plain::write_array<FocusCone>(data.focus_cones, memory, [&](auto& out, const auto& in) { write_focus_cone(out, in, resolve, memory); });
			native.vfCurves = plain::write_array<SndCurve>(data.volume_falloff_curves, memory, [&](auto& out, const auto& in) { write_sound_curve(out, in, resolve, memory); });
			native.lpfCurves = plain::write_array<SndCurve>(data.lpf_curves, memory, [&](auto& out, const auto& in) { write_sound_curve(out, in, resolve, memory); });
			native.hpfCurves = plain::write_array<SndCurve>(data.hpf_curves, memory, [&](auto& out, const auto& in) { write_sound_curve(out, in, resolve, memory); });
			native.rvbCurves = plain::write_array<SndCurve>(data.reverb_curves, memory, [&](auto& out, const auto& in) { write_sound_curve(out, in, resolve, memory); });
			native.speakerMaps = plain::write_array<SpeakerMap>(data.speaker_maps, memory, [&](auto& out, const auto& in) { write_speaker_map(out, in, resolve, memory); });
			native.futzs = plain::write_array<SndFutz>(data.futzs, memory, [&](auto& out, const auto& in) { write_sound_futz(out, in, resolve, memory); });
			write_sound_bank_stream_info(native.streamInfo, data.stream_info, resolve, memory);
			native.aliasCount = plain::count_of<std::uint32_t>(data.alias);
			native.sendEffectCount = plain::count_of<std::uint32_t>(data.send_effects);
			native.duckCount = plain::count_of<std::uint32_t>(data.ducks);
			native.musicSetCount = plain::count_of<std::uint32_t>(data.music_sets);
			native.weapReflectionCount = plain::count_of<std::uint32_t>(data.weapon_reflections);
			native.dopplerPresetCount = plain::count_of<std::uint32_t>(data.doppler_presets);
			native.occlusionShapeCount = plain::count_of<std::uint32_t>(data.occlusion_shapes);
			native.focusConeCount = plain::count_of<std::uint32_t>(data.focus_cones);
			native.vfCurveCount = plain::count_of<std::uint32_t>(data.volume_falloff_curves);
			native.lpfCurveCount = plain::count_of<std::uint32_t>(data.lpf_curves);
			native.hpfCurveCount = plain::count_of<std::uint32_t>(data.hpf_curves);
			native.rvbCurveCount = plain::count_of<std::uint32_t>(data.reverb_curves);
			native.speakerMapCount = plain::count_of<std::uint32_t>(data.speaker_maps);
			native.futzCount = plain::count_of<std::uint32_t>(data.futzs);
		}

		std::size_t size_sound_bank([[maybe_unused]] const asset_data::sound_bank& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			size += plain::string_size(data.zone);
			size += plain::string_size(data.game_language);
			size += plain::string_size(data.sound_language);
			if (data.alias)
			{
				size += plain::rounded(std::max<std::size_t>(data.alias->size(), 1) * sizeof(SndAliasList));
				for (const auto& item : *data.alias)
				{
					size += size_sound_alias_list(item);
				}
			}
			if (data.alias_index)
			{
				size += plain::rounded(std::max<std::size_t>(data.alias_index->size(), 1) * sizeof(SndIndexEntry));
				for (const auto& item : *data.alias_index)
				{
					size += size_sound_index_entry(item);
				}
			}
			if (data.send_effects)
			{
				size += plain::rounded(std::max<std::size_t>(data.send_effects->size(), 1) * sizeof(SndSendEffectParams));
				for (const auto& item : *data.send_effects)
				{
					size += size_sound_send_effect_params(item);
				}
			}
			size += size_sound_table(data.tables);
			if (data.ducks)
			{
				size += plain::rounded(std::max<std::size_t>(data.ducks->size(), 1) * sizeof(SndDuck));
				for (const auto& item : *data.ducks)
				{
					size += size_sound_duck(item);
				}
			}
			if (data.music_sets)
			{
				size += plain::rounded(std::max<std::size_t>(data.music_sets->size(), 1) * sizeof(SndMusicSet));
				for (const auto& item : *data.music_sets)
				{
					size += size_sound_music_set(item);
				}
			}
			if (data.weapon_reflections)
			{
				size += plain::rounded(std::max<std::size_t>(data.weapon_reflections->size(), 1) * sizeof(WeaponReflectionDef));
				for (const auto& item : *data.weapon_reflections)
				{
					size += size_weapon_reflection_def(item);
				}
			}
			if (data.doppler_presets)
			{
				size += plain::rounded(std::max<std::size_t>(data.doppler_presets->size(), 1) * sizeof(DopplerPreset));
				for (const auto& item : *data.doppler_presets)
				{
					size += size_doppler_preset(item);
				}
			}
			if (data.occlusion_shapes)
			{
				size += plain::rounded(std::max<std::size_t>(data.occlusion_shapes->size(), 1) * sizeof(OcclusionShape));
				for (const auto& item : *data.occlusion_shapes)
				{
					size += size_occlusion_shape(item);
				}
			}
			if (data.focus_cones)
			{
				size += plain::rounded(std::max<std::size_t>(data.focus_cones->size(), 1) * sizeof(FocusCone));
				for (const auto& item : *data.focus_cones)
				{
					size += size_focus_cone(item);
				}
			}
			if (data.volume_falloff_curves)
			{
				size += plain::rounded(std::max<std::size_t>(data.volume_falloff_curves->size(), 1) * sizeof(SndCurve));
				for (const auto& item : *data.volume_falloff_curves)
				{
					size += size_sound_curve(item);
				}
			}
			if (data.lpf_curves)
			{
				size += plain::rounded(std::max<std::size_t>(data.lpf_curves->size(), 1) * sizeof(SndCurve));
				for (const auto& item : *data.lpf_curves)
				{
					size += size_sound_curve(item);
				}
			}
			if (data.hpf_curves)
			{
				size += plain::rounded(std::max<std::size_t>(data.hpf_curves->size(), 1) * sizeof(SndCurve));
				for (const auto& item : *data.hpf_curves)
				{
					size += size_sound_curve(item);
				}
			}
			if (data.reverb_curves)
			{
				size += plain::rounded(std::max<std::size_t>(data.reverb_curves->size(), 1) * sizeof(SndCurve));
				for (const auto& item : *data.reverb_curves)
				{
					size += size_sound_curve(item);
				}
			}
			if (data.speaker_maps)
			{
				size += plain::rounded(std::max<std::size_t>(data.speaker_maps->size(), 1) * sizeof(SpeakerMap));
				for (const auto& item : *data.speaker_maps)
				{
					size += size_speaker_map(item);
				}
			}
			if (data.futzs)
			{
				size += plain::rounded(std::max<std::size_t>(data.futzs->size(), 1) * sizeof(SndFutz));
				for (const auto& item : *data.futzs)
				{
					size += size_sound_futz(item);
				}
			}
			size += size_sound_bank_stream_info(data.stream_info);
			return size;
		}

		asset_data::sound_bank_resident read_sound_bank_resident(const SndBankResident& native)
		{
			asset_data::sound_bank_resident data;
			data.bank = read_sound_bank(native.bank);
			return data;
		}

		void write_sound_bank_resident(SndBankResident& native, const asset_data::sound_bank_resident& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_sound_bank(native.bank, data.bank, resolve, memory);
		}

		std::size_t size_sound_bank_resident([[maybe_unused]] const asset_data::sound_bank_resident& data)
		{
			std::size_t size = 0;
			size += size_sound_bank(data.bank);
			return size;
		}

		asset_data::sound_bank_transient read_sound_bank_transient(const SndBankTransient& native)
		{
			asset_data::sound_bank_transient data;
			data.bank = read_sound_bank(native.bank);
			return data;
		}

		void write_sound_bank_transient(SndBankTransient& native, const asset_data::sound_bank_transient& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_sound_bank(native.bank, data.bank, resolve, memory);
		}

		std::size_t size_sound_bank_transient([[maybe_unused]] const asset_data::sound_bank_transient& data)
		{
			std::size_t size = 0;
			size += size_sound_bank(data.bank);
			return size;
		}

	}

	std::expected<asset_data::sound_bank_resident, std::string> to_neutral(const SndBankResident& native)
	{
		try
		{
			auto data = read_sound_bank_resident(native);
			asset_data::validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	SndBankResident* from_neutral(const asset_data::sound_bank_resident& data, const asset_resolver& resolve, zone_memory& memory)
	{
		asset_data::validate(data);
		plain::context context(memory);
		auto* native = context.allocate<SndBankResident>();
		write_sound_bank_resident(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::sound_bank_resident& data)
	{
		return plain::rounded(sizeof(SndBankResident)) + size_sound_bank_resident(data) + 64;
	}

	std::expected<asset_data::sound_bank_transient, std::string> to_neutral(const SndBankTransient& native)
	{
		try
		{
			auto data = read_sound_bank_transient(native);
			asset_data::validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	SndBankTransient* from_neutral(const asset_data::sound_bank_transient& data, const asset_resolver& resolve, zone_memory& memory)
	{
		asset_data::validate(data);
		plain::context context(memory);
		auto* native = context.allocate<SndBankTransient>();
		write_sound_bank_transient(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::sound_bank_transient& data)
	{
		return plain::rounded(sizeof(SndBankTransient)) + size_sound_bank_transient(data) + 64;
	}

}
