#include <std_include.hpp>
#include "techset.hpp"
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

		void require(const bool condition, const char* what)
		{
			if (!condition)
			{
				throw std::runtime_error(what);
			}
		}

		// The definition name of the asset that a field points to (a reference stub's name without its comma), or none.
		std::optional<std::string> reference_name(const std::int32_t type, const void* header)
		{
			if (!header)
			{
				return std::nullopt;
			}

			const auto* name = get_asset_name(type, header);
			require(name && *name, "a technique shader has no name");
			return name[0] == ',' ? name + 1 : name;
		}

		asset_data::technique read_technique(const MaterialTechnique& native)
		{
			require(native.name, "a technique has no name");
			asset_data::technique data;
			data.name = native.name;
			data.type = native.type;
			data.flags = native.flags;
			data.input_layout_count = native.pipelineStateInputLayoutCount;
			std::ranges::copy(native.pipelineStateMap, data.pipeline_state_map.begin());

			const std::size_t states = native.pipelineStateCount;
			const std::size_t hashes = states * native.pipelineStateInputLayoutCount;
			require(!hashes || native.pipelineStates, "a technique has pipeline states without a table");
			for (std::size_t state = 0; state < hashes; ++state)
			{
				require(!native.pipelineStates[state].pso, "a technique holds a runtime pipeline state");
				data.pipeline_state_hashes.push_back(native.pipelineStates[state].hash);
			}

			std::ranges::copy(native.customBufferFlags, data.custom_buffer_flags.begin());
			if (native.rootParameterMap)
			{
				auto& map = data.root_parameter_map.emplace();
				std::copy_n(native.rootParameterMap, map.size(), map.begin());
			}

			data.vertex_shader = reference_name(ASSET_TYPE_VERTEXSHADER, native.vertexShader);
			data.hull_shader = reference_name(ASSET_TYPE_HULLSHADER, native.hullShader);
			data.domain_shader = reference_name(ASSET_TYPE_DOMAINSHADER, native.domainShader);
			data.pixel_shader = reference_name(ASSET_TYPE_PIXELSHADER, native.pixelShader);

			data.argument_counts = {native.neverArgCount, native.perObjArgCount, native.perPrimArgCount, native.stableArgCount};
			data.argument_sizes = {native.perPrimArgSize, native.perObjArgSize, native.stableArgSize};
			const std::size_t arguments = static_cast<std::size_t>(native.neverArgCount) + native.perObjArgCount + native.perPrimArgCount +
				native.stableArgCount;
			require(!arguments || native.args, "a technique has arguments without a table");
			for (std::size_t argument = 0; argument < arguments; ++argument)
			{
				const auto& value = native.args[argument];
				data.arguments.push_back({value.type, value.shader, value.dest, value.index});
			}

			data.color_target_count = native.colorRtCount;
			require(!states || native.stateBits, "a technique has pipeline states without state bits");
			for (std::size_t state = 0; state < states; ++state)
			{
				data.state_bits.push_back(std::to_array(native.stateBits[state].bits));
			}

			data.target_formats = {native.renderTargetFormats.dsFormat, native.renderTargetFormats.colorRtFormats[0],
				native.renderTargetFormats.colorRtFormats[1], native.renderTargetFormats.colorRtFormats[2],
				native.renderTargetFormats.colorRtFormats[3]};
			data.custom_sampler_flags = native.customSamplerFlags;
			data.constant_layout = native.precompiledIndex;
			data.stage_config = native.stageConfig;
			data.default_state_bits = {static_cast<std::uint64_t>(native.defaultStateBits.otherBits),
				static_cast<std::uint64_t>(native.defaultStateBits.blendBits)};
			return data;
		}

		// The content of a technique as a string, the key under which the rebuild shares it.
		std::string technique_key(const asset_data::technique& value)
		{
			std::string key;
			const auto put = [&key](const auto& item)
			{
				key.append(reinterpret_cast<const char*>(&item), sizeof(item));
			};
			const auto put_text = [&key, &put](const std::optional<std::string>& text)
			{
				put(static_cast<std::uint8_t>(text.has_value()));
				if (text)
				{
					put(static_cast<std::uint32_t>(text->size()));
					key += *text;
				}
			};

			put_text(value.name);
			put(value.type);
			put(value.flags);
			put(value.input_layout_count);
			put(value.pipeline_state_map);
			put(static_cast<std::uint32_t>(value.pipeline_state_hashes.size()));
			for (const auto hash : value.pipeline_state_hashes)
			{
				put(hash);
			}

			put(value.custom_buffer_flags);
			put(static_cast<std::uint8_t>(value.root_parameter_map.has_value()));
			if (value.root_parameter_map)
			{
				put(*value.root_parameter_map);
			}

			put_text(value.vertex_shader);
			put_text(value.hull_shader);
			put_text(value.domain_shader);
			put_text(value.pixel_shader);
			put(static_cast<std::uint32_t>(value.arguments.size()));
			for (const auto& argument : value.arguments)
			{
				put(argument.kind);
				put(argument.stage);
				put(argument.destination);
				put(argument.index);
			}

			put(value.argument_counts);
			put(value.argument_sizes);
			put(value.color_target_count);
			put(static_cast<std::uint32_t>(value.state_bits.size()));
			for (const auto& bits : value.state_bits)
			{
				put(bits);
			}

			put(value.target_formats);
			put(value.custom_sampler_flags);
			put(value.constant_layout);
			put(value.stage_config);
			put(value.default_state_bits);
			return key;
		}

		template <typename T>
		T* resolved(const asset_resolver& resolve, const std::int32_t type, const std::optional<std::string>& name)
		{
			if (!name)
			{
				return nullptr;
			}

			auto* header = static_cast<T*>(resolve(type, *name));
			require(header, "a technique shader is absent");
			return header;
		}

		// The array of a kind with these bytes: the one an earlier technique got, else a new one.
		template <typename T>
		T* pooled(technique_cache& cache, const char* kind, const void* bytes, const std::size_t size, zone_memory& memory)
		{
			auto& array = cache.arrays[std::string(kind) + std::string(static_cast<const char*>(bytes), size)];
			if (!array)
			{
				array = memory.manual_allocate<T>(rounded(size));
				std::memcpy(array, bytes, size);
			}

			return static_cast<T*>(array);
		}

		MaterialTechnique* write_technique(const asset_data::technique& data, const asset_resolver& resolve, technique_cache& cache,
			zone_memory& memory)
		{
			auto* native = memory.allocate<MaterialTechnique>();
			native->name = copy_text(memory, data.name);
			native->type = static_cast<MaterialTechniqueType>(data.type);
			native->flags = static_cast<std::uint16_t>(data.flags);
			native->pipelineStateInputLayoutCount = static_cast<std::uint8_t>(data.input_layout_count);
			std::ranges::copy(data.pipeline_state_map, native->pipelineStateMap);
			native->pipelineStateCount = static_cast<std::uint8_t>(data.state_bits.size());
			if (!data.pipeline_state_hashes.empty())
			{
				native->pipelineStates = memory.manual_allocate<MaterialPipelineState>(rounded(data.pipeline_state_hashes.size() * sizeof(MaterialPipelineState)));
				for (std::size_t state = 0; state < data.pipeline_state_hashes.size(); ++state)
				{
					native->pipelineStates[state].hash = data.pipeline_state_hashes[state];
				}
			}

			std::ranges::copy(data.custom_buffer_flags, native->customBufferFlags);
			if (data.root_parameter_map)
			{
				native->rootParameterMap = pooled<std::uint8_t>(cache, "root", data.root_parameter_map->data(), data.root_parameter_map->size(), memory);
			}

			native->vertexShader = resolved<ComputeShader>(resolve, ASSET_TYPE_VERTEXSHADER, data.vertex_shader);
			native->hullShader = resolved<ComputeShader>(resolve, ASSET_TYPE_HULLSHADER, data.hull_shader);
			native->domainShader = resolved<ComputeShader>(resolve, ASSET_TYPE_DOMAINSHADER, data.domain_shader);
			native->pixelShader = resolved<ComputeShader>(resolve, ASSET_TYPE_PIXELSHADER, data.pixel_shader);

			native->neverArgCount = static_cast<std::uint8_t>(data.argument_counts[0]);
			native->perObjArgCount = static_cast<std::uint8_t>(data.argument_counts[1]);
			native->perPrimArgCount = static_cast<std::uint8_t>(data.argument_counts[2]);
			native->stableArgCount = static_cast<std::uint8_t>(data.argument_counts[3]);
			native->perPrimArgSize = static_cast<std::uint16_t>(data.argument_sizes[0]);
			native->perObjArgSize = static_cast<std::uint16_t>(data.argument_sizes[1]);
			native->stableArgSize = static_cast<std::uint16_t>(data.argument_sizes[2]);
			if (!data.arguments.empty())
			{
				native->args = memory.manual_allocate<MaterialShaderArgument>(rounded(data.arguments.size() * sizeof(MaterialShaderArgument)));
				for (std::size_t argument = 0; argument < data.arguments.size(); ++argument)
				{
					native->args[argument] = {static_cast<MaterialArgumentType>(data.arguments[argument].kind),
						static_cast<std::uint8_t>(data.arguments[argument].stage), static_cast<std::uint16_t>(data.arguments[argument].destination),
						static_cast<std::uint16_t>(data.arguments[argument].index)};
				}
			}

			native->colorRtCount = static_cast<std::uint8_t>(data.color_target_count);
			if (!data.state_bits.empty())
			{
				native->stateBits = memory.manual_allocate<Replay_MaterialTechniqueStateBits>(rounded(data.state_bits.size() * sizeof(Replay_MaterialTechniqueStateBits)));
				for (std::size_t state = 0; state < data.state_bits.size(); ++state)
				{
					std::ranges::copy(data.state_bits[state], native->stateBits[state].bits);
				}
			}

			native->renderTargetFormats.dsFormat = data.target_formats[0];
			std::copy_n(data.target_formats.begin() + 1, 4, native->renderTargetFormats.colorRtFormats);
			native->customSamplerFlags = data.custom_sampler_flags;
			native->precompiledIndex = static_cast<MaterialConstantLayout>(data.constant_layout);
			native->stageConfig = static_cast<MaterialStageConfig>(data.stage_config);
			native->defaultStateBits.otherBits = static_cast<GfxOtherStateBits>(data.default_state_bits[0]);
			native->defaultStateBits.blendBits = static_cast<GfxBlendStateBits>(data.default_state_bits[1]);
			return native;
		}
	}

	std::expected<asset_data::technique_set, std::string> to_neutral(const MaterialTechniqueSet& native)
	{
		try
		{
			require(native.name, "the technique set has no name");
			asset_data::technique_set data;
			data.name = native.name;
			data.flags = native.flags;
			data.geometry_type = native.materialType;
			std::ranges::copy(native.techniqueMask.mask, data.mask.begin());
			std::size_t count = 0;
			for (const auto word : native.techniqueMask.mask)
			{
				count += static_cast<std::size_t>(std::popcount(word));
			}

			require(!count || native.maskedTechniques, "the technique set has techniques without a table");
			for (std::size_t index = 0; index < count; ++index)
			{
				const auto* technique = native.maskedTechniques[index];
				data.techniques.push_back(technique ? std::optional(read_technique(*technique)) : std::nullopt);
			}

			asset_data::validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	MaterialTechniqueSet* from_neutral(const asset_data::technique_set& data, const asset_resolver& resolve, technique_cache& cache,
		zone_memory& memory)
	{
		asset_data::validate(data);
		auto* native = memory.allocate<MaterialTechniqueSet>();
		native->name = copy_text(memory, data.name);
		native->flags = data.flags;
		native->materialType = static_cast<MaterialGeometryType>(data.geometry_type);
		std::ranges::copy(data.mask, native->techniqueMask.mask);
		if (!data.techniques.empty())
		{
			native->maskedTechniques = memory.manual_allocate<MaterialTechnique*>(rounded(data.techniques.size() * sizeof(MaterialTechnique*)));
			for (std::size_t index = 0; index < data.techniques.size(); ++index)
			{
				if (!data.techniques[index])
				{
					continue;
				}

				auto& shared = cache.techniques[technique_key(*data.techniques[index])];
				if (!shared)
				{
					shared = write_technique(*data.techniques[index], resolve, cache, memory);
				}

				native->maskedTechniques[index] = shared;
			}
		}

		return native;
	}

	std::size_t native_size(const asset_data::technique_set& data)
	{
		std::size_t size = rounded(sizeof(MaterialTechniqueSet)) + rounded(data.name.size() + 1) + rounded(data.techniques.size() * sizeof(MaterialTechnique*)) + 64;
		for (const auto& value : data.techniques)
		{
			if (value)
			{
				size += rounded(sizeof(MaterialTechnique)) + rounded(value->name.size() + 1) + rounded(value->pipeline_state_hashes.size() * sizeof(MaterialPipelineState)) +
					64 + rounded(value->arguments.size() * sizeof(MaterialShaderArgument)) + rounded(value->state_bits.size() * sizeof(Replay_MaterialTechniqueStateBits));
			}
		}

		return size;
	}
}
