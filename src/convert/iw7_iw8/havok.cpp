#include <std_include.hpp>
#include "export.hpp"

namespace zonetool::convert::iw7_iw8
{
	namespace
	{
		// Names each form of Havok data that does not convert, once.
		void report_unknown(const std::string& form)
		{
			static std::mutex mutex;
			static std::set<std::string> reported;
			const std::lock_guard lock(mutex);
			if (reported.insert(form).second)
			{
				ZONETOOL_INFO("iw7: Havok data of a form that does not convert: %s", form.data());
			}
		}

		using vec3 = std::array<float, 3>;

		// The game units of a Havok unit.
		constexpr auto havok_unit = 32.0f;

		template <typename T>
		T read(const std::string_view bytes, const std::size_t offset)
		{
			if (offset > bytes.size() || bytes.size() - offset < sizeof(T))
			{
				throw std::runtime_error("a Havok packfile read is outside the file");
			}

			T value{};
			std::memcpy(&value, bytes.data() + offset, sizeof(T));
			return value;
		}

		// A Havok binary packfile of file version 11 in the native layout of IW7 (8-byte pointers, little endian): its
		// data section, the pointers that its local and global fixups place in it, and the class of each object that its
		// virtual fixups name. Offsets count from the start of the data section.
		class packfile
		{
		public:
			struct block
			{
				std::size_t data{};
				std::uint32_t count{};
			};

			packfile(const char* data, const std::size_t size)
			{
				const std::string_view file(data, size);
				if (size < 0x40 || read<std::uint32_t>(file, 0) != 0x57E0E057 || read<std::uint32_t>(file, 4) != 0x10C0C010)
				{
					throw std::runtime_error("the Havok data is not a packfile");
				}

				if (read<std::int32_t>(file, 0xC) != 11 || read<std::uint8_t>(file, 0x10) != 8)
				{
					throw std::runtime_error("the Havok packfile is not of version 11 with 8-byte pointers");
				}

				const auto section_count = read<std::int32_t>(file, 0x14);
				const auto contents = read<std::int32_t>(file, 0x18);
				const auto names = read<std::int32_t>(file, 0x20);
				const auto predicates = read<std::int16_t>(file, 0x3C);
				const auto predicate_size = read<std::int16_t>(file, 0x3E);
				const std::size_t headers = 0x40 + (predicates != -1 ? predicate_size : 0);
				if (contents < 0 || contents >= section_count || names < 0 || names >= section_count)
				{
					throw std::runtime_error("the Havok packfile has no data section");
				}

				// A section header: its tag, then the offsets of its data and of its local, global and virtual fixups, its
				// exports and imports, and its end.
				struct section
				{
					std::size_t start{};
					std::size_t local{};
					std::size_t global{};
					std::size_t virtual_fixups{};
					std::size_t exports{};
				};

				const auto section_at = [&](const std::int32_t index)
				{
					const auto base = headers + static_cast<std::size_t>(index) * 0x40 + 0x14;
					section result{};
					result.start = read<std::uint32_t>(file, base);
					result.local = result.start + read<std::uint32_t>(file, base + 4);
					result.global = result.start + read<std::uint32_t>(file, base + 8);
					result.virtual_fixups = result.start + read<std::uint32_t>(file, base + 12);
					result.exports = result.start + read<std::uint32_t>(file, base + 16);
					if (result.start > result.local || result.local > result.global || result.global > result.virtual_fixups ||
						result.virtual_fixups > result.exports || result.exports > size)
					{
						throw std::runtime_error("a Havok packfile section is outside the file");
					}

					return result;
				};

				// The class names: (signature, 0x09, name) records, which the virtual fixups name by the offset of the name.
				std::unordered_map<std::size_t, std::string> class_names;
				const auto name_section = section_at(names);
				for (auto offset = name_section.start; offset + 5 < name_section.local;)
				{
					if (read<std::uint8_t>(file, offset + 4) != 0x09)
					{
						break;
					}

					const auto end = file.find('\0', offset + 5);
					if (end == std::string_view::npos || end >= name_section.local)
					{
						break;
					}

					class_names.emplace(offset + 5 - name_section.start, std::string(file.substr(offset + 5, end - offset - 5)));
					offset = end + 1;
				}

				const auto data_section = section_at(contents);
				this->data_ = file.substr(data_section.start, data_section.local - data_section.start);
				for (auto offset = data_section.local; offset + 8 <= data_section.global; offset += 8)
				{
					const auto source = read<std::int32_t>(file, offset);
					if (source != -1)
					{
						this->pointers_[source] = read<std::uint32_t>(file, offset + 4);
					}
				}

				for (auto offset = data_section.global; offset + 12 <= data_section.virtual_fixups; offset += 12)
				{
					const auto source = read<std::int32_t>(file, offset);
					if (source != -1 && read<std::int32_t>(file, offset + 4) == contents)
					{
						this->pointers_[source] = read<std::uint32_t>(file, offset + 8);
					}
				}

				for (auto offset = data_section.virtual_fixups; offset + 12 <= data_section.exports; offset += 12)
				{
					const auto object = read<std::int32_t>(file, offset);
					const auto name = class_names.find(read<std::uint32_t>(file, offset + 8));
					if (object != -1 && name != class_names.end())
					{
						this->classes_[object] = name->second;
					}
				}
			}

