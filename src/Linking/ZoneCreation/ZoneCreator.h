#pragma once

#include "Zone/Zone.h"
#include "ZoneCreationContext.h"

namespace zone_creator
{
    [[nodiscard]] std::unique_ptr<Zone> CreateZoneForDefinition(ZoneCreationContext& context);
}
