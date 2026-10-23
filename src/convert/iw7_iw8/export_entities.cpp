#include <std_include.hpp>
#include "export.hpp"

#include <bit>

#include <iw8/dump/maps_convert.h>
#include <iw8/havok/objects.hpp>
#include <xsk/gsc/engine/iw7.hpp>

namespace xsk::gsc::iw7
{
	extern std::array<std::pair<u32, char const*>, token_count> const token_list;
}

namespace zonetool::convert::iw7_iw8
{
	namespace
	{
		// IW7 entity strings name their keys by script string token ("157" is classname); the IW3 pipeline reads names.
		std::string with_key_names(const std::string& entities)
		{
			static const auto names = []
			{
				std::unordered_map<std::uint32_t, std::string_view> result;
				for (const auto& [id, name] : xsk::gsc::iw7::token_list)
				{
					result.try_emplace(id, name);
				}

				return result;
			}();

			std::string result;
			result.reserve(entities.size() * 2);
			std::size_t start = 0;
			while (start < entities.size())
			{
				const auto end = std::min(entities.find('\n', start), entities.size());
				const std::string_view line(entities.data() + start, end - start);
				const auto digits = line.find_first_not_of("0123456789");
				if (digits && digits != std::string_view::npos && line.substr(digits).starts_with(" \""))
				{
					const auto id = static_cast<std::uint32_t>(std::stoul(std::string(line.substr(0, digits))));
					// A key without a token is 0 followed by its name ("0 \"uselmapcompression\" \"0\"": Mansion, Rally).
					if (!id)
					{
						result += line.substr(digits + 1);
						result += '\n';
						start = end + 1;
						continue;
					}

					const auto found = names.find(id);
					result += found != names.end() && !found->second.empty() ? std::format("\"{}\"", found->second)
						: std::format("\"_id_{:X}\"", id);
					result += line.substr(digits);
				}
				else
				{
					result += line;
				}

				result += '\n';
				start = end + 1;
			}

			return result;
		}

		json vector_json(const float* values, const std::size_t count)
		{
			auto result = json::array();
			for (std::size_t i = 0; i < count; i++)
			{
				result.push_back(values[i]);
			}

			return result;
		}

		// The surface types of IW7 materials by their bit in surfaceTypeBits (up to metal_thin; the robot types and the user
		// terrains do not make clutter).
		constexpr std::array<std::string_view, 43> surface_names{"bark", "brick", "carpet_solid", "cloth", "concrete_dry", "dirt",
			"flesh", "foliage_dry", "glass_pane", "grass_short", "gravel", "ice_solid", "metal_thick", "metal_grate", "mud", "paper",
			"plaster", "rock", "sand", "snow", "water", "wood_solid", "asphalt_dry", "ceramic", "plastic", "rubber", "fruit",
			"metal_painted", "riotshield", "slush", "asphalt_wet", "carpet_wood", "carpet_metal", "concrete_wet", "cushion", "default",
			"foliage_wet", "glass_broken", "glass_solid", "glass_vehicle", "grass_tall", "ice_thin", "metal_thin"};

		// IW7 light types with their shadow-map variants, as the IW3 types (1 directional, 2 spot, 3 omni).
		int light_type(const iw7::GfxLightType type)
		{
			switch (type)
			{
			case iw7::GFX_LIGHT_TYPE_DIR:
			case iw7::GFX_LIGHT_TYPE_DIR_SHADOWMAP:
				return 1;
			case iw7::GFX_LIGHT_TYPE_SPOT:
			case iw7::GFX_LIGHT_TYPE_SPOT_SHADOWMAP:
			case iw7::GFX_LIGHT_TYPE_SPOT_SHADOWMAP_CUCOLORIS:
				return 2;
			case iw7::GFX_LIGHT_TYPE_OMNI:
			case iw7::GFX_LIGHT_TYPE_OMNI_SHADOWMAP:
				return 3;
			default:
				return 0;
			}
		}

		// Keeps the entities of the classes that the IW3 entity conversion takes: the others are IW7 features without a
		// Replay counterpart (Blitz spawns, 3D path nodes, client triggers).
		std::string with_converting_classes(const std::string& entities)
		{
			std::string result;
			result.reserve(entities.size());
			std::map<std::string, std::size_t> dropped;
			std::string block;
			std::string classname;
			std::size_t start = 0;
			while (start < entities.size())
			{
				const auto end = std::min(entities.find('\n', start), entities.size());
				const std::string_view line(entities.data() + start, end - start);
				start = end + 1;

				block.append(line);
				block += '\n';
				constexpr std::string_view key = "\"classname\" \"";
				if (line.starts_with(key) && line.size() > key.size())
				{
					classname = line.substr(key.size(), line.size() - key.size() - 1);
				}

				if (line != "}")
				{
					continue;
				}

				if (::convert::iw3EntityClassConverts(classname))
				{
					result += block;
				}
				else
				{
					dropped[classname]++;
				}

				block.clear();
				classname.clear();
			}

			result += block;
			for (const auto& [name, count] : dropped)
			{
				ZONETOOL_INFO("iw7: dropped %zu entities of class \"%s\", which Replay maps do not have", count, name.data());
			}

			return result;
		}

		// IW7 trigger entities name their volume "?N", trigger model N of the map entities (client triggers use
		// "?N" too, for their own list). The extraction appends the trigger models to the brush models.
		std::string with_trigger_models(const source_map& source, const std::string& entities)
		{
			const auto& triggers = source.map_ents()->trigger;
			std::string result;
			result.reserve(entities.size());
			std::size_t converted{};
			std::size_t start = 0;
			while (start < entities.size())
			{
				const auto end = std::min(entities.find('\n', start), entities.size());
				const std::string_view line(entities.data() + start, end - start);
				start = end + 1;

				constexpr std::string_view key = "\"model\" \"?";
				unsigned int index{};
				const auto number = line.substr(std::min(key.size(), line.size()));
				const auto parsed = std::from_chars(number.data(), number.data() + number.size(), index);
				if (line.starts_with(key) && parsed.ec == std::errc{} && std::string_view(parsed.ptr, number.data() + number.size()) == "\"" &&
					index < triggers.count)
				{
					result += std::format("\"model\" \"*{}\"\n", trigger_brush_model(source, index));
					converted++;
					continue;
				}

				result += line;
				result += '\n';
			}

			ZONETOOL_INFO("iw7: %zu entities use trigger models", converted);
			return result;
		}

		std::string script_string(const iw7::scr_string_t id)
		{
			const auto string = SL_ConvertToString(static_cast<std::uint32_t>(id));
			return string ? string : "";
		}

		std::string vector_text(const float* values)
		{
			return std::format("{} {} {}", values[0], values[1], values[2]);
		}

		using axes = std::array<std::array<double, 3>, 3>;

		// AnglesToAxis: the forward, left and up axes of pitch, yaw and roll (degrees).
		axes angles_to_axes(const float* angles)
		{
			constexpr auto radians = std::numbers::pi / 180.0;
			const auto sp = std::sin(angles[0] * radians), cp = std::cos(angles[0] * radians);
			const auto sy = std::sin(angles[1] * radians), cy = std::cos(angles[1] * radians);
			const auto sr = std::sin(angles[2] * radians), cr = std::cos(angles[2] * radians);
			return {{{cp * cy, cp * sy, -sp}, {sr * sp * cy - cr * sy, sr * sp * sy + cr * cy, sr * cp},
				{cr * sp * cy + sr * sy, cr * sp * sy - sr * cy, cr * cp}}};
		}

		// The angles of forward, left and up axes (the inverse of angles_to_axes).
		std::array<float, 3> axes_to_angles(const axes& axis)
		{
			constexpr auto degrees = 180.0 / std::numbers::pi;
			const auto& forward = axis[0];
			const auto yaw = std::atan2(forward[1], forward[0]);
			const auto pitch = std::atan2(-forward[2], std::sqrt(forward[0] * forward[0] + forward[1] * forward[1]));
			// The left axis turns from the left of the unrolled axes toward their up.
			const std::array unrolled_left{-std::sin(yaw), std::cos(yaw), 0.0};
			const std::array unrolled_up{std::sin(pitch) * std::cos(yaw), std::sin(pitch) * std::sin(yaw), std::cos(pitch)};
			const auto dot = [](const std::array<double, 3>& a, const std::array<double, 3>& b)
			{
				return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
			};

			const auto roll = std::atan2(dot(axis[1], unrolled_up), dot(axis[1], unrolled_left));
			return {static_cast<float>(pitch * degrees), static_cast<float>(yaw * degrees), static_cast<float>(roll * degrees)};
		}

