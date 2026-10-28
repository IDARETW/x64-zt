#pragma once

#include <core/assets/provenance.hpp>
#include <core/assets/stream_data.hpp>
#include "../structs.hpp"

#include <expected>
#include <string>

namespace zonetool
{
	class zone_memory;
}

// Replay 1.20 stream keys to and from neutral data (docs/NEUTRAL_ASSETS.md, family F8). The XPak entry, the flags, the
// behavior's user value or asset hash and the size of a payload outside the zone go to the provenance, which from_neutral
// restores for a source of the same profile and otherwise rejects.
namespace zonetool::iw8::neutral
{
	inline constexpr auto stream_profile = "iw8 replay 1.20";

	struct stream_key_source
	{
		asset_data::stream_key key;
		asset_data::provenance provenance;

		bool operator==(const stream_key_source&) const = default;
	};

	std::expected<stream_key_source, std::string> to_neutral(const StreamKey& native);
	StreamKey* from_neutral(const asset_data::stream_key& data, const asset_data::provenance* source, zone_memory& memory);
	std::size_t native_size(const asset_data::stream_key& data);

	inline StreamKey* from_neutral(const stream_key_source& source, zone_memory& memory)
	{
		return from_neutral(source.key, &source.provenance, memory);
	}

	inline std::size_t native_size(const stream_key_source& source)
	{
		return native_size(source.key);
	}
}
