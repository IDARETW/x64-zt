#pragma once

#include <filesystem>
#include <memory>
#include <set>
#include <string>

#include <json.hpp>

namespace iw8::stock
{
// Stock Replay technique sets and materials, read from the game install when a conversion needs them (AGENTS.md plan
// 1.4: world surfaces drawn by Replay's own world shaders). The technique sets of a stock map's techsets zone and the
// shared shaders of code_post_gfx are loaded with x64-zt's own Replay reader and given in the converter's technique set
// and material JSON (resources/techset_template.json, material_template.json schema 1); nothing stock is stored in this
// repository.
class Library
{
  public:
    // The zone folder of the game install (<game>/zone) and the stock map whose techsets zone holds the technique sets
    // and materials (mp_shipment's holds the world families: tw, twc, tw4, twc4).
    Library(std::filesystem::path zoneFolder, std::string map = "mp_shipment");
    ~Library();
    Library(const Library &) = delete;
    Library &operator=(const Library &) = delete;

    // A technique set: its 64-byte header, its techniques (184-byte records with their pipeline states, root parameter
    // map, state bits and arguments; pointers zeroed) and their shaders (40-byte headers with the program length, DXBC).
    nlohmann::json TechniqueSet(const std::string &name) const;
    // A material: its 32-byte info, technique set name, texture headers and image names, constant table, buffer
    // indices and constant buffers, and the names of its layer materials.
    nlohmann::json Material(const std::string &name) const;

  private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

// The Materials among `names` that a common techsets zone or code_post_gfx owns with a decal volume definition
// (Material::decalVolumeMaterial). Replay loads these zones with every multiplayer map, so a map can name such a
// Material in its decal volume table.
std::set<std::string> CommonDecalVolumeMaterials(const std::filesystem::path &zoneFolder,
                                                 const std::set<std::string> &names);
} // namespace iw8::stock
