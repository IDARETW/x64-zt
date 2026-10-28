#pragma once

#include "asset_data.hpp"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <optional>
#include <string>

// The neutral data files of a zone (docs/NEUTRAL_ASSETS.md): NEUTRAL_OUT=<folder> makes a game's verifyzone write the
// neutral data of the assets its adapters decode as <folder>/<kind>/<name>.json, so that the adapters of two profiles
// can be compared (tools/compare_neutral.py).
namespace zonetool::asset_data
{
inline std::optional<std::filesystem::path> neutral_output_folder()
{
	char folder[MAX_PATH]{};
	if (!GetEnvironmentVariableA("NEUTRAL_OUT", folder, sizeof(folder)))
	{
		return std::nullopt;
	}

	return std::filesystem::path(folder);
}

inline void write_neutral_file(const std::filesystem::path& folder, const std::string& kind, const std::string& name,
	const detail::data_json& data)
{
	auto file = name;
	for (auto& c : file)
	{
		if (!std::isalnum(static_cast<unsigned char>(c)) && c != '.' && c != '_' && c != '-')
		{
			c = '_';
		}
	}

	const auto path = folder / kind / (file + ".json");
	std::filesystem::create_directories(path.parent_path());
	std::ofstream(path, std::ios::binary) << data.dump(1) << '\n';
}
} // namespace zonetool::asset_data
