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

// DynEntityType of an entry. Every dynamic entity of a stock map belongs to a scriptable instance (IW8 Radiant research
// 86): one SCRIPTABLEINST entry for most definitions, and one SCRIPTABLEPHYSICS entry per dynamic entity the
// definition requires (maxNumDynEntsRequired).
enum class DynamicEntityKind : std::uint8_t
{
    clutter = 1,
    scriptableInstance = 3,
    scriptablePhysics = 4,
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
    // Scriptable entries (model basis, no XModel): the instance, the entry's index among the instance's physics
    // entries, the definition's maxNumDynEntPartsBase (kept in rigidBodies: the two share a byte) and, for an
    // instance entry, sparseOccluder (stock doors: priority 3, the second spatial population).
    DynamicEntityKind kind{DynamicEntityKind::clutter};
    std::uint32_t scriptableMapIndex{};
    std::uint16_t scriptableSubIndex{};
    bool sparseOccluder{};
};

// One loot point of the map (MapEnts.spawnGroupLoot.points; IW8 Radiant research 87 section 2.4 and 89): a place where
// the battle royale scripts may put loot. A custom map has no clusters and no zones (stock mp_backlot2 has no zones,
// stock mp_br_tut no clusters): every point is a free point (0xFFFD) or a loot cache point (0xFFFB).
struct MapLootPoint
{
    std::array<float, 3> origin{};
    std::array<float, 3> angles{};
    bool cache = false;
};

// One scriptable instance of the map (MapEnts ScriptableMapEnts.mapInstances) with the numbers of its definition the
// map-wide counts need. The definition itself is a name-only reference to an always-loaded zone, or, with carryZone,
// a copy of the stock definition taken from that stock zone when the map zone is written.
struct ScriptableInstance
{
    std::string definition;
    std::string carryZone; // stock zone that holds the definition in full ("" = always loaded)
    std::array<float, 3> origin{};
    std::array<float, 3> angles{};
    std::string targetname, target, linkname, linkto, noteworthy;
    std::uint8_t flags{};
    std::uint32_t serverInstancedParts{}; // ScriptableDef.serverInstancedPartCount
    std::uint32_t usableParts{};          // ScriptableDef.usablePartCount
    // The definition's share of ScriptablePhysicsLimits (IW8 Radiant research 89, exact on 21 stock maps): the main
    // rigid body and motion counts take definitions with flag bits 13 and 14, the detailed rigid body counts those
    // with bits 13 and 15. Stock doors have all three.
    bool physicsLimit{};       // main rigid body and motion counts
    bool physicsLimitDetail{}; // detailed rigid body counts
    std::uint8_t maxParts{1};             // ScriptableDef.maxNumDynEntPartsBase
    bool instanceEntry{true};             // has a SCRIPTABLEINST dynamic entity
    bool sparseOccluder{};                // that entry is a sparse occluder (doors)
    std::uint8_t collisionFlags{};        // of the instance entry (doors: 1)
    std::uint8_t physicsEntries{};        // ScriptableDef.maxNumDynEntsRequired
};

// The dynamic entities of the instances, in stock order (per instance: its instance entry, then its physics entries).
std::vector<DynamicEntity> ScriptableDynamicEntities(const std::vector<ScriptableInstance> &instances);

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
    // Model entries by where they are linked: the radial population (clutter and scriptable instance entries), the
    // sparse-occluder population (door instance entries) and the no-spatial list (scriptable physics entries).
    std::uint32_t radialModels{};
    std::uint32_t occluderModels{};
    std::uint32_t noSpatialModels{};
    std::array<float, 3> occluderMins{};
    std::array<float, 3> occluderMaxs{};
};

DynamicEntityCounts CountDynamicEntities(const std::vector<DynamicEntity> &entities);
void RegisterDynamicEntityList(ZoneWriter &writer, const std::vector<DynamicEntity> &entities);
} // namespace iw8
