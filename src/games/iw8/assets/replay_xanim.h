#pragma once

#include "../zone/iw8_zone.h"

#include <array>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace iw8::xanim
{
// Root motion (XAnimDeltaPart, the same layout in IW3, IW7 and Replay 1.20): a
// translation track and a rotation track of 2 (yaw: quat2) or 4 (quat) shorts
// per key. size = keys - 1 (0: one key, frame0); indices are the keys' frames
// (bytes when numframes < 256); translation keys are mins + extent * key / 255
// (small, bytes) or / 65535.
struct DeltaTrans
{
    uint16_t size = 0;
    bool small = false;
    std::array<float, 3> frame0{};
    std::array<float, 3> mins{};
    std::array<float, 3> extent{};
    std::vector<uint16_t> indices;
    std::vector<uint16_t> frames; // 3 per key
};
struct DeltaQuat
{
    uint16_t size = 0;
    std::vector<int16_t> frame0; // components
    std::vector<uint16_t> indices;
    std::vector<int16_t> frames; // components per key
};

// An XAnimParts in the stream encoding IW3 and Replay 1.20 share: part types
// (boneCount[10]: no/half/full quat, small/full/no trans, all), key counts in
// dataShort, frame indices in dataByte/dataShort/indices, keys in the random
// data streams. Verified on shipped desktop_fan_fn_01_loop: Replay decodes
// these streams exactly as IW3 does, so conversion is a repack, not a re-encode.
struct Anim
{
    std::string name;
    uint16_t numframes = 0;
    float framerate = 30;
    float frequency = 0;
    bool loop = false;
    std::array<uint8_t, 10> boneCount{};
    std::vector<std::string> names;
    std::vector<std::pair<std::string, float>> notify;
    std::vector<uint8_t> dataByte;
    std::vector<int16_t> dataShort;
    std::vector<int32_t> dataInt;
    std::vector<uint8_t> randomDataByte;
    std::vector<int16_t> randomDataShort;
    std::vector<int32_t> randomDataInt;
    std::vector<uint16_t> indices; // bytes on disk when numframes < 256
    std::optional<DeltaTrans> deltaTrans;
    std::optional<DeltaQuat> deltaQuat2; // 2 components
    std::optional<DeltaQuat> deltaQuat;  // 4 components
};

// Reads an Unlinker replay_export::XAnim dump (xanim/<name>.iw3.json), with its
// delta part: "trans", "quat" (2 components, IW3's) and "quat4" (IW7's).
Anim LoadIw3(const std::filesystem::path &path);
// Registers a top-level XAnimParts (asset type 7). Bone and notify names are
// interned as zone script strings.
void Register(ZoneWriter &writer, const Anim &anim);
} // namespace iw8::xanim
