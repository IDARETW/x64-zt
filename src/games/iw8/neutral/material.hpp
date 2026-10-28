#pragma once

#include <core/assets/material_data.hpp>
#include <core/assets/provenance.hpp>
#include "resolver.hpp"
#include "../structs.hpp"

#include <expected>
#include <string>

namespace zonetool
{
	class zone_memory;
}

// Replay 1.20 materials to and from neutral data (docs/NEUTRAL_ASSETS.md, family F3). The decal volume material's flags
// and its two packed fields (displacement scale and bias, temperature base and scale) have no mapped meaning: they go
// to the provenance, which from_neutral restores for a source of the same profile and otherwise rejects.
namespace zonetool::iw8::neutral
{
	inline constexpr auto material_profile = "iw8 replay 1.20";

	struct decal_volume_material_source
	{
		asset_data::decal_volume_material material;
		asset_data::provenance provenance;

		bool operator==(const decal_volume_material_source&) const = default;
	};

	std::expected<decal_volume_material_source, std::string> to_neutral(const GfxDecalVolumeMaterial& native);
	GfxDecalVolumeMaterial* from_neutral(const asset_data::decal_volume_material& data, const asset_data::provenance* source,
		const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::decal_volume_material& data);

	inline GfxDecalVolumeMaterial* from_neutral(const decal_volume_material_source& source, const asset_resolver& resolve,
		zone_memory& memory)
	{
		return from_neutral(source.material, &source.provenance, resolve, memory);
	}

	inline std::size_t native_size(const decal_volume_material_source& source)
	{
		return native_size(source.material);
	}
}
