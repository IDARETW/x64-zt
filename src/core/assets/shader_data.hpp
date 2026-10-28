#pragma once

#include "asset_data.hpp"
#include <algorithm>
#include <cstdint>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

// Neutral shaders (family F3 of docs/NEUTRAL_ASSETS.md): a compiled program of one stage. The program is the bytecode of
// its profile's compiler (DXBC for the Direct3D profiles); turning another profile's shaders into it is a compilation,
// which the shading model of the material family owns.
namespace zonetool::asset_data
{
struct shader
{
	std::string name;
	// The name that the compiler gave the program, when the asset has one.
	std::optional<std::string> debug_name;
	// vertex, hull, domain, pixel, compute or library.
	std::string stage;
	// An absent program and a present empty one stay distinct.
	std::optional<std::vector<std::uint8_t>> bytecode;

	bool operator==(const shader&) const = default;
};

inline constexpr const char* shader_stages[] = {"vertex", "hull", "domain", "pixel", "compute", "library"};

inline void validate(const shader& data)
{
	detail::name(data.name, "shader");
	if (data.debug_name && data.debug_name->find('\0') != std::string::npos)
	{
		throw std::runtime_error("shader " + data.name + " has a debug name with a null character");
	}

	if (std::ranges::find(shader_stages, data.stage) == std::end(shader_stages))
	{
		throw std::runtime_error("shader " + data.name + " has an unknown stage: " + data.stage);
	}
}
} // namespace zonetool::asset_data
