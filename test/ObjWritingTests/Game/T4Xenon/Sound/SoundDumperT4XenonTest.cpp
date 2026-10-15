#include "Game/T4Xenon/Sound/SoundDumperT4Xenon.h"

#include "Game/T4Xenon/Sound/LoadedSoundDumperT4Xenon.h"
#include "SearchPath/MockOutputPath.h"
#include "SearchPath/MockSearchPath.h"
#include "Utils/Endianness.h"

#include <algorithm>
#include <array>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <memory>
#include <string>

using namespace T4Xenon;

namespace
{
    // Name and value from the Xbox 360 SDK's xma2defs.h.
    constexpr auto XMA_INFINITE_LOOP = std::uint8_t{255};

    std::uint16_t ReadU16Le(const MockOutputFile& file, const std::size_t offset)
    {
        return static_cast<std::uint16_t>(file.m_data[offset]) | static_cast<std::uint16_t>(file.m_data[offset + 1u]) << 8u;
    }

    std::uint32_t ReadU32Le(const MockOutputFile& file, const std::size_t offset)
    {
        return static_cast<std::uint32_t>(file.m_data[offset]) | static_cast<std::uint32_t>(file.m_data[offset + 1u]) << 8u
               | static_cast<std::uint32_t>(file.m_data[offset + 2u]) << 16u | static_cast<std::uint32_t>(file.m_data[offset + 3u]) << 24u;
    }

    std::string ReadFourCc(const MockOutputFile& file, const std::size_t offset)
    {
        return {reinterpret_cast<const char*>(file.m_data.data() + offset), 4u};
    }

    TEST_CASE("LoadedSoundDumperT4Xenon: Wraps embedded XMA packets in a legacy XMA1 RIFF", "[t4xenon][sound][loaded-sound][assetdumper]")
    {
        const auto sampleType = 3u;
        Zone zone("MockZone", 0, GameId::T4Xenon, GamePlatform::XBOX);
        MockSearchPath mockObjPath;
        MockOutputPath mockOutput;
        AssetDumpingContext context(zone, "", mockOutput, mockObjPath, std::nullopt);

        constexpr auto XMA_DATA = "embedded XMA data";
        auto* loadedSound = zone.Memory().Alloc<LoadedSound>();
        loadedSound->name = zone.Memory().Dup("sfx/test_sound");
        loadedSound->sound.packet.pBuffer = zone.Memory().Dup(XMA_DATA);
        loadedSound->sound.packet.BufferSize = static_cast<unsigned int>(std::strlen(XMA_DATA));
        loadedSound->sound.packet.LoopCount = GENERATE(static_cast<unsigned int>(XMA_INFINITE_LOOP), std::numeric_limits<unsigned int>::max());
        loadedSound->sound.packet.XMALoop[0].LoopStart = 32u;
        loadedSound->sound.packet.XMALoop[0].LoopEnd = 256u;
        loadedSound->sound.packet.XMALoop[0].LoopSubframeEnd = 2u;
        loadedSound->sound.packet.XMALoop[0].LoopSubframeSkip = 3u;
        loadedSound->sound.format.SampleType = static_cast<uint8_t>(sampleType);
        loadedSound->sound.format.___u1.NumStreams = 1u;
        loadedSound->sound.format.___u2.Stream[0].SampleRate = 44100u;
        loadedSound->sound.format.___u2.Stream[0].ChannelCount = 2u;
        loadedSound->sound.xaIwXmaDataInfo.totalMsec = 1000;
        const std::array seekTable{1u, 6u, 0u, 7168u};
        loadedSound->sound.seekTable.size = static_cast<int>(seekTable.size());
        loadedSound->sound.seekTable.data = zone.Memory().Alloc<unsigned int>(seekTable.size());
        std::copy(seekTable.begin(), seekTable.end(), loadedSound->sound.seekTable.data);
        zone.m_pools.AddAsset(std::make_unique<XAssetInfo<LoadedSound>>(ASSET_TYPE_LOADED_SOUND, loadedSound->name, loadedSound));

        sound::LoadedSoundDumperT4Xenon dumper;
        dumper.Dump(context);

        const auto* file = mockOutput.GetMockedFile("sound/sfx/test_sound.xma");
        REQUIRE(file != nullptr);
        REQUIRE(ReadFourCc(*file, 0u) == "RIFF");
        REQUIRE(ReadU32Le(*file, 4u) == file->m_data.size() - 8u);
        REQUIRE(ReadFourCc(*file, 8u) == "WAVE");

        REQUIRE(ReadFourCc(*file, 12u) == "fmt ");
        REQUIRE(ReadU32Le(*file, 16u) == 32u);
        REQUIRE(ReadU16Le(*file, 20u) == 0x0165u);
        REQUIRE(ReadU16Le(*file, 22u) == 16u);
        REQUIRE(ReadU16Le(*file, 28u) == 1u);
        REQUIRE(file->m_data[30u] == XMA_INFINITE_LOOP);
        REQUIRE(file->m_data[31u] == 2u);
        REQUIRE(ReadU32Le(*file, 32u) == std::strlen(XMA_DATA));
        REQUIRE(ReadU32Le(*file, 36u) == 44100u);
        REQUIRE(ReadU32Le(*file, 40u) == 32u);
        REQUIRE(ReadU32Le(*file, 44u) == 256u);
        REQUIRE(file->m_data[48u] == 0x23u);
        REQUIRE(file->m_data[49u] == 2u);
        REQUIRE(ReadU16Le(*file, 50u) == 0u);

        REQUIRE(ReadFourCc(*file, 52u) == "IWXI");
        REQUIRE(ReadU32Le(*file, 56u) == 4u);
        REQUIRE(ReadU32Le(*file, 60u) == 1000u);
        REQUIRE(ReadFourCc(*file, 64u) == "seek");
        REQUIRE(ReadU32Le(*file, 68u) == seekTable.size() * sizeof(unsigned int));
        REQUIRE(ReadU32Le(*file, 72u) == seekTable[0u]);
        REQUIRE(ReadU32Le(*file, 76u) == seekTable[1u]);
        REQUIRE(ReadFourCc(*file, 88u) == "data");
        REQUIRE(ReadU32Le(*file, 92u) == std::strlen(XMA_DATA));
        REQUIRE(std::string(reinterpret_cast<const char*>(file->m_data.data() + 96u), std::strlen(XMA_DATA)) == XMA_DATA);
    }

