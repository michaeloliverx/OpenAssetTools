#pragma once

#include "ObjWriter.h"

namespace QOSXenon
{
    class ObjWriter final : public IObjWriter
    {
    protected:
        void RegisterAssetDumpers(AssetDumpingContext& context) override;
    };
} // namespace QOSXenon
