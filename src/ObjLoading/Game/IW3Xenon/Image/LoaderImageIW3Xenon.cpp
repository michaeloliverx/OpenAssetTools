#include "LoaderImageIW3Xenon.h"

#include "Image/DdsLoader.h"
#include "Image/ImageCommon.h"
#include "Image/ImageLoaderCommon.h"
#include "Image/TextureConverter.h"
#include "Image/XenonTextureEncoder.h"
#include "Utils/Logging/Log.h"

#include <cstring>
#include <limits>

using namespace IW3Xenon;
using namespace image;

namespace
{
    class ImageLoader final : public AssetCreator<AssetImage>
    {
    public:
        ImageLoader(MemoryManager& memory, ISearchPath& searchPath)
            : m_memory(memory),
              m_search_path(searchPath)
        {
        }

        AssetCreationResult CreateAsset(const std::string& assetName, AssetCreationContext& context) override
        {
            CommonIwiMetaData meta{};
            std::unique_ptr<Texture> texture;
            const auto file = m_search_path.Open(GetFileNameForAsset(assetName, ".dds"));
            if (file.IsOpen())
            {
                texture = LoadDds(*file.m_stream);
                if (!texture)
                {
                    con::error("Failed to load dds file for image asset \"{}\"", assetName);
                    return AssetCreationResult::Failure();
                }
                meta.m_no_picmip = !texture->HasMipMaps();
            }
            else
            {
                auto result = LoadImageCommon(assetName, m_search_path, IwiVersion::IWI_6, CommonImageLoaderHashType::NONE);
                if (const auto cancelled = result.GetResultIfCancelled())
                    return *cancelled;
                meta = result.m_meta;
                texture = std::move(result.m_texture);
            }

            switch (texture->GetFormat()->GetId())
            {
            case ImageFormatId::B8_G8_R8:
            case ImageFormatId::B8_G8_R8_X8:
            case ImageFormatId::R8_G8_B8_A8:
                texture = TextureConverter(texture.get(), &format::B8_G8_R8_A8).Convert();
                break;
            default:
                break;
            }

            xenon::XenonTextureEncodeResult encoded;
            std::string error;
            if (!texture || !xenon::EncodeTexture(*texture, encoded, &error))
            {
                con::error("Failed to encode Xbox 360 image \"{}\": {}", assetName, error);
                return AssetCreationResult::Failure();
            }

            if (encoded.pixels.size() > static_cast<size_t>(std::numeric_limits<int>::max()))
            {
                con::error("Encoded Xbox 360 image \"{}\" exceeds the target image size limit.", assetName);
                return AssetCreationResult::Failure();
            }

            auto* image = m_memory.Alloc<GfxImage>();
            image->name = m_memory.Dup(assetName.c_str());
            const auto isFunction = !assetName.empty() && (assetName[0] == '*' || assetName[0] == '$');
            image->semantic = isFunction ? TS_FUNCTION : TS_2D;
            image->category = isFunction ? IMG_CATEGORY_AUTO_GENERATED : IMG_CATEGORY_LOAD_FROM_FILE;
            image->mapType = texture->GetTextureType() == TextureType::T_CUBE ? MAPTYPE_CUBE : MAPTYPE_2D;
            image->width = static_cast<uint16_t>(texture->GetWidth());
            image->height = static_cast<uint16_t>(texture->GetHeight());
            image->depth = static_cast<uint16_t>(texture->GetDepth());
            image->delayLoadPixels = !isFunction;
            image->cardMemory.platform[0] = static_cast<int>(encoded.pixels.size());
            image->pixels = m_memory.Alloc<uint8_t>(encoded.pixels.size());
            std::memcpy(image->pixels, encoded.pixels.data(), encoded.pixels.size());
            image->baseSize = encoded.baseSize;
            image->streamSlot = std::numeric_limits<uint16_t>::max();
            image->streaming = false;

            auto* loadDef = m_memory.Alloc<GfxImageLoadDef>();
            loadDef->levelCount = static_cast<uint8_t>(encoded.levelCount);
            loadDef->flags = 0;
            if (meta.m_no_picmip)
                loadDef->flags |= IMG_FLAG_NOPICMIP;
            if (!texture->HasMipMaps())
                loadDef->flags |= IMG_FLAG_NOMIPMAPS;
            if (image->mapType == MAPTYPE_CUBE)
                loadDef->flags |= IMG_FLAG_CUBEMAP;
            if (meta.m_clamp_u)
                loadDef->flags |= IMG_FLAG_CLAMP_U;
            if (meta.m_clamp_v)
                loadDef->flags |= IMG_FLAG_CLAMP_V;
            loadDef->dimensions[0] = static_cast<int16_t>(image->width);
            loadDef->dimensions[1] = static_cast<int16_t>(image->height);
            loadDef->dimensions[2] = static_cast<int16_t>(image->depth);
            loadDef->format = static_cast<int>(encoded.rawFormat);
            static_assert(sizeof(D3DTexture) == sizeof(encoded.textureHeader));
            loadDef->texture.map = m_memory.Alloc<D3DTexture>();
            std::memcpy(loadDef->texture.map, encoded.textureHeader.data(), encoded.textureHeader.size());
            image->texture.loadDef = loadDef;

            return AssetCreationResult::Success(context.AddAsset<AssetImage>(assetName, image));
        }

    private:
        MemoryManager& m_memory;
        ISearchPath& m_search_path;
    };
} // namespace

namespace image
{
    std::unique_ptr<AssetCreator<AssetImage>> CreateLoaderIW3Xenon(MemoryManager& memory, ISearchPath& searchPath)
    {
        return std::make_unique<ImageLoader>(memory, searchPath);
    }
} // namespace image
