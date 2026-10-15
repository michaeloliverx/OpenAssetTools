#pragma once

#include <memory>
#include <typeindex>
#include <vector>

class InMemoryZoneData
{
    static constexpr size_t BUFFER_SIZE = 0x400000;

public:
    class MemoryBuffer
    {
    public:
        std::unique_ptr<char[]> m_data;
        size_t m_size;

        explicit MemoryBuffer(size_t size);
    };

    struct ReusableWriteOrderEntry
    {
        void* m_ptr;
        std::type_index m_type;
    };

    std::vector<ReusableWriteOrderEntry> m_reusable_write_order;
    int64_t m_total_size;
    std::vector<MemoryBuffer> m_buffers;

    InMemoryZoneData();
    void* GetBufferOfSize(size_t size);
};
