#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon328/IW3Xenon328.h"

namespace localize
{
    class DumperIW3Xenon328 final : public AbstractSingleProgressAssetDumper<IW3Xenon328::AssetLocalize>
    {
    public:
        void Dump(AssetDumpingContext& context) override;
    };
} // namespace localize
