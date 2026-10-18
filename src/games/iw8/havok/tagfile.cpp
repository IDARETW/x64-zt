#include <std_include.hpp>
#include "tagfile.hpp"

namespace zonetool::iw8::havok
{
	namespace
	{
		constexpr std::uint32_t leaf_section = 0x40000000;
		constexpr std::uint32_t section_size_mask = 0x3FFFFFFF;
		constexpr std::uint8_t object_item = 0x10;
		constexpr std::uint8_t array_item = 0x20;
		constexpr std::uint64_t pointer_format = 6;
		constexpr std::uint64_t format_kind_mask = 0x1F;

		// Optional parts of a TBDY type body.
		constexpr std::uint32_t body_format = 0x01;
		constexpr std::uint32_t body_subtype = 0x02;
		constexpr std::uint32_t body_version = 0x04;
		constexpr std::uint32_t body_size_alignment = 0x08;
		constexpr std::uint32_t body_flags = 0x10;
		constexpr std::uint32_t body_members = 0x20;
		constexpr std::uint32_t body_known = 0x3F;

		struct section
		{
			std::string_view tag;
			std::size_t begin{};
			std::size_t end{};
		};

		std::uint32_t read_be32(const std::span<const std::uint8_t> data, const std::size_t offset)
		{
			return static_cast<std::uint32_t>(data[offset]) << 24 | static_cast<std::uint32_t>(data[offset + 1]) << 16 |
				static_cast<std::uint32_t>(data[offset + 2]) << 8 | data[offset + 3];
		}

		template <typename T>
		T read_value(const std::span<const std::uint8_t> data, const std::size_t offset)
		{
			if (offset + sizeof(T) > data.size())
			{
				throw std::runtime_error("the tagfile is truncated");
			}

			T value{};
			std::memcpy(&value, data.data() + offset, sizeof(T));
			return value;
		}

		// The child sections of [begin, end).
		std::vector<section> read_sections(const std::span<const std::uint8_t> data, const std::size_t begin, const std::size_t end)
		{
			std::vector<section> sections;
			for (auto offset = begin; offset + 8 <= end;)
			{
				const auto size = read_be32(data, offset) & section_size_mask;
				if (size < 8 || offset + size > end)
				{
					throw std::runtime_error("a tagfile section is invalid");
				}

				sections.emplace_back(std::string_view(reinterpret_cast<const char*>(data.data() + offset + 4), 4), offset + 8, offset + size);
				offset += size;
			}

			return sections;
		}

		const section& find_section(const std::vector<section>& sections, const std::string_view tag)
		{
			const auto found = std::ranges::find(sections, tag, &section::tag);
			if (found == sections.end())
			{
				throw std::runtime_error(std::format("the tagfile has no {} section", tag));
			}

			return *found;
		}

		class varint_reader
		{
		public:
			varint_reader(const std::span<const std::uint8_t> data, const std::size_t begin, const std::size_t end)
				: data_(data)
				, position_(begin)
				, end_(end)
			{
			}

			bool done() const
			{
				return this->position_ >= this->end_;
			}

			std::size_t position() const
			{
				return this->position_;
			}

			void skip(const std::size_t bytes)
			{
				if (this->position_ + bytes > this->end_)
				{
					throw std::runtime_error("a tagfile type table is truncated");
				}

				this->position_ += bytes;
			}

			std::uint64_t read()
			{
				if (this->position_ >= this->end_)
				{
					throw std::runtime_error("a tagfile type table is truncated");
				}

				const auto prefix = this->data_[this->position_++];
				std::size_t length{};
				std::uint64_t value{};
				if ((prefix & 0x80) == 0)
				{
					return prefix;
				}

				if ((prefix & 0xC0) == 0x80)
				{
					length = 2;
					value = prefix & 0x3F;
				}
				else if ((prefix & 0xE0) == 0xC0)
				{
					length = 3;
					value = prefix & 0x1F;
				}
				else if ((prefix & 0xF8) == 0xE0)
				{
					length = 4;
					value = prefix & 7;
				}
				else if ((prefix & 0xF8) == 0xE8)
				{
					length = 5;
					value = prefix & 7;
				}
				else if (prefix == 0xF8)
				{
					length = 6;
				}
				else if ((prefix & 0xF8) == 0xF0)
				{
					length = 8;
					value = prefix & 7;
				}
				else if (prefix == 0xF9)
				{
					length = 9;
				}
				else
				{
					throw std::runtime_error("a tagfile type table has an invalid integer");
				}

				if (this->position_ + length - 1 > this->end_)
				{
					throw std::runtime_error("a tagfile type table integer is truncated");
				}

				for (std::size_t i = 1; i < length; i++)
				{
					value = value << 8 | this->data_[this->position_++];
				}

				return value;
			}

