#include "ObjLoaderIW3Xenon.h"

#include "Asset/GlobalAssetPoolsLoader.h"
#include "Game/IW3Xenon/AssetMarkerIW3Xenon.h"
#include "Image/LoaderImageIW3Xenon.h"
#include "Localize/AssetLoaderLocalizeIW3Xenon.h"
#include "RawFile/AssetLoaderRawFileIW3Xenon.h"
#include "StringTable/LoaderStringTableIW3Xenon.h"

#include <memory>

using namespace IW3Xenon;

void ObjLoader::LoadReferencedContainersForZone(ISearchPath& searchPath, Zone& zone) const {}

void ObjLoader::UnloadContainersOfZone(Zone& zone) const {}

void ObjLoader::ConfigureCreatorCollection(AssetCreatorCollection& collection, Zone& zone, ISearchPath& searchPath, IGdtQueryable& gdt) const
{
    auto& memory = zone.Memory();

    collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetImage>>(memory));
    collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetLocalize>>(memory));
    collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetRawFile>>(memory));
    collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetStringTable>>(memory));

    collection.AddAssetCreator(image::CreateLoaderIW3Xenon(memory, searchPath));
    collection.AddAssetCreator(localize::CreateLoaderIW3Xenon(memory, searchPath, zone));
    collection.AddAssetCreator(raw_file::CreateLoaderIW3Xenon(memory, searchPath));
    collection.AddAssetCreator(string_table::CreateLoaderIW3Xenon(memory, searchPath));

    collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetImage>>(zone));
    collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetLocalize>>(zone));
    collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetRawFile>>(zone));
    collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetStringTable>>(zone));
}
