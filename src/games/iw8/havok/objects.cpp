#include <std_include.hpp>
#include "objects.hpp"
#include "detail.hpp"

#include <bit>
#include <numeric>

namespace zonetool::iw8::havok
{
	using namespace detail;

	namespace
	{
		// hkHalf16: the high 16 bits of a float, rounded.
		std::uint16_t to_half16(const float value)
		{
			std::uint32_t bits{};
			std::memcpy(&bits, &value, sizeof(bits));
			return static_cast<std::uint16_t>((bits + 0x8000) >> 16);
		}

		const item& target_of(const tagfile& file, const item& owner, const std::uint32_t offset)
		{
			for (const auto& pointer : owner.pointers)
			{
				if (pointer.offset == offset)
				{
					return file.items.at(pointer.target);
				}
			}

			throw std::runtime_error("a Havok member points at nothing");
		}

		// The SIMD tree of a compound shape as the Replay tools build it: the instances in order, four to a leaf. A node
		// holds three leaves and a node with the rest. Node 0 is empty and node 1 is the root.
		std::vector<tree_node> build_tree(const std::vector<bounds>& boxes, const std::vector<std::uint32_t>& order)
		{
			std::vector<tree_node> nodes(2);
			const auto count = order.size();
			std::uint32_t parent = 1;
			std::size_t first{};
			while (true)
			{
				std::vector<std::pair<std::size_t, std::size_t>> ranges;
				for (auto position = first; position < count && ranges.size() < 4;)
				{
					const auto remaining = count - position;
					const auto take = ranges.size() < 3 || remaining <= 4 ? std::min<std::size_t>(remaining, 4) : remaining;
					ranges.emplace_back(position, position + take);
					position += take;
				}

				const auto block = static_cast<std::uint32_t>(nodes.size());
				nodes.resize(nodes.size() + ranges.size());
				for (std::size_t c = 0; c < ranges.size(); c++)
				{
					const auto [begin, end] = ranges[c];
					auto& child = nodes[block + c];
					auto& slot = nodes[parent].children[c];
					nodes[parent].data[c] = block + static_cast<std::uint32_t>(c);
					for (auto k = begin; k < end; k++)
					{
						include(slot, boxes[order[k]]);
					}

					if (end - begin <= 4)
					{
						child.leaf = true;
						child.data.fill(0xFFFFFFFF);
						for (auto k = begin; k < end; k++)
						{
							child.children[k - begin] = boxes[order[k]];
							child.data[k - begin] = order[k];
						}
					}
				}

				const auto [begin, end] = ranges.back();
				if (end - begin <= 4)
				{
					return nodes;
				}

				parent = block + static_cast<std::uint32_t>(ranges.size() - 1);
				first = begin;
			}
		}
	}

	object_builder::object_builder(const type_table& compendium)
		: types_(compendium)
	{
		this->items_.emplace_back();
	}

	const type_table& object_builder::types() const
	{
		return this->types_;
	}

	std::uint32_t object_builder::type_index(const std::string_view identity) const
	{
		const auto index = this->types_.find(identity);
		if (!index)
		{
			throw std::runtime_error(std::format("the type compendium has no type {}", identity));
		}

		return *index;
	}

	std::pair<std::uint32_t, std::uint32_t> object_builder::member(const std::uint32_t item, const std::string_view name) const
	{
		const auto type = this->items_.at(item).type;
		const auto found = this->types_.member(type, name);
		if (!found)
		{
			throw std::runtime_error(std::format("{} has no member {}", this->types_.identity(type), name));
		}

		return *found;
	}

	std::uint32_t object_builder::object(const std::string_view type)
	{
		auto& entry = this->items_.emplace_back();
		entry.type = this->type_index(type);
		entry.flags = 0x10;
		entry.count = 1;
		entry.data.assign(this->types_.size_of(entry.type), 0);
		return static_cast<std::uint32_t>(this->items_.size() - 1);
	}

	std::uint32_t object_builder::array(const std::string_view element_type, const std::uint32_t count, const std::span<const std::uint8_t> data)
	{
		auto& entry = this->items_.emplace_back();
		entry.type = this->type_index(element_type);
		entry.flags = 0x20;
		entry.count = count;
		if (data.size() != static_cast<std::size_t>(this->types_.size_of(entry.type)) * count)
		{
			throw std::runtime_error(std::format("an array of {} has {} bytes for {} elements", element_type, data.size(), count));
		}

		entry.data.assign(data.begin(), data.end());
		return static_cast<std::uint32_t>(this->items_.size() - 1);
	}

