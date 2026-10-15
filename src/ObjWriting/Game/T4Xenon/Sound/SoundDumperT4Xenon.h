#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/T4Xenon/T4Xenon.h"

namespace sound
{
    class SoundDumperT4Xenon final : public AbstractAssetDumper<T4Xenon::AssetSound>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<T4Xenon::AssetSound::Type>& asset) override;
    };
} // namespace sound
