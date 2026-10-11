#include "Game/IW3Xenon/IW3Xenon.h"
#include "Utils/Endianness.h"
#include "ZoneLoading.h"

#include <algorithm>
#include <array>
#include <catch2/catch_test_macros.hpp>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#ifdef ARCH_x86

namespace
{
    using RiffChunks = std::map<std::string, std::pair<size_t, size_t>>;

    uint32_t ReadLittleEndian(const std::vector<char>& bytes, const size_t offset, const size_t size = 4)
    {
        REQUIRE(offset + size <= bytes.size());
        uint32_t result = 0;
        for (auto i = 0uz; i < size; ++i)
            result |= static_cast<uint32_t>(static_cast<uint8_t>(bytes[offset + i])) << (8u * i);
        return result;
    }

    std::vector<char> ReadFile(const std::filesystem::path& path)
    {
        INFO(path.string());
        std::ifstream stream(path, std::ios::binary);
        REQUIRE(stream);
        return {std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>()};
    }

    RiffChunks ReadRiffChunks(const std::vector<char>& bytes, const bool exactSize = true)
    {
        REQUIRE(bytes.size() >= 12);
        REQUIRE(std::string(bytes.data(), 4) == "RIFF");
        REQUIRE(std::string(bytes.data() + 8, 4) == "WAVE");
        if (exactSize)
            REQUIRE(ReadLittleEndian(bytes, 4) + 8u == bytes.size());
        else
            REQUIRE(ReadLittleEndian(bytes, 4) + 8u <= bytes.size());
        RiffChunks chunks;
        for (auto offset = 12uz; offset < bytes.size();)
        {
            REQUIRE(offset + 8u <= bytes.size());
            const auto length = ReadLittleEndian(bytes, offset + 4u);
            REQUIRE(length <= bytes.size() - offset - 8u);
            const auto id = std::string(bytes.data() + offset, 4);
            REQUIRE(chunks.emplace(id, std::pair{offset + 8u, static_cast<size_t>(length)}).second);
            offset += 8u + length + (length & 1u);
            REQUIRE(offset <= bytes.size());
        }
        return chunks;
    }
} // namespace