			std::uint32_t read32()
			{
				const auto value = this->read();
				if (value > std::numeric_limits<std::uint32_t>::max())
				{
					throw std::runtime_error("a tagfile type table value is out of range");
				}

				return static_cast<std::uint32_t>(value);
			}

		private:
			std::span<const std::uint8_t> data_;
			std::size_t position_;
			std::size_t end_;
		};

		// The shortest varint encoding.
		void write_varint(std::vector<std::uint8_t>& out, const std::uint64_t value)
		{
			const auto put = [&out](const std::uint8_t prefix, const std::uint64_t rest, const std::size_t bytes)
			{
				out.push_back(prefix);
				for (auto i = bytes; i > 0; i--)
				{
					out.push_back(static_cast<std::uint8_t>(rest >> ((i - 1) * 8)));
				}
			};

			if (value < 0x80)
			{
				out.push_back(static_cast<std::uint8_t>(value));
			}
			else if (value < 0x4000)
			{
				put(static_cast<std::uint8_t>(0x80 | value >> 8), value, 1);
			}
			else if (value < 0x200000)
			{
				put(static_cast<std::uint8_t>(0xC0 | value >> 16), value, 2);
			}
			else if (value < 0x8000000)
			{
				put(static_cast<std::uint8_t>(0xE0 | value >> 24), value, 3);
			}
			else if (value < 0x800000000)
			{
				put(static_cast<std::uint8_t>(0xE8 | value >> 32), value, 4);
			}
			else if (value < 0x10000000000)
			{
				put(0xF8, value, 5);
			}
			else if (value < 0x800000000000000)
			{
				put(static_cast<std::uint8_t>(0xF0 | value >> 56), value, 7);
			}
			else
			{
				put(0xF9, value, 8);
			}
		}

		std::vector<std::string> read_strings(const std::span<const std::uint8_t> data, const section& strings)
		{
			std::vector<std::string> result;
			for (auto offset = strings.begin; offset < strings.end;)
			{
				const auto begin = reinterpret_cast<const char*>(data.data() + offset);
				const auto terminator = static_cast<const char*>(std::memchr(begin, 0, strings.end - offset));
				if (!terminator)
				{
					throw std::runtime_error("a tagfile string table is invalid");
				}

				result.emplace_back(begin, terminator);
				offset += static_cast<std::size_t>(terminator - begin) + 1;
			}

			return result;
		}

		void put_be32(std::vector<std::uint8_t>& out, const std::uint32_t value)
		{
			for (const auto shift : {24, 16, 8, 0})
			{
				out.push_back(static_cast<std::uint8_t>(value >> shift));
			}
		}

		template <typename T>
		void put_value(std::vector<std::uint8_t>& out, const T& value)
		{
			const auto bytes = reinterpret_cast<const std::uint8_t*>(&value);
			out.insert(out.end(), bytes, bytes + sizeof(T));
		}

		std::vector<std::uint8_t> make_section(const std::string_view tag, const std::span<const std::uint8_t> payload, const bool leaf)
		{
			std::vector<std::uint8_t> out;
			out.reserve(payload.size() + 8);
			put_be32(out, (leaf ? leaf_section : 0) | static_cast<std::uint32_t>(payload.size() + 8));
			out.insert(out.end(), tag.begin(), tag.end());
			out.insert(out.end(), payload.begin(), payload.end());
			return out;
		}

		void append(std::vector<std::uint8_t>& out, const std::span<const std::uint8_t> bytes)
		{
			out.insert(out.end(), bytes.begin(), bytes.end());
		}