    TEST_CASE("LoadedSoundDumperT4Xenon: Retains all XMA stream formats and pads odd packets", "[t4xenon][sound][assetdumper]")
    {
        const auto streamCount = GENERATE(1u, 3u, 6u);
        Zone zone("MockZone", 0, GameId::T4Xenon, GamePlatform::XBOX);
        MockSearchPath search;
        MockOutputPath output;
        AssetDumpingContext context(zone, "", output, search, std::nullopt);
        LoadedSound loaded{};
        loaded.name = "multi.wav";
        std::array<char, 3> data{'x', '\0', 'z'};
        loaded.sound.packet.pBuffer = data.data();
        loaded.sound.packet.BufferSize = static_cast<unsigned int>(data.size());
        loaded.sound.format.SampleType = 3u;
        loaded.sound.format.___u1.NumStreams = static_cast<uint8_t>(streamCount);
        for (auto i = 0u; i < streamCount; ++i)
        {
            loaded.sound.format.___u2.Stream[i].SampleRate = 48000u - 1000u * i;
            loaded.sound.format.___u2.Stream[i].ChannelCount = static_cast<uint8_t>(1u + (i % 2u));
            loaded.sound.packet.XMALoop[i].LoopStart = 100u + i;
            loaded.sound.packet.XMALoop[i].LoopEnd = 200u + i;
        }
        zone.m_pools.AddAsset(std::make_unique<XAssetInfo<LoadedSound>>(ASSET_TYPE_LOADED_SOUND, loaded.name, &loaded));
        sound::LoadedSoundDumperT4Xenon dumper;
        dumper.Dump(context);
        const auto* file = output.GetMockedFile("sound/multi.xma");
        REQUIRE(file);
        CHECK(ReadU32Le(*file, 4) == file->m_data.size() - 8u);
        CHECK(ReadU32Le(*file, 16) == 12u + 20u * streamCount);
        CHECK(ReadU16Le(*file, 28) == streamCount);
        for (auto i = 0u; i < streamCount; ++i)
        {
            CHECK(ReadU32Le(*file, 36u + 20u * i) == 48000u - 1000u * i);
            CHECK(ReadU32Le(*file, 40u + 20u * i) == 100u + i);
            CHECK(ReadU32Le(*file, 44u + 20u * i) == 200u + i);
            CHECK(file->m_data[49u + 20u * i] == 1u + (i % 2u));
        }
        const auto dataChunk = 52u + 20u * streamCount;
        CHECK(ReadFourCc(*file, dataChunk) == "data");
        CHECK(ReadU32Le(*file, dataChunk + 4u) == data.size());
        CHECK(std::memcmp(file->m_data.data() + dataChunk + 8u, data.data(), data.size()) == 0);
        CHECK(file->m_data.back() == 0u);
    }

