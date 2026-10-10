#include "XenonTextureEncoder.h"

#include "Platform/Xenon/D3DFormat.h"
#include "XenonTextureDecoder.h"
#include "XenonTextureLayout.h"

#include <algorithm>
#include <cstring>
#include <limits>
#include <utility>

namespace image::xenon
{
    using namespace detail;

    namespace
    {
        struct XenonTextureEncodeFormatInfo
        {
            ImageFormatId imageFormat;
            D3DFORMAT d3dFormat;
            std::uint32_t blockWidth;
            std::uint32_t blockHeight;
            std::uint32_t bytesPerBlock;
        };

        void CopyBlockRectToSurface(const std::uint8_t* source,
                                    const std::uint32_t sourceWidthBlocks,
                                    const std::uint32_t sourceHeightBlocks,
                                    std::uint8_t* destination,
                                    const std::uint32_t destinationWidthBlocks,
                                    const std::uint32_t destinationXBlocks,
                                    const std::uint32_t destinationYBlocks,
                                    const std::uint32_t bytesPerBlock)
        {
            const auto sourceRowBytes = sourceWidthBlocks * bytesPerBlock;
            for (auto row = 0u; row < sourceHeightBlocks; ++row)
            {
                const auto* sourceRow = source + row * sourceRowBytes;
                auto* destinationRow = destination + ((destinationYBlocks + row) * destinationWidthBlocks + destinationXBlocks) * bytesPerBlock;
                std::memcpy(destinationRow, sourceRow, sourceRowBytes);
            }
        }

        void TileTextureBlocks(const std::uint8_t* linearData,
                               const std::size_t linearDataSize,
                               const std::uint32_t widthBlocks,
                               const std::uint32_t heightBlocks,
                               const std::uint32_t outputPitchBlocks,
                               const std::uint32_t bytesPerBlock,
                               std::uint8_t* tiledData,
                               const std::size_t tiledDataSize)
        {
            const auto log2BytesPerBlock = CalculateLog2BytesPerBlock(bytesPerBlock);
            for (auto y = 0u; y < heightBlocks; ++y)
            {
                const auto tiledRow = TiledOffset2DRow(y, outputPitchBlocks, log2BytesPerBlock);
                for (auto x = 0u; x < widthBlocks; ++x)
                {
                    auto tiledOffset = TiledOffset2DColumn(x, y, log2BytesPerBlock, tiledRow);
                    tiledOffset = (tiledOffset >> log2BytesPerBlock) * bytesPerBlock;

                    const auto linearOffset = static_cast<std::size_t>(y * widthBlocks + x) * bytesPerBlock;
                    if (linearOffset + bytesPerBlock <= linearDataSize && tiledOffset + bytesPerBlock <= tiledDataSize)
                        std::memcpy(tiledData + tiledOffset, linearData + linearOffset, bytesPerBlock);
                }
            }
        }

        void SetError(std::string* error, std::string message)
        {
            if (error != nullptr)
                *error = std::move(message);
        }

        const XenonTextureEncodeFormatInfo* GetEncodeFormatInfo(const ImageFormatId imageFormat)
        {
            static constexpr XenonTextureEncodeFormatInfo FORMATS[]{
                {ImageFormatId::BC1,         D3DFMT_DXT1,     4u, 4u, 8u },
                {ImageFormatId::BC2,         D3DFMT_DXT3,     4u, 4u, 16u},
                {ImageFormatId::BC3,         D3DFMT_DXT5,     4u, 4u, 16u},
                {ImageFormatId::BC4,         D3DFMT_DXT5A,    4u, 4u, 8u },
                {ImageFormatId::BC5,         D3DFMT_DXN,      4u, 4u, 16u},
                {ImageFormatId::B8_G8_R8_A8, D3DFMT_A8R8G8B8, 1u, 1u, 4u },
                {ImageFormatId::R8,          D3DFMT_L8,       1u, 1u, 1u },
                {ImageFormatId::A8,          D3DFMT_A8,       1u, 1u, 1u },
                {ImageFormatId::R8_A8,       D3DFMT_A8L8,     1u, 1u, 2u },
            };

            for (const auto& format : FORMATS)
            {
                if (format.imageFormat == imageFormat)
                    return &format;
            }

            return nullptr;
        }

    } // namespace