	std::uint32_t object_builder::raw(const std::uint32_t type, const std::uint8_t flags, const std::uint32_t count, std::vector<std::uint8_t> data)
	{
		auto& entry = this->items_.emplace_back();
		entry.type = type;
		entry.flags = flags;
		entry.count = count;
		entry.data = std::move(data);
		return static_cast<std::uint32_t>(this->items_.size() - 1);
	}

	void object_builder::pointer(const std::uint32_t item, const std::uint32_t offset, const std::uint32_t pointer_type, const std::uint32_t target)
	{
		this->items_.at(item).pointers.emplace_back(offset, pointer_type, target);
	}

	void object_builder::point(const std::uint32_t item, const std::string_view name, const std::uint32_t target, const std::uint32_t element)
	{
		const auto [offset, type] = this->member(item, name);
		const auto stride = this->types_.size_of(this->items_.at(item).type);
		this->items_[item].pointers.emplace_back(offset + element * stride, type, target);
	}

	void object_builder::set(const std::uint32_t item, const std::string_view name, const std::span<const std::uint8_t> bytes,
		const std::uint32_t element)
	{
		const auto [offset, type] = this->member(item, name);
		auto& data = this->items_.at(item).data;
		const auto at = offset + element * this->types_.size_of(this->items_[item].type);
		if (at + bytes.size() > data.size())
		{
			throw std::runtime_error(std::format("member {} is outside its object", name));
		}

		std::ranges::copy(bytes, data.begin() + at);
	}

	tagfile object_builder::finish(const std::string_view sdk_version, const std::optional<std::uint64_t> compendium, const std::uint32_t root)
	{
		if (root == 0 || root >= this->items_.size())
		{
			throw std::runtime_error("the root of a tagfile is not one of its items");
		}

		// The root becomes item 1, and the items before it move up by one.
		const auto renumber = [root](const std::uint32_t index) -> std::uint32_t
		{
			return index == root ? 1u : index && index < root ? index + 1 : index;
		};

		tagfile file;
		file.sdk_version = sdk_version;
		file.compendium = compendium;
		file.items.resize(this->items_.size());
		for (std::uint32_t index = 0; index < this->items_.size(); index++)
		{
			file.items[renumber(index)] = std::move(this->items_[index]);
		}

		for (auto& entry : file.items)
		{
			for (auto& pointer : entry.pointers)
			{
				pointer.target = renumber(pointer.target);
			}

			std::ranges::sort(entry.pointers, {}, &pointer_field::offset);
		}

		this->items_.clear();
		this->items_.emplace_back();
		return file;
	}

