#include "GameIW3Xenon.h"

#include "IW3Xenon.h"

#include <algorithm>

using namespace IW3Xenon;

namespace
{
    constexpr const char* ASSET_TYPE_NAMES[]{
        "xmodelpieces", "physpreset",     "xanim",   "xmodel",      "material",    "pixelshader",      "techniqueset", "image",    "sound",    "soundcurve",
        "loadedsound",  "clipmap_unused", "clipmap", "comworld",    "gameworldsp", "gameworldmp",      "mapents",      "gfxworld", "lightdef", "uimap",
        "font",         "menulist",       "menu",    "localize",    "weapon",      "snddriverglobals", "fx",           "impactfx", "aitype",   "mptype",
        "character",    "xmodelalias",    "rawfile", "stringtable",
    };
    static_assert(std::extent_v<decltype(ASSET_TYPE_NAMES)> == ASSET_TYPE_COUNT);

    constexpr const char* SUB_ASSET_TYPE_NAMES[]{
        "vertexshader",
        "accuracygraph",
    };
    static_assert(std::extent_v<decltype(SUB_ASSET_TYPE_NAMES)> == SUB_ASSET_TYPE_COUNT);
} // namespace

namespace IW3Xenon
{
    Game::Game()
        : AbstractGame(ASSET_TYPE_NAMES, std::extent_v<decltype(ASSET_TYPE_NAMES)>, SUB_ASSET_TYPE_NAMES, std::extent_v<decltype(SUB_ASSET_TYPE_NAMES)>)
    {
    }

    GameId Game::GetId() const
    {
        return GameId::IW3Xenon;
    }

    const std::string& Game::GetFullName() const
    {
        static std::string fullName = "Call Of Duty 4: Modern Warfare (Xenon)";
        return fullName;
    }

    const std::string& Game::GetShortName() const
    {
        static std::string shortName = "IW3Xenon";
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
} // namespace IW3Xenon
