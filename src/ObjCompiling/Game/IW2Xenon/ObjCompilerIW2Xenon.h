#pragma once

#include "IObjCompiler.h"

namespace IW2Xenon
{
    class ObjCompiler final : public IObjCompiler
    {
    public:
        void ConfigureCreatorCollection(AssetCreatorCollection& collection,
                                        Zone& zone,
                                        const ZoneDefinitionContext& zoneDefinition,
                                        ISearchPath& searchPath,
                                        IGdtQueryable& gdt,
                                        ZoneAssetCreationStateContainer& zoneStates,
                                        IOutputPath& outDir,
                                        IOutputPath& cacheDir) const override;
    };
} // namespace IW2Xenon
