#include <std_include.hpp>
#include "iw8.hpp"
#include "havok/compendium.hpp"
#include "havok/mesh.hpp"
#include "havok/objects.hpp"
#include "havok/physics.hpp"
#include "havok/tagfile.hpp"
#include "zone/asset_files.hpp"
#include "zone/loader.hpp"
#include "zone/writer.hpp"
#include "neutral/aiasm.hpp"
#include "neutral/behavior.hpp"
#include "neutral/bones.hpp"
#include "neutral/content.hpp"
#include "neutral/effects.hpp"
#include "neutral/fx.hpp"
#include "neutral/fxworld.hpp"
#include "neutral/gameplay.hpp"
#include "neutral/gfxworld.hpp"
#include "neutral/image.hpp"
#include "neutral/impact.hpp"
#include "neutral/lighting.hpp"
#include "neutral/mapents.hpp"
#include "neutral/material.hpp"
#include "neutral/path_data.hpp"
#include "neutral/physics.hpp"
#include "neutral/physics_events.hpp"
#include "neutral/plain.hpp"
#include "neutral/rumble.hpp"
#include "neutral/scriptable.hpp"
#include "neutral/shader.hpp"
#include "neutral/tacgraph.hpp"
#include "neutral/sound.hpp"
#include "neutral/sound_globals.hpp"
#include "neutral/techset.hpp"
#include "neutral/stream.hpp"
#include "neutral/text.hpp"
#include "neutral/vfx.hpp"
#include "neutral/world.hpp"
#include "neutral/xanim.hpp"
#include "neutral/xmodel.hpp"
#include <core/assets/neutral_files.hpp>

#include <game.hpp>

namespace zonetool::iw8
{
	const game& definition();

	namespace
	{
		using asset_key = std::pair<std::int32_t, std::string>;

		std::string asset_name(const std::int32_t type, const void* header)
		{
			const auto name = get_asset_name(type, header);
			return name ? name : "";
		}

		std::vector<zone_asset> to_zone_assets(const std::vector<loaded_asset>& assets)
		{
			std::vector<zone_asset> result;
			result.reserve(assets.size());
			for (const auto& asset : assets)
			{
				result.emplace_back(asset.type, asset.header, asset.referenced, asset.marker);
			}

			return result;
		}

		// The assets of a zone by type and name, with the references to assets that the zone does not hold. A zone can
		// hold an asset twice, and a writer can write it once: a reference to an asset of the zone uses that asset.
		std::set<std::tuple<std::int32_t, std::string, bool>> distinct_assets(const loaded_zone& zone)
		{
			std::set<std::pair<std::int32_t, std::string>> held;
			for (const auto& asset : zone.assets)
			{
				const auto name = get_asset_name(asset.type, asset.header);
				if (!asset.referenced)
				{
					held.emplace(asset.type, name ? name : "");
				}
			}

			std::set<std::tuple<std::int32_t, std::string, bool>> assets;
			for (const auto& asset : zone.assets)
			{
				const std::string name = get_asset_name(asset.type, asset.header) ? get_asset_name(asset.type, asset.header) : "";
				assets.emplace(asset.type, name, asset.referenced && !held.contains({asset.type, name}));
			}

			return assets;
		}

		// IW8_REWRITE_OUT=<folder> also writes the zone there, to load it with the game loaders.
		void save_written_zone(const loaded_zone& zone, const written_zone& written)
		{
			char output[MAX_PATH]{};
			if (!GetEnvironmentVariableA("IW8_REWRITE_OUT", output, sizeof(output)))
			{
				return;
			}

			xfile::fastfile file{};
			file.payload = written.data;
			file.block_sizes = written.block_sizes;
			file.xfile_version = zone.file.xfile_version;
			file.transient = zone.file.transient;
			file.preload_walk_size = std::max(written.preload_walk_size, zone.file.preload_walk_size);
			xfile::write(std::filesystem::path(output) / (zone.name + ".ff"), definition().format, file);
		}

		// IW8_REWRITE=fresh writes the zone with the choices of the writer (as the IW3 converter writes its zones), loads
		// the result and compares the assets.
		void verify_fresh_write(const loaded_zone& zone)
		{
			const auto written = write_zone(zone);
			save_written_zone(zone, written);

			xfile::fastfile file{};
			file.payload = written.data;
			file.block_sizes = written.block_sizes;
			const auto reloaded = load_zone(zone.name, std::move(file));

			const auto written_assets = distinct_assets(*reloaded);
			const auto zone_assets = distinct_assets(zone);
			if (written_assets != zone_assets || reloaded->list.size() != zone.list.size())
			{
				throw std::runtime_error(std::format("the written zone holds other assets: {} of {} assets, {} of {} list entries",
					written_assets.size(), zone_assets.size(), reloaded->list.size(), zone.list.size()));
			}

			ZONETOOL_INFO("%s: the written zone loads with the same assets (0x%zX of 0x%zX bytes)", zone.name.data(),
				written.data.size(), zone.file.payload.size());
		}

		std::map<asset_key, asset_file> dump_assets(const loaded_zone& zone)
		{
			std::map<asset_key, asset_file> dumps;
			for (const auto& asset : zone.assets)
			{
				const asset_key key{asset.type, asset_name(asset.type, asset.header)};
				if (!asset.referenced && !dumps.contains(key))
				{
					dumps.emplace(key, dump_asset_file(asset.type, asset.header));
				}
			}

			return dumps;
		}

		// IW8_REWRITE=dump dumps each asset, parses the dumps, writes them as a zone, loads it and dumps it again.
		void verify_dump_round_trip(const loaded_zone& zone)
		{
			const auto dumps = dump_assets(zone);

			asset_parser parser;
			std::map<asset_key, void*> parsed;
			std::vector<zone_asset> assets;
			std::size_t shared{};
			for (const auto& [key, file] : dumps)
			{
				const auto header = parser.parse_asset_file(key.first, key.second, file);
				parsed.emplace(key, header);
				assets.emplace_back(key.first, header, false);
				shared += file.shared;

				// An asset of a type with a shared file that the shared file does not hold.
				if (!file.shared && asset_file_locations(key.first, key.second).size() > 1)
				{
					ZONETOOL_INFO("%s: %s", zone.name.data(), file.path.data());
				}
			}

			std::vector<zone_asset> list;
			for (const auto& entry : zone.list)
			{
				const asset_key key{entry.type, asset_name(entry.type, entry.header)};
				const auto found = parsed.find(key);
				list.emplace_back(entry.type, found != parsed.end() ? found->second : parser.stub(entry.type, key.second),
					found == parsed.end());
			}

			const std::array<const char*, 1> strings{nullptr};
			const auto written = write_zone(list, assets, strings);

			xfile::fastfile fastfile{};
			fastfile.payload = written.data;
			fastfile.block_sizes = written.block_sizes;
			const auto reloaded = load_zone(zone.name, std::move(fastfile), {.keep_temp_data = true});
			if (distinct_assets(*reloaded) != distinct_assets(zone))
			{
				throw std::runtime_error("the zone that the dumps make holds other assets");
			}

			const auto again = dump_assets(*reloaded);
			for (const auto& [key, file] : dumps)
			{
				const auto found = again.find(key);
				if (found == again.end() || found->second.path != file.path || found->second.data != file.data)
				{
					char debug[MAX_PATH]{};
					if (GetEnvironmentVariableA("IW8_DUMP_DEBUG", debug, sizeof(debug)))
					{
						std::ofstream(std::filesystem::path(debug) / "first", std::ios::binary).write(file.data.data(), static_cast<std::streamsize>(file.data.size()));
						if (found != again.end())
						{
							std::ofstream(std::filesystem::path(debug) / "again", std::ios::binary).write(found->second.data.data(), static_cast<std::streamsize>(found->second.data.size()));
						}
					}

					throw std::runtime_error(std::format("{} dumps differently from the zone that its dump makes", file.path));
				}
			}

			ZONETOOL_INFO("%s: %zu assets (%zu in shared files) dump, parse and write again to the same files", zone.name.data(),
				dumps.size(), shared);
		}

		// IW8_REWRITE=1 writes the zone again from the loaded assets and compares it with the zone data.
		void verify_rewrite(const loaded_zone& zone)
		{
			const auto list = to_zone_assets(zone.list);
			const auto assets = to_zone_assets(zone.assets);
			const auto written = write_zone(list, assets, zone.script_strings, {.markers = &zone.markers,
				.strings_marker = zone.strings_marker, .assets_marker = zone.assets_marker});

			save_written_zone(zone, written);

			const auto& original = zone.file.payload;
			const auto size = std::min(original.size(), written.data.size());
			const auto difference = std::mismatch(original.begin(), original.begin() + size, written.data.begin());
			const auto offset = static_cast<std::size_t>(difference.first - original.begin());

			std::string blocks;
			for (std::size_t i = 0; i < written.block_sizes.size(); i++)
			{
				if (written.block_sizes[i] != zone.file.block_sizes[i])
				{
					blocks += std::format(" {}:0x{:X}/0x{:X}", i, written.block_sizes[i], zone.file.block_sizes[i]);
				}
			}

			if (offset == size && original.size() == written.data.size())
			{
				// Stock block sizes add slack to the peaks that the writer computes (decal volume materials).
				ZONETOOL_INFO("%s: the written zone data is equal to the zone data; block sizes (written/zone)%s",
					zone.name.data(), blocks.empty() ? " equal" : blocks.data());
				return;
			}

			const auto bytes_at = [offset](const std::string& data)
			{
				std::string text;
				for (auto i = offset & ~std::size_t(15); i < std::min(data.size(), (offset & ~std::size_t(15)) + 32); i++)
				{
					text += std::format("{:02X}", static_cast<std::uint8_t>(data[i]));
				}

				return text;
			};

			std::string asset = "the asset list";
			for (const auto& [start, type, name] : written.asset_offsets)
			{
				if (start > offset)
				{
					break;
				}

				const auto format = find_asset_format(type);
				asset = std::format("{} \"{}\" at 0x{:X}", format ? format->name : "?", name, start);
			}

			throw std::runtime_error(std::format("the written zone differs: 0x{:X} bytes, zone data 0x{:X} bytes, first "
				"difference at 0x{:X} in {} (written {} zone {}); block sizes (written/zone){}", written.data.size(),
				original.size(), offset, asset, bytes_at(written.data), bytes_at(original), blocks.empty() ? " equal" : blocks));
		}

		// The neutral data of an asset that has a neutral adapter (neutral/*.hpp).
		using neutral_data = std::variant<asset_data::localize_entry, asset_data::net_const_strings, asset_data::raw_file,
			asset_data::string_table, asset_data::rumble_graph, asset_data::rumble_info, neutral::image_source,
			neutral::xanim_source, neutral::light_def_source, neutral::grading_clut_source, neutral::xmodel_source,
			neutral::xmodel_surfs_source, neutral::decal_volume_material_source, neutral::stream_key_source,
			neutral::material_source, neutral::shader_source, asset_data::technique_set, asset_data::physics_fx_shape,
			asset_data::detail_collision, asset_data::physics_asset, asset_data::sound::sound_bank_resident, asset_data::sound::sound_bank_transient,
			asset_data::sound_globals::sound_globals, asset_data::world::com_world, asset_data::world::glass_world, asset_data::world::map_edge_list,
			asset_data::world::nav_mesh, asset_data::world::clip_map, asset_data::world::fog_spline, asset_data::effects::particle_sim_animation, asset_data::vfx::particle_system, asset_data::scriptable::scriptable,
			asset_data::impact_table, asset_data::surface_effect_table, asset_data::physics_fx_pipeline,
			asset_data::fxworld::fx_world,
			asset_data::fxworld::fx_world_transient_zone,
			asset_data::behavior::behaviortree,
			asset_data::aiasm::aiasm,
			asset_data::aiasm::aianimset,
			asset_data::path_data::path_data,
			asset_data::tacgraph::tac_graph,
			asset_data::bones::dynamicbones,
			asset_data::bones::proceduralbones,
			asset_data::mapents::map_ents,
			asset_data::gfxworld::gfx_world,
			asset_data::gfxworld::gfx_world_transient_zone,
			asset_data::physics_events::physics_sfx_event,
			asset_data::physics_events::physics_vfx_event,
			asset_data::content::vehicle,
			asset_data::content::xcam,
			asset_data::content::dynentitylist,
			asset_data::content::attachment,
			asset_data::content::animclass,
			asset_data::content::luafile,
			asset_data::content::leaderboarddef,
			asset_data::content::virtualleaderboarddef,
			asset_data::content::clothasset,
			asset_data::content::stterrain,
			asset_data::content::collisiontile,
			asset_data::gameplay::animpkg,
			asset_data::gameplay::sfxpkg,
			asset_data::gameplay::vfxpkg,
			asset_data::gameplay::tracer,
			asset_data::gameplay::laser,
			asset_data::gameplay::camera,
			asset_data::gameplay::hudoutline,
			asset_data::gameplay::footstepvfx,
			asset_data::gameplay::xcompositemodel,
			asset_data::gameplay::xanimcurve,
			asset_data::gameplay::keyvaluepairs,
			asset_data::gameplay::blendspace2d,
			asset_data::gameplay::camo,
			asset_data::gameplay::accessory,
			asset_data::gameplay::locdmgtable,
			asset_data::gameplay::bulletpenetration,
			asset_data::gameplay::cinematicmotion,
			asset_data::gameplay::decalvolumemask,
			asset_data::gameplay::gesture,
			asset_data::gameplay::reticle,
			asset_data::gameplay::streaminginfo,
			asset_data::gameplay::suit,
			asset_data::gameplay::suitanimpackage,
			asset_data::gameplay::coverselector,
			asset_data::gameplay::enemyselector,
			asset_data::gameplay::carryobject,
			asset_data::gameplay::execution,
			asset_data::gameplay::clientcharacter,
			asset_data::gameplay::physicslibrary,
			asset_data::gameplay::ttf,
			neutral::vector_field_source>;

		template <typename T>
		std::optional<neutral_data> accepted(std::expected<T, std::string> value, const char* kind, std::string* kept)
		{
			if (value)
			{
				return std::move(*value);
			}

			if (kept)
			{
				*kept = std::format("{}: {}", kind, value.error());
			}

			return std::nullopt;
		}

		// An asset of an adapter-backed type that the adapter declines (an image whose texels the zone does not hold, an
		// animation whose streams it does not decode) has no neutral data: `kept` receives the reason when it is given.
		// IW8_NEUTRAL_SKIP=<type id>,... keeps the assets of those types native, to find the adapter that changes a zone.
		bool neutral_skipped(const std::int32_t type)
		{
			char list[256]{};
			if (!GetEnvironmentVariableA("IW8_NEUTRAL_SKIP", list, sizeof(list)))
			{
				return false;
			}

			for (const auto id : std::string_view(list) | std::views::split(','))
			{
				if (std::string_view(id) == std::to_string(type))
				{
					return true;
				}
			}

			return false;
		}

		// IW8_NEUTRAL_TRACE=<type id> logs the name of each asset of that type before its adapter reads it, so that a crash
		// in an adapter names the asset.
		bool neutral_traced(const std::int32_t type)
		{
			char list[256]{};
			return GetEnvironmentVariableA("IW8_NEUTRAL_TRACE", list, sizeof(list)) && std::string_view(list) == std::to_string(type);
		}

