#include <std_include.hpp>
#include "../iw8.hpp"
#include "asset_files.hpp"
#include "loader.hpp"

#include <formats/compression.hpp>
#include <formats/csv.hpp>

#include <zlib.h>

namespace zonetool::iw8
{
	namespace
	{
		std::string name_of(const std::int32_t type, const void* header)
		{
			const auto name = get_asset_name(type, header);
			return name ? name : "";
		}

		const asset_format& format_of(const std::int32_t type)
		{
			const auto format = find_asset_format(type);
			if (!format)
			{
				throw std::runtime_error(std::format("asset type {} has no format", type));
			}

			return *format;
		}

		// Characters that Windows does not take in a file name; the path keeps the folders of the name.
		std::string file_name(std::string name)
		{
			for (auto& c : name)
			{
				if (static_cast<unsigned char>(c) < 32 || std::string_view("<>:\"|?*").contains(c))
				{
					c = '_';
				}
			}

			return name;
		}

		std::optional<std::string> read_file(const std::string& path)
		{
			filesystem::file file(path);
			file.open("rb");
			if (!file.get_fp())
			{
				return {};
			}

			const auto bytes = file.read_bytes(file.size());
			file.close();
			return std::string(bytes.begin(), bytes.end());
		}

		// A cell as the core string table dump writes it.
		std::string csv_cell(const std::string_view value)
		{
			std::string cell;
			for (const auto c : value)
			{
				cell += c == '\n' ? "\\n" : std::string(1, c);
			}

			return value.contains(',') ? std::format("\"{}\"", cell) : cell;
		}

		std::string dump_localize(const void* header)
		{
			const auto& entry = *static_cast<const LocalizeEntry*>(header);
			if (!entry.value)
			{
				throw std::runtime_error("the entry has no value");
			}

			return entry.value;
		}

		void* parse_localize(asset_parser& parser, const std::string& name, const std::string& data)
		{
			auto& entry = *parser.allocate<LocalizeEntry>();
			entry.name = parser.duplicate(localize_name(name));
			entry.value = parser.duplicate(data);
			return &entry;
		}

		std::string dump_rawfile(const void* header)
		{
			const auto& file = *static_cast<const RawFile*>(header);
			std::string data(file.len, '\0');
			auto size = static_cast<uLongf>(data.size());
			if (!file.buffer || uncompress(reinterpret_cast<Bytef*>(data.data()), &size,
				reinterpret_cast<const Bytef*>(file.buffer), file.compressedLen) != Z_OK || size != data.size())
			{
				throw std::runtime_error("the buffer does not decompress to the file length");
			}

			return data;
		}

		void* parse_rawfile(asset_parser& parser, const std::string& name, const std::string& data)
		{
			const auto compressed = compression::compress_zlib(reinterpret_cast<const std::uint8_t*>(data.data()), data.size());
			auto& file = *parser.allocate<RawFile>();
			file.name = parser.duplicate(name);
			file.compressedLen = static_cast<std::uint32_t>(compressed.size());
			file.len = static_cast<std::uint32_t>(data.size());
			const auto buffer = parser.allocate<char>(compressed.size());
			std::ranges::copy(compressed, buffer);
			file.buffer = buffer;
			return &file;
		}

		std::string dump_stringtable(const void* header)
		{
			const auto& table = *static_cast<const StringTable*>(header);
			if ((!table.cellIndices || !table.strings) && table.rowCount * table.columnCount)
			{
				throw std::runtime_error("the table has no cells");
			}

			std::string text;
			for (auto row = 0; row < table.rowCount; row++)
			{
				for (auto column = 0; column < table.columnCount; column++)
				{
					const auto index = table.cellIndices[row * table.columnCount + column];
					if (index >= table.uniqueCellCount)
					{
						throw std::runtime_error(std::format("cell {} has the string index {}", row * table.columnCount + column, index));
					}

					const auto value = table.strings[index];
					text += csv_cell(value ? value : "");
					text += column + 1 < table.columnCount ? ',' : '\n';
				}
			}

			return text;
		}

