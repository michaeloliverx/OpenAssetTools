#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon253/IW3Xenon253.h"

namespace localize
{
    class DumperIW3Xenon253 final : public AbstractSingleProgressAssetDumper<IW3Xenon253::AssetLocalize>
    {
    public:
        void Dump(AssetDumpingContext& context) override;
    };
} // namespace localize