		std::size_t align(const std::size_t value, const std::size_t alignment)
		{
			return alignment > 1 ? (value + alignment - 1) / alignment * alignment : value;
		}

		// The layout of the items: the number and the data offset that each one takes.
		struct layout
		{
			// new index -> old index
			std::vector<std::uint32_t> order;
			std::vector<std::uint32_t> numbers;
			std::vector<std::uint32_t> offsets;
			std::vector<std::uint8_t> flags;
			// old indices in the order of their data
			std::vector<std::uint32_t> emitted;
			std::size_t data_size{};
		};

		// The writer numbers and places the items as the Replay tools do: objects in number order, each followed by the
		// arrays that it reaches through arrays. In each item, pointers and relative arrays are numbered before arrays
		// and strings. Arrays align to 16 bytes, strings to 2 and objects to their type.
		layout lay_out(const tagfile& file, const type_table& types)
		{
			const auto count = file.items.size();
			layout result;
			result.numbers.assign(count, 0);
			result.offsets.assign(count, 0);
			result.flags.assign(count, 0);
			std::vector<bool> strings(count);
			result.order.push_back(0);

			const auto number = [&](const std::uint32_t old, const std::uint8_t flags)
			{
				if (old == 0 || result.numbers[old])
				{
					return false;
				}

				result.numbers[old] = static_cast<std::uint32_t>(result.order.size());
				result.order.push_back(old);
				result.flags[old] = flags;
				return true;
			};

			const auto visit = [&](const std::uint32_t old)
			{
				// Member by member over the elements of an array.
				std::vector<std::uint32_t> found;
				auto pointers = file.items[old].pointers;
				const auto element = std::max(types.size_of(file.items[old].type), 1u);
				std::ranges::sort(pointers, {}, [element](const pointer_field& pointer)
				{
					return std::pair(pointer.offset % element, pointer.offset / element);
				});
				for (const auto pass : {0, 1})
				{
					for (const auto& pointer : pointers)
					{
						const auto kind = types.kind_of(pointer.pointer_type);
						if ((kind == pointer_kind::array) != (pass == 1))
						{
							continue;
						}

						if (number(pointer.target, kind == pointer_kind::object ? object_item : array_item))
						{
							strings[pointer.target] = types.at(pointer.pointer_type).name == "hkStringPtr";
							found.push_back(pointer.target);
						}
					}
				}

				return found;
			};

			if (count < 2)
			{
				throw std::runtime_error("the tagfile has no root object");
			}

			number(1, object_item);
			std::size_t position{};
			for (std::size_t next = 1; next < result.order.size(); next++)
			{
				const auto old = result.order[next];
				if (result.flags[old] != object_item)
				{
					continue;
				}

				const auto& object = file.items[old];
				position = align(position, types.alignment_of(object.type));
				result.offsets[old] = static_cast<std::uint32_t>(position);
				position += object.data.size();
				result.emitted.push_back(old);

				std::deque<std::uint32_t> closure;
				for (const auto target : visit(old))
				{
					if (result.flags[target] == array_item)
					{
						closure.push_back(target);
					}
				}

				while (!closure.empty())
				{
					const auto array = closure.front();
					closure.pop_front();
					position = align(position, strings[array] ? 2 : 16);
					result.offsets[array] = static_cast<std::uint32_t>(position);
					position += file.items[array].data.size();
					result.emitted.push_back(array);
					for (const auto target : visit(array))
					{
						if (result.flags[target] == array_item)
						{
							closure.push_back(target);
						}
					}
				}
			}

			if (result.order.size() != count)
			{
				throw std::runtime_error("the tagfile has items that the root does not reach");
			}

			result.data_size = align(position, 16);
			return result;
		}

		struct written_index
		{
			std::vector<std::uint8_t> data;
			std::vector<std::uint8_t> items;
			// pointer type (in the index space of the caller) -> DATA offsets
			std::map<std::uint32_t, std::vector<std::uint32_t>> patches;
		};

