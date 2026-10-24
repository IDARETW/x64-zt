#include <std_include.hpp>
#include "export.hpp"
#include "images.hpp"

#include <execution>
#include <utils/compression.hpp>

namespace zonetool::convert::iw7_iw8
{
	namespace
	{
		// The atlas of the map pipeline draws 256-pixel tiles, and the pipeline keeps model images as 8-bit colours
		// with mip levels: 512 pixels keep the map zone at the size of a converted IW3 map (Frontier: 1.1 GB of model
		// images, against 3 GB at 1024).
		constexpr std::size_t max_image_size = 512;
		// The colour maps of world materials keep up to 1024 texels: native world materials sample them at their own
		// size, where the ground fills the view.
		constexpr std::size_t world_color_size = 1024;
		// The minimap and the loading screen are single 2D images: they keep IW7's size (1024 minimaps).
		constexpr std::size_t hud_image_size = 4096;

		enum class image_role
		{
			color,
			normal,
			specular,
			count,
		};

		constexpr auto image_role_count = static_cast<std::size_t>(image_role::count);

		// An image file of the extraction: the image that it holds, and whether it was written. IW7 colour maps of
		// TS_COLOR_SPECULAR_MAP hold the specular reflectance in their alpha, and the alpha of the surface in a texture
		// of TS_ALPHA_REVEAL_THICKNESS_MAP: their files take the alpha of that texture's channel, else none (opaque).
		struct image_file
		{
			const iw7::GfxImage* image{};
			image_role role{};
			const iw7::GfxImage* alpha{};
			std::size_t alpha_channel{};
			// A second channel of the alpha's image that the alpha is multiplied by (a reveal layer's own alpha).
			std::optional<std::size_t> alpha_factor_channel;
			bool opaque{};
			// A specular map made of the colour's alpha (IW7's F0) and the gloss in the red of a packed normal map.
			bool derived_specular{};
			const iw7::GfxImage* gloss{};
			std::optional<bool> written;
			// The written colour's alpha is 1 everywhere.
			bool opaque_alpha{};
			// The largest size that the file keeps (HUD images keep theirs).
			std::size_t limit{max_image_size};
		};

		// FNV-1a: a short name for a combination of images, the same in each run.
		std::uint32_t name_hash(const std::string_view name)
		{
			std::uint32_t hash = 0x811C9DC5;
			for (const auto c : name)
			{
				hash = (hash ^ static_cast<std::uint8_t>(c)) * 0x01000193;
			}

			return hash;
		}

		// The channel of a map in a texture of TS_ALPHA_REVEAL_THICKNESS_MAP: alpha in red, reveal in green, thickness in
		// blue, the ones it holds named after "_packed_" ("a", "r", "t"); the others keep their defaults (Afghan's
		// rock_wall_mars_01_c_3411a269_packed_r, BC1: red and blue 255, the reveal in green, as IW7's layered pixel shader
		// reads it).
		std::optional<std::size_t> packed_channel(const std::string_view image, const char map)
		{
			const auto packed = image.rfind("_packed_");
			if (packed == std::string_view::npos || image.substr(packed + 8).find(map) == std::string_view::npos)
			{
				return {};
			}

			return map == 'a' ? 0u : map == 'r' ? 1u : 2u;
		}

		std::optional<std::size_t> alpha_channel(const std::string_view image)
		{
			return packed_channel(image, 'a');
		}

		// The pixels of an image at one size, with the mip levels below it.
		struct image_pixels
		{
			std::vector<std::uint8_t> data;
			std::size_t width{};
			std::size_t height{};
		};

		// The reads of a game file source that cannot read on several threads at once, and the paks found missing.
		std::mutex read_mutex;

		// The role of a texture, by its semantic and the first character of its shader name (colorMap, normalMap,
		// specularMap; a colour semantic can also hold a reveal or emissive map).
		std::optional<image_role> texture_role(const iw7::MaterialTextureDef& texture)
		{
			switch (texture.semantic)
			{
			case iw7::TS_COLOR_MAP:
			case iw7::TS_COLOR_SPECULAR_MAP:
				return texture.nameStart == 'c' ? std::optional(image_role::color) : std::nullopt;
			case iw7::TS_NORMAL_MAP:
			case iw7::TS_NORMAL_OCCLUSSION_GLOSS_MAP:
				return texture.nameStart == 'n' ? std::optional(image_role::normal) : std::nullopt;
			case iw7::TS_SPECULAR_MAP:
				return texture.nameStart == 's' ? std::optional(image_role::specular) : std::nullopt;
			default:
				return {};
			}
		}

		// The layer of a texture of a layered material: the digit that ends its shader name (colorMap1), else 0.
		int texture_layer(const iw7::MaterialTextureDef& texture)
		{
			return texture.nameEnd >= '1' && texture.nameEnd <= '3' ? texture.nameEnd - '0' : 0;
		}

		// The engine images that the pipeline makes itself: a zone holds them without pixels.
		std::optional<std::string> builtin_image(const std::string_view name, const image_role role)
		{
			if (name == "$white" || name == "$black" || name == "$gray" || name == "$identitynormalmap")
			{
				return std::string(name);
			}

			if (name == "flat_nml" && role == image_role::normal)
			{
				return "$identitynormalmap";
			}

			return {};
		}

		// The images that streamed from a pak file that the game folder lacks, by pak (the owner's install misses some).
		std::map<std::string, std::set<std::string>> missing_paks;

		// The resident pixels of an image, or the largest stream that the atlas can use.
		std::optional<image_pixels> read_pixels(const iw7::GfxImage& image, const std::size_t limit)
		{
			if (!image.streamed)
			{
				if (!image.pixelData)
				{
					return {};
				}

				return image_pixels{{image.pixelData, image.pixelData + image.dataLen1}, image.width, image.height};
			}

			// The streams hold the image at growing sizes: the largest one that the atlas can use decodes fastest.
			const auto files = get_image_stream_files(&image);
			auto first = static_cast<int>(files.size()) - 1;
			while (first > 0 && std::max<std::size_t>(image.streams[first].width, image.streams[first].height) > limit &&
				image.streams[first - 1].width)
			{
				first--;
			}

			for (auto i = first; i >= 0; i--)
			{
				const auto& file = files[i];
				if (!file.offset || file.offsetEnd <= file.offset)
				{
					continue;
				}

				std::string compressed(file.offsetEnd - file.offset, '\0');
				const auto pak = std::format("imagefile{}.pak", file.fileIndex);
				{
					std::unique_lock lock(read_mutex, std::defer_lock);
					if (!game_files::get().concurrent_reads())
					{
						lock.lock();
					}

					if (!game_files::get().read(pak, file.offset, compressed.size(), compressed.data()))
					{
						if (!lock.owns_lock())
						{
							lock.lock();
						}

						auto& images = missing_paks[pak];
						if (images.empty())
						{
							ZONETOOL_WARNING("iw7: streamed images need %s", pak.data());
						}

						images.insert(image.name ? image.name : "?");
						continue;
					}
				}

				auto pixels = iw7::decompress_lz4_block(compressed.data() + sizeof(iw7::XFileCompressorHeader),
					compressed.size() - sizeof(iw7::XFileCompressorHeader));
				if (!pixels.empty())
				{
					// Some images' stream tables leave the top stream's size zero, and their own size 1x1 (Prime's
					// cosmetic_golden_gloves: 1024x1024 BC7 in stream 3 of 4, 512x512 in stream 2): each stream doubles the
					// size of the one below it.
					std::size_t width = image.streams[i].width;
					std::size_t height = image.streams[i].height;
					for (auto below = i - 1; (!width || !height) && below >= 0; below--)
					{
						if (image.streams[below].width && image.streams[below].height)
						{
							width = static_cast<std::size_t>(image.streams[below].width) << (i - below);
							height = static_cast<std::size_t>(image.streams[below].height) << (i - below);
						}
					}

					return image_pixels{std::move(pixels), width, height};
				}
			}

			return {};
		}

