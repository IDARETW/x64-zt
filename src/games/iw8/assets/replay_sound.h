#pragma once

#include "../zone/iw8_zone.h"

#include <array>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace iw8::sound
{
// Replay SND hash (0x14125B000): 65599 * h + lowercase byte from 5381; zero maps
// to one. Alias, secondary alias, curve and SAB entry keys all use it.
uint32_t HashName(std::string_view name);

// A 16-bit PCM sound stored in the bank's loaded SAB as codec 8 (FLAC frames).
struct Sample
{
    std::string name; // SAB asset name; HashName(name) is the entry key / alias assetId
    uint32_t rate{};
    uint8_t channels{};
    bool looping{};
    std::vector<int16_t> pcm; // interleaved frames
};

// One 232-byte SndAlias. Fields left at their defaults keep the stock values
// shared by every Replay map-bank alias ("default" curves, no context, no duck).
struct Alias
{
    std::string assetName;
    std::string secondaryName; // empty for none
    uint32_t flags{};          // bit0 loop, bits 7..9 load type, bits 10..17 channel
    int32_t volModIndex{};
    float volMin{1}, volMax{1};
    float pitchMin{1}, pitchMax{1};
    float distMin{}, distMax{};
    float probability{1};
    float lfePercentage{};
    float centerPercentage{};                       // +124
    float envelopMin{}, envelopMax{}, envelopPercentage{}; // +152..+160 (IW3 envelope)
    float reverbMultiplier{1};
    float farReverbMultiplier{1};
    int32_t startDelay{};
    int32_t startDelayMax{};
    uint32_t volumeFalloffCurve{};
    float stereoSpreadMinDist{}, stereoSpreadMaxDist{}, stereoSpreadMidPoint{};
    uint8_t stereoSpreadMaxAngle{};
    int16_t maxDurationDsec{};
    // Source-game fields (IW7 aliases); the defaults keep the stock map-bank values.
    uint32_t lpfCurve{}, hpfCurve{}, reverbSendCurve{}; // 0 = "default"
    float donutFadeEnd{-1};                             // < 0: distMin (stock)
    float velocityMin{};
    float smartPanDistance2d{}, smartPanDistance3d{}, smartPanAttenuation2d{1};
    uint8_t masterPriority{};
    float masterPercentage{}, masterSlavePercentage{1};
    uint32_t contextType{}, contextValue{};
    uint32_t speakerMap{}, occlusionShape{}, dopplerPreset{}; // 0 = "default"
    uint32_t duck{}; // +60: SndDuck id the alias starts (0 = none)
};

// SndDuck (168 B, SndBank +304/+312): while it holds, each volmod (the 128-entry
// arrays' index; stock defaults 1 / 0 / 1 / 0 / 0) plays attenuated and filtered.
struct Duck
{
    std::string name;
    uint32_t id{};
    float fadeIn{}, fadeOut{}, startDelay{}, minDistance{}, distance{}, length{};
    uint32_t fadeInCurve{}, fadeOutCurve{};
    bool updateWhilePaused{}, trackAmplitude{}, disableInSplitscreen{};
    float aliasAttenuation{1}, aliasHpf{}, aliasLpf{1};
    uint32_t duckAlias{};
    std::array<float, 128> attenuation{}, hpf{}, lpf{}, focusAmount{};
    std::array<uint32_t, 128> focusCone{};
    Duck()
    {
        attenuation.fill(1.0f);
        lpf.fill(1.0f);
    }
};

struct AliasList
{
    std::string name;
    std::vector<Alias> aliases;
};

// SndCurve: name, id and up to 16 (x, y) points.
struct Curve
{
    std::string name;
    std::vector<std::array<float, 2>> points;
};

// Replay SAB v10 container (magic 2UX#, build 16, 44-byte entries) with every
// sample as fixed 1024-sample FLAC frames behind its 18-byte seek table.
std::vector<uint8_t> BuildSab(const std::vector<Sample> &samples, const std::string &zone);

// Index of a Replay SndGlobals ("singleton", code_post_gfx) volmod / entity channel by
// name, or -1.
int VolModIndex(std::string_view name);
int EntChannelIndex(std::string_view name);
// Whether a hash names one of Replay's 20 SndGlobals context types.
bool IsContextType(uint32_t id);

// One SoundTable array (or the bank's send effects): `count` records of the Replay
// layout. Pointer fields hold zero in `records`; `strings` lists each record's
// string fields in stream order (stringOffsets), empty for a null pointer.
struct TableArray
{
    uint32_t stride{};
    std::vector<uint32_t> stringOffsets;
    std::vector<uint8_t> records;
    std::vector<std::string> strings; // count * stringOffsets.size()
    size_t count() const
    {
        return stride ? records.size() / stride : 0;
    }
};

// SndBank.soundTable (+72, 232 B): fourteen (pointer, count) arrays from +8 in
// this order. CG_StartClientSideTriggersAudio 0x1416F5E00 selects the table whose
// id is HashName(map name) (SND_GetSoundTableById 0x141394410).
enum TableKind
{
    TABLE_ADSR,
    TABLE_AMBIENT_DEF,
    TABLE_AMBIENT_EVENT,
    TABLE_AMBIENT_ELEMENT,
    TABLE_FILTER,
    TABLE_MIX,
    TABLE_NPC_ADSR_ZONE,
    TABLE_OCCLUSION,
    TABLE_PLAYER_ADSR_ZONE,
    TABLE_REVERB,
    TABLE_TIMESCALE,
    TABLE_ZONE,
    TABLE_FULL_OCCLUSION,
    TABLE_BREATH_STATE,
    TABLE_COUNT
};

struct SoundTable
{
    uint32_t id{};
    std::array<TableArray, TABLE_COUNT> arrays;
    bool empty() const
    {
        for (const TableArray &array : arrays)
            if (array.count())
                return false;
        return true;
    }
};

struct Bank
{
    std::string name;      // SndBankResident asset name
    std::string zone;      // the loader opens zone\<zone>.sabs (and patch_<zone>.sabs)
    std::string streamKey; // StreamKey asset holding the loaded SAB
    std::vector<AliasList> lists;
    std::vector<Curve> volumeCurves;
    std::vector<Curve> lpfCurves, hpfCurves, reverbCurves; // SndBank +416/+432/+448
    std::vector<uint8_t> sab;
    TableArray sendEffects; // SndSendEffectParams (236 B): the reverbs' room types
    // The aliases' presets (stock map banks hold their own): DopplerPreset 88 B (+352),
    // OcclusionShape 100 B (+368), SpeakerMap 452 B (+464).
    TableArray dopplerPresets, occlusionShapes, speakerMaps;
    std::vector<Duck> ducks;
    SoundTable table;       // id 0 = none (SND_GetSoundTableById never selects the bank)
};

// Registers the StreamKey (type 102, resident data) and the always-loaded
// SndBankResident (type 21) that references it.
void RegisterBank(ZoneWriter &writer, const Bank &bank);
} // namespace iw8::sound