			template <typename T>
			T get(const std::size_t offset) const
			{
				return read<T>(this->data_, offset);
			}

			std::optional<std::size_t> target(const std::size_t offset) const
			{
				const auto found = this->pointers_.find(offset);
				return found != this->pointers_.end() ? std::optional<std::size_t>(found->second) : std::nullopt;
			}

			// An hkArray: its pointer and size.
			block array(const std::size_t offset) const
			{
				const auto count = this->get<std::uint32_t>(offset + 8);
				const auto data = this->target(offset);
				if (count && !data)
				{
					throw std::runtime_error("a Havok array has elements but no data");
				}

				return {data.value_or(0), count};
			}

			std::string_view class_of(const std::size_t object) const
			{
				const auto found = this->classes_.find(object);
				return found != this->classes_.end() ? std::string_view(found->second) : std::string_view{};
			}

			std::optional<std::size_t> first(const std::string_view name) const
			{
				for (const auto& [object, class_name] : this->classes_)
				{
					if (class_name == name)
					{
						return object;
					}
				}

				return std::nullopt;
			}

		private:
			std::string_view data_;
			std::unordered_map<std::size_t, std::size_t> pointers_;
			std::map<std::size_t, std::string> classes_;
		};

		// A primitive that the tools removed.
		constexpr std::array<std::uint8_t, 4> removed_primitive{0xDE, 0xAD, 0xDE, 0xAD};

