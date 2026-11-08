#include <std_include.hpp>
#include "export.hpp"
#include <core/assets/rumble_data.hpp>

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
		// IW7's brush physics-asset key (0xCAF9) names dummy physics assets. The package defines the inline assets
		// scriptbrushmodeldummydefault and triggermodelstaticdummydefault (map_zone.h emitPhysicsAssetReference), and
		// stock mp_shipment defines triggermodeldummydefault, scriptbrushmodeldummydefault and triggermodelstaticdummydefault.
		// ScriptBrushModelDummyFixed and ScriptBrushModelDummy_MPAirdropCrate are in no checked stock zone (mp_shipment,
		// common, common_mp, global, global_mp, global_stream_mp), and triggermodeldummydefault is in none but mp_shipment.
		// Trigger physics comes from the inline trigger model (CM_GetTriggerModel +16, G_Utils_GetPhysicsAsset), which the
		// package always gives triggermodelstaticdummydefault, so a trigger keeps the name it had before 0xCAF9 was named.
		// A script brush takes the dummy its map zone already gives script brush models.
		std::string_view physasset_replacement(const std::string_view value)
		{
			static constexpr std::array<std::pair<std::string_view, std::string_view>, 3> replacements{{
				{"triggermodeldummydefault", "TriggerModelStaticDummyDefault"},
				{"ScriptBrushModelDummyFixed", "scriptbrushmodeldummydefault"},
				{"ScriptBrushModelDummy_MPAirdropCrate", "scriptbrushmodeldummydefault"},
			}};
			for (const auto& [name, replacement] : replacements)
			{
				if (value == name)
				{
					return replacement;
				}
			}
			return value;
		}

		// The suffix of a key line (` "value"`) with its physasset value replaced when Replay's zones lack the value.
		std::string physasset_suffix(const std::string_view suffix, std::size_t& replaced)
		{
			const auto open = suffix.find('"');
			const auto close = suffix.rfind('"');
			if (open == std::string_view::npos || close <= open)
			{
				return std::string(suffix);
			}

			const auto value = suffix.substr(open + 1, close - open - 1);
			const auto replacement = physasset_replacement(value);
			if (replacement == value)
			{
				return std::string(suffix);
			}

			++replaced;
			return std::string(suffix.substr(0, open + 1)) + std::string(replacement) + std::string(suffix.substr(close));
		}

		// IW7 entity strings name their keys by script string token ("157" is classname); the IW3 pipeline reads names.
		std::string with_key_names(const std::string& entities)
		{
			static const auto names = []
			{
				std::unordered_map<std::uint32_t, std::string_view> result;
				for (const auto& [id, name] : xsk::gsc::iw7::token_list)
				{
					if (*name)
					{
						result.try_emplace(id, name);
					}
				}

				// Keys that the token list has no text for, named by their use. 0x213 is the worldspawn's north yaw: 90 on
				// Afghan, Breakneck and Pixel, whose minimap art has +y up (their spawns land on its floor so), and absent
				// on Paris, whose art has +x up.
				result.try_emplace(0x213, "northyaw");
				// 0xCAF9 names the physics asset of a brush model or trigger: its values are dummy physics assets
				// (triggermodeldummydefault, scriptbrushmodeldummydefault, TriggerModelStaticDummyDefault, ...). Replay's
				// physasset key is 0x9333 (iw3KeyToIw8KeyId), which the IW3 route writes for triggers as well.
				result.try_emplace(0xCAF9, "physasset");
				return result;
			}();

			std::string result;
			result.reserve(entities.size() * 2);
			std::size_t start = 0;
			std::size_t physasset_replaced = 0;
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
					result += found != names.end() ? std::format("\"{}\"", found->second)
						: std::format("\"_id_{:X}\"", id);
					result += id == 0xCAF9 ? physasset_suffix(line.substr(digits), physasset_replaced)
						: std::string(line.substr(digits));
				}
				else
				{
					result += line;
				}

				result += '\n';
				start = end + 1;
			}

			if (physasset_replaced)
			{
				ZONETOOL_INFO("iw7: %zu physasset values name a dummy that the package does not define; they take its replacement",
					physasset_replaced);
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

		// A GSC-scripted light entity: a light class whose "pl#" lies in the source's scriptable range, with the index that
		// the light takes in the extraction (light_layout). Other light entities describe lights that ComWorld already holds.
		std::optional<std::string> script_light_block(const std::string& block, const std::string_view classname,
			const light_layout& layout)
		{
			if (!layout.scriptable_count || (classname != "light" && classname != "light_spot" && classname != "light_omni"))
			{
				return std::nullopt;
			}

			static const std::regex index_key(R"re("pl#" "(\d+)")re");
			std::smatch match;
			std::uint32_t index{};
			if (!std::regex_search(block, match, index_key) ||
				std::from_chars(block.data() + match.position(1), block.data() + match.position(1) + match.length(1), index).ec !=
				std::errc{} || index < layout.first_scriptable || index >= layout.first_scriptable + layout.scriptable_count)
			{
				return std::nullopt;
			}

			return block.substr(0, match.position(1)) + std::to_string(layout.index(index)) +
				block.substr(match.position(1) + match.length(1));
		}

		// Keeps the entities of the classes that the IW3 entity conversion takes: the others are IW7 features without a
		// Replay counterpart (Blitz spawns, 3D path nodes, client triggers). Light entities stay when they are GSC-scripted
		// lights ("pl#" in the scriptable range), as Replay spawns them (G_Spawn_ScriptPrimaryLight 0x140FC4A00: classes
		// light, light_spot and light_omni, key pl#; mp_neon's script_light_flicker), with their index in the layout.
		std::string with_converting_classes(const std::string& entities, const light_layout& layout)
		{
			std::size_t script_lights{};
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
				else if (const auto light = script_light_block(block, classname, layout))
				{
					result += *light;
					script_lights++;
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

			if (script_lights)
			{
				ZONETOOL_INFO("iw7: kept %zu GSC-scripted light entities", script_lights);
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
		// base poses are in model space). An empty tag uses the model origin; tag_origin is resolved like any other bone.
		// Effects that do not use tag angles keep the world identity orientation at that position.
		std::optional<extraction::scriptable_effect> tag_placement(const iw7::XModel* model, const std::string& tag, const float* origin,
			const float* angles, const bool use_tag_angles)
		{
			const auto instance = angles_to_axes(angles);
			std::array<double, 3> position{};
			axes rotation{{{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}}};
			if (!tag.empty())
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

				// Some source models have no tag_origin. Retain the origin fallback for that name only.
				if (!pose && tag != "tag_origin")
				{
					return {};
				}

				if (pose)
				{
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

			result.angles = use_tag_angles ? axes_to_angles(world) : std::array<float, 3>{};
			return result;
		}

		// The targetname of the brush models that stand for a scriptable collision map (collmap_scriptable): one per instance
		// that uses the map, which the instance's state machine claims by its origin (iw7_scriptable_collision).
		std::string collmap_target(const std::string_view collmap)
		{
			std::string name("mw120r_collmap_");
			for (const unsigned char c : collmap)
			{
				name += std::isalnum(c) ? static_cast<char>(std::tolower(c)) : '_';
			}

			return name;
		}

		// IW7's scriptable collision maps: each collmap_scriptable record names a map and owns one brush model, authored in the
		// space of the scriptable that uses it (one map serves every instance of a kind: Neon's hovden cars, intact, destroyed
		// and as an AI obstacle). Replay's own Scriptables resolve the same map to the clip map's brush model (collision event,
		// ScriptableSv_PrecacheAssetEventDef 0x1413108F8 -> CM_BrushModelIsValid).
		std::map<std::string, std::uint32_t> collmap_models(const std::string& entities)
		{
			std::map<std::string, std::uint32_t> result;
			static const std::regex block(R"re(\{([^{}]*)\})re");
			static const std::regex pair(R"re("([^"]+)"\s+"([^"]*)")re");
			for (auto it = std::sregex_iterator(entities.begin(), entities.end(), block); it != std::sregex_iterator(); ++it)
			{
				const auto text = (*it)[1].str();
				std::string classname, name, model;
				for (auto kv = std::sregex_iterator(text.begin(), text.end(), pair); kv != std::sregex_iterator(); ++kv)
				{
					const auto key = (*kv)[1].str();
					if (key == "classname")
					{
						classname = (*kv)[2].str();
					}
					else if (key == "targetname")
					{
						name = (*kv)[2].str();
					}
					else if (key == "model")
					{
						model = (*kv)[2].str();
					}
				}

				std::uint32_t index{};
				if (classname != "collmap_scriptable" || name.empty() || model.size() < 2 || model.front() != '*' ||
					std::from_chars(model.data() + 1, model.data() + model.size(), index).ptr != model.data() + model.size() || !index)
				{
					continue;
				}

				result.emplace(name, index);
			}

			return result;
		}

		// The collision maps that a scriptable's states turn on (Scriptable_EventType_Collision), and whether a part's first
		// state does (the collision that an instance has before any state changes). AI obstacle maps (aiObstacle) only block
		// IW7's bots; Replay's bot obstacles do not convert, so they are counted apart.
		struct scriptable_collisions
		{
			std::map<std::string, bool> collmaps;
			std::size_t ai_obstacles{};
		};

		scriptable_collisions collisions_of(const iw7::ScriptableDef& def)
		{
			scriptable_collisions result;
			const std::function<void(const iw7::ScriptableEventDef*, unsigned, bool)> events = [&](const iw7::ScriptableEventDef* list,
				const unsigned count, const bool initial)
			{
				for (auto e = 0u; list && e < count; e++)
				{
					const auto& event = list[e];
					if (event.type == iw7::Scriptable_EventType_Random)
					{
						events(event.data.random.eventsA, event.data.random.eventACount, initial);
						events(event.data.random.eventsB, event.data.random.eventBCount, initial);
					}
					else if (event.type == iw7::Scriptable_EventType_Collision && event.data.collision.collmapName &&
						*event.data.collision.collmapName)
					{
						if (event.data.collision.aiObstacle)
						{
							result.ai_obstacles++;
							continue;
						}

						auto& first = result.collmaps[event.data.collision.collmapName];
						first = first || initial;
					}
				}
			};

			const std::function<void(const iw7::ScriptablePartDef*, unsigned)> parts = [&](const iw7::ScriptablePartDef* list,
				const unsigned count)
			{
				for (auto p = 0u; list && p < count; p++)
				{
					const auto& part = list[p];
					for (auto s = 0u; part.states && s < part.numStates; s++)
					{
						events(part.states[s].base.events, part.states[s].base.numEvents, s == 0);
					}

					parts(part.childParts, part.numChildParts);
				}
			};

			parts(def.parts, def.numParts);
			return result;
		}

		// Two definitions whose names sanitize to one kind name would share a targetname: the second kind's shortcut was dropped
		// by try_emplace, and its instances broke with the first kind's model. The later kind takes _2, _3, ... unless its break
		// equals the kept one, which then shares it.
		extraction::scriptable_destructible unique_destructible(const extraction& output, const extraction::scriptable_destructible& kind)
		{
			auto candidate = kind;
			for (auto n = 2u;; n++)
			{
				const auto found = output.scriptable_destructibles.find(candidate.targetname);
				if (found == output.scriptable_destructibles.end() || found->second == candidate)
				{
					return candidate;
				}

				candidate.targetname = kind.targetname + "_" + std::to_string(n);
			}
		}

		// A scriptable that breaks: one part whose first state takes damage (a health state) and whose second and last state
		// swaps the model, plays effects or a sound. Other kinds (multi-stage, several parts, debris, collision maps that the
		// states change) run as state machines.
		std::optional<extraction::scriptable_destructible> destructible_of(const source_map& source, const iw7::ScriptableDef& def,
			extraction& output)
		{
			if (!def.parts || def.numParts != 1 || def.parts[0].numChildParts != 0 || !def.parts[0].states || def.parts[0].numStates != 2 ||
				!collisions_of(def).collmaps.empty())
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
			result.mindamage = healthy.data.health.minimumDamage;
			auto changes = false;
			// Replay's ScriptableCl_RunStateEventModel keeps one model per instance and every Model event overwrites it, and the
			// state machine's setmodel runs in event order. The state's last Model event therefore decides the broken model.
			const iw7::ScriptableEventDef* last_model{};
			for (auto e = 0u; broken.base.events && e < broken.base.numEvents; e++)
			{
				const auto& event = broken.base.events[e];
				// The shortcut takes only a model swap, effects and sounds. Every other event type (debris, explosions, shakes,
				// damage, state or part changes, waits, bone hides, forces, lights, suns, animations, script calls, rumbles,
				// chunks, collisions, the mayhem and view kinds) runs the break as a state machine (scriptable_machine), which
				// converts it or names the event it omits. The shortcut used to skip those types silently (default: break).
				switch (event.type)
				{
				case iw7::Scriptable_EventType_Model:
				case iw7::Scriptable_EventType_PFX:
				case iw7::Scriptable_EventType_Sound:
					break;
				default:
					return std::nullopt;
				}

				if (event.type == iw7::Scriptable_EventType_Model)
				{
					changes = true;
					last_model = &event;
				}
				else if (event.type == iw7::Scriptable_EventType_PFX)
				{
					const auto& fx = event.data.particleFX;
					const auto vfx = fx.effectDef.type == iw7::FX_COMBINED_VFX ? source.resolve(iw7::ASSET_TYPE_VFX, fx.effectDef.u.vfx) : nullptr;
					if (vfx)
					{
						changes = true;
						// Replay's PFX takes its tag from the event's tag list at random on every play (ScriptableCl_PlayParticleFX:
						// BG_irand over scrTagCount), so every tag is kept and the break picks one as Replay does. A looping effect
						// repeats at its loop rate while the piece stands (iw7_destructible_fx).
						extraction::scriptable_destructible::break_effect break_fx;
						break_fx.effect = std::string(asset_name(iw7::get_asset_name(iw7::ASSET_TYPE_VFX, vfx)));
						for (auto t = 0u; fx.scrTagNames && t < fx.scrTagCount; t++)
						{
							break_fx.tags.push_back(script_string(fx.scrTagNames[t]));
						}

						break_fx.use_tag_angles = fx.useTagAngles;
						break_fx.loop = fx.loopRate > 0.0f ? fx.loopRate : 0.0f;
						result.effects.push_back(std::move(break_fx));
					}
				}
				else if (event.type == iw7::Scriptable_EventType_Sound && event.data.sound.soundAlias)
				{
					result.sounds.push_back({std::string(asset_name(event.data.sound.soundAlias)),
						event.data.sound.tagName ? event.data.sound.tagName : ""});
				}
			}

			if (!changes)
			{
				return std::nullopt;
			}

			if (last_model)
			{
				const auto model = source.resolve(iw7::ASSET_TYPE_XMODEL, last_model->data.model.model);
				if (model)
				{
					result.dead_model = asset_name(iw7::get_asset_name(iw7::ASSET_TYPE_XMODEL, model));
					output.models.try_emplace(file_name(result.dead_model), model);
				}
				else if (last_model->data.model.model)
				{
					// A model pointer that the loaded zones do not hold: the piece still goes away, as an empty model does.
					ZONETOOL_WARNING("iw7: the broken model of %s is not in the loaded zones; its piece is removed", def.name ? def.name : "?");
				}
			}

			result.removes = last_model && result.dead_model.empty();
			// The hits that count: the health state's minimumDamage and validDamageCause (applied by iw7_destructible_cause).
			result.valid_causes = healthy.data.health.validDamageCause;

			std::string kind(def.name ? asset_name(def.name) : std::string_view{"scriptable"});
			std::ranges::transform(kind, kind.begin(), [](const unsigned char c)
			{
				return std::isalnum(c) ? static_cast<char>(std::tolower(c)) : '_';
			});
			result.targetname = "iw7_destructible_" + kind;
			return result;
		}

		// The playback of an IW7 Scriptable animation event that the converter carries: stateful, client-only in MP, with
		// no override, no pause on exit, no start time, rate 1 and no blend. Other settings are not played.
		bool default_scriptable_animation(const iw7::ScriptableEventAnimationDef& value)
		{
			return value.stateful && value.clientOnlyInMP && !value.overrideAnimation && !value.pauseOnExit &&
				value.startTimeMin == 0.0f && value.startTimeMax == 0.0f && !value.startTimeNormalized &&
				value.playbackRateMin == 1.0f && value.playbackRateMax == 1.0f && value.blendTime == 0.0f;
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
							auto& result = states.back();
							result.event = &event.data.animation;
							auto* current = result.event;
							auto* current_anim = anim;
							std::set<const iw7::ScriptableEventAnimationDef*> visited;
							const auto label = std::string(def.name ? def.name : "<unnamed>");
							while (current->eventAtEndCount)
							{
								// Replay's script-mover path below supports a sequential, non-looping animation
								// followed by one animation with the source's default playback settings.
								if (!visited.insert(current).second || current->eventAtEndCount != 1 || !current->eventsAtEnd ||
									(current_anim->flags & 1) || !current_anim->numframes || !std::isfinite(current_anim->framerate) ||
									current_anim->framerate <= 0.0f || !default_scriptable_animation(*current))
								{
									throw std::runtime_error("unsupported IW7 Scriptable animation end event: " + label);
								}
								const auto& end = current->eventsAtEnd[0];
								if (end.type != iw7::Scriptable_EventType_Animation || !end.data.animation.animation)
								{
									throw std::runtime_error("unsupported IW7 Scriptable animation end action: " + label);
								}
								current = &end.data.animation;
								current_anim = source.resolve(iw7::ASSET_TYPE_XANIMPARTS, current->animation);
								if (!current_anim || !current_anim->name || !default_scriptable_animation(*current))
								{
									throw std::runtime_error("unsupported IW7 Scriptable end animation: " + label);
								}
								result.on_end.push_back({current, current_anim});
							}
							// A root animation without an end chain is played by the plain path, which carries none of these settings;
							// the chain above checks its own events.
							if (!default_scriptable_animation(*result.event))
							{
								ZONETOOL_WARNING("iw7 scriptable %s: animation %s keeps the source's start time, rate, blend, pause, override or "
									"stateful settings, which the converter does not play", label.data(), std::string(asset_name(anim->name)).data());
							}
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
						// The raw field is read as a state index (its source layout is unverified, ledger d42f680): an index past the
						// part's states drops the change, which is logged so the loss is visible. Every change logs its raw value and the
						// reference's name, so a field that reads 0 everywhere (every change to state 0) shows in the conversion log.
						if (convert)
						{
							ZONETOOL_INFO("iw7 scriptable %s: state change of part %d reads %#zx as its target state (%u states, reference \"%s\")",
								def.name ? def.name : "<unnamed>", part, state, states,
								data.stateChange.partReference.base.name ? data.stateChange.partReference.base.name : "");
						}

						if (convert && state >= states)
						{
							ZONETOOL_WARNING("iw7 scriptable %s: a state change to %zx of part %d (%u states) is left out", def.name ? def.name : "<unnamed>",
								state, part, states);
						}
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
							std::string{}}, {"tags", tags}, {"loop", fx.loopRate}, {"kill", fx.killOnExit}, {"use_tag_angles", fx.useTagAngles},
							{"allow_missing_tag", fx.allowMissingTag}, {"root_on_missing_tag", fx.useRootOnMissingTag},
							{"oneshot_looping", fx.oneshotLooping}, {"viewmodel", fx.isViewmodel},
							{"nearest_vehicle_velocity", fx.allowNearestVehicleVelocity}});
						break;
					}
					case iw7::Scriptable_EventType_Sound:
						result.push_back({{"t", "sound"}, {"alias", data.sound.soundAlias ? std::string(asset_name(data.sound.soundAlias)) :
							std::string{}}, {"tag", data.sound.tagName ? data.sound.tagName : ""}, {"stateful", data.sound.stateful}});
						break;
					case iw7::Scriptable_EventType_HideShowBone:
						result.push_back({{"t", "hide"}, {"tag", data.hideShowBone.tagName ? data.hideShowBone.tagName : ""},
							{"hide", data.hideShowBone.hide}, {"children", data.hideShowBone.hideShowChildren},
							{"allow_missing_tag", data.hideShowBone.allowMissingTag}});
						break;
					case iw7::Scriptable_EventType_Explosion:
						result.push_back({{"t", "explosion"}, {"tag", data.explosion.tagName ? data.explosion.tagName : ""},
							{"radius", data.explosion.radius}, {"inner", data.explosion.dmgInner}, {"outer", data.explosion.dmgOuter},
							{"weapon", data.explosion.weapon != nullptr}, {"allow_missing_tag", data.explosion.allowMissingTag},
							{"root_on_missing_tag", data.explosion.useRootOnMissingTag}});
						break;
					case iw7::Scriptable_EventType_Screenshake:
						result.push_back({{"t", "shake"}, {"duration", data.screenshake.duration}, {"radius", data.screenshake.radius},
							{"scale", data.screenshake.scaleEarthquake}, {"earthquake", data.screenshake.isEarthquake},
							{"screen_shake", data.screenshake.isScreenShake}});
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
						if (convert && !name.empty() && anim && !default_scriptable_animation(data.animation))
						{
							ZONETOOL_WARNING("iw7 scriptable %s: animation %s keeps the source's start time, rate, blend, pause, override or "
								"stateful settings, which the converter does not play", def.name ? def.name : "<unnamed>", name.data());
						}
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
						const auto& damage = data.partDamage;
						// Byte 60 sits right after amount (IW7 structs.hpp ScriptableEventPartDamageDef: amount 56, unknown60 60, amountDoT 64).
						// Replay's ScriptableEventPartDamageDef (iw8 structs.hpp, static_asserts at 21507-21512) keeps the same field order with a
						// 4-byte partReference: amount 0xC, a bool destroy 0x10, amountDoT 0x14, intervalDoT 0x18. By layout, byte 60 is that
						// destroy flag, and the 1e9 damage below is this converter's emulation of it. The warning prints one line per set event;
						// grep the log per kind to count them.
						const auto destroy = damage.unknown60[0] != 0;
						if (convert && destroy)
						{
							ZONETOOL_WARNING("iw7 scriptable %s: a part-damage event sets byte 60, converted as destroy (1e9 damage); that meaning is unverified",
								def.name ? def.name : "<unnamed>");
						}
						result.push_back({{"t", "damage"}, {"part", reference_part(damage.partReference, self)},
							{"amount", destroy ? 1.0e9f : damage.amount}, {"dot", damage.amountDoT}, {"interval", damage.intervalDoT}});
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
							{"target", collmap_target(data.collision.collmapName ? data.collision.collmapName : "")},
							{"cmodel", data.collision.clipmapCModelIndex}, {"ai_obstacle", data.collision.aiObstacle},
							{"destroy", data.collision.destroyOnExit}});
						break;
					case iw7::Scriptable_EventType_Rumble:
					{
						// The rumble asset converts natively (export_rumbles, which follows the same asset pointer: delivered
						// says the converted asset exists); whether it plays per client or at a position follows its broadcast
						// flag.
						const auto* rumble = data.rumble.rumbleAsset ? source.resolve(iw7::ASSET_TYPE_RUMBLE, data.rumble.rumbleAsset) :
							nullptr;
						result.push_back({{"t", "rumble"}, {"rumble", rumble && rumble->name ? std::string(asset_name(rumble->name)) :
							std::string(data.rumble.rumble ? data.rumble.rumble : "")}, {"delivered", rumble && rumble->name},
							{"broadcast", rumble && rumble->broadcast}, {"tag", data.rumble.tagName ? data.rumble.tagName : ""},
							{"root_on_missing_tag", data.rumble.useRootOnMissingTag}, {"stateful", data.rumble.stateful}});
						break;
					}
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

			// The fields the machine does not carry (missing-tag rules, the explosion's weapon, viewmodel and vehicle-velocity
			// flags, the screen shake) are tallied per kind, so the conversion log shows how many events would play differently
			// from the source; random thresholds outside [0, 1] are tallied too. The machine itself is unchanged by them.
			if (convert)
			{
				std::map<std::string, std::size_t> dropped;
				const auto flag = [&](const char* field, const bool set)
				{
					if (set)
					{
						dropped[field]++;
					}
				};
				const std::function<void(const json&)> tally = [&](const json& fields)
				{
					for (const auto& event : fields)
					{
						const auto t = event.value("t", std::string{});
						if (t == "fx")
						{
							flag("PFX allow_missing_tag false", !event.value("allow_missing_tag", true));
							flag("PFX root_on_missing_tag true", event.value("root_on_missing_tag", false));
							flag("PFX oneshot_looping true", event.value("oneshot_looping", false));
							flag("PFX viewmodel true", event.value("viewmodel", false));
							flag("PFX nearest_vehicle_velocity true", event.value("nearest_vehicle_velocity", false));
						}
						else if (t == "hide")
						{
							flag("hide children true", event.value("children", false));
							flag("hide allow_missing_tag false", !event.value("allow_missing_tag", true));
						}
						else if (t == "explosion")
						{
							flag("explosion weapon set", event.value("weapon", false));
							flag("explosion allow_missing_tag false", !event.value("allow_missing_tag", true));
							flag("explosion root_on_missing_tag true", event.value("root_on_missing_tag", false));
						}
						else if (t == "shake")
						{
							flag("shake screen_shake true", event.value("screen_shake", false));
						}
						else if (t == "random")
						{
							// The threshold is read as a probability (export_effects: randomfloat(1) < threshold); the count checks the range.
							const auto threshold = event.value("threshold", 0.0f);
							flag("random threshold outside [0, 1]", !(threshold >= 0.0f && threshold <= 1.0f));
							tally(event.value("a", json::array()));
							tally(event.value("b", json::array()));
						}
					}
				};
				for (const auto& part : list)
				{
					for (const auto& state : part.at("states"))
					{
						tally(state.at("events"));
					}
				}

				for (const auto& [field, count] : dropped)
				{
					ZONETOOL_INFO("iw7 scriptable %s: %zu events set %s", def.name ? def.name : "<unnamed>",
						count, field.data());
				}
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
						auto placement = tag_placement(model, tag, context.initialOrigin, context.initialAngles, fx.useTagAngles);
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
		// With IW7's own exposure the map keeps IW7's units.
		if (iw7_exposure())
		{
			return 1.0f;
		}

		const auto com_world = source.com_world();
		if (!com_world || com_world->primaryLightCount <= sun_light)
		{
			return 1.0f;
		}

		const auto& sun = com_world->primaryLights[sun_light];
		const auto luminance = 0.2126f * sun.color[0] + 0.7152f * sun.color[1] + 0.0722f * sun.color[2];
		return light_type(sun.type) == 1 && luminance > 0.0f ? 1.0f / luminance : 1.0f;
	}

	light_layout light_layout_of(const source_map& source)
	{
		light_layout layout;
		const auto com_world = source.com_world();
		if (!com_world)
		{
			return layout;
		}

		layout.source_count = com_world->primaryLightCount;
		layout.first_scriptable = com_world->primaryLightCount;
		// IW7 keeps its scriptable lights last too (mp_neon: light 59 of 60); a range elsewhere stays static.
		if (com_world->scriptablePrimaryLightCount && com_world->firstScriptablePrimaryLight > sun_light &&
			com_world->firstScriptablePrimaryLight + com_world->scriptablePrimaryLightCount == com_world->primaryLightCount)
		{
			layout.first_scriptable = com_world->firstScriptablePrimaryLight;
			layout.scriptable_count = com_world->scriptablePrimaryLightCount;
			// The pipeline's sorted-light slots (convert.cpp sortedLightLimit).
			constexpr std::uint32_t sorted_light_slots = 256;
			layout.padding = layout.source_count < sorted_light_slots ? sorted_light_slots - layout.source_count : 0;
		}

		return layout;
	}

	void export_com_world(const source_map& source, extraction& output)
	{
		const auto com_world = source.com_world();
		if (!com_world)
		{
			throw std::runtime_error(std::format("zone \"{}\" holds no com world", source.name()));
		}

		// IW7 local lights are physical, as Replay's: a light of colour c lights a surface with c * N.L / (pi d^2)
		// inside its radius. Authored spot and omni intensities are used as they are: convert.cpp's calibrateLocalLights
		// skips them (only the lightUnitScale multiply applies), so no brightest-lightmap-texel calibration happens here.
		// The IW7-to-Replay unit of these lights is open (AGENTS.md 3.2 and the local-light unit ledger). Each takes
		// Replay's linear light definition.
		const auto scale = light_unit_scale(source);
		auto lights = json::array();
		std::size_t local_lights{};
		std::size_t narrowed_spots{};
		std::size_t leaning_ups{};
		std::size_t large_bulbs{};
		std::size_t dropped_directionals{};
		std::size_t dark_locals{};

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
		const auto layout = light_layout_of(source);
		for (auto i = 0u; i < com_world->primaryLightCount; i++)
		{
			// The pipeline's null slots go before the scriptable lights, which stay last (light_layout).
			if (layout.padding && i == layout.first_scriptable)
			{
				for (auto p = 0u; p < layout.padding; p++)
				{
					lights.push_back({{"type", 0}, {"can_use_shadow_map", false}, {"exponent", 0}, {"color", {0.0f, 0.0f, 0.0f}},
						{"direction", {0.0f, 0.0f, 0.0f}}, {"origin", {0.0f, 0.0f, 0.0f}}, {"radius", 0.0f},
						{"cos_half_fov_outer", 0.0f}, {"cos_half_fov_inner", 0.0f}, {"rotation_limit", 0.0f},
						{"translation_limit", 0.0f}, {"definition", ""}});
				}
			}

			const auto& light = com_world->primaryLights[i];
			auto type = light_type(light.type);
			std::array<float, 3> color{light.color[0], light.color[1], light.color[2]};
			const auto luminance = 0.2126f * color[0] + 0.7152f * color[1] + 0.0722f * color[2];
			std::optional<float> intensity;
			if (type == 1 && i != sun_light)
			{
				// One sun lights the world. Logged: the sun is the only directional light Replay takes, so every other one is
				// dropped (MEASURE: the count and luminances per map, grep "directional light").
				type = 0;
				dropped_directionals++;
				ZONETOOL_INFO("iw7: lights: directional light %u (luminance %g) is dropped; Replay has one sun", i, luminance);
			}
			else if (type == 1)
			{
				std::ranges::for_each(color, [&](float& channel) { channel *= scale; });
				ZONETOOL_INFO("iw7: lights: sun %u raw colour luminance %g (unit scale %g)", i, luminance, scale);
			}
			else if (type >= 2 && luminance > 0.0f && light.radius > 0.0f)
			{
				std::ranges::for_each(color, [&](float& channel) { channel /= luminance; });
				intensity = luminance / std::numbers::pi_v<float>;
				local_lights++;
			}
			else
			{
				// Zero-radius or black lights take no Replay light (MEASURE: count per map in the summary line).
				if (type >= 2)
				{
					dark_locals++;
				}
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

		// The light definitions that effect lights link (export_effects): those with an IES profile convert like the spots'
		// own, and the pipeline keeps them by name.
		std::size_t effect_ies{};
		for (const auto& name : output.particle_light_defs)
		{
			if (!ies_definition(name.data()).empty())
			{
				effect_ies++;
			}
		}

		if (!output.particle_light_defs.empty())
		{
			ZONETOOL_INFO("iw7: effect lights link %zu light definitions of their own, %zu with an IES profile",
				output.particle_light_defs.size(), effect_ies);
		}

		if (dropped_directionals || dark_locals)
		{
			ZONETOOL_INFO("iw7: lights: %zu directional lights other than the sun and %zu zero-radius or black local lights are dropped",
				dropped_directionals, dark_locals);
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

		json common_world =
		{
			{"schema", 1},
			{"name", bsp_name(source)},
			{"in_use", true},
			{"primary_lights", std::move(lights)},
			{"ies_profiles", std::move(ies_profiles)},
		};
		if (layout.scriptable_count)
		{
			common_world["scriptable_primary_light_count"] = layout.scriptable_count;
			common_world["first_scriptable_primary_light"] = layout.index(layout.first_scriptable);
			ZONETOOL_INFO("iw7: lights: %u GSC-scripted lights from %u, last after %u null slots", layout.scriptable_count,
				layout.index(layout.first_scriptable), layout.padding);
		}

		write_json(output.extracted / (bsp_name(source) + ".replay-comworld.json"), common_world);

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
		// The entity being read: its class and name, and the models taken from it. Each removal is logged when its block
		// closes, since the name lines may follow the model line.
		std::string classname;
		std::string targetname;
		std::vector<std::string> removed_models;
		std::size_t start = 0;
		while (start < entities.size())
		{
			const auto end = std::min(entities.find('\n', start), entities.size());
			const std::string_view line(entities.data() + start, end - start);
			constexpr std::string_view key = "\"model\" \"";
			const auto name = line.starts_with(key) && line.size() > key.size() + 1 && line[key.size()] != '*'
				? line.substr(key.size(), line.size() - key.size() - 1) : std::string_view{};
			const auto value_of = [&](const std::string_view prefix) -> std::string
			{
				return line.starts_with(prefix) && line.size() > prefix.size() + 1
					? std::string(line.substr(prefix.size(), line.size() - prefix.size() - 1)) : std::string{};
			};

			if (line == "{")
			{
				classname.clear();
				targetname.clear();
				removed_models.clear();
			}
			else if (auto read_classname = value_of("\"classname\" \""); !read_classname.empty())
			{
				classname = std::move(read_classname);
			}
			else if (auto read_targetname = value_of("\"targetname\" \""); !read_targetname.empty())
			{
				targetname = std::move(read_targetname);
			}

			if (!name.empty() && !output.exported_models.contains(file_name(name)))
			{
				removed++;
				removed_models.emplace_back(name);
			}
			else
			{
				result.append(line);
				result += '\n';
			}

			if (line == "}")
			{
				for (const auto& model : removed_models)
				{
					ZONETOOL_WARNING("iw7: entity \"%s\" (class \"%s\") loses its model \"%s\": the model is not in the loaded zones",
						targetname.empty() ? "(no targetname)" : targetname.c_str(), classname.c_str(), model.c_str());
				}

				removed_models.clear();
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
		// between zones of equal priority, the one whose box lies inside the other's (CG_TriggerUpdateForCameraBounds 0x1416F1B90, verified-static), so
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
					// The vision set of a vision trigger (role 0x1): IW7 names it by an offset into triggerString. Replay reads
					// vision/<name>.vision (CG_StartClientSideTriggersVisionTrigger); the package carries only the map's own.
					{"vision", (type & 0x1) ? trigger_string(triggers, value16(triggers.visionSetTriggers)) : std::string{}},
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

			// The vision sets that the vision triggers name. The vision role is cleared (kept as role 0x1 only when the package
			// ships the set), so these triggers keep the map's vision: the names are logged for the owner's decision.
			std::map<std::string, std::size_t> vision_sets;
			for (const auto& item : items)
			{
				if (const auto& name = item["vision"].get_ref<const std::string&>(); !name.empty())
				{
					vision_sets[name]++;
				}
			}

			for (const auto& [name, triggers_named] : vision_sets)
			{
				ZONETOOL_WARNING("iw7: %zu client triggers name the vision set \"%s\"; the package ships only the map's vision, so they keep it",
					triggers_named, name.c_str());
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

	namespace
	{
		// The mass of a model's bodies as its IW7 PhysicsAsset keeps them (each motion's inverse mass); nothing for a model
		// without one, or with a body that does not move or does not read.
		std::optional<float> physics_asset_mass(const source_map& source, const iw7::XModel& model)
		{
			const auto* resolved = source.resolve(iw7::ASSET_TYPE_XMODEL, const_cast<iw7::XModel*>(&model));
			const auto* asset = resolved && resolved->physicsAsset
				? source.resolve(iw7::ASSET_TYPE_PHYSICSASSET, resolved->physicsAsset) : nullptr;
			if (!asset || !asset->havokData || !asset->havokDataSize)
			{
				return std::nullopt;
			}

			try
			{
				const auto system = read_havok_system(asset->havokData, asset->havokDataSize);
				float mass{};
				for (const auto& body : system.bodies)
				{
					if (!(body.mass > 0.0f) || !std::isfinite(body.mass))
					{
						return std::nullopt;
					}

					mass += body.mass;
				}

				return system.bodies.empty() ? std::nullopt : std::optional(mass);
			}
			catch (const std::exception&)
			{
				return std::nullopt;
			}
		}
	}

	// The mass of IW7 clutter is that of the bodies of its PhysicsAsset. A model without one takes the mass of the convex
	// hull of its first LOD at the density that gives Replay's own clutter its mass (stock buckets: 1.5 kg in 0.054 cubic
	// Havok units, 28 kg per cubic Havok unit, a Havok unit being 32 game units). Both stay within Replay's clutter range.
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
		const auto mass = physics_asset_mass(source, model).value_or(volume * density);
		const auto surface = std::ranges::max_element(areas, {}, [](const auto& entry) { return entry.second; });
		return clutter_body{std::clamp(mass, 0.1f, 200.0f), surface != areas.end() ? std::string(surface->first) : "default"};
	}

	std::string write_physics_preset(const extraction& output, const std::string& model, const clutter_body& body)
	{
		const auto name = "iw7_" + model;
		std::filesystem::create_directories(output.extracted / "physic");
		std::ofstream(output.extracted / "physic" / name, std::ios::binary | std::ios::trunc)
			<< std::format("PHYSIC\\mass\\{:.3f}\\sndAliasPrefix\\{}", body.mass, body.surface);
		return name;
	}

	std::optional<std::string> write_physics_system(const source_map& source, const extraction& output, const std::string& model)
	{
		const auto found = output.models.find(model);
		const auto* resolved = found != output.models.end() && found->second
			? source.resolve(iw7::ASSET_TYPE_XMODEL, const_cast<iw7::XModel*>(found->second)) : nullptr;
		const auto* asset = resolved && resolved->physicsAsset
			? source.resolve(iw7::ASSET_TYPE_PHYSICSASSET, resolved->physicsAsset) : nullptr;
		if (!asset || asset->numRigidBodies < 2 || !asset->havokData || !asset->havokDataSize)
		{
			return std::nullopt;
		}

		havok_system system;
		try
		{
			system = read_havok_system(asset->havokData, asset->havokDataSize);
		}
		catch (const std::exception& e)
		{
			ZONETOOL_WARNING("iw7: the physics system of clutter model %s does not read (%s); it moves as one body", model.data(), e.what());
			return std::nullopt;
		}

		const auto finite = [](const auto& values) { return std::ranges::all_of(values, [](const float v) { return std::isfinite(v); }); };
		for (const auto& body : system.bodies)
		{
			if (!finite(body.position) || !finite(body.orientation) || !std::isfinite(body.mass) || body.pieces.empty() ||
				std::ranges::any_of(body.pieces, [&](const havok_piece& piece) { return piece.points.empty() || !std::isfinite(piece.convex_radius) ||
					!std::ranges::all_of(piece.points, finite); }))
			{
				system.unsupported.push_back(std::format("body {} has no shape or values that do not convert", body.name));
			}
			else if (!(body.mass > 0.0f))
			{
				// A fixed body (IW7's inverse mass 0): the system writer makes moving bodies only.
				system.unsupported.push_back(std::format("body {} does not move", body.name));
			}
		}

		if (system.bodies.size() < 2 || !system.unsupported.empty())
		{
			std::string reasons;
			for (const auto& reason : system.unsupported)
			{
				reasons += (reasons.empty() ? "" : "; ") + reason;
			}

			ZONETOOL_WARNING("iw7: the physics system of clutter model %s does not convert (%zu bodies%s%s); it moves as one body",
				model.data(), system.bodies.size(), reasons.empty() ? "" : ": ", reasons.data());
			return std::nullopt;
		}

		const auto frame_json = [&](const havok_frame& frame)
		{
			return json{{"rotation", {vector_json(frame.rotation[0].data(), 3), vector_json(frame.rotation[1].data(), 3),
				vector_json(frame.rotation[2].data(), 3)}}, {"translation", vector_json(frame.translation.data(), 3)}};
		};

		auto bodies = json::array();
		for (const auto& body : system.bodies)
		{
			auto pieces = json::array();
			for (const auto& piece : body.pieces)
			{
				auto points = json::array();
				for (const auto& point : piece.points)
				{
					points.push_back(vector_json(point.data(), 3));
				}

				pieces.push_back({{"points", std::move(points)}, {"convex_radius", piece.convex_radius}});
			}

			bodies.push_back({{"name", body.name}, {"position", vector_json(body.position.data(), 3)},
				{"orientation", vector_json(body.orientation.data(), 4)}, {"mass", body.mass}, {"pieces", std::move(pieces)}});
		}

		auto hinges = json::array();
		for (const auto& hinge : system.hinges)
		{
			auto entry = json{{"name", hinge.name}, {"body_a", hinge.body_a}, {"body_b", *hinge.body_b}, {"limited", hinge.limited},
				{"frame_a", frame_json(hinge.frame_a)}, {"frame_b", frame_json(hinge.frame_b)},
				{"stabilization", {{"enabled", hinge.stabilization}, {"max_linear_impulse", hinge.max_linear_impulse},
					{"max_angular_impulse", hinge.max_angular_impulse}, {"max_angle", hinge.max_angle}}},
				{"free_axis", hinge.free_axis},
				{"ball_socket", {{"solving_method", hinge.solving_method}, {"bodies_to_notify", hinge.bodies_to_notify},
					{"velocity_stabilization", hinge.velocity_stabilization}, {"linear_impulse_limit", hinge.linear_impulse_limit},
					{"breach_impulse", hinge.breach_impulse}, {"inertia_stabilization", hinge.inertia_stabilization}}}};
			if (hinge.limited)
			{
				entry["friction"] = {{"enabled", hinge.friction}, {"first_axis", hinge.friction_axis}, {"axis_count", hinge.friction_axes},
					{"max_torque", hinge.max_friction_torque}};
				entry["limit"] = {{"enabled", hinge.limit}, {"axis", hinge.limit_axis}, {"cosine_axis", hinge.cosine_axis},
					{"min_angle", hinge.min_angle}, {"max_angle", hinge.max_limit_angle}, {"tau", hinge.limit_tau}, {"damping", hinge.limit_damping}};
			}

			hinges.push_back(std::move(entry));
		}

		const auto name = "iw7_" + model + ".system.json";
		std::filesystem::create_directories(output.extracted / "physic");
		write_json(output.extracted / "physic" / name, {{"schema", 1}, {"asset", asset->name ? asset->name : ""},
			{"bodies", std::move(bodies)}, {"hinges", std::move(hinges)}});
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
		// Two definitions whose names sanitize to one kind name must not share a machine or an animation: the contains/try_emplace
		// on that name kept the first definition's entry, so the second kind's instances ran the first kind's machine or played its
		// animation. Each definition keeps the first free name of its prefix (iw7_scriptable_<kind>, then _2, _3, ...).
		std::map<std::pair<std::string, const iw7::ScriptableDef*>, std::string> kind_names;
		std::set<std::string> taken_names;
		const auto name_of = [&](const std::string& prefix, const iw7::ScriptableDef* definition, const std::string& kind)
		{
			const auto key = std::pair{prefix, definition};
			if (const auto found = kind_names.find(key); found != kind_names.end())
			{
				return found->second;
			}

			auto name = prefix + kind;
			for (auto n = 2u; taken_names.contains(name); n++)
			{
				name = prefix + kind + "_" + std::to_string(n);
			}

			taken_names.insert(name);
			kind_names.emplace(key, name);
			return name;
		};
		std::size_t placed{};
		std::size_t destructibles{};
		// Scriptables the placement leaves out, by definition name: game-mode chevrons (hardpoint) and kinds without a model or pose.
		std::map<std::string, std::size_t> hardpoints_left;
		std::map<std::string, std::size_t> modelless_left;
		const auto collmaps = collmap_models(entities);
		std::size_t collmap_brushes{};
		std::size_t collmap_static{};
		std::size_t collmap_missing{};
		std::size_t collmap_obstacles{};
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
			const auto hardpoint = def_name.find("hardpoint") != std::string_view::npos;
			if (name.empty() || !finite || hardpoint)
			{
				if (hardpoint)
					hardpoints_left[std::string(def_name)]++;
				else
					modelless_left[std::string(def_name)]++;
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
					const auto machine_name = name_of("iw7_scriptable_", def, kind);
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
				const auto unique = unique_destructible(output, *destructible);
				entities += std::format("\"targetname\" \"{}\"\n", unique.targetname);
				const auto added = output.scriptable_destructibles.try_emplace(unique.targetname, unique).second;
				destructibles++;
				// Reported once per kind (the shortcut is found once per instance): the hit filters of the kinds that have them.
				if (added && unique.mindamage > 0)
				{
					ZONETOOL_INFO("iw7: break kind %s counts only hits of %d or more (its health state's minimumDamage)", unique.targetname.data(),
						unique.mindamage);
				}

				if (added && unique.valid_causes != 0)
				{
					ZONETOOL_INFO("iw7: break kind %s counts only the damage causes %d (Scriptable_DamageCause)", unique.targetname.data(),
						unique.valid_causes);
				}
			}
			else if (animation)
			{
				animated = name_of("iw7_animated_", def, kind);
				entities += std::format("\"targetname\" \"{}\"\n", animated);
			}
			// A kind with several states that change (IW7 runs a state's events on every instance, named or not) runs its machine
			// as a kind that takes damage does; the machine plays its first states' effects (iw7_scriptable_think).
			else if (def && (takes_damage(*def) || changes_state(*def)))
			{
				const auto machine_name = name_of("iw7_scriptable_", def, kind);
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

			// The collision maps of the instance's states: a brush model of each at the instance's placement. A state machine
			// turns them on and off as IW7's collision events do (they start off); an instance that never changes state keeps
			// the maps of its parts' first states.
			if (def)
			{
				const auto collisions = collisions_of(*def);
				collmap_obstacles += collisions.ai_obstacles;
				for (const auto& [collmap, initial] : collisions.collmaps)
				{
					const auto brush = collmaps.find(collmap);
					if (brush == collmaps.end())
					{
						collmap_missing++;
						continue;
					}

					if (!machine && !initial)
					{
						continue;
					}

					entities += "{\n\"classname\" \"script_brushmodel\"\n";
					entities += std::format("\"model\" \"*{}\"\n", brush->second);
					entities += std::format("\"origin\" \"{}\"\n", vector_text(context.initialOrigin));
					entities += std::format("\"angles\" \"{}\"\n", vector_text(context.initialAngles));
					if (machine)
					{
						entities += std::format("\"targetname\" \"{}\"\n", collmap_target(collmap));
						collmap_brushes++;
					}
					else
					{
						collmap_static++;
					}

					entities += "}\n";
				}
			}
			// The state machines play the effects of their first states themselves (as effect entities, iw7_scriptable_fx).
			if (def && !machine)
			{
				add_scriptable_effects(source, *def, model, context, output);
			}
		}

		ZONETOOL_INFO("iw7: placed %zu of %u scriptables (%u runtime, %u reserved) as script models, with %zu effects of their first states",
			placed, scriptables.totalInstanceCount, scriptables.runtimeInstanceCount, scriptables.reservedInstanceCount,
			output.scriptable_effects.size());
		for (const auto& [kind, count] : hardpoints_left)
			ZONETOOL_INFO("iw7: %zu scriptables of kind %s left out by their hardpoint name (game-mode zone chevrons)", count, kind.data());
		for (const auto& [kind, count] : modelless_left)
			ZONETOOL_WARNING("iw7: %zu scriptables of kind %s have no model or pose and are left out: their state machines and first-state "
				"effects do not run", count, kind.data());
		ZONETOOL_INFO("iw7: %zu of them break (%zu kinds: a health state, then the broken model, its effects and sound)", destructibles,
			output.scriptable_destructibles.size());
		if (!collmaps.empty() || collmap_missing || collmap_obstacles)
		{
			ZONETOOL_INFO("iw7: scriptable collision maps: %zu defined, %zu brush models that state machines switch, %zu that stay "
				"solid, %zu references to maps the map lacks, %zu AI obstacle maps left out (Replay's bot obstacles do not convert)",
				collmaps.size(), collmap_brushes, collmap_static, collmap_missing, collmap_obstacles);
		}
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

		entities = with_converting_classes(entities, light_layout_of(source));
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

	std::string converted_rumble_name(const source_map& source, const std::string_view original, const bool graph)
	{
		if (original.empty())
		{
			throw std::runtime_error("IW7 rumble asset has no name");
		}
		return std::format("mw120r/iw7/{}/{}/{}", source.name(), graph ? "rumblegraph" : "rumble", original);
	}

	void export_rumbles(const source_map& source, const std::filesystem::path& extracted)
	{
		const auto map = source.map_ents();
		if (!map)
		{
			throw std::runtime_error("IW7 map has no MapEnts for Scriptable asset closure");
		}
		using asset_data::rumble_graph;
		using asset_data::rumble_info;
		std::map<std::string, rumble_info> infos;
		std::map<std::string, rumble_graph> graphs;
		std::set<const iw7::RumbleInfo*> info_headers;
		std::set<const iw7::RumbleGraph*> graph_headers;
		ordered_json evidence{{"profile", "iw7"},
							  {"snapshotScope", "decoded postload headers, not original packed source records"},
							  {"unsupportedBySource", {"leftTrigger", "rightTrigger", "swapTriggerRumblesOnLefty", "panTriggerRumbles"}},
							  {"assets", ordered_json::array()}};
		// Common Replay rumbles use these source names with different curves and trigger channels.
		const auto semantic_name = [&](const iw7::XAssetType type, const char* name)
		{
			return converted_rumble_name(source, asset_name(name), type != iw7::ASSET_TYPE_RUMBLE);
		};
		const auto bytes = [](const void* header, const std::size_t count)
		{
			constexpr char digits[] = "0123456789abcdef";
			const auto data = static_cast<const unsigned char*>(header);
			std::string result;
			result.reserve(count * 2);
			for (std::size_t i = 0; i < count; ++i)
			{
				result += digits[data[i] >> 4];
				result += digits[data[i] & 15];
			}
			return result;
		};
		const auto record = [&](const iw7::XAssetType type, const void* header, const std::size_t size, const std::size_t padding)
		{
			evidence["assets"].push_back(
				{{"sourceType", static_cast<int>(type)},
				 {"sourceName", asset_name(iw7::get_asset_name(type, header))},
				 {"semanticName", semantic_name(type, iw7::get_asset_name(type, header))},
				 {"ownerZone", source.asset_zone(type, header)},
				 {"rootSize", size},
				 {"decodedHeaderHex", bytes(header, size)},
				 {"alignmentPadding",
				  {{"offset", padding}, {"hex", bytes(static_cast<const unsigned char*>(header) + padding, size - padding)}}}});
		};
		static_assert(sizeof(iw7::RumbleInfo) == 40 && offsetof(iw7::RumbleInfo, broadcast) == 33);
		static_assert(sizeof(iw7::RumbleGraph) == 144 && offsetof(iw7::RumbleGraph, knotCount) == 136);
		const auto graph = [&](iw7::RumbleGraph* pointer) -> std::optional<std::string>
		{
			if (!pointer)
			{
				return std::nullopt;
			}
			const auto native = source.resolve(iw7::ASSET_TYPE_RUMBLE_GRAPH, pointer);
			if (!native)
			{
				throw std::runtime_error("IW7 RumbleGraph dependency is unresolved");
			}
			rumble_graph value;
			value.name = semantic_name(iw7::ASSET_TYPE_RUMBLE_GRAPH, native->name);
			if (!graph_headers.insert(native).second)
			{
				return value.name;
			}
			record(iw7::ASSET_TYPE_RUMBLE_GRAPH, native, sizeof(*native), 138);
			value.knot_count = native->knotCount;
			for (std::size_t i = 0; i < 16; ++i)
			{
				value.x[i] = native->knots[i][0];
				value.y[i] = native->knots[i][1];
			}
			const auto [found, added] = graphs.try_emplace(value.name, value);
			if (!added)
			{
				bool equal = found->second.knot_count == value.knot_count;
				for (std::size_t i = 0; i < 16; ++i)
				{
					equal &= std::bit_cast<std::uint32_t>(found->second.x[i]) == std::bit_cast<std::uint32_t>(value.x[i]) &&
							 std::bit_cast<std::uint32_t>(found->second.y[i]) == std::bit_cast<std::uint32_t>(value.y[i]);
				}
				if (!equal)
				{
					throw std::runtime_error("conflicting IW7 RumbleGraph definitions: " + value.name);
				}
			}
			return value.name;
		};
		const auto info = [&](iw7::RumbleInfo* pointer)
		{
			if (!pointer)
			{
				return;
			}
			const auto native = source.resolve(iw7::ASSET_TYPE_RUMBLE, pointer);
			if (!native)
			{
				throw std::runtime_error("IW7 RumbleInfo dependency is unresolved");
			}
			if (!info_headers.insert(native).second)
			{
				return;
			}
			record(iw7::ASSET_TYPE_RUMBLE, native, sizeof(*native), 34);
			rumble_info value;
			value.name = semantic_name(iw7::ASSET_TYPE_RUMBLE, native->name);
			value.duration_milliseconds = native->duration;
			value.range = native->range;
			value.high = graph(native->highRumbleGraph);
			value.low = graph(native->lowRumbleGraph);
			value.fade_with_distance = native->fadeWithDistance;
			value.broadcast = native->broadcast;
			const auto [found, added] = infos.try_emplace(value.name, value);
			if (!added && (found->second.duration_milliseconds != value.duration_milliseconds ||
						   std::bit_cast<std::uint32_t>(found->second.range) != std::bit_cast<std::uint32_t>(value.range) ||
						   found->second.high != value.high || found->second.low != value.low ||
						   found->second.fade_with_distance != value.fade_with_distance || found->second.broadcast != value.broadcast))
			{
				throw std::runtime_error("conflicting IW7 RumbleInfo definitions: " + value.name);
			}
		};
		std::set<const iw7::ScriptableEventDef*> seen_events, active_events;
		std::size_t rumble_events{};
		const auto events = [&](auto&& self, const iw7::ScriptableEventDef* list, const unsigned count) -> void
		{
			if (count && !list)
			{
				throw std::runtime_error("IW7 Scriptable event count has no array");
			}
			for (unsigned i = 0; i < count; ++i)
			{
				const auto& event = list[i];
				if (active_events.contains(&event))
				{
					throw std::runtime_error("cyclic IW7 Scriptable event arrays");
				}
				if (!seen_events.insert(&event).second)
				{
					continue;
				}
				active_events.insert(&event);
				switch (event.type)
				{
				case iw7::Scriptable_EventType_Rumble:
					++rumble_events;
					info(event.data.rumble.rumbleAsset);
					break;
				case iw7::Scriptable_EventType_Random:
					self(self, event.data.random.eventsA, event.data.random.eventACount);
					self(self, event.data.random.eventsB, event.data.random.eventBCount);
					break;
				case iw7::Scriptable_EventType_Animation:
					self(self, event.data.animation.eventsAtEnd, event.data.animation.eventAtEndCount);
					break;
				case iw7::Scriptable_EventType_ClientViewSelector:
					self(self, event.data.clientViewSelector.events1p, event.data.clientViewSelector.event1pCount);
					self(self, event.data.clientViewSelector.events3p, event.data.clientViewSelector.event3pCount);
					break;
				case iw7::Scriptable_EventType_TeamSelector:
					self(self, event.data.teamSelector.eventsPass, event.data.teamSelector.eventPassCount);
					self(self, event.data.teamSelector.eventsFail, event.data.teamSelector.eventFailCount);
					break;
				case iw7::Scriptable_EventType_NoteTrack:
					if (event.data.noteTrack.noteTrackCount && !event.data.noteTrack.noteTracks)
					{
						throw std::runtime_error("IW7 Scriptable notetrack count has no array");
					}
					for (unsigned n = 0; n < event.data.noteTrack.noteTrackCount; ++n)
					{
						self(self, event.data.noteTrack.noteTracks[n].events, event.data.noteTrack.noteTracks[n].numEvents);
					}
					break;
				default:
					break;
				}
				active_events.erase(&event);
			}
		};
		std::set<const iw7::ScriptablePartDef*> seen_parts, active_parts;
		const auto part = [&](auto&& self, const iw7::ScriptablePartDef& value) -> void
		{
			if (active_parts.contains(&value))
			{
				throw std::runtime_error("cyclic IW7 Scriptable part arrays");
			}
			if (!seen_parts.insert(&value).second)
			{
				return;
			}
			active_parts.insert(&value);
			if (value.numStates && !value.states)
			{
				throw std::runtime_error("IW7 Scriptable state count has no array");
			}
			for (unsigned s = 0; s < value.numStates; ++s)
			{
				const auto& state = value.states[s];
				events(events, state.base.events, state.base.numEvents);
				const iw7::ScriptableStateBaseDef* base{};
				switch (state.type)
				{
				case iw7::Scriptable_StateType_Simple:
					base = state.data.simple.base;
					break;
				case iw7::Scriptable_StateType_Health:
					base = state.data.health.base;
					break;
				case iw7::Scriptable_StateType_Scripted:
					base = state.data.scripted.base;
					break;
				default:
					throw std::runtime_error("unknown IW7 Scriptable state type in rumble closure");
				}
				if (base)
				{
					events(events, base->events, base->numEvents);
				}
			}
			if (value.numChildParts && !value.childParts)
			{
				throw std::runtime_error("IW7 Scriptable child part count has no array");
			}
			for (unsigned c = 0; c < value.numChildParts; ++c)
			{
				self(self, value.childParts[c]);
			}
			active_parts.erase(&value);
		};
		std::set<const iw7::ScriptableDef*> definitions;
		const auto definition = [&](auto&& self, iw7::ScriptableDef* pointer) -> void
		{
			if (!pointer)
			{
				return;
			}
			const auto def = source.resolve(iw7::ASSET_TYPE_SCRIPTABLE, pointer);
			if (!def)
			{
				throw std::runtime_error("IW7 placed Scriptable definition is unresolved");
			}
			if (!definitions.insert(def).second)
			{
				return;
			}
			(void)source.asset_zone(iw7::ASSET_TYPE_SCRIPTABLE, def);
			if (def->numParts && !def->parts)
			{
				throw std::runtime_error("IW7 Scriptable part count has no array");
			}
			for (unsigned p = 0; p < def->numParts; ++p)
			{
				part(part, def->parts[p]);
			}
			self(self, def->nextScriptableDef);
		};
		std::set<const iw7::ScriptableDef*> placed_definitions;
		const auto root = [&](iw7::ScriptableDef* pointer)
		{
			if (!pointer)
			{
				return;
			}
			const auto def = source.resolve(iw7::ASSET_TYPE_SCRIPTABLE, pointer);
			if (!def)
			{
				throw std::runtime_error("IW7 placed Scriptable definition is unresolved");
			}
			placed_definitions.insert(def);
			definition(definition, def);
		};
		const auto& placed = map->scriptableMapEnts;
		if (placed.totalInstanceCount && !placed.instances)
		{
			throw std::runtime_error("IW7 Scriptable instance count has no array");
		}
		for (unsigned i = 0; i < placed.totalInstanceCount; ++i)
		{
			const auto& instance = placed.instances[i];
			root(instance.contextHeader.context.def);
			for (const auto& local : instance.contextHeaderLocalClient)
			{
				root(local.context.def);
			}
		}
		if (infos.empty() && graphs.empty())
		{
			return;
		}
		asset_data::rumble_collection data;
		for (auto& [name, value] : infos)
		{
			data.infos.push_back(std::move(value));
		}
		for (auto& [name, value] : graphs)
		{
			data.graphs.push_back(std::move(value));
		}
		const auto semantic = asset_data::write_rumbles(data);
		evidence["placedDefinitions"] = placed_definitions.size();
		evidence["reachableDefinitions"] = definitions.size();
		evidence["uniqueRumbleEvents"] = rumble_events;
		evidence["unmappedFields"] = ordered_json::array();
		const auto output = extracted / "asset_data";
		std::filesystem::create_directories(output);
		const auto write = [&](const char* name, const ordered_json& value)
		{
			std::ofstream file(output / name, std::ios::binary | std::ios::trunc);
			file << value.dump(2) << '\n';
			if (!file.good())
			{
				throw std::runtime_error("cannot write IW7 rumble asset data");
			}
		};
		write("rumbles.json", semantic);
		write("rumbles.iw7.provenance.json", evidence);
		ZONETOOL_INFO("iw7: exported %zu rumble(s), %zu graph(s) from %zu Scriptable rumble event(s)", data.infos.size(),
					  data.graphs.size(), rumble_events);
	}
}
