#include <std_include.hpp>
#include "iw3_lightgrid.h"

#include <iw8/common/fs_util.h>
#include <iw8/common/log.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <cwctype>
#include <filesystem>
#include <fstream>
#include <limits>
#include <map>
#include <optional>
#include <unordered_map>
#include <span>
#include <stdexcept>
#include <string_view>
#include <vector>

namespace iw3
{
namespace
{
using Json = nlohmann::json;

constexpr std::string_view Magic = "IW8GLG01";
constexpr float SqrtThree = 1.7320508075688772f;
constexpr float SqrtFive = 2.2360679774997897f;

using Probe = std::array<std::array<float, 9>, 3>;

struct Entry
{
    std::uint16_t color{};
    std::uint8_t primaryLight{};
    std::uint8_t traceMask{};
};

struct Cell
{
    std::array<std::int32_t, 3> position{};
    Entry entry{};
};

template <typename T> T Read(const std::span<const std::uint8_t> data, const std::size_t offset)
{
    if (offset > data.size() || sizeof(T) > data.size() - offset)
        throw std::runtime_error("truncated IW3 light-grid data");
    T value{};
    std::memcpy(&value, data.data() + offset, sizeof(value));
    return value;
}

template <typename T> void Append(std::vector<std::uint8_t> &output, const T &value)
{
    const auto *data = reinterpret_cast<const std::uint8_t *>(&value);
    output.insert(output.end(), data, data + sizeof(value));
}

void AppendBytes(std::vector<std::uint8_t> &output, const void *value, const std::size_t size)
{
    const auto *data = static_cast<const std::uint8_t *>(value);
    output.insert(output.end(), data, data + size);
}

std::uint16_t FloatToHalf(const float value)
{
    std::uint32_t bits{};
    std::memcpy(&bits, &value, sizeof(bits));
    const std::uint32_t sign = (bits >> 16) & 0x8000u;
    const std::uint32_t exponent = (bits >> 23) & 0xFFu;
    std::uint32_t mantissa = bits & 0x7FFFFFu;
    if (exponent == 0xFFu)
        return static_cast<std::uint16_t>(sign | (mantissa ? 0x7E00u : 0x7C00u));

    const int adjusted = static_cast<int>(exponent) - 127 + 15;
    if (adjusted >= 31)
        return static_cast<std::uint16_t>(sign | 0x7C00u);
    if (adjusted <= 0)
    {
        if (adjusted < -10)
            return static_cast<std::uint16_t>(sign);
        mantissa |= 0x800000u;
        const unsigned shift = static_cast<unsigned>(14 - adjusted);
        const std::uint32_t rounded =
            (mantissa + ((1u << (shift - 1)) - 1u) + ((mantissa >> shift) & 1u)) >> shift;
        return static_cast<std::uint16_t>(sign | rounded);
    }

    mantissa += 0xFFFu + ((mantissa >> 13) & 1u);
    if (mantissa & 0x800000u)
    {
        mantissa = 0;
        if (adjusted + 1 >= 31)
            return static_cast<std::uint16_t>(sign | 0x7C00u);
        return static_cast<std::uint16_t>(sign | ((adjusted + 1) << 10));
    }
    return static_cast<std::uint16_t>(sign | (adjusted << 10) | (mantissa >> 13));
}

Json ReadJson(const std::filesystem::path &path)
{
    std::ifstream input(path);
    if (!input)
        throw std::runtime_error("cannot read IW3 light-grid metadata " + path.string());
    Json value;
    input >> value;
    return value;
}

std::vector<std::uint8_t> ReadArray(const std::filesystem::path &root, const Json &metadata,
                                    const char *key, const std::size_t expected)
{
    const Json &description = metadata.at(key);
    const std::string relative = description.at("file").get<std::string>();
    const auto declared = description.at("bytes").get<std::uint64_t>();
    const std::filesystem::path relativePath(relative);
    if (relative.empty() || relativePath.is_absolute() || relative.find(':') != std::string::npos ||
        relative.find('\\') != std::string::npos ||
        std::ranges::find(relativePath, "..") != relativePath.end() || declared != expected ||
        declared > 64ull * 1024 * 1024)
        throw std::runtime_error(std::string("invalid IW3 light-grid ") + key + " descriptor");

    std::error_code error;
    const auto canonicalRoot = std::filesystem::weakly_canonical(root, error);
    if (error)
        throw std::runtime_error("cannot resolve IW3 light-grid root");
    const auto path = std::filesystem::weakly_canonical(root / relativePath, error);
    const auto &rootText = canonicalRoot.native();
    const auto &pathText = path.native();
    const bool rootPrefix = !error && pathText.size() >= rootText.size() &&
                            std::equal(rootText.begin(), rootText.end(), pathText.begin(),
                                       [](const wchar_t left, const wchar_t right) {
                                           return std::towlower(left) == std::towlower(right);
                                       });
    const bool componentBoundary =
        rootPrefix && (pathText.size() == rootText.size() ||
                       pathText[rootText.size()] == std::filesystem::path::preferred_separator);
    if (!componentBoundary)
        throw std::runtime_error(std::string("unsafe IW3 light-grid ") + key + " path");

    std::vector<std::uint8_t> data;
    if (!zt::read_file(path.string(), data) || data.size() != expected)
        throw std::runtime_error(std::string("incorrect IW3 light-grid ") + key + " length");
    return data;
}

std::vector<Cell> ReadCells(const std::filesystem::path &root, const Json &metadata,
                            std::vector<std::uint8_t> &palette)
{
    const int rowAxis = metadata.at("row_axis").get<int>();
    const int columnAxis = metadata.at("col_axis").get<int>();
    if (!((rowAxis == 0 && columnAxis == 1) || (rowAxis == 1 && columnAxis == 0)))
        throw std::runtime_error("unsupported IW3 light-grid axes");

    std::array<int, 3> minimum{};
    std::array<int, 3> maximum{};
    for (std::size_t axis = 0; axis < 3; ++axis)
    {
        minimum[axis] = metadata.at("mins").at(axis).get<int>();
        maximum[axis] = metadata.at("maxs").at(axis).get<int>();
        if (minimum[axis] < 0 || maximum[axis] < minimum[axis] || maximum[axis] > 65535)
            throw std::runtime_error("invalid IW3 light-grid bounds");
    }
    const std::size_t rows = static_cast<std::size_t>(maximum[rowAxis] - minimum[rowAxis] + 1);
    const std::size_t entryCount = metadata.at("entry_count").get<std::size_t>();
    const std::size_t colorCount = metadata.at("color_count").get<std::size_t>();
    if (metadata.at("row_count").get<std::size_t>() != rows || entryCount > 8'388'608 ||
        colorCount > 65'536)
        throw std::runtime_error("invalid IW3 light-grid counts");

    const auto starts = ReadArray(root, metadata, "row_starts", rows * 2);
    const auto rawBytes = metadata.at("row_data").at("bytes").get<std::size_t>();
    const auto raw = ReadArray(root, metadata, "row_data", rawBytes);
    const auto entriesRaw = ReadArray(root, metadata, "entries", entryCount * 4);
    palette = ReadArray(root, metadata, "colors", colorCount * 168);

    std::vector<Entry> entries(entryCount);
    for (std::size_t index = 0; index < entryCount; ++index)
    {
        entries[index].color = Read<std::uint16_t>(entriesRaw, index * 4);
        entries[index].primaryLight = entriesRaw[index * 4 + 2];
        entries[index].traceMask = entriesRaw[index * 4 + 3];
        if (entries[index].color >= colorCount)
            throw std::runtime_error("IW3 light-grid entry references a missing color");
    }

    std::vector<Cell> cells;
    cells.reserve(entryCount);
    std::size_t expectedEntry = 0;
    std::size_t previousEnd = 0;
    for (std::size_t row = 0; row < rows; ++row)
    {
        const std::uint16_t start = Read<std::uint16_t>(starts, row * 2);
        if (start == 0xFFFFu)
            continue;
        std::size_t cursor = static_cast<std::size_t>(start) * 4;
        if (cursor < previousEnd || cursor > raw.size() || 12 > raw.size() - cursor)
            throw std::runtime_error("overlapping or truncated IW3 light-grid row");
        const std::uint16_t firstColumn = Read<std::uint16_t>(raw, cursor);
        const std::uint16_t width = Read<std::uint16_t>(raw, cursor + 2);
        const std::uint16_t bottom = Read<std::uint16_t>(raw, cursor + 4);
        const std::uint16_t height = Read<std::uint16_t>(raw, cursor + 6);
        const std::uint32_t firstEntry = Read<std::uint32_t>(raw, cursor + 8);
        cursor += 12;
        if (firstEntry != expectedEntry || !width || !height ||
            firstColumn < minimum[columnAxis] || firstColumn + width - 1 > maximum[columnAxis] ||
            bottom < minimum[2] || bottom + height - 1 > maximum[2])
            throw std::runtime_error("invalid IW3 light-grid row range");

        std::size_t consumed = 0;
        while (consumed < width)
        {
            if (cursor > raw.size() || 2 > raw.size() - cursor)
                throw std::runtime_error("truncated IW3 light-grid column run");
            const std::uint8_t run = raw[cursor++];
            const std::uint8_t count = raw[cursor++];
            if (!run || consumed + run > width)
                throw std::runtime_error("invalid IW3 light-grid column run");
            std::uint8_t startZ = 0;
            if (count)
            {
                if (cursor == raw.size())
                    throw std::runtime_error("missing IW3 light-grid column height");
                startZ = raw[cursor++];
                if (startZ + count > height || expectedEntry + std::size_t(run) * count > entryCount)
                    throw std::runtime_error("IW3 light-grid column exceeds its row");
            }
            for (unsigned column = 0; column < run; ++column)
            {
                for (unsigned z = 0; z < count; ++z)
                {
                    std::array<int, 3> coordinate{0, 0, bottom + startZ + static_cast<int>(z)};
                    coordinate[rowAxis] = minimum[rowAxis] + static_cast<int>(row);
                    coordinate[columnAxis] = firstColumn + static_cast<int>(consumed + column);
                    Cell cell;
                    cell.position = {coordinate[0] * 32 - 131072,
                                     coordinate[1] * 32 - 131072,
                                     coordinate[2] * 64 - 131072};
                    cell.entry = entries[expectedEntry++];
                    cells.push_back(cell);
                }
            }
            consumed += run;
        }
        previousEnd = (cursor + 3) & ~std::size_t{3};
        if (previousEnd > raw.size() ||
            std::any_of(raw.begin() + cursor, raw.begin() + previousEnd,
                        [](const std::uint8_t value) { return value != 0; }))
            throw std::runtime_error("nonzero or truncated IW3 light-grid row padding");
    }
    if (expectedEntry != entryCount || previousEnd != raw.size())
        throw std::runtime_error("unconsumed IW3 light-grid data");
    return cells;
}

std::array<float, 9> EvaluateShBasis(const std::array<float, 3> &direction)
{
    const auto [x, y, z] = direction;
    return {0.2820947918f,
            -0.4886025119f * y,
            0.4886025119f * z,
            -0.4886025119f * x,
            1.0925484306f * x * y,
            -1.0925484306f * y * z,
            0.3153915653f * (3.0f * z * z - 1.0f),
            -1.0925484306f * x * z,
            0.5462742153f * (x * x - y * y)};
}

// IW3 light-grid colour s is the light along the direction of shell texel s of
// the 4x4x4 model-lighting volume itself: the lit model shaders index the volume
// with the world normal (ps_lp_sun_t0c0n0: uvw = N * lightingLookupScale /
// max|N| + base). IW3's s_lightGridRotAxis only maps the legacy 8-corner colours
// into them (R_ModernizeLegacyLightGridColors). Nuketown's colours fit an L1
// that points up (mean direction 0.02, 0.06, 0.94); through the rotation it
// pointed along the cube diagonal (0.56, 0.58, 0.49).
const std::array<std::array<double, 56>, 9> &LightGridProjection()
{
    static const auto projection = [] {
        std::array<std::array<double, 9>, 56> samples{};
        std::size_t sampleIndex = 0;
        for (int z = 0; z < 4; ++z)
        {
            for (int y = 0; y < 4; ++y)
            {
                for (int x = 0; x < 4; ++x)
                {
                    if (x > 0 && x < 3 && y > 0 && y < 3 && z > 0 && z < 3)
                        continue;
                    const std::array<float, 3> delta{
                        x * (2.0f / 3.0f) - 1.0f,
                        y * (2.0f / 3.0f) - 1.0f,
                        z * (2.0f / 3.0f) - 1.0f};
                    std::array<float, 3> direction = delta;
                    const float length = std::sqrt(direction[0] * direction[0] +
                                                   direction[1] * direction[1] +
                                                   direction[2] * direction[2]);
                    if (!(length > 0.0f) || sampleIndex >= samples.size())
                        throw std::runtime_error("invalid IW3 light-grid sample basis");
                    for (float &component : direction)
                        component /= length;
                    const auto basis = EvaluateShBasis(direction);
                    for (unsigned coefficient = 0; coefficient < 9; ++coefficient)
                        samples[sampleIndex][coefficient] = basis[coefficient];
                    ++sampleIndex;
                }
            }
        }
        if (sampleIndex != samples.size())
            throw std::runtime_error("incorrect IW3 light-grid sample count");

        std::array<std::array<double, 18>, 9> inverse{};
        for (unsigned row = 0; row < 9; ++row)
        {
            for (unsigned column = 0; column < 9; ++column)
                for (const auto &sample : samples)
                    inverse[row][column] += sample[row] * sample[column];
            inverse[row][row + 9] = 1.0;
        }
        for (unsigned column = 0; column < 9; ++column)
        {
            unsigned pivot = column;
            for (unsigned row = column + 1; row < 9; ++row)
                if (std::abs(inverse[row][column]) > std::abs(inverse[pivot][column]))
                    pivot = row;
            if (std::abs(inverse[pivot][column]) < 1.0e-10)
                throw std::runtime_error("singular IW3 light-grid SH projection");
            if (pivot != column)
                std::swap(inverse[pivot], inverse[column]);
            const double divisor = inverse[column][column];
            for (double &value : inverse[column])
                value /= divisor;
            for (unsigned row = 0; row < 9; ++row)
            {
                if (row == column)
                    continue;
                const double factor = inverse[row][column];
                for (unsigned entry = 0; entry < 18; ++entry)
                    inverse[row][entry] -= factor * inverse[column][entry];
            }
        }

        std::array<std::array<double, 56>, 9> result{};
        for (unsigned coefficient = 0; coefficient < 9; ++coefficient)
            for (unsigned sample = 0; sample < 56; ++sample)
                for (unsigned term = 0; term < 9; ++term)
                    result[coefficient][sample] +=
                        inverse[coefficient][term + 9] * samples[sample][term];

        for (unsigned expected = 0; expected < 9; ++expected)
        {
            for (unsigned actual = 0; actual < 9; ++actual)
            {
                double value = 0.0;
                for (unsigned sample = 0; sample < 56; ++sample)
                    value += result[actual][sample] * samples[sample][expected];
                if (std::abs(value - (actual == expected ? 1.0 : 0.0)) > 1.0e-5)
                    throw std::runtime_error("invalid IW3 light-grid SH projection");
            }
        }
        return result;
    }();
    return projection;
}

std::vector<Probe> BuildProbes(const std::vector<std::uint8_t> &palette, const float lightUnitScale = 1.0f)
{
    std::vector<Probe> probes(palette.size() / 168);
    const auto &projection = LightGridProjection();
    // IW3 grid samples are shading values in the same 8-bit units as its
    // lightmaps, which the world materials consume unscaled beside the source
    // sun. The former x32/pi gain put dynamic objects at ~10x the ambient of
    // the walls around them: Office's DC sat at 18-20 everywhere against a
    // 3.0 sun. Shipped Shipment keeps outdoor DC near 1.3x its sun intensity;
    // these units give Nuketown ~1.4x, and preserve the source contrast.
    constexpr float sourceScale = 1.0f / 255.0f;
    for (std::size_t probe = 0; probe < probes.size(); ++probe)
    {
        for (unsigned channel = 0; channel < 3; ++channel)
        {
            for (unsigned coefficient = 0; coefficient < 9; ++coefficient)
            {
                double value = 0.0;
                for (unsigned sample = 0; sample < 56; ++sample)
                    value += projection[coefficient][sample] *
                             palette[probe * 168 + sample * 3 + channel];
                probes[probe][channel][coefficient] =
                    static_cast<float>(value * sourceScale * lightUnitScale);
            }
        }
    }
    return probes;
}

std::array<std::uint8_t, 32> PackProbe(const Probe &probe, const std::uint8_t traceMask)
{
    for (const auto &channel : probe)
        for (const float value : channel)
            if (!std::isfinite(value) || std::abs(value) > 60000.0f)
                throw std::runtime_error("invalid IW3 diffuse probe coefficient");
    if (probe[0][0] < 0.0f || probe[1][0] < 0.0f || probe[2][0] < 0.0f)
        throw std::runtime_error("negative IW3 diffuse probe intensity");
    const auto red = FloatToHalf(probe[0][0]);
    const auto green = FloatToHalf(probe[1][0]);
    const auto blue = FloatToHalf(probe[2][0]);
    const std::uint32_t dc = (red >> 4) | (std::uint32_t(green >> 4) << 11) |
                             (std::uint32_t(blue >> 5) << 22);
    std::array<std::uint8_t, 32> packed{};
    std::memcpy(packed.data(), &dc, sizeof(dc));
    // Replay keeps the L1 terms negated against EvaluateShBasis: the probes of
    // stock maps that see the sky point down in this basis (mp_hideout's bright
    // directional probes: mean z -0.74 under a sun 45 degrees up), as IW7's GPU
    // light grid does; packed as they are, converted maps lit models from the
    // side away from their light.
    for (unsigned channel = 0; channel < 3; ++channel)
    {
        const float dcValue = probe[channel][0];
        for (unsigned coefficient = 1; coefficient < 9; ++coefficient)
        {
            float normalized = 0.0f;
            if (dcValue > 0.0f)
            {
                const float range = coefficient < 4 ? SqrtThree : SqrtFive;
                const float value = coefficient < 4 ? -probe[channel][coefficient]
                                                    : probe[channel][coefficient];
                normalized = std::clamp(value / (dcValue * range), -1.0f, 1.0f);
            }
            packed[4 + channel * 8 + coefficient - 1] =
                static_cast<std::uint8_t>(normalized * 127.0f + 127.5f);
        }
    }
    // Replay keeps a per-probe 8-bit visibility/trace value in byte 28 of the
    // otherwise compressed SH record.  IW3 stores the corresponding mask on
    // each light-grid entry, so preserve it byte-for-byte; its bit ordering is
    // already authored and must not be inferred from the primary-light index.
    packed[28] = traceMask;
    packed[29] = 255;
    return packed;
}

float Median(std::vector<float> values)
{
    const std::size_t middle = values.size() / 2;
    std::nth_element(values.begin(), values.begin() + middle, values.end());
    if (values.size() % 2)
        return values[middle];
    const float high = values[middle];
    std::nth_element(values.begin(), values.begin() + middle - 1, values.begin() + middle);
    return (values[middle - 1] + high) * 0.5f;
}

// Triangulate only the convex hull of corners actually present in an IW3 cell.
// The least corner is the fan apex; a four-corner face uses its least corner
// and opposite diagonal, matching the complete-cell boundary triangulation.
std::vector<std::array<unsigned, 4>> SparseCellTetrahedra(const unsigned mask)
{
    constexpr std::array<std::array<int, 3>, 8> points{{
        {0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {1, 1, 0},
        {0, 0, 1}, {1, 0, 1}, {0, 1, 1}, {1, 1, 1}}};
    std::array<unsigned, 8> vertices{};
    unsigned count = 0;
    for (unsigned vertex = 0; vertex < 8; ++vertex)
        if (mask & (1u << vertex))
            vertices[count++] = vertex;
    if (count < 4)
        return {};

    const unsigned anchor = vertices[0];
    std::vector<unsigned> faces;
    for (unsigned i = 0; i < count; ++i)
    {
        for (unsigned j = i + 1; j < count; ++j)
        {
            for (unsigned k = j + 1; k < count; ++k)
            {
                const auto &a = points[vertices[i]];
                const auto &b = points[vertices[j]];
                const auto &c = points[vertices[k]];
                const std::array<int, 3> u{b[0] - a[0], b[1] - a[1], b[2] - a[2]};
                const std::array<int, 3> v{c[0] - a[0], c[1] - a[1], c[2] - a[2]};
                const std::array<int, 3> normal{
                    u[1] * v[2] - u[2] * v[1], u[2] * v[0] - u[0] * v[2],
                    u[0] * v[1] - u[1] * v[0]};
                if (normal == std::array<int, 3>{0, 0, 0})
                    continue;
                bool positive = false;
                bool negative = false;
                unsigned face = 0;
                for (unsigned n = 0; n < count; ++n)
                {
                    const unsigned vertex = vertices[n];
                    const auto &p = points[vertex];
                    const int side = normal[0] * (p[0] - a[0]) +
                                     normal[1] * (p[1] - a[1]) +
                                     normal[2] * (p[2] - a[2]);
                    positive |= side > 0;
                    negative |= side < 0;
                    if (!side)
                        face |= 1u << vertex;
                }
                if (positive == negative)
                    continue;
                if (!(face & (1u << anchor)) &&
                    std::ranges::find(faces, face) == faces.end())
                    faces.push_back(face);
            }
        }
    }

    std::ranges::sort(faces);
    std::vector<std::array<unsigned, 4>> tetrahedra;
    for (const unsigned face : faces)
    {
        std::array<unsigned, 4> corners{};
        unsigned faceCount = 0;
        for (unsigned vertex = 0; vertex < 8; ++vertex)
            if (face & (1u << vertex))
                corners[faceCount++] = vertex;
        if (faceCount == 3)
        {
            tetrahedra.push_back({anchor, corners[0], corners[1], corners[2]});
            continue;
        }
        if (faceCount != 4)
            throw std::runtime_error("unsupported IW3 light-grid hull face");

        unsigned opposite = 1;
        int longest = -1;
        for (unsigned index = 1; index < 4; ++index)
        {
            const auto delta = std::array<int, 3>{
                points[corners[index]][0] - points[corners[0]][0],
                points[corners[index]][1] - points[corners[0]][1],
                points[corners[index]][2] - points[corners[0]][2]};
            const int length = delta[0] * delta[0] + delta[1] * delta[1] +
                               delta[2] * delta[2];
            if (length > longest)
            {
                longest = length;
                opposite = index;
            }
        }
        std::array<unsigned, 2> others{};
        for (unsigned index = 1, next = 0; index < 4; ++index)
            if (index != opposite)
                others[next++] = corners[index];
        tetrahedra.push_back({anchor, corners[0], others[0], corners[opposite]});
        tetrahedra.push_back({anchor, corners[0], corners[opposite], others[1]});
    }
    return tetrahedra;
}

bool SparseTetrahedronContains(const std::array<std::int32_t, 3> &point,
                               const std::array<unsigned, 4> &tetrahedron)
{
    const auto corner = [](const unsigned mask) {
        return std::array<std::int64_t, 3>{(mask & 1) ? 64 : 0, (mask & 2) ? 64 : 0,
                                           (mask & 4) ? 64 : 0};
    };
    const auto difference = [](const auto &a, const auto &b) {
        return std::array<std::int64_t, 3>{a[0] - b[0], a[1] - b[1], a[2] - b[2]};
    };
    const auto determinant = [](const auto &a, const auto &b, const auto &c) {
        return a[0] * (b[1] * c[2] - b[2] * c[1]) +
               a[1] * (b[2] * c[0] - b[0] * c[2]) +
               a[2] * (b[0] * c[1] - b[1] * c[0]);
    };
    const auto origin = corner(tetrahedron[0]);
    const auto a = difference(corner(tetrahedron[1]), origin);
    const auto b = difference(corner(tetrahedron[2]), origin);
    const auto c = difference(corner(tetrahedron[3]), origin);
    const auto q = difference(std::array<std::int64_t, 3>{point[0], point[1], point[2]},
                              origin);
    const auto volume = determinant(a, b, c);
    if (!volume)
        throw std::runtime_error("degenerate IW3 light-grid tetrahedron");
    const auto sign = volume < 0 ? -1 : 1;
    const auto x = sign * determinant(q, b, c);
    const auto y = sign * determinant(a, q, c);
    const auto z = sign * determinant(a, b, q);
    return x >= 0 && y >= 0 && z >= 0 && x + y + z <= sign * volume;
}

std::vector<std::uint8_t> BuildPayload(std::vector<Cell> cells,
                                       const std::vector<std::uint8_t> &palette, const float lightUnitScale = 1.0f)
{
    if (cells.empty() || cells.size() > 2'000'000 || palette.empty() || palette.size() % 168)
        throw std::runtime_error("invalid IW3 light-grid probe or palette count");
    std::ranges::sort(cells, {}, &Cell::position);
    std::map<std::array<std::int32_t, 3>, std::uint32_t> lookup;
    std::array<std::int32_t, 3> low = cells.front().position;
    std::array<std::int32_t, 3> high = cells.front().position;
    for (std::uint32_t index = 0; index < cells.size(); ++index)
    {
        const auto &position = cells[index].position;
        if (std::abs(position[0]) > 100000 || std::abs(position[1]) > 100000 ||
            std::abs(position[2]) > 100000 || position[0] % 32 || position[1] % 32 ||
            position[2] % 64 || !lookup.emplace(position, index).second)
            throw std::runtime_error("invalid, duplicate, or off-lattice IW3 light-grid probe");
        for (unsigned axis = 0; axis < 3; ++axis)
        {
            low[axis] = std::min(low[axis], position[axis]);
            high[axis] = std::max(high[axis], position[axis]);
        }
    }
    auto alignDown = [](const std::int32_t value, const std::int32_t alignment) {
        std::int32_t quotient = value / alignment;
        if (value < 0 && value % alignment)
            --quotient;
        return quotient * alignment;
    };
    auto alignUp = [&](const std::int32_t value, const std::int32_t alignment) {
        return -alignDown(-value, alignment);
    };

    std::array<std::int32_t, 3> gridLow{};
    std::array<std::int32_t, 3> gridHigh{};
    std::array<std::uint32_t, 3> dimensions{};
    std::uint64_t voxelCount = 0;
    std::uint64_t rootCount64 = 0;
    unsigned rootShift = 5;
    for (; rootShift <= 20; ++rootShift)
    {
        const std::int32_t voxelSize = 1 << rootShift;
        voxelCount = 1;
        for (unsigned axis = 0; axis < 3; ++axis)
        {
            gridLow[axis] = alignDown(low[axis], voxelSize);
            gridHigh[axis] = alignUp(high[axis], voxelSize);
            dimensions[axis] =
                static_cast<std::uint32_t>((gridHigh[axis] - gridLow[axis]) / voxelSize);
            if (!dimensions[axis])
                throw std::runtime_error("unsupported IW3 light-grid extent");
            voxelCount *= dimensions[axis];
        }
        rootCount64 = static_cast<std::uint64_t>(dimensions[0]) * dimensions[1];
        if (rootCount64 <= 1'000'000 && voxelCount <= 8'000'000)
            break;
    }
    if (rootShift > 20)
        throw std::runtime_error("IW3 light-grid extent exceeds Replay voxel-tree limits");

    std::vector<std::uint32_t> voxels(static_cast<std::size_t>(voxelCount), UINT32_MAX);
    std::vector<std::array<std::uint32_t, 4>> tetrahedra;
    constexpr std::array<std::array<unsigned, 3>, 6> permutations{{
        {0, 1, 2}, {0, 2, 1}, {1, 0, 2}, {1, 2, 0}, {2, 0, 1}, {2, 1, 0}}};
    std::array<std::vector<std::array<unsigned, 4>>, 256> cellTetrahedra;
    for (unsigned mask = 1; mask < 255; ++mask)
        cellTetrahedra[mask] = SparseCellTetrahedra(mask);
    for (const auto &order : permutations)
    {
        unsigned mask = 0;
        std::array<unsigned, 4> tetrahedron{0, 0, 0, 0};
        for (unsigned step = 0; step < 3; ++step)
        {
            mask |= 1u << order[step];
            tetrahedron[step + 1] = mask;
        }
        cellTetrahedra[255].push_back(tetrahedron);
    }

    // A sparse hull need not contain its lowest corner. Enumerate each possible
    // source-cell base around authored probes, then deduplicate the bases.
    std::vector<std::array<std::int32_t, 3>> candidateBases;
    candidateBases.reserve(cells.size() * 8);
    for (const Cell &cell : cells)
        for (unsigned mask = 0; mask < 8; ++mask)
            candidateBases.push_back({cell.position[0] - ((mask & 1) ? 32 : 0),
                                      cell.position[1] - ((mask & 2) ? 32 : 0),
                                      cell.position[2] - ((mask & 4) ? 64 : 0)});
    std::ranges::sort(candidateBases);
    candidateBases.erase(std::unique(candidateBases.begin(), candidateBases.end()),
                         candidateBases.end());

    // Keep complete cells first: their tetrahedron numbers and voxel starts are
    // stable, while sparse cells contribute only their authored convex hulls.
    std::size_t octantStarts = 0;
    for (const bool sparsePass : {false, true})
    {
        const std::size_t baseCount = sparsePass ? candidateBases.size() : cells.size();
        for (std::size_t baseIndex = 0; baseIndex < baseCount; ++baseIndex)
        {
            const auto &base = sparsePass ? candidateBases[baseIndex] : cells[baseIndex].position;
            std::array<std::uint32_t, 8> corners{};
            unsigned presentMask = 0;
            for (unsigned mask = 0; mask < 8; ++mask)
            {
                const std::array<std::int32_t, 3> position{
                    base[0] + ((mask & 1) ? 32 : 0), base[1] + ((mask & 2) ? 32 : 0),
                    base[2] + ((mask & 4) ? 64 : 0)};
                const auto found = lookup.find(position);
                if (found == lookup.end())
                    continue;
                corners[mask] = found->second;
                presentMask |= 1u << mask;
            }
            if ((presentMask == 255) == sparsePass)
                continue;
            const auto &localTetrahedra = cellTetrahedra[presentMask];
            if (localTetrahedra.empty())
                continue;
            std::vector<std::uint32_t> starts;
            starts.reserve(localTetrahedra.size());
            for (const auto &local : localTetrahedra)
            {
                if (tetrahedra.size() >= 8'000'000)
                    throw std::runtime_error("IW3 light-grid tetrahedra exceed Replay limits");
                starts.push_back(static_cast<std::uint32_t>(tetrahedra.size()));
                tetrahedra.push_back({corners[local[0]], corners[local[1]],
                                      corners[local[2]], corners[local[3]]});
            }
            const std::int32_t voxelSize = 1 << rootShift;
            std::array<std::uint32_t, 3> firstVoxel{};
            std::array<std::uint32_t, 3> lastVoxel{};
            constexpr std::array<std::int32_t, 3> sourceCellSize{32, 32, 64};
            bool intersectsGrid = true;
            for (unsigned axis = 0; axis < 3; ++axis)
            {
                const auto start = std::max(base[axis], gridLow[axis]);
                const auto end = std::min(base[axis] + sourceCellSize[axis], gridHigh[axis]);
                if (end <= start)
                {
                    intersectsGrid = false;
                    break;
                }
                firstVoxel[axis] = static_cast<std::uint32_t>((start - gridLow[axis]) / voxelSize);
                lastVoxel[axis] =
                    static_cast<std::uint32_t>((end - 1 - gridLow[axis]) / voxelSize);
            }
            if (!intersectsGrid)
                continue;
            for (std::uint32_t y = firstVoxel[1]; y <= lastVoxel[1]; ++y)
            {
                for (std::uint32_t x = firstVoxel[0]; x <= lastVoxel[0]; ++x)
                {
                    for (std::uint32_t z = firstVoxel[2]; z <= lastVoxel[2]; ++z)
                    {
                        const std::size_t voxel =
                            (static_cast<std::size_t>(y) * dimensions[0] + x) * dimensions[2] + z;
                        if (voxels[voxel] != UINT32_MAX)
                            continue;
                        if (!sparsePass)
                        {
                            voxels[voxel] = starts[0];
                            continue;
                        }
                        // A sparse voxel starts a walk only when its center or
                        // an octant center lies inside an authored simplex.
                        // This reaches partial voxels without extending the hull.
                        for (unsigned sample = 0; sample < 9 && voxels[voxel] == UINT32_MAX;
                             ++sample)
                        {
                            const unsigned octant = sample ? sample - 1 : 0;
                            const std::array<std::int32_t, 3> offset{
                                static_cast<std::int32_t>(voxelSize *
                                    (sample ? ((octant & 1) ? 3 : 1) : 2) / 4),
                                static_cast<std::int32_t>(voxelSize *
                                    (sample ? ((octant & 2) ? 3 : 1) : 2) / 4),
                                static_cast<std::int32_t>(voxelSize *
                                    (sample ? ((octant & 4) ? 3 : 1) : 2) / 4)};
                            const std::array<std::int32_t, 3> coordinate{
                                2 * (gridLow[0] + static_cast<std::int32_t>(x) * voxelSize +
                                     offset[0] - base[0]),
                                2 * (gridLow[1] + static_cast<std::int32_t>(y) * voxelSize +
                                     offset[1] - base[1]),
                                gridLow[2] + static_cast<std::int32_t>(z) * voxelSize +
                                    offset[2] - base[2]};
                            for (std::size_t localIndex = 0;
                                 localIndex < localTetrahedra.size(); ++localIndex)
                            {
                                if (SparseTetrahedronContains(coordinate,
                                                              localTetrahedra[localIndex]))
                                {
                                    voxels[voxel] = starts[localIndex];
                                    octantStarts += sample != 0;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if (tetrahedra.empty())
        throw std::runtime_error("IW3 light-grid has no authored tetrahedra");

    // Replay treats UINT32_MAX as an unassigned voxel and samples its fallback
    // probe.  Preserve sparse IW3 columns instead of extending tetrahedra into
    // cells without authored light-grid coverage.
    const std::size_t unassignedVoxelCount =
        std::count(voxels.begin(), voxels.end(), UINT32_MAX);
    if (unassignedVoxelCount == voxels.size())
        throw std::runtime_error("IW3 light-grid has no populated Replay voxels");
    zt::info("iw3: retained %zu unassigned Replay light-grid voxels and %zu authored starts "
             "(%zu sparse octant starts)", unassignedVoxelCount,
             voxels.size() - unassignedVoxelCount, octantStarts);

    std::vector<std::array<std::uint32_t, 4>> neighbors(tetrahedra.size(),
                                                        {UINT32_MAX, UINT32_MAX, UINT32_MAX,
                                                         UINT32_MAX});
    std::map<std::array<std::uint32_t, 3>, std::pair<std::uint32_t, unsigned>> faces;
    for (std::uint32_t index = 0; index < tetrahedra.size(); ++index)
    {
        for (unsigned face = 0; face < 4; ++face)
        {
            std::array<std::uint32_t, 3> key{};
            for (unsigned source = 0, target = 0; source < 4; ++source)
                if (source != face)
                    key[target++] = tetrahedra[index][source];
            std::ranges::sort(key);
            const auto found = faces.find(key);
            if (found == faces.end())
            {
                faces.emplace(key, std::pair{index, face});
            }
            else
            {
                const auto [otherIndex, otherFace] = found->second;
                neighbors[index][face] = otherIndex;
                neighbors[otherIndex][otherFace] = index;
                faces.erase(found);
            }
        }
    }

    const auto probes = BuildProbes(palette, lightUnitScale);
    std::array<float, 32> fallback{};
    for (unsigned channel = 0; channel < 3; ++channel)
    {
        for (unsigned coefficient = 0; coefficient < 9; ++coefficient)
        {
            std::vector<float> occupied;
            occupied.reserve(cells.size());
            for (const Cell &cell : cells)
                occupied.push_back(probes[cell.entry.color][channel][coefficient]);
            fallback[channel * 9 + coefficient] = Median(std::move(occupied));
        }
    }
    fallback[27] = 1.0f;
    fallback[28] = 1.0f;

    std::vector<std::uint8_t> output;
    const std::uint32_t probeCount = static_cast<std::uint32_t>(cells.size());
    const std::uint32_t tetrahedronCount = static_cast<std::uint32_t>(tetrahedra.size());
    const std::uint32_t rootCount = static_cast<std::uint32_t>(rootCount64);
    output.reserve(264ull + probeCount * 44ull + tetrahedronCount * 32ull +
                   rootCount * 12ull + voxelCount * 4ull);
    AppendBytes(output, Magic.data(), Magic.size());
    Append(output, probeCount);
    Append(output, tetrahedronCount);
    Append(output, rootCount);
    Append(output, static_cast<std::uint32_t>(voxelCount));

    constexpr float diagonal = 0.5773502691896258f;
    constexpr std::array<std::array<float, 3>, 7> axes{{
        {1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {diagonal, diagonal, diagonal},
        {diagonal, -diagonal, diagonal}, {-diagonal, diagonal, diagonal},
        {-diagonal, -diagonal, diagonal}}};
    for (const auto &axis : axes)
    {
        float minimum = 0.0f;
        float maximum = 0.0f;
        for (unsigned component = 0; component < 3; ++component)
        {
            minimum += axis[component] * (axis[component] >= 0 ? low[component] : high[component]);
            maximum += axis[component] * (axis[component] >= 0 ? high[component] : low[component]);
        }
        Append(output, axis[0]);
        Append(output, axis[1]);
        Append(output, minimum);
        Append(output, maximum);
    }
    for (const float value : fallback)
        Append(output, FloatToHalf(value));

    for (const auto dimension : dimensions)
        Append(output, static_cast<std::int32_t>(dimension));
    for (const std::int32_t value : {0, static_cast<std::int32_t>(rootShift),
                                     std::max(0, static_cast<std::int32_t>(rootShift) - 2),
                                     std::max(0, static_cast<std::int32_t>(rootShift) - 4), 0})
        Append(output, value);
    for (const auto value : gridLow)
        Append(output, static_cast<float>(value));
    Append(output, 0.0f);
    for (const auto value : gridHigh)
        Append(output, static_cast<float>(value));
    Append(output, 0.0f);

    for (const Cell &cell : cells)
    {
        const auto packed = PackProbe(probes[cell.entry.color], cell.entry.traceMask);
        AppendBytes(output, packed.data(), packed.size());
    }
    for (const Cell &cell : cells)
        for (const auto value : cell.position)
            Append(output, static_cast<float>(value));
    for (const auto &tetrahedron : tetrahedra)
        AppendBytes(output, tetrahedron.data(), sizeof(tetrahedron));
    for (const auto &neighbor : neighbors)
        AppendBytes(output, neighbor.data(), sizeof(neighbor));
    for (std::uint32_t y = 0; y < dimensions[1]; ++y)
    {
        for (std::uint32_t x = 0; x < dimensions[0]; ++x)
        {
            Append(output, 0x80000000u | ((y * dimensions[0] + x) * dimensions[2]));
            Append(output, 0u);
            Append(output, dimensions[2] - 1);
        }
    }
    AppendBytes(output, voxels.data(), voxels.size() * sizeof(voxels.front()));
    if (output.size() > 512ull * 1024 * 1024)
        throw std::runtime_error("native Replay light-grid exceeds the supported size");
    zt::info("iw3: converted %u authored light probes into %u native tetrahedra "
             "(%ux%ux%u, %u-unit voxels)",
             probeCount, tetrahedronCount, dimensions[0], dimensions[1], dimensions[2],
             1u << rootShift);
    return output;
}
// A light grid that the extraction keeps as SH probes in a tetrahedral mesh
// (IW7's GPU light grid, written by the IW7 export as <bsp>.gpulightgrid.bin:
// "IW7GLG01", probe and tetrahedron counts, positions, 27 SH floats per probe
// in this file's basis and light-grid units, tetrahedra, neighbours). Replay's
// native grid takes the probes and tetrahedra as they are; each voxel starts
// its walk at the tetrahedron that holds its centre, found by walking the mesh
// from the last voxel's (UINT32_MAX outside the mesh: the fallback probe).
std::vector<std::uint8_t> BuildMeshPayload(const std::vector<std::uint8_t> &source,
                                           const float lightUnitScale)
{
    const std::span<const std::uint8_t> data(source);
    if (source.size() < 16 || std::memcmp(source.data(), "IW7GLG01", 8))
        throw std::runtime_error("unknown extracted light-grid mesh");
    const auto probeCount = Read<std::uint32_t>(data, 8);
    const auto tetrahedronCount = Read<std::uint32_t>(data, 12);
    if (!probeCount || probeCount > 2'000'000 || !tetrahedronCount || tetrahedronCount > 8'000'000 ||
        source.size() != 16ull + probeCount * (12ull + 108ull) + tetrahedronCount * 32ull)
        throw std::runtime_error("inconsistent extracted light-grid mesh");

    std::vector<std::array<double, 3>> positions(probeCount);
    std::vector<Probe> probes(probeCount);
    std::vector<std::array<std::uint32_t, 4>> tetrahedra(tetrahedronCount);
    std::vector<std::array<std::uint32_t, 4>> neighbors(tetrahedronCount);
    std::size_t offset = 16;
    std::array<float, 3> low{FLT_MAX, FLT_MAX, FLT_MAX};
    std::array<float, 3> high{-FLT_MAX, -FLT_MAX, -FLT_MAX};
    for (auto &position : positions)
        for (unsigned axis = 0; axis < 3; ++axis, offset += 4)
        {
            const auto value = Read<float>(data, offset);
            if (!std::isfinite(value) || std::abs(value) > 100000.0f)
                throw std::runtime_error("invalid extracted light-grid probe position");
            position[axis] = value;
            low[axis] = std::min(low[axis], value);
            high[axis] = std::max(high[axis], value);
        }
    for (auto &probe : probes)
        for (unsigned channel = 0; channel < 3; ++channel)
            for (unsigned coefficient = 0; coefficient < 9; ++coefficient, offset += 4)
                probe[channel][coefficient] = Read<float>(data, offset) * lightUnitScale;
    for (auto &tetrahedron : tetrahedra)
        for (auto &index : tetrahedron)
        {
            index = Read<std::uint32_t>(data, offset);
            offset += 4;
            if (index >= probeCount)
                throw std::runtime_error("extracted light-grid tetrahedron names a missing probe");
        }
    for (auto &tetrahedron : neighbors)
        for (auto &neighbor : tetrahedron)
        {
            neighbor = Read<std::uint32_t>(data, offset);
            offset += 4;
            if (neighbor != UINT32_MAX && neighbor >= tetrahedronCount)
                throw std::runtime_error("extracted light-grid tetrahedron names a missing neighbour");
        }

    // The voxels: the smallest power of two that keeps Replay's limits, as BuildPayload.
    const auto alignDown = [](const double value, const std::int32_t alignment) {
        return static_cast<std::int32_t>(std::floor(value / alignment)) * alignment;
    };
    std::array<std::int32_t, 3> gridLow{};
    std::array<std::int32_t, 3> gridHigh{};
    std::array<std::uint32_t, 3> dimensions{};
    std::uint64_t voxelCount = 0;
    std::uint64_t rootCount = 0;
    unsigned rootShift = 5;
    for (; rootShift <= 20; ++rootShift)
    {
        const std::int32_t voxelSize = 1 << rootShift;
        voxelCount = 1;
        for (unsigned axis = 0; axis < 3; ++axis)
        {
            gridLow[axis] = alignDown(low[axis], voxelSize);
            gridHigh[axis] = alignDown(high[axis], voxelSize) + voxelSize;
            dimensions[axis] = static_cast<std::uint32_t>((gridHigh[axis] - gridLow[axis]) / voxelSize);
            voxelCount *= dimensions[axis];
        }
        rootCount = std::uint64_t(dimensions[0]) * dimensions[1];
        if (rootCount <= 1'000'000 && voxelCount <= 8'000'000)
            break;
    }
    if (rootShift > 20)
        throw std::runtime_error("extracted light grid exceeds Replay voxel-tree limits");

    const auto barycentric = [&](const std::uint32_t tetrahedron, const std::array<double, 3> &point,
                                 std::array<double, 4> &weights) {
        const auto &corner = positions[tetrahedra[tetrahedron][0]];
        std::array<std::array<double, 3>, 3> m{};
        std::array<double, 3> r{};
        for (unsigned axis = 0; axis < 3; ++axis)
        {
            for (unsigned vertex = 0; vertex < 3; ++vertex)
                m[axis][vertex] = positions[tetrahedra[tetrahedron][vertex + 1]][axis] - corner[axis];
            r[axis] = point[axis] - corner[axis];
        }
        const auto determinant = [](const std::array<std::array<double, 3>, 3> &a) {
            return a[0][0] * (a[1][1] * a[2][2] - a[1][2] * a[2][1]) -
                   a[0][1] * (a[1][0] * a[2][2] - a[1][2] * a[2][0]) +
                   a[0][2] * (a[1][0] * a[2][1] - a[1][1] * a[2][0]);
        };
        const double det = determinant(m);
        if (std::abs(det) < 1e-9)
            return false;
        double sum = 0.0;
        for (unsigned column = 0; column < 3; ++column)
        {
            auto n = m;
            for (unsigned axis = 0; axis < 3; ++axis)
                n[axis][column] = r[axis];
            weights[column + 1] = determinant(n) / det;
            sum += weights[column + 1];
        }
        weights[0] = 1.0 - sum;
        return true;
    };
    std::uint32_t hint = 0;
    const auto locate = [&](const std::array<double, 3> &point) -> std::uint32_t {
        auto tetrahedron = hint;
        for (unsigned step = 0; step < 16384; ++step)
        {
            std::array<double, 4> weights{};
            if (!barycentric(tetrahedron, point, weights))
                return UINT32_MAX;
            const auto lowest = static_cast<unsigned>(std::ranges::min_element(weights) - weights.begin());
            if (weights[lowest] >= -1e-6)
                return hint = tetrahedron;
            const auto next = neighbors[tetrahedron][lowest];
            if (next == UINT32_MAX)
                return UINT32_MAX;
            tetrahedron = next;
        }
        return UINT32_MAX;
    };

    const std::int32_t voxelSize = 1 << rootShift;
    std::vector<std::uint32_t> voxels(static_cast<std::size_t>(voxelCount), UINT32_MAX);
    std::size_t assigned = 0;
    for (std::uint32_t y = 0; y < dimensions[1]; ++y)
        for (std::uint32_t x = 0; x < dimensions[0]; ++x)
            for (std::uint32_t z = 0; z < dimensions[2]; ++z)
            {
                const std::array<double, 3> centre{gridLow[0] + (x + 0.5) * voxelSize,
                                                   gridLow[1] + (y + 0.5) * voxelSize,
                                                   gridLow[2] + (z + 0.5) * voxelSize};
                auto &voxel = voxels[(std::size_t(y) * dimensions[0] + x) * dimensions[2] + z];
                voxel = locate(centre);
                assigned += voxel != UINT32_MAX;
            }
    if (!assigned)
        throw std::runtime_error("extracted light grid holds no voxel centre");

    std::array<float, 32> fallback{};
    for (unsigned channel = 0; channel < 3; ++channel)
        for (unsigned coefficient = 0; coefficient < 9; ++coefficient)
        {
            std::vector<float> values;
            values.reserve(probes.size());
            for (const auto &probe : probes)
                values.push_back(probe[channel][coefficient]);
            fallback[channel * 9 + coefficient] = Median(std::move(values));
        }
    fallback[27] = 1.0f;
    fallback[28] = 1.0f;

    std::vector<std::uint8_t> output;
    output.reserve(264ull + probeCount * 44ull + tetrahedronCount * 32ull + rootCount * 12ull + voxelCount * 4ull);
    AppendBytes(output, Magic.data(), Magic.size());
    Append(output, probeCount);
    Append(output, tetrahedronCount);
    Append(output, static_cast<std::uint32_t>(rootCount));
    Append(output, static_cast<std::uint32_t>(voxelCount));
    constexpr float diagonal = 0.5773502691896258f;
    constexpr std::array<std::array<float, 3>, 7> axes{{
        {1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {diagonal, diagonal, diagonal},
        {diagonal, -diagonal, diagonal}, {-diagonal, diagonal, diagonal},
        {-diagonal, -diagonal, diagonal}}};
    for (const auto &axis : axes)
    {
        float minimum = 0.0f;
        float maximum = 0.0f;
        for (unsigned component = 0; component < 3; ++component)
        {
            minimum += axis[component] * (axis[component] >= 0 ? low[component] : high[component]);
            maximum += axis[component] * (axis[component] >= 0 ? high[component] : low[component]);
        }
        Append(output, axis[0]);
        Append(output, axis[1]);
        Append(output, minimum);
        Append(output, maximum);
    }
    for (const float value : fallback)
        Append(output, FloatToHalf(value));
    for (const auto dimension : dimensions)
        Append(output, static_cast<std::int32_t>(dimension));
    for (const std::int32_t value : {0, static_cast<std::int32_t>(rootShift),
                                     std::max(0, static_cast<std::int32_t>(rootShift) - 2),
                                     std::max(0, static_cast<std::int32_t>(rootShift) - 4), 0})
        Append(output, value);
    for (const auto value : gridLow)
        Append(output, static_cast<float>(value));
    Append(output, 0.0f);
    for (const auto value : gridHigh)
        Append(output, static_cast<float>(value));
    Append(output, 0.0f);
    std::size_t scaledProbes = 0;
    for (const auto &probe : probes)
    {
        // DC must not be negative for PackProbe; IW7's interpolation weights keep it so.
        Probe clamped = probe;
        for (auto &channel : clamped)
            channel[0] = std::max(channel[0], 0.0f);
        // Probes keep half floats: a day map's light units take IW7's brightest probes past their range (Scorch: raw
        // 7171 x 23.9). Such a probe scales as a whole into it, keeping its colour and direction.
        float peak = 0.0f;
        for (const auto &channel : clamped)
            for (const float value : channel)
                peak = std::max(peak, std::isfinite(value) ? std::abs(value) : 0.0f);
        if (peak > 60000.0f)
        {
            for (auto &channel : clamped)
                for (float &value : channel)
                    value *= 60000.0f / peak;
            ++scaledProbes;
        }
        const auto packed = PackProbe(clamped, 0);
        AppendBytes(output, packed.data(), packed.size());
    }
    if (scaledProbes)
        zt::warn("iw3: %zu light-grid probe(s) exceed the half-float range; scaled into it", scaledProbes);
    for (const auto &position : positions)
        for (const auto value : position)
            Append(output, static_cast<float>(value));
    for (const auto &tetrahedron : tetrahedra)
        AppendBytes(output, tetrahedron.data(), sizeof(tetrahedron));
    for (const auto &neighbor : neighbors)
        AppendBytes(output, neighbor.data(), sizeof(neighbor));
    for (std::uint32_t y = 0; y < dimensions[1]; ++y)
        for (std::uint32_t x = 0; x < dimensions[0]; ++x)
        {
            Append(output, 0x80000000u | ((y * dimensions[0] + x) * dimensions[2]));
            Append(output, 0u);
            Append(output, dimensions[2] - 1);
        }
    AppendBytes(output, voxels.data(), voxels.size() * sizeof(voxels.front()));
    if (output.size() > 512ull * 1024 * 1024)
        throw std::runtime_error("native Replay light-grid exceeds the supported size");
    zt::info("iw3: native light grid from the extraction's %u probes and %u tetrahedra (%ux%ux%u "
             "%u-unit voxels, %zu with a start)",
             probeCount, tetrahedronCount, dimensions[0], dimensions[1], dimensions[2], 1u << rootShift,
             assigned);
    return output;
}
} // namespace

bool PrepareNativeLightGrid(const std::filesystem::path &sourceRoot,
                            const std::string &sourceAsset,
                            const std::filesystem::path &output, const float lightUnitScale)
{
    std::error_code error;
    // An extraction that keeps its own probe mesh (IW7) gives the native grid as it is.
    const auto meshPath = sourceRoot / "maps" / "mp" / (sourceAsset + ".gpulightgrid.bin");
    if (std::filesystem::is_regular_file(meshPath, error))
    {
        std::ifstream input(meshPath, std::ios::binary);
        const std::vector<std::uint8_t> mesh{std::istreambuf_iterator<char>(input), {}};
        if (!zt::write_file(output.string(), BuildMeshPayload(mesh, lightUnitScale)))
            throw std::runtime_error("cannot write temporary native Replay light grid");
        return true;
    }
    const auto metadataPath =
        sourceRoot / "maps" / "mp" / (sourceAsset + ".lightgrid.json");
    if (!std::filesystem::is_regular_file(metadataPath, error))
    {
        zt::warn("iw3: source map has no exported light grid");
        return false;
    }
    const Json metadata = ReadJson(metadataPath);
    if (metadata.value("available", true) == false)
    {
        zt::warn("iw3: source map has no compiled light grid");
        return false;
    }
    if (metadata.at("schema").get<int>() != 1 ||
        metadata.at("name").get<std::string>() != "maps/mp/" + sourceAsset)
        throw std::runtime_error("mismatched IW3 light-grid export");

    std::vector<std::uint8_t> palette;
    const auto cells = ReadCells(sourceRoot, metadata, palette);
    const auto payload = BuildPayload(cells, palette, lightUnitScale);
    if (!zt::write_file(output.string(), payload))
        throw std::runtime_error("cannot write temporary native Replay light grid");
    return true;
}

std::vector<std::array<std::array<float, 3>, 6>> SampleLightGridAmbientCubes(
    const std::filesystem::path &sourceRoot, const std::string &sourceAsset,
    const std::vector<std::array<float, 3>> &points, const unsigned sunPrimaryLight,
    std::vector<float> *sunVisibility)
{
    std::vector<std::array<std::array<float, 3>, 6>> cubes;
    const auto metadataPath = sourceRoot / "maps" / "mp" / (sourceAsset + ".lightgrid.json");
    std::error_code error;
    if (points.empty() || !std::filesystem::is_regular_file(metadataPath, error))
        return cubes;
    const Json metadata = ReadJson(metadataPath);
    if (metadata.value("available", true) == false)
        return cubes;
    std::vector<std::uint8_t> palette;
    const auto cells = ReadCells(sourceRoot, metadata, palette);
    const auto probes = BuildProbes(palette);
    if (cells.empty() || probes.empty())
        return cubes;

    // Grid coordinates: 32 units in x/y, 64 in z, from -131072 (ReadCells).
    const auto key = [](const std::int64_t x, const std::int64_t y, const std::int64_t z) {
        return static_cast<std::uint64_t>(x & 0xffff) | static_cast<std::uint64_t>(y & 0xffff) << 16 |
               static_cast<std::uint64_t>(z & 0xffff) << 32;
    };
    std::unordered_map<std::uint64_t, Entry> grid;
    grid.reserve(cells.size());
    for (const Cell &cell : cells)
        grid.emplace(key((cell.position[0] + 131072) / 32, (cell.position[1] + 131072) / 32,
                         (cell.position[2] + 131072) / 64),
                     cell.entry);
    constexpr std::array<std::array<float, 3>, 6> directions{
        {{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1}}};
    std::array<std::array<float, 9>, 6> basis{};
    for (std::size_t face = 0; face < 6; ++face)
        basis[face] = EvaluateShBasis(directions[face]);

    cubes.resize(points.size());
    if (sunVisibility)
        sunVisibility->assign(points.size(), 1.0f);
    std::size_t nearestFallbacks = 0, unlit = 0, shaded = 0;
    for (std::size_t index = 0; index < points.size(); ++index)
    {
        const std::array<double, 3> coordinate{(points[index][0] + 131072.0) / 32.0,
                                               (points[index][1] + 131072.0) / 32.0,
                                               (points[index][2] + 131072.0) / 64.0};
        std::array<std::int64_t, 3> base{};
        std::array<double, 3> fraction{};
        for (unsigned axis = 0; axis < 3; ++axis)
        {
            base[axis] = static_cast<std::int64_t>(std::floor(coordinate[axis]));
            fraction[axis] = coordinate[axis] - static_cast<double>(base[axis]);
        }
        Probe sum{};
        double total = 0.0, sunlit = 0.0;
        for (unsigned corner = 0; corner < 8; ++corner)
        {
            const auto found = grid.find(key(base[0] + (corner & 1), base[1] + ((corner >> 1) & 1),
                                             base[2] + ((corner >> 2) & 1)));
            if (found == grid.end())
                continue;
            double weight = 1.0;
            for (unsigned axis = 0; axis < 3; ++axis)
                weight *= (corner >> axis) & 1 ? fraction[axis] : 1.0 - fraction[axis];
            if (weight <= 0.0)
                continue;
            for (unsigned channel = 0; channel < 3; ++channel)
                for (unsigned coefficient = 0; coefficient < 9; ++coefficient)
                    sum[channel][coefficient] += static_cast<float>(
                        weight * probes.at(found->second.color)[channel][coefficient]);
            total += weight;
            if (found->second.primaryLight == sunPrimaryLight)
                sunlit += weight;
        }
        if (total < 1.0e-3)
        {
            // No surrounding cells (solid volume or outside the grid): the
            // nearest cell in growing shells, as IW3 falls back to.
            std::optional<Entry> nearest;
            for (std::int64_t radius = 1; radius <= 8 && !nearest; ++radius)
                for (std::int64_t z = -radius; z <= radius && !nearest; ++z)
                    for (std::int64_t y = -radius; y <= radius && !nearest; ++y)
                        for (std::int64_t x = -radius; x <= radius && !nearest; ++x)
                        {
                            if (std::max({std::abs(x), std::abs(y), std::abs(z)}) != radius)
                                continue;
                            const auto found = grid.find(key(base[0] + x, base[1] + y, base[2] + z));
                            if (found != grid.end())
                                nearest = found->second;
                        }
            if (!nearest)
            {
                ++unlit;
                continue;
            }
            ++nearestFallbacks;
            sum = probes.at(nearest->color);
            total = 1.0;
            sunlit = nearest->primaryLight == sunPrimaryLight ? 1.0 : 0.0;
        }
        if (sunVisibility)
        {
            (*sunVisibility)[index] = static_cast<float>(sunlit / total);
            shaded += sunlit / total < 0.5;
        }
        for (std::size_t face = 0; face < 6; ++face)
            for (unsigned channel = 0; channel < 3; ++channel)
            {
                double value = 0.0;
                for (unsigned coefficient = 0; coefficient < 9; ++coefficient)
                    value += sum[channel][coefficient] / total * basis[face][coefficient];
                cubes[index][face][channel] = static_cast<float>(std::max(0.0, value));
            }
    }
    zt::info("iw3: lit %zu static model instance(s) from the IW3 light grid (%zu nearest-cell, %zu "
             "outside the grid, %zu out of the sun)",
             points.size() - unlit, nearestFallbacks, unlit, shaded);
    return cubes;
}

namespace
{
// Com_CanPrimaryLightAffectPoint (light->dir points toward the light).
bool CanPrimaryLightAffectPoint(const LightGridPrimaryLight &light, const std::array<float, 3> &point)
{
    const std::array<float, 3> delta{light.origin[0] - point[0], light.origin[1] - point[1],
                                     light.origin[2] - point[2]};
    const float distanceSquared = delta[0] * delta[0] + delta[1] * delta[1] + delta[2] * delta[2];
    if (distanceSquared >= light.radius * light.radius)
        return false;
    if (light.type == 3 || light.rotationLimit <= -light.cosHalfFovOuter)
        return true;
    const float spotDotTimesDistance = delta[0] * light.dir[0] + delta[1] * light.dir[1] + delta[2] * light.dir[2];
    float cosHalfFov = light.cosHalfFovOuter;
    if (light.rotationLimit != 1.0f)
    {
        // CosOfSumOfArcCos
        cosHalfFov = light.cosHalfFovOuter * light.rotationLimit -
                     std::sqrt(std::max(0.0f, (1 - light.cosHalfFovOuter * light.cosHalfFovOuter) *
                                                  (1 - light.rotationLimit * light.rotationLimit)));
        if (cosHalfFov <= 0.0f)
            return spotDotTimesDistance <= cosHalfFov * light.radius;
    }
    return spotDotTimesDistance > 0.0f &&
           cosHalfFov * cosHalfFov * distanceSquared <= spotDotTimesDistance * spotDotTimesDistance;
}

// R_SetLightGridColors: the 56 grid directions per texel of the 4x4x4 volume
// (x fastest, then y, then z); the inner core repeats corner samples.
constexpr std::array<std::uint8_t, 64> kModelLightingTexelColors{
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 0,  3,  21, 22, 12, 15, 23, 24, 25, 26, 27,
    28, 29, 30, 31, 32, 40, 43, 33, 34, 52, 55, 35, 36, 37, 38, 39,
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55};
} // namespace

std::vector<std::array<std::uint8_t, 256>> SampleStaticModelLighting(
    const std::filesystem::path &sourceRoot, const std::string &sourceAsset,
    const std::vector<StaticModelLightingInput> &inputs,
    const std::vector<LightGridPrimaryLight> &primaryLights, const LightGridSightBlocked &blocked)
{
    std::vector<std::array<std::uint8_t, 256>> volumes;
    // R_SetModelGroundLighting: the packed colour in every texel (B, G, R, A).
    const auto groundLighting = [](std::array<std::uint8_t, 256> &texels, const std::uint32_t color) {
        for (unsigned texel = 0; texel < 64; ++texel)
        {
            texels[texel * 4 + 0] = static_cast<std::uint8_t>(color >> 16);
            texels[texel * 4 + 1] = static_cast<std::uint8_t>(color >> 8);
            texels[texel * 4 + 2] = static_cast<std::uint8_t>(color);
            texels[texel * 4 + 3] = static_cast<std::uint8_t>(color >> 24);
        }
    };
    // A model with its own colours takes them in the texels of their directions.
    const auto ownColors = [](std::array<std::uint8_t, 256> &texels, const StaticModelLightingInput &input) {
        for (unsigned texel = 0; texel < 64; ++texel)
        {
            const unsigned color = kModelLightingTexelColors[texel];
            for (unsigned channel = 0; channel < 3; ++channel)
                texels[texel * 4 + channel] = (*input.colors)[color * 3 + channel];
            texels[texel * 4 + 3] = static_cast<std::uint8_t>(input.groundLighting >> 24);
        }
    };
    // Without a light grid, the models with ground lighting keep it and the others take the flat ambient.
    const auto withoutGrid = [&] {
        if (std::ranges::none_of(inputs, [](const StaticModelLightingInput &input) {
                return (input.groundLit && input.groundLighting) || input.colors;
            }))
            return volumes;
        volumes.resize(inputs.size());
        for (std::size_t index = 0; index < inputs.size(); ++index)
        {
            if (inputs[index].colors)
                ownColors(volumes[index], inputs[index]);
            else
                groundLighting(volumes[index], inputs[index].groundLit && inputs[index].groundLighting
                                                   ? inputs[index].groundLighting
                                                   : 0xFF282828u);
        }
        return volumes;
    };
    const auto metadataPath = sourceRoot / "maps" / "mp" / (sourceAsset + ".lightgrid.json");
    std::error_code error;
    if (inputs.empty() || !std::filesystem::is_regular_file(metadataPath, error))
        return withoutGrid();
    const Json metadata = ReadJson(metadataPath);
    if (metadata.value("available", true) == false)
        return withoutGrid();
    std::vector<std::uint8_t> palette;
    const auto cells = ReadCells(sourceRoot, metadata, palette);
    const std::size_t colorCount = palette.size() / 168;
    if (cells.empty() || !colorCount)
        return withoutGrid();
    const int rowAxis = metadata.at("row_axis").get<int>();
    const int columnAxis = metadata.at("col_axis").get<int>();
    const unsigned sun = metadata.at("sun_primary_light_index").get<unsigned>();
    const bool hasLightRegions = metadata.value("has_light_regions", false);

    // Grid points by integer coordinate (32 units in x/y, 64 in z, from -131072).
    const auto key = [](const std::int64_t x, const std::int64_t y, const std::int64_t z) {
        return static_cast<std::uint64_t>(x & 0xffff) | static_cast<std::uint64_t>(y & 0xffff) << 16 |
               static_cast<std::uint64_t>(z & 0xffff) << 32;
    };
    std::unordered_map<std::uint64_t, Entry> grid;
    std::unordered_map<std::uint64_t, std::int64_t> columnBottom;
    grid.reserve(cells.size());
    for (const Cell &cell : cells)
    {
        const std::int64_t x = (cell.position[0] + 131072) / 32, y = (cell.position[1] + 131072) / 32,
                           z = (cell.position[2] + 131072) / 64;
        grid.emplace(key(x, y, z), cell.entry);
        auto [bottom, inserted] = columnBottom.try_emplace(key(x, y, 0), z);
        if (!inserted)
            bottom->second = std::min(bottom->second, z);
    }
    const auto light = [&](const unsigned index) -> const LightGridPrimaryLight * {
        return index < primaryLights.size() ? &primaryLights[index] : nullptr;
    };

    volumes.resize(inputs.size());
    std::size_t ground = 0, extrapolated = 0, traced = 0, suppressed = 0, own = 0;
    for (std::size_t index = 0; index < inputs.size(); ++index)
    {
        const StaticModelLightingInput &input = inputs[index];
        auto &texels = volumes[index];
        if (input.colors)
        {
            ownColors(texels, input);
            ++own;
            continue;
        }
        if (input.groundLit && input.groundLighting)
        {
            groundLighting(texels, input.groundLighting);
            ++ground;
            continue;
        }

        // R_LightGridLookup.
        const auto &sample = input.origin;
        std::array<std::int64_t, 3> pos{(static_cast<std::int64_t>(std::floor(sample[0])) + 0x20000) >> 5,
                                        (static_cast<std::int64_t>(std::floor(sample[1])) + 0x20000) >> 5,
                                        (static_cast<std::int64_t>(std::floor(sample[2])) + 0x20000) >> 6};
        const float lerpRow = (sample[rowAxis] + 131072.0f) * 0.03125f - static_cast<float>(pos[rowAxis]);
        const float lerpColumn =
            (sample[columnAxis] + 131072.0f) * 0.03125f - static_cast<float>(pos[columnAxis]);
        const float lerpZ = (sample[2] + 131072.0f) * 0.015625f - static_cast<float>(pos[2]);
        std::array<float, 8> weight{};
        std::array<const Entry *, 8> entry{};
        unsigned defaultEntry = 1;
        for (unsigned corner = 0; corner < 8; ++corner)
        {
            weight[corner] = ((corner & 4) ? lerpRow : 1 - lerpRow) *
                             ((corner & 2) ? lerpColumn : 1 - lerpColumn) * ((corner & 1) ? lerpZ : 1 - lerpZ);
            std::array<std::int64_t, 3> p = pos;
            p[rowAxis] += (corner >> 2) & 1;
            p[columnAxis] += (corner >> 1) & 1;
            p[2] += corner & 1;
            const auto found = grid.find(key(p[0], p[1], p[2]));
            entry[corner] = found == grid.end() ? nullptr : &found->second;
        }
        // R_GetLightGridSampleEntryQuad: below the data of either base column,
        // extrapolation uses palette entry 0 instead of 1.
        for (unsigned row = 0; row < 2; ++row)
        {
            std::array<std::int64_t, 3> p = pos;
            p[rowAxis] += row;
            const auto bottom = columnBottom.find(key(p[0], p[1], 0));
            if (bottom != columnBottom.end() && pos[2] < bottom->second)
                defaultEntry = 0;
        }
        std::uint8_t lookupPrimary = 0;
        float bestWeight = 0;
        bool honorSuppression = false;
        for (unsigned corner = 0; corner < 8; ++corner)
        {
            const Entry *current = entry[corner];
            if (!current)
                continue;
            if (weight[corner] < 0.001f) // EQUAL_EPSILON
            {
                entry[corner] = nullptr;
                continue;
            }
            bool suppress = false;
            if (current->traceMask & (1u << corner))
            {
                // R_IsValidLightGridSample: sight from the sample to the corner,
                // nudged 0.01 toward the sample.
                std::array<float, 3> gridPos{static_cast<float>(pos[0]) * 32.0f - 131072.0f,
                                             static_cast<float>(pos[1]) * 32.0f - 131072.0f,
                                             static_cast<float>(pos[2]) * 64.0f - 131072.0f};
                gridPos[rowAxis] += (corner & 4) ? 32.0f : 0.0f;
                gridPos[columnAxis] += (corner & 2) ? 32.0f : 0.0f;
                gridPos[2] += (corner & 1) ? 64.0f : 0.0f;
                std::array<float, 3> direction{sample[0] - gridPos[0], sample[1] - gridPos[1],
                                               sample[2] - gridPos[2]};
                const float length = std::sqrt(direction[0] * direction[0] + direction[1] * direction[1] +
                                               direction[2] * direction[2]);
                if (length > 0)
                    for (float &component : direction)
                        component /= length;
                const std::array<float, 3> nudged{gridPos[0] + 0.01f * direction[0],
                                                  gridPos[1] + 0.01f * direction[1],
                                                  gridPos[2] + 0.01f * direction[2]};
                ++traced;
                suppress = blocked(sample, nudged);
                suppressed += suppress;
            }
            if (suppress)
            {
                if (honorSuppression)
                {
                    entry[corner] = nullptr;
                    continue;
                }
            }
            else if (!honorSuppression)
            {
                honorSuppression = true;
                bestWeight = weight[corner];
                lookupPrimary = current->primaryLight;
                for (unsigned previous = 0; previous < corner; ++previous)
                    entry[previous] = nullptr;
                continue;
            }
            const std::uint8_t candidate = current->primaryLight;
            const bool take = lookupPrimary ? candidate && (lookupPrimary == 255 ||
                                                            (candidate != 255 && weight[corner] > bestWeight))
                                            : true;
            if (take)
            {
                bestWeight = weight[corner];
                lookupPrimary = candidate;
            }
        }

        // R_GetLightingAtPoint.
        unsigned primary = lookupPrimary;
        if (primary == 255)
            primary = sun;
        else if (hasLightRegions && primary != sun)
            primary = input.primaryLight;
        std::array<std::uint16_t, 8> sampleColor{};
        std::array<float, 8> sampleWeight{};
        unsigned sampleCount = 0;
        float maxWeight = 0, visible = 0, occluded = 0;
        for (unsigned corner = 0; corner < 8; ++corner)
        {
            const Entry *current = entry[corner];
            if (!current)
                continue;
            if (current->primaryLight == primary)
                visible += weight[corner];
            else if (!current->primaryLight || (current->primaryLight == 255 && primary))
            {
                // R_CanLightInfluenceLightGridCorner.
                const LightGridPrimaryLight *primaryLight = light(primary);
                bool influences = !primaryLight || primaryLight->type == 1;
                if (!influences)
                {
                    std::array<float, 3> gridPos{std::floor(sample[0] * 0.03125f) * 32.0f,
                                                 std::floor(sample[1] * 0.03125f) * 32.0f,
                                                 std::floor(sample[2] * 0.015625f) * 64.0f};
                    gridPos[rowAxis] += (corner & 4) ? 32.0f : 0.0f;
                    gridPos[columnAxis] += (corner & 2) ? 32.0f : 0.0f;
                    gridPos[2] += (corner & 1) ? 64.0f : 0.0f;
                    influences = CanPrimaryLightAffectPoint(*primaryLight, gridPos);
                }
                if (influences)
                    occluded += weight[corner];
            }
            maxWeight += weight[corner];
            // R_AddLightGridSample
            unsigned slot = 0;
            while (slot < sampleCount && sampleColor[slot] != current->color)
                ++slot;
            if (slot == sampleCount)
            {
                sampleColor[sampleCount] = current->color;
                sampleWeight[sampleCount++] = 0;
            }
            sampleWeight[slot] += weight[corner];
        }

        std::array<std::uint8_t, 168> colors{};
        float primaryWeight = 1.0f;
        if (sampleCount)
        {
            if (primary)
                primaryWeight = occluded == 0 ? (visible != 0 ? 1.0f : 0.0f) : visible / (visible + occluded);
            else
                primaryWeight = 0;
            if (sampleCount == 1)
                std::memcpy(colors.data(), palette.data() + std::size_t(sampleColor[0]) * 168, 168);
            else
            {
                // R_GetLightGridColorsFixedPointBlendWeights + R_FixedPointBlendLightGridColors.
                const float scale = 1.0f / maxWeight;
                std::array<std::uint16_t, 8> fixedWeight{};
                std::uint16_t sum = 0;
                unsigned largest = 0;
                for (unsigned slot = 0; slot < sampleCount; ++slot)
                {
                    fixedWeight[slot] = static_cast<std::uint16_t>(static_cast<int>(scale * 256.0f * sampleWeight[slot] + 0.5f));
                    sum = static_cast<std::uint16_t>(sum + fixedWeight[slot]);
                    if (fixedWeight[largest] < fixedWeight[slot])
                        largest = slot;
                }
                fixedWeight[largest] = static_cast<std::uint16_t>(fixedWeight[largest] + 256 - sum);
                std::array<std::uint16_t, 168> accumulated{};
                for (unsigned slot = 0; slot < sampleCount; ++slot)
                    for (unsigned byte = 0; byte < 168; ++byte)
                        accumulated[byte] = static_cast<std::uint16_t>(
                            accumulated[byte] + fixedWeight[slot] * palette[std::size_t(sampleColor[slot]) * 168 + byte]);
                for (unsigned byte = 0; byte < 168; ++byte)
                    colors[byte] = static_cast<std::uint8_t>(static_cast<std::uint16_t>(accumulated[byte] + 127) >> 8);
            }
        }
        else
        {
            // R_ExtrapolateLightingAtPoint (GFX_MODELLIGHT_EXTRAPOLATE).
            const std::size_t fallback = colorCount <= defaultEntry ? colorCount - 1 : defaultEntry;
            std::memcpy(colors.data(), palette.data() + fallback * 168, 168);
            primary = sun;
            ++extrapolated;
        }
        // The model's primary light gets the weight; only a sun primary lights the
        // model with the sun (alpha = sun weight).
        const auto alpha = static_cast<std::uint8_t>(
            primary == sun ? static_cast<int>(primaryWeight * 255.0f + 0.5f) : 0);
        for (unsigned texel = 0; texel < 64; ++texel)
        {
            const unsigned color = kModelLightingTexelColors[texel];
            texels[texel * 4 + 0] = colors[color * 3 + 0];
            texels[texel * 4 + 1] = colors[color * 3 + 1];
            texels[texel * 4 + 2] = colors[color * 3 + 2];
            texels[texel * 4 + 3] = alpha;
        }
    }
    zt::info("iw3: lit %zu static model(s) as IW3 does (%zu with their own colours, %zu ground-lit, %zu "
             "extrapolated; %zu grid corner trace(s), %zu suppressed)",
             inputs.size(), own, ground, extrapolated, traced, suppressed);
    return volumes;
}

std::optional<EntityLightGrid> BuildEntityLightGrid(const std::filesystem::path &sourceRoot,
                                                    const std::string &sourceAsset)
{
    const auto metadataPath = sourceRoot / "maps" / "mp" / (sourceAsset + ".lightgrid.json");
    std::error_code error;
    if (!std::filesystem::is_regular_file(metadataPath, error))
        return std::nullopt;
    const Json metadata = ReadJson(metadataPath);
    if (metadata.value("available", true) == false)
        return std::nullopt;
    std::vector<std::uint8_t> palette;
    const auto cells = ReadCells(sourceRoot, metadata, palette);
    const std::size_t colorCount = palette.size() / 168;
    if (cells.empty() || !colorCount || colorCount >= 65535)
        return std::nullopt;
    EntityLightGrid grid;
    std::array<int, 3> maximum{};
    for (unsigned axis = 0; axis < 3; ++axis)
    {
        grid.minimum[axis] = metadata.at("mins").at(axis).get<int>();
        maximum[axis] = metadata.at("maxs").at(axis).get<int>();
        grid.dims[axis] = static_cast<unsigned>(maximum[axis] - grid.minimum[axis] + 1);
    }
    const std::size_t entryTexels = std::size_t(grid.dims[0]) * grid.dims[1] * grid.dims[2];
    grid.width = 4096;
    grid.paletteStart = static_cast<unsigned>((entryTexels + grid.width - 1) / grid.width * grid.width);
    const auto pointOf = [&](const Cell &cell) {
        const std::array<int, 3> point{(cell.position[0] + 131072) / 32 - grid.minimum[0],
                                       (cell.position[1] + 131072) / 32 - grid.minimum[1],
                                       (cell.position[2] + 131072) / 64 - grid.minimum[2]};
        if (point[0] < 0 || point[1] < 0 || point[2] < 0 || point[0] >= int(grid.dims[0]) ||
            point[1] >= int(grid.dims[1]) || point[2] >= int(grid.dims[2]))
            throw std::runtime_error("IW3 light-grid entry is outside its bounds");
        return (std::size_t(point[2]) * grid.dims[1] + point[1]) * grid.dims[0] + point[0];
    };

    // The extraction's own light grid at each entry: records of its SH.
    if (metadata.contains("sh"))
    {
        constexpr unsigned recordTexels = 14;
        const std::size_t total = grid.paletteStart + (cells.size() + 1) * recordTexels;
        if ((total + grid.width - 1) / grid.width <= 4096 && cells.size() * 27 * sizeof(float) <= 64ull * 1024 * 1024)
        {
            const auto bytes = ReadArray(sourceRoot, metadata, "sh", cells.size() * 27 * sizeof(float));
            std::vector<float> sh(cells.size() * 27);
            std::memcpy(sh.data(), bytes.data(), bytes.size());
            grid.sh = true;
            grid.paletteColors = cells.size() + 1;
            grid.height = static_cast<unsigned>((total + grid.width - 1) / grid.width);
            grid.pixels.assign(std::size_t(grid.width) * grid.height * 4, 0);
            const auto writeRecord = [&](const std::size_t record, const float *values) {
                std::array<std::uint16_t, recordTexels * 2> halves{};
                for (unsigned k = 0; k < 27; ++k)
                    halves[k] = FloatToHalf(values[k]);
                std::memcpy(grid.pixels.data() + (grid.paletteStart + record * recordTexels) * 4, halves.data(),
                            sizeof(halves));
            };
            std::array<double, 27> sum{};
            std::size_t valid = 0;
            for (std::size_t entry = 0; entry < cells.size(); ++entry)
            {
                const float *values = sh.data() + entry * 27;
                if (!std::all_of(values, values + 27, [](const float value) { return std::isfinite(value); }))
                    continue;
                for (unsigned k = 0; k < 27; ++k)
                    sum[k] += values[k];
                ++valid;
                const std::size_t texel = pointOf(cells[entry]);
                const auto record = static_cast<std::uint32_t>(entry + 1);
                grid.pixels[texel * 4 + 0] = static_cast<std::uint8_t>(record);
                grid.pixels[texel * 4 + 1] = static_cast<std::uint8_t>(record >> 8);
                grid.pixels[texel * 4 + 2] = static_cast<std::uint8_t>(record >> 16);
                grid.pixels[texel * 4 + 3] = 255;
                writeRecord(record, values);
            }
            if (valid)
            {
                std::array<float, 27> mean{};
                for (unsigned k = 0; k < 27; ++k)
                    mean[k] = static_cast<float>(sum[k] / static_cast<double>(valid));
                writeRecord(0, mean.data());
                zt::info("iw3: entity light grid %ux%ux%u points from the extraction's own light grid (%zu of %zu "
                         "entries, %ux%u image)",
                         grid.dims[0], grid.dims[1], grid.dims[2], valid, cells.size(), grid.width, grid.height);
                return grid;
            }
            grid.sh = false;
            zt::warn("iw3: the extraction's light grid holds no entry; entities take the light-grid colours");
        }
        else
            zt::warn("iw3: the extraction's light grid at %zu entries needs %zu image rows (Replay allows 4096); "
                     "entities take the light-grid colours",
                     cells.size(), (total + grid.width - 1) / grid.width);
    }
    grid.paletteColors = colorCount;
    const std::size_t total = grid.paletteStart + colorCount * 56;
    // One texel per point of the grid's box: a resident RGBA8 image holds at
    // most 4096 rows. Greenband's IW3 grid spans 1379x1300x247 points with
    // 0.12% populated (a 108106-row image); such maps keep the earlier entity
    // lighting (no MAP_ENTITY_GRID).
    if ((total + grid.width - 1) / grid.width > 4096)
    {
        zt::warn("iw3: entity light grid %ux%ux%u points needs %zu image rows (Replay allows 4096); entities "
                 "use the lighting without the IW3 grid",
                 grid.dims[0], grid.dims[1], grid.dims[2], (total + grid.width - 1) / grid.width);
        return std::nullopt;
    }
    grid.height = static_cast<unsigned>((total + grid.width - 1) / grid.width);
    grid.pixels.assign(std::size_t(grid.width) * grid.height * 4, 0);
    for (const Cell &cell : cells)
    {
        const std::size_t texel = pointOf(cell);
        const unsigned color = cell.entry.color + 1u;
        grid.pixels[texel * 4 + 0] = static_cast<std::uint8_t>(color);
        grid.pixels[texel * 4 + 1] = static_cast<std::uint8_t>(color >> 8);
        grid.pixels[texel * 4 + 2] = cell.entry.primaryLight;
        grid.pixels[texel * 4 + 3] = 255;
    }
    for (std::size_t color = 0; color < colorCount; ++color)
        for (unsigned sample = 0; sample < 56; ++sample)
        {
            const std::size_t texel = grid.paletteStart + color * 56 + sample;
            for (unsigned channel = 0; channel < 3; ++channel)
                grid.pixels[texel * 4 + channel] = palette[color * 168 + sample * 3 + channel];
            grid.pixels[texel * 4 + 3] = 255;
        }
    zt::info("iw3: entity light grid %ux%ux%u points, %zu palette colours (%ux%u image)", grid.dims[0],
             grid.dims[1], grid.dims[2], colorCount, grid.width, grid.height);
    return grid;
}
} // namespace iw3
