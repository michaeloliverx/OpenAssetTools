#pragma once

#include "XenonTextureDecoder.h"

#include <algorithm>
#include <cassert>
#include <cstdint>

namespace image::xenon::detail
{
    struct XenonTextureLevelLayout
    {
        std::uint32_t widthBlocks;
        std::uint32_t heightBlocks;
        std::uint32_t rowPitchBytes;
        std::uint32_t zSliceStrideBlockRows;
        std::uint32_t arraySliceStrideBytes;
    };

    constexpr std::uint32_t DivideRoundUp(const std::uint32_t value, const std::uint32_t divisor)
    {
        return (value + divisor - 1u) / divisor;
    }

    constexpr std::uint32_t AlignTo(const std::uint32_t value, const std::uint32_t alignment)
    {
        return DivideRoundUp(value, alignment) * alignment;
    }

    inline std::uint32_t NextPow2(const std::uint32_t value)
    {
        if (value <= 1u)
            return 1u;

        auto result = 1u;
        while (result < value)
            result <<= 1u;
        return result;
    }

    inline std::uint32_t Log2Ceil(const std::uint32_t value)
    {
        assert(value != 0u);

        auto result = 0u;
        auto current = value - 1u;
        while (current != 0u)
        {
            current >>= 1u;
            result++;
        }

        return result;
    }

    inline std::uint32_t GetPackedMipLevel(const std::uint32_t width, const std::uint32_t height)
    {
        const auto log2Size = Log2Ceil(std::min(width, height));
        return log2Size > 4u ? log2Size - 4u : 0u;
    }

    template<typename FormatInfo>
    bool GetPackedMipOffsetBlocks(const std::uint32_t width,
                                  const std::uint32_t height,
                                  const FormatInfo& formatInfo,
                                  const std::uint32_t mipLevel,
                                  std::uint32_t& xBlocks,
                                  std::uint32_t& yBlocks)
    {
        const auto log2Width = Log2Ceil(width);
        const auto log2Height = Log2Ceil(height);
        const auto log2Size = std::min(log2Width, log2Height);

        if (log2Size > 4u + mipLevel)
        {
            xBlocks = 0u;
            yBlocks = 0u;
            return false;
        }

        const auto packedMipBase = log2Size > 4u ? log2Size - 4u : 0u;
        const auto packedMip = mipLevel - packedMipBase;

        std::uint32_t xTexels;
        std::uint32_t yTexels;

        if (packedMip < 3u)
        {
            if (log2Width > log2Height)
            {
                xTexels = 0u;
                yTexels = 16u >> packedMip;
            }
            else
            {
                xTexels = 16u >> packedMip;
                yTexels = 0u;
            }
        }
        else
        {
            const auto offsetTexels = (1u << ((log2Width > log2Height ? log2Width : log2Height) - packedMipBase)) >> (packedMip - 2u);

            if (log2Width > log2Height)
            {
                xTexels = offsetTexels;
                yTexels = 0u;
            }
            else
            {
                xTexels = 0u;
                yTexels = offsetTexels;
            }
        }

        xBlocks = xTexels / formatInfo.blockWidth;
        yBlocks = yTexels / formatInfo.blockHeight;
        return true;
    }

    inline std::uint32_t TiledOffset2DRow(const std::uint32_t y, const std::uint32_t width, const std::uint32_t log2BytesPerBlock)
    {
        const auto macro = ((y / GPU_TEXTURE_TILE_DIMENSION) * (width / GPU_TEXTURE_TILE_DIMENSION)) << (log2BytesPerBlock + 7u);
        const auto micro = ((y & 6u) << 2u) << log2BytesPerBlock;
        return macro + ((micro & ~0xFu) << 1u) + (micro & 0xFu) + ((y & 8u) << (3u + log2BytesPerBlock)) + ((y & 1u) << 4u);
    }

