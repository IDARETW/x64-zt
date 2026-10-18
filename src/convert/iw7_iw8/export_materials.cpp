#include <std_include.hpp>
#include "export.hpp"
#include "images.hpp"

#include <execution>

namespace zonetool::convert::iw7_iw8
{
	namespace
	{
		// The atlas of the map pipeline draws 256-pixel tiles, and the pipeline keeps model images as 8-bit colours
		// with mip levels: 512 pixels keep the map zone at the size of a converted IW3 map (Frontier: 1.1 GB of model
		// images, against 3 GB at 1024).
		constexpr std::size_t max_image_size = 512;

		enum class image_role
		{
			color,
			normal,
			specular,
			count,
		};

		constexpr auto image_role_count = static_cast<std::size_t>(image_role::count);

		// An image file of the extraction: the image that it holds, and whether it was written.
		struct image_file
		{
			const iw7::GfxImage* image{};
			image_role role{};
			std::optional<bool> written;
		};

		// The pixels of an image at one size, with the mip levels below it.
		struct image_pixels
		{
			std::vector<std::uint8_t> data;
			std::size_t width{};
			std::size_t height{};
		};

		// The game file sources need not read on several threads at once.
		std::mutex read_mutex;

		std::optional<image_role> texture_role(const int semantic)
		{
			switch (semantic)
			{
			case iw7::TS_COLOR_MAP:
			case iw7::TS_COLOR_SPECULAR_MAP:
				return image_role::color;
			case iw7::TS_NORMAL_MAP:
			case iw7::TS_NORMAL_OCCLUSSION_GLOSS_MAP:
				return image_role::normal;
			case iw7::TS_SPECULAR_MAP:
				return image_role::specular;
			default:
				return {};
			}
		}

		// The engine images that the pipeline makes itself: a zone holds them without pixels.
		std::optional<std::string> builtin_image(const std::string_view name, const image_role role)
		{
			if (name == "$white" || name == "$black" || name == "$identitynormalmap")
			{
				return std::string(name);
			}

			if (name == "flat_nml" && role == image_role::normal)
			{
				return "$identitynormalmap";
			}

			return {};
		}

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
					const std::lock_guard lock(read_mutex);
					if (!game_files::get().read(pak, file.offset, compressed.size(), compressed.data()))
					{
						static std::set<std::string> missing;
						if (missing.insert(pak).second)
						{
							ZONETOOL_WARNING("iw7: streamed images need %s", pak.data());
						}

						continue;
					}
				}

