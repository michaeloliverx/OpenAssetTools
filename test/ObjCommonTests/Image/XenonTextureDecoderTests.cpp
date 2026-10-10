#include "Image/XenonTextureDecoder.h"

#include <algorithm>
#include <array>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <cstring>

namespace test::image::xenon
{
    namespace
    {
        using namespace ::image;
        using namespace ::image::xenon;

        std::array<std::uint8_t, 52> MakeHeader(std::uint32_t format, std::uint32_t endian, bool tiled = false, bool packedMips = false)
        {
            std::array<std::uint32_t, 13> words{};
            words[7] = 2u | (1u << 22u) | (tiled ? 1u << 31u : 0u);
            words[8] = format | (endian << 6u);
            words[12] = (GPUDIMENSION_2D << 9u) | (packedMips ? 1u << 11u : 0u);

            std::array<std::uint8_t, 52> header{};
            for (auto word = 0u; word < words.size(); ++word)
                for (auto byte = 0u; byte < 4u; ++byte)
                    header[word * 4u + byte] = static_cast<std::uint8_t>(words[word] >> (byte * 8u));
            return header;
        }

        XenonTextureLoadConfig MakeConfig(const std::array<std::uint8_t, 52>& header,
                                          const std::vector<std::uint8_t>& pixels,
                                          std::uint32_t width,
                                          std::uint32_t height,
                                          std::uint32_t levels = 1u,
                                          TextureType type = TextureType::T_2D)
        {
            return {header.data(), header.size(), pixels.data(), pixels.size(), width, height, 1u, levels, 0u, type};
        }
    } // namespace

    TEST_CASE("Xenon GPU endian modes preserve trailing bytes", "[image][xenon]")
    {
        constexpr std::array<std::uint8_t, 9> input{0, 1, 2, 3, 4, 5, 6, 7, 8};
        constexpr std::array<std::array<std::uint8_t, 9>, 4> expected{
            {
             {0, 1, 2, 3, 4, 5, 6, 7, 8},
             {1, 0, 3, 2, 5, 4, 7, 6, 8},
             {3, 2, 1, 0, 7, 6, 5, 4, 8},
             {2, 3, 0, 1, 6, 7, 4, 5, 8},
             }
        };
        for (auto mode = 0u; mode < expected.size(); ++mode)
        {
            auto data = input;
            ApplyGpuEndian(mode, data.data(), data.size());
            REQUIRE(data == expected[mode]);
        }
    }

    TEST_CASE("Xenon texture headers use little-endian GPU words", "[image][xenon]")
    {
        const auto bytes = MakeHeader(GPUTEXTUREFORMAT_DXT1, GPUENDIAN_8IN16, true, true);
        XenonTextureHeader header{};
        REQUIRE(DecodeTextureHeader(bytes.data(), bytes.size(), header));
        REQUIRE(header.fetch.dataFormat == GPUTEXTUREFORMAT_DXT1);
        REQUIRE(header.fetch.endian == GPUENDIAN_8IN16);
        REQUIRE(header.fetch.pitch == 1u);
        REQUIRE(header.fetch.tiled == 1u);
        REQUIRE(header.fetch.packedMips == 1u);
        REQUIRE(header.fetch.dimension == GPUDIMENSION_2D);
        REQUIRE_FALSE(DecodeTextureHeader(bytes.data(), bytes.size() - 1u, header));
    }

