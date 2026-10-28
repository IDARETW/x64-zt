// Emitted by tools/gen_iw8_neutral.py from tools/neutral_families/world.json: do not edit.
#include <std_include.hpp>
#include "world.hpp"
#include "plain.hpp"

namespace zonetool::iw8::neutral
{
	namespace
	{
		asset_data::com_primary_light read_com_primary_light(const ComPrimaryLight& native)
		{
			asset_data::com_primary_light data;
			data.spawn_flags = native.spawnFlags;
			data.type = native.type;
			data.can_use_shadow_map = native.canUseShadowMap;
			data.needs_dynamic_shadows = native.needsDynamicShadows;
			data.is_volumetric = native.isVolumetric;
			data.transient_zone_list = native.transientZoneList;
			data.entity_id = native.entityId;
			data.intensity = native.intensity;
			data.uv_intensity = native.uvIntensity;
			data.ir_intensity = native.irIntensity;
			data.heat_intensity = native.heatIntensity;
			data.color_linear_srgb = plain::read_floats<3>(&native.colorLinearSrgb);
			data.dir = plain::read_floats<3>(&native.dir);
			data.up = plain::read_floats<3>(&native.up);
			data.origin = plain::read_floats<3>(&native.origin);
			data.radius = native.radius;
			data.fade_offset = plain::read_floats<2>(&native.fadeOffset);
			data.bulb_radius = native.bulbRadius;
			data.bulb_length = plain::read_floats<3>(&native.bulbLength);
			data.cos_half_fov_outer = native.cosHalfFovOuter;
			data.cos_half_fov_inner = native.cosHalfFovInner;
			data.shadow_near_plane_bias = native.shadowNearPlaneBias;
			data.shadow_softness = native.shadowSoftness;
			data.shadow_bias = native.shadowBias;
			data.shadow_area = native.shadowArea;
			data.distance_falloff = native.distanceFalloff;
			data.cos_half_fov_collimation = native.cosHalfFovCollimation;
			data.rotation_limit = native.rotationLimit;
			data.translation_limit = native.translationLimit;
			data.def_name = plain::read_string(native.defName);
			return data;
		}

		void write_com_primary_light(ComPrimaryLight& native, const asset_data::com_primary_light& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.spawnFlags = data.spawn_flags;
			native.type = data.type;
			native.canUseShadowMap = data.can_use_shadow_map;
			native.needsDynamicShadows = data.needs_dynamic_shadows;
			native.isVolumetric = data.is_volumetric;
			native.transientZoneList = data.transient_zone_list;
			native.entityId = data.entity_id;
			native.intensity = data.intensity;
			native.uvIntensity = data.uv_intensity;
			native.irIntensity = data.ir_intensity;
			native.heatIntensity = data.heat_intensity;
			std::ranges::copy(data.color_linear_srgb, native.colorLinearSrgb.v);
			std::ranges::copy(data.dir, native.dir.v);
			std::ranges::copy(data.up, native.up.v);
			std::ranges::copy(data.origin, native.origin.v);
			native.radius = data.radius;
			std::ranges::copy(data.fade_offset, native.fadeOffset.v);
			native.bulbRadius = data.bulb_radius;
			std::ranges::copy(data.bulb_length, native.bulbLength.v);
			native.cosHalfFovOuter = data.cos_half_fov_outer;
			native.cosHalfFovInner = data.cos_half_fov_inner;
			native.shadowNearPlaneBias = data.shadow_near_plane_bias;
			native.shadowSoftness = data.shadow_softness;
			native.shadowBias = data.shadow_bias;
			native.shadowArea = data.shadow_area;
			native.distanceFalloff = data.distance_falloff;
			native.cosHalfFovCollimation = data.cos_half_fov_collimation;
			native.rotationLimit = data.rotation_limit;
			native.translationLimit = data.translation_limit;
			native.defName = plain::write_string(memory, data.def_name);
		}

		std::size_t size_com_primary_light([[maybe_unused]] const asset_data::com_primary_light& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.def_name);
			return size;
		}

		asset_data::com_change_list_info read_com_change_list_info(const ComChangeListInfo& native)
		{
			asset_data::com_change_list_info data;
			data.change_list_number = native.changeListNumber;
			data.time = native.time;
			data.user_name = plain::read_string(native.userName);
			return data;
		}

