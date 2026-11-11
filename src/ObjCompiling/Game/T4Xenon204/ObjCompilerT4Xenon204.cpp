#include "ObjCompilerT4Xenon204.h"

using namespace T4Xenon204;

void ObjCompiler::ConfigureCreatorCollection(AssetCreatorCollection& collection,
                                             Zone& zone,
                                             const ZoneDefinitionContext& zoneDefinition,
                                             ISearchPath& searchPath,
                                             IGdtQueryable& gdt,
                                             ZoneAssetCreationStateContainer& zoneStates,
                                             IOutputPath& outDir,
                                             IOutputPath& cacheDir) const
{
    (void)collection;
    (void)zone;
    (void)zoneDefinition;
    (void)searchPath;
    (void)gdt;
    (void)zoneStates;
    (void)outDir;
    (void)cacheDir;
}