		// The top level of each face of an image (one, the six of a cube map, or six for each cube map of an array) in a
		// format that DirectXTex processes, at most `limit` pixels.
		std::vector<DirectX::ScratchImage> load_faces(const iw7::GfxImage& image, const DXGI_FORMAT format,
			const std::size_t limit = max_image_size)
		{
			// A cube map array holds the six faces of each of its cube maps in turn.
			const auto faces = image.mapType == iw7::MAPTYPE_CUBE ? 6u
				: image.mapType == iw7::MAPTYPE_CUBE_ARRAY ? 6u * std::max<unsigned int>(image.numElements, 1) : 1u;
			if (image.mapType != iw7::MAPTYPE_2D && image.mapType != iw7::MAPTYPE_CUBE && image.mapType != iw7::MAPTYPE_CUBE_ARRAY)
			{
				return {};
			}

			auto pixels = read_pixels(image, limit);
			if (!pixels || pixels->data.empty() || !pixels->width || !pixels->height)
			{
				return {};
			}

			// Each face holds its mip levels after its top level, as the resident pixels of an image do.
			const auto stride = pixels->data.size() / faces;
			std::size_t row_pitch{};
			std::size_t slice_pitch{};
			if (FAILED(DirectX::ComputePitch(image.imageFormat, pixels->width, pixels->height, row_pitch, slice_pitch)) ||
				slice_pitch > stride)
			{
				return {};
			}

			std::vector<DirectX::ScratchImage> result;
			for (auto face = 0u; face < faces; face++)
			{
				DirectX::Image source{};
				source.width = pixels->width;
				source.height = pixels->height;
				source.format = image.imageFormat;
				source.rowPitch = row_pitch;
				source.slicePitch = slice_pitch;
				source.pixels = pixels->data.data() + face * stride;

				// DirectXTex does not convert an image to its own format (E_INVALIDARG: Scorch's RGBA8 LED dots and heat
				// shimmer): those copy.
				DirectX::ScratchImage decoded;
				const auto decoded_result = source.format == format ? decoded.InitializeFromImage(source)
					: DirectX::IsCompressed(source.format) ? DirectX::Decompress(source, format, decoded)
					: DirectX::Convert(source, format, DirectX::TEX_FILTER_FORCE_NON_WIC, DirectX::TEX_THRESHOLD_DEFAULT, decoded);
				if (FAILED(decoded_result))
				{
					ZONETOOL_WARNING("iw7: image \"%s\" does not decode (format %d, HRESULT 0x%08X)", image.name, image.imageFormat,
						static_cast<unsigned int>(decoded_result));
					return {};
				}

				while (decoded.GetMetadata().width > limit || decoded.GetMetadata().height > limit)
				{
					const auto& metadata = decoded.GetMetadata();
					DirectX::ScratchImage smaller;
					// WIC filters need COM, which the tool does not start.
					if (const auto resized = DirectX::Resize(*decoded.GetImage(0, 0, 0), std::max<std::size_t>(1, metadata.width / 2),
						std::max<std::size_t>(1, metadata.height / 2), DirectX::TEX_FILTER_BOX | DirectX::TEX_FILTER_FORCE_NON_WIC, smaller);
						FAILED(resized))
					{
						ZONETOOL_WARNING("iw7: image \"%s\" does not resize (HRESULT 0x%08X)", image.name, static_cast<unsigned int>(resized));
						return {};
					}

					decoded = std::move(smaller);
				}

				result.push_back(std::move(decoded));
			}

			return result;
		}

		// IW3 normal maps hold the slopes x/z and y/z, which its shaders decode as xy * 4.08 - 2.08. IW7 normal maps keep x
		// in alpha and y in green (packed ones keep the gloss in red and the occlusion in blue): read as x, the gloss
		// tilted every normal of a map by its own amount (Frontier's floors by about 30 degrees, noisy reflections).
		void encode_normal_slopes(DirectX::ScratchImage& image)
		{
			const auto& metadata = image.GetMetadata();
			auto* pixels = image.GetPixels();
			for (std::size_t i = 0; i < metadata.width * metadata.height; i++)
			{
				auto* pixel = pixels + i * 4;
				const auto x = pixel[3] / 255.0f * 2.0f - 1.0f;
				const auto y = pixel[1] / 255.0f * 2.0f - 1.0f;
				const auto z = std::max(std::sqrt(std::max(0.0f, 1.0f - x * x - y * y)), 0.25f);
				const auto encode = [](const float slope, const float scale, const float bias)
				{
					return static_cast<std::uint8_t>(std::clamp(std::lround((slope + bias) / scale * 255.0f), 0l, 255l));
				};

				pixel[0] = encode(x / z, 4.07999992f, 2.07999992f);
				pixel[1] = encode(y / z, 4.06451607f, 2.06451607f);
				pixel[2] = 255;
				pixel[3] = 255;
			}
		}

		// An IW3 specular map of an IW7 colour map of TS_COLOR_SPECULAR_MAP: its alpha is the linear reflectance at normal
		// incidence (F0 x 255: plastics and tiles 10, floors 5, metals 250, chrome 125). IW3 specular maps hold the reflectance
		// gamma encoded in RGB (the pipeline decodes it to Replay's specular intensity) and the gloss in alpha, which is the
		// red of IW7's packed normal maps (128 without one). Without it every converted surface was matte and glass, which
		// shows only by its reflection, was missing (Frontier's patterned roof glass).
		bool derive_specular(DirectX::ScratchImage& pixels, const iw7::GfxImage* gloss)
		{
			const auto& metadata = pixels.GetMetadata();
			DirectX::ScratchImage resized;
			const DirectX::Image* source{};
			std::vector<DirectX::ScratchImage> faces;
			if (gloss)
			{
				faces = load_faces(*gloss, DXGI_FORMAT_R8G8B8A8_UNORM);
				if (faces.size() == 1)
				{
					source = faces.front().GetImage(0, 0, 0);
					if (source->width != metadata.width || source->height != metadata.height)
					{
						if (FAILED(DirectX::Resize(*source, metadata.width, metadata.height, DirectX::TEX_FILTER_LINEAR |
							DirectX::TEX_FILTER_FORCE_NON_WIC, resized)))
						{
							return false;
						}

						source = resized.GetImage(0, 0, 0);
					}
				}
				else
				{
					ZONETOOL_WARNING("iw7: gloss image \"%s\" cannot be read: %s", gloss->name, image_read_failure(*gloss).data());
				}
			}

			static const auto encoded = []
			{
				std::array<std::uint8_t, 256> table{};
				for (std::size_t value = 0; value < table.size(); value++)
				{
					const auto linear = static_cast<float>(value) / 255.0f;
					const auto srgb = linear <= 0.0031308f ? linear * 12.92f : 1.055f * std::pow(linear, 1.0f / 2.4f) - 0.055f;
					table[value] = static_cast<std::uint8_t>(std::clamp(std::lround(srgb * 255.0f), 0l, 255l));
				}

				return table;
			}();

			const auto* target = pixels.GetImage(0, 0, 0);
			for (std::size_t y = 0; y < metadata.height; y++)
			{
				auto* row = target->pixels + y * target->rowPitch;
				const auto* gloss_row = source ? source->pixels + y * source->rowPitch : nullptr;
				for (std::size_t x = 0; x < metadata.width; x++)
				{
					auto* pixel = row + x * 4;
					pixel[0] = pixel[1] = pixel[2] = encoded[pixel[3]];
					pixel[3] = gloss_row ? gloss_row[x * 4] : 128;
				}
			}

			return true;
		}

		// Replaces the alpha of a colour image: with the channel of an alpha texture (at the colour's size), or opaque.
		// The alpha of an image from a channel of another (none: opaque), times a second channel of it when one is given.
		bool replace_alpha(DirectX::ScratchImage& pixels, const iw7::GfxImage* alpha, const std::size_t channel,
			const std::optional<std::size_t> factor = std::nullopt)
		{
			const auto& metadata = pixels.GetMetadata();
			DirectX::ScratchImage resized;
			const DirectX::Image* source{};
			std::vector<DirectX::ScratchImage> faces;
			if (alpha)
			{
				faces = load_faces(*alpha, DXGI_FORMAT_R8G8B8A8_UNORM);
				if (faces.size() != 1)
				{
					ZONETOOL_WARNING("iw7: alpha image \"%s\" cannot be read: %s", alpha->name, image_read_failure(*alpha).data());
					return false;
				}

				source = faces.front().GetImage(0, 0, 0);
				if (source->width != metadata.width || source->height != metadata.height)
				{
					if (FAILED(DirectX::Resize(*source, metadata.width, metadata.height, DirectX::TEX_FILTER_LINEAR |
						DirectX::TEX_FILTER_FORCE_NON_WIC, resized)))
					{
						return false;
					}

					source = resized.GetImage(0, 0, 0);
				}
			}

			const auto* target = pixels.GetImage(0, 0, 0);
			for (std::size_t y = 0; y < metadata.height; y++)
			{
				auto* row = target->pixels + y * target->rowPitch;
				const auto* alpha_row = source ? source->pixels + y * source->rowPitch : nullptr;
				for (std::size_t x = 0; x < metadata.width; x++)
				{
					row[x * 4 + 3] = !alpha_row ? 0xFF : factor
						? static_cast<std::uint8_t>((alpha_row[x * 4 + channel] * alpha_row[x * 4 + *factor] + 127) / 255)
						: alpha_row[x * 4 + channel];
				}
			}

			return true;
		}

