#include "XenonTextureDecoder.h"

#include "Platform/Xenon/D3DFormat.h"
#include "Utils/Endianness.h"
#include "XenonTextureLayout.h"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <limits>
#include <utility>

namespace image::xenon
{
    using namespace detail;

    namespace
    {
        constexpr std::uint32_t BitMask(const std::uint32_t bitCount)
        {
            return bitCount >= 32u ? std::numeric_limits<std::uint32_t>::max() : ((1u << bitCount) - 1u);
        }

        constexpr std::uint32_t GetBits(const std::uint32_t value, const std::uint32_t shift, const std::uint32_t bitCount)
        {
            return (value >> shift) & BitMask(bitCount);
        }

        constexpr std::int32_t SignExtend(const std::uint32_t value, const std::uint32_t bitCount)
        {
            const auto sign = 1u << (bitCount - 1u);
            return static_cast<std::int32_t>((value ^ sign) - sign);
        }

        void SetBits(std::uint32_t& word, const std::uint32_t value, const std::uint32_t shift, const std::uint32_t bitCount)
        {
            const auto mask = BitMask(bitCount);
            word = (word & ~(mask << shift)) | ((value & mask) << shift);
        }

        void EndianSwap8In16(std::uint8_t* data, const std::size_t size)
        {
            for (auto i = 0uz; i + 1u < size; i += 2u)
                std::swap(data[i], data[i + 1u]);
        }

        void EndianSwap8In32(std::uint8_t* data, const std::size_t size)
        {
            for (auto i = 0uz; i + 3u < size; i += 4u)
            {
                std::swap(data[i], data[i + 3u]);
                std::swap(data[i + 1u], data[i + 2u]);
            }
        }

        void EndianSwap16In32(std::uint8_t* data, const std::size_t size)
        {
            for (auto i = 0uz; i + 3u < size; i += 4u)
            {
                std::swap(data[i], data[i + 2u]);
                std::swap(data[i + 1u], data[i + 3u]);
            }
        }

        std::unique_ptr<Texture> CreateTextureStorage(const XenonTextureFormatInfo& formatInfo,
                                                      const TextureType textureType,
                                                      const std::uint32_t width,
                                                      const std::uint32_t height,
                                                      const std::uint32_t depth,
                                                      const bool hasMipMaps)
        {
            std::unique_ptr<Texture> texture;
            switch (textureType)
            {
            case TextureType::T_2D:
                texture = std::make_unique<Texture2D>(formatInfo.imageFormat, width, height, hasMipMaps);
                break;

            case TextureType::T_CUBE:
                texture = std::make_unique<TextureCube>(formatInfo.imageFormat, width, height, hasMipMaps);
                break;

            case TextureType::T_3D:
                texture = std::make_unique<Texture3D>(formatInfo.imageFormat, width, height, depth, hasMipMaps);
                break;

            default:
                return nullptr;
            }

            texture->Allocate();
            return texture;
        }

        bool CopyBlockRect(const std::uint8_t* source,
                           const std::size_t sourceSize,
                           const std::uint32_t sourceWidthBlocks,
                           const std::uint32_t sourceXBlocks,
                           const std::uint32_t sourceYBlocks,
                           std::uint8_t* destination,
                           const std::uint32_t destinationWidthBlocks,
                           const std::uint32_t copyWidthBlocks,
                           const std::uint32_t copyHeightBlocks,
                           const std::uint32_t bytesPerBlock)
        {
            if (copyWidthBlocks == 0u || copyHeightBlocks == 0u || sourceXBlocks > sourceWidthBlocks || copyWidthBlocks > sourceWidthBlocks - sourceXBlocks)
                return false;

            const auto lastRow = static_cast<std::uint64_t>(sourceYBlocks) + copyHeightBlocks - 1u;
            const auto requiredBytes = (lastRow * sourceWidthBlocks + sourceXBlocks + copyWidthBlocks) * bytesPerBlock;
            if (requiredBytes > sourceSize)
                return false;

            const auto destinationRowBytes = destinationWidthBlocks * bytesPerBlock;
            const auto copyRowBytes = copyWidthBlocks * bytesPerBlock;

            for (auto row = 0u; row < copyHeightBlocks; ++row)
            {
                const auto* sourceRow = source + ((sourceYBlocks + row) * sourceWidthBlocks + sourceXBlocks) * bytesPerBlock;
                auto* destinationRow = destination + row * destinationRowBytes;
                std::memcpy(destinationRow, sourceRow, copyRowBytes);
            }
            return true;
        }

        void DecodeDxt3AlphaBlocksToR8(const std::uint8_t* source,
                                       const std::uint32_t widthBlocks,
                                       const std::uint32_t heightBlocks,
                                       const std::uint32_t width,
                                       const std::uint32_t height,
                                       std::uint8_t* destination)
        {
            for (auto blockY = 0u; blockY < heightBlocks; ++blockY)
            {
                for (auto blockX = 0u; blockX < widthBlocks; ++blockX)
                {
                    const auto* block = source + (blockY * widthBlocks + blockX) * 8u;
                    for (auto pixelY = 0u; pixelY < 4u; ++pixelY)
                    {
                        const auto y = blockY * 4u + pixelY;
                        if (y >= height)
                            continue;

                        for (auto pixelX = 0u; pixelX < 4u; ++pixelX)
                        {
                            const auto x = blockX * 4u + pixelX;
                            if (x >= width)
                                continue;

                            const auto alphaIndex = pixelY * 4u + pixelX;
                            const auto alphaByte = block[alphaIndex / 2u];
                            const auto alpha4 = (alphaIndex & 1u) != 0u ? alphaByte >> 4u : alphaByte & 0xFu;
                            destination[y * width + x] = static_cast<std::uint8_t>(alpha4 | (alpha4 << 4u));
                        }
                    }
                }
            }
        }

