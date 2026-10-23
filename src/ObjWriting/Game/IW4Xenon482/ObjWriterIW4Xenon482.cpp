#include "ObjWriterIW4Xenon482.h"

#include "Font/FontDumperIW4Xenon482.h"
#include "Game/IW4Xenon482/Maps/MapEntsDumperIW4Xenon482.h"
#include "Image/ImageDumperIW4Xenon482.h"
#include "Localize/LocalizeDumperIW4Xenon482.h"
#include "Maps/AddonMapEntsDumperIW4Xenon482.h"
#include "RawFile/RawFileDumperIW4Xenon482.h"
#include "Sound/SndCurveDumperIW4Xenon482.h"
#include "StringTable/StringTableDumperIW4Xenon482.h"
#include "XModel/XModelDumperIW4Xenon482.h"

using namespace IW4Xenon482;

void ObjWriter::RegisterAssetDumpers(AssetDumpingContext& context)
{
    RegisterAssetDumper(std::make_unique<xmodel::DumperIW4Xenon482>());
    RegisterAssetDumper(std::make_unique<image::DumperIW4Xenon482>());
    RegisterAssetDumper(std::make_unique<sound_curve::DumperIW4Xenon482>());
    RegisterAssetDumper(std::make_unique<map_ents::DumperIW4Xenon482>());
    RegisterAssetDumper(std::make_unique<font::JsonDumperIW4Xenon482>());
    RegisterAssetDumper(std::make_unique<localize::DumperIW4Xenon482>());
    RegisterAssetDumper(std::make_unique<raw_file::DumperIW4Xenon482>());
    RegisterAssetDumper(std::make_unique<string_table::DumperIW4Xenon482>());
    RegisterAssetDumper(std::make_unique<addon_map_ents::DumperIW4Xenon482>());
}
