#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/T4Xenon/T4Xenon.h"

namespace sound
{
    class LoadedSoundDumperT4Xenon final : public AbstractAssetDumper<T4Xenon::AssetLoadedSound>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<T4Xenon::AssetLoadedSound::Type>& asset) override;
    };
} // namespace sound
