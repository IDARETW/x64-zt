#pragma once

namespace zonetool::iw8
{
	// Pointer fields and their values in a zone, for the values that a writer does not choose by itself: shared
	// data (-1), inserted data (-3), followed assets (-2) and offsets. A zone that is written again keeps them.
	using pointer_markers = std::unordered_map<const void*, std::uint64_t>;

	// The asset traversals of zone/asset_streams.cpp call these steps. load_stream maps them to zone_reader.
	class load_stream
	{
	public:
		explicit load_stream(zone_reader& reader, std::vector<std::pair<const void*, std::uint64_t>>* markers = nullptr)
			: reader_(reader)
			, markers_(markers)
		{
		}

		zone_reader& reader()
		{
			return this->reader_;
		}

		void push(const std::uint8_t block)
		{
			this->reader_.push_stream(block);
		}

		void pop()
		{
			this->reader_.pop_stream();
		}

		void align(const std::size_t alignment)
		{
			this->reader_.align(alignment);
		}

		void begin_asset()
		{
			this->reader_.begin_asset();
		}

		void end_asset()
		{
			this->reader_.end_asset();
		}

		void bytes(const std::size_t size)
		{
			this->reader_.read_bytes(size);
		}

		template <typename T>
		T* array(T*& field, const std::size_t alignment, const std::size_t count)
		{
			this->note(field, false);
			return this->reader_.read_array(field, alignment, count);
		}

		template <typename T>
		T* single(T*& field, const std::size_t alignment)
		{
			this->note(field, false);
			return this->reader_.read_single(field, alignment);
		}

		template <typename T>
		std::uint8_t* elements(T*& field, const std::size_t alignment, const std::size_t element_size, const std::size_t count)
		{
			this->note(field, false);
			return this->reader_.read_stream(field, alignment, element_size, count);
		}

		template <typename T>
		void string(T*& field)
		{
			this->note(field, false);
			this->reader_.read_string(field);
		}

		template <typename T>
		void asset(const std::int32_t type, T*& field)
		{
			this->note(field, true);
			this->reader_.read_asset(type, field);
		}

		template <typename T>
		void script_string(T& field)
		{
			this->reader_.read_script_string(field);
		}

		template <typename T>
		void script_strings(T* fields, const std::size_t count)
		{
			this->reader_.read_script_strings(fields, count);
		}

		template <typename T, typename Value>
		T temp(const std::size_t alignment, Value&&)
		{
			return this->reader_.read_temp<T>(alignment);
		}

		template <typename T>
		T missing_temp()
		{
			throw std::runtime_error("the stream has no value for a temp count");
		}

		void unsupported(const char* traversal) const
		{
			this->reader_.unsupported(traversal);
		}

	private:
		zone_reader& reader_;
		std::vector<std::pair<const void*, std::uint64_t>>* markers_;

		template <typename T>
		void note(T* const& field, const bool asset)
		{
			const auto value = reinterpret_cast<std::uint64_t>(field);
			if (this->markers_ && value && (asset || value != 0xFFFFFFFFFFFFFFFE))
			{
				this->markers_->emplace_back(&field, value);
			}
		}
	};

	// Maps the steps to zone_writer: the data of the source objects is copied, and each pointer field of a copy
	// gets the marker that the loader follows, or the offset of data that the zone holds already.
	class write_stream
	{
	public:
		// Writes the asset that a field of a source object points to, and sets the field in its copy.
		using asset_writer = std::function<void(write_stream& stream, std::int32_t type, void** field)>;
		// The zone index of a script string.
		using script_string_mapper = std::function<std::uint32_t(std::uint32_t id)>;
		// The marker of a source field that a zone written again keeps, if any.
		using marker_source = std::function<std::optional<std::uint64_t>(const void* field)>;

		static constexpr std::uint64_t following = 0xFFFFFFFFFFFFFFFE;
		static constexpr std::uint64_t insert = 0xFFFFFFFFFFFFFFFD;
		static constexpr std::uint8_t temp_block = 1;
		static constexpr std::uint8_t virtual_block = 5;

		write_stream(zone_writer& writer, asset_writer assets, script_string_mapper script_strings, marker_source markers = {})
			: writer_(writer)
			, assets_(std::move(assets))
			, script_strings_(std::move(script_strings))
			, markers_(std::move(markers))
		{
		}

