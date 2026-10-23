#include "ObjWriterIW3Xenon253.h"

#include "Image/ImageDumperIW3Xenon253.h"
#include "Localize/LocalizeDumperIW3Xenon253.h"
#include "Maps/MapEntsDumperIW3Xenon253.h"
#include "RawFile/RawFileDumperIW3Xenon253.h"
#include "Sound/SndCurveDumperIW3Xenon253.h"
#include "XAnim/XAnimDumperIW3Xenon253.h"
#include "XModel/XModelDumperIW3Xenon253.h"

using namespace IW3Xenon253;

void ObjWriter::RegisterAssetDumpers(AssetDumpingContext& context)
{
    RegisterAssetDumper(std::make_unique<xmodel::DumperIW3Xenon253>());
    RegisterAssetDumper(std::make_unique<image::DumperIW3Xenon253>());
    RegisterAssetDumper(std::make_unique<localize::DumperIW3Xenon253>());
    RegisterAssetDumper(std::make_unique<map_ents::DumperIW3Xenon253>());
    RegisterAssetDumper(std::make_unique<raw_file::DumperIW3Xenon253>());
    RegisterAssetDumper(std::make_unique<sound_curve::DumperIW3Xenon253>());
    RegisterAssetDumper(std::make_unique<xanim::DumperIW3Xenon253>());
}
