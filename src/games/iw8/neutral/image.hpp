#pragma once

#include <core/assets/image_data.hpp>
#include <core/assets/provenance.hpp>
#include "../structs.hpp"

#include <optional>

namespace zonetool
{
	class zone_memory;
}

// Replay 1.20 images to and from neutral data (docs/NEUTRAL_ASSETS.md, family F2). Resident images convert; to_neutral
// returns none for an image whose texels the zone does not hold (streamed, packed atlas, fallback, Oodle compressed)
// or whose layout it does not know. The Replay fields without a neutral meaning go to the provenance, which
// from_neutral restores for a source of the same profile.
namespace zonetool::iw8::neutral
{
	inline constexpr auto image_profile = "iw8 replay 1.20";

	struct image_source
	{
		asset_data::image image;
		asset_data::provenance provenance;

		bool operator==(const image_source&) const = default;
	};

	std::optional<image_source> to_neutral(const GfxImage& native);
	GfxImage* from_neutral(const asset_data::image& data, const asset_data::provenance* source, zone_memory& memory);
	std::size_t native_size(const asset_data::image& data);

	inline GfxImage* from_neutral(const image_source& source, zone_memory& memory)
	{
		return from_neutral(source.image, &source.provenance, memory);
	}

	inline std::size_t native_size(const image_source& source)
	{
		return native_size(source.image);
	}
}