		// DATA, ITEM and the patches of the items, with type indices that type_index maps.
		template <typename TypeIndex>
		written_index write_index(const tagfile& file, const layout& layout, TypeIndex&& type_index)
		{
			written_index result;
			result.data.assign(layout.data_size, 0);
			for (const auto old : layout.emitted)
			{
				const auto& item = file.items[old];
				const auto offset = layout.offsets[old];
				std::ranges::copy(item.data, result.data.begin() + offset);
				for (const auto& pointer : item.pointers)
				{
					const auto target = pointer.target ? layout.numbers[pointer.target] : 0;
					std::memcpy(result.data.data() + offset + pointer.offset, &target, sizeof(target));
					result.patches[type_index(pointer.pointer_type)].push_back(offset + pointer.offset);
				}
			}

			result.items.assign(12, 0);
			for (std::size_t index = 1; index < layout.order.size(); index++)
			{
				const auto old = layout.order[index];
				const auto& item = file.items[old];
				put_value(result.items, type_index(item.type) | static_cast<std::uint32_t>(layout.flags[old]) << 24);
				put_value(result.items, layout.offsets[old]);
				put_value(result.items, item.count);
			}

			for (auto& [type, offsets] : result.patches)
			{
				std::ranges::sort(offsets);
			}

			return result;
		}

		std::vector<std::uint8_t> write_patches(const std::map<std::uint32_t, std::vector<std::uint32_t>>& patches)
		{
			std::vector<std::uint8_t> out;
			for (const auto& [type, offsets] : patches)
			{
				put_value(out, type);
				put_value(out, static_cast<std::uint32_t>(offsets.size()));
				for (const auto offset : offsets)
				{
					put_value(out, offset);
				}
			}

			return out;
		}

		std::vector<std::uint8_t> write_root(const tagfile& file, const std::span<const std::uint8_t> data,
			const std::span<const std::uint8_t> types, const std::span<const std::uint8_t> items, const std::span<const std::uint8_t> patches)
		{
			std::vector<std::uint8_t> version(file.sdk_version.begin(), file.sdk_version.end());
			std::vector<std::uint8_t> body;
			append(body, make_section("SDKV", version, true));
			append(body, make_section("DATA", data, true));
			append(body, types);
			std::vector<std::uint8_t> index;
			append(index, make_section("ITEM", items, true));
			append(index, make_section("PTCH", patches, true));
			append(body, make_section("INDX", index, false));
			return make_section("TAG0", body, false);
		}
	}

