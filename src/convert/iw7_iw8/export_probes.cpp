#include <std_include.hpp>
#include "export.hpp"
#include "images.hpp"

namespace zonetool::convert::iw7_iw8
{
	namespace
	{
		// The reflection probes export as cube maps of 128 pixels with their mip levels. IW3 keeps 64, but IW7's faces are
		// larger, and at 64 the window reflections were blurred (owner test of cbf4948).
		constexpr std::size_t probe_size = 128;
		constexpr std::size_t probe_levels = 7;

		std::array<float, 3> vector(const float* values)
		{
			return {values[0], values[1], values[2]};
		}

		float distance_squared(const std::array<float, 3>& a, const float* b)
		{
			const auto x = a[0] - b[0];
			const auto y = a[1] - b[1];
			const auto z = a[2] - b[2];
			return x * x + y * y + z * z;
		}

		// Writes the six faces of a probe (linear colours) as a half-float cube map with its mip levels, which the importer
		// reads as a high-range capture (it keeps the colour, unlike IW3's captures).
		bool write_probe(const std::span<const DirectX::ScratchImage> faces, const std::filesystem::path& path)
		{
			DirectX::ScratchImage cube;
			if (FAILED(cube.InitializeCube(DXGI_FORMAT_R32G32B32A32_FLOAT, probe_size, probe_size, 1, 1)))
			{
				return false;
			}

			for (auto face = 0u; face < 6; face++)
			{
				DirectX::ScratchImage resized;
				const auto* top = faces[face].GetImage(0, 0, 0);
				if (top->width != probe_size || top->height != probe_size)
				{
					if (FAILED(DirectX::Resize(*top, probe_size, probe_size, DirectX::TEX_FILTER_BOX | DirectX::TEX_FILTER_FORCE_NON_WIC, resized)))
					{
						return false;
					}

					top = resized.GetImage(0, 0, 0);
				}

				const auto& target = *cube.GetImage(0, face, 0);
				for (std::size_t y = 0; y < probe_size; y++)
				{
					std::memcpy(target.pixels + y * target.rowPitch, top->pixels + y * top->rowPitch, probe_size * 16);
				}
			}

			DirectX::ScratchImage mips;
			if (FAILED(DirectX::GenerateMipMaps(cube.GetImages(), cube.GetImageCount(), cube.GetMetadata(),
				DirectX::TEX_FILTER_BOX | DirectX::TEX_FILTER_FORCE_NON_WIC, probe_levels, mips)))
			{
				return false;
			}

			// The colours stay linear and of their whole range (half floats): the importer scales each probe itself.
			DirectX::ScratchImage stored;
			if (FAILED(DirectX::Convert(mips.GetImages(), mips.GetImageCount(), mips.GetMetadata(), DXGI_FORMAT_R16G16B16A16_FLOAT,
				DirectX::TEX_FILTER_FORCE_NON_WIC, DirectX::TEX_THRESHOLD_DEFAULT, stored)))
			{
				return false;
			}

			std::filesystem::create_directories(path.parent_path());
			return SUCCEEDED(DirectX::SaveToDDSFile(stored.GetImages(), stored.GetImageCount(), stored.GetMetadata(),
				DirectX::DDS_FLAGS_FORCE_DX9_LEGACY, path.wstring().data()));
		}
	}

	unsigned int reflection_probes::find(const float* point) const
	{
		const volume* best{};
		for (const auto& candidate : this->volumes)
		{
			auto inside = true;
			for (auto axis = 0; axis < 3 && inside; axis++)
			{
				const auto& direction = candidate.axes[axis];
				const auto offset = (point[0] - candidate.center[0]) * direction[0] + (point[1] - candidate.center[1]) * direction[1] +
					(point[2] - candidate.center[2]) * direction[2];
				inside = std::abs(offset) <= candidate.half_size[axis];
			}

			if (inside && (!best || candidate.priority > best->priority))
			{
				best = &candidate;
			}
		}

		if (best)
		{
			return best->probe;
		}

		auto nearest = 0u;
		auto nearest_distance = FLT_MAX;
		for (auto i = 1u; i < this->origins.size(); i++)
		{
			if (const auto distance = distance_squared(this->origins[i], point); distance < nearest_distance)
			{
				nearest = i;
				nearest_distance = distance;
			}
		}

		return nearest;
	}

