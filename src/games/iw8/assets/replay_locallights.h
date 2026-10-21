#pragma once

#include "../zone/iw8_zone.h"
#include "../common/replay_bounds.h"
#include "../common/replay_render.h"

#include <array>
#include <cstdint>
#include <vector>

namespace iw8::locallights
{
// One ComWorld primary light as the converter emits it (index = ComWorld slot).
struct Light
{
    uint8_t type = 0; // 0 null, 1 sun, 2 spot, 3 omni
    bool canUseShadowMap = false;
    std::array<float, 3> origin{};
    std::array<float, 3> direction{}; // emission axis (= -ComPrimaryLight dir)
    float radius = 0;
    float cosHalfFovOuter = 0;
    // ComPrimaryLight bulb radius (+0x5C) and the length of its tube (bulbLength), which widen what it lights.
    float bulbRadius = 1;
    float tubeLength = 0;
    // A shadow-mapped spot whose shadows hold entities (IW7 sets it per light; IW3's all do).
    bool dynamicShadows = true;
};

// ComPrimaryLight +3 needsDynamicShadows. Shipped mp_hideout sets it on exactly
// the shadow-mapped spots that shadow entities (37 of its 65 shadow-mapped
// lights), and lightAABB lists exactly those lights.
inline bool NeedsDynamicShadows(const Light &light)
{
    return light.type == 2 && light.canUseShadowMap && light.dynamicShadows;
}

// GfxWorld local-light structures in the layout of shipped Replay worlds:
//  - frustumLights (+0x3CA0): per-light convex volume mesh, as shipped worlds
//    carry. The light binning (0x186C4F0 -> 0x186F0A0) takes a static light's
//    view-depth range and binning draws from it only without the per-client
//    runtime volumes (0x186D2E0, live on Frontier 2026-10-21), which it builds
//    from the light records each frame instead (0x186EAE0 spots, 0x186DE10 omnis).
//  - lightViewFrustums (+0x3D60): 6-plane hexahedron per shadow-mapped spot
//    (R_PrimaryLightFrustumTestSphere/Box, entity linking).
//  - lightAABB (+0x3F80): BVH over needsDynamicShadows lights
//    (R_Link*ToPrimaryLights); node 0 is the root, firstChild bit 15 = leaf.
//  - shadowGeomOptimized (+0x3F78): per-light static casters (raw surface
//    indices, remapped by 0x19296B0; static-model instance indices).
//  - shadow/motion bitset sizes (+0x3F10..+0x3F68), zero-filled in stream 4.
struct Data
{
    uint32_t lightCount = 0;
    uint32_t aligned = 0; // staticSpotOmniPrimaryLightCountAligned / shadowBitsArrayPitch
    std::vector<uint32_t> umbraLights;

    std::vector<uint32_t> indexOffsets, vertexOffsets;
    std::vector<uint16_t> indexCounts, vertexCounts, indices;
    std::vector<std::array<float, 4>> vertices;

    struct View
    {
        std::vector<std::array<float, 4>> planes;
        std::vector<uint8_t> indices;
        std::vector<std::array<float, 3>> vertices;
    };
    std::vector<View> views;

    struct Node
    {
        replaybounds::Bounds bounds{};
        uint16_t firstChild = 0, childCount = 0;
    };
    std::vector<Node> nodes;
    std::vector<uint16_t> aabbLights;

    struct Shadow
    {
        std::vector<uint32_t> surfaces;
        std::vector<uint16_t> models;
    };
    std::vector<Shadow> shadows;

    bool Empty() const
    {
        return umbraLights.empty();
    }
};

Data Build(const std::vector<Light> &lights, uint32_t lastSunPrimaryLightIndex,
           const replayrender::Mesh &mesh, const replayrender::StaticModels &staticModels);
// Stamps the GfxWorld fields; dynamic counts size the dynent shadow bitsets.
void Stamp(std::vector<uint8_t> &world, const Data &data, uint32_t dynamicModelCount,
           uint32_t dynamicBrushCount);
// Stream-4 bytes for the zero-filled bitsets Stamp declares.
size_t ZeroFillBytes(const Data &data, uint32_t dynamicModelCount, uint32_t dynamicBrushCount);
// Load_GfxWorld order: frustumLights after GfxWorldDraw, then lightViewFrustums
// (before the heightfields); shadowGeomOptimized then lightAABB before the DPVS.
void EmitFrustumLights(ZoneWriter &writer, const Data &data);
void EmitViewFrustums(ZoneWriter &writer, const Data &data);
void EmitShadowGeometry(ZoneWriter &writer, const Data &data);
void EmitLightAABB(ZoneWriter &writer, const Data &data);
} // namespace iw8::locallights
