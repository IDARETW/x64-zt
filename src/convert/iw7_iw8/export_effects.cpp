#include <std_include.hpp>
#include "export.hpp"
#include "images.hpp"

#include <iw8/iw8_focus_structs.h>

namespace zonetool::convert::iw7_iw8
{
	namespace
	{
		namespace focus = iw8_focus;

		std::string hex(const void* data, const std::size_t size)
		{
			static constexpr char digits[] = "0123456789abcdef";
			std::string result;
			result.reserve(size * 2);
			for (std::size_t i = 0; i < size; i++)
			{
				const auto byte = static_cast<const std::uint8_t*>(data)[i];
				result += digits[byte >> 4];
				result += digits[byte & 15];
			}

			return result;
		}

		template <typename T>
		std::string hex(const T& value)
		{
			return hex(&value, sizeof(value));
		}

		// Replay's particle state flags (the game-test enum); IW7 has two more flags at bit 10, so that its later bits
		// are Replay's two higher.
		constexpr std::uint64_t state_spawn_shape = 1ull << 2;
		constexpr std::uint64_t state_position_curve = 1ull << 3;
		constexpr std::uint64_t state_rotation_1d_curve = 1ull << 4;
		constexpr std::uint64_t state_rotation_3d_curve = 1ull << 5;
		constexpr std::uint64_t state_rotation_1d_init = 1ull << 6;
		constexpr std::uint64_t state_rotation_3d_init = 1ull << 7;
		constexpr std::uint64_t state_velocity_curves = 3ull << 8;
		constexpr std::uint64_t state_use_physics = 1ull << 10;
		constexpr std::uint64_t state_mirror_texture = 15ull << 11;
		constexpr std::uint64_t state_handle_on_impact = 1ull << 16;
		constexpr std::uint64_t state_player_facing = 1ull << 17;
		constexpr std::uint64_t state_occlusion_query = 1ull << 19;
		constexpr std::uint64_t state_ray_cast_physics = 1ull << 21;
		constexpr std::uint64_t state_emissive_curve = 1ull << 22;
		constexpr std::uint64_t state_intensity_curve = 1ull << 23;
		constexpr std::uint64_t state_vector_fields = 1ull << 24;
		constexpr std::uint64_t state_inherit_parent_velocity = 1ull << 25;
		constexpr std::uint64_t state_play_sounds = 1ull << 27;
		constexpr std::uint64_t state_child_effects = 1ull << 32;
		constexpr std::uint64_t state_size_curve = 1ull << 39;
		constexpr std::uint64_t state_size_lerp = 1ull << 40;
		// The flags that the modules of a state decide.
		constexpr std::uint64_t state_module_flags = state_spawn_shape | state_position_curve | state_rotation_1d_curve |
			state_rotation_3d_curve | state_rotation_1d_init | state_rotation_3d_init | state_velocity_curves | state_use_physics |
			state_mirror_texture | state_handle_on_impact | state_player_facing | state_occlusion_query | state_ray_cast_physics |
			state_emissive_curve | state_intensity_curve | state_vector_fields | state_inherit_parent_velocity | state_play_sounds |
			state_child_effects | state_size_curve | state_size_lerp;

		std::uint64_t replay_state_flags(const std::uint64_t flags)
		{
			return (flags & 0x3FF) | (flags >> 12 << 10);
		}

