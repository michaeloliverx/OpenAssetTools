#include "ObjWriterT4Xenon302.h"

#include "Maps/D3DBspDumperT4Xenon302.h"

void T4Xenon302::ObjWriter::RegisterAssetDumpers(AssetDumpingContext&)
{
    RegisterAssetDumper(std::make_unique<map_d3dbsp::DumperT4Xenon302>());
}
