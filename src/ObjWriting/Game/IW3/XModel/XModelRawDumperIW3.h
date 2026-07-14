#pragma once

#include "Game/IW3/IW3.h"
#include "Pool/AssetPool.h"
#include "Zone/ZoneScriptStrings.h"

#include <array>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

class AssetDumpingContext;

namespace xmodel
{
    struct RawXModelFilesIW3
    {
        std::string partsName;
        std::array<std::string, 4> surfaceNames;
        std::vector<uint8_t> model;
        std::vector<uint8_t> parts;
        std::array<std::vector<uint8_t>, 4> surfaces;
    };

    bool BuildRawXModelFilesIW3(
        const IW3::XModel& model, std::string_view assetName, const ZoneScriptStrings& scriptStrings, RawXModelFilesIW3& output, std::string& error);

    void DumpRawXModelIW3(AssetDumpingContext& context, const XAssetInfo<IW3::XModel>& asset);
} // namespace xmodel