				auto pixels = iw7::decompress_lz4_block(compressed.data() + sizeof(iw7::XFileCompressorHeader),
					compressed.size() - sizeof(iw7::XFileCompressorHeader));
				if (!pixels.empty())
				{
					return image_pixels{std::move(pixels), image.streams[i].width, image.streams[i].height};
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

				DirectX::ScratchImage decoded;
				const auto decoded_result = DirectX::IsCompressed(source.format)
					? DirectX::Decompress(source, format, decoded)
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

		// IW3 normal maps hold the slopes x/z and y/z, which its shaders decode as xy * 4.08 - 2.08.
		void encode_normal_slopes(DirectX::ScratchImage& image)
		{
			const auto& metadata = image.GetMetadata();
			auto* pixels = image.GetPixels();
			for (std::size_t i = 0; i < metadata.width * metadata.height; i++)
			{
				auto* pixel = pixels + i * 4;
				const auto x = pixel[0] / 255.0f * 2.0f - 1.0f;
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

		// Writes an image as the DDS file that the pipeline reads.
		bool write_image(const iw7::GfxImage& image, const image_role role, const std::filesystem::path& path)
		{
			auto faces = load_faces(image, DXGI_FORMAT_R8G8B8A8_UNORM);
			if (faces.size() != 1)
			{
				ZONETOOL_WARNING("iw7: image \"%s\" (format %d, map type %d) cannot be read", image.name, image.imageFormat,
					image.mapType);
				return false;
			}

			auto& pixels = faces.front();
			if (role == image_role::normal)
			{
				encode_normal_slopes(pixels);
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

			DirectX::ScratchImage mips;
			if (FAILED(DirectX::GenerateMipMaps(*top, DirectX::TEX_FILTER_BOX | DirectX::TEX_FILTER_FORCE_NON_WIC, 0, mips)))
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
				const auto format = mips.IsAlphaAllOpaque() ? DXGI_FORMAT_BC1_UNORM : DXGI_FORMAT_BC3_UNORM;
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
		json state_json(const std::string& cull, const bool color, const surface_kind kind)
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
				{"polygonOffset", "offset0"},
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

		// The IW3 material of a material, with the images of its roles (the pipeline's defaults for the missing ones).
		json export_material(const iw7::Material* material,
			const std::array<std::optional<std::string>, image_role_count>& images, const material_class& kind)
		{
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
			return
			{
				{"$schema", "http://openassettools.dev/schema/material.v1.json"},
				{"_game", "iw3"},
				{"_type", "material"},
				{"_version", 1},
				{"cameraRegion", "lit"},
				{"constants", json::array({{{"name", "colorTint"}, {"literal", {1.0f, 1.0f, 1.0f, 1.0f}}}})},
				{"gameFlags", shadow ? json::array({"CASTS_SHADOW"}) : json::array()},
				{"sortKey", material ? iw3_sort(*material, kind) : 4u},
				{"stateBits", json::array({state_json(cull, false, kind.kind), state_json(cull, true, kind.kind)})},
				{"stateBitsEntry", std::move(entries)},
				{"stateFlags", 0},
				{"surfaceTypeBits", 0},
				{"techniqueSet", iw3_technique_set(kind)},
				{"textureAtlas", {{"columns", 1}, {"rows", 1}}},
				{"textures", std::move(textures)},
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

		result.unlit = has("unlit");
		result.falloff = has("falloff");
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
				// pipeline's nearest blend.
				result.kind = surface_kind::additive;
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

	std::vector<DirectX::ScratchImage> load_image_faces(const iw7::GfxImage& image, const DXGI_FORMAT format,
		const std::size_t limit)
	{
		return load_faces(image, format, limit ? limit : max_image_size);
	}

	const iw7::GfxImage* color_image(const source_map& source, const iw7::Material* material)
	{
		for (auto i = 0; material && i < material->textureCount; i++)
		{
			const auto& texture = material->textureTable[i];
			if (texture_role(texture.semantic) == image_role::color)
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

	void export_materials(const source_map& source, extraction& output)
	{
		// The images that each material can use for a role, in the order of its textures: the first one that converts
		// is the material's.
		std::map<std::string, image_file> files;
		std::map<std::string, std::array<std::vector<std::string>, image_role_count>> candidates;
		std::map<std::string, std::array<std::optional<std::string>, image_role_count>> builtins;
		for (const auto& [name, material] : output.materials)
		{
			auto& roles = candidates[name];
			for (auto i = 0; material && i < material->textureCount; i++)
			{
				const auto& texture = material->textureTable[i];
				const auto role = texture_role(texture.semantic);
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

				const auto file = file_name(asset_name(image->name)) + (*role == image_role::normal ? "_slopes" : "");
				files.try_emplace(file, image_file{image, *role});
				roles[index].push_back(file);
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
					file.written = write_image(*file.image, file.role, folder / (name + ".dds"));
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
		for (const auto& [name, material] : output.materials)
		{
			std::array<std::optional<std::string>, image_role_count> images;
			for (std::size_t role = 0; role < image_role_count; role++)
			{
				const auto& role_files = candidates.at(name)[role];
				const auto written = std::ranges::find_if(role_files, [&](const std::string& file)
				{
					return files.at(file).written.value_or(false);
				});

				images[role] = written != role_files.end() ? std::optional(*written) : builtins[name][role];
			}

			// Glass panes blend as the pipeline's glass. Replay maps have no refraction: a distortion layer of a model
			// adds black.
			auto kind = classify_material(material);
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
			missing += material == nullptr;
			write_json(output.extracted / "materials" / (name + ".json"), export_material(material, images, kind));
		}

		const auto written = std::ranges::count_if(files, [](const auto& entry)
		{
			return entry.second.written.value_or(false);
		});

		ZONETOOL_INFO("iw7: exported %zu materials (%zu not in the loaded zones) with %zu of their %zu images",
			output.materials.size(), missing, static_cast<std::size_t>(written), files.size());
		ZONETOOL_INFO("iw7: %zu opaque, %zu alpha tested, %zu blended, %zu additive, %zu multiply, %zu sky and %zu distortion materials",
			kinds[0], kinds[1], kinds[2], kinds[3], kinds[4], kinds[5], kinds[6]);
	}
}
