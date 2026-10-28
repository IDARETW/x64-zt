#include <std_include.hpp>
#include "material.hpp"
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

		// The definition name of the image that a channel points to (a reference stub's name without its comma), or none.
		std::optional<std::string> image_name(const GfxImage* image)
		{
			if (!image)
			{
				return std::nullopt;
			}

			const auto* name = get_asset_name(ASSET_TYPE_IMAGE, image);
			if (!name || !*name)
			{
				throw std::runtime_error("a decal volume channel image has no name");
			}

			return name[0] == ',' ? name + 1 : name;
		}
	}

	std::expected<decal_volume_material_source, std::string> to_neutral(const GfxDecalVolumeMaterial& native)
	{
		try
		{
			if (!native.name)
			{
				throw std::runtime_error("the decal volume material has no name");
			}

			if (!std::ranges::all_of(native.padding, [](const std::uint8_t byte) { return !byte; }))
			{
				throw std::runtime_error("the decal volume material's padding holds data");
			}

			decal_volume_material_source source;
			auto& data = source.material;
			data.name = native.name;
			for (std::size_t channel = 0; channel < data.channels.size(); ++channel)
			{
				data.channels[channel] = image_name(native.channels[channel]);
			}

			std::ranges::copy(native.colorTint.v, data.color_tint.begin());
			data.alpha_dissolve = native.alphaDissolveParms;
			data.emissive_scale = native.emissiveScale;
			data.displacement_cutoff_distance = native.displacementCutoffDistance;
			data.displacement_cutoff_falloff = native.displacementCutoffFalloff;
			data.atlas_rows = native.textureAtlasRowCount;
			data.atlas_columns = native.textureAtlasColumnCount;

			source.provenance = {material_profile, "decalvolumematerial", data.name, {{"flags", asset_data::hex(native.flags)},
				{"packedDisplacementScaleAndBias", asset_data::hex(native.packedDisplacementScaleAndBias)},
				{"packedTemperatureBaseAndScale", asset_data::hex(native.packedTemperatureBaseAndScale)}}};
			asset_data::validate(data);
			return source;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	GfxDecalVolumeMaterial* from_neutral(const asset_data::decal_volume_material& data, const asset_data::provenance* source,
		const asset_resolver& resolve, zone_memory& memory)
	{
		asset_data::validate(data);
		if (!source || source->profile != material_profile)
		{
			throw std::runtime_error("decalvolumematerial " + data.name + " has no Replay flags and packed fields");
		}

		auto* native = memory.allocate<GfxDecalVolumeMaterial>();
		native->name = copy_text(memory, data.name);
		for (std::size_t channel = 0; channel < data.channels.size(); ++channel)
		{
			if (const auto& name = data.channels[channel])
			{
				native->channels[channel] = static_cast<GfxImage*>(resolve(ASSET_TYPE_IMAGE, *name));
				if (!native->channels[channel])
				{
					throw std::runtime_error("decalvolumematerial " + data.name + " image is absent: " + *name);
				}
			}
		}

		native->flags = source->value<std::uint32_t>("flags");
		std::ranges::copy(data.color_tint, native->colorTint.v);
		native->alphaDissolveParms = data.alpha_dissolve;
		native->emissiveScale = data.emissive_scale;
		native->packedDisplacementScaleAndBias = source->value<std::uint32_t>("packedDisplacementScaleAndBias");
		native->displacementCutoffDistance = data.displacement_cutoff_distance;
		native->displacementCutoffFalloff = data.displacement_cutoff_falloff;
		native->packedTemperatureBaseAndScale = source->value<std::uint32_t>("packedTemperatureBaseAndScale");
		native->textureAtlasRowCount = static_cast<std::uint8_t>(data.atlas_rows);
		native->textureAtlasColumnCount = static_cast<std::uint8_t>(data.atlas_columns);
		return native;
	}

	std::size_t native_size(const asset_data::decal_volume_material& data)
	{
		return rounded(sizeof(GfxDecalVolumeMaterial)) + rounded(data.name.size() + 1);
	}
}
