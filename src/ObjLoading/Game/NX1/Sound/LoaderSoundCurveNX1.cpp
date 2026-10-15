#include "LoaderSoundCurveNX1.h"

#include "Game/NX1/NX1.h"
#include "ObjLoading.h"
#include "Parsing/Graph2D/Graph2DReader.h"
#include "Pool/GlobalAssetPool.h"
#include "Sound/SoundCurveCommon.h"
#include "Utils/Logging/Log.h"

#include <cstring>
#include <format>
#include <iostream>
#include <sstream>

using namespace NX1;

namespace
{
    class LoaderSoundCurve final : public AssetCreator<AssetSndVfCurve>
    {
    public:
        LoaderSoundCurve(MemoryManager& memory, ISearchPath& searchPath)
            : m_memory(memory),
              m_search_path(searchPath)
        {
        }

        AssetCreationResult CreateAsset(const std::string& assetName, AssetCreationContext& context) override
        {
            const auto fileName = sound_curve::GetFileNameForAssetName(assetName);
            const auto file = m_search_path.Open(fileName);
            if (!file.IsOpen())
                return AssetCreationResult::NoAction();

            const auto sndCurveData = graph2d::Read("sound curve", "SNDCURVE", *file.m_stream, fileName, assetName);

            if (!sndCurveData)
                return AssetCreationResult::Failure();

            if (sndCurveData->knots.size() > std::extent_v<decltype(SndCurve::knots)>)
            {
                con::error("Failed to load SndVfCurve \"{}\": Too many knots ({})", assetName, sndCurveData->knots.size());
                return AssetCreationResult::Failure();
            }

            auto* sndCurve = m_memory.Alloc<SndVfCurve>();
            sndCurve->filename = m_memory.Dup(assetName.c_str());
            sndCurve->sndCurve.knotCount = static_cast<uint16_t>(sndCurveData->knots.size());

            for (auto i = 0u; i < std::extent_v<decltype(SndCurve::knots)>; i++)
            {
                if (i < sndCurveData->knots.size())
                {
                    const auto& [x, y] = sndCurveData->knots[i];
                    sndCurve->sndCurve.knots[i][0] = static_cast<float>(x);
                    sndCurve->sndCurve.knots[i][1] = static_cast<float>(y);
                }
                else
                {
                    sndCurve->sndCurve.knots[i][0] = 0;
                    sndCurve->sndCurve.knots[i][1] = 0;
                }
            }

            return AssetCreationResult::Success(context.AddAsset<AssetSndVfCurve>(assetName, sndCurve));
        }

    private:
        MemoryManager& m_memory;
        ISearchPath& m_search_path;
    };
} // namespace

namespace sound_curve
{
    std::unique_ptr<AssetCreator<AssetSndVfCurve>> CreateLoaderNX1(MemoryManager& memory, ISearchPath& searchPath)
    {
        return std::make_unique<LoaderSoundCurve>(memory, searchPath);
    }
} // namespace sound_curve
