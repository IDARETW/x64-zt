#pragma once

#include "asset_data.hpp"
#include <algorithm>
#include <cstdint>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

// Neutral stream keys (family F8 of docs/NEUTRAL_ASSETS.md): the key under which a sound, a terrain, a stream tree or a
// clutter bitmask is streamed, and its bytes when the zone holds them. An absent payload and a present empty payload stay
// distinct: an absent one is streamed from a pack outside the zone.
namespace zonetool::asset_data
{
struct stream_key
{
	std::string name;
	// none, terrain, sound, streamtree or clutter_bitmask.
	std::string behavior;
	std::optional<std::vector<std::uint8_t>> payload;

	bool operator==(const stream_key&) const = default;
};

inline constexpr const char* stream_key_behaviors[] = {"none", "terrain", "sound", "streamtree", "clutter_bitmask"};

inline void validate(const stream_key& data)
{
	detail::name(data.name, "streamkey");
	if (std::ranges::find(stream_key_behaviors, data.behavior) == std::end(stream_key_behaviors))
	{
		throw std::runtime_error("streamkey " + data.name + " has an unknown behavior: " + data.behavior);
	}
}
} // namespace zonetool::asset_data
