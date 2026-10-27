#pragma once

#include <core/assets/text_data.hpp>
#include "../structs.hpp"

#include <expected>

namespace zonetool
{
	class zone_memory;
}

// Replay 1.20 text assets to and from neutral data (docs/NEUTRAL_ASSETS.md, family F1). to_neutral rejects a native
// field that has no neutral meaning yet; for a string table whose stored hashes do not follow Replay's rule it gives
// the reason, and the native record stays. from_neutral allocates fresh native records in the zone memory, which
// native_size sizes.
namespace zonetool::iw8::neutral
{
	asset_data::localize_entry to_neutral(const LocalizeEntry& native);
	asset_data::net_const_strings to_neutral(const NetConstStrings& native);
	asset_data::raw_file to_neutral(const RawFile& native);
	std::expected<asset_data::string_table, std::string> to_neutral(const StringTable& native);

	LocalizeEntry* from_neutral(const asset_data::localize_entry& data, zone_memory& memory);
	NetConstStrings* from_neutral(const asset_data::net_const_strings& data, zone_memory& memory);
	RawFile* from_neutral(const asset_data::raw_file& data, zone_memory& memory);
	StringTable* from_neutral(const asset_data::string_table& data, zone_memory& memory);

	std::size_t native_size(const asset_data::localize_entry& data);
	std::size_t native_size(const asset_data::net_const_strings& data);
	std::size_t native_size(const asset_data::raw_file& data);
	std::size_t native_size(const asset_data::string_table& data);
}
