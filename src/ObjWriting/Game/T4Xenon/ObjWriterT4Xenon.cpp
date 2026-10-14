#include "ObjWriterT4Xenon.h"

#include "Game/T4Xenon/Image/ImageDumperT4Xenon.h"
#include "Game/T4Xenon/Maps/MapEntsDumperT4Xenon.h"
#include "Game/T4Xenon/XModel/XModelDumperT4Xenon.h"
#include "Localize/LocalizeDumperT4Xenon.h"
#include "RawFile/RawFileDumperT4Xenon.h"
#include "StringTable/StringTableDumperT4Xenon.h"

using namespace T4Xenon;

void ObjWriter::RegisterAssetDumpers(AssetDumpingContext& context)
{
    RegisterAssetDumper(std::make_unique<xmodel::DumperT4Xenon>());
    RegisterAssetDumper(std::make_unique<image::DumperT4Xenon>());
    RegisterAssetDumper(std::make_unique<map_ents::DumperT4Xenon>());
    RegisterAssetDumper(std::make_unique<localize::DumperT4Xenon>());
    RegisterAssetDumper(std::make_unique<raw_file::DumperT4Xenon>());
    RegisterAssetDumper(std::make_unique<string_table::DumperT4Xenon>());
}
