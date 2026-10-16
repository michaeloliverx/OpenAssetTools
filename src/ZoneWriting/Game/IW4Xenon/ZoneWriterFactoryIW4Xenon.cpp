#include "ZoneWriterFactoryIW4Xenon.h"

#include "ContentWriterIW4Xenon.h"
#include "Game/IW4Xenon/IW4Xenon.h"
#include "Game/IW4Xenon/ZoneConstantsIW4Xenon.h"
#include "Pool/XAssetInfo.h"
#include "Utils/ClassUtils.h"
#include "Utils/Endianness.h"
#include "Utils/Logging/Log.h"
#include "Writing/IWritingStep.h"
#include "Writing/Processor/OutputProcessorDeflate.h"
#include "Writing/Steps/StepAddOutputProcessor.h"
#include "Writing/Steps/StepRemoveOutputProcessor.h"
#include "Writing/Steps/StepWriteByte.h"
#include "Writing/Steps/StepWriteTimestamp.h"
#include "Writing/Steps/StepWriteXBlockSizes.h"
#include "Writing/Steps/StepWriteZoneContentToFile.h"
#include "Writing/Steps/StepWriteZoneContentToMemory.h"
#include "Writing/Steps/StepWriteZoneHeader.h"
#include "Writing/Steps/StepWriteZoneImageHeaders.h"
#include "Writing/Steps/StepWriteZoneSizes.h"
#include "Writing/WritingException.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <limits>
#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace IW4Xenon;

namespace
{
    void SetupBlocks(ZoneWriter& writer)
    {
#define XBLOCK_DEF(name, type) std::make_unique<XBlock>(STR(name), name, type)

        writer.AddXBlock(XBLOCK_DEF(XFILE_BLOCK_TEMP, XBlockType::BLOCK_TYPE_TEMP));
        writer.AddXBlock(XBLOCK_DEF(XFILE_BLOCK_PHYSICAL, XBlockType::BLOCK_TYPE_NORMAL));
        writer.AddXBlock(XBLOCK_DEF(XFILE_BLOCK_RUNTIME, XBlockType::BLOCK_TYPE_RUNTIME));
        writer.AddXBlock(XBLOCK_DEF(XFILE_BLOCK_VIRTUAL, XBlockType::BLOCK_TYPE_NORMAL));
        writer.AddXBlock(XBLOCK_DEF(XFILE_BLOCK_LARGE, XBlockType::BLOCK_TYPE_NORMAL));
        writer.AddXBlock(XBLOCK_DEF(XFILE_BLOCK_CALLBACK, XBlockType::BLOCK_TYPE_NORMAL));

#undef XBLOCK_DEF
    }

    ZoneHeader CreateHeader()
    {
        ZoneHeader header{};
        header.m_version = endianness::ToBigEndian(ZoneConstants::ZONE_VERSION);

        std::memcpy(header.m_magic, ZoneConstants::MAGIC_UNSIGNED, sizeof(header.m_magic));
        return header;
    }

    std::vector<XAssetStreamFile> CollectImageStreamFilesToWrite(const Zone& zone, const InMemoryZoneData& data)
    {
        std::vector<XAssetStreamFile> files;
        std::unordered_map<const Zone*, std::unordered_map<std::string, size_t>> nextRecords;
        for (const auto& entry : data.m_reusable_write_order)
        {
            if (entry.m_type != std::type_index(typeid(GfxImage)))
                continue;
            const auto& image = *static_cast<const GfxImage*>(entry.m_ptr);
            if (!image.cached)
                continue;

            XAssetStreamFilesForImage records{};
            const auto name = XAssetInfoGeneric::NormalizeAssetName(image.name);
            const auto* info = zone.m_pools.GetAssetOrAssetReference(AssetImage::EnumEntry, name);
            const auto* source = zone.m_xfile_image_stream_files_by_name.contains(name) ? &zone : info && info->m_zone ? info->m_zone : &zone;
            const auto found = source->m_xfile_image_stream_files_by_name.find(name);
            if (found != source->m_xfile_image_stream_files_by_name.end() && !found->second.empty())
            {
                auto& next = nextRecords[source][name];
                records = found->second[std::min(next++, found->second.size() - 1u)];
            }
            files.insert(files.end(), records.begin(), records.end());
        }
        return files;
    }

    class StepWriteImageStreams final : public IWritingStep
    {
    public:
        StepWriteImageStreams(const Zone& zone, const StepWriteZoneContentToMemory& content, std::shared_ptr<XFileSizePatch> patch)
            : m_zone(zone),
              m_content(content),
              m_patch(std::move(patch))
        {
        }