	std::uint32_t add_convex_polytope_shape(object_builder& builder, const polytope& shape, const convex_options& options)
	{
		if (shape.vertices.size() > 256 || shape.faces.size() > 0xFFFF)
		{
			throw std::runtime_error("a convex polytope has more than 256 vertices");
		}

		const auto connectivity = connect(shape);
		const auto angles = min_half_angles(shape, connectivity);

		const auto object = builder.object("hknpConvexPolytopeShape");
		builder.set(object, "flags", options.flags);
		builder.set(object, "type", std::uint8_t{1});
		builder.set(object, "dispatchType", options.dispatch_type);
		builder.set(object, "userData", options.user_data);
		builder.set(object, "maxAllowedPenetration", to_half16(static_cast<float>(0.33 * inner_radius(shape))));

		std::vector<std::uint8_t> vertices;
		for (std::uint32_t i = 0; i < shape.vertices.size(); i++)
		{
			for (const auto value : shape.vertices[i])
			{
				put(vertices, value);
			}

			// The vertex index in the bits of w (0.5 + index).
			put(vertices, std::uint32_t{0x3F000000} | i);
		}

		std::vector<std::uint8_t> planes;
		std::vector<std::uint8_t> faces;
		std::vector<std::uint8_t> indices;
		std::vector<std::size_t> first_index;
		for (std::size_t f = 0; f < shape.faces.size(); f++)
		{
			for (const auto value : shape.planes[f])
			{
				put(planes, value);
			}

			// A face that repeats an earlier ring (flat shapes) uses its indices.
			auto first = indices.size();
			bool repeated = false;
			for (std::size_t earlier = 0; earlier < f; earlier++)
			{
				if (shape.faces[earlier] == shape.faces[f])
				{
					first = first_index[earlier];
					repeated = true;
					break;
				}
			}

			first_index.push_back(first);

			// minHalfAngle: 163 per radian (exact for 94.5% of the faces of clean stock hulls), 127 at most.
			put(faces, static_cast<std::uint16_t>(first));
			put(faces, static_cast<std::uint8_t>(shape.faces[f].size()));
			put(faces, static_cast<std::uint8_t>(std::min(static_cast<int>(163.0 * angles[f]), 127)));
			if (!repeated)
			{
				for (const auto v : shape.faces[f])
				{
					indices.push_back(static_cast<std::uint8_t>(v));
				}
			}
		}

		const auto edges = [](const std::vector<std::pair<std::uint32_t, std::uint32_t>>& list)
		{
			std::vector<std::uint8_t> out;
			for (const auto& [face, edge] : list)
			{
				put(out, static_cast<std::uint16_t>(face));
				put(out, static_cast<std::uint8_t>(edge));
				put(out, std::uint8_t{0});
			}

			return out;
		};

		const auto face_count = static_cast<std::uint32_t>(shape.faces.size());
		builder.point(object, "vertices", builder.array("hkVector4f", static_cast<std::uint32_t>(shape.vertices.size()), vertices));
		builder.point(object, "planes", builder.array("hkVector4", face_count, planes));
		builder.point(object, "faces", builder.array("hknpConvexPolytopeShape::Face", face_count, faces));
		builder.point(object, "indices", builder.array("hkUint8", static_cast<std::uint32_t>(indices.size()), indices));

		const auto links = builder.object("hknpConvexPolytopeShape::Connectivity");
		builder.point(object, "connectivity", links);
		builder.point(links, "vertexEdges", builder.array("hknpConvexPolytopeShape::Connectivity::Edge",
			static_cast<std::uint32_t>(connectivity.vertex_edges.size()), edges(connectivity.vertex_edges)));
		builder.point(links, "faceLinks", builder.array("hknpConvexPolytopeShape::Connectivity::Edge",
			static_cast<std::uint32_t>(connectivity.face_links.size()), edges(connectivity.face_links)));
		return object;
	}

	polytope read_convex_polytope_shape(const tagfile& file, const type_table& types, const std::uint32_t index)
	{
		const auto& shape = file.items.at(index);
		const auto offset_of = [&](const std::string_view name)
		{
			const auto found = types.member(shape.type, name);
			if (!found)
			{
				throw std::runtime_error(std::format("{} has no member {}", types.identity(shape.type), name));
			}

			return found->first;
		};

		const auto& vertices = target_of(file, shape, offset_of("vertices"));
		const auto& planes = target_of(file, shape, offset_of("planes"));
		const auto& faces = target_of(file, shape, offset_of("faces"));
		const auto& indices = target_of(file, shape, offset_of("indices"));

		polytope result;
		for (std::uint32_t i = 0; i < vertices.count; i++)
		{
			result.vertices.push_back({get<float>(vertices.data, i * 16), get<float>(vertices.data, i * 16 + 4), get<float>(vertices.data, i * 16 + 8)});
		}

		for (std::uint32_t f = 0; f < faces.count; f++)
		{
			result.planes.push_back({get<float>(planes.data, f * 16), get<float>(planes.data, f * 16 + 4), get<float>(planes.data, f * 16 + 8),
				get<float>(planes.data, f * 16 + 12)});
			const auto first = get<std::uint16_t>(faces.data, f * 4);
			const auto count = get<std::uint8_t>(faces.data, f * 4 + 2);
			std::vector<std::uint32_t> ring;
			for (std::uint32_t k = 0; k < count; k++)
			{
				ring.push_back(get<std::uint8_t>(indices.data, first + k));
			}

			result.faces.push_back(std::move(ring));
		}

		return result;
	}

	std::vector<vector3> placed_vertices(const compound_instance& instance)
	{
		if (instance.vertices.empty())
		{
			throw std::runtime_error("a shape instance has no vertices");
		}

		std::vector<vector3> placed;
		for (const auto& v : instance.vertices)
		{
			vector3 p{};
			for (std::size_t axis = 0; axis < 3; axis++)
			{
				p[axis] = instance.rotation[0][axis] * v[0] + instance.rotation[1][axis] * v[1] + instance.rotation[2][axis] * v[2] +
					instance.translation[axis];
			}

			placed.push_back(p);
		}

		return placed;
	}

