#include "GameIW4Xenon482.h"

#include "IW4Xenon482.h"

#include <algorithm>

using namespace IW4Xenon482;

namespace
{
    constexpr const char* ASSET_TYPE_NAMES[]{
        "physpreset",   "physcollmap", "xanim",          "xmodelsurfs", "xmodel",   "material",    "pixelshader", "techniqueset",      "image",   "sound",
        "soundcurve",   "loadedsound", "clipmap_unused", "clipmap",     "comworld", "gameworldsp", "gameworldmp", "mapents",           "fxworld", "gfxworld",
        "lightdef",     "uimap",       "font",           "menulist",    "menu",     "localize",    "weapon",      "snddriverglobals",  "fx",      "impactfx",
        "aitype",       "mptype",      "character",      "xmodelalias", "rawfile",  "stringtable", "leaderboard", "structureddatadef", "tracer",  "vehicle",
        "addonmapents",
    };
    static_assert(std::extent_v<decltype(ASSET_TYPE_NAMES)> == ASSET_TYPE_COUNT);

    constexpr const char* SUB_ASSET_TYPE_NAMES[]{
        "technique",
        "vertexdecl",
        "vertexshader",
        "accuracygraph",
    };
    static_assert(std::extent_v<decltype(SUB_ASSET_TYPE_NAMES)> == SUB_ASSET_TYPE_COUNT);
} // namespace

namespace IW4Xenon482
{
    Game::Game()
        : AbstractGame(ASSET_TYPE_NAMES, std::extent_v<decltype(ASSET_TYPE_NAMES)>, SUB_ASSET_TYPE_NAMES, std::extent_v<decltype(SUB_ASSET_TYPE_NAMES)>)
    {
        AddAssetTypeNameAlias<AssetTechniqueSet>("techset");
        AddAssetTypeNameAlias<AssetLightDef>("gfxlightdef");
        AddAssetTypeNameAlias<AssetPhysCollMap>("phys_collmap");
        AddAssetTypeNameAlias<AssetSoundCurve>("sndcurve");
        AddAssetTypeNameAlias<AssetLoadedSound>("loaded_sound");
        AddAssetTypeNameAlias<AssetClipMapSp>("col_map_sp");
        AddAssetTypeNameAlias<AssetClipMapMp>("col_map_mp");
        AddAssetTypeNameAlias<AssetComWorld>("com_map");
        AddAssetTypeNameAlias<AssetGameWorldSp>("game_map_sp");
        AddAssetTypeNameAlias<AssetGameWorldMp>("game_map_mp");
        AddAssetTypeNameAlias<AssetMapEnts>("map_ents");
        AddAssetTypeNameAlias<AssetFxWorld>("fx_map");
        AddAssetTypeNameAlias<AssetGfxWorld>("gfx_map");
        AddAssetTypeNameAlias<AssetMenuList>("menufile");
        AddAssetTypeNameAlias<AssetLeaderboard>("leaderboarddef");
        AddAssetTypeNameAlias<AssetAddonMapEnts>("addon_map_ents");
    }

    GameId Game::GetId() const
    {
        return GameId::IW4Xenon482;
    }

    const std::string& Game::GetFullName() const
    {
        static std::string fullName = "Call of Duty: Modern Warfare 2 (Xenon alpha 482)";
        return fullName;
    }

    const std::string& Game::GetShortName() const
    {
        static std::string shortName = "IW4Xenon482";
        return shortName;
    }
} // namespace IW4Xenon482
