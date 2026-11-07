#include <std_include.hpp>
#include "physics.hpp"
#include "detail.hpp"

#include <bit>

namespace zonetool::iw8::havok
{
	using namespace detail;

	namespace
	{
		std::uint32_t type_of(const object_builder& builder, const std::string_view identity)
		{
			const auto index = builder.types().find(identity);
			if (!index)
			{
				throw std::runtime_error(std::format("the type compendium has no type {}", identity));
			}

			return *index;
		}

		std::uint32_t add_string(object_builder& builder, const std::string_view text)
		{
			std::vector<std::uint8_t> bytes(text.begin(), text.end());
			bytes.push_back(0);
			return builder.array("char", static_cast<std::uint32_t>(bytes.size()), bytes);
		}

		// An array of hkStringPtr that point at the strings.
		std::uint32_t add_strings(object_builder& builder, const std::vector<std::uint32_t>& strings)
		{
			const auto list = builder.array("hkStringPtr", static_cast<std::uint32_t>(strings.size()), std::vector<std::uint8_t>(strings.size() * 8));
			const auto pointer_type = type_of(builder, "hkStringPtr");
			for (std::uint32_t i = 0; i < strings.size(); i++)
			{
				builder.pointer(list, i * 8, pointer_type, strings[i]);
			}

			return list;
		}

		template <typename T>
		std::uint32_t add_values(object_builder& builder, const std::string_view type, const std::vector<T>& values)
		{
			std::vector<std::uint8_t> bytes;
			for (const auto& value : values)
			{
				put(bytes, value);
			}

			return builder.array(type, static_cast<std::uint32_t>(values.size()), bytes);
		}

		// hkHalf16 values of the defaults.
		constexpr std::uint16_t half_0_5 = 0x3F00;
		constexpr std::uint16_t half_1 = 0x3F80;
		constexpr std::uint16_t half_5 = 0x40A0;

		// The defaults of hknpMaterial as Replay physics assets keep them.
		void set_default_material(object_builder& builder, const std::uint32_t material)
		{
			builder.set(material, "dynamicFriction", half_0_5);
			builder.set(material, "staticFriction", half_0_5);
			builder.set(material, "frictionCombinePolicy", std::uint8_t{1});
			builder.set(material, "restitutionCombinePolicy", std::uint8_t{2});
			builder.set(material, "weldingTolerance", 0.05f);
			builder.set(material, "maxContactImpulse", real_max);
			builder.set(material, "fractionOfClippedImpulseToApply", 1.0f);
			builder.set(material, "massChangerHeavyObjectFactor", half_1);
			builder.set(material, "disablingCollisionsBetweenCvxCvxDynamicObjectsDistance", half_5);
		}

		// The defaults of hknpMotionProperties as Replay physics assets keep them.
		void set_default_motion_properties(object_builder& builder, const std::uint32_t motion)
		{
			builder.set(motion, "gravityFactor", 1.0f);
			builder.set(motion, "timeFactor", 1.0f);
			builder.set(motion, "maxLinearSpeed", 200.0f);
			builder.set(motion, "maxAngularSpeed", 100.0f);
			// 0.05 squared, and 0.025 one float step down, as the assets keep them.
			builder.set(motion, "deactivationSettings.maxDistSqrd", std::bit_cast<float>(0x3B23D70Bu));
			builder.set(motion, "deactivationSettings.maxRotSqrd", std::bit_cast<float>(0x3B23D70Bu));
			builder.set(motion, "deactivationSettings.invBlockSize", 1.0f);
			builder.set(motion, "deactivationSettings.pathingUpperThreshold", std::int16_t{0x67FF});
			builder.set(motion, "deactivationSettings.pathingLowerThreshold", static_cast<std::int16_t>(-0x6665));
			builder.set(motion, "deactivationSettings.numDeactivationFrequencyPasses", std::uint8_t{4});
			builder.set(motion, "deactivationSettings.deactivationVelocityScaleSquare", std::uint8_t{0x73});
			builder.set(motion, "deactivationSettings.minimumPathingVelocityScaleSquare", std::uint8_t{0x75});
			builder.set(motion, "deactivationSettings.spikingVelocityScaleThresholdSquared", std::uint8_t{6});
			builder.set(motion, "deactivationSettings.minimumSpikingVelocityScaleSquared", std::uint8_t{0x73});
			builder.set(motion, "fullCastSettings.minSeparation", -0.5f);
			builder.set(motion, "fullCastSettings.minExtraSeparation", -0.125f);
			builder.set(motion, "fullCastSettings.toiSeparation", -0.25f);
			builder.set(motion, "fullCastSettings.toiExtraSeparation", -0.0875f);
			builder.set(motion, "fullCastSettings.toiAccuracy", 0.01f);
			builder.set(motion, "fullCastSettings.relativeSafeDeltaTime", std::bit_cast<float>(0x3CCCCCCCu));
			builder.set(motion, "fullCastSettings.absoluteSafeDeltaTime", 2.5e-5f);
			builder.set(motion, "fullCastSettings.keepTime", 0.05f);
			builder.set(motion, "fullCastSettings.keepDistance", 0.05f);
			builder.set(motion, "fullCastSettings.maxIterations", std::int32_t{5});
		}
	}

