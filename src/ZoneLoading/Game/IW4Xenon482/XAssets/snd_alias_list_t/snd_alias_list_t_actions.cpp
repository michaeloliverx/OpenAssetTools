#include "snd_alias_list_t_actions.h"

using namespace IW4Xenon482;

Actions_snd_alias_list_t::Actions_snd_alias_list_t(Zone& zone)
    : AssetLoadingActions(zone)
{
}

void Actions_snd_alias_list_t::SetSoundFile(snd_alias_t* alias, SoundFile* soundFile) const
{
    // SoundFile uses the temporary block and must outlive the next asset load.
    alias->soundFile = m_zone.Memory().Alloc<SoundFile>();
    *alias->soundFile = *soundFile;
}
