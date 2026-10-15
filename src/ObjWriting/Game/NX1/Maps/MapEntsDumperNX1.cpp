#include "MapEntsDumperNX1.h"

#include "Maps/MapEntsCommon.h"

#include <algorithm>

using namespace NX1;

namespace map_ents
{
    void DumperNX1::DumpAsset(AssetDumpingContext& context, const XAssetInfo<AssetMapEnts::Type>& asset)
    {
        const auto* mapEnts = asset.Asset();
        const auto assetFile = context.OpenAssetFile(GetEntsFileNameForAssetName(asset.m_name));

        if (!assetFile)
            return;

        auto& stream = *assetFile;
        stream.write(mapEnts->entityString, std::max(mapEnts->numEntityChars - 1, 0));
    }
} // namespace map_ents
