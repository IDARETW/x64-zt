#pragma once

#include "xanim_data.hpp"
#include <format>
#include <span>
#include <stdexcept>
#include <string>
#include <vector>

// The streamed bone channel encoding that IW7 and Replay share (docs/NEUTRAL_ASSETS.md, family F5). The rotation groups
// (identity, keyed z and w, keyed x, y, z and w, constant z and w, constant x, y, z and w) name the bones in order; the
// translation groups (keyed with 8-bit codes, keyed with 16-bit codes, constant, zero) name them by place. The shared
// byte, short, int and index streams carry the key counts, frames, codes and affine terms.
namespace zonetool::asset_data::xanim_codec
{
template <typename T>
class stream
{
public:
	stream(const char* name, const T* data, const std::size_t count)
		: name_(name)
		, data_(data, data ? count : 0)
	{
		if (!data && count)
		{
			throw std::runtime_error(std::format("the {} stream has {} elements and no data", name, count));
		}
	}

	std::span<const T> take(const std::size_t count)
	{
		if (count > this->data_.size() - this->position_)
		{
			throw std::runtime_error(std::format("the {} stream ends before element {}", this->name_, this->position_ + count));
		}

		const auto items = this->data_.subspan(this->position_, count);
		this->position_ += count;
		return items;
	}

	void close() const
	{
		if (this->position_ != this->data_.size())
		{
			throw std::runtime_error(std::format("the {} stream holds {} elements after its channels", this->name_,
				this->data_.size() - this->position_));
		}
	}

private:
	const char* name_;
	std::span<const T> data_;
	std::size_t position_{};
};

// The shared streams of the bone channels of a source, and the frame intervals of its animation.
struct source_streams
{
	std::uint32_t frames;
	stream<std::uint8_t> bytes;
	stream<std::int16_t> shorts;
	stream<std::int32_t> ints;
	stream<std::uint8_t> random_bytes;
	stream<std::int16_t> random_shorts;
	stream<std::uint16_t> indices;

	void close() const
	{
		this->bytes.close();
		this->shorts.close();
		this->ints.close();
		this->random_bytes.close();
		this->random_shorts.close();
		this->indices.close();
	}
};

struct encoded_streams
{
	std::vector<std::uint8_t> bytes;
	std::vector<std::int16_t> shorts;
	std::vector<std::int32_t> ints;
	std::vector<std::uint8_t> random_bytes;
	std::vector<std::int16_t> random_shorts;
	std::vector<std::uint16_t> indices;
};

// groups: the bone counts of the rotation groups (0-4), the translation groups (5-8) and all bones (9); names: the bones
// in the order of the rotation groups. Every stream must close on its last element.
std::vector<xanim_bone> decode(source_streams& in, std::span<const std::uint8_t, 10> groups, std::vector<std::string> names);

// Encodes the bones in Replay's order (the rotation groups in turn, each in the neutral order) and returns that order.
std::vector<const xanim_bone*> encode(const std::vector<xanim_bone>& bones, std::uint32_t frames, encoded_streams& out,
	std::span<std::uint8_t, 10> groups);
} // namespace zonetool::asset_data::xanim_codec
