#include "ObjWriterIW3Xenon.h"

#include "Game/IW3Xenon/GameAssetPoolIW3Xenon.h"
#include "Image/ImageDumperIW3Xenon.h"
#include "Localize/LocalizeDumperIW3Xenon.h"
#include "Maps/MapEntsDumperIW3Xenon.h"
#include "ObjWriting.h"
#include "RawFile/RawFileDumperIW3Xenon.h"
#include "StringTable/StringTableDumperIW3Xenon.h"
#include "XModel/XModelDumperIW3Xenon.h"

using namespace IW3Xenon;

bool ObjWriter::DumpZone(AssetDumpingContext& context) const
{
#define REGISTER_DUMPER(dumperType, poolName)                                                                                                                  \
    if (assetPools->poolName && ObjWriting::ShouldHandleAssetType(dumperType::AssetType_t::EnumEntry))                                                         \
    {                                                                                                                                                          \
        dumpers.emplace_back(std::make_unique<dumperType>(*assetPools->poolName));                                                                             \
    }

    const auto* assetPools = dynamic_cast<GameAssetPoolIW3Xenon*>(context.m_zone.m_pools.get());
    std::vector<std::unique_ptr<IAssetDumper>> dumpers;

    REGISTER_DUMPER(xmodel::DumperIW3Xenon, m_xmodel)
    REGISTER_DUMPER(image::DumperIW3Xenon, m_image)
    REGISTER_DUMPER(map_ents::DumperIW3Xenon, m_map_ents)
    REGISTER_DUMPER(localize::DumperIW3Xenon, m_localize)
    REGISTER_DUMPER(raw_file::DumperIW3Xenon, m_raw_file)
    REGISTER_DUMPER(string_table::DumperIW3Xenon, m_string_table)

    if (context.ShouldTrackProgress())
    {
        size_t totalProgress = 0uz;
        for (const auto& dumper : dumpers)
            totalProgress += dumper->GetProgressTotalCount();

        context.SetTotalProgress(totalProgress);
    }

    for (const auto& dumper : dumpers)
        dumper->Dump(context);

    return true;

#undef REGISTER_DUMPER
}
