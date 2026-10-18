#include <std_include.hpp>
#include "mesh.hpp"
#include "detail.hpp"

#include <bit>
#include <numeric>

namespace zonetool::iw8::havok
{
	using namespace detail;

	namespace
	{
		// The static mesh trees keep each child AABB in 4 bits per side, as (q * q) / 226 of the parent extent.
		constexpr auto tree_step = 1.0f / 226.0f;
		// A primitive (a, a, 4, a) is a custom primitive: 4 entries of the section table from a, here a convex shape.
		constexpr std::uint16_t convex_record = 146;
		constexpr std::array<std::uint8_t, 4> removed_primitive{0xDE, 0xAD, 0xDE, 0xAD};
		constexpr std::size_t section_primitives = 127;
		constexpr std::size_t section_entries = 256;
		constexpr std::array<std::uint32_t, 3> packed_bits{11, 11, 10};
		constexpr std::array<std::uint32_t, 3> shared_bits{21, 21, 22};

		std::uint32_t offset_of(const type_table& types, const std::uint32_t type, const std::string_view name)
		{
			const auto found = types.member(type, name);
			if (!found)
			{
				throw std::runtime_error(std::format("{} has no member {}", types.identity(type), name));
			}

			return found->first;
		}

		const item* target_of(const tagfile& file, const item& owner, const std::uint32_t offset)
		{
			const auto found = std::ranges::find(owner.pointers, offset, &pointer_field::offset);
			return found != owner.pointers.end() && found->target ? &file.items.at(found->target) : nullptr;
		}

		// The child bounds that three 4-bit pairs give within the parent bounds, with the float operations of the game.
		bounds decode_child(const bounds& parent, const std::array<std::uint8_t, 3>& xyz)
		{
			bounds child{};
			for (std::size_t axis = 0; axis < 3; axis++)
			{
				const float step = (parent[axis + 3] - parent[axis]) * tree_step;
				const auto low = xyz[axis] >> 4;
				const auto high = xyz[axis] & 15;
				child[axis] = parent[axis] + static_cast<float>(low * low) * step;
				child[axis + 3] = parent[axis + 3] - static_cast<float>(high * high) * step;
			}

			return child;
		}

		// The smallest child bounds within the parent that hold the box.
		std::array<std::uint8_t, 3> encode_child(const bounds& parent, const bounds& box, bounds& decoded)
		{
			std::array<std::uint8_t, 3> xyz{};
			for (std::size_t axis = 0; axis < 3; axis++)
			{
				const float step = (parent[axis + 3] - parent[axis]) * tree_step;
				auto low = 15;
				while (low > 0 && parent[axis] + static_cast<float>(low * low) * step > box[axis])
				{
					low--;
				}

				auto high = 15;
				while (high > 0 && parent[axis + 3] - static_cast<float>(high * high) * step < box[axis + 3])
				{
					high--;
				}

				xyz[axis] = static_cast<std::uint8_t>(low << 4 | high);
				decoded[axis] = parent[axis] + static_cast<float>(low * low) * step;
				decoded[axis + 3] = parent[axis + 3] - static_cast<float>(high * high) * step;
			}

			return xyz;
		}

		// Whether the outer bounds hold the inner ones within the slack and a few float steps of their values (the Replay tools
		// build the trees from the vertices before quantization, and round the convex radius and the bounds of convex shapes
		// differently).
		bool holds(const bounds& outer, const bounds& inner, const float slack = 0.0f)
		{
			for (std::size_t axis = 0; axis < 6; axis++)
			{
				const auto allowed = slack + 1e-5f + 4.0f * std::numeric_limits<float>::epsilon() * std::abs(inner[axis]);
				if (axis < 3 ? inner[axis] < outer[axis] - allowed : inner[axis] > outer[axis] + allowed)
				{
					return false;
				}
			}

			return true;
		}

		bounds point_bounds(const std::span<const vector3> points)
		{
			auto box = empty_bounds;
			for (const auto& p : points)
			{
				include(box, {p[0], p[1], p[2], p[0], p[1], p[2]});
			}

			return box;
		}

		vector3 centre_of(const bounds& box)
		{
			return {(box[0] + box[3]) / 2.0f, (box[1] + box[4]) / 2.0f, (box[2] + box[5]) / 2.0f};
		}

		// A grid of 2^bits - 1 steps per axis: the game decodes low + q * scale.
		struct grid
		{
			std::array<float, 3> low{};
			std::array<float, 3> scale{};
			std::array<std::uint32_t, 3> bits{};

			float decode(const std::size_t axis, const std::uint32_t q) const
			{
				return this->low[axis] + static_cast<float>(q) * this->scale[axis];
			}

			std::uint32_t encode(const std::size_t axis, const float value) const
			{
				const auto steps = (1u << this->bits[axis]) - 1;
				if (!(this->scale[axis] > 0.0f))
				{
					return 0;
				}

				const auto guess = std::clamp<double>(std::round((static_cast<double>(value) - this->low[axis]) / this->scale[axis]), 0.0, steps);
				auto best = static_cast<std::uint32_t>(guess);
				for (const auto q : {best > 0 ? best - 1 : best, best + 1})
				{
					if (q <= steps && std::abs(this->decode(axis, q) - value) < std::abs(this->decode(axis, best) - value))
					{
						best = q;
					}
				}

				return best;
			}
		};

		grid make_grid(const bounds& box, const std::array<std::uint32_t, 3>& bits)
		{
			grid result;
			result.bits = bits;
			for (std::size_t axis = 0; axis < 3; axis++)
			{
				result.low[axis] = box[axis];
				result.scale[axis] = (box[axis + 3] - box[axis]) / static_cast<float>((1u << bits[axis]) - 1);
			}

			return result;
		}

		// A binary tree in depth-first order. Each node splits its items in two at the median of their centres along the axis
		// where the centres spread most, until `fits` takes the items as one leaf.
		struct split_tree
		{
			struct node
			{
				bool leaf{};
				// Leaf: the group. Inner node: the leaves of the left subtree.
				std::uint32_t value{};
			};

			std::vector<node> nodes;
			std::vector<std::vector<std::uint32_t>> groups;
		};

