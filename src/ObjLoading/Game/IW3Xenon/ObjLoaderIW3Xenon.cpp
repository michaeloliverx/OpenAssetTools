#include "ObjLoaderIW3Xenon.h"

#include "Asset/GlobalAssetPoolsLoader.h"
#include "Game/IW3Xenon/AssetMarkerIW3Xenon.h"
#include "Game/IW3Xenon/Material/LoaderMaterialIW3Xenon.h"
#include "Game/IW3Xenon/Weapon/AccuracyGraphLoaderIW3Xenon.h"
#include "Game/IW3Xenon/Weapon/WeaponGdtLoaderIW3Xenon.h"
#include "Game/IW3Xenon/Weapon/WeaponRawLoaderIW3Xenon.h"
#include "Game/IW3Xenon/XAnim/XAnimLoaderIW3Xenon.h"
#include "Game/IW3Xenon/XModel/LoaderXModelIW3Xenon.h"
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

    collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetPhysPreset>>(memory));
    collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetXAnim>>(memory));
    collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetXModel>>(memory));
    collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetMaterial>>(memory));
    collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetPixelShader>>(memory));
    collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetTechniqueSet>>(memory));
    collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetImage>>(memory));
    collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetSound>>(memory));
    collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetSoundCurve>>(memory));
    collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetLoadedSound>>(memory));
    collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetLocalize>>(memory));
    collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetWeapon>>(memory));
    collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetFx>>(memory));
    collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetRawFile>>(memory));
    collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetStringTable>>(memory));

    collection.AddAssetCreator(xanim::CreateLoaderIW3Xenon(memory, searchPath, zone));
    collection.AddAssetCreator(xmodel::CreateLoaderIW3Xenon(memory, searchPath, zone));
    collection.AddAssetCreator(material::CreateLoaderIW3Xenon(memory, searchPath));
    collection.AddAssetCreator(image::CreateLoaderIW3Xenon(memory, searchPath));
    collection.AddAssetCreator(localize::CreateLoaderIW3Xenon(memory, searchPath, zone));
    collection.AddAssetCreator(weapon::CreateRawLoaderIW3Xenon(memory, searchPath, zone));
    collection.AddAssetCreator(weapon::CreateGdtLoaderIW3Xenon(memory, searchPath, gdt, zone));
    collection.AddAssetCreator(raw_file::CreateLoaderIW3Xenon(memory, searchPath));
    collection.AddAssetCreator(string_table::CreateLoaderIW3Xenon(memory, searchPath));

    collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetPhysPreset>>(zone));
    collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetXAnim>>(zone));
    collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetXModel>>(zone));
    collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetMaterial>>(zone));
    collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetPixelShader>>(zone));
    collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetTechniqueSet>>(zone));
    collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetImage>>(zone));
    collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetSound>>(zone));
    collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetSoundCurve>>(zone));
    collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetLoadedSound>>(zone));
    collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetLocalize>>(zone));
    collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetWeapon>>(zone));
    collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetFx>>(zone));
    collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetRawFile>>(zone));
    collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetStringTable>>(zone));

    collection.AddSubAssetCreator(weapon::CreateAccuracyGraphLoaderIW3Xenon(memory, searchPath));
}
