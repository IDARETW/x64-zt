#include <std_include.hpp>
#include "export.hpp"

namespace zonetool::convert::iw7_iw8
{
	namespace
	{
		// IW3 CONTENTS_SOLID, which the pipeline converts to the Replay contents.
		constexpr std::uint32_t contents_solid = 1;

		json vector_json(const float* values, const std::size_t count)
		{
			auto result = json::array();
			for (std::size_t i = 0; i < count; i++)
			{
				result.push_back(values[i]);
			}

			return result;
		}

		json submodel_json(const iw7::Bounds& bounds)
		{
			float mins[3]{};
			float maxs[3]{};
			for (auto i = 0; i < 3; i++)
			{
				mins[i] = bounds.midPoint[i] - bounds.halfSize[i];
				maxs[i] = bounds.midPoint[i] + bounds.halfSize[i];
			}

			return {{"leaf", 0}, {"mins", vector_json(mins, 3)}, {"maxs", vector_json(maxs, 3)}};
		}

		// The contents of IW3 trigger brushes: solid and detail. Only the trigger entity uses its brush model.
		constexpr std::uint32_t contents_trigger = 0x8000001;

		// An IW3 brush: its bounds, and planes (normal, distance; the inside is below the distance) that cut them.
		json brush_json(const box& bounds, json planes)
		{
			auto sides = json::array();
			for (std::size_t i = 0; i < planes.size(); i++)
			{
				sides.push_back(-1);
			}

			return
			{
				{"axial_materials", {{-1, -1, -1}, {-1, -1, -1}}},
				{"contents", contents_trigger},
				{"ladder_planes", json::array()},
				{"maxs", bounds.maxs},
				{"mins", bounds.mins},
				{"planes", std::move(planes)},
				{"side_materials", std::move(sides)},
			};
		}
	}

	unsigned int trigger_brush_model(const source_map& source, const unsigned int trigger)
	{
		return static_cast<unsigned int>(source.world()->modelCount) + trigger;
	}

	box trigger_bounds(const iw7::MapTriggers& triggers, const unsigned int model)
	{
		box result{{FLT_MAX, FLT_MAX, FLT_MAX}, {-FLT_MAX, -FLT_MAX, -FLT_MAX}};
		const auto& trigger = triggers.models[model];
		for (auto h = trigger.firstHull; h < trigger.firstHull + trigger.hullCount && h < triggers.hullCount; h++)
		{
			const auto& bounds = triggers.hulls[h].bounds;
			for (auto axis = 0; axis < 3; axis++)
			{
				result.mins[axis] = std::min(result.mins[axis], bounds.midPoint[axis] - bounds.halfSize[axis] - 1.0f);
				result.maxs[axis] = std::max(result.maxs[axis], bounds.midPoint[axis] + bounds.halfSize[axis] + 1.0f);
			}
		}

		// A trigger model without hulls keeps a unit box, so that its entity keeps a volume.
		return result.mins[0] <= result.maxs[0] ? result : box{{-1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}};
	}

