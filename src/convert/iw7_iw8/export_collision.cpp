#include <std_include.hpp>
#include "export.hpp"

namespace zonetool::convert::iw7_iw8
{
	namespace
	{
		using vec3 = std::array<float, 3>;

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

		box bounds_of(const iw7::Bounds& bounds)
		{
			box result{};
			for (auto i = 0; i < 3; i++)
			{
				result.mins[i] = bounds.midPoint[i] - bounds.halfSize[i];
				result.maxs[i] = bounds.midPoint[i] + bounds.halfSize[i];
			}

			return result;
		}

		json submodel_json(const box& bounds, const std::size_t leaf = 0)
		{
			return {{"leaf", leaf}, {"mins", bounds.mins}, {"maxs", bounds.maxs}};
		}

		// The contents of IW3 trigger brushes: solid and detail. Only the trigger entity uses its brush model.
		constexpr std::uint32_t contents_trigger = 0x8000001;

		// An IW3 brush: its bounds, and planes (normal, distance; the inside is below the distance) that cut them.
		json brush_json(const box& bounds, json planes, const std::uint32_t contents = contents_trigger)
		{
			auto sides = json::array();
			for (std::size_t i = 0; i < planes.size(); i++)
			{
				sides.push_back(-1);
			}

			return
			{
				{"axial_materials", {{-1, -1, -1}, {-1, -1, -1}}},
				{"contents", contents},
				{"ladder_planes", json::array()},
				{"maxs", bounds.maxs},
				{"mins", bounds.mins},
				{"planes", std::move(planes)},
				{"side_materials", std::move(sides)},
			};
		}

		using dvec3 = std::array<double, 3>;

		dvec3 subtract(const dvec3& a, const dvec3& b)
		{
			return {a[0] - b[0], a[1] - b[1], a[2] - b[2]};
		}

		dvec3 cross(const dvec3& a, const dvec3& b)
		{
			return {a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]};
		}

		double dot(const dvec3& a, const dvec3& b)
		{
			return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
		}

		// The faces of the convex hull of the points that are not axial (the bounds of a brush give those), as planes
		// that hold every point, or nothing for points that enclose no volume. The points of an IW7 piece lie on the grid
		// of its mesh, so a face of its brush can be a few triangles that tilt by the rounding to the grid: faces whose
		// planes differ by less than a hundredth of a unit over the piece merge.
		std::optional<std::vector<std::array<float, 4>>> hull_planes(const std::vector<vec3>& source)
		{
			// Relative to their centre, so that the planes keep the precision of the points.
			dvec3 centre{};
			for (const auto& point : source)
			{
				for (std::size_t axis = 0; axis < 3; axis++)
				{
					centre[axis] += point[axis] / static_cast<double>(source.size());
				}
			}

			std::vector<dvec3> points;
			double size{};
			for (const auto& point : source)
			{
				points.push_back(subtract({point[0], point[1], point[2]}, centre));
				size = std::max(size, std::sqrt(dot(points.back(), points.back())));
			}

			constexpr auto flat_thickness = 0.01;
			const auto exact = std::max(size, 1.0) * 1e-9;
			struct face
			{
				dvec3 normal;
				double distance;
			};

			std::vector<face> faces;
			bool solid = false;
			for (std::size_t i = 0; i < points.size(); i++)
			{
				for (std::size_t j = i + 1; j < points.size(); j++)
				{
					for (std::size_t k = j + 1; k < points.size(); k++)
					{
						auto normal = cross(subtract(points[j], points[i]), subtract(points[k], points[i]));
						const auto length = std::sqrt(dot(normal, normal));
						if (length < 1e-12)
						{
							continue;
						}

						for (auto& value : normal)
						{
							value /= length;
						}

						auto low = DBL_MAX;
						auto high = -DBL_MAX;
						for (const auto& point : points)
						{
							low = std::min(low, dot(normal, point));
							high = std::max(high, dot(normal, point));
						}

						solid |= high - low > flat_thickness;
						// A face: every point lies on one side of the plane of the three.
						const auto here = dot(normal, points[i]);
						if (here - low <= exact)
						{
							normal = {-normal[0], -normal[1], -normal[2]};
							high = -low;
						}
						else if (high - here > exact)
						{
							continue;
						}

						const auto merged = std::ranges::find_if(faces, [&](const face& other)
						{
							const auto angle = std::acos(std::clamp(dot(normal, other.normal), -1.0, 1.0));
							return angle * 2.0 * size < flat_thickness;
						});

						if (merged == faces.end())
						{
							faces.push_back({normal, high});
						}
					}
				}
			}

			if (!solid)
			{
				return std::nullopt;
			}

			std::vector<std::array<float, 4>> planes;
			for (const auto& [normal, distance] : faces)
			{
				// A merged face holds every point: its distance is the largest along its normal.
				auto farthest = -DBL_MAX;
				for (const auto& point : points)
				{
					farthest = std::max(farthest, dot(normal, point));
				}

				auto components = 0;
				for (const auto value : normal)
				{
					components += std::abs(value) > 1e-6;
				}

				if (components > 1)
				{
					planes.push_back({static_cast<float>(normal[0]), static_cast<float>(normal[1]), static_cast<float>(normal[2]),
						static_cast<float>(farthest + dot(normal, centre))});
				}
			}

			return planes;
		}

