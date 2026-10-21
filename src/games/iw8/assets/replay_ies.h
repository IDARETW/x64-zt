#pragma once

#include "../zone/iw8_zone.h"

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace iw8::ies
{
// The world's IES lookup strip (GfxWorldDraw +0xF0) and the IES LightDefs that
// index it, in the layout of shipped Replay MP maps (mp_hideout, mp_shipment):
//  - "*ieslookup": 512x1 R8 (GfxPixelFormat 1), 8 levels, one 128-texel segment
//    per profile: 0 = white (lights without a profile), then IES_16 and
//    ies_nvg_flashlight, then an empty segment (the strip is a power of two).
//  - LightDefs "flashlight" (IES_16) and "light_ies_nvg_flashlight": stock maps
//    carry them for the weapon flashlight and the NVG illuminator, which look
//    them up by name in the loaded map. IW3 has no IES lights, so converted
//    local lights keep their profile-less definitions (segment 0).
constexpr const char *kLookupName = "*ieslookup";

// The IES profile of a source light definition (IW7): 128 texels in a segment's
// sampling (texel j = profile coordinate j / 255, the spot's forward
// hemisphere), registered as a LightDef of the definition's name.
struct Profile
{
    std::string definition, profile;
    std::array<uint8_t, 128> texels{};
};

// Registers the strip and the LightDefs (the stock two, then `profiles` from
// segment 3 on, the strip growing by powers of two); must precede the gfx_map asset.
void Register(ZoneWriter &writer, const std::vector<Profile> &profiles = {});
} // namespace iw8::ies