        bool ExtractLevelFromStoredSurface(const std::uint8_t* surfaceData,
                                           const std::size_t surfaceDataSize,
                                           const bool isTiled,
                                           const std::uint32_t surfaceWidthBlocks,
                                           const std::uint32_t surfaceHeightBlocks,
                                           const std::uint32_t sourceXBlocks,
                                           const std::uint32_t sourceYBlocks,
                                           std::uint8_t* destination,
                                           const std::uint32_t destinationWidthBlocks,
                                           const std::uint32_t copyWidthBlocks,
                                           const std::uint32_t copyHeightBlocks,
                                           const std::uint32_t bytesPerBlock)
        {
            if (isTiled)
            {
                const auto linearSurface =
                    UntileTextureBlocks(surfaceData, surfaceDataSize, surfaceWidthBlocks, surfaceHeightBlocks, surfaceWidthBlocks, bytesPerBlock);
                return CopyBlockRect(linearSurface.data(),
                                     linearSurface.size(),
                                     surfaceWidthBlocks,
                                     sourceXBlocks,
                                     sourceYBlocks,
                                     destination,
                                     destinationWidthBlocks,
                                     copyWidthBlocks,
                                     copyHeightBlocks,
                                     bytesPerBlock);
            }

            const auto requiredBytes = static_cast<std::uint64_t>(surfaceWidthBlocks) * surfaceHeightBlocks * bytesPerBlock;
            if (requiredBytes > surfaceDataSize)
                return false;

            return CopyBlockRect(surfaceData,
                                 surfaceDataSize,
                                 surfaceWidthBlocks,
                                 sourceXBlocks,
                                 sourceYBlocks,
                                 destination,
                                 destinationWidthBlocks,
                                 copyWidthBlocks,
                                 copyHeightBlocks,
                                 bytesPerBlock);
        }

        void SetError(std::string* error, std::string message)
        {
            if (error != nullptr)
                *error = std::move(message);
        }

        void DecodeFetchConstant(XenonTextureFetchConstant& fetch)
        {
            const auto word0 = fetch.words[0];
            const auto word1 = fetch.words[1];
            const auto word2 = fetch.words[2];
            const auto word3 = fetch.words[3];
            const auto word4 = fetch.words[4];
            const auto word5 = fetch.words[5];

            fetch.type = static_cast<GPUCONSTANTTYPE>(GetBits(word0, 0u, 2u));
            fetch.signX = static_cast<GPUSIGN>(GetBits(word0, 2u, 2u));
            fetch.signY = static_cast<GPUSIGN>(GetBits(word0, 4u, 2u));
            fetch.signZ = static_cast<GPUSIGN>(GetBits(word0, 6u, 2u));
            fetch.signW = static_cast<GPUSIGN>(GetBits(word0, 8u, 2u));
            fetch.clampX = static_cast<GPUCLAMP>(GetBits(word0, 10u, 3u));
            fetch.clampY = static_cast<GPUCLAMP>(GetBits(word0, 13u, 3u));
            fetch.clampZ = static_cast<GPUCLAMP>(GetBits(word0, 16u, 3u));
            fetch.pitch = GetBits(word0, 22u, 9u);
            fetch.tiled = GetBits(word0, 31u, 1u);

            fetch.dataFormat = static_cast<GPUTEXTUREFORMAT>(GetBits(word1, 0u, 6u));
            fetch.endian = static_cast<GPUENDIAN>(GetBits(word1, 6u, 2u));
            fetch.requestSize = static_cast<GPUREQUESTSIZE>(GetBits(word1, 8u, 2u));
            fetch.stacked = GetBits(word1, 10u, 1u);
            fetch.clampPolicy = static_cast<GPUCLAMPPOLICY>(GetBits(word1, 11u, 1u));
            fetch.baseAddress = GetBits(word1, 12u, 20u);

            fetch.numFormat = static_cast<GPUNUMFORMAT>(GetBits(word3, 0u, 1u));
            fetch.swizzleX = static_cast<GPUSWIZZLE>(GetBits(word3, 1u, 3u));
            fetch.swizzleY = static_cast<GPUSWIZZLE>(GetBits(word3, 4u, 3u));
            fetch.swizzleZ = static_cast<GPUSWIZZLE>(GetBits(word3, 7u, 3u));
            fetch.swizzleW = static_cast<GPUSWIZZLE>(GetBits(word3, 10u, 3u));
            fetch.expAdjust = SignExtend(GetBits(word3, 13u, 6u), 6u);
            fetch.magFilter = static_cast<GPUMINMAGFILTER>(GetBits(word3, 19u, 2u));
            fetch.minFilter = static_cast<GPUMINMAGFILTER>(GetBits(word3, 21u, 2u));
            fetch.mipFilter = static_cast<GPUMIPFILTER>(GetBits(word3, 23u, 2u));
            fetch.anisoFilter = static_cast<GPUANISOFILTER>(GetBits(word3, 25u, 3u));
            fetch.borderSize = GetBits(word3, 31u, 1u);

            fetch.volMagFilter = static_cast<GPUMINMAGFILTER>(GetBits(word4, 0u, 1u));
            fetch.volMinFilter = static_cast<GPUMINMAGFILTER>(GetBits(word4, 1u, 1u));
            fetch.minMipLevel = GetBits(word4, 2u, 4u);
            fetch.maxMipLevel = GetBits(word4, 6u, 4u);
            fetch.magAnisoWalk = GetBits(word4, 10u, 1u);
            fetch.minAnisoWalk = GetBits(word4, 11u, 1u);
            fetch.lodBias = SignExtend(GetBits(word4, 12u, 10u), 10u);
            fetch.gradExpAdjustH = SignExtend(GetBits(word4, 22u, 5u), 5u);
            fetch.gradExpAdjustV = SignExtend(GetBits(word4, 27u, 5u), 5u);

            fetch.borderColor = static_cast<GPUBORDERCOLOR>(GetBits(word5, 0u, 2u));
            fetch.forceBcwToMax = GetBits(word5, 2u, 1u);
            fetch.triClamp = static_cast<GPUTRICLAMP>(GetBits(word5, 3u, 2u));
            fetch.anisoBias = SignExtend(GetBits(word5, 5u, 4u), 4u);
            fetch.dimension = static_cast<GPUDIMENSION>(GetBits(word5, 9u, 2u));
            fetch.packedMips = GetBits(word5, 11u, 1u);
            fetch.mipAddress = GetBits(word5, 12u, 20u);

            if (fetch.dimension == GPUDIMENSION_3D)
            {
                fetch.width = GetBits(word2, 0u, 11u) + 1u;
                fetch.height = GetBits(word2, 11u, 11u) + 1u;
                fetch.depth = GetBits(word2, 22u, 10u) + 1u;
            }
            else if (fetch.stacked != 0u || fetch.dimension == GPUDIMENSION_CUBEMAP)
            {
                fetch.width = GetBits(word2, 0u, 13u) + 1u;
                fetch.height = GetBits(word2, 13u, 13u) + 1u;
                fetch.depth = GetBits(word2, 26u, 6u) + 1u;
            }
            else if (fetch.dimension == GPUDIMENSION_1D)
            {
                fetch.width = GetBits(word2, 0u, 24u) + 1u;
                fetch.height = 1u;
                fetch.depth = 1u;
            }
            else
            {
                fetch.width = GetBits(word2, 0u, 13u) + 1u;
                fetch.height = GetBits(word2, 13u, 13u) + 1u;
                fetch.depth = 1u;
            }
        }

