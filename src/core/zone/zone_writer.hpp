#pragma once

#include "zone_reader.hpp"

namespace zonetool
{
	// Writes zone data with the stream rules of zone_reader: the same layout, block positions and markers.
	// Data is copied from source objects, and the writer remembers where each copy went: a pointer field of a
	// source object is then set in its copy, and data that is in the zone already can be shared by offset.
	class zone_writer
	{
	public:
		// Without keep_data the writer only counts: it measures a zone, for example to collect its strings first.
		zone_writer(std::size_t block_count, const zone_layout& layout, bool keep_data = true);

		zone_writer(const zone_writer&) = delete;
		zone_writer& operator=(const zone_writer&) = delete;

		void push_stream(std::uint8_t block);
		void pop_stream();
		void align(std::size_t alignment);
		// Offsets to data of the outermost asset count from its start when the layout has relative offsets.
		void begin_asset();
		void end_asset();
		std::uint8_t current_stream() const;
		std::size_t position() const;

		// Copies `size` bytes of `source` to the current block position. The runtime block gets no file bytes.
		void write_copy(const void* source, std::size_t size);
		// Copies `size` bytes that continue the last copy in the current block (inline data of a record).
		void write_continuation(std::size_t size);
		// Writes bytes that no source object holds, for counts that only the stream stores.
		void write_bytes(const void* data, std::size_t size);
		// Takes `size` bytes of the current block without file bytes, for memory that the game fills.
		std::size_t reserve(std::size_t size);
		// File bytes before the block data, for the asset list header.
		void write_raw(const void* data, std::size_t size);

		// Sets the copy of a field. A field in the runtime block has no file bytes and keeps no value.
		void set_field(const void* source_field, const void* value, std::size_t size);
		void set_pointer(const void* source_field, std::uint64_t value);
		// The packed offset of an earlier copy of [source, source + size) in the virtual block.
		std::optional<std::uint64_t> find_copy(const void* source, std::size_t size) const;
		// The virtual block position of the latest copy of a source field, when that copy is in the virtual block.
		std::optional<std::size_t> position_of(const void* source_field) const;
		// The packed offset of a block position, relative to the outermost asset when the position is in it.
		std::uint64_t offset_value(std::uint8_t block, std::size_t position) const;
		// True when the loader resolves a pointer value as an offset (the rules of zone_reader::is_offset).
		bool is_offset(std::uint64_t value) const;

		const std::string& data() const;
		// Moves the written data out (a large zone is not held twice).
		std::string take_data();
		// Reserves the data for a zone of about `size` bytes (it grows by doubling otherwise).
		void reserve_data(std::size_t size);
		std::size_t data_size() const;
		std::vector<std::uint64_t> block_sizes() const;

	private:
		struct copy
		{
			const std::uint8_t* source;
			std::size_t size;
			std::uint8_t block;
			std::size_t position;
			std::optional<std::size_t> file_offset;
			std::size_t sequence;
		};

		struct block
		{
			std::size_t position;
			std::size_t size;
			const std::uint8_t* cursor;
		};

		struct stream_state
		{
			std::uint8_t previous_block;
			std::size_t position_at_push;
		};

		std::string data_;
		std::size_t data_size_{};
		bool keep_data_;
		std::vector<block> blocks_;
		std::uint8_t current_block_{};
		std::vector<stream_state> stream_stack_;
		zone_layout layout_;

		std::map<std::uintptr_t, copy> copies_;
		std::map<std::uintptr_t, copy> virtual_copies_;
		std::size_t sequence_{};
		std::vector<std::vector<std::size_t>> asset_starts_;

		block& current_block();
		void advance(std::size_t size);
		const copy* find_latest_copy(const void* address, std::size_t size) const;
		static const copy* find_latest(const std::map<std::uintptr_t, copy>& copies, std::uintptr_t key, std::size_t size);
	};
}