		// The hkcdStaticMeshTree of an hknpCompressedMeshShapeData (2014): sections of up to 255 primitives (triangles,
		// quads and custom primitives) over vertices on a grid of 21, 21 and 22 bits in the bounds of the mesh (shared
		// vertices, which a section names through its part of the shared vertex table) or of 11, 11 and 10 bits in the
		// bounds of the section (packed vertices). A section reaches the shared vertices of its page (65536 each). A
		// custom primitive (a, b, b, b) is a convex piece: entry a of the section table is its vertex count << 8 | 2 with
		// the format of its points, the next entry its first shared vertex.
		void read_compressed_mesh(const packfile& file, const std::size_t data, havok_shape& shape, havok_shapes& shapes)
		{
			const auto tree = data + 0x10;
			vec3 low{};
			vec3 scale{};
			for (std::size_t axis = 0; axis < 3; axis++)
			{
				low[axis] = file.get<float>(tree + 0x10 + axis * 4);
				const auto bits = axis < 2 ? 21u : 22u;
				scale[axis] = (file.get<float>(tree + 0x20 + axis * 4) - low[axis]) / static_cast<float>((1u << bits) - 1);
			}

			const auto sections = file.array(tree + 0x40);
			const auto primitives = file.array(tree + 0x50);
			const auto table = file.array(tree + 0x60);
			const auto packed = file.array(tree + 0x70);
			const auto shared = file.array(tree + 0x80);
			const auto runs = file.array(tree + 0x90);
			const auto shared_vertex = [&](const std::uint32_t index) -> vec3
			{
				if (index >= shared.count)
				{
					throw std::runtime_error("a Havok mesh primitive names a vertex that the mesh does not have");
				}

				const auto word = file.get<std::uint64_t>(shared.data + index * std::size_t{8});
				return {low[0] + static_cast<float>(word & 0x1FFFFF) * scale[0], low[1] + static_cast<float>(word >> 21 & 0x1FFFFF) * scale[1],
					low[2] + static_cast<float>(word >> 42) * scale[2]};
			};

			const auto step = std::max({scale[0], scale[1], scale[2]});
			for (std::uint32_t s = 0; s < sections.count; s++)
			{
				const auto base = sections.data + s * std::size_t{96};
				std::array<float, 6> codec{};
				std::array<float, 6> domain{};
				for (std::size_t i = 0; i < 3; i++)
				{
					domain[i] = file.get<float>(base + 0x10 + i * 4);
					domain[i + 3] = file.get<float>(base + 0x20 + i * 4);
				}

				for (std::size_t i = 0; i < 6; i++)
				{
					codec[i] = file.get<float>(base + 0x30 + i * 4);
				}

				const auto first_packed = file.get<std::uint32_t>(base + 0x48);
				const auto first_shared = file.get<std::uint32_t>(base + 0x4C) >> 8;
				const auto first_primitive = file.get<std::uint32_t>(base + 0x50) >> 8;
				const auto primitive_count = file.get<std::uint32_t>(base + 0x50) & 0xFF;
				const auto first_run = file.get<std::uint32_t>(base + 0x54) >> 8;
				const auto run_count = file.get<std::uint32_t>(base + 0x54) & 0xFF;
				const std::uint32_t packed_count = file.get<std::uint8_t>(base + 0x58);
				const auto page = static_cast<std::uint32_t>(file.get<std::uint8_t>(base + 0x5C)) << 16;
				const auto table_entry = [&](const std::uint32_t local) -> std::uint32_t
				{
					const auto index = first_shared + local - packed_count;
					if (local < packed_count || index >= table.count)
					{
						throw std::runtime_error("a Havok mesh section names an entry that its vertex table does not have");
					}

					return file.get<std::uint16_t>(table.data + index * std::size_t{2});
				};

				const auto vertex = [&](const std::uint32_t local) -> vec3
				{
					if (local >= packed_count)
					{
						return shared_vertex(page | table_entry(local));
					}

					if (first_packed + local >= packed.count)
					{
						throw std::runtime_error("a Havok mesh section names a packed vertex that the mesh does not have");
					}

					const auto word = file.get<std::uint32_t>(packed.data + (first_packed + local) * std::size_t{4});
					return {codec[0] + static_cast<float>(word & 0x7FF) * codec[3], codec[1] + static_cast<float>(word >> 11 & 0x7FF) * codec[4],
						codec[2] + static_cast<float>(word >> 22) * codec[5]};
				};

				// A vertex lies in the bounds of its section, within a step of the grids.
				auto tolerance = step + 0.01f;
				if (packed_count)
				{
					tolerance += std::max({codec[3], codec[4], codec[5]});
				}

				const auto check = [&](const vec3& point)
				{
					for (std::size_t axis = 0; axis < 3; axis++)
					{
						if (point[axis] < domain[axis] - tolerance || point[axis] > domain[axis + 3] + tolerance)
						{
							shapes.misplaced++;
							return;
						}
					}
				};

				// Each data run gives the shape tag of a range of the primitives of the section.
				std::vector<std::uint16_t> tags(primitive_count, 0xFFFF);
				for (std::uint32_t r = 0; r < run_count; r++)
				{
					const auto run = runs.data + (first_run + r) * std::size_t{4};
					if (first_run + r >= runs.count)
					{
						throw std::runtime_error("a Havok mesh section names a data run that the mesh does not have");
					}

					const auto value = file.get<std::uint16_t>(run);
					const std::uint32_t first = file.get<std::uint8_t>(run + 2);
					const std::uint32_t count = file.get<std::uint8_t>(run + 3);
					for (auto k = first; k < first + count && k < primitive_count; k++)
					{
						tags[k] = value;
					}
				}

				for (std::uint32_t k = 0; k < primitive_count; k++)
				{
					if (first_primitive + k >= primitives.count)
					{
						throw std::runtime_error("a Havok mesh section names a primitive that the mesh does not have");
					}

					std::array<std::uint8_t, 4> indices{};
					for (std::size_t i = 0; i < 4; i++)
					{
						indices[i] = file.get<std::uint8_t>(primitives.data + (first_primitive + k) * std::size_t{4} + i);
					}

					if (indices == removed_primitive)
					{
						continue;
					}

					if (indices[1] == indices[2] && indices[2] == indices[3])
					{
						// A convex piece: entry a is its vertex count << 8 | 2, with the format of its points in bits 4 and 5;
						// entry a + 1 its first shared vertex. 0: consecutive shared vertices. 0x10: 32-bit vertices in the
						// bounds of the section (11, 11 and 10 bits, two to a shared vertex: Afghan's sky walls, Rally's clip).
						// 0x20: 16-bit vertices in the bounds of the section (5, 5 and 6 bits, four to a shared vertex: the sky
						// boxes of Frontier, Mansion and Parkour). The sections of these pieces have no packed vertices and no
						// codec (FLT_MAX, -inf). Verified from the sky pieces, whose points are the corners of their sections;
						// the scale between the corners is inferred.
						const auto header = table_entry(indices[0]);
						const auto type = header & 0xFF;
						const auto format = type & 0x30;
						if ((type & 0xCF) != 2 || format == 0x30)
						{
							report_unknown(std::format("compressed mesh primitive of type {}", type));
							shapes.unknown++;
							continue;
						}

						const auto first = page | table_entry(indices[0] + 1u);
						const auto count = header >> 8;
						const auto per_word = format == 0x20 ? 4u : format == 0x10 ? 2u : 1u;
						if (first + (count + per_word - 1) / per_word > shared.count)
						{
							throw std::runtime_error("a Havok convex piece names vertices that the mesh does not have");
						}

						havok_convex convex;
						convex.tag = tags[k];
						for (auto v = 0u; v < count; v++)
						{
							if (!format)
							{
								convex.points.push_back(shared_vertex(first + v));
							}
							else
							{
								const auto word = file.get<std::uint64_t>(shared.data + (first + v / per_word) * std::size_t{8}) >>
									(64 / per_word * (v % per_word));
								const auto wide = format == 0x10;
								const std::array<float, 3> q{
									static_cast<float>(wide ? word & 0x7FF : word & 0x1F) / (wide ? 2047.0f : 31.0f),
									static_cast<float>(wide ? word >> 11 & 0x7FF : word >> 5 & 0x1F) / (wide ? 2047.0f : 31.0f),
									static_cast<float>(wide ? word >> 22 & 0x3FF : word >> 10 & 0x3F) / (wide ? 1023.0f : 63.0f)};
								convex.points.push_back({domain[0] + q[0] * (domain[3] - domain[0]),
									domain[1] + q[1] * (domain[4] - domain[1]), domain[2] + q[2] * (domain[5] - domain[2])});
							}

							check(convex.points.back());
						}

						shape.convexes.push_back(std::move(convex));
						continue;
					}

					// A quad (a, b, c, d) holds the triangles (a, b, c) and (a, c, d); a triangle repeats c.
					for (auto half = 0u; half < (indices[2] != indices[3] ? 2u : 1u); half++)
					{
						havok_triangle triangle;
						triangle.tag = tags[k];
						const std::array<std::uint8_t, 3> corners{indices[0], indices[half + 1], indices[half + 2]};
						for (std::size_t c = 0; c < 3; c++)
						{
							triangle.corners[c] = vertex(corners[c]);
							check(triangle.corners[c]);
						}

						shape.triangles.push_back(triangle);
					}
				}
			}
		}

