#include "Game/IW3Xenon/Sound/SoundDumperIW3Xenon.h"

#include "Game/IW3Xenon/Sound/LoadedSoundDumperIW3Xenon.h"
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

using namespace IW3Xenon;

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

    TEST_CASE("LoadedSoundDumperIW3Xenon: Wraps embedded XMA packets in a legacy XMA1 RIFF", "[iw3xenon][sound][loaded-sound][assetdumper]")
    {
        const auto sampleType = GENERATE(3u, 4u);
        Zone zone("MockZone", 0, GameId::IW3Xenon, GamePlatform::XBOX);
        MockSearchPath mockObjPath;
        MockOutputPath mockOutput;
        AssetDumpingContext context(zone, "", mockOutput, mockObjPath, std::nullopt);

        constexpr auto XMA_DATA = "embedded XMA data";
        auto* loadedSound = zone.Memory().Alloc<LoadedSound>();
        loadedSound->name = zone.Memory().Dup("sfx/test_sound");
        loadedSound->sound.packet.pBuffer = zone.Memory().Dup(XMA_DATA);
        loadedSound->sound.packet.BufferSize = static_cast<unsigned int>(std::strlen(XMA_DATA));
        loadedSound->sound.packet.LoopCount = XMA_INFINITE_LOOP;
        loadedSound->sound.packet.XMALoop[0].LoopStart = 32u;
        loadedSound->sound.packet.XMALoop[0].LoopEnd = 256u;
        loadedSound->sound.packet.XMALoop[0].LoopSubframeEnd = 2u;
        loadedSound->sound.packet.XMALoop[0].LoopSubframeSkip = 3u;
        loadedSound->sound.format.SampleType = static_cast<uint8_t>(sampleType);
        loadedSound->sound.format.___u1.NumStreams = 1u;
        loadedSound->sound.format.___u2.Stream[0].SampleRate = endianness::ToBigEndian(44100u);
        loadedSound->sound.format.___u2.Stream[0].ChannelCount = 2u;
        loadedSound->sound.xaIwXmaDataInfo.totalMsec = 1000;
        const std::array seekTable{1u, 6u, 0u, 7168u};
        loadedSound->sound.seekTable.size = static_cast<int>(seekTable.size());
        loadedSound->sound.seekTable.data = zone.Memory().Alloc<unsigned int>(seekTable.size());
        std::copy(seekTable.begin(), seekTable.end(), loadedSound->sound.seekTable.data);
        zone.m_pools.AddAsset(std::make_unique<XAssetInfo<LoadedSound>>(ASSET_TYPE_LOADED_SOUND, loadedSound->name, loadedSound));

        sound::LoadedSoundDumperIW3Xenon dumper;
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

    TEST_CASE("LoadedSoundDumperIW3Xenon: Retains all XMA stream formats and pads odd packets", "[iw3xenon][sound][assetdumper]")
    {
        const auto streamCount = GENERATE(1u, 3u, 6u);
        Zone zone("MockZone", 0, GameId::IW3Xenon, GamePlatform::XBOX);
        MockSearchPath search;
        MockOutputPath output;
        AssetDumpingContext context(zone, "", output, search, std::nullopt);
        LoadedSound loaded{};
        loaded.name = "multi.wav";
        std::array<char, 3> data{'x', '\0', 'z'};
        loaded.sound.packet.pBuffer = data.data();
        loaded.sound.packet.BufferSize = static_cast<unsigned int>(data.size());
        loaded.sound.format.SampleType = 4u;
        loaded.sound.format.___u1.NumStreams = static_cast<uint8_t>(streamCount);
        for (auto i = 0u; i < streamCount; ++i)
        {
            loaded.sound.format.___u2.Stream[i].SampleRate = endianness::ToBigEndian(48000u - 1000u * i);
            loaded.sound.format.___u2.Stream[i].ChannelCount = static_cast<uint8_t>(1u + (i % 2u));
            loaded.sound.packet.XMALoop[i].LoopStart = 100u + i;
            loaded.sound.packet.XMALoop[i].LoopEnd = 200u + i;
        }
        zone.m_pools.AddAsset(std::make_unique<XAssetInfo<LoadedSound>>(ASSET_TYPE_LOADED_SOUND, loaded.name, &loaded));
        sound::LoadedSoundDumperIW3Xenon dumper;
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

    TEST_CASE("LoadedSoundDumperIW3Xenon: Rejects invalid metadata before opening output", "[iw3xenon][sound][assetdumper]")
    {
        Zone zone("MockZone", 0, GameId::IW3Xenon, GamePlatform::XBOX);
        MockSearchPath search;
        MockOutputPath output;
        AssetDumpingContext context(zone, "", output, search, std::nullopt);
        LoadedSound loaded{};
        loaded.name = "invalid";
        std::array<char, 1> data{'x'};
        loaded.sound.packet.pBuffer = data.data();
        loaded.sound.packet.BufferSize = static_cast<unsigned int>(data.size());
        loaded.sound.format.SampleType = 4u;
        loaded.sound.format.___u1.NumStreams = 1u;
        loaded.sound.format.___u2.Stream[0].SampleRate = endianness::ToBigEndian(48000u);
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
        sound::LoadedSoundDumperIW3Xenon dumper;
        dumper.Dump(context);
        CHECK(output.GetMockedFileList().empty());
    }

    TEST_CASE("SoundDumperIW3Xenon: Extracts packed XMA bytes from the auto-discovered packfile", "[iw3xenon][sound][assetdumper]")
    {
        Zone zone("MockZone", 0, GameId::IW3Xenon, GamePlatform::XBOX);
        MockSearchPath mockObjPath;
        MockOutputPath mockOutput;
        AssetDumpingContext context(zone, "", mockOutput, mockObjPath, std::nullopt);

        mockObjPath.AddFileData("packfile1.pak", "0123456789");

        auto* soundFile = zone.Memory().Alloc<SoundFile>();
        soundFile->type = SAT_STREAMED;
        soundFile->u.streamSnd.filename.fileIndex = 1u;
        soundFile->u.streamSnd.filename.info.packed.offset = 3u;
        soundFile->u.streamSnd.filename.info.packed.length = 5u;

        auto* aliases = zone.Memory().Alloc<snd_alias_t>();
        aliases->soundFile = soundFile;

        auto* sound = zone.Memory().Alloc<snd_alias_list_t>();
        sound->head = aliases;
        sound->count = 1;
        zone.m_pools.AddAsset(std::make_unique<XAssetInfo<snd_alias_list_t>>(ASSET_TYPE_SOUND, "test_alias", sound));

        sound::SoundDumperIW3Xenon dumper;
        dumper.Dump(context);

        const auto* file = mockOutput.GetMockedFile("sound/test_alias.0.xma");
        REQUIRE(file != nullptr);
        REQUIRE(file->AsString() == "34567");
    }

    TEST_CASE("SoundDumperIW3Xenon: Copies packed ranges across buffer boundaries", "[iw3xenon][sound][assetdumper]")
    {
        Zone zone("MockZone", 0, GameId::IW3Xenon, GamePlatform::XBOX);
        MockSearchPath search;
        MockOutputPath output;
        AssetDumpingContext context(zone, "", output, search, std::nullopt);
        std::string audio(64u * 1024u + 37u, '\0');
        for (auto i = 0uz; i < audio.size(); ++i)
            audio[i] = static_cast<char>(i % 256u);
        search.AddFileData("packfile2.pak", "prefix" + audio + "suffix");
        SoundFile file{};
        file.type = SAT_STREAMED;
        file.u.streamSnd.filename.fileIndex = 2u;
        file.u.streamSnd.filename.info.packed = {6u, static_cast<unsigned int>(audio.size())};
        snd_alias_t alias{};
        alias.soundFile = &file;
        snd_alias_list_t list{"stream", &alias, 1};
        zone.m_pools.AddAsset(std::make_unique<XAssetInfo<snd_alias_list_t>>(ASSET_TYPE_SOUND, list.aliasName, &list));
        sound::SoundDumperIW3Xenon dumper;
        dumper.Dump(context);
        const auto* result = output.GetMockedFile("sound/stream.0.xma");
        REQUIRE(result);
        CHECK(result->AsString() == audio);
    }

    TEST_CASE("SoundDumperIW3Xenon: Skips unavailable and invalid packed ranges", "[iw3xenon][sound][assetdumper]")
    {
        Zone zone("MockZone", 0, GameId::IW3Xenon, GamePlatform::XBOX);
        MockSearchPath search;
        MockOutputPath output;
        AssetDumpingContext context(zone, "", output, search, std::nullopt);
        SoundFile file{};
        file.type = SAT_STREAMED;
        file.u.streamSnd.filename.fileIndex = 1u;
        file.u.streamSnd.filename.info.packed = {3u, 5u};
        SECTION("Missing packfile") {}
        SECTION("Offset past EOF")
        {
            search.AddFileData("packfile1.pak", "1234");
            file.u.streamSnd.filename.info.packed.offset = 5u;
        }
        SECTION("Length past EOF")
        {
            search.AddFileData("packfile1.pak", "1234");
        }
        SECTION("Unsigned range overflow")
        {
            search.AddFileData("packfile1.pak", "1234");
            file.u.streamSnd.filename.info.packed.length = std::numeric_limits<unsigned int>::max();
        }
        snd_alias_t alias{};
        alias.soundFile = &file;
        snd_alias_list_t list{"invalid", &alias, 1};
        zone.m_pools.AddAsset(std::make_unique<XAssetInfo<snd_alias_list_t>>(ASSET_TYPE_SOUND, list.aliasName, &list));
        sound::SoundDumperIW3Xenon dumper;
        dumper.Dump(context);
        CHECK(output.GetMockedFileList().empty());
    }

    TEST_CASE("SoundDumperIW3Xenon: Finds loose XMA by its raw name", "[iw3xenon][sound][assetdumper]")
    {
        const auto fileIndex = GENERATE(0u, std::numeric_limits<unsigned int>::max());
        Zone zone("MockZone", 0, GameId::IW3Xenon, GamePlatform::XBOX);
        MockSearchPath search;
        MockOutputPath output;
        AssetDumpingContext context(zone, "", output, search, std::nullopt);
        search.AddFileData("voice/test.xma", "XMA bytes");
        SoundFile file{};
        file.type = SAT_STREAMED;
        file.u.streamSnd.filename.fileIndex = fileIndex;
        file.u.streamSnd.filename.info.raw = {"voice", "test"};
        snd_alias_t alias{};
        alias.soundFile = &file;
        snd_alias_list_t list{"loose", &alias, 1};
        zone.m_pools.AddAsset(std::make_unique<XAssetInfo<snd_alias_list_t>>(ASSET_TYPE_SOUND, list.aliasName, &list));
        sound::SoundDumperIW3Xenon dumper;
        dumper.Dump(context);
        const auto* result = output.GetMockedFile("sound/loose.0.xma");
        REQUIRE(result);
        CHECK(result->AsString() == "XMA bytes");
    }
} // namespace
