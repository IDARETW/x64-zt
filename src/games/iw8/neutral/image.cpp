#include <std_include.hpp>
#include "image.hpp"

#include <zone/zone_memory.hpp>

namespace zonetool::iw8::neutral
{
	namespace
	{
		constexpr std::size_t rounded(const std::size_t size, const std::size_t alignment = 8)
		{
			return (size + alignment - 1) & ~(alignment - 1);
		}

		// A Replay pixel format: its neutral name, the side of its block in pixels (1: not block compressed) and the bytes
		// of a block. GFX_PF_R1 (bits per pixel) does not convert.
		struct format_info
		{
			GfxPixelFormat native;
			const char* name;
			std::uint32_t block;
			std::uint32_t bytes;
		};

		constexpr format_info formats[] = {
			{GFX_PF_R8, "R8_UNORM", 1, 1}, {GFX_PF_A8, "A8_UNORM", 1, 1}, {GFX_PF_L8, "L8_UNORM", 1, 1},
			{GFX_PF_R8G8, "R8G8_UNORM", 1, 2}, {GFX_PF_L8A8, "L8A8_UNORM", 1, 2},
			{GFX_PF_R8G8B8A8, "R8G8B8A8_UNORM", 1, 4}, {GFX_PF_R8G8B8A8_SRGB, "R8G8B8A8_UNORM_SRGB", 1, 4},
			{GFX_PF_R8_SNORM, "R8_SNORM", 1, 1}, {GFX_PF_R8G8_SNORM, "R8G8_SNORM", 1, 2}, {GFX_PF_R16, "R16_UNORM", 1, 2},
			{GFX_PF_R16G16, "R16G16_UNORM", 1, 4}, {GFX_PF_R16G16B16A16, "R16G16B16A16_UNORM", 1, 8},
			{GFX_PF_R16_SNORM, "R16_SNORM", 1, 2}, {GFX_PF_R16F, "R16_FLOAT", 1, 2}, {GFX_PF_R16G16F, "R16G16_FLOAT", 1, 4},
			{GFX_PF_R16G16B16A16F, "R16G16B16A16_FLOAT", 1, 8}, {GFX_PF_R32F, "R32_FLOAT", 1, 4},
			{GFX_PF_R32G32F, "R32G32_FLOAT", 1, 8}, {GFX_PF_R32G32B32A32F, "R32G32B32A32_FLOAT", 1, 16},
			{GFX_PF_D16, "D16_UNORM", 1, 2}, {GFX_PF_D32F, "D32_FLOAT", 1, 4}, {GFX_PF_D32F_S8, "D32_FLOAT_S8X24_UINT", 1, 8},
			{GFX_PF_R8_UINT, "R8_UINT", 1, 1}, {GFX_PF_R16_UINT, "R16_UINT", 1, 2}, {GFX_PF_R32_UINT, "R32_UINT", 1, 4},
			{GFX_PF_R32G32_UINT, "R32G32_UINT", 1, 8}, {GFX_PF_R32G32B32A32_UINT, "R32G32B32A32_UINT", 1, 16},
			{GFX_PF_R10G10B10A2_UINT, "R10G10B10A2_UINT", 1, 4}, {GFX_PF_R5G6B5, "R5G6B5_UNORM", 1, 2},
			{GFX_PF_R10G10B10A2, "R10G10B10A2_UNORM", 1, 4}, {GFX_PF_R9G9B9E5_SHAREDEXP, "R9G9B9E5_SHAREDEXP", 1, 4},
			{GFX_PF_R11G11B10F, "R11G11B10_FLOAT", 1, 4}, {GFX_PF_BC1, "BC1_UNORM", 4, 8},
			{GFX_PF_BC1_SRGB, "BC1_UNORM_SRGB", 4, 8}, {GFX_PF_BC2, "BC2_UNORM", 4, 16}, {GFX_PF_BC2_SRGB, "BC2_UNORM_SRGB", 4, 16},
			{GFX_PF_BC3, "BC3_UNORM", 4, 16}, {GFX_PF_BC3_SRGB, "BC3_UNORM_SRGB", 4, 16}, {GFX_PF_BC4, "BC4_UNORM", 4, 8},
			{GFX_PF_BC5, "BC5_UNORM", 4, 16}, {GFX_PF_BC5S, "BC5_SNORM", 4, 16}, {GFX_PF_BC6H, "BC6H_UF16", 4, 16},
			{GFX_PF_BC6HS, "BC6H_SF16", 4, 16}, {GFX_PF_BC7, "BC7_UNORM", 4, 16}, {GFX_PF_BC7_SRGB, "BC7_UNORM_SRGB", 4, 16},
			{GFX_PF_R8G8B8A8_SNORM, "R8G8B8A8_SNORM", 1, 4}, {GFX_PF_R4G4, "R4G4_UNORM", 1, 1},
			{GFX_PF_R10G10B10A2_SNORM, "R10G10B10A2_SNORM", 1, 4},
		};

