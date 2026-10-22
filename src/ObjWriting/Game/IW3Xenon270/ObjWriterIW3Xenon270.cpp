#include "ObjWriterIW3Xenon270.h"

#include "Image/ImageDumperIW3Xenon270.h"
#include "Localize/LocalizeDumperIW3Xenon270.h"
#include "Maps/MapEntsDumperIW3Xenon270.h"
#include "RawFile/RawFileDumperIW3Xenon270.h"
#include "Sound/SndCurveDumperIW3Xenon270.h"
#include "StringTable/StringTableDumperIW3Xenon270.h"

using namespace IW3Xenon270;

void ObjWriter::RegisterAssetDumpers(AssetDumpingContext& context)
{
    RegisterAssetDumper(std::make_unique<image::DumperIW3Xenon270>());
    RegisterAssetDumper(std::make_unique<localize::DumperIW3Xenon270>());
    RegisterAssetDumper(std::make_unique<map_ents::DumperIW3Xenon270>());
    RegisterAssetDumper(std::make_unique<raw_file::DumperIW3Xenon270>());
    RegisterAssetDumper(std::make_unique<sound_curve::DumperIW3Xenon270>());
    RegisterAssetDumper(std::make_unique<string_table::DumperIW3Xenon270>());
}
