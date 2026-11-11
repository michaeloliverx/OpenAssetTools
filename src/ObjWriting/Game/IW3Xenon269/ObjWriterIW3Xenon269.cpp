#include "ObjWriterIW3Xenon269.h"

#include "Maps/D3DBspDumperIW3Xenon269.h"

void IW3Xenon269::ObjWriter::RegisterAssetDumpers(AssetDumpingContext&)
{
    RegisterAssetDumper(std::make_unique<map_d3dbsp::DumperIW3Xenon269>());
}
