#include "ObjWriterIW4Xenon.h"

#include "Game/IW4Xenon/Maps/MapEntsDumperIW4Xenon.h"
#include "Image/ImageDumperIW4Xenon.h"
#include "LightDef/LightDefDumperIW4Xenon.h"
#include "Localize/LocalizeDumperIW4Xenon.h"
#include "Maps/AddonMapEntsDumperIW4Xenon.h"
#include "Material/MaterialJsonDumperIW4Xenon.h"
#include "PhysPreset/PhysPresetInfoStringDumperIW4Xenon.h"
#include "RawFile/RawFileDumperIW4Xenon.h"
#include "Sound/SndCurveDumperIW4Xenon.h"
#include "StringTable/StringTableDumperIW4Xenon.h"
#include "XAnim/XAnimDumperIW4Xenon.h"
#include "XModel/XModelDumperIW4Xenon.h"

using namespace IW4Xenon;

void ObjWriter::RegisterAssetDumpers(AssetDumpingContext& context)
{
    RegisterAssetDumper(std::make_unique<phys_preset::InfoStringDumperIW4Xenon>());
    RegisterAssetDumper(std::make_unique<light_def::DumperIW4Xenon>());
    RegisterAssetDumper(std::make_unique<material::JsonDumperIW4Xenon>());
    RegisterAssetDumper(std::make_unique<xanim::DumperIW4Xenon>());
    RegisterAssetDumper(std::make_unique<xmodel::DumperIW4Xenon>());
    RegisterAssetDumper(std::make_unique<image::DumperIW4Xenon>());
    RegisterAssetDumper(std::make_unique<sound_curve::DumperIW4Xenon>());
    RegisterAssetDumper(std::make_unique<map_ents::DumperIW4Xenon>());
    RegisterAssetDumper(std::make_unique<localize::DumperIW4Xenon>());
    RegisterAssetDumper(std::make_unique<raw_file::DumperIW4Xenon>());
    RegisterAssetDumper(std::make_unique<string_table::DumperIW4Xenon>());
    RegisterAssetDumper(std::make_unique<addon_map_ents::DumperIW4Xenon>());
}
