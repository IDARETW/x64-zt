#pragma once

#include "asset_data.hpp"
#include <map>
#include <stdexcept>
#include <string>

// Version-specific evidence of an asset (docs/NEUTRAL_ASSETS.md): the native fields of its source profile that have no
// neutral meaning, by native field name, as hexadecimal bytes in source order. A writer of the same profile may restore
// them; a writer of another profile must not read them.
namespace zonetool::asset_data
{
struct provenance
{
	std::string profile;
	std::string kind;
	std::string name;
	std::map<std::string, std::string> fields;

	bool operator==(const provenance&) const = default;

	// The bytes of a field, as written by hex(): the profile must match the reader of the field.
	template <typename T>
	T value(const std::string& field) const
	{
		const auto found = this->fields.find(field);
		if (found == this->fields.end() || found->second.size() != sizeof(T) * 2)
		{
			throw std::runtime_error(this->kind + " " + this->name + " provenance lacks " + field);
		}

		T result{};
		auto* bytes = reinterpret_cast<unsigned char*>(&result);
		for (std::size_t i = 0; i < sizeof(T); ++i)
		{
			bytes[i] = static_cast<unsigned char>(std::stoul(found->second.substr(i * 2, 2), nullptr, 16));
		}

		return result;
	}
};

template <typename T>
std::string hex(const T& value)
{
	constexpr char digits[] = "0123456789ABCDEF";
	std::string text;
	const auto* bytes = reinterpret_cast<const unsigned char*>(&value);
	for (std::size_t i = 0; i < sizeof(T); ++i)
	{
		text += digits[bytes[i] >> 4];
		text += digits[bytes[i] & 15];
	}

	return text;
}

inline detail::data_json write_provenance(const provenance& data)
{
	auto fields = detail::data_json::object();
	for (const auto& [field, bytes] : data.fields)
	{
		fields[field] = bytes;
	}

	return {{"profile", data.profile}, {"kind", data.kind}, {"name", data.name}, {"fields", std::move(fields)}};
}

inline provenance read_provenance(const detail::data_json& value)
{
	detail::fields(value, {"profile", "kind", "name", "fields"}, "provenance");
	provenance result{detail::text(value.at("profile"), "provenance"), detail::text(value.at("kind"), "provenance"),
		detail::text(value.at("name"), "provenance"), {}};
	if (!value.at("fields").is_object())
	{
		throw std::runtime_error("provenance fields must be an object");
	}

	for (const auto& [field, bytes] : value.at("fields").items())
	{
		const auto text = detail::text(bytes, "provenance");
		if (text.size() % 2 || text.find_first_not_of("0123456789ABCDEF") != std::string::npos)
		{
			throw std::runtime_error("provenance field " + field + " must be uppercase hexadecimal bytes");
		}

		result.fields.emplace(field, text);
	}

	return result;
}
} // namespace zonetool::asset_data
