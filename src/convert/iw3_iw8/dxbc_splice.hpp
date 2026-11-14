// Copied from IW8 Radiant (src/core/dxbc_splice.h and .cpp) by tools/patches/zt_lit_filter.py: do not edit here.
#pragma once
// IW8 Radiant Suite - splicing a colour filter into a compiled pixel shader (plan M6: custom shaders on stock maps).
//
// A stock map's surfaces are lit by the game's own pixel shaders, which exist only as compiled DXBC (SM 5.0). A map
// shader's filter function (`float3 iw8rFilter(float3 colour)`, core custom_shader) is compiled on its own into a
// small pixel shader that reads the colour from input v0 and writes the result to o0; this module moves that shader's
// instructions into a stock shader, right before its end, so the stock shader's final colour passes through the
// filter. Nothing else of the stock shader changes: its inputs, resources and every instruction stay as shipped.
//
//   stock:   ... ; min o0.xyzw, r0.xyzw, l(32255...) ; ret
//   spliced: ... ; min rT.xyzw, r0.xyzw, l(32255...) ; mov rI, rT ; <filter: v0 -> rI, o0 -> o0, rk -> r(base + k)> ; ret
//
// Requirements, checked: both blobs are DXBC with a SHEX / SHDR chunk of a pixel shader; the stock shader ends in one
// `ret` and has no other `ret` / `retc` / discard after an output write; the filter shader declares only v0, o0 and
// temps (no resources, constant buffers, immediate constant buffers or indexable temps).
//
// A filter that reads the screen (DxbcSpliceOptions::sceneDepthRegister >= 0; a map shader's iw8rFilterScreen, the ink
// lines of a stock map): its program also has input 1, SV_Position, and the texture t0, read with Load only. Input 1
// becomes the stock shader's own position input (it must have one that carries every part the filter reads), t0
// becomes the stock shader's texture register `sceneDepthRegister`, where the technique binds the scene depth; when
// the stock shader does not declare that register the filter's declaration is added.
//
// The container's checksum is recomputed (dxbcChecksum: Direct3D refuses a blob whose checksum is wrong).

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace iw8r {

// The checksum Direct3D stores at bytes 4..19 of a DXBC container, computed over bytes 20..end (a modified MD5).
std::array<uint8_t, 16> dxbcChecksum(const uint8_t* blob, size_t size);
// True when the blob is a DXBC container whose stored checksum is right.
bool dxbcChecksumValid(const std::vector<uint8_t>& blob);

struct DxbcInfo {
    bool pixelShader = false;
    unsigned major = 0, minor = 0;
    unsigned temps = 0;          // dcl_temps
    unsigned instructions = 0;   // non-declaration instructions
    unsigned outputWrites = 0;   // instructions whose destination is o0
    unsigned rets = 0;           // ret instructions
    bool endsInRet = false;
};
bool dxbcInspect(const std::vector<uint8_t>& blob, DxbcInfo& info, std::string* err = nullptr);

struct DxbcSpliceOptions {
    // >= 0: the filter reads the screen (see the top); the stock shader's texture register for the scene depth.
    int sceneDepthRegister = -1;
};

// The stock shader with the filter shader's instructions between its last output write and its final ret (see the
// top). False with *err when a requirement is not met; `out` is then empty.
bool dxbcSpliceFilter(const std::vector<uint8_t>& stock, const std::vector<uint8_t>& filter, std::vector<uint8_t>& out,
                      std::string* err = nullptr, const DxbcSpliceOptions& options = {});

}  // namespace iw8r


#include <cmath>
#include <cstring>

