#pragma once

#include <core/assets/lighting_data.hpp>
#include <core/assets/provenance.hpp>
#include "../structs.hpp"

#include <functional>

namespace zonetool
{
	class zone_memory;
}

// Replay 1.20 light definitions and grading tables to and from neutral data (docs/NEUTRAL_ASSETS.md). The place of a
// light's profile in Replay's IES lookup strip (firstPixel, coordOffset, coordScale) and a table's colorimetries go to
// the provenance: from_neutral restores them for a source of the same profile and rejects a light with a profile and
// no strip place.
namespace zonetool::iw8::neutral
{
	inline constexpr auto lighting_profile = "iw8 replay 1.20";

	// The header of the asset of a type and definition name that a rebuilt asset points to, or nullptr.
	using asset_resolver = std::function<void*(std::int32_t type, const std::string& name)>;

	struct light_def_source
	{
		asset_data::light_def light_def;
		asset_data::provenance provenance;

		bool operator==(const light_def_source&) const = default;
	};

	struct grading_clut_source
	{
		asset_data::grading_clut clut;
		asset_data::provenance provenance;

		bool operator==(const grading_clut_source&) const = default;
	};

	light_def_source to_neutral(const GfxLightDef& native);
	grading_clut_source to_neutral(const GfxGradingClut& native);

	GfxLightDef* from_neutral(const asset_data::light_def& data, const asset_data::provenance* source, zone_memory& memory);
	GfxGradingClut* from_neutral(const asset_data::grading_clut& data, const asset_data::provenance* source,
		const asset_resolver& resolve, zone_memory& memory);

	std::size_t native_size(const asset_data::light_def& data);
	std::size_t native_size(const asset_data::grading_clut& data);

	inline GfxLightDef* from_neutral(const light_def_source& source, zone_memory& memory)
	{
		return from_neutral(source.light_def, &source.provenance, memory);
	}

	inline GfxGradingClut* from_neutral(const grading_clut_source& source, const asset_resolver& resolve, zone_memory& memory)
	{
		return from_neutral(source.clut, &source.provenance, resolve, memory);
	}

	inline std::size_t native_size(const light_def_source& source)
	{
		return native_size(source.light_def);
	}

	inline std::size_t native_size(const grading_clut_source& source)
	{
		return native_size(source.clut);
	}
}
