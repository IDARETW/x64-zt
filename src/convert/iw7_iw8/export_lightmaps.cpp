#include <std_include.hpp>
#include "export.hpp"
#include "images.hpp"

namespace zonetool::convert::iw7_iw8
{
	namespace
	{
		// The lightmaps keep at most this size: an IW7 lightmap covers the map in one 8192-pixel image, and the
		// pipeline packs the lightmaps into its 8192 atlas with the world materials.
		constexpr std::size_t max_lightmap_size = 4096;

		// The formats of IW3 lightmap files: D3DFMT_L8 (the sun visibility) and D3DFMT_A8R8G8B8 (the two layers).
		constexpr int format_l8 = 50;
		constexpr int format_a8r8g8b8 = 21;

		// The texels of a decoded image, row after row.
		template <typename T>
		std::vector<T> texels(const DirectX::ScratchImage& image, const std::size_t components)
		{
			const auto& top = *image.GetImage(0, 0, 0);
			std::vector<T> result(top.width * top.height * components);
			for (std::size_t y = 0; y < top.height; y++)
			{
				std::memcpy(result.data() + y * top.width * components, top.pixels + y * top.rowPitch, top.width * components * sizeof(T));
			}

			return result;
		}

		std::uint8_t encode_unit(const float value)
		{
			return static_cast<std::uint8_t>(std::lround(std::clamp(value, 0.0f, 1.0f) * 255.0f));
		}

		// IW3 stores a lightmap direction as the slopes x / z and y / z in the alpha of its two layers.
		std::uint8_t encode_slope(const float slope, const float scale, const float bias)
		{
			return encode_unit((slope + bias) / scale);
		}

		std::optional<DirectX::ScratchImage> load(const source_map& source, iw7::GfxImage* header, const DXGI_FORMAT format)
		{
			const auto image = source.resolve(iw7::ASSET_TYPE_IMAGE, header);
			auto faces = image ? load_image_faces(*image, format, max_lightmap_size) : std::vector<DirectX::ScratchImage>{};
			if (faces.size() != 1)
			{
				ZONETOOL_WARNING("iw7: lightmap image \"%s\" cannot be read", image && image->name ? image->name : "?");
				return {};
			}

			return std::move(faces.front());
		}

		// Resizes an image to a size when it has another.
		bool fit(DirectX::ScratchImage& image, const std::size_t width, const std::size_t height)
		{
			const auto& metadata = image.GetMetadata();
			if (metadata.width == width && metadata.height == height)
			{
				return true;
			}

			DirectX::ScratchImage resized;
			if (FAILED(DirectX::Resize(*image.GetImage(0, 0, 0), width, height, DirectX::TEX_FILTER_BOX | DirectX::TEX_FILTER_FORCE_NON_WIC,
				resized)))
			{
				return false;
			}

			image = std::move(resized);
			return true;
		}

		void write_file(const std::filesystem::path& path, const std::vector<std::uint8_t>& bytes)
		{
			std::filesystem::create_directories(path.parent_path());
			std::ofstream stream(path, std::ios::binary | std::ios::trunc);
			stream.write(reinterpret_cast<const char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
			if (!stream)
			{
				throw std::runtime_error(std::format("cannot write {}", path.string()));
			}
		}
	}

	// IW7 exposes the interiors that its lightmaps light far more than its sunlit exteriors (Frontier: a median
	// interior irradiance of 0.34 against a sun of 9.5). The sun converts at the brightness of an IW3 sun, and the
	// lightmaps and probes keep their units, which put lit interiors where IW3 lightmaps put them. In the 8-bit IW3
	// colours (probes, light-grid colours) a soft knee above 0.5 keeps the light of lamps from clipping; the lightmap
	// layers keep the whole range (lightmap_level).
	float light_level(const float value)
	{
		return value <= 0.5f ? value : 0.5f + 0.5f * (1.0f - std::exp((0.5f - value) * 2.0f));
	}

	float lightmap_level(const float value)
	{
		const auto clamped = std::clamp(std::isfinite(value) ? value : 0.0f, 0.0f, lightmap_maximum);
		return std::log2(1.0f + lightmap_log_scale * clamped) / std::log2(1.0f + lightmap_log_scale * lightmap_maximum);
	}