    TEST_CASE("Xenon texture header serialization preserves resource and reserved bits", "[image][xenon]")
    {
        using namespace ::image::xenon;
        const auto dimension = GENERATE(GPUDIMENSION_1D, GPUDIMENSION_2D, GPUDIMENSION_3D, GPUDIMENSION_CUBEMAP);
        const auto stacked = GENERATE(false, true);
        const auto fill = GENERATE(0xffu, 0x73u);
        std::array<std::uint8_t, XENON_TEXTURE_HEADER_SIZE> bytes;
        bytes.fill(static_cast<std::uint8_t>(fill));
        bytes[49] = static_cast<std::uint8_t>((bytes[49] & ~0x06u) | (dimension << 1u));
        bytes[33] = static_cast<std::uint8_t>((bytes[33] & ~0x04u) | (stacked ? 0x04u : 0u));
        XenonTextureHeader header;
        REQUIRE(DecodeTextureHeader(bytes.data(), bytes.size(), header));
        REQUIRE(EncodeTextureHeader(header) == bytes);
        if (fill == 0xffu)
        {
            REQUIRE(header.fetch.expAdjust == -1);
            REQUIRE(header.fetch.lodBias == -1);
            REQUIRE(header.fetch.gradExpAdjustH == -1);
            REQUIRE(header.fetch.gradExpAdjustV == -1);
            REQUIRE(header.fetch.anisoBias == -1);
        }
    }

    TEST_CASE("Xenon tiled BC1 blocks decode to their original row order", "[image][xenon]")
    {
        // The first four blocks in each of two Xenos tile rows, at eight bytes per block.
        constexpr std::array<std::size_t, 8> offsets{0, 8, 32, 40, 16, 24, 48, 56};
        std::vector<std::uint8_t> pixels(4096);
        std::array<std::uint8_t, 64> expected{};
        for (auto block = 0u; block < offsets.size(); ++block)
            for (auto byte = 0u; byte < 8u; ++byte)
            {
                const auto value = static_cast<std::uint8_t>(block * 16u + byte);
                expected[block * 8u + byte] = value;
                pixels[offsets[block] + (byte ^ 1u)] = value;
            }

        const auto header = MakeHeader(GPUTEXTUREFORMAT_DXT1, GPUENDIAN_8IN16, true);
        const auto texture = LoadTexture(MakeConfig(header, pixels, 16, 8));
        REQUIRE(texture);
        REQUIRE(texture->GetFormat()->GetId() == ImageFormatId::BC1);
        REQUIRE(texture->GetSizeOfMipLevel(0) == expected.size());
        REQUIRE(std::memcmp(texture->GetBufferForMipLevel(0), expected.data(), expected.size()) == 0);
    }

