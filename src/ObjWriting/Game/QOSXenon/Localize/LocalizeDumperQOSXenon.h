#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/QOSXenon/QOSXenon.h"

namespace localize
{
    class DumperQOSXenon final : public AbstractSingleProgressAssetDumper<QOSXenon::AssetLocalize>
    {
    public:
        void Dump(AssetDumpingContext& context) override;
    };
} // namespace localize
