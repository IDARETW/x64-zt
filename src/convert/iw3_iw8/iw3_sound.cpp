#include <std_include.hpp>
#include "iw3_sound.h"
#include "iw3_ledger.h"

#include <iw8/common/fs_util.h>
#include <iw8/common/log.h>

#include <Windows.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <shlwapi.h>
#include <wrl/client.h>

#include <algorithm>
#include <array>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <memory>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <unordered_map>

namespace iw3::sound
{
namespace
{
using Json = nlohmann::json;

std::string Lower(std::string text)
{
    std::transform(text.begin(), text.end(), text.begin(),
                   [](const unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return text;
}

std::string Trim(const std::string &text)
{
    const auto first = text.find_first_not_of(" \t\r\n");
    if (first == std::string::npos)
        return {};
    return text.substr(first, text.find_last_not_of(" \t\r\n") - first + 1);
}

// IW3 sound paths are relative to sound/; zone dumps prefix referenced
// loaded sounds with ','.
std::string NormalizeSoundPath(std::string path)
{
    path = Lower(Trim(path));
    while (!path.empty() && path.front() == ',')
        path.erase(path.begin());
    std::replace(path.begin(), path.end(), '\\', '/');
    if (path.starts_with("sound/"))
        path.erase(0, 6);
    return path;
}

// ------------------------------------------------------------- inflate
// RFC 1951 decoder for IWD (zip) members, after Mark Adler's puff.
class Inflater
{
  public:
    Inflater(const uint8_t *data, const size_t size, std::vector<uint8_t> &out)
        : data_(data)
        , size_(size)
        , out_(out)
    {
    }

    void Run()
    {
        int last = 0;
        do
        {
            last = Bits(1);
            const int type = Bits(2);
            if (type == 0)
                Stored();
            else if (type == 1)
                Fixed();
            else if (type == 2)
                Dynamic();
            else
                throw std::runtime_error("invalid deflate block type");
        } while (!last);
    }

  private:
    struct Huffman
    {
        std::array<int16_t, 16> count{};
        std::array<int16_t, 320> symbol{};
    };

    int Bits(const int need)
    {
        uint32_t value = bitBuffer_;
        while (bitCount_ < need)
        {
            if (position_ >= size_)
                throw std::runtime_error("truncated deflate stream");
            value |= static_cast<uint32_t>(data_[position_++]) << bitCount_;
            bitCount_ += 8;
        }
        bitBuffer_ = value >> need;
        bitCount_ -= need;
        return static_cast<int>(value & ((1u << need) - 1));
    }

    void Stored()
    {
        bitBuffer_ = 0;
        bitCount_ = 0;
        if (position_ + 4 > size_)
            throw std::runtime_error("truncated stored deflate block");
        const unsigned length = data_[position_] | (data_[position_ + 1] << 8);
        const unsigned check = data_[position_ + 2] | (data_[position_ + 3] << 8);
        position_ += 4;
        if (length != (~check & 0xFFFF) || position_ + length > size_)
            throw std::runtime_error("invalid stored deflate block");
        out_.insert(out_.end(), data_ + position_, data_ + position_ + length);
        position_ += length;
    }

    int Decode(const Huffman &h)
    {
        int code = 0, first = 0, index = 0;
        for (int length = 1; length <= 15; ++length)
        {
            code |= Bits(1);
            const int count = h.count[length];
            if (code - count < first)
                return h.symbol[index + (code - first)];
            index += count;
            first += count;
            first <<= 1;
            code <<= 1;
        }
        throw std::runtime_error("invalid deflate Huffman code");
    }

    static void Construct(Huffman &h, const int16_t *length, const int n)
    {
        h.count.fill(0);
        for (int symbol = 0; symbol < n; ++symbol)
            ++h.count[length[symbol]];
        std::array<int16_t, 16> offsets{};
        for (int len = 1; len < 15; ++len)
            offsets[len + 1] = static_cast<int16_t>(offsets[len] + h.count[len]);
        for (int symbol = 0; symbol < n; ++symbol)
            if (length[symbol] != 0)
                h.symbol[offsets[length[symbol]]++] = static_cast<int16_t>(symbol);
    }

    void Codes(const Huffman &lengthCode, const Huffman &distanceCode)
    {
        static constexpr int16_t lengthBase[29] = {3,  4,  5,  6,  7,  8,  9,  10, 11,  13,
                                                   15, 17, 19, 23, 27, 31, 35, 43, 51,  59,
                                                   67, 83, 99, 115, 131, 163, 195, 227, 258};
        static constexpr int16_t lengthExtra[29] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2,
                                                    2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0};
        static constexpr int16_t distanceBase[30] = {
            1,   2,   3,   4,   5,   7,    9,    13,   17,   25,   33,   49,   65,    97,    129,
            193, 257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577};
        static constexpr int16_t distanceExtra[30] = {0, 0, 0, 0, 1, 1, 2, 2,  3,  3,  4,  4,  5,  5,  6,
                                                      6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13};
        for (;;)
        {
            int symbol = Decode(lengthCode);
            if (symbol < 256)
            {
                out_.push_back(static_cast<uint8_t>(symbol));
                continue;
            }
            if (symbol == 256)
                return;
            symbol -= 257;
            if (symbol >= 29)
                throw std::runtime_error("invalid deflate length symbol");
            const size_t length = lengthBase[symbol] + Bits(lengthExtra[symbol]);
            const int distanceSymbol = Decode(distanceCode);
            if (distanceSymbol >= 30)
                throw std::runtime_error("invalid deflate distance symbol");
            const size_t distance = distanceBase[distanceSymbol] + Bits(distanceExtra[distanceSymbol]);
            if (distance > out_.size())
                throw std::runtime_error("deflate distance exceeds output");
            const size_t from = out_.size() - distance;
            for (size_t i = 0; i < length; ++i)
                out_.push_back(out_[from + i]);
        }
    }

    void Fixed()
    {
        static const auto tables = [] {
            std::pair<Huffman, Huffman> result;
            int16_t lengths[288];
            for (int symbol = 0; symbol < 144; ++symbol)
                lengths[symbol] = 8;
            for (int symbol = 144; symbol < 256; ++symbol)
                lengths[symbol] = 9;
            for (int symbol = 256; symbol < 280; ++symbol)
                lengths[symbol] = 7;
            for (int symbol = 280; symbol < 288; ++symbol)
                lengths[symbol] = 8;
            Construct(result.first, lengths, 288);
            for (int symbol = 0; symbol < 30; ++symbol)
                lengths[symbol] = 5;
            Construct(result.second, lengths, 30);
            return result;
        }();
        Codes(tables.first, tables.second);
    }

    void Dynamic()
    {
        static constexpr int16_t order[19] = {16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};
        const int lengthCount = Bits(5) + 257;
        const int distanceCount = Bits(5) + 1;
        const int codeCount = Bits(4) + 4;
        if (lengthCount > 286 || distanceCount > 30)
            throw std::runtime_error("invalid dynamic deflate counts");
        int16_t lengths[320]{};
        for (int i = 0; i < codeCount; ++i)
            lengths[order[i]] = static_cast<int16_t>(Bits(3));
        Huffman codeLengths;
        Construct(codeLengths, lengths, 19);
        int index = 0;
        while (index < lengthCount + distanceCount)
        {
            int symbol = Decode(codeLengths);
            if (symbol < 16)
            {
                lengths[index++] = static_cast<int16_t>(symbol);
                continue;
            }
            int16_t repeat = 0;
            int times = 0;
            if (symbol == 16)
            {
                if (index == 0)
                    throw std::runtime_error("deflate repeat without a previous length");
                repeat = lengths[index - 1];
                times = 3 + Bits(2);
            }
            else if (symbol == 17)
                times = 3 + Bits(3);
            else
                times = 11 + Bits(7);
            if (index + times > lengthCount + distanceCount)
                throw std::runtime_error("deflate code lengths overflow");
            while (times--)
                lengths[index++] = repeat;
        }
        Huffman lengthCode, distanceCode;
        Construct(lengthCode, lengths, lengthCount);
        Construct(distanceCode, lengths + lengthCount, distanceCount);
        Codes(lengthCode, distanceCode);
    }

    const uint8_t *data_;
    size_t size_;
    size_t position_{};
    uint32_t bitBuffer_{};
    int bitCount_{};
    std::vector<uint8_t> &out_;
};

uint32_t Crc32(const std::vector<uint8_t> &data)
{
    static const auto table = [] {
        std::array<uint32_t, 256> result{};
        for (uint32_t i = 0; i < 256; ++i)
        {
            uint32_t c = i;
            for (int bit = 0; bit < 8; ++bit)
                c = (c & 1) ? 0xEDB88320u ^ (c >> 1) : c >> 1;
            result[i] = c;
        }
        return result;
    }();
    uint32_t crc = 0xFFFFFFFFu;
    for (const uint8_t byte : data)
        crc = table[(crc ^ byte) & 0xFF] ^ (crc >> 8);
    return crc ^ 0xFFFFFFFFu;
}

// ------------------------------------------------------------------ IWD
class Iwd
{
  public:
    explicit Iwd(const std::filesystem::path &path)
        : path_(path)
    {
        std::ifstream file(path, std::ios::binary);
        if (!file)
            throw std::runtime_error("cannot open " + path.string());
        file.seekg(0, std::ios::end);
        const uint64_t size = static_cast<uint64_t>(file.tellg());
        const uint64_t tail = std::min<uint64_t>(size, 65536 + 22);
        std::vector<uint8_t> end(static_cast<size_t>(tail));
        file.seekg(static_cast<std::streamoff>(size - tail));
        file.read(reinterpret_cast<char *>(end.data()), static_cast<std::streamsize>(end.size()));
        size_t eocd = std::string::npos;
        for (size_t i = end.size() >= 22 ? end.size() - 22 : 0; i != size_t(-1); --i)
            if (Read32(end, i) == 0x06054B50u)
            {
                eocd = i;
                break;
            }
        if (eocd == std::string::npos)
            throw std::runtime_error("no zip directory in " + path.string());
        const uint32_t directorySize = Read32(end, eocd + 12);
        const uint32_t directoryOffset = Read32(end, eocd + 16);
        std::vector<uint8_t> directory(directorySize);
        file.seekg(directoryOffset);
        file.read(reinterpret_cast<char *>(directory.data()), directorySize);
        if (!file)
            throw std::runtime_error("cannot read zip directory of " + path.string());
        for (size_t at = 0; at + 46 <= directory.size();)
        {
            if (Read32(directory, at) != 0x02014B50u)
                break;
            Member member;
            member.method = Read16(directory, at + 10);
            member.crc = Read32(directory, at + 16);
            member.compressed = Read32(directory, at + 20);
            member.size = Read32(directory, at + 24);
            const uint16_t nameLength = Read16(directory, at + 28);
            const uint16_t extraLength = Read16(directory, at + 30);
            const uint16_t commentLength = Read16(directory, at + 32);
            member.header = Read32(directory, at + 42);
            std::string name(reinterpret_cast<const char *>(directory.data() + at + 46), nameLength);
            std::replace(name.begin(), name.end(), '\\', '/');
            members_[Lower(name)] = member;
            at += 46 + nameLength + extraLength + commentLength;
        }
    }

    bool Contains(const std::string &name) const
    {
        return members_.contains(name);
    }

    std::vector<std::string> Names() const
    {
        std::vector<std::string> names;
        for (const auto &[name, member] : members_)
            names.push_back(name);
        return names;
    }

    std::vector<uint8_t> Read(const std::string &name) const
    {
        const Member &member = members_.at(name);
        std::ifstream file(path_, std::ios::binary);
        std::vector<uint8_t> local(30);
        file.seekg(member.header);
        file.read(reinterpret_cast<char *>(local.data()), 30);
        if (!file || Read32(local, 0) != 0x04034B50u)
            throw std::runtime_error("bad zip member header for " + name);
        file.seekg(member.header + 30 + Read16(local, 26) + Read16(local, 28));
        std::vector<uint8_t> packed(member.compressed);
        file.read(reinterpret_cast<char *>(packed.data()), static_cast<std::streamsize>(packed.size()));
        if (!file)
            throw std::runtime_error("cannot read zip member " + name);
        std::vector<uint8_t> data;
        if (member.method == 0)
            data = std::move(packed);
        else if (member.method == 8)
        {
            data.reserve(member.size);
            Inflater(packed.data(), packed.size(), data).Run();
        }
        else
            throw std::runtime_error("unsupported zip method for " + name);
        if (data.size() != member.size || Crc32(data) != member.crc)
            throw std::runtime_error("zip member failed its CRC check: " + name);
        return data;
    }

  private:
    struct Member
    {
        uint16_t method{};
        uint32_t crc{};
        uint32_t compressed{};
        uint32_t size{};
        uint32_t header{};
    };
    static uint16_t Read16(const std::vector<uint8_t> &b, const size_t at)
    {
        return static_cast<uint16_t>(b.at(at) | (b.at(at + 1) << 8));
    }
    static uint32_t Read32(const std::vector<uint8_t> &b, const size_t at)
    {
        return Read16(b, at) | (static_cast<uint32_t>(Read16(b, at + 2)) << 16);
    }
    std::filesystem::path path_;
    std::unordered_map<std::string, Member> members_;
};

// ------------------------------------------------------------------ WAV
struct Pcm
{
    uint32_t rate{};
    uint8_t channels{};
    std::vector<int16_t> samples;
};

// IW3 also streams MP3 (ambient loops such as ambient/amb_middle_east1v7_lr.mp3).
// Windows Media Foundation decodes it to 16-bit PCM in memory.
std::optional<Pcm> DecodeCompressedAudio(const std::vector<uint8_t> &file, std::string &error)
{
    using Microsoft::WRL::ComPtr;
    static const HRESULT started = MFStartup(MF_VERSION, MFSTARTUP_LITE);
    if (FAILED(started))
    {
        error = "Media Foundation is unavailable";
        return std::nullopt;
    }
    const HRESULT com = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    const bool comOwned = SUCCEEDED(com);
    struct ComScope
    {
        bool owned;
        ~ComScope()
        {
            if (owned)
                CoUninitialize();
        }
    } scope{comOwned};
    if (file.empty() || file.size() > 0x7FFFFFFF)
    {
        error = "compressed audio has an invalid size";
        return std::nullopt;
    }
    ComPtr<IStream> stream;
    stream.Attach(SHCreateMemStream(file.data(), static_cast<UINT>(file.size())));
    ComPtr<IMFByteStream> byteStream;
    ComPtr<IMFSourceReader> reader;
    ComPtr<IMFMediaType> requested;
    if (!stream || FAILED(MFCreateMFByteStreamOnStream(stream.Get(), &byteStream)) ||
        FAILED(MFCreateSourceReaderFromByteStream(byteStream.Get(), nullptr, &reader)) ||
        FAILED(MFCreateMediaType(&requested)) ||
        FAILED(requested->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio)) ||
        FAILED(requested->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM)) ||
        FAILED(requested->SetUINT32(MF_MT_AUDIO_BITS_PER_SAMPLE, 16)) ||
        FAILED(reader->SetStreamSelection(static_cast<DWORD>(MF_SOURCE_READER_ALL_STREAMS), FALSE)) ||
        FAILED(reader->SetStreamSelection(static_cast<DWORD>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), TRUE)) ||
        FAILED(reader->SetCurrentMediaType(static_cast<DWORD>(MF_SOURCE_READER_FIRST_AUDIO_STREAM),
                                           nullptr, requested.Get())))
    {
        error = "Media Foundation cannot decode the audio";
        return std::nullopt;
    }
    ComPtr<IMFMediaType> actual;
    if (FAILED(reader->GetCurrentMediaType(static_cast<DWORD>(MF_SOURCE_READER_FIRST_AUDIO_STREAM),
                                           &actual)))
    {
        error = "Media Foundation has no PCM output";
        return std::nullopt;
    }
    Pcm result;
    result.rate = MFGetAttributeUINT32(actual.Get(), MF_MT_AUDIO_SAMPLES_PER_SECOND, 0);
    const UINT32 channels = MFGetAttributeUINT32(actual.Get(), MF_MT_AUDIO_NUM_CHANNELS, 0);
    if (!result.rate || channels < 1 || channels > 2 ||
        MFGetAttributeUINT32(actual.Get(), MF_MT_AUDIO_BITS_PER_SAMPLE, 0) != 16)
    {
        error = "decoded audio has an unsupported format";
        return std::nullopt;
    }
    result.channels = static_cast<uint8_t>(channels);
    for (;;)
    {
        DWORD flags = 0;
        ComPtr<IMFSample> sample;
        if (FAILED(reader->ReadSample(static_cast<DWORD>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0,
                                      nullptr, &flags, nullptr, &sample)))
        {
            error = "Media Foundation failed while decoding";
            return std::nullopt;
        }
        if (sample)
        {
            ComPtr<IMFMediaBuffer> buffer;
            BYTE *data = nullptr;
            DWORD length = 0;
            if (FAILED(sample->ConvertToContiguousBuffer(&buffer)) ||
                FAILED(buffer->Lock(&data, nullptr, &length)))
            {
                error = "Media Foundation returned an unreadable sample";
                return std::nullopt;
            }
            const auto *pcm = reinterpret_cast<const int16_t *>(data);
            result.samples.insert(result.samples.end(), pcm, pcm + length / 2);
            buffer->Unlock();
        }
        if (flags & MF_SOURCE_READERF_ENDOFSTREAM)
            break;
    }
    if (result.samples.empty())
    {
        error = "decoded audio is empty";
        return std::nullopt;
    }
    return result;
}

