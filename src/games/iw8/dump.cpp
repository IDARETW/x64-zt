#include <std_include.hpp>
#include "iw8.hpp"
#include "zone/asset_files.hpp"
#include "zone/loader.hpp"
#include "zone/writer.hpp"

#include <game.hpp>

namespace zonetool::iw8
{
	const game& definition();

	namespace
	{
		using asset_key = std::pair<std::int32_t, std::string>;

		std::string asset_name(const std::int32_t type, const void* header)
		{
			const auto name = get_asset_name(type, header);
			return name ? name : "";
		}

		std::vector<zone_asset> to_zone_assets(const std::vector<loaded_asset>& assets)
		{
			std::vector<zone_asset> result;
			result.reserve(assets.size());
			for (const auto& asset : assets)
			{
				result.emplace_back(asset.type, asset.header, asset.referenced, asset.marker);
			}

			return result;
		}

		// The assets of a zone by type and name, with the references to assets that the zone does not hold. A zone can
		// hold an asset twice, and a writer can write it once: a reference to an asset of the zone uses that asset.
		std::set<std::tuple<std::int32_t, std::string, bool>> distinct_assets(const loaded_zone& zone)
		{
			std::set<std::pair<std::int32_t, std::string>> held;
			for (const auto& asset : zone.assets)
			{
				const auto name = get_asset_name(asset.type, asset.header);
				if (!asset.referenced)
				{
					held.emplace(asset.type, name ? name : "");
				}
			}

			std::set<std::tuple<std::int32_t, std::string, bool>> assets;
			for (const auto& asset : zone.assets)
			{
				const std::string name = get_asset_name(asset.type, asset.header) ? get_asset_name(asset.type, asset.header) : "";
				assets.emplace(asset.type, name, asset.referenced && !held.contains({asset.type, name}));
			}

			return assets;
		}

		// IW8_REWRITE_OUT=<folder> also writes the zone there, to load it with the game loaders.
		void save_written_zone(const loaded_zone& zone, const written_zone& written)
		{
			char output[MAX_PATH]{};
			if (!GetEnvironmentVariableA("IW8_REWRITE_OUT", output, sizeof(output)))
			{
				return;
			}

			xfile::fastfile file{};
			file.payload = written.data;
			file.block_sizes = written.block_sizes;
			file.xfile_version = zone.file.xfile_version;
			file.transient = zone.file.transient;
			file.preload_walk_size = std::max(written.preload_walk_size, zone.file.preload_walk_size);
			xfile::write(std::filesystem::path(output) / (zone.name + ".ff"), definition().format, file);
		}

		// IW8_REWRITE=fresh writes the zone with the choices of the writer, loads the result and compares the assets.
		void verify_fresh_write(const loaded_zone& zone)
		{
			const auto list = to_zone_assets(zone.list);
			const auto assets = to_zone_assets(zone.assets);
			const auto written = write_zone(list, assets, zone.script_strings);
			save_written_zone(zone, written);

			xfile::fastfile file{};
			file.payload = written.data;
			file.block_sizes = written.block_sizes;
			const auto reloaded = load_zone(zone.name, std::move(file));

			const auto written_assets = distinct_assets(*reloaded);
			const auto zone_assets = distinct_assets(zone);
			if (written_assets != zone_assets || reloaded->list.size() != zone.list.size())
			{
				throw std::runtime_error(std::format("the written zone holds other assets: {} of {} assets, {} of {} list entries",
					written_assets.size(), zone_assets.size(), reloaded->list.size(), zone.list.size()));
			}

			ZONETOOL_INFO("%s: the written zone loads with the same assets (0x%zX of 0x%zX bytes)", zone.name.data(),
				written.data.size(), zone.file.payload.size());
		}

		std::map<asset_key, asset_file> dump_assets(const loaded_zone& zone)
		{
			std::map<asset_key, asset_file> dumps;
			for (const auto& asset : zone.assets)
			{
				const asset_key key{asset.type, asset_name(asset.type, asset.header)};
				if (!asset.referenced && !dumps.contains(key))
				{
					dumps.emplace(key, dump_asset_file(asset.type, asset.header));
				}
			}

			return dumps;
		}