    TEST_CASE("LoadedSoundDumperT4Xenon: Rejects invalid metadata before opening output", "[t4xenon][sound][assetdumper]")
    {
        Zone zone("MockZone", 0, GameId::T4Xenon, GamePlatform::XBOX);
        MockSearchPath search;
        MockOutputPath output;
        AssetDumpingContext context(zone, "", output, search, std::nullopt);
        LoadedSound loaded{};
        loaded.name = "invalid";
        std::array<char, 1> data{'x'};
        loaded.sound.packet.pBuffer = data.data();
        loaded.sound.packet.BufferSize = static_cast<unsigned int>(data.size());
        loaded.sound.format.SampleType = 3u;
        loaded.sound.format.___u1.NumStreams = 1u;
        loaded.sound.format.___u2.Stream[0].SampleRate = 48000u;
        loaded.sound.format.___u2.Stream[0].ChannelCount = 1u;
        SECTION("Unsupported sample type")
        {
            loaded.sound.format.SampleType = 1u;
        }
        SECTION("Too many streams")
        {
            loaded.sound.format.___u1.NumStreams = 7u;
        }
        SECTION("No streams")
        {
            loaded.sound.format.___u1.NumStreams = 0u;
        }
        SECTION("Invalid channel count")
        {
            loaded.sound.format.___u2.Stream[0].ChannelCount = 3u;
        }
        SECTION("Missing seek data")
        {
            loaded.sound.seekTable.size = 1;
        }
        SECTION("Negative seek size")
        {
            loaded.sound.seekTable.size = -1;
        }
        SECTION("RIFF size overflow")
        {
            loaded.sound.packet.BufferSize = std::numeric_limits<unsigned int>::max();
        }
        zone.m_pools.AddAsset(std::make_unique<XAssetInfo<LoadedSound>>(ASSET_TYPE_LOADED_SOUND, loaded.name, &loaded));
        sound::LoadedSoundDumperT4Xenon dumper;
        dumper.Dump(context);
        CHECK(output.GetMockedFileList().empty());
    }

    TEST_CASE("SoundDumperT4Xenon: Finds loose streams or resolves PackIndex ranges", "[t4xenon][sound][assetdumper]")
    {
        Zone zone("MockZone", 0, GameId::T4Xenon, GamePlatform::XBOX);
        MockSearchPath search;
        MockOutputPath output;
        AssetDumpingContext context(zone, "", output, search, std::nullopt);
        SoundFile file{};
        file.type = SAT_STREAMED;
        file.u.streamSnd.filename = {0u, "Voice", "test"};
        snd_alias_t alias{};
        alias.soundFile = &file;
        snd_alias_list_t list{"stream", &alias, 1};
        zone.m_pools.AddAsset(std::make_unique<XAssetInfo<snd_alias_list_t>>(ASSET_TYPE_SOUND, list.aliasName, &list));

        std::string audio(64u * 1024u + 37u, '\0');
        for (auto i = 0uz; i < audio.size(); ++i)
            audio[i] = static_cast<char>(i % 256u);
        constexpr auto PATH_HASH = 0x04084774u; // T4 hash of "voice/test", without an extension.
        std::array<PackIndexEntry, 3> entries{
            {{PATH_HASH - 1u, 0u, 1u}, {PATH_HASH, 3u, static_cast<unsigned int>(audio.size())}, {PATH_HASH + 1u, 0u, 1u}}
        };
        PackIndex index{
            "sounds.pak", {0x4B434150u, 1234u, 3u, 2048u, 4096u},
             entries.data()
        };
        zone.m_pools.AddAsset(std::make_unique<XAssetInfo<PackIndex>>(ASSET_TYPE_PACKINDEX, index.name, &index));
        const auto offset = index.header.data_start + entries[1].offset * index.header.alignment;
        std::string pak(offset, '\0');
        const std::array header{index.header.magic, index.header.timestamp, index.header.count, index.header.alignment, index.header.data_start};
        for (auto i = 0uz; i < header.size(); ++i)
        {
            const auto value = endianness::ToLittleEndian(header[i]);
            std::memcpy(pak.data() + i * sizeof(value), &value, sizeof(value));
        }
        pak += audio;
        pak += "trailing pack data";

        bool expectOutput = true;
        bool addPack = true;
        auto expected = audio;
        SECTION("Packed stream") {}
        SECTION("Backslash path")
        {
            file.u.streamSnd.filename.dir = "";
            file.u.streamSnd.filename.name = "Voice\\test";
        }
        SECTION("Loose file takes priority")
        {
            expected = "SDNS loose audio";
            search.AddFileData("Voice/test.xma", expected);
        }
        SECTION("No PackIndex is needed for loose files")
        {
            expected = "SDNS loose audio";
            search.AddFileData("Voice/test.xma", expected);
            addPack = false;
            index.header.count = 0u;
        }
        SECTION("Missing pack")
        {
            addPack = false;
            expectOutput = false;
        }
        SECTION("Mismatched pack header")
        {
            pak[4] ^= 1;
            expectOutput = false;
        }
        SECTION("Truncated pack header")
        {
            pak.resize(10u);
            expectOutput = false;
        }
        SECTION("Offset past EOF")
        {
            entries[1].offset = std::numeric_limits<unsigned int>::max();
            expectOutput = false;
        }
        SECTION("Length past EOF")
        {
            entries[1].size = std::numeric_limits<unsigned int>::max();
            expectOutput = false;
        }
        SECTION("Unmatched path")
        {
            file.u.streamSnd.filename.name = "other";
            expectOutput = false;
        }
        if (addPack)
            search.AddFileData(index.name, pak);

        sound::SoundDumperT4Xenon dumper;
        dumper.Dump(context);
        const auto* result = output.GetMockedFile("sound/stream.0.xma");
        if (expectOutput)
        {
            REQUIRE(result);
            CHECK(result->AsString() == expected);
        }
        else
            CHECK(output.GetMockedFileList().empty());
    }
} // namespace
