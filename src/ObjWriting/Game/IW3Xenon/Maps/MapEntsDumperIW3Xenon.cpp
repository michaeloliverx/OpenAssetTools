#include "MapEntsDumperIW3Xenon.h"

using namespace IW3Xenon;

namespace map_ents
{
    DumperIW3Xenon::DumperIW3Xenon(const AssetPool<IW3Xenon::AssetMapEnts::Type>& pool)
        : AbstractAssetDumper(pool)
    {
    }

    void DumperIW3Xenon::DumpAsset(AssetDumpingContext& context, const XAssetInfo<MapEnts>& asset)
    {
        const auto* mapEnts = asset.Asset();
        const auto assetFile = context.OpenAssetFile(asset.m_name + ".ents");

        if (!assetFile)
            return;

        auto& stream = *assetFile;
        stream.write(mapEnts->entityString, mapEnts->numEntityChars);
    }
} // namespace map_ents
