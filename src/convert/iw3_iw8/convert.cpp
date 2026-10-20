#include <std_include.hpp>
#include <game.hpp>
#include <iw8/zone/ff_io.h>
#include <iw8/common/fs_util.h>
#include <iw8/common/log.h>
#include <iw8/dump/maps_convert.h>
#include <iw8/dump/dump_source.h>
#include <iw8/dump/image_dump.h>
#include <iw8/dump/material_dumpsrc.h>
#include <iw8/dump/xmodel_dump.h>
#include "iw3_fastfile.h"
#include "iw3_ledger.h"
#include "iw3_render_assets.h"
#include "iw3_vision.h"
#include "iw3_scripts.h"
#include <iw8/zone/iw8_ffheader.h>
#include <iw8/zone/iw8_zone.h>
#include <iw8/zone/map_zone.h>
#include <iw8/zone/maps_write.h>
#include <iw8/assets/replay_dynentity.h>
#include <iw8/common/replay_havok.h>
#include <iw8/assets/replay_heightfield.h>
#include <iw8/assets/replay_ies.h>
#include <iw8/assets/replay_locallights.h>
#include <iw8/assets/replay_xanim.h>
#include <iw8/assets/replay_impact.h>
#include <iw8/common/replay_navigation.h>
#include <iw8/assets/replay_rawfile.h>
#include <iw8/assets/replay_grading.h>
#include <iw8/common/replay_render.h>
#include <iw8/assets/replay_script.h>
#include <iw8/assets/replay_stock.h>
#include <iw8/assets/replay_vfx.h>
#include <iw8/assets/write_xsurface.h>
#include <iw8/assets/write_weapon.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <limits>
#include <memory>
#include <optional>
#include <string_view>
#include <regex>
#include <set>
#include <string>
#include <vector>

#include <Windows.h>

using namespace zt;

namespace zonetool::iw8
{
	const game& definition();
	xfile::fastfile rewrite_fastfile(const std::string& name, xfile::fastfile file);
	std::vector<std::int32_t> zone_list_types(const std::filesystem::path& path);
}

