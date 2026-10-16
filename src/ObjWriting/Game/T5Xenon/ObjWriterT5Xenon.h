#pragma once

#include "ObjWriter.h"

namespace T5Xenon
{
    class ObjWriter final : public IObjWriter
    {
    protected:
        void RegisterAssetDumpers(AssetDumpingContext& context) override;
    };
} // namespace T5Xenon
