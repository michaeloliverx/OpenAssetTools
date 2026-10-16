#pragma once

#include "Game/T5Xenon/T5Xenon.h"
#include "Writing/ContentWriterBase.h"
#include "Writing/IContentWritingEntryPoint.h"

#include <cstddef>

namespace T5Xenon
{
    class ContentWriter final : public ContentWriterBase, public IContentWritingEntryPoint
    {
    public:
        explicit ContentWriter(const Zone& zone);

        void WriteContent(ZoneOutputStream& stream) override;

    private:
        void CreateXAssetList(XAssetList& xAssetList, MemoryManager& memory);
        void WriteScriptStringList(bool atStreamStart);
        void WriteXAsset(bool atStreamStart);
        void WriteXAssetArray(bool atStreamStart, size_t count);

        XAssetList* varXAssetList;
        XAsset* varXAsset;
        ScriptStringList* varScriptStringList;

        ZoneOutputOffset varXAssetListWritten;
        ZoneOutputOffset varXAssetWritten;
        ZoneOutputOffset varScriptStringListWritten;
    };
} // namespace T5Xenon
