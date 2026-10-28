#include <std_include.hpp>
#include "text.hpp"
#include "../zone/asset_files.hpp"
#include "../zone/replay_netconst.h"

#include <formats/compression.hpp>
#include <zone/zone_memory.hpp>

#include <zlib.h>

namespace zonetool::iw8::neutral
{
	namespace
	{
		// zone_memory is a byte allocator: rounding every allocation to 8 keeps each later record aligned.
		constexpr std::size_t rounded(const std::size_t size)
		{
			return (size + 7) & ~std::size_t(7);
		}

		const char* copy_text(zone_memory& memory, const std::string& text)
		{
			auto* copy = memory.manual_allocate<char>(rounded(text.size() + 1));
			std::ranges::copy(text, copy);
			return copy;
		}

		std::string native_name(const char* name, const char* what)
		{
			if (!name)
			{
				throw std::runtime_error(std::string("native ") + what + " has no name");
			}

			return name;
		}

		// NETCONSTSTRINGTYPE_NONE: the type of the placeholder table (void) that code_post_gfx holds.
		constexpr auto no_category = "none";

		// NetConstStringSource values in Replay order, named as the canonical scopes.
		constexpr std::array<std::pair<NetConstStringSource, const char*>, 6> scopes{{
			{NETCONSTSTRINGSOURCE_GLOBAL, "global"},
			{NETCONSTSTRINGSOURCE_COMMON, "common"},
			{NETCONSTSTRINGSOURCE_PRE_MAP, "before_map"},
			{NETCONSTSTRINGSOURCE_MAP, "map"},
			{NETCONSTSTRINGSOURCE_MYCHANGES, "changes"},
			{NETCONSTSTRINGSOURCE_NONE, "none"},
		}};
	}

	asset_data::localize_entry to_neutral(const LocalizeEntry& native)
	{
		asset_data::localize_entry data{native_name(native.name, "LocalizeEntry"), {}};
		if (native.value)
		{
			data.value = native.value;
		}

		asset_data::validate(data);
		return data;
	}

	asset_data::net_const_strings to_neutral(const NetConstStrings& native)
	{
		asset_data::net_const_strings data;
		data.name = native_name(native.name, "NetConstStrings");
		if (native.stringType > NETCONSTSTRINGTYPE_NONE)
		{
			throw std::runtime_error(std::format("NetConstStrings {} has unknown string type {}", data.name,
				static_cast<std::uint32_t>(native.stringType)));
		}

		data.category = native.stringType == NETCONSTSTRINGTYPE_NONE ? no_category : replayncs::Tags[native.stringType];
		const auto scope = std::ranges::find(scopes, native.sourceType, &decltype(scopes)::value_type::first);
		if (scope == scopes.end())
		{
			throw std::runtime_error(std::format("NetConstStrings {} has unknown source {}", data.name,
				static_cast<std::uint32_t>(native.sourceType)));
		}

		data.scope = scope->second;
		// No mapping is known for the flags: stock tables hold none.
		if (native.flags)
		{
			throw std::runtime_error(std::format("NetConstStrings {} has unmapped flags 0x{:X}", data.name, native.flags));
		}

		if (native.stringList)
		{
			auto& strings = data.strings.emplace();
			for (std::uint32_t i = 0; i < native.entryCount; i++)
			{
				strings.push_back(native.stringList[i] ? std::optional<std::string>(native.stringList[i]) : std::nullopt);
			}
		}
		else if (native.entryCount)
		{
			throw std::runtime_error(std::format("NetConstStrings {} counts {} entries without a list", data.name,
				native.entryCount));
		}

		asset_data::validate(data);
		return data;
	}

