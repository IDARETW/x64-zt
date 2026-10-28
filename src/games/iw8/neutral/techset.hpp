#pragma once

#include <core/assets/material_data.hpp>
#include "resolver.hpp"
#include "../structs.hpp"

#include <expected>
#include <map>
#include <string>

namespace zonetool
{
	class zone_memory;
}

// Replay 1.20 technique sets to and from neutral data (docs/NEUTRAL_ASSETS.md, family F3). A technique that two sets
// share is one object in the zone, and techniques with equal arrays share them: the rebuild keeps what it has written by
// content, so equal data is written once.
namespace zonetool::iw8::neutral
{
	struct technique_cache
	{
		std::map<std::string, MaterialTechnique*> techniques;
		// The arrays of the techniques by their kind and bytes.
		std::map<std::string, void*> arrays;
	};

	std::expected<asset_data::technique_set, std::string> to_neutral(const MaterialTechniqueSet& native);
	MaterialTechniqueSet* from_neutral(const asset_data::technique_set& data, const asset_resolver& resolve, technique_cache& cache,
		zone_memory& memory);
	std::size_t native_size(const asset_data::technique_set& data);
}