        void PerformStep(ZoneWriter* zoneWriter, IWritingStream* stream) override
        {
            stream->Flush();
            const auto zoneEnd = stream->Pos();
            const auto& data = m_zone.m_xfile_image_stream_data;
            if (zoneEnd < 0 || static_cast<uint64_t>(zoneEnd) + data.size() > std::numeric_limits<uint32_t>::max())
                throw WritingException("IW4 Xenon fastfile exceeds the image stream offset limit");

            auto files = CollectImageStreamFilesToWrite(m_zone, *m_content.GetData());
            const auto writeStream = [&](XAssetStreamFile& file)
            {
                if (file.fileIndex == 0u && file.offsetEnd != 0u)
                {
                    if (file.offsetEnd <= file.offset || file.offsetEnd > data.size())
                        throw WritingException("IW4 Xenon image stream data was not copied from the source fastfile");
                    const auto length = file.offsetEnd - file.offset;
                    const auto start = stream->Pos();
                    stream->Write(data.data() + file.offset, length);
                    file.offset = static_cast<uint32_t>(start);
                    file.offsetEnd = file.offset + length;
                }
            };

            for (size_t index = 0; index < files.size(); index += XAssetStreamFilesForImage{}.size())
                writeStream(files[index]);
            const auto loadEnd = stream->Pos();
            for (size_t index = 0; index < files.size(); ++index)
                if (index % XAssetStreamFilesForImage{}.size() != 0u)
                    writeStream(files[index]);
            const auto fileEnd = stream->Pos();

            for (auto& file : files)
            {
                file.fileIndex = endianness::ToBigEndian(file.fileIndex);
                file.offset = endianness::ToBigEndian(file.offset);
                file.offsetEnd = endianness::ToBigEndian(file.offsetEnd);
            }

            if (!stream->SetPos(m_patch->m_offset - static_cast<int64_t>(files.size() * sizeof(XAssetStreamFile))))
                throw WritingException("Could not seek to IW4 Xenon image stream records");
            if (!files.empty())
                stream->Write(files.data(), files.size() * sizeof(XAssetStreamFile));

            // The first size ends the zone payload; the second includes images needed at load time.
            const auto xfileSize = endianness::ToBigEndian(static_cast<uint32_t>(zoneEnd));
            const auto xfileLoadSize = endianness::ToBigEndian(static_cast<uint32_t>(loadEnd));
            stream->Write(&xfileSize, sizeof(xfileSize));
            stream->Write(&xfileLoadSize, sizeof(xfileLoadSize));
            stream->Flush();
            if (!stream->SetPos(fileEnd))
                throw WritingException("Could not restore IW4 Xenon fastfile output position");
        }

    private:
        const Zone& m_zone;
        const StepWriteZoneContentToMemory& m_content;
        std::shared_ptr<XFileSizePatch> m_patch;
    };

    constexpr size_t CALLBACK_SOUND_FILE_SIZE = 0x10u;
    constexpr size_t CALLBACK_SOUND_FILE_ALIGNMENT = 4u;
    constexpr size_t CALLBACK_D3D_VERTEX_DECL_BASE_SIZE = 56u;
    constexpr size_t CALLBACK_D3D_VERTEX_DECL_ELEMENT_SIZE = 12u;
    constexpr size_t CALLBACK_D3D_VERTEX_DECL_ALIGNMENT = 16u;
    constexpr size_t CALLBACK_MATERIAL_STREAM_IMAGE_INDEX_SIZE = 4u;
    constexpr size_t CALLBACK_MATERIAL_STREAM_IMAGE_INDEX_ALIGNMENT = 4u;
    constexpr size_t MATERIAL_STREAM_SOURCE_TABLE_COUNT = 15u;
    constexpr size_t MATERIAL_STREAM_SOURCE_COUNT = 9u;

    constexpr std::array<std::array<bool, MATERIAL_STREAM_SOURCE_COUNT>, MATERIAL_STREAM_SOURCE_TABLE_COUNT> MATERIAL_STREAM_SOURCE_PRESENT{
        {
         {{true, true, true, true, false, false, false, false, false}},
         {{true, true, true, true, true, false, false, false, false}},
         {{true, true, true, true, true, false, false, false, false}},
         {{true, true, true, true, true, true, false, false, false}},
         {{true, true, true, true, true, true, false, true, false}},
         {{true, true, true, true, true, true, false, false, false}},
         {{true, true, true, true, true, true, false, true, false}},
         {{true, true, true, true, true, true, false, true, true}},
         {{true, true, true, true, true, true, true, false, false}},
         {{true, true, true, true, true, true, true, true, false}},
         {{true, true, true, true, true, true, true, true, true}},
         {{true, true, true, true, true, true, true, false, false}},
         {{true, true, true, true, true, true, true, true, false}},
         {{true, true, true, true, true, true, true, true, true}},
         {{true, false, true, false, false, false, false, false, false}},
         }
    };

