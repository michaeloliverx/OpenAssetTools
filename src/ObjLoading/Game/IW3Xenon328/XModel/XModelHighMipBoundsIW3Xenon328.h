#pragma once

#include "Game/IW3Xenon328/IW3Xenon328.h"
#include "Utils/MemoryManager.h"

namespace xmodel
{
    IW3Xenon328::XModelHighMipBounds* GenerateHighMipBoundsIW3Xenon328(const IW3Xenon328::XModel& model, MemoryManager& memory);
}
