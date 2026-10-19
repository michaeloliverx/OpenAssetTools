#include "ObjLoaderIW2Xenon.h"

#include "Asset/GlobalAssetPoolsLoader.h"
#include "Game/IW2Xenon/AssetMarkerIW2Xenon.h"
#include "Game/IW2Xenon/IW2Xenon.h"
#include "Localize/AssetLoaderLocalizeIW2Xenon.h"
#include "RawFile/AssetLoaderRawFileIW2Xenon.h"
#include "Sound/LoaderSoundCurveIW2Xenon.h"

using namespace IW2Xenon;

void ObjLoader::LoadReferencedContainersForZone(ISearchPath& searchPath, Zone& zone) const {}

void ObjLoader::UnloadContainersOfZone(Zone& zone) const {}

namespace
{
    void ConfigureDefaultCreators(AssetCreatorCollection& collection, Zone& zone)
    {
        auto& memory = zone.Memory();
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetXAnim>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetXModel>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetMaterial>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetImage>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetSound>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetSoundCurve>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetClipMapSp>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetClipMapMp>>(memory));
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
    }

    void ConfigureLoaders(AssetCreatorCollection& collection, Zone& zone, ISearchPath& searchPath)
    {
        auto& memory = zone.Memory();
        collection.AddAssetCreator(localize::CreateLoaderIW2Xenon(memory, searchPath, zone));
        collection.AddAssetCreator(raw_file::CreateLoaderIW2Xenon(memory, searchPath));
        collection.AddAssetCreator(sound_curve::CreateLoaderIW2Xenon(memory, searchPath));
    }

    void ConfigureGlobalAssetPoolsLoaders(AssetCreatorCollection& collection, Zone& zone)
    {
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetXAnim>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetXModel>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetMaterial>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetImage>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetSound>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetSoundCurve>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetClipMapMp>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetGfxWorld>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetLightDef>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetFont>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetMenuList>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetMenu>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetLocalize>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetWeapon>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetSoundDriverGlobals>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetFx>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetImpactFx>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetRawFile>>(zone));
    }
} // namespace

void ObjLoader::ConfigureCreatorCollection(AssetCreatorCollection& collection, Zone& zone, ISearchPath& searchPath, IGdtQueryable& gdt) const
{
    ConfigureDefaultCreators(collection, zone);
    ConfigureLoaders(collection, zone, searchPath);
    ConfigureGlobalAssetPoolsLoaders(collection, zone);
}
