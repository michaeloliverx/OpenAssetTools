#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/T4Xenon/T4Xenon.h"

namespace weapon
{
    class DumperT4Xenon final : public AbstractAssetDumper<T4Xenon::AssetWeapon>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<T4Xenon::AssetWeapon::Type>& asset) override;
    };
} // namespace weapon
