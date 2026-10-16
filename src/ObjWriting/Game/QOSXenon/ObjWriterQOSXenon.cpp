#include "ObjWriterQOSXenon.h"

#include "Image/ImageDumperQOSXenon.h"
#include "Localize/LocalizeDumperQOSXenon.h"
#include "Maps/MapEntsDumperQOSXenon.h"
#include "RawFile/RawFileDumperQOSXenon.h"
#include "Sound/SndCurveDumperQOSXenon.h"
#include "StringTable/StringTableDumperQOSXenon.h"

using namespace QOSXenon;

void ObjWriter::RegisterAssetDumpers(AssetDumpingContext& context)
{
    RegisterAssetDumper(std::make_unique<image::DumperQOSXenon>());
    RegisterAssetDumper(std::make_unique<localize::DumperQOSXenon>());
    RegisterAssetDumper(std::make_unique<map_ents::DumperQOSXenon>());
    RegisterAssetDumper(std::make_unique<raw_file::DumperQOSXenon>());
    RegisterAssetDumper(std::make_unique<sound_curve::DumperQOSXenon>());
    RegisterAssetDumper(std::make_unique<string_table::DumperQOSXenon>());
}
