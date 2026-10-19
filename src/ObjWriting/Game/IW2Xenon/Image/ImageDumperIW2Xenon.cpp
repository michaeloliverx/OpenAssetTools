#include "ImageDumperIW2Xenon.h"

#include "Game/IW2Xenon/Image/ImageToCommonConverterIW2Xenon.h"
#include "Image/DdsWriter.h"
#include "Image/Dx9TextureLoader.h"
#include "Image/ImageCommon.h"
#include "Image/IwiLoader.h"
#include "Image/IwiWriter6.h"
#include "ObjWriting.h"
#include "Utils/Logging/Log.h"

#include <cassert>

using namespace IW2Xenon;
using namespace image;

namespace image
{
    DumperIW2Xenon::DumperIW2Xenon()
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

    void DumperIW2Xenon::DumpAsset(AssetDumpingContext& context, const XAssetInfo<AssetImage::Type>& asset)
    {
        const auto* image = asset.Asset();
        auto texture = ToCommonConverterIW2Xenon().Convert(asset, context.m_obj_search_path);
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