		std::optional<neutral_data> neutral_of(const std::int32_t type, const void* header, std::string* kept = nullptr)
		{
			if (neutral_skipped(type))
			{
				return std::nullopt;
			}

			neutral::plain::trace_enabled = neutral_traced(type);
			if (neutral::plain::trace_enabled)
			{
				ZONETOOL_INFO("neutral adapter reads %s", asset_name(type, header).data());
			}

			switch (type)
			{
			case ASSET_TYPE_LOCALIZE_ENTRY:
				return neutral::to_neutral(*static_cast<const LocalizeEntry*>(header));
			case ASSET_TYPE_NET_CONST_STRINGS:
				return neutral::to_neutral(*static_cast<const NetConstStrings*>(header));
			case ASSET_TYPE_RAWFILE:
				return neutral::to_neutral(*static_cast<const RawFile*>(header));
			case ASSET_TYPE_RUMBLE_GRAPH:
				return neutral::to_neutral(*static_cast<const RumbleGraph*>(header));
			case ASSET_TYPE_RUMBLE:
				return neutral::to_neutral(*static_cast<const RumbleInfo*>(header));
			case ASSET_TYPE_IMAGE:
				if (auto source = neutral::to_neutral(*static_cast<const GfxImage*>(header)))
				{
					return std::move(*source);
				}

				if (kept)
				{
					*kept = "image texels outside the zone";
				}

				return std::nullopt;
			case ASSET_TYPE_LIGHT_DEF:
				return neutral::to_neutral(*static_cast<const GfxLightDef*>(header));
			case ASSET_TYPE_GRADING_CLUT:
				return neutral::to_neutral(*static_cast<const GfxGradingClut*>(header));
			case ASSET_TYPE_STRINGTABLE:
				return accepted(neutral::to_neutral(*static_cast<const StringTable*>(header)), "string table", kept);
			case ASSET_TYPE_XANIMPARTS:
				return accepted(neutral::to_neutral(*static_cast<const XAnimParts*>(header)), "animation", kept);
			case ASSET_TYPE_XMODEL:
				return accepted(neutral::to_neutral(*static_cast<const XModel*>(header)), "model", kept);
			case ASSET_TYPE_PHYSICS_FX_PIPELINE:
				return accepted(neutral::to_neutral(*static_cast<const PhysicsFXPipeline*>(header)), "physics fx pipeline", kept);
			case ASSET_TYPE_VECTORFIELD:
				return accepted(neutral::to_neutral(*static_cast<const VectorField*>(header)), "vector field", kept);
			case ASSET_TYPE_STREAM_KEY:
				return accepted(neutral::to_neutral(*static_cast<const StreamKey*>(header)), "stream key", kept);
			case ASSET_TYPE_VERTEXSHADER:
			case ASSET_TYPE_HULLSHADER:
			case ASSET_TYPE_DOMAINSHADER:
			case ASSET_TYPE_PIXELSHADER:
			case ASSET_TYPE_COMPUTESHADER:
			case ASSET_TYPE_LIBSHADER:
				return accepted(neutral::to_neutral(*static_cast<const ComputeShader*>(header), type), "shader", kept);
			case ASSET_TYPE_IMPACT_FX:
				return accepted(neutral::to_neutral(*static_cast<const FxImpactTable*>(header)), "impact effect table", kept);
			case ASSET_TYPE_SURFACE_FX:
				return accepted(neutral::to_neutral(*static_cast<const SurfaceFxTable*>(header)), "surface effect table", kept);
			case ASSET_TYPE_SOUND_GLOBALS:
				return accepted(neutral::to_neutral(*static_cast<const SndGlobals*>(header)), "sound globals", kept);
			case ASSET_TYPE_PARTICLE_SIM_ANIMATION:
				return accepted(neutral::to_neutral(*static_cast<const FxParticleSimAnimation*>(header)), "particle simulation animation", kept);
			case ASSET_TYPE_EDGE_LIST:
				return accepted(neutral::to_neutral(*static_cast<const MapEdgeList*>(header)), "edge list", kept);
			case ASSET_TYPE_NAVMESH:
				return accepted(neutral::to_neutral(*static_cast<const NavMeshData*>(header)), "nav mesh", kept);
			case ASSET_TYPE_CLIPMAP:
				return accepted(neutral::to_neutral(*static_cast<const clipMap_t*>(header)), "clip map", kept);
			case ASSET_TYPE_FOG_SPLINE:
				return accepted(neutral::to_neutral(*static_cast<const GfxFogSpline*>(header)), "fog spline", kept);
			case ASSET_TYPE_VFX:
				return accepted(neutral::to_neutral(*static_cast<const ParticleSystemDef*>(header)), "particle system", kept);
			case ASSET_TYPE_ANIM_PACKAGE:
				return accepted(neutral::to_neutral(*static_cast<const WeaponAnimPackage*>(header)), "animation package", kept);
			case ASSET_TYPE_SFX_PACKAGE:
				return accepted(neutral::to_neutral(*static_cast<const WeaponSFXPackage*>(header)), "sound package", kept);
			case ASSET_TYPE_VFX_PACKAGE:
				return accepted(neutral::to_neutral(*static_cast<const WeaponVFXPackage*>(header)), "effects package", kept);
			case ASSET_TYPE_TRACER:
				return accepted(neutral::to_neutral(*static_cast<const TracerDef*>(header)), "tracer", kept);
			case ASSET_TYPE_LASER:
				return accepted(neutral::to_neutral(*static_cast<const LaserDef*>(header)), "laser", kept);
			case ASSET_TYPE_CAMERA:
				return accepted(neutral::to_neutral(*static_cast<const CameraDef*>(header)), "camera", kept);
			case ASSET_TYPE_HUDOUTLINE:
				return accepted(neutral::to_neutral(*static_cast<const HudOutlineDef*>(header)), "hudoutline", kept);
			case ASSET_TYPE_FOOTSTEP_VFX:
				return accepted(neutral::to_neutral(*static_cast<const FootstepVFX*>(header)), "footstepvfx", kept);
			case ASSET_TYPE_XCOMPOSITEMODEL:
				return accepted(neutral::to_neutral(*static_cast<const XCompositeModelDef*>(header)), "xcompositemodel", kept);
			case ASSET_TYPE_XANIMCURVE:
				return accepted(neutral::to_neutral(*static_cast<const XAnimCurve*>(header)), "xanimcurve", kept);
			case ASSET_TYPE_KEYVALUEPAIRS:
				return accepted(neutral::to_neutral(*static_cast<const KeyValuePairs*>(header)), "keyvaluepairs", kept);
			case ASSET_TYPE_BLENDSPACE2D:
				return accepted(neutral::to_neutral(*static_cast<const BlendSpace2DDef*>(header)), "blendspace2d", kept);
			case ASSET_TYPE_CAMO:
				return accepted(neutral::to_neutral(*static_cast<const Camo*>(header)), "camo", kept);
			case ASSET_TYPE_ACCESSORY:
				return accepted(neutral::to_neutral(*static_cast<const AccessoryDef*>(header)), "accessory", kept);
			case ASSET_TYPE_LOCDMGTABLE:
				return accepted(neutral::to_neutral(*static_cast<const LocDmgTable*>(header)), "locdmgtable", kept);
			case ASSET_TYPE_BULLETPENETRATION:
				return accepted(neutral::to_neutral(*static_cast<const BulletPenetration*>(header)), "bulletpenetration", kept);
			case ASSET_TYPE_CINEMATICMOTION:
				return accepted(neutral::to_neutral(*static_cast<const CinematicMotionDef*>(header)), "cinematicmotion", kept);
			case ASSET_TYPE_DECAL_VOLUME_MASK:
				return accepted(neutral::to_neutral(*static_cast<const GfxDecalVolumeMask*>(header)), "decalvolumemask", kept);
			case ASSET_TYPE_GESTURE:
				return accepted(neutral::to_neutral(*static_cast<const Gesture*>(header)), "gesture", kept);
			case ASSET_TYPE_RETICLE:
				return accepted(neutral::to_neutral(*static_cast<const ReticleDef*>(header)), "reticle", kept);
			case ASSET_TYPE_STREAMING_INFO:
				return accepted(neutral::to_neutral(*static_cast<const StreamingInfo*>(header)), "streaminginfo", kept);
			case ASSET_TYPE_SUIT:
				return accepted(neutral::to_neutral(*static_cast<const SuitDef*>(header)), "suit", kept);
			case ASSET_TYPE_SUITANIMPACKAGE:
				return accepted(neutral::to_neutral(*static_cast<const SuitAnimPackage*>(header)), "suitanimpackage", kept);
			case ASSET_TYPE_COVERSELECTOR:
				return accepted(neutral::to_neutral(*static_cast<const CoverSelector*>(header)), "coverselector", kept);
			case ASSET_TYPE_ENEMYSELECTOR:
				return accepted(neutral::to_neutral(*static_cast<const EnemySelector*>(header)), "enemyselector", kept);
			case ASSET_TYPE_CARRYOBJECT:
				return accepted(neutral::to_neutral(*static_cast<const CarryObjectDef*>(header)), "carryobject", kept);
			case ASSET_TYPE_EXECUTION:
				return accepted(neutral::to_neutral(*static_cast<const ExecutionDef*>(header)), "execution", kept);
			case ASSET_TYPE_CLIENTCHARACTER:
				return accepted(neutral::to_neutral(*static_cast<const ClientCharacter*>(header)), "clientcharacter", kept);
			case ASSET_TYPE_PHYSICSLIBRARY:
				return accepted(neutral::to_neutral(*static_cast<const PhysicsLibrary*>(header)), "physics library", kept);
			case ASSET_TYPE_TTF:
				return accepted(neutral::to_neutral(*static_cast<const TTFDef*>(header)), "font", kept);
			case ASSET_TYPE_ATTACHMENT:
				return accepted(neutral::to_neutral(*static_cast<const WeaponAttachment*>(header)), "weapon attachment", kept);
			case ASSET_TYPE_ANIMCLASS:
				return accepted(neutral::to_neutral(*static_cast<const AnimationClass*>(header)), "animation class", kept);
			case ASSET_TYPE_LUA_FILE:
				return accepted(neutral::to_neutral(*static_cast<const LuaFile*>(header)), "Lua file", kept);
			case ASSET_TYPE_LEADERBOARD:
				return accepted(neutral::to_neutral(*static_cast<const LeaderboardDef*>(header)), "leaderboard", kept);
			case ASSET_TYPE_VIRTUAL_LEADERBOARD:
				return accepted(neutral::to_neutral(*static_cast<const VirtualLeaderboardDef*>(header)), "virtual leaderboard", kept);
			case ASSET_TYPE_CLOTHASSET:
				return accepted(neutral::to_neutral(*static_cast<const ClothAsset*>(header)), "cloth", kept);
			case ASSET_TYPE_SUPER_TERRAIN:
				return accepted(neutral::to_neutral(*static_cast<const StTerrain*>(header)), "super terrain", kept);
			case ASSET_TYPE_COLLISION_TILE:
				return accepted(neutral::to_neutral(*static_cast<const CollisionTile*>(header)), "collision tile", kept);
			case ASSET_TYPE_PHYSICS_SFX_EVENT_ASSET:
				return accepted(neutral::to_neutral(*static_cast<const PhysicsSFXEventAsset*>(header)), "physics sound events", kept);
			case ASSET_TYPE_PHYSICS_VFX_EVENT_ASSET:
				return accepted(neutral::to_neutral(*static_cast<const PhysicsVFXEventAsset*>(header)), "physics effect events", kept);
			case ASSET_TYPE_VEHICLE:
				return accepted(neutral::to_neutral(*static_cast<const VehicleDef*>(header)), "vehicle", kept);
			case ASSET_TYPE_FXWORLD:
				return accepted(neutral::to_neutral(*static_cast<const FxWorld*>(header)), "effects world", kept);
			case ASSET_TYPE_FXWORLD_TRANSIENT_ZONE:
				return accepted(neutral::to_neutral(*static_cast<const FxWorldTransientZone*>(header)), "effects transient zone", kept);
			case ASSET_TYPE_PATHDATA:
				return accepted(neutral::to_neutral(*static_cast<const PathData*>(header)), "path data", kept);
			case ASSET_TYPE_TACGRAPH:
				return accepted(neutral::to_neutral(*static_cast<const TacticalGraphData*>(header)), "tactical graph", kept);
			case ASSET_TYPE_ASM:
				return accepted(neutral::to_neutral(*static_cast<const ASM*>(header)), "state machine", kept);
			case ASSET_TYPE_ANIMSET:
				return accepted(neutral::to_neutral(*static_cast<const Animset*>(header)), "animation set", kept);
			case ASSET_TYPE_BEHAVIOR_TREE:
				return accepted(neutral::to_neutral(*static_cast<const BehaviorTree*>(header)), "behavior tree", kept);
			case ASSET_TYPE_XANIM_PROCEDURALBONES:
				return accepted(neutral::to_neutral(*static_cast<const XAnimProceduralBones*>(header)), "procedural bones", kept);
			case ASSET_TYPE_XANIM_DYNAMICBONES:
				return accepted(neutral::to_neutral(*static_cast<const XAnimDynamicBones*>(header)), "dynamic bones", kept);
			case ASSET_TYPE_MAP_ENTS:
				return accepted(neutral::to_neutral(*static_cast<const MapEnts*>(header)), "map entities", kept);
			case ASSET_TYPE_GFXWORLD:
				return accepted(neutral::to_neutral(*static_cast<const GfxWorld*>(header)), "graphics world", kept);
			case ASSET_TYPE_GFXWORLD_TRANSIENT_ZONE:
				return accepted(neutral::to_neutral(*static_cast<const GfxWorldTransientZone*>(header)), "graphics transient zone", kept);
			case ASSET_TYPE_DYNENTITY_LIST:
				return accepted(neutral::to_neutral(*static_cast<const DynEntityList*>(header)), "dynamic entity list", kept);
			case ASSET_TYPE_XCAM:
				return accepted(neutral::to_neutral(*static_cast<const XCam*>(header)), "camera animation", kept);
			case ASSET_TYPE_SCRIPTABLE:
				return accepted(neutral::to_neutral(*static_cast<const ScriptableDef*>(header)), "scriptable", kept);
			case ASSET_TYPE_COMWORLD:
				return accepted(neutral::to_neutral(*static_cast<const ComWorld*>(header)), "com world", kept);
			case ASSET_TYPE_GLASSWORLD:
				return accepted(neutral::to_neutral(*static_cast<const GlassWorld*>(header)), "glass world", kept);
			case ASSET_TYPE_SOUND_BANK:
				return accepted(neutral::to_neutral(*static_cast<const SndBankResident*>(header)), "sound bank", kept);
			case ASSET_TYPE_SOUND_BANK_TRANSIENT:
				return accepted(neutral::to_neutral(*static_cast<const SndBankTransient*>(header)), "transient sound bank", kept);
			case ASSET_TYPE_PHYSICSASSET:
				return accepted(neutral::to_neutral(*static_cast<const PhysicsAsset*>(header)), "physics asset", kept);
			case ASSET_TYPE_PHYSICS_FX_SHAPE:
				return accepted(neutral::to_neutral(*static_cast<const PhysicsFXShape*>(header)), "physics fx shape", kept);
			case ASSET_TYPE_XMODEL_DETAIL_COLLISION:
				return accepted(neutral::to_neutral(*static_cast<const XModelDetailCollision*>(header)), "detail collision", kept);
			case ASSET_TYPE_TECHNIQUE_SET:
				return accepted(neutral::to_neutral(*static_cast<const MaterialTechniqueSet*>(header)), "technique set", kept);
			case ASSET_TYPE_MATERIAL:
				return accepted(neutral::to_neutral(*static_cast<const Material*>(header)), "material", kept);
			case ASSET_TYPE_DECAL_VOLUME_MATERIAL:
				return accepted(neutral::to_neutral(*static_cast<const GfxDecalVolumeMaterial*>(header)), "decal volume material", kept);
			case ASSET_TYPE_XMODEL_SURFS:
				return accepted(neutral::to_neutral(*static_cast<const XModelSurfs*>(header)), "model surfaces", kept);
			default:
				return std::nullopt;
			}
		}

		// IW8_REWRITE=neutral rebuilds each asset that has a neutral adapter from its neutral data, in fresh memory, and
		// writes the zone as IW8_REWRITE=fresh does: the kept markers and packed offsets of the zone describe its own
		// layout, and the shared data of the rebuilt assets (strings that several assets point to) is not neutral data.
		// The written zone must load with the same assets and give the same neutral data for each rebuilt asset.
		void verify_neutral_rewrite(const loaded_zone& zone)
		{
			std::map<void*, neutral_data> data;
			std::map<asset_key, neutral_data> by_key;
			std::size_t size{};
			// The assets that keep their native records, by reason: the count and the first name.
			std::map<std::string, std::pair<std::size_t, std::string>> kept;
			for (const auto& asset : zone.assets)
			{
				if (asset.referenced || data.contains(asset.header))
				{
					continue;
				}

				std::string reason;
				auto value = neutral_of(asset.type, asset.header, &reason);
				if (!reason.empty())
				{
					auto& [count, example] = kept[reason];
					if (!count++)
					{
						example = asset_name(asset.type, asset.header);
					}
				}

				if (value)
				{
					size += std::visit([](const auto& item) { return neutral::native_size(item); }, *value);
					by_key.emplace(asset_key{asset.type, asset_name(asset.type, asset.header)}, *value);
					data.emplace(asset.header, std::move(*value));
				}
			}

			zone_memory memory(std::max<std::size_t>(size, 1));
			// The header of each asset of the zone by type and definition name (a reference stub without its comma).
			// A definition is preferred to a reference stub of the same asset.
			std::map<asset_key, void*> headers;
			for (const auto defined : {true, false})
			{
				for (const auto& asset : zone.assets)
				{
					if (asset.referenced != defined)
					{
						auto name = asset_name(asset.type, asset.header);
						headers.try_emplace({asset.type, name.starts_with(',') ? name.substr(1) : name}, asset.header);
					}
				}
			}

			// An asset that a rebuilt asset points to is rebuilt first when it has neutral data.
			std::map<void*, void*> rebuilt;
			std::map<void*, void*> building;
			std::set<void*> cyclic;
			neutral::technique_cache techniques;
			std::function<void*(void*)> rebuild;
			const neutral::asset_resolver resolve = [&](const std::int32_t type, const std::string& name) -> void*
			{
				const auto found = headers.find({type, name});
				return found != headers.end() ? rebuild(found->second) : nullptr;
			};

			const auto resolve_graph = [&resolve](const std::string& name)
			{
				return static_cast<RumbleGraph*>(resolve(ASSET_TYPE_RUMBLE_GRAPH, name));
			};

			rebuild = [&](void* header) -> void*
			{
				const auto value = data.find(header);
				if (value == data.end())
				{
					return header;
				}

				if (const auto found = rebuilt.find(header); found != rebuilt.end())
				{
					return found->second;
				}

				// A reference that comes back to an asset being rebuilt (a model and the scriptable that destroys it point to
				// each other) gets the root of that asset: an adapter allocates its root before it resolves references.
				if (const auto found = building.find(header); found != building.end())
				{
					cyclic.insert(header);
					return found->second;
				}

				void* const root = memory.cursor();
				building.emplace(header, root);
				auto* result = std::visit([&](const auto& item) -> void*
				{
					using type = std::decay_t<decltype(item)>;
					if constexpr (std::is_same_v<type, asset_data::rumble_info>)
					{
						return neutral::from_neutral(item, memory, resolve_graph);
					}
					else if constexpr (std::is_same_v<type, asset_data::technique_set>)
					{
						return neutral::from_neutral(item, resolve, techniques, memory);
					}
					else if constexpr (requires { neutral::from_neutral(item, resolve, memory); })
					{
						return neutral::from_neutral(item, resolve, memory);
					}
					else
					{
						return neutral::from_neutral(item, memory);
					}
				}, value->second);
				building.erase(header);
				if (cyclic.contains(header) && result != root)
				{
					const auto named = std::ranges::find_if(headers, [&](const auto& entry) { return entry.second == header; });
					throw std::runtime_error(std::format("{} is in a reference cycle, but its adapter does not allocate its root first",
						named != headers.end() ? named->first.second : std::string("an asset")));
				}

				rebuilt.emplace(header, result);
				return result;
			};

			for (const auto& [header, value] : data)
			{
				rebuild(header);
			}

			// A list entry that points at data of the zone by its packed offset (a converter's zone) describes the layout of
			// that zone: the rebuilt zone has another, so the writer finds the place again. Special markers stay.
			const auto replace = [&rebuilt](std::vector<zone_asset> assets)
			{
				for (auto& asset : assets)
				{
					if (const auto found = rebuilt.find(asset.header); found != rebuilt.end())
					{
						asset.header = found->second;
					}

					if (asset.marker && asset.marker < 0xFFFFFFFFFFFFFFF0ull)
					{
						asset.marker = 0;
					}
				}

				return assets;
			};

			const auto written = write_zone(replace(to_zone_assets(zone.list)), replace(to_zone_assets(zone.assets)),
				zone.script_strings);
			save_written_zone(zone, written);

			xfile::fastfile file{};
			file.payload = written.data;
			file.block_sizes = written.block_sizes;
			// Resident texels load into a temporary block: keep it for the comparison.
			const auto reloaded = load_zone(zone.name, std::move(file), {.keep_temp_data = true});
			if (const auto written_assets = distinct_assets(*reloaded), zone_assets = distinct_assets(zone); written_assets != zone_assets)
			{
				std::string difference;
				for (const auto& [from, to, sign] : {std::tuple{&written_assets, &zone_assets, '+'}, {&zone_assets, &written_assets, '-'}})
				{
					for (const auto& [type, name, referenced] : *from)
					{
						if (!to->contains({type, name, referenced}) && difference.size() < 400)
						{
							const auto format = find_asset_format(type);
							difference += std::format(" {}{} {}{}", sign, format ? format->name : "?", name, referenced ? " (reference)" : "");
						}
					}
				}

				throw std::runtime_error("the zone that the neutral data makes holds other assets:" + difference);
			}

			// The asset list entry by entry: a reference to an asset of the zone must stay the reference that it was.
			for (std::size_t index = 0; index < std::min(zone.list.size(), reloaded->list.size()); ++index)
			{
				const auto& before = zone.list[index];
				const auto& after = reloaded->list[index];
				if (before.type != after.type || before.referenced != after.referenced ||
					asset_name(before.type, before.header) != asset_name(after.type, after.header))
				{
					throw std::runtime_error(std::format("list entry {} reads back as {} \"{}\"{} instead of {} \"{}\"{}", index, after.type,
						asset_name(after.type, after.header), after.referenced ? " (reference)" : "", before.type,
						asset_name(before.type, before.header), before.referenced ? " (reference)" : ""));
				}
			}

			std::set<asset_key> compared;
			for (const auto& asset : reloaded->assets)
			{
				const auto found = asset.referenced ? by_key.end() : by_key.find({asset.type, asset_name(asset.type, asset.header)});
				if (found == by_key.end())
				{
					continue;
				}

				if (const auto again = neutral_of(asset.type, asset.header); again != found->second)
				{
					std::string detail;
					const auto* before = std::get_if<neutral::image_source>(&found->second);
					const auto* after = again ? std::get_if<neutral::image_source>(&*again) : nullptr;
					if (before && after)
					{
						detail = before->image == after->image ? "; image equal" : "; image differs";
						for (const auto& [field, bytes] : before->provenance.fields)
						{
							const auto other = after->provenance.fields.find(field);
							if (other == after->provenance.fields.end() || other->second != bytes)
							{
								detail += std::format("; {} {} -> {}", field, bytes,
									other != after->provenance.fields.end() ? other->second : "absent");
							}
						}
					}

					const auto* before_xanim = std::get_if<neutral::xanim_source>(&found->second);
					const auto* after_xanim = again ? std::get_if<neutral::xanim_source>(&*again) : nullptr;
					if (before_xanim && after_xanim)
					{
						const auto& [a, b] = std::tie(before_xanim->xanim, after_xanim->xanim);
						for (const auto& [part, equal] : {std::pair{"bones", a.bones == b.bones}, {"delta", a.delta == b.delta},
							{"notes", a.notes == b.notes}, {"blend shapes", a.blend_shapes == b.blend_shapes &&
							a.blend_shape_weights == b.blend_shape_weights}, {"flags", a.flags == b.flags}, {"frames", a.frames == b.frames &&
							a.framerate == b.framerate && a.frequency == b.frequency}, {"provenance", before_xanim->provenance ==
							after_xanim->provenance}})
						{
							detail += equal ? "" : std::format("; {} differ", part);
						}
					}
					const auto* before_model = std::get_if<neutral::xmodel_source>(&found->second);
					const auto* after_model = again ? std::get_if<neutral::xmodel_source>(&*again) : nullptr;
					if (before_model && after_model)
					{
						const auto& [a, b] = std::tie(before_model->model, after_model->model);
						for (const auto& [part, equal] : {std::pair{"bones", a.bones == b.bones && a.client_bones == b.client_bones},
							{"poses", a.base_poses == b.base_poses && a.bone_infos == b.bone_infos && a.classifications == b.classifications},
							{"surfaces", a.materials == b.materials && a.high_mip_radius_inv_sq == b.high_mip_radius_inv_sq},
							{"levels", a.lods == b.lods}, {"references", a.physics_asset == b.physics_asset && a.cloth == b.cloth &&
							a.scriptable_mover == b.scriptable_mover}, {"provenance", before_model->provenance == after_model->provenance}})
						{
							detail += equal ? "" : std::format("; {} differ", part);
						}
					}

					if (std::holds_alternative<neutral::xanim_source>(found->second) && !after_xanim)
					{
						detail = "; the adapter declines the written animation";
					}

					throw std::runtime_error(std::format("{} reads back with other neutral data{}", found->first.second, detail));
				}

				compared.insert(found->first);
			}

			if (compared.size() != by_key.size())
			{
				throw std::runtime_error(std::format("{} of {} rebuilt assets read back", compared.size(), by_key.size()));
			}

			std::size_t native{};
			for (const auto& [reason, item] : kept)
			{
				native += item.first;
				ZONETOOL_INFO("%s: %zu kept native (%s), e.g. %s", zone.name.data(), item.first, reason.data(), item.second.data());
			}

			ZONETOOL_INFO("%s: %zu assets rebuilt from neutral data (%zu kept native); the written zone (0x%zX of 0x%zX bytes) "
				"loads with the same assets and gives the same neutral data for each", zone.name.data(), compared.size(), native,
				written.data.size(), zone.file.payload.size());
		}

