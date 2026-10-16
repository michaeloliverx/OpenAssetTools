#pragma once

#include "Asset/AssetCreationContext.h"
#include "Asset/AssetRegistration.h"
#include "Game/IW4Xenon/IW4Xenon.h"
#include "Utils/MemoryManager.h"

#include <istream>

namespace IW4Xenon
{
    bool LoadMaterialAsJson(
        std::istream& stream, Material& material, MemoryManager& memory, AssetCreationContext& context, AssetRegistration<AssetMaterial>& registration);
} // namespace IW4Xenon
