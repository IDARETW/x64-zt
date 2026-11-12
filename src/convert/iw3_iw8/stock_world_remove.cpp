#include <std_include.hpp>
#include <iw8/iw8.hpp>
#include "stock_world_remove.h"

#include <iw8/havok/mesh.hpp>
#include <iw8/common/replay_render.h>

#include <algorithm>
#include <bit>
#include <functional>
#include <cmath>
#include <cstring>
#include <format>
#include <map>
#include <optional>
#include <stdexcept>
#include <unordered_map>

namespace iw8::stockworld
{
	namespace
	{
		namespace hk = zonetool::iw8::havok;

		// Havok units: 32 game units each (research 90).
		constexpr float kHavokScale = 32.0f;

		template <typename T>
		T value_at(const hk::item& item, const std::size_t offset)
		{
			if (offset + sizeof(T) > item.data.size())
			{
				throw std::runtime_error("a Havok item is shorter than its members");
			}
			T value{};
			std::memcpy(&value, item.data.data() + offset, sizeof(T));
			return value;
		}

		std::uint32_t target_at(const hk::item& item, const std::uint32_t offset)
		{
			const auto found = std::ranges::find(item.pointers, offset, &hk::pointer_field::offset);
			return found != item.pointers.end() ? found->target : 0;
		}

		std::uint32_t member_offset(const hk::type_table& types, const std::uint32_t type, const std::string_view name)
		{
			const auto found = types.member(type, name);
			if (!found)
			{
				throw std::runtime_error(std::format("{} has no member {}", types.identity(type), name));
			}
			return found->first;
		}

		std::string item_string(const hk::tagfile& file, const std::uint32_t index)
		{
			if (!index)
			{
				return {};
			}
			const auto& text = file.items.at(index).data;
			return std::string(reinterpret_cast<const char*>(text.data()), strnlen(reinterpret_cast<const char*>(text.data()), text.size()));
		}

		struct ShapeList
		{
			const hk::tagfile& file;
			const hk::type_table& types;
			const hk::item& root;

			const hk::item* array(const std::string_view name) const
			{
				const auto target = target_at(root, member_offset(types, root.type, name));
				return target ? &file.items.at(target) : nullptr;
			}

			std::uint32_t value(const std::string_view name, const std::uint32_t index) const
			{
				const auto* list = array(name);
				return list && index < list->count ? value_at<std::uint32_t>(*list, index * 4) : 0u;
			}
		};

		bool TriangleInside(std::span<const Box> boxes, const std::array<hk::vector3, 3>& triangle)
		{
			// One box must hold all three corners.
			for (const auto& box : boxes)
			{
				if (Inside(box, triangle[0].data(), kHavokScale) && Inside(box, triangle[1].data(), kHavokScale) &&
					Inside(box, triangle[2].data(), kHavokScale))
				{
					return true;
				}
			}
			return false;
		}

		bool InstanceInside(std::span<const Box> boxes, const hk::compound_instance& instance)
		{
			if (instance.vertices.empty())
			{
				return false;
			}
			const auto placed = hk::placed_vertices(instance);
			for (const auto& box : boxes)
			{
				if (std::ranges::all_of(placed, [&](const hk::vector3& p) { return Inside(box, p.data(), kHavokScale); }))
				{
					return true;
				}
			}
			return false;
		}

		const hk::item& ListRoot(const hk::tagfile& file, const hk::type_table& types)
		{
			if (file.items.size() < 2 || types.identity(file.items[1].type) != "HavokPhysicsShapeList")
			{
				throw std::runtime_error("the world collision is not a HavokPhysicsShapeList");
			}
			return file.items[1];
		}
	}

	bool Inside(const Box& box, const float* point, const float scale)
	{
		for (int axis = 0; axis < 3; ++axis)
		{
			const auto value = point[axis] * scale;
			if (!(value >= box.mins[axis] && value <= box.maxs[axis]))
			{
				return false;
			}
		}
		return true;
	}

