#include "ImageToCommonConverterIW3Xenon270.h"

#include "Game/IW3Xenon270/IW3Xenon270.h"
#include "Image/XenonImageToCommonConverter.h"
#include "Utils/Logging/Log.h"

using namespace IW3Xenon270;

namespace image
{
    std::unique_ptr<Texture> ToCommonConverterIW3Xenon270::Convert(const XAssetInfoGeneric& assetInfo, ISearchPath& searchPath)
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
        auto texture = xenon::LoadImage(image, textureType, searchPath, &error);
        if (!texture)
        {
            con::warn("Could not decode Xbox 360 image \"{}\": {}", assetInfo.m_name, error);
            return nullptr;
        }

        return texture;
    }
} // namespace image
