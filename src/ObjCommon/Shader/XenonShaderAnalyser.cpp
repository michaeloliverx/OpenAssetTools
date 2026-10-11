#include "Utils/Endianness.h"
#include "XenonShader.h"

#include <cstring>
#include <stdexcept>

namespace
{
    class ConstantTableReader
    {
    public:
        explicit ConstantTableReader(const std::span<const uint8_t> data)
            : m_data(data)
        {
        }

        template<typename T> T Read(const size_t offset) const
        {
            if (offset > m_data.size() || sizeof(T) > m_data.size() - offset)
                throw std::runtime_error("truncated shader constant table");
            T result;
            std::memcpy(&result, m_data.data() + offset, sizeof(result));
            return endianness::ToBigEndian(result);
        }

        std::string String(const size_t offset) const
        {
            if (offset >= m_data.size())
                throw std::runtime_error("invalid shader string offset");
            const auto* first = reinterpret_cast<const char*>(m_data.data() + offset);
            const auto* last = static_cast<const char*>(std::memchr(first, 0, m_data.size() - offset));
            if (!last)
                throw std::runtime_error("unterminated shader string");
            return {first, last};
        }

    private:
        std::span<const uint8_t> m_data;
    };
} // namespace

std::unique_ptr<d3d9::ShaderInfo> shader::xenon::GetShaderInfo(const std::span<const uint8_t> cached)
{
    try
    {
        const ConstantTableReader object(cached);
        const auto signature = object.Read<uint32_t>(0);
        if ((signature & ~1u) != 0x102a1100u || object.Read<uint32_t>(4) != cached.size())
            return nullptr;
        // The object header points to a size-prefixed constant table.
        const auto metadataOffset = object.Read<uint32_t>(16);
        if (!metadataOffset)
            return nullptr;
        const auto tableSize = object.Read<uint32_t>(metadataOffset);
        const auto tableOffset = static_cast<size_t>(metadataOffset) + sizeof(uint32_t);
        if (tableOffset > cached.size() || tableSize > cached.size() - tableOffset)
            return nullptr;
        const ConstantTableReader table(cached.subspan(tableOffset, tableSize));
        constexpr size_t TABLE_HEADER_SIZE = 28;
        constexpr size_t CONSTANT_INFO_SIZE = 20;
        if (table.Read<uint32_t>(0) != TABLE_HEADER_SIZE)
            return nullptr;
        auto result = std::make_unique<d3d9::ShaderInfo>();
        result->m_type = signature & 1u ? d3d9::ShaderType::VERTEX_SHADER : d3d9::ShaderType::PIXEL_SHADER;
        const auto version = table.Read<uint32_t>(8);
        if ((version >> 16u) != (signature & 1u ? 0xfffeu : 0xffffu))
            return nullptr;
        result->m_version_major = (version >> 8u) & 0xffu;
        result->m_version_minor = version & 0xffu;
        if (const auto creator = table.Read<uint32_t>(4))
            result->m_creator = table.String(creator);
        if (const auto target = table.Read<uint32_t>(24))
            result->m_target = table.String(target);
        const auto count = table.Read<uint32_t>(12);
        const auto entries = table.Read<uint32_t>(16);
        if (entries > tableSize || count > (tableSize - entries) / CONSTANT_INFO_SIZE)
            return nullptr;
        for (auto i = 0u; i < count; ++i)
        {
            const auto offset = entries + i * CONSTANT_INFO_SIZE;
            d3d9::ShaderConstant constant;
            constant.m_name = table.String(table.Read<uint32_t>(offset));
            const auto registerSet = table.Read<uint16_t>(offset + 4);
            if (registerSet >= static_cast<unsigned>(d3d9::RegisterSet::MAX))
                return nullptr;
            constant.m_register_set = static_cast<d3d9::RegisterSet>(registerSet);
            constant.m_register_index = table.Read<uint16_t>(offset + 6);
            constant.m_register_count = table.Read<uint16_t>(offset + 8);
            if (const auto type = table.Read<uint32_t>(offset + 12))
            {
                const auto parameterClass = table.Read<uint16_t>(type);
                const auto parameterType = table.Read<uint16_t>(type + 2);
                if (parameterClass >= static_cast<unsigned>(d3d9::ParameterClass::MAX) || parameterType >= static_cast<unsigned>(d3d9::ParameterType::MAX))
                    return nullptr;
                constant.m_class = static_cast<d3d9::ParameterClass>(parameterClass);
                constant.m_type = static_cast<d3d9::ParameterType>(parameterType);
                constant.m_type_rows = table.Read<uint16_t>(type + 4);
                constant.m_type_columns = table.Read<uint16_t>(type + 6);
                constant.m_type_elements = table.Read<uint16_t>(type + 8);
            }
            result->m_constants.push_back(std::move(constant));
        }
        return result;
    }
    catch (const std::runtime_error&)
    {
        return nullptr;
    }
}
