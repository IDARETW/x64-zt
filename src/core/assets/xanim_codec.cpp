#include <std_include.hpp>
#include "xanim_codec.hpp"

namespace zonetool::asset_data::xanim_codec
{
	namespace
	{
		// A long channel (64 intervals or more, in an animation of 256 frames or more) keeps the frame of every 256th key
		// and of its last key in the short stream, and the frame of every key in the index stream.
		constexpr bool long_channel(const std::uint32_t frames, const std::size_t intervals)
		{
			return frames >= 256 && intervals >= 0x40;
		}

		constexpr std::size_t page_frames(const std::size_t intervals)
		{
			return (intervals - 1) / 0x100 + 2;
		}


		std::vector<std::uint32_t> read_frames(source_streams& in)
		{
			const std::size_t intervals = static_cast<std::uint16_t>(in.shorts.take(1)[0]);
			std::vector<std::uint32_t> frames;
			if (in.frames < 256)
			{
				std::ranges::copy(in.bytes.take(intervals + 1), std::back_inserter(frames));
			}
			else if (!long_channel(in.frames, intervals))
			{
				std::ranges::transform(in.shorts.take(intervals + 1), std::back_inserter(frames),
					[](const std::int16_t frame) { return static_cast<std::uint16_t>(frame); });
			}
			else
			{
				const auto pages = in.shorts.take(page_frames(intervals));
				std::ranges::copy(in.indices.take(intervals + 1), std::back_inserter(frames));
				for (std::size_t page = 0; page < pages.size(); ++page)
				{
					if (static_cast<std::uint16_t>(pages[page]) != frames[std::min(page * 0x100, intervals)])
					{
						throw std::runtime_error("a page frame of a long channel differs from the frame of its key");
					}
				}
			}

			return frames;
		}

		template <typename Key>
		void write_frames(encoded_streams& out, const std::vector<Key>& keys, const std::uint32_t frames)
		{
			const auto intervals = keys.size() - 1;
			if (intervals > 0xFFFF)
			{
				throw std::runtime_error("a channel has more keys than Replay counts");
			}

			out.shorts.push_back(static_cast<std::int16_t>(intervals));
			if (frames < 256)
			{
				std::ranges::transform(keys, std::back_inserter(out.bytes), [](const Key& key) { return static_cast<std::uint8_t>(key.frame); });
			}
			else if (!long_channel(frames, intervals))
			{
				std::ranges::transform(keys, std::back_inserter(out.shorts), [](const Key& key) { return static_cast<std::int16_t>(key.frame); });
			}
			else
			{
				for (std::size_t page = 0; page < page_frames(intervals); ++page)
				{
					out.shorts.push_back(static_cast<std::int16_t>(keys[std::min(page * 0x100, intervals)].frame));
				}

				std::ranges::transform(keys, std::back_inserter(out.indices), [](const Key& key) { return static_cast<std::uint16_t>(key.frame); });
			}
		}

		// The rotation groups of Replay: identity, keyed z and w, keyed x, y, z and w, constant z and w, constant x, y, z
		// and w. The translation groups: keyed with 8-bit codes, keyed with 16-bit codes, constant, zero.
		std::size_t rotation_group(const asset_data::bone_rotation& rotation)
		{
			return std::visit([](const auto& value) -> std::size_t
			{
				using type = std::decay_t<decltype(value)>;
				if constexpr (std::is_same_v<type, asset_data::identity_rotation>)
				{
					return 0;
				}
				else if constexpr (std::is_same_v<type, asset_data::keyed_rotation>)
				{
					return value.keys.front().codes.size() == 2 ? 1 : 2;
				}
				else
				{
					return value.codes.size() == 2 ? 3 : 4;
				}
			}, rotation);
		}

		std::size_t translation_group(const asset_data::bone_translation& translation)
		{
			return std::visit([](const auto& value) -> std::size_t
			{
				using type = std::decay_t<decltype(value)>;
				if constexpr (std::is_same_v<type, asset_data::keyed_translation>)
				{
					return value.code_bits == 8 ? 5 : 6;
				}
				else if constexpr (std::is_same_v<type, asset_data::constant_translation>)
				{
					return 7;
				}
				else
				{
					return 8;
				}
			}, translation);
		}

