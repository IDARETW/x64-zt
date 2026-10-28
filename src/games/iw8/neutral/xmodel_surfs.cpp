#include <std_include.hpp>
#include "xmodel.hpp"
#include "../dump/xsurface_convert.h"

#include <zone/zone_memory.hpp>

namespace zonetool::iw8::neutral
{
	namespace
	{
		static_assert(sizeof(DObjPartBits) == sizeof(asset_data::mesh_surface::part_bits));

		// The resident shared data holds, per surface, the triangles followed by a triangle work region of the same size,
		// the 20-byte vertices, a 24-byte cluster per 64 triangles, the colors, and the second UV and normal transform codes.
		// A stream that a surface lacks has the offset no_stream.
		constexpr std::uint32_t no_stream = 0xFFFFFFFF;
		constexpr std::size_t triangle_size = 3 * sizeof(std::uint16_t);
		constexpr std::size_t vertex_size = 20;
		constexpr std::size_t cluster_size = 24;
		constexpr std::size_t cluster_triangles = 64;
		constexpr std::uint32_t streamed_flag = 1;

		constexpr std::size_t rounded(const std::size_t size, const std::size_t alignment = 8)
		{
			return (size + alignment - 1) & ~(alignment - 1);
		}

		const char* copy_text(zone_memory& memory, const std::string& text)
		{
			auto* copy = memory.manual_allocate<char>(rounded(text.size() + 1));
			std::ranges::copy(text, copy);
			return copy;
		}

		template <typename T>
		T* copy_array(zone_memory& memory, const T* values, const std::size_t count)
		{
			if (!count)
			{
				return nullptr;
			}

			auto* copy = memory.manual_allocate<T>(rounded(count * sizeof(T)));
			std::copy_n(values, count, copy);
			return copy;
		}

		void require(const bool condition, const char* what)
		{
			if (!condition)
			{
				throw std::runtime_error(what);
			}
		}

		template <typename T>
		T read_at(const std::span<const std::uint8_t> bytes, const std::size_t offset)
		{
			T value{};
			std::memcpy(&value, bytes.data() + offset, sizeof(value));
			return value;
		}

		template <typename T>
		void write_at(std::vector<std::uint8_t>& bytes, const std::size_t offset, const T& value)
		{
			std::memcpy(bytes.data() + offset, &value, sizeof(value));
		}

		std::span<const std::uint8_t> stream_at(const std::span<const std::uint8_t> data, const std::uint32_t offset, const std::size_t size,
			const char* what)
		{
			if (offset > data.size() || size > data.size() - offset)
			{
				throw std::runtime_error(std::format("the {} stream lies outside the shared data", what));
			}

			return data.subspan(offset, size);
		}

		// A stream of one 32-bit code a vertex, or none.
		std::optional<std::vector<std::uint32_t>> vertex_codes(const std::span<const std::uint8_t> data, const std::uint32_t offset,
			const std::size_t vertices, const char* what)
		{
			if (offset == no_stream)
			{
				return std::nullopt;
			}

			const auto bytes = stream_at(data, offset, vertices * sizeof(std::uint32_t), what);
			std::vector<std::uint32_t> codes(vertices);
			std::memcpy(codes.data(), bytes.data(), bytes.size());
			return codes;
		}

		asset_data::bounds_box box(const Bounds& bounds)
		{
			return {{bounds.midPoint.v[0], bounds.midPoint.v[1], bounds.midPoint.v[2]},
				{bounds.halfSize.v[0], bounds.halfSize.v[1], bounds.halfSize.v[2]}};
		}

		std::string surface_field(const std::size_t surface, const char* field)
		{
			return std::format("surface.{}.{}", surface, field);
		}

		// The bytes from a stream's offset to the next stream of any surface or the end of the shared data.
		std::size_t stream_gap(const std::uint32_t offset, const std::vector<std::uint32_t>& offsets, const std::size_t size)
		{
			const auto next = std::ranges::upper_bound(offsets, offset);
			return (next != offsets.end() ? *next : size) - offset;
		}

		void require_absent(const std::uint32_t offset, const char* stream, const XSurface& native, const std::vector<std::uint32_t>& offsets,
			const std::size_t size)
		{
			if (offset != no_stream)
			{
				throw std::runtime_error(std::format("a surface has a {} stream ({} bytes a vertex to the next stream)", stream,
					native.vertCount ? stream_gap(offset, offsets, size) / native.vertCount : 0));
			}
		}

