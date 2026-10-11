#include "XenonShader.h"

#include "Utils/Endianness.h"

#include <array>
#include <cstring>
#include <format>

namespace shader::xenon
{
    std::optional<ShaderParts> GetShaderParts(const std::span<const uint8_t> data, const ShaderStage stage)
    {
        std::array<uint32_t, 3> header{};
        if (data.size() < sizeof(header))
            return std::nullopt;

        std::memcpy(header.data(), data.data(), sizeof(header));
        const auto version = endianness::ToBigEndian(header[0]);
        const auto cachedSize = endianness::ToBigEndian(header[1]);
        const auto physicalSize = endianness::ToBigEndian(header[2]);
        constexpr auto MICROCODE_VERSION = 0x102a1100u;
        if (version != (MICROCODE_VERSION | static_cast<uint32_t>(stage)) || cachedSize < sizeof(header)
            || static_cast<uint64_t>(cachedSize) + physicalSize != data.size())
            return std::nullopt;

        return ShaderParts{data.first(cachedSize), data.subspan(cachedSize)};
    }

    std::string EncodeFileName(const std::string_view name)
    {
        constexpr char HEX_DIGITS[] = "0123456789ABCDEF";
        std::string result;
        for (const auto character : name)
        {
            const auto byte = static_cast<unsigned char>(character);
            if ((byte >= 'a' && byte <= 'z') || (byte >= 'A' && byte <= 'Z') || (byte >= '0' && byte <= '9') || byte == '_' || byte == '-' || byte == '.')
                result.push_back(character);
            else
            {
                // Shader variants contain '*' and other characters that cannot appear in Windows filenames.
                result.push_back('~');
                result.push_back(HEX_DIGITS[byte >> 4u]);
                result.push_back(HEX_DIGITS[byte & 0xfu]);
            }
        }
        return result;
    }

    std::string GetFileNameForPixelShaderAssetName(const std::string_view assetName)
    {
        return std::format("shader_bin/ps_{}.xpu", EncodeFileName(assetName));
    }

} // namespace shader::xenon
