#include "MenuConversionZoneStateIW3Xenon.h"

using namespace IW3Xenon;

void MenuConversionZoneState::AddLoadedFile(std::string loadedFileName, std::vector<XAssetInfo<menuDef_t>*> menusOfFile)
{
    m_menus_by_filename.emplace(std::move(loadedFileName), std::move(menusOfFile));
}