		asset_data::mesh_surface read_surface(const XSurface& native, const std::span<const std::uint8_t> data, const std::size_t index,
			const std::vector<std::uint32_t>& offsets, std::map<std::string, std::string>& fields)
		{
			require(!native.subdivLevelCount && !native.subdiv, "a surface has subdivision levels");
			require(!native.blendShapeTargetCount && !native.blendShapesPerVert && !native.blendShapesRecalcTangentFrameData,
				"a surface has blend shapes");
			require_absent(native.sharedTensionAccumTableOffset, "tension table", native, offsets, data.size());
			require_absent(native.sharedTensionDataOffset, "tension", native, offsets, data.size());
			require(!(native.blendVertSize & 1) && (!native.blendVertSize || native.blendVerts), "a surface has odd or absent blend words");
			require(!native.rigidVertListCount || native.rigidVertLists, "a surface has rigid lists without data");

			asset_data::mesh_surface surface;
			surface.bounds = box(native.surfBounds);
			const std::size_t triangles = native.triCount;
			const std::size_t vertices = native.vertCount;
			const auto indices = stream_at(data, native.sharedIndexDataOffset, 2 * triangles * triangle_size, "index");
			for (std::size_t triangle = 0; triangle < triangles; ++triangle)
			{
				surface.triangles.push_back(read_at<std::array<std::uint16_t, 3>>(indices, triangle * triangle_size));
			}

			require(std::ranges::all_of(indices.subspan(triangles * triangle_size), [](const std::uint8_t byte) { return !byte; }),
				"a surface's triangle work region holds data");

			const auto vertex_bytes = stream_at(data, native.sharedVertDataOffset, vertices * vertex_size, "vertex");
			for (std::size_t vertex = 0; vertex < vertices; ++vertex)
			{
				const auto offset = vertex * vertex_size;
				const auto position = read_at<std::uint64_t>(vertex_bytes, offset);
				require(!(position >> 63), "a vertex position uses its top bit");
				const auto uv = read_at<std::uint32_t>(vertex_bytes, offset + 12);
				surface.vertices.push_back({{static_cast<std::uint32_t>(position & 0x1FFFFF), static_cast<std::uint32_t>((position >> 21) & 0x1FFFFF),
					static_cast<std::uint32_t>((position >> 42) & 0x1FFFFF)}, read_at<std::uint32_t>(vertex_bytes, offset + 8),
					{xsurf_conv::halfToFloat(static_cast<std::uint16_t>(uv)), xsurf_conv::halfToFloat(static_cast<std::uint16_t>(uv >> 16))},
					read_at<std::uint32_t>(vertex_bytes, offset + 16)});
			}

			const auto clusters = (triangles + cluster_triangles - 1) / cluster_triangles;
			const auto cluster_bytes = stream_at(data, native.sharedTriClusterDataOffset, clusters * cluster_size, "cluster");
			for (std::size_t cluster = 0; cluster < clusters; ++cluster)
			{
				surface.clusters.push_back({read_at<std::array<std::uint16_t, 6>>(cluster_bytes, cluster * cluster_size),
					read_at<std::array<std::uint32_t, 3>>(cluster_bytes, cluster * cluster_size + 12)});
			}

			if (native.sharedColorDataOffset != no_stream)
			{
				const auto colors = stream_at(data, native.sharedColorDataOffset, vertices * sizeof(std::uint32_t), "color");
				auto& values = surface.colors.emplace();
				for (std::size_t vertex = 0; vertex < vertices; ++vertex)
				{
					values.push_back(read_at<std::uint32_t>(colors, vertex * sizeof(std::uint32_t)));
				}
			}

			surface.second_uv = vertex_codes(data, native.sharedSecondUVDataOffset, vertices, "second UV");
			surface.normal_transforms = vertex_codes(data, native.sharedNormalTransformDataOffset, vertices, "normal transform");

			for (std::size_t list = 0; list < native.rigidVertListCount; ++list)
			{
				const auto& value = native.rigidVertLists[list];
				surface.rigid_lists.push_back({value.boneIndexOffset, value.vertCount, value.triOffset, value.triCount});
			}

			if (native.childBounds)
			{
				auto& bounds = surface.rigid_bounds.emplace();
				std::transform(native.childBounds, native.childBounds + native.rigidVertListCount, std::back_inserter(bounds), box);
			}

			std::ranges::copy(native.blendVertCounts, surface.blend_vertex_counts.begin());
			surface.blend_words.assign(native.blendVerts, native.blendVerts + native.blendVertSize / sizeof(std::uint16_t));
			if (native.lmapCoords)
			{
				auto& coords = surface.lightmap_coords.emplace();
				std::transform(native.lmapCoords, native.lmapCoords + vertices, std::back_inserter(coords),
					[](const PackedLmapCoords& value) { return value.packed; });
			}

			std::memcpy(surface.part_bits.data(), &native.partBits, sizeof(native.partBits));
			fields[surface_field(index, "flags")] = asset_data::hex(native.flags);
			fields[surface_field(index, "ugbID")] = asset_data::hex(native.ugbID);
			fields[surface_field(index, "hash")] = asset_data::hex(native.hash);
			return surface;
		}

