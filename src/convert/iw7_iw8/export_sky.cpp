#include <std_include.hpp>
#include "export.hpp"
#include "images.hpp"

#include <numeric>

namespace zonetool::convert::iw7_iw8
{
	namespace
	{
		using vec3 = std::array<float, 3>;
		using color = std::array<float, 4>;

		float dot(const vec3& a, const vec3& b)
		{
			return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
		}

		vec3 normalized(const vec3& value)
		{
			const auto length = std::sqrt(dot(value, value));
			return length > 0.0f ? vec3{value[0] / length, value[1] / length, value[2] / length} : value;
		}

		float srgb_to_linear(const float value)
		{
			return value <= 0.04045f ? value / 12.92f : std::pow((value + 0.055f) / 1.055f, 2.4f);
		}

		// The faces of a cube map in D3D order: the texel at (a, b) in [-1, 1] of a face looks along w + a * u + b * v,
		// as the sky box of the pipeline maps it.
		struct cube_face
		{
			vec3 w;
			vec3 u;
			vec3 v;
		};

		constexpr std::array<cube_face, 6> cube_faces
		{{
			{{1, 0, 0}, {0, 0, -1}, {0, -1, 0}},
			{{-1, 0, 0}, {0, 0, 1}, {0, -1, 0}},
			{{0, 1, 0}, {1, 0, 0}, {0, 0, 1}},
			{{0, -1, 0}, {1, 0, 0}, {0, 0, -1}},
			{{0, 0, 1}, {1, 0, 0}, {0, -1, 0}},
			{{0, 0, -1}, {-1, 0, 0}, {0, -1, 0}},
		}};

		// A sky cube map as linear colours, with the depth (the inverse distance along the face axis) of the nearest
		// opaque model texel.
		struct sky_faces
		{
			std::size_t size{};
			std::array<std::vector<color>, 6> colors;
			std::array<std::vector<float>, 6> depths;
		};

		// A colour map as linear colours.
		struct texture
		{
			std::size_t width{};
			std::size_t height{};
			std::vector<color> texels;

			color sample(const float u, const float v) const
			{
				if (this->texels.empty())
				{
					return {1.0f, 1.0f, 1.0f, 1.0f};
				}

				const auto x = static_cast<std::size_t>((u - std::floor(u)) * static_cast<float>(this->width)) % this->width;
				const auto y = static_cast<std::size_t>((v - std::floor(v)) * static_cast<float>(this->height)) % this->height;
				return this->texels[y * this->width + x];
			}
		};

		texture load_texture(const source_map& source, const iw7::Material* material)
		{
			texture result;
			const auto image = color_image(source, material);
			const auto faces = image ? load_image_faces(*image, DXGI_FORMAT_R8G8B8A8_UNORM) : std::vector<DirectX::ScratchImage>{};
			if (faces.size() != 1)
			{
				return result;
			}

			const auto& top = *faces.front().GetImage(0, 0, 0);
			result.width = top.width;
			result.height = top.height;
			result.texels.resize(top.width * top.height);
			for (std::size_t y = 0; y < top.height; y++)
			{
				for (std::size_t x = 0; x < top.width; x++)
				{
					const auto* pixel = top.pixels + y * top.rowPitch + x * 4;
					result.texels[y * top.width + x] = {srgb_to_linear(pixel[0] / 255.0f), srgb_to_linear(pixel[1] / 255.0f),
						srgb_to_linear(pixel[2] / 255.0f), pixel[3] / 255.0f};
				}
			}

			return result;
		}

