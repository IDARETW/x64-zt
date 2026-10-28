#pragma once

#include <core/assets/provenance.hpp>
#include <core/assets/xmodel_data.hpp>
#include "resolver.hpp"
#include "../structs.hpp"

#include <expected>
#include <string>

namespace zonetool
{
	class zone_memory;
}

// Replay 1.20 models to and from neutral data (docs/NEUTRAL_ASSETS.md, family F4). to_neutral gives the reason for a
// record it does not decode, and the native record stays. The flags, contents, character collision bounds type, LGV
// data, physics usage counter and runtime words go to the provenance; from_neutral restores them for a source of the
// same profile and rejects a model without them, since their meanings are not mapped.
namespace zonetool::iw8::neutral
{
	inline constexpr auto xmodel_profile = "iw8 replay 1.20";

	struct xmodel_source
	{
		asset_data::xmodel model;
		asset_data::provenance provenance;

		bool operator==(const xmodel_source&) const = default;
	};

	struct xmodel_surfs_source
	{
		asset_data::xmodel_surfs surfs;
		asset_data::provenance provenance;

		bool operator==(const xmodel_surfs_source&) const = default;
	};

	std::expected<xmodel_source, std::string> to_neutral(const XModel& native);
	XModel* from_neutral(const asset_data::xmodel& data, const asset_data::provenance* source, const asset_resolver& resolve,
		zone_memory& memory);
	std::size_t native_size(const asset_data::xmodel& data);

	// Resident geometry only: streamed geometry, subdivision, blend shapes and tension streams give their reason.
	// Surface flags, UGB ids and hashes go to the provenance.
	std::expected<xmodel_surfs_source, std::string> to_neutral(const XModelSurfs& native);
	XModelSurfs* from_neutral(const asset_data::xmodel_surfs& data, const asset_data::provenance* source, zone_memory& memory);
	std::size_t native_size(const asset_data::xmodel_surfs& data);

	inline XModel* from_neutral(const xmodel_source& source, const asset_resolver& resolve, zone_memory& memory)
	{
		return from_neutral(source.model, &source.provenance, resolve, memory);
	}

	inline XModelSurfs* from_neutral(const xmodel_surfs_source& source, zone_memory& memory)
	{
		return from_neutral(source.surfs, &source.provenance, memory);
	}

	inline std::size_t native_size(const xmodel_surfs_source& source)
	{
		return native_size(source.surfs);
	}

	inline std::size_t native_size(const xmodel_source& source)
	{
		return native_size(source.model);
	}
}
