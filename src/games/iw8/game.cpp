#include <std_include.hpp>

#include "iw8.hpp"
#include "zone/loader.hpp"

#include <game.hpp>

namespace zonetool::iw8
{
	namespace
	{
		void dump_zone(const std::filesystem::path&, const std::filesystem::path&, const asset_dumper)
		{
			throw std::runtime_error("iw8 zones cannot be dumped yet");
		}

		void verify_zone(const std::filesystem::path&, const std::filesystem::path& zone_path)
		{
			const auto _ = gsl::finally(db_clear);

			const auto zone = load_zone(zone_path);
			std::map<std::string, std::size_t> counts;
			for (const auto& asset : zone->assets)
			{
				const auto format = find_asset_format(asset.type);
				++counts[format ? format->name : std::to_string(asset.type)];
			}

			ZONETOOL_INFO("%s: %zu assets, %zu script strings, all bytes read", zone->name.data(), zone->assets.size(),
				zone->script_string_count);
			for (const auto& [name, count] : counts)
			{
				ZONETOOL_INFO("  %s: %zu", name.data(), count);
			}
		}

		void build_zone(const std::filesystem::path&, const std::string&)
		{
			throw std::runtime_error("iw8 zones build only through a converter, for example: -game iw3 dumpzone <zone> --target iw8");
		}
	}

	const game& definition()
	{
		static const game iw8
		{
			.name = "iw8",
			.display_name = "Call of Duty: Modern Warfare (Replay 1.20)",
			.format =
			{
				.container = xfile::container::iw8,
				.unsigned_magic = "IWffc100",
				.signed_magic = "IWffa100",
				.version = 11,
				.block_count = 11,
			},
			.dump_zone = dump_zone,
			.verify_zone = verify_zone,
			.build_zone = build_zone,
		};

		return iw8;
	}
}
