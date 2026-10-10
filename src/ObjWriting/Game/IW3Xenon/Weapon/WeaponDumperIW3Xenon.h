#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon/IW3Xenon.h"

namespace weapon
{
    class DumperIW3Xenon final : public AbstractAssetDumper<IW3Xenon::AssetWeapon>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon::AssetWeapon::Type>& asset) override;
    };
} // namespace weapon
