#include <std_include.hpp>
#include "replay_stock_library.h"
#include "../iw8.hpp"
#include "../zone/loader.hpp"

#include <bit>
#include <cstring>
#include <stdexcept>
#include <unordered_map>

namespace iw8::stock
{
namespace
{
namespace replay = ::zonetool::iw8;
using Json = nlohmann::json;

// The record sizes of the converter's JSON (games/iw8/common/replay_render.cpp reads them back).
static_assert(sizeof(replay::MaterialTechniqueSet) == 64);
static_assert(sizeof(replay::MaterialTechnique) == 184);
static_assert(sizeof(replay::MaterialPipelineState) == 16);
static_assert(sizeof(replay::ComputeShader) == 40);
static_assert(sizeof(replay::Replay_MaterialTechniqueStateBits) == 5);
static_assert(sizeof(replay::MaterialShaderArgument) == 6);
static_assert(sizeof(replay::MaterialConstantDef) == 20);

std::string Hex(const void *data, const std::size_t size)
{
    static constexpr char digits[] = "0123456789abcdef";
    const auto *bytes = static_cast<const std::uint8_t *>(data);
    std::string text(size * 2, '0');
    for (std::size_t i = 0; i < size; ++i)
    {
        text[2 * i] = digits[bytes[i] >> 4];
        text[2 * i + 1] = digits[bytes[i] & 15];
    }
    return text;
}

template <typename T> void Zero(std::uint8_t *record, const T &field, const void *base)
{
    std::memset(record + (reinterpret_cast<const std::uint8_t *>(&field) - static_cast<const std::uint8_t *>(base)), 0,
                sizeof(T));
}
} // namespace

struct Library::Impl
{
    std::unique_ptr<replay::loaded_zone> techsets;
    std::unique_ptr<replay::loaded_zone> shared;
    std::unordered_map<std::string, const replay::MaterialTechniqueSet *> techniqueSets;
    std::unordered_map<std::string, const replay::Material *> materials;
    // Shaders with their programs, by type and name: the map zone's own, then code_post_gfx's shared ones.
    std::unordered_map<std::string, const replay::ComputeShader *> shaders;

