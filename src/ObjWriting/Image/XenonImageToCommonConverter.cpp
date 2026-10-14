#include "XenonImageToCommonConverter.h"

#include "Utils/Logging/Log.h"

#include <cstring>
#include <format>
#include <limits>

namespace
{
    std::unique_ptr<image::Texture> LoadHighMip(const std::string_view imageName,
                                                image::xenon::XenonTextureLoadConfig config,
                                                const image::Texture& residentTexture,
                                                ISearchPath& searchPath)
    {
        auto highMipPath = std::format("highmip/{}.hi", imageName);
        auto file = searchPath.Open(highMipPath);
        if (!file.IsOpen())
        {
            // Later T4 builds also split highmips by packed (~) and ordinary image names.
            highMipPath = std::format("highmip/{}/{}.hi", imageName.starts_with('~') ? "h0" : "h1", imageName);
            file = searchPath.Open(highMipPath);
            if (!file.IsOpen())
                return nullptr;
        }

        image::xenon::XenonTextureHeader header{};
        const auto baseSize = image::xenon::GetTextureBaseSize(config);
        if (!baseSize || !image::xenon::DecodeTextureHeader(config.textureHeader, config.textureHeaderSize, header)
            || config.width > oat::xenon::GPU_MAX_TEXTURE_DIMENSION / 2u || config.height > oat::xenon::GPU_MAX_TEXTURE_DIMENSION / 2u
            || header.fetch.pitch > 255u)
        {
            con::warn("Could not load high mip for image \"{}\": invalid texture layout", imageName);
            return nullptr;
        }

        // Load_Texture rebuilds baseSize in the engine. Use the serialized texture layout instead of the stale fastfile field.
        // R_StreamLoadFileSynchronously reads one higher mip containing four times the resident base size.
        const auto highMipSize = static_cast<std::uint64_t>(*baseSize) * 4u;
        if (highMipSize > std::numeric_limits<std::size_t>::max() || file.m_length < 0 || static_cast<std::uint64_t>(file.m_length) != highMipSize)
        {
            con::warn("Could not load high mip for image \"{}\": expected {} bytes, got {}", imageName, highMipSize, file.m_length);
            return nullptr;
        }

        std::vector<std::uint8_t> pixels(static_cast<std::size_t>(highMipSize));
        file.m_stream->read(reinterpret_cast<char*>(pixels.data()), static_cast<std::streamsize>(pixels.size()));
        if (file.m_stream->gcount() != static_cast<std::streamsize>(pixels.size()))
        {
            con::warn("Could not read high mip for image \"{}\"", imageName);
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
            con::warn("Could not decode high mip for image \"{}\": {}", imageName, error);
            return nullptr;
        }

        auto texture = image::Texture::CreateForType(config.textureType, residentTexture.GetFormat(), config.width, config.height, config.depth, true);
        texture->Allocate();
        const auto faceCount = residentTexture.GetFaceCount();
        std::memcpy(texture->GetBufferForMipLevel(0), highMip->GetBufferForMipLevel(0), highMip->GetSizeOfMipLevel(0) * faceCount);
        const auto mipCount = residentTexture.HasMipMaps() ? residentTexture.GetMipMapCount() : 1;
        for (auto mip = 0; mip < mipCount; ++mip)
            std::memcpy(texture->GetBufferForMipLevel(mip + 1), residentTexture.GetBufferForMipLevel(mip), residentTexture.GetSizeOfMipLevel(mip) * faceCount);
        con::debug("Used high mip \"{}\": {}x{} -> {}x{}", highMipPath, residentTexture.GetWidth(), residentTexture.GetHeight(), config.width, config.height);
        return texture;
    }
} // namespace

namespace image::xenon
{
    std::unique_ptr<Texture>
        LoadImage(const XenonTextureLoadConfig& config, const std::string_view imageName, const bool streaming, ISearchPath& searchPath, std::string* error)
    {
        auto texture = LoadTexture(config, error);
        if (!texture)
            return nullptr;

        if (streaming)
        {
            auto highMip = LoadHighMip(imageName, config, *texture, searchPath);
            if (highMip)
                return highMip;
        }
        return texture;
    }
} // namespace image::xenon