		// Writes an image as the DDS file that the pipeline reads.
		bool write_image(image_file& file, const std::filesystem::path& path)
		{
			const auto& image = *file.image;
			const auto role = file.role;
			auto faces = load_faces(image, DXGI_FORMAT_R8G8B8A8_UNORM, file.limit);
			if (faces.size() != 1)
			{
				ZONETOOL_WARNING("iw7: image \"%s\" cannot be read: %s", image.name, image_read_failure(image).data());
				return false;
			}

			auto& pixels = faces.front();
			if (file.derived_specular && !derive_specular(pixels, file.gloss))
			{
				return false;
			}

			if (role == image_role::normal)
			{
				encode_normal_slopes(pixels);
			}
			else if ((file.alpha || file.opaque) && !replace_alpha(pixels, file.alpha, file.alpha_channel, file.alpha_factor_channel))
			{
				return false;
			}

			// The pipeline decodes the files and encodes the zone's images itself: block compression keeps the
			// extraction small. A normal map stays uncompressed (its slopes would band in BC1 colours) at half the size.
			auto top = pixels.GetImage(0, 0, 0);
			DirectX::ScratchImage smaller;
			if (role == image_role::normal && std::max(top->width, top->height) > max_image_size / 2)
			{
				if (FAILED(DirectX::Resize(*top, std::max<std::size_t>(1, top->width / 2), std::max<std::size_t>(1, top->height / 2),
					DirectX::TEX_FILTER_BOX | DirectX::TEX_FILTER_FORCE_NON_WIC, smaller)))
				{
					return false;
				}

				top = smaller.GetImage(0, 0, 0);
			}

			// The box filter takes only sizes of powers of two (the map art of the map select is 16:9): other sizes filter
			// linearly.
			DirectX::ScratchImage mips;
			if (FAILED(DirectX::GenerateMipMaps(*top, DirectX::TEX_FILTER_BOX | DirectX::TEX_FILTER_FORCE_NON_WIC, 0, mips)) &&
				FAILED(DirectX::GenerateMipMaps(*top, DirectX::TEX_FILTER_LINEAR | DirectX::TEX_FILTER_FORCE_NON_WIC, 0, mips)))
			{
				ZONETOOL_WARNING("iw7: image \"%s\" has no mip chain", image.name);
				return false;
			}

			DirectX::ScratchImage stored;
			if (role == image_role::normal)
			{
				// The A8R8G8B8 masks: blue, green, red and alpha bytes.
				if (FAILED(DirectX::Convert(mips.GetImages(), mips.GetImageCount(), mips.GetMetadata(), DXGI_FORMAT_B8G8R8A8_UNORM,
					DirectX::TEX_FILTER_FORCE_NON_WIC, DirectX::TEX_THRESHOLD_DEFAULT, stored)))
				{
					return false;
				}
			}
			else
			{
				// DirectXTex compresses on several threads only with OpenMP, which the build leaves out: the images
				// convert on several threads instead.
				file.opaque_alpha = mips.IsAlphaAllOpaque();
				const auto format = file.opaque_alpha ? DXGI_FORMAT_BC1_UNORM : DXGI_FORMAT_BC3_UNORM;
				if (const auto compressed = DirectX::Compress(mips.GetImages(), mips.GetImageCount(), mips.GetMetadata(), format,
					DirectX::TEX_COMPRESS_DEFAULT, DirectX::TEX_THRESHOLD_DEFAULT, stored); FAILED(compressed))
				{
					ZONETOOL_WARNING("iw7: image \"%s\" does not compress (HRESULT 0x%08X)", image.name,
						static_cast<unsigned int>(compressed));
					return false;
				}
			}

			if (FAILED(DirectX::SaveToDDSFile(stored.GetImages(), stored.GetImageCount(), stored.GetMetadata(),
				DirectX::DDS_FLAGS_FORCE_DX9_LEGACY, path.wstring().data())))
			{
				ZONETOOL_WARNING("iw7: cannot write %s", path.string().data());
				return false;
			}

			return true;
		}

		json texture_json(const std::string& semantic, const std::string& image)
		{
			return
			{
				{"name", semantic},
				{"semantic", semantic},
				{"image", image},
				{"samplerState", {{"filter", "aniso4x"}, {"mipMap", "linear"}, {"clampU", false}, {"clampV", false},
					{"clampW", false}}},
			};
		}

		// The state of a pass: the depth pass writes no colour; the colour pass blends as the material draws.
		json state_json(const std::string& cull, const bool color, const surface_kind kind, const bool decal_offset = false)
		{
			const auto blended = color && kind != surface_kind::opaque && kind != surface_kind::alpha_test;
			const auto [source, destination] = !blended ? std::pair{"one", "zero"}
				: kind == surface_kind::additive ? std::pair{"one", "one"}
				: kind == surface_kind::multiply ? std::pair{"zero", "srccolor"}
				: std::pair{"srcalpha", "invsrcalpha"};
			return
			{
				{"alphaTest", kind == surface_kind::alpha_test ? "ge128" : "disabled"},
				{"blendOpAlpha", blended ? "add" : "disabled"},
				{"blendOpRgb", blended ? "add" : "disabled"},
				{"colorWriteAlpha", color},
				{"colorWriteRgb", color},
				{"cullFace", cull},
				{"depthTest", "less_equal"},
				{"depthWrite", !blended},
				{"dstBlendAlpha", blended ? destination : "zero"},
				{"dstBlendRgb", destination},
				{"polygonOffset", decal_offset && color ? "offset1" : "offset0"},
				{"polymodeLine", false},
				{"srcBlendAlpha", blended ? source : "one"},
				{"srcBlendRgb", source},
			};
		}

		// The IW3 technique set that the pipeline reads the same way: "unlit" emits the colour map, b0 blends, t0
		// tests alpha, "falloff" fades edge on; sky surfaces draw nothing (the pipeline adds the sky box).
		std::string iw3_technique_set(const material_class& kind)
		{
			if (kind.kind == surface_kind::sky)
			{
				return "sky";
			}

			if (kind.unlit)
			{
				const auto mode = kind.kind == surface_kind::blend ? "blend" : kind.kind == surface_kind::additive ? "add"
					: kind.kind == surface_kind::multiply ? "multiply" : "replace";
				return std::format("wc_unlit{}_{}", kind.falloff ? "_falloff" : "", mode);
			}

			return kind.kind == surface_kind::blend ? "wc_l_sm_b0c0n0s0" : kind.kind == surface_kind::alpha_test ? "wc_l_sm_t0c0n0s0"
				: "wc_l_sm_r0c0n0s0";
		}

		// IW7 sorts its opaque surfaces at 2 and its decals and effects from 7, in the order that IW3 sorts them from 9.
		unsigned int iw3_sort(const iw7::Material& material, const material_class& kind)
		{
			const unsigned int sort = material.info.sortKey;
			return kind.kind == surface_kind::opaque || sort < 7 ? 4 : std::min(sort + 2, 63u);
		}

		// Whether IW7 draws a material with a depth bias over the surface beneath it: the polygon offset (1 or 2; 3 is the
		// shadow maps') of the state of its emissive technique (8) or lit ones (13 to 26: the lprobe techniques). Breakneck:
		// 258 of the decal camera region's 282 materials have it, and none of the other 5408.
		bool iw7_decal_offset(const iw7::Material* material)
		{
			if (!material || !material->stateBitsTable)
			{
				return false;
			}

			for (auto technique = 8; technique <= 26; technique++)
			{
				const auto state = material->stateBitsEntry[technique];
				if ((technique == 8 || technique >= 13) && state != 0xFF && state < material->stateBitsCount)
				{
					const auto offset = (material->stateBitsTable[state].loadBits[1] & iw7::GFXS1_POLYGON_OFFSET_MASK) >>
						iw7::GFXS1_POLYGON_OFFSET_SHIFT;
					if (offset == 1 || offset == 2)
					{
						return true;
					}
				}
			}

			return false;
		}

		// Whether a drawn technique of a material (unlit 8, emissive 10, lit 13 to 26) has no depth test: IW7's nodepth glows
		// (Scorch's lava glare), which draw over the world while their occlusion query sees their source.
		bool iw7_depthless(const iw7::Material* material)
		{
			if (!material || !material->stateBitsTable)
			{
				return false;
			}

			for (auto technique = 8; technique <= 26; technique++)
			{
				const auto state = material->stateBitsEntry[technique];
				if ((technique == 8 || technique == 10 || technique >= 13) && state != 0xFF && state < material->stateBitsCount)
				{
					if (material->stateBitsTable[state].loadBits[1] & iw7::GFXS1_DEPTHTEST_DISABLE)
					{
						return true;
					}
				}
			}

			return false;
		}

		// The value of a constant of a material (names keep 12 characters).
		std::optional<std::array<float, 4>> material_constant(const iw7::Material* material, const std::string_view name)
		{
			for (auto i = 0; material && i < material->constantCount; i++)
			{
				const auto& constant = material->constantTable[i];
				if (std::string_view(constant.name, strnlen(constant.name, sizeof(constant.name))) == name.substr(0, sizeof(constant.name)))
				{
					// Unused components can hold NaN, which JSON cannot.
					std::array<float, 4> value{};
					for (std::size_t c = 0; c < 4; c++)
					{
						value[c] = std::isfinite(constant.literal[c]) ? constant.literal[c] : 0.0f;
					}

					return value;
				}
			}

			return std::nullopt;
		}

