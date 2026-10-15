#pragma once

#include "Writing/IZoneWriterFactory.h"

namespace NX1
{
    class ZoneWriterFactory final : public IZoneWriterFactory
    {
    public:
        std::unique_ptr<ZoneWriter> CreateWriter(const Zone& zone) const override;
    };
} // namespace NX1
