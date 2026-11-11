#include "GameT4Xenon302.h"

#include "T4Xenon302.h"

using namespace T4Xenon302;

namespace
{
    constexpr const char* ASSET_TYPE_NAMES[]{
        "xmodelpieces",     "physpreset", "xanim",    "xmodel",   "material", "techniqueset", "image",       "sound",   "soundcurve", "clipmap_unused",
        "clipmap",          "comworld",   "gfxworld", "lightdef", "uimap",    "font",         "menulist",    "menu",    "localize",   "weapon",
        "snddriverglobals", "fx",         "impactfx", "aitype",   "mptype",   "character",    "xmodelalias", "rawfile",
    };
    constexpr const char* SUB_ASSET_TYPE_NAMES[]{"technique", "vertexdecl", "vertexshader", "pixelshader"};
    static_assert(std::extent_v<decltype(ASSET_TYPE_NAMES)> == ASSET_TYPE_COUNT);
} // namespace

Game::Game()
    : AbstractGame(ASSET_TYPE_NAMES, ASSET_TYPE_COUNT, SUB_ASSET_TYPE_NAMES, std::extent_v<decltype(SUB_ASSET_TYPE_NAMES)>)
{
}

GameId Game::GetId() const
{
    return GameId::T4Xenon302;
}

const std::string& Game::GetFullName() const
{
    static const std::string name = "Call of Duty: World at War (Xenon, March 27, 2007, FF 302)";
    return name;
}

const std::string& Game::GetShortName() const
{
    static const std::string name = "T4Xenon302";
    return name;
}
