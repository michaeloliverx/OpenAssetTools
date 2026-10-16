#pragma once

#include "Writing/IZoneWriterFactory.h"

namespace T5Xenon
{
    class ZoneWriterFactory final : public IZoneWriterFactory
    {
    public:
        [[nodiscard]] std::unique_ptr<ZoneWriter> CreateWriter(const Zone& zone) const override;
    };
} // namespace T5Xenon
