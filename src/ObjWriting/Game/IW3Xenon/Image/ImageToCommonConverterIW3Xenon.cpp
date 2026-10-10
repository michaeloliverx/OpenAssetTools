#include "ImageToCommonConverterIW3Xenon.h"

#include "Game/IW3Xenon/IW3Xenon.h"
#include "Image/XenonImageToCommonConverter.h"
#include "Utils/Logging/Log.h"

#include <cstring>
#include <format>
#include <limits>

using namespace IW3Xenon;

namespace
{
    std::unique_ptr<image::Texture> LoadHighMip(const GfxImage& image, const image::Texture& residentTexture, ISearchPath& searchPath)
    {
        const auto file = searchPath.Open(std::format("highmip/{}.hi", image.name));
        if (!file.IsOpen())
            return nullptr;

        const auto& loadDef = *image.texture.loadDef;
        image::xenon::XenonTextureLoadConfig config{reinterpret_cast<const std::uint8_t*>(loadDef.texture.map),
                                                    sizeof(*loadDef.texture.map),
                                                    image.pixels,
                                                    static_cast<std::size_t>(image.cardMemory.platform[0]),
                                                    image.width,
                                                    image.height,
                                                    image.depth,
                                                    loadDef.levelCount,
                                                    static_cast<std::uint32_t>(loadDef.format),
                                                    residentTexture.GetTextureType()};
        image::xenon::XenonTextureHeader header{};
        const auto baseSize = image::xenon::GetTextureBaseSize(config);
        if (!baseSize || !image::xenon::DecodeTextureHeader(config.textureHeader, config.textureHeaderSize, header)
            || image.width > oat::xenon::GPU_MAX_TEXTURE_DIMENSION / 2u || image.height > oat::xenon::GPU_MAX_TEXTURE_DIMENSION / 2u
            || header.fetch.pitch > 255u)
        {
            con::warn("Could not load high mip for image \"{}\": invalid texture layout", image.name);
            return nullptr;
        }

        // Load_Texture rebuilds baseSize in the engine. Use the serialized texture layout instead of the stale fastfile field.
        // R_StreamLoadFileSynchronously reads one higher mip containing four times the resident base size.
        const auto highMipSize = static_cast<std::uint64_t>(*baseSize) * 4u;
        if (highMipSize > std::numeric_limits<std::size_t>::max() || file.m_length < 0 || static_cast<std::uint64_t>(file.m_length) != highMipSize)
        {
            con::warn("Could not load high mip for image \"{}\": expected {} bytes, got {}", image.name, highMipSize, file.m_length);
            return nullptr;
        }

        std::vector<std::uint8_t> pixels(static_cast<std::size_t>(highMipSize));
        file.m_stream->read(reinterpret_cast<char*>(pixels.data()), static_cast<std::streamsize>(pixels.size()));
        if (file.m_stream->gcount() != static_cast<std::streamsize>(pixels.size()))
        {
            con::warn("Could not read high mip for image \"{}\"", image.name);
            return nullptr;
        }

        // RB_StreamSetHighMipNow doubles the dimensions and pitch. Decode only the external base level,
        // then prepend it to the resident mip chain so its original packed layout remains intact.
        config.width *= 2u;
        config.height *= 2u;
        config.levelCount = 1u;
        config.pixelData = pixels.data();
        config.pixelDataSize = pixels.size();
        header.fetch.pitch *= 2u;
        header.fetch.width = config.width;
        header.fetch.height = config.height;
        header.fetch.baseAddress = 0u;
        header.fetch.maxMipLevel = 0u;
        header.fetch.packedMips = 0u;
        header.fetch.mipAddress = 0u;
        const auto textureHeader = image::xenon::EncodeTextureHeader(header);
        config.textureHeader = textureHeader.data();
        config.textureHeaderSize = textureHeader.size();

        std::string error;
        const auto highMip = image::xenon::LoadTexture(config, &error);
        if (!highMip)
        {
            con::warn("Could not decode high mip for image \"{}\": {}", image.name, error);
            return nullptr;
        }

        auto texture = image::Texture::CreateForType(config.textureType, residentTexture.GetFormat(), config.width, config.height, config.depth, true);
        texture->Allocate();
        const auto faceCount = residentTexture.GetFaceCount();
        std::memcpy(texture->GetBufferForMipLevel(0), highMip->GetBufferForMipLevel(0), highMip->GetSizeOfMipLevel(0) * faceCount);
        const auto mipCount = residentTexture.HasMipMaps() ? residentTexture.GetMipMapCount() : 1;
        for (auto mip = 0; mip < mipCount; ++mip)
            std::memcpy(texture->GetBufferForMipLevel(mip + 1), residentTexture.GetBufferForMipLevel(mip), residentTexture.GetSizeOfMipLevel(mip) * faceCount);
        con::debug("Used high mip \"highmip/{}.hi\": {}x{} -> {}x{}", image.name, image.width, image.height, config.width, config.height);
        return texture;
    }
} // namespace

namespace image
{
    std::unique_ptr<Texture> ToCommonConverterIW3Xenon::Convert(const XAssetInfoGeneric& assetInfo, ISearchPath& searchPath)
    {
        const auto* image = reinterpret_cast<const XAssetInfo<AssetImage::Type>*>(&assetInfo)->Asset();
        if (!image)
            return nullptr;

        TextureType textureType;
        switch (image->mapType)
        {
        case MAPTYPE_2D:
            textureType = TextureType::T_2D;
            break;
        case MAPTYPE_CUBE:
            textureType = TextureType::T_CUBE;
            break;
        case MAPTYPE_3D:
            textureType = TextureType::T_3D;
            break;
        default:
            return nullptr;
        }

        if (!image->pixels)
            return nullptr;

        std::string error;
        auto texture = xenon::LoadImage(image, textureType, &error);
        if (!texture)
        {
            con::warn("Could not decode Xbox 360 image \"{}\": {}", assetInfo.m_name, error);
            return nullptr;
        }

        if (image->streaming)
        {
            auto highMipTexture = LoadHighMip(*image, *texture, searchPath);
            if (highMipTexture)
                return highMipTexture;
        }
        return texture;
    }
} // namespace image
