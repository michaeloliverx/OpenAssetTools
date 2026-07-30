#pragma once

#include "Dumping/AssetDumpingContext.h"
#include "Game/IW4/IW4.h"

#include <string>

namespace map_ents
{
    void DumpMapEntsTriggerMapIW4(const std::string& mapEntsName, const AssetDumpingContext& context, const IW4::MapTriggers& triggers);
} // namespace map_ents
