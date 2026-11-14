// Copied from IW8 Radiant (src/core/map_edges.h) by tools/patches/zt_map_edges.py: do not edit here.
// The edge list of a built map (MapEdgeList): the static edges the game queries for weapon mounting, mantling and
// ladders, with the octrees it walks to find them (research 125).
//
// Header only and standard C++ only: the converter builds the same file (copied by tools/patches/zt_map_edges.py),
// because it writes the asset.
//
// What the game reads (research 125 sections 2 to 4, 8 and 9, all checked against stock maps):
//   - an edge: two end points and 8 bytes of metadata: one face normal as an angle about the edge, the angle to the
//     other face (a half float), and flags (1 mount, 2 mantle on, 4 ladder centre line, 8 mantle over);
//   - per bucket (0 mount, 1 mantle, 2 ladder centre lines) one octree over the edges with that bucket's flag. A node
//     is a cube `minExtent + gridIndex * 2 * halfWidth[depth]`; an inner node has a set of eight children; a leaf
//     lists every edge that crosses its cube.
// A query for a bucket walks that bucket's tree with a box and gets the edges of the leaves the box touches.
#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <vector>

namespace iw8r {

inline constexpr uint8_t kEdgeFlagMount = 1, kEdgeFlagMantleOn = 2, kEdgeFlagLadder = 4, kEdgeFlagMantleOver = 8;
inline constexpr int kEdgeBucketMount = 0, kEdgeBucketMantle = 1, kEdgeBucketLadder = 2;
// Node flags (PMROctreeNode.flags).
inline constexpr uint8_t kEdgeNodeEmpty = 1, kEdgeNodeLeaf = 2, kEdgeNodeRoot = 4;
// The parent index the children of a root carry in stock maps.
inline constexpr uint32_t kEdgeRootParent = 0xFFFFF8u;

// One static edge in world space, as the asset stores it.
struct MapEdge {
    float a[3] = {0, 0, 0};
    float b[3] = {0, 0, 0};
    float adjacentNormal = 0;  // radians: the reference turned right-handed about (b - a) by this is one face's normal
    uint16_t openAngle = 0;    // half float, radians: that normal turned on by this is the other face's normal
    uint8_t flags = 0;
};

// A 16-byte octree node. Inner node: a = the parent node (set * 8 + slot; kEdgeRootParent below a root), b = its set
// of children. Leaf: a = first entry in the index list, b = the number of entries.
struct MapEdgeNode {
    uint16_t grid[3] = {0, 0, 0};
    uint8_t depth = 0;
    uint8_t flags = 0;
    uint32_t a = 0;
    uint32_t b = 0;
};

struct MapEdgeTree {
    int bucket = 0;
    float minExtent[3] = {0, 0, 0};  // the root cube's lowest corner
    float halfWidth = 0.5f;          // half the root cube's side; depth d has halfWidth / 2^d
    MapEdgeNode root;                // flags kEdgeNodeRoot, or kEdgeNodeRoot | kEdgeNodeLeaf for a tree of one leaf
    uint32_t nodeSetCount = 0;       // the sets this tree owns (they follow each other from root.b)
};

struct MapEdgeList {
    std::vector<MapEdge> edges;
    std::vector<MapEdgeTree> trees;                     // by bucket, ascending; only buckets that have edges
    std::vector<std::array<MapEdgeNode, 8>> nodeSets;   // all trees' sets
    std::vector<uint32_t> indices;                      // the leaves' edge lists
    float mins[3] = {0, 0, 0}, maxs[3] = {0, 0, 0};     // the box of all edges
    uint32_t queryTypes = 0;                            // bit `bucket` for every tree
};

// The offsets of a set's eight children from twice the parent's grid index, in the order stock maps store them.
inline constexpr int kEdgeChildOffset[8][3] = {{0, 0, 0}, {0, 0, 1}, {0, 1, 1}, {0, 1, 0}, {1, 1, 0}, {1, 1, 1}, {1, 0, 1}, {1, 0, 0}};

inline uint16_t edgeHalfFromFloat(float value) {
    uint32_t bits;
    std::memcpy(&bits, &value, 4);
    const uint32_t sign = (bits >> 16) & 0x8000u;
    const int exponent = int((bits >> 23) & 0xFF) - 127 + 15;
    uint32_t mantissa = bits & 0x7FFFFFu;
    if (exponent <= 0) {
        if (exponent < -10) return uint16_t(sign);
        mantissa = (mantissa | 0x800000u) >> (1 - exponent);
        return uint16_t(sign | ((mantissa + 0x1000u) >> 13));
    }
    if (exponent >= 31) return uint16_t(sign | 0x7C00u);
    return uint16_t((sign | (uint32_t(exponent) << 10) | (mantissa >> 13)) + ((mantissa & 0x1000u) ? 1u : 0u));
}

inline float edgeFloatFromHalf(uint16_t h) {
    const int exponent = (h >> 10) & 0x1F;
    const int mantissa = h & 0x3FF;
    float value;
    if (exponent == 0) value = std::ldexp(float(mantissa), -24);
    else if (exponent == 31) value = mantissa ? std::nanf("") : INFINITY;
    else value = std::ldexp(float(mantissa + 1024), exponent - 25);
    return (h & 0x8000) ? -value : value;
}

namespace edge_detail {
inline void cross(const double a[3], const double b[3], double out[3]) {
    out[0] = a[1] * b[2] - a[2] * b[1];
    out[1] = a[2] * b[0] - a[0] * b[2];
    out[2] = a[0] * b[1] - a[1] * b[0];
}
inline double dot(const double a[3], const double b[3]) { return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]; }
inline bool normalize(double v[3]) {
    const double len = std::sqrt(dot(v, v));
    if (len < 1e-12) return false;
    v[0] /= len, v[1] /= len, v[2] /= len;
    return true;
}
// The reference the stored angle turns: world X for an edge within about 2.6 degrees of vertical, else world up made
// perpendicular to the edge (the game's Edge_OrientStaticSegment, research 125 section 4).
inline void reference(const double d[3], double out[3]) {
    if (std::fabs(d[2]) > 0.999) {
        const double x[3] = {1, 0, 0};
        const double k = dot(x, d);
        out[0] = 1 - d[0] * k, out[1] = -d[1] * k, out[2] = -d[2] * k;
    } else {
        out[0] = -d[0] * d[2], out[1] = -d[1] * d[2], out[2] = 1 - d[2] * d[2];
    }
    normalize(out);
}
// `v` turned right-handed about the unit axis `d` by `angle`.
inline void rotate(const double v[3], const double d[3], double angle, double out[3]) {
    double c[3];
    cross(d, v, c);
    const double k = dot(d, v) * (1 - std::cos(angle));
    for (int i = 0; i < 3; ++i) out[i] = v[i] * std::cos(angle) + c[i] * std::sin(angle) + d[i] * k;
}
// The right-handed angle about `d` that turns `from` into `to` (both perpendicular to d), in (-pi, pi].
inline double angleAbout(const double from[3], const double to[3], const double d[3]) {
    double c[3];
    cross(from, to, c);
    return std::atan2(dot(c, d), dot(from, to));
}
}  // namespace edge_detail