	RenderRemoval RemoveRenderTriangles(zonetool::iw8::GfxWorld& world, std::span<const Box> boxes,
		const std::set<std::uint32_t>& whole, std::deque<std::vector<std::uint16_t>>& kept)
	{
		using namespace zonetool::iw8;
		RenderRemoval result;
		if (!world.surfaces.surfaces)
		{
			throw std::runtime_error("the GfxWorld has no surfaces");
		}
		for (const auto index : whole)
		{
			if (index >= world.surfaces.count)
			{
				throw std::runtime_error(std::format("world surface {} is beyond the {} surfaces of the map", index, world.surfaces.count));
			}
		}

		// The index buffer of each transient zone, copied once.
		std::map<std::uint32_t, std::uint16_t*> copies;
		const auto writable = [&](GfxWorldTransientZone& zone, const std::uint32_t z) -> std::uint16_t*
		{
			if (const auto found = copies.find(z); found != copies.end())
			{
				return found->second;
			}
			auto& copy = kept.emplace_back(zone.drawVerts.indices, zone.drawVerts.indices + zone.drawVerts.indexCount);
			copy.resize(std::max<std::size_t>(copy.size(), 1));
			zone.drawVerts.indices = copy.data();
			copies.emplace(z, copy.data());
			return copy.data();
		};

		for (std::uint32_t s = 0; s < world.surfaces.count; ++s)
		{
			const auto& surface = world.surfaces.surfaces[s];
			const bool all = whole.contains(s);
			if (!all && boxes.empty())
			{
				continue;
			}
			auto* zone = surface.transientZone < world.draw.transientZoneCount && surface.transientZone < 1536
				? world.draw.transientZones[surface.transientZone] : nullptr;
			const auto& tris = surface.tris;
			if (!zone || !zone->drawVerts.posData || !zone->drawVerts.indices ||
				std::uint64_t(tris.posOffset) + 12ull * tris.vertexCount > zone->drawVerts.posDataSize ||
				std::uint64_t(tris.baseIndex) + 3ull * tris.triCount > zone->drawVerts.indexCount)
			{
				if (all)
				{
					throw std::runtime_error(std::format("world surface {} has no resident vertices or indices (a streamed transient "
						"zone); it cannot be removed", s));
				}
				++result.notResident;
				continue;
			}
			const auto* indices = zone->drawVerts.indices;
			std::uint16_t* out = nullptr;
			std::size_t removed = 0;
			for (std::uint32_t t = 0; t < tris.triCount; ++t)
			{
				const auto at = tris.baseIndex + 3u * t;
				const std::uint16_t i0 = indices[at], i1 = indices[at + 1], i2 = indices[at + 2];
				bool selected = all;
				if (!selected)
				{
					std::array<const float*, 3> corners{};
					bool valid = true;
					for (int k = 0; k < 3; ++k)
					{
						const std::uint16_t vertex = k == 0 ? i0 : k == 1 ? i1 : i2;
						if (vertex >= tris.vertexCount)
						{
							valid = false;
							break;
						}
						corners[k] = reinterpret_cast<const float*>(zone->drawVerts.posData + tris.posOffset + 12u * vertex);
					}
					if (valid)
					{
						for (const auto& box : boxes)
						{
							float p[3][3];
							for (int k = 0; k < 3; ++k)
							{
								std::memcpy(p[k], corners[k], 12);
							}
							if (Inside(box, p[0]) && Inside(box, p[1]) && Inside(box, p[2]))
							{
								selected = true;
								break;
							}
						}
					}
				}
				if (!selected)
				{
					continue;
				}
				++removed;
				if (i0 == i1 && i1 == i2)
				{
					++result.alreadyDegenerate;
					continue;
				}
				if (!out)
				{
					out = writable(*zone, surface.transientZone);
					indices = out;
				}
				out[at + 1] = i0;
				out[at + 2] = i0;
				++result.triangles;
			}
			if (removed)
			{
				result.touched.push_back(s);
				if (removed == tris.triCount)
				{
					result.emptied.push_back(s);
				}
			}
		}
		return result;
	}

