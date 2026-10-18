#include <std_include.hpp>
#include "export.hpp"

namespace zonetool::convert::iw7_iw8
{
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

		json export_vertex(const iw7::GfxWorldVertex& vertex)
		{
			const auto normal = unpack_unit_vec(vertex.normal);
			const auto tangent = unpack_unit_vec(vertex.tangent);
			return
			{
				{"position", vector_json(vertex.xyz, 3)},
				{"normal", vector_json(normal.data(), 3)},
				{"tangent", vector_json(tangent.data(), 3)},
				{"binormal_sign", vertex.binormalSign < 0.0f ? -1.0f : 1.0f},
				{"uv", vector_json(vertex.texCoord, 2)},
				{"lightmap_uv", vector_json(vertex.lmapCoord, 2)},
				{"color", json::array({vertex.color.array[0], vertex.color.array[1], vertex.color.array[2],
					vertex.color.array[3]})},
			};
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
		std::vector<json> export_surface(const source_map& source, const iw7::GfxWorld& world, const iw7::GfxSurface& surface,
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

			auto vertices = json::array();
			auto indices = json::array();
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
							vertices.back()["color"] = {255, 255, 255, 255};
						}

						for (auto axis = 0; axis < 3; axis++)
						{
							center[axis] += vertex(triangle[corner]).xyz[axis];
						}
					}

