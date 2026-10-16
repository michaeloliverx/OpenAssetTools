#include "ObjWriterT5Xenon.h"

#include "Game/T5Xenon/Maps/MapEntsDumperT5Xenon.h"
#include "Localize/LocalizeDumperT5Xenon.h"
#include "RawFile/RawFileDumperT5Xenon.h"
#include "StringTable/StringTableDumperT5Xenon.h"

using namespace T5Xenon;

void ObjWriter::RegisterAssetDumpers(AssetDumpingContext& context)
{
    RegisterAssetDumper(std::make_unique<map_ents::DumperT5Xenon>());
    RegisterAssetDumper(std::make_unique<localize::DumperT5Xenon>());
    RegisterAssetDumper(std::make_unique<raw_file::DumperT5Xenon>());
    RegisterAssetDumper(std::make_unique<string_table::DumperT5Xenon>());
}
