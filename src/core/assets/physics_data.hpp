#pragma once

#include "asset_data.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

// Neutral physics payload assets (families F4 and F7 of docs/NEUTRAL_ASSETS.md): the shape of a physics effect and the
// detail collision of a model. The shapes are Havok tagfiles; the neutral data keeps the file and the SDK version it names.
namespace zonetool::asset_data
{
// A Havok binary tagfile: a chunk "TAG0" whose "SDKV" chunk names the SDK that wrote it, e.g. "20180200". The bytes are
// the file; a writer for another SDK version has to convert the shape, so the version travels with them.
struct havok_tagfile
{
	std::string sdk_version;
	std::vector<std::uint8_t> bytes;

	bool operator==(const havok_tagfile&) const = default;
};

// The SDK version that a tagfile's SDKV chunk names, or an empty string when the bytes are not a tagfile.
inline std::string havok_tagfile_version(const std::vector<std::uint8_t>& bytes)
{
	// A chunk is a big-endian size (with flags in the top bits) and a four-character tag; the first chunk is TAG0.
	if (bytes.size() < 16 || std::string_view(reinterpret_cast<const char*>(bytes.data()) + 4, 4) != "TAG0")
	{
		return {};
	}

	// The first child of TAG0 is the SDKV chunk: its tag at byte 12 of the file, then the version text.
	if (std::string_view(reinterpret_cast<const char*>(bytes.data()) + 12, 4) != "SDKV" || bytes.size() < 24)
	{
		return {};
	}

	return std::string(reinterpret_cast<const char*>(bytes.data()) + 16, 8);
}

// The shape of a physics effect and the sound it makes when it hits: the impulse range maps to the volume range.
struct physics_fx_shape
{
	std::string name;
	havok_tagfile shape;
	std::optional<std::string> sound_alias;
	float sound_min_impulse{};
	float sound_max_impulse{};
	float sound_min_impulse_volume{};
	float sound_max_impulse_volume{};

	bool operator==(const physics_fx_shape&) const = default;
};

// The collision of the details of a model (its LOD shapes) and the names of its parts, in the order of the shape.
struct detail_collision
{
	std::string name;
	havok_tagfile shape;
	std::vector<std::string> part_names;

	bool operator==(const detail_collision&) const = default;
};

inline void validate(const havok_tagfile& data, const std::string& what)
{
	if (data.bytes.empty())
	{
		if (!data.sdk_version.empty())
		{
			throw std::runtime_error(what + " names an SDK version for no tagfile");
		}

		return;
	}

	if (havok_tagfile_version(data.bytes) != data.sdk_version || data.sdk_version.empty())
	{
		throw std::runtime_error(what + " holds bytes that are not a tagfile of the SDK version it names");
	}

	if (data.bytes.size() > 0xFFFFFFFFull)
	{
		throw std::runtime_error(what + " holds a tagfile larger than 4 GiB");
	}
}

inline void validate(const physics_fx_shape& data)
{
	detail::name(data.name, "physicsfxshape");
	validate(data.shape, "physicsfxshape " + data.name);
	if (data.sound_alias)
	{
		detail::name(*data.sound_alias, "physicsfxshape sound alias");
	}

	if (!std::isfinite(data.sound_min_impulse) || !std::isfinite(data.sound_max_impulse) ||
		!std::isfinite(data.sound_min_impulse_volume) || !std::isfinite(data.sound_max_impulse_volume))
	{
		throw std::runtime_error("physicsfxshape " + data.name + " has a sound range that is not finite");
	}
}

inline void validate(const detail_collision& data)
{
	detail::name(data.name, "xmodeldetailcollision");
	validate(data.shape, "xmodeldetailcollision " + data.name);
	if (data.part_names.size() > 0xFFFFFFFFull)
	{
		throw std::runtime_error("xmodeldetailcollision " + data.name + " has more part names than a count holds");
	}

	for (const auto& part : data.part_names)
	{
		if (part.empty())
		{
			throw std::runtime_error("xmodeldetailcollision " + data.name + " has an empty part name");
		}
	}
}
} // namespace zonetool::asset_data
