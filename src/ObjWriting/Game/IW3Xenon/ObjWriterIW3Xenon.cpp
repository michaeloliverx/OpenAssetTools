#include "ObjWriterIW3Xenon.h"

#include "RawFile/RawFileDumperIW3Xenon.h"
#include "StringTable/StringTableDumperIW3Xenon.h"

using namespace IW3Xenon;

void ObjWriter::RegisterAssetDumpers(AssetDumpingContext& context)
{
    RegisterAssetDumper(std::make_unique<raw_file::DumperIW3Xenon>());
    RegisterAssetDumper(std::make_unique<string_table::DumperIW3Xenon>());
}