	// An IW7 lightmap keeps IW3's two layers as high dynamic range colours (its secondary image holds layer A over
	// layer B) and moves their directions to a third image: octahedral (x = (r + g) - 1, y = r - g, z = 1 - |x| -
	// |y| from the 2 * rg - 1 of a texel), in the tangent space of the surface. Its primary image holds the sun
	// visibility as a signed field (2 * r - 1, the edge at 0). Both games light a texel with the layers as
	// A + B * saturate(dot(normal, direction)); IW3 takes A * normal.z, which is the same for an unmapped normal.
	world_lightmaps export_lightmaps(const source_map& source, extraction& output)
	{
		const auto& world = *source.world();
		world_lightmaps result;
		for (auto i = 0u; i < world.draw.lightMapCount; i++)
		{
			const auto lightmap = source.resolve(iw7::ASSET_TYPE_GFXLIGHTMAP, world.draw.lightMaps[i]);
			if (!lightmap)
			{
				break;
			}

			auto primary = load(source, lightmap->textures[0], DXGI_FORMAT_R8G8B8A8_UNORM);
			auto secondary = load(source, lightmap->textures[1], DXGI_FORMAT_R32G32B32A32_FLOAT);
			auto directions = load(source, lightmap->textures[2], DXGI_FORMAT_R8G8B8A8_UNORM);
			if (!primary || !secondary || !directions)
			{
				break;
			}

			const auto width = secondary->GetMetadata().width;
			const auto height = secondary->GetMetadata().height / 2;
			if (!width || !height || !fit(*primary, width, height) || !fit(*directions, width, height))
			{
				ZONETOOL_WARNING("iw7: lightmap %u has images that do not match", i);
				break;
			}

			const auto layers = texels<float>(*secondary, 4);
			const auto direction_texels = texels<std::uint8_t>(*directions, 4);
			const auto primary_texels = texels<std::uint8_t>(*primary, 4);
			const auto count = width * height;
			std::vector<std::uint8_t> visibility(count);
			std::vector<std::uint8_t> two_layers(count * 8);
			auto& page = result.pages.emplace_back();
			page.width = width;
			page.height = height;
			page.ambient.resize(count);
			for (std::size_t t = 0; t < count; t++)
			{
				// The sun visibility: lit above the edge of the signed field, with the softness of a few texels.
				const auto field = primary_texels[t * 4] / 255.0f * 2.0f - 1.0f;
				const auto lit = std::clamp(0.5f + field * 4.0f, 0.0f, 1.0f);
				visibility[t] = encode_unit(lit * lit * (3.0f - 2.0f * lit));

				const auto encoded_x = direction_texels[t * 4] / 255.0f * 2.0f - 1.0f;
				const auto encoded_y = direction_texels[t * 4 + 1] / 255.0f * 2.0f - 1.0f;
				const auto x = (encoded_x + encoded_y) * 0.5f;
				const auto y = (encoded_x - encoded_y) * 0.5f;
				// IW3 slopes reach about 2: the direction stays within 63 degrees of the normal.
				const auto z = std::max(1.0f - std::abs(x) - std::abs(y), 0.45f * std::sqrt(x * x + y * y));
				const std::array<std::uint8_t, 2> slopes{encode_slope(z > 0.0f ? x / z : 0.0f, 4.07999992f, 2.07999992f),
					encode_slope(z > 0.0f ? y / z : 0.0f, 4.06451607f, 2.06451607f)};

				for (auto channel = 0; channel < 3; channel++)
				{
					page.ambient[t][channel] = layers[t * 4 + channel] + layers[(count + t) * 4 + channel] * 0.5f;
				}

				// The layers: blue, green, red and alpha bytes, layer A over layer B.
				for (auto layer = 0u; layer < 2; layer++)
				{
					const auto* color = layers.data() + (layer * count + t) * 4;
					auto* target = two_layers.data() + (layer * count + t) * 4;
					target[0] = encode_unit(lightmap_level(color[2]));
					target[1] = encode_unit(lightmap_level(color[1]));
					target[2] = encode_unit(lightmap_level(color[0]));
					target[3] = slopes[layer];
				}
			}

			const auto stem = bsp_name(source) + std::format(".lightmap_{}", i);
			write_file(output.extracted / (stem + "_0.bin"), visibility);
			write_file(output.extracted / (stem + "_1.bin"), two_layers);
			page.visibility = visibility;
			result.entries.push_back(json::array(
			{
				{{"bytes", visibility.size()}, {"file", stem + "_0.bin"}, {"format", format_l8}, {"height", height}, {"levels", 1},
					{"width", width}},
				{{"bytes", two_layers.size()}, {"file", stem + "_1.bin"}, {"format", format_a8r8g8b8}, {"height", height * 2},
					{"levels", 1}, {"width", width}},
			}));

			// How much of the lightmap the knee compresses (the brightest channel of each layer A texel).
			std::array<std::size_t, 3> above{};
			auto brightest = 0.0f;
			for (std::size_t t = 0; t < count; t++)
			{
				const auto value = std::max({layers[t * 4], layers[t * 4 + 1], layers[t * 4 + 2]});
				brightest = std::isfinite(value) ? std::max(brightest, value) : brightest;
				above[0] += value > 0.5f;
				above[1] += value > 1.0f;
				above[2] += value > 2.0f;
			}

			ZONETOOL_INFO("iw7: lightmap %u: %zux%zu (layer A over 0.5: %.2f%%, over 1: %.2f%%, over 2: %.2f%%, brightest %g)", i, width,
				height, 100.0 * above[0] / count, 100.0 * above[1] / count, 100.0 * above[2] / count, brightest);
		}

		if (result.entries.size() != world.draw.lightMapCount)
		{
			ZONETOOL_WARNING("iw7: %zu of %u lightmaps convert; surfaces of the others have no lightmap", result.entries.size(),
				world.draw.lightMapCount);
		}

		return result;
	}
}