	std::array<float, 6> instance_bounds(const compound_instance& instance)
	{
		auto box = empty_bounds;
		for (const auto& p : placed_vertices(instance))
		{
			include(box, {p[0], p[1], p[2], p[0], p[1], p[2]});
		}

		for (std::size_t axis = 0; axis < 3; axis++)
		{
			box[axis] -= instance.convex_radius;
			box[axis + 3] += instance.convex_radius;
		}

		return box;
	}

	std::vector<std::uint8_t> instance_data(const compound_instance& instance, const std::uint32_t translation_w)
	{
		const auto translated = std::ranges::any_of(instance.translation, [](const float value)
		{
			return value != 0.0f;
		});

		// The stock rotation flags start between 7e-5 and 4e-3 from the identity.
		auto rotated = false;
		for (std::size_t column = 0; column < 3; column++)
		{
			for (std::size_t axis = 0; axis < 3; axis++)
			{
				rotated |= std::abs(instance.rotation[column][axis] - (column == axis ? 1.0f : 0.0f)) > 1e-4f;
			}
		}

		// The w of column 0 holds the transform flags (0x2 translation, 0x4 rotation) in the bits of 0.5.
		std::vector<std::uint8_t> out;
		for (std::size_t column = 0; column < 3; column++)
		{
			for (const auto value : instance.rotation[column])
			{
				put(out, value);
			}

			put(out, column ? 0u : 0x3F000040u | (translated ? 0x2u : 0u) | (rotated ? 0x4u : 0u));
		}

		for (const auto value : instance.translation)
		{
			put(out, value);
		}

		put(out, translation_w);
		for (std::size_t axis = 0; axis < 4; axis++)
		{
			put(out, 1.0f);
		}

		// shape, shapeTag, destructionTag, then isEmpty, nextEmptyElement, instanceId and parentShape.
		put(out, std::uint64_t{});
		put(out, instance.shape_tag);
		put(out, std::uint16_t{0xFFFF});
		out.insert(out.end(), 20, 0);
		return out;
	}