		// The neutral data of each owned animation of the zone, for a comparison with another profile's adapter.
		void write_neutral_files(const loaded_zone& zone, const std::filesystem::path& folder)
		{
			std::size_t written{};
			for (const auto& asset : zone.assets)
			{
				if (asset.referenced || asset.type != ASSET_TYPE_XANIMPARTS)
				{
					continue;
				}

				const auto source = neutral::to_neutral(*static_cast<const XAnimParts*>(asset.header));
				if (!source)
				{
					ZONETOOL_INFO("%s: animation %s has no neutral data: %s", zone.name.data(), asset_name(asset.type, asset.header).data(),
						source.error().data());
					continue;
				}

				asset_data::write_neutral_file(folder, "xanim", source->xanim.name, asset_data::write_xanim(source->xanim));
				++written;
			}

			ZONETOOL_INFO("%s: neutral data of %zu animations written to %s", zone.name.data(), written, folder.string().data());
		}

		using havok::replay_compendium;

		// The Havok tagfiles in the data records of an asset dump.
		std::vector<std::string_view> tagfile_records(const std::string_view dump)
		{
			const auto read = [&dump](const std::size_t offset, auto& value)
			{
				if (offset + sizeof(value) > dump.size())
				{
					throw std::runtime_error("the asset dump is truncated");
				}

				std::memcpy(&value, dump.data() + offset, sizeof(value));
			};

			std::uint32_t count{};
			read(12, count);
			std::size_t offset = 16;
			std::vector<std::string_view> tagfiles;
			for (std::uint32_t i = 0; i < count; i++)
			{
				std::uint8_t kind{};
				read(offset++, kind);
				switch (static_cast<dump_record>(kind))
				{
				case dump_record::data:
				case dump_record::string:
				case dump_record::asset:
				case dump_record::script_string:
				{
					std::uint64_t size{};
					read(offset + 8, size);
					const auto bytes = dump.substr(offset + 16, size);
					if (static_cast<dump_record>(kind) == dump_record::data && bytes.size() > 8 && bytes.substr(4, 4) == "TAG0")
					{
						tagfiles.push_back(bytes);
					}

					offset += 16 + size;
					break;
				}
				case dump_record::runtime:
				case dump_record::temp:
					offset += 8;
					break;
				case dump_record::offset:
					offset += 16;
					break;
				default:
					break;
				}
			}

			return tagfiles;
		}

		// IW8_REWRITE=havok reads the Havok tagfiles of each asset and writes them again. Compact tagfiles must give the
		// same bytes, and self-describing ones the same items when they are read again.
		void verify_havok(const std::filesystem::path& zone_folder, const loaded_zone& zone)
		{
			const auto& compendium = replay_compendium(zone_folder);
			std::set<asset_key> seen;
			std::size_t compact{};
			std::size_t self_describing{};
			std::size_t other{};
			for (const auto& asset : zone.assets)
			{
				const asset_key key{asset.type, asset_name(asset.type, asset.header)};
				if (asset.referenced || !seen.emplace(key).second)
				{
					continue;
				}

				// The compendium holds types, not objects.
				if (asset.type == ASSET_TYPE_PHYSICSLIBRARY && static_cast<const PhysicsLibrary*>(asset.header)->isGlobalTypeCompendium)
				{
					continue;
				}

				const auto dump = dump_asset_data(asset.type, asset.header);
				for (const auto record : tagfile_records(dump))
				{
					const std::span bytes(reinterpret_cast<const std::uint8_t*>(record.data()), record.size());
					const auto format = find_asset_format(asset.type);
					const auto fail = [&](const std::string& what, const std::span<const std::uint8_t> written = {})
					{
						char debug[MAX_PATH]{};
						if (GetEnvironmentVariableA("IW8_DUMP_DEBUG", debug, sizeof(debug)))
						{
							std::ofstream(std::filesystem::path(debug) / "havok_first.hkx", std::ios::binary).write(record.data(), static_cast<std::streamsize>(record.size()));
							std::ofstream(std::filesystem::path(debug) / "havok_again.hkx", std::ios::binary).write(reinterpret_cast<const char*>(written.data()), static_cast<std::streamsize>(written.size()));
						}

						return std::runtime_error(std::format("{} \"{}\": {}", format ? format->name : "?", key.second, what));
					};

					// Tagfiles of other type compendiums (cloth) name types that this compendium does not have.
					if (const auto reference = havok::compendium_reference(bytes); reference && *reference != havok::replay_compendium_signature)
					{
						other++;
						continue;
					}

					havok::tagfile file;
					try
					{
						file = havok::read_tagfile(bytes, &compendium);
					}
					catch (const havok::unknown_type&)
					{
						other++;
						continue;
					}
					catch (const std::exception& e)
					{
						throw fail(e.what());
					}

					if (file.compendium)
					{
						const auto written = havok::write_compact_tagfile(file, compendium);
						if (!std::ranges::equal(written, bytes))
						{
							throw fail("the compact tagfile is written differently", written);
						}

						compact++;
						continue;
					}

					const auto written = havok::write_self_describing_tagfile(file, compendium);
					const auto again = havok::read_tagfile(written, &compendium);
					const auto same_items = std::ranges::equal(again.items, file.items, [](const havok::item& a, const havok::item& b)
					{
						return a.type == b.type && a.flags == b.flags && a.count == b.count && a.data == b.data &&
							std::ranges::equal(a.pointers, b.pointers, [](const havok::pointer_field& x, const havok::pointer_field& y)
							{
								return x.offset == y.offset && x.pointer_type == y.pointer_type && x.target == y.target;
							});
					});

					if (!same_items)
					{
						throw fail("the self-describing tagfile reads back with other items", written);
					}

					self_describing++;
				}
			}

			ZONETOOL_INFO("%s: %zu compact and %zu self-describing Havok tagfiles write again (%zu with other types)", zone.name.data(),
				compact, self_describing, other);
		}

		// IW8_REWRITE=convex builds each convex polytope shape of the zone again from its vertices. The two hulls must hold
		// each other's vertices, and the built shape must write and read back as the same polytope.
		void verify_convex(const std::filesystem::path& zone_folder, const loaded_zone& zone)
		{
			const auto& compendium = replay_compendium(zone_folder);
			const auto convex_type = *compendium.find("hknpConvexPolytopeShape");
			const auto bipyramid = [](const std::size_t ring_vertices)
			{
				constexpr double pi = 3.14159265358979323846;
				std::vector<havok::vector3> points;
				points.reserve(ring_vertices + 2);
				for (std::size_t i = 0; i < ring_vertices; i++)
				{
					const auto angle = 2.0 * pi * static_cast<double>(i) / static_cast<double>(ring_vertices);
					points.push_back({static_cast<float>(100.0 * std::cos(angle)), static_cast<float>(100.0 * std::sin(angle)), 0.0f});
				}
				points.push_back({0.0f, 0.0f, 100.0f});
				points.push_back({0.0f, 0.0f, -100.0f});
				auto result = havok::convex_hull(points, 0.0f);
				if (result.vertices.size() != points.size() || result.faces.size() != ring_vertices * 2)
				{
					throw std::runtime_error("the convex vertex-limit fixture did not produce a full bipyramid");
				}

				return result;
			};

			const auto boundary = bipyramid(250);
			havok::object_builder boundary_builder(compendium);
			const auto boundary_shape = havok::add_convex_polytope_shape(boundary_builder, boundary);
			const auto boundary_tag = havok::write_compact_tagfile(
				boundary_builder.finish("20180200", havok::replay_compendium_signature), compendium);
			const auto boundary_readback = havok::read_tagfile(boundary_tag, &compendium);
			const auto boundary_polytope = havok::read_convex_polytope_shape(boundary_readback, compendium, boundary_shape);
			if (boundary_shape != 1 || boundary_polytope.vertices != boundary.vertices || boundary_polytope.faces != boundary.faces ||
				boundary_polytope.planes != boundary.planes)
			{
				throw std::runtime_error("the valid 252-vertex convex boundary fixture did not round-trip");
			}

			for (const auto ring_vertices : {251u, 254u})
			{
				const auto oversized = bipyramid(ring_vertices);
				havok::object_builder oversized_builder(compendium);
				bool rejected = false;
				try
				{
					havok::add_convex_polytope_shape(oversized_builder, oversized);
				}
				catch (const std::runtime_error&)
				{
					rejected = true;
				}

				if (!rejected)
				{
					throw std::runtime_error(std::format("the {}-vertex convex boundary fixture was not rejected by the native limit",
						oversized.vertices.size()));
				}
			}

			std::set<asset_key> seen;
			std::size_t shapes{};
			std::size_t same_counts{};
			double worst{};
			for (const auto& asset : zone.assets)
			{
				const asset_key key{asset.type, asset_name(asset.type, asset.header)};
				if (asset.referenced || !seen.emplace(key).second)
				{
					continue;
				}

				if (asset.type == ASSET_TYPE_PHYSICSLIBRARY && static_cast<const PhysicsLibrary*>(asset.header)->isGlobalTypeCompendium)
				{
					continue;
				}

				const auto dump = dump_asset_data(asset.type, asset.header);
				for (const auto record : tagfile_records(dump))
				{
					const std::span bytes(reinterpret_cast<const std::uint8_t*>(record.data()), record.size());
					if (const auto reference = havok::compendium_reference(bytes); reference && *reference != havok::replay_compendium_signature)
					{
						continue;
					}

					havok::tagfile file;
					try
					{
						file = havok::read_tagfile(bytes, &compendium);
					}
					catch (const havok::unknown_type&)
					{
						continue;
					}

					for (std::uint32_t index = 1; index < file.items.size(); index++)
					{
						if (file.items[index].type != convex_type)
						{
							continue;
						}

						const auto original = havok::read_convex_polytope_shape(file, compendium, index);
						havok::polytope built;
						try
						{
							built = havok::convex_hull(original.vertices, 0.0f);
						}
						catch (const std::exception&)
						{
							char debug[MAX_PATH]{};
							if (GetEnvironmentVariableA("IW8_DUMP_DEBUG", debug, sizeof(debug)))
							{
								std::ofstream(std::filesystem::path(debug) / "convex.hkx", std::ios::binary).write(record.data(), static_cast<std::streamsize>(record.size()));
								std::ofstream(std::filesystem::path(debug) / "convex_item.txt") << index;
							}

							throw;
						}

						// Each hull holds the vertices of the other.
						const auto outside = [](const havok::polytope& hull, const std::vector<havok::vector3>& points)
						{
							double result{};
							for (const auto& plane : hull.planes)
							{
								for (const auto& p : points)
								{
									result = std::max(result, static_cast<double>(plane[0]) * p[0] + static_cast<double>(plane[1]) * p[1] +
										static_cast<double>(plane[2]) * p[2] + plane[3]);
								}
							}

							return result;
						};

						const auto distance = std::max(outside(original, built.vertices), outside(built, original.vertices));
						worst = std::max(worst, distance);
						if (distance > 1e-3)
						{
							char debug[MAX_PATH]{};
							if (GetEnvironmentVariableA("IW8_DUMP_DEBUG", debug, sizeof(debug)))
							{
								std::ofstream(std::filesystem::path(debug) / "convex.hkx", std::ios::binary).write(record.data(), static_cast<std::streamsize>(record.size()));
								std::ofstream text(std::filesystem::path(debug) / "convex.txt");
								for (const auto& v : original.vertices)
								{
									text << std::format("v {} {} {}\n", v[0], v[1], v[2]);
								}

								for (std::size_t f = 0; f < built.faces.size(); f++)
								{
									text << "f";
									for (const auto v : built.faces[f])
									{
										text << ' ' << v;
									}

									text << std::format(" | {} {} {} {}\n", built.planes[f][0], built.planes[f][1], built.planes[f][2], built.planes[f][3]);
								}

								for (const auto& v : built.vertices)
								{
									text << std::format("b {} {} {}\n", v[0], v[1], v[2]);
								}
							}

							const auto format = find_asset_format(asset.type);
							throw std::runtime_error(std::format("{} \"{}\": convex shape {} differs by {:.6f}", format ? format->name : "?",
								key.second, index, distance));
						}

						same_counts += built.vertices.size() == original.vertices.size() && built.faces.size() == original.faces.size();

						havok::object_builder builder(compendium);
						const auto shape = havok::add_convex_polytope_shape(builder, built);
						const auto written = havok::write_compact_tagfile(builder.finish(file.sdk_version, havok::replay_compendium_signature), compendium);
						const auto again = havok::read_tagfile(written, &compendium);
						const auto read = havok::read_convex_polytope_shape(again, compendium, 1);
						if (shape != 1 || read.vertices != built.vertices || read.faces != built.faces || read.planes != built.planes)
						{
							throw std::runtime_error("a built convex shape reads back differently");
						}

						shapes++;
					}
				}
			}

			ZONETOOL_INFO("%s: %zu convex shapes build again (%zu with the same vertex and face counts), largest distance %.6f",
				zone.name.data(), shapes, same_counts, worst);
		}

		template <typename T>
		T item_value(const havok::item& item, const std::size_t offset)
		{
			if (offset + sizeof(T) > item.data.size())
			{
				throw std::runtime_error("a Havok item is shorter than its members");
			}

			T value{};
			std::memcpy(&value, item.data.data() + offset, sizeof(T));
			return value;
		}

