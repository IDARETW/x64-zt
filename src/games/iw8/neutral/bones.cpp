// Emitted by tools/gen_iw8_neutral.py from tools/neutral_families/bones.json: do not edit.
#include <std_include.hpp>
#include "bones.hpp"
#include "plain.hpp"

namespace zonetool::iw8::neutral
{
	namespace
	{
		asset_data::bones::dynamicbones read_dynamicbones(const XAnimDynamicBones& native);
		void write_dynamicbones(XAnimDynamicBones& native, const asset_data::bones::dynamicbones& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_dynamicbones(const asset_data::bones::dynamicbones& data);
		asset_data::bones::x_anim_dynamic_bone_properties read_x_anim_dynamic_bone_properties(const XAnimDynamicBoneProperties& native);
		void write_x_anim_dynamic_bone_properties(XAnimDynamicBoneProperties& native, const asset_data::bones::x_anim_dynamic_bone_properties& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_x_anim_dynamic_bone_properties(const asset_data::bones::x_anim_dynamic_bone_properties& data);
		asset_data::bones::snd_alias_lookup read_snd_alias_lookup(const SndAliasLookup& native);
		void write_snd_alias_lookup(SndAliasLookup& native, const asset_data::bones::snd_alias_lookup& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_snd_alias_lookup(const asset_data::bones::snd_alias_lookup& data);
		asset_data::bones::x_anim_rotational_spring_properties read_x_anim_rotational_spring_properties(const XAnimRotationalSpringProperties& native);
		void write_x_anim_rotational_spring_properties(XAnimRotationalSpringProperties& native, const asset_data::bones::x_anim_rotational_spring_properties& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_x_anim_rotational_spring_properties(const asset_data::bones::x_anim_rotational_spring_properties& data);
		asset_data::bones::x_anim_translational_spring_properties read_x_anim_translational_spring_properties(const XAnimTranslationalSpringProperties& native);
		void write_x_anim_translational_spring_properties(XAnimTranslationalSpringProperties& native, const asset_data::bones::x_anim_translational_spring_properties& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_x_anim_translational_spring_properties(const asset_data::bones::x_anim_translational_spring_properties& data);
		asset_data::bones::x_anim_dynamic_bone_noise read_x_anim_dynamic_bone_noise(const XAnimDynamicBoneNoise& native);
		void write_x_anim_dynamic_bone_noise(XAnimDynamicBoneNoise& native, const asset_data::bones::x_anim_dynamic_bone_noise& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_x_anim_dynamic_bone_noise(const asset_data::bones::x_anim_dynamic_bone_noise& data);
		asset_data::bones::x_anim_dynamic_bone_noise_properties read_x_anim_dynamic_bone_noise_properties(const XAnimDynamicBoneNoiseProperties& native);
		void write_x_anim_dynamic_bone_noise_properties(XAnimDynamicBoneNoiseProperties& native, const asset_data::bones::x_anim_dynamic_bone_noise_properties& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_x_anim_dynamic_bone_noise_properties(const asset_data::bones::x_anim_dynamic_bone_noise_properties& data);
		asset_data::bones::x_anim_dynamic_bone_noise_axis read_x_anim_dynamic_bone_noise_axis(const XAnimDynamicBoneNoiseAxis& native);
		void write_x_anim_dynamic_bone_noise_axis(XAnimDynamicBoneNoiseAxis& native, const asset_data::bones::x_anim_dynamic_bone_noise_axis& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_x_anim_dynamic_bone_noise_axis(const asset_data::bones::x_anim_dynamic_bone_noise_axis& data);
		asset_data::bones::x_anim_collidable_bone read_x_anim_collidable_bone(const XAnimCollidableBone& native);
		void write_x_anim_collidable_bone(XAnimCollidableBone& native, const asset_data::bones::x_anim_collidable_bone& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_x_anim_collidable_bone(const asset_data::bones::x_anim_collidable_bone& data);
		asset_data::bones::x_anim_collision_shape read_x_anim_collision_shape(const XAnimCollisionShape& native);
		void write_x_anim_collision_shape(XAnimCollisionShape& native, const asset_data::bones::x_anim_collision_shape& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_x_anim_collision_shape(const asset_data::bones::x_anim_collision_shape& data);
		asset_data::bones::x_anim_sphere_collision_shape read_x_anim_sphere_collision_shape(const XAnimSphereCollisionShape& native);
		void write_x_anim_sphere_collision_shape(XAnimSphereCollisionShape& native, const asset_data::bones::x_anim_sphere_collision_shape& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_x_anim_sphere_collision_shape(const asset_data::bones::x_anim_sphere_collision_shape& data);
		asset_data::bones::x_anim_capsule_collision_shape read_x_anim_capsule_collision_shape(const XAnimCapsuleCollisionShape& native);
		void write_x_anim_capsule_collision_shape(XAnimCapsuleCollisionShape& native, const asset_data::bones::x_anim_capsule_collision_shape& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_x_anim_capsule_collision_shape(const asset_data::bones::x_anim_capsule_collision_shape& data);
		asset_data::bones::x_anim_box_collision_shape read_x_anim_box_collision_shape(const XAnimBoxCollisionShape& native);
		void write_x_anim_box_collision_shape(XAnimBoxCollisionShape& native, const asset_data::bones::x_anim_box_collision_shape& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_x_anim_box_collision_shape(const asset_data::bones::x_anim_box_collision_shape& data);
		asset_data::bones::x_anim_custom_pin_constraint read_x_anim_custom_pin_constraint(const XAnimCustomPinConstraint& native);
		void write_x_anim_custom_pin_constraint(XAnimCustomPinConstraint& native, const asset_data::bones::x_anim_custom_pin_constraint& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_x_anim_custom_pin_constraint(const asset_data::bones::x_anim_custom_pin_constraint& data);
		asset_data::bones::proceduralbones read_proceduralbones(const XAnimProceduralBones& native);
		void write_proceduralbones(XAnimProceduralBones& native, const asset_data::bones::proceduralbones& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_proceduralbones(const asset_data::bones::proceduralbones& data);
		asset_data::bones::x_anim_constraint read_x_anim_constraint(const XAnimConstraint& native);
		void write_x_anim_constraint(XAnimConstraint& native, const asset_data::bones::x_anim_constraint& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_x_anim_constraint(const asset_data::bones::x_anim_constraint& data);
		asset_data::bones::x_anim_position_constraint read_x_anim_position_constraint(const XAnimPositionConstraint& native);
		void write_x_anim_position_constraint(XAnimPositionConstraint& native, const asset_data::bones::x_anim_position_constraint& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_x_anim_position_constraint(const asset_data::bones::x_anim_position_constraint& data);
		asset_data::bones::x_anim_rotation_constraint read_x_anim_rotation_constraint(const XAnimRotationConstraint& native);
		void write_x_anim_rotation_constraint(XAnimRotationConstraint& native, const asset_data::bones::x_anim_rotation_constraint& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_x_anim_rotation_constraint(const asset_data::bones::x_anim_rotation_constraint& data);
		asset_data::bones::x_anim_aim_constraint read_x_anim_aim_constraint(const XAnimAimConstraint& native);
		void write_x_anim_aim_constraint(XAnimAimConstraint& native, const asset_data::bones::x_anim_aim_constraint& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_x_anim_aim_constraint(const asset_data::bones::x_anim_aim_constraint& data);
		asset_data::bones::distance_driven_translation_offset_constraint read_distance_driven_translation_offset_constraint(const XAnimDistanceDrivenOffsetConstraint& native);
		void write_distance_driven_translation_offset_constraint(XAnimDistanceDrivenOffsetConstraint& native, const asset_data::bones::distance_driven_translation_offset_constraint& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_distance_driven_translation_offset_constraint(const asset_data::bones::distance_driven_translation_offset_constraint& data);
		asset_data::bones::distance_driven_rotation_offset_constraint read_distance_driven_rotation_offset_constraint(const XAnimDistanceDrivenOffsetConstraint& native);
		void write_distance_driven_rotation_offset_constraint(XAnimDistanceDrivenOffsetConstraint& native, const asset_data::bones::distance_driven_rotation_offset_constraint& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_distance_driven_rotation_offset_constraint(const asset_data::bones::distance_driven_rotation_offset_constraint& data);

		asset_data::bones::dynamicbones read_dynamicbones(const XAnimDynamicBones& native)
		{
			plain::trace("read_dynamicbones");
			asset_data::bones::dynamicbones data;
			data.name = plain::read_string(native.name);
			data.dynamic_bone_properties = plain::read_array(native.dynamicBoneProperties, native.numDynamicBones, "XAnimDynamicBones.dynamicBoneProperties", [&](const auto& item) { return read_x_anim_dynamic_bone_properties(item); });
			data.collidable_bones = plain::read_array(native.collidableBones, native.numCollidableBones, "XAnimDynamicBones.collidableBones", [&](const auto& item) { return read_x_anim_collidable_bone(item); });
			data.num_collidable_bone_collision_shapes = native.numCollidableBoneCollisionShapes;
			data.collision_shapes = plain::read_array(native.collisionShapes, native.numCollisionShapes, "XAnimDynamicBones.collisionShapes", [&](const auto& item) { return read_x_anim_collision_shape(item); });
			data.custom_pin_constraints = plain::read_array(native.customPinConstraints, native.numCustomPinConstraints, "XAnimDynamicBones.customPinConstraints", [&](const auto& item) { return read_x_anim_custom_pin_constraint(item); });
			data.pin_constraint_bone_names = plain::read_array(native.pinConstraintBoneNames, native.numPinConstraintBoneNames, "XAnimDynamicBones.pinConstraintBoneNames", [&](const auto& item) { return plain::read_script_string(item); });
			return data;
		}

		void write_dynamicbones(XAnimDynamicBones& native, const asset_data::bones::dynamicbones& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.dynamicBoneProperties = plain::write_array<XAnimDynamicBoneProperties>(data.dynamic_bone_properties, memory, [&](auto& out, const auto& in) { write_x_anim_dynamic_bone_properties(out, in, resolve, memory); });
			native.collidableBones = plain::write_array<XAnimCollidableBone>(data.collidable_bones, memory, [&](auto& out, const auto& in) { write_x_anim_collidable_bone(out, in, resolve, memory); });
			native.numCollidableBoneCollisionShapes = data.num_collidable_bone_collision_shapes;
			native.collisionShapes = plain::write_array<XAnimCollisionShape>(data.collision_shapes, memory, [&](auto& out, const auto& in) { write_x_anim_collision_shape(out, in, resolve, memory); });
			native.customPinConstraints = plain::write_array<XAnimCustomPinConstraint>(data.custom_pin_constraints, memory, [&](auto& out, const auto& in) { write_x_anim_custom_pin_constraint(out, in, resolve, memory); });
			native.pinConstraintBoneNames = plain::write_array<scr_string_t>(data.pin_constraint_bone_names, memory, [&](auto& out, const auto& in) { out = static_cast<scr_string_t>(plain::write_script_string(in)); });
			native.numDynamicBones = plain::count_of<std::uint32_t>(data.dynamic_bone_properties);
			native.numCollidableBones = plain::count_of<std::uint32_t>(data.collidable_bones);
			native.numCollisionShapes = plain::count_of<std::uint32_t>(data.collision_shapes);
			native.numCustomPinConstraints = plain::count_of<std::uint32_t>(data.custom_pin_constraints);
			native.numPinConstraintBoneNames = plain::count_of<std::uint32_t>(data.pin_constraint_bone_names);
		}

		std::size_t size_dynamicbones([[maybe_unused]] const asset_data::bones::dynamicbones& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.dynamic_bone_properties)
			{
				size += plain::sized(std::max<std::size_t>(data.dynamic_bone_properties->size(), 1) * sizeof(XAnimDynamicBoneProperties));
				for (const auto& item : *data.dynamic_bone_properties)
				{
					size += size_x_anim_dynamic_bone_properties(item);
				}
			}
			if (data.collidable_bones)
			{
				size += plain::sized(std::max<std::size_t>(data.collidable_bones->size(), 1) * sizeof(XAnimCollidableBone));
				for (const auto& item : *data.collidable_bones)
				{
					size += size_x_anim_collidable_bone(item);
				}
			}
			if (data.collision_shapes)
			{
				size += plain::sized(std::max<std::size_t>(data.collision_shapes->size(), 1) * sizeof(XAnimCollisionShape));
				for (const auto& item : *data.collision_shapes)
				{
					size += size_x_anim_collision_shape(item);
				}
			}
			if (data.custom_pin_constraints)
			{
				size += plain::sized(std::max<std::size_t>(data.custom_pin_constraints->size(), 1) * sizeof(XAnimCustomPinConstraint));
				for (const auto& item : *data.custom_pin_constraints)
				{
					size += size_x_anim_custom_pin_constraint(item);
				}
			}
			if (data.pin_constraint_bone_names)
			{
				size += plain::sized(std::max<std::size_t>(data.pin_constraint_bone_names->size(), 1) * sizeof(scr_string_t));
			}
			return size;
		}

		asset_data::bones::snd_alias_lookup read_snd_alias_lookup(const SndAliasLookup& native)
		{
			plain::trace("read_snd_alias_lookup");
			asset_data::bones::snd_alias_lookup data;
			data.name = plain::read_string(native.name);
			return data;
		}

		void write_snd_alias_lookup(SndAliasLookup& native, const asset_data::bones::snd_alias_lookup& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
		}

		std::size_t size_snd_alias_lookup([[maybe_unused]] const asset_data::bones::snd_alias_lookup& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			return size;
		}

		asset_data::bones::x_anim_rotational_spring_properties read_x_anim_rotational_spring_properties(const XAnimRotationalSpringProperties& native)
		{
			plain::trace("read_x_anim_rotational_spring_properties");
			asset_data::bones::x_anim_rotational_spring_properties data;
			data.swing_strength = native.swingStrength;
			data.twist_strength = native.twistStrength;
			return data;
		}

		void write_x_anim_rotational_spring_properties(XAnimRotationalSpringProperties& native, const asset_data::bones::x_anim_rotational_spring_properties& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.swingStrength = data.swing_strength;
			native.twistStrength = data.twist_strength;
		}

		std::size_t size_x_anim_rotational_spring_properties([[maybe_unused]] const asset_data::bones::x_anim_rotational_spring_properties& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::bones::x_anim_translational_spring_properties read_x_anim_translational_spring_properties(const XAnimTranslationalSpringProperties& native)
		{
			plain::trace("read_x_anim_translational_spring_properties");
			asset_data::bones::x_anim_translational_spring_properties data;
			data.strength = plain::read_scalars<float, 3>(&native.strength);
			data.damping = plain::read_scalars<float, 3>(&native.damping);
			data.min_limit = plain::read_scalars<float, 3>(&native.minLimit);
			data.max_limit = plain::read_scalars<float, 3>(&native.maxLimit);
			return data;
		}

		void write_x_anim_translational_spring_properties(XAnimTranslationalSpringProperties& native, const asset_data::bones::x_anim_translational_spring_properties& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.strength, data.strength);
			plain::write_scalars(&native.damping, data.damping);
			plain::write_scalars(&native.minLimit, data.min_limit);
			plain::write_scalars(&native.maxLimit, data.max_limit);
		}

		std::size_t size_x_anim_translational_spring_properties([[maybe_unused]] const asset_data::bones::x_anim_translational_spring_properties& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::bones::x_anim_dynamic_bone_noise_axis read_x_anim_dynamic_bone_noise_axis(const XAnimDynamicBoneNoiseAxis& native)
		{
			plain::trace("read_x_anim_dynamic_bone_noise_axis");
			asset_data::bones::x_anim_dynamic_bone_noise_axis data;
			data.curve = plain::read_asset(ASSET_TYPE_XANIMCURVE, native.curve);
			data.strength = native.strength;
			data.rate = native.rate;
			data.start = native.start;
			return data;
		}

		void write_x_anim_dynamic_bone_noise_axis(XAnimDynamicBoneNoiseAxis& native, const asset_data::bones::x_anim_dynamic_bone_noise_axis& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.curve = plain::resolve_asset<XAnimCurve>(resolve, ASSET_TYPE_XANIMCURVE, data.curve);
			native.strength = data.strength;
			native.rate = data.rate;
			native.start = data.start;
		}

		std::size_t size_x_anim_dynamic_bone_noise_axis([[maybe_unused]] const asset_data::bones::x_anim_dynamic_bone_noise_axis& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::bones::x_anim_dynamic_bone_noise_properties read_x_anim_dynamic_bone_noise_properties(const XAnimDynamicBoneNoiseProperties& native)
		{
			plain::trace("read_x_anim_dynamic_bone_noise_properties");
			asset_data::bones::x_anim_dynamic_bone_noise_properties data;
			data.relative = plain::enum_name(asset_data::bones::x_anim_dynamic_bone_noise_axis_space_names, static_cast<std::int64_t>(native.relative), "XAnimDynamicBoneNoiseProperties.relative");
			data.axis = [&] { std::array<asset_data::bones::x_anim_dynamic_bone_noise_axis, 3> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_x_anim_dynamic_bone_noise_axis(native.axis[i]); } return out; }();
			return data;
		}

		void write_x_anim_dynamic_bone_noise_properties(XAnimDynamicBoneNoiseProperties& native, const asset_data::bones::x_anim_dynamic_bone_noise_properties& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.relative = static_cast<XAnimDynamicBoneNoiseAxisSpace>(plain::enum_value(asset_data::bones::x_anim_dynamic_bone_noise_axis_space_names, data.relative, "XAnimDynamicBoneNoiseProperties.relative"));
			for (std::size_t i = 0; i < data.axis.size(); ++i)
			{
				write_x_anim_dynamic_bone_noise_axis(native.axis[i], data.axis[i], resolve, memory);
			}
		}

		std::size_t size_x_anim_dynamic_bone_noise_properties([[maybe_unused]] const asset_data::bones::x_anim_dynamic_bone_noise_properties& data)
		{
			std::size_t size = 0;
			for (const auto& item : data.axis)
			{
				size += size_x_anim_dynamic_bone_noise_axis(item);
			}
			return size;
		}

		asset_data::bones::x_anim_dynamic_bone_noise read_x_anim_dynamic_bone_noise(const XAnimDynamicBoneNoise& native)
		{
			plain::trace("read_x_anim_dynamic_bone_noise");
			asset_data::bones::x_anim_dynamic_bone_noise data;
			data.enable = native.enable;
			data.translation = read_x_anim_dynamic_bone_noise_properties(native.translation);
			data.rotation = read_x_anim_dynamic_bone_noise_properties(native.rotation);
			return data;
		}

		void write_x_anim_dynamic_bone_noise(XAnimDynamicBoneNoise& native, const asset_data::bones::x_anim_dynamic_bone_noise& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.enable = data.enable;
			write_x_anim_dynamic_bone_noise_properties(native.translation, data.translation, resolve, memory);
			write_x_anim_dynamic_bone_noise_properties(native.rotation, data.rotation, resolve, memory);
		}

		std::size_t size_x_anim_dynamic_bone_noise([[maybe_unused]] const asset_data::bones::x_anim_dynamic_bone_noise& data)
		{
			std::size_t size = 0;
			size += size_x_anim_dynamic_bone_noise_properties(data.translation);
			size += size_x_anim_dynamic_bone_noise_properties(data.rotation);
			return size;
		}

		asset_data::bones::x_anim_dynamic_bone_properties read_x_anim_dynamic_bone_properties(const XAnimDynamicBoneProperties& native)
		{
			plain::trace("read_x_anim_dynamic_bone_properties");
			asset_data::bones::x_anim_dynamic_bone_properties data;
			data.bone_name = plain::read_script_string(native.boneName);
			data.parent_dynamic_bone_index = native.parentDynamicBoneIndex;
			data.dynamic_bone_chain_root_index = native.dynamicBoneChainRootIndex;
			data.chain_length = native.chainLength;
			data.inv_mass = native.invMass;
			data.center_of_mass = plain::read_scalars<float, 3>(&native.centerOfMass);
			data.inv_inertia_tensor = native.invInertiaTensor;
			data.twist_axis = plain::read_scalars<float, 3>(&native.twistAxis);
			data.min_twist_angle = native.minTwistAngle;
			data.max_twist_angle = native.maxTwistAngle;
			data.damping = native.damping;
			data.gravity_scale = native.gravityScale;
			data.cos_max_swing_angle = native.cosMaxSwingAngle;
			data.local_base_pose_quat = plain::read_scalars<float, 4>(&native.localBasePoseQuat);
			data.local_base_pose_trans = plain::read_scalars<float, 3>(&native.localBasePoseTrans);
			data.first_collision_shape_index = native.firstCollisionShapeIndex;
			data.num_collision_shapes = native.numCollisionShapes;
			data.light_hit_sound = read_snd_alias_lookup(native.lightHitSound);
			data.heavy_hit_sound = read_snd_alias_lookup(native.heavyHitSound);
			data.light_hit_threshold = native.lightHitThreshold;
			data.heavy_hit_threshold = native.heavyHitThreshold;
			data.enable_swing_limit = native.enableSwingLimit;
			data.enable_twist_limit = native.enableTwistLimit;
			data.enable_rotational_spring = native.enableRotationalSpring;
			data.enable_translational_spring = native.enableTranslationalSpring;
			data.rotational_spring = read_x_anim_rotational_spring_properties(native.rotationalSpring);
			data.translational_spring = read_x_anim_translational_spring_properties(native.translationalSpring);
			data.noise = read_x_anim_dynamic_bone_noise(native.noise);
			return data;
		}

		void write_x_anim_dynamic_bone_properties(XAnimDynamicBoneProperties& native, const asset_data::bones::x_anim_dynamic_bone_properties& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.boneName = static_cast<scr_string_t>(plain::write_script_string(data.bone_name));
			native.parentDynamicBoneIndex = data.parent_dynamic_bone_index;
			native.dynamicBoneChainRootIndex = data.dynamic_bone_chain_root_index;
			native.chainLength = data.chain_length;
			native.invMass = data.inv_mass;
			plain::write_scalars(&native.centerOfMass, data.center_of_mass);
			native.invInertiaTensor = data.inv_inertia_tensor;
			plain::write_scalars(&native.twistAxis, data.twist_axis);
			native.minTwistAngle = data.min_twist_angle;
			native.maxTwistAngle = data.max_twist_angle;
			native.damping = data.damping;
			native.gravityScale = data.gravity_scale;
			native.cosMaxSwingAngle = data.cos_max_swing_angle;
			plain::write_scalars(&native.localBasePoseQuat, data.local_base_pose_quat);
			plain::write_scalars(&native.localBasePoseTrans, data.local_base_pose_trans);
			native.firstCollisionShapeIndex = data.first_collision_shape_index;
			native.numCollisionShapes = data.num_collision_shapes;
			write_snd_alias_lookup(native.lightHitSound, data.light_hit_sound, resolve, memory);
			write_snd_alias_lookup(native.heavyHitSound, data.heavy_hit_sound, resolve, memory);
			native.lightHitThreshold = data.light_hit_threshold;
			native.heavyHitThreshold = data.heavy_hit_threshold;
			native.enableSwingLimit = data.enable_swing_limit;
			native.enableTwistLimit = data.enable_twist_limit;
			native.enableRotationalSpring = data.enable_rotational_spring;
			native.enableTranslationalSpring = data.enable_translational_spring;
			write_x_anim_rotational_spring_properties(native.rotationalSpring, data.rotational_spring, resolve, memory);
			write_x_anim_translational_spring_properties(native.translationalSpring, data.translational_spring, resolve, memory);
			write_x_anim_dynamic_bone_noise(native.noise, data.noise, resolve, memory);
		}

		std::size_t size_x_anim_dynamic_bone_properties([[maybe_unused]] const asset_data::bones::x_anim_dynamic_bone_properties& data)
		{
			std::size_t size = 0;
			size += size_snd_alias_lookup(data.light_hit_sound);
			size += size_snd_alias_lookup(data.heavy_hit_sound);
			size += size_x_anim_rotational_spring_properties(data.rotational_spring);
			size += size_x_anim_translational_spring_properties(data.translational_spring);
			size += size_x_anim_dynamic_bone_noise(data.noise);
			return size;
		}

		asset_data::bones::x_anim_collidable_bone read_x_anim_collidable_bone(const XAnimCollidableBone& native)
		{
			plain::trace("read_x_anim_collidable_bone");
			asset_data::bones::x_anim_collidable_bone data;
			data.bone_name = plain::read_script_string(native.boneName);
			data.first_collision_shape_index = native.firstCollisionShapeIndex;
			data.num_collision_shapes = native.numCollisionShapes;
			return data;
		}

		void write_x_anim_collidable_bone(XAnimCollidableBone& native, const asset_data::bones::x_anim_collidable_bone& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.boneName = static_cast<scr_string_t>(plain::write_script_string(data.bone_name));
			native.firstCollisionShapeIndex = data.first_collision_shape_index;
			native.numCollisionShapes = data.num_collision_shapes;
		}

		std::size_t size_x_anim_collidable_bone([[maybe_unused]] const asset_data::bones::x_anim_collidable_bone& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::bones::x_anim_sphere_collision_shape read_x_anim_sphere_collision_shape(const XAnimSphereCollisionShape& native)
		{
			plain::trace("read_x_anim_sphere_collision_shape");
			asset_data::bones::x_anim_sphere_collision_shape data;
			data.center = plain::read_scalars<float, 3>(&native.center);
			data.radius = native.radius;
			return data;
		}

		void write_x_anim_sphere_collision_shape(XAnimSphereCollisionShape& native, const asset_data::bones::x_anim_sphere_collision_shape& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.center, data.center);
			native.radius = data.radius;
		}

		std::size_t size_x_anim_sphere_collision_shape([[maybe_unused]] const asset_data::bones::x_anim_sphere_collision_shape& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::bones::x_anim_capsule_collision_shape read_x_anim_capsule_collision_shape(const XAnimCapsuleCollisionShape& native)
		{
			plain::trace("read_x_anim_capsule_collision_shape");
			asset_data::bones::x_anim_capsule_collision_shape data;
			data.start = plain::read_scalars<float, 3>(&native.start);
			data.end = plain::read_scalars<float, 3>(&native.end);
			data.radius = native.radius;
			return data;
		}

		void write_x_anim_capsule_collision_shape(XAnimCapsuleCollisionShape& native, const asset_data::bones::x_anim_capsule_collision_shape& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.start, data.start);
			plain::write_scalars(&native.end, data.end);
			native.radius = data.radius;
		}

		std::size_t size_x_anim_capsule_collision_shape([[maybe_unused]] const asset_data::bones::x_anim_capsule_collision_shape& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::bones::x_anim_box_collision_shape read_x_anim_box_collision_shape(const XAnimBoxCollisionShape& native)
		{
			plain::trace("read_x_anim_box_collision_shape");
			asset_data::bones::x_anim_box_collision_shape data;
			data.center = plain::read_scalars<float, 3>(&native.center);
			data.extents = plain::read_scalars<float, 3>(&native.extents);
			return data;
		}

		void write_x_anim_box_collision_shape(XAnimBoxCollisionShape& native, const asset_data::bones::x_anim_box_collision_shape& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.center, data.center);
			plain::write_scalars(&native.extents, data.extents);
		}

		std::size_t size_x_anim_box_collision_shape([[maybe_unused]] const asset_data::bones::x_anim_box_collision_shape& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::bones::x_anim_collision_shape read_x_anim_collision_shape(const XAnimCollisionShape& native)
		{
			plain::trace("read_x_anim_collision_shape");
			asset_data::bones::x_anim_collision_shape data;
			data.u = [&]() -> std::variant<std::monostate, asset_data::bones::x_anim_sphere_collision_shape, asset_data::bones::x_anim_capsule_collision_shape, asset_data::bones::x_anim_box_collision_shape> { switch (static_cast<std::uint32_t>(native.type)) { case 0: { plain::module_scope scope(0); auto item = read_x_anim_sphere_collision_shape(native.u.sphere); return std::variant<std::monostate, asset_data::bones::x_anim_sphere_collision_shape, asset_data::bones::x_anim_capsule_collision_shape, asset_data::bones::x_anim_box_collision_shape>(std::move(item)); } case 1: { plain::module_scope scope(1); auto item = read_x_anim_capsule_collision_shape(native.u.capsule); return std::variant<std::monostate, asset_data::bones::x_anim_sphere_collision_shape, asset_data::bones::x_anim_capsule_collision_shape, asset_data::bones::x_anim_box_collision_shape>(std::move(item)); } case 2: { plain::module_scope scope(2); auto item = read_x_anim_box_collision_shape(native.u.box); return std::variant<std::monostate, asset_data::bones::x_anim_sphere_collision_shape, asset_data::bones::x_anim_capsule_collision_shape, asset_data::bones::x_anim_box_collision_shape>(std::move(item)); } default: plain::fail("XAnimCollisionShape.u", "has a tag value without a case"); } }();
			return data;
		}

		void write_x_anim_collision_shape(XAnimCollisionShape& native, const asset_data::bones::x_anim_collision_shape& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			std::visit([&](const auto& value)
			{
				using value_type = std::decay_t<decltype(value)>;
				if constexpr (std::is_same_v<value_type, std::monostate>)
				{
					plain::fail("XAnimCollisionShape.u", "holds no module");
				}
				else if constexpr (std::is_same_v<value_type, asset_data::bones::x_anim_sphere_collision_shape>)
				{
					native.type = static_cast<XAnimCollisionShapeType>(0);
					plain::module_scope scope(0);
					write_x_anim_sphere_collision_shape(native.u.sphere, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::bones::x_anim_capsule_collision_shape>)
				{
					native.type = static_cast<XAnimCollisionShapeType>(1);
					plain::module_scope scope(1);
					write_x_anim_capsule_collision_shape(native.u.capsule, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::bones::x_anim_box_collision_shape>)
				{
					native.type = static_cast<XAnimCollisionShapeType>(2);
					plain::module_scope scope(2);
					write_x_anim_box_collision_shape(native.u.box, value, resolve, memory);
				}
			}, data.u);
		}

		std::size_t size_x_anim_collision_shape([[maybe_unused]] const asset_data::bones::x_anim_collision_shape& data)
		{
			std::size_t size = 0;
			std::visit([&](const auto& value)
			{
				using value_type = std::decay_t<decltype(value)>;
				if constexpr (std::is_same_v<value_type, std::monostate>)
				{
				}
				else if constexpr (std::is_same_v<value_type, asset_data::bones::x_anim_sphere_collision_shape>)
				{
					size += size_x_anim_sphere_collision_shape(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::bones::x_anim_capsule_collision_shape>)
				{
					size += size_x_anim_capsule_collision_shape(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::bones::x_anim_box_collision_shape>)
				{
					size += size_x_anim_box_collision_shape(value);
				}
			}, data.u);
			return size;
		}

		asset_data::bones::x_anim_custom_pin_constraint read_x_anim_custom_pin_constraint(const XAnimCustomPinConstraint& native)
		{
			plain::trace("read_x_anim_custom_pin_constraint");
			asset_data::bones::x_anim_custom_pin_constraint data;
			data.dynamic_bone_index_a = native.dynamicBoneIndexA;
			data.dynamic_bone_index_b = native.dynamicBoneIndexB;
			data.bone_name_index_a = native.boneNameIndexA;
			data.local_translation_a = plain::read_scalars<float, 3>(&native.localTranslationA);
			data.local_translation_b = plain::read_scalars<float, 3>(&native.localTranslationB);
			data.distance = native.distance;
			data.stiffness = native.stiffness;
			return data;
		}

		void write_x_anim_custom_pin_constraint(XAnimCustomPinConstraint& native, const asset_data::bones::x_anim_custom_pin_constraint& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.dynamicBoneIndexA = data.dynamic_bone_index_a;
			native.dynamicBoneIndexB = data.dynamic_bone_index_b;
			native.boneNameIndexA = data.bone_name_index_a;
			plain::write_scalars(&native.localTranslationA, data.local_translation_a);
			plain::write_scalars(&native.localTranslationB, data.local_translation_b);
			native.distance = data.distance;
			native.stiffness = data.stiffness;
		}

		std::size_t size_x_anim_custom_pin_constraint([[maybe_unused]] const asset_data::bones::x_anim_custom_pin_constraint& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::bones::proceduralbones read_proceduralbones(const XAnimProceduralBones& native)
		{
			plain::trace("read_proceduralbones");
			asset_data::bones::proceduralbones data;
			data.name = plain::read_string(native.name);
			data.constraints = plain::read_array(native.constraints, native.numConstraints, "XAnimProceduralBones.constraints", [&](const auto& item) { return read_x_anim_constraint(item); });
			data.target_bone_names = plain::read_array(native.targetBoneNames, native.numTargetBones, "XAnimProceduralBones.targetBoneNames", [&](const auto& item) { return plain::read_script_string(item); });
			return data;
		}

		void write_proceduralbones(XAnimProceduralBones& native, const asset_data::bones::proceduralbones& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.constraints = plain::write_array<XAnimConstraint>(data.constraints, memory, [&](auto& out, const auto& in) { write_x_anim_constraint(out, in, resolve, memory); });
			native.targetBoneNames = plain::write_array<scr_string_t>(data.target_bone_names, memory, [&](auto& out, const auto& in) { out = static_cast<scr_string_t>(plain::write_script_string(in)); });
			native.numConstraints = plain::count_of<std::uint32_t>(data.constraints);
			native.numTargetBones = plain::count_of<std::uint32_t>(data.target_bone_names);
		}

		std::size_t size_proceduralbones([[maybe_unused]] const asset_data::bones::proceduralbones& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.constraints)
			{
				size += plain::sized(std::max<std::size_t>(data.constraints->size(), 1) * sizeof(XAnimConstraint));
				for (const auto& item : *data.constraints)
				{
					size += size_x_anim_constraint(item);
				}
			}
			if (data.target_bone_names)
			{
				size += plain::sized(std::max<std::size_t>(data.target_bone_names->size(), 1) * sizeof(scr_string_t));
			}
			return size;
		}

		asset_data::bones::x_anim_position_constraint read_x_anim_position_constraint(const XAnimPositionConstraint& native)
		{
			plain::trace("read_x_anim_position_constraint");
			asset_data::bones::x_anim_position_constraint data;
			data.blend_fraction = native.blendFraction;
			data.min_local_translation_limit = plain::read_scalars<float, 3>(&native.minLocalTranslationLimit);
			data.max_local_translation_limit = plain::read_scalars<float, 3>(&native.maxLocalTranslationLimit);
			data.offset = plain::read_scalars<float, 3>(&native.offset);
			data.source_bone_offsets = [&] { std::array<std::array<float, 3>, 2> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = plain::read_scalars<float, 3>(&native.sourceBoneOffsets[i]); } return out; }();
			data.axis_mask = plain::read_scalars<float, 3>(&native.axisMask);
			return data;
		}

		void write_x_anim_position_constraint(XAnimPositionConstraint& native, const asset_data::bones::x_anim_position_constraint& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.blendFraction = data.blend_fraction;
			plain::write_scalars(&native.minLocalTranslationLimit, data.min_local_translation_limit);
			plain::write_scalars(&native.maxLocalTranslationLimit, data.max_local_translation_limit);
			plain::write_scalars(&native.offset, data.offset);
			for (std::size_t i = 0; i < data.source_bone_offsets.size(); ++i)
			{
				plain::write_scalars(&native.sourceBoneOffsets[i], data.source_bone_offsets[i]);
			}
			plain::write_scalars(&native.axisMask, data.axis_mask);
		}

		std::size_t size_x_anim_position_constraint([[maybe_unused]] const asset_data::bones::x_anim_position_constraint& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::bones::x_anim_rotation_constraint read_x_anim_rotation_constraint(const XAnimRotationConstraint& native)
		{
			plain::trace("read_x_anim_rotation_constraint");
			asset_data::bones::x_anim_rotation_constraint data;
			data.rotation_offset_quat = plain::read_scalars<float, 4>(&native.rotationOffsetQuat);
			data.axis_mask = plain::read_scalars<float, 3>(&native.axisMask);
			data.blend_fraction = native.blendFraction;
			return data;
		}

		void write_x_anim_rotation_constraint(XAnimRotationConstraint& native, const asset_data::bones::x_anim_rotation_constraint& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.rotationOffsetQuat, data.rotation_offset_quat);
			plain::write_scalars(&native.axisMask, data.axis_mask);
			native.blendFraction = data.blend_fraction;
		}

		std::size_t size_x_anim_rotation_constraint([[maybe_unused]] const asset_data::bones::x_anim_rotation_constraint& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::bones::x_anim_aim_constraint read_x_anim_aim_constraint(const XAnimAimConstraint& native)
		{
			plain::trace("read_x_anim_aim_constraint");
			asset_data::bones::x_anim_aim_constraint data;
			data.aim_target_local_offset = plain::read_scalars<float, 3>(&native.aimTargetLocalOffset);
			data.local_aim_vector = plain::read_scalars<float, 3>(&native.localAimVector);
			data.local_up_vector = plain::read_scalars<float, 3>(&native.localUpVector);
			data.axis_mask = plain::read_scalars<float, 3>(&native.axisMask);
			data.up_vector = plain::read_scalars<float, 3>(&native.upVector);
			data.up_vector_type = plain::enum_name(asset_data::bones::x_anim_up_vector_type_names, static_cast<std::int64_t>(native.upVectorType), "XAnimAimConstraint.upVectorType");
			return data;
		}

		void write_x_anim_aim_constraint(XAnimAimConstraint& native, const asset_data::bones::x_anim_aim_constraint& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.aimTargetLocalOffset, data.aim_target_local_offset);
			plain::write_scalars(&native.localAimVector, data.local_aim_vector);
			plain::write_scalars(&native.localUpVector, data.local_up_vector);
			plain::write_scalars(&native.axisMask, data.axis_mask);
			plain::write_scalars(&native.upVector, data.up_vector);
			native.upVectorType = static_cast<XAnimUpVectorType>(plain::enum_value(asset_data::bones::x_anim_up_vector_type_names, data.up_vector_type, "XAnimAimConstraint.upVectorType"));
		}

		std::size_t size_x_anim_aim_constraint([[maybe_unused]] const asset_data::bones::x_anim_aim_constraint& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::bones::distance_driven_translation_offset_constraint read_distance_driven_translation_offset_constraint(const XAnimDistanceDrivenOffsetConstraint& native)
		{
			plain::trace("read_distance_driven_translation_offset_constraint");
			asset_data::bones::distance_driven_translation_offset_constraint data;
			data.min_local_offset = plain::read_scalars<float, 4>(&native.minLocalOffset);
			data.max_local_offset = plain::read_scalars<float, 4>(&native.maxLocalOffset);
			data.source_bone_offsets = [&] { std::array<std::array<float, 3>, 2> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = plain::read_scalars<float, 3>(&native.sourceBoneOffsets[i]); } return out; }();
			data.distance_scale = native.distanceScale;
			data.distance_bias = native.distanceBias;
			data.curve_type = plain::enum_name(asset_data::bones::x_anim_curve_type_names, static_cast<std::int64_t>(native.curveType), "distanceDrivenTranslationOffsetConstraint.curveType");
			return data;
		}

		void write_distance_driven_translation_offset_constraint(XAnimDistanceDrivenOffsetConstraint& native, const asset_data::bones::distance_driven_translation_offset_constraint& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.minLocalOffset, data.min_local_offset);
			plain::write_scalars(&native.maxLocalOffset, data.max_local_offset);
			for (std::size_t i = 0; i < data.source_bone_offsets.size(); ++i)
			{
				plain::write_scalars(&native.sourceBoneOffsets[i], data.source_bone_offsets[i]);
			}
			native.distanceScale = data.distance_scale;
			native.distanceBias = data.distance_bias;
			native.curveType = static_cast<XAnimCurveType>(plain::enum_value(asset_data::bones::x_anim_curve_type_names, data.curve_type, "distanceDrivenTranslationOffsetConstraint.curveType"));
		}

		std::size_t size_distance_driven_translation_offset_constraint([[maybe_unused]] const asset_data::bones::distance_driven_translation_offset_constraint& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::bones::distance_driven_rotation_offset_constraint read_distance_driven_rotation_offset_constraint(const XAnimDistanceDrivenOffsetConstraint& native)
		{
			plain::trace("read_distance_driven_rotation_offset_constraint");
			asset_data::bones::distance_driven_rotation_offset_constraint data;
			data.min_local_offset = plain::read_scalars<float, 4>(&native.minLocalOffset);
			data.max_local_offset = plain::read_scalars<float, 4>(&native.maxLocalOffset);
			data.source_bone_offsets = [&] { std::array<std::array<float, 3>, 2> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = plain::read_scalars<float, 3>(&native.sourceBoneOffsets[i]); } return out; }();
			data.distance_scale = native.distanceScale;
			data.distance_bias = native.distanceBias;
			data.curve_type = plain::enum_name(asset_data::bones::x_anim_curve_type_names, static_cast<std::int64_t>(native.curveType), "distanceDrivenRotationOffsetConstraint.curveType");
			return data;
		}

		void write_distance_driven_rotation_offset_constraint(XAnimDistanceDrivenOffsetConstraint& native, const asset_data::bones::distance_driven_rotation_offset_constraint& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.minLocalOffset, data.min_local_offset);
			plain::write_scalars(&native.maxLocalOffset, data.max_local_offset);
			for (std::size_t i = 0; i < data.source_bone_offsets.size(); ++i)
			{
				plain::write_scalars(&native.sourceBoneOffsets[i], data.source_bone_offsets[i]);
			}
			native.distanceScale = data.distance_scale;
			native.distanceBias = data.distance_bias;
			native.curveType = static_cast<XAnimCurveType>(plain::enum_value(asset_data::bones::x_anim_curve_type_names, data.curve_type, "distanceDrivenRotationOffsetConstraint.curveType"));
		}

		std::size_t size_distance_driven_rotation_offset_constraint([[maybe_unused]] const asset_data::bones::distance_driven_rotation_offset_constraint& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::bones::x_anim_constraint read_x_anim_constraint(const XAnimConstraint& native)
		{
			plain::trace("read_x_anim_constraint");
			asset_data::bones::x_anim_constraint data;
			data.local_target_bone_index = native.localTargetBoneIndex;
			data.source_bone_names = [&] { std::array<std::optional<std::string>, 2> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = plain::read_script_string(native.sourceBoneNames[i]); } return out; }();
			data.u = [&]() -> std::variant<std::monostate, asset_data::bones::x_anim_position_constraint, asset_data::bones::x_anim_rotation_constraint, asset_data::bones::x_anim_aim_constraint, asset_data::bones::distance_driven_translation_offset_constraint, asset_data::bones::distance_driven_rotation_offset_constraint> { switch (static_cast<std::uint32_t>(native.type)) { case 0: { plain::module_scope scope(0); auto item = read_x_anim_position_constraint(native.u.positionConstraint); return std::variant<std::monostate, asset_data::bones::x_anim_position_constraint, asset_data::bones::x_anim_rotation_constraint, asset_data::bones::x_anim_aim_constraint, asset_data::bones::distance_driven_translation_offset_constraint, asset_data::bones::distance_driven_rotation_offset_constraint>(std::move(item)); } case 1: { plain::module_scope scope(1); auto item = read_x_anim_rotation_constraint(native.u.rotationConstraint); return std::variant<std::monostate, asset_data::bones::x_anim_position_constraint, asset_data::bones::x_anim_rotation_constraint, asset_data::bones::x_anim_aim_constraint, asset_data::bones::distance_driven_translation_offset_constraint, asset_data::bones::distance_driven_rotation_offset_constraint>(std::move(item)); } case 2: { plain::module_scope scope(2); auto item = read_x_anim_aim_constraint(native.u.aimConstraint); return std::variant<std::monostate, asset_data::bones::x_anim_position_constraint, asset_data::bones::x_anim_rotation_constraint, asset_data::bones::x_anim_aim_constraint, asset_data::bones::distance_driven_translation_offset_constraint, asset_data::bones::distance_driven_rotation_offset_constraint>(std::move(item)); } case 3: { plain::module_scope scope(3); auto item = read_distance_driven_translation_offset_constraint(native.u.distanceDrivenOffsetConstraint); return std::variant<std::monostate, asset_data::bones::x_anim_position_constraint, asset_data::bones::x_anim_rotation_constraint, asset_data::bones::x_anim_aim_constraint, asset_data::bones::distance_driven_translation_offset_constraint, asset_data::bones::distance_driven_rotation_offset_constraint>(std::move(item)); } case 4: { plain::module_scope scope(4); auto item = read_distance_driven_rotation_offset_constraint(native.u.distanceDrivenOffsetConstraint); return std::variant<std::monostate, asset_data::bones::x_anim_position_constraint, asset_data::bones::x_anim_rotation_constraint, asset_data::bones::x_anim_aim_constraint, asset_data::bones::distance_driven_translation_offset_constraint, asset_data::bones::distance_driven_rotation_offset_constraint>(std::move(item)); } default: plain::fail("XAnimConstraint.u", "has a tag value without a case"); } }();
			return data;
		}

		void write_x_anim_constraint(XAnimConstraint& native, const asset_data::bones::x_anim_constraint& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.localTargetBoneIndex = data.local_target_bone_index;
			for (std::size_t i = 0; i < data.source_bone_names.size(); ++i)
			{
				native.sourceBoneNames[i] = static_cast<scr_string_t>(plain::write_script_string(data.source_bone_names[i]));
			}
			std::visit([&](const auto& value)
			{
				using value_type = std::decay_t<decltype(value)>;
				if constexpr (std::is_same_v<value_type, std::monostate>)
				{
					plain::fail("XAnimConstraint.u", "holds no module");
				}
				else if constexpr (std::is_same_v<value_type, asset_data::bones::x_anim_position_constraint>)
				{
					native.type = static_cast<XAnimConstraintType>(0);
					plain::module_scope scope(0);
					write_x_anim_position_constraint(native.u.positionConstraint, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::bones::x_anim_rotation_constraint>)
				{
					native.type = static_cast<XAnimConstraintType>(1);
					plain::module_scope scope(1);
					write_x_anim_rotation_constraint(native.u.rotationConstraint, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::bones::x_anim_aim_constraint>)
				{
					native.type = static_cast<XAnimConstraintType>(2);
					plain::module_scope scope(2);
					write_x_anim_aim_constraint(native.u.aimConstraint, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::bones::distance_driven_translation_offset_constraint>)
				{
					native.type = static_cast<XAnimConstraintType>(3);
					plain::module_scope scope(3);
					write_distance_driven_translation_offset_constraint(native.u.distanceDrivenOffsetConstraint, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::bones::distance_driven_rotation_offset_constraint>)
				{
					native.type = static_cast<XAnimConstraintType>(4);
					plain::module_scope scope(4);
					write_distance_driven_rotation_offset_constraint(native.u.distanceDrivenOffsetConstraint, value, resolve, memory);
				}
			}, data.u);
		}

		std::size_t size_x_anim_constraint([[maybe_unused]] const asset_data::bones::x_anim_constraint& data)
		{
			std::size_t size = 0;
			std::visit([&](const auto& value)
			{
				using value_type = std::decay_t<decltype(value)>;
				if constexpr (std::is_same_v<value_type, std::monostate>)
				{
				}
				else if constexpr (std::is_same_v<value_type, asset_data::bones::x_anim_position_constraint>)
				{
					size += size_x_anim_position_constraint(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::bones::x_anim_rotation_constraint>)
				{
					size += size_x_anim_rotation_constraint(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::bones::x_anim_aim_constraint>)
				{
					size += size_x_anim_aim_constraint(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::bones::distance_driven_translation_offset_constraint>)
				{
					size += size_distance_driven_translation_offset_constraint(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::bones::distance_driven_rotation_offset_constraint>)
				{
					size += size_distance_driven_rotation_offset_constraint(value);
				}
			}, data.u);
			return size;
		}

	}

	std::expected<asset_data::bones::dynamicbones, std::string> to_neutral(const XAnimDynamicBones& native)
	{
		try
		{
			auto data = read_dynamicbones(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	XAnimDynamicBones* from_neutral(const asset_data::bones::dynamicbones& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<XAnimDynamicBones>();
		write_dynamicbones(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::bones::dynamicbones& data)
	{
		return plain::rounded(sizeof(XAnimDynamicBones)) + size_dynamicbones(data) + 64;
	}

	std::expected<asset_data::bones::proceduralbones, std::string> to_neutral(const XAnimProceduralBones& native)
	{
		try
		{
			auto data = read_proceduralbones(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	XAnimProceduralBones* from_neutral(const asset_data::bones::proceduralbones& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<XAnimProceduralBones>();
		write_proceduralbones(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::bones::proceduralbones& data)
	{
		return plain::rounded(sizeof(XAnimProceduralBones)) + size_proceduralbones(data) + 64;
	}

}
