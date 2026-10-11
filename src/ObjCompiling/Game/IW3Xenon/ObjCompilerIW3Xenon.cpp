#include "ObjCompilerIW3Xenon.h"

#include "Game/IW3Xenon/Techset/TechsetCompilerIW3Xenon.h"

using namespace IW3Xenon;

void ObjCompiler::ConfigureCreatorCollection(AssetCreatorCollection& collection,
                                             Zone& zone,
                                             const ZoneDefinitionContext& zoneDefinition,
                                             ISearchPath& searchPath,
                                             IGdtQueryable& gdt,
                                             ZoneAssetCreationStateContainer& zoneStates,
                                             IOutputPath& outDir,
                                             IOutputPath& cacheDir) const
{
    auto& memory = zone.Memory();
    collection.AddAssetCreator(techset::CreateTechsetCompilerIW3Xenon(memory, searchPath));
    collection.AddSubAssetCreator(techset::CreateTechniqueCompilerIW3Xenon(memory, searchPath));
}
