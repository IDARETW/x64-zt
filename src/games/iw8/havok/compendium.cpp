#include <std_include.hpp>
#include "compendium.hpp"
#include "../iw8.hpp"
#include "../zone/loader.hpp"

namespace zonetool::iw8::havok
{
	const type_table& replay_compendium(const std::filesystem::path& zone_folder)
	{
		static const auto compendium = [&zone_folder]
		{
			const auto zone = load_zone(zone_folder / "code_post_gfx.ff");
			for (const auto& asset : zone->assets)
			{
				const auto& library = *static_cast<const PhysicsLibrary*>(asset.header);
				if (asset.type == ASSET_TYPE_PHYSICSLIBRARY && !asset.referenced && library.isGlobalTypeCompendium)
				{
					return type_table::read({reinterpret_cast<const std::uint8_t*>(library.havokData), library.havokDataSize});
				}
			}

			throw std::runtime_error("code_post_gfx has no Havok type compendium");
		}();

		return compendium;
	}
}