        std::array<std::uint32_t, 6> EncodeFetchConstant(const XenonTextureFetchConstant& fetch)
        {
            auto words = fetch.words;

            SetBits(words[0], fetch.type, 0u, 2u);
            SetBits(words[0], fetch.signX, 2u, 2u);
            SetBits(words[0], fetch.signY, 4u, 2u);
            SetBits(words[0], fetch.signZ, 6u, 2u);
            SetBits(words[0], fetch.signW, 8u, 2u);
            SetBits(words[0], fetch.clampX, 10u, 3u);
            SetBits(words[0], fetch.clampY, 13u, 3u);
            SetBits(words[0], fetch.clampZ, 16u, 3u);
            SetBits(words[0], fetch.pitch, 22u, 9u);
            SetBits(words[0], fetch.tiled, 31u, 1u);

            SetBits(words[1], fetch.dataFormat, 0u, 6u);
            SetBits(words[1], fetch.endian, 6u, 2u);
            SetBits(words[1], fetch.requestSize, 8u, 2u);
            SetBits(words[1], fetch.stacked, 10u, 1u);
            SetBits(words[1], fetch.clampPolicy, 11u, 1u);
            SetBits(words[1], fetch.baseAddress, 12u, 20u);

            SetBits(words[3], fetch.numFormat, 0u, 1u);
            SetBits(words[3], fetch.swizzleX, 1u, 3u);
            SetBits(words[3], fetch.swizzleY, 4u, 3u);
            SetBits(words[3], fetch.swizzleZ, 7u, 3u);
            SetBits(words[3], fetch.swizzleW, 10u, 3u);
            SetBits(words[3], static_cast<std::uint32_t>(fetch.expAdjust), 13u, 6u);
            SetBits(words[3], fetch.magFilter, 19u, 2u);
            SetBits(words[3], fetch.minFilter, 21u, 2u);
            SetBits(words[3], fetch.mipFilter, 23u, 2u);
            SetBits(words[3], fetch.anisoFilter, 25u, 3u);
            SetBits(words[3], fetch.borderSize, 31u, 1u);

            SetBits(words[4], fetch.volMagFilter, 0u, 1u);
            SetBits(words[4], fetch.volMinFilter, 1u, 1u);
            SetBits(words[4], fetch.minMipLevel, 2u, 4u);
            SetBits(words[4], fetch.maxMipLevel, 6u, 4u);
            SetBits(words[4], fetch.magAnisoWalk, 10u, 1u);
            SetBits(words[4], fetch.minAnisoWalk, 11u, 1u);
            SetBits(words[4], static_cast<std::uint32_t>(fetch.lodBias), 12u, 10u);
            SetBits(words[4], static_cast<std::uint32_t>(fetch.gradExpAdjustH), 22u, 5u);
            SetBits(words[4], static_cast<std::uint32_t>(fetch.gradExpAdjustV), 27u, 5u);

            SetBits(words[5], fetch.borderColor, 0u, 2u);
            SetBits(words[5], fetch.forceBcwToMax, 2u, 1u);
            SetBits(words[5], fetch.triClamp, 3u, 2u);
            SetBits(words[5], static_cast<std::uint32_t>(fetch.anisoBias), 5u, 4u);
            SetBits(words[5], fetch.dimension, 9u, 2u);
            SetBits(words[5], fetch.packedMips, 11u, 1u);
            SetBits(words[5], fetch.mipAddress, 12u, 20u);

            if (fetch.dimension == GPUDIMENSION_3D)
            {
                SetBits(words[2], fetch.width - 1u, 0u, 11u);
                SetBits(words[2], fetch.height - 1u, 11u, 11u);
                SetBits(words[2], fetch.depth - 1u, 22u, 10u);
            }
            else if (fetch.stacked != 0u || fetch.dimension == GPUDIMENSION_CUBEMAP)
            {
                SetBits(words[2], fetch.width - 1u, 0u, 13u);
                SetBits(words[2], fetch.height - 1u, 13u, 13u);
                SetBits(words[2], fetch.depth - 1u, 26u, 6u);
            }
            else if (fetch.dimension == GPUDIMENSION_1D)
            {
                SetBits(words[2], fetch.width - 1u, 0u, 24u);
            }
            else
            {
                SetBits(words[2], fetch.width - 1u, 0u, 13u);
                SetBits(words[2], fetch.height - 1u, 13u, 13u);
            }

            return words;
        }

