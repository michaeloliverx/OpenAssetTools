#pragma once

#include "Asset/IZoneAssetCreationState.h"
#include "Game/IW3Xenon/IW3Xenon.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace IW3Xenon
{
    class MenuConversionZoneState final : public IZoneAssetCreationState
    {
    public:
        void AddLoadedFile(std::string loadedFileName, std::vector<XAssetInfo<menuDef_t>*> menusOfFile);

        std::unordered_map<std::string, std::vector<XAssetInfo<menuDef_t>*>> m_menus_by_filename;
    };
} // namespace IW3Xenon