	type_table type_table::read(const std::span<const std::uint8_t> tagfile)
	{
		if (tagfile.size() < 8 || std::memcmp(tagfile.data() + 4, "TAG0", 4) != 0)
		{
			throw std::runtime_error("the data is not a Havok tagfile");
		}

		const auto root = read_sections(tagfile, 8, read_be32(tagfile, 0) & section_size_mask);
		const auto& type = find_section(root, "TYPE");
		const auto parts = read_sections(tagfile, type.begin, type.end);
		const auto type_strings = read_strings(tagfile, find_section(parts, "TSTR"));
		const auto field_strings = read_strings(tagfile, find_section(parts, "FSTR"));

		const auto string_at = [](const std::vector<std::string>& strings, const std::uint64_t index) -> const std::string&
		{
			if (index >= strings.size())
			{
				throw std::runtime_error("a tagfile type names a string that does not exist");
			}

			return strings[index];
		};

		type_table table;
		const auto& names = find_section(parts, "TNA1");
		varint_reader reader(tagfile, names.begin, names.end);
		const auto count = reader.read32();
		if (count == 0)
		{
			throw std::runtime_error("the tagfile has no types");
		}

		table.types_.resize(count);
		for (std::uint32_t index = 1; index < count; index++)
		{
			auto& info = table.types_[index];
			info.name = string_at(type_strings, reader.read());
			const auto parameter_count = reader.read();
			for (std::uint64_t parameter = 0; parameter < parameter_count; parameter++)
			{
				const auto& name = string_at(type_strings, reader.read());
				info.parameters.emplace_back(name, reader.read());
			}
		}

		const auto check_type = [count](const std::uint32_t index)
		{
			if (index >= count)
			{
				throw std::runtime_error("a tagfile type names a type that does not exist");
			}

			return index;
		};

		const auto& bodies = find_section(parts, "TBDY");
		varint_reader body(tagfile, bodies.begin, bodies.end);
		while (!body.done())
		{
			const auto index = body.read32();
			if (index == 0)
			{
				continue;
			}

			auto& info = table.types_.at(check_type(index));
			info.has_body = true;
			info.parent = check_type(body.read32());
			info.optionals = body.read32();
			if (info.optionals & ~body_known)
			{
				throw std::runtime_error(std::format("type {} has unknown body parts {:#x}", info.name, info.optionals));
			}

			if (info.optionals & body_format)
			{
				info.format = body.read();
			}

			if (info.optionals & body_subtype)
			{
				info.subtype = check_type(body.read32());
			}

			if (info.optionals & body_version)
			{
				info.version = body.read();
			}

			if (info.optionals & body_size_alignment)
			{
				info.size = body.read32();
				info.alignment = body.read32();
			}

			if (info.optionals & body_flags)
			{
				info.flags = body.read();
			}

			if (info.optionals & body_members)
			{
				const auto word = body.read32();
				info.member_count_high = word >> 16;
				for (std::uint32_t member = 0; member < (word & 0xFFFF); member++)
				{
					type_member entry;
					entry.name = string_at(field_strings, body.read());
					entry.flags = body.read32();
					entry.offset = body.read32();
					entry.type = check_type(body.read32());
					info.members.push_back(std::move(entry));
				}
			}
		}

		const auto& hashes = find_section(parts, "THSH");
		varint_reader hash_reader(tagfile, hashes.begin, hashes.end);
		const auto hash_count = hash_reader.read();
		for (std::uint64_t hash = 0; hash < hash_count; hash++)
		{
			const auto index = check_type(hash_reader.read32());
			table.types_[index].hash = read_value<std::uint32_t>(tagfile, hash_reader.position());
			hash_reader.skip(4);
		}

		for (std::uint32_t index = 1; index < count; index++)
		{
			table.identities_.try_emplace(table.identity(index), index);
		}

		return table;
	}

	std::size_t type_table::size() const
	{
		return this->types_.size();
	}

	const type_info& type_table::at(const std::uint32_t index) const
	{
		return this->types_.at(index);
	}

	std::string type_table::identity(const std::uint32_t index) const
	{
		if (index == 0)
		{
			return "NULL";
		}

		const auto& info = this->types_.at(index);
		if (info.parameters.empty())
		{
			return info.name;
		}

		std::string identity = info.name + "<";
		for (std::size_t i = 0; i < info.parameters.size(); i++)
		{
			const auto& [name, value] = info.parameters[i];
			identity += i ? ", " : "";
			identity += name.starts_with('t') ? this->identity(static_cast<std::uint32_t>(value)) : std::to_string(value);
		}

		return identity + ">";
	}

	std::optional<std::uint32_t> type_table::find(const std::string_view identity) const
	{
		const auto found = this->identities_.find(std::string(identity));
		return found != this->identities_.end() ? std::optional(found->second) : std::nullopt;
	}

	std::optional<std::pair<std::uint32_t, std::uint32_t>> type_table::member(std::uint32_t index, const std::string_view name) const
	{
		const auto own = [this](std::uint32_t type, const std::string_view part) -> std::optional<std::pair<std::uint32_t, std::uint32_t>>
		{
			for (std::size_t depth = 0; type && depth < 64; depth++)
			{
				const auto& info = this->types_.at(type);
				for (const auto& entry : info.members)
				{
					if (entry.name == part)
					{
						return std::pair(entry.offset, entry.type);
					}
				}

				type = info.parent;
			}

			return {};
		};

		std::uint32_t offset{};
		for (std::size_t start = 0;;)
		{
			const auto end = name.find('.', start);
			const auto found = own(index, name.substr(start, end == std::string_view::npos ? std::string_view::npos : end - start));
			if (!found)
			{
				return {};
			}

			offset += found->first;
			index = found->second;
			if (end == std::string_view::npos)
			{
				return std::pair(offset, index);
			}

			start = end + 1;
		}
	}

	std::uint32_t type_table::size_of(std::uint32_t index) const
	{
		for (std::size_t depth = 0; index && depth < 64; depth++)
		{
			const auto& info = this->types_.at(index);
			if (info.optionals & body_size_alignment)
			{
				return info.size;
			}

			index = info.parent;
		}

		return 0;
	}

