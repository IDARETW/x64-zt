#pragma once

// Emitted by tools/gen_iw8_neutral.py from tools/neutral_families/gameplay.json: do not edit.
#include <core/assets/gameplay_data.hpp>
#include "resolver.hpp"
#include "../structs.hpp"

#include <expected>
#include <string>

namespace zonetool
{
	class zone_memory;
}

// Replay 1.20 gameplay assets to and from neutral data (docs/NEUTRAL_ASSETS.md).
namespace zonetool::iw8::neutral
{
	std::expected<asset_data::gameplay::animpkg, std::string> to_neutral(const WeaponAnimPackage& native);
	WeaponAnimPackage* from_neutral(const asset_data::gameplay::animpkg& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::gameplay::animpkg& data);

	std::expected<asset_data::gameplay::sfxpkg, std::string> to_neutral(const WeaponSFXPackage& native);
	WeaponSFXPackage* from_neutral(const asset_data::gameplay::sfxpkg& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::gameplay::sfxpkg& data);

	std::expected<asset_data::gameplay::vfxpkg, std::string> to_neutral(const WeaponVFXPackage& native);
	WeaponVFXPackage* from_neutral(const asset_data::gameplay::vfxpkg& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::gameplay::vfxpkg& data);

	std::expected<asset_data::gameplay::tracer, std::string> to_neutral(const TracerDef& native);
	TracerDef* from_neutral(const asset_data::gameplay::tracer& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::gameplay::tracer& data);

	std::expected<asset_data::gameplay::laser, std::string> to_neutral(const LaserDef& native);
	LaserDef* from_neutral(const asset_data::gameplay::laser& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::gameplay::laser& data);

	std::expected<asset_data::gameplay::camera, std::string> to_neutral(const CameraDef& native);
	CameraDef* from_neutral(const asset_data::gameplay::camera& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::gameplay::camera& data);

	std::expected<asset_data::gameplay::hudoutline, std::string> to_neutral(const HudOutlineDef& native);
	HudOutlineDef* from_neutral(const asset_data::gameplay::hudoutline& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::gameplay::hudoutline& data);

	std::expected<asset_data::gameplay::footstepvfx, std::string> to_neutral(const FootstepVFX& native);
	FootstepVFX* from_neutral(const asset_data::gameplay::footstepvfx& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::gameplay::footstepvfx& data);

	std::expected<asset_data::gameplay::xcompositemodel, std::string> to_neutral(const XCompositeModelDef& native);
	XCompositeModelDef* from_neutral(const asset_data::gameplay::xcompositemodel& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::gameplay::xcompositemodel& data);

	std::expected<asset_data::gameplay::xanimcurve, std::string> to_neutral(const XAnimCurve& native);
	XAnimCurve* from_neutral(const asset_data::gameplay::xanimcurve& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::gameplay::xanimcurve& data);

	std::expected<asset_data::gameplay::keyvaluepairs, std::string> to_neutral(const KeyValuePairs& native);
	KeyValuePairs* from_neutral(const asset_data::gameplay::keyvaluepairs& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::gameplay::keyvaluepairs& data);

	std::expected<asset_data::gameplay::blendspace2d, std::string> to_neutral(const BlendSpace2DDef& native);
	BlendSpace2DDef* from_neutral(const asset_data::gameplay::blendspace2d& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::gameplay::blendspace2d& data);

	std::expected<asset_data::gameplay::camo, std::string> to_neutral(const Camo& native);
	Camo* from_neutral(const asset_data::gameplay::camo& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::gameplay::camo& data);

	std::expected<asset_data::gameplay::accessory, std::string> to_neutral(const AccessoryDef& native);
	AccessoryDef* from_neutral(const asset_data::gameplay::accessory& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::gameplay::accessory& data);

	std::expected<asset_data::gameplay::locdmgtable, std::string> to_neutral(const LocDmgTable& native);
	LocDmgTable* from_neutral(const asset_data::gameplay::locdmgtable& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::gameplay::locdmgtable& data);

	std::expected<asset_data::gameplay::bulletpenetration, std::string> to_neutral(const BulletPenetration& native);
	BulletPenetration* from_neutral(const asset_data::gameplay::bulletpenetration& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::gameplay::bulletpenetration& data);

	std::expected<asset_data::gameplay::cinematicmotion, std::string> to_neutral(const CinematicMotionDef& native);
	CinematicMotionDef* from_neutral(const asset_data::gameplay::cinematicmotion& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::gameplay::cinematicmotion& data);

	std::expected<asset_data::gameplay::decalvolumemask, std::string> to_neutral(const GfxDecalVolumeMask& native);
	GfxDecalVolumeMask* from_neutral(const asset_data::gameplay::decalvolumemask& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::gameplay::decalvolumemask& data);

	std::expected<asset_data::gameplay::gesture, std::string> to_neutral(const Gesture& native);
	Gesture* from_neutral(const asset_data::gameplay::gesture& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::gameplay::gesture& data);

	std::expected<asset_data::gameplay::reticle, std::string> to_neutral(const ReticleDef& native);
	ReticleDef* from_neutral(const asset_data::gameplay::reticle& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::gameplay::reticle& data);

	std::expected<asset_data::gameplay::streaminginfo, std::string> to_neutral(const StreamingInfo& native);
	StreamingInfo* from_neutral(const asset_data::gameplay::streaminginfo& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::gameplay::streaminginfo& data);

	std::expected<asset_data::gameplay::suit, std::string> to_neutral(const SuitDef& native);
	SuitDef* from_neutral(const asset_data::gameplay::suit& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::gameplay::suit& data);

	std::expected<asset_data::gameplay::suitanimpackage, std::string> to_neutral(const SuitAnimPackage& native);
	SuitAnimPackage* from_neutral(const asset_data::gameplay::suitanimpackage& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::gameplay::suitanimpackage& data);

	std::expected<asset_data::gameplay::coverselector, std::string> to_neutral(const CoverSelector& native);
	CoverSelector* from_neutral(const asset_data::gameplay::coverselector& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::gameplay::coverselector& data);

	std::expected<asset_data::gameplay::enemyselector, std::string> to_neutral(const EnemySelector& native);
	EnemySelector* from_neutral(const asset_data::gameplay::enemyselector& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::gameplay::enemyselector& data);

	std::expected<asset_data::gameplay::carryobject, std::string> to_neutral(const CarryObjectDef& native);
	CarryObjectDef* from_neutral(const asset_data::gameplay::carryobject& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::gameplay::carryobject& data);

	std::expected<asset_data::gameplay::execution, std::string> to_neutral(const ExecutionDef& native);
	ExecutionDef* from_neutral(const asset_data::gameplay::execution& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::gameplay::execution& data);

	std::expected<asset_data::gameplay::clientcharacter, std::string> to_neutral(const ClientCharacter& native);
	ClientCharacter* from_neutral(const asset_data::gameplay::clientcharacter& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::gameplay::clientcharacter& data);

	std::expected<asset_data::gameplay::physicslibrary, std::string> to_neutral(const PhysicsLibrary& native);
	PhysicsLibrary* from_neutral(const asset_data::gameplay::physicslibrary& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::gameplay::physicslibrary& data);

	std::expected<asset_data::gameplay::ttf, std::string> to_neutral(const TTFDef& native);
	TTFDef* from_neutral(const asset_data::gameplay::ttf& data, const asset_resolver& resolve, zone_memory& memory);
	std::size_t native_size(const asset_data::gameplay::ttf& data);

}
