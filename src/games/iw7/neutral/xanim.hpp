#pragma once

#include <core/assets/provenance.hpp>
#include <core/assets/xanim_data.hpp>
#include "../structs.hpp"

#include <expected>
#include <string>

// IW7 animations to neutral data (docs/NEUTRAL_ASSETS.md, family F5). The bone streams decode with the codec that IW7
// and Replay share; a record that does not decode gives its reason. The asset type, the two bytes after it and the flags
// without a neutral name (IW7's default and shmem bits) go to the provenance.
namespace zonetool::iw7::neutral
{
	inline constexpr auto xanim_profile = "iw7";

	struct xanim_source
	{
		asset_data::xanim xanim;
		asset_data::provenance provenance;

		bool operator==(const xanim_source&) const = default;
	};

	std::expected<xanim_source, std::string> to_neutral(const XAnimParts& native);
}
