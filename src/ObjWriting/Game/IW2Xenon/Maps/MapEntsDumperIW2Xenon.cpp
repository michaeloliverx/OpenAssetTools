#include "MapEntsDumperIW2Xenon.h"

#include "Maps/MapEntsCommon.h"

#include <algorithm>

using namespace IW2Xenon;

namespace map_ents
{
    void DumperIW2Xenon::DumpAsset(AssetDumpingContext& context, const XAssetInfo<AssetClipMapMp::Type>& asset)
    {
        const auto* clipMap = asset.Asset();
        if (!clipMap->entityString)
            return;

        const auto assetFile = context.OpenAssetFile(GetEntsFileNameForAssetName(asset.m_name));
        if (!assetFile)
            return;

        auto& stream = *assetFile;
        stream.write(clipMap->entityString, std::max(clipMap->numEntityChars - 1, 0));
    }
} // namespace map_ents
