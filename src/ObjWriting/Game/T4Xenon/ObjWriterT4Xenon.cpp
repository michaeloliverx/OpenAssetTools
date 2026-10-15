#include "ObjWriterT4Xenon.h"

#include "Game/T4Xenon/Font/FontDumperT4Xenon.h"
#include "Game/T4Xenon/Image/ImageDumperT4Xenon.h"
#include "Game/T4Xenon/Maps/MapEntsDumperT4Xenon.h"
#include "Game/T4Xenon/Material/MaterialJsonDumperT4Xenon.h"
#include "Game/T4Xenon/XAnim/XAnimDumperT4Xenon.h"
#include "Game/T4Xenon/XModel/XModelDumperT4Xenon.h"
#include "Localize/LocalizeDumperT4Xenon.h"
#include "Menu/MenuDumperT4Xenon.h"
#include "Menu/MenuListDumperT4Xenon.h"
#include "PhysPreset/PhysPresetInfoStringDumperT4Xenon.h"
#include "RawFile/RawFileDumperT4Xenon.h"
#include "Sound/LoadedSoundDumperT4Xenon.h"
#include "Sound/SoundDumperT4Xenon.h"
#include "StringTable/StringTableDumperT4Xenon.h"
#include "Techset/PixelShaderDumperT4Xenon.h"
#include "Weapon/WeaponDumperT4Xenon.h"

using namespace T4Xenon;

void ObjWriter::RegisterAssetDumpers(AssetDumpingContext& context)
{
    RegisterAssetDumper(std::make_unique<phys_preset::InfoStringDumperT4Xenon>());
    RegisterAssetDumper(std::make_unique<xanim::DumperT4Xenon>());
    RegisterAssetDumper(std::make_unique<xmodel::DumperT4Xenon>());
    RegisterAssetDumper(std::make_unique<material::JsonDumperT4Xenon>());
    RegisterAssetDumper(std::make_unique<techset::PixelShaderDumperT4Xenon>());
    RegisterAssetDumper(std::make_unique<image::DumperT4Xenon>());
    RegisterAssetDumper(std::make_unique<sound::SoundDumperT4Xenon>());
    RegisterAssetDumper(std::make_unique<sound::LoadedSoundDumperT4Xenon>());
    RegisterAssetDumper(std::make_unique<map_ents::DumperT4Xenon>());
    RegisterAssetDumper(std::make_unique<font::JsonDumperT4Xenon>());
    RegisterAssetDumper(std::make_unique<menu::MenuListDumperT4Xenon>());
    RegisterAssetDumper(std::make_unique<menu::MenuDumperT4Xenon>());
    RegisterAssetDumper(std::make_unique<localize::DumperT4Xenon>());
    RegisterAssetDumper(std::make_unique<raw_file::DumperT4Xenon>());
    RegisterAssetDumper(std::make_unique<string_table::DumperT4Xenon>());
    RegisterAssetDumper(std::make_unique<weapon::DumperT4Xenon>());
}
