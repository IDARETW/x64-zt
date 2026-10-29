#include <std_include.hpp>
#include "zone_writer.hpp"

namespace zonetool
{
	namespace
	{
		std::uintptr_t address_of(const void* pointer)
		{
			return reinterpret_cast<std::uintptr_t>(pointer);
		}
	}

	zone_writer::zone_writer(const std::size_t block_count, const zone_layout& layout, const bool keep_data)
		: keep_data_(keep_data)
		, blocks_(block_count)
		, layout_(layout)
	{
	}

	void zone_writer::push_stream(const std::uint8_t block)
	{
		if (block >= this->blocks_.size())
		{
			throw std::runtime_error(std::format("block {} does not exist", block));
		}

		this->stream_stack_.emplace_back(this->current_block_, this->blocks_[block].position);
		this->current_block_ = block;
	}

	void zone_writer::pop_stream()
	{
		if (this->stream_stack_.empty())
		{
			throw std::runtime_error("stream stack is empty");
		}

		const auto state = this->stream_stack_.back();
		this->stream_stack_.pop_back();

		if (this->current_block_ == this->layout_.temp_block)
		{
			this->blocks_[this->layout_.temp_block].position = state.position_at_push;
		}
		else if (this->current_block_ == this->layout_.pop_aligned_block)
		{
			this->align(this->layout_.pop_alignment);
		}

		this->current_block_ = state.previous_block;
	}

	void zone_writer::align(const std::size_t alignment)
	{
		auto& block = this->current_block();
		block.position = (block.position + alignment) & ~alignment;
		block.size = std::max(block.size, block.position);
	}

	void zone_writer::begin_asset()
	{
		std::vector<std::size_t> positions;
		positions.reserve(this->blocks_.size());
		for (const auto& block : this->blocks_)
		{
			positions.push_back(block.position);
		}

		this->asset_starts_.push_back(std::move(positions));
	}

	void zone_writer::end_asset()
	{
		if (this->asset_starts_.empty())
		{
			throw std::runtime_error("asset stack is empty");
		}

		this->asset_starts_.pop_back();
	}

	std::uint8_t zone_writer::current_stream() const
	{
		return this->current_block_;
	}

	std::size_t zone_writer::position() const
	{
		return this->blocks_[this->current_block_].position;
	}

	void zone_writer::write_copy(const void* source, const std::size_t size)
	{
		auto& block = this->current_block();
		const auto bytes = static_cast<const std::uint8_t*>(source);

		copy record{bytes, size, this->current_block_, block.position, {}, this->sequence_++};
		if (this->current_block_ != this->layout_.runtime_block)
		{
			record.file_offset = this->data_size_;
			this->write_raw(bytes, size);
		}

		if (size)
		{
			this->copies_[address_of(bytes)] = record;
			// Stock zones share the latest copy of data that they hold twice.
			if (this->current_block_ == this->layout_.virtual_block)
			{
				this->virtual_copies_[address_of(bytes)] = record;
			}
		}

		block.cursor = bytes + size;
		this->advance(size);
	}

	void zone_writer::write_continuation(const std::size_t size)
	{
		const auto cursor = this->current_block().cursor;
		if (!cursor && size)
		{
			throw std::runtime_error("inline data has no record before it");
		}

		this->write_copy(cursor, size);
	}

	void zone_writer::write_bytes(const void* data, const std::size_t size)
	{
		if (this->current_block_ != this->layout_.runtime_block)
		{
			this->write_raw(data, size);
		}

		this->advance(size);
	}

	std::size_t zone_writer::reserve(const std::size_t size)
	{
		const auto position = this->current_block().position;
		this->advance(size);
		return position;
	}

	void zone_writer::write_raw(const void* data, const std::size_t size)
	{
		if (this->keep_data_)
		{
			this->data_.append(static_cast<const char*>(data), size);
		}

		this->data_size_ += size;
	}

	void zone_writer::set_field(const void* source_field, const void* value, const std::size_t size)
	{
		const auto found = this->find_latest_copy(source_field, size);
		if (!found)
		{
			throw std::runtime_error("a field is outside the written data");
		}

		if (!found->file_offset || !this->keep_data_)
		{
			return;
		}

		const auto offset = *found->file_offset + (address_of(source_field) - address_of(found->source));
		std::memcpy(this->data_.data() + offset, value, size);
	}