					indices.push_back(found->second);
				}
			}

			if (indices.empty())
			{
				return {};
			}

			const auto material_name = output.add_material(source, surface.material);
			for (auto& value : center)
			{
				value /= static_cast<float>(vertices.size());
			}

			const auto lightmap = surface.lightmapIndex < lightmap_count ? static_cast<int>(surface.lightmapIndex) : -1;
			const auto probe = probes.find(center);
			std::vector<json> result;
			result.push_back(
			{
				{"material", material_name},
				{"lightmap", lightmap},
				{"reflection_probe", probe},
				{"vertices", vertices},
				{"indices", indices},
			});

			for (auto layer = 1; layer < layers; layer++)
			{
				// Green, blue and alpha weigh the second to fourth layers.
				const auto weight = [&](const std::uint16_t index)
				{
					return vertex(index).color.array[layer];
				};

				if (std::ranges::none_of(order, [&](const auto index) { return weight(index) != 0; }))
				{
					continue;
				}

				auto overlay = vertices;
				for (std::size_t v = 0; v < overlay.size(); v++)
				{
					overlay[v]["color"] = {255, 255, 255, weight(order[v])};
				}

				result.push_back(
				{
					{"material", output.add_material_layer(source, surface.material, layer)},
					{"lightmap", lightmap},
					{"reflection_probe", probe},
					{"vertices", std::move(overlay)},
					{"indices", indices},
				});
			}

			return result;
		}

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

			// The packed colour of the floor (red, green, blue and sun visibility bytes, as IW3 packs ground
			// lighting).
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

				return floor->color[0] | floor->color[1] << 8 | floor->color[2] << 16 | static_cast<std::uint32_t>(floor->visibility) << 24;
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

		json export_static_model(const source_map& source, const iw7::GfxWorld& world, const unsigned int index,
			const reflection_probes& probes, const floor_light& floors, extraction& output)
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
			return
			{
				{"model", model_name},
				{"origin", vector_json(instance.placement.origin, 3)},
				{"axis", std::move(axis)},
				{"scale", instance.placement.scale},
				{"cull_dist", instance.cullDist ? static_cast<float>(instance.cullDist) : 1000000.0f},
				{"ground_lighting", ground.value_or(0)},
				// IW3's STATIC_MODEL_FLAG_GROUND_LIGHTING.
				{"model_flags", ground ? 1 : 0},
				{"mins", bounds[0]},
				{"maxs", bounds[1]},
				{"primary_light", 0},
				{"reflection_probe", probes.find(instance.placement.origin)},
			};
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

		// Writes an IW3 light grid in the layout of the IW3 extraction: rows of columns, each row a run of points from
		// its lowest floor to over its highest one. A point takes the light of the floor under it in all 56
		// directions; the sun lights the points over floors that see it. IW7 keeps its light grid in a compressed
		// tree, which does not convert yet.
		void export_light_grid(const source_map& source, const floor_light& floors, extraction& output)
		{
			// The grid covers the play space: the spawn points with a margin. The world reaches far beyond it (set
			// dressing in the distance), which would take more points than Replay's light grid holds.
			const auto& spawns = source.map_ents()->spawnList;
			std::array<float, 6> play{FLT_MAX, FLT_MAX, FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX};
			for (auto i = 0u; i < spawns.spawnsCount; i++)
			{
				for (auto axis = 0; axis < 3; axis++)
				{
					play[axis] = std::min(play[axis], spawns.spawns[i].origin[axis]);
					play[axis + 3] = std::max(play[axis + 3], spawns.spawns[i].origin[axis]);
				}
			}

			auto area = floors.floor_area();
			if (spawns.spawnsCount)
			{
				area = {std::max(area[0], play[0] - play_margin), std::max(area[1], play[1] - play_margin),
					std::min(area[2], play[3] + play_margin), std::min(area[3], play[4] + play_margin)};
			}

			const auto lowest = spawns.spawnsCount ? play[2] - 256.0f : -FLT_MAX;
			const auto highest = spawns.spawnsCount ? play[5] + 768.0f : FLT_MAX;
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
					// Blue, green and red bytes, as the model lighting texels take them.
					palette.push_back(key[2]);
					palette.push_back(key[1]);
					palette.push_back(key[0]);
				}

				return created;
			};

			std::vector<std::uint8_t> row_starts;
			std::vector<std::uint8_t> row_data;
			std::vector<std::uint8_t> entries;
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
						for (auto z = bottom; filled && z <= top; z++)
						{
							const auto point = static_cast<float>(z * grid_spacing_z - grid_base);
							const auto floor = std::ranges::find_if(it->rbegin(), it->rend(), [&](const auto& hit) { return hit.height <= point + 32.0f; });
							const auto& hit = floor == it->rend() ? it->front() : *floor;
							append<std::uint16_t>(entries, palette_index(hit.color));
							entries.push_back(static_cast<std::uint8_t>(hit.visibility > 127 ? sun_light : 0));
							entries.push_back(0);
							entry_count++;
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
			write_json(output.extracted / (stem + ".json"),
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
			});

			ZONETOOL_INFO("iw7: light grid of %zu points with %zu colours", entry_count, palette_indices.size());
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

	void collect_static_models(const source_map& source, extraction& output)
	{
		const auto& world = *source.world();
		for (auto i = 0u; i < world.dpvs.smodelCount; i++)
		{
			output.add_model(source, world.dpvs.smodelDrawInsts[i].model);
		}
	}

	void write_json(const std::filesystem::path& path, const json& value)
	{
		std::filesystem::create_directories(path.parent_path());
		std::ofstream stream(path, std::ios::binary | std::ios::trunc);
		stream << value.dump();
		if (!stream)
		{
			throw std::runtime_error(std::format("cannot write {}", path.string()));
		}
	}

	void export_world(const source_map& source, extraction& output)
	{
		const auto& world = *source.world();
		const auto& dpvs = world.dpvs;
		auto lightmaps = export_lightmaps(source, output);
		auto probes = export_reflection_probes(source, output);
		const floor_light floors(source, world, lightmaps);
		export_light_grid(source, floors, output);

		// The index of each surface among the surfaces with an area.
		auto surfaces = json::array();
		auto surface_indices = json::array();
		std::vector<std::size_t> first_surface(world.surfaceCount + 1);
		for (auto i = 0u; i < world.surfaceCount; i++)
		{
			first_surface[i] = surfaces.size();
			for (auto& surface : export_surface(source, world, dpvs.surfaces[i], lightmaps.entries.size(), probes, output))
			{
				surface_indices.push_back(surfaces.size());
				surfaces.push_back(std::move(surface));
			}
		}

		first_surface[world.surfaceCount] = surfaces.size();

		// The static models whose model exported (export_models runs first).
		auto models = json::array();
		auto model_indices = json::array();
		std::size_t ground_lit{};
		for (auto i = 0u; i < dpvs.smodelCount; i++)
		{
			auto model = export_static_model(source, world, i, probes, floors, output);
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
			auto vertices = json::array();
			float center[3]{};
			for (auto k = 0; k < 4; k++)
			{
				vertices.push_back(
				{
					{"position", pane.corners[k]},
					{"normal", pane.normal},
					{"tangent", pane.tangent},
					{"binormal_sign", 1.0f},
					{"uv", pane.uvs[k]},
					{"lightmap_uv", {0.0f, 0.0f}},
					{"color", {255, 255, 255, 255}},
				});

				for (auto axis = 0; axis < 3; axis++)
				{
					center[axis] += pane.corners[k][axis] * 0.25f;
				}
			}

			const auto material = output.add_material(source, pane.material);
			output.glass_materials.insert(material);
			brush_models.push_back({{"bounds", {pane.mins, pane.maxs}}, {"start", surfaces.size()}, {"count", 1}});
			surface_indices.push_back(surfaces.size());
			surfaces.push_back(
			{
				{"material", material},
				{"lightmap", -1},
				{"reflection_probe", probes.find(center)},
				{"vertices", std::move(vertices)},
				{"indices", {0, 1, 2, 0, 2, 3}},
			});
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

		json value =
		{
			{"schema", 1},
			{"name", bsp_name(source)},
			{"bounds", bounds},
			{"sky", export_sky(source, output)},
			{"sun", {{"ambient", {0.5f, 0.5f, 0.5f}}, {"angles", {-45.0f, 0.0f, 0.0f}}, {"color", {1.0f, 1.0f, 1.0f}},
				{"intensity", 1.0f}}},
			{"sun_primary_light_index", sun_index},
			{"primary_light_count", light_count},
			{"surfaces", std::move(surfaces)},
			{"models", std::move(models)},
			{"brush_models", std::move(brush_models)},
			{"dpvs", {{"planes", json::array()}, {"nodes", json::array({1})},
				{"cells", json::array({std::move(cell)})}}},
			{"lightmaps", std::move(lightmaps.entries)},
			{"light_regions", json::array()},
			{"reflection_probes", std::move(probes.entries)},
		};

		write_json(output.extracted / (bsp_name(source) + ".replay-world.json"), value);
		ZONETOOL_INFO("iw7: exported %zu of %u world surfaces and %zu of %u static models (%zu lit by the floor under them)",
			surface_indices.size(), world.surfaceCount, model_indices.size(), dpvs.smodelCount, ground_lit);
	}
}
