#pragma once

#include "../zone/iw8_zone.h"

#include <array>
#include <string>
#include <vector>

namespace iw8
{
enum class DynamicEntityBasis : std::uint8_t
{
    model = 0,
    brush = 1,
};

struct DynamicEntity
{
    std::string model;
    std::array<float, 4> quaternion{};
    std::array<float, 3> origin{};
    bool noPhysics{true};
    // Simulated clutter: physics-asset rigid-body count and collision flags
    // (shipped DynEnt clutter: one body per model bone, collisionFlags 2).
    std::uint8_t rigidBodies{};
    std::uint8_t collisionFlags{};
    DynamicEntityBasis basis{DynamicEntityBasis::model};
    std::uint16_t brushModel{};
};

struct DynamicEntityCounts
{
    std::uint32_t models{};
    std::uint32_t brushes{};
    // Model clutter sits in the radial spatial population like stock clutter:
    // one leaf partition bounding every model origin (padded). Brush dynents
    // stay on the no-spatial list.
    std::array<float, 3> modelMins{};
    std::array<float, 3> modelMaxs{};
    // MapEnts dynEntMaxPosePartCount / dynEntsWithExtraPosePartsCount: the most pose parts of an entry and the entries
    // with more than one. A simulated model entry has one pose part per rigid body (DynEntCL_IsEntryUsable 0x1417F0770:
    // physicsAssetRigidBodiesCount unless the basis is a brush or the entry has no physics), and Replay keeps the extra
    // parts of each such entry in a client pool of (max - 1) * min(entries, client entries) records
    // (DynEntCL_AllocateClientMemory 0x1417F0410); DynEntCL_AddEntity pops one block per entry.
    std::uint8_t maxPoseParts{1};
    std::uint32_t entriesWithExtraPoseParts{};
};

DynamicEntityCounts CountDynamicEntities(const std::vector<DynamicEntity> &entities);
void RegisterDynamicEntityList(ZoneWriter &writer, const std::vector<DynamicEntity> &entities);
} // namespace iw8
