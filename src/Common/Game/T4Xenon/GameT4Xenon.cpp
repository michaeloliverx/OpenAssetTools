#include "GameT4Xenon.h"

#include "T4Xenon.h"

#include <algorithm>

using namespace T4Xenon;

namespace
{
    constexpr const char* ASSET_TYPE_NAMES[]{
        "xmodelpieces",
        "physpreset",
        "physconstraints",
        "destructibledef",
        "xanim",
        "xmodel",
        "material",
        "pixelshader",
        "techniqueset",
        "image",
        "sound",
        "loadedsound",
        "clipmap_unused",
        "clipmap",
        "comworld",
        "gameworldsp",
        "gameworldmp",
        "mapents",
        "gfxworld",
        "lightdef",
        "uimap",
        "font",
        "menulist",
        "menu",
        "localize",
        "weapon",
        "snddriverglobals",
        "fx",
        "impactfx",
        "aitype",
        "mptype",
        "character",
        "xmodelalias",
        "rawfile",
        "stringtable",
        "packindex",
    };
    static_assert(std::extent_v<decltype(ASSET_TYPE_NAMES)> == ASSET_TYPE_COUNT);

    constexpr const char* SUB_ASSET_TYPE_NAMES[]{
        "technique",
        "vertexdecl",
        "vertexshader",
        "accuracygraph",
        "flametable",
    };
    static_assert(std::extent_v<decltype(SUB_ASSET_TYPE_NAMES)> == SUB_ASSET_TYPE_COUNT);
} // namespace

namespace T4Xenon
{
    Game::Game()
        : AbstractGame(ASSET_TYPE_NAMES, std::extent_v<decltype(ASSET_TYPE_NAMES)>, SUB_ASSET_TYPE_NAMES, std::extent_v<decltype(SUB_ASSET_TYPE_NAMES)>)
    {
        // Keep T4's public .zone vocabulary platform-neutral. The database-style
        // names are retained as aliases for compatibility with native zone dumps.
        AddAssetTypeNameAlias<AssetTechniqueSet>("techset");
        AddAssetTypeNameAlias<AssetLoadedSound>("loaded_sound");
        AddAssetTypeNameAlias<AssetClipMap>("col_map_sp");
        AddAssetTypeNameAlias<AssetClipMapPvs>("col_map_mp");
        AddAssetTypeNameAlias<AssetComWorld>("com_map");
        AddAssetTypeNameAlias<AssetGameWorldSp>("game_map_sp");
        AddAssetTypeNameAlias<AssetGameWorldMp>("game_map_mp");
        AddAssetTypeNameAlias<AssetMapEnts>("map_ents");
        AddAssetTypeNameAlias<AssetGfxWorld>("gfx_map");
        AddAssetTypeNameAlias<AssetLightDef>("gfxlightdef");
        AddAssetTypeNameAlias<AssetMenuList>("menufile");
    }

    GameId Game::GetId() const
    {
        return GameId::T4Xenon;
    }

    const std::string& Game::GetFullName() const
    {
        static std::string fullName = "Call of Duty: World at War (Xenon)";
        return fullName;
    }

    const std::string& Game::GetShortName() const
    {
        static std::string shortName = "T4Xenon";
        return shortName;
    }

    const std::vector<GameLanguagePrefix>& Game::GetLanguagePrefixes() const
    {
        static std::vector<GameLanguagePrefix> prefixes;
        return prefixes;
    }

    asset_type_t Game::GetAssetTypeCount() const
    {
        return ASSET_TYPE_COUNT;
    }

    std::optional<const char*> Game::GetAssetTypeName(const asset_type_t assetType) const
    {
        if (assetType < std::extent_v<decltype(ASSET_TYPE_NAMES)>)
            return ASSET_TYPE_NAMES[assetType];

        return std::nullopt;
    }

    asset_type_t Game::GetSubAssetTypeCount() const
    {
        return std::extent_v<decltype(SUB_ASSET_TYPE_NAMES)>;
    }

    std::optional<const char*> Game::GetSubAssetTypeName(const asset_type_t subAssetType) const
    {
        if (subAssetType < std::extent_v<decltype(SUB_ASSET_TYPE_NAMES)>)
            return SUB_ASSET_TYPE_NAMES[subAssetType];

        return std::nullopt;
    }
} // namespace T4Xenon
