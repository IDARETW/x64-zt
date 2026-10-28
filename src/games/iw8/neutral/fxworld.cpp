// Emitted by tools/gen_iw8_neutral.py from tools/neutral_families/fxworld.json: do not edit.
#include <std_include.hpp>
#include "fxworld.hpp"
#include "plain.hpp"

namespace zonetool::iw8::neutral
{
	namespace
	{
		asset_data::fxworld::fx_world read_fx_world(const FxWorld& native);
		void write_fx_world(FxWorld& native, const asset_data::fxworld::fx_world& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_fx_world(const asset_data::fxworld::fx_world& data);
		asset_data::fxworld::fx_glass_globals read_fx_glass_globals(const FxGlassGlobals& native);
		void write_fx_glass_globals(FxGlassGlobals& native, const asset_data::fxworld::fx_glass_globals& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_fx_glass_globals(const asset_data::fxworld::fx_glass_globals& data);
		asset_data::fxworld::fx_glass_def read_fx_glass_def(const FxGlassDef& native);
		void write_fx_glass_def(FxGlassDef& native, const asset_data::fxworld::fx_glass_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_fx_glass_def(const asset_data::fxworld::fx_glass_def& data);
		asset_data::fxworld::fx_combined_def read_fx_combined_def(const FxCombinedDef& native);
		void write_fx_combined_def(FxCombinedDef& native, const asset_data::fxworld::fx_combined_def& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_fx_combined_def(const asset_data::fxworld::fx_combined_def& data);
		asset_data::fxworld::snd_alias_lookup read_snd_alias_lookup(const SndAliasLookup& native);
		void write_snd_alias_lookup(SndAliasLookup& native, const asset_data::fxworld::snd_alias_lookup& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_snd_alias_lookup(const asset_data::fxworld::snd_alias_lookup& data);
		asset_data::fxworld::fx_glass_transient_lookup read_fx_glass_transient_lookup(const FxGlassTransientLookup& native);
		void write_fx_glass_transient_lookup(FxGlassTransientLookup& native, const asset_data::fxworld::fx_glass_transient_lookup& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_fx_glass_transient_lookup(const asset_data::fxworld::fx_glass_transient_lookup& data);
		asset_data::fxworld::fx_glass_system read_fx_glass_system(const FxGlassSystem& native);
		void write_fx_glass_system(FxGlassSystem& native, const asset_data::fxworld::fx_glass_system& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_fx_glass_system(const asset_data::fxworld::fx_glass_system& data);
		asset_data::fxworld::fx_glass_piece_state read_fx_glass_piece_state(const FxGlassPieceState& native);
		void write_fx_glass_piece_state(FxGlassPieceState& native, const asset_data::fxworld::fx_glass_piece_state& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_fx_glass_piece_state(const asset_data::fxworld::fx_glass_piece_state& data);
		asset_data::fxworld::fx_glass_piece_dynamics read_fx_glass_piece_dynamics(const FxGlassPieceDynamics& native);
		void write_fx_glass_piece_dynamics(FxGlassPieceDynamics& native, const asset_data::fxworld::fx_glass_piece_dynamics& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_fx_glass_piece_dynamics(const asset_data::fxworld::fx_glass_piece_dynamics& data);
		asset_data::fxworld::fx_glass_init_piece_state read_fx_glass_init_piece_state(const FxGlassInitPieceState& native);
		void write_fx_glass_init_piece_state(FxGlassInitPieceState& native, const asset_data::fxworld::fx_glass_init_piece_state& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_fx_glass_init_piece_state(const asset_data::fxworld::fx_glass_init_piece_state& data);
		asset_data::fxworld::fx_spatial_frame read_fx_spatial_frame(const FxSpatialFrame& native);
		void write_fx_spatial_frame(FxSpatialFrame& native, const asset_data::fxworld::fx_spatial_frame& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_fx_spatial_frame(const asset_data::fxworld::fx_spatial_frame& data);
		asset_data::fxworld::fx_world_transient_zone read_fx_world_transient_zone(const FxWorldTransientZone& native);
		void write_fx_world_transient_zone(FxWorldTransientZone& native, const asset_data::fxworld::fx_world_transient_zone& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_fx_world_transient_zone(const asset_data::fxworld::fx_world_transient_zone& data);

		asset_data::fxworld::fx_glass_globals read_fx_glass_globals(const FxGlassGlobals& native)
		{
			plain::trace("read_fx_glass_globals");
			asset_data::fxworld::fx_glass_globals data;
			data.def_count = native.defCount;
			data.defs = plain::read_array(native.defs, native.defCount, "FxGlassGlobals.defs", [&](const auto& item) { return read_fx_glass_def(item); });
			data.glass_transient_lookup = plain::read_array(native.glassTransientLookup, native.glassTransientLookupCount, "FxGlassGlobals.glassTransientLookup", [&](const auto& item) { return read_fx_glass_transient_lookup(item); });
			data.distances = plain::read_array(native.distances, native.defCount, "FxGlassGlobals.distances", [&](const auto& item) { return item; });
			return data;
		}

		void write_fx_glass_globals(FxGlassGlobals& native, const asset_data::fxworld::fx_glass_globals& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.defCount = data.def_count;
			native.defs = plain::write_array<FxGlassDef>(data.defs, memory, [&](auto& out, const auto& in) { write_fx_glass_def(out, in, resolve, memory); });
			native.glassTransientLookup = plain::write_array<FxGlassTransientLookup>(data.glass_transient_lookup, memory, [&](auto& out, const auto& in) { write_fx_glass_transient_lookup(out, in, resolve, memory); });
			native.distances = plain::write_array<float>(data.distances, memory, [&](auto& out, const auto& in) { out = in; });
			native.glassTransientLookupCount = plain::count_of<std::uint32_t>(data.glass_transient_lookup);
		}

		std::size_t size_fx_glass_globals([[maybe_unused]] const asset_data::fxworld::fx_glass_globals& data)
		{
			std::size_t size = 0;
			if (data.defs)
			{
				size += plain::sized(std::max<std::size_t>(data.defs->size(), 1) * sizeof(FxGlassDef));
				for (const auto& item : *data.defs)
				{
					size += size_fx_glass_def(item);
				}
			}
			if (data.glass_transient_lookup)
			{
				size += plain::sized(std::max<std::size_t>(data.glass_transient_lookup->size(), 1) * sizeof(FxGlassTransientLookup));
				for (const auto& item : *data.glass_transient_lookup)
				{
					size += size_fx_glass_transient_lookup(item);
				}
			}
			if (data.distances)
			{
				size += plain::sized(std::max<std::size_t>(data.distances->size(), 1) * sizeof(float));
			}
			return size;
		}

		asset_data::fxworld::fx_glass_system read_fx_glass_system(const FxGlassSystem& native)
		{
			plain::trace("read_fx_glass_system");
			asset_data::fxworld::fx_glass_system data;
			data.time = native.time;
			data.prev_time = native.prevTime;
			data.cell_count = native.cellCount;
			data.active_piece_count = native.activePieceCount;
			data.first_free_piece = native.firstFreePiece;
			data.geo_data_count = native.geoDataCount;
			data.piece_places = plain::read_array(native.piecePlaces, native.pieceLimit, "FxGlassSystem.piecePlaces", [&](const auto& item) { return plain::read_block<std::uint32_t>(&item); });
			data.piece_states = plain::read_array(native.pieceStates, native.pieceLimit, "FxGlassSystem.pieceStates", [&](const auto& item) { return read_fx_glass_piece_state(item); });
			data.piece_dynamics = plain::read_array(native.pieceDynamics, native.pieceLimit, "FxGlassSystem.pieceDynamics", [&](const auto& item) { return read_fx_glass_piece_dynamics(item); });
			data.geo_data = plain::read_array(native.geoData, native.geoDataLimit, "FxGlassSystem.geoData", [&](const auto& item) { return plain::read_block<std::array<std::uint8_t, 4>>(&item); });
			data.is_in_use = plain::read_array(native.isInUse, native.pieceWordCount, "FxGlassSystem.isInUse", [&](const auto& item) { return item; });
			data.cell_bits = plain::read_array(native.cellBits, (native.cellCount * native.pieceWordCount), "FxGlassSystem.cellBits", [&](const auto& item) { return item; });
			data.vis_data = plain::read_array(native.visData, ((native.pieceLimit + 15) & 0xFFFFFFF0ull), "FxGlassSystem.visData", [&](const auto& item) { return item; });
			data.link_org = plain::read_array(native.linkOrg, native.pieceLimit, "FxGlassSystem.linkOrg", [&](const auto& item) { return plain::read_scalars<float, 3>(&item); });
			data.half_thickness = plain::read_array(native.halfThickness, ((native.pieceLimit + 3) & 0xFFFFFFFCull), "FxGlassSystem.halfThickness", [&](const auto& item) { return item; });
			data.init_geo_data = plain::read_array(native.initGeoData, native.initGeoDataCount, "FxGlassSystem.initGeoData", [&](const auto& item) { return plain::read_block<std::array<std::uint8_t, 4>>(&item); });
			data.need_to_compact_data = native.needToCompactData;
			data.init_count = native.initCount;
			data.effect_chance_accum = native.effectChanceAccum;
			data.last_piece_deletion_time = native.lastPieceDeletionTime;
			data.init_piece_states = plain::read_array(native.initPieceStates, native.initPieceCount, "FxGlassSystem.initPieceStates", [&](const auto& item) { return read_fx_glass_init_piece_state(item); });
			return data;
		}

		void write_fx_glass_system(FxGlassSystem& native, const asset_data::fxworld::fx_glass_system& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.time = data.time;
			native.prevTime = data.prev_time;
			native.cellCount = data.cell_count;
			native.activePieceCount = data.active_piece_count;
			native.firstFreePiece = data.first_free_piece;
			native.geoDataCount = data.geo_data_count;
			native.piecePlaces = plain::write_array<FxGlassPiecePlace>(data.piece_places, memory, [&](auto& out, const auto& in) { plain::write_block(&out, in); });
			native.pieceStates = plain::write_array<FxGlassPieceState>(data.piece_states, memory, [&](auto& out, const auto& in) { write_fx_glass_piece_state(out, in, resolve, memory); });
			native.pieceDynamics = plain::write_array<FxGlassPieceDynamics>(data.piece_dynamics, memory, [&](auto& out, const auto& in) { write_fx_glass_piece_dynamics(out, in, resolve, memory); });
			native.geoData = plain::write_array<FxGlassGeometryData>(data.geo_data, memory, [&](auto& out, const auto& in) { plain::write_block(&out, in); });
			native.isInUse = plain::write_array<std::uint32_t>(data.is_in_use, memory, [&](auto& out, const auto& in) { out = in; });
			native.cellBits = plain::write_array<std::uint32_t>(data.cell_bits, memory, [&](auto& out, const auto& in) { out = in; });
			native.visData = plain::write_array<std::uint8_t>(data.vis_data, memory, [&](auto& out, const auto& in) { out = in; });
			native.linkOrg = plain::write_array<vec3_t>(data.link_org, memory, [&](auto& out, const auto& in) { plain::write_scalars(&out, in); });
			native.halfThickness = plain::write_array<float>(data.half_thickness, memory, [&](auto& out, const auto& in) { out = in; });
			native.initGeoData = plain::write_array<FxGlassGeometryData>(data.init_geo_data, memory, [&](auto& out, const auto& in) { plain::write_block(&out, in); });
			native.needToCompactData = data.need_to_compact_data;
			native.initCount = data.init_count;
			native.effectChanceAccum = data.effect_chance_accum;
			native.lastPieceDeletionTime = data.last_piece_deletion_time;
			native.initPieceStates = plain::write_array<FxGlassInitPieceState>(data.init_piece_states, memory, [&](auto& out, const auto& in) { write_fx_glass_init_piece_state(out, in, resolve, memory); });
			native.pieceLimit = plain::count_of<std::uint32_t>(data.piece_places);
			native.geoDataLimit = plain::count_of<std::uint32_t>(data.geo_data);
			native.pieceWordCount = plain::count_of<std::uint32_t>(data.is_in_use);
			native.initGeoDataCount = plain::count_of<std::uint32_t>(data.init_geo_data);
			native.initPieceCount = plain::count_of<std::uint32_t>(data.init_piece_states);
		}

		std::size_t size_fx_glass_system([[maybe_unused]] const asset_data::fxworld::fx_glass_system& data)
		{
			std::size_t size = 0;
			if (data.piece_places)
			{
				size += plain::sized(std::max<std::size_t>(data.piece_places->size(), 1) * sizeof(FxGlassPiecePlace));
			}
			if (data.piece_states)
			{
				size += plain::sized(std::max<std::size_t>(data.piece_states->size(), 1) * sizeof(FxGlassPieceState));
				for (const auto& item : *data.piece_states)
				{
					size += size_fx_glass_piece_state(item);
				}
			}
			if (data.piece_dynamics)
			{
				size += plain::sized(std::max<std::size_t>(data.piece_dynamics->size(), 1) * sizeof(FxGlassPieceDynamics));
				for (const auto& item : *data.piece_dynamics)
				{
					size += size_fx_glass_piece_dynamics(item);
				}
			}
			if (data.geo_data)
			{
				size += plain::sized(std::max<std::size_t>(data.geo_data->size(), 1) * sizeof(FxGlassGeometryData));
			}
			if (data.is_in_use)
			{
				size += plain::sized(std::max<std::size_t>(data.is_in_use->size(), 1) * sizeof(std::uint32_t));
			}
			if (data.cell_bits)
			{
				size += plain::sized(std::max<std::size_t>(data.cell_bits->size(), 1) * sizeof(std::uint32_t));
			}
			if (data.vis_data)
			{
				size += plain::sized(std::max<std::size_t>(data.vis_data->size(), 1) * sizeof(std::uint8_t));
			}
			if (data.link_org)
			{
				size += plain::sized(std::max<std::size_t>(data.link_org->size(), 1) * sizeof(vec3_t));
			}
			if (data.half_thickness)
			{
				size += plain::sized(std::max<std::size_t>(data.half_thickness->size(), 1) * sizeof(float));
			}
			if (data.init_geo_data)
			{
				size += plain::sized(std::max<std::size_t>(data.init_geo_data->size(), 1) * sizeof(FxGlassGeometryData));
			}
			if (data.init_piece_states)
			{
				size += plain::sized(std::max<std::size_t>(data.init_piece_states->size(), 1) * sizeof(FxGlassInitPieceState));
				for (const auto& item : *data.init_piece_states)
				{
					size += size_fx_glass_init_piece_state(item);
				}
			}
			return size;
		}

		asset_data::fxworld::fx_world read_fx_world(const FxWorld& native)
		{
			plain::trace("read_fx_world");
			asset_data::fxworld::fx_world data;
			data.name = plain::read_string(native.name);
			data.glass_glob = read_fx_glass_globals(native.glassGlob);
			data.glass_sys = read_fx_glass_system(native.glassSys);
			data.transient_zones = plain::read_prefix(native.transientZones, native.transientZoneCount, "FxWorld.transientZones", [&](const auto& item) { return plain::read_asset(ASSET_TYPE_FXWORLD_TRANSIENT_ZONE, item); });
			data.active_transient_zones = std::to_array(native.activeTransientZones);
			data.active_transient_zone_count = native.activeTransientZoneCount;
			return data;
		}

		void write_fx_world(FxWorld& native, const asset_data::fxworld::fx_world& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			write_fx_glass_globals(native.glassGlob, data.glass_glob, resolve, memory);
			write_fx_glass_system(native.glassSys, data.glass_sys, resolve, memory);
			plain::write_prefix(native.transientZones, data.transient_zones, [&](auto& out, const auto& in) { out = plain::resolve_asset<FxWorldTransientZone>(resolve, ASSET_TYPE_FXWORLD_TRANSIENT_ZONE, in); });
			std::ranges::copy(data.active_transient_zones, native.activeTransientZones);
			native.activeTransientZoneCount = data.active_transient_zone_count;
			native.transientZoneCount = plain::count_of<std::uint32_t>(data.transient_zones);
		}

		std::size_t size_fx_world([[maybe_unused]] const asset_data::fxworld::fx_world& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			size += size_fx_glass_globals(data.glass_glob);
			size += size_fx_glass_system(data.glass_sys);
			if (data.transient_zones)
			{
				size += plain::sized(std::max<std::size_t>(data.transient_zones->size(), 1) * sizeof(FxWorldTransientZone*));
			}
			return size;
		}

		asset_data::fxworld::fx_combined_def read_fx_combined_def(const FxCombinedDef& native)
		{
			plain::trace("read_fx_combined_def");
			asset_data::fxworld::fx_combined_def data;
			data.particle_system_def = plain::read_asset(ASSET_TYPE_VFX, native.particleSystemDef);
			return data;
		}

		void write_fx_combined_def(FxCombinedDef& native, const asset_data::fxworld::fx_combined_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.particleSystemDef = plain::resolve_asset<ParticleSystemDef>(resolve, ASSET_TYPE_VFX, data.particle_system_def);
		}

		std::size_t size_fx_combined_def([[maybe_unused]] const asset_data::fxworld::fx_combined_def& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::fxworld::snd_alias_lookup read_snd_alias_lookup(const SndAliasLookup& native)
		{
			plain::trace("read_snd_alias_lookup");
			asset_data::fxworld::snd_alias_lookup data;
			data.name = plain::read_string(native.name);
			return data;
		}

		void write_snd_alias_lookup(SndAliasLookup& native, const asset_data::fxworld::snd_alias_lookup& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
		}

		std::size_t size_snd_alias_lookup([[maybe_unused]] const asset_data::fxworld::snd_alias_lookup& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			return size;
		}

		asset_data::fxworld::fx_glass_def read_fx_glass_def(const FxGlassDef& native)
		{
			plain::trace("read_fx_glass_def");
			asset_data::fxworld::fx_glass_def data;
			data.half_thickness = native.halfThickness;
			data.tex_vecs = [&] { std::array<std::array<float, 2>, 2> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = plain::read_scalars<float, 2>(&native.texVecs[i]); } return out; }();
			data.color = plain::read_block<std::array<std::uint8_t, 4>>(&native.color);
			data.material = plain::read_asset(ASSET_TYPE_MATERIAL, native.material);
			data.material_shattered = plain::read_asset(ASSET_TYPE_MATERIAL, native.materialShattered);
			data.physics_asset = plain::read_asset(ASSET_TYPE_PHYSICSASSET, native.physicsAsset);
			data.piece_break_effect = read_fx_combined_def(native.pieceBreakEffect);
			data.shatter_effect = read_fx_combined_def(native.shatterEffect);
			data.shatter_small_effect = read_fx_combined_def(native.shatterSmallEffect);
			data.crack_decal_effect = read_fx_combined_def(native.crackDecalEffect);
			data.damaged_sound = read_snd_alias_lookup(native.damagedSound);
			data.destroyed_sound = read_snd_alias_lookup(native.destroyedSound);
			data.destroyed_quiet_sound = read_snd_alias_lookup(native.destroyedQuietSound);
			data.high_mip_radius_inv_sq = native.highMipRadiusInvSq;
			data.shattered_high_mip_radius_inv_sq = native.shatteredHighMipRadiusInvSq;
			data.num_crack_rings = native.numCrackRings;
			data.transient_index = native.transientIndex;
			return data;
		}

		void write_fx_glass_def(FxGlassDef& native, const asset_data::fxworld::fx_glass_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.halfThickness = data.half_thickness;
			for (std::size_t i = 0; i < data.tex_vecs.size(); ++i)
			{
				plain::write_scalars(&native.texVecs[i], data.tex_vecs[i]);
			}
			plain::write_block(&native.color, data.color);
			native.material = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, data.material);
			native.materialShattered = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, data.material_shattered);
			native.physicsAsset = plain::resolve_asset<PhysicsAsset>(resolve, ASSET_TYPE_PHYSICSASSET, data.physics_asset);
			write_fx_combined_def(native.pieceBreakEffect, data.piece_break_effect, resolve, memory);
			write_fx_combined_def(native.shatterEffect, data.shatter_effect, resolve, memory);
			write_fx_combined_def(native.shatterSmallEffect, data.shatter_small_effect, resolve, memory);
			write_fx_combined_def(native.crackDecalEffect, data.crack_decal_effect, resolve, memory);
			write_snd_alias_lookup(native.damagedSound, data.damaged_sound, resolve, memory);
			write_snd_alias_lookup(native.destroyedSound, data.destroyed_sound, resolve, memory);
			write_snd_alias_lookup(native.destroyedQuietSound, data.destroyed_quiet_sound, resolve, memory);
			native.highMipRadiusInvSq = data.high_mip_radius_inv_sq;
			native.shatteredHighMipRadiusInvSq = data.shattered_high_mip_radius_inv_sq;
			native.numCrackRings = data.num_crack_rings;
			native.transientIndex = data.transient_index;
		}

		std::size_t size_fx_glass_def([[maybe_unused]] const asset_data::fxworld::fx_glass_def& data)
		{
			std::size_t size = 0;
			size += size_fx_combined_def(data.piece_break_effect);
			size += size_fx_combined_def(data.shatter_effect);
			size += size_fx_combined_def(data.shatter_small_effect);
			size += size_fx_combined_def(data.crack_decal_effect);
			size += size_snd_alias_lookup(data.damaged_sound);
			size += size_snd_alias_lookup(data.destroyed_sound);
			size += size_snd_alias_lookup(data.destroyed_quiet_sound);
			return size;
		}

		asset_data::fxworld::fx_glass_transient_lookup read_fx_glass_transient_lookup(const FxGlassTransientLookup& native)
		{
			plain::trace("read_fx_glass_transient_lookup");
			asset_data::fxworld::fx_glass_transient_lookup data;
			data.global_piece_start_index = native.globalPieceStartIndex;
			data.transient_index = native.transientIndex;
			return data;
		}

		void write_fx_glass_transient_lookup(FxGlassTransientLookup& native, const asset_data::fxworld::fx_glass_transient_lookup& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.globalPieceStartIndex = data.global_piece_start_index;
			native.transientIndex = data.transient_index;
		}

		std::size_t size_fx_glass_transient_lookup([[maybe_unused]] const asset_data::fxworld::fx_glass_transient_lookup& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::fxworld::fx_glass_piece_state read_fx_glass_piece_state(const FxGlassPieceState& native)
		{
			plain::trace("read_fx_glass_piece_state");
			asset_data::fxworld::fx_glass_piece_state data;
			data.tex_coord_origin = plain::read_scalars<float, 2>(&native.texCoordOrigin);
			data.support_mask = native.supportMask;
			data.geo_data_start = native.geoDataStart;
			data.init_index = native.initIndex;
			data.def_index = native.defIndex;
			plain::require_zero(native.pad, "FxGlassPieceState.pad");
			data.vert_count = native.vertCount;
			data.hole_data_count = native.holeDataCount;
			data.crack_data_count = native.crackDataCount;
			data.fan_data_count = native.fanDataCount;
			data.flags = native.flags;
			data.area_x2 = native.areaX2;
			return data;
		}

		void write_fx_glass_piece_state(FxGlassPieceState& native, const asset_data::fxworld::fx_glass_piece_state& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.texCoordOrigin, data.tex_coord_origin);
			native.supportMask = data.support_mask;
			native.geoDataStart = data.geo_data_start;
			native.initIndex = data.init_index;
			native.defIndex = data.def_index;
			native.vertCount = data.vert_count;
			native.holeDataCount = data.hole_data_count;
			native.crackDataCount = data.crack_data_count;
			native.fanDataCount = data.fan_data_count;
			native.flags = data.flags;
			native.areaX2 = data.area_x2;
		}

		std::size_t size_fx_glass_piece_state([[maybe_unused]] const asset_data::fxworld::fx_glass_piece_state& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::fxworld::fx_glass_piece_dynamics read_fx_glass_piece_dynamics(const FxGlassPieceDynamics& native)
		{
			plain::trace("read_fx_glass_piece_dynamics");
			asset_data::fxworld::fx_glass_piece_dynamics data;
			data.fall_time = native.fallTime;
			data.physics_instance = native.physicsInstance;
			data.physics_detail_instance = native.physicsDetailInstance;
			data.vel = plain::read_scalars<float, 3>(&native.vel);
			data.avel = plain::read_scalars<float, 3>(&native.avel);
			data.pending_creation = native.pendingCreation;
			return data;
		}

		void write_fx_glass_piece_dynamics(FxGlassPieceDynamics& native, const asset_data::fxworld::fx_glass_piece_dynamics& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.fallTime = data.fall_time;
			native.physicsInstance = data.physics_instance;
			native.physicsDetailInstance = data.physics_detail_instance;
			plain::write_scalars(&native.vel, data.vel);
			plain::write_scalars(&native.avel, data.avel);
			native.pendingCreation = data.pending_creation;
		}

		std::size_t size_fx_glass_piece_dynamics([[maybe_unused]] const asset_data::fxworld::fx_glass_piece_dynamics& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::fxworld::fx_spatial_frame read_fx_spatial_frame(const FxSpatialFrame& native)
		{
			plain::trace("read_fx_spatial_frame");
			asset_data::fxworld::fx_spatial_frame data;
			data.quat = plain::read_scalars<float, 4>(&native.quat);
			data.origin = plain::read_scalars<float, 3>(&native.origin);
			return data;
		}

		void write_fx_spatial_frame(FxSpatialFrame& native, const asset_data::fxworld::fx_spatial_frame& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.quat, data.quat);
			plain::write_scalars(&native.origin, data.origin);
		}

		std::size_t size_fx_spatial_frame([[maybe_unused]] const asset_data::fxworld::fx_spatial_frame& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::fxworld::fx_glass_init_piece_state read_fx_glass_init_piece_state(const FxGlassInitPieceState& native)
		{
			plain::trace("read_fx_glass_init_piece_state");
			asset_data::fxworld::fx_glass_init_piece_state data;
			data.frame = read_fx_spatial_frame(native.frame);
			data.radius = native.radius;
			data.tex_coord_origin = plain::read_scalars<float, 2>(&native.texCoordOrigin);
			data.support_mask = native.supportMask;
			data.area_x2 = native.areaX2;
			data.def_index = native.defIndex;
			data.vert_count = native.vertCount;
			data.fan_data_count = native.fanDataCount;
			plain::require_zero(native.pad, "FxGlassInitPieceState.pad");
			return data;
		}

		void write_fx_glass_init_piece_state(FxGlassInitPieceState& native, const asset_data::fxworld::fx_glass_init_piece_state& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_fx_spatial_frame(native.frame, data.frame, resolve, memory);
			native.radius = data.radius;
			plain::write_scalars(&native.texCoordOrigin, data.tex_coord_origin);
			native.supportMask = data.support_mask;
			native.areaX2 = data.area_x2;
			native.defIndex = data.def_index;
			native.vertCount = data.vert_count;
			native.fanDataCount = data.fan_data_count;
		}

		std::size_t size_fx_glass_init_piece_state([[maybe_unused]] const asset_data::fxworld::fx_glass_init_piece_state& data)
		{
			std::size_t size = 0;
			size += size_fx_spatial_frame(data.frame);
			return size;
		}

		asset_data::fxworld::fx_world_transient_zone read_fx_world_transient_zone(const FxWorldTransientZone& native)
		{
			plain::trace("read_fx_world_transient_zone");
			asset_data::fxworld::fx_world_transient_zone data;
			data.name = plain::read_string(native.name);
			data.transient_zone_index = native.transientZoneIndex;
			data.glass_sys = read_fx_glass_system(native.glassSys);
			return data;
		}

		void write_fx_world_transient_zone(FxWorldTransientZone& native, const asset_data::fxworld::fx_world_transient_zone& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.transientZoneIndex = data.transient_zone_index;
			write_fx_glass_system(native.glassSys, data.glass_sys, resolve, memory);
		}

		std::size_t size_fx_world_transient_zone([[maybe_unused]] const asset_data::fxworld::fx_world_transient_zone& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			size += size_fx_glass_system(data.glass_sys);
			return size;
		}

	}

	std::expected<asset_data::fxworld::fx_world, std::string> to_neutral(const FxWorld& native)
	{
		try
		{
			auto data = read_fx_world(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	FxWorld* from_neutral(const asset_data::fxworld::fx_world& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<FxWorld>();
		write_fx_world(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::fxworld::fx_world& data)
	{
		return plain::rounded(sizeof(FxWorld)) + size_fx_world(data) + 64;
	}

	std::expected<asset_data::fxworld::fx_world_transient_zone, std::string> to_neutral(const FxWorldTransientZone& native)
	{
		try
		{
			auto data = read_fx_world_transient_zone(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	FxWorldTransientZone* from_neutral(const asset_data::fxworld::fx_world_transient_zone& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<FxWorldTransientZone>();
		write_fx_world_transient_zone(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::fxworld::fx_world_transient_zone& data)
	{
		return plain::rounded(sizeof(FxWorldTransientZone)) + size_fx_world_transient_zone(data) + 64;
	}

}
