#include <std_include.hpp>
#include "xanim.hpp"

#include <core/assets/xanim_codec.hpp>
#include <zone/script_strings.hpp>
#include <zone/zone_memory.hpp>

namespace zonetool::iw8::neutral
{
	namespace
	{
		// The record of a keyed root channel continues with the frames of its keys (generated stream reads: a header of 8
		// bytes, then 32 or 8 bytes before them).
		constexpr std::size_t translation_frames = offsetof(XAnimPartTrans, u) + offsetof(XAnimPartTransFrames, indices);
		constexpr std::size_t quat2_frames = offsetof(XAnimDeltaPartQuat2, u) + offsetof(XAnimDeltaPartQuatDataFrames2, indices);
		constexpr std::size_t quat_frames = offsetof(XAnimDeltaPartQuat, u) + offsetof(XAnimDeltaPartQuatDataFrames, indices);
		static_assert(translation_frames == 40 && quat2_frames == 16 && quat_frames == 16);

		// The flags with a neutral name; the other bits go to the provenance. Replay selects the 2D and 3D root motion by
		// bits 1 and 2.
		constexpr std::pair<std::uint8_t, const char*> flags[] = {{1, "looping"}, {2, "delta_2d"}, {4, "delta_3d"}};

		// A blend shape weight is code * weight_scale - 2, the product rounded to a float first, as the game computes it.
		constexpr auto weight_scale = std::bit_cast<float>(0x38800100u);

		constexpr std::size_t rounded(const std::size_t size)
		{
			return (size + 7) & ~std::size_t(7);
		}

		float weight_of(const std::uint16_t code)
		{
			const volatile float product = static_cast<float>(code) * weight_scale;
			return product - 2.0f;
		}

		std::uint16_t weight_code(const float weight)
		{
			const auto code = std::round((static_cast<double>(weight) + 2.0) / weight_scale);
			if (!(code >= 0 && code <= 0xFFFF) ||
				std::bit_cast<std::uint32_t>(weight_of(static_cast<std::uint16_t>(code))) != std::bit_cast<std::uint32_t>(weight))
			{
				throw std::runtime_error(std::format("blend shape weight {} has no Replay weight code", weight));
			}

			return static_cast<std::uint16_t>(code);
		}

		std::string script_string(const scr_string_t id, const char* what)
		{
			const auto* text = SL_ConvertToString(id);
			if (!text)
			{
				throw std::runtime_error(std::format("a {} name is a null script string", what));
			}

			return text;
		}

		const char* copy_text(zone_memory& memory, const std::string& text)
		{
			auto* copy = memory.manual_allocate<char>(rounded(text.size() + 1));
			std::ranges::copy(text, copy);
			return copy;
		}

		template <typename T>
		T* copy_array(zone_memory& memory, const std::vector<T>& values)
		{
			if (values.empty())
			{
				return nullptr;
			}

			auto* copy = memory.manual_allocate<T>(rounded(values.size() * sizeof(T)));
			std::ranges::copy(values, copy);
			return copy;
		}

