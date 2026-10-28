#include <std_include.hpp>
#include "physics.hpp"
#include "../iw8.hpp"

#include <zone/script_strings.hpp>
#include <zone/zone_memory.hpp>

namespace zonetool::iw8::neutral
{
	namespace
	{
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

		asset_data::havok_tagfile read_tagfile(const char* bytes, const std::uint32_t size, const std::string& what)
		{
			if (size && !bytes)
			{
				throw std::runtime_error(what + " has a size and no bytes");
			}

			asset_data::havok_tagfile data;
			if (size)
			{
				data.bytes.assign(reinterpret_cast<const std::uint8_t*>(bytes), reinterpret_cast<const std::uint8_t*>(bytes) + size);
				data.sdk_version = asset_data::havok_tagfile_version(data.bytes);
			}

			return data;
		}

		char* write_tagfile(const asset_data::havok_tagfile& data, zone_memory& memory)
		{
			if (data.bytes.empty())
			{
				return nullptr;
			}

			auto* bytes = memory.manual_allocate<char>(rounded(data.bytes.size()));
			std::memcpy(bytes, data.bytes.data(), data.bytes.size());
			return bytes;
		}
	}

	std::expected<asset_data::physics_fx_shape, std::string> to_neutral(const PhysicsFXShape& native)
	{
		try
		{
			if (!native.name)
			{
				throw std::runtime_error("the physics effect shape has no name");
			}

			asset_data::physics_fx_shape data;
			data.name = native.name;
			data.shape = read_tagfile(native.havokData, native.havokDataSize, "physicsfxshape " + data.name);
			if (native.soundAlias)
			{
				data.sound_alias = native.soundAlias;
			}

			data.sound_min_impulse = native.soundMinImpulse;
			data.sound_max_impulse = native.soundMaxImpulse;
			data.sound_min_impulse_volume = native.soundMinImpulseVolume;
			data.sound_max_impulse_volume = native.soundMaxImpulseVolume;
			asset_data::validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	PhysicsFXShape* from_neutral(const asset_data::physics_fx_shape& data, zone_memory& memory)
	{
		asset_data::validate(data);
		auto* native = memory.allocate<PhysicsFXShape>();
		native->name = copy_text(memory, data.name);
		native->havokDataSize = static_cast<std::uint32_t>(data.shape.bytes.size());
		native->havokData = write_tagfile(data.shape, memory);
		native->soundAlias = data.sound_alias ? copy_text(memory, *data.sound_alias) : nullptr;
		native->soundMinImpulse = data.sound_min_impulse;
		native->soundMaxImpulse = data.sound_max_impulse;
		native->soundMinImpulseVolume = data.sound_min_impulse_volume;
		native->soundMaxImpulseVolume = data.sound_max_impulse_volume;
		return native;
	}

	std::size_t native_size(const asset_data::physics_fx_shape& data)
	{
		return rounded(sizeof(PhysicsFXShape)) + rounded(data.name.size() + 1) + rounded(data.shape.bytes.size()) +
			(data.sound_alias ? rounded(data.sound_alias->size() + 1) : 0);
	}

	std::expected<asset_data::detail_collision, std::string> to_neutral(const XModelDetailCollision& native)
	{
		try
		{
			if (!native.name)
			{
				throw std::runtime_error("the detail collision has no name");
			}

			asset_data::detail_collision data;
			data.name = native.name;
			data.shape = read_tagfile(native.physicsLODData, native.physicsLODDataSize, "xmodeldetailcollision " + data.name);
			if (native.physicsLODDataNameCount && !native.physicsLODDataNames)
			{
				throw std::runtime_error("the detail collision has part names and no table");
			}

			for (std::uint32_t part = 0; part < native.physicsLODDataNameCount; ++part)
			{
				const auto* text = SL_ConvertToString(native.physicsLODDataNames[part]);
				if (!text)
				{
					throw std::runtime_error("the detail collision has a part name that is not a script string");
				}

				data.part_names.emplace_back(text);
			}

			asset_data::validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	XModelDetailCollision* from_neutral(const asset_data::detail_collision& data, zone_memory& memory)
	{
		asset_data::validate(data);
		auto* native = memory.allocate<XModelDetailCollision>();
		native->name = copy_text(memory, data.name);
		native->physicsLODDataSize = static_cast<std::uint32_t>(data.shape.bytes.size());
		native->physicsLODData = write_tagfile(data.shape, memory);
		native->physicsLODDataNameCount = static_cast<std::uint32_t>(data.part_names.size());
		if (!data.part_names.empty())
		{
			native->physicsLODDataNames = memory.manual_allocate<scr_string_t>(rounded(data.part_names.size() * sizeof(scr_string_t)));
			for (std::size_t part = 0; part < data.part_names.size(); ++part)
			{
				native->physicsLODDataNames[part] = static_cast<scr_string_t>(SL_GetString(data.part_names[part].data()));
			}
		}

		return native;
	}

	std::size_t native_size(const asset_data::detail_collision& data)
	{
		return rounded(sizeof(XModelDetailCollision)) + rounded(data.name.size() + 1) + rounded(data.shape.bytes.size()) +
			rounded(data.part_names.size() * sizeof(scr_string_t));
	}
}
