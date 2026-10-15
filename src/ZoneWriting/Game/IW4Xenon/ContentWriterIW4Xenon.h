#pragma once

#include "Game/IW4Xenon/IW4Xenon.h"
#include "Writing/ContentWriterBase.h"
#include "Writing/IContentWritingEntryPoint.h"

namespace IW4Xenon
{
    class ContentWriter final : public ContentWriterBase, public IContentWritingEntryPoint
    {
    public:
        explicit ContentWriter(const Zone& zone);

        void WriteContent(ZoneOutputStream& stream) override;

    private:
        void CreateXAssetList(XAssetList& xAssetList, MemoryManager& memory) const;
        void WriteScriptStringList(bool atStreamStart);
        void WriteXAsset(bool atStreamStart);
        void WriteXAssetArray(bool atStreamStart, size_t count);

        XAssetList* varXAssetList;
        ZoneOutputOffset varXAssetListWritten;
        XAsset* varXAsset;
        ZoneOutputOffset varXAssetWritten;
        ScriptStringList* varScriptStringList;
        ZoneOutputOffset varScriptStringListWritten;
    };
} // namespace IW4Xenon
