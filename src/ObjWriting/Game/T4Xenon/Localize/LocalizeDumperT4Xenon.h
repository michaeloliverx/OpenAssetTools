#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/T4Xenon/T4Xenon.h"

namespace localize
{
    class DumperT4Xenon final : public AbstractSingleProgressAssetDumper<T4Xenon::AssetLocalize>
    {
    public:
        void Dump(AssetDumpingContext& context) override;
    };
} // namespace localize
