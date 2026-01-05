#include "AssetNameResolverIW3Xenon.h"

#include "Game/IW3Xenon/GameAssetPoolIW3Xenon.h"
#include "Utils/StringUtils.h"

using namespace IW3Xenon;

AssetNameResolver::AssetNameResolver()
{
    for (auto assetType = 0; assetType < ASSET_TYPE_COUNT; assetType++)
        AddAssetTypeName(assetType, *GameAssetPoolIW3Xenon::AssetTypeNameByType(assetType));
}

GameId AssetNameResolver::GetGameId() const
{
    return GameId::IW3;
}

std::optional<const char*> AssetNameResolver::GetAssetTypeName(const asset_type_t assetType) const
{
    return GameAssetPoolIW3Xenon::AssetTypeNameByType(assetType);
}
