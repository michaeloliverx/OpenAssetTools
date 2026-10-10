#pragma once

#include "Game/IW3Xenon/IW3Xenon.h"
#include "Utils/MemoryManager.h"

namespace xmodel
{
    IW3Xenon::XModelHighMipBounds* GenerateHighMipBoundsIW3Xenon(const IW3Xenon::XModel& model, MemoryManager& memory);
}
