#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon270/IW3Xenon270.h"

namespace localize
{
    class DumperIW3Xenon270 final : public AbstractSingleProgressAssetDumper<IW3Xenon270::AssetLocalize>
    {
    public:
        void Dump(AssetDumpingContext& context) override;
    };
} // namespace localize
