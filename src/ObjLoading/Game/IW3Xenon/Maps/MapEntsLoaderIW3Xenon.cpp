#include "MapEntsLoaderIW3Xenon.h"

#include "Game/IW3Xenon/IW3Xenon.h"
#include "Maps/MapEntsCommon.h"
#include "Utils/Logging/Log.h"

#include <limits>

using namespace IW3Xenon;

namespace
{
    class MapEntsLoader final : public AssetCreator<AssetMapEnts>
    {
    public:
        MapEntsLoader(MemoryManager& memory, ISearchPath& searchPath)
            : m_memory(memory),
              m_search_path(searchPath)
        {
        }

        AssetCreationResult CreateAsset(const std::string& assetName, AssetCreationContext& context) override
        {
            const auto file = m_search_path.Open(map_ents::GetEntsFileNameForAssetName(assetName));
            if (!file.IsOpen())
                return AssetCreationResult::NoAction();

            if (file.m_length < 0 || file.m_length >= std::numeric_limits<int>::max())
            {
                con::error("Map ents \"{}\" has an invalid size", assetName);
                return AssetCreationResult::Failure();
            }

            auto* mapEnts = m_memory.Alloc<MapEnts>();
            mapEnts->name = m_memory.Dup(assetName.c_str());
            mapEnts->numEntityChars = static_cast<int>(file.m_length + 1);

            auto* fileBuffer = m_memory.Alloc<char>(static_cast<size_t>(file.m_length + 1));
            file.m_stream->read(fileBuffer, file.m_length);
            if (file.m_stream->gcount() != file.m_length)
                return AssetCreationResult::Failure();
            fileBuffer[static_cast<size_t>(file.m_length)] = '\0';

            mapEnts->entityString = fileBuffer;

            return AssetCreationResult::Success(context.AddAsset<AssetMapEnts>(assetName, mapEnts));
        }

    private:
        MemoryManager& m_memory;
        ISearchPath& m_search_path;
    };
} // namespace

namespace map_ents
{
    std::unique_ptr<AssetCreator<AssetMapEnts>> CreateLoaderIW3Xenon(MemoryManager& memory, ISearchPath& searchPath)
    {
        return std::make_unique<MapEntsLoader>(memory, searchPath);
    }
} // namespace map_ents
