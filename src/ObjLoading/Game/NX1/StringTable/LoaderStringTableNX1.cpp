#include "LoaderStringTableNX1.h"

#include "Csv/CsvStream.h"
#include "Game/NX1/CommonNX1.h"
#include "Game/NX1/NX1.h"
#include "StringTable/StringTableLoader.h"

using namespace NX1;

namespace
{
    class LoaderStringTable final : public AssetCreator<AssetStringTable>
    {
    public:
        LoaderStringTable(MemoryManager& memory, ISearchPath& searchPath)
            : m_memory(memory),
              m_search_path(searchPath)
        {
        }

        AssetCreationResult CreateAsset(const std::string& assetName, AssetCreationContext& context) override
        {
            const auto file = m_search_path.Open(assetName);
            if (!file.IsOpen())
                return AssetCreationResult::NoAction();

            string_table::StringTableLoaderV2<StringTable, Common::StringTable_HashString> loader;
            auto* stringTable = loader.LoadFromStream(assetName, m_memory, *file.m_stream);
            if (!stringTable)
                return AssetCreationResult::Failure();

            return AssetCreationResult::Success(context.AddAsset<AssetStringTable>(assetName, stringTable));
        }

    private:
        MemoryManager& m_memory;
        ISearchPath& m_search_path;
    };
} // namespace

namespace string_table
{
    std::unique_ptr<AssetCreator<AssetStringTable>> CreateLoaderNX1(MemoryManager& memory, ISearchPath& searchPath)
    {
        return std::make_unique<LoaderStringTable>(memory, searchPath);
    }
} // namespace string_table