		// Replay particle element types: IW7 draws clouds and spark clouds as sprites in Replay's element list, which has
		// no cloud.
		std::optional<std::uint32_t> replay_element(const std::uint32_t element)
		{
			static constexpr std::array<std::uint32_t, 15> elements{0, 1, 0, 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
			return element < elements.size() ? std::optional(elements[element]) : std::nullopt;
		}

		// Replay module types: IW7's list without Replay's later additions (kill wrap box 8, dismember 32, shader graph
		// 38, force drag graph 40, temperature graph 43, scale by distance 50, vector field graph 54, test time in state 61).
		std::uint16_t replay_module(const std::uint16_t type)
		{
			if (type <= 7)
			{
				return type;
			}

			if (type <= 30)
			{
				return type + 1;
			}

			static constexpr std::array<std::uint16_t, 23> update{33, 34, 35, 36, 37, 39, 41, 42, 44, 45, 46, 47, 48, 49, 51, 52, 53, 55, 56, 57,
				58, 59, 60};
			return type - 31u < update.size() ? update[type - 31u] : 0xFFFF;
		}

		// Replay's flare element (PARTICLE_ELEMENT_TYPE_FLARE).
		constexpr std::uint32_t flare_element = 3;

		std::vector<std::uint8_t> unhex(const std::string_view text)
		{
			std::vector<std::uint8_t> bytes(text.size() / 2);
			for (std::size_t i = 0; i < bytes.size(); i++)
			{
				bytes[i] = static_cast<std::uint8_t>(std::stoul(std::string(text.substr(i * 2, 2)), nullptr, 16));
			}

			return bytes;
		}

		// A flare material of Replay's common_mp and the material data that stock flares give it (their two templates).
		struct flare_material
		{
			const char* material;
			const char* material_data;
		};

		const flare_material& stock_flare(const std::string_view iw7_material)
		{
			static constexpr char streaks[] = "20bcbe4d7f403731fad69b3fe8ab5ebe0000803f0000803f0000803f0000803f0000003f0000003f0000003f0000003f00000000000000006f12033b0000803f000000000000000000000000000000000000803f000000400000803f00000000cdcccc3c0000204200002042cdcccc3ccdcccc3e00000000cdcccc3ecdcccc3e000000000000000000000000000000000000803f0000803f0000803f0000803f0000803f0000803f0000803f0000803fcdccccbd000000000000000000000000";
			static constexpr char stripes[] = "20bcbe4d7f4037317ff90140d0b6b9be0000803f0000803f0000803f0000803f0000003f0000003f0000003f0000003f0000803f000000006f12033b0000803f000000000000000000000000000000000000803f000000400000803f000000000000803f0000803f0000803f0000803f0ad7233c000000000ad7233c0ad7233c000000000000000000000000000000000000803f0000803f0000803f0000803f0000803f0000803f0000803f0000803fcdccccbd000000000000000000000000";
			static constexpr char glow[] = "20bcbe4d7f403731fad69b3fe8ab5ebe0000803f0000803f0000803f0000803f0000003f0000003f0000003f0000003f00000000000000006f12033b0000803f000000000000000000000000000000000000803f000000400000803f000000000000003f0000004000000040cdcccc3c0ad7a33c000000000ad7a33ccdcccc3e000000000000000000000000000000000000803f0000803f0000803f0000803f0000803f0000803f0000803f0000803fcdccccbd000000000000000000000000";
			static const flare_material blue{"ecf/vfx_postfx_anamorphic_blue_1", streaks};
			static const flare_material streak{"ecf/vfx_postfx_flare_streak_1", streaks};
			static const flare_material stripe{"ecf/vfx_postfx_stripe_1", stripes};
			static const flare_material spectrum{"ecf/vfx_postfx_chroma_star_1", streaks};
			static const flare_material round{"ecf/gfx_flare_wtr", glow};
			const auto has = [&](const std::string_view part) { return iw7_material.find(part) != std::string_view::npos; };
			return has("blue") ? blue : has("streak") ? streak : has("strip") ? stripe :
				has("spectrum") || has("rainbow") || has("clr") ? spectrum : round;
		}

		// Replay's particle shape (common_mp), which the importer names without building it.
		constexpr char default_fx_shape[] = "vfx_physfxpip_particle_default";

		// The debris models of Replay's common_mp (ACTS xmodel dump, 2026-10-20), which effects can name without the model.
		constexpr std::array<std::string_view, 38> replay_debris_models{
			"vfx_debris_bone_shards_s1_f1", "vfx_debris_bone_shards_s1_f2", "vfx_debris_bone_shards_s1_f3", "vfx_debris_bone_shards_s1_f4",
			"vfx_debris_bone_shards_s1_f5", "vfx_debris_bone_shards_s1_f6", "vfx_debris_flesh_chunk_a_01", "vfx_debris_flesh_chunk_a_02",
			"vfx_debris_flesh_chunk_a_03", "vfx_debris_flesh_chunk_a_04", "vfx_debris_flesh_chunk_a_05", "vfx_debris_flesh_chunk_a_06",
			"vfx_debris_flesh_chunk_a_07", "vfx_debris_flesh_chunk_a_08", "vfx_debris_glass_shard_1_g1", "vfx_debris_glass_shard_1_g2",
			"vfx_debris_glass_shard_1_g3", "vfx_debris_glass_shard_1_g4", "vfx_debris_glass_shard_2_g1", "vfx_debris_glass_shard_2_g2",
			"vfx_debris_glass_shard_2_g3", "vfx_debris_glass_shard_2_g4", "vfx_debris_glass_shard_a_01", "vfx_debris_glass_shard_a_02",
			"vfx_debris_glass_shard_a_03", "vfx_debris_glass_shard_a_04", "vfx_debris_glass_shard_a_05", "vfx_debris_glass_shard_a_06",
			"vfx_debris_glass_shard_a_07", "vfx_debris_glass_shard_a_08", "vfx_debris_glass_shard_b_01", "vfx_debris_glass_shard_b_03",
			"vfx_debris_glass_shard_b_05", "vfx_debris_glass_shard_b_07", "vfx_debris_glass_shard_c_03", "vfx_debris_glass_shard_c_05",
			"vfx_debris_glass_shard_c_07", "vfx_debris_glass_shard_c_08"};

		// Replay's debris model of a name: the same name, else one of its family (the name without its last number).
		std::optional<std::string_view> replay_debris_model(const std::string_view name)
		{
			if (const auto found = std::ranges::find(replay_debris_models, name); found != replay_debris_models.end())
			{
				return *found;
			}

			const auto family = name.substr(0, name.find_last_not_of("0123456789") + 1);
			const auto found = std::ranges::find_if(replay_debris_models, [&](const auto model) { return model.starts_with(family); });
			return found != replay_debris_models.end() ? std::optional(*found) : std::nullopt;
		}

		// The Replay pipeline of an IW7 debris pipeline (both in Replay's common zones): sparks bounce as Replay's sparks, the
		// rest (plastic, wood, glass, concrete, cards) as its debris.
		std::string replay_pipeline(const std::string_view name)
		{
			return name.find("spark") != std::string_view::npos ? "physfxpip_sparks_callback" : "physfxpip_debris_callback";
		}

		// Replay's emitter data flags (bits of the per-particle streams): the streams every emitter has, and those that
		// stock emitters have when they hold these modules (Replay types).
		constexpr std::uint32_t data_base = 0x192F8FF;
		constexpr std::uint32_t data_emissive = 1u << 8 | 1u << 18 | 1u << 25;
		constexpr std::uint32_t data_parent_velocity = 1u << 9 | 1u << 21;
		constexpr std::uint32_t data_child_system = 1u << 10;
		constexpr std::uint32_t data_module_tests = 1u << 16;
		constexpr std::uint32_t data_ray_cast_handle = 1u << 22;
		constexpr std::uint32_t data_previous_placement = 1u << 26;

		json curve_points(const iw7::ParticleCurveDef& curve)
		{
			auto points = json::array();
			for (auto i = 0; curve.controlPoints && i < curve.numControlPoints; i++)
			{
				const auto& point = curve.controlPoints[i];
				points.push_back({point.time, point.value, point.invTimeDelta});
			}

			return points;
		}

		// A Replay module: its record and the children that its pointers name, in member order (curves and linked asset
		// lists, at their offsets in the record).
		struct module_record
		{
			focus::ParticleModuleDef record{};
			std::map<std::size_t, json> children;

			std::uint8_t* data()
			{
				return this->record.moduleData;
			}

			template <typename T>
			void set(const std::size_t offset, const T& value)
			{
				std::memcpy(this->record.moduleData + offset, &value, sizeof(T));
			}

			// A curve at an offset of the module data: its count and scale stay in the record, its points follow.
			void curve(const std::size_t offset, const iw7::ParticleCurveDef& source)
			{
				focus::ParticleCurveDef curve{};
				curve.numControlPoints = source.controlPoints ? source.numControlPoints : 0;
				curve.scale = source.scale;
				this->set(offset, curve);
				if (curve.numControlPoints)
				{
					this->children[0x10 + offset] = {{"offset", 0x10 + offset}, {"curve", curve_points(source)}};
				}
			}

			// A constant curve (value 1 over the life, times the scale).
			void constant_curve(const std::size_t offset, const float scale)
			{
				iw7::ParticleCurveControlPointDef points[2]{{0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}};
				iw7::ParticleCurveDef source{points, 2, scale};
				this->curve(offset, source);
			}

			// A linked asset list at an offset of the module data, with its entries (0x20 bytes each).
			void list(const std::size_t offset, json entries)
			{
				focus::ParticleLinkedAssetListDef list{};
				list.numAssets = static_cast<std::int32_t>(entries.size());
				this->set(offset, list);
				if (!entries.empty())
				{
					this->children[0x10 + offset] = {{"offset", 0x10 + offset}, {"entries", std::move(entries)}};
				}
			}

			json to_json() const
			{
				auto list = json::array();
				for (const auto& [offset, child] : this->children)
				{
					list.push_back(child);
				}

				return {{"module", hex(this->record)}, {"children", std::move(list)}};
			}
		};

		// A linked asset: the asset that the entry names at +0 (the pipeline gives the converted name), with selector
		// data at +8.
		json linked_asset(const std::string_view type, const std::string_view name, const std::uint32_t selector = 0)
		{
			std::array<std::uint8_t, 0x20> entry{};
			std::memcpy(entry.data() + 8, &selector, sizeof(selector));
			return {{"entry", hex(entry.data(), entry.size())}, {"assets", json::array({{0, type, name}})}};
		}

		class effect_converter
		{
		public:
			effect_converter(const source_map& source, extraction& output)
				: source_(source)
				, output_(output)
				, light_unit_(light_unit_scale(source))
			{
			}

			// Converts an effect and the effects that it starts; returns false if it does not convert.
			bool convert(const std::string& name)
			{
				if (const auto found = this->results_.find(name); found != this->results_.end())
				{
					return found->second;
				}

				// A cycle of child effects converts once.
				this->results_[name] = true;
				const auto parent = std::exchange(this->current_, name);
				const auto _ = gsl::finally([&] { this->current_ = parent; });
				const auto* system = static_cast<const iw7::ParticleSystemDef*>(this->source_.find(iw7::ASSET_TYPE_VFX, name));
				if (!system)
				{
					this->results_[name] = false;
					this->missing_.insert(name);
					return false;
				}

				auto effect = this->convert_system(*system);
				if (!effect)
				{
					this->results_[name] = false;
					return false;
				}

				write_json(this->output_.extracted / "vfx" / (file_name(name) + ".json"), *effect);
				this->converted_++;
				return true;
			}

			std::size_t converted() const
			{
				return this->converted_;
			}

			const std::set<std::string>& missing() const
			{
				return this->missing_;
			}

			const std::map<std::string, std::size_t>& dropped() const
			{
				return this->dropped_;
			}

		private:
			const source_map& source_;
			extraction& output_;
			std::unordered_map<std::string, bool> results_;
			std::set<std::string> missing_;
			std::map<std::string, std::size_t> dropped_;
			std::size_t converted_{};
			std::string current_;

			std::set<std::string> shapes_;
			std::set<std::string> particle_sims_;
			float light_unit_{};
			// The state being converted draws through IW7's unlit effect materials (effectunlit), whose tints carry the map's
			// light unit (export_effect_material); lit ones keep their tint.
			bool unlit_state_{};
			// The state being converted has an atlas module: its materials keep their atlas (extraction::atlas_materials).
			bool atlas_state_{};
			// The state being converted draws oriented sprites (their velocity frame orients them).
			bool oriented_state_{};
			// The state being converted is an IW7 glare (a flare at its source, outside sun effects) drawn as a billboard, and
			// the size of its flare at the source (the radial scales), which its size modules take.
			bool glare_state_{};
			float glare_scale_{1.0f};
			std::optional<std::set<std::string>> bank_aliases_;

			// The aliases of the map's sound bank (<map>.all).
			const std::set<std::string>& bank_aliases()
			{
				if (!this->bank_aliases_)
				{
					// The aliases of every loaded bank: export_sounds takes them from the map bank, then from the common zones'
					// banks (with each bank's own sound files).
					this->bank_aliases_.emplace();
					for (const auto& [bank_name, asset] : this->source_.assets(iw7::ASSET_TYPE_SOUND_BANK))
					{
						const auto* bank = static_cast<const iw7::SndBank*>(asset);
						for (auto i = 0u; bank && bank->alias && i < bank->aliasCount; i++)
						{
							if (bank->alias[i].aliasName)
							{
								this->bank_aliases_->emplace(asset_name(bank->alias[i].aliasName));
							}
						}
					}
				}

				return *this->bank_aliases_;
			}

			// Writes the points of an IW7 debris shape once, in Havok units (IW7 keeps game units: its
			// vfx_debris_glass_shard_b is 32 times Replay's); returns its name.
			std::string add_shape(const iw7::PhysicsFXShape& shape)
			{
				const std::string name(asset_name(shape.name));
				if (this->shapes_.insert(name).second)
				{
					auto points = json::array();
					for (auto i = 0u; i < shape.size; i++)
					{
						points.push_back({shape.vecs[i][0] / 32.0f, shape.vecs[i][1] / 32.0f, shape.vecs[i][2] / 32.0f});
					}

					write_json(this->output_.extracted / "physicsfxshape" / (file_name(name) + ".json"), {{"name", name}, {"points", std::move(points)}});
				}

				return name;
			}

			// Writes an IW7 particle sim animation once (its frames of baked particles, in Replay's layout, which
			// FX_GenParticleSimAnimationVerts 0x17FD1E0 reads) with its material as an effect material; returns its name, or
			// nothing when its material or its particles are missing (Replay draws every animation through its material).
			std::optional<std::string> add_particle_sim(const iw7::FxParticleSimAnimation& sim)
			{
				const auto& header = sim.header;
				const auto* material = sim.material ? this->source_.resolve(iw7::ASSET_TYPE_MATERIAL, sim.material) : nullptr;
				if (!sim.name || !material || !header.frameCount || !sim.frames || (header.particleDataCount && !sim.particleData) ||
					(header.colorTableSize && !sim.colorTable) || (header.hasOrientation3D && header.particleDataCount &&
					!sim.particleDataOrientation3D) || (header.hasIndex && header.particleDataCount && !sim.particleDataIndex))
				{
					return std::nullopt;
				}

				const std::string name(asset_name(sim.name));
				if (this->particle_sims_.insert(name).second)
				{
					const auto material_name = this->output_.add_material(this->source_, sim.material);
					this->output_.effect_materials.insert(material_name);
					// Baked particles keep their cells (the animation's index data).
					this->output_.atlas_materials.insert(material_name);
					const auto count = header.particleDataCount;
					write_json(this->output_.extracted / "particlesimanimation" / (file_name(name) + ".json"), {
						{"name", name},
						{"material", material_name},
						{"header", hex(header)},
						{"particle_data", hex(sim.particleData, sizeof(*sim.particleData) * count)},
						{"orientation_3d", header.hasOrientation3D ? hex(sim.particleDataOrientation3D,
							sizeof(*sim.particleDataOrientation3D) * count) : ""},
						{"index", header.hasIndex ? hex(sim.particleDataIndex, sizeof(*sim.particleDataIndex) * count) : ""},
						{"frames", hex(sim.frames, sizeof(*sim.frames) * header.frameCount)},
						{"color_table", hex(sim.colorTable, sizeof(*sim.colorTable) * header.colorTableSize)}});
				}

				return name;
			}

			void drop(const std::string& what)
			{
				this->dropped_[what]++;
				ZONETOOL_INFO("iw7: effect \"%s\" leaves out a %s", this->current_.data(), what.data());
			}

			std::optional<json> convert_system(const iw7::ParticleSystemDef& system)
			{
				focus::ParticleSystemDef root{};
				// The writer gives converted effects the current version and their name, emitter and node pointers.
				root.flags = system.flags & ~0x20u;
				root.occlusionOverrideEmitterIndex = system.occlusionOverrideEmitterIndex;
				root.phaseOptions = static_cast<std::uint32_t>(system.phaseOptions);
				root.drawFrustumCullRadius = system.drawFrustumCullRadius;
				root.updateFrustumCullRadius = system.updateFrustumCullRadius;
				root.sunDistance = system.sunDistance;
				root.preRollMSec = system.preRollMSec;
				std::memcpy(&root.editorPosition, &system.editorPosition, 16);
				std::memcpy(&root.editorRotation, &system.editorRotation, 16);
				std::memcpy(&root.gameTweakPosition, &system.gameTweakPosition, 16);
				std::memcpy(&root.gameTweakRotation, &system.gameTweakRotation, 16);

				auto emitters = json::array();
				std::vector<int> kept(system.numEmitters, -1);
				for (auto e = 0; e < system.numEmitters; e++)
				{
					if (auto converted = this->convert_emitter(system.emitterDefs[e]))
					{
						kept[e] = static_cast<int>(emitters.size());
						emitters.push_back(std::move(*converted));
					}
				}

				if (emitters.empty())
				{
					ZONETOOL_INFO("iw7: effect \"%s\" does not convert: none of its %d emitters does", this->current_.data(),
						system.numEmitters);
					return std::nullopt;
				}

				// The emitter that overrides the occlusion keeps its new index, or none when it was left out.
				if (system.occlusionOverrideEmitterIndex >= 0 && system.occlusionOverrideEmitterIndex < system.numEmitters)
				{
					root.occlusionOverrideEmitterIndex = static_cast<decltype(root.occlusionOverrideEmitterIndex)>(
						kept[system.occlusionOverrideEmitterIndex]);
				}

				return json{{"schema", 1}, {"name", asset_name(system.name)}, {"system", hex(root)}, {"emitters", std::move(emitters)}};
			}

			std::optional<json> convert_emitter(const iw7::ParticleEmitterDef& emitter)
			{
				focus::ParticleEmitterDef record{};
				std::memcpy(&record.particleSpawnRate, &emitter.particleSpawnRate, 8);
				std::memcpy(&record.particleLife, &emitter.particleLife, 8);
				std::memcpy(&record.particleDelay, &emitter.particleDelay, 8);
				record.particleCountMax = emitter.particleCountMax;
				std::memcpy(&record.particleBurstCount, &emitter.particleBurstCount, 8);
				std::memcpy(&record.emitterLife, &emitter.emitterLife, 8);
				std::memcpy(&record.emitterDelay, &emitter.emitterDelay, 8);
				record.randomSeed = emitter.randomSeed;
				std::memcpy(&record.spawnRangeSq, &emitter.spawnRangeSq, 8);
				record.fadeOutMaxDistance = emitter.fadeOutMaxDistance;
				record.fadeCurveDef.scale = emitter.fadeCurveDef.scale;
				record.spawnFrustumCullRadius = emitter.spawnFrustumCullRadius;
				// Without scripted inputs (the writer has none).
				record.flags = emitter.flags & ~0x40u;
				record.gravityOptions = static_cast<std::uint32_t>(emitter.gravityOptions);
				std::memcpy(record.groupIDs, emitter.groupIDs, sizeof(record.groupIDs));
				// Replay's own conversions of IW7 effects (common_mp) emit by distance at 0.1, as converted IW3 effects;
				// IW7's two floats in its place are not that density.
				record.emitByDistanceDensity = {0.1f, 0.1f};
				std::memcpy(&record.particleSpawnShapeRange, &emitter.particleSpawnShapeRange, 8);

				auto states = json::array();
				std::set<std::uint16_t> types;
				auto complete = true;
				for (auto s = 0; s < emitter.numStates; s++)
				{
					states.push_back(this->convert_state(emitter.stateDefs[s], types, complete));
				}

				// An emitter with a state that would read a missing module is left out.
				if (!complete)
				{
					return std::nullopt;
				}

				// The particle streams of the modules (rules that the stock emitters follow).
				const auto any = [&](const std::initializer_list<std::uint16_t> list)
				{
					return std::ranges::any_of(list, [&](const auto type) { return types.contains(type); });
				};

				record.dataFlags = data_base;
				record.dataFlags |= any({37, 13, 42, 7}) ? data_emissive : 0;
				record.dataFlags |= any({44}) ? data_parent_velocity : 0;
				record.dataFlags |= any({21}) ? data_child_system : 0;
				record.dataFlags |= any({55, 56, 57, 58, 59, 60, 61}) ? data_module_tests : 0;
				record.dataFlags |= any({39, 40, 41, 45, 46}) ? data_ray_cast_handle : 0;
				record.dataFlags |= any({13}) ? data_previous_placement : 0;
				return json{{"emitter", hex(record)}, {"fade_curve", curve_points(emitter.fadeCurveDef)}, {"states", std::move(states)}};
			}

			// The module that each Replay element draws from (Replay types): a state without it reads a null pointer when its
			// particles update (Breakneck: a spot light whose module did not convert crashed the map load at 0x141BA67BA).
			static std::optional<std::uint16_t> required_module(const std::uint32_t element)
			{
				switch (element)
				{
				case 1: return replay_module(iw7::PARTICLE_MODULE_INIT_BEAM);
				case 2: return replay_module(iw7::PARTICLE_MODULE_INIT_DECAL);
				case 3: return replay_module(iw7::PARTICLE_MODULE_INIT_FLARE);
				case 4: return replay_module(iw7::PARTICLE_MODULE_INIT_GEO_TRAIL);
				case 5: return replay_module(iw7::PARTICLE_MODULE_INIT_LIGHT_OMNI);
				case 6: return replay_module(iw7::PARTICLE_MODULE_INIT_LIGHT_SPOT);
				case 7: return replay_module(iw7::PARTICLE_MODULE_INIT_MODEL);
				case 9: return replay_module(iw7::PARTICLE_MODULE_INIT_RUNNER);
				default: return std::nullopt;
				}
			}

			json convert_state(const iw7::ParticleStateDef& state, std::set<std::uint16_t>& emitter_types, bool& complete)
			{
				focus::ParticleStateDef record{};
				const auto element = replay_element(state.elementType);
				record.elementType = element.value_or(0);

				// IW7's glares (Frontier's fixture, warning and boost glares: flares at their source with IW7's additive glow
				// materials, which ignore depth) flickered as Replay flares: Replay's flare occlusion (cs_flare_occlusion, a 3x3
				// screen mask at the source) switched them on and off with the view, the source lying on its fixture. They
				// draw as billboards of the flare's size at the source, with their own materials; the screen-space pieces of a
				// flare away from its source (ghosts) are left out. Sun flares stay Replay flares.
				this->glare_state_ = false;
				this->glare_scale_ = 1.0f;
				if (state.elementType == iw7::PARTICLE_ELEMENT_TYPE_FLARE && this->current_.find("sun") == std::string::npos)
				{
					const iw7::ParticleModuleInitFlare* flare_module{};
					for (auto g = 0; g < 3 && !flare_module; g++)
					{
						const auto& group = state.moduleGroupDefs[g];
						for (auto m = 0; !group.disabled && m < group.numModules; m++)
						{
							if (group.moduleDefs[m].moduleType == iw7::PARTICLE_MODULE_INIT_FLARE)
							{
								flare_module = &group.moduleDefs[m].moduleData.initFlare;
							}
						}
					}

					if (!flare_module || std::abs(flare_module->m_position - 1.0f) > 0.05f)
					{
						this->drop("flare away from its source (no Replay flare draws without its occlusion)");
						complete = false;
						return {{"state", hex(record)}, {"groups", json::array({json::array(), json::array(), json::array()})}};
					}

					auto scale = std::max(flare_module->m_radialScaleX.min, flare_module->m_radialScaleY.min);
					if (flare_module->m_depthScaleRange.max > flare_module->m_depthScaleRange.min)
					{
						scale *= flare_module->m_depthScaleValue.max;
					}

					this->glare_state_ = true;
					this->glare_scale_ = scale > 0.0f && std::isfinite(scale) ? scale : 1.0f;
					record.elementType = 0;
				}
				const auto source_flags = replay_state_flags(state.flags);

				// Whether the state's materials are IW7's unlit effect materials (the emissive graph's intensities depend on it),
				// and whether the state has an atlas module.
				this->unlit_state_ = false;
				this->atlas_state_ = false;
				this->oriented_state_ = state.elementType == iw7::PARTICLE_ELEMENT_TYPE_ORIENTED_SPRITE;
				for (auto g = 0; g < 3; g++)
				{
					const auto& group = state.moduleGroupDefs[g];
					for (auto m = 0; !group.disabled && m < group.numModules; m++)
					{
						const auto& module = group.moduleDefs[m];
						this->atlas_state_ = this->atlas_state_ || module.moduleType == iw7::PARTICLE_MODULE_INIT_ATLAS;
						if (module.moduleType != iw7::PARTICLE_MODULE_INIT_MATERIAL)
						{
							continue;
						}

						const auto& list = module.moduleData.initMaterial.m_linkedAssetList;
						for (auto a = 0; list.assetList && a < list.numAssets; a++)
						{
							const auto* material = list.assetList[a].material ? this->source_.resolve(iw7::ASSET_TYPE_MATERIAL,
								const_cast<iw7::Material*>(list.assetList[a].material)) : nullptr;
							this->unlit_state_ = this->unlit_state_ || (material && material->techniqueSet && material->techniqueSet->name &&
								std::string_view(material->techniqueSet->name).find("effectunlit") != std::string_view::npos);
						}
					}
				}

				auto groups = json::array();
				std::set<std::uint16_t> types;
				for (auto g = 0; g < 3; g++)
				{
					auto modules = json::array();
					const auto& group = state.moduleGroupDefs[g];
					for (auto m = 0; !group.disabled && m < group.numModules; m++)
					{
						if (this->glare_state_ && group.moduleDefs[m].moduleType == iw7::PARTICLE_MODULE_INIT_FLARE)
						{
							continue;
						}

						auto module = this->convert_module(group.moduleDefs[m], static_cast<std::uint16_t>(modules.size()),
							record.elementType == flare_element);
						if (module)
						{
							types.insert(module->record.moduleType);
							modules.push_back(module->to_json());
						}
					}

					groups.push_back(std::move(modules));
				}

				// The flags of the modules that the state holds now.
				const auto has = [&](const std::uint16_t type)
				{
					return types.contains(type);
				};

				// IW7's flags (in Replay's places), without those of the modules that do not convert, and with Replay's
				// size curve and size lerp flags, which IW7 has not.
				const auto keep = [&](const bool present, const std::uint64_t mask)
				{
					return present ? mask : 0;
				};

				auto kept = ~state_module_flags;
				kept |= keep(has(24) || has(25) || has(26) || has(27) || has(28), state_spawn_shape);
				kept |= keep(has(47), state_position_curve);
				kept |= keep(has(48), state_rotation_1d_curve);
				kept |= keep(has(49), state_rotation_3d_curve);
				kept |= keep(has(19), state_rotation_1d_init);
				kept |= keep(has(20), state_rotation_3d_init);
				kept |= keep(has(53), state_velocity_curves);
				kept |= keep(has(45) || has(46), state_use_physics);
				kept |= keep(has(12), state_mirror_texture);
				kept |= keep(has(59), state_handle_on_impact);
				kept |= keep(has(17), state_player_facing | 1ull << 18);
				kept |= keep(has(14), state_occlusion_query);
				kept |= keep(has(46), state_ray_cast_physics);
				kept |= keep(has(37), state_emissive_curve);
				kept |= keep(has(42), state_intensity_curve);
				kept |= keep(has(30), state_vector_fields);
				kept |= keep(has(44), state_inherit_parent_velocity);
				kept |= keep(has(22), state_play_sounds);
				kept |= keep(has(21) || has(56) || has(57) || has(59), state_child_effects);
				auto flags = source_flags & kept;
				flags |= has(51) ? state_size_curve : 0;
				flags |= has(52) ? state_size_lerp : 0;
				record.flags = flags;
				emitter_types.insert(types.begin(), types.end());
				if (!element)
				{
					this->drop(std::format("element {}", state.elementType));
				}

				if (const auto needed = required_module(record.elementType); needed && !types.contains(*needed))
				{
					this->drop(std::format("state of element {} without its module {}", record.elementType, *needed));
					complete = false;
				}

				// A flare state without its material (its image cannot be read) leaves its emitter out.
				if (record.elementType == flare_element && !types.contains(replay_module(iw7::PARTICLE_MODULE_INIT_MATERIAL)))
				{
					this->drop("flare without its material");
					complete = false;
				}

				// IW7_NO_FLARES=1 leaves the flares out (a switch for testing their conversion).
				if (record.elementType == flare_element && std::getenv("IW7_NO_FLARES"))
				{
					this->drop("flare");
					complete = false;
				}

				return {{"state", hex(record)}, {"groups", std::move(groups)}};
			}

			// Converts a module of the IW7 layout to Replay's, or leaves it out (counted by type).
			std::optional<module_record> convert_module(const iw7::ParticleModuleDef& module, const std::uint16_t index,
				const bool flare = false)
			{
				const auto& data = module.moduleData;
				const auto type = static_cast<std::uint16_t>(module.moduleType);
				module_record result;
				result.record.moduleType = replay_module(type);
				const auto base = [&]
				{
					focus::ParticleModuleBase value{result.record.moduleType, 0, data.moduleBase.m_flags};
					result.set(0, value);
				};

				// Modules whose Replay layout is IW7's: their bytes, without the pointers of their curves and lists.
				const auto copy = [&](const std::size_t size)
				{
					std::memcpy(result.data(), &data, size);
					base();
				};

				// IW7 never sets a graph's firstCurve byte (after the module base) and draws its graphs as values (a size graph's
				// curves times their scale are the size). Replay multiplies a graph without it into the particle's value: a size
				// graph squared IW7's start size (Scorch's sun: tails of 25 million units, wedges through the view). Stock
				// graphs set it (97%), as do Replay's own ports of IW7 effects (common_mp: every one).
				const auto first_curve = [&]
				{
					result.data()[offsetof(iw7::ParticleModuleSizeGraph, firstCurve)] = 1;
				};

				switch (module.moduleType)
				{
				case iw7::PARTICLE_MODULE_INIT_ATLAS:
				{
					// IW7: play rate, start frame, loop count, random index, play over life. Replay keeps the play rate as
					// two constant curves, scaled by it (stock IW7 effects of common_mp).
					focus::ParticleModuleInitAtlas atlas{};
					const auto* bytes = reinterpret_cast<const std::uint8_t*>(&data);
					std::int32_t play_rate{};
					std::memcpy(&play_rate, bytes + 8, 4);
					std::memcpy(&atlas.startFrame, bytes + 12, 4);
					std::memcpy(&atlas.loopCount, bytes + 16, 4);
					atlas.randomIndex = bytes[20];
					atlas.playOverLife = bytes[21];
					result.set(0, atlas);
					base();
					result.constant_curve(offsetof(focus::ParticleModuleInitAtlas, curves), static_cast<float>(play_rate));
					result.constant_curve(offsetof(focus::ParticleModuleInitAtlas, curves) + 16, static_cast<float>(play_rate));
					return result;
				}
				case iw7::PARTICLE_MODULE_INIT_ATTRIBUTES:
					copy(sizeof(iw7::ParticleModuleInitAttributes));
					if (this->glare_state_)
					{
						auto sizes = std::array{data.initAttributes.m_sizeMin, data.initAttributes.m_sizeMax};
						for (auto& extent : sizes)
						{
							for (auto& value : extent.v)
							{
								value *= this->glare_scale_;
							}
						}

						result.set(offsetof(iw7::ParticleModuleInitAttributes, m_sizeMin), sizes[0]);
						result.set(offsetof(iw7::ParticleModuleInitAttributes, m_sizeMax), sizes[1]);
					}

					return result;
				case iw7::PARTICLE_MODULE_INIT_BEAM:
				{
					// Replay's layout is IW7's (game-test PDB); stock beams (common_mp vfx_blue_laser_bright) keep no
					// linked assets and take their material from the state's material module.
					copy(sizeof(iw7::ParticleModuleInitBeam));
					std::memset(result.data() + offsetof(iw7::ParticleModuleInitBeam, m_linkedAssetList), 0,
						sizeof(iw7::ParticleLinkedAssetListDef));
					return result;
				}
				case iw7::PARTICLE_MODULE_INIT_RELATIVE_VELOCITY:
				{
					copy(16);
					// Replay orients an oriented sprite by its velocity frame, and none of its own (137 in common_mp, 112 of
					// them LOCAL) keeps the bolt frame that 80% of its other velocity modules use (LOCAL_WITH_BOLT_INFO).
					// Scorch's sun (vfx_div_sun_sm, a createfx one-shot) is two such sprites with LOCAL_WITH_BOLT_INFO:
					// they lay flat in the sky instead of facing the map. Oriented sprites take the frame without the bolt.
					const auto& source = data.initRelativeVelocity;
					if (this->oriented_state_ && source.m_velocityType >= iw7::PARTICLE_RELATIVE_VELOCITY_TYPE_LOCAL_WITH_BOLT_INFO)
					{
						auto velocity = source;
						velocity.m_velocityType = static_cast<iw7::ParticleRelativeVelocityType>(
							velocity.m_velocityType - iw7::PARTICLE_RELATIVE_VELOCITY_TYPE_LOCAL_WITH_BOLT_INFO);
						velocity.m_useBoltInfo = false;
						result.set(0, velocity);
					}
					return result;
				}
				case iw7::PARTICLE_MODULE_INIT_CAMERA_OFFSET:
				case iw7::PARTICLE_MODULE_INIT_PLAYER_FACING:
				case iw7::PARTICLE_MODULE_INIT_MIRROR_TEXTURE:
				case iw7::PARTICLE_MODULE_INIT_TAIL:
				case iw7::PARTICLE_MODULE_GRAVITY:
					copy(16);
					return result;
				case iw7::PARTICLE_MODULE_INIT_ORIENTED_SPRITE:
					copy(sizeof(iw7::ParticleModuleInitOrientedSprite));
					return result;
				case iw7::PARTICLE_MODULE_INIT_ROTATION:
					copy(sizeof(iw7::ParticleModuleInitRotation));
					return result;
				case iw7::PARTICLE_MODULE_INIT_ROTATION_3D:
					copy(sizeof(iw7::ParticleModuleInitRotation3D));
					return result;
				case iw7::PARTICLE_MODULE_INIT_GEO_TRAIL:
					copy(sizeof(iw7::ParticleModuleInitGeoTrail));
					return result;
				case iw7::PARTICLE_MODULE_INIT_VOLUMETRIC:
					copy(sizeof(iw7::ParticleModuleInitVolumetric));
					return result;
				case iw7::PARTICLE_MODULE_ATTRACTOR:
					copy(sizeof(iw7::ParticleModuleAttractor));
					return result;
				case iw7::PARTICLE_MODULE_COLOR_LERP:
					copy(sizeof(iw7::ParticleModuleColorLerp));
					return result;
				case iw7::PARTICLE_MODULE_FORCE:
					copy(sizeof(iw7::ParticleModuleForce));
					return result;
				case iw7::PARTICLE_MODULE_PHYSICS_RAY_CAST:
					copy(sizeof(iw7::ParticleModulePhysicsRayCast));
					return result;
				case iw7::PARTICLE_MODULE_SIZE_LERP:
					copy(sizeof(iw7::ParticleModuleSizeLerp));
					return result;
				case iw7::PARTICLE_MODULE_INIT_FLARE:
				{
					// The layouts agree. Replay's flare builder (sub_141BA5920) scales a flare by value.min + value.max * (depth
					// - range.min) / range.max, unclamped, unless range.max is 0, and divides its size by its depth: a size in
					// world units. IW7's ranges grow the flare from nothing over its first units to full size (range 1 to 99,
					// value 0 to 1: a clamped ramp); as Replay's they scaled a far flare by its depth / 99, so that Breakneck's
					// sun and klaxon flares covered the screen. Past the ramp (as flares are seen) IW7's scale is value.max:
					// the flare takes no depth scale and value.max in its radial scales.
					copy(sizeof(iw7::ParticleModuleInitFlare));
					auto scaled = data.initFlare;
					if (scaled.m_depthScaleRange.max > scaled.m_depthScaleRange.min)
					{
						const auto scale = scaled.m_depthScaleValue.max;
						scaled.m_radialScaleX.min *= scale;
						scaled.m_radialScaleX.max *= scale;
						scaled.m_radialScaleY.min *= scale;
						scaled.m_radialScaleY.max *= scale;
					}

					scaled.m_depthScaleRange = {};
					scaled.m_depthScaleValue = {};
					result.set(offsetof(iw7::ParticleModuleInitFlare, m_depthScaleRange), scaled.m_depthScaleRange);
					result.set(offsetof(iw7::ParticleModuleInitFlare, m_depthScaleValue), scaled.m_depthScaleValue);
					result.set(offsetof(iw7::ParticleModuleInitFlare, m_radialScaleX), scaled.m_radialScaleX);
					result.set(offsetof(iw7::ParticleModuleInitFlare, m_radialScaleY), scaled.m_radialScaleY);
					for (auto c = 0; c < 4; c++)
					{
						result.curve(offsetof(iw7::ParticleModuleInitFlare, m_curves) + c * 16, data.initFlare.m_curves[c]);
					}

					return result;
				}
				case iw7::PARTICLE_MODULE_INIT_SPAWN:
					copy(sizeof(iw7::ParticleModuleInitSpawn));
					result.curve(offsetof(iw7::ParticleModuleInitSpawn, m_curves), data.initSpawn.m_curves[0]);
					return result;
				case iw7::PARTICLE_MODULE_COLOR_GRAPH:
					copy(sizeof(iw7::ParticleModuleColorGraph));
					first_curve();
					for (auto c = 0; c < 8; c++)
					{
						result.curve(offsetof(iw7::ParticleModuleColorGraph, m_curves) + c * 16, data.colorGraph.m_curves[c]);
					}

					return result;
				case iw7::PARTICLE_MODULE_EMISSION_GRAPH:
					copy(sizeof(iw7::ParticleModuleEmissionGraph));
					for (auto c = 0; c < 2; c++)
					{
						result.curve(offsetof(iw7::ParticleModuleEmissionGraph, m_curves) + c * 16, data.emissionGraph.m_curves[c]);
					}

					return result;
				case iw7::PARTICLE_MODULE_INTENSITY_GRAPH:
					copy(sizeof(iw7::ParticleModuleIntensityGraph));
					first_curve();
					for (auto c = 0; c < 2; c++)
					{
						result.curve(offsetof(iw7::ParticleModuleIntensityGraph, m_curves) + c * 16, data.intensityGraph.m_curves[c]);
					}

					return result;
				case iw7::PARTICLE_MODULE_PARENT_VELOCITY_GRAPH:
					copy(sizeof(iw7::ParticleModuleParentVelocityGraph));
					for (auto c = 0; c < 2; c++)
					{
						result.curve(offsetof(iw7::ParticleModuleParentVelocityGraph, m_curves) + c * 16,
							data.parentVelocityGraph.m_curves[c]);
					}

					return result;
				case iw7::PARTICLE_MODULE_ROTATION_GRAPH:
					copy(sizeof(iw7::ParticleModuleRotationGraph));
					for (auto c = 0; c < 2; c++)
					{
						result.curve(offsetof(iw7::ParticleModuleRotationGraph, m_curves) + c * 16, data.rotationGraph.m_curves[c]);
					}

					return result;
				case iw7::PARTICLE_MODULE_POSITION_GRAPH:
				case iw7::PARTICLE_MODULE_ROTATION_GRAPH_3D:
				case iw7::PARTICLE_MODULE_SIZE_GRAPH:
				case iw7::PARTICLE_MODULE_VELOCITY_GRAPH:
				{
					// Six curves after the 16 bytes of the module (and the begin and end values of the size and velocity
					// graphs).
					const iw7::ParticleCurveDef* curves{};
					std::size_t size{};
					if (module.moduleType == iw7::PARTICLE_MODULE_POSITION_GRAPH)
					{
						curves = data.positionGraph.m_curves;
						size = sizeof(iw7::ParticleModulePositionGraph);
					}
					else if (module.moduleType == iw7::PARTICLE_MODULE_ROTATION_GRAPH_3D)
					{
						curves = data.rotationGraph3D.m_curves;
						size = sizeof(iw7::ParticleModuleRotationGraph3D);
					}
					else if (module.moduleType == iw7::PARTICLE_MODULE_SIZE_GRAPH)
					{
						curves = data.sizeGraph.m_curves;
						size = sizeof(iw7::ParticleModuleSizeGraph);
					}
					else
					{
						curves = data.velocityGraph.m_curves;
						size = sizeof(iw7::ParticleModuleVelocityGraph);
					}

					copy(size);
					if (module.moduleType == iw7::PARTICLE_MODULE_SIZE_GRAPH)
					{
						first_curve();
					}

					const auto first = reinterpret_cast<const std::uint8_t*>(curves) - reinterpret_cast<const std::uint8_t*>(&data);
					for (auto c = 0; c < 6; c++)
					{
						auto curve = curves[c];
						if (this->glare_state_ && module.moduleType == iw7::PARTICLE_MODULE_SIZE_GRAPH)
						{
							curve.scale *= this->glare_scale_;
						}

						result.curve(first + c * 16, curve);
					}

					if (this->glare_state_ && module.moduleType == iw7::PARTICLE_MODULE_SIZE_GRAPH)
					{
						auto sizes = std::array{data.sizeGraph.m_sizeBegin, data.sizeGraph.m_sizeEnd};
						for (auto& extent : sizes)
						{
							for (auto& value : extent.v)
							{
								value *= this->glare_scale_;
							}
						}

						result.set(offsetof(iw7::ParticleModuleSizeGraph, m_sizeBegin), sizes[0]);
						result.set(offsetof(iw7::ParticleModuleSizeGraph, m_sizeEnd), sizes[1]);
					}

					// Replay's rotation graphs (stock and its own conversions of IW7 effects) have no curve scale of 1: the
					// scale is 0 where IW7's is 1.
					if (module.moduleType == iw7::PARTICLE_MODULE_ROTATION_GRAPH_3D)
					{
						for (auto c = 0; c < 6; c++)
						{
							if (curves[c].scale == 1.0f)
							{
								result.set(first + c * 16 + 12, 0.0f);
							}
						}
					}

					return result;
				}
				case iw7::PARTICLE_MODULE_INIT_MATERIAL:
				{
					// Replay adds render and shader-graph options (0, as converted IW3 effects) and the material data, which
					// the pipeline takes from the converted material.
					base();
					auto entries = json::array();
					const auto& list = data.initMaterial.m_linkedAssetList;
					for (auto a = 0; a < list.numAssets; a++)
					{
						// A material of another zone (the lens-flare materials of techsets_common_core_mp) reads from the zone that holds it.
						const auto* material = list.assetList[a].material ? this->source_.resolve(iw7::ASSET_TYPE_MATERIAL,
							const_cast<iw7::Material*>(list.assetList[a].material)) : nullptr;
						// A flare never draws through an effect sprite material: Replay draws its screen-space vertices as screen-wide
						// wedges through the screen centre (Scorch's lava glare, owner test of cbf4948). A flare whose own image reads
						// takes Replay's flare material of its role; one whose image cannot be read (Frontier's fixture glares: stubs of
						// paks the zone folder lacks) is left out with its emitter, since a stock flare in its place drew white glows
						// that the occlusion test switched on and off with the view (owner test of 58aac89). Read at the extraction's
						// size, as the material export reads it (the small streams of an image can lie in a pak that the larger do not).
						const auto readable = [&]
						{
							const auto* color = color_image(this->source_, material);
							return color && color->name && (color->name[0] == '$' ||
								load_image_faces(*color, DXGI_FORMAT_R8G8B8A8_UNORM).size() == 1);
						};
						if (material && flare && !readable())
						{
							const auto* color = color_image(this->source_, material);
							std::string reason;
							if (color && color->name)
							{
								reason = std::format("colour map \"{}\": {}", color->name, image_read_failure(*color));
							}
							else if (!material->textureCount)
							{
								reason = "no loaded zone holds more than a reference to it";
							}
							else
							{
								// The textures that hold no colour map: semantic, shader name characters and image.
								reason = std::format("no colour map in {} textures:", material->textureCount);
								for (auto t = 0; t < material->textureCount; t++)
								{
									const auto& texture = material->textureTable[t];
									const auto* image = this->source_.resolve(iw7::ASSET_TYPE_IMAGE, texture.image);
									reason += std::format(" {}/{}{} {}", static_cast<int>(texture.semantic), texture.nameStart, texture.nameEnd,
										image && image->name ? image->name : "?");
								}
							}

							ZONETOOL_INFO("iw7: flare material \"%s\" cannot be read: %s", asset_name(material->name).data(), reason.data());
							this->drop("flare whose image cannot be read");
							continue;
						}

						if (material && flare)
						{
							// Replay draws flares through flare materials (MTL_TYPE_EFFECT_FLARE: screen-space flare
							// vertices); IW7's lens-flare materials converted as effect sprites drew those vertices as
							// screen-wide quads through the screen centre (Breakneck's sun flare). Flares take Replay's
							// own of the role (common_mp) with their stock material data.
							const auto& stock = stock_flare(asset_name(material->name));
							entries.push_back({{"entry", hex(std::array<std::uint8_t, 0x20>{}.data(), 0x20)},
								{"assets", json::array({{0, "stockmaterial", stock.material}})}});
							const auto bytes = unhex(stock.material_data);
							std::memcpy(result.data() + offsetof(focus::ParticleModuleInitMaterial, materialData), bytes.data(), bytes.size());
						}
						else if (material)
						{
							const auto name = this->output_.add_material(this->source_, const_cast<iw7::Material*>(material));
							this->output_.effect_materials.insert(name);
							if (this->atlas_state_)
							{
								this->output_.atlas_materials.insert(name);
							}

							entries.push_back(linked_asset("material", name));
						}
					}

					if (entries.empty())
					{
						this->drop(list.numAssets ? "material module whose materials the zones do not hold" : "material module that IW7 leaves empty");
						return std::nullopt;
					}

					result.list(offsetof(focus::ParticleModuleInitMaterial, linkedAssets), std::move(entries));
					return result;
				}
				case iw7::PARTICLE_MODULE_INIT_MODEL:
				{
					copy(16);
					auto entries = json::array();
					const auto& list = data.initModel.m_linkedAssetList;
					for (auto a = 0; a < list.numAssets; a++)
					{
						// A model whose surfaces are in no loaded zone or stream file would not export.
						const auto* model = list.assetList[a].model;
						if (model && model_has_surfaces(this->source_, this->source_.resolve(iw7::ASSET_TYPE_XMODEL,
							const_cast<iw7::XModel*>(model))))
						{
							entries.push_back(linked_asset("xmodel", this->output_.add_model(this->source_, const_cast<iw7::XModel*>(model))));
						}
						else if (const auto stock = model && model->name ? replay_debris_model(asset_name(model->name)) : std::nullopt)
						{
							entries.push_back(linked_asset("stockxmodel", *stock));
						}
						else if (model && model->name)
						{
							ZONETOOL_INFO("iw7: effect \"%s\" names model \"%s\", whose surfaces are in no loaded zone or stream file%s",
								this->current_.data(), model->name,
								this->source_.resolve(iw7::ASSET_TYPE_XMODEL, const_cast<iw7::XModel*>(model)) ? "" : " (the model too)");
						}
					}

					if (entries.empty())
					{
						this->drop(list.numAssets ? "model module without models" : "model module that IW7 leaves empty");
						return std::nullopt;
					}

					result.list(offsetof(iw7::ParticleModuleInitModel, m_linkedAssetList), std::move(entries));
					return result;
				}
				case iw7::PARTICLE_MODULE_INIT_PARTICLE_SIM:
				{
					// Replay's particle sim module is IW7's (its list, scale factor and hold-last-frame byte); its sprites draw the
					// animation's baked particles (Afghan's background battle).
					copy(sizeof(iw7::ParticleModuleInitParticleSim));
					auto entries = json::array();
					const auto& list = data.initParticleSim.m_linkedAssetList;
					for (auto a = 0; list.assetList && a < list.numAssets; a++)
					{
						const auto* sim = list.assetList[a].particleSim ? this->source_.resolve(iw7::ASSET_TYPE_PARTICLE_SIM_ANIMATION,
							list.assetList[a].particleSim) : nullptr;
						if (const auto name = sim ? this->add_particle_sim(*sim) : std::nullopt)
						{
							entries.push_back(linked_asset("particlesimanimation", *name));
						}
					}

					if (entries.empty())
					{
						this->drop(list.numAssets ? "particle sim module whose animations the zones do not hold" :
							"particle sim module that IW7 leaves empty");
						return std::nullopt;
					}

					result.list(offsetof(iw7::ParticleModuleInitParticleSim, m_linkedAssetList), std::move(entries));
					return result;
				}
				case iw7::PARTICLE_MODULE_INIT_RUNNER:
				{
					// Replay runners: unit scale and velocity modifiers, the child kept with its parent (converted IW3
					// runners).
					focus::ParticleModuleInitRunner runner{};
					runner.scaleMin = runner.scaleMax = runner.velocityMin = runner.velocityMax = {{1.0f, 1.0f, 1.0f, 0.0f}};
					runner.orientationOptions = 2;
					runner.attachToParent = 1;
					runner.stopChildOnDeath = 1;
					runner.killChildOnDeath = 1;
					runner.legacyOrientationRotation = 1;
					result.set(0, runner);
					base();
					auto entries = this->child_effects(data.initRunner.m_linkedAssetList, 0);
					if (entries.empty())
					{
						this->drop(data.initRunner.m_linkedAssetList.numAssets ? "runner without child effects" : "runner that IW7 leaves empty");
						return std::nullopt;
					}

					result.list(offsetof(focus::ParticleModuleInitRunner, linkedAssets), std::move(entries));
					return result;
				}
				case iw7::PARTICLE_MODULE_INIT_LIGHT_SPOT:
				{
					// Replay's spot (game-test PDB layout): the cone, bulb and intensities of IW7's, the collimation and
					// light definition of stock spots (light_fx_default, collimation pi/2, near plane 8). The spot points
					// along the effect's X axis (sub_141BA6670, like IW7's). IW7's layout is that of x64-zt's IW7
					// structs (Breakneck's vfx_rivet_spotlight_orange: cone 60/50 degrees, bulb 4 by 1, brightness 16, UV
					// 100, near plane 0.5, no shadow values); values outside their range take the stock spots' values.
					const auto& source = data.initLightSpot;
					const auto value = [](const float v, const float low, const float high, const float fallback)
					{
						return std::isfinite(v) && v >= low && v <= high ? v : fallback;
					};

					focus::ParticleModuleInitLightSpot light{};
					light.fovOuter = value(source.m_fovOuter, 0.01f, 3.1f, 1.2217305f);
					light.fovInner = std::min(value(source.m_fovInner, 0.0f, 3.1f, 0.0f), light.fovOuter);
					light.bulbRadius = value(source.m_bulbRadius, 0.01f, 1000.0f, 1.0f);
					light.bulbLength = value(source.m_bulbLength, 0.0f, 1000.0f, 1.0f / 255.0f);
					light.distanceFalloff = 1.0f;
					light.fovCollimation = 1.5707964f;
					light.brightness = value(source.m_brightness, 0.0f, 1000.0f, 1.0f);
					light.intensityUV = value(source.m_intensityUV, 0.0f, 1000.0f, 0.0f);
					light.intensityIR = value(source.m_intensityIR, 0.0f, 1000.0f, 0.0f);
					light.shadowSoftness = value(source.m_shadowSoftness, 0.01f, 10.0f, 0.55f);
					light.shadowBias = value(source.m_shadowBias, 0.01f, 10.0f, 0.4f);
					light.shadowArea = value(source.m_shadowArea, 0.0001f, 10.0f, 0.018f);
					light.shadowNearPlane = value(source.m_shadowNearPlane, 0.1f, 1000.0f, 8.0f);
					light.toneMappingScaleFactor = value(source.m_toneMappingScaleFactor, 0.0f, 100.0f, 1.0f);
					light.disableVolumetric = source.m_disableVolumetric ? 1 : 0;
					light.disableShadowMap = source.m_disableShadowMap ? 1 : 0;
					light.disableDynamicShadows = source.m_disableDynamicShadows ? 1 : 0;
					light.scriptScale = 1;
					{
						const auto* bytes = reinterpret_cast<const std::uint8_t*>(&source);
						ZONETOOL_INFO("iw7: effect \"%s\" spot light module %s", this->current_.data(), hex(bytes, 64).data());
					}

					result.set(0, light);
					base();
					result.list(offsetof(focus::ParticleModuleInitLightSpot, linkedAssets),
						json::array({linked_asset("lightdef", "light_fx_default")}));
					return result;
				}
				case iw7::PARTICLE_MODULE_INIT_LIGHT_OMNI:
				{
					// Replay's omni lights carry their own light shape; IW7's keep the light definition's. The shape is that
					// of converted IW3 lights (stock values), the rest IW7's.
					focus::ParticleModuleInitLightOmni light{};
					const auto& source = data.initLightOmni;
					light.fovOuter = 0.7853981852531433f;
					light.bulbRadius = 2.0f;
					light.bulbLength = 1.0f / 255.0f;
					light.distanceFalloff = 1.0f;
					light.brightness = 1.0f;
					light.intensityIR = source.m_intensityIR;
					light.shadowSoftness = source.m_shadowSoftness;
					light.shadowBias = source.m_shadowBias;
					light.shadowArea = source.m_shadowArea;
					light.toneMappingScaleFactor = source.m_tonemappingScaleFactor;
					light.disableVolumetric = source.m_disableVolumetric;
					light.disableDynamicShadows = 1;
					light.scriptScale = 1;
					result.set(0, light);
					base();
					result.list(offsetof(focus::ParticleModuleInitLightOmni, linkedAssets),
						json::array({linked_asset("lightdef", "light_fx_default")}));
					return result;
				}
				case iw7::PARTICLE_MODULE_INIT_DECAL:
				{
					// IW7 (Frontier's raw modules): fade-in and fade-out times as bytes at 8 and 9 (its plant: 1 and 20), four
					// bytes that its decals leave zero, the projection axis at 15 (5: -Z, along the impact normal, as most
					// stock decals), the lerp colour at 16 and the marks at 32 (model, world and displacement materials).
					// Replay: 16-bit times, no stop fade (0xFFFF, as every stock decal), the marks at 16, the lerp colour at
					// 32, then lerp time, dynamic decal and projection axis; every stock decal names one material in its
					// three slots.
					const auto* bytes = reinterpret_cast<const std::uint8_t*>(&data);
					focus::ParticleModuleInitDecal decal{};
					decal.fadeInTime = bytes[8];
					decal.fadeOutTime = bytes[9];
					decal.stoppableFadeOutTime = 0xFFFF;
					std::memcpy(&decal.lerpColor, bytes + 16, sizeof(decal.lerpColor));
					decal.projectionAxis = bytes[15];
					if (bytes[10] || bytes[11] || bytes[12] || bytes[13] || bytes[14])
					{
						ZONETOOL_INFO("iw7: effect \"%s\" has a decal with unknown bytes %02x %02x %02x %02x %02x", this->current_.data(),
							bytes[10], bytes[11], bytes[12], bytes[13], bytes[14]);
					}

					result.set(0, decal);
					base();
					auto entries = json::array();
					const auto& list = data.initDecal.m_linkedAssetList;
					for (auto a = 0; list.assetList && a < list.numAssets; a++)
					{
						// The world material (wim/), else the model one (mim/).
						const auto& marks = list.assetList[a].decal.materials;
						auto* material = this->source_.resolve(iw7::ASSET_TYPE_MATERIAL, const_cast<iw7::Material*>(marks[1] ? marks[1] : marks[0]));
						if (!material || !material->textureTable || !material->textureCount)
						{
							this->drop("decal without its material");
							continue;
						}

						// A decal draws its colour map (its alpha is the decal's opacity): one that cannot be read (a stub of
						// a pak that is not in the zone folder: Frontier's blood pool) would leave the decal material out.
						const auto* color = color_image(this->source_, material);
						if (!color || !color->name || (color->name[0] != '$' &&
							load_image_faces(*color, DXGI_FORMAT_R8G8B8A8_UNORM, 64).size() != 1))
						{
							this->drop("decal whose colour map cannot be read");
							continue;
						}

						const auto name = this->output_.add_material(this->source_, material);
						this->output_.decal_materials.insert(name);
						std::array<std::uint8_t, 0x20> entry{};
						entries.push_back({{"entry", hex(entry.data(), entry.size())}, {"assets", json::array({
							{0, "material", name}, {8, "material", name}, {16, "material", name}})}});
					}

					if (entries.empty())
					{
						this->drop("decal module without materials");
						return std::nullopt;
					}

					result.list(offsetof(focus::ParticleModuleInitDecal, linkedAssets), std::move(entries));
					return result;
				}
				case iw7::PARTICLE_MODULE_INIT_SOUND:
				{
					// Replay's layout is IW7's, each entry an alias name at +0 (stock entries: the string only). The aliases of
					// the loaded sound banks convert with their sounds (export_sounds); the importer leaves out any that do not.
					copy(sizeof(iw7::ParticleModuleInitSound));
					auto entries = json::array();
					const auto& list = data.initSound.m_linkedAssetList;
					for (auto a = 0; list.assetList && a < list.numAssets; a++)
					{
						const auto* sound = list.assetList[a].sound;
						const auto name = sound ? std::string(asset_name(sound)) : std::string();
						if (name.empty() || !this->bank_aliases().contains(name))
						{
							this->drop("sound of no loaded sound bank");
							continue;
						}

						this->output_.sound_names.insert(name);
						std::array<std::uint8_t, 0x20> entry{};
						entries.push_back({{"entry", hex(entry.data(), entry.size())}, {"assets", json::array()},
							{"strings", json::array({{0, name}})}});
					}

					if (entries.empty())
					{
						this->drop("sound module without sounds");
						return std::nullopt;
					}

					result.list(offsetof(iw7::ParticleModuleInitSound, m_linkedAssetList), std::move(entries));
					return result;
				}
				case iw7::PARTICLE_MODULE_EMISSIVE_GRAPH:
				{
					// IW7's emissive graph is Replay's first version (PARTICLE_EMISSIVE_CURVE_NUM_V1 = 2): an intensity for each
					// of the two sets that module flag 4 picks between. Replay's is the third (10 curves): per set a colour (curves
					// 0-2 and 5-7) times an intensity (3 and 8), which its update (sub_141BBD5E0) writes per particle, and a value
					// (4 and 9) that only states with flag bit 42 read and Init leaves at 0. The colours are white. Exactly one of the
					// tint and the emissive intensity carries the map's light unit: the tint of an unlit effect material
					// (export_effect_material), else the intensities (the glow of lit embers and smoke).
					std::memcpy(result.data(), &data, offsetof(iw7::ParticleModuleEmissiveGraph, m_curves));
					base();
					first_curve();
					constexpr auto curves = offsetof(iw7::ParticleModuleEmissiveGraph, m_curves);
					for (const auto c : {0, 1, 2, 5, 6, 7})
					{
						result.constant_curve(curves + c * 16, 1.0f);
					}

					for (const auto c : {0, 1})
					{
						auto intensity = data.emissiveGraph.m_curves[c];
						intensity.scale *= this->unlit_state_ ? 1.0f : this->light_unit_;
						result.curve(curves + (c ? 8 : 3) * 16, intensity);
					}

					return result;
				}
				case iw7::PARTICLE_MODULE_PHYSICS_LIGHT:
				{
					// Debris that Havok simulates. Replay's layout is IW7's; each entry names a pipeline at +0 (Replay's of the
					// kind of debris, which its common zones hold) and a shape at +8 (IW7's points, which the importer builds as
					// a Havok shape).
					copy(sizeof(iw7::ParticleModulePhysicsLight));
					auto entries = json::array();
					const auto& list = data.physicsLight.m_linkedAssetList;
					for (auto a = 0; list.assetList && a < list.numAssets; a++)
					{
						const auto& fx = list.assetList[a].physicsFXData;
						const auto* pipeline = this->source_.resolve(iw7::ASSET_TYPE_PHYSICS_FX_PIPELINE, fx.physicsFXPipeline);
						// A shape of a zone that is not loaded takes Replay's particle shape (common_mp).
						const auto* shape = this->source_.resolve(iw7::ASSET_TYPE_PHYSICS_FX_SHAPE, fx.physicsFXShape);
						auto shape_name = std::string(default_fx_shape);
						if (shape && shape->vecs && shape->size >= 4)
						{
							shape_name = this->add_shape(*shape);
						}
						else
						{
							ZONETOOL_INFO("iw7: effect \"%s\" takes Replay's %s for a debris shape that the zones do not hold",
								this->current_.data(), default_fx_shape);
						}

						const std::string_view pipeline_name = pipeline && pipeline->name ? asset_name(pipeline->name) : "";
						std::array<std::uint8_t, 0x20> entry{};
						entries.push_back({{"entry", hex(entry.data(), entry.size())}, {"assets", json::array({
							{0, "physicsfxpipeline", replay_pipeline(pipeline_name)}, {8, "physicsfxshape", shape_name}})}});
					}

					if (entries.empty())
					{
						this->drop("debris module that IW7 leaves empty");
						return std::nullopt;
					}

					result.list(offsetof(iw7::ParticleModulePhysicsLight, m_linkedAssetList), std::move(entries));
					return result;
				}
				case iw7::PARTICLE_MODULE_INIT_OCCLUSION_QUERY:
				{
					// Replay adds a depth bias before the fade times and a ray cast flag after the world size. Its stock
					// effects scale their queries by 1 (IW7's glares keep 0, a query of no size), and bias them toward the
					// camera by up to 1 unit: IW7 places its fixture glares about 0.4 units inside the fixture, which hid
					// them from most views and let them flicker on and off.
					const auto& source = data.initOcclusionQuery;
					base();
					auto scale = source.m_scale;
					if (scale.min == 0.0f && scale.max == 0.0f)
					{
						scale.min = 1.0f;
						scale.max = 1.0f;
					}

					result.set(8, 0.5f);
					result.set(12, source.m_fadeInTime);
					result.set(16, source.m_fadeOutTime);
					result.set(20, scale);
					result.set(28, source.m_worldSize);
					return result;
				}
				case iw7::PARTICLE_MODULE_INIT_SPAWN_SHAPE_BOX:
				case iw7::PARTICLE_MODULE_INIT_SPAWN_SHAPE_CYLINDER:
				case iw7::PARTICLE_MODULE_INIT_SPAWN_SHAPE_ELLIPSOID:
				case iw7::PARTICLE_MODULE_INIT_SPAWN_SHAPE_SPHERE:
				case iw7::PARTICLE_MODULE_INIT_SPAWN_SHAPE_MESH:
					return this->convert_spawn_shape(module, result);
				case iw7::PARTICLE_MODULE_INIT_CLOUD:
					// Replay has no cloud element (IW7's clouds draw as sprites) and its cloud module no data.
					return std::nullopt;
				case iw7::PARTICLE_MODULE_TEST_AGE:
				case iw7::PARTICLE_MODULE_TEST_BIRTH:
				case iw7::PARTICLE_MODULE_TEST_DEATH:
				case iw7::PARTICLE_MODULE_TEST_GRAVITY:
				case iw7::PARTICLE_MODULE_TEST_IMPACT:
				case iw7::PARTICLE_MODULE_TEST_POS:
				{
					// Replay inserts scale and velocity modifiers before the event handler, and its tests keep their own
					// value after it (age, gravity state, impact direction, position).
					const auto& source = data.testBirth;
					focus::ParticleModuleTest test{};
					test.moduleIndex = index;
					// Replay's own conversions of IW7 tests (common_mp) orient children relative to the particle and keep
					// their scale and velocity; IW7's option bytes are not Replay's enums.
					test.orientationOptions = 6;
					test.eventHandlerData.nextState = source.m_eventHandlerData.m_nextState;
					test.eventHandlerData.kill = source.m_eventHandlerData.m_kill;
					result.set(0, test);
					base();
					if (module.moduleType == iw7::PARTICLE_MODULE_TEST_AGE)
					{
						result.set(sizeof(focus::ParticleModuleTest), data.testAge.m_age);
					}

					// Birth, death and impact tests start the effects of their list.
					auto entries = this->child_effects(source.m_eventHandlerData.m_linkedAssetList, 0x00010000u);
					result.list(offsetof(focus::ParticleModuleTest, eventHandlerData) + offsetof(focus::ParticleModuleTestEventHandlerData, linkedAssets),
						std::move(entries));
					return result;
				}
				default:
					break;
				}

				this->drop(std::format("module {}", type));
				return std::nullopt;
			}

			// IW7 spawn shapes have no calculated offset in their common part, which Replay has before the offset; the
			// IW7 cylinder keeps it after its radius. Replay centres a box on its offset, and adds curves that scale the shape
			// over the life (constant, scale 0, as Replay's own conversions of IW7 shapes in common_mp).
			std::optional<module_record> convert_spawn_shape(const iw7::ParticleModuleDef& module, module_record& result)
			{
				const auto* bytes = reinterpret_cast<const std::uint8_t*>(&module.moduleData);
				focus::ParticleModuleInitSpawnShape shape{};
				shape.base = {result.record.moduleType, 0, module.moduleData.moduleBase.m_flags};
				shape.axisFlags = bytes[8];
				shape.spawnFlags = bytes[9];
				// IW7 keeps the normal axis as a word; Replay's spawn type is the kind of the shape.
				std::uint32_t normal_axis{};
				std::memcpy(&normal_axis, bytes + 12, 4);
				shape.normalAxis = static_cast<std::uint8_t>(normal_axis);
				std::memcpy(&shape.offset, bytes + 16, 16);
				const auto curves = [&](const std::size_t offset, const int count)
				{
					for (auto c = 0; c < count; c++)
					{
						result.constant_curve(offset + c * 16, 0.0f);
					}
				};

				switch (module.moduleType)
				{
				case iw7::PARTICLE_MODULE_INIT_SPAWN_SHAPE_BOX:
				{
					focus::ParticleModuleInitSpawnShapeBox box{};
					box.base = shape;
					box.base.spawnType = 0;
					box.useBeamInfo = bytes[32];
					std::memcpy(&box.dimensionsMin, bytes + 48, 16);
					std::memcpy(&box.dimensionsMax, bytes + 64, 16);
					for (std::size_t axis = 0; axis < 3; axis++)
					{
						box.base.offset.v[axis] += (box.dimensionsMin.v[axis] + box.dimensionsMax.v[axis]) * 0.5f;
					}

					result.set(0, box);
					curves(offsetof(focus::ParticleModuleInitSpawnShapeBox, curves), 6);
					return result;
				}
				case iw7::PARTICLE_MODULE_INIT_SPAWN_SHAPE_CYLINDER:
				{
					focus::ParticleModuleInitSpawnShapeCylinder cylinder{};
					cylinder.base = shape;
					cylinder.base.spawnType = 1;
					std::memcpy(&cylinder.base.calculationOffset, bytes + 48, 16);
					cylinder.hasRotation = bytes[32];
					cylinder.rotateCalculatedOffset = bytes[33];
					std::memcpy(&cylinder.halfHeight, bytes + 36, 4);
					std::memcpy(&cylinder.radius, bytes + 40, 8);
					std::memcpy(&cylinder.directionQuat, bytes + 64, 16);
					result.set(0, cylinder);
					curves(offsetof(focus::ParticleModuleInitSpawnShapeCylinder, curves), 5);
					return result;
				}
				case iw7::PARTICLE_MODULE_INIT_SPAWN_SHAPE_SPHERE:
				{
					focus::ParticleModuleInitSpawnShapeSphere sphere{};
					sphere.base = shape;
					sphere.base.spawnType = 4;
					std::memcpy(&sphere.radius, bytes + 40, 8);
					result.set(0, sphere);
					curves(offsetof(focus::ParticleModuleInitSpawnShapeSphere, curves), 4);
					return result;
				}
				case iw7::PARTICLE_MODULE_INIT_SPAWN_SHAPE_MESH:
				{
					// IW7: the mesh count at 32, the models at 40, their vertex and triangle counts at 56. No stock Replay
					// effect spawns on a mesh (common zones, Shipment), so Replay's mesh tables are not checked: the
					// particles spawn on the sphere of the first model's bounds (Frontier: the ops table's sun).
					const auto& list = *reinterpret_cast<const iw7::ParticleLinkedAssetListDef*>(bytes + 40);
					const auto* model = list.assetList && list.numAssets ? list.assetList[0].model : nullptr;
					model = model ? this->source_.resolve(iw7::ASSET_TYPE_XMODEL, const_cast<iw7::XModel*>(model)) : nullptr;
					if (!model)
					{
						this->drop("spawn mesh without its model");
						return std::nullopt;
					}

					focus::ParticleModuleInitSpawnShapeSphere sphere{};
					sphere.base = shape;
					sphere.base.base.type = replay_module(iw7::PARTICLE_MODULE_INIT_SPAWN_SHAPE_SPHERE);
					sphere.base.spawnType = 4;
					auto radius = 0.0f;
					for (std::size_t axis = 0; axis < 3; axis++)
					{
						sphere.base.offset.v[axis] += model->bounds.midPoint[axis];
						radius = std::max(radius, model->bounds.halfSize[axis]);
					}

					sphere.radius = {radius, radius};
					result.record.moduleType = sphere.base.base.type;
					result.set(0, sphere);
					curves(offsetof(focus::ParticleModuleInitSpawnShapeSphere, curves), 4);
					ZONETOOL_INFO("iw7: effect \"%s\" spawns on the sphere of model \"%s\" (radius %g) for its spawn mesh",
						this->current_.data(), model->name, radius);
					return result;
				}
				default:
				{
					// The ellipsoid: radius ranges after the common part; Replay's six curves follow them.
					shape.spawnType = 2;
					result.set(0, shape);
					std::memcpy(result.data() + 0x30, bytes + 32, 32);
					curves(0x50, 6);
					return result;
				}
				}
			}

			// The entries of a list of child effects, which convert with the effect.
			json child_effects(const iw7::ParticleLinkedAssetListDef& list, const std::uint32_t selector)
			{
				auto entries = json::array();
				for (auto a = 0; a < list.numAssets; a++)
				{
					const auto* child = list.assetList[a].particleSystem;
					if (!child)
					{
						continue;
					}

					const auto* resolved = this->source_.resolve(iw7::ASSET_TYPE_VFX, const_cast<iw7::ParticleSystemDef*>(child));
					const std::string name(asset_name(resolved ? resolved->name : child->name));
					if (this->convert(name))
					{
						entries.push_back(linked_asset("vfx", name, selector));
					}
					else
					{
						this->drop("child effect that does not convert");
					}
				}

				return entries;
			}
		};
	}