namespace iw8r {

namespace {

// ---- MD5 block function (the DXBC checksum is the MD5 state after a non-standard last block)
struct Md5 {
    uint32_t s[4] = {0x67452301u, 0xefcdab89u, 0x98badcfeu, 0x10325476u};
    void block(const uint8_t* p) {
        static const uint32_t K[64] = {
            0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501, 0x698098d8, 0x8b44f7af, 0xffff5bb1,
            0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821, 0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453,
            0xd8a1e681, 0xe7d3fbc8, 0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a, 0xfffa3942,
            0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70, 0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
            0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665, 0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d,
            0x85845dd1, 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};
        static const int R[64] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20,
                                  4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};
        uint32_t m[16];
        for (int i = 0; i < 16; ++i) m[i] = uint32_t(p[i * 4]) | uint32_t(p[i * 4 + 1]) << 8 | uint32_t(p[i * 4 + 2]) << 16 | uint32_t(p[i * 4 + 3]) << 24;
        uint32_t a = s[0], b = s[1], c = s[2], d = s[3];
        for (int i = 0; i < 64; ++i) {
            uint32_t f;
            int g;
            if (i < 16) f = (b & c) | (~b & d), g = i;
            else if (i < 32) f = (d & b) | (~d & c), g = (5 * i + 1) & 15;
            else if (i < 48) f = b ^ c ^ d, g = (3 * i + 5) & 15;
            else f = c ^ (b | ~d), g = (7 * i) & 15;
            const uint32_t t = d;
            d = c;
            c = b;
            const uint32_t x = a + f + K[i] + m[g];
            b = b + ((x << R[i]) | (x >> (32 - R[i])));
            a = t;
        }
        s[0] += a, s[1] += b, s[2] += c, s[3] += d;
    }
};

uint32_t rd32(const uint8_t* p) { return uint32_t(p[0]) | uint32_t(p[1]) << 8 | uint32_t(p[2]) << 16 | uint32_t(p[3]) << 24; }
void wr32(uint8_t* p, uint32_t v) {
    p[0] = uint8_t(v), p[1] = uint8_t(v >> 8), p[2] = uint8_t(v >> 16), p[3] = uint8_t(v >> 24);
}

// ---- the token stream
constexpr uint32_t kOpCustomData = 53, kOpDiscard = 13, kOpRet = 62, kOpRetc = 63, kOpDclTemps = 104, kOpDclGlobalFlags = 106,
                   kOpDclInputPs = 98, kOpDclOutput = 101, kOpDclResource = 88, kOpDclInputPsSiv = 100;
constexpr uint32_t kTypeTemp = 0, kTypeInput = 1, kTypeOutput = 2, kTypeImm32 = 4, kTypeImm64 = 5, kTypeNull = 13, kTypeResource = 7;
constexpr uint32_t kNamePosition = 1;       // the system value name of SV_Position
constexpr uint32_t kDimensionTexture2D = 3; // dcl_resource's dimension field (bits 11..15)

// The components (bit 0 x .. bit 3 w) an operand token names: its mask, its swizzle or its one selected component.
uint32_t operandComponents(uint32_t tok) {
    if ((tok & 3u) != 2u) return 0xFu;  // not a four-component operand: taken as all
    const uint32_t mode = (tok >> 2) & 3u;
    if (mode == 0) return (tok >> 4) & 0xFu;
    if (mode == 1) return (1u << ((tok >> 4) & 3u)) | (1u << ((tok >> 6) & 3u)) | (1u << ((tok >> 8) & 3u)) | (1u << ((tok >> 10) & 3u));
    return 1u << ((tok >> 4) & 3u);
}

bool isDeclaration(uint32_t op) { return (op >= 88 && op <= 106) || (op >= 143 && op <= 162) || op == kOpCustomData; }

struct Operand {
    size_t at = 0;        // token index of the operand's first token
    size_t tokens = 0;
    uint32_t type = 0;
    int dims = 0;
    size_t index0 = 0;    // token index of the first index (an immediate), when dims >= 1 and it is immediate
    bool immediateIndex = false;
};

// Parses the operand at code[at]; false when it runs past `end` or uses a form this module does not handle.
bool parseOperand(const std::vector<uint32_t>& code, size_t at, size_t end, Operand& o) {
    if (at >= end) return false;
    const uint32_t tok = code[at];
    const uint32_t comps = tok & 3u;
    o.at = at;
    o.type = (tok >> 12) & 0xFFu;
    o.dims = int((tok >> 20) & 3u);
    size_t n = 1;
    bool ext = (tok >> 31) != 0;
    while (ext) {
        if (at + n >= end) return false;
        ext = (code[at + n] >> 31) != 0;
        ++n;
    }
    if (o.type == kTypeImm32) n += comps == 1 ? 1 : comps == 2 ? 4 : 0;
    else if (o.type == kTypeImm64) n += comps == 1 ? 2 : comps == 2 ? 8 : 0;
    o.immediateIndex = false;
    for (int k = 0; k < o.dims; ++k) {
        const uint32_t rep = (tok >> (22 + 3 * k)) & 7u;
        if (rep == 0) {
            if (k == 0) o.index0 = at + n, o.immediateIndex = true;
            n += 1;
        } else if (rep == 1) {
            n += 2;
        } else {
            // A relative index holds another operand (after its immediate part).
            if (rep == 3) n += 1;
            else if (rep == 4) n += 2;
            else if (rep != 2) return false;
            Operand sub;
            if (!parseOperand(code, at + n, end, sub)) return false;
            n += sub.tokens;
        }
    }
    o.tokens = n;
    return at + n <= end;
}

struct Instruction {
    size_t at = 0, tokens = 0;
    uint32_t op = 0;
    size_t operandsAt = 0;  // first operand token
};

bool nextInstruction(const std::vector<uint32_t>& code, size_t at, Instruction& in) {
    if (at >= code.size()) return false;
    const uint32_t tok = code[at];
    in.at = at;
    in.op = tok & 0x7FFu;
    if (in.op == kOpCustomData) {
        if (at + 1 >= code.size()) return false;
        in.tokens = code[at + 1];
        in.operandsAt = at + in.tokens;
    } else {
        in.tokens = (tok >> 24) & 0x7Fu;
        size_t n = 1;
        bool ext = (tok >> 31) != 0;
        while (ext) {
            if (at + n >= code.size()) return false;
            ext = (code[at + n] >> 31) != 0;
            ++n;
        }
        in.operandsAt = at + n;
    }
    return in.tokens >= 1 && at + in.tokens <= code.size();
}

struct Container {
    struct Chunk {
        uint32_t fourcc = 0;
        std::vector<uint8_t> data;
    };
    std::vector<Chunk> chunks;
    int shader = -1;  // the SHEX / SHDR chunk
    uint32_t version = 0;
    std::vector<uint32_t> code;  // the instruction tokens (after the version and length tokens)
};

constexpr uint32_t fourcc(char a, char b, char c, char d) {
    return uint32_t(uint8_t(a)) | uint32_t(uint8_t(b)) << 8 | uint32_t(uint8_t(c)) << 16 | uint32_t(uint8_t(d)) << 24;
}

bool readContainer(const std::vector<uint8_t>& blob, Container& c, std::string* err) {
    const auto fail = [&](const char* m) {
        if (err) *err = m;
        return false;
    };
    if (blob.size() < 32 || rd32(blob.data()) != fourcc('D', 'X', 'B', 'C')) return fail("not a DXBC container");
    const uint32_t total = rd32(&blob[24]), count = rd32(&blob[28]);
    if (total != blob.size() || count > 64 || 32 + size_t(count) * 4 > blob.size()) return fail("the DXBC header does not match the blob");
    for (uint32_t i = 0; i < count; ++i) {
        const uint32_t off = rd32(&blob[32 + i * 4]);
        if (size_t(off) + 8 > blob.size()) return fail("a DXBC chunk lies outside the blob");
        const uint32_t size = rd32(&blob[off + 4]);
        if (size_t(off) + 8 + size > blob.size()) return fail("a DXBC chunk lies outside the blob");
        Container::Chunk ch;
        ch.fourcc = rd32(&blob[off]);
        ch.data.assign(blob.begin() + off + 8, blob.begin() + off + 8 + size);
        if (ch.fourcc == fourcc('S', 'H', 'E', 'X') || ch.fourcc == fourcc('S', 'H', 'D', 'R')) c.shader = int(c.chunks.size());
        c.chunks.push_back(std::move(ch));
    }
    if (c.shader < 0) return fail("the blob has no shader chunk (SHEX / SHDR): not a DXBC shader model 4 / 5 program");
    const std::vector<uint8_t>& sh = c.chunks[size_t(c.shader)].data;
    if (sh.size() < 8 || sh.size() % 4) return fail("the shader chunk is malformed");
    c.version = rd32(sh.data());
    const uint32_t tokens = rd32(&sh[4]);
    if (tokens < 2 || size_t(tokens) * 4 > sh.size()) return fail("the shader chunk's length is wrong");
    c.code.resize(tokens - 2);
    for (size_t i = 0; i < c.code.size(); ++i) c.code[i] = rd32(&sh[8 + i * 4]);
    return true;
}

std::vector<uint8_t> writeContainer(Container& c) {
    std::vector<uint8_t>& sh = c.chunks[size_t(c.shader)].data;
    sh.assign(8 + c.code.size() * 4, 0);
    wr32(sh.data(), c.version);
    wr32(&sh[4], uint32_t(c.code.size() + 2));
    for (size_t i = 0; i < c.code.size(); ++i) wr32(&sh[8 + i * 4], c.code[i]);
    size_t total = 32 + c.chunks.size() * 4;
    for (const auto& ch : c.chunks) total += 8 + ch.data.size();
    std::vector<uint8_t> out(total, 0);
    wr32(out.data(), fourcc('D', 'X', 'B', 'C'));
    wr32(&out[20], 1);
    wr32(&out[24], uint32_t(total));
    wr32(&out[28], uint32_t(c.chunks.size()));
    size_t at = 32 + c.chunks.size() * 4;
    for (size_t i = 0; i < c.chunks.size(); ++i) {
        wr32(&out[32 + i * 4], uint32_t(at));
        wr32(&out[at], c.chunks[i].fourcc);
        wr32(&out[at + 4], uint32_t(c.chunks[i].data.size()));
        if (!c.chunks[i].data.empty()) std::memcpy(&out[at + 8], c.chunks[i].data.data(), c.chunks[i].data.size());
        at += 8 + c.chunks[i].data.size();
    }
    const auto sum = dxbcChecksum(out.data(), out.size());
    std::memcpy(&out[4], sum.data(), 16);
    return out;
}

bool pixelShader(uint32_t version) { return (version >> 16) == 0; }

}  // namespace

