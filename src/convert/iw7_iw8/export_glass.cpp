#include <std_include.hpp>
#include "export.hpp"

namespace zonetool::convert::iw7_iw8
{
	namespace
	{
		// Glass vertices count in 1/32 units, as Replay's glass dimensions do.
		constexpr float glass_unit = 1.0f / 32.0f;
		// The brush model of a pane is as deep as 16 half thicknesses (the glass of an IW3 pane 8 units deep has the 0.5
		// that Replay's FxGlassDef stores), so that its depth carries the thickness of the glass.
		constexpr float pane_depth_scale = 16.0f;
		// Replay's FxGlassDef.halfThickness is read as at least 1/8 of a unit (Load_FxGlassDef).
		constexpr float min_half_thickness = 0.125f;

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

	// An IW7 glass piece holds its outline in the plane of its frame (x and y in 1/32 units). Its definition gives its
	// materials, its thickness, its colour and its high-mip and crack settings; the piece gives its origin, frame,
	// texture coordinates, outline and support mask (FxGlassInitPieceState). The native pane keeps the outline, the frame
	// origin and the texture origin as IW7 has them: Replay's piece geometry is read the same way (FX_GlassPiece_EmitTransformedVerts
	// 0x14180F930: vertCount vertices, then fanDataCount four-byte units, each transformed by the piece's frame).
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
			if (piece.vertCount < 3 || first + piece.vertCount + piece.fanDataCount > glass.initGeoDataCount ||
				piece.defIndex >= glass.defCount)
			{
				continue;
			}

			// The pipeline's brush bounds and surfaces take the rectangle of the outline.
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
			// The shards draw FxGlassDef.materialShattered; a definition without one keeps its intact material.
			pane.material_shattered = source.resolve(iw7::ASSET_TYPE_MATERIAL, def.materialShattered);
			if (!pane.material_shattered)
			{
				pane.material_shattered = pane.material;
			}

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

			// The native pane takes IW7's frame origin, since the outline, the texture origin and the radius are relative to it
			// (an outline is not always centred on its origin: 8 of the 1578 pieces over the 24 glass maps are not).
			pane.origin = {piece.frame.origin[0], piece.frame.origin[1], piece.frame.origin[2]};
			// The centre of the bounding rectangle is where the box of a pane without collision goes.
			const std::array<float, 2> centre{(low[0] + high[0]) * 0.5f, (low[1] + high[1]) * 0.5f};
			const auto centre_local = rotate(piece.frame.quat, {centre[0] * glass_unit, centre[1] * glass_unit, 0.0f});
			for (auto axis = 0; axis < 3; axis++)
			{
				pane.centre[axis] = piece.frame.origin[axis] + centre_local[axis];
			}

			for (auto axis = 0; axis < 4; axis++)
			{
				pane.quaternion[axis] = piece.frame.quat[axis];
			}

			pane.half_width = (high[0] - low[0]) * 0.5f * glass_unit;
			pane.half_height = (high[1] - low[1]) * 0.5f * glass_unit;
			// Its texture gradients are IW7's (per 1/32 unit, the same units as the outline): uv.u = o.u + x * texVecs[0][0] +
			// y * texVecs[1][0] and uv.v likewise, which Replay's FxGlassDef.texVecs takes as the gradient of u followed by
			// the gradient of v.
			pane.tex_vecs = {def.texVecs[0][0], def.texVecs[1][0], def.texVecs[0][1], def.texVecs[1][1]};
			pane.tex_coord_origin = {piece.texCoordOrigin[0], piece.texCoordOrigin[1]};
			// The outline as IW7 stores it: its vertices, then its fan units.
			const auto units = static_cast<std::size_t>(piece.vertCount) + piece.fanDataCount;
			pane.geometry.reserve(units);
			for (auto k = 0u; k < units; k++)
			{
				const auto& unit = glass.initGeoData[first + k].vert;
				pane.geometry.push_back({unit.x, unit.y});
			}

			pane.vertex_count = piece.vertCount;
			pane.fan_count = piece.fanDataCount;
			pane.area_x2 = piece.areaX2;
			pane.radius = piece.radius;

			// FxGlassDef.halfThickness is the pane's own thickness (not the depth of its brush model, which the pipeline
			// used to derive it from for IW3 panes, and which is zero for a pane out of the axis planes).
			pane.half_thickness = def.halfThickness;
			if (pane.half_thickness < min_half_thickness)
			{
				ZONETOOL_WARNING("iw7: glass piece %u has a half thickness of %g; Replay's glass is at least %g thick",
					i, def.halfThickness, min_half_thickness);
				pane.half_thickness = min_half_thickness;
			}

			pane.color = def.color.packed;
			pane.high_mip_radius_inv_sq = def.highMipRadiusInvSq;
			pane.shattered_high_mip_radius_inv_sq = def.shatteredHighMipRadiusInvSq;
			pane.crack_rings = def.numCrackRings;
			pane.support_mask = piece.supportMask;
			pane.piece = i;

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