        bool BuildFetchConstantFromRawFormat(const XenonTextureLoadConfig& config, XenonTextureFetchConstant& fetch, const XenonTextureFormatInfo*& formatInfo)
        {
            const auto dataFormat = (config.rawFormat & D3DFORMAT_TEXTUREFORMAT_MASK) >> D3DFORMAT_TEXTUREFORMAT_SHIFT;
            formatInfo = GetTextureFormatInfo(dataFormat);
            if (formatInfo == nullptr)
                return false;

            const auto levelCount = std::max(1u, config.levelCount);
            const auto faceCount = config.textureType == TextureType::T_CUBE ? 6u : 1u;
            const auto dimension = config.textureType == TextureType::T_CUBE ? GPUDIMENSION_CUBEMAP
                                   : config.textureType == TextureType::T_3D ? GPUDIMENSION_3D
                                                                             : GPUDIMENSION_2D;
            const auto pitch = CalculatePitch(config.width, *formatInfo);
            const auto packedLevel = levelCount > 1u ? GetPackedMipLevel(config.width, config.height) : std::numeric_limits<std::uint32_t>::max();
            const auto hasPackedMips = packedLevel < levelCount;

            XenonTextureFetchConstant temporaryFetch{};
            temporaryFetch.pitch = pitch;
            const auto baseLayout = CalculateBaseLevelLayout(temporaryFetch, config.width, config.height, *formatInfo);
            const auto baseRegionSize = baseLayout.arraySliceStrideBytes * faceCount;
            const auto mipAddressPages = levelCount > 1u ? baseRegionSize >> GPU_TEXTURE_ADDRESS_SHIFT : 0u;

            fetch = {};
            SetTextureFetchFormat(fetch, config.rawFormat);
            fetch.type = GPUCONSTANTTYPE_TEXTURE;
            fetch.pitch = pitch;
            fetch.width = config.width;
            fetch.height = config.height;
            fetch.depth = dimension == GPUDIMENSION_3D ? std::max(config.depth, 1u) : faceCount;
            fetch.maxMipLevel = levelCount - 1u;
            fetch.dimension = dimension;
            fetch.packedMips = hasPackedMips;
            fetch.mipAddress = mipAddressPages;
            return true;
        }

    } // namespace

    bool DecodeTextureHeader(const void* data, const std::size_t dataSize, XenonTextureHeader& header)
    {
        if (data == nullptr || dataSize < XENON_TEXTURE_HEADER_SIZE)
            return false;

        std::array<std::uint32_t, XENON_TEXTURE_HEADER_SIZE / sizeof(std::uint32_t)> words;
        const auto* bytes = static_cast<const std::uint8_t*>(data);
        for (auto wordIndex = 0uz; wordIndex < words.size(); ++wordIndex)
        {
            std::uint32_t word;
            std::memcpy(&word, bytes + wordIndex * sizeof(word), sizeof(word));
            words[wordIndex] = endianness::FromLittleEndian(word);
        }

        header.common = words[0];
        header.referenceCount = words[1];
        header.fence = words[2];
        header.readFence = words[3];
        header.identifier = words[4];
        header.baseFlush = words[5];
        header.mipFlush = words[6];
        for (auto fetchWordIndex = 0uz; fetchWordIndex < header.fetch.words.size(); ++fetchWordIndex)
            header.fetch.words[fetchWordIndex] = words[7u + fetchWordIndex];

        DecodeFetchConstant(header.fetch);
        return true;
    }

    std::array<std::uint8_t, XENON_TEXTURE_HEADER_SIZE> EncodeTextureHeader(const XenonTextureHeader& header)
    {
        const auto fetchWords = EncodeFetchConstant(header.fetch);
        const std::array words{header.common,
                               header.referenceCount,
                               header.fence,
                               header.readFence,
                               header.identifier,
                               header.baseFlush,
                               header.mipFlush,
                               fetchWords[0],
                               fetchWords[1],
                               fetchWords[2],
                               fetchWords[3],
                               fetchWords[4],
                               fetchWords[5]};
        std::array<std::uint8_t, XENON_TEXTURE_HEADER_SIZE> bytes;
        for (auto wordIndex = 0uz; wordIndex < words.size(); ++wordIndex)
        {
            const auto word = endianness::ToLittleEndian(words[wordIndex]);
            std::memcpy(bytes.data() + wordIndex * sizeof(word), &word, sizeof(word));
        }
        return bytes;
    }

