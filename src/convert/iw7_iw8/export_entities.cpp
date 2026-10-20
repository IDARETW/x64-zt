#include <std_include.hpp>
#include "export.hpp"

#include <iw8/dump/maps_convert.h>
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
		// Replay counterpart (Uplink ball spawns, client triggers).
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
		// swaps the model, plays effects or a sound. Other kinds (swinging frames, multi-stage carts) do not convert.
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
				inner = std::clamp(inner, outer + 0.001f, 1.0f);
			}

			json entry =
			{
				{"type", type},
				{"can_use_shadow_map", light.canUseShadowMap != 0},
				{"exponent", light.exponent},
				{"color", vector_json(color.data(), 3)},
				{"direction", vector_json(direction.data(), 3)},
				{"up", vector_json(light.up, 3)},
				{"authored_up", true},
				{"origin", vector_json(light.origin, 3)},
				{"radius", light.radius},
				{"cos_half_fov_outer", outer},
				{"cos_half_fov_inner", inner},
				{"cos_half_fov_expanded", outer},
				{"rotation_limit", light.rotationLimit},
				{"translation_limit", light.translationLimit},
				{"entity_id", light.entityId},
				{"bulb_radius", std::max(light.bulbRadius, 1.0f)},
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
				{"definition", intensity ? "light_point_linear" : ""},
			};

			if (intensity)
			{
				entry["intensity"] = *intensity;
			}

			lights.push_back(std::move(entry));
		}

		// The attenuation that IW3's light_point_linear definition names: the pipeline matches it to Replay's own.
		if (local_lights)
		{
			const auto path = output.extracted / "lights" / "light_point_linear";
			std::filesystem::create_directories(path.parent_path());
			std::ofstream(path, std::ios::binary | std::ios::trunc).write("bfalloff_linear", 16);
		}

		write_json(output.extracted / (bsp_name(source) + ".replay-comworld.json"),
		{
			{"schema", 1},
			{"name", bsp_name(source)},
			{"in_use", true},
			{"primary_lights", std::move(lights)},
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

	void export_entities(const source_map& source, extraction& output)
	{
		const auto map_ents = source.map_ents();
		if (!map_ents || !map_ents->entityString)
		{
			throw std::runtime_error(std::format("zone \"{}\" holds no map entities", source.name()));
		}

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
		// targetname that the map script breaks them by. Their other states, animations and other events do not convert.
		const auto& scriptables = map_ents->scriptableMapEnts;
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
			if (const auto targetname = script_string(scriptables.instances[i].targetname); !targetname.empty())
			{
				entities += std::format("\"targetname\" \"{}\"\n", targetname);
			}
			else if (const auto destructible = def ? destructible_of(source, *def, output) : std::nullopt)
			{
				entities += std::format("\"targetname\" \"{}\"\n", destructible->targetname);
				output.scriptable_destructibles.try_emplace(destructible->targetname, *destructible);
				destructibles++;
			}

			entities += "}\n";
			output.models.try_emplace(file_name(name), model);
			placed++;
			if (def)
			{
				add_scriptable_effects(source, *def, model, context, output);
			}
		}

		ZONETOOL_INFO("iw7: placed %zu of %u scriptables (%u runtime, %u reserved) as script models, with %zu effects of their first states",
			placed, scriptables.totalInstanceCount, scriptables.runtimeInstanceCount, scriptables.reservedInstanceCount,
			output.scriptable_effects.size());
		ZONETOOL_INFO("iw7: %zu of them break (%zu kinds: a health state, then the broken model, its effects and sound)", destructibles,
			output.scriptable_destructibles.size());

		// IW7's breakable glass: an IW3 glass entity for each pane.
		const auto panes = glass_panes(source);
		for (auto i = 0u; i < panes.size(); i++)
		{
			entities += std::format("{{\n\"classname\" \"script_brushmodel\"\n\"model\" \"*{}\"\n}}\n", glass_brush_model(source, i));
		}

		entities = with_converting_classes(entities);
		const auto path = output.extracted / (bsp_name(source) + ".ents");
		std::filesystem::create_directories(path.parent_path());
		std::ofstream(path, std::ios::binary | std::ios::trunc).write(entities.data(), static_cast<std::streamsize>(entities.size()));
		ZONETOOL_INFO("iw7: exported %zu bytes of entities with %u spawn points", entities.size(), spawns.spawnsCount);
	}
}