		// Stock tables keep the unique strings in the order of their first cell.
		void* parse_stringtable(asset_parser& parser, const std::string& name, const std::string& data)
		{
			csv::parser_raw file(data.data(), static_cast<int>(data.size()));
			auto& table = *parser.allocate<StringTable>();
			table.name = parser.duplicate(name);
			table.rowCount = file.get_num_rows();
			table.columnCount = file.get_max_columns();

			const auto indices = parser.allocate<std::uint16_t>(static_cast<std::size_t>(table.rowCount) * table.columnCount);
			std::vector<std::string_view> strings;
			std::unordered_map<std::string_view, std::size_t> unique;
			const auto rows = file.get_rows();
			for (auto row = 0; row < table.rowCount; row++)
			{
				for (auto column = 0; column < table.columnCount; column++)
				{
					const std::string_view value = column < rows[row]->num_fields ? rows[row]->fields[column] : "";
					const auto [entry, added] = unique.try_emplace(value, strings.size());
					if (added)
					{
						strings.emplace_back(value);
					}

					if (entry->second > std::numeric_limits<std::uint16_t>::max())
					{
						throw std::runtime_error("the table has more than 65536 unique strings");
					}

					indices[row * table.columnCount + column] = static_cast<std::uint16_t>(entry->second);
				}
			}

			const auto values = parser.allocate<const char*>(strings.size());
			table.hashes = parser.allocate<std::int32_t>(strings.size());
			for (std::size_t i = 0; i < strings.size(); i++)
			{
				values[i] = parser.duplicate(strings[i]);
				table.hashes[i] = string_table_hash(strings[i]);
			}

			table.uniqueCellCount = static_cast<std::int32_t>(strings.size());
			table.cellIndices = indices;
			table.strings = values;
			return &table;
		}

		std::string dump_scriptfile(const void* header)
		{
			const auto& script = *static_cast<const ScriptFile*>(header);
			if (script.compressedLen < 0 || script.bytecodeLen < 0 || (!script.buffer && script.compressedLen)
				|| (!script.bytecode && script.bytecodeLen))
			{
				throw std::runtime_error("the script has no data");
			}

			std::string data(script.name ? script.name : "");
			data += '\0';
			for (const auto length : {script.compressedLen, script.len, script.bytecodeLen})
			{
				data.append(reinterpret_cast<const char*>(&length), sizeof(length));
			}

			data.append(script.buffer ? script.buffer : "", static_cast<std::size_t>(script.compressedLen));
			data.append(script.bytecode ? reinterpret_cast<const char*>(script.bytecode) : "", static_cast<std::size_t>(script.bytecodeLen));
			return data;
		}

		void* parse_scriptfile(asset_parser& parser, const std::string& name, const std::string& data)
		{
			auto position = data.find('\0');
			if (position == std::string::npos || data.size() - position < 13)
			{
				throw std::runtime_error("the file has no lengths");
			}

			auto& script = *parser.allocate<ScriptFile>();
			script.name = parser.duplicate(name);
			position++;
			for (const auto length : {&script.compressedLen, &script.len, &script.bytecodeLen})
			{
				std::memcpy(length, data.data() + position, sizeof(*length));
				position += sizeof(*length);
			}

			if (script.compressedLen < 0 || script.bytecodeLen < 0
				|| data.size() - position != static_cast<std::size_t>(script.compressedLen) + script.bytecodeLen)
			{
				throw std::runtime_error("the lengths do not match the file size");
			}

			// Stock scripts have no pointer for an empty buffer or byte code.
			if (script.compressedLen)
			{
				const auto buffer = parser.allocate<char>(script.compressedLen);
				std::memcpy(buffer, data.data() + position, script.compressedLen);
				script.buffer = buffer;
			}

			if (script.bytecodeLen)
			{
				script.bytecode = parser.allocate<std::uint8_t>(script.bytecodeLen);
				std::memcpy(script.bytecode, data.data() + position + script.compressedLen, script.bytecodeLen);
			}

			return &script;
		}

