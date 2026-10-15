#pragma once

#include "Game/NX1/NX1.h"
#include "Writing/ContentWriterBase.h"
#include "Writing/IContentWritingEntryPoint.h"

namespace NX1
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
} // namespace NX1
