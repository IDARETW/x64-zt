#pragma once

// Emitted by tools/gen_iw8_neutral.py from tools/neutral_families/sound.json: do not edit.
#include <core/assets/sound_data.hpp>
#include "resolver.hpp"
#include "../structs.hpp"

#include <expected>
#include <string>

namespace zonetool
{
	class zone_memory;
}

// Replay 1.20 sound assets to and from neutral data (docs/NEUTRAL_ASSETS.md).
namespace zonetool::iw8::neutral
{
	std::expected<asset_data::sound::sound_bank_resident, std::string> to_neutral(const SndBankResident& native);
	SndBankResident* from_neutral(const asset_data::sound::sound_bank_resident& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::sound::sound_bank_resident& data);

	std::expected<asset_data::sound::sound_bank_transient, std::string> to_neutral(const SndBankTransient& native);
	SndBankTransient* from_neutral(const asset_data::sound::sound_bank_transient& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::sound::sound_bank_transient& data);

}
