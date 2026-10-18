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
			extraction& output)
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
					}

					indices.push_back(found->second);
				}
			}

			if (indices.empty())
			{
				return {};
			}

			const auto material_name = output.add_material(source, surface.material);

			return json
			{
				{"material", material_name},
				// The IW7 lightmaps do not convert yet.
				{"lightmap", -1},
				{"reflection_probe", 0},
				{"vertices", std::move(vertices)},
				{"indices", std::move(indices)},
			};
		}

		json export_static_model(const source_map& source, const iw7::GfxWorld& world, const unsigned int index,
			extraction& output)
		{
			const auto& instance = world.dpvs.smodelDrawInsts[index];
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
				{"ground_lighting", 0},
				{"model_flags", 0},
				{"mins", bounds[0]},
				{"maxs", bounds[1]},
				{"primary_light", 0},
				{"reflection_probe", 0},
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

		// The index of each surface among the surfaces with an area.
		auto surfaces = json::array();
		auto surface_indices = json::array();
		std::vector<std::size_t> first_surface(world.surfaceCount + 1);
		for (auto i = 0u; i < world.surfaceCount; i++)
		{
			first_surface[i] = surfaces.size();
			if (auto surface = export_surface(source, world, dpvs.surfaces[i], output))
			{
				surface_indices.push_back(surfaces.size());
				surfaces.push_back(std::move(*surface));
			}
		}

		first_surface[world.surfaceCount] = surfaces.size();

		// The static models whose model exported (export_models runs first).
		auto models = json::array();
		auto model_indices = json::array();
		for (auto i = 0u; i < dpvs.smodelCount; i++)
		{
			auto model = export_static_model(source, world, i, output);
			if (output.exported_models.contains(model.at("model").get<std::string>()))
			{
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
			{"reflection_probes", json::array({0})},
			{"surfaces", surface_indices},
			{"models", model_indices},
			{"trees", json::array({std::move(tree)})},
		};

		const auto com_world = source.com_world();
		const auto sun_index = 1u;
		const auto light_count = com_world ? std::min(com_world->primaryLightCount, primary_light_count) : 0u;

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
			{"lightmaps", json::array()},
			{"light_regions", json::array()},
			{"reflection_probes", json::array({{{"image", export_default_probe(output)}, {"origin", {0.0f, 0.0f, 0.0f}}}})},
		};

		write_json(output.extracted / (bsp_name(source) + ".replay-world.json"), value);
		ZONETOOL_INFO("iw7: exported %zu of %u world surfaces and %zu of %u static models", surface_indices.size(), world.surfaceCount,
			model_indices.size(), dpvs.smodelCount);
	}
}