		// IW8_REWRITE=dump dumps each asset, parses the dumps, writes them as a zone, loads it and dumps it again.
		void verify_dump_round_trip(const loaded_zone& zone)
		{
			const auto dumps = dump_assets(zone);

			asset_parser parser;
			std::map<asset_key, void*> parsed;
			std::vector<zone_asset> assets;
			std::size_t shared{};
			for (const auto& [key, file] : dumps)
			{
				const auto header = parser.parse_asset_file(key.first, key.second, file);
				parsed.emplace(key, header);
				assets.emplace_back(key.first, header, false);
				shared += file.shared;

				// An asset of a type with a shared file that the shared file does not hold.
				if (!file.shared && asset_file_locations(key.first, key.second).size() > 1)
				{
					ZONETOOL_INFO("%s: %s", zone.name.data(), file.path.data());
				}
			}

			std::vector<zone_asset> list;
			for (const auto& entry : zone.list)
			{
				const asset_key key{entry.type, asset_name(entry.type, entry.header)};
				const auto found = parsed.find(key);
				list.emplace_back(entry.type, found != parsed.end() ? found->second : parser.stub(entry.type, key.second),
					found == parsed.end());
			}

			const std::array<const char*, 1> strings{nullptr};
			const auto written = write_zone(list, assets, strings);

			xfile::fastfile fastfile{};
			fastfile.payload = written.data;
			fastfile.block_sizes = written.block_sizes;
			const auto reloaded = load_zone(zone.name, std::move(fastfile), {.keep_temp_data = true});
			if (distinct_assets(*reloaded) != distinct_assets(zone))
			{
				throw std::runtime_error("the zone that the dumps make holds other assets");
			}

			const auto again = dump_assets(*reloaded);
			for (const auto& [key, file] : dumps)
			{
				const auto found = again.find(key);
				if (found == again.end() || found->second.path != file.path || found->second.data != file.data)
				{
					char debug[MAX_PATH]{};
					if (GetEnvironmentVariableA("IW8_DUMP_DEBUG", debug, sizeof(debug)))
					{
						std::ofstream(std::filesystem::path(debug) / "first", std::ios::binary).write(file.data.data(), static_cast<std::streamsize>(file.data.size()));
						if (found != again.end())
						{
							std::ofstream(std::filesystem::path(debug) / "again", std::ios::binary).write(found->second.data.data(), static_cast<std::streamsize>(found->second.data.size()));
						}
					}

					throw std::runtime_error(std::format("{} dumps differently from the zone that its dump makes", file.path));
				}
			}

			ZONETOOL_INFO("%s: %zu assets (%zu in shared files) dump, parse and write again to the same files", zone.name.data(),
				dumps.size(), shared);
		}

		// IW8_REWRITE=1 writes the zone again from the loaded assets and compares it with the zone data.
		void verify_rewrite(const loaded_zone& zone)
		{
			const auto list = to_zone_assets(zone.list);
			const auto assets = to_zone_assets(zone.assets);
			const auto written = write_zone(list, assets, zone.script_strings, {.markers = &zone.markers,
				.strings_marker = zone.strings_marker, .assets_marker = zone.assets_marker});

			save_written_zone(zone, written);

			const auto& original = zone.file.payload;
			const auto size = std::min(original.size(), written.data.size());
			const auto difference = std::mismatch(original.begin(), original.begin() + size, written.data.begin());
			const auto offset = static_cast<std::size_t>(difference.first - original.begin());

			std::string blocks;
			for (std::size_t i = 0; i < written.block_sizes.size(); i++)
			{
				if (written.block_sizes[i] != zone.file.block_sizes[i])
				{
					blocks += std::format(" {}:0x{:X}/0x{:X}", i, written.block_sizes[i], zone.file.block_sizes[i]);
				}
			}

			if (offset == size && original.size() == written.data.size())
			{
				// Stock block sizes add slack to the peaks that the writer computes (decal volume materials).
				ZONETOOL_INFO("%s: the written zone data is equal to the zone data; block sizes (written/zone)%s",
					zone.name.data(), blocks.empty() ? " equal" : blocks.data());
				return;
			}

			const auto bytes_at = [offset](const std::string& data)
			{
				std::string text;
				for (auto i = offset & ~std::size_t(15); i < std::min(data.size(), (offset & ~std::size_t(15)) + 32); i++)
				{
					text += std::format("{:02X}", static_cast<std::uint8_t>(data[i]));
				}

				return text;
			};

			std::string asset = "the asset list";
			for (const auto& [start, type, name] : written.asset_offsets)
			{
				if (start > offset)
				{
					break;
				}

				const auto format = find_asset_format(type);
				asset = std::format("{} \"{}\" at 0x{:X}", format ? format->name : "?", name, start);
			}

			throw std::runtime_error(std::format("the written zone differs: 0x{:X} bytes, zone data 0x{:X} bytes, first "
				"difference at 0x{:X} in {} (written {} zone {}); block sizes (written/zone){}", written.data.size(),
				original.size(), offset, asset, bytes_at(written.data), bytes_at(original), blocks.empty() ? " equal" : blocks));
		}

