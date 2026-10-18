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
			asset_parser parser;
			std::vector<zone_asset> list;
			std::vector<zone_asset> assets;
			std::map<asset_key, void*> parsed;
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

		// Parses the dump of an asset once: nullptr when the asset has no dump.
		void* parse_asset(build_context& context, const std::int32_t type, const std::string& name)
		{
			const auto [entry, added] = context.parsed.try_emplace({type, name}, nullptr);
			if (added)
			{
				entry->second = context.parser.parse_file(type, name);
				if (entry->second)
				{
					context.assets.emplace_back(type, entry->second, false);
				}
			}

			return entry->second;
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

			if (referenced)
			{
				context.list.emplace_back(type, context.parser.stub(type, name), true);
				return;
			}

			const auto header = parse_asset(context, type, name);
			if (!header)
			{
				std::string paths;
				for (const auto& file : asset_file_locations(type, name))
				{
					paths += std::format("{}{}", paths.empty() ? "" : " or ", file.path);
				}

				throw std::runtime_error(std::format("there is no dump of {} \"{}\" ({})", field(row, 0), name, paths));
			}

			context.list.emplace_back(type, header, false);
		}

		void parse_csv(build_context& context, const std::string& name)
		{
			csv::parser parser("zone_source\\" + name + ".csv", ',');
			if (!parser.valid())
			{
				throw std::runtime_error(std::format("Could not find csv file \"{}\" to build zone", name));
			}

			for (const auto row : read_csv_rows(parser))
			{
				const auto command = field(row, 0);
				const auto argument = field(row, 1);
				if (command == "include")
				{
					parse_csv(context, std::string(argument));
				}
				else if (command == "reference")
				{
					context.is_referencing = argument == "true";
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

		// The assets that the dumps name and that have a dump of their own: the zone holds them too.
		void parse_dependencies(build_context& context)
		{
			for (std::size_t i = 0; i < context.parser.references().size(); i++)
			{
				const auto [type, name] = context.parser.references()[i];
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

		build_context context;
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
