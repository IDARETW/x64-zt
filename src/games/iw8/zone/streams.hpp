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
}