	std::vector<std::uint8_t> RemoveCollision(std::span<const std::uint8_t> data, const hk::type_table& types,
		std::span<const Box> boxes, CollisionRemoval& report)
	{
		// The primitives inside a box are taken out in place, as Havok's own primitive removal does it (Replay
		// 0x141E24BE7: a triangle becomes the removed marker DE AD DE AD, a quad loses one half by repeating an index):
		// the mesh keeps its vertices, sections and trees (a tree may now be larger than what it holds), so nothing that
		// stays moves. Only the bytes of the primitive array (and the interior flags of a quad half that moves) change.
		report.bytesBefore = data.size();
		auto file = hk::read_tagfile(data, &types);
		const auto& root = ListRoot(file, types);
		const ShapeList list{file, types, root};
		const auto* shapes = list.array("shapes");
		const auto count = shapes ? shapes->count : 0;
		for (const auto name : {"shapeIndices", "shapeNames", "vertCounts", "triCounts", "shapeContents", "convexCounts"})
		{
			const auto* rows = list.array(name);
			if ((rows ? rows->count : 0) != count)
			{
				throw std::runtime_error(std::format("the world shape list has {} {} rows for {} shapes: a map with collision tiles "
					"(research 90) is not edited", rows ? rows->count : 0, name, count));
			}
		}

		// The edited items: item index -> its new data.
		std::map<std::uint32_t, std::vector<std::uint8_t>> edited;
		constexpr std::array<std::uint8_t, 4> removed_marker{0xDE, 0xAD, 0xDE, 0xAD};
		for (std::uint32_t i = 0; i < count; i++)
		{
			const auto shape_index = target_at(*shapes, i * 8);
			if (!shape_index || types.identity(file.items.at(shape_index).type) != "hknpCompressedMeshShape")
			{
				continue;
			}
			++report.shapes;
			const auto decoded = hk::read_compressed_mesh_shape(file, types, shape_index);
			report.trianglesBefore += decoded.triangles.size();
			report.convexBefore += decoded.convex_shapes.size();

			const auto& shape = file.items.at(shape_index);
			const auto data_index = target_at(shape, member_offset(types, shape.type, "data"));
			if (!data_index)
			{
				throw std::runtime_error("a compressed mesh has no data");
			}
			const auto& mesh_data = file.items.at(data_index);
			const auto sections_index = target_at(mesh_data, member_offset(types, mesh_data.type, "meshTree.sections"));
			const auto primitives_index = target_at(mesh_data, member_offset(types, mesh_data.type, "meshTree.primitives"));
			if (!sections_index || !primitives_index)
			{
				throw std::runtime_error("a compressed mesh has no sections or primitives");
			}
			const auto& sections = file.items.at(sections_index);
			const auto bits_per_key = value_at<std::int32_t>(mesh_data, member_offset(types, mesh_data.type, "meshTree.bitsPerKey"));
			const auto section_bits = static_cast<std::int32_t>(std::bit_width(std::max(sections.count, 1u) - 1));
			const auto primitive_bits = bits_per_key - section_bits;
			const auto section_size = types.size_of(sections.type);
			const auto first_primitive_at = member_offset(types, sections.type, "firstPrimitiveIndex");
			const auto primitive_byte = [&](const std::uint32_t key) -> std::size_t
			{
				const auto section = key >> primitive_bits;
				const auto local = (key & ((1u << primitive_bits) - 1u)) >> 1;
				if (section >= sections.count)
				{
					throw std::runtime_error("a compressed mesh key names a section that the mesh does not have");
				}
				return (std::size_t(value_at<std::uint32_t>(sections, section * section_size + first_primitive_at)) + local) * 4;
			};

			// The halves of each primitive that go.
			std::map<std::size_t, std::pair<std::uint32_t, std::uint8_t>> halves; // byte -> (key without half, half mask)
			std::size_t triangles = 0, convex = 0;
			for (std::size_t t = 0; t < decoded.triangles.size(); t++)
			{
				if (!TriangleInside(boxes, decoded.triangles[t]))
				{
					continue;
				}
				const auto key = decoded.triangle_keys[t];
				auto& entry = halves[primitive_byte(key)];
				entry.first = key & ~1u;
				entry.second |= static_cast<std::uint8_t>(1u << (key & 1u));
				++triangles;
			}
			std::vector<std::size_t> convex_bytes;
			for (std::size_t c = 0; c < decoded.convex_shapes.size(); c++)
			{
				if (InstanceInside(boxes, decoded.convex_shapes[c]))
				{
					if (decoded.convex_keys[c] == 0xFFFFFFFF)
					{
						throw std::runtime_error("a convex shape of the world collision has no primitive");
					}
					convex_bytes.push_back(primitive_byte(decoded.convex_keys[c]));
					++convex;
				}
			}
			if (!triangles && !convex)
			{
				continue;
			}

			auto primitives = file.items.at(primitives_index).data;
			// Keys of primitives that go entirely: the SIMD tree drops them (as the stock trees leave out the primitives the
			// Replay tools removed).
			std::set<std::uint32_t> gone_keys;
			std::optional<std::vector<std::uint8_t>> interior;
			const auto interior_index = target_at(shape, member_offset(types, shape.type, "triangleIsInterior.storage.words"));
			const auto interior_bit = [&](const std::uint32_t bit) -> bool
			{
				const auto& words = interior ? *interior : file.items.at(interior_index).data;
				return bit / 8 < words.size() && (words[bit / 8] >> (bit % 8) & 1) != 0;
			};
			for (const auto& [at, entry] : halves)
			{
				if (at + 4 > primitives.size())
				{
					throw std::runtime_error("a compressed mesh primitive is outside its array");
				}
				auto* indices = primitives.data() + at;
				const bool quad = indices[2] != indices[3];
				const auto [key, mask] = entry;
				if (!quad || mask == 3)
				{
					std::memcpy(indices, removed_marker.data(), 4);
					gone_keys.insert(key);
				}
				else if (mask == 2)
				{
					// (a, b, c, d) without (a, c, d): the triangle (a, b, c).
					indices[3] = indices[2];
				}
				else
				{
					// (a, b, c, d) without (a, b, c): the triangle (a, c, d), which is now half 0 and takes its interior flag.
					indices[1] = indices[2];
					indices[2] = indices[3];
					if (interior_index)
					{
						const auto value = interior_bit(key | 1u);
						if (!interior)
						{
							interior = file.items.at(interior_index).data;
						}
						auto& byte = (*interior)[key / 8];
						byte = static_cast<std::uint8_t>(value ? byte | 1u << (key % 8) : byte & ~(1u << (key % 8)));
					}
				}
			}
			for (std::size_t c = 0; c < convex_bytes.size(); c++)
			{
				std::memcpy(primitives.data() + convex_bytes[c], removed_marker.data(), 4);
			}
			for (std::size_t c = 0; c < decoded.convex_shapes.size(); c++)
			{
				if (decoded.convex_keys[c] != 0xFFFFFFFF && InstanceInside(boxes, decoded.convex_shapes[c]))
				{
					gone_keys.insert(decoded.convex_keys[c]);
				}
			}

			// The SIMD tree (hkcdSimdTree::Node, 128 bytes: lx, hx, ly, hy, lz, hz as four floats each, four data words,
			// the leaf flag): a leaf slot of a key that went gets the data and bounds of an empty slot.
			const auto simd_index = target_at(mesh_data, member_offset(types, mesh_data.type, "simdTree.nodes"));
			if (simd_index && !gone_keys.empty())
			{
				auto nodes = file.items.at(simd_index).data;
				const auto node_count = nodes.size() / 128;
				std::array<float, 6> empty{3.40282e38f, -3.40282e38f, 3.40282e38f, -3.40282e38f, 3.40282e38f, -3.40282e38f};
				bool empty_found = false;
				for (std::size_t n = 1; n < node_count && !empty_found; n++)
				{
					for (std::size_t slot = 0; slot < 4 && nodes[n * 128 + 112]; slot++)
					{
						std::uint32_t value{};
						std::memcpy(&value, nodes.data() + n * 128 + 96 + slot * 4, 4);
						if (value == 0xFFFFFFFF)
						{
							for (std::size_t side = 0; side < 6; side++)
							{
								std::memcpy(&empty[side], nodes.data() + n * 128 + side * 16 + slot * 4, 4);
							}
							empty_found = true;
							break;
						}
					}
				}
				std::size_t cleared = 0;
				for (std::size_t n = 1; n < node_count; n++)
				{
					if (!nodes[n * 128 + 112])
					{
						continue;
					}
					for (std::size_t slot = 0; slot < 4; slot++)
					{
						std::uint32_t value{};
						std::memcpy(&value, nodes.data() + n * 128 + 96 + slot * 4, 4);
						if (value == 0xFFFFFFFF || !gone_keys.contains(value))
						{
							continue;
						}
						constexpr std::uint32_t none = 0xFFFFFFFF;
						std::memcpy(nodes.data() + n * 128 + 96 + slot * 4, &none, 4);
						for (std::size_t side = 0; side < 6; side++)
						{
							std::memcpy(nodes.data() + n * 128 + side * 16 + slot * 4, &empty[side], 4);
						}
						++cleared;
					}
				}
				if (cleared != gone_keys.size())
				{
					throw std::runtime_error(std::format("the SIMD tree of the world collision holds {} of the {} primitive keys that go",
						cleared, gone_keys.size()));
				}
				edited[simd_index] = std::move(nodes);
			}
			edited[primitives_index] = std::move(primitives);
			if (interior)
			{
				edited[interior_index] = std::move(*interior);
			}
			report.trianglesRemoved += triangles;
			report.convexRemoved += convex;
		}

		if (edited.empty())
		{
			report.trianglesAfter = report.trianglesBefore;
			report.convexAfter = report.convexBefore;
			report.bytesAfter = data.size();
			return {data.begin(), data.end()};
		}

		// The edited arrays hold no pointers, so their bytes are in the stock data as they are: each is found (once) and
		// overwritten there, and every other byte stays stock.
		std::vector<std::uint8_t> written(data.begin(), data.end());
		for (const auto& [index, bytes] : edited)
		{
			const auto& stock = file.items.at(index).data;
			if (!file.items.at(index).pointers.empty() || stock.size() != bytes.size() || stock.size() < 64)
			{
				throw std::runtime_error("an edited Havok array cannot be placed in the stock data");
			}
			const std::boyer_moore_horspool_searcher searcher(stock.begin(), stock.end());
			const auto found = std::search(written.begin(), written.end(), searcher);
			if (found == written.end() || std::search(found + 1, written.end(), searcher) != written.end())
			{
				throw std::runtime_error("an edited Havok array is not found once in the stock data");
			}
			std::copy(bytes.begin(), bytes.end(), found);
		}

		// Read back: the meshes must decode and hold nothing inside a box.
		const auto again = hk::read_tagfile(written, &types);
		const ShapeList built_list{again, types, ListRoot(again, types)};
		const auto* built_shapes = built_list.array("shapes");
		for (std::uint32_t i = 0; built_shapes && i < built_shapes->count; i++)
		{
			const auto shape = target_at(*built_shapes, i * 8);
			if (!shape || types.identity(again.items.at(shape).type) != "hknpCompressedMeshShape")
			{
				continue;
			}
			const auto decoded = hk::read_compressed_mesh_shape(again, types, shape);
			report.trianglesAfter += decoded.triangles.size();
			report.convexAfter += std::ranges::count_if(decoded.convex_keys, [](const std::uint32_t key) { return key != 0xFFFFFFFF; });
			for (const auto& triangle : decoded.triangles)
			{
				if (TriangleInside(boxes, triangle))
				{
					throw std::runtime_error("the edited world collision still holds a triangle inside a removal box");
				}
			}
			for (std::size_t c = 0; c < decoded.convex_shapes.size(); c++)
			{
				if (decoded.convex_keys[c] != 0xFFFFFFFF && InstanceInside(boxes, decoded.convex_shapes[c]))
				{
					throw std::runtime_error("the edited world collision still holds a convex shape inside a removal box");
				}
			}
		}
		if (report.trianglesAfter + report.trianglesRemoved != report.trianglesBefore ||
			report.convexAfter + report.convexRemoved != report.convexBefore)
		{
			throw std::runtime_error(std::format("the edited world collision holds {} triangles and {} convex shapes; {} and {} were expected",
				report.trianglesAfter, report.convexAfter, report.trianglesBefore - report.trianglesRemoved,
				report.convexBefore - report.convexRemoved));
		}
		report.bytesAfter = written.size();
		return written;
	}

