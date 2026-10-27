#pragma once

#include <json.hpp>
#include <cmath>
#include <cstdint>
#include <initializer_list>
#include <limits>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>

// Strict reading of neutral asset data (docs/NEUTRAL_ASSETS.md): every object holds exactly its fields, numbers keep
// their range and names are definition names. `what` names the asset kind in the error.
namespace zonetool::asset_data::detail
{
using data_json = nlohmann::ordered_json;

inline void fields(const data_json& value, const std::initializer_list<std::string_view> names, const char* what)
{
	if (!value.is_object() || value.size() != names.size())
	{
		throw std::runtime_error(std::string(what) + " data has missing or unknown fields");
	}
	for (const auto name : names)
	{
		if (!value.contains(std::string(name)))
		{
			throw std::runtime_error(std::string(what) + " data is missing " + std::string(name));
		}
	}
}

inline void name(const std::string& value, const char* what)
{
	if (value.empty() || value.front() == ',' || value.find('\0') != std::string::npos)
	{
		throw std::runtime_error(std::string(what) + " asset name must be a nonempty definition name");
	}
}

inline std::string text(const data_json& value, const char* what)
{
	if (!value.is_string())
	{
		throw std::runtime_error(std::string(what) + " value must be a string");
	}
	auto result = value.get<std::string>();
	if (result.find('\0') != std::string::npos)
	{
		throw std::runtime_error(std::string(what) + " string holds a null character");
	}
	return result;
}

// A string that may be absent (null): absent and present-empty stay distinct.
inline std::optional<std::string> optional_text(const data_json& value, const char* what)
{
	return value.is_null() ? std::nullopt : std::optional(text(value, what));
}

inline data_json optional_json(const std::optional<std::string>& value)
{
	return value ? data_json(*value) : data_json(nullptr);
}

inline float number(const data_json& value, const char* what)
{
	if (!value.is_number())
	{
		throw std::runtime_error(std::string(what) + " value must be a finite float");
	}
	const double input = value.get<double>();
	if (!std::isfinite(input) || std::abs(input) > std::numeric_limits<float>::max())
	{
		throw std::runtime_error(std::string(what) + " value is outside the finite float range");
	}
	const auto result = static_cast<float>(input);
	if (input != 0 && result == 0)
	{
		throw std::runtime_error(std::string(what) + " value underflows the float range");
	}
	return result;
}

inline std::int32_t integer(const data_json& value, const char* what)
{
	if (value.is_number_unsigned())
	{
		const auto number = value.get<std::uint64_t>();
		if (number > static_cast<std::uint64_t>(std::numeric_limits<std::int32_t>::max()))
		{
			throw std::runtime_error(std::string(what) + " integer exceeds int32");
		}
		return static_cast<std::int32_t>(number);
	}
	if (!value.is_number_integer())
	{
		throw std::runtime_error(std::string(what) + " integer must have an integer encoding");
	}
	const auto number = value.get<std::int64_t>();
	if (number < std::numeric_limits<std::int32_t>::min() || number > std::numeric_limits<std::int32_t>::max())
	{
		throw std::runtime_error(std::string(what) + " integer exceeds int32");
	}
	return static_cast<std::int32_t>(number);
}
} // namespace zonetool::asset_data::detail
