#include <std_include.hpp>
#include "xmodel.hpp"
#include "../iw8.hpp"
#include "../zone/loader.hpp"

#include <zone/script_strings.hpp>
#include <zone/zone_memory.hpp>

namespace zonetool::iw8::neutral
{
	namespace
	{
		static_assert(sizeof(XModel::noScalePartBits) == sizeof(asset_data::xmodel::unscaled_parts));
		static_assert(sizeof(DObjPartBits) == sizeof(asset_data::xmodel_lod::part_bits));
		static_assert(sizeof(GfxDecalVolumeObb::axes) == sizeof(asset_data::xmodel_decal_volume::axes));

		constexpr auto lod_count = std::extent_v<decltype(XModel::lodInfo)>;

		constexpr std::pair<std::uint8_t, const char*> lod_flags[] = {{XMODEL_LODINFO_FLAG_SUBDIV, "subdiv"},
			{XMODEL_LODINFO_FLAG_SUBDIV_NON_ADAPTIVE, "subdiv_non_adaptive"}};
		constexpr std::pair<std::uint8_t, const char*> reactive_flags[] = {{REACTIVEMOTION_MODELFLAG_USE_PART_TWEAKS, "use_part_tweaks"},
			{REACTIVEMOTION_MODELFLAG_USE_HIERARCHY, "use_hierarchy"}};

		constexpr std::size_t rounded(const std::size_t size)
		{
			return (size + 7) & ~std::size_t(7);
		}

		const char* copy_text(zone_memory& memory, const std::string& text)
		{
			auto* copy = memory.manual_allocate<char>(rounded(text.size() + 1));
			std::ranges::copy(text, copy);
			return copy;
		}

		template <typename T>
		T* allocate_array(zone_memory& memory, const std::size_t count)
		{
			return count ? memory.manual_allocate<T>(rounded(count * sizeof(T))) : nullptr;
		}

		std::string script_string(const scr_string_t id, const char* what)
		{
			const auto* text = SL_ConvertToString(id);
			if (!text)
			{
				throw std::runtime_error(std::format("a {} name is a null script string", what));
			}

			return text;
		}

		scr_string_t script_string_id(const std::string& text)
		{
			return static_cast<scr_string_t>(SL_GetString(text.data()));
		}

		// The definition name of the asset that a field points to (a reference stub's name without its comma), or none.
		std::optional<std::string> reference(const std::int32_t type, const void* header)
		{
			if (!header)
			{
				return std::nullopt;
			}

			const auto* name = get_asset_name(type, header);
			if (!name || !*name)
			{
				throw std::runtime_error(std::format("a referenced asset of type {} has no name", type));
			}

			return name[0] == ',' ? name + 1 : name;
		}

		template <typename T>
		T* resolved(const asset_resolver& resolve, const std::int32_t type, const std::optional<std::string>& name)
		{
			if (!name)
			{
				return nullptr;
			}

			auto* header = static_cast<T*>(resolve(type, *name));
			if (!header)
			{
				throw std::runtime_error(std::format("the referenced asset {} of type {} is absent", *name, type));
			}

			return header;
		}

		template <std::size_t Count>
		std::string enum_name(const char* const (&names)[Count], const std::uint8_t value, const char* what)
		{
			if (value >= Count)
			{
				throw std::runtime_error(std::format("the {} {} has no neutral name", what, value));
			}

			return names[value];
		}

		template <std::size_t Count>
		std::uint8_t enum_value(const char* const (&names)[Count], const std::string& name)
		{
			return static_cast<std::uint8_t>(std::ranges::find(names, name) - std::begin(names));
		}

		template <std::size_t Count>
		std::vector<std::string> flag_names(const std::pair<std::uint8_t, const char*> (&names)[Count], std::uint8_t bits, const char* what)
		{
			std::vector<std::string> result;
			for (const auto& [bit, name] : names)
			{
				if (bits & bit)
				{
					result.emplace_back(name);
					bits = static_cast<std::uint8_t>(bits & ~bit);
				}
			}

			if (bits)
			{
				throw std::runtime_error(std::format("the {} flags hold unknown bits {:#x}", what, bits));
			}

			std::ranges::sort(result);
			return result;
		}