inline std::array<uint8_t, 16> dxbcChecksum(const uint8_t* blob, size_t size) {
    std::array<uint8_t, 16> out{};
    if (size < 20) return out;
    const uint8_t* data = blob + 20;
    const size_t len = size - 20;
    const uint32_t bits = uint32_t(len * 8);
    const size_t full = len & ~size_t(63);
    const size_t last = len - full;
    Md5 md;
    for (size_t i = 0; i < full; i += 64) md.block(data + i);
    uint8_t buf[64] = {};
    if (last >= 56) {
        // The rest, 0x80 and zeros to the block's end; then a block of the bit count, zeros and (bits >> 2) | 1.
        std::memcpy(buf, data + full, last);
        buf[last] = 0x80;
        md.block(buf);
        std::memset(buf, 0, sizeof buf);
        wr32(buf, bits);
        wr32(buf + 60, (bits >> 2) | 1u);
        md.block(buf);
    } else {
        // One block: the bit count, the rest, 0x80, zeros, (bits >> 2) | 1.
        wr32(buf, bits);
        if (last) std::memcpy(buf + 4, data + full, last);
        buf[4 + last] = 0x80;
        wr32(buf + 60, (bits >> 2) | 1u);
        md.block(buf);
    }
    for (int i = 0; i < 4; ++i) wr32(&out[size_t(i) * 4], md.s[i]);
    return out;
}

