#include <std_include.hpp>
#include <iw8/iw8.hpp>
#include <game.hpp>
#include <iw8/zone/loader.hpp>
#include <iw8/zone/writer.hpp>
#include <iw8/zone/patch_writer.h>
#include <iw8/zone/ff_io.h>
#include <iw8/common/fs_util.h>
#include <iw8/common/log.h>
#include <iw8/common/scratch_files.h>
#include <iw8/dump/maps_convert.h>
#include <iw8/dump/dump_source.h>
#include <iw8/dump/image_dump.h>
#include <iw8/dump/material_dumpsrc.h>
#include <iw8/dump/xmodel_dump.h>
#include "iw3_fastfile.h"
#include "iw3_ledger.h"
#include "iw3_render_assets.h"
#include "iw3_vision.h"
#include "stock_sun.h"
#include "stock_world_remove.h"
#include "dxbc_splice.hpp"
#include <iw8/havok/compendium.hpp>
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
#include <deque>
#include <filesystem>
#include <fstream>
#include <functional>
#include <future>
#include <limits>
#include <map>
#include <memory>
#include <optional>
#include <string_view>
#include <regex>
#include <set>
#include <string>
#include <vector>

#include <formats/compression.hpp>
#include <zlib.h>

#include <Windows.h>

using namespace zt;

