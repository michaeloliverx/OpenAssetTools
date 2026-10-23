#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/IW3Xenon270/IW3Xenon270.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace xmodel
{
    std::unique_ptr<AssetCreator<IW3Xenon270::AssetXModel>> CreateLoaderIW3Xenon270(MemoryManager& memory, ISearchPath& searchPath, Zone& zone);
} // namespace xmodel
