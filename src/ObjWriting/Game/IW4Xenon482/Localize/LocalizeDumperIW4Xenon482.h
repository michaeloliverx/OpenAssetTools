#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW4Xenon482/IW4Xenon482.h"

namespace localize
{
    class DumperIW4Xenon482 final : public AbstractSingleProgressAssetDumper<IW4Xenon482::AssetLocalize>
    {
    public:
        void Dump(AssetDumpingContext& context) override;
    };
} // namespace localize
