#pragma once

#include "LoadingException.h"

#include <cstddef>
#include <cstdint>
#include <format>

class InvalidXStringIndexException final : public LoadingException
{
public:
    InvalidXStringIndexException(const std::uint32_t index, const std::size_t count)
        : m_index(index),
          m_count(count)
    {
    }

    std::string DetailedMessage() override
    {
        return std::format("Zone referenced string index {} in a table containing {} strings", m_index, m_count);
    }

private:
    std::uint32_t m_index;
    std::size_t m_count;
};
