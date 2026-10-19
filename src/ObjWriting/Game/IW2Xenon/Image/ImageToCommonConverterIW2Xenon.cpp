#include "ImageToCommonConverterIW2Xenon.h"

#include "Game/IW2Xenon/IW2Xenon.h"
#include "Image/XenonTextureDecoder.h"
#include "Utils/Logging/Log.h"

#include <array>
#include <cstring>

using namespace IW2Xenon;

namespace image
{
    std::unique_ptr<Texture> ToCommonConverterIW2Xenon::Convert(const XAssetInfoGeneric& assetInfo, ISearchPath& searchPath)
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

        if (!image->pixels || image->dataSize <= 0 || !image->texture.loadDef || !image->texture.loadDef->texture.map)
            return nullptr;

        std::string error;
        const auto* loadDef = image->texture.loadDef;
        // The launch SDK header has four resource words before the fetch constant.
        std::array<uint8_t, xenon::XENON_TEXTURE_HEADER_SIZE> header{};
        std::memcpy(header.data() + 28, loadDef->texture.map->Format, sizeof(D3DTexture::Format));
        auto texture = xenon::LoadTexture({header.data(),
                                           header.size(),
                                           image->pixels,
                                           static_cast<size_t>(image->dataSize),
                                           image->width,
                                           image->height,
                                           image->depth,
                                           loadDef->levelCount,
                                           static_cast<uint32_t>(loadDef->format),
                                           textureType},
                                          &error);
        if (!texture)
        {
            con::warn("Could not decode Xbox 360 image \"{}\": {}", assetInfo.m_name, error);
            return nullptr;
        }

        return texture;
    }
} // namespace image