		// The world placement of a tag of a model at an instance's origin and angles, in the model's bind pose (DObjAnimMat
		// base poses are in model space). An empty tag or tag_origin is the model's origin.
		std::optional<extraction::scriptable_effect> tag_placement(const iw7::XModel* model, const std::string& tag, const float* origin,
			const float* angles)
		{
			const auto instance = angles_to_axes(angles);
			std::array<double, 3> position{};
			axes rotation{{{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}}};
			if (!tag.empty() && tag != "tag_origin")
			{
				const iw7::DObjAnimMat* pose{};
				for (auto bone = 0u; model && model->boneNames && model->baseMat && bone < model->numBones; bone++)
				{
					if (script_string(model->boneNames[bone]) == tag)
					{
						pose = &model->baseMat[bone];
						break;
					}
				}

				if (!pose)
				{
					return {};
				}

				const double length = std::sqrt(pose->quat[0] * pose->quat[0] + pose->quat[1] * pose->quat[1] +
					pose->quat[2] * pose->quat[2] + pose->quat[3] * pose->quat[3]);
				if (!(length > 0.0))
				{
					return {};
				}

				const auto x = pose->quat[0] / length, y = pose->quat[1] / length, z = pose->quat[2] / length, w = pose->quat[3] / length;
				rotation = {{{1 - 2 * (y * y + z * z), 2 * (x * y + w * z), 2 * (x * z - w * y)},
					{2 * (x * y - w * z), 1 - 2 * (x * x + z * z), 2 * (y * z + w * x)},
					{2 * (x * z + w * y), 2 * (y * z - w * x), 1 - 2 * (x * x + y * y)}}};
				position = {pose->trans[0], pose->trans[1], pose->trans[2]};
			}

			extraction::scriptable_effect result;
			axes world{};
			for (auto c = 0u; c < 3; c++)
			{
				result.origin[c] = static_cast<float>(origin[c] + position[0] * instance[0][c] + position[1] * instance[1][c] +
					position[2] * instance[2][c]);
				for (auto k = 0u; k < 3; k++)
				{
					world[k][c] = rotation[k][0] * instance[0][c] + rotation[k][1] * instance[1][c] + rotation[k][2] * instance[2][c];
				}
			}

			result.angles = axes_to_angles(world);
			return result;
		}

		// A scriptable that breaks: one part whose first state takes damage (a health state) and whose second and last state
		// swaps the model, plays effects or a sound. Other kinds (multi-stage, several parts, debris) run as state machines.
		std::optional<extraction::scriptable_destructible> destructible_of(const source_map& source, const iw7::ScriptableDef& def,
			extraction& output)
		{
			if (!def.parts || def.numParts != 1 || !def.parts[0].states || def.parts[0].numStates != 2)
			{
				return std::nullopt;
			}

			const auto& healthy = def.parts[0].states[0];
			const auto& broken = def.parts[0].states[1];
			if (healthy.type != iw7::Scriptable_StateType_Health || healthy.data.health.health <= 0 ||
				healthy.data.health.health > 100000 || broken.type != iw7::Scriptable_StateType_Simple)
			{
				return std::nullopt;
			}

			extraction::scriptable_destructible result;
			result.health = healthy.data.health.health;
			auto changes = false;
			for (auto e = 0u; broken.base.events && e < broken.base.numEvents; e++)
			{
				const auto& event = broken.base.events[e];
				// A break that does more (throws debris, explodes, shakes, damages or changes parts, waits, hides bones)
				// runs as a state machine (scriptable_machine).
				switch (event.type)
				{
				case iw7::Scriptable_EventType_SpawnDynent:
				case iw7::Scriptable_EventType_Explosion:
				case iw7::Scriptable_EventType_Screenshake:
				case iw7::Scriptable_EventType_PartDamage:
				case iw7::Scriptable_EventType_StateChange:
				case iw7::Scriptable_EventType_Wait:
				case iw7::Scriptable_EventType_Random:
				case iw7::Scriptable_EventType_HideShowBone:
				case iw7::Scriptable_EventType_AddModel:
					return std::nullopt;
				default:
					break;
				}

				if (event.type == iw7::Scriptable_EventType_Model)
				{
					changes = true;
					const auto model = source.resolve(iw7::ASSET_TYPE_XMODEL, event.data.model.model);
					if (model && result.dead_model.empty())
					{
						result.dead_model = asset_name(iw7::get_asset_name(iw7::ASSET_TYPE_XMODEL, model));
						output.models.try_emplace(file_name(result.dead_model), model);
					}
				}
				else if (event.type == iw7::Scriptable_EventType_PFX)
				{
					const auto& fx = event.data.particleFX;
					const auto vfx = fx.effectDef.type == iw7::FX_COMBINED_VFX ? source.resolve(iw7::ASSET_TYPE_VFX, fx.effectDef.u.vfx) : nullptr;
					if (vfx)
					{
						changes = true;
						result.effects.emplace_back(asset_name(iw7::get_asset_name(iw7::ASSET_TYPE_VFX, vfx)));
					}
				}
				else if (event.type == iw7::Scriptable_EventType_Sound && event.data.sound.soundAlias && result.sound.empty())
				{
					result.sound = asset_name(event.data.sound.soundAlias);
				}
			}

			if (!changes)
			{
				return std::nullopt;
			}

			std::string kind(def.name ? asset_name(def.name) : std::string_view{"scriptable"});
			std::ranges::transform(kind, kind.begin(), [](const unsigned char c)
			{
				return std::isalnum(c) ? static_cast<char>(std::tolower(c)) : '_';
			});
			result.targetname = "iw7_destructible_" + kind;
			return result;
		}

		// The states of a scriptable's parts that play an animation (Frontier: the ops table's solar system idle, 1799
		// frames on 35 bones, from its first state; the floor panels' closed pose from their first state and their raise,
		// a scripted state that the map script sets).
		std::vector<extraction::scriptable_state> animation_states(const source_map& source, const iw7::ScriptableDef& def)
		{
			std::vector<extraction::scriptable_state> states;
			for (auto p = 0u; def.parts && p < def.numParts; p++)
			{
				const auto& part = def.parts[p];
				for (auto s = 0u; part.states && s < part.numStates; s++)
				{
					const auto& state = part.states[s];
					for (auto e = 0u; state.base.events && e < state.base.numEvents; e++)
					{
						const auto& event = state.base.events[e];
						const auto anim = event.type == iw7::Scriptable_EventType_Animation && event.data.animation.animation ?
							source.resolve(iw7::ASSET_TYPE_XANIMPARTS, event.data.animation.animation) : nullptr;
						if (anim && anim->name)
						{
							states.push_back({part.name ? part.name : "", state.base.name ? state.base.name : "", anim, s == 0});
							break;
						}
					}
				}
			}

			return states;
		}

		// Whether a scriptable kind takes damage: a part (or a child part) has a health state.
		bool any_part(const iw7::ScriptableDef& def, const std::function<bool(const iw7::ScriptablePartDef&)>& predicate)
		{
			const std::function<bool(const iw7::ScriptablePartDef*, unsigned)> any = [&](const iw7::ScriptablePartDef* parts,
				const unsigned count)
			{
				for (auto p = 0u; parts && p < count; p++)
				{
					if (predicate(parts[p]) || any(parts[p].childParts, parts[p].numChildParts))
					{
						return true;
					}
				}

				return false;
			};

			return any(def.parts, def.numParts);
		}

		bool takes_damage(const iw7::ScriptableDef& def)
		{
			return any_part(def, [](const iw7::ScriptablePartDef& part)
			{
				for (auto s = 0u; part.states && s < part.numStates; s++)
				{
					if (part.states[s].type == iw7::Scriptable_StateType_Health)
					{
						return true;
					}
				}

				return false;
			});
		}

		// A kind whose parts have states to change to (by script, or by the events of their states).
		bool changes_state(const iw7::ScriptableDef& def)
		{
			return any_part(def, [](const iw7::ScriptablePartDef& part) { return part.states && part.numStates > 1; });
		}

