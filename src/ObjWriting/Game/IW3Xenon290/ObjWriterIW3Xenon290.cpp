#include "ObjWriterIW3Xenon290.h"

#include "Image/ImageDumperIW3Xenon290.h"
#include "Localize/LocalizeDumperIW3Xenon290.h"
#include "Maps/MapEntsDumperIW3Xenon290.h"
#include "RawFile/RawFileDumperIW3Xenon290.h"
#include "Sound/SndCurveDumperIW3Xenon290.h"
#include "StringTable/StringTableDumperIW3Xenon290.h"
#include "XAnim/XAnimDumperIW3Xenon290.h"

using namespace IW3Xenon290;

void ObjWriter::RegisterAssetDumpers(AssetDumpingContext& context)
{
    RegisterAssetDumper(std::make_unique<image::DumperIW3Xenon290>());
    RegisterAssetDumper(std::make_unique<localize::DumperIW3Xenon290>());
    RegisterAssetDumper(std::make_unique<map_ents::DumperIW3Xenon290>());
    RegisterAssetDumper(std::make_unique<raw_file::DumperIW3Xenon290>());
    RegisterAssetDumper(std::make_unique<sound_curve::DumperIW3Xenon290>());
    RegisterAssetDumper(std::make_unique<string_table::DumperIW3Xenon290>());
    RegisterAssetDumper(std::make_unique<xanim::DumperIW3Xenon290>());
}
