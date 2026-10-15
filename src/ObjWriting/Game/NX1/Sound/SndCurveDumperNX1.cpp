#include "SndCurveDumperNX1.h"

#include "Sound/SndCurveDumper.h"
#include "Sound/SoundCurveCommon.h"

using namespace NX1;

namespace sound_curve
{
    void DumperNX1::DumpAsset(AssetDumpingContext& context, const XAssetInfo<AssetSndVfCurve::Type>& asset)
    {
        const auto* sndCurve = asset.Asset();

        const auto assetFile = context.OpenAssetFile(GetFileNameForAssetName(sndCurve->filename));

        if (!assetFile)
            return;

        SndCurveDumper dumper(*assetFile);

        const auto knotCount = std::min(static_cast<size_t>(sndCurve->sndCurve.knotCount), std::extent_v<decltype(SndCurve::knots)>);
        dumper.Init(knotCount);

        for (auto i = 0u; i < knotCount; i++)
            dumper.WriteKnot(sndCurve->sndCurve.knots[i][0], sndCurve->sndCurve.knots[i][1]);
    }
} // namespace sound_curve
