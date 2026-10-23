#pragma once

#include "Game/IW3Xenon253/IW3Xenon253.h"
#include "Utils/MemoryManager.h"

namespace xmodel
{
    IW3Xenon253::XModelHighMipBounds* GenerateHighMipBoundsIW3Xenon253(const IW3Xenon253::XModel& model, MemoryManager& memory);
}