		void split_half(const std::vector<vector3>& centres, std::span<std::uint32_t> items)
		{
			auto spread = empty_bounds;
			for (const auto i : items)
			{
				include(spread, {centres[i][0], centres[i][1], centres[i][2], centres[i][0], centres[i][1], centres[i][2]});
			}

			std::size_t axis{};
			for (std::size_t a = 1; a < 3; a++)
			{
				if (spread[a + 3] - spread[a] > spread[axis + 3] - spread[axis])
				{
					axis = a;
				}
			}

			std::ranges::nth_element(items, items.begin() + items.size() / 2, [&](const std::uint32_t a, const std::uint32_t b)
			{
				return centres[a][axis] != centres[b][axis] ? centres[a][axis] < centres[b][axis] : a < b;
			});
		}

		template <typename Fits>
		split_tree split_items(const std::vector<vector3>& centres, std::vector<std::uint32_t> items, const Fits& fits)
		{
			split_tree tree;
			const std::function<std::uint32_t(std::span<std::uint32_t>)> visit = [&](const std::span<std::uint32_t> range)
			{
				const auto index = tree.nodes.size();
				tree.nodes.emplace_back();
				if (fits(std::span<const std::uint32_t>(range)))
				{
					tree.nodes[index] = {true, static_cast<std::uint32_t>(tree.groups.size())};
					tree.groups.emplace_back(range.begin(), range.end());
					return 1u;
				}

				split_half(centres, range);
				const auto middle = range.size() / 2;
				const auto left = visit(range.first(middle));
				tree.nodes[index].value = left;
				return left + visit(range.subspan(middle));
			};

			visit(items);
			return tree;
		}

		struct encoded_tree
		{
			std::vector<std::array<std::uint8_t, 3>> xyz;
			// The decoded bounds of the leaves, by group.
			std::vector<bounds> leaves;
			std::vector<std::uint32_t> leaf_nodes;
		};

		// The 4-bit bounds of each node of a split tree, the root within `domain`.
		encoded_tree encode_tree(const split_tree& tree, const std::vector<bounds>& group_bounds, const bounds& domain)
		{
			encoded_tree result;
			result.xyz.resize(tree.nodes.size());
			result.leaves.resize(tree.groups.size());
			result.leaf_nodes.resize(tree.groups.size());

			// The true bounds of each subtree.
			std::vector<bounds> subtree(tree.nodes.size(), empty_bounds);
			const std::function<std::uint32_t(std::uint32_t)> measure = [&](const std::uint32_t node)
			{
				if (tree.nodes[node].leaf)
				{
					subtree[node] = group_bounds[tree.nodes[node].value];
					return node + 1;
				}

				const auto right = measure(node + 1);
				const auto end = measure(right);
				subtree[node] = subtree[node + 1];
				include(subtree[node], subtree[right]);
				return end;
			};

			measure(0);

			const std::function<std::uint32_t(std::uint32_t, const bounds&)> encode = [&](const std::uint32_t node, const bounds& parent)
			{
				bounds decoded{};
				result.xyz[node] = encode_child(parent, subtree[node], decoded);
				if (tree.nodes[node].leaf)
				{
					result.leaves[tree.nodes[node].value] = decoded;
					result.leaf_nodes[tree.nodes[node].value] = node;
					return node + 1;
				}

				return encode(encode(node + 1, decoded), decoded);
			};

			encode(0, domain);
			return result;
		}

		// A SIMD tree over boxes: node 0 empty, node 1 the root. An inner node splits its items in two at the median, and halves
		// of more than four items in two again; a child of at most four items is a leaf.
		std::vector<tree_node> build_simd_tree(const std::vector<bounds>& boxes, const std::vector<vector3>& centres,
			const std::vector<std::uint32_t>& values)
		{
			std::vector<tree_node> nodes(2);
			std::vector<std::uint32_t> all(boxes.size());
			std::iota(all.begin(), all.end(), 0u);

			const std::function<void(std::uint32_t, std::span<std::uint32_t>)> fill = [&](const std::uint32_t node, const std::span<std::uint32_t> items)
			{
				std::vector<std::span<std::uint32_t>> groups;
				if (items.size() <= 4)
				{
					groups.push_back(items);
				}
				else
				{
					split_half(centres, items);
					const auto middle = items.size() / 2;
					for (const auto half : {items.first(middle), items.subspan(middle)})
					{
						if (half.size() > 4)
						{
							split_half(centres, half);
							groups.push_back(half.first(half.size() / 2));
							groups.push_back(half.subspan(half.size() / 2));
						}
						else
						{
							groups.push_back(half);
						}
					}
				}

				const auto block = static_cast<std::uint32_t>(nodes.size());
				nodes.resize(nodes.size() + groups.size());
				for (std::size_t c = 0; c < groups.size(); c++)
				{
					auto& child = nodes[block + c];
					auto& slot = nodes[node].children[c];
					nodes[node].data[c] = block + static_cast<std::uint32_t>(c);
					for (const auto i : groups[c])
					{
						include(slot, boxes[i]);
					}

					if (groups[c].size() <= 4)
					{
						child.leaf = true;
						child.data.fill(0xFFFFFFFF);
						for (std::size_t k = 0; k < groups[c].size(); k++)
						{
							child.children[k] = boxes[groups[c][k]];
							child.data[k] = values[groups[c][k]];
						}
					}
				}

				for (std::size_t c = 0; c < groups.size(); c++)
				{
					if (groups[c].size() > 4)
					{
						fill(block + static_cast<std::uint32_t>(c), groups[c]);
					}
				}
			};

			fill(1, all);
			return nodes;
		}

		std::vector<std::uint8_t> bounds_data(const bounds& box)
		{
			std::vector<std::uint8_t> out;
			for (std::size_t side = 0; side < 6; side += 3)
			{
				for (std::size_t axis = 0; axis < 3; axis++)
				{
					put(out, box[side + axis]);
				}

				put(out, 0.0f);
			}

			return out;
		}

		template <typename T>
		std::vector<std::uint8_t> bytes_of(const std::vector<T>& values)
		{
			std::vector<std::uint8_t> out(values.size() * sizeof(T));
			std::memcpy(out.data(), values.data(), out.size());
			return out;
		}
	}

	std::uint32_t add_compressed_mesh_shape(object_builder& builder, const compressed_mesh& mesh, compressed_mesh_stats* stats)
	{
		struct primitive
		{
			bool convex{};
			// The convex shape, or the corners of a triangle (3) or of a quad (4): (a, b, c, d) holds (a, b, c) and (a, c, d).
			std::uint32_t index{};
			std::uint16_t tag{};
			std::array<std::uint32_t, 4> vertices{};
			std::uint32_t corners{};
		};

		using dvector = std::array<double, 3>;
		const auto vector_of = [&](const std::uint32_t from, const std::uint32_t to)
		{
			return dvector{static_cast<double>(mesh.vertices[to][0]) - mesh.vertices[from][0], static_cast<double>(mesh.vertices[to][1]) -
				mesh.vertices[from][1], static_cast<double>(mesh.vertices[to][2]) - mesh.vertices[from][2]};
		};

		const auto cross = [](const dvector& a, const dvector& b)
		{
			return dvector{a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]};
		};

