#include <std_include.hpp>
#include "physics.hpp"
#include "../iw8.hpp"
#include "../zone/loader.hpp"

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

		// The definition names of the assets that an array of asset pointers holds (a reference stub's name without its
		// comma); a null pointer is an empty slot.
		template <typename Header>
		std::optional<std::vector<std::optional<std::string>>> read_event_names(const std::int32_t type, Header* const* headers, const std::int32_t count,
			const char* what)
		{
			if (count < 0 || (count && !headers))
			{
				throw std::runtime_error(std::string("the physics asset has ") + what + " events and no array");
			}

			if (!headers)
			{
				return std::nullopt;
			}

			std::vector<std::optional<std::string>> names;
			for (std::int32_t index = 0; index < count; ++index)
			{
				if (!headers[index])
				{
					names.emplace_back(std::nullopt);
					continue;
				}

				const auto* name = get_asset_name(type, headers[index]);
				if (!name || !*name)
				{
					throw std::runtime_error(std::string("the physics asset has a ") + what + " event without a name");
				}

				names.emplace_back(name[0] == ',' ? name + 1 : name);
			}

			return names;
		}

		template <typename Header>
		Header** write_event_names(const std::int32_t type, const std::optional<std::vector<std::optional<std::string>>>& names,
			const asset_resolver& resolve, zone_memory& memory)
		{
			if (!names)
			{
				return nullptr;
			}

			auto** headers = memory.manual_allocate<Header*>(rounded(names->size() * sizeof(Header*)));
			for (std::size_t index = 0; index < names->size(); ++index)
			{
				if (!(*names)[index])
				{
					continue;
				}

				headers[index] = static_cast<Header*>(resolve(type, *(*names)[index]));
				if (!headers[index])
				{
					throw std::runtime_error("the physics event asset " + *(*names)[index] + " is absent");
				}
			}

			return headers;
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

	std::expected<asset_data::physics_asset, std::string> to_neutral(const PhysicsAsset& native)
	{
		try
		{
			if (!native.name)
			{
				throw std::runtime_error("the physics asset has no name");
			}

			asset_data::physics_asset data;
			data.name = native.name;
			data.shape = read_tagfile(native.havokData, native.havokDataSize, "physicsasset " + data.name);
			if (native.useCategory >= std::size(asset_data::physics_use_categories))
			{
				throw std::runtime_error("the physics asset has an unknown use category");
			}

			data.use_category = asset_data::physics_use_categories[native.useCategory];
			data.constraint_count = native.numConstraints;
			if (native.numRigidBodies < 0 || (native.numRigidBodies && (!native.simulationCategories || !native.bodyContents)) ||
				(!native.simulationCategories != !native.bodyContents))
			{
				throw std::runtime_error("the physics asset has bodies that its category and contents arrays do not match");
			}

			if (native.simulationCategories)
			{
				auto& bodies = data.bodies.emplace();
				for (std::int32_t body = 0; body < native.numRigidBodies; ++body)
				{
					if (native.simulationCategories[body] >= std::size(asset_data::physics_simulation_categories))
					{
						throw std::runtime_error("the physics asset has a body with an unknown simulation category");
					}

					bodies.push_back({asset_data::physics_simulation_categories[native.simulationCategories[body]], native.bodyContents[body]});
				}
			}

			data.sound_events = read_event_names(ASSET_TYPE_PHYSICS_SFX_EVENT_ASSET, native.sfxEventAssets, native.numSFXEventAssets, "sound");
			data.effect_events = read_event_names(ASSET_TYPE_PHYSICS_VFX_EVENT_ASSET, native.vfxEventAssets, native.numVFXEventAssets, "effect");
			data.usage = {native.usageCounter.serverEnt, native.usageCounter.clientEnt, native.usageCounter.dynEnt};
			data.contains_dynamic_bodies = native.containsDynamicBodies;
			asset_data::validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	PhysicsAsset* from_neutral(const asset_data::physics_asset& data, const asset_resolver& resolve, zone_memory& memory)
	{
		asset_data::validate(data);
		auto* native = memory.allocate<PhysicsAsset>();
		native->name = copy_text(memory, data.name);
		native->havokDataSize = static_cast<std::uint32_t>(data.shape.bytes.size());
		native->havokData = write_tagfile(data.shape, memory);
		native->useCategory = static_cast<PhysicsAssetUseCategory>(
			std::ranges::find(asset_data::physics_use_categories, data.use_category) - std::begin(asset_data::physics_use_categories));
		native->numConstraints = data.constraint_count;
		native->numRigidBodies = data.bodies ? static_cast<std::int32_t>(data.bodies->size()) : 0;
		if (data.bodies)
		{
			native->simulationCategories = memory.manual_allocate<PhysicsAssetSimulationCategory>(
				rounded(data.bodies->size() * sizeof(PhysicsAssetSimulationCategory)));
			native->bodyContents = memory.manual_allocate<std::uint32_t>(rounded(data.bodies->size() * sizeof(std::uint32_t)));
			for (std::size_t body = 0; body < data.bodies->size(); ++body)
			{
				native->simulationCategories[body] = static_cast<PhysicsAssetSimulationCategory>(
					std::ranges::find(asset_data::physics_simulation_categories, (*data.bodies)[body].simulation_category) -
					std::begin(asset_data::physics_simulation_categories));
				native->bodyContents[body] = (*data.bodies)[body].contents;
			}
		}

		native->numSFXEventAssets = data.sound_events ? static_cast<std::int32_t>(data.sound_events->size()) : 0;
		native->sfxEventAssets = write_event_names<PhysicsSFXEventAsset>(ASSET_TYPE_PHYSICS_SFX_EVENT_ASSET, data.sound_events, resolve, memory);
		native->numVFXEventAssets = data.effect_events ? static_cast<std::int32_t>(data.effect_events->size()) : 0;
		native->vfxEventAssets = write_event_names<PhysicsVFXEventAsset>(ASSET_TYPE_PHYSICS_VFX_EVENT_ASSET, data.effect_events, resolve, memory);
		native->usageCounter = {data.usage.server_entities, data.usage.client_entities, data.usage.dynent_entities};
		native->containsDynamicBodies = data.contains_dynamic_bodies;
		return native;
	}

	std::size_t native_size(const asset_data::physics_asset& data)
	{
		return rounded(sizeof(PhysicsAsset)) + rounded(data.name.size() + 1) + rounded(data.shape.bytes.size()) +
			(data.bodies ? 2 * rounded(data.bodies->size() * sizeof(std::uint32_t)) : 0) +
			(data.sound_events ? rounded(data.sound_events->size() * sizeof(void*)) : 0) +
			(data.effect_events ? rounded(data.effect_events->size() * sizeof(void*)) : 0) + 64;
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