		// IW7 surface types are Replay's up to metal_thin (43) and from water_knee (50) to user_terrain_9 (61). The robot
		// types (44-49) take metal; the last IW7 user terrain and the reserved type take the floor's (0).
		std::uint32_t replay_surface_type(const std::uint32_t type)
		{
			if (type >= 44 && type <= 49)
			{
				return 13;
			}

			return type <= 61 ? type : 0;
		}

		// The collision of the extraction: brushes, the leaf nodes that give brush models their brushes, and the world
		// triangles with shared vertices.
		struct collision_writer
		{
			json brushes = json::array();
			json nodes = json::array({{{"brushes", json::array()}, {"children", {0, 0}}, {"count", 0}}});
			json vertices = json::array();
			json triangles = json::array();
			json contents = json::array();
			json surfaces = json::array();
			std::map<std::array<std::uint32_t, 3>, std::size_t> vertex_indices;
			std::size_t flat{};

			// Adds a brush of a convex piece and returns its index.
			std::optional<std::size_t> add_brush(const havok_convex& convex, const havok_tag& tag)
			{
				const auto planes = hull_planes(convex.points);
				if (!planes || convex.points.size() < 4)
				{
					this->flat++;
					return std::nullopt;
				}

				box bounds{{FLT_MAX, FLT_MAX, FLT_MAX}, {-FLT_MAX, -FLT_MAX, -FLT_MAX}};
				for (const auto& point : convex.points)
				{
					for (std::size_t axis = 0; axis < 3; axis++)
					{
						bounds.mins[axis] = std::min(bounds.mins[axis], point[axis]);
						bounds.maxs[axis] = std::max(bounds.maxs[axis], point[axis]);
					}
				}

				auto brush = brush_json(bounds, *planes, tag.contents);
				brush["surface_type"] = replay_surface_type(tag.surface_type);
				brush["surface_flags"] = tag.surface_flags;
				this->brushes.push_back(std::move(brush));
				return this->brushes.size() - 1;
			}

			std::size_t vertex(const vec3& point)
			{
				const auto key = std::bit_cast<std::array<std::uint32_t, 3>>(point);
				const auto [found, added] = this->vertex_indices.try_emplace(key, this->vertices.size());
				if (added)
				{
					this->vertices.push_back(point);
				}

				return found->second;
			}