		std::optional<std::uint64_t> marker_of(const void* field) const
		{
			return this->markers_ ? this->markers_(field) : std::nullopt;
		}

		// A kept value that is an offset, not a marker of inline data.
		std::optional<std::uint64_t> kept_offset(const std::uint64_t value) const
		{
			return this->writer_.is_offset(value) ? std::optional(value) : std::nullopt;
		}

		zone_writer& writer()
		{
			return this->writer_;
		}

		void push(const std::uint8_t block)
		{
			this->writer_.push_stream(block);
		}

		void pop()
		{
			this->writer_.pop_stream();
		}

		void align(const std::size_t alignment)
		{
			this->writer_.align(alignment);
		}

		void begin_asset()
		{
			this->writer_.begin_asset();
		}

		void end_asset()
		{
			this->writer_.end_asset();
		}

		void bytes(const std::size_t size)
		{
			this->writer_.write_continuation(size);
		}

		template <typename T>
		T* array(T*& field, const std::size_t alignment, const std::size_t count)
		{
			if (!field)
			{
				return nullptr;
			}

			const auto size = sizeof(T) * count;
			const auto marker = this->marker_of(&field);
			if (const auto offset = marker ? this->kept_offset(*marker) : this->writer_.find_copy(field, size))
			{
				this->writer_.set_pointer(&field, *offset);
				return nullptr;
			}

			this->writer_.set_pointer(&field, marker.value_or(following));
			this->writer_.align(alignment);
			this->writer_.write_copy(field, size);
			return field;
		}

		template <typename T>
		T* single(T*& field, const std::size_t alignment)
		{
			return this->array(field, alignment, 1);
		}

		template <typename T>
		std::uint8_t* elements(T*& field, const std::size_t alignment, const std::size_t element_size, const std::size_t count)
		{
			auto& bytes = reinterpret_cast<std::uint8_t*&>(field);
			return this->array(bytes, alignment, element_size * count);
		}

		template <typename T>
		void string(T*& field)
		{
			static_assert(sizeof(T) == 1);

			if (!field)
			{
				return;
			}

			const auto size = std::strlen(reinterpret_cast<const char*>(field)) + 1;
			const auto marker = this->marker_of(&field);
			if (const auto offset = marker ? this->kept_offset(*marker) : this->writer_.find_copy(field, size))
			{
				this->writer_.set_pointer(&field, *offset);
				return;
			}

			this->writer_.set_pointer(&field, marker.value_or(following));
			this->writer_.write_copy(field, size);
		}

		template <typename T>
		void asset(const std::int32_t type, T*& field)
		{
			this->writer_.push_stream(temp_block);
			if (field)
			{
				this->assets_(*this, type, reinterpret_cast<void**>(const_cast<std::remove_const_t<T>**>(&field)));
			}

			this->writer_.pop_stream();
		}

		template <typename T>
		void script_string(T& field)
		{
			static_assert(sizeof(T) <= sizeof(std::uint32_t));
			const auto index = static_cast<T>(this->script_strings_(static_cast<std::uint32_t>(field)));
			this->writer_.set_field(&field, &index, sizeof(T));
		}

		template <typename T>
		void script_strings(T* fields, const std::size_t count)
		{
			for (std::size_t i = 0; i < count; i++)
			{
				this->script_string(fields[i]);
			}
		}

		template <typename T, typename Value>
		T temp(const std::size_t alignment, Value&& value)
		{
			const T result = value();
			this->writer_.push_stream(temp_block);
			this->writer_.align(alignment);
			this->writer_.write_bytes(&result, sizeof(T));
			this->writer_.pop_stream();
			return result;
		}

		template <typename T>
		T missing_temp()
		{
			throw std::runtime_error("the writer has no value for a temp count");
		}

		void unsupported(const char* traversal) const
		{
			throw std::runtime_error(std::format("the zone writer does not support {}", traversal));
		}

	private:
		zone_writer& writer_;
		asset_writer assets_;
		script_string_mapper script_strings_;
		marker_source markers_;
	};