	std::uint32_t add_compound_shape(object_builder& builder, const std::span<const compound_instance> instances)
	{
		if (instances.empty())
		{
			throw std::runtime_error("a compound shape has no instances");
		}

		// The bounds of each instance in the frame of the compound, and the radius of its vertices about their centre.
		const auto count = static_cast<std::uint32_t>(instances.size());
		std::vector<bounds> boxes;
		std::vector<double> radii;
		for (const auto& instance : instances)
		{
			const auto box = instance_bounds(instance);
			double radius{};
			for (const auto& p : placed_vertices(instance))
			{
				double square{};
				for (std::size_t axis = 0; axis < 3; axis++)
				{
					const auto d = static_cast<double>(p[axis]) - (static_cast<double>(box[axis]) + box[axis + 3]) / 2.0;
					square += d * d;
				}

				radius = std::max(radius, std::sqrt(square));
			}

			boxes.push_back(box);
			radii.push_back(radius + instance.convex_radius);
		}

		auto aabb = empty_bounds;
		for (const auto& box : boxes)
		{
			include(aabb, box);
		}

		// boundingRadius: the smaller of half the AABB diagonal and the largest reach of an instance sphere from the centre.
		double half_diagonal{};
		double reach{};
		for (std::size_t axis = 0; axis < 3; axis++)
		{
			const auto extent = (static_cast<double>(aabb[axis + 3]) - aabb[axis]) / 2.0;
			half_diagonal += extent * extent;
		}

		for (std::uint32_t i = 0; i < count; i++)
		{
			double square{};
			for (std::size_t axis = 0; axis < 3; axis++)
			{
				const auto d = (static_cast<double>(boxes[i][axis]) + boxes[i][axis + 3] - aabb[axis] - aabb[axis + 3]) / 2.0;
				square += d * d;
			}

			reach = std::max(reach, std::sqrt(square) + radii[i]);
		}

		// A tree of more than one leaf takes the instances in the order of their box centres along the axis where the centres
		// spread most.
		std::vector<vector3> points(count);
		auto spread = empty_bounds;
		for (std::uint32_t i = 0; i < count; i++)
		{
			for (std::size_t axis = 0; axis < 3; axis++)
			{
				points[i][axis] = boxes[i][axis] + boxes[i][axis + 3];
				spread[axis] = std::min(spread[axis], points[i][axis]);
				spread[axis + 3] = std::max(spread[axis + 3], points[i][axis]);
			}
		}

		std::size_t sort_axis{};
		for (std::size_t axis = 1; axis < 3; axis++)
		{
			if (spread[axis + 3] - spread[axis] > spread[sort_axis + 3] - spread[sort_axis])
			{
				sort_axis = axis;
			}
		}

		std::vector<std::uint32_t> order(count);
		std::iota(order.begin(), order.end(), 0u);
		if (count > 4)
		{
			std::ranges::stable_sort(order, {}, [&](const std::uint32_t i)
			{
				return points[i][sort_axis];
			});
		}

		std::vector<std::uint32_t> position(count);
		std::vector<std::uint8_t> point_data;
		for (std::uint32_t k = 0; k < count; k++)
		{
			position[order[k]] = k;
			for (const auto value : points[order[k]])
			{
				put(point_data, value);
			}

			// The instance index in the bits of w (0.5 + index).
			put(point_data, std::uint32_t{0x3F000000} | order[k]);
		}

		// The w of the translation holds the position of the instance in the tree points, in the bits of 0.5.
		std::vector<std::uint8_t> elements;
		std::uint32_t shape_keys{};
		for (std::uint32_t i = 0; i < count; i++)
		{
			std::ranges::copy(instance_data(instances[i], std::uint32_t{0x3F000000} | position[i]), std::back_inserter(elements));
			shape_keys += instances[i].shape_keys;
		}

		std::vector<std::uint8_t> aabb_data;
		for (std::size_t side = 0; side < 6; side += 3)
		{
			for (std::size_t axis = 0; axis < 3; axis++)
			{
				put(aabb_data, aabb[side + axis]);
			}

			put(aabb_data, 0.0f);
		}

		const auto object = builder.object("hknpCompoundShape");
		builder.set(object, "flags", std::uint16_t{0x4});
		builder.set(object, "type", std::uint8_t{12});
		builder.set(object, "numShapeKeyBits", static_cast<std::uint8_t>(std::bit_width(count)));
		builder.set(object, "dispatchType", std::uint8_t{3});
		builder.set(object, "shapeTagCodecInfo", std::uint32_t{0xFFFFFFFF});

		const auto elements_item = builder.array("hkFreeListArrayElement<hknpShapeInstance>", count, elements);
		builder.point(object, "instances.elements", elements_item);
		for (std::uint32_t i = 0; i < count; i++)
		{
			builder.point(elements_item, "shape", instances[i].shape, i);
		}

		builder.set(object, "instances.firstFree", std::int32_t{-1});
		builder.set(object, "instances.numAllocated", count);
		builder.set(object, "aabb", std::span<const std::uint8_t>(aabb_data));
		builder.set(object, "boundingRadius", static_cast<float>(std::min(half_diagonal > 0.0 ? std::sqrt(half_diagonal) : 0.0, reach)));
		builder.set(object, "estimatedNumShapeKeys", static_cast<std::int32_t>(shape_keys));

		const auto nodes = build_tree(boxes, order);
		builder.point(object, "boundingVolumeData.simdTree.nodes", builder.array("hkcdSimdTree::Node", static_cast<std::uint32_t>(nodes.size()),
			write_nodes(nodes)));
		builder.set(object, "boundingVolumeData.simdTree.isCompact", std::uint8_t{1});
		builder.point(object, "boundingVolumeData.simdTree.points", builder.array("hkVector4", count, point_data));
		builder.set(object, "boundingVolumeData.type", std::uint8_t{2});
		return object;
	}

	namespace
	{
		// hkPackedVector3: three 16-bit mantissas and the high half of a float with their shared exponent. Each value is the
		// mantissa times that float times 65536.
		std::array<std::int16_t, 4> pack_vector3(const vector3& value)
		{
			const auto bits = [](const float x)
			{
				return std::bit_cast<std::uint32_t>(x);
			};

			// The largest exponent of the values and a tiny one, rounded up a little.
			std::uint32_t exponent{};
			for (const auto x : {value[0], value[1], value[2], std::bit_cast<float>(0x28800000u)})
			{
				exponent = std::max(exponent, bits(x * std::bit_cast<float>(0x3F800089u)) & 0x7F800000);
			}

			std::array<std::int16_t, 4> packed{};
			for (std::size_t axis = 0; axis < 3; axis++)
			{
				// The value times 2^30 / 2^exponent, truncated, saturated and rounded to its high half.
				const auto scaled = std::bit_cast<float>(bits(value[axis]) + 0x4E800000u - exponent);
				const auto whole = scaled >= 2147483648.0f ? 0x7FFFFFFF : scaled < -2147483648.0f ? std::numeric_limits<std::int32_t>::min() :
					static_cast<std::int32_t>(scaled);
				packed[axis] = static_cast<std::int16_t>((static_cast<std::uint32_t>(whole) + 0x8000u) >> 16);
			}

			packed[3] = static_cast<std::int16_t>(static_cast<std::int32_t>(exponent - 0x4E800000u + 0x3F800000u) >> 16);
			return packed;
		}