			// The pipeline reverses the winding of IW3 triangles (clockwise seen from the front) to Havok's; IW7 triangles
			// already wind as Havok does.
			void add_triangle(const havok_triangle& triangle, const havok_tag& tag)
			{
				this->triangles.push_back(json::array({this->vertex(triangle.corners[0]), this->vertex(triangle.corners[2]),
					this->vertex(triangle.corners[1])}));
				this->contents.push_back(tag.contents);
				this->surfaces.push_back(json::array({replay_surface_type(tag.surface_type), tag.surface_flags}));
			}

			// A leaf node of brushes for a brush model.
			std::size_t add_leaf(json leaf)
			{
				const auto count = leaf.size();
				this->nodes.push_back({{"brushes", std::move(leaf)}, {"count", count}});
				return this->nodes.size() - 1;
			}
		};

		// The world's collision from its surfaces: the render geometry of the solid materials of the world model.
		void add_render_collision(const source_map& source, collision_writer& writer)
		{
			const auto& world = *source.world();
			std::vector<std::size_t> zone_bases(world.draw.transientZoneCount);
			for (auto z = 0u; z < world.draw.transientZoneCount; z++)
			{
				zone_bases[z] = writer.vertices.size();
				const auto zone = source.resolve(iw7::ASSET_TYPE_GFXWORLD_TRANSIENT_ZONE, world.draw.transientZones[z]);
				for (auto v = 0u; zone && zone->vd.vertices && v < zone->vertexCount; v++)
				{
					writer.vertices.push_back(vector_json(zone->vd.vertices[v].xyz, 3));
				}
			}

			// The world model draws the first surfaces.
			const auto world_surfaces = world.modelCount ? world.models[0].surfaceCount : world.surfaceCount;
			for (auto i = 0u; i < world_surfaces && i < world.surfaceCount; i++)
			{
				const auto& surface = world.dpvs.surfaces[i];
				const auto& tris = surface.tris;
				if (!classify_material(source.resolve(iw7::ASSET_TYPE_MATERIAL, surface.material)).solid() ||
					surface.transientZone >= world.draw.transientZoneCount)
				{
					continue;
				}

				const auto first = zone_bases[surface.transientZone] + tris.firstVertex;
				for (auto t = 0u; t < tris.triCount; t++)
				{
					const auto index = world.draw.indices + tris.baseIndex + t * 3;
					writer.triangles.push_back(json::array({first + index[0], first + index[1], first + index[2]}));
					writer.contents.push_back(contents_solid);
					writer.surfaces.push_back(json::array({0, 0}));
				}
			}

			ZONETOOL_INFO("iw7: exported %zu collision triangles from the world surfaces", writer.triangles.size());
		}

		// IW7's unbreakable glass (Frontier's patterned roof, glass_clear_unbreakable_*) has no bullet collision of its own:
		// only the player clips over it, so bullets passed through it (owner test of 58aac89). Its render triangles become
		// collision that stops shots and nothing else (clipshot and missileclip, as IW7's other shot clips), of Replay's
		// glass_solid surface (39: bulletproof, glass impacts).
		void add_unbreakable_glass_collision(const source_map& source, collision_writer& writer)
		{
			constexpr std::uint32_t contents_shot_clip = 0x2080;
			constexpr std::uint32_t surface_glass_solid = 39;
			const auto& world = *source.world();
			const auto world_surfaces = world.modelCount ? world.models[0].surfaceCount : world.surfaceCount;
			std::size_t added{};
			for (auto i = 0u; i < world_surfaces && i < world.surfaceCount; i++)
			{
				const auto& surface = world.dpvs.surfaces[i];
				const auto* material = source.resolve(iw7::ASSET_TYPE_MATERIAL, surface.material);
				const std::string_view name = material ? asset_name(material->name) : std::string_view{};
				if (name.find("glass") == std::string_view::npos || name.find("unbreakable") == std::string_view::npos ||
					surface.transientZone >= world.draw.transientZoneCount)
				{
					continue;
				}

				const auto zone = source.resolve(iw7::ASSET_TYPE_GFXWORLD_TRANSIENT_ZONE, world.draw.transientZones[surface.transientZone]);
				if (!zone || !zone->vd.vertices)
				{
					continue;
				}

				const auto& tris = surface.tris;
				for (auto t = 0u; t < tris.triCount; t++)
				{
					const auto* index = world.draw.indices + tris.baseIndex + t * 3;
					std::array<std::size_t, 3> corners{};
					auto valid = true;
					for (auto c = 0; c < 3; c++)
					{
						const auto v = tris.firstVertex + index[c];
						if (v >= zone->vertexCount)
						{
							valid = false;
							break;
						}

						const auto& xyz = zone->vd.vertices[v].xyz;
						corners[c] = writer.vertex({xyz[0], xyz[1], xyz[2]});
					}

					if (!valid)
					{
						continue;
					}

					// Render triangles wind as the extraction's IW3 triangles (the pipeline reverses them).
					writer.triangles.push_back(json::array({corners[0], corners[1], corners[2]}));
					writer.contents.push_back(contents_shot_clip);
					writer.surfaces.push_back(json::array({surface_glass_solid, 0}));
					added++;
				}
			}

			if (added)
			{
				ZONETOOL_INFO("iw7: added %zu shot-clip triangles of unbreakable glass", added);
			}
		}