	asset_data::raw_file to_neutral(const RawFile& native)
	{
		asset_data::raw_file data{native_name(native.name, "RawFile"), {}};
		if (native.buffer)
		{
			auto& bytes = data.bytes.emplace(native.len);
			auto size = static_cast<uLongf>(bytes.size());
			if (uncompress(bytes.data(), &size, reinterpret_cast<const Bytef*>(native.buffer), native.compressedLen) != Z_OK ||
				size != bytes.size())
			{
				throw std::runtime_error(std::format("RawFile {} does not decompress to its length", data.name));
			}
		}
		else if (native.len || native.compressedLen)
		{
			throw std::runtime_error(std::format("RawFile {} has lengths without a buffer", data.name));
		}

		asset_data::validate(data);
		return data;
	}

	LocalizeEntry* from_neutral(const asset_data::localize_entry& data, zone_memory& memory)
	{
		asset_data::validate(data);
		auto* native = memory.allocate<LocalizeEntry>();
		native->name = copy_text(memory, data.name);
		native->value = data.value ? copy_text(memory, *data.value) : nullptr;
		return native;
	}

	NetConstStrings* from_neutral(const asset_data::net_const_strings& data, zone_memory& memory)
	{
		asset_data::validate(data);
		const auto tag = std::ranges::find(replayncs::Tags, data.category);
		if (tag == std::end(replayncs::Tags) && data.category != no_category)
		{
			throw std::runtime_error("NetConstStrings category has no Replay string type: " + data.category);
		}

		const auto scope = std::ranges::find(scopes, data.scope, &decltype(scopes)::value_type::second);
		if (scope == scopes.end())
		{
			throw std::runtime_error("NetConstStrings scope has no Replay source: " + data.scope);
		}

		auto* native = memory.allocate<NetConstStrings>();
		native->stringType = static_cast<NetConstStringType>(tag - std::begin(replayncs::Tags));
		native->sourceType = scope->first;
		if (data.strings)
		{
			// A present empty list keeps a list pointer.
			native->entryCount = static_cast<std::uint32_t>(data.strings->size());
			auto* list = memory.allocate<const char*>(std::max<std::size_t>(data.strings->size(), 1));
			for (std::size_t i = 0; i < data.strings->size(); i++)
			{
				const auto& entry = (*data.strings)[i];
				list[i] = entry ? copy_text(memory, *entry) : nullptr;
			}

			native->stringList = list;
		}

		native->name = copy_text(memory, data.name);
		return native;
	}

	RawFile* from_neutral(const asset_data::raw_file& data, zone_memory& memory)
	{
		asset_data::validate(data);
		auto* native = memory.allocate<RawFile>();
		if (data.bytes)
		{
			const auto compressed = compression::compress_zlib(data.bytes->data(), data.bytes->size());
			auto* buffer = memory.manual_allocate<char>(rounded(std::max<std::size_t>(compressed.size(), 1)));
			std::ranges::copy(compressed, buffer);
			native->buffer = buffer;
			native->compressedLen = static_cast<std::uint32_t>(compressed.size());
			native->len = static_cast<std::uint32_t>(data.bytes->size());
		}

		native->name = copy_text(memory, data.name);
		return native;
	}

	std::expected<asset_data::string_table, std::string> to_neutral(const StringTable& native)
	{
		const auto cells = static_cast<std::size_t>(std::max(native.rowCount, 0)) * std::max(native.columnCount, 0);
		if (!native.name || native.rowCount < 0 || native.columnCount < 0 || native.uniqueCellCount < 0 ||
			(cells && (!native.cellIndices || !native.strings || !native.hashes)))
		{
			return std::unexpected("the table has no name, a negative count or cells without data");
		}

		asset_data::string_table data{native.name, static_cast<std::uint32_t>(native.columnCount),
			static_cast<std::uint32_t>(native.rowCount), {}};
		for (std::size_t cell = 0; cell < cells; ++cell)
		{
			const auto index = native.cellIndices[cell];
			if (index >= native.uniqueCellCount)
			{
				return std::unexpected(std::format("cell {} has the string index {} of {}", cell, index, native.uniqueCellCount));
			}

			const auto* value = native.strings[index];
			if (native.hashes[index] != (value ? string_table_hash(value) : 0))
			{
				return std::unexpected(std::format("string {} has a hash that is not Replay's cell hash", index));
			}

			data.cells.push_back(value ? std::optional<std::string>(value) : std::nullopt);
		}

		asset_data::validate(data);
		return data;
	}

