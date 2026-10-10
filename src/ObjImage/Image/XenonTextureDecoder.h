#pragma once

#include "Platform/Xenon/XenonGraphics.h"
#include "Texture.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace image::xenon
{
    using namespace oat::xenon;

    inline constexpr std::size_t XENON_TEXTURE_HEADER_SIZE = 52u;

    struct XenonTextureFormatInfo
    {
        std::uint32_t dataFormat;
        const ImageFormat* imageFormat;
        std::uint32_t blockWidth;
        std::uint32_t blockHeight;
        std::uint32_t bytesPerBlock;
        const char* name;
        bool expandDxt3Alpha;
    };

    struct XenonTextureFetchConstant
    {
        // Host-order backing words retain the SDK's unused/reserved bits.
        // EncodeTextureHeader writes the named fields over these words.
        std::array<std::uint32_t, 6> words{};
        GPUCONSTANTTYPE type;
        GPUSIGN signX;
        GPUSIGN signY;
        GPUSIGN signZ;
        GPUSIGN signW;
        GPUCLAMP clampX;
        GPUCLAMP clampY;
        GPUCLAMP clampZ;
        std::uint32_t pitch; // Units of GPU_TEXTURE_TEXEL_PITCH_ALIGNMENT texels.
        std::uint32_t tiled;
        GPUTEXTUREFORMAT dataFormat;
        GPUENDIAN endian;
        GPUREQUESTSIZE requestSize;
        std::uint32_t stacked;
        GPUCLAMPPOLICY clampPolicy;
        std::uint32_t baseAddress; // Units of GPU_TEXTURE_ALIGNMENT bytes.
        // Actual dimensions; serialization handles the GPU's minus-one encoding.
        std::uint32_t width;
        std::uint32_t height;
        std::uint32_t depth;
        GPUNUMFORMAT numFormat;
        GPUSWIZZLE swizzleX;
        GPUSWIZZLE swizzleY;
        GPUSWIZZLE swizzleZ;
        GPUSWIZZLE swizzleW;
        std::int32_t expAdjust;
        GPUMINMAGFILTER magFilter;
        GPUMINMAGFILTER minFilter;
        GPUMIPFILTER mipFilter;
        GPUANISOFILTER anisoFilter;
        std::uint32_t borderSize;
        GPUMINMAGFILTER volMagFilter;
        GPUMINMAGFILTER volMinFilter;
        std::uint32_t minMipLevel;
        std::uint32_t maxMipLevel;
        std::uint32_t magAnisoWalk;
        std::uint32_t minAnisoWalk;
        std::int32_t lodBias;
        std::int32_t gradExpAdjustH;
        std::int32_t gradExpAdjustV;
        GPUBORDERCOLOR borderColor;
        std::uint32_t forceBcwToMax;
        GPUTRICLAMP triClamp;
        std::int32_t anisoBias;
        GPUDIMENSION dimension;
        std::uint32_t packedMips;
        std::uint32_t mipAddress; // Units of GPU_TEXTURE_ALIGNMENT bytes.
    };

    struct XenonTextureHeader
    {
        // Decoded D3DBaseTexture resource fields, followed by its Format.
        std::uint32_t common = 0u;
        std::uint32_t referenceCount = 0u;
        std::uint32_t fence = 0u;
        std::uint32_t readFence = 0u;
        std::uint32_t identifier = 0u;
        std::uint32_t baseFlush = 0u;
        std::uint32_t mipFlush = 0u;
        XenonTextureFetchConstant fetch{};
    };

    struct XenonTextureLoadConfig
    {
        const std::uint8_t* textureHeader = nullptr;
        std::size_t textureHeaderSize = 0u;
        const std::uint8_t* pixelData = nullptr;
        std::size_t pixelDataSize = 0u;
        std::uint32_t width = 0u;
        std::uint32_t height = 0u;
        std::uint32_t depth = 1u;
        std::uint32_t levelCount = 1u;
        std::uint32_t rawFormat = 0u;
        TextureType textureType = TextureType::T_2D;
    };

    bool DecodeTextureHeader(const void* data, std::size_t dataSize, XenonTextureHeader& header);
    std::array<std::uint8_t, XENON_TEXTURE_HEADER_SIZE> EncodeTextureHeader(const XenonTextureHeader& header);
    void SetTextureFetchFormat(XenonTextureFetchConstant& fetch, std::uint32_t rawFormat);
    std::optional<std::size_t> GetTextureBaseSize(const XenonTextureLoadConfig& config);
    const XenonTextureFormatInfo* GetTextureFormatInfo(std::uint32_t dataFormat);
    const char* GetTextureFormatName(std::uint32_t dataFormat);
    const char* GetEndianName(std::uint32_t endian);
    const char* GetDimensionName(std::uint32_t dimension);
    void ApplyGpuEndian(std::uint32_t endian, std::uint8_t* data, std::size_t size);
    std::vector<std::uint8_t> UntileTextureBlocks(const std::uint8_t* data,
                                                  std::size_t dataSize,
                                                  std::uint32_t widthInBlocks,
                                                  std::uint32_t heightInBlocks,
                                                  std::uint32_t inputPitchBlocks,
                                                  std::uint32_t bytesPerBlock);
    std::unique_ptr<Texture> LoadTexture(const XenonTextureLoadConfig& config, std::string* error = nullptr);
} // namespace image::xenon
