#include <std_include.hpp>
#include "commands.hpp"
#include "game_registry.hpp"

#include <formats/csv_generator.hpp>
#include <game_mode.hpp>

namespace zonetool::cli
{
	namespace
	{
		std::optional<std::filesystem::path> require_zone(const settings& settings, const std::span<const std::string> args,
			const std::string_view usage)
		{
			if (args.empty())
			{
				std::cout << std::format("usage: {}\n", usage);
				return {};
			}

			auto path = game_files::find_zone(args.front());
			if (!path)
			{
				std::cout << std::format("zone \"{}\" not found in {}\n", args.front(), game_files::get().description());
			}

			return path;
		}

		void verify_all(const settings& settings)
		{
			const auto zones = game_files::list_zones();

			std::size_t failed{};
			for (const auto& zone : zones)
			{
				try
				{
					settings.game->verify_zone(settings.zone_folder(), zone);
				}
				catch (const std::exception& e)
				{
					failed++;
					std::cout << std::format("{}: {}\n", zone.string(), e.what());
				}
			}

			std::cout << std::format("{} of {} zones passed\n", zones.size() - failed, zones.size());
		}

		void convert_zone(const settings& settings, const converter& converter, const std::filesystem::path& zone_path,
			std::unordered_map<std::string, std::string> options)
		{
			zone_conversion conversion{};
			conversion.source_game_path = settings.game_path;
			conversion.target_game_path = settings.game_path_of(converter.target).value_or(std::filesystem::path{});
			conversion.output_folder = "output";

			if (settings.config.contains("tools"))
			{
				for (const auto& [name, value] : settings.config["tools"].items())
				{
					options.try_emplace(name, value.get<std::string>());
				}
			}

			conversion.options = std::move(options);
			converter.convert_zone(zone_path, conversion);
		}
	}

	void dump_zone(const settings& settings, const std::span<const std::string> args)
	{
		constexpr std::string_view usage = "dumpzone <zone> [--target <game> [--<option> <value>]...]";

		const auto has_target = args.size() >= 3 && args.size() % 2 == 1 && args[1] == "--target";
		if (args.size() != 1 && !has_target)
		{
			std::cout << std::format("usage: {}\n", usage);
			return;
		}

		std::unordered_map<std::string, std::string> options;
		for (auto i = 3u; i + 1 < args.size(); i += 2)
		{
			if (!args[i].starts_with("--"))
			{
				std::cout << std::format("usage: {}\n", usage);
				return;
			}

			options[args[i].substr(2)] = args[i + 1];
		}

		const converter* converter{};
		if (has_target && args[2] != settings.game->name)
		{
			converter = find_converter(settings.game->name, args[2]);
			if (!converter)
			{
				std::cout << std::format("no converter from {} to {} in this build\n", settings.game->name, args[2]);
				return;
			}
		}

		const auto path = require_zone(settings, args, usage);
		if (!path)
		{
			return;
		}

		if (converter && converter->convert_zone)
		{
			convert_zone(settings, *converter, *path, std::move(options));
			return;
		}

		if (!options.empty())
		{
			std::cout << "options need a converter that converts whole zones\n";
			return;
		}

		settings.game->dump_zone(settings.zone_folder(), *path, converter ? converter->dump_asset : nullptr);
	}

	void verify_zone(const settings& settings, const std::span<const std::string> args)
	{
		if (!args.empty() && args.front() == "--all")
		{
			verify_all(settings);
			return;
		}

		if (const auto path = require_zone(settings, args, "verifyzone <zone> | verifyzone --all"))
		{
			settings.game->verify_zone(settings.zone_folder(), *path);
			std::cout << std::format("zone \"{}\" passed\n", args.front());
		}
	}

	void build_zone(const settings& settings, const std::span<const std::string> args)
	{
		if (args.empty())
		{
			std::cout << "usage: buildzone <zone>\n";
			return;
		}

		settings.game->build_zone(settings.zone_folder(), args.front());
	}

	void generate_csv(const settings& settings, const std::span<const std::string> args)
	{
		if (args.empty())
		{
			std::cout << "usage: generatecsv <map> [sp]\n";
			return;
		}

		const auto token_name = settings.game->token_name;
		const auto get_token_name = [token_name](const std::uint32_t id)
		{
			return token_name ? token_name(id) : std::to_string(id);
		};

		auto is_sp = false;
		std::vector<std::string> extra_paths;
		for (auto i = 1u; i < args.size(); ++i)
		{
			if (args[i] == "sp")
			{
				is_sp = true;
			}
			else if (args[i] == "--path" && i + 1 < args.size())
			{
				extra_paths.emplace_back(args[++i]);
			}
		}

		csv_generator::generate_map_csv(args.front(), get_token_name, is_sp,
			::game::get_mode_from_string(std::string(settings.game->name)), extra_paths);
	}
}