std::optional<Pcm> ReadWav(const std::vector<uint8_t> &file, std::string &error)
{
    const auto read16 = [&](const size_t at) { return static_cast<uint16_t>(file[at] | (file[at + 1] << 8)); };
    const auto read32 = [&](const size_t at) { return read16(at) | (static_cast<uint32_t>(read16(at + 2)) << 16); };
    if (file.size() < 12 || std::memcmp(file.data(), "RIFF", 4) || std::memcmp(file.data() + 8, "WAVE", 4))
    {
        error = "not a RIFF WAVE file";
        return std::nullopt;
    }
    uint16_t format = 0, channels = 0, bits = 0;
    uint32_t rate = 0;
    const uint8_t *data = nullptr;
    size_t dataSize = 0;
    for (size_t at = 12; at + 8 <= file.size();)
    {
        const uint32_t size = read32(at + 4);
        const size_t body = at + 8;
        const size_t available = std::min<size_t>(size, file.size() - body);
        if (!std::memcmp(file.data() + at, "fmt ", 4) && available >= 16)
        {
            format = read16(body);
            channels = read16(body + 2);
            rate = read32(body + 4);
            bits = read16(body + 14);
            if (format == 0xFFFE && available >= 26)
                format = read16(body + 24);
        }
        else if (!std::memcmp(file.data() + at, "data", 4))
        {
            data = file.data() + body;
            dataSize = available;
        }
        at = body + size + (size & 1);
    }
    if (!data || !channels || channels > 2 || !rate)
    {
        error = "missing fmt/data chunk or unsupported channel count";
        return std::nullopt;
    }
    Pcm pcm;
    pcm.rate = rate;
    pcm.channels = static_cast<uint8_t>(channels);
    const size_t width = bits / 8;
    if (!width || (format == 1 && width > 3) || (format == 3 && bits != 32) || (format != 1 && format != 3))
    {
        error = "unsupported WAV encoding (format " + std::to_string(format) + ", " +
                std::to_string(bits) + " bits)";
        return std::nullopt;
    }
    const size_t count = dataSize / width / channels * channels;
    pcm.samples.resize(count);
    for (size_t i = 0; i < count; ++i)
    {
        const uint8_t *p = data + i * width;
        int32_t value = 0;
        if (format == 3)
        {
            float f;
            std::memcpy(&f, p, 4);
            value = static_cast<int32_t>(std::lround(std::clamp(f, -1.0f, 1.0f) * 32767.0f));
        }
        else if (width == 1)
            value = (static_cast<int32_t>(p[0]) - 128) << 8;
        else if (width == 2)
            value = static_cast<int16_t>(p[0] | (p[1] << 8));
        else
            value = static_cast<int32_t>((static_cast<uint32_t>(p[0]) << 8) | (static_cast<uint32_t>(p[1]) << 16) |
                                         (static_cast<uint32_t>(p[2]) << 24)) >> 16;
        pcm.samples[i] = static_cast<int16_t>(value);
    }
    if (pcm.samples.empty())
    {
        error = "WAV has no samples";
        return std::nullopt;
    }
    return pcm;
}

class SoundFiles
{
  public:
    SoundFiles(const std::filesystem::path &exportRoot, const std::vector<std::filesystem::path> &searchPaths)
    {
        roots_.push_back(exportRoot / "sound");
        for (const auto &path : searchPaths)
            roots_.push_back(path / "sound");
        for (const auto &path : searchPaths)
        {
            std::error_code error;
            if (!std::filesystem::is_directory(path, error))
                continue;
            std::vector<std::filesystem::path> archives;
            for (const auto &entry : std::filesystem::directory_iterator(path, error))
                if (entry.is_regular_file() && Lower(entry.path().extension().string()) == ".iwd")
                    archives.push_back(entry.path());
            // IW3 searches later archives first (iw_13 overrides iw_00).
            std::sort(archives.rbegin(), archives.rend());
            for (const auto &archive : archives)
            {
                try
                {
                    iwds_.push_back(std::make_unique<Iwd>(archive));
                }
                catch (const std::exception &failure)
                {
                    zt::warn("iw3 sound: skipped %s: %s", archive.string().c_str(), failure.what());
                }
            }
        }
    }

    std::optional<std::vector<uint8_t>> Find(const std::string &path, std::string &source) const
    {
        for (const auto &root : roots_)
        {
            std::vector<uint8_t> bytes;
            std::error_code error;
            const auto candidate = root / path;
            if (std::filesystem::is_regular_file(candidate, error) && zt::read_file(candidate.string(), bytes))
            {
                source = candidate.string();
                return bytes;
            }
        }
        const std::string member = "sound/" + path;
        for (const auto &iwd : iwds_)
            if (iwd->Contains(member))
            {
                source = member + " (IWD)";
                return iwd->Read(member);
            }
        return std::nullopt;
    }

  private:
    std::vector<std::filesystem::path> roots_;
    std::vector<std::unique_ptr<Iwd>> iwds_;
};