		// Draws a triangle, given by the directions of its corners from the viewpoint, on each face that it lies in
		// front of. `shade` takes the face, the texel, the perspective-correct weights of the corners and the depth.
		template <typename Shade>
		void rasterize(const std::array<vec3, 3>& directions, const std::size_t size, Shade&& shade)
		{
			const auto extent = static_cast<float>(size);
			for (auto f = 0u; f < cube_faces.size(); f++)
			{
				const auto& face = cube_faces[f];
				std::array<float, 3> x{};
				std::array<float, 3> y{};
				std::array<float, 3> depth{};
				auto in_front = true;
				for (auto k = 0; k < 3 && in_front; k++)
				{
					const auto w = dot(directions[k], face.w);
					in_front = w > 1.0e-4f * std::sqrt(dot(directions[k], directions[k]));
					x[k] = in_front ? (dot(directions[k], face.u) / w + 1.0f) * 0.5f * extent : 0.0f;
					y[k] = in_front ? (dot(directions[k], face.v) / w + 1.0f) * 0.5f * extent : 0.0f;
					depth[k] = in_front ? 1.0f / w : 0.0f;
				}

				const auto edge = [&](const int a, const int b, const float px, const float py)
				{
					return (x[b] - x[a]) * (py - y[a]) - (y[b] - y[a]) * (px - x[a]);
				};

				const auto area = in_front ? edge(0, 1, x[2], y[2]) : 0.0f;
				if (std::abs(area) < 1.0e-9f)
				{
					continue;
				}

				const auto last = static_cast<int>(size) - 1;
				const auto left = std::max(0, static_cast<int>(std::floor(std::min({x[0], x[1], x[2]}))));
				const auto right = std::min(last, static_cast<int>(std::ceil(std::max({x[0], x[1], x[2]}))));
				const auto top = std::max(0, static_cast<int>(std::floor(std::min({y[0], y[1], y[2]}))));
				const auto bottom = std::min(last, static_cast<int>(std::ceil(std::max({y[0], y[1], y[2]}))));
				for (auto py = top; py <= bottom; py++)
				{
					for (auto px = left; px <= right; px++)
					{
						const auto cx = static_cast<float>(px) + 0.5f;
						const auto cy = static_cast<float>(py) + 0.5f;
						const std::array<float, 3> e{edge(1, 2, cx, cy) / area, edge(2, 0, cx, cy) / area, edge(0, 1, cx, cy) / area};
						if (e[0] < 0.0f || e[1] < 0.0f || e[2] < 0.0f)
						{
							continue;
						}

						const auto inverse = e[0] * depth[0] + e[1] * depth[1] + e[2] * depth[2];
						const std::array<float, 3> weights{e[0] * depth[0] / inverse, e[1] * depth[1] / inverse, e[2] * depth[2] / inverse};
						shade(f, static_cast<std::size_t>(py) * size + static_cast<std::size_t>(px), weights, inverse);
					}
				}
			}
		}

