#pragma once

// editzone "remove_world_surfaces" (IW8 Radiant research 96): world geometry of a stock map taken out by world-space
// boxes. The render triangles inside a box become degenerate (their three indices the same vertex), so every draw path
// that reads the world index buffer (opaque, depth prepass, shadow casters, decals) draws nothing for them while every
// count, offset and surface index stays as stock. The collision triangles and convex shapes inside a box leave the
// world's Havok compressed mesh in place, as Havok's own primitive removal does it: nothing that stays moves.

#include <array>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <set>
#include <span>
#include <string>
#include <vector>

namespace zonetool::iw8
{
	struct GfxWorld;
	namespace havok
	{
		class type_table;
	}
}

namespace iw8::stockworld
{
	// Game units, inclusive.
	struct Box
	{
		std::array<float, 3> mins{};
		std::array<float, 3> maxs{};
	};

	bool Inside(const Box& box, const float* point, float scale = 1.0f);

	struct RenderRemoval
	{
		std::size_t triangles = 0;        // triangles made degenerate
		std::size_t alreadyDegenerate = 0; // selected triangles that were degenerate in the stock data
		std::vector<std::uint32_t> touched; // surfaces with at least one triangle removed
		std::vector<std::uint32_t> emptied; // surfaces with every triangle removed
		std::size_t notResident = 0;      // surfaces whose vertices or indices are streamed (not looked at)
	};

	// Triangles whose three vertices lie inside one of the boxes, and every triangle of the surfaces listed in `whole`,
	// become (i0, i0, i0). The index buffers are copied into `kept` (kept alive by the caller until the zone is written)
	// and the transient zones point at the copies.
	RenderRemoval RemoveRenderTriangles(zonetool::iw8::GfxWorld& world, std::span<const Box> boxes,
		const std::set<std::uint32_t>& whole, std::deque<std::vector<std::uint16_t>>& kept);

	struct CollisionRemoval
	{
		std::size_t shapes = 0;
		std::size_t trianglesBefore = 0, trianglesRemoved = 0, trianglesAfter = 0;
		std::size_t convexBefore = 0, convexRemoved = 0, convexAfter = 0;
		std::size_t bytesBefore = 0, bytesAfter = 0;
	};

	// The Havok data of clipMap_t.havokWorldShapeData without the mesh triangles and convex shape instances that lie
	// inside a box (every vertex; the convex radius is not counted): their primitives become removed primitives (DE AD
	// DE AD) or a quad keeps its other half; every other byte stays stock (the list's vertex, triangle and convex counts
	// too). Returns the stock bytes when nothing is inside. A list with collision-tile slots (research 90) is refused.
	std::vector<std::uint8_t> RemoveCollision(std::span<const std::uint8_t> data, const zonetool::iw8::havok::type_table& compendium,
		std::span<const Box> boxes, CollisionRemoval& report);

	// The world shape list as JSON for checks: per compressed mesh its triangles (game units, nine numbers each) and
	// shape tags, and its convex instances (placed bounds in game units, shape tag, live: a primitive names it).
	std::string DescribeCollision(std::span<const std::uint8_t> data, const zonetool::iw8::havok::type_table& compendium);

	// Umbra occlusion (research 96, "Occlusion"). Replay culls world surfaces, static models, lights, probes and decal
	// collections with the precomputed Umbra tome (GfxWorld.umbraTomeData): the occluders (surfaces with
	// GFX_SURF_IS_UMBRA_OCCLUDER, static model collections with STATIC_MODEL_COLLECTION_FLAG_UMBRA_OCCLUDER) were
	// voxelized into its cells and portals when the map was built, so a removed occluder still blocks the view there.
	struct StockTome
	{
		std::uint32_t objects = 0;
		std::uint32_t gates = 0, tomes = 0, clusters = 0, tiles = 0, leafTiles = 0;
		bool userIDStarts = false; // objects with more than one user ID
		std::array<float, 3> mins{}, maxs{};
		std::vector<std::uint32_t> userIDs; // one per object
		bool checksumValid = false;
		bool checksumDisabled = false; // m_crc32 0xD15AB1ED: the stock tomes carry no checksum
	};
	StockTome ReadTome(std::span<const std::uint8_t> tome);

	struct OcclusionOpening
	{
		std::uint32_t objects = 0;
		std::array<std::uint32_t, 8> classes{}; // objects per user-ID class (bits 28-30)
		std::size_t bytesBefore = 0, bytesAfter = 0;
	};

	// The stock tome replaced by the converter's conservative tome (replayrender::BuildConservativeUmbraTome, the one
	// every converted map runs with): the same objects with the same user IDs, in the stock order, one cell over the
	// stock tome's bounds, so Umbra hides nothing and the GPU passes (frustum, Hi-Z, LOD) do all the culling. Cells,
	// portals and clusters of the stock tome are opaque to the converter, so a removed occluder cannot be taken out of
	// them alone. Refused: a tome with gates (GfxWorld.umbraGates name them), tome collections or several user IDs per
	// object.
	std::vector<std::uint8_t> OpenOcclusion(std::span<const std::uint8_t> stockTome, OcclusionOpening& report);
}