	std::uint32_t add_shape_list(object_builder& builder, const std::span<const shape_list_entry> entries, const std::span<const shape_tag> tags)
	{
		const auto list = builder.object("HavokPhysicsShapeList");
		if (!entries.empty())
		{
			const auto count = static_cast<std::uint32_t>(entries.size());
			const auto shapes = builder.array("T*<hknpShape>", count, std::vector<std::uint8_t>(count * 8));
			const auto pointer_type = type_of(builder, "T*<hknpShape>");
			std::vector<std::uint32_t> names;
			std::vector<std::int32_t> indices;
			std::vector<std::uint32_t> vertices;
			std::vector<std::uint32_t> triangles;
			std::vector<std::array<float, 4>> bounds;
			std::vector<std::uint32_t> contents;
			std::vector<std::uint32_t> convex_counts;
			for (std::uint32_t i = 0; i < count; i++)
			{
				const auto& entry = entries[i];
				builder.pointer(shapes, i * 8, pointer_type, entry.shape);
				names.push_back(add_string(builder, entry.name));
				indices.push_back(entry.index);
				vertices.push_back(entry.vertex_count);
				triangles.push_back(entry.triangle_count);
				bounds.push_back({entry.minimum[0], entry.minimum[1], entry.minimum[2], 0.0f});
				bounds.push_back({entry.maximum[0], entry.maximum[1], entry.maximum[2], 0.0f});
				contents.push_back(entry.contents);
				convex_counts.push_back(entry.convex_count);
			}

			builder.point(list, "shapes", shapes);
			builder.point(list, "shapeIndices", add_values(builder, "int", indices));
			builder.point(list, "shapeNames", add_strings(builder, names));
			builder.point(list, "vertCounts", add_values(builder, "int", vertices));
			builder.point(list, "triCounts", add_values(builder, "int", triangles));
			builder.point(list, "minMaxes", add_values(builder, "hkVector4", bounds));
			builder.point(list, "shapeContents", add_values(builder, "int", contents));
			builder.point(list, "convexCounts", add_values(builder, "int", convex_counts));
		}

		if (!tags.empty())
		{
			std::vector<std::uint8_t> bytes;
			for (const auto& tag : tags)
			{
				put(bytes, tag.collision_filter_info);
				put(bytes, tag.material_crc);
				put(bytes, tag.material_id);
				bytes.insert(bytes.end(), 6, 0);
				put(bytes, tag.user_data);
			}

			builder.point(list, "shapeTagData", builder.array("HavokPhysicsShapeList::ShapeTagData", static_cast<std::uint32_t>(tags.size()), bytes));
		}

		return list;
	}

