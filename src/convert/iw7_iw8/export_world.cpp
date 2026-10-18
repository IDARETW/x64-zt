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

		// The surface, or nothing when none of its triangles has an area.
		std::optional<json> export_surface(const source_map& source, const iw7::GfxWorld& world, const iw7::GfxSurface& surface,
			const std::size_t lightmap_count, const reflection_probes& probes, extraction& output)
		{
			const auto& tris = surface.tris;
			if (surface.transientZone >= world.draw.transientZoneCount)
			{
				throw std::runtime_error(std::format("surface transient zone {} is out of range", surface.transientZone));
			}

			// Replay maps have no refraction layers.
			if (classify_material(source.resolve(iw7::ASSET_TYPE_MATERIAL, surface.material)).kind == surface_kind::distortion)
			{
				return {};
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

			auto vertices = json::array();
			auto indices = json::array();
			std::unordered_map<std::uint16_t, std::size_t> remap;
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

			return json
			{
				{"material", material_name},
				{"lightmap", surface.lightmapIndex < lightmap_count ? static_cast<int>(surface.lightmapIndex) : -1},
				{"reflection_probe", probes.find(center)},
				{"vertices", std::move(vertices)},
				{"indices", std::move(indices)},
			};
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
						if (valid)
						{
							this->add(std::move(corners));
						}
					}
				}
			}

			// The packed colour of the floor (red, green, blue and sun visibility bytes, as IW3 packs ground
			// lighting) for the volume texels, which the pipeline doubles.
			std::optional<std::uint32_t> ground_lighting(const float* point) const
			{
				const auto cell = this->cell_of(point[0], point[1]);
				if (!cell)
				{
					return {};
				}

				const triangle* floor{};
				std::array<float, 3> weights{};
				auto height = -FLT_MAX;
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
						((p[1][0] - point[0]) * (p[2][1] - point[1]) - (p[2][0] - point[0]) * (p[1][1] - point[1])) / area,
						((p[2][0] - point[0]) * (p[0][1] - point[1]) - (p[0][0] - point[0]) * (p[2][1] - point[1])) / area,
						((p[0][0] - point[0]) * (p[1][1] - point[1]) - (p[1][0] - point[0]) * (p[0][1] - point[1])) / area,
					};

					if (w[0] < 0.0f || w[1] < 0.0f || w[2] < 0.0f)
					{
						continue;
					}

					const auto z = w[0] * p[0][2] + w[1] * p[1][2] + w[2] * p[2][2];
					if (z <= point[2] + 1.0f && z >= point[2] - max_drop && z > height)
					{
						floor = &candidate;
						weights = w;
						height = z;
					}
				}

				if (!floor)
				{
					return {};
				}

				const auto& page = this->lightmaps_.pages[floor->page];
				float u{};
				float v{};
				for (auto k = 0; k < 3; k++)
				{
					u += weights[k] * floor->uvs[k][0];
					v += weights[k] * floor->uvs[k][1];
				}

				const auto x = std::min(static_cast<std::size_t>(std::clamp(u, 0.0f, 1.0f) * static_cast<float>(page.width)), page.width - 1);
				const auto y = std::min(static_cast<std::size_t>(std::clamp(v, 0.0f, 1.0f) * static_cast<float>(page.height)), page.height - 1);
				const auto texel = y * page.width + x;
				auto color = static_cast<std::uint32_t>(page.visibility[texel]) << 24;
				for (auto channel = 0; channel < 3; channel++)
				{
					const auto level = std::clamp(light_level(page.ambient[texel][channel]) * 0.5f, 0.0f, 1.0f);
					color |= static_cast<std::uint32_t>(std::lround(level * 255.0f)) << (8 * channel);
				}

				return color;
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
			std::array<float, 2> origin_{};
			std::size_t columns_{};
			std::size_t rows_{};
			std::vector<triangle> triangles_;
			std::vector<std::vector<unsigned int>> cells_;

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

		// The index of each surface among the surfaces with an area.
		auto surfaces = json::array();
		auto surface_indices = json::array();
		std::vector<std::size_t> first_surface(world.surfaceCount + 1);
		for (auto i = 0u; i < world.surfaceCount; i++)
		{
			first_surface[i] = surfaces.size();
			if (auto surface = export_surface(source, world, dpvs.surfaces[i], lightmaps.entries.size(), probes, output))
			{
				surface_indices.push_back(surfaces.size());
				surfaces.push_back(std::move(*surface));
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
