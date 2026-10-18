#include <std_include.hpp>
#include "iw8.hpp"
#include "zone/asset_files.hpp"
#include "zone/loader.hpp"
#include "zone/writer.hpp"

#include <formats/csv.hpp>
#include <formats/localized_strings.hpp>
#include <game.hpp>

namespace zonetool::iw8
{
	const game& definition();

	namespace
	{
		using asset_key = std::pair<std::int32_t, std::string>;

		struct build_context
		{
			std::filesystem::path zone_folder;
			std::string fastfile;
			asset_parser parser;
			std::vector<zone_asset> list;
			std::vector<zone_asset> assets;
			std::map<asset_key, void*> parsed;
			std::vector<std::unique_ptr<loaded_zone>> required_zones;
			std::map<asset_key, void*> required;
			std::vector<asset_key> required_dependencies;
			std::set<asset_key> ignored;
			bool is_referencing{};
		};

		std::int32_t type_of(const std::string_view name)
		{
			for (std::int32_t type = 0; type < ASSET_TYPE_COUNT; type++)
			{
				const auto format = find_asset_format(type);
				if (format && format->name == name)
				{
					return type;
				}
			}

			return -1;
		}

		std::string name_of(const std::int32_t type, const void* header)
		{
			const auto name = get_asset_name(type, header);
			return name ? name : "";
		}

		std::string_view field(const csv::row* row, const int index)
		{
			if (index >= row->num_fields || !row->fields[index])
			{
				return {};
			}

			return row->fields[index];
		}

		std::vector<csv::row*> read_csv_rows(csv::parser& parser)
		{
			std::vector<csv::row*> rows;
			const auto raw_rows = parser.get_rows();
			for (auto i = 0; raw_rows && i < parser.get_num_rows(); i++)
			{
				const auto row = raw_rows[i];
				if (!row || !row->fields || !row->num_fields)
				{
					continue;
				}

				const std::string_view command = row->fields[0];
				if (!command.empty() && !command.starts_with('#') && !command.starts_with("//"))
				{
					rows.emplace_back(row);
				}
			}

			return rows;
		}

		csv::parser open_csv(const std::string& name)
		{
			csv::parser parser("zone_source\\" + name + ".csv", ',');
			if (!parser.valid())
			{
				throw std::runtime_error(std::format("Could not find csv file \"{}\"", name));
			}

			return parser;
		}

		// The asset from its dump, else from a required zone. nullptr when the asset has neither or is ignored.
		void* parse_asset(build_context& context, const std::int32_t type, const std::string& name)
		{
			const auto [entry, added] = context.parsed.try_emplace({type, name}, nullptr);
			if (!added || context.ignored.contains({type, name}))
			{
				return entry->second;
			}

			entry->second = context.parser.parse_file(type, name);
			if (const auto required = context.required.find({type, name}); !entry->second && required != context.required.end())
			{
				entry->second = required->second;
				for (auto& dependency : asset_dependencies(type, required->second))
				{
					context.required_dependencies.emplace_back(std::move(dependency));
				}
			}

			if (entry->second)
			{
				context.assets.emplace_back(type, entry->second, false);
			}

			return entry->second;
		}

		void add_reference(build_context& context, const std::int32_t type, const std::string& name)
		{
			context.ignored.emplace(type, name);
			context.list.emplace_back(type, context.parser.stub(type, name), true);
		}

		void add_named_asset(build_context& context, const std::int32_t type, const std::string& name, const bool referenced)
		{
			if (referenced || context.ignored.contains({type, name}))
			{
				add_reference(context, type, name);
				return;
			}

			if (const auto header = parse_asset(context, type, name))
			{
				context.list.emplace_back(type, header, false);
				return;
			}

			std::string paths;
			for (const auto& file : asset_file_locations(type, name))
			{
				paths += std::format("{}{}", paths.empty() ? "" : " or ", file.path);
			}

			ZONETOOL_WARNING("%s \"%s\" not found (%s), adding it as a reference", find_asset_format(type)->name, name.data(),
				paths.data());
			add_reference(context, type, name);
		}

		// localize,<file> adds the entries of localizedstrings/<file>.str or .json when there is such a file. An entry
		// keeps the first value of its name.
		bool add_localized_strings(build_context& context, const std::string& file_name)
		{
			const auto add = [&context](const std::string& entry_name, const std::string& value)
			{
				const auto name = localize_name(entry_name);
				const auto [entry, added] = context.parsed.try_emplace({ASSET_TYPE_LOCALIZE_ENTRY, name}, nullptr);
				if (!added && entry->second)
				{
					return;
				}

				auto& localize = *context.parser.allocate<LocalizeEntry>();
				localize.name = context.parser.duplicate(name);
				localize.value = context.parser.duplicate(value);
				entry->second = &localize;
				context.assets.emplace_back(ASSET_TYPE_LOCALIZE_ENTRY, &localize, false);
				context.list.emplace_back(ASSET_TYPE_LOCALIZE_ENTRY, &localize, false);
			};

			if (filesystem::file("localizedstrings/" + file_name + ".str").exists())
			{
				localized_strings::parse_str(file_name, add);
				return true;
			}

			if (filesystem::file("localizedstrings/" + file_name + ".json").exists())
			{
				localized_strings::parse_json(file_name, add);
				return true;
			}

			return false;
		}

