#pragma once

#include "resolver.hpp"
#include "../iw8.hpp"
#include "../zone/loader.hpp"

#include <zone/script_strings.hpp>
#include <zone/zone_memory.hpp>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstring>
#include <map>
#include <optional>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <variant>
#include <vector>

// The reads and writes that the generated adapters (tools/gen_iw8_neutral.py) share: strings, character buffers, enum
// names, arrays with a count, asset references by name, and the members that must hold nothing.
namespace zonetool::iw8::neutral::plain
{
	constexpr std::size_t rounded(const std::size_t size)
	{
		return (size + 7) & ~std::size_t(7);
	}

	// An upper bound for one allocation of this many bytes: the memory of a rebuild is sized in advance from these.
	constexpr std::size_t sized(const std::size_t size)
	{
		return rounded(size) + 32;
	}

	// The memory one asset is rebuilt in. Equal strings of an asset are one string, as in the zones the game ships.
	class context
	{
	public:
		explicit context(zone_memory& memory)
			: memory_(memory)
		{
		}

		template <typename T>
		T* allocate(const std::size_t count = 1)
		{
			return this->memory_.allocate<T>(count);
		}

		template <typename T>
		T* manual_allocate(const std::size_t size, const std::size_t count = 1)
		{
			return this->memory_.manual_allocate<T>(size, count);
		}

		char* text(const std::string& value)
		{
			auto& pooled = this->strings_[value];
			if (!pooled)
			{
				auto* copy = this->memory_.manual_allocate<char>(rounded(value.size() + 1));
				std::memcpy(copy, value.data(), value.size());
				pooled = copy;
			}

			return pooled;
		}

	private:
		zone_memory& memory_;
		std::map<std::string, char*> strings_;
	};

	// The type of the module that an event list or a module member is read or written in, for the structs whose meaning
	// depends on it (the generated tagged unions set it).
	class module_scope
	{
	public:
		explicit module_scope(const std::uint32_t type)
			: previous_(current_)
		{
			current_ = type;
		}

		~module_scope()
		{
			current_ = previous_;
		}

		module_scope(const module_scope&) = delete;
		module_scope& operator=(const module_scope&) = delete;

		static std::uint32_t current()
		{
			return current_;
		}

	private:
		inline static thread_local std::uint32_t current_ = 0xFFFFFFFFu;
		std::uint32_t previous_;
	};

	// Set by the verifier while it reads an asset whose type IW8_NEUTRAL_TRACE names: each generated reader logs its name, so
	// that a crash in an adapter shows the struct it was reading.
	inline bool trace_enabled = false;

	inline void trace(const char* what)
	{
		if (trace_enabled)
		{
			std::fprintf(stderr, "  %s\n", what);
			std::fflush(stderr);
		}
	}

	[[noreturn]] inline void fail(const std::string& what, const char* problem)
	{
		throw std::runtime_error(what + " " + problem);
	}

	// A character buffer holds a zero-terminated text; whatever follows the terminator has to be zero.
	template <std::size_t Size>
	std::string read_text(const char (&buffer)[Size], const char* what)
	{
		const auto* end = static_cast<const char*>(std::memchr(buffer, 0, Size));
		if (!end)
		{
			fail(what, "has no terminator in its character buffer");
		}

		for (auto* byte = end; byte < buffer + Size; ++byte)
		{
			if (*byte)
			{
				fail(what, "has data after the terminator of its character buffer");
			}
		}

		return std::string(buffer, end);
	}

	template <std::size_t Size>
	void write_text(char (&buffer)[Size], const std::string& text, const char* what)
	{
		if (text.size() >= Size || text.find('\0') != std::string::npos)
		{
			fail(what, "has a text that does not fit its character buffer");
		}

		std::memset(buffer, 0, Size);
		std::memcpy(buffer, text.data(), text.size());
	}

	inline std::optional<std::string> read_string(const char* text)
	{
		return text ? std::optional<std::string>(text) : std::nullopt;
	}

	inline char* write_string(context& memory, const std::optional<std::string>& text)
	{
		return text ? memory.text(*text) : nullptr;
	}