		template <std::size_t Count>
		std::uint8_t flag_bits(const std::pair<std::uint8_t, const char*> (&names)[Count], const std::vector<std::string>& flags)
		{
			std::uint8_t bits = 0;
			for (const auto& flag : flags)
			{
				bits = static_cast<std::uint8_t>(bits | std::ranges::find(names, flag, &std::pair<std::uint8_t, const char*>::second)->first);
			}

			return bits;
		}

		template <std::size_t Count>
		std::array<float, Count> floats(const float (&values)[Count])
		{
			std::array<float, Count> result{};
			std::ranges::copy(values, result.begin());
			return result;
		}

		asset_data::bounds_box box(const Bounds& bounds)
		{
			return {floats(bounds.midPoint.v), floats(bounds.halfSize.v)};
		}

		Bounds native_box(const asset_data::bounds_box& bounds)
		{
			Bounds result{};
			std::ranges::copy(bounds.midpoint, result.midPoint.v);
			std::ranges::copy(bounds.half_size, result.halfSize.v);
			return result;
		}

		bool zero_bytes(const void* data, const std::size_t size)
		{
			const auto* bytes = static_cast<const std::uint8_t*>(data);
			return std::all_of(bytes, bytes + size, [](const std::uint8_t byte) { return !byte; });
		}

		void require(const bool condition, const char* what)
		{
			if (!condition)
			{
				throw std::runtime_error(what);
			}
		}

		asset_data::reactive_motion_info read_reactive(const ReactiveMotionModelInfo& native)
		{
			require(zero_bytes(&native.modelPartsBuffer, sizeof(native.modelPartsBuffer)) && zero_bytes(native.pad, sizeof(native.pad)) &&
				zero_bytes(native.runtime_0x54, sizeof(native.runtime_0x54)), "the reactive motion holds runtime data");
			require(!native.numParts || native.modelParts, "the reactive motion has parts without data");

			asset_data::reactive_motion_info result;
			result.wind = {native.windParams.windAreaScale, native.windParams.windAmplitudeScale, native.windParams.windFrequencyScale,
				native.windParams.unused0};
			result.flags = flag_names(reactive_flags, native.flags, "reactive motion");
			for (std::size_t part = 0; part < native.numParts; ++part)
			{
				const auto& value = native.modelParts[part];
				result.parts.push_back({floats(value.centerMS.v), value.stiffness, value.parentIdx, floats(value.pivotPosMS.v),
					value.pivotQuatMS.packed});
			}

			if (native.modelPartTweaks)
			{
				auto& tweaks = result.tweaks.emplace();
				for (std::size_t part = 0; part < native.numParts; ++part)
				{
					tweaks.push_back({floats(native.modelPartTweaks[part].ampScale.v), floats(native.modelPartTweaks[part].freqScale.v)});
				}
			}

			return result;
		}

		ReactiveMotionModelInfo* write_reactive(const asset_data::reactive_motion_info& data, zone_memory& memory)
		{
			require(data.parts.size() <= 0xFF, "a reactive motion has more parts than Replay counts");
			auto* native = memory.allocate<ReactiveMotionModelInfo>();
			native->windParams = {data.wind[0], data.wind[1], data.wind[2], data.wind[3]};
			native->flags = static_cast<ReactiveMotionModelFlags>(flag_bits(reactive_flags, data.flags));
			native->numParts = static_cast<std::uint8_t>(data.parts.size());
			native->modelParts = allocate_array<ReactiveMotionModelPart>(memory, data.parts.size());
			for (std::size_t part = 0; part < data.parts.size(); ++part)
			{
				const auto& value = data.parts[part];
				auto& target = native->modelParts[part];
				std::ranges::copy(value.center, target.centerMS.v);
				target.stiffness = value.stiffness;
				target.parentIdx = value.parent;
				std::ranges::copy(value.pivot, target.pivotPosMS.v);
				target.pivotQuatMS.packed = value.pivot_rotation;
			}

			if (data.tweaks)
			{
				native->modelPartTweaks = allocate_array<ReactiveMotionModelPartTweaks>(memory, data.tweaks->size());
				for (std::size_t part = 0; part < data.tweaks->size(); ++part)
				{
					std::ranges::copy((*data.tweaks)[part].amplitude_scale, native->modelPartTweaks[part].ampScale.v);
					std::ranges::copy((*data.tweaks)[part].frequency_scale, native->modelPartTweaks[part].freqScale.v);
				}
			}

			return native;
		}