	namespace
	{
		// Decompiles the script files of the loaded zones with gsc-tool (IW7 mode): the source of each, by asset name.
		std::map<std::string, std::string> decompile_scripts(const source_map& source, const std::filesystem::path& folder,
			const std::filesystem::path& gsc_tool)
		{
			std::filesystem::remove_all(folder);
			const auto& script_assets = source.assets(iw7::ASSET_TYPE_SCRIPTFILE);
			if (script_assets.empty())
			{
				return {};
			}

			const auto input = folder / "gscbin";
			for (const auto& [name, header] : script_assets)
			{
				// The format of zonetool's script dumps: the name, the three lengths, the compressed source and the bytecode.
				const auto* script = static_cast<const iw7::ScriptFile*>(header);
				if (!script || !script->name || script->compressedLen < 0 || script->len < 0 || script->bytecodeLen < 0 ||
					(script->compressedLen && !script->buffer) || (script->bytecodeLen && !script->bytecode))
				{
					throw std::runtime_error(std::format("invalid IW7 script asset: {}", name));
				}
				const auto path = input / (name + ".gscbin");
				std::filesystem::create_directories(path.parent_path());
				std::ofstream file(path, std::ios::binary | std::ios::trunc);
				file.write(script->name, std::strlen(script->name) + 1);
				file.write(reinterpret_cast<const char*>(&script->compressedLen), 4);
				file.write(reinterpret_cast<const char*>(&script->len), 4);
				file.write(reinterpret_cast<const char*>(&script->bytecodeLen), 4);
				if (script->compressedLen)
				{
					file.write(script->buffer, script->compressedLen);
				}
				if (script->bytecodeLen)
				{
					file.write(script->bytecode, script->bytecodeLen);
				}
				file.close();
				if (!file)
				{
					throw std::runtime_error(std::format("cannot write IW7 script {}", path.string()));
				}
			}

			std::wstring command = L"\"" + gsc_tool.wstring() + L"\" -m decomp -g iw7 -s pc \"" + input.wstring() + L"\"";
			const auto log = folder / "gsc-tool.log";
			SECURITY_ATTRIBUTES security{sizeof(security), nullptr, TRUE};
			const auto output = CreateFileW(log.c_str(), GENERIC_WRITE, FILE_SHARE_READ, &security, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,
				nullptr);
			if (output == INVALID_HANDLE_VALUE)
			{
				throw std::runtime_error(std::format("cannot create {}", log.string()));
			}

			STARTUPINFOW startup{sizeof(startup)};
			startup.dwFlags = STARTF_USESTDHANDLES;
			startup.hStdOutput = output;
			startup.hStdError = output;
			startup.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
			PROCESS_INFORMATION process{};
			const auto directory = folder.wstring();
			const auto started = CreateProcessW(nullptr, command.data(), nullptr, nullptr, TRUE, CREATE_NO_WINDOW, nullptr, directory.data(),
				&startup, &process);
			CloseHandle(output);
			if (!started)
			{
				throw std::runtime_error(std::format("cannot run gsc-tool: {}", gsc_tool.string()));
			}

			const auto close_process = gsl::finally([&] { CloseHandle(process.hThread); CloseHandle(process.hProcess); });
			const auto wait_result = WaitForSingleObject(process.hProcess, INFINITE);
			DWORD exit_code{};
			if (wait_result != WAIT_OBJECT_0 || !GetExitCodeProcess(process.hProcess, &exit_code))
			{
				throw std::runtime_error(std::format("cannot wait for gsc-tool; see {}", log.string()));
			}
			if (exit_code != 0)
			{
				throw std::runtime_error(std::format("gsc-tool exited with code {}; see {}", exit_code, log.string()));
			}

			std::map<std::string, std::string> result;
			const auto decompiled = folder / "decompiled" / "iw7";
			for (const auto& [name, header] : script_assets)
			{
				const auto path = decompiled / (name + ".gsc");
				std::ifstream file(path, std::ios::binary);
				if (!file)
				{
					throw std::runtime_error(std::format("gsc-tool did not produce {}; see {}", path.string(), log.string()));
				}
				result.emplace(name, std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()));
				if (file.bad())
				{
					throw std::runtime_error(std::format("cannot read decompiled script {}", path.string()));
				}
			}