	// The IW8 asset dump: the records of one asset in traversal order. parse_stream reads them back with the same
	// traversal. A data record holds the bytes of an array with the inline data that continues it; an offset record
	// points into an earlier record; the other records hold a string, an asset name, a script string or a count.
	enum class dump_record : std::uint8_t
	{
		null,
		data,
		runtime,
		offset,
		string,
		asset,
		script_string,
		temp,
	};

	constexpr std::uint32_t dump_magic = 0x44385749; // "IW8D"
	constexpr std::uint32_t dump_version = 1;

	class dump_stream
	{
	public:
		static constexpr std::uint8_t runtime_block = 4;

		using asset_namer = std::function<std::string(std::int32_t type, const void* header)>;

		explicit dump_stream(asset_namer names)
			: names_(std::move(names))
		{
		}

		// Starts the dump of an asset with the bytes of its header.
		void begin(const std::int32_t type, const void* header, const std::size_t size)
		{
			this->type_ = type;
			this->add_data(header, size, false);
		}

		std::string finish() const
		{
			std::string out;
			const auto put = [&out](const auto& value)
			{
				out.append(reinterpret_cast<const char*>(&value), sizeof(value));
			};

			put(dump_magic);
			put(dump_version);
			put(this->type_);
			put(static_cast<std::uint32_t>(this->records_.size()));
			for (const auto& record : this->records_)
			{
				put(record.kind);
				switch (record.kind)
				{
				case dump_record::null:
					break;
				case dump_record::data:
				case dump_record::string:
				case dump_record::asset:
				case dump_record::script_string:
					put(record.value);
					put(static_cast<std::uint64_t>(record.bytes.size()));
					out.append(record.bytes);
					break;
				case dump_record::runtime:
				case dump_record::temp:
					put(record.value);
					break;
				case dump_record::offset:
					put(record.value);
					put(record.delta);
					break;
				}
			}

			return out;
		}

		void push(const std::uint8_t block)
		{
			this->blocks_.push_back(block);
		}

		void pop()
		{
			this->blocks_.pop_back();
		}

		void align(const std::size_t)
		{
		}

		void begin_asset()
		{
		}

		void end_asset()
		{
		}

		void bytes(const std::size_t size)
		{
			const auto found = this->open_.find(this->block());
			if (found == this->open_.end())
			{
				throw std::runtime_error("inline data has no record before it");
			}

			auto& record = this->records_[found->second];
			const auto end = record.source + this->record_size(record);
			if (record.kind == dump_record::data)
			{
				record.bytes.append(reinterpret_cast<const char*>(end), size);
			}
			else
			{
				record.value += size;
			}

			this->sources_[reinterpret_cast<std::uintptr_t>(record.source)] = found->second;
		}

		template <typename T>
		T* array(T*& field, const std::size_t, const std::size_t count)
		{
			this->clear_field(&field, sizeof(field));
			return static_cast<T*>(this->array_bytes(field, sizeof(T) * count));
		}

		template <typename T>
		T* single(T*& field, const std::size_t alignment)
		{
			return this->array(field, alignment, 1);
		}

		template <typename T>
		std::uint8_t* elements(T*& field, const std::size_t, const std::size_t element_size, const std::size_t count)
		{
			this->clear_field(&field, sizeof(field));
			return static_cast<std::uint8_t*>(this->array_bytes(field, element_size * count));
		}

		template <typename T>
		void string(T*& field)
		{
			static_assert(sizeof(T) == 1);
			this->clear_field(&field, sizeof(field));
			if (!field)
			{
				this->records_.emplace_back(dump_record::null);
				return;
			}

			const auto size = std::strlen(reinterpret_cast<const char*>(field)) + 1;
			if (this->add_offset(field, size))
			{
				return;
			}

			auto& record = this->records_.emplace_back(dump_record::string);
			record.source = reinterpret_cast<const std::uint8_t*>(field);
			record.bytes.assign(reinterpret_cast<const char*>(field), size);
			this->sources_[reinterpret_cast<std::uintptr_t>(field)] = this->records_.size() - 1;
		}

		template <typename T>
		void asset(const std::int32_t type, T*& field)
		{
			this->clear_field(&field, sizeof(field));
			if (!field)
			{
				this->records_.emplace_back(dump_record::null);
				return;
			}

			auto& record = this->records_.emplace_back(dump_record::asset);
			record.value = static_cast<std::uint32_t>(type);
			record.bytes = this->names_(type, field);
		}

