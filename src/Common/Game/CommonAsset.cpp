#include "CommonAsset.h"

#include "IW3/CommonAssetIW3.h"
#include "IW3Xenon/CommonAssetIW3Xenon.h"
#include "IW3Xenon253/CommonAssetIW3Xenon253.h"
#include "IW3Xenon270/CommonAssetIW3Xenon270.h"
#include "IW3Xenon290/CommonAssetIW3Xenon290.h"
#include "IW4/CommonAssetIW4.h"
#include "IW4Xenon/CommonAssetIW4Xenon.h"
#include "IW5/CommonAssetIW5.h"
#include "NX1/CommonAssetNX1.h"
#include "QOS/CommonAssetQOS.h"
#include "QOSXenon/CommonAssetQOSXenon.h"
#include "T4/CommonAssetT4.h"
#include "T4Xenon/CommonAssetT4Xenon.h"
#include "T5/CommonAssetT5.h"
#include "T5Xenon/CommonAssetT5Xenon.h"
#include "T6/CommonAssetT6.h"

#include <cassert>

ICommonAssetTypeMapper* ICommonAssetTypeMapper::GetCommonAssetMapperByGame(GameId gameId)
{
    static ICommonAssetTypeMapper* assetTypeMappers[]{
        new IW3::CommonAssetTypeMapper(),
        new IW3Xenon::CommonAssetTypeMapper(),
        new IW4::CommonAssetTypeMapper(),
        new IW4Xenon::CommonAssetTypeMapper(),
        new IW5::CommonAssetTypeMapper(),
        new QOS::CommonAssetTypeMapper(),
        new T4::CommonAssetTypeMapper(),
        new T4Xenon::CommonAssetTypeMapper(),
        new T5::CommonAssetTypeMapper(),
        new T6::CommonAssetTypeMapper(),
        new NX1::CommonAssetTypeMapper(),
        new T5Xenon::CommonAssetTypeMapper(),
        new QOSXenon::CommonAssetTypeMapper(),
        new IW3Xenon253::CommonAssetTypeMapper(),
        new IW3Xenon270::CommonAssetTypeMapper(),
        new IW3Xenon290::CommonAssetTypeMapper(),
    };
    static_assert(std::extent_v<decltype(assetTypeMappers)> == static_cast<unsigned>(GameId::COUNT));

    assert(static_cast<unsigned>(gameId) < static_cast<unsigned>(GameId::COUNT));
    auto* result = assetTypeMappers[static_cast<unsigned>(gameId)];
    assert(result);

    return result;
}