		// The bones are named in the order of their rotation groups; a translation names its bone by its place.
		std::vector<asset_data::xanim_bone> read_bones(source_streams& in, const std::span<const std::uint8_t, 10> groups, std::vector<std::string> names)
		{
			const auto count = groups[9];
			const auto sum = [&groups](const std::size_t first, const std::size_t last)
			{
				return std::accumulate(groups.begin() + first, groups.begin() + last + 1, 0u);
			};

			if (sum(0, 4) != count || sum(5, 8) != count || names.size() != count)
			{
				throw std::runtime_error("the rotation and translation groups do not each hold every bone");
			}

			std::vector<asset_data::xanim_bone> bones(count);
			for (std::size_t bone = 0; bone < count; ++bone)
			{
				bones[bone].name = std::move(names[bone]);
			}

			auto bone = bones.begin() + groups[0];
			for (const auto group : {1, 2})
			{
				for (std::size_t track = 0; track < groups[group]; ++track, ++bone)
				{
					asset_data::keyed_rotation rotation;
					for (const auto frame : read_frames(in))
					{
						const auto codes = in.random_shorts.take(group == 1 ? 2 : 4);
						rotation.keys.push_back({frame, {codes.begin(), codes.end()}});
					}

					bone->rotation = std::move(rotation);
				}
			}

			for (const auto group : {3, 4})
			{
				for (std::size_t track = 0; track < groups[group]; ++track, ++bone)
				{
					const auto codes = in.shorts.take(group == 3 ? 2 : 4);
					bone->rotation = asset_data::constant_rotation{{codes.begin(), codes.end()}};
				}
			}

			std::vector<bool> placed(count);
			const auto select = [&]() -> asset_data::xanim_bone&
			{
				const auto place = in.bytes.take(1)[0];
				if (place >= count || placed[place])
				{
					throw std::runtime_error("a translation names a bone that is absent or already translated");
				}

				placed[place] = true;
				return bones[place];
			};

			for (const auto group : {5, 6})
			{
				for (std::size_t track = 0; track < groups[group]; ++track)
				{
					auto& target = select();
					asset_data::keyed_translation translation;
					translation.code_bits = group == 5 ? 8 : 16;
					const auto frames = read_frames(in);
					const auto affine = in.ints.take(6);
					for (std::size_t axis = 0; axis < 3; ++axis)
					{
						translation.origin[axis] = std::bit_cast<float>(affine[axis]);
						translation.scale[axis] = std::bit_cast<float>(affine[axis + 3]);
					}

					for (const auto frame : frames)
					{
						asset_data::translation_key key{frame};
						if (group == 5)
						{
							std::ranges::copy(in.random_bytes.take(3), key.codes.begin());
						}
						else
						{
							std::ranges::transform(in.random_shorts.take(3), key.codes.begin(),
								[](const std::int16_t code) { return static_cast<std::uint16_t>(code); });
						}

						translation.keys.push_back(key);
					}

					target.translation = std::move(translation);
				}
			}

			for (std::size_t track = 0; track < groups[7]; ++track)
			{
				auto& target = select();
				asset_data::constant_translation translation;
				std::ranges::transform(in.ints.take(3), translation.value.begin(), [](const std::int32_t bits) { return std::bit_cast<float>(bits); });
				target.translation = translation;
			}

			for (std::size_t track = 0; track < groups[8]; ++track)
			{
				select().translation = asset_data::zero_translation{};
			}

			return bones;
		}

		// Replay's bone order: the rotation groups in turn, each in the neutral bone order.
		std::vector<const asset_data::xanim_bone*> native_order(const std::vector<asset_data::xanim_bone>& source)
		{
			std::vector<const asset_data::xanim_bone*> bones;
			for (const auto& bone : source)
			{
				bones.push_back(&bone);
			}

			std::ranges::stable_sort(bones, {}, [](const asset_data::xanim_bone* bone) { return rotation_group(bone->rotation); });
			return bones;
		}

		void write_bones(const std::vector<const asset_data::xanim_bone*>& bones, const std::uint32_t frames,
			encoded_streams& out, const std::span<std::uint8_t, 10> counts)
		{
			if (bones.size() > 0xFF)
			{
				throw std::runtime_error("an animation has more bones than Replay counts");
			}

			counts[9] = static_cast<std::uint8_t>(bones.size());
			for (const auto* bone : bones)
			{
				++counts[rotation_group(bone->rotation)];
				++counts[translation_group(bone->translation)];
			}

			for (const auto* bone : bones)
			{
				if (const auto* rotation = std::get_if<asset_data::keyed_rotation>(&bone->rotation))
				{
					write_frames(out, rotation->keys, frames);
					for (const auto& key : rotation->keys)
					{
						out.random_shorts.insert(out.random_shorts.end(), key.codes.begin(), key.codes.end());
					}
				}
			}

			for (const auto* bone : bones)
			{
				if (const auto* rotation = std::get_if<asset_data::constant_rotation>(&bone->rotation))
				{
					out.shorts.insert(out.shorts.end(), rotation->codes.begin(), rotation->codes.end());
				}
			}

			for (const std::size_t group : {5, 6, 7, 8})
			{
				for (std::size_t place = 0; place < bones.size(); ++place)
				{
					const auto& translation = bones[place]->translation;
					if (translation_group(translation) != group)
					{
						continue;
					}

					out.bytes.push_back(static_cast<std::uint8_t>(place));
					if (const auto* keyed = std::get_if<asset_data::keyed_translation>(&translation))
					{
						write_frames(out, keyed->keys, frames);
						for (const auto& values : {keyed->origin, keyed->scale})
						{
							std::ranges::transform(values, std::back_inserter(out.ints), [](const float value) { return std::bit_cast<std::int32_t>(value); });
						}

						for (const auto& key : keyed->keys)
						{
							for (const auto code : key.codes)
							{
								if (keyed->code_bits == 8)
								{
									out.random_bytes.push_back(static_cast<std::uint8_t>(code));
								}
								else
								{
									out.random_shorts.push_back(static_cast<std::int16_t>(code));
								}
							}
						}
					}
					else if (const auto* constant = std::get_if<asset_data::constant_translation>(&translation))
					{
						std::ranges::transform(constant->value, std::back_inserter(out.ints), [](const float value) { return std::bit_cast<std::int32_t>(value); });
					}
				}
			}
		}
	}

	std::vector<xanim_bone> decode(source_streams& in, const std::span<const std::uint8_t, 10> groups, std::vector<std::string> names)
	{
		return read_bones(in, groups, std::move(names));
	}

	std::vector<const xanim_bone*> encode(const std::vector<xanim_bone>& bones, const std::uint32_t frames, encoded_streams& out,
		const std::span<std::uint8_t, 10> groups)
	{
		auto ordered = native_order(bones);
		write_bones(ordered, frames, out, groups);
		return ordered;
	}
}