	std::uint32_t type_table::alignment_of(std::uint32_t index) const
	{
		for (std::size_t depth = 0; index && depth < 64; depth++)
		{
			const auto& info = this->types_.at(index);
			if (info.optionals & body_size_alignment)
			{
				return std::max(info.alignment, 1u);
			}

			index = info.parent;
		}

		return 1;
	}

	pointer_kind type_table::kind_of(const std::uint32_t pointer_type) const
	{
		const auto& info = this->types_.at(pointer_type);
		if (info.name == "hkRelArray")
		{
			return pointer_kind::relative_array;
		}

		if (info.name == "hkStringPtr")
		{
			return pointer_kind::array;
		}

		for (auto index = pointer_type; index;)
		{
			const auto& type = this->types_.at(index);
			if (type.optionals & body_format)
			{
				return (type.format & format_kind_mask) == pointer_format ? pointer_kind::object : pointer_kind::array;
			}

			index = type.parent;
		}

		throw std::runtime_error(std::format("{} is not a pointer type", this->identity(pointer_type)));
	}

	std::optional<std::uint64_t> compendium_reference(const std::span<const std::uint8_t> data)
	{
		if (data.size() < 8 || std::memcmp(data.data() + 4, "TAG0", 4) != 0)
		{
			throw std::runtime_error("the data is not a Havok tagfile");
		}

		const auto root = read_sections(data, 8, std::min<std::size_t>(read_be32(data, 0) & section_size_mask, data.size()));
		const auto reference = std::ranges::find(root, "TCRF", &section::tag);
		if (reference == root.end())
		{
			return {};
		}

		return read_value<std::uint64_t>(data, reference->begin);
	}