		template <typename T>
		void script_string(T& field)
		{
			this->clear_field(&field, sizeof(field));
			const auto text = SL_ConvertToString(static_cast<std::uint32_t>(field));
			if (!text)
			{
				this->records_.emplace_back(dump_record::null);
				return;
			}

			auto& record = this->records_.emplace_back(dump_record::script_string);
			record.bytes = text;
		}

		template <typename T>
		void script_strings(T* fields, const std::size_t count)
		{
			for (std::size_t i = 0; i < count; i++)
			{
				this->script_string(fields[i]);
			}
		}

		template <typename T, typename Value>
		T temp(const std::size_t, Value&& value)
		{
			const T result = value();
			auto& record = this->records_.emplace_back(dump_record::temp);
			record.value = static_cast<std::uint64_t>(result);
			return result;
		}

		template <typename T>
		T missing_temp()
		{
			throw std::runtime_error("the dump has no value for a temp count");
		}

		void unsupported(const char* traversal) const
		{
			throw std::runtime_error(std::format("the dump does not support {}", traversal));
		}

	private:
		struct record
		{
			dump_record kind;
			std::uint64_t value{};
			std::uint64_t delta{};
			const std::uint8_t* source{};
			std::string bytes;
		};

		asset_namer names_;
		std::int32_t type_{};
		std::vector<record> records_;
		std::vector<std::uint8_t> blocks_;
		// record index by the source address of its data
		std::map<std::uintptr_t, std::size_t> sources_;
		// the data record of each block that inline data continues
		std::map<std::uint8_t, std::size_t> open_;

		std::uint8_t block() const
		{
			return this->blocks_.empty() ? 0 : this->blocks_.back();
		}

		static std::size_t record_size(const record& record)
		{
			return record.kind == dump_record::runtime ? record.value : record.bytes.size();
		}

		// A field that a record describes holds a memory address or a script string id: the dump stores zero there,
		// so that the dump of one asset is the same in every load.
		void clear_field(const void* field, const std::size_t size)
		{
			const auto key = reinterpret_cast<std::uintptr_t>(field);
			auto it = this->sources_.upper_bound(key);
			for (auto steps = 0; it != this->sources_.begin() && steps < 64; steps++)
			{
				--it;
				auto& record = this->records_[it->second];
				if (record.kind == dump_record::data && key + size <= it->first + record.bytes.size())
				{
					std::memset(record.bytes.data() + (key - it->first), 0, size);
					return;
				}
			}
		}

		void* array_bytes(const void* field, const std::size_t size)
		{
			if (!field)
			{
				this->records_.emplace_back(dump_record::null);
				return nullptr;
			}

			if (this->add_offset(field, size))
			{
				return nullptr;
			}

			this->add_data(field, size, this->block() == runtime_block);
			return const_cast<void*>(field);
		}

		void add_data(const void* source, const std::size_t size, const bool runtime)
		{
			auto& record = this->records_.emplace_back(runtime ? dump_record::runtime : dump_record::data);
			record.source = static_cast<const std::uint8_t*>(source);
			if (runtime)
			{
				record.value = size;
			}
			else
			{
				record.bytes.assign(static_cast<const char*>(source), size);
			}

			const auto index = this->records_.size() - 1;
			this->sources_[reinterpret_cast<std::uintptr_t>(source)] = index;
			this->open_[this->block()] = index;
		}

		// An offset record for data that an earlier record holds.
		bool add_offset(const void* source, const std::size_t size)
		{
			if (!size)
			{
				return false;
			}

			const auto key = reinterpret_cast<std::uintptr_t>(source);
			auto it = this->sources_.upper_bound(key);
			for (auto steps = 0; it != this->sources_.begin() && steps < 64; steps++)
			{
				--it;
				const auto& found = this->records_[it->second];
				if (key + size <= it->first + record_size(found))
				{
					auto& record = this->records_.emplace_back(dump_record::offset);
					record.value = it->second;
					record.delta = key - it->first;
					return true;
				}
			}

			return false;
		}
	};

