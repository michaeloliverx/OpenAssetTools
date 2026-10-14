#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/T4Xenon/T4Xenon.h"

namespace phys_preset
{
    class InfoStringDumperT4Xenon final : public AbstractAssetDumper<T4Xenon::AssetPhysPreset>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<T4Xenon::AssetPhysPreset::Type>& asset) override;
    };
} // namespace phys_preset
