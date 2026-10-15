#include "Image/XenonTextureDecoder.h"
#include "Image/XenonTextureEncoder.h"

#include <array>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <cstring>

namespace image::xenon
{
    TEST_CASE("Native Xenon raw formats decode a shared packed base allocation", "[image][xenon][rawformat]")
    {
        const auto format = GENERATE(ImageFormatId::BC1, ImageFormatId::BC5, ImageFormatId::B8_G8_R8_A8);
        const auto height = GENERATE(8u, 16u, 32u);
        Texture2D source(ImageFormat::GetImageFormatById(format), 8u, height, true);
        source.Allocate();
        for (auto mip = 0; mip < source.GetMipMapCount(); ++mip)
            for (auto byte = 0uz; byte < source.GetSizeOfMipLevel(mip); ++byte)
                source.GetBufferForMipLevel(mip, 0)[byte] = static_cast<uint8_t>(byte * 17u + mip * 31u);

        XenonTextureEncodeResult encoded;
        REQUIRE(EncodeTexture(source, encoded));
        XenonTextureHeader header{};
        REQUIRE(DecodeTextureHeader(encoded.textureHeader.data(), encoded.textureHeader.size(), header));
        REQUIRE(header.fetch.packedMips == 1u);
        REQUIRE(header.fetch.mipAddress == 0u);

        // Cached images retain the raw format instead of a populated GPU header.
        const std::array<uint8_t, XENON_TEXTURE_HEADER_SIZE> emptyHeader{};
        const auto decoded = LoadTexture({emptyHeader.data(),
                                          emptyHeader.size(),
                                          encoded.pixels.data(),
                                          encoded.pixels.size(),
                                          source.GetWidth(),
                                          source.GetHeight(),
                                          1u,
                                          encoded.levelCount,
                                          encoded.rawFormat,
                                          TextureType::T_2D});
        REQUIRE(decoded);
        CHECK(decoded->GetMipMapCount() == source.GetMipMapCount());
        for (auto mip = 0; mip < source.GetMipMapCount(); ++mip)
            CHECK(std::memcmp(decoded->GetBufferForMipLevel(mip), source.GetBufferForMipLevel(mip, 0), source.GetSizeOfMipLevel(mip)) == 0);
    }
} // namespace image::xenon
