#pragma once

#include "Game/IW3Xenon270/IW3Xenon270.h"
#include "Utils/MemoryManager.h"

namespace xmodel
{
    IW3Xenon270::XModelHighMipBounds* GenerateHighMipBoundsIW3Xenon270(const IW3Xenon270::XModel& model, MemoryManager& memory);
}
