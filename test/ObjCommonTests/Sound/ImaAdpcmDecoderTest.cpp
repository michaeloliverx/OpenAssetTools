#include "Sound/ImaAdpcmDecoder.h"

#include <catch2/catch_test_macros.hpp>

namespace ima_adpcm
{
    namespace
    {
        constexpr uint8_t MONO_BLOCK[]{0xe8, 0x03, 0, 0, 0x71, 0x8f, 0x24, 0x9b};
        constexpr uint8_t STEREO_BLOCK[]{0xe8, 0x03, 0, 0, 0x18, 0xfc, 0, 0, 0x71, 0x8f, 0x24, 0x9b, 0x17, 0xf8, 0x42, 0xb9};
    } // namespace

    // Expected PCM samples were checked against Miles 7's AIL_decompress_ADPCM.
    TEST_CASE("ImaAdpcmDecoder: Decodes mono samples in low nibble order", "[sound][ima-adpcm]")
    {
        std::vector<int16_t> samples;
        REQUIRE(Decode(MONO_BLOCK, 1, sizeof(MONO_BLOCK), 9, samples));
        REQUIRE(samples == std::vector<int16_t>{1000, 1001, 1012, 982, 978, 1012, 1034, 1005, 995});
    }

    TEST_CASE("ImaAdpcmDecoder: Interleaves stereo channel groups", "[sound][ima-adpcm]")
    {
        std::vector<int16_t> samples;
        REQUIRE(Decode(STEREO_BLOCK, 2, sizeof(STEREO_BLOCK), 9, samples));
        REQUIRE(samples == std::vector<int16_t>{1000, -1000, 1001, -989, 1012, -983, 982, -984, 978, -1007, 1012, -990, 1034, -962, 1005, -972, 995, -996});
    }

    TEST_CASE("ImaAdpcmDecoder: Resets channel state at each block", "[sound][ima-adpcm]")
    {
        std::vector<uint8_t> data(std::begin(MONO_BLOCK), std::end(MONO_BLOCK));
        data.insert(data.end(), std::begin(MONO_BLOCK), std::end(MONO_BLOCK));

        std::vector<int16_t> samples;
        REQUIRE(Decode(data, 1, sizeof(MONO_BLOCK), 12, samples));
        REQUIRE(samples == std::vector<int16_t>{1000, 1001, 1012, 982, 978, 1012, 1034, 1005, 995, 1000, 1001, 1012});
    }

    TEST_CASE("ImaAdpcmDecoder: Trims block padding to the requested frame count", "[sound][ima-adpcm]")
    {
        std::vector<int16_t> samples;
        REQUIRE(Decode(STEREO_BLOCK, 2, sizeof(STEREO_BLOCK), 3, samples));
        REQUIRE(samples == std::vector<int16_t>{1000, -1000, 1001, -989, 1012, -983});
    }

    TEST_CASE("ImaAdpcmDecoder: Clamps predictors and step indices", "[sound][ima-adpcm]")
    {
        constexpr uint8_t data[]{0xf8, 0x7f, 88, 0, 0x77, 0xff, 0, 0};
        std::vector<int16_t> samples;
        REQUIRE(Decode(data, 1, sizeof(data), 9, samples));
        REQUIRE(samples == std::vector<int16_t>{32760, 32767, 32767, -28669, -32768, -28673, -24949, -21564, -18487});
    }

    TEST_CASE("ImaAdpcmDecoder: Rejects malformed input", "[sound][ima-adpcm]")
    {
        auto channelCount = 1u;
        auto blockSize = static_cast<unsigned>(sizeof(MONO_BLOCK));
        auto frameCount = 9uz;
        std::vector<uint8_t> data(std::begin(MONO_BLOCK), std::end(MONO_BLOCK));
        SECTION("No channels")
        {
            channelCount = 0;
        }
        SECTION("Unsupported channel count")
        {
            channelCount = 3;
        }
        SECTION("No block size")
        {
            blockSize = 0;
        }
        SECTION("Block too small for the header")
        {
            blockSize = 2;
        }
        SECTION("Incomplete channel group")
        {
            blockSize = 7;
            data.resize(blockSize);
        }
        SECTION("Truncated block")
        {
            data.pop_back();
        }
        SECTION("Not enough frames")
        {
            frameCount = 10;
        }
        SECTION("Invalid initial step index")
        {
            data[2] = 89;
        }
        SECTION("Invalid later block")
        {
            data.insert(data.end(), std::begin(MONO_BLOCK), std::end(MONO_BLOCK));
            data[sizeof(MONO_BLOCK) + 2] = 89;
            frameCount = 18;
        }
        SECTION("Missing data")
        {
            data.clear();
        }

        std::vector<int16_t> samples{123};
        REQUIRE_FALSE(Decode(data, channelCount, blockSize, frameCount, samples));
        REQUIRE(samples.empty());
    }
} // namespace ima_adpcm
