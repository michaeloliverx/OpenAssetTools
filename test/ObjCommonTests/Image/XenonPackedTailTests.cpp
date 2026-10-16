#include "Image/XenonTextureDecoder.h"
#include "Image/XenonTextureEncoder.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <cstring>

TEST_CASE("Small native block-compressed shared mip tails occupy a single GPU page", "[xenon][packedtail]")
{
    const auto dimension = GENERATE(8u, 16u);
    const auto format = GENERATE(image::ImageFormatId::BC1, image::ImageFormatId::BC3);
    auto source = image::Texture::CreateForType(image::TextureType::T_2D, image::ImageFormat::GetImageFormatById(format), dimension, dimension, 1u, true);
    source->Allocate();
    for (auto mip = 0; mip < source->GetMipMapCount(); ++mip)
        for (auto offset = 0uz; offset < source->GetSizeOfMipLevel(mip); ++offset)
            source->GetBufferForMipLevel(mip)[offset] = static_cast<uint8_t>(offset * 13u + mip * 29u);

    image::xenon::XenonTextureEncodeResult encoded;
    REQUIRE(image::xenon::EncodeTexture(*source, encoded));
    REQUIRE(encoded.pixels.size() > 4096u);
    encoded.pixels.resize(4096u);
    image::xenon::XenonTextureLoadConfig config{encoded.textureHeader.data(),
                                                encoded.textureHeader.size(),
                                                encoded.pixels.data(),
                                                encoded.pixels.size(),
                                                dimension,
                                                dimension,
                                                1u,
                                                encoded.levelCount,
                                                encoded.rawFormat,
                                                image::TextureType::T_2D};
    const auto decoded = image::xenon::LoadTexture(config);
    REQUIRE(decoded);
    for (auto mip = 0; mip < source->GetMipMapCount(); ++mip)
        REQUIRE(std::memcmp(decoded->GetBufferForMipLevel(mip), source->GetBufferForMipLevel(mip), source->GetSizeOfMipLevel(mip)) == 0);

    config.pixelDataSize = 4095u;
    REQUIRE_FALSE(image::xenon::LoadTexture(config));
}
