#include <std_include.hpp>
#include "xanim.hpp"

#include <core/assets/xanim_codec.hpp>
#include <zone/script_strings.hpp>

namespace zonetool::iw7::neutral
{
	namespace
	{
		constexpr std::pair<std::uint8_t, const char*> flags[] = {{ANIM_LOOP, "looping"}, {ANIM_DELTA, "delta_2d"},
			{ANIM_DELTA_3D, "delta_3d"}};

		std::string script_string(const scr_string_t id, const char* what)
		{
			const auto* text = SL_ConvertToString(id);
			if (!text)
			{
				throw std::runtime_error(std::format("a {} name is a null script string", what));
			}

			return text;
		}

		// The frames of the keys of a root channel: bytes, or shorts from 256 frames.
		std::vector<std::uint32_t> delta_frames(const XAnimDynamicIndices& indices, const std::size_t count, const std::uint32_t frames)
		{
			const auto* bytes = reinterpret_cast<const std::uint8_t*>(&indices);
			std::vector<std::uint32_t> result(count);
			for (std::size_t key = 0; key < count; ++key)
			{
				std::uint16_t frame = bytes[key];
				if (frames >= 256)
				{
					std::memcpy(&frame, bytes + key * 2, sizeof(frame));
				}

				result[key] = frame;
			}

			return result;
		}

		asset_data::delta_translation read_delta(const XAnimPartTrans& native, const std::uint32_t frames)
		{
			if (!native.size)
			{
				if (native.smallTrans)
				{
					throw std::runtime_error("a constant root translation has a code size");
				}

				return asset_data::constant_translation{{native.u.frame0[0], native.u.frame0[1], native.u.frame0[2]}};
			}

			const auto& keyed = native.u.frames;
			if (native.smallTrans > 1 || !keyed.frames._1)
			{
				throw std::runtime_error("a keyed root translation has no codes or an unknown code size");
			}

			asset_data::keyed_translation result;
			result.code_bits = native.smallTrans ? 8 : 16;
			std::copy_n(keyed.mins, 3, result.origin.begin());
			std::copy_n(keyed.size, 3, result.scale.begin());
			const auto keys = delta_frames(keyed.indices, native.size + 1, frames);
			for (std::size_t key = 0; key < keys.size(); ++key)
			{
				asset_data::translation_key item{keys[key]};
				for (std::size_t axis = 0; axis < 3; ++axis)
				{
					item.codes[axis] = native.smallTrans ? keyed.frames._1[key][axis] : keyed.frames._2[key][axis];
				}

				result.keys.push_back(item);
			}

			return result;
		}

		template <typename Quat>
		asset_data::delta_rotation read_delta(const Quat& native, const std::uint32_t frames)
		{
			const auto components = std::size(native.u.frame0);
			if (!native.size)
			{
				return asset_data::constant_rotation{{std::begin(native.u.frame0), std::end(native.u.frame0)}};
			}

			if (!native.u.frames.frames)
			{
				throw std::runtime_error("a keyed root rotation has no codes");
			}

			asset_data::keyed_rotation result;
			const auto keys = delta_frames(native.u.frames.indices, native.size + 1, frames);
			for (std::size_t key = 0; key < keys.size(); ++key)
			{
				const auto* codes = native.u.frames.frames[key];
				result.keys.push_back({keys[key], {codes, codes + components}});
			}

			return result;
		}
	}

	std::expected<xanim_source, std::string> to_neutral(const XAnimParts& native)
	{
		try
		{
			if (!native.name)
			{
				throw std::runtime_error("the animation has no name");
			}

			if (native.randomDataIntCount)
			{
				throw std::runtime_error("the random int stream holds data without a known meaning");
			}

			if (native.numframes < 256 && native.indexCount)
			{
				throw std::runtime_error("the byte frame encoding holds an index stream");
			}

			xanim_source source;
			auto& data = source.xanim;
			data.name = native.name;
			data.frames = native.numframes;
			data.framerate = native.framerate;
			data.frequency = native.frequency;
			auto rest = native.flags;
			for (const auto& [bit, name] : flags)
			{
				if (rest & bit)
				{
					data.flags.emplace_back(name);
					rest = static_cast<std::uint8_t>(rest & ~bit);
				}
			}

			std::ranges::sort(data.flags);
			asset_data::xanim_codec::source_streams in{native.numframes,
				{"dataByte", reinterpret_cast<const std::uint8_t*>(native.dataByte), native.dataByteCount},
				{"dataShort", native.dataShort, native.dataShortCount}, {"dataInt", native.dataInt, native.dataIntCount},
				{"randomDataByte", reinterpret_cast<const std::uint8_t*>(native.randomDataByte), native.randomDataByteCount},
				{"randomDataShort", native.randomDataShort, native.randomDataShortCount},
				{"indices", native.indices._2, native.numframes >= 256 ? native.indexCount : 0}};
			if (native.boneCount[9] && !native.names)
			{
				throw std::runtime_error("the animation has bones without names");
			}

			std::vector<std::string> names;
			std::transform(native.names, native.names + native.boneCount[9], std::back_inserter(names),
				[](const scr_string_t id) { return script_string(id, "bone"); });
			data.bones = asset_data::xanim_codec::decode(in, native.boneCount, std::move(names));
			in.close();

			if (const auto* delta = native.deltaPart)
			{
				auto& motion = data.delta.emplace();
				if (delta->trans)
				{
					motion.translation = read_delta(*delta->trans, native.numframes);
				}

				if (delta->quat2)
				{
					motion.rotation_2d = read_delta(*delta->quat2, native.numframes);
				}

				if (delta->quat)
				{
					motion.rotation_3d = read_delta(*delta->quat, native.numframes);
				}
			}

			if (native.notifyCount && !native.notify)
			{
				throw std::runtime_error("the notes have a count and no data");
			}

			for (std::size_t note = 0; note < native.notifyCount; ++note)
			{
				data.notes.push_back({script_string(native.notify[note].name, "note"), native.notify[note].time});
			}

			source.provenance = {xanim_profile, "xanim", data.name, {{"flags", asset_data::hex(rest)},
				{"assetType", asset_data::hex(native.assetType)}, {"unk1", asset_data::hex(native.unk1)}, {"unk2", asset_data::hex(native.unk2)}}};
			asset_data::validate(data);
			return source;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}
}