			return result;
		}

		// A createfx placement of IW7's scripts\engine\utility::create*effect(): its kind, effect id, origin, angles and
		// the other values that it sets.
		struct effect_placement
		{
			std::string kind;
			std::string fxid;
			std::string origin = "0, 0, 0";
			std::string angles = "0, 0, 0";
			std::vector<std::pair<std::string, std::string>> values;
		};

		std::string trim(std::string_view text)
		{
			while (!text.empty() && std::isspace(static_cast<unsigned char>(text.front())))
			{
				text.remove_prefix(1);
			}

			while (!text.empty() && std::isspace(static_cast<unsigned char>(text.back())))
			{
				text.remove_suffix(1);
			}

			return std::string(text);
		}

		// The IW3 names of IW7's createfx calls.
		std::optional<std::string> iw3_create_call(const std::string_view kind)
		{
			if (kind == "createoneshoteffect")
			{
				return "createOneshotEffect";
			}

			if (kind == "createloopeffect")
			{
				return "createLoopEffect";
			}

			if (kind == "createexploder")
			{
				return "createExploder";
			}

			if (kind == "createloopsound")
			{
				return "createLoopSound";
			}

			return std::nullopt;
		}

		// The index after the parenthesis that closes the one at `open`, or npos.
		std::size_t closing(const std::string_view text, const std::size_t open)
		{
			auto depth = 0;
			auto string = false;
			for (auto i = open; i < text.size(); i++)
			{
				if (string)
				{
					string = text[i] != '"' || text[i - 1] == '\\';
					continue;
				}

				string = text[i] == '"';
				depth += text[i] == '(' ? 1 : text[i] == ')' ? -1 : 0;
				if (depth == 0 && text[i] == ')')
				{
					return i + 1;
				}
			}

			return std::string_view::npos;
		}