	// IW7 keeps the cube maps of its probes in one cube map array. Each probe instance places one of them, with a
	// box (an oriented bounding box and a priority) that the surfaces in it reflect.
	reflection_probes export_reflection_probes(const source_map& source, extraction& output)
	{
		reflection_probes result;
		result.entries.push_back({{"image", export_default_probe(output)}, {"origin", {0.0f, 0.0f, 0.0f}}});
		result.origins.push_back({});

		const auto& data = source.world()->draw.reflectionProbeData;
		const auto array = source.resolve(iw7::ASSET_TYPE_IMAGE, data.reflectionProbeArrayImage);
		const auto faces = array ? load_image_faces(*array, DXGI_FORMAT_R32G32B32A32_FLOAT, probe_size * 2)
			: std::vector<DirectX::ScratchImage>{};
		if (faces.size() < 6 || faces.size() % 6)
		{
			ZONETOOL_WARNING("iw7: the reflection probes (%u) cannot be read; the world reflects a neutral probe",
				data.reflectionProbeCount);
			return result;
		}

		// The origin of each cube map: the probe of the same index, else an instance that places it.
		const auto cube_count = faces.size() / 6;
		std::vector<std::optional<std::array<float, 3>>> origins(cube_count);
		for (auto i = 0u; i < data.reflectionProbeCount && i < cube_count; i++)
		{
			origins[i] = vector(data.reflectionProbes[i].origin);
		}

		for (auto i = 0u; i < data.reflectionProbeInstanceCount; i++)
		{
			const auto& instance = data.reflectionProbeInstances[i];
			if (instance.probeImageIndex < cube_count && !origins[instance.probeImageIndex])
			{
				origins[instance.probeImageIndex] = vector(instance.probePosition);
			}
		}

		std::vector<unsigned int> entries(cube_count);
		for (std::size_t cube = 0; cube < cube_count; cube++)
		{
			const auto name = std::format("iw7_reflection_probe_{}", cube);
			if (!write_probe(std::span(faces).subspan(cube * 6, 6), output.extracted / "images" / (name + ".dds")))
			{
				ZONETOOL_WARNING("iw7: reflection probe %zu cannot be written", cube);
				continue;
			}

			const auto origin = origins[cube].value_or(std::array<float, 3>{});
			entries[cube] = static_cast<unsigned int>(result.entries.size());
			result.entries.push_back({{"image", name}, {"origin", origin}});
			result.origins.push_back(origin);
		}

		// Each instance keeps IW7's placement of its cube map, which is Replay's instance layout: the box it covers (an
		// oriented box with a priority and a feather), the projection origin and the extents that box projection
		// reflects to, and the rotation of the capture.
		auto rotated = 0u;
		auto expanded = 0u;
		for (auto i = 0u; i < data.reflectionProbeInstanceCount; i++)
		{
			const auto& instance = data.reflectionProbeInstances[i];
			if (instance.probeImageIndex >= cube_count || !entries[instance.probeImageIndex])
			{
				continue;
			}

			const auto& box = instance.volumeObb;
			result.volumes.push_back(
			{
				vector(box.center),
				{vector(box.xAxis), vector(box.yAxis), vector(box.zAxis)},
				vector(box.halfSize),
				instance.priority,
				entries[instance.probeImageIndex],
			});

			const auto& rotation = instance.probeRotation;
			if (std::abs(rotation[3]) < 0.9999f)
			{
				rotated++;
			}

			if (std::ranges::any_of(instance.expandProjectionNeg, [](const float v) { return v != 0.0f; }) ||
				std::ranges::any_of(instance.expandProjectionPos, [](const float v) { return v != 0.0f; }))
			{
				expanded++;
			}

			result.entries[entries[instance.probeImageIndex]]["instances"].push_back(
			{
				{"position", vector(instance.probePosition)},
				{"rotation", {rotation[0], rotation[1], rotation[2], rotation[3]}},
				{"center", vector(box.center)},
				{"axes", {vector(box.xAxis), vector(box.yAxis), vector(box.zAxis)}},
				{"half_size", vector(box.halfSize)},
				{"priority", instance.priority},
				{"feather", vector(instance.feather)},
				{"expand_negative", vector(instance.expandProjectionNeg)},
				{"expand_positive", vector(instance.expandProjectionPos)},
			});
		}

		ZONETOOL_INFO("iw7: %zu reflection probes (%u in the world, %u instances, %zu cube maps; %u instances rotated, %u with "
			"expanded projections)", result.entries.size() - 1, data.reflectionProbeCount, data.reflectionProbeInstanceCount,
			cube_count, rotated, expanded);
		return result;
	}
}