		// A scriptable kind's state machine as the map script runs it (iw7_scriptables): its parts in tree order (name, tag,
		// parent), each part's states (name, kind, health), and each state's events in order. Models, effects and sound
		// aliases convert with the map; event kinds that do not convert keep their IW7 type (e<n>) for the log.
		json scriptable_machine(const source_map& source, const iw7::ScriptableDef& def, extraction& output, const bool convert)
		{
			std::vector<const iw7::ScriptablePartDef*> parts;
			std::vector<int> parents;
			const std::function<void(const iw7::ScriptablePartDef*, unsigned, int)> flatten =
				[&](const iw7::ScriptablePartDef* list, const unsigned count, const int parent)
			{
				for (auto p = 0u; list && p < count; p++)
				{
					const auto index = static_cast<int>(parts.size());
					parts.push_back(&list[p]);
					parents.push_back(parent);
					flatten(list[p].childParts, list[p].numChildParts, index);
				}
			};
			flatten(def.parts, def.numParts, -1);

			// A part reference names a part relative to the event's (verified on IW7 data: Fallen's coffee sack, Riot's cars,
			// Mansion's body scanner): 0 its parent, 1 itself, 2 its child of the index in the reference, 3 the part of the
			// index path from the top-level parts. A state change's target state is the index that its part field holds.
			const auto reference_part = [&](const iw7::ScriptablePartReference& reference, const int self) -> int
			{
				switch (reference.type)
				{
				case 0:
					return self >= 0 ? parents[self] : -1;
				case 1:
					return self;
				case 2:
				{
					std::uint32_t child{};
					std::memcpy(&child, reference.u.__2.__pad0, sizeof(child));
					for (std::size_t p = 0, n = 0; p < parts.size(); p++)
					{
						if (parents[p] == self && n++ == child)
						{
							return static_cast<int>(p);
						}
					}

					return -1;
				}
				case 3:
				{
					const auto& path = reference.u.__3;
					if (!path.val || !path.count || path.count > 8)
					{
						return -1;
					}

					int part = -1;
					for (auto step = 0u; step < path.count; step++)
					{
						int found = -1;
						for (std::size_t p = 0, n = 0; p < parts.size() && found < 0; p++)
						{
							if (parents[p] == part && n++ == path.val[step])
							{
								found = static_cast<int>(p);
							}
						}

						if (found < 0)
						{
							return -1;
						}

						part = found;
					}

					return part;
				}
				default:
					return -1;
				}
			};

			// A model that a state sets or attaches is a script model; one that it throws is debris.
			const auto model_name = [&](iw7::XModel* header, const bool debris = false) -> std::string
			{
				const auto* model = header ? source.resolve(iw7::ASSET_TYPE_XMODEL, header) : nullptr;
				if (!model)
				{
					return {};
				}

				const auto name = file_name(asset_name(iw7::get_asset_name(iw7::ASSET_TYPE_XMODEL, model)));
				if (convert)
				{
					output.models.try_emplace(name, model);
					if (!debris)
					{
						output.script_models.insert(name);
						output.machine_models[name]++;
					}
					else if (!output.debris_models.contains(name))
					{
						output.debris_models.emplace(name, clutter_physics(source, *model));
					}
				}

				return name;
			};

			const std::function<json(const iw7::ScriptableEventDef*, unsigned, int)> events = [&](const iw7::ScriptableEventDef* list,
				const unsigned count, const int self)
			{
				auto result = json::array();
				for (auto e = 0u; list && e < count; e++)
				{
					const auto& event = list[e];
					const auto& data = event.data;
					switch (event.type)
					{
					case iw7::Scriptable_EventType_StateChange:
					{
						const auto part = reference_part(data.stateChange.partReference, self);
						const auto state = reinterpret_cast<std::uintptr_t>(data.stateChange.part);
						const auto states = part >= 0 && parts[part]->states ? parts[part]->numStates : 0u;
						result.push_back({{"t", "state"}, {"part", part}, {"index", state < states ? static_cast<int>(state) : -1}});
						break;
					}
					case iw7::Scriptable_EventType_Wait:
						result.push_back({{"t", "wait"}, {"min", data.wait.delayMin}, {"max", data.wait.delayMax}});
						break;
					case iw7::Scriptable_EventType_Random:
						result.push_back({{"t", "random"}, {"threshold", data.random.threshold},
							{"a", events(data.random.eventsA, data.random.eventACount, self)},
							{"b", events(data.random.eventsB, data.random.eventBCount, self)}});
						break;
					case iw7::Scriptable_EventType_Model:
						result.push_back({{"t", "model"}, {"model", model_name(data.model.model)}, {"physics", data.model.activatePhysics},
							{"dynamic", data.model.dynamicSimulation}, {"never_moves", data.model.neverMoves}});
						break;
					case iw7::Scriptable_EventType_PFX:
					{
						const auto& fx = data.particleFX;
						const auto vfx = fx.effectDef.type == iw7::FX_COMBINED_VFX ? source.resolve(iw7::ASSET_TYPE_VFX, fx.effectDef.u.vfx) :
							nullptr;
						auto tags = json::array();
						for (auto t = 0u; fx.scrTagNames && t < fx.scrTagCount; t++)
						{
							tags.push_back(script_string(fx.scrTagNames[t]));
						}

						result.push_back({{"t", "fx"}, {"effect", vfx ? std::string(asset_name(iw7::get_asset_name(iw7::ASSET_TYPE_VFX, vfx))) :
							std::string{}}, {"tags", tags}, {"loop", fx.loopRate}, {"kill", fx.killOnExit}});
						break;
					}
					case iw7::Scriptable_EventType_Sound:
						result.push_back({{"t", "sound"}, {"alias", data.sound.soundAlias ? std::string(asset_name(data.sound.soundAlias)) :
							std::string{}}, {"tag", data.sound.tagName ? data.sound.tagName : ""}, {"stateful", data.sound.stateful}});
						break;
					case iw7::Scriptable_EventType_HideShowBone:
						result.push_back({{"t", "hide"}, {"tag", data.hideShowBone.tagName ? data.hideShowBone.tagName : ""},
							{"hide", data.hideShowBone.hide}, {"children", data.hideShowBone.hideShowChildren}});
						break;
					case iw7::Scriptable_EventType_Explosion:
						result.push_back({{"t", "explosion"}, {"tag", data.explosion.tagName ? data.explosion.tagName : ""},
							{"radius", data.explosion.radius}, {"inner", data.explosion.dmgInner}, {"outer", data.explosion.dmgOuter}});
						break;
					case iw7::Scriptable_EventType_Screenshake:
						result.push_back({{"t", "shake"}, {"duration", data.screenshake.duration}, {"radius", data.screenshake.radius},
							{"scale", data.screenshake.scaleEarthquake}, {"earthquake", data.screenshake.isEarthquake}});
						break;
					case iw7::Scriptable_EventType_ApplyForce:
						result.push_back({{"t", "force"}, {"pos", {data.applyForce.forcePos[0], data.applyForce.forcePos[1],
							data.applyForce.forcePos[2]}}, {"vector", {data.applyForce.forceVector[0], data.applyForce.forceVector[1],
							data.applyForce.forceVector[2]}}});
						break;
					case iw7::Scriptable_EventType_Animation:
					{
						const auto* anim = data.animation.animation ? source.resolve(iw7::ASSET_TYPE_XANIMPARTS, data.animation.animation) :
							nullptr;
						std::string name = anim && anim->name ? std::string(asset_name(anim->name)) : std::string{};
						if (convert && !name.empty() && !export_script_animation(source, name, output))
						{
							name.clear();
						}

						result.push_back({{"t", "anim"}, {"anim", name}});
						break;
					}
					case iw7::Scriptable_EventType_Script:
						result.push_back({{"t", "script"}, {"notify", data.script.notification ? data.script.notification : ""},
							{"param", data.script.param}});
						break;
					case iw7::Scriptable_EventType_PartDamage:
					{
						// IW7's event keeps IW8's layout after its amount (verified on Riot's cars: a destroy flag, then the
						// damage over time 12 and its interval 0.2), which ScriptableEventPartDamageDef leaves out.
						const auto* bytes = reinterpret_cast<const std::uint8_t*>(&data.partDamage);
						float amount{}, dot{}, interval{};
						std::memcpy(&amount, bytes + 56, sizeof(amount));
						std::memcpy(&dot, bytes + 64, sizeof(dot));
						std::memcpy(&interval, bytes + 68, sizeof(interval));
						const bool destroy = bytes[60] != 0;
						result.push_back({{"t", "damage"}, {"part", reference_part(data.partDamage.partReference, self)},
							{"amount", destroy ? 1.0e9f : amount}, {"dot", dot}, {"interval", interval}});
						break;
					}
					case iw7::Scriptable_EventType_SpawnDynent:
					{
						const auto& dynent = data.spawnDynent;
						const auto model = model_name(dynent.model, true);
						if (!model.empty())
						{
							result.push_back({{"t", "debris"}, {"model", model}, {"tag", dynent.tagName ? dynent.tagName : ""},
								{"velocity", {dynent.launchLinVel[0], dynent.launchLinVel[1], dynent.launchLinVel[2]}},
								{"world", dynent.worldSpaceLinVel}});
						}

						break;
					}
					case iw7::Scriptable_EventType_AddModel:
						result.push_back({{"t", "addmodel"}, {"tag", data.addModel.tagName ? data.addModel.tagName : ""},
							{"model", model_name(data.addModel.model)}});
						break;
					case iw7::Scriptable_EventType_Light:
					{
						const auto& light = data.light;
						json entry = {{"t", "light"}, {"name", light.name ? light.name : ""}, {"scr_name", script_string(light.scrName)},
							{"intensity", {light.intensityScaleMin, light.intensityScaleMax}}, {"radius", {light.radiusMin, light.radiusMax}}};
						if (light.useColor)
						{
							entry["color"] = {light.colorLinearSrgb[0], light.colorLinearSrgb[1], light.colorLinearSrgb[2]};
						}

						if (light.useStateTransitionTime)
						{
							entry["transition"] = {light.transitionMin, light.transitionMax};
						}

						result.push_back(std::move(entry));
						break;
					}
					case iw7::Scriptable_EventType_Collision:
						result.push_back({{"t", "collision"}, {"collmap", data.collision.collmapName ? data.collision.collmapName : ""},
							{"cmodel", data.collision.clipmapCModelIndex}, {"ai_obstacle", data.collision.aiObstacle}});
						break;
					default:
						result.push_back({{"t", std::format("e{}", static_cast<int>(event.type))}});
						break;
					}
				}

				return result;
			};

			auto list = json::array();
			for (std::size_t p = 0; p < parts.size(); p++)
			{
				const auto& part = *parts[p];
				auto states = json::array();
				for (auto s = 0u; part.states && s < part.numStates; s++)
				{
					const auto& state = part.states[s];
					// A force on a state that swaps in a physics model drops the piece (a fall); others rock it. A last state of
					// the root part that swaps its model and then pushes it (picture frames that fall, broken cash registers and
					// keyboards that the push kicks away; IW7 sets no physics flag on them) drops it too: when only such states
					// set the model, it becomes a physics prop that the push launches (export_effects: physics_models).
					auto state_events = events(state.base.events, state.base.numEvents, static_cast<int>(p));
					const auto last = state.type == iw7::Scriptable_StateType_Simple && std::ranges::none_of(state_events, [](const json& event)
					{
						return event.value("t", std::string{}) == "state";
					});
					std::string swapped;
					auto falls = false;
					for (const auto& event : state_events)
					{
						const auto t = event.value("t", std::string{});
						if (t == "model")
						{
							swapped = event.value("model", std::string{});
							falls = falls || event.value("physics", false) || event.value("dynamic", false);
						}
						else if (t == "force" && last && parents[p] < 0 && !swapped.empty())
						{
							falls = true;
							if (convert)
							{
								output.swap_models[swapped]++;
							}

							break;
						}
					}

					for (auto& event : state_events)
					{
						if (event.value("t", std::string{}) == "force")
						{
							event["fall"] = falls;
						}
					}

					const auto health = state.type == iw7::Scriptable_StateType_Health;
					states.push_back({{"name", state.base.name ? state.base.name : ""},
						{"kind", health ? "health" : state.type == iw7::Scriptable_StateType_Scripted ? "scripted" : "simple"},
						{"health", health ? state.data.health.health : 0}, {"mindamage", health ? state.data.health.minimumDamage : 0},
						{"to_parent", health ? state.data.health.damagePropagationToParent : 0.0f},
						{"events", std::move(state_events)}});
				}

				list.push_back({{"name", part.name ? part.name : ""}, {"tag", part.tagName ? part.tagName : ""}, {"parent", parents[p]},
					{"states", states}});
			}

			return {{"def", def.name ? std::string(asset_name(def.name)) : std::string{}}, {"parts", list}};
		}

