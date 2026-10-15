#include "LoaderImageNX1.h"

#include "Image/DdsLoader.h"
#include "Image/ImageCommon.h"
#include "Image/ImageLoaderCommon.h"
#include "Image/TextureConverter.h"
#include "Image/XenonTextureEncoder.h"
#include "Utils/Logging/Log.h"

#include <format>
#include <limits>
#include <vector>
#include <zlib.h>

using namespace NX1;
using namespace image;

namespace
{
    constexpr unsigned STREAM_PIXEL_SIZE_MASK = 0x03FFFFFFu;
    constexpr unsigned STREAM_LEVEL_COUNT_SHIFT = 26u;

    class ImageLoader final : public AssetCreator<AssetImage>
    {
    public:
        ImageLoader(MemoryManager& memory, ISearchPath& searchPath, Zone& zone, IOutputPath& outDir, const unsigned imageFileIndex)
            : m_memory(memory),
              m_search_path(searchPath),
              m_zone(zone),
              m_out_dir(outDir),
              m_image_file_index(imageFileIndex),
              m_offset(0u)
        {
        }

        AssetCreationResult CreateAsset(const std::string& assetName, AssetCreationContext& context) override
        {
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
            }
            else
            {
                auto result = LoadImageCommon(assetName, m_search_path, IwiVersion::IWI_8, CommonImageLoaderHashType::NONE);
                if (const auto cancelled = result.GetResultIfCancelled())
                    return *cancelled;
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
            if (encoded.pixels.size() > STREAM_PIXEL_SIZE_MASK)
            {
                con::error("Encoded Xbox 360 image \"{}\" exceeds the image stream size limit", assetName);
                return AssetCreationResult::Failure();
            }

            auto compressedSize = compressBound(static_cast<uLong>(encoded.pixels.size()));
            std::vector<unsigned char> compressed(compressedSize);
            if (compress2(compressed.data(), &compressedSize, encoded.pixels.data(), static_cast<uLong>(encoded.pixels.size()), Z_DEFAULT_COMPRESSION) != Z_OK)
            {
                con::error("Failed to compress Xbox 360 image \"{}\"", assetName);
                return AssetCreationResult::Failure();
            }
            if (m_image_file_index == 0u)
            {
                if (m_zone.m_xfile_image_stream_data.size() > std::numeric_limits<unsigned>::max())
                    return AssetCreationResult::Failure();
                m_offset = static_cast<unsigned>(m_zone.m_xfile_image_stream_data.size());
            }
            if (compressedSize > std::numeric_limits<unsigned>::max() - m_offset)
            {
                con::error("Imagefile exceeds the maximum stream offset for image \"{}\"", assetName);
                return AssetCreationResult::Failure();
            }

            if (m_image_file_index == 0u)
            {
                m_zone.m_xfile_image_stream_data.insert(m_zone.m_xfile_image_stream_data.end(), compressed.begin(), compressed.begin() + compressedSize);
            }
            else
            {
                const auto fileName = std::format("imagefile{}.pak", m_image_file_index);
                if (!m_output)
                    m_output = m_out_dir.Open(fileName);
                if (!m_output)
                {
                    con::error("Could not open imagefile \"{}\"", fileName);
                    return AssetCreationResult::Failure();
                }
                m_output->write(reinterpret_cast<const char*>(compressed.data()), static_cast<std::streamsize>(compressedSize));
                if (!*m_output)
                {
                    con::error("Could not write image \"{}\" to {}", assetName, fileName);
                    return AssetCreationResult::Failure();
                }
            }

            auto* image = m_memory.Alloc<GfxImage>();
            image->name = m_memory.Dup(assetName.c_str());
            image->format = static_cast<int>(encoded.rawFormat);
            image->mapType = texture->GetTextureType() == TextureType::T_CUBE ? MAPTYPE_CUBE : MAPTYPE_2D;
            const auto isFunction = !assetName.empty() && (assetName[0] == '*' || assetName[0] == '$');
            image->semantic = isFunction ? TS_FUNCTION : TS_2D;
            image->category = isFunction ? IMG_CATEGORY_AUTO_GENERATED : IMG_CATEGORY_LOAD_FROM_FILE;
            image->width = image->height = image->depth = 1u;
            image->levelCount = 1u;
            image->cached = 1u;
            image->streams[0] = {static_cast<uint16_t>(texture->GetWidth()),
                                 static_cast<uint16_t>(texture->GetHeight()),
                                 static_cast<unsigned>(encoded.pixels.size()) | (encoded.levelCount << STREAM_LEVEL_COUNT_SHIFT)};

            XAssetStreamFilesForImage records{};
            const auto endOffset = m_offset + static_cast<unsigned>(compressedSize);
            records[0] = {m_image_file_index, m_offset, endOffset};
            m_offset = endOffset;
            m_zone.m_xfile_image_stream_files_by_name[XAssetInfoGeneric::NormalizeAssetName(assetName)].emplace_back(records);

            return AssetCreationResult::Success(context.AddAsset<AssetImage>(assetName, image));
        }

        void FinalizeZone(AssetCreationContext& context) override
        {
            m_output.reset();
        }

    private:
        MemoryManager& m_memory;
        ISearchPath& m_search_path;
        Zone& m_zone;
        IOutputPath& m_out_dir;
        unsigned m_image_file_index;
        unsigned m_offset;
        std::unique_ptr<std::ostream> m_output;
    };
} // namespace

namespace image
{
    std::unique_ptr<AssetCreator<AssetImage>>
        CreateLoaderNX1(MemoryManager& memory, ISearchPath& searchPath, Zone& zone, IOutputPath& outDir, const unsigned imageFileIndex)
    {
        return std::make_unique<ImageLoader>(memory, searchPath, zone, outDir, imageFileIndex);
    }
} // namespace image