		const auto dot = [](const dvector& a, const dvector& b)
		{
			return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
		};

		// Triangles with an area.
		std::vector<std::uint32_t> triangles;
		for (std::uint32_t t = 0; t < mesh.triangles.size(); t++)
		{
			const auto& v = mesh.triangles[t].vertices;
			if (std::ranges::any_of(v, [&](const std::uint32_t i) { return i >= mesh.vertices.size(); }))
			{
				throw std::runtime_error("a mesh triangle names a vertex that the mesh does not have");
			}

			if (v[0] == v[1] || v[1] == v[2] || v[0] == v[2])
			{
				continue;
			}

			const auto normal = cross(vector_of(v[0], v[1]), vector_of(v[0], v[2]));
			if (normal[0] != 0.0 || normal[1] != 0.0 || normal[2] != 0.0)
			{
				triangles.push_back(t);
			}
		}

		// Two triangles with the same shape tag that share an edge (in opposite directions) and make a flat convex quad become
		// one primitive, as the Replay tools store most world faces.
		std::unordered_map<std::uint64_t, std::uint32_t> edges;
		for (std::uint32_t k = 0; k < triangles.size(); k++)
		{
			const auto& v = mesh.triangles[triangles[k]].vertices;
			for (std::size_t e = 0; e < 3; e++)
			{
				edges.emplace(static_cast<std::uint64_t>(v[e]) << 32 | v[(e + 1) % 3], k);
			}
		}

		std::vector<primitive> primitives;
		std::vector<bool> paired(triangles.size());
		for (std::uint32_t k = 0; k < triangles.size(); k++)
		{
			if (paired[k])
			{
				continue;
			}

			const auto& triangle = mesh.triangles[triangles[k]];
			const auto& v = triangle.vertices;
			primitive result{false, triangles[k], triangle.shape_tag, {v[0], v[1], v[2], v[2]}, 3};
			for (std::size_t e = 0; e < 3; e++)
			{
				// The shared edge runs from c to a in (a, b, c), and from a to c in (a, c, d).
				const auto a = v[(e + 1) % 3];
				const auto b = v[(e + 2) % 3];
				const auto c = v[e];
				const auto other = edges.find(static_cast<std::uint64_t>(a) << 32 | c);
				if (other == edges.end() || other->second == k || paired[other->second])
				{
					continue;
				}

				const auto& second = mesh.triangles[triangles[other->second]];
				const auto d = second.vertices[0] != a && second.vertices[0] != c ? second.vertices[0] :
					second.vertices[1] != a && second.vertices[1] != c ? second.vertices[1] : second.vertices[2];
				if (second.shape_tag != triangle.shape_tag || d == b || d == a || d == c)
				{
					continue;
				}

				const auto normal = cross(vector_of(a, b), vector_of(a, c));
				const auto length = std::sqrt(dot(normal, normal));
				const std::array<std::uint32_t, 4> ring{a, b, c, d};
				auto flat = std::abs(dot(vector_of(a, d), normal)) / length <= 1e-4;
				for (std::size_t corner = 0; corner < 4 && flat; corner++)
				{
					const auto turn = cross(vector_of(ring[corner], ring[(corner + 1) % 4]), vector_of(ring[(corner + 1) % 4], ring[(corner + 2) % 4]));
					flat = dot(turn, normal) > 0.0;
				}

				if (flat)
				{
					result.vertices = ring;
					result.corners = 4;
					paired[other->second] = true;
					break;
				}
			}

			paired[k] = true;
			primitives.push_back(result);
		}

		std::vector<bounds> exact;
		for (const auto& p : primitives)
		{
			auto box = empty_bounds;
			for (std::uint32_t corner = 0; corner < p.corners; corner++)
			{
				const auto& point = mesh.vertices[p.vertices[corner]];
				include(box, {point[0], point[1], point[2], point[0], point[1], point[2]});
			}

			exact.push_back(box);
		}

		for (std::uint32_t i = 0; i < mesh.convex_shapes.size(); i++)
		{
			primitives.push_back({true, i, mesh.convex_shapes[i].shape_tag});
			exact.push_back(instance_bounds(mesh.convex_shapes[i]));
		}

		const auto corners_of = [&](const std::uint32_t p)
		{
			return std::span<const std::uint32_t>(primitives[p].vertices.data(), primitives[p].corners);
		};

		if (primitives.empty())
		{
			throw std::runtime_error("a compressed mesh has no primitives");
		}

		// The domain of the mesh: triangles grow by the convex radius.
		auto domain = empty_bounds;
		std::vector<vector3> centres;
		for (std::size_t p = 0; p < primitives.size(); p++)
		{
			include(domain, primitives[p].convex ? exact[p] : expanded(exact[p], mesh.convex_radius));
			centres.push_back(centre_of(exact[p]));
		}

		const auto shared_grid = make_grid(domain, shared_bits);

		// Sections of at most 127 primitives and 256 vertex entries (a convex shape takes 4).
		std::vector<std::uint32_t> order(primitives.size());
		std::iota(order.begin(), order.end(), 0u);
		const auto sections = split_items(centres, order, [&](const std::span<const std::uint32_t> range)
		{
			if (range.size() > section_primitives)
			{
				return false;
			}

			std::unordered_set<std::uint32_t> vertices;
			std::size_t entries{};
			for (const auto p : range)
			{
				if (primitives[p].convex)
				{
					entries += 4;
					continue;
				}

				for (const auto v : corners_of(p))
				{
					vertices.insert(v);
				}
			}

			return entries + vertices.size() <= section_entries;
		});

		const auto section_count = static_cast<std::uint32_t>(sections.groups.size());
		if (section_count > 0x8000)
		{
			throw std::runtime_error("a compressed mesh needs more than 32768 sections");
		}

		// A vertex that more than one section uses is shared.
		constexpr auto unused = 0xFFFFFFFFu;
		std::vector<std::uint32_t> vertex_section(mesh.vertices.size(), unused);
		std::vector<bool> shared(mesh.vertices.size());
		for (std::uint32_t s = 0; s < section_count; s++)
		{
			for (const auto p : sections.groups[s])
			{
				if (primitives[p].convex)
				{
					continue;
				}

				for (const auto v : corners_of(p))
				{
					if (vertex_section[v] == unused)
					{
						vertex_section[v] = s;
					}
					else if (vertex_section[v] != s)
					{
						shared[v] = true;
					}
				}
			}
		}