		asset_data::xmodel_decal_volume read_decal(const XModelDecalVolume& native)
		{
			asset_data::xmodel_decal_volume result;
			result.center = floats(native.obb.center.v);
			std::memcpy(result.axes.data(), &native.obb.axes, sizeof(result.axes));
			result.half_size = floats(native.obb.halfSize.v);
			result.material = reference(ASSET_TYPE_MATERIAL, native.material);
			result.blend_mask = reference(ASSET_TYPE_DECAL_VOLUME_MASK, native.blendMapOverride);
			result.bone = native.boneIndex;
			result.uv_offset = floats(native.uvOffset.v);
			result.uv_matrix = floats(native.uvMatrix.v);
			return result;
		}

		void write_decal(const asset_data::xmodel_decal_volume& data, XModelDecalVolume& native, const asset_resolver& resolve)
		{
			std::ranges::copy(data.center, native.obb.center.v);
			std::memcpy(&native.obb.axes, data.axes.data(), sizeof(data.axes));
			std::ranges::copy(data.half_size, native.obb.halfSize.v);
			native.material = resolved<Material>(resolve, ASSET_TYPE_MATERIAL, data.material);
			native.blendMapOverride = resolved<GfxDecalVolumeMask>(resolve, ASSET_TYPE_DECAL_VOLUME_MASK, data.blend_mask);
			native.boneIndex = data.bone;
			std::ranges::copy(data.uv_offset, native.uvOffset.v);
			std::ranges::copy(data.uv_matrix, native.uvMatrix.v);
		}
	}