namespace
{
struct Args
{
    std::string command;
    std::vector<std::string> positional;
    std::string outputDirectory;
    std::string replayExecutable;
    std::string collisionPath;
    std::string footstepsPath;
    std::string unlinkerExecutable;
    std::vector<std::string> iw3Roots;
    std::vector<std::string> searchPaths;
    std::string metadataPath;
    std::string lightingProfile = "source";
    float sunIntensityScale = 1.0f;
    bool navigation = true;
    bool iw3ModeAssets = true;
    std::string style = "iw3";
    std::string gscTool;
    std::string actsExecutable;
    std::string stockCache;
    // An extraction that another front end wrote (iw3::ImportOptions::extraction).
    std::string extraction;
};

struct MapMetadata
{
    bool present{};
    std::string id;
    std::string title;
    std::string description;
    unsigned compassRotation{};
    bool compassRotationSet{};
    // Ladder rung spacing for the DW client (0 = Replay's 12).
    float ladderRungDistance{};
    // Contraction of the IW3 minimap region about its centre (see the compass
    // bounds below); build-time only, not written to the package map.json.
    // 1.0 = IW3's own squared region: Nuketown's art measures about 12 units per
    // pixel over it (playable strip 378 x 170 px against ~4500 x 2100 units of
    // playable geometry). The former 0.875, from one in-game point (fixes-25),
    // left the marker drifting (owner test 2026-10-14). In game the console dvar
    // mw120r_minimap_scale overrides it (see iw3compat_minimap).
    float minimapScale{1.0f};
};

bool isMapId(const std::string &map)
{
    if (map.size() < 4 || map.size() > 15 || !map.starts_with("mp_"))
    {
        return false;
    }
    return std::all_of(map.begin(), map.end(), [](const unsigned char character) {
        return character == '_' || (character >= 'a' && character <= 'z') ||
               (character >= '0' && character <= '9');
    });
}

bool requireMapId(const std::string &map)
{
    if (isMapId(map))
    {
        return true;
    }
    err("map id '%s' is invalid; use at most 15 lower-case characters in mp_<name>", map.c_str());
    return false;
}

std::array<std::string, 5> zoneNames(const std::string &map)
{
    return {map, "srv_" + map, "eng_" + map, "ww_" + map, "techsets_" + map};
}

std::vector<std::string> expectedFiles(const std::string &map)
{
    std::vector<std::string> files;
    for (const std::string &zone : zoneNames(map))
    {
        files.push_back(zone + ".ff");
    }
    files.emplace_back("map.json");
    std::sort(files.begin(), files.end());
    return files;
}

bool readMetadata(const std::string &path, MapMetadata &metadata)
{
    metadata = {};
    if (path.empty())
    {
        return true;
    }

    std::ifstream input(path);
    if (!input)
    {
        err("metadata: cannot read %s", path.c_str());
        return false;
    }

    try
    {
        const nlohmann::json data = nlohmann::json::parse(input);
        if (!data.is_object())
        {
            throw std::runtime_error("the root must be an object");
        }
        const auto readString = [&data](const char *key, std::string &value) {
            const auto item = data.find(key);
            if (item == data.end())
            {
                return true;
            }
            if (!item->is_string())
            {
                return false;
            }
            value = item->get<std::string>();
            return true;
        };
        std::string name;
        if (!readString("id", metadata.id) || !readString("title", metadata.title) ||
            !readString("name", name) || !readString("description", metadata.description))
        {
            throw std::runtime_error("id, name, title, and description must be strings");
        }
        if (!name.empty() && !metadata.title.empty() && name != metadata.title)
            throw std::runtime_error("name and title must match when both are present");
        if (metadata.title.empty())
            metadata.title = std::move(name);
        if (const auto rotation = data.find("compassRotation"); rotation != data.end())
        {
            if (!rotation->is_number_unsigned())
                throw std::runtime_error("compassRotation must be 0, 90, 180, or 270 clockwise");
            metadata.compassRotation = rotation->get<unsigned>();
            metadata.compassRotationSet = true;
            if (metadata.compassRotation != 0 && metadata.compassRotation != 90 &&
                metadata.compassRotation != 180 && metadata.compassRotation != 270)
                throw std::runtime_error("compassRotation must be 0, 90, 180, or 270 clockwise");
        }
        if (const auto rungs = data.find("ladderRungDistance"); rungs != data.end())
        {
            if (!rungs->is_number() || rungs->get<double>() < 8 || rungs->get<double>() > 32)
                throw std::runtime_error("ladderRungDistance must be a number from 8 to 32");
            metadata.ladderRungDistance = rungs->get<float>();
        }
        if (const auto scale = data.find("minimapScale"); scale != data.end())
        {
            if (!scale->is_number() || scale->get<double>() < 0.5 || scale->get<double>() > 1.5)
                throw std::runtime_error("minimapScale must be a number from 0.5 to 1.5");
            metadata.minimapScale = scale->get<float>();
        }
        metadata.present = true;
    }
    catch (const std::exception &exception)
    {
        err("metadata: %s is invalid: %s", path.c_str(), exception.what());
        return false;
    }
    return true;
}

bool validateMetadata(const std::string &directory, const std::string &map, bool &present)
{
    const std::filesystem::path path = std::filesystem::path(directory) / "map.json";
    present = std::filesystem::exists(path);
    if (!present)
    {
        err("metadata: package is missing map.json");
        return false;
    }

    MapMetadata metadata;
    if (!readMetadata(path.string(), metadata))
    {
        return false;
    }
    if (!metadata.id.empty() && metadata.id != map)
    {
        err("metadata: id '%s' does not match output map '%s'", metadata.id.c_str(), map.c_str());
        return false;
    }
    return true;
}

bool writeMetadata(const std::string &directory, const MapMetadata &metadata)
{
    nlohmann::json data = nlohmann::json::object();
    if (!metadata.id.empty())
    {
        data["id"] = metadata.id;
    }
    if (!metadata.title.empty())
    {
        data["title"] = metadata.title;
    }
    if (!metadata.description.empty())
    {
        data["description"] = metadata.description;
    }
    if (metadata.compassRotation)
    {
        data["compassRotation"] = metadata.compassRotation;
    }
    if (metadata.ladderRungDistance > 0.0f && metadata.ladderRungDistance != 12.0f)
    {
        data["ladderRungDistance"] = metadata.ladderRungDistance;
    }
    std::ofstream output(std::filesystem::path(directory) / "map.json", std::ios::binary);
    if (!output)
    {
        err("metadata: cannot write map.json");
        return false;
    }
    output << data.dump(2) << '\n';
    return output.good();
}

// Optional package file: the map's loading-screen artwork for the custom-map
// UI ("MWRI", version 1, width, height, then RGBA8; 1..2048 on each side).
constexpr char kPreviewFile[] = "preview.rgba";

bool validPreview(const std::filesystem::path &path)
{
    std::ifstream input(path, std::ios::binary | std::ios::ate);
    if (!input)
        return false;
    const auto size = static_cast<std::uint64_t>(input.tellg());
    input.seekg(0);
    std::uint32_t header[4]{};
    if (!input.read(reinterpret_cast<char *>(header), sizeof(header)) || header[0] != 0x4952574Du ||
        header[1] != 1 || header[2] < 1 || header[2] > 2048 || header[3] < 1 || header[3] > 2048)
        return false;
    return size == sizeof(header) + std::uint64_t(header[2]) * header[3] * 4;
}

bool prepareOutputDirectory(const std::string &outputDirectory, const std::string &map)
{
    std::error_code error;
    if (!std::filesystem::exists(outputDirectory, error))
    {
        return mkdirs(outputDirectory);
    }
    if (error || !std::filesystem::is_directory(outputDirectory, error))
    {
        err("build-map: output is not a directory: %s", outputDirectory.c_str());
        return false;
    }

    const std::vector<std::string> expected = expectedFiles(map);
    for (const auto &entry : std::filesystem::directory_iterator(outputDirectory, error))
    {
        if (error || !entry.is_regular_file(error) ||
            (std::find(expected.begin(), expected.end(), entry.path().filename().string()) ==
                 expected.end() &&
             entry.path().filename() != kPreviewFile))
        {
            err("build-map: output directory must be empty or contain only this map's files");
            return false;
        }
    }
    return !error;
}

// --writer iw8-zonetool writes each zone in the stream layout of the converter, for byte comparisons with
// iw8-zonetool builds.
bool g_converterLayout = false;

struct ZoneValidation
{
    uint32_t assetCount{};
    std::array<uint32_t, ASSET_TYPE_COUNT> typeCounts{};
};

bool validateZoneFile(const std::string &path, ZoneValidation *details = nullptr)
{
    std::vector<uint8_t> bytes;
    if (!read_file(path, bytes) || bytes.size() < 0x8C + sizeof(IW8_XAssetList))
    {
        err("validate: cannot read a complete IW8 header from %s", path.c_str());
        return false;
    }

    IW8_DB_FFHeader header{};
    std::memcpy(&header, bytes.data(), (std::min)(bytes.size(), sizeof(header)));
    if (std::memcmp(header.magic, iw8ff::kMagicUnsec, 8) != 0 ||
        header.headerVersion != iw8ff::kHeaderVersion ||
        header.xfileVersion != iw8ff::kXFileVersion || header.dashCompressBuild != 0 ||
        header.dashEncryptBuild != 0)
    {
        err("validate: %s has an unsupported Replay header", path.c_str());
        return false;
    }
    if (std::memcmp(bytes.data() + 0x88, "\x01IWC", 4) != 0 ||
        header.xfileHeader.size != bytes.size() - 0x8C ||
        header.residentPartSize != bytes.size() - 0x88 ||
        header.alwaysLoadedPartSize != header.xfileHeader.size)
    {
        err("validate: %s has invalid resident framing", path.c_str());
        return false;
    }

    // The converter layout declares the asset list header in block 0. The x64-zt writer declares it in block 1 with
    // the peak of the loader there, as most stock zones do.
    const auto &blocks = header.xfileHeader.blockSize;
    const bool listBlock = g_converterLayout ? blocks[iw8::XFILE_BLOCK_TEMP] == sizeof(IW8_XAssetList)
                                             : blocks[iw8::XFILE_BLOCK_TEMP] == 0 &&
                                                   blocks[iw8::XFILE_BLOCK_TEMP_PRELOAD] >= sizeof(IW8_XAssetList);
    if (header.transientFileType != 0 || header.residentHash != 0 ||
        header.xfileHeader.preloadWalkSize == 0 || !listBlock ||
        header.xfileHeader.blockSize[iw8::XFILE_BLOCK_CALLBACK] != 0 ||
        header.xfileHeader.blockSize[iw8::XFILE_BLOCK_SCRIPT] != 0 ||
        header.xfileHeader.blockSize[iw8::XFILE_BLOCK_UNK10] != 0)
    {
        err("validate: %s has invalid Replay stream/header policy", path.c_str());
        return false;
    }

    uint64_t streamTotal = 0;
    for (const uint64_t size : header.xfileHeader.blockSize)
    {
        if (size > (std::numeric_limits<uint64_t>::max)() - streamTotal)
        {
            err("validate: %s has overflowing stream sizes", path.c_str());
            return false;
        }
        streamTotal += size;
    }
    // The converter layout keeps every block byte. The x64-zt writer declares the temp block peak, as the loader
    // reuses it for each asset, so stock zones and its zones can declare fewer bytes than the zone data.
    if (g_converterLayout && streamTotal < header.xfileHeader.size)
    {
        err("validate: %s has invalid stream sizes", path.c_str());
        return false;
    }

    if (!g_converterLayout)
    {
        // The zones of the x64-zt writer must load with the IW8 zone loader, which reads every asset.
        std::vector<std::int32_t> types;
        try
        {
            types = zonetool::iw8::zone_list_types(path);
        }
        catch (const std::exception &e)
        {
            err("validate: %s does not load: %s", path.c_str(), e.what());
            return false;
        }

        std::array<uint32_t, ASSET_TYPE_COUNT> typeCounts{};
        for (const auto type : types)
        {
            if (type < 0 || type >= ASSET_TYPE_COUNT)
            {
                err("validate: %s has an invalid top-level asset type", path.c_str());
                return false;
            }
            ++typeCounts[type];
        }
        info("validate: %s loads with %zu top-level asset(s)", path.c_str(), types.size());
        if (details)
        {
            details->assetCount = static_cast<uint32_t>(types.size());
            details->typeCounts = typeCounts;
        }
        return true;
    }

    IW8_XAssetList root{};
    std::memcpy(&root, bytes.data() + 0x8C, sizeof(root));
    const bool hasStrings = root.stringList.count != 0;
    const bool hasAssets = root.assetCount != 0;
    if (root.stringList.count < 0 || root.stringList.loaded != 0 || root.assetReadPos != 0 ||
        root.stringList.strings !=
            (hasStrings ? iw8::PTR_FOLLOWS : iw8::PTR_NULL) ||
        root.assets != (hasAssets ? iw8::PTR_FOLLOWS : iw8::PTR_NULL) ||
        (hasAssets && root.assetCount >
                          header.xfileHeader.blockSize[iw8::XFILE_BLOCK_VIRTUAL] /
                              sizeof(IW8_XAsset)))
    {
        err("validate: %s has an invalid root asset list", path.c_str());
        return false;
    }

    size_t cursor = 0x8C + sizeof(root);
    if (hasStrings)
    {
        const size_t stringCount = static_cast<size_t>(root.stringList.count);
        if (stringCount > (bytes.size() - cursor) / sizeof(uint64_t))
        {
            err("validate: %s has a truncated script-string pointer list", path.c_str());
            return false;
        }
        for (size_t index = 0; index < stringCount; ++index)
        {
            uint64_t pointer{};
            std::memcpy(&pointer, bytes.data() + cursor, sizeof(pointer));
            cursor += sizeof(pointer);
            if (pointer != (index == 0 ? iw8::PTR_NULL : iw8::PTR_FOLLOWS))
            {
                err("validate: %s has an invalid script-string pointer", path.c_str());
                return false;
            }
        }
        for (size_t index = 1; index < stringCount; ++index)
        {
            const auto end = std::find(bytes.begin() + cursor, bytes.end(), uint8_t{});
            if (end == bytes.end())
            {
                err("validate: %s has an unterminated script string", path.c_str());
                return false;
            }
            cursor = static_cast<size_t>(end - bytes.begin()) + 1;
        }
    }

    if (root.assetCount > (bytes.size() - cursor) / sizeof(IW8_XAsset))
    {
        err("validate: %s has a truncated top-level asset list", path.c_str());
        return false;
    }
    std::array<uint32_t, ASSET_TYPE_COUNT> typeCounts{};
    for (uint32_t index = 0; index < root.assetCount; ++index)
    {
        IW8_XAsset asset{};
        std::memcpy(&asset, bytes.data() + cursor, sizeof(asset));
        cursor += sizeof(asset);
        if (asset.type < 0 || asset.type >= ASSET_TYPE_COUNT || asset._pad04 != 0 ||
            (asset.header != iw8::PTR_INSERT &&
             !(asset.type == ASSET_TYPE_LIGHTDEF && asset.header == iw8::PTR_FOLLOWS)))
        {
            err("validate: %s has an invalid top-level asset descriptor", path.c_str());
            return false;
        }
        ++typeCounts[asset.type];
    }
    std::string summary;
    for (size_t type = 0; type < typeCounts.size(); ++type)
    {
        if (!typeCounts[type])
            continue;
        if (!summary.empty())
            summary += ", ";
        const char *name = iw8sz::type_name(static_cast<int>(type));
        summary += std::strcmp(name, "unknown") == 0 ? "type" + std::to_string(type) : name;
        summary += '=';
        summary += std::to_string(typeCounts[type]);
    }
    info("validate: %s contains %u top-level asset(s)%s%s", path.c_str(), root.assetCount,
         summary.empty() ? "" : ": ", summary.c_str());
    if (details)
    {
        details->assetCount = root.assetCount;
        details->typeCounts = typeCounts;
    }
    return true;
}

bool validatePackage(const std::string &packageDirectory, const std::string &map)
{
    if (!requireMapId(map))
    {
        return false;
    }

    bool hasMetadata = false;
    if (!validateMetadata(packageDirectory, map, hasMetadata))
    {
        return false;
    }

    std::vector<std::string> actual;
    std::error_code error;
    for (const auto &entry : std::filesystem::directory_iterator(packageDirectory, error))
    {
        if (error || !entry.is_regular_file(error))
        {
            err("validate: package contains an unreadable or non-file entry");
            return false;
        }
        actual.push_back(entry.path().filename().string());
    }
    if (error)
    {
        err("validate: cannot enumerate %s", packageDirectory.c_str());
        return false;
    }
    std::sort(actual.begin(), actual.end());
    if (const auto preview = std::find(actual.begin(), actual.end(), kPreviewFile);
        preview != actual.end())
    {
        if (!validPreview(std::filesystem::path(packageDirectory) / kPreviewFile))
        {
            err("validate: preview.rgba is not a valid MWRI image");
            return false;
        }
        actual.erase(preview);
    }
    if (actual != expectedFiles(map))
    {
        err("validate: output must contain exactly five map fastfiles and map.json "
            "(and optionally preview.rgba)");
        return false;
    }

    const auto zones = zoneNames(map);
    std::array<ZoneValidation, 5> validation{};
    bool valid = true;
    for (size_t index = 0; index < zones.size(); ++index)
    {
        valid = validateZoneFile(path_join(packageDirectory, zones[index] + ".ff"),
                                 &validation[index]) &&
                valid;
    }
    if (valid)
    {
        const auto &main = validation[0].typeCounts;
        const auto &server = validation[1].typeCounts;
        const auto &techsets = validation[4].typeCounts;
        if (main[ASSET_TYPE_GFX_MAP] != 1 || main[ASSET_TYPE_GLASS_MAP] != 1 ||
            main[ASSET_TYPE_IMPACT_FX] != 1 || main[ASSET_TYPE_COL_MAP] != 1 ||
            main[ASSET_TYPE_COM_MAP] != 1 || main[ASSET_TYPE_MAP_ENTS] != 1 ||
            main[ASSET_TYPE_NET_CONST_STRINGS] != replayncs::Count ||
            std::any_of(server.begin(), server.end(), [](const auto count) { return count; }))
        {
            err("validate: package is missing a required native Replay map asset");
            valid = false;
        }
        if (!techsets[ASSET_TYPE_MATERIAL] ||
            main[ASSET_TYPE_MATERIAL] != techsets[ASSET_TYPE_MATERIAL] ||
            !techsets[ASSET_TYPE_TECHSET] ||
            !techsets[ASSET_TYPE_VERTEXSHADER] || !techsets[ASSET_TYPE_PIXELSHADER] ||
            techsets[ASSET_TYPE_IMAGE] || main[ASSET_TYPE_TECHSET] ||
            main[ASSET_TYPE_VERTEXSHADER] || main[ASSET_TYPE_PIXELSHADER])
        {
            err("validate: map render definitions must reside in the techset zone and images in the main zone");
            valid = false;
        }
        // eng_<map> carries the map's English strings (LocalizeEntry) and the
        // stock CODCaster image stock packs there; ww_<map> only the worldwide
        // CODCaster images (section 19).
        const auto &english = validation[2].typeCounts;
        const auto &worldwide = validation[3].typeCounts;
        if (validation[2].assetCount != english[ASSET_TYPE_LOCALIZE] + english[ASSET_TYPE_IMAGE] ||
            validation[3].assetCount != worldwide[ASSET_TYPE_IMAGE])
        {
            err("validate: eng_ may hold only localized strings and images, ww_ only images");
            valid = false;
        }
    }
    if (valid)
    {
        info("validate: map output '%s' passes structural checks", map.c_str());
    }
    return valid;
}

const char *defaultEntities =
    "{ 212 \"worldspawn\" }\n"
    "{ 212 \"info_player_start\" 709 \"0 0 64\" 80 \"0 0 0\" }\n"
    "{ 212 \"mp_tdm_spawn\" 709 \"0 0 64\" 80 \"0 0 0\" }\n"
    "{ 212 \"mp_tdm_spawn_allies_start\" 709 \"-64 0 64\" 80 \"0 0 0\" }\n"
    "{ 212 \"mp_tdm_spawn_axis_start\" 709 \"64 0 64\" 80 \"0 180 0\" }\n"
    "{ 212 \"script_model\" 709 \"0 0 16\" 80 \"0 0 0\" }\n";

struct AssetTally
{
    int materials{};
    int images{};
    int models{};
    int surfaces{};
    int failures{};
    std::string firstFailure;
    bool hasCompass{};
};

AssetTally addMapAssets(iw8::ZoneWriter &writer, iw8::ZoneWriter &techsets,
                        const std::string &dumpDirectory,
                        const dumpsrc::DumpSource &source, const std::string &map,
                        const unsigned compassRotation)
{
    AssetTally tally;
    for (const std::string &relativePath : source.listMaterials())
    {
        const std::string path = path_join(path_join(dumpDirectory, "materials"), relativePath);
        std::string materialName;
        if (convdump::mtl::emitMaterialFromDump(techsets, path, relativePath, &materialName))
        {
            replayrender::RegisterMaterialReference(writer, materialName);
            ++tally.materials;
        }
        else
        {
            ++tally.failures;
            if (tally.firstFailure.empty())
                tally.firstFailure = "material " + path;
        }
    }

    const std::string compassName = "compass_map_" + map;
    bool hasCompass = false;
    for (const std::string &name : dumpimg::listImageDumps(dumpDirectory))
    {
        const dumpimg::ImageDumpFile input = dumpimg::readImageDump(dumpDirectory, name);
        const unsigned rotation = input.name == compassName ? compassRotation : 0;
        if (!input.loaded || !dumpimg::addImageAsset(writer, dumpimg::convertImage(input, rotation)))
        {
            ++tally.failures;
            if (tally.firstFailure.empty())
                tally.firstFailure = "image " + name + " in " + dumpDirectory;
            continue;
        }
        ++tally.images;
        hasCompass = hasCompass || input.name == compassName;
    }
    if (hasCompass)
    {
        tally.hasCompass = true;
        if (!convdump::mtl::writeCompassMaterial(techsets, compassName))
        {
            ++tally.failures;
            if (tally.firstFailure.empty())
                tally.firstFailure = "compass material " + compassName;
        }
        else
        {
            replayrender::RegisterMaterialReference(writer, compassName);
        }
    }
    else
    {
        warn("assets: no images/%s.dds, .iwi, or .ffImg; HUD compass will use the default material",
             compassName.c_str());
    }

    std::vector<std::string> surfaceFiles;
    if (list_dir(path_join(dumpDirectory, "XSurface"), surfaceFiles))
    {
        for (const std::string &file : surfaceFiles)
        {
            if (!file.ends_with(".xse"))
            {
                continue;
            }
            const std::string name = file.substr(0, file.size() - 4);
            if (iw8xs_dump::writeXModelSurfsFromDump(writer, dumpDirectory, name))
            {
                ++tally.surfaces;
            }
            else
            {
                ++tally.failures;
                if (tally.firstFailure.empty())
                    tally.firstFailure = "XModelSurfs " + name + " in " + dumpDirectory;
            }
        }
    }

    // XModel bodies use packed references, so their XModelSurfs dependencies must precede them.
    for (const std::string &name : source.listXModels())
    {
        if (iw8::addXModelFromDump(writer, dumpDirectory, name))
        {
            ++tally.models;
        }
        else
        {
            ++tally.failures;
            if (tally.firstFailure.empty())
                tally.firstFailure = "XModel " + name + " in " + dumpDirectory;
        }
    }

    info("assets: %d materials, %d images, %d models, %d model surfaces, %d failures",
         tally.materials, tally.images, tally.models, tally.surfaces, tally.failures);
    return tally;
}

template <typename Buffer> Iw8WriteParams writeParams(const Buffer &buffer)
{
    Iw8WriteParams params;
    for (int index = 0; index < iw8::IW8_MAX_XFILE_COUNT; ++index)
    {
        params.blockSize[index] = buffer.blockSize(index);
        params.totalDecompressed += params.blockSize[index];
    }
    params.calcSize = buffer.calcSize();
    // Converted maps record ~2.5 plan entries of ~3.5 bytes per top-level asset
    // (Nuketown 19905 bytes / 2535 assets, Gulag 26469 / 2908).
    params.preloadWalkSize =
        (std::max)(params.preloadWalkSize, uint64_t(128) * buffer.assetCount());
    return params;
}

template <> Iw8WriteParams writeParams(const iw8::ZoneBuffer &buffer)
{
    Iw8WriteParams params;
    for (int index = 0; index < iw8::IW8_MAX_XFILE_COUNT; ++index)
    {
        params.blockSize[index] = buffer.streamSize(index);
        params.totalDecompressed += params.blockSize[index];
    }
    params.calcSize = buffer.calcSize();
    return params;
}

// Writes a zone that the converter built: the x64-zt zone writer lays it out again and the core container stores it.
bool writeZone(const std::string &outputPath, const std::vector<uint8_t> &body, const Iw8WriteParams &params)
{
    if (g_converterLayout)
        return iw8_write(outputPath, body, params);

    try
    {
        zonetool::xfile::fastfile file{};
        file.payload.assign(body.begin(), body.end());
        file.block_sizes.assign(std::begin(params.blockSize), std::end(params.blockSize));
        file.xfile_version = params.xfileVersion;
        file.transient = params.transientFileType != 0;
        file.preload_walk_size = params.preloadWalkSize;
        const auto written = zonetool::iw8::rewrite_fastfile(
            std::filesystem::path(outputPath).stem().string(), std::move(file));
        zonetool::xfile::write(outputPath, zonetool::iw8::definition().format, written);
        info("iw8: wrote %s (%zu bytes of zone data)", outputPath.c_str(), written.payload.size());
        // Stock zones stay under 1.1 GB and IW7 Frontier (2.06 GB) loads, but nothing above 2 GiB
        // has been checked: block sizes and offsets may pass through signed 32-bit values in the loader.
        if (written.payload.size() > 0x7FFFFFFFull)
            warn("iw8: %s holds %zu bytes of zone data, more than 2 GiB: no zone that large has loaded in Replay; "
                 "--detail_collision 0 saves about 3%%, but bullets then miss models near the player",
                 outputPath.c_str(), written.payload.size());
        return true;
    }
    catch (const std::exception &e)
    {
        err("iw8: cannot write %s: %s", outputPath.c_str(), e.what());
        return false;
    }
}

iw8::MapSun loadLighting(const Args &args, const std::string &dumpDirectory,
                         const std::string &assetName)
{
    if (args.lightingProfile != "source" && args.lightingProfile != "aniyah-incursion")
    {
        throw std::runtime_error("unknown lighting profile: " + args.lightingProfile);
    }

    iw8::MapSun lighting;
    if (args.lightingProfile == "source")
    {
        const std::string path = path_join(dumpDirectory, assetName + ".lighting.json");
        if (!file_exists(path))
        {
            throw std::runtime_error("source lighting profile needs .lighting.json");
        }

        std::ifstream input(path);
        const nlohmann::json data = nlohmann::json::parse(input);
        if (data.at("schema") != 1)
        {
            throw std::runtime_error("invalid map lighting schema");
        }
        lighting.intensity = data.at("intensity").get<float>();
        lighting.lightUnitScale = data.value("light_unit_scale", 1.0f);

        float directionLength = 0.0f;
        float upLength = 0.0f;
        float dot = 0.0f;
        for (size_t index = 0; index < 3; ++index)
        {
            lighting.color[index] = data.at("color").at(index).get<float>();
            lighting.direction[index] = data.at("direction").at(index).get<float>();
            lighting.up[index] = data.at("up").at(index).get<float>();
            if (!std::isfinite(lighting.color[index]) || lighting.color[index] < 0.0f)
            {
                throw std::runtime_error("invalid sun color");
            }
            directionLength += lighting.direction[index] * lighting.direction[index];
            upLength += lighting.up[index] * lighting.up[index];
            dot += lighting.direction[index] * lighting.up[index];
        }
        if (!std::isfinite(lighting.intensity) || lighting.intensity < 0.0f ||
            !std::isfinite(directionLength + upLength + dot) ||
            std::abs(directionLength - 1.0f) > 0.001f ||
            (upLength != 0.0f && std::abs(upLength - 1.0f) > 0.001f) || std::abs(dot) > 0.001f)
        {
            throw std::runtime_error("invalid sun lighting data");
        }

        if (data.contains("primary_lights"))
        {
            const auto &sourceLights = data.at("primary_lights");
            if (!sourceLights.is_array() || sourceLights.size() < 2 || sourceLights.size() > 65535)
            {
                throw std::runtime_error("invalid source primary-light array");
            }
            lighting.sunPrimaryLightIndex = data.at("sun_primary_light_index").get<uint32_t>();
            if (lighting.sunPrimaryLightIndex >= sourceLights.size())
            {
                throw std::runtime_error("invalid source sun primary-light index");
            }

            const auto readVector = [](const nlohmann::json &value, const char *name) {
                if (!value.is_array() || value.size() != 3)
                    throw std::runtime_error(std::string("invalid primary-light ") + name);
                std::array<float, 3> result{};
                for (size_t component = 0; component < result.size(); ++component)
                {
                    result[component] = value.at(component).get<float>();
                    if (!std::isfinite(result[component]))
                        throw std::runtime_error(std::string("invalid primary-light ") + name);
                }
                return result;
            };

            lighting.primaryLights.reserve(sourceLights.size());
            for (const auto &source : sourceLights)
            {
                iw8::MapSun::PrimaryLight light;
                const unsigned type = source.at("type").get<unsigned>();
                const unsigned exponent = source.at("exponent").get<unsigned>();
                if (type > 3 || exponent > 255)
                    throw std::runtime_error("invalid IW3 primary-light type or exponent");
                light.type = static_cast<uint8_t>(type);
                light.exponent = static_cast<uint8_t>(exponent);
                light.canUseShadowMap = source.at("can_use_shadow_map").get<bool>();

                const auto color = readVector(source.at("color"), "color");
                const auto direction = readVector(source.at("direction"), "direction");
                const auto origin = readVector(source.at("origin"), "origin");
                if (std::ranges::any_of(color, [](const float value) { return value < 0.0f; }))
                    throw std::runtime_error("invalid primary-light color");
                std::copy(color.begin(), color.end(), light.color);
                std::copy(origin.begin(), origin.end(), light.origin);

                const float sourceDirectionLength =
                    std::sqrt(direction[0] * direction[0] + direction[1] * direction[1] +
                              direction[2] * direction[2]);
                if (type != 0 && sourceDirectionLength <= 0.000001f)
                    throw std::runtime_error("primary-light direction has zero length");
                if (sourceDirectionLength > 0.000001f)
                    for (size_t component = 0; component < direction.size(); ++component)
                        light.direction[component] = direction[component] / sourceDirectionLength;

                if (type >= 2)
                {
                    const std::array<float, 3> reference =
                        std::abs(light.direction[2]) < 0.999f
                            ? std::array<float, 3>{0.0f, 0.0f, 1.0f}
                            : std::array<float, 3>{0.0f, 1.0f, 0.0f};
                    const float projection = reference[0] * light.direction[0] +
                                             reference[1] * light.direction[1] +
                                             reference[2] * light.direction[2];
                    float sourceUpLength = 0.0f;
                    for (size_t component = 0; component < reference.size(); ++component)
                    {
                        light.up[component] =
                            reference[component] - projection * light.direction[component];
                        sourceUpLength += light.up[component] * light.up[component];
                    }
                    sourceUpLength = std::sqrt(sourceUpLength);
                    for (float &component : light.up)
                        component /= sourceUpLength;
                    // Sources with an authored up axis (IW7) keep its roll: it orients the spot's shadow
                    // map. IW7 does not keep it orthogonal to the direction, so it is made so.
                    if (source.value("authored_up", false))
                    {
                        const auto authored = readVector(source.at("up"), "up");
                        float along = 0.0f;
                        for (size_t component = 0; component < 3; ++component)
                            along += authored[component] * light.direction[component];
                        std::array<float, 3> up{};
                        float length = 0.0f;
                        for (size_t component = 0; component < 3; ++component)
                        {
                            up[component] = authored[component] - along * light.direction[component];
                            length += up[component] * up[component];
                        }
                        length = std::sqrt(length);
                        if (std::isfinite(length) && length > 1e-3f)
                            for (size_t component = 0; component < 3; ++component)
                                light.up[component] = up[component] / length;
                    }
                }

                light.radius = source.at("radius").get<float>();
                light.cosHalfFovOuter = source.at("cos_half_fov_outer").get<float>();
                light.cosHalfFovInner = source.at("cos_half_fov_inner").get<float>();
                light.rotationLimit = source.at("rotation_limit").get<float>();
                light.translationLimit = source.at("translation_limit").get<float>();
                light.entityId = source.value("entity_id", 0u);
                // Light shape and shadow parameters of sources that have them (IW7).
                light.bulbRadius = source.value("bulb_radius", light.bulbRadius);
                if (const auto length = source.find("bulb_length"); length != source.end())
                    for (size_t component = 0; component < 3; ++component)
                        light.bulbLength[component] = length->at(component).get<float>();
                light.isVolumetric = source.value("volumetric", light.isVolumetric);
                light.needsDynamicShadows = source.value("needs_dynamic_shadows", light.needsDynamicShadows);
                light.shadowSoftness = source.value("shadow_softness", light.shadowSoftness);
                light.shadowBias = source.value("shadow_bias", light.shadowBias);
                light.shadowArea = source.value("shadow_area", light.shadowArea);
                light.distanceFalloff = source.value("distance_falloff", light.distanceFalloff);
                if (const auto fade = source.find("fade_offset"); fade != source.end())
                    for (size_t component = 0; component < 2; ++component)
                        light.fadeOffset[component] = fade->at(component).get<float>();
                light.uvIntensity = source.value("uv_intensity", light.uvIntensity);
                light.irIntensity = source.value("ir_intensity", light.irIntensity);
                if (!std::isfinite(light.bulbRadius) || light.bulbRadius <= 0.0f || !std::isfinite(light.shadowBias) ||
                    !std::isfinite(light.distanceFalloff) || light.distanceFalloff <= 0.0f)
                    throw std::runtime_error("invalid primary-light shape data");
                light.definition = source.at("definition").get<std::string>();
                // Converters of engines with physical light units give each local light its intensity.
                if (const auto intensity = source.find("intensity"); intensity != source.end())
                {
                    light.intensity = intensity->get<float>();
                    light.authoredIntensity = true;
                    if (!std::isfinite(light.intensity) || light.intensity < 0.0f)
                        throw std::runtime_error("invalid primary-light intensity");
                }
                if (!std::isfinite(light.radius) || light.radius < 0.0f ||
                    !std::isfinite(light.cosHalfFovOuter) ||
                    !std::isfinite(light.cosHalfFovInner) || !std::isfinite(light.rotationLimit) ||
                    !std::isfinite(light.translationLimit) || light.definition.size() > 255)
                {
                    throw std::runtime_error("invalid primary-light scalar data");
                }
                if (type == 2 && (light.cosHalfFovOuter <= 0.0f ||
                                  light.cosHalfFovOuter >= light.cosHalfFovInner ||
                                  light.cosHalfFovInner > 1.0f))
                {
                    throw std::runtime_error("invalid IW3 spot-light field of view");
                }
                lighting.primaryLights.push_back(std::move(light));
            }
        }
    }

    if (lighting.primaryLights.empty())
        lighting.primaryLights.resize(2);

    lighting.intensity *= args.sunIntensityScale;
    if (!std::isfinite(lighting.intensity))
    {
        throw std::runtime_error("scaled sun intensity is not finite");
    }
    info("lighting: %s, scale %.6f, native intensity %.6f", args.lightingProfile.c_str(),
         args.sunIntensityScale, lighting.intensity);
    return lighting;
}

// A shadowed spot inside its fixture (IW7 puts lights in their lamp housings and behind light
// panels) has its shadow map covered by the fixture beyond the shadow near plane (sub_141985070
// places it shadowNearPlaneBias, 8 for converted lights, in front of the light): the light then
// goes dark whenever it gets a shadow map, which Replay gives the spots near the camera, and
// IW7 Frontier's lamps switched off as the owner came close or turned (2026-10-18). Each shadowed
// spot's shadow map starts past the casters within 32 units of it; a spot whose cone the casters
// beyond that close within 64 units on most rays keeps its light without a shadow map.
void dropEnclosedShadowedSpots(iw8::MapSun &lighting, const std::vector<float> &solidTriangles,
                               const replaysunshadow::Scene *casters)
{
    constexpr double kNearPlane = 8.0; // emitComWorldBody's default shadowNearPlaneBias
    constexpr double kFixtureZone = 32.0;
    constexpr double kReach = 64.0;
    std::vector<float> triangles = solidTriangles;
    if (casters)
        for (const auto &surface : casters->surfaces)
            for (const auto &vertex : surface.vertices)
                triangles.insert(triangles.end(), vertex.position.begin(), vertex.position.end());
    size_t dropped = 0, moved = 0;
    for (size_t index = 0; index < lighting.primaryLights.size(); ++index)
    {
        auto &light = lighting.primaryLights[index];
        if (light.type != 2 || !light.canUseShadowMap || index == lighting.sunPrimaryLightIndex)
            continue;
        const double o[3]{light.origin[0], light.origin[1], light.origin[2]};
        const double reach = std::min<double>(light.radius, kReach);
        std::vector<size_t> candidates;
        for (size_t t = 0; t + 9 <= triangles.size(); t += 9)
        {
            bool outside = false;
            for (size_t axis = 0; axis < 3 && !outside; ++axis)
            {
                const float *v = &triangles[t];
                const double low = std::min({v[axis], v[3 + axis], v[6 + axis]});
                const double high = std::max({v[axis], v[3 + axis], v[6 + axis]});
                outside = low > o[axis] + reach || high < o[axis] - reach;
            }
            if (!outside)
                candidates.push_back(t);
        }
        // Emission axis = -ComPrimaryLight dir; rays on the axis and on rings of the cone.
        const double axis[3]{-light.direction[0], -light.direction[1], -light.direction[2]};
        // Across the axis: z x axis, or y x axis for a vertical axis.
        double u[3]{-axis[1], axis[0], 0.0};
        if (std::abs(axis[2]) >= 0.999)
        {
            u[0] = axis[2];
            u[1] = 0.0;
            u[2] = -axis[0];
        }
        const double length = std::sqrt(u[0] * u[0] + u[1] * u[1] + u[2] * u[2]);
        for (auto &c : u)
            c /= length;
        const double v[3]{axis[1] * u[2] - axis[2] * u[1], axis[2] * u[0] - axis[0] * u[2],
                          axis[0] * u[1] - axis[1] * u[0]};
        const double outer = std::acos(std::clamp<double>(light.cosHalfFovOuter, -1.0, 1.0));
        // The distances along the axis of every caster hit of each ray within reach.
        std::vector<std::vector<double>> hits;
        for (const double fraction : {0.0, 0.3, 0.6, 0.9})
            for (unsigned k = 0; k < (fraction == 0.0 ? 1u : 8u); ++k)
            {
                const double polar = outer * fraction, azimuth = 2.0 * std::numbers::pi * k / 8.0;
                double d[3];
                for (unsigned c = 0; c < 3; ++c)
                    d[c] = std::cos(polar) * axis[c] +
                           std::sin(polar) * (std::cos(azimuth) * u[c] + std::sin(azimuth) * v[c]);
                auto &ray = hits.emplace_back();
                for (const size_t t : candidates)
                {
                    const float *p = &triangles[t];
                    const double e1[3]{p[3] - p[0], p[4] - p[1], p[5] - p[2]};
                    const double e2[3]{p[6] - p[0], p[7] - p[1], p[8] - p[2]};
                    const double h[3]{d[1] * e2[2] - d[2] * e2[1], d[2] * e2[0] - d[0] * e2[2],
                                      d[0] * e2[1] - d[1] * e2[0]};
                    const double det = e1[0] * h[0] + e1[1] * h[1] + e1[2] * h[2];
                    if (std::abs(det) < 1e-9)
                        continue;
                    const double s[3]{o[0] - p[0], o[1] - p[1], o[2] - p[2]};
                    const double a = (s[0] * h[0] + s[1] * h[1] + s[2] * h[2]) / det;
                    if (a < 0.0 || a > 1.0)
                        continue;
                    const double q[3]{s[1] * e1[2] - s[2] * e1[1], s[2] * e1[0] - s[0] * e1[2],
                                      s[0] * e1[1] - s[1] * e1[0]};
                    const double b = (d[0] * q[0] + d[1] * q[1] + d[2] * q[2]) / det;
                    if (b < 0.0 || a + b > 1.0)
                        continue;
                    const double distance = (e2[0] * q[0] + e2[1] * q[1] + e2[2] * q[2]) / det;
                    if (distance > 0.0 && distance <= reach)
                        ray.push_back(distance * std::cos(polar));
                }
            }
        // The fixture: casters within kFixtureZone of the light along its axis (housings, the wall a
        // lamp is mounted on). The shadow map starts past them, as shipped lights' biases (8 to 60) do.
        double nearPlane = kNearPlane;
        for (const auto &ray : hits)
            for (const double hit : ray)
                if (hit <= kFixtureZone)
                    nearPlane = std::max(nearPlane, std::min(hit + 1.0, kFixtureZone));
        unsigned blocked = 0;
        for (const auto &ray : hits)
            blocked += std::ranges::any_of(ray, [&](const double hit) { return hit > nearPlane; });
        const auto rays = static_cast<unsigned>(hits.size());
        if (blocked * 5 >= rays * 3)
        {
            light.canUseShadowMap = false;
            ++dropped;
            info("local light %zu: shadow casters close %u of %u cone rays within %.0f units; no shadow map",
                 index, blocked, rays, reach);
        }
        else if (nearPlane > kNearPlane)
        {
            light.shadowNearPlaneBias = static_cast<float>(nearPlane);
            ++moved;
        }
    }
    if (moved)
        info("local lights: %zu shadowed spot(s) start their shadow maps past their fixture", moved);
    if (dropped)
        info("local lights: %zu shadowed spot(s) enclosed by their fixture keep no shadow map", dropped);
}

// Replay picks the shadowed spots of each frame (sub_141959080): R_AddPotentiallyShadowedLight
// keeps the best scores of the spots in the view cone (radius * intensity / distance) in a list
// of at most 64 (0x10C794F4, 0x40 at init) less the dynamic lights. Each listed spot holds one of
// 64 shadow-map cache entries (0x11740FA0, 0x70 bytes, per-light entry byte at +0x1C18), and every
// entry is in the frame's shadow list with its atlas slot (sub_1419860D0), rendered or not. A spot
// whose entry is not valid is rendered only when it is among the first 16 of the score order (an
// int dvar of default and maximum 16, 0x1055A448); a later one is deferred (flag 0x400) and its
// entry stays invalid, but it keeps its slot in the list: the shader samples a slot that holds no
// shadow of that light (inferred from the code; the slot contents were not traced). Every Umbra
// object of a converted world spans the scene, so every shadowed spot in the view cone competes,
// walls or not: with more than 16 shadowed spots the ones past the 16th went dark or lit as the view
// turned (owner tests of IW7 Frontier, 40 of 90 kept). Keep 16, the most important; the others
// keep their light without a shadow map.
void limitShadowedSpotLights(iw8::MapSun &lighting)
{
    constexpr size_t kMaxShadowedSpots = 16;
    std::vector<size_t> shadowed;
    for (size_t index = 0; index < lighting.primaryLights.size(); ++index)
    {
        const auto &light = lighting.primaryLights[index];
        if (light.type == 2 && light.canUseShadowMap && index != lighting.sunPrimaryLightIndex)
            shadowed.push_back(index);
    }
    if (shadowed.size() <= kMaxShadowedSpots)
        return;
    const auto importance = [&](const size_t index) {
        const auto &light = lighting.primaryLights[index];
        const double luminance =
            0.2126 * light.color[0] + 0.7152 * light.color[1] + 0.0722 * light.color[2];
        return double(light.intensity) * luminance * double(light.radius) * light.radius;
    };
    // Spots that shadow entities come first (IW7 marks them per light; IW3's all are): the
    // players' shadows need them.
    std::ranges::stable_sort(shadowed, [&](const size_t a, const size_t b) {
        const bool dynamicA = lighting.primaryLights[a].needsDynamicShadows;
        const bool dynamicB = lighting.primaryLights[b].needsDynamicShadows;
        if (dynamicA != dynamicB)
            return dynamicA;
        return importance(a) > importance(b);
    });
    for (size_t rank = kMaxShadowedSpots; rank < shadowed.size(); ++rank)
        lighting.primaryLights[shadowed[rank]].canUseShadowMap = false;
    info("local lights: shadow maps for %zu of %zu shadowed spots (Replay's per-frame budget)",
         kMaxShadowedSpots, shadowed.size());
}

// IW3 lights a surface with diffuse = color * r_diffuseColorScale (1) * falloff(d / radius),
// in the units of the IW3 sun the world keeps (R_SetLightProperties; light_point_linear
// samples falloff_linear = 1 - d/r). Replay's record builder (0x19A9020) turns ComPrimaryLight
// intensity into sphere-light radiance intensity * color / bulb^2, and the stock forward+ shader
// applies the sphere form factor ~ N.L * bulb^2 / d^2: irradiance = intensity * color * N.L / d^2,
// inside a window that is 1 up to 80% of the radius (1 / distance falloff 0.2). Each IW3 spot or
// omni gets the intensity that gives IW3's brightness where its axis meets the world (a spot's
// lit surface), capped at half its radius.
void calibrateLocalLights(iw8::MapSun &lighting, const std::vector<float> &solidTriangles)
{
    for (size_t index = 0; index < lighting.primaryLights.size(); ++index)
    {
        auto &light = lighting.primaryLights[index];
        if (light.type < 2 || index == lighting.sunPrimaryLightIndex || light.radius <= 0.0f)
            continue;
        if (light.authoredIntensity)
        {
            light.intensity *= lighting.lightUnitScale;
            continue;
        }
        if (light.definition != "light_point_linear")
            warn("local light %zu: IW3 light def '%s' falloff assumed linear", index,
                 light.definition.c_str());
        double reference = light.radius * 0.5;
        if (light.type == 2)
        {
            // Emission axis = -ComPrimaryLight dir. Nearest triangle hit (Moller-Trumbore).
            const double o[3]{light.origin[0], light.origin[1], light.origin[2]};
            const double d[3]{-light.direction[0], -light.direction[1], -light.direction[2]};
            double nearest = reference;
            for (size_t t = 0; t + 9 <= solidTriangles.size(); t += 9)
            {
                const float *v = &solidTriangles[t];
                const double e1[3]{v[3] - v[0], v[4] - v[1], v[5] - v[2]};
                const double e2[3]{v[6] - v[0], v[7] - v[1], v[8] - v[2]};
                const double p[3]{d[1] * e2[2] - d[2] * e2[1], d[2] * e2[0] - d[0] * e2[2],
                                  d[0] * e2[1] - d[1] * e2[0]};
                const double det = e1[0] * p[0] + e1[1] * p[1] + e1[2] * p[2];
                if (std::abs(det) < 1e-9)
                    continue;
                const double s[3]{o[0] - v[0], o[1] - v[1], o[2] - v[2]};
                const double u = (s[0] * p[0] + s[1] * p[1] + s[2] * p[2]) / det;
                if (u < 0.0 || u > 1.0)
                    continue;
                const double q[3]{s[1] * e1[2] - s[2] * e1[1], s[2] * e1[0] - s[0] * e1[2],
                                  s[0] * e1[1] - s[1] * e1[0]};
                const double w = (d[0] * q[0] + d[1] * q[1] + d[2] * q[2]) / det;
                if (w < 0.0 || u + w > 1.0)
                    continue;
                const double distance = (e2[0] * q[0] + e2[1] * q[1] + e2[2] * q[2]) / det;
                if (distance > 1.0 && distance < nearest)
                    nearest = distance;
            }
            reference = nearest;
        }
        reference = std::max(reference, 16.0);
        const double falloff = 1.0 - reference / light.radius;
        light.intensity = static_cast<float>(falloff * reference * reference) * lighting.lightUnitScale;
        info("local light %zu: IW3 color %.3f %.3f %.3f, reference %.1f of radius %.0f -> intensity %.1f",
             index, light.color[0], light.color[1], light.color[2], reference, light.radius,
             light.intensity);
    }
}

iw8::havok::BakeResult loadCollision(const Args &args, const std::string &dumpDirectory,
                                     const std::string &assetName)
{
    if (!args.collisionPath.empty())
    {
        if (args.replayExecutable.empty())
        {
            throw std::runtime_error("--replay is required with --collision");
        }
        return iw8::havok::BakeCollision(
            {args.replayExecutable, args.collisionPath, args.footstepsPath});
    }

    iw8::havok::BakeResult collision;
    const std::string path = path_join(dumpDirectory, assetName + ".havok");
    if (!read_file(path, collision.world) || collision.world.size() < 16 ||
        collision.world.size() > 256 * 1024 * 1024 ||
        std::memcmp(collision.world.data() + 4, "TAG0", 4) != 0)
    {
        throw std::runtime_error("native collision is missing; pass --replay and --collision");
    }
    return collision;
}

// Output side of the per-asset ledger (checklist section 20): every asset the
// map's zones register, with where it came from. Origins come from the render
// materials (their IW3 source, textures, techsets and shaders), the prepared
// IW3 models, effects and other prepared data, and fixed rules for the world and
// generated assets. Anything else is UNEXPLAINED.
struct OutputLedgerRow
{
    std::string zone, type, name, origin;
};

std::vector<OutputLedgerRow> outputLedgerRows(
    const std::string &zone, const std::vector<std::pair<IW8_XAssetType, std::string>> &assets,
    const replayrender::Mesh &mesh, const iw3::PreparedMap *prepared, const std::string &map,
    const iw8::stock::Copier *stock)
{
    std::map<std::pair<int, std::string>, std::string> origins;
    const auto set = [&](const int type, const std::string &name, const std::string &origin) {
        if (!name.empty())
            origins.emplace(std::pair{type, name}, origin);
    };
    const auto addMaterial = [&](const replayrender::Material &m) {
        const std::string what = m.material + (m.source.empty() ? std::string(" (generated variant)")
                                                                : " (IW3 " + m.source + ")");
        set(ASSET_TYPE_MATERIAL, m.material,
            m.source.empty() ? "generated material variant" : "IW3 material " + m.source);
        set(ASSET_TYPE_TECHSET, m.techset, "techset of material " + what);
        for (const auto &shader : m.shaders)
            set(shader.type, shader.name, "shader of material " + what);
        for (const auto &image : m.imageDefinitions)
            set(ASSET_TYPE_IMAGE, image.name, "texture of material " + what);
        set(ASSET_TYPE_DECAL_VOLUME_MATERIAL, m.decalVolumeMaterial, "decal volume of material " + what);
    };
    addMaterial(mesh);
    for (const auto &m : mesh.additionalMaterials)
        addMaterial(m);
    for (const auto &m : mesh.assetMaterials)
        addMaterial(m);
    set(ASSET_TYPE_IMAGE, mesh.reflectionProbeArrayImage.name, "IW3 reflection probes (GfxWorld)");
    for (const auto &name : mesh.stockMaterials)
        set(ASSET_TYPE_MATERIAL, name,
            "name-only reference to the stock common-zone material (loaded with the common zones)");
    if (prepared)
    {
        for (const auto &model : prepared->xmodels)
        {
            const std::string origin = model.role + " from IW3 model " + model.source;
            set(ASSET_TYPE_XMODEL, model.model.name, origin);
            for (const auto &lod : model.lods)
                set(ASSET_TYPE_XMODELSURFS, lod.name, "LOD of " + origin);
            set(ASSET_TYPE_PHYSICSASSET, model.physicsAsset.name, "physics of " + origin);
            set(ASSET_TYPE_XMODEL_DETAIL_COLLISION, model.detailCollision.name, "detail collision of " + origin);
        }
        for (const auto &effect : prepared->vfxEffects)
            set(ASSET_TYPE_VFX, effect.name, "IW3 effect");
        for (const auto &anim : prepared->xanims)
            set(ASSET_TYPE_XANIM, anim.name, "IW3 xanim");
        for (const auto &raw : prepared->rawFiles)
            set(ASSET_TYPE_RAWFILE, raw.name,
                raw.name.starts_with("vision/") ? "IW3 vision (converted, layer 17)"
                : raw.name.starts_with("sun/")  ? "IW3 sun (layer 17)"
                                                : "IW3 map-local raw file");
        if (prepared->soundBank)
            set(ASSET_TYPE_SOUNDBANK, prepared->soundBank->name, "IW3 sound aliases (map bank)");
        if (!prepared->gradingClutName.empty())
        {
            set(ASSET_TYPE_GRADINGCLUT, prepared->gradingClutName, "IW3 film grade (layer 17)");
            set(ASSET_TYPE_IMAGE, "gradingclut" + prepared->gradingClutName, "IW3 film grade (layer 17)");
        }
        if (!prepared->vfxLightDef.empty())
            set(ASSET_TYPE_LIGHTDEF, prepared->vfxLightDef, "light of IW3 effects / local lights");
        for (const auto &shape : prepared->vfxShapes)
            set(ASSET_TYPE_PHYSICSFXSHAPE, shape.name, "debris shape of a native effect (native Havok)");
        for (const auto &[type, name] : prepared->vfxReferences)
            set(type, name, "name-only reference to the common-zone asset (debris of a native effect)");
    }
    if (stock)
        for (const auto &[key, origin] : stock->Origins())
            origins.emplace(key, origin);

    std::vector<OutputLedgerRow> rows;
    for (const auto &[type, name] : assets)
    {
        std::string origin;
        if (const auto found = origins.find({static_cast<int>(type), name}); found != origins.end())
            origin = found->second;
        else
            switch (type)
            {
            case ASSET_TYPE_GFX_MAP:
            case ASSET_TYPE_GFX_MAP_TRZONE:
            case ASSET_TYPE_COL_MAP:
            case ASSET_TYPE_COM_MAP:
            case ASSET_TYPE_GLASS_MAP:
            case ASSET_TYPE_FX_MAP:
            case ASSET_TYPE_MAP_ENTS:
                origin = "IW3 world (gfxworld, clipmap, comworld, mapents)";
                break;
            case ASSET_TYPE_AIPATHS:
            case ASSET_TYPE_NAVMESH:
            case ASSET_TYPE_TACGRAPH:
                origin = "bot navigation generated from the converted collision";
                break;
            case ASSET_TYPE_EDGELIST:
                origin = "IW3 ladders (clipmap)";
                break;
            case ASSET_TYPE_DYNENTITYLIST:
                origin = "IW3 dynamic entities";
                break;
            case ASSET_TYPE_IMPACT_FX:
                origin = "impact table (IW3 impacts or Replay's, per --style)";
                break;
            case ASSET_TYPE_NET_CONST_STRINGS:
                origin = "level string tables (models, effects, anims the zone uses)";
                break;
            case ASSET_TYPE_SCRIPTFILE:
                origin = "IW3 map GSC, translated";
                break;
            case ASSET_TYPE_LOCALIZE:
                origin = "map-intro chyron (generated, section 19)";
                break;
            case ASSET_TYPE_STREAMKEY:
                if (prepared && prepared->soundBank)
                    origin = "IW3 sound aliases (map bank SAB)";
                break;
            case ASSET_TYPE_RAWFILE:
                if (name == map || name == map + "_load")
                    origin = "zone marker";
                break;
            case ASSET_TYPE_LIGHTDEF:
                if (name == "flashlight" || name == "light_ies_nvg_flashlight")
                    origin = "stock IES light (section 19)";
                else if (name == "light_point_linear")
                    origin = "IW3 local lights (comworld)";
                break;
            case ASSET_TYPE_IMAGE:
                if (name == iw8::heightfield::kHeightmapName || name == iw8::heightfield::kOutdoorName)
                    origin = "generated from the converted world (heightfield)";
                else if (name == iw8::ies::kLookupName)
                    origin = "stock IES profiles (section 19)";
                else if (name == "compass_map_" + map)
                    origin = "IW3 minimap image";
                break;
            case ASSET_TYPE_MATERIAL:
                if (name == "compass_map_" + map)
                    origin = "minimap material for the IW3 minimap image";
                break;
            case ASSET_TYPE_PHYSICSASSET:
                if (name.starts_with("mw120r_glasschunk"))
                    origin = "glass shard physics (IW3 glass)";
                break;
            default:
                break;
            }
        rows.push_back({zone, iw8sz::type_name(type), name, origin.empty() ? "UNEXPLAINED" : origin});
    }
    return rows;
}

void writeOutputLedger(const std::vector<OutputLedgerRow> &rows)
{
    std::filesystem::path path = iw3::ledger::Report();
    if (path.empty())
        return;
    std::string text = path.string();
    if (text.ends_with(".iw3-assets.tsv"))
        text.replace(text.size() - 15, 15, ".iw8-assets.tsv");
    std::ofstream output(text, std::ios::binary | std::ios::trunc);
    output << "zone\ttype\tname\torigin\n";
    std::map<std::string, std::size_t> unexplained;
    std::map<std::pair<std::string, std::string>, std::size_t> counts;
    for (const auto &row : rows)
    {
        output << row.zone << '\t' << row.type << '\t' << row.name << '\t' << row.origin << '\n';
        ++counts[{row.zone, row.type}];
        if (row.origin == "UNEXPLAINED")
            ++unexplained[row.type];
    }
    info("build-map: output ledger: %zu asset(s) across %zu zone/type group(s) written to %s",
         rows.size(), counts.size(), text.c_str());
    for (const auto &[type, count] : unexplained)
    {
        std::string examples;
        for (const auto &row : rows)
            if (row.type == type && row.origin == "UNEXPLAINED" && examples.size() < 160)
                examples += (examples.empty() ? "" : ", ") + row.name;
        warn("build-map: output ledger: %zu %s asset(s) are UNEXPLAINED: %s", count, type.c_str(),
             examples.c_str());
    }
}

int writeMapPackage(const Args &args, const std::string &map, const std::string &outputDirectory,
                    const std::string &entities, const iw8::MapBounds &bounds,
                    const std::string &dumpDirectory, const MapMetadata &metadata,
                    const std::vector<convert::TriggerModelSource> &triggers,
                    const iw3::PreparedMap *prepared,
                    const convert::CompassBounds *compassBounds)
{
    if (!prepareOutputDirectory(outputDirectory, map))
    {
        return 2;
    }

    LapTimer timing("package");
    std::vector<OutputLedgerRow> ledgerRows;
    // Stock assets every shipped map carries and IW3 has no source for (section
    // 19): surface-fx `void` with its effects, and the CODCaster images.
    std::unique_ptr<iw8::stock::Copier> stockCopier;
    {
        std::string acts = args.actsExecutable;
        if (acts.empty())
            if (const char *value = std::getenv("IW8_ACTS"); value && *value)
                acts = value;
        if (acts.empty() || args.replayExecutable.empty())
            warn("build-map: no --acts (or IW8_ACTS) and --replay: the stock surface-fx table `void` and the "
                 "CODCaster images are not copied");
        else
        {
            iw8::stock::Options options;
            options.acts = acts;
            options.replayExecutable = args.replayExecutable;
            options.cacheDirectory =
                args.stockCache.empty()
                    ? std::filesystem::absolute(outputDirectory).parent_path() / "_stock_cache"
                    : std::filesystem::path(args.stockCache);
            stockCopier = std::make_unique<iw8::stock::Copier>(options);
            timing.lap("stock exports");
        }
    }
    const std::string assetName = "maps/mp/" + map + ".d3dbsp";
    iw8::MapSun lighting = loadLighting(args, dumpDirectory, assetName);
    const std::string renderPath = path_join(dumpDirectory, assetName + ".render.json");
    if (!file_exists(renderPath))
    {
        throw std::runtime_error("native render data is missing: " + renderPath);
    }
    const auto renderMesh = replayrender::Load(renderPath);
    timing.lap("render load");
    calibrateLocalLights(lighting, renderMesh.solidTriangles);
    dropEnclosedShadowedSpots(lighting, renderMesh.solidTriangles, prepared ? &prepared->shadowScene : nullptr);
    limitShadowedSpotLights(lighting);
    // Use the final ComWorld sun vector, including any requested profile.
    // Replay rejects compressed shadows whose stored direction differs from
    // the active sun. Moving entities and breakable panes are not baked.
    // Shipped worlds keep this blob near 14-20 MB; a large IW3 world at one unit
    // per texel can reach hundreds of MB, so it uses Backlot's two-unit texels.
    // Worlds far larger than their play space (Greenband: 5.2M caster triangles
    // over +/-100000 units) overflow even then; their last step bakes only the
    // casters that can shadow the minimap region plus a margin.
    constexpr std::size_t kSunShadowBudget = 96u * 1024u * 1024u;
    const auto bakeSunShadow = [&](const float sampleSize, const replaybounds::Bounds *focus)
        -> std::optional<replaysunshadow::Data> {
        try
        {
            auto data = replaysunshadow::Bake(
                prepared->shadowScene, {lighting.direction[0], lighting.direction[1], lighting.direction[2]},
                renderMesh.sceneBounds, sampleSize, focus);
            if (data.bytes.size() <= kSunShadowBudget)
                return data;
            info("sun-shadow: %zu bytes at %.0f unit(s) per texel%s exceed the %zu-byte budget",
                 data.bytes.size(), sampleSize, focus ? " (play-space crop)" : "", kSunShadowBudget);
        }
        catch (const std::runtime_error &error)
        {
            if (std::string_view(error.what()).find("exceeds") == std::string_view::npos)
                throw;
            info("sun-shadow: %s at %.0f unit(s) per texel%s", error.what(), sampleSize,
                 focus ? " (play-space crop)" : "");
        }
        return std::nullopt;
    };
    replaysunshadow::Data sunShadow;
    if (prepared)
    {
        auto baked = bakeSunShadow(1.0f, nullptr);
        if (!baked)
            baked = bakeSunShadow(2.0f, nullptr);
        if (!baked && compassBounds)
        {
            constexpr float margin = 4096.0f;
            replaybounds::Bounds focus = renderMesh.sceneBounds;
            const float minX = std::min(compassBounds->northwestX, compassBounds->southeastX) - margin;
            const float maxX = std::max(compassBounds->northwestX, compassBounds->southeastX) + margin;
            const float minY = std::min(compassBounds->northwestY, compassBounds->southeastY) - margin;
            const float maxY = std::max(compassBounds->northwestY, compassBounds->southeastY) + margin;
            focus.midpoint[0] = (minX + maxX) * 0.5f;
            focus.midpoint[1] = (minY + maxY) * 0.5f;
            focus.halfSize[0] = (maxX - minX) * 0.5f;
            focus.halfSize[1] = (maxY - minY) * 0.5f;
            info("sun-shadow: baking only casters that shadow the minimap region +%.0f units "
                 "(%.0f..%.0f, %.0f..%.0f)", margin, minX, maxX, minY, maxY);
            baked = bakeSunShadow(2.0f, &focus);
        }
        if (!baked)
            throw std::runtime_error("compressed sun-shadow does not fit the native format even at two "
                                     "units per texel over the play space");
        sunShadow = std::move(*baked);
    }
    if (!sunShadow.bytes.empty())
        info("sun-shadow: baked %u cells at %u resolution (%u uniform, %u quadtree), %zu native "
             "compressed bytes",
             sunShadow.parameters.forestSize, sunShadow.parameters.resolution,
             sunShadow.uniformCells, sunShadow.treeCells, sunShadow.bytes.size());
    timing.lap("sun shadow bake");
    const auto glassInitCount = static_cast<uint32_t>(renderMesh.glassPanes.size());
    iw8::havok::PhysicsAsset glassPhysics;
    if (glassInitCount)
    {
        if (args.replayExecutable.empty())
            throw std::runtime_error("native glass physics requires --replay");
        iw8::havok::PrepareCollisionBaker(args.replayExecutable);
        iw8::havok::PhysicsMesh dummy;
        dummy.contents = 45440; // shipped Replay glasschunkdummydefault bodyContents
        dummy.vertices = {{{-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f},
                           {0.5f, 0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f},
                           {-0.5f, -0.5f, 0.5f}, {0.5f, -0.5f, 0.5f},
                           {0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f}}};
        dummy.triangles = {{{0, 2, 1}, {0, 3, 2}, {4, 5, 6}, {4, 6, 7},
                            {0, 1, 5}, {0, 5, 4}, {1, 2, 6}, {1, 6, 5},
                            {2, 3, 7}, {2, 7, 6}, {3, 0, 4}, {3, 4, 7}}};
        glassPhysics = iw8::havok::BakePhysicsAsset(
            iw8maps::kGlassPhysicsName, dummy, 7, 9);
    }
    const auto glassPieceLimit = glassInitCount ? glassInitCount + 512u : 0u;
    iw8::navigation::Result navigation;
    if (prepared && args.navigation)
    {
        navigation = iw8::navigation::Build(prepared->navigation);
        timing.lap("bot navigation");
    }
    // Replay spawns each fixed pathnode from a node_* MapEnts record, in order.
    std::string mapEntities = entities;
    if (navigation)
    {
        uint32_t classnameKey = 0;
        uint32_t originKey = 0;
        if (!convert::iw3KeyToIw8KeyId("classname", classnameKey) ||
            !convert::iw3KeyToIw8KeyId("origin", originKey))
            throw std::runtime_error("Replay entity key table lacks classname or origin");
        const auto origins = iw8::navigation::FixedPathNodes(navigation);
        for (const auto &origin : origins)
        {
            char line[192];
            std::snprintf(line, sizeof(line), "{ %u \"node_pathnode\" %u \"%.3f %.3f %.3f\" }\n",
                          classnameKey, originKey, origin[0], origin[1], origin[2]);
            mapEntities += line;
        }
        info("navigation: appended %zu node_pathnode MapEnts records", origins.size());
    }
    int result = 0;
    auto collision = loadCollision(args, dumpDirectory, assetName);
    timing.lap("collision bake");
    {
        if (collision.models.empty())
        {
            iw8::havok::CollisionModel worldModel;
            if (bounds.valid)
            {
                std::copy_n(bounds.mn, 3, worldModel.minimum.begin());
                std::copy_n(bounds.mx, 3, worldModel.maximum.begin());
            }
            collision.models.push_back(worldModel);
        }
        collision.triggers.reserve(triggers.size());
        for (const auto &trigger : triggers)
        {
            if (trigger.collisionModel >= collision.models.size())
                throw std::runtime_error("IW3 trigger references a missing collision model");
            if (collision.models[trigger.collisionModel].shapeIndex ==
                std::numeric_limits<uint16_t>::max())
                throw std::runtime_error("IW3 trigger collision model has no native Havok shape");
            collision.triggers.push_back({trigger.collisionModel, trigger.staticPhysics});
        }
        info("collision: serialized native world %zu bytes, entities %zu bytes, %zu brush models",
             collision.world.size(), collision.entities.size(), collision.models.size());
    }

    {
        iw8::ZoneWriter writer;
        iw8::ZoneWriter techsets;
        const auto dynamicCounts = prepared ? iw8::CountDynamicEntities(prepared->dynamicEntities)
                                            : iw8::DynamicEntityCounts{};
        static const std::vector<iw8::staticmodelcollision::Model> noStaticCollisionModels;
        const auto worldStrings = iw8::internMapWorldStrings(
            writer, mapEntities, lighting,
            prepared ? prepared->staticModelCollisionModels : noStaticCollisionModels);
        replayrender::RegisterMapMaterials(writer, techsets, renderMesh);
        replayrender::RegisterReflectionProbeImage(writer, renderMesh);
        if (glassInitCount)
            iw8::writePhysicsAsset(writer, glassPhysics);
        // Models and effects this zone registers; only these go into the level
        // NetConstStrings (stock names already sit in the common tables).
        std::set<std::string> zoneModels, zoneEffects, scriptModels, scriptEffects;
        if (prepared)
        {
            for (const auto &model : prepared->xmodels)
                zoneModels.insert(model.model.name);
            for (const auto &effect : prepared->vfxEffects)
                zoneEffects.insert(effect.name);
        }
        if (prepared)
        {
            if (!prepared->gradingClutName.empty())
                iw8::grading::RegisterGradingClut(writer, prepared->gradingClutName,
                                                  iw3::GradingClutSize,
                                                  prepared->gradingClutPixels);
            for (const auto &rawFile : prepared->rawFiles)
                iw8::rawfile::Register(writer, rawFile.name, rawFile.data);
            for (const auto &anim : prepared->xanims)
                iw8::xanim::Register(writer, anim);
            if (prepared->soundBank)
                iw8::sound::RegisterBank(writer, *prepared->soundBank);
            for (const auto &model : prepared->xmodels)
            {
                if (!model.physicsAsset.name.empty())
                {
                    iw8::writePhysicsAsset(writer, model.physicsAsset);
                }
                if (!model.detailCollision.name.empty())
                {
                    iw8::writeXModelDetailCollision(writer, model.detailCollision);
                }
                for (const auto &lod : model.lods)
                    iw8xs_dump::writeXModelSurfs(writer, lod.name, lod);
                iw8::writeXModel(writer, model.model);
            }
            if (!prepared->vfxLightDef.empty())
                iw8::vfx::RegisterLightDef(writer, prepared->vfxLightDef);
            for (auto &shape : prepared->vfxShapes)
                iw8::stock::RegisterPhysicsShape(writer, shape.name, std::move(shape.havokData));
            for (const auto &[type, name] : prepared->vfxReferences)
            {
                iw8::stock::RegisterNameReference(writer, type, name);
                if (type == ASSET_TYPE_MATERIAL)
                    iw8::stock::RegisterNameReference(techsets, type, name);
            }
            for (auto &effect : prepared->vfxEffects)
                iw8::vfx::Register(writer, std::move(effect));
            iw8::RegisterDynamicEntityList(writer, prepared->dynamicEntities);
        }
        iw8::impact::Register(writer, map,
                              prepared ? prepared->smallGlassEffect : std::string{},
                              prepared ? prepared->impactOverrides
                                       : iw8::impact::EffectOverrides{});
        if (stockCopier)
            stockCopier->RegisterSurfaceEffects(writer, techsets);
        if (!renderMesh.glassPanes.empty())
        {
            writer.add(ASSET_TYPE_FX_MAP, assetName,
                       [assetName, &renderMesh](iw8::ZoneWriter &output) {
                           iw8maps::emitFxMapBody(output, assetName.c_str(), renderMesh);
                       });
        }
        const auto primaryLightCount = static_cast<uint32_t>(lighting.primaryLights.size());
        const auto sunPrimaryLightIndex = lighting.sunPrimaryLightIndex;
        const replayrender::StaticModels staticModels =
            prepared ? prepared->staticModels : replayrender::StaticModels{};
        const std::array<float, 3> sunDirection{lighting.direction[0], lighting.direction[1],
                                                lighting.direction[2]};
        // The world's IES lookup strip and the stock IES LightDefs (flashlight,
        // NVG illuminator), as shipped maps carry; registered before the world.
        iw8::ies::Register(writer);
        // Native GfxWorld heightfield and outdoor mask (shipped worlds carry
        // both); the images precede the world that references them.
        std::optional<iw8::heightfield::Data> heightfield;
        if (!renderMesh.solidTriangles.empty())
        {
            // Opaque world triangles plus every sun-shadow caster, which adds the
            // static models (shipped heightmaps include model-built structures).
            std::vector<float> solid = renderMesh.solidTriangles;
            if (prepared)
                for (const auto &surface : prepared->shadowScene.surfaces)
                    for (const auto &vertex : surface.vertices)
                        solid.insert(solid.end(), vertex.position.begin(), vertex.position.end());
            heightfield = iw8::heightfield::Build(solid, renderMesh.sceneBounds);
            iw8::heightfield::Register(writer, *heightfield);
            info("heightfield: %ux%u over %zu triangles", heightfield->size, heightfield->size,
                 renderMesh.solidTriangles.size() / 9);
        }
        // Local (spot/omni) primary lights: GfxWorld volumes, shadow frustums,
        // BVH and static caster lists.
        std::vector<iw8::locallights::Light> localLightSources;
        for (const auto &source : lighting.primaryLights)
        {
            iw8::locallights::Light light;
            light.type = source.type;
            light.canUseShadowMap = source.canUseShadowMap;
            std::copy(std::begin(source.origin), std::end(source.origin), light.origin.begin());
            // ComPrimaryLight dir points back toward the light (IW3 negates the
            // angle vector, cg_ents_mp; shipped Replay spot volumes cover -dir:
            // mp_hideout 42/42 unshadowed spots). The volumes need the emission axis.
            for (size_t component = 0; component < 3; ++component)
                light.direction[component] = -source.direction[component];
            light.radius = source.radius;
            light.cosHalfFovOuter = source.cosHalfFovOuter;
            light.bulbRadius = source.bulbRadius;
            light.tubeLength = std::sqrt(source.bulbLength[0] * source.bulbLength[0] +
                                         source.bulbLength[1] * source.bulbLength[1] +
                                         source.bulbLength[2] * source.bulbLength[2]);
            light.dynamicShadows = source.needsDynamicShadows;
            localLightSources.push_back(light);
        }
        const auto localLights = iw8::locallights::Build(localLightSources, sunPrimaryLightIndex,
                                                         renderMesh, staticModels);
        if (!localLights.Empty())
            info("local lights: %zu volume(s), %zu dynamic-shadow, %zu mesh vertices",
                 localLights.umbraLights.size(), localLights.aabbLights.size(),
                 localLights.vertices.size());
        writer.add(ASSET_TYPE_GFX_MAP, assetName,
                   [assetName, renderPath, &renderMesh, primaryLightCount,
                    sunPrimaryLightIndex, staticModels, dynamicCounts,
                    &sunShadow, sunDirection, &heightfield,
                    &localLights](iw8::ZoneWriter &output) {
                       iw8maps::emitGfxMapBody(output, assetName.c_str(), renderPath,
                                               renderMesh, primaryLightCount, sunPrimaryLightIndex,
                                               staticModels, dynamicCounts.models,
                                               dynamicCounts.brushes, sunShadow, sunDirection,
                                               heightfield ? &*heightfield : nullptr,
                                               &localLights);
                   });
        writer.add(ASSET_TYPE_GLASS_MAP, assetName,
                   [assetName, &renderMesh](iw8::ZoneWriter &output) {
                       iw8maps::emitGlassMapBody(output, assetName.c_str(), renderMesh);
                   });
        iw8::navigation::Register(writer, assetName, navigation);
        const dumpsrc::DumpSource source(dumpDirectory, map);
        const AssetTally assets =
            addMapAssets(writer, techsets, dumpDirectory, source, map, metadata.compassRotation);
        if (assets.failures)
            throw std::runtime_error("failed to import " + std::to_string(assets.failures) +
                                     " map assets; first: " + assets.firstFailure);
        std::string gscTool = args.gscTool;
        if (gscTool.empty())
            if (const char *value = std::getenv("IW8_GSC_TOOL"); value && *value)
                gscTool = value;
        bool scriptCompiled = false;
        if (prepared && !prepared->mapScript.empty() && !gscTool.empty())
        {
            // Converted IW3 map scripts plus the IW3 compatibility layer, with
            // the minimap call generated from the squared compass bounds.
            // setminimap( material, x1, y1, x2, y2, 1, material ) is the
            // argument order of the proven hand-assembled startup bytecode.
            std::string generated = "iw3compat_minimap()\n{\n";
            if (assets.hasCompass && compassBounds)
            {
                // The console dvar mw120r_minimap_scale overrides the region scale
                // relative to IW3's squared region (restart the match to apply), so
                // the minimap can be calibrated in game without a rebuild.
                const float cx = (compassBounds->northwestX + compassBounds->southeastX) * 0.5f;
                const float cy = (compassBounds->northwestY + compassBounds->southeastY) * 0.5f;
                const float base = metadata.minimapScale;
                char line[1024];
                std::snprintf(
                    line, sizeof(line),
                    "    scale = getdvarfloat( \"mw120r_minimap_scale\" );\n"
                    "    if ( scale <= 0 )\n"
                    "        scale = %.4f;\n"
                    "    k = scale / %.4f;\n"
                    "    setminimap( \"compass_map_%s\", %.3f + %.3f * k, %.3f + %.3f * k, "
                    "%.3f + %.3f * k, %.3f + %.3f * k, 1, \"compass_map_%s\" );\n",
                    base, base, map.c_str(), cx, compassBounds->northwestX - cx, cy,
                    compassBounds->northwestY - cy, cx, compassBounds->southeastX - cx, cy,
                    compassBounds->southeastY - cy, map.c_str());
                generated += line;
            }
            generated += "}\n";
            const std::string scriptSource = prepared->mapScript + "\n" + generated;
            // Names the script resolves at runtime: precachemodel/setmodel through the
            // model table, loadfx through the effect table.
            static const std::regex scriptModel(R"re((?:precachemodel|setmodel)\s*\(\s*"([^"]+)")re",
                                                std::regex::icase);
            static const std::regex scriptEffect(R"re(loadfx\s*\(\s*"([^"]+)")re", std::regex::icase);
            for (auto match = std::sregex_iterator(scriptSource.begin(), scriptSource.end(),
                                                   scriptModel);
                 match != std::sregex_iterator(); ++match)
                scriptModels.insert((*match)[1].str());
            for (auto match = std::sregex_iterator(scriptSource.begin(), scriptSource.end(),
                                                   scriptEffect);
                 match != std::sregex_iterator(); ++match)
                scriptEffects.insert((*match)[1].str());
            const std::filesystem::path work =
                std::filesystem::path(path_dir(outputDirectory)) / ".iw3-scripts" / map;
            try
            {
                const auto gscbin = iw3::scripts::Compile(scriptSource, gscTool, work, map);
                const std::string scriptName = "scripts/mp/maps/" + map + "/" + map + ".gsc";
                writer.add(ASSET_TYPE_SCRIPTFILE, scriptName,
                           [scriptName, gscbin](iw8::ZoneWriter &output) {
                               iw8::replay_script::emitCompiledScript(output, scriptName, gscbin);
                           });
                scriptCompiled = true;
                info("scripts: compiled converted map script (%zu bytes gscbin, source %s)",
                     gscbin.size(), (work / (map + ".gsc")).string().c_str());
            }
            catch (const std::exception &error)
            {
                warn("scripts: converted map script did not compile, using the minimap-only "
                     "startup script: %s", error.what());
            }
        }
        else if (prepared && !prepared->mapScript.empty())
            warn("scripts: pass --gsc-tool (or set IW8_GSC_TOOL) to compile the converted IW3 map "
                 "scripts; using the minimap-only startup script");
        if (!scriptCompiled && assets.hasCompass && compassBounds)
        {
            const std::string scriptName = "scripts/mp/maps/" + map + "/" + map + ".gsc";
            const convert::CompassBounds compass = *compassBounds;
            writer.add(ASSET_TYPE_SCRIPTFILE, scriptName,
                       [scriptName, map, compass](iw8::ZoneWriter &output) {
                           iw8::replay_script::emitCompassStartup(
                               output, scriptName, map, compass.northwestX,
                               compass.northwestY, compass.southeastX, compass.southeastY);
                       });
        }
        else if (!scriptCompiled && assets.hasCompass)
        {
            warn("assets: compass image has no valid pair of IW3 minimap_corner entities");
        }
        // Map entity models (MapEnts key 652 = "model"; '*' values are brush models).
        iw8::LevelNetConstStrings levelStrings;
        {
            static const std::regex entityModel(R"re((?:^|[\s{])652 "([^"*][^"]*)")re");
            std::set<std::string> models;
            for (auto match = std::sregex_iterator(mapEntities.begin(), mapEntities.end(),
                                                   entityModel);
                 match != std::sregex_iterator(); ++match)
                if (zoneModels.contains((*match)[1].str()))
                    models.insert((*match)[1].str());
            for (const auto &model : scriptModels)
                if (zoneModels.contains(model))
                    models.insert(model);
            // Stock stand-ins (--style iw8 effects, unconverted graphs) live in the
            // always-loaded common MP zones; loadfx still needs their name in a
            // loaded "vfx" table.
            std::set<std::string> effects;
            for (const auto &effect : scriptEffects)
                if (zoneEffects.contains(effect) ||
                    (effect.starts_with("vfx/") && !effect.starts_with("vfx/mw120r/")))
                    effects.insert(effect);
            levelStrings[0].assign(models.begin(), models.end());  // "mdl"
            levelStrings[6].assign(effects.begin(), effects.end()); // "vfx"
            // Script-played animations (scriptmodelplayanim) resolve through
            // the level "anm" table, like script models through "mdl".
            if (prepared)
                for (const auto &anim : prepared->xanims)
                    levelStrings[16].push_back(anim.name); // "anm"
            info("assets: level NetConstStrings: %zu model(s), %zu effect(s)", models.size(),
                 effects.size());
        }
        iw8::registerMapWorldAssets(
            writer, worldStrings, assetName, mapEntities, bounds, lighting, collision,
            glassInitCount, glassPieceLimit, dynamicCounts,
            prepared && prepared->vfxLightDef == "light_point_linear", levelStrings,
            prepared ? prepared->clientTriggers : nullptr);
        timing.lap("asset registration");
        techsets.build();
        writer.build();
        timing.lap("zone build");

        if (!writeZone(path_join(outputDirectory, "techsets_" + map + ".ff"),
                       techsets.body(), writeParams(techsets)))
        {
            result = 1;
        }

        if (!writeZone(path_join(outputDirectory, map + ".ff"), writer.body(), writeParams(writer)))
        {
            result = 1;
        }
        timing.lap("zone write");
        for (auto &row : outputLedgerRows(map, writer.assetList(), renderMesh, prepared, map, stockCopier.get()))
            ledgerRows.push_back(std::move(row));
        for (auto &row : outputLedgerRows("techsets_" + map, techsets.assetList(), renderMesh, prepared, map,
                                          stockCopier.get()))
            ledgerRows.push_back(std::move(row));
        info("build-map: main zone contains %zu assets", writer.assetCount());
        info("build-map: techset zone contains %zu assets", techsets.assetCount());
    }

    const auto writeEmpty = [&](const std::string &name) {
        iw8::ZoneBuffer buffer;
        iw8::buildEmptyZone(buffer);
        if (!writeZone(path_join(outputDirectory, name), buffer.data(), writeParams(buffer)))
        {
            result = 1;
        }
    };
    // Shipped srv_<map> zones hold only server extras; the world lives in the main zone.
    writeEmpty("srv_" + map + ".ff");
    iw8::ZoneWriter english;
    if (prepared && !prepared->chyronClock.empty())
    {
        // The map-intro chyron the MP HUD shows at match start (stock keeps
        // every shipped map's in eng_code_post_gfx): per team, operation,
        // clock (the game appends the running seconds), location, unit.
        // IW3 maps name no location; "Classified" is the stock wording for one.
        std::string operation = metadata.title.empty() ? map : metadata.title;
        if (operation.ends_with(" (Classic)"))
            operation.resize(operation.size() - 10);
        const char *teams[]{"allies", "axis"};
        for (size_t team = 0; team < 2; ++team)
        {
            const std::string prefix = "mp_chyron/" + map + "_" + teams[team] + "_";
            const std::string lines[]{"Operation: " + operation, prepared->chyronClock,
                                      "Classified", prepared->chyronUnits[team]};
            for (size_t line = 0; line < 4; ++line)
                iw8::localize::Register(english, prefix + std::to_string(line + 1), lines[line]);
        }
        info("build-map: eng_%s: map-intro chyron '%s' / '%s' / %s vs %s", map.c_str(),
             operation.c_str(), prepared->chyronClock.c_str(), prepared->chyronUnits[0].c_str(),
             prepared->chyronUnits[1].c_str());
    }
    iw8::ZoneWriter worldwide;
    if (stockCopier)
    {
        stockCopier->RegisterEnglishImages(english);
        stockCopier->RegisterWorldwideImages(worldwide);
    }
    // The language packs: eng_<map> (English strings and images), ww_<map>
    // (worldwide images); empty zones when there is nothing to carry.
    for (auto *zone : {&english, &worldwide})
    {
        const std::string name = (zone == &english ? "eng_" : "ww_") + map;
        if (!zone->assetCount())
        {
            writeEmpty(name + ".ff");
            continue;
        }
        zone->build();
        if (!writeZone(path_join(outputDirectory, name + ".ff"), zone->body(), writeParams(*zone)))
            result = 1;
        for (auto &row : outputLedgerRows(name, zone->assetList(), renderMesh, prepared, map, stockCopier.get()))
            ledgerRows.push_back(std::move(row));
        info("build-map: %s contains %zu assets", name.c_str(), zone->assetCount());
    }
    writeOutputLedger(ledgerRows);

    if (result == 0 && !writeMetadata(outputDirectory, metadata))
    {
        result = 1;
    }
    if (const auto preview = std::filesystem::path(dumpDirectory) / kPreviewFile;
        result == 0 && std::filesystem::is_regular_file(preview))
    {
        std::error_code copyError;
        std::filesystem::copy_file(preview, std::filesystem::path(outputDirectory) / kPreviewFile,
                                   std::filesystem::copy_options::overwrite_existing, copyError);
        if (copyError)
        {
            err("build-map: cannot copy preview.rgba: %s", copyError.message().c_str());
            result = 1;
        }
    }

    if (result == 0 && !validatePackage(outputDirectory, map))
    {
        result = 1;
    }
    timing.lap("metadata and validation");
    if (result == 0)
    {
        info("build-map: wrote map output to %s", outputDirectory.c_str());
    }
    return result;
}

int buildMap(const Args &args, const iw3::PreparedMap *prepared = nullptr)
{
    if (args.positional.size() < 2)
    {
        err("build-map: expected <dump> <map>");
        return 2;
    }

    const std::string dumpDirectory = args.positional[0];
    const std::string map = args.positional[1];
    if (!requireMapId(map))
    {
        return 2;
    }

    // The Replay Havok image needs its preferred address. Reserve it before
    // render/lighting preparation in standalone build-map as build-iw3 does.
    // Otherwise a map with native collision can fail late at BakeCollision.
    if (!args.replayExecutable.empty())
        iw8::havok::PrepareCollisionBaker(args.replayExecutable);

    MapMetadata metadata;
    if (!readMetadata(args.metadataPath, metadata))
    {
        return 2;
    }
    if (!metadata.id.empty() && metadata.id != map)
    {
        err("metadata: id '%s' does not match map '%s'", metadata.id.c_str(), map.c_str());
        return 2;
    }
    metadata.present = true;
    if (metadata.id.empty())
        metadata.id = map;
    if (metadata.title.empty())
        metadata.title = map;
    const std::string outputDirectory = args.outputDirectory.empty()
                                            ? path_join(dumpDirectory, map + "_out")
                                            : args.outputDirectory;

    const dumpsrc::DumpSource source(dumpDirectory, map);
    if (!source.hasMapFiles())
    {
        err("build-map: no maps/mp/%s.d3dbsp.* files under %s", map.c_str(), dumpDirectory.c_str());
        return 1;
    }

    std::string entities;
    std::vector<convert::TriggerModelSource> triggers;
    bool foundEntities = false;
    for (const std::string &path : {path_join(dumpDirectory, map + "_iw8_ents.txt"),
                                    path_join(path_dir(dumpDirectory), map + "_iw8_ents.txt")})
    {
        if (file_exists(path) && read_file_str(path, entities) && !entities.empty())
        {
            info("entities: using %s", path.c_str());
            foundEntities = true;
            break;
        }
    }

    const dumpsrc::EntsDump sourceEntities = source.loadEnts();
    convert::CompassBounds compassBounds;
    const bool hasCompassBounds = sourceEntities.loaded && !sourceEntities.text.empty() &&
                                  convert::iw3CompassBounds(sourceEntities.text, compassBounds);
    if (hasCompassBounds)
    {
        info("compass: northwest %.3f %.3f, southeast %.3f %.3f", compassBounds.northwestX,
             compassBounds.northwestY, compassBounds.southeastX, compassBounds.southeastY);
        // IW3 compass images are square, uniformly scaled maps. IW3 stretched
        // them over the minimap_corner rectangle, but Replay's map keeps the
        // world aspect, so a non-square corner pair squashes the image (Nuketown:
        // 6136 x 2904 units). Square the region around its center.
        const float spanX = std::abs(compassBounds.southeastX - compassBounds.northwestX);
        const float spanY = std::abs(compassBounds.northwestY - compassBounds.southeastY);
        if (std::abs(spanX - spanY) > 1.0f)
        {
            const float half = std::max(spanX, spanY) * 0.5f;
            const float centerX = (compassBounds.northwestX + compassBounds.southeastX) * 0.5f;
            const float centerY = (compassBounds.northwestY + compassBounds.southeastY) * 0.5f;
            const float signX = compassBounds.southeastX >= compassBounds.northwestX ? 1.0f : -1.0f;
            const float signY = compassBounds.northwestY >= compassBounds.southeastY ? 1.0f : -1.0f;
            compassBounds.northwestX = centerX - signX * half;
            compassBounds.southeastX = centerX + signX * half;
            compassBounds.northwestY = centerY + signY * half;
            compassBounds.southeastY = centerY - signY * half;
            info("compass: squared minimap region to northwest %.3f %.3f, southeast %.3f %.3f",
                 compassBounds.northwestX, compassBounds.northwestY, compassBounds.southeastX,
                 compassBounds.southeastY);
        }
        // Optional contraction of the declared region about its centre (metadata
        // minimapScale; default 1.0 = IW3's region). fixes-25 used 0.875 from one
        // in-game point ((1206, 630) shown at (1072, 545)); measured against the
        // art that over-contracted, and the marker drifted (owner test 2026-10-14).
        if (metadata.minimapScale != 1.0f)
        {
            const float centerX = (compassBounds.northwestX + compassBounds.southeastX) * 0.5f;
            const float centerY = (compassBounds.northwestY + compassBounds.southeastY) * 0.5f;
            const float scale = metadata.minimapScale;
            compassBounds.northwestX = centerX + (compassBounds.northwestX - centerX) * scale;
            compassBounds.southeastX = centerX + (compassBounds.southeastX - centerX) * scale;
            compassBounds.northwestY = centerY + (compassBounds.northwestY - centerY) * scale;
            compassBounds.southeastY = centerY + (compassBounds.southeastY - centerY) * scale;
            info("compass: scaled minimap region by %.3f to northwest %.3f %.3f, southeast %.3f %.3f",
                 scale, compassBounds.northwestX, compassBounds.northwestY,
                 compassBounds.southeastX, compassBounds.southeastY);
        }
        // IW3 draws the compass image with northyaw up; Replay's minimap wants
        // it turned clockwise by northyaw (Office, default 90, was verified at 90).
        if (!metadata.compassRotationSet)
        {
            const float yaw = std::fmod(std::fmod(compassBounds.northYaw, 360.0f) + 360.0f, 360.0f);
            const float quarter = std::round(yaw / 90.0f);
            if (std::abs(yaw - quarter * 90.0f) < 0.01f)
            {
                metadata.compassRotation = static_cast<unsigned>(quarter * 90.0f) % 360u;
                info("compass: rotating the IW3 minimap %u degrees for northyaw %.1f",
                     metadata.compassRotation, compassBounds.northYaw);
            }
            else
                warn("compass: northyaw %.3f is not a quarter turn; minimap left unrotated",
                     compassBounds.northYaw);
        }
    }
    if (!foundEntities && sourceEntities.loaded && !sourceEntities.text.empty())
    {
        const size_t converted =
            convert::iw3ToIw8EntityString(sourceEntities.text, entities, &triggers);
        if (converted == 0)
        {
            err("build-map: could not convert any source entities");
            return 1;
        }
        info("entities: converted %zu source entities", converted);
        foundEntities = true;
    }
    if (!foundEntities)
    {
        entities = defaultEntities;
        warn("entities: using the built-in Replay TDM spawn set");
    }
    convert::validateIw8EntityString(entities);

    iw8::MapBounds bounds;
    const dumpsrc::ClipMapDump clipMap = source.loadClipMap();
    if (clipMap.loaded && clipMap.boundsFromVerts)
    {
        for (size_t index = 0; index < 3; ++index)
        {
            bounds.mn[index] = clipMap.boundsMin[index];
            bounds.mx[index] = clipMap.boundsMax[index];
        }
        bounds.valid = true;
    }

    const std::string boundsPath =
        path_join(dumpDirectory, "maps/mp/" + map + ".d3dbsp.bounds.json");
    if (file_exists(boundsPath))
    {
        std::ifstream file(boundsPath);
        const nlohmann::json data = nlohmann::json::parse(file);
        if (data.at("schema") != 1 || data.at("min").size() != 3 || data.at("max").size() != 3)
        {
            throw std::runtime_error("invalid map bounds schema");
        }
        for (size_t index = 0; index < 3; ++index)
        {
            bounds.mn[index] = data.at("min").at(index).get<float>();
            bounds.mx[index] = data.at("max").at(index).get<float>();
            if (!std::isfinite(bounds.mn[index]) || !std::isfinite(bounds.mx[index]) ||
                bounds.mn[index] >= bounds.mx[index] || std::abs(bounds.mn[index]) > 262144.0f ||
                std::abs(bounds.mx[index]) > 262144.0f)
            {
                throw std::runtime_error("invalid map bounds");
            }
        }
        bounds.valid = true;
    }

    const dumpsrc::ComWorldDump commonWorld = source.loadComWorld();
    if (commonWorld.loaded)
    {
        info("common world: %d primary lights", commonWorld.primaryLightCount);
    }

    // Build the complete family beside the requested output, then publish the
    // directory only after all five zones and map.json pass validation.
    const std::filesystem::path target =
        std::filesystem::absolute(outputDirectory).lexically_normal();
    const std::filesystem::path parent = target.parent_path();
    std::error_code error;
    std::filesystem::create_directories(parent, error);
    if (error)
    {
        err("build-map: cannot create output parent %s: %s", parent.string().c_str(),
            error.message().c_str());
        return 1;
    }
    const bool initialTarget = std::filesystem::exists(target, error);
    if (error ||
        (initialTarget &&
         (!prepareOutputDirectory(target.string(), map) ||
          (GetFileAttributesW(target.c_str()) & FILE_ATTRIBUTE_REPARSE_POINT))))
    {
        err("build-map: output directory is unavailable or contains unexpected files: %s",
            target.string().c_str());
        return 1;
    }

    const std::string tag = std::to_string(GetCurrentProcessId()) + "-" +
                            std::to_string(GetTickCount64());
    const std::filesystem::path stage =
        parent / ("." + map + ".stage-" + tag);
    const std::filesystem::path backup =
        parent / ("." + map + ".backup-" + tag);
    const bool backupExists = std::filesystem::exists(backup, error);
    if (error || stage.parent_path() != parent || backup.parent_path() != parent ||
        backupExists || !std::filesystem::create_directory(stage, error) || error)
    {
        err("build-map: could not create a private output stage beside %s",
            target.string().c_str());
        return 1;
    }
    const auto cleanKnownFiles = [&](const std::filesystem::path &directory) {
        if (directory.parent_path() != parent ||
            (GetFileAttributesW(directory.c_str()) & FILE_ATTRIBUTE_REPARSE_POINT))
            return;
        std::error_code cleanupError;
        auto names = expectedFiles(map);
        names.push_back(kPreviewFile);
        for (const std::string &name : names)
        {
            std::filesystem::remove(directory / name, cleanupError);
            cleanupError.clear();
            std::filesystem::remove(directory / (name + ".partial"), cleanupError);
            cleanupError.clear();
        }
        std::filesystem::remove(directory, cleanupError);
        if (cleanupError)
            warn("build-map: could not clean %s: %s", directory.string().c_str(),
                 cleanupError.message().c_str());
    };

    int result = 1;
    try
    {
        result = writeMapPackage(args, map, stage.string(), entities, bounds,
                                 dumpDirectory, metadata, triggers, prepared,
                                 hasCompassBounds ? &compassBounds : nullptr);
    }
    catch (...)
    {
        cleanKnownFiles(stage);
        throw;
    }
    if (result)
    {
        cleanKnownFiles(stage);
        return result;
    }
    const bool hadTarget = std::filesystem::exists(target, error);
    if (error ||
        (hadTarget &&
         ((GetFileAttributesW(target.c_str()) & FILE_ATTRIBUTE_REPARSE_POINT) ||
          !prepareOutputDirectory(target.string(), map))))
    {
        cleanKnownFiles(stage);
        err("build-map: output changed while conversion was running");
        return 1;
    }
    if (hadTarget)
    {
        std::filesystem::rename(target, backup, error);
        if (error)
        {
            cleanKnownFiles(stage);
            err("build-map: could not preserve previous output: %s",
                error.message().c_str());
            return 1;
        }
    }
    std::filesystem::rename(stage, target, error);
    if (error)
    {
        const std::string publishError = error.message();
        std::string recovery = "the validated stage remains at " + stage.string();
        if (hadTarget)
        {
            error.clear();
            std::filesystem::rename(backup, target, error);
            recovery += error ? "; previous output remains at " + backup.string()
                              : "; previous output was restored";
        }
        err("build-map: publish failed (%s); %s", publishError.c_str(), recovery.c_str());
        return 1;
    }
    if (hadTarget)
        cleanKnownFiles(backup);
    info("build-map: published validated map output to %s", target.string().c_str());
    return 0;
}

int buildIw3(const Args &args)
{
    if (args.positional.empty() || args.positional.size() > 2 || args.replayExecutable.empty())
    {
        err("iw3: expected the fastfile, an optional map id and the Replay executable");
        return 2;
    }

    const bool hasExplicitMap = args.positional.size() == 2;
    std::string map = hasExplicitMap ? args.positional[1]
                                     : std::filesystem::path(args.positional[0]).stem().string();
    MapMetadata metadata;
    if (!readMetadata(args.metadataPath, metadata))
    {
        return 2;
    }
    if (!metadata.id.empty())
    {
        if (hasExplicitMap && metadata.id != map)
        {
            err("metadata: id '%s' does not match map '%s'", metadata.id.c_str(), map.c_str());
            return 2;
        }
        map = metadata.id;
    }
    if (!requireMapId(map))
    {
        return 2;
    }

    // Direct conversion produces a complete installable package. Always emit
    // the one supported loose configuration file, even when the caller does
    // not provide a custom title or description.
    metadata.present = true;
    if (metadata.id.empty())
    {
        metadata.id = map;
    }
    if (metadata.title.empty())
    {
        metadata.title = map;
    }

    iw8::havok::PrepareCollisionBaker(args.replayExecutable);

    iw3::ImportOptions options;
    options.fastfile = args.positional[0];
    options.map = map;
    options.unlinker = args.unlinkerExecutable;
    options.extraction = args.extraction;
    options.iw3ModeAssets = args.iw3ModeAssets;
    options.style =
        args.style == "iw8" ? iw3::MapStyle::iw8 : iw3::MapStyle::iw3;
    const auto requestedOutput = args.outputDirectory.empty()
                                     ? std::filesystem::path(args.positional[0]).parent_path() /
                                           (map + "_iw8")
                                     : std::filesystem::path(args.outputDirectory);
    options.scratchRoot = std::filesystem::absolute(requestedOutput).parent_path();
    for (const std::string &path : args.searchPaths)
    {
        options.searchPaths.emplace_back(path);
    }
    // The fastfile directory is always a valid source root. A user may set
    // IW3_GAME_ROOT or COD4_ROOT once for the matching main/raw installation;
    // explicit --search-path values remain repeatable for custom asset packs.
    options.searchPaths.emplace_back(options.fastfile.parent_path());
    for (const char *name : {"IW3_GAME_ROOT", "COD4_ROOT"})
    {
        if (const char *value = std::getenv(name); value && *value)
        {
            const std::filesystem::path root(value);
            options.searchPaths.push_back(root);
            options.searchPaths.push_back(root / "main");
            options.searchPaths.push_back(root / "raw");
        }
    }
    for (const std::string &value : args.iw3Roots)
    {
        const std::filesystem::path root(value);
        options.searchPaths.push_back(root);
        options.searchPaths.push_back(root / "main");
        options.searchPaths.push_back(root / "raw");
    }
    // Per-asset IW3 ledger beside the package (the package itself may only
    // hold the map's files): <output>.iw3-assets.tsv.
    iw3::ledger::Clear();
    iw3::ledger::SetReport(std::filesystem::absolute(requestedOutput).string() + ".iw3-assets.tsv");
    iw3::PreparedMap prepared = [&] {
        StageTimer timer("iw3 conversion");
        return iw3::PrepareFastfile(options);
    }();

    Args build = args;
    build.command = "build-map";
    build.positional = {prepared.root.string(), map};
    build.collisionPath = prepared.collision.string();
    build.footstepsPath = prepared.footsteps.string();
    metadata.ladderRungDistance = prepared.ladderRungDistance;
    if (!writeMetadata(prepared.root.string(), metadata))
    {
        return 1;
    }
    build.metadataPath = (prepared.root / "map.json").string();
    if (build.outputDirectory.empty())
    {
        build.outputDirectory =
            (std::filesystem::path(args.positional[0]).parent_path() / (map + "_iw8")).string();
    }
    return buildMap(build, &prepared);
}

} // namespace

namespace zonetool::convert::iw3_iw8
{
	namespace
	{
	void convert_map(const std::filesystem::path& zone_path, const zone_conversion& conversion,
		const std::filesystem::path& extraction)
	{
		const auto option = [&](const std::string& name)
		{
			const auto found = conversion.options.find(name);
			return found == conversion.options.end() ? std::string{} : found->second;
		};

		Args args;
		args.command = "build-iw3";
		args.positional.push_back(zone_path.string());
		auto map = zone_path.stem().string();
		if (const auto requested = option("map"); !requested.empty())
		{
			map = requested;
			args.positional.push_back(requested);
		}

		const auto replay = option("replay");
		args.replayExecutable = replay.empty() ? (conversion.target_game_path / "game_dx12_ship_replay.exe").string() : replay;

		// The collision and physics are built without the Replay executable. --havok exe bakes them with it, and
		// native-check also loads the native collision with it to check the bounds.
		const auto havok = option("havok").empty() ? std::string("native") : option("havok");
		if (havok != "native" && havok != "native-check" && havok != "exe")
		{
			throw std::runtime_error(std::format("--havok must be native, native-check or exe, not \"{}\"", havok));
		}

		if (havok != "exe")
		{
			// The type compendium comes from the zone folder of the game.
			::iw8::havok::UseNativeHavok(std::filesystem::path(args.replayExecutable).parent_path() / "zone", havok == "native-check");
		}

		// Static models also get an XModelDetailCollision, as shipped ones have (native Havok only). --detail_collision 0 leaves it out.
		const auto detail = option("detail_collision").empty() ? std::string("1") : option("detail_collision");
		if (detail != "0" && detail != "1")
		{
			throw std::runtime_error(std::format("--detail_collision must be 0 or 1, not \"{}\"", detail));
		}

		::iw8::havok::UseDetailCollision(havok != "exe" && detail == "1");

		// World and model images are block compressed (BC7) when the extraction asks for it (IW7 maps do: their zones are
		// twice the size of IW3 ones). --bc7_images 0 keeps them RGBA8, 1 compresses them for any map.
		const auto bc7 = option("bc7_images");
		if (!bc7.empty() && bc7 != "0" && bc7 != "1")
		{
			throw std::runtime_error(std::format("--bc7_images must be 0 or 1, not \"{}\"", bc7));
		}

		iw3::UseBc7Images(bc7.empty() ? -1 : bc7 == "1" ? 1 : 0);

		// The x64-zt zone writer lays out the zones. --writer iw8-zonetool keeps the stream layout of the converter.
		const auto writer = option("writer");
		if (!writer.empty() && writer != "x64-zt" && writer != "iw8-zonetool")
		{
			throw std::runtime_error(std::format("--writer must be x64-zt or iw8-zonetool, not \"{}\"", writer));
		}
		g_converterLayout = writer == "iw8-zonetool";
		args.unlinkerExecutable = option("unlinker");
		args.gscTool = option("gsc_tool");
		args.metadataPath = option("metadata");
		args.extraction = extraction.string();
		if (extraction.empty())
		{
			args.iw3Roots.push_back(conversion.source_game_path.string());
		}
		args.outputDirectory = (conversion.output_folder / map).string();
		if (const auto profile = option("lighting_profile"); !profile.empty())
		{
			args.lightingProfile = profile;
		}
		if (const auto scale = option("sun_intensity_scale"); !scale.empty())
		{
			args.sunIntensityScale = std::stof(scale);
		}
		args.navigation = option("navigation") != "0";
		if (const auto style = option("style"); !style.empty())
		{
			if (style != "iw3" && style != "iw8")
			{
				throw std::runtime_error(std::format("--style must be iw3 or iw8, not \"{}\"", style));
			}
			args.style = style;
		}
		if (const auto mode = option("iw3_mode_assets"); !mode.empty())
		{
			args.iw3ModeAssets = mode != "off" && mode != "0";
		}
		args.actsExecutable = option("acts");
		args.stockCache = option("stock_cache");
		if (const auto search = option("search_path"); !search.empty())
		{
			args.searchPaths.push_back(search);
		}

		if (buildIw3(args) != 0)
		{
			throw std::runtime_error(std::format("could not convert \"{}\" to iw8", zone_path.string()));
		}

		// --dump 1 also dumps the IW8 zones of the map (dump/<zone>/ and its csv), which buildzone builds again.
		if (const auto dump = option("dump"); !dump.empty() && dump != "0")
		{
			const auto zone_folder = std::filesystem::path(args.replayExecutable).parent_path() / "zone";
			for (const auto& entry : std::filesystem::directory_iterator(args.outputDirectory))
			{
				if (entry.path().extension() == ".ff")
				{
					zonetool::iw8::definition().dump_zone(zone_folder, entry.path(), nullptr);
				}
			}
		}
	}
	}

	void convert_zone(const std::filesystem::path& zone_path, const zone_conversion& conversion)
	{
		convert_map(zone_path, conversion, {});
	}

	void convert_extraction(const std::filesystem::path& zone_path, const std::filesystem::path& extraction,
		const zone_conversion& conversion)
	{
		convert_map(zone_path, conversion, extraction);
	}
}