		struct section_data
		{
			std::vector<std::uint32_t> primitives;
			std::vector<std::uint32_t> packed;
			grid packed_grid{};
			std::vector<std::uint16_t> table;
			std::vector<std::array<std::uint8_t, 4>> encoded;
			std::vector<bounds> decoded_bounds;
			bounds domain = empty_bounds;
			split_tree tree;
		};

		std::vector<section_data> data(section_count);
		std::vector<std::uint32_t> shared_index(mesh.vertices.size(), unused);
		std::vector<std::uint64_t> shared_vertices;
		std::vector<vector3> decoded(mesh.vertices.size());

		// The vertices of one section are packed when the grid of the section keeps them within the tolerance. The tolerance
		// grows until the shared vertices fit in the 16 bits of the section tables; the Replay tools pack every vertex of one
		// section.
		const auto multi = shared;
		auto tolerance = mesh.packed_tolerance;
		for (;; tolerance = tolerance < 1e6f ? tolerance * 8.0f : std::numeric_limits<float>::infinity())
		{
			shared = multi;
			for (std::uint32_t s = 0; s < section_count; s++)
			{
				auto& section = data[s];
				std::vector<std::uint32_t> candidates;
				std::unordered_set<std::uint32_t> seen;
				for (const auto p : sections.groups[s])
				{
					if (!primitives[p].convex)
					{
						for (const auto v : corners_of(p))
						{
							if (!shared[v] && seen.insert(v).second)
							{
								candidates.push_back(v);
							}
						}
					}
				}

				for (auto changed = true; changed && !candidates.empty();)
				{
					auto box = empty_bounds;
					for (const auto v : candidates)
					{
						const auto& p = mesh.vertices[v];
						include(box, {p[0], p[1], p[2], p[0], p[1], p[2]});
					}

					section.packed_grid = make_grid(box, packed_bits);
					const auto before = candidates.size();
					std::erase_if(candidates, [&](const std::uint32_t v)
					{
						for (std::size_t axis = 0; axis < 3; axis++)
						{
							const auto value = mesh.vertices[v][axis];
							if (std::abs(section.packed_grid.decode(axis, section.packed_grid.encode(axis, value)) - value) > tolerance)
							{
								shared[v] = true;
								return true;
							}
						}

						return false;
					});

					changed = candidates.size() != before;
				}

				section.packed = candidates;
			}

			if (std::ranges::count(shared, true) <= 0x10000 || std::isinf(tolerance))
			{
				break;
			}
		}

		for (const auto& section : data)
		{
			for (const auto v : section.packed)
			{
				for (std::size_t axis = 0; axis < 3; axis++)
				{
					decoded[v][axis] = section.packed_grid.decode(axis, section.packed_grid.encode(axis, mesh.vertices[v][axis]));
				}
			}
		}

		// Shared vertices in order of use.
		for (std::uint32_t s = 0; s < section_count; s++)
		{
			for (const auto p : sections.groups[s])
			{
				if (primitives[p].convex)
				{
					continue;
				}

				for (const auto v : corners_of(p))
				{
					if (!shared[v] || shared_index[v] != unused)
					{
						continue;
					}

					std::array<std::uint64_t, 3> q{};
					for (std::size_t axis = 0; axis < 3; axis++)
					{
						q[axis] = shared_grid.encode(axis, mesh.vertices[v][axis]);
						decoded[v][axis] = shared_grid.decode(axis, static_cast<std::uint32_t>(q[axis]));
					}

					shared_index[v] = static_cast<std::uint32_t>(shared_vertices.size());
					shared_vertices.push_back(q[0] | q[1] << 21 | q[2] << 42);
				}
			}
		}

		if (shared_vertices.size() > 0x10000)
		{
			throw std::runtime_error(std::format("a compressed mesh needs {} shared vertices (65536 at most)", shared_vertices.size()));
		}

		if (stats)
		{
			stats->sections = section_count;
			stats->shared_vertices = shared_vertices.size();
			stats->packed_vertices = 0;
			for (const auto& section : data)
			{
				stats->packed_vertices += section.packed.size();
			}

			stats->tolerance = tolerance;
			stats->largest_error = 0.0f;
			for (std::size_t v = 0; v < mesh.vertices.size(); v++)
			{
				if (vertex_section[v] != unused)
				{
					for (std::size_t axis = 0; axis < 3; axis++)
					{
						stats->largest_error = std::max(stats->largest_error, std::abs(decoded[v][axis] - mesh.vertices[v][axis]));
					}
				}
			}
		}

		// The decoded bounds of each primitive: triangles with and without the convex radius.
		std::vector<bounds> primitive_bounds(primitives.size());
		std::vector<bounds> tree_bounds(primitives.size());
		for (std::size_t p = 0; p < primitives.size(); p++)
		{
			if (primitives[p].convex)
			{
				primitive_bounds[p] = exact[p];
				tree_bounds[p] = exact[p];
				continue;
			}

			primitive_bounds[p] = empty_bounds;
			for (const auto v : corners_of(static_cast<std::uint32_t>(p)))
			{
				include(primitive_bounds[p], {decoded[v][0], decoded[v][1], decoded[v][2], decoded[v][0], decoded[v][1], decoded[v][2]});
			}

			tree_bounds[p] = expanded(primitive_bounds[p], mesh.convex_radius);
		}

		// Primitive order in a section: the leaves of its tree, then by shape tag.
		std::uint32_t most_primitives{};
		for (std::uint32_t s = 0; s < section_count; s++)
		{
			auto& section = data[s];
			auto tree = split_items(centres, sections.groups[s], [](const std::span<const std::uint32_t> range)
			{
				return range.size() == 1;
			});

			std::vector<std::uint32_t> leaf_order;
			for (const auto& group : tree.groups)
			{
				leaf_order.push_back(group[0]);
			}

			section.primitives = leaf_order;
			std::ranges::stable_sort(section.primitives, {}, [&](const std::uint32_t p)
			{
				return primitives[p].tag;
			});

			// The tree leaves name primitives by their index in the section.
			std::unordered_map<std::uint32_t, std::uint32_t> local;
			for (std::uint32_t k = 0; k < section.primitives.size(); k++)
			{
				local[section.primitives[k]] = k;
			}

			for (auto& group : tree.groups)
			{
				group[0] = local.at(group[0]);
			}

			section.tree = std::move(tree);
			most_primitives = std::max(most_primitives, static_cast<std::uint32_t>(section.primitives.size()));
			for (const auto p : section.primitives)
			{
				include(section.domain, tree_bounds[p]);
			}
		}