    void SetTextureFetchFormat(XenonTextureFetchConstant& fetch, const std::uint32_t rawFormat)
    {
        fetch.dataFormat = static_cast<GPUTEXTUREFORMAT>((rawFormat & D3DFORMAT_TEXTUREFORMAT_MASK) >> D3DFORMAT_TEXTUREFORMAT_SHIFT);
        fetch.endian = static_cast<GPUENDIAN>((rawFormat & D3DFORMAT_ENDIAN_MASK) >> D3DFORMAT_ENDIAN_SHIFT);
        fetch.tiled = (rawFormat & D3DFORMAT_TILED_MASK) >> D3DFORMAT_TILED_SHIFT;
        fetch.signX = static_cast<GPUSIGN>((rawFormat & D3DFORMAT_SIGNX_MASK) >> D3DFORMAT_SIGNX_SHIFT);
        fetch.signY = static_cast<GPUSIGN>((rawFormat & D3DFORMAT_SIGNY_MASK) >> D3DFORMAT_SIGNY_SHIFT);
        fetch.signZ = static_cast<GPUSIGN>((rawFormat & D3DFORMAT_SIGNZ_MASK) >> D3DFORMAT_SIGNZ_SHIFT);
        fetch.signW = static_cast<GPUSIGN>((rawFormat & D3DFORMAT_SIGNW_MASK) >> D3DFORMAT_SIGNW_SHIFT);
        fetch.numFormat = static_cast<GPUNUMFORMAT>((rawFormat & D3DFORMAT_NUMFORMAT_MASK) >> D3DFORMAT_NUMFORMAT_SHIFT);
        fetch.swizzleX = static_cast<GPUSWIZZLE>((rawFormat & D3DFORMAT_SWIZZLEX_MASK) >> D3DFORMAT_SWIZZLEX_SHIFT);
        fetch.swizzleY = static_cast<GPUSWIZZLE>((rawFormat & D3DFORMAT_SWIZZLEY_MASK) >> D3DFORMAT_SWIZZLEY_SHIFT);
        fetch.swizzleZ = static_cast<GPUSWIZZLE>((rawFormat & D3DFORMAT_SWIZZLEZ_MASK) >> D3DFORMAT_SWIZZLEZ_SHIFT);
        fetch.swizzleW = static_cast<GPUSWIZZLE>((rawFormat & D3DFORMAT_SWIZZLEW_MASK) >> D3DFORMAT_SWIZZLEW_SHIFT);
    }

    const XenonTextureFormatInfo* GetTextureFormatInfo(const std::uint32_t dataFormat)
    {
        static const XenonTextureFormatInfo formatBc1{
            GPUTEXTUREFORMAT_DXT1,
            &format::BC1,
            4u,
            4u,
            8u,
            "dxt1",
            false,
        };
        static const XenonTextureFormatInfo formatBc2{
            GPUTEXTUREFORMAT_DXT2_3,
            &format::BC2,
            4u,
            4u,
            16u,
            "dxt2_3",
            false,
        };
        static const XenonTextureFormatInfo formatBc3{
            GPUTEXTUREFORMAT_DXT4_5,
            &format::BC3,
            4u,
            4u,
            16u,
            "dxt4_5",
            false,
        };
        static const XenonTextureFormatInfo formatBc5{
            GPUTEXTUREFORMAT_DXN,
            &format::BC5,
            4u,
            4u,
            16u,
            "dxn",
            false,
        };
        static const XenonTextureFormatInfo formatBgra8{
            GPUTEXTUREFORMAT_8_8_8_8,
            &format::B8_G8_R8_A8,
            1u,
            1u,
            4u,
            "8_8_8_8",
            false,
        };
        static const XenonTextureFormatInfo formatR8{
            GPUTEXTUREFORMAT_8,
            &format::R8,
            1u,
            1u,
            1u,
            "8",
            false,
        };
        static const XenonTextureFormatInfo formatR8A8{
            GPUTEXTUREFORMAT_8_8,
            &format::R8_A8,
            1u,
            1u,
            2u,
            "8_8",
            false,
        };
        static const XenonTextureFormatInfo formatDxt3A{
            GPUTEXTUREFORMAT_DXT3A,
            &format::R8,
            4u,
            4u,
            8u,
            "dxt3a",
            true,
        };
        static const XenonTextureFormatInfo formatDxt5A{
            GPUTEXTUREFORMAT_DXT5A,
            &format::BC4,
            4u,
            4u,
            8u,
            "dxt5a",
            false,
        };

        switch (dataFormat)
        {
        case GPUTEXTUREFORMAT_8:
            return &formatR8;

        case GPUTEXTUREFORMAT_DXT1:
            return &formatBc1;

        case GPUTEXTUREFORMAT_DXT2_3:
            return &formatBc2;

        case GPUTEXTUREFORMAT_DXT4_5:
            return &formatBc3;

        case GPUTEXTUREFORMAT_DXN:
            return &formatBc5;

        case GPUTEXTUREFORMAT_8_8_8_8:
            return &formatBgra8;

        case GPUTEXTUREFORMAT_8_8:
            return &formatR8A8;

        case GPUTEXTUREFORMAT_DXT3A:
            return &formatDxt3A;

        case GPUTEXTUREFORMAT_DXT5A:
            return &formatDxt5A;

        default:
            return nullptr;
        }
    }

    const char* GetTextureFormatName(const std::uint32_t dataFormat)
    {
        const auto* formatInfo = GetTextureFormatInfo(dataFormat);
        return formatInfo != nullptr ? formatInfo->name : "unknown";
    }

    const char* GetEndianName(const std::uint32_t endian)
    {
        switch (endian)
        {
        case GPUENDIAN_NONE:
            return "none";
        case GPUENDIAN_8IN16:
            return "8in16";
        case GPUENDIAN_8IN32:
            return "8in32";
        case GPUENDIAN_16IN32:
            return "16in32";
        default:
            return "unknown";
        }
    }

    const char* GetDimensionName(const std::uint32_t dimension)
    {
        switch (dimension)
        {
        case GPUDIMENSION_1D:
            return "1d";
        case GPUDIMENSION_2D:
            return "2d";
        case GPUDIMENSION_3D:
            return "3d";
        case GPUDIMENSION_CUBEMAP:
            return "cube";
        default:
            return "unknown";
        }
    }

