#include "ObjLoaderIW3Xenon.h"

#include "Asset/GlobalAssetPoolsLoader.h"
#include "Game/IW3Xenon/AssetMarkerIW3Xenon.h"
#include "RawFile/AssetLoaderRawFileIW3Xenon.h"

#include <memory>

using namespace IW3Xenon;

void ObjLoader::LoadReferencedContainersForZone(ISearchPath& searchPath, Zone& zone) const {}

void ObjLoader::UnloadContainersOfZone(Zone& zone) const {}

void ObjLoader::ConfigureCreatorCollection(AssetCreatorCollection& collection, Zone& zone, ISearchPath& searchPath, IGdtQueryable& gdt) const
{
    auto& memory = zone.Memory();

    collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetRawFile>>(memory));
    collection.AddAssetCreator(raw_file::CreateLoaderIW3Xenon(memory, searchPath));
    collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetRawFile>>(zone));
}
