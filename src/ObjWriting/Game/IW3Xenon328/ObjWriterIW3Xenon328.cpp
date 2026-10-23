#include "ObjWriterIW3Xenon328.h"

#include "Image/ImageDumperIW3Xenon328.h"
#include "Localize/LocalizeDumperIW3Xenon328.h"
#include "Maps/MapEntsDumperIW3Xenon328.h"
#include "RawFile/RawFileDumperIW3Xenon328.h"
#include "Sound/SndCurveDumperIW3Xenon328.h"
#include "StringTable/StringTableDumperIW3Xenon328.h"
#include "XAnim/XAnimDumperIW3Xenon328.h"
#include "XModel/XModelDumperIW3Xenon328.h"

using namespace IW3Xenon328;

void ObjWriter::RegisterAssetDumpers(AssetDumpingContext& context)
{
    RegisterAssetDumper(std::make_unique<xmodel::DumperIW3Xenon328>());
    RegisterAssetDumper(std::make_unique<image::DumperIW3Xenon328>());
    RegisterAssetDumper(std::make_unique<localize::DumperIW3Xenon328>());
    RegisterAssetDumper(std::make_unique<map_ents::DumperIW3Xenon328>());
    RegisterAssetDumper(std::make_unique<raw_file::DumperIW3Xenon328>());
    RegisterAssetDumper(std::make_unique<sound_curve::DumperIW3Xenon328>());
    RegisterAssetDumper(std::make_unique<string_table::DumperIW3Xenon328>());
    RegisterAssetDumper(std::make_unique<xanim::DumperIW3Xenon328>());
}
