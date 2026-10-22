#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon290/IW3Xenon290.h"

namespace localize
{
    class DumperIW3Xenon290 final : public AbstractSingleProgressAssetDumper<IW3Xenon290::AssetLocalize>
    {
    public:
        void Dump(AssetDumpingContext& context) override;
    };
} // namespace localize
