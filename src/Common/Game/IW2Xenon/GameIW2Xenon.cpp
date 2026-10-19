#include "GameIW2Xenon.h"

#include "IW2Xenon.h"

using namespace IW2Xenon;

namespace
{
    constexpr const char* ASSET_TYPE_NAMES[]{
        "xanim",    "xmodel", "material", "image",     "sound",       "soundcurve", "clipmap_unused", "clipmap",          "gfxworld",
        "lightdef", "uimap",  "font",     "menulist",  "menu",        "localize",   "weapon",         "snddriverglobals", "fx",
        "impactfx", "aitype", "mptype",   "character", "xmodelalias", "rawfile",
    };
    constexpr const char* SUB_ASSET_TYPE_NAMES[]{
        "techniqueset",
        "technique",
        "vertexdecl",
        "shader",
    };
    static_assert(std::extent_v<decltype(ASSET_TYPE_NAMES)> == ASSET_TYPE_COUNT);
    static_assert(std::extent_v<decltype(SUB_ASSET_TYPE_NAMES)> == SUB_ASSET_TYPE_COUNT);
} // namespace

Game::Game()
    : AbstractGame(ASSET_TYPE_NAMES, ASSET_TYPE_COUNT, SUB_ASSET_TYPE_NAMES, SUB_ASSET_TYPE_COUNT)
{
}

GameId Game::GetId() const
{
    return GameId::IW2Xenon;
}

const std::string& Game::GetFullName() const
{
    static const std::string name = "Call of Duty 2 (Xenon)";
    return name;
}

const std::string& Game::GetShortName() const
{
    static const std::string name = "IW2";
    return name;
}