	std::expected<xmodel_source, std::string> to_neutral(const XModel& native)
	{
		try
		{
			require(native.name, "the model has no name");
			xmodel_source source;
			auto& data = source.model;
			data.name = native.name;
			data.scale = native.scale;
			data.radius = native.radius;
			data.edge_length = native.edgeLength;
			data.bounds = box(native.bounds);
			data.collision_lod = native.collLod;
			data.shadow_cutoff_lod = native.shadowCutoffLod;
			data.physics_use = enum_name(asset_data::xmodel_physics_uses, native.physicsUseCategory, "physics use category");
			data.impact_type = enum_name(asset_data::xmodel_impact_types, native.impactType, "impact type");
			data.mdao_type = enum_name(asset_data::xmodel_mdao_types, native.mdaoType, "MDAO type");

			const std::size_t bones = native.numBones + native.numClientBones;
			require(native.numRootBones <= native.numBones, "the model has more root bones than bones");
			const std::size_t links = bones - native.numRootBones;
			require((!bones || native.boneNames) && (!links || (native.parentList && native.quats && native.trans)),
				"the model has bones without names or rest poses");
			for (std::size_t bone = 0; bone < bones; ++bone)
			{
				auto& value = data.bones.emplace_back(asset_data::xmodel_bone{script_string(native.boneNames[bone], "bone"), {}});
				if (bone >= native.numRootBones)
				{
					const auto link = bone - native.numRootBones;
					const auto offset = native.parentList[link];
					require(offset && offset <= bone, "a bone's parent offset does not name an earlier bone");
					auto& pose = value.link.emplace();
					pose.parent = static_cast<std::uint32_t>(bone - offset);
					std::copy_n(native.quats + link * 4, 4, pose.rotation.begin());
					std::copy_n(native.trans + link * 3, 3, pose.translation.begin());
				}
			}

			data.client_bones = native.numClientBones;
			if (native.partClassification)
			{
				data.classifications.emplace(native.partClassification, native.partClassification + native.numBones);
			}

			if (native.baseMat)
			{
				auto& poses = data.base_poses.emplace();
				for (std::size_t bone = 0; bone < bones; ++bone)
				{
					const auto& value = native.baseMat[bone];
					poses.push_back({floats(value.quat.v), floats(value.trans.v), value.transWeight});
				}
			}

			if (native.boneInfo)
			{
				auto& infos = data.bone_infos.emplace();
				for (std::size_t bone = 0; bone < bones; ++bone)
				{
					infos.push_back({box(native.boneInfo[bone].bounds), native.boneInfo[bone].___u1.radiusSquared});
				}
			}

			require(!native.numAimAssistBones || native.aimAssistBones, "the model has aim assist bones without names");
			for (std::size_t bone = 0; bone < native.numAimAssistBones; ++bone)
			{
				data.aim_assist_bones.push_back(script_string(native.aimAssistBones[bone], "aim assist bone"));
			}

			if (native.ikHingeAxis)
			{
				auto& axes = data.ik_hinge_axes.emplace();
				for (std::size_t axis = 0; axis < axes.size(); ++axis)
				{
					axes[axis] = floats(native.ikHingeAxis[axis].v);
				}
			}

			std::memcpy(data.unscaled_parts.data(), native.noScalePartBits, sizeof(native.noScalePartBits));

			require(!native.numsurfs || native.materialHandles, "the model has surfaces without materials");
			for (std::size_t surface = 0; surface < native.numsurfs; ++surface)
			{
				data.materials.push_back(reference(ASSET_TYPE_MATERIAL, native.materialHandles[surface]));
			}

			if (native.himipRadiusInvSq)
			{
				data.high_mip_radius_inv_sq.emplace(native.himipRadiusInvSq, native.himipRadiusInvSq + native.numsurfs);
			}

			require(native.numLods <= lod_count, "the model has more levels of detail than Replay holds");
			std::array<std::int32_t, lod_count> lod_masks{};
			// Stock models give the slots past their count a distance (1e6): its meaning is not mapped.
			std::array<float, lod_count> unused_distances{};
			for (std::size_t lod = 0; lod < lod_count; ++lod)
			{
				const auto& value = native.lodInfo[lod];
				lod_masks[lod] = value.subdivLodValidMask;
				if (lod >= native.numLods)
				{
					if (value.modelSurfsStaging || value.numsurfs || value.surfIndex || value.flags || !zero_bytes(&value.partBits, sizeof(value.partBits)))
					{
						throw std::runtime_error(std::format("level {} past the count {} holds surfaces {}, {} surfaces at {}, flags {}, part bits {}",
							lod, native.numLods, value.modelSurfsStaging != nullptr, value.numsurfs, value.surfIndex, static_cast<int>(value.flags),
							!zero_bytes(&value.partBits, sizeof(value.partBits))));
					}

					unused_distances[lod] = value.dist;
					continue;
				}

				auto& target = data.lods.emplace_back();
				target.surfaces = reference(ASSET_TYPE_XMODEL_SURFS, value.modelSurfsStaging);
				target.distance = value.dist;
				target.first_surface = value.surfIndex;
				target.surface_count = value.numsurfs;
				std::memcpy(target.part_bits.data(), &value.partBits, sizeof(value.partBits));
				target.flags = flag_names(lod_flags, value.flags, "level of detail");
			}

			data.physics_asset = reference(ASSET_TYPE_PHYSICSASSET, native.physicsAsset);
			data.physics_fx_shape = reference(ASSET_TYPE_PHYSICS_FX_SHAPE, native.physicsFXShape);
			data.detail_collision = reference(ASSET_TYPE_XMODEL_DETAIL_COLLISION, native.detailCollision);
			data.scriptable_mover = reference(ASSET_TYPE_SCRIPTABLE, native.scriptableMoverDef);
			data.procedural_bones = reference(ASSET_TYPE_XANIM_PROCEDURALBONES, native.proceduralBones);
			data.dynamic_bones = reference(ASSET_TYPE_XANIM_DYNAMICBONES, native.dynamicBones);

			require(!native.numClothAssets || native.clothAssets, "the model has cloth without assets");
			for (std::size_t cloth = 0; cloth < native.numClothAssets; ++cloth)
			{
				data.cloth.push_back(reference(ASSET_TYPE_CLOTHASSET, native.clothAssets[cloth]));
			}

			if (native.reactiveMotionInfo)
			{
				data.reactive_motion = read_reactive(*native.reactiveMotionInfo);
			}

			if (const auto* shapes = native.blendShapeInfo)
			{
				require((!shapes->numberOfWeights || shapes->weightNames) && (!shapes->numberOfWeightMaps || shapes->weightMaps),
					"the blend shapes have counts without data");
				auto& value = data.blend_shapes.emplace();
				value.targets = shapes->targetCount;
				for (std::size_t weight = 0; weight < shapes->numberOfWeights; ++weight)
				{
					value.weights.push_back(script_string(shapes->weightNames[weight], "blend shape weight"));
				}

				for (std::size_t map = 0; map < shapes->numberOfWeightMaps; ++map)
				{
					value.maps.push_back({shapes->weightMaps[map].weightIndex, shapes->weightMaps[map].targetIndex, shapes->weightMaps[map].fullWeight});
				}
			}

			require(!native.mdaoVolumeCount || native.mdaoVolumes, "the model has MDAO volumes without data");
			for (std::size_t volume = 0; volume < native.mdaoVolumeCount; ++volume)
			{
				const auto& value = native.mdaoVolumes[volume];
				data.mdao_volumes.push_back({floats(value.bounds.mins.v), floats(value.bounds.maxs.v),
					{value.cellCount[0], value.cellCount[1], value.cellCount[2]}, value.parentBoneIndex, reference(ASSET_TYPE_IMAGE, value.volumeData)});
			}

			if (const auto* decals = native.decalVolumesInfo)
			{
				require(!decals->numDecalVolumes || decals->decalVolumes, "the model has decal volumes without data");
				auto& volumes = data.decal_volumes.emplace();
				for (std::size_t volume = 0; volume < decals->numDecalVolumes; ++volume)
				{
					volumes.push_back(read_decal(decals->decalVolumes[volume]));
				}
			}

			source.provenance = {xmodel_profile, "xmodel", data.name, {{"flags", asset_data::hex(native.flags)},
				{"contents", asset_data::hex(native.contents)}, {"characterCollBoundsType", asset_data::hex(native.characterCollBoundsType)},
				{"lgvData", asset_data::hex(native.lgvData)}, {"physicsUsageCounter", asset_data::hex(native.physicsUsageCounter)},
				{"runtime_0x298", asset_data::hex(native.runtime_0x298)}, {"subdivLodValidMask", asset_data::hex(lod_masks)},
				{"unusedLodDistances", asset_data::hex(unused_distances)}}};
			asset_data::validate(data);
			return source;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	XModel* from_neutral(const asset_data::xmodel& data, const asset_data::provenance* source, const asset_resolver& resolve,
		zone_memory& memory)
	{
		asset_data::validate(data);
		if (!source || source->profile != xmodel_profile)
		{
			throw std::runtime_error("model " + data.name + " has no Replay flags and contents");
		}

		const auto bones = data.bones.size();
		const auto roots = static_cast<std::size_t>(std::ranges::count_if(data.bones, [](const asset_data::xmodel_bone& bone) { return !bone.link; }));
		const auto animated = bones - data.client_bones;
		if (animated > 0xFF || roots > animated || data.client_bones > 0xFFFF || data.materials.size() > 0xFFFF ||
			data.lods.size() > lod_count || data.aim_assist_bones.size() > 0xFF || data.cloth.size() > 0xFF ||
			data.mdao_volumes.size() > 0xFFFF || data.collision_lod > 0xFF || data.shadow_cutoff_lod > 0xFF)
		{
			throw std::runtime_error("model " + data.name + " exceeds Replay's bone, surface, level or volume counts");
		}

		auto* native = memory.allocate<XModel>();
		native->name = copy_text(memory, data.name);
		native->scale = data.scale;
		native->radius = data.radius;
		native->edgeLength = data.edge_length;
		native->bounds = native_box(data.bounds);
		native->collLod = static_cast<std::uint8_t>(data.collision_lod);
		native->shadowCutoffLod = static_cast<std::uint8_t>(data.shadow_cutoff_lod);
		native->physicsUseCategory = static_cast<XModelPhysicsUseCategory>(enum_value(asset_data::xmodel_physics_uses, data.physics_use));
		native->impactType = static_cast<ImpactEffectType>(enum_value(asset_data::xmodel_impact_types, data.impact_type));
		native->mdaoType = static_cast<MDAOType>(enum_value(asset_data::xmodel_mdao_types, data.mdao_type));
		native->flags = source->value<std::uint32_t>("flags");
		native->contents = source->value<std::int32_t>("contents");
		native->characterCollBoundsType = source->value<char>("characterCollBoundsType");
		native->lgvData = source->value<std::uint32_t>("lgvData");
		native->physicsUsageCounter = source->value<XModelPhysicsUsageCounter>("physicsUsageCounter");
		native->runtime_0x298 = source->value<Replay_XModelRuntimeWord>("runtime_0x298");
		const auto lod_masks = source->value<std::array<std::int32_t, lod_count>>("subdivLodValidMask");
		const auto unused_distances = source->value<std::array<float, lod_count>>("unusedLodDistances");

		native->numBones = static_cast<std::uint8_t>(animated);
		native->numRootBones = static_cast<std::uint8_t>(roots);
		native->numClientBones = static_cast<std::uint16_t>(data.client_bones);
		native->boneNames = allocate_array<scr_string_t>(memory, bones);
		native->parentList = allocate_array<std::uint8_t>(memory, bones - roots);
		native->quats = allocate_array<std::int16_t>(memory, (bones - roots) * 4);
		native->trans = allocate_array<float>(memory, (bones - roots) * 3);
		for (std::size_t bone = 0; bone < bones; ++bone)
		{
			native->boneNames[bone] = script_string_id(data.bones[bone].name);
			if (const auto& link = data.bones[bone].link)
			{
				const auto index = bone - roots;
				if (bone - link->parent > 0xFF)
				{
					throw std::runtime_error("model " + data.name + " has a bone more than 255 bones after its parent");
				}

				native->parentList[index] = static_cast<std::uint8_t>(bone - link->parent);
				std::ranges::copy(link->rotation, native->quats + index * 4);
				std::ranges::copy(link->translation, native->trans + index * 3);
			}
		}

		if (data.classifications)
		{
			native->partClassification = allocate_array<std::uint8_t>(memory, std::max<std::size_t>(data.classifications->size(), 1));
			std::ranges::copy(*data.classifications, native->partClassification);
		}

		if (data.base_poses)
		{
			native->baseMat = allocate_array<DObjAnimMat>(memory, std::max<std::size_t>(bones, 1));
			for (std::size_t bone = 0; bone < bones; ++bone)
			{
				const auto& value = (*data.base_poses)[bone];
				std::ranges::copy(value.rotation, native->baseMat[bone].quat.v);
				std::ranges::copy(value.translation, native->baseMat[bone].trans.v);
				native->baseMat[bone].transWeight = value.translation_weight;
			}
		}

		if (data.bone_infos)
		{
			native->boneInfo = allocate_array<XBoneInfo>(memory, std::max<std::size_t>(bones, 1));
			for (std::size_t bone = 0; bone < bones; ++bone)
			{
				native->boneInfo[bone].bounds = native_box((*data.bone_infos)[bone].bounds);
				native->boneInfo[bone].___u1.radiusSquared = (*data.bone_infos)[bone].radius_squared;
			}
		}

		native->numAimAssistBones = static_cast<std::uint8_t>(data.aim_assist_bones.size());
		native->aimAssistBones = allocate_array<scr_string_t>(memory, data.aim_assist_bones.size());
		std::ranges::transform(data.aim_assist_bones, native->aimAssistBones, script_string_id);

		if (data.ik_hinge_axes)
		{
			native->ikHingeAxis = allocate_array<vec3_t>(memory, data.ik_hinge_axes->size());
			for (std::size_t axis = 0; axis < data.ik_hinge_axes->size(); ++axis)
			{
				std::ranges::copy((*data.ik_hinge_axes)[axis], native->ikHingeAxis[axis].v);
			}
		}

		std::memcpy(native->noScalePartBits, data.unscaled_parts.data(), sizeof(native->noScalePartBits));

		native->numsurfs = static_cast<std::uint16_t>(data.materials.size());
		native->materialHandles = allocate_array<Material*>(memory, data.materials.size());
		for (std::size_t surface = 0; surface < data.materials.size(); ++surface)
		{
			native->materialHandles[surface] = resolved<Material>(resolve, ASSET_TYPE_MATERIAL, data.materials[surface]);
		}

		if (data.high_mip_radius_inv_sq)
		{
			native->himipRadiusInvSq = allocate_array<float>(memory, std::max<std::size_t>(data.high_mip_radius_inv_sq->size(), 1));
			std::ranges::copy(*data.high_mip_radius_inv_sq, native->himipRadiusInvSq);
		}

		native->numLods = static_cast<std::uint8_t>(data.lods.size());
		for (std::size_t lod = 0; lod < lod_count; ++lod)
		{
			auto& target = native->lodInfo[lod];
			target.subdivLodValidMask = lod_masks[lod];
			if (lod >= data.lods.size())
			{
				target.dist = unused_distances[lod];
				continue;
			}

			const auto& value = data.lods[lod];
			if (value.first_surface > 0xFFFF || value.surface_count > 0xFFFF)
			{
				throw std::runtime_error("model " + data.name + " has a level of detail beyond Replay's surface counts");
			}

			target.modelSurfsStaging = resolved<XModelSurfs>(resolve, ASSET_TYPE_XMODEL_SURFS, value.surfaces);
			target.dist = value.distance;
			target.surfIndex = static_cast<std::uint16_t>(value.first_surface);
			target.numsurfs = static_cast<std::uint16_t>(value.surface_count);
			std::memcpy(&target.partBits, value.part_bits.data(), sizeof(target.partBits));
			target.flags = static_cast<XModelLodInfoFlags>(flag_bits(lod_flags, value.flags));
		}

		native->physicsAsset = resolved<PhysicsAsset>(resolve, ASSET_TYPE_PHYSICSASSET, data.physics_asset);
		native->physicsFXShape = resolved<PhysicsFXShape>(resolve, ASSET_TYPE_PHYSICS_FX_SHAPE, data.physics_fx_shape);
		native->detailCollision = resolved<XModelDetailCollision>(resolve, ASSET_TYPE_XMODEL_DETAIL_COLLISION, data.detail_collision);
		native->scriptableMoverDef = resolved<ScriptableDef>(resolve, ASSET_TYPE_SCRIPTABLE, data.scriptable_mover);
		native->proceduralBones = resolved<XAnimProceduralBones>(resolve, ASSET_TYPE_XANIM_PROCEDURALBONES, data.procedural_bones);
		native->dynamicBones = resolved<XAnimDynamicBones>(resolve, ASSET_TYPE_XANIM_DYNAMICBONES, data.dynamic_bones);

		native->numClothAssets = static_cast<std::uint8_t>(data.cloth.size());
		native->clothAssets = allocate_array<ClothAsset*>(memory, data.cloth.size());
		for (std::size_t cloth = 0; cloth < data.cloth.size(); ++cloth)
		{
			native->clothAssets[cloth] = resolved<ClothAsset>(resolve, ASSET_TYPE_CLOTHASSET, data.cloth[cloth]);
		}

		if (data.reactive_motion)
		{
			native->reactiveMotionInfo = write_reactive(*data.reactive_motion, memory);
		}

		if (data.blend_shapes)
		{
			const auto& value = *data.blend_shapes;
			if (value.targets > 0xFFFF || value.weights.size() > 0xFFFF || value.maps.size() > 0xFFFF)
			{
				throw std::runtime_error("model " + data.name + " exceeds Replay's blend shape counts");
			}

			auto* shapes = native->blendShapeInfo = memory.allocate<XModelBlendShapeInfo>();
			shapes->targetCount = static_cast<std::uint16_t>(value.targets);
			shapes->numberOfWeights = static_cast<std::uint16_t>(value.weights.size());
			shapes->numberOfWeightMaps = static_cast<std::uint16_t>(value.maps.size());
			shapes->weightNames = allocate_array<scr_string_t>(memory, value.weights.size());
			std::ranges::transform(value.weights, shapes->weightNames, script_string_id);
			shapes->weightMaps = allocate_array<BlendShapeWeightMap>(memory, value.maps.size());
			for (std::size_t map = 0; map < value.maps.size(); ++map)
			{
				shapes->weightMaps[map] = {value.maps[map].weight, value.maps[map].target, value.maps[map].full_weight};
			}
		}

		native->mdaoVolumeCount = static_cast<std::uint16_t>(data.mdao_volumes.size());
		native->mdaoVolumes = allocate_array<MdaoVolume>(memory, data.mdao_volumes.size());
		for (std::size_t volume = 0; volume < data.mdao_volumes.size(); ++volume)
		{
			const auto& value = data.mdao_volumes[volume];
			auto& target = native->mdaoVolumes[volume];
			std::ranges::copy(value.mins, target.bounds.mins.v);
			std::ranges::copy(value.maxs, target.bounds.maxs.v);
			std::ranges::copy(value.cells, target.cellCount);
			target.parentBoneIndex = value.parent_bone;
			target.volumeData = resolved<GfxImage>(resolve, ASSET_TYPE_IMAGE, value.image);
		}

		if (data.decal_volumes)
		{
			auto* decals = native->decalVolumesInfo = memory.allocate<XModelDecalVolumesInfo>();
			decals->numDecalVolumes = static_cast<std::uint32_t>(data.decal_volumes->size());
			decals->decalVolumes = allocate_array<XModelDecalVolume>(memory, data.decal_volumes->size());
			for (std::size_t volume = 0; volume < data.decal_volumes->size(); ++volume)
			{
				write_decal((*data.decal_volumes)[volume], decals->decalVolumes[volume], resolve);
			}
		}

		return native;
	}

	std::size_t native_size(const asset_data::xmodel& data)
	{
		// An upper bound: each array rounds up to 8 bytes.
		std::size_t size = rounded(sizeof(XModel)) + rounded(data.name.size() + 1) + 16 * 8;
		size += data.bones.size() * (sizeof(scr_string_t) + 1 + 4 * sizeof(std::int16_t) + 3 * sizeof(float) + 1 + sizeof(DObjAnimMat) +
			sizeof(XBoneInfo)) + 8 * 8;
		size += data.aim_assist_bones.size() * sizeof(scr_string_t) + 4 * sizeof(vec3_t);
		size += data.materials.size() * (sizeof(Material*) + sizeof(float)) + data.cloth.size() * sizeof(ClothAsset*);
		if (data.reactive_motion)
		{
			size += rounded(sizeof(ReactiveMotionModelInfo)) + data.reactive_motion->parts.size() * (sizeof(ReactiveMotionModelPart) +
				sizeof(ReactiveMotionModelPartTweaks)) + 16;
		}

		if (data.blend_shapes)
		{
			size += rounded(sizeof(XModelBlendShapeInfo)) + data.blend_shapes->weights.size() * sizeof(scr_string_t) +
				data.blend_shapes->maps.size() * sizeof(BlendShapeWeightMap) + 16;
		}

		size += data.mdao_volumes.size() * sizeof(MdaoVolume) + rounded(sizeof(XModelDecalVolumesInfo)) +
			data.decal_volumes.value_or(std::vector<asset_data::xmodel_decal_volume>{}).size() * sizeof(XModelDecalVolume);
		return size;
	}
}
