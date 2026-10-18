#pragma once

namespace zonetool::iw8::havok
{
	using vector3 = std::array<float, 3>;

	// A convex polytope: hull vertices, and the faces as vertex rings, counter-clockwise seen from outside.
	struct polytope
	{
		std::vector<vector3> vertices;
		std::vector<std::vector<std::uint32_t>> faces;
		// Unit outward normal and distance: n.x + d = 0 on the face, n.x + d <= 0 inside.
		std::vector<std::array<float, 4>> planes;
	};

	// The convex hull of the points. Points closer than weld_distance count as one; triangles in one plane merge into one
	// face. Throws for fewer than four points that do not span a volume.
	polytope convex_hull(std::span<const vector3> points, float weld_distance);

	// Connectivity of a polytope: for each face edge (from faces[f][e] to the next vertex) the face and edge on the other
	// side, and for each vertex one face edge that starts at it.
	struct polytope_connectivity
	{
		std::vector<std::pair<std::uint32_t, std::uint32_t>> face_links;
		std::vector<std::pair<std::uint32_t, std::uint32_t>> vertex_edges;
	};

	polytope_connectivity connect(const polytope& shape);

	// Half of the smallest angle between the normal of each face and the normal of a face across one of its edges.
	std::vector<double> min_half_angles(const polytope& shape, const polytope_connectivity& connectivity);

	// The radius of the largest sphere inside the polytope.
	double inner_radius(const polytope& shape);
}