		vector3 unpack_vector3(const std::array<std::int16_t, 4>& packed)
		{
			const auto scale = std::bit_cast<float>(static_cast<std::uint32_t>(static_cast<std::uint16_t>(packed[3])) << 16) * 65536.0f;
			return {packed[0] * scale, packed[1] * scale, packed[2] * scale};
		}

		// A unit quaternion in four shorts: the high half of q * 30000 * 65536 with the sign bit flipped.
		std::array<std::int16_t, 4> pack_rotation(const std::array<float, 4>& rotation)
		{
			std::array<std::int16_t, 4> packed{};
			for (std::size_t i = 0; i < 4; i++)
			{
				const auto whole = static_cast<std::int32_t>(rotation[i] * 1966080000.0f);
				packed[i] = static_cast<std::int16_t>((static_cast<std::uint32_t>(whole) + 0x80000000u) >> 16);
			}

			return packed;
		}

		std::array<float, 4> unpack_rotation(const std::array<std::int16_t, 4>& packed)
		{
			std::array<float, 4> rotation{};
			for (std::size_t i = 0; i < 4; i++)
			{
				rotation[i] = static_cast<std::int16_t>(static_cast<std::uint16_t>(packed[i]) ^ 0x8000) / 30000.0f;
			}

			return rotation;
		}

		using matrix3 = std::array<std::array<double, 3>, 3>;

		// The eigenvalues and eigenvectors (columns) of a symmetric matrix, by Jacobi rotations.
		std::pair<std::array<double, 3>, matrix3> diagonalize(matrix3 a)
		{
			matrix3 v{{{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}}};
			for (auto sweep = 0; sweep < 64; sweep++)
			{
				const auto off = std::abs(a[0][1]) + std::abs(a[0][2]) + std::abs(a[1][2]);
				const auto scale = std::abs(a[0][0]) + std::abs(a[1][1]) + std::abs(a[2][2]);
				if (off <= 1e-15 * scale || off == 0.0)
				{
					break;
				}

				for (const auto [p, q] : {std::pair(0, 1), std::pair(0, 2), std::pair(1, 2)})
				{
					if (a[p][q] == 0.0)
					{
						continue;
					}

					const auto theta = (a[q][q] - a[p][p]) / (2.0 * a[p][q]);
					const auto t = (theta >= 0.0 ? 1.0 : -1.0) / (std::abs(theta) + std::sqrt(theta * theta + 1.0));
					const auto c = 1.0 / std::sqrt(t * t + 1.0);
					const auto s = t * c;
					for (auto k = 0; k < 3; k++)
					{
						const auto akp = a[k][p];
						const auto akq = a[k][q];
						a[k][p] = c * akp - s * akq;
						a[k][q] = s * akp + c * akq;
					}

					for (auto k = 0; k < 3; k++)
					{
						const auto apk = a[p][k];
						const auto aqk = a[q][k];
						a[p][k] = c * apk - s * aqk;
						a[q][k] = s * apk + c * aqk;
					}

					for (auto k = 0; k < 3; k++)
					{
						const auto vkp = v[k][p];
						const auto vkq = v[k][q];
						v[k][p] = c * vkp - s * vkq;
						v[k][q] = s * vkp + c * vkq;
					}
				}
			}

			return {{a[0][0], a[1][1], a[2][2]}, v};
		}

