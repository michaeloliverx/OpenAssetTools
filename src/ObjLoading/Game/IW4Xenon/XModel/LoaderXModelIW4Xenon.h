#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/IW4Xenon/IW4Xenon.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace xmodel
{
    std::unique_ptr<AssetCreator<IW4Xenon::AssetXModel>> CreateLoaderIW4Xenon(MemoryManager& memory, ISearchPath& searchPath, Zone& zone);
} // namespace xmodel
