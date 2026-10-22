#pragma once
#include "../zone/iw8_zone.h"
#include <array>
#include <string>
#include <vector>

namespace replaylightgrid
{
struct Grid
{
    std::array<uint32_t, 4> counts{};
    std::vector<uint8_t> payload;
    // IW8GLG02: every tetrahedron is visible (bit 31 of its corners) and gets a fully
    // visible light-visibility record, as stock grids have for the tetrahedra that
    // entities sample.
    bool visibility = false;
    explicit operator bool() const
    {
        return !payload.empty();
    }
};
Grid Load(const std::string &meshPath);
void Emit(iw8::ZoneWriter &writer, const Grid &grid);
} // namespace replaylightgrid
