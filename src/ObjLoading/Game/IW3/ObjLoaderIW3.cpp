#include "ObjLoaderIW3.h"

#include "Asset/GlobalAssetPoolsLoader.h"
#include "Game/IW3/AssetMarkerIW3.h"
#include "Game/IW3Xenon/AssetMarkerIW3Xenon.h"
#include "Game/IW3/GameIW3.h"
#include "Game/IW3/IW3.h"
#include "Game/IW3/XModel/LoaderXModelIW3.h"
#include "Game/IW3Xenon/IW3Xenon.h"
#include "Image/AssetLoaderImageIW3.h"
#include "Localize/AssetLoaderLocalizeIW3.h"
#include "Material/LoaderMaterialIW3.h"
#include "ObjLoading.h"
#include "RawFile/AssetLoaderRawFileIW3.h"
#include "StringTable/AssetLoaderStringTableIW3.h"

#include <memory>

using namespace IW3;

void ObjLoader::LoadReferencedContainersForZone(ISearchPath& searchPath, Zone& zone) const {}

void ObjLoader::UnloadContainersOfZone(Zone& zone) const {}

namespace
{
    void ConfigureDefaultCreators(AssetCreatorCollection& collection, Zone& zone)
    {
        auto& memory = zone.Memory();

        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetPhysPreset>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetXAnim>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetXModel>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetMaterial>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetTechniqueSet>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetImage>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetSound>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetSoundCurve>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetLoadedSound>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetClipMap>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetClipMapPvs>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetComWorld>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetGameWorldSp>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetGameWorldMp>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetMapEnts>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetGfxWorld>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetLightDef>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetFont>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetMenuList>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetMenu>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetLocalize>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetWeapon>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetSoundDriverGlobals>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetFx>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetImpactFx>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetRawFile>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetStringTable>>(memory));
    }

    void ConfigureGlobalAssetPoolsLoaders(AssetCreatorCollection& collection, Zone& zone)
    {
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetPhysPreset>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetXAnim>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetXModel>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetMaterial>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetTechniqueSet>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetImage>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetSound>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetSoundCurve>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetLoadedSound>>(zone));
        // collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetClipMap>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetClipMapPvs>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetComWorld>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetGameWorldSp>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetGameWorldMp>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetMapEnts>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetGfxWorld>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetLightDef>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetFont>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetMenuList>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetMenu>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetLocalize>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetWeapon>>(zone));
        // collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetSoundDriverGlobals>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetFx>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetImpactFx>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetRawFile>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetStringTable>>(zone));
    }

    void ConfigureDefaultCreatorsXenon(AssetCreatorCollection& collection, Zone& zone)
    {
        auto& memory = zone.Memory();

        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<IW3Xenon::AssetPhysPreset>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<IW3Xenon::AssetXAnim>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<IW3Xenon::AssetXModel>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<IW3Xenon::AssetMaterial>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<IW3Xenon::AssetPixelShader>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<IW3Xenon::AssetTechniqueSet>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<IW3Xenon::AssetImage>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<IW3Xenon::AssetSound>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<IW3Xenon::AssetSoundCurve>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<IW3Xenon::AssetLoadedSound>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<IW3Xenon::AssetClipMap>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<IW3Xenon::AssetClipMapPvs>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<IW3Xenon::AssetComWorld>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<IW3Xenon::AssetGameWorldSp>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<IW3Xenon::AssetGameWorldMp>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<IW3Xenon::AssetMapEnts>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<IW3Xenon::AssetGfxWorld>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<IW3Xenon::AssetLightDef>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<IW3Xenon::AssetFont>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<IW3Xenon::AssetMenuList>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<IW3Xenon::AssetMenu>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<IW3Xenon::AssetLocalize>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<IW3Xenon::AssetWeapon>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<IW3Xenon::AssetSndDriverGlobals>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<IW3Xenon::AssetFx>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<IW3Xenon::AssetImpactFx>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<IW3Xenon::AssetRawFile>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<IW3Xenon::AssetStringTable>>(memory));
    }

    void ConfigureGlobalAssetPoolsLoadersXenon(AssetCreatorCollection& collection, Zone& zone)
    {
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<IW3Xenon::AssetPhysPreset>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<IW3Xenon::AssetXAnim>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<IW3Xenon::AssetXModel>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<IW3Xenon::AssetMaterial>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<IW3Xenon::AssetPixelShader>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<IW3Xenon::AssetTechniqueSet>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<IW3Xenon::AssetImage>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<IW3Xenon::AssetSound>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<IW3Xenon::AssetSoundCurve>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<IW3Xenon::AssetLoadedSound>>(zone));
        // collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<IW3Xenon::AssetClipMap>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<IW3Xenon::AssetClipMapPvs>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<IW3Xenon::AssetComWorld>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<IW3Xenon::AssetGameWorldSp>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<IW3Xenon::AssetGameWorldMp>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<IW3Xenon::AssetMapEnts>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<IW3Xenon::AssetGfxWorld>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<IW3Xenon::AssetLightDef>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<IW3Xenon::AssetFont>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<IW3Xenon::AssetMenuList>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<IW3Xenon::AssetMenu>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<IW3Xenon::AssetLocalize>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<IW3Xenon::AssetWeapon>>(zone));
        // collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<IW3Xenon::AssetSndDriverGlobals>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<IW3Xenon::AssetFx>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<IW3Xenon::AssetImpactFx>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<IW3Xenon::AssetRawFile>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<IW3Xenon::AssetStringTable>>(zone));
    }

    void ConfigureLoaders(AssetCreatorCollection& collection, Zone& zone, ISearchPath& searchPath)
    {
        auto& memory = zone.Memory();

        // collection.AddAssetCreator(std::make_unique<AssetLoaderPhysPreset>(memory));
        // collection.AddAssetCreator(std::make_unique<AssetLoaderXAnim>(memory));
        collection.AddAssetCreator(xmodel::CreateLoaderIW3(memory, searchPath, zone));
        collection.AddAssetCreator(material::CreateLoaderIW3(memory, searchPath));
        // collection.AddAssetCreator(std::make_unique<AssetLoaderTechniqueSet>(memory));
        collection.AddAssetCreator(image::CreateLoaderIW3(memory, searchPath));
        // collection.AddAssetCreator(std::make_unique<AssetLoaderSound>(memory));
        // collection.AddAssetCreator(std::make_unique<AssetLoaderSoundCurve>(memory));
        // collection.AddAssetCreator(std::make_unique<AssetLoaderLoadedSound>(memory));
        // collection.AddAssetCreator(std::make_unique<AssetLoaderClipMapPvs>(memory));
        // collection.AddAssetCreator(std::make_unique<AssetLoaderComWorld>(memory));
        // collection.AddAssetCreator(std::make_unique<AssetLoaderGameWorldSp>(memory));
        // collection.AddAssetCreator(std::make_unique<AssetLoaderGameWorldMp>(memory));
        // collection.AddAssetCreator(std::make_unique<AssetLoaderMapEnts>(memory));
        // collection.AddAssetCreator(std::make_unique<AssetLoaderGfxWorld>(memory));
        // collection.AddAssetCreator(std::make_unique<AssetLoaderLightDef>(memory));
        // collection.AddAssetCreator(std::make_unique<AssetLoaderFont>(memory));
        // collection.AddAssetCreator(std::make_unique<AssetLoaderMenuList>(memory));
        // collection.AddAssetCreator(std::make_unique<AssetLoaderMenu>(memory));
        collection.AddAssetCreator(localize::CreateLoaderIW3(memory, searchPath, zone));
        // collection.AddAssetCreator(std::make_unique<AssetLoaderWeapon>(memory));
        // collection.AddAssetCreator(std::make_unique<AssetLoaderSoundDriverGlobals>(memory));
        // collection.AddAssetCreator(std::make_unique<AssetLoaderFx>(memory));
        // collection.AddAssetCreator(std::make_unique<AssetLoaderImpactFx>(memory));
        collection.AddAssetCreator(raw_file::CreateLoaderIW3(memory, searchPath));
        collection.AddAssetCreator(string_table::CreateLoaderIW3(memory, searchPath));
    }
} // namespace

void ObjLoader::ConfigureCreatorCollection(AssetCreatorCollection& collection, Zone& zone, ISearchPath& searchPath, IGdtQueryable& gdt) const
{
    if (zone.m_platform == GamePlatform::XBOX)
    {
        ConfigureDefaultCreatorsXenon(collection, zone);
        ConfigureGlobalAssetPoolsLoadersXenon(collection, zone);
    }
    else
    {
        ConfigureDefaultCreators(collection, zone);
        ConfigureLoaders(collection, zone, searchPath);
        ConfigureGlobalAssetPoolsLoaders(collection, zone);
    }
}
