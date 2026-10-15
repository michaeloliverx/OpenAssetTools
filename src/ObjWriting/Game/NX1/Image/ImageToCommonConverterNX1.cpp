#include "ImageToCommonConverterNX1.h"

#include "Game/NX1/NX1.h"
#include "Image/XenonTextureDecoder.h"
#include "Utils/Logging/Log.h"

#include <format>
#include <fstream>
#include <vector>
#include <zlib.h>

using namespace NX1;

namespace
{
    constexpr unsigned STREAM_PIXEL_SIZE_MASK = 0x03FFFFFFu;
    constexpr unsigned STREAM_LEVEL_COUNT_SHIFT = 26u;

    bool ReadStream(
        ISearchPath& searchPath, const Zone& sourceZone, const XAssetStreamFile& record, unsigned char* destination, const size_t size, std::string& error)
    {
        const auto fileName = record.fileIndex ? std::format("imagefile{}.pak", record.fileIndex) : std::format("{}.ff", sourceZone.m_name);
        SearchPathOpenFile file;
        if (record.fileIndex == 0u && !sourceZone.m_source_path.empty())
        {
            auto input = std::make_unique<std::ifstream>(sourceZone.m_source_path, std::ios::binary | std::ios::ate);
            if (input->is_open())
            {
                const auto length = input->tellg();
                input->seekg(0);
                file = SearchPathOpenFile(std::move(input), static_cast<int64_t>(length));
            }
        }
        else
            file = searchPath.Open(fileName);
        if (!file.IsOpen())
        {
            error = std::format("Could not open {}", fileName);
            return false;
        }

        if (record.offsetEnd <= record.offset || file.m_length < 0 || record.offsetEnd > static_cast<uint64_t>(file.m_length))
        {
            error = std::format("Invalid stream range in {}", fileName);
            return false;
        }

        std::vector<unsigned char> compressed(record.offsetEnd - record.offset);
        file.m_stream->seekg(record.offset);
        file.m_stream->read(reinterpret_cast<char*>(compressed.data()), static_cast<std::streamsize>(compressed.size()));
        if (file.m_stream->gcount() != static_cast<std::streamsize>(compressed.size()))
        {
            error = std::format("Could not read stream from {}", fileName);
            return false;
        }

        auto outputSize = static_cast<uLongf>(size);
        if (uncompress(destination, &outputSize, compressed.data(), static_cast<uLong>(compressed.size())) != Z_OK || outputSize != size)
        {
            error = std::format("Invalid compressed image stream in {}", fileName);
            return false;
        }

        return true;
    }
} // namespace

namespace image
{
    std::unique_ptr<Texture> ToCommonConverterNX1::Convert(const XAssetInfoGeneric& assetInfo, ISearchPath& searchPath)
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
        default:
            return nullptr;
        }

        xenon::XenonTextureLoadConfig config{image->texture.data,
                                             sizeof(image->texture.data),
                                             image->pixels,
                                             image->cardMemory.platform[0] > 0 ? static_cast<size_t>(image->cardMemory.platform[0]) : 0u,
                                             image->width,
                                             image->height,
                                             image->depth,
                                             image->levelCount,
                                             static_cast<unsigned>(image->format),
                                             textureType};
        std::vector<unsigned char> pixels;
        std::string error;
        if (image->cached)
        {
            if (!assetInfo.m_zone)
                return nullptr;
            const auto& zone = *assetInfo.m_zone;
            const auto found = zone.m_xfile_image_stream_files_by_name.find(XAssetInfoGeneric::NormalizeAssetName(assetInfo.m_name));
            if (found == zone.m_xfile_image_stream_files_by_name.end() || found->second.empty())
            {
                con::warn("Could not decode Xbox 360 image \"{}\": Missing image stream records", assetInfo.m_name);
                return nullptr;
            }

            // Each slot adds higher mips in front of the preceding slot's data.
            // pixelSize stores the cumulative byte count and, in its upper six bits, the mip count.
            size_t previousSize = 0u;
            for (size_t slot = 0u; slot < std::size(image->streams); ++slot)
            {
                const auto& stream = image->streams[slot];
                const auto size = stream.pixelSize & STREAM_PIXEL_SIZE_MASK;
                if (!size)
                    continue;
                if (!stream.width || !stream.height || size < previousSize || !(stream.pixelSize >> STREAM_LEVEL_COUNT_SHIFT))
                {
                    con::warn("Could not decode Xbox 360 image \"{}\": Invalid image stream dimensions or size", assetInfo.m_name);
                    return nullptr;
                }
                previousSize = size;
                config.width = stream.width;
                config.height = stream.height;
                config.levelCount = stream.pixelSize >> STREAM_LEVEL_COUNT_SHIFT;
            }
            pixels.resize(previousSize);

            previousSize = 0u;
            for (size_t slot = 0u; slot < std::size(image->streams); ++slot)
            {
                const auto size = image->streams[slot].pixelSize & STREAM_PIXEL_SIZE_MASK;
                if (!size)
                    continue;
                if (!ReadStream(searchPath, zone, found->second.front()[slot], pixels.data() + pixels.size() - size, size - previousSize, error))
                {
                    con::warn("Could not decode Xbox 360 image \"{}\": {}", assetInfo.m_name, error);
                    return nullptr;
                }
                previousSize = size;
            }
            config.pixelData = pixels.data();
            config.pixelDataSize = pixels.size();
        }

        auto texture = xenon::LoadTexture(config, &error);
        if (!texture)
            con::warn("Could not decode Xbox 360 image \"{}\": {}", assetInfo.m_name, error);
        return texture;
    }
} // namespace image
