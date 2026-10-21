#include <std_include.hpp>
#include "replay_ies.h"

#include "../iw8_structs.h"

#include <array>
#include <cstring>
#include <string>
#include <vector>

namespace iw8::ies
{
namespace
{
#include "replay_ies_data.inc"

template <class T> void Store(uint8_t *record, const size_t offset, const T value)
{
    std::memcpy(record + offset, &value, sizeof(value));
}

constexpr unsigned kSegment = 128;
constexpr unsigned kStockSegments = 3; // white, IES_16, ies_nvg_flashlight
constexpr unsigned kLevels = 8; // down to one texel per segment

// Segments of a strip with `profiles` source profiles after the stock ones: 4 as shipped, else the next power of two.
unsigned SegmentCount(const size_t profiles)
{
    unsigned segments = 4;
    while (segments < kStockSegments + profiles)
        segments *= 2;
    return segments;
}

// The mip chain of a source profile laid out as the stock profiles' data: level 0, then each level the mean of the
// texel pairs of the one above.
std::vector<uint8_t> ProfileChain(const Profile &profile)
{
    std::vector<uint8_t> chain(profile.texels.begin(), profile.texels.end());
    for (unsigned texels = kSegment / 2; texels; texels /= 2)
    {
        const size_t above = chain.size() - texels * 2;
        for (unsigned texel = 0; texel < texels; ++texel)
            chain.push_back(static_cast<uint8_t>((chain[above + texel * 2] + chain[above + texel * 2 + 1] + 1) / 2));
    }
    return chain;
}

// Shipped header (mp_hideout "*ieslookup"): format 1 (R8), flags 0x18001,
// semantic 1, category 1, 8 levels, byte 0x31 = 4. Each level starts on a
// 16-byte boundary; the size field covers the padded total.
std::vector<uint8_t> BuildStrip(const std::vector<Profile> &sources)
{
    std::vector<std::vector<uint8_t>> chains;
    for (const auto &source : sources)
        chains.push_back(ProfileChain(source));
    const unsigned segments = SegmentCount(sources.size());
    std::vector<const uint8_t *> profiles(segments, nullptr);
    profiles[1] = kIes16.data();
    profiles[2] = kIesNvgFlashlight.data();
    for (size_t source = 0; source < chains.size(); ++source)
        profiles[kStockSegments + source] = chains[source].data();
    std::vector<uint8_t> pixels;
    size_t chainOffset = 0;
    for (unsigned level = 0; level < kLevels; ++level)
    {
        const unsigned texels = kSegment >> level;
        for (unsigned segment = 0; segment < segments; ++segment)
        {
            if (segment == 0)
                pixels.insert(pixels.end(), texels, 0xFF);
            else if (!profiles[segment])
                pixels.insert(pixels.end(), texels, 0x00);
            else
                pixels.insert(pixels.end(), profiles[segment] + chainOffset,
                              profiles[segment] + chainOffset + texels);
        }
        chainOffset += texels;
        pixels.resize((pixels.size() + 15) & ~size_t{15});
    }
    return pixels;
}

// GfxLightDef (0x20): name +0x00, iesProfile +0x08, firstPixel +0x10,
// coordOffset +0x14, coordScale +0x18; both strings follow inline. A profile
// spans its segment: offset = (firstPixel + 0.5) / width, scale = 255 / width.
void RegisterLightDef(ZoneWriter &writer, const std::string &name, const std::string &profile,
                      const unsigned segment, const unsigned width)
{
    writer.add(ASSET_TYPE_LIGHTDEF, name, [name, profile, segment, width](ZoneWriter &out) {
        std::array<uint8_t, iw8sz::LIGHTDEF> lightDef{};
        const uint32_t firstPixel = segment * kSegment;
        Store(lightDef.data(), 0x00, PTR_FOLLOWS);
        Store(lightDef.data(), 0x08, PTR_FOLLOWS);
        Store(lightDef.data(), 0x10, firstPixel);
        Store(lightDef.data(), 0x14, (float(firstPixel) + 0.5f) / float(width));
        Store(lightDef.data(), 0x18, 255.0f / float(width));
        out.pushStream(XFILE_BLOCK_TEMP_PRELOAD);
        out.align(7);
        out.write(lightDef.data(), lightDef.size());
        out.pushStream(XFILE_BLOCK_VIRTUAL);
        out.writeStr(name.c_str());
        out.writeStr(profile.c_str());
        out.popStream();
        out.popStream();
    });
}
} // namespace

void Register(ZoneWriter &writer, const std::vector<Profile> &profiles)
{
    const unsigned width = SegmentCount(profiles.size()) * kSegment;
    writer.add(ASSET_TYPE_IMAGE, kLookupName, [pixels = BuildStrip(profiles), width](ZoneWriter &out) {
        out.pushStream(XFILE_BLOCK_TEMP_PRELOAD);
        out.align(15);
        std::array<uint8_t, iw8sz::IMAGE> image{};
        Store(image.data(), 0x00, PTR_FOLLOWS);
        Store(image.data(), 0x14, uint32_t{1});
        Store(image.data(), 0x18, uint32_t{0x18001});
        Store(image.data(), 0x1C, static_cast<uint32_t>(pixels.size()));
        Store(image.data(), 0x24, static_cast<uint16_t>(width));
        Store(image.data(), 0x26, uint16_t{1});
        Store(image.data(), 0x28, uint16_t{1});
        Store(image.data(), 0x2A, uint16_t{1});
        image[0x2E] = 1; // semantic
        image[0x2F] = 1; // category
        image[0x30] = kLevels;
        image[0x31] = 4;
        Store(image.data(), 0xE0, PTR_FOLLOWS);
        out.write(image.data(), image.size());
        out.pushStream(XFILE_BLOCK_VIRTUAL);
        out.writeStr(kLookupName);
        out.pushStream(XFILE_BLOCK_TEMP_PRELOAD);
        out.align(15);
        out.write(pixels.data(), pixels.size());
        out.popStream();
        out.popStream();
        out.popStream();
        out.pushStream(XFILE_BLOCK_TEMP_POSTLOAD);
        out.align(15);
        out.reserveCalc(iw8sz::IMAGE);
        out.popStream();
    });
    RegisterLightDef(writer, "flashlight", "IES_16", 1, width);
    RegisterLightDef(writer, "light_ies_nvg_flashlight", "ies_nvg_flashlight", 2, width);
    for (size_t source = 0; source < profiles.size(); ++source)
        RegisterLightDef(writer, profiles[source].definition, profiles[source].profile,
                         kStockSegments + static_cast<unsigned>(source), width);
}
} // namespace iw8::ies
