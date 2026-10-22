#include "LoaderStringTableIW4Xenon482.h"

#include "Csv/CsvStream.h"
#include "Game/IW4Xenon482/CommonIW4Xenon482.h"
#include "Game/IW4Xenon482/IW4Xenon482.h"
#include "StringTable/StringTableLoader.h"

using namespace IW4Xenon482;

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
    std::unique_ptr<AssetCreator<AssetStringTable>> CreateLoaderIW4Xenon482(MemoryManager& memory, ISearchPath& searchPath)
    {
        return std::make_unique<LoaderStringTable>(memory, searchPath);
    }
} // namespace string_table
