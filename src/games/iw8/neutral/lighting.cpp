#include <std_include.hpp>
#include "lighting.hpp"
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

		std::string native_name(const char* name, const char* what)
		{
			if (!name)
			{
				throw std::runtime_error(std::string("native ") + what + " has no name");
			}

			return name;
		}

		// The definition name of an asset that a field points to: a reference stub's name without its comma.
		std::string reference_name(const char* name, const char* what)
		{
			const auto result = native_name(name, what);
			return result.starts_with(',') ? result.substr(1) : result;
		}

		bool same_profile(const asset_data::provenance* source)
		{
			return source && source->profile == lighting_profile;
		}
	}

	light_def_source to_neutral(const GfxLightDef& native)
	{
		light_def_source source;
		source.light_def.name = native_name(native.name, "GfxLightDef");
		if (native.iesProfile)
		{
			source.light_def.ies_profile = native.iesProfile;
		}

		source.provenance = {lighting_profile, "lightdef", source.light_def.name, {{"firstPixel", asset_data::hex(native.firstPixel)},
			{"coordOffset", asset_data::hex(native.coordOffset)}, {"coordScale", asset_data::hex(native.coordScale)}}};
		asset_data::validate(source.light_def);
		return source;
	}

	grading_clut_source to_neutral(const GfxGradingClut& native)
	{
		grading_clut_source source;
		source.clut.name = native_name(native.name, "GfxGradingClut");
		source.clut.size = native.clutSize;
		if (native.hdrClutImage)
		{
			source.clut.image = reference_name(native.hdrClutImage->name, "grading table image");
		}

		source.provenance = {lighting_profile, "gradingclut", source.clut.name,
			{{"inputColorimetry", asset_data::hex(native.inputColorimetry)}, {"outputColorimetry", asset_data::hex(native.outputColorimetry)}}};
		asset_data::validate(source.clut);
		return source;
	}

	GfxLightDef* from_neutral(const asset_data::light_def& data, const asset_data::provenance* source, zone_memory& memory)
	{
		asset_data::validate(data);
		auto* native = memory.allocate<GfxLightDef>();
		if (same_profile(source))
		{
			native->firstPixel = source->value<std::uint32_t>("firstPixel");
			native->coordOffset = source->value<float>("coordOffset");
			native->coordScale = source->value<float>("coordScale");
		}
		else if (data.ies_profile)
		{
			throw std::runtime_error("lightdef " + data.name + " has an IES profile and no place in Replay's IES lookup strip");
		}

		native->name = copy_text(memory, data.name);
		native->iesProfile = data.ies_profile ? copy_text(memory, *data.ies_profile) : nullptr;
		return native;
	}

	GfxGradingClut* from_neutral(const asset_data::grading_clut& data, const asset_data::provenance* source,
		const asset_resolver& resolve, zone_memory& memory)
	{
		asset_data::validate(data);
		if (!same_profile(source))
		{
			throw std::runtime_error("gradingclut " + data.name + " has no Replay colorimetries");
		}

		auto* native = memory.allocate<GfxGradingClut>();
		native->inputColorimetry = source->value<std::uint8_t>("inputColorimetry");
		native->outputColorimetry = source->value<std::uint8_t>("outputColorimetry");
		native->clutSize = data.size;
		if (data.image)
		{
			native->hdrClutImage = static_cast<GfxImage*>(resolve(ASSET_TYPE_IMAGE, *data.image));
			if (!native->hdrClutImage)
			{
				throw std::runtime_error("gradingclut " + data.name + " image is absent: " + *data.image);
			}
		}

		native->name = copy_text(memory, data.name);
		return native;
	}

	std::size_t native_size(const asset_data::light_def& data)
	{
		return rounded(sizeof(GfxLightDef)) + rounded(data.name.size() + 1) + (data.ies_profile ? rounded(data.ies_profile->size() + 1) : 0);
	}

	std::size_t native_size(const asset_data::grading_clut& data)
	{
		return rounded(sizeof(GfxGradingClut)) + rounded(data.name.size() + 1);
	}
}