    bool EncodeTexture(const Texture& texture, XenonTextureEncodeResult& result, std::string* error)
    {
        const auto textureType = texture.GetTextureType();
        if (textureType != TextureType::T_2D && textureType != TextureType::T_CUBE)
        {
            SetError(error, "Only 2D and cube Xenon textures are supported.");
            return false;
        }

        if (texture.GetWidth() == 0u || texture.GetWidth() > GPU_MAX_TEXTURE_DIMENSION || texture.GetHeight() == 0u
            || texture.GetHeight() > GPU_MAX_TEXTURE_DIMENSION || texture.GetDepth() != 1u
            || (textureType == TextureType::T_CUBE && texture.GetWidth() != texture.GetHeight()))
        {
            SetError(error, "Xenon texture dimensions are invalid.");
            return false;
        }

        if (texture.Empty())
        {
            SetError(error, "Xenon texture has no pixel data.");
            return false;
        }

        const auto* formatInfo = GetEncodeFormatInfo(texture.GetFormat()->GetId());
        if (formatInfo == nullptr)
        {
            SetError(error, "Image format is not supported by the Xenon texture encoder.");
            return false;
        }

        const auto width = texture.GetWidth();
        const auto height = texture.GetHeight();
        const auto faceCount = static_cast<std::uint32_t>(texture.GetFaceCount());
        const auto levelCount = texture.HasMipMaps() ? static_cast<std::uint32_t>(texture.GetMipMapCount()) : 1u;
        XenonTextureFetchConstant fetch{};
        SetTextureFetchFormat(fetch, formatInfo->d3dFormat);
        fetch.pitch = CalculatePitch(width, *formatInfo);
        const auto baseLayout = CalculateBaseLevelLayout(fetch, width, height, *formatInfo);
        const auto packedLevel = levelCount > 1u ? GetPackedMipLevel(width, height) : std::numeric_limits<std::uint32_t>::max();
        const auto hasPackedMips = packedLevel < levelCount;

        std::vector<XenonTextureLevelLayout> mipLayouts(levelCount);
        std::vector<std::uint32_t> mipOffsets(levelCount, 0u);
        auto mipRegionSize = 0u;
        for (auto mipLevel = 1u; mipLevel < levelCount; ++mipLevel)
        {
            mipLayouts[mipLevel] = CalculateMipLevelLayout(width, height, mipLevel, *formatInfo);
            if (!hasPackedMips || mipLevel < packedLevel)
            {
                mipOffsets[mipLevel] = mipRegionSize;
                mipRegionSize += mipLayouts[mipLevel].arraySliceStrideBytes * faceCount;
            }
        }

        XenonTextureLevelLayout packedLayout{};
        auto packedRegionOffset = 0u;
        if (hasPackedMips && packedLevel > 0u)
        {
            packedLayout = mipLayouts[packedLevel];
            packedRegionOffset = mipRegionSize;
            mipRegionSize += packedLayout.arraySliceStrideBytes * faceCount;
        }

        const auto baseRegionSize = baseLayout.arraySliceStrideBytes * faceCount;
        result = {};
        result.levelCount = levelCount;
        result.baseSize = baseRegionSize;
        const auto mipAddressPages = mipRegionSize > 0u ? baseRegionSize >> GPU_TEXTURE_ADDRESS_SHIFT : 0u;
        result.rawFormat = formatInfo->d3dFormat;
        result.pixels.resize(baseRegionSize + mipRegionSize);

        const auto baseSurfaceWidthBlocks = baseLayout.rowPitchBytes / formatInfo->bytesPerBlock;
        for (auto faceIndex = 0u; faceIndex < faceCount; ++faceIndex)
        {
            std::vector<std::uint8_t> baseSurface(baseLayout.arraySliceStrideBytes);
            std::vector<std::uint8_t> packedSurface;
            auto packedSurfaceWidthBlocks = 0u;
            if (hasPackedMips && packedLevel > 0u)
            {
                packedSurface.resize(packedLayout.arraySliceStrideBytes);
                packedSurfaceWidthBlocks = packedLayout.rowPitchBytes / formatInfo->bytesPerBlock;
            }

            for (auto mipLevel = 0u; mipLevel < levelCount; ++mipLevel)
            {
                const auto mipWidth = std::max(width >> mipLevel, 1u);
                const auto mipHeight = std::max(height >> mipLevel, 1u);
                const auto mipWidthBlocks = std::max(1u, DivideRoundUp(mipWidth, formatInfo->blockWidth));
                const auto mipHeightBlocks = std::max(1u, DivideRoundUp(mipHeight, formatInfo->blockHeight));
                const auto* linearData = texture.GetBufferForMipLevel(static_cast<int>(mipLevel), static_cast<int>(faceIndex));
                const auto linearDataSize = texture.GetSizeOfMipLevel(static_cast<int>(mipLevel));

                if (hasPackedMips && mipLevel >= packedLevel)
                {
                    std::uint32_t xBlocks;
                    std::uint32_t yBlocks;
                    if (!GetPackedMipOffsetBlocks(width, height, *formatInfo, mipLevel, xBlocks, yBlocks))
                    {
                        SetError(error, "Failed to calculate a packed Xenon mip offset.");
                        return false;
                    }

                    auto& target = packedLevel == 0u ? baseSurface : packedSurface;
                    const auto targetWidthBlocks = packedLevel == 0u ? baseSurfaceWidthBlocks : packedSurfaceWidthBlocks;
                    CopyBlockRectToSurface(
                        linearData, mipWidthBlocks, mipHeightBlocks, target.data(), targetWidthBlocks, xBlocks, yBlocks, formatInfo->bytesPerBlock);
                }
                else if (mipLevel == 0u)
                {
                    CopyBlockRectToSurface(
                        linearData, mipWidthBlocks, mipHeightBlocks, baseSurface.data(), baseSurfaceWidthBlocks, 0u, 0u, formatInfo->bytesPerBlock);
                }
                else
                {
                    const auto& layout = mipLayouts[mipLevel];
                    const auto levelOffset = baseRegionSize + mipOffsets[mipLevel] + faceIndex * layout.arraySliceStrideBytes;
                    TileTextureBlocks(linearData,
                                      linearDataSize,
                                      mipWidthBlocks,
                                      mipHeightBlocks,
                                      layout.rowPitchBytes / formatInfo->bytesPerBlock,
                                      formatInfo->bytesPerBlock,
                                      result.pixels.data() + levelOffset,
                                      layout.arraySliceStrideBytes);
                }
            }

            const auto baseOffset = faceIndex * baseLayout.arraySliceStrideBytes;
            TileTextureBlocks(baseSurface.data(),
                              baseSurface.size(),
                              baseSurfaceWidthBlocks,
                              baseLayout.zSliceStrideBlockRows,
                              baseSurfaceWidthBlocks,
                              formatInfo->bytesPerBlock,
                              result.pixels.data() + baseOffset,
                              baseLayout.arraySliceStrideBytes);

            if (hasPackedMips && packedLevel > 0u)
            {
                const auto faceOffset = baseRegionSize + packedRegionOffset + faceIndex * packedLayout.arraySliceStrideBytes;
                TileTextureBlocks(packedSurface.data(),
                                  packedSurface.size(),
                                  packedSurfaceWidthBlocks,
                                  packedLayout.zSliceStrideBlockRows,
                                  packedSurfaceWidthBlocks,
                                  formatInfo->bytesPerBlock,
                                  result.pixels.data() + faceOffset,
                                  packedLayout.arraySliceStrideBytes);
            }
        }

        ApplyGpuEndian(fetch.endian, result.pixels.data(), result.pixels.size());

        fetch.type = GPUCONSTANTTYPE_TEXTURE;
        fetch.width = width;
        fetch.height = height;
        fetch.depth = faceCount;
        fetch.maxMipLevel = levelCount - 1u;
        fetch.dimension = textureType == TextureType::T_CUBE ? GPUDIMENSION_CUBEMAP : GPUDIMENSION_2D;
        fetch.packedMips = hasPackedMips;
        fetch.mipAddress = mipAddressPages;

        XenonTextureHeader header{};
        header.common = D3DCOMMON_TYPE_TEXTURE;
        header.referenceCount = 1u;
        header.baseFlush = D3DFLUSH_INITIAL_VALUE;
        header.mipFlush = D3DFLUSH_INITIAL_VALUE;
        header.fetch = fetch;
        result.textureHeader = EncodeTextureHeader(header);
        return true;
    }
} // namespace image::xenon