		constexpr std::uint32_t map_type_mask = 0x38000;
		constexpr std::pair<std::uint32_t, const char*> shapes[] = {{0, "2d"}, {IMG_DISK_FLAG_MAPTYPE_CUBE, "cube"},
			{IMG_DISK_FLAG_MAPTYPE_3D, "3d"}, {IMG_DISK_FLAG_MAPTYPE_1D, "1d"}, {IMG_DISK_FLAG_MAPTYPE_ARRAY, "2d_array"},
			{IMG_DISK_FLAG_MAPTYPE_CUBE_ARRAY, "cube_array"}};

		// Authoring flags with a neutral name; mip generation is a field of three bits.
		constexpr std::pair<std::uint32_t, const char*> flags[] = {{IMG_DISK_FLAG_NOPICMIP, "no_picmip"},
			{IMG_DISK_FLAG_NOMIPMAPS, "no_mipmaps"}, {IMG_DISK_FLAG_NORMAL_OCCLUSON_GLOSS, "normal_occlusion_gloss"},
			{IMG_DISK_FLAG_CLAMP_U, "clamp_u"}, {IMG_DISK_FLAG_CLAMP_V, "clamp_v"}, {IMG_DISK_FLAG_GAMMA_SRGB, "gamma_srgb"},
			{IMG_DISK_FLAG_AUTOMETALNESS, "auto_metalness"}, {IMG_DISK_FLAG_AUTODISPLACEMENT, "auto_displacement"},
			{IMG_DISK_FLAG_INVERT_ALPHA, "invert_alpha"}, {IMG_DISK_FLAG_PREMUL_ALPHA, "premultiplied_alpha"},
			{IMG_DISK_FLAG_PREMUL_KEEP_ALPHA, "premultiplied_keep_alpha"}, {IMG_DISK_FLAG_EXTRACT_ALPHA, "extract_alpha"},
			{IMG_DISK_FLAG_OCTAHEDRON, "octahedron"}};
		constexpr std::uint32_t mip_generation_mask = 0x700000;
		constexpr const char* mip_generations[] = {nullptr, "mipgen_lanczos3", "mipgen_catmull_rom", "mipgen_cubic_bspline",
			"mipgen_box", "mipgen_coverage_preserving"};

		// The texels of these images are elsewhere: a stream, a packed atlas or a compressed resident block.
		constexpr std::uint32_t elsewhere_mask = IMG_DISK_FLAG_STREAMED | IMG_DISK_FLAG_USE_OODLE_COMPRESSION |
			IMG_DISK_FLAG_PACKED_ATLAS;

		constexpr const char* usages[] = {"2d", "function", "color", "gray", "signed_velocity", "normal", "metalness",
			"normal_occlusion_gloss", "signed_distance_field", "card_imposter_normal"};

		const format_info* find_format(const GfxPixelFormat native)
		{
			const auto found = std::ranges::find(formats, native, &format_info::native);
			return found != std::end(formats) ? found : nullptr;
		}

		std::uint32_t slices(const asset_data::image& data)
		{
			return data.elements * (data.shape == "cube" || data.shape == "cube_array" ? 6 : 1);
		}