	void zone_writer::set_pointer(const void* source_field, const std::uint64_t value)
	{
		this->set_field(source_field, &value, sizeof(value));
	}

	std::optional<std::uint64_t> zone_writer::find_copy(const void* source, const std::size_t size) const
	{
		if (!size)
		{
			return {};
		}

		const auto key = address_of(source);
		const auto found = find_latest(this->virtual_copies_, key, size);
		if (!found)
		{
			return {};
		}

		return this->offset_value(this->layout_.virtual_block, found->position + (key - address_of(found->source)));
	}

	std::optional<std::size_t> zone_writer::position_of(const void* source_field) const
	{
		const auto found = this->find_latest_copy(source_field, sizeof(void*));
		if (!found || found->block != this->layout_.virtual_block)
		{
			return {};
		}

		return found->position + (address_of(source_field) - address_of(found->source));
	}

	std::uint64_t zone_writer::offset_value(const std::uint8_t block, const std::size_t position) const
	{
		// An offset keeps 32 bits of position (one-based): a larger block cannot be addressed.
		if (position >= 0xFFFFFFFFull)
		{
			throw std::runtime_error("zone block position exceeds the 32-bit offset range");
		}

		// Stock zones address the data of the asset that is loading from its start (bit 36), other data absolutely.
		if (this->layout_.relative_offsets && !this->asset_starts_.empty())
		{
			const auto start = this->asset_starts_.front()[block];
			if (position >= start)
			{
				return (1ull << 36) | (static_cast<std::uint64_t>(block) << 32) | static_cast<std::uint32_t>(position - start + 1);
			}
		}

		return (this->layout_.offset_tag << 36) | (static_cast<std::uint64_t>(block) << 32) |
			static_cast<std::uint32_t>(position + 1);
	}

	bool zone_writer::is_offset(const std::uint64_t value) const
	{
		if (!value || std::ranges::contains(this->layout_.following_markers, value) || value == this->layout_.insert_marker)
		{
			return false;
		}

		const auto tag = value >> 36;
		const auto relative = this->layout_.relative_offsets && tag == 1;
		return (tag == 0 || tag == this->layout_.offset_tag || relative) && ((value >> 32) & 0xF) < this->blocks_.size();
	}

	const std::string& zone_writer::data() const
	{
		return this->data_;
	}

	std::string zone_writer::take_data()
	{
		return std::move(this->data_);
	}

	void zone_writer::reserve_data(const std::size_t size)
	{
		if (this->keep_data_)
		{
			this->data_.reserve(size);
		}
	}

	std::size_t zone_writer::data_size() const
	{
		return this->data_size_;
	}

	std::vector<std::uint64_t> zone_writer::block_sizes() const
	{
		std::vector<std::uint64_t> sizes;
		sizes.reserve(this->blocks_.size());
		for (const auto& block : this->blocks_)
		{
			sizes.push_back(block.size);
		}

		return sizes;
	}

	zone_writer::block& zone_writer::current_block()
	{
		return this->blocks_[this->current_block_];
	}

	void zone_writer::advance(const std::size_t size)
	{
		auto& block = this->current_block();
		block.position += size;
		block.size = std::max(block.size, block.position);
	}

	const zone_writer::copy* zone_writer::find_latest_copy(const void* address, const std::size_t size) const
	{
		// Copies of one source object are written again in later assets: the latest one is the one in use.
		return find_latest(this->copies_, address_of(address), size);
	}

	const zone_writer::copy* zone_writer::find_latest(const std::map<std::uintptr_t, copy>& copies, const std::uintptr_t key,
		const std::size_t size)
	{
		const copy* latest = nullptr;
		auto it = copies.upper_bound(key);
		for (auto steps = 0; it != copies.begin() && steps < 64; steps++)
		{
			--it;
			if (key + size <= it->first + it->second.size && (!latest || it->second.sequence > latest->sequence))
			{
				latest = &it->second;
			}
		}

		return latest;
	}
}
