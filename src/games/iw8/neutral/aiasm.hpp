#pragma once

// Emitted by tools/gen_iw8_neutral.py from tools/neutral_families/aiasm.json: do not edit.
#include <core/assets/aiasm_data.hpp>
#include "resolver.hpp"
#include "../structs.hpp"

#include <expected>
#include <string>

namespace zonetool
{
	class zone_memory;
}

// Replay 1.20 aiasm assets to and from neutral data (docs/NEUTRAL_ASSETS.md).
namespace zonetool::iw8::neutral
{
	std::expected<asset_data::aiasm::aiasm, std::string> to_neutral(const ASM& native);
	ASM* from_neutral(const asset_data::aiasm::aiasm& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::aiasm::aiasm& data);

	std::expected<asset_data::aiasm::aianimset, std::string> to_neutral(const Animset& native);
	Animset* from_neutral(const asset_data::aiasm::aianimset& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::aiasm::aianimset& data);

}
