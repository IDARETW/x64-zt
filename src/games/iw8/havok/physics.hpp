#pragma once

#include "objects.hpp"

namespace zonetool::iw8::havok
{
	// HavokPhysicsShapeList::ShapeTagData: what a shape tag of the list means to the game.
	struct shape_tag
	{
		std::uint32_t collision_filter_info{};
		std::uint32_t material_crc{};
		std::uint16_t material_id = 0xFFFF;
		std::uint64_t user_data{};
	};

	struct shape_list_entry
	{
		std::uint32_t shape{};
		std::string name;
		std::int32_t index = -1;
		std::uint32_t vertex_count{};
		std::uint32_t triangle_count{};
		std::array<float, 3> minimum{};
		std::array<float, 3> maximum{};
		std::uint32_t contents{};
		std::uint32_t convex_count{};
	};

	// A HavokPhysicsShapeList, the root of the Havok data of col_map and map_ents. Returns the list item.
	std::uint32_t add_shape_list(object_builder& builder, std::span<const shape_list_entry> entries, std::span<const shape_tag> tags);

	// The body of a one-body physics asset (hknpPhysicsSystemData::bodyCinfoWithAttachment).
	struct physics_body
	{
		std::uint32_t shape{};
		std::string name = "tag_origin";
		std::uint32_t collision_filter_info{};
		std::uint16_t material_id{};
		std::uint8_t quality_id{};
		std::uint8_t motion_type{};
		float mass = -1.0f;
		std::uint16_t motion_properties_id{};
		std::int8_t activation_priority{};
	};

	// A HavokPhysicsAsset with one body, as the Replay tools make them for models.
	struct physics_asset
	{
		std::int32_t use_category{};
		std::uint32_t body_quality_crc{};
		std::uint32_t material_crc{};
		std::uint32_t motion_properties_crc{};
		std::int32_t server_usage{};
		std::int32_t body_driver{};
		std::int32_t simulation_category{};
		// One default hknpMaterial and hknpMotionProperties in the system (simulated clutter takes them from the lookups).
		bool system_properties = true;
		physics_body body;
	};

	// Returns the asset item.
	std::uint32_t add_physics_asset(object_builder& builder, const physics_asset& asset);

	// A HavokPhysicsXModelLOD with one body, the Havok data of an XModelDetailCollision: the shape that bullets and melee
	// hit, in the detail world.
	struct xmodel_lod_body
	{
		std::uint32_t shape{};
		std::string name = "tag_origin";
		// The shape tag that the primitives of a compressed mesh name by index.
		std::uint32_t surface_flags{};
		std::uint32_t collision_filter_info{};
		// The three rotation columns and the translation of the bind pose (hkTransform).
		std::array<float, 16> bind_pose{1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
	};

	// Returns the root item.
	std::uint32_t add_xmodel_lod(object_builder& builder, const xmodel_lod_body& body);
}