	tagfile read_tagfile(const std::span<const std::uint8_t> data, const type_table* compendium)
	{
		if (data.size() < 8 || std::memcmp(data.data() + 4, "TAG0", 4) != 0)
		{
			throw std::runtime_error("the data is not a Havok tagfile");
		}

		const auto end = read_be32(data, 0) & section_size_mask;
		if (end > data.size())
		{
			throw std::runtime_error("the tagfile is truncated");
		}

		const auto root = read_sections(data, 8, end);
		tagfile file;
		const auto& version = find_section(root, "SDKV");
		file.sdk_version.assign(reinterpret_cast<const char*>(data.data() + version.begin), version.end - version.begin);

		const type_table* types = compendium;
		if (std::ranges::find(root, "TYPE", &section::tag) != root.end())
		{
			file.types = type_table::read(data.subspan(0, end));
			types = &*file.types;
		}
		else
		{
			const auto& reference = find_section(root, "TCRF");
			file.compendium = read_value<std::uint64_t>(data, reference.begin);
		}

		if (!types)
		{
			throw std::runtime_error("the tagfile names a type compendium and none was given");
		}

		// Item and pointer types become compendium indices.
		const auto to_compendium = [&](const std::uint32_t index) -> std::uint32_t
		{
			if (!file.types || !index)
			{
				return index;
			}

			if (!compendium)
			{
				return index;
			}

			const auto identity = file.types->identity(index);
			const auto found = compendium->find(identity);
			if (!found)
			{
				throw unknown_type(std::format("the type compendium has no type {}", identity));
			}

			return *found;
		};

		const auto& payload = find_section(root, "DATA");
		const auto data_bytes = data.subspan(payload.begin, payload.end - payload.begin);
		const auto index = read_sections(data, find_section(root, "INDX").begin, find_section(root, "INDX").end);
		const auto& items = find_section(index, "ITEM");
		if ((items.end - items.begin) % 12)
		{
			throw std::runtime_error("the tagfile item table is invalid");
		}

		struct raw_item
		{
			std::uint32_t type;
			std::uint8_t flags;
			std::uint32_t offset;
			std::uint32_t count;
		};

		std::vector<raw_item> raw;
		for (auto offset = items.begin; offset < items.end; offset += 12)
		{
			const auto word = read_value<std::uint32_t>(data, offset);
			raw.emplace_back(word & 0xFFFFFF, static_cast<std::uint8_t>(word >> 24), read_value<std::uint32_t>(data, offset + 4),
				read_value<std::uint32_t>(data, offset + 8));
		}

		file.items.resize(raw.size());
		std::vector<std::pair<std::size_t, std::size_t>> spans;
		for (std::size_t i = 1; i < raw.size(); i++)
		{
			auto& item = file.items[i];
			item.type = to_compendium(raw[i].type);
			item.flags = raw[i].flags;
			item.count = raw[i].count;
			const auto size = static_cast<std::size_t>(types->size_of(raw[i].type)) * raw[i].count;
			if (raw[i].offset + size > data_bytes.size())
			{
				throw std::runtime_error(std::format("tagfile item {} is outside the data", i));
			}

			item.data.assign(data_bytes.begin() + raw[i].offset, data_bytes.begin() + raw[i].offset + size);
			if (size)
			{
				spans.emplace_back(raw[i].offset, i);
			}
		}

		std::ranges::sort(spans);
		const auto& patches = find_section(index, "PTCH");
		for (auto offset = patches.begin; offset < patches.end;)
		{
			const auto pointer_type = read_value<std::uint32_t>(data, offset);
			const auto count = read_value<std::uint32_t>(data, offset + 4);
			offset += 8;
			for (std::uint32_t i = 0; i < count; i++, offset += 4)
			{
				const auto at = read_value<std::uint32_t>(data, offset);
				const auto owner = std::upper_bound(spans.begin(), spans.end(), std::pair<std::size_t, std::size_t>(at, std::numeric_limits<std::size_t>::max()));
				if (owner == spans.begin())
				{
					throw std::runtime_error("a tagfile pointer is outside the items");
				}

				const auto item_index = std::prev(owner)->second;
				const auto relative = at - raw[item_index].offset;
				if (relative + 4 > file.items[item_index].data.size())
				{
					throw std::runtime_error("a tagfile pointer is outside its item");
				}

				const auto target = read_value<std::uint32_t>(data_bytes, at);
				if (target >= raw.size())
				{
					throw std::runtime_error("a tagfile pointer names an item that does not exist");
				}

				file.items[item_index].pointers.emplace_back(static_cast<std::uint32_t>(relative), to_compendium(pointer_type), target);
			}
		}

		for (auto& item : file.items)
		{
			std::ranges::sort(item.pointers, {}, &pointer_field::offset);
		}

		return file;
	}

	std::vector<std::uint8_t> write_compact_tagfile(const tagfile& file, const type_table& compendium)
	{
		const auto layout = lay_out(file, compendium);
		const auto index = write_index(file, layout, [](const std::uint32_t type)
		{
			return type;
		});

		std::vector<std::uint8_t> reference(24, 0);
		const auto signature = file.compendium.value_or(0);
		std::memcpy(reference.data(), &signature, sizeof(signature));
		return write_root(file, index.data, make_section("TCRF", reference, true), index.items, write_patches(index.patches));
	}