		// Draws the models that are too large for the world (planets) into the sky as the centre of the world sees them:
		// lit by the sun, the opaque surfaces first and the blended ones over them. Unlit surfaces emit their colour map
		// times their tint in IW7's units, which `scale` takes to the sky's.
		void draw_distant_models(const source_map& source, const extraction& output, sky_faces& sky, const float scale)
		{
			if (output.distant_models.empty())
			{
				return;
			}

			const auto& world = *source.world();
			const auto& bounds = world.modelCount ? world.models[0].bounds : world.bounds;
			const vec3 center{bounds.midPoint[0], bounds.midPoint[1], bounds.midPoint[2]};
			auto sun = vec3{0.0f, 0.0f, 1.0f};
			if (const auto com_world = source.com_world(); com_world && com_world->primaryLightCount > sun_light)
			{
				const auto& light = com_world->primaryLights[sun_light];
				sun = normalized({light.dir[0], light.dir[1], light.dir[2]});
			}

			struct surface_draw
			{
				const mesh_surface* mesh{};
				material_class kind{};
				const texture* colors{};
				std::array<float, 4> tint{};
				std::vector<vec3> directions;
				std::vector<vec3> normals;
			};

			std::map<std::string, std::vector<mesh_surface>> meshes;
			std::map<const iw7::Material*, texture> textures;
			std::vector<surface_draw> draws;
			for (auto i = 0u; i < world.dpvs.smodelCount; i++)
			{
				const auto& instance = world.dpvs.smodelDrawInsts[i];
				const auto name = file_name(asset_name(instance.model ? iw7::get_asset_name(iw7::ASSET_TYPE_XMODEL, instance.model) : nullptr));
				const auto model = output.models.find(name);
				if (!output.distant_models.contains(name) || model == output.models.end() || !model->second)
				{
					continue;
				}

				auto mesh = meshes.find(name);
				if (mesh == meshes.end())
				{
					mesh = meshes.emplace(name, model_mesh(source, *model->second)).first;
				}

				const auto& placement = instance.placement;
				const vec3 origin{placement.origin[0], placement.origin[1], placement.origin[2]};
				const auto offset = vec3{origin[0] - center[0], origin[1] - center[1], origin[2] - center[2]};
				const auto distance = std::sqrt(dot(offset, offset));
				ZONETOOL_INFO("iw7: the sky draws \"%s\" %.0f units from the centre of the world (azimuth %.0f, elevation %.0f)",
					name.data(), distance, std::atan2(offset[1], offset[0]) * 57.2957795f, std::asin(offset[2] / distance) * 57.2957795f);
				for (const auto& surface : mesh->second)
				{
					auto& draw = draws.emplace_back();
					draw.mesh = &surface;
					draw.kind = classify_material(surface.material);
					draw.tint = material_color_tint(surface.material);
					auto colors = textures.find(surface.material);
					if (colors == textures.end())
					{
						colors = textures.emplace(surface.material, load_texture(source, surface.material)).first;
					}

					draw.colors = &colors->second;
					for (const auto& vertex : surface.vertices)
					{
						vec3 position{};
						vec3 normal{};
						for (auto axis = 0; axis < 3; axis++)
						{
							for (auto row = 0; row < 3; row++)
							{
								position[axis] += vertex.position[row] * placement.axis[row][axis];
								normal[axis] += vertex.normal[row] * placement.axis[row][axis];
							}

							position[axis] = origin[axis] + position[axis] * placement.scale - center[axis];
						}

						draw.directions.push_back(position);
						draw.normals.push_back(normalized(normal));
					}
				}
			}

			for (const auto blended : {false, true})
			{
				for (const auto& draw : draws)
				{
					const auto kind = draw.kind.kind;
					if (blended == draw.kind.solid() || kind == surface_kind::sky || kind == surface_kind::distortion)
					{
						continue;
					}

					for (const auto& triangle : draw.mesh->triangles)
					{
						if (std::ranges::any_of(triangle, [&](const std::uint16_t index) { return index >= draw.directions.size(); }))
						{
							continue;
						}

						const std::array<vec3, 3> corners{draw.directions[triangle[0]], draw.directions[triangle[1]], draw.directions[triangle[2]]};
						rasterize(corners, sky.size, [&](const unsigned int face, const std::size_t texel, const std::array<float, 3>& weights,
							const float depth)
						{
							if (depth <= sky.depths[face][texel])
							{
								return;
							}

							std::array<float, 2> uv{};
							vec3 normal{};
							for (auto k = 0; k < 3; k++)
							{
								const auto& vertex = draw.mesh->vertices[triangle[k]];
								uv[0] += weights[k] * vertex.uv[0];
								uv[1] += weights[k] * vertex.uv[1];
								for (auto axis = 0; axis < 3; axis++)
								{
									normal[axis] += weights[k] * draw.normals[triangle[k]][axis];
								}
							}

							auto texel_color = draw.colors->sample(uv[0], uv[1]);
							if (kind == surface_kind::alpha_test && texel_color[3] < 0.5f)
							{
								return;
							}

							// Space has almost no ambient light; the sun has a luminance of 1 in the sky's units.
							const auto light = draw.kind.unlit ? scale : 0.03f + std::max(0.0f, dot(normalized(normal), sun));
							auto& target = sky.colors[face][texel];
							for (auto channel = 0; channel < 3; channel++)
							{
								const auto value = texel_color[channel] * draw.tint[channel] * light;
								target[channel] = kind == surface_kind::additive ? target[channel] + value
									: kind == surface_kind::multiply ? target[channel] * value
									: kind == surface_kind::blend ? target[channel] * (1.0f - texel_color[3]) + value * texel_color[3]
									: value;
							}

							if (!blended)
							{
								sky.depths[face][texel] = depth;
							}
						});
					}
				}
			}

			ZONETOOL_INFO("iw7: drew %zu surfaces of %zu distant models into the sky", draws.size(), meshes.size());
		}
	}