		const auto primitive_bits = static_cast<std::uint32_t>(std::bit_width(most_primitives - 1)) + 1;
		const auto section_bits = static_cast<std::uint32_t>(std::bit_width(section_count - 1));
		const auto key_of = [&](const std::uint32_t section, const std::uint32_t local)
		{
			return section << primitive_bits | local << 1;
		};

		// Section tables, primitives, data runs and trees.
		std::vector<std::uint8_t> section_bytes;
		std::vector<std::uint8_t> primitive_bytes;
		std::vector<std::uint16_t> table;
		std::vector<std::uint32_t> packed_vertices;
		std::vector<std::uint8_t> run_bytes;
		std::vector<std::vector<std::uint8_t>> section_nodes(section_count);
		std::vector<bounds> section_domains(section_count);
		std::vector<std::uint32_t> simd_values;
		std::vector<bounds> simd_boxes;
		std::vector<vector3> simd_centres;
		std::uint32_t max_key{};
		std::size_t keys{};
		std::size_t runs{};
		for (std::uint32_t s = 0; s < section_count; s++)
		{
			auto& section = data[s];
			std::unordered_map<std::uint32_t, std::uint32_t> entry;
			for (std::uint32_t k = 0; k < section.packed.size(); k++)
			{
				entry[section.packed[k]] = k;
				std::uint32_t word{};
				for (std::size_t axis = 0; axis < 3; axis++)
				{
					word |= section.packed_grid.encode(axis, mesh.vertices[section.packed[k]][axis]) << (axis == 0 ? 0 : axis == 1 ? 11 : 22);
				}

				packed_vertices.push_back(word);
			}

			const auto first_shared = static_cast<std::uint32_t>(table.size());
			const auto packed_count = static_cast<std::uint32_t>(section.packed.size());
			const auto table_entry = [&]
			{
				return packed_count + static_cast<std::uint32_t>(table.size()) - first_shared;
			};

			auto has_convex = false;
			std::vector<std::uint16_t> tags;
			for (std::uint32_t k = 0; k < section.primitives.size(); k++)
			{
				const auto p = section.primitives[k];
				tags.push_back(primitives[p].tag);
				simd_values.push_back(key_of(s, k));
				simd_boxes.push_back(primitive_bounds[p]);
				simd_centres.push_back(centre_of(primitive_bounds[p]));
				max_key = std::max(max_key, key_of(s, k) | (primitives[p].corners == 4 ? 1u : 0u));
				keys += primitives[p].corners == 4 ? 2 : 1;
				if (primitives[p].convex)
				{
					// The record keeps the number of shared vertices so far, as the Replay tools do.
					has_convex = true;
					const auto at = static_cast<std::uint8_t>(table_entry());
					table.push_back(convex_record);
					table.push_back(static_cast<std::uint16_t>(std::min<std::size_t>(shared_vertices.size(), 0xFFFF)));
					table.push_back(static_cast<std::uint16_t>(primitives[p].index & 0xFFFF));
					table.push_back(static_cast<std::uint16_t>(primitives[p].index >> 16));
					primitive_bytes.insert(primitive_bytes.end(), {at, at, 4, at});
					continue;
				}

				std::array<std::uint8_t, 4> local{};
				const auto corners = corners_of(p);
				for (std::size_t corner = 0; corner < corners.size(); corner++)
				{
					auto found = entry.find(corners[corner]);
					if (found == entry.end())
					{
						found = entry.emplace(corners[corner], table_entry()).first;
						table.push_back(static_cast<std::uint16_t>(shared_index.at(corners[corner])));
					}

					local[corner] = static_cast<std::uint8_t>(found->second);
				}

				// A triangle repeats its last corner. A flat convex quad keeps b above d: (c, d, a, b) holds the same triangles.
				if (corners.size() == 3)
				{
					primitive_bytes.insert(primitive_bytes.end(), {local[0], local[1], local[2], local[2]});
				}
				else if (local[1] > local[3])
				{
					primitive_bytes.insert(primitive_bytes.end(), {local[0], local[1], local[2], local[3]});
				}
				else
				{
					primitive_bytes.insert(primitive_bytes.end(), {local[2], local[3], local[0], local[1]});
				}
			}

			if (table_entry() > section_entries)
			{
				throw std::runtime_error("a compressed mesh section has more than 256 vertex entries");
			}

			// Runs of primitives with the same shape tag.
			const auto first_run = runs;
			for (std::size_t k = 0; k < tags.size();)
			{
				auto end = k;
				while (end < tags.size() && tags[end] == tags[k])
				{
					end++;
				}

				put(run_bytes, tags[k]);
				put(run_bytes, static_cast<std::uint8_t>(k));
				put(run_bytes, static_cast<std::uint8_t>(end - k));
				runs++;
				k = end;
			}

			// The tree of the section, within its domain.
			section_domains[s] = section.domain;
			std::vector<bounds> leaf_bounds;
			for (const auto& group : section.tree.groups)
			{
				leaf_bounds.push_back(tree_bounds[section.primitives[group[0]]]);
			}

			const auto encoded = encode_tree(section.tree, leaf_bounds, section.domain);
			for (std::size_t n = 0; n < section.tree.nodes.size(); n++)
			{
				const auto& node = section.tree.nodes[n];
				const auto value = node.leaf ? section.tree.groups[node.value][0] << 1 : node.value << 1 | 1;
				section_nodes[s].insert(section_nodes[s].end(), encoded.xyz[n].begin(), encoded.xyz[n].end());
				section_nodes[s].push_back(static_cast<std::uint8_t>(value));
			}

			// Section: nodes, domain, codecParms, then the first indices and counts.
			std::vector<std::uint8_t> bytes(16);
			std::ranges::copy(bounds_data(section.domain), std::back_inserter(bytes));
			const auto parms = section.packed.empty() ? std::array<float, 6>{real_max, real_max, real_max, -std::numeric_limits<float>::infinity(),
				-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity()} : std::array<float, 6>{section.packed_grid.low[0],
				section.packed_grid.low[1], section.packed_grid.low[2], section.packed_grid.scale[0], section.packed_grid.scale[1], section.packed_grid.scale[2]};
			for (const auto value : parms)
			{
				put(bytes, value);
			}

			put(bytes, static_cast<std::uint32_t>(packed_vertices.size() - section.packed.size()));
			put(bytes, first_shared);
			put(bytes, static_cast<std::uint32_t>(primitive_bytes.size() / 4 - section.primitives.size()));
			put(bytes, static_cast<std::uint32_t>(first_run));
			put(bytes, static_cast<std::uint8_t>(section.packed.size()));
			put(bytes, static_cast<std::uint8_t>(section.primitives.size()));
			put(bytes, static_cast<std::uint8_t>(runs - first_run));
			put(bytes, std::uint8_t{});
			// leafIndex is set with the top tree.
			put(bytes, std::uint16_t{});
			put(bytes, std::uint8_t{});
			put(bytes, static_cast<std::uint8_t>(has_convex));
			section_bytes.insert(section_bytes.end(), bytes.begin(), bytes.end());
		}