		std::uint32_t item_target(const havok::item& item, const std::uint32_t offset)
		{
			const auto found = std::ranges::find(item.pointers, offset, &havok::pointer_field::offset);
			return found != item.pointers.end() ? found->target : 0;
		}

		// IW8_REWRITE=fxshape builds the Havok data of each PhysicsFXShape of the zone again, as the IW7 converter builds it.
		// From the stock levels (convex shapes, scales, mass properties) the tagfile must be the stock one; from the vertices of
		// the first level alone the levels must agree with the stock ones.
		void verify_fxshape(const std::filesystem::path& zone_folder, const loaded_zone& zone)
		{
			const auto& compendium = replay_compendium(zone_folder);
			const auto tuple_type = *compendium.find("hkTuple<hkArray<T*<hknpConvexShape>, hkContainerHeapAllocator>, hkArray<float, hkContainerHeapAllocator>, void, void, void, void, void, void>");
			char log[8]{};
			const auto verbose = GetEnvironmentVariableA("IW8_FXSHAPE_LOG", log, sizeof(log)) != 0;
			std::size_t shapes{};
			std::size_t levels_checked{};
			double worst_vertex{};
			double worst_volume{};
			double worst_centre{};
			double worst_inertia{};
			const auto convex_type = *compendium.find("hknpConvexPolytopeShape");
			std::size_t penetration_levels{};
			std::size_t penetration_same{};
			std::size_t penetration_close{};
			int penetration_worst{};
			std::size_t radii{};
			std::size_t with_radius{};
			std::size_t without_indices{};
			std::size_t angles{};
			std::size_t angles_same{};
			std::size_t angles_close{};
			std::size_t differing{};
			std::size_t vertex_edges{};
			std::size_t vertex_edges_same{};
			std::size_t masses{};
			std::size_t masses_same{};
			for (const auto& asset : zone.assets)
			{
				if (asset.type != ASSET_TYPE_PHYSICS_FX_SHAPE || asset.referenced)
				{
					continue;
				}

				const auto* shape = static_cast<const PhysicsFXShape*>(asset.header);
				const std::string name = shape->name ? shape->name : "?";
				const std::span bytes(reinterpret_cast<const std::uint8_t*>(shape->havokData), shape->havokDataSize);
				const auto file = havok::read_tagfile(bytes, &compendium);
				if (file.items.size() < 2 || file.items[1].type != tuple_type)
				{
					throw std::runtime_error(std::format("physics shape \"{}\" has a root of another type", name));
				}

				const auto& tuple = file.items[1];
				const auto& list = file.items.at(item_target(tuple, 0));
				const auto& scales = file.items.at(item_target(tuple, 16));
				std::vector<havok::fx_shape_level> stock;
				std::vector<std::uint16_t> penetrations;
				std::vector<float> level_radii;
				bool indexed_vertices = true;
				havok::convex_options options;
				for (std::uint32_t s = 0; s < list.count; s++)
				{
					const auto index = item_target(list, s * 8);
					const auto& convex = file.items.at(index);
					const auto member = [&](const std::string_view member_name)
					{
						return compendium.member(convex.type, member_name).value().first;
					};

					const havok::convex_options level_options{item_value<std::uint16_t>(convex, member("flags")),
						item_value<std::uint8_t>(convex, member("dispatchType")), item_value<std::uint64_t>(convex, member("userData")),
						item_value<float>(convex, member("convexRadius"))};
					radii += level_options.convex_radius != 0.0f;
					if (s && (level_options.flags != options.flags || level_options.dispatch_type != options.dispatch_type ||
						level_options.user_data != options.user_data))
					{
						throw std::runtime_error(std::format("physics shape \"{}\" has levels with other headers", name));
					}

					options = level_options;
					const auto mass = havok::read_mass_properties(file, compendium, index);
					if (!mass)
					{
						throw std::runtime_error(std::format("physics shape \"{}\" has a level without mass properties", name));
					}

					stock.push_back({item_value<float>(scales, s * 4), havok::read_convex_polytope_shape(file, compendium, index), *mass});
					level_radii.push_back(level_options.convex_radius);
					penetrations.push_back(item_value<std::uint16_t>(convex, member("maxAllowedPenetration")));
					const auto& vertices = file.items.at(item_target(convex, member("vertices")));
					for (std::uint32_t v = 0; v < vertices.count; v++)
					{
						indexed_vertices &= (item_value<std::uint32_t>(vertices, v * 16 + 12) & 0xFFFFFF00u) == 0x3F000000u;
					}
				}

				// The w of each vertex holds its index (0.5 + index) in the shapes of the Replay tools.
				if (!indexed_vertices)
				{
					without_indices++;
					continue;
				}

				// maxAllowedPenetration: a third of the inner radius, as the builder gives it; the tagfile compares with the stock values.
				for (std::size_t level = 0; level < stock.size(); level++)
				{
					havok::object_builder probe(compendium);
					const auto item = havok::add_convex_polytope_shape(probe, stock[level].shape);
					const auto ours = item_value<std::uint16_t>(probe.finish(file.sdk_version, {}, item).items[1],
						compendium.member(convex_type, "maxAllowedPenetration").value().first);
					const auto ulps = std::abs(static_cast<int>(ours) - static_cast<int>(penetrations[level]));
					penetration_same += ulps == 0;
					penetration_close += ulps <= 1;
					penetration_worst = std::max(penetration_worst, ulps);
					penetration_levels++;
					stock[level].max_allowed_penetration = penetrations[level];
				}

				// The stock tools scale the convex radius with the level; the IW7 converter builds shapes without one.
				if (std::ranges::any_of(level_radii, [](const float r) { return r != 0.0f; }))
				{
					with_radius++;
					continue;
				}

				havok::object_builder builder(compendium);
				havok::add_fx_shape(builder, stock, options);
				// As the writer orders the items.
				auto built_file = havok::read_tagfile(havok::write_self_describing_tagfile(havok::breadth_first(builder.finish(file.sdk_version, {})),
					compendium), &compendium);

				// What the builder estimates is counted, then the stock values are taken: minHalfAngle (the last byte of each face),
				// the face edge that vertexEdges names for each vertex (one of the edges that start at it), and the packed mass
				// properties (the levels compare with the stock ones below).
				const auto face_type = *compendium.find("hknpConvexPolytopeShape::Face");
				const auto links_type = *compendium.find("hknpConvexPolytopeShape::Connectivity");
				const auto mass_type = *compendium.find("hknpShapeMassProperties");
				const auto vertex_edges_offset = compendium.member(links_type, "vertexEdges").value().first;
				std::set<std::uint32_t> vertex_edge_items;
				for (const auto& entry : built_file.items)
				{
					if (entry.type == links_type)
					{
						vertex_edge_items.insert(item_target(entry, vertex_edges_offset));
					}
				}

				for (std::uint32_t index = 1; index < std::min(built_file.items.size(), file.items.size()); index++)
				{
					auto& ours = built_file.items[index];
					const auto& theirs = file.items[index];
					if (ours.type != theirs.type || ours.data.size() != theirs.data.size())
					{
						continue;
					}

					if (ours.type == face_type)
					{
						for (std::size_t at = 3; at < ours.data.size(); at += 4)
						{
							const auto steps = std::abs(static_cast<int>(ours.data[at]) - static_cast<int>(theirs.data[at]));
							angles_same += steps == 0;
							angles_close += steps <= 2;
							angles++;
							ours.data[at] = theirs.data[at];
						}
					}
					else if (ours.type == mass_type)
					{
						masses_same += ours.data == theirs.data;
						masses++;
						ours.data = theirs.data;
					}
					else if (vertex_edge_items.contains(index))
					{
						for (std::size_t at = 0; at + 4 <= ours.data.size(); at += 4)
						{
							vertex_edges_same += std::equal(ours.data.begin() + at, ours.data.begin() + at + 4, theirs.data.begin() + at);
							vertex_edges++;
						}

						ours.data = theirs.data;
					}
				}

				const auto built = havok::write_self_describing_tagfile(built_file, compendium);
				const auto expected = havok::write_self_describing_tagfile(file, compendium);
				if (built != expected)
				{
					const auto again = havok::read_tagfile(built, &compendium);
					std::string difference = std::format("{} items, stock {}", again.items.size(), file.items.size());
					for (std::size_t index = 1; index < std::min(again.items.size(), file.items.size()); index++)
					{
						const auto& a = again.items[index];
						const auto& b = file.items[index];
						if (a.type != b.type || a.count != b.count || a.data.size() != b.data.size() || a.pointers.size() != b.pointers.size())
						{
							difference += std::format("; item {} is {} x{} (stock {} x{})", index, compendium.identity(a.type), a.count,
								compendium.identity(b.type), b.count);
							break;
						}

						if (a.data != b.data)
						{
							const auto at = static_cast<std::size_t>(std::ranges::mismatch(a.data, b.data).in1 - a.data.begin());
							std::string member_name = "?";
							for (auto type = a.type; type; type = compendium.at(type).parent)
							{
								std::uint32_t best{};
								for (const auto& m : compendium.at(type).members)
								{
									if (m.offset <= at && m.offset >= best)
									{
										best = m.offset;
										member_name = compendium.identity(type) + "::" + m.name;
									}
								}

								if (member_name != "?")
								{
									break;
								}
							}

							difference += std::format("; item {} ({}) differs at byte {} (after member {}): {:02X}{:02X} (stock {:02X}{:02X})", index,
								compendium.identity(a.type), at, member_name, a.data[at + 1 < a.data.size() ? at + 1 : at], a.data[at],
								b.data[at + 1 < b.data.size() ? at + 1 : at], b.data[at]);
							break;
						}

						if (!std::ranges::equal(a.pointers, b.pointers, [](const auto& x, const auto& y) { return x.offset == y.offset &&
							x.pointer_type == y.pointer_type && x.target == y.target; }))
						{
							difference += std::format("; item {} ({}) points elsewhere", index, compendium.identity(a.type));
							break;
						}
					}

					ZONETOOL_INFO("  physics shape \"%s\" builds differently: %s", name.data(), difference.data());
					if (verbose)
					{
						for (std::size_t index = 1; index < std::min(again.items.size(), file.items.size()); index++)
						{
							const auto& a = again.items[index];
							const auto& b = file.items[index];
							std::string pa, pb;
							for (const auto& x : a.pointers) pa += std::format(" {}@{}", x.target, x.offset);
							for (const auto& x : b.pointers) pb += std::format(" {}@{}", x.target, x.offset);
							ZONETOOL_INFO("    %zu: %s x%u%s %s| stock %s x%u%s", index, compendium.identity(a.type).data(), a.count, pa.data(),
								a.data == b.data ? "" : "(data) ", compendium.identity(b.type).data(), b.count, pb.data());
						}
					}
					differing++;
				}

				// The levels from the vertices of the first.
				const auto ours = havok::fx_shape_levels(stock.front().shape.vertices);
				if (ours.size() != stock.size())
				{
					throw std::runtime_error(std::format("physics shape \"{}\" has {} levels", name, stock.size()));
				}

				const auto relative = [](const double a, const double b)
				{
					return std::abs(a - b) / std::max(std::abs(b), 1e-12);
				};

				for (std::size_t level = 0; level < stock.size(); level++)
				{
					const auto& a = ours[level];
					const auto& b = stock[level];
					if (a.scale != b.scale)
					{
						throw std::runtime_error(std::format("physics shape \"{}\" level {} has scale {} (stock {})", name, level, a.scale, b.scale));
					}

					auto extent = 0.0;
					for (std::size_t axis = 0; axis < 3; axis++)
					{
						const auto [low, high] = std::ranges::minmax(b.shape.vertices | std::views::transform([axis](const havok::vector3& v) { return v[axis]; }));
						extent = std::max(extent, static_cast<double>(high) - low);
					}

					// Each stock vertex is a vertex of ours.
					auto vertex = 0.0;
					for (const auto& v : b.shape.vertices)
					{
						auto nearest = std::numeric_limits<double>::max();
						for (const auto& w : a.shape.vertices)
						{
							nearest = std::min(nearest, std::hypot(static_cast<double>(v[0]) - w[0], static_cast<double>(v[1]) - w[1],
								static_cast<double>(v[2]) - w[2]));
						}

						vertex = std::max(vertex, nearest / extent);
					}

					auto centre = 0.0;
					for (std::size_t axis = 0; axis < 3; axis++)
					{
						centre = std::max(centre, std::abs(static_cast<double>(a.mass.centre[axis]) - b.mass.centre[axis]) / extent);
					}

					auto ours_moments = a.mass.inertia;
					auto stock_moments = b.mass.inertia;
					std::ranges::sort(ours_moments);
					std::ranges::sort(stock_moments);
					auto inertia = 0.0;
					for (std::size_t axis = 0; axis < 3; axis++)
					{
						inertia = std::max(inertia, relative(ours_moments[axis], stock_moments[axis]));
					}

					const auto volume = std::max(relative(a.mass.volume, b.mass.volume), relative(a.mass.mass, b.mass.mass));
					if (verbose)
					{
						ZONETOOL_INFO("  %s level %zu scale %g: %zu/%zu vertices, vertex %.2g volume %.2g centre %.2g inertia %.2g (mass %g, stock %g)",
							name.data(), level, b.scale, a.shape.vertices.size(), b.shape.vertices.size(), vertex, volume, centre, inertia,
							a.mass.mass, b.mass.mass);
					}

					// The first level of the stock shapes keeps another centre of mass than its other levels.
					worst_vertex = std::max(worst_vertex, vertex);
					worst_volume = std::max(worst_volume, volume);
					worst_centre = std::max(worst_centre, level ? centre : 0.0);
					worst_inertia = std::max(worst_inertia, inertia);
					levels_checked++;
				}

				shapes++;
			}

			ZONETOOL_INFO("%s: %zu physics shapes build again (%zu with a convex radius and %zu without vertex indices are left out); %zu levels from the hulls of their "
				"vertices: vertices within %.2g, mass and volume within %.2g, centre within %.2g of the extent (after the first level), inertia "
				"within %.2g; maxAllowedPenetration the same for %zu of %zu levels, within one step for %zu, at most %d steps; minHalfAngle "
				"the same for %zu of %zu faces, within two steps for %zu; vertexEdges the same for %zu of %zu vertices; packed mass "
				"properties the same for %zu of %zu levels", zone.name.data(), shapes, with_radius, without_indices, levels_checked, worst_vertex,
				worst_volume, worst_centre, worst_inertia, penetration_same, penetration_levels, penetration_close, penetration_worst,
				angles_same, angles, angles_close, vertex_edges_same, vertex_edges, masses_same, masses);
			if (differing)
			{
				throw std::runtime_error(std::format("{} physics shapes build differently", differing));
			}
		}


		// The instances that the leaves of a SIMD tree hold, leaf by leaf from the left. With check_bounds, each inner slot must
		// hold the bounds of its child node.
		std::vector<std::uint32_t> tree_instances(const havok::item& nodes, const bool check_bounds)
		{
			const auto bound = [&](const std::uint32_t node, const std::size_t side, const std::size_t slot)
			{
				return item_value<float>(nodes, node * 128 + side * 16 + slot * 4);
			};

			const auto is_leaf = [&](const std::uint32_t node)
			{
				return item_value<std::uint8_t>(nodes, node * 128 + 112) != 0;
			};

			// The used slots of a node.
			const auto slots = [&](const std::uint32_t node)
			{
				std::vector<std::pair<std::size_t, std::uint32_t>> used;
				for (std::size_t slot = 0; slot < 4; slot++)
				{
					const auto data = item_value<std::uint32_t>(nodes, node * 128 + 96 + slot * 4);
					if (is_leaf(node) ? data != 0xFFFFFFFF : data != 0)
					{
						used.emplace_back(slot, data);
					}
				}

				return used;
			};

			std::vector<std::uint32_t> instances;
			std::vector<std::uint32_t> pending{1};
			while (!pending.empty())
			{
				const auto node = pending.back();
				pending.pop_back();
				if (node == 0 || node >= nodes.count)
				{
					throw std::runtime_error("a SIMD tree node is outside the tree");
				}

				const auto used = slots(node);
				if (is_leaf(node))
				{
					for (const auto& [slot, instance] : used)
					{
						instances.push_back(instance);
					}

					continue;
				}

				for (auto entry = used.rbegin(); entry != used.rend(); ++entry)
				{
					const auto [slot, child] = *entry;
					if (child >= nodes.count)
					{
						throw std::runtime_error("a SIMD tree node is outside the tree");
					}

					for (std::size_t side = 0; side < 6 && check_bounds; side++)
					{
						std::optional<float> expected;
						for (const auto& [child_slot, data] : slots(child))
						{
							const auto value = bound(child, side, child_slot);
							expected = !expected ? value : side % 2 ? std::max(*expected, value) : std::min(*expected, value);
						}

						if (expected != bound(node, side, slot))
						{
							throw std::runtime_error("a SIMD tree node does not hold the bounds of its child");
						}
					}

					pending.push_back(child);
				}
			}

			return instances;
		}

