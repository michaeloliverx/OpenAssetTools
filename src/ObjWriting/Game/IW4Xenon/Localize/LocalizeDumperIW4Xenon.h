#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW4Xenon/IW4Xenon.h"

namespace localize
{
    class DumperIW4Xenon final : public AbstractSingleProgressAssetDumper<IW4Xenon::AssetLocalize>
    {
    public:
        void Dump(AssetDumpingContext& context) override;
    };
} // namespace localize