	// A script string is a number that names a string of the game's table: the neutral data holds the text, and
	// the number is made again when the asset is written.
	inline std::optional<std::string> read_script_string(const scr_string_t id)
	{
		const auto* text = SL_ConvertToString(id);
		return text ? std::optional<std::string>(text) : std::nullopt;
	}

	inline std::uint32_t write_script_string(const std::optional<std::string>& text)
	{
		return text ? static_cast<std::uint32_t>(SL_GetString(text->data())) : 0;
	}

	inline std::size_t string_size(const std::optional<std::string>& text)
	{
		return text ? rounded(text->size() + 1) : 0;
	}

	inline std::string enum_name(const std::span<const std::pair<std::int64_t, std::string_view>> table, const std::int64_t value,
		const char* what)
	{
		for (const auto& [number, name] : table)
		{
			if (number == value)
			{
				return std::string(name);
			}
		}

		fail(what, "has an enum value without a name");
		return {};
	}

	inline std::int64_t enum_value(const std::span<const std::pair<std::int64_t, std::string_view>> table, const std::string& name,
		const char* what)
	{
		for (const auto& [number, text] : table)
		{
			if (text == name)
			{
				return number;
			}
		}

		fail(what, "has an enum name that is not known");
		return 0;
	}

	// A set of flags as the names of the bits that are set; a bit without a name cannot be carried.
	inline std::vector<std::string> flag_names(const std::span<const std::pair<std::int64_t, std::string_view>> table,
		std::uint64_t value, const char* what)
	{
		std::vector<std::string> names;
		for (const auto& [number, name] : table)
		{
			const auto bits = static_cast<std::uint64_t>(number);
			// Only a single bit names a flag: a name for several bits is a shorthand for them.
			if (bits && (bits & (bits - 1)) == 0 && (value & bits) == bits)
			{
				names.emplace_back(name);
				value &= ~bits;
			}
		}

		if (value)
		{
			fail(what, "has flag bits without a name");
		}

		return names;
	}

	inline std::uint64_t flag_value(const std::span<const std::pair<std::int64_t, std::string_view>> table,
		const std::vector<std::string>& names, const char* what)
	{
		std::uint64_t value = 0;
		for (const auto& name : names)
		{
			value |= static_cast<std::uint64_t>(enum_value(table, name, what));
		}

		return value;
	}

	// The scalars of a vector type (a union or a register type) or of an overlay: copied as they are, so that a NaN keeps
	// its bits.
	template <typename Scalar, std::size_t Count>
	std::array<Scalar, Count> read_scalars(const void* vector)
	{
		std::array<Scalar, Count> out;
		std::memcpy(out.data(), vector, sizeof(out));
		return out;
	}

	template <typename Scalar, std::size_t Count>
	void write_scalars(void* vector, const std::array<Scalar, Count>& values)
	{
		std::memcpy(vector, values.data(), sizeof(values));
	}

	// A block of scalars of more than one dimension: the neutral type is nested arrays of the same layout.
	template <typename Block>
	Block read_block(const void* block)
	{
		Block out;
		std::memcpy(&out, block, sizeof(out));
		return out;
	}

	template <typename Block>
	void write_block(void* block, const Block& values)
	{
		std::memcpy(block, &values, sizeof(values));
	}

	// A pointer that should alias a member of its owner and does not: where it points, in bytes from that member.
	[[noreturn]] inline void fail_alias(const std::string& what, const void* actual, const void* expected)
	{
		throw std::runtime_error(what + " has a pointer " + std::to_string(reinterpret_cast<std::intptr_t>(actual) -
			reinterpret_cast<std::intptr_t>(expected)) + " bytes from the member of its owner that it should be");
	}

	// A pointer member that no stream rule describes holds nothing in a zone; a value would be lost.
	template <typename T>
	void require_zero(const T& value, const char* what)
	{
		if constexpr (std::is_pointer_v<T>)
		{
			if (value)
			{
				fail(what, "points at data that no stream rule describes");
			}
		}
		else if constexpr (std::is_array_v<T>)
		{
			for (const auto& item : value)
			{
				require_zero(item, what);
			}
		}
		else
		{
			std::array<std::uint8_t, sizeof(T)> bytes{};
			std::memcpy(bytes.data(), &value, sizeof(T));
			if (std::ranges::any_of(bytes, [](const std::uint8_t byte) { return byte != 0; }))
			{
				fail(what, "holds a value in a member that is padding or unused");
			}
		}
	}

