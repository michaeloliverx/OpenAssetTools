#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/NX1/NX1.h"

namespace localize
{
    class DumperNX1 final : public AbstractSingleProgressAssetDumper<NX1::AssetLocalize>
    {
    public:
        void Dump(AssetDumpingContext& context) override;
    };
} // namespace localize
