#include <std_include.hpp>
#include "replay_dynentity.h"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>
#include <stdexcept>
#include <utility>

namespace iw8
{
namespace
{
constexpr char AssetName[] = "dynentitylist0";
constexpr std::uint32_t InvalidNode = std::numeric_limits<std::uint32_t>::max();
constexpr std::size_t SpatialPopulationSize = 0x38;
// Clutter moves when shot; the leaf bounds the spawn origins with some slack.
constexpr float SpatialLeafPadding = 128.0f;

template <typename T> void Put(std::uint8_t *bytes, const std::size_t offset, const T &value)
{
    std::memcpy(bytes + offset, &value, sizeof(value));
}

using EntityPartition = std::array<std::vector<const DynamicEntity *>, 2>;

// Where a model entry is linked: 0 radial population, 1 sparse-occluder population, 2 the no-spatial list.
std::size_t ChainOf(const DynamicEntity &entity)
{
    if (entity.kind == DynamicEntityKind::scriptablePhysics)
        return 2;
    return entity.kind == DynamicEntityKind::scriptableInstance && entity.sparseOccluder ? 1 : 0;
}

// Quaternion (x, y, z, w) of Euler angles (pitch, yaw, roll in degrees), as AnglesToQuat.
std::array<float, 4> QuaternionOf(const std::array<float, 3> &angles)
{
    const double k = 3.14159265358979323846 / 360.0;
    const double sp = std::sin(angles[0] * k), cp = std::cos(angles[0] * k);
    const double sy = std::sin(angles[1] * k), cy = std::cos(angles[1] * k);
    const double sr = std::sin(angles[2] * k), cr = std::cos(angles[2] * k);
    return {static_cast<float>(sr * cp * cy - cr * sp * sy), static_cast<float>(cr * sp * cy + sr * cp * sy),
            static_cast<float>(cr * cp * sy - sr * sp * cy), static_cast<float>(cr * cp * cy + sr * sp * sy)};
}

EntityPartition Partition(const std::vector<DynamicEntity> &entities)
{
    EntityPartition result;
    for (const auto &entity : entities)
    {
        const auto basis = static_cast<std::size_t>(entity.basis);
        if (basis >= result.size())
            throw std::runtime_error("Replay dynamic entity has an invalid basis");
        result[basis].push_back(&entity);
    }
    return result;
}

// Stock mp_hideout clutter (verified ACTS dump 2026-10-16): type CLUTTER, priority
// DYNENT_RADIAL_PRIORITY_LEVEL_LOW, spawnActive false, clientId 0xFFFF/0xFFFF
// (unassigned), scriptableMapIndex 0, linked into its leaf's bucket chain
// (head -> ... -> tail, tail next -1). Stock keeps only scriptable physics on the
// no-spatial list (ALWAYS_ACTIVE, spawnActive, clientId 0); clutter placed there
// was never activated, so no prop rendered.
// `lastNoSpatialModel`: the last model entry of the no-spatial list (scriptable physics), which the first brush entry
// continues; InvalidNode without one.
void WriteDefinitions(ZoneWriter &writer, const std::vector<const DynamicEntity *> &entities,
                      const DynamicEntityBasis basis, const std::uint32_t globalBase,
                      const std::uint32_t lastNoSpatialModel = InvalidNode)
{
    if (entities.empty())
        return;

    writer.align(7);
    // The previous entry of each chain of model entries (ChainOf).
    std::array<std::uint32_t, 3> previous{InvalidNode, InvalidNode, InvalidNode};
    for (std::size_t index = 0; index < entities.size(); ++index)
    {
        const auto &entity = *entities[index];
        const auto globalIndex = globalBase + static_cast<std::uint32_t>(index);
        const bool scriptable = entity.kind != DynamicEntityKind::clutter;
        std::uint8_t definition[0x50]{};
        if (basis == DynamicEntityBasis::model && !scriptable)
            Put(definition, 0x00, writer.assetAlias(ASSET_TYPE_XMODEL, entity.model));
        std::memcpy(definition + 0x10, entity.quaternion.data(), sizeof(entity.quaternion));
        std::memcpy(definition + 0x20, entity.origin.data(), sizeof(entity.origin));
        // Each chain runs from its last entry down to its first.
        if (basis == DynamicEntityBasis::model)
        {
            auto &link = previous[ChainOf(entity)];
            Put(definition, 0x2C, link);
            link = globalIndex;
        }
        else
            Put(definition, 0x2C, globalIndex == globalBase ? lastNoSpatialModel : globalIndex - 1);
        definition[0x31] = static_cast<std::uint8_t>(basis);
        definition[0x32] = static_cast<std::uint8_t>(entity.kind);
        if (basis == DynamicEntityBasis::model && scriptable)
        {
            // Stock scriptable entries (mp_shipment, mp_backlot2; IW8 Radiant research 86): clientId 0 / 0, the
            // instance's index; an instance entry is radial (priority 0) or a sparse occluder (3), a physics entry is
            // always active, spawned active and on the no-spatial list.
            const bool physics = entity.kind == DynamicEntityKind::scriptablePhysics;
            definition[0x33] = physics ? 0xFF : entity.sparseOccluder ? 3 : 0;
            Put<std::uint32_t>(definition, 0x34, 0); // spatial leaf 0
            Put<std::uint32_t>(definition, 0x3C, entity.scriptableMapIndex);
            Put(definition, 0x44, entity.scriptableSubIndex);
            definition[0x49] = physics ? 1 : 0; // spawnActive
        }
        else if (basis == DynamicEntityBasis::model)
        {
            definition[0x33] = 0;                            // DYNENT_RADIAL_PRIORITY_LEVEL_LOW
            Put<std::uint32_t>(definition, 0x34, 0);         // spatial leaf 0
            Put<std::uint16_t>(definition, 0x38, 0xFFFF);    // clientId: none yet
            Put<std::uint16_t>(definition, 0x3A, 0xFFFF);
            Put<std::uint32_t>(definition, 0x3C, 0);         // scriptableMapIndex (stock 0)
        }
        else
        {
            definition[0x33] = 0xFF;                           // always active
            Put<std::uint32_t>(definition, 0x3C, InvalidNode); // no ScriptableMap index
            Put(definition, 0x42, entity.brushModel);
            definition[0x49] = 1; // spawnActive
        }
        Put<std::uint16_t>(definition, 0x40, 0); // dynentitylist0
        definition[0x48] = entity.rigidBodies;  // physicsAssetRigidBodiesCount / scriptableMaxPartCount
        definition[0x4A] = entity.noPhysics ? 1 : 0;
        definition[0x4D] = entity.collisionFlags;
        writer.write(definition, sizeof(definition));
    }
}

void EmitBody(ZoneWriter &writer, const std::vector<DynamicEntity> &entities)
{
    if (entities.empty())
        throw std::runtime_error("invalid Replay dynamic-entity count");
    const EntityPartition partition = Partition(entities);
    const auto counts = CountDynamicEntities(entities);
    const auto total = counts.models + counts.brushes;

    std::uint8_t list[iw8sz::DYNENTITYLIST]{};
    Put(list, 0x00, PTR_FOLLOWS);
    Put<std::uint16_t>(list, 0x08, 0);
    Put(list, 0x0C, counts.models);
    Put(list, 0x10, counts.brushes);
    Put(list, 0x14, total);
    Put(list, 0x18, counts.models ? PTR_FOLLOWS : PTR_NULL);
    Put(list, 0x20, counts.brushes ? PTR_FOLLOWS : PTR_NULL);
    Put(list, 0x28, PTR_FOLLOWS);
    Put(list, 0x30, PTR_FOLLOWS);
    // The first and last model entry of each chain (ChainOf).
    std::array<std::uint32_t, 3> first{InvalidNode, InvalidNode, InvalidNode}, last = first;
    for (std::size_t index = 0; index < partition[0].size(); ++index)
    {
        const auto chain = ChainOf(*partition[0][index]);
        if (first[chain] == InvalidNode)
            first[chain] = static_cast<std::uint32_t>(index);
        last[chain] = static_cast<std::uint32_t>(index);
    }
    // dynEntNoSpatialList head/tail: scriptable physics entries, then brush dynents (head = the last entry).
    Put(list, 0x38, counts.brushes ? total - 1 : last[2]);
    Put(list, 0x3C, first[2] != InvalidNode ? first[2] : counts.brushes ? counts.models : InvalidNode);

    writer.pushStream(XFILE_BLOCK_TEMP_PRELOAD);
    writer.align(7);
    writer.write(list, sizeof(list));
    writer.pushStream(XFILE_BLOCK_VIRTUAL);
    writer.writeStr(AssetName);
    WriteDefinitions(writer, partition[0], DynamicEntityBasis::model, 0);
    WriteDefinitions(writer, partition[1], DynamicEntityBasis::brush, counts.models, last[2]);
    // q_Load_SpatialPartition_TransientPopulation DDFF90: the 0x38 struct, then
    // buckets, bucketTails and partitionIds (partitionCount u32 each, 4-aligned).
    // Population 0 = radial (clutter, scriptable instance entries), population 1 = sparse occluders (door instance
    // entries); one leaf each, empty without entries.
    const std::array<std::uint32_t, 2> populations{counts.radialModels, counts.occluderModels};
    for (std::size_t chain = 0; chain < populations.size(); ++chain)
    {
        std::uint8_t population[SpatialPopulationSize]{};
        if (populations[chain])
        {
            Put(population, 0x00, PTR_FOLLOWS);
            Put(population, 0x08, PTR_FOLLOWS);
            Put(population, 0x10, PTR_FOLLOWS);
            Put<std::uint32_t>(population, 0x30, 1);
            Put<std::uint32_t>(population, 0x34, populations[chain]);
        }
        writer.align(7);
        writer.write(population, sizeof(population));
        if (populations[chain])
        {
            writer.align(3);
            writer.writeT<std::uint32_t>(last[chain]); // bucket head
            writer.align(3);
            writer.writeT<std::uint32_t>(first[chain]); // bucket tail
            writer.align(3);
            writer.writeT<std::uint32_t>(0); // partition id
        }
    }
    writer.popStream();
    writer.popStream();
}
} // namespace

DynamicEntityCounts CountDynamicEntities(const std::vector<DynamicEntity> &entities)
{
    DynamicEntityCounts counts;
    counts.modelMins.fill(std::numeric_limits<float>::max());
    counts.modelMaxs.fill(std::numeric_limits<float>::lowest());
    counts.occluderMins.fill(std::numeric_limits<float>::max());
    counts.occluderMaxs.fill(std::numeric_limits<float>::lowest());
    for (const auto &entity : entities)
    {
        std::uint32_t *count = nullptr;
        switch (entity.basis)
        {
        case DynamicEntityBasis::model:
            count = &counts.models;
            if (const auto chain = ChainOf(entity); chain == 2)
                ++counts.noSpatialModels;
            else
            {
                ++(chain ? counts.occluderModels : counts.radialModels);
                auto &mins = chain ? counts.occluderMins : counts.modelMins;
                auto &maxs = chain ? counts.occluderMaxs : counts.modelMaxs;
                for (std::size_t axis = 0; axis < 3; ++axis)
                {
                    mins[axis] = std::min(mins[axis], entity.origin[axis] - SpatialLeafPadding);
                    maxs[axis] = std::max(maxs[axis], entity.origin[axis] + SpatialLeafPadding);
                }
            }
            break;
        case DynamicEntityBasis::brush:
            count = &counts.brushes;
            break;
        default:
            throw std::runtime_error("Replay dynamic entity has an invalid basis");
        }
        if (*count == std::numeric_limits<std::uint16_t>::max())
            throw std::runtime_error("Replay dynamic-entity basis exceeds MapEnts limits");
        ++*count;
        // The pose parts of the entry as DynEntCL_IsEntryUsable counts them (a clutter entry: type 1).
        // A scriptable entry: scriptableMaxPartCount, the same byte (stock: dynEntMaxPosePartCount is the largest and
        // dynEntsWithExtraPosePartsCount the entries above 1, mp_shipment 47 / 163, mp_backlot2 39 / 194).
        const std::uint8_t parts =
            entity.basis == DynamicEntityBasis::model && !entity.noPhysics ? entity.rigidBodies : 1;
        counts.maxPoseParts = std::max(counts.maxPoseParts, parts);
        if (parts > 1)
            ++counts.entriesWithExtraPoseParts;
    }
    if (!counts.radialModels)
    {
        counts.modelMins.fill(0.0f);
        counts.modelMaxs.fill(0.0f);
    }
    if (!counts.occluderModels)
    {
        counts.occluderMins.fill(0.0f);
        counts.occluderMaxs.fill(0.0f);
    }
    return counts;
}

std::vector<DynamicEntity> ScriptableDynamicEntities(const std::vector<ScriptableInstance> &instances)
{
    std::vector<DynamicEntity> result;
    for (std::size_t index = 0; index < instances.size(); ++index)
    {
        const auto &instance = instances[index];
        DynamicEntity entity;
        entity.quaternion = QuaternionOf(instance.angles);
        entity.origin = instance.origin;
        entity.noPhysics = false;
        entity.rigidBodies = instance.maxParts; // scriptableMaxPartCount
        entity.scriptableMapIndex = static_cast<std::uint32_t>(index);
        if (instance.instanceEntry)
        {
            entity.kind = DynamicEntityKind::scriptableInstance;
            entity.sparseOccluder = instance.sparseOccluder;
            entity.collisionFlags = instance.collisionFlags;
            result.push_back(entity);
        }
        entity.kind = DynamicEntityKind::scriptablePhysics;
        entity.sparseOccluder = false;
        entity.collisionFlags = 0;
        for (std::uint16_t sub = 0; sub < instance.physicsEntries; ++sub)
        {
            entity.scriptableSubIndex = sub;
            result.push_back(entity);
        }
    }
    return result;
}

void RegisterDynamicEntityList(ZoneWriter &writer, const std::vector<DynamicEntity> &entities)
{
    for (const auto &entity : entities)
    {
        if (entity.basis == DynamicEntityBasis::model && entity.kind == DynamicEntityKind::clutter && entity.model.empty())
            throw std::runtime_error("Replay model dynamic entity has no XModel");
        if (entity.basis == DynamicEntityBasis::brush && !entity.model.empty())
            throw std::runtime_error("Replay brush dynamic entity unexpectedly names an XModel");
        float length = 0.0f;
        for (const float value : entity.quaternion)
        {
            if (!std::isfinite(value))
                throw std::runtime_error("Replay dynamic entity has an invalid quaternion");
            length += value * value;
        }
        if (std::abs(length - 1.0f) > 0.01f)
            throw std::runtime_error("Replay dynamic entity has a non-unit quaternion");
        for (const float value : entity.origin)
            if (!std::isfinite(value))
                throw std::runtime_error("Replay dynamic entity has an invalid origin");
    }
    if (!entities.empty())
    {
        writer.add(ASSET_TYPE_DYNENTITYLIST, AssetName,
                   [entities](ZoneWriter &output) { EmitBody(output, entities); });
    }
}
} // namespace iw8
