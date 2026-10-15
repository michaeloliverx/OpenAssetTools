#pragma once

#include "ObjWriter.h"

namespace NX1
{
    class ObjWriter final : public IObjWriter
    {
    protected:
        void RegisterAssetDumpers(AssetDumpingContext& context) override;
    };
} // namespace NX1
