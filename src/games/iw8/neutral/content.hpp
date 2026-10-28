#pragma once

// Emitted by tools/gen_iw8_neutral.py from tools/neutral_families/content.json: do not edit.
#include <core/assets/content_data.hpp>
#include "resolver.hpp"
#include "../structs.hpp"

#include <expected>
#include <string>

namespace zonetool
{
	class zone_memory;
}

// Replay 1.20 content assets to and from neutral data (docs/NEUTRAL_ASSETS.md).
namespace zonetool::iw8::neutral
{
	std::expected<asset_data::content::attachment, std::string> to_neutral(const WeaponAttachment& native);
	WeaponAttachment* from_neutral(const asset_data::content::attachment& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::content::attachment& data);

	std::expected<asset_data::content::animclass, std::string> to_neutral(const AnimationClass& native);
	AnimationClass* from_neutral(const asset_data::content::animclass& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::content::animclass& data);

	std::expected<asset_data::content::luafile, std::string> to_neutral(const LuaFile& native);
	LuaFile* from_neutral(const asset_data::content::luafile& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::content::luafile& data);

	std::expected<asset_data::content::leaderboarddef, std::string> to_neutral(const LeaderboardDef& native);
	LeaderboardDef* from_neutral(const asset_data::content::leaderboarddef& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::content::leaderboarddef& data);

	std::expected<asset_data::content::virtualleaderboarddef, std::string> to_neutral(const VirtualLeaderboardDef& native);
	VirtualLeaderboardDef* from_neutral(const asset_data::content::virtualleaderboarddef& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::content::virtualleaderboarddef& data);

	std::expected<asset_data::content::clothasset, std::string> to_neutral(const ClothAsset& native);
	ClothAsset* from_neutral(const asset_data::content::clothasset& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::content::clothasset& data);

	std::expected<asset_data::content::stterrain, std::string> to_neutral(const StTerrain& native);
	StTerrain* from_neutral(const asset_data::content::stterrain& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::content::stterrain& data);

	std::expected<asset_data::content::collisiontile, std::string> to_neutral(const CollisionTile& native);
	CollisionTile* from_neutral(const asset_data::content::collisiontile& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::content::collisiontile& data);

	std::expected<asset_data::content::vehicle, std::string> to_neutral(const VehicleDef& native);
	VehicleDef* from_neutral(const asset_data::content::vehicle& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::content::vehicle& data);

	std::expected<asset_data::content::xcam, std::string> to_neutral(const XCam& native);
	XCam* from_neutral(const asset_data::content::xcam& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::content::xcam& data);

	std::expected<asset_data::content::dynentitylist, std::string> to_neutral(const DynEntityList& native);
	DynEntityList* from_neutral(const asset_data::content::dynentitylist& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::content::dynentitylist& data);

}
