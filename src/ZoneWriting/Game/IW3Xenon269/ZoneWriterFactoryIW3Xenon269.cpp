#include "ZoneWriterFactoryIW3Xenon269.h"

#include "Utils/Logging/Log.h"

std::unique_ptr<ZoneWriter> IW3Xenon269::ZoneWriterFactory::CreateWriter(const Zone&) const
{
    con::error("IW3 Xenon FF 269 writing is not supported.");
    return nullptr;
}
