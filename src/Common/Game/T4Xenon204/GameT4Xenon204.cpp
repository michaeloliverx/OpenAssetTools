#include "GameT4Xenon204.h"

#include "T4Xenon204.h"

using namespace T4Xenon204;

namespace
{
    constexpr const char* ASSET_TYPE_NAMES[]{
        "xanim",    "xmodel",   "material", "techniqueset", "image",     "sound",       "soundcurve", "clipmap", "comworld",
        "gfxworld", "lightdef", "uimap",    "font",         "menulist",  "menu",        "localize",   "weapon",  "snddriverglobals",
        "fx",       "impactfx", "aitype",   "mptype",       "character", "xmodelalias", "rawfile",
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
    return GameId::T4Xenon204;
}

const std::string& Game::GetFullName() const
{
    static const std::string name = "Call of Duty: World at War (Xenon, experimental FF 204)";
    return name;
}

const std::string& Game::GetShortName() const
{
    static const std::string name = "T4Xenon204";
    return name;
}
