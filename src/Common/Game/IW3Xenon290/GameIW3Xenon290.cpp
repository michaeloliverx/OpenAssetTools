#include "GameIW3Xenon290.h"

#include "IW3Xenon290.h"

using namespace IW3Xenon290;

namespace
{
    constexpr const char* ASSET_TYPE_NAMES[]{
        "xmodelpieces", "physpreset",     "xanim",     "xmodel",      "material",    "pixelshader", "techniqueset",     "image",    "sound",
        "soundcurve",   "clipmap_unused", "clipmap",   "comworld",    "gameworldsp", "gameworldmp", "mapents",          "gfxworld", "lightdef",
        "uimap",        "font",           "menulist",  "menu",        "localize",    "weapon",      "snddriverglobals", "fx",       "impactfx",
        "aitype",       "mptype",         "character", "xmodelalias", "rawfile",     "stringtable",
    };
    constexpr const char* SUB_ASSET_TYPE_NAMES[]{"technique", "vertexdecl", "vertexshader"};
    static_assert(std::extent_v<decltype(ASSET_TYPE_NAMES)> == ASSET_TYPE_COUNT);
} // namespace

Game::Game()
    : AbstractGame(ASSET_TYPE_NAMES, ASSET_TYPE_COUNT, SUB_ASSET_TYPE_NAMES, std::extent_v<decltype(SUB_ASSET_TYPE_NAMES)>)
{
}

GameId Game::GetId() const
{
    return GameId::IW3Xenon290;
}

const std::string& Game::GetFullName() const
{
    static const std::string name = "Call of Duty 4: Modern Warfare (Xenon 290)";
    return name;
}

const std::string& Game::GetShortName() const
{
    static const std::string name = "IW3Xenon290";
    return name;
}
