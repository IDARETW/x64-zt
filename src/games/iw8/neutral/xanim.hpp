#pragma once

#include <core/assets/provenance.hpp>
#include <core/assets/xanim_data.hpp>
#include "../structs.hpp"

#include <expected>
#include <string>

namespace zonetool
{
	class zone_memory;
}

// Replay 1.20 animations to and from neutral data (docs/NEUTRAL_ASSETS.md, family F5). to_neutral decodes the shared
// byte, short, int and index streams into the channels of each bone; for streams that do not follow that encoding it
// gives the reason, and the native record stays. The asset, IK and finger pose types and the flags without a neutral
// name go to the provenance, which from_neutral restores for a source of the same profile.
namespace zonetool::iw8::neutral
{
	inline constexpr auto xanim_profile = "iw8 replay 1.20";

	struct xanim_source
	{
		asset_data::xanim xanim;
		asset_data::provenance provenance;

		bool operator==(const xanim_source&) const = default;
	};

	std::expected<xanim_source, std::string> to_neutral(const XAnimParts& native);
	XAnimParts* from_neutral(const asset_data::xanim& data, const asset_data::provenance* source, zone_memory& memory);
	std::size_t native_size(const asset_data::xanim& data);

	inline XAnimParts* from_neutral(const xanim_source& source, zone_memory& memory)
	{
		return from_neutral(source.xanim, &source.provenance, memory);
	}

	inline std::size_t native_size(const xanim_source& source)
	{
		return native_size(source.xanim);
	}
}
