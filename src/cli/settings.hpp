#pragma once

#include <game.hpp>

namespace zonetool::cli
{
	struct settings
	{
		const game* game{};
		std::filesystem::path game_path;
		json config;

		std::filesystem::path zone_folder() const;
		std::optional<std::filesystem::path> game_path_of(std::string_view name) const;
	};

	settings load_settings(std::vector<std::string>& args);
}
