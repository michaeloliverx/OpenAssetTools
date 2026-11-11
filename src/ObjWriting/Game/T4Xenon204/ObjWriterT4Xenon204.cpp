#include "ObjWriterT4Xenon204.h"

#include "Maps/D3DBspDumperT4Xenon204.h"

void T4Xenon204::ObjWriter::RegisterAssetDumpers(AssetDumpingContext&)
{
    RegisterAssetDumper(std::make_unique<map_d3dbsp::DumperT4Xenon204>());
}
