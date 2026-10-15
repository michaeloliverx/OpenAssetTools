#include "MenuConversionZoneStateT4Xenon.h"

using namespace T4Xenon;

void MenuConversionZoneState::AddLoadedFile(std::string loadedFileName, std::vector<XAssetInfo<menuDef_t>*> menusOfFile)
{
    m_menus_by_filename.emplace(std::move(loadedFileName), std::move(menusOfFile));
}