		std::optional<havok_shapes> read_shapes(const char* data, const unsigned int size, const char* owner)
		{
			if (!data || !size)
			{
				return std::nullopt;
			}

			try
			{
				auto shapes = read_havok_shapes(data, size);
				if (shapes.misplaced)
				{
					ZONETOOL_WARNING("iw7: %zu points of the Havok collision of the %s lie outside their mesh sections", shapes.misplaced,
						owner);
				}

				return shapes;
			}
			catch (const std::exception& e)
			{
				ZONETOOL_WARNING("iw7: the Havok collision of the %s cannot be read: %s", owner, e.what());
				return std::nullopt;
			}
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

	// IW7 keeps the collision of the world and of the brush models in Havok data: the clip map's shape is the world's
	// (convex pieces for the brushes, triangles for the rest), and the cmodel of each brush model names its shape in the
	// shape list of the map entities (physicsShapeOverrideIdx). The convex pieces become IW3 brushes and the triangles the
	// world's collision triangles, with the contents and the Replay surface types and flags of their shape tags. The pieces
	// of breakable glass go to the brush models of the glass panes, whose hulls the pipeline makes native glass.
	void export_collision(const source_map& source, extraction& output)
	{
		const auto& world = *source.world();
		const auto* clip_map = source.clip_map();
		const auto* map_ents = source.map_ents();
		collision_writer writer;

		std::size_t glass_pieces{};
		std::size_t untagged{};
		const auto tag_of = [&](const havok_shapes& shapes, const std::uint16_t tag) -> const havok_tag*
		{
			if (tag >= shapes.tags.size())
			{
				untagged++;
				return nullptr;
			}

			if (shapes.tags[tag].glass_id)
			{
				glass_pieces++;
				return nullptr;
			}

			return &shapes.tags[tag];
		};

		const auto world_shapes = clip_map ? read_shapes(clip_map->havokWorldShapeData, clip_map->havokWorldShapeDataSize, "world") :
			std::nullopt;
		// The collision of the glass pieces, which goes to the brush models of their panes.
		std::vector<const havok_convex*> glass;
		if (world_shapes && !world_shapes->shapes.empty())
		{
			std::size_t world_brushes{};
			for (const auto& shape : world_shapes->shapes)
			{
				for (const auto& convex : shape.convexes)
				{
					if (convex.tag < world_shapes->tags.size() && world_shapes->tags[convex.tag].glass_id)
					{
						glass.push_back(&convex);
						continue;
					}

					const auto* tag = tag_of(*world_shapes, convex.tag);
					world_brushes += tag && writer.add_brush(convex, *tag);
				}

				for (const auto& triangle : shape.triangles)
				{
					if (const auto* tag = tag_of(*world_shapes, triangle.tag))
					{
						writer.add_triangle(triangle, *tag);
					}
				}
			}

			ZONETOOL_INFO("iw7: exported the Havok collision of the world: %zu brushes and %zu triangles (%zu pieces of a form that "
				"does not convert)", world_brushes, writer.triangles.size(), world_shapes->unknown);
			add_unbreakable_glass_collision(source, writer);
		}
		else
		{
			add_render_collision(source, writer);
		}

		// The brush models, in the space of their entities.
		auto submodels = json::array();
		submodels.push_back(submodel_json(bounds_of(world.models[0].bounds)));
		const auto ents_shapes = map_ents ? read_shapes(map_ents->havokEntsShapeData, map_ents->havokEntsShapeDataSize, "map entities") :
			std::nullopt;
		std::size_t solid_models{};
		std::size_t model_triangles{};
		for (auto i = 1; i < world.modelCount; i++)
		{
			const auto* cmodel = map_ents && static_cast<unsigned int>(i) < map_ents->numSubModels ? &map_ents->cmodels[i] : nullptr;
			const auto bounds = bounds_of(cmodel ? cmodel->bounds : world.models[i].bounds);
			if (!cmodel || !ents_shapes || cmodel->physicsShapeOverrideIdx >= ents_shapes->shapes.size())
			{
				submodels.push_back(submodel_json(bounds));
				continue;
			}

			const auto& shape = ents_shapes->shapes[cmodel->physicsShapeOverrideIdx];
			auto leaf = json::array();
			for (const auto& convex : shape.convexes)
			{
				if (const auto* tag = tag_of(*ents_shapes, convex.tag))
				{
					if (const auto brush = writer.add_brush(convex, *tag))
					{
						leaf.push_back(*brush);
					}
				}
			}

			// IW3 brush models collide through their brushes alone: each triangle of the model's mesh becomes a brush, the
			// triangle and a copy of it a unit behind its front.
			for (const auto& triangle : shape.triangles)
			{
				const auto* tag = tag_of(*ents_shapes, triangle.tag);
				const auto& [a, b, c] = triangle.corners;
				const std::array<float, 3> u{b[0] - a[0], b[1] - a[1], b[2] - a[2]};
				const std::array<float, 3> v{c[0] - a[0], c[1] - a[1], c[2] - a[2]};
				std::array<float, 3> normal{u[1] * v[2] - u[2] * v[1], u[2] * v[0] - u[0] * v[2], u[0] * v[1] - u[1] * v[0]};
				const auto length = std::sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
				if (!tag || !(length > 1e-6f))
				{
					model_triangles++;
					continue;
				}

				havok_convex prism{{}, triangle.tag};
				for (const auto& corner : triangle.corners)
				{
					prism.points.push_back(corner);
					prism.points.push_back({corner[0] - normal[0] / length, corner[1] - normal[1] / length, corner[2] - normal[2] / length});
				}

				if (const auto brush = writer.add_brush(prism, *tag))
				{
					leaf.push_back(*brush);
				}
				else
				{
					model_triangles++;
				}
			}

			solid_models += !leaf.empty();
			submodels.push_back(submodel_json(bounds, leaf.empty() ? 0 : writer.add_leaf(std::move(leaf))));
		}

		// Each hull of a trigger model is a brush: its bounds cut by its slabs (a direction, and the midpoint and half
		// size of the range of the volume along it).
		const auto& triggers = map_ents->trigger;
		for (auto t = 0u; t < triggers.count; t++)
		{
			const auto& trigger = triggers.models[t];
			auto leaf = json::array();
			for (auto h = trigger.firstHull; h < trigger.firstHull + trigger.hullCount && h < triggers.hullCount; h++)
			{
				const auto& hull = triggers.hulls[h];
				auto planes = json::array();
				for (auto s = hull.firstSlab; s < hull.firstSlab + hull.slabCount && s < triggers.slabCount; s++)
				{
					const auto& slab = triggers.slabs[s];
					planes.push_back({slab.dir[0], slab.dir[1], slab.dir[2], slab.midPoint + slab.halfSize});
					planes.push_back({-slab.dir[0], -slab.dir[1], -slab.dir[2], slab.halfSize - slab.midPoint});
				}

				leaf.push_back(writer.brushes.size());
				writer.brushes.push_back(brush_json(bounds_of(hull.bounds), std::move(planes)));
			}

			if (leaf.empty())
			{
				leaf.push_back(writer.brushes.size());
				writer.brushes.push_back(brush_json({{-0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, 0.5f}}, json::array()));
			}

			submodels.push_back(submodel_json(trigger_bounds(triggers, t), writer.add_leaf(std::move(leaf))));
		}

		// The glass panes: the brush model of each holds IW7's collision of its piece (the piece whose centre is the
		// pane's), which the pipeline makes the hull of its breakable glass. Without one, the pipeline gives a pane the
		// box of its bounds, which are as deep as the glass is thick in Replay's units.
		const auto panes = glass_panes(source);
		std::size_t glass_hulls{};
		const auto centre_of = [](const std::array<float, 3>& mins, const std::array<float, 3>& maxs)
		{
			return std::array<float, 3>{(mins[0] + maxs[0]) / 2.0f, (mins[1] + maxs[1]) / 2.0f, (mins[2] + maxs[2]) / 2.0f};
		};

		for (const auto& pane : panes)
		{
			const auto centre = centre_of(pane.mins, pane.maxs);
			const havok_convex* piece{};
			auto nearest = 1.0f;
			for (const auto* convex : glass)
			{
				std::array<float, 3> mins{FLT_MAX, FLT_MAX, FLT_MAX};
				std::array<float, 3> maxs{-FLT_MAX, -FLT_MAX, -FLT_MAX};
				for (const auto& point : convex->points)
				{
					for (std::size_t axis = 0; axis < 3; axis++)
					{
						mins[axis] = std::min(mins[axis], point[axis]);
						maxs[axis] = std::max(maxs[axis], point[axis]);
					}
				}

				const auto other = centre_of(mins, maxs);
				const auto distance = std::max({std::abs(other[0] - centre[0]), std::abs(other[1] - centre[1]), std::abs(other[2] - centre[2])});
				if (distance < nearest)
				{
					nearest = distance;
					piece = convex;
				}
			}

			const auto brush = piece ? writer.add_brush(*piece, world_shapes->tags[piece->tag]) : std::nullopt;
			glass_hulls += brush.has_value();
			submodels.push_back(submodel_json({pane.mins, pane.maxs}, brush ? writer.add_leaf(json::array({*brush})) : 0));
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

		ZONETOOL_INFO("iw7: exported the collision of %zu brush models (%zu of their triangles do not convert), %u trigger models "
			"and %zu of %zu glass panes; %zu glass pieces outside the panes, %zu flat pieces and %zu pieces without a tag left out",
			solid_models, model_triangles, triggers.count, glass_hulls, panes.size(), glass_pieces, writer.flat, untagged);

		write_json(output.extracted / (bsp_name(source) + ".replay-collision.json"),
		{
			{"schema", 1},
			{"name", bsp_name(source)},
			{"brushes", std::move(writer.brushes)},
			{"leaf_brush_nodes", std::move(writer.nodes)},
			{"materials", json::array()},
			{"submodel_count", submodels.size()},
			{"submodels", std::move(submodels)},
			{"static_models", std::move(static_models)},
			{"dynamic_model_count", 0},
			{"dynamic_models", json::array()},
			{"dynamic_brush_count", 0},
			{"dynamic_brushes", json::array()},
			{"vertices", std::move(writer.vertices)},
			{"triangles", std::move(writer.triangles)},
			{"triangle_contents_schema", "material-partitions-v1"},
			{"triangle_contents", std::move(writer.contents)},
			{"triangle_surfaces", std::move(writer.surfaces)},
			// IW7's player clips at windows are made for its breakable glass, as Replay's are.
			{"keep_window_clips", true},
		});
	}
}
