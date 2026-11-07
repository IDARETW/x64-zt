#pragma once

#include "../zone/iw8_zone.h"

#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace iw8::navigation
{
using Vec3 = std::array<float, 3>;

// What a converted collision piece blocks for the generated bot data.
inline constexpr std::uint32_t kBlocksMovement = 1u;
inline constexpr std::uint32_t kBlocksSight = 2u;

struct Hull
{
    std::vector<Vec3> points; // vertices of one convex solid
    std::uint32_t flags{};
};

struct Triangle
{
    std::array<Vec3, 3> corners{};
    std::uint32_t flags{};
};

// An authored traversal (IW7 node_negotiation_begin -> its node_negotiation_end). It becomes a Begin pathnode
// (type 16) with the animscript and one negotiation link to an End pathnode (type 17): Replay builds a bfx nav link
// of each such link at load (Nav_SetupNavLinks 0x140FFD280) and reads the animscript for the bot zone graph
// (Bot_InitZoneGraph 0x14135A260, Bot_AnimscriptToNegotiationType 0x1413398B0).
struct Traversal
{
    Vec3 begin{};
    Vec3 end{};
    std::string animscript;
};

// World collision and authoring markers of one converted map.
struct Source
{
    std::vector<Hull> hulls;
    std::vector<Triangle> triangles;
    std::vector<Vec3> pathNodes; // IW3 node_pathnode origins
    std::vector<Vec3> spawns;    // every mp_*_spawn* origin
    std::vector<Traversal> traversals;
};

struct Data;
struct Result
{
    std::shared_ptr<const Data> data;
    explicit operator bool() const
    {
        return data != nullptr;
    }
};

// Generates the Replay MP bot navigation set from collision: a bfx NavGraph
// (navmesh, type 27), PathData with path extra data (aipaths, type 26) and
// the tactical visibility graph (tacgraph, type 28). Returns an empty result
// when the source has no walkable seed.
Result Build(const Source &source);

// Origins of the fixed pathnodes in node order. Replay activates fixed nodes
// only through matching node_* MapEnts records (SP_path_node 0x1411D7290 takes
// the next node index per record), so the map entity string must carry one
// node_pathnode per origin, in this order.
std::vector<Vec3> FixedPathNodes(const Result &result);

// Registers the three assets under the map's d3dbsp name.
void Register(ZoneWriter &writer, const std::string &assetName, const Result &result);
} // namespace iw8::navigation