		// The convex pieces of an hknpDynamicCompoundShape (2014): its instances (128 bytes each) place convex shapes
		// with a transform (rotation columns, translation), a scale and a shape tag.
		void read_compound(const packfile& file, const std::size_t compound, havok_shape& shape, havok_shapes& shapes)
		{
			const auto instances = file.array(compound + 0x60);
			for (std::uint32_t i = 0; i < instances.count; i++)
			{
				const auto base = instances.data + i * std::size_t{0x80};
				// An empty element of the free list has no shape.
				const auto child = file.target(base + 0x50);
				if (!child)
				{
					continue;
				}

				const auto child_class = file.class_of(*child);
				if (child_class != "hknpConvexPolytopeShape" && child_class != "hknpConvexShape")
				{
					report_unknown(std::format("compound child {}", child_class));
					shapes.unknown++;
					continue;
				}

				std::array<vec3, 4> transform{};
				vec3 scale{};
				for (std::size_t axis = 0; axis < 3; axis++)
				{
					for (std::size_t column = 0; column < 4; column++)
					{
						transform[column][axis] = file.get<float>(base + column * 16 + axis * 4);
					}

					scale[axis] = file.get<float>(base + 0x40 + axis * 4);
				}

				havok_convex convex;
				convex.tag = file.get<std::uint16_t>(base + 0x58);
				// hknpConvexShape::m_vertices, an hkRelArray: the count, then the offset of the vertices from the array.
				const auto count = file.get<std::uint16_t>(*child + 0x30);
				const auto vertices = *child + 0x30 + file.get<std::uint16_t>(*child + 0x32);
				for (auto v = 0u; v < count; v++)
				{
					vec3 local{};
					for (std::size_t axis = 0; axis < 3; axis++)
					{
						local[axis] = file.get<float>(vertices + v * std::size_t{16} + axis * 4) * scale[axis];
					}

					vec3 point = transform[3];
					for (std::size_t axis = 0; axis < 3; axis++)
					{
						for (std::size_t column = 0; column < 3; column++)
						{
							point[axis] += transform[column][axis] * local[column];
						}
					}

					convex.points.push_back(point);
				}

				shape.convexes.push_back(std::move(convex));
			}
		}
	}