inline bool dxbcChecksumValid(const std::vector<uint8_t>& blob) {
    if (blob.size() < 32 || rd32(blob.data()) != fourcc('D', 'X', 'B', 'C')) return false;
    const auto sum = dxbcChecksum(blob.data(), blob.size());
    return std::memcmp(sum.data(), &blob[4], 16) == 0;
}

inline bool dxbcInspect(const std::vector<uint8_t>& blob, DxbcInfo& info, std::string* err) {
    info = DxbcInfo{};
    Container c;
    if (!readContainer(blob, c, err)) return false;
    info.pixelShader = pixelShader(c.version);
    info.major = (c.version >> 4) & 15u, info.minor = c.version & 15u;
    Instruction in;
    for (size_t at = 0; at < c.code.size(); at += in.tokens) {
        if (!nextInstruction(c.code, at, in)) {
            if (err) *err = "the shader's instruction stream is malformed";
            return false;
        }
        if (in.op == kOpDclTemps && in.tokens >= 2) info.temps = c.code[at + 1];
        if (isDeclaration(in.op)) continue;
        ++info.instructions;
        info.endsInRet = in.op == kOpRet;
        if (in.op == kOpRet || in.op == kOpRetc) ++info.rets;
        Operand o;
        for (size_t p = in.operandsAt; p < at + in.tokens; p += o.tokens) {
            if (!parseOperand(c.code, p, at + in.tokens, o)) {
                if (err) *err = "an instruction's operands could not be read";
                return false;
            }
            if (o.type == kTypeOutput && o.immediateIndex && c.code[o.index0] == 0) {
                ++info.outputWrites;
                break;
            }
        }
    }
    return true;
}

