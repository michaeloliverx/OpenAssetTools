#pragma once

#include "Game/IW4Xenon482/IW4Xenon482.h"
#include "Loading/AssetLoadingActions.h"

namespace IW4Xenon482
{
    class Actions_snd_alias_list_t final : public AssetLoadingActions
    {
    public:
        explicit Actions_snd_alias_list_t(Zone& zone);

        void SetSoundFile(snd_alias_t* alias, SoundFile* soundFile) const;
    };
} // namespace IW4Xenon482
