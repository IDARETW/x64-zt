#pragma once

// editzone "sun": {"direction"}: the sun-shadow caster scene of a stock GfxWorld that the converter loaded. The world
// surfaces come from the zone in memory; the static models' geometry is streamed (XPak), so ACTS exports it (--geometry
// -a xmodelsurfs, the sidecar the editor's model cache uses).

#include <iw8/common/replay_bounds.h>
#include <iw8/common/replay_sunshadow.h>

#include <array>
#include <cstddef>
#include <filesystem>
#include <string>
#include <vector>

namespace zonetool::iw8
{
	struct GfxWorld;
}

namespace iw8::stocksun
{
	struct GeometrySource
	{
		std::filesystem::path acts;     // acts.exe
		std::filesystem::path game;     // the game folder (game_dx12_ship_replay.exe, oo2core_7_win64.dll, zone\)
		std::filesystem::path zoneFile; // the stock zone the GfxWorld was read from
		std::filesystem::path scratch;  // ACTS output folder (removed by the caller)
		replaybounds::Bounds playRegion; // the static models are taken at the LOD seen from this box
	};

	struct SceneReport
	{
		std::size_t worldSurfaces = 0, worldTriangles = 0, worldSkipped = 0;
		std::size_t modelInstances = 0, modelTriangles = 0, models = 0;
		std::size_t hiddenInstances = 0, nonCastingInstances = 0, splinedInstances = 0, modelSurfacesSkipped = 0;
		std::size_t lowerLodInstances = 0; // instances outside the play region taken at a lower LOD
		std::size_t missingInstances = 0; // instances whose LOD0 geometry ACTS did not give
		std::vector<std::string> missingModels;
		// The 3D box of the world surfaces that cast (the play space and its surroundings, not the sky).
		replaybounds::Bounds worldCasterBounds;
	};

	// Casters: the lit-opaque world surfaces (GfxWorldSurfaces [0, litOpaqueSurfsEnd)) except the sky, and every
	// static model instance of a collection with STATIC_MODEL_COLLECTION_FLAG_SUN_SHADOW_1 (a hidden one, scale 0, and
	// splined ones excepted), the surfaces that cast and are opaque (GfxSModelSurfMatIndirection emitType bit 1, cullType
	// 239) of the LOD a player in source.playRegion sees it at, placed with the instance's translation, quaternion and
	// scale.
	replaysunshadow::Scene BuildScene(const zonetool::iw8::GfxWorld& world, const GeometrySource& source, SceneReport& report);

	// The editor's sun direction convention (worldspawn sundirection, IW3): pitch and yaw in degrees, toward the sun
	// (cos p cos y, cos p sin y, -sin p); a negative pitch is above the horizon.
	std::array<float, 3> TowardSun(double pitch, double yaw);
	void Angles(const std::array<float, 3>& towardSun, double& pitch, double& yaw);
}
