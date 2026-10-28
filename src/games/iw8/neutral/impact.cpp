#include <std_include.hpp>
#include "impact.hpp"
#include "../iw8.hpp"
#include "../zone/loader.hpp"

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

		// The definition name of the effect that a slot points to (a reference stub's name without its comma), or none.
		std::optional<std::string> effect_name(const FxCombinedDef& slot)
		{
			if (!slot.particleSystemDef)
			{
				return std::nullopt;
			}

			const auto* name = get_asset_name(ASSET_TYPE_VFX, slot.particleSystemDef);
			if (!name || !*name)
			{
				throw std::runtime_error("an effect slot points at an effect without a name");
			}

			return name[0] == ',' ? name + 1 : name;
		}

		void set_effect(FxCombinedDef& slot, const std::optional<std::string>& name, const asset_resolver& resolve)
		{
			if (!name)
			{
				slot.particleSystemDef = nullptr;
				return;
			}

			slot.particleSystemDef = static_cast<ParticleSystemDef*>(resolve(ASSET_TYPE_VFX, *name));
			if (!slot.particleSystemDef)
			{
				throw std::runtime_error("the effect " + *name + " is absent");
			}
		}

		asset_data::impact_effects read_effects(const FxImpactPack& pack)
		{
			asset_data::impact_effects data;
			for (std::size_t kind = 0; kind < data.surface.size(); ++kind)
			{
				data.surface[kind] = effect_name(pack.nonflesh[kind]);
			}

			for (std::size_t kind = 0; kind < data.flesh.size(); ++kind)
			{
				data.flesh[kind] = effect_name(pack.flesh[kind]);
			}

			return data;
		}

		void write_effects(FxImpactPack& pack, const asset_data::impact_effects& data, const asset_resolver& resolve)
		{
			for (std::size_t kind = 0; kind < data.surface.size(); ++kind)
			{
				set_effect(pack.nonflesh[kind], data.surface[kind], resolve);
			}

			for (std::size_t kind = 0; kind < data.flesh.size(); ++kind)
			{
				set_effect(pack.flesh[kind], data.flesh[kind], resolve);
			}
		}

		std::optional<std::array<asset_data::impact_effects, 2>> read_sets(const FxImpactPack* packs)
		{
			if (!packs)
			{
				return std::nullopt;
			}

			return std::array{read_effects(packs[0]), read_effects(packs[1])};
		}

		FxImpactPack* write_sets(const std::optional<std::array<asset_data::impact_effects, 2>>& sets, const asset_resolver& resolve,
			zone_memory& memory)
		{
			if (!sets)
			{
				return nullptr;
			}

			auto* packs = memory.manual_allocate<FxImpactPack>(rounded(2 * sizeof(FxImpactPack)));
			write_effects(packs[0], (*sets)[0], resolve);
			write_effects(packs[1], (*sets)[1], resolve);
			return packs;
		}
	}

	std::expected<asset_data::impact_table, std::string> to_neutral(const FxImpactTable& native)
	{
		try
		{
			if (!native.name)
			{
				throw std::runtime_error("the impact table has no name");
			}

			asset_data::impact_table data;
			data.name = native.name;
			if (!native.table && native.impactTypeCount)
			{
				throw std::runtime_error("the impact table has types and no entries");
			}

			if (native.table)
			{
				auto& entries = data.entries.emplace();
				for (std::uint32_t type = 0; type < native.impactTypeCount; ++type)
				{
					entries.push_back({read_sets(native.table[type].normal), read_sets(native.table[type].exit)});
				}
			}

			if (native.genericToRegistered)
			{
				data.generic_to_registered.emplace(reinterpret_cast<const std::uint32_t*>(native.genericToRegistered),
					reinterpret_cast<const std::uint32_t*>(native.genericToRegistered) + native.impactTypeCount);
			}
			else if (native.impactTypeCount)
			{
				throw std::runtime_error("the impact table has types and no generic map");
			}

			if (native.registeredToGeneric)
			{
				data.registered_to_generic.emplace();
				std::copy_n(native.registeredToGeneric, data.registered_to_generic->size(), data.registered_to_generic->begin());
			}

			asset_data::validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	FxImpactTable* from_neutral(const asset_data::impact_table& data, const asset_resolver& resolve, zone_memory& memory)
	{
		asset_data::validate(data);
		auto* native = memory.allocate<FxImpactTable>();
		native->name = copy_text(memory, data.name);
		native->impactTypeCount = data.entries ? static_cast<std::uint32_t>(data.entries->size()) : 0;
		if (data.entries)
		{
			native->table = memory.manual_allocate<FxImpactEntry>(rounded(data.entries->size() * sizeof(FxImpactEntry)));
			for (std::size_t type = 0; type < data.entries->size(); ++type)
			{
				native->table[type].normal = write_sets((*data.entries)[type].normal, resolve, memory);
				native->table[type].exit = write_sets((*data.entries)[type].exit, resolve, memory);
			}
		}

		if (data.generic_to_registered)
		{
			native->genericToRegistered = memory.manual_allocate<RegisteredImpactType>(rounded(data.generic_to_registered->size() * sizeof(std::uint32_t)));
			std::memcpy(native->genericToRegistered, data.generic_to_registered->data(), data.generic_to_registered->size() * sizeof(std::uint32_t));
		}

		if (data.registered_to_generic)
		{
			native->registeredToGeneric = memory.manual_allocate<std::int32_t>(rounded(data.registered_to_generic->size() * sizeof(std::int32_t)));
			std::ranges::copy(*data.registered_to_generic, native->registeredToGeneric);
		}

		return native;
	}

	std::size_t native_size(const asset_data::impact_table& data)
	{
		std::size_t size = rounded(sizeof(FxImpactTable)) + rounded(data.name.size() + 1) + 64;
		if (data.entries)
		{
			size += rounded(data.entries->size() * sizeof(FxImpactEntry)) + data.entries->size() * 2 * rounded(2 * sizeof(FxImpactPack));
		}

		if (data.generic_to_registered)
		{
			size += rounded(data.generic_to_registered->size() * sizeof(std::uint32_t));
		}

		return size + rounded(asset_data::impact_registered_types * sizeof(std::int32_t));
	}

	std::expected<asset_data::surface_effect_table, std::string> to_neutral(const SurfaceFxTable& native)
	{
		try
		{
			if (!native.name)
			{
				throw std::runtime_error("the surface effect table has no name");
			}

			asset_data::surface_effect_table data;
			data.name = native.name;
			if (native.numTableEntry < 0 || (!native.table && native.numTableEntry))
			{
				throw std::runtime_error("the surface effect table has a count that its entries do not match");
			}

			if (native.table)
			{
				auto& entries = data.entries.emplace();
				for (std::int32_t kind = 0; kind < native.numTableEntry; ++kind)
				{
					auto& entry = entries.emplace_back();
					entry.base = effect_name(native.table[kind].baseEffect);
					for (std::size_t surface = 0; surface < entry.surface.size(); ++surface)
					{
						entry.surface[surface] = effect_name(native.table[kind].surfaceEffect[surface]);
					}
				}
			}

			data.map_pool_size = native.mapPoolBufferSize;
			data.has_map_pool = native.mapPoolBuffer != nullptr;
			asset_data::validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	SurfaceFxTable* from_neutral(const asset_data::surface_effect_table& data, const asset_resolver& resolve, zone_memory& memory)
	{
		asset_data::validate(data);
		auto* native = memory.allocate<SurfaceFxTable>();
		native->name = copy_text(memory, data.name);
		native->numTableEntry = data.entries ? static_cast<std::int32_t>(data.entries->size()) : 0;
		if (data.entries)
		{
			native->table = memory.manual_allocate<SurfaceFxEntry>(rounded(data.entries->size() * sizeof(SurfaceFxEntry)));
			for (std::size_t kind = 0; kind < data.entries->size(); ++kind)
			{
				const auto& entry = (*data.entries)[kind];
				set_effect(native->table[kind].baseEffect, entry.base, resolve);
				for (std::size_t surface = 0; surface < entry.surface.size(); ++surface)
				{
					set_effect(native->table[kind].surfaceEffect[surface], entry.surface[surface], resolve);
				}
			}
		}

		native->mapPoolBufferSize = data.map_pool_size;
		if (data.has_map_pool)
		{
			native->mapPoolBuffer = memory.manual_allocate<std::uint8_t>(rounded(std::max<std::size_t>(data.map_pool_size, 1)));
		}

		return native;
	}

	std::size_t native_size(const asset_data::surface_effect_table& data)
	{
		return rounded(sizeof(SurfaceFxTable)) + rounded(data.name.size() + 1) + 64 +
			(data.entries ? rounded(data.entries->size() * sizeof(SurfaceFxEntry)) : 0) +
			(data.has_map_pool ? rounded(std::max<std::size_t>(data.map_pool_size, 1)) : 0);
	}
}