// --------------------------------------------------------------- aliases
struct Iw3Alias
{
    std::string file; // relative to sound/
    float volMin{1}, volMax{1};
    float pitchMin{1}, pitchMax{1};
    float distMin{120}, distMax{1000};
    float probability{1};
    float lfe{};
    int startDelay{};
    bool looping{};
    float center{};
    float envelopMin{}, envelopMax{}, envelopPercentage{};
    unsigned channel{1}; // IW3 entity channel (flags bits 8-13, soundaliases/channels.def order)
    std::string secondary;
    std::string curve; // IW3 volume falloff curve (soundaliases/<curve>.vfcurve)
    // IW7 aliases (export_sounds.cpp "iw7"): their own mix fields, which replace the
    // role template's.
    struct Native
    {
        std::string volmod, channel; // Replay names
        unsigned shape{};
        std::optional<unsigned> looping; // 0 nonlooping, 1 looping, 3 rlooping (stock loop template: 3)
        bool fullDryLevel{}, precached{};
        float donutFadeEnd{-1}, velocityMin{};
        std::array<std::string, 4> curves; // volume falloff, lpf, hpf, reverb send (IW7 names)
        float reverb{1}, farReverb{1};
        std::array<float, 3> smartPan{0, 0, 1};
        std::array<float, 3> spread{};
        uint8_t spreadAngle{};
        uint8_t masterPriority{};
        float masterPercentage{}, masterSlavePercentage{1};
        uint32_t contextType{}, contextValue{};
        std::string speakerMap, occlusionShape, dopplerPreset; // IW7 names
        uint32_t duck{};                                         // SndDuck id
    };
    std::optional<Native> native;
};

std::map<std::string, std::vector<Iw3Alias>> ReadZoneAliases(const std::filesystem::path &exportRoot)
{
    std::map<std::string, std::vector<Iw3Alias>> result;
    const auto directory = exportRoot / "soundaliases";
    std::error_code error;
    if (!std::filesystem::is_directory(directory, error))
        return result;
    for (const auto &entry : std::filesystem::directory_iterator(directory, error))
    {
        if (!entry.is_regular_file() || !entry.path().string().ends_with(".iw3.json"))
            continue;
        std::string text;
        if (!zt::read_file_str(entry.path().string(), text))
            continue;
        const Json json = Json::parse(text, nullptr, false);
        if (json.is_discarded() || !json.contains("aliases"))
            continue;
        const std::string name = Lower(json.value("name", std::string{}));
        for (const Json &source : json.at("aliases"))
        {
            Iw3Alias alias;
            const Json &file = source.at("file");
            alias.file = file.value("name", std::string{});
            if (file.contains("directory") && file.at("directory").is_string())
                alias.file = file.at("directory").get<std::string>() + "/" + alias.file;
            alias.file = NormalizeSoundPath(alias.file);
            alias.volMin = source.at("volume").value("min", 1.0f);
            alias.volMax = source.at("volume").value("max", 1.0f);
            alias.pitchMin = source.at("pitch").value("min", 1.0f);
            alias.pitchMax = source.at("pitch").value("max", 1.0f);
            alias.distMin = source.at("distance").value("min", 120.0f);
            alias.distMax = source.at("distance").value("max", 1000.0f);
            alias.probability = source.value("probability", 1.0f);
            alias.lfe = source.value("lfe_percentage", 0.0f);
            alias.startDelay = source.value("start_delay", 0);
            alias.looping = (source.value("flags", 0) & 1) != 0;
            alias.channel = (source.value("flags", 0u) >> 8) & 63u;
            alias.center = source.value("center_percentage", 0.0f);
            if (source.contains("envelope") && source.at("envelope").is_object())
            {
                alias.envelopMin = source.at("envelope").value("min", 0.0f);
                alias.envelopMax = source.at("envelope").value("max", 0.0f);
                alias.envelopPercentage = source.at("envelope").value("percentage", 0.0f);
            }
            if (source.contains("secondary_alias") && source.at("secondary_alias").is_string())
                alias.secondary = Lower(source.at("secondary_alias").get<std::string>());
            if (source.contains("volume_falloff_curve") && source.at("volume_falloff_curve").is_string())
            {
                // Curves from another zone carry IW3's ',' reference prefix.
                alias.curve = Lower(source.at("volume_falloff_curve").get<std::string>());
                while (!alias.curve.empty() && alias.curve.front() == ',')
                    alias.curve.erase(alias.curve.begin());
            }
            if (source.contains("iw7") && source.at("iw7").is_object())
            {
                const Json &fields = source.at("iw7");
                Iw3Alias::Native native;
                native.volmod = fields.value("volmod", std::string{});
                native.channel = fields.value("channel", std::string{});
                native.shape = fields.value("shape", 0u) & 7u;
                if (fields.contains("looping") && fields.at("looping").is_number_unsigned())
                    native.looping = fields.at("looping").get<unsigned>() & 7u;
                native.fullDryLevel = fields.value("full_dry_level", false);
                native.precached = fields.value("precached", false);
                native.donutFadeEnd = fields.value("donut_fade_end", -1.0f);
                native.velocityMin = fields.value("velocity_min", 0.0f);
                if (fields.contains("curves") && fields.at("curves").is_object())
                {
                    const Json &curves = fields.at("curves");
                    static constexpr std::array<const char *, 4> kKeys{"volume_falloff", "lpf", "hpf", "reverb_send"};
                    for (size_t i = 0; i < kKeys.size(); ++i)
                        native.curves[i] = Lower(curves.value(kKeys[i], std::string{}));
                }
                native.reverb = fields.value("reverb_multiplier", 1.0f);
                native.farReverb = fields.value("far_reverb_multiplier", 1.0f);
                const auto floats = [&](const char *key, auto &out) {
                    if (fields.contains(key) && fields.at(key).is_array())
                        for (size_t i = 0; i < out.size() && i < fields.at(key).size(); ++i)
                            if (fields.at(key)[i].is_number())
                                out[i] = fields.at(key)[i].get<float>();
                };
                floats("smart_pan", native.smartPan);
                if (fields.contains("stereo_spread") && fields.at("stereo_spread").is_array() &&
                    fields.at("stereo_spread").size() == 4)
                {
                    const Json &spread = fields.at("stereo_spread");
                    for (size_t i = 0; i < 3; ++i)
                        native.spread[i] = spread[i].get<float>();
                    native.spreadAngle = static_cast<uint8_t>(std::clamp(spread[3].get<int>(), 0, 255));
                }
                if (fields.contains("master") && fields.at("master").is_array() && fields.at("master").size() == 3)
                {
                    const Json &master = fields.at("master");
                    native.masterPriority = static_cast<uint8_t>(std::clamp(master[0].get<int>(), 0, 255));
                    native.masterPercentage = master[1].get<float>();
                    native.masterSlavePercentage = master[2].get<float>();
                }
                if (fields.contains("context") && fields.at("context").is_array() && fields.at("context").size() == 2)
                {
                    native.contextType = fields.at("context")[0].get<uint32_t>();
                    native.contextValue = fields.at("context")[1].get<uint32_t>();
                }
                native.speakerMap = Lower(fields.value("speaker_map", std::string{}));
                native.occlusionShape = Lower(fields.value("occlusion_shape", std::string{}));
                native.dopplerPreset = Lower(fields.value("doppler_preset", std::string{}));
                native.duck = fields.value("duck", 0u);
                alias.native = std::move(native);
            }
            if (!alias.file.empty())
                result[name].push_back(std::move(alias));
        }
    }
    return result;
}

std::vector<std::string> SplitCsv(const std::string &line)
{
    std::vector<std::string> fields;
    std::string field;
    for (const char c : line)
    {
        if (c == ',')
        {
            fields.push_back(Trim(field));
            field.clear();
        }
        else
            field += c;
    }
    fields.push_back(Trim(field));
    return fields;
}

// IW3 raw soundaliases/*.csv rows that apply to an MP map: a blank loadspec
// applies everywhere, a list only to the named levels ("all_mp" = every MP map)
// and a "!"-list to every level not named.
std::map<std::string, std::vector<Iw3Alias>> ReadCsvAliases(const std::vector<std::filesystem::path> &searchPaths,
                                                            const std::string &map)
{
    std::map<std::string, std::vector<Iw3Alias>> result;
    for (const auto &root : searchPaths)
    {
        const auto directory = root / "soundaliases";
        std::error_code error;
        if (!std::filesystem::is_directory(directory, error))
            continue;
        std::map<std::string, float> volumeMods;
        {
            std::string text;
            if (zt::read_file_str((directory / "volumemodgroups.def").string(), text))
            {
                std::istringstream lines(text);
                std::string line;
                while (std::getline(lines, line))
                {
                    std::istringstream words(line);
                    std::string key;
                    float value = 0;
                    if (words >> key >> value && !key.starts_with("//"))
                        volumeMods[Lower(key)] = value;
                }
            }
        }
        for (const auto &entry : std::filesystem::directory_iterator(directory, error))
        {
            if (!entry.is_regular_file() || Lower(entry.path().extension().string()) != ".csv")
                continue;
            std::string text;
            if (!zt::read_file_str(entry.path().string(), text))
                continue;
            std::istringstream lines(text);
            std::string line;
            std::map<std::string, size_t> columns;
            while (std::getline(lines, line))
            {
                const auto fields = SplitCsv(line);
                if (fields.empty() || fields[0].empty() || fields[0].starts_with("#") || fields[0].starts_with("//"))
                    continue;
                if (Lower(fields[0]) == "name")
                {
                    columns.clear();
                    for (size_t i = 0; i < fields.size(); ++i)
                        columns[Lower(fields[i])] = i;
                    continue;
                }
                if (columns.empty())
                    continue;
                const auto get = [&](const char *column) -> std::string {
                    const auto found = columns.find(column);
                    return found != columns.end() && found->second < fields.size() ? fields[found->second] : std::string{};
                };
                const auto number = [&](const char *column, const float fallback) {
                    const std::string value = get(column);
                    if (value.empty())
                        return fallback;
                    try
                    {
                        return std::stof(value);
                    }
                    catch (...)
                    {
                        return fallback;
                    }
                };
                std::istringstream specWords(Lower(get("loadspec")));
                std::vector<std::string> spec;
                for (std::string word; specWords >> word;)
                    spec.push_back(word);
                bool applies = spec.empty();
                if (!spec.empty())
                {
                    const bool negated = spec.front().starts_with("!");
                    bool named = false;
                    for (std::string word : spec)
                    {
                        if (word.starts_with("!"))
                            word.erase(0, 1);
                        named = named || word == map || word == "all_mp";
                    }
                    applies = negated ? !named : named;
                }
                if (!applies || get("file").empty())
                    continue;
                Iw3Alias alias;
                alias.file = NormalizeSoundPath(get("file"));
                const auto mod = volumeMods.find(Lower(get("vol_mod")));
                const float scale = mod != volumeMods.end() ? mod->second : 1.0f;
                alias.volMin = number("vol_min", 1.0f) * scale;
                alias.volMax = number("vol_max", alias.volMin / scale) * scale;
                alias.pitchMin = number("pitch_min", 1.0f);
                alias.pitchMax = number("pitch_max", alias.pitchMin);
                alias.distMin = number("dist_min", 120.0f);
                alias.distMax = number("dist_max", 1000.0f);
                alias.probability = number("probability", 1.0f);
                alias.lfe = number("lfe percentage", 0.0f);
                alias.startDelay = static_cast<int>(number("startdelay", 0.0f));
                alias.looping = Lower(get("loop")).find("looping") != std::string::npos;
                // The channel column by name, in soundaliases/channels.def order.
                static constexpr std::array<std::string_view, 33> kIw3Channels{
                    "physics",  "auto",    "auto2",       "autodog", "bulletimpact", "bulletwhizby",
                    "element",  "auto2d",  "vehicle",     "vehiclelimited", "menu",  "body",
                    "body2d",   "reload",  "reload2d",    "item",    "effects1",     "effects2",
                    "weapon",   "weapon2d", "nonshock",   "voice",   "local",        "local2",
                    "ambient",  "hurt",    "player1",     "player2", "music",        "musicnopause",
                    "mission",  "announcer", "shellshock"};
                const std::string channelName = Lower(get("channel"));
                for (std::size_t channel = 0; channel < kIw3Channels.size(); ++channel)
                    if (channelName == kIw3Channels[channel])
                        alias.channel = static_cast<unsigned>(channel);
                alias.secondary = Lower(get("secondaryaliasname"));
                result[Lower(fields[0])].push_back(std::move(alias));
            }
        }
    }
    return result;
}

