#pragma once

#include <core/assets/asset_data.hpp>
#include <cstdint>
#include <optional>
#include <stdexcept>
#include <string>

// Neutral light definitions (family F7) and colour grading tables (family F6) of docs/NEUTRAL_ASSETS.md. A reference
// names its asset by definition name; whether the zone holds it or another zone does is the writer's concern.
namespace zonetool::asset_data
{
// A light definition: the IES profile that shapes the light's intensity over its angle, or none.
struct light_def
{
	std::string name;
	std::optional<std::string> ies_profile;

	bool operator==(const light_def&) const = default;
};

// A colour grading lookup table of size^3 entries, held by an image.
struct grading_clut
{
	std::string name;
	std::uint32_t size{};
	std::optional<std::string> image;

	bool operator==(const grading_clut&) const = default;
};

inline void validate(const light_def& data)
{
	detail::name(data.name, "lightdef");
	if (data.ies_profile && data.ies_profile->empty())
	{
		throw std::runtime_error("lightdef " + data.name + " names an empty IES profile");
	}
}

inline void validate(const grading_clut& data)
{
	detail::name(data.name, "gradingclut");
	if (data.image)
	{
		detail::name(*data.image, "gradingclut image");
	}
}
} // namespace zonetool::asset_data
