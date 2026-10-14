#include <std_include.hpp>

#include <game.hpp>

namespace zonetool::iw3
{
	namespace
	{
		void dump_zone(const std::filesystem::path&, const std::filesystem::path&, const asset_dumper)
		{
			throw std::runtime_error("iw3 zones are read through OAT Unlinker: use dumpzone <zone> --target iw8");
		}

		void verify_zone(const std::filesystem::path&, const std::filesystem::path&)
		{
			throw std::runtime_error("iw3 zones cannot be verified");
		}

		void build_zone(const std::filesystem::path&, const std::string&)
		{
			throw std::runtime_error("iw3 zones cannot be built");
		}
	}

	const game& definition()
	{
		static const game iw3
		{
			.name = "iw3",
			.display_name = "Call of Duty 4: Modern Warfare",
			.steam_folder = "Call of Duty 4",
			.format =
			{
				.container = xfile::container::unsupported,
				.unsigned_magic = "IWffu100",
				.version = 5,
				.block_count = 9,
			},
			.dump_zone = dump_zone,
			.verify_zone = verify_zone,
			.build_zone = build_zone,
		};

		return iw3;
	}
}
