#include <std_include.hpp>
#include "export.hpp"
#include "images.hpp"

#pragma warning(push, 0)
#include <glaze/json.hpp>
#pragma warning(pop)
#include <future>

namespace zonetool::convert::iw7_iw8
{
	// A world surface of the world file, written by glaze (the members are its keys).
	struct world_geometry_surface
	{
		std::string material;
		int lightmap{};
		int reflection_probe{};
		std::vector<geometry_vertex> vertices;
		std::vector<std::uint32_t> indices;
	};

	namespace
	{
		json vector_json(const float* values, const std::size_t count)
		{
			auto result = json::array();
			for (std::size_t i = 0; i < count; i++)
			{
				result.push_back(values[i]);
			}

			return result;
		}

		json bounds_json(const iw7::Bounds& bounds)
		{
			float mins[3]{};
			float maxs[3]{};
			for (auto i = 0; i < 3; i++)
			{
				mins[i] = bounds.midPoint[i] - bounds.halfSize[i];
				maxs[i] = bounds.midPoint[i] + bounds.halfSize[i];
			}

			return json::array({vector_json(mins, 3), vector_json(maxs, 3)});
		}

		// A unit vector packed in 10 bits per component, as IW7 and H1 store normals and tangents.
		std::array<float, 3> unpack_unit_vec(const iw7::PackedUnitVec& vector)
		{
			std::array<float, 3> result{};
			for (auto i = 0; i < 3; i++)
			{
				result[i] = static_cast<float>((vector.packed >> (10 * i)) & 0x3FF) / 1023.0f * 2.0f - 1.0f;
			}

			return result;
		}

		geometry_vertex export_vertex(const iw7::GfxWorldVertex& vertex)
		{
			const auto normal = unpack_unit_vec(vertex.normal);
			const auto tangent = unpack_unit_vec(vertex.tangent);
			geometry_vertex result;
			for (auto i = 0; i < 3; i++)
			{
				result.position[i] = vertex.xyz[i];
				result.normal[i] = normal[i];
				result.tangent[i] = tangent[i];
			}

			result.binormal_sign = vertex.binormalSign < 0.0f ? -1.0f : 1.0f;
			result.uv = {vertex.texCoord[0], vertex.texCoord[1]};
			result.lightmap_uv = {vertex.lmapCoord[0], vertex.lmapCoord[1]};
			result.color = {vertex.color.array[0], vertex.color.array[1], vertex.color.array[2], vertex.color.array[3]};
			return result;
		}

		// The pipeline rejects triangles with no area (IW7 keeps some between strips).
		bool degenerate(const iw7::GfxWorldVertex& a, const iw7::GfxWorldVertex& b, const iw7::GfxWorldVertex& c)
		{
			float u[3]{};
			float v[3]{};
			for (auto i = 0; i < 3; i++)
			{
				u[i] = b.xyz[i] - a.xyz[i];
				v[i] = c.xyz[i] - a.xyz[i];
			}

			const float cross[3]{u[1] * v[2] - u[2] * v[1], u[2] * v[0] - u[0] * v[2], u[0] * v[1] - u[1] * v[0]};
			return cross[0] * cross[0] + cross[1] * cross[1] + cross[2] * cross[2] < 1.0e-12f;
		}