		// The effects that the first state of each part of a scriptable plays, on the tags of the instance's model.
		void add_scriptable_effects(const source_map& source, const iw7::ScriptableDef& def, const iw7::XModel* model,
			const iw7::ScriptableInstanceContext& context, extraction& output)
		{
			for (auto p = 0u; def.parts && p < def.numParts; p++)
			{
				const auto& part = def.parts[p];
				if (!part.states || !part.numStates)
				{
					continue;
				}

				const auto& state = part.states[0];
				for (auto e = 0u; state.base.events && e < state.base.numEvents; e++)
				{
					const auto& event = state.base.events[e];
					if (event.type != iw7::Scriptable_EventType_PFX)
					{
						continue;
					}

					const auto& fx = event.data.particleFX;
					const auto vfx = fx.effectDef.type == iw7::FX_COMBINED_VFX ? source.resolve(iw7::ASSET_TYPE_VFX, fx.effectDef.u.vfx)
						: nullptr;
					if (!vfx)
					{
						continue;
					}

					std::vector<std::string> tags;
					for (auto t = 0u; fx.scrTagNames && t < fx.scrTagCount; t++)
					{
						tags.push_back(script_string(fx.scrTagNames[t]));
					}

					if (tags.empty())
					{
						tags.push_back(part.tagName ? part.tagName : "");
					}

					const auto effect = std::string(asset_name(iw7::get_asset_name(iw7::ASSET_TYPE_VFX, vfx)));
					for (const auto& tag : tags)
					{
						auto placement = tag_placement(model, tag, context.initialOrigin, context.initialAngles);
						if (!placement)
						{
							ZONETOOL_WARNING("iw7: scriptable %s: its model has no tag %s for effect %s", def.name ? def.name : "?", tag.data(),
								effect.data());
							continue;
						}

						// The effects play at the bind pose of their tags, as createfx one-shots (effect entities, which every client
						// receives), also on animated models. Played on the tag (playfxontag) they followed the animation, but
						// Replay sends playfxontag as an entity event, which a client that connects after it never receives:
						// the map script plays them at the start, before the host's own client is in the game, and the ops
						// table's planets never showed (owner test of 58aac89; they had shown as one-shots before 7668e89).
						// IW7 plays them client-side in its scriptable system.
						placement->effect = effect;
						output.scriptable_effects.push_back(std::move(*placement));
					}
				}
			}
		}
	}

	float light_unit_scale(const source_map& source)
	{
		const auto com_world = source.com_world();
		if (!com_world || com_world->primaryLightCount <= sun_light)
		{
			return 1.0f;
		}

		const auto& sun = com_world->primaryLights[sun_light];
		const auto luminance = 0.2126f * sun.color[0] + 0.7152f * sun.color[1] + 0.0722f * sun.color[2];
		return light_type(sun.type) == 1 && luminance > 0.0f ? 1.0f / luminance : 1.0f;
	}

