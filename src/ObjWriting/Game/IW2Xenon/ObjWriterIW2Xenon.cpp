#include "ObjWriterIW2Xenon.h"

#include "Image/ImageDumperIW2Xenon.h"
#include "Localize/LocalizeDumperIW2Xenon.h"
#include "Maps/MapEntsDumperIW2Xenon.h"
#include "RawFile/RawFileDumperIW2Xenon.h"
#include "Sound/SndCurveDumperIW2Xenon.h"

using namespace IW2Xenon;

void ObjWriter::RegisterAssetDumpers(AssetDumpingContext& context)
{
    RegisterAssetDumper(std::make_unique<image::DumperIW2Xenon>());
    RegisterAssetDumper(std::make_unique<localize::DumperIW2Xenon>());
    RegisterAssetDumper(std::make_unique<map_ents::DumperIW2Xenon>());
    RegisterAssetDumper(std::make_unique<raw_file::DumperIW2Xenon>());
    RegisterAssetDumper(std::make_unique<sound_curve::DumperIW2Xenon>());
}