		// The tint of a layer of a material (colorTint, colorTint1 to colorTint3): lit materials keep albedo tints up to
		// 1, unlit ones (high dynamic range emission) the hue of theirs.
		std::array<float, 4> material_tint(const iw7::Material* material, const int layer, const material_class& kind)
		{
			const auto name = layer ? std::format("colorTint{}", layer) : std::string("colorTint");
			auto tint = material_constant(material, name).value_or(std::array{1.0f, 1.0f, 1.0f, 1.0f});
			const auto peak = std::max({tint[0], tint[1], tint[2]});
			for (std::size_t c = 0; c < 3; c++)
			{
				tint[c] = std::clamp(kind.unlit && peak > 1.0f ? tint[c] / peak : tint[c], 0.0f, 1.0f);
			}

			tint[3] = 1.0f;
			return tint;
		}

		// The IW3 material of a material, with the images of its roles (the pipeline's defaults for the missing ones).
		json export_material(const iw7::Material* material,
			const std::array<std::optional<std::string>, image_role_count>& images, const material_class& kind, const int layer,
			const float light_scale)
		{
			// An unlit material emits its tint in the units of the converted lights (the sun's level is 1): one that emits
			// below it keeps the tint, one above it the hue of its tint and gives the pipeline its gain (Frontier's lamps:
			// tint 318, 33 times the sun). Unlit overlays (screen dots over screens of tint 15) then keep IW7's balance.
			auto constants = json::array({{{"name", "colorTint"}, {"literal", material_tint(material, layer, kind)}}});
			if (kind.unlit)
			{
				const auto tint = material_color_tint(material);
				const auto peak = std::max({tint[0], tint[1], tint[2], 0.0f});
				const auto emission = peak * light_scale;
				std::array<float, 4> color{1.0f, 1.0f, 1.0f, 1.0f};
				for (std::size_t c = 0; c < 3; c++)
				{
					color[c] = peak > 0.0f ? std::clamp(tint[c] / peak * std::min(emission, 1.0f), 0.0f, 1.0f) : 0.0f;
				}

				constants[0]["literal"] = color;
				// The pipeline takes constants up to 64, far past white.
				if (emission > 1.0f)
				{
					constants.push_back({{"name", "emissionScale"}, {"literal", {std::min(emission, 64.0f), std::min(emission, 64.0f),
						std::min(emission, 64.0f), 1.0f}}});
				}

				// IW3's falloff is IW7's: f = saturate((N.V)^2 * falloffParms.z + falloffParms.w), colour * f * lerp(falloffEndColor,
				// falloffBeginColor, f). Without it the ops table's hologram planes (tint 17) drew as solid white discs.
				if (kind.falloff)
				{
					if (const auto parms = material_constant(material, "falloffParms"))
					{
						constants.push_back({{"name", "falloffParms"}, {"literal", *parms}});
						constants.push_back({{"name", "falloffBeginColor"},
							{"literal", material_constant(material, "falloffBeginColor").value_or(std::array{1.0f, 1.0f, 1.0f, 1.0f})}});
						constants.push_back({{"name", "falloffEndColor"},
							{"literal", material_constant(material, "falloffEndColor").value_or(std::array{1.0f, 1.0f, 1.0f, 1.0f})}});
					}
				}
			}

			std::string cull = "back";
			if (material)
			{
				cull = (material->stateFlags & iw7::STATE_FLAG_CULL_MASK) == iw7::STATE_FLAG_CULL_BACK ? "back"
					: (material->stateFlags & iw7::STATE_FLAG_CULL_MASK) == iw7::STATE_FLAG_CULL_FRONT ? "front" : "none";
			}

			auto textures = json::array();
			textures.push_back(texture_json("colorMap", images[static_cast<std::size_t>(image_role::color)].value_or("$white")));
			textures.push_back(texture_json("normalMap",
				images[static_cast<std::size_t>(image_role::normal)].value_or("$identitynormalmap")));
			textures.push_back(texture_json("specularMap", images[static_cast<std::size_t>(image_role::specular)].value_or("$black")));

			// The IW3 techniques: 4 unlit, 7 lit. An unlit material has no lit pass.
			auto entries = json::array();
			for (auto i = 0; i < 34; i++)
			{
				entries.push_back(kind.unlit ? (i == 4 ? 1 : -1) : (i == 7 ? 1 : i == 4 ? 0 : -1));
			}

			const auto shadow = !kind.unlit && kind.solid();
			// IW7 draws its decals (decal sheets, stripes, signs, scrapes) over the coplanar surface beneath with a depth
			// bias: the polygon offset of the material's lit or emissive technique state. Its lit pass takes IW3's decal
			// polygon offset, which the pipeline draws biased; the pipeline's rank nudge alone left the wayfinding signs
			// half hidden in the wall (owner test of 58aac89). The decal camera region (2) stood in for the state before,
			// and its opaque and unlit decals (stencils, lit signs) fought the wall under them. The layers of a layered
			// decal keep its bias over the surface beneath (their rank orders them over their own base): unbiased, they
			// fought the layers of that surface (Scorch's ground).
			const auto decal_offset = iw7_decal_offset(material);
			json result =
			{
				{"$schema", "http://openassettools.dev/schema/material.v1.json"},
				{"_game", "iw3"},
				{"_type", "material"},
				{"_version", 1},
				{"cameraRegion", "lit"},
				{"constants", std::move(constants)},
				{"gameFlags", shadow ? json::array({"CASTS_SHADOW"}) : json::array()},
				// A layer sorts as the first decals, in IW7's order (layer n at 8 + n): the pipeline pulls a coplanar surface
				// toward the camera by its rank (sort - 8), and layers of one rank swapped with the camera (Breakneck's
				// walls and containers showed one layer or another as the view moved).
				{"sortKey", layer ? 8u + static_cast<unsigned>(layer) : material ? iw3_sort(*material, kind) : 4u},
				{"stateBits", json::array({state_json(cull, false, kind.kind), state_json(cull, true, kind.kind, decal_offset)})},
				{"stateBitsEntry", std::move(entries)},
				{"stateFlags", 0},
				{"surfaceTypeBits", 0},
				{"techniqueSet", iw3_technique_set(kind)},
				{"textureAtlas", {{"columns", 1}, {"rows", 1}}},
				{"textures", std::move(textures)},
			};

			// Flag cloth and foliage move by IW7's flag wave (the pipeline's MAP_IW7_FLAG_* variants of static models).
			if (const auto wave = layer ? std::nullopt : flag_wave(material))
			{
				result["iw7FlagWave"] = wave->values;
				result["iw7FlagClassic"] = wave->classic;
			}

			// Reactive foliage bends by Replay's reactive-motion sim (the pipeline's MAP_REACTIVE_MOTION variant).
			if (!layer && reactive_motion(material))
			{
				result["iw7ReactiveMotion"] = true;
			}

			// IW7's lit pixel shaders negate the tangent frame on back faces (SV_IsFrontFace, the disassembled foliage
			// shader of mopw_l_sm_atest_*_ssvsc_trans): two-sided leaves and fences lit from behind keep their light.
			if (cull == "none")
			{
				result["iw7BackFaceNormals"] = true;
			}

			// A layer that blends by its reveal map ramps its colour's alpha (the reveal) by the weight and sharpness in its
			// vertex colours (export_surface).
			if (layer && material_layer_reveal(material, layer))
			{
				result["iw7RevealLayer"] = true;
			}

			return result;
		}
	}