namespace zonetool::iw8
{
	const game& definition();
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
    // The world collision is cut into streamed collision tiles (research 90).
    bool collisionTiles = false;
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
    // --render_cache: the encoded image, probe, shader, sun-shadow and sound cache, shared by conversions in other
    // output folders (default: .iw3-render-cache beside the output folder).
    std::string renderCache;
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
    float exposureAdjust{};
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

// The zone of one streamed collision tile, and of the combined collision package (the names the game reads from the
// map's streaming info: <map>_cg_%05u_tr and <map>_cg_ls_tr).
std::string collisionTileZone(const std::string &map, const std::uint32_t tile)
{
    char number[16];
    std::snprintf(number, sizeof(number), "%05u", tile);
    return map + "_cg_" + number + "_tr";
}

std::string collisionPackageZone(const std::string &map)
{
    return map + "_cg_ls_tr";
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
        if (const auto exposure = data.find("exposureAdjust"); exposure != data.end())
        {
            if (!exposure->is_number() || !std::isfinite(exposure->get<double>()) ||
                std::abs(exposure->get<double>()) > 8.0)
                throw std::runtime_error("exposureAdjust must be a finite number from -8 to 8 EV");
            metadata.exposureAdjust = exposure->get<float>();
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
    // An authored rotation of 0 stays authored: left out, build-map would turn the minimap by northyaw instead.
    if (metadata.compassRotation || metadata.compassRotationSet)
    {
        data["compassRotation"] = metadata.compassRotation;
    }
    if (metadata.ladderRungDistance > 0.0f && metadata.ladderRungDistance != 12.0f)
    {
        data["ladderRungDistance"] = metadata.ladderRungDistance;
    }
    if (metadata.minimapScale != 1.0f)
        data["minimapScale"] = metadata.minimapScale;
    if (metadata.exposureAdjust != 0.0f)
        data["exposureAdjust"] = metadata.exposureAdjust;
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

const char *outputAssetTypeName(const int type)
{
    const char *name = iw8sz::type_name(type);
    if (std::strcmp(name, "unknown") != 0)
        return name;
    const auto format = zonetool::iw8::find_asset_format(type);
    return format ? format->name : name;
}

struct ZoneValidation
{
    uint32_t assetCount{};
    std::array<uint32_t, ASSET_TYPE_COUNT> typeCounts{};
    std::vector<std::string> images;
};

bool validateZoneFile(const std::string &path, ZoneValidation *details = nullptr)
{
    std::string bytes;
    if (!read_file_str(path, bytes) || bytes.size() < 0x8C + sizeof(IW8_XAssetList))
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
    // A stored frame ("\x01IWC" and the zone), or an Oodle Kraken one ("\x02IWC" and blocks: xfile::write).
    const bool compressed = std::memcmp(bytes.data() + 0x88, "\x02IWC", 4) == 0;
    if ((!compressed && std::memcmp(bytes.data() + 0x88, "\x01IWC", 4) != 0) ||
        (!compressed && header.xfileHeader.size != bytes.size() - 0x8C) ||
        header.residentPartSize != bytes.size() - 0x88 ||
        header.alwaysLoadedPartSize != header.xfileHeader.size)
    {
        err("validate: %s has invalid resident framing", path.c_str());
        return false;
    }
    if (compressed)
    {
        // The checks below read the stored layout: decompress the zone with the reader and lay it out stored.
        try
        {
            const auto file = zonetool::xfile::read(path, bytes, zonetool::iw8::definition().format);
            if (file.payload.size() != header.xfileHeader.size)
            {
                err("validate: %s decompresses to %zu bytes, not %llu", path.c_str(), file.payload.size(),
                    static_cast<unsigned long long>(header.xfileHeader.size));
                return false;
            }
            bytes = bytes.substr(0, 0x88) + std::string("\x01IWC", 4) + std::string(file.payload);
        }
        catch (const std::exception &e)
        {
            err("validate: %s does not decompress: %s", path.c_str(), e.what());
            return false;
        }
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
        std::vector<std::pair<std::int32_t, std::string>> assets;
        try
        {
            assets = zonetool::iw8::zone_list_assets(path, std::move(bytes));
        }
        catch (const std::exception &e)
        {
            err("validate: %s does not load: %s", path.c_str(), e.what());
            return false;
        }

        std::array<uint32_t, ASSET_TYPE_COUNT> typeCounts{};
        std::vector<std::string> images;
        for (const auto &[type, name] : assets)
        {
            if (type < 0 || type >= ASSET_TYPE_COUNT)
            {
                err("validate: %s has an invalid top-level asset type", path.c_str());
                return false;
            }
            ++typeCounts[type];
            if (type == ASSET_TYPE_IMAGE)
                images.push_back(name);
        }
        info("validate: %s loads with %zu top-level asset(s)", path.c_str(), assets.size());
        if (details)
        {
            details->assetCount = static_cast<uint32_t>(assets.size());
            details->typeCounts = typeCounts;
            details->images = std::move(images);
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
        const char *name = outputAssetTypeName(static_cast<int>(type));
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
    // A map with streamed collision (research 90) also has its 1,024 collision tile zones and the combined package:
    // all of them or none.
    std::vector<std::string> tileFiles;
    for (std::uint32_t tile = 0; tile < iw8::havok::kCollisionTileCount; ++tile)
        tileFiles.push_back(collisionTileZone(map, tile) + ".ff");
    tileFiles.push_back(collisionPackageZone(map) + ".ff");
    std::sort(tileFiles.begin(), tileFiles.end());
    std::vector<std::string> presentTiles;
    std::set_intersection(actual.begin(), actual.end(), tileFiles.begin(), tileFiles.end(), std::back_inserter(presentTiles));
    if (!presentTiles.empty())
    {
        if (presentTiles != tileFiles)
        {
            err("validate: the package holds %zu of the %zu collision tile zones; a tiled map has all of them",
                presentTiles.size(), tileFiles.size());
            return false;
        }
        std::vector<std::string> rest;
        std::set_difference(actual.begin(), actual.end(), tileFiles.begin(), tileFiles.end(), std::back_inserter(rest));
        actual = std::move(rest);
    }
    if (actual != expectedFiles(map))
    {
        err("validate: output must contain exactly five map fastfiles and map.json "
            "(and optionally preview.rgba, and the collision tile zones of a tiled map)");
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
            main[ASSET_TYPE_TECHSET] || main[ASSET_TYPE_VERTEXSHADER] || main[ASSET_TYPE_PIXELSHADER])
        {
            err("validate: map render definitions must reside in the techset zone");
            valid = false;
        }
        // Images live in the main zone. Past the main zone's image budget the largest share the techset zone, which
        // Replay queues first; each image has one definition across the two.
        const std::set<std::string> mainImages(validation[0].images.begin(), validation[0].images.end());
        for (const auto &image : validation[4].images)
        {
            if (mainImages.contains(image))
            {
                err("validate: image %s is defined in both the main and the techset zone", image.c_str());
                valid = false;
            }
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

// Replay's preload places the map script's stream (6) right after block 2 (TEMP_POSTLOAD): a block-2 peak past the
// declared size overwrites the script, which then fails to link ("unknown function": Greenband, 2026-10-16; Scorch,
// 2026-10-25, with 32 bytes of slack by ACTS's measure). The map zone declares this much more block 2 than its
// writers reserve, as memory only (no file payload).
constexpr uint64_t kMapPostloadMargin = 64 * 1024;

Iw8WriteParams withPostloadMargin(Iw8WriteParams params)
{
    params.postloadMargin = kMapPostloadMargin;
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
using ZonePrepare = std::function<std::vector<zonetool::iw8::zone_asset>(zonetool::iw8::loaded_zone &)>;

// Stock scriptable definitions a map carries (IW8 Radiant research 86, step F3): the definition and everything it
// uses that its stock zone holds in full (models, their surfaces, materials, images, physics assets, effects), taken
// from the stock zone in memory and added when the map zone is written again; and the technique sets of those
// materials, taken from the stock zone's techsets companion and added to the map's techsets zone. Assets the stock
// zone only references (common zones) stay name references.
using AssetKeyOf = std::pair<std::int32_t, std::string>;

struct ScriptableCarry
{
    std::vector<std::unique_ptr<zonetool::iw8::loaded_zone>> zones; // alive until both zones are written
    std::vector<zonetool::iw8::zone_asset> assets;                  // for the map zone
    std::vector<zonetool::iw8::zone_asset> techsets;                // for the techsets zone
    std::map<std::string, void *> definitions;                      // definition name -> its header
    std::set<AssetKeyOf> roots;                                     // every asset asked for and found
    std::set<std::string> materialNames;                            // carried materials (techsets zone)
    bool empty() const
    {
        return definitions.empty() && assets.empty() && techsets.empty();
    }
};

using AssetKey = std::pair<std::int32_t, std::string>;

// Additions that the loaded zone does not hold or name yet, each once.
std::vector<zonetool::iw8::zone_asset> NewAdditions(const zonetool::iw8::loaded_zone &zone,
                                                    const std::vector<zonetool::iw8::zone_asset> &wanted)
{
    std::set<AssetKey> have;
    for (const auto &asset : zone.assets)
        have.emplace(asset.type, zonetool::iw8::zone_asset_name(asset.type, asset.header));
    for (const auto &asset : zone.list)
        have.emplace(asset.type, zonetool::iw8::zone_asset_name(asset.type, asset.header));
    std::vector<zonetool::iw8::zone_asset> result;
    for (const auto &asset : wanted)
        if (have.emplace(asset.type, zonetool::iw8::zone_asset_name(asset.type, asset.header)).second)
            result.push_back(asset);
    return result;
}

// `wanted`: stock zone -> the assets (type, name) to take from it with everything they use.
ScriptableCarry CarryStockAssets(const std::map<std::string, std::set<AssetKey>> &wanted,
                                 const std::filesystem::path &zoneFolder)
{
    ScriptableCarry carry;
    if (wanted.empty())
        return carry;

    // One stock zone in memory with its full definitions by type and name.
    struct Source
    {
        std::unique_ptr<zonetool::iw8::loaded_zone> zone;
        std::map<AssetKey, zonetool::iw8::zone_asset> full;
        std::vector<zonetool::iw8::zone_asset> roots;
        std::set<AssetKey> rootKeys;
        std::vector<zonetool::iw8::zone_asset> closure;
    };
    const auto open = [&](const std::string &name, const bool required) {
        Source source;
        const auto path = zoneFolder / (name + ".ff");
        if (!std::filesystem::exists(path))
        {
            if (required)
                throw std::runtime_error("stock zone " + path.string() + " (scriptable definitions) was not found");
            return source;
        }
        source.zone = zonetool::iw8::load_zone(path, {.keep_temp_data = true});
        for (const auto &asset : source.zone->assets)
            if (!asset.referenced)
                source.full.try_emplace(AssetKey{asset.type, zonetool::iw8::zone_asset_name(asset.type, asset.header)},
                                        zonetool::iw8::zone_asset{asset.type, asset.header, false});
        return source;
    };

    for (const auto &[zoneName, names] : wanted)
    {
        info("scriptables: reading stock zone %s for %zu asset(s)", zoneName.c_str(), names.size());
        // The map zone holds the definition, its models and the images; its techsets companion holds the materials
        // of those models with their technique sets and shaders. Each names the other's assets by reference, so the
        // two closures are followed until neither names anything the other holds.
        std::array<Source, 2> sources{open(zoneName, true), open("techsets_" + zoneName, false)};
        for (const auto &key : names)
        {
            const auto found = sources[0].full.find(key);
            if (found == sources[0].full.end())
            {
                const auto format = zonetool::iw8::find_asset_format(key.first);
                throw std::runtime_error("stock zone " + zoneName + " does not hold the " +
                                         std::string(format ? format->name : "asset") + " " + key.second);
            }
            carry.roots.insert(key);
            if (key.first == zonetool::iw8::ASSET_TYPE_SCRIPTABLE)
                carry.definitions.emplace(key.second, found->second.header);
            if (sources[0].rootKeys.insert(key).second)
                sources[0].roots.push_back(found->second);
        }
        std::set<AssetKey> unresolved;
        for (int round = 0; round < 8; ++round)
        {
            bool grew = false;
            unresolved.clear();
            std::vector<AssetKey> references;
            for (auto &source : sources)
            {
                if (!source.zone || source.roots.empty())
                    continue;
                std::vector<AssetKey> named;
                source.closure = zonetool::iw8::asset_closure(*source.zone, source.roots, &named);
                references.insert(references.end(), named.begin(), named.end());
            }
            for (const auto &key : references)
            {
                bool held = false;
                for (auto &source : sources)
                {
                    const auto found = source.full.find(key);
                    if (found == source.full.end())
                        continue;
                    held = true;
                    if (source.rootKeys.insert(key).second)
                    {
                        source.roots.push_back(found->second);
                        grew = true;
                    }
                }
                if (!held)
                    unresolved.insert(key);
            }
            if (!grew)
                break;
        }
        const auto summary = [](const std::vector<zonetool::iw8::zone_asset> &assets) {
            std::map<std::string, std::size_t> byType;
            for (const auto &asset : assets)
            {
                const auto format = zonetool::iw8::find_asset_format(asset.type);
                ++byType[format ? format->name : "?"];
            }
            std::string text;
            for (const auto &[type, count] : byType)
                text += (text.empty() ? "" : ", ") + std::to_string(count) + " " + type;
            return text;
        };
        info("scriptables: %s gives %zu asset(s): %s", zoneName.c_str(), sources[0].closure.size(),
             summary(sources[0].closure).c_str());
        if (sources[1].zone)
            info("scriptables: techsets_%s gives %zu asset(s): %s", zoneName.c_str(), sources[1].closure.size(),
                 summary(sources[1].closure).c_str());
        {
            // What stays a name reference must be in a zone every multiplayer match has loaded, as it is for the
            // stock map. The zones are read smallest first and only while something is still unaccounted for.
            // "$..." images are engine-made.
            static const char *const kAlwaysLoaded[] = {
                "techsets_common",    "techsets_common_mp", "techsets_common_core_mp", "techsets_common_base_mp",
                "techsets_global_mp", "techsets_global_stream_mp", "code_pre_gfx", "global_core_mp", "common", "global",
                "common_core_mp",     "global_mp",          "global_stream_mp",        "code_post_gfx",
                "common_base_mp",     "common_mp"};
            const std::size_t referenced = unresolved.size();
            std::erase_if(unresolved, [](const AssetKey &key) { return !key.second.empty() && key.second.front() == '$'; });
            std::map<std::string, std::size_t> foundIn;
            for (const char *const name : kAlwaysLoaded)
            {
                if (unresolved.empty())
                    break;
                const auto path = zoneFolder / (std::string(name) + ".ff");
                if (!std::filesystem::exists(path))
                    continue;
                std::set<AssetKey> held;
                try
                {
                    const auto zone = zonetool::iw8::load_zone(path, {});
                    for (const auto &asset : zone->assets)
                        if (!asset.referenced)
                            held.emplace(asset.type, zonetool::iw8::zone_asset_name(asset.type, asset.header));
                }
                catch (const std::exception &e)
                {
                    warn("scriptables: cannot read %s to check name references: %s", name, e.what());
                    continue;
                }
                const auto before = unresolved.size();
                std::erase_if(unresolved, [&](const AssetKey &key) { return held.count(key) != 0; });
                if (before != unresolved.size())
                    foundIn[name] = before - unresolved.size();
            }
            std::string where;
            for (const auto &[name, count] : foundIn)
                where += (where.empty() ? "" : ", ") + std::to_string(count) + " in " + name;
            info("scriptables: %zu asset(s) stay name references to always-loaded zones (%s)", referenced - unresolved.size(),
                 where.empty() ? "engine-made" : where.c_str());
            std::map<std::string, std::pair<std::size_t, std::string>> byType;
            for (const auto &[type, name] : unresolved)
            {
                const auto format = zonetool::iw8::find_asset_format(type);
                auto &[count, listed] = byType[format ? format->name : "?"];
                ++count;
                if (listed.size() < 300)
                    listed += (listed.empty() ? "" : ", ") + name;
            }
            for (const auto &[type, entry] : byType)
                warn("scriptables: %zu %s named by the carried definitions are in no zone that is always loaded; "
                     "they will be missing in game: %s",
                     entry.first, type.c_str(), entry.second.c_str());
        }
        // Each carried material lives in the techsets zone and is named from the main zone by a reference entry, as
        // the map's own materials are.
        for (const auto &asset : sources[1].closure)
            if (asset.type == zonetool::iw8::ASSET_TYPE_MATERIAL)
                carry.materialNames.insert(zonetool::iw8::zone_asset_name(asset.type, asset.header));
        carry.assets.insert(carry.assets.end(), sources[0].closure.begin(), sources[0].closure.end());
        carry.techsets.insert(carry.techsets.end(), sources[1].closure.begin(), sources[1].closure.end());
        for (auto &source : sources)
            if (source.zone)
                carry.zones.push_back(std::move(source.zone));
    }
    return carry;
}

// The asset type of a type name as the zone formats name it ("vehicle", "xmodel", ...); -1 when none does.
std::int32_t AssetTypeOfName(const std::string &typeName)
{
    for (std::int32_t candidate = 0; candidate < 256; candidate++)
    {
        const auto format = zonetool::iw8::find_asset_format(candidate);
        if (format && typeName == format->name)
            return candidate;
    }
    return -1;
}

ScriptableCarry CarryScriptableDefinitions(const std::vector<iw8::ScriptableInstance> &instances,
                                           const std::string &replayExecutable,
                                           const std::vector<iw3::PreparedMap::CarryAsset> &more = {})
{
    std::map<std::string, std::set<AssetKey>> wanted; // stock zone -> definitions
    for (const auto &instance : instances)
        if (!instance.carryZone.empty())
            wanted[instance.carryZone].insert({zonetool::iw8::ASSET_TYPE_SCRIPTABLE, instance.definition});
    for (const auto &asset : more)
    {
        const auto type = AssetTypeOfName(asset.type);
        if (type < 0)
            throw std::runtime_error("carry_assets: unknown asset type " + asset.type);
        wanted[asset.zone].insert({type, asset.name});
    }
    if (wanted.empty())
        return {};
    if (replayExecutable.empty())
        throw std::runtime_error("carrying a stock scriptable definition needs --replay (the game folder)");
    return CarryStockAssets(wanted, std::filesystem::path(replayExecutable).parent_path() / "zone");
}

// `prepare` (optional) runs on the loaded zone before it is written again and returns more additions.
bool writeZone(const std::string &outputPath, std::vector<uint8_t> body, const Iw8WriteParams &params,
               const std::span<const zonetool::iw8::zone_asset> additional = {},
               zonetool::iw8::zone_inventory *inventory = nullptr, const ZonePrepare &prepare = {})
{
    if (g_converterLayout)
    {
        if (!additional.empty() || prepare)
        {
            err("iw8: native asset additions require the x64-zt writer");
            return false;
        }
        // The margin is memory without file payload: a calc reservation.
        auto layout = params;
        layout.blockSize[iw8::XFILE_BLOCK_TEMP_POSTLOAD] += params.postloadMargin;
        layout.totalDecompressed += params.postloadMargin;
        layout.calcSize += params.postloadMargin;
        return iw8_write(outputPath, body, layout);
    }

    try
    {
        zonetool::xfile::fastfile file{};
        file.payload.assign(body.begin(), body.end());
        // A 2 GB map zone is loaded and written again: it is not held a third time.
        std::vector<uint8_t>().swap(body);
        file.block_sizes.assign(std::begin(params.blockSize), std::end(params.blockSize));
        file.xfile_version = params.xfileVersion;
        file.transient = params.transientFileType != 0;
        file.preload_walk_size = params.preloadWalkSize;
        auto written = prepare
                           ? zonetool::iw8::rewrite_fastfile(
                                 std::filesystem::path(outputPath).stem().string(), std::move(file),
                                 [&](zonetool::iw8::loaded_zone &zone) {
                                     std::vector<zonetool::iw8::zone_asset> all(additional.begin(), additional.end());
                                     const auto more = prepare(zone);
                                     all.insert(all.end(), more.begin(), more.end());
                                     return all;
                                 },
                                 inventory)
                           : zonetool::iw8::rewrite_fastfile(std::filesystem::path(outputPath).stem().string(),
                                                             std::move(file), additional, inventory);
        written.block_sizes[iw8::XFILE_BLOCK_TEMP_POSTLOAD] += params.postloadMargin;
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
    // The source's GSC-scripted lights (IW7's scriptable range): their count and first index, checked once the table
    // has its final size below.
    std::optional<std::pair<uint32_t, uint32_t>> scriptableLights;
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
        if (data.contains("scriptable_primary_light_count"))
            scriptableLights = {data.at("scriptable_primary_light_count").get<uint32_t>(),
                                data.at("first_scriptable_primary_light").get<uint32_t>()};
        // IES profiles of the source's light definitions (IW7), already in Replay's segment sampling.
        if (const auto profiles = data.find("ies_profiles"); profiles != data.end())
            for (const auto &source : *profiles)
            {
                iw8::ies::Profile profile;
                profile.definition = source.at("definition").get<std::string>();
                profile.profile = source.at("profile").get<std::string>();
                const auto &texels = source.at("texels");
                if (profile.definition.empty() || profile.definition.size() > 255 || !texels.is_array() ||
                    texels.size() != profile.texels.size())
                    throw std::runtime_error("invalid source IES profile");
                for (size_t texel = 0; texel < profile.texels.size(); ++texel)
                    profile.texels[texel] = texels.at(texel).get<uint8_t>();
                lighting.iesProfiles.push_back(std::move(profile));
            }
    }

    if (lighting.primaryLights.empty())
        lighting.primaryLights.resize(2);

    // Replay allocates min(primaryLightCount, 256) proxy slots (0x186D2E0), but indexes them
    // by the combined static/dynamic sorted rank (0x186CA43). Null suffix slots keep that
    // rank in bounds without changing authored light IDs or adding visible lights.
    constexpr size_t sortedLightLimit = 256;
    if (lighting.primaryLights.size() < sortedLightLimit &&
        std::any_of(lighting.primaryLights.begin(), lighting.primaryLights.end(),
                    [](const auto &light) { return light.type == 2 || light.type == 3; }))
    {
        info("local lights: reserve %zu null primary slots for Replay's %zu sorted lights",
             sortedLightLimit - lighting.primaryLights.size(), sortedLightLimit);
        lighting.primaryLights.resize(sortedLightLimit);
    }

    // The source's GSC-scripted lights: the last lights of its table (the IW7 extraction places its padding before
    // them), which the ComWorld and GfxWorld writers give Replay's scriptable ranges.
    if (scriptableLights && scriptableLights->first)
    {
        const auto [count, first] = *scriptableLights;
        if (first + count != lighting.primaryLights.size() || first <= lighting.sunPrimaryLightIndex)
            throw std::runtime_error("source scriptable primary lights are not the last lights of the table");
        lighting.scriptablePrimaryLightCount = count;
        info("lighting: %u script primary light(s) from %u", count, first);
    }

    lighting.intensity *= args.sunIntensityScale;
    if (!std::isfinite(lighting.intensity))
    {
        throw std::runtime_error("scaled sun intensity is not finite");
    }
    info("lighting: %s, scale %.6f, native intensity %.6f", args.lightingProfile.c_str(),
         args.sunIntensityScale, lighting.intensity);
    return lighting;
}

// The attenuation strip of an IW3 light def (raw/lights/<def>: a sampler byte and the image name; the image a
// luminance, RGB or RGBA .iwi whose first row runs from the light to its radius), from the IW3 roots. Empty when
// the files are not there.
std::vector<float> lightDefFalloff(const Args &args, const std::string &definition)
{
    const auto plainName = [](const std::string &name) {
        return !name.empty() && name.size() < 64 && name.find_first_of("/\\:.") == std::string::npos;
    };
    if (!plainName(definition))
        return {};
    std::vector<std::filesystem::path> roots;
    for (const std::string &root : args.iw3Roots)
    {
        roots.emplace_back(root);
        roots.emplace_back(std::filesystem::path(root) / "raw");
    }
    for (const std::string &root : args.searchPaths)
        roots.emplace_back(root);
    for (const auto &root : roots)
    {
        std::vector<uint8_t> def;
        if (!read_file((root / "lights" / definition).string(), def) || def.size() < 3)
            continue;
        const std::string image(reinterpret_cast<const char *>(def.data()) + 1,
                                strnlen(reinterpret_cast<const char *>(def.data()) + 1, def.size() - 1));
        std::vector<uint8_t> iwi;
        if (!plainName(image) || !read_file((root / "images" / (image + ".iwi")).string(), iwi) || iwi.size() < 28 ||
            std::memcmp(iwi.data(), "IWi", 3) != 0 || iwi[3] != 6)
            continue;
        const unsigned format = iwi[4];
        const unsigned width = iwi[6] | (iwi[7] << 8);
        const unsigned height = iwi[8] | (iwi[9] << 8);
        const unsigned bytes = format == 1 ? 4 : format == 2 ? 3 : format == 4 ? 1 : 0;
        if (!bytes || !width || width > 4096 || !height || iwi.size() < 28 + size_t(width) * height * bytes)
            continue;
        // Levels are stored smallest first: the full level ends the file.
        const uint8_t *row = iwi.data() + iwi.size() - size_t(width) * height * bytes;
        std::vector<float> curve(width);
        for (unsigned x = 0; x < width; ++x)
        {
            const unsigned channels = std::min(bytes, 3u);
            float sum = 0.0f;
            for (unsigned channel = 0; channel < channels; ++channel)
                sum += row[x * bytes + channel];
            curve[x] = sum / (255.0f * channels);
        }
        return curve;
    }
    return {};
}

// A falloff strip at a fraction of the radius, filtered and clamped as the light shader samples it.
float sampleFalloff(const std::vector<float> &curve, const double fraction)
{
    const double texel = std::clamp(fraction, 0.0, 1.0) * curve.size() - 0.5;
    const auto first = static_cast<ptrdiff_t>(std::floor(texel));
    const auto at = [&](const ptrdiff_t index) {
        return curve[static_cast<size_t>(std::clamp<ptrdiff_t>(index, 0, ptrdiff_t(curve.size()) - 1))];
    };
    const float weight = static_cast<float>(texel - double(first));
    return at(first) + (at(first + 1) - at(first)) * weight;
}

// IW3 lights a surface with diffuse = color * r_diffuseColorScale (1) * falloff(d / radius),
// in the units of the IW3 sun the world keeps (R_SetLightProperties; light_point_linear
// samples falloff_linear = 1 - d/r). Replay's record builder (0x19A9020) turns ComPrimaryLight
// intensity into sphere-light radiance intensity * color / bulb^2, and the stock forward+ shader
// applies the sphere form factor ~ N.L * bulb^2 / d^2: irradiance = intensity * color * N.L / d^2,
// inside a window that is 1 up to 80% of the radius (1 / distance falloff 0.2). Each IW3 spot or
// omni gets the intensity that gives IW3's brightness where its axis meets the world (a spot's
// lit surface), capped at half its radius. Other light defs (light_point_quadratic: about
// (1 - d/r)^2) scale that by their falloff strip over light_point_linear's at the same point.
void calibrateLocalLights(const Args &args, iw8::MapSun &lighting, const std::vector<float> &solidTriangles)
{
    std::map<std::string, std::vector<float>> falloffs;
    const auto falloffOf = [&](const std::string &definition) -> const std::vector<float> & {
        auto found = falloffs.find(definition);
        if (found == falloffs.end())
            found = falloffs.emplace(definition, lightDefFalloff(args, definition)).first;
        return found->second;
    };
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
        double falloff = 1.0 - reference / light.radius;
        if (light.definition != "light_point_linear")
        {
            const auto &own = falloffOf(light.definition);
            const auto &linear = falloffOf("light_point_linear");
            const double fraction = reference / light.radius;
            if (!own.empty() && !linear.empty() && sampleFalloff(linear, fraction) > 1.0e-3f)
            {
                const double ratio = sampleFalloff(own, fraction) / sampleFalloff(linear, fraction);
                falloff *= ratio;
                info("local light %zu: IW3 light def '%s' falls to %.3f of light_point_linear at %.2f of its "
                     "radius",
                     index, light.definition.c_str(), ratio, fraction);
            }
            else
                warn("local light %zu: IW3 light def '%s' falloff assumed linear (no falloff image found)", index,
                     light.definition.c_str());
        }
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
            {args.replayExecutable, args.collisionPath, args.footstepsPath, args.collisionTiles});
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
        for (const auto &asset : prepared->nativeAssets)
            set(asset.type, zonetool::iw8::get_asset_name(asset.type, asset.header),
                "semantic rumble collection (asset_data/rumbles.json)");
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
        for (const auto &sim : prepared->vfxParticleSims)
            set(ASSET_TYPE_PARTICLESIMANIMATION, sim.name, "particle sim animation of a native effect");
        for (const auto &[type, name] : prepared->vfxReferences)
            set(type, name, "name-only reference to the common-zone asset (debris of a native effect)");
        for (const auto &instance : prepared->scriptableInstances)
            set(ASSET_TYPE_SCRIPTABLE, instance.definition,
                instance.carryZone.empty() ? "name-only reference to the definition of a scriptable instance of the map"
                                           : "stock scriptable definition carried from " + instance.carryZone);
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
                else
                    origin = "IES light definition of the source's local lights (comworld)";
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
        rows.push_back({zone, outputAssetTypeName(type), name, origin.empty() ? "UNEXPLAINED" : origin});
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
                    iw3::PreparedMap *prepared,
                    const convert::CompassBounds *compassBounds)
{
    if (!prepareOutputDirectory(outputDirectory, map))
    {
        return 2;
    }

    LapTimer timing("package");
    const std::string assetName = "maps/mp/" + map + ".d3dbsp";
    // Bot navigation and the native world collision need only the prepared map: they build beside the render load,
    // the lights and the sun shadow (the Replay executable's collision baker stays on this thread).
    std::future<iw8::navigation::Result> pendingNavigation;
    if (prepared && args.navigation)
        pendingNavigation = std::async(std::launch::async, [prepared] { return iw8::navigation::Build(prepared->navigation); });
    std::future<iw8::havok::BakeResult> pendingCollision;
    if (iw8::havok::UsesNativeHavok())
        pendingCollision = std::async(std::launch::async, [&] { return loadCollision(args, dumpDirectory, assetName); });
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
    iw8::MapSun lighting = loadLighting(args, dumpDirectory, assetName);
    const std::string renderPath = path_join(dumpDirectory, assetName + ".render.json");
    const bool renderInMemory = prepared && !prepared->render.is_null();
    if (!renderInMemory && !file_exists(renderPath))
    {
        throw std::runtime_error("native render data is missing: " + renderPath);
    }
    const auto renderMesh =
        renderInMemory ? replayrender::Load(renderPath, prepared->render) : replayrender::Load(renderPath);
    // The materials share the held images' bytes; the held definitions are no longer needed.
    zt::scratch::Release();
    // The definition's tree (millions of nodes) is freed beside the package work; the thread joins on return.
    std::jthread releaseRender;
    if (renderInMemory)
        releaseRender = std::jthread([render = std::move(prepared->render)]() mutable { render = nullptr; });
    timing.lap("render load");
    calibrateLocalLights(args, lighting, renderMesh.solidTriangles);
    // Keep authored shadow flags and the target near-plane default; Replay budgets updates per view.
    timing.lap("local lights");
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
            // A bake past the budget stops as soon as its tiles pass it.
            auto data = replaysunshadow::Bake(
                prepared->shadowScene, {lighting.direction[0], lighting.direction[1], lighting.direction[2]},
                renderMesh.sceneBounds, sampleSize, focus, kSunShadowBudget);
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
        // The crops: the minimap region with a 4096-unit margin, then (IW7 Depot: 112 MB) a 2048-unit
        // one, then 1024 and none (IW7 Flip). Replay's format takes one or two units per texel (the shipped values).
        for (const auto &[margin, sampleSize] : {std::pair{4096.0f, 2.0f}, std::pair{2048.0f, 2.0f},
                                                  std::pair{1024.0f, 2.0f}, std::pair{0.0f, 2.0f}})
        {
            if (baked || !compassBounds)
                break;
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
            baked = bakeSunShadow(sampleSize, &focus);
        }
        // Without a compressed sun shadow Replay shadows entities from its cascades only; the converted
        // surfaces keep their own baked sun depth.
        if (baked)
            sunShadow = std::move(*baked);
        else
            warn("sun-shadow: the compressed sun shadow does not fit Replay's format even at two units per texel over "
                 "the play space; the map ships without it");
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
    if (pendingNavigation.valid())
    {
        navigation = pendingNavigation.get();
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
    auto collision = pendingCollision.valid() ? pendingCollision.get() : loadCollision(args, dumpDirectory, assetName);
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
        std::string shapeless;
        for (const auto &trigger : triggers)
        {
            if (trigger.collisionModel >= collision.models.size())
                throw std::runtime_error("IW3 trigger references a missing collision model");
            const auto &model = collision.models[trigger.collisionModel];
            if (model.shapeIndex == std::numeric_limits<uint16_t>::max())
                shapeless += std::format("{}*{} ({:g} {:g} {:g} .. {:g} {:g} {:g}, {} hulls)", shapeless.empty() ? "" : ", ",
                                         trigger.collisionModel, model.minimum[0], model.minimum[1], model.minimum[2],
                                         model.maximum[0], model.maximum[1], model.maximum[2], model.hullCount);
            collision.triggers.push_back({trigger.collisionModel, trigger.staticPhysics});
        }
        if (!shapeless.empty())
            throw std::runtime_error("IW3 trigger collision models have no native Havok shape: " + shapeless);
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
            prepared ? prepared->staticModelCollisionModels : noStaticCollisionModels,
            prepared ? &prepared->scriptableInstances : nullptr,
            prepared ? &prepared->lootPoints : nullptr, prepared ? prepared->lootZone : std::string(),
            prepared ? &prepared->lootClusters : nullptr, prepared ? &prepared->lootZones : nullptr);
        replayrender::RegisterMapMaterials(writer, techsets, renderMesh);
        replayrender::RegisterReflectionProbeImage(writer, renderMesh);
        replayrender::RegisterVolumetricMaskImages(writer, renderMesh);
        if (glassInitCount)
            iw8::writePhysicsAsset(writer, glassPhysics);
        // Models and effects this zone registers; only these go into the level
        // NetConstStrings (stock names already sit in the common tables).
        std::set<std::string> zoneModels, zoneEffects, scriptModels, scriptEffects, scriptMaterials, scriptAnims;
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
            for (const auto &shape : prepared->vfxShapes)
                iw8::stock::RegisterPhysicsShape(writer, shape.name, shape.havokData);
            for (const auto &sim : prepared->vfxParticleSims)
                iw8::vfx::RegisterParticleSim(writer, sim);
            for (const auto &[type, name] : prepared->vfxReferences)
            {
                iw8::stock::RegisterNameReference(writer, type, name);
                if (type == ASSET_TYPE_MATERIAL)
                    iw8::stock::RegisterNameReference(techsets, type, name);
            }
            // Copies: the output ledger names them after the zone build.
            for (const auto &effect : prepared->vfxEffects)
                iw8::vfx::Register(writer, effect);
            // The definitions of the scriptable instances: name-only references to an always-loaded zone, which
            // the MapEnts instance records alias.
            for (const auto &instance : prepared->scriptableInstances)
                if (instance.carryZone.empty())
                    iw8::stock::RegisterNameReference(writer, ASSET_TYPE_SCRIPTABLE, instance.definition);
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
        // NVG illuminator), as shipped maps carry, with the source lights' IES
        // definitions (IW7); registered before the world.
        iw8::ies::Register(writer, lighting.iesProfiles);
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
        const uint32_t scriptablePrimaryLightCount = lighting.scriptablePrimaryLightCount;
        writer.add(ASSET_TYPE_GFX_MAP, assetName,
                   [assetName, renderPath, &renderMesh, primaryLightCount,
                    sunPrimaryLightIndex, staticModels, dynamicCounts,
                    &sunShadow, sunDirection, &heightfield,
                    &localLights, scriptablePrimaryLightCount](iw8::ZoneWriter &output) {
                       iw8maps::emitGfxMapBody(output, assetName.c_str(), renderPath,
                                               renderMesh, primaryLightCount, sunPrimaryLightIndex,
                                               staticModels, dynamicCounts.models,
                                               dynamicCounts.brushes, sunShadow, sunDirection,
                                               heightfield ? &*heightfield : nullptr,
                                               &localLights, scriptablePrimaryLightCount);
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
                // Stock map scripts call scripts\mp\compass::setupminimap, which besides setminimap leaves the map's
                // size and its two corner entities in level.mapsize / level.mapcorners; the airstrike plane and the
                // support helicopter read them (plane.gsc, chopper_support.gsc). The same values for the region set
                // above (IW8 Radiant research 75, section 4.1): the size is measured along north, each corner faces
                // the other turned by 45 degrees.
                std::snprintf(
                    line, sizeof(line),
                    "    corners = getentarray( \"minimap_corner\", \"targetname\" );\n"
                    "    if ( corners.size == 2 )\n"
                    "    {\n"
                    "        a = ( %.3f + %.3f * k, %.3f + %.3f * k, 0 );\n"
                    "        b = ( %.3f + %.3f * k, %.3f + %.3f * k, 0 );\n"
                    "        north = ( cos( getnorthyaw() ), sin( getnorthyaw() ), 0 );\n"
                    "        size = vectordot( a - b, north );\n"
                    "        if ( size < 0 )\n"
                    "            size = 0 - size;\n"
                    "        corners[0].origin = a;\n"
                    "        corners[1].origin = b;\n"
                    "        corners[0].angles = vectortoangles( b - a ) + ( 0, 45, 0 );\n"
                    "        corners[1].angles = vectortoangles( a - b ) + ( 0, 45, 0 );\n"
                    "        level.mapsize = size;\n"
                    "        level.mapcorners = corners;\n"
                    "    }\n",
                    cx, compassBounds->northwestX - cx, cy, compassBounds->northwestY - cy, cx,
                    compassBounds->southeastX - cx, cy, compassBounds->southeastY - cy);
                generated += line;
            }
            generated += "}\n";
            const std::string scriptSource = prepared->mapScript + "\n" + generated;
            // Names the script resolves at runtime: precachemodel/setmodel through the
            // model table, loadfx through the effect table.
            static const std::regex scriptModel(R"re((?:precachemodel|setmodel)\s*\(\s*"([^"]+)")re",
                                                std::regex::icase);
            static const std::regex scriptEffect(R"re(loadfx\s*\(\s*"([^"]+)")re", std::regex::icase);
            // HUD materials (setshader / precacheshader): named in the level "mat" table, as script models are in
            // "mdl", so a map script can draw stock HUD icons (IW8 Radiant research 36).
            static const std::regex scriptMaterial(R"re((?:setshader|precacheshader)\s*\(\s*"([^"]+)")re",
                                                   std::regex::icase);
            for (auto match = std::sregex_iterator(scriptSource.begin(), scriptSource.end(),
                                                   scriptMaterial);
                 match != std::sregex_iterator(); ++match)
                scriptMaterials.insert((*match)[1].str());
            for (auto match = std::sregex_iterator(scriptSource.begin(), scriptSource.end(),
                                                   scriptModel);
                 match != std::sregex_iterator(); ++match)
                scriptModels.insert((*match)[1].str());
            for (auto match = std::sregex_iterator(scriptSource.begin(), scriptSource.end(),
                                                   scriptEffect);
                 match != std::sregex_iterator(); ++match)
                scriptEffects.insert((*match)[1].str());
            // Stock animations played by name (IW8 Radiant iw8r_anim: scriptmodelplayanim / precachempanim): named in
            // the level "anm" table, as loaded effects are in "vfx"; the xanim itself stays in its stock zone.
            static const std::regex scriptAnim(R"re((?:scriptmodelplayanim|precachempanim)\s*\(\s*"([^"]+)")re",
                                               std::regex::icase);
            for (auto match = std::sregex_iterator(scriptSource.begin(), scriptSource.end(), scriptAnim);
                 match != std::sregex_iterator(); ++match)
                scriptAnims.insert((*match)[1].str());
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
        // New game type scripts of the map (IW8 Radiant research 91 / 92), compiled as they are.
        if (prepared && !prepared->gameTypeScripts.empty())
        {
            if (gscTool.empty())
                throw std::runtime_error("the map carries game type scripts; pass --gsc-tool (or set IW8_GSC_TOOL) to compile them");
            const std::filesystem::path work = std::filesystem::path(path_dir(outputDirectory)) / ".iw3-scripts" / map / "gametypes";
            for (const auto &[name, gameTypeSource] : prepared->gameTypeScripts)
            {
                // A script that does not compile stops the build: a match started with this game type would drop.
                const auto gscbin = iw3::scripts::Compile(gameTypeSource, gscTool, work, name);
                const std::string scriptName = "scripts/mp/gametypes/" + name + ".gsc";
                writer.add(ASSET_TYPE_SCRIPTFILE, scriptName,
                           [scriptName, gscbin](iw8::ZoneWriter &output) {
                               iw8::replay_script::emitCompiledScript(output, scriptName, gscbin);
                           });
                info("scripts: compiled game type script %s (%zu bytes gscbin)", scriptName.c_str(), gscbin.size());
            }
        }
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
            if (prepared)
                scriptMaterials.insert(prepared->scriptMaterials.begin(), prepared->scriptMaterials.end());
            levelStrings[1].assign(scriptMaterials.begin(), scriptMaterials.end()); // "mat"
            // Script-played animations (scriptmodelplayanim) resolve through
            // the level "anm" table, like script models through "mdl".
            if (prepared)
                for (const auto &anim : prepared->xanims)
                    levelStrings[16].push_back(anim.name); // "anm"
            for (const auto &anim : scriptAnims)
                if (std::find(levelStrings[16].begin(), levelStrings[16].end(), anim) == levelStrings[16].end())
                    levelStrings[16].push_back(anim); // "anm": stock animations named by the map script
            if (prepared)
                for (const auto &asset : prepared->nativeAssets)
                    if (asset.type == zonetool::iw8::ASSET_TYPE_RUMBLE)
                        levelStrings[3].push_back(zonetool::iw8::get_asset_name(asset.type, asset.header)); // "rmb"
            info("assets: level NetConstStrings: %zu model(s), %zu effect(s), %zu HUD material(s)", models.size(),
                 effects.size(), scriptMaterials.size());
        }
        iw8::registerMapWorldAssets(
            writer, worldStrings, assetName, mapEntities, bounds, lighting, collision,
            glassInitCount, glassPieceLimit, dynamicCounts,
            prepared && prepared->vfxLightDef == "light_point_linear", levelStrings,
            prepared ? prepared->clientTriggers : nullptr);
        timing.lap("asset registration");
        techsets.build();
        timing.lap("techsets zone build");

        static const std::vector<iw8::ScriptableInstance> noScriptables;
        const auto &scriptableInstances = prepared ? prepared->scriptableInstances : noScriptables;
        static const std::vector<iw3::PreparedMap::CarryAsset> noCarryAssets;
        const ScriptableCarry carry =
            CarryScriptableDefinitions(scriptableInstances, args.replayExecutable, prepared ? prepared->carryAssets : noCarryAssets);
        if (prepared && !prepared->carryAssets.empty())
            info("carry: %zu stock asset(s) asked for, %zu asset(s) for the map zone, %zu for the techsets zone",
                 prepared->carryAssets.size(), carry.assets.size(), carry.techsets.size());
        for (const auto &name : carry.materialNames)
            if (!writer.has(ASSET_TYPE_MATERIAL, name))
                iw8::stock::RegisterNameReference(writer, ASSET_TYPE_MATERIAL, name);
        timing.lap("stock scriptable definitions");

        const auto techsetParams = writeParams(techsets);
        const ZonePrepare techsetPrepare = carry.techsets.empty() ? ZonePrepare{} : [&carry](zonetool::iw8::loaded_zone &zone) {
            auto additions = NewAdditions(zone, carry.techsets);
            info("scriptables: %zu stock technique-set asset(s) added to the techsets zone", additions.size());
            return additions;
        };
        if (!writeZone(path_join(outputDirectory, "techsets_" + map + ".ff"), techsets.takeBody(), techsetParams, {},
                       nullptr, techsetPrepare))
        {
            result = 1;
        }

        // Release the companion's resident body before building the main one.
        writer.build();
        timing.lap("main zone build");
        const auto mapParams = withPostloadMargin(writeParams(writer));
        const auto native = prepared ? std::span<const zonetool::iw8::zone_asset>(prepared->nativeAssets)
                                     : std::span<const zonetool::iw8::zone_asset>{};
        zonetool::iw8::zone_inventory nativeInventory;
        // Streamed collision (research 90): the tile zones and the combined package are written first, because the
        // streaming info of the main zone states what each of them costs to load.
        struct TiledCollision
        {
            std::deque<std::string> names;
            std::vector<zonetool::iw8::CollisionTile> tiles;
            std::vector<zonetool::iw8::TransientFileReference> files;
            std::vector<zonetool::iw8::TransientAssetReference> assets;
            std::vector<std::uint16_t> residentPages, tempPages, contentCounts;
            std::vector<zonetool::iw8::StreamingItemContents> contents;
            zonetool::iw8::TransientInfo transient{};
            zonetool::iw8::StreamingInfo info{};
        } tiled;
        if (collision.tiled)
        {
            namespace zi = zonetool::iw8;
            const auto keep = [&](std::string text) -> const char * {
                tiled.names.push_back(std::move(text));
                return tiled.names.back().c_str();
            };
            const float gridMin = -iw8::havok::kCollisionTileSize * iw8::havok::kCollisionTileGrid * 0.5f;
            tiled.tiles.resize(iw8::havok::kCollisionTileCount);
            for (std::uint32_t index = 0; index < iw8::havok::kCollisionTileCount; ++index)
            {
                auto &tile = tiled.tiles[index];
                const auto gridX = index % iw8::havok::kCollisionTileGrid, gridY = index / iw8::havok::kCollisionTileGrid;
                tile.name = keep("collisiontile" + std::to_string(index));
                tile.index = index;
                tile.gridId[0] = static_cast<std::uint8_t>(gridX);
                tile.gridId[1] = static_cast<std::uint8_t>(gridY);
                // The centre of the tile's square.
                tile.origin.v[0] = gridMin + (static_cast<float>(gridX) + 0.5f) * iw8::havok::kCollisionTileSize;
                tile.origin.v[1] = gridMin + (static_cast<float>(gridY) + 0.5f) * iw8::havok::kCollisionTileSize;
                tile.origin.v[2] = 0.0f;
            }
            for (auto &shape : collision.tiles)
            {
                if (shape.index >= tiled.tiles.size() || shape.data.empty())
                    throw std::runtime_error("collision tile shape is invalid");
                auto &tile = tiled.tiles[shape.index];
                tile.havokTileShapeDataSize = static_cast<std::uint32_t>(shape.data.size());
                tile.havokTileShapeData = reinterpret_cast<char *>(shape.data.data());
                tile.worldCollisionShapeIdx = shape.slot;
            }
            const auto pages = [](const std::uint64_t bytes) {
                return static_cast<std::uint16_t>(std::clamp<std::uint64_t>((bytes + 0xFFFFu) / 0x10000u, 1, 0xFFFFu));
            };
            // One zone from tile structures: a collision zone has transient file type 3 and xfile version 0xFF3, as
            // the stock tiles. Returns its resident and temporary cost in 64 KiB pages (block 5; blocks 1 and 2).
            const auto writeTiles = [&](const std::string &zoneName, const std::span<const zi::zone_asset> list) {
                auto written = zi::write_zone(list, list, {}, {});
                zonetool::xfile::fastfile file{};
                file.payload = std::move(written.data);
                file.block_sizes = written.block_sizes;
                file.xfile_version = 0xFF3;
                file.transient = true;
                file.transient_type = 3;
                file.preload_walk_size = written.preload_walk_size;
                const std::pair<std::uint16_t, std::uint16_t> cost{pages(file.block_sizes.at(5)),
                                                                   pages(file.block_sizes.at(1) + file.block_sizes.at(2))};
                zonetool::xfile::write(path_join(outputDirectory, zoneName + ".ff"), zi::definition().format, file);
                return cost;
            };
            // The files of the streaming info: the map, every tile, the combined package.
            tiled.files.push_back({keep(map), 0});
            tiled.residentPages.push_back(0);
            tiled.tempPages.push_back(0);
            std::vector<zi::zone_asset> all;
            for (std::uint32_t index = 0; index < iw8::havok::kCollisionTileCount; ++index)
            {
                const zi::zone_asset asset{zi::ASSET_TYPE_COLLISION_TILE, &tiled.tiles[index], false};
                all.push_back(asset);
                const auto zoneName = collisionTileZone(map, index);
                const auto cost = writeTiles(zoneName, std::span<const zi::zone_asset>(&asset, 1));
                tiled.files.push_back({keep(zoneName), 3});
                tiled.residentPages.push_back(cost.first);
                tiled.tempPages.push_back(cost.second);
                tiled.assets.push_back({tiled.tiles[index].name, static_cast<std::uint8_t>(zi::ASSET_TYPE_COLLISION_TILE), 0xFFFFFFFFu});
            }
            const auto packageCost = writeTiles(collisionPackageZone(map), all);
            tiled.files.push_back({keep(collisionPackageZone(map)), 3});
            tiled.residentPages.push_back(packageCost.first);
            tiled.tempPages.push_back(packageCost.second);
            tiled.contentCounts.assign(tiled.files.size(), 0);
            tiled.contents.assign(tiled.files.size(), zi::StreamingItemContents{});

            auto &transient = tiled.transient;
            transient.transientFiles = tiled.files.data();
            transient.transientFileCount = static_cast<std::uint32_t>(tiled.files.size());
            transient.transientAssets = tiled.assets.data();
            transient.transientAssetCount = static_cast<std::uint32_t>(tiled.assets.size());
            transient.transientCollisionTileCount = iw8::havok::kCollisionTileCount;
            transient.transientCosts.streamContents.contentCounts = tiled.contentCounts.data();
            transient.transientCosts.streamContents.contents = tiled.contents.data();
            transient.transientCosts.residentPageSizes = tiled.residentPages.data();
            transient.transientCosts.tempPageSizes = tiled.tempPages.data();
            transient.transientCosts.transientCostCount = static_cast<std::uint32_t>(tiled.files.size());
            tiled.info.name = keep(map);
            tiled.info.transientInfo = &transient;
            info("collision tiles: wrote %u tile zones (%zu with collision) and %s (%u resident pages); the main zone "
                 "gets a streaming info with %zu files",
                 iw8::havok::kCollisionTileCount, collision.tiles.size(), collisionPackageZone(map).c_str(),
                 static_cast<unsigned>(packageCost.first), tiled.files.size());
            timing.lap("collision tile zones");
        }

        // Kept alive until the main zone is written: the level model table with carried models.
        std::deque<std::string> carriedModelNames;
        std::vector<const char *> carriedModelList;
        // Carried scriptable definitions join the zone here, and each instance record gets its definition.
        const ZonePrepare mapPrepare = carry.empty() && !collision.tiled ? ZonePrepare{} : [&](zonetool::iw8::loaded_zone &zone) {
            auto additions = NewAdditions(zone, carry.assets);
            if (collision.tiled)
                additions.push_back({zonetool::iw8::ASSET_TYPE_STREAMING_INFO, &tiled.info, false});
            // A carried model that an entity names (MapEnts key 652, for example the model of a vehicle spawn) joins the
            // level's model table, as the map's own models do above: script spawns it by name.
            if (!carry.assets.empty())
            {
                static const std::regex entityModel(R"re((?:^|[\s{])652 "([^"*][^"]*)")re");
                std::set<std::string> named;
                for (auto match = std::sregex_iterator(mapEntities.begin(), mapEntities.end(), entityModel);
                     match != std::sregex_iterator(); ++match)
                    named.insert((*match)[1].str());
                std::set<std::string> carriedModels;
                for (const auto &asset : carry.assets)
                    if (asset.type == zonetool::iw8::ASSET_TYPE_XMODEL)
                        if (const auto name = zonetool::iw8::zone_asset_name(asset.type, asset.header); named.contains(name))
                            carriedModels.insert(name);
                if (!carriedModels.empty())
                    for (const auto &asset : zone.assets)
                    {
                        if (asset.type != zonetool::iw8::ASSET_TYPE_NET_CONST_STRINGS || asset.referenced)
                            continue;
                        auto *table = static_cast<zonetool::iw8::NetConstStrings *>(asset.header);
                        if (static_cast<std::uint32_t>(table->stringType) != 0 || static_cast<std::uint32_t>(table->sourceType) != 3)
                            continue;
                        std::set<std::string> listed;
                        for (std::uint32_t i = 0; i < table->entryCount; i++)
                            listed.insert(table->stringList[i] ? table->stringList[i] : "");
                        const auto before = listed.size();
                        listed.insert(carriedModels.begin(), carriedModels.end());
                        for (const auto &text : listed)
                            carriedModelNames.push_back(text);
                        carriedModelList.clear();
                        for (const auto &text : carriedModelNames)
                            carriedModelList.push_back(text.c_str());
                        table->stringList = carriedModelList.data();
                        table->entryCount = static_cast<std::uint32_t>(carriedModelList.size());
                        info("carry: %zu carried model(s) added to the level model table", listed.size() - before);
                        break;
                    }
            }
            if (carry.definitions.empty())
                return additions;
            std::size_t pointed = 0;
            for (const auto &asset : zone.assets)
            {
                if (asset.type != zonetool::iw8::ASSET_TYPE_MAP_ENTS || asset.referenced)
                    continue;
                auto &block = static_cast<zonetool::iw8::MapEnts *>(asset.header)->scriptableMapEnts;
                if (block.mapInstanceCount != scriptableInstances.size() || (!block.mapInstances && !scriptableInstances.empty()))
                    throw std::runtime_error("the map zone's scriptable instances do not match the extraction");
                for (std::size_t index = 0; index < scriptableInstances.size(); ++index)
                {
                    if (scriptableInstances[index].carryZone.empty())
                        continue;
                    block.mapInstances[index].initialDef = static_cast<zonetool::iw8::ScriptableDef *>(
                        carry.definitions.at(scriptableInstances[index].definition));
                    ++pointed;
                }
            }
            info("scriptables: %zu stock asset(s) added to the map zone, %zu instance(s) given their definition",
                 additions.size(), pointed);
            return additions;
        };
        const bool wantInventory = !native.empty() || !carry.empty() || collision.tiled;
        if (!writeZone(path_join(outputDirectory, map + ".ff"), writer.takeBody(), mapParams, native,
                       wantInventory ? &nativeInventory : nullptr, mapPrepare))
        {
            result = 1;
        }
        timing.lap("zone write");
        auto emittedAssets = writer.assetList();
        if (wantInventory)
        {
            emittedAssets.clear();
            emittedAssets.reserve(nativeInventory.asset_offsets.size());
            for (const auto &[offset, type, name] : nativeInventory.asset_offsets)
                emittedAssets.emplace_back(static_cast<IW8_XAssetType>(type), name);
        }
        for (auto &row : outputLedgerRows(map, emittedAssets, renderMesh, prepared, map, stockCopier.get()))
            ledgerRows.push_back(std::move(row));
        for (auto &row : outputLedgerRows("techsets_" + map, techsets.assetList(), renderMesh, prepared, map,
                                          stockCopier.get()))
            ledgerRows.push_back(std::move(row));
        info("build-map: main zone contains %zu assets",
             !wantInventory ? writer.assetCount() : nativeInventory.list_asset_count);
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
        for (const std::string_view suffix : {" (Classic)", " (IW7)"})
            if (operation.ends_with(suffix))
                operation.resize(operation.size() - suffix.size());
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

int buildMap(const Args &args, iw3::PreparedMap *prepared = nullptr)
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
        // The source draws the compass image with its north (worldspawn northyaw,
        // default 0) up; Replay's minimap (north 90) takes it turned clockwise by
        // 90 - northyaw. Spawn points land on the drawn floor only so: Office and
        // IW7 Paris (no key) at 90, Arnhem and IW7 Afghan, Breakneck, Pixel (90) at 0.
        if (!metadata.compassRotationSet)
        {
            const float turn = std::fmod(std::fmod(90.0f - compassBounds.northYaw, 360.0f) + 360.0f, 360.0f);
            const float quarter = std::round(turn / 90.0f);
            if (std::abs(turn - quarter * 90.0f) < 0.01f)
            {
                metadata.compassRotation = static_cast<unsigned>(quarter * 90.0f) % 360u;
                info("compass: rotating the minimap %u degrees for northyaw %.1f",
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
    options.replayZones = std::filesystem::path(args.replayExecutable).parent_path() / "zone";
    // The install's oo2core compresses the written fastfiles (Oodle Kraken, as stock ones are).
    zt::iw8_set_oodle_directory(std::filesystem::path(args.replayExecutable).parent_path().string());
    options.exposureAdjust = metadata.exposureAdjust;
    options.iw3ModeAssets = args.iw3ModeAssets;
    options.style =
        args.style == "iw8" ? iw3::MapStyle::iw8 : iw3::MapStyle::iw3;
    const auto requestedOutput = args.outputDirectory.empty()
                                     ? std::filesystem::path(args.positional[0]).parent_path() /
                                           (map + "_iw8")
                                     : std::filesystem::path(args.outputDirectory);
    options.scratchRoot = std::filesystem::absolute(requestedOutput).parent_path();
    options.renderCache = args.renderCache.empty() ? options.scratchRoot / ".iw3-render-cache"
                                                   : std::filesystem::absolute(args.renderCache);
    replaysunshadow::SetCacheDirectory(options.renderCache);
    iw8::sound::SetCacheDirectory(options.renderCache);
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
    build.collisionTiles = prepared.collisionTiles;
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
		args.renderCache = option("render_cache");
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

	// The streaming info of a map zone as JSON (the streaminfo command; IW8 Radiant research 90).
	std::string stream_info_json(const std::filesystem::path& zone_path, const std::string& zone_name)
	{
		using namespace zonetool::iw8;
		const auto zone = load_zone(zone_path, {.keep_temp_data = true});
		const StreamingInfo* info = nullptr;
		for (const auto& asset : zone->assets)
		{
			if (asset.type == ASSET_TYPE_STREAMING_INFO && !asset.referenced)
			{
				info = static_cast<const StreamingInfo*>(asset.header);
			}
		}
		nlohmann::ordered_json out;
		out["zone"] = zone_name;
		if (!info)
		{
			out["streaminginfo"] = nullptr;
		}
		else
		{
			out["name"] = info->name ? info->name : "";
			const auto& set = info->alwaysloadedFlagSets[0];
			out["alwaysloaded"] = {{"modelCount", set.modelCount}, {"modelSurfCount", set.modelSurfCount}};
			if (const auto* t = info->transientInfo)
			{
				auto& j = out["transientInfo"];
				j["flags"] = static_cast<std::uint32_t>(t->transientInfoFlags);
				j["forcedStubAssetCount"] = t->forcedStubAssetCount;
				j["transientFileCount"] = t->transientFileCount;
				j["transientAssetCount"] = t->transientAssetCount;
				j["transientWorldGridTileMapCount"] = t->transientWorldGridTileMapCount;
				j["transientCollisionTileCount"] = t->transientCollisionTileCount;
				j["transientMPCommonCount"] = t->transientMPCommonCount;
				j["transientWorldGridBoundsCount"] = t->transientWorldGridBoundsCount;
				j["transientWorldGridHighLodCount"] = t->transientWorldGridHighLodCount;
				j["transientWorldGridHighLodLookupCount"] = t->transientWorldGridHighLodLookupCount;
				j["skipLoading"] = {{"wordCount", t->transientSkipLoading.wordCount}, {"bitCount", t->transientSkipLoading.bitCount}};
				auto sets = nlohmann::ordered_json::array();
				for (std::uint32_t i = 0; i < t->transientSets.count; i++)
				{
					sets.push_back({{"name", t->transientSets.names && t->transientSets.names[i] ? t->transientSets.names[i] : ""},
						{"mask", t->transientSets.masks ? t->transientSets.masks[i].array[0] : 0}});
				}
				j["transientSets"] = sets;
				const auto& costs = t->transientCosts;
				j["costs"] = {{"streamingItemCount", costs.streamingItemCount}, {"transientCostCount", costs.transientCostCount}};
				auto files = nlohmann::ordered_json::array();
				for (std::uint32_t i = 0; i < t->transientFileCount; i++)
				{
					nlohmann::ordered_json file;
					file["name"] = t->transientFiles[i].name ? t->transientFiles[i].name : "";
					file["type"] = t->transientFiles[i].type;
					if (i < costs.transientCostCount)
					{
						if (costs.residentPageSizes) file["residentPages"] = costs.residentPageSizes[i];
						if (costs.tempPageSizes) file["tempPages"] = costs.tempPageSizes[i];
					}
					files.push_back(std::move(file));
				}
				j["transientFiles"] = files;
				auto stream_costs = nlohmann::ordered_json::array();
				for (std::uint32_t i = 0; costs.streamingCosts && i < costs.streamingItemCount && i < 64; i++)
				{
					stream_costs.push_back(costs.streamingCosts[i].costInBytes);
				}
				j["streamingCostsHead"] = stream_costs;
				auto content_counts = nlohmann::ordered_json::array();
				for (std::uint32_t i = 0; costs.streamContents.contentCounts && i < costs.transientCostCount && i < 4096; i++)
				{
					content_counts.push_back(costs.streamContents.contentCounts[i]);
				}
				j["contentCounts"] = content_counts;
				auto assets = nlohmann::ordered_json::array();
				for (std::uint32_t i = 0; i < t->transientAssetCount; i++)
				{
					assets.push_back({{"name", t->transientAssets[i].name ? t->transientAssets[i].name : ""},
						{"type", t->transientAssets[i].type}, {"mpParentIndex", t->transientAssets[i].mpParentIndex}});
				}
				j["transientAssets"] = assets;
				auto stubs = nlohmann::ordered_json::array();
				for (std::uint32_t i = 0; i < t->forcedStubAssetCount; i++)
				{
					stubs.push_back({{"name", t->forcedStubAssets[i].name ? t->forcedStubAssets[i].name : ""}, {"type", t->forcedStubAssets[i].type}});
				}
				j["forcedStubAssets"] = stubs;
				auto maps = nlohmann::ordered_json::array();
				for (std::uint32_t i = 0; i < t->transientWorldGridTileMapCount; i++)
				{
					const auto& m = t->transientWorldGridTileMaps[i];
					nlohmann::ordered_json map = {{"indexCount", m.indexCount}, {"level", m.level}, {"lodCount", m.lodCount},
						{"gridCountX", m.gridCountX}, {"gridCountY", m.gridCountY}, {"cellSize", m.cellSize}, {"minX", m.minX}, {"minY", m.minY}};
					auto head = nlohmann::ordered_json::array();
					for (std::uint32_t k = 0; m.spaceToTransientIndex && k < m.indexCount && k < 48; k++)
					{
						head.push_back(m.spaceToTransientIndex[k]);
					}
					map["indicesHead"] = head;
					maps.push_back(std::move(map));
				}
				j["worldGridTileMaps"] = maps;
			}
			else
			{
				out["transientInfo"] = nullptr;
			}
		}
		return out.dump(1);
	}

	// editzone (IW8 Radiant research 88): a stock map zone is loaded, changed in memory and written again with the
	// choices of this writer, as the converter's own map zones are (the stock file's own pointer layout cannot be
	// kept: it aliases data by offset, and an edit moves the data). Every definition an edit does not touch is
	// written from the stock definition in memory. Edits (a JSON file):
	//   "entities": path of an IW8 entity text (numeric keys, as the game stores it). A stock map keeps its spawn
	//       points only in a compiled list, not in the text; the editor shows them as mp_*_spawn* entities. When the
	//       text has such entities they become the compiled list (in file order) and are taken out of the text;
	//       when it has none the stock list stays. "rebuild_spawns": false keeps the stock list in every case;
	//   "hide_static_models": indices into GfxWorld.smodels.smodelInstanceData whose scale becomes 0;
	//   "carry": [{"zone", "type", "name"}] assets of other stock zones added with everything they use (their
	//       materials and technique sets go to the techsets companion); a carried xmodel is also named in the
	//       level's model table so that a script_model entity can use it.
	//   "loot_points": [{"origin": [x, y, z], "angles": [p, y, r], "cluster": n | "free" | "cache" | "auto",
	//       "zone": n | "auto"}] replaces the loot points of the map (MapEnts.spawnGroupLoot.points; IW8 Radiant
	//       research 87 section 2.4 and 89). The clusters and zones of the stock map are authored volumes and stay;
	//       their point lists, and the cluster list of each zone, are rebuilt from the points. "cluster" is a cluster
	//       index, "free" (0xFFFD: a point outside every cluster), "cache" (0xFFFB: a loot cache point) or "auto" (the
	//       first cluster whose box holds the point, else free); "zone" is 0 for none, a 1-based zone number, or "auto"
	//       (the first zone whose box holds the point, else none). The scriptable counts that include the loot count
	//       move with it.
	//   "replace_materials": [{"from": "tw/sky_mp_backlot2", "to": "tw/sky_mp_shipment"}] points every world surface
	//       (GfxWorld surfaces) of material "from" at material "to": the stock sky, or a stock texture swapped. "to" must
	//       be in this zone or carried in with "carry" (type material). The surfaces' draw keys are left as they are: the
	//       engine fills them at load (the converter's own maps write zeros and draw). "from": "@sky" names the map's sky
	//       (any world material <folder>/sky_...).
	//   "sun": {"color": [r, g, b], "intensity": x, "direction": [pitch, yaw], "acts": "<acts.exe>"} sets the colour
	//       (linear sRGB), the intensity and / or the direction of the stock sun: the ComWorld primary light the first
	//       stage names (clipMap_t.stages[0].sunPrimaryLightIndex). "direction" is in degrees as the worldspawn
	//       sundirection (a negative pitch is above the horizon, -90 to -1): toward the sun (cos p cos y, cos p sin y,
	//       -sin p) (stock mp_shipment: -20.261 -15.4 = its light's 0.904 -0.249 0.346). A new direction also re-bakes
	//       the GfxWorld's compressed sun shadow (GfxWorldDraw.compressedSunShadow and its params; Replay drops a sun
	//       shadow whose stored direction is not the sun's) with the converter's own baker (replay_sunshadow) at the stock
	//       texel size: the casters are the lit-opaque world surfaces of the zone (not the sky) and the static models of
	//       the casting collections (STATIC_MODEL_COLLECTION_FLAG_SUN_SHADOW_1, hidden ones excepted), whose LOD0
	//       geometry is streamed and is exported by ACTS ("acts", else IW8_ACTS; --geometry with the game's XPak files),
	//       over the box of the casting world surfaces + 1024. Cut-out (alpha-tested) leaves cast as solid cards. The
	//       vision's fogSunDir turns by the same pitch and yaw change, and the worldspawn's sundirection (key 968) is
	//       rewritten. What keeps the old sun: the baked lightmaps and light grid (the bounce light of the old sun) and the
	//       sky texture. A map whose sun shadow is in streamed transient zones (Warzone) is refused.
	//   "loot_boxes": {"clusters": [{"mid": [x, y, z], "half": [x, y, z]}], "zones": [{"name": "...", "mid": [...],
	//       "half": [...]}]} adds loot clusters and named loot zones after the stock ones (IW8 Radiant research 89
	//       section 9). The stock boxes stay. A new zone's name is what the loot tables pick the loot by, so it should
	//       be a name the map's tables know. A new cluster's zone is the first zone whose box holds its middle (0:
	//       none). With "loot_points" (applied after this) "auto" points find the new boxes like the stock ones.
	//   "scriptable_instances": {"move": [{"index": n, "origin": [x, y, z], "angles": [p, y, r]}],
	//       "add": [{"like": n, "origin": [x, y, z], "angles": [p, y, r], "server_instanced_parts": n, "usable_parts": n,
	//       "physics_limit": bool, "physics_limit_detail": bool}], "remove": [{"index": n, <the same numbers>}]} moves
	//       placed scriptables (MapEnts scriptableMapEnts.mapInstances), adds copies of one and removes them (IW8 Radiant
	//       research 89 section 8). A copy takes the definition, the class name, the flags, the targetname and the script_noteworthy of instance "like"; the
	//       instance count, the three server counts and the physics limits rise (or fall, for "remove") by the numbers
	//       given (those of the definition, from the editor's catalogue). The dynamic entities of an instance follow: a
	//       move gives them the new pose and spatial partition, a copy gets copies of the entries of "like" (signs,
	//       fences, props and doors: radial and sparse-occluder entries, and no-spatial ones), a removal drops them; the
	//       lists, chains, counts and the GfxWorld client arrays are rebuilt. Refused: streamed (transient) entries,
	//       entries placed apart from their instance, and a removal when a list in a streamed zone names a later instance.
	//   "remove_world_surfaces": {"boxes": [{"mins": [x, y, z], "maxs": [x, y, z], "render": true, "collision": true}],
	//       "surfaces": [n, ...], "collision": true, "rebake_sun_shadow": false, "acts": "<acts.exe>"} takes world
	//       geometry out (a box with "render": false only its collision, "collision": false only its triangles) (IW8 Radiant
	//       research 96). Render: every world triangle whose three vertices lie inside one box, and every triangle of the
	//       listed surfaces (GfxWorld surface indices), becomes degenerate (its three indices the first one), so no draw
	//       path (opaque, prepass, shadow maps, decals) rasterizes it; counts, offsets and surface indices stay stock.
	//       Collision (boxes only; "collision": false keeps it): the triangles and convex shapes (brush hulls) of the
	//       world's Havok compressed mesh (clipMap_t.havokWorldShapeData) that lie inside a box leave it in place, as
	//       Havok's own primitive removal does (removed marker DE AD DE AD, a quad keeps its other half); nothing else
	//       moves. A map with collision tiles is refused. "rebake_sun_shadow": true bakes
	//       the compressed sun shadow again at the stock sun direction without the removed triangles (as "sun.direction";
	//       "acts" or IW8_ACTS for the static models). "occlusion": "auto" (default) | "open" | "stock": the stock Umbra
	//       tome was baked with the occluder surfaces (GFX_SURF_IS_UMBRA_OCCLUDER) as walls, so a removed occluder would
	//       still hide what was behind it; "auto" replaces the tome with the conservative one of the converted maps (the
	//       same objects and user IDs, one cell: Umbra culls nothing, the GPU passes cull) when a removed triangle belongs
	//       to an occluder surface, "open" always, "stock" keeps it. A tome with gates is refused. What stays: the
	//       lightmaps, light grid and probes (baked contact shadows), static models (hide them with hide_static_models /
	//       remove_static_model_collision) and the stock sun shadow unless re-baked.
	//   "stock_patches": {"<zone>.fp": "<file>" | ""} names, per patch file of the game's zone folder, the stock patch
	//       to read that zone with ("" = the stock game has no patch of that name). The game ships patches with real
	//       map data (mp_br_quarry.fp, mp_donetsk.fp, ...), and an installed edit replaces the patch of its zone: with
	//       the saved stock patch named here the edit starts from the stock map again, not from the installed edit.
	//       Without the key every zone is read with the patch beside it, as the game reads it.
	//   "stock_zones": {"<zone>.ff": "<file>"} names, per zone file of the game's zone folder, the file to read in its
	//       place (with the patch beside that file). The game takes a patch of a signed zone only with its own
	//       signature, so an edit is installed as a whole unsigned zone in place of the stock file, which is kept
	//       elsewhere: named here, the edit starts from the stock zone again (IW8 Radiant research 116).
	// Output: <output>/<zone>.ff, techsets_<zone>.ff when it changed, <output>/patch/<same names>.fp (the patches that
	// turn the stock files into them; "patch": false leaves them out) and <zone>.edit-report.json.
	void edit_zone(const std::filesystem::path& game_path, const std::string& zone_name,
		const std::filesystem::path& edits_path, const std::filesystem::path& output)
	{
		using namespace zonetool::iw8;
		const auto zone_folder = game_path / "zone";
		const auto zone_path = zone_folder / (zone_name + ".ff");
		if (!std::filesystem::exists(zone_path))
		{
			throw std::runtime_error("stock zone " + zone_path.string() + " was not found");
		}

		std::ifstream input(edits_path, std::ios::binary);
		if (!input)
		{
			throw std::runtime_error("cannot read the edits file " + edits_path.string());
		}
		const auto edits = nlohmann::json::parse(input);
		nlohmann::ordered_json report;
		report["zone"] = zone_name;

		if (edits.contains("stock_patches"))
		{
			const auto& named = edits.at("stock_patches");
			if (!named.is_object())
			{
				throw std::runtime_error("stock_patches must be an object: patch file name -> stock patch file or \"\"");
			}
			auto list = nlohmann::ordered_json::array();
			for (const auto& [name, value] : named.items())
			{
				std::filesystem::path patch_name(name);
				if (!value.is_string() || patch_name.has_parent_path() || patch_name.extension() != ".fp")
				{
					throw std::runtime_error("stock_patches: \"" + name + "\" must be a patch file name (<zone>.fp) with a file path or \"\"");
				}
				const auto file = value.get<std::string>();
				if (!file.empty() && !std::filesystem::exists(std::filesystem::path(file)))
				{
					throw std::runtime_error("stock_patches: the stock patch " + file + " was not found");
				}
				auto zone_file = zone_folder / patch_name;
				zone_file.replace_extension(".ff");
				zonetool::xfile::use_patch_file(zone_file, std::filesystem::path(file));
				list.push_back(name + (file.empty() ? " (no stock patch)" : ""));
			}
			report["stock_patches"] = list;
		}

		if (edits.contains("stock_zones"))
		{
			const auto& named = edits.at("stock_zones");
			if (!named.is_object())
			{
				throw std::runtime_error("stock_zones must be an object: zone file name -> stock zone file");
			}
			auto list = nlohmann::ordered_json::array();
			for (const auto& [name, value] : named.items())
			{
				const std::filesystem::path file_name(name);
				if (!value.is_string() || file_name.has_parent_path() || file_name.extension() != ".ff")
				{
					throw std::runtime_error("stock_zones: \"" + name + "\" must be a zone file name (<zone>.ff) with a file path");
				}
				const auto file = value.get<std::string>();
				if (file.empty() || !std::filesystem::exists(std::filesystem::path(file)))
				{
					throw std::runtime_error("stock_zones: the stock zone " + file + " was not found");
				}
				zonetool::xfile::use_zone_file(zone_folder / file_name, std::filesystem::path(file));
				list.push_back(name);
			}
			report["stock_zones"] = list;
		}

		auto zone = load_zone(zone_path, {.keep_temp_data = true});
		const auto find = [&](const std::int32_t type) -> void*
		{
			for (const auto& asset : zone->assets)
			{
				if (asset.type == type && !asset.referenced)
				{
					return asset.header;
				}
			}
			return nullptr;
		};

		// Kept alive until the zone is written.
		std::string entity_text;
		std::vector<SpawnPointEntityRecord> spawn_records;
		std::vector<std::vector<const char*>> string_lists;
		std::deque<std::string> names;

		if (edits.contains("entities"))
		{
			auto* ents = static_cast<MapEnts*>(find(ASSET_TYPE_MAP_ENTS));
			if (!ents)
			{
				throw std::runtime_error(zone_name + " has no map entities");
			}
			std::ifstream text(std::filesystem::path(edits.at("entities").get<std::string>()), std::ios::binary);
			if (!text)
			{
				throw std::runtime_error("cannot read the entity text " + edits.at("entities").get<std::string>());
			}
			entity_text.assign(std::istreambuf_iterator<char>(text), {});
			while (!entity_text.empty() && entity_text.back() == '\0')
			{
				entity_text.pop_back();
			}
			if (entity_text.empty() || entity_text.size() > 0x7FFFFFF0u)
			{
				throw std::runtime_error("the entity text is empty or too large");
			}
			report["entity_chars_before"] = ents->numEntityChars;
			const ::iw8::ReplaySpawns spawns(entity_text);
			const auto rebuild_spawns = edits.value("rebuild_spawns", true) && !spawns.records.empty();
			if (rebuild_spawns)
			{
				// The spawn entities leave the text, as in a stock map (the same test as ReplaySpawns: classname key 212
				// starting with mp_ and holding _spawn).
				static const std::regex entity(R"(\{([^{}]*)\}\r?\n?)");
				static const std::regex classname(R"re((?:^|\n)212\s+"([^"]*)")re");
				std::string kept;
				kept.reserve(entity_text.size());
				std::size_t last = 0, removed = 0;
				for (auto match = std::sregex_iterator(entity_text.begin(), entity_text.end(), entity);
					match != std::sregex_iterator(); ++match)
				{
					const auto body = (*match)[1].str();
					std::smatch name;
					const auto spawn = std::regex_search(body, name, classname) && name[1].str().compare(0, 3, "mp_") == 0 &&
						name[1].str().find("_spawn") != std::string::npos;
					if (!spawn)
					{
						continue;
					}
					kept.append(entity_text, last, static_cast<std::size_t>(match->position()) - last);
					last = static_cast<std::size_t>(match->position() + match->length());
					++removed;
				}
				kept.append(entity_text, last, std::string::npos);
				entity_text = std::move(kept);
				report["spawn_entities_taken_out"] = removed;
			}
			ents->entityString = entity_text.data(); // c_str: the terminator is part of numEntityChars
			ents->numEntityChars = static_cast<std::int32_t>(entity_text.size() + 1);
			report["entity_chars"] = ents->numEntityChars;

			if (rebuild_spawns)
			{
				// The compiled spawn list: one record per mp_*_spawn* entity in file order (checked: the records rebuilt
				// from the stock spawns of mp_shipment written as entities equal the stock records).
				const auto id = [&](const std::uint32_t index) -> scr_string_t
				{
					// A spawn without a target or a script_noteworthy holds the empty string, not the null string, as
					// the stock records do.
					return static_cast<scr_string_t>(SL_GetString(spawns.strings.at(index).data()));
				};
				spawn_records.reserve(spawns.records.size());
				for (const auto& record : spawns.records)
				{
					SpawnPointEntityRecord native{};
					native.index = record.index;
					native.name = id(record.name);
					native.target = id(record.target);
					native.script_noteworthy = id(record.noteworthy);
					for (auto axis = 0; axis < 3; axis++)
					{
						native.origin.v[axis] = record.origin[axis];
						native.angles.v[axis] = record.angles[axis];
					}
					spawn_records.push_back(native);
				}
				report["spawns_before"] = ents->spawnList.spawnsCount;
				ents->spawnList.spawnsCount = static_cast<std::uint16_t>(spawn_records.size());
				ents->spawnList.spawns = spawn_records.empty() ? nullptr : spawn_records.data();
				report["spawns"] = spawn_records.size();
			}
		}

		if (edits.contains("hide_static_models"))
		{
			auto* world = static_cast<GfxWorld*>(find(ASSET_TYPE_GFXWORLD));
			if (!world || !world->smodels.smodelInstanceData)
			{
				throw std::runtime_error(zone_name + " has no static model instances");
			}
			std::size_t hidden = 0;
			for (const auto& entry : edits.at("hide_static_models"))
			{
				const auto index = entry.get<std::uint64_t>();
				if (index >= world->smodels.instanceCount)
				{
					throw std::runtime_error("hide_static_models: instance " + std::to_string(index) + " is beyond the " +
						std::to_string(world->smodels.instanceCount) + " instances of the map");
				}
				world->smodels.smodelInstanceData[index].scale = 0.0f;
				++hidden;
			}
			report["static_model_instances"] = world->smodels.instanceCount;
			report["hidden_static_models"] = hidden;
		}

		//   "remove_static_model_collision": [{"model": name, "origin": [x, y, z]}] takes the collision of those static
		//   models out (IW8 Radiant research 88, roadmap 11.5): the instance of that model at that place (within half a
		//   unit) leaves the collision map's static model collision lists. A big map keeps that collision in its
		//   collision tiles instead, which this does not edit: the report says how many were found.
		// Kept alive until the zone is written.
		std::deque<std::vector<StaticModelCollisionInstance>> kept_collision;
		if (edits.contains("remove_static_model_collision"))
		{
			auto* clip = static_cast<clipMap_t*>(find(ASSET_TYPE_CLIPMAP));
			if (!clip)
			{
				throw std::runtime_error(zone_name + " has no collision map");
			}
			struct Wanted
			{
				std::string model;
				vec3_t origin{};
				bool found{};
			};
			std::vector<Wanted> wanted;
			for (const auto& entry : edits.at("remove_static_model_collision"))
			{
				Wanted w;
				// The world names a model with its folder (tm/<name>), the collision lists by the name alone.
				w.model = entry.at("model").get<std::string>();
				if (const auto slash = w.model.find_last_of("/\\"); slash != std::string::npos)
				{
					w.model = w.model.substr(slash + 1);
				}
				const auto& o = entry.at("origin");
				if (!o.is_array() || o.size() != 3)
				{
					throw std::runtime_error("remove_static_model_collision: origin is not three numbers");
				}
				for (auto axis = 0; axis < 3; axis++)
				{
					w.origin.v[axis] = o[axis].get<float>();
				}
				wanted.push_back(std::move(w));
			}
			const auto text_of = [&](const scr_string_t name) -> std::string
			{
				const auto index = static_cast<std::size_t>(name);
				return index < zone->script_strings.size() && zone->script_strings[index] ? zone->script_strings[index] : "";
			};
			std::size_t removed = 0;
			for (std::uint32_t l = 0; l < clip->numStaticModelCollisionModelLists; l++)
			{
				auto& list = clip->staticModelCollisionModelLists[l];
				for (std::uint32_t m = 0; m < list.numModels; m++)
				{
					auto& model = list.models[m];
					auto name = text_of(model.name);
					if (const auto slash = name.find_last_of("/\\"); slash != std::string::npos)
					{
						name = name.substr(slash + 1);
					}
					auto& keep = kept_collision.emplace_back();
					for (std::uint32_t i = 0; i < model.numInstances; i++)
					{
						const auto& instance = model.instances[i];
						bool drop = false;
						for (auto& w : wanted)
						{
							if (w.found || w.model != name)
							{
								continue;
							}
							bool close_enough = true;
							for (auto axis = 0; axis < 3; axis++)
							{
								close_enough &= std::abs(instance.origin.v[axis] - w.origin.v[axis]) <= 0.5f;
							}
							if (close_enough)
							{
								w.found = drop = true;
								break;
							}
						}
						if (drop)
						{
							++removed;
						}
						else
						{
							keep.push_back(instance);
						}
					}
					if (keep.size() != model.numInstances)
					{
						model.numInstances = static_cast<std::uint32_t>(keep.size());
						model.instances = keep.empty() ? nullptr : keep.data();
					}
				}
			}
			report["static_model_collision_asked"] = wanted.size();
			report["static_model_collision_removed"] = removed;
		}

		// Kept alive until the zone is written.
		std::deque<std::vector<std::uint16_t>> kept_world_indices;
		std::vector<std::uint8_t> kept_world_collision;
		std::vector<std::uint8_t> kept_world_tome;
		bool rebake_sun_for_removal = false;
		std::string rebake_acts;
		if (edits.contains("remove_world_surfaces"))
		{
			const auto& node = edits.at("remove_world_surfaces");
			if (!node.is_object())
			{
				throw std::runtime_error("remove_world_surfaces must be an object with \"boxes\" and / or \"surfaces\"");
			}
			std::vector<::iw8::stockworld::Box> boxes, collision_boxes;
			if (node.contains("boxes"))
			{
				if (!node.at("boxes").is_array())
				{
					throw std::runtime_error("remove_world_surfaces.boxes must be a list of {\"mins\", \"maxs\"}");
				}
				std::size_t at = 0;
				for (const auto& entry : node.at("boxes"))
				{
					const auto where = "remove_world_surfaces.boxes[" + std::to_string(at++) + "]";
					::iw8::stockworld::Box box;
					for (const char* key : {"mins", "maxs"})
					{
						if (!entry.is_object() || !entry.contains(key) || !entry.at(key).is_array() || entry.at(key).size() != 3)
						{
							throw std::runtime_error(where + ": " + key + " is not three numbers");
						}
						auto& out = std::string(key) == "mins" ? box.mins : box.maxs;
						for (auto axis = 0; axis < 3; axis++)
						{
							out[axis] = entry.at(key)[axis].get<float>();
							if (!std::isfinite(out[axis]))
							{
								throw std::runtime_error(where + ": " + key + " is not finite");
							}
						}
					}
					for (auto axis = 0; axis < 3; axis++)
					{
						if (box.mins[axis] > box.maxs[axis])
						{
							throw std::runtime_error(where + ": mins is above maxs");
						}
					}
					for (const char* key : {"render", "collision"})
					{
						if (entry.contains(key) && !entry.at(key).is_boolean())
						{
							throw std::runtime_error(where + ": " + key + " must be true or false");
						}
					}
					if (entry.value("render", true))
					{
						boxes.push_back(box);
					}
					if (entry.value("collision", true))
					{
						collision_boxes.push_back(box);
					}
				}
			}
			std::set<std::uint32_t> whole;
			if (node.contains("surfaces"))
			{
				if (!node.at("surfaces").is_array())
				{
					throw std::runtime_error("remove_world_surfaces.surfaces must be a list of surface indices");
				}
				for (const auto& entry : node.at("surfaces"))
				{
					whole.insert(entry.get<std::uint32_t>());
				}
			}
			if (boxes.empty() && collision_boxes.empty() && whole.empty())
			{
				throw std::runtime_error("remove_world_surfaces names no box and no surface");
			}
			auto* world = static_cast<GfxWorld*>(find(ASSET_TYPE_GFXWORLD));
			if (!world)
			{
				throw std::runtime_error(zone_name + " has no GfxWorld");
			}
			const auto render = ::iw8::stockworld::RemoveRenderTriangles(*world, boxes, whole, kept_world_indices);
			nlohmann::ordered_json out;
			out["boxes"] = boxes.size();
			out["collision_boxes"] = collision_boxes.size();
			out["surfaces_asked"] = whole.size();
			out["triangles_removed"] = render.triangles;
			out["triangles_already_degenerate"] = render.alreadyDegenerate;
			out["surfaces_touched"] = render.touched.size();
			out["surfaces_emptied"] = render.emptied;
			out["surfaces_not_resident"] = render.notResident;
			ZONETOOL_INFO("editzone: remove_world_surfaces: %zu triangle(s) of %zu surface(s) no longer draw (%zu surface(s) emptied)",
				render.triangles, render.touched.size(), render.emptied.size());

			// Occlusion: the stock Umbra tome was baked with the occluder surfaces (GFX_SURF_IS_UMBRA_OCCLUDER) as walls.
			// "auto" (default) replaces it with the conservative tome when a removed triangle belongs to an occluder
			// surface, "open" always, "stock" never (research 96, "Occlusion").
			{
				const auto mode = node.value("occlusion", std::string("auto"));
				if (mode != "auto" && mode != "open" && mode != "stock")
				{
					throw std::runtime_error("remove_world_surfaces.occlusion must be \"auto\", \"open\" or \"stock\"");
				}
				std::vector<std::uint32_t> occluders;
				for (const auto s : render.touched)
				{
					if (world->surfaces.surfaces[s].flags & GFX_SURF_IS_UMBRA_OCCLUDER)
					{
						occluders.push_back(s);
					}
				}
				nlohmann::ordered_json occlusion;
				occlusion["mode"] = mode;
				occlusion["occluder_surfaces_touched"] = occluders;
				const bool open = mode == "open" || (mode == "auto" && !occluders.empty());
				occlusion["opened"] = open;
				if (open)
				{
					if (!world->umbraTomeData || !world->umbraTomeSize || world->numUmbraGates)
					{
						throw std::runtime_error(zone_name + (world->numUmbraGates ? " has Umbra gates" : " has no Umbra tome") +
							": its occlusion cannot be opened (\"occlusion\": \"stock\" keeps it)");
					}
					const std::span<const std::uint8_t> stock_tome(world->umbraTomeData, world->umbraTomeSize);
					const auto before = ::iw8::stockworld::ReadTome(stock_tome);
					::iw8::stockworld::OcclusionOpening opening;
					kept_world_tome = ::iw8::stockworld::OpenOcclusion(stock_tome, opening);
					world->umbraTomeData = kept_world_tome.data();
					world->umbraTomeSize = static_cast<std::uint32_t>(kept_world_tome.size());
					world->umbraTome = nullptr;
					occlusion["objects"] = opening.objects;
					occlusion["objects_by_class"] = opening.classes;
					occlusion["stock_tiles"] = before.tiles;
					occlusion["stock_leaf_tiles"] = before.leafTiles;
					occlusion["stock_clusters"] = before.clusters;
					occlusion["tome_bytes_before"] = opening.bytesBefore;
					occlusion["tome_bytes_after"] = opening.bytesAfter;
					ZONETOOL_INFO("editzone: remove_world_surfaces: %zu occluder surface(s) touched; the Umbra tome (%zu bytes, %u tiles) "
						"is now the conservative one (%zu bytes, %u objects): Umbra hides nothing, the GPU culls",
						occluders.size(), opening.bytesBefore, before.tiles, opening.bytesAfter, opening.objects);
				}
				out["occlusion"] = occlusion;
			}
			if (node.value("collision", true) && !collision_boxes.empty())
			{
				auto* clip = static_cast<clipMap_t*>(find(ASSET_TYPE_CLIPMAP));
				if (!clip || !clip->havokWorldShapeData || !clip->havokWorldShapeDataSize)
				{
					throw std::runtime_error(zone_name + " has no world collision shape");
				}
				const auto& compendium = zonetool::iw8::havok::replay_compendium(zone_folder);
				::iw8::stockworld::CollisionRemoval collision;
				kept_world_collision = ::iw8::stockworld::RemoveCollision(std::span(reinterpret_cast<const std::uint8_t*>(
					clip->havokWorldShapeData), clip->havokWorldShapeDataSize), compendium, collision_boxes, collision);
				if (collision.trianglesRemoved || collision.convexRemoved)
				{
					clip->havokWorldShapeData = reinterpret_cast<char*>(kept_world_collision.data());
					clip->havokWorldShapeDataSize = static_cast<std::uint32_t>(kept_world_collision.size());
				}
				out["collision"] = {{"shapes", collision.shapes}, {"triangles_before", collision.trianglesBefore},
					{"triangles_removed", collision.trianglesRemoved}, {"triangles_after", collision.trianglesAfter},
					{"convex_before", collision.convexBefore}, {"convex_removed", collision.convexRemoved},
					{"convex_after", collision.convexAfter}, {"bytes_before", collision.bytesBefore},
					{"bytes_after", collision.bytesAfter}};
				ZONETOOL_INFO("editzone: remove_world_surfaces: collision %zu of %zu triangle(s) and %zu of %zu convex shape(s) removed",
					collision.trianglesRemoved, collision.trianglesBefore, collision.convexRemoved, collision.convexBefore);
			}
			rebake_sun_for_removal = node.value("rebake_sun_shadow", false);
			rebake_acts = node.value("acts", std::string{});
			out["rebake_sun_shadow"] = rebake_sun_for_removal;
			report["remove_world_surfaces"] = out;
		}

		// Kept alive until the zone is written.
		std::vector<std::uint8_t> sun_shadow_bytes;
		std::deque<std::vector<std::uint8_t>> vision_buffers;
		std::string sun_entity_text;
		// remove_world_surfaces "rebake_sun_shadow" without a new sun direction: the stock direction baked again.
		const bool rebake_only = rebake_sun_for_removal &&
			!(edits.contains("sun") && edits.at("sun").is_object() && edits.at("sun").contains("direction"));
		if (edits.contains("sun") || rebake_only)
		{
			auto node = edits.contains("sun") ? edits.at("sun") : nlohmann::json::object();
			if (node.is_object() && !rebake_acts.empty() && !node.contains("acts"))
			{
				node["acts"] = rebake_acts;
			}
			if (!node.is_object() || (!node.contains("color") && !node.contains("intensity") && !node.contains("direction") && !rebake_only))
			{
				throw std::runtime_error("sun must be an object with \"color\", \"intensity\" and / or \"direction\"");
			}
			auto* clip = static_cast<clipMap_t*>(find(ASSET_TYPE_CLIPMAP));
			auto* com = static_cast<ComWorld*>(find(ASSET_TYPE_COM_MAP));
			if (!clip || !com || !clip->stageCount || !clip->stages)
			{
				throw std::runtime_error(zone_name + " has no stage or no ComWorld: the sun cannot be found");
			}
			const auto index = clip->stages[0].sunPrimaryLightIndex;
			if (index >= com->primaryLightCount || !com->primaryLights)
			{
				throw std::runtime_error(zone_name + ": the sun primary light " + std::to_string(index) + " is beyond the map's " +
					std::to_string(com->primaryLightCount));
			}
			auto& sun = com->primaryLights[index];
			report["sun_primary_light"] = index;
			report["sun_color_before"] = {sun.colorLinearSrgb.v[0], sun.colorLinearSrgb.v[1], sun.colorLinearSrgb.v[2]};
			report["sun_intensity_before"] = sun.intensity;
			if (node.contains("color"))
			{
				const auto& c = node.at("color");
				if (!c.is_array() || c.size() != 3)
				{
					throw std::runtime_error("sun.color is not three numbers");
				}
				for (auto k = 0; k < 3; k++)
				{
					const auto value = c[k].get<float>();
					if (!std::isfinite(value) || value < 0.0f || value > 16.0f)
					{
						throw std::runtime_error("sun.color values must be 0 to 16 (linear)");
					}
					sun.colorLinearSrgb.v[k] = value;
				}
			}
			if (node.contains("intensity"))
			{
				const auto value = node.at("intensity").get<float>();
				if (!std::isfinite(value) || value < 0.0f || value > 1000000.0f)
				{
					throw std::runtime_error("sun.intensity must be 0 to 1000000");
				}
				sun.intensity = value;
			}
			report["sun_color"] = {sun.colorLinearSrgb.v[0], sun.colorLinearSrgb.v[1], sun.colorLinearSrgb.v[2]};
			report["sun_intensity"] = sun.intensity;
			if (node.contains("direction") || rebake_only)
			{
				// The stock direction when only the removed geometry is baked out.
				const std::array<float, 3> stock_toward{sun.dir.v[0], sun.dir.v[1], sun.dir.v[2]};
				double pitch = 0, yaw = 0;
				if (rebake_only)
				{
					::iw8::stocksun::Angles(stock_toward, pitch, yaw);
				}
				else
				{
					const auto& d = node.at("direction");
					if (!d.is_array() || d.size() < 2 || d.size() > 3 || !d[0].is_number() || !d[1].is_number())
					{
						throw std::runtime_error("sun.direction is not [pitch, yaw] in degrees");
					}
					pitch = d[0].get<double>();
					yaw = d[1].get<double>();
				}
				if (!std::isfinite(pitch) || !std::isfinite(yaw) || pitch < -90.0 || pitch > -1.0)
				{
					throw std::runtime_error("sun.direction pitch must be -90 to -1 (a negative pitch is above the horizon)");
				}
				auto* world = static_cast<GfxWorld*>(find(ASSET_TYPE_GFXWORLD));
				if (!world)
				{
					throw std::runtime_error(zone_name + " has no GfxWorld");
				}
				auto& draw = world->draw;
				if (!draw.compressedSunShadow || !draw.compressedSunShadowSize)
				{
					throw std::runtime_error(zone_name + " keeps no compressed sun shadow in its GfxWorld: the direction cannot change");
				}
				for (std::uint32_t z = 0; z < draw.transientZoneCount && z < 1536; ++z)
				{
					if (draw.transientZones[z] && draw.transientZones[z]->compressedSunShadow)
					{
						throw std::runtime_error(zone_name + " keeps sun shadow tiles in its streamed transient zones: the direction "
							"cannot change");
					}
				}
				const std::array<float, 3> before{sun.dir.v[0], sun.dir.v[1], sun.dir.v[2]};
				double before_pitch = 0, before_yaw = 0;
				::iw8::stocksun::Angles(before, before_pitch, before_yaw);
				const auto toward = rebake_only ? stock_toward : ::iw8::stocksun::TowardSun(pitch, yaw);
				const auto& stock_params = draw.compressedSunShadowParams;
				report["sun_direction_before"] = {before_pitch, before_yaw};
				report["sun_vector_before"] = {before[0], before[1], before[2]};
				report["sun_shadow_before"] = {{"bytes", draw.compressedSunShadowSize}, {"resolution", stock_params.resolution},
					{"sample_size", stock_params.sampleSize}, {"cells", stock_params.forestSize},
					{"center", {stock_params.centerX, stock_params.centerY}},
					{"direction", {stock_params.sunDirection.v[0], stock_params.sunDirection.v[1], stock_params.sunDirection.v[2]}}};

				// The casters: the world surfaces of the zone and the static models' LOD0 geometry (streamed: ACTS exports it).
				std::string acts = node.value("acts", std::string{});
				if (acts.empty())
				{
					if (const char* value = std::getenv("IW8_ACTS"); value && *value)
					{
						acts = value;
					}
				}
				const auto scratch = output / "_sun_geometry";
				std::error_code removed;
				std::filesystem::remove_all(scratch, removed);
				// The play region: the GfxWorld's heightfields (the outdoor play space of an MP map), else the whole world.
				replaybounds::Bounds play;
				{
					std::array<float, 3> low{INFINITY, INFINITY, INFINITY}, high{-INFINITY, -INFINITY, -INFINITY};
					for (std::int32_t h = 0; world->heightfields && h < world->heightfieldCount; ++h)
					{
						for (int axis = 0; axis < 3; ++axis)
						{
							const auto& b = world->heightfields[h].bounds;
							low[axis] = std::min(low[axis], b.midPoint.v[axis] - b.halfSize.v[axis]);
							high[axis] = std::max(high[axis], b.midPoint.v[axis] + b.halfSize.v[axis]);
						}
					}
					for (int axis = 0; axis < 3; ++axis)
					{
						const bool found = low[axis] <= high[axis];
						play.midpoint[axis] = found ? (low[axis] + high[axis]) * 0.5f : world->bounds.midPoint.v[axis];
						play.halfSize[axis] = found ? (high[axis] - low[axis]) * 0.5f : world->bounds.halfSize.v[axis];
					}
				}
				::iw8::stocksun::SceneReport scene_report;
				replaysunshadow::Scene scene;
				try
				{
					::iw8::stocksun::GeometrySource source{acts, game_path, zone_path, scratch};
					source.playRegion = play;
					scene = ::iw8::stocksun::BuildScene(*world, source, scene_report);
				}
				catch (...)
				{
					std::filesystem::remove_all(scratch, removed);
					throw;
				}
				std::filesystem::remove_all(scratch, removed);
				ZONETOOL_INFO("editzone: sun-shadow casters: %zu world surface(s) (%zu triangles, %zu skipped), %zu static model "
					"instance(s) of %zu model(s) (%zu triangles, %zu at a lower LOD), %zu instance(s) without geometry",
					scene_report.worldSurfaces, scene_report.worldTriangles, scene_report.worldSkipped, scene_report.modelInstances,
					scene_report.models, scene_report.modelTriangles, scene_report.lowerLodInstances, scene_report.missingInstances);

				// Bake as the converter bakes its own maps (replay_sunshadow), at the stock texel size, over the box of the
				// casting world surfaces (a static model far outside, a vista prop, casts only where it overlaps it).
				// The converter's own 32-unit tolerance (replaysunshadow::DefaultTolerance) gave mp_shipment 308 MB (stock:
				// 51 MB). 256 units (89 MB) leaves fewer players-under-cover lit than the stock bake does.
				// A sun direction whose footprint does not fit the budget is cropped to the play region and a margin
				// (a higher sun sees more of a wide map), then baked with a 2048-unit tolerance.
				replaybounds::Bounds receivers;
				for (int axis = 0; axis < 3; ++axis)
				{
					receivers.midpoint[axis] = world->bounds.midPoint.v[axis];
					receivers.halfSize[axis] = world->bounds.halfSize.v[axis];
				}
				const auto grown = [](replaybounds::Bounds box, const float margin) {
					for (auto& half : box.halfSize)
					{
						half += margin;
					}
					return box;
				};
				struct Step
				{
					float sample_size, tolerance;
					replaybounds::Bounds focus;
					const char* what;
				};
				constexpr std::size_t budget = 96u * 1024u * 1024u; // the converter's own maps' budget
				const float stock_sample = stock_params.sampleSize == 1.0f ? 1.0f : 2.0f;
				const std::vector<Step> ladder{{stock_sample, 256.0f, grown(scene_report.worldCasterBounds, 1024.0f), "world"},
					{stock_sample, 256.0f, grown(play, 16384.0f), "play region + 16384"},
					{stock_sample, 256.0f, grown(play, 12288.0f), "play region + 12288"},
					{stock_sample, 256.0f, grown(play, 8192.0f), "play region + 8192"},
					{2.0f, 2048.0f, grown(play, 8192.0f), "play region + 8192"},
					{2.0f, 2048.0f, grown(play, 4096.0f), "play region + 4096"}};
				replaysunshadow::Data baked;
				float tolerance = 0.0f;
				const char* coverage = "";
				for (const auto& step : ladder)
				{
					try
					{
						baked = replaysunshadow::Bake(scene, toward, receivers, step.sample_size, &step.focus, budget, step.tolerance);
						if (baked.bytes.size() <= budget)
						{
							tolerance = step.tolerance;
							coverage = step.what;
							break;
						}
					}
					catch (const std::runtime_error& error)
					{
						if (std::string_view(error.what()).find("exceeds") == std::string_view::npos)
						{
							throw;
						}
					}
					ZONETOOL_INFO("editzone: the sun shadow over the %s at %.0f unit(s) per texel and a %.0f-unit tolerance "
						"exceeds %zu bytes", step.what, step.sample_size, step.tolerance, budget);
					baked = {};
				}
				if (!tolerance)
				{
					throw std::runtime_error("sun.direction: the baked sun shadow exceeds Replay's budget even over the play region "
						"and a 4096-unit margin");
				}
				if (baked.bytes.empty())
				{
					throw std::runtime_error("sun.direction: the sun shadow bake produced nothing");
				}
				scene = {};
				sun_shadow_bytes = std::move(baked.bytes);
				draw.compressedSunShadow = sun_shadow_bytes.data();
				draw.compressedSunShadowSize = static_cast<std::uint32_t>(sun_shadow_bytes.size());
				static_assert(sizeof(draw.compressedSunShadowParams) == sizeof(baked.parameters));
				std::memcpy(&draw.compressedSunShadowParams, &baked.parameters, sizeof(baked.parameters));
				for (int axis = 0; axis < 3; ++axis)
				{
					sun.dir.v[axis] = toward[axis];
				}

				// The vision's fog sun direction (fogSunDir, Replay token RLNQONPKS) turns with the sun: the same pitch and
				// yaw change, so the sun-tinted fog keeps its place beside the sun.
				auto visions = nlohmann::ordered_json::array();
				for (const auto& asset : zone->assets)
				{
					if (rebake_only)
					{
						break;
					}
					if (asset.type != zonetool::iw8::ASSET_TYPE_RAWFILE || asset.referenced || !asset.header)
					{
						continue;
					}
					auto* raw = static_cast<RawFile*>(asset.header);
					const std::string_view raw_name = raw->name ? raw->name : "";
					if (!raw_name.starts_with("vision/") || !raw_name.ends_with(".vision") || !raw->buffer || !raw->len)
					{
						continue;
					}
					std::string text(raw->len, '\0');
					auto size = static_cast<uLongf>(text.size());
					if (uncompress(reinterpret_cast<Bytef*>(text.data()), &size, reinterpret_cast<const Bytef*>(raw->buffer),
						raw->compressedLen) != Z_OK || size != text.size())
					{
						throw std::runtime_error("sun.direction: the vision " + std::string(raw_name) + " does not decompress");
					}
					static const std::regex fog_sun(R"re((^|\n)RLNQONPKS "([^"]*)")re");
					std::smatch match;
					if (!std::regex_search(text, match, fog_sun))
					{
						continue;
					}
					std::array<float, 3> fog{};
					if (std::sscanf(match[2].str().c_str(), "%f %f %f", &fog[0], &fog[1], &fog[2]) != 3)
					{
						continue;
					}
					double fog_pitch = 0, fog_yaw = 0;
					::iw8::stocksun::Angles(fog, fog_pitch, fog_yaw);
					const auto turned = ::iw8::stocksun::TowardSun(std::clamp(fog_pitch + (pitch - before_pitch), -89.9, 89.9),
						fog_yaw + (yaw - before_yaw));
					char value[96];
					std::snprintf(value, sizeof(value), "%.7g %.7g %.7g", turned[0], turned[1], turned[2]);
					text.replace(match.position(2), match.length(2), value);
					const auto compressed = compression::compress_zlib(reinterpret_cast<const std::uint8_t*>(text.data()), text.size());
					auto& kept = vision_buffers.emplace_back(compressed.begin(), compressed.end());
					kept.resize(std::max<std::size_t>(kept.size(), 1));
					raw->buffer = reinterpret_cast<const char*>(kept.data());
					raw->compressedLen = static_cast<std::uint32_t>(compressed.size());
					raw->len = static_cast<std::uint32_t>(text.size());
					visions.push_back({{"name", raw_name}, {"fog_sun_dir_before", {fog[0], fog[1], fog[2]}},
						{"fog_sun_dir", {turned[0], turned[1], turned[2]}}});
				}

				// The worldspawn's sundirection (key 968), as the stock map writes it ("pitch yaw roll").
				bool worldspawn_key = false;
				if (auto* ents = static_cast<MapEnts*>(find(ASSET_TYPE_MAP_ENTS)); !rebake_only && ents && ents->entityString &&
					ents->numEntityChars > 1)
				{
					std::string text(ents->entityString, static_cast<std::size_t>(ents->numEntityChars - 1));
					const auto open = text.find('{'), close = text.find('}');
					static const std::regex key(R"re((^|\n)968 "([^"]*)")re");
					std::smatch match;
					const std::string first = open != std::string::npos && close != std::string::npos && open < close
						? text.substr(open, close - open) : std::string{};
					if (std::regex_search(first, match, key))
					{
						char value[64];
						std::snprintf(value, sizeof(value), "%.6g %.6g 0", pitch, yaw);
						text.replace(open + match.position(2), match.length(2), value);
						sun_entity_text = std::move(text);
						ents->entityString = sun_entity_text.data();
						ents->numEntityChars = static_cast<std::int32_t>(sun_entity_text.size() + 1);
						worldspawn_key = true;
					}
				}

				report["sun_direction"] = {pitch, yaw};
				report["sun_vector"] = {toward[0], toward[1], toward[2]};
				report["sun_shadow"] = {{"bytes", draw.compressedSunShadowSize}, {"resolution", baked.parameters.resolution},
					{"sample_size", baked.parameters.sampleSize}, {"cells", baked.parameters.forestSize},
					{"uniform_cells", baked.uniformCells}, {"tree_cells", baked.treeCells},
					{"center", {baked.parameters.centerX, baked.parameters.centerY}}, {"tolerance", tolerance}, {"coverage", coverage},
					{"world_surfaces", scene_report.worldSurfaces}, {"world_triangles", scene_report.worldTriangles},
					{"world_surfaces_skipped", scene_report.worldSkipped}, {"models", scene_report.models},
					{"model_instances", scene_report.modelInstances}, {"model_triangles", scene_report.modelTriangles},
					{"hidden_instances", scene_report.hiddenInstances}, {"non_casting_instances", scene_report.nonCastingInstances},
					{"splined_instances", scene_report.splinedInstances}, {"model_surfaces_skipped", scene_report.modelSurfacesSkipped},
					{"lower_lod_instances", scene_report.lowerLodInstances},
					{"play_region", {{"mid", {play.midpoint[0], play.midpoint[1], play.midpoint[2]}},
						{"half", {play.halfSize[0], play.halfSize[1], play.halfSize[2]}}}},
					{"missing_instances", scene_report.missingInstances},
					{"missing_models", scene_report.missingModels}};
				report["sun_visions"] = visions;
				report["sun_worldspawn_sundirection"] = worldspawn_key;
				report["sun_shadow_rebaked_for_removal"] = rebake_only;
			}
		}

		// Kept alive until the zone is written.
		std::vector<SpawnGroupCluster> loot_cluster_records;
		std::vector<SpawnGroupZone> loot_zone_records;
		if (edits.contains("loot_boxes"))
		{
			auto* ents = static_cast<MapEnts*>(find(ASSET_TYPE_MAP_ENTS));
			if (!ents)
			{
				throw std::runtime_error(zone_name + " has no map entities");
			}
			auto& loot = ents->spawnGroupLoot;
			const auto& boxes = edits.at("loot_boxes");
			if (!boxes.is_object())
			{
				throw std::runtime_error("loot_boxes must be an object with \"clusters\" and \"zones\" lists");
			}
			const auto box_of = [](const nlohmann::json& entry, const std::string& where)
			{
				Bounds bounds{};
				for (const char* key : {"mid", "half"})
				{
					if (!entry.is_object() || !entry.contains(key) || !entry.at(key).is_array() || entry.at(key).size() != 3)
					{
						throw std::runtime_error(where + ": " + key + " is not three numbers");
					}
					auto& out = std::string(key) == "mid" ? bounds.midPoint : bounds.halfSize;
					for (auto axis = 0; axis < 3; axis++)
					{
						out.v[axis] = entry.at(key)[axis].get<float>();
						if (!std::isfinite(out.v[axis]) || (std::string(key) == "half" && out.v[axis] <= 0.0f))
						{
							throw std::runtime_error(where + ": " + key + " is not finite" + (std::string(key) == "half" ? " and above 0" : ""));
						}
					}
				}
				return bounds;
			};
			const auto list_of = [&](const char* key) -> nlohmann::json
			{
				if (!boxes.contains(key))
				{
					return nlohmann::json::array();
				}
				if (!boxes.at(key).is_array())
				{
					throw std::runtime_error(std::string("loot_boxes.") + key + " is not a list");
				}
				return boxes.at(key);
			};
			// Zones first: a new cluster takes the zone that holds its middle.
			const auto zones = list_of("zones");
			loot_zone_records.assign(loot.zones, loot.zones + loot.zoneCount);
			std::size_t index = 0;
			for (const auto& entry : zones)
			{
				const auto where = "loot_boxes.zones[" + std::to_string(index++) + "]";
				const auto name = entry.is_object() ? entry.value("name", std::string()) : std::string();
				if (name.empty())
				{
					throw std::runtime_error(where + ": no name");
				}
				for (const char c : name)
				{
					if (!(std::islower(static_cast<unsigned char>(c)) || std::isdigit(static_cast<unsigned char>(c)) || c == '_'))
					{
						throw std::runtime_error(where + ": '" + name + "' is not a lower-case name");
					}
				}
				SpawnGroupZone added{};
				added.name = static_cast<scr_string_t>(SL_GetString(name.data()));
				added.bounds = box_of(entry, where);
				loot_zone_records.push_back(added);
			}
			const auto clusters = list_of("clusters");
			loot_cluster_records.assign(loot.clusters, loot.clusters + loot.clusterCount);
			index = 0;
			for (const auto& entry : clusters)
			{
				SpawnGroupCluster cluster{};
				cluster.bounds = box_of(entry, "loot_boxes.clusters[" + std::to_string(index++) + "]");
				for (std::size_t i = 0; i < loot_zone_records.size(); i++)
				{
					const auto& zone_box = loot_zone_records[i].bounds;
					bool holds = true;
					for (auto axis = 0; axis < 3; axis++)
					{
						holds = holds && std::abs(cluster.bounds.midPoint.v[axis] - zone_box.midPoint.v[axis]) <= zone_box.halfSize.v[axis];
					}
					if (holds)
					{
						cluster.zone = static_cast<std::uint16_t>(i + 1);
						break;
					}
				}
				loot_cluster_records.push_back(cluster);
			}
			// Cluster indices from 0xFFFB up are the special point values; zone numbers are u16 and 1-based.
			if (loot_cluster_records.size() >= 0xFFFB || loot_zone_records.size() >= 0xFFFF)
			{
				throw std::runtime_error("loot_boxes: too many clusters or zones");
			}
			if (!clusters.empty())
			{
				loot.clusterCount = static_cast<std::uint16_t>(loot_cluster_records.size());
				loot.clusters = loot_cluster_records.data();
			}
			if (!zones.empty())
			{
				loot.zoneCount = static_cast<std::uint16_t>(loot_zone_records.size());
				loot.zones = loot_zone_records.data();
			}
			report["loot_clusters_added"] = clusters.size();
			report["loot_zones_added"] = zones.size();
		}

		std::vector<SpawnGroupPoint> loot_points;
		std::vector<std::vector<std::uint16_t>> loot_lists;
		if (edits.contains("loot_points"))
		{
			auto* ents = static_cast<MapEnts*>(find(ASSET_TYPE_MAP_ENTS));
			if (!ents)
			{
				throw std::runtime_error(zone_name + " has no map entities");
			}
			auto& loot = ents->spawnGroupLoot;
			const auto& list = edits.at("loot_points");
			if (!list.is_array() || list.size() > 65000)
			{
				throw std::runtime_error("loot_points must be a list of at most 65000 points");
			}
			const auto inside = [](const Bounds& box, const vec3_t& point)
			{
				for (auto axis = 0; axis < 3; axis++)
				{
					if (std::abs(point.v[axis] - box.midPoint.v[axis]) > box.halfSize.v[axis])
					{
						return false;
					}
				}
				return true;
			};
			constexpr std::uint16_t free_point = 0xFFFD, cache_point = 0xFFFB;
			loot_points.reserve(list.size());
			std::size_t index = 0;
			for (const auto& entry : list)
			{
				const auto fail = [&](const std::string& what)
				{
					throw std::runtime_error("loot_points[" + std::to_string(index) + "]: " + what);
				};
				SpawnGroupPoint point{};
				const auto vector = [&](const char* key, vec3_t& out, const bool required)
				{
					if (!entry.contains(key))
					{
						if (required)
						{
							fail(std::string("no ") + key);
						}
						return;
					}
					const auto& node = entry.at(key);
					if (!node.is_array() || node.size() != 3)
					{
						fail(std::string(key) + " is not three numbers");
					}
					for (auto axis = 0; axis < 3; axis++)
					{
						out.v[axis] = node[axis].get<float>();
						if (!std::isfinite(out.v[axis]))
						{
							fail(std::string(key) + " is not finite");
						}
					}
				};
				vector("origin", point.origin, true);
				vector("angles", point.angles, false);
				const auto cluster = entry.value("cluster", nlohmann::json("auto"));
				if (cluster.is_number_unsigned())
				{
					const auto value = cluster.get<std::uint64_t>();
					if (value >= loot.clusterCount && value != free_point && value != cache_point && value != 0xFFFF)
					{
						fail("cluster " + std::to_string(value) + " is not one of the map's " + std::to_string(loot.clusterCount) + " clusters");
					}
					point.cluster = static_cast<std::uint16_t>(value);
				}
				else if (cluster == "free")
				{
					point.cluster = free_point;
				}
				else if (cluster == "cache")
				{
					point.cluster = cache_point;
				}
				else if (cluster == "auto")
				{
					point.cluster = free_point;
					for (std::uint32_t i = 0; i < loot.clusterCount; i++)
					{
						if (inside(loot.clusters[i].bounds, point.origin))
						{
							point.cluster = static_cast<std::uint16_t>(i);
							break;
						}
					}
				}
				else
				{
					fail("cluster is not a number, \"free\", \"cache\" or \"auto\"");
				}
				const auto zone_value = entry.value("zone", nlohmann::json("auto"));
				if (zone_value.is_number_unsigned())
				{
					const auto value = zone_value.get<std::uint64_t>();
					if (value > loot.zoneCount)
					{
						fail("zone " + std::to_string(value) + " is beyond the map's " + std::to_string(loot.zoneCount) + " zones");
					}
					point.zone = static_cast<std::uint16_t>(value);
				}
				else if (zone_value == "auto")
				{
					point.zone = 0;
					for (std::uint32_t i = 0; i < loot.zoneCount; i++)
					{
						if (inside(loot.zones[i].bounds, point.origin))
						{
							point.zone = static_cast<std::uint16_t>(i + 1);
							break;
						}
					}
				}
				else
				{
					fail("zone is not a number or \"auto\"");
				}
				loot_points.push_back(point);
				++index;
			}

			// The point lists follow from the points (ascending, as in the stock block); the cluster list of a zone is
			// the clusters whose points are in it. Boxes, names and the zone of each cluster stay as authored.
			auto& cluster_lists = loot_lists;
			cluster_lists.assign(static_cast<std::size_t>(loot.clusterCount) + loot.zoneCount * 2, {});
			const auto zone_points = [&](const std::size_t zone) -> std::vector<std::uint16_t>& { return cluster_lists[loot.clusterCount + zone * 2]; };
			const auto zone_clusters = [&](const std::size_t zone) -> std::vector<std::uint16_t>& { return cluster_lists[loot.clusterCount + zone * 2 + 1]; };
			for (std::size_t i = 0; i < loot_points.size(); i++)
			{
				const auto& point = loot_points[i];
				if (point.cluster < loot.clusterCount)
				{
					cluster_lists[point.cluster].push_back(static_cast<std::uint16_t>(i));
				}
				// A loot cache point names its zone but is not in the zone's list (the rule holds on every zone of
				// mp_br_tut, mp_br_quarry and mp_donetsk).
				if (point.zone && point.cluster != cache_point)
				{
					zone_points(point.zone - 1u).push_back(static_cast<std::uint16_t>(i));
				}
			}
			for (std::uint32_t i = 0; i < loot.clusterCount; i++)
			{
				auto& cluster = loot.clusters[i];
				cluster.pointCount = static_cast<std::uint16_t>(cluster_lists[i].size());
				cluster.points = cluster_lists[i].empty() ? nullptr : cluster_lists[i].data();
				if (!cluster_lists[i].empty())
				{
					const auto point_zone = loot_points[cluster_lists[i].front()].zone;
					if (point_zone)
					{
						auto& of_zone = zone_clusters(point_zone - 1u);
						if (of_zone.empty() || of_zone.back() != i)
						{
							of_zone.push_back(static_cast<std::uint16_t>(i));
						}
					}
				}
			}
			for (std::uint32_t i = 0; i < loot.zoneCount; i++)
			{
				auto& loot_zone = loot.zones[i];
				loot_zone.pointCount = static_cast<std::uint16_t>(zone_points(i).size());
				loot_zone.points = zone_points(i).empty() ? nullptr : zone_points(i).data();
				loot_zone.clusterCount = static_cast<std::uint16_t>(zone_clusters(i).size());
				loot_zone.clusters = zone_clusters(i).empty() ? nullptr : zone_clusters(i).data();
			}
			const auto before = static_cast<std::int64_t>(loot.pointCount);
			const auto after = static_cast<std::int64_t>(loot_points.size());
			loot.pointCount = static_cast<std::uint16_t>(loot_points.size());
			loot.points = loot_points.empty() ? nullptr : loot_points.data();
			// Each loot point takes one scriptable slot: the loot count and the three server counts that include it
			// (research 86: checked on 21 maps) move by the difference.
			auto& scriptables = ents->scriptableMapEnts;
			const auto moved = [&](std::uint32_t& count)
			{
				count = static_cast<std::uint32_t>(std::max<std::int64_t>(0, static_cast<std::int64_t>(count) + after - before));
			};
			if (scriptables.lootInstanceCount == static_cast<std::uint32_t>(before))
			{
				scriptables.lootInstanceCount = static_cast<std::uint32_t>(after);
				moved(scriptables.serverInstanceCount);
				moved(scriptables.serverFixedPartCount);
				moved(scriptables.serverUsablePartCount);
			}
			else
			{
				warn("editzone: %s has %u loot instances for %lld loot points; the scriptable counts are left as they are",
					zone_name.c_str(), scriptables.lootInstanceCount, static_cast<long long>(before));
			}
			report["loot_points_before"] = before;
			report["loot_points"] = after;
		}

		// Kept alive until the zone is written.
		using PlacedScriptable = std::remove_pointer_t<decltype(std::declval<MapEnts&>().scriptableMapEnts.mapInstances)>;
		std::vector<PlacedScriptable> scriptable_records;
		// The rebuilt dynamic entity lists, bucket arrays and GfxWorld dynamic-entity arrays of a copy or removal.
		std::deque<std::vector<DynEntityDef>> dynent_storage;
		std::deque<std::vector<SpatialPartition_Population_Node>> dynent_node_storage;
		std::deque<std::vector<std::uint32_t>> dynent_u32_storage;
		std::deque<std::vector<std::uint8_t>> dynent_byte_storage;
		std::vector<GfxSceneDynModel> dynent_scene_models;
		if (edits.contains("scriptable_instances"))
		{
			auto* ents = static_cast<MapEnts*>(find(ASSET_TYPE_MAP_ENTS));
			if (!ents)
			{
				throw std::runtime_error(zone_name + " has no map entities");
			}
			auto& block = ents->scriptableMapEnts;
			const auto& node = edits.at("scriptable_instances");
			if (!node.is_object())
			{
				throw std::runtime_error("scriptable_instances must be an object with \"move\", \"add\" and \"remove\"");
			}
			const auto before = block.mapInstanceCount;
			if (before && !block.mapInstances)
			{
				throw std::runtime_error(zone_name + ": the scriptable instances are not in the zone");
			}
			// The instances that a dynamic entity of this zone names: those keep their own placement and are refused.
			std::set<std::uint32_t> with_dynamic_entity;
			for (const auto& asset : zone->assets)
			{
				if (asset.type != ASSET_TYPE_DYNENTITYLIST || asset.referenced || !asset.header)
				{
					continue;
				}
				const auto* list = static_cast<const DynEntityList*>(asset.header);
				for (auto side = 0; side < 2; side++)
				{
					for (std::uint32_t i = 0; list->dynEntDefList[side] && i < list->dynEntCount[side]; i++)
					{
						const auto& entry = list->dynEntDefList[side][i];
						if (entry.type == DYNENT_TYPE_SCRIPTABLEINST || entry.type == DYNENT_TYPE_SCRIPTABLEPHYSICS)
						{
							with_dynamic_entity.insert(entry.scriptableMapIndex);
						}
					}
				}
			}
			const auto placement = [&](const nlohmann::json& entry, const std::string& where, PlacedScriptable& record)
			{
				const auto vector = [&](const char* key, vec3_t& out, const bool required)
				{
					if (!entry.contains(key))
					{
						if (required)
						{
							throw std::runtime_error(where + ": no " + key);
						}
						return;
					}
					const auto& value = entry.at(key);
					if (!value.is_array() || value.size() != 3)
					{
						throw std::runtime_error(where + ": " + key + " is not three numbers");
					}
					for (auto axis = 0; axis < 3; axis++)
					{
						if (!value[axis].is_number())
						{
							throw std::runtime_error(where + ": " + key + " is not three numbers");
						}
						out.v[axis] = value[axis].get<float>();
						if (!std::isfinite(out.v[axis]))
						{
							throw std::runtime_error(where + ": " + key + " is not finite");
						}
					}
				};
				vector("origin", record.initialOrigin, true);
				vector("angles", record.initialAngles, false);
			};
			const auto index_of = [&](const nlohmann::json& entry, const char* key, const std::string& where) -> std::uint32_t
			{
				if (!entry.is_object() || !entry.contains(key) || !entry.at(key).is_number_unsigned())
				{
					throw std::runtime_error(where + ": no \"" + key + "\" (the index of a placed instance)");
				}
				const auto value = entry.at(key).get<std::uint64_t>();
				if (value >= before)
				{
					throw std::runtime_error(where + ": instance " + std::to_string(value) + " is beyond the map's " + std::to_string(before));
				}
				// An instance with dynamic entities of its own is copied or removed with them (the pass after "remove").
				return static_cast<std::uint32_t>(value);
			};
			constexpr std::uint32_t no_node = 0xFFFFFFFF;
			// The partition of the MapEnts spatial tree of population "pop" (0: the radial entries, 1: the sparse occluders,
			// doors) that holds "origin": node i is a leaf for axis 3, else origin[axis] < dist -> i + 1, else rightChild
			// (IW8 Radiant research 89 section 8: exact for the ordinary entries on Quarry, and for all 319 doors on tree 1).
			// With "grow" the extents of every node on the path take the origin in (a node's extent holds its entries: z is
			// the range of their origins).
			const auto walk_tree = [&](const int pop, const vec3_t& origin, const bool grow, const std::string& where) -> std::uint32_t
			{
				auto* population = ents->dynEntSpatialPopulation[pop];
				if (!population || !population->tree || !population->tree->partitions || !population->tree->partitionCount)
				{
					throw std::runtime_error(where + ": the map has no spatial tree for population " + std::to_string(pop));
				}
				auto* tree = population->tree;
				std::uint32_t node = 0;
				for (std::uint32_t guard = 0; ; guard++)
				{
					if (node >= tree->partitionCount || guard > tree->partitionCount)
					{
						throw std::runtime_error(where + ": the spatial tree is not walkable");
					}
					if (grow && tree->extents)
					{
						auto& box = tree->extents[node];
						for (auto k = 0; k < 3; k++)
						{
							box.mins.v[k] = (std::min)(box.mins.v[k], origin.v[k]);
							box.maxs.v[k] = (std::max)(box.maxs.v[k], origin.v[k]);
						}
					}
					const auto& part = tree->partitions[node];
					if (part.axis == 3)
					{
						return node;
					}
					node = origin.v[part.axis] < part.dist ? node + 1 : part.rightChild;
				}
			};
			scriptable_records.assign(block.mapInstances, block.mapInstances + before);
			std::size_t moved = 0, added = 0, moved_with_dynamic = 0, relinked = 0;
			// A move of an instance whose dynamic entities are ordinary ones (not sparse occluders such as doors, not
			// transient): the entities copy the placement (initialPose origin = the instance origin, quat = AnglesToQuat
			// of its angles) and sit in the spatial partition the MapEnts tree gives for the origin, chained per partition
			// in DynEntityList.dynEntSpatialPopulation in descending entry order (IW8 Radiant research 89 section 8: exact
			// on Quarry, tools/dynent_pose_rules.py). Entries of the no-spatial list (partition 0) keep it.
			const auto quat_of = [](const vec3_t& a, float* q)
			{
				const auto rad = 0.00872664625997164788; // pi / 360: half of the angle in radians per degree
				const double cp = std::cos(a.v[0] * rad), sp = std::sin(a.v[0] * rad), cy = std::cos(a.v[1] * rad),
					sy = std::sin(a.v[1] * rad), cr = std::cos(a.v[2] * rad), sr = std::sin(a.v[2] * rad);
				q[0] = static_cast<float>(sr * cp * cy - cr * sp * sy);
				q[1] = static_cast<float>(cr * sp * cy + sr * cp * sy);
				q[2] = static_cast<float>(cr * cp * sy - sr * sp * cy);
				q[3] = static_cast<float>(cr * cp * cy + sr * sp * sy);
			};
			// True when a dynamic entity's pose is the placement of "record" (the stock rule: origin, AnglesToQuat).
			const auto pose_is = [&](const DynEntityDef& def, const ScriptableInstance& record)
			{
				float q[4];
				quat_of(record.initialAngles, q);
				bool same = true, q_same = true, flipped = true;
				for (auto k = 0; k < 3; k++)
				{
					same = same && std::abs(def.initialPose.origin.v[k] - record.initialOrigin.v[k]) < 0.01f;
				}
				for (auto k = 0; k < 4; k++)
				{
					q_same = q_same && std::abs(def.initialPose.quat.v[k] - q[k]) < 0.002f;
					flipped = flipped && std::abs(def.initialPose.quat.v[k] + q[k]) < 0.002f;
				}
				return same && (q_same || flipped);
			};
			const auto move_dynamic = [&](const std::uint32_t index, const ScriptableInstance& record, const std::string& where)
			{
				auto* map_ents = ents;
				for (auto& asset : zone->assets)
				{
					if (asset.type != ASSET_TYPE_DYNENTITYLIST || asset.referenced || !asset.header)
					{
						continue;
					}
					auto* list = static_cast<DynEntityList*>(asset.header);
					for (auto side = 0; side < 2; side++)
					{
						auto* defs = list->dynEntDefList[side];
						for (std::uint32_t i = 0; defs && i < list->dynEntCount[side]; i++)
						{
							auto& def = defs[i];
							if ((def.type != DYNENT_TYPE_SCRIPTABLEINST && def.type != DYNENT_TYPE_SCRIPTABLEPHYSICS) || def.scriptableMapIndex != index)
							{
								continue;
							}
							if (def.isTransient)
							{
								throw std::runtime_error(where + ": instance " + std::to_string(index) +
									" has a streamed dynamic entity: it cannot be moved yet");
							}
							// Doors and other sparse occluders: their spatialPartitionIndex names an area (about 20 doors each,
							// research 89 section 8) whose structure is not known. A door moves only inside the box of the stock
							// doors of its area, and keeps the area.
							if (def.priority == DYNENT_PRIORITY_SPARSE_OCCLUDER)
							{
								float lo[3] = {1e30f, 1e30f, 1e30f}, hi[3] = {-1e30f, -1e30f, -1e30f};
								for (std::uint32_t j = 0; j < list->dynEntCount[side]; j++)
								{
									const auto& other = defs[j];
									if (other.priority != DYNENT_PRIORITY_SPARSE_OCCLUDER || other.spatialPartitionIndex != def.spatialPartitionIndex)
									{
										continue;
									}
									const auto& at = (other.scriptableMapIndex < before) ? block.mapInstances[other.scriptableMapIndex].initialOrigin
										: other.initialPose.origin;
									for (auto k = 0; k < 3; k++)
									{
										lo[k] = (std::min)(lo[k], at.v[k]);
										hi[k] = (std::max)(hi[k], at.v[k]);
									}
								}
								for (auto k = 0; k < 3; k++)
								{
									// A little room around the area's doors (a door is about 64 units wide).
									if (record.initialOrigin.v[k] < lo[k] - 64.0f || record.initialOrigin.v[k] > hi[k] + 64.0f)
									{
										throw std::runtime_error(where + ": door " + std::to_string(index) +
											" would leave its area of the map (the box of the stock doors that share it); move it less far");
									}
								}
							}
							// Only where the stock rule holds for this entry: its pose is the instance's stock placement.
							{
								const auto& stock = block.mapInstances[index];
								float q[4];
								quat_of(stock.initialAngles, q);
								bool same = true, flipped = true;
								for (auto k = 0; k < 3; k++)
								{
									same = same && std::abs(def.initialPose.origin.v[k] - stock.initialOrigin.v[k]) < 0.01f;
								}
								bool q_same = true;
								for (auto k = 0; k < 4; k++)
								{
									q_same = q_same && std::abs(def.initialPose.quat.v[k] - q[k]) < 0.002f;
									flipped = flipped && std::abs(def.initialPose.quat.v[k] + q[k]) < 0.002f;
								}
								if (!same || !(q_same || flipped))
								{
									throw std::runtime_error(where + ": instance " + std::to_string(index) +
										" has a dynamic entity placed apart from it; it cannot be moved yet");
								}
							}
							def.initialPose.origin = record.initialOrigin;
							quat_of(record.initialAngles, def.initialPose.quat.v);
							if (def.spatialPartitionIndex == 0 || def.priority == DYNENT_PRIORITY_SPARSE_OCCLUDER)
							{
								continue; // the no-spatial list, or a door that keeps its area
							}
							auto* population = map_ents->dynEntSpatialPopulation[side];
							auto* chains = list->dynEntSpatialPopulation[side];
							if (!population || !population->tree || !population->tree->partitions || !chains || !chains->buckets)
							{
								throw std::runtime_error(where + ": the map has no spatial tree for the dynamic entity of instance " + std::to_string(index));
							}
							// The walked partition; its extents (and those above it) take the new origin in.
							const auto node = walk_tree(0, record.initialOrigin, true, where);
							if (node == def.spatialPartitionIndex)
							{
								continue;
							}
							const auto bucket_of = [&](const std::uint32_t partition) -> std::int64_t
							{
								for (std::uint32_t b = 0; b < chains->partitionCount; b++)
								{
									if (chains->partitionIds[b] == partition)
									{
										return b;
									}
								}
								return -1;
							};
							const auto from = bucket_of(def.spatialPartitionIndex), to = bucket_of(node);
							if (from < 0 || to < 0)
							{
								throw std::runtime_error(where + ": instance " + std::to_string(index) +
									" would move into an area of the map that holds no other object of this kind (spatial partition " +
									std::to_string(node) + "); move it less far");
							}
							constexpr std::uint32_t none = 0xFFFFFFFF;
							// Unlink from the old chain.
							{
								auto& head = chains->buckets[from].spatialNodeNext;
								std::uint32_t prev = none;
								for (std::uint32_t at = head, guard = 0; at != none && guard < list->dynEntCount[side]; at = defs[at].spatialNode.spatialNodeNext, guard++)
								{
									if (at == i)
									{
										if (prev == none) head = def.spatialNode.spatialNodeNext;
										else defs[prev].spatialNode.spatialNodeNext = def.spatialNode.spatialNodeNext;
										if (chains->bucketTails && chains->bucketTails[from].spatialNodeNext == i)
										{
											chains->bucketTails[from].spatialNodeNext = prev;
										}
										break;
									}
									prev = at;
								}
							}
							// Link into the new chain, keeping the descending order.
							{
								auto& head = chains->buckets[to].spatialNodeNext;
								std::uint32_t prev = none, at = head;
								for (std::uint32_t guard = 0; at != none && at > i && guard < list->dynEntCount[side]; guard++)
								{
									prev = at;
									at = defs[at].spatialNode.spatialNodeNext;
								}
								def.spatialNode.spatialNodeNext = at;
								if (prev == none) head = i;
								else defs[prev].spatialNode.spatialNodeNext = i;
								if (chains->bucketTails && at == none)
								{
									chains->bucketTails[to].spatialNodeNext = i;
								}
							}
							def.spatialPartitionIndex = node;
							++relinked;
						}
					}
				}
			};
			if (node.contains("move"))
			{
				std::size_t at = 0;
				for (const auto& entry : node.at("move"))
				{
					const auto where = "scriptable_instances.move[" + std::to_string(at++) + "]";
					if (!entry.is_object() || !entry.contains("index") || !entry.at("index").is_number_unsigned() ||
						entry.at("index").get<std::uint64_t>() >= before)
					{
						throw std::runtime_error(where + ": no \"index\" of a placed instance (0 .. " + std::to_string(before) + ")");
					}
					const auto index = static_cast<std::uint32_t>(entry.at("index").get<std::uint64_t>());
					auto& record = scriptable_records[index];
					placement(entry, where, record);
					if (with_dynamic_entity.contains(index))
					{
						move_dynamic(index, record, where);
						++moved_with_dynamic;
					}
					++moved;
				}
			}
			std::int64_t server = 0, fixed_parts = 0, usable_parts = 0, limits = 0, detail_limits = 0;
			std::vector<std::uint32_t> added_like; // per added instance: the instance it copies
			if (node.contains("add"))
			{
				std::size_t at = 0;
				for (const auto& entry : node.at("add"))
				{
					const auto where = "scriptable_instances.add[" + std::to_string(at++) + "]";
					// A copy of the stock record as it was read (a moved original does not move its copies).
					const auto like = index_of(entry, "like", where);
					added_like.push_back(like);
					auto record = block.mapInstances[like];
					record.initialAngles = {};
					placement(entry, where, record);
					// The group names stay (scripts find loot rocks, extraction sites and the like by targetname and
					// script_noteworthy, which stock maps share between instances); the one-to-one links are cleared.
					record.target = record.script_linkname = record.script_linkto = scr_string_t{};
					record.mapEntLookup = 0;
					const auto number = [&](const char* key) -> std::int64_t
					{
						if (!entry.contains(key) || !entry.at(key).is_number_unsigned() || entry.at(key).get<std::uint64_t>() > 255)
						{
							throw std::runtime_error(where + ": no " + key + " (a number of the definition)");
						}
						return entry.at(key).get<std::int64_t>();
					};
					const auto flag = [&](const char* key, const bool fallback)
					{
						if (!entry.contains(key))
						{
							return fallback;
						}
						if (!entry.at(key).is_boolean())
						{
							throw std::runtime_error(where + ": " + key + " is not true or false");
						}
						return entry.at(key).get<bool>();
					};
					const auto parts = number("server_instanced_parts");
					server += parts ? 1 : 0;
					fixed_parts += parts;
					usable_parts += number("usable_parts");
					const auto main_limit = flag("physics_limit", false);
					limits += main_limit ? 1 : 0;
					detail_limits += flag("physics_limit_detail", main_limit) ? 1 : 0;
					scriptable_records.push_back(record);
					++added;
				}
			}
			// "remove": [{"index": n, "server_instanced_parts": n, "usable_parts": n, "physics_limit": bool,
			// "physics_limit_detail": bool}] drops stock instances (the numbers are the definition's, as for "add"), with
			// their dynamic entities (the pass below). The dynamic entities that name later instances are renumbered; the
			// MapEnts spatial trees hold split planes and extents only (no instance indices), so they stay.
			const auto current = scriptable_records; // moved and added, before a removal
			std::set<std::uint32_t> removed;
			if (node.contains("remove"))
			{
				std::size_t at = 0;
				for (const auto& entry : node.at("remove"))
				{
					const auto where = "scriptable_instances.remove[" + std::to_string(at++) + "]";
					const auto index = index_of(entry, "index", where);
					if (!removed.insert(index).second)
					{
						throw std::runtime_error(where + ": instance " + std::to_string(index) + " is removed twice");
					}
					const auto number = [&](const char* key) -> std::int64_t
					{
						if (!entry.contains(key) || !entry.at(key).is_number_unsigned() || entry.at(key).get<std::uint64_t>() > 255)
						{
							throw std::runtime_error(where + ": no " + key + " (a number of the definition)");
						}
						return entry.at(key).get<std::int64_t>();
					};
					const auto flag = [&](const char* key, const bool fallback)
					{
						if (!entry.contains(key))
						{
							return fallback;
						}
						if (!entry.at(key).is_boolean())
						{
							throw std::runtime_error(where + ": " + key + " is not true or false");
						}
						return entry.at(key).get<bool>();
					};
					const auto parts = number("server_instanced_parts");
					server -= parts ? 1 : 0;
					fixed_parts -= parts;
					usable_parts -= number("usable_parts");
					const auto main_limit = flag("physics_limit", false);
					limits -= main_limit ? 1 : 0;
					detail_limits -= flag("physics_limit_detail", main_limit) ? 1 : 0;
				}
				if (!removed.empty())
				{
					std::vector<PlacedScriptable> kept;
					kept.reserve(scriptable_records.size() - removed.size());
					for (std::size_t i = 0; i < scriptable_records.size(); i++)
					{
						if (!removed.contains(static_cast<std::uint32_t>(i)))
						{
							kept.push_back(scriptable_records[i]);
						}
					}
					scriptable_records = std::move(kept);
				}
			}
			// The dynamic entities of the removed and the copied instances (IW8 Radiant research 89 section 8). Every
			// stock entry of a list is on exactly one chain: the list's no-spatial chain (partition 0), a bucket of
			// population 0 (radial entries: signs, fences, props) or of population 1 (sparse occluders: doors), each chain
			// in descending entry order with its tail the last; partitionIds[b] names the bucket's partition of the MapEnts
			// tree of that population. The entries of a removed instance leave the list; a copy gets copies of the entries
			// of its "like" instance (same definition numbers, the copy's pose, the walked partition), inserted after the
			// last entry that names a real instance (the stock order: instance entries by instance, then the 64 reserved
			// physics entries that name 500000, then clutter). Everything that names an entry by index is renumbered: the
			// chains, the bucket heads and tails, the no-spatial head and tail. Counts follow: the list's, the MapEnts
			// totals, the no-spatial count, the entries with extra pose parts, the populations, and the GfxWorld dynamic
			// entity client arrays (stock: clients = MapEnts total + 10, words, cell bits, visibility, motion bits and
			// light shadow bits per client; runtime arrays, zero in the zone).
			{
				const auto kept_count = static_cast<std::uint32_t>(before - removed.size());
				const auto final_index = [&](const std::uint32_t old) -> std::uint32_t
				{
					return old - static_cast<std::uint32_t>(std::distance(removed.begin(), removed.lower_bound(old)));
				};
				const auto scriptable_type = [](const DynEntityDef& def)
				{
					return def.type == DYNENT_TYPE_SCRIPTABLEINST || def.type == DYNENT_TYPE_SCRIPTABLEPHYSICS;
				};
				// A dynamic entity list kept in a streamed zone (Verdansk) is not rebuilt: when one of its entries names an
				// instance at or after a removed one, its index would go stale, and nothing is removed.
				if (!removed.empty())
				{
					std::set<std::uint32_t> held;
					for (const auto& asset : zone->assets)
					{
						if (asset.type == ASSET_TYPE_DYNENTITYLIST && !asset.referenced && asset.header)
						{
							held.insert(static_cast<std::uint32_t>(static_cast<const DynEntityList*>(asset.header)->index));
						}
					}
					std::size_t checked = 0;
					const auto lists = static_cast<std::uint32_t>(ents->dynEntityListsCount);
					for (std::uint32_t l = 0; ents->dynEntListIds && l < lists; l++)
					{
						const auto id = static_cast<std::uint32_t>(ents->dynEntListIds[l]);
						if (held.contains(id))
						{
							continue;
						}
						const auto path = zone_folder / std::format("{}_{:05}_tr.ff", zone_name, id);
						if (!std::filesystem::exists(path))
						{
							throw std::runtime_error(std::format("scriptable_instances.remove: dynamic entity list {} of {} is in a streamed zone that "
								"was not found ({}); its entries may name instances by index, so nothing is removed", id, zone_name, path.filename().string()));
						}
						const auto other = load_zone(path, {});
						for (const auto& asset : other->assets)
						{
							if (asset.type != ASSET_TYPE_DYNENTITYLIST || asset.referenced || !asset.header)
							{
								continue;
							}
							const auto* list = static_cast<const DynEntityList*>(asset.header);
							for (auto side = 0; side < 2; side++)
							{
								for (std::uint32_t i = 0; list->dynEntDefList[side] && i < list->dynEntCount[side]; i++)
								{
									const auto& def = list->dynEntDefList[side][i];
									if (scriptable_type(def) && def.scriptableMapIndex < before && def.scriptableMapIndex >= *removed.begin())
									{
										throw std::runtime_error(std::format("scriptable_instances.remove: the streamed zone {} has a dynamic entity that names "
											"instance {}; that zone is not rebuilt, so no instance at or before it can be removed",
											path.filename().string(), def.scriptableMapIndex));
									}
								}
							}
						}
						++checked;
					}
					report["scriptable_streamed_lists_checked"] = checked;
				}
				std::int64_t total_delta = 0, no_spatial_delta = 0, extra_pose_delta = 0;
				auto max_pose_parts = ents->dynEntMaxPosePartCount;
				std::size_t dyn_removed = 0, dyn_added = 0, dyn_renumbered = 0, buckets_added = 0, buckets_dropped = 0, off_chain = 0;
				for (auto& asset : zone->assets)
				{
					if (asset.type != ASSET_TYPE_DYNENTITYLIST || asset.referenced || !asset.header)
					{
						continue;
					}
					auto* list = static_cast<DynEntityList*>(asset.header);
					const auto list_name = "dynamic entity list " + std::to_string(static_cast<std::uint32_t>(list->index));
					// Brush entries (none on a stock map) are only renumbered: a removed or copied instance must not have one.
					for (std::uint32_t i = 0; list->dynEntDefList[1] && i < list->dynEntCount[1]; i++)
					{
						auto& def = list->dynEntDefList[1][i];
						if (!scriptable_type(def) || def.scriptableMapIndex >= before)
						{
							continue;
						}
						if (removed.contains(def.scriptableMapIndex) || std::ranges::find(added_like, def.scriptableMapIndex) != added_like.end())
						{
							throw std::runtime_error("scriptable_instances: instance " + std::to_string(def.scriptableMapIndex) +
								" has a brush dynamic entity; it cannot be copied or removed");
						}
						const auto now = final_index(def.scriptableMapIndex);
						if (now != def.scriptableMapIndex)
						{
							def.scriptableMapIndex = now;
							++dyn_renumbered;
						}
					}
					auto* defs = list->dynEntDefList[0];
					const auto n = defs ? list->dynEntCount[0] : 0u;
					std::vector<bool> drop(n, false);
					bool any_drop = false;
					std::uint32_t insert_at = 0;
					for (std::uint32_t i = 0; i < n; i++)
					{
						const auto& def = defs[i];
						if (!scriptable_type(def) || def.scriptableMapIndex >= before)
						{
							continue; // the reserved physics entries (500000) and clutter stay
						}
						insert_at = i + 1;
						if (removed.contains(def.scriptableMapIndex))
						{
							if (def.isTransient)
							{
								throw std::runtime_error("scriptable_instances.remove: instance " + std::to_string(def.scriptableMapIndex) +
									" has a streamed dynamic entity; it cannot be removed");
							}
							drop[i] = true;
							any_drop = true;
						}
					}
					// The new entries: per copy, the entries of its "like" instance in their order.
					struct NewEntry
					{
						DynEntityDef def;
						int pop; // -1: the no-spatial chain, else the population
					};
					std::vector<NewEntry> fresh;
					for (std::size_t k = 0; k < added_like.size(); k++)
					{
						const auto like = added_like[k];
						const auto where = "scriptable_instances.add[" + std::to_string(k) + "]";
						const auto& now = current[like];        // the instance as it is (after a move)
						const auto& placed = current[before + k]; // the copy
						for (std::uint32_t i = 0; i < n; i++)
						{
							const auto& def = defs[i];
							if (!scriptable_type(def) || def.scriptableMapIndex != like)
							{
								continue;
							}
							if (def.isTransient)
							{
								throw std::runtime_error(where + ": instance " + std::to_string(like) + " has a streamed dynamic entity; it cannot be copied");
							}
							if (!pose_is(def, now) && !pose_is(def, block.mapInstances[like]))
							{
								throw std::runtime_error(where + ": instance " + std::to_string(like) +
									" has a dynamic entity placed apart from it; it cannot be copied");
							}
							auto copy = def;
							copy.initialPose.origin = placed.initialOrigin;
							quat_of(placed.initialAngles, copy.initialPose.quat.v);
							copy.scriptableMapIndex = kept_count + static_cast<std::uint32_t>(k);
							copy.spatialNode.spatialNodeNext = no_node;
							auto pop = -1;
							if (def.priority == DYNENT_PRIORITY_SPARSE_OCCLUDER)
							{
								pop = 1;
							}
							else if (def.spatialPartitionIndex != 0)
							{
								pop = 0;
							}
							if (pop >= 0)
							{
								if (walk_tree(pop, def.initialPose.origin, false, where) != def.spatialPartitionIndex)
								{
									throw std::runtime_error(where + ": the dynamic entity of instance " + std::to_string(like) +
										" is not in the spatial partition of its place (the stock rule does not hold for it); it cannot be copied");
								}
								copy.spatialPartitionIndex = walk_tree(pop, placed.initialOrigin, true, where);
							}
							fresh.push_back({copy, pop});
						}
					}
					if (!any_drop && fresh.empty())
					{
						// Only the instance numbers change (instances after a removed one).
						for (std::uint32_t i = 0; i < n; i++)
						{
							auto& def = defs[i];
							if (scriptable_type(def) && def.scriptableMapIndex < before)
							{
								const auto now = final_index(def.scriptableMapIndex);
								if (now != def.scriptableMapIndex)
								{
									def.scriptableMapIndex = now;
									++dyn_renumbered;
								}
							}
						}
						continue;
					}
					// The rebuilt list.
					auto& out = dynent_storage.emplace_back();
					out.reserve(n + fresh.size());
					std::vector<std::uint32_t> map_old(n, no_node), old_of, fresh_at(fresh.size());
					const auto put_fresh = [&]
					{
						for (std::size_t k = 0; k < fresh.size(); k++)
						{
							fresh_at[k] = static_cast<std::uint32_t>(out.size());
							out.push_back(fresh[k].def);
							old_of.push_back(no_node);
						}
					};
					for (std::uint32_t i = 0; i < n; i++)
					{
						if (i == insert_at)
						{
							put_fresh();
						}
						if (drop[i])
						{
							extra_pose_delta -= defs[i].___u15.scriptableMaxPartCount > 1 ? 1 : 0;
							++dyn_removed;
							continue;
						}
						map_old[i] = static_cast<std::uint32_t>(out.size());
						old_of.push_back(i);
						auto def = defs[i];
						if (scriptable_type(def) && def.scriptableMapIndex < before)
						{
							const auto now = final_index(def.scriptableMapIndex);
							if (now != def.scriptableMapIndex)
							{
								def.scriptableMapIndex = now;
								++dyn_renumbered;
							}
						}
						out.push_back(def);
					}
					if (insert_at >= n)
					{
						put_fresh();
					}
					for (const auto& entry : fresh)
					{
						extra_pose_delta += entry.def.___u15.scriptableMaxPartCount > 1 ? 1 : 0;
						max_pose_parts = (std::max)(max_pose_parts, entry.def.___u15.scriptableMaxPartCount);
					}
					dyn_added += fresh.size();
					// The chains, read from the stock list and written for the new one.
					const auto read_chain = [&](const std::uint32_t head)
					{
						std::vector<std::uint32_t> seq;
						for (auto at = head; at != no_node; at = defs[at].spatialNode.spatialNodeNext)
						{
							if (at >= n || seq.size() > n)
							{
								throw std::runtime_error(zone_name + ": a chain of " + list_name + " is broken");
							}
							seq.push_back(at);
						}
						return seq;
					};
					const auto renumbered_chain = [&](const std::vector<std::uint32_t>& seq)
					{
						std::vector<std::uint32_t> now;
						for (const auto at : seq)
						{
							if (map_old[at] != no_node)
							{
								now.push_back(map_old[at]);
							}
						}
						return now;
					};
					const auto insert_descending = [](std::vector<std::uint32_t>& seq, const std::uint32_t at)
					{
						seq.insert(std::find_if(seq.begin(), seq.end(), [&](const std::uint32_t other) { return other < at; }), at);
					};
					std::vector<bool> on_chain(out.size(), false);
					const auto write_chain = [&](const std::vector<std::uint32_t>& seq, std::uint32_t& head, std::uint32_t* tail)
					{
						head = seq.empty() ? no_node : seq.front();
						for (std::size_t j = 0; j < seq.size(); j++)
						{
							if (on_chain[seq[j]])
							{
								throw std::runtime_error(zone_name + ": entry " + std::to_string(seq[j]) + " of " + list_name + " is on two chains");
							}
							on_chain[seq[j]] = true;
							out[seq[j]].spatialNode.spatialNodeNext = j + 1 < seq.size() ? seq[j + 1] : no_node;
						}
						if (tail)
						{
							*tail = seq.empty() ? no_node : seq.back();
						}
					};
					const auto old_no_spatial = read_chain(list->dynEntNoSpatialList.spatialNodeNext);
					auto no_spatial = renumbered_chain(old_no_spatial);
					struct Bucket
					{
						std::uint32_t partition;
						std::vector<std::uint32_t> seq;
					};
					std::array<std::vector<Bucket>, 2> buckets;
					std::array<std::int64_t, 2> population_delta{};
					for (auto p = 0; p < 2; p++)
					{
						const auto* chains = list->dynEntSpatialPopulation[p];
						for (std::uint32_t b = 0; chains && chains->buckets && chains->partitionIds && b < chains->partitionCount; b++)
						{
							const auto old = read_chain(chains->buckets[b].spatialNodeNext);
							population_delta[p] -= static_cast<std::int64_t>(old.size());
							buckets[p].push_back({chains->partitionIds[b], renumbered_chain(old)});
						}
					}
					for (std::size_t k = 0; k < fresh.size(); k++)
					{
						const auto pop = fresh[k].pop;
						if (pop < 0)
						{
							insert_descending(no_spatial, fresh_at[k]);
							continue;
						}
						if (!list->dynEntSpatialPopulation[pop])
						{
							throw std::runtime_error(zone_name + ": " + list_name + " has no spatial population " + std::to_string(pop));
						}
						auto& v = buckets[pop];
						const auto partition = fresh[k].def.spatialPartitionIndex;
						auto it = std::ranges::find_if(v, [&](const Bucket& b) { return b.partition == partition; });
						if (it == v.end())
						{
							// A partition with no entry of this list yet: a new bucket, in the stock order (ascending ids).
							it = v.insert(std::ranges::find_if(v, [&](const Bucket& b) { return b.partition > partition; }), Bucket{partition, {}});
							++buckets_added;
						}
						insert_descending(it->seq, fresh_at[k]);
					}
					write_chain(no_spatial, list->dynEntNoSpatialList.spatialNodeNext, &list->dynEntNoSpatialListTail.spatialNodeNext);
					no_spatial_delta += static_cast<std::int64_t>(no_spatial.size()) - static_cast<std::int64_t>(old_no_spatial.size());
					for (auto p = 0; p < 2; p++)
					{
						auto* chains = list->dynEntSpatialPopulation[p];
						if (!chains)
						{
							continue;
						}
						auto& v = buckets[p];
						const auto count_before = v.size();
						std::erase_if(v, [](const Bucket& b) { return b.seq.empty(); });
						buckets_dropped += count_before - v.size();
						auto& heads = dynent_node_storage.emplace_back(v.size());
						auto& tails = dynent_node_storage.emplace_back(v.size());
						auto& ids = dynent_u32_storage.emplace_back(v.size());
						for (std::size_t b = 0; b < v.size(); b++)
						{
							ids[b] = v[b].partition;
							write_chain(v[b].seq, heads[b].spatialNodeNext, &tails[b].spatialNodeNext);
							population_delta[p] += static_cast<std::int64_t>(v[b].seq.size());
						}
						chains->buckets = heads.data();
						chains->bucketTails = chains->bucketTails ? tails.data() : nullptr;
						chains->partitionIds = ids.data();
						chains->partitionCount = static_cast<std::uint32_t>(v.size());
						chains->currentPopulation = static_cast<std::uint32_t>(static_cast<std::int64_t>(chains->currentPopulation) + population_delta[p]);
					}
					// An entry on no chain (none on a stock map) keeps its link, renumbered.
					for (std::size_t i = 0; i < out.size(); i++)
					{
						if (on_chain[i])
						{
							continue;
						}
						++off_chain;
						const auto old = old_of[i];
						const auto next = old == no_node ? no_node : defs[old].spatialNode.spatialNodeNext;
						out[i].spatialNode.spatialNodeNext = next != no_node && next < n ? map_old[next] : no_node;
					}
					const auto delta = static_cast<std::int64_t>(out.size()) - static_cast<std::int64_t>(n);
					list->dynEntDefList[0] = out.data();
					list->dynEntCount[0] = static_cast<std::uint32_t>(out.size());
					list->dynEntCountTotal = static_cast<std::uint32_t>(static_cast<std::int64_t>(list->dynEntCountTotal) + delta);
					total_delta += delta;
				}
				if (dyn_added || dyn_removed)
				{
					const auto shift = [](auto& count, const std::int64_t by)
					{
						count = static_cast<std::remove_reference_t<decltype(count)>>(static_cast<std::int64_t>(count) + by);
					};
					shift(ents->dynEntCount[0], total_delta);
					shift(ents->dynEntCountTotal, total_delta);
					shift(ents->dynEntNoSpatialCount[0], no_spatial_delta);
					shift(ents->dynEntsWithExtraPosePartsCount, extra_pose_delta);
					ents->dynEntMaxPosePartCount = max_pose_parts;
				}
				if (total_delta != 0)
				{
					auto* world = static_cast<GfxWorld*>(find(ASSET_TYPE_GFXWORLD));
					if (!world)
					{
						throw std::runtime_error(zone_name + ": no GfxWorld to size the dynamic entity arrays by");
					}
					auto& dpvs = world->dpvsDyn;
					const auto old_count = dpvs.dynEntClientCount[0];
					const auto old_words = dpvs.dynEntClientWordCount[0];
					const auto new_signed = static_cast<std::int64_t>(old_count) + total_delta;
					const auto old_base = (old_count + 31) / 32;
					const auto factor = old_base ? old_words / old_base : 0;
					if (!old_count || new_signed <= 0 || (factor != 1 && factor != 2) || factor * old_base != old_words ||
						world->dynEntMotionBitsEntries[0] != old_words || world->numPrimaryLightDynEntShadowVisEntries[0] % old_count)
					{
						throw std::runtime_error(zone_name + ": the GfxWorld dynamic entity arrays do not have the stock sizes; "
							"no dynamic entity can be added or removed");
					}
					const auto new_count = static_cast<std::uint32_t>(new_signed);
					const auto new_words = factor * ((new_count + 31) / 32);
					const auto shadow_per = world->numPrimaryLightDynEntShadowVisEntries[0] / old_count;
					const auto cells = world->dpvsPlanes.cellCount;
					const auto zero = [](const void* data, const std::size_t bytes)
					{
						const auto* b = static_cast<const std::uint8_t*>(data);
						return !data || std::all_of(b, b + bytes, [](const std::uint8_t x) { return x == 0; });
					};
					bool empty = zero(dpvs.dynEntCellBits[0], std::size_t(cells) * old_words * 4) && zero(world->dynEntMotionBits[0], std::size_t(old_words) * 4) &&
						zero(world->primaryLightDynEntShadowVis[0], std::size_t(world->numPrimaryLightDynEntShadowVisEntries[0]) * 4);
					for (auto j = 0; j < 12; j++)
					{
						empty = empty && (j == 3 || zero(dpvs.dynEntVisData[0][j], std::size_t(old_words) * 32));
					}
					if (!empty)
					{
						throw std::runtime_error(zone_name + ": the GfxWorld dynamic entity runtime arrays are not empty; "
							"no dynamic entity can be added or removed");
					}
					const auto u32_array = [&](std::uint32_t*& data, const std::size_t count)
					{
						if (data)
						{
							data = dynent_u32_storage.emplace_back((std::max)(count, std::size_t(1)), 0u).data();
						}
					};
					u32_array(dpvs.dynEntCellBits[0], std::size_t(cells) * new_words);
					u32_array(world->dynEntMotionBits[0], new_words);
					u32_array(world->primaryLightDynEntShadowVis[0], std::size_t(shadow_per) * new_count);
					for (auto j = 0; j < 12; j++)
					{
						if (j != 3 && dpvs.dynEntVisData[0][j])
						{
							dpvs.dynEntVisData[0][j] = dynent_byte_storage.emplace_back(std::size_t(new_words) * 32, std::uint8_t(0)).data();
						}
					}
					if (world->sceneDynModel)
					{
						dynent_scene_models.assign(new_count, GfxSceneDynModel{});
						std::copy_n(world->sceneDynModel, (std::min)(old_count, new_count), dynent_scene_models.data());
						world->sceneDynModel = dynent_scene_models.data();
					}
					dpvs.dynEntClientCount[0] = new_count;
					dpvs.dynEntClientWordCount[0] = new_words;
					world->dynEntMotionBitsEntries[0] = new_words;
					world->numPrimaryLightDynEntShadowVisEntries[0] = shadow_per * new_count;
					report["gfx_dynamic_entity_clients_before"] = old_count;
					report["gfx_dynamic_entity_clients"] = new_count;
				}
				report["scriptable_dynamic_entities_renumbered"] = dyn_renumbered;
				report["scriptable_dynamic_entities_removed"] = dyn_removed;
				report["scriptable_dynamic_entities_added"] = dyn_added;
				report["scriptable_dynamic_entity_buckets_added"] = buckets_added;
				report["scriptable_dynamic_entity_buckets_dropped"] = buckets_dropped;
				if (off_chain)
				{
					report["scriptable_dynamic_entities_off_chain"] = off_chain;
				}
			}
			if (scriptable_records.size() > 60000)
			{
				throw std::runtime_error("more than 60000 scriptable instances");
			}
			block.mapInstances = scriptable_records.data();
			block.mapInstanceCount = static_cast<std::uint32_t>(scriptable_records.size());
			const auto raise = [](std::uint32_t& count, const std::int64_t by)
			{
				count = static_cast<std::uint32_t>(static_cast<std::int64_t>(count) + by);
			};
			raise(block.serverInstanceCount, server);
			raise(block.serverFixedPartCount, fixed_parts);
			raise(block.serverUsablePartCount, usable_parts);
			raise(block.physicsLimits.clientStandaloneMainRigidBodyCount, limits);
			raise(block.physicsLimits.clientStandaloneMotionCount, limits);
			raise(block.physicsLimits.serverStandaloneMainRigidBodyCount, limits);
			raise(block.physicsLimits.serverStandaloneMotionCount, limits);
			raise(block.physicsLimits.clientStandaloneDetailedRigidBodyCount, detail_limits);
			raise(block.physicsLimits.serverStandaloneDetailedRigidBodyCount, detail_limits);
			report["scriptable_instances_before"] = before;
			report["scriptable_instances"] = scriptable_records.size();
			report["scriptable_instances_moved"] = moved;
			report["scriptable_instances_added"] = added;
			report["scriptable_instances_moved_with_dynamic_entity"] = moved_with_dynamic;
			report["scriptable_dynamic_entities_relinked"] = relinked;
			report["scriptable_instances_removed"] = removed.size();
		}

		ScriptableCarry carry;
		if (edits.contains("carry"))
		{
			// Per source folder (the game's zone folder, or the "folder" of an entry: a zone the editor built, such as
			// the brushes added to a stock map, research 88), the zones and the assets to take from them.
			std::map<std::filesystem::path, std::map<std::string, std::set<AssetKey>>> wanted_by_folder;
			std::set<std::string> models;
			for (const auto& entry : edits.at("carry"))
			{
				auto& wanted = wanted_by_folder[entry.contains("folder") ? std::filesystem::path(entry.at("folder").get<std::string>()) : zone_folder];
				const auto type_name = entry.at("type").get<std::string>();
				std::int32_t type = -1;
				for (std::int32_t candidate = 0; candidate < 256; candidate++)
				{
					const auto format = find_asset_format(candidate);
					if (format && type_name == format->name)
					{
						type = candidate;
						break;
					}
				}
				if (type < 0)
				{
					throw std::runtime_error("carry: unknown asset type " + type_name);
				}
				const auto name = entry.at("name").get<std::string>();
				wanted[entry.at("zone").get<std::string>()].insert({type, name});
				if (type == ASSET_TYPE_XMODEL)
				{
					models.insert(name);
				}
			}
			for (auto& [folder, wanted] : wanted_by_folder)
			{
				if (!std::filesystem::is_directory(folder))
				{
					throw std::runtime_error("carry: the zone folder " + folder.string() + " was not found");
				}
				auto part = CarryStockAssets(wanted, folder);
				carry.assets.insert(carry.assets.end(), part.assets.begin(), part.assets.end());
				carry.techsets.insert(carry.techsets.end(), part.techsets.begin(), part.techsets.end());
				carry.definitions.insert(part.definitions.begin(), part.definitions.end());
				carry.roots.insert(part.roots.begin(), part.roots.end());
				carry.materialNames.insert(part.materialNames.begin(), part.materialNames.end());
				for (auto& zone_in_memory : part.zones)
				{
					carry.zones.push_back(std::move(zone_in_memory));
				}
			}

			// A carried model joins the level's model table (NetConstStrings of type 0 from the map), kept sorted.
			if (!models.empty())
			{
				for (const auto& asset : zone->assets)
				{
					if (asset.type != ASSET_TYPE_NET_CONST_STRINGS || asset.referenced)
					{
						continue;
					}
					auto* table = static_cast<NetConstStrings*>(asset.header);
					if (static_cast<std::uint32_t>(table->stringType) != 0 ||
						static_cast<std::uint32_t>(table->sourceType) != 3)
					{
						continue;
					}
					std::set<std::string> listed;
					for (std::uint32_t i = 0; i < table->entryCount; i++)
					{
						listed.insert(table->stringList[i] ? table->stringList[i] : "");
					}
					const auto before = listed.size();
					listed.insert(models.begin(), models.end());
					auto& list = string_lists.emplace_back();
					for (const auto& text : listed)
					{
						list.push_back(names.emplace_back(text).data());
					}
					table->stringList = list.data();
					table->entryCount = static_cast<std::uint32_t>(list.size());
					report["level_models_added"] = listed.size() - before;
					break;
				}
			}
		}

		if (edits.contains("replace_materials"))
		{
			const auto& list = edits.at("replace_materials");
			if (!list.is_array())
			{
				throw std::runtime_error("replace_materials must be a list of {\"from\", \"to\"}");
			}
			const auto plain = [](std::string name)
			{
				if (!name.empty() && name[0] == ',')
				{
					name.erase(0, 1);
				}
				return name;
			};
			// Every material this zone can point at: its own and referenced entries, and the carried ones.
			std::map<std::string, Material*> materials;
			const auto collect = [&](const auto& assets)
			{
				for (const auto& asset : assets)
				{
					if (asset.type == ASSET_TYPE_MATERIAL && asset.header)
					{
						materials.emplace(plain(zone_asset_name(asset.type, asset.header)), static_cast<Material*>(asset.header));
					}
				}
			};
			collect(zone->assets);
			collect(carry.assets);
			collect(carry.techsets);
			auto* world = static_cast<GfxWorld*>(find(ASSET_TYPE_GFX_MAP));
			if (!world)
			{
				throw std::runtime_error(zone_name + " has no world (gfx_map)");
			}
			std::size_t replaced = 0;
			std::size_t at = 0;
			for (const auto& entry : list)
			{
				const auto where = "replace_materials[" + std::to_string(at++) + "]";
				if (!entry.is_object() || !entry.contains("from") || !entry.contains("to") || !entry.at("from").is_string() || !entry.at("to").is_string())
				{
					throw std::runtime_error(where + ": needs \"from\" and \"to\" material names");
				}
				const auto from = plain(entry.at("from").get<std::string>()), to = plain(entry.at("to").get<std::string>());
				const auto found = materials.find(to);
				if (found == materials.end())
				{
					throw std::runtime_error(where + ": material " + to + " is not in this zone; carry it in (\"carry\" with type material)");
				}
				std::size_t count = 0;
				for (std::uint32_t i = 0; i < world->surfaces.count && world->surfaces.surfaces; i++)
				{
					auto& surface = world->surfaces.surfaces[i];
					if (!surface.material || !surface.material->name)
					{
						continue;
					}
					const auto name = plain(surface.material->name);
					// "@sky": the map's own sky, a material named <folder>/sky_<...>.
					const auto slash = name.find('/');
					const bool sky = from == "@sky" && slash != std::string::npos && name.compare(slash + 1, 4, "sky_") == 0 && name != to;
					if (sky || name == from)
					{
						surface.material = found->second;
						++count;
					}
				}
				if (!count)
				{
					throw std::runtime_error(where + ": no world surface of " + zone_name + " uses material " + from);
				}
				replaced += count;
			}
			report["world_surfaces_rematerialed"] = replaced;
		}

		// "lit_filter": {"program": "<compiled filter pixel shader>", "shared": ["<zone>", ...]}
		//   IW8 Radiant's map shader filter on a stock map: every lit technique of the map's technique sets (zone
		//   techsets_<map>) gets its pixel shader with the filter's instructions spliced in before its end
		//   (dxbc_splice.hpp), so the game's own lighting stays and its final colour passes through the filter. What
		//   changes gets a new identity (research 114 section 7): the shader a new name, the technique a new name and
		//   new pipeline-state hashes, the technique set a new name; the materials of this zone then name the new sets.
		//   A shader the techset zone only names (its program is in a shared zone) is looked up in the "shared" zones
		//   and added to the techset zone under its new name.
		std::unique_ptr<loaded_zone> filtered_techsets;
		std::vector<std::unique_ptr<loaded_zone>> filter_shared_zones;
		std::deque<std::vector<std::uint8_t>> filter_programs;
		std::deque<ComputeShader> filter_shaders;
		std::vector<zone_asset> filter_additions;
		if (edits.contains("lit_filter"))
		{
			const auto& node = edits.at("lit_filter");
			if (!node.is_object() || !node.contains("program") || !node.at("program").is_string())
			{
				throw std::runtime_error("lit_filter: needs \"program\", the compiled filter pixel shader file");
			}
			std::vector<std::uint8_t> filter;
			{
				std::ifstream file(std::filesystem::path(node.at("program").get<std::string>()), std::ios::binary);
				if (!file)
				{
					throw std::runtime_error("lit_filter: cannot read " + node.at("program").get<std::string>());
				}
				filter.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
			}
			if (!iw8r::dxbcChecksumValid(filter))
			{
				throw std::runtime_error("lit_filter: the program is not a DXBC shader with a valid checksum");
			}
			const auto techsets_path = zone_folder / ("techsets_" + zone_name + ".ff");
			if (!std::filesystem::exists(techsets_path))
			{
				throw std::runtime_error("stock zone " + techsets_path.string() + " was not found");
			}
			filtered_techsets = load_zone(techsets_path, {.keep_temp_data = true});

			const auto hex = [](const std::uint8_t* bytes, const std::size_t count)
			{
				static const char digits[] = "0123456789abcdef";
				std::string text;
				for (std::size_t i = 0; i < count; i++)
				{
					text += digits[bytes[i] >> 4];
					text += digits[bytes[i] & 15];
				}
				return text;
			};
			const auto fnv = [](const std::string& text, std::uint64_t seed)
			{
				std::uint64_t value = 1469598103934665603ull ^ seed;
				for (const unsigned char byte : text)
				{
					value = (value ^ byte) * 1099511628211ull;
				}
				return value;
			};
			const auto hex64 = [&](const std::uint64_t value)
			{
				std::uint8_t bytes[8];
				for (int i = 0; i < 8; i++)
				{
					bytes[i] = static_cast<std::uint8_t>(value >> (56 - 8 * i));
				}
				return hex(bytes, 8);
			};
			const std::string filter_id = hex(filter.data() + 4, 16);

			// A shader's program by name from the shared zones (loaded on first use, in the order given).
			std::vector<std::string> shared_names;
			if (node.contains("shared"))
			{
				for (const auto& entry : node.at("shared"))
				{
					shared_names.push_back(entry.get<std::string>());
				}
			}
			std::map<std::string, const ComputeShader*> shared_shaders;
			std::size_t shared_loaded = 0;
			const auto shared_program = [&](const std::string& name) -> const ComputeShader*
			{
				for (;;)
				{
					if (const auto found = shared_shaders.find(name); found != shared_shaders.end())
					{
						return found->second;
					}
					if (shared_loaded >= shared_names.size())
					{
						return nullptr;
					}
					const auto path = zone_folder / (shared_names[shared_loaded++] + ".ff");
					if (!std::filesystem::exists(path))
					{
						continue;
					}
					filter_shared_zones.push_back(load_zone(path, {.keep_temp_data = true}));
					for (const auto& asset : filter_shared_zones.back()->assets)
					{
						if (asset.type != ASSET_TYPE_PIXELSHADER || asset.referenced)
						{
							continue;
						}
						const auto* shader = static_cast<const ComputeShader*>(asset.header);
						if (shader->name && *shader->name != ',' && shader->prog.loadDef.program && shader->prog.loadDef.programSize)
						{
							shared_shaders.try_emplace(shader->name, shader);
						}
					}
				}
			};

			const auto lit = [](const MaterialTechniqueType type)
			{
				const auto value = static_cast<std::uint32_t>(type);
				return value == 34 || value == 35 || value == 36 || value == 151;  // LIT_FORWARDPLUS_BITMASK, its VRS forms, WORLDTESS
			};
			std::map<const ComputeShader*, ComputeShader*> filtered;  // a stock shader -> its filtered one (nullptr: not possible)
			std::set<const MaterialTechnique*> done_techniques;
			std::set<const MaterialPipelineState*> done_states;
			std::map<std::string, std::string> renamed_sets;
			std::size_t techniques = 0, shaders = 0, shaders_shared = 0, shaders_missing = 0, shaders_refused = 0, sets = 0;
			std::string first_refusal;
			auto missing_names = nlohmann::ordered_json::array();
			for (auto& asset : filtered_techsets->assets)
			{
				if (asset.type != ASSET_TYPE_TECHNIQUE_SET || asset.referenced)
				{
					continue;
				}
				auto* set = static_cast<MaterialTechniqueSet*>(asset.header);
				if (!set->name || *set->name == ',' || !set->maskedTechniques)
				{
					continue;
				}
				std::size_t count = 0;
				for (const auto word : set->techniqueMask.mask)
				{
					count += static_cast<std::size_t>(std::popcount(word));
				}
				bool changed = false;
				for (std::size_t i = 0; i < count; i++)
				{
					auto* technique = set->maskedTechniques[i];
					if (!technique || !lit(technique->type) || !technique->pixelShader || !technique->pixelShader->name)
					{
						continue;
					}
					if (!done_techniques.insert(technique).second)
					{
						changed = changed || filtered.contains(technique->pixelShader);
						continue;
					}
					auto* shader = technique->pixelShader;
					ComputeShader* fresh = nullptr;
					if (const auto known = filtered.find(shader); known != filtered.end())
					{
						fresh = known->second;
					}
					else
					{
						std::string plain = shader->name;
						const bool named_only = !plain.empty() && plain.front() == ',';
						if (named_only)
						{
							plain.erase(0, 1);
						}
						const ComputeShader* source = shader;
						if (named_only || !shader->prog.loadDef.program || !shader->prog.loadDef.programSize)
						{
							source = shared_program(plain);
						}
						std::vector<std::uint8_t> spliced;
						std::string error;
						if (!source)
						{
							++shaders_missing;
							if (missing_names.size() < 32)
							{
								missing_names.push_back(plain);
							}
						}
						else if (!iw8r::dxbcSpliceFilter({source->prog.loadDef.program, source->prog.loadDef.program + source->prog.loadDef.programSize},
							filter, spliced, &error))
						{
							++shaders_refused;
							if (first_refusal.empty())
							{
								first_refusal = plain + ": " + error;
							}
						}
						else
						{
							filter_programs.push_back(std::move(spliced));
							auto& program = filter_programs.back();
							names.push_back("iw8rf_" + hex(program.data() + 4, 13));
							if (source == shader)
							{
								fresh = shader;  // owned by the techset zone: changed in place
							}
							else
							{
								filter_shaders.push_back(*source);
								fresh = &filter_shaders.back();
								filter_additions.push_back({ASSET_TYPE_PIXELSHADER, fresh, false});
								++shaders_shared;
							}
							fresh->name = names.back().data();
							fresh->prog.cs = nullptr;
							fresh->prog.loadDef.program = program.data();
							fresh->prog.loadDef.programSize = static_cast<std::uint32_t>(program.size());
							// The 4-byte identity after the program size (research 114 section 7.3).
							std::memcpy(reinterpret_cast<std::uint8_t*>(fresh) + 36, program.data() + 4, 4);
							++shaders;
						}
						filtered[shader] = fresh;
						if (fresh)
						{
							filtered[fresh] = fresh;
						}
					}
					if (!fresh)
					{
						continue;
					}
					const std::string old_name = technique->name ? technique->name : "TECHNIQUE";
					technique->pixelShader = fresh;
					// TECHNIQUE_..._<16 hex>: the hash part is replaced; any other name gets one appended.
					std::string stem = old_name;
					if (stem.size() > 17 && stem[stem.size() - 17] == '_' &&
						stem.find_first_not_of("0123456789abcdef", stem.size() - 16) == std::string::npos)
					{
						stem.erase(stem.size() - 17);
					}
					names.push_back(stem + "_" + hex64(fnv(old_name + ";" + fresh->name, 1)));
					technique->name = names.back().data();
					const std::size_t states = std::size_t{technique->pipelineStateCount} * technique->pipelineStateInputLayoutCount;
					if (technique->pipelineStates && done_states.insert(technique->pipelineStates).second)
					{
						for (std::size_t state = 0; state < states; state++)
						{
							auto& entry = technique->pipelineStates[state];
							entry.hash = fnv(std::to_string(entry.hash) + ";" + fresh->name + ";" + filter_id, 2 + state);
							entry.pso = nullptr;
						}
					}
					changed = true;
					++techniques;
				}
				if (changed)
				{
					const std::string old_name = set->name;
					const auto slash = old_name.find('/');
					names.push_back((slash == std::string::npos ? std::string() : old_name.substr(0, slash + 1)) + "iw8rf_" +
						hex64(fnv(old_name + ";" + filter_id, 3)) + hex64(fnv(old_name + ";" + filter_id, 4)).substr(0, 8));
					set->name = names.back().data();
					renamed_sets.emplace(old_name, names.back());
					++sets;
				}
			}

			// The materials of this zone name the new sets.
			std::set<const void*> renamed_references;
			std::size_t materials = 0;
			for (const auto& asset : zone->assets)
			{
				if (asset.type != ASSET_TYPE_MATERIAL || asset.referenced)
				{
					continue;
				}
				auto* material = static_cast<Material*>(asset.header);
				if (!material->techniqueSet || !material->techniqueSet->name)
				{
					continue;
				}
				std::string plain = material->techniqueSet->name;
				const bool comma = !plain.empty() && plain.front() == ',';
				if (comma)
				{
					plain.erase(0, 1);
				}
				const auto found = renamed_sets.find(plain);
				if (found == renamed_sets.end())
				{
					// Already renamed through another material of the same reference?
					if (renamed_references.contains(material->techniqueSet))
					{
						++materials;
					}
					continue;
				}
				names.push_back((comma ? "," : "") + found->second);
				material->techniqueSet->name = names.back().data();
				renamed_references.insert(material->techniqueSet);
				++materials;
			}
			report["lit_filter"] = {
				{"filter", filter_id},
				{"technique_sets", sets},
				{"techniques", techniques},
				{"shaders", shaders},
				{"shaders_from_shared_zones", shaders_shared},
				{"shaders_without_program", shaders_missing},
				{"shaders_without_program_names", missing_names},
				{"shaders_refused", shaders_refused},
				{"first_refusal", first_refusal},
				{"materials_renamed", materials},
				{"shared_zones_read", filter_shared_zones.size()},
			};
			if (!techniques)
			{
				throw std::runtime_error("lit_filter: no lit technique of techsets_" + zone_name + " could take the filter" +
					(first_refusal.empty() ? std::string() : " (" + first_refusal + ")"));
			}
		}

		const write_options options{.expected_size = zone->file.payload.size() + zone->file.payload.size() / 16};
		std::filesystem::create_directories(output);
		const auto write = [&](const loaded_zone& source, const std::vector<zone_asset>& wanted_additions,
			const std::string& name, const write_options& how)
		{
			const auto additions = NewAdditions(source, wanted_additions);
			auto written = write_zone(source, how, additions);
			xfile::fastfile file{};
			file.payload = std::move(written.data);
			file.block_sizes = std::move(written.block_sizes);
			file.xfile_version = source.file.xfile_version;
			file.transient = source.file.transient;
			file.transient_type = source.file.transient_type;
			file.preload_walk_size = std::max<std::uint64_t>(written.preload_walk_size, source.file.preload_walk_size);
			// Stock zones declare more for the preload block than the writer computes: never less than the stock size.
			for (std::size_t block = 0; block < file.block_sizes.size() && block < source.file.block_sizes.size(); block++)
			{
				file.block_sizes[block] = std::max(file.block_sizes[block], source.file.block_sizes[block]);
			}
			xfile::write(output / (name + ".ff"), definition().format, file);
			// The patch that turns the stock file into the edited zone: the game applies <zone>.fp beside <zone>.ff, so
			// the edit installs as this one file and the stock zone stays as shipped. A stock patch of the same name is
			// replaced by it (the edit was made on the patched zone).
			if (edits.value("patch", true))
			{
				std::filesystem::create_directories(output / "patch");
				write_patch(zone_folder / (name + ".ff"), output / (name + ".ff"), output / "patch" / (name + ".fp"));
			}
			return additions.size();
		};
		// "techsets_only": true: the map zone is not written (an edit that changes only the techset zone, a lit filter).
		if (!edits.value("techsets_only", false))
		{
			report["assets_added"] = write(*zone, carry.assets, zone_name, options);
		}

		if (!carry.techsets.empty() || filtered_techsets)
		{
			const auto techsets_path = zone_folder / ("techsets_" + zone_name + ".ff");
			if (!std::filesystem::exists(techsets_path))
			{
				throw std::runtime_error("stock zone " + techsets_path.string() + " was not found");
			}
			// The techset zone as the lit filter changed it, or as shipped.
			const auto techsets = filtered_techsets ? std::move(filtered_techsets) : load_zone(techsets_path, {.keep_temp_data = true});
			auto additions = carry.techsets;
			additions.insert(additions.end(), filter_additions.begin(), filter_additions.end());
			const write_options techset_options{.expected_size = techsets->file.payload.size() + techsets->file.payload.size() / 4};
			report["techset_assets_added"] = write(*techsets, additions, "techsets_" + zone_name, techset_options);
		}

		std::ofstream(output / (zone_name + ".edit-report.json"), std::ios::binary | std::ios::trunc) << report.dump(2) << "\n";
		ZONETOOL_INFO("editzone: wrote %s (%s)", (output / (zone_name + ".ff")).string().data(), report.dump().data());
	}
}
