#include "ZoneWriterFactoryT4Xenon204.h"

#include "Utils/Logging/Log.h"

std::unique_ptr<ZoneWriter> T4Xenon204::ZoneWriterFactory::CreateWriter(const Zone&) const
{
    con::error("T4 Xenon FF 204 writing is not supported.");
    return nullptr;
}
