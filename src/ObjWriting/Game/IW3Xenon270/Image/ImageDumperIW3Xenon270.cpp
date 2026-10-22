#include "ImageDumperIW3Xenon270.h"

#include "Game/IW3Xenon270/Image/ImageToCommonConverterIW3Xenon270.h"
#include "Image/DdsWriter.h"
#include "Image/Dx9TextureLoader.h"
#include "Image/ImageCommon.h"
#include "Image/IwiLoader.h"
#include "Image/IwiWriter6.h"
#include "ObjWriting.h"
#include "Utils/Logging/Log.h"

#include <cassert>

using namespace IW3Xenon270;
using namespace image;

namespace image
{
    DumperIW3Xenon270::DumperIW3Xenon270()
    {
        switch (ObjWriting::Configuration.ImageOutputFormat)
        {
        case ImageOutputFormat_e::DDS:
            m_writer = std::make_unique<DdsWriter>();
            break;
        case ImageOutputFormat_e::IWI:
            m_writer = std::make_unique<iwi6::IwiWriter>();
            break;
        default:
            assert(false);
            m_writer = nullptr;
            break;
        }
    }

    void DumperIW3Xenon270::DumpAsset(AssetDumpingContext& context, const XAssetInfo<AssetImage::Type>& asset)
    {
        const auto* image = asset.Asset();
        auto texture = ToCommonConverterIW3Xenon270().Convert(asset, context.m_obj_search_path);
        if (!texture)
            return;

        auto convertedTexture = ConvertTextureForDdsFileOutputIfNecessary(texture.get());
        if (convertedTexture)
            texture = std::move(convertedTexture);

        if (!m_writer->SupportsImageFormat(texture->GetFormat()))
        {
            con::warn("Not dumping image {} as {} does not support the image format {}",
                      image->name,
                      GetImageOutputFormatName(ObjWriting::Configuration.ImageOutputFormat),
                      texture->GetFormat()->GetName());
            return;
        }

        const auto assetFile = context.OpenAssetFile(GetFileNameForAsset(asset.m_name, m_writer->GetFileExtension()));

        if (!assetFile)
            return;

        auto& stream = *assetFile;
        m_writer->DumpImage(stream, texture.get());
    }
} // namespace image
