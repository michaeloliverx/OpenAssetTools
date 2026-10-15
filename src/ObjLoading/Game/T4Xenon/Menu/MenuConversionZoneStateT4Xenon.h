#pragma once

#include "Asset/IZoneAssetCreationState.h"
#include "Game/T4Xenon/T4Xenon.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace T4Xenon
{
    class MenuConversionZoneState final : public IZoneAssetCreationState
    {
    public:
        void AddLoadedFile(std::string loadedFileName, std::vector<XAssetInfo<menuDef_t>*> menusOfFile);

        std::unordered_map<std::string, std::vector<XAssetInfo<menuDef_t>*>> m_menus_by_filename;
    };
} // namespace T4Xenon
