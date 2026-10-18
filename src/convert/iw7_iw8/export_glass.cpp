#include <std_include.hpp>
#include "export.hpp"

namespace zonetool::convert::iw7_iw8
{
	namespace
	{
		// Glass vertices count in 1/32 units, as Replay's glass dimensions do.
		constexpr float glass_unit = 1.0f / 32.0f;
		// The pipeline gives a pane the half thickness of its brush model's depth / 16 (an IW3 pane 8 units deep has
		// the 0.5 of IW7's glass).
		constexpr float pane_depth_scale = 16.0f;

		using vec3 = std::array<float, 3>;

		// Rotates a vector by a quaternion (x, y, z, w).
		vec3 rotate(const float* q, const vec3& v)
		{
			const vec3 u{q[0], q[1], q[2]};
			const auto s = q[3];
			const auto dot = u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
			const vec3 cross{u[1] * v[2] - u[2] * v[1], u[2] * v[0] - u[0] * v[2], u[0] * v[1] - u[1] * v[0]};
			const auto uu = u[0] * u[0] + u[1] * u[1] + u[2] * u[2];
			vec3 result{};
			for (auto i = 0; i < 3; i++)
			{
				result[i] = 2.0f * dot * u[i] + (s * s - uu) * v[i] + 2.0f * s * cross[i];
			}

			return result;
		}
	}

	unsigned int glass_brush_model(const source_map& source, const unsigned int pane)
	{
		return trigger_brush_model(source, source.map_ents()->trigger.count) + pane;
	}

	// An IW7 glass piece holds its outline in the plane of its frame (x and y in 1/32 units); its definition gives its
	// material and the texture coordinates per unit of the outline.
	std::vector<glass_pane> glass_panes(const source_map& source)
	{
		std::vector<glass_pane> result;
		const auto fx_world = source.fx_world();
		if (!fx_world)
		{
			return result;
		}

		const auto& glass = fx_world->glassSys;
		std::size_t offset{};
		for (auto i = 0u; i < glass.initPieceCount; i++)
		{
			const auto& piece = glass.initPieceStates[i];
			const auto first = offset;
			offset += piece.vertCount + piece.fanDataCount;
			if (piece.vertCount < 3 || first + piece.vertCount > glass.initGeoDataCount || piece.defIndex >= glass.defCount)
			{
				continue;
			}

			// The pipeline takes rectangular panes: the rectangle of the outline.
			auto low = std::array<float, 2>{FLT_MAX, FLT_MAX};
			auto high = std::array<float, 2>{-FLT_MAX, -FLT_MAX};
			for (auto k = 0u; k < piece.vertCount; k++)
			{
				const auto& vertex = glass.initGeoData[first + k].vert;
				low = {std::min(low[0], static_cast<float>(vertex.x)), std::min(low[1], static_cast<float>(vertex.y))};
				high = {std::max(high[0], static_cast<float>(vertex.x)), std::max(high[1], static_cast<float>(vertex.y))};
			}

			const auto& def = glass.defs[piece.defIndex];
			glass_pane pane{};
			pane.material = source.resolve(iw7::ASSET_TYPE_MATERIAL, def.material);
			pane.normal = rotate(piece.frame.quat, {0.0f, 0.0f, 1.0f});
			pane.tangent = rotate(piece.frame.quat, {1.0f, 0.0f, 0.0f});
			pane.mins = {FLT_MAX, FLT_MAX, FLT_MAX};
			pane.maxs = {-FLT_MAX, -FLT_MAX, -FLT_MAX};
			const std::array<std::array<float, 2>, 4> outline{{{low[0], low[1]}, {high[0], low[1]}, {high[0], high[1]}, {low[0], high[1]}}};
			for (auto k = 0; k < 4; k++)
			{
				const auto local = rotate(piece.frame.quat, {outline[k][0] * glass_unit, outline[k][1] * glass_unit, 0.0f});
				for (auto axis = 0; axis < 3; axis++)
				{
					pane.corners[k][axis] = piece.frame.origin[axis] + local[axis];
				}

				pane.uvs[k] = {piece.texCoordOrigin[0] + outline[k][0] * def.texVecs[0][0] + outline[k][1] * def.texVecs[1][0],
					piece.texCoordOrigin[1] + outline[k][0] * def.texVecs[0][1] + outline[k][1] * def.texVecs[1][1]};
			}

			// The depth of the brush model (along the normal) carries the thickness of the glass.
			const auto depth = def.halfThickness * pane_depth_scale * 0.5f;
			for (const auto& corner : pane.corners)
			{
				for (auto axis = 0; axis < 3; axis++)
				{
					const auto extent = std::abs(pane.normal[axis]) * depth;
					pane.mins[axis] = std::min(pane.mins[axis], corner[axis] - extent);
					pane.maxs[axis] = std::max(pane.maxs[axis], corner[axis] + extent);
				}
			}

			result.push_back(pane);
		}

		return result;
	}
}
