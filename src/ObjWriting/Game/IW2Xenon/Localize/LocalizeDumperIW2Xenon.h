#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW2Xenon/IW2Xenon.h"

namespace localize
{
    class DumperIW2Xenon final : public AbstractSingleProgressAssetDumper<IW2Xenon::AssetLocalize>
    {
    public:
        void Dump(AssetDumpingContext& context) override;
    };
} // namespace localize