		// IW8_REWRITE=compound builds each compound shape of the zone again from its instances and child shapes. The built
		// shape must have the same header, instances, bounds and radius, and a tree that holds each instance once.
		void verify_compound(const std::filesystem::path& zone_folder, const loaded_zone& zone)
		{
			const auto& compendium = replay_compendium(zone_folder);
			const auto compound_type = *compendium.find("hknpCompoundShape");
			const auto convex_type = *compendium.find("hknpConvexShape");
			const auto polytope_type = *compendium.find("hknpConvexPolytopeShape");
			const auto offset_of = [&](const std::uint32_t type, const std::string_view name)
			{
				const auto found = compendium.member(type, name);
				if (!found)
				{
					throw std::runtime_error(std::format("{} has no member {}", compendium.identity(type), name));
				}

				return found->first;
			};

			// Capsules and spheres are convex shapes with a radius about their vertices, and the Replay tools give them
			// larger bounds than the radius (0.0314788 for a radius of 0.03125).
			const auto is_polytope = [&](std::uint32_t type)
			{
				for (std::size_t depth = 0; type && depth < 64; depth++)
				{
					if (type == polytope_type)
					{
						return true;
					}

					type = compendium.at(type).parent;
				}

				return false;
			};

			const auto elements_at = offset_of(compound_type, "instances.elements");
			const auto aabb_at = offset_of(compound_type, "aabb");
			const auto radius_at = offset_of(compound_type, "boundingRadius");
			const auto volume_type_at = offset_of(compound_type, "boundingVolumeData.type");
			const auto nodes_at = offset_of(compound_type, "boundingVolumeData.simdTree.nodes");
			const auto points_at = offset_of(compound_type, "boundingVolumeData.simdTree.points");
			const auto vertices_at = offset_of(convex_type, "vertices");
			const auto convex_radius_at = offset_of(convex_type, "convexRadius");

			std::set<asset_key> seen;
			std::size_t shapes{};
			std::size_t other_children{};
			std::size_t other_volumes{};
			std::size_t same_trees{};
			std::size_t same_orders{};
			std::size_t same_bounds{};
			double worst_bounds{};
			double worst_radius{};
			for (const auto& asset : zone.assets)
			{
				const asset_key key{asset.type, asset_name(asset.type, asset.header)};
				if (asset.referenced || !seen.emplace(key).second)
				{
					continue;
				}

				if (asset.type == ASSET_TYPE_PHYSICSLIBRARY && static_cast<const PhysicsLibrary*>(asset.header)->isGlobalTypeCompendium)
				{
					continue;
				}

				const auto dump = dump_asset_data(asset.type, asset.header);
				for (const auto record : tagfile_records(dump))
				{
					const std::span bytes(reinterpret_cast<const std::uint8_t*>(record.data()), record.size());
					if (const auto reference = havok::compendium_reference(bytes); reference && *reference != havok::replay_compendium_signature)
					{
						continue;
					}

					havok::tagfile file;
					try
					{
						file = havok::read_tagfile(bytes, &compendium);
					}
					catch (const havok::unknown_type&)
					{
						continue;
					}

					for (std::uint32_t index = 1; index < file.items.size(); index++)
					{
						const auto& stock = file.items[index];
						if (stock.type != compound_type)
						{
							continue;
						}

						const auto format = find_asset_format(asset.type);
						const auto fail = [&](const std::string& what)
						{
							char debug[MAX_PATH]{};
							if (GetEnvironmentVariableA("IW8_DUMP_DEBUG", debug, sizeof(debug)))
							{
								std::ofstream(std::filesystem::path(debug) / "compound.hkx", std::ios::binary).write(record.data(), static_cast<std::streamsize>(record.size()));
								std::ofstream(std::filesystem::path(debug) / "compound_item.txt") << index;
							}

							return std::runtime_error(std::format("{} \"{}\": compound shape {}: {}", format ? format->name : "?", key.second, index, what));
						};

						if (item_value<std::uint8_t>(stock, volume_type_at) != 2)
						{
							other_volumes++;
							continue;
						}

						const auto& elements = file.items.at(item_target(stock, elements_at));
						havok::object_builder builder(compendium);
						std::unordered_map<std::uint32_t, std::uint32_t> copies;
						std::vector<havok::compound_instance> instances;
						auto convex_children = true;
						for (std::uint32_t i = 0; i < elements.count && convex_children; i++)
						{
							const auto child_index = item_target(elements, i * 112 + 80);
							const auto& child = file.items.at(child_index);
							if (!is_polytope(child.type))
							{
								convex_children = false;
								break;
							}

							havok::compound_instance instance;
							for (std::size_t column = 0; column < 3; column++)
							{
								for (std::size_t axis = 0; axis < 3; axis++)
								{
									instance.rotation[column][axis] = item_value<float>(elements, i * 112 + column * 16 + axis * 4);
								}
							}

							for (std::size_t axis = 0; axis < 3; axis++)
							{
								instance.translation[axis] = item_value<float>(elements, i * 112 + 48 + axis * 4);
							}

							instance.shape_tag = item_value<std::uint16_t>(elements, i * 112 + 88);
							instance.convex_radius = item_value<float>(child, convex_radius_at);
							const auto& vertices = file.items.at(item_target(child, vertices_at));
							for (std::uint32_t v = 0; v < vertices.count; v++)
							{
								instance.vertices.push_back({item_value<float>(vertices, v * 16), item_value<float>(vertices, v * 16 + 4),
									item_value<float>(vertices, v * 16 + 8)});
							}

							instance.shape = havok::copy_item(builder, file, child_index, copies);
							instances.push_back(std::move(instance));
						}

						if (!convex_children)
						{
							other_children++;
							continue;
						}

						const auto shape = havok::add_compound_shape(builder, instances);
						const auto built = builder.finish(file.sdk_version, havok::replay_compendium_signature, shape);
						const auto& ours = built.items[1];

						// flags, type, numShapeKeyBits, dispatchType, convexRadius, userData; shapeTagCodecInfo; instances
						// firstFree and numAllocated; isMutable, estimatedNumShapeKeys; isCompact and the volume type.
						for (const auto [begin, end] : {std::pair(24, 48), std::pair(56, 60), std::pair(88, 96), std::pair(148, 156),
							std::pair(256, 257), std::pair(280, 281)})
						{
							if (!std::equal(ours.data.begin() + begin, ours.data.begin() + end, stock.data.begin() + begin))
							{
								throw fail(std::format("bytes 0x{:X}-0x{:X} differ", begin, end));
							}
						}

						const auto& our_elements = built.items.at(item_target(ours, elements_at));
						for (std::uint32_t i = 0; i < elements.count; i++)
						{
							// All but the shape pointer and the tree position in the w of the translation.
							for (const auto [begin, end] : {std::pair(0u, 60u), std::pair(64u, 80u), std::pair(88u, 112u)})
							{
								if (!std::equal(our_elements.data.begin() + i * 112 + begin, our_elements.data.begin() + i * 112 + end,
									elements.data.begin() + i * 112 + begin))
								{
									throw fail(std::format("instance {} differs in bytes 0x{:X}-0x{:X}", i, begin, end));
								}
							}
						}

						auto bounds_equal = true;
						for (std::size_t k = 0; k < 8; k++)
						{
							const auto a = item_value<float>(ours, aabb_at + k * 4);
							const auto b = item_value<float>(stock, aabb_at + k * 4);
							bounds_equal &= a == b;
							worst_bounds = std::max(worst_bounds, static_cast<double>(std::abs(a - b)));
						}

						same_bounds += bounds_equal;
						const auto radius = item_value<float>(stock, radius_at);
						const auto radius_error = std::abs(static_cast<double>(item_value<float>(ours, radius_at)) - radius) / std::max(radius, 1e-6f);
						worst_radius = std::max(worst_radius, radius_error);
						if (worst_bounds > 1e-4 || radius_error > 1e-5)
						{
							throw fail(std::format("the bounds differ by {:.7f}, the radius by {:.7f} of it", worst_bounds, radius_error));
						}

						const auto& our_nodes = built.items.at(item_target(ours, nodes_at));
						const auto& stock_nodes = file.items.at(item_target(stock, nodes_at));
						const auto our_order = tree_instances(our_nodes, true);
						const auto stock_order = tree_instances(stock_nodes, false);
						auto held = our_order;
						std::ranges::sort(held);
						if (held.size() != elements.count || std::ranges::adjacent_find(held) != held.end() || held.back() >= elements.count)
						{
							throw fail("the tree does not hold each instance once");
						}

						same_orders += our_order == stock_order;
						if (our_order != stock_order)
						{
							char debug[MAX_PATH]{};
							if (GetEnvironmentVariableA("IW8_DUMP_DEBUG", debug, sizeof(debug)))
							{
								std::ofstream text(std::filesystem::path(debug) / "compound_orders.txt", std::ios::app);
								text << std::format("{} {} n {} stock", key.second, index, elements.count);
								for (const auto i : stock_order)
								{
									text << ' ' << i;
								}

								text << " ours";
								for (const auto i : our_order)
								{
									text << ' ' << i;
								}

								const auto& points = file.items.at(item_target(stock, points_at));
								text << " points";
								for (std::uint32_t k = 0; k < points.count; k++)
								{
									text << std::format(" {:.9g},{:.9g},{:.9g},{:X}", item_value<float>(points, k * 16), item_value<float>(points, k * 16 + 4),
										item_value<float>(points, k * 16 + 8), item_value<std::uint32_t>(points, k * 16 + 12));
								}

								text << '\n';
							}
						}
						same_trees += our_nodes.data == stock_nodes.data &&
							built.items.at(item_target(ours, points_at)).data == file.items.at(item_target(stock, points_at)).data;

						const auto written = havok::write_compact_tagfile(built, compendium);
						if (havok::write_compact_tagfile(havok::read_tagfile(written, &compendium), compendium) != written)
						{
							throw fail("the built shape reads back differently");
						}

						shapes++;
					}
				}
			}

			ZONETOOL_INFO("%s: %zu compound shapes build again (%zu with the same tree, %zu the same leaf order, %zu the same bounds); "
				"largest bounds difference %.7f, radius %.7f; %zu with other children, %zu with other trees", zone.name.data(), shapes, same_trees,
				same_orders, same_bounds, worst_bounds, worst_radius, other_children, other_volumes);
		}

		// The triangles of `expected` that `found` has (same shape tag and winding, vertices within the tolerance).
		std::size_t matching_triangles(const havok::decoded_mesh& expected, const havok::decoded_mesh& found, const float tolerance,
			std::vector<bool>* found_used = nullptr)
		{
			const auto cell_of = [](const std::array<havok::vector3, 3>& triangle)
			{
				std::array<std::int64_t, 3> cell{};
				for (std::size_t axis = 0; axis < 3; axis++)
				{
					cell[axis] = static_cast<std::int64_t>(std::floor((triangle[0][axis] + triangle[1][axis] + triangle[2][axis]) / 3.0f));
				}

				return cell;
			};

			const auto key_of = [](const std::array<std::int64_t, 3>& cell)
			{
				return (cell[0] * 73856093) ^ (cell[1] * 19349663) ^ (cell[2] * 83492791);
			};

			std::unordered_multimap<std::int64_t, std::size_t> grid;
			for (std::size_t t = 0; t < found.triangles.size(); t++)
			{
				grid.emplace(key_of(cell_of(found.triangles[t])), t);
			}

			std::vector<bool> used(found.triangles.size());
			std::size_t matched{};
			for (std::size_t t = 0; t < expected.triangles.size(); t++)
			{
				// The closest unused triangle within the tolerance.
				const auto& want = expected.triangles[t];
				const auto cell = cell_of(want);
				std::optional<std::size_t> best;
				auto best_distance = tolerance;
				for (std::int64_t dx = -1; dx <= 1; dx++)
				{
					for (std::int64_t dy = -1; dy <= 1; dy++)
					{
						for (std::int64_t dz = -1; dz <= 1; dz++)
						{
							const auto [begin, end] = grid.equal_range(key_of({cell[0] + dx, cell[1] + dy, cell[2] + dz}));
							for (auto entry = begin; entry != end; ++entry)
							{
								const auto& have = found.triangles[entry->second];
								if (used[entry->second] || found.triangle_tags[entry->second] != expected.triangle_tags[t])
								{
									continue;
								}

								for (std::size_t turn = 0; turn < 3; turn++)
								{
									float distance{};
									for (std::size_t corner = 0; corner < 3; corner++)
									{
										for (std::size_t axis = 0; axis < 3; axis++)
										{
											distance = std::max(distance, std::abs(have[(corner + turn) % 3][axis] - want[corner][axis]));
										}
									}

									if (distance <= best_distance)
									{
										best = entry->second;
										best_distance = distance;
									}
								}
							}
						}
					}
				}

				if (best)
				{
					used[*best] = true;
					matched++;
				}
			}

			if (found_used)
			{
				*found_used = std::move(used);
			}

			return matched;
		}

		// IW8_REWRITE=mesh decodes each compressed mesh shape of the zone and checks its trees, builds it again from the
		// decoded triangles and convex shapes without the game, and decodes the built mesh: it must hold the same triangles
		// (within the vertex quantization) and convex shapes, and write and read back the same.
		void verify_mesh(const std::filesystem::path& zone_folder, const loaded_zone& zone)
		{
			const auto& compendium = replay_compendium(zone_folder);
			const auto packed_boundary = []
			{
				havok::compressed_mesh mesh;
				mesh.vertices.reserve(256);
				mesh.triangles.reserve(128);
				for (std::uint32_t y = 0; y < 8; y++)
				{
					for (std::uint32_t x = 0; x < 8; x++)
					{
						const auto first = static_cast<std::uint32_t>(mesh.vertices.size());
						const auto left = static_cast<float>(x) * 0.125f;
						const auto bottom = static_cast<float>(y) * 0.125f;
						mesh.vertices.insert(mesh.vertices.end(), {{left, bottom, 0.0f}, {left + 0.1f, bottom, 0.0f},
							{left + 0.1f, bottom + 0.1f, 0.0f}, {left, bottom + 0.1f, 0.0f}});
						const auto tag = static_cast<std::uint16_t>(y * 8 + x);
						mesh.triangles.push_back({{first, first + 1, first + 2}, tag});
						mesh.triangles.push_back({{first, first + 2, first + 3}, tag});
					}
				}
				return mesh;
			}();
			havok::object_builder boundary_builder(compendium);
			havok::compressed_mesh_stats boundary_stats;
			const auto boundary_shape = havok::add_compressed_mesh_shape(boundary_builder, packed_boundary, &boundary_stats);
			const auto boundary = boundary_builder.finish("20180200", havok::replay_compendium_signature, boundary_shape);
			const auto boundary_bytes = havok::write_compact_tagfile(boundary, compendium);
			const auto boundary_file = havok::read_tagfile(boundary_bytes, &compendium);
			const auto boundary_readback = havok::read_compressed_mesh_shape(boundary_file, compendium, boundary_shape, true);
			havok::decoded_mesh boundary_expected;
			for (const auto& triangle : packed_boundary.triangles)
			{
				boundary_expected.triangles.push_back({packed_boundary.vertices[triangle.vertices[0]], packed_boundary.vertices[triangle.vertices[1]],
					packed_boundary.vertices[triangle.vertices[2]]});
				boundary_expected.triangle_tags.push_back(triangle.shape_tag);
			}
			if (boundary_stats.sections != 1 || boundary_stats.packed_vertices != 255 || boundary_stats.shared_vertices != 1 ||
				boundary_stats.collapsed != 0 ||
				boundary_readback.triangles.size() != 128 || matching_triangles(boundary_expected, boundary_readback,
					boundary_stats.largest_error * 1.01f + 1e-6f) != 128)
			{
				throw std::runtime_error("the 256-entry compressed-mesh packed-vertex boundary did not round-trip");
			}

			const auto mesh_type = *compendium.find("hknpCompressedMeshShape");
			std::set<asset_key> seen;
			std::size_t meshes{};
			std::size_t triangles{};
			std::size_t convex_shapes{};
			std::size_t removed{};
			std::size_t degenerate{};
			std::size_t stock_bytes{};
			std::size_t built_bytes{};
			std::size_t other_primitives{};
			std::size_t relaxed{};
			float largest_error{};
			for (const auto& asset : zone.assets)
			{
				const asset_key key{asset.type, asset_name(asset.type, asset.header)};
				if (asset.referenced || !seen.emplace(key).second)
				{
					continue;
				}

				if (asset.type == ASSET_TYPE_PHYSICSLIBRARY && static_cast<const PhysicsLibrary*>(asset.header)->isGlobalTypeCompendium)
				{
					continue;
				}

				const auto dump = dump_asset_data(asset.type, asset.header);
				for (const auto record : tagfile_records(dump))
				{
					const std::span bytes(reinterpret_cast<const std::uint8_t*>(record.data()), record.size());
					if (const auto reference = havok::compendium_reference(bytes); reference && *reference != havok::replay_compendium_signature)
					{
						continue;
					}

					havok::tagfile file;
					try
					{
						file = havok::read_tagfile(bytes, &compendium);
					}
					catch (const havok::unknown_type&)
					{
						continue;
					}

					for (std::uint32_t index = 1; index < file.items.size(); index++)
					{
						if (file.items[index].type != mesh_type)
						{
							continue;
						}

						const auto format = find_asset_format(asset.type);
						const auto fail = [&](const std::string& what)
						{
							char debug[MAX_PATH]{};
							if (GetEnvironmentVariableA("IW8_DUMP_DEBUG", debug, sizeof(debug)))
							{
								std::ofstream(std::filesystem::path(debug) / "mesh.hkx", std::ios::binary).write(record.data(), static_cast<std::streamsize>(record.size()));
								std::ofstream(std::filesystem::path(debug) / "mesh_item.txt") << index;
							}

							return std::runtime_error(std::format("{} \"{}\": compressed mesh {}: {}", format ? format->name : "?", key.second, index, what));
						};

						havok::decoded_mesh stock;
						try
						{
							stock = havok::read_compressed_mesh_shape(file, compendium, index);
						}
						catch (const std::exception& e)
						{
							throw fail(std::format("the stock mesh: {}", e.what()));
						}

						if (std::ranges::find(stock.convex_keys, 0xFFFFFFFF) != stock.convex_keys.end())
						{
							throw fail("a stock convex shape has no primitive");
						}

						// Other custom primitives (convex pieces of shared vertices) are not built.
						if (stock.custom)
						{
							other_primitives++;
							continue;
						}

						// The decoded triangles, with one vertex per position.
						havok::object_builder builder(compendium);
						havok::compressed_mesh mesh;
						mesh.convex_radius = stock.convex_radius;
						mesh.user_data = stock.user_data;
						std::map<havok::vector3, std::uint32_t> vertex_index;
						for (std::size_t t = 0; t < stock.triangles.size(); t++)
						{
							havok::mesh_triangle triangle;
							triangle.shape_tag = stock.triangle_tags[t];
							for (std::size_t corner = 0; corner < 3; corner++)
							{
								const auto& p = stock.triangles[t][corner];
								const auto [found, added] = vertex_index.try_emplace(p, static_cast<std::uint32_t>(mesh.vertices.size()));
								if (added)
								{
									mesh.vertices.push_back(p);
								}

								triangle.vertices[corner] = found->second;
							}

							mesh.triangles.push_back(triangle);
						}

						std::unordered_map<std::uint32_t, std::uint32_t> copies;
						for (std::size_t i = 0; i < stock.convex_shapes.size(); i++)
						{
							auto instance = stock.convex_shapes[i];
							instance.shape = havok::copy_item(builder, file, stock.convex_items[i], copies);
							mesh.convex_shapes.push_back(std::move(instance));
						}

						havok::compressed_mesh_stats stats;
						const auto shape = havok::add_compressed_mesh_shape(builder, mesh, &stats);
						const auto built = builder.finish(file.sdk_version, havok::replay_compendium_signature, shape);
						havok::decoded_mesh ours;
						try
						{
							ours = havok::read_compressed_mesh_shape(built, compendium, 1, true);
						}
						catch (const std::exception& e)
						{
							throw fail(std::format("the built mesh: {}", e.what()));
						}

						// Triangles without an area are left out.
						std::size_t areas{};
						for (const auto& triangle : stock.triangles)
						{
							const auto& [a, b, c] = triangle;
							const std::array<double, 3> ab{static_cast<double>(b[0]) - a[0], static_cast<double>(b[1]) - a[1], static_cast<double>(b[2]) - a[2]};
							const std::array<double, 3> ac{static_cast<double>(c[0]) - a[0], static_cast<double>(c[1]) - a[1], static_cast<double>(c[2]) - a[2]};
							areas += std::abs(ab[1] * ac[2] - ab[2] * ac[1]) + std::abs(ab[2] * ac[0] - ab[0] * ac[2]) + std::abs(ab[0] * ac[1] - ab[1] * ac[0]) > 0.0;
						}

						degenerate += stock.triangles.size() - areas;
						// The writer leaves out the triangles that its vertex grids collapse.
						if (ours.triangles.size() + stats.collapsed != areas)
						{
							throw fail(std::format("the built mesh has {} triangles ({} collapsed), the stock mesh {} with an area",
								ours.triangles.size(), stats.collapsed, areas));
						}

						// A vertex moves by the largest quantization error of the built mesh at most.
						const auto tolerance = stats.largest_error * 1.01f + 1e-6f;
						largest_error = std::max(largest_error, stats.largest_error);
						relaxed += stats.tolerance > mesh.packed_tolerance;
						const auto matched = matching_triangles(stock, ours, tolerance);
						if (matched + stats.collapsed != areas)
						{
							char debug[MAX_PATH]{};
							if (GetEnvironmentVariableA("IW8_DUMP_DEBUG", debug, sizeof(debug)))
							{
								for (const auto& [name, mesh_data] : {std::pair("mesh_stock.txt", &stock), std::pair("mesh_built.txt", &ours)})
								{
									std::ofstream text(std::filesystem::path(debug) / name);
									for (std::size_t t = 0; t < mesh_data->triangles.size(); t++)
									{
										const auto& tri = mesh_data->triangles[t];
										text << std::format("{} {} {} {} {} {} {} {} {} {}\n", tri[0][0], tri[0][1], tri[0][2], tri[1][0], tri[1][1], tri[1][2], tri[2][0],
											tri[2][1], tri[2][2], mesh_data->triangle_tags[t]);
									}
								}
							}

							throw fail(std::format("{} of {} triangles of the built mesh match the stock mesh", matched, areas));
						}

						if (ours.convex_shapes.size() != stock.convex_shapes.size() || std::ranges::find(ours.convex_keys, 0xFFFFFFFF) != ours.convex_keys.end())
						{
							throw fail("the built mesh does not hold each convex shape once");
						}

						for (std::size_t i = 0; i < ours.convex_shapes.size(); i++)
						{
							const auto& a = ours.convex_shapes[i];
							const auto& b = stock.convex_shapes[i];
							if (a.rotation != b.rotation || a.translation != b.translation || a.shape_tag != b.shape_tag || a.vertices != b.vertices)
							{
								throw fail(std::format("convex shape {} differs", i));
							}
						}

						const auto written = havok::write_compact_tagfile(built, compendium);
						if (havok::write_compact_tagfile(havok::read_tagfile(written, &compendium), compendium) != written)
						{
							throw fail("the built mesh reads back differently");
						}

						meshes++;
						triangles += areas;
						convex_shapes += stock.convex_shapes.size();
						removed += stock.removed;
						stock_bytes += record.size();
						built_bytes += written.size();
					}
				}
			}

			ZONETOOL_INFO("%s: %zu compressed meshes build again (%zu triangles, %zu convex shapes; %zu removed and %zu degenerate stock "
				"primitives left out); tagfiles %zu bytes, built %zu; %zu with other custom primitives; largest vertex error %.6f, %zu meshes "
				"with a larger packing tolerance", zone.name.data(), meshes, triangles, convex_shapes, removed, degenerate, stock_bytes, built_bytes,
				other_primitives, largest_error, relaxed);
		}

