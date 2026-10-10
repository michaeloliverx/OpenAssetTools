#pragma once

#include "Image/XenonTextureDecoder.h"

#include <type_traits>

namespace image::xenon
{
    template<typename GfxImage> std::unique_ptr<Texture> LoadImage(const GfxImage* image, TextureType textureType, std::string* error = nullptr)
    {
        if (!image)
        {
            if (error)
                *error = "Image metadata is missing.";
            return nullptr;
        }

        const auto* loadDef = image->texture.loadDef;
        if (!loadDef)
        {
            if (error)
                *error = "Image load definition is missing.";
            return nullptr;
        }

        using TextureHeader = std::remove_pointer_t<decltype(loadDef->texture.map)>;
        return LoadTexture({reinterpret_cast<const std::uint8_t*>(loadDef->texture.map),
                            sizeof(TextureHeader),
                            image->pixels,
                            image->cardMemory.platform[0] > 0 ? static_cast<std::size_t>(image->cardMemory.platform[0]) : 0u,
                            image->width,
                            image->height,
                            image->depth,
                            loadDef->levelCount,
                            static_cast<std::uint32_t>(loadDef->format),
                            textureType},
                           error);
    }
} // namespace image::xenon