		// The arguments of a call, split at the commas outside nested parentheses, brackets and strings.
		std::vector<std::string> call_arguments(const std::string_view inner)
		{
			std::vector<std::string> arguments;
			auto depth = 0;
			auto string = false;
			std::size_t start{};
			for (std::size_t i = 0; i < inner.size(); i++)
			{
				if (string)
				{
					string = inner[i] != '"' || inner[i - 1] == '\\';
					continue;
				}

				string = inner[i] == '"';
				depth += inner[i] == '(' || inner[i] == '[' ? 1 : inner[i] == ')' || inner[i] == ']' ? -1 : 0;
				if (depth == 0 && inner[i] == ',')
				{
					arguments.push_back(trim(inner.substr(start, i - start)));
					start = i + 1;
				}
			}

			arguments.push_back(trim(inner.substr(start)));
			return arguments;
		}

		struct script_function
		{
			std::string name;
			std::string parameters;
			std::string body;
		};

		// The functions of a decompiled script, as gsc-tool writes them: "name( parameters )" at the start of a line,
		// then the body in braces.
		std::vector<script_function> script_functions(const std::string& text)
		{
			static const std::regex header(R"re(^(\w+)\(([^)]*)\)\s*$)re");
			std::vector<script_function> functions;
			std::istringstream lines(text);
			std::string line;
			std::optional<script_function> current;
			auto depth = 0;
			while (std::getline(lines, line))
			{
				if (!line.empty() && line.back() == '\r')
				{
					line.pop_back();
				}

				std::smatch match;
				if (!current)
				{
					if (std::regex_match(line, match, header))
					{
						current = script_function{match[1].str(), trim(match[2].str())};
						depth = 0;
					}

					continue;
				}

				for (const auto c : line)
				{
					depth += c == '{' ? 1 : c == '}' ? -1 : 0;
				}

				current->body += line + "\n";
				if (depth == 0 && line.find('}') != std::string::npos)
				{
					functions.push_back(std::move(*current));
					current.reset();
				}
			}

			return functions;
		}

		// Rewrites the calls of `name(` in a script body with `rewrite(arguments)`, which returns their replacement.
		std::string rewrite_calls(const std::string& body, const std::string_view name,
			const std::function<std::string(const std::vector<std::string>&)>& rewrite)
		{
			std::string result;
			std::size_t at{};
			for (auto found = body.find(name); found != std::string::npos; found = body.find(name, at))
			{
				const auto open = found + name.size();
				const auto starts_word = found == 0 || !(std::isalnum(static_cast<unsigned char>(body[found - 1])) ||
					body[found - 1] == '_' || body[found - 1] == '\\' || body[found - 1] == ':');
				if (!starts_word || open >= body.size() || body[open] != '(')
				{
					result += body.substr(at, open - at);
					at = open;
					continue;
				}

				const auto end = closing(body, open);
				if (end == std::string::npos)
				{
					break;
				}

				result += body.substr(at, found - at);
				result += rewrite(call_arguments(std::string_view(body).substr(open + 1, end - open - 2)));
				at = end;
			}

			return result + body.substr(std::min(at, body.size()));
		}

		std::string join(const std::vector<std::string>& values)
		{
			std::string result;
			for (std::size_t i = 0; i < values.size(); i++)
			{
				result += (i ? ", " : "") + values[i];
			}

			return result;
		}

		// The IW3 versions of the IW7 utilities that map scripts call.
		constexpr std::string_view map_script_helpers = R"gsc(
iw7_getstruct( name, key )
{
	structs = common_scripts\utility::getstructarray( name, key );
	if ( structs.size )
		return structs[ 0 ];
	return undefined;
}

iw7_waittill_either( first, second )
{
	self endon( first );
	self waittill( second );
}

iw7_waittill_any_return( a, b, c, d, e, f )
{
	ent = spawnstruct();
	if ( isdefined( a ) )
		self thread iw7_waittill_string( a, ent );
	if ( isdefined( b ) )
		self thread iw7_waittill_string( b, ent );
	if ( isdefined( c ) )
		self thread iw7_waittill_string( c, ent );
	if ( isdefined( d ) )
		self thread iw7_waittill_string( d, ent );
	if ( isdefined( e ) )
		self thread iw7_waittill_string( e, ent );
	if ( isdefined( f ) )
		self thread iw7_waittill_string( f, ent );
	ent waittill( "returned", message );
	ent notify( "die" );
	return message;
}

iw7_waittill_string( message, ent )
{
	ent endon( "die" );
	self waittill( message );
	ent notify( "returned", message );
}

iw7_waittill_any( a, b, c, d, e, f )
{
	self iw7_waittill_any_return( a, b, c, d, e, f );
}

iw7_cointoss()
{
	return randomint( 100 ) >= 50;
}

iw7_random( values )
{
	keys = getarraykeys( values );
	if ( !keys.size )
		return undefined;
	return values[ keys[ randomint( keys.size ) ] ];
}

iw7_array_remove( values, removed )
{
	result = [];
	foreach ( value in values )
	{
		if ( value != removed )
			result[ result.size ] = value;
	}
	return result;
}

iw7_spawn_tag_origin( origin, angles )
{
	if ( !isdefined( origin ) )
		origin = self.origin;
	ent = spawn( "script_model", origin );
	ent setmodel( "tag_origin" );
	if ( isdefined( angles ) )
		ent.angles = angles;
	else if ( isdefined( self.angles ) )
		ent.angles = self.angles;
	return ent;
}

iw7_flag_init( name )
{
	if ( !isdefined( level.iw7_flags ) )
		level.iw7_flags = [];
	level.iw7_flags[ name ] = 0;
}

iw7_flag( name )
{
	return isdefined( level.iw7_flags ) && isdefined( level.iw7_flags[ name ] ) && level.iw7_flags[ name ];
}

iw7_flag_set( name )
{
	if ( !isdefined( level.iw7_flags ) )
		level.iw7_flags = [];
	level.iw7_flags[ name ] = 1;
	level notify( name );
}

iw7_flag_clear( name )
{
	if ( isdefined( level.iw7_flags ) )
		level.iw7_flags[ name ] = 0;
	level notify( name );
}

iw7_flag_wait( name )
{
	while ( !iw7_flag( name ) )
		level waittill( name );
}

iw7_delaythread( delay, function, a, b, c, d )
{
	self thread iw7_delaythread_proc( delay, function, a, b, c, d );
}

iw7_delaythread_proc( delay, function, a, b, c, d )
{
	wait( delay );
	if ( isdefined( d ) )
		self [[ function ]]( a, b, c, d );
	else if ( isdefined( c ) )
		self [[ function ]]( a, b, c );
	else if ( isdefined( b ) )
		self [[ function ]]( a, b );
	else if ( isdefined( a ) )
		self [[ function ]]( a );
	else
		self [[ function ]]();
}

iw7_unsupported( a, b, c, d, e, f )
{
}

iw7_bullettrace( start, end, characters, ignore )
{
	// Replay global_mp ScriptFile 1805: _bullet_trace, ray_trace and their result helpers.
	contents = [ "physicscontents_solid", "physicscontents_sky", "physicscontents_water",
		"physicscontents_vehicle", "physicscontents_item" ];
	if ( characters )
	{
		contents[ contents.size ] = "physicscontents_player";
		contents[ contents.size ] = "physicscontents_actor";
	}
	contents[ contents.size ] = "physicscontents_clipshot";
	contents[ contents.size ] = "physicscontents_glass";
	hits = physics_raycast( start, end, physics_createcontents( contents ), ignore, 0, "physicsquery_closest", 0, undefined );
	if ( hits.size )
		trace = hits[ 0 ];
	else
	{
		trace = [];
		trace[ "fraction" ] = 1;
		trace[ "surfaceflags" ] = 0;
		trace[ "distance" ] = 0;
		trace[ "position" ] = end;
		trace[ "shape_position" ] = end;
		trace[ "normal" ] = ( 0, 0, 0 );
		trace[ "contact_normal" ] = ( 0, 0, 0 );
		trace[ "hittype" ] = "hittype_none";
	}
	surface = physics_getsurfacetypefromflags( trace[ "surfaceflags" ] );
	trace[ "surfaceindex" ] = surface[ "index" ];
	trace[ "surfacetype" ] = surface[ "name" ];
	return trace;
}

iw7_istrue( value )
{
	return isdefined( value ) && value;
}

iw7_string_starts_with( text, start )
{
	if ( text.size < start.size )
		return false;
	return getsubstr( text, 0, start.size ) == start;
}

iw7_array_add( values, value )
{
	values[ values.size ] = value;
	return values;
}

iw7_play_loop_sound_on_entity( alias )
{
	self playloopsound( alias );
}

iw7_array_combine( a, b, c, d )
{
	result = [];
	result = iw7_array_append( result, a );
	result = iw7_array_append( result, b );
	result = iw7_array_append( result, c );
	return iw7_array_append( result, d );
}

iw7_array_append( result, values )
{
	if ( isdefined( values ) )
	{
		foreach ( value in values )
			result[ result.size ] = value;
	}
	return result;
}

iw7_trigger_on( name, key )
{
	if ( isdefined( name ) && isdefined( key ) )
	{
		triggers = getentarray( name, key );
		foreach ( trigger in triggers )
			trigger iw7_trigger_on_proc();
	}
	else
		self iw7_trigger_on_proc();
}

iw7_trigger_on_proc()
{
	if ( isdefined( self.realorigin ) )
		self.origin = self.realorigin;
	self.trigger_off = undefined;
}

iw7_trigger_off( name, key )
{
	if ( isdefined( name ) && isdefined( key ) )
	{
		triggers = getentarray( name, key );
		foreach ( trigger in triggers )
			trigger iw7_trigger_off_proc();
	}
	else
		self iw7_trigger_off_proc();
}

iw7_trigger_off_proc()
{
	if ( !isdefined( self.realorigin ) )
		self.realorigin = self.origin;
	if ( self.origin == self.realorigin )
		self.origin = self.origin + ( 0, 0, -10000 );
	self.trigger_off = 1;
	self notify( "trigger_off" );
}

iw7_exploder( name )
{
	name = "" + name;
	level notify( "exploding_" + name );
	if ( !isdefined( level.mw120r_createfx ) )
		return;
	foreach ( ent in level.mw120r_createfx )
	{
		if ( ent.v[ "type" ] == "exploder" && isdefined( ent.v[ "exploder" ] ) && ( "" + ent.v[ "exploder" ] ) == name )
			ent thread iw7_exploder_activate();
	}
}

iw7_exploder_activate()
{
	if ( isdefined( self.v[ "firefx" ] ) )
		self thread iw7_exploder_fire();
	if ( isdefined( self.v[ "fxid" ] ) && self.v[ "fxid" ] != "No FX" )
		self thread iw7_exploder_cannon();
	else if ( isdefined( self.v[ "soundalias" ] ) && self.v[ "soundalias" ] != "nil" )
		self thread iw7_exploder_sound();
	if ( isdefined( self.v[ "loopsound" ] ) && self.v[ "loopsound" ] != "nil" )
		self thread iw7_exploder_loopsound();
	if ( isdefined( self.v[ "damage" ] ) )
		self thread iw7_exploder_damage();
	if ( isdefined( self.v[ "earthquake" ] ) )
		self thread iw7_exploder_earthquake();
}

iw7_exploder_delay()
{
	if ( !isdefined( self.v[ "delay" ] ) )
		self.v[ "delay" ] = 0;
	low = self.v[ "delay" ];
	high = self.v[ "delay" ] + 0.001;
	if ( isdefined( self.v[ "delay_min" ] ) )
		low = self.v[ "delay_min" ];
	if ( isdefined( self.v[ "delay_max" ] ) )
		high = self.v[ "delay_max" ];
	if ( low > 0 )
		wait randomfloatrange( low, high );
}

iw7_exploder_effect( fxid )
{
	if ( !isdefined( fxid ) || !isdefined( level._effect ) || !isdefined( level._effect[ fxid ] ) )
		return undefined;
	return level._effect[ fxid ];
}

iw7_exploder_cannon()
{
	fx = iw7_exploder_effect( self.v[ "fxid" ] );
	if ( !isdefined( fx ) )
		return;
	forward = anglestoforward( self.v[ "angles" ] );
	up = anglestoup( self.v[ "angles" ] );
	if ( isdefined( self.v[ "repeat" ] ) )
	{
		self thread iw7_exploder_playsound();
		for ( i = 0; i < self.v[ "repeat" ]; i++ )
		{
			playfx( fx, self.v[ "origin" ], forward, up );
			self iw7_exploder_delay();
		}
		return;
	}
	self iw7_exploder_delay();
	if ( isdefined( self.looper ) )
		self.looper delete();
	self.looper = spawnfx( fx, self.v[ "origin" ], forward, up );
	triggerfx( self.looper );
	self iw7_exploder_playsound();
}

iw7_exploder_fire()
{
	fx = iw7_exploder_effect( self.v[ "firefx" ] );
	if ( !isdefined( fx ) )
		return;
	self iw7_exploder_delay();
	playfx( fx, self.v[ "origin" ], anglestoforward( self.v[ "angles" ] ), anglestoup( self.v[ "angles" ] ) );
}

iw7_exploder_playsound()
{
	if ( isdefined( self.v[ "soundalias" ] ) && self.v[ "soundalias" ] != "nil" )
		playsoundatpos( self.v[ "origin" ], self.v[ "soundalias" ] );
}

iw7_exploder_sound()
{
	origin = self.v[ "origin" ];
	alias = self.v[ "soundalias" ];
	self iw7_exploder_delay();
	playsoundatpos( origin, alias );
}

iw7_exploder_loopsound()
{
	if ( isdefined( self.loopsound_ent ) )
	{
		self.loopsound_ent stoploopsound();
		self.loopsound_ent delete();
	}
	origin = self.v[ "origin" ];
	alias = self.v[ "loopsound" ];
	self iw7_exploder_delay();
	self.loopsound_ent = spawn( "script_origin", origin );
	self.loopsound_ent playloopsound( alias );
}

iw7_exploder_damage()
{
	radius = 128;
	if ( isdefined( self.v[ "damage_radius" ] ) )
		radius = self.v[ "damage_radius" ];
	if ( isdefined( self.v[ "delay" ] ) && self.v[ "delay" ] > 0 )
		wait self.v[ "delay" ];
	radiusdamage( self.v[ "origin" ], radius, self.v[ "damage" ], self.v[ "damage" ] );
}

iw7_exploder_earthquake()
{
	self iw7_exploder_delay();
	if ( !isdefined( level.earthquake ) || !isdefined( level.earthquake[ self.v[ "earthquake" ] ] ) )
		return;
	quake = level.earthquake[ self.v[ "earthquake" ] ];
	earthquake( quake[ "magnitude" ], quake[ "duration" ], self.v[ "origin" ], quake[ "radius" ] );
}

iw7_getspawnpointarray( classname )
{
	if ( !isdefined( level.iw7_spawnpoints ) )
		level.iw7_spawnpoints = [];
	if ( !isdefined( level.iw7_spawnpoints[ classname ] ) )
		level.iw7_spawnpoints[ classname ] = getspawnarray( classname );
	return level.iw7_spawnpoints[ classname ];
}
)gsc";

		// IW7's setscriptablepartstate on the converted scriptables (script models): the animation of the state, from the
		// table that iw7_animations fills, as a move of the entity (a model of one moving bone; its collision moves with
		// it) or an animation of the model.
		constexpr std::string_view scriptable_state_helpers = R"gsc(
iw7_setscriptablepartstate( part, state, instant )
{
	if ( isdefined( self.iw7_parts ) && self iw7_scriptable_set( part, state ) )
		return;
	if ( !isdefined( level.iw7_states ) || !isdefined( self.model ) || !isdefined( level.iw7_states[self.model] ) )
		return;
	entry = level.iw7_states[self.model][part + ":" + state];
	if ( !isdefined( entry ) )
		return;
	if ( entry.mode == "move" )
	{
		if ( !isdefined( self.iw7_base ) )
			self.iw7_base = self.origin;
		goal = self.iw7_base + anglestoforward( self.angles ) * entry.to[0] - anglestoright( self.angles ) * entry.to[1] +
			anglestoup( self.angles ) * entry.to[2];
		if ( isdefined( instant ) && instant || entry.time < 0.1 )
			self.origin = goal;
		else
			self moveto( goal, entry.time, entry.time * 0.25, entry.time * 0.25 );
		return;
	}
	self scriptmodelplayanim( entry.animation );
}

iw7_state( mode, animation, to, time )
{
	state = spawnstruct();
	state.mode = mode;
	state.animation = animation;
	state.to = to;
	state.time = time;
	return state;
}
)gsc";

		constexpr std::string_view scriptable_machine_runtime = R"gsc(
// The runtime of the converted IW7 scriptables (iw7_scriptables), after IW7's scriptable system: each part of a piece
// runs the events of its state in turn, and a state change stops the events of the part's state before (effects that
// IW7 kills on exit stop, loop sounds stop). Damage goes to the part of the tag that it hit, else to the first part in a
// health state; a part that took its state's health moves to its next state, and passes its share of the damage to its
// parent.
iw7_mev( t, a, b, c, d, use_tag_angles )
{
	e = spawnstruct();
	e.t = t;
	e.a = a;
	e.b = b;
	e.c = c;
	e.d = d;
	e.use_tag_angles = use_tag_angles;
	return e;
}

