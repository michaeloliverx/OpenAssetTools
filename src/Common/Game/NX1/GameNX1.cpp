#include "GameNX1.h"

#include "NX1.h"

#include <algorithm>

using namespace NX1;

namespace
{
    constexpr const char* ASSET_TYPE_NAMES[]{
        "physpreset",
        "physcollmap",
        "xanim",
        "xmodelsurfs",
        "xmodel",
        "material",
        "pixelshader",
        "techniqueset",
        "image",
        "sound",
        "soundcurve",
        "soundlpfcurve",
        "loadedsound",
        "clipmap_unused",
        "clipmap",
        "comworld",
        "gameworldsp",
        "gameworldmp",
        "mapents",
        "fxworld",
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
        "surfacefx",
        "aitype",
        "mptype",
        "character",
        "xmodelalias",
        "rawfile",
        "stringtable",
        "leaderboard",
        "structureddatadef",
        "tracer",
        "laser",
        "vehicle",
        "addonmapents",
        "shockfile",
        "volumesetting",
        "reverbpreset",
        "fog",
    };
    static_assert(std::extent_v<decltype(ASSET_TYPE_NAMES)> == ASSET_TYPE_COUNT);

    constexpr const char* SUB_ASSET_TYPE_NAMES[]{
        "technique",
        "vertexdecl",
        "vertexshader",
    };
    static_assert(std::extent_v<decltype(SUB_ASSET_TYPE_NAMES)> == SUB_ASSET_TYPE_COUNT);
} // namespace

namespace NX1
{
    Game::Game()
        : AbstractGame(ASSET_TYPE_NAMES, std::extent_v<decltype(ASSET_TYPE_NAMES)>, SUB_ASSET_TYPE_NAMES, std::extent_v<decltype(SUB_ASSET_TYPE_NAMES)>)
    {
        AddAssetTypeNameAlias<AssetTechniqueSet>("techset");
        AddAssetTypeNameAlias<AssetLightDef>("gfxlightdef");
        AddAssetTypeNameAlias<AssetSndVfCurve>("sndvfcurve");
        AddAssetTypeNameAlias<AssetSndLpfCurve>("sndlpfcurve");
    }

    GameId Game::GetId() const
    {
        return GameId::NX1;
    }

    const std::string& Game::GetFullName() const
    {
        static std::string fullName = "Call of Duty: Future Warfare";
        return fullName;
    }

    const std::string& Game::GetShortName() const
    {
        static std::string shortName = "NX1";
        return shortName;
    }
} // namespace NX1
