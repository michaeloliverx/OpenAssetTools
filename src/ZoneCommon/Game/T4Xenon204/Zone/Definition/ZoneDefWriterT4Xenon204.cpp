#include "ZoneDefWriterT4Xenon204.h"

#include "Game/T4Xenon204/T4Xenon204.h"

using namespace T4Xenon204;

void ZoneDefWriter::WriteMetaData(ZoneDefinitionOutputStream& stream, const Zone& zone) const {}

void ZoneDefWriter::WriteContent(ZoneDefinitionOutputStream& stream, const Zone& zone, const ZoneDefFilter& filter) const
{
    const auto* game = IGame::GetGameById(zone.m_game_id);

    // Localized strings share one file.
    auto localizePoolAssets = zone.m_pools.PoolAssets<AssetLocalize>();
    if (localizePoolAssets.begin() != localizePoolAssets.end())
        stream.WriteEntry(*game->GetAssetTypeName(ASSET_TYPE_LOCALIZE_ENTRY), zone.m_name);

    for (const auto& asset : zone.m_pools)
    {
        if (!filter.ShouldWriteAsset(*asset))
            continue;

        switch (asset->m_type)
        {
        case ASSET_TYPE_LOCALIZE_ENTRY:
            break;

        default:
            stream.WriteEntry(*game->GetAssetTypeName(asset->m_type), asset->m_name);
            break;
        }
    }
}
