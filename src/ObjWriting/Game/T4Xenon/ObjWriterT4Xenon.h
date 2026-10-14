#pragma once

#include "ObjWriter.h"

namespace T4Xenon
{
    class ObjWriter final : public IObjWriter
    {
    protected:
        void RegisterAssetDumpers(AssetDumpingContext& context) override;
    };
} // namespace T4Xenon
