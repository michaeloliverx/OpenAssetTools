#include "MapEntsDumperIW3Xenon328.h"

#include "Maps/MapEntsCommon.h"

#include <algorithm>

using namespace IW3Xenon328;

namespace map_ents
{
    void DumperIW3Xenon328::DumpAsset(AssetDumpingContext& context, const XAssetInfo<AssetMapEnts::Type>& asset)
    {
        const auto* mapEnts = asset.Asset();
        const auto assetFile = context.OpenAssetFile(GetEntsFileNameForAssetName(asset.m_name));

        if (!assetFile)
            return;

        auto& stream = *assetFile;
        stream.write(mapEnts->entityString, std::max(mapEnts->numEntityChars - 1, 0));
    }
} // namespace map_ents
