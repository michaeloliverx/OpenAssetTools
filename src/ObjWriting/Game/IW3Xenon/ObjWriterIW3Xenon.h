#pragma once

#include "IObjWriter.h"

namespace IW3Xenon
{
    class ObjWriter final : public IObjWriter
    {
    public:
        bool DumpZone(AssetDumpingContext& context) const override;
    };
} // namespace IW3Xenon
