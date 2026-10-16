#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/IW4Xenon/IW4Xenon.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace light_def
{
    std::unique_ptr<AssetCreator<IW4Xenon::AssetLightDef>> CreateLoaderIW4Xenon(MemoryManager& memory, ISearchPath& searchPath);
} // namespace light_def
