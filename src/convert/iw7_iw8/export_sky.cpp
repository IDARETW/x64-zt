#include <std_include.hpp>
#include "export.hpp"

#include <execution>
#include "images.hpp"

#include <utils/compression.hpp>

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

		// Draws a triangle, given by the directions of its corners from the viewpoint, on each face of the cube: the part in
		// front of the face, clipped at a plane just in front of the viewpoint. A large triangle that passes beside or
		// behind the viewpoint (Breakneck's black hole rings, which reach around the map) keeps what each face sees of it;
		// before, a triangle drew only on a face that all its corners lay in front of, and such triangles left holes with
		// hard edges in the sky. `shade` takes the face, the texel, the perspective-correct weights of the corners and the
		// depth.
		template <typename Shade>
		void rasterize(const std::array<vec3, 3>& directions, const std::size_t size, Shade&& shade)
		{
			struct corner
			{
				vec3 direction;
				std::array<float, 3> weights;
			};

			const auto extent = static_cast<float>(size);
			const auto plane = 1.0e-3f * std::sqrt(std::min({dot(directions[0], directions[0]), dot(directions[1], directions[1]),
				dot(directions[2], directions[2])}));
			const std::array<corner, 3> triangle{{{directions[0], {1.0f, 0.0f, 0.0f}}, {directions[1], {0.0f, 1.0f, 0.0f}},
				{directions[2], {0.0f, 0.0f, 1.0f}}}};
			for (auto f = 0u; f < cube_faces.size(); f++)
			{
				const auto& face = cube_faces[f];
				std::vector<corner> polygon;
				for (std::size_t k = 0; k < triangle.size(); k++)
				{
					const auto& a = triangle[k];
					const auto& b = triangle[(k + 1) % triangle.size()];
					const auto da = dot(a.direction, face.w) - plane;
					const auto db = dot(b.direction, face.w) - plane;
					if (da >= 0.0f)
					{
						polygon.push_back(a);
					}

					if ((da >= 0.0f) != (db >= 0.0f))
					{
						const auto t = da / (da - db);
						corner between{};
						for (auto c = 0; c < 3; c++)
						{
							between.direction[c] = a.direction[c] + (b.direction[c] - a.direction[c]) * t;
							between.weights[c] = a.weights[c] + (b.weights[c] - a.weights[c]) * t;
						}

						polygon.push_back(between);
					}
				}

				for (std::size_t fan = 1; fan + 1 < polygon.size(); fan++)
				{
					const std::array<const corner*, 3> part{&polygon[0], &polygon[fan], &polygon[fan + 1]};
					std::array<float, 3> x{};
					std::array<float, 3> y{};
					std::array<float, 3> depth{};
					for (auto k = 0; k < 3; k++)
					{
						const auto w = dot(part[k]->direction, face.w);
						x[k] = (dot(part[k]->direction, face.u) / w + 1.0f) * 0.5f * extent;
						y[k] = (dot(part[k]->direction, face.v) / w + 1.0f) * 0.5f * extent;
						depth[k] = 1.0f / w;
					}

					const auto edge = [&](const int a, const int b, const float px, const float py)
					{
						return (x[b] - x[a]) * (py - y[a]) - (y[b] - y[a]) * (px - x[a]);
					};

					const auto area = edge(0, 1, x[2], y[2]);
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
							std::array<float, 3> weights{};
							for (auto k = 0; k < 3; k++)
							{
								const auto share = e[k] * depth[k] / inverse;
								for (auto c = 0; c < 3; c++)
								{
									weights[c] += share * part[k]->weights[c];
								}
							}

							shade(f, static_cast<std::size_t>(py) * size + static_cast<std::size_t>(px), weights, inverse);
						}
					}
				}
			}
		}

		// Draws the models that are too large for the world (planets) into the sky as the centre of the world sees them:
		// lit by the sun, the opaque surfaces first and the blended ones over them. Unlit surfaces emit their colour map
		// times their tint in IW7's units, which `scale` takes to the sky's.
		void draw_distant_models(const source_map& source, const extraction& output, sky_faces& sky, const float scale)
		{
			if (output.distant_models.empty() && output.distant_surfaces.empty())
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
				// The technique multiplies the colour by the vertex colour (techsets *_vc_*, *_evc_*: the black hole's black
				// core is a white map times black vertices).
				bool vertex_color{};
				std::vector<vec3> directions;
				std::vector<vec3> normals;
			};

			const auto uses_vertex_color = [](const iw7::Material* material)
			{
				if (!material || !material->techniqueSet)
				{
					return false;
				}

				for (const auto token : std::views::split(asset_name(material->techniqueSet->name), '_'))
				{
					const std::string_view name(token.begin(), token.end());
					if (name == "vc" || name == "evc")
					{
						return true;
					}
				}

				return false;
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
					draw.vertex_color = uses_vertex_color(surface.material);
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

			// The world surfaces beyond the range of the world, in world space.
			for (const auto& surface : output.distant_surfaces)
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
					draw.directions.push_back({vertex.position[0] - center[0], vertex.position[1] - center[1], vertex.position[2] - center[2]});
					draw.normals.push_back(normalized({vertex.normal[0], vertex.normal[1], vertex.normal[2]}));
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
							if (draw.vertex_color)
							{
								for (auto channel = 0; channel < 4; channel++)
								{
									auto value = 0.0f;
									for (auto k = 0; k < 3; k++)
									{
										value += weights[k] * draw.mesh->vertices[triangle[k]].color[channel];
									}

									texel_color[channel] *= channel < 3 ? srgb_to_linear(value) : value;
								}
							}

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

			ZONETOOL_INFO("iw7: drew %zu surfaces of %zu distant models and %zu distant world surfaces into the sky", draws.size(),
				meshes.size(), output.distant_surfaces.size());
		}
	}

	float linear_to_srgb(const float value)
	{
		const auto clamped = std::clamp(value, 0.0f, 1.0f);
		return clamped <= 0.0031308f ? clamped * 12.92f : 1.055f * std::pow(clamped, 1.0f / 2.4f) - 0.055f;
	}

	std::string raw_file(const source_map& source, const std::string& name)
	{
		const auto* raw = static_cast<const iw7::RawFile*>(source.find(iw7::ASSET_TYPE_RAWFILE, name));
		if (!raw || !raw->buffer || raw->len <= 0)
		{
			return {};
		}

		return raw->compressedLen > 0 ? utils::compression::zlib::decompress(std::string(raw->buffer, raw->compressedLen))
			: std::string(raw->buffer, raw->len);
	}

	namespace
	{
		// The sky's colour scale: IW7's sky shader multiplies the cube map by r_hdrSkyIntensity times r_hdrSkyColorTint
		// (w_sky PS: cb2[11]), which the map's vision file sets (10 on Scorch, 2.3 on Frontier).
		vec3 sky_color_scale(const source_map& source)
		{
			const auto vision = vision_settings(source);
			vec3 tint{1.0f, 1.0f, 1.0f};
			auto intensity = 1.0f;
			if (const auto found = vision.find("r_hdrSkyIntensity"); found != vision.end())
			{
				std::istringstream(found->second) >> intensity;
			}

			if (const auto found = vision.find("r_hdrSkyColorTint"); found != vision.end())
			{
				std::istringstream(found->second) >> tint[0] >> tint[1] >> tint[2];
			}

			return {tint[0] * intensity, tint[1] * intensity, tint[2] * intensity};
		}

		// The forward, left and up axes of the sky's rotation: worldspawn key 0x102A4 (it has no name in the token list),
		// the angles that IW7's sky shader turns view directions back by before it samples the cube map (w_sky VS: cb2
		// rows 12-14, CONST_SRC_CODE_SKY_ROTATION_MATRIX_INVERSE). Inferred from Scorch, the only map with it ("0 167 0"):
		// its cube map paints the sun at azimuth -1.2 degrees, which the rotation turns to 165.8 degrees, where the map
		// places its sun effect (164 degrees from the playing area, at the same 48 degree elevation).
		std::array<vec3, 3> sky_axes(const source_map& source)
		{
			std::array<float, 3> angles{};
			const auto* map_ents = source.map_ents();
			if (map_ents && map_ents->entityString)
			{
				// The worldspawn is the first entity.
				const std::string_view entities(map_ents->entityString, strnlen(map_ents->entityString, map_ents->numEntityChars));
				const auto first = entities.substr(0, entities.find('}'));
				constexpr std::string_view key = "\n66212 \"";
				if (const auto found = first.find(key); found != std::string_view::npos)
				{
					const auto start = found + key.size();
					std::istringstream(std::string(first.substr(start, first.find('"', start) - start))) >> angles[0] >> angles[1]
						>> angles[2];
				}
			}

			constexpr auto radians = std::numbers::pi_v<float> / 180.0f;
			const auto sp = std::sin(angles[0] * radians), cp = std::cos(angles[0] * radians);
			const auto sy = std::sin(angles[1] * radians), cy = std::cos(angles[1] * radians);
			const auto sr = std::sin(angles[2] * radians), cr = std::cos(angles[2] * radians);
			return {{{cp * cy, cp * sy, -sp}, {sr * sp * cy - cr * sy, sr * sp * sy + cr * cy, sr * cp},
				{cr * sp * cy + sr * sy, cr * sp * sy - sr * cy, cr * cp}}};
		}

		// The colour of a cube map (faces of linear RGBA texels) along a direction, filtered bilinearly within its face.
		color sample_cube(const std::vector<DirectX::ScratchImage>& faces, const vec3& direction)
		{
			auto best = 0u;
			for (auto f = 1u; f < cube_faces.size(); f++)
			{
				if (dot(direction, cube_faces[f].w) > dot(direction, cube_faces[best].w))
				{
					best = f;
				}
			}

			const auto& face = cube_faces[best];
			const auto size = static_cast<int>(faces[best].GetMetadata().width);
			const auto w = dot(direction, face.w);
			const auto x = (dot(direction, face.u) / w + 1.0f) * 0.5f * static_cast<float>(size) - 0.5f;
			const auto y = (dot(direction, face.v) / w + 1.0f) * 0.5f * static_cast<float>(size) - 0.5f;
			const auto x0 = static_cast<int>(std::floor(x));
			const auto y0 = static_cast<int>(std::floor(y));
			const auto fx = x - static_cast<float>(x0);
			const auto fy = y - static_cast<float>(y0);
			const auto* texels = reinterpret_cast<const float*>(faces[best].GetPixels());
			color result{};
			for (auto k = 0; k < 4; k++)
			{
				const auto tx = std::clamp(x0 + (k & 1), 0, size - 1);
				const auto ty = std::clamp(y0 + (k >> 1), 0, size - 1);
				const auto weight = ((k & 1) ? fx : 1.0f - fx) * ((k >> 1) ? fy : 1.0f - fy);
				for (auto c = 0; c < 4; c++)
				{
					result[c] += texels[(static_cast<std::size_t>(ty) * size + tx) * 4 + c] * weight;
				}
			}

			return result;
		}
	}

	std::map<std::string, std::string> vision_settings(const source_map& source)
	{
		std::map<std::string, std::string> result;
		std::istringstream vision(raw_file(source, "vision/" + source.name() + ".vision"));
		for (std::string line; std::getline(vision, line);)
		{
			std::istringstream fields(line);
			std::string key;
			std::string value;
			if (fields >> key >> std::quoted(value))
			{
				result[key] = value;
			}
		}

		return result;
	}

	json export_fog(const source_map& source)
	{
		const auto vision = vision_settings(source);
		if (vision.empty())
		{
			return nullptr;
		}

		const auto scalar = [&](const char* key, const float fallback)
		{
			auto value = fallback;
			if (const auto found = vision.find(key); found != vision.end())
			{
				std::istringstream(found->second) >> value;
			}

			return value;
		};

		const auto vector = [&](const char* key, const vec3& fallback)
		{
			auto value = fallback;
			if (const auto found = vision.find(key); found != vision.end())
			{
				std::istringstream(found->second) >> value[0] >> value[1] >> value[2];
			}

			return json::array({value[0], value[1], value[2]});
		};

		// IW7 fogs as Replay does (setExpFog's IW6 fields: fogDensity is the halfway distance); the colours are in IW7's
		// light units, which the converted lights take to a sun of luminance 1.
		const auto scale = light_unit_scale(source);
		return
		{
			{"density", scalar("fogDensity", 70000.0f)},
			{"start", scalar("fogStartDist", 0.0f)},
			{"color", vector("fogColor", {1.0f, 1.0f, 1.0f})},
			{"color_intensity", scalar("fogColorIntensity", 1.0f) * scale},
			{"max_opacity", scalar("fogMaxOpacity", 0.0f)},
			{"sun_density", scalar("fogSunDensity", 70000.0f)},
			{"sun_color", vector("fogSunColor", {1.0f, 1.0f, 1.0f})},
			{"sun_color_intensity", scalar("fogSunColorIntensity", 1.0f) * scale},
			{"sun_begin_fade_angle", scalar("fogSunBeginFadeAngle", 0.0f)},
			{"sun_end_fade_angle", scalar("fogSunEndFadeAngle", 360.0f)},
			{"sun_dir", vector("fogSunDir", {1.0f, 0.0f, 0.0f})},
			{"sky_intensity", scalar("fogSkyIntensity", 0.0f)},
			{"sky_min_angle", scalar("fogSkyMinAngle", 0.0f)},
			{"sky_max_angle", scalar("fogSkyMaxAngle", 90.0f)},
		};
	}

	json export_grading_clut(const source_map& source, const extraction& output)
	{
		// The vision's colour grading lookup table (clut0Image: Scorch's mp_divide_clut, 32^3 RGBA8, red along x, green
		// along y, blue along z), written as its texels for the pipeline's Replay grading CLUT.
		const auto vision = vision_settings(source);
		const auto found = vision.find("clut0Image");
		if (found == vision.end() || found->second.empty())
		{
			return nullptr;
		}

		const auto* image = static_cast<const iw7::GfxImage*>(source.find(iw7::ASSET_TYPE_IMAGE, found->second));
		const auto size = image ? static_cast<std::size_t>(image->width) : 0u;
		const auto bytes = size * size * size * 4;
		if (!image || image->mapType != iw7::MAPTYPE_3D || image->height != size || image->depth != size || size < 2 ||
			(image->imageFormat != DXGI_FORMAT_R8G8B8A8_UNORM && image->imageFormat != DXGI_FORMAT_R8G8B8A8_UNORM_SRGB) ||
			image->streamed || !image->pixelData || image->dataLen1 < bytes)
		{
			ZONETOOL_WARNING("iw7: the vision's colour grading table \"%s\" does not convert (%s)", found->second.data(),
				image ? std::format("map type {}, format {}, {}x{}x{}, {} bytes{}", static_cast<int>(image->mapType),
					static_cast<int>(image->imageFormat), image->width, image->height, image->depth, image->dataLen1,
					image->streamed ? ", streamed" : "").data() : "not in the loaded zones");
			return nullptr;
		}

		const auto file = std::filesystem::path("images") / (file_name(found->second) + ".clut.rgba");
		std::filesystem::create_directories((output.extracted / file).parent_path());
		std::ofstream stream(output.extracted / file, std::ios::binary);
		stream.write(reinterpret_cast<const char*>(image->pixelData), static_cast<std::streamsize>(bytes));
		if (!stream)
		{
			throw std::runtime_error(std::format("cannot write {}", (output.extracted / file).string()));
		}

		ZONETOOL_INFO("iw7: colour grading table \"%s\" (%zu^3)", found->second.data(), size);
		return {{"file", file.generic_string()}, {"size", size}};
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
				ZONETOOL_WARNING("iw7: sky image \"%s\" cannot be read: %s", image->name, image_read_failure(*image).data());
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

			// IW7 draws the cube map turned by the sky's rotation and scaled by the vision's sky intensity.
			const auto axes = sky_axes(source);
			const auto scale = sky_color_scale(source);

			// Then it fogs the sky toward the fog colour by the view's angle from the zenith (the shader takes (1 - z) * pi
			// / 2 for it), between the vision's sky fog angles and times its sky fog intensity; toward the sun (between the
			// sun fog's end and begin angles) the colour turns to the sun fog colour (w_sky PS: cb2 rows 0-4). The colours
			// are in the converted units, as the sky's.
			auto fog = export_fog(source);
			// The fog's values are read once (the sky has millions of texels).
			struct sky_fog_values
			{
				float intensity, minimum, maximum, end, begin;
				vec3 sun;
				std::array<float, 3> plain, sunward;
			};
			std::optional<sky_fog_values> fog_values;
			if (fog.is_object() && fog["sky_intensity"].get<float>() > 0.0f)
			{
				constexpr auto radians = std::numbers::pi_v<float> / 180.0f;
				auto& values = fog_values.emplace();
				values.intensity = fog["sky_intensity"].get<float>();
				values.minimum = fog["sky_min_angle"].get<float>() * radians;
				values.maximum = fog["sky_max_angle"].get<float>() * radians;
				values.sun = normalized({fog["sun_dir"][0].get<float>(), fog["sun_dir"][1].get<float>(), fog["sun_dir"][2].get<float>()});
				values.end = std::cos(std::min(fog["sun_end_fade_angle"].get<float>(), 180.0f) * radians);
				values.begin = std::cos(std::min(fog["sun_begin_fade_angle"].get<float>(), 180.0f) * radians);
				for (auto c = 0; c < 3; c++)
				{
					values.plain[c] = fog["color"][c].get<float>() * fog["color_intensity"].get<float>();
					values.sunward[c] = fog["sun_color"][c].get<float>() * fog["sun_color_intensity"].get<float>();
				}
			}

			const auto sky_fog = [&](const vec3& view, color& value)
			{
				if (!fog_values)
				{
					return;
				}

				const auto& values = *fog_values;
				const auto direction = normalized(view);
				const auto amount = std::clamp(((1.0f - direction[2]) * std::numbers::pi_v<float> * 0.5f - values.minimum) /
					std::max(values.maximum - values.minimum, 1e-4f), 0.0f, 1.0f) * values.intensity;
				const auto toward = std::clamp((dot(direction, values.sun) - values.end) / std::max(values.begin - values.end, 1e-4f),
					0.0f, 1.0f);
				for (auto c = 0; c < 3; c++)
				{
					value[c] += (values.plain[c] + (values.sunward[c] - values.plain[c]) * toward - value[c]) * amount;
				}
			};
			// A sky that draws distant models (Breakneck's black hole, Frontier's Neptune) keeps them at 1024 texels a face
			// at least: the cube map of the sky alone samples into it.
			const auto sky_size = output.distant_models.empty() && output.distant_surfaces.empty() ? size : std::max<std::size_t>(size, 1024);
			const auto sky_texels = sky_size * sky_size;
			sky_faces sky{sky_size};
			// The faces sample on their own threads.
			std::array<unsigned, 6> face_indices{0, 1, 2, 3, 4, 5};
			std::for_each(std::execution::par, face_indices.begin(), face_indices.end(), [&](const unsigned face)
			{
				const auto& axis = cube_faces[face];
				sky.colors[face].resize(sky_texels);
				sky.depths[face].assign(sky_texels, 0.0f);
				for (std::size_t t = 0; t < sky_texels; t++)
				{
					const auto a = (static_cast<float>(t % sky_size) + 0.5f) / static_cast<float>(sky_size) * 2.0f - 1.0f;
					const auto b = (static_cast<float>(t / sky_size) + 0.5f) / static_cast<float>(sky_size) * 2.0f - 1.0f;
					const vec3 view{axis.w[0] + a * axis.u[0] + b * axis.v[0], axis.w[1] + a * axis.u[1] + b * axis.v[1],
						axis.w[2] + a * axis.u[2] + b * axis.v[2]};
					const auto sample = sample_cube(faces, {dot(view, axes[0]), dot(view, axes[1]), dot(view, axes[2])});
					sky.colors[face][t] = {sample[0] * exposure * scale[0], sample[1] * exposure * scale[1],
						sample[2] * exposure * scale[2], 1.0f};
					sky_fog(view, sky.colors[face][t]);
				}
			});

			draw_distant_models(source, output, sky, exposure);

			DirectX::ScratchImage cube;
			if (FAILED(cube.InitializeCube(DXGI_FORMAT_B8G8R8A8_UNORM, sky_size, sky_size, 1, 1)))
			{
				continue;
			}

			for (auto face = 0u; face < 6; face++)
			{
				const auto& target = *cube.GetImage(0, face, 0);
				for (std::size_t t = 0; t < sky_texels; t++)
				{
					auto* pixel = target.pixels + (t / sky_size) * target.rowPitch + (t % sky_size) * 4;
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

			ZONETOOL_INFO("iw7: sky \"%s\": %zux%zu faces, mean luminance %g, 99.9th percentile %g, exposure %g, intensity %g %g %g, "
				"rotation forward %g %g %g", image->name, sky_size, sky_size, mean, *bright, exposure, scale[0], scale[1], scale[2], axes[0][0],
				axes[0][1], axes[0][2]);
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