    inline std::uint32_t
        TiledOffset2DColumn(const std::uint32_t x, const std::uint32_t y, const std::uint32_t log2BytesPerBlock, const std::uint32_t baseOffset)
    {
        const auto macro = (x / GPU_TEXTURE_TILE_DIMENSION) << (log2BytesPerBlock + 7u);
        const auto micro = (x & 7u) << log2BytesPerBlock;
        const auto offset = baseOffset + macro + ((micro & ~0xFu) << 1u) + (micro & 0xFu);
        return ((offset & ~0x1FFu) << 3u) + ((offset & 0x1C0u) << 2u) + (offset & 0x3Fu) + ((y & 16u) << 7u) + (((((y & 8u) >> 2u) + (x >> 3u)) & 3u) << 6u);
    }

    inline std::uint32_t CalculateLog2BytesPerBlock(const std::uint32_t bytesPerBlock)
    {
        return (bytesPerBlock / 4u) + ((bytesPerBlock / 2u) >> (bytesPerBlock / 4u));
    }

    template<typename FormatInfo>
    XenonTextureLevelLayout
        CalculateBaseLevelLayout(const XenonTextureFetchConstant& fetch, const std::uint32_t width, const std::uint32_t height, const FormatInfo& formatInfo)
    {
        const auto rowPitchTexels = fetch.pitch * GPU_TEXTURE_TEXEL_PITCH_ALIGNMENT;

        XenonTextureLevelLayout layout{};
        layout.widthBlocks = std::max(1u, DivideRoundUp(width, formatInfo.blockWidth));
        layout.heightBlocks = std::max(1u, DivideRoundUp(height, formatInfo.blockHeight));
        layout.rowPitchBytes = std::max(1u, DivideRoundUp(rowPitchTexels, formatInfo.blockWidth)) * formatInfo.bytesPerBlock;
        layout.zSliceStrideBlockRows = AlignTo(layout.heightBlocks, GPU_TEXTURE_TILE_DIMENSION);
        layout.arraySliceStrideBytes = AlignTo(layout.rowPitchBytes * layout.zSliceStrideBlockRows, GPU_TEXTURE_ALIGNMENT);
        return layout;
    }

    template<typename FormatInfo> std::uint32_t CalculatePitch(const std::uint32_t width, const FormatInfo& formatInfo)
    {
        if (formatInfo.blockWidth > 1u)
        {
            const auto widthInBlocks = std::max(1u, DivideRoundUp(width, formatInfo.blockWidth));
            return AlignTo(widthInBlocks, GPU_TEXTURE_TILE_DIMENSION) * formatInfo.blockWidth / GPU_TEXTURE_TEXEL_PITCH_ALIGNMENT;
        }

        return AlignTo(width, GPU_TEXTURE_TEXEL_PITCH_ALIGNMENT) / GPU_TEXTURE_TEXEL_PITCH_ALIGNMENT;
    }

    template<typename FormatInfo>
    XenonTextureLevelLayout
        CalculateMipLevelLayout(const std::uint32_t width, const std::uint32_t height, const std::uint32_t mipLevel, const FormatInfo& formatInfo)
    {
        const auto mipWidthTexels = std::max(NextPow2(width) >> mipLevel, 1u);
        const auto mipHeightTexels = std::max(NextPow2(height) >> mipLevel, 1u);

        XenonTextureLevelLayout layout{};
        layout.widthBlocks = std::max(1u, DivideRoundUp(std::max(width >> mipLevel, 1u), formatInfo.blockWidth));
        layout.heightBlocks = std::max(1u, DivideRoundUp(std::max(height >> mipLevel, 1u), formatInfo.blockHeight));
        layout.rowPitchBytes = AlignTo(DivideRoundUp(mipWidthTexels, formatInfo.blockWidth), GPU_TEXTURE_TILE_DIMENSION) * formatInfo.bytesPerBlock;
        layout.zSliceStrideBlockRows = AlignTo(DivideRoundUp(mipHeightTexels, formatInfo.blockHeight), GPU_TEXTURE_TILE_DIMENSION);
        layout.arraySliceStrideBytes = AlignTo(layout.rowPitchBytes * layout.zSliceStrideBlockRows, GPU_TEXTURE_ALIGNMENT);
        return layout;
    }
} // namespace image::xenon::detail
