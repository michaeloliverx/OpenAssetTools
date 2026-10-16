#include "IZoneLoaderFactory.h"

#include "Game/IW3/ZoneLoaderFactoryIW3.h"
#include "Game/IW3Xenon/ZoneLoaderFactoryIW3Xenon.h"
#include "Game/IW4/ZoneLoaderFactoryIW4.h"
#include "Game/IW4Xenon/ZoneLoaderFactoryIW4Xenon.h"
#include "Game/IW5/ZoneLoaderFactoryIW5.h"
#include "Game/NX1/ZoneLoaderFactoryNX1.h"
#include "Game/QOS/ZoneLoaderFactoryQOS.h"
#include "Game/QOSXenon/ZoneLoaderFactoryQOSXenon.h"
#include "Game/T4/ZoneLoaderFactoryT4.h"
#include "Game/T4Xenon/ZoneLoaderFactoryT4Xenon.h"
#include "Game/T5/ZoneLoaderFactoryT5.h"
#include "Game/T5Xenon/ZoneLoaderFactoryT5Xenon.h"
#include "Game/T6/ZoneLoaderFactoryT6.h"

#include <cassert>

const IZoneLoaderFactory* IZoneLoaderFactory::GetZoneLoaderFactoryForGame(GameId game)
{
    static const IZoneLoaderFactory* zoneCreators[]{
        new IW3::ZoneLoaderFactory(),
        new IW3Xenon::ZoneLoaderFactory(),
        new IW4::ZoneLoaderFactory(),
        new IW4Xenon::ZoneLoaderFactory(),
        new IW5::ZoneLoaderFactory(),
        new QOS::ZoneLoaderFactory(),
        new T4::ZoneLoaderFactory(),
        new T4Xenon::ZoneLoaderFactory(),
        new T5::ZoneLoaderFactory(),
        new T6::ZoneLoaderFactory(),
        new NX1::ZoneLoaderFactory(),
        new T5Xenon::ZoneLoaderFactory(),
        new QOSXenon::ZoneLoaderFactory(),
    };
    static_assert(std::extent_v<decltype(zoneCreators)> == static_cast<unsigned>(GameId::COUNT));

    assert(static_cast<unsigned>(game) < static_cast<unsigned>(GameId::COUNT));
    const auto* result = zoneCreators[static_cast<unsigned>(game)];
    assert(result);

    return result;
}
