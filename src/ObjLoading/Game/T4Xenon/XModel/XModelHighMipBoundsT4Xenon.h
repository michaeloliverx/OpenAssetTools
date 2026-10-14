#pragma once

#include "Game/T4Xenon/T4Xenon.h"
#include "Utils/MemoryManager.h"

namespace xmodel
{
    T4Xenon::XModelHighMipBounds* GenerateHighMipBoundsT4Xenon(const T4Xenon::XModel& model, MemoryManager& memory);
}
