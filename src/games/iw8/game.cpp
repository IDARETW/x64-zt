#include <std_include.hpp>
#include "iw8.hpp"

#include <game.hpp>

namespace zonetool::iw8
{
	void dump_zone(const std::filesystem::path& zone_folder, const std::filesystem::path& zone_path, const converter* converter);
	void dump_asset(const std::filesystem::path& zone_folder, const std::filesystem::path& zone_path, const std::string& type,
		const std::string& name, const converter* converter);
	void verify_zone(const std::filesystem::path& zone_folder, const std::filesystem::path& zone_path);
	void build_zone(const std::filesystem::path& zone_folder, const std::string& name);

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
			.dump_asset = dump_asset,
			.verify_zone = verify_zone,
			.build_zone = build_zone,
		};

		return iw8;
	}
}