	float linear_to_srgb(const float value)
	{
		const auto clamped = std::clamp(value, 0.0f, 1.0f);
		return clamped <= 0.0031308f ? clamped * 12.92f : 1.055f * std::pow(clamped, 1.0f / 2.4f) - 0.055f;
	}

	std::string export_sky(const source_map& source, extraction& output)
	{
		const auto& world = *source.world();
		for (auto i = 0; i < world.skyCount; i++)
		{
			const auto image = source.resolve(iw7::ASSET_TYPE_IMAGE, world.skies[i].skyImage);
			if (!image || !image->name || image->mapType != iw7::MAPTYPE_CUBE)
			{
				continue;
			}

			const auto faces = load_image_faces(*image, DXGI_FORMAT_R32G32B32A32_FLOAT);
			if (faces.size() != 6 || faces.front().GetMetadata().width != faces.front().GetMetadata().height)
			{
				ZONETOOL_WARNING("iw7: sky image \"%s\" (format %d) cannot be read", image->name, image->imageFormat);
				continue;
			}

			// The sky holds high dynamic range colours that IW7's exposure scales; IW3 draws its sky colours as they
			// are. The sky and the models that it draws take the units of the converted lights, in which the sun has a
			// luminance of 1 (Frontier: IW7 shows Neptune, its colour map times 20, at about 4 times the map; these
			// units put it at 2.1, which Replay draws at about 2.5).
			const auto size = faces.front().GetMetadata().width;
			const auto texels = size * size;
			std::vector<float> luminances;
			luminances.reserve(texels * faces.size());
			for (const auto& face : faces)
			{
				const auto* colors = reinterpret_cast<const float*>(face.GetPixels());
				for (std::size_t t = 0; t < texels; t++)
				{
					luminances.push_back(0.2126f * colors[t * 4] + 0.7152f * colors[t * 4 + 1] + 0.0722f * colors[t * 4 + 2]);
				}
			}

			const auto mean = std::accumulate(luminances.begin(), luminances.end(), 0.0) / static_cast<double>(luminances.size());
			const auto bright = luminances.begin() + static_cast<std::ptrdiff_t>(luminances.size() * 999 / 1000);
			std::ranges::nth_element(luminances, bright);
			const auto exposure = light_unit_scale(source);

			sky_faces sky{size};
			for (auto face = 0u; face < 6; face++)
			{
				const auto* colors = reinterpret_cast<const float*>(faces[face].GetPixels());
				sky.colors[face].resize(texels);
				sky.depths[face].assign(texels, 0.0f);
				for (std::size_t t = 0; t < texels; t++)
				{
					sky.colors[face][t] = {colors[t * 4] * exposure, colors[t * 4 + 1] * exposure, colors[t * 4 + 2] * exposure, 1.0f};
				}
			}

			draw_distant_models(source, output, sky, exposure);

			DirectX::ScratchImage cube;
			if (FAILED(cube.InitializeCube(DXGI_FORMAT_B8G8R8A8_UNORM, size, size, 1, 1)))
			{
				continue;
			}

			for (auto face = 0u; face < 6; face++)
			{
				const auto& target = *cube.GetImage(0, face, 0);
				for (std::size_t t = 0; t < texels; t++)
				{
					auto* pixel = target.pixels + (t / size) * target.rowPitch + (t % size) * 4;
					for (auto channel = 0; channel < 3; channel++)
					{
						// The masks: blue, green, red and alpha bytes.
						pixel[2 - channel] = static_cast<std::uint8_t>(std::lround(linear_to_srgb(sky.colors[face][t][channel]) * 255.0f));
					}

					pixel[3] = 0xFF;
				}
			}

			const auto name = file_name(asset_name(image->name));
			const auto path = output.extracted / "images" / (name + ".dds");
			std::filesystem::create_directories(path.parent_path());
			if (FAILED(DirectX::SaveToDDSFile(cube.GetImages(), cube.GetImageCount(), cube.GetMetadata(),
				DirectX::DDS_FLAGS_FORCE_DX9_LEGACY, path.wstring().data())))
			{
				throw std::runtime_error(std::format("cannot write {}", path.string()));
			}

			ZONETOOL_INFO("iw7: sky \"%s\": %zux%zu faces, mean luminance %g, 99.9th percentile %g, exposure %g", image->name, size,
				size, mean, *bright, exposure);
			return name;
		}

		ZONETOOL_WARNING("iw7: the world has no sky cube map that converts; the pipeline draws a dark sky%s",
			output.distant_models.empty() ? "" : std::format(" without the {} models that the sky would draw",
				output.distant_models.size()).data());
		return {};
	}