		// The asset list of the zone: <pool>,<name> for an asset, <pool>,,<name> for a reference.
		void write_asset_csv(const loaded_zone& zone)
		{
			std::string csv;
			for (const auto& entry : zone.list)
			{
				const auto format = find_asset_format(entry.type);
				csv += std::format("{},{}{}\n", format ? format->name : std::to_string(entry.type), entry.referenced ? "," : "",
					asset_name(entry.type, entry.header));
			}

			filesystem::file file(zone.name + ".csv");
			file.open("wb");
			file.write(csv.data(), csv.size(), 1);
			file.close();
		}
	}

	void dump_zone(const std::filesystem::path& zone_folder, const std::filesystem::path& zone_path, const asset_dumper converter)
	{
		const auto _ = gsl::finally(db_clear);

		filesystem::set_zone_folder(zone_folder);
		filesystem::set_fastfile(zone_path.stem().string());
		ZONETOOL_INFO("Loading zone \"%s\"...", zone_path.stem().string().data());
		const auto zone = load_zone(zone_path, {.keep_temp_data = true});

		std::set<asset_key> dumped;
		for (const auto& asset : zone->assets)
		{
			const asset_key key{asset.type, asset_name(asset.type, asset.header)};
			if (asset.referenced || !dumped.emplace(key).second)
			{
				continue;
			}

			try
			{
				if (converter)
				{
					converter(asset.type, asset.header);
				}
				else
				{
					dump_asset(asset.type, asset.header);
				}
			}
			catch (const std::exception& e)
			{
				const auto format = find_asset_format(asset.type);
				ZONETOOL_ERROR("Failed to dump %s \"%s\": %s", format ? format->name : "?", key.second.data(), e.what());
			}
		}

		write_asset_csv(*zone);
		ZONETOOL_INFO("Zone \"%s\" dumped to %s", zone->name.data(), filesystem::get_dump_path().data());
	}

	void verify_zone(const std::filesystem::path&, const std::filesystem::path& zone_path)
	{
		const auto _ = gsl::finally(db_clear);

		char rewrite[8]{};
		const auto check_rewrite = GetEnvironmentVariableA("IW8_REWRITE", rewrite, sizeof(rewrite)) != 0;
		const auto zone = load_zone(zone_path, {.keep_temp_data = check_rewrite, .keep_markers = check_rewrite});
		std::map<std::string, std::size_t> counts;
		for (const auto& asset : zone->assets)
		{
			const auto format = find_asset_format(asset.type);
			++counts[format ? format->name : std::to_string(asset.type)];
		}

		ZONETOOL_INFO("%s: %zu assets, %zu script strings, all bytes read", zone->name.data(), zone->assets.size(),
			zone->script_strings.size());
		for (const auto& [name, count] : counts)
		{
			ZONETOOL_INFO("  %s: %zu", name.data(), count);
		}

		if (check_rewrite)
		{
			if (std::string_view(rewrite) == "fresh")
			{
				verify_fresh_write(*zone);
			}
			else if (std::string_view(rewrite) == "dump")
			{
				verify_dump_round_trip(*zone);
			}
			else
			{
				verify_rewrite(*zone);
			}
		}
	}
}
