// Emitted by tools/gen_iw8_neutral.py from tools/neutral_families/physics_events.json: do not edit.
#include <std_include.hpp>
#include "physics_events.hpp"
#include "plain.hpp"

namespace zonetool::iw8::neutral
{
	namespace
	{
		asset_data::physics_events::physics_sfx_event read_physics_sfx_event(const PhysicsSFXEventAsset& native);
		void write_physics_sfx_event(PhysicsSFXEventAsset& native, const asset_data::physics_events::physics_sfx_event& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_sfx_event(const asset_data::physics_events::physics_sfx_event& data);
		asset_data::physics_events::physics_sfx_event_asset_rules read_physics_sfx_event_asset_rules(const PhysicsSFXEventAssetRules& native);
		void write_physics_sfx_event_asset_rules(PhysicsSFXEventAssetRules& native, const asset_data::physics_events::physics_sfx_event_asset_rules& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_sfx_event_asset_rules(const asset_data::physics_events::physics_sfx_event_asset_rules& data);
		asset_data::physics_events::physics_sfx_event_asset_rule read_physics_sfx_event_asset_rule(const PhysicsSFXEventAssetRule& native);
		void write_physics_sfx_event_asset_rule(PhysicsSFXEventAssetRule& native, const asset_data::physics_events::physics_sfx_event_asset_rule& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_sfx_event_asset_rule(const asset_data::physics_events::physics_sfx_event_asset_rule& data);
		asset_data::physics_events::physics_sfx_event_asset_sound_rule read_physics_sfx_event_asset_sound_rule(const PhysicsSFXEventAssetSoundRule& native);
		void write_physics_sfx_event_asset_sound_rule(PhysicsSFXEventAssetSoundRule& native, const asset_data::physics_events::physics_sfx_event_asset_sound_rule& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_sfx_event_asset_sound_rule(const asset_data::physics_events::physics_sfx_event_asset_sound_rule& data);
		asset_data::physics_events::physics_sfx_event_asset_material_rule read_physics_sfx_event_asset_material_rule(const PhysicsSFXEventAssetMaterialRule& native);
		void write_physics_sfx_event_asset_material_rule(PhysicsSFXEventAssetMaterialRule& native, const asset_data::physics_events::physics_sfx_event_asset_material_rule& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_sfx_event_asset_material_rule(const asset_data::physics_events::physics_sfx_event_asset_material_rule& data);
		asset_data::physics_events::physics_sfx_event_asset_material_entry read_physics_sfx_event_asset_material_entry(const PhysicsSFXEventAssetMaterialEntry& native);
		void write_physics_sfx_event_asset_material_entry(PhysicsSFXEventAssetMaterialEntry& native, const asset_data::physics_events::physics_sfx_event_asset_material_entry& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_sfx_event_asset_material_entry(const asset_data::physics_events::physics_sfx_event_asset_material_entry& data);
		asset_data::physics_events::physics_sfx_event_asset_angle_rule read_physics_sfx_event_asset_angle_rule(const PhysicsSFXEventAssetAngleRule& native);
		void write_physics_sfx_event_asset_angle_rule(PhysicsSFXEventAssetAngleRule& native, const asset_data::physics_events::physics_sfx_event_asset_angle_rule& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_sfx_event_asset_angle_rule(const asset_data::physics_events::physics_sfx_event_asset_angle_rule& data);
		asset_data::physics_events::physics_sfx_event_asset_angle_entry read_physics_sfx_event_asset_angle_entry(const PhysicsSFXEventAssetAngleEntry& native);
		void write_physics_sfx_event_asset_angle_entry(PhysicsSFXEventAssetAngleEntry& native, const asset_data::physics_events::physics_sfx_event_asset_angle_entry& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_sfx_event_asset_angle_entry(const asset_data::physics_events::physics_sfx_event_asset_angle_entry& data);
		asset_data::physics_events::physics_sfx_event_asset_speed_rule read_physics_sfx_event_asset_speed_rule(const PhysicsSFXEventAssetSpeedRule& native);
		void write_physics_sfx_event_asset_speed_rule(PhysicsSFXEventAssetSpeedRule& native, const asset_data::physics_events::physics_sfx_event_asset_speed_rule& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_sfx_event_asset_speed_rule(const asset_data::physics_events::physics_sfx_event_asset_speed_rule& data);
		asset_data::physics_events::physics_sfx_event_asset_speed_entry read_physics_sfx_event_asset_speed_entry(const PhysicsSFXEventAssetSpeedEntry& native);
		void write_physics_sfx_event_asset_speed_entry(PhysicsSFXEventAssetSpeedEntry& native, const asset_data::physics_events::physics_sfx_event_asset_speed_entry& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_sfx_event_asset_speed_entry(const asset_data::physics_events::physics_sfx_event_asset_speed_entry& data);
		asset_data::physics_events::physics_sfx_event_asset_mass_rule read_physics_sfx_event_asset_mass_rule(const PhysicsSFXEventAssetMassRule& native);
		void write_physics_sfx_event_asset_mass_rule(PhysicsSFXEventAssetMassRule& native, const asset_data::physics_events::physics_sfx_event_asset_mass_rule& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_sfx_event_asset_mass_rule(const asset_data::physics_events::physics_sfx_event_asset_mass_rule& data);
		asset_data::physics_events::physics_sfx_event_asset_mass_entry read_physics_sfx_event_asset_mass_entry(const PhysicsSFXEventAssetMassEntry& native);
		void write_physics_sfx_event_asset_mass_entry(PhysicsSFXEventAssetMassEntry& native, const asset_data::physics_events::physics_sfx_event_asset_mass_entry& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_sfx_event_asset_mass_entry(const asset_data::physics_events::physics_sfx_event_asset_mass_entry& data);
		asset_data::physics_events::physics_sfx_event_asset_gravity_rule read_physics_sfx_event_asset_gravity_rule(const PhysicsSFXEventAssetGravityRule& native);
		void write_physics_sfx_event_asset_gravity_rule(PhysicsSFXEventAssetGravityRule& native, const asset_data::physics_events::physics_sfx_event_asset_gravity_rule& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_sfx_event_asset_gravity_rule(const asset_data::physics_events::physics_sfx_event_asset_gravity_rule& data);
		asset_data::physics_events::physics_sfx_event_asset_gravity_entry read_physics_sfx_event_asset_gravity_entry(const PhysicsSFXEventAssetGravityEntry& native);
		void write_physics_sfx_event_asset_gravity_entry(PhysicsSFXEventAssetGravityEntry& native, const asset_data::physics_events::physics_sfx_event_asset_gravity_entry& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_sfx_event_asset_gravity_entry(const asset_data::physics_events::physics_sfx_event_asset_gravity_entry& data);
		asset_data::physics_events::physics_sfx_event_asset_atmosphere_rule read_physics_sfx_event_asset_atmosphere_rule(const PhysicsSFXEventAssetAtmosphereRule& native);
		void write_physics_sfx_event_asset_atmosphere_rule(PhysicsSFXEventAssetAtmosphereRule& native, const asset_data::physics_events::physics_sfx_event_asset_atmosphere_rule& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_sfx_event_asset_atmosphere_rule(const asset_data::physics_events::physics_sfx_event_asset_atmosphere_rule& data);
		asset_data::physics_events::physics_sfx_event_asset_atmosphere_entry read_physics_sfx_event_asset_atmosphere_entry(const PhysicsSFXEventAssetAtmosphereEntry& native);
		void write_physics_sfx_event_asset_atmosphere_entry(PhysicsSFXEventAssetAtmosphereEntry& native, const asset_data::physics_events::physics_sfx_event_asset_atmosphere_entry& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_sfx_event_asset_atmosphere_entry(const asset_data::physics_events::physics_sfx_event_asset_atmosphere_entry& data);
		asset_data::physics_events::physics_sfx_event_asset_camera_distance_rule read_physics_sfx_event_asset_camera_distance_rule(const PhysicsSFXEventAssetCameraDistanceRule& native);
		void write_physics_sfx_event_asset_camera_distance_rule(PhysicsSFXEventAssetCameraDistanceRule& native, const asset_data::physics_events::physics_sfx_event_asset_camera_distance_rule& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_sfx_event_asset_camera_distance_rule(const asset_data::physics_events::physics_sfx_event_asset_camera_distance_rule& data);
		asset_data::physics_events::physics_sfx_event_asset_camera_distance_entry read_physics_sfx_event_asset_camera_distance_entry(const PhysicsSFXEventAssetCameraDistanceEntry& native);
		void write_physics_sfx_event_asset_camera_distance_entry(PhysicsSFXEventAssetCameraDistanceEntry& native, const asset_data::physics_events::physics_sfx_event_asset_camera_distance_entry& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_sfx_event_asset_camera_distance_entry(const asset_data::physics_events::physics_sfx_event_asset_camera_distance_entry& data);
		asset_data::physics_events::physics_sfx_event_asset_frequency_rule read_physics_sfx_event_asset_frequency_rule(const PhysicsSFXEventAssetFrequencyRule& native);
		void write_physics_sfx_event_asset_frequency_rule(PhysicsSFXEventAssetFrequencyRule& native, const asset_data::physics_events::physics_sfx_event_asset_frequency_rule& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_sfx_event_asset_frequency_rule(const asset_data::physics_events::physics_sfx_event_asset_frequency_rule& data);
		asset_data::physics_events::physics_sfx_event_asset_frequency_entry read_physics_sfx_event_asset_frequency_entry(const PhysicsSFXEventAssetFrequencyEntry& native);
		void write_physics_sfx_event_asset_frequency_entry(PhysicsSFXEventAssetFrequencyEntry& native, const asset_data::physics_events::physics_sfx_event_asset_frequency_entry& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_sfx_event_asset_frequency_entry(const asset_data::physics_events::physics_sfx_event_asset_frequency_entry& data);
		asset_data::physics_events::physics_vfx_event read_physics_vfx_event(const PhysicsVFXEventAsset& native);
		void write_physics_vfx_event(PhysicsVFXEventAsset& native, const asset_data::physics_events::physics_vfx_event& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_vfx_event(const asset_data::physics_events::physics_vfx_event& data);
		asset_data::physics_events::physics_vfx_event_asset_rules read_physics_vfx_event_asset_rules(const PhysicsVFXEventAssetRules& native);
		void write_physics_vfx_event_asset_rules(PhysicsVFXEventAssetRules& native, const asset_data::physics_events::physics_vfx_event_asset_rules& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_vfx_event_asset_rules(const asset_data::physics_events::physics_vfx_event_asset_rules& data);
		asset_data::physics_events::physics_vfx_event_asset_rule read_physics_vfx_event_asset_rule(const PhysicsVFXEventAssetRule& native);
		void write_physics_vfx_event_asset_rule(PhysicsVFXEventAssetRule& native, const asset_data::physics_events::physics_vfx_event_asset_rule& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_vfx_event_asset_rule(const asset_data::physics_events::physics_vfx_event_asset_rule& data);
		asset_data::physics_events::physics_vfx_event_asset_particle_effect_rule read_physics_vfx_event_asset_particle_effect_rule(const PhysicsVFXEventAssetParticleEffectRule& native);
		void write_physics_vfx_event_asset_particle_effect_rule(PhysicsVFXEventAssetParticleEffectRule& native, const asset_data::physics_events::physics_vfx_event_asset_particle_effect_rule& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_vfx_event_asset_particle_effect_rule(const asset_data::physics_events::physics_vfx_event_asset_particle_effect_rule& data);
		asset_data::physics_events::physics_vfx_event_asset_material_rule read_physics_vfx_event_asset_material_rule(const PhysicsVFXEventAssetMaterialRule& native);
		void write_physics_vfx_event_asset_material_rule(PhysicsVFXEventAssetMaterialRule& native, const asset_data::physics_events::physics_vfx_event_asset_material_rule& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_vfx_event_asset_material_rule(const asset_data::physics_events::physics_vfx_event_asset_material_rule& data);
		asset_data::physics_events::physics_vfx_event_asset_material_entry read_physics_vfx_event_asset_material_entry(const PhysicsVFXEventAssetMaterialEntry& native);
		void write_physics_vfx_event_asset_material_entry(PhysicsVFXEventAssetMaterialEntry& native, const asset_data::physics_events::physics_vfx_event_asset_material_entry& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_vfx_event_asset_material_entry(const asset_data::physics_events::physics_vfx_event_asset_material_entry& data);
		asset_data::physics_events::physics_vfx_event_asset_angle_rule read_physics_vfx_event_asset_angle_rule(const PhysicsVFXEventAssetAngleRule& native);
		void write_physics_vfx_event_asset_angle_rule(PhysicsVFXEventAssetAngleRule& native, const asset_data::physics_events::physics_vfx_event_asset_angle_rule& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_vfx_event_asset_angle_rule(const asset_data::physics_events::physics_vfx_event_asset_angle_rule& data);
		asset_data::physics_events::physics_vfx_event_asset_angle_entry read_physics_vfx_event_asset_angle_entry(const PhysicsVFXEventAssetAngleEntry& native);
		void write_physics_vfx_event_asset_angle_entry(PhysicsVFXEventAssetAngleEntry& native, const asset_data::physics_events::physics_vfx_event_asset_angle_entry& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_vfx_event_asset_angle_entry(const asset_data::physics_events::physics_vfx_event_asset_angle_entry& data);
		asset_data::physics_events::physics_vfx_event_asset_speed_rule read_physics_vfx_event_asset_speed_rule(const PhysicsVFXEventAssetSpeedRule& native);
		void write_physics_vfx_event_asset_speed_rule(PhysicsVFXEventAssetSpeedRule& native, const asset_data::physics_events::physics_vfx_event_asset_speed_rule& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_vfx_event_asset_speed_rule(const asset_data::physics_events::physics_vfx_event_asset_speed_rule& data);
		asset_data::physics_events::physics_vfx_event_asset_speed_entry read_physics_vfx_event_asset_speed_entry(const PhysicsVFXEventAssetSpeedEntry& native);
		void write_physics_vfx_event_asset_speed_entry(PhysicsVFXEventAssetSpeedEntry& native, const asset_data::physics_events::physics_vfx_event_asset_speed_entry& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_vfx_event_asset_speed_entry(const asset_data::physics_events::physics_vfx_event_asset_speed_entry& data);
		asset_data::physics_events::physics_vfx_event_asset_mass_rule read_physics_vfx_event_asset_mass_rule(const PhysicsVFXEventAssetMassRule& native);
		void write_physics_vfx_event_asset_mass_rule(PhysicsVFXEventAssetMassRule& native, const asset_data::physics_events::physics_vfx_event_asset_mass_rule& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_vfx_event_asset_mass_rule(const asset_data::physics_events::physics_vfx_event_asset_mass_rule& data);
		asset_data::physics_events::physics_vfx_event_asset_mass_entry read_physics_vfx_event_asset_mass_entry(const PhysicsVFXEventAssetMassEntry& native);
		void write_physics_vfx_event_asset_mass_entry(PhysicsVFXEventAssetMassEntry& native, const asset_data::physics_events::physics_vfx_event_asset_mass_entry& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_vfx_event_asset_mass_entry(const asset_data::physics_events::physics_vfx_event_asset_mass_entry& data);
		asset_data::physics_events::physics_vfx_event_asset_gravity_rule read_physics_vfx_event_asset_gravity_rule(const PhysicsVFXEventAssetGravityRule& native);
		void write_physics_vfx_event_asset_gravity_rule(PhysicsVFXEventAssetGravityRule& native, const asset_data::physics_events::physics_vfx_event_asset_gravity_rule& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_vfx_event_asset_gravity_rule(const asset_data::physics_events::physics_vfx_event_asset_gravity_rule& data);
		asset_data::physics_events::physics_vfx_event_asset_gravity_entry read_physics_vfx_event_asset_gravity_entry(const PhysicsVFXEventAssetGravityEntry& native);
		void write_physics_vfx_event_asset_gravity_entry(PhysicsVFXEventAssetGravityEntry& native, const asset_data::physics_events::physics_vfx_event_asset_gravity_entry& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_vfx_event_asset_gravity_entry(const asset_data::physics_events::physics_vfx_event_asset_gravity_entry& data);
		asset_data::physics_events::physics_vfx_event_asset_atmosphere_rule read_physics_vfx_event_asset_atmosphere_rule(const PhysicsVFXEventAssetAtmosphereRule& native);
		void write_physics_vfx_event_asset_atmosphere_rule(PhysicsVFXEventAssetAtmosphereRule& native, const asset_data::physics_events::physics_vfx_event_asset_atmosphere_rule& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_vfx_event_asset_atmosphere_rule(const asset_data::physics_events::physics_vfx_event_asset_atmosphere_rule& data);
		asset_data::physics_events::physics_vfx_event_asset_atmosphere_entry read_physics_vfx_event_asset_atmosphere_entry(const PhysicsVFXEventAssetAtmosphereEntry& native);
		void write_physics_vfx_event_asset_atmosphere_entry(PhysicsVFXEventAssetAtmosphereEntry& native, const asset_data::physics_events::physics_vfx_event_asset_atmosphere_entry& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_vfx_event_asset_atmosphere_entry(const asset_data::physics_events::physics_vfx_event_asset_atmosphere_entry& data);
		asset_data::physics_events::physics_vfx_event_asset_camera_distance_rule read_physics_vfx_event_asset_camera_distance_rule(const PhysicsVFXEventAssetCameraDistanceRule& native);
		void write_physics_vfx_event_asset_camera_distance_rule(PhysicsVFXEventAssetCameraDistanceRule& native, const asset_data::physics_events::physics_vfx_event_asset_camera_distance_rule& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_vfx_event_asset_camera_distance_rule(const asset_data::physics_events::physics_vfx_event_asset_camera_distance_rule& data);
		asset_data::physics_events::physics_vfx_event_asset_camera_distance_entry read_physics_vfx_event_asset_camera_distance_entry(const PhysicsVFXEventAssetCameraDistanceEntry& native);
		void write_physics_vfx_event_asset_camera_distance_entry(PhysicsVFXEventAssetCameraDistanceEntry& native, const asset_data::physics_events::physics_vfx_event_asset_camera_distance_entry& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_vfx_event_asset_camera_distance_entry(const asset_data::physics_events::physics_vfx_event_asset_camera_distance_entry& data);
		asset_data::physics_events::physics_vfx_event_asset_frequency_rule read_physics_vfx_event_asset_frequency_rule(const PhysicsVFXEventAssetFrequencyRule& native);
		void write_physics_vfx_event_asset_frequency_rule(PhysicsVFXEventAssetFrequencyRule& native, const asset_data::physics_events::physics_vfx_event_asset_frequency_rule& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_vfx_event_asset_frequency_rule(const asset_data::physics_events::physics_vfx_event_asset_frequency_rule& data);
		asset_data::physics_events::physics_vfx_event_asset_frequency_entry read_physics_vfx_event_asset_frequency_entry(const PhysicsVFXEventAssetFrequencyEntry& native);
		void write_physics_vfx_event_asset_frequency_entry(PhysicsVFXEventAssetFrequencyEntry& native, const asset_data::physics_events::physics_vfx_event_asset_frequency_entry& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_physics_vfx_event_asset_frequency_entry(const asset_data::physics_events::physics_vfx_event_asset_frequency_entry& data);

		asset_data::physics_events::physics_sfx_event_asset_rules read_physics_sfx_event_asset_rules(const PhysicsSFXEventAssetRules& native)
		{
			plain::trace("read_physics_sfx_event_asset_rules");
			asset_data::physics_events::physics_sfx_event_asset_rules data;
			data.numrules = native.numrules;
			data.rules = plain::read_array(native.rules, native.numrules, "PhysicsSFXEventAssetRules.rules", [&](const auto& item) { return read_physics_sfx_event_asset_rule(item); });
			return data;
		}

		void write_physics_sfx_event_asset_rules(PhysicsSFXEventAssetRules& native, const asset_data::physics_events::physics_sfx_event_asset_rules& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.numrules = data.numrules;
			native.rules = plain::write_array<PhysicsSFXEventAssetRule>(data.rules, memory, [&](auto& out, const auto& in) { write_physics_sfx_event_asset_rule(out, in, resolve, memory); });
		}

		std::size_t size_physics_sfx_event_asset_rules([[maybe_unused]] const asset_data::physics_events::physics_sfx_event_asset_rules& data)
		{
			std::size_t size = 0;
			if (data.rules)
			{
				size += plain::sized(std::max<std::size_t>(data.rules->size(), 1) * sizeof(PhysicsSFXEventAssetRule));
				for (const auto& item : *data.rules)
				{
					size += size_physics_sfx_event_asset_rule(item);
				}
			}
			return size;
		}

		asset_data::physics_events::physics_sfx_event read_physics_sfx_event(const PhysicsSFXEventAsset& native)
		{
			plain::trace("read_physics_sfx_event");
			asset_data::physics_events::physics_sfx_event data;
			data.name = plain::read_string(native.name);
			data.priority = native.priority;
			data.rules = read_physics_sfx_event_asset_rules(native.rules);
			return data;
		}

		void write_physics_sfx_event(PhysicsSFXEventAsset& native, const asset_data::physics_events::physics_sfx_event& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.priority = data.priority;
			write_physics_sfx_event_asset_rules(native.rules, data.rules, resolve, memory);
		}

		std::size_t size_physics_sfx_event([[maybe_unused]] const asset_data::physics_events::physics_sfx_event& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			size += size_physics_sfx_event_asset_rules(data.rules);
			return size;
		}

		asset_data::physics_events::physics_sfx_event_asset_sound_rule read_physics_sfx_event_asset_sound_rule(const PhysicsSFXEventAssetSoundRule& native)
		{
			plain::trace("read_physics_sfx_event_asset_sound_rule");
			asset_data::physics_events::physics_sfx_event_asset_sound_rule data;
			data.hit_sound_alias = plain::read_string(native.hitSoundAlias);
			data.scrape_sound_alias = plain::read_string(native.scrapeSoundAlias);
			data.scrape_sound_volume_speed_threshold_min = native.scrapeSoundVolumeSpeedThresholdMin;
			data.scrape_sound_volume_speed_threshold_max = native.scrapeSoundVolumeSpeedThresholdMax;
			data.scrape_sound_volume_blend_speed = native.scrapeSoundVolumeBlendSpeed;
			return data;
		}

		void write_physics_sfx_event_asset_sound_rule(PhysicsSFXEventAssetSoundRule& native, const asset_data::physics_events::physics_sfx_event_asset_sound_rule& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.hitSoundAlias = plain::write_string(memory, data.hit_sound_alias);
			native.scrapeSoundAlias = plain::write_string(memory, data.scrape_sound_alias);
			native.scrapeSoundVolumeSpeedThresholdMin = data.scrape_sound_volume_speed_threshold_min;
			native.scrapeSoundVolumeSpeedThresholdMax = data.scrape_sound_volume_speed_threshold_max;
			native.scrapeSoundVolumeBlendSpeed = data.scrape_sound_volume_blend_speed;
		}

		std::size_t size_physics_sfx_event_asset_sound_rule([[maybe_unused]] const asset_data::physics_events::physics_sfx_event_asset_sound_rule& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.hit_sound_alias);
			size += plain::string_size(data.scrape_sound_alias);
			return size;
		}

		asset_data::physics_events::physics_sfx_event_asset_material_rule read_physics_sfx_event_asset_material_rule(const PhysicsSFXEventAssetMaterialRule& native)
		{
			plain::trace("read_physics_sfx_event_asset_material_rule");
			asset_data::physics_events::physics_sfx_event_asset_material_rule data;
			data.numentries = native.numentries;
			data.entries = plain::read_array(native.entries, native.numentries, "PhysicsSFXEventAssetMaterialRule.entries", [&](const auto& item) { return read_physics_sfx_event_asset_material_entry(item); });
			return data;
		}

		void write_physics_sfx_event_asset_material_rule(PhysicsSFXEventAssetMaterialRule& native, const asset_data::physics_events::physics_sfx_event_asset_material_rule& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.numentries = data.numentries;
			native.entries = plain::write_array<PhysicsSFXEventAssetMaterialEntry>(data.entries, memory, [&](auto& out, const auto& in) { write_physics_sfx_event_asset_material_entry(out, in, resolve, memory); });
		}

		std::size_t size_physics_sfx_event_asset_material_rule([[maybe_unused]] const asset_data::physics_events::physics_sfx_event_asset_material_rule& data)
		{
			std::size_t size = 0;
			if (data.entries)
			{
				size += plain::sized(std::max<std::size_t>(data.entries->size(), 1) * sizeof(PhysicsSFXEventAssetMaterialEntry));
				for (const auto& item : *data.entries)
				{
					size += size_physics_sfx_event_asset_material_entry(item);
				}
			}
			return size;
		}

		asset_data::physics_events::physics_sfx_event_asset_angle_rule read_physics_sfx_event_asset_angle_rule(const PhysicsSFXEventAssetAngleRule& native)
		{
			plain::trace("read_physics_sfx_event_asset_angle_rule");
			asset_data::physics_events::physics_sfx_event_asset_angle_rule data;
			data.numentries = native.numentries;
			data.entries = plain::read_array(native.entries, native.numentries, "PhysicsSFXEventAssetAngleRule.entries", [&](const auto& item) { return read_physics_sfx_event_asset_angle_entry(item); });
			return data;
		}

		void write_physics_sfx_event_asset_angle_rule(PhysicsSFXEventAssetAngleRule& native, const asset_data::physics_events::physics_sfx_event_asset_angle_rule& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.numentries = data.numentries;
			native.entries = plain::write_array<PhysicsSFXEventAssetAngleEntry>(data.entries, memory, [&](auto& out, const auto& in) { write_physics_sfx_event_asset_angle_entry(out, in, resolve, memory); });
		}

		std::size_t size_physics_sfx_event_asset_angle_rule([[maybe_unused]] const asset_data::physics_events::physics_sfx_event_asset_angle_rule& data)
		{
			std::size_t size = 0;
			if (data.entries)
			{
				size += plain::sized(std::max<std::size_t>(data.entries->size(), 1) * sizeof(PhysicsSFXEventAssetAngleEntry));
				for (const auto& item : *data.entries)
				{
					size += size_physics_sfx_event_asset_angle_entry(item);
				}
			}
			return size;
		}

		asset_data::physics_events::physics_sfx_event_asset_speed_rule read_physics_sfx_event_asset_speed_rule(const PhysicsSFXEventAssetSpeedRule& native)
		{
			plain::trace("read_physics_sfx_event_asset_speed_rule");
			asset_data::physics_events::physics_sfx_event_asset_speed_rule data;
			data.numentries = native.numentries;
			data.entries = plain::read_array(native.entries, native.numentries, "PhysicsSFXEventAssetSpeedRule.entries", [&](const auto& item) { return read_physics_sfx_event_asset_speed_entry(item); });
			return data;
		}

		void write_physics_sfx_event_asset_speed_rule(PhysicsSFXEventAssetSpeedRule& native, const asset_data::physics_events::physics_sfx_event_asset_speed_rule& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.numentries = data.numentries;
			native.entries = plain::write_array<PhysicsSFXEventAssetSpeedEntry>(data.entries, memory, [&](auto& out, const auto& in) { write_physics_sfx_event_asset_speed_entry(out, in, resolve, memory); });
		}

		std::size_t size_physics_sfx_event_asset_speed_rule([[maybe_unused]] const asset_data::physics_events::physics_sfx_event_asset_speed_rule& data)
		{
			std::size_t size = 0;
			if (data.entries)
			{
				size += plain::sized(std::max<std::size_t>(data.entries->size(), 1) * sizeof(PhysicsSFXEventAssetSpeedEntry));
				for (const auto& item : *data.entries)
				{
					size += size_physics_sfx_event_asset_speed_entry(item);
				}
			}
			return size;
		}

		asset_data::physics_events::physics_sfx_event_asset_mass_rule read_physics_sfx_event_asset_mass_rule(const PhysicsSFXEventAssetMassRule& native)
		{
			plain::trace("read_physics_sfx_event_asset_mass_rule");
			asset_data::physics_events::physics_sfx_event_asset_mass_rule data;
			data.numentries = native.numentries;
			data.entries = plain::read_array(native.entries, native.numentries, "PhysicsSFXEventAssetMassRule.entries", [&](const auto& item) { return read_physics_sfx_event_asset_mass_entry(item); });
			return data;
		}

		void write_physics_sfx_event_asset_mass_rule(PhysicsSFXEventAssetMassRule& native, const asset_data::physics_events::physics_sfx_event_asset_mass_rule& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.numentries = data.numentries;
			native.entries = plain::write_array<PhysicsSFXEventAssetMassEntry>(data.entries, memory, [&](auto& out, const auto& in) { write_physics_sfx_event_asset_mass_entry(out, in, resolve, memory); });
		}

		std::size_t size_physics_sfx_event_asset_mass_rule([[maybe_unused]] const asset_data::physics_events::physics_sfx_event_asset_mass_rule& data)
		{
			std::size_t size = 0;
			if (data.entries)
			{
				size += plain::sized(std::max<std::size_t>(data.entries->size(), 1) * sizeof(PhysicsSFXEventAssetMassEntry));
				for (const auto& item : *data.entries)
				{
					size += size_physics_sfx_event_asset_mass_entry(item);
				}
			}
			return size;
		}

		asset_data::physics_events::physics_sfx_event_asset_gravity_rule read_physics_sfx_event_asset_gravity_rule(const PhysicsSFXEventAssetGravityRule& native)
		{
			plain::trace("read_physics_sfx_event_asset_gravity_rule");
			asset_data::physics_events::physics_sfx_event_asset_gravity_rule data;
			data.numentries = native.numentries;
			data.entries = plain::read_array(native.entries, native.numentries, "PhysicsSFXEventAssetGravityRule.entries", [&](const auto& item) { return read_physics_sfx_event_asset_gravity_entry(item); });
			return data;
		}

		void write_physics_sfx_event_asset_gravity_rule(PhysicsSFXEventAssetGravityRule& native, const asset_data::physics_events::physics_sfx_event_asset_gravity_rule& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.numentries = data.numentries;
			native.entries = plain::write_array<PhysicsSFXEventAssetGravityEntry>(data.entries, memory, [&](auto& out, const auto& in) { write_physics_sfx_event_asset_gravity_entry(out, in, resolve, memory); });
		}

		std::size_t size_physics_sfx_event_asset_gravity_rule([[maybe_unused]] const asset_data::physics_events::physics_sfx_event_asset_gravity_rule& data)
		{
			std::size_t size = 0;
			if (data.entries)
			{
				size += plain::sized(std::max<std::size_t>(data.entries->size(), 1) * sizeof(PhysicsSFXEventAssetGravityEntry));
				for (const auto& item : *data.entries)
				{
					size += size_physics_sfx_event_asset_gravity_entry(item);
				}
			}
			return size;
		}

		asset_data::physics_events::physics_sfx_event_asset_atmosphere_rule read_physics_sfx_event_asset_atmosphere_rule(const PhysicsSFXEventAssetAtmosphereRule& native)
		{
			plain::trace("read_physics_sfx_event_asset_atmosphere_rule");
			asset_data::physics_events::physics_sfx_event_asset_atmosphere_rule data;
			data.numentries = native.numentries;
			data.entries = plain::read_array(native.entries, native.numentries, "PhysicsSFXEventAssetAtmosphereRule.entries", [&](const auto& item) { return read_physics_sfx_event_asset_atmosphere_entry(item); });
			return data;
		}

		void write_physics_sfx_event_asset_atmosphere_rule(PhysicsSFXEventAssetAtmosphereRule& native, const asset_data::physics_events::physics_sfx_event_asset_atmosphere_rule& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.numentries = data.numentries;
			native.entries = plain::write_array<PhysicsSFXEventAssetAtmosphereEntry>(data.entries, memory, [&](auto& out, const auto& in) { write_physics_sfx_event_asset_atmosphere_entry(out, in, resolve, memory); });
		}

		std::size_t size_physics_sfx_event_asset_atmosphere_rule([[maybe_unused]] const asset_data::physics_events::physics_sfx_event_asset_atmosphere_rule& data)
		{
			std::size_t size = 0;
			if (data.entries)
			{
				size += plain::sized(std::max<std::size_t>(data.entries->size(), 1) * sizeof(PhysicsSFXEventAssetAtmosphereEntry));
				for (const auto& item : *data.entries)
				{
					size += size_physics_sfx_event_asset_atmosphere_entry(item);
				}
			}
			return size;
		}

		asset_data::physics_events::physics_sfx_event_asset_camera_distance_rule read_physics_sfx_event_asset_camera_distance_rule(const PhysicsSFXEventAssetCameraDistanceRule& native)
		{
			plain::trace("read_physics_sfx_event_asset_camera_distance_rule");
			asset_data::physics_events::physics_sfx_event_asset_camera_distance_rule data;
			data.numentries = native.numentries;
			data.entries = plain::read_array(native.entries, native.numentries, "PhysicsSFXEventAssetCameraDistanceRule.entries", [&](const auto& item) { return read_physics_sfx_event_asset_camera_distance_entry(item); });
			return data;
		}

		void write_physics_sfx_event_asset_camera_distance_rule(PhysicsSFXEventAssetCameraDistanceRule& native, const asset_data::physics_events::physics_sfx_event_asset_camera_distance_rule& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.numentries = data.numentries;
			native.entries = plain::write_array<PhysicsSFXEventAssetCameraDistanceEntry>(data.entries, memory, [&](auto& out, const auto& in) { write_physics_sfx_event_asset_camera_distance_entry(out, in, resolve, memory); });
		}

		std::size_t size_physics_sfx_event_asset_camera_distance_rule([[maybe_unused]] const asset_data::physics_events::physics_sfx_event_asset_camera_distance_rule& data)
		{
			std::size_t size = 0;
			if (data.entries)
			{
				size += plain::sized(std::max<std::size_t>(data.entries->size(), 1) * sizeof(PhysicsSFXEventAssetCameraDistanceEntry));
				for (const auto& item : *data.entries)
				{
					size += size_physics_sfx_event_asset_camera_distance_entry(item);
				}
			}
			return size;
		}

		asset_data::physics_events::physics_sfx_event_asset_frequency_rule read_physics_sfx_event_asset_frequency_rule(const PhysicsSFXEventAssetFrequencyRule& native)
		{
			plain::trace("read_physics_sfx_event_asset_frequency_rule");
			asset_data::physics_events::physics_sfx_event_asset_frequency_rule data;
			data.numentries = native.numentries;
			data.entries = plain::read_array(native.entries, native.numentries, "PhysicsSFXEventAssetFrequencyRule.entries", [&](const auto& item) { return read_physics_sfx_event_asset_frequency_entry(item); });
			return data;
		}

		void write_physics_sfx_event_asset_frequency_rule(PhysicsSFXEventAssetFrequencyRule& native, const asset_data::physics_events::physics_sfx_event_asset_frequency_rule& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.numentries = data.numentries;
			native.entries = plain::write_array<PhysicsSFXEventAssetFrequencyEntry>(data.entries, memory, [&](auto& out, const auto& in) { write_physics_sfx_event_asset_frequency_entry(out, in, resolve, memory); });
		}

		std::size_t size_physics_sfx_event_asset_frequency_rule([[maybe_unused]] const asset_data::physics_events::physics_sfx_event_asset_frequency_rule& data)
		{
			std::size_t size = 0;
			if (data.entries)
			{
				size += plain::sized(std::max<std::size_t>(data.entries->size(), 1) * sizeof(PhysicsSFXEventAssetFrequencyEntry));
				for (const auto& item : *data.entries)
				{
					size += size_physics_sfx_event_asset_frequency_entry(item);
				}
			}
			return size;
		}

		asset_data::physics_events::physics_sfx_event_asset_rule read_physics_sfx_event_asset_rule(const PhysicsSFXEventAssetRule& native)
		{
			plain::trace("read_physics_sfx_event_asset_rule");
			asset_data::physics_events::physics_sfx_event_asset_rule data;
			data.u = [&]() -> std::variant<std::monostate, asset_data::physics_events::physics_sfx_event_asset_sound_rule, asset_data::physics_events::physics_sfx_event_asset_material_rule, asset_data::physics_events::physics_sfx_event_asset_angle_rule, asset_data::physics_events::physics_sfx_event_asset_speed_rule, asset_data::physics_events::physics_sfx_event_asset_mass_rule, asset_data::physics_events::physics_sfx_event_asset_gravity_rule, asset_data::physics_events::physics_sfx_event_asset_atmosphere_rule, asset_data::physics_events::physics_sfx_event_asset_camera_distance_rule, asset_data::physics_events::physics_sfx_event_asset_frequency_rule> { switch (static_cast<std::uint32_t>(native.type)) { case 1: { plain::module_scope scope(1); auto item = read_physics_sfx_event_asset_sound_rule(native.u.soundRule); return std::variant<std::monostate, asset_data::physics_events::physics_sfx_event_asset_sound_rule, asset_data::physics_events::physics_sfx_event_asset_material_rule, asset_data::physics_events::physics_sfx_event_asset_angle_rule, asset_data::physics_events::physics_sfx_event_asset_speed_rule, asset_data::physics_events::physics_sfx_event_asset_mass_rule, asset_data::physics_events::physics_sfx_event_asset_gravity_rule, asset_data::physics_events::physics_sfx_event_asset_atmosphere_rule, asset_data::physics_events::physics_sfx_event_asset_camera_distance_rule, asset_data::physics_events::physics_sfx_event_asset_frequency_rule>(std::move(item)); } case 2: { plain::module_scope scope(2); auto item = read_physics_sfx_event_asset_material_rule(native.u.materialRule); return std::variant<std::monostate, asset_data::physics_events::physics_sfx_event_asset_sound_rule, asset_data::physics_events::physics_sfx_event_asset_material_rule, asset_data::physics_events::physics_sfx_event_asset_angle_rule, asset_data::physics_events::physics_sfx_event_asset_speed_rule, asset_data::physics_events::physics_sfx_event_asset_mass_rule, asset_data::physics_events::physics_sfx_event_asset_gravity_rule, asset_data::physics_events::physics_sfx_event_asset_atmosphere_rule, asset_data::physics_events::physics_sfx_event_asset_camera_distance_rule, asset_data::physics_events::physics_sfx_event_asset_frequency_rule>(std::move(item)); } case 3: { plain::module_scope scope(3); auto item = read_physics_sfx_event_asset_angle_rule(native.u.angleRule); return std::variant<std::monostate, asset_data::physics_events::physics_sfx_event_asset_sound_rule, asset_data::physics_events::physics_sfx_event_asset_material_rule, asset_data::physics_events::physics_sfx_event_asset_angle_rule, asset_data::physics_events::physics_sfx_event_asset_speed_rule, asset_data::physics_events::physics_sfx_event_asset_mass_rule, asset_data::physics_events::physics_sfx_event_asset_gravity_rule, asset_data::physics_events::physics_sfx_event_asset_atmosphere_rule, asset_data::physics_events::physics_sfx_event_asset_camera_distance_rule, asset_data::physics_events::physics_sfx_event_asset_frequency_rule>(std::move(item)); } case 4: { plain::module_scope scope(4); auto item = read_physics_sfx_event_asset_speed_rule(native.u.speedRule); return std::variant<std::monostate, asset_data::physics_events::physics_sfx_event_asset_sound_rule, asset_data::physics_events::physics_sfx_event_asset_material_rule, asset_data::physics_events::physics_sfx_event_asset_angle_rule, asset_data::physics_events::physics_sfx_event_asset_speed_rule, asset_data::physics_events::physics_sfx_event_asset_mass_rule, asset_data::physics_events::physics_sfx_event_asset_gravity_rule, asset_data::physics_events::physics_sfx_event_asset_atmosphere_rule, asset_data::physics_events::physics_sfx_event_asset_camera_distance_rule, asset_data::physics_events::physics_sfx_event_asset_frequency_rule>(std::move(item)); } case 5: { plain::module_scope scope(5); auto item = read_physics_sfx_event_asset_mass_rule(native.u.massRule); return std::variant<std::monostate, asset_data::physics_events::physics_sfx_event_asset_sound_rule, asset_data::physics_events::physics_sfx_event_asset_material_rule, asset_data::physics_events::physics_sfx_event_asset_angle_rule, asset_data::physics_events::physics_sfx_event_asset_speed_rule, asset_data::physics_events::physics_sfx_event_asset_mass_rule, asset_data::physics_events::physics_sfx_event_asset_gravity_rule, asset_data::physics_events::physics_sfx_event_asset_atmosphere_rule, asset_data::physics_events::physics_sfx_event_asset_camera_distance_rule, asset_data::physics_events::physics_sfx_event_asset_frequency_rule>(std::move(item)); } case 6: { plain::module_scope scope(6); auto item = read_physics_sfx_event_asset_gravity_rule(native.u.gravityRule); return std::variant<std::monostate, asset_data::physics_events::physics_sfx_event_asset_sound_rule, asset_data::physics_events::physics_sfx_event_asset_material_rule, asset_data::physics_events::physics_sfx_event_asset_angle_rule, asset_data::physics_events::physics_sfx_event_asset_speed_rule, asset_data::physics_events::physics_sfx_event_asset_mass_rule, asset_data::physics_events::physics_sfx_event_asset_gravity_rule, asset_data::physics_events::physics_sfx_event_asset_atmosphere_rule, asset_data::physics_events::physics_sfx_event_asset_camera_distance_rule, asset_data::physics_events::physics_sfx_event_asset_frequency_rule>(std::move(item)); } case 7: { plain::module_scope scope(7); auto item = read_physics_sfx_event_asset_atmosphere_rule(native.u.atmosphereRule); return std::variant<std::monostate, asset_data::physics_events::physics_sfx_event_asset_sound_rule, asset_data::physics_events::physics_sfx_event_asset_material_rule, asset_data::physics_events::physics_sfx_event_asset_angle_rule, asset_data::physics_events::physics_sfx_event_asset_speed_rule, asset_data::physics_events::physics_sfx_event_asset_mass_rule, asset_data::physics_events::physics_sfx_event_asset_gravity_rule, asset_data::physics_events::physics_sfx_event_asset_atmosphere_rule, asset_data::physics_events::physics_sfx_event_asset_camera_distance_rule, asset_data::physics_events::physics_sfx_event_asset_frequency_rule>(std::move(item)); } case 8: { plain::module_scope scope(8); auto item = read_physics_sfx_event_asset_camera_distance_rule(native.u.cameraDistanceRule); return std::variant<std::monostate, asset_data::physics_events::physics_sfx_event_asset_sound_rule, asset_data::physics_events::physics_sfx_event_asset_material_rule, asset_data::physics_events::physics_sfx_event_asset_angle_rule, asset_data::physics_events::physics_sfx_event_asset_speed_rule, asset_data::physics_events::physics_sfx_event_asset_mass_rule, asset_data::physics_events::physics_sfx_event_asset_gravity_rule, asset_data::physics_events::physics_sfx_event_asset_atmosphere_rule, asset_data::physics_events::physics_sfx_event_asset_camera_distance_rule, asset_data::physics_events::physics_sfx_event_asset_frequency_rule>(std::move(item)); } case 9: { plain::module_scope scope(9); auto item = read_physics_sfx_event_asset_frequency_rule(native.u.frequencyRule); return std::variant<std::monostate, asset_data::physics_events::physics_sfx_event_asset_sound_rule, asset_data::physics_events::physics_sfx_event_asset_material_rule, asset_data::physics_events::physics_sfx_event_asset_angle_rule, asset_data::physics_events::physics_sfx_event_asset_speed_rule, asset_data::physics_events::physics_sfx_event_asset_mass_rule, asset_data::physics_events::physics_sfx_event_asset_gravity_rule, asset_data::physics_events::physics_sfx_event_asset_atmosphere_rule, asset_data::physics_events::physics_sfx_event_asset_camera_distance_rule, asset_data::physics_events::physics_sfx_event_asset_frequency_rule>(std::move(item)); } default: plain::fail("PhysicsSFXEventAssetRule.u", "has a tag value without a case"); } }();
			return data;
		}

		void write_physics_sfx_event_asset_rule(PhysicsSFXEventAssetRule& native, const asset_data::physics_events::physics_sfx_event_asset_rule& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			std::visit([&](const auto& value)
			{
				using value_type = std::decay_t<decltype(value)>;
				if constexpr (std::is_same_v<value_type, std::monostate>)
				{
					plain::fail("PhysicsSFXEventAssetRule.u", "holds no module");
				}
				else if constexpr (std::is_same_v<value_type, asset_data::physics_events::physics_sfx_event_asset_sound_rule>)
				{
					native.type = static_cast<PhysicsSFXEventAssetRuleType>(1);
					plain::module_scope scope(1);
					write_physics_sfx_event_asset_sound_rule(native.u.soundRule, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::physics_events::physics_sfx_event_asset_material_rule>)
				{
					native.type = static_cast<PhysicsSFXEventAssetRuleType>(2);
					plain::module_scope scope(2);
					write_physics_sfx_event_asset_material_rule(native.u.materialRule, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::physics_events::physics_sfx_event_asset_angle_rule>)
				{
					native.type = static_cast<PhysicsSFXEventAssetRuleType>(3);
					plain::module_scope scope(3);
					write_physics_sfx_event_asset_angle_rule(native.u.angleRule, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::physics_events::physics_sfx_event_asset_speed_rule>)
				{
					native.type = static_cast<PhysicsSFXEventAssetRuleType>(4);
					plain::module_scope scope(4);
					write_physics_sfx_event_asset_speed_rule(native.u.speedRule, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::physics_events::physics_sfx_event_asset_mass_rule>)
				{
					native.type = static_cast<PhysicsSFXEventAssetRuleType>(5);
					plain::module_scope scope(5);
					write_physics_sfx_event_asset_mass_rule(native.u.massRule, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::physics_events::physics_sfx_event_asset_gravity_rule>)
				{
					native.type = static_cast<PhysicsSFXEventAssetRuleType>(6);
					plain::module_scope scope(6);
					write_physics_sfx_event_asset_gravity_rule(native.u.gravityRule, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::physics_events::physics_sfx_event_asset_atmosphere_rule>)
				{
					native.type = static_cast<PhysicsSFXEventAssetRuleType>(7);
					plain::module_scope scope(7);
					write_physics_sfx_event_asset_atmosphere_rule(native.u.atmosphereRule, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::physics_events::physics_sfx_event_asset_camera_distance_rule>)
				{
					native.type = static_cast<PhysicsSFXEventAssetRuleType>(8);
					plain::module_scope scope(8);
					write_physics_sfx_event_asset_camera_distance_rule(native.u.cameraDistanceRule, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::physics_events::physics_sfx_event_asset_frequency_rule>)
				{
					native.type = static_cast<PhysicsSFXEventAssetRuleType>(9);
					plain::module_scope scope(9);
					write_physics_sfx_event_asset_frequency_rule(native.u.frequencyRule, value, resolve, memory);
				}
			}, data.u);
		}

		std::size_t size_physics_sfx_event_asset_rule([[maybe_unused]] const asset_data::physics_events::physics_sfx_event_asset_rule& data)
		{
			std::size_t size = 0;
			std::visit([&](const auto& value)
			{
				using value_type = std::decay_t<decltype(value)>;
				if constexpr (std::is_same_v<value_type, std::monostate>)
				{
				}
				else if constexpr (std::is_same_v<value_type, asset_data::physics_events::physics_sfx_event_asset_sound_rule>)
				{
					size += size_physics_sfx_event_asset_sound_rule(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::physics_events::physics_sfx_event_asset_material_rule>)
				{
					size += size_physics_sfx_event_asset_material_rule(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::physics_events::physics_sfx_event_asset_angle_rule>)
				{
					size += size_physics_sfx_event_asset_angle_rule(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::physics_events::physics_sfx_event_asset_speed_rule>)
				{
					size += size_physics_sfx_event_asset_speed_rule(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::physics_events::physics_sfx_event_asset_mass_rule>)
				{
					size += size_physics_sfx_event_asset_mass_rule(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::physics_events::physics_sfx_event_asset_gravity_rule>)
				{
					size += size_physics_sfx_event_asset_gravity_rule(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::physics_events::physics_sfx_event_asset_atmosphere_rule>)
				{
					size += size_physics_sfx_event_asset_atmosphere_rule(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::physics_events::physics_sfx_event_asset_camera_distance_rule>)
				{
					size += size_physics_sfx_event_asset_camera_distance_rule(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::physics_events::physics_sfx_event_asset_frequency_rule>)
				{
					size += size_physics_sfx_event_asset_frequency_rule(value);
				}
			}, data.u);
			return size;
		}

		asset_data::physics_events::physics_sfx_event_asset_material_entry read_physics_sfx_event_asset_material_entry(const PhysicsSFXEventAssetMaterialEntry& native)
		{
			plain::trace("read_physics_sfx_event_asset_material_entry");
			asset_data::physics_events::physics_sfx_event_asset_material_entry data;
			data.surface_type = native.surfaceType;
			data.rules = read_physics_sfx_event_asset_rules(native.rules);
			return data;
		}

		void write_physics_sfx_event_asset_material_entry(PhysicsSFXEventAssetMaterialEntry& native, const asset_data::physics_events::physics_sfx_event_asset_material_entry& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.surfaceType = data.surface_type;
			write_physics_sfx_event_asset_rules(native.rules, data.rules, resolve, memory);
		}

		std::size_t size_physics_sfx_event_asset_material_entry([[maybe_unused]] const asset_data::physics_events::physics_sfx_event_asset_material_entry& data)
		{
			std::size_t size = 0;
			size += size_physics_sfx_event_asset_rules(data.rules);
			return size;
		}

		asset_data::physics_events::physics_sfx_event_asset_angle_entry read_physics_sfx_event_asset_angle_entry(const PhysicsSFXEventAssetAngleEntry& native)
		{
			plain::trace("read_physics_sfx_event_asset_angle_entry");
			asset_data::physics_events::physics_sfx_event_asset_angle_entry data;
			data.angle = native.angle;
			data.hit_sound_volume_modifier_min = native.hitSoundVolumeModifierMin;
			data.hit_sound_volume_modifier_max = native.hitSoundVolumeModifierMax;
			data.hit_sound_pitch_modifier_min = native.hitSoundPitchModifierMin;
			data.hit_sound_pitch_modifier_max = native.hitSoundPitchModifierMax;
			data.rules = read_physics_sfx_event_asset_rules(native.rules);
			return data;
		}

		void write_physics_sfx_event_asset_angle_entry(PhysicsSFXEventAssetAngleEntry& native, const asset_data::physics_events::physics_sfx_event_asset_angle_entry& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.angle = data.angle;
			native.hitSoundVolumeModifierMin = data.hit_sound_volume_modifier_min;
			native.hitSoundVolumeModifierMax = data.hit_sound_volume_modifier_max;
			native.hitSoundPitchModifierMin = data.hit_sound_pitch_modifier_min;
			native.hitSoundPitchModifierMax = data.hit_sound_pitch_modifier_max;
			write_physics_sfx_event_asset_rules(native.rules, data.rules, resolve, memory);
		}

		std::size_t size_physics_sfx_event_asset_angle_entry([[maybe_unused]] const asset_data::physics_events::physics_sfx_event_asset_angle_entry& data)
		{
			std::size_t size = 0;
			size += size_physics_sfx_event_asset_rules(data.rules);
			return size;
		}

		asset_data::physics_events::physics_sfx_event_asset_speed_entry read_physics_sfx_event_asset_speed_entry(const PhysicsSFXEventAssetSpeedEntry& native)
		{
			plain::trace("read_physics_sfx_event_asset_speed_entry");
			asset_data::physics_events::physics_sfx_event_asset_speed_entry data;
			data.speed_threshold = native.speedThreshold;
			data.hit_sound_volume_modifier_min = native.hitSoundVolumeModifierMin;
			data.hit_sound_volume_modifier_max = native.hitSoundVolumeModifierMax;
			data.hit_sound_pitch_modifier_min = native.hitSoundPitchModifierMin;
			data.hit_sound_pitch_modifier_max = native.hitSoundPitchModifierMax;
			data.rules = read_physics_sfx_event_asset_rules(native.rules);
			return data;
		}

		void write_physics_sfx_event_asset_speed_entry(PhysicsSFXEventAssetSpeedEntry& native, const asset_data::physics_events::physics_sfx_event_asset_speed_entry& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.speedThreshold = data.speed_threshold;
			native.hitSoundVolumeModifierMin = data.hit_sound_volume_modifier_min;
			native.hitSoundVolumeModifierMax = data.hit_sound_volume_modifier_max;
			native.hitSoundPitchModifierMin = data.hit_sound_pitch_modifier_min;
			native.hitSoundPitchModifierMax = data.hit_sound_pitch_modifier_max;
			write_physics_sfx_event_asset_rules(native.rules, data.rules, resolve, memory);
		}

		std::size_t size_physics_sfx_event_asset_speed_entry([[maybe_unused]] const asset_data::physics_events::physics_sfx_event_asset_speed_entry& data)
		{
			std::size_t size = 0;
			size += size_physics_sfx_event_asset_rules(data.rules);
			return size;
		}

		asset_data::physics_events::physics_sfx_event_asset_mass_entry read_physics_sfx_event_asset_mass_entry(const PhysicsSFXEventAssetMassEntry& native)
		{
			plain::trace("read_physics_sfx_event_asset_mass_entry");
			asset_data::physics_events::physics_sfx_event_asset_mass_entry data;
			data.mass_threshold = native.massThreshold;
			data.hit_sound_volume_modifier_min = native.hitSoundVolumeModifierMin;
			data.hit_sound_volume_modifier_max = native.hitSoundVolumeModifierMax;
			data.hit_sound_pitch_modifier_min = native.hitSoundPitchModifierMin;
			data.hit_sound_pitch_modifier_max = native.hitSoundPitchModifierMax;
			data.rules = read_physics_sfx_event_asset_rules(native.rules);
			return data;
		}

		void write_physics_sfx_event_asset_mass_entry(PhysicsSFXEventAssetMassEntry& native, const asset_data::physics_events::physics_sfx_event_asset_mass_entry& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.massThreshold = data.mass_threshold;
			native.hitSoundVolumeModifierMin = data.hit_sound_volume_modifier_min;
			native.hitSoundVolumeModifierMax = data.hit_sound_volume_modifier_max;
			native.hitSoundPitchModifierMin = data.hit_sound_pitch_modifier_min;
			native.hitSoundPitchModifierMax = data.hit_sound_pitch_modifier_max;
			write_physics_sfx_event_asset_rules(native.rules, data.rules, resolve, memory);
		}

		std::size_t size_physics_sfx_event_asset_mass_entry([[maybe_unused]] const asset_data::physics_events::physics_sfx_event_asset_mass_entry& data)
		{
			std::size_t size = 0;
			size += size_physics_sfx_event_asset_rules(data.rules);
			return size;
		}

		asset_data::physics_events::physics_sfx_event_asset_gravity_entry read_physics_sfx_event_asset_gravity_entry(const PhysicsSFXEventAssetGravityEntry& native)
		{
			plain::trace("read_physics_sfx_event_asset_gravity_entry");
			asset_data::physics_events::physics_sfx_event_asset_gravity_entry data;
			data.gravity = native.gravity;
			data.rules = read_physics_sfx_event_asset_rules(native.rules);
			return data;
		}

		void write_physics_sfx_event_asset_gravity_entry(PhysicsSFXEventAssetGravityEntry& native, const asset_data::physics_events::physics_sfx_event_asset_gravity_entry& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.gravity = data.gravity;
			write_physics_sfx_event_asset_rules(native.rules, data.rules, resolve, memory);
		}

		std::size_t size_physics_sfx_event_asset_gravity_entry([[maybe_unused]] const asset_data::physics_events::physics_sfx_event_asset_gravity_entry& data)
		{
			std::size_t size = 0;
			size += size_physics_sfx_event_asset_rules(data.rules);
			return size;
		}

		asset_data::physics_events::physics_sfx_event_asset_atmosphere_entry read_physics_sfx_event_asset_atmosphere_entry(const PhysicsSFXEventAssetAtmosphereEntry& native)
		{
			plain::trace("read_physics_sfx_event_asset_atmosphere_entry");
			asset_data::physics_events::physics_sfx_event_asset_atmosphere_entry data;
			data.atmosphere = native.atmosphere;
			data.rules = read_physics_sfx_event_asset_rules(native.rules);
			return data;
		}

		void write_physics_sfx_event_asset_atmosphere_entry(PhysicsSFXEventAssetAtmosphereEntry& native, const asset_data::physics_events::physics_sfx_event_asset_atmosphere_entry& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.atmosphere = data.atmosphere;
			write_physics_sfx_event_asset_rules(native.rules, data.rules, resolve, memory);
		}

		std::size_t size_physics_sfx_event_asset_atmosphere_entry([[maybe_unused]] const asset_data::physics_events::physics_sfx_event_asset_atmosphere_entry& data)
		{
			std::size_t size = 0;
			size += size_physics_sfx_event_asset_rules(data.rules);
			return size;
		}

		asset_data::physics_events::physics_sfx_event_asset_camera_distance_entry read_physics_sfx_event_asset_camera_distance_entry(const PhysicsSFXEventAssetCameraDistanceEntry& native)
		{
			plain::trace("read_physics_sfx_event_asset_camera_distance_entry");
			asset_data::physics_events::physics_sfx_event_asset_camera_distance_entry data;
			data.distance = native.distance;
			data.rules = read_physics_sfx_event_asset_rules(native.rules);
			return data;
		}

		void write_physics_sfx_event_asset_camera_distance_entry(PhysicsSFXEventAssetCameraDistanceEntry& native, const asset_data::physics_events::physics_sfx_event_asset_camera_distance_entry& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.distance = data.distance;
			write_physics_sfx_event_asset_rules(native.rules, data.rules, resolve, memory);
		}

		std::size_t size_physics_sfx_event_asset_camera_distance_entry([[maybe_unused]] const asset_data::physics_events::physics_sfx_event_asset_camera_distance_entry& data)
		{
			std::size_t size = 0;
			size += size_physics_sfx_event_asset_rules(data.rules);
			return size;
		}

		asset_data::physics_events::physics_sfx_event_asset_frequency_entry read_physics_sfx_event_asset_frequency_entry(const PhysicsSFXEventAssetFrequencyEntry& native)
		{
			plain::trace("read_physics_sfx_event_asset_frequency_entry");
			asset_data::physics_events::physics_sfx_event_asset_frequency_entry data;
			data.max_events = native.maxEvents;
			data.rules = read_physics_sfx_event_asset_rules(native.rules);
			return data;
		}

		void write_physics_sfx_event_asset_frequency_entry(PhysicsSFXEventAssetFrequencyEntry& native, const asset_data::physics_events::physics_sfx_event_asset_frequency_entry& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.maxEvents = data.max_events;
			write_physics_sfx_event_asset_rules(native.rules, data.rules, resolve, memory);
		}

		std::size_t size_physics_sfx_event_asset_frequency_entry([[maybe_unused]] const asset_data::physics_events::physics_sfx_event_asset_frequency_entry& data)
		{
			std::size_t size = 0;
			size += size_physics_sfx_event_asset_rules(data.rules);
			return size;
		}

		asset_data::physics_events::physics_vfx_event_asset_rules read_physics_vfx_event_asset_rules(const PhysicsVFXEventAssetRules& native)
		{
			plain::trace("read_physics_vfx_event_asset_rules");
			asset_data::physics_events::physics_vfx_event_asset_rules data;
			data.numrules = native.numrules;
			data.rules = plain::read_array(native.rules, native.numrules, "PhysicsVFXEventAssetRules.rules", [&](const auto& item) { return read_physics_vfx_event_asset_rule(item); });
			return data;
		}

		void write_physics_vfx_event_asset_rules(PhysicsVFXEventAssetRules& native, const asset_data::physics_events::physics_vfx_event_asset_rules& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.numrules = data.numrules;
			native.rules = plain::write_array<PhysicsVFXEventAssetRule>(data.rules, memory, [&](auto& out, const auto& in) { write_physics_vfx_event_asset_rule(out, in, resolve, memory); });
		}

		std::size_t size_physics_vfx_event_asset_rules([[maybe_unused]] const asset_data::physics_events::physics_vfx_event_asset_rules& data)
		{
			std::size_t size = 0;
			if (data.rules)
			{
				size += plain::sized(std::max<std::size_t>(data.rules->size(), 1) * sizeof(PhysicsVFXEventAssetRule));
				for (const auto& item : *data.rules)
				{
					size += size_physics_vfx_event_asset_rule(item);
				}
			}
			return size;
		}

		asset_data::physics_events::physics_vfx_event read_physics_vfx_event(const PhysicsVFXEventAsset& native)
		{
			plain::trace("read_physics_vfx_event");
			asset_data::physics_events::physics_vfx_event data;
			data.name = plain::read_string(native.name);
			data.priority = native.priority;
			data.rules = read_physics_vfx_event_asset_rules(native.rules);
			return data;
		}

		void write_physics_vfx_event(PhysicsVFXEventAsset& native, const asset_data::physics_events::physics_vfx_event& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.priority = data.priority;
			write_physics_vfx_event_asset_rules(native.rules, data.rules, resolve, memory);
		}

		std::size_t size_physics_vfx_event([[maybe_unused]] const asset_data::physics_events::physics_vfx_event& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			size += size_physics_vfx_event_asset_rules(data.rules);
			return size;
		}

		asset_data::physics_events::physics_vfx_event_asset_particle_effect_rule read_physics_vfx_event_asset_particle_effect_rule(const PhysicsVFXEventAssetParticleEffectRule& native)
		{
			plain::trace("read_physics_vfx_event_asset_particle_effect_rule");
			asset_data::physics_events::physics_vfx_event_asset_particle_effect_rule data;
			data.hit_particle_effect_alias = plain::read_string(native.hitParticleEffectAlias);
			data.scrape_particle_effect_alias = plain::read_string(native.scrapeParticleEffectAlias);
			return data;
		}

		void write_physics_vfx_event_asset_particle_effect_rule(PhysicsVFXEventAssetParticleEffectRule& native, const asset_data::physics_events::physics_vfx_event_asset_particle_effect_rule& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.hitParticleEffectAlias = plain::write_string(memory, data.hit_particle_effect_alias);
			native.scrapeParticleEffectAlias = plain::write_string(memory, data.scrape_particle_effect_alias);
		}

		std::size_t size_physics_vfx_event_asset_particle_effect_rule([[maybe_unused]] const asset_data::physics_events::physics_vfx_event_asset_particle_effect_rule& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.hit_particle_effect_alias);
			size += plain::string_size(data.scrape_particle_effect_alias);
			return size;
		}

		asset_data::physics_events::physics_vfx_event_asset_material_rule read_physics_vfx_event_asset_material_rule(const PhysicsVFXEventAssetMaterialRule& native)
		{
			plain::trace("read_physics_vfx_event_asset_material_rule");
			asset_data::physics_events::physics_vfx_event_asset_material_rule data;
			data.numentries = native.numentries;
			data.entries = plain::read_array(native.entries, native.numentries, "PhysicsVFXEventAssetMaterialRule.entries", [&](const auto& item) { return read_physics_vfx_event_asset_material_entry(item); });
			return data;
		}

		void write_physics_vfx_event_asset_material_rule(PhysicsVFXEventAssetMaterialRule& native, const asset_data::physics_events::physics_vfx_event_asset_material_rule& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.numentries = data.numentries;
			native.entries = plain::write_array<PhysicsVFXEventAssetMaterialEntry>(data.entries, memory, [&](auto& out, const auto& in) { write_physics_vfx_event_asset_material_entry(out, in, resolve, memory); });
		}

		std::size_t size_physics_vfx_event_asset_material_rule([[maybe_unused]] const asset_data::physics_events::physics_vfx_event_asset_material_rule& data)
		{
			std::size_t size = 0;
			if (data.entries)
			{
				size += plain::sized(std::max<std::size_t>(data.entries->size(), 1) * sizeof(PhysicsVFXEventAssetMaterialEntry));
				for (const auto& item : *data.entries)
				{
					size += size_physics_vfx_event_asset_material_entry(item);
				}
			}
			return size;
		}

		asset_data::physics_events::physics_vfx_event_asset_angle_rule read_physics_vfx_event_asset_angle_rule(const PhysicsVFXEventAssetAngleRule& native)
		{
			plain::trace("read_physics_vfx_event_asset_angle_rule");
			asset_data::physics_events::physics_vfx_event_asset_angle_rule data;
			data.numentries = native.numentries;
			data.entries = plain::read_array(native.entries, native.numentries, "PhysicsVFXEventAssetAngleRule.entries", [&](const auto& item) { return read_physics_vfx_event_asset_angle_entry(item); });
			return data;
		}

		void write_physics_vfx_event_asset_angle_rule(PhysicsVFXEventAssetAngleRule& native, const asset_data::physics_events::physics_vfx_event_asset_angle_rule& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.numentries = data.numentries;
			native.entries = plain::write_array<PhysicsVFXEventAssetAngleEntry>(data.entries, memory, [&](auto& out, const auto& in) { write_physics_vfx_event_asset_angle_entry(out, in, resolve, memory); });
		}

		std::size_t size_physics_vfx_event_asset_angle_rule([[maybe_unused]] const asset_data::physics_events::physics_vfx_event_asset_angle_rule& data)
		{
			std::size_t size = 0;
			if (data.entries)
			{
				size += plain::sized(std::max<std::size_t>(data.entries->size(), 1) * sizeof(PhysicsVFXEventAssetAngleEntry));
				for (const auto& item : *data.entries)
				{
					size += size_physics_vfx_event_asset_angle_entry(item);
				}
			}
			return size;
		}

		asset_data::physics_events::physics_vfx_event_asset_speed_rule read_physics_vfx_event_asset_speed_rule(const PhysicsVFXEventAssetSpeedRule& native)
		{
			plain::trace("read_physics_vfx_event_asset_speed_rule");
			asset_data::physics_events::physics_vfx_event_asset_speed_rule data;
			data.numentries = native.numentries;
			data.entries = plain::read_array(native.entries, native.numentries, "PhysicsVFXEventAssetSpeedRule.entries", [&](const auto& item) { return read_physics_vfx_event_asset_speed_entry(item); });
			return data;
		}

		void write_physics_vfx_event_asset_speed_rule(PhysicsVFXEventAssetSpeedRule& native, const asset_data::physics_events::physics_vfx_event_asset_speed_rule& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.numentries = data.numentries;
			native.entries = plain::write_array<PhysicsVFXEventAssetSpeedEntry>(data.entries, memory, [&](auto& out, const auto& in) { write_physics_vfx_event_asset_speed_entry(out, in, resolve, memory); });
		}

		std::size_t size_physics_vfx_event_asset_speed_rule([[maybe_unused]] const asset_data::physics_events::physics_vfx_event_asset_speed_rule& data)
		{
			std::size_t size = 0;
			if (data.entries)
			{
				size += plain::sized(std::max<std::size_t>(data.entries->size(), 1) * sizeof(PhysicsVFXEventAssetSpeedEntry));
				for (const auto& item : *data.entries)
				{
					size += size_physics_vfx_event_asset_speed_entry(item);
				}
			}
			return size;
		}

		asset_data::physics_events::physics_vfx_event_asset_mass_rule read_physics_vfx_event_asset_mass_rule(const PhysicsVFXEventAssetMassRule& native)
		{
			plain::trace("read_physics_vfx_event_asset_mass_rule");
			asset_data::physics_events::physics_vfx_event_asset_mass_rule data;
			data.numentries = native.numentries;
			data.entries = plain::read_array(native.entries, native.numentries, "PhysicsVFXEventAssetMassRule.entries", [&](const auto& item) { return read_physics_vfx_event_asset_mass_entry(item); });
			return data;
		}

		void write_physics_vfx_event_asset_mass_rule(PhysicsVFXEventAssetMassRule& native, const asset_data::physics_events::physics_vfx_event_asset_mass_rule& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.numentries = data.numentries;
			native.entries = plain::write_array<PhysicsVFXEventAssetMassEntry>(data.entries, memory, [&](auto& out, const auto& in) { write_physics_vfx_event_asset_mass_entry(out, in, resolve, memory); });
		}

		std::size_t size_physics_vfx_event_asset_mass_rule([[maybe_unused]] const asset_data::physics_events::physics_vfx_event_asset_mass_rule& data)
		{
			std::size_t size = 0;
			if (data.entries)
			{
				size += plain::sized(std::max<std::size_t>(data.entries->size(), 1) * sizeof(PhysicsVFXEventAssetMassEntry));
				for (const auto& item : *data.entries)
				{
					size += size_physics_vfx_event_asset_mass_entry(item);
				}
			}
			return size;
		}

		asset_data::physics_events::physics_vfx_event_asset_gravity_rule read_physics_vfx_event_asset_gravity_rule(const PhysicsVFXEventAssetGravityRule& native)
		{
			plain::trace("read_physics_vfx_event_asset_gravity_rule");
			asset_data::physics_events::physics_vfx_event_asset_gravity_rule data;
			data.numentries = native.numentries;
			data.entries = plain::read_array(native.entries, native.numentries, "PhysicsVFXEventAssetGravityRule.entries", [&](const auto& item) { return read_physics_vfx_event_asset_gravity_entry(item); });
			return data;
		}

		void write_physics_vfx_event_asset_gravity_rule(PhysicsVFXEventAssetGravityRule& native, const asset_data::physics_events::physics_vfx_event_asset_gravity_rule& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.numentries = data.numentries;
			native.entries = plain::write_array<PhysicsVFXEventAssetGravityEntry>(data.entries, memory, [&](auto& out, const auto& in) { write_physics_vfx_event_asset_gravity_entry(out, in, resolve, memory); });
		}

		std::size_t size_physics_vfx_event_asset_gravity_rule([[maybe_unused]] const asset_data::physics_events::physics_vfx_event_asset_gravity_rule& data)
		{
			std::size_t size = 0;
			if (data.entries)
			{
				size += plain::sized(std::max<std::size_t>(data.entries->size(), 1) * sizeof(PhysicsVFXEventAssetGravityEntry));
				for (const auto& item : *data.entries)
				{
					size += size_physics_vfx_event_asset_gravity_entry(item);
				}
			}
			return size;
		}

		asset_data::physics_events::physics_vfx_event_asset_atmosphere_rule read_physics_vfx_event_asset_atmosphere_rule(const PhysicsVFXEventAssetAtmosphereRule& native)
		{
			plain::trace("read_physics_vfx_event_asset_atmosphere_rule");
			asset_data::physics_events::physics_vfx_event_asset_atmosphere_rule data;
			data.numentries = native.numentries;
			data.entries = plain::read_array(native.entries, native.numentries, "PhysicsVFXEventAssetAtmosphereRule.entries", [&](const auto& item) { return read_physics_vfx_event_asset_atmosphere_entry(item); });
			return data;
		}

		void write_physics_vfx_event_asset_atmosphere_rule(PhysicsVFXEventAssetAtmosphereRule& native, const asset_data::physics_events::physics_vfx_event_asset_atmosphere_rule& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.numentries = data.numentries;
			native.entries = plain::write_array<PhysicsVFXEventAssetAtmosphereEntry>(data.entries, memory, [&](auto& out, const auto& in) { write_physics_vfx_event_asset_atmosphere_entry(out, in, resolve, memory); });
		}

		std::size_t size_physics_vfx_event_asset_atmosphere_rule([[maybe_unused]] const asset_data::physics_events::physics_vfx_event_asset_atmosphere_rule& data)
		{
			std::size_t size = 0;
			if (data.entries)
			{
				size += plain::sized(std::max<std::size_t>(data.entries->size(), 1) * sizeof(PhysicsVFXEventAssetAtmosphereEntry));
				for (const auto& item : *data.entries)
				{
					size += size_physics_vfx_event_asset_atmosphere_entry(item);
				}
			}
			return size;
		}

		asset_data::physics_events::physics_vfx_event_asset_camera_distance_rule read_physics_vfx_event_asset_camera_distance_rule(const PhysicsVFXEventAssetCameraDistanceRule& native)
		{
			plain::trace("read_physics_vfx_event_asset_camera_distance_rule");
			asset_data::physics_events::physics_vfx_event_asset_camera_distance_rule data;
			data.numentries = native.numentries;
			data.entries = plain::read_array(native.entries, native.numentries, "PhysicsVFXEventAssetCameraDistanceRule.entries", [&](const auto& item) { return read_physics_vfx_event_asset_camera_distance_entry(item); });
			return data;
		}

		void write_physics_vfx_event_asset_camera_distance_rule(PhysicsVFXEventAssetCameraDistanceRule& native, const asset_data::physics_events::physics_vfx_event_asset_camera_distance_rule& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.numentries = data.numentries;
			native.entries = plain::write_array<PhysicsVFXEventAssetCameraDistanceEntry>(data.entries, memory, [&](auto& out, const auto& in) { write_physics_vfx_event_asset_camera_distance_entry(out, in, resolve, memory); });
		}

		std::size_t size_physics_vfx_event_asset_camera_distance_rule([[maybe_unused]] const asset_data::physics_events::physics_vfx_event_asset_camera_distance_rule& data)
		{
			std::size_t size = 0;
			if (data.entries)
			{
				size += plain::sized(std::max<std::size_t>(data.entries->size(), 1) * sizeof(PhysicsVFXEventAssetCameraDistanceEntry));
				for (const auto& item : *data.entries)
				{
					size += size_physics_vfx_event_asset_camera_distance_entry(item);
				}
			}
			return size;
		}

		asset_data::physics_events::physics_vfx_event_asset_frequency_rule read_physics_vfx_event_asset_frequency_rule(const PhysicsVFXEventAssetFrequencyRule& native)
		{
			plain::trace("read_physics_vfx_event_asset_frequency_rule");
			asset_data::physics_events::physics_vfx_event_asset_frequency_rule data;
			data.numentries = native.numentries;
			data.entries = plain::read_array(native.entries, native.numentries, "PhysicsVFXEventAssetFrequencyRule.entries", [&](const auto& item) { return read_physics_vfx_event_asset_frequency_entry(item); });
			return data;
		}

		void write_physics_vfx_event_asset_frequency_rule(PhysicsVFXEventAssetFrequencyRule& native, const asset_data::physics_events::physics_vfx_event_asset_frequency_rule& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.numentries = data.numentries;
			native.entries = plain::write_array<PhysicsVFXEventAssetFrequencyEntry>(data.entries, memory, [&](auto& out, const auto& in) { write_physics_vfx_event_asset_frequency_entry(out, in, resolve, memory); });
		}

		std::size_t size_physics_vfx_event_asset_frequency_rule([[maybe_unused]] const asset_data::physics_events::physics_vfx_event_asset_frequency_rule& data)
		{
			std::size_t size = 0;
			if (data.entries)
			{
				size += plain::sized(std::max<std::size_t>(data.entries->size(), 1) * sizeof(PhysicsVFXEventAssetFrequencyEntry));
				for (const auto& item : *data.entries)
				{
					size += size_physics_vfx_event_asset_frequency_entry(item);
				}
			}
			return size;
		}

		asset_data::physics_events::physics_vfx_event_asset_rule read_physics_vfx_event_asset_rule(const PhysicsVFXEventAssetRule& native)
		{
			plain::trace("read_physics_vfx_event_asset_rule");
			asset_data::physics_events::physics_vfx_event_asset_rule data;
			data.u = [&]() -> std::variant<std::monostate, asset_data::physics_events::physics_vfx_event_asset_particle_effect_rule, asset_data::physics_events::physics_vfx_event_asset_material_rule, asset_data::physics_events::physics_vfx_event_asset_angle_rule, asset_data::physics_events::physics_vfx_event_asset_speed_rule, asset_data::physics_events::physics_vfx_event_asset_mass_rule, asset_data::physics_events::physics_vfx_event_asset_gravity_rule, asset_data::physics_events::physics_vfx_event_asset_atmosphere_rule, asset_data::physics_events::physics_vfx_event_asset_camera_distance_rule, asset_data::physics_events::physics_vfx_event_asset_frequency_rule> { switch (static_cast<std::uint32_t>(native.type)) { case 1: { plain::module_scope scope(1); auto item = read_physics_vfx_event_asset_particle_effect_rule(native.u.particleEffectRule); return std::variant<std::monostate, asset_data::physics_events::physics_vfx_event_asset_particle_effect_rule, asset_data::physics_events::physics_vfx_event_asset_material_rule, asset_data::physics_events::physics_vfx_event_asset_angle_rule, asset_data::physics_events::physics_vfx_event_asset_speed_rule, asset_data::physics_events::physics_vfx_event_asset_mass_rule, asset_data::physics_events::physics_vfx_event_asset_gravity_rule, asset_data::physics_events::physics_vfx_event_asset_atmosphere_rule, asset_data::physics_events::physics_vfx_event_asset_camera_distance_rule, asset_data::physics_events::physics_vfx_event_asset_frequency_rule>(std::move(item)); } case 2: { plain::module_scope scope(2); auto item = read_physics_vfx_event_asset_material_rule(native.u.materialRule); return std::variant<std::monostate, asset_data::physics_events::physics_vfx_event_asset_particle_effect_rule, asset_data::physics_events::physics_vfx_event_asset_material_rule, asset_data::physics_events::physics_vfx_event_asset_angle_rule, asset_data::physics_events::physics_vfx_event_asset_speed_rule, asset_data::physics_events::physics_vfx_event_asset_mass_rule, asset_data::physics_events::physics_vfx_event_asset_gravity_rule, asset_data::physics_events::physics_vfx_event_asset_atmosphere_rule, asset_data::physics_events::physics_vfx_event_asset_camera_distance_rule, asset_data::physics_events::physics_vfx_event_asset_frequency_rule>(std::move(item)); } case 3: { plain::module_scope scope(3); auto item = read_physics_vfx_event_asset_angle_rule(native.u.angleRule); return std::variant<std::monostate, asset_data::physics_events::physics_vfx_event_asset_particle_effect_rule, asset_data::physics_events::physics_vfx_event_asset_material_rule, asset_data::physics_events::physics_vfx_event_asset_angle_rule, asset_data::physics_events::physics_vfx_event_asset_speed_rule, asset_data::physics_events::physics_vfx_event_asset_mass_rule, asset_data::physics_events::physics_vfx_event_asset_gravity_rule, asset_data::physics_events::physics_vfx_event_asset_atmosphere_rule, asset_data::physics_events::physics_vfx_event_asset_camera_distance_rule, asset_data::physics_events::physics_vfx_event_asset_frequency_rule>(std::move(item)); } case 4: { plain::module_scope scope(4); auto item = read_physics_vfx_event_asset_speed_rule(native.u.speedRule); return std::variant<std::monostate, asset_data::physics_events::physics_vfx_event_asset_particle_effect_rule, asset_data::physics_events::physics_vfx_event_asset_material_rule, asset_data::physics_events::physics_vfx_event_asset_angle_rule, asset_data::physics_events::physics_vfx_event_asset_speed_rule, asset_data::physics_events::physics_vfx_event_asset_mass_rule, asset_data::physics_events::physics_vfx_event_asset_gravity_rule, asset_data::physics_events::physics_vfx_event_asset_atmosphere_rule, asset_data::physics_events::physics_vfx_event_asset_camera_distance_rule, asset_data::physics_events::physics_vfx_event_asset_frequency_rule>(std::move(item)); } case 5: { plain::module_scope scope(5); auto item = read_physics_vfx_event_asset_mass_rule(native.u.massRule); return std::variant<std::monostate, asset_data::physics_events::physics_vfx_event_asset_particle_effect_rule, asset_data::physics_events::physics_vfx_event_asset_material_rule, asset_data::physics_events::physics_vfx_event_asset_angle_rule, asset_data::physics_events::physics_vfx_event_asset_speed_rule, asset_data::physics_events::physics_vfx_event_asset_mass_rule, asset_data::physics_events::physics_vfx_event_asset_gravity_rule, asset_data::physics_events::physics_vfx_event_asset_atmosphere_rule, asset_data::physics_events::physics_vfx_event_asset_camera_distance_rule, asset_data::physics_events::physics_vfx_event_asset_frequency_rule>(std::move(item)); } case 6: { plain::module_scope scope(6); auto item = read_physics_vfx_event_asset_gravity_rule(native.u.gravityRule); return std::variant<std::monostate, asset_data::physics_events::physics_vfx_event_asset_particle_effect_rule, asset_data::physics_events::physics_vfx_event_asset_material_rule, asset_data::physics_events::physics_vfx_event_asset_angle_rule, asset_data::physics_events::physics_vfx_event_asset_speed_rule, asset_data::physics_events::physics_vfx_event_asset_mass_rule, asset_data::physics_events::physics_vfx_event_asset_gravity_rule, asset_data::physics_events::physics_vfx_event_asset_atmosphere_rule, asset_data::physics_events::physics_vfx_event_asset_camera_distance_rule, asset_data::physics_events::physics_vfx_event_asset_frequency_rule>(std::move(item)); } case 7: { plain::module_scope scope(7); auto item = read_physics_vfx_event_asset_atmosphere_rule(native.u.atmosphereRule); return std::variant<std::monostate, asset_data::physics_events::physics_vfx_event_asset_particle_effect_rule, asset_data::physics_events::physics_vfx_event_asset_material_rule, asset_data::physics_events::physics_vfx_event_asset_angle_rule, asset_data::physics_events::physics_vfx_event_asset_speed_rule, asset_data::physics_events::physics_vfx_event_asset_mass_rule, asset_data::physics_events::physics_vfx_event_asset_gravity_rule, asset_data::physics_events::physics_vfx_event_asset_atmosphere_rule, asset_data::physics_events::physics_vfx_event_asset_camera_distance_rule, asset_data::physics_events::physics_vfx_event_asset_frequency_rule>(std::move(item)); } case 8: { plain::module_scope scope(8); auto item = read_physics_vfx_event_asset_camera_distance_rule(native.u.cameraDistanceRule); return std::variant<std::monostate, asset_data::physics_events::physics_vfx_event_asset_particle_effect_rule, asset_data::physics_events::physics_vfx_event_asset_material_rule, asset_data::physics_events::physics_vfx_event_asset_angle_rule, asset_data::physics_events::physics_vfx_event_asset_speed_rule, asset_data::physics_events::physics_vfx_event_asset_mass_rule, asset_data::physics_events::physics_vfx_event_asset_gravity_rule, asset_data::physics_events::physics_vfx_event_asset_atmosphere_rule, asset_data::physics_events::physics_vfx_event_asset_camera_distance_rule, asset_data::physics_events::physics_vfx_event_asset_frequency_rule>(std::move(item)); } case 9: { plain::module_scope scope(9); auto item = read_physics_vfx_event_asset_frequency_rule(native.u.frequencyRule); return std::variant<std::monostate, asset_data::physics_events::physics_vfx_event_asset_particle_effect_rule, asset_data::physics_events::physics_vfx_event_asset_material_rule, asset_data::physics_events::physics_vfx_event_asset_angle_rule, asset_data::physics_events::physics_vfx_event_asset_speed_rule, asset_data::physics_events::physics_vfx_event_asset_mass_rule, asset_data::physics_events::physics_vfx_event_asset_gravity_rule, asset_data::physics_events::physics_vfx_event_asset_atmosphere_rule, asset_data::physics_events::physics_vfx_event_asset_camera_distance_rule, asset_data::physics_events::physics_vfx_event_asset_frequency_rule>(std::move(item)); } default: plain::fail("PhysicsVFXEventAssetRule.u", "has a tag value without a case"); } }();
			return data;
		}

		void write_physics_vfx_event_asset_rule(PhysicsVFXEventAssetRule& native, const asset_data::physics_events::physics_vfx_event_asset_rule& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			std::visit([&](const auto& value)
			{
				using value_type = std::decay_t<decltype(value)>;
				if constexpr (std::is_same_v<value_type, std::monostate>)
				{
					plain::fail("PhysicsVFXEventAssetRule.u", "holds no module");
				}
				else if constexpr (std::is_same_v<value_type, asset_data::physics_events::physics_vfx_event_asset_particle_effect_rule>)
				{
					native.type = static_cast<PhysicsVFXEventAssetRuleType>(1);
					plain::module_scope scope(1);
					write_physics_vfx_event_asset_particle_effect_rule(native.u.particleEffectRule, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::physics_events::physics_vfx_event_asset_material_rule>)
				{
					native.type = static_cast<PhysicsVFXEventAssetRuleType>(2);
					plain::module_scope scope(2);
					write_physics_vfx_event_asset_material_rule(native.u.materialRule, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::physics_events::physics_vfx_event_asset_angle_rule>)
				{
					native.type = static_cast<PhysicsVFXEventAssetRuleType>(3);
					plain::module_scope scope(3);
					write_physics_vfx_event_asset_angle_rule(native.u.angleRule, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::physics_events::physics_vfx_event_asset_speed_rule>)
				{
					native.type = static_cast<PhysicsVFXEventAssetRuleType>(4);
					plain::module_scope scope(4);
					write_physics_vfx_event_asset_speed_rule(native.u.speedRule, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::physics_events::physics_vfx_event_asset_mass_rule>)
				{
					native.type = static_cast<PhysicsVFXEventAssetRuleType>(5);
					plain::module_scope scope(5);
					write_physics_vfx_event_asset_mass_rule(native.u.massRule, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::physics_events::physics_vfx_event_asset_gravity_rule>)
				{
					native.type = static_cast<PhysicsVFXEventAssetRuleType>(6);
					plain::module_scope scope(6);
					write_physics_vfx_event_asset_gravity_rule(native.u.gravityRule, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::physics_events::physics_vfx_event_asset_atmosphere_rule>)
				{
					native.type = static_cast<PhysicsVFXEventAssetRuleType>(7);
					plain::module_scope scope(7);
					write_physics_vfx_event_asset_atmosphere_rule(native.u.atmosphereRule, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::physics_events::physics_vfx_event_asset_camera_distance_rule>)
				{
					native.type = static_cast<PhysicsVFXEventAssetRuleType>(8);
					plain::module_scope scope(8);
					write_physics_vfx_event_asset_camera_distance_rule(native.u.cameraDistanceRule, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::physics_events::physics_vfx_event_asset_frequency_rule>)
				{
					native.type = static_cast<PhysicsVFXEventAssetRuleType>(9);
					plain::module_scope scope(9);
					write_physics_vfx_event_asset_frequency_rule(native.u.frequencyRule, value, resolve, memory);
				}
			}, data.u);
		}

		std::size_t size_physics_vfx_event_asset_rule([[maybe_unused]] const asset_data::physics_events::physics_vfx_event_asset_rule& data)
		{
			std::size_t size = 0;
			std::visit([&](const auto& value)
			{
				using value_type = std::decay_t<decltype(value)>;
				if constexpr (std::is_same_v<value_type, std::monostate>)
				{
				}
				else if constexpr (std::is_same_v<value_type, asset_data::physics_events::physics_vfx_event_asset_particle_effect_rule>)
				{
					size += size_physics_vfx_event_asset_particle_effect_rule(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::physics_events::physics_vfx_event_asset_material_rule>)
				{
					size += size_physics_vfx_event_asset_material_rule(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::physics_events::physics_vfx_event_asset_angle_rule>)
				{
					size += size_physics_vfx_event_asset_angle_rule(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::physics_events::physics_vfx_event_asset_speed_rule>)
				{
					size += size_physics_vfx_event_asset_speed_rule(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::physics_events::physics_vfx_event_asset_mass_rule>)
				{
					size += size_physics_vfx_event_asset_mass_rule(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::physics_events::physics_vfx_event_asset_gravity_rule>)
				{
					size += size_physics_vfx_event_asset_gravity_rule(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::physics_events::physics_vfx_event_asset_atmosphere_rule>)
				{
					size += size_physics_vfx_event_asset_atmosphere_rule(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::physics_events::physics_vfx_event_asset_camera_distance_rule>)
				{
					size += size_physics_vfx_event_asset_camera_distance_rule(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::physics_events::physics_vfx_event_asset_frequency_rule>)
				{
					size += size_physics_vfx_event_asset_frequency_rule(value);
				}
			}, data.u);
			return size;
		}

		asset_data::physics_events::physics_vfx_event_asset_material_entry read_physics_vfx_event_asset_material_entry(const PhysicsVFXEventAssetMaterialEntry& native)
		{
			plain::trace("read_physics_vfx_event_asset_material_entry");
			asset_data::physics_events::physics_vfx_event_asset_material_entry data;
			data.surface_type = native.surfaceType;
			data.rules = read_physics_vfx_event_asset_rules(native.rules);
			return data;
		}

		void write_physics_vfx_event_asset_material_entry(PhysicsVFXEventAssetMaterialEntry& native, const asset_data::physics_events::physics_vfx_event_asset_material_entry& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.surfaceType = data.surface_type;
			write_physics_vfx_event_asset_rules(native.rules, data.rules, resolve, memory);
		}

		std::size_t size_physics_vfx_event_asset_material_entry([[maybe_unused]] const asset_data::physics_events::physics_vfx_event_asset_material_entry& data)
		{
			std::size_t size = 0;
			size += size_physics_vfx_event_asset_rules(data.rules);
			return size;
		}

		asset_data::physics_events::physics_vfx_event_asset_angle_entry read_physics_vfx_event_asset_angle_entry(const PhysicsVFXEventAssetAngleEntry& native)
		{
			plain::trace("read_physics_vfx_event_asset_angle_entry");
			asset_data::physics_events::physics_vfx_event_asset_angle_entry data;
			data.angle = native.angle;
			data.rules = read_physics_vfx_event_asset_rules(native.rules);
			return data;
		}

		void write_physics_vfx_event_asset_angle_entry(PhysicsVFXEventAssetAngleEntry& native, const asset_data::physics_events::physics_vfx_event_asset_angle_entry& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.angle = data.angle;
			write_physics_vfx_event_asset_rules(native.rules, data.rules, resolve, memory);
		}

		std::size_t size_physics_vfx_event_asset_angle_entry([[maybe_unused]] const asset_data::physics_events::physics_vfx_event_asset_angle_entry& data)
		{
			std::size_t size = 0;
			size += size_physics_vfx_event_asset_rules(data.rules);
			return size;
		}

		asset_data::physics_events::physics_vfx_event_asset_speed_entry read_physics_vfx_event_asset_speed_entry(const PhysicsVFXEventAssetSpeedEntry& native)
		{
			plain::trace("read_physics_vfx_event_asset_speed_entry");
			asset_data::physics_events::physics_vfx_event_asset_speed_entry data;
			data.speed_threshold = native.speedThreshold;
			data.rules = read_physics_vfx_event_asset_rules(native.rules);
			return data;
		}

		void write_physics_vfx_event_asset_speed_entry(PhysicsVFXEventAssetSpeedEntry& native, const asset_data::physics_events::physics_vfx_event_asset_speed_entry& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.speedThreshold = data.speed_threshold;
			write_physics_vfx_event_asset_rules(native.rules, data.rules, resolve, memory);
		}

		std::size_t size_physics_vfx_event_asset_speed_entry([[maybe_unused]] const asset_data::physics_events::physics_vfx_event_asset_speed_entry& data)
		{
			std::size_t size = 0;
			size += size_physics_vfx_event_asset_rules(data.rules);
			return size;
		}

		asset_data::physics_events::physics_vfx_event_asset_mass_entry read_physics_vfx_event_asset_mass_entry(const PhysicsVFXEventAssetMassEntry& native)
		{
			plain::trace("read_physics_vfx_event_asset_mass_entry");
			asset_data::physics_events::physics_vfx_event_asset_mass_entry data;
			data.mass_threshold = native.massThreshold;
			data.rules = read_physics_vfx_event_asset_rules(native.rules);
			return data;
		}

		void write_physics_vfx_event_asset_mass_entry(PhysicsVFXEventAssetMassEntry& native, const asset_data::physics_events::physics_vfx_event_asset_mass_entry& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.massThreshold = data.mass_threshold;
			write_physics_vfx_event_asset_rules(native.rules, data.rules, resolve, memory);
		}

		std::size_t size_physics_vfx_event_asset_mass_entry([[maybe_unused]] const asset_data::physics_events::physics_vfx_event_asset_mass_entry& data)
		{
			std::size_t size = 0;
			size += size_physics_vfx_event_asset_rules(data.rules);
			return size;
		}

		asset_data::physics_events::physics_vfx_event_asset_gravity_entry read_physics_vfx_event_asset_gravity_entry(const PhysicsVFXEventAssetGravityEntry& native)
		{
			plain::trace("read_physics_vfx_event_asset_gravity_entry");
			asset_data::physics_events::physics_vfx_event_asset_gravity_entry data;
			data.gravity = native.gravity;
			data.rules = read_physics_vfx_event_asset_rules(native.rules);
			return data;
		}

		void write_physics_vfx_event_asset_gravity_entry(PhysicsVFXEventAssetGravityEntry& native, const asset_data::physics_events::physics_vfx_event_asset_gravity_entry& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.gravity = data.gravity;
			write_physics_vfx_event_asset_rules(native.rules, data.rules, resolve, memory);
		}

		std::size_t size_physics_vfx_event_asset_gravity_entry([[maybe_unused]] const asset_data::physics_events::physics_vfx_event_asset_gravity_entry& data)
		{
			std::size_t size = 0;
			size += size_physics_vfx_event_asset_rules(data.rules);
			return size;
		}

		asset_data::physics_events::physics_vfx_event_asset_atmosphere_entry read_physics_vfx_event_asset_atmosphere_entry(const PhysicsVFXEventAssetAtmosphereEntry& native)
		{
			plain::trace("read_physics_vfx_event_asset_atmosphere_entry");
			asset_data::physics_events::physics_vfx_event_asset_atmosphere_entry data;
			data.atmosphere = native.atmosphere;
			data.rules = read_physics_vfx_event_asset_rules(native.rules);
			return data;
		}

		void write_physics_vfx_event_asset_atmosphere_entry(PhysicsVFXEventAssetAtmosphereEntry& native, const asset_data::physics_events::physics_vfx_event_asset_atmosphere_entry& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.atmosphere = data.atmosphere;
			write_physics_vfx_event_asset_rules(native.rules, data.rules, resolve, memory);
		}

		std::size_t size_physics_vfx_event_asset_atmosphere_entry([[maybe_unused]] const asset_data::physics_events::physics_vfx_event_asset_atmosphere_entry& data)
		{
			std::size_t size = 0;
			size += size_physics_vfx_event_asset_rules(data.rules);
			return size;
		}

		asset_data::physics_events::physics_vfx_event_asset_camera_distance_entry read_physics_vfx_event_asset_camera_distance_entry(const PhysicsVFXEventAssetCameraDistanceEntry& native)
		{
			plain::trace("read_physics_vfx_event_asset_camera_distance_entry");
			asset_data::physics_events::physics_vfx_event_asset_camera_distance_entry data;
			data.distance = native.distance;
			data.rules = read_physics_vfx_event_asset_rules(native.rules);
			return data;
		}

		void write_physics_vfx_event_asset_camera_distance_entry(PhysicsVFXEventAssetCameraDistanceEntry& native, const asset_data::physics_events::physics_vfx_event_asset_camera_distance_entry& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.distance = data.distance;
			write_physics_vfx_event_asset_rules(native.rules, data.rules, resolve, memory);
		}

		std::size_t size_physics_vfx_event_asset_camera_distance_entry([[maybe_unused]] const asset_data::physics_events::physics_vfx_event_asset_camera_distance_entry& data)
		{
			std::size_t size = 0;
			size += size_physics_vfx_event_asset_rules(data.rules);
			return size;
		}

		asset_data::physics_events::physics_vfx_event_asset_frequency_entry read_physics_vfx_event_asset_frequency_entry(const PhysicsVFXEventAssetFrequencyEntry& native)
		{
			plain::trace("read_physics_vfx_event_asset_frequency_entry");
			asset_data::physics_events::physics_vfx_event_asset_frequency_entry data;
			data.max_events = native.maxEvents;
			data.rules = read_physics_vfx_event_asset_rules(native.rules);
			return data;
		}

		void write_physics_vfx_event_asset_frequency_entry(PhysicsVFXEventAssetFrequencyEntry& native, const asset_data::physics_events::physics_vfx_event_asset_frequency_entry& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.maxEvents = data.max_events;
			write_physics_vfx_event_asset_rules(native.rules, data.rules, resolve, memory);
		}

		std::size_t size_physics_vfx_event_asset_frequency_entry([[maybe_unused]] const asset_data::physics_events::physics_vfx_event_asset_frequency_entry& data)
		{
			std::size_t size = 0;
			size += size_physics_vfx_event_asset_rules(data.rules);
			return size;
		}

	}

	std::expected<asset_data::physics_events::physics_sfx_event, std::string> to_neutral(const PhysicsSFXEventAsset& native)
	{
		try
		{
			auto data = read_physics_sfx_event(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	PhysicsSFXEventAsset* from_neutral(const asset_data::physics_events::physics_sfx_event& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<PhysicsSFXEventAsset>();
		write_physics_sfx_event(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::physics_events::physics_sfx_event& data)
	{
		return plain::rounded(sizeof(PhysicsSFXEventAsset)) + size_physics_sfx_event(data) + 64;
	}

	std::expected<asset_data::physics_events::physics_vfx_event, std::string> to_neutral(const PhysicsVFXEventAsset& native)
	{
		try
		{
			auto data = read_physics_vfx_event(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	PhysicsVFXEventAsset* from_neutral(const asset_data::physics_events::physics_vfx_event& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<PhysicsVFXEventAsset>();
		write_physics_vfx_event(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::physics_events::physics_vfx_event& data)
	{
		return plain::rounded(sizeof(PhysicsVFXEventAsset)) + size_physics_vfx_event(data) + 64;
	}

}
