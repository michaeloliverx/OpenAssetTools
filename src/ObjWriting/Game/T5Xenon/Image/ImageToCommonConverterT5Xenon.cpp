#include "ImageToCommonConverterT5Xenon.h"

#include "Game/T5Xenon/T5Xenon.h"
#include "Image/XenonTextureDecoder.h"
#include "Utils/Endianness.h"
#include "Utils/Logging/Log.h"

#include <array>
#include <cstring>
#include <format>
#include <minilzo.h>
#include <span>
#include <vector>

using namespace T5Xenon;

namespace
{
    uint32_t ReadBigEndian32(const unsigned char* data)
    {
        uint32_t value;
        std::memcpy(&value, data, sizeof(value));
        return endianness::FromBigEndian(value);
    }

    bool DecompressBlock(const std::span<const unsigned char> input, const std::span<unsigned char> output)
    {
        if (input.size() == output.size())
        {
            std::memcpy(output.data(), input.data(), output.size());
            return true;
        }

        auto size = static_cast<lzo_uint>(output.size());
        return lzo1x_decompress_safe(input.data(), static_cast<lzo_uint>(input.size()), output.data(), &size, nullptr) == LZO_E_OK && size == output.size();
    }

    bool DecompressStream(const std::span<const unsigned char> input, const std::span<unsigned char> output)
    {
        if (input.size() < 4)
            return false;

        // The first block can hold an overlapping tail, which the native reader copies after the remaining blocks.
        const auto header = ReadBigEndian32(input.data());
        auto overlap = (header & 0x80000000u) != 0u;
        auto offset = static_cast<size_t>(header & 0x7FFFFFFFu) + 4u;
        size_t written = 0u;
        std::vector<unsigned char> tail;
        while (written + tail.size() < output.size())
        {
            if (offset > input.size() || input.size() - offset < 8u)
                return false;
            const auto compressedSize = ReadBigEndian32(input.data() + offset);
            const auto size = ReadBigEndian32(input.data() + offset + 4u);
            offset += 8u;
            if (!size || compressedSize > size || compressedSize > input.size() - offset || size > output.size() - written - tail.size())
                return false;

            const auto compressed = input.subspan(offset, compressedSize);
            if (overlap)
            {
                if (size > 0x2000u)
                    return false;
                tail.resize(size);
                if (!DecompressBlock(compressed, tail))
                    return false;
                overlap = false;
            }
            else
            {
                if (!DecompressBlock(compressed, output.subspan(written, size)))
                    return false;
                written += size;
            }
            offset += compressedSize;
        }

        std::memcpy(output.data() + written, tail.data(), tail.size());
        return true;
    }

    bool ReadStream(ISearchPath& searchPath, const unsigned index, const std::span<unsigned char> output, std::string& error)
    {
        if (index >> 24u)
        {
            error = "Unsupported image pack slot";
            return false;
        }

        for (const auto* name : {"images.pak", "images_low.pak"})
        {
            const auto file = searchPath.Open(name);
            if (!file.IsOpen())
                continue;
            std::array<unsigned char, 28> header{};
            file.m_stream->read(reinterpret_cast<char*>(header.data()), header.size());
            const auto count = ReadBigEndian32(header.data() + 12u);
            const auto alignment = ReadBigEndian32(header.data() + 16u);
            const auto dataStart = ReadBigEndian32(header.data() + 20u);
            const auto hasSizes = ReadBigEndian32(header.data() + 24u) != 0u;
            const auto tableEnd = 28ull + static_cast<uint64_t>(count) * (hasSizes ? 6u : 4u);
            if (!*file.m_stream || std::memcmp(header.data(), "pak2", 4u) || ReadBigEndian32(header.data() + 8u) != 41u || alignment != 2048u
                || file.m_length < 0 || tableEnd > dataStart || dataStart > static_cast<uint64_t>(file.m_length) || index >= count)
            {
                error = std::format("Invalid image pack index in {}", name);
                return false;
            }

            uint32_t entry;
            file.m_stream->seekg(28ull + static_cast<uint64_t>(index) * 4u);
            file.m_stream->read(reinterpret_cast<char*>(&entry), sizeof(entry));
            entry = endianness::FromBigEndian(entry);
            if (entry == 0xFFFFFFFFu)
                continue;

            uint16_t sizeEntry = 0u;
            if (hasSizes)
            {
                file.m_stream->seekg(28ull + static_cast<uint64_t>(count) * 4u + static_cast<uint64_t>(index) * 2u);
                file.m_stream->read(reinterpret_cast<char*>(&sizeEntry), sizeof(sizeEntry));
                sizeEntry = endianness::FromBigEndian(sizeEntry);
            }
            const auto offset = static_cast<uint64_t>(entry) * alignment;
            const auto size = sizeEntry ? static_cast<size_t>(sizeEntry & 0x7FFFu) * alignment : output.size();
            if (!*file.m_stream || offset < dataStart || offset > static_cast<uint64_t>(file.m_length) || size > static_cast<uint64_t>(file.m_length) - offset)
            {
                error = std::format("Invalid image stream range in {}", name);
                return false;
            }

            std::vector<unsigned char> data(size);
            file.m_stream->seekg(static_cast<std::streamoff>(offset));
            file.m_stream->read(reinterpret_cast<char*>(data.data()), static_cast<std::streamsize>(data.size()));
            if (!*file.m_stream || ((sizeEntry & 0x8000u) ? !DecompressStream(data, output) : !DecompressBlock(data, output)))
            {
                error = std::format("Invalid compressed image stream in {}", name);
                return false;
            }
            return true;
        }

        error = std::format("Could not find image pack entry {}", index);
        return false;
    }
} // namespace

