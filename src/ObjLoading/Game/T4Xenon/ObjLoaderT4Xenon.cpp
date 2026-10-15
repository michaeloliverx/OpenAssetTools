#include "ObjLoaderT4Xenon.h"

#include "Asset/GlobalAssetPoolsLoader.h"
#include "Game/T4Xenon/AssetMarkerT4Xenon.h"
#include "Game/T4Xenon/Font/FontLoaderT4Xenon.h"
#include "Game/T4Xenon/T4Xenon.h"
#include "Game/T4Xenon/Weapon/AccuracyGraphLoaderT4Xenon.h"
#include "Game/T4Xenon/XAnim/XAnimLoaderT4Xenon.h"
#include "Game/T4Xenon/XModel/LoaderXModelT4Xenon.h"
#include "Image/LoaderImageT4Xenon.h"
#include "Localize/AssetLoaderLocalizeT4Xenon.h"
#include "Maps/MapEntsLoaderT4Xenon.h"
#include "Material/LoaderMaterialT4Xenon.h"
#include "Menu/LoaderMenuListT4Xenon.h"
#include "PhysPreset/GdtLoaderPhysPresetT4Xenon.h"
#include "PhysPreset/RawLoaderPhysPresetT4Xenon.h"
#include "RawFile/AssetLoaderRawFileT4Xenon.h"
#include "StringTable/LoaderStringTableT4Xenon.h"
#include "Techset/PixelShaderLoaderT4Xenon.h"
#include "Weapon/FlameTableLoaderT4Xenon.h"
#include "Weapon/WeaponGdtLoaderT4Xenon.h"
#include "Weapon/WeaponRawLoaderT4Xenon.h"

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
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetXAnim>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetXModel>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetMaterial>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetTechniqueSet>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetPixelShader>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetImage>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetSound>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetLoadedSound>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetFont>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetMenuList>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetMenu>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetMapEnts>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetLocalize>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetWeapon>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetFx>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetRawFile>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetStringTable>>(memory));
    }

    void ConfigureGlobalAssetPoolsLoaders(AssetCreatorCollection& collection, Zone& zone)
    {
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetPhysPreset>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetPhysConstraints>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetXAnim>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetXModel>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetMaterial>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetTechniqueSet>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetPixelShader>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetImage>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetSound>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetLoadedSound>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetFont>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetMenuList>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetMenu>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetMapEnts>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetLocalize>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetWeapon>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetFx>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetRawFile>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetStringTable>>(zone));
    }

    void ConfigureLoaders(AssetCreatorCollection& collection, Zone& zone, ISearchPath& searchPath, IGdtQueryable& gdt)
    {
        auto& memory = zone.Memory();

        collection.AddAssetCreator(xanim::CreateLoaderT4Xenon(memory, searchPath, zone));
        collection.AddAssetCreator(xmodel::CreateLoaderT4Xenon(memory, searchPath, zone));
        collection.AddAssetCreator(techset::CreatePixelShaderLoaderT4Xenon(memory, searchPath));
        collection.AddAssetCreator(image::CreateLoaderT4Xenon(memory, searchPath));
        collection.AddAssetCreator(material::CreateLoaderT4Xenon(memory, searchPath));
        collection.AddAssetCreator(font::CreateLoaderT4Xenon(memory, searchPath));
        collection.AddAssetCreator(menu::CreateMenuListLoaderT4Xenon(memory, searchPath));
        collection.AddAssetCreator(localize::CreateLoaderT4Xenon(memory, searchPath, zone));
        collection.AddAssetCreator(map_ents::CreateLoaderT4Xenon(memory, searchPath));
        collection.AddAssetCreator(phys_preset::CreateRawLoaderT4Xenon(memory, searchPath, zone));
        collection.AddAssetCreator(phys_preset::CreateGdtLoaderT4Xenon(memory, gdt, zone));
        collection.AddAssetCreator(raw_file::CreateLoaderT4Xenon(memory, searchPath));
        collection.AddAssetCreator(string_table::CreateLoaderT4Xenon(memory, searchPath));
        collection.AddAssetCreator(weapon::CreateRawLoaderT4Xenon(memory, searchPath, zone));
        collection.AddAssetCreator(weapon::CreateGdtLoaderT4Xenon(memory, searchPath, gdt, zone));

        collection.AddSubAssetCreator(weapon::CreateAccuracyGraphLoaderT4Xenon(memory, searchPath));
        collection.AddSubAssetCreator(weapon::CreateFlameTableLoaderT4Xenon(memory, searchPath, zone));
    }
} // namespace

void ObjLoader::ConfigureCreatorCollection(AssetCreatorCollection& collection, Zone& zone, ISearchPath& searchPath, IGdtQueryable& gdt) const
{
    ConfigureDefaultCreators(collection, zone);
    ConfigureLoaders(collection, zone, searchPath, gdt);
    ConfigureGlobalAssetPoolsLoaders(collection, zone);
}
