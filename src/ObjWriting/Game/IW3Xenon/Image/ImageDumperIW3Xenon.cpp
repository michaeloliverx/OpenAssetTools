#include "ImageDumperIW3Xenon.h"

#include "Image/DdsWriter.h"
#include "Image/Dx9TextureLoader.h"
#include "Image/ImageCommon.h"
#include "Image/ImageFormat.h"
#include "Image/IwiWriter6.h"
#include "Image/Texture.h"
#include "ObjWriting.h"

#include <algorithm>
#include <cassert>
#include <cstring>

using namespace IW3Xenon;

namespace xenon
{
    // Xbox 360 texture untiling (from Xenia emulator - texture_conversion.cc)
    // https://github.com/xenia-project/xenia/blob/master/src/xenia/gpu/texture_conversion.cc

    static uint32_t TiledOffset2DRow(uint32_t y, uint32_t width, uint32_t log2_bpp)
    {
        uint32_t macro = ((y / 32) * (width / 32)) << (log2_bpp + 7);
        uint32_t micro = ((y & 6) << 2) << log2_bpp;
        return macro + ((micro & ~0xFu) << 1) + (micro & 0xF) + ((y & 8) << (3 + log2_bpp)) + ((y & 1) << 4);
    }

    static uint32_t TiledOffset2DColumn(uint32_t x, uint32_t y, uint32_t log2_bpp, uint32_t base_offset)
    {
        uint32_t macro = (x / 32) << (log2_bpp + 7);
        uint32_t micro = (x & 7) << log2_bpp;
        uint32_t offset = base_offset + (macro + ((micro & ~0xFu) << 1) + (micro & 0xF));
        return ((offset & ~0x1FFu) << 3) + ((offset & 0x1C0) << 2) + (offset & 0x3F) + ((y & 16) << 7) + (((((y & 8) >> 2) + (x >> 3)) & 3) << 6);
    }

    std::vector<uint8_t> UntileTexture(const uint8_t* data, size_t dataSize, uint32_t width, uint32_t height, uint32_t bytesPerBlock)
    {
        uint32_t widthInBlocks = width / 4;
        uint32_t heightInBlocks = height / 4;

        // Calculate log2 of bytes per block (same formula as Xenia)
        uint32_t log2_bpp = (bytesPerBlock / 4) + ((bytesPerBlock / 2) >> (bytesPerBlock / 4));

        // Input pitch aligned to 32 blocks for tiled addressing
        uint32_t inputPitch = (widthInBlocks + 31) & ~31u;

        size_t linearSize = widthInBlocks * heightInBlocks * bytesPerBlock;
        std::vector<uint8_t> destData(linearSize);

        uint32_t outputRowOffset = 0;
        for (uint32_t y = 0; y < heightInBlocks; y++)
        {
            uint32_t inputRowOffset = TiledOffset2DRow(y, inputPitch, log2_bpp);
            uint32_t outputOffset = outputRowOffset;

            for (uint32_t x = 0; x < widthInBlocks; x++)
            {
                uint32_t inputOffset = TiledOffset2DColumn(x, y, log2_bpp, inputRowOffset);
                inputOffset >>= log2_bpp;

                size_t srcByteOffset = inputOffset * bytesPerBlock;
                if (srcByteOffset + bytesPerBlock <= dataSize)
                {
                    std::memcpy(&destData[outputOffset], &data[srcByteOffset], bytesPerBlock);
                }

                outputOffset += bytesPerBlock;
            }

            outputRowOffset += widthInBlocks * bytesPerBlock;
        }

        return destData;
    }

    void EndianSwap_8In16(uint8_t* data, size_t size)
    {
        for (size_t i = 0; i + 1 < size; i += 2)
            std::swap(data[i], data[i + 1]);
    }

    void EndianSwap_8In32(uint8_t* data, size_t size)
    {
        for (size_t i = 0; i + 3 < size; i += 4)
        {
            std::swap(data[i], data[i + 3]);
            std::swap(data[i + 1], data[i + 2]);
        }
    }

    void EndianSwap_16In32(uint8_t* data, size_t size)
    {
        for (size_t i = 0; i + 3 < size; i += 4)
        {
            std::swap(data[i], data[i + 2]);
            std::swap(data[i + 1], data[i + 3]);
        }
    }

