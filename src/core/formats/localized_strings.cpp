#include <std_include.hpp>
#include "localized_strings.hpp"
#include "../zonetool.hpp"

namespace localized_strings
{
	bool parse_json(const std::string& file_name, const entry_callback& add)
	{
		const auto path = "localizedstrings\\"s + file_name + ".json";
		auto file = zonetool::filesystem::file(path);
		file.open("rb");

		auto* fp = file.get_fp();

		if (fp == nullptr)
		{
			return false;
		}

		const auto size = file.size();
		const auto bytes = file.read_bytes(size);
		const auto localize = json::parse(bytes);

		ZONETOOL_INFO("Parsing localizedstrings \"%s.json\"...", file_name.data());
		if (!localize.is_object())
		{
			ZONETOOL_ERROR("Localized strings json file should be an object!");
			return false;
		}

		for (const auto& [key, value] : localize.items())
		{
			add(key, value.get<std::string>());
		}

		return true;
	}

	bool parse_str(const std::string& file_name, const entry_callback& add)
	{
		const auto path = "localizedstrings\\"s + file_name + ".str";
		auto file = zonetool::filesystem::file(path);
		file.open("rb");

		auto* fp = file.get_fp();
		if (fp)
		{
			ZONETOOL_INFO("Parsing localizedstrings \"%s.str\"...", file_name.data());

			auto bytes = file.read_bytes(file.size());
			std::string str = std::string(bytes.begin(), bytes.end());

			std::stringstream stringstream(str);
			std::string string;

			char* data;
			size_t size = 0;

			bool failed = false;
			std::string name;
			std::string value;
			size_t line = 0;
			size_t i = 0;
			while (getline(stringstream, string))
			{
				data = string.data();
				size = string.size();
				++line;
				if (size < 2)
					continue;
				for (i = 0; i < size; i++)
				{
					if (i + 1 < size && data[i] == '/' && data[i + 1] == '/')
						break;
					if (isspace(data[i]))
						continue;
					if (data[i] >= 'A' && data[i] <= 'Z')
					{
						if (!strncmp(&data[i], "REFERENCE", 9))
						{
							i += 9;
							while (isspace(data[i]))
							{
								if (i >= size)
								{
									failed = true;
									break;
								}
								i++;
							}
							if (failed)
							{
								break;
							}

							name.clear();
							while (!isspace(data[i]))
							{
								if (i >= size)
								{
									break;
								}
								name += data[i];
								i++;
							}
							break;
						}
						if (!strncmp(&data[i], "LANG_", 5))
						{
							i += 5;
							while (data[i] != '"')
							{
								if (i >= size)
								{
									failed = true;
									break;
								}
								i++;
							}
							if (failed)
							{
								break;
							}

							i++;
							value.clear();
							while (data[i] != '"')
							{
								if (i >= size)
								{
									failed = true;
									break;
								}
								if (data[i] == '\\' && i + 1 < size)
								{
									switch (data[i + 1])
									{
									case 'n':
										value += '\n';
										i += 2;
										break;
									case 't':
										value += '\t';
										i += 2;
										break;
									default:
										value += '\\';
										i++;
										break;
									}
								}
								else
								{
									value += data[i];
									i++;
								}
							}
							break;
						}
						if (!strncmp(&data[i], "ENDMARKER", 9))
						{
							file.close();
							return true;
						}
					}
				}
				if (failed)
				{
					ZONETOOL_WARNING("\"%s\" parse failed at line: %zu index: %zu", path.data(), line, i);
					file.close();
					return false;
				}
				if (!name.empty() && !value.empty())
				{
					add(name, value);
					name.clear();
					value.clear();
				}
			}
			file.close();
			return true;
		}
		return false;
	}
}
