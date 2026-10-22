#include "ImageToCommonConverter.h"

#include "Game/IW3/Image/ImageToCommonConverterIW3.h"
#include "Game/IW3Xenon/Image/ImageToCommonConverterIW3Xenon.h"
#include "Game/IW3Xenon253/Image/ImageToCommonConverterIW3Xenon253.h"
#include "Game/IW3Xenon270/Image/ImageToCommonConverterIW3Xenon270.h"
#include "Game/IW3Xenon290/Image/ImageToCommonConverterIW3Xenon290.h"
#include "Game/IW4/Image/ImageToCommonConverterIW4.h"
#include "Game/IW4Xenon/Image/ImageToCommonConverterIW4Xenon.h"
#include "Game/IW5/Image/ImageToCommonConverterIW5.h"
#include "Game/NX1/Image/ImageToCommonConverterNX1.h"
#include "Game/QOS/Image/ImageToCommonConverterQOS.h"
#include "Game/QOSXenon/Image/ImageToCommonConverterQOSXenon.h"
#include "Game/T4/Image/ImageToCommonConverterT4.h"
#include "Game/T4Xenon/Image/ImageToCommonConverterT4Xenon.h"
#include "Game/T5/Image/ImageToCommonConverterT5.h"
#include "Game/T5Xenon/Image/ImageToCommonConverterT5Xenon.h"
#include "Game/T6/Image/ImageToCommonConverterT6.h"

#include <cassert>

namespace image
{
    ToCommonConverter* ToCommonConverter::GetForGame(const GameId gameId)
    {
        static ToCommonConverter* toCommonConverters[]{
            new ToCommonConverterIW3(),
            new ToCommonConverterIW3Xenon(),
            new ToCommonConverterIW4(),
            new ToCommonConverterIW4Xenon(),
            new ToCommonConverterIW5(),
            new ToCommonConverterQOS(),
            new ToCommonConverterT4(),
            new ToCommonConverterT4Xenon(),
            new ToCommonConverterT5(),
            new ToCommonConverterT6(),
            new ToCommonConverterNX1(),
            new ToCommonConverterT5Xenon(),
            new ToCommonConverterQOSXenon(),
            new ToCommonConverterIW3Xenon253(),
            new ToCommonConverterIW3Xenon270(),
            new ToCommonConverterIW3Xenon290(),
        };
        static_assert(std::extent_v<decltype(toCommonConverters)> == static_cast<unsigned>(GameId::COUNT));
        assert(static_cast<unsigned>(gameId) < static_cast<unsigned>(GameId::COUNT));

        return toCommonConverters[std::to_underlying(gameId)];
    }
} // namespace image