	namespace
	{
		// An effect material as the pipeline reads IW3 effect materials: an emissive sprite material of one colour map in
		// the IW3 effect technique set of its blend (feathered, faded edge on), a cloud or a distortion. Without its colour
		// map (an image of a pak that is not there) it adds black: white drew the table hologram's dot grid of Frontier as a
		// white slab.
		// The tint of an unlit one (IW7's effectunlit techniques: HDR tints, Scorch's lava squibs 1500, spark streaks up to
		// 8000) takes the light unit of the map, as unlit world materials do: the pipeline scales effect tints into Replay's
		// units (Scorch x23.9) as it does the world, and with IW7's raw tints those effects drew 1 / light unit (Scorch: 24.5x)
		// too bright; their additive sprites saturated into white polygons across the screen (owner tests of cbf4948: Scorch
		// flicker). A lit one's tint (smoke, steam: 1) is a colour, which the pipeline treats as IW3's.
		json export_effect_material(const iw7::Material* material, const std::optional<std::string>& color, const float light_scale,
			const bool atlas)
		{
			std::vector<std::string_view> tokens;
			if (material && material->techniqueSet)
			{
				for (const auto token : std::views::split(asset_name(material->techniqueSet->name), '_'))
				{
					tokens.emplace_back(token.begin(), token.end());
				}
			}

			const auto has = [&](const std::string_view token)
			{
				return std::ranges::find(tokens, token) != tokens.end();
			};

			const auto additive = !color || has("add") || has("blendadd");
			const auto feather = has("zfeather");
			// IW7_FXMAT_DUMP=1: each effect material's technique set and constants.
			if (std::getenv("IW7_FXMAT_DUMP"))
			{
				std::string values;
				for (auto i = 0; material && i < material->constantCount; i++)
				{
					const auto& entry = material->constantTable[i];
					values += std::format(" {}=({:g} {:g} {:g} {:g})", std::string_view(entry.name, strnlen(entry.name, sizeof(entry.name))),
						entry.literal[0], entry.literal[1], entry.literal[2], entry.literal[3]);
				}

				std::string depths;
				for (auto technique = 0; material && material->stateBitsTable && technique < 34; technique++)
				{
					const auto state = material->stateBitsEntry[technique];
					if (state != 0xFF && state < material->stateBitsCount)
					{
						depths += std::format(" t{}:{:x}", technique, material->stateBitsTable[state].loadBits[1] &
							(iw7::GFXS1_DEPTHTEST_DISABLE | iw7::GFXS1_DEPTHTEST_MASK));
					}
				}

				ZONETOOL_INFO("iw7 effect material %s: techset %s%s depth%s", material && material->name ? material->name : "?",
					material && material->techniqueSet ? material->techniqueSet->name : "?", values.data(), depths.data());
			}

			auto constants = json::array();
			const auto constant = [&](const std::string_view iw3_name, const std::string_view iw7_name, const std::array<float, 4> fallback)
			{
				constants.push_back({{"name", iw3_name}, {"literal", material_constant(material, iw7_name).value_or(fallback)}});
			};

			std::string technique;
			auto clamped = false;
			if (has("distortion"))
			{
				technique = "distortion_scale_zfeather";
				constant("distortionScale", "distortionScale", {1.0f, 1.0f, 0.0f, 0.0f});
			}
			else if (has("cloud"))
			{
				// IW3 clouds have no constants and clamp their colour map.
				technique = additive ? "particle_cloud_add" : "particle_cloud";
				clamped = true;
			}
			else
			{
				const auto falloff = (has("falloff") || has("psfalloff")) && material_constant(material, "falloffParms");
				// IW7's eye offset (eyeOffsetParms.x: units toward the camera; Scorch's screen glows 15, its electric glows 5),
				// as IW3's eye-offset techniques take it.
				const auto eye_offset = has("eyeoffset") && material_constant(material, "eyeOffsetParms");
				technique = std::format("effect{}{}{}{}", feather ? "_zfeather" : "", falloff ? "_falloff" : "", additive ? "_add" : "",
					eye_offset ? "_eyeoffset" : "");
				auto tint = material_constant(material, "colorTint").value_or(std::array{1.0f, 1.0f, 1.0f, 1.0f});
				for (std::size_t c = 0; has("effectunlit") && c < 3; c++)
				{
					tint[c] *= light_scale;
				}

				constants.push_back({{"name", "colorTint"}, {"literal", tint}});
				if (feather)
				{
					auto parameters = material_constant(material, "featherParms").value_or(std::array{0.05f, 0.0f, 0.0f, 0.0f});
					parameters[0] = parameters[0] > 0.0f ? parameters[0] : 0.05f;
					constants.push_back({{"name", "featherParms"}, {"literal", parameters}});
				}

				if (falloff)
				{
					constant("falloffParms", "falloffParms", {0.0f, 0.0f, 0.0f, 0.0f});
					constant("falloffBeginColor", "falloffBeginColor", {1.0f, 1.0f, 1.0f, 1.0f});
					constant("falloffEndColor", "falloffEndColor", {1.0f, 1.0f, 1.0f, 1.0f});
				}

				if (eye_offset)
				{
					constant("eyeOffsetParms", "eyeOffsetParms", {0.0f, 0.0f, 0.0f, 0.0f});
				}
			}

			const auto [source, destination] = additive ? std::pair{"one", "one"} : std::pair{"srcalpha", "invsrcalpha"};
			const json state =
			{
				{"alphaTest", additive ? "disabled" : "gt0"},
				{"blendOpAlpha", "add"},
				{"blendOpRgb", "add"},
				{"colorWriteAlpha", false},
				{"colorWriteRgb", true},
				{"cullFace", "none"},
				{"depthTest", iw7_depthless(material) ? "always" : "less_equal"},
				{"depthWrite", false},
				{"dstBlendAlpha", destination},
				{"dstBlendRgb", destination},
				{"polygonOffset", "offset0"},
				{"polymodeLine", false},
				{"srcBlendAlpha", source},
				{"srcBlendRgb", source},
			};

			auto entries = json::array();
			for (auto i = 0; i < 34; i++)
			{
				entries.push_back(i == 4 ? 0 : -1);
			}

			// Without a particle that picks its cells (extraction::atlas_materials), the whole texture draws.
			const auto rows = material && atlas ? std::max<unsigned>(material->info.textureAtlasRowCount, 1) : 1u;
			const auto columns = material && atlas ? std::max<unsigned>(material->info.textureAtlasColumnCount, 1) : 1u;
			// IW7 blends between the atlas frames of these (textureAtlasFrameBlend; their technique sets carry "tab":
			// Frontier's fireballs, smoke billows and dust bursts), as Replay's frame-blend effect techniques do.
			const auto frame_blend = material && material->info.textureAtlasFrameBlend && std::has_single_bit(columns) &&
				rows * columns > 1;
			return
			{
				{"$schema", "http://openassettools.dev/schema/material.v1.json"},
				{"_game", "iw3"},
				{"_type", "material"},
				{"_version", 1},
				{"cameraRegion", "emissive"},
				{"constants", std::move(constants)},
				// IW7 lights the particles of its lit effect materials (not effectunlit) by the scene: the pipeline
				// draws the alpha-blended ones through Replay's lit effect technique.
				{"lit", !has("effectunlit") && !additive && !has("distortion") && !has("cloud")},
				{"frame_blend", frame_blend},
				{"gameFlags", json::array()},
				{"sortKey", material ? std::min(material->info.sortKey + 2u, 63u) : 26u},
				{"stateBits", json::array({state})},
				{"stateBitsEntry", std::move(entries)},
				{"stateFlags", 16},
				{"surfaceTypeBits", 0},
				{"techniqueSet", technique},
				{"textureAtlas", {{"columns", std::has_single_bit(columns) ? columns : 1u}, {"rows", std::has_single_bit(columns) ? rows : 1u}}},
				{"textures", json::array({
					{
						{"name", "colorMap"},
						{"semantic", "colorMap"},
						{"image", color.value_or("$black")},
						{"samplerState", {{"filter", "linear"}, {"mipMap", "linear"}, {"clampU", clamped}, {"clampV", clamped},
							{"clampW", clamped}}},
					}})},
			};
		}
	}

	material_class classify_material(const iw7::Material* material)
	{
		material_class result{};
		if (!material || !material->techniqueSet)
		{
			return result;
		}

		std::vector<std::string_view> tokens;
		for (const auto token : std::views::split(asset_name(material->techniqueSet->name), '_'))
		{
			tokens.emplace_back(token.begin(), token.end());
		}

		const auto has = [&](const std::string_view token)
		{
			return std::ranges::find(tokens, token) != tokens.end();
		};

		if (has("sky"))
		{
			result.kind = surface_kind::sky;
			return result;
		}

		if (has("distortion"))
		{
			result.kind = surface_kind::distortion;
			return result;
		}

		// "effectunlit": IW7's unlit effect shading of models (the ops table's planets, mo_effectunlit_falloff_add_...): as
		// lit additive surfaces they added only the dim room light, so the planets did not show (owner test of 58aac89).
		result.unlit = has("unlit") || has("effectunlit");
		// "psfalloff": the falloff of the pixel shader (the ops table's hologram planes).
		result.falloff = has("falloff") || has("psfalloff");
		for (const auto token : tokens)
		{
			if (token == "atest")
			{
				result.kind = surface_kind::alpha_test;
			}
			else if (token == "blend")
			{
				result.kind = surface_kind::blend;
			}
			else if (token == "add" || token == "blendadd")
			{
				// blendadd adds the colour over the alpha-darkened surface (one, inverse source alpha): additive is the
				// pipeline's nearest blend, unless the colour holds the alpha (export_materials).
				result.kind = surface_kind::additive;
				result.premultiplied = token == "blendadd";
			}
			else if (token == "multiply")
			{
				result.kind = surface_kind::multiply;
			}
			else if (!token.starts_with("replace"))
			{
				continue;
			}

			break;
		}

		return result;
	}

	std::array<float, 4> material_color_tint(const iw7::Material* material)
	{
		return material_constant(material, "colorTint").value_or(std::array{1.0f, 1.0f, 1.0f, 1.0f});
	}

