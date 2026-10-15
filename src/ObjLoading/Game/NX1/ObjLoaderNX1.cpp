#include "ObjLoaderNX1.h"

#include "Asset/GlobalAssetPoolsLoader.h"
#include "Game/NX1/AssetMarkerNX1.h"
#include "Game/NX1/Maps/MapEntsLoaderNX1.h"
#include "Game/NX1/NX1.h"
#include "Image/ImageStreamPostProcessorNX1.h"
#include "Localize/LoaderLocalizeNX1.h"
#include "RawFile/LoaderRawFileNX1.h"
#include "Sound/LoaderSoundCurveNX1.h"
#include "StringTable/LoaderStringTableNX1.h"

using namespace NX1;

DEFINE_MARKER_CLASS_FOR_ASSET(AssetClipMapSp, AssetMarker<AssetClipMapMp>)

void ObjLoader::LoadReferencedContainersForZone(ISearchPath& searchPath, Zone& zone) const {}

void ObjLoader::UnloadContainersOfZone(Zone& zone) const {}

namespace
{
    void ConfigureDefaultCreators(AssetCreatorCollection& collection, Zone& zone)
    {
        auto& memory = zone.Memory();

        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetPhysPreset>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetPhysCollMap>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetXAnim>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetXModelSurfs>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetXModel>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetMaterial>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetPixelShader>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetTechniqueSet>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetImage>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetSound>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetSndVfCurve>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetSndLpfCurve>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetLoadedSound>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetClipMapSp>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetClipMapMp>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetComWorld>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetGameWorldSp>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetGameWorldMp>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetMapEnts>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetFxWorld>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetGfxWorld>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetLightDef>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetFont>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetMenuList>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetMenu>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetLocalize>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetWeapon>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetSndDriverGlobals>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetFx>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetImpactFx>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetSurfaceFx>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetRawFile>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetStringTable>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetLeaderboard>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetStructuredDataDef>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetTracer>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetLaser>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetVehicle>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetAddonMapEnts>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetShockFile>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetVolumeSetting>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetReverbPreset>>(memory));
        collection.AddDefaultAssetCreator(std::make_unique<DefaultAssetCreator<AssetFog>>(memory));
    }

    void ConfigureGlobalAssetPoolsLoaders(AssetCreatorCollection& collection, Zone& zone)
    {
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetPhysPreset>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetPhysCollMap>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetXAnim>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetXModel>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetMaterial>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetPixelShader>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetTechniqueSet>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetImage>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetSound>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetSndVfCurve>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetSndLpfCurve>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetLoadedSound>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetClipMapSp>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetClipMapMp>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetComWorld>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetGameWorldSp>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetGameWorldMp>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetMapEnts>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetFxWorld>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetGfxWorld>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetLightDef>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetFont>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetMenuList>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetMenu>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetLocalize>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetWeapon>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetSndDriverGlobals>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetFx>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetImpactFx>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetSurfaceFx>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetRawFile>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetStringTable>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetLeaderboard>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetStructuredDataDef>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetTracer>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetLaser>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetVehicle>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetAddonMapEnts>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetShockFile>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetVolumeSetting>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetReverbPreset>>(zone));
        collection.AddAssetCreator(std::make_unique<GlobalAssetPoolsLoader<AssetFog>>(zone));
    }

    void ConfigureLoaders(AssetCreatorCollection& collection, Zone& zone, ISearchPath& searchPath, IGdtQueryable& gdt)
    {
        auto& memory = zone.Memory();

        collection.AddAssetCreator(sound_curve::CreateLoaderNX1(memory, searchPath));
        collection.AddAssetCreator(map_ents::CreateLoaderNX1(memory, searchPath));
        collection.AddAssetCreator(localize::CreateLoaderNX1(memory, searchPath, zone));
        collection.AddAssetCreator(raw_file::CreateLoaderNX1(memory, searchPath));
        collection.AddAssetCreator(string_table::CreateLoaderNX1(memory, searchPath));
    }
} // namespace

void ObjLoader::ConfigureCreatorCollection(AssetCreatorCollection& collection, Zone& zone, ISearchPath& searchPath, IGdtQueryable& gdt) const
{
    collection.AddAssetPostProcessor(image::CreateStreamPostProcessorNX1(zone, searchPath));
    ConfigureDefaultCreators(collection, zone);
    ConfigureLoaders(collection, zone, searchPath, gdt);
    ConfigureGlobalAssetPoolsLoaders(collection, zone);
}