		std::string dump_luafile(const void* header)
		{
			const auto& file = *static_cast<const LuaFile*>(header);
			if (!file.buffer)
			{
				throw std::runtime_error("the file has no buffer");
			}

			return {reinterpret_cast<const char*>(file.buffer), static_cast<std::size_t>(file.len)};
		}

		void* parse_luafile(asset_parser& parser, const std::string& name, const std::string& data)
		{
			auto& file = *parser.allocate<LuaFile>();
			file.name = parser.duplicate(name);
			file.len = static_cast<std::int32_t>(data.size());
			// The stripping type of the stock Lua files.
			file.strippingType = 1;
			file.buffer = reinterpret_cast<const std::uint8_t*>(parser.duplicate(data));
			return &file;
		}

		std::string dump_ttf(const void* header)
		{
			const auto& font = *static_cast<const TTFDef*>(header);
			if (!font.file)
			{
				throw std::runtime_error("the font has no file");
			}

			return {font.file, static_cast<std::size_t>(font.fileLen)};
		}

		void* parse_ttf(asset_parser& parser, const std::string& name, const std::string& data)
		{
			auto& font = *parser.allocate<TTFDef>();
			font.name = parser.duplicate(name);
			font.fileLen = static_cast<std::int32_t>(data.size());
			font.file = parser.duplicate(data);
			return &font;
		}

		struct shared_format
		{
			std::int32_t type;
			std::string_view folder;
			std::string_view extension;
			// Raw files compress their file again, so their data can differ from the dumped data.
			bool exact;
			std::string (*dump)(const void* header);
			void* (*parse)(asset_parser& parser, const std::string& name, const std::string& data);
		};

		constexpr shared_format shared_formats[]
		{
			{ASSET_TYPE_LOCALIZE_ENTRY, "localizedstrings/", "", true, dump_localize, parse_localize},
			{ASSET_TYPE_RAWFILE, "", "", false, dump_rawfile, parse_rawfile},
			{ASSET_TYPE_SCRIPTFILE, "", ".gscbin", true, dump_scriptfile, parse_scriptfile},
			{ASSET_TYPE_STRINGTABLE, "", "", true, dump_stringtable, parse_stringtable},
			{ASSET_TYPE_LUA_FILE, "", "", true, dump_luafile, parse_luafile},
			{ASSET_TYPE_TTF, "", "", true, dump_ttf, parse_ttf},
		};

		const shared_format* find_shared_format(const std::int32_t type)
		{
			const auto found = std::ranges::find(shared_formats, type, &shared_format::type);
			return found != std::end(shared_formats) ? &*found : nullptr;
		}

		std::string iw8_file_path(const std::int32_t type, const std::string& name)
		{
			return std::format("{}/{}.iw8", format_of(type).name, file_name(name));
		}

		std::string shared_file_path(const shared_format& format, const std::string& name)
		{
			return std::format("{}{}{}", format.folder, file_name(name), format.extension);
		}

		// A shared file holds the asset only when it parses back to the same asset data.
		std::optional<std::string> shared_file_data(const shared_format& format, const std::int32_t type, const void* header)
		{
			try
			{
				auto data = format.dump(header);
				if (format.exact)
				{
					asset_parser parser;
					if (dump_asset_data(type, format.parse(parser, name_of(type, header), data)) != dump_asset_data(type, header))
					{
						return {};
					}
				}

				return data;
			}
			catch (const std::exception&)
			{
				return {};
			}
		}
	}

	std::string localize_name(const std::string& name)
	{
		return utils::string::to_lower(name);
	}

	std::int32_t string_table_hash(const std::string_view value)
	{
		std::uint32_t hash{};
		for (const auto c : value)
		{
			const auto character = static_cast<signed char>(c);
			hash = hash * 31 + static_cast<std::uint32_t>(character >= 'A' && character <= 'Z' ? character + 32 : character);
		}

		return static_cast<std::int32_t>(hash);
	}

	std::vector<asset_file> asset_file_locations(const std::int32_t type, const std::string& name)
	{
		std::vector<asset_file> files;
		if (const auto format = find_shared_format(type))
		{
			files.emplace_back(shared_file_path(*format, name), std::string{}, true);
		}

		files.emplace_back(iw8_file_path(type, name), std::string{}, false);
		return files;
	}