		std::uint32_t member_offset(const havok::type_table& types, const std::uint32_t type, const std::string_view name)
		{
			const auto found = types.member(type, name);
			if (!found)
			{
				throw std::runtime_error(std::format("{} has no member {}", types.identity(type), name));
			}

			return found->first;
		}

		std::string item_string(const havok::tagfile& file, const std::uint32_t index)
		{
			if (!index)
			{
				return {};
			}

			const auto& text = file.items.at(index).data;
			return std::string(reinterpret_cast<const char*>(text.data()), strnlen(reinterpret_cast<const char*>(text.data()), text.size()));
		}

		// The input of a compressed mesh that holds the decoded geometry, with one vertex per position.
		havok::compressed_mesh mesh_input(const havok::decoded_mesh& decoded)
		{
			havok::compressed_mesh mesh;
			mesh.convex_radius = decoded.convex_radius;
			mesh.user_data = decoded.user_data;
			std::map<havok::vector3, std::uint32_t> vertex_index;
			for (std::size_t t = 0; t < decoded.triangles.size(); t++)
			{
				havok::mesh_triangle triangle;
				triangle.shape_tag = decoded.triangle_tags[t];
				for (std::size_t corner = 0; corner < 3; corner++)
				{
					const auto& p = decoded.triangles[t][corner];
					const auto [found, added] = vertex_index.try_emplace(p, static_cast<std::uint32_t>(mesh.vertices.size()));
					if (added)
					{
						mesh.vertices.push_back(p);
					}

					triangle.vertices[corner] = found->second;
				}

				mesh.triangles.push_back(triangle);
			}

			mesh.convex_shapes = decoded.convex_shapes;
			return mesh;
		}

		struct rebuilt_shapes
		{
			// Built item -> the largest vertex error of a built compressed mesh.
			std::unordered_map<std::uint32_t, float> mesh_errors;
			// Built item -> the triangles that the vertex grids of a built compressed mesh collapse (left out).
			std::unordered_map<std::uint32_t, std::size_t> mesh_collapsed;
			std::size_t built{};
			std::size_t copied{};
			std::size_t masses{};
			double mass_error{};
		};

		// The inertia tensor of mass properties: R diag(inertia) R^T.
		std::array<std::array<double, 3>, 3> inertia_tensor(const havok::mass_properties& properties)
		{
			const auto [x, y, z, w] = properties.rotation;
			const std::array<std::array<double, 3>, 3> r{{{1 - 2.0 * (y * y + z * z), 2.0 * (x * y - z * w), 2.0 * (x * z + y * w)},
				{2.0 * (x * y + z * w), 1 - 2.0 * (x * x + z * z), 2.0 * (y * z - x * w)},
				{2.0 * (x * z - y * w), 2.0 * (y * z + x * w), 1 - 2.0 * (x * x + y * y)}}};
			std::array<std::array<double, 3>, 3> tensor{};
			for (std::size_t i = 0; i < 3; i++)
			{
				for (std::size_t j = 0; j < 3; j++)
				{
					for (std::size_t k = 0; k < 3; k++)
					{
						tensor[i][j] += r[i][k] * properties.inertia[k] * r[j][k];
					}
				}
			}

			return tensor;
		}

		// The largest difference of two mass properties relative to their size: volume, centre (to the size of the shape)
		// and inertia tensor.
		double mass_difference(const havok::mass_properties& a, const havok::mass_properties& b)
		{
			auto result = std::abs(static_cast<double>(a.volume) - b.volume) / std::max(std::abs(static_cast<double>(a.volume)), 1e-9);
			const auto size = std::cbrt(std::abs(static_cast<double>(a.volume)));
			for (std::size_t i = 0; i < 3; i++)
			{
				result = std::max(result, std::abs(static_cast<double>(a.centre[i]) - b.centre[i]) / std::max(size, 1e-6));
			}

			const auto x = inertia_tensor(a);
			const auto y = inertia_tensor(b);
			double largest{};
			for (const auto& row : x)
			{
				for (const auto value : row)
				{
					largest = std::max(largest, std::abs(value));
				}
			}

			for (std::size_t i = 0; i < 3; i++)
			{
				for (std::size_t j = 0; j < 3; j++)
				{
					result = std::max(result, std::abs(x[i][j] - y[i][j]) / std::max(largest, 1e-12));
				}
			}

			return result;
		}

		// A shape of a tagfile built again without the game: compressed meshes from their geometry, compound shapes with a SIMD
		// tree from their instances, convex polytopes from their vertices. Other shapes are copied, as are shape properties.
		std::uint32_t rebuild_shape(havok::object_builder& builder, const havok::tagfile& file, const havok::type_table& types,
			const std::uint32_t index, std::unordered_map<std::uint32_t, std::uint32_t>& copies, rebuilt_shapes& rebuilt)
		{
			if (const auto found = copies.find(index); found != copies.end())
			{
				return found->second;
			}

			const auto& shape = file.items.at(index);
			const auto identity = types.identity(shape.type);
			const auto properties = item_target(shape, member_offset(types, shape.type, "properties"));
			const auto flags = item_value<std::uint16_t>(shape, member_offset(types, shape.type, "flags"));
			const auto dispatch = item_value<std::uint8_t>(shape, member_offset(types, shape.type, "dispatchType"));
			const auto radius = item_value<float>(shape, member_offset(types, shape.type, "convexRadius"));
			const auto user_data = item_value<std::uint64_t>(shape, member_offset(types, shape.type, "userData"));
			std::optional<std::uint32_t> built;
			std::optional<havok::mass_properties> masses;
			if (identity == "hknpConvexPolytopeShape" && radius == 0.0f)
			{
				const auto source = havok::read_convex_polytope_shape(file, types, index);
				const auto hull = havok::convex_hull(source.vertices, 0.0f);
				built = havok::add_convex_polytope_shape(builder, hull, {flags, dispatch, user_data});

				// Mass properties are built from the hull and must decode close to the stock ones.
				if (const auto stock = havok::read_mass_properties(file, types, index))
				{
					const auto ours = havok::convex_mass_properties(hull, stock->mass);
					havok::add_mass_properties(builder, *built, ours);
					masses = ours;
					rebuilt.masses++;

					// Some stock shapes keep no inertia, and flat ones are thin slabs of their own.
					const auto flat = havok::inner_radius(hull) <= std::numeric_limits<float>::epsilon();
					if (stock->inertia != havok::vector3{} && !flat)
					{
						rebuilt.mass_error = std::max(rebuilt.mass_error, mass_difference(*stock, ours));
					}
				}
			}
			else if (identity == "hknpCompressedMeshShape")
			{
				const auto decoded = havok::read_compressed_mesh_shape(file, types, index);
				if (!decoded.custom)
				{
					auto mesh = mesh_input(decoded);
					for (std::size_t i = 0; i < mesh.convex_shapes.size(); i++)
					{
						mesh.convex_shapes[i].shape = rebuild_shape(builder, file, types, decoded.convex_items[i], copies, rebuilt);
					}

					havok::compressed_mesh_stats stats;
					built = havok::add_compressed_mesh_shape(builder, mesh, &stats);
					rebuilt.mesh_errors[*built] = stats.largest_error;
					rebuilt.mesh_collapsed[*built] = stats.collapsed;
				}
			}
			else if (identity == "hknpCompoundShape" &&
				item_value<std::uint8_t>(shape, member_offset(types, shape.type, "boundingVolumeData.type")) == 2)
			{
				const auto& elements = file.items.at(item_target(shape, member_offset(types, shape.type, "instances.elements")));
				std::vector<havok::compound_instance> instances;
				for (std::uint32_t i = 0; i < elements.count; i++)
				{
					const auto child = item_target(elements, i * 112 + 80);
					const auto& child_item = file.items.at(child);
					if (types.identity(child_item.type) != "hknpConvexPolytopeShape")
					{
						instances.clear();
						break;
					}

					havok::compound_instance instance;
					for (std::size_t column = 0; column < 3; column++)
					{
						for (std::size_t axis = 0; axis < 3; axis++)
						{
							instance.rotation[column][axis] = item_value<float>(elements, i * 112 + column * 16 + axis * 4);
						}
					}

					for (std::size_t axis = 0; axis < 3; axis++)
					{
						instance.translation[axis] = item_value<float>(elements, i * 112 + 48 + axis * 4);
					}

					instance.shape_tag = item_value<std::uint16_t>(elements, i * 112 + 88);
					instance.shape = rebuild_shape(builder, file, types, child, copies, rebuilt);
					// The bounds come from the hull that was built.
					instance.vertices = havok::read_convex_polytope_shape(file, types, child).vertices;
					instances.push_back(std::move(instance));
				}

				if (!instances.empty())
				{
					built = havok::add_compound_shape(builder, instances);
				}
			}

			if (!built)
			{
				rebuilt.copied++;
				return havok::copy_item(builder, file, index, copies);
			}

			rebuilt.built++;
			if (properties && !masses)
			{
				builder.point(*built, "properties", havok::copy_item(builder, file, properties, copies));
			}

			copies.emplace(index, *built);
			return *built;
		}

		// The geometry of two shapes must agree: compressed meshes their triangles (within the vertex error of the built mesh)
		// and convex shapes, compound shapes their instances, convex polytopes their bounds.
		void compare_shapes(const havok::tagfile& stock_file, const std::uint32_t stock, const havok::tagfile& built_file, const std::uint32_t built,
			const havok::type_table& types, const float tolerance, const std::size_t collapsed = 0)
		{
			const auto& a = stock_file.items.at(stock);
			const auto& b = built_file.items.at(built);
			const auto identity = types.identity(a.type);
			if (a.type != b.type)
			{
				throw std::runtime_error(std::format("a {} became a {}", identity, types.identity(b.type)));
			}

			if (identity == "hknpCompressedMeshShape")
			{
				const auto expected = havok::read_compressed_mesh_shape(stock_file, types, stock);
				const auto found = havok::read_compressed_mesh_shape(built_file, types, built, true);
				if (expected.custom)
				{
					return;
				}

				const auto matched = matching_triangles(expected, found, tolerance * 1.01f + 1e-6f);
				// The writer leaves out the triangles that its vertex grids collapse.
				if (matched + collapsed != expected.triangles.size() || found.triangles.size() + collapsed != expected.triangles.size() ||
					found.convex_shapes.size() != expected.convex_shapes.size())
				{
					throw std::runtime_error(std::format("a compressed mesh has {} of {} triangles and {} of {} convex shapes", matched,
						expected.triangles.size(), found.convex_shapes.size(), expected.convex_shapes.size()));
				}

				for (std::size_t i = 0; i < expected.convex_shapes.size(); i++)
				{
					const auto& x = expected.convex_shapes[i];
					const auto& y = found.convex_shapes[i];
					if (x.rotation != y.rotation || x.translation != y.translation || x.shape_tag != y.shape_tag)
					{
						throw std::runtime_error(std::format("convex shape {} of a compressed mesh moved", i));
					}
				}
			}
			else if (identity == "hknpConvexPolytopeShape")
			{
				const auto x = havok::instance_bounds({.vertices = havok::read_convex_polytope_shape(stock_file, types, stock).vertices});
				const auto y = havok::instance_bounds({.vertices = havok::read_convex_polytope_shape(built_file, types, built).vertices});
				for (std::size_t k = 0; k < 6; k++)
				{
					if (std::abs(x[k] - y[k]) > 1e-3f)
					{
						throw std::runtime_error("a convex polytope has other bounds");
					}
				}
			}
		}

		struct physics_counts
		{
			std::size_t lists{};
			std::size_t other_lists{};
			std::size_t assets{};
			std::size_t other_assets{};
			std::size_t lods{};
			std::size_t other_lods{};
			std::size_t lod_differences{};
			std::string first_lod_difference;
			std::size_t other_values{};
			std::string first_difference;
			rebuilt_shapes shapes;
		};

		// A HavokPhysicsShapeList built again: the same entries and tags, shapes with the same geometry.
		void verify_shape_list(const havok::tagfile& file, const havok::type_table& types, physics_counts& counts)
		{
			const auto& root = file.items.at(1);
			const auto array_of = [&](const std::string_view name) -> const havok::item*
			{
				const auto target = item_target(root, member_offset(types, root.type, name));
				return target ? &file.items.at(target) : nullptr;
			};

			const auto* shapes = array_of("shapes");
			const auto count = shapes ? shapes->count : 0;
			const auto values = [&](const std::string_view name, const std::uint32_t index)
			{
				const auto* list = array_of(name);
				return list && index < list->count ? item_value<std::uint32_t>(*list, index * 4) : 0u;
			};

			// Some stock lists keep more rows than shapes; the writer keeps one row per shape.
			for (const auto name : {"shapeIndices", "shapeNames", "vertCounts", "triCounts", "shapeContents", "convexCounts"})
			{
				const auto* list = array_of(name);
				if ((list ? list->count : 0) != count)
				{
					counts.other_lists++;
					return;
				}
			}

			havok::object_builder builder(types);
			std::unordered_map<std::uint32_t, std::uint32_t> copies;
			std::vector<havok::shape_list_entry> entries;
			for (std::uint32_t i = 0; i < count; i++)
			{
				havok::shape_list_entry entry;
				entry.shape = rebuild_shape(builder, file, types, item_target(*shapes, i * 8), copies, counts.shapes);
				const auto* names = array_of("shapeNames");
				entry.name = names ? item_string(file, item_target(*names, i * 8)) : std::string{};
				entry.index = static_cast<std::int32_t>(values("shapeIndices", i));
				entry.vertex_count = values("vertCounts", i);
				entry.triangle_count = values("triCounts", i);
				entry.contents = values("shapeContents", i);
				entry.convex_count = values("convexCounts", i);
				if (const auto* bounds = array_of("minMaxes"))
				{
					for (std::size_t axis = 0; axis < 3; axis++)
					{
						entry.minimum[axis] = item_value<float>(*bounds, i * 32 + axis * 4);
						entry.maximum[axis] = item_value<float>(*bounds, i * 32 + 16 + axis * 4);
					}
				}

				entries.push_back(std::move(entry));
			}

			std::vector<havok::shape_tag> tags;
			if (const auto* data = array_of("shapeTagData"))
			{
				for (std::uint32_t i = 0; i < data->count; i++)
				{
					tags.push_back({item_value<std::uint32_t>(*data, i * 24), item_value<std::uint32_t>(*data, i * 24 + 4),
						item_value<std::uint16_t>(*data, i * 24 + 8), item_value<std::uint64_t>(*data, i * 24 + 16)});
				}
			}

			const auto list = havok::add_shape_list(builder, entries, tags);
			auto built = builder.finish(file.sdk_version, {}, list);
			const auto written = havok::write_self_describing_tagfile(built, types);
			const auto again = havok::read_tagfile(written, &types);

			// The arrays of the list must hold the same values.
			const auto& built_root = again.items.at(1);
			for (const auto name : {"shapeIndices", "vertCounts", "triCounts", "minMaxes", "shapeTagData", "shapeContents", "convexCounts"})
			{
				const auto offset = member_offset(types, root.type, name);
				const auto stock_target = item_target(root, offset);
				const auto built_target = item_target(built_root, offset);
				const auto stock_data = stock_target ? file.items.at(stock_target).data : std::vector<std::uint8_t>{};
				const auto built_data = built_target ? again.items.at(built_target).data : std::vector<std::uint8_t>{};
				if (stock_data != built_data)
				{
					throw std::runtime_error(std::format("the shape list {} differ", name));
				}
			}

			const auto* built_names = again.items.at(item_target(built_root, member_offset(types, root.type, "shapeNames"))).count ?
				&again.items.at(item_target(built_root, member_offset(types, root.type, "shapeNames"))) : nullptr;
			const auto& built_shapes = again.items.at(item_target(built_root, member_offset(types, root.type, "shapes")));
			for (std::uint32_t i = 0; i < count; i++)
			{
				if (!built_names || item_string(again, item_target(*built_names, i * 8)) != entries[i].name)
				{
					throw std::runtime_error(std::format("shape list entry {} has another name", i));
				}

				const auto built_shape = item_target(built_shapes, i * 8);
				const auto error = counts.shapes.mesh_errors.contains(entries[i].shape) ? counts.shapes.mesh_errors.at(entries[i].shape) : 0.0f;
				const auto collapsed = counts.shapes.mesh_collapsed.contains(entries[i].shape) ? counts.shapes.mesh_collapsed.at(entries[i].shape) : std::size_t{};
				compare_shapes(file, item_target(*shapes, i * 8), again, built_shape, types, error, collapsed);
			}

			counts.lists++;
		}

