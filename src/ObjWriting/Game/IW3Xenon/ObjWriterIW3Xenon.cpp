#include "ObjWriterIW3Xenon.h"

#include "RawFile/RawFileDumperIW3Xenon.h"

using namespace IW3Xenon;

void ObjWriter::RegisterAssetDumpers(AssetDumpingContext& context)
{
    RegisterAssetDumper(std::make_unique<raw_file::DumperIW3Xenon>());
}
