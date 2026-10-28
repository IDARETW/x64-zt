#pragma once

#include <core/assets/fx_data.hpp>
#include <core/assets/provenance.hpp>
#include "../structs.hpp"

#include <expected>
#include <string>

namespace zonetool
{
	class zone_memory;
}

// Replay 1.20 effect support assets to and from neutral data (docs/NEUTRAL_ASSETS.md, family F7). A vector field's
// sub field flags have no mapped meaning and go to the provenance, which from_neutral restores for a source of the same
// profile and otherwise rejects; a pipeline needs none.
namespace zonetool::iw8::neutral
{
	inline constexpr auto fx_profile = "iw8 replay 1.20";

	struct vector_field_source
	{
		asset_data::vector_field field;
		asset_data::provenance provenance;

		bool operator==(const vector_field_source&) const = default;
	};

	std::expected<asset_data::physics_fx_pipeline, std::string> to_neutral(const PhysicsFXPipeline& native);
	PhysicsFXPipeline* from_neutral(const asset_data::physics_fx_pipeline& data, zone_memory& memory);
	std::size_t native_size(const asset_data::physics_fx_pipeline& data);

	std::expected<vector_field_source, std::string> to_neutral(const VectorField& native);
	VectorField* from_neutral(const asset_data::vector_field& data, const asset_data::provenance* source, zone_memory& memory);
	std::size_t native_size(const asset_data::vector_field& data);

	inline VectorField* from_neutral(const vector_field_source& source, zone_memory& memory)
	{
		return from_neutral(source.field, &source.provenance, memory);
	}

	inline std::size_t native_size(const vector_field_source& source)
	{
		return native_size(source.field);
	}
}
