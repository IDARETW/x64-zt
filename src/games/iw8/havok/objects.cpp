#include <std_include.hpp>
#include "objects.hpp"

namespace zonetool::iw8::havok
{
	namespace
	{
		// hkHalf16: the high 16 bits of a float, rounded.
		std::uint16_t to_half16(const float value)
		{
			std::uint32_t bits{};
			std::memcpy(&bits, &value, sizeof(bits));
			return static_cast<std::uint16_t>((bits + 0x8000) >> 16);
		}

		template <typename T>
		void put(std::vector<std::uint8_t>& out, const T& value)
		{
			const auto bytes = reinterpret_cast<const std::uint8_t*>(&value);
			out.insert(out.end(), bytes, bytes + sizeof(T));
		}

		template <typename T>
		T get(const std::vector<std::uint8_t>& data, const std::size_t offset)
		{
			if (offset + sizeof(T) > data.size())
			{
				throw std::runtime_error("a Havok object is shorter than its members");
			}

			T value{};
			std::memcpy(&value, data.data() + offset, sizeof(T));
			return value;
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

	tagfile object_builder::finish(const std::string_view sdk_version, const std::optional<std::uint64_t> compendium)
	{
		tagfile file;
		file.sdk_version = sdk_version;
		file.compendium = compendium;
		file.items = std::move(this->items_);
		for (auto& entry : file.items)
		{
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
}