		// A one-body HavokPhysicsAsset built again from its values: the objects other than the shape must have the same bytes.
		void verify_physics_asset(const havok::tagfile& file, const havok::type_table& types, physics_counts& counts)
		{
			const auto& root = file.items.at(1);
			const auto system_index = item_target(root, member_offset(types, root.type, "physicsSystemData"));
			if (!system_index)
			{
				counts.other_assets++;
				return;
			}

			const auto& system = file.items.at(system_index);
			const auto target_of = [&](const havok::item& owner, const std::string_view name) -> const havok::item*
			{
				const auto target = item_target(owner, member_offset(types, owner.type, name));
				return target ? &file.items.at(target) : nullptr;
			};

			const auto* bodies = target_of(system, "bodyCinfos");
			const auto* materials = target_of(system, "materials");
			const auto* motions = target_of(system, "motionProperties");
			const auto one = [&](const std::string_view name)
			{
				const auto* list = target_of(root, name);
				return list && list->count == 1;
			};

			if (!bodies || bodies->count != 1 || target_of(system, "constraintCinfos") || (materials && materials->count != 1) ||
				(motions && motions->count != 1) || !one("bodyQualityNameCRCLookup") || !one("materialNameCRCLookup") ||
				!one("motionPropertiesNameCRCLookup") || !one("bodySFXAssetNames") || !one("bodyVFXAssetNames") || !one("bodyServerUsage") ||
				!one("bodyDrivers") || !one("simulationCategories") || target_of(root, "constraintServerUsage") || target_of(root, "mutable") ||
				target_of(root, "mutableShapeNames") || !!materials != !!motions)
			{
				counts.other_assets++;
				return;
			}

			const auto first = [&](const std::string_view name)
			{
				return item_value<std::uint32_t>(*target_of(root, name), 0);
			};

			const auto body_member = [&](const std::string_view name)
			{
				return member_offset(types, bodies->type, name);
			};

			havok::physics_asset asset;
			asset.use_category = item_value<std::int32_t>(root, member_offset(types, root.type, "useCategory"));
			asset.body_quality_crc = first("bodyQualityNameCRCLookup");
			asset.material_crc = first("materialNameCRCLookup");
			asset.motion_properties_crc = first("motionPropertiesNameCRCLookup");
			asset.server_usage = static_cast<std::int32_t>(first("bodyServerUsage"));
			asset.body_driver = static_cast<std::int32_t>(first("bodyDrivers"));
			asset.simulation_category = static_cast<std::int32_t>(first("simulationCategories"));
			asset.system_properties = materials != nullptr;
			auto& body = asset.body;
			body.name = item_string(file, item_target(*bodies, body_member("name")));
			body.collision_filter_info = item_value<std::uint32_t>(*bodies, body_member("collisionFilterInfo"));
			body.material_id = item_value<std::uint16_t>(*bodies, body_member("materialId"));
			body.quality_id = item_value<std::uint8_t>(*bodies, body_member("qualityId"));
			body.motion_type = item_value<std::uint8_t>(*bodies, body_member("motionType"));
			body.mass = item_value<float>(*bodies, body_member("mass"));
			body.motion_properties_id = item_value<std::uint16_t>(*bodies, body_member("motionPropertiesId"));
			body.activation_priority = item_value<std::int8_t>(*bodies, body_member("activationPriority"));

			havok::object_builder builder(types);
			std::unordered_map<std::uint32_t, std::uint32_t> copies;
			const auto stock_shape = item_target(*bodies, body_member("shape"));
			body.shape = rebuild_shape(builder, file, types, stock_shape, copies, counts.shapes);
			const auto error = counts.shapes.mesh_errors.contains(body.shape) ? counts.shapes.mesh_errors.at(body.shape) : 0.0f;
			const auto collapsed = counts.shapes.mesh_collapsed.contains(body.shape) ? counts.shapes.mesh_collapsed.at(body.shape) : std::size_t{};
			const auto built_root = havok::add_physics_asset(builder, asset);
			const auto written = havok::write_compact_tagfile(builder.finish(file.sdk_version, havok::replay_compendium_signature, built_root), types);
			const auto again = havok::read_tagfile(written, &types);

			// The bytes of each object without its pointer fields.
			const auto bytes_of = [&](const havok::tagfile& source, const havok::item* object)
			{
				if (!object)
				{
					return std::vector<std::uint8_t>{};
				}

				auto data = object->data;
				for (const auto& pointer : object->pointers)
				{
					std::fill_n(data.begin() + pointer.offset, std::min<std::size_t>(8, data.size() - pointer.offset), 0);
				}

				(void)source;
				return data;
			};

			const auto& built_asset = again.items.at(1);
			const auto& built_system = again.items.at(item_target(built_asset, member_offset(types, root.type, "physicsSystemData")));
			const auto built_target = [&](const havok::item& owner, const std::string_view name) -> const havok::item*
			{
				const auto target = item_target(owner, member_offset(types, owner.type, name));
				return target ? &again.items.at(target) : nullptr;
			};

			const auto* built_body = built_target(built_system, "bodyCinfos");
			for (const auto& [name, x, y] : {std::tuple("asset", &root, &built_asset), std::tuple("system", &system, &built_system),
				std::tuple("material", materials, built_target(built_system, "materials")),
				std::tuple("motion properties", motions, built_target(built_system, "motionProperties")), std::tuple("body", bodies, built_body)})
			{
				const auto a = bytes_of(file, x);
				const auto b = bytes_of(again, y);
				if (a != b)
				{
					const auto at = std::ranges::mismatch(a, b).in1 - a.begin();
					if (counts.first_difference.empty())
					{
						counts.first_difference = std::format("{} at 0x{:X} ({} and {} bytes)", name, at, a.size(), b.size());
					}

					counts.other_values++;
					return;
				}
			}

			compare_shapes(file, stock_shape, again, item_target(*built_body, body_member("shape")), types, error, collapsed);
			counts.assets++;
		}

		// A one-body HavokPhysicsXModelLOD built again from its values: the objects other than the shape must have the same bytes.
		void verify_xmodel_lod(const havok::tagfile& file, const havok::type_table& types, physics_counts& counts)
		{
			const auto& root = file.items.at(1);
			const auto target_of = [&](const havok::item& owner, const std::string_view name) -> const havok::item*
			{
				const auto target = item_target(owner, member_offset(types, owner.type, name));
				return target ? &file.items.at(target) : nullptr;
			};

			const auto* shapes = target_of(root, "shapes");
			const auto* names = target_of(root, "bodyNames");
			const auto* poses = target_of(root, "bodyBindPoses");
			const auto* tags = target_of(root, "shapeTagData");
			if (!shapes || shapes->count != 1 || !names || names->count != 1 || !poses || poses->count != 1 || !tags || tags->count != 1)
			{
				counts.other_lods++;
				return;
			}

			havok::xmodel_lod_body body;
			body.name = item_string(file, item_target(*names, 0));
			body.surface_flags = item_value<std::uint32_t>(*tags, 0);
			body.collision_filter_info = item_value<std::uint32_t>(*tags, 4);
			for (std::size_t i = 0; i < body.bind_pose.size(); i++)
			{
				body.bind_pose[i] = item_value<float>(*poses, i * 4);
			}

			havok::object_builder builder(types);
			std::unordered_map<std::uint32_t, std::uint32_t> copies;
			const auto stock_shape = item_target(*shapes, 0);
			body.shape = rebuild_shape(builder, file, types, stock_shape, copies, counts.shapes);
			const auto error = counts.shapes.mesh_errors.contains(body.shape) ? counts.shapes.mesh_errors.at(body.shape) : 0.0f;
			const auto collapsed = counts.shapes.mesh_collapsed.contains(body.shape) ? counts.shapes.mesh_collapsed.at(body.shape) : std::size_t{};
			const auto built_root = havok::add_xmodel_lod(builder, body);
			const auto written = havok::write_compact_tagfile(builder.finish(file.sdk_version, havok::replay_compendium_signature, built_root), types);
			const auto again = havok::read_tagfile(written, &types);

			const auto bytes_of = [](const havok::item* object)
			{
				if (!object)
				{
					return std::vector<std::uint8_t>{};
				}

				auto data = object->data;
				for (const auto& pointer : object->pointers)
				{
					std::fill_n(data.begin() + pointer.offset, std::min<std::size_t>(8, data.size() - pointer.offset), 0);
				}

				return data;
			};

			const auto& built = again.items.at(1);
			const auto built_target = [&](const std::string_view name) -> const havok::item*
			{
				const auto target = item_target(built, member_offset(types, built.type, name));
				return target ? &again.items.at(target) : nullptr;
			};

			for (const auto& [name, x, y] : {std::tuple("root", &root, &built), std::tuple("bind pose", poses, built_target("bodyBindPoses")),
				std::tuple("tag data", tags, built_target("shapeTagData"))})
			{
				const auto a = bytes_of(x);
				const auto b = bytes_of(y);
				if (a != b)
				{
					if (counts.first_lod_difference.empty())
					{
						counts.first_lod_difference = std::format("{} ({} and {} bytes)", name, a.size(), b.size());
					}

					counts.lod_differences++;
					return;
				}
			}

			compare_shapes(file, stock_shape, again, item_target(*built_target("shapes"), 0), types, error, collapsed);
			counts.lods++;
		}

		// IW8_REWRITE=physics builds the Havok data of the zone again without the game: each HavokPhysicsShapeList (col_map,
		// map_ents) and each one-body HavokPhysicsAsset, with their shapes. Lists must keep their entries and tags, assets the
		// bytes of their objects, and shapes their geometry.
		void verify_physics(const std::filesystem::path& zone_folder, const loaded_zone& zone)
		{
			const auto& compendium = replay_compendium(zone_folder);
			const auto list_type = *compendium.find("HavokPhysicsShapeList");
			const auto asset_type = *compendium.find("HavokPhysicsAsset");
			const auto lod_type = *compendium.find("HavokPhysicsXModelLOD");
			std::set<asset_key> seen;
			physics_counts counts;
			for (const auto& asset : zone.assets)
			{
				const asset_key key{asset.type, asset_name(asset.type, asset.header)};
				if (asset.referenced || !seen.emplace(key).second)
				{
					continue;
				}

				if (asset.type == ASSET_TYPE_PHYSICSLIBRARY && static_cast<const PhysicsLibrary*>(asset.header)->isGlobalTypeCompendium)
				{
					continue;
				}

				const auto dump = dump_asset_data(asset.type, asset.header);
				for (const auto record : tagfile_records(dump))
				{
					const std::span bytes(reinterpret_cast<const std::uint8_t*>(record.data()), record.size());
					if (const auto reference = havok::compendium_reference(bytes); reference && *reference != havok::replay_compendium_signature)
					{
						continue;
					}

					havok::tagfile file;
					try
					{
						file = havok::read_tagfile(bytes, &compendium);
					}
					catch (const havok::unknown_type&)
					{
						continue;
					}

					if (file.items.size() < 2 || (file.items[1].type != list_type && file.items[1].type != asset_type && file.items[1].type != lod_type))
					{
						continue;
					}

					try
					{
						if (file.items[1].type == list_type)
						{
							verify_shape_list(file, compendium, counts);
						}
						else if (file.items[1].type == lod_type)
						{
							verify_xmodel_lod(file, compendium, counts);
						}
						else
						{
							verify_physics_asset(file, compendium, counts);
						}
					}
					catch (const std::exception& e)
					{
						char debug[MAX_PATH]{};
						if (GetEnvironmentVariableA("IW8_DUMP_DEBUG", debug, sizeof(debug)))
						{
							std::ofstream(std::filesystem::path(debug) / "physics.hkx", std::ios::binary).write(record.data(), static_cast<std::streamsize>(record.size()));
						}

						const auto format = find_asset_format(asset.type);
						throw std::runtime_error(std::format("{} \"{}\": {}", format ? format->name : "?", key.second, e.what()));
					}
				}
			}

			ZONETOOL_INFO("%s: %zu shape lists (%zu with other rows), %zu one-body physics assets and %zu one-body detail collision LODs "
				"(%zu with other bodies, %zu with other values%s%s) build again (%zu physics assets with other values%s%s, %zu of another kind); "
				"%zu shapes built, %zu copied; %zu mass properties built, largest difference %.6f", zone.name.data(), counts.lists,
				counts.other_lists, counts.assets, counts.lods, counts.other_lods, counts.lod_differences,
				counts.first_lod_difference.empty() ? "" : ", first: ", counts.first_lod_difference.data(), counts.other_values,
				counts.first_difference.empty() ? "" : ", first: ", counts.first_difference.data(), counts.other_assets,
				counts.shapes.built, counts.shapes.copied, counts.shapes.masses, counts.shapes.mass_error);
		}

		struct comparison
		{
			std::size_t same{};
			std::size_t havok{};
			std::size_t shapes{};
			float largest_distance{};
			double largest_mass_difference{};
			// Triangles that the built meshes keep beyond the reference ones.
			std::size_t extra_triangles{};
			double largest_extra_area{};
			// Assets without Havok data that differ, by type.
			std::map<std::string, std::size_t> other;
			std::string first_other;
		};

		// The largest distance from a triangle of the reference mesh to the closest triangle with the same tag and winding of the
		// built one, measured at the corners (the meshes quantize their vertices differently). The built mesh may keep more
		// triangles: small ones that the Replay tools leave out.
		float mesh_distance(const havok::decoded_mesh& built, const havok::decoded_mesh& reference, const float limit, comparison& result)
		{
			for (auto tolerance = 1.0f / 64.0f;; tolerance *= 2.0f)
			{
				std::vector<bool> used;
				if (matching_triangles(reference, built, tolerance, &used) == reference.triangles.size())
				{
					for (std::size_t t = 0; t < built.triangles.size(); t++)
					{
						if (!used[t])
						{
							const auto& [a, b, c] = built.triangles[t];
							const std::array<double, 3> ab{static_cast<double>(b[0]) - a[0], static_cast<double>(b[1]) - a[1], static_cast<double>(b[2]) - a[2]};
							const std::array<double, 3> ac{static_cast<double>(c[0]) - a[0], static_cast<double>(c[1]) - a[1], static_cast<double>(c[2]) - a[2]};
							const std::array<double, 3> n{ab[1] * ac[2] - ab[2] * ac[1], ab[2] * ac[0] - ab[0] * ac[2], ab[0] * ac[1] - ab[1] * ac[0]};
							result.extra_triangles++;
							result.largest_extra_area = std::max(result.largest_extra_area, std::sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]) / 2.0);
						}
					}

					return tolerance;
				}