inline bool dxbcSpliceFilter(const std::vector<uint8_t>& stock, const std::vector<uint8_t>& filter, std::vector<uint8_t>& out, std::string* err,
                      const DxbcSpliceOptions& options) {
    out.clear();
    const bool screen = options.sceneDepthRegister >= 0;
    const auto fail = [&](const std::string& m) {
        if (err) *err = m;
        return false;
    };
    Container s, f;
    std::string e;
    if (!readContainer(stock, s, &e)) return fail("stock shader: " + e);
    if (!readContainer(filter, f, &e)) return fail("filter shader: " + e);
    if (!pixelShader(s.version) || !pixelShader(f.version)) return fail("both shaders must be pixel shaders");

    // ---- the stock shader: its temps, its writes to o0 (sent to a new temp), its one ret at the end
    size_t tempsAt = 0, lastRet = 0, declEnd = 0;
    uint32_t stockTemps = 0;
    unsigned rets = 0, writes = 0;
    bool haveTemps = false, endsInRet = false, sawCode = false;
    int positionInput = -1;          // the stock shader's SV_Position input register
    uint32_t positionMask = 0;       // the components it declares
    int sceneDepthDimension = -1;    // the dimension the stock shader declares for the scene depth register (-1: none)
    std::vector<std::pair<size_t, size_t>> outputOperands;  // operand token, index token
    Instruction in;
    for (size_t at = 0; at < s.code.size(); at += in.tokens) {
        if (!nextInstruction(s.code, at, in)) return fail("stock shader: the instruction stream is malformed");
        if (in.op == kOpDclTemps && in.tokens >= 2) tempsAt = at + 1, stockTemps = s.code[at + 1], haveTemps = true;
        if (isDeclaration(in.op)) {
            if (!sawCode) declEnd = at + in.tokens;
            if (screen && (in.op == kOpDclInputPsSiv || in.op == kOpDclResource) && in.tokens >= 3) {
                Operand d;
                if (parseOperand(s.code, in.operandsAt, at + in.tokens, d) && d.dims == 1 && d.immediateIndex) {
                    if (in.op == kOpDclInputPsSiv && s.code[at + in.tokens - 1] == kNamePosition)
                        positionInput = int(s.code[d.index0]), positionMask = operandComponents(s.code[d.at]);
                    if (in.op == kOpDclResource && d.type == kTypeResource && int(s.code[d.index0]) == options.sceneDepthRegister)
                        sceneDepthDimension = int((s.code[at] >> 11) & 31u);
                }
            }
            continue;
        }
        sawCode = true;
        endsInRet = in.op == kOpRet;
        if (in.op == kOpRet) ++rets, lastRet = at;
        if (in.op == kOpRetc) return fail("stock shader: it has a conditional return, which the splice does not handle");
        Operand o;
        for (size_t p = in.operandsAt; p < at + in.tokens; p += o.tokens) {
            if (!parseOperand(s.code, p, at + in.tokens, o)) return fail("stock shader: an instruction's operands could not be read");
            if (o.type == kTypeOutput && o.immediateIndex && o.dims == 1 && s.code[o.index0] == 0) {
                outputOperands.push_back({o.at, o.index0});
                ++writes;
            }
        }
    }
    if (rets != 1 || !endsInRet) return fail("stock shader: it must end in its only return (" + std::to_string(rets) + " found)");
    if (!writes) return fail("stock shader: it never writes output 0");
    if (screen && positionInput < 0) return fail("stock shader: it has no screen position input for a filter that reads the screen");
    if (screen && sceneDepthDimension >= 0 && uint32_t(sceneDepthDimension) != kDimensionTexture2D)
        return fail("stock shader: its register t" + std::to_string(options.sceneDepthRegister) + " is not a 2D texture");

    // ---- the filter shader: v0 in, o0 out, temps; everything else is refused
    uint32_t filterTemps = 0;
    std::vector<uint32_t> body;
    const uint32_t result = stockTemps;        // the stock colour
    const uint32_t base = stockTemps + 1;      // the filter's temp 0
    bool filterEnded = false;
    std::vector<uint32_t> textureDeclaration;  // the filter's dcl of t0, with the stock register (screen mode)
    for (size_t at = 0; at < f.code.size(); at += in.tokens) {
        if (!nextInstruction(f.code, at, in)) return fail("filter shader: the instruction stream is malformed");
        if (isDeclaration(in.op)) {
            if (in.op == kOpDclTemps && in.tokens >= 2) {
                filterTemps = f.code[at + 1];
            } else if (in.op == kOpDclInputPs || in.op == kOpDclOutput) {
                Operand o;
                if (!parseOperand(f.code, in.operandsAt, at + in.tokens, o) || !o.immediateIndex || f.code[o.index0] != 0)
                    return fail("filter shader: it may only read input 0 and write output 0");
            } else if (screen && in.op == kOpDclInputPsSiv) {
                Operand o;
                if (!parseOperand(f.code, in.operandsAt, at + in.tokens, o) || !o.immediateIndex || f.code[o.index0] != 1 ||
                    f.code[at + in.tokens - 1] != kNamePosition)
                    return fail("filter shader: its second input must be the screen position");
            } else if (screen && in.op == kOpDclResource) {
                Operand o;
                if (!parseOperand(f.code, in.operandsAt, at + in.tokens, o) || o.type != kTypeResource || o.dims != 1 || !o.immediateIndex ||
                    f.code[o.index0] != 0 || ((f.code[at] >> 11) & 31u) != kDimensionTexture2D)
                    return fail("filter shader: its only texture must be the 2D texture t0 (the scene depth)");
                textureDeclaration.assign(f.code.begin() + long(at), f.code.begin() + long(at + in.tokens));
                textureDeclaration[o.index0 - at] = uint32_t(options.sceneDepthRegister);
            } else if (in.op != kOpDclGlobalFlags) {
                return fail("filter shader: it declares something other than its input, output and temps (no textures, buffers, "
                            "arrays or constant tables in a filter)");
            }
            continue;
        }
        if (filterEnded) return fail("filter shader: instructions follow its return");
        if (in.op == kOpRetc || in.op == kOpDiscard) return fail("filter shader: no conditional return or discard in a filter");
        if (in.op == kOpRet) {
            filterEnded = true;
            continue;
        }
        std::vector<uint32_t> copy(f.code.begin() + long(at), f.code.begin() + long(at + in.tokens));
        Operand o;
        for (size_t p = in.operandsAt; p < at + in.tokens; p += o.tokens) {
            if (!parseOperand(f.code, p, at + in.tokens, o)) return fail("filter shader: an instruction's operands could not be read");
            if (o.type == kTypeImm32 || o.type == kTypeImm64 || o.type == kTypeNull) continue;
            if (o.dims != 1 || !o.immediateIndex) return fail("filter shader: an operand form the splice does not handle");
            const size_t rel = o.at - at, idx = o.index0 - at;
            if (o.type == kTypeTemp) {
                copy[idx] += base;
            } else if (o.type == kTypeInput && screen && copy[idx] == 1) {
                // The screen position: the stock shader's own, which must carry what the filter reads.
                if (operandComponents(copy[rel]) & ~positionMask)
                    return fail("stock shader: its screen position input lacks a part the filter reads");
                copy[idx] = uint32_t(positionInput);
            } else if (o.type == kTypeInput) {
                if (copy[idx] != 0) return fail("filter shader: it reads an input other than 0");
                copy[rel] = (copy[rel] & ~(0xFFu << 12)) | (kTypeTemp << 12);
                copy[idx] = result;
            } else if (o.type == kTypeResource && screen) {
                if (copy[idx] != 0) return fail("filter shader: it reads a texture other than t0");
                copy[idx] = uint32_t(options.sceneDepthRegister);
            } else if (o.type == kTypeOutput) {
                if (copy[idx] != 0) return fail("filter shader: it writes an output other than 0");
            } else {
                return fail("filter shader: it uses a register kind a filter may not (type " + std::to_string(o.type) + ")");
            }
        }
        body.insert(body.end(), copy.begin(), copy.end());
    }
    if (!filterEnded) return fail("filter shader: it does not end in a return");

    // ---- put it together
    for (const auto& [operand, index] : outputOperands) {
        s.code[operand] = (s.code[operand] & ~(0xFFu << 12)) | (kTypeTemp << 12);
        s.code[index] = result;
    }
    const uint32_t temps = stockTemps + 1 + filterTemps;
    if (temps > 4096) return fail("too many temporary registers");
    if (haveTemps) s.code[tempsAt] = temps;
    s.code.insert(s.code.begin() + long(lastRet), body.begin(), body.end());
    if (!haveTemps) {
        const uint32_t dcl[2] = {kOpDclTemps | (2u << 24), temps};
        s.code.insert(s.code.begin() + long(declEnd), dcl, dcl + 2);
    }
    // The scene depth's declaration, when the stock shader has none for that register (after the insert above, which
    // is at or behind it, so declEnd still holds).
    if (screen && sceneDepthDimension < 0 && !textureDeclaration.empty())
        s.code.insert(s.code.begin() + long(declEnd), textureDeclaration.begin(), textureDeclaration.end());
    out = writeContainer(s);
    return true;
}

}  // namespace iw8r
