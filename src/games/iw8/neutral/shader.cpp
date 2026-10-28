#include <std_include.hpp>
#include "shader.hpp"
#include "../iw8.hpp"

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
	}

	std::string shader_stage(const std::int32_t asset_type)
	{
		switch (asset_type)
		{
		case ASSET_TYPE_VERTEXSHADER:
			return "vertex";
		case ASSET_TYPE_HULLSHADER:
			return "hull";
		case ASSET_TYPE_DOMAINSHADER:
			return "domain";
		case ASSET_TYPE_PIXELSHADER:
			return "pixel";
		case ASSET_TYPE_COMPUTESHADER:
			return "compute";
		case ASSET_TYPE_LIBSHADER:
			return "library";
		default:
			return {};
		}
	}

	std::expected<shader_source, std::string> to_neutral(const ComputeShader& native, const std::int32_t asset_type)
	{
		try
		{
			if (!native.name)
			{
				throw std::runtime_error("the shader has no name");
			}

			if (native.prog.cs)
			{
				throw std::runtime_error("the shader holds a runtime pipeline state");
			}

			if (native.prog.loadDef.programSize && !native.prog.loadDef.program)
			{
				throw std::runtime_error("the shader has a size and no program");
			}

			shader_source source;
			auto& data = source.shader;
			data.name = native.name;
			if (native.debugName)
			{
				data.debug_name = native.debugName;
			}

			data.stage = shader_stage(asset_type);
			if (native.prog.loadDef.program)
			{
				data.bytecode.emplace(native.prog.loadDef.program, native.prog.loadDef.program + native.prog.loadDef.programSize);
			}

			// The padding after the program size: struct ComputeShader holds name, debug name, the pipeline state and the
			// load definition (program pointer, size), then these four bytes.
			static_assert(offsetof(ComputeShader, prog) + offsetof(ComputeShaderProgram, loadDef) + offsetof(GfxComputeShaderLoadDef, programSize) + 4 == 36);
			std::array<std::uint8_t, 4> hash{};
			std::memcpy(hash.data(), reinterpret_cast<const std::uint8_t*>(&native) + 36, hash.size());
			source.provenance = {shader_profile, "shader", data.name, {{"unknown_0x24", asset_data::hex(hash)}}};
			asset_data::validate(data);
			return source;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	ComputeShader* from_neutral(const asset_data::shader& data, const asset_data::provenance* source, zone_memory& memory)
	{
		asset_data::validate(data);
		if (!source || source->profile != shader_profile)
		{
			throw std::runtime_error("shader " + data.name + " has no Replay program value");
		}

		if (data.bytecode && data.bytecode->size() > std::numeric_limits<std::uint32_t>::max())
		{
			throw std::runtime_error("shader " + data.name + " exceeds Replay's program size");
		}

		auto* native = memory.allocate<ComputeShader>();
		const auto hash = source->value<std::array<std::uint8_t, 4>>("unknown_0x24");
		std::memcpy(reinterpret_cast<std::uint8_t*>(native) + 36, hash.data(), hash.size());
		native->name = copy_text(memory, data.name);
		// A debug name equal to the name is the same string, as in stock zones.
		native->debugName = !data.debug_name ? nullptr : *data.debug_name == data.name ? native->name : copy_text(memory, *data.debug_name);
		if (data.bytecode)
		{
			native->prog.loadDef.programSize = static_cast<std::uint32_t>(data.bytecode->size());
			native->prog.loadDef.program = memory.manual_allocate<std::uint8_t>(rounded(std::max<std::size_t>(data.bytecode->size(), 1)));
			std::ranges::copy(*data.bytecode, native->prog.loadDef.program);
		}

		return native;
	}

	std::size_t native_size(const asset_data::shader& data)
	{
		return rounded(sizeof(ComputeShader)) + rounded(data.name.size() + 1) + (data.debug_name ? rounded(data.debug_name->size() + 1) : 0) +
			(data.bytecode ? rounded(std::max<std::size_t>(data.bytecode->size(), 1)) : 0);
	}
}