	// The elements of an array member. The pointer is null when the asset has none; a count without an array is an error.
	template <typename Element, typename Count, typename Read>
	auto read_array(const Element* elements, const Count count, const char* what, Read&& read)
		-> std::optional<std::vector<decltype(read(*elements))>>
	{
		if (!elements)
		{
			if (count)
			{
				fail(what, "has a count and no array");
			}

			return std::nullopt;
		}

		std::vector<decltype(read(*elements))> out;
		out.reserve(static_cast<std::size_t>(count));
		for (std::size_t index = 0; index < static_cast<std::size_t>(count); ++index)
		{
			out.push_back(read(elements[index]));
		}

		return out;
	}

	// A pointer to a struct that is one slot of an array: null is an empty slot.
	template <typename Pointee, typename Read>
	auto read_pointee(const Pointee* pointee, Read&& read) -> std::optional<decltype(read(*pointee))>
	{
		if (!pointee)
		{
			return std::nullopt;
		}

		return read(*pointee);
	}

	template <typename Pointee, typename Item, typename Write>
	Pointee* write_pointee(const std::optional<Item>& item, context& memory, Write&& write)
	{
		if (!item)
		{
			return nullptr;
		}

		auto* pointee = memory.manual_allocate<Pointee>(rounded(sizeof(Pointee)));
		write(*pointee, *item);
		return pointee;
	}

	// A fixed number of elements behind a pointer, each read by the caller.
	template <std::size_t Count, typename Element, typename Read>
	auto read_fixed_with(const Element* elements, Read&& read) -> std::optional<std::array<decltype(read(*elements)), Count>>
	{
		if (!elements)
		{
			return std::nullopt;
		}

		std::array<decltype(read(*elements)), Count> out;
		for (std::size_t index = 0; index < Count; ++index)
		{
			out[index] = read(elements[index]);
		}

		return out;
	}

	// A pointer to one object.
	template <typename Element, typename Read>
	auto read_single(const Element* element, Read&& read) -> std::optional<decltype(read(*element))>
	{
		if (!element)
		{
			return std::nullopt;
		}

		return read(*element);
	}

	template <typename Element, std::size_t Count>
	std::optional<std::array<Element, Count>> read_fixed(const Element* elements)
	{
		if (!elements)
		{
			return std::nullopt;
		}

		std::array<Element, Count> out;
		std::copy_n(elements, Count, out.begin());
		return out;
	}

	template <typename Element, typename Items, typename Write>
	Element* write_array(const std::optional<Items>& items, context& memory, Write&& write)
	{
		if (!items)
		{
			return nullptr;
		}

		auto* elements = memory.manual_allocate<Element>(rounded(std::max<std::size_t>(items->size(), 1) * sizeof(Element)));
		for (std::size_t index = 0; index < items->size(); ++index)
		{
			write(elements[index], (*items)[index]);
		}

		return elements;
	}

	template <typename Element, typename Item, typename Write>
	Element* write_single(const std::optional<Item>& item, context& memory, Write&& write)
	{
		if (!item)
		{
			return nullptr;
		}

		auto* element = memory.manual_allocate<Element>(rounded(sizeof(Element)));
		write(*element, *item);
		return element;
	}

	template <typename Count, typename Items>
	Count count_of(const std::optional<Items>& items)
	{
		return items ? static_cast<Count>(items->size()) : Count{};
	}

	// The definition name of the asset that a pointer holds (a reference stub's name without its comma).
	inline std::optional<std::string> read_asset(const std::int32_t type, const void* header)
	{
		if (!header)
		{
			return std::nullopt;
		}

		const auto* name = get_asset_name(type, header);
		if (!name || !*name)
		{
			fail("an asset reference", "points at an asset without a name");
		}

		return name[0] == ',' ? name + 1 : name;
	}

	template <typename Header>
	Header* resolve_asset(const asset_resolver& resolve, const std::int32_t type, const std::optional<std::string>& name)
	{
		if (!name)
		{
			return nullptr;
		}

		auto* header = static_cast<Header*>(resolve(type, *name));
		if (!header)
		{
			fail("the referenced asset " + *name, "is absent");
		}

		return header;
	}
}