    void ApplyGpuEndian(GPUENDIAN endian, uint8_t* data, size_t size)
    {
        switch (endian)
        {
        case GPUENDIAN_8IN16:
            EndianSwap_8In16(data, size);
            break;
        case GPUENDIAN_8IN32:
            EndianSwap_8In32(data, size);
            break;
        case GPUENDIAN_16IN32:
            EndianSwap_16In32(data, size);
            break;
        default:
            break;
        }
    }

} // namespace xenon

namespace
{
    std::unique_ptr<Texture> LoadImageFromLoadDef(const GfxImage& image)
    {
        const auto& loadDef = *image.texture.loadDef;

        Dx9TextureLoader textureLoader;
        textureLoader.Width(image.width).Height(image.height).Depth(1);

        if (loadDef.flags & IMG_FLAG_VOLMAP)
            textureLoader.Type(TextureType::T_3D);
        else if (loadDef.flags & IMG_FLAG_CUBEMAP)
            textureLoader.Type(TextureType::T_CUBE);
        else
            textureLoader.Type(TextureType::T_2D);

        const auto format = loadDef.format;
        const auto endian = static_cast<GPUENDIAN>((format >> 6) & 0x3);
        const auto tiledDataSize = image.cardMemory.platform[0]; // Actual tiled memory size
        xenon::ApplyGpuEndian(endian, image.pixels, tiledDataSize);

        // DXT1 (BC1) - 8 bytes per block
        if (format == 0x1A200152)
        {
            textureLoader.Format(oat::D3DFMT_DXT1);
            auto linear = xenon::UntileTexture(image.pixels, tiledDataSize, image.width, image.height, 8);
            return textureLoader.LoadTexture(linear.data());
        }

        // DXT3 (BC2) - 16 bytes per block
        if (format == 0x1A200153)
        {
            textureLoader.Format(oat::D3DFMT_DXT3);
            auto linear = xenon::UntileTexture(image.pixels, tiledDataSize, image.width, image.height, 16);
            return textureLoader.LoadTexture(linear.data());
        }

        // DXT5 (BC3) - 16 bytes per block
        if (format == 0x1A200154)
        {
            textureLoader.Format(oat::D3DFMT_DXT5);
            auto linear = xenon::UntileTexture(image.pixels, tiledDataSize, image.width, image.height, 16);
            return textureLoader.LoadTexture(linear.data());
        }

        // BC5 (ATI2/3Dc) - 16 bytes per block - no D3D9 format, must create texture directly
        if (format == 0x1A200171)
        {
            auto linear = xenon::UntileTexture(image.pixels, tiledDataSize, image.width, image.height, 16);
            auto texture = std::make_unique<Texture2D>(&ImageFormat::FORMAT_BC5, image.width, image.height);
            texture->Allocate();
            std::memcpy(texture->GetBufferForMipLevel(0, 0), linear.data(), linear.size());
            return texture;
        }

        return nullptr;
    }
} // namespace

namespace image
{
    DumperIW3Xenon::DumperIW3Xenon(const AssetPool<AssetImage::Type>& pool)
        : AbstractAssetDumper(pool)
    {
        switch (ObjWriting::Configuration.ImageOutputFormat)
        {
        case ObjWriting::Configuration_t::ImageOutputFormat_e::DDS:
            m_writer = std::make_unique<DdsWriter>();
            break;
        case ObjWriting::Configuration_t::ImageOutputFormat_e::IWI:
            m_writer = std::make_unique<iwi6::IwiWriter>();
            break;
        default:
            assert(false);
            m_writer = nullptr;
            break;
        }
    }

    void DumperIW3Xenon::DumpAsset(AssetDumpingContext& context, const XAssetInfo<GfxImage>& asset)
    {
        const auto* image = asset.Asset();
        std::printf("Dumping image asset: %s\n", asset.m_name.c_str());
        if (image->mapType != MAPTYPE_2D)
        {
            std::printf("Only 2D images are supported?\n");
            return;
        }
        if (image->texture.loadDef->dimensions[0] == -1)
        {
            std::printf("Image has invalid dimensions?\n");
            return;
        }

        const auto texture = LoadImageFromLoadDef(*image);
        if (!texture)
            return;

        const auto assetFile = context.OpenAssetFile(GetFileNameForAsset(asset.m_name, m_writer->GetFileExtension()));
        if (!assetFile)
            return;

        m_writer->DumpImage(*assetFile, texture.get());
    }
} // namespace image