    void ApplyGpuEndian(const std::uint32_t endian, std::uint8_t* data, const std::size_t size)
    {
        switch (endian)
        {
        case GPUENDIAN_8IN16:
            EndianSwap8In16(data, size);
            break;

        case GPUENDIAN_8IN32:
            EndianSwap8In32(data, size);
            break;

        case GPUENDIAN_16IN32:
            EndianSwap16In32(data, size);
            break;

        default:
            break;
        }
    }

    std::vector<std::uint8_t> UntileTextureBlocks(const std::uint8_t* data,
                                                  const std::size_t dataSize,
                                                  const std::uint32_t widthInBlocks,
                                                  const std::uint32_t heightInBlocks,
                                                  const std::uint32_t inputPitchBlocks,
                                                  const std::uint32_t bytesPerBlock)
    {
        const auto log2BytesPerBlock = CalculateLog2BytesPerBlock(bytesPerBlock);
        const auto linearSize = static_cast<std::size_t>(widthInBlocks) * heightInBlocks * bytesPerBlock;
        std::vector<std::uint8_t> destData(linearSize);

        auto outputRowOffset = 0u;
        for (auto y = 0u; y < heightInBlocks; y++)
        {
            const auto inputRowOffset = TiledOffset2DRow(y, inputPitchBlocks, log2BytesPerBlock);
            auto outputOffset = outputRowOffset;

            for (auto x = 0u; x < widthInBlocks; x++)
            {
                auto inputOffset = TiledOffset2DColumn(x, y, log2BytesPerBlock, inputRowOffset);
                inputOffset >>= log2BytesPerBlock;

                const auto srcByteOffset = static_cast<std::size_t>(inputOffset) * bytesPerBlock;
                if (srcByteOffset + bytesPerBlock <= dataSize)
                    std::memcpy(&destData[outputOffset], &data[srcByteOffset], bytesPerBlock);

                outputOffset += bytesPerBlock;
            }

            outputRowOffset += widthInBlocks * bytesPerBlock;
        }

        return destData;
    }

    std::optional<std::size_t> GetTextureBaseSize(const XenonTextureLoadConfig& config)
    {
        XenonTextureHeader header{};
        if (!DecodeTextureHeader(config.textureHeader, config.textureHeaderSize, header) || config.width == 0u || config.height == 0u
            || config.textureType == TextureType::T_3D)
            return std::nullopt;

        const auto* formatInfo = GetTextureFormatInfo(header.fetch.dataFormat);
        if (!formatInfo && !BuildFetchConstantFromRawFormat(config, header.fetch, formatInfo))
            return std::nullopt;

        const auto layout = CalculateBaseLevelLayout(header.fetch, config.width, config.height, *formatInfo);
        const auto size = static_cast<std::uint64_t>(layout.arraySliceStrideBytes) * (config.textureType == TextureType::T_CUBE ? 6u : 1u);
        if (size > std::numeric_limits<std::size_t>::max())
            return std::nullopt;

        return static_cast<std::size_t>(size);
    }

