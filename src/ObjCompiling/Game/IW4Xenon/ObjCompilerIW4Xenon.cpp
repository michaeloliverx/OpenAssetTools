#include "ObjCompilerIW4Xenon.h"

#include "Game/IW4Xenon/Image/LoaderImageIW4Xenon.h"
#include "Utils/Logging/Log.h"

#include <charconv>

using namespace IW4Xenon;

void ObjCompiler::ConfigureCreatorCollection(AssetCreatorCollection& collection,
                                             Zone& zone,
                                             const ZoneDefinitionContext& zoneDefinition,
                                             ISearchPath& searchPath,
                                             IGdtQueryable& gdt,
                                             ZoneAssetCreationStateContainer& zoneStates,
                                             IOutputPath& outDir,
                                             IOutputPath& cacheDir) const
{
    const auto& properties = zoneDefinition.m_zone_definition.m_properties.m_properties;
    const auto imageFile = properties.find("imagefile");
    if (imageFile == properties.end())
        return;

    unsigned index = 0u;
    const auto& value = imageFile->second;
    const auto [end, error] = std::from_chars(value.data(), value.data() + value.size(), index);
    if (properties.count("imagefile") != 1u || error != std::errc() || end != value.data() + value.size() || index > 20u)
    {
        con::error("IW4 Xenon requires a single >imagefile,<index> directive with an index from 0 to 20");
        return;
    }

    collection.AddAssetCreator(image::CreateLoaderIW4Xenon(zone.Memory(), searchPath, zone, outDir, index));
}