// Replay stock aliases whose fields the converted IW3 aliases share. Volumes
// scale the IW3 values to the level of the stock analog in the Replay mix.
enum class Role
{
    explosion,   // stock "o2_barrel_explode"
    fireBurst,   // stock "dst_propane_fire_whoosh"
    loopEmitter, // stock "vehicle_fire_small" (IW8's port of the IW3 alias)
    breakable,   // stock "dst_personal_computer"
};

struct RoleTemplate
{
    uint32_t flags; // bit19 = precached (G_CStringMP_SoundAliasIndex), bits 10..17 channel
    int32_t volModIndex;
    const char *curve;
    float reverb, farReverb;
    float spreadMin, spreadMax, spreadMid;
    uint8_t spreadAngle;
    float gain;
};

const RoleTemplate &TemplateFor(const Role role)
{
    static const RoleTemplate explosion{0x92808, 38, "standard_concave", 0.0f, 1.0f, 200, 600, 600, 60, 1.0f};
    static const RoleTemplate fireBurst{0x92400, 37, "standard_concave", 1.0f, 1.0f, 50, 300, 300, 30, 0.35f};
    static const RoleTemplate loopEmitter{0x2A003, 75, "default", 1.0f, 1.0f, 0, 0, 0, 0, 0.18f};
    static const RoleTemplate breakable{0x92400, 36, "default", 1.0f, 1.0f, 0, 0, 0, 0, 0.45f};
    switch (role)
    {
    case Role::explosion:
        return explosion;
    case Role::fireBurst:
        return fireBurst;
    case Role::loopEmitter:
        return loopEmitter;
    default:
        return breakable;
    }
}

Role RoleFor(const std::string &name, const bool looping)
{
    if (looping)
        return Role::loopEmitter;
    const auto has = [&](const char *word) { return name.find(word) != std::string::npos; };
    if (has("glass") || has("break") || has("shatter") || has("tv_") || has("door"))
        return Role::breakable;
    if (has("fire") || has("flare") || has("flame") || has("burn"))
        return Role::fireBurst;
    return Role::explosion;
}

// Stock Replay map-bank curves (mp_shipment.all vfCurves), copied by value.
// IW3 SNDCURVE text: "SNDCURVE", the knot count, then "distance volume" knots,
// both normalized; Replay vfCurves use the same (distance, volume) points.
std::optional<iw8::sound::Curve> ReadIw3Curve(const std::filesystem::path &exportRoot,
                                              const std::string &name)
{
    std::string text;
    if (name.empty() || name.find_first_of("/\\.") != std::string::npos ||
        !zt::read_file_str((exportRoot / "soundaliases" / (name + ".vfcurve")).string(), text))
        return std::nullopt;
    ledger::Use("soundcurve", name);
    std::istringstream input(text);
    std::string magic;
    std::size_t count = 0;
    if (!(input >> magic >> count) || magic != "SNDCURVE" || count < 2 || count > 16)
        return std::nullopt;
    iw8::sound::Curve curve{"mw120r_" + name, {}};
    float previous = -1.0f;
    for (std::size_t index = 0; index < count; ++index)
    {
        float distance{}, volume{};
        if (!(input >> distance >> volume) || !std::isfinite(distance) || !std::isfinite(volume) ||
            distance < 0.0f || distance > 1.0f || distance < previous || volume < 0.0f ||
            volume > 1.0f)
            return std::nullopt;
        previous = distance;
        curve.points.push_back({distance, volume});
    }
    return curve;
}

iw8::sound::Curve CurveNamed(const std::string &name)
{
    if (name == "standard_concave")
        return {name, {{0.0f, 1.0f}, {0.1047f, 0.6834f}, {0.2982f, 0.4123f}, {0.6039f, 0.1756f}, {1.0f, 0.0f}}};
    return {"default", {{0.0f, 1.0f}, {1.0f, 0.0f}}};
}
} // namespace