	havok_shapes read_havok_shapes(const char* data, const std::size_t size)
	{
		const packfile file(data, size);
		const auto list = file.first("HavokPhysicsShapeList");
		if (!list)
		{
			throw std::runtime_error("the Havok packfile has no shape list");
		}

		// HavokPhysicsShapeList: shapes (+0x00) and the shape tags (+0x68) of 24 bytes: contents, the CRC of the
		// physics material name, the material id, then the user data: class << 48 | glass id << 32 | surface type << 19
		// | surface flags.
		havok_shapes result;
		const auto tags = file.array(*list + 0x68);
		for (std::uint32_t i = 0; i < tags.count; i++)
		{
			const auto base = tags.data + i * std::size_t{24};
			const auto user_data = file.get<std::uint64_t>(base + 16);
			result.tags.push_back({file.get<std::uint32_t>(base), static_cast<std::uint32_t>(user_data >> 19 & 0x1FFF),
				static_cast<std::uint32_t>(user_data & 0x7FFFF), static_cast<std::uint16_t>(user_data >> 32)});
		}

		const auto shapes = file.array(*list);
		result.shapes.resize(shapes.count);
		for (std::uint32_t i = 0; i < shapes.count; i++)
		{
			const auto shape = file.target(shapes.data + i * std::size_t{8});
			if (!shape)
			{
				continue;
			}

			const auto shape_class = file.class_of(*shape);
			if (shape_class == "hknpCompressedMeshShape")
			{
				const auto mesh = file.target(*shape + 0x60);
				if (!mesh || file.class_of(*mesh) != "hknpCompressedMeshShapeData")
				{
					throw std::runtime_error("a Havok compressed mesh has no data");
				}

				read_compressed_mesh(file, *mesh, result.shapes[i], result);
			}
			else if (shape_class == "hknpDynamicCompoundShape")
			{
				read_compound(file, *shape, result.shapes[i], result);
			}
			else
			{
				report_unknown(std::string(shape_class));
				result.unknown++;
			}
		}

		// IW7 Havok keeps positions in units of 32 game units, as Replay's does.
		for (auto& shape : result.shapes)
		{
			for (auto& convex : shape.convexes)
			{
				for (auto& point : convex.points)
				{
					for (auto& value : point)
					{
						value *= havok_unit;
					}
				}
			}

			for (auto& triangle : shape.triangles)
			{
				for (auto& corner : triangle.corners)
				{
					for (auto& value : corner)
					{
						value *= havok_unit;
					}
				}
			}
		}

		return result;
	}
}
