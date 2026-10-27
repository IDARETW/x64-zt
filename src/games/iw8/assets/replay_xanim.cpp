#include <std_include.hpp>
#include "replay_xanim.h"

#include "../iw8_structs.h"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <limits>
#include <stdexcept>

namespace iw8::xanim
{
namespace
{
template <class T> void Store(uint8_t *record, const size_t offset, const T value)
{
    std::memcpy(record + offset, &value, sizeof(value));
}

// Replay XAnimParts (0xA0; Load_XAnimParts 0xDE0B10, ACTS schema).
constexpr size_t kName = 0x00, kNames = 0x08, kDataByte = 0x10, kDataShort = 0x18,
                 kDataInt = 0x20, kRandomDataShort = 0x28, kRandomDataByte = 0x30,
                 kRandomDataInt = 0x38, kIndices = 0x40, kNotify = 0x48, kDeltaPart = 0x50,
                 kRandomDataShortCount = 0x58, kRandomDataByteCount = 0x5C, kIndexCount = 0x60,
                 kFramerate = 0x64, kFrequency = 0x68, kDataByteCount = 0x6C,
                 kDataShortCount = 0x70, kDataIntCount = 0x72, kRandomDataIntCount = 0x74,
                 kNumFrames = 0x76, kFlags = 0x78, kBoneCount = 0x79, kNotifyCount = 0x83,
                 kAssetType = 0x84;
constexpr unsigned kPartTypeAll = 9;
// flags bit 0 = loop; bits 1 and 2 select the native 2D/3D delta paths.
// Non-looping prop anims played on script models ship flags 2 and assetType 7
// (equipment_box_fan_anim_stop); assetType is read only by a script query
// (0xE8FF68 -> XAnimGetAssetType), never by playback. Legacy IW3 dumps omit flags.
constexpr uint8_t kFlagLoop = 1, kFlagShipped = 2, kAssetTypeProp = 7;

template <class T> std::vector<T> Array(const nlohmann::json &source, const char *field)
{
    std::vector<T> out;
    for (const auto &value : source.at(field))
    {
        const auto number = value.get<int64_t>();
        if (number < std::numeric_limits<T>::min() || number > std::numeric_limits<T>::max())
            throw std::runtime_error(std::string("IW3 XAnim value out of range: ") + field);
        out.push_back(static_cast<T>(number));
    }
    return out;
}

template <class T> void Write(ZoneWriter &writer, const std::vector<T> &data, const unsigned alignment)
{
    if (data.empty())
        return;
    writer.align(alignment);
    writer.write(data.data(), data.size() * sizeof(T));
}

std::array<float, 3> Vector(const nlohmann::json &source, const char *field)
{
    const auto &value = source.at(field);
    if (value.size() != 3)
        throw std::runtime_error(std::string("IW3 XAnim delta vector has the wrong size: ") + field);
    return {value[0].get<float>(), value[1].get<float>(), value[2].get<float>()};
}

DeltaQuat LoadQuat(const nlohmann::json &source, const size_t components, const uint16_t numframes)
{
    DeltaQuat quat;
    quat.size = source.at("size").get<uint16_t>();
    if (!quat.size)
    {
        quat.frame0 = Array<int16_t>(source, "frame0");
        if (quat.frame0.size() != components)
            throw std::runtime_error("IW3 XAnim delta rotation key has the wrong size");
        return quat;
    }
    quat.indices = Array<uint16_t>(source, "indices");
    quat.frames = Array<int16_t>(source, "frames");
    if (quat.indices.size() != quat.size + 1u || quat.frames.size() != (quat.size + 1u) * components ||
        (numframes < 256 && std::any_of(quat.indices.begin(), quat.indices.end(), [](uint16_t i) { return i > 255; })))
        throw std::runtime_error("inconsistent IW3 XAnim delta rotation");
    return quat;
}

// The key frames of a delta track: bytes below 256 frames, else shorts (XAnimDynamicIndices).
void WriteIndices(ZoneWriter &out, const std::vector<uint16_t> &indices, const uint16_t numframes)
{
    if (numframes < 256)
    {
        std::vector<uint8_t> bytes(indices.begin(), indices.end());
        Write(out, bytes, 0);
    }
    else
        Write(out, indices, 1);
}

// Load_XAnimDeltaPartQuat2 / Quat: an 8-byte head (size), then frame0 or the
// frames pointer, the indices and the frames.
void WriteQuat(ZoneWriter &out, const DeltaQuat &quat, const size_t components, const uint16_t numframes)
{
    out.align(7);
    std::array<uint8_t, 8> head{};
    Store(head.data(), 0, quat.size);
    out.write(head.data(), head.size());
    if (!quat.size)
    {
        out.write(quat.frame0.data(), components * sizeof(int16_t));
        return;
    }
    out.writeT<uint64_t>(PTR_FOLLOWS);
    WriteIndices(out, quat.indices, numframes);
    Write(out, quat.frames, 1);
}
} // namespace

Anim LoadIw3(const std::filesystem::path &path)
{
    std::ifstream input(path);
    if (!input)
        throw std::runtime_error("cannot read IW3 XAnim dump " + path.string());
    const auto source = nlohmann::json::parse(input);
    if (source.value("schema", 0) != 1)
        throw std::runtime_error("unsupported IW3 XAnim dump schema: " + path.string());
    Anim anim;
    anim.name = source.at("name");
    anim.numframes = source.at("numframes");
    if (const auto &delta = source.at("delta_part"); !delta.is_null())
    {
        if (const auto trans = delta.find("trans"); trans != delta.end())
        {
            DeltaTrans track;
            track.size = trans->at("size").get<uint16_t>();
            track.small = trans->at("small").get<bool>();
            if (!track.size)
                track.frame0 = Vector(*trans, "frame0");
            else
            {
                track.mins = Vector(*trans, "mins");
                track.extent = Vector(*trans, "extent");
                track.indices = Array<uint16_t>(*trans, "indices");
                track.frames = Array<uint16_t>(*trans, "frames");
                if (track.indices.size() != track.size + 1u || track.frames.size() != (track.size + 1u) * 3 ||
                    (track.small && std::any_of(track.frames.begin(), track.frames.end(), [](uint16_t v) { return v > 255; })) ||
                    (anim.numframes < 256 &&
                     std::any_of(track.indices.begin(), track.indices.end(), [](uint16_t i) { return i > 255; })))
                    throw std::runtime_error("inconsistent IW3 XAnim delta translation: " + anim.name);
            }
            anim.deltaTrans = std::move(track);
        }
        if (const auto quat = delta.find("quat"); quat != delta.end())
            anim.deltaQuat2 = LoadQuat(*quat, 2, anim.numframes);
        if (const auto quat = delta.find("quat4"); quat != delta.end())
            anim.deltaQuat = LoadQuat(*quat, 4, anim.numframes);
    }
    anim.framerate = source.at("framerate");
    anim.frequency = source.at("frequency");
    anim.loop = source.at("loop");
    if (const auto flags = source.find("iw7_flags"); flags != source.end())
    {
        if (!flags->is_number_integer())
            throw std::runtime_error("invalid IW7 XAnim flags: " + anim.name);
        const auto value = flags->get<int64_t>();
        if (value < 0 || value > UINT8_MAX || ((value & kFlagLoop) != 0) != anim.loop)
            throw std::runtime_error("inconsistent IW7 XAnim flags: " + anim.name);
        anim.iw7Flags = static_cast<uint8_t>(value);
    }
    const auto counts = source.at("bone_count");
    if (counts.size() != anim.boneCount.size())
        throw std::runtime_error("IW3 XAnim bone-count table has the wrong size");
    for (size_t i = 0; i < anim.boneCount.size(); ++i)
        anim.boneCount[i] = counts[i].get<uint8_t>();
    for (const auto &name : source.at("names"))
        anim.names.push_back(name);
    for (const auto &note : source.at("notify"))
        anim.notify.emplace_back(note.at("name").get<std::string>(), note.at("time").get<float>());
    anim.dataByte = Array<uint8_t>(source, "data_byte");
    anim.dataShort = Array<int16_t>(source, "data_short");
    anim.dataInt = Array<int32_t>(source, "data_int");
    anim.randomDataByte = Array<uint8_t>(source, "random_data_byte");
    anim.randomDataShort = Array<int16_t>(source, "random_data_short");
    anim.randomDataInt = Array<int32_t>(source, "random_data_int");
    anim.indices = Array<uint16_t>(source, "indices");
    if (anim.names.size() != anim.boneCount[kPartTypeAll] || anim.notify.size() > 255 ||
        anim.dataShort.size() > UINT16_MAX || anim.dataInt.size() > UINT16_MAX ||
        anim.randomDataInt.size() > UINT16_MAX || !anim.numframes ||
        (anim.numframes < 256 && std::any_of(anim.indices.begin(), anim.indices.end(),
                                             [](uint16_t index) { return index > 255; })))
        throw std::runtime_error("inconsistent IW3 XAnim dump: " + anim.name);
    return anim;
}

void Register(ZoneWriter &writer, const Anim &anim)
{
    std::vector<uint32_t> names;
    for (const auto &name : anim.names)
        names.push_back(writer.internScriptString(name));
    std::vector<std::pair<uint32_t, float>> notify;
    for (const auto &[name, time] : anim.notify)
        notify.emplace_back(writer.internScriptString(name), time);

    writer.add(ASSET_TYPE_XANIM, anim.name, [anim, names, notify](ZoneWriter &out) {
        std::array<uint8_t, iw8sz::XANIM> parts{};
        const auto follows = [](const bool present) { return present ? PTR_FOLLOWS : PTR_NULL; };
        Store(parts.data(), kName, PTR_FOLLOWS);
        Store(parts.data(), kNames, follows(!names.empty()));
        Store(parts.data(), kDataByte, follows(!anim.dataByte.empty()));
        Store(parts.data(), kDataShort, follows(!anim.dataShort.empty()));
        Store(parts.data(), kDataInt, follows(!anim.dataInt.empty()));
        Store(parts.data(), kRandomDataShort, follows(!anim.randomDataShort.empty()));
        Store(parts.data(), kRandomDataByte, follows(!anim.randomDataByte.empty()));
        Store(parts.data(), kRandomDataInt, follows(!anim.randomDataInt.empty()));
        Store(parts.data(), kIndices, follows(!anim.indices.empty()));
        Store(parts.data(), kNotify, follows(!notify.empty()));
        // Shipped anims carry a delta part even without root motion (all three
        // members null); match them.
        Store(parts.data(), kDeltaPart, PTR_FOLLOWS);
        Store(parts.data(), kRandomDataShortCount, static_cast<uint32_t>(anim.randomDataShort.size()));
        Store(parts.data(), kRandomDataByteCount, static_cast<uint32_t>(anim.randomDataByte.size()));
        Store(parts.data(), kIndexCount, static_cast<uint32_t>(anim.indices.size()));
        Store(parts.data(), kFramerate, anim.framerate);
        Store(parts.data(), kFrequency, anim.frequency);
        Store(parts.data(), kDataByteCount, static_cast<uint32_t>(anim.dataByte.size()));
        Store(parts.data(), kDataShortCount, static_cast<uint16_t>(anim.dataShort.size()));
        Store(parts.data(), kDataIntCount, static_cast<uint16_t>(anim.dataInt.size()));
        Store(parts.data(), kRandomDataIntCount, static_cast<uint16_t>(anim.randomDataInt.size()));
        Store(parts.data(), kNumFrames, anim.numframes);
        parts[kFlags] = anim.iw7Flags.value_or(static_cast<uint8_t>(kFlagShipped | (anim.loop ? kFlagLoop : 0)));
        std::memcpy(parts.data() + kBoneCount, anim.boneCount.data(), anim.boneCount.size());
        parts[kNotifyCount] = static_cast<uint8_t>(notify.size());
        parts[kAssetType] = kAssetTypeProp;

        out.pushStream(XFILE_BLOCK_TEMP_PRELOAD);
        out.align(7);
        out.write(parts.data(), parts.size());
        // Load_XAnimParts order, all in the virtual stream (DB_PushStreamPos 5).
        out.pushStream(XFILE_BLOCK_VIRTUAL);
        out.writeStr(anim.name);
        Write(out, names, 3);
        if (!notify.empty())
        {
            out.align(3);
            for (const auto &[name, time] : notify)
            {
                out.writeT<uint32_t>(name);
                out.writeT<float>(time);
            }
        }
        // Load_XAnimDeltaPart: trans, quat2, quat, each followed by its data.
        out.align(7);
        out.writeT<uint64_t>(anim.deltaTrans ? PTR_FOLLOWS : PTR_NULL);
        out.writeT<uint64_t>(anim.deltaQuat2 ? PTR_FOLLOWS : PTR_NULL);
        out.writeT<uint64_t>(anim.deltaQuat ? PTR_FOLLOWS : PTR_NULL);
        if (const auto &trans = anim.deltaTrans)
        {
            out.align(7);
            std::array<uint8_t, 8> head{};
            Store(head.data(), 0, trans->size);
            head[2] = trans->small ? 1 : 0;
            out.write(head.data(), head.size());
            if (!trans->size)
                out.write(trans->frame0.data(), sizeof(trans->frame0));
            else
            {
                out.write(trans->mins.data(), sizeof(trans->mins));
                out.write(trans->extent.data(), sizeof(trans->extent));
                out.writeT<uint64_t>(PTR_FOLLOWS);
                WriteIndices(out, trans->indices, anim.numframes);
                if (trans->small)
                {
                    std::vector<uint8_t> bytes(trans->frames.begin(), trans->frames.end());
                    Write(out, bytes, 0);
                }
                else
                    Write(out, trans->frames, 1);
            }
        }
        if (anim.deltaQuat2)
            WriteQuat(out, *anim.deltaQuat2, 2, anim.numframes);
        if (anim.deltaQuat)
            WriteQuat(out, *anim.deltaQuat, 4, anim.numframes);
        Write(out, anim.dataByte, 0);
        Write(out, anim.dataShort, 1);
        Write(out, anim.dataInt, 3);
        Write(out, anim.randomDataShort, 1);
        Write(out, anim.randomDataByte, 0);
        Write(out, anim.randomDataInt, 3);
        if (anim.numframes < 256)
        {
            std::vector<uint8_t> bytes(anim.indices.begin(), anim.indices.end());
            Write(out, bytes, 0);
        }
        else
            Write(out, anim.indices, 1);
        out.popStream();
        out.popStream();
        // Postload_XAnimPartsPtr (0xDEA2D0) re-allocates the struct in stream 2.
        out.pushStream(XFILE_BLOCK_TEMP_POSTLOAD);
        out.align(7);
        out.reserveCalc(iw8sz::XANIM);
        out.popStream();
    });
}
} // namespace iw8::xanim
