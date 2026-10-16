#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/T5Xenon/T5Xenon.h"

namespace localize
{
    class DumperT5Xenon final : public AbstractSingleProgressAssetDumper<T5Xenon::AssetLocalize>
    {
    public:
        void Dump(AssetDumpingContext& context) override;
    };
} // namespace localize