	class parse_stream
	{
	public:
		using allocator = std::function<void*(std::size_t size)>;
		// A header for an asset that the dump names: a stub that the zone resolves by name.
		using asset_stub = std::function<void*(std::int32_t type, const std::string& name)>;

		parse_stream(const std::string_view data, allocator allocate, asset_stub stubs)
			: data_(data)
			, allocate_(std::move(allocate))
			, stubs_(std::move(stubs))
		{
			if (this->read<std::uint32_t>() != dump_magic || this->read<std::uint32_t>() != dump_version)
			{
				throw std::runtime_error("the file is not an IW8 asset dump of this version");
			}

			this->type_ = this->read<std::int32_t>();
			this->remaining_ = this->read<std::uint32_t>();
		}

		std::int32_t type() const
		{
			return this->type_;
		}

		// The header of the asset: the first record.
		void* begin(const std::size_t size)
		{
			const auto& entry = this->next();
			if (entry.kind != dump_record::data || entry.size != size)
			{
				throw std::runtime_error("the dump does not start with the asset header");
			}

			return entry.base;
		}

		void finish() const
		{
			if (this->remaining_ || this->position_ != this->data_.size())
			{
				throw std::runtime_error("the asset dump holds more records than the asset reads");
			}
		}

		void push(const std::uint8_t block)
		{
			this->blocks_.push_back(block);
		}

		void pop()
		{
			this->blocks_.pop_back();
		}

		void align(const std::size_t)
		{
		}

		void begin_asset()
		{
		}

		void end_asset()
		{
		}

		void bytes(const std::size_t size)
		{
			const auto found = this->open_.find(this->block());
			if (found == this->open_.end())
			{
				throw std::runtime_error("inline data has no record before it");
			}

			auto& cursor = found->second;
			if (cursor.first + size > cursor.second)
			{
				throw std::runtime_error("inline data passes the end of its record");
			}

			cursor.first += size;
		}

		template <typename T>
		T* array(T*& field, const std::size_t, const std::size_t count)
		{
			return static_cast<T*>(this->array_bytes(reinterpret_cast<void*&>(const_cast<std::remove_const_t<T>*&>(field)),
				sizeof(T) * count));
		}

		template <typename T>
		T* single(T*& field, const std::size_t alignment)
		{
			return this->array(field, alignment, 1);
		}

		template <typename T>
		std::uint8_t* elements(T*& field, const std::size_t, const std::size_t element_size, const std::size_t count)
		{
			return static_cast<std::uint8_t*>(this->array_bytes(reinterpret_cast<void*&>(const_cast<std::remove_const_t<T>*&>(field)),
				element_size * count));
		}

		template <typename T>
		void string(T*& field)
		{
			static_assert(sizeof(T) == 1);
			const auto& entry = this->next();
			switch (entry.kind)
			{
			case dump_record::null:
				field = nullptr;
				return;
			case dump_record::offset:
				field = reinterpret_cast<T*>(entry.base);
				return;
			case dump_record::string:
				field = reinterpret_cast<T*>(entry.base);
				return;
			default:
				throw std::runtime_error("the dump has no string where the asset reads one");
			}
		}

		template <typename T>
		void asset(const std::int32_t type, T*& field)
		{
			const auto& entry = this->next();
			if (entry.kind == dump_record::null)
			{
				field = nullptr;
				return;
			}

			if (entry.kind != dump_record::asset)
			{
				throw std::runtime_error("the dump has no asset where the asset reads one");
			}

			field = static_cast<T*>(this->stubs_(type, entry.text));
		}

		template <typename T>
		void script_string(T& field)
		{
			const auto& entry = this->next();
			if (entry.kind != dump_record::null && entry.kind != dump_record::script_string)
			{
				throw std::runtime_error("the dump has no script string where the asset reads one");
			}

			field = static_cast<T>(entry.kind == dump_record::null ? 0 : SL_GetString(entry.text.data()));
		}

		template <typename T>
		void script_strings(T* fields, const std::size_t count)
		{
			for (std::size_t i = 0; i < count; i++)
			{
				this->script_string(fields[i]);
			}
		}

