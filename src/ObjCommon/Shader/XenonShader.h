#pragma once

#include "D3D9ShaderAnalyser.h"

#include <cstddef>
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
    // The cached microcode contains a big-endian D3D constant table, independently of the GPU instruction stream.
    std::unique_ptr<d3d9::ShaderInfo> GetShaderInfo(std::span<const uint8_t> cached);
    bool HaveMatchingConstantTables(const d3d9::ShaderInfo& first, const d3d9::ShaderInfo& second);
    std::string_view GetVertexShaderSourceName(std::string_view assetName);
    std::string GetVertexShaderVariantName(std::string_view sourceName, std::string_view declarationName);
    std::string EncodeFileName(std::string_view name);
    std::string GetFileNameForPixelShaderAssetName(std::string_view assetName);
    std::string GetFileNameForVertexShaderAssetName(std::string_view assetName);
} // namespace shader::xenon
