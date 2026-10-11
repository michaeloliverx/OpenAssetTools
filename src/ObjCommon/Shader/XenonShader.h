#pragma once

#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <string_view>

namespace shader::xenon
{
    enum class ShaderStage : uint8_t
    {
        PIXEL,
        VERTEX,
    };

    struct ShaderParts
    {
        std::span<const uint8_t> cached;
        std::span<const uint8_t> physical;
    };

    // SDK microcode objects are big-endian and contain cached data followed by physical data.
    std::optional<ShaderParts> GetShaderParts(std::span<const uint8_t> data, ShaderStage stage);
    std::string EncodeFileName(std::string_view name);
    std::string GetFileNameForPixelShaderAssetName(std::string_view assetName);
} // namespace shader::xenon
