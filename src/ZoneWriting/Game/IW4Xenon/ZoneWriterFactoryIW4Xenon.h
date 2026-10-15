#pragma once

#include "Writing/IZoneWriterFactory.h"

namespace IW4Xenon
{
    class ZoneWriterFactory final : public IZoneWriterFactory
    {
    public:
        std::unique_ptr<ZoneWriter> CreateWriter(const Zone& zone) const override;
    };
} // namespace IW4Xenon
