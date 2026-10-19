#pragma once

#include "Game/IW2Xenon/IW2Xenon.h"
#include "Writing/ContentWriterBase.h"
#include "Writing/IContentWritingEntryPoint.h"

#include <cstddef>

namespace IW2Xenon
{
    class ContentWriter final : public ContentWriterBase, public IContentWritingEntryPoint
    {
    public:
        explicit ContentWriter(const Zone& zone);

        void WriteContent(ZoneOutputStream& stream) override;

    private:
        void CreateXAssetList(XAssetList& xAssetList, MemoryManager& memory);
        void WriteXStringList(bool atStreamStart);
        void WriteXAsset(bool atStreamStart);
        void WriteXAssetArray(bool atStreamStart, size_t count);

        XAssetList* varXAssetList;
        XAsset* varXAsset;
        XStringList* varXStringList;

        ZoneOutputOffset varXAssetListWritten;
        ZoneOutputOffset varXAssetWritten;
        ZoneOutputOffset varXStringListWritten;
    };
} // namespace IW2Xenon
