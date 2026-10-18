#include <std_include.hpp>

#include <game.hpp>

namespace zonetool::convert::iw7_iw8
{
	void convert_zone(const std::filesystem::path& zone_path, const zone_conversion& conversion);

	const converter& definition()
	{
		static const converter iw7_iw8
		{
			.source = "iw7",
			.target = "iw8",
			.convert_zone = convert_zone,
		};

		return iw7_iw8;
	}
}