    TEST_CASE("Xenon linear images remove pitch padding and GPU byte order", "[image][xenon]")
    {
        std::vector<std::uint8_t> pixels(4096);
        constexpr std::array<std::uint8_t, 16> expected{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
        for (auto row = 0u; row < 2u; ++row)
            for (auto byte = 0u; byte < 8u; ++byte)
                pixels[row * 128u + (byte ^ 3u)] = expected[row * 8u + byte];

        const auto header = MakeHeader(GPUTEXTUREFORMAT_8_8_8_8, GPUENDIAN_8IN32);
        const auto texture = LoadTexture(MakeConfig(header, pixels, 2, 2));
        REQUIRE(texture);
        REQUIRE(texture->GetFormat()->GetId() == ImageFormatId::B8_G8_R8_A8);
        REQUIRE(std::memcmp(texture->GetBufferForMipLevel(0), expected.data(), expected.size()) == 0);
    }

    TEST_CASE("Xenon packed mip tails preserve each level", "[image][xenon][mip]")
    {
        // Packed positions for an 8x8 one-byte surface: 8x8, 4x4, 2x2 and 1x1.
        constexpr std::array<std::array<unsigned, 2>, 4> origins{
            {{16, 0}, {8, 0}, {4, 0}, {0, 4}}
        };
        std::vector<std::uint8_t> pixels(4096);
        for (auto level = 0u; level < origins.size(); ++level)
        {
            const auto dimension = 8u >> level;
            for (auto y = 0u; y < dimension; ++y)
                for (auto x = 0u; x < dimension; ++x)
                    pixels[(origins[level][1] + y) * 32u + origins[level][0] + x] = static_cast<std::uint8_t>(0x20u + level);
        }

        const auto header = MakeHeader(GPUTEXTUREFORMAT_8, GPUENDIAN_NONE, false, true);
        const auto texture = LoadTexture(MakeConfig(header, pixels, 8, 8, 4));
        REQUIRE(texture);
        REQUIRE(texture->HasMipMaps());
        for (auto level = 0; level < 4; ++level)
        {
            const auto* data = texture->GetBufferForMipLevel(level);
            REQUIRE(std::all_of(data,
                                data + texture->GetSizeOfMipLevel(level),
                                [level](auto value)
                                {
                                    return value == 0x20u + level;
                                }));
        }
    }

    TEST_CASE("Xenon cubemap faces use separate aligned surfaces", "[image][xenon][cube]")
    {
        std::vector<std::uint8_t> pixels(6u * 4096u);
        for (auto face = 0u; face < 6u; ++face)
            pixels[face * 4096u] = static_cast<std::uint8_t>(0x40u + face);

        const auto header = MakeHeader(GPUTEXTUREFORMAT_8, GPUENDIAN_NONE);
        const auto texture = LoadTexture(MakeConfig(header, pixels, 1, 1, 1, TextureType::T_CUBE));
        REQUIRE(texture);
        REQUIRE(texture->GetFaceCount() == 6);
        for (auto face = 0; face < texture->GetFaceCount(); ++face)
            REQUIRE(texture->GetBufferForMipLevel(0, face)[0] == 0x40u + face);
    }

    TEST_CASE("Xenon DXT3A expands four-bit alpha to eight-bit samples", "[image][xenon]")
    {
        std::vector<std::uint8_t> pixels(4096);
        constexpr std::array<std::uint8_t, 8> block{0x10, 0x32, 0x54, 0x76, 0x98, 0xBA, 0xDC, 0xFE};
        std::copy(block.begin(), block.end(), pixels.begin());
        const auto header = MakeHeader(GPUTEXTUREFORMAT_DXT3A, GPUENDIAN_NONE);
        const auto texture = LoadTexture(MakeConfig(header, pixels, 4, 4));
        REQUIRE(texture);
        REQUIRE(texture->GetFormat()->GetId() == ImageFormatId::R8);
        for (auto pixel = 0u; pixel < 16u; ++pixel)
            REQUIRE(texture->GetBufferForMipLevel(0)[pixel] == pixel * 17u);
    }

    TEST_CASE("Xenon decoding rejects truncated data and unsupported volume textures", "[image][xenon]")
    {
        const auto header = MakeHeader(GPUTEXTUREFORMAT_8, GPUENDIAN_NONE);
        const std::vector<std::uint8_t> pixels(4095);
        std::string error;
        REQUIRE_FALSE(LoadTexture(MakeConfig(header, pixels, 1, 1), &error));
        REQUIRE(error == "Image base data is smaller than expected.");
        REQUIRE_FALSE(LoadTexture(MakeConfig(header, pixels, 1, 1, 1, TextureType::T_3D), &error));
        REQUIRE(error == "3D Xenon textures are not supported.");
    }

    TEST_CASE("Xenon decoding rejects invalid pitch and truncated packed mip rectangles", "[image][xenon]")
    {
        const std::vector<std::uint8_t> pixels(4096);
        const auto header = MakeHeader(GPUTEXTUREFORMAT_8, GPUENDIAN_NONE, true);
        std::string error;
        REQUIRE_FALSE(LoadTexture(MakeConfig(header, pixels, 64, 64), &error));
        REQUIRE(error == "Failed to extract base level.");

        auto packedHeader = MakeHeader(GPUTEXTUREFORMAT_8, GPUENDIAN_NONE, false, true);
        packedHeader[49] |= 0x10u; // Put the packed mip region at byte offset 4096.
        const std::vector<std::uint8_t> truncatedTail(4097);
        REQUIRE_FALSE(LoadTexture(MakeConfig(packedHeader, truncatedTail, 8, 8, 4), &error));
        REQUIRE(error == "Packed mip rectangle extends beyond its surface.");
    }
} // namespace test::image::xenon
