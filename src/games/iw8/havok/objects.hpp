#pragma once

#include "convex.hpp"
#include "tagfile.hpp"

namespace zonetool::iw8::havok
{
	// Builds the items of a tagfile from compendium types. The first object is the root.
	class object_builder
	{
	public:
		explicit object_builder(const type_table& compendium);

		const type_table& types() const;

		// A zeroed object of the type.
		std::uint32_t object(std::string_view type);
		// An array of count elements of the type, with their bytes.
		std::uint32_t array(std::string_view element_type, std::uint32_t count, std::span<const std::uint8_t> data);

		// An item as it is: type, count, data. Its pointers come from pointer().
		std::uint32_t raw(std::uint32_t type, std::uint8_t flags, std::uint32_t count, std::vector<std::uint8_t> data);
		void pointer(std::uint32_t item, std::uint32_t offset, std::uint32_t pointer_type, std::uint32_t target);

		// Points a member of an item (pointer, hkRefPtr, hkArray, hkRelArray or hkStringPtr) at another item.
		void point(std::uint32_t item, std::string_view member, std::uint32_t target, std::uint32_t element = 0);
		// Writes the bytes of a member of an item.
		void set(std::uint32_t item, std::string_view member, std::span<const std::uint8_t> bytes, std::uint32_t element = 0);

		template <typename T>
		void set(const std::uint32_t item, const std::string_view member, const T& value, const std::uint32_t element = 0)
		{
			this->set(item, member, std::span(reinterpret_cast<const std::uint8_t*>(&value), sizeof(T)), element);
		}

		// The items as a tagfile with the root object as item 1.
		tagfile finish(std::string_view sdk_version, std::optional<std::uint64_t> compendium, std::uint32_t root = 1);

	private:
		const type_table& types_;
		std::vector<item> items_;

		std::uint32_t type_index(std::string_view identity) const;
		std::pair<std::uint32_t, std::uint32_t> member(std::uint32_t item, std::string_view name) const;
	};

	struct convex_options
	{
		// hknpShape::m_flags of the shape.
		std::uint16_t flags = 0x943;
		std::uint8_t dispatch_type = 2;
		std::uint64_t user_data{};
		float convex_radius{};
		// hknpShape::m_maxAllowedPenetration (hkHalf16); otherwise a third of the inner radius.
		std::optional<std::uint16_t> max_allowed_penetration;
	};

	// An hknpConvexPolytopeShape of the polytope, with its connectivity. Returns the shape item.
	std::uint32_t add_convex_polytope_shape(object_builder& builder, const polytope& shape, const convex_options& options = {});

	// The polytope that an hknpConvexPolytopeShape item holds.
	polytope read_convex_polytope_shape(const tagfile& file, const type_table& types, std::uint32_t item);

	struct compound_instance
	{
		std::uint32_t shape{};
		// The columns of the rotation.
		std::array<vector3, 3> rotation{{{1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}}};
		vector3 translation{};
		std::uint16_t shape_tag{};
		// The estimated shape key count of the child shape (a convex shape has 1).
		std::uint32_t shape_keys = 1;
		// The vertices and convex radius of the child shape in its own frame, for the bounds.
		std::vector<vector3> vertices;
		float convex_radius{};
	};

	// The vertices of the child shape placed by the transform of the instance, as the Replay tools place them.
	std::vector<vector3> placed_vertices(const compound_instance& instance);
	// The placed vertices with the convex radius: minimum x, y, z, then maximum x, y, z.
	std::array<float, 6> instance_bounds(const compound_instance& instance);
	// An hknpShapeInstance without its shape pointer. translation_w is the bit pattern of the w of the translation.
	std::vector<std::uint8_t> instance_data(const compound_instance& instance, std::uint32_t translation_w);

	// An hknpCompoundShape of the instances with a SIMD tree, as the Replay tools make them. Returns the shape item.
	std::uint32_t add_compound_shape(object_builder& builder, std::span<const compound_instance> instances);

	// The mass properties of a shape as hknpShapeMassProperties keeps them: the inertia in the frame of the rotation (a
	// quaternion x, y, z, w).
	struct mass_properties
	{
		float volume{};
		float mass{};
		vector3 centre{};
		vector3 inertia{};
		std::array<float, 4> rotation{0.0f, 0.0f, 0.0f, 1.0f};
	};

	// The mass properties of a convex polytope of uniform density. The Replay tools scale the inertia by 1.5.
	mass_properties convex_mass_properties(const polytope& shape, float mass, float inertia_factor = 1.5f);

	// Gives the shape its mass properties (hkRefCountedProperties with an hknpShapeMassProperties).
	void add_mass_properties(object_builder& builder, std::uint32_t shape, const mass_properties& properties);

	// The mass properties that a shape item keeps, as they decode.
	std::optional<mass_properties> read_mass_properties(const tagfile& file, const type_table& types, std::uint32_t shape);

	// A convex shape of a PhysicsFXShape, at its scale.
	struct fx_shape_level
	{
		float scale{};
		polytope shape;
		mass_properties mass;
		std::optional<std::uint16_t> max_allowed_penetration;
	};

	// The levels of a PhysicsFXShape as the Replay tools make them: the hull of the points at 1, then divided by 1.33 eight
	// times, then multiplied by 1.33 eight times, each with the mass properties of density 1000.
	std::vector<fx_shape_level> fx_shape_levels(std::span<const vector3> points);

	// The Havok data of a PhysicsFXShape: an hkTuple of its convex shapes and their scales. Returns the tuple item.
	std::uint32_t add_fx_shape(object_builder& builder, std::span<const fx_shape_level> levels, const convex_options& options = {});

	// The items of the tagfile in breadth-first order from the root, each item's targets in the order of their offsets (as
	// the Replay tools write self-describing tagfiles).
	tagfile breadth_first(tagfile file);

	// A copy of an item of another tagfile, with the items that it points at.
	std::uint32_t copy_item(object_builder& builder, const tagfile& file, std::uint32_t item,
		std::unordered_map<std::uint32_t, std::uint32_t>& copies);
}
