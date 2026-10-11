#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/IW3Xenon/IW3Xenon.h"
#include "Gdt/IGdtQueryable.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace phys_preset
{
    std::unique_ptr<AssetCreator<IW3Xenon::AssetPhysPreset>> CreateGdtLoaderIW3Xenon(MemoryManager& memory, IGdtQueryable& gdt, Zone& zone);
} // namespace phys_preset