		// The surfaces of a surface (none when none of its triangles has an area): the surface, and for each layer of a
		// layered material that its vertices reveal, the same triangles in the material of the layer, blended over it by
		// the weight of the layer in vertex alpha. The vertex colours of a layered material hold the weights of its
		// layers, not colours.
		std::vector<world_geometry_surface> export_surface(const source_map& source, const iw7::GfxWorld& world, const iw7::GfxSurface& surface,
			const std::size_t lightmap_count, const reflection_probes& probes, extraction& output)
		{
			const auto& tris = surface.tris;
			if (surface.transientZone >= world.draw.transientZoneCount)
			{
				throw std::runtime_error(std::format("surface transient zone {} is out of range", surface.transientZone));
			}

			// Replay maps have no refraction layers.
			const auto* resolved = source.resolve(iw7::ASSET_TYPE_MATERIAL, surface.material);
			if (classify_material(resolved).kind == surface_kind::distortion)
			{
				return {};
			}

			const auto layers = material_layer_count(resolved);

			// IW7_LAYER_DUMP=1 logs the layer vertex data of layered surfaces (offset, count, first bytes): the layout
			// study of GfxWorldTransientZone.vld.
			if (layers > 1 && std::getenv("IW7_LAYER_DUMP"))
			{
				static std::size_t logged{};
				const auto* layer_zone = source.resolve(iw7::ASSET_TYPE_GFXWORLD_TRANSIENT_ZONE, world.draw.transientZones[surface.transientZone]);
				if (layer_zone)
				{
					if (logged++ < 40)
					{
						std::string bytes;
						for (auto b = 0u; layer_zone->vld.data && b < 48 && tris.vertexLayerData + b < layer_zone->vertexLayerDataSize; b++)
						{
							bytes += std::format("{:02x}", layer_zone->vld.data[tris.vertexLayerData + b]);
						}

						ZONETOOL_INFO("iw7 layers: %s (%s) layers %u verts %u first %u offset %u zone %u size %u: %s",
							resolved ? resolved->name : "?", resolved && resolved->techniqueSet ? resolved->techniqueSet->name : "?",
							layers, tris.vertexCount, tris.firstVertex, tris.vertexLayerData, surface.transientZone,
							layer_zone->vertexLayerDataSize, bytes.data());
					}

					// The material's constants and textures (semantic, the first and last characters of the shader's texture
					// name: the last is the layer digit, image), once per material.
					static std::unordered_set<const iw7::Material*> described;
					if (resolved && described.insert(resolved).second)
					{
						ZONETOOL_INFO("iw7 layers: material %s (%s)", resolved->name, resolved->techniqueSet ? resolved->techniqueSet->name : "?");
						for (auto c = 0; resolved->constantTable && c < resolved->constantCount; c++)
						{
							const auto& constant = resolved->constantTable[c];
							ZONETOOL_INFO("iw7 layers:   constant %.12s %08x (%g %g %g %g)", constant.name, constant.nameHash, constant.literal[0],
								constant.literal[1], constant.literal[2], constant.literal[3]);
						}

						for (auto t = 0; resolved->textureTable && t < resolved->textureCount; t++)
						{
							const auto& texture = resolved->textureTable[t];
							const auto* image = source.resolve(iw7::ASSET_TYPE_IMAGE, texture.image);
							ZONETOOL_INFO("iw7 layers:   texture semantic %u name %c..%c sampler %u %s", texture.semantic, texture.nameStart,
								texture.nameEnd, texture.samplerState, image && image->name ? image->name : "?");
							// Alpha, reveal and thickness maps: the format and the mean of each channel and of |R - G|.
							if (image && texture.semantic == iw7::TS_ALPHA_REVEAL_THICKNESS_MAP)
							{
								const auto faces = load_image_faces(*image, DXGI_FORMAT_R8G8B8A8_UNORM, 64);
								if (faces.size() == 1)
								{
									const auto* top = faces.front().GetImage(0, 0, 0);
									std::array<double, 5> sums{};
									for (std::size_t y = 0; y < top->height; y++)
									{
										const auto* row = top->pixels + y * top->rowPitch;
										for (std::size_t x = 0; x < top->width; x++)
										{
											for (auto c = 0; c < 4; c++)
											{
												sums[c] += row[x * 4 + c];
											}

											sums[4] += std::abs(row[x * 4] - row[x * 4 + 1]);
										}
									}

									const auto texels = static_cast<double>(top->width * top->height);
									ZONETOOL_INFO("iw7 layers:     format %d: mean r %.1f g %.1f b %.1f a %.1f |r-g| %.2f", image->imageFormat,
										sums[0] / texels, sums[1] / texels, sums[2] / texels, sums[3] / texels, sums[4] / texels);
								}
							}
						}
					}
				}
			}

			const auto zone = source.resolve(iw7::ASSET_TYPE_GFXWORLD_TRANSIENT_ZONE,
				world.draw.transientZones[surface.transientZone]);
			if (!zone || !zone->vd.vertices || tris.firstVertex + tris.vertexCount > zone->vertexCount)
			{
				throw std::runtime_error("surface vertices are out of range");
			}

			// The indices of a surface count from its first vertex, in a vertex block that surfaces share: the surface
			// keeps the vertices that its triangles use.
			const auto index_count = static_cast<std::size_t>(tris.triCount) * 3;
			if (tris.baseIndex + index_count > world.draw.indexCount)
			{
				throw std::runtime_error("surface indices are out of range");
			}

			std::vector<geometry_vertex> vertices;
			std::vector<std::uint32_t> indices;
			std::unordered_map<std::uint16_t, std::size_t> remap;
			std::vector<std::uint16_t> order;
			const auto vertex = [&](const std::uint16_t index) -> const iw7::GfxWorldVertex&
			{
				if (index >= tris.vertexCount)
				{
					throw std::runtime_error(std::format("surface index {} is not below its vertex count {}", index,
						tris.vertexCount));
				}

				return zone->vd.vertices[tris.firstVertex + index];
			};

			// The probe of a surface is the probe at the centre of its vertices.
			float center[3]{};
			for (std::size_t t = 0; t < index_count; t += 3)
			{
				const auto triangle = world.draw.indices + tris.baseIndex + t;
				if (degenerate(vertex(triangle[0]), vertex(triangle[1]), vertex(triangle[2])))
				{
					continue;
				}

				for (auto corner = 0; corner < 3; corner++)
				{
					const auto [found, inserted] = remap.try_emplace(triangle[corner], remap.size());
					if (inserted)
					{
						vertices.push_back(export_vertex(vertex(triangle[corner])));
						order.push_back(triangle[corner]);
						if (layers > 1)
						{
							vertices.back().color = {255, 255, 255, 255};
						}

						for (auto axis = 0; axis < 3; axis++)
						{
							center[axis] += vertex(triangle[corner]).xyz[axis];
						}
					}

					indices.push_back(static_cast<std::uint32_t>(found->second));
				}
			}

			if (indices.empty())
			{
				return {};
			}

			// A surface beyond the range of the world (Parkour's mountains) is drawn into the sky, as the distant models
			// are. The sky's own surface stays.
			const auto beyond = std::ranges::any_of(order, [&](const std::uint16_t index)
			{
				const auto& xyz = vertex(index).xyz;
				return std::abs(xyz[0]) > largest_world_coordinate || std::abs(xyz[1]) > largest_world_coordinate ||
					std::abs(xyz[2]) > largest_world_coordinate;
			});
			if (beyond && classify_material(resolved).kind != surface_kind::sky)
			{
				mesh_surface distant;
				distant.material = resolved;
				for (const auto index : order)
				{
					const auto& source_vertex = vertex(index);
					const auto normal = unpack_unit_vec(source_vertex.normal);
					distant.vertices.push_back({{source_vertex.xyz[0], source_vertex.xyz[1], source_vertex.xyz[2]},
						{normal[0], normal[1], normal[2]}, {source_vertex.texCoord[0], source_vertex.texCoord[1]}});
				}

				for (std::size_t t = 0; t + 2 < indices.size(); t += 3)
				{
					distant.triangles.push_back({static_cast<std::uint16_t>(indices[t]), static_cast<std::uint16_t>(indices[t + 1]),
						static_cast<std::uint16_t>(indices[t + 2])});
				}

				ZONETOOL_WARNING("iw7: a world surface of \"%s\" (%zu triangles) reaches beyond %.0f units: the sky draws it",
					resolved && resolved->name ? resolved->name : "?", distant.triangles.size(), largest_world_coordinate);
				output.distant_surfaces.push_back(std::move(distant));
				return {};
			}

			const auto material_name = output.add_material(source, surface.material);
			output.world_materials.insert(material_name);
			for (auto& value : center)
			{
				value /= static_cast<float>(vertices.size());
			}

			const auto lightmap = surface.lightmapIndex < lightmap_count ? static_cast<int>(surface.lightmapIndex) : -1;
			const auto probe = probes.find(center);
			std::vector<world_geometry_surface> result;
			result.push_back({material_name, lightmap, static_cast<int>(probe), vertices, indices});

			// The weight of layer 4 is the x of its c2 colour (B8G8R8A8: the third byte); the other bytes hold 0 on Carnage.
			constexpr std::size_t fourth_weight_byte = 2;

			// Each layer has its own UV in the transient zone's layer vertex data (vld), per vertex as its IW7 vertex
			// declaration routes it (layer_vertex_layout). The base UV put the decal sheets of layers (Frontier's wayfinding
			// signs) as repeated, cut slices across the panels (owner test of cbf4948); a UV read one tangent off cut
			// Carnage's graffiti, a layer after a normal-mapped one.
			const auto layout = material_layer_vertex_layout(resolved);
			const auto layer_bytes = [&](const std::uint16_t index, const unsigned int offset, const std::size_t size) -> const std::uint8_t*
			{
				const auto at = static_cast<std::size_t>(tris.vertexLayerData) + static_cast<std::size_t>(index) * layout.size + offset;
				return zone->vld.data && layout.size && at + size <= zone->vertexLayerDataSize ? zone->vld.data + at : nullptr;
			};
			const auto layer_uv = [&](const std::uint16_t index, const int layer) -> std::optional<std::array<float, 2>>
			{
				const auto* bytes = layer_bytes(index, layer_vertex_layout::uv(layer), 8);
				if (!bytes)
				{
					return std::nullopt;
				}

				std::array<float, 2> uv{};
				std::memcpy(uv.data(), bytes, sizeof(uv));
				return std::isfinite(uv[0]) && std::isfinite(uv[1]) ? std::optional(uv) : std::nullopt;
			};

			// A stock layered surface (MW120R_STOCK_WORLD) draws as one surface with Replay's twc4 world shader, which blends
			// its layers as IW7 does (weights, reveal ramp, per-layer UVs and tangent frames): its vertices keep IW7's layer
			// weights in their colour, each layer's UV and each layer's tangent transform. IW7 keeps the transform of each
			// normal-mapped layer after the material's first (whose frame is the vertex's own) as four bytes of a 2x2 matrix
			// decoded byte / 127 - 1; Replay decodes byte * 2 / 255 - 1. The layers are materials of their own, for their
			// images, without surfaces.
			if (lightmap >= 0 && classify_material(resolved).kind == surface_kind::opaque && material_stock_layered(resolved))
			{
				std::array<std::optional<unsigned int>, max_material_layer> tangents{};
				for (auto layer = 0, normal_mapped = 0; layer < layers; layer++)
				{
					if (material_layer_normal_map(resolved, layer))
					{
						if (normal_mapped++ && layer)
						{
							tangents[layer] = layer_vertex_layout::tangent(layers, static_cast<unsigned int>(normal_mapped - 2));
						}
					}
				}

				auto& merged = result.front();
				for (std::size_t v = 0; v < merged.vertices.size(); v++)
				{
					const auto& source_vertex = vertex(order[v]);
					auto& target = merged.vertices[v];
					target.color = {source_vertex.color.array[0], source_vertex.color.array[1], source_vertex.color.array[2],
						source_vertex.color.array[3]};
					auto& uvs = target.layer_uvs.emplace();
					auto& transforms = target.layer_transforms.emplace();
					for (auto layer = 1; layer < layers; layer++)
					{
						const auto uv = layer_uv(order[v], layer);
						uvs.push_back(uv ? std::array<double, 2>{(*uv)[0], (*uv)[1]} : target.uv);
						std::uint32_t transform{};
						if (const auto* bytes = tangents[layer] ? layer_bytes(order[v], *tangents[layer], 4) : nullptr)
						{
							for (auto k = 0; k < 4; k++)
							{
								const auto value = bytes[k] / 127.0f - 1.0f;
								transform |= static_cast<std::uint32_t>(std::clamp(std::lround((value + 1.0f) * 127.5f), 0l, 255l)) << (8 * k);
							}
						}

						transforms.push_back(transform);
					}
				}

				output.stock_layered_materials.emplace(material_name, layers);
				for (auto layer = 1; layer < layers; layer++)
				{
					output.world_materials.insert(output.add_material_layer(source, surface.material, layer));
				}

				return result;
			}

			for (auto layer = 1; layer < layers; layer++)
			{
				// Green, blue and alpha weigh the second to fourth layers, the layer vertex data the fifth.
				const auto weight = [&](const std::uint16_t index) -> std::uint8_t
				{
					if (layer < max_material_layer)
					{
						return vertex(index).color.array[layer];
					}

					const auto* bytes = layout.fourth_weight ? layer_bytes(index, *layout.fourth_weight, 4) : nullptr;
					return bytes ? bytes[fourth_weight_byte] : std::uint8_t{};
				};

				if (std::ranges::none_of(order, [&](const auto index) { return weight(index) != 0; }))
				{
					continue;
				}

				// A layer that blends by its reveal map keeps its sharpness k in red as sqrt(k / 2) (IW7 maps keep 0.015 to 1:
				// within 1% down to 0.015): the pipeline ramps the reveal by the weight (kRevealLayerFlag), and IW7 tints no
				// layer by its vertex colour.
				const auto reveal = material_layer_reveal(resolved, layer);
				const auto red = reveal
					? static_cast<std::uint8_t>(std::lround(std::sqrt(std::clamp(*reveal / 2.0f, 0.0f, 1.0f)) * 255.0f))
					: std::uint8_t{255};
				auto overlay = vertices;
				for (std::size_t v = 0; v < overlay.size(); v++)
				{
					overlay[v].color = {red, 255, 255, weight(order[v])};
					if (const auto uv = layer_uv(order[v], layer))
					{
						overlay[v].uv = {(*uv)[0], (*uv)[1]};
					}
				}

				const auto layer_name = output.add_material_layer(source, surface.material, layer);
				output.world_materials.insert(layer_name);
				result.push_back({layer_name, lightmap, static_cast<int>(probe),
					std::move(overlay), indices});
			}

			return result;
		}

		float half_to_float(const std::uint16_t half)
		{
			const std::uint32_t sign = (half & 0x8000u) << 16;
			const std::uint32_t exponent = (half >> 10) & 0x1Fu;
			const std::uint32_t mantissa = half & 0x3FFu;
			std::uint32_t bits{};
			if (exponent == 0)
			{
				float value = std::ldexp(static_cast<float>(mantissa), -24);
				return sign ? -value : value;
			}

			bits = exponent == 31 ? sign | 0x7F800000u | (mantissa << 13) : sign | ((exponent + 112) << 23) | (mantissa << 13);
			float value{};
			std::memcpy(&value, &bits, sizeof(value));
			return value;
		}

		// The real SH basis of the pipeline's light grid (iw3_lightgrid.cpp EvaluateShBasis).
		std::array<float, 9> sh_basis(const std::array<float, 3>& d)
		{
			const auto [x, y, z] = d;
			return {0.2820947918f, -0.4886025119f * y, 0.4886025119f * z, -0.4886025119f * x, 1.0925484306f * x * y,
				-1.0925484306f * y * z, 0.3153915653f * (3.0f * z * z - 1.0f), -1.0925484306f * x * z, 0.5462742153f * (x * x - y * y)};
		}