		// The frames of the keys of a root channel: bytes, or shorts from 256 frames.
		std::vector<std::uint32_t> read_delta_frames(const void* record, const std::size_t offset, const std::size_t count,
			const std::uint32_t frames)
		{
			const auto* bytes = static_cast<const std::uint8_t*>(record) + offset;
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

		template <typename Key>
		void write_delta_frames(void* record, const std::size_t offset, const std::vector<Key>& keys, const std::uint32_t frames)
		{
			auto* bytes = static_cast<std::uint8_t*>(record) + offset;
			for (std::size_t key = 0; key < keys.size(); ++key)
			{
				const auto frame = static_cast<std::uint16_t>(keys[key].frame);
				if (frames >= 256)
				{
					std::memcpy(bytes + key * 2, &frame, sizeof(frame));
				}
				else
				{
					bytes[key] = static_cast<std::uint8_t>(frame);
				}
			}
		}

		// A record of a keyed root channel: its header and the frames of its keys.
		template <typename T>
		T* allocate_keyed(zone_memory& memory, const std::size_t offset, const std::size_t keys, const std::uint32_t frames)
		{
			if (keys < 2 || keys > 0x10000)
			{
				throw std::runtime_error("a keyed root channel needs 2 to 65536 keys");
			}

			return memory.manual_allocate<T>(rounded(offset + keys * (frames >= 256 ? 2 : 1)));
		}

		asset_data::delta_translation read_delta(const XAnimPartTrans& native, const std::uint32_t frames)
		{
			if (!native.size)
			{
				if (native.smallTrans)
				{
					throw std::runtime_error("a constant root translation has a code size");
				}

				return asset_data::constant_translation{{native.u.frame0.v[0], native.u.frame0.v[1], native.u.frame0.v[2]}};
			}

			const auto& keyed = native.u.frames;
			if (native.smallTrans > 1 || !keyed.frames._1)
			{
				throw std::runtime_error("a keyed root translation has no codes or an unknown code size");
			}

			asset_data::keyed_translation result;
			result.code_bits = native.smallTrans ? 8 : 16;
			std::ranges::copy(keyed.mins.v, result.origin.begin());
			std::ranges::copy(keyed.size.v, result.scale.begin());
			const auto keys = read_delta_frames(&native, translation_frames, native.size + 1, frames);
			for (std::size_t key = 0; key < keys.size(); ++key)
			{
				asset_data::translation_key item{keys[key]};
				for (std::size_t axis = 0; axis < 3; ++axis)
				{
					item.codes[axis] = native.smallTrans ? keyed.frames._1[key * 3 + axis] : keyed.frames._2[key * 3 + axis];
				}

				result.keys.push_back(item);
			}

			return result;
		}

		XAnimPartTrans* write_delta(const asset_data::delta_translation& translation, const std::uint32_t frames, zone_memory& memory)
		{
			if (const auto* constant = std::get_if<asset_data::constant_translation>(&translation))
			{
				auto* native = memory.manual_allocate<XAnimPartTrans>(rounded(offsetof(XAnimPartTrans, u) + sizeof(vec3_t)));
				std::ranges::copy(constant->value, native->u.frame0.v);
				return native;
			}

			const auto& keyed = std::get<asset_data::keyed_translation>(translation);
			auto* native = allocate_keyed<XAnimPartTrans>(memory, translation_frames, keyed.keys.size(), frames);
			native->size = static_cast<std::uint16_t>(keyed.keys.size() - 1);
			native->smallTrans = keyed.code_bits == 8;
			std::ranges::copy(keyed.origin, native->u.frames.mins.v);
			std::ranges::copy(keyed.scale, native->u.frames.size.v);
			write_delta_frames(native, translation_frames, keyed.keys, frames);
			const auto width = keyed.code_bits / 8;
			auto* codes = memory.manual_allocate<std::uint8_t>(rounded(keyed.keys.size() * 3 * width));
			for (std::size_t key = 0; key < keyed.keys.size(); ++key)
			{
				for (std::size_t axis = 0; axis < 3; ++axis)
				{
					const auto code = keyed.keys[key].codes[axis];
					std::memcpy(codes + (key * 3 + axis) * width, &code, width);
				}
			}

			native->u.frames.frames._1 = codes;
			return native;
		}

		template <typename Quat>
		asset_data::delta_rotation read_delta(const Quat& native, const std::size_t offset, const std::uint32_t frames)
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
			const auto keys = read_delta_frames(&native, offset, native.size + 1, frames);
			for (std::size_t key = 0; key < keys.size(); ++key)
			{
				const auto* codes = native.u.frames.frames + key * components;
				result.keys.push_back({keys[key], {codes, codes + components}});
			}

			return result;
		}

		template <typename Quat>
		Quat* write_delta(const asset_data::delta_rotation& rotation, const std::size_t offset, const std::uint32_t frames,
			zone_memory& memory)
		{
			if (const auto* constant = std::get_if<asset_data::constant_rotation>(&rotation))
			{
				auto* native = memory.manual_allocate<Quat>(rounded(offsetof(Quat, u) + sizeof(std::declval<Quat&>().u.frame0)));
				std::ranges::copy(constant->codes, native->u.frame0);
				return native;
			}

			const auto& keyed = std::get<asset_data::keyed_rotation>(rotation);
			auto* native = allocate_keyed<Quat>(memory, offset, keyed.keys.size(), frames);
			native->size = static_cast<std::uint16_t>(keyed.keys.size() - 1);
			write_delta_frames(native, offset, keyed.keys, frames);
			const auto components = std::size(native->u.frame0);
			auto* codes = memory.manual_allocate<std::int16_t>(rounded(keyed.keys.size() * components * sizeof(std::int16_t)));
			for (std::size_t key = 0; key < keyed.keys.size(); ++key)
			{
				std::ranges::copy(keyed.keys[key].codes, codes + key * components);
			}

			native->u.frames.frames = codes;
			return native;
		}

