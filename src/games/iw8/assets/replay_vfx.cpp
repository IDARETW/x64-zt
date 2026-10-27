#include <std_include.hpp>
#include "replay_vfx.h"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <stdexcept>
#include <utility>

namespace iw8::vfx
{
namespace
{
template <typename T> int32_t Count(const std::vector<T> &items)
{
    if (items.size() > static_cast<std::size_t>(std::numeric_limits<int32_t>::max()))
        throw std::runtime_error("Replay VFX array exceeds signed count range");
    return static_cast<int32_t>(items.size());
}

void StorePointer(void *record, const std::size_t offset, const uint64_t value)
{
    std::memcpy(static_cast<uint8_t *>(record) + offset, &value, sizeof(value));
}

template <std::size_t N>
void SetCurves(iw8_focus::ParticleCurveDef (&curves)[N], const std::vector<Module::Curve> &values)
{
    if (values.size() > N)
        throw std::runtime_error("Replay VFX module has too many curves");
    for (std::size_t index = 0; index < N; ++index)
    {
        curves[index].numControlPoints = index < values.size() ? Count(values[index].points) : 0;
        StorePointer(&curves[index], 0,
                     curves[index].numControlPoints ? PTR_FOLLOWS : PTR_NULL);
    }
}

void WriteCurves(ZoneWriter &writer, const std::vector<Module::Curve> &curves)
{
    for (const auto &curve : curves)
    {
        if (curve.points.empty())
            continue;
        writer.align(15);
        writer.write(curve.points.data(), curve.points.size() *
                                            sizeof(iw8_focus::ParticleCurveControlPointDef));
    }
}

void WriteModules(ZoneWriter &writer, const std::vector<Module> &modules)
{
    if (modules.empty())
        return;
    writer.align(15);
    for (const auto &module : modules)
    {
        auto record = module.native;
        if (module.imported)
        {
            constexpr auto meshSelector =
                static_cast<uint16_t>(iw8_focus::ParticleModuleType::initSpawnShapeMesh);
            constexpr auto payloadOffset = offsetof(iw8_focus::ParticleModuleDef, moduleData);
            constexpr auto meshPointer = payloadOffset +
                offsetof(iw8_focus::ParticleModuleInitSpawnShapeMesh, meshAssetData);
            constexpr auto listPointer = payloadOffset +
                offsetof(iw8_focus::ParticleModuleInitSpawnShapeMesh, linkedAssets);
            if (record.moduleType == meshSelector)
            {
                iw8_focus::ParticleModuleInitSpawnShapeMesh mesh{};
                std::memcpy(&mesh, record.moduleData, sizeof(mesh));
                const auto count = static_cast<std::size_t>(mesh.numMeshAssets);
                if (mesh.linkedAssets.numAssets < 0 ||
                    count != static_cast<std::size_t>(mesh.linkedAssets.numAssets))
                    throw std::runtime_error("Replay VFX mesh model and count arrays differ");
                const Module::ImportedChild *meshChild = nullptr;
                const Module::ImportedChild *listChild = nullptr;
                std::size_t previousOffset = 0;
                bool havePrevious = false;
                for (const auto &child : module.importedChildren)
                {
                    if (havePrevious && child.offset <= previousOffset)
                        throw std::runtime_error("Replay VFX mesh children are not in loader order");
                    previousOffset = child.offset;
                    havePrevious = true;
                    if (child.offset == listPointer)
                    {
                        if (listChild || child.curve || child.meshData || !child.points.empty() ||
                            !child.meshAssets.empty())
                            throw std::runtime_error("Replay VFX has an invalid mesh-model array");
                        listChild = &child;
                    }
                    if (child.meshData)
                    {
                        if (meshChild || child.offset != meshPointer || child.curve || !child.entries.empty() ||
                            !child.points.empty())
                            throw std::runtime_error("Replay VFX has an invalid mesh-count array");
                        meshChild = &child;
                    }
                }
                if ((meshChild ? meshChild->meshAssets.size() : 0) != count ||
                    (listChild ? listChild->entries.size() : 0) != count)
                    throw std::runtime_error("Replay VFX mesh-count array size does not match the module");
                if (listChild)
                    for (const auto &entry : listChild->entries)
                        if (entry.assets.size() != 1 || entry.assets[0].first != 0 ||
                            entry.assets[0].second.first != ASSET_TYPE_XMODEL ||
                            entry.assets[0].second.second.empty() || !entry.strings.empty())
                            throw std::runtime_error("Replay VFX mesh entry must reference one model");
            }
            // Shipped layout: only the pointer slots change (counts stay).
            for (const auto &child : module.importedChildren)
            {
                if (child.offset > sizeof(record) - (child.meshData ? 8 : 16) ||
                    (child.meshData && (record.moduleType != meshSelector || child.offset != meshPointer)))
                    throw std::runtime_error("Replay VFX imported child lies outside its module");
                const bool any = child.meshData ? !child.meshAssets.empty() :
                    (child.curve ? !child.points.empty() : !child.entries.empty());
                StorePointer(&record, child.offset, any ? PTR_FOLLOWS : PTR_NULL);
            }
            writer.writeT(record);
            continue;
        }
        if (record.moduleType != static_cast<uint16_t>(iw8_focus::ParticleModuleType::initRunner) &&
            record.moduleType != static_cast<uint16_t>(iw8_focus::ParticleModuleType::testBirth) &&
            record.moduleType != static_cast<uint16_t>(iw8_focus::ParticleModuleType::testImpact) &&
            !module.childEffects.empty())
            throw std::runtime_error("Replay VFX child effect list belongs only to a runner or test module");
        switch (static_cast<iw8_focus::ParticleModuleType>(record.moduleType))
        {
        case iw8_focus::ParticleModuleType::initSpawn:
        {
            if (!module.models.empty() || !module.materials.empty() ||
                !module.lightDefs.empty() || !module.decalMaterials.empty() ||
                !module.curves.empty())
                throw std::runtime_error("Replay VFX spawn module has unrelated pointer assets");
            iw8_focus::ParticleModuleInitSpawn payload{};
            std::memcpy(&payload, record.moduleData, sizeof(payload));
            payload.curve.numControlPoints = Count(module.spawnCurve);
            StorePointer(&payload, offsetof(iw8_focus::ParticleModuleInitSpawn, curve),
                         payload.curve.numControlPoints ? PTR_FOLLOWS : PTR_NULL);
            std::memcpy(record.moduleData, &payload, sizeof(payload));
            break;
        }
        case iw8_focus::ParticleModuleType::initModel:
        {
            if (!module.spawnCurve.empty() || !module.materials.empty() ||
                !module.lightDefs.empty() || !module.decalMaterials.empty() ||
                !module.curves.empty())
                throw std::runtime_error("Replay VFX model module has unrelated pointer assets");
            iw8_focus::ParticleModuleInitModel payload{};
            std::memcpy(&payload, record.moduleData, sizeof(payload));
            payload.linkedAssets.numAssets = Count(module.models);
            StorePointer(&payload, offsetof(iw8_focus::ParticleModuleInitModel, linkedAssets),
                         payload.linkedAssets.numAssets ? PTR_FOLLOWS : PTR_NULL);
            std::memcpy(record.moduleData, &payload, sizeof(payload));
            break;
        }
        case iw8_focus::ParticleModuleType::initLightOmni:
        {
            if (!module.spawnCurve.empty() || !module.models.empty() ||
                !module.materials.empty() || !module.decalMaterials.empty() ||
                !module.curves.empty())
                throw std::runtime_error("Replay VFX omni-light module has unrelated pointer assets");
            iw8_focus::ParticleModuleInitLightOmni payload{};
            std::memcpy(&payload, record.moduleData, sizeof(payload));
            payload.linkedAssets.numAssets = Count(module.lightDefs);
            StorePointer(&payload, offsetof(iw8_focus::ParticleModuleInitLightOmni, linkedAssets),
                         payload.linkedAssets.numAssets ? PTR_FOLLOWS : PTR_NULL);
            std::memcpy(record.moduleData, &payload, sizeof(payload));
            break;
        }
        case iw8_focus::ParticleModuleType::initMaterial:
        {
            if (!module.spawnCurve.empty() || !module.models.empty() ||
                !module.lightDefs.empty() || !module.decalMaterials.empty() ||
                !module.curves.empty())
                throw std::runtime_error("Replay VFX material module has unrelated pointer assets");
            iw8_focus::ParticleModuleInitMaterial payload{};
            std::memcpy(&payload, record.moduleData, sizeof(payload));
            payload.linkedAssets.numAssets = Count(module.materials);
            StorePointer(&payload, offsetof(iw8_focus::ParticleModuleInitMaterial, linkedAssets),
                         payload.linkedAssets.numAssets ? PTR_FOLLOWS : PTR_NULL);
            std::memcpy(record.moduleData, &payload, sizeof(payload));
            break;
        }
        case iw8_focus::ParticleModuleType::initDecal:
        {
            if (!module.spawnCurve.empty() || !module.models.empty() ||
                !module.materials.empty() || !module.lightDefs.empty() ||
                !module.curves.empty())
                throw std::runtime_error("Replay VFX decal module has unrelated pointer assets");
            iw8_focus::ParticleModuleInitDecal payload{};
            std::memcpy(&payload, record.moduleData, sizeof(payload));
            payload.linkedAssets.numAssets = Count(module.decalMaterials);
            StorePointer(&payload, offsetof(iw8_focus::ParticleModuleInitDecal, linkedAssets),
                         payload.linkedAssets.numAssets ? PTR_FOLLOWS : PTR_NULL);
            std::memcpy(record.moduleData, &payload, sizeof(payload));
            break;
        }
        case iw8_focus::ParticleModuleType::initRunner:
        {
            if (!module.spawnCurve.empty() || !module.models.empty() ||
                !module.materials.empty() || !module.lightDefs.empty() ||
                !module.decalMaterials.empty() || !module.curves.empty())
                throw std::runtime_error("Replay VFX runner module has unrelated pointer assets");
            iw8_focus::ParticleModuleInitRunner payload{};
            std::memcpy(&payload, record.moduleData, sizeof(payload));
            payload.linkedAssets.numAssets = Count(module.childEffects);
            StorePointer(&payload, offsetof(iw8_focus::ParticleModuleInitRunner, linkedAssets),
                         payload.linkedAssets.numAssets ? PTR_FOLLOWS : PTR_NULL);
            std::memcpy(record.moduleData, &payload, sizeof(payload));
            break;
        }
        case iw8_focus::ParticleModuleType::testBirth:
        case iw8_focus::ParticleModuleType::testDeath:
        {
            if (!module.spawnCurve.empty() || !module.models.empty() ||
                !module.materials.empty() || !module.lightDefs.empty() ||
                !module.decalMaterials.empty() || !module.curves.empty())
                throw std::runtime_error("Replay VFX birth test has unrelated pointer assets");
            iw8_focus::ParticleModuleTest payload{};
            std::memcpy(&payload, record.moduleData, sizeof(payload));
            payload.eventHandlerData.linkedAssets.numAssets = Count(module.childEffects);
            constexpr std::size_t linkedAssetsOffset =
                offsetof(iw8_focus::ParticleModuleTest, eventHandlerData) +
                offsetof(iw8_focus::ParticleModuleTestEventHandlerData, linkedAssets);
            StorePointer(&payload, linkedAssetsOffset,
                         payload.eventHandlerData.linkedAssets.numAssets ? PTR_FOLLOWS
                                                                         : PTR_NULL);
            std::memcpy(record.moduleData, &payload, sizeof(payload));
            break;
        }
        case iw8_focus::ParticleModuleType::testImpact:
        {
            if (!module.spawnCurve.empty() || !module.models.empty() ||
                !module.materials.empty() || !module.lightDefs.empty() ||
                !module.decalMaterials.empty() || !module.curves.empty())
                throw std::runtime_error("Replay VFX impact test has unrelated pointer assets");
            iw8_focus::ParticleModuleTestImpact payload{};
            std::memcpy(&payload, record.moduleData, sizeof(payload));
            payload.test.eventHandlerData.linkedAssets.numAssets = Count(module.childEffects);
            constexpr std::size_t linkedAssetsOffset =
                offsetof(iw8_focus::ParticleModuleTestImpact, test) +
                offsetof(iw8_focus::ParticleModuleTest, eventHandlerData) +
                offsetof(iw8_focus::ParticleModuleTestEventHandlerData, linkedAssets);
            StorePointer(&payload, linkedAssetsOffset,
                         payload.test.eventHandlerData.linkedAssets.numAssets ? PTR_FOLLOWS
                                                                             : PTR_NULL);
            std::memcpy(record.moduleData, &payload, sizeof(payload));
            break;
        }
        case iw8_focus::ParticleModuleType::initAtlas:
        {
            if (!module.spawnCurve.empty() || !module.models.empty() ||
                !module.materials.empty() || !module.lightDefs.empty() ||
                !module.decalMaterials.empty())
                throw std::runtime_error("Replay VFX atlas module has unrelated pointer assets");
            iw8_focus::ParticleModuleInitAtlas payload{};
            std::memcpy(&payload, record.moduleData, sizeof(payload));
            SetCurves(payload.curves, module.curves);
            std::memcpy(record.moduleData, &payload, sizeof(payload));
            break;
        }
        case iw8_focus::ParticleModuleType::colorGraph:
        {
            if (!module.spawnCurve.empty() || !module.models.empty() ||
                !module.materials.empty() || !module.lightDefs.empty() ||
                !module.decalMaterials.empty())
                throw std::runtime_error("Replay VFX color graph has unrelated pointer assets");
            iw8_focus::ParticleModuleColorGraph payload{};
            std::memcpy(&payload, record.moduleData, sizeof(payload));
            SetCurves(payload.curves, module.curves);
            std::memcpy(record.moduleData, &payload, sizeof(payload));
            break;
        }
        case iw8_focus::ParticleModuleType::sizeGraph:
        {
            if (!module.spawnCurve.empty() || !module.models.empty() ||
                !module.materials.empty() || !module.lightDefs.empty() ||
                !module.decalMaterials.empty())
                throw std::runtime_error("Replay VFX size graph has unrelated pointer assets");
            iw8_focus::ParticleModuleSizeGraph payload{};
            std::memcpy(&payload, record.moduleData, sizeof(payload));
            SetCurves(payload.curves, module.curves);
            std::memcpy(record.moduleData, &payload, sizeof(payload));
            break;
        }
        case iw8_focus::ParticleModuleType::initSpawnShapeBox:
        {
            if (!module.spawnCurve.empty() || !module.models.empty() ||
                !module.materials.empty() || !module.lightDefs.empty() ||
                !module.decalMaterials.empty())
                throw std::runtime_error("Replay VFX box module has unrelated pointer assets");
            iw8_focus::ParticleModuleInitSpawnShapeBox payload{};
            std::memcpy(&payload, record.moduleData, sizeof(payload));
            SetCurves(payload.curves, module.curves);
            std::memcpy(record.moduleData, &payload, sizeof(payload));
            break;
        }
        case iw8_focus::ParticleModuleType::initSpawnShapeCylinder:
        {
            if (!module.spawnCurve.empty() || !module.models.empty() ||
                !module.materials.empty() || !module.lightDefs.empty() ||
                !module.decalMaterials.empty())
                throw std::runtime_error("Replay VFX cylinder module has unrelated pointer assets");
            iw8_focus::ParticleModuleInitSpawnShapeCylinder payload{};
            std::memcpy(&payload, record.moduleData, sizeof(payload));
            SetCurves(payload.curves, module.curves);
            std::memcpy(record.moduleData, &payload, sizeof(payload));
            break;
        }
        case iw8_focus::ParticleModuleType::initSpawnShapeSphere:
        {
            if (!module.spawnCurve.empty() || !module.models.empty() ||
                !module.materials.empty() || !module.lightDefs.empty() ||
                !module.decalMaterials.empty())
                throw std::runtime_error("Replay VFX sphere module has unrelated pointer assets");
            iw8_focus::ParticleModuleInitSpawnShapeSphere payload{};
            std::memcpy(&payload, record.moduleData, sizeof(payload));
            SetCurves(payload.curves, module.curves);
            std::memcpy(record.moduleData, &payload, sizeof(payload));
            break;
        }
        case iw8_focus::ParticleModuleType::forceDragGraph:
        {
            if (!module.spawnCurve.empty() || !module.models.empty() ||
                !module.materials.empty() || !module.lightDefs.empty() ||
                !module.decalMaterials.empty())
                throw std::runtime_error("Replay VFX force drag module has unrelated pointer assets");
            iw8_focus::ParticleModuleForceDragGraph payload{};
            std::memcpy(&payload, record.moduleData, sizeof(payload));
            SetCurves(payload.curves, module.curves);
            std::memcpy(record.moduleData, &payload, sizeof(payload));
            break;
        }
        case iw8_focus::ParticleModuleType::velocityGraph:
        {
            if (!module.spawnCurve.empty() || !module.models.empty() ||
                !module.materials.empty() || !module.lightDefs.empty() ||
                !module.decalMaterials.empty())
                throw std::runtime_error("Replay VFX velocity graph has unrelated pointer assets");
            iw8_focus::ParticleModuleVelocityGraph payload{};
            std::memcpy(&payload, record.moduleData, sizeof(payload));
            SetCurves(payload.curves, module.curves);
            std::memcpy(record.moduleData, &payload, sizeof(payload));
            break;
        }
        case iw8_focus::ParticleModuleType::initAttributes:
        case iw8_focus::ParticleModuleType::initCloud:
        case iw8_focus::ParticleModuleType::initTail:
        case iw8_focus::ParticleModuleType::initOrientedSprite:
        case iw8_focus::ParticleModuleType::initMirrorTexture:
        case iw8_focus::ParticleModuleType::initRelativeVelocity:
        case iw8_focus::ParticleModuleType::initRotation:
        case iw8_focus::ParticleModuleType::initRotation3D:
        case iw8_focus::ParticleModuleType::gravity:
        case iw8_focus::ParticleModuleType::physicsRayCast:
            if (!module.spawnCurve.empty() || !module.models.empty() ||
                !module.materials.empty() || !module.lightDefs.empty() ||
                !module.decalMaterials.empty() || !module.curves.empty())
                throw std::runtime_error("Replay VFX value module has pointer assets");
            break;
        default:
            throw std::runtime_error("Replay VFX module selector has no pinned writer layout");
        }
        writer.writeT(record);
    }

    for (const auto &module : modules)
    {
        if (module.imported)
        {
            // Load order: each child in member order; a curve's points 16-aligned;
            // a list's entries 8-aligned, then each entry's inline strings
            // (Preload_ParticleLinkedAssetDef: INIT_SOUND reads a TempString).
            for (const auto &child : module.importedChildren)
            {
                if (child.meshData)
                {
                    if (!child.meshAssets.empty())
                    {
                        writer.align(3);
                        writer.write(child.meshAssets.data(), child.meshAssets.size() *
                                                                  sizeof(iw8_focus::ParticleSpawnMeshAssetDef));
                    }
                    continue;
                }
                if (child.curve)
                {
                    if (child.points.empty())
                        continue;
                    writer.align(15);
                    writer.write(child.points.data(),
                                 child.points.size() * sizeof(iw8_focus::ParticleCurveControlPointDef));
                    continue;
                }
                if (child.entries.empty())
                    continue;
                writer.align(7);
                for (const auto &entry : child.entries)
                {
                    auto bytes = entry.native;
                    for (const auto &[offset, asset] : entry.assets)
                        StorePointer(bytes.data(), offset, writer.assetAlias(asset.first, asset.second));
                    for (const auto &[offset, text] : entry.strings)
                        StorePointer(bytes.data(), offset, PTR_FOLLOWS);
                    writer.write(bytes.data(), bytes.size());
                }
                for (const auto &entry : child.entries)
                    for (const auto &[offset, text] : entry.strings)
                        writer.writeStr(text);
            }
            continue;
        }
        switch (static_cast<iw8_focus::ParticleModuleType>(module.native.moduleType))
        {
        case iw8_focus::ParticleModuleType::initSpawn:
            if (!module.spawnCurve.empty())
            {
                writer.align(15);
                writer.write(module.spawnCurve.data(), module.spawnCurve.size() *
                                                        sizeof(iw8_focus::ParticleCurveControlPointDef));
            }
            break;
        case iw8_focus::ParticleModuleType::initMaterial:
            if (!module.materials.empty())
            {
                writer.align(7);
                for (const auto &name : module.materials)
                {
                    iw8_focus::ParticleLinkedAssetDef linked{};
                    StorePointer(&linked, 0, writer.assetAlias(ASSET_TYPE_MATERIAL, name));
                    writer.writeT(linked);
                }
            }
            break;
        case iw8_focus::ParticleModuleType::initLightOmni:
            if (!module.lightDefs.empty())
            {
                writer.align(7);
                for (const auto &name : module.lightDefs)
                {
                    iw8_focus::ParticleLinkedAssetDef linked{};
                    StorePointer(&linked, 0, writer.assetAlias(ASSET_TYPE_LIGHTDEF, name));
                    writer.writeT(linked);
                }
            }
            break;
        case iw8_focus::ParticleModuleType::initDecal:
            if (!module.decalMaterials.empty())
            {
                writer.align(7);
                for (const auto &names : module.decalMaterials)
                {
                    iw8_focus::ParticleLinkedAssetDef linked{};
                    for (std::size_t index = 0; index < names.size(); ++index)
                        StorePointer(&linked, index * sizeof(uint64_t),
                                     names[index].empty() ? PTR_NULL
                                                          : writer.assetAlias(ASSET_TYPE_MATERIAL,
                                                                              names[index]));
                    writer.writeT(linked);
                }
            }
            break;
        case iw8_focus::ParticleModuleType::initRunner:
        case iw8_focus::ParticleModuleType::testBirth:
        case iw8_focus::ParticleModuleType::testDeath:
        case iw8_focus::ParticleModuleType::testImpact:
            if (!module.childEffects.empty())
            {
                writer.align(7);
                for (const auto &name : module.childEffects)
                {
                    iw8_focus::ParticleLinkedAssetDef linked{};
                    StorePointer(&linked, 0, writer.assetAlias(ASSET_TYPE_VFX, name));
                    if (module.native.moduleType !=
                        static_cast<uint16_t>(iw8_focus::ParticleModuleType::initRunner))
                    {
                        // Replay's TEST_* particle-system union marks the linked
                        // member with selector data 0x00010000. INIT_RUNNER
                        // records leave this union data zeroed (selector 21).
                        const uint32_t selector = 0x00010000u;
                        std::memcpy(linked.selectorData, &selector, sizeof(selector));
                    }
                    writer.writeT(linked);
                }
            }
            break;
        case iw8_focus::ParticleModuleType::initAtlas:
        case iw8_focus::ParticleModuleType::colorGraph:
        case iw8_focus::ParticleModuleType::sizeGraph:
        case iw8_focus::ParticleModuleType::initSpawnShapeBox:
        case iw8_focus::ParticleModuleType::initSpawnShapeCylinder:
        case iw8_focus::ParticleModuleType::initSpawnShapeSphere:
        case iw8_focus::ParticleModuleType::forceDragGraph:
        case iw8_focus::ParticleModuleType::velocityGraph:
            WriteCurves(writer, module.curves);
            break;
        case iw8_focus::ParticleModuleType::initModel:
            if (!module.models.empty())
            {
                writer.align(7);
                for (const auto &name : module.models)
                {
                    iw8_focus::ParticleLinkedAssetDef linked{};
                    StorePointer(&linked, 0, writer.assetAlias(ASSET_TYPE_XMODEL, name));
                    writer.writeT(linked);
                }
            }
            break;
        default:
            break;
        }
    }
}

void WriteBody(ZoneWriter &writer, const Effect &effect)
{
    if (effect.name.empty())
        throw std::runtime_error("Replay VFX has no asset name");
    auto root = effect.native;
    // Converted effects use the current format; imported stock effects keep
    // their shipped version (14, 15, 17 and 23 all ship), which Replay's
    // loader and postload interpret.
    if (!root.version)
        root.version = 23;
    root.numEmitters = Count(effect.emitters);
    root.numScriptedInputNodes = 0;
    StorePointer(&root, offsetof(iw8_focus::ParticleSystemDef, name), PTR_FOLLOWS);
    StorePointer(&root, offsetof(iw8_focus::ParticleSystemDef, emitterDefs),
                 root.numEmitters ? PTR_FOLLOWS : PTR_NULL);
    StorePointer(&root, offsetof(iw8_focus::ParticleSystemDef, scriptedInputNodeDefs), PTR_NULL);

    writer.pushStream(XFILE_BLOCK_TEMP_PRELOAD);
    writer.align(15);
    writer.writeT(root);
    writer.pushStream(XFILE_BLOCK_VIRTUAL);
    writer.writeStr(effect.name);

    if (!effect.emitters.empty())
    {
        writer.align(15);
        for (const auto &emitter : effect.emitters)
        {
            auto record = emitter.native;
            record.numStates = Count(emitter.states);
            StorePointer(&record, offsetof(iw8_focus::ParticleEmitterDef, stateDefs),
                         record.numStates ? PTR_FOLLOWS : PTR_NULL);
            record.fadeCurveDef.numControlPoints = Count(emitter.fadeCurve);
            StorePointer(&record, offsetof(iw8_focus::ParticleEmitterDef, fadeCurveDef),
                         record.fadeCurveDef.numControlPoints ? PTR_FOLLOWS : PTR_NULL);
            writer.writeT(record);
        }

        for (const auto &emitter : effect.emitters)
        {
            if (!emitter.states.empty())
            {
                writer.align(15);
                for (const auto &state : emitter.states)
                {
                    auto record = state.native;
                    StorePointer(&record, offsetof(iw8_focus::ParticleStateDef, moduleGroupDefs),
                                 PTR_FOLLOWS);
                    writer.writeT(record);
                }
                for (const auto &state : emitter.states)
                {
                    writer.align(15);
                    for (const auto &modules : state.groups)
                    {
                        iw8_focus::ParticleModuleGroupDef group{};
                        group.numModules = Count(modules);
                        StorePointer(&group, 0, group.numModules ? PTR_FOLLOWS : PTR_NULL);
                        writer.writeT(group);
                    }
                    for (const auto &modules : state.groups)
                        WriteModules(writer, modules);
                }
            }
            if (!emitter.fadeCurve.empty())
            {
                writer.align(15);
                writer.write(emitter.fadeCurve.data(), emitter.fadeCurve.size() *
                                                        sizeof(iw8_focus::ParticleCurveControlPointDef));
            }
        }
    }
    writer.popStream();
    writer.popStream();
}
} // namespace

void Register(ZoneWriter &writer, Effect effect)
{
    const auto name = effect.name;
    writer.add(ASSET_TYPE_VFX, name,
               [effect = std::move(effect)](ZoneWriter &output) { WriteBody(output, effect); });
}

void RegisterLightDef(ZoneWriter &writer, std::string name)
{
    if (name.empty())
        throw std::runtime_error("Replay VFX LightDef has no asset name");
    const auto assetName = name;
    writer.add(ASSET_TYPE_LIGHTDEF, assetName, [name = std::move(name)](ZoneWriter &output) {
        std::array<uint8_t, iw8sz::LIGHTDEF> lightDef{};
        StorePointer(lightDef.data(), 0, PTR_FOLLOWS);
        output.pushStream(XFILE_BLOCK_TEMP_PRELOAD);
        output.align(7);
        output.write(lightDef.data(), lightDef.size());
        output.pushStream(XFILE_BLOCK_VIRTUAL);
        output.writeStr(name);
        output.popStream();
        output.popStream();
    });
}

void RegisterParticleSim(ZoneWriter &writer, ParticleSim sim)
{
    if (sim.name.empty() || sim.material.empty())
        throw std::runtime_error("Replay particle sim animation has no name or material");
    // The counts of the header (+0x18 frames, +0x3C colours, +0x40 particles, +0x4A/+0x4B
    // the optional orientation and index arrays) size the arrays the loader reads.
    const auto count = [&](const std::size_t offset) {
        uint32_t value{};
        std::memcpy(&value, sim.header.data() + offset - 0x10, sizeof(value));
        return std::size_t{value};
    };
    const auto particles = count(0x40);
    const bool orientation = sim.header[0x4A - 0x10] != 0;
    const bool indexed = sim.header[0x4B - 0x10] != 0;
    if (sim.frames.size() != count(0x18) * 8 || sim.colorTable.size() != count(0x3C) * 16 ||
        sim.particleData.size() != particles * 32 || sim.orientation3D.size() != (orientation ? particles * 8 : 0) ||
        sim.index.size() != (indexed ? particles * 2 : 0))
        throw std::runtime_error("Replay particle sim animation '" + sim.name + "' has arrays of the wrong size");
    const auto assetName = sim.name;
    writer.add(ASSET_TYPE_PARTICLESIMANIMATION, assetName, [sim = std::move(sim)](ZoneWriter &output) {
        std::array<uint8_t, iw8sz::PARTICLESIMANIMATION> record{};
        std::memcpy(record.data() + 0x10, sim.header.data(), sim.header.size());
        StorePointer(record.data(), 0x00, PTR_FOLLOWS);
        StorePointer(record.data(), 0x08, output.assetAlias(ASSET_TYPE_MATERIAL, sim.material));
        StorePointer(record.data(), 0x50, sim.particleData.empty() ? PTR_NULL : PTR_FOLLOWS);
        StorePointer(record.data(), 0x58, sim.orientation3D.empty() ? PTR_NULL : PTR_FOLLOWS);
        StorePointer(record.data(), 0x60, sim.index.empty() ? PTR_NULL : PTR_FOLLOWS);
        StorePointer(record.data(), 0x68, sim.frames.empty() ? PTR_NULL : PTR_FOLLOWS);
        StorePointer(record.data(), 0x70, sim.colorTable.empty() ? PTR_NULL : PTR_FOLLOWS);
        output.pushStream(XFILE_BLOCK_TEMP_PRELOAD);
        output.align(7);
        output.write(record.data(), record.size());
        // Load_FxParticleSimAnimation's order and alignments (stock 'default', code_post_gfx).
        output.pushStream(XFILE_BLOCK_VIRTUAL);
        output.writeStr(sim.name);
        const auto array = [&](const std::vector<uint8_t> &bytes, const uint64_t alignment) {
            if (bytes.empty())
                return;
            output.align(alignment);
            output.write(bytes.data(), bytes.size());
        };
        array(sim.particleData, 31);
        array(sim.orientation3D, 7);
        array(sim.index, 0);
        array(sim.frames, 3);
        array(sim.colorTable, 3);
        output.popStream();
        output.popStream();
        // Preload_FxParticleSimAnimationPtr (0xE3BEA0; its postload 0xE3B3A0) places the
        // header in stream 2 at a 32-byte boundary: reserve it there.
        output.pushStream(XFILE_BLOCK_TEMP_POSTLOAD);
        output.align(31);
        output.reserveCalc(record.size());
        output.align(31);
        output.popStream();
    });
}
} // namespace iw8::vfx
