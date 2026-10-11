#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon/IW3Xenon.h"

namespace sound
{
    class SoundDumperIW3Xenon final : public AbstractAssetDumper<IW3Xenon::AssetSound>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon::AssetSound::Type>& asset) override;
    };
} // namespace sound