		std::size_t key_count(const auto& channel)
		{
			return std::visit([](const auto& value) -> std::size_t
			{
				if constexpr (requires { value.keys; })
				{
					return value.keys.size();
				}
				else
				{
					return 0;
				}
			}, channel);
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
			asset_data::xanim_codec::source_streams in{native.numframes, {"dataByte", native.dataByte, native.dataByteCount},
				{"dataShort", native.dataShort, native.dataShortCount}, {"dataInt", native.dataInt, native.dataIntCount},
				{"randomDataByte", native.randomDataByte, native.randomDataByteCount},
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
					motion.rotation_2d = read_delta(*delta->quat2, quat2_frames, native.numframes);
				}

				if (delta->quat)
				{
					motion.rotation_3d = read_delta(*delta->quat, quat_frames, native.numframes);
				}
			}

			if ((native.notifyCount && !native.notify) || (native.blendShapeWeightCount && (!native.blendShapeWeightNames ||
				!native.blendShapeWeights)))
			{
				throw std::runtime_error("the notes or blend shapes have a count and no data");
			}

			for (std::size_t note = 0; note < native.notifyCount; ++note)
			{
				data.notes.push_back({script_string(native.notify[note].name, "note"), native.notify[note].time});
			}

			for (std::size_t shape = 0; shape < native.blendShapeWeightCount; ++shape)
			{
				data.blend_shapes.push_back(script_string(native.blendShapeWeightNames[shape], "blend shape"));
			}

			const auto weights = (static_cast<std::size_t>(native.numframes) + 1) * native.blendShapeWeightCount;
			std::ranges::transform(std::span(native.blendShapeWeights, native.blendShapeWeights ? weights : 0),
				std::back_inserter(data.blend_shape_weights), weight_of);

