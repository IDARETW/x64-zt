#pragma once

#include <cstdint>
#include <functional>
#include <string>

namespace zonetool::iw8::neutral
{
	// The header of the asset of a type and definition name that a rebuilt asset points to, or nullptr.
	using asset_resolver = std::function<void*(std::int32_t type, const std::string& name)>;
}