	std::string DescribeCollision(std::span<const std::uint8_t> data, const hk::type_table& types)
	{
		const auto file = hk::read_tagfile(data, &types);
		const auto& root = ListRoot(file, types);
		const ShapeList list{file, types, root};
		const auto* shapes = list.array("shapes");
		nlohmann::ordered_json out;
		out["bytes"] = data.size();
		auto rows = nlohmann::ordered_json::array();
		for (std::uint32_t i = 0; shapes && i < shapes->count; i++)
		{
			const auto shape = target_at(*shapes, i * 8);
			nlohmann::ordered_json row;
			const auto* names = list.array("shapeNames");
			row["name"] = names ? item_string(file, target_at(*names, i * 8)) : std::string{};
			row["type"] = shape ? types.identity(file.items.at(shape).type) : std::string{};
			row["vertCount"] = list.value("vertCounts", i);
			row["triCount"] = list.value("triCounts", i);
			row["convexCount"] = list.value("convexCounts", i);
			row["contents"] = list.value("shapeContents", i);
			if (shape && row["type"] == "hknpCompressedMeshShape")
			{
				const auto decoded = hk::read_compressed_mesh_shape(file, types, shape);
				auto triangles = nlohmann::json::array();
				for (std::size_t t = 0; t < decoded.triangles.size(); t++)
				{
					for (const auto& p : decoded.triangles[t])
					{
						for (const auto v : p)
						{
							triangles.push_back(v * kHavokScale);
						}
					}
				}
				row["triangles"] = std::move(triangles);
				row["triangle_tags"] = decoded.triangle_tags;
				auto convex = nlohmann::json::array();
				for (std::size_t c = 0; c < decoded.convex_shapes.size(); c++)
				{
					const auto& instance = decoded.convex_shapes[c];
					const auto placed = hk::placed_vertices(instance);
					std::array<float, 6> b{INFINITY, INFINITY, INFINITY, -INFINITY, -INFINITY, -INFINITY};
					for (const auto& p : placed)
					{
						for (int axis = 0; axis < 3; ++axis)
						{
							b[axis] = std::min(b[axis], p[axis] * kHavokScale);
							b[axis + 3] = std::max(b[axis + 3], p[axis] * kHavokScale);
						}
					}
					convex.push_back({{"min", {b[0], b[1], b[2]}}, {"max", {b[3], b[4], b[5]}}, {"tag", instance.shape_tag},
						{"points", placed.size()}, {"live", decoded.convex_keys[c] != 0xFFFFFFFF}});
				}
				row["convex"] = std::move(convex);
				row["custom"] = decoded.custom;
				row["removed"] = decoded.removed;
			}
			rows.push_back(std::move(row));
		}
		out["shapes"] = std::move(rows);
		return out.dump();
	}