	// IW7 keeps the sun's sprite, flare, blind and glare in the world (sunflare_t); IW3 keeps them in sun/<map>.sun, which
	// the pipeline reads into Replay's sunflare (angles as degrees, fade times as seconds). The sprite's shader stays out:
	// the pipeline would draw it as a disc in the sky, and IW7's sky shows its own sun.
	void export_sun(const source_map& source, extraction& output)
	{
		const auto& sun = source.world()->sun;
		if (!sun.hasValidData)
		{
			ZONETOOL_INFO("iw7: the world has no sun flare data; Replay's blind and glare apply");
			return;
		}

		const auto degrees = [](const float dot)
		{
			return std::acos(std::clamp(dot, -1.0f, 1.0f)) * 180.0f / std::numbers::pi_v<float>;
		};

		const auto seconds = [](const int milliseconds)
		{
			return static_cast<float>(milliseconds) / 1000.0f;
		};

		const std::vector<std::pair<std::string_view, float>> values =
		{
			{"r_sunsprite_size", sun.spriteSize},
			{"r_sunflare_min_size", sun.flareMinSize},
			{"r_sunflare_min_angle", degrees(sun.flareMinDot)},
			{"r_sunflare_max_size", sun.flareMaxSize},
			{"r_sunflare_max_angle", degrees(sun.flareMaxDot)},
			{"r_sunflare_max_alpha", sun.flareMaxAlpha},
			{"r_sunflare_fadein", seconds(sun.flareFadeInTime)},
			{"r_sunflare_fadeout", seconds(sun.flareFadeOutTime)},
			{"r_sunblind_min_angle", degrees(sun.blindMinDot)},
			{"r_sunblind_max_angle", degrees(sun.blindMaxDot)},
			{"r_sunblind_max_darken", sun.blindMaxDarken},
			{"r_sunblind_fadein", seconds(sun.blindFadeInTime)},
			{"r_sunblind_fadeout", seconds(sun.blindFadeOutTime)},
			{"r_sunglare_min_angle", degrees(sun.glareMinDot)},
			{"r_sunglare_max_angle", degrees(sun.glareMaxDot)},
			{"r_sunglare_max_lighten", sun.glareMaxLighten},
			{"r_sunglare_fadein", seconds(sun.glareFadeInTime)},
			{"r_sunglare_fadeout", seconds(sun.glareFadeOutTime)},
		};

		std::string text;
		for (const auto& [key, value] : values)
		{
			text += std::format("{} \"{}\"\n", key, std::isfinite(value) ? value : 0.0f);
		}

		const auto path = output.extracted / "sun" / (source.name() + ".sun");
		std::filesystem::create_directories(path.parent_path());
		std::ofstream(path, std::ios::binary) << text;
		ZONETOOL_INFO("iw7: sun: sprite %g (%s), flare %g..%g alpha %g (%s), blind darken %g, glare lighten %g", sun.spriteSize,
			sun.spriteMaterial && sun.spriteMaterial->name ? sun.spriteMaterial->name : "no material", sun.flareMinSize,
			sun.flareMaxSize, sun.flareMaxAlpha, sun.flareMaterial && sun.flareMaterial->name ? sun.flareMaterial->name : "no material",
			sun.blindMaxDarken, sun.glareMaxLighten);
	}
}
