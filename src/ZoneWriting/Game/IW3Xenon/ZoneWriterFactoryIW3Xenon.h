#pragma once

#include "Writing/IZoneWriterFactory.h"

namespace IW3Xenon
{
    class ZoneWriterFactory final : public IZoneWriterFactory
    {
    public:
        [[nodiscard]] std::unique_ptr<ZoneWriter> CreateWriter(const Zone& zone) const override;
    };
} // namespace IW3Xenon
