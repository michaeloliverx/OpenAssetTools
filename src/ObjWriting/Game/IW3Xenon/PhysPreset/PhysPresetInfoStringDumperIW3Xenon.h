#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon/IW3Xenon.h"

namespace phys_preset
{
    class InfoStringDumperIW3Xenon final : public AbstractAssetDumper<IW3Xenon::AssetPhysPreset>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon::AssetPhysPreset::Type>& asset) override;
    };
} // namespace phys_preset