TEST_CASE("IW3 Xenon dumped audio retains native packets and packed byte ranges", "[.iw3xenon-sound-files]")
{
    const auto* sourcePath = std::getenv("OAT_SOUND_FASTFILE");
    const auto* dumpPath = std::getenv("OAT_SOUND_DUMP");
    REQUIRE(sourcePath);
    REQUIRE(dumpPath);
    const auto loaded = ZoneLoading::LoadZone(sourcePath, std::nullopt);
    REQUIRE(loaded);
    const auto& zone = **loaded;
    const auto sourceRoot = std::filesystem::path(sourcePath).parent_path();
    const auto dumpRoot = std::filesystem::path(dumpPath) / "sound";
    nlohmann::json report;
    report["sampleRates"] = nlohmann::json::object();
    report["sampleTypes"] = nlohmann::json::object();
    size_t embeddedCount = 0;
    size_t packedCount = 0;
    uint64_t packedBytes = 0;
    std::map<unsigned int, std::ifstream> packfiles;
    for (const auto* info : zone.m_pools.PoolAssets<IW3Xenon::AssetLoadedSound>())
    {
        if (info->IsReference())
            continue;
        INFO(info->m_name);
        const auto& sound = info->Asset()->sound;
        auto name = std::filesystem::path(info->m_name);
        name.replace_extension(".xma");
        const auto bytes = ReadFile(dumpRoot / name);
        const auto chunks = ReadRiffChunks(bytes);
        REQUIRE(chunks.contains("fmt "));
        REQUIRE(chunks.contains("IWXI"));
        REQUIRE(chunks.contains("seek"));
        REQUIRE(chunks.contains("data"));
        const auto [formatOffset, formatSize] = chunks.at("fmt ");
        REQUIRE(formatSize == 12u + 20u * sound.format.___u1.NumStreams);
        CHECK(ReadLittleEndian(bytes, formatOffset, 2) == 0x165u);
        CHECK(ReadLittleEndian(bytes, formatOffset + 2u, 2) == 16u);
        CHECK(ReadLittleEndian(bytes, formatOffset + 8u, 2) == sound.format.___u1.NumStreams);
        CHECK(static_cast<uint8_t>(bytes[formatOffset + 10u]) == static_cast<uint8_t>(sound.packet.LoopCount));
        for (auto i = 0u; i < sound.format.___u1.NumStreams; ++i)
        {
            const auto streamOffset = formatOffset + 12u + 20u * i;
            const auto& stream = sound.format.___u2.Stream[i];
            const auto& loop = sound.packet.XMALoop[i];
            const auto rate = endianness::FromBigEndian(stream.SampleRate);
            CHECK(ReadLittleEndian(bytes, streamOffset + 4u) == rate);
            CHECK(ReadLittleEndian(bytes, streamOffset + 8u) == loop.LoopStart);
            CHECK(ReadLittleEndian(bytes, streamOffset + 12u) == loop.LoopEnd);
            CHECK(static_cast<uint8_t>(bytes[streamOffset + 16u]) == (loop.LoopSubframeEnd << 4u | loop.LoopSubframeSkip));
            CHECK(static_cast<uint8_t>(bytes[streamOffset + 17u]) == stream.ChannelCount);
            report["sampleRates"][std::to_string(rate)] = report["sampleRates"].value(std::to_string(rate), 0u) + 1u;
        }
        CHECK(chunks.at("IWXI").second == sizeof(uint32_t));
        CHECK(ReadLittleEndian(bytes, chunks.at("IWXI").first) == static_cast<uint32_t>(sound.xaIwXmaDataInfo.totalMsec));
        REQUIRE(chunks.at("seek").second == sizeof(uint32_t) * sound.seekTable.size);
        for (auto i = 0; i < sound.seekTable.size; ++i)
            CHECK(ReadLittleEndian(bytes, chunks.at("seek").first + sizeof(uint32_t) * i) == sound.seekTable.data[i]);
        REQUIRE(chunks.at("data").second == sound.packet.BufferSize);
        CHECK(std::memcmp(bytes.data() + chunks.at("data").first, sound.packet.pBuffer, sound.packet.BufferSize) == 0);
        report["sampleTypes"][std::to_string(sound.format.SampleType)] = report["sampleTypes"].value(std::to_string(sound.format.SampleType), 0u) + 1u;
        ++embeddedCount;
    }
    for (const auto* info : zone.m_pools.PoolAssets<IW3Xenon::AssetSound>())
    {
        if (info->IsReference())
            continue;
        for (int i = 0; i < info->Asset()->count; ++i)
        {
            INFO(info->m_name << "." << i);
            const auto* soundFile = info->Asset()->head[i].soundFile;
            if (!soundFile || soundFile->type != IW3Xenon::SAT_STREAMED)
                continue;
            const auto& filename = soundFile->u.streamSnd.filename;
            if (filename.fileIndex == 0u || filename.fileIndex == 0xffffffffu)
                continue;
            const auto outputName = info->m_name + "." + std::to_string(i) + ".xma";
            const auto bytes = ReadFile(dumpRoot / outputName);
            REQUIRE(bytes.size() == filename.info.packed.length);
            // Some stock packed RIFF headers under-report the file size by eight bytes.
            // Extraction preserves those original headers and the entire recorded byte range.
            ReadRiffChunks(bytes, false);
            auto& packfile = packfiles[filename.fileIndex];
            if (!packfile.is_open())
                packfile.open(sourceRoot / ("packfile" + std::to_string(filename.fileIndex) + ".pak"), std::ios::binary);
            REQUIRE(packfile);
            packfile.seekg(filename.info.packed.offset);
            std::array<char, 65536> sourceBytes;
            for (auto position = 0uz; position < bytes.size();)
            {
                const auto count = std::min(sourceBytes.size(), bytes.size() - position);
                packfile.read(sourceBytes.data(), count);
                REQUIRE(packfile.gcount() == count);
                CHECK(std::memcmp(sourceBytes.data(), bytes.data() + position, count) == 0);
                position += count;
            }
            report["packedRanges"][outputName] = {filename.fileIndex, filename.info.packed.offset, filename.info.packed.length};
            packedBytes += bytes.size();
            ++packedCount;
        }
    }
    report["embeddedSoundsChecked"] = embeddedCount;
    report["packedSoundsChecked"] = packedCount;
    report["packedBytesChecked"] = packedBytes;
    if (const auto* path = std::getenv("OAT_SOUND_REPORT"))
        std::ofstream(path) << report.dump(2);
    CHECK(embeddedCount > 0u);
    CHECK(packedCount > 0u);
}

#endif