	// Stock tables keep the unique strings in the order of their first cell.
	StringTable* from_neutral(const asset_data::string_table& data, zone_memory& memory)
	{
		asset_data::validate(data);
		std::map<std::optional<std::string>, std::uint16_t> unique;
		std::vector<const std::optional<std::string>*> strings;
		std::vector<std::uint16_t> indices;
		for (const auto& cell : data.cells)
		{
			const auto [entry, added] = unique.try_emplace(cell, static_cast<std::uint16_t>(strings.size()));
			if (added)
			{
				if (strings.size() > std::numeric_limits<std::uint16_t>::max())
				{
					throw std::runtime_error("stringtable " + data.name + " has more than 65536 unique strings");
				}

				strings.push_back(&entry->first);
			}

			indices.push_back(entry->second);
		}

		if (data.cells.size() > static_cast<std::size_t>(std::numeric_limits<std::int32_t>::max()) ||
			data.rows > static_cast<std::uint32_t>(std::numeric_limits<std::int32_t>::max()) ||
			data.columns > static_cast<std::uint32_t>(std::numeric_limits<std::int32_t>::max()))
		{
			throw std::runtime_error("stringtable " + data.name + " exceeds Replay's counts");
		}

		auto* native = memory.allocate<StringTable>();
		native->name = copy_text(memory, data.name);
		native->columnCount = static_cast<std::int32_t>(data.columns);
		native->rowCount = static_cast<std::int32_t>(data.rows);
		native->uniqueCellCount = static_cast<std::int32_t>(strings.size());
		// The arrays are present even without cells, as in every empty stock table: a table without cells keeps zero-length
		// arrays, and each gets its own address.
		native->cellIndices = memory.manual_allocate<std::uint16_t>(rounded(std::max<std::size_t>(indices.size() * sizeof(std::uint16_t), 1)));
		std::ranges::copy(indices, native->cellIndices);
		native->hashes = memory.manual_allocate<std::int32_t>(rounded(std::max<std::size_t>(strings.size() * sizeof(std::int32_t), 1)));
		native->strings = memory.manual_allocate<const char*>(rounded(std::max<std::size_t>(strings.size() * sizeof(const char*), 1)));
		for (std::size_t index = 0; index < strings.size(); ++index)
		{
			if (const auto& value = *strings[index])
			{
				native->strings[index] = copy_text(memory, *value);
				native->hashes[index] = string_table_hash(*value);
			}
		}

		return native;
	}

	std::size_t native_size(const asset_data::localize_entry& data)
	{
		return sizeof(LocalizeEntry) + rounded(data.name.size() + 1) + (data.value ? rounded(data.value->size() + 1) : 0);
	}

	std::size_t native_size(const asset_data::net_const_strings& data)
	{
		auto size = sizeof(NetConstStrings) + rounded(data.name.size() + 1);
		if (data.strings)
		{
			size += sizeof(const char*) * std::max<std::size_t>(data.strings->size(), 1);
			for (const auto& entry : *data.strings)
			{
				size += entry ? rounded(entry->size() + 1) : 0;
			}
		}

		return size;
	}

	std::size_t native_size(const asset_data::string_table& data)
	{
		auto size = sizeof(StringTable) + rounded(data.name.size() + 1) + rounded(data.cells.size() * sizeof(std::uint16_t));
		for (const auto& cell : data.cells)
		{
			size += sizeof(std::int32_t) + sizeof(const char*) + (cell ? rounded(cell->size() + 1) : 0);
		}

		return size + 32;
	}

	std::size_t native_size(const asset_data::raw_file& data)
	{
		const auto bytes = data.bytes ? rounded(std::max<std::size_t>(compressBound(static_cast<uLong>(data.bytes->size())), 1)) : 0;
		return sizeof(RawFile) + rounded(data.name.size() + 1) + bytes;
	}
}
