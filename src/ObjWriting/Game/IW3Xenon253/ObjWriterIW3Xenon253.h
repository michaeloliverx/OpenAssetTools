#pragma once

#include "ObjWriter.h"

namespace IW3Xenon253
{
    class ObjWriter final : public IObjWriter
    {
    protected:
        void RegisterAssetDumpers(AssetDumpingContext& context) override;
    };
} // namespace IW3Xenon253