		if (runs > 0xFFFFFFFF || primitive_bytes.size() / 4 > 0xFFFFFFFF)
		{
			throw std::runtime_error("a compressed mesh is too large");
		}

		// The top tree over the sections, within the domain of the mesh.
		const auto top = encode_tree(sections, section_domains, domain);
		std::vector<std::uint8_t> top_nodes;
		for (std::size_t n = 0; n < sections.nodes.size(); n++)
		{
			const auto& node = sections.nodes[n];
			const auto value = node.leaf ? node.value : 0x8000 | node.value;
			top_nodes.insert(top_nodes.end(), top.xyz[n].begin(), top.xyz[n].end());
			top_nodes.push_back(static_cast<std::uint8_t>(value >> 8));
			top_nodes.push_back(static_cast<std::uint8_t>(value));
		}

		for (std::uint32_t s = 0; s < section_count; s++)
		{
			const auto leaf = static_cast<std::uint16_t>(top.leaf_nodes[s]);
			std::memcpy(section_bytes.data() + s * 96 + 92, &leaf, sizeof(leaf));
		}

		const auto simd = build_simd_tree(simd_boxes, simd_centres, simd_values);
		const auto key_bits = section_bits + primitive_bits;
		const auto interior_bits = max_key + 1;

		const auto object = builder.object("hknpCompressedMeshShape");
		builder.set(object, "flags", std::uint16_t{0x4});
		builder.set(object, "type", std::uint8_t{9});
		builder.set(object, "numShapeKeyBits", static_cast<std::uint8_t>(key_bits));
		builder.set(object, "dispatchType", std::uint8_t{3});
		builder.set(object, "convexRadius", mesh.convex_radius);
		builder.set(object, "userData", mesh.user_data);
		builder.set(object, "shapeTagCodecInfo", std::uint32_t{0xFFFFFFFF});

		const auto tree = builder.object("hknpCompressedMeshShapeData");
		builder.point(object, "data", tree);

		// No triangle is marked interior.
		const auto words = (interior_bits + 31) / 32;
		builder.point(object, "triangleIsInterior.storage.words", builder.array("hkUint32", words, std::vector<std::uint8_t>(words * 4)));
		builder.set(object, "triangleIsInterior.storage.numBits", static_cast<std::int32_t>(interior_bits));

		if (!mesh.convex_shapes.empty())
		{
			std::vector<std::uint8_t> instances;
			for (const auto& instance : mesh.convex_shapes)
			{
				// The w of the translation is 1.
				std::ranges::copy(instance_data(instance, 0x3F800000), std::back_inserter(instances));
			}

			const auto externs = builder.array("hknpShapeInstance", static_cast<std::uint32_t>(mesh.convex_shapes.size()), instances);
			builder.point(object, "externShapes", externs);
			for (std::uint32_t i = 0; i < mesh.convex_shapes.size(); i++)
			{
				builder.point(externs, "shape", mesh.convex_shapes[i].shape, i);
			}
		}

		builder.point(tree, "meshTree.nodes", builder.array("hkcdCompressedAabbCodecs::Aabb5BytesCodec", static_cast<std::uint32_t>(sections.nodes.size()), top_nodes));
		builder.set(tree, "meshTree.domain", std::span<const std::uint8_t>(bounds_data(domain)));
		builder.set(tree, "meshTree.numPrimitiveKeys", static_cast<std::int32_t>(keys));
		builder.set(tree, "meshTree.bitsPerKey", static_cast<std::int32_t>(key_bits));
		builder.set(tree, "meshTree.maxKeyValue", max_key);
		builder.set(tree, "meshTree.primitiveStoresIsFlatConvex", std::uint8_t{0xFF});

		const auto section_items = builder.array("hkcdStaticMeshTree::Section", section_count, section_bytes);
		builder.point(tree, "meshTree.sections", section_items);
		for (std::uint32_t s = 0; s < section_count; s++)
		{
			builder.point(section_items, "nodes", builder.array("hkcdCompressedAabbCodecs::Aabb4BytesCodec",
				static_cast<std::uint32_t>(section_nodes[s].size() / 4), section_nodes[s]), s);
		}

		builder.point(tree, "meshTree.primitives", builder.array("hkcdStaticMeshTree::Primitive", static_cast<std::uint32_t>(primitive_bytes.size() / 4),
			primitive_bytes));
		if (!table.empty())
		{
			builder.point(tree, "meshTree.sharedVerticesIndex", builder.array("hkUint16", static_cast<std::uint32_t>(table.size()), bytes_of(table)));
		}

		if (!packed_vertices.empty())
		{
			builder.point(tree, "meshTree.packedVertices", builder.array("unsigned int", static_cast<std::uint32_t>(packed_vertices.size()),
				bytes_of(packed_vertices)));
		}

		if (!shared_vertices.empty())
		{
			builder.point(tree, "meshTree.sharedVertices", builder.array("unsigned long long", static_cast<std::uint32_t>(shared_vertices.size()),
				bytes_of(shared_vertices)));
		}