		// The 56 directions of an IW3 light grid colour, in its order (iw3_lightgrid.cpp LightGridProjection): the shell
		// texels of a 4x4x4 cube, which IW3's model shaders index with the world normal.
		const std::array<std::array<float, 3>, 56>& grid_directions()
		{
			static const auto directions = []
			{
				std::array<std::array<float, 3>, 56> result{};
				std::size_t index{};
				for (auto z = 0; z < 4; z++)
				{
					for (auto y = 0; y < 4; y++)
					{
						for (auto x = 0; x < 4; x++)
						{
							if (x > 0 && x < 3 && y > 0 && y < 3 && z > 0 && z < 3)
							{
								continue;
							}

							std::array<float, 3> direction{x * (2.0f / 3.0f) - 1.0f, y * (2.0f / 3.0f) - 1.0f, z * (2.0f / 3.0f) - 1.0f};
							const auto length = std::sqrt(direction[0] * direction[0] + direction[1] * direction[1] + direction[2] * direction[2]);
							for (auto& component : direction)
							{
								component /= length;
							}

							result[index++] = direction;
						}
					}
				}

				return result;
			}();
			return directions;
		}

		// IW7's GPU light grid: L2 SH probes (32 halves each: the 9 coefficients of red, green and blue, then a sky weight)
		// at their positions, in a tetrahedral mesh (neighbour i lies across the face opposite vertex i; the indices keep
		// flags in their top bits). Frontier: 71737 probes, 426507 tetrahedra. IW7 keeps the L1 terms negated against the
		// pipeline's basis: with them as stored, the probes that see the sky point away from the sun (mean cosine -0.82 on
		// Frontier, +0.82 negated).
		class iw7_light_grid
		{
		public:
			// `intensity`: the map's light-grid intensity tweak (extraction::light_grid_intensity).
			iw7_light_grid(const iw7::GfxLightGridProbeData& data, const float intensity)
			{
				if (!data.probes || !data.probePositions || !data.tetrahedrons || !data.tetrahedronNeighbors || !data.probeCount ||
					!data.tetrahedronCount)
				{
					return;
				}

				this->positions_.resize(data.probeCount);
				this->sh_.resize(data.probeCount);
				for (auto p = 0u; p < data.probeCount; p++)
				{
					for (auto c = 0; c < 3; c++)
					{
						this->positions_[p][c] = data.probePositions[p].origin[c];
					}

					for (auto channel = 0; channel < 3; channel++)
					{
						for (auto k = 0; k < 9; k++)
						{
							const auto value = half_to_float(data.probes[p].coeffs[channel * 9 + k]);
							this->sh_[p][channel * 9 + k] = std::isfinite(value) ? (k >= 1 && k <= 3 ? -value : value) * intensity : 0.0f;
						}
					}
				}

				this->tetrahedra_.resize(data.tetrahedronCount);
				this->neighbors_.resize(data.tetrahedronCount);
				for (auto t = 0u; t < data.tetrahedronCount; t++)
				{
					for (auto v = 0; v < 4; v++)
					{
						this->tetrahedra_[t][v] = data.tetrahedrons[t].indexFlags[v] & 0x0FFFFFFFu;
						this->neighbors_[t][v] = data.tetrahedronNeighbors[t].neighbors[v];
						if (this->tetrahedra_[t][v] >= data.probeCount)
						{
							this->positions_.clear();
							return;
						}
					}
				}
			}

			bool empty() const
			{
				return this->positions_.empty();
			}

			// The SH at a point: the probes of the tetrahedron that holds it, by their barycentric weights, found by walking
			// the mesh from the last tetrahedron (the grid visits its points in order).
			std::optional<std::array<float, 27>> sample(const std::array<double, 3>& point)
			{
				auto tetrahedron = this->hint_;
				for (auto step = 0; step < 8192; step++)
				{
					const auto weights = this->barycentric(tetrahedron, point);
					if (!weights)
					{
						return std::nullopt;
					}

					const auto lowest = static_cast<std::size_t>(std::ranges::min_element(*weights) - weights->begin());
					if ((*weights)[lowest] >= -1e-6)
					{
						this->hint_ = tetrahedron;
						std::array<float, 27> result{};
						for (auto v = 0; v < 4; v++)
						{
							const auto& sh = this->sh_[this->tetrahedra_[tetrahedron][v]];
							for (auto k = 0; k < 27; k++)
							{
								result[k] += static_cast<float>((*weights)[v]) * sh[k];
							}
						}

						return result;
					}

					const auto next = this->neighbors_[tetrahedron][lowest];
					if (next >= this->tetrahedra_.size())
					{
						return std::nullopt;
					}

					tetrahedron = next;
				}

				return std::nullopt;
			}

		private:
			std::optional<std::array<double, 4>> barycentric(const std::size_t tetrahedron, const std::array<double, 3>& point) const
			{
				const auto& indices = this->tetrahedra_[tetrahedron];
				const auto& a = this->positions_[indices[0]];
				std::array<std::array<double, 3>, 3> m{};
				std::array<double, 3> r{};
				for (auto c = 0; c < 3; c++)
				{
					for (auto v = 0; v < 3; v++)
					{
						m[c][v] = this->positions_[indices[v + 1]][c] - a[c];
					}

					r[c] = point[c] - a[c];
				}

				const auto det = m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) - m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
					m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
				if (std::abs(det) < 1e-9)
				{
					return std::nullopt;
				}

				// Cramer's rule for m * l = r.
				const auto solve = [&](const int column)
				{
					auto n = m;
					for (auto c = 0; c < 3; c++)
					{
						n[c][column] = r[c];
					}

					return (n[0][0] * (n[1][1] * n[2][2] - n[1][2] * n[2][1]) - n[0][1] * (n[1][0] * n[2][2] - n[1][2] * n[2][0]) +
						n[0][2] * (n[1][0] * n[2][1] - n[1][1] * n[2][0])) / det;
				};

				const auto l1 = solve(0), l2 = solve(1), l3 = solve(2);
				return std::array<double, 4>{1.0 - l1 - l2 - l3, l1, l2, l3};
			}

			std::vector<std::array<double, 3>> positions_;
			std::vector<std::array<float, 27>> sh_;
			std::vector<std::array<std::uint32_t, 4>> tetrahedra_;
			std::vector<std::array<std::uint32_t, 4>> neighbors_;
			std::size_t hint_{};
		};

		// The light of the floor under a static model (IW3's ground lighting): the lightmap of the highest lightmapped
		// world surface under the centre of its bounds. IW7 lights its models from its light grid, which does not
		// convert yet.
		class floor_light
		{
		public:
			floor_light(const source_map& source, const iw7::GfxWorld& world, const world_lightmaps& lightmaps)
				: lightmaps_(lightmaps)
			{
				const auto& bounds = world.bounds;
				this->origin_ = {bounds.midPoint[0] - bounds.halfSize[0], bounds.midPoint[1] - bounds.halfSize[1]};
				this->columns_ = std::clamp(static_cast<std::size_t>(bounds.halfSize[0] * 2.0f / cell_size) + 1, std::size_t{1},
					std::size_t{1024});
				this->rows_ = std::clamp(static_cast<std::size_t>(bounds.halfSize[1] * 2.0f / cell_size) + 1, std::size_t{1},
					std::size_t{1024});
				this->cells_.resize(this->columns_ * this->rows_);

				const auto world_surfaces = world.modelCount ? world.models[0].surfaceCount : world.surfaceCount;
				for (auto i = 0u; i < world_surfaces && i < world.surfaceCount; i++)
				{
					const auto& surface = world.dpvs.surfaces[i];
					const auto zone = surface.transientZone < world.draw.transientZoneCount
						? source.resolve(iw7::ASSET_TYPE_GFXWORLD_TRANSIENT_ZONE, world.draw.transientZones[surface.transientZone]) : nullptr;
					if (!zone || !zone->vd.vertices || surface.lightmapIndex >= lightmaps.pages.size() ||
						!classify_material(source.resolve(iw7::ASSET_TYPE_MATERIAL, surface.material)).solid())
					{
						continue;
					}

					const auto& tris = surface.tris;
					for (auto t = 0u; t < tris.triCount; t++)
					{
						const auto index = world.draw.indices + tris.baseIndex + t * 3;
						triangle corners{};
						auto valid = true;
						for (auto k = 0; k < 3 && valid; k++)
						{
							valid = tris.firstVertex + index[k] < zone->vertexCount;
							if (valid)
							{
								const auto& vertex = zone->vd.vertices[tris.firstVertex + index[k]];
								std::copy_n(vertex.xyz, 3, corners.positions[k].begin());
								std::copy_n(vertex.lmapCoord, 2, corners.uvs[k].begin());
							}
						}

						corners.page = surface.lightmapIndex;
						if (valid && horizontal(corners))
						{
							this->add(std::move(corners));
						}
					}
				}
			}

			// A floor: its height and its light as the bytes of IW3 model lighting (the colour halved, as the
			// pipeline doubles the texels; the sun visibility).
			struct floor_hit
			{
				float height{};
				std::array<std::uint8_t, 3> color{};
				std::uint8_t visibility{};
			};

			// The lightmapped surfaces over a point, from the lowest.
			std::vector<floor_hit> floors(const float x, const float y) const
			{
				std::vector<floor_hit> result;
				const auto cell = this->cell_of(x, y);
				if (!cell)
				{
					return result;
				}

				for (const auto index : this->cells_[*cell])
				{
					const auto& candidate = this->triangles_[index];
					const auto& p = candidate.positions;
					const auto area = (p[1][0] - p[0][0]) * (p[2][1] - p[0][1]) - (p[2][0] - p[0][0]) * (p[1][1] - p[0][1]);
					if (std::abs(area) < 1.0e-3f)
					{
						continue;
					}

					const std::array<float, 3> w
					{
						((p[1][0] - x) * (p[2][1] - y) - (p[2][0] - x) * (p[1][1] - y)) / area,
						((p[2][0] - x) * (p[0][1] - y) - (p[0][0] - x) * (p[2][1] - y)) / area,
						((p[0][0] - x) * (p[1][1] - y) - (p[1][0] - x) * (p[0][1] - y)) / area,
					};

					if (w[0] < 0.0f || w[1] < 0.0f || w[2] < 0.0f)
					{
						continue;
					}

					const auto& page = this->lightmaps_.pages[candidate.page];
					float u{};
					float v{};
					for (auto k = 0; k < 3; k++)
					{
						u += w[k] * candidate.uvs[k][0];
						v += w[k] * candidate.uvs[k][1];
					}

					const auto tx = std::min(static_cast<std::size_t>(std::clamp(u, 0.0f, 1.0f) * static_cast<float>(page.width)), page.width - 1);
					const auto ty = std::min(static_cast<std::size_t>(std::clamp(v, 0.0f, 1.0f) * static_cast<float>(page.height)), page.height - 1);
					const auto texel = ty * page.width + tx;
					floor_hit hit{w[0] * p[0][2] + w[1] * p[1][2] + w[2] * p[2][2], {}, page.visibility[texel]};
					for (auto channel = 0; channel < 3; channel++)
					{
						const auto level = std::clamp(light_level(page.ambient[texel][channel]) * 0.5f, 0.0f, 1.0f);
						hit.color[channel] = static_cast<std::uint8_t>(std::lround(level * 255.0f));
					}

					result.push_back(hit);
				}

				std::ranges::sort(result, {}, &floor_hit::height);
				return result;
			}