				if (tolerance > limit)
				{
					throw std::runtime_error(std::format("compressed mesh triangles differ by more than {}", limit));
				}
			}
		}

		// Two shapes built from the same input must agree: compressed meshes on their triangles and convex shapes, compound
		// shapes on their instances, convex polytopes on their bounds.
		float compare_built_shapes(const havok::tagfile& a_file, const std::uint32_t a, const havok::tagfile& b_file, const std::uint32_t b,
			const havok::type_table& types, comparison& result)
		{
			const auto& x = a_file.items.at(a);
			const auto& y = b_file.items.at(b);
			const auto identity = types.identity(x.type);
			if (x.type != y.type)
			{
				throw std::runtime_error(std::format("a {} is a {} in the other zone", identity, types.identity(y.type)));
			}

			if (identity == "hknpCompressedMeshShape")
			{
				const auto first = havok::read_compressed_mesh_shape(a_file, types, a);
				const auto second = havok::read_compressed_mesh_shape(b_file, types, b);
				if (first.triangles.size() < second.triangles.size() || first.convex_shapes.size() != second.convex_shapes.size())
				{
					char debug[MAX_PATH]{};
					if (GetEnvironmentVariableA("IW8_DUMP_DEBUG", debug, sizeof(debug)))
					{
						for (const auto& [name, mesh_data] : {std::pair("compare_a.txt", &first), std::pair("compare_b.txt", &second)})
						{
							std::ofstream text(std::filesystem::path(debug) / name);
							for (std::size_t t = 0; t < mesh_data->triangles.size(); t++)
							{
								const auto& tri = mesh_data->triangles[t];
								text << std::format("{} {} {} {} {} {} {} {} {} {}\n", tri[0][0], tri[0][1], tri[0][2], tri[1][0], tri[1][1], tri[1][2],
									tri[2][0], tri[2][1], tri[2][2], mesh_data->triangle_tags[t]);
							}
						}
					}

					throw std::runtime_error(std::format("compressed meshes have {} and {} triangles, {} and {} convex shapes", first.triangles.size(),
						second.triangles.size(), first.convex_shapes.size(), second.convex_shapes.size()));
				}

				for (std::size_t i = 0; i < first.convex_shapes.size(); i++)
				{
					const auto& p = first.convex_shapes[i];
					const auto& q = second.convex_shapes[i];
					if (p.translation != q.translation || p.rotation != q.rotation || p.shape_tag != q.shape_tag)
					{
						throw std::runtime_error(std::format("convex shape {} of a compressed mesh moved", i));
					}

					compare_built_shapes(a_file, first.convex_items[i], b_file, second.convex_items[i], types, result);
				}

				return mesh_distance(first, second, 1.0f, result);
			}

			if (identity == "hknpConvexPolytopeShape")
			{
				const auto p = havok::instance_bounds({.vertices = havok::read_convex_polytope_shape(a_file, types, a).vertices});
				const auto q = havok::instance_bounds({.vertices = havok::read_convex_polytope_shape(b_file, types, b).vertices});
				float largest{};
				for (std::size_t k = 0; k < 6; k++)
				{
					largest = std::max(largest, std::abs(p[k] - q[k]));
				}

				if (largest > 0.01f)
				{
					throw std::runtime_error(std::format("convex polytopes differ by {}", largest));
				}

				// Hulls of model points: the Replay tools simplify them a little.
				const auto pm = havok::read_mass_properties(a_file, types, a);
				const auto qm = havok::read_mass_properties(b_file, types, b);
				const auto difference = pm && qm ? mass_difference(*pm, *qm) : 0.0;
				result.largest_mass_difference = std::max(result.largest_mass_difference, difference);
				if (!!pm != !!qm || difference > 0.02)
				{
					throw std::runtime_error(std::format("convex polytopes have mass properties {:.4f} apart", difference));
				}

				return largest;
			}

			if (identity == "hknpCompoundShape")
			{
				const auto elements_at = member_offset(types, x.type, "instances.elements");
				const auto& p = a_file.items.at(item_target(x, elements_at));
				const auto& q = b_file.items.at(item_target(y, elements_at));
				if (p.count != q.count)
				{
					throw std::runtime_error("compound shapes have other instance counts");
				}

				float largest{};
				for (std::uint32_t i = 0; i < p.count; i++)
				{
					// Transforms and tags, without the tree position in the translation w.
					if (!std::equal(p.data.begin() + i * 112, p.data.begin() + i * 112 + 60, q.data.begin() + i * 112) ||
						item_value<std::uint16_t>(p, i * 112 + 88) != item_value<std::uint16_t>(q, i * 112 + 88))
					{
						throw std::runtime_error(std::format("compound shape instance {} differs", i));
					}

					largest = std::max(largest, compare_built_shapes(a_file, item_target(p, i * 112 + 80), b_file, item_target(q, i * 112 + 80), types,
						result));
				}

				return largest;
			}

			return 0.0f;
		}

		// Two tagfiles built from the same input: shape lists with the same entries and tags, physics assets with the same
		// objects, and shapes that agree.
		void compare_tagfiles(const havok::tagfile& a, const havok::tagfile& b, const havok::type_table& types, comparison& result)
		{
			const auto& x = a.items.at(1);
			const auto& y = b.items.at(1);
			if (x.type != y.type)
			{
				throw std::runtime_error("the tagfiles have other root types");
			}

			const auto identity = types.identity(x.type);
			const auto array_of = [&](const havok::tagfile& file, const havok::item& root, const std::string_view name) -> const havok::item*
			{
				const auto target = item_target(root, member_offset(types, root.type, name));
				return target ? &file.items.at(target) : nullptr;
			};

			if (identity == "HavokPhysicsShapeList")
			{
				for (const auto name : {"shapeIndices", "vertCounts", "triCounts", "shapeTagData", "shapeContents", "convexCounts"})
				{
					const auto* p = array_of(a, x, name);
					const auto* q = array_of(b, y, name);
					if ((p ? p->data : std::vector<std::uint8_t>{}) != (q ? q->data : std::vector<std::uint8_t>{}))
					{
						throw std::runtime_error(std::format("the shape lists have other {}", name));
					}
				}

				const auto* p_bounds = array_of(a, x, "minMaxes");
				const auto* q_bounds = array_of(b, y, "minMaxes");
				for (std::size_t k = 0; p_bounds && q_bounds && k < p_bounds->data.size() / 4; k++)
				{
					if (std::abs(item_value<float>(*p_bounds, k * 4) - item_value<float>(*q_bounds, k * 4)) > 1e-4f)
					{
						throw std::runtime_error("the shape lists have other bounds");
					}
				}

				const auto* p_shapes = array_of(a, x, "shapes");
				const auto* q_shapes = array_of(b, y, "shapes");
				const auto* p_names = array_of(a, x, "shapeNames");
				const auto* q_names = array_of(b, y, "shapeNames");
				for (std::uint32_t i = 0; p_shapes && q_shapes && i < p_shapes->count; i++)
				{
					if (item_string(a, item_target(*p_names, i * 8)) != item_string(b, item_target(*q_names, i * 8)))
					{
						throw std::runtime_error(std::format("shape list entry {} has another name", i));
					}

					result.largest_distance = std::max(result.largest_distance, compare_built_shapes(a, item_target(*p_shapes, i * 8), b,
						item_target(*q_shapes, i * 8), types, result));
					result.shapes++;
				}

				return;
			}

			if (identity == "HavokPhysicsAsset")
			{
				// The objects other than shapes must have the same bytes, without their pointers.
				const auto bytes_of = [](const havok::item* object)
				{
					if (!object)
					{
						return std::vector<std::uint8_t>{};
					}

					auto data = object->data;
					for (const auto& pointer : object->pointers)
					{
						std::fill_n(data.begin() + pointer.offset, std::min<std::size_t>(8, data.size() - pointer.offset), 0);
					}

					return data;
				};

				const auto* p_system = array_of(a, x, "physicsSystemData");
				const auto* q_system = array_of(b, y, "physicsSystemData");
				if (bytes_of(&x) != bytes_of(&y) || bytes_of(p_system) != bytes_of(q_system))
				{
					throw std::runtime_error("the physics assets have other values");
				}

				for (const auto name : {"materials", "motionProperties", "bodyCinfos"})
				{
					if (bytes_of(p_system ? array_of(a, *p_system, name) : nullptr) != bytes_of(q_system ? array_of(b, *q_system, name) : nullptr))
					{
						throw std::runtime_error(std::format("the physics systems have other {}", name));
					}
				}

				const auto* p_bodies = p_system ? array_of(a, *p_system, "bodyCinfos") : nullptr;
				const auto* q_bodies = q_system ? array_of(b, *q_system, "bodyCinfos") : nullptr;
				const auto shape_at = p_bodies ? member_offset(types, p_bodies->type, "shape") : 0;
				const auto stride = p_bodies ? types.size_of(p_bodies->type) : 0;
				for (std::uint32_t i = 0; p_bodies && q_bodies && i < p_bodies->count; i++)
				{
					result.largest_distance = std::max(result.largest_distance, compare_built_shapes(a, item_target(*p_bodies, i * stride + shape_at), b,
						item_target(*q_bodies, i * stride + shape_at), types, result));
					result.shapes++;
				}
			}
		}

		// IW8_REWRITE=compare compares the zone with IW8_COMPARE_ZONE, built from the same input (with the Havok data of the
		// Replay executable and without it): all assets must be the same, except Havok data that must agree.
		void verify_compare(const std::filesystem::path& zone_folder, const loaded_zone& zone)
		{
			char other_path[MAX_PATH]{};
			if (!GetEnvironmentVariableA("IW8_COMPARE_ZONE", other_path, sizeof(other_path)))
			{
				throw std::runtime_error("IW8_REWRITE=compare needs IW8_COMPARE_ZONE");
			}

			// Both zones keep the data that the game drops, so that each asset dumps the same way whatever its layout.
			const auto& compendium = replay_compendium(zone_folder);
			const auto other = load_zone(std::filesystem::path(other_path), {.keep_temp_data = true});
			std::map<asset_key, const void*> other_assets;
			for (const auto& asset : other->assets)
			{
				if (!asset.referenced)
				{
					other_assets.emplace(asset_key{asset.type, asset_name(asset.type, asset.header)}, asset.header);
				}
			}

			// The zones list the same assets in the same order, with the same script strings, whatever their layout.
			const auto list_of = [](const loaded_zone& z)
			{
				std::vector<std::tuple<std::int32_t, std::string, bool>> list;
				for (const auto& entry : z.list)
				{
					list.emplace_back(entry.type, asset_name(entry.type, entry.header), entry.referenced);
				}

				return list;
			};

			const auto strings_of = [](const loaded_zone& z)
			{
				std::vector<std::optional<std::string>> strings;
				for (const auto string : z.script_strings)
				{
					strings.emplace_back(string ? std::optional<std::string>(string) : std::nullopt);
				}

				return strings;
			};

			if (list_of(zone) != list_of(*other))
			{
				throw std::runtime_error(std::format("the asset lists differ ({} and {} entries)", zone.list.size(), other->list.size()));
			}

			if (strings_of(zone) != strings_of(*other))
			{
				throw std::runtime_error(std::format("the script strings differ ({} and {})", zone.script_strings.size(),
					other->script_strings.size()));
			}

			std::set<asset_key> held;
			for (const auto& asset : zone.assets)
			{
				if (!asset.referenced)
				{
					held.emplace(asset.type, asset_name(asset.type, asset.header));
				}
			}

			for (const auto& [key, header] : other_assets)
			{
				if (!held.contains(key))
				{
					const auto format = find_asset_format(key.first);
					throw std::runtime_error(std::format("{} \"{}\" is missing from this zone", format ? format->name : "?", key.second));
				}
			}

			comparison result;
			std::set<asset_key> seen;
			for (const auto& asset : zone.assets)
			{
				const asset_key key{asset.type, asset_name(asset.type, asset.header)};
				if (asset.referenced || !seen.emplace(key).second)
				{
					continue;
				}

				const auto format = find_asset_format(asset.type);
				const auto found = other_assets.find(key);
				if (found == other_assets.end())
				{
					throw std::runtime_error(std::format("{} \"{}\" is missing from the other zone", format ? format->name : "?", key.second));
				}

				const auto dump = dump_asset_data(asset.type, asset.header);
				const auto other_dump = dump_asset_data(asset.type, found->second);
				if (dump == other_dump)
				{
					result.same++;
					continue;
				}

				// Without the Havok data, the dumps must be the same.
				auto blank = dump;
				auto other_blank = other_dump;
				const auto records = tagfile_records(dump);
				const auto other_records = tagfile_records(other_dump);
				if (records.size() != other_records.size() || records.empty())
				{
					result.other[format ? format->name : "?"]++;
					if (result.first_other.empty())
					{
						result.first_other = std::format("{} \"{}\"", format ? format->name : "?", key.second);
					}

					continue;
				}

				try
				{
					for (std::size_t r = 0; r < records.size(); r++)
					{
						const std::span x(reinterpret_cast<const std::uint8_t*>(records[r].data()), records[r].size());
						const std::span y(reinterpret_cast<const std::uint8_t*>(other_records[r].data()), other_records[r].size());
						compare_tagfiles(havok::read_tagfile(x, &compendium), havok::read_tagfile(y, &compendium), compendium, result);
					}
				}
				catch (const std::exception& e)
				{
					throw std::runtime_error(std::format("{} \"{}\": {}", format ? format->name : "?", key.second, e.what()));
				}

				result.havok++;
			}

			std::string differences;
			for (const auto& [type, count] : result.other)
			{
				differences += std::format(" {} {}", count, type);
			}

			ZONETOOL_INFO("%s: the same asset list (%zu entries) and script strings (%zu); %zu assets are the same and %zu agree on their "
				"Havok data (%zu shapes, largest vertex distance %.6f, %zu triangles kept beyond the other zone, largest area %.6f, mass "
				"properties %.4f apart); other differences:%s%s%s", zone.name.data(), zone.list.size(), zone.script_strings.size(),
				result.same, result.havok, result.shapes, result.largest_distance, result.extra_triangles, result.largest_extra_area,
				result.largest_mass_difference, differences.empty() ? " none" :
				differences.data(), result.first_other.empty() ? "" : ", first ", result.first_other.data());
		}

		// The asset list of the zone: <pool>,<name> for an asset, <pool>,,<name> for a reference.
		void write_asset_csv(const loaded_zone& zone)
		{
			std::string csv;
			for (const auto& entry : zone.list)
			{
				const auto format = find_asset_format(entry.type);
				csv += std::format("{},{}{}\n", format ? format->name : std::to_string(entry.type), entry.referenced ? "," : "",
					asset_name(entry.type, entry.header));
			}

			filesystem::file file(zone.name + ".csv");
			file.open("wb");
			file.write(csv.data(), csv.size(), 1);
			file.close();
		}
	}

	void dump_zone(const std::filesystem::path& zone_folder, const std::filesystem::path& zone_path, const converter* converter)
	{
		const auto _ = gsl::finally(db_clear);

		filesystem::set_zone_folder(zone_folder);
		filesystem::set_fastfile(zone_path.stem().string());
		ZONETOOL_INFO("Loading zone \"%s\"...", zone_path.stem().string().data());
		const auto zone = load_zone(zone_path, {.keep_temp_data = true});

		std::set<asset_key> dumped;
		for (const auto& asset : zone->assets)
		{
			const asset_key key{asset.type, asset_name(asset.type, asset.header)};
			if (asset.referenced || !dumped.emplace(key).second)
			{
				continue;
			}

			try
			{
				if (converter)
				{
					converter->dump_asset(asset.type, asset.header);
				}
				else
				{
					dump_asset(asset.type, asset.header);
				}
			}
			catch (const std::exception& e)
			{
				const auto format = find_asset_format(asset.type);
				ZONETOOL_ERROR("Failed to dump %s \"%s\": %s", format ? format->name : "?", key.second.data(), e.what());
			}
		}

		write_asset_csv(*zone);
		if (converter && converter->finish)
		{
			converter->finish(zone->name);
		}
		ZONETOOL_INFO("Zone \"%s\" dumped to %s", zone->name.data(), filesystem::get_dump_path().data());
	}

	void dump_asset(const std::filesystem::path& zone_folder, const std::filesystem::path& zone_path, const std::string& type_name,
		const std::string& name, const converter* converter)
	{
		std::int32_t type = -1;
		for (std::int32_t i = 0; i < ASSET_TYPE_COUNT && type < 0; i++)
		{
			const auto format = find_asset_format(i);
			type = format && format->name == type_name ? i : -1;
		}

		if (type < 0)
		{
			throw std::runtime_error(std::format("unknown asset type \"{}\"", type_name));
		}

		const auto _ = gsl::finally(db_clear);

		filesystem::set_zone_folder(zone_folder);
		filesystem::set_fastfile(zone_path.stem().string());
		const auto zone = load_zone(zone_path, {.keep_temp_data = true});
		const auto asset = std::ranges::find_if(zone->assets, [&](const loaded_asset& entry)
		{
			return entry.type == type && !entry.referenced && name == asset_name(entry.type, entry.header);
		});

		if (asset == zone->assets.end())
		{
			throw std::runtime_error(std::format("{} \"{}\" is not in zone \"{}\"", type_name, name, zone->name));
		}

		filesystem::set_dump_folder("assets");
		if (converter)
		{
			converter->dump_asset(asset->type, asset->header);
			if (converter->finish)
			{
				converter->finish(zone->name);
			}
		}
		else
		{
			dump_asset(asset->type, asset->header);
		}

		ZONETOOL_INFO("%s \"%s\" dumped to %s", type_name.data(), name.data(), filesystem::get_dump_path().data());
	}

	void verify_zone(const std::filesystem::path& zone_folder, const std::filesystem::path& zone_path)
	{
		const auto _ = gsl::finally(db_clear);

		char rewrite[16]{};
		const auto check_rewrite = GetEnvironmentVariableA("IW8_REWRITE", rewrite, sizeof(rewrite)) != 0;
		const auto zone = load_zone(zone_path, {.keep_temp_data = check_rewrite, .keep_markers = check_rewrite});
		std::map<std::string, std::size_t> counts;
		for (const auto& asset : zone->assets)
		{
			const auto format = find_asset_format(asset.type);
			++counts[format ? format->name : std::to_string(asset.type)];
		}

		ZONETOOL_INFO("%s: %zu assets, %zu script strings, all bytes read", zone->name.data(), zone->assets.size(),
			zone->script_strings.size());
		for (const auto& [name, count] : counts)
		{
			ZONETOOL_INFO("  %s: %zu", name.data(), count);
		}

		if (const auto folder = asset_data::neutral_output_folder())
		{
			write_neutral_files(*zone, *folder);
		}

		if (check_rewrite)
		{
			if (std::string_view(rewrite) == "fresh")
			{
				verify_fresh_write(*zone);
			}
			else if (std::string_view(rewrite) == "dump")
			{
				verify_dump_round_trip(*zone);
			}
			else if (std::string_view(rewrite) == "havok")
			{
				verify_havok(zone_folder, *zone);
			}
			else if (std::string_view(rewrite) == "convex")
			{
				verify_convex(zone_folder, *zone);
			}
			else if (std::string_view(rewrite) == "compound")
			{
				verify_compound(zone_folder, *zone);
			}
			else if (std::string_view(rewrite) == "fxshape")
			{
				verify_fxshape(zone_folder, *zone);
			}
			else if (std::string_view(rewrite) == "mesh")
			{
				verify_mesh(zone_folder, *zone);
			}
			else if (std::string_view(rewrite) == "physics")
			{
				verify_physics(zone_folder, *zone);
			}
			else if (std::string_view(rewrite) == "compare")
			{
				verify_compare(zone_folder, *zone);
			}
			else if (std::string_view(rewrite) == "neutral")
			{
				verify_neutral_rewrite(*zone);
			}
			else
			{
				verify_rewrite(*zone);
			}
		}
	}
}