		// The shared data of a set of surfaces: its bytes, and the place of each stream already in it by kind and content.
		struct shared_buffer
		{
			std::vector<std::uint8_t> data;
			std::map<std::pair<char, std::string>, std::uint32_t> streams;
		};

		// Appends the streams of a surface to the shared data and sets the surface's offsets, as every stock model does: a
		// stream that equals one of an earlier surface (the same kind and bytes) is not written again; a new triangle stream
		// starts on 16 bytes and is followed by its work region of the same size; the other streams follow without padding, in
		// the order of the vertices, the colors, the second UV and normal transform codes, and the triangle clusters.
		void write_streams(const asset_data::mesh_surface& surface, XSurface& native, shared_buffer& buffer)
		{
			// size is the bytes that the stream takes (the triangle stream also takes its work region); bytes is its content.
			const auto place = [&buffer](const char kind, const std::string& bytes, const std::size_t size, const bool aligned)
			{
				const auto key = std::pair{kind, bytes};
				if (const auto found = buffer.streams.find(key); found != buffer.streams.end())
				{
					return found->second;
				}

				if (aligned)
				{
					buffer.data.resize(rounded(buffer.data.size(), 16));
				}

				const auto offset = buffer.data.size();
				if (offset + size > std::numeric_limits<std::uint32_t>::max())
				{
					throw std::runtime_error("model surfaces exceed Replay's shared data offsets");
				}

				buffer.data.resize(offset + size);
				std::memcpy(buffer.data.data() + offset, bytes.data(), bytes.size());
				buffer.streams.emplace(key, static_cast<std::uint32_t>(offset));
				return static_cast<std::uint32_t>(offset);
			};

			const auto text = [](const auto& values) -> std::string
			{
				return {reinterpret_cast<const char*>(values.data()), values.size() * sizeof(values.front())};
			};

			native.sharedIndexDataOffset = place('i', text(surface.triangles), 2 * surface.triangles.size() * triangle_size, true);

			std::string vertices(surface.vertices.size() * vertex_size, char{});
			for (std::size_t vertex = 0; vertex < surface.vertices.size(); ++vertex)
			{
				const auto& value = surface.vertices[vertex];
				const auto position = std::uint64_t{value.position[0]} | std::uint64_t{value.position[1]} << 21 | std::uint64_t{value.position[2]} << 42;
				const auto uv = static_cast<std::uint32_t>(xsurf_conv::floatToHalf(value.uv[0]) | std::uint32_t{xsurf_conv::floatToHalf(value.uv[1])} << 16);
				auto* at = vertices.data() + vertex * vertex_size;
				std::memcpy(at, &position, sizeof(position));
				std::memcpy(at + 8, &value.self_visibility, sizeof(value.self_visibility));
				std::memcpy(at + 12, &uv, sizeof(uv));
				std::memcpy(at + 16, &value.tangent_frame, sizeof(value.tangent_frame));
			}

			native.sharedVertDataOffset = place('v', vertices, vertices.size(), false);

			const auto codes = [&](const char kind, const std::optional<std::vector<std::uint32_t>>& values)
			{
				if (!values)
				{
					return no_stream;
				}

				const auto bytes = text(*values);
				return place(kind, bytes, bytes.size(), false);
			};

			native.sharedColorDataOffset = codes('c', surface.colors);
			native.sharedSecondUVDataOffset = codes('u', surface.second_uv);
			native.sharedNormalTransformDataOffset = codes('n', surface.normal_transforms);

			std::string clusters(surface.clusters.size() * cluster_size, char{});
			for (std::size_t cluster = 0; cluster < surface.clusters.size(); ++cluster)
			{
				std::memcpy(clusters.data() + cluster * cluster_size, surface.clusters[cluster].bounds.data(), 12);
				std::memcpy(clusters.data() + cluster * cluster_size + 12, surface.clusters[cluster].cone.data(), 12);
			}

			native.sharedTriClusterDataOffset = place('t', clusters, clusters.size(), false);
			native.sharedTensionAccumTableOffset = no_stream;
			native.sharedTensionDataOffset = no_stream;
		}
	}