	int material_layer_count(const iw7::Material* material)
	{
		if (!material || !material->techniqueSet)
		{
			return 1;
		}

		// Each layer is one token that opens with its blend (b, t, a or m) and its digit: Frontier's t1, a2 and m2 layers
		// were left out when only b counted, and their vertex data (vld) then fell out of step.
		auto layers = 1;
		for (auto layer = 1; layer <= 3; layer++)
		{
			layers = material_layer_token(material, layer).empty() ? layers : layer + 1;
		}

		return std::min(layers, 4);
	}

	std::string_view material_layer_token(const iw7::Material* material, const int layer)
	{
		if (!material || !material->techniqueSet || layer < 1 || layer > 3)
		{
			return {};
		}

		for (const auto token : std::views::split(asset_name(material->techniqueSet->name), '_'))
		{
			const std::string_view text(token.begin(), token.end());
			if (text.size() > 1 && text[0] >= 'a' && text[0] <= 'z' && text[1] == '0' + layer)
			{
				return text;
			}
		}

		return {};
	}

	unsigned int material_layer_vertex_size(const iw7::Material* material, const int layer)
	{
		const auto token = material_layer_token(material, layer);
		if (token.empty())
		{
			return 0;
		}

		const char digit = static_cast<char>('0' + layer);
		const auto has = [&](const std::string_view prefix)
		{
			return token.find(std::string(prefix) + digit) != std::string_view::npos;
		};

		return 8 + (has("nl") || has("nal") || has("nbl") ? 4 : 0);
	}

	std::optional<float> material_layer_reveal(const iw7::Material* material, const int layer)
	{
		const auto token = material_layer_token(material, layer);
		if (token.find(std::format("v{}", layer)) == std::string_view::npos)
		{
			return std::nullopt;
		}

		// revealParams<layer>.x (IW7's layered pixel shader reads only x), found by its name hash: the 13-character names
		// fill the 12 characters that a constant keeps. IW7's hashes of these names differ by the code of their last
		// character alone (colorTint1 to 3: 0x7793A24B, 0x7793A248, 0x7793A249), which places revealParams1 to 3 (Afghan
		// keeps all three) at 0x1963B944, 0x1963B947 and 0x1963B946. A material without the constant takes 1, the value
		// that every revealParams of Afghan holds.
		static constexpr std::array<std::uint32_t, 3> hashes{0x1963B944, 0x1963B947, 0x1963B946};
		for (auto i = 0; material && i < material->constantCount; i++)
		{
			const auto& constant = material->constantTable[i];
			if (constant.nameHash == hashes[layer - 1] && std::isfinite(constant.literal[0]))
			{
				return std::max(constant.literal[0], 0.0f);
			}
		}

		return 1.0f;
	}

	std::vector<DirectX::ScratchImage> load_image_faces(const iw7::GfxImage& image, const DXGI_FORMAT format,
		const std::size_t limit)
	{
		return load_faces(image, format, limit ? limit : max_image_size);
	}

	std::string image_read_failure(const iw7::GfxImage& image)
	{
		{
			const std::lock_guard lock(read_mutex);
			for (const auto& [pak, images] : missing_paks)
			{
				if (images.contains(image.name ? image.name : "?"))
				{
					return std::format("it streams from {}, which the game folder lacks", pak);
				}
			}
		}

		if (!image.streamed && !image.pixelData)
		{
			return "the loaded zones hold only its name";
		}

		return std::format("its pixels do not match format {}, map type {}", static_cast<int>(image.imageFormat),
			static_cast<int>(image.mapType));
	}

	const iw7::GfxImage* color_image(const source_map& source, const iw7::Material* material)
	{
		for (auto i = 0; material && i < material->textureCount; i++)
		{
			const auto& texture = material->textureTable[i];
			if (texture_role(texture) == image_role::color && !texture_layer(texture))
			{
				return source.resolve(iw7::ASSET_TYPE_IMAGE, texture.image);
			}
		}

		return nullptr;
	}

	std::string export_default_probe(extraction& output)
	{
		constexpr std::size_t size = 64;
		constexpr std::size_t levels = 7;
		DirectX::ScratchImage cube;
		if (FAILED(cube.InitializeCube(DXGI_FORMAT_B8G8R8A8_UNORM, size, size, 1, levels)))
		{
			throw std::runtime_error("cannot create the default reflection probe");
		}

		// A neutral grey environment: flat ambient and dim reflections until the IW7 probes convert.
		for (std::size_t i = 0; i < cube.GetImageCount(); i++)
		{
			const auto& image = cube.GetImages()[i];
			for (std::size_t y = 0; y < image.height; y++)
			{
				std::memset(image.pixels + y * image.rowPitch, 0x60, image.width * 4);
				for (std::size_t x = 0; x < image.width; x++)
				{
					image.pixels[y * image.rowPitch + x * 4 + 3] = 0xFF;
				}
			}
		}

		const std::string name = "iw7_default_reflection_probe";
		const auto path = output.extracted / "images" / (name + ".dds");
		std::filesystem::create_directories(path.parent_path());
		if (FAILED(DirectX::SaveToDDSFile(cube.GetImages(), cube.GetImageCount(), cube.GetMetadata(),
			DirectX::DDS_FLAGS_FORCE_DX9_LEGACY, path.wstring().data())))
		{
			throw std::runtime_error(std::format("cannot write {}", path.string()));
		}

		return name;
	}

	// A neutral grey colour map for a material whose own cannot be read (a streamed image of an image pack that this
	// install lacks: Scorch's 284 BC7 colour maps in imagefile2/52/69/84/85.pak drew white and blew the exposure out).
	std::string export_missing_color_map(extraction& output)
	{
		const std::string name = "iw7_missing_colormap";
		const auto path = output.extracted / "images" / (name + ".dds");
		if (std::filesystem::exists(path))
		{
			return name;
		}

		constexpr std::size_t size = 4;
		DirectX::ScratchImage image;
		if (FAILED(image.Initialize2D(DXGI_FORMAT_R8G8B8A8_UNORM, size, size, 1, 1)))
		{
			throw std::runtime_error("cannot create the missing colour map");
		}

		const auto& face = *image.GetImage(0, 0, 0);
		for (std::size_t y = 0; y < face.height; y++)
		{
			std::memset(face.pixels + y * face.rowPitch, 0x80, face.width * 4);
			for (std::size_t x = 0; x < face.width; x++)
			{
				face.pixels[y * face.rowPitch + x * 4 + 3] = 0xFF;
			}
		}

		std::filesystem::create_directories(path.parent_path());
		if (FAILED(DirectX::SaveToDDSFile(image.GetImages(), image.GetImageCount(), image.GetMetadata(),
			DirectX::DDS_FLAGS_FORCE_DX9_LEGACY, path.wstring().data())))
		{
			throw std::runtime_error(std::format("cannot write {}", path.string()));
		}

		return name;
	}