	void export_com_world(const source_map& source, extraction& output)
	{
		const auto com_world = source.com_world();
		if (!com_world)
		{
			throw std::runtime_error(std::format("zone \"{}\" holds no com world", source.name()));
		}

		// IW7 local lights are physical, as Replay's: a light of colour c lights a surface with c * N.L / (pi d^2)
		// inside its radius, in the units of the lightmaps (calibration: lamps then light their surroundings at the
		// level of the brightest lightmap texels). Each takes Replay's linear light definition.
		const auto scale = light_unit_scale(source);
		auto lights = json::array();
		std::size_t local_lights{};
		std::size_t narrowed_spots{};
		std::size_t leaning_ups{};
		std::size_t large_bulbs{};

		// IW7 IES light definitions (Frontier: 24 wall sconces of `light_ies_sconce_zmb`) sample the map's 128-texel R8
		// strip `*ieslookup` at coordOffset + coordinate * coordScale, the coordinate from the spot's angle as Replay's
		// (both shaders: c > 0 ? (1 - c^2) / 2 : 1 - (1 - c^2) / 2). A spot takes its profile as a Replay segment (texel j =
		// coordinate j / 255), linear between IW7's texels as its sampler reads them. Without it the sconces lit their
		// whole cone evenly; IW7's peaks 16 degrees off the axis and stays under 8% across most of the cone.
		auto ies_profiles = json::array();
		std::unordered_map<std::string, bool> ies_definitions;
		std::size_t ies_lights{};
		const auto* ies_lookup = static_cast<const iw7::GfxImage*>(source.find(iw7::ASSET_TYPE_IMAGE, "*ieslookup"));
		const auto ies_definition = [&](const char* def_name) -> std::string
		{
			if (!def_name || !ies_lookup || !ies_lookup->pixelData || ies_lookup->streamed ||
				ies_lookup->imageFormat != DXGI_FORMAT_R8_UNORM || !ies_lookup->width || ies_lookup->dataLen1 < ies_lookup->width)
			{
				return {};
			}

			const std::string name(asset_name(def_name));
			if (const auto found = ies_definitions.find(name); found != ies_definitions.end())
			{
				return found->second ? name : std::string{};
			}

			const auto* def = static_cast<const iw7::GfxLightDef*>(source.find(iw7::ASSET_TYPE_LIGHT_DEF, name));
			const auto usable = def && def->iesProfile && *def->iesProfile && def->coordScale > 0.0f;
			ies_definitions.emplace(name, usable);
			if (!usable)
			{
				return {};
			}

			const auto width = static_cast<float>(ies_lookup->width);
			auto texels = json::array();
			for (auto texel = 0; texel < 128; texel++)
			{
				const auto coordinate = static_cast<float>(texel) / 255.0f;
				const auto x = std::clamp((def->coordOffset + coordinate * def->coordScale) * width - 0.5f, 0.0f, width - 1.0f);
				const auto low = static_cast<std::size_t>(x);
				const auto high = std::min<std::size_t>(low + 1, ies_lookup->width - 1);
				const auto t = x - static_cast<float>(low);
				const auto value = ies_lookup->pixelData[low] * (1.0f - t) + ies_lookup->pixelData[high] * t;
				texels.push_back(std::clamp(static_cast<int>(std::lround(value)), 0, 255));
			}

			ies_profiles.push_back({{"definition", name}, {"profile", def->iesProfile}, {"texels", std::move(texels)}});
			return name;
		};
		for (auto i = 0u; i < com_world->primaryLightCount; i++)
		{
			const auto& light = com_world->primaryLights[i];
			auto type = light_type(light.type);
			std::array<float, 3> color{light.color[0], light.color[1], light.color[2]};
			const auto luminance = 0.2126f * color[0] + 0.7152f * color[1] + 0.0722f * color[2];
			std::optional<float> intensity;
			if (type == 1 && i != sun_light)
			{
				// One sun lights the world.
				type = 0;
			}
			else if (type == 1)
			{
				std::ranges::for_each(color, [&](float& channel) { channel *= scale; });
			}
			else if (type >= 2 && luminance > 0.0f && light.radius > 0.0f)
			{
				std::ranges::for_each(color, [&](float& channel) { channel /= luminance; });
				intensity = luminance / std::numbers::pi_v<float>;
				local_lights++;
			}
			else
			{
				type = 0;
			}

			// The pipeline takes a direction for each light and a cone that opens for each spot.
			std::array<float, 3> direction{light.dir[0], light.dir[1], light.dir[2]};
			if (type && direction[0] * direction[0] + direction[1] * direction[1] + direction[2] * direction[2] < 1.0e-6f)
			{
				direction = {0.0f, 0.0f, 1.0f};
			}

			auto outer = light.cosHalfFovOuter;
			auto inner = light.cosHalfFovInner;
			if (type == 2)
			{
				outer = std::clamp(outer, 0.001f, 0.998f);
				// Replay's shadowed spots open at most 75 degrees (stock mp_hideout: cos 0.259 at the widest); IW7
				// places a few near-hemispherical spots (Frontier: 89 degrees), whose shadow projection degenerates.
				if (light.canUseShadowMap && outer < 0.259f)
				{
					outer = 0.259f;
					narrowed_spots++;
				}

				inner = std::clamp(inner, outer + 0.001f, 1.0f);
			}

			// Replay keeps a spot's up perpendicular to its direction (stock: |dir.up| = 0 on every light); IW7's authored ups
			// lean along it (Frontier: 66 of 119 spots by more than 0.1, and two shadowed spots have their up along the
			// direction), which skews or breaks the shadow view and the light volume (owner: lights switching off and
			// misplaced). Gram-Schmidt against the direction.
			std::array<float, 3> up{light.up[0], light.up[1], light.up[2]};
			{
				const auto dot = up[0] * direction[0] + up[1] * direction[1] + up[2] * direction[2];
				for (auto c = 0; c < 3; c++)
				{
					up[c] -= dot * direction[c];
				}

				auto length = std::sqrt(up[0] * up[0] + up[1] * up[1] + up[2] * up[2]);
				if (length < 1.0e-3f)
				{
					// Any axis across the direction: the world axis the direction leans least on.
					const std::array<float, 3> axis = std::abs(direction[2]) < 0.9f ? std::array{0.0f, 0.0f, 1.0f} : std::array{1.0f, 0.0f, 0.0f};
					const auto along = axis[0] * direction[0] + axis[1] * direction[1] + axis[2] * direction[2];
					for (auto c = 0; c < 3; c++)
					{
						up[c] = axis[c] - along * direction[c];
					}

					length = std::sqrt(up[0] * up[0] + up[1] * up[1] + up[2] * up[2]);
				}

				if (std::abs(dot) > 1.0e-3f)
				{
					leaning_ups++;
				}

				for (auto& c : up)
				{
					c /= length;
				}
			}

			// Stock spots' bulbs reach 8 units; IW7's area lights reach 40, which moves Replay's cone apex (bulb radius
			// over tan of the outer angle) far behind the fixture.
			auto bulb_radius = std::max(light.bulbRadius, 1.0f);
			if (bulb_radius > 8.0f)
			{
				bulb_radius = 8.0f;
				large_bulbs++;
			}

			// Spots keep an IES definition of IW7's (the profile covers their forward hemisphere); the others take Replay's
			// linear light definition.
			auto definition = intensity ? std::string("light_point_linear") : std::string{};
			if (type == 2 && intensity)
			{
				if (auto ies = ies_definition(light.defName); !ies.empty())
				{
					definition = std::move(ies);
					ies_lights++;
				}
			}

			json entry =
			{
				{"type", type},
				{"can_use_shadow_map", light.canUseShadowMap != 0},
				{"exponent", light.exponent},
				{"color", vector_json(color.data(), 3)},
				{"direction", vector_json(direction.data(), 3)},
				{"up", vector_json(up.data(), 3)},
				{"authored_up", true},
				{"origin", vector_json(light.origin, 3)},
				{"radius", light.radius},
				{"cos_half_fov_outer", outer},
				{"cos_half_fov_inner", inner},
				{"cos_half_fov_expanded", outer},
				{"rotation_limit", light.rotationLimit},
				{"translation_limit", light.translationLimit},
				{"entity_id", light.entityId},
				{"bulb_radius", bulb_radius},
				{"bulb_length", vector_json(light.bulbLength, 3)},
				{"volumetric", light.isVolumetric != 0},
				{"needs_dynamic_shadows", light.needsDynamicShadows != 0},
				{"shadow_softness", light.shadowSoftness},
				{"shadow_bias", light.shadowBias},
				{"shadow_area", light.shadowArea},
				{"distance_falloff", light.distanceFalloff > 0.0f ? light.distanceFalloff : 0.2f},
				{"fade_offset", {light.fadeOffsetRt[0], light.fadeOffsetRt[1]}},
				{"uv_intensity", light.uvIntensity},
				{"ir_intensity", light.irIntensity},
				{"definition", definition},
			};

			if (intensity)
			{
				entry["intensity"] = *intensity;
			}

			lights.push_back(std::move(entry));
		}

		if (narrowed_spots || leaning_ups || large_bulbs)
		{
			ZONETOOL_INFO("iw7: lights: %zu shadowed spots narrowed to 75 degrees, %zu ups made perpendicular to their direction, "
				"%zu bulb radii capped at 8", narrowed_spots, leaning_ups, large_bulbs);
		}

		// The attenuation that IW3's light_point_linear definition names: the pipeline matches it to Replay's own.
		if (local_lights)
		{
			const auto path = output.extracted / "lights" / "light_point_linear";
			std::filesystem::create_directories(path.parent_path());
			std::ofstream(path, std::ios::binary | std::ios::trunc).write("bfalloff_linear", 16);
		}

		if (ies_lights)
		{
			ZONETOOL_INFO("iw7: lights: %zu spots keep the IES profiles of %zu light definitions", ies_lights, ies_profiles.size());
		}

		write_json(output.extracted / (bsp_name(source) + ".replay-comworld.json"),
		{
			{"schema", 1},
			{"name", bsp_name(source)},
			{"in_use", true},
			{"primary_lights", std::move(lights)},
			{"ies_profiles", std::move(ies_profiles)},
		});

		ZONETOOL_INFO("iw7: exported %u primary lights with %zu local lights", com_world->primaryLightCount, local_lights);
	}