std::set<std::string> ReadScriptSoundNames(const std::filesystem::path &scriptDirectory)
{
    std::set<std::string> names;
    std::error_code error;
    if (!std::filesystem::is_directory(scriptDirectory, error))
        return names;
    static const std::regex direct(
        R"re(\b(?:playsound|playloopsound|playsoundasmaster|soundexists|ambientplay)\s*\(\s*"([^"]+)")re", std::regex::icase);
    static const std::regex positional(R"re(\bplaysoundatpos\s*\([^;]*?,\s*"([^"]+)"\s*\))re", std::regex::icase);
    // createfx loop sounds: ent.v[ "soundalias" ] = "alias";
    static const std::regex createfx(R"re(\[\s*"soundalias"\s*\]\s*=\s*"([^"]+)")re", std::regex::icase);
    for (const auto &entry : std::filesystem::directory_iterator(scriptDirectory, error))
    {
        if (Lower(entry.path().extension().string()) != ".gsc")
            continue;
        std::string text;
        if (!zt::read_file_str(entry.path().string(), text))
            continue;
        for (const std::regex *pattern : {&direct, &positional, &createfx})
            for (auto match = std::sregex_iterator(text.begin(), text.end(), *pattern);
                 match != std::sregex_iterator(); ++match)
                names.insert(Lower((*match)[1].str()));
    }
    return names;
}

std::set<std::string> ReadScriptStringLiterals(const std::filesystem::path &scriptDirectory)
{
    std::set<std::string> names;
    std::error_code error;
    if (!std::filesystem::is_directory(scriptDirectory, error))
        return names;
    static const std::regex literal(R"re("([A-Za-z0-9_]+)")re");
    for (const auto &entry : std::filesystem::directory_iterator(scriptDirectory, error))
    {
        if (Lower(entry.path().extension().string()) != ".gsc")
            continue;
        std::string text;
        if (!zt::read_file_str(entry.path().string(), text))
            continue;
        for (auto match = std::sregex_iterator(text.begin(), text.end(), literal);
             match != std::sregex_iterator(); ++match)
            names.insert(Lower((*match)[1].str()));
    }
    return names;
}

namespace
{
// ------------------------------------------------------------ sound table
// sound/soundtable.json (IW7 exporter, export_sounds.cpp): the source bank's
// SoundTable and send effects with the Replay field names of
// games/iw8/structs.hpp; channels and volmods by name.
enum class TableField
{
    u8,
    u16,
    i16,
    u32,
    i32,
    f32,
    str,
    char64,
    channel8,
    channel16,
    channel32,
    volmod16,
};

struct TableFieldSpec
{
    const char *key;
    uint32_t offset;
    TableField kind;
};

struct TableSpec
{
    const char *key; // JSON array
    uint32_t stride; // Replay record size
    std::vector<TableFieldSpec> fields;
};

const TableSpec &SendEffectSpec()
{
    static const TableSpec spec = [] {
        TableSpec result{"send_effects", 236, {{"name", 0, TableField::char64}, {"id", 64, TableField::u32}}};
        static constexpr std::array<const char *, 17> kFloats{
            "smoothing", "earlyTime", "lateTime", "earlyGain", "lateGain", "lateGainProx", "returnGain", "earlyLpf",
            "lateLpf", "inputLpf", "dampLpf", "wallReflect", "dryGain", "earlySize", "lateSize", "diffusion",
            "rearLevel"};
        for (size_t i = 0; i < kFloats.size(); ++i)
            result.fields.push_back({kFloats[i], static_cast<uint32_t>(68 + i * 4), TableField::f32});
        // Replay's five delay taps (+136, 20 B each: bool enabled, then four floats;
        // IW7 has none, so they stay disabled).
        static constexpr std::array<std::array<const char *, 5>, 5> kDelays{{
            {"delay1enabled", "delay1delayTimeMS", "delay1Level", "delay1LpfCutoff", "delay1HpfCutoff"},
            {"delay2enabled", "delay2delayTimeMS", "delay2Level", "delay2LpfCutoff", "delay2HpfCutoff"},
            {"delay3enabled", "delay3delayTimeMS", "delay3Level", "delay3LpfCutoff", "delay3HpfCutoff"},
            {"delay4enabled", "delay4delayTimeMS", "delay4Level", "delay4LpfCutoff", "delay4HpfCutoff"},
            {"delay5enabled", "delay5delayTimeMS", "delay5Level", "delay5LpfCutoff", "delay5HpfCutoff"},
        }};
        for (size_t i = 0; i < kDelays.size(); ++i)
            for (size_t f = 0; f < 5; ++f)
                result.fields.push_back({kDelays[i][f], static_cast<uint32_t>(136 + i * 20 + f * 4),
                                         f ? TableField::f32 : TableField::u8});
        return result;
    }();
    return spec;
}

// In iw8::sound::TableKind order.
const std::array<TableSpec, iw8::sound::TABLE_COUNT> &TableSpecs()
{
    using F = TableField;
    static const std::array<TableSpec, iw8::sound::TABLE_COUNT> specs{{
        {"adsr",
         48,
         {{"name", 0, F::str},
          {"id", 8, F::u32},
          {"attackLength", 12, F::f32},
          {"attackCurve", 16, F::u32},
          {"decayLength", 20, F::f32},
          {"decayCurve", 24, F::u32},
          {"releaseLength", 28, F::f32},
          {"releaseCurve", 32, F::u32},
          {"sustainLevel", 36, F::f32},
          {"sustainLength", 40, F::f32}}},
        {"ambient_defs",
         24,
         {{"name", 0, F::str},
          {"id", 8, F::u32},
          {"ambientEventIndex", 12, F::u16},
          {"numEvents", 14, F::u16},
          {"timeMin", 16, F::f32},
          {"timeMax", 20, F::f32}}},
        {"ambient_events", 12, {{"id", 0, F::u32}, {"ambientElementIndex", 4, F::u16}, {"weight", 8, F::f32}}},
        // unusedNameField (+8) is not streamed: it stays null.
        {"ambient_elements",
         40,
         {{"id", 0, F::u32},
          {"aliasName", 16, F::str},
          {"rangeMin", 24, F::f32},
          {"rangeMax", 28, F::f32},
          {"coneMin", 32, F::f32},
          {"coneMax", 36, F::f32}}},
        {"filters",
         24,
         {{"id", 0, F::u32},
          {"entChannel", 4, F::channel8},
          {"band", 5, F::u8},
          {"type", 8, F::u32},
          {"freq", 12, F::f32},
          {"gain", 16, F::f32},
          {"q", 20, F::f32}}},
        {"mixes", 16, {{"id", 0, F::u32}, {"volMod", 4, F::volmod16}, {"volume", 8, F::f32}, {"fade", 12, F::f32}}},
        {"npc_adsr_zones",
         32,
         {{"name", 0, F::str},
          {"id", 8, F::u32},
          {"weaponName", 16, F::str},
          {"adsrIdx", 24, F::i32},
          {"weaponIdx", 28, F::u32}}},
        {"occlusion_filters",
         40,
         {{"id", 0, F::u32},
          {"entChannel", 4, F::channel16},
          {"cutoffLevel1", 8, F::f32},
          {"cutoffLevel2", 12, F::f32},
          {"cutoffLevel3", 16, F::f32},
          {"cutoffLevel4", 20, F::f32},
          {"attnLevel1", 24, F::f32},
          {"attnLevel2", 28, F::f32},
          {"attnLevel3", 32, F::f32},
          {"attnLevel4", 36, F::f32}}},
        {"player_adsr_zones",
         32,
         {{"name", 0, F::str},
          {"id", 8, F::u32},
          {"weaponName", 16, F::str},
          {"adsrIdx", 24, F::i32},
          {"weaponIdx", 28, F::u32}}},
        {"reverbs",
         84,
         {{"id", 0, F::u32},
          {"roomTypeId", 4, F::u32},
          {"roomType", 8, F::char64},
          {"dryLevel", 72, F::f32},
          {"wetLevel", 76, F::f32},
          {"fadeTime", 80, F::f32}}},
        {"timescales",
         16,
         {{"id", 0, F::u32}, {"presetName", 4, F::u32}, {"entChannel", 8, F::channel32}, {"scale", 12, F::f32}}},
        {"zones",
         200,
         {{"id", 0, F::u32},
          {"stateId", 4, F::u32},
          {"reverbIndex", 8, F::i16},
          {"startOcclusionIndex", 10, F::i16},
          {"numOcclusion", 12, F::i16},
          {"startFilterIndex", 14, F::i16},
          {"numFilter", 16, F::i16},
          {"startMixIndex", 18, F::i16},
          {"numMix", 20, F::i16},
          {"ambientStream", 24, F::str},
          {"startAdsrZoneIndex", 32, F::i16},
          {"numAdsrZone", 34, F::i16},
          {"startNPCAdsrZoneIndex", 36, F::i16},
          {"numNPCAdsrZone", 38, F::i16},
          {"ambientDefIndex", 40, F::i16},
          {"startFullOccIndex", 42, F::i16},
          {"numDisableFullOcc", 44, F::i16},
          {"startPlayerBreathStateIndex", 46, F::i16},
          {"numPlayerBreathStates", 48, F::i16},
          {"weapReflId", 52, F::u32},
          {"exterior", 56, F::u8},
          {"zoneName", 64, F::str},
          {"stateName", 72, F::str},
          {"ambientName", 80, F::str},
          {"mixName", 88, F::str},
          {"reverbName", 96, F::str},
          {"filterName", 104, F::str},
          {"occlusionName", 112, F::str},
          {"plrAdsrName", 120, F::str},
          {"npcAdsrName", 128, F::str},
          {"weapReflName", 136, F::str},
          {"fullOccName", 144, F::str},
          {"playerBreathStateName", 152, F::str},
          {"contextType1", 160, F::u32},
          {"contextValue1", 164, F::u32},
          {"contextType2", 168, F::u32},
          {"contextValue2", 172, F::u32},
          {"contextType3", 176, F::u32},
          {"contextValue3", 180, F::u32},
          {"contextType4", 184, F::u32},
          {"contextValue4", 188, F::u32},
          {"duck", 192, F::u32}}},
        {"full_occlusions", 12, {{"id", 0, F::u32}, {"presetName", 4, F::u32}, {"entChannel", 8, F::channel32}}},
        {"breath_states",
         32,
         {{"stateType", 0, F::u32},
          {"inhaleAlias", 8, F::str},
          {"exhaleAlias", 16, F::str},
          {"minBreaths", 24, F::i32}}},
    }};
    return specs;
}

template <class T> void StoreField(std::vector<uint8_t> &bytes, const size_t offset, const T value)
{
    std::memcpy(bytes.data() + offset, &value, sizeof(value));
}

// Packs one JSON array into Replay records. Unknown channel / volmod names fall
// back to index 0 with a warning (the exporter maps IW7 names first).
iw8::sound::TableArray PackTable(const Json &json, const TableSpec &spec, std::set<std::string> &badNames)
{
    iw8::sound::TableArray array;
    array.stride = spec.stride;
    for (const auto &field : spec.fields)
        if (field.kind == TableField::str)
            array.stringOffsets.push_back(field.offset);
    if (!json.contains(spec.key) || !json.at(spec.key).is_array())
        return array;
    for (const Json &item : json.at(spec.key))
    {
        const size_t base = array.records.size();
        array.records.resize(base + spec.stride, 0);
        for (const auto &field : spec.fields)
        {
            const size_t at = base + field.offset;
            const bool has = item.contains(field.key) && !item.at(field.key).is_null();
            const Json &value = has ? item.at(field.key) : Json();
            const auto number = [&]() -> double { return has && value.is_number() ? value.get<double>() : 0.0; };
            const auto name = [&]() { return has && value.is_string() ? value.get<std::string>() : std::string{}; };
            const auto index = [&](const bool channel) -> int {
                const std::string text = name();
                const int found = channel ? iw8::sound::EntChannelIndex(text) : iw8::sound::VolModIndex(text);
                if (found < 0 && !text.empty())
                    badNames.insert(std::string(channel ? "channel " : "volmod ") + text);
                return std::max(found, 0);
            };
            switch (field.kind)
            {
            case TableField::u8:
                array.records[at] = static_cast<uint8_t>(has && value.is_boolean() ? value.get<bool>() : number());
                break;
            case TableField::u16:
                StoreField(array.records, at, static_cast<uint16_t>(number()));
                break;
            case TableField::i16:
                StoreField(array.records, at, static_cast<int16_t>(number()));
                break;
            case TableField::u32:
                StoreField(array.records, at, static_cast<uint32_t>(has && value.is_number_unsigned()
                                                                        ? value.get<uint64_t>()
                                                                        : static_cast<int64_t>(number())));
                break;
            case TableField::i32:
                StoreField(array.records, at, static_cast<int32_t>(number()));
                break;
            case TableField::f32:
                StoreField(array.records, at, static_cast<float>(number()));
                break;
            case TableField::str:
                array.strings.push_back(name());
                break;
            case TableField::char64: {
                const std::string text = name().substr(0, 63);
                std::memcpy(array.records.data() + at, text.data(), text.size());
                break;
            }
            case TableField::channel8:
                array.records[at] = static_cast<uint8_t>(index(true));
                break;
            case TableField::channel16:
                StoreField(array.records, at, static_cast<uint16_t>(index(true)));
                break;
            case TableField::channel32:
                StoreField(array.records, at, static_cast<int32_t>(index(true)));
                break;
            case TableField::volmod16:
                StoreField(array.records, at, static_cast<uint16_t>(index(false)));
                break;
            }
        }
    }
    return array;
}

struct LoadedTable
{
    iw8::sound::SoundTable table;
    iw8::sound::TableArray sendEffects;
    std::set<std::string> aliases; // zone ambient streams and ambient elements
};

std::optional<LoadedTable> ReadSoundTable(const std::filesystem::path &exportRoot, const std::string &map)
{
    std::string text;
    const auto path = exportRoot / "sound" / "soundtable.json";
    if (!std::filesystem::exists(path) || !zt::read_file_str(path.string(), text))
        return std::nullopt;
    Json json = Json::parse(text, nullptr, false);
    if (json.is_discarded() || !json.is_object())
    {
        zt::warn("iw3 sound: %s is not valid JSON; the map has no sound zones", path.string().c_str());
        return std::nullopt;
    }
    // The source map's own zone is the map's default zone (stock mp_shipment.all
    // holds zone "mp_shipment", id = the table id): it takes the converted name.
    const std::string source = Lower(json.value("map", std::string{}));
    const std::string target = Lower(map);
    if (json.contains("zones") && json.at("zones").is_array())
        for (Json &zone : json.at("zones"))
            if (!source.empty() && Lower(zone.value("zoneName", std::string{})) == source)
            {
                zone["zoneName"] = target;
                zone["id"] = iw8::sound::HashName(target);
            }

    // Breath states are Replay's player breathing (state types shared with IW7, whose
    // aliases name IW7 weapon sounds): every bank carries stock mp_shipment.all's 23
    // states with the plr_breath_* aliases of the common banks, and each zone that
    // uses breath states takes all of them, as every stock zone does.
    if (json.contains("breath_states") && json.at("breath_states").is_array() &&
        !json.at("breath_states").empty())
    {
        static constexpr struct
        {
            uint32_t stateType;
            const char *inhale, *exhale;
            int minBreaths;
        } kStockBreathStates[] = {
            {0xfda9899f, "plr_breath_null", "plr_breath_null", 1},
            {0x9fdde97c, "plr_breath_sniper_gasp", "plr_breath_null", 1},
            {0xd5429184, "plr_breath_sniper_exh", "plr_breath_sniper_inh", 1},
            {0xfe5eb7b9, "plr_breath_nose_inh", "plr_breath_nose_exh", 2},
            {0x35459053, "plr_breath_jump", "plr_breath_null", 1},
            {0x58194f32, "plr_breath_land_hi", "plr_breath_null", 1},
            {0xf5b29e44, "plr_breath_land_lo", "plr_breath_null", 1},
            {0x19557838, "plr_breath_climb", "plr_breath_null", 1},
            {0x35d1876d, "plr_breath_sprint_rec_exh", "plr_breath_null", 1},
            {0xb23dfa3c, "plr_breath_melee", "plr_breath_null", 1},
            {0xa4312752, "plr_breath_offhand_throw", "plr_breath_null", 1},
            {0xc442f7a9, "plr_breath_pain_init", "plr_breath_null", 1},
            {0x58d6c020, "plr_breath_pain_ong_inh", "plr_breath_pain_ong_exh", 1},
            {0xfe6f86f0, "plr_breath_pain_rec_inh", "plr_breath_pain_rec_exh", 1},
            {0x3059c366, "plr_breath_run_inh", "plr_breath_run_exh", 2},
            {0xbe1ff69b, "plr_breath_run_rec_inh", "plr_breath_run_rec_exh", 2},
            {0x02e09fc3, "plr_breath_sniper_gasp", "plr_breath_null", 1},
            {0x4debe9bf, "plr_breath_sprint_inh", "plr_breath_sprint_exh", 2},
            {0x6dd92af4, "plr_breath_sprint_rec_inh", "plr_breath_sniper_gasp", 2},
            {0x51bfc2f0, "plr_breath_supersprint_inh", "plr_breath_supersprint_exh", 2},
            {0x8843f3ae, "plr_breath_idle_walk_inh", "plr_breath_idle_walk_exh", 2},
            {0x385edb59, "plr_breath_null", "plr_breath_null", 2},
            {0xc244174e, "plr_breath_null", "plr_breath_null", 2},
        };
        Json states = Json::array();
        for (const auto &state : kStockBreathStates)
            states.push_back({{"stateType", state.stateType},
                              {"inhaleAlias", state.inhale},
                              {"exhaleAlias", state.exhale},
                              {"minBreaths", state.minBreaths}});
        json["breath_states"] = std::move(states);
        if (json.contains("zones") && json.at("zones").is_array())
            for (Json &zone : json.at("zones"))
                if (zone.value("numPlayerBreathStates", 0) > 0)
                {
                    zone["startPlayerBreathStateIndex"] = 0;
                    zone["numPlayerBreathStates"] = static_cast<int>(std::size(kStockBreathStates));
                }
    }

    // ADSR settings and their zone entries name source-game weapons; stock Replay map
    // banks hold none (mp_shipment.all: 0 / 0 / 0) and no zone references them.
    for (const char *key : {"adsr", "npc_adsr_zones", "player_adsr_zones"})
        json.erase(key);
    if (json.contains("zones") && json.at("zones").is_array())
        for (Json &zone : json.at("zones"))
        {
            zone["startAdsrZoneIndex"] = -1;
            zone["numAdsrZone"] = 0;
            zone["startNPCAdsrZoneIndex"] = -1;
            zone["numNPCAdsrZone"] = 0;
            zone.erase("plrAdsrName");
            zone.erase("npcAdsrName");
            // weapReflId selects one of the bank's weapon reflections; converted
            // banks carry none.
            zone["weapReflId"] = 0;
        }

    LoadedTable result;
    std::set<std::string> badNames;
    result.table.id = iw8::sound::HashName(target);
    for (size_t kind = 0; kind < iw8::sound::TABLE_COUNT; ++kind)
        result.table.arrays[kind] = PackTable(json, TableSpecs()[kind], badNames);
    result.sendEffects = PackTable(json, SendEffectSpec(), badNames);
    for (const auto &name : badNames)
        zt::warn("iw3 sound: sound table names unknown Replay %s; it uses index 0", name.c_str());

    // Every zone index must address its array (Replay does not check them).
    const auto count = [&](const iw8::sound::TableKind kind) {
        return static_cast<int>(result.table.arrays[kind].count());
    };
    const auto &zones = result.table.arrays[iw8::sound::TABLE_ZONE];
    const auto i16 = [&](const size_t zone, const uint32_t offset) {
        int16_t value;
        std::memcpy(&value, zones.records.data() + zone * zones.stride + offset, sizeof(value));
        return static_cast<int>(value);
    };
    const struct
    {
        uint32_t start, num;
        iw8::sound::TableKind kind;
    } ranges[] = {{10, 12, iw8::sound::TABLE_OCCLUSION},       {14, 16, iw8::sound::TABLE_FILTER},
                  {18, 20, iw8::sound::TABLE_MIX},             {32, 34, iw8::sound::TABLE_PLAYER_ADSR_ZONE},
                  {36, 38, iw8::sound::TABLE_NPC_ADSR_ZONE},   {42, 44, iw8::sound::TABLE_FULL_OCCLUSION},
                  {46, 48, iw8::sound::TABLE_BREATH_STATE}};
    for (size_t z = 0; z < zones.count(); ++z)
    {
        const std::string &name = zones.strings[z * zones.stringOffsets.size() + 1];
        const int reverb = i16(z, 8);
        bool bad = reverb >= count(iw8::sound::TABLE_REVERB) || reverb < -1;
        for (const auto &range : ranges)
        {
            const int start = i16(z, range.start), num = i16(z, range.num);
            bad = bad || (num > 0 && (start < 0 || start + num > count(range.kind)));
        }
        const int ambient = i16(z, 40);
        bad = bad || ambient >= count(iw8::sound::TABLE_AMBIENT_DEF) || ambient < -1;
        if (bad)
            throw std::runtime_error("sound table zone " + name + " indexes past its arrays");
    }

    // The map aliases the table plays (the stock breath aliases live in the common
    // banks: stock map banks hold none).
    const auto collect = [&](const iw8::sound::TableKind kind, const uint32_t offset) {
        const auto &array = result.table.arrays[kind];
        const auto field = std::ranges::find(array.stringOffsets, offset) - array.stringOffsets.begin();
        for (size_t i = 0; i < array.count(); ++i)
            if (const auto &alias = array.strings[i * array.stringOffsets.size() + field]; !alias.empty())
                result.aliases.insert(Lower(alias));
    };
    collect(iw8::sound::TABLE_ZONE, 24);
    collect(iw8::sound::TABLE_AMBIENT_ELEMENT, 16);
    zt::info("iw3 sound: sound table %08x: %zu zone(s), %zu reverb(s), %zu filter(s), %zu mix(es), %zu "
             "occlusion filter(s), %zu send effect(s)",
             result.table.id, zones.count(), result.table.arrays[iw8::sound::TABLE_REVERB].count(),
             result.table.arrays[iw8::sound::TABLE_FILTER].count(),
             result.table.arrays[iw8::sound::TABLE_MIX].count(),
             result.table.arrays[iw8::sound::TABLE_OCCLUSION].count(), result.sendEffects.count());
    return result;
}
// sound/curves.json (IW7 exporter): the IW7 curves the aliases name, by kind
// (volume falloff, lpf, hpf, reverb send) and name, as (x, y) points.
std::array<std::map<std::string, iw8::sound::Curve>, 4> ReadIw7Curves(const std::filesystem::path &exportRoot)
{
    std::array<std::map<std::string, iw8::sound::Curve>, 4> result;
    std::string text;
    if (!zt::read_file_str((exportRoot / "sound" / "curves.json").string(), text))
        return result;
    const Json json = Json::parse(text, nullptr, false);
    if (json.is_discarded() || !json.is_object())
        return result;
    static constexpr std::array<const char *, 4> kKeys{"volume_falloff", "lpf", "hpf", "reverb_send"};
    for (size_t kind = 0; kind < kKeys.size(); ++kind)
    {
        if (!json.contains(kKeys[kind]) || !json.at(kKeys[kind]).is_object())
            continue;
        for (const auto &entry : json.at(kKeys[kind]).items())
        {
            const std::string curveName = entry.key();
            const Json &points = entry.value();
            // Bank curves take a prefix: Replay's common banks hold curves of the same names.
            iw8::sound::Curve curve{"mw120r_iw7_" + Lower(curveName), {}};
            for (const Json &point : points)
                if (point.is_array() && point.size() == 2)
                    curve.points.push_back({point[0].get<float>(), point[1].get<float>()});
            if (!curve.points.empty() && curve.points.size() <= 16 && curve.name.size() < 64)
                result[kind].emplace(Lower(curveName), std::move(curve));
            else
                zt::warn("iw3 sound: IW7 curve %s has %zu points; its aliases use the default", curveName.c_str(),
                         curve.points.size());
        }
    }
    return result;
}
// The presets in sound/curves.json as Replay records by kind (0 doppler 88 B,
// 1 occlusion shape 100 B, 2 speaker map 452 B) and IW7 name; each record names
// itself mw120r_iw7_<name>.
std::array<std::map<std::string, std::vector<uint8_t>>, 3> ReadIw7Presets(const std::filesystem::path &exportRoot)
{
    std::array<std::map<std::string, std::vector<uint8_t>>, 3> result;
    std::string text;
    if (!zt::read_file_str((exportRoot / "sound" / "curves.json").string(), text))
        return result;
    const Json json = Json::parse(text, nullptr, false);
    if (json.is_discarded() || !json.is_object())
        return result;
    static constexpr std::array<const char *, 3> kKeys{"doppler_presets", "occlusion_shapes", "speaker_maps"};
    static constexpr std::array<size_t, 3> kSizes{88, 100, 452};
    const auto put = [](std::vector<uint8_t> &record, const size_t offset, const Json &value, const size_t count) {
        for (size_t i = 0; i < count; ++i)
        {
            const float number = value.is_array() && i < value.size() && value[i].is_number() ? value[i].get<float>()
                                 : !value.is_array() && value.is_number()                    ? value.get<float>()
                                                                                             : 0.0f;
            std::memcpy(record.data() + offset + i * 4, &number, 4);
        }
    };
    for (size_t kind = 0; kind < kKeys.size(); ++kind)
    {
        if (!json.contains(kKeys[kind]) || !json.at(kKeys[kind]).is_object())
            continue;
        for (const auto &entry : json.at(kKeys[kind]).items())
        {
            const std::string presetName = "mw120r_iw7_" + Lower(entry.key());
            if (presetName.size() >= 64)
                continue;
            const Json &fields = entry.value();
            std::vector<uint8_t> record(kSizes[kind], 0);
            std::memcpy(record.data(), presetName.data(), presetName.size());
            const uint32_t id = iw8::sound::HashName(presetName);
            std::memcpy(record.data() + 64, &id, 4);
            const auto field = [&](const char *key) { return fields.contains(key) ? fields.at(key) : Json(); };
            if (kind == 0)
            {
                put(record, 68, field("speedOfSound"), 1);
                put(record, 72, field("playerVelocityScale"), 1);
                put(record, 76, field("minPitch"), 1);
                put(record, 80, field("maxPitch"), 1);
                put(record, 84, field("smoothing"), 1);
            }
            else if (kind == 1)
            {
                // yaw/pitch offsets (+92/+96, Replay only) stay 0.
                put(record, 68, field("innerAngleVolume"), 1);
                put(record, 72, field("outerAngleVolume"), 1);
                put(record, 76, field("occludedVolume"), 1);
                put(record, 80, field("innerAngleLPF"), 1);
                put(record, 84, field("outerAngleLPF"), 1);
                put(record, 88, field("occludedLPF"), 1);
            }
            else
            {
                put(record, 68, field("monoToStereo"), 2);
                put(record, 76, field("stereoToStereo"), 4);
                put(record, 92, field("monoTo51"), 6);
                put(record, 116, field("stereoTo51"), 12);
                put(record, 164, field("monoTo71"), 8);
                put(record, 196, field("stereoTo71"), 16);
                // 7.1.4 ([input][16 outputs]): the 7.1 outputs, no height channels, as
                // every stock speaker map (mp_shipment.all) holds them.
                const Json mono = field("monoTo71"), stereo = field("stereoTo71");
                put(record, 260, mono, 8);
                for (size_t input = 0; input < 2; ++input)
                {
                    Json row = Json::array();
                    for (size_t output = 0; output < 8; ++output)
                        row.push_back(stereo.is_array() && input * 8 + output < stereo.size() ? stereo[input * 8 + output]
                                                                                             : Json(0.0f));
                    put(record, 324 + input * 64, row, 8);
                }
            }
            result[kind].emplace(Lower(entry.key()), std::move(record));
        }
    }
    return result;
}
// sound/ducks.json (IW7 exporter): the ducks the aliases start, their per-volmod
// arrays by Replay volmod name (Replay volmods without an IW7 counterpart keep the
// stock defaults: no attenuation, no filter).
std::map<uint32_t, iw8::sound::Duck> ReadIw7Ducks(const std::filesystem::path &exportRoot)
{
    std::map<uint32_t, iw8::sound::Duck> result;
    std::string text;
    if (!zt::read_file_str((exportRoot / "sound" / "ducks.json").string(), text))
        return result;
    const Json json = Json::parse(text, nullptr, false);
    if (json.is_discarded() || !json.is_array())
        return result;
    for (const Json &item : json)
    {
        iw8::sound::Duck duck;
        duck.name = Lower(item.value("name", std::string{}));
        duck.id = item.value("id", 0u);
        if (duck.name.empty() || duck.name.size() >= 64 || !duck.id)
            continue;
        duck.fadeIn = item.value("fadeIn", 0.0f);
        duck.fadeOut = item.value("fadeOut", 0.0f);
        duck.startDelay = item.value("startDelay", 0.0f);
        duck.minDistance = item.value("minDistance", 0.0f);
        duck.distance = item.value("distance", 0.0f);
        duck.length = item.value("length", 0.0f);
        duck.fadeInCurve = item.value("fadeInCurve", 0u);
        duck.fadeOutCurve = item.value("fadeOutCurve", 0u);
        duck.updateWhilePaused = item.value("updateWhilePaused", false);
        duck.trackAmplitude = item.value("trackAmplitude", false);
        duck.disableInSplitscreen = item.value("disableInSplitscreen", false);
        duck.aliasAttenuation = item.value("aliasAttenuation", 1.0f);
        duck.aliasLpf = item.value("aliasLpf", 1.0f);
        duck.duckAlias = item.value("duckAlias", 0u);
        for (const auto &[key, values] : {std::pair{"attenuation", &duck.attenuation}, std::pair{"lpf", &duck.lpf}})
        {
            if (!item.contains(key) || !item.at(key).is_object())
                continue;
            for (const auto &entry : item.at(key).items())
                if (const int index = iw8::sound::VolModIndex(entry.key());
                    index >= 0 && index < 128 && entry.value().is_number())
                    (*values)[static_cast<size_t>(index)] = entry.value().get<float>();
        }
        const uint32_t id = duck.id;
        result.emplace(id, std::move(duck));
    }
    return result;
}
} // namespace

std::optional<ConvertedSounds> Convert(const std::filesystem::path &exportRoot,
                                       const std::vector<std::filesystem::path> &searchPaths,
                                       const std::string &map, const std::set<std::string> &requested,
                                       const std::set<std::string> &candidates)
{
    // A source sound table (IW7) ships even when no map alias converts: its reverb,
    // filter and mix zones shape every sound of the map (weapons and footsteps too).
    auto table = ReadSoundTable(exportRoot, map);
    if (requested.empty() && candidates.empty() && !table)
        return std::nullopt;
    const auto zoneAliases = ReadZoneAliases(exportRoot);
    const auto csvAliases = ReadCsvAliases(searchPaths, Lower(map));
    const auto definition = [&](const std::string &name) -> const std::vector<Iw3Alias> * {
        if (const auto found = zoneAliases.find(name); found != zoneAliases.end())
            return &found->second;
        if (const auto found = csvAliases.find(name); found != csvAliases.end())
            return &found->second;
        return nullptr;
    };

    // Requested aliases and their secondary chains (IW3 plays each secondary
    // alongside its primary; Replay follows SndAlias.secondaryId the same way).
    std::vector<std::string> order;
    std::set<std::string> visited;
    std::vector<std::string> pending(requested.begin(), requested.end());
    if (table)
        pending.insert(pending.end(), table->aliases.begin(), table->aliases.end());
    std::size_t literalAliases = 0;
    for (const auto &name : candidates)
        if (!requested.contains(name) && definition(Lower(name)))
        {
            pending.push_back(name);
            ++literalAliases;
        }
    if (literalAliases)
        zt::info("iw3 sound: %zu alias(es) passed to map script helpers as literals", literalAliases);
    while (!pending.empty())
    {
        const std::string name = Lower(pending.back());
        pending.pop_back();
        if (!visited.insert(name).second)
            continue;
        const auto *aliases = definition(name);
        if (!aliases)
        {
            zt::warn("iw3 sound: alias %s has no IW3 definition; left silent", name.c_str());
            continue;
        }
        order.push_back(name);
        for (const auto &alias : *aliases)
            if (!alias.secondary.empty())
                pending.push_back(alias.secondary);
    }

    SoundFiles files(exportRoot, searchPaths);
    std::map<std::string, iw8::sound::Sample> samples; // by IW3 path
    std::map<std::string, bool> missing;
    const std::string prefix = "mw120r\\" + Lower(map) + "\\";
    const auto sampleFor = [&](const std::string &path, const bool looping) -> const iw8::sound::Sample * {
        if (auto found = samples.find(path); found != samples.end())
        {
            found->second.looping = found->second.looping || looping;
            return &found->second;
        }
        if (missing.contains(path))
            return nullptr;
        std::string source, failure;
        const auto bytes = files.Find(path, source);
        std::optional<Pcm> pcm;
        if (bytes)
        {
            pcm = ReadWav(*bytes, failure);
            if (!pcm && Lower(path).ends_with(".mp3"))
                pcm = DecodeCompressedAudio(*bytes, failure);
        }
        else
            failure = "not found in the extracted zone, raw sound folders or IWDs";
        if (!pcm)
        {
            zt::warn("iw3 sound: %s: %s", path.c_str(), failure.c_str());
            missing[path] = true;
            return nullptr;
        }
        ledger::Use("loadedsound", path);
        iw8::sound::Sample sample;
        std::string stem = path.ends_with(".wav") || path.ends_with(".mp3")
                               ? path.substr(0, path.size() - 4)
                               : path;
        std::replace(stem.begin(), stem.end(), '/', '\\');
        sample.name = prefix + stem;
        sample.rate = pcm->rate;
        sample.channels = pcm->channels;
        sample.looping = looping;
        sample.pcm = std::move(pcm->samples);
        zt::info("iw3 sound: %s <- %s (%u Hz, %u ch, %.2f s)", sample.name.c_str(), source.c_str(), sample.rate,
                 static_cast<unsigned>(sample.channels),
                 double(sample.pcm.size() / sample.channels) / sample.rate);
        return &samples.emplace(path, std::move(sample)).first->second;
    };

    // First pass: load audio so lists whose files are all missing drop out
    // (and are not used as secondaries).
    std::set<std::string> converted;
    for (const auto &name : order)
    {
        bool any = false;
        for (const auto &alias : *definition(name))
            any = sampleFor(alias.file, alias.looping) != nullptr || any;
        if (any)
            converted.insert(name);
        else
            zt::warn("iw3 sound: alias %s has no usable audio; left silent", name.c_str());
    }
    if (converted.empty() && !table)
        return std::nullopt;

    ConvertedSounds result;
    result.bank.name = Lower(map) + ".all";
    // The loader opens zone\<zone>.sabs (and patch_<zone>.sabs) even for a
    // fully resident bank and fails the whole bank when it cannot. These two
    // stock files are the 4096-byte "no streamed sounds" placeholders, so the
    // bank loads in every Replay install without shipping another file.
    result.bank.zone = "vehicle_engine_type4_npc";
    result.bank.streamKey = Lower(map) + "_all_loaded";
    if (table)
    {
        // A zone bed without audio is cleared (stock zones without a bed hold a null
        // ambientStream) so no zone asks for a missing alias.
        auto &zones = table->table.arrays[iw8::sound::TABLE_ZONE];
        for (size_t i = 0; i < zones.count(); ++i)
            if (auto &alias = zones.strings[i * zones.stringOffsets.size()];
                !alias.empty() && !converted.contains(Lower(alias)))
            {
                zt::warn("iw3 sound: zone %s ambient stream %s has no audio; the zone plays no bed",
                         zones.strings[i * zones.stringOffsets.size() + 1].c_str(), alias.c_str());
                alias.clear();
            }
        result.bank.table = std::move(table->table);
        result.bank.sendEffects = std::move(table->sendEffects);
    }
    std::set<std::string> curves{"default"};
    std::map<std::string, std::optional<iw8::sound::Curve>> iw3Curves;
    const auto iw7Curves = ReadIw7Curves(exportRoot);
    const auto iw7Presets = ReadIw7Presets(exportRoot);
    const auto iw7Ducks = ReadIw7Ducks(exportRoot);
    std::set<uint32_t> usedDucks;
    std::array<std::set<std::string>, 3> usedIw7Presets;
    // The bank preset of an IW7 preset name, or 0 for Replay's "default".
    const auto iw7Preset = [&](const size_t kind, const std::string &presetName) -> uint32_t {
        if (presetName.empty() || presetName == "default")
            return 0;
        if (!iw7Presets[kind].contains(presetName))
        {
            zt::warn("iw3 sound: IW7 preset %s has no data; its aliases use the default", presetName.c_str());
            return 0;
        }
        usedIw7Presets[kind].insert(presetName);
        return iw8::sound::HashName("mw120r_iw7_" + presetName);
    };
    std::array<std::set<std::string>, 4> usedIw7Curves;
    // The bank curve of an IW7 curve name, or 0 for Replay's "default".
    const auto iw7Curve = [&](const size_t kind, const std::string &curveName) -> uint32_t {
        if (curveName.empty() || curveName == "default")
            return 0;
        const auto found = iw7Curves[kind].find(curveName);
        if (found == iw7Curves[kind].end())
        {
            zt::warn("iw3 sound: IW7 curve %s has no points; its aliases use the default", curveName.c_str());
            return 0;
        }
        usedIw7Curves[kind].insert(curveName);
        return iw8::sound::HashName(found->second.name);
    };
    for (const auto &name : order)
    {
        if (!converted.contains(name))
            continue;
        iw8::sound::AliasList list;
        list.name = name;
        bool looping = false;
        for (const auto &source : *definition(name))
        {
            const auto *sample = sampleFor(source.file, source.looping);
            if (!sample)
                continue;
            looping = looping || source.looping;
            const RoleTemplate &role = TemplateFor(RoleFor(name, source.looping));
            iw8::sound::Alias alias;
            alias.assetName = sample->name;
            if (!source.secondary.empty() && converted.contains(source.secondary))
                alias.secondaryName = source.secondary;
            alias.flags = role.flags;
            // IW3 plays 2D channels (channels.def: auto2d, menu, body2d, reload2d,
            // weapon2d, local, local2, ambient, hurt, player1/2, music, announcer,
            // shellshock) without spatialisation; the role templates' Replay channels
            // are 3D, so a 2D IW3 alias takes a 2D Replay entchannel (SndGlobals:
            // 125 amb_bed_2d for loops, 2 scn_fx_unres_2d otherwise; flags bits 10-17).
            static constexpr std::array<bool, 33> kIw3Channel2d{
                false, false, false, false, false, false, false, true,  false, false, true,
                false, true,  false, true,  false, false, false, false, true,  false, false,
                true,  true,  true,  true,  true,  true,  true,  true,  false, true,  true};
            if (source.channel < kIw3Channel2d.size() && kIw3Channel2d[source.channel])
                alias.flags = (alias.flags & ~(0xFFu << 10)) | ((source.looping ? 125u : 2u) << 10);
            alias.volModIndex = role.volModIndex;
            alias.volMin = std::clamp(source.volMin * role.gain, 0.0f, 1.0f);
            alias.volMax = std::clamp(std::max(source.volMax, source.volMin) * role.gain, 0.0f, 1.0f);
            alias.pitchMin = std::clamp(source.pitchMin, 0.25f, 4.0f);
            alias.pitchMax = std::clamp(std::max(source.pitchMax, source.pitchMin), 0.25f, 4.0f);
            alias.distMin = std::max(0.0f, source.distMin);
            alias.distMax = std::max(source.distMax, alias.distMin + 1.0f);
            alias.probability = source.probability > 0 ? source.probability : 1.0f;
            alias.lfePercentage = std::clamp(source.lfe, 0.0f, 1.0f);
            alias.centerPercentage = std::clamp(source.center, 0.0f, 1.0f);
            alias.envelopMin = std::max(0.0f, source.envelopMin);
            alias.envelopMax = std::max(alias.envelopMin, source.envelopMax);
            alias.envelopPercentage = std::clamp(source.envelopPercentage, 0.0f, 1.0f);
            alias.startDelay = std::max(0, source.startDelay);
            // The alias's own IW3 falloff curve, else IW3's linear default. Replay
            // evaluates vfCurves exactly as IW3 does: piecewise-linear knots
            // (0x141B75310) over t = (dist - distMin) / (distMax - distMin)
            // clamped to [0, 1] (0x141B71959); the donut fade (flags bits 4-6) is off.
            auto found = iw3Curves.find(source.curve);
            if (!source.curve.empty() && found == iw3Curves.end())
                found = iw3Curves.emplace(source.curve, ReadIw3Curve(exportRoot, source.curve)).first;
            if (!source.curve.empty() && found->second)
                alias.volumeFalloffCurve = iw8::sound::HashName(found->second->name);
            else
            {
                if (!source.curve.empty() && source.curve != "default")
                    zt::warn("iw3 sound: %s falloff curve %s is unreadable; linear default",
                             name.c_str(), source.curve.c_str());
                alias.volumeFalloffCurve = iw8::sound::HashName("default");
            }
            alias.reverbMultiplier = role.reverb;
            alias.farReverbMultiplier = role.farReverb;
            alias.stereoSpreadMinDist = role.spreadMin;
            alias.stereoSpreadMaxDist = role.spreadMax;
            alias.stereoSpreadMidPoint = role.spreadMid;
            alias.stereoSpreadMaxAngle = role.spreadAngle;
            if (source.native)
            {
                // IW7 aliases keep their own mix: the volume group, channel, curves,
                // reverb sends and spread they were mixed with (the role gain does not
                // apply). Flags: IW7's fields in the layout the two games share (bits 0-2
                // loop type, as the stock loop template's 3 = rlooping; bit 3 full dry
                // level; bits 4-6 shape; bit 19 precached), the channel at bits 10-17
                // (IW7: bit 9). Load type 0: the samples are resident.
                const auto &native = *source.native;
                if (const int volMod = iw8::sound::VolModIndex(native.volmod); volMod >= 0)
                    alias.volModIndex = volMod;
                else
                    zt::warn("iw3 sound: %s volmod %s is not a Replay volmod; the role template gives it",
                             name.c_str(), native.volmod.c_str());
                uint32_t channel = (role.flags >> 10) & 0xFFu;
                if (const int channelIndex = iw8::sound::EntChannelIndex(native.channel); channelIndex >= 0)
                    channel = static_cast<uint32_t>(channelIndex);
                else
                    zt::warn("iw3 sound: %s channel %s is not a Replay channel; the role template gives it",
                             name.c_str(), native.channel.c_str());
                alias.flags = native.looping.value_or(role.flags & 0x7u) | (native.fullDryLevel ? 0x8u : 0u) | (native.shape << 4) |
                              (channel << 10) | ((native.precached || (role.flags & 0x80000u)) ? 0x80000u : 0u);
                alias.volMin = std::clamp(source.volMin, 0.0f, 1.0f);
                alias.volMax = std::clamp(std::max(source.volMax, source.volMin), 0.0f, 1.0f);
                alias.donutFadeEnd = native.donutFadeEnd;
                alias.velocityMin = native.velocityMin;
                alias.volumeFalloffCurve = iw7Curve(0, native.curves[0]);
                if (!alias.volumeFalloffCurve)
                    alias.volumeFalloffCurve = iw8::sound::HashName("default");
                alias.lpfCurve = iw7Curve(1, native.curves[1]);
                alias.hpfCurve = iw7Curve(2, native.curves[2]);
                alias.reverbSendCurve = iw7Curve(3, native.curves[3]);
                alias.reverbMultiplier = native.reverb;
                alias.farReverbMultiplier = native.farReverb;
                alias.smartPanDistance2d = native.smartPan[0];
                alias.smartPanDistance3d = native.smartPan[1];
                alias.smartPanAttenuation2d = native.smartPan[2];
                alias.stereoSpreadMinDist = native.spread[0];
                alias.stereoSpreadMaxDist = native.spread[1];
                alias.stereoSpreadMidPoint = native.spread[2];
                alias.stereoSpreadMaxAngle = native.spreadAngle;
                alias.masterPriority = native.masterPriority;
                alias.masterPercentage = native.masterPercentage;
                alias.masterSlavePercentage = native.masterSlavePercentage;
                alias.dopplerPreset = iw7Preset(0, native.dopplerPreset);
                alias.occlusionShape = iw7Preset(1, native.occlusionShape);
                alias.speakerMap = iw7Preset(2, native.speakerMap);
                if (native.duck && iw7Ducks.contains(native.duck))
                {
                    alias.duck = native.duck;
                    usedDucks.insert(native.duck);
                }
                // A context type Replay lacks would never select the variant.
                if (native.contextType && iw8::sound::IsContextType(native.contextType))
                {
                    alias.contextType = native.contextType;
                    alias.contextValue = native.contextValue;
                }
            }
            const double seconds = double(sample->pcm.size() / sample->channels) / sample->rate;
            alias.maxDurationDsec = static_cast<int16_t>(std::clamp(std::ceil(seconds * 10.0), 1.0, 32767.0));
            list.aliases.push_back(std::move(alias));
        }
        result.aliases[name] = looping;
        zt::info("iw3 sound: alias %s -> %zu variant(s)%s", name.c_str(), list.aliases.size(),
                 looping ? " (looping)" : "");
        result.bank.lists.push_back(std::move(list));
    }
    if (result.bank.lists.empty())
    {
        // A bank holds at least one alias and one sound (RegisterBank): the table
        // ships with a silent placeholder that nothing plays.
        iw8::sound::Sample silence;
        silence.name = prefix + "table_placeholder";
        silence.rate = 48000;
        silence.channels = 1;
        silence.pcm.assign(4800, 0);
        iw8::sound::AliasList list;
        list.name = "mw120r_" + Lower(map) + "_table_placeholder";
        iw8::sound::Alias alias;
        alias.assetName = silence.name;
        alias.volumeFalloffCurve = iw8::sound::HashName("default");
        alias.maxDurationDsec = 1;
        list.aliases.push_back(std::move(alias));
        result.bank.lists.push_back(std::move(list));
        samples.emplace(silence.name, std::move(silence));
        zt::warn("iw3 sound: no map alias has audio; the bank ships the sound table with a silent placeholder alias");
    }
    for (const auto &name : curves)
        result.bank.volumeCurves.push_back(CurveNamed(name));
    std::size_t convertedCurves = 0;
    for (const auto &[name, curve] : iw3Curves)
        if (curve)
        {
            result.bank.volumeCurves.push_back(*curve);
            ++convertedCurves;
        }
        else
            zt::warn("iw3 sound: falloff curve %s did not convert; its aliases use the role curve",
                     name.c_str());
    if (convertedCurves)
        zt::info("iw3 sound: converted %zu IW3 falloff curve(s)", convertedCurves);
    const std::array<std::vector<iw8::sound::Curve> *, 4> bankCurves{
        &result.bank.volumeCurves, &result.bank.lpfCurves, &result.bank.hpfCurves, &result.bank.reverbCurves};
    std::size_t iw7CurveCount = 0;
    for (size_t kind = 0; kind < bankCurves.size(); ++kind)
        for (const auto &curveName : usedIw7Curves[kind])
        {
            bankCurves[kind]->push_back(iw7Curves[kind].at(curveName));
            ++iw7CurveCount;
        }
    if (iw7CurveCount)
        zt::info("iw3 sound: converted %zu IW7 curve(s)", iw7CurveCount);
    const std::array<std::pair<iw8::sound::TableArray *, uint32_t>, 3> bankPresets{
        std::pair{&result.bank.dopplerPresets, 88u}, std::pair{&result.bank.occlusionShapes, 100u},
        std::pair{&result.bank.speakerMaps, 452u}};
    for (size_t kind = 0; kind < bankPresets.size(); ++kind)
    {
        auto &[array, stride] = bankPresets[kind];
        array->stride = stride;
        for (const auto &presetName : usedIw7Presets[kind])
        {
            const auto &record = iw7Presets[kind].at(presetName);
            array->records.insert(array->records.end(), record.begin(), record.end());
        }
    }
    for (const uint32_t id : usedDucks)
        result.bank.ducks.push_back(iw7Ducks.at(id));
    if (!usedDucks.empty())
        zt::info("iw3 sound: converted %zu IW7 duck(s)", usedDucks.size());
    if (std::ranges::any_of(usedIw7Presets, [](const auto &used) { return !used.empty(); }))
        zt::info("iw3 sound: converted %zu IW7 doppler preset(s), %zu occlusion shape(s), %zu speaker map(s)",
                 usedIw7Presets[0].size(), usedIw7Presets[1].size(), usedIw7Presets[2].size());
    std::vector<iw8::sound::Sample> ordered;
    for (auto &[path, sample] : samples)
        ordered.push_back(std::move(sample));
    result.bank.sab = iw8::sound::BuildSab(ordered, Lower(map));
    zt::info("iw3 sound: bank %s: %zu alias list(s), %zu sound(s), %zu-byte loaded SAB", result.bank.name.c_str(),
             result.bank.lists.size(), ordered.size(), result.bank.sab.size());
    // Diagnostic: IW8_ZONETOOL_SAB_DUMP=<file> keeps a copy for offline checks.
    if (const char *dump = std::getenv("IW8_ZONETOOL_SAB_DUMP"); dump && *dump)
    {
        std::ofstream file(dump, std::ios::binary);
        file.write(reinterpret_cast<const char *>(result.bank.sab.data()),
                   static_cast<std::streamsize>(result.bank.sab.size()));
        zt::info("iw3 sound: wrote SAB copy to %s", dump);
    }
    return result;
}
std::size_t ExtractIwdMembers(const std::filesystem::path &directory,
                              const std::vector<std::string> &prefixes,
                              const std::filesystem::path &destination)
{
    std::error_code error;
    if (!std::filesystem::is_directory(directory, error))
        return 0;
    std::vector<std::filesystem::path> archives;
    for (const auto &entry : std::filesystem::directory_iterator(directory, error))
        if (entry.is_regular_file() && Lower(entry.path().extension().string()) == ".iwd")
            archives.push_back(entry.path());
    // IW3 searches later archives first, so they extract first and win.
    std::sort(archives.rbegin(), archives.rend());
    std::size_t extracted = 0;
    for (const auto &archive : archives)
    {
        try
        {
            const Iwd iwd(archive);
            for (const auto &name : iwd.Names())
            {
                if (name.empty() || name.back() == '/' ||
                    std::ranges::none_of(prefixes, [&](const std::string &prefix) {
                        return name.starts_with(prefix);
                    }))
                    continue;
                // Archive member names are untrusted: keep each one inside
                // the destination.
                const std::filesystem::path relative(name);
                if (relative.is_absolute() || relative.has_root_name() ||
                    std::ranges::any_of(relative, [](const std::filesystem::path &part) {
                        return part == "..";
                    }))
                    continue;
                const auto target = destination / relative;
                if (std::filesystem::exists(target, error))
                    continue;
                std::filesystem::create_directories(target.parent_path(), error);
                const auto data = iwd.Read(name);
                std::ofstream file(target, std::ios::binary);
                file.write(reinterpret_cast<const char *>(data.data()),
                           static_cast<std::streamsize>(data.size()));
                if (file)
                    ++extracted;
            }
        }
        catch (const std::exception &failure)
        {
            zt::warn("iw3: skipped IWD %s: %s", archive.string().c_str(), failure.what());
        }
    }
    return extracted;
}
} // namespace iw3::sound