// The edge where two faces of a solid meet, from the faces' outward normals. The end points may be swapped and the
// faces take the order the game expects: cross(adjacent, other) points along the edge. `preferFirst`: keep `n1` as
// the adjacent face (a ledge top: the top face, whose angle is then 0) and turn the edge round if needed.
inline bool makeMapEdge(const double a[3], const double b[3], const double n1[3], const double n2[3], uint8_t flags, MapEdge& out,
                        bool preferFirst = true) {
    using namespace edge_detail;
    double d[3] = {b[0] - a[0], b[1] - a[1], b[2] - a[2]};
    if (!normalize(d)) return false;
    // The normals without their part along the edge.
    double p1[3], p2[3];
    for (int i = 0; i < 3; ++i) p1[i] = n1[i] - d[i] * dot(n1, d), p2[i] = n2[i] - d[i] * dot(n2, d);
    if (!normalize(p1) || !normalize(p2)) return false;
    double c[3];
    cross(p1, p2, c);
    const double turn = dot(c, d);
    if (std::fabs(turn) < 1e-6) return false;  // the faces lie in one plane, or back to back
    const double* first = a;
    const double* second = b;
    const double* adjacent = p1;
    const double* other = p2;
    if (turn < 0) {
        if (preferFirst) {
            first = b, second = a;
            d[0] = -d[0], d[1] = -d[1], d[2] = -d[2];
        } else {
            adjacent = p2, other = p1;
        }
    }
    double ref[3];
    reference(d, ref);
    for (int i = 0; i < 3; ++i) out.a[i] = float(first[i]), out.b[i] = float(second[i]);
    double angle = angleAbout(ref, adjacent, d);
    if (std::fabs(angle) < 1e-6) angle = 0;
    out.adjacentNormal = float(angle);
    out.openAngle = edgeHalfFromFloat(float(angleAbout(adjacent, other, d)));
    out.flags = flags;
    return true;
}

