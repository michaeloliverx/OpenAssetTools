#include "ObjLoaderT4Xenon.h"

#include "Asset/GlobalAssetPoolsLoader.h"
#include "Game/T4Xenon/AssetMarkerT4Xenon.h"
#include "Game/T4Xenon/T4Xenon.h"
#include "Game/T4Xenon/XModel/LoaderXModelT4Xenon.h"
#include "Image/LoaderImageT4Xenon.h"
#include "Localize/AssetLoaderLocalizeT4Xenon.h"
#include "Maps/MapEntsLoaderT4Xenon.h"
#include "PhysPreset/GdtLoaderPhysPresetT4Xenon.h"
#include "PhysPreset/RawLoaderPhysPresetT4Xenon.h"
#include "RawFile/AssetLoaderRawFileT4Xenon.h"
#include "StringTable/LoaderStringTableT4Xenon.h"

using namespace T4Xenon;

void ObjLoader::LoadReferencedContainersForZone(ISearchPath& searchPath, Zone& zone) const {}

void ObjLoader::UnloadContainersOfZone(Zone& zone) const {}

namespace
{
    void ConfigureDefaultCreators(AssetCreatorCollection& collection, const Zone& zone)
    {
        auto& memory = zone.Memory();

        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetPhysPreset>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetPhysConstraints>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetXModel>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetMaterial>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetImage>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetMapEnts>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetLocalize>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetRawFile>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetStringTable>>(memory));
    }

    void ConfigureGlobalAssetPoolsLoaders(AssetCreatorCollection& collection, Zone& zone)
    {
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetPhysPreset>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetPhysConstraints>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetXModel>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetMaterial>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetTechniqueSet>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetPixelShader>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetImage>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetMapEnts>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetLocalize>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetRawFile>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetStringTable>>(zone));
    }

    void ConfigureLoaders(AssetCreatorCollection& collection, Zone& zone, ISearchPath& searchPath, IGdtQueryable& gdt)
    {
        auto& memory = zone.Memory();

        collection.AddAssetCreator(xmodel::CreateLoaderT4Xenon(memory, searchPath, zone));
        collection.AddAssetCreator(image::CreateLoaderT4Xenon(memory, searchPath));
        collection.AddAssetCreator(localize::CreateLoaderT4Xenon(memory, searchPath, zone));
        collection.AddAssetCreator(map_ents::CreateLoaderT4Xenon(memory, searchPath));
        collection.AddAssetCreator(phys_preset::CreateRawLoaderT4Xenon(memory, searchPath, zone));
        collection.AddAssetCreator(phys_preset::CreateGdtLoaderT4Xenon(memory, gdt, zone));
        collection.AddAssetCreator(raw_file::CreateLoaderT4Xenon(memory, searchPath));
        collection.AddAssetCreator(string_table::CreateLoaderT4Xenon(memory, searchPath));
    }
} // namespace

void ObjLoader::ConfigureCreatorCollection(AssetCreatorCollection& collection, Zone& zone, ISearchPath& searchPath, IGdtQueryable& gdt) const
{
    ConfigureDefaultCreators(collection, zone);
    ConfigureLoaders(collection, zone, searchPath, gdt);
    ConfigureGlobalAssetPoolsLoaders(collection, zone);
}
