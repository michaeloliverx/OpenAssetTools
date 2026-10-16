#include "Image/XenonImageToCommonConverter.h"

#include "SearchPath/MockSearchPath.h"

#include <algorithm>
#include <array>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <cstdint>
#include <string>

namespace
{
    class StreamedImageData
    {
    public:
        StreamedImageData()
        {
            using namespace image::xenon;
            XenonTextureHeader header{};
            header.fetch.type = GPUCONSTANTTYPE_TEXTURE;
            header.fetch.pitch = 1;
            header.fetch.dataFormat = GPUTEXTUREFORMAT_8;
            header.fetch.endian = GPUENDIAN_8IN32;
            header.fetch.width = 8;
            header.fetch.height = 8;
            header.fetch.depth = 1;
            header.fetch.maxMipLevel = 3;
            header.fetch.dimension = GPUDIMENSION_2D;
            header.fetch.packedMips = 1;
            m_header = EncodeTextureHeader(header);

            constexpr unsigned origins[4][2]{
                {16, 0},
                {8,  0},
                {4,  0},
                {0,  4}
            };
            for (auto mip = 0u; mip < 4u; ++mip)
                for (auto y = 0u; y < (8u >> mip); ++y)
                    for (auto x = 0u; x < (8u >> mip); ++x)
                        m_pixels[((origins[mip][1] + y) * 32u + origins[mip][0] + x) ^ 3u] = static_cast<uint8_t>(0x20u + mip);
        }

        image::xenon::XenonTextureLoadConfig Config() const
        {
            using namespace image::xenon;
            return {m_header.data(),
                    m_header.size(),
                    m_pixels.data(),
                    m_pixels.size(),
                    8,
                    8,
                    1,
                    4,
                    GPUTEXTUREFORMAT_8 | (GPUENDIAN_8IN32 << 6u),
                    image::TextureType::T_2D};
        }

    private:
        std::array<uint8_t, image::xenon::XENON_TEXTURE_HEADER_SIZE> m_header{};
        std::array<uint8_t, 4096> m_pixels{};
    };

    std::string MakeHighMip(const uint8_t value)
    {
        std::string pixels(16384, '\0');
        for (auto y = 0u; y < 16u; ++y)
            for (auto x = 0u; x < 16u; ++x)
                pixels[y * 64u + (x ^ 3u)] = static_cast<char>(value);
        return pixels;
    }

    void CheckMip(const image::Texture& texture, const int mip, const uint8_t value)
    {
        const auto* data = texture.GetBufferForMipLevel(mip);
        CHECK(std::all_of(data,
                          data + texture.GetSizeOfMipLevel(mip),
                          [value](auto pixel)
                          {
                              return pixel == value;
                          }));
    }

    TEST_CASE("Xenon images find QoS highmips in all four numbered directories", "[xenon][image][highmip]")
    {
        const auto bucket = GENERATE(0u, 1u, 2u, 3u);
        constexpr const char* names[]{"dstreamed", "astreamed", "bstreamed", "cstreamed"};
        constexpr const char* paths[]{"highmip/0/dstreamed.hi", "highmip/1/astreamed.hi", "highmip/2/bstreamed.hi", "highmip/3/cstreamed.hi"};
        const auto value = static_cast<uint8_t>(0x80u + bucket);
        const StreamedImageData resident;
        MockSearchPath searchPath;
        searchPath.AddFileData(paths[bucket], MakeHighMip(value));

        const auto texture = image::xenon::LoadImage(resident.Config(), names[bucket], true, searchPath);
        REQUIRE(texture);
        CHECK(texture->GetWidth() == 16u);
        CHECK(texture->GetHeight() == 16u);
        REQUIRE(texture->GetMipMapCount() == 5);
        CheckMip(*texture, 0, value);
        for (auto mip = 0; mip < 4; ++mip)
            CheckMip(*texture, mip + 1, static_cast<uint8_t>(0x20 + mip));
    }

    TEST_CASE("Xenon image highmips prefer existing layouts over numbered directories", "[xenon][image][highmip]")
    {
        const auto layout = GENERATE(0u, 1u, 2u);
        constexpr const char* names[]{"streamed", "~streamed", "streamed"};
        constexpr const char* existingPaths[]{"highmip/streamed.hi", "highmip/h0/~streamed.hi", "highmip/h1/streamed.hi"};
        constexpr const char* numberedPaths[]{"highmip/3/streamed.hi", "highmip/2/~streamed.hi", "highmip/3/streamed.hi"};
        const StreamedImageData resident;
        MockSearchPath searchPath;
        searchPath.AddFileData(existingPaths[layout], MakeHighMip(0x71));
        searchPath.AddFileData(numberedPaths[layout], MakeHighMip(0x39));

        const auto texture = image::xenon::LoadImage(resident.Config(), names[layout], true, searchPath);
        REQUIRE(texture);
        CHECK(texture->GetWidth() == 16u);
        CheckMip(*texture, 0, 0x71);
    }
} // namespace