// The two face normals an edge's metadata stands for, as the game unpacks them.
inline void mapEdgeNormals(const MapEdge& e, double adjacent[3], double other[3]) {
    using namespace edge_detail;
    double d[3] = {double(e.b[0]) - e.a[0], double(e.b[1]) - e.a[1], double(e.b[2]) - e.a[2]};
    normalize(d);
    double ref[3];
    reference(d, ref);
    rotate(ref, d, e.adjacentNormal, adjacent);
    rotate(adjacent, d, edgeFloatFromHalf(e.openAngle), other);
}

inline int mapEdgeBucketMask(uint8_t flags) {
    return ((flags & kEdgeFlagMount) ? 1 : 0) | ((flags & (kEdgeFlagMantleOn | kEdgeFlagMantleOver)) ? 2 : 0) | ((flags & kEdgeFlagLadder) ? 4 : 0);
}

namespace edge_detail {
// Does the segment a-b touch the cube (lowest corner `lo`, side `side`, grown by `grow`)?
inline bool segmentInCube(const float a[3], const float b[3], const double lo[3], double side, double grow) {
    double t0 = 0, t1 = 1;
    for (int axis = 0; axis < 3; ++axis) {
        const double mn = lo[axis] - grow, mx = lo[axis] + side + grow;
        const double p = a[axis], dir = double(b[axis]) - a[axis];
        if (std::fabs(dir) < 1e-12) {
            if (p < mn || p > mx) return false;
            continue;
        }
        double u0 = (mn - p) / dir, u1 = (mx - p) / dir;
        if (u0 > u1) std::swap(u0, u1);
        t0 = std::max(t0, u0), t1 = std::min(t1, u1);
        if (t0 > t1) return false;
    }
    return true;
}

struct TreeBuild {
    MapEdgeList& list;
    const MapEdgeTree& tree;
    int maxLeaf, maxDepth;

    void cube(const uint16_t grid[3], int depth, double lo[3], double& side) const {
        side = 2.0 * std::ldexp(double(tree.halfWidth), -depth);
        for (int i = 0; i < 3; ++i) lo[i] = tree.minExtent[i] + grid[i] * side;
    }
    MapEdgeNode leaf(const uint16_t grid[3], int depth, const std::vector<uint32_t>& edges, uint8_t extra) const {
        MapEdgeNode n;
        std::copy_n(grid, 3, n.grid);
        n.depth = uint8_t(depth);
        n.flags = uint8_t(kEdgeNodeLeaf | extra | (edges.empty() ? kEdgeNodeEmpty : 0));
        if (!edges.empty()) {
            n.a = uint32_t(list.indices.size());
            n.b = uint32_t(edges.size());
            list.indices.insert(list.indices.end(), edges.begin(), edges.end());
        }
        return n;
    }
    // The children of the node `parent` (its index, or kEdgeRootParent) at `grid`/`depth` holding `edges`: a new set.
    uint32_t split(const uint16_t grid[3], int depth, const std::vector<uint32_t>& edges, uint32_t parent) {
        const uint32_t set = uint32_t(list.nodeSets.size());
        list.nodeSets.emplace_back();
        for (int slot = 0; slot < 8; ++slot) {
            const uint16_t g[3] = {uint16_t(grid[0] * 2 + kEdgeChildOffset[slot][0]), uint16_t(grid[1] * 2 + kEdgeChildOffset[slot][1]),
                                   uint16_t(grid[2] * 2 + kEdgeChildOffset[slot][2])};
            double lo[3], side;
            cube(g, depth + 1, lo, side);
            std::vector<uint32_t> mine;
            for (const uint32_t e : edges)
                if (segmentInCube(list.edges[e].a, list.edges[e].b, lo, side, 0.01)) mine.push_back(e);
            MapEdgeNode n;
            if (int(mine.size()) <= maxLeaf || depth + 1 >= maxDepth) {
                n = leaf(g, depth + 1, mine, 0);
            } else {
                std::copy_n(g, 3, n.grid);
                n.depth = uint8_t(depth + 1);
                n.flags = 0;
                n.a = parent;
                n.b = split(g, depth + 1, mine, set * 8u + uint32_t(slot));  // its children name it as their parent
            }
            list.nodeSets[set][size_t(slot)] = n;
        }
        return set;
    }
};
}  // namespace edge_detail