	namespace
	{
		// Umbra 3 ImpTome header (version 0xD6000014; the field names are those of the IW7 headers in
		// src/games/iw7/structs.hpp, Umbra::ImpTome, whose offsets match Replay 1.20's tomes).
		constexpr std::uint32_t kTomeVersion = 0xD6000014;
		constexpr std::size_t kTomeHeaderSize = 0x170;

		std::uint32_t tome_u32(std::span<const std::uint8_t> tome, const std::size_t offset)
		{
			if (offset + 4 > tome.size())
			{
				throw std::runtime_error("the Umbra tome is shorter than its header says");
			}
			std::uint32_t value;
			std::memcpy(&value, tome.data() + offset, 4);
			return value;
		}

		std::uint32_t tome_crc32c(const std::uint8_t* data, const std::size_t size)
		{
			std::uint32_t crc = 0xFFFFFFFFu;
			for (std::size_t i = 0; i < size; ++i)
			{
				crc ^= data[i];
				for (int bit = 0; bit < 8; ++bit)
				{
					crc = (crc >> 1) ^ (0x82F63B78u & (0u - (crc & 1u)));
				}
			}
			return ~crc;
		}
	}

	StockTome ReadTome(std::span<const std::uint8_t> tome)
	{
		if (tome.size() < kTomeHeaderSize || tome_u32(tome, 0x00) != kTomeVersion || tome_u32(tome, 0x08) != tome.size())
		{
			throw std::runtime_error("the GfxWorld Umbra tome is not a Replay 1.20 tome (version 0xD6000014)");
		}
		StockTome out;
		out.checksumDisabled = tome_u32(tome, 0x04) == 0xD15AB1EDu;
		out.checksumValid = !out.checksumDisabled && tome_u32(tome, 0x04) == tome_crc32c(tome.data() + 8, tome.size() - 8);
		for (int axis = 0; axis < 3; ++axis)
		{
			std::memcpy(&out.mins[axis], tome.data() + 0x14 + 4 * axis, 4); // m_treeMin
			std::memcpy(&out.maxs[axis], tome.data() + 0x20 + 4 * axis, 4); // m_treeMax
		}
		out.objects = tome_u32(tome, 0x40);           // m_numObjects
		out.userIDStarts = tome_u32(tome, 0x4C) != 0; // m_userIDStarts
		out.gates = tome_u32(tome, 0x68);             // m_numGates
		out.clusters = tome_u32(tome, 0x7C);          // m_numClusters
		out.leafTiles = tome_u32(tome, 0x8C);         // m_numLeafTiles
		out.tiles = tome_u32(tome, 0x90);             // m_numTiles
		out.tomes = tome_u32(tome, 0xB0);             // m_numTomes
		const auto ids = tome_u32(tome, 0x50);        // m_userIDs
		if (!out.objects || !ids || ids + 4ull * out.objects > tome.size())
		{
			throw std::runtime_error("the Umbra tome's user ID table is outside the tome");
		}
		out.userIDs.resize(out.objects);
		std::memcpy(out.userIDs.data(), tome.data() + ids, 4ull * out.objects);
		return out;
	}

