#include <std_include.hpp>
#include "convex.hpp"

#include <intrin.h>
#include <numeric>

namespace zonetool::iw8::havok
{
	namespace
	{
		using point = std::array<double, 3>;

		point to_point(const vector3& value)
		{
			return {value[0], value[1], value[2]};
		}

		point sub(const point& a, const point& b)
		{
			return {a[0] - b[0], a[1] - b[1], a[2] - b[2]};
		}

		point cross(const point& a, const point& b)
		{
			return {a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]};
		}

		double dot(const point& a, const point& b)
		{
			return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
		}

		double length(const point& a)
		{
			return std::sqrt(dot(a, a));
		}

		using grid_point = std::array<std::int64_t, 3>;

		struct wide
		{
			std::uint64_t low;
			std::int64_t high;
		};

		wide multiply(const std::int64_t a, const std::int64_t b)
		{
			std::int64_t high{};
			const auto low = static_cast<std::uint64_t>(_mul128(a, b, &high));
			return {low, high};
		}

		wide add(const wide& a, const wide& b)
		{
			const auto low = a.low + b.low;
			return {low, a.high + b.high + (low < a.low ? 1 : 0)};
		}

		// The sign of (b - a) . ((c - a) x (d - a)): positive when d is on the counter-clockwise side of a, b, c. Exact for
		// coordinates below 2^26.
		int orient3d(const grid_point& a, const grid_point& b, const grid_point& c, const grid_point& d)
		{
			const auto bx = b[0] - a[0];
			const auto by = b[1] - a[1];
			const auto bz = b[2] - a[2];
			const auto cx = c[0] - a[0];
			const auto cy = c[1] - a[1];
			const auto cz = c[2] - a[2];
			const auto dx = d[0] - a[0];
			const auto dy = d[1] - a[1];
			const auto dz = d[2] - a[2];
			const auto sum = add(add(multiply(bx, cy * dz - cz * dy), multiply(by, cz * dx - cx * dz)), multiply(bz, cx * dy - cy * dx));
			return sum.high < 0 ? -1 : (sum.high > 0 || sum.low) ? 1 : 0;
		}

		std::uint64_t edge_key(const std::uint32_t from, const std::uint32_t to)
		{
			return static_cast<std::uint64_t>(from) << 32 | to;
		}