// Builds the list: the box, and one tree per bucket that has edges. A leaf holds at most `maxLeafEdges` edges unless
// it sits at `maxDepth` (stock leaves hold at most 10, at depths up to 7; the game's tables have 16 depths).
// `flatBuckets`: bit `bucket` set = that bucket's tree is one leaf, whatever it holds (the ladder tree as it has been
// written, and seen working, since before the mount edges).
inline MapEdgeList buildMapEdgeList(std::vector<MapEdge> edges, int maxLeafEdges = 8, int maxDepth = 10, unsigned flatBuckets = 0) {
    MapEdgeList list;
    list.edges = std::move(edges);
    maxDepth = std::clamp(maxDepth, 1, 15);
    if (list.edges.empty()) return list;
    for (int i = 0; i < 3; ++i) list.mins[i] = INFINITY, list.maxs[i] = -INFINITY;
    for (const MapEdge& e : list.edges)
        for (int i = 0; i < 3; ++i) {
            list.mins[i] = std::min({list.mins[i], e.a[i], e.b[i]});
            list.maxs[i] = std::max({list.maxs[i], e.a[i], e.b[i]});
        }
    for (int bucket = 0; bucket < 3; ++bucket) {
        std::vector<uint32_t> mine;
        float lo[3] = {INFINITY, INFINITY, INFINITY}, hi[3] = {-INFINITY, -INFINITY, -INFINITY};
        for (uint32_t i = 0; i < list.edges.size(); ++i) {
            const MapEdge& e = list.edges[i];
            if (!(mapEdgeBucketMask(e.flags) & (1 << bucket))) continue;
            mine.push_back(i);
            for (int k = 0; k < 3; ++k) lo[k] = std::min({lo[k], e.a[k], e.b[k]}), hi[k] = std::max({hi[k], e.a[k], e.b[k]});
        }
        if (mine.empty()) continue;
        MapEdgeTree tree;
        tree.bucket = bucket;
        // The root cube: as wide as the edges' longest extent (a little more, so no edge lies on its skin), centred.
        float half = 0.5f;
        for (int k = 0; k < 3; ++k) half = std::max(half, (hi[k] - lo[k]) * 0.5f);
        half *= 1.001f;
        tree.halfWidth = half;
        for (int k = 0; k < 3; ++k) tree.minExtent[k] = (lo[k] + hi[k]) * 0.5f - half;
        list.trees.push_back(tree);
        MapEdgeTree& t = list.trees.back();
        edge_detail::TreeBuild build{list, t, std::max(1, maxLeafEdges), maxDepth};
        const uint16_t origin[3] = {0, 0, 0};
        if (int(mine.size()) <= build.maxLeaf || (flatBuckets & (1u << bucket))) {
            t.root = build.leaf(origin, 0, mine, kEdgeNodeRoot);
            t.root.flags = uint8_t(kEdgeNodeLeaf | kEdgeNodeRoot);
        } else {
            const uint32_t before = uint32_t(list.nodeSets.size());
            t.root.flags = kEdgeNodeRoot;
            t.root.a = 0xFFFFFFFFu;
            t.root.b = build.split(origin, 0, mine, kEdgeRootParent);
            t.nodeSetCount = uint32_t(list.nodeSets.size()) - before;
        }
        list.queryTypes |= 1u << bucket;
    }
    return list;
}

