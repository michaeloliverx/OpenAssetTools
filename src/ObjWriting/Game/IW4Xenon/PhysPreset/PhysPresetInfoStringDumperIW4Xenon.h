#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW4Xenon/IW4Xenon.h"

namespace phys_preset
{
    class InfoStringDumperIW4Xenon final : public AbstractAssetDumper<IW4Xenon::AssetPhysPreset>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW4Xenon::AssetPhysPreset::Type>& asset) override;
    };
} // namespace phys_preset