	void export_collision(const source_map& source, extraction& output)
	{
		const auto& world = *source.world();

		// Each transient zone holds one vertex array that its surfaces index from their first vertex.
		auto vertices = json::array();
		std::vector<std::size_t> zone_bases(world.draw.transientZoneCount);
		for (auto z = 0u; z < world.draw.transientZoneCount; z++)
		{
			zone_bases[z] = vertices.size();
			const auto zone = source.resolve(iw7::ASSET_TYPE_GFXWORLD_TRANSIENT_ZONE, world.draw.transientZones[z]);
			for (auto v = 0u; zone && zone->vd.vertices && v < zone->vertexCount; v++)
			{
				vertices.push_back(vector_json(zone->vd.vertices[v].xyz, 3));
			}
		}

		// The world model draws the first surfaces; its geometry is the collision mesh.
		auto triangles = json::array();
		auto contents = json::array();
		const auto world_surfaces = world.modelCount ? world.models[0].surfaceCount : world.surfaceCount;
		for (auto i = 0u; i < world_surfaces && i < world.surfaceCount; i++)
		{
			const auto& surface = world.dpvs.surfaces[i];
			const auto& tris = surface.tris;
			if (!classify_material(source.resolve(iw7::ASSET_TYPE_MATERIAL, surface.material)).solid())
			{
				continue;
			}

			if (surface.transientZone >= world.draw.transientZoneCount)
			{
				continue;
			}

			const auto first = zone_bases[surface.transientZone] + tris.firstVertex;
			for (auto t = 0u; t < tris.triCount; t++)
			{
				const auto index = world.draw.indices + tris.baseIndex + t * 3;
				triangles.push_back(json::array({first + index[0], first + index[1], first + index[2]}));
				contents.push_back(contents_solid);
			}
		}

		auto submodels = json::array();
		for (auto i = 0; i < world.modelCount; i++)
		{
			submodels.push_back(submodel_json(world.models[i].bounds));
		}

		// Each hull of a trigger model is a brush: its bounds cut by its slabs (a direction, and the midpoint and half
		// size of the range of the volume along it).
		auto brushes = json::array();
		auto nodes = json::array({{{"brushes", json::array()}, {"children", {0, 0}}, {"count", 0}}});
		const auto& triggers = source.map_ents()->trigger;
		for (auto t = 0u; t < triggers.count; t++)
		{
			const auto& trigger = triggers.models[t];
			auto leaf = json::array();
			for (auto h = trigger.firstHull; h < trigger.firstHull + trigger.hullCount && h < triggers.hullCount; h++)
			{
				const auto& hull = triggers.hulls[h];
				box bounds{};
				for (auto axis = 0; axis < 3; axis++)
				{
					bounds.mins[axis] = hull.bounds.midPoint[axis] - hull.bounds.halfSize[axis];
					bounds.maxs[axis] = hull.bounds.midPoint[axis] + hull.bounds.halfSize[axis];
				}

				auto planes = json::array();
				for (auto s = hull.firstSlab; s < hull.firstSlab + hull.slabCount && s < triggers.slabCount; s++)
				{
					const auto& slab = triggers.slabs[s];
					planes.push_back({slab.dir[0], slab.dir[1], slab.dir[2], slab.midPoint + slab.halfSize});
					planes.push_back({-slab.dir[0], -slab.dir[1], -slab.dir[2], slab.halfSize - slab.midPoint});
				}

				leaf.push_back(brushes.size());
				brushes.push_back(brush_json(bounds, std::move(planes)));
			}

			const auto bounds = trigger_bounds(triggers, t);
			if (leaf.empty())
			{
				leaf.push_back(brushes.size());
				brushes.push_back(brush_json({{-0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, 0.5f}}, json::array()));
			}

			submodels.push_back({{"leaf", nodes.size()}, {"mins", bounds.mins}, {"maxs", bounds.maxs}});
			const auto count = leaf.size();
			nodes.push_back({{"brushes", std::move(leaf)}, {"count", count}});
		}

		// The glass panes: the pipeline gives each the hull of its breakable glass.
		const auto panes = glass_panes(source);
		for (const auto& pane : panes)
		{
			submodels.push_back({{"leaf", 0}, {"mins", pane.mins}, {"maxs", pane.maxs}});
		}

		auto static_models = json::array();
		for (auto i = 0u; i < world.dpvs.smodelCount; i++)
		{
			const auto& instance = world.dpvs.smodelDrawInsts[i];
			const auto name = output.add_model(source, instance.model);
			if (output.exported_models.contains(name))
			{
				static_models.push_back({{"model", name}, {"origin", vector_json(instance.placement.origin, 3)}});
			}
		}

		const auto triangle_count = triangles.size();
		write_json(output.extracted / (bsp_name(source) + ".replay-collision.json"),
		{
			{"schema", 1},
			{"name", bsp_name(source)},
			{"brushes", std::move(brushes)},
			{"leaf_brush_nodes", std::move(nodes)},
			{"materials", json::array()},
			{"submodel_count", submodels.size()},
			{"submodels", std::move(submodels)},
			{"static_models", std::move(static_models)},
			{"dynamic_model_count", 0},
			{"dynamic_models", json::array()},
			{"dynamic_brush_count", 0},
			{"dynamic_brushes", json::array()},
			{"vertices", std::move(vertices)},
			{"triangles", std::move(triangles)},
			{"triangle_contents_schema", "material-partitions-v1"},
			{"triangle_contents", std::move(contents)},
		});

		ZONETOOL_INFO("iw7: exported %zu collision triangles from the world surfaces and %u trigger models", triangle_count,
			triggers.count);
	}
}