    std::unique_ptr<Texture> LoadTexture(const XenonTextureLoadConfig& config, std::string* error)
    {
        if (config.textureHeader == nullptr)
        {
            SetError(error, "Image has no texture header.");
            return nullptr;
        }

        if (config.pixelData == nullptr || config.pixelDataSize == 0u)
        {
            SetError(error, "Image has no pixel data.");
            return nullptr;
        }

        XenonTextureHeader header{};
        if (!DecodeTextureHeader(config.textureHeader, config.textureHeaderSize, header))
        {
            SetError(error, "Image has an invalid texture header.");
            return nullptr;
        }

        if (config.width == 0u || config.height == 0u)
        {
            SetError(error, "Image has invalid dimensions.");
            return nullptr;
        }

        auto* formatInfo = GetTextureFormatInfo(header.fetch.dataFormat);
        if (formatInfo == nullptr)
        {
            if (config.rawFormat == 0u || !BuildFetchConstantFromRawFormat(config, header.fetch, formatInfo))
            {
                SetError(error, "Unsupported Xenon image format.");
                return nullptr;
            }
        }

        if (config.textureType == TextureType::T_3D)
        {
            SetError(error, "3D Xenon textures are not supported.");
            return nullptr;
        }

        std::vector<std::uint8_t> sourceData(config.pixelData, config.pixelData + config.pixelDataSize);
        ApplyGpuEndian(header.fetch.endian, sourceData.data(), sourceData.size());

        const auto faceCount = config.textureType == TextureType::T_CUBE ? 6u : 1u;
        const auto levelCount = std::max(1u, config.levelCount);
        const auto baseLayout = CalculateBaseLevelLayout(header.fetch, config.width, config.height, *formatInfo);
        const auto isTiled = header.fetch.tiled != 0u;
        const auto packedLevel =
            header.fetch.packedMips != 0u && levelCount > 0u ? GetPackedMipLevel(config.width, config.height) : std::numeric_limits<std::uint32_t>::max();
        const auto mipRegionStart = header.fetch.mipAddress << GPU_TEXTURE_ADDRESS_SHIFT;
        const auto hasMipMaps = levelCount > 1u;

        auto outputFormat = *formatInfo;
        // A8 and L8 share the GPU's 8-bit storage format. The swizzle selects
        // whether that byte is sampled as alpha or luminance.
        if (header.fetch.dataFormat == GPUTEXTUREFORMAT_8 && header.fetch.swizzleX == GPUSWIZZLE_0 && header.fetch.swizzleY == GPUSWIZZLE_0
            && header.fetch.swizzleZ == GPUSWIZZLE_0 && header.fetch.swizzleW == GPUSWIZZLE_X)
            outputFormat.imageFormat = &format::A8;

        auto texture = CreateTextureStorage(outputFormat, config.textureType, config.width, config.height, config.depth, hasMipMaps);
        if (!texture)
        {
            SetError(error, "Failed to allocate texture storage.");
            return nullptr;
        }

        const auto maxTextureLevel = static_cast<std::uint32_t>(texture->GetMipMapCount());
        const auto storedLevelCount = std::min(levelCount, maxTextureLevel);
        const auto baseRegionSizeRequired = static_cast<std::uint64_t>(baseLayout.arraySliceStrideBytes) * faceCount;
        if (baseRegionSizeRequired > sourceData.size())
        {
            SetError(error, "Image base data is smaller than expected.");
            return nullptr;
        }

        std::vector<XenonTextureLevelLayout> mipLayouts(storedLevelCount);
        std::vector<std::uint32_t> mipOffsets(storedLevelCount, 0u);
        auto mipTailRegionOffset = 0u;
        for (auto mipLevel = 1u; mipLevel < storedLevelCount; ++mipLevel)
        {
            mipLayouts[mipLevel] = CalculateMipLevelLayout(config.width, config.height, mipLevel, *formatInfo);
            if (packedLevel == std::numeric_limits<std::uint32_t>::max() || mipLevel < packedLevel)
            {
                mipOffsets[mipLevel] = mipTailRegionOffset;
                mipTailRegionOffset += mipLayouts[mipLevel].arraySliceStrideBytes * faceCount;
            }
        }

        std::vector<std::vector<std::uint8_t>> basePackedLinear(faceCount);
        std::vector<std::vector<std::uint8_t>> mipPackedLinear(faceCount);

        for (auto faceIndex = 0u; faceIndex < faceCount; ++faceIndex)
        {
            if (packedLevel == 0u)
            {
                const auto faceOffset = faceIndex * baseLayout.arraySliceStrideBytes;
                const auto faceDataSize = std::min<std::size_t>(baseLayout.arraySliceStrideBytes, sourceData.size() - faceOffset);
                basePackedLinear[faceIndex] = isTiled
                                                  ? UntileTextureBlocks(sourceData.data() + faceOffset,
                                                                        faceDataSize,
                                                                        baseLayout.rowPitchBytes / formatInfo->bytesPerBlock,
                                                                        baseLayout.zSliceStrideBlockRows,
                                                                        baseLayout.rowPitchBytes / formatInfo->bytesPerBlock,
                                                                        formatInfo->bytesPerBlock)
                                                  : std::vector<std::uint8_t>(sourceData.begin() + faceOffset, sourceData.begin() + faceOffset + faceDataSize);
            }

            if (packedLevel != std::numeric_limits<std::uint32_t>::max() && packedLevel < storedLevelCount && (packedLevel > 0u || mipRegionStart != 0u))
            {
                const auto packedLayout = packedLevel == 0u ? CalculateMipLevelLayout(config.width, config.height, 0u, *formatInfo) : mipLayouts[packedLevel];
                const auto packedRegionOffset = packedLevel == 0u ? 0u : mipTailRegionOffset;
                const auto faceOffset = mipRegionStart + packedRegionOffset + faceIndex * packedLayout.arraySliceStrideBytes;

                if (faceOffset >= sourceData.size())
                {
                    SetError(error, "Image packed mip tail offset is out of range.");
                    return nullptr;
                }

                const auto faceDataSize = std::min<std::size_t>(packedLayout.arraySliceStrideBytes, sourceData.size() - faceOffset);
                mipPackedLinear[faceIndex] = isTiled
                                                 ? UntileTextureBlocks(sourceData.data() + faceOffset,
                                                                       faceDataSize,
                                                                       packedLayout.rowPitchBytes / formatInfo->bytesPerBlock,
                                                                       packedLayout.zSliceStrideBlockRows,
                                                                       packedLayout.rowPitchBytes / formatInfo->bytesPerBlock,
                                                                       formatInfo->bytesPerBlock)
                                                 : std::vector<std::uint8_t>(sourceData.begin() + faceOffset, sourceData.begin() + faceOffset + faceDataSize);
            }

            for (auto mipLevel = 0u; mipLevel < storedLevelCount; ++mipLevel)
            {
                const auto mipWidth = std::max(config.width >> mipLevel, 1u);
                const auto mipHeight = std::max(config.height >> mipLevel, 1u);
                const auto widthBlocks = std::max(1u, DivideRoundUp(mipWidth, formatInfo->blockWidth));
                const auto heightBlocks = std::max(1u, DivideRoundUp(mipHeight, formatInfo->blockHeight));
                auto* destination = texture->GetBufferForMipLevel(static_cast<int>(mipLevel), static_cast<int>(faceIndex));

                if (packedLevel != std::numeric_limits<std::uint32_t>::max() && mipLevel >= packedLevel)
                {
                    std::uint32_t xBlocks;
                    std::uint32_t yBlocks;
                    if (!GetPackedMipOffsetBlocks(config.width, config.height, *formatInfo, mipLevel, xBlocks, yBlocks))
                    {
                        SetError(error, "Failed to calculate packed mip offset.");
                        return nullptr;
                    }

                    const auto useBasePackedSurface = packedLevel == 0u && (mipLevel == 0u || mipRegionStart == 0u);
                    const auto& packedSurface = useBasePackedSurface ? basePackedLinear[faceIndex] : mipPackedLinear[faceIndex];
                    const auto packedLayout = useBasePackedSurface ? baseLayout
                                                                   : (packedLevel == 0u ? CalculateMipLevelLayout(config.width, config.height, 0u, *formatInfo)
                                                                                        : mipLayouts[packedLevel]);
                    if (packedSurface.empty())
                    {
                        SetError(error, "Packed mip surface is missing.");
                        return nullptr;
                    }

                    if (formatInfo->expandDxt3Alpha)
                    {
                        std::vector<std::uint8_t> compressedLevel(static_cast<std::size_t>(widthBlocks) * heightBlocks * formatInfo->bytesPerBlock);
                        if (!CopyBlockRect(packedSurface.data(),
                                           packedSurface.size(),
                                           packedLayout.rowPitchBytes / formatInfo->bytesPerBlock,
                                           xBlocks,
                                           yBlocks,
                                           compressedLevel.data(),
                                           widthBlocks,
                                           widthBlocks,
                                           heightBlocks,
                                           formatInfo->bytesPerBlock))
                        {
                            SetError(error, "Packed mip rectangle extends beyond its surface.");
                            return nullptr;
                        }
                        DecodeDxt3AlphaBlocksToR8(compressedLevel.data(), widthBlocks, heightBlocks, mipWidth, mipHeight, destination);
                    }
                    else
                    {
                        if (!CopyBlockRect(packedSurface.data(),
                                           packedSurface.size(),
                                           packedLayout.rowPitchBytes / formatInfo->bytesPerBlock,
                                           xBlocks,
                                           yBlocks,
                                           destination,
                                           widthBlocks,
                                           widthBlocks,
                                           heightBlocks,
                                           formatInfo->bytesPerBlock))
                        {
                            SetError(error, "Packed mip rectangle extends beyond its surface.");
                            return nullptr;
                        }
                    }
                    continue;
                }

                if (mipLevel == 0u)
                {
                    const auto faceOffset = faceIndex * baseLayout.arraySliceStrideBytes;
                    const auto faceDataSize = std::min<std::size_t>(baseLayout.arraySliceStrideBytes, sourceData.size() - faceOffset);
                    if (formatInfo->expandDxt3Alpha)
                    {
                        std::vector<std::uint8_t> compressedLevel(static_cast<std::size_t>(widthBlocks) * heightBlocks * formatInfo->bytesPerBlock);
                        if (!ExtractLevelFromStoredSurface(sourceData.data() + faceOffset,
                                                           faceDataSize,
                                                           isTiled,
                                                           baseLayout.rowPitchBytes / formatInfo->bytesPerBlock,
                                                           baseLayout.zSliceStrideBlockRows,
                                                           0u,
                                                           0u,
                                                           compressedLevel.data(),
                                                           widthBlocks,
                                                           widthBlocks,
                                                           heightBlocks,
                                                           formatInfo->bytesPerBlock))
                        {
                            SetError(error, "Failed to extract base level.");
                            return nullptr;
                        }

                        DecodeDxt3AlphaBlocksToR8(compressedLevel.data(), widthBlocks, heightBlocks, mipWidth, mipHeight, destination);
                        continue;
                    }

                    if (!ExtractLevelFromStoredSurface(sourceData.data() + faceOffset,
                                                       faceDataSize,
                                                       isTiled,
                                                       baseLayout.rowPitchBytes / formatInfo->bytesPerBlock,
                                                       baseLayout.zSliceStrideBlockRows,
                                                       0u,
                                                       0u,
                                                       destination,
                                                       widthBlocks,
                                                       widthBlocks,
                                                       heightBlocks,
                                                       formatInfo->bytesPerBlock))
                    {
                        SetError(error, "Failed to extract base level.");
                        return nullptr;
                    }
                    continue;
                }

                const auto& mipLayout = mipLayouts[mipLevel];
                const auto levelOffset = mipRegionStart + mipOffsets[mipLevel] + faceIndex * mipLayout.arraySliceStrideBytes;
                if (levelOffset >= sourceData.size())
                {
                    SetError(error, "Mip level offset is out of range.");
                    return nullptr;
                }

                const auto levelDataSize = std::min<std::size_t>(mipLayout.arraySliceStrideBytes, sourceData.size() - levelOffset);
                if (formatInfo->expandDxt3Alpha)
                {
                    std::vector<std::uint8_t> compressedLevel(static_cast<std::size_t>(widthBlocks) * heightBlocks * formatInfo->bytesPerBlock);
                    if (!ExtractLevelFromStoredSurface(sourceData.data() + levelOffset,
                                                       levelDataSize,
                                                       isTiled,
                                                       mipLayout.rowPitchBytes / formatInfo->bytesPerBlock,
                                                       mipLayout.zSliceStrideBlockRows,
                                                       0u,
                                                       0u,
                                                       compressedLevel.data(),
                                                       widthBlocks,
                                                       widthBlocks,
                                                       heightBlocks,
                                                       formatInfo->bytesPerBlock))
                    {
                        SetError(error, "Failed to extract mip level.");
                        return nullptr;
                    }

                    DecodeDxt3AlphaBlocksToR8(compressedLevel.data(), widthBlocks, heightBlocks, mipWidth, mipHeight, destination);
                    continue;
                }

                if (!ExtractLevelFromStoredSurface(sourceData.data() + levelOffset,
                                                   levelDataSize,
                                                   isTiled,
                                                   mipLayout.rowPitchBytes / formatInfo->bytesPerBlock,
                                                   mipLayout.zSliceStrideBlockRows,
                                                   0u,
                                                   0u,
                                                   destination,
                                                   widthBlocks,
                                                   widthBlocks,
                                                   heightBlocks,
                                                   formatInfo->bytesPerBlock))
                {
                    SetError(error, "Failed to extract mip level.");
                    return nullptr;
                }
            }
        }

        return texture;
    }
} // namespace image::xenon