		// The unit quaternion of a rotation matrix (columns are the rotated axes).
		std::array<float, 4> quaternion_of(const matrix3& r)
		{
			std::array<double, 4> q{};
			const auto trace = r[0][0] + r[1][1] + r[2][2];
			if (trace > 0.0)
			{
				const auto s = std::sqrt(trace + 1.0) * 2.0;
				q = {(r[2][1] - r[1][2]) / s, (r[0][2] - r[2][0]) / s, (r[1][0] - r[0][1]) / s, 0.25 * s};
			}
			else if (r[0][0] > r[1][1] && r[0][0] > r[2][2])
			{
				const auto s = std::sqrt(1.0 + r[0][0] - r[1][1] - r[2][2]) * 2.0;
				q = {0.25 * s, (r[0][1] + r[1][0]) / s, (r[0][2] + r[2][0]) / s, (r[2][1] - r[1][2]) / s};
			}
			else if (r[1][1] > r[2][2])
			{
				const auto s = std::sqrt(1.0 + r[1][1] - r[0][0] - r[2][2]) * 2.0;
				q = {(r[0][1] + r[1][0]) / s, 0.25 * s, (r[1][2] + r[2][1]) / s, (r[0][2] - r[2][0]) / s};
			}
			else
			{
				const auto s = std::sqrt(1.0 + r[2][2] - r[0][0] - r[1][1]) * 2.0;
				q = {(r[0][2] + r[2][0]) / s, (r[1][2] + r[2][1]) / s, 0.25 * s, (r[1][0] - r[0][1]) / s};
			}

			const auto sign = q[3] < 0.0 ? -1.0 : 1.0;
			const auto length = std::sqrt(q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
			return {static_cast<float>(sign * q[0] / length), static_cast<float>(sign * q[1] / length), static_cast<float>(sign * q[2] / length),
				static_cast<float>(sign * q[3] / length)};
		}
	}

	mass_properties convex_mass_properties(const polytope& shape, const float mass, const float inertia_factor)
	{
		if (shape.vertices.empty())
		{
			throw std::runtime_error("a convex shape without vertices has no mass properties");
		}

		// Tetrahedra from a point inside to each face triangle.
		std::array<double, 3> origin{};
		for (const auto& v : shape.vertices)
		{
			for (std::size_t axis = 0; axis < 3; axis++)
			{
				origin[axis] += v[axis] / static_cast<double>(shape.vertices.size());
			}
		}

		double volume{};
		std::array<double, 3> centre{};
		matrix3 second{};
		for (const auto& face : shape.faces)
		{
			const auto point = [&](const std::uint32_t index)
			{
				const auto& v = shape.vertices.at(index);
				return std::array<double, 3>{v[0] - origin[0], v[1] - origin[1], v[2] - origin[2]};
			};

			for (std::size_t k = 1; k + 1 < face.size(); k++)
			{
				const auto a = point(face[0]);
				const auto b = point(face[k]);
				const auto c = point(face[k + 1]);
				const auto d = (a[0] * (b[1] * c[2] - b[2] * c[1]) + a[1] * (b[2] * c[0] - b[0] * c[2]) + a[2] * (b[0] * c[1] - b[1] * c[0])) / 6.0;
				volume += d;
				const std::array<double, 3> sum{a[0] + b[0] + c[0], a[1] + b[1] + c[1], a[2] + b[2] + c[2]};
				for (std::size_t i = 0; i < 3; i++)
				{
					centre[i] += d * sum[i] / 4.0;
					for (std::size_t j = 0; j < 3; j++)
					{
						second[i][j] += d / 20.0 * (a[i] * a[j] + b[i] * b[j] + c[i] * c[j] + sum[i] * sum[j]);
					}
				}
			}
		}

		if (!(volume > 0.0))
		{
			throw std::runtime_error("a convex shape has no volume");
		}

		mass_properties result;
		result.volume = static_cast<float>(volume);
		result.mass = mass;
		for (std::size_t i = 0; i < 3; i++)
		{
			centre[i] /= volume;
		}

		// The inertia about the centre, for the mass.
		matrix3 inertia{};
		const auto density = mass / volume;
		const auto trace = second[0][0] + second[1][1] + second[2][2] - volume * (centre[0] * centre[0] + centre[1] * centre[1] + centre[2] * centre[2]);
		for (std::size_t i = 0; i < 3; i++)
		{
			for (std::size_t j = 0; j < 3; j++)
			{
				const auto covariance = second[i][j] - volume * centre[i] * centre[j];
				inertia[i][j] = density * inertia_factor * ((i == j ? trace : 0.0) - covariance);
			}
		}

		auto [moments, axes] = diagonalize(inertia);
		const auto determinant = axes[0][0] * (axes[1][1] * axes[2][2] - axes[1][2] * axes[2][1]) -
			axes[0][1] * (axes[1][0] * axes[2][2] - axes[1][2] * axes[2][0]) + axes[0][2] * (axes[1][0] * axes[2][1] - axes[1][1] * axes[2][0]);
		if (determinant < 0.0)
		{
			for (auto& row : axes)
			{
				row[2] = -row[2];
			}
		}

		for (std::size_t i = 0; i < 3; i++)
		{
			result.centre[i] = static_cast<float>(centre[i] + origin[i]);
			result.inertia[i] = static_cast<float>(moments[i]);
		}

		result.rotation = quaternion_of(axes);
		return result;
	}

