#include <std_include.hpp>

#include <game.hpp>

namespace zonetool::convert::iw3_iw8
{
	void convert_zone(const std::filesystem::path& zone_path, const zone_conversion& conversion);

	const converter& definition()
	{
		static const converter iw3_iw8
		{
			.source = "iw3",
			.target = "iw8",
			.convert_zone = convert_zone,
		};

		return iw3_iw8;
	}
}
