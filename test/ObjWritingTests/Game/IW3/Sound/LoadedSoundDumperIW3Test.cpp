#include "Game/IW3/Sound/LoadedSoundDumperIW3.h"

#include "SearchPath/MockOutputPath.h"
#include "SearchPath/MockSearchPath.h"
#include "Sound/WavTypes.h"

#include <catch2/catch_test_macros.hpp>
#include <cstring>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <vector>

using namespace IW3;

namespace
{
    constexpr uint8_t MONO_BLOCK[]{0xe8, 0x03, 0, 0, 0x71, 0x8f, 0x24, 0x9b};
    constexpr uint8_t STEREO_BLOCK[]{0xe8, 0x03, 0, 0, 0x18, 0xfc, 0, 0, 0x71, 0x8f, 0x24, 0x9b, 0x17, 0xf8, 0x42, 0xb9};

    std::optional<std::string> DumpSound(const AILSOUNDINFO& info, const std::span<const uint8_t> data)
    {
        Zone zone("MockZone", 0, GameId::IW3, GamePlatform::PC);
        MockSearchPath mockObjPath;
        MockOutputPath mockOutput;
        AssetDumpingContext context(zone, "", mockOutput, mockObjPath, std::nullopt);

        LoadedSound loadedSound{};
        loadedSound.name = "test.wav";
        loadedSound.sound.info = info;
        loadedSound.sound.data = reinterpret_cast<char*>(const_cast<uint8_t*>(data.data()));
        zone.m_pools.AddAsset(std::make_unique<XAssetInfo<LoadedSound>>(ASSET_TYPE_LOADED_SOUND, loadedSound.name, &loadedSound));

        sound::LoadedSoundDumperIW3 dumper;
        dumper.Dump(context);
        const auto* file = mockOutput.GetMockedFile("sound/test.wav");
        return file ? std::make_optional(file->AsString()) : std::nullopt;
    }

    TEST_CASE("LoadedSoundDumperIW3: Preserves PCM loaded sounds", "[iw3][loaded-sound][assetdumper]")
    {
        constexpr uint8_t data[]{0, 0, 0xe8, 0x03};
        AILSOUNDINFO info{};
        info.format = static_cast<int>(WavFormat::PCM);
        info.data_len = sizeof(data);
        info.rate = 8000;
        info.bits = 16;
        info.channels = 1;

        const auto result = DumpSound(info, data);
        REQUIRE(result.has_value());
        REQUIRE(result->size() == sizeof(WavHeader) + sizeof(data));
        REQUIRE(std::memcmp(result->data() + sizeof(WavHeader), data, sizeof(data)) == 0);
    }

    TEST_CASE("LoadedSoundDumperIW3: Dumps IMA ADPCM as PCM16 WAV", "[iw3][loaded-sound][assetdumper][ima-adpcm]")
    {
        auto data = std::span<const uint8_t>(MONO_BLOCK);
        auto channelCount = 1;
        std::vector<int16_t> expected{1000, 1001, 1012};
        SECTION("Mono") {}
        SECTION("Stereo")
        {
            data = STEREO_BLOCK;
            channelCount = 2;
            expected = {1000, -1000, 1001, -989, 1012, -983};
        }

        AILSOUNDINFO info{};
        info.format = static_cast<int>(WavFormat::IMA_ADPCM);
        info.data_len = static_cast<unsigned>(data.size());
        info.rate = 44100;
        info.bits = 4;
        info.channels = channelCount;
        info.samples = 3;
        info.block_size = static_cast<unsigned>(data.size());

        const auto result = DumpSound(info, data);
        REQUIRE(result.has_value());
        REQUIRE(result->size() == sizeof(WavHeader) + expected.size() * sizeof(int16_t));
        WavHeader header{};
        std::memcpy(&header, result->data(), sizeof(header));
        REQUIRE(header.formatChunk.wFormatTag == WavFormat::PCM);
        REQUIRE(header.formatChunk.nChannels == channelCount);
        REQUIRE(header.formatChunk.nSamplesPerSec == 44100);
        REQUIRE(header.formatChunk.nAvgBytesPerSec == 44100 * channelCount * 2);
        REQUIRE(header.formatChunk.wBitsPerSample == 16);
        REQUIRE(header.formatChunk.nBlockAlign == channelCount * 2);
        REQUIRE(header.subChunkHeader.chunkSize == expected.size() * sizeof(int16_t));
        REQUIRE(std::memcmp(result->data() + sizeof(WavHeader), expected.data(), expected.size() * sizeof(int16_t)) == 0);
    }

    TEST_CASE("LoadedSoundDumperIW3: Does not create a file for invalid sound data", "[iw3][loaded-sound][assetdumper][ima-adpcm]")
    {
        AILSOUNDINFO info{};
        info.format = static_cast<int>(WavFormat::IMA_ADPCM);
        info.data_len = sizeof(MONO_BLOCK);
        info.rate = 8000;
        info.bits = 4;
        info.channels = 1;
        info.samples = 9;
        info.block_size = sizeof(MONO_BLOCK);
        auto data = std::span<const uint8_t>(MONO_BLOCK);
        SECTION("Unsupported format")
        {
            info.format = 99;
        }
        SECTION("Wrong bits per sample")
        {
            info.bits = 8;
        }
        SECTION("Invalid block size")
        {
            info.block_size = 0;
        }
        SECTION("Missing data")
        {
            data = {};
        }
        REQUIRE_FALSE(DumpSound(info, data).has_value());
    }
} // namespace