namespace image
{
    std::unique_ptr<Texture> ToCommonConverterT5Xenon::Convert(const XAssetInfoGeneric& assetInfo, ISearchPath& searchPath)
    {
        const auto* image = reinterpret_cast<const XAssetInfo<AssetImage::Type>*>(&assetInfo)->Asset();
        if (!image || image->cardMemory.platform[0] < 0)
            return nullptr;
        const auto pixelDataSize = static_cast<size_t>(image->cardMemory.platform[0]);
        TextureType type;
        switch (image->mapType)
        {
        case MAPTYPE_2D:
            type = TextureType::T_2D;
            break;
        case MAPTYPE_CUBE:
            type = TextureType::T_CUBE;
            break;
        case MAPTYPE_3D:
            type = TextureType::T_3D;
            break;
        default:
            return nullptr;
        }
        xenon::XenonTextureLoadConfig config{image->texture,
                                             sizeof(image->texture),
                                             image->pixels,
                                             pixelDataSize,
                                             image->width,
                                             image->height,
                                             image->depth,
                                             image->levelCount,
                                             image->format,
                                             type};
        std::string error;
        std::vector<unsigned char> pixels;
        if (image->streaming)
        {
            if (!image->streamedPartCount || image->streamedPartCount > std::size(image->streams))
                return nullptr;
            const auto& last = image->streams[image->streamedPartCount - 1u];
            const auto totalSize = last.pixelSize >> 4u;
            if (!totalSize || pixelDataSize > totalSize || (pixelDataSize && !image->pixels))
                return nullptr;
            pixels.resize(totalSize);
            auto previousSize = pixelDataSize;
            if (previousSize)
                std::memcpy(pixels.data() + totalSize - previousSize, image->pixels, previousSize);
            for (auto part = 0u; part < image->streamedPartCount; ++part)
            {
                const auto& stream = image->streams[part];
                const auto size = stream.pixelSize >> 4u;
                if (size < previousSize || size > totalSize || !stream.width || !stream.height || !(stream.pixelSize & 0xFu))
                {
                    error = "Invalid image stream dimensions or size";
                    break;
                }
                if (size > previousSize && !ReadStream(searchPath, stream.packIndex, std::span(pixels).subspan(totalSize - size, size - previousSize), error))
                    break;
                previousSize = size;
            }
            if (!error.empty())
            {
                con::warn("Could not decode Xbox 360 image \"{}\": {}", assetInfo.m_name, error);
                return nullptr;
            }
            // Stream parts prepend successively larger mip levels to the existing allocation.
            config.width = last.width;
            config.height = last.height;
            config.levelCount = last.pixelSize & 0xFu;
            config.pixelData = pixels.data();
            config.pixelDataSize = pixels.size();
        }
        auto result = xenon::LoadTexture(config, &error);
        if (!result)
            con::warn("Could not decode Xbox 360 image \"{}\": {}", assetInfo.m_name, error);
        return result;
    }
} // namespace image
