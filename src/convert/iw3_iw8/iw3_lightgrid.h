#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <optional>
#include <string>
#include <vector>

namespace iw3
{
bool PrepareNativeLightGrid(const std::filesystem::path &sourceRoot,
                            const std::string &sourceAsset,
                            const std::filesystem::path &output, float lightUnitScale = 1.0f);

// IW3 lights a static model from its light grid at the model's lighting origin.
// For each point: the grid trilinearly interpolated (missing cells skipped, else
// the nearest cell within 8 steps), as an ambient cube of shading values in IW3
// light-grid units (the units the world lightmaps use) for +X,-X,+Y,-Y,+Z,-Z.
// Empty when the map has no exported light grid.
// With sunVisibility, also each point's IW3 sun visibility: the interpolated
// share of grid cells whose primary light is the sun (IW3 lit a static model
// by the sun only where its grid point's primary light was the sun).
std::vector<std::array<std::array<float, 3>, 6>> SampleLightGridAmbientCubes(
    const std::filesystem::path &sourceRoot, const std::string &sourceAsset,
    const std::vector<std::array<float, 3>> &points, unsigned sunPrimaryLight = 0,
    std::vector<float> *sunVisibility = nullptr);

// IW3 R_SetStaticModelLighting, ported: each static model's 4x4x4 model-lighting
// volume as IW3 uploads it (R_SetLightGridColors: x fastest, then y, then z; the
// outer shell holds the 56 grid directions, the inner 2x2x2 core copies corner
// samples), RGBA8 with A = the primary-light (sun) weight. Ground-lit models
// (XModel flags bit 0 with a non-zero groundLighting) fill every texel with that
// colour (bytes B, G, R, A as IW3 stores it); the rest blend the grid at the
// lighting origin with R_GetLightingAtPoint's corner weights, trace suppression,
// fixed-point colour blend and extrapolation. Empty when the map has no grid.
struct StaticModelLightingInput
{
    std::array<float, 3> origin{}; // avg(mins, maxs)
    std::uint8_t primaryLight{};   // GfxStaticModelDrawInst::primaryLightIndex
    bool groundLit{};
    std::uint32_t groundLighting{}; // GfxColor packed
    // The model's own 56 light-grid colours (R, G, B each, in the order of a
    // light-grid palette entry), from an extraction that lights each model
    // itself (IW7: its GPU light grid at the model); the sun weight stays
    // groundLighting's alpha.
    std::optional<std::array<std::uint8_t, 168>> colors;
};
struct LightGridPrimaryLight
{
    unsigned type{}; // 1 dir, 2 spot, 3 omni
    std::array<float, 3> origin{}, dir{};
    float radius{}, cosHalfFovOuter{}, rotationLimit{1};
};
// CM_BoxSightTrace(start, end, contents 0x2001) != 0.
using LightGridSightBlocked =
    std::function<bool(const std::array<float, 3> &start, const std::array<float, 3> &end)>;
std::vector<std::array<std::uint8_t, 256>> SampleStaticModelLighting(
    const std::filesystem::path &sourceRoot, const std::string &sourceAsset,
    const std::vector<StaticModelLightingInput> &inputs,
    const std::vector<LightGridPrimaryLight> &primaryLights, const LightGridSightBlocked &blocked);

// The IW3 light grid for entity (DObj) shading, as IW3's R_GetLightingAtPoint
// reads it: one RGBA8 image of `width` texels per row holding first the grid
// entries over [minimum, minimum + dims) (texel x + dims.x * (y + dims.y * z):
// R + 256 G = palette colour + 1, 0 = no entry; B = primary light), then from
// texel `paletteStart` the palette's 56 colours per entry (RGB). The entity
// pixel shader blends the 8 corners at the model's lighting origin and samples
// the 4x4x4 model-lighting volume by normal, as the lit model shaders do.
// With `sh` (extractions that keep their own light grid at each entry, IW7's
// <grid>.sh.bin) an entry texel holds R + 256 G + 65536 B = record, 0 = no
// entry, and from paletteStart each record is 14 texels of 28 halves (the 27
// SH coefficients of red, green and blue in EvaluateShBasis, then a pad), or
// 8 texels (27 signed bytes in 127ths of the largest coefficient, a pad byte,
// that largest as a half); record 0 is the fallback (the mean of the entries),
// then one record per entry with SH, in entry order (IW7 leaves the entries
// outside its grid mesh NaN: no entry). With `blocks`, the entry texels are instead a table of
// 8x8x8-point blocks (texel bx + blocks.x * (by + blocks.y * bz): R + 256 G +
// 65536 B = page, 0 = no entry) and from `pageStart` 512 texels per page
// (x + 8 * (y + 8 * z) within the block).
struct EntityLightGrid
{
    std::array<int, 3> minimum{}; // grid coordinates (32, 32, 64 units from -131072)
    std::array<unsigned, 3> dims{};
    unsigned width{};
    unsigned height{};
    unsigned paletteStart{};
    std::size_t paletteColors{}; // 56 texels each from paletteStart (records with sh)
    bool sh{};
    unsigned recordTexels{}; // with sh: 14 or 8
    std::array<unsigned, 3> blocks{}; // zero: the dense box
    unsigned pageStart{};
    std::vector<std::uint8_t> pixels;
};
std::optional<EntityLightGrid> BuildEntityLightGrid(const std::filesystem::path &sourceRoot,
                                                    const std::string &sourceAsset);
}