	void export_materials(const source_map& source, extraction& output)
	{
		// The images that each material can use for a role, in the order of its textures: the first one that converts
		// is the material's.
		std::map<std::string, image_file> files;
		std::map<std::string, std::array<std::vector<std::string>, image_role_count>> candidates;
		std::map<std::string, std::array<std::optional<std::string>, image_role_count>> builtins;
		const auto layer_of = [&](const std::string& name)
		{
			const auto found = output.material_layers.find(name);
			return found != output.material_layers.end() ? found->second : 0;
		};

		for (const auto& [name, material] : output.materials)
		{
			auto& roles = candidates[name];
			const auto layer = layer_of(name);
			// The sky's cube map converts with the sky (export_sky); the sky material takes no image of its own.
			if (classify_material(material).kind == surface_kind::sky)
			{
				continue;
			}

			// The alpha of the layer's colour: the first texture of its alpha, reveal and thickness maps that holds one; for a
			// layer that blends by its reveal map, the reveal (the pipeline ramps it by the layer's weight: kRevealLayerFlag).
			const auto reveals = layer && material_layer_reveal(material, layer);
			const iw7::GfxImage* alpha{};
			std::size_t channel{};
			// A reveal layer with an alpha of its own (packed "ar") blends by the reveal ramp times that alpha in IW7; the
			// colour keeps one alpha, so the alpha scales the reveal: exact where it is 0 or 1 (inside a decal and its
			// holes), its soft edges erode a little as the weight falls.
			std::optional<std::size_t> factor;
			for (auto i = 0; material && i < material->textureCount && !alpha; i++)
			{
				const auto& texture = material->textureTable[i];
				const auto image = texture.semantic == iw7::TS_ALPHA_REVEAL_THICKNESS_MAP && texture_layer(texture) == layer
					? source.resolve(iw7::ASSET_TYPE_IMAGE, texture.image) : nullptr;
				const auto found = image && image->name ? packed_channel(asset_name(image->name), reveals ? 'r' : 'a') : std::nullopt;
				if (found)
				{
					alpha = image;
					channel = *found;
					factor = reveals ? packed_channel(asset_name(image->name), 'a') : std::nullopt;
				}
			}

			for (auto i = 0; material && i < material->textureCount; i++)
			{
				const auto& texture = material->textureTable[i];
				const auto role = texture_layer(texture) == layer ? texture_role(texture) : std::nullopt;
				const auto image = role ? source.resolve(iw7::ASSET_TYPE_IMAGE, texture.image) : nullptr;
				if (!image || !image->name)
				{
					continue;
				}

				const auto index = static_cast<std::size_t>(*role);
				if (const auto builtin = builtin_image(asset_name(image->name), *role))
				{
					auto& slot = builtins[name][index];
					slot = slot ? slot : builtin;
					continue;
				}

				auto file = file_name(asset_name(image->name)) + (*role == image_role::normal ? "_slopes" : "");
				image_file entry{image, *role};
				if (*role == image_role::color && texture.semantic == iw7::TS_COLOR_SPECULAR_MAP)
				{
					// The colour of each alpha (or reveal) is a file of its own; without one, the colour is opaque.
					entry.alpha = alpha;
					entry.alpha_channel = channel;
					entry.alpha_factor_channel = factor;
					entry.opaque = !alpha;
					file += alpha ? std::format("_{}{:08x}", reveals ? factor ? "ra" : "r" : "a", name_hash(asset_name(alpha->name))) : "";
				}

				if (*role == image_role::color && output.world_materials.contains(name))
				{
					entry.limit = world_color_size;
				}

				const auto [slot, inserted] = files.try_emplace(file, entry);
				slot->second.limit = std::max(slot->second.limit, entry.limit);
				roles[index].push_back(file);
			}

			// The specular map that IW7 keeps in the colour's alpha, after any specular map of the material's own.
			const iw7::GfxImage* specular_color{};
			const iw7::GfxImage* gloss{};
			for (auto i = 0; material && i < material->textureCount; i++)
			{
				const auto& texture = material->textureTable[i];
				if (texture_layer(texture) != layer)
				{
					continue;
				}

				const auto image = source.resolve(iw7::ASSET_TYPE_IMAGE, texture.image);
				if (!image || !image->name)
				{
					continue;
				}

				const auto role = texture_role(texture);
				if (role == image_role::color && texture.semantic == iw7::TS_COLOR_SPECULAR_MAP && !specular_color)
				{
					specular_color = image;
				}
				else if (role == image_role::normal && !gloss)
				{
					// Packed normal maps (n, o, g after "_packed_") keep the gloss in red.
					const auto image_name = asset_name(image->name);
					const auto packed = image_name.rfind("_packed_n");
					if (packed != std::string_view::npos && image_name.substr(packed + 9).find('g') != std::string_view::npos)
					{
						gloss = image;
					}
				}
			}

			if (specular_color && !builtin_image(asset_name(specular_color->name), image_role::color))
			{
				auto file = file_name(asset_name(specular_color->name)) + "_f0" +
					(gloss ? std::format("_g{:08x}", name_hash(asset_name(gloss->name))) : std::string{});
				image_file entry{specular_color, image_role::specular};
				entry.derived_specular = true;
				entry.gloss = gloss;
				files.try_emplace(file, entry);
				roles[static_cast<std::size_t>(image_role::specular)].push_back(file);
			}
		}

		const auto folder = output.extracted / "images";
		std::filesystem::create_directories(folder);
		const auto usable = [&](const std::string& file)
		{
			const auto& written = files.at(file).written;
			return !written || *written;
		};

		// Each round writes the first image of each role that has not failed, on several threads.
		for (;;)
		{
			std::set<std::string> pending;
			for (const auto& [name, roles] : candidates)
			{
				for (const auto& role : roles)
				{
					const auto next = std::ranges::find_if(role, usable);
					if (next != role.end() && !files.at(*next).written)
					{
						pending.insert(*next);
					}
				}
			}

			if (pending.empty())
			{
				break;
			}

			const std::vector<std::string> round(pending.begin(), pending.end());
			std::for_each(std::execution::par, round.begin(), round.end(), [&](const std::string& name)
			{
				// An exception that leaves a parallel algorithm terminates the tool.
				auto& file = files.at(name);
				try
				{
					file.written = write_image(file, folder / (name + ".dds"));
				}
				catch (const std::exception& e)
				{
					ZONETOOL_WARNING("iw7: image \"%s\" does not convert: %s", file.image->name, e.what());
					file.written = false;
				}
			});
		}

		std::size_t missing{};
		std::array<std::size_t, 7> kinds{};
		std::size_t flags{};
		std::size_t missing_color_maps{};
		for (const auto& [name, material] : output.materials)
		{
			// IW7's tool materials (technique *_tools_*: nodraw, caulk_shadow) draw nothing: the fire alarm speakers'
			// nodraw surface drew IW7's yellow "nodraw" texture (owner test of 58aac89). They take the pipeline's
			// shadowcaster technique, which it never draws; shadow tools (caulk_shadow, not "nocast") still cast.
			// IW7's camera region 11 holds them all (w_tools_*, w_shadowcaster: Scorch's "Shadow" rectangle, owner test of
			// cbf4948).
			// Model nodraw materials (mo/nodraw, mopw/model_nodraw: Scorch's pillar signs) use a lit technique and
			// IW7's own "nodraw" image: they are tools by name.
			if (const auto tools = material && material->techniqueSet ? asset_name(material->techniqueSet->name) : std::string_view{};
				(material && material->cameraRegion == 11) || tools.find("_tools_") != std::string_view::npos ||
				tools.ends_with("_tools") || tools.find("shadowcaster") != std::string_view::npos ||
				name.ends_with("/nodraw") || name.ends_with("/model_nodraw") || name == "nodraw")
			{
				const auto casts = name.find("shadow") != std::string::npos && tools.find("nocast") == std::string_view::npos;
				auto state = state_json("none", false, surface_kind::opaque);
				write_json_async(output.extracted / "materials" / (name + ".json"),
				{
					{"$schema", "http://openassettools.dev/schema/material.v1.json"},
					{"_game", "iw3"},
					{"_type", "material"},
					{"_version", 1},
					{"cameraRegion", "lit"},
					{"constants", json::array()},
					{"gameFlags", casts ? json::array({"CASTS_SHADOW"}) : json::array()},
					{"sortKey", 4},
					{"stateBits", json::array({std::move(state)})},
					{"stateBitsEntry", json::array()},
					{"stateFlags", 0},
					{"surfaceTypeBits", 0},
					{"techniqueSet", "shadowcaster"},
					{"textureAtlas", {{"columns", 1}, {"rows", 1}}},
					{"textures", json::array({texture_json("colorMap", "$white")})},
				}, 16);
				continue;
			}

			std::array<std::optional<std::string>, image_role_count> images;
			for (std::size_t role = 0; role < image_role_count; role++)
			{
				const auto& role_files = candidates.at(name)[role];
				const auto written = std::ranges::find_if(role_files, [&](const std::string& file)
				{
					return files.at(file).written.value_or(false);
				});

				images[role] = written != role_files.end() ? std::optional(*written) : builtins[name][role];
				if (written == role_files.end() && !role_files.empty())
				{
					ZONETOOL_INFO("iw7: material \"%s\" loses its image %s (role %zu), which cannot be read", name.data(),
						files.at(role_files.front()).image->name, role);
					// Without its colour the material drew white ($white); a neutral grey keeps the scene readable.
					if (role == static_cast<std::size_t>(image_role::color) && !images[role])
					{
						images[role] = export_missing_color_map(output);
						missing_color_maps++;
					}
				}
			}

			if (output.decal_materials.contains(name))
			{
				// A particle decal: the pipeline's decal material (a Replay decal volume) of its colour, normal and specular
				// maps; the colour's alpha is the decal's opacity.
				const auto& color = images[static_cast<std::size_t>(image_role::color)];
				if (!color)
				{
					ZONETOOL_WARNING("iw7: decal material \"%s\" has no colour map", name.data());
					continue;
				}

				auto textures = json::array({texture_json("colorMap", *color)});
				if (const auto& normal = images[static_cast<std::size_t>(image_role::normal)]; normal && normal->front() != '$')
				{
					textures.push_back(texture_json("normalMap", *normal));
				}

				if (const auto& specular = images[static_cast<std::size_t>(image_role::specular)]; specular && specular->front() != '$')
				{
					textures.push_back(texture_json("specularMap", *specular));
				}

				const auto tint = material ? material_constant(material, "colorTint").value_or(std::array{1.0f, 1.0f, 1.0f, 1.0f})
					: std::array{1.0f, 1.0f, 1.0f, 1.0f};
				write_json_async(output.extracted / "materials" / (name + ".json"),
				{
					{"$schema", "http://openassettools.dev/schema/material.v1.json"},
					{"_game", "iw3"},
					{"_type", "material"},
					{"_version", 1},
					{"cameraRegion", "decal"},
					{"constants", json::array({{{"name", "colorTint"}, {"literal", tint}}})},
					{"gameFlags", json::array()},
					{"sortKey", material ? std::min(material->info.sortKey + 2u, 63u) : 9u},
					{"stateBits", json::array()},
					{"stateBitsEntry", json::array()},
					{"stateFlags", 52},
					{"surfaceTypeBits", 0},
					{"techniqueSet", "wc_l_sm_b0c0n0s0"},
					{"textureAtlas", {{"columns", 1}, {"rows", 1}}},
					{"textures", std::move(textures)},
				}, 16);
				continue;
			}

			if (output.effect_materials.contains(name))
			{
				const auto& color = images[static_cast<std::size_t>(image_role::color)];
				if (!color)
				{
					ZONETOOL_WARNING("iw7: effect material \"%s\" has no colour map and adds nothing", name.data());
				}

				write_json_async(output.extracted / "materials" / (name + ".json"), export_effect_material(material, color,
					light_unit_scale(source), output.atlas_materials.contains(name)), 16);
				continue;
			}

			// Glass panes blend as the pipeline's glass. Replay maps have no refraction: a distortion layer of a model
			// adds black. The layers of a layered material blend over its surfaces as decals.
			const auto layer = layer_of(name);
			auto kind = layer ? material_class{surface_kind::blend} : classify_material(material);
			// A layer blends as its token says: t tests its alpha (drawn as a blend over the surface), a adds, m multiplies.
			if (const auto token = layer ? material_layer_token(material, layer) : std::string_view{}; !token.empty())
			{
				kind.kind = token[0] == 'a' ? surface_kind::additive : token[0] == 'm' ? surface_kind::multiply : surface_kind::blend;
			}
			// A premultiplied surface whose colour holds its alpha (IW7's patterned glass: the metal of the pattern
			// covers, the glass between shows through) blends by that alpha.
			const auto& color = images[static_cast<std::size_t>(image_role::color)];
			if (kind.kind == surface_kind::additive && kind.premultiplied && color && files.contains(*color) &&
				!files.at(*color).opaque_alpha)
			{
				kind.kind = surface_kind::blend;
			}

			if (output.glass_materials.contains(name) && kind.kind != surface_kind::opaque)
			{
				kind = {surface_kind::blend};
			}
			else if (kind.kind == surface_kind::distortion)
			{
				kind = {surface_kind::additive, true};
				images = {"$black", {}, {}};
			}

			kinds[static_cast<std::size_t>(kind.kind)]++;
			flags += flag_wave(material).has_value();
			missing += material == nullptr;
			write_json_async(output.extracted / "materials" / (name + ".json"), export_material(material, images, kind, layer,
				light_unit_scale(source)), 16);
		}

		const auto written = std::ranges::count_if(files, [](const auto& entry)
		{
			return entry.second.written.value_or(false);
		});

		for (const auto& [pak, images] : missing_paks)
		{
			ZONETOOL_WARNING("iw7: %zu images stream from %s, which the game folder lacks (first: %s)", images.size(), pak.data(),
				images.begin()->data());
		}

		ZONETOOL_INFO("iw7: exported %zu materials (%zu not in the loaded zones) with %zu of their %zu images",
			output.materials.size(), missing, static_cast<std::size_t>(written), files.size());
		if (missing_color_maps)
		{
			ZONETOOL_INFO("iw7: %zu materials take the neutral grey colour map for an image that cannot be read (streamed from an image pack "
				"this install lacks)", missing_color_maps);
		}

		ZONETOOL_INFO("iw7: %zu opaque, %zu alpha tested, %zu blended, %zu additive, %zu multiply, %zu sky and %zu distortion materials",
			kinds[0], kinds[1], kinds[2], kinds[3], kinds[4], kinds[5], kinds[6]);
		if (flags)
		{
			ZONETOOL_INFO("iw7: %zu flag and foliage materials move by IW7's flag wave", flags);
		}
	}

