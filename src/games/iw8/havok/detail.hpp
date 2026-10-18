#pragma once

// Byte, bounds and SIMD tree helpers of the Havok object builders.
namespace zonetool::iw8::havok::detail
{
	template <typename T>
	void put(std::vector<std::uint8_t>& out, const T& value)
	{
		const auto bytes = reinterpret_cast<const std::uint8_t*>(&value);
		out.insert(out.end(), bytes, bytes + sizeof(T));
	}

	template <typename T>
	T get(const std::vector<std::uint8_t>& data, const std::size_t offset)
	{
		if (offset + sizeof(T) > data.size())
		{
			throw std::runtime_error("a Havok object is shorter than its members");
		}

		T value{};
		std::memcpy(&value, data.data() + offset, sizeof(T));
		return value;
	}

	// HK_REAL_MAX: the bounds of an empty AABB are inverted with it.
	constexpr auto real_max = 3.40282e38f;

	// Minimum x, y, z, then maximum x, y, z.
	using bounds = std::array<float, 6>;

	constexpr bounds empty_bounds{real_max, real_max, real_max, -real_max, -real_max, -real_max};

	inline void include(bounds& target, const bounds& source)
	{
		for (std::size_t axis = 0; axis < 3; axis++)
		{
			target[axis] = std::min(target[axis], source[axis]);
			target[axis + 3] = std::max(target[axis + 3], source[axis + 3]);
		}
	}

	inline bounds expanded(bounds box, const float radius)
	{
		for (std::size_t axis = 0; axis < 3; axis++)
		{
			box[axis] -= radius;
			box[axis + 3] += radius;
		}

		return box;
	}

	// An hkcdSimdTree node: the bounds of four children, then four data words (child nodes or, in a leaf, values).
	struct tree_node
	{
		std::array<bounds, 4> children{empty_bounds, empty_bounds, empty_bounds, empty_bounds};
		std::array<std::uint32_t, 4> data{};
		bool leaf{};
	};

	inline std::vector<std::uint8_t> write_nodes(const std::vector<tree_node>& nodes)
	{
		std::vector<std::uint8_t> out;
		for (const auto& node : nodes)
		{
			// lx, hx, ly, hy, lz, hz
			for (std::size_t axis = 0; axis < 3; axis++)
			{
				for (const auto side : {axis, axis + 3})
				{
					for (const auto& child : node.children)
					{
						put(out, child[side]);
					}
				}
			}

			for (const auto word : node.data)
			{
				put(out, word);
			}

			put(out, static_cast<std::uint8_t>(node.leaf));
			out.insert(out.end(), 15, 0);
		}

		return out;
	}
}