		template <typename T, typename Value>
		T temp(const std::size_t, Value&&)
		{
			const auto& entry = this->next();
			if (entry.kind != dump_record::temp)
			{
				throw std::runtime_error("the dump has no count where the asset reads one");
			}

			return static_cast<T>(entry.value);
		}

		template <typename T>
		T missing_temp()
		{
			throw std::runtime_error("the dump has no value for a temp count");
		}

		void unsupported(const char* traversal) const
		{
			throw std::runtime_error(std::format("the dump parser does not support {}", traversal));
		}

	private:
		struct entry
		{
			dump_record kind;
			std::uint8_t* base{};
			std::size_t size{};
			std::uint64_t value{};
			std::string text;
		};

		std::string_view data_;
		std::size_t position_{};
		std::uint32_t remaining_{};
		std::int32_t type_{};
		allocator allocate_;
		asset_stub stubs_;
		std::vector<entry> entries_;
		std::vector<std::uint8_t> blocks_;
		// cursor and end of the data record of each block that inline data continues
		std::map<std::uint8_t, std::pair<std::uint8_t*, std::uint8_t*>> open_;

		std::uint8_t block() const
		{
			return this->blocks_.empty() ? 0 : this->blocks_.back();
		}

		template <typename T>
		T read()
		{
			if (this->position_ + sizeof(T) > this->data_.size())
			{
				throw std::runtime_error("the asset dump ends early");
			}

			T value{};
			std::memcpy(&value, this->data_.data() + this->position_, sizeof(T));
			this->position_ += sizeof(T);
			return value;
		}

		std::string_view read_bytes(const std::size_t size)
		{
			if (this->position_ + size > this->data_.size())
			{
				throw std::runtime_error("the asset dump ends early");
			}

			const auto bytes = this->data_.substr(this->position_, size);
			this->position_ += size;
			return bytes;
		}

		const entry& next()
		{
			if (!this->remaining_)
			{
				throw std::runtime_error("the asset reads more records than the dump holds");
			}

			this->remaining_--;
			auto& result = this->entries_.emplace_back(this->read<dump_record>());
			switch (result.kind)
			{
			case dump_record::null:
				break;
			case dump_record::data:
			case dump_record::string:
			case dump_record::asset:
			case dump_record::script_string:
			{
				result.value = this->read<std::uint64_t>();
				const auto bytes = this->read_bytes(this->read<std::uint64_t>());
				result.size = bytes.size();
				if (result.kind == dump_record::data || result.kind == dump_record::string)
				{
					result.base = static_cast<std::uint8_t*>(this->allocate_(std::max<std::size_t>(bytes.size(), 1)));
					std::memcpy(result.base, bytes.data(), bytes.size());
				}
				else
				{
					result.text = bytes;
				}
				break;
			}
			case dump_record::runtime:
				result.value = this->read<std::uint64_t>();
				result.size = result.value;
				result.base = static_cast<std::uint8_t*>(this->allocate_(std::max<std::size_t>(result.size, 1)));
				std::memset(result.base, 0, result.size);
				break;
			case dump_record::temp:
				result.value = this->read<std::uint64_t>();
				break;
			case dump_record::offset:
			{
				const auto index = this->read<std::uint64_t>();
				const auto delta = this->read<std::uint64_t>();
				if (index >= this->entries_.size() - 1 || !this->entries_[index].base || delta > this->entries_[index].size)
				{
					throw std::runtime_error("an offset record points outside the dump");
				}

				result.base = this->entries_[index].base + delta;
				break;
			}
			default:
				throw std::runtime_error("the asset dump holds an unknown record");
			}

			return result;
		}

		void* array_bytes(void*& field, const std::size_t size)
		{
			const auto& entry = this->next();
			switch (entry.kind)
			{
			case dump_record::null:
				field = nullptr;
				return nullptr;
			case dump_record::offset:
				field = entry.base;
				return nullptr;
			case dump_record::data:
			case dump_record::runtime:
				if (entry.size < size)
				{
					throw std::runtime_error("a data record is shorter than the array that the asset reads");
				}

				field = entry.base;
				this->open_[this->block()] = {entry.base + size, entry.base + entry.size};
				return entry.base;
			default:
				throw std::runtime_error("the dump has no data where the asset reads an array");
			}
		}
	};
}
