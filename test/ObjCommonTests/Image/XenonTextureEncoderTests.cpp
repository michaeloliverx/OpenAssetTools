#include "Image/XenonTextureDecoder.h"
#include "Image/XenonTextureEncoder.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <cstring>
#include <string>
#include <utility>

namespace
{
    void FillTexture(image::Texture& texture)
    {
        texture.Allocate();
        const auto levels = texture.HasMipMaps() ? texture.GetMipMapCount() : 1;
        for (auto face = 0; face < texture.GetFaceCount(); ++face)
            for (auto mip = 0; mip < levels; ++mip)
                for (auto offset = 0uz; offset < texture.GetSizeOfMipLevel(mip); ++offset)
                    texture.GetBufferForMipLevel(mip, face)[offset] = static_cast<uint8_t>(offset * 17u + mip * 31u + face * 43u);
    }

    TEST_CASE("Xenon texture encoding preserves all faces and mip levels", "[image][xenon][encoder]")
    {
        using namespace image;
        using namespace image::xenon;
        const auto formatId = GENERATE(ImageFormatId::BC1,
                                       ImageFormatId::BC2,
                                       ImageFormatId::BC3,
                                       ImageFormatId::BC4,
                                       ImageFormatId::BC5,
                                       ImageFormatId::B8_G8_R8_A8,
                                       ImageFormatId::R8,
                                       ImageFormatId::A8,
                                       ImageFormatId::R8_A8);
        const auto size = GENERATE(std::pair{64u, 64u}, std::pair{8u, 8u}, std::pair{256u, 128u}, std::pair{37u, 19u}, std::pair{1u, 64u}, std::pair{64u, 1u});
        const auto mipMaps = GENERATE(false, true);
        // Cubemap dimensions must be square.
        const auto type = GENERATE(TextureType::T_2D, TextureType::T_CUBE);
        if (type == TextureType::T_CUBE && size.first != size.second)
            return;

        CAPTURE(formatId, size, mipMaps, type);
        auto source = Texture::CreateForType(type, ImageFormat::GetImageFormatById(formatId), size.first, size.second, 1u, mipMaps);
        FillTexture(*source);
        XenonTextureEncodeResult encoded;
        std::string error;
        REQUIRE(EncodeTexture(*source, encoded, &error));
        XenonTextureHeader header;
        REQUIRE(DecodeTextureHeader(encoded.textureHeader.data(), encoded.textureHeader.size(), header));
        REQUIRE(header.fetch.tiled == 1u);
        REQUIRE(header.fetch.width == size.first);
        REQUIRE(header.fetch.height == size.second);
        REQUIRE(header.fetch.depth == static_cast<unsigned>(source->GetFaceCount()));
        REQUIRE(header.fetch.maxMipLevel == encoded.levelCount - 1u);
        if (type == TextureType::T_CUBE && size.first == 64u)
            REQUIRE(header.fetch.words[2] == 0x1407E03Fu);

        const XenonTextureLoadConfig config{encoded.textureHeader.data(),
                                            encoded.textureHeader.size(),
                                            encoded.pixels.data(),
                                            encoded.pixels.size(),
                                            size.first,
                                            size.second,
                                            1u,
                                            encoded.levelCount,
                                            encoded.rawFormat,
                                            type};
        const auto decoded = LoadTexture(config, &error);
        INFO(error);
        REQUIRE(decoded);
        REQUIRE(decoded->GetFormat()->GetId() == formatId);
        REQUIRE(decoded->HasMipMaps() == mipMaps);
        for (auto face = 0; face < source->GetFaceCount(); ++face)
            for (auto mip = 0u; mip < encoded.levelCount; ++mip)
                REQUIRE(std::memcmp(decoded->GetBufferForMipLevel(mip, face), source->GetBufferForMipLevel(mip, face), source->GetSizeOfMipLevel(mip)) == 0);
    }

    TEST_CASE("Xenon R8A8 encoding uses the native format and GPU header", "[image][xenon][encoder]")
    {
        image::Texture2D source(&image::format::R8_A8, 256u, 256u, true);
        FillTexture(source);
        image::xenon::XenonTextureEncodeResult encoded;
        REQUIRE(image::xenon::EncodeTexture(source, encoded));
        REQUIRE(encoded.rawFormat == 0x0800014Au);
        REQUIRE(encoded.levelCount == 9u);
        REQUIRE(encoded.baseSize == 131072u);
        REQUIRE(encoded.pixels.size() == 180224u);
        image::xenon::XenonTextureHeader header;
        REQUIRE(image::xenon::DecodeTextureHeader(encoded.textureHeader.data(), encoded.textureHeader.size(), header));
        REQUIRE(header.common == image::xenon::D3DCOMMON_TYPE_TEXTURE);
        REQUIRE(header.referenceCount == 1u);
        REQUIRE(header.baseFlush == image::xenon::D3DFLUSH_INITIAL_VALUE);
        REQUIRE(header.mipFlush == image::xenon::D3DFLUSH_INITIAL_VALUE);
        REQUIRE(header.fetch.words[0] == 2181038082u);
        REQUIRE(header.fetch.words[1] == 74u);
        REQUIRE(header.fetch.words[2] == 2089215u);
        REQUIRE(header.fetch.words[3] == 1024u);
        REQUIRE(header.fetch.words[4] == 512u);
        REQUIRE(header.fetch.words[5] == 133632u);
    }

    TEST_CASE("Xenon format fallback retains alpha swizzling and texture offsets", "[image][xenon][encoder]")
    {
        using namespace image;
        using namespace image::xenon;
        Texture2D source(&format::A8, 64u, 64u, true);
        FillTexture(source);
        XenonTextureEncodeResult encoded;
        REQUIRE(EncodeTexture(source, encoded));
        // An unrecognized fetch format makes the decoder rebuild from loadDef.format.
        encoded.textureHeader[32] = (encoded.textureHeader[32] & 0xc0u) | GPUTEXTUREFORMAT_CTX1;
        const XenonTextureLoadConfig config{encoded.textureHeader.data(),
                                            encoded.textureHeader.size(),
                                            encoded.pixels.data(),
                                            encoded.pixels.size(),
                                            source.GetWidth(),
                                            source.GetHeight(),
                                            1u,
                                            encoded.levelCount,
                                            encoded.rawFormat,
                                            TextureType::T_2D};
        const auto decoded = LoadTexture(config);
        REQUIRE(decoded);
        REQUIRE(decoded->GetFormat()->GetId() == ImageFormatId::A8);
        for (auto mip = 0u; mip < encoded.levelCount; ++mip)
            REQUIRE(std::memcmp(decoded->GetBufferForMipLevel(mip), source.GetBufferForMipLevel(mip, 0), source.GetSizeOfMipLevel(mip)) == 0);
    }
} // namespace
