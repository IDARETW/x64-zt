#include <std_include.hpp>
#include "scratch_files.h"

#include <mutex>
#include <string>
#include <unordered_map>

namespace zt::scratch
{
namespace
{
std::mutex g_mutex;
std::wstring g_directory; // normal absolute form with a trailing separator; empty: nothing is held
std::unordered_map<std::wstring, Bytes> g_files;

std::wstring Key(const std::filesystem::path &path)
{
    std::error_code error;
    auto absolute = std::filesystem::absolute(path, error);
    if (error)
        absolute = path;
    return absolute.lexically_normal().make_preferred().wstring();
}
} // namespace

void Hold(const std::filesystem::path &directory)
{
    auto key = Key(directory);
    if (!key.empty() && key.back() != std::filesystem::path::preferred_separator)
        key += std::filesystem::path::preferred_separator;
    const std::lock_guard lock(g_mutex);
    g_files.clear();
    g_directory = std::move(key);
}

void Release()
{
    const std::lock_guard lock(g_mutex);
    g_files.clear();
    g_directory.clear();
}

bool Holds(const std::filesystem::path &path)
{
    const auto key = Key(path);
    const std::lock_guard lock(g_mutex);
    return !g_directory.empty() && key.starts_with(g_directory);
}

bool Put(const std::filesystem::path &path, Bytes bytes)
{
    auto key = Key(path);
    const std::lock_guard lock(g_mutex);
    if (g_directory.empty() || !key.starts_with(g_directory))
        return false;
    g_files[std::move(key)] = std::move(bytes);
    return true;
}

Bytes Get(const std::filesystem::path &path)
{
    const auto key = Key(path);
    const std::lock_guard lock(g_mutex);
    if (g_files.empty())
        return nullptr;
    const auto found = g_files.find(key);
    return found == g_files.end() ? nullptr : found->second;
}
} // namespace zt::scratch