	std::string dump_asset_data(const std::int32_t type, const void* header)
	{
		const auto& format = format_of(type);
		dump_stream stream(name_of);
		stream.begin(type, header, format.size);
		format.dump(stream, const_cast<void*>(header));
		return stream.finish();
	}

	std::vector<std::pair<std::int32_t, std::string>> asset_dependencies(const std::int32_t type, const void* header)
	{
		std::vector<std::pair<std::int32_t, std::string>> dependencies;
		const auto& format = format_of(type);
		dump_stream stream([&dependencies](const std::int32_t dependency, const void* dependency_header)
		{
			return dependencies.emplace_back(dependency, name_of(dependency, dependency_header)).second;
		});
		stream.begin(type, header, format.size);
		format.dump(stream, const_cast<void*>(header));
		return dependencies;
	}

	asset_file dump_asset_file(const std::int32_t type, const void* header)
	{
		const auto name = name_of(type, header);
		if (const auto format = find_shared_format(type))
		{
			if (auto data = shared_file_data(*format, type, header))
			{
				return {shared_file_path(*format, name), std::move(*data), true};
			}
		}

		return {iw8_file_path(type, name), dump_asset_data(type, header), false};
	}

	void dump_asset(const std::int32_t type, const void* header)
	{
		const auto asset = dump_asset_file(type, header);
		filesystem::file file(asset.path);
		file.open("wb");
		file.write(asset.data.data(), asset.data.size(), 1);
		file.close();
	}

	void* asset_parser::parse(const std::string_view data, const std::optional<std::int32_t> expected_type)
	{
		parse_stream stream(data,
			[this](const std::size_t size)
			{
				return this->allocate(size);
			},
			[this](const std::int32_t type, const std::string& name)
			{
				this->references_.emplace_back(type, name);
				return this->stub(type, name);
			});

		if (expected_type && stream.type() != *expected_type)
		{
			throw std::runtime_error(std::format("native asset type {} does not match requested type {}",
				stream.type(), *expected_type));
		}

		const auto& format = format_of(stream.type());
		const auto header = stream.begin(format.size);
		format.parse(stream, header);
		stream.finish();
		return header;
	}

	void* asset_parser::parse_file(const std::int32_t type, const std::string& name)
	{
		for (auto file : asset_file_locations(type, name))
		{
			if (auto data = read_file(file.path))
			{
				file.data = std::move(*data);
				return this->parse_asset_file(type, name, file);
			}
		}

		return nullptr;
	}

	void* asset_parser::parse_asset_file(const std::int32_t type, const std::string& name, const asset_file& file)
	{
		try
		{
			if (!file.shared)
			{
				return this->parse(file.data, type);
			}

			const auto format = find_shared_format(type);
			if (!format)
			{
				throw std::runtime_error("the asset type has no shared file");
			}

			return format->parse(*this, name, file.data);
		}
		catch (const std::exception& e)
		{
			throw std::runtime_error(std::format("{} \"{}\" ({}): {}", format_of(type).name, name, file.path, e.what()));
		}
	}

	void* asset_parser::stub(const std::int32_t type, const std::string& name)
	{
		const auto header = static_cast<const char**>(this->allocate(format_of(type).size));
		*header = this->duplicate(name);
		return header;
	}

	const std::vector<std::pair<std::int32_t, std::string>>& asset_parser::references() const
	{
		return this->references_;
	}

	void* asset_parser::allocate(const std::size_t size)
	{
		auto& memory = this->memory_.emplace_back(std::make_unique<std::uint8_t[]>(std::max<std::size_t>(size, 1)));
		std::memset(memory.get(), 0, std::max<std::size_t>(size, 1));
		return memory.get();
	}

	const char* asset_parser::duplicate(const std::string_view value)
	{
		const auto copy = this->allocate<char>(value.size() + 1);
		std::ranges::copy(value, copy);
		return copy;
	}
}
