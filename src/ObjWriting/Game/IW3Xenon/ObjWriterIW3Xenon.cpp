#include "ObjWriterIW3Xenon.h"

#include "Game/IW3Xenon/Font/FontDumperIW3Xenon.h"
#include "Game/IW3Xenon/Image/ImageDumperIW3Xenon.h"
#include "Game/IW3Xenon/Maps/MapEntsDumperIW3Xenon.h"
#include "Game/IW3Xenon/Material/MaterialJsonDumperIW3Xenon.h"
#include "Game/IW3Xenon/Techset/PixelShaderDumperIW3Xenon.h"
#include "Game/IW3Xenon/Weapon/WeaponDumperIW3Xenon.h"
#include "Game/IW3Xenon/XAnim/XAnimDumperIW3Xenon.h"
#include "Game/IW3Xenon/XModel/XModelDumperIW3Xenon.h"
#include "LightDef/LightDefDumperIW3Xenon.h"
#include "Localize/LocalizeDumperIW3Xenon.h"
#include "PhysPreset/PhysPresetInfoStringDumperIW3Xenon.h"
#include "RawFile/RawFileDumperIW3Xenon.h"
#include "Sound/LoadedSoundDumperIW3Xenon.h"
#include "Sound/SndCurveDumperIW3Xenon.h"
#include "Sound/SoundDumperIW3Xenon.h"
#include "StringTable/StringTableDumperIW3Xenon.h"

using namespace IW3Xenon;

void ObjWriter::RegisterAssetDumpers(AssetDumpingContext& context)
{
    RegisterAssetDumper(std::make_unique<phys_preset::InfoStringDumperIW3Xenon>());
    RegisterAssetDumper(std::make_unique<xanim::DumperIW3Xenon>());
    RegisterAssetDumper(std::make_unique<xmodel::DumperIW3Xenon>());
    RegisterAssetDumper(std::make_unique<material::JsonDumperIW3Xenon>());
    RegisterAssetDumper(std::make_unique<techset::PixelShaderDumperIW3Xenon>());
    RegisterAssetDumper(std::make_unique<image::DumperIW3Xenon>());
    RegisterAssetDumper(std::make_unique<sound::SoundDumperIW3Xenon>());
    RegisterAssetDumper(std::make_unique<sound_curve::DumperIW3Xenon>());
    RegisterAssetDumper(std::make_unique<sound::LoadedSoundDumperIW3Xenon>());
    RegisterAssetDumper(std::make_unique<map_ents::DumperIW3Xenon>());
    RegisterAssetDumper(std::make_unique<light_def::DumperIW3Xenon>());
    RegisterAssetDumper(std::make_unique<font::JsonDumperIW3Xenon>());
    RegisterAssetDumper(std::make_unique<localize::DumperIW3Xenon>());
    RegisterAssetDumper(std::make_unique<weapon::DumperIW3Xenon>());
    RegisterAssetDumper(std::make_unique<raw_file::DumperIW3Xenon>());
    RegisterAssetDumper(std::make_unique<string_table::DumperIW3Xenon>());
}
