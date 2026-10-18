#pragma once

#include "objects.hpp"

namespace zonetool::iw8::havok
{
	struct mesh_triangle
	{
		// Seen from the front, (b - a) x (c - a) points out.
		std::array<std::uint32_t, 3> vertices{};
		std::uint16_t shape_tag{};
	};

	struct compressed_mesh
	{
		std::vector<vector3> vertices;
		std::vector<mesh_triangle> triangles;
		// Convex shapes that the mesh tree holds as primitives (the Replay tools put brush hulls there).
		std::vector<compound_instance> convex_shapes;
		float convex_radius{};
		std::uint64_t user_data{};
		// A vertex that only one section uses keeps 11, 11 and 10 bits in the bounds of the section when that moves it by no
		// more than this. Other vertices keep 21, 21 and 22 bits in the bounds of the mesh.
		float packed_tolerance = 1.0f / 512.0f;
	};

	struct compressed_mesh_stats
	{
		std::size_t sections{};
		std::size_t packed_vertices{};
		std::size_t shared_vertices{};
		// The packing tolerance that the shared vertices needed, and the largest distance of a vertex from its input.
		float tolerance{};
		float largest_error{};
		// The triangles that the vertex grids collapsed, which the mesh leaves out.
		std::size_t collapsed{};
	};

	// An hknpCompressedMeshShape of the triangles and convex shapes, with its static mesh tree and SIMD tree. Returns the
	// shape item.
	std::uint32_t add_compressed_mesh_shape(object_builder& builder, const compressed_mesh& mesh, compressed_mesh_stats* stats = nullptr);

	// The geometry that an hknpCompressedMeshShape holds, with the decoded vertex positions.
	struct decoded_mesh
	{
		// Three vertices per triangle; a quad gives two triangles.
		std::vector<std::array<vector3, 3>> triangles;
		std::vector<std::uint16_t> triangle_tags;
		std::vector<std::uint32_t> triangle_keys;
		// The convex shape instances, with the item of each child shape.
		std::vector<compound_instance> convex_shapes;
		std::vector<std::uint32_t> convex_items;
		std::vector<std::uint32_t> convex_keys;
		float convex_radius{};
		std::uint64_t user_data{};
		// Primitives that the Replay tools removed (0xDE, 0xAD, 0xDE, 0xAD), and custom primitives that are not convex shapes.
		std::size_t removed{};
		std::size_t custom{};
	};

	// Decodes the mesh and checks its trees: each node must hold the bounds of the primitives below it, within one step of the
	// vertex grids (the Replay tools build the trees before they quantize the vertices) or, with exact, within float rounding.
	decoded_mesh read_compressed_mesh_shape(const tagfile& file, const type_table& types, std::uint32_t item, bool exact = false);
}