	std::uint32_t add_physics_asset(object_builder& builder, const physics_asset& asset)
	{
		const auto root = builder.object("HavokPhysicsAsset");
		builder.set(root, "useCategory", asset.use_category);

		const auto system = builder.object("hknpPhysicsSystemData");
		builder.point(root, "physicsSystemData", system);
		builder.point(root, "bodyQualityNameCRCLookup", add_values(builder, "int", std::vector{asset.body_quality_crc}));
		builder.point(root, "materialNameCRCLookup", add_values(builder, "int", std::vector{asset.material_crc}));
		builder.point(root, "motionPropertiesNameCRCLookup", add_values(builder, "int", std::vector{asset.motion_properties_crc}));

		// One empty event name per body, for sounds and effects.
		const auto empty = add_string(builder, "");
		builder.point(root, "bodySFXAssetNames", add_strings(builder, {empty}));
		builder.point(root, "bodyVFXAssetNames", add_strings(builder, {empty}));
		builder.point(root, "bodyServerUsage", add_values(builder, "int", std::vector{asset.server_usage}));
		builder.point(root, "bodyDrivers", add_values(builder, "int", std::vector{asset.body_driver}));
		builder.point(root, "simulationCategories", add_values(builder, "int", std::vector{asset.simulation_category}));

		if (asset.system_properties)
		{
			const auto material = builder.array("hknpMaterial", 1, std::vector<std::uint8_t>(builder.types().size_of(type_of(builder, "hknpMaterial"))));
			set_default_material(builder, material);
			builder.point(system, "materials", material);

			const auto motion = builder.array("hknpMotionProperties", 1,
				std::vector<std::uint8_t>(builder.types().size_of(type_of(builder, "hknpMotionProperties"))));
			set_default_motion_properties(builder, motion);
			builder.point(system, "motionProperties", motion);
		}

		builder.set(system, "microStepMultiplier", std::uint8_t{1});

		const auto& source = asset.body;
		const auto body_type = "hknpPhysicsSystemData::bodyCinfoWithAttachment";
		const auto body = builder.array(body_type, 1, std::vector<std::uint8_t>(builder.types().size_of(type_of(builder, body_type))));
		builder.point(system, "bodyCinfos", body);
		builder.point(body, "shape", source.shape);
		builder.point(body, "name", add_string(builder, source.name));
		builder.set(body, "collisionFilterInfo", source.collision_filter_info);
		builder.set(body, "materialId", source.material_id);
		builder.set(body, "qualityId", source.quality_id);
		builder.set(body, "motionType", source.motion_type);
		builder.set(body, "position", source.position);
		builder.set(body, "orientation", source.orientation);
		builder.set(body, "mass", source.mass);
		builder.set(body, "motionPropertiesId", source.motion_properties_id);
		builder.set(body, "desiredBodyId", std::uint32_t{0x00FFFFFF});
		builder.set(body, "motionId", std::uint32_t{0x7FFFFFFF});
		builder.set(body, "activationPriority", source.activation_priority);
		builder.set(body, "attachedBody", std::int32_t{-1});
		return root;
	}

