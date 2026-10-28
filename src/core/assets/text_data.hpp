#pragma once

#include "asset_data.hpp"
#include <algorithm>
#include <cstdint>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

// Neutral text assets (family F1 of docs/NEUTRAL_ASSETS.md). The JSON forms are the canonical forms of iw8-zonetool's
// docs/ASSET_CONVERSION.md, so the bundles of both tools hold the same records.
namespace zonetool::asset_data
{
// A localized string. An absent value and a present empty value stay distinct.
struct localize_entry
{
	std::string name;
	std::optional<std::string> value;

	bool operator==(const localize_entry&) const = default;
};

// A table of network-indexed strings: its category names the asset kind it indexes as a namespace identifier (mdl,
// vfx, rmb ...), its scope the load level that owns it. An absent list and a present empty list stay distinct, and
// each position keeps its string, null or empty, as authored.
struct net_const_strings
{
	std::string name;
	std::string category;
	std::string scope;
	std::optional<std::vector<std::optional<std::string>>> strings;

	bool operator==(const net_const_strings&) const = default;
};

// A raw file's decoded content. Compression is the writer's concern. An absent payload and a present empty payload
// stay distinct.
struct raw_file
{
	std::string name;
	std::optional<std::vector<std::uint8_t>> bytes;

	bool operator==(const raw_file&) const = default;
};

// A table of strings: rows of `columns` cells, row after row. A null cell and an empty cell stay distinct. The target
// encoding (a dictionary of the unique strings with their hashes) is the writer's concern.
struct string_table
{
	std::string name;
	std::uint32_t columns{};
	std::uint32_t rows{};
	std::vector<std::optional<std::string>> cells;

