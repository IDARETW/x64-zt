#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace iw8
{
// MapEnts.clientTrigger (+0x68, ClientTriggers 0xB0) and clientTriggerBlend (+0x118):
// client-side audio zones. CG_StartClientSideTriggers 0x1416F1360 sets them up by
// triggerType (2 audio zone, 0x10 NPC zone, 8 audio blend, 0x40 propagation); each
// frame (0x1416F1B90) the client tests its point minus origin against each hull's
// trigger-space bounds and slabs, and plays the highest-priority audio zone, whose
// name matches a SoundTable zoneName of the map bank (0x1416F6280).
struct ClientTriggerSet
{
    struct Slab
    {
        std::array<float, 3> dir{};
        float midPoint{}; // trigger space: dot(dir, point - origin)
        float halfSize{};
    };
    struct Hull
    {
        std::array<float, 3> midPoint{}; // trigger space (relative to the origin)
        std::array<float, 3> halfSize{};
        std::vector<Slab> slabs;
    };
    struct ReverbPan // CTAudRvbPanInfo, 24 B
    {
        bool hasCustomPosition{};
        std::array<float, 3> position{};
        float maxWallDistance{};
        float minReverbVolume{};
    };
    struct Trigger
    {
        uint16_t type{};
        std::array<float, 3> origin{};
        float scriptDelay{};
        float priority{};
        std::string audio; // SoundTable zoneName (empty = none)
        std::string npc;   // NPC zone name (empty = none)
        int16_t blend{-1}; // blend node index
        int16_t audioState{-1};
        ReverbPan reverbPan;
        std::vector<Hull> hulls;
    };
    struct BlendNode // ClientTriggerBlendNode, 28 B
    {
        std::array<float, 3> pointA{}, pointB{};
        uint16_t triggerA{}, triggerB{};
    };

    struct PaSpeaker // AudioPASpeaker, 20 B (MapEnts +0x3C8 count, +0x3D0 pointer)
    {
        std::array<float, 3> origin{};
        uint32_t nameHash{}, entChannelHash{};
    };

    std::vector<Trigger> triggers;
    std::vector<BlendNode> blendNodes;
    std::vector<PaSpeaker> paSpeakers;
};
} // namespace iw8