			auto& provenance = source.provenance;
			provenance.profile = xanim_profile;
			provenance.kind = "xanim";
			provenance.name = data.name;
			provenance.fields = {{"flags", asset_data::hex(rest)}, {"assetType", asset_data::hex(native.assetType)},
				{"ikType", asset_data::hex(native.ikType)}, {"fingerPoseType", asset_data::hex(native.fingerPoseType)}};
			asset_data::validate(data);
			return source;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	XAnimParts* from_neutral(const asset_data::xanim& data, const asset_data::provenance* source, zone_memory& memory)
	{
		asset_data::validate(data);
		if (data.frames > 0xFFFF || data.notes.size() > 0xFF || data.blend_shapes.size() > 0xFFFF)
		{
			throw std::runtime_error("animation " + data.name + " exceeds Replay's frame, note or blend shape count");
		}

		auto* native = memory.allocate<XAnimParts>();
		asset_data::xanim_codec::encoded_streams out;
		const auto bones = asset_data::xanim_codec::encode(data.bones, data.frames, out, native->boneCount);
		constexpr auto words = std::numeric_limits<std::uint16_t>::max();
		constexpr auto dwords = std::numeric_limits<std::uint32_t>::max();
		if (out.shorts.size() > words || out.ints.size() > words || out.bytes.size() > dwords || out.random_bytes.size() > dwords ||
			out.random_shorts.size() > dwords || out.indices.size() > dwords)
		{
			throw std::runtime_error("animation " + data.name + " exceeds the stream sizes of Replay");
		}

		std::uint8_t native_flags = 0;
		for (const auto& flag : data.flags)
		{
			native_flags = static_cast<std::uint8_t>(native_flags | std::ranges::find(flags, flag, &std::pair<std::uint8_t, const char*>::second)->first);
		}

		if (source && source->profile == xanim_profile)
		{
			native_flags = static_cast<std::uint8_t>(native_flags | source->value<std::uint8_t>("flags"));
			native->assetType = source->value<std::uint8_t>("assetType");
			native->ikType = source->value<std::uint8_t>("ikType");
			native->fingerPoseType = source->value<std::uint8_t>("fingerPoseType");
		}

		native->name = copy_text(memory, data.name);
		native->numframes = static_cast<std::uint16_t>(data.frames);
		native->framerate = data.framerate;
		native->frequency = data.frequency;
		native->flags = native_flags;

		std::vector<scr_string_t> names;
		std::ranges::transform(bones, std::back_inserter(names),
			[](const asset_data::xanim_bone* bone) { return static_cast<scr_string_t>(SL_GetString(bone->name.data())); });
		native->names = copy_array(memory, names);

		native->dataByte = copy_array(memory, out.bytes);
		native->dataByteCount = static_cast<std::uint32_t>(out.bytes.size());
		native->dataShort = copy_array(memory, out.shorts);
		native->dataShortCount = static_cast<std::uint16_t>(out.shorts.size());
		native->dataInt = copy_array(memory, out.ints);
		native->dataIntCount = static_cast<std::uint16_t>(out.ints.size());
		native->randomDataByte = copy_array(memory, out.random_bytes);
		native->randomDataByteCount = static_cast<std::uint32_t>(out.random_bytes.size());
		native->randomDataShort = copy_array(memory, out.random_shorts);
		native->randomDataShortCount = static_cast<std::uint32_t>(out.random_shorts.size());
		native->indices._2 = copy_array(memory, out.indices);
		native->indexCount = static_cast<std::uint32_t>(out.indices.size());

		if (data.delta)
		{
			auto* delta = native->deltaPart = memory.allocate<XAnimDeltaPart>();
			if (data.delta->translation)
			{
				delta->trans = write_delta(*data.delta->translation, data.frames, memory);
			}

			if (data.delta->rotation_2d)
			{
				delta->quat2 = write_delta<XAnimDeltaPartQuat2>(*data.delta->rotation_2d, quat2_frames, data.frames, memory);
			}

			if (data.delta->rotation_3d)
			{
				delta->quat = write_delta<XAnimDeltaPartQuat>(*data.delta->rotation_3d, quat_frames, data.frames, memory);
			}
		}

		std::vector<XAnimNotifyInfo> notes;
		std::ranges::transform(data.notes, std::back_inserter(notes), [](const asset_data::xanim_note& note)
		{
			return XAnimNotifyInfo{static_cast<scr_string_t>(SL_GetString(note.name.data())), note.time};
		});
		native->notify = copy_array(memory, notes);
		native->notifyCount = static_cast<std::uint8_t>(notes.size());

		std::vector<scr_string_t> shapes;
		std::ranges::transform(data.blend_shapes, std::back_inserter(shapes),
			[](const std::string& shape) { return static_cast<scr_string_t>(SL_GetString(shape.data())); });
		std::vector<std::uint16_t> weights;
		std::ranges::transform(data.blend_shape_weights, std::back_inserter(weights), weight_code);
		native->blendShapeWeightNames = copy_array(memory, shapes);
		native->blendShapeWeights = copy_array(memory, weights);
		native->blendShapeWeightCount = static_cast<std::uint16_t>(shapes.size());
		return native;
	}

	std::size_t native_size(const asset_data::xanim& data)
	{
		// An upper bound: a keyed channel takes at most 16 bytes a key and 64 more (frames in two streams, codes, its
		// count, affine and record); a bone at most 48 more (its name, constants and selector).
		std::size_t keys = 0;
		std::size_t channels = 0;
		const auto add = [&](const auto& channel)
		{
			keys += key_count(channel);
			++channels;
		};

		for (const auto& bone : data.bones)
		{
			add(bone.rotation);
			add(bone.translation);
		}

		if (data.delta)
		{
			for (const auto* channel : {&data.delta->rotation_2d, &data.delta->rotation_3d})
			{
				if (*channel)
				{
					add(**channel);
				}
			}

			if (data.delta->translation)
			{
				add(*data.delta->translation);
			}
		}

		return rounded(sizeof(XAnimParts)) + rounded(data.name.size() + 1) + rounded(sizeof(XAnimDeltaPart)) +
			16 * (keys + 4 * channels) + 48 * data.bones.size() + 8 * data.notes.size() + 4 * data.blend_shapes.size() +
			2 * data.blend_shape_weights.size() + 256;
	}
}
