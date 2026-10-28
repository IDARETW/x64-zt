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

	namespace
	{
		// The definition name of the asset that a field points to (a reference stub's name without its comma), or none.
		std::optional<std::string> reference_name(const std::int32_t type, const void* header)
		{
			if (!header)
			{
				return std::nullopt;
			}

			const auto* name = get_asset_name(type, header);
			if (!name || !*name)
			{
				throw std::runtime_error("a material reference has no name");
			}

			return name[0] == ',' ? name + 1 : name;
		}

		bool zero_bytes(const void* data, const std::size_t size)
		{
			const auto* bytes = static_cast<const std::uint8_t*>(data);
			return std::all_of(bytes, bytes + size, [](const std::uint8_t byte) { return !byte; });
		}

		void require(const bool condition, const char* what)
		{
			if (!condition)
			{
				throw std::runtime_error(what);
			}
		}

		std::vector<std::uint8_t> bytes_of(const std::uint8_t* data, const std::uint32_t size, const char* what)
		{
			require(!size || data, what);
			return {data, data + size};
		}

		template <typename T>
		T* copy_array(zone_memory& memory, const T* values, const std::size_t count)
		{
			if (!count)
			{
				return nullptr;
			}

			auto* copy = memory.manual_allocate<T>(rounded(count * sizeof(T)));
			std::copy_n(values, count, copy);
			return copy;
		}

		template <typename T>
		T* resolved(const asset_resolver& resolve, const std::int32_t type, const std::optional<std::string>& name)
		{
			if (!name)
			{
				return nullptr;
			}

			auto* header = static_cast<T*>(resolve(type, *name));
			if (!header)
			{
				throw std::runtime_error("the referenced asset " + *name + " of type " + std::to_string(type) + " is absent");
			}

			return header;
		}

		constexpr std::size_t constant_buffer_slots = 195;
	}

	std::expected<material_source, std::string> to_neutral(const Material& native)
	{
		try
		{
			require(native.name, "the material has no name");
			require(!native.textureCount || native.textureTable, "the material has textures without a table");
			require(!native.constantCount || native.constantTable, "the material has constants without a table");
			require(!native.constantBufferCount || native.constantBufferTable, "the material has constant buffers without a table");
			require(!native.layerCount || native.subMaterials, "the material has layers without names");
			require(!native.packedAtlasDataSize || native.packedAtlasData, "the material has atlas data without bytes");

			material_source source;
			auto& data = source.material;
			data.name = native.name;
			data.contents = native.contents;
			data.surface_flags = native.surfaceFlags;
			data.max_displacement = native.maxDisplacement;
			data.sort_key = native.sortKey;
			data.atlas_rows = native.textureAtlasRowCount;
			data.atlas_columns = native.textureAtlasColumnCount;
			if (native.packedAtlasData)
			{
				data.atlas_data = bytes_of(native.packedAtlasData, native.packedAtlasDataSize, "the atlas data has no bytes");
			}

			data.techset = reference_name(ASSET_TYPE_TECHNIQUE_SET, native.techniqueSet);
			for (std::size_t texture = 0; texture < native.textureCount; ++texture)
			{
				const auto& value = native.textureTable[texture];
				data.textures.push_back({value.index, reference_name(ASSET_TYPE_IMAGE, value.image)});
			}

			for (std::size_t constant = 0; constant < native.constantCount; ++constant)
			{
				const auto& value = native.constantTable[constant];
				data.constants.push_back({value.index, {value.literal.v[0], value.literal.v[1], value.literal.v[2], value.literal.v[3]}});
			}

			data.decal_volume = reference_name(ASSET_TYPE_DECAL_VOLUME_MATERIAL, native.decalVolumeMaterial);
			if (native.constantBufferIndex)
			{
				data.constant_buffer_slots.emplace(native.constantBufferIndex, native.constantBufferIndex + constant_buffer_slots);
			}

			for (std::size_t buffer = 0; buffer < native.constantBufferCount; ++buffer)
			{
				const auto& value = native.constantBufferTable[buffer];
				require(zero_bytes(&value.vsConstantBuffer, sizeof(value.vsConstantBuffer)) && zero_bytes(&value.hsConstantBuffer, sizeof(value.hsConstantBuffer)) &&
					zero_bytes(&value.dsConstantBuffer, sizeof(value.dsConstantBuffer)) && zero_bytes(&value.psConstantBuffer, sizeof(value.psConstantBuffer)),
					"a constant buffer holds runtime data");
				data.constant_buffers.push_back({bytes_of(value.vsData, value.vsDataSize, "a constant buffer has a size and no bytes"),
					bytes_of(value.hsData, value.hsDataSize, "a constant buffer has a size and no bytes"),
					bytes_of(value.dsData, value.dsDataSize, "a constant buffer has a size and no bytes"),
					bytes_of(value.psData, value.psDataSize, "a constant buffer has a size and no bytes")});
			}

			for (std::size_t layer = 0; layer < native.layerCount; ++layer)
			{
				require(native.subMaterials[layer], "a layer has no material name");
				data.layers.emplace_back(native.subMaterials[layer]);
			}

			source.provenance = {material_profile, "material", data.name, {{"materialType", asset_data::hex(native.materialType)},
				{"cameraRegion", asset_data::hex(native.cameraRegion)}, {"flags", asset_data::hex(native.___u7.flags)},
				{"drawSurf", asset_data::hex(native.drawSurf)}}};
			asset_data::validate(data);
			return source;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	Material* from_neutral(const asset_data::material& data, const asset_data::provenance* source, const asset_resolver& resolve,
		zone_memory& memory)
	{
		asset_data::validate(data);
		if (!source || source->profile != material_profile)
		{
			throw std::runtime_error("material " + data.name + " has no Replay geometry type, camera region, flags and draw surface");
		}

		auto* native = memory.allocate<Material>();
		native->name = copy_text(memory, data.name);
		native->contents = data.contents;
		native->surfaceFlags = data.surface_flags;
		native->maxDisplacement = data.max_displacement;
		native->materialType = source->value<MaterialGeometryType>("materialType");
		native->cameraRegion = source->value<std::uint8_t>("cameraRegion");
		native->sortKey = static_cast<std::uint8_t>(data.sort_key);
		native->___u7.flags = source->value<std::uint16_t>("flags");
		native->drawSurf = source->value<GfxDrawSurf>("drawSurf");
		native->textureAtlasRowCount = static_cast<std::uint8_t>(data.atlas_rows);
		native->textureAtlasColumnCount = static_cast<std::uint8_t>(data.atlas_columns);
		if (data.atlas_data)
		{
			native->packedAtlasDataSize = static_cast<std::uint16_t>(data.atlas_data->size());
			native->packedAtlasData = memory.manual_allocate<std::uint8_t>(rounded(std::max<std::size_t>(data.atlas_data->size(), 1)));
			std::ranges::copy(*data.atlas_data, native->packedAtlasData);
		}

		native->techniqueSet = resolved<MaterialTechniqueSet>(resolve, ASSET_TYPE_TECHNIQUE_SET, data.techset);
		native->textureCount = static_cast<std::uint8_t>(data.textures.size());
		native->textureTable = data.textures.empty() ? nullptr : memory.manual_allocate<MaterialTextureDef>(rounded(data.textures.size() * sizeof(MaterialTextureDef)));
		for (std::size_t texture = 0; texture < data.textures.size(); ++texture)
		{
			native->textureTable[texture].index = static_cast<std::uint8_t>(data.textures[texture].slot);
			native->textureTable[texture].image = resolved<GfxImage>(resolve, ASSET_TYPE_IMAGE, data.textures[texture].image);
		}

		native->constantCount = static_cast<std::uint8_t>(data.constants.size());
		native->constantTable = data.constants.empty() ? nullptr : memory.manual_allocate<MaterialConstantDef>(rounded(data.constants.size() * sizeof(MaterialConstantDef)));
		for (std::size_t constant = 0; constant < data.constants.size(); ++constant)
		{
			native->constantTable[constant].index = static_cast<std::uint8_t>(data.constants[constant].slot);
			std::ranges::copy(data.constants[constant].value, native->constantTable[constant].literal.v);
		}

		native->decalVolumeMaterial = resolved<GfxDecalVolumeMaterial>(resolve, ASSET_TYPE_DECAL_VOLUME_MATERIAL, data.decal_volume);
		if (data.constant_buffer_slots)
		{
			require(data.constant_buffer_slots->size() == constant_buffer_slots, "the constant buffer slot table is not 195 entries");
			native->constantBufferIndex = copy_array(memory, data.constant_buffer_slots->data(), data.constant_buffer_slots->size());
		}

		native->constantBufferCount = static_cast<std::uint8_t>(data.constant_buffers.size());
		native->constantBufferTable = data.constant_buffers.empty() ? nullptr :
			memory.manual_allocate<MaterialConstantBufferDef>(rounded(data.constant_buffers.size() * sizeof(MaterialConstantBufferDef)));
		for (std::size_t buffer = 0; buffer < data.constant_buffers.size(); ++buffer)
		{
			const auto& value = data.constant_buffers[buffer];
			auto& target = native->constantBufferTable[buffer];
			target.vsDataSize = static_cast<std::uint32_t>(value.vertex.size());
			target.hsDataSize = static_cast<std::uint32_t>(value.hull.size());
			target.dsDataSize = static_cast<std::uint32_t>(value.domain.size());
			target.psDataSize = static_cast<std::uint32_t>(value.pixel.size());
			target.vsData = copy_array(memory, value.vertex.data(), value.vertex.size());
			target.hsData = copy_array(memory, value.hull.data(), value.hull.size());
			target.dsData = copy_array(memory, value.domain.data(), value.domain.size());
			target.psData = copy_array(memory, value.pixel.data(), value.pixel.size());
		}

		native->layerCount = static_cast<std::uint8_t>(data.layers.size());
		if (!data.layers.empty())
		{
			native->subMaterials = memory.manual_allocate<const char*>(rounded(data.layers.size() * sizeof(const char*)));
			// A layer name that a material lists more than once is one string, as in stock zones.
			std::map<std::string, const char*> names;
			for (std::size_t layer = 0; layer < data.layers.size(); ++layer)
			{
				auto& name = names[data.layers[layer]];
				if (!name)
				{
					name = copy_text(memory, data.layers[layer]);
				}

				native->subMaterials[layer] = name;
			}
		}

		return native;
	}

	std::size_t native_size(const asset_data::material& data)
	{
		std::size_t size = rounded(sizeof(Material)) + rounded(data.name.size() + 1) + 8 * 8;
		size += data.atlas_data ? rounded(data.atlas_data->size()) : 0;
		size += data.textures.size() * sizeof(MaterialTextureDef) + data.constants.size() * sizeof(MaterialConstantDef) +
			rounded(constant_buffer_slots) + data.layers.size() * (sizeof(const char*) + 256);
		for (const auto& buffer : data.constant_buffers)
		{
			size += sizeof(MaterialConstantBufferDef) + 4 * 16 + buffer.vertex.size() + buffer.hull.size() + buffer.domain.size() + buffer.pixel.size();
		}

		for (const auto& layer : data.layers)
		{
			size += rounded(layer.size() + 1);
		}

		return size;
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
