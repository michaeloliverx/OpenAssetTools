#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon/IW3Xenon.h"

namespace sound
{
    class LoadedSoundDumperIW3Xenon final : public AbstractAssetDumper<IW3Xenon::AssetLoadedSound>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon::AssetLoadedSound::Type>& asset) override;
    };
} // namespace sound
