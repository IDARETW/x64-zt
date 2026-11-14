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
#include <unordered_map>
#include <utility>
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

// One neighbour of a mount edge (EdgeAdjacency, 12 bytes): the part of this edge (0..1 from a to b, half floats) that
// lies beside the part adjStart..adjEnd of the edge adjEdge. A mounted player can move from one to the other.
struct MapEdgeAdjacency {
    uint16_t start = 0, end = 0;
    uint32_t adjEdge = 0;
    uint16_t adjStart = 0, adjEnd = 0;
};

struct MapEdgeList {
    std::vector<MapEdge> edges;
    std::vector<MapEdgeTree> trees;                     // by bucket, ascending; only buckets that have edges
    std::vector<std::array<MapEdgeNode, 8>> nodeSets;   // all trees' sets
    std::vector<uint32_t> indices;                      // the leaves' edge lists
    // Mount adjacency: empty, or one word per edge (first record in the low 23 bits, the number of records in bits 23
    // and 24) and the records.
    std::vector<uint32_t> adjacencyMeta;
    std::vector<MapEdgeAdjacency> adjacency;
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

// The mount adjacency of a list's edges, as stock maps have it (research 125 section 11): two mount edges are
// neighbours when they are the two long sides of one narrow face: parallel (within about 2.5 degrees), 0.5 to 16
// units apart, one face of each in the same plane between them, their other faces turned away from each other, and at
// least 4 units of their lengths side by side. Each names the other with the parts that lie side by side. An edge
// keeps at most three neighbours (the count has two bits; stock has at most two), the nearest first.
inline void buildMapEdgeAdjacency(MapEdgeList& list) {
    using namespace edge_detail;
    list.adjacencyMeta.clear();
    list.adjacency.clear();
    struct Info {
        double d[3], adjacent[3], other[3], length;
    };
    std::vector<Info> info(list.edges.size());
    std::vector<uint32_t> mount;
    // Edges by grid cell: an edge is put into every cell a point of it (one every 16 units) lies in or near.
    constexpr double kCell = 64.0, kReach = 16.0;
    std::unordered_map<uint64_t, std::vector<uint32_t>> cells;
    const auto cellKey = [](long long x, long long y, long long z) {
        return (uint64_t(x & 0x1FFFFF) << 42) | (uint64_t(y & 0x1FFFFF) << 21) | uint64_t(z & 0x1FFFFF);
    };
    for (uint32_t i = 0; i < list.edges.size(); ++i) {
        const MapEdge& e = list.edges[i];
        if (!(e.flags & kEdgeFlagMount)) continue;
        Info& in = info[i];
        for (int k = 0; k < 3; ++k) in.d[k] = double(e.b[k]) - e.a[k];
        in.length = std::sqrt(dot(in.d, in.d));
        if (in.length < 1e-6) continue;
        for (int k = 0; k < 3; ++k) in.d[k] /= in.length;
        mapEdgeNormals(e, in.adjacent, in.other);
        mount.push_back(i);
        const int steps = std::max(1, int(std::ceil(in.length / 16.0)));
        for (int sIdx = 0; sIdx <= steps; ++sIdx) {
            const double t = in.length * sIdx / steps;
            long long c[3];
            for (int k = 0; k < 3; ++k) c[k] = static_cast<long long>(std::floor((e.a[k] + in.d[k] * t) / kCell));
            auto& cell = cells[cellKey(c[0], c[1], c[2])];
            if (cell.empty() || cell.back() != i) cell.push_back(i);
        }
    }
    struct Neighbour {
        uint32_t edge;
        double distance, start, end, adjStart, adjEnd;
    };
    std::vector<std::vector<Neighbour>> found(list.edges.size());
    const auto consider = [&](uint32_t i, uint32_t j) {
        const Info &a = info[i], &b = info[j];
        if (std::fabs(dot(a.d, b.d)) < 0.999) return;
        double off[3], across[3];
        for (int k = 0; k < 3; ++k) off[k] = double(list.edges[j].a[k]) - list.edges[i].a[k];
        const double along = dot(off, a.d);
        for (int k = 0; k < 3; ++k) across[k] = off[k] - a.d[k] * along;
        const double apart = std::sqrt(dot(across, across));
        if (apart < 0.5 || apart > kReach) return;
        // The face they share, and each edge's other face.
        const double* shared = nullptr;
        const double* otherA = nullptr;
        const double* otherB = nullptr;
        const double* facesA[2] = {a.adjacent, a.other};
        const double* facesB[2] = {b.adjacent, b.other};
        for (int x = 0; x < 2 && !shared; ++x)
            for (int y = 0; y < 2 && !shared; ++y)
                if (dot(facesA[x], facesB[y]) > 0.99) shared = facesA[x], otherA = facesA[1 - x], otherB = facesB[1 - y];
        if (!shared || std::fabs(dot(across, shared)) > 0.5) return;
        if (dot(otherA, across) > -0.1 * apart || dot(otherB, across) < 0.1 * apart) return;
        // The parts side by side, in each edge's own measure.
        double endB[3];
        for (int k = 0; k < 3; ++k) endB[k] = double(list.edges[j].b[k]) - list.edges[i].a[k];
        const double t0 = along / a.length, t1 = dot(endB, a.d) / a.length;
        const double lo = std::max(0.0, std::min(t0, t1)), hi = std::min(1.0, std::max(t0, t1));
        if ((hi - lo) * a.length < 4.0) return;
        double backA[3], backB[3];
        for (int k = 0; k < 3; ++k) backA[k] = double(list.edges[i].a[k]) - list.edges[j].a[k], backB[k] = double(list.edges[i].b[k]) - list.edges[j].a[k];
        const double u0 = dot(backA, b.d) / b.length, u1 = dot(backB, b.d) / b.length;
        const double lo2 = std::max(0.0, std::min(u0, u1)), hi2 = std::min(1.0, std::max(u0, u1));
        if (hi2 <= lo2) return;
        found[i].push_back({j, apart, lo, hi, lo2, hi2});
        found[j].push_back({i, apart, lo2, hi2, lo, hi});
    };
    for (const uint32_t i : mount) {
        const MapEdge& e = list.edges[i];
        const Info& in = info[i];
        std::vector<uint32_t> close;  // (not "near": a macro in the Windows headers the converter includes)
        const int steps = std::max(1, int(std::ceil(in.length / 16.0)));
        for (int sIdx = 0; sIdx <= steps; ++sIdx) {
            const double t = in.length * sIdx / steps;
            long long lo[3], hi[3];
            for (int k = 0; k < 3; ++k) {
                const double v = e.a[k] + in.d[k] * t;
                lo[k] = static_cast<long long>(std::floor((v - kReach) / kCell));
                hi[k] = static_cast<long long>(std::floor((v + kReach) / kCell));
            }
            for (long long x = lo[0]; x <= hi[0]; ++x)
                for (long long y = lo[1]; y <= hi[1]; ++y)
                    for (long long z = lo[2]; z <= hi[2]; ++z) {
                        const auto it = cells.find(cellKey(x, y, z));
                        if (it == cells.end()) continue;
                        for (const uint32_t j : it->second)
                            if (j > i) close.push_back(j);  // each pair once
                    }
        }
        std::sort(close.begin(), close.end());
        close.erase(std::unique(close.begin(), close.end()), close.end());
        for (const uint32_t j : close) consider(i, j);
    }
    bool any = false;
    for (const auto& f : found) any = any || !f.empty();
    if (!any) return;
    list.adjacencyMeta.assign(list.edges.size(), 0);
    for (uint32_t i = 0; i < list.edges.size(); ++i) {
        auto& f = found[i];
        if (f.empty()) continue;
        std::stable_sort(f.begin(), f.end(), [](const Neighbour& x, const Neighbour& y) { return x.distance < y.distance; });
        if (f.size() > 3) f.resize(3);
        list.adjacencyMeta[i] = uint32_t(list.adjacency.size()) | (uint32_t(f.size()) << 23);
        for (const Neighbour& n : f)
            list.adjacency.push_back({edgeHalfFromFloat(float(n.start)), edgeHalfFromFloat(float(n.end)), n.edge,
                                      edgeHalfFromFloat(float(n.adjStart)), edgeHalfFromFloat(float(n.adjEnd))});
    }
}

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
    buildMapEdgeAdjacency(list);
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
    if (list.adjacencyMeta.empty() != list.adjacency.empty()) return "adjacency records without their index, or the index without records";
    if (!list.adjacencyMeta.empty()) {
        if (list.adjacencyMeta.size() != list.edges.size()) return "the adjacency index does not have one word per edge";
        if (list.adjacency.size() >= (1u << 23)) return "too many adjacency records";
        for (uint32_t i = 0; i < list.edges.size(); ++i) {
            const uint32_t first = list.adjacencyMeta[i] & 0x7FFFFFu, count = (list.adjacencyMeta[i] >> 23) & 3u;
            if (list.adjacencyMeta[i] >> 25) return "an adjacency word uses bits beyond the mount count";
            if (size_t(first) + count > list.adjacency.size()) return "an edge's adjacency records lie outside the table";
            for (uint32_t k = 0; k < count; ++k) {
                const MapEdgeAdjacency& r = list.adjacency[first + k];
                if (r.adjEdge >= list.edges.size() || r.adjEdge == i) return "an adjacency record names no other edge";
                if (!(list.edges[i].flags & kEdgeFlagMount) || !(list.edges[r.adjEdge].flags & kEdgeFlagMount)) return "an adjacency record joins an edge that is no mount edge";
                // The neighbour names this edge back.
                const uint32_t back = list.adjacencyMeta[r.adjEdge] & 0x7FFFFFu, backCount = (list.adjacencyMeta[r.adjEdge] >> 23) & 3u;
                bool named = false;
                for (uint32_t q = 0; q < backCount && size_t(back) + q < list.adjacency.size(); ++q) named = named || list.adjacency[back + q].adjEdge == i;
                if (!named && backCount < 3) return "an adjacency record is not answered by its neighbour";
            }
        }
    }
    return "";
}

}  // namespace iw8r
