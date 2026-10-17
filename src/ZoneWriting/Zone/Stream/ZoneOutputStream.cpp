#include "ZoneOutputStream.h"

#include "InMemoryZoneData.h"
#include "Utils/Alignment.h"
#include "Utils/Endianness.h"
#include "Writing/WritingException.h"
#include "Zone/XBlock.h"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <stack>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace
{
    class ReusableEntry
    {
    public:
        ReusableEntry(void* startPtr, const size_t entrySize, const size_t entryCount, const size_t blockSize, const uintptr_t startZonePtr)
            : m_start_ptr(startPtr),
              m_end_ptr(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(startPtr) + entrySize * entryCount)),
              m_start_zone_ptr(startZonePtr),
              m_entry_size(entrySize),
              m_entry_count(entryCount),
              m_block_size(blockSize)
        {
        }

        void* m_start_ptr;
        void* m_end_ptr;
        uintptr_t m_start_zone_ptr;
        size_t m_entry_size;
        size_t m_entry_count;
        size_t m_block_size;
    };

    class InMemoryZoneOutputStream final : public ZoneOutputStream
    {
    public:
        InMemoryZoneOutputStream(const unsigned pointerBitCount,
                                 const unsigned blockBitCount,
                                 std::vector<XBlock*>& blocks,
                                 const block_t insertBlock,
                                 InMemoryZoneData& zoneData,
                                 InMemoryZoneData& delayData,
                                 const GameEndianness endianness)
            : m_zone_data(zoneData),
              m_delay_data(delayData),
              m_blocks(blocks),
              m_block_bit_count(blockBitCount),
              m_pointer_byte_count(pointerBitCount / 8u),
              m_endianness(endianness),

              // -1
              m_zone_ptr_following(std::numeric_limits<std::uintptr_t>::max() >> ((sizeof(std::uintptr_t) * 8u) - pointerBitCount)),

              // -2
              m_zone_ptr_insert((std::numeric_limits<std::uintptr_t>::max() >> ((sizeof(std::uintptr_t) * 8u) - pointerBitCount)) - 1u)
        {
            assert(pointerBitCount % 8u == 0u);
            assert(insertBlock < static_cast<block_t>(blocks.size()));

            m_insert_block = blocks[insertBlock];
        }

        [[nodiscard]] unsigned GetPointerByteCount() const override
        {
            return m_pointer_byte_count;
        }

        void PushBlock(const block_t block) override
        {
            assert(block < static_cast<block_t>(m_blocks.size()));

            auto* newBlock = m_blocks[block];

            assert(newBlock->m_index == block);

            m_block_stack.push(newBlock);

            if (newBlock->m_type == XBlockType::BLOCK_TYPE_TEMP)
            {
                if (m_temp_sizes.empty())
                    m_temp_sizes.push(0);
                else
                    m_temp_sizes.push(m_temp_sizes.top());
            }
        }

        block_t PopBlock() override
        {
            assert(!m_block_stack.empty());

            if (m_block_stack.empty())
                return -1;

            auto* poppedBlock = m_block_stack.top();
            m_block_stack.pop();

            // If temp block is popped, see if its size is bigger than the current maximum temp size
            if (poppedBlock->m_type == XBlockType::BLOCK_TYPE_TEMP)
            {
                const auto tempSize = m_temp_sizes.top();
                m_temp_sizes.pop();

                poppedBlock->m_buffer_size = std::max(tempSize, poppedBlock->m_buffer_size);
            }

            return poppedBlock->m_index;
        }

        void Align(const unsigned align) override
        {
            assert(!m_block_stack.empty());

            if (align > 1)
            {
                auto* block = m_block_stack.top();

                if (block->m_type == XBlockType::BLOCK_TYPE_TEMP)
                    m_temp_sizes.top() = utils::Align(m_temp_sizes.top(), static_cast<size_t>(align));
                else
                    block->m_buffer_size = utils::Align(block->m_buffer_size, static_cast<size_t>(align));
            }
        }

        ZoneOutputOffset WriteDataRaw(const void* src, const size_t size) override
        {
            auto* result = m_zone_data.GetBufferOfSize(size);
            memcpy(result, src, size);

            return ZoneOutputOffset(result);
        }

        ZoneOutputOffset WriteDataInBlock(const void* src, const size_t size) override
        {
            assert(!m_block_stack.empty());

            if (m_block_stack.empty())
                return ZoneOutputOffset();

            const auto* block = m_block_stack.top();

            void* result = nullptr;
            switch (block->m_type)
            {
            case XBlockType::BLOCK_TYPE_TEMP:
            case XBlockType::BLOCK_TYPE_NORMAL:
                result = m_zone_data.GetBufferOfSize(size);
                memcpy(result, src, size);
                break;

            case XBlockType::BLOCK_TYPE_RUNTIME:
                break;

            case XBlockType::BLOCK_TYPE_DELAY:
                result = m_delay_data.GetBufferOfSize(size);
                memcpy(result, src, size);
                break;
            }

            const auto zonePointer = GetCurrentZonePointer();
            IncBlockPos(size);

            return ZoneOutputOffset(result, zonePointer);
        }

        void IncBlockPos(const size_t size) override
        {
            assert(!m_block_stack.empty());

            if (m_block_stack.empty())
                return;

            auto* block = m_block_stack.top();
            if (block->m_type == XBlockType::BLOCK_TYPE_TEMP)
            {
                m_temp_sizes.top() += size;
            }
            else
            {
                block->m_buffer_size += size;
            }
        }

        void WriteNullTerminated(const void* src) override
        {
            const auto len = strlen(static_cast<const char*>(src));
            WriteDataInBlock(src, len + 1);
        }

        ZoneStreamFillWriteAccessor WriteWithFill(const size_t size) override
        {
            // If no block has been pushed, load raw
            if (!m_block_stack.empty())
            {
                const auto* block = m_block_stack.top();

                void* result = nullptr;
                switch (block->m_type)
                {
                case XBlockType::BLOCK_TYPE_TEMP:
                case XBlockType::BLOCK_TYPE_NORMAL:
                    result = m_zone_data.GetBufferOfSize(size);
                    break;

                case XBlockType::BLOCK_TYPE_RUNTIME:
                    assert(false);
                    break;

                case XBlockType::BLOCK_TYPE_DELAY:
                    result = m_delay_data.GetBufferOfSize(size);
                    break;
                }

                const auto zonePointer = GetCurrentZonePointer();
                IncBlockPos(size);

                return ZoneStreamFillWriteAccessor(result, size, zonePointer);
            }

            return ZoneStreamFillWriteAccessor(m_zone_data.GetBufferOfSize(size), size);
        }

        void SetIndexedXStrings(const std::span<const char* const> strings) override
        {
            m_indexed_xstrings.emplace();
            for (size_t i = 0; i < strings.size(); ++i)
                if (strings[i])
                    m_indexed_xstrings->emplace(strings[i], static_cast<uint32_t>(i + 1));
        }

        bool WriteXStringIndex(const char* value, const ZoneOutputOffset outputOffset) override
        {
            if (!m_indexed_xstrings)
                return false;
            uint32_t index = 0;
            if (value)
            {
                const auto found = m_indexed_xstrings->find(value);
                if (found == m_indexed_xstrings->end())
                    throw WritingException("String missing from indexed string table: " + std::string(value));
                index = found->second;
            }
            WriteZonePointer(outputOffset.Offset(), index);
            return true;
        }

        void UseOriginalPointerSlots(const block_t block) override
        {
            m_use_original_pointer_slots = true;
            m_original_pointer_slot_block = block;
        }

        void MarkFollowing(const ZoneOutputOffset outputOffset) override
        {
            assert(!m_block_stack.empty());

            auto* ptr = static_cast<char*>(outputOffset.Offset());
            assert(ptr != nullptr);

            const auto zonePtr =
                !m_use_original_pointer_slots && m_block_stack.top()->m_type == XBlockType::BLOCK_TYPE_TEMP ? m_zone_ptr_insert : m_zone_ptr_following;
            WriteZonePointer(ptr, zonePtr);
        }

        void WriteZonePointer(void* outputOffset, const uintptr_t zonePtr) const
        {
            auto* ptr = static_cast<char*>(outputOffset);
            assert(ptr != nullptr);

            if (m_endianness == GameEndianness::BE)
            {
                if (m_pointer_byte_count == sizeof(uint32_t))
                {
                    const auto swapped = endianness::ToBigEndian(static_cast<uint32_t>(zonePtr));
                    std::memcpy(ptr, &swapped, sizeof(swapped));
                    return;
                }

                if (m_pointer_byte_count == sizeof(uint64_t))
                {
                    const auto swapped = endianness::ToBigEndian(static_cast<uint64_t>(zonePtr));
                    std::memcpy(ptr, &swapped, sizeof(swapped));
                    return;
                }
            }

            for (auto i = 0u; i < m_pointer_byte_count; i++)
                ptr[i] = reinterpret_cast<const char*>(&zonePtr)[i];
        }

        bool ReusableShouldWrite(void* ptr, const ZoneOutputOffset outputOffset, const size_t entrySize, const std::type_index type) override
        {
            assert(!m_block_stack.empty());

            if (ptr == nullptr)
                return false;

            if (m_use_original_pointer_slots && m_block_stack.top()->m_index == m_original_pointer_slot_block)
                m_pending_alias_offsets.insert_or_assign(ptr, outputOffset);

            const auto foundEntriesForType = m_reusable_entries.find(type);
            if (foundEntriesForType == m_reusable_entries.end())
            {
                return true;
            }

            for (const auto& entry : foundEntriesForType->second)
            {
                if (ptr >= entry.m_start_ptr && ptr < entry.m_end_ptr)
                {
                    assert((reinterpret_cast<uintptr_t>(ptr) - reinterpret_cast<uintptr_t>(entry.m_start_ptr)) % entrySize == 0);

                    const auto entryIndex = (reinterpret_cast<uintptr_t>(ptr) - reinterpret_cast<uintptr_t>(entry.m_start_ptr)) / entry.m_entry_size;
                    const auto finalZonePointer = entry.m_start_zone_ptr + entryIndex * entry.m_block_size;
                    auto* writtenPtrOffset = outputOffset.Offset();

                    WriteZonePointer(writtenPtrOffset, finalZonePointer);

                    return false;
                }
            }

            return true;
        }

        void ReusableAddOffset(
            void* ptr, const size_t size, const size_t count, const size_t blockSize, std::type_index type, const ZoneOutputOffset aliasOffset) override
        {
            assert(!m_block_stack.empty());

            m_zone_data.m_reusable_write_order.push_back({ptr, type});

            const auto inTemp = m_block_stack.top()->m_type == XBlockType::BLOCK_TYPE_TEMP;
            uintptr_t zoneOffset;
            if (aliasOffset.Offset())
            {
                if (!aliasOffset.ZonePointer())
                    throw WritingException("Alias pointer has no serialized block offset");
                zoneOffset = *aliasOffset.ZonePointer();
            }
            else if (m_use_original_pointer_slots && m_block_stack.top()->m_index == m_original_pointer_slot_block)
            {
                const auto found = m_pending_alias_offsets.find(ptr);
                if (found == m_pending_alias_offsets.end() || !found->second.ZonePointer())
                    throw WritingException("Asset alias has no original serialized pointer slot");
                zoneOffset = *found->second.ZonePointer();
                m_pending_alias_offsets.erase(found);
            }
            else
                zoneOffset = inTemp ? InsertPointer() : GetCurrentZonePointer();
            const auto foundEntriesForType = m_reusable_entries.find(type);
            if (foundEntriesForType == m_reusable_entries.end())
            {
                std::vector<ReusableEntry> entries;
                entries.emplace_back(ptr, size, count, blockSize, zoneOffset);
                m_reusable_entries.emplace(std::make_pair(type, std::move(entries)));
            }
            else
            {
                foundEntriesForType->second.emplace_back(ptr, size, count, blockSize, zoneOffset);
            }
        }

    private:
        uintptr_t GetCurrentZonePointer()
        {
            assert(!m_block_stack.empty());

            uintptr_t ptr = 0;
            ptr |= static_cast<uintptr_t>(m_block_stack.top()->m_index) << (m_pointer_byte_count * 8 - m_block_bit_count);
            ptr |= (m_block_stack.top()->m_type == XBlockType::BLOCK_TYPE_TEMP ? m_temp_sizes.top() : m_block_stack.top()->m_buffer_size)
                   & (UINTPTR_MAX >> (m_block_bit_count + (sizeof(uintptr_t) - m_pointer_byte_count) * 8));
            ptr++;

            return ptr;
        }

        uintptr_t InsertPointer()
        {
            PushBlock(m_insert_block->m_index);

            Align(m_pointer_byte_count);
            const auto result = GetCurrentZonePointer();
            IncBlockPos(m_pointer_byte_count);

            PopBlock();

            return result;
        }

        InMemoryZoneData& m_zone_data;
        InMemoryZoneData& m_delay_data;
        std::vector<XBlock*>& m_blocks;

        std::stack<XBlock*> m_block_stack;
        std::stack<size_t> m_temp_sizes;

        unsigned m_block_bit_count;
        unsigned m_pointer_byte_count;
        GameEndianness m_endianness;
        XBlock* m_insert_block;

        uintptr_t m_zone_ptr_following;
        uintptr_t m_zone_ptr_insert;

        std::unordered_map<std::type_index, std::vector<ReusableEntry>> m_reusable_entries;
        std::optional<std::unordered_map<std::string_view, uint32_t>> m_indexed_xstrings;
        bool m_use_original_pointer_slots = false;
        block_t m_original_pointer_slot_block = 0;
        std::unordered_map<void*, ZoneOutputOffset> m_pending_alias_offsets;
    };
} // namespace

