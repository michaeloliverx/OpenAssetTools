#pragma once

#include "Game/IW4Xenon/IW4Xenon.h"
#include "Loading/ContentLoaderBase.h"
#include "Loading/IContentLoadingEntryPoint.h"

namespace IW4Xenon
{
    class ContentLoader final : public ContentLoaderBase, public IContentLoadingEntryPoint
    {
    public:
        ContentLoader(Zone& zone, ZoneInputStream& stream);
        void Load() override;

    private:
        void LoadScriptStringList(bool atStreamStart);

        void LoadXAsset(bool atStreamStart) const;
        void LoadXAssetArray(bool atStreamStart, size_t count);

        XAssetList* varXAssetList;
        XAsset* varXAsset;
        ScriptStringList* varScriptStringList;
    };
} // namespace IW4Xenon
