#pragma once

#include <functional>
#include <string>

namespace localized_strings
{
	using entry_callback = std::function<void(const std::string& name, const std::string& value)>;

	// Reads localizedstrings/<file_name>.json, an object of names and values. False when there is no such file or the
	// file is not an object.
	bool parse_json(const std::string& file_name, const entry_callback& add);

	// Reads the REFERENCE and LANG_ lines of localizedstrings/<file_name>.str. False when there is no such file or a
	// line does not parse. The entries before that line are added.
	bool parse_str(const std::string& file_name, const entry_callback& add);
}