	std::vector<std::uint8_t> write_self_describing_tagfile(const tagfile& file, const type_table& compendium)
	{
		const auto layout = lay_out(file, compendium);

		// Local types: each item type in the order of the data, with the types that its type names (breadth first:
		// parent, subtype, type parameters, member types).
		std::vector<std::uint32_t> local{0};
		std::unordered_map<std::uint32_t, std::uint32_t> local_index{{0, 0}};
		const auto add = [&](const std::uint32_t type)
		{
			if (local_index.try_emplace(type, static_cast<std::uint32_t>(local.size())).second)
			{
				local.push_back(type);
			}
		};

		std::size_t processed = 1;
		const auto close = [&]
		{
			for (; processed < local.size(); processed++)
			{
				const auto& info = compendium.at(local[processed]);
				add(info.parent);
				add(info.subtype);
				for (const auto& [name, value] : info.parameters)
				{
					if (name.starts_with('t'))
					{
						add(static_cast<std::uint32_t>(value));
					}
				}

				for (const auto& member : info.members)
				{
					add(member.type);
				}
			}
		};

		for (const auto old : layout.emitted)
		{
			add(file.items[old].type);
			close();
		}

		for (const auto& item : file.items)
		{
			for (const auto& pointer : item.pointers)
			{
				add(pointer.pointer_type);
				close();
			}
		}

		const auto local_of = [&](const std::uint32_t type)
		{
			return local_index.at(type);
		};

		std::vector<std::string> type_strings;
		std::unordered_map<std::string, std::uint32_t> type_string_index;
		const auto type_string = [&](const std::string& value)
		{
			const auto [found, added] = type_string_index.try_emplace(value, static_cast<std::uint32_t>(type_strings.size()));
			if (added)
			{
				type_strings.push_back(value);
			}

			return found->second;
		};

		std::vector<std::uint8_t> names;
		write_varint(names, local.size());
		for (std::size_t i = 1; i < local.size(); i++)
		{
			const auto& info = compendium.at(local[i]);
			write_varint(names, type_string(info.name));
			write_varint(names, info.parameters.size());
			for (const auto& [name, value] : info.parameters)
			{
				write_varint(names, type_string(name));
				write_varint(names, name.starts_with('t') ? local_of(static_cast<std::uint32_t>(value)) : value);
			}
		}

		std::vector<std::string> field_strings;
		std::unordered_map<std::string, std::uint32_t> field_string_index;
		std::vector<std::uint8_t> bodies;
		for (std::size_t i = 1; i < local.size(); i++)
		{
			const auto& info = compendium.at(local[i]);
			if (!info.has_body)
			{
				continue;
			}

			write_varint(bodies, i);
			write_varint(bodies, local_of(info.parent));
			write_varint(bodies, info.optionals);
			if (info.optionals & body_format)
			{
				write_varint(bodies, info.format);
			}

			if (info.optionals & body_subtype)
			{
				write_varint(bodies, local_of(info.subtype));
			}

			if (info.optionals & body_version)
			{
				write_varint(bodies, info.version);
			}

			if (info.optionals & body_size_alignment)
			{
				write_varint(bodies, info.size);
				write_varint(bodies, info.alignment);
			}

			if (info.optionals & body_flags)
			{
				write_varint(bodies, info.flags);
			}

			if (info.optionals & body_members)
			{
				write_varint(bodies, static_cast<std::uint64_t>(info.member_count_high) << 16 | info.members.size());
				for (const auto& member : info.members)
				{
					const auto [found, added] = field_string_index.try_emplace(member.name, static_cast<std::uint32_t>(field_strings.size()));
					if (added)
					{
						field_strings.push_back(member.name);
					}

					write_varint(bodies, found->second);
					write_varint(bodies, member.flags);
					write_varint(bodies, member.offset);
					write_varint(bodies, local_of(member.type));
				}
			}
		}

		std::vector<std::uint8_t> hashes;
		std::vector<std::pair<std::uint32_t, std::uint32_t>> hashed;
		for (std::size_t i = 1; i < local.size(); i++)
		{
			if (const auto hash = compendium.at(local[i]).hash)
			{
				hashed.emplace_back(static_cast<std::uint32_t>(i), *hash);
			}
		}

		write_varint(hashes, hashed.size());
		for (const auto& [type, hash] : hashed)
		{
			write_varint(hashes, type);
			put_value(hashes, hash);
		}

		const auto join = [](const std::vector<std::string>& strings)
		{
			std::vector<std::uint8_t> out;
			for (const auto& value : strings)
			{
				out.insert(out.end(), value.begin(), value.end());
				out.push_back(0);
			}

			return out;
		};

		std::vector<std::uint8_t> type_section;
		append(type_section, make_section("TPTR", std::vector<std::uint8_t>(local.size() * 8, 0), true));
		append(type_section, make_section("TSTR", join(type_strings), true));
		append(type_section, make_section("TNA1", names, true));
		append(type_section, make_section("FSTR", join(field_strings), true));
		append(type_section, make_section("TBDY", bodies, true));
		append(type_section, make_section("THSH", hashes, true));
		append(type_section, make_section("TPAD", {}, true));

		const auto index = write_index(file, layout, local_of);
		return write_root(file, index.data, make_section("TYPE", type_section, false), index.items, write_patches(index.patches));
	}
}