    struct CallbackReservationState
    {
        size_t size = 0u;
        std::unordered_set<const Material*> materials;
        std::unordered_set<const MaterialTechniqueSet*> techniqueSets;
        std::unordered_set<const MaterialVertexDeclaration*> vertexDecls;
    };

    size_t AlignCallbackMemory(const size_t value, const size_t alignment)
    {
        assert(alignment > 0u);
        return (value + alignment - 1u) & ~(alignment - 1u);
    }

    void ReserveCallbackMemory(CallbackReservationState& state, const size_t size, const size_t alignment)
    {
        if (size == 0u)
            return;

        state.size = AlignCallbackMemory(state.size, alignment);
        state.size += size;
    }

    void ReserveSoundAliasCallbackMemory(CallbackReservationState& state, const snd_alias_list_t* sound)
    {
        if (sound == nullptr || sound->head == nullptr || sound->count <= 0)
            return;

        for (auto aliasIndex = 0; aliasIndex < sound->count; aliasIndex++)
        {
            if (sound->head[aliasIndex].soundFile != nullptr)
                ReserveCallbackMemory(state, CALLBACK_SOUND_FILE_SIZE, CALLBACK_SOUND_FILE_ALIGNMENT);
        }
    }

    size_t CountMaterialVertexDeclBuilds(const MaterialVertexDeclaration* vertexDecl)
    {
        if (vertexDecl == nullptr)
            return 0u;

        auto buildCount = 0u;
        for (const auto& sourceTable : MATERIAL_STREAM_SOURCE_PRESENT)
        {
            auto canBuild = true;
            for (auto routingIndex = 0u; routingIndex < vertexDecl->streamCount; routingIndex++)
            {
                const auto source = vertexDecl->routing.data[routingIndex].source;
                if (source >= sourceTable.size() || !sourceTable[source])
                {
                    canBuild = false;
                    break;
                }
            }

            if (canBuild)
                buildCount++;
        }

        return buildCount;
    }

    void ReserveMaterialVertexDeclCallbackMemory(CallbackReservationState& state, const MaterialVertexDeclaration* vertexDecl)
    {
        if (vertexDecl == nullptr || !state.vertexDecls.emplace(vertexDecl).second)
            return;

        const auto allocSize = CALLBACK_D3D_VERTEX_DECL_BASE_SIZE + CALLBACK_D3D_VERTEX_DECL_ELEMENT_SIZE * vertexDecl->streamCount;
        const auto buildCount = CountMaterialVertexDeclBuilds(vertexDecl);

        for (auto buildIndex = 0u; buildIndex < buildCount; buildIndex++)
            ReserveCallbackMemory(state, allocSize, CALLBACK_D3D_VERTEX_DECL_ALIGNMENT);
    }

    void ReserveMaterialTechniqueSetCallbackMemory(CallbackReservationState& state, const MaterialTechniqueSet* techniqueSet)
    {
        if (techniqueSet == nullptr || !state.techniqueSets.emplace(techniqueSet).second)
            return;

        for (const auto* technique : techniqueSet->techniques)
        {
            if (technique == nullptr)
                continue;

            for (auto passIndex = 0u; passIndex < technique->passCount; passIndex++)
                ReserveMaterialVertexDeclCallbackMemory(state, technique->passArray[passIndex].vertexDecl);
        }
    }

    void ReserveMaterialCallbackMemory(CallbackReservationState& state, const Material* material)
    {
        if (material == nullptr || !state.materials.emplace(material).second)
            return;

        ReserveMaterialTechniqueSetCallbackMemory(state, material->techniqueSet);

        ReserveCallbackMemory(
            state, static_cast<size_t>(material->textureCount) * CALLBACK_MATERIAL_STREAM_IMAGE_INDEX_SIZE, CALLBACK_MATERIAL_STREAM_IMAGE_INDEX_ALIGNMENT);
    }

