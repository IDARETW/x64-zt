#include <std_include.hpp>
#include "commands.hpp"
#include "games/iw8/havok/compendium.hpp"
#include "games/iw8/havok/tagfile.hpp"
#include "game_registry.hpp"

#include <formats/csv_generator.hpp>
#include <game_mode.hpp>
#include <utils/io.hpp>

namespace zonetool::convert::iw3_iw8
{
	std::string stream_info_json(const std::filesystem::path& zone_path, const std::string& zone_name);
	void edit_zone(const std::filesystem::path& game_path, const std::string& zone_name,
		const std::filesystem::path& edits_path, const std::filesystem::path& output);
	void convert_extraction(const std::filesystem::path& zone_path, const std::filesystem::path& extraction,
		const zone_conversion& conversion);
}

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

		std::optional<std::string_view> target_argument(const std::span<const std::string> args, const std::size_t positional_count)
		{
			if (args.size() == positional_count)
			{
				return std::string_view{};
			}

			if (args.size() == positional_count + 2 && args[positional_count] == "--target")
			{
				return args[positional_count + 1];
			}

			return {};
		}

		std::optional<const converter*> select_converter(const settings& settings, const std::string_view target_name)
		{
			if (target_name.empty() || target_name == settings.game->name)
			{
				return nullptr;
			}

			const auto* converter = find_converter(settings.game->name, target_name);
			if (!converter)
			{
				std::cout << std::format("no converter from {} to {} in this build\n", settings.game->name, target_name);
				return {};
			}

			if (converter->setup)
			{
				const auto* target = find_game(converter->target);
				if (!target)
				{
					std::cout << std::format("game {} is not in this build\n", converter->target);
					return {};
				}

				converter->setup(settings.game_folder(*target));
			}

			return converter;
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
			conversion.output_folder = "output";
			try
			{
				conversion.target_game_path = settings.game_folder(*find_game(converter.target));
			}
			catch (const std::exception&)
			{
				// The converter can also take the target game folder from its options (iw3_iw8: --replay).
			}

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

		std::filesystem::path listing_path(const settings& settings, const std::filesystem::path& zone_path)
		{
			auto relative = zone_path.lexically_relative(game_files::get().path("").parent_path());
			if (relative.empty() || *relative.begin() == "..")
			{
				relative = zone_path.filename();
			}

			return std::filesystem::path("asset_listing") / settings.game->name / relative.replace_extension(".zone");
		}

		void write_listing(const settings& settings, const std::filesystem::path& zone_path)
		{
			const auto path = listing_path(settings, zone_path);
			if (!utils::io::write_file(path.string(), settings.game->list_zone(settings.zone_folder(), zone_path)))
			{
				throw std::runtime_error(std::format("failed to write {}", path.string()));
			}

			std::cout << std::format("{}\n", path.string());
		}

		void list_all(const settings& settings)
		{
			const auto zones = game_files::list_zones();

			std::size_t failed{};
			for (const auto& zone : zones)
			{
				try
				{
					write_listing(settings, zone);
				}
				catch (const std::exception& e)
				{
					failed++;
					std::cout << std::format("{}: {}\n", zone.string(), e.what());
				}
			}

			std::cout << std::format("{} of {} zones listed\n", zones.size() - failed, zones.size());
		}
	}

	void dump_zone(const settings& settings, const std::span<const std::string> args)
	{
		constexpr std::string_view usage = "dumpzone <zone> [--target <game> [--<option> <value>]...]";

		// A converter of whole zones takes options after the target.
		const std::size_t option_count = args.size() > 3 && args[1] == "--target" ? args.size() - 3 : 0;
		const auto target = target_argument(args.first(args.size() - option_count), 1);
		if (!target || option_count % 2)
		{
			std::cout << std::format("usage: {}\n", usage);
			return;
		}

		std::unordered_map<std::string, std::string> options;
		for (auto i = args.size() - option_count; i < args.size(); i += 2)
		{
			if (!args[i].starts_with("--"))
			{
				std::cout << std::format("usage: {}\n", usage);
				return;
			}

			options[args[i].substr(2)] = args[i + 1];
		}

		const auto path = require_zone(settings, args, usage);
		if (!path)
		{
			return;
		}

		const auto converter = select_converter(settings, *target);
		if (!converter)
		{
			return;
		}

		if (*converter && (*converter)->convert_zone)
		{
			convert_zone(settings, **converter, *path, std::move(options));
			return;
		}

		if (!options.empty())
		{
			std::cout << "options need a converter that converts whole zones\n";
			return;
		}

		if (*converter && !(*converter)->dump_asset)
		{
			throw std::runtime_error(std::format("converter {} -> {} does not support zone conversion",
				settings.game->name, (*converter)->target));
		}

		settings.game->dump_zone(settings.zone_folder(), *path, *converter);
	}

	void dump_asset(const settings& settings, const std::span<const std::string> args)
	{
		constexpr std::string_view usage = "dumpasset <type> <name> <zone> [--target <game>]";

		const auto target = target_argument(args, 3);
		if (!target)
		{
			std::cout << std::format("usage: {}\n", usage);
			return;
		}

		const auto path = require_zone(settings, args.subspan(2), usage);
		if (!path)
		{
			return;
		}

		const auto converter = select_converter(settings, *target);
		if (!converter)
		{
			return;
		}

		if (*converter && !(*converter)->dump_asset)
		{
			throw std::runtime_error(std::format("converter {} -> {} does not support single-asset conversion",
				settings.game->name, (*converter)->target));
		}

		settings.game->dump_asset(settings.zone_folder(), *path, args[0], args[1], *converter);
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

	void list_zone(const settings& settings, const std::span<const std::string> args)
	{
		if (!settings.game->list_zone)
		{
			std::cout << std::format("listzone is not supported for {}\n", settings.game->name);
			return;
		}

		if (!args.empty() && args.front() == "--all")
		{
			list_all(settings);
			return;
		}

		if (const auto path = require_zone(settings, args, "listzone <zone> | listzone --all"))
		{
			write_listing(settings, *path);
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

	// Writes the streaming info of a map zone as JSON: the transient sets, the streamed (transient) files with their
	// costs, the transient assets, the world grid tile maps and the counts (IW8 Radiant research 90). The game takes
	// the collision tile count and the names of the collision tile zones from this asset
	// (CL_TransientsCollisionMP_SetupFromAsset / RegisterFileIndex).
	void stream_info(const settings& settings, const std::span<const std::string> args)
	{
		if (settings.game->name != "iw8" || args.empty())
		{
			std::cout << "usage: -game iw8 streaminfo <zone> [<out.json>]\n";
			return;
		}
		try
		{
			// A zone of the game folder by name, or any zone file by path (a built map).
			std::optional<std::filesystem::path> path;
			if (std::error_code error; std::filesystem::is_regular_file(std::filesystem::path(args[0]), error))
			{
				path = std::filesystem::path(args[0]);
			}
			else
			{
				path = game_files::find_zone(args[0]);
			}
			if (!path)
			{
				throw std::runtime_error("zone " + args[0] + " was not found");
			}
			const auto text = convert::iw3_iw8::stream_info_json(*path, path->stem().string());
			if (args.size() > 1)
			{
				std::ofstream(std::filesystem::path(args[1]), std::ios::binary) << text;
				std::cout << "streaminfo: wrote " << args[1] << "\n";
			}
			else
			{
				std::cout << text << "\n";
			}
		}
		catch (const std::exception& e)
		{
			std::cout << std::format("streaminfo failed: {}\n", e.what());
			std::exit(1);
		}
	}

	// Lists the objects of a Havok tagfile (the shape data of a collision map, a collision tile or a physics asset,
	// saved as a file): per type the number of items, elements and bytes; with --items every item with its pointers.
	void havok_info(const settings& settings, const std::span<const std::string> args)
	{
		if (settings.game->name != "iw8" || args.empty())
		{
			std::cout << "usage: -game iw8 havokinfo <file with Havok tagfile data> [--items]\n";
			return;
		}
		try
		{
			using namespace zonetool::iw8::havok;
			std::ifstream stream(std::filesystem::path(args[0]), std::ios::binary);
			if (!stream)
			{
				throw std::runtime_error("cannot read " + args[0]);
			}
			const std::vector<std::uint8_t> data((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
			const auto& compendium = replay_compendium(settings.zone_folder());
			const auto file = read_tagfile(data, &compendium);
			// The reader maps the local types of a self-describing file onto the compendium.
			const auto& types = compendium;
			const auto listed = std::ranges::find(args, std::string("--items")) != args.end();
			std::cout << std::format("{}: {} bytes, sdk {}, {}, {} items\n", args[0], data.size(), file.sdk_version,
				file.compendium ? "compact (compendium types)" : "self-describing", file.items.size());
			struct total
			{
				std::size_t items{}, elements{}, bytes{};
			};
			std::map<std::string, total> totals;
			for (std::size_t index = 1; index < file.items.size(); index++)
			{
				const auto& entry = file.items[index];
				const auto name = types.identity(entry.type);
				auto& sum = totals[name];
				sum.items++;
				sum.elements += entry.count;
				sum.bytes += entry.data.size();
				if (listed || index == 1)
				{
					std::string targets;
					for (std::size_t p = 0; p < entry.pointers.size() && p < 12; p++)
					{
						targets += std::format(" +0x{:X}->{}", entry.pointers[p].offset, entry.pointers[p].target);
					}
					std::cout << std::format("  item {} {} x{} ({} bytes, flags {}, {} pointers{})\n", index, name, entry.count,
						entry.data.size(), entry.flags, entry.pointers.size(), targets);
				}
			}
			// --type <identity>: the members of a compendium type. --dump <item>: the bytes of one item.
			for (std::size_t at = 1; at + 1 < args.size(); at++)
			{
				if (args[at] == "--type")
				{
					const auto found = types.find(args[at + 1]);
					if (!found)
					{
						std::cout << "  no type " << args[at + 1] << "\n";
						continue;
					}
					for (auto index = *found; index;)
					{
						const auto& info = types.at(index);
						std::cout << std::format("  type {} size {} align {} parent {}\n", types.identity(index), info.size, info.alignment,
							info.parent ? types.identity(info.parent) : std::string("-"));
						for (const auto& member : info.members)
						{
							std::cout << std::format("    +0x{:X} {} : {}\n", member.offset, member.name, types.identity(member.type));
						}
						index = info.parent;
					}
				}
				if (args[at] == "--dump")
				{
					const auto index = static_cast<std::size_t>(std::stoul(args[at + 1]));
					if (index < file.items.size())
					{
						const auto& entry = file.items[index];
						std::cout << std::format("  item {} {} x{}:", index, types.identity(entry.type), entry.count);
						for (std::size_t i = 0; i < entry.data.size() && i < 65536; i++)
						{
							std::cout << (i % 32 == 0 ? "\n    " : "") << std::format("{:02x}", entry.data[i]) << (i % 4 == 3 ? " " : "");
						}
						std::cout << "\n   pointers:";
						for (const auto& pointer : entry.pointers)
						{
							std::cout << std::format(" +0x{:X}->{}", pointer.offset, pointer.target);
						}
						std::cout << "\n";
					}
				}
			}
			for (const auto& [name, sum] : totals)
			{
				std::cout << std::format("  {:>8} items {:>10} elements {:>12} bytes  {}\n", sum.items, sum.elements, sum.bytes, name);
			}
		}
		catch (const std::exception& e)
		{
			std::cout << std::format("havokinfo failed: {}\n", e.what());
			std::exit(1);
		}
	}

	void edit_zone(const settings& settings, const std::span<const std::string> args)
	{
		constexpr std::string_view usage = "editzone <zone> <edits.json> <output folder>";
		if (settings.game->name != "iw8")
		{
			std::cout << "editzone is only supported for -game iw8\n";
			return;
		}
		if (args.size() != 3)
		{
			std::cout << std::format("usage: {}\n", usage);
			return;
		}
		try
		{
			convert::iw3_iw8::edit_zone(settings.game_path, args[0], args[1], args[2]);
		}
		catch (const std::exception& e)
		{
			std::cout << std::format("editzone failed: {}\n", e.what());
			std::exit(1);
		}
	}

	void build_extraction(const settings& settings, const std::span<const std::string> args)
	{
		constexpr std::string_view usage =
			"buildextraction <extraction folder> --map <id> --replay <game_dx12_ship_replay.exe> [--metadata <json>] [--style iw3|iw8] [--acts <acts.exe>] [--output <folder>]";

		if (settings.game->name != "iw8")
		{
			std::cout << "buildextraction is only supported for -game iw8\n";
			return;
		}

		if (args.empty() || args.size() % 2 == 0)
		{
			std::cout << std::format("usage: {}\n", usage);
			return;
		}

		std::unordered_map<std::string, std::string> options;
		for (auto i = 1u; i < args.size(); i += 2)
		{
			if (!args[i].starts_with("--"))
			{
				std::cout << std::format("usage: {}\n", usage);
				return;
			}

			options[args[i].substr(2)] = args[i + 1];
		}

		if (!options.contains("map") || !options.contains("replay"))
		{
			std::cout << std::format("usage: {}\n", usage);
			return;
		}

		const std::filesystem::path extraction = args.front();
		const auto map = options.at("map");

		zone_conversion conversion{};
		conversion.source_game_path = settings.game_path;
		conversion.target_game_path = settings.game_path;
		conversion.output_folder = options.contains("output") ? options.at("output") : std::string("output");
		options.erase("output");
		conversion.options = std::move(options);

		// The extraction replaces the .ff input, so the zone path only names the map.
		convert::iw3_iw8::convert_extraction(extraction / (map + ".ff"), extraction, conversion);
	}

	void refresh_zone(const settings& settings, const std::span<const std::string> args)
	{
		if (args.size() != 2 && args.size() != 3)
		{
			std::cout << "usage: refreshzone <deployed map folder> <rebuilt map folder> [<output folder>]\n";
			return;
		}

		if (!settings.game->refresh_zone)
		{
			std::cout << std::format("refreshzone is not supported for {}\n", settings.game->name);
			return;
		}

		// By default the refresh zone goes beside the deployed package, where the game's map_refresh reads it.
		settings.game->refresh_zone(args[0], args[1], args.size() == 3 ? args[2] : args[0]);
	}

	void patch_zone(const settings& settings, const std::span<const std::string> args)
	{
		if (args.size() != 2 && args.size() != 3)
		{
			std::cout << "usage: patchzone <base.ff> <new.ff> [<out.fp>]\n";
			return;
		}

		if (!settings.game->patch_zone)
		{
			std::cout << std::format("patchzone is not supported for {}\n", settings.game->name);
			return;
		}

		// By default the patch takes the base's name beside it: the game applies <zone>.fp to <zone>.ff only.
		const auto output = args.size() == 3 ? std::filesystem::path(args[2]) :
			std::filesystem::path(args[0]).replace_extension(".fp");
		settings.game->patch_zone(args[0], args[1], output);
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
