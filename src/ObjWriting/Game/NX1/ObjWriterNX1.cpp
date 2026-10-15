#include "ObjWriterNX1.h"

#include "Game/NX1/Maps/MapEntsDumperNX1.h"
#include "Image/ImageDumperNX1.h"
#include "Localize/LocalizeDumperNX1.h"
#include "Maps/AddonMapEntsDumperNX1.h"
#include "RawFile/RawFileDumperNX1.h"
#include "Sound/SndCurveDumperNX1.h"
#include "StringTable/StringTableDumperNX1.h"

using namespace NX1;

void ObjWriter::RegisterAssetDumpers(AssetDumpingContext& context)
{
    RegisterAssetDumper(std::make_unique<image::DumperNX1>());
    RegisterAssetDumper(std::make_unique<sound_curve::DumperNX1>());
    RegisterAssetDumper(std::make_unique<map_ents::DumperNX1>());
    RegisterAssetDumper(std::make_unique<localize::DumperNX1>());
    RegisterAssetDumper(std::make_unique<raw_file::DumperNX1>());
    RegisterAssetDumper(std::make_unique<string_table::DumperNX1>());
    RegisterAssetDumper(std::make_unique<addon_map_ents::DumperNX1>());
}