    void Index(const replay::loaded_zone &zone)
    {
        for (const auto &asset : zone.assets)
        {
            const char *name = replay::get_asset_name(asset.type, asset.header);
            if (!name || !*name || *name == ',')
                continue;
            switch (asset.type)
            {
            case replay::ASSET_TYPE_TECHNIQUE_SET:
                techniqueSets.try_emplace(name, static_cast<const replay::MaterialTechniqueSet *>(asset.header));
                break;
            case replay::ASSET_TYPE_MATERIAL:
                materials.try_emplace(name, static_cast<const replay::Material *>(asset.header));
                break;
            case replay::ASSET_TYPE_VERTEXSHADER:
            case replay::ASSET_TYPE_HULLSHADER:
            case replay::ASSET_TYPE_DOMAINSHADER:
            case replay::ASSET_TYPE_PIXELSHADER:
            {
                // A shader of the map zone that names a shared one has no program: the shared one's replaces it. Some
                // shaders hold none (null.hlsl).
                const auto *shader = static_cast<const replay::ComputeShader *>(asset.header);
                auto [entry, inserted] = shaders.try_emplace(std::to_string(asset.type) + ":" + name, shader);
                if (!inserted && !entry->second->prog.loadDef.programSize && shader->prog.loadDef.programSize)
                    entry->second = shader;
                break;
            }
            default:
                break;
            }
        }
    }
};

Library::Library(std::filesystem::path zoneFolder, std::string map) : impl_(std::make_unique<Impl>())
{
    const auto techsetsZone = zoneFolder / ("techsets_" + map + ".ff");
    const auto sharedZone = zoneFolder / "code_post_gfx.ff";
    if (!std::filesystem::exists(techsetsZone) || !std::filesystem::exists(sharedZone))
        throw std::runtime_error("stock Replay technique sets need " + techsetsZone.string() + " and " +
                                 sharedZone.string());
    impl_->techsets = replay::load_zone(techsetsZone, {.keep_temp_data = true});
    impl_->shared = replay::load_zone(sharedZone, {.keep_temp_data = true});
    impl_->Index(*impl_->techsets);
    impl_->Index(*impl_->shared);
}

Library::~Library() = default;

Json Library::TechniqueSet(const std::string &name) const
{
    const auto found = impl_->techniqueSets.find(name);
    if (found == impl_->techniqueSets.end())
        throw std::runtime_error("no stock Replay technique set " + name);
    const auto &set = *found->second;
    std::uint8_t header[sizeof(replay::MaterialTechniqueSet)];
    std::memcpy(header, &set, sizeof(header));
    Zero(header, set.name, &set);
    Zero(header, set.maskedTechniques, &set);
    std::size_t count = 0;
    for (const auto word : set.techniqueMask.mask)
        count += std::popcount(word);

    Json shaders = Json::object();
    Json techniques = Json::array();
    for (std::size_t index = 0; index < count; ++index)
    {
        const auto &technique = *set.maskedTechniques[index];
        std::uint8_t record[sizeof(replay::MaterialTechnique)];
        std::memcpy(record, &technique, sizeof(record));
        Zero(record, technique.name, &technique);
        Zero(record, technique.pipelineStates, &technique);
        Zero(record, technique.rootParameterMap, &technique);
        Zero(record, technique.vertexShader, &technique);
        Zero(record, technique.hullShader, &technique);
        Zero(record, technique.domainShader, &technique);
        Zero(record, technique.pixelShader, &technique);
        Zero(record, technique.stateBits, &technique);
        Zero(record, technique.args, &technique);

        // The pipeline states without their PSO handles.
        const std::size_t states = std::size_t{technique.pipelineStateCount} * technique.pipelineStateInputLayoutCount;
        std::vector<replay::MaterialPipelineState> pipelineStates(technique.pipelineStates,
                                                                  technique.pipelineStates + states);
        for (auto &state : pipelineStates)
            state.pso = nullptr;

        Json stages = Json::array();
        const std::pair<int, const replay::ComputeShader *> shaderStages[]{
            {replay::ASSET_TYPE_VERTEXSHADER, technique.vertexShader},
            {replay::ASSET_TYPE_HULLSHADER, technique.hullShader},
            {replay::ASSET_TYPE_DOMAINSHADER, technique.domainShader},
            {replay::ASSET_TYPE_PIXELSHADER, technique.pixelShader}};
        for (const auto &[type, shader] : shaderStages)
        {
            if (!shader || !shader->name)
            {
                stages.push_back(nullptr);
                continue;
            }
            std::string shaderName = shader->name;
            if (!shaderName.empty() && shaderName.front() == ',')
                shaderName.erase(0, 1);
            const std::string key = std::to_string(type) + ":" + shaderName;
            const auto program = impl_->shaders.find(key);
            if (program == impl_->shaders.end())
                throw std::runtime_error("no stock Replay shader " + key);
            if (!shaders.contains(key))
            {
                const auto &source = *program->second;
                std::uint8_t shaderHeader[sizeof(replay::ComputeShader)]{};
                const std::uint32_t length = source.prog.loadDef.programSize;
                std::memcpy(shaderHeader + 32, &length, sizeof(length));
                std::string debugName = source.debugName ? source.debugName : "";
                if (!debugName.empty() && debugName.front() == ',')
                    debugName.erase(0, 1);
                shaders[key] = {{"type", type},
                                {"name", shaderName},
                                {"debugName", debugName},
                                {"header", Hex(shaderHeader, sizeof(shaderHeader))},
                                {"program", Hex(source.prog.loadDef.program, source.prog.loadDef.program ? length : 0)}};
            }
            stages.push_back(shaderName);
        }

        const std::size_t argCount = std::size_t{technique.perPrimArgCount} + technique.perObjArgCount +
                                     technique.stableArgCount + technique.neverArgCount;
        techniques.push_back(
            {{"name", technique.name ? technique.name : ""},
             {"header", Hex(record, sizeof(record))},
             {"shaders", std::move(stages)},
             {"states", Hex(pipelineStates.data(), pipelineStates.size() * sizeof(replay::MaterialPipelineState))},
             {"rootsig", Hex(technique.rootParameterMap, 16)},
             {"statebits", Hex(technique.stateBits, technique.pipelineStateCount * sizeof(*technique.stateBits))},
             {"args", Hex(technique.args, argCount * sizeof(*technique.args))}});
    }
    return {{"schema", 1},
            {"name", name},
            {"header", Hex(header, sizeof(header))},
            {"techniques", std::move(techniques)},
            {"shaders", std::move(shaders)},
            {"source", "Replay stock technique set " + name + " (read from the game install); pointers removed."}};
}

Json Library::Material(const std::string &name) const
{
    const auto found = impl_->materials.find(name);
    if (found == impl_->materials.end())
        throw std::runtime_error("no stock Replay material " + name);
    const auto &material = *found->second;
    const auto *bytes = reinterpret_cast<const std::uint8_t *>(&material);
    Json textures = Json::array();
    for (std::size_t i = 0; i < material.textureCount; ++i)
    {
        std::uint8_t textureHeader[8]{};
        textureHeader[0] = material.textureTable[i].index;
        const char *image = material.textureTable[i].image
                                ? replay::get_asset_name(replay::ASSET_TYPE_IMAGE, material.textureTable[i].image)
                                : nullptr;
        std::string imageName = image ? image : "";
        if (!imageName.empty() && imageName.front() == ',')
            imageName.erase(0, 1);
        textures.push_back({{"header", Hex(textureHeader, sizeof(textureHeader))}, {"image", imageName}});
    }
    Json buffers = Json::array();
    for (std::size_t i = 0; i < material.constantBufferCount; ++i)
    {
        const auto &buffer = material.constantBufferTable[i];
        buffers.push_back({Hex(buffer.vsData, buffer.vsData ? buffer.vsDataSize : 0),
                           Hex(buffer.hsData, buffer.hsData ? buffer.hsDataSize : 0),
                           Hex(buffer.dsData, buffer.dsData ? buffer.dsDataSize : 0),
                           Hex(buffer.psData, buffer.psData ? buffer.psDataSize : 0)});
    }
    std::string techset = material.techniqueSet
                              ? replay::get_asset_name(replay::ASSET_TYPE_TECHNIQUE_SET, material.techniqueSet)
                              : "";
    if (!techset.empty() && techset.front() == ',')
        techset.erase(0, 1);
    Json result = {{"schema", 1},
                   {"source", name},
                   {"info", Hex(bytes + 8, 32)},
                   {"techset", techset},
                   {"textures", std::move(textures)},
                   {"constants", Hex(material.constantTable,
                                     material.constantTable ? material.constantCount * sizeof(*material.constantTable) : 0)},
                   {"bufferIndices", Hex(material.constantBufferIndex, material.constantBufferIndex ? 195 : 0)},
                   {"buffers", std::move(buffers)}};
    if (material.layerCount && material.subMaterials)
    {
        Json layers = Json::array();
        for (std::size_t i = 0; i < material.layerCount; ++i)
            layers.push_back(material.subMaterials[i] ? material.subMaterials[i] : "");
        result["subMaterials"] = std::move(layers);
    }
    return result;
}
} // namespace iw8::stock
