#pragma once

#include "XenonGraphics.h"

#include <cstdint>

namespace oat::xenon
{
    // D3DFORMAT identifiers pack different fields from GPU fetch constants.
    // These shifts and masks match d3d9types.h in the Xbox 360 SDK.
    inline constexpr std::uint32_t D3DFORMAT_TEXTUREFORMAT_SHIFT = 0u;
    inline constexpr std::uint32_t D3DFORMAT_ENDIAN_SHIFT = 6u;
    inline constexpr std::uint32_t D3DFORMAT_TILED_SHIFT = 8u;
    inline constexpr std::uint32_t D3DFORMAT_SIGNX_SHIFT = 9u;
    inline constexpr std::uint32_t D3DFORMAT_SIGNY_SHIFT = 11u;
    inline constexpr std::uint32_t D3DFORMAT_SIGNZ_SHIFT = 13u;
    inline constexpr std::uint32_t D3DFORMAT_SIGNW_SHIFT = 15u;
    inline constexpr std::uint32_t D3DFORMAT_NUMFORMAT_SHIFT = 17u;
    inline constexpr std::uint32_t D3DFORMAT_SWIZZLEX_SHIFT = 18u;
    inline constexpr std::uint32_t D3DFORMAT_SWIZZLEY_SHIFT = 21u;
    inline constexpr std::uint32_t D3DFORMAT_SWIZZLEZ_SHIFT = 24u;
    inline constexpr std::uint32_t D3DFORMAT_SWIZZLEW_SHIFT = 27u;

    inline constexpr std::uint32_t D3DFORMAT_TEXTUREFORMAT_MASK = 0x0000003fu;
    inline constexpr std::uint32_t D3DFORMAT_ENDIAN_MASK = 0x000000c0u;
    inline constexpr std::uint32_t D3DFORMAT_TILED_MASK = 0x00000100u;
    inline constexpr std::uint32_t D3DFORMAT_SIGNX_MASK = 0x00000600u;
    inline constexpr std::uint32_t D3DFORMAT_SIGNY_MASK = 0x00001800u;
    inline constexpr std::uint32_t D3DFORMAT_SIGNZ_MASK = 0x00006000u;
    inline constexpr std::uint32_t D3DFORMAT_SIGNW_MASK = 0x00018000u;
    inline constexpr std::uint32_t D3DFORMAT_NUMFORMAT_MASK = 0x00020000u;
    inline constexpr std::uint32_t D3DFORMAT_SWIZZLEX_MASK = 0x001c0000u;
    inline constexpr std::uint32_t D3DFORMAT_SWIZZLEY_MASK = 0x00e00000u;
    inline constexpr std::uint32_t D3DFORMAT_SWIZZLEZ_MASK = 0x07000000u;
    inline constexpr std::uint32_t D3DFORMAT_SWIZZLEW_MASK = 0x38000000u;

    constexpr std::uint32_t MakeSwizzle(const GPUSWIZZLE x, const GPUSWIZZLE y, const GPUSWIZZLE z, const GPUSWIZZLE w)
    {
        return x | (y << 3u) | (z << 6u) | (w << 9u);
    }

    inline constexpr std::uint32_t GPUSWIZZLE_ARGB = MakeSwizzle(GPUSWIZZLE_Z, GPUSWIZZLE_Y, GPUSWIZZLE_X, GPUSWIZZLE_W);
    inline constexpr std::uint32_t GPUSWIZZLE_ABGR = MakeSwizzle(GPUSWIZZLE_X, GPUSWIZZLE_Y, GPUSWIZZLE_Z, GPUSWIZZLE_W);
    inline constexpr std::uint32_t GPUSWIZZLE_RZZZ = MakeSwizzle(GPUSWIZZLE_0, GPUSWIZZLE_0, GPUSWIZZLE_0, GPUSWIZZLE_X);
    inline constexpr std::uint32_t GPUSWIZZLE_ORRR = MakeSwizzle(GPUSWIZZLE_X, GPUSWIZZLE_X, GPUSWIZZLE_X, GPUSWIZZLE_1);
    inline constexpr std::uint32_t GPUSWIZZLE_GRRR = MakeSwizzle(GPUSWIZZLE_X, GPUSWIZZLE_X, GPUSWIZZLE_X, GPUSWIZZLE_Y);

    constexpr std::uint32_t MakeD3DFormat(const GPUTEXTUREFORMAT format, const GPUENDIAN endian, const std::uint32_t swizzle)
    {
        return (format << D3DFORMAT_TEXTUREFORMAT_SHIFT) | (endian << D3DFORMAT_ENDIAN_SHIFT) | D3DFORMAT_TILED_MASK | (swizzle << D3DFORMAT_SWIZZLEX_SHIFT);
    }

    // Supported tiled formats, with unsigned fractional channels.
    enum D3DFORMAT : std::uint32_t
    {
        D3DFMT_DXT1 = MakeD3DFormat(GPUTEXTUREFORMAT_DXT1, GPUENDIAN_8IN16, GPUSWIZZLE_ABGR),
        D3DFMT_DXT3 = MakeD3DFormat(GPUTEXTUREFORMAT_DXT2_3, GPUENDIAN_8IN16, GPUSWIZZLE_ABGR),
        D3DFMT_DXT5 = MakeD3DFormat(GPUTEXTUREFORMAT_DXT4_5, GPUENDIAN_8IN16, GPUSWIZZLE_ABGR),
        D3DFMT_DXT3A = MakeD3DFormat(GPUTEXTUREFORMAT_DXT3A, GPUENDIAN_8IN16, GPUSWIZZLE_ABGR),
        D3DFMT_DXT5A = MakeD3DFormat(GPUTEXTUREFORMAT_DXT5A, GPUENDIAN_8IN16, GPUSWIZZLE_ABGR),
        D3DFMT_DXN = MakeD3DFormat(GPUTEXTUREFORMAT_DXN, GPUENDIAN_8IN16, GPUSWIZZLE_ABGR),
        D3DFMT_A8R8G8B8 = MakeD3DFormat(GPUTEXTUREFORMAT_8_8_8_8, GPUENDIAN_8IN32, GPUSWIZZLE_ARGB),
        D3DFMT_L8 = MakeD3DFormat(GPUTEXTUREFORMAT_8, GPUENDIAN_NONE, GPUSWIZZLE_ORRR),
        D3DFMT_A8 = MakeD3DFormat(GPUTEXTUREFORMAT_8, GPUENDIAN_NONE, GPUSWIZZLE_RZZZ),
        D3DFMT_A8L8 = MakeD3DFormat(GPUTEXTUREFORMAT_8_8, GPUENDIAN_8IN16, GPUSWIZZLE_GRRR),
    };
} // namespace oat::xenon
