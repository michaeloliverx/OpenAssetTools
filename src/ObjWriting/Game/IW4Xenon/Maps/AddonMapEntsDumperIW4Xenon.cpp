#include "AddonMapEntsDumperIW4Xenon.h"

#include <algorithm>

using namespace IW4Xenon;

namespace addon_map_ents
{
    void DumperIW4Xenon::DumpAsset(AssetDumpingContext& context, const XAssetInfo<AssetAddonMapEnts::Type>& asset)
    {
        const auto* addonMapEnts = asset.Asset();
        const auto assetFile = context.OpenAssetFile(asset.m_name);

        if (!assetFile)
            return;

        auto& stream = *assetFile;

        stream.write(addonMapEnts->entityString, std::max(addonMapEnts->numEntityChars - 1, 0));
    }
} // namespace addon_map_ents