		// The bytes of one slice of a level; Replay starts each slice of each level on 16 bytes.
		std::size_t slice_size(const format_info& format, const std::uint32_t width, const std::uint32_t height,
			const std::uint32_t depth)
		{
			const std::size_t columns = (width + format.block - 1) / format.block;
			const std::size_t rows = (height + format.block - 1) / format.block;
			return columns * rows * depth * format.bytes;
		}
	}

	std::optional<image_source> to_neutral(const GfxImage& native)
	{
		const auto native_flags = static_cast<std::uint32_t>(native.flags);
		const auto* format = find_format(native.format);
		const auto shape = std::ranges::find(shapes, native_flags & map_type_mask, &std::pair<std::uint32_t, const char*>::first);
		if (!native.name || !format || shape == std::end(shapes) || (native_flags & elsewhere_mask) || native.fallback ||
			native.packedAtlasData || native.streamedPartCount || !native.pixels.residentData ||
			native.semantic >= std::size(usages) || !native.width || !native.height || !native.depth || !native.numElements)
		{
			return std::nullopt;
		}

		image_source source;
		auto& data = source.image;
		data.name = native.name;
		data.format = format->name;
		data.shape = shape->second;
		data.width = native.width;
		data.height = native.height;
		data.depth = native.depth;
		data.elements = native.numElements;
		data.usage = usages[native.semantic];
		auto rest = native_flags & ~map_type_mask & ~mip_generation_mask;
		for (const auto& [bit, name] : flags)
		{
			if (rest & bit)
			{
				data.flags.emplace_back(name);
				rest &= ~bit;
			}
		}

		const auto mip_generation = (native_flags & mip_generation_mask) >> 20;
		if (mip_generation >= std::size(mip_generations))
		{
			return std::nullopt;
		}

		if (mip_generation)
		{
			data.flags.emplace_back(mip_generations[mip_generation]);
		}

		std::ranges::sort(data.flags);

		// The texels: each level holds its slices, each on 16 bytes; the sum must be the resident size.
		std::size_t offset = 0;
		for (std::uint32_t level = 0; level < native.levelCount; ++level)
		{
			auto& value = data.levels.emplace_back();
			value.width = std::max(1u, data.width >> level);
			value.height = std::max(1u, data.height >> level);
			value.depth = data.shape == "3d" ? std::max(1u, data.depth >> level) : 1u;
			const auto size = slice_size(*format, value.width, value.height, value.depth);
			for (std::uint32_t slice = 0; slice < slices(data); ++slice)
			{
				if (offset + size > native.totalSize)
				{
					return std::nullopt;
				}

				value.bytes.insert(value.bytes.end(), native.pixels.residentData + offset, native.pixels.residentData + offset + size);
				offset += rounded(size, 16);
			}
		}

		if (offset != native.totalSize)
		{
			return std::nullopt;
		}

		auto& provenance = source.provenance;
		provenance.profile = image_profile;
		provenance.kind = "image";
		provenance.name = data.name;
		provenance.fields = {{"flags", asset_data::hex(rest)}, {"textureId", asset_data::hex(native.textureId)},
			{"semanticSpecific", asset_data::hex(native.semanticSpecific)}, {"atlasInfo", asset_data::hex(native.atlasInfo)},
			{"category", asset_data::hex(native.category)}, {"unknown_0x31", asset_data::hex(native.unknown_0x31)},
			{"freqDomainMetricBias", asset_data::hex(native.freqDomainMetricBias)}, {"streams", asset_data::hex(native.streams)}};
		asset_data::validate(data);
		return source;
	}