	// The flag techsets (*_flag_*: the disassembled vertex shader runs three fixed waves on (time + flagParms.y) *
	// flagParms.x) and the flagext ones (*_flagext_wv0[wv1]_*: two waves of flagWave0Parms and flagWave1Parms on a phase
	// of flagParms), both weighted by the vertex alpha.
	std::optional<flag_wave_parameters> flag_wave(const iw7::Material* material)
	{
		if (!material || !material->techniqueSet)
		{
			return std::nullopt;
		}

		std::optional<bool> classic;
		for (const auto token : std::views::split(asset_name(material->techniqueSet->name), '_'))
		{
			const std::string_view name(token.begin(), token.end());
			classic = name == "flag" ? std::optional(true) : name == "flagext" ? std::optional(false) : classic;
		}

		const auto phase = material_constant(material, "flagParms");
		if (!classic || !phase)
		{
			return std::nullopt;
		}

		flag_wave_parameters result{*classic};
		const std::array<std::optional<std::array<float, 4>>, 3> parts{phase, material_constant(material, "flagWave0Parms"),
			material_constant(material, "flagWave1Parms")};
		for (std::size_t part = 0; part < parts.size(); part++)
		{
			std::ranges::copy(parts[part].value_or(std::array<float, 4>{}), result.values.begin() + part * 4);
		}

		return result;
	}

	bool reactive_motion(const iw7::Material* material)
	{
		if (!material || !material->techniqueSet)
		{
			return false;
		}

		return std::ranges::any_of(std::views::split(asset_name(material->techniqueSet->name), '_'), [](const auto token)
		{
			return std::string_view(token.begin(), token.end()) == "reactivemotion";
		});
	}

	// IW7's map art of the map select (in the ui zone; IW7's own loading screen draws the minimap with the spawns): the
	// importer takes images/loadscreen_<map>.dds as the package's preview, which the game shows while the map loads. IW7's
	// arena file (mp/basemaps.arena, in code_post_gfx) names each map's art: Divide shows Scorch's, Turista2 Mars Oasis'.
	void export_loadscreen(const source_map& source, const extraction& output)
	{
		std::string name;
		std::string long_name;
		const auto arena = raw_file(source, "mp/basemaps.arena");
		static const std::regex block(R"re(\{([^{}]*)\})re");
		static const std::regex field(R"re((\w+)\s+"([^"]*)")re");
		for (auto match = std::sregex_iterator(arena.begin(), arena.end(), block); match != std::sregex_iterator() && name.empty(); ++match)
		{
			const auto text = (*match)[1].str();
			std::map<std::string, std::string> values;
			for (auto entry = std::sregex_iterator(text.begin(), text.end(), field); entry != std::sregex_iterator(); ++entry)
			{
				values.emplace((*entry)[1].str(), (*entry)[2].str());
			}

			if (values["map"] == source.name())
			{
				name = values["mapimage"];
				long_name = values["longname"];
			}
		}

		if (name.empty())
		{
			name = "image_mp_map_" + source.name().substr(source.name().starts_with("mp_") ? 3 : 0);
		}

		const auto* image = static_cast<const iw7::GfxImage*>(source.find(iw7::ASSET_TYPE_IMAGE, name));
		image_file entry{image, image_role::color};
		entry.opaque = true;
		entry.limit = hud_image_size;
		if (!image)
		{
			ZONETOOL_WARNING("iw7: the ui zone holds no map art %s; the map loads with the default loading screen", name.data());
		}
		else if (write_image(entry, output.extracted / "images" / ("loadscreen_" + source.name() + ".dds")))
		{
			ZONETOOL_INFO("iw7: exported the map art %s as the loading screen (the arena names the map %s)", image->name,
				long_name.empty() ? "nothing" : long_name.data());
		}
	}

	// The HUD minimap: the image of IW7's 2D material compass_map_<map>, which the map script sets (setupminimap). The map's
	// zones lack Frontier's material, and their only Frontier material, compass_map_mp_frontier_tdmd, is a game mode's mask,
	// not the layout; its image is in the ui zone. The importer (iw3_fastfile.cpp CopyCompass) takes
	// images/compass_map_<map>.dds as the converted map's compass, with its alpha (the transparent outside).
	void export_compass(const source_map& source, const extraction& output)
	{
		const auto name = "compass_map_" + source.name();
		const auto* material = static_cast<const iw7::Material*>(source.find(iw7::ASSET_TYPE_MATERIAL, name));
		const auto* image = material && material->textureCount && material->textureTable
			? source.resolve(iw7::ASSET_TYPE_IMAGE, material->textureTable[0].image) : nullptr;
		if (!image)
		{
			image = static_cast<const iw7::GfxImage*>(source.find(iw7::ASSET_TYPE_IMAGE, name));
		}

		image_file entry{image, image_role::color};
		entry.limit = hud_image_size;
		if (!image)
		{
			ZONETOOL_WARNING("iw7: the map has no minimap (%s); the HUD compass keeps the default", name.data());
		}
		else if (write_image(entry, output.extracted / "images" / (name + ".dds")))
		{
			ZONETOOL_INFO("iw7: exported the minimap %s (%ux%u)%s", image->name, image->width, image->height,
				material ? "" : " without its material");
		}
	}
}
