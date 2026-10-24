#pragma once

#include <iw8/common/replay_sunshadow.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <string>
#include <set>
#include <unordered_map>
#include <vector>

namespace iw3
{
// Material flag bit 21: a two-sided surface lights its back faces with flipped normals, as IW7's lit pixel shaders
// do (SV_IsFrontFace; the disassembled foliage shader negates N, T and B). IW3 materials never set it. Model
// vertices carry it as attribute bit 24 clear (their metadata word keeps only flag bits 0-17).
constexpr unsigned kBackFaceNormalsFlag = 1u << 21;
// IW7 layers that blend by their reveal map (layer token v<n>): the colour's alpha holds the reveal, the vertex colour
// the layer's weight (alpha) and sharpness (red = sqrt(sharpness / 2)); the world shader's MAP_REVEAL_LAYERS ramp makes
// the alpha.
constexpr unsigned kRevealLayerFlag = 1u << 22;

enum class SurfaceKind : unsigned
{
    opaque,
    sky,
    glass,
    cutout,
    skipped,
};

struct MaterialPlan
{
    std::size_t tile{};
    SurfaceKind kind{SurfaceKind::opaque};
    bool castsShadow{true};
    unsigned flags{};
    // IW3 cull state: 0 none, 1 back, 2 front.
    unsigned cullMode{1};
    // IW3 polygonOffset or a decal sort (decals layered on coplanar geometry).
    bool depthBias{};
    // Blended layer: 0 alpha, 1 additive, 2 multiply, 3 srcalpha additive.
    unsigned blend{};
    unsigned sourceSurfaceTypeBits{};
    std::array<float, 4> environment{0.8f, 4.0f, 2.5f, 0.625f};
    // Model falloff layers: row of RenderPlan::modelFalloffDefines (0 = the
    // stock ramp), packed into bits 10-15 of the model vertex metadata.
    unsigned falloffIndex{};
    // IW7 reactive foliage: its static-model surfaces bend by Replay's reactive-motion sim (XSurface flag 4).
    bool reactiveMotion{};
    // The material's native world material (a Replay material of its own: additionalMaterials index + 1), 0 for
    // none (its surfaces draw with their world variant's atlas material).
    unsigned worldMaterialIndex{};
    // Its stock Replay world material (MW120R_STOCK_WORLD: Replay's own world technique set and images,
    // additionalMaterials index + 1), 0 for none; its lightmapped surfaces draw with it.
    unsigned stockWorldMaterialIndex{};
    // The layer count of its stock layered material (0: a single-layer one): its surfaces carry each layer's UV and
    // tangent transform.
    unsigned stockLayers{};
    // World surfaces with an IW3 detail map: row of RenderPlan::worldDetails
    // (a "_detail<row>" world material variant), -1 for none.
    int worldDetail{-1};
    // IW3 wc_water world surfaces: row of RenderPlan::worldWaters (a
    // "_water<row>" world material variant), -1 for none.
    int worldWater{-1};
    std::string modelMaterial;
    // DObj-path (script, dynamic and effect XModel) material.
    std::string entityMaterial;
    std::string glassMaterial;
};

// World cutout surfaces share generated atlas materials. IW3 double-sided
// lattices are two coplanar faces, so back-face culling must survive per
// material, as must decal depth bias.
inline std::string CutoutVariantKind(const MaterialPlan &material)
{
    return std::string("foliage") + (material.cullMode == 1 ? "_back" : "") +
           (material.depthBias ? "_bias" : "");
}

// World blended surfaces share generated atlas materials per blend, detail row
// (_d<row>: IW3 wc_l_sm_b0c0d0 layers such as decal cracks) and depth bias.
inline std::string GlassVariantKind(const MaterialPlan &material)
{
    return std::string("glass") + (material.blend ? "_b" + std::to_string(material.blend) : "") +
           (material.worldDetail >= 0 ? "_d" + std::to_string(material.worldDetail) : "") +
           (material.depthBias ? "_bias" : "");
}

// The world material variant that draws a surface material ("" for the world material itself): its water or
// detail row, its blended layer, depth-biased opaque surfaces, its cut-out kind or the sky.
inline std::string WorldVariantKind(const MaterialPlan &material)
{
    if (material.worldWater >= 0)
        return "water" + std::to_string(material.worldWater);
    if (material.kind == SurfaceKind::glass)
        return GlassVariantKind(material);
    if (material.worldDetail >= 0)
        return "detail" + std::to_string(material.worldDetail);
    if (material.kind == SurfaceKind::opaque)
        return material.depthBias ? "opaque_bias" : "";
    if (material.kind == SurfaceKind::cutout)
        return CutoutVariantKind(material);
    return "sky";
}

// Replay GfxBlendStateBits: srcRgb | dstRgb << 4 | opRgb << 8 | srcAlpha << 11 |
// dstAlpha << 15 | opAlpha << 19 (source ONE = 0, ZERO = 1, SRCCOLOR = 2,
// SRCALPHA = 4, INVSRCALPHA = 5; destination ZERO = 0, ONE = 1, SRCCOLOR = 2).
inline std::uint64_t GlassBlendBits(const unsigned blend)
{
    switch (blend)
    {
    case 1: return 0x8010;  // one / one
    case 2: return 0x8821;  // zero / srccolor (alpha zero / one)
    case 3: return 0x8010;  // srcalpha / one: one / one, the shader applies alpha
    default: return 0x28050; // srcalpha / invsrcalpha (alpha one / invsrcalpha)
    }
}

struct LightmapRectangle
{
    unsigned x{};
    unsigned y{};
    unsigned width{};
    unsigned height{};
};

struct ReflectionProbePlan
{
    std::size_t sourceIndex{};
    std::array<float, 3> origin{};
    nlohmann::json image;
    std::array<std::array<float, 9>, 4> sh{};
};

struct NativeLightmapPlan
{
    unsigned width{};
    unsigned height{};
    std::array<unsigned, 3> formats{39u, 32u, 40u};
    std::array<std::string, 3> files;
};

// The light grid of entity (DObj) materials: an RGBA8 image (iw3_lightgrid.h
// EntityLightGrid) bound as the fourth texture, and the pixel shader defines
// that locate it (MAP_ENTITY_GRID_*).
struct EntityLightVolume
{
    std::string image;
    std::string file;
    unsigned width{};
    unsigned height{};
    std::string defines;
};

struct RenderPlan
{
    std::unordered_map<std::string, MaterialPlan> materials;
    std::unordered_map<std::string, std::string> fxMaterialAliases;
    // Replay Particle_MaterialData (ParticleModuleInitMaterial +0x20, 192 bytes)
    // for IW3 FX materials with feather, falloff or eye-offset terms.
    std::unordered_map<std::string, std::array<std::uint8_t, 192>> fxParticleMaterialData;
    // IW3 distortion materials. Their elements draw with a stock Replay
    // distortion material (common_mp, always loaded in MP) and the stock
    // distortion particle state.
    std::set<std::string> fxDistortionMaterials;
    // Stock Replay materials the map references by name (",<name>").
    std::set<std::string> stockMaterials;
    // Per-instance IW3 light-grid lighting for static placements (6 RGBA8
    // texels per instance, 256 instances per row; written by PrepareFastfile).
    std::string instanceLightingImage;
    std::string instanceLightingFile;
    // Baked world-space sun depth (replaysunshadow::BakeDepthImage), sampled by
    // the world, static-model and entity shaders; written by PrepareFastfile.
    std::string sunDepthImage;
    std::string sunDepthFile;
    unsigned instanceLightingCount{};
    std::vector<LightmapRectangle> lightmaps;
    std::array<std::size_t, 6> skyTiles{};
    // The native sky faces' materials (additionalMaterials index + 1), 0 when the sky draws from the atlas.
    std::array<unsigned, 6> skyMaterials{};
    // IW3 world detail maps (wc_*d0*): the detail image's own colour-atlas tile
    // and detailScale.xy, one world technique variant each.
    struct WorldDetail
    {
        std::size_t tile{};
        std::array<float, 2> scale{1, 1};
    };
    std::vector<WorldDetail> worldDetails;
    // IW3 wc_water: the baked wave-map flipbook (material slot 0x3D, t10) and
    // the water_l_sun constants, one world technique variant each.
    struct WorldWater
    {
        std::string image;
        std::string file;
        std::array<float, 4> environment{};
        std::array<float, 3> color{};
    };
    std::vector<WorldWater> worldWaters;
    unsigned columns{};
    // Pixel size of the square source atlas (ATLAS_SIZE in the shaders).
    unsigned atlasSize{4096};
    bool hasCutout{};
    bool hasCutoutBias{};
    bool hasOpaqueBias{};
    bool hasGlass{};
    std::string material;
    std::string materialDefinition;
    std::vector<nlohmann::json> additionalMaterials;
    std::vector<nlohmann::json> assetMaterials;
    std::vector<ReflectionProbePlan> reflectionProbes;
    nlohmann::json reflectionProbeArrayImage;
    // HLSL define: map-average reflection SH evaluated by translucent glass.
    std::string glassEnvironment;
    // HLSL defines for the model vertex shaders: the per-material IW3 falloff
    // parameters, near fade and eye offset (MaterialPlan::falloffIndex rows).
    std::string modelFalloffDefines;
    std::vector<NativeLightmapPlan> nativeLightmaps;
    // The region of the lightmap rectangles that the native lightmap covers (a single lightmap's own rectangle), in
    // which stock world surfaces keep their lightmap UVs.
    LightmapRectangle nativeLightmapRegion;
    replaysunshadow::Scene shadowScene;
    std::unordered_map<std::string, unsigned> worldShadowMaterials;
    std::unordered_map<std::string, unsigned> modelShadowMaterials;
};

// Block compresses the world and model images when `mode` is 1, keeps them RGBA8 when 0, and follows the extraction
// ("bc7_images", set by IW7 maps) when negative.
void UseBc7Images(int mode);

RenderPlan PrepareRenderAssets(const std::filesystem::path &exportRoot,
                               const nlohmann::json &world,
                               const std::vector<std::string> &surfaceMaterials,
                               const std::vector<std::string> &worldMaterials,
                               const std::vector<std::string> &modelMaterials,
                               const std::vector<std::string> &entityMaterials,
                               const std::vector<std::string> &fxMaterials,
                               const std::vector<std::string> &shadowWorldMaterials,
                               const std::vector<std::string> &shadowModelMaterials,
                               const std::vector<std::filesystem::path> &sourcePaths,
                               const std::filesystem::path &mapDirectory,
                               const std::string &map,
                               const std::filesystem::path &cacheRoot = {},
                               const std::string &surfaceShaderDefines = {},
                               unsigned staticModelInstances = 0,
                               const EntityLightVolume *entityLightVolume = nullptr,
                               float lightUnitScale = 1.0f, bool iw3Effects = false,
                               const std::filesystem::path &replayZones = {});

// The map's IW3 loading-screen image (images/loadscreen_<map>, from its _load
// zone or IWD) as a custom-map package preview: "MWRI", version 1, width,
// height, then top-down RGBA8, at most 2048 on each side. Empty when the map
// has no loading-screen image.
std::vector<std::uint8_t> BuildLoadscreenPreview(const std::filesystem::path &exportRoot,
                                                 const std::vector<std::filesystem::path> &sourcePaths,
                                                 const std::string &sourceMap);

} // namespace iw3
