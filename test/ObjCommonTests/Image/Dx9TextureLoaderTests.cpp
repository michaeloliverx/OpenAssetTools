#include "Image/D3DFormat.h"
#include "Image/Dx9TextureLoader.h"

#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <vector>

namespace
{
    std::vector<std::uint8_t> CreateFaceMajorCubeData(const image::Texture& texture)
    {
        std::vector<std::uint8_t> data;

        for (auto face = 0; face < texture.GetFaceCount(); face++)
        {
            for (auto mipLevel = 0; mipLevel < texture.GetMipMapCount(); mipLevel++)
            {
                const auto value = static_cast<std::uint8_t>(face * texture.GetMipMapCount() + mipLevel);
                const auto mipSize = texture.GetSizeOfMipLevel(mipLevel);
                data.insert(data.end(), mipSize, value);
            }
        }

        return data;
    }

    TEST_CASE("DX9 texture loader supports face-major cube map data", "[image][dx9][cube]")
    {
        image::TextureCube expected(&image::format::BC1, 16u, 16u, true);
        const auto data = CreateFaceMajorCubeData(expected);

        const auto texture = image::Dx9TextureLoader()
                                 .Format(oat::D3DFMT_DXT1)
                                 .Type(image::TextureType::T_CUBE)
                                 .HasMipMaps(true)
                                 .CubeMapOrder(image::Dx9TextureLoader::CubeMapDataOrder::FaceMajor)
                                 .Width(16u)
                                 .Height(16u)
                                 .LoadTexture(data.data());

        REQUIRE(texture);
        for (auto face = 0; face < texture->GetFaceCount(); face++)
        {
            for (auto mipLevel = 0; mipLevel < texture->GetMipMapCount(); mipLevel++)
            {
                const auto value = static_cast<std::uint8_t>(face * texture->GetMipMapCount() + mipLevel);
                const auto mipSize = texture->GetSizeOfMipLevel(mipLevel);
                const auto* pixels = texture->GetBufferForMipLevel(mipLevel, face);
                REQUIRE(std::all_of(pixels,
                                    pixels + mipSize,
                                    [value](const auto pixel)
                                    {
                                        return pixel == value;
                                    }));
            }
        }
    }
} // namespace