	std::vector<std::uint8_t> OpenOcclusion(std::span<const std::uint8_t> stockTome, OcclusionOpening& report)
	{
		const auto stock = ReadTome(stockTome);
		if (!stock.checksumValid && !stock.checksumDisabled)
		{
			throw std::runtime_error("the stock Umbra tome fails its checksum");
		}
		if (stock.gates || stock.tomes || stock.userIDStarts)
		{
			throw std::runtime_error(std::format("the stock Umbra tome has {} gate(s), {} tome(s) in a collection and {} user ID "
				"starts: the conservative tome has none, so its occlusion is not replaced", stock.gates, stock.tomes,
				stock.userIDStarts ? "its own" : "no"));
		}
		for (int axis = 0; axis < 3; ++axis)
		{
			if (!std::isfinite(stock.mins[axis]) || !std::isfinite(stock.maxs[axis]) || stock.mins[axis] >= stock.maxs[axis])
			{
				throw std::runtime_error("the stock Umbra tome has invalid bounds");
			}
		}
		const float mins[3]{stock.mins[0], stock.mins[1], stock.mins[2]};
		const float maxs[3]{stock.maxs[0], stock.maxs[1], stock.maxs[2]};
		auto out = replayrender::BuildConservativeUmbraTome(stock.userIDs, mins, maxs);
		report.objects = stock.objects;
		report.classes = {};
		for (const auto id : stock.userIDs)
		{
			++report.classes[(id >> 28) & 7];
		}
		report.bytesBefore = stockTome.size();
		report.bytesAfter = out.size();
		return out;
	}
}