	GfxImage* from_neutral(const asset_data::image& data, const asset_data::provenance* source, zone_memory& memory)
	{
		asset_data::validate(data);
		const auto format = std::ranges::find_if(formats, [&data](const format_info& item) { return data.format == item.name; });
		const auto shape = std::ranges::find(shapes, data.shape, &std::pair<std::uint32_t, const char*>::second);
		if (format == std::end(formats) || shape == std::end(shapes) || data.levels.empty() || data.levels.size() > 255 ||
			data.width > 0xFFFF || data.height > 0xFFFF || data.depth > 0xFFFF || data.elements > 0xFFFF)
		{
			throw std::runtime_error("image " + data.name + " has no Replay format, shape or resident size");
		}

		auto* native = memory.allocate<GfxImage>();
		std::uint32_t native_flags = shape->first;
		for (const auto& flag : data.flags)
		{
			const auto bit = std::ranges::find(flags, flag, &std::pair<std::uint32_t, const char*>::second);
			const auto generation = std::ranges::find_if(mip_generations, [&flag](const char* name) { return name && flag == name; });
			if (bit != std::end(flags))
			{
				native_flags |= bit->first;
			}
			else if (generation != std::end(mip_generations))
			{
				native_flags |= static_cast<std::uint32_t>(generation - std::begin(mip_generations)) << 20;
			}
			else
			{
				throw std::runtime_error("image " + data.name + " has a flag without a Replay bit: " + flag);
			}
		}

		const auto same_profile = source && source->profile == image_profile;
		native->format = format->native;
		native->width = static_cast<std::uint16_t>(data.width);
		native->height = static_cast<std::uint16_t>(data.height);
		native->depth = static_cast<std::uint16_t>(data.depth);
		native->numElements = static_cast<std::uint16_t>(data.elements);
		native->semantic = static_cast<TextureSemantic>(std::ranges::find(usages, data.usage) - std::begin(usages));
		if (native->semantic >= std::size(usages))
		{
			throw std::runtime_error("image " + data.name + " has a usage without a Replay semantic: " + data.usage);
		}

		native->levelCount = static_cast<std::uint8_t>(data.levels.size());
		// The converter's resident images (owner-tested) use the auto-generated category.
		native->category = IMG_CATEGORY_AUTO_GENERATED;
		if (same_profile)
		{
			native_flags |= source->value<std::uint32_t>("flags");
			native->textureId = source->value<GfxTextureId>("textureId");
			native->semanticSpecific = source->value<GfxImageSemanticSpecific>("semanticSpecific");
			native->atlasInfo = source->value<GfxImageAtlasInfo>("atlasInfo");
			native->category = source->value<GfxImageCategory>("category");
			native->unknown_0x31 = source->value<std::uint8_t>("unknown_0x31");
			native->freqDomainMetricBias = source->value<char>("freqDomainMetricBias");
			const auto streams = source->value<std::array<GfxImageStreamData, std::size(native->streams)>>("streams");
			std::ranges::copy(streams, native->streams);
		}

		native->flags = static_cast<GfxImageFlags>(native_flags);
		std::size_t total = 0;
		for (const auto& level : data.levels)
		{
			const auto size = slice_size(*format, level.width, level.height, level.depth);
			if (level.bytes.size() != size * slices(data))
			{
				throw std::runtime_error("image " + data.name + " has a mip level whose bytes do not match its size");
			}

			total += rounded(size, 16) * slices(data);
		}

		if (total > std::numeric_limits<std::uint32_t>::max())
		{
			throw std::runtime_error("image " + data.name + " exceeds Replay's resident image size");
		}

		auto* texels = memory.manual_allocate<std::uint8_t>(rounded(std::max<std::size_t>(total, 1)));
		std::size_t offset = 0;
		for (const auto& level : data.levels)
		{
			const auto size = slice_size(*format, level.width, level.height, level.depth);
			for (std::uint32_t slice = 0; slice < slices(data); ++slice)
			{
				std::ranges::copy_n(level.bytes.begin() + slice * size, size, texels + offset);
				offset += rounded(size, 16);
			}
		}

		native->totalSize = static_cast<std::uint32_t>(total);
		native->pixels.residentData = texels;
		auto* name = memory.manual_allocate<char>(rounded(data.name.size() + 1));
		std::ranges::copy(data.name, name);
		native->name = name;
		return native;
	}

	std::size_t native_size(const asset_data::image& data)
	{
		std::size_t total = 0;
		for (const auto& level : data.levels)
		{
			total += rounded(level.bytes.size() + 16 * slices(data), 16);
		}

		return rounded(sizeof(GfxImage)) + rounded(std::max<std::size_t>(total, 1)) + rounded(data.name.size() + 1);
	}
}