		// Maximise r with n.c + d + r <= 0 for every plane, by the simplex method. The origin must be inside.
		double largest_inner_sphere(const std::vector<std::array<double, 4>>& planes)
		{
			// Variables: c+ (3), c- (3), r, then one slack per plane. Row i: n.(c+ - c-) + r + s_i = -d_i.
			const auto rows = planes.size();
			const auto columns = 7 + rows + 1;
			std::vector<double> table(rows * columns + columns, 0.0);
			const auto at = [&](const std::size_t row, const std::size_t column) -> double&
			{
				return table[row * columns + column];
			};

			std::vector<std::size_t> basis(rows);
			for (std::size_t i = 0; i < rows; i++)
			{
				for (std::size_t axis = 0; axis < 3; axis++)
				{
					at(i, axis) = planes[i][axis];
					at(i, 3 + axis) = -planes[i][axis];
				}

				at(i, 6) = 1.0;
				at(i, 7 + i) = 1.0;
				at(i, columns - 1) = std::max(0.0, -planes[i][3]);
				basis[i] = 7 + i;
			}

			// Objective row: maximise r -> reduced costs of -r.
			at(rows, 6) = -1.0;
			for (std::size_t iteration = 0; iteration < 1000; iteration++)
			{
				std::size_t entering = columns;
				for (std::size_t column = 0; column + 1 < columns; column++)
				{
					if (at(rows, column) < -1e-12)
					{
						entering = column;
						break;
					}
				}

				if (entering == columns)
				{
					break;
				}

				std::size_t leaving = rows;
				double best = std::numeric_limits<double>::infinity();
				for (std::size_t row = 0; row < rows; row++)
				{
					if (at(row, entering) > 1e-12)
					{
						const auto ratio = at(row, columns - 1) / at(row, entering);
						if (ratio < best - 1e-15 || (ratio <= best + 1e-15 && leaving < rows && basis[row] < basis[leaving]))
						{
							best = ratio;
							leaving = row;
						}
					}
				}

				if (leaving == rows)
				{
					return 0.0;
				}

				const auto pivot = at(leaving, entering);
				for (std::size_t column = 0; column < columns; column++)
				{
					at(leaving, column) /= pivot;
				}

				for (std::size_t row = 0; row <= rows; row++)
				{
					if (row != leaving && at(row, entering) != 0.0)
					{
						const auto factor = at(row, entering);
						for (std::size_t column = 0; column < columns; column++)
						{
							at(row, column) -= factor * at(leaving, column);
						}
					}
				}

				basis[leaving] = entering;
			}

			for (std::size_t row = 0; row < rows; row++)
			{
				if (basis[row] == 6)
				{
					return at(row, columns - 1);
				}
			}

			return 0.0;
		}
	}

	namespace
	{
		// Points in one plane: a two-sided polygon. Face 0 faces along (b - a) x (c - a), face 1 the other way, and faces 2
		// and 3 repeat face 0, as Havok stores flat shapes.
		polytope flat_hull(const std::span<const vector3> input, const std::vector<std::uint32_t>& source, const std::vector<point>& points,
			const std::vector<grid_point>& grid, const std::uint32_t a, const std::uint32_t b, const std::uint32_t c)
		{
			const auto ab = grid_point{grid[b][0] - grid[a][0], grid[b][1] - grid[a][1], grid[b][2] - grid[a][2]};
			const auto ac = grid_point{grid[c][0] - grid[a][0], grid[c][1] - grid[a][1], grid[c][2] - grid[a][2]};
			const std::array<std::int64_t, 3> normal{ab[1] * ac[2] - ab[2] * ac[1], ab[2] * ac[0] - ab[0] * ac[2], ab[0] * ac[1] - ab[1] * ac[0]};

			// Project away the axis where the normal is largest; the 2D orientation then has the sign of that component.
			std::size_t drop = 0;
			for (std::size_t axis = 1; axis < 3; axis++)
			{
				if (std::abs(normal[axis]) > std::abs(normal[drop]))
				{
					drop = axis;
				}
			}

			const auto u = (drop + 1) % 3;
			const auto v = (drop + 2) % 3;
			const auto flip = normal[drop] < 0;
			const auto orient2d = [&](const std::uint32_t o, const std::uint32_t p, const std::uint32_t q)
			{
				const auto value = (grid[p][u] - grid[o][u]) * (grid[q][v] - grid[o][v]) - (grid[p][v] - grid[o][v]) * (grid[q][u] - grid[o][u]);
				return flip ? -value : value;
			};

			std::vector<std::uint32_t> order(points.size());
			std::iota(order.begin(), order.end(), 0);
			std::ranges::sort(order, [&](const std::uint32_t x, const std::uint32_t y)
			{
				return std::pair(grid[x][u], grid[x][v]) < std::pair(grid[y][u], grid[y][v]);
			});

			// Monotone chain, counter-clockwise about the normal; collinear points drop out.
			std::vector<std::uint32_t> ring;
			for (const auto pass : {0, 1})
			{
				const auto start = ring.size();
				for (std::size_t k = 0; k < order.size(); k++)
				{
					const auto p = order[pass == 0 ? k : order.size() - 1 - k];
					while (ring.size() >= start + 2 && orient2d(ring[ring.size() - 2], ring.back(), p) <= 0)
					{
						ring.pop_back();
					}

					ring.push_back(p);
				}

				ring.pop_back();
			}

			if (ring.size() < 3)
			{
				throw std::runtime_error("the convex hull points are on one line");
			}

			polytope result;
			std::vector<std::uint32_t> front;
			for (const auto p : ring)
			{
				front.push_back(static_cast<std::uint32_t>(result.vertices.size()));
				result.vertices.push_back(input[source[p]]);
			}

			point n{static_cast<double>(normal[0]), static_cast<double>(normal[1]), static_cast<double>(normal[2])};
			const auto size = length(n);
			n = {n[0] / size, n[1] / size, n[2] / size};
			const auto distance = -dot(n, points[ring[0]]);
			std::vector<std::uint32_t> back(front.rbegin(), front.rend());
			const std::array<float, 4> plane{static_cast<float>(n[0]), static_cast<float>(n[1]), static_cast<float>(n[2]), static_cast<float>(distance)};
			const std::array<float, 4> reverse{-plane[0], -plane[1], -plane[2], -plane[3]};
			result.faces = {front, back, front, front};
			result.planes = {plane, reverse, plane, plane};
			return result;
		}
	}

	polytope convex_hull(const std::span<const vector3> input, const float weld_distance)
	{
		// Weld points; keep the first of each cluster in input order.
		std::vector<point> points;
		std::vector<std::uint32_t> source;
		for (std::size_t i = 0; i < input.size(); i++)
		{
			const auto p = to_point(input[i]);
			const auto close = std::ranges::any_of(points, [&](const point& q)
			{
				return length(sub(p, q)) <= weld_distance;
			});

			if (!close)
			{
				points.push_back(p);
				source.push_back(static_cast<std::uint32_t>(i));
			}
		}

		if (points.size() < 4)
		{
			throw std::runtime_error("a convex hull needs four points that span a volume");
		}

		// Exact orientation tests on an integer grid around the centre of the points.
		point low{points[0]};
		point high{points[0]};
		for (const auto& p : points)
		{
			for (std::size_t axis = 0; axis < 3; axis++)
			{
				low[axis] = std::min(low[axis], p[axis]);
				high[axis] = std::max(high[axis], p[axis]);
			}
		}

		const point middle{(low[0] + high[0]) / 2, (low[1] + high[1]) / 2, (low[2] + high[2]) / 2};
		const auto extent = std::max({high[0] - low[0], high[1] - low[1], high[2] - low[2], 1e-30});
		const auto grid_scale = static_cast<double>(1ll << 26) / extent;
		std::vector<grid_point> grid;
		for (const auto& p : points)
		{
			grid.push_back({std::llround((p[0] - middle[0]) * grid_scale), std::llround((p[1] - middle[1]) * grid_scale),
				std::llround((p[2] - middle[2]) * grid_scale)});
		}

		const auto orientation = [&](const std::uint32_t x, const std::uint32_t y, const std::uint32_t z, const std::uint32_t w)
		{
			return orient3d(grid[x], grid[y], grid[z], grid[w]);
		};

		// Initial simplex: two distinct points, a third off their line, a fourth off their plane.
		std::uint32_t a = 0;
		std::uint32_t b = 0;
		for (std::uint32_t i = 1; i < points.size(); i++)
		{
			if (points[i][0] < points[a][0])
			{
				a = i;
			}

			if (points[i][0] > points[b][0])
			{
				b = i;
			}
		}

		if (grid[a] == grid[b])
		{
			b = a == 0 ? 1 : 0;
		}

		const auto line = sub(points[b], points[a]);
		std::uint32_t c = a;
		double farthest = 0.0;
		for (std::uint32_t i = 0; i < points.size(); i++)
		{
			const auto distance = length(cross(line, sub(points[i], points[a])));
			if (distance > farthest)
			{
				farthest = distance;
				c = i;
			}
		}

		std::uint32_t d = a;
		farthest = 0.0;
		const auto base = cross(sub(points[b], points[a]), sub(points[c], points[a]));
		for (std::uint32_t i = 0; i < points.size(); i++)
		{
			const auto distance = std::abs(dot(base, sub(points[i], points[a])));
			if (distance > farthest && orientation(a, b, c, i) != 0)
			{
				farthest = distance;
				d = i;
			}
		}

		if (c == a || grid[c] == grid[a] || grid[c] == grid[b])
		{
			throw std::runtime_error("the convex hull points are on one line");
		}

		if (d == a || orientation(a, b, c, d) == 0)
		{
			return flat_hull(input, source, points, grid, a, b, c);
		}

		// Faces wind counter-clockwise seen from outside: orientation(x, y, z, w) is positive when w is outside.
		std::vector<std::array<std::uint32_t, 3>> faces;
		std::vector<bool> alive;
		const auto add_face = [&](const std::uint32_t x, const std::uint32_t y, const std::uint32_t z, const std::uint32_t inside)
		{
			if (orientation(x, y, z, inside) > 0)
			{
				faces.push_back({x, z, y});
			}
			else
			{
				faces.push_back({x, y, z});
			}

			alive.push_back(true);
		};

		add_face(a, b, c, d);
		add_face(a, b, d, c);
		add_face(a, c, d, b);
		add_face(b, c, d, a);

		for (std::uint32_t p = 0; p < points.size(); p++)
		{
			if (p == a || p == b || p == c || p == d)
			{
				continue;
			}

			// A face sees the point when the point is strictly outside its plane.
			std::vector<std::size_t> visible;
			for (std::size_t f = 0; f < faces.size(); f++)
			{
				if (alive[f] && orientation(faces[f][0], faces[f][1], faces[f][2], p) > 0)
				{
					visible.push_back(f);
				}
			}

			if (visible.empty())
			{
				continue;
			}

			std::unordered_set<std::uint64_t> visible_edges;
			for (const auto f : visible)
			{
				for (std::size_t e = 0; e < 3; e++)
				{
					visible_edges.insert(edge_key(faces[f][e], faces[f][(e + 1) % 3]));
				}
			}

			std::vector<std::pair<std::uint32_t, std::uint32_t>> horizon;
			for (const auto f : visible)
			{
				for (std::size_t e = 0; e < 3; e++)
				{
					const auto from = faces[f][e];
					const auto to = faces[f][(e + 1) % 3];
					if (!visible_edges.contains(edge_key(to, from)))
					{
						horizon.emplace_back(from, to);
					}
				}

				alive[f] = false;
			}

			for (const auto& [from, to] : horizon)
			{
				faces.push_back({from, to, p});
				alive.push_back(true);
			}
		}

		std::vector<std::array<std::uint32_t, 3>> triangles;
		for (std::size_t f = 0; f < faces.size(); f++)
		{
			if (alive[f])
			{
				triangles.push_back(faces[f]);
			}
		}

		// Merge neighbouring triangles that lie exactly in one plane of the grid.
		std::unordered_map<std::uint64_t, std::size_t> owner;
		for (std::size_t t = 0; t < triangles.size(); t++)
		{
			for (std::size_t e = 0; e < 3; e++)
			{
				owner[edge_key(triangles[t][e], triangles[t][(e + 1) % 3])] = t;
			}
		}

		std::vector<std::size_t> group(triangles.size());
		std::iota(group.begin(), group.end(), 0);
		const auto find = [&](std::size_t x)
		{
			while (group[x] != x)
			{
				x = group[x] = group[group[x]];
			}

			return x;
		};

		for (std::size_t t = 0; t < triangles.size(); t++)
		{
			for (std::size_t e = 0; e < 3; e++)
			{
				const auto twin = owner.find(edge_key(triangles[t][(e + 1) % 3], triangles[t][e]));
				if (twin == owner.end())
				{
					throw std::runtime_error("a convex hull edge has no neighbour triangle");
				}

				const auto other = twin->second;
				const auto coplanar = std::ranges::all_of(triangles[other], [&](const std::uint32_t v)
				{
					return orientation(triangles[t][0], triangles[t][1], triangles[t][2], v) == 0;
				});

				if (coplanar)
				{
					group[find(t)] = find(other);
				}
			}
		}

		// Face rings: the boundary edges of each group, chained.
		std::map<std::size_t, std::vector<std::size_t>> groups;
		for (std::size_t t = 0; t < triangles.size(); t++)
		{
			groups[find(t)].push_back(t);
		}

		std::vector<std::vector<std::uint32_t>> rings;
		for (const auto& [root, members] : groups)
		{
			std::unordered_set<std::uint64_t> edges;
			for (const auto t : members)
			{
				for (std::size_t e = 0; e < 3; e++)
				{
					edges.insert(edge_key(triangles[t][e], triangles[t][(e + 1) % 3]));
				}
			}

			std::unordered_map<std::uint32_t, std::uint32_t> next;
			for (const auto key : edges)
			{
				const auto from = static_cast<std::uint32_t>(key >> 32);
				const auto to = static_cast<std::uint32_t>(key);
				if (!edges.contains(edge_key(to, from)))
				{
					if (next.contains(from))
					{
						throw std::runtime_error("a convex hull face is not a simple polygon");
					}

					next[from] = to;
				}
			}

			auto start = next.begin()->first;
			for (const auto& [from, to] : next)
			{
				start = std::min(start, from);
			}

			std::vector<std::uint32_t> ring;
			auto current = start;
			do
			{
				ring.push_back(current);
				current = next.at(current);
			} while (current != start && ring.size() <= next.size());

			if (ring.size() != next.size())
			{
				throw std::runtime_error("a convex hull face is not a simple polygon");
			}

			rings.push_back(std::move(ring));
		}

		// Drop the points in the middle of an edge: a hull vertex belongs to three or more faces.
		std::vector<std::uint32_t> face_count(points.size());
		for (const auto& ring : rings)
		{
			for (const auto v : ring)
			{
				face_count[v]++;
			}
		}

		for (auto& ring : rings)
		{
			std::erase_if(ring, [&](const std::uint32_t v)
			{
				return face_count[v] < 3;
			});
		}

		std::vector<std::int64_t> number(points.size(), -1);
		std::vector<std::uint32_t> kept;
		polytope result;
		for (std::uint32_t v = 0; v < points.size(); v++)
		{
			if (face_count[v] >= 3)
			{
				number[v] = static_cast<std::int64_t>(result.vertices.size());
				result.vertices.push_back(input[source[v]]);
				kept.push_back(v);
			}
		}

		for (const auto& ring : rings)
		{
			if (ring.size() < 3)
			{
				throw std::runtime_error("a convex hull face has fewer than three corners");
			}

			std::vector<std::uint32_t> face;
			point normal{};
			for (std::size_t i = 0; i < ring.size(); i++)
			{
				face.push_back(static_cast<std::uint32_t>(number[ring[i]]));
				const auto& p = points[ring[i]];
				const auto& q = points[ring[(i + 1) % ring.size()]];
				normal[0] += (p[1] - q[1]) * (p[2] + q[2]);
				normal[1] += (p[2] - q[2]) * (p[0] + q[0]);
				normal[2] += (p[0] - q[0]) * (p[1] + q[1]);
			}

			const auto size = length(normal);
			normal = {normal[0] / size, normal[1] / size, normal[2] / size};

			// The supporting plane: every hull vertex is on its inner side.
			double support = -std::numeric_limits<double>::infinity();
			for (const auto v : kept)
			{
				support = std::max(support, dot(normal, points[v]));
			}

			result.faces.push_back(std::move(face));
			result.planes.push_back({static_cast<float>(normal[0]), static_cast<float>(normal[1]), static_cast<float>(normal[2]),
				static_cast<float>(-support)});
		}

		return result;
	}

	polytope_connectivity connect(const polytope& shape)
	{
		std::unordered_map<std::uint64_t, std::pair<std::uint32_t, std::uint32_t>> edges;
		for (std::uint32_t f = 0; f < shape.faces.size(); f++)
		{
			const auto& ring = shape.faces[f];
			for (std::uint32_t e = 0; e < ring.size(); e++)
			{
				edges.try_emplace(edge_key(ring[e], ring[(e + 1) % ring.size()]), f, e);
			}
		}

		polytope_connectivity result;
		result.vertex_edges.assign(shape.vertices.size(), {0, 0});
		std::vector<bool> has_edge(shape.vertices.size());
		for (std::uint32_t f = 0; f < shape.faces.size(); f++)
		{
			const auto& ring = shape.faces[f];
			for (std::uint32_t e = 0; e < ring.size(); e++)
			{
				const auto twin = edges.find(edge_key(ring[(e + 1) % ring.size()], ring[e]));
				if (twin == edges.end())
				{
					throw std::runtime_error("a convex hull edge has no neighbour face");
				}

				result.face_links.push_back(twin->second);
				if (!has_edge[ring[e]])
				{
					has_edge[ring[e]] = true;
					result.vertex_edges[ring[e]] = {f, e};
				}
			}
		}

		return result;
	}

	std::vector<double> min_half_angles(const polytope& shape, const polytope_connectivity& connectivity)
	{
		std::vector<double> result;
		std::size_t link = 0;
		for (std::size_t f = 0; f < shape.faces.size(); f++)
		{
			const auto& n = shape.planes[f];
			auto smallest = std::numbers::pi;
			for (std::size_t e = 0; e < shape.faces[f].size(); e++, link++)
			{
				const auto& m = shape.planes[connectivity.face_links[link].first];
				const auto cosine = std::clamp(static_cast<double>(n[0]) * m[0] + static_cast<double>(n[1]) * m[1] +
					static_cast<double>(n[2]) * m[2], -1.0, 1.0);
				smallest = std::min(smallest, std::acos(cosine) / 2);
			}

			result.push_back(smallest);
		}

		return result;
	}

	double inner_radius(const polytope& shape)
	{
		point centre{};
		for (const auto& v : shape.vertices)
		{
			for (std::size_t axis = 0; axis < 3; axis++)
			{
				centre[axis] += v[axis];
			}
		}

		for (auto& value : centre)
		{
			value /= static_cast<double>(shape.vertices.size());
		}

		// Planes relative to the vertex centroid, which is inside.
		std::vector<std::array<double, 4>> planes;
		for (const auto& plane : shape.planes)
		{
			const point normal{plane[0], plane[1], plane[2]};
			planes.push_back({normal[0], normal[1], normal[2], plane[3] + dot(normal, centre)});
		}

		return std::max(largest_inner_sphere(planes), static_cast<double>(std::numeric_limits<float>::epsilon()));
	}
}