	std::expected<xmodel_surfs_source, std::string> to_neutral(const XModelSurfs& native)
	{
		try
		{
			require(native.name, "the model surfaces have no name");
			require(native.shared, "the model surfaces have no shared data");
			const auto& shared = *native.shared;
			require(!(shared.flags & streamed_flag), "the geometry is streamed outside the zone");
			require(!shared.dataSize || shared.data.residentData, "the shared data has a size and no bytes");
			require(!native.numsurfs || native.surfs, "the model surfaces have a count and no surfaces");
			const std::span<const std::uint8_t> data(shared.data.residentData, shared.dataSize);

			xmodel_surfs_source source;
			source.surfs.name = native.name;
			std::memcpy(source.surfs.part_bits.data(), &native.partBits, sizeof(native.partBits));
			std::vector<std::uint32_t> offsets;
			for (std::size_t surface = 0; surface < native.numsurfs; ++surface)
			{
				const auto& value = native.surfs[surface];
				for (const auto offset : {value.sharedIndexDataOffset, value.sharedVertDataOffset, value.sharedTriClusterDataOffset,
					value.sharedColorDataOffset, value.sharedSecondUVDataOffset, value.sharedNormalTransformDataOffset,
					value.sharedTensionAccumTableOffset, value.sharedTensionDataOffset})
				{
					if (offset != no_stream)
					{
						offsets.push_back(offset);
					}
				}
			}

			std::ranges::sort(offsets);
			auto& fields = source.provenance.fields;
			for (std::size_t surface = 0; surface < native.numsurfs; ++surface)
			{
				require(native.surfs[surface].shared == native.shared, "a surface has shared data of its own");
				source.surfs.surfaces.push_back(read_surface(native.surfs[surface], data, surface, offsets, fields));
			}

			fields["xpakEntry"] = asset_data::hex(native.xpakEntry);
			fields["ugbState"] = asset_data::hex(native.ugbState);
			fields["sharedFlags"] = asset_data::hex(shared.flags);
			source.provenance.profile = xmodel_profile;
			source.provenance.kind = "xmodelsurfs";
			source.provenance.name = source.surfs.name;
			asset_data::validate(source.surfs);
			return source;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	XModelSurfs* from_neutral(const asset_data::xmodel_surfs& data, const asset_data::provenance* source, zone_memory& memory)
	{
		asset_data::validate(data);
		if (!source || source->profile != xmodel_profile || data.surfaces.size() > 0xFFFF)
		{
			throw std::runtime_error("model surfaces " + data.name + " have no Replay surface flags or too many surfaces");
		}

		auto* native = memory.allocate<XModelSurfs>();
		native->name = copy_text(memory, data.name);
		native->numsurfs = static_cast<std::uint16_t>(data.surfaces.size());
		native->xpakEntry = source->value<XPakEntryInfo>("xpakEntry");
		native->ugbState = source->value<XModelSurfsUGBState>("ugbState");
		std::memcpy(&native->partBits, data.part_bits.data(), sizeof(native->partBits));
		native->surfs = memory.manual_allocate<XSurface>(rounded(std::max<std::size_t>(data.surfaces.size(), 1) * sizeof(XSurface)));
		native->shared = memory.allocate<XSurfaceShared>();
		native->shared->flags = source->value<XSurfaceSharedFlags>("sharedFlags");

		shared_buffer shared;
		for (std::size_t index = 0; index < data.surfaces.size(); ++index)
		{
			const auto& surface = data.surfaces[index];
			auto& target = native->surfs[index];
			target.flags = source->value<std::uint16_t>(surface_field(index, "flags"));
			target.ugbID = source->value<std::uint16_t>(surface_field(index, "ugbID"));
			target.hash = source->value<std::uint32_t>(surface_field(index, "hash"));
			target.vertCount = static_cast<std::uint16_t>(surface.vertices.size());
			target.triCount = static_cast<std::uint16_t>(surface.triangles.size());
			target.rigidVertListCount = static_cast<std::uint8_t>(surface.rigid_lists.size());
			std::ranges::copy(surface.blend_vertex_counts, target.blendVertCounts);
			target.blendVertSize = static_cast<std::uint32_t>(surface.blend_words.size() * sizeof(std::uint16_t));
			target.surfBounds = {{surface.bounds.midpoint[0], surface.bounds.midpoint[1], surface.bounds.midpoint[2]},
				{surface.bounds.half_size[0], surface.bounds.half_size[1], surface.bounds.half_size[2]}};
			std::memcpy(&target.partBits, surface.part_bits.data(), sizeof(target.partBits));
			target.shared = native->shared;
			write_streams(surface, target, shared);

			std::vector<XRigidVertList> lists;
			for (const auto& list : surface.rigid_lists)
			{
				if (list.bone > 0xFFFF || list.vertex_count > 0xFFFF || list.first_triangle > 0xFFFF || list.triangle_count > 0xFFFF)
				{
					throw std::runtime_error("model surfaces " + data.name + " have a rigid list beyond Replay's counts");
				}

				lists.push_back({static_cast<std::uint16_t>(list.bone), static_cast<std::uint16_t>(list.vertex_count),
					static_cast<std::uint16_t>(list.first_triangle), static_cast<std::uint16_t>(list.triangle_count)});
			}

			target.rigidVertLists = copy_array(memory, lists.data(), lists.size());
			target.blendVerts = copy_array(memory, surface.blend_words.data(), surface.blend_words.size());
			if (surface.rigid_bounds)
			{
				std::vector<Bounds> bounds;
				std::ranges::transform(*surface.rigid_bounds, std::back_inserter(bounds), [](const asset_data::bounds_box& value)
				{
					return Bounds{{value.midpoint[0], value.midpoint[1], value.midpoint[2]}, {value.half_size[0], value.half_size[1], value.half_size[2]}};
				});
				target.childBounds = memory.manual_allocate<Bounds>(rounded(std::max<std::size_t>(bounds.size(), 1) * sizeof(Bounds)));
				std::ranges::copy(bounds, target.childBounds);
			}

			if (surface.lightmap_coords)
			{
				target.lmapCoords = memory.manual_allocate<PackedLmapCoords>(rounded(std::max<std::size_t>(surface.lightmap_coords->size(), 1) *
					sizeof(PackedLmapCoords)));
				std::ranges::transform(*surface.lightmap_coords, target.lmapCoords, [](const std::uint32_t packed) { return PackedLmapCoords{packed}; });
			}
		}

		shared.data.resize(rounded(shared.data.size(), 16));
		native->shared->dataSize = static_cast<std::uint32_t>(shared.data.size());
		native->shared->data.residentData = copy_array(memory, shared.data.data(), shared.data.size());
		return native;
	}

	std::size_t native_size(const asset_data::xmodel_surfs& data)
	{
		std::size_t size = rounded(sizeof(XModelSurfs)) + rounded(data.name.size() + 1) + rounded(sizeof(XSurfaceShared)) + 16;
		for (const auto& surface : data.surfaces)
		{
			size += sizeof(XSurface) + 2 * surface.triangles.size() * triangle_size + surface.vertices.size() * (vertex_size + 4 * sizeof(std::uint32_t)) +
				surface.clusters.size() * cluster_size + surface.rigid_lists.size() * (sizeof(XRigidVertList) + sizeof(Bounds)) +
				surface.blend_words.size() * sizeof(std::uint16_t) + 8 * 16;
		}

		return size;
	}
}
