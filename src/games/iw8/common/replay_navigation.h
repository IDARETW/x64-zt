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

// PathData type of a Path 3D node (IW7 NODE_PATHNODE_3D): the volume graph, spawned from node_3d_pathnode.
inline constexpr std::uint16_t kPathnode3DType = 20;

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

// One link of an authored PathData node: the index of its node in the same list, the authored fDist (IW7 stores the
// horizontal length) and whether it is the negotiation link of a traversal (pathlink_s +7).
struct AuthoredLink
{
    std::uint32_t node{};
    float distance{};
    bool negotiation{};
};

// One node of an authored PathData (IW7 aipaths) in the zone's node order: its PathData type (1 pathnode marker, 16 and 17
// traversal Begin and End, 20 Path 3D, 0 an unnamed error node) and its authored links.
struct AuthoredNode
{
    Vec3 origin{};
    std::uint16_t type{};
    std::vector<AuthoredLink> links;
};

// World collision and authoring markers of one converted map.
struct Source
{
    std::vector<Hull> hulls;
    std::vector<Triangle> triangles;
    std::vector<Vec3> pathNodes; // IW3 node_pathnode origins
    std::vector<Vec3> spawns;    // every mp_*_spawn* origin
    std::vector<Traversal> traversals;
    // The authored PathData of an IW7 map (empty for IW3 and for extractions without it). Build keeps the authored
    // links of the markers, traversal ends and Path 3D nodes that it matches to this graph.
    std::vector<AuthoredNode> authored;
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

// A fixed pathnode: its origin and its PathData type (1 marker, 16/17 traversal ends, 20 Path 3D).
struct FixedPathNode
{
    Vec3 origin{};
    std::uint16_t type{};
};

// The fixed pathnodes in node order. Replay activates fixed nodes only through matching node_* MapEnts records (SP_path_node
// 0x1411D7290 takes the next node index per record), so the map entity string must carry one record per node, in this
// order: node_3d_pathnode for a Path 3D node (type 20, stock mp_m_speed carries 61 of them), node_pathnode otherwise.
std::vector<FixedPathNode> FixedPathNodes(const Result &result);

// Registers the three assets under the map's d3dbsp name.
void Register(ZoneWriter &writer, const std::string &assetName, const Result &result);
} // namespace iw8::navigation
