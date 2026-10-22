#pragma once

#include "ObjWriter.h"

namespace IW4Xenon482
{
    class ObjWriter final : public IObjWriter
    {
    protected:
        void RegisterAssetDumpers(AssetDumpingContext& context) override;
    };
} // namespace IW4Xenon482