	void remove_missing_entity_models(const source_map& source, const extraction& output)
	{
		const auto path = output.extracted / (bsp_name(source) + ".ents");
		std::ifstream input(path, std::ios::binary);
		std::string entities((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
		input.close();

		std::string result;
		result.reserve(entities.size());
		std::size_t removed{};
		std::size_t start = 0;
		while (start < entities.size())
		{
			const auto end = std::min(entities.find('\n', start), entities.size());
			const std::string_view line(entities.data() + start, end - start);
			constexpr std::string_view key = "\"model\" \"";
			const auto name = line.starts_with(key) && line.size() > key.size() + 1 && line[key.size()] != '*'
				? line.substr(key.size(), line.size() - key.size() - 1) : std::string_view{};
			if (!name.empty() && !output.exported_models.contains(file_name(name)))
			{
				removed++;
			}
			else
			{
				result.append(line);
				result += '\n';
			}

			start = end + 1;
		}

		std::ofstream(path, std::ios::binary | std::ios::trunc).write(result.data(), static_cast<std::streamsize>(result.size()));
		ZONETOOL_INFO("iw7: removed the model of %zu entities whose model is not in the loaded zones", removed);
	}

	namespace
	{
		std::string trigger_string(const iw7::ClientTriggers& triggers, const int offset)
		{
			if (offset < 0 || !triggers.triggerString || static_cast<unsigned>(offset) >= triggers.triggerStringLength)
			{
				return {};
			}

			const auto* text = triggers.triggerString + offset;
			return std::string(text, strnlen(text, triggers.triggerStringLength - offset));
		}

		// maps/clienttriggers.json for the importer (iw3_fastfile.cpp ReadClientTriggers): IW7's client triggers (audio zones,
		// NPC audio zones, blends) in trigger space, as Replay tests them (CG point - origins[i] against triggerSpaceBounds and
		// the slabs; IW7 keeps world-space hulls). Audio and NPC zone names come out by name. The roles without a Replay
		// counterpart in converted maps clear: vision sets (1, blend 4: converted maps ship no vision sets) and audio occluders
		// (0x20); a trigger keeps its index either way. IW7 has no priority; Replay plays the highest-priority audio zone and,
		// between zones of equal priority, the one whose box lies inside the other's (sub_1416F1B90, verified-static), so
		// every IW7 zone takes one priority (10) and the nesting picks the inner zone. Stock zones' default ambience is a
		// map-wide priority-0 trigger of the map's own zone (mp_shipment trigger 1); IW7 plays its map zone outside every
		// trigger, so a map-wide priority-0 trigger of that zone follows the others.
		void export_client_triggers(const source_map& source, const iw7::MapEnts& map_ents, extraction& output)
		{
			// The PA speakers (the scripts' announcer speakers: origin, group name hash, channel hash) keep IW7's layout in
			// Replay (AudioPASpeaker, 20 B).
			auto speakers = json::array();
			for (auto i = 0u; map_ents.audioPASpeakers && i < map_ents.audioPASpeakerCount; i++)
			{
				const auto& speaker = map_ents.audioPASpeakers[i];
				speakers.push_back({{"origin", {speaker.origin[0], speaker.origin[1], speaker.origin[2]}}, {"nameHash", speaker.nameHash},
					{"entChannelHash", speaker.entChannelHash}});
			}

			const auto& triggers = map_ents.clientTrigger;
			auto count = triggers.trigger.count;
			if (!triggers.trigger.models || !triggers.triggerType || !triggers.origins)
			{
				count = 0;
			}


			const auto& hulls = triggers.trigger.hulls;
			const auto& slabs = triggers.trigger.slabs;
			constexpr std::uint16_t audio_roles = 0x2 | 0x8 | 0x10 | 0x40;
			auto items = json::array();
			std::size_t audio{};
			std::size_t cleared{};
			for (auto i = 0u; i < count; i++)
			{
				const auto& model = triggers.trigger.models[i];
				const auto& origin = triggers.origins[i];
				const auto type = static_cast<std::uint16_t>(triggers.triggerType[i]);
				const auto kept = static_cast<std::uint16_t>(type & audio_roles);
				cleared += kept != type;
				audio += (kept & 0x2) != 0;

				auto hull_items = json::array();
				for (auto h = 0u; h < model.hullCount && model.firstHull + h < triggers.trigger.hullCount; h++)
				{
					const auto& hull = hulls[model.firstHull + h];
					auto slab_items = json::array();
					for (auto s = 0u; s < hull.slabCount && hull.firstSlab + s < triggers.trigger.slabCount; s++)
					{
						const auto& slab = slabs[hull.firstSlab + s];
						const auto along = slab.dir[0] * origin[0] + slab.dir[1] * origin[1] + slab.dir[2] * origin[2];
						slab_items.push_back({{"dir", {slab.dir[0], slab.dir[1], slab.dir[2]}}, {"midPoint", slab.midPoint - along},
							{"halfSize", slab.halfSize}});
					}

					hull_items.push_back({
						{"midPoint", {hull.bounds.midPoint[0] - origin[0], hull.bounds.midPoint[1] - origin[1], hull.bounds.midPoint[2] - origin[2]}},
						{"halfSize", {hull.bounds.halfSize[0], hull.bounds.halfSize[1], hull.bounds.halfSize[2]}},
						{"slabs", std::move(slab_items)}});
				}

				const auto value16 = [&](const short* values) { return values ? values[i] : static_cast<short>(-1); };
				json item = {
					{"type", kept},
					{"origin", {origin[0], origin[1], origin[2]}},
					{"scriptDelay", triggers.scriptDelay ? triggers.scriptDelay[i] : 0.0f},
					{"audio", (kept & 0x2) ? trigger_string(triggers, value16(triggers.audioTriggers)) : std::string{}},
					{"npc", (kept & 0x10) ? trigger_string(triggers, value16(triggers.npcTriggers)) : std::string{}},
					{"blend", (kept & 0x8) ? value16(triggers.blendLookup) : -1},
					{"audioState", value16(triggers.audioStateIds)},
					{"hulls", std::move(hull_items)},
				};
				if (triggers.audioRvbPanInfo)
				{
					const auto& pan = triggers.audioRvbPanInfo[i];
					item["rvbPan"] = {{"hasCustomPosition", pan.hasCustomPosition}, {"position", {pan.position[0], pan.position[1], pan.position[2]}},
						{"maxWallDistance", pan.maxWallDistance}, {"minReverbVolume", pan.minReverbVolume}};
				}

				items.push_back(std::move(item));
			}

			for (auto& item : items)
			{
				item["priority"] = (item["type"].get<int>() & 0x2) ? 10.0f : 0.0f;
			}

			// The blend nodes of the kept blends (audio blends), renumbered in order.
			auto nodes = json::array();
			const auto& blend = map_ents.clientTriggerBlend;
			std::map<int, int> renumbered;
			for (auto& item : items)
			{
				const auto index = item["blend"].get<int>();
				if (index < 0)
				{
					continue;
				}

				if (!blend.blendNodes || index >= blend.numClientTriggerBlendNodes)
				{
					item["blend"] = -1;
					item["type"] = item["type"].get<int>() & ~0x8;
					continue;
				}

				const auto [found, added] = renumbered.try_emplace(index, static_cast<int>(nodes.size()));
				if (added)
				{
					const auto& node = blend.blendNodes[index];
					nodes.push_back({{"pointA", {node.pointA[0], node.pointA[1], node.pointA[2]}},
						{"pointB", {node.pointB[0], node.pointB[1], node.pointB[2]}}, {"triggerA", node.triggerA}, {"triggerB", node.triggerB}});
				}

				item["blend"] = found->second;
			}

			// The map-wide trigger of the map's own zone, when the sound table has one.
			const auto bank = static_cast<const iw7::SndBank*>(source.find(iw7::ASSET_TYPE_SOUND_BANK, source.name() + ".all"));
			const auto world = source.world();
			bool map_zone{};
			for (auto z = 0u; bank && z < bank->soundTable.zoneCount; z++)
			{
				const auto* name = bank->soundTable.zones[z].zoneName;
				map_zone = map_zone || (name && source.name() == name);
			}

			if (map_zone && world)
			{
				const auto& bounds = world->bounds;
				json hull = {{"midPoint", {0.0f, 0.0f, 0.0f}},
					{"halfSize", {bounds.halfSize[0] + 512.0f, bounds.halfSize[1] + 512.0f, bounds.halfSize[2] + 512.0f}}, {"slabs", json::array()}};
				items.push_back({{"type", 0x2}, {"origin", {bounds.midPoint[0], bounds.midPoint[1], bounds.midPoint[2]}}, {"scriptDelay", 0.25f},
					{"audio", source.name()}, {"npc", ""}, {"blend", -1}, {"audioState", -1}, {"priority", 0.0f},
					{"hulls", json::array({std::move(hull)})}});
				audio++;
			}

			const auto node_count = nodes.size();
			const auto speaker_count = speakers.size();
			if (items.empty() && !speaker_count)
			{
				return;
			}

			write_json(output.extracted / "maps" / "clienttriggers.json", {{"map", source.name()}, {"triggers", std::move(items)},
				{"blend_nodes", std::move(nodes)}, {"pa_speakers", std::move(speakers)}});
			if (speaker_count)
			{
				ZONETOOL_INFO("iw7: exported %zu PA speakers", speaker_count);
			}

			ZONETOOL_INFO("iw7: exported %u client triggers (%zu audio zones%s; %zu lose their vision or occluder roles) and %zu of %u blend nodes",
				count, audio, map_zone ? ", with the map-wide map zone" : "", cleared, node_count, blend.numClientTriggerBlendNodes);
		}
	}

	// IW7 keeps no mass for its clutter (a model's physics LOD holds only its collision mesh): the mass of the convex hull of
	// its first LOD at the density that gives Replay's own clutter its mass (stock buckets: 1.5 kg in 0.054 cubic Havok
	// units, 28 kg per cubic Havok unit, a Havok unit being 32 game units), within Replay's clutter range.
	std::optional<clutter_body> clutter_physics(const source_map& source, const iw7::XModel& model)
	{
		std::vector<std::array<float, 3>> points;
		std::map<std::string_view, double> areas;
		for (const auto& surface : model_mesh(source, model))
		{
			for (const auto& vertex : surface.vertices)
			{
				points.push_back(vertex.position);
			}

			const auto bits = surface.material ? static_cast<std::uint64_t>(surface.material->info.surfaceTypeBits) : 0;
			const auto type = bits ? static_cast<std::size_t>(std::countr_zero(bits)) : surface_names.size();
			for (const auto& triangle : surface.triangles)
			{
				if (type >= surface_names.size() || std::ranges::any_of(triangle, [&](const auto i) { return i >= surface.vertices.size(); }))
				{
					continue;
				}

				const auto& a = surface.vertices[triangle[0]].position;
				const auto& b = surface.vertices[triangle[1]].position;
				const auto& c = surface.vertices[triangle[2]].position;
				const std::array<double, 3> u{b[0] - a[0], b[1] - a[1], b[2] - a[2]};
				const std::array<double, 3> v{c[0] - a[0], c[1] - a[1], c[2] - a[2]};
				areas[surface_names[type]] += std::hypot(u[1] * v[2] - u[2] * v[1], u[2] * v[0] - u[0] * v[2], u[0] * v[1] - u[1] * v[0]);
			}
		}

		float volume{};
		try
		{
			volume = iw8::havok::convex_mass_properties(iw8::havok::convex_hull(points, 0.0f), 1.0f).volume;
		}
		catch (const std::exception&)
		{
			return std::nullopt;
		}

		constexpr auto density = 28.0f / (32.0f * 32.0f * 32.0f);
		const auto surface = std::ranges::max_element(areas, {}, [](const auto& entry) { return entry.second; });
		return clutter_body{std::clamp(volume * density, 0.1f, 200.0f), surface != areas.end() ? std::string(surface->first) : "default"};
	}

	std::string write_physics_preset(const extraction& output, const std::string& model, const clutter_body& body)
	{
		const auto name = "iw7_" + model;
		std::filesystem::create_directories(output.extracted / "physic");
		std::ofstream(output.extracted / "physic" / name, std::ios::binary | std::ios::trunc)
			<< std::format("PHYSIC\\mass\\{:.3f}\\sndAliasPrefix\\{}", body.mass, body.surface);
		return name;
	}

	void export_entities(const source_map& source, extraction& output)
	{
		const auto map_ents = source.map_ents();
		if (!map_ents || !map_ents->entityString)
		{
			throw std::runtime_error(std::format("zone \"{}\" holds no map entities", source.name()));
		}

		export_client_triggers(source, *map_ents, output);

		auto entities = with_trigger_models(source,
			with_key_names(std::string(map_ents->entityString, strnlen(map_ents->entityString, map_ents->numEntityChars))));

		// The models that entities place (script models) convert with the static models; "*N" names a brush model and
		// "?N" a client trigger.
		static const std::regex model_key(R"re("model"\s+"([^"*?][^"]*)")re");
		for (auto match = std::sregex_iterator(entities.begin(), entities.end(), model_key); match != std::sregex_iterator(); ++match)
		{
			output.models.try_emplace(file_name((*match)[1].str()), static_cast<const iw7::XModel*>(
				source.find(iw7::ASSET_TYPE_XMODEL, (*match)[1].str())));
		}

		// IW7 keeps the spawn points beside the entity string.
		const auto& spawns = map_ents->spawnList;
		for (auto i = 0u; i < spawns.spawnsCount; i++)
		{
			const auto& spawn = spawns.spawns[i];
			entities += "{\n";
			entities += std::format("\"classname\" \"{}\"\n", script_string(spawn.name));
			entities += std::format("\"origin\" \"{}\"\n", vector_text(spawn.origin));
			entities += std::format("\"angles\" \"{}\"\n", vector_text(spawn.angles));
			if (const auto target = script_string(spawn.target); !target.empty())
			{
				entities += std::format("\"target\" \"{}\"\n", target);
			}

			if (const auto noteworthy = script_string(spawn.script_noteworthy); !noteworthy.empty())
			{
				entities += std::format("\"script_noteworthy\" \"{}\"\n", noteworthy);
			}

			entities += "}\n";
		}

		// IW7's scriptables (screens, displays, lamps and props with states): an IW3 script model of each instance's
		// model at its initial placement (the first model of its definition when the instance names none), and the
		// effects of the first state of each of its parts on their tags. The kinds that break (destructible_of) take a
		// targetname that the map script breaks them by; those whose first state plays an animation, one that the map script
		// animates them by. Their other states and events do not convert.
		const auto& scriptables = map_ents->scriptableMapEnts;
		std::set<const iw7::ScriptableDef*> dumped_machines;
		std::size_t placed{};
		std::size_t destructibles{};
		for (auto i = 0u; scriptables.instances && i < scriptables.totalInstanceCount; i++)
		{
			const auto& context = scriptables.instances[i].contextHeader.context;
			const auto def = source.resolve(iw7::ASSET_TYPE_SCRIPTABLE, context.def);
			auto model = source.resolve(iw7::ASSET_TYPE_XMODEL, context.modelData.model);
			if (!model && def && def->numXModels && def->models)
			{
				model = source.resolve(iw7::ASSET_TYPE_XMODEL, def->models[0]);
			}

			const auto name = model ? asset_name(iw7::get_asset_name(iw7::ASSET_TYPE_XMODEL, model)) : std::string_view{};

			// IW7_SCRIPTABLE_DUMP=2: each definition's state machine (scriptable_machine), once.
			if (const auto* level = std::getenv("IW7_SCRIPTABLE_DUMP"); level && std::string_view(level) == "2" && def &&
				dumped_machines.insert(def).second)
			{
				ZONETOOL_INFO("iw7 scriptable machine: %s", scriptable_machine(source, *def, output, false).dump().data());
			}

			// IW7_SCRIPTABLE_DUMP=1: each instance's definition, model source, parts and first states with their events.
			if (std::getenv("IW7_SCRIPTABLE_DUMP"))
			{
				std::string parts;
				for (auto part = 0u; def && def->parts && part < def->numParts; part++)
				{
					const auto& p = def->parts[part];
					parts += std::format(" [{}:", p.name ? p.name : "?");
					for (auto st = 0u; p.states && st < p.numStates && st < 4; st++)
					{
						const auto& state = p.states[st];
						parts += std::format(" {}({}", state.base.name ? state.base.name : "?", static_cast<int>(state.type));
						for (auto e = 0u; state.base.events && e < state.base.numEvents; e++)
						{
							const auto& event = state.base.events[e];
							if (event.type == iw7::Scriptable_EventType_Model)
							{
								const auto* m = source.resolve(iw7::ASSET_TYPE_XMODEL, event.data.model.model);
								parts += std::format(" model={}", m ? asset_name(iw7::get_asset_name(iw7::ASSET_TYPE_XMODEL, m)) : "null");
							}
							else if (event.type == iw7::Scriptable_EventType_HideShowBone)
							{
								parts += " hideshow";
							}
							else if (event.type == iw7::Scriptable_EventType_Animation)
							{
								parts += " anim";
							}
							else if (event.type == iw7::Scriptable_EventType_Collision)
							{
								const auto& collision = event.data.collision;
								parts += std::format(" collision(cmodel {} collmap {} neverMoves {} canPush {} canTouch {} aiObstacle {})",
									collision.clipmapCModelIndex, collision.collmapName ? collision.collmapName : "-", collision.neverMoves,
									collision.canPush, collision.canTouch, collision.aiObstacle);
							}
							else
							{
								parts += std::format(" e{}", static_cast<int>(event.type));
							}
						}

						parts += ")";
					}

					parts += "]";
				}

				// The instance's index words (light indices, inferred: IW8's instances name up to 5 primary lights).
				std::string indices;
				for (const auto value : scriptables.instances[i].contextHeader.unk02)
				{
					indices += std::format(" {}", value);
				}

				indices += " |";
				for (const auto value : scriptables.instances[i].contextHeaderLocalClient[0].unk02)
				{
					indices += std::format(" {}", value);
				}

				ZONETOOL_INFO("iw7 scriptable %u: def %s flags %#x instance model %s (%s) at %s targetname %s noteworthy %s indices%s parts%s", i,
					def && def->name ? def->name : "?", def ? def->flags : 0, name.empty() ? "-" : name.data(),
					context.modelData.model ? "instance" : "def", vector_text(context.initialOrigin).data(),
					script_string(scriptables.instances[i].targetname).data(), script_string(scriptables.instances[i].debugNameScr).data(),
					indices.data(), parts.data());
			}

			const auto finite = std::ranges::all_of(context.initialOrigin, [](const float v) { return std::isfinite(v); }) &&
				std::ranges::all_of(context.initialAngles, [](const float v) { return std::isfinite(v); });
			// The game modes' scriptables (Hardpoint's zone chevrons) show only while their mode runs them. The ops table's
			// scriptable is its solar system (opsmap_solar_system_large), which shows without a script.
			const std::string_view def_name = def && def->name ? asset_name(def->name) : std::string_view{};
			if (name.empty() || !finite || def_name.find("hardpoint") != std::string_view::npos)
			{
				continue;
			}

			entities += "{\n\"classname\" \"script_model\"\n";
			entities += std::format("\"model\" \"{}\"\n", name);
			entities += std::format("\"origin\" \"{}\"\n", vector_text(context.initialOrigin));
			entities += std::format("\"angles\" \"{}\"\n", vector_text(context.initialAngles));
			// An animated scriptable (a model of several bones) plays its first state's animation by the map script.
			const auto states = def && model && model->numBones > 1 && model->boneNames && model->baseMat ?
				animation_states(source, *def) : std::vector<extraction::scriptable_state>{};
			const auto first = std::ranges::find_if(states, [](const auto& state) { return state.first; });
			const auto* animation = first != states.end() ? first->anim : nullptr;
			std::string animated;
			bool machine{};
			std::string kind(def_name);
			std::ranges::transform(kind, kind.begin(), [](const unsigned char c)
			{
				return std::isalnum(c) ? static_cast<char>(std::tolower(c)) : '_';
			});
			if (const auto targetname = script_string(scriptables.instances[i].targetname); !targetname.empty())
			{
				entities += std::format("\"targetname\" \"{}\"\n", targetname);
				animated = animation ? targetname : std::string{};
				// The map script sets the states of the instances it finds by their targetname (Neon's holograms build, a
				// car's hologram regenerates it): the instance runs its kind's state machine, which IW7's
				// setscriptablepartstate (iw7_setscriptablepartstate) changes the states of. Animated kinds keep the table of
				// their animations.
				if (!animation && def && changes_state(*def))
				{
					const auto machine_name = "iw7_scriptable_" + kind;
					if (!output.scriptable_machines.contains(machine_name))
					{
						output.scriptable_machines.emplace(machine_name, scriptable_machine(source, *def, output, true));
					}

					output.scriptable_targets.push_back({machine_name, std::string(targetname),
						{context.initialOrigin[0], context.initialOrigin[1], context.initialOrigin[2]}});
					machine = true;
				}
			}
			else if (const auto destructible = def ? destructible_of(source, *def, output) : std::nullopt)
			{
				entities += std::format("\"targetname\" \"{}\"\n", destructible->targetname);
				output.scriptable_destructibles.try_emplace(destructible->targetname, *destructible);
				destructibles++;
			}
			else if (animation)
			{
				animated = "iw7_animated_" + kind;
				entities += std::format("\"targetname\" \"{}\"\n", animated);
			}
			else if (def && takes_damage(*def))
			{
				const auto machine_name = "iw7_scriptable_" + kind;
				entities += std::format("\"targetname\" \"{}\"\n", machine_name);
				if (!output.scriptable_machines.contains(machine_name))
				{
					output.scriptable_machines.emplace(machine_name, scriptable_machine(source, *def, output, true));
				}

				machine = true;
			}

			if (!animated.empty())
			{
				const auto axis = angles_to_axes(context.initialAngles);
				auto& placement = output.scriptable_placements.emplace_back();
				placement.model = file_name(name);
				for (auto c = 0u; c < 3; c++)
				{
					placement.origin[c] = context.initialOrigin[c];
					for (auto k = 0u; k < 3; k++)
					{
						placement.axes[k][c] = static_cast<float>(axis[k][c]);
					}
				}

				output.scriptable_animations.try_emplace(animated, std::string(asset_name(animation->name)));
				output.animated_models.try_emplace(file_name(name), model);
				output.scriptable_animation_models[animated].insert(file_name(name));
				output.scriptable_states.try_emplace(file_name(name), states);
			}

			// IW7 keeps the instance's script_noteworthy after its targetname (the floor panels' "start", "green" and "red",
			// which the map script raises them by).
			if (const auto noteworthy = script_string(scriptables.instances[i].debugNameScr); !noteworthy.empty())
			{
				entities += std::format("\"script_noteworthy\" \"{}\"\n", noteworthy);
			}

			entities += "}\n";
			output.models.try_emplace(file_name(name), model);
			placed++;
			// The state machines play the effects of their first states themselves (as effect entities, iw7_scriptable_fx).
			if (def && !machine)
			{
				add_scriptable_effects(source, *def, model, context, output);
			}
		}

		ZONETOOL_INFO("iw7: placed %zu of %u scriptables (%u runtime, %u reserved) as script models, with %zu effects of their first states",
			placed, scriptables.totalInstanceCount, scriptables.runtimeInstanceCount, scriptables.reservedInstanceCount,
			output.scriptable_effects.size());
		ZONETOOL_INFO("iw7: %zu of them break (%zu kinds: a health state, then the broken model, its effects and sound)", destructibles,
			output.scriptable_destructibles.size());
		ZONETOOL_INFO("iw7: %zu kinds of them animate (%zu instances)", output.scriptable_animations.size(),
			output.scriptable_placements.size());

		// IW7's dynamic entities (MapEnts dynEntDefList: clutter props with physics, hinged doors, linked props) drew
		// nothing (Frontier: 77 fire extinguishers, serving trays, bottles and other props of its rooms and tables). The
		// clutter that IW7 simulates becomes Replay clutter (dynamic_models), which bullets and explosions push as in IW7;
		// each other one that spawns with a model places an IW3 script model at its initial pose (quaternion and origin).
		// Those of scriptable instances (DYNENT_TYPE_SCRIPTABLEINST) are the scriptables placed above; brush ones keep
		// their brush model's static collision. The hinge entries without a model or pose are empty slots.
		std::map<const iw7::XModel*, std::optional<clutter_body>> physics;
		std::size_t dynents_clutter{};
		std::size_t dynents_placed{};
		std::size_t dynents_brushes{};
		std::size_t dynents_scriptable{};
		std::size_t dynents_unflagged{};
		std::size_t dynents_left{};
		std::map<int, std::size_t> dynent_types;
		for (auto list = 0; list < 2; list++)
		{
			for (auto i = 0u; map_ents->dynEntDefList[list] && i < map_ents->dynEntCount[list]; i++)
			{
				const auto& dynent = map_ents->dynEntDefList[list][i];
				dynent_types[static_cast<int>(dynent.type)]++;
				auto model = source.resolve(iw7::ASSET_TYPE_XMODEL, dynent.baseModel);
				const auto& pose = dynent.initialPose;
				const auto finite = std::ranges::all_of(pose.quat, [](const float v) { return std::isfinite(v); }) &&
					std::ranges::all_of(pose.origin, [](const float v) { return std::isfinite(v); });
				const double length = std::sqrt(pose.quat[0] * pose.quat[0] + pose.quat[1] * pose.quat[1] + pose.quat[2] * pose.quat[2] +
					pose.quat[3] * pose.quat[3]);
				if (std::getenv("IW7_DYNENT_DUMP"))
				{
					const auto* poses = map_ents->dynEntPoseList[0][list];
					ZONETOOL_INFO("iw7 dynent %d/%u: type %d model %s brush %u active %d enabled %d physics %d transient %d at %s quat %g %g %g %g; "
						"pose list at %s (%u poses, radius %g) link %d",
						list, i, static_cast<int>(dynent.type), model ? iw7::get_asset_name(iw7::ASSET_TYPE_XMODEL, model) : "-", dynent.brushModel,
						dynent.spawnActive, dynent.spawnEnabled, !dynent.noPhysics, dynent.isTransient, vector_text(pose.origin).data(),
						pose.quat[0], pose.quat[1], pose.quat[2], pose.quat[3], poses ? vector_text(poses[i].pose.origin).data() : "-",
						poses ? poses[i].numPoses : 0u, poses ? poses[i].radius : 0.0f, dynent.linkTo ? dynent.linkTo->anchorIndex : -1);
				}

				if (dynent.type == iw7::DYNENT_TYPE_SCRIPTABLEINST)
				{
					dynents_scriptable++;
					continue;
				}

				if (!model)
				{
					dynents_brushes += dynent.brushModel != 0;
					dynents_left += dynent.brushModel == 0;
					continue;
				}

				// The spawn flags of x64-zt's unchecked layout read false on every clutter prop of Frontier (fire extinguishers,
				// trays, bottles on its tables): they do not decide what spawns.
				dynents_unflagged += !dynent.spawnActive && !dynent.spawnEnabled;
				if (!finite || !(length > 0.0))
				{
					dynents_left++;
					continue;
				}

				const auto x = pose.quat[0] / length, y = pose.quat[1] / length, z = pose.quat[2] / length, w = pose.quat[3] / length;
				const auto name = asset_name(iw7::get_asset_name(iw7::ASSET_TYPE_XMODEL, model));
				output.models.try_emplace(file_name(name), model);
				const auto clutter = (dynent.type == iw7::DYNENT_TYPE_CLUTTER || dynent.type == iw7::DYNENT_TYPE_CLUTTER_NOSHADOW) &&
					!dynent.noPhysics;
				if (clutter && !physics.contains(model))
				{
					physics.emplace(model, clutter_physics(source, *model));
				}

				if (const auto& body = clutter ? physics.at(model) : std::nullopt)
				{
					output.dynamic_models.push_back({file_name(name), {static_cast<float>(x), static_cast<float>(y), static_cast<float>(z),
						static_cast<float>(w)}, {pose.origin[0], pose.origin[1], pose.origin[2]}, *body});
					dynents_clutter++;
					continue;
				}

				const axes rotation{{{1 - 2 * (y * y + z * z), 2 * (x * y + w * z), 2 * (x * z - w * y)},
					{2 * (x * y - w * z), 1 - 2 * (x * x + z * z), 2 * (y * z + w * x)},
					{2 * (x * z + w * y), 2 * (y * z - w * x), 1 - 2 * (x * x + y * y)}}};
				const auto angles = axes_to_angles(rotation);
				entities += "{\n\"classname\" \"script_model\"\n";
				entities += std::format("\"model\" \"{}\"\n", name);
				entities += std::format("\"origin\" \"{}\"\n", vector_text(pose.origin));
				entities += std::format("\"angles\" \"{}\"\n", vector_text(angles.data()));
				entities += "}\n";
				dynents_placed++;
			}
		}

		std::string dynent_kinds;
		for (const auto& [type, count] : dynent_types)
		{
			dynent_kinds += std::format("{}type {}: {}", dynent_kinds.empty() ? "" : ", ", type, count);
		}

		ZONETOOL_INFO("iw7: of %u dynamic entities (%u and %u in the lists; %s), %zu are clutter and %zu script models; %zu of scriptables "
			"placed above, %zu brush ones keep their static collision, %zu without a model or pose left out (%zu read as not spawning)",
			map_ents->dynEntCountTotal, map_ents->dynEntCount[0], map_ents->dynEntCount[1], dynent_kinds.data(), dynents_clutter, dynents_placed,
			dynents_scriptable, dynents_brushes, dynents_left, dynents_unflagged);

		// IW7's breakable glass: an IW3 glass entity for each pane.
		const auto panes = glass_panes(source);
		for (auto i = 0u; i < panes.size(); i++)
		{
			entities += std::format("{{\n\"classname\" \"script_brushmodel\"\n\"model\" \"*{}\"\n}}\n", glass_brush_model(source, i));
		}

		entities = with_converting_classes(entities);
		for (auto match = std::sregex_iterator(entities.begin(), entities.end(), model_key); match != std::sregex_iterator(); ++match)
		{
			output.entity_models.insert(file_name((*match)[1].str()));
		}

		output.out_of_bounds = entities.find("\"targetname\" \"OutOfBounds\"") != std::string::npos;

		const auto path = output.extracted / (bsp_name(source) + ".ents");
		std::filesystem::create_directories(path.parent_path());
		std::ofstream(path, std::ios::binary | std::ios::trunc).write(entities.data(), static_cast<std::streamsize>(entities.size()));
		ZONETOOL_INFO("iw7: exported %zu bytes of entities with %u spawn points", entities.size(), spawns.spawnsCount);
	}
}
