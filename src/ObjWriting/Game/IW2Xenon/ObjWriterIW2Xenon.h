#pragma once

#include "ObjWriter.h"

namespace IW2Xenon
{
    class ObjWriter final : public IObjWriter
    {
    protected:
        void RegisterAssetDumpers(AssetDumpingContext& context) override;
    };
} // namespace IW2Xenon