		builder.point(tree, "meshTree.primitiveDataRuns", builder.array("hkcdDefaultStaticMeshTree::PrimitiveDataRun", static_cast<std::uint32_t>(runs),
			run_bytes));
		builder.point(tree, "simdTree.nodes", builder.array("hkcdSimdTree::Node", static_cast<std::uint32_t>(simd.size()), write_nodes(simd)));
		builder.set(tree, "simdTree.isCompact", std::uint8_t{1});
		builder.set(tree, "hasSimdTree", std::uint8_t{1});
		return object;
	}

	decoded_mesh read_compressed_mesh_shape(const tagfile& file, const type_table& types, const std::uint32_t index, const bool exact)
	{
		const auto& shape = file.items.at(index);
		const auto* data_item = target_of(file, shape, offset_of(types, shape.type, "data"));
		if (!data_item)
		{
			throw std::runtime_error("a compressed mesh has no data");
		}

		const auto& data = *data_item;
		const auto member = [&](const std::string_view name)
		{
			return offset_of(types, data.type, name);
		};

		const auto array = [&](const item& owner, const std::uint32_t offset) -> const item&
		{
			static const item empty{};
			const auto* found = target_of(file, owner, offset);
			return found ? *found : empty;
		};

		decoded_mesh result;
		result.convex_radius = get<float>(shape.data, offset_of(types, shape.type, "convexRadius"));
		result.user_data = get<std::uint64_t>(shape.data, offset_of(types, shape.type, "userData"));

		bounds domain{};
		const auto domain_at = member("meshTree.domain");
		for (std::size_t axis = 0; axis < 3; axis++)
		{
			domain[axis] = get<float>(data.data, domain_at + axis * 4);
			domain[axis + 3] = get<float>(data.data, domain_at + 16 + axis * 4);
		}

		const auto bits_per_key = get<std::int32_t>(data.data, member("meshTree.bitsPerKey"));
		const auto& sections = array(data, member("meshTree.sections"));
		const auto& primitives = array(data, member("meshTree.primitives"));
		const auto& table = array(data, member("meshTree.sharedVerticesIndex"));
		const auto& packed = array(data, member("meshTree.packedVertices"));
		const auto& shared = array(data, member("meshTree.sharedVertices"));
		const auto& runs = array(data, member("meshTree.primitiveDataRuns"));
		const auto& top = array(data, member("meshTree.nodes"));
		const auto& externs = array(shape, offset_of(types, shape.type, "externShapes"));

		// Convex shape instances.
		for (std::uint32_t i = 0; i < externs.count; i++)
		{
			compound_instance instance;
			for (std::size_t column = 0; column < 3; column++)
			{
				for (std::size_t axis = 0; axis < 3; axis++)
				{
					instance.rotation[column][axis] = get<float>(externs.data, i * 112 + column * 16 + axis * 4);
				}
			}

			for (std::size_t axis = 0; axis < 3; axis++)
			{
				instance.translation[axis] = get<float>(externs.data, i * 112 + 48 + axis * 4);
			}

			instance.shape_tag = get<std::uint16_t>(externs.data, i * 112 + 88);
			const auto child = std::ranges::find(externs.pointers, i * 112 + 80, &pointer_field::offset);
			if (child == externs.pointers.end())
			{
				throw std::runtime_error("a compressed mesh convex shape has no shape");
			}

			const auto& child_item = file.items.at(child->target);
			instance.convex_radius = get<float>(child_item.data, offset_of(types, child_item.type, "convexRadius"));
			const auto* vertices = target_of(file, child_item, offset_of(types, child_item.type, "vertices"));
			for (std::uint32_t v = 0; vertices && v < vertices->count; v++)
			{
				instance.vertices.push_back({get<float>(vertices->data, v * 16), get<float>(vertices->data, v * 16 + 4), get<float>(vertices->data, v * 16 + 8)});
			}

			result.convex_shapes.push_back(std::move(instance));
			result.convex_items.push_back(child->target);
		}

		result.convex_keys.assign(externs.count, 0xFFFFFFFF);

		const auto section_bits = static_cast<std::uint32_t>(std::bit_width(std::max(sections.count, 1u) - 1));
		const auto primitive_bits = static_cast<std::uint32_t>(bits_per_key) - section_bits;
		const auto section_type = sections.type;
		const auto section_size = sections.count ? types.size_of(section_type) : 0;
		const auto section_member = [&](const std::string_view name)
		{
			return offset_of(types, section_type, name);
		};

		auto shared_grid = make_grid(domain, shared_bits);
		std::vector<bounds> section_geometry(sections.count, empty_bounds);
		float largest_step = std::max({shared_grid.scale[0], shared_grid.scale[1], shared_grid.scale[2]});
		for (std::uint32_t s = 0; s < sections.count; s++)
		{
			const auto base = s * section_size;
			bounds section_domain{};
			const auto domain_offset = base + section_member("domain");
			for (std::size_t axis = 0; axis < 3; axis++)
			{
				section_domain[axis] = get<float>(sections.data, domain_offset + axis * 4);
				section_domain[axis + 3] = get<float>(sections.data, domain_offset + 16 + axis * 4);
			}

			grid packed_grid{};
			packed_grid.bits = packed_bits;
			for (std::size_t axis = 0; axis < 3; axis++)
			{
				packed_grid.low[axis] = get<float>(sections.data, base + section_member("codecParms") + axis * 4);
				packed_grid.scale[axis] = get<float>(sections.data, base + section_member("codecParms") + 12 + axis * 4);
			}

			// A vertex is at most one step of its grid from where the tree saw it.
			auto step = std::max({shared_grid.scale[0], shared_grid.scale[1], shared_grid.scale[2]});
			const auto first_packed = get<std::uint32_t>(sections.data, base + section_member("firstPackedVertexIndex"));
			const auto first_shared = get<std::uint32_t>(sections.data, base + section_member("firstSharedVertexIndex"));
			const auto first_primitive = get<std::uint32_t>(sections.data, base + section_member("firstPrimitiveIndex"));
			const auto first_run = get<std::uint32_t>(sections.data, base + section_member("firstDataRunIndex"));
			const auto packed_count = get<std::uint8_t>(sections.data, base + section_member("numPackedVertices"));
			const auto primitive_count = get<std::uint8_t>(sections.data, base + section_member("numPrimitives"));
			const auto run_count = get<std::uint8_t>(sections.data, base + section_member("numDataRuns"));
			if (packed_count)
			{
				step = std::max({step, packed_grid.scale[0], packed_grid.scale[1], packed_grid.scale[2]});
			}

			largest_step = std::max(largest_step, step);

			const auto vertex = [&](const std::uint32_t local) -> vector3
			{
				if (local < packed_count)
				{
					const auto word = get<std::uint32_t>(packed.data, (first_packed + local) * 4);
					const std::array<std::uint32_t, 3> q{word & 0x7FF, word >> 11 & 0x7FF, word >> 22};
					return {packed_grid.decode(0, q[0]), packed_grid.decode(1, q[1]), packed_grid.decode(2, q[2])};
				}

				const auto shared_at = get<std::uint16_t>(table.data, (first_shared + local - packed_count) * 2);
				const auto word = get<std::uint64_t>(shared.data, shared_at * 8);
				const std::array<std::uint32_t, 3> q{static_cast<std::uint32_t>(word & 0x1FFFFF), static_cast<std::uint32_t>(word >> 21 & 0x1FFFFF),
					static_cast<std::uint32_t>(word >> 42)};
				return {shared_grid.decode(0, q[0]), shared_grid.decode(1, q[1]), shared_grid.decode(2, q[2])};
			};

			std::vector<std::uint16_t> tags(primitive_count);
			for (std::uint32_t r = 0; r < run_count; r++)
			{
				const auto value = get<std::uint16_t>(runs.data, (first_run + r) * 4);
				const auto first = get<std::uint8_t>(runs.data, (first_run + r) * 4 + 2);
				const auto count = get<std::uint8_t>(runs.data, (first_run + r) * 4 + 3);
				for (std::uint32_t k = first; k < static_cast<std::uint32_t>(first + count) && k < primitive_count; k++)
				{
					tags[k] = value;
				}
			}

			// The bounds that the tree of the section must hold for each primitive.
			std::vector<std::optional<bounds>> needed(primitive_count);
			for (std::uint32_t k = 0; k < primitive_count; k++)
			{
				std::array<std::uint8_t, 4> indices{};
				for (std::size_t i = 0; i < 4; i++)
				{
					indices[i] = get<std::uint8_t>(primitives.data, (first_primitive + k) * 4 + i);
				}

				const auto key = s << primitive_bits | k << 1;
				if (indices == removed_primitive)
				{
					result.removed++;
					continue;
				}

				// A custom primitive (a, b, n, b) keeps n entries of the section table from a. A convex shape keeps 146, the
				// number of shared vertices so far, then the index of the shape (low and high half).
				if (indices[1] == indices[3])
				{
					const auto at = first_shared + indices[0] - packed_count;
					if (indices[2] != 4 || get<std::uint16_t>(table.data, at * 2) != convex_record)
					{
						result.custom++;
						continue;
					}

					const auto convex = static_cast<std::uint32_t>(get<std::uint16_t>(table.data, (at + 2) * 2)) |
						static_cast<std::uint32_t>(get<std::uint16_t>(table.data, (at + 3) * 2)) << 16;
					if (convex >= result.convex_shapes.size())
					{
						throw std::runtime_error("a compressed mesh primitive names a convex shape that the mesh does not have");
					}

					result.convex_keys[convex] = key;
					needed[k] = instance_bounds(result.convex_shapes[convex]);
					include(section_geometry[s], *needed[k]);
					continue;
				}

				const std::array<std::array<std::uint8_t, 3>, 2> halves{{{indices[0], indices[1], indices[2]}, {indices[0], indices[2], indices[3]}}};
				auto box = empty_bounds;
				for (std::uint32_t half = 0; half < (indices[2] != indices[3] ? 2u : 1u); half++)
				{
					std::array<vector3, 3> triangle{};
					for (std::size_t corner = 0; corner < 3; corner++)
					{
						triangle[corner] = vertex(halves[half][corner]);
					}

					include(box, point_bounds(triangle));
					result.triangles.push_back(triangle);
					result.triangle_tags.push_back(tags[k]);
					result.triangle_keys.push_back(key | half);
				}

				needed[k] = expanded(box, result.convex_radius);
				include(section_geometry[s], *needed[k]);
			}

			// The tree of the section: odd data = inner node (2 * left leaves + 1), even = leaf (2 * primitive).
			const auto* nodes = target_of(file, sections, base + section_member("nodes"));
			if (!nodes || !nodes->count)
			{
				throw std::runtime_error("a compressed mesh section has no tree");
			}

			std::vector<std::pair<std::uint32_t, bounds>> pending{{0, section_domain}};
			std::size_t leaves{};
			while (!pending.empty())
			{
				const auto [node, parent] = pending.back();
				pending.pop_back();
				if (node >= nodes->count)
				{
					throw std::runtime_error("a compressed mesh section tree node is outside the tree");
				}

				const std::array<std::uint8_t, 3> xyz{get<std::uint8_t>(nodes->data, node * 4), get<std::uint8_t>(nodes->data, node * 4 + 1),
					get<std::uint8_t>(nodes->data, node * 4 + 2)};
				const auto value = get<std::uint8_t>(nodes->data, node * 4 + 3);
				const auto box = decode_child(parent, xyz);
				if (value & 1)
				{
					pending.emplace_back(node + 2 * (value >> 1), box);
					pending.emplace_back(node + 1, box);
					continue;
				}

				leaves++;
				const auto k = value >> 1u;
				if (k >= primitive_count || (needed[k] && !holds(box, *needed[k], exact ? 0.0f : step)))
				{
					throw std::runtime_error(std::format("a compressed mesh section tree leaf does not hold primitive {} of section {}", k, s));
				}
			}

			if (leaves != primitive_count)
			{
				throw std::runtime_error("a compressed mesh section tree does not hold each primitive once");
			}
		}

		// The top tree: 0x8000 marks an inner node with the count of left leaves, a leaf holds a section.
		std::vector<std::pair<std::uint32_t, bounds>> pending;
		if (top.count)
		{
			pending.emplace_back(0, domain);
		}

		std::size_t top_leaves{};
		while (!pending.empty())
		{
			const auto [node, parent] = pending.back();
			pending.pop_back();
			if (node >= top.count)
			{
				throw std::runtime_error("a compressed mesh tree node is outside the tree");
			}

			const std::array<std::uint8_t, 3> xyz{get<std::uint8_t>(top.data, node * 5), get<std::uint8_t>(top.data, node * 5 + 1),
				get<std::uint8_t>(top.data, node * 5 + 2)};
			const auto value = static_cast<std::uint32_t>(get<std::uint8_t>(top.data, node * 5 + 3)) << 8 | get<std::uint8_t>(top.data, node * 5 + 4);
			const auto box = decode_child(parent, xyz);
			if (value & 0x8000)
			{
				pending.emplace_back(node + 2 * (value & 0x7FFF), box);
				pending.emplace_back(node + 1, box);
				continue;
			}

			top_leaves++;
			if (value >= sections.count || !holds(box, section_geometry[value], exact ? 0.0f : largest_step))
			{
				throw std::runtime_error(std::format("a compressed mesh tree leaf does not hold section {}", value));
			}
		}

		if (top_leaves != sections.count)
		{
			throw std::runtime_error("a compressed mesh tree does not hold each section once");
		}

		return result;
	}
}