	void add_mass_properties(object_builder& builder, const std::uint32_t shape, const mass_properties& properties)
	{
		const auto values = builder.object("hknpShapeMassProperties");
		builder.set(values, "compressedMassProperties.centerOfMass", pack_vector3(properties.centre));
		builder.set(values, "compressedMassProperties.inertia", pack_vector3(properties.inertia));
		builder.set(values, "compressedMassProperties.majorAxisSpace", pack_rotation(properties.rotation));
		builder.set(values, "compressedMassProperties.mass", properties.mass);
		builder.set(values, "compressedMassProperties.volume", properties.volume);

		// hknpShape keeps its mass properties under the key 0xF100.
		std::vector<std::uint8_t> entry(16);
		const std::uint16_t key = 0xF100;
		std::memcpy(entry.data() + 8, &key, sizeof(key));
		const auto entries = builder.array("hkRefCountedProperties::Entry", 1, entry);
		builder.point(entries, "object", values);

		const auto list = builder.object("hkRefCountedProperties");
		builder.point(list, "entries", entries);
		builder.point(shape, "properties", list);
	}

	std::optional<mass_properties> read_mass_properties(const tagfile& file, const type_table& types, const std::uint32_t index)
	{
		const auto& shape = file.items.at(index);
		const auto offset_of = [&](const std::uint32_t type, const std::string_view name)
		{
			const auto found = types.member(type, name);
			if (!found)
			{
				throw std::runtime_error(std::format("{} has no member {}", types.identity(type), name));
			}

			return found->first;
		};

		const auto pointer = [&](const item& owner, const std::uint32_t offset) -> const item*
		{
			const auto found = std::ranges::find(owner.pointers, offset, &pointer_field::offset);
			return found != owner.pointers.end() && found->target ? &file.items.at(found->target) : nullptr;
		};

		const auto* list = pointer(shape, offset_of(shape.type, "properties"));
		const auto* entries = list ? pointer(*list, offset_of(list->type, "entries")) : nullptr;
		for (std::uint32_t i = 0; entries && i < entries->count; i++)
		{
			const auto* object = pointer(*entries, i * 16);
			if (!object || get<std::uint16_t>(entries->data, i * 16 + 8) != 0xF100 || types.identity(object->type) != "hknpShapeMassProperties")
			{
				continue;
			}

			const auto packed = [&](const std::string_view name)
			{
				std::array<std::int16_t, 4> values{};
				std::memcpy(values.data(), object->data.data() + offset_of(object->type, name), sizeof(values));
				return values;
			};

			mass_properties result;
			result.centre = unpack_vector3(packed("compressedMassProperties.centerOfMass"));
			result.inertia = unpack_vector3(packed("compressedMassProperties.inertia"));
			result.rotation = unpack_rotation(packed("compressedMassProperties.majorAxisSpace"));
			result.mass = get<float>(object->data, offset_of(object->type, "compressedMassProperties.mass"));
			result.volume = get<float>(object->data, offset_of(object->type, "compressedMassProperties.volume"));
			return result;
		}

		return {};
	}

	std::uint32_t copy_item(object_builder& builder, const tagfile& file, const std::uint32_t index,
		std::unordered_map<std::uint32_t, std::uint32_t>& copies)
	{
		if (index == 0)
		{
			return 0;
		}

		if (const auto found = copies.find(index); found != copies.end())
		{
			return found->second;
		}

		const auto& source = file.items.at(index);
		const auto copy = builder.raw(source.type, source.flags, source.count, source.data);
		copies.emplace(index, copy);
		for (const auto& pointer : source.pointers)
		{
			builder.pointer(copy, pointer.offset, pointer.pointer_type, copy_item(builder, file, pointer.target, copies));
		}

		return copy;
	}
}