			// The packed colour of the floor as IW3 packs ground lighting: a D3DCOLOR (blue, green, red and sun
			// visibility bytes; R_SetModelGroundLighting copies them into an A8R8G8B8 texel).
			std::optional<std::uint32_t> ground_lighting(const float* point) const
			{
				const auto hits = this->floors(point[0], point[1]);
				const auto floor = std::ranges::find_if(hits.rbegin(), hits.rend(), [&](const floor_hit& hit)
				{
					return hit.height <= point[2] + 1.0f && hit.height >= point[2] - max_drop;
				});

				if (floor == hits.rend())
				{
					return {};
				}

				return floor->color[2] | floor->color[1] << 8 | floor->color[0] << 16 | static_cast<std::uint32_t>(floor->visibility) << 24;
			}

			// The area of the floors: the lowest and highest x and y.
			const std::array<float, 4>& floor_area() const
			{
				return this->floor_area_;
			}

		private:
			static constexpr float cell_size = 128.0f;
			// The floor is at most this far under the centre of a model.
			static constexpr float max_drop = 1024.0f;

			struct triangle
			{
				std::array<std::array<float, 3>, 3> positions{};
				std::array<std::array<float, 2>, 3> uvs{};
				unsigned int page{};
			};

			const world_lightmaps& lightmaps_;
			std::array<float, 4> floor_area_{FLT_MAX, FLT_MAX, -FLT_MAX, -FLT_MAX};
			std::array<float, 2> origin_{};
			std::size_t columns_{};
			std::size_t rows_{};
			std::vector<triangle> triangles_;
			std::vector<std::vector<unsigned int>> cells_;

			// Floors and ceilings, not walls.
			static bool horizontal(const triangle& corners)
			{
				const auto& p = corners.positions;
				const std::array<float, 3> u{p[1][0] - p[0][0], p[1][1] - p[0][1], p[1][2] - p[0][2]};
				const std::array<float, 3> v{p[2][0] - p[0][0], p[2][1] - p[0][1], p[2][2] - p[0][2]};
				const std::array<float, 3> n{u[1] * v[2] - u[2] * v[1], u[2] * v[0] - u[0] * v[2], u[0] * v[1] - u[1] * v[0]};
				const auto length = std::sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
				return length > 0.0f && std::abs(n[2]) > 0.5f * length;
			}

			std::optional<std::size_t> cell_of(const float x, const float y) const
			{
				const auto column = std::floor((x - this->origin_[0]) / cell_size);
				const auto row = std::floor((y - this->origin_[1]) / cell_size);
				if (column < 0.0f || row < 0.0f || column >= static_cast<float>(this->columns_) || row >= static_cast<float>(this->rows_))
				{
					return {};
				}

				return static_cast<std::size_t>(row) * this->columns_ + static_cast<std::size_t>(column);
			}

			void add(triangle corners)
			{
				const auto& p = corners.positions;
				const auto index = static_cast<unsigned int>(this->triangles_.size());
				const auto clamp_cell = [](const float value, const std::size_t count)
				{
					return static_cast<std::size_t>(std::clamp(value / cell_size, 0.0f, static_cast<float>(count - 1)));
				};

				const auto left = clamp_cell(std::min({p[0][0], p[1][0], p[2][0]}) - this->origin_[0], this->columns_);
				const auto right = clamp_cell(std::max({p[0][0], p[1][0], p[2][0]}) - this->origin_[0], this->columns_);
				const auto bottom = clamp_cell(std::min({p[0][1], p[1][1], p[2][1]}) - this->origin_[1], this->rows_);
				const auto top = clamp_cell(std::max({p[0][1], p[1][1], p[2][1]}) - this->origin_[1], this->rows_);
				for (const auto& corner : p)
				{
					this->floor_area_[0] = std::min(this->floor_area_[0], corner[0]);
					this->floor_area_[1] = std::min(this->floor_area_[1], corner[1]);
					this->floor_area_[2] = std::max(this->floor_area_[2], corner[0]);
					this->floor_area_[3] = std::max(this->floor_area_[3], corner[1]);
				}

				for (auto row = bottom; row <= top; row++)
				{
					for (auto column = left; column <= right; column++)
					{
						this->cells_[row * this->columns_ + column].push_back(index);
					}
				}

				this->triangles_.push_back(std::move(corners));
			}
		};

		// A static model's own light-grid colours: IW7's light grid at the centre of its bounds in the 56 directions of an
		// IW3 light-grid colour, at half the lightmap level through the lightmaps' knee (IW3's light-grid units; IW7's SH
		// taken from above matches its lightmaps). IW7 lit its static models by their own vertex lighting or lightmaps,
		// which hold the direct light of the static lights as well; Replay's lights add that at run time.
		std::optional<std::string> model_lighting_colors(iw7_light_grid& grid, const float* point)
		{
			if (grid.empty())
			{
				return std::nullopt;
			}

			const auto sh = grid.sample({point[0], point[1], point[2]});
			if (!sh)
			{
				return std::nullopt;
			}

			std::string text;
			const auto& directions = grid_directions();
			for (const auto& direction : directions)
			{
				const auto basis = sh_basis(direction);
				for (auto channel = 0; channel < 3; channel++)
				{
					auto value = 0.0f;
					for (auto k = 0; k < 9; k++)
					{
						value += (*sh)[channel * 9 + k] * basis[k];
					}

					const auto level = std::clamp(0.5f * light_level(std::max(value, 0.0f)), 0.0f, 1.0f);
					text += std::format("{:02x}", static_cast<unsigned>(std::lround(level * 255.0f)));
				}
			}

			return text;
		}

		// A static model's own light-grid colours from IW7's vertex lighting (STATIC_MODEL_FLAG_VERTEXLIT_LIGHTING), with
		// the mean sun visibility of its vertices. IW7's vertex-lit pixel shader lights a vertex by ambient + highlight *
		// saturate(dot(N, D)): the colours of GfxStaticModelVertexLighting, with D in the vertex's tangent frame (x and y
		// in the w of the ambient and the highlight, z = sqrt(1 - x^2 - y^2); binormal cross(N, T), negated with the
		// tangent's sign bits); visibility[0] is the sun's. Each of the 56 directions takes the mean over the vertices of
		// the first LOD, at the level of model_lighting_colors.
		std::optional<std::pair<std::string, std::uint8_t>> vertex_lighting_colors(const source_map& source,
			const iw7::GfxStaticModelDrawInst& instance)
		{
			const auto& info = instance.vertexLightingInfo;
			const auto* model = (instance.flags & iw7::STATIC_MODEL_FLAG_VERTEXLIT_LIGHTING) && info.lightingValues && info.numLightingValues
				? source.resolve(iw7::ASSET_TYPE_XMODEL, instance.model) : nullptr;
			if (!model || !model->numLods)
			{
				return std::nullopt;
			}

			const auto unit = [](const iw7::PackedUnitVec& vector)
			{
				std::array<float, 3> result{};
				for (auto i = 0; i < 3; i++)
				{
					result[i] = static_cast<float>((vector.packed >> (10 * i)) & 0x3FF) / 1023.0f * 2.0f - 1.0f;
				}

				return result;
			};

			// The tangent frames (normal, tangent, binormal) of the first LOD's vertices, in the order of its surfaces.
			std::vector<std::array<std::array<float, 3>, 3>> frames;
			const auto add = [&](const auto& vertices)
			{
				for (const auto& vertex : vertices)
				{
					const auto n = unit(vertex.normal);
					const auto t = unit(vertex.tangent);
					const auto sign = (vertex.tangent.packed >> 30) ? -1.0f : 1.0f;
					frames.push_back({n, t, std::array<float, 3>{(n[1] * t[2] - n[2] * t[1]) * sign, (n[2] * t[0] - n[0] * t[2]) * sign,
						(n[0] * t[1] - n[1] * t[0]) * sign}});
				}
			};

			// The values cover the vertices of every LOD in turn (Frontier: 352 values for 350 vertices, a little padding); the
			// first LOD's lead.
			std::size_t first_lod{};
			for (auto l = 0; l < model->numLods && l < 6; l++)
			{
				const auto& lod = model->lodInfo[l];
				const auto surfaces = source.resolve(iw7::ASSET_TYPE_XMODEL_SURFS, lod.modelSurfs);
				const auto streamed = lod.modelSurfs ? source.streamed_surfaces(asset_name(lod.modelSurfs->name)) : nullptr;
				for (std::size_t s = 0; s < lod.numsurfs; s++)
				{
					if (surfaces && surfaces->surfs && s < surfaces->numsurfs)
					{
						const auto& surface = surfaces->surfs[s];
						if ((surface.flags & 8) != 0)
						{
							add(std::span(surface.verts0.packedMotionVerts0, surface.vertCount));
						}
						else
						{
							add(std::span(surface.verts0.packedVerts0, surface.vertCount));
						}
					}
					else if (streamed && s < streamed->size())
					{
						const auto& stream = (*streamed)[s];
						stream.motion ? add(stream.motion_vertices) : add(stream.vertices);
					}
				}

				first_lod = first_lod ? first_lod : frames.size();
			}

			if (frames.size() > info.numLightingValues || info.numLightingValues - frames.size() >= 64 || !first_lod)
			{
				ZONETOOL_WARNING("iw7: the vertex lighting of a placement of \"%s\" has %u values for %zu vertices; it takes the light grid",
					model->name, info.numLightingValues, frames.size());
				return std::nullopt;
			}

			const auto& axis = instance.placement.axis;
			const auto& directions = grid_directions();
			std::vector<std::array<double, 3>> sums(directions.size());
			double visibility{};
			frames.resize(first_lod);
			for (std::size_t v = 0; v < frames.size(); v++)
			{
				const auto& lighting = info.lightingValues[v];
				const auto& [n, t, b] = frames[v];
				const auto x = half_to_float(lighting.ambientColorFloat16[3]);
				const auto y = half_to_float(lighting.highlightColorFloat16[3]);
				const auto z = std::sqrt(std::max(0.0f, 1.0f - x * x - y * y));
				std::array<float, 3> local{x * t[0] + z * n[0] + y * b[0], x * t[1] + z * n[1] + y * b[1], x * t[2] + z * n[2] + y * b[2]};
				std::array<float, 3> along{};
				for (auto c = 0; c < 3; c++)
				{
					along[c] = local[0] * axis[0][c] + local[1] * axis[1][c] + local[2] * axis[2][c];
				}

				const auto length = std::sqrt(along[0] * along[0] + along[1] * along[1] + along[2] * along[2]);
				for (std::size_t d = 0; d < directions.size(); d++)
				{
					const auto& direction = directions[d];
					const auto facing = length > 0.0f ? std::max(0.0f, (direction[0] * along[0] + direction[1] * along[1] +
						direction[2] * along[2]) / length) : 0.0f;
					for (auto c = 0; c < 3; c++)
					{
						const auto ambient = half_to_float(lighting.ambientColorFloat16[c]);
						const auto highlight = half_to_float(lighting.highlightColorFloat16[c]);
						sums[d][c] += (std::isfinite(ambient) ? ambient : 0.0f) + (std::isfinite(highlight) ? highlight : 0.0f) * facing;
					}
				}

				visibility += lighting.visibility[0];
			}

			std::string text;
			for (const auto& sum : sums)
			{
				for (auto c = 0; c < 3; c++)
				{
					const auto value = static_cast<float>(sum[c] / static_cast<double>(frames.size()));
					const auto level = std::clamp(0.5f * light_level(std::max(value, 0.0f)), 0.0f, 1.0f);
					text += std::format("{:02x}", static_cast<unsigned>(std::lround(level * 255.0f)));
				}
			}

			return std::pair(std::move(text), static_cast<std::uint8_t>(std::lround(visibility / static_cast<double>(frames.size()))));
		}

