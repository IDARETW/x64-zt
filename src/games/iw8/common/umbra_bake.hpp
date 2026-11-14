// Copied from IW8 Radiant (src/core/umbra_bake.h) by tools/patches/zt_umbra_bake.py: do not edit here.
// An occlusion bake for a converted map: an Umbra tome with real cells and portals (plan 2.4, research 110 and 118).
//
// Header only and standard C++ only: the converter builds the same file (copied by tools/patches/zt_umbra_bake.py),
// because the tome's objects are the converter's final surfaces and placements.
//
// What it makes. One leaf tile (the layout of the converter's proven one-cell tome, with more cells):
//   - occluders: axis-aligned solid boxes (opaque box brushes). Free space is cut into boxes along their faces; every
//     free box is a cell, solid boxes are no cell.
//   - portals: the whole shared rectangle of two touching cells, both ways. Sight through free space always crosses
//     portals, so a walk through them reaches every cell a straight line can reach (it can only reach more).
//   - objects: every object keeps the scene-sized bounds of the one-cell tome (the game's contribution cull judged
//     real bounds too small, owner playtest 2026-11), so an object is hidden only when no cell that lists it is
//     reached. A cell lists the objects whose real box touches it; `everywhere` objects are listed by every cell.
//   - the view tree: the tile's cube split at midpoints until a leaf lies in one cell; a leaf in no cell (solid, or
//     outside the map) lists the nearest cell, so a camera is never without a cell. Occluder faces are moved inward
//     to a grid of about 4 units so that the midpoints can meet them. A leaf never lists several cells (stock tomes
//     never do): when the tree would outgrow its budget, the space is cut again with half the cells.
// Not seen in game. The converter uses it only when the map asks for it.
#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <limits>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