iw7_mstate( name, kind, health, mindamage, toparent, events )
{
	s = spawnstruct();
	s.name = name;
	s.kind = kind;
	s.health = health;
	s.mindamage = mindamage;
	s.toparent = toparent;
	s.events = events;
	return s;
}

iw7_mpart( name, tag, parent, states )
{
	p = spawnstruct();
	p.name = name;
	p.tag = tag;
	p.parent = parent;
	p.states = states;
	return p;
}

iw7_scriptable( name, parts )
{
	pieces = getentarray( name, "targetname" );
	foreach ( piece in pieces )
		piece thread iw7_scriptable_think( parts );
}

// An instance that keeps its IW7 targetname (the map script finds it by it): the one at its initial origin.
iw7_scriptable_at( name, origin, parts )
{
	pieces = getentarray( name, "targetname" );
	foreach ( piece in pieces )
	{
		if ( !isdefined( piece.iw7_parts ) && distancesquared( piece.origin, origin ) < 4 )
		{
			piece thread iw7_scriptable_think( parts );
			return;
		}
	}
}

// IW7's setscriptablepartstate on a piece that runs a state machine: the part of that name enters the state of that name.
iw7_scriptable_set( part, state )
{
	part = tolower( part );
	state = tolower( state );
	for ( p = 0; p < self.iw7_parts.size; p++ )
	{
		if ( tolower( self.iw7_parts[p].name ) != part )
			continue;
		for ( s = 0; s < self.iw7_parts[p].states.size; s++ )
		{
			if ( tolower( self.iw7_parts[p].states[s].name ) == state )
			{
				self thread iw7_scriptable_enter( p, s );
				return 1;
			}
		}
	}

	return 0;
}

iw7_scriptable_think( parts )
{
	self endon( "death" );
	self.iw7_parts = parts;
	self.iw7_states = [];
	self.iw7_taken = [];
	self.iw7_attached = [];
	health = 0;
	for ( p = 0; p < parts.size; p++ )
	{
		self.iw7_states[p] = -1;
		self.iw7_taken[p] = 0;
		foreach ( s in parts[p].states )
		{
			if ( s.kind == "health" )
				health = 1;
		}
	}

	for ( p = 0; p < parts.size; p++ )
		self thread iw7_scriptable_enter( p, 0 );
	if ( health )
		self thread iw7_scriptable_damage();
}

iw7_scriptable_enter( part, state )
{
	self notify( "iw7_part_" + part );
	self endon( "iw7_part_" + part );
	self endon( "death" );
	if ( part < 0 || part >= self.iw7_parts.size || state < 0 || state >= self.iw7_parts[part].states.size )
		return;
	self.iw7_states[part] = state;
	self.iw7_taken[part] = 0;
	self iw7_scriptable_run( part, self.iw7_parts[part].states[state].events );
}

iw7_scriptable_run( part, events )
{
	foreach ( e in events )
	{
		if ( e.t == "wait" )
		{
			if ( e.b > e.a )
				wait( randomfloatrange( e.a, e.b ) );
			else if ( e.a > 0 )
				wait( e.a );
		}
		else if ( e.t == "random" )
		{
			if ( randomfloat( 1 ) < e.a )
				self iw7_scriptable_run( part, e.b );
			else
				self iw7_scriptable_run( part, e.c );
		}
		else if ( e.t == "state" )
		{
			self thread iw7_scriptable_enter( e.a, e.b );
			if ( e.a == part )
				return;
		}
		else if ( e.t == "model" )
			self iw7_scriptable_model( part, e.a );
		else if ( e.t == "addmodel" )
			self iw7_scriptable_attach( e.a, e.b );
		else if ( e.t == "fx" )
			self thread iw7_scriptable_fx( part, e.a, e.b, e.c, e.d, e.use_tag_angles );
		else if ( e.t == "sound" )
			self iw7_scriptable_sound( part, e.a );
		else if ( e.t == "anim" )
			self scriptmodelplayanim( e.a );
		else if ( e.t == "hide" )
		{
			if ( isdefined( self gettagorigin( e.a, 1 ) ) )
			{
				if ( e.b )
					self hidepart( e.a );
				else
					self showpart( e.a );
			}
		}
		else if ( e.t == "explosion" )
		{
			origin = self iw7_scriptable_origin( e.a );
			if ( isdefined( self.iw7_attacker ) )
				radiusdamage( origin, e.b, e.c, e.d, self.iw7_attacker, "MOD_EXPLOSIVE" );
			else
				radiusdamage( origin, e.b, e.c, e.d );
		}
		else if ( e.t == "shake" )
			earthquake( e.a, e.b, self.origin, e.c );
		else if ( e.t == "damage" )
		{
			if ( e.b > 0 )
				self iw7_scriptable_hurt( e.a, e.b );
			if ( e.c > 0 && e.d > 0 )
				self thread iw7_scriptable_burn( part, e.a, e.c, e.d );
		}
		else if ( e.t == "force" )
			self thread iw7_scriptable_force( part, e.a, e.b, e.c );
		else if ( e.t == "debris" )
			self iw7_scriptable_debris( e.a, e.b, e.c, e.d );
		else if ( e.t == "script" )
			self notify( e.a, e.b );
	}
}

iw7_scriptable_origin( tag )
{
	if ( tag != "" )
	{
		origin = self gettagorigin( tag, 1 );
		if ( isdefined( origin ) )
			return origin;
	}

	return self.origin;
}

iw7_scriptable_model( part, model )
{
	if ( self.iw7_parts[part].parent >= 0 )
	{
		self iw7_scriptable_attach( self.iw7_parts[part].tag, model );
		return;
	}

	if ( isdefined( self.iw7_prop ) )
		self.iw7_prop delete();
	if ( model == "" )
	{
		self hide();
		self notsolid();
		return;
	}

	// A model that only the push of a last state swaps in (a picture frame that falls, a broken cash register) is a
	// physics prop in the piece's place, which the push launches.
	if ( isdefined( level.iw7_physics_mass[model] ) )
	{
		self.iw7_prop = spawn( "script_model", self.origin );
		self.iw7_prop.angles = self.angles;
		self.iw7_prop setmodel( model );
		self hide();
		self notsolid();
		return;
	}

	self show();
	self solid();
	self setmodel( model );
}

iw7_scriptable_attach( tag, model )
{
	if ( tag == "" || !isdefined( self gettagorigin( tag, 1 ) ) )
		return;
	if ( isdefined( self.iw7_attached[tag] ) )
		self detach( self.iw7_attached[tag], tag );
	self.iw7_attached[tag] = undefined;
	if ( model == "" )
		return;
	self attach( model, tag );
	self.iw7_attached[tag] = model;
}

// The effects play as effect entities at their tags, which every client receives (also one that connects later, as the
// host's own client does after the first states at the start): playfxontag is an entity event that reaches only the
// clients in the game at the time. An effect that IW7 kills on exit is deleted when its part leaves the state.
iw7_scriptable_fx( part, effect, tag, loop, kill, use_tag_angles )
{
	self endon( "iw7_part_" + part );
	self endon( "death" );
	if ( !isdefined( level._effect[effect] ) )
		return;
	origin = self.origin;
	angles = ( 0, 0, 0 );
	if ( use_tag_angles )
		angles = self.angles;
	if ( tag != "" && isdefined( self gettagorigin( tag, 1 ) ) )
	{
		origin = self gettagorigin( tag );
		if ( use_tag_angles )
			angles = self gettagangles( tag );
	}

	fx = spawnfx( level._effect[effect], origin, anglestoforward( angles ), anglestoup( angles ) );
	triggerfx( fx );
	if ( kill )
		self thread iw7_scriptable_fx_stop( part, fx );
	if ( loop <= 0 )
		return;
	if ( loop < 0.05 )
		loop = 0.05;
	for ( ;; )
	{
		wait( loop );
		triggerfx( fx );
	}
}

iw7_scriptable_fx_stop( part, fx )
{
	self waittill( "iw7_part_" + part );
	fx delete();
}

// The compatibility layer plays each alias as its kind (playsound a one-shot, playloopsound a loop): one of the two plays.
iw7_scriptable_sound( part, alias )
{
	if ( alias == "" )
		return;
	self playsound( alias );
	self playloopsound( alias );
	self thread iw7_scriptable_sound_stop( part );
}

iw7_scriptable_sound_stop( part )
{
	self endon( "death" );
	self waittill( "iw7_part_" + part );
	self stoploopsound();
}

// Damage over time (IW7's burning barrels and cars) while the part that started it keeps its state.
iw7_scriptable_burn( part, target, amount, interval )
{
	self endon( "iw7_part_" + part );
	self endon( "death" );
	if ( interval < 0.05 )
		interval = 0.05;
	for ( ;; )
	{
		wait( interval );
		self iw7_scriptable_hurt( target, amount );
	}
}

// IW7 pushes its pieces by physics. A physics prop (iw7_scriptable_model) takes the push as an impulse at its point, both
// in the piece's space; without a Replay physics body the converted piece falls to the ground under it (a fall), or rocks
// on its origin (a push).
iw7_scriptable_force( part, fall, point, vector )
{
	self endon( "death" );
	if ( isdefined( self.iw7_prop ) && self.iw7_parts[part].parent < 0 )
	{
		prop = self.iw7_prop;
		angles = prop.angles;
		origin = prop.origin + anglestoforward( angles ) * point[0] - anglestoright( angles ) * point[1] + anglestoup( angles ) * point[2];
		impulse = anglestoforward( angles ) * vector[0] - anglestoright( angles ) * vector[1] + anglestoup( angles ) * vector[2];
		prop thread iw7_scriptable_launch( origin, impulse );
		prop thread iw7_scriptable_unpushed( prop.origin );
		return;
	}

	if ( fall )
	{
		trace = iw7_bullettrace( self.origin + ( 0, 0, 4 ), self.origin - ( 0, 0, 4096 ), 0, self );
		if ( isdefined( trace ) && isdefined( trace["position"] ) && self.origin[2] - trace["position"][2] > 1 )
		{
			time = sqrt( 2 * ( self.origin[2] - trace["position"][2] ) / 800 );
			self moveto( trace["position"], time, time, 0 );
		}

		return;
	}

	for ( i = 0; i < 4; i++ )
	{
		angle = 8 - i * 2;
		if ( i % 2 )
			angle = 0 - angle;
		self rotateroll( angle, 0.2, 0.05, 0.05 );
		wait 0.2;
	}
}

// IW7's debris (SpawnDynent: dynamic entities that a breaking piece throws): a physics prop of the model, launched at the
// velocity times its mass (Replay's physicslaunchserver gives an impulse a speed of impulse / mass). Debris without a
// physics body (a model that a state also sets), or a launch that creates none, flies its launch arc as a script model.
// iw3compat_physics_prop keeps at most 25 props.
iw7_scriptable_debris( model, tag, velocity, world )
{
	origin = self iw7_scriptable_origin( tag );
	angles = self.angles;
	if ( !world )
		velocity = anglestoforward( angles ) * velocity[0] - anglestoright( angles ) * velocity[1] + anglestoup( angles ) * velocity[2];
	prop = iw3compat_physics_prop( origin, angles, model );
	if ( isdefined( level.iw7_physics_mass[model] ) )
	{
		prop thread iw7_scriptable_launch( origin, velocity * level.iw7_physics_mass[model] );
		prop thread iw7_scriptable_unlaunched( origin, velocity );
	}
	else
		prop thread iw7_scriptable_fly( origin, velocity );
}

// Its own thread: a launch that cannot create the body raises a script error, which ends only this thread.
iw7_scriptable_launch( point, impulse )
{
	self endon( "death" );
	self physicslaunchserver( point, impulse );
}

// A pushed prop that has not moved a quarter second later got no body: it falls to the ground under it instead.
iw7_scriptable_unpushed( start )
{
	self endon( "death" );
	wait 0.25;
	if ( distancesquared( self.origin, start ) > 1 )
		return;
	prop = spawn( "script_model", start );
	prop.angles = self.angles;
	prop setmodel( self.model );
	self delete();
	prop iw7_scriptable_force( 0, 1, ( 0, 0, 0 ), ( 0, 0, 0 ) );
}

// A prop that has not moved a quarter second after its launch got no body: a fresh script model flies the arc instead.
iw7_scriptable_unlaunched( start, velocity )
{
	self endon( "death" );
	wait 0.25;
	if ( distancesquared( self.origin, start ) > 1 )
		return;
	prop = iw3compat_physics_prop( start, self.angles, self.model );
	self delete();
	prop thread iw7_scriptable_fly( start, velocity );
}

iw7_scriptable_fly( start, velocity )
{
	self endon( "death" );
	gravity = 800;
	time = 3;
	previous = start;
	for ( step = 1; step <= 60; step++ )
	{
		t = step * 0.05;
		next = start + velocity * t - ( 0, 0, 0.5 * gravity * t * t );
		if ( step > 2 )
		{
			stop = physicstrace( previous, next );
			if ( distancesquared( stop, next ) > 0.01 )
			{
				time = step * 0.05;
				break;
			}
		}

		previous = next;
	}

	self movegravity( velocity, time );
	self rotatevelocity( ( randomfloatrange( 180, 540 ), randomfloatrange( -360, 360 ), randomfloatrange( 180, 540 ) ), time );
}

iw7_scriptable_damage()
{
	self endon( "death" );
	self setcandamage( 1 );
	for ( ;; )
	{
		self.maxhealth = 1000000;
		self.health = 1000000;
		self waittill( "damage", amount, attacker, direction, point, type, modelname, tagname );
		if ( isdefined( attacker ) )
			self.iw7_attacker = attacker;
		part = -1;
		if ( isdefined( tagname ) && tagname != "" )
		{
			for ( p = 0; p < self.iw7_parts.size && part < 0; p++ )
			{
				if ( tolower( self.iw7_parts[p].tag ) == tolower( tagname ) && self iw7_scriptable_takes( p ) )
					part = p;
			}
		}

		for ( p = 0; p < self.iw7_parts.size && part < 0; p++ )
		{
			if ( self iw7_scriptable_takes( p ) )
				part = p;
		}

		if ( part >= 0 )
			self iw7_scriptable_hurt( part, amount );
	}
}

iw7_scriptable_takes( part )
{
	if ( part < 0 || part >= self.iw7_parts.size )
		return 0;
	state = self.iw7_states[part];
	return state >= 0 && self.iw7_parts[part].states[state].kind == "health";
}

iw7_scriptable_hurt( part, amount )
{
	if ( !self iw7_scriptable_takes( part ) )
		return;
	s = self.iw7_parts[part].states[self.iw7_states[part]];
	if ( amount < s.mindamage )
		return;
	if ( s.toparent > 0 )
		self iw7_scriptable_hurt( self.iw7_parts[part].parent, amount * s.toparent );
	self.iw7_taken[part] = self.iw7_taken[part] + amount;
	if ( self.iw7_taken[part] >= s.health )
		self thread iw7_scriptable_enter( part, self.iw7_states[part] + 1 );
}
)gsc";

		constexpr std::string_view out_of_bounds_runtime = R"gsc(
// IW7's out-of-bounds volumes (the map's OutOfBounds triggers, which its script may name, minus the patches that its
// script spawns): a living player inside one sees Replay's out-of-bounds countdown and dies when it ends. As in IW7
// (scripts\mp\spawnlogic), the limit is 3 s, and the time left after leaving a volume holds for the next one until a
// 3 s cooldown ends. Replay runs its own watcher only in Ground War and Battle Royale.
iw7_out_of_bounds()
{
	level endon( "game_ended" );
	if ( !isdefined( level.iw7_c7b3 ) )
		level.iw7_c7b3 = getentarray( "OutOfBounds", "targetname" );
	for ( ;; )
	{
		wait 0.05;
		if ( !isdefined( level.players ) )
			continue;
		foreach ( player in level.players )
		{
			if ( isdefined( player ) )
				player iw7_out_of_bounds_check();
		}
	}
}

iw7_out_of_bounds_check()
{
	inside = 0;
	if ( isdefined( self.health ) && self.health > 0 && self.sessionstate == "playing" )
	{
		foreach ( trigger in level.iw7_c7b3 )
		{
			if ( isdefined( trigger ) && self istouching( trigger ) )
			{
				inside = 1;
				break;
			}
		}

		if ( inside && isdefined( level.outofboundstriggerpatches ) )
		{
			foreach ( patch in level.outofboundstriggerpatches )
			{
				if ( isdefined( patch ) && self istouching( patch ) )
				{
					inside = 0;
					break;
				}
			}
		}
	}

	if ( inside && !isdefined( self.iw7_oob_end ) )
	{
		left = 3;
		if ( isdefined( self.iw7_oob_left ) && gettime() < self.iw7_oob_cooldown )
			left = self.iw7_oob_left;
		self.iw7_oob_end = gettime() + int( left * 1000 );
		self setclientomnvar( "ui_out_of_bounds_type", 1 );
		self setclientomnvar( "ui_out_of_bounds_countdown", self.iw7_oob_end );
	}
	else if ( inside && gettime() >= self.iw7_oob_end )
	{
		self iw7_out_of_bounds_clear();
		self.iw7_oob_left = undefined;
		self dodamage( 100000, self.origin );
	}
	else if ( !inside && isdefined( self.iw7_oob_end ) )
	{
		self.iw7_oob_left = ( self.iw7_oob_end - gettime() ) / 1000;
		self.iw7_oob_cooldown = gettime() + 3000;
		self iw7_out_of_bounds_clear();
	}
}

