#include <std_include.hpp>
#include "stream.hpp"

#include <zone/zone_memory.hpp>

namespace zonetool::iw8::neutral
{
	namespace
	{
		// The flags bit that says the zone holds the key's bytes.
		constexpr std::uint8_t resident_flag = 2;

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
	}

	std::expected<stream_key_source, std::string> to_neutral(const StreamKey& native)
	{
		try
		{
			if (!native.name)
			{
				throw std::runtime_error("the stream key has no name");
			}

			if (native.behaviorIndex >= SKBI_COUNT)
			{
				throw std::runtime_error("the stream key's behavior has no neutral name");
			}

			const auto resident = (native.flags & resident_flag) != 0;
			if (resident && !native.data.residentData && native.dataSize)
			{
				throw std::runtime_error("the stream key has a size and no resident bytes");
			}

			stream_key_source source;
			auto& data = source.key;
			data.name = native.name;
			data.behavior = asset_data::stream_key_behaviors[native.behaviorIndex];
			if (resident)
			{
				data.payload.emplace(native.data.residentData, native.data.residentData + native.dataSize);
			}

			source.provenance = {stream_profile, "streamkey", data.name, {{"xpakInfo", asset_data::hex(native.xpakInfo)},
				{"flags", asset_data::hex(native.flags)}, {"behaviorUser", asset_data::hex(native.___u3.assetHash)}}};
			if (!resident)
			{
				source.provenance.fields["dataSize"] = asset_data::hex(native.dataSize);
			}

			asset_data::validate(data);
			return source;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	StreamKey* from_neutral(const asset_data::stream_key& data, const asset_data::provenance* source, zone_memory& memory)
	{
		asset_data::validate(data);
		if (!source || source->profile != stream_profile)
		{
			throw std::runtime_error("streamkey " + data.name + " has no Replay XPak entry and flags");
		}

		const auto flags = source->value<std::uint8_t>("flags");
		if (static_cast<bool>(flags & resident_flag) != data.payload.has_value())
		{
			throw std::runtime_error("streamkey " + data.name + " has a payload that its Replay flags do not match");
		}

		auto* native = memory.allocate<StreamKey>();
		native->name = copy_text(memory, data.name);
		native->xpakInfo = source->value<XPakEntryInfo>("xpakInfo");
		native->flags = flags;
		native->behaviorIndex = static_cast<StreamKeyBehaviorIndex>(std::ranges::find(asset_data::stream_key_behaviors, data.behavior) -
			std::begin(asset_data::stream_key_behaviors));
		native->___u3.assetHash = source->value<std::uint64_t>("behaviorUser");
		if (data.payload)
		{
			if (data.payload->size() > std::numeric_limits<std::uint32_t>::max())
			{
				throw std::runtime_error("streamkey " + data.name + " exceeds Replay's resident size");
			}

			native->dataSize = static_cast<std::uint32_t>(data.payload->size());
			if (!data.payload->empty())
			{
				auto* bytes = memory.manual_allocate<std::uint8_t>(rounded(data.payload->size()));
				std::ranges::copy(*data.payload, bytes);
				native->data.residentData = bytes;
			}
		}
		else
		{
			native->dataSize = source->value<std::uint32_t>("dataSize");
		}

		return native;
	}

	std::size_t native_size(const asset_data::stream_key& data)
	{
		return rounded(sizeof(StreamKey)) + rounded(data.name.size() + 1) + (data.payload ? rounded(data.payload->size()) : 0);
	}
}
