#pragma once

#include "Game/IGame.h"
#include "Game/IW4/IW4.h"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace IW4
{
    namespace platform_asset_type
    {
        inline constexpr int UNSUPPORTED = -1;

        constexpr std::array<int, ASSET_TYPE_COUNT> CreatePcMapping()
        {
            std::array<int, ASSET_TYPE_COUNT> result{};
            for (auto index = 0u; index < result.size(); index++)
                result[index] = static_cast<int>(index);
            return result;
        }

        constexpr std::array<int, ASSET_TYPE_COUNT> CreateXboxMapping()
        {
            std::array<int, ASSET_TYPE_COUNT> result{};
            result.fill(UNSUPPORTED);

            // Only asset layouts verified compatible with the shared IW4
            // generated loaders are mapped in this experiment.
            result[ASSET_TYPE_PHYSPRESET] = 0;
            result[ASSET_TYPE_PHYSCOLLMAP] = 1;
            result[ASSET_TYPE_SOUND_CURVE] = 10;
            result[ASSET_TYPE_MAP_ENTS] = 17;
            result[ASSET_TYPE_LIGHT_DEF] = 20;
            result[ASSET_TYPE_LOCALIZE_ENTRY] = 25;
            result[ASSET_TYPE_RAWFILE] = 34;
            result[ASSET_TYPE_STRINGTABLE] = 35;
            result[ASSET_TYPE_LEADERBOARD] = 36;
            result[ASSET_TYPE_TRACER] = 38;
            result[ASSET_TYPE_ADDON_MAP_ENTS] = 40;

            return result;
        }
    } // namespace platform_asset_type

    struct PlatformTraits
    {
        GameEndianness m_endianness;
        bool m_has_vertex_and_index_blocks;
        std::array<int, ASSET_TYPE_COUNT> m_serialized_asset_types;

        [[nodiscard]] constexpr int GetSerializedAssetType(const XAssetType logicalAssetType) const
        {
            return m_serialized_asset_types[static_cast<std::size_t>(logicalAssetType)];
        }
    };

    inline constexpr PlatformTraits PLATFORM_TRAITS_PC{
        .m_endianness = GameEndianness::LE,
        .m_has_vertex_and_index_blocks = true,
        .m_serialized_asset_types = platform_asset_type::CreatePcMapping(),
    };

    inline constexpr PlatformTraits PLATFORM_TRAITS_XBOX{
        .m_endianness = GameEndianness::BE,
        .m_has_vertex_and_index_blocks = false,
        .m_serialized_asset_types = platform_asset_type::CreateXboxMapping(),
    };

    inline const PlatformTraits& GetPlatformTraits(const GamePlatform platform)
    {
        switch (platform)
        {
        case GamePlatform::PC:
            return PLATFORM_TRAITS_PC;

        case GamePlatform::XBOX:
            return PLATFORM_TRAITS_XBOX;

        default:
            throw std::invalid_argument("IW4 does not define traits for this platform");
        }
    }
} // namespace IW4
