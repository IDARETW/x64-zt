#pragma once

#include <core/assets/provenance.hpp>
#include <core/assets/shader_data.hpp>
#include "../structs.hpp"

#include <expected>
#include <string>

namespace zonetool
{
	class zone_memory;
}

// Replay 1.20 shaders to and from neutral data (docs/NEUTRAL_ASSETS.md, family F3). The five shader asset types and the
// library shader share one record; the stage is the asset type. The four bytes after the program size hold a value that
// differs for every shader of a stock zone (not a CRC, Adler, FNV or MD5 prefix of the program or the name): it goes to
// the provenance, which from_neutral restores for a source of the same profile and otherwise rejects.
namespace zonetool::iw8::neutral
{
	inline constexpr auto shader_profile = "iw8 replay 1.20";

	struct shader_source
	{
		asset_data::shader shader;
		asset_data::provenance provenance;

		bool operator==(const shader_source&) const = default;
	};

	std::expected<shader_source, std::string> to_neutral(const ComputeShader& native, std::int32_t asset_type);
	ComputeShader* from_neutral(const asset_data::shader& data, const asset_data::provenance* source, zone_memory& memory);
	std::size_t native_size(const asset_data::shader& data);

	inline ComputeShader* from_neutral(const shader_source& source, zone_memory& memory)
	{
		return from_neutral(source.shader, &source.provenance, memory);
	}

	inline std::size_t native_size(const shader_source& source)
	{
		return native_size(source.shader);
	}

	// The stage of a shader asset type, or an empty string for another type.
	std::string shader_stage(std::int32_t asset_type);
}
