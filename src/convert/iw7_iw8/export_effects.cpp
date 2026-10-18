#include <std_include.hpp>
#include "export.hpp"

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

			void drop(const std::string& what)
			{
				this->dropped_[what]++;
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
				for (auto e = 0; e < system.numEmitters; e++)
				{
					emitters.push_back(this->convert_emitter(system.emitterDefs[e]));
				}

				return json{{"schema", 1}, {"name", asset_name(system.name)}, {"system", hex(root)}, {"emitters", std::move(emitters)}};
			}

			json convert_emitter(const iw7::ParticleEmitterDef& emitter)
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
				for (auto s = 0; s < emitter.numStates; s++)
				{
					states.push_back(this->convert_state(emitter.stateDefs[s], types));
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
				return {{"emitter", hex(record)}, {"fade_curve", curve_points(emitter.fadeCurveDef)}, {"states", std::move(states)}};
			}

			json convert_state(const iw7::ParticleStateDef& state, std::set<std::uint16_t>& emitter_types)
			{
				focus::ParticleStateDef record{};
				const auto element = replay_element(state.elementType);
				record.elementType = element.value_or(0);
				const auto source_flags = replay_state_flags(state.flags);

				auto groups = json::array();
				std::set<std::uint16_t> types;
				for (auto g = 0; g < 3; g++)
				{
					auto modules = json::array();
					const auto& group = state.moduleGroupDefs[g];
					for (auto m = 0; !group.disabled && m < group.numModules; m++)
					{
						auto module = this->convert_module(group.moduleDefs[m], static_cast<std::uint16_t>(modules.size()));
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

				return {{"state", hex(record)}, {"groups", std::move(groups)}};
			}

			// Converts a module of the IW7 layout to Replay's, or leaves it out (counted by type).
			std::optional<module_record> convert_module(const iw7::ParticleModuleDef& module, const std::uint16_t index)
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
					return result;
				case iw7::PARTICLE_MODULE_INIT_CAMERA_OFFSET:
				case iw7::PARTICLE_MODULE_INIT_PLAYER_FACING:
				case iw7::PARTICLE_MODULE_INIT_MIRROR_TEXTURE:
				case iw7::PARTICLE_MODULE_INIT_RELATIVE_VELOCITY:
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
					copy(sizeof(iw7::ParticleModuleInitFlare));
					for (auto c = 0; c < 4; c++)
					{
						result.curve(offsetof(iw7::ParticleModuleInitFlare, m_curves) + c * 16, data.initFlare.m_curves[c]);
					}

					return result;
				case iw7::PARTICLE_MODULE_INIT_SPAWN:
					copy(sizeof(iw7::ParticleModuleInitSpawn));
					result.curve(offsetof(iw7::ParticleModuleInitSpawn, m_curves), data.initSpawn.m_curves[0]);
					return result;
				case iw7::PARTICLE_MODULE_COLOR_GRAPH:
					copy(sizeof(iw7::ParticleModuleColorGraph));
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
					const auto first = reinterpret_cast<const std::uint8_t*>(curves) - reinterpret_cast<const std::uint8_t*>(&data);
					for (auto c = 0; c < 6; c++)
					{
						result.curve(first + c * 16, curves[c]);
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
						const auto* material = list.assetList[a].material;
						if (material)
						{
							const auto name = this->output_.add_material(this->source_, const_cast<iw7::Material*>(material));
							this->output_.effect_materials.insert(name);
							entries.push_back(linked_asset("material", name));
						}
					}

					if (entries.empty())
					{
						this->drop("material module without materials");
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
						if (const auto* model = list.assetList[a].model)
						{
							entries.push_back(linked_asset("xmodel", this->output_.add_model(this->source_, const_cast<iw7::XModel*>(model))));
						}
					}

					result.list(offsetof(iw7::ParticleModuleInitModel, m_linkedAssetList), std::move(entries));
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
						this->drop("runner without child effects");
						return std::nullopt;
					}

					result.list(offsetof(focus::ParticleModuleInitRunner, linkedAssets), std::move(entries));
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
				case iw7::PARTICLE_MODULE_INIT_OCCLUSION_QUERY:
				{
					// Replay adds a depth bias before the fade times and a ray cast flag after the world size.
					const auto& source = data.initOcclusionQuery;
					base();
					result.set(8, 0.0f);
					result.set(12, source.m_fadeInTime);
					result.set(16, source.m_fadeOutTime);
					result.set(20, source.m_scale);
					result.set(28, source.m_worldSize);
					return result;
				}
				case iw7::PARTICLE_MODULE_INIT_SPAWN_SHAPE_BOX:
				case iw7::PARTICLE_MODULE_INIT_SPAWN_SHAPE_CYLINDER:
				case iw7::PARTICLE_MODULE_INIT_SPAWN_SHAPE_ELLIPSOID:
				case iw7::PARTICLE_MODULE_INIT_SPAWN_SHAPE_SPHERE:
					return this->convert_spawn_shape(module, result);
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
			const auto input = folder / "gscbin";
			for (const auto& [name, header] : source.assets(iw7::ASSET_TYPE_SCRIPTFILE))
			{
				// The format of zonetool's script dumps: the name, the three lengths, the compressed source and the bytecode.
				const auto* script = static_cast<const iw7::ScriptFile*>(header);
				const auto path = input / (name + ".gscbin");
				std::filesystem::create_directories(path.parent_path());
				std::ofstream file(path, std::ios::binary | std::ios::trunc);
				file.write(script->name, std::strlen(script->name) + 1);
				file.write(reinterpret_cast<const char*>(&script->compressedLen), 4);
				file.write(reinterpret_cast<const char*>(&script->len), 4);
				file.write(reinterpret_cast<const char*>(&script->bytecodeLen), 4);
				file.write(script->buffer, script->compressedLen);
				file.write(script->bytecode, script->bytecodeLen);
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

			WaitForSingleObject(process.hProcess, 300000);
			CloseHandle(process.hThread);
			CloseHandle(process.hProcess);

			std::map<std::string, std::string> result;
			const auto decompiled = folder / "decompiled" / "iw7";
			for (const auto& [name, header] : source.assets(iw7::ASSET_TYPE_SCRIPTFILE))
			{
				std::ifstream file(decompiled / (name + ".gsc"), std::ios::binary);
				if (file)
				{
					result.emplace(name, std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()));
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

iw7_unsupported( a, b, c, d, e, f )
{
}
)gsc";

		// The map's own script (scripts\mp\maps\<map>\<map>) as the IW3 map script that the pipeline translates: the
		// functions that its main runs, with IW7's utilities replaced by the IW3 ones or their IW3 versions. Calls
		// of other IW7 scripts drop out in the pipeline. The converted models have one bone (tag_origin), which the
		// effects on their tags take; sounds, animations, cinematics and scriptable states do not convert. Adds the
		// effects that it loads.
		std::string translate_map_script(const std::string& map, const std::string& text, std::set<std::string>& effects)
		{
			const auto functions = script_functions(text);
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
			for (const auto& function : functions)
			{
				if (!reached.contains(function.name))
				{
					continue;
				}

				auto body = function.body;
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

				for (const auto& [from, to] : std::initializer_list<std::pair<std::string_view, std::string_view>>{
					{"scripts\\engine\\utility::getstructarray(", "common_scripts\\utility::getstructarray("},
					{"scripts\\engine\\utility::getstruct(", "iw7_getstruct("},
					{"scripts\\engine\\utility::waittill_either(", "iw7_waittill_either("},
					{"scripts\\engine\\utility::waittill_any_return(", "iw7_waittill_any_return("},
					{"scripts\\engine\\utility::waitframe(", "waitframe("},
					{"scripts\\engine\\utility::array_thread(", "common_scripts\\utility::array_thread("},
				})
				{
					for (auto at = body.find(from); at != std::string::npos; at = body.find(from, at + to.size()))
					{
						body.replace(at, from.size(), to);
					}
				}

				for (const auto name : {"playfxontag", "stopfxontag"})
				{
					body = rewrite_calls(body, name, [&](std::vector<std::string> arguments)
					{
						if (arguments.size() == 3 && arguments[2].starts_with('"'))
						{
							arguments[2] = "\"tag_origin\"";
						}

						return std::format("{}( {} )", name, join(arguments));
					});
				}

				for (const auto name : {"playsoundonmovingent", "playcinematicforalllooping", "scriptmodelplayanim",
					"precachempanim", "setscriptablepartstate"})
				{
					body = rewrite_calls(body, name, [](const std::vector<std::string>& arguments)
					{
						return "iw7_unsupported( " + join(arguments) + " )";
					});
				}

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
			return std::regex_replace(result, token_name, "iw7_$1") + std::string(map_script_helpers);
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
			std::set<std::string> loaded;
			map_script = translate_map_script(source.name(), found->second, loaded);
			ZONETOOL_INFO("iw7: translated the map script (%zu bytes) with %zu effects of its own", map_script.size(), loaded.size());
			placed.insert(loaded.begin(), loaded.end());
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
				file << std::format("\tent = maps\\mp\\_utility::{}( \"{}\" );\n", *iw3_create_call(entry.kind), entry.fxid);
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