iw7_out_of_bounds_clear()
{
	self.iw7_oob_end = undefined;
	self setclientomnvar( "ui_out_of_bounds_type", 0 );
	self setclientomnvar( "ui_out_of_bounds_countdown", 0 );
}
)gsc";

		// The map's own script (scripts\mp\maps\<map>\<map>) as the IW3 map script that the pipeline translates: the
		// functions that its main runs, with IW7's utilities replaced by the IW3 ones or their IW3 versions. Calls
		// of other IW7 scripts drop out in the pipeline. The animations that the script names (add_animation: their
		// lengths) play on its script models by name, as Replay's script models play theirs; cinematics do not convert, and
		// scriptable states set the states of the animated scriptables (iw7_setscriptablepartstate). Adds the effects that
		// it loads.
		std::string translate_map_script(const std::string& map, const std::string& text, const std::map<std::string, std::string>& scripts,
			std::set<std::string>& effects, const std::function<bool(const std::string&)>& add_model,
			const std::function<std::optional<float>(const std::string&)>& add_animation)
		{
			// The shared IW7 scripts that convert with the map script (their functions join it; only those that the map
			// script reaches are written): the animation suite, which rotates and moves the map's animObj entities, and the
			// MP utilities (Frontier's zero-g ragdolls, physics_setgravityragdollscalar). gsc-tool names a script whose name
			// it does not know by its id, so the script is the one that defines the functions that the map script calls.
			auto own = text;
			std::vector<script_function> shared;
			std::vector<std::string> paths{R"(scripts\mp\animation_suite)", R"(scripts\mp\utility)"};
			// The map's other scripts convert with it too (Rivet's drone lights in mp_rivet_fx), but for their main: the
			// pipeline's own effect script and createfx take the place of the effect script's main.
			const auto local = std::format(R"(scripts\mp\maps\{}\)", map);
			for (auto at = own.find(local); at != std::string::npos; at = own.find(local, at + 1))
			{
				const auto end = own.find("::", at);
				const auto path = end == std::string::npos ? std::string{} : own.substr(at, end - at);
				if (!path.empty() && path != local + map && std::ranges::find(paths, path) == paths.end())
				{
					paths.push_back(path);
				}
			}

			const auto identifier = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";
			for (const auto& path : paths)
			{
				const bool map_script = path.starts_with(local);
				const auto prefix = path + "::";
				std::set<std::string> called;
				for (auto at = own.find(prefix); at != std::string::npos; at = own.find(prefix, at + 1))
				{
					const auto start = at + prefix.size();
					const auto end = own.find_first_not_of(identifier, start);
					if (const auto name = own.substr(start, end - start); !(map_script && name == "main"))
					{
						called.insert(name);
					}
				}

				if (called.empty())
				{
					continue;
				}

				const std::string* found{};
				for (const auto& [name, source] : scripts)
				{
					const auto defined = script_functions(source);
					if (std::ranges::all_of(called, [&](const auto& function)
					{
						return std::ranges::any_of(defined, [&](const auto& definition) { return definition.name == function; });
					}))
					{
						found = &source;
						break;
					}
				}

				if (!found)
				{
					ZONETOOL_WARNING("iw7: the map script runs %s, whose script is not in the loaded zones", path.data());
					continue;
				}

				for (auto at = own.find(prefix); at != std::string::npos; at = own.find(prefix, at + 1))
				{
					const auto start = at + prefix.size();
					if (called.contains(own.substr(start, own.find_first_not_of(identifier, start) - start)))
					{
						own.erase(at, prefix.size());
					}
				}

				auto defined = script_functions(*found);
				if (map_script)
				{
					std::erase_if(defined, [](const script_function& function) { return function.name == "main"; });
				}

				shared.insert(shared.end(), defined.begin(), defined.end());
				ZONETOOL_INFO("iw7: the map script runs %s, which converts with it (%zu functions)", path.data(), defined.size());
			}

			auto functions = script_functions(own);
			for (auto& function : shared)
			{
				if (std::ranges::any_of(functions, [&](const auto& definition) { return definition.name == function.name; }))
				{
					ZONETOOL_WARNING("iw7: the shared function %s has the name of a map script function; the map script's stays",
						function.name.data());
					continue;
				}

				functions.push_back(std::move(function));
			}
			std::map<std::string, const script_function*> by_name;
			for (const auto& function : functions)
			{
				by_name.emplace(function.name, &function);
			}

			// The functions that main reaches.
			std::set<std::string> reached;
			std::vector<std::string> pending{"main"};
			static const std::regex reference(R"re((?:^|[^\w\\:])(\w+)\s*\(|::(\w+))re");
			while (!pending.empty())
			{
				const auto name = pending.back();
				pending.pop_back();
				const auto found = by_name.find(name);
				if (found == by_name.end() || !reached.insert(name).second)
				{
					continue;
				}

				const auto& body = found->second->body;
				for (auto it = std::sregex_iterator(body.begin(), body.end(), reference); it != std::sregex_iterator(); ++it)
				{
					pending.push_back((*it)[1].matched ? (*it)[1].str() : (*it)[2].str());
				}
			}

			static const std::regex dropped_main(
				R"re(\s*scripts\\mp\\(?:maps\\\w+\\[\w\\]+|load|compass)::\w+\([^;]*\);)re");
			static const std::regex loaded(R"re(loadfx\(\s*"([^"]+?)(?:\.vfx)?"\s*\))re");
			std::string result = std::format("// The map script (IW7 scripts\\mp\\maps\\{0}\\{0}).\n\n", map);
			// The lengths of the animations that the script names, for iw7_getanimlength (written when the script calls it).
			std::map<std::string, float> animation_lengths;
			bool takes_lengths{};
			bool variable_models{};
			const auto animation = [&](const std::string& name)
			{
				const auto length = add_animation(name);
				if (length)
				{
					animation_lengths.emplace(name, *length);
				}

				return length.has_value();
			};

			for (const auto& function : functions)
			{
				if (!reached.contains(function.name))
				{
					continue;
				}

				auto body = function.body;
				// IW7's animation references (%name: Rivet's drone paths) as names, which Replay's script models play
				// (scriptmodelplayanim and scriptmodelplayanimdeltamotion take the name of the XAnimParts). A reference
				// whose animation does not convert stays one, which the pipeline drops (undefined).
				static const std::regex animation_reference(R"re(%([A-Za-z_]\w*))re");
				std::string named;
				auto last = body.cbegin();
				for (auto it = std::sregex_iterator(body.begin(), body.end(), animation_reference); it != std::sregex_iterator(); ++it)
				{
					named.append(last, (*it)[0].first);
					named += animation((*it)[1].str()) ? std::format("\"{}\"", (*it)[1].str()) : (*it)[0].str();
					last = (*it)[0].second;
				}

				named.append(last, body.cend());
				body = std::move(named);
				if (function.name == "main")
				{
					body = std::regex_replace(body, dropped_main, "");
					const auto open = body.find('{');
					body.insert(open + 1, std::format("\n    maps\\mp\\{0}_fx::main();\n    maps\\createfx\\{0}_fx::main();\n"
						"    maps\\mp\\_load::main();", map));
				}

				body = rewrite_calls(body, "scripts\\engine\\utility::getfx", [](const std::vector<std::string>& arguments)
				{
					return "level._effect[ " + join(arguments) + " ]";
				});

				// IW7's exploder( name, players, start time ) runs the createfx exploders of the name (iw7_exploder).
				body = rewrite_calls(body, "scripts\\engine\\utility::exploder", [](const std::vector<std::string>& arguments)
				{
					return "iw7_exploder( " + arguments.front() + " )";
				});

				body = rewrite_calls(body, "bullettrace", [](const std::vector<std::string>& arguments)
				{
					return "iw7_bullettrace( " +
						join(std::vector<std::string>(arguments.begin(), arguments.begin() + std::min<std::size_t>(arguments.size(), 4))) + " )";
				});

				// IW7 method 0x8113, which gsc-tool does not name, returns the corpse of a player that has died (inferred from
				// its uses: Junk's grinder and Rally's gas hide it and play a gib effect at it): Replay's getcorpseentity.
				// IW7 methods 0x84A4 and 0x84A6 sit on either side of setusefov (0x84A5), as sethintdisplayrange and
				// sethintdisplayfov do in Replay's table; Carnage's and Rally's gas buttons call them beside setuserange and
				// setusefov with the same values (inferred).
				for (const auto& [from, to] : std::initializer_list<std::pair<std::string_view, std::string_view>>{
					{"_meth_8113(", "getcorpseentity("},
					{"_meth_84A4(", "sethintdisplayrange("},
					{"_meth_84A6(", "sethintdisplayfov("},
				})
				{
					for (auto at = body.find(from); at != std::string::npos; at = body.find(from, at))
					{
						body.replace(at, from.size(), to);
					}
				}

				for (const auto& [from, to] : std::initializer_list<std::pair<std::string_view, std::string_view>>{
					{"scripts\\engine\\utility::getstructarray(", "common_scripts\\utility::getstructarray("},
					{"scripts\\engine\\utility::waitframe(", "waitframe("},
					{"scripts\\engine\\utility::array_thread(", "common_scripts\\utility::array_thread("},
					{"scripts\\mp\\utility::istrue(", "iw7_istrue("},
					{"scripts\\engine\\utility::is_true(", "iw7_istrue("},
					{"scripts\\mp\\spawnlogic::getspawnpointarray(", "iw7_getspawnpointarray("},
				})
				{
					for (auto at = body.find(from); at != std::string::npos; at = body.find(from, at + to.size()))
					{
						body.replace(at, from.size(), to);
					}
				}

				// The utilities that the script's own IW3 versions replace (map_script_helpers).
				for (const auto name : {"getstruct", "waittill_either", "waittill_any_return", "waittill_any", "cointoss",
					"random", "array_remove", "spawn_tag_origin", "flag_init", "flag_set", "flag_clear", "flag_wait", "flag",
					"delaythread", "string_starts_with", "array_add", "play_loop_sound_on_entity", "array_combine", "trigger_on",
					"trigger_off"})
				{
					const auto from = std::format("scripts\\engine\\utility::{}(", name);
					const auto to = std::format("iw7_{}(", name);
					for (auto at = body.find(from); at != std::string::npos; at = body.find(from, at + to.size()))
					{
						body.replace(at, from.size(), to);
					}
				}

				// A model that the script names by a string converts with the map's models; one that the zones do not
				// hold would stop the script. A model that a variable names (Rivet's drone panels: setmodel( random(
				// level.panels ) )) sets through iw7_setmodel, which sets only the converted models that the script's
				// strings name; precaching one is not needed.
				for (const auto name : {"precachemodel", "setmodel"})
				{
					body = rewrite_calls(body, name, [&](const std::vector<std::string>& arguments)
					{
						const auto& model = arguments.front();
						const auto literal = arguments.size() == 1 && model.size() > 2 && model.starts_with('"') && model.ends_with('"');
						if (literal)
						{
							return std::format("{}( {} )", add_model(model.substr(1, model.size() - 2)) ? name : "iw7_unsupported",
								join(arguments));
						}

						const auto sets = std::string_view(name) == "setmodel" && arguments.size() == 1;
						variable_models |= sets;
						return std::format("{}( {} )", sets ? "iw7_setmodel" : "iw7_unsupported", join(arguments));
					});
				}

				// A sound on a moving entity plays as IW3's playsound, which the pipeline passes for the converted aliases.
				body = rewrite_calls(body, "playsoundonmovingent", [](const std::vector<std::string>& arguments)
				{
					return "playsound( " + join(arguments) + " )";
				});

				// The converted scriptables are script models of their targetname, whose states the table of
				// iw7_animations converts.
				body = rewrite_calls(body, "setscriptablepartstate", [](const std::vector<std::string>& arguments)
				{
					return arguments.size() >= 2 ? std::format("iw7_setscriptablepartstate( {}, {} )", arguments[0], arguments[1])
						: "iw7_unsupported( " + join(arguments) + " )";
				});

				for (auto at = body.find("getscriptablearray("); at != std::string::npos; at = body.find("getscriptablearray(", at))
				{
					body.replace(at, std::string_view("getscriptablearray(").size(), "getentarray(");
				}

				for (const auto name : {"playcinematicforalllooping", "precachempanim"})
				{
					body = rewrite_calls(body, name, [](const std::vector<std::string>& arguments)
					{
						return "iw7_unsupported( " + join(arguments) + " )";
					});
				}

				// An animation that a script model plays by its name converts (add_animation); one that does not would stop
				// the script, as would one that scriptmodelplayanim takes from a variable (IW7's names IW7 animations).
				for (const auto name : {"scriptmodelplayanim", "scriptmodelplayanimdeltamotion"})
				{
					const bool by_variable = std::string_view(name) == "scriptmodelplayanimdeltamotion";
					body = rewrite_calls(body, name, [&](const std::vector<std::string>& arguments)
					{
						const auto& anim = arguments.front();
						const auto literal = anim.size() > 2 && anim.starts_with('"') && anim.ends_with('"');
						return std::format("{}( {} )", (literal ? animation(anim.substr(1, anim.size() - 2)) : by_variable) ? name :
							"iw7_unsupported", join(arguments));
					});
				}

				// Replay's getanimlength takes an animation, which the converted scripts name: the lengths of the animations
				// that they name (iw7_getanimlength).
				body = rewrite_calls(body, "getanimlength", [&](const std::vector<std::string>& arguments)
				{
					takes_lengths = true;
					return "iw7_getanimlength( " + join(arguments) + " )";
				});

				for (auto it = std::sregex_iterator(body.begin(), body.end(), loaded); it != std::sregex_iterator(); ++it)
				{
					effects.insert((*it)[1].str());
				}

				body = std::regex_replace(body, loaded, "loadfx( \"$1\" )");
				result += std::format("{}({})\n{}\n", function.name, function.parameters.empty() ? "" : " " + function.parameters + " ",
					body);
			}

			// gsc-tool writes the names that it has no text for as their IW7 token ids (_id_XXXX), which compile to the
			// unrelated IW8 tokens of those ids: the script's own functions and fields take names of their own.
			static const std::regex token_name(R"re(\b_id_([0-9A-Fa-f]+)\b)re");
			std::string lengths;
			if (takes_lengths)
			{
				lengths = "\niw7_getanimlength( animation )\n{\n";
				for (const auto& [name, length] : animation_lengths)
				{
					lengths += std::format("\tif ( animation == \"{}\" )\n\t\treturn {:.4f};\n", name, length);
				}

				lengths += "\treturn undefined;\n}\n";
			}

			// The converted models that the script's strings name, which iw7_setmodel sets.
			std::string models;
			if (variable_models)
			{
				static const std::regex string_literal(R"re("([A-Za-z0-9_]+)")re");
				std::set<std::string> checked;
				std::set<std::string> converted;
				for (const auto& function : functions)
				{
					if (!reached.contains(function.name))
					{
						continue;
					}

					for (auto it = std::sregex_iterator(function.body.begin(), function.body.end(), string_literal);
						it != std::sregex_iterator(); ++it)
					{
						if (const auto name = (*it)[1].str(); checked.insert(name).second && add_model(name))
						{
							converted.insert(name);
						}
					}
				}

				models = "\niw7_setmodel( model )\n{\n";
				if (!converted.empty())
				{
					models += "\tswitch ( model )\n\t{\n";
					for (const auto& name : converted)
					{
						models += std::format("\tcase \"{}\":\n", name);
					}

					models += "\t\tself setmodel( model );\n\t\tbreak;\n\t}\n";
				}

				models += "}\n";
			}

			return std::regex_replace(result, token_name, "iw7_$1") + std::string(map_script_helpers) +
				std::string(scriptable_state_helpers) + lengths + models;
		}
	}

	// IW7 places the ambient effects of a map with the createfx script of the map (scripts\mp\maps\<map>\gen\<map>_fx,
	// whose calls set up each placement) and loads their particle systems in another of its scripts (level._effect[id] =
	// loadfx(path)). Both become IW3 map scripts of the extraction, as an IW3 map has them (maps\mp\<map>_fx and
	// maps\createfx\<map>_fx, which maps\mp\<map> runs), and the particle systems become native Replay effects.
	void export_effects(const source_map& source, extraction& output, const zone_conversion& conversion)
	{
		const auto tool = conversion.options.find("gsc_tool");
		if (tool == conversion.options.end() || tool->second.empty())
		{
			ZONETOOL_WARNING("iw7: no --gsc_tool to read the map scripts with; the map has no effects");
			return;
		}

		const auto scripts = decompile_scripts(source, output.root / "scripts", tool->second);
		static const std::regex registration(R"re(level\._effect\[\s*"([^"]+)"\s*\]\s*=\s*loadfx\(\s*"([^"]+)"\s*\);)re");
		static const std::regex creation(R"re((\w+)\s*=\s*scripts\\engine\\utility::(create\w+)\(\s*"([^"]+)"\s*\);)re");
		static const std::regex placement(
			R"re((\w+)\s+scripts\\common\\createfx::set_origin_and_angles\(\s*\(([^)]*)\)\s*,\s*\(([^)]*)\)\s*\);)re");
		static const std::regex value(R"re((\w+)\.v\[\s*"(\w+)"\s*\]\s*=\s*([^;]+);)re");
		// The sound script of the map (gen\<map>_sound) places its emitters with createfx's own calls.
		static const std::regex sound_creation(R"re((\w+)\s*=\s*scripts\\common\\createfx::(createloopsound)\(\s*\);)re");

		std::map<std::string, std::string> effects;
		std::vector<effect_placement> placements;
		std::size_t unknown_calls{};
		for (const auto& [name, text] : scripts)
		{
			for (auto it = std::sregex_iterator(text.begin(), text.end(), registration); it != std::sregex_iterator(); ++it)
			{
				auto path = (*it)[2].str();
				if (path.ends_with(".vfx"))
				{
					path.resize(path.size() - 4);
				}

				effects.try_emplace((*it)[1].str(), path);
			}

			// A placement script: its statements, in order.
			if (text.find("createfx::set_origin_and_angles") == std::string::npos)
			{
				continue;
			}

			std::unordered_map<std::string, std::size_t> current;
			std::istringstream lines(text);
			for (std::string line; std::getline(lines, line);)
			{
				std::smatch match;
				if (std::regex_search(line, match, creation))
				{
					if (!iw3_create_call(match[2].str()))
					{
						unknown_calls++;
						current.erase(match[1].str());
						continue;
					}

					current[match[1].str()] = placements.size();
					placements.push_back({match[2].str(), match[3].str()});
				}
				else if (std::regex_search(line, match, sound_creation))
				{
					current[match[1].str()] = placements.size();
					placements.push_back({match[2].str(), {}});
				}
				else if (std::regex_search(line, match, placement))
				{
					if (const auto found = current.find(match[1].str()); found != current.end())
					{
						placements[found->second].origin = trim(match[2].str());
						placements[found->second].angles = trim(match[3].str());
					}
				}
				else if (std::regex_search(line, match, value))
				{
					if (const auto found = current.find(match[1].str()); found != current.end())
					{
						placements[found->second].values.emplace_back(match[2].str(), trim(match[3].str()));
					}
				}
			}
		}

		// Start first-state Scriptable effects when the state enters, without inventing a createfx pre-roll.
		for (const auto& effect : output.scriptable_effects)
		{
			effects.try_emplace(effect.effect, effect.effect);
			effect_placement scripted{"createoneshoteffect", effect.effect};
			scripted.origin = std::format("{}, {}, {}", effect.origin[0], effect.origin[1], effect.origin[2]);
			scripted.angles = std::format("{}, {}, {}", effect.angles[0], effect.angles[1], effect.angles[2]);
			scripted.values.emplace_back("delay", "0");
			placements.push_back(std::move(scripted));
		}

		// The particle systems of the placed effects, and of the effects that the map's own script loads.
		std::set<std::string> placed;
		for (const auto& entry : placements)
		{
			if (const auto found = effects.find(entry.fxid); found != effects.end())
			{
				placed.insert(found->second);
			}
		}

		std::string map_script;
		if (const auto found = scripts.find(std::format("scripts/mp/maps/{0}/{0}", source.name())); found != scripts.end())
		{
			// The lighting dvars of the map script (the last value it sets wins, as at run time).
			static const std::regex grid_dvar(R"re(setdvar\(\s*"(r_lightGridEnableTweaks|r_lightGridIntensity|r_volumetrics)"\s*,\s*(-?[0-9]*\.?[0-9]+)\s*\);)re",
				std::regex::icase);
			auto tweaks = false;
			auto intensity = 1.0f;
			for (auto it = std::sregex_iterator(found->second.begin(), found->second.end(), grid_dvar); it != std::sregex_iterator(); ++it)
			{
				const auto setting = std::stof((*it)[2].str());
				if (_stricmp((*it)[1].str().data(), "r_lightGridEnableTweaks") == 0)
				{
					tweaks = setting != 0.0f;
				}
				else if (_stricmp((*it)[1].str().data(), "r_volumetrics") == 0)
				{
					output.volumetrics = setting != 0.0f;
				}
				else
				{
					intensity = setting;
				}
			}

			if (tweaks && intensity > 0.0f && std::isfinite(intensity))
			{
				output.light_grid_intensity = intensity;
				ZONETOOL_INFO("iw7: the map script sets the light-grid intensity to %g", intensity);
			}

			// IW7 Frontier's script turns volumetrics off (r_volumetrics 0). Its lights converted without volumetrics and the
			// converted script set the dvar too: Frontier's lit areas went dark up close and flickered from afar, which no map
			// with volumetric lights showed (Scorch, owner test 2026-10-22). The lights keep IW7's own setting and the converted
			// script leaves Replay's dvar alone; the vision has no volumetric air, so volumetric lights add no haze.
			if (!output.volumetrics)
			{
				ZONETOOL_INFO("iw7: the map script turns volumetric lighting off; the converted map keeps Replay's volumetrics");
			}

			std::set<std::string> loaded;
			map_script = translate_map_script(source.name(), found->second, scripts, loaded, [&](const std::string& name)
			{
				const auto model = static_cast<const iw7::XModel*>(source.find(iw7::ASSET_TYPE_XMODEL, name));
				if (model)
				{
					output.models.try_emplace(file_name(name), model);
					// Replay's common zones hold tag_origin, which its own scripts set.
					if (name != "tag_origin")
					{
						output.script_models.insert(file_name(name));
						output.map_script_models.insert(file_name(name));
					}
				}

				return model != nullptr;
			}, [&](const std::string& name) { return export_script_animation(source, name, output); });
			static const std::regex volumetrics_dvar(R"re([ \t]*setdvar\(\s*"r_volumetrics"\s*,[^;]*\);[ \t]*\r?\n?)re", std::regex::icase);
			map_script = std::regex_replace(map_script, volumetrics_dvar, "");
			ZONETOOL_INFO("iw7: translated the map script (%zu bytes) with %zu effects of its own", map_script.size(), loaded.size());
			placed.insert(loaded.begin(), loaded.end());

			// The animation suite plays the effect that each animObj entity names by its script_noteworthy (vfx_...).
			if (map_script.find("setupvfxobjs(") != std::string::npos)
			{
				std::ifstream file(output.extracted / (bsp_name(source) + ".ents"), std::ios::binary);
				const std::string entities((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
				static const std::regex block(R"re(\{([^{}]*)\})re");
				static const std::regex noteworthy(R"re("script_noteworthy" "(vfx_\w+)")re");
				for (auto it = std::sregex_iterator(entities.begin(), entities.end(), block); it != std::sregex_iterator(); ++it)
				{
					const auto body = (*it)[1].str();
					std::smatch match;
					if (body.find("\"targetname\" \"animObj\"") == std::string::npos || !std::regex_search(body, match, noteworthy))
					{
						continue;
					}

					if (const auto effect = effects.find(match[1].str()); effect != effects.end())
					{
						placed.insert(effect->second);
					}
				}
			}
		}

		// The map script that the generated parts join: IW7 maps run their effects and createfx scripts first.
		const auto start_thread = [&](const std::string& call)
		{
			if (map_script.empty())
			{
				map_script = std::format("// The map script (IW7 maps run their effects and createfx scripts first).\nmain()\n{{\n"
					"\tmaps\\mp\\{0}_fx::main();\n\tmaps\\createfx\\{0}_fx::main();\n\tmaps\\mp\\_load::main();\n}}\n", source.name());
			}

			const std::string load = "maps\\mp\\_load::main();";
			if (const auto at = map_script.find(load); at != std::string::npos)
			{
				map_script.insert(at + load.size(), "\n    thread " + call + "();");
			}
		};

		// The animated scriptables (export_animations): the map script fills the table of their states, which IW7's
		// setscriptablepartstate (translated) sets, and sets the first state of each part from the start. Their effects play
		// at the bind pose of their tags (export_entities).
		if (!output.scriptable_animations.empty())
		{
			std::string calls = "\tlevel.iw7_states = [];\n";
			for (const auto& [model, states] : output.scriptable_states)
			{
				calls += std::format("\tlevel.iw7_states[\"{}\"] = [];\n", model);
				for (const auto& state : states)
				{
					calls += std::format("\tlevel.iw7_states[\"{}\"][\"{}:{}\"] = iw7_state( \"{}\", \"{}\", ( {}, {}, {} ), {} );\n", model,
						state.part, state.state, state.moves ? "move" : "anim", asset_name(state.anim->name), state.to[0], state.to[1],
						state.to[2], state.time);
				}
			}

			for (const auto& [targetname, anim] : output.scriptable_animations)
			{
				std::set<std::pair<std::string, std::string>> first;
				for (const auto& model : output.scriptable_animation_models[targetname])
				{
					for (const auto& state : output.scriptable_states[model])
					{
						if (state.first)
						{
							first.emplace(state.part, state.state);
						}
					}
				}

				for (const auto& [part, state] : first)
				{
					calls += std::format("\tiw7_first_state( \"{}\", \"{}\", \"{}\" );\n", targetname, part, state);
				}
			}

			start_thread("iw7_animations");
			map_script += "\n// The scriptables that animate: IW7 plays the animation of each part's first state from the start.\n"
				"iw7_animations()\n{\n" + calls + "}\n" + std::string(R"gsc(
iw7_first_state( name, part, state )
{
    pieces = getentarray( name, "targetname" );
    foreach ( piece in pieces )
        piece iw7_setscriptablepartstate( part, state, 1 );
}
)gsc");
			if (map_script.find("iw7_setscriptablepartstate( part, state, instant )") == std::string::npos)
			{
				map_script += scriptable_state_helpers;
			}

			ZONETOOL_INFO("iw7: the map script animates %zu kinds of scriptables (%zu models)", output.scriptable_animations.size(),
				output.scriptable_states.size());
		}

		// The scriptables that break (export_entities): the map script breaks each when it took its health in damage. Their
		// effects load by their own names.
		if (!output.scriptable_destructibles.empty())
		{
			std::string calls;
			for (const auto& [targetname, kind] : output.scriptable_destructibles)
			{
				for (const auto& effect : kind.effects)
				{
					effects.try_emplace(effect, effect);
					placed.insert(effect);
				}

				calls += std::format("\tiw7_destructible( \"{}\", \"{}\", \"{}\", \"{}\", {} );\n", targetname, kind.dead_model,
					kind.effects.empty() ? std::string{} : kind.effects.front(), kind.sound, kind.health);
			}

			start_thread("iw7_destructibles");
			map_script += "\n// The scriptables that break: IW7 swaps their model, plays an effect and a sound when their health runs out.\n"
				"iw7_destructibles()\n{\n" + calls + "}\n" + std::string(R"gsc(
iw7_destructible( name, model, effect, sound, health )
{
    pieces = getentarray( name, "targetname" );
    foreach ( piece in pieces )
        piece thread iw7_destructible_think( model, effect, sound, health );
}

iw7_destructible_think( model, effect, sound, health )
{
    self endon( "death" );
    self.maxhealth = 1000000;
    self.health = 1000000;
    self setcandamage( 1 );
    total = 0;
    while ( total < health )
    {
        self waittill( "damage", amount );
        self.health = 1000000;
        total = total + amount;
    }

    self setcandamage( 0 );
    if ( effect != "" && isdefined( level._effect[effect] ) )
        playfx( level._effect[effect], self.origin + ( 0, 0, 4 ) );
    if ( sound != "" )
        self playsound( sound );
    if ( model == "" )
        self delete();
    else
        self setmodel( model );
}
)gsc");
			ZONETOOL_INFO("iw7: the map script breaks %zu kinds of scriptables", output.scriptable_destructibles.size());
		}

		// The scriptables of the other kinds that take damage (export_entities: several parts, stages, reactions,
		// explosions, IW7's destructible cars): the map script runs their state machines (iw7_scriptables). Their effects
		// load by their own names.
		if (!output.scriptable_machines.empty())
		{
			const auto quote = [](const std::string& text)
			{
				std::string result = "\"";
				for (const auto c : text)
				{
					if (c == '"' || c == '\\')
					{
						result += '\\';
					}

					result += c;
				}

				return result + "\"";
			};

			const auto number = [](const json& value)
			{
				if (value.is_boolean())
				{
					return std::string(value.get<bool>() ? "1" : "0");
				}

				if (value.is_number_integer())
				{
					return std::to_string(value.get<std::int64_t>());
				}

				const auto f = value.is_number() ? value.get<double>() : 0.0;
				if (!std::isfinite(f) || std::abs(f) < 1e-6)
				{
					return std::string("0");
				}

				auto text = std::format("{:.6f}", f);
				text.erase(text.find_last_not_of('0') + 1);
				if (text.back() == '.')
				{
					text.pop_back();
				}

				return text;
			};

			std::string tables;
			std::string calls;
			std::size_t machine_index{};
			std::map<std::string, std::size_t> machine_indices;
			// The models that fly as physics props, with their masses (physicslaunchserver turns an impulse into a speed of
			// impulse / mass): the debris that the machines only throw, and the models that only the pushes of their last
			// states swap in. A model has one physics asset: one that a state also sets or attaches, that the map script sets
			// or that the map places keeps the keyframed body of script models (the debris flies its arc as a script model).
			const auto keyframed = [&](const std::string& model)
			{
				return output.entity_models.contains(model) || output.map_script_models.contains(model) ||
					std::ranges::any_of(output.scriptable_destructibles, [&](const auto& entry)
					{
						return !entry.second.dead_model.empty() && file_name(entry.second.dead_model) == model;
					});
			};

			for (const auto& [model, body] : output.debris_models)
			{
				if (body && !keyframed(model) && !output.script_models.contains(model))
				{
					output.physics_models.emplace(model, *body);
				}
				else
				{
					output.script_models.insert(model);
				}
			}

			for (const auto& [model, swaps] : output.swap_models)
			{
				const auto* asset = output.models.contains(model) ? output.models.at(model) : nullptr;
				if (asset && swaps == output.machine_models[model] && !keyframed(model) && !output.debris_models.contains(model))
				{
					if (const auto body = clutter_physics(source, *asset))
					{
						output.physics_models.emplace(model, *body);
						output.script_models.erase(model);
					}
				}
			}

			calls += "\tlevel.iw7_physics_mass = [];\n";
			for (const auto& [model, body] : output.physics_models)
			{
				calls += std::format("\tlevel.iw7_physics_mass[{}] = {};\n", quote(model), number(body.mass));
			}

			for (const auto& [targetname, machine] : output.scriptable_machines)
			{
				std::string body;
				std::size_t temporary{};
				const std::function<std::string(const json&)> event_list = [&](const json& events) -> std::string
				{
					std::string text;
					std::vector<std::string> added;
					for (const auto& event : events)
					{
						const auto t = event.value("t", std::string{});
						const auto omitted = [&](const char* reason)
						{
							ZONETOOL_WARNING("iw7: scriptable event \"%s\" omitted in \"%s\": %s", t.c_str(),
								machine.value("def", std::string{}).c_str(), reason);
						};
						const auto str = [&](const char* key) { return quote(event.value(key, std::string{})); };
						const auto num = [&](const char* key) { return number(event.contains(key) ? event.at(key) : json(0)); };
						std::string args;
						if (t == "wait")
						{
							args = std::format("{}, {}", num("min"), num("max"));
						}
						else if (t == "random")
						{
							const auto a = event_list(event.at("a"));
							const auto b = event_list(event.at("b"));
							args = std::format("{}, {}, {}", num("threshold"), a, b);
						}
						else if (t == "state")
						{
							if (event.value("part", -1) < 0 || event.value("index", -1) < 0)
							{
								omitted("invalid part or state reference");
								continue;
							}

							args = std::format("{}, {}", num("part"), num("index"));
						}
						else if (t == "model")
						{
							args = str("model");
						}
						else if (t == "addmodel")
						{
							args = std::format("{}, {}", str("tag"), str("model"));
						}
						else if (t == "fx")
						{
							const auto effect = event.value("effect", std::string{});
							if (effect.empty())
							{
								omitted("missing effect");
								continue;
							}

							effects.try_emplace(effect, effect);
							placed.insert(effect);
							const auto& tags = event.at("tags");
							args = std::format("{}, {}, {}, {}, {}", quote(effect),
								quote(tags.empty() ? std::string{} : tags.front().get<std::string>()), num("loop"), num("kill"),
								num("use_tag_angles"));
						}
						else if (t == "sound")
						{
							if (event.value("alias", std::string{}).empty())
							{
								omitted("missing sound alias");
								continue;
							}

							args = str("alias");
						}
						else if (t == "anim")
						{
							if (event.value("anim", std::string{}).empty())
							{
								omitted("missing animation");
								continue;
							}

							args = str("anim");
						}
						else if (t == "hide")
						{
							args = std::format("{}, {}", str("tag"), num("hide"));
						}
						else if (t == "explosion")
						{
							args = std::format("{}, {}, {}, {}", str("tag"), num("radius"), num("inner"), num("outer"));
						}
						else if (t == "shake")
						{
							args = std::format("{}, {}, {}", num("scale"), num("duration"), num("radius"));
						}
						else if (t == "force")
						{
							const auto& pos = event.at("pos");
							const auto& vector = event.at("vector");
							args = std::format("{}, ( {}, {}, {} ), ( {}, {}, {} )", num("fall"), number(pos.at(0)), number(pos.at(1)),
								number(pos.at(2)), number(vector.at(0)), number(vector.at(1)), number(vector.at(2)));
						}
						else if (t == "debris")
						{
							const auto& velocity = event.at("velocity");
							args = std::format("{}, {}, ( {}, {}, {} ), {}", str("model"), str("tag"), number(velocity.at(0)),
								number(velocity.at(1)), number(velocity.at(2)), num("world"));
						}
						else if (t == "damage")
						{
							if (event.value("part", -1) < 0)
							{
								omitted("invalid part reference");
								continue;
							}

							args = std::format("{}, {}, {}, {}", num("part"), num("amount"), num("dot"), num("interval"));
						}
						else if (t == "script")
						{
							if (event.value("notify", std::string{}).empty())
							{
								omitted("missing notification");
								continue;
							}

							args = std::format("{}, {}", str("notify"), num("param"));
						}
						else
						{
							omitted("no converted handler");
							continue;
						}

						added.push_back(std::format("iw7_mev( \"{}\", {} )", t, args));
					}

					const auto name = std::format("e{}", temporary++);
					text = std::format("\t{} = [];\n", name);
					for (const auto& event : added)
					{
						text += std::format("\t{0}[{0}.size] = {1};\n", name, event);
					}

					body += text;
					return name;
				};

				body += "\tparts = [];\n";
				for (const auto& part : machine.at("parts"))
				{
					body += "\tstates = [];\n";
					for (const auto& state : part.at("states"))
					{
						const auto events = event_list(state.at("events"));
						body += std::format("\tstates[states.size] = iw7_mstate( {}, {}, {}, {}, {}, {} );\n", quote(state.value("name", std::string{})),
							quote(state.value("kind", std::string{})), number(state.at("health")), number(state.at("mindamage")),
							number(state.at("to_parent")), events);
					}

					body += std::format("\tparts[parts.size] = iw7_mpart( {}, {}, {}, states );\n", quote(part.value("name", std::string{})),
						quote(part.value("tag", std::string{})), number(part.at("parent")));
				}

				tables += std::format("\n// {}\niw7_machine_{}()\n{{\n{}\treturn parts;\n}}\n", machine.value("def", std::string{}),
					machine_index, body);
				calls += std::format("\tm{0} = iw7_machine_{0}();\n\tiw7_scriptable( \"{1}\", m{0} );\n", machine_index, targetname);
				machine_indices[targetname] = machine_index;
				machine_index++;
			}

			for (const auto& target : output.scriptable_targets)
			{
				calls += std::format("\tiw7_scriptable_at( {}, ( {}, {}, {} ), m{} );\n", quote(target.targetname), number(target.origin[0]),
					number(target.origin[1]), number(target.origin[2]), machine_indices.at(target.machine));
			}

			start_thread("iw7_scriptables");
			map_script += "\n// The scriptables of other kinds: IW7's state machines of their parts.\niw7_scriptables()\n{\n" + calls + "}\n" +
				tables + std::string(scriptable_machine_runtime);
			ZONETOOL_INFO("iw7: the map script runs the state machines of %zu kinds of scriptables (%zu instances by their own targetname)",
				output.scriptable_machines.size(), output.scriptable_targets.size());
		}
		else if (map_script.find("iw7_setscriptablepartstate( part, state, instant )") != std::string::npos)
		{
			// The setscriptablepartstate of the map script asks the machines first.
			map_script += "\niw7_scriptable_set( part, state )\n{\n\treturn 0;\n}\n";
		}

		if (output.out_of_bounds)
		{
			start_thread("iw7_out_of_bounds");
			map_script += std::string(out_of_bounds_runtime);
			ZONETOOL_INFO("iw7: the map script watches the map's out-of-bounds volumes (OutOfBounds triggers)");
		}

		// The strings that can name sound aliases: the aliases of the loop sounds and the map script's strings.
		for (const auto& entry : placements)
		{
			for (const auto& [key, text] : entry.values)
			{
				if (key == "soundalias" && text.size() > 2 && text.front() == '"' && text.back() == '"')
				{
					output.sound_names.insert(text.substr(1, text.size() - 2));
				}
			}
		}

		static const std::regex literal(R"re("([\w/]+)")re");
		for (auto it = std::sregex_iterator(map_script.begin(), map_script.end(), literal); it != std::sregex_iterator(); ++it)
		{
			output.sound_names.insert((*it)[1].str());
		}

		export_effect_graphs(source, output, {placed.begin(), placed.end()});

		const auto folder = output.extracted / "maps";
		std::filesystem::create_directories(folder / "mp");
		std::filesystem::create_directories(folder / "createfx");
		{
			std::ofstream file(folder / "mp" / (source.name() + "_fx.gsc"), std::ios::binary | std::ios::trunc);
			file << "// The effects of the map (IW7).\nmain()\n{\n";
			for (const auto& [id, path] : effects)
			{
				if (placed.contains(path))
				{
					file << std::format("\tlevel._effect[ \"{}\" ] = loadfx( \"{}\" );\n", id, path);
				}
			}

			file << "}\n";
		}

		{
			std::ofstream file(folder / "createfx" / (source.name() + "_fx.gsc"), std::ios::binary | std::ios::trunc);
			file << "// The effect placements of the map (IW7 createfx).\nmain()\n{\n";
			for (const auto& entry : placements)
			{
				file << (entry.fxid.empty() ? std::format("\tent = maps\\mp\\_utility::{}();\n", *iw3_create_call(entry.kind))
					: std::format("\tent = maps\\mp\\_utility::{}( \"{}\" );\n", *iw3_create_call(entry.kind), entry.fxid));
				file << std::format("\tent.v[ \"origin\" ] = ( {} );\n", entry.origin);
				file << std::format("\tent.v[ \"angles\" ] = ( {} );\n", entry.angles);
				for (const auto& [key, text] : entry.values)
				{
					file << std::format("\tent.v[ \"{}\" ] = {};\n", key, text);
				}

				file << "\n";
			}

			file << "}\n";
		}

		{
			std::ofstream file(folder / "mp" / (source.name() + ".gsc"), std::ios::binary | std::ios::trunc);
			if (!map_script.empty())
			{
				file << map_script;
			}
			else
			{
				file << std::format("// The map script (IW7 maps run their effects and createfx scripts first).\nmain()\n{{\n"
					"\tmaps\\mp\\{0}_fx::main();\n\tmaps\\createfx\\{0}_fx::main();\n\tmaps\\mp\\_load::main();\n}}\n", source.name());
			}
		}

		ZONETOOL_INFO("iw7: %zu effect placements of %zu effects (%zu effect ids, %zu createfx calls of other kinds)", placements.size(),
			placed.size(), effects.size(), unknown_calls);
	}

	void export_effect_graphs(const source_map& source, extraction& output, const std::vector<std::string>& effects)
	{
		effect_converter converter(source, output);
		std::size_t converted{};
		for (const auto& name : effects)
		{
			converted += converter.convert(name);
		}

		std::string dropped;
		for (const auto& [what, count] : converter.dropped())
		{
			dropped += std::format("{}{} x{}", dropped.empty() ? "" : ", ", what, count);
		}

		ZONETOOL_INFO("iw7: converted %zu of %zu effects (%zu with their children), %zu not in the loaded zones; left out: %s", converted,
			effects.size(), converter.converted(), converter.missing().size(), dropped.empty() ? "nothing" : dropped.data());
	}
}