namespace umbrabake {

struct Box {
    float mn[3] = {0, 0, 0}, mx[3] = {0, 0, 0};
};
struct Object {
    uint32_t userId = 0;
    Box box;                  // the object's real bounds
    bool everywhere = false;  // listed by every cell (lights, probes, volumes: no useful box)
};
struct Input {
    Box scene;                    // around everything the game draws
    std::vector<Box> occluders;   // solid axis-aligned boxes
    std::vector<Object> objects;  // in user ID list order: object i is tome object i
    uint32_t maxCells = 4096;     // 2..60000
    float minOccluderSide = 64;   // an occluder needs two sides at least this long
    float objectMargin = 2;       // an object's box grows by this before the cells are chosen
    uint32_t maxViewNodes = 1000000;  // the view tree's size limit (a leaf never lists several cells)
};
struct Stats {
    uint32_t cells = 0, portals = 0, occludersUsed = 0, occludersGiven = 0, solidBoxes = 0;
    uint32_t viewNodes = 0, viewLeaves = 0, listEntries = 0, objectsEverywhere = 0, objectsByNearestCell = 0;
    uint32_t retries = 0;  // times the space was cut again with half the cells, for the view tree to fit
};
struct Result {
    bool ok = false;
    std::string error;
    std::vector<uint8_t> tome;
    Stats stats;
};

namespace detail {

constexpr uint32_t kVersion = 0xD6000014u;
constexpr uint32_t kHeaderSize = 0x170, kTileSize = 0x60, kCellSize = 36, kPortalSize = 16;
constexpr uint32_t kSide = 65536;  // tile quanta along each side of the cube

struct QBox {
    uint32_t lo[3], hi[3];  // quanta, [lo, hi)
};
inline bool overlaps(const QBox& a, const QBox& b) {
    for (int k = 0; k < 3; ++k)
        if (a.lo[k] >= b.hi[k] || b.lo[k] >= a.hi[k]) return false;
    return true;
}
inline bool inside(const QBox& a, const QBox& b) {  // a within b
    for (int k = 0; k < 3; ++k)
        if (a.lo[k] < b.lo[k] || a.hi[k] > b.hi[k]) return false;
    return true;
}
template <typename T>
inline void put(std::vector<uint8_t>& d, size_t at, T v) {
    std::memcpy(d.data() + at, &v, sizeof(T));
}
inline uint32_t allocate16(std::vector<uint8_t>& d, size_t size) {
    d.resize((d.size() + 15) & ~size_t(15));
    const uint32_t at = uint32_t(d.size());
    d.resize(d.size() + size);
    return at;
}
inline uint32_t crc32c(const uint8_t* data, size_t size) {
    uint32_t crc = 0xFFFFFFFFu;
    for (size_t i = 0; i < size; ++i) {
        crc ^= data[i];
        for (int b = 0; b < 8; ++b) crc = (crc >> 1) ^ (0x82F63B78u & (0u - (crc & 1u)));
    }
    return ~crc;
}
// A serialized tree: 2 bits per node in breadth-first order (3 = leaf, else the split axis), then the rank directory
// of inner-node counts (research 110): u32 per 65536-node block after the first, u16 per 256-node superblock that
// does not start a block (relative to its block), u8 per tree word that does not start a superblock (relative to its
// superblock); the u16 and u8 lists are each padded to 4 bytes.
inline std::vector<uint8_t> serializeTree(const std::vector<uint8_t>& codes) {
    const uint32_t nodes = uint32_t(codes.size());
    const uint32_t words = (nodes * 2 + 31) / 32, supers = (words + 15) / 16, blocks = (words + 4095) / 4096;
    std::vector<uint32_t> before(size_t(nodes) + 1, 0);  // inner nodes before node i
    for (uint32_t i = 0; i < nodes; ++i) before[i + 1] = before[i] + (codes[i] != 3 ? 1u : 0u);
    const auto innerBefore = [&](uint64_t node) { return before[size_t(std::min<uint64_t>(node, nodes))]; };
    std::vector<uint8_t> out(size_t(words) * 4, 0);
    for (uint32_t i = 0; i < nodes; ++i) out[size_t(i >> 2)] |= uint8_t((codes[i] & 3u) << (2 * (i & 3u)));
    for (uint32_t b = 1; b < blocks; ++b) {
        const size_t at = out.size();
        out.resize(at + 4);
        put(out, at, uint32_t(innerBefore(uint64_t(b) * 65536)));
    }
    size_t start = out.size();
    for (uint32_t s = 1; s < supers; ++s) {
        if (s % 256 == 0) continue;
        const size_t at = out.size();
        out.resize(at + 2);
        put(out, at, uint16_t(innerBefore(uint64_t(s) * 256) - innerBefore(uint64_t(s / 256) * 65536)));
    }
    out.resize(start + ((out.size() - start) + 3) / 4 * 4);
    start = out.size();
    for (uint32_t s = 0; s < supers; ++s)
        for (uint32_t w = s * 16 + 1; w < std::min(words, s * 16 + 16); ++w)
            out.push_back(uint8_t(innerBefore(uint64_t(w) * 16) - innerBefore(uint64_t(s) * 256)));
    out.resize(start + ((out.size() - start) + 3) / 4 * 4);
    return out;
}
inline unsigned bitsFor(uint32_t maxValue) {
    unsigned w = 1;
    while (w < 31 && (uint32_t(1) << w) <= maxValue) ++w;
    return w;
}
inline void putBits(std::vector<uint8_t>& d, uint64_t bit, unsigned width, uint32_t value) {
    for (unsigned b = 0; b < width; ++b)
        if (value & (uint32_t(1) << b)) d[size_t((bit + b) >> 3)] |= uint8_t(1u << ((bit + b) & 7));
}

}  // namespace detail

inline Result bake(const Input& in) {
    using namespace detail;
    Result res;
    const auto fail = [&](const std::string& why) {
        res.ok = false;
        res.error = why;
        res.tome.clear();
        return res;
    };
    const uint32_t objectCount = uint32_t(in.objects.size());
    if (!objectCount || objectCount > 0x01000000u) return fail("the tome needs 1 to 16777216 objects");
    double side = 0;
    for (int k = 0; k < 3; ++k) {
        if (!std::isfinite(in.scene.mn[k]) || !std::isfinite(in.scene.mx[k]) || !(in.scene.mn[k] < in.scene.mx[k])) return fail("the scene box is not valid");
        side = std::max(side, double(in.scene.mx[k]) - double(in.scene.mn[k]));
    }
    // The tile: a cube from the scene's minimum corner, 65536 quanta a side.
    float scale = float(side / double(kSide));
    if (double(scale) * kSide < side) scale = std::nextafter(scale, std::numeric_limits<float>::infinity());
    const float invScale = 1.0f / scale;
    if (!(scale > 0) || !std::isfinite(scale) || !std::isfinite(invScale)) return fail("the scene box cannot be quantized");
    const auto quantum = [&](float world, int axis, bool up) -> uint32_t {
        const double q = (double(world) - double(in.scene.mn[axis])) / double(scale);
        const double r = up ? std::ceil(q - 1e-6) : std::floor(q + 1e-6);
        return uint32_t(std::clamp(r, 0.0, double(kSide)));
    };
    // Cell boundaries lie on a grid of 2^gridShift quanta (about 4 units): the view tree splits at midpoints of the
    // cube, so it meets such a boundary exactly after a bounded number of splits.
    unsigned gridShift = 0;
    while (gridShift < 12 && double(scale) * double(1u << (gridShift + 1)) <= 4.0) ++gridShift;
    const uint32_t grid = 1u << gridShift;
    const auto snapUp = [&](uint32_t q) { return std::min<uint32_t>(kSide, (q + grid - 1) / grid * grid); };
    const auto snapDown = [&](uint32_t q) { return q / grid * grid; };

    // Occluders as boxes of quanta that lie inside the real box (low side rounded up, high side down), the big ones first.
    struct Occ {
        QBox q;
        double area;
    };
    std::vector<Occ> occ;
    res.stats.occludersGiven = uint32_t(in.occluders.size());
    for (const Box& b : in.occluders) {
        Occ o;
        bool good = true;
        double sides[3];
        for (int k = 0; k < 3; ++k) {
            if (!(b.mn[k] < b.mx[k]) || !std::isfinite(b.mn[k]) || !std::isfinite(b.mx[k])) good = false;
            o.q.lo[k] = snapUp(quantum(b.mn[k], k, true));
            o.q.hi[k] = snapDown(quantum(b.mx[k], k, false));
            if (good && o.q.lo[k] >= o.q.hi[k]) good = false;
            sides[k] = double(b.mx[k]) - double(b.mn[k]);
        }
        if (!good) continue;
        std::sort(sides, sides + 3);
        if (sides[1] < double(in.minOccluderSide)) continue;  // two long sides: a wall, a floor, a block
        o.area = sides[1] * sides[2];
        occ.push_back(o);
    }
    std::stable_sort(occ.begin(), occ.end(), [](const Occ& a, const Occ& b) { return a.area > b.area; });

    // Free space, cut along the occluders' faces; then the view tree, which maps a point to its cell. A leaf of that
    // tree must lie in one cell (stock tomes hold exactly one cell per leaf, research 110), so when the tree does not
    // fit its node budget the space is cut again with half the cells.
    struct Portal {
        uint32_t target;
        uint8_t face;
        uint16_t plane;
        uint16_t rect[2][2];
    };
    struct Node {
        QBox q;
        std::vector<uint32_t> touching;
    };
    // The scene box is the free space to start with. The rest of the cube is outside the map: no cell (a camera
    // there gets the nearest one), so it joins nothing.
    QBox whole{{0, 0, 0}, {kSide, kSide, kSide}};
    for (int k = 0; k < 3; ++k) whole.hi[k] = std::max<uint32_t>(grid, snapUp(quantum(in.scene.mx[k], k, true)));
    std::vector<QBox> cells, solid;
    std::vector<uint8_t> codes;
    std::vector<std::vector<uint32_t>> leafCells;
    const auto nearestCell = [&](const QBox& q) {
        uint32_t best = 0;
        double bestD = 1e300;
        for (uint32_t i = 0; i < uint32_t(cells.size()); ++i) {
            double d = 0;
            for (int k = 0; k < 3; ++k) {
                const double gap = q.lo[k] >= cells[i].hi[k] ? double(q.lo[k]) - cells[i].hi[k] + 1 : (cells[i].lo[k] >= q.hi[k] ? double(cells[i].lo[k]) - q.hi[k] + 1 : 0.0);
                d += gap * gap;
            }
            if (d < bestD) bestD = d, best = i;
        }
        return best;
    };
    uint32_t maxCells = std::clamp<uint32_t>(in.maxCells, 2u, 60000u);
    for (;;) {
        cells.assign(1, whole);
        solid.clear();
        res.stats.occludersUsed = 0;
        for (const Occ& o : occ) {
            size_t touched = 0;
            for (const QBox& c : cells) touched += overlaps(c, o.q) ? 1 : 0;
            if (!touched) continue;  // already inside other occluders
            if (cells.size() + touched * 6 > maxCells) continue;  // would make too many cells: this one hides nothing
            std::vector<QBox> next;
            next.reserve(cells.size() + touched * 6);
            for (const QBox& c : cells) {
                if (!overlaps(c, o.q)) {
                    next.push_back(c);
                    continue;
                }
                QBox rest = c;  // what is left of the cell; the slabs outside the occluder are cut off it, axis by axis
                for (int k = 0; k < 3; ++k) {
                    if (o.q.lo[k] > rest.lo[k]) {
                        QBox below = rest;
                        below.hi[k] = o.q.lo[k];
                        next.push_back(below);
                        rest.lo[k] = o.q.lo[k];
                    }
                    if (o.q.hi[k] < rest.hi[k]) {
                        QBox above = rest;
                        above.lo[k] = o.q.hi[k];
                        next.push_back(above);
                        rest.hi[k] = o.q.hi[k];
                    }
                }
                solid.push_back(rest);  // inside the occluder
            }
            cells.swap(next);
            ++res.stats.occludersUsed;
        }
        if (cells.empty()) return fail("the occluders fill the whole scene");

        // The view tree: the cube split at midpoints until every leaf lies in one cell, or in no cell (solid, or
        // outside the map), which gets the nearest cell. A node is split along an axis on which a boundary of a cell
        // it touches lies strictly inside it, the longest such axis first. Breadth first, so the k-th inner node's
        // children are nodes 2k + 1 and 2k + 2.
        codes.clear();
        leafCells.clear();
        bool fits = true;
        std::vector<Node> level;
        Node root;
        root.q = QBox{{0, 0, 0}, {kSide, kSide, kSide}};
        root.touching.resize(cells.size());
        for (uint32_t i = 0; i < uint32_t(cells.size()); ++i) root.touching[i] = i;
        level.push_back(std::move(root));
        size_t pending = 1;  // nodes not yet given a code
        while (!level.empty() && fits) {
            std::vector<Node> nextLevel;
            for (Node& n : level) {
                --pending;
                int axis = -1;
                for (int k = 0; k < 3; ++k) {
                    const uint32_t size = n.q.hi[k] - n.q.lo[k];
                    if (size < 2 || (axis >= 0 && size <= n.q.hi[axis] - n.q.lo[axis])) continue;
                    bool cut = false;
                    for (const uint32_t c : n.touching)
                        if ((cells[c].lo[k] > n.q.lo[k] && cells[c].lo[k] < n.q.hi[k]) || (cells[c].hi[k] > n.q.lo[k] && cells[c].hi[k] < n.q.hi[k])) {
                            cut = true;
                            break;
                        }
                    if (cut) axis = k;
                }
                if (axis < 0) {
                    codes.push_back(3);
                    if (n.touching.empty()) n.touching.push_back(nearestCell(n.q));
                    leafCells.push_back(std::move(n.touching));
                    continue;
                }
                if (codes.size() + pending + nextLevel.size() + 2 > in.maxViewNodes) {
                    fits = false;
                    break;
                }
                codes.push_back(uint8_t(axis));
                const uint32_t mid = n.q.lo[axis] + (n.q.hi[axis] - n.q.lo[axis]) / 2;
                Node lo, hi;
                lo.q = hi.q = n.q;
                lo.q.hi[axis] = mid;
                hi.q.lo[axis] = mid;
                for (const uint32_t c : n.touching) {
                    if (overlaps(cells[c], lo.q)) lo.touching.push_back(c);
                    if (overlaps(cells[c], hi.q)) hi.touching.push_back(c);
                }
                nextLevel.push_back(std::move(lo));
                nextLevel.push_back(std::move(hi));
            }
            pending += nextLevel.size();
            level.swap(nextLevel);
        }
        if (fits) break;
        if (cells.size() <= 1) return fail("the view tree does not fit even for one cell");
        maxCells = std::max<uint32_t>(2u, std::min<uint32_t>(maxCells, uint32_t(cells.size())) / 2);
        ++res.stats.retries;
    }
    const uint32_t cellCount = uint32_t(cells.size());
    res.stats.cells = cellCount;
    res.stats.solidBoxes = uint32_t(solid.size());

    // Portals: the shared rectangle of two cells that touch on a face. Found per plane.
    std::vector<std::vector<Portal>> portals(cellCount);
    for (int a = 0; a < 3; ++a) {
        const int b = (a + 1) % 3, c = (a + 2) % 3;
        std::unordered_map<uint32_t, std::vector<uint32_t>> endAt, startAt;  // plane -> cells
        for (uint32_t i = 0; i < cellCount; ++i) {
            endAt[cells[i].hi[a]].push_back(i);
            startAt[cells[i].lo[a]].push_back(i);
        }
        for (const auto& [plane, lows] : endAt) {
            const auto it = startAt.find(plane);
            if (it == startAt.end()) continue;
            for (const uint32_t lo : lows)
                for (const uint32_t hi : it->second) {
                    const uint32_t b0 = std::max(cells[lo].lo[b], cells[hi].lo[b]), b1 = std::min(cells[lo].hi[b], cells[hi].hi[b]);
                    const uint32_t c0 = std::max(cells[lo].lo[c], cells[hi].lo[c]), c1 = std::min(cells[lo].hi[c], cells[hi].hi[c]);
                    if (b0 >= b1 || c0 >= c1) continue;
                    Portal p;
                    p.plane = uint16_t(plane);
                    p.rect[0][0] = uint16_t(b0), p.rect[0][1] = uint16_t(b1 - 1);
                    p.rect[1][0] = uint16_t(c0), p.rect[1][1] = uint16_t(c1 - 1);
                    p.target = hi, p.face = uint8_t(a * 2 + 1);  // from the low cell towards +axis
                    portals[lo].push_back(p);
                    p.target = lo, p.face = uint8_t(a * 2);      // and back
                    portals[hi].push_back(p);
                }
        }
    }
    for (auto& list : portals)
        std::sort(list.begin(), list.end(), [](const Portal& x, const Portal& y) {
            return x.face != y.face ? x.face < y.face : (x.target != y.target ? x.target < y.target : x.plane < y.plane);
        });

    // Which cells list each object.
    std::vector<std::vector<uint32_t>> lists(cellCount);
    for (uint32_t oi = 0; oi < objectCount; ++oi) {
        const Object& o = in.objects[oi];
        bool valid = !o.everywhere;
        for (int k = 0; k < 3 && valid; ++k) valid = std::isfinite(o.box.mn[k]) && std::isfinite(o.box.mx[k]) && o.box.mn[k] <= o.box.mx[k];
        if (!valid) {
            for (auto& l : lists) l.push_back(oi);
            ++res.stats.objectsEverywhere;
            continue;
        }
        QBox q;
        for (int k = 0; k < 3; ++k) {
            q.lo[k] = quantum(o.box.mn[k] - in.objectMargin, k, false);
            q.hi[k] = std::max(quantum(o.box.mx[k] + in.objectMargin, k, true), q.lo[k] + 1);
            if (q.lo[k] > 0) --q.lo[k];  // a face on a cell's boundary belongs to both sides
            if (q.hi[k] < kSide) ++q.hi[k];
        }
        bool any = false;
        for (uint32_t i = 0; i < cellCount; ++i)
            if (overlaps(q, cells[i])) lists[i].push_back(oi), any = true;
        if (!any) {
            lists[nearestCell(q)].push_back(oi);
            ++res.stats.objectsByNearestCell;
        }
    }
    res.stats.viewNodes = uint32_t(codes.size());
    res.stats.viewLeaves = uint32_t(leafCells.size());

    // ---- The bytes: the one-cell tome's layout (replay_render.cpp BuildConservativeUmbraTome) with more cells. ----
    unsigned indexWidth = 1;
    while (indexWidth < 31 && (uint32_t(1) << indexWidth) < objectCount) ++indexWidth;
    uint64_t listEntries = 0;
    for (const auto& l : lists) listEntries += l.size();
    if (listEntries > 0x7FFFFFFFull) return fail("the cells' object lists are too long");
    res.stats.listEntries = uint32_t(listEntries);

    std::vector<uint8_t>& t = res.tome;
    t.assign(kHeaderSize, 0);
    float tileMax[3];
    for (int k = 0; k < 3; ++k) tileMax[k] = in.scene.mn[k] + float(kSide) * scale;
    put(t, 0x00, kVersion);
    put(t, 0x0C, 128.0f);
    put(t, 0x40, objectCount);
    put(t, 0x54, uint32_t(indexWidth));
    put(t, 0x5C, uint32_t(listEntries));
    put(t, 0x8C, uint32_t(1));
    put(t, 0x90, uint32_t(1));
    std::memcpy(t.data() + 0x14, in.scene.mn, 12);
    std::memcpy(t.data() + 0x20, tileMax, 12);
    std::memcpy(t.data() + 0x150, in.scene.mn, 12);
    std::memcpy(t.data() + 0x15C, in.scene.mx, 12);
    put(t, 0x168, scale);

    const uint32_t topTree = allocate16(t, 4);
    put(t, topTree, uint32_t(3));
    const uint32_t topMap = allocate16(t, 4);
    put(t, 0x2C, uint32_t(33));
    put(t, 0x30, topTree);
    put(t, 0x34, topMap);

    const uint32_t objBounds = allocate16(t, size_t(objectCount) * 24);
    const uint32_t objDistances = allocate16(t, size_t(objectCount) * 32);
    for (uint32_t o = 0; o < objectCount; ++o) {
        std::memcpy(t.data() + objBounds + size_t(o) * 24, in.scene.mn, 12);
        std::memcpy(t.data() + objBounds + size_t(o) * 24 + 12, in.scene.mx, 12);
        const size_t d = objDistances + size_t(o) * 32;
        std::memcpy(t.data() + d, in.scene.mn, 12);
        put(t, d + 12, 0.0f);
        std::memcpy(t.data() + d + 16, in.scene.mx, 12);
        put(t, d + 28, std::numeric_limits<float>::infinity());
    }
    put(t, 0x44, objBounds);
    put(t, 0x48, objDistances);
    const uint32_t userIds = allocate16(t, size_t(objectCount) * 4);
    for (uint32_t o = 0; o < objectCount; ++o) put(t, userIds + size_t(o) * 4, in.objects[o].userId);
    put(t, 0x50, userIds);

    const uint32_t objectLists = allocate16(t, size_t((listEntries * indexWidth + 31) / 32 * 4));
    std::vector<uint32_t> listStart(cellCount);
    {
        std::vector<uint8_t> bits(size_t((listEntries * indexWidth + 31) / 32 * 4), 0);
        uint64_t entry = 0;
        for (uint32_t c = 0; c < cellCount; ++c) {
            listStart[c] = uint32_t(entry);
            for (const uint32_t o : lists[c]) putBits(bits, entry++ * indexWidth, indexWidth, o);
        }
        if (!bits.empty()) std::memcpy(t.data() + objectLists, bits.data(), bits.size());
    }
    put(t, 0x58, objectLists);

    const uint32_t cellStarts = allocate16(t, 8);
    put(t, cellStarts, uint32_t(0));
    put(t, cellStarts + 4, cellCount);
    put(t, 0x88, cellStarts);
    const uint32_t lodLevels = allocate16(t, 4);
    put(t, lodLevels, 1.0f);
    put(t, 0x9C, lodLevels);
    const uint32_t tileOffsets = allocate16(t, 4);
    put(t, 0xA0, tileOffsets);
    const uint32_t portalExpands = allocate16(t, 4);
    put(t, portalExpands, 0.0f);
    put(t, 0x14C, portalExpands);

    const uint32_t tile = allocate16(t, kTileSize);
    std::memcpy(t.data() + tile, in.scene.mn, 12);
    std::memcpy(t.data() + tile + 12, tileMax, 12);
    put(t, tile + 0x30, 0.0f);
    put(t, tile + 0x34, cellCount);  // cells in bits 0-15 (clusters, bits 16-31: none)
    put(t, tile + 0x44, scale);
    put(t, tile + 0x48, invScale);
    for (int k = 0; k < 3; ++k) {
        put(t, tile + 0x4C + size_t(k) * 4, uint16_t(0));
        put(t, tile + 0x4C + size_t(k) * 4 + 2, uint16_t(kSide - 1));
    }
    // View tree, its map and the cell index lists (a list ends at 0xFFFFFFFF; equal lists are stored once).
    const std::vector<uint8_t> treeBytes = serializeTree(codes);
    const uint32_t viewTree = allocate16(t, treeBytes.size());
    std::memcpy(t.data() + viewTree, treeBytes.data(), treeBytes.size());
    std::vector<uint32_t> indexWords, leafValue(leafCells.size());
    {
        std::map<std::vector<uint32_t>, uint32_t> known;
        for (size_t l = 0; l < leafCells.size(); ++l) {
            std::vector<uint32_t>& list = leafCells[l];
            std::sort(list.begin(), list.end());
            const auto it = known.find(list);
            if (it != known.end()) {
                leafValue[l] = it->second;
                continue;
            }
            leafValue[l] = uint32_t(indexWords.size());
            known.emplace(list, leafValue[l]);
            indexWords.insert(indexWords.end(), list.begin(), list.end());
            indexWords.push_back(0xFFFFFFFFu);
        }
    }
    const unsigned mapWidth = bitsFor(indexWords.empty() ? 0u : uint32_t(indexWords.size() - 1));
    if (codes.size() >= (1u << 27)) return fail("the view tree is too large");
    const uint32_t viewMap = allocate16(t, size_t((uint64_t(leafCells.size()) * mapWidth + 31) / 32 * 4));
    {
        std::vector<uint8_t> bits(size_t((uint64_t(leafCells.size()) * mapWidth + 31) / 32 * 4), 0);
        for (size_t l = 0; l < leafCells.size(); ++l) putBits(bits, uint64_t(l) * mapWidth, mapWidth, leafValue[l]);
        std::memcpy(t.data() + viewMap, bits.data(), bits.size());
    }
    const uint32_t cellIndices = allocate16(t, indexWords.size() * 4);
    std::memcpy(t.data() + cellIndices, indexWords.data(), indexWords.size() * 4);
    put(t, tile + 0x18, uint32_t(uint32_t(codes.size()) << 5 | mapWidth));
    put(t, tile + 0x1C, viewTree - tile);
    put(t, tile + 0x20, viewMap - tile);
    put(t, tile + 0x40, cellIndices - tile);

    // Cells and portals.
    uint32_t portalTotal = 0;
    for (const auto& l : portals) portalTotal += uint32_t(l.size());
    res.stats.portals = portalTotal;
    const uint32_t cellsAt = allocate16(t, size_t(cellCount) * kCellSize);
    const uint32_t portalsAt = allocate16(t, size_t(portalTotal) * kPortalSize);
    uint32_t portalAt = 0;
    for (uint32_t c = 0; c < cellCount; ++c) {
        const size_t co = cellsAt + size_t(c) * kCellSize;
        put(t, co + 0x00, portalAt);
        put(t, co + 0x04, uint32_t(portals[c].size()));
        put(t, co + 0x08, listStart[c]);
        put(t, co + 0x0C, uint32_t(lists[c].size()));
        put(t, co + 0x10, uint32_t(0xFFFFFFFFu));  // no cluster
        put(t, co + 0x14, uint32_t(0x80000000u));
        const QBox& q = cells[c];
        put(t, co + 0x18, uint32_t(q.lo[0] << 16 | q.lo[1]));
        put(t, co + 0x1C, uint32_t(q.lo[2] << 16 | (q.hi[0] - 1)));
        put(t, co + 0x20, uint32_t((q.hi[1] - 1) << 16 | (q.hi[2] - 1)));
        for (const Portal& p : portals[c]) {
            const size_t po = portalsAt + size_t(portalAt++) * kPortalSize;
            put(t, po + 0, uint32_t(uint32_t(p.face) << 29 | 0x02000000u));           // a portal within the tile
            put(t, po + 4, uint32_t(p.target << 16 | p.plane));
            put(t, po + 8, uint32_t(uint32_t(p.rect[0][0]) << 16 | p.rect[0][1]));
            put(t, po + 12, uint32_t(uint32_t(p.rect[1][0]) << 16 | p.rect[1][1]));
        }
    }
    put(t, tile + 0x38, cellsAt - tile);
    put(t, tile + 0x3C, portalTotal ? portalsAt - tile : uint32_t(0));
    t.resize((t.size() + 15) & ~size_t(15));
    const uint64_t tileBytes = t.size() - tile;
    if (tileBytes >= (1u << 24)) return fail("the tile is larger than 16 MB: fewer cells or occluders are needed");
    // Flags 5, as every stock leaf tile with a view tree (shipment, rust, speedball): bit 2 tells the game that a view
    // leaf's value is an offset into the cell index list, which is what is written below. Without it the game takes
    // the value for the cell number itself (Umbra_KDTree_lookupLeafRange 0x1420079D0, the leaf reader 0x142007B60;
    // research 129): a tome written with 3 hid surfaces and models in the game test of 2026-11-14.
    put(t, tile + 0x2C, uint32_t(uint32_t(tileBytes) << 8 | 5u));
    put(t, tileOffsets, tile);
    put(t, 0x08, uint32_t(t.size()));
    put(t, 0x04, crc32c(t.data() + 8, t.size() - 8));
    res.ok = true;
    return res;
}

}  // namespace umbrabake
