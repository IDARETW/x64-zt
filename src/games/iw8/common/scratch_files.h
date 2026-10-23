#pragma once
#include <cstdint>
#include <filesystem>
#include <memory>
#include <vector>

namespace zt::scratch
{
// Files a conversion writes for its own package step (material and technique definitions, resident images) are
// held in memory: thousands of small files cost more to create, scan, read back and delete than to build. Only
// files in the held directory (or below) are kept; the package reads a held file before the disk.
using Bytes = std::shared_ptr<std::vector<std::uint8_t>>;

// Holds the files written in `directory` from now on (and drops any held before).
void Hold(const std::filesystem::path &directory);
// Drops every held file and holds no more.
void Release();
// Whether a file at `path` would be held.
bool Holds(const std::filesystem::path &path);
// Keeps `bytes` as the file at `path` when it is in the held directory; false leaves the file to the disk.
bool Put(const std::filesystem::path &path, Bytes bytes);
// The held file at `path`, or null.
Bytes Get(const std::filesystem::path &path);
} // namespace zt::scratch