	std::uint32_t add_physics_system(object_builder& builder, const physics_system_asset& asset)
	{
		if (asset.bodies.empty())
		{
			throw std::runtime_error("a physics system has no bodies");
		}

		const auto root = builder.object("HavokPhysicsAsset");
		builder.set(root, "useCategory", asset.use_category);
		const auto system = builder.object("hknpPhysicsSystemData");
		builder.point(root, "physicsSystemData", system);

		const auto count = static_cast<std::uint32_t>(asset.bodies.size());
		const auto per_body = [&](const auto& value)
		{
			std::vector<std::int32_t> values;
			for (const auto& body : asset.bodies)
			{
				values.push_back(static_cast<std::int32_t>(value(body)));
			}

			return add_values(builder, "int", values);
		};

		builder.point(root, "bodyQualityNameCRCLookup", per_body([](const auto& body) { return body.body_quality_crc; }));
		builder.point(root, "materialNameCRCLookup", per_body([](const auto& body) { return body.material_crc; }));
		builder.point(root, "motionPropertiesNameCRCLookup", per_body([](const auto& body) { return body.motion_properties_crc; }));

		// One empty event name per body, for sounds and effects.
		const std::vector<std::uint32_t> names(count, add_string(builder, ""));
		builder.point(root, "bodySFXAssetNames", add_strings(builder, names));
		builder.point(root, "bodyVFXAssetNames", add_strings(builder, names));
		builder.point(root, "bodyServerUsage", add_values(builder, "int", std::vector<std::int32_t>(count)));
		if (!asset.hinges.empty())
		{
			builder.point(root, "constraintServerUsage", add_values(builder, "int", std::vector<std::int32_t>(asset.hinges.size())));
		}

		builder.point(root, "bodyDrivers", add_values(builder, "int", std::vector<std::int32_t>(count)));
		builder.point(root, "simulationCategories", per_body([](const auto& body) { return body.simulation_category; }));
		builder.point(system, "name", add_string(builder, "Default Physics System Data"));
		builder.set(system, "microStepMultiplier", std::uint8_t{1});

		const auto body_type = "hknpPhysicsSystemData::bodyCinfoWithAttachment";
		const auto bodies = builder.array(body_type, count, std::vector<std::uint8_t>(count * builder.types().size_of(type_of(builder, body_type))));
		builder.point(system, "bodyCinfos", bodies);
		for (std::uint32_t i = 0; i < count; i++)
		{
			const auto& source = asset.bodies[i].body;
			builder.point(bodies, "shape", source.shape, i);
			builder.point(bodies, "name", add_string(builder, source.name), i);
			builder.set(bodies, "collisionFilterInfo", source.collision_filter_info, i);
			builder.set(bodies, "materialId", source.material_id, i);
			builder.set(bodies, "qualityId", source.quality_id, i);
			builder.set(bodies, "motionType", source.motion_type, i);
			builder.set(bodies, "position", source.position, i);
			builder.set(bodies, "orientation", source.orientation, i);
			builder.set(bodies, "mass", source.mass, i);
			builder.set(bodies, "motionPropertiesId", source.motion_properties_id, i);
			builder.set(bodies, "desiredBodyId", std::uint32_t{0x00FFFFFF}, i);
			builder.set(bodies, "motionId", std::uint32_t{0x7FFFFFFF}, i);
			builder.set(bodies, "activationPriority", source.activation_priority, i);
			builder.set(bodies, "attachedBody", std::int32_t{-1}, i);
		}

		if (asset.hinges.empty())
		{
			return root;
		}

		const auto hinge_count = static_cast<std::uint32_t>(asset.hinges.size());
		const auto constraints = builder.array("hknpConstraintCinfo", hinge_count,
			std::vector<std::uint8_t>(hinge_count * builder.types().size_of(type_of(builder, "hknpConstraintCinfo"))));
		builder.point(system, "constraintCinfos", constraints);
		for (std::uint32_t i = 0; i < hinge_count; i++)
		{
			const auto& hinge = asset.hinges[i];
			if (hinge.body_a >= count || hinge.body_b >= count || hinge.body_a == hinge.body_b)
			{
				throw std::runtime_error(std::format("hinge {} joins bodies {} and {} of {}", hinge.name, hinge.body_a, hinge.body_b, count));
			}

			const auto data = builder.object(hinge.limited ? "hkpLimitedHingeConstraintData" : "hkpHingeConstraintData");
			builder.set(data, "atoms.transforms.type", std::uint16_t{2});
			builder.set(data, "atoms.transforms.transformA", hinge.transform_a);
			builder.set(data, "atoms.transforms.transformB", hinge.transform_b);
			builder.set(data, "atoms.setupStabilization.type", std::uint16_t{23});
			builder.set(data, "atoms.setupStabilization.enabled", std::uint8_t{hinge.stabilization});
			builder.set(data, "atoms.setupStabilization.maxLinImpulse", hinge.max_linear_impulse);
			builder.set(data, "atoms.setupStabilization.maxAngImpulse", hinge.max_angular_impulse);
			builder.set(data, "atoms.setupStabilization.maxAngle", hinge.max_angle);
			if (hinge.limited)
			{
				// The angular motor is off and has no motor, as in shipped limited hinges.
				builder.set(data, "atoms.angMotor.type", std::uint16_t{18});
				builder.set(data, "atoms.angFriction.type", std::uint16_t{17});
				builder.set(data, "atoms.angFriction.isEnabled", std::uint8_t{hinge.friction});
				builder.set(data, "atoms.angFriction.firstFrictionAxis", hinge.friction_axis);
				builder.set(data, "atoms.angFriction.numFrictionAxes", hinge.friction_axes);
				builder.set(data, "atoms.angFriction.maxFrictionTorque", hinge.max_friction_torque);
				builder.set(data, "atoms.angLimit.type", std::uint16_t{14});
				builder.set(data, "atoms.angLimit.isEnabled", std::uint8_t{hinge.limit});
				builder.set(data, "atoms.angLimit.limitAxis", hinge.limit_axis);
				builder.set(data, "atoms.angLimit.cosineAxis", hinge.cosine_axis);
				builder.set(data, "atoms.angLimit.minAngle", hinge.min_angle);
				builder.set(data, "atoms.angLimit.maxAngle", hinge.max_limit_angle);
				builder.set(data, "atoms.angLimit.angularLimitsTauFactor", hinge.limit_tau);
				builder.set(data, "atoms.angLimit.angularLimitsDampFactor", hinge.limit_damping);
			}

			// The tau and damping factors of the 2D angular and ball and socket atoms are those of shipped hinges (-1: the
			// solver's own; 1).
			builder.set(data, "atoms.2dAng.type", std::uint16_t{12});
			builder.set(data, "atoms.2dAng.freeRotationAxis", hinge.free_axis);
			builder.set(data, "atoms.2dAng.tauFactor", -1.0f);
			builder.set(data, "atoms.2dAng.dampingFactor", -1.0f);
			builder.set(data, "atoms.ballSocket.type", std::uint16_t{5});
			builder.set(data, "atoms.ballSocket.solvingMethod", hinge.solving_method);
			builder.set(data, "atoms.ballSocket.bodiesToNotify", hinge.bodies_to_notify);
			builder.set(data, "atoms.ballSocket.velocityStabilizationFactor", hinge.velocity_stabilization);
			builder.set(data, "atoms.ballSocket.enableLinearImpulseLimit", std::uint8_t{hinge.linear_impulse_limit});
			builder.set(data, "atoms.ballSocket.breachImpulse", hinge.breach_impulse);
			builder.set(data, "atoms.ballSocket.tauFactor", 1.0f);
			builder.set(data, "atoms.ballSocket.dampingFactor", 1.0f);
			builder.set(data, "atoms.ballSocket.inertiaStabilizationFactor", hinge.inertia_stabilization);

			builder.point(constraints, "constraintData", data, i);
			builder.set(constraints, "bodyA", hinge.body_a, i);
			builder.set(constraints, "bodyB", hinge.body_b, i);
			// The flags of every shipped Replay constraint (bit 2, IS_DISABLED in the hk2018.1 headers). IW7's are 0, in the
			// meaning of Havok 2014; HavokPhysics_AddConstraintInstance (0x14161BE30) adds each one to the world and then
			// makes a second world call on it (vtable +0x1C8, with 1).
			builder.set(constraints, "flags", std::uint16_t{4}, i);
			builder.point(constraints, "name", add_string(builder, hinge.name), i);
			builder.set(constraints, "desiredConstraintId", std::uint32_t{0x7FFFFFFF}, i);
			builder.set(constraints, "constraintGroupId", std::uint32_t{0x7FFFFFFF}, i);
		}

		return root;
	}

	std::uint32_t add_xmodel_lod(object_builder& builder, const xmodel_lod_body& body)
	{
		const auto root = builder.object("HavokPhysicsXModelLOD");

		const auto shapes = builder.array("T*<hknpShape>", 1, std::vector<std::uint8_t>(8));
		builder.pointer(shapes, 0, type_of(builder, "T*<hknpShape>"), body.shape);
		builder.point(root, "shapes", shapes);
		builder.point(root, "bodyNames", add_strings(builder, {add_string(builder, body.name)}));

		std::vector<std::uint8_t> pose;
		for (const auto value : body.bind_pose)
		{
			put(pose, value);
		}

		builder.point(root, "bodyBindPoses", builder.array("hkTransform", 1, pose));

		std::vector<std::uint8_t> tag;
		put(tag, body.surface_flags);
		put(tag, body.collision_filter_info);
		builder.point(root, "shapeTagData", builder.array("HavokPhysicsXModelLOD::XModelCollisionTagData", 1, tag));
		return root;
	}
}