	bool operator==(const string_table&) const = default;
};

inline void validate(const localize_entry& data)
{
	detail::name(data.name, "localize");
}

// iw8-zonetool's scopes, and none: the scope of the placeholder table (void) of Replay's code_post_gfx.
inline constexpr const char* net_const_string_scopes[] = {"global", "common", "before_map", "map", "changes", "none"};

inline void validate(const net_const_strings& data)
{
	detail::name(data.name, "netconststrings");
	if (data.category.empty() || data.category.size() > 64 ||
		data.category.find_first_not_of("abcdefghijklmnopqrstuvwxyz0123456789_") != std::string::npos)
	{
		throw std::runtime_error("netconststrings category must be a namespace identifier");
	}
	if (std::ranges::find(net_const_string_scopes, data.scope) == std::end(net_const_string_scopes))
	{
		throw std::runtime_error("netconststrings scope is unknown: " + data.scope);
	}
}

inline void validate(const raw_file& data)
{
	detail::name(data.name, "rawfile");
}

inline void validate(const string_table& data)
{
	detail::name(data.name, "stringtable");
	if (data.cells.size() != static_cast<std::size_t>(data.rows) * data.columns)
	{
		throw std::runtime_error("stringtable " + data.name + " does not have rows * columns cells");
	}
}

inline detail::data_json write_localize_entry(const localize_entry& data)
{
	validate(data);
	return {{"format", "canonical-localizeentry"}, {"schema_version", 1}, {"name", data.name},
		{"value", detail::optional_json(data.value)}};
}

inline localize_entry read_localize_entry(const detail::data_json& value)
{
	detail::fields(value, {"format", "schema_version", "name", "value"}, "localize");
	if (value.at("format") != "canonical-localizeentry" || detail::integer(value.at("schema_version"), "localize") != 1)
	{
		throw std::runtime_error("unsupported localize data schema");
	}
	localize_entry result{detail::text(value.at("name"), "localize"), detail::optional_text(value.at("value"), "localize")};
	validate(result);
	return result;
}

inline detail::data_json write_net_const_strings(const net_const_strings& data)
{
	validate(data);
	auto strings = detail::data_json(nullptr);
	if (data.strings)
	{
		strings = detail::data_json::array();
		for (const auto& entry : *data.strings)
		{
			strings.push_back(detail::optional_json(entry));
		}
	}
	return {{"format", "canonical-netconststrings"}, {"schema_version", 1}, {"name", data.name},
		{"category", data.category}, {"scope", data.scope}, {"strings", std::move(strings)}};
}

inline net_const_strings read_net_const_strings(const detail::data_json& value)
{
	detail::fields(value, {"format", "schema_version", "name", "category", "scope", "strings"}, "netconststrings");
	if (value.at("format") != "canonical-netconststrings" ||
		detail::integer(value.at("schema_version"), "netconststrings") != 1 ||
		!(value.at("strings").is_array() || value.at("strings").is_null()))
	{
		throw std::runtime_error("unsupported netconststrings data schema");
	}
	net_const_strings result{detail::text(value.at("name"), "netconststrings"),
		detail::text(value.at("category"), "netconststrings"), detail::text(value.at("scope"), "netconststrings"), {}};
	if (const auto& strings = value.at("strings"); strings.is_array())
	{
		auto& entries = result.strings.emplace();
		for (const auto& entry : strings)
		{
			entries.push_back(detail::optional_text(entry, "netconststrings"));
		}
	}
	validate(result);
	return result;
}

inline detail::data_json write_raw_file(const raw_file& data)
{
	validate(data);
	return {{"format", "canonical-rawfile"}, {"schema_version", 1}, {"name", data.name},
		{"bytes", data.bytes ? detail::data_json(*data.bytes) : detail::data_json(nullptr)}};
}

inline raw_file read_raw_file(const detail::data_json& value)
{
	detail::fields(value, {"format", "schema_version", "name", "bytes"}, "rawfile");
	if (value.at("format") != "canonical-rawfile" || detail::integer(value.at("schema_version"), "rawfile") != 1)
	{
		throw std::runtime_error("unsupported rawfile data schema");
	}
	raw_file result{detail::text(value.at("name"), "rawfile"), {}};
	if (const auto& bytes = value.at("bytes"); !bytes.is_null())
	{
		if (!bytes.is_array())
		{
			throw std::runtime_error("rawfile bytes must be an array or null");
		}
		auto& content = result.bytes.emplace();
		content.reserve(bytes.size());
		for (const auto& byte : bytes)
		{
			const auto number = detail::integer(byte, "rawfile");
			if (number < 0 || number > 255)
			{
				throw std::runtime_error("rawfile byte is outside 0..255");
			}
			content.push_back(static_cast<std::uint8_t>(number));
		}
	}
	validate(result);
	return result;
}
inline detail::data_json write_string_table(const string_table& data)
{
	validate(data);
	auto rows = detail::data_json::array();
	for (std::size_t row = 0; row < data.rows; ++row)
	{
		auto& cells = rows.emplace_back(detail::data_json::array());
		for (std::size_t column = 0; column < data.columns; ++column)
		{
			cells.push_back(detail::optional_json(data.cells[row * data.columns + column]));
		}
	}
	return {{"format", "canonical-stringtable"}, {"schema_version", 1}, {"name", data.name}, {"columns", data.columns},
		{"rows", std::move(rows)}};
}

inline string_table read_string_table(const detail::data_json& value)
{
	detail::fields(value, {"format", "schema_version", "name", "columns", "rows"}, "stringtable");
	const auto columns = detail::integer(value.at("columns"), "stringtable");
	if (value.at("format") != "canonical-stringtable" || detail::integer(value.at("schema_version"), "stringtable") != 1 ||
		columns < 0 || !value.at("rows").is_array())
	{
		throw std::runtime_error("unsupported stringtable data schema");
	}
	string_table result{detail::text(value.at("name"), "stringtable"), static_cast<std::uint32_t>(columns),
		static_cast<std::uint32_t>(value.at("rows").size()), {}};
	for (const auto& row : value.at("rows"))
	{
		if (!row.is_array() || row.size() != result.columns)
		{
			throw std::runtime_error("stringtable rows must be arrays of `columns` cells");
		}
		for (const auto& cell : row)
		{
			result.cells.push_back(detail::optional_text(cell, "stringtable"));
		}
	}
	validate(result);
	return result;
}
} // namespace zonetool::asset_data