		void add_asset(build_context& context, const csv::row* row)
		{
			const auto type = type_of(field(row, 0));
			if (type < 0)
			{
				ZONETOOL_ERROR("Unknown asset type \"%s\"", std::string(field(row, 0)).data());
				return;
			}

			// <pool>,<name> adds an asset, and <pool>,,<name> a reference to an asset of another zone.
			auto name = std::string(field(row, 1));
			if (type == ASSET_TYPE_LOCALIZE_ENTRY && !name.empty() && add_localized_strings(context, name))
			{
				return;
			}

			auto referenced = context.is_referencing;
			if (name.empty())
			{
				name = field(row, 2);
				referenced = true;
			}

			if (type == ASSET_TYPE_LOCALIZE_ENTRY)
			{
				name = localize_name(name);
			}

			add_named_asset(context, type, name, referenced);
		}

		// require,<zone> loads <zone>.ff from the zone folder. An asset without a dump comes from the required zones.
		void require_zone(build_context& context, const std::string_view name)
		{
			const auto path = context.zone_folder / std::filesystem::path(name).replace_extension(".ff");
			ZONETOOL_INFO("Loading required zone \"%s\"...", path.string().data());
			const auto& zone = context.required_zones.emplace_back(load_zone(path, {.keep_temp_data = true}));
			for (const auto& asset : zone->assets)
			{
				if (!asset.referenced)
				{
					context.required.try_emplace({asset.type, name_of(asset.type, asset.header)}, asset.header);
				}
			}
		}

		// ignore,<csv> makes references of the assets in zone_source/<csv>.csv.
		void ignore_assets(build_context& context, const std::string& name)
		{
			auto parser = open_csv(name);
			for (const auto row : read_csv_rows(parser))
			{
				const auto type = type_of(field(row, 0));
				const auto asset = std::string(field(row, 1));
				if (type >= 0 && !asset.empty())
				{
					context.ignored.emplace(type, type == ASSET_TYPE_LOCALIZE_ENTRY ? localize_name(asset) : asset);
				}
			}
		}

		// iterate,<pool> adds each .iw8 dump in zonetool/<zone>/<pool>/, and iterate,true the dumps of each pool.
		void iterate_assets(build_context& context, const std::string_view pool)
		{
			for (std::int32_t type = 0; type < ASSET_TYPE_COUNT; type++)
			{
				const auto format = find_asset_format(type);
				if (!format || (pool != "true" && format->name != pool))
				{
					continue;
				}

				const auto folder = std::filesystem::path("zonetool") / context.fastfile / format->name;
				if (!std::filesystem::is_directory(folder))
				{
					continue;
				}

				for (const auto& file : std::filesystem::recursive_directory_iterator(folder))
				{
					if (file.is_regular_file() && file.path().extension() == ".iw8" && !file.path().filename().string().starts_with(','))
					{
						add_named_asset(context, type, std::filesystem::relative(file.path(), folder).replace_extension().generic_string(),
							context.is_referencing);
					}
				}
			}
		}

		void parse_csv(build_context& context, const std::string& name)
		{
			auto parser = open_csv(name);
			for (const auto row : read_csv_rows(parser))
			{
				const auto command = field(row, 0);
				const auto argument = field(row, 1);
				if (command == "require")
				{
					require_zone(context, argument);
				}
				else if (command == "include")
				{
					parse_csv(context, std::string(argument));
				}
				else if (command == "ignore")
				{
					ignore_assets(context, std::string(argument));
				}
				else if (command == "reference")
				{
					context.is_referencing = argument == "true";
				}
				else if (command == "iterate")
				{
					iterate_assets(context, argument);
				}
				else if (command == "addpath")
				{
					filesystem::add_path(std::string(argument), field(row, 2) == "true");
				}
				else if (command == "addpaths")
				{
					filesystem::add_paths_from_directory(std::string(argument), field(row, 2) == "true");
				}
				else
				{
					add_asset(context, row);
				}
			}
		}

		// The assets that the zone assets name and that have a dump or a required zone: the zone holds them too.
		void parse_dependencies(build_context& context)
		{
			std::size_t dumped{};
			std::size_t required{};
			while (dumped < context.parser.references().size() || required < context.required_dependencies.size())
			{
				const auto [type, name] = dumped < context.parser.references().size() ? context.parser.references()[dumped++]
					: context.required_dependencies[required++];
				parse_asset(context, type, name);
			}
		}
	}

	void build_zone(const std::filesystem::path& zone_folder, const std::string& name)
	{
		const auto _ = gsl::finally(db_clear);

		filesystem::set_zone_folder(zone_folder);
		filesystem::set_fastfile(name);
		ZONETOOL_INFO("Building fastfile \"%s\"", name.data());

		build_context context{.zone_folder = zone_folder, .fastfile = name};
		const auto search_paths = filesystem::get_search_paths();
		parse_csv(context, name);
		parse_dependencies(context);
		filesystem::get_search_paths() = search_paths;

		const std::array<const char*, 1> strings{nullptr};
		const auto written = write_zone(context.list, context.assets, strings);

		xfile::fastfile file{};
		file.payload = written.data;
		file.block_sizes = written.block_sizes;
		file.transient = name.ends_with("_tr");
		file.preload_walk_size = written.preload_walk_size;

		const auto path = std::filesystem::path(filesystem::get_output_path()) / (name + ".ff");
		xfile::write(path, definition().format, file);
		ZONETOOL_INFO("Zone \"%s\" written to %s: %zu assets, 0x%zX bytes", name.data(), path.string().data(),
			context.list.size(), written.data.size());
	}
}
