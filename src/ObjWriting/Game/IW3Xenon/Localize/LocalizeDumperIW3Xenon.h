#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon/IW3Xenon.h"
#include "Pool/AssetPool.h"

namespace localize
{
    class DumperIW3Xenon final : public AbstractSingleProgressAssetDumper<IW3Xenon::AssetLocalize>
    {
    public:
        explicit DumperIW3Xenon(const AssetPool<IW3Xenon::AssetLocalize::Type>& pool);

        [[nodiscard]] size_t GetProgressTotalCount() const override;
        void Dump(AssetDumpingContext& context) override;
    };
} // namespace localize
