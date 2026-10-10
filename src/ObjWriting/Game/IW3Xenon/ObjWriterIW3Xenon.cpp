#include "ObjWriterIW3Xenon.h"

#include "Game/IW3Xenon/Image/ImageDumperIW3Xenon.h"
#include "Localize/LocalizeDumperIW3Xenon.h"
#include "RawFile/RawFileDumperIW3Xenon.h"
#include "StringTable/StringTableDumperIW3Xenon.h"

using namespace IW3Xenon;

void ObjWriter::RegisterAssetDumpers(AssetDumpingContext& context)
{
    RegisterAssetDumper(std::make_unique<image::DumperIW3Xenon>());
    RegisterAssetDumper(std::make_unique<localize::DumperIW3Xenon>());
    RegisterAssetDumper(std::make_unique<raw_file::DumperIW3Xenon>());
    RegisterAssetDumper(std::make_unique<string_table::DumperIW3Xenon>());
}