// The edges a query box gets for a bucket, found the way the game walks the tree (each edge once, ascending).
inline std::vector<uint32_t> queryMapEdgeList(const MapEdgeList& list, int bucket, const float mins[3], const float maxs[3]) {
    std::vector<uint32_t> out;
    for (const MapEdgeTree& tree : list.trees) {
        if (tree.bucket != bucket) continue;
        std::vector<MapEdgeNode> stack{tree.root};
        while (!stack.empty()) {
            const MapEdgeNode n = stack.back();
            stack.pop_back();
            const double side = 2.0 * std::ldexp(double(tree.halfWidth), -int(n.depth));
            bool touches = true;
            for (int i = 0; i < 3 && touches; ++i) {
                const double lo = tree.minExtent[i] + n.grid[i] * side;
                touches = lo <= maxs[i] && lo + side >= mins[i];
            }
            if (!touches) continue;
            if (n.flags & kEdgeNodeLeaf) {
                for (uint32_t k = 0; k < n.b && size_t(n.a) + k < list.indices.size(); ++k) out.push_back(list.indices[n.a + k]);
            } else if (n.b < list.nodeSets.size()) {
                for (const MapEdgeNode& c : list.nodeSets[n.b]) stack.push_back(c);
            }
        }
    }
    std::sort(out.begin(), out.end());
    out.erase(std::unique(out.begin(), out.end()), out.end());
    return out;
}

// Checks a list against the rules the game's walk relies on. Returns "" or what is wrong.
inline const char* validateMapEdgeList(const MapEdgeList& list) {
    uint32_t nextSet = 0;
    for (const MapEdgeTree& tree : list.trees) {
        if (!(tree.root.flags & kEdgeNodeRoot)) return "a tree's root lacks the root flag";
        if (tree.root.flags & kEdgeNodeLeaf) {
            if (tree.nodeSetCount) return "a one-leaf tree owns node sets";
            if (size_t(tree.root.a) + tree.root.b > list.indices.size()) return "a root leaf's edges lie outside the index list";
            continue;
        }
        if (tree.root.b != nextSet) return "a tree's first node set does not follow the tree before it";
        struct Item {
            uint32_t set, parent;
            uint16_t grid[3];
            int depth;
        };
        std::vector<Item> stack{{tree.root.b, kEdgeRootParent, {0, 0, 0}, 0}};
        uint32_t seen = 0;
        while (!stack.empty()) {
            const Item it = stack.back();
            stack.pop_back();
            if (it.set >= list.nodeSets.size()) return "a node set index is out of range";
            ++seen;
            for (uint32_t slot = 0; slot < 8; ++slot) {
                const MapEdgeNode& n = list.nodeSets[it.set][slot];
                if (n.depth != it.depth + 1) return "a node's depth is not its parent's plus one";
                for (int i = 0; i < 3; ++i)
                    if (n.grid[i] != it.grid[i] * 2 + kEdgeChildOffset[slot][i]) return "a node's grid index does not fit its slot";
                if (n.flags & kEdgeNodeLeaf) {
                    if (size_t(n.a) + n.b > list.indices.size()) return "a leaf's edges lie outside the index list";
                    if (bool(n.flags & kEdgeNodeEmpty) != (n.b == 0)) return "a leaf's empty flag does not fit its count";
                    for (uint32_t k = 0; k < n.b; ++k)
                        if (list.indices[n.a + k] >= list.edges.size()) return "a leaf names an edge that does not exist";
                } else {
                    if (n.a != it.parent) return "an inner node does not name its parent";
                    Item child{n.b, it.set * 8u + slot, {n.grid[0], n.grid[1], n.grid[2]}, it.depth + 1};
                    stack.push_back(child);
                }
            }
        }
        if (seen != tree.nodeSetCount) return "a tree's node set count does not fit its nodes";
        nextSet += tree.nodeSetCount;
    }
    if (nextSet != list.nodeSets.size()) return "node sets no tree owns";
    return "";
}

}  // namespace iw8r
