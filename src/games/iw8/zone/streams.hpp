#pragma once

namespace zonetool::iw8
{
	// The asset traversals of zone/asset_streams.cpp call these steps. load_stream maps them to zone_reader.
	class load_stream
	{
	public:
		explicit load_stream(zone_reader& reader)
			: reader_(reader)
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
			return this->reader_.read_array(field, alignment, count);
		}

		template <typename T>
		T* single(T*& field, const std::size_t alignment)
		{
			return this->reader_.read_single(field, alignment);
		}

		template <typename T>
		std::uint8_t* elements(T*& field, const std::size_t alignment, const std::size_t element_size, const std::size_t count)
		{
			return this->reader_.read_stream(field, alignment, element_size, count);
		}

		template <typename T>
		void string(T*& field)
		{
			this->reader_.read_string(field);
		}

		template <typename T>
		void asset(const std::int32_t type, T*& field)
		{
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
	};
}