		void write_com_change_list_info(ComChangeListInfo& native, const asset_data::com_change_list_info& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.changeListNumber = data.change_list_number;
			native.time = data.time;
			native.userName = plain::write_string(memory, data.user_name);
		}

		std::size_t size_com_change_list_info([[maybe_unused]] const asset_data::com_change_list_info& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.user_name);
			return size;
		}

		asset_data::bitarray_384 read_bitarray_384(const bitarray_384& native)
		{
			asset_data::bitarray_384 data;
			data.array = std::to_array(native.array);
			return data;
		}

		void write_bitarray_384(bitarray_384& native, const asset_data::bitarray_384& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			std::ranges::copy(data.array, native.array);
		}

		std::size_t size_bitarray_384([[maybe_unused]] const asset_data::bitarray_384& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::com_world read_com_world(const ComWorld& native)
		{
			asset_data::com_world data;
			data.name = plain::read_string(native.name);
			data.is_in_use = native.isInUse;
			data.use_forward_plus = native.useForwardPlus;
			data.bake_quality = native.bakeQuality;
			data.proxy_lod_quality = native.proxyLODQuality;
			data.tr_vis_radii = std::to_array(native.trVisRadii);
			data.tr_vis_facing_dist_add = native.trVisFacingDistAdd;
			data.primary_lights = plain::read_array(native.primaryLights, native.primaryLightCount, "ComWorld.primaryLights", [&](const auto& item) { return read_com_primary_light(item); });
			data.scriptable_primary_light_count = native.scriptablePrimaryLightCount;
			data.first_scriptable_primary_light = native.firstScriptablePrimaryLight;
			data.transient_table = plain::read_array(native.transientTable, native.transientTableSize, "ComWorld.transientTable", [&](const auto& item) { return item; });
			data.change_list_info = read_com_change_list_info(native.changeListInfo);
			data.umbra_gate_names = plain::read_array(native.umbraGateNames, native.numUmbraGates, "ComWorld.umbraGateNames", [&](const auto& item) { return plain::read_string(item); });
			data.umbra_gate_initial_states = read_bitarray_384(native.umbraGateInitialStates);
			return data;
		}

		void write_com_world(ComWorld& native, const asset_data::com_world& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.isInUse = data.is_in_use;
			native.useForwardPlus = data.use_forward_plus;
			native.bakeQuality = data.bake_quality;
			native.proxyLODQuality = data.proxy_lod_quality;
			std::ranges::copy(data.tr_vis_radii, native.trVisRadii);
			native.trVisFacingDistAdd = data.tr_vis_facing_dist_add;
			native.primaryLights = plain::write_array<ComPrimaryLight>(data.primary_lights, memory, [&](auto& out, const auto& in) { write_com_primary_light(out, in, resolve, memory); });
			native.scriptablePrimaryLightCount = data.scriptable_primary_light_count;
			native.firstScriptablePrimaryLight = data.first_scriptable_primary_light;
			native.transientTable = plain::write_array<std::uint16_t>(data.transient_table, memory, [&](auto& out, const auto& in) { out = in; });
			write_com_change_list_info(native.changeListInfo, data.change_list_info, resolve, memory);
			native.umbraGateNames = plain::write_array<const char*>(data.umbra_gate_names, memory, [&](auto& out, const auto& in) { out = plain::write_string(memory, in); });
			write_bitarray_384(native.umbraGateInitialStates, data.umbra_gate_initial_states, resolve, memory);
			native.primaryLightCount = plain::count_of<std::uint32_t>(data.primary_lights);
			native.transientTableSize = plain::count_of<std::uint32_t>(data.transient_table);
			native.numUmbraGates = plain::count_of<std::uint32_t>(data.umbra_gate_names);
		}

		std::size_t size_com_world([[maybe_unused]] const asset_data::com_world& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.primary_lights)
			{
				size += plain::rounded(std::max<std::size_t>(data.primary_lights->size(), 1) * sizeof(ComPrimaryLight));
				for (const auto& item : *data.primary_lights)
				{
					size += size_com_primary_light(item);
				}
			}
			if (data.transient_table)
			{
				size += plain::rounded(std::max<std::size_t>(data.transient_table->size(), 1) * sizeof(std::uint16_t));
			}
			size += size_com_change_list_info(data.change_list_info);
			if (data.umbra_gate_names)
			{
				size += plain::rounded(std::max<std::size_t>(data.umbra_gate_names->size(), 1) * sizeof(const char*));
				for (const auto& item : *data.umbra_gate_names)
				{
					size += plain::string_size(item);
				}
			}
			size += size_bitarray_384(data.umbra_gate_initial_states);
			return size;
		}

		asset_data::glass_piece read_glass_piece(const G_GlassPiece& native)
		{
			asset_data::glass_piece data;
			data.damage_taken = native.damageTaken;
			data.collapse_time = native.collapseTime;
			data.last_state_change_time = native.lastStateChangeTime;
			data.impact_dir = native.impactDir;
			data.impact_pos = std::to_array(native.impactPos);
			return data;
		}

		void write_glass_piece(G_GlassPiece& native, const asset_data::glass_piece& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.damageTaken = data.damage_taken;
			native.collapseTime = data.collapse_time;
			native.lastStateChangeTime = data.last_state_change_time;
			native.impactDir = data.impact_dir;
			std::ranges::copy(data.impact_pos, native.impactPos);
		}

		std::size_t size_glass_piece([[maybe_unused]] const asset_data::glass_piece& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::glass_name read_glass_name(const G_GlassName& native)
		{
			asset_data::glass_name data;
			data.name_str = plain::read_string(native.nameStr);
			data.name = plain::read_script_string(native.name);
			data.piece_indices = plain::read_array(native.pieceIndices, native.pieceCount, "G_GlassName.pieceIndices", [&](const auto& item) { return item; });
			return data;
		}

		void write_glass_name(G_GlassName& native, const asset_data::glass_name& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.nameStr = plain::write_string(memory, data.name_str);
			native.name = static_cast<scr_string_t>(plain::write_script_string(data.name));
			native.pieceIndices = plain::write_array<std::uint16_t>(data.piece_indices, memory, [&](auto& out, const auto& in) { out = in; });
			native.pieceCount = plain::count_of<std::uint16_t>(data.piece_indices);
		}

		std::size_t size_glass_name([[maybe_unused]] const asset_data::glass_name& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name_str);
			if (data.piece_indices)
			{
				size += plain::rounded(std::max<std::size_t>(data.piece_indices->size(), 1) * sizeof(std::uint16_t));
			}
			return size;
		}

		asset_data::spatial_partition_tree read_spatial_partition_tree(const SpatialPartition_Tree& native)
		{
			asset_data::spatial_partition_tree data;
			data.tree = plain::read_array(native.tree, native.treeSize, "SpatialPartition_Tree.tree", [&](const auto& item) { return item; });
			data.always_list = plain::read_array(native.alwaysList, native.alwaysListLength, "SpatialPartition_Tree.alwaysList", [&](const auto& item) { return item; });
			return data;
		}

		void write_spatial_partition_tree(SpatialPartition_Tree& native, const asset_data::spatial_partition_tree& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.tree = plain::write_array<std::uint8_t>(data.tree, memory, [&](auto& out, const auto& in) { out = in; });
			native.alwaysList = plain::write_array<std::uint32_t>(data.always_list, memory, [&](auto& out, const auto& in) { out = in; });
			native.treeSize = plain::count_of<std::uint32_t>(data.tree);
			native.alwaysListLength = plain::count_of<std::uint32_t>(data.always_list);
		}

		std::size_t size_spatial_partition_tree([[maybe_unused]] const asset_data::spatial_partition_tree& data)
		{
			std::size_t size = 0;
			if (data.tree)
			{
				size += plain::rounded(std::max<std::size_t>(data.tree->size(), 1) * sizeof(std::uint8_t));
			}
			if (data.always_list)
			{
				size += plain::rounded(std::max<std::size_t>(data.always_list->size(), 1) * sizeof(std::uint32_t));
			}
			return size;
		}

		asset_data::glass_data read_glass_data(const G_GlassData& native)
		{
			asset_data::glass_data data;
			data.glass_pieces = plain::read_array(native.glassPieces, native.pieceCount, "G_GlassData.glassPieces", [&](const auto& item) { return read_glass_piece(item); });
			data.damage_to_weaken = native.damageToWeaken;
			data.damage_to_destroy = native.damageToDestroy;
			data.glass_names = plain::read_array(native.glassNames, native.glassNameCount, "G_GlassData.glassNames", [&](const auto& item) { return read_glass_name(item); });
			data.spatial_tree = plain::read_single(native.spatialTree, [&](const auto& item) { return read_spatial_partition_tree(item); });
			return data;
		}

		void write_glass_data(G_GlassData& native, const asset_data::glass_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.glassPieces = plain::write_array<G_GlassPiece>(data.glass_pieces, memory, [&](auto& out, const auto& in) { write_glass_piece(out, in, resolve, memory); });
			native.damageToWeaken = data.damage_to_weaken;
			native.damageToDestroy = data.damage_to_destroy;
			native.glassNames = plain::write_array<G_GlassName>(data.glass_names, memory, [&](auto& out, const auto& in) { write_glass_name(out, in, resolve, memory); });
			native.spatialTree = plain::write_single<SpatialPartition_Tree>(data.spatial_tree, memory, [&](auto& out, const auto& in) { write_spatial_partition_tree(out, in, resolve, memory); });
			native.pieceCount = plain::count_of<std::uint32_t>(data.glass_pieces);
			native.glassNameCount = plain::count_of<std::uint32_t>(data.glass_names);
		}

		std::size_t size_glass_data([[maybe_unused]] const asset_data::glass_data& data)
		{
			std::size_t size = 0;
			if (data.glass_pieces)
			{
				size += plain::rounded(std::max<std::size_t>(data.glass_pieces->size(), 1) * sizeof(G_GlassPiece));
				for (const auto& item : *data.glass_pieces)
				{
					size += size_glass_piece(item);
				}
			}
			if (data.glass_names)
			{
				size += plain::rounded(std::max<std::size_t>(data.glass_names->size(), 1) * sizeof(G_GlassName));
				for (const auto& item : *data.glass_names)
				{
					size += size_glass_name(item);
				}
			}
			if (data.spatial_tree)
			{
				size += plain::rounded(sizeof(SpatialPartition_Tree));
				size += size_spatial_partition_tree(*data.spatial_tree);
			}
			return size;
		}

		asset_data::glass_world read_glass_world(const GlassWorld& native)
		{
			asset_data::glass_world data;
			data.name = plain::read_string(native.name);
			data.g_glass_data = plain::read_single(native.g_glassData, [&](const auto& item) { return read_glass_data(item); });
			return data;
		}

		void write_glass_world(GlassWorld& native, const asset_data::glass_world& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.g_glassData = plain::write_single<G_GlassData>(data.g_glass_data, memory, [&](auto& out, const auto& in) { write_glass_data(out, in, resolve, memory); });
		}

		std::size_t size_glass_world([[maybe_unused]] const asset_data::glass_world& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.g_glass_data)
			{
				size += plain::rounded(sizeof(G_GlassData));
				size += size_glass_data(*data.g_glass_data);
			}
			return size;
		}

	}

	std::expected<asset_data::com_world, std::string> to_neutral(const ComWorld& native)
	{
		try
		{
			auto data = read_com_world(native);
			asset_data::validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	ComWorld* from_neutral(const asset_data::com_world& data, const asset_resolver& resolve, zone_memory& memory)
	{
		asset_data::validate(data);
		plain::context context(memory);
		auto* native = context.allocate<ComWorld>();
		write_com_world(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::com_world& data)
	{
		return plain::rounded(sizeof(ComWorld)) + size_com_world(data) + 64;
	}

	std::expected<asset_data::glass_world, std::string> to_neutral(const GlassWorld& native)
	{
		try
		{
			auto data = read_glass_world(native);
			asset_data::validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	GlassWorld* from_neutral(const asset_data::glass_world& data, const asset_resolver& resolve, zone_memory& memory)
	{
		asset_data::validate(data);
		plain::context context(memory);
		auto* native = context.allocate<GlassWorld>();
		write_glass_world(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::glass_world& data)
	{
		return plain::rounded(sizeof(GlassWorld)) + size_glass_world(data) + 64;
	}

}