ZoneOutputOffset::ZoneOutputOffset()
    : m_offset(nullptr)
{
}

ZoneOutputOffset::ZoneOutputOffset(void* offset, const std::optional<uintptr_t> zonePointer)
    : m_offset(offset),
      m_zone_pointer(zonePointer)
{
}

ZoneOutputOffset ZoneOutputOffset::AtOffset(const size_t innerOffset) const
{
    return ZoneOutputOffset(static_cast<char*>(m_offset) + innerOffset, m_zone_pointer ? std::optional(*m_zone_pointer + innerOffset) : std::nullopt);
}

void ZoneOutputOffset::Inc(const size_t size)
{
    m_offset = static_cast<void*>(static_cast<char*>(m_offset) + size);
    if (m_zone_pointer)
        *m_zone_pointer += size;
}

void* ZoneOutputOffset::Offset() const
{
    return m_offset;
}

std::optional<uintptr_t> ZoneOutputOffset::ZonePointer() const
{
    return m_zone_pointer;
}

std::unique_ptr<ZoneOutputStream> ZoneOutputStream::Create(const unsigned pointerBitCount,
                                                           const unsigned blockBitCount,
                                                           std::vector<XBlock*>& blocks,
                                                           const block_t insertBlock,
                                                           InMemoryZoneData& zoneData,
                                                           InMemoryZoneData& delayData,
                                                           const GameEndianness endianness)
{
    return std::make_unique<InMemoryZoneOutputStream>(pointerBitCount, blockBitCount, blocks, insertBlock, zoneData, delayData, endianness);
}

ZoneStreamFillWriteAccessor::ZoneStreamFillWriteAccessor(void* blockBuffer, const size_t bufferSize, const std::optional<uintptr_t> zonePointer)
    : m_block_buffer(blockBuffer),
      m_buffer_size(bufferSize),
      m_zone_pointer(zonePointer)
{
}

ZoneStreamFillWriteAccessor ZoneStreamFillWriteAccessor::AtOffset(const size_t offset) const
{
    assert(m_block_buffer);
    return ZoneStreamFillWriteAccessor(
        static_cast<char*>(m_block_buffer) + offset, m_buffer_size - offset, m_zone_pointer ? std::optional(*m_zone_pointer + offset) : std::nullopt);
}

ZoneOutputOffset ZoneStreamFillWriteAccessor::Offset() const
{
    assert(m_block_buffer);
    return ZoneOutputOffset(m_block_buffer, m_zone_pointer);
}