		// IW7 placements scale their model's LOD distances by g_lodDistIndexToScale[flags & 0xF] (index 6 is 1; designers
		// raise it on large props seen from afar: Renaissance 554 of 9221 placements, up to 4).
		float lod_distance_scale(const unsigned short flags)
		{
			constexpr std::array<float, 16> scales{0.25f, 0.375f, 0.5f, 0.625f, 0.75f, 0.875f, 1.0f, 1.125f, 1.25f, 1.5f, 1.75f, 2.0f,
				2.5f, 3.0f, 3.5f, 4.0f};
			return scales[flags & iw7::STATIC_MODEL_FLAG_SCALE_MODIFIER_MASK];
		}

		json export_static_model(const source_map& source, const iw7::GfxWorld& world, const unsigned int index,
			const reflection_probes& probes, const floor_light& floors, iw7_light_grid& grid, extraction& output)
		{
			const auto& instance = world.dpvs.smodelDrawInsts[index];
			const auto ground = floors.ground_lighting(world.dpvs.smodelInsts[index].bounds.midPoint);
			const auto model_name = output.add_model(source, instance.model);

			auto axis = json::array();
			for (const auto& row : instance.placement.axis)
			{
				axis.push_back(vector_json(row, 3));
			}

			const auto bounds = bounds_json(world.dpvs.smodelInsts[index].bounds);
			json result =
			{
				{"model", model_name},
				{"origin", vector_json(instance.placement.origin, 3)},
				{"axis", std::move(axis)},
				{"scale", instance.placement.scale},
				{"cull_dist", instance.cullDist ? static_cast<float>(instance.cullDist) : 1000000.0f},
				// The placement's LOD distance scale: the index in its flags into g_lodDistIndexToScale (the table of
				// IW7's engine generation, as H1's; index 6 is 1).
				{"lod_scale", lod_distance_scale(instance.flags)},
				{"ground_lighting", ground.value_or(0)},
				// IW3's STATIC_MODEL_FLAG_GROUND_LIGHTING.
				{"model_flags", ground ? 1 : 0},
				{"mins", bounds[0]},
				{"maxs", bounds[1]},
				{"primary_light", 0},
				{"reflection_probe", probes.find(instance.placement.origin)},
			};

			if (const auto vertex = vertex_lighting_colors(source, instance))
			{
				// The sun weight of the colours is the alpha of the ground lighting (a D3DCOLOR).
				result["lighting_colors"] = vertex->first;
				result["ground_lighting"] = (ground.value_or(0) & 0x00FFFFFFu) | static_cast<std::uint32_t>(vertex->second) << 24;
			}
			else if (const auto colors = model_lighting_colors(grid, world.dpvs.smodelInsts[index].bounds.midPoint))
			{
				result["lighting_colors"] = *colors;
			}

			return result;
		}
	}

	namespace
	{
		// IW3's light grid: points 32 units apart in x and y and 64 in z, counted from -131072.
		constexpr std::int64_t grid_base = 131072;
		constexpr std::int64_t grid_spacing_xy = 32;
		constexpr std::int64_t grid_spacing_z = 64;
		// The points of a column reach this far over its highest floor.
		constexpr float grid_headroom = 192.0f;
		// The grid reaches this far around the spawn points.
		constexpr float play_margin = 1024.0f;

		void write_bytes(const std::filesystem::path& path, const std::vector<std::uint8_t>& bytes)
		{
			std::filesystem::create_directories(path.parent_path());
			std::ofstream stream(path, std::ios::binary | std::ios::trunc);
			stream.write(reinterpret_cast<const char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
			if (!stream)
			{
				throw std::runtime_error(std::format("cannot write {}", path.string()));
			}
		}

		template <typename T>
		void append(std::vector<std::uint8_t>& bytes, const T value)
		{
			const auto* data = reinterpret_cast<const std::uint8_t*>(&value);
			bytes.insert(bytes.end(), data, data + sizeof(T));
		}

		// IW7's GPU light grid, which Replay keeps in the same form (GfxGpuLightGrid): L2 SH probes (32 halves each: the 9
		// coefficients of red, green and blue, then a sky weight) at their positions, in a tetrahedral mesh (neighbour i
		// lies across the face opposite vertex i; the probe indices keep flags in their top bits). Frontier: 71737
		// probes, 426507 tetrahedra in one zone. It holds the light that the lightmaps bake and the light grid of IW7
		// does not apply again at run time (bounce and sky; the primary lights light models at run time, as in Replay).
		// The extraction keeps it for the pipeline's native light grid (extracted/<bsp>.gpulightgrid.bin): "IW7GLG01",
		// the probe, tetrahedron counts, then the positions (3 floats), the SH (27 floats, in the pipeline's light-grid
		// units and basis), the tetrahedra and their neighbours (4 uints each).
		//
		// Units: IW7's SH taken from above equals its lightmaps (Frontier: the median over the grid points over floors is
		// 1.08 times), and the pipeline's light grid holds half the lightmap level, so the SH is halved. Basis: stock
		// Replay probes and IW7's keep the L1 terms negated against the pipeline's (their sky-lit probes point down in
		// the pipeline's basis: mp_hideout mean z -0.74, IW7 Frontier -0.63), which PackProbe negates when it packs. The
		// map's light-grid intensity tweak scales it, as IW7 scales its grid at run time.
		void export_gpu_light_grid(const source_map& source, extraction& output)
		{
			const auto& data = source.world()->lightGrid.probeData;
			if (!data.probes || !data.probePositions || !data.tetrahedrons || !data.tetrahedronNeighbors || !data.probeCount ||
				!data.tetrahedronCount)
			{
				ZONETOOL_WARNING("iw7: the world has no GPU light grid; models take the light grid of the floors");
				return;
			}

			std::vector<std::uint8_t> bytes;
			const auto put = [&](const auto value)
			{
				const auto* raw = reinterpret_cast<const std::uint8_t*>(&value);
				bytes.insert(bytes.end(), raw, raw + sizeof(value));
			};

			bytes.insert(bytes.end(), {'I', 'W', '7', 'G', 'L', 'G', '0', '1'});
			put(data.probeCount);
			put(data.tetrahedronCount);
			for (auto p = 0u; p < data.probeCount; p++)
			{
				for (const auto component : data.probePositions[p].origin)
				{
					put(component);
				}
			}

			for (auto p = 0u; p < data.probeCount; p++)
			{
				for (auto coefficient = 0; coefficient < 27; coefficient++)
				{
					const auto order = coefficient % 9;
					auto value = half_to_float(data.probes[p].coeffs[coefficient]);
					value = std::isfinite(value) ? value * 0.5f * output.light_grid_intensity : 0.0f;
					put(order >= 1 && order <= 3 ? -value : value);
				}
			}

			for (auto t = 0u; t < data.tetrahedronCount; t++)
			{
				for (const auto index : data.tetrahedrons[t].indexFlags)
				{
					if ((index & 0x0FFFFFFFu) >= data.probeCount)
					{
						ZONETOOL_WARNING("iw7: the GPU light grid names a probe it does not have; models take the light grid of the floors");
						return;
					}

					put(index & 0x0FFFFFFFu);
				}
			}

			for (auto t = 0u; t < data.tetrahedronCount; t++)
			{
				for (const auto neighbor : data.tetrahedronNeighbors[t].neighbors)
				{
					put(neighbor < data.tetrahedronCount ? neighbor : UINT32_MAX);
				}
			}

			write_bytes(output.extracted / (bsp_name(source) + ".gpulightgrid.bin"), bytes);
			ZONETOOL_INFO("iw7: GPU light grid of %u probes and %u tetrahedra", data.probeCount, data.tetrahedronCount);
		}

		// Writes an IW3 light grid in the layout of the IW3 extraction: rows of columns, each row a run of points from
		// its lowest floor to over its highest one. A point takes the light of the floor under it in all 56
		// directions; the sun lights the points over floors that see it. Beside it, each point keeps IW7's light grid
		// there (<grid>.sh.bin: 27 floats per entry in the pipeline's basis, NaN outside IW7's mesh), which the
		// entities' lighting takes in place of the floor colours.
		void export_light_grid(const source_map& source, const floor_light& floors, iw7_light_grid& grid, extraction& output)
		{
			// The grid covers the play space: the spawn points with a margin. The world reaches far beyond it (set
			// dressing in the distance), which would take more points than Replay's light grid holds.
			const auto play = spawn_bounds(source);
			auto area = floors.floor_area();
			if (play)
			{
				area = {std::max(area[0], (*play)[0] - play_margin), std::max(area[1], (*play)[1] - play_margin),
					std::min(area[2], (*play)[3] + play_margin), std::min(area[3], (*play)[4] + play_margin)};
			}

			const auto lowest = play ? (*play)[2] - 256.0f : -FLT_MAX;
			const auto highest = play ? (*play)[5] + 768.0f : FLT_MAX;
			if (area[0] > area[2] || area[1] > area[3])
			{
				ZONETOOL_WARNING("iw7: the world has no floors for a light grid");
				return;
			}

			const auto index = [](const float value, const std::int64_t spacing)
			{
				return (static_cast<std::int64_t>(std::floor(value)) + grid_base) / spacing;
			};

			const auto first_column = index(area[0], grid_spacing_xy);
			const auto last_column = index(area[2], grid_spacing_xy) + 1;
			const auto first_row = index(area[1], grid_spacing_xy);
			const auto last_row = index(area[3], grid_spacing_xy) + 1;

			std::map<std::array<std::uint8_t, 3>, std::uint16_t> palette_indices;
			std::vector<std::uint8_t> palette;
			const auto palette_index = [&](const std::array<std::uint8_t, 3>& color)
			{
				// 5 bits a channel keep the palette within its 65536 entries.
				const std::array<std::uint8_t, 3> key{static_cast<std::uint8_t>(color[0] & 0xF8), static_cast<std::uint8_t>(color[1] & 0xF8),
					static_cast<std::uint8_t>(color[2] & 0xF8)};
				if (const auto found = palette_indices.find(key); found != palette_indices.end())
				{
					return found->second;
				}

				const auto created = static_cast<std::uint16_t>(palette_indices.size());
				palette_indices.emplace(key, created);
				for (auto sample = 0; sample < 56; sample++)
				{
					// Red, green and blue bytes, as IW3's GfxLightGridColors (R_SetLightGridColors swaps them into its
					// A8R8G8B8 texels).
					palette.push_back(key[0]);
					palette.push_back(key[1]);
					palette.push_back(key[2]);
				}

				return created;
			};

			std::vector<std::uint8_t> row_starts;
			std::vector<std::uint8_t> row_data;
			std::vector<std::uint8_t> entries;
			std::vector<float> sh;
			std::size_t sampled{};
			auto min_z = INT64_MAX;
			auto max_z = INT64_MIN;
			std::size_t entry_count{};
			for (auto row = first_row; row <= last_row; row++)
			{
				const auto y = static_cast<float>(row * grid_spacing_xy - grid_base);
				std::vector<std::vector<floor_light::floor_hit>> columns;
				auto bottom = INT64_MAX;
				auto top = INT64_MIN;
				for (auto c = first_column; c <= last_column; c++)
				{
					auto hits = floors.floors(static_cast<float>(c * grid_spacing_xy - grid_base), y);
					std::erase_if(hits, [&](const auto& hit) { return hit.height < lowest || hit.height > highest; });
					if (!hits.empty())
					{
						bottom = std::min(bottom, index(hits.front().height, grid_spacing_z));
						top = std::max(top, index(hits.back().height + grid_headroom, grid_spacing_z));
					}

					columns.push_back(std::move(hits));
				}

				const auto used = std::ranges::find_if(columns, [](const auto& hits) { return !hits.empty(); });
				if (used == columns.end())
				{
					append<std::uint16_t>(row_starts, 0xFFFF);
					continue;
				}

				const auto last_used = std::find_if(columns.rbegin(), columns.rend(), [](const auto& hits) { return !hits.empty(); }).base();
				top = std::min(top, bottom + 254);
				min_z = std::min(min_z, bottom);
				max_z = std::max(max_z, top);
				if (row_data.size() / 4 > 0xFFFE)
				{
					throw std::runtime_error("the light grid rows exceed the IW3 format");
				}

				const auto height = static_cast<std::uint8_t>(top - bottom + 1);
				append<std::uint16_t>(row_starts, static_cast<std::uint16_t>(row_data.size() / 4));
				append<std::uint16_t>(row_data, static_cast<std::uint16_t>(first_column + (used - columns.begin())));
				append<std::uint16_t>(row_data, static_cast<std::uint16_t>(last_used - used));
				append<std::uint16_t>(row_data, static_cast<std::uint16_t>(bottom));
				append<std::uint16_t>(row_data, height);
				append<std::uint32_t>(row_data, static_cast<std::uint32_t>(entry_count));

				// Runs of columns: the columns with floors hold the points of the whole row, the others none.
				for (auto it = used; it != last_used;)
				{
					const auto filled = !it->empty();
					std::uint8_t run{};
					for (; it != last_used && !it->empty() == filled && run < 255; ++it, ++run)
					{
						const auto x = static_cast<double>((first_column + (it - columns.begin())) * grid_spacing_xy - grid_base);
						for (auto z = bottom; filled && z <= top; z++)
						{
							const auto point = static_cast<float>(z * grid_spacing_z - grid_base);
							const auto floor = std::ranges::find_if(it->rbegin(), it->rend(), [&](const auto& hit) { return hit.height <= point + 32.0f; });
							const auto& hit = floor == it->rend() ? it->front() : *floor;
							append<std::uint16_t>(entries, palette_index(hit.color));
							entries.push_back(static_cast<std::uint8_t>(hit.visibility > 127 ? sun_light : 0));
							entries.push_back(0);
							entry_count++;

							const auto probe = grid.empty() ? std::nullopt : grid.sample({x, static_cast<double>(y), static_cast<double>(point)});
							sampled += probe.has_value();
							for (auto k = 0; k < 27; k++)
							{
								sh.push_back(probe ? (*probe)[k] : std::numeric_limits<float>::quiet_NaN());
							}
						}
					}

					row_data.push_back(run);
					row_data.push_back(filled ? height : 0);
					if (filled)
					{
						row_data.push_back(0);
					}
				}

				row_data.resize((row_data.size() + 3) & ~std::size_t{3});
			}

			if (!entry_count)
			{
				ZONETOOL_WARNING("iw7: the world has no floors for a light grid");
				return;
			}

			const auto stem = bsp_name(source) + ".lightgrid";
			write_bytes(output.extracted / (stem + ".rows.bin"), row_starts);
			write_bytes(output.extracted / (stem + ".raw.bin"), row_data);
			write_bytes(output.extracted / (stem + ".entries.bin"), entries);
			write_bytes(output.extracted / (stem + ".colors.bin"), palette);
			json metadata =
			{
				{"schema", 1},
				{"name", bsp_name(source)},
				{"row_axis", 1},
				{"col_axis", 0},
				{"mins", {first_column, first_row, min_z}},
				{"maxs", {last_column, last_row, max_z}},
				{"row_count", last_row - first_row + 1},
				{"entry_count", entry_count},
				{"color_count", palette_indices.size()},
				{"sun_primary_light_index", sun_light},
				{"has_light_regions", false},
				{"row_starts", {{"bytes", row_starts.size()}, {"file", stem + ".rows.bin"}}},
				{"row_data", {{"bytes", row_data.size()}, {"file", stem + ".raw.bin"}}},
				{"entries", {{"bytes", entries.size()}, {"file", stem + ".entries.bin"}}},
				{"colors", {{"bytes", palette.size()}, {"file", stem + ".colors.bin"}}},
			};

			if (sampled)
			{
				std::vector<std::uint8_t> bytes(sh.size() * sizeof(float));
				std::memcpy(bytes.data(), sh.data(), bytes.size());
				write_bytes(output.extracted / (stem + ".sh.bin"), bytes);
				metadata["sh"] = {{"bytes", bytes.size()}, {"file", stem + ".sh.bin"}};
			}

			write_json(output.extracted / (stem + ".json"), metadata);
			ZONETOOL_INFO("iw7: light grid of %zu points with %zu colours, %zu with IW7's light grid", entry_count, palette_indices.size(), sampled);
		}
	}

	std::string bsp_name(const source_map& source)
	{
		return "maps/mp/" + source.name() + ".d3dbsp";
	}

	std::string file_name(const std::string_view name)
	{
		std::string result(name);
		for (auto& character : result)
		{
			if (std::string_view(R"(*?"<>|:\)").find(character) != std::string_view::npos)
			{
				character = '_';
			}
		}

		return result;
	}

	std::string extraction::add_material(const source_map& source, iw7::Material* header)
	{
		const auto material = source.resolve(iw7::ASSET_TYPE_MATERIAL, header);
		const auto name = file_name(asset_name(header ? iw7::get_asset_name(iw7::ASSET_TYPE_MATERIAL, header) : nullptr));
		auto& entry = this->materials[name];
		entry = entry ? entry : material;
		return name;
	}

	std::string extraction::add_material_layer(const source_map& source, iw7::Material* header, const int layer)
	{
		const auto name = this->add_material(source, header) + std::format("__layer{}", layer);
		this->materials[name] = source.resolve(iw7::ASSET_TYPE_MATERIAL, header);
		this->material_layers[name] = layer;
		return name;
	}

	std::string extraction::add_model(const source_map& source, iw7::XModel* header)
	{
		const auto model = source.resolve(iw7::ASSET_TYPE_XMODEL, header);
		const auto name = file_name(asset_name(header ? iw7::get_asset_name(iw7::ASSET_TYPE_XMODEL, header) : nullptr));
		auto& entry = this->models[name];
		entry = entry ? entry : model;
		return name;
	}

	std::optional<std::array<float, 6>> spawn_bounds(const source_map& source)
	{
		const auto* map_ents = source.map_ents();
		if (!map_ents || !map_ents->spawnList.spawnsCount)
		{
			return std::nullopt;
		}

		const auto& spawns = map_ents->spawnList;
		std::array<float, 6> bounds{FLT_MAX, FLT_MAX, FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX};
		for (auto i = 0u; i < spawns.spawnsCount; i++)
		{
			for (auto axis = 0; axis < 3; axis++)
			{
				bounds[axis] = std::min(bounds[axis], spawns.spawns[i].origin[axis]);
				bounds[axis + 3] = std::max(bounds[axis + 3], spawns.spawns[i].origin[axis]);
			}
		}

		return bounds;
	}

	bool beyond_world(const iw7::GfxWorld& world, const unsigned int instance)
	{
		const auto& bounds = world.dpvs.smodelInsts[instance].bounds;
		for (auto axis = 0; axis < 3; axis++)
		{
			if (std::abs(bounds.midPoint[axis]) + bounds.halfSize[axis] > largest_world_coordinate)
			{
				return true;
			}
		}

		return false;
	}

	void collect_static_models(const source_map& source, extraction& output)
	{
		const auto& world = *source.world();
		for (auto i = 0u; i < world.dpvs.smodelCount; i++)
		{
			output.add_model(source, world.dpvs.smodelDrawInsts[i].model);
		}
	}

	void write_text(const std::filesystem::path& path, const std::string_view text)
	{
		// The folder is made only when the file does not open (thousands of materials and models go into a few).
		std::ofstream stream(path, std::ios::binary | std::ios::trunc);
		if (!stream)
		{
			std::filesystem::create_directories(path.parent_path());
			stream.clear();
			stream.open(path, std::ios::binary | std::ios::trunc);
		}
		stream.write(text.data(), static_cast<std::streamsize>(text.size()));
		if (!stream)
		{
			throw std::runtime_error(std::format("cannot write {}", path.string()));
		}
	}

	void write_json(const std::filesystem::path& path, const json& value)
	{
		write_text(path, value.dump());
	}

	namespace
	{
		std::mutex json_writes_mutex;
		std::deque<std::future<void>> json_writes;
	}

	void write_json_async(std::filesystem::path path, json value, const std::size_t limit)
	{
		write_async([path = std::move(path), value = std::move(value)] { write_json(path, value); }, limit);
	}

	void write_async(std::move_only_function<void()> write, const std::size_t limit)
	{
		std::future<void> oldest;
		{
			std::lock_guard lock(json_writes_mutex);
			// The values wait in memory (a large model's is gigabytes as a json value): `limit` at a time.
			if (json_writes.size() >= limit)
			{
				oldest = std::move(json_writes.front());
				json_writes.pop_front();
			}
		}

		if (oldest.valid())
		{
			oldest.get();
		}

		auto pending = std::async(std::launch::async, [write = std::move(write)]() mutable
		{
			write();
		});
		std::lock_guard lock(json_writes_mutex);
		json_writes.push_back(std::move(pending));
	}

	void finish_json_writes()
	{
		std::deque<std::future<void>> pending;
		{
			std::lock_guard lock(json_writes_mutex);
			pending.swap(json_writes);
		}

		std::exception_ptr failure;
		for (auto& write : pending)
		{
			try
			{
				write.get();
			}
			catch (...)
			{
				if (!failure)
				{
					failure = std::current_exception();
				}
			}
		}

		if (failure)
		{
			std::rethrow_exception(failure);
		}
	}

	void export_world(const source_map& source, extraction& output)
	{
		const auto& world = *source.world();
		const auto& dpvs = world.dpvs;
		auto lightmaps = export_lightmaps(source, output);
		auto probes = export_reflection_probes(source, output);
		const floor_light floors(source, world, lightmaps);
		iw7_light_grid grid(world.lightGrid.probeData, output.light_grid_intensity);
		export_light_grid(source, floors, grid, output);
		export_gpu_light_grid(source, output);

		// IW7_LAYER_DUMP=1: bytes of layer vertex data per vertex of each layered surface (the gap to the next surface's
		// data in its transient zone), by technique set and layer count.
		if (std::getenv("IW7_LAYER_DUMP"))
		{
			std::map<std::uint32_t, std::vector<std::tuple<std::uint32_t, std::uint32_t, unsigned, std::string>>> by_zone;
			for (auto i = 0u; i < world.surfaceCount; i++)
			{
				const auto& surface = dpvs.surfaces[i];
				const auto* material = source.resolve(iw7::ASSET_TYPE_MATERIAL, surface.material);
				const auto layers = material_layer_count(material);
				const auto technique = material && material->techniqueSet ? std::string(asset_name(material->techniqueSet->name)) : "?";
				by_zone[surface.transientZone].emplace_back(surface.tris.vertexLayerData, surface.tris.vertexCount, layers, technique);
			}

			std::map<std::string, std::map<std::uint32_t, std::size_t>> strides;
			for (auto& [zone, list] : by_zone)
			{
				std::ranges::sort(list);
				list.erase(std::unique(list.begin(), list.end(), [](const auto& a, const auto& b) { return std::get<0>(a) == std::get<0>(b); }),
					list.end());
				for (std::size_t s = 0; s + 1 < list.size(); s++)
				{
					const auto& [offset, count, layers, technique] = list[s];
					if (layers > 1 && count)
					{
						strides[std::format("{} layers {}", technique, layers)][(std::get<0>(list[s + 1]) - offset) / count]++;
					}
				}
			}

			for (const auto& [key, histogram] : strides)
			{
				std::string text;
				for (const auto& [stride, n] : histogram)
				{
					text += std::format(" {}B x{}", stride, n);
				}

				ZONETOOL_INFO("iw7 layer stride: %s:%s", key.data(), text.data());
			}
		}

		// The index of each surface among the surfaces with an area.
		std::vector<world_geometry_surface> surfaces;
		auto surface_indices = json::array();
		std::vector<std::size_t> first_surface(world.surfaceCount + 1);
		for (auto i = 0u; i < world.surfaceCount; i++)
		{
			first_surface[i] = surfaces.size();
			const auto decal_range = i >= dpvs.litDecalSurfsBegin && i < dpvs.litDecalSurfsEnd;
			for (auto& surface : export_surface(source, world, dpvs.surfaces[i], lightmaps.entries.size(), probes, output))
			{
				if (decal_range)
				{
					output.decal_range_materials.insert(surface.material);
				}

				surface_indices.push_back(surfaces.size());
				surfaces.push_back(std::move(surface));
			}
		}

		first_surface[world.surfaceCount] = surfaces.size();

		// IW7_DRAW_RANGES=1: IW7's draw ranges of the world's surfaces and the materials of its lit decal range.
		if (std::getenv("IW7_DRAW_RANGES"))
		{
			ZONETOOL_INFO("iw7: draw ranges: opaque %u-%u, lit decals %u-%u, translucent %u-%u, emissive %u-%u of %u",
				dpvs.litOpaqueSurfsBegin, dpvs.litOpaqueSurfsEnd, dpvs.litDecalSurfsBegin, dpvs.litDecalSurfsEnd,
				dpvs.litTransSurfsBegin, dpvs.litTransSurfsEnd, dpvs.emissiveSurfsBegin, dpvs.emissiveSurfsEnd, world.surfaceCount);
			std::map<std::string, std::size_t> decals;
			for (auto i = dpvs.litDecalSurfsBegin; i < dpvs.litDecalSurfsEnd && i < world.surfaceCount; i++)
			{
				const auto* material = dpvs.surfaces[i].material;
				decals[std::string(asset_name(material ? iw7::get_asset_name(iw7::ASSET_TYPE_MATERIAL, material) : nullptr))]++;
			}

			for (const auto& [name, count] : decals)
			{
				ZONETOOL_INFO("iw7: lit decal range: %s x%zu", name.data(), count);
			}
		}

		// The static models whose model exported (export_models runs first).
		auto models = json::array();
		auto model_indices = json::array();
		std::size_t ground_lit{};
		std::size_t grid_lit{};
		// IW7's own lighting of its static models (STATIC_MODEL_FLAG_*_LIGHTING), for the log.
		std::array<std::size_t, 4> iw7_lighting{};
		// The placements by their LOD distance scale (STATIC_MODEL_FLAG_SCALE_MODIFIER_MASK), for the log.
		std::array<std::size_t, 16> lod_scales{};
		// The others by their flags (without the scale): how many, with lightmap or vertex lighting data, and one model.
		std::map<unsigned, std::tuple<std::size_t, std::size_t, std::size_t, std::string>> other_lighting;
		for (auto i = 0u; i < dpvs.smodelCount; i++)
		{
			// The sky draws the static models beyond the range of the world.
			if (beyond_world(world, i))
			{
				continue;
			}

			const auto& draw = dpvs.smodelDrawInsts[i];
			const auto flags = draw.flags;
			lod_scales[flags & iw7::STATIC_MODEL_FLAG_SCALE_MODIFIER_MASK]++;
			const auto lighting = (flags & iw7::STATIC_MODEL_FLAG_LIGHTGRID_LIGHTING) ? 0 : (flags & iw7::STATIC_MODEL_FLAG_VERTEXLIT_LIGHTING) ? 1
				: (flags & iw7::STATIC_MODEL_FLAG_LIGHTMAP_LIGHTING) ? 2 : 3;
			iw7_lighting[lighting]++;
			if (lighting == 3)
			{
				auto& [count, lightmapped, vertex_lit, example] = other_lighting[flags & ~iw7::STATIC_MODEL_FLAG_SCALE_MODIFIER_MASK];
				count++;
				lightmapped += draw.modelLightmapInfo.lightmapIndex >= 0 && draw.modelLightmapInfo.scale[0] != 0.0f;
				vertex_lit += draw.vertexLightingInfo.numLightingValues != 0;
				example = example.empty() && draw.model ? std::string(asset_name(draw.model->name)) : example;
			}
			auto model = export_static_model(source, world, i, probes, floors, grid, output);
			grid_lit += model.contains("lighting_colors");
			if (output.exported_models.contains(model.at("model").get<std::string>()))
			{
				ground_lit += model.at("model_flags").get<int>() != 0;
				model_indices.push_back(models.size());
				models.push_back(std::move(model));
			}
		}

		auto brush_models = json::array();
		for (auto i = 0; i < world.modelCount; i++)
		{
			const auto& model = world.models[i];
			const auto start = std::min<std::size_t>(model.startSurfIndex, world.surfaceCount);
			const auto end = std::min<std::size_t>(start + model.surfaceCount, world.surfaceCount);
			brush_models.push_back(
			{
				{"bounds", bounds_json(model.bounds)},
				{"start", first_surface[start]},
				{"count", first_surface[end] - first_surface[start]},
			});
		}

		// The trigger models draw nothing; the collision export gives them their brushes.
		const auto& triggers = source.map_ents()->trigger;
		for (auto t = 0u; t < triggers.count; t++)
		{
			const auto bounds = trigger_bounds(triggers, t);
			brush_models.push_back({{"bounds", {bounds.mins, bounds.maxs}}, {"start", surfaces.size()}, {"count", 0}});
		}

		// A glass pane draws its rectangle, which Replay's breakable glass replaces.
		for (const auto& pane : glass_panes(source))
		{
			std::vector<geometry_vertex> vertices;
			float center[3]{};
			for (auto k = 0; k < 4; k++)
			{
				geometry_vertex& vertex = vertices.emplace_back();
				for (auto axis = 0; axis < 3; axis++)
				{
					vertex.position[axis] = pane.corners[k][axis];
					vertex.normal[axis] = pane.normal[axis];
					vertex.tangent[axis] = pane.tangent[axis];
				}

				vertex.binormal_sign = 1.0f;
				vertex.uv = {pane.uvs[k][0], pane.uvs[k][1]};
				vertex.color = {255, 255, 255, 255};

				for (auto axis = 0; axis < 3; axis++)
				{
					center[axis] += pane.corners[k][axis] * 0.25f;
				}
			}

			const auto material = output.add_material(source, pane.material);
			output.glass_materials.insert(material);
			brush_models.push_back({{"bounds", {pane.mins, pane.maxs}}, {"start", surfaces.size()}, {"count", 1}});
			surface_indices.push_back(surfaces.size());
			surfaces.push_back({material, -1, static_cast<int>(probes.find(center)), std::move(vertices), {0, 1, 2, 0, 2, 3}});
		}

		// Replay draws one DPVS cell and leaves occlusion to Umbra: one cell with one tree that holds every surface
		// and static model.
		const auto bounds = bounds_json(world.bounds);
		auto probe_indices = json::array();
		for (std::size_t i = 0; i < probes.entries.size(); i++)
		{
			probe_indices.push_back(i);
		}

		json tree =
		{
			{"bounds", bounds},
			{"surfaces", surface_indices},
			{"models", model_indices},
		};

		json cell =
		{
			{"bounds", bounds},
			{"portals", json::array()},
			{"reflection_probes", probe_indices},
			{"surfaces", surface_indices},
			{"models", model_indices},
			{"trees", json::array({std::move(tree)})},
		};

		const auto com_world = source.com_world();
		const auto sun_index = 1u;
		const auto light_count = com_world ? com_world->primaryLightCount : 0u;

		// The broken models of the scriptables that break, which only the map script sets, and the models that the map
		// script sets by name.
		auto script_models = json::array();
		std::set<std::string> listed;
		for (const auto& [targetname, kind] : output.scriptable_destructibles)
		{
			if (!kind.dead_model.empty() && listed.insert(file_name(kind.dead_model)).second)
			{
				script_models.push_back(file_name(kind.dead_model));
			}
		}

		for (const auto& model : output.script_models)
		{
			if (listed.insert(model).second)
			{
				script_models.push_back(model);
			}
		}

		// The debris that flies as physics props (export_effects), with its physics presets.
		auto physics_models = json::array();
		for (const auto& [model, body] : output.physics_models)
		{
			if (output.exported_models.contains(model))
			{
				physics_models.push_back({{"model", model}, {"physics_preset", write_physics_preset(output, model, body)}});
			}
		}

		// The animations of the animated scriptables (export_animations), which the map script plays.
		auto xanims = json::array();
		for (const auto& [name, anim] : output.xanims)
		{
			xanims.push_back(name);
		}

		json value =
		{
			{"schema", 1},
			{"name", bsp_name(source)},
			{"bounds", bounds},
			{"sky", export_sky(source, output)},
			{"fog", export_fog(source)},
			{"grading_clut", export_grading_clut(source, output)},
			{"sun", {{"ambient", {0.5f, 0.5f, 0.5f}}, {"angles", {-45.0f, 0.0f, 0.0f}}, {"color", {1.0f, 1.0f, 1.0f}},
				{"intensity", 1.0f}}},
			{"sun_primary_light_index", sun_index},
			{"primary_light_count", light_count},
			// Blended world surfaces draw after the sky (the roof glass of Frontier shows the sky through it).
			{"translucent_glass", true},
			// IW7 zones are twice the size of IW3 ones: the pipeline block compresses the images that keep their detail.
			{"bc7_images", true},
			// Merge world surfaces only within 512-unit cells, so that GPU culling and the light shadow maps can skip them.
			{"spatial_surfaces", true},
			// The specular maps hold IW7's linear F0 (gamma encoded) and IW7's gloss (derive_specular): the pipeline
			// shades them as IW7 does (MAP_IW7_SPECULAR).
			{"iw7_specular", true},
			{"script_models", script_models},
			{"physics_models", std::move(physics_models)},
			{"xanims", std::move(xanims)},
			{"models", std::move(models)},
			{"brush_models", std::move(brush_models)},
			{"dpvs", {{"planes", json::array()}, {"nodes", json::array({1})},
				{"cells", json::array({std::move(cell)})}}},
			{"lightmaps", std::move(lightmaps.entries)},
			// IW7 lights a texel as A + B * saturate(dot(normal, direction)): the first layer is a constant ambient.
			{"lightmap_encoding", {{"log_scale", lightmap_log_scale}, {"maximum", lightmap_maximum}, {"constant_ambient", true}}},
			// The world draws with Replay's stock world technique sets (MW120R_STOCK_WORLD, AGENTS.md plan 1.4).
			{"replay_stock_world", replay_stock_world()},
			{"light_regions", json::array()},
			{"reflection_probes", std::move(probes.entries)},
		};

		// The surfaces (nearly all of the file) are written by glaze, ahead of the tree's keys.
		write_async([path = output.extracted / (bsp_name(source) + ".replay-world.json"), value = std::move(value),
			surfaces = std::move(surfaces)]
		{
			std::string surfaces_text;
			if (glz::write_json(surfaces, surfaces_text))
			{
				throw std::runtime_error("cannot write the world surfaces");
			}

			std::string text = "{\"surfaces\":" + surfaces_text;
			surfaces_text.clear();
			surfaces_text.shrink_to_fit();
			const auto rest = value.dump();
			if (rest.size() > 2)
			{
				text += ',';
				text.append(rest, 1);
			}
			else
			{
				text += '}';
			}

			write_text(path, text);
		});
		export_sun(source, output);
		ZONETOOL_INFO("iw7: exported %zu of %u world surfaces and %zu of %u static models (%zu lit by the floor under them)",
			surface_indices.size(), world.surfaceCount, model_indices.size(), dpvs.smodelCount, ground_lit);
		ZONETOOL_INFO("iw7: %zu of %u static models take IW7's light grid at their centre", grid_lit, dpvs.smodelCount);
		ZONETOOL_INFO("iw7: IW7 lights its static models by the light grid %zu, vertex lighting %zu, lightmaps %zu, other %zu",
			iw7_lighting[0], iw7_lighting[1], iw7_lighting[2], iw7_lighting[3]);
		for (const auto& [flags, entry] : other_lighting)
		{
			const auto& [count, lightmapped, vertex_lit, example] = entry;
			ZONETOOL_INFO("iw7: other static model lighting: flags %#x x%zu (lightmap data %zu, vertex data %zu), e.g. %s", flags, count,
				lightmapped, vertex_lit, example.data());
		}

		std::string scales;
		for (std::size_t scale = 0; scale < lod_scales.size(); scale++)
		{
			scales += lod_scales[scale] ? std::format("{}{} x{}", scales.empty() ? "" : ", ", scale, lod_scales[scale]) : "";
		}

		ZONETOOL_INFO("iw7: static model LOD scale indices: %s", scales.data());
	}
}