    void ReserveAssetCallbackMemory(CallbackReservationState& state, const XAssetInfoGeneric* asset)
    {
        if (asset == nullptr || asset->IsReference())
            return;

        switch (asset->m_type)
        {
        case ASSET_TYPE_SOUND:
            ReserveSoundAliasCallbackMemory(state, static_cast<const snd_alias_list_t*>(asset->m_ptr));
            break;

        case ASSET_TYPE_MATERIAL:
            ReserveMaterialCallbackMemory(state, static_cast<const Material*>(asset->m_ptr));
            break;

        case ASSET_TYPE_TECHNIQUE_SET:
            ReserveMaterialTechniqueSetCallbackMemory(state, static_cast<const MaterialTechniqueSet*>(asset->m_ptr));
            break;

        default:
            break;
        }
    }

    size_t CalculateCallbackMemoryReservation(const Zone& zone)
    {
        CallbackReservationState state;

        const auto end = zone.m_pools.end();
        for (auto i = zone.m_pools.begin(); i != end; ++i)
            ReserveAssetCallbackMemory(state, *i);

        return state.size;
    }

    class StepReserveCallbackMemory final : public IWritingStep
    {
    public:
        explicit StepReserveCallbackMemory(const Zone& zone)
            : m_zone(zone)
        {
        }

        void PerformStep(ZoneWriter* zoneWriter, IWritingStream* stream) override
        {
            assert(zoneWriter != nullptr);
            (void)stream;

            if (zoneWriter == nullptr || zoneWriter->m_blocks.size() <= XFILE_BLOCK_CALLBACK)
                return;

            auto& callbackBlock = zoneWriter->m_blocks[XFILE_BLOCK_CALLBACK];
            callbackBlock->m_buffer_size = std::max(callbackBlock->m_buffer_size, CalculateCallbackMemoryReservation(m_zone));
        }

    private:
        const Zone& m_zone;
    };

#ifdef ARCH_x86
    static_assert(sizeof(SoundFile) == CALLBACK_SOUND_FILE_SIZE);
    static_assert(sizeof(MaterialVertexDeclaration) == 0x40u);
#endif
} // namespace

std::unique_ptr<ZoneWriter> ZoneWriterFactory::CreateWriter(const Zone& zone) const
{
    if constexpr (sizeof(void*) != 4u)
    {
        con::error("IW4 Xenon zone writing requires the x86 tools.");
        return nullptr;
    }

    auto writer = std::make_unique<ZoneWriter>();

    SetupBlocks(*writer);
    auto xfileSizePatch = std::make_shared<XFileSizePatch>();

    auto contentInMemory = std::make_unique<StepWriteZoneContentToMemory>(
        std::make_unique<ContentWriter>(zone), zone, 32u, ZoneConstants::OFFSET_BLOCK_BIT_COUNT, ZoneConstants::INSERT_BLOCK, GameEndianness::BE);
    auto* contentInMemoryPtr = contentInMemory.get();
    writer->AddWritingStep(std::move(contentInMemory));

    writer->AddWritingStep(std::make_unique<StepWriteZoneHeader>(CreateHeader()));
    writer->AddWritingStep(std::make_unique<StepWriteByte>(1));
    writer->AddWritingStep(std::make_unique<StepWriteTimestamp>(GameEndianness::BE));

    writer->AddWritingStep(std::make_unique<StepWriteZoneImageHeaders>(
        GameEndianness::BE,
        [&zone, contentInMemoryPtr]()
        {
            return CollectImageStreamFilesToWrite(zone, *contentInMemoryPtr->GetData());
        },
        xfileSizePatch));

    auto deflateProcessor = std::make_unique<OutputProcessorDeflate>();
    auto* deflateProcessorPtr = deflateProcessor.get();
    writer->AddWritingStep(std::make_unique<StepAddOutputProcessor>(std::move(deflateProcessor)));
    writer->AddWritingStep(std::make_unique<StepWriteZoneSizes>(contentInMemoryPtr, GameEndianness::BE));
    writer->AddWritingStep(std::make_unique<StepReserveCallbackMemory>(zone));
    writer->AddWritingStep(std::make_unique<StepWriteXBlockSizes>(zone, GameEndianness::BE));
    writer->AddWritingStep(std::make_unique<StepWriteZoneContentToFile>(contentInMemoryPtr));
    writer->AddWritingStep(std::make_unique<StepRemoveOutputProcessor>(deflateProcessorPtr));
    writer->AddWritingStep(std::make_unique<StepWriteImageStreams>(zone, *contentInMemoryPtr, std::move(xfileSizePatch)));

    return writer;
}
