#pragma once

#include "Game/IW3Xenon290/IW3Xenon290.h"
#include "Utils/MemoryManager.h"

namespace xmodel
{
    IW3Xenon290::XModelHighMipBounds* GenerateHighMipBoundsIW3Xenon290(const IW3Xenon290::XModel& model, MemoryManager& memory);
}
