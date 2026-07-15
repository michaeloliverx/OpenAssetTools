#pragma once

#include <bit>
#include <concepts>
#include <cstdint>
#include <stdexcept>
#include <type_traits>

namespace endianness
{
    template<std::integral T> constexpr T ByteSwap(const T in)
    {
        using Unsigned_t = std::make_unsigned_t<T>;
        return static_cast<T>(std::byteswap(static_cast<Unsigned_t>(in)));
    }

    template<std::integral T> constexpr T ToLittleEndian(const T in)
    {
        static_assert(std::endian::native == std::endian::little || std::endian::native == std::endian::big, "Mixed-endian platforms are not supported");

        if constexpr (std::endian::native == std::endian::little)
            return in;
        else
            return ByteSwap(in);
    }

    template<std::integral T> constexpr T FromLittleEndian(const T in)
    {
        return ToLittleEndian(in);
    }

    template<std::integral T> constexpr T ToBigEndian(const T in)
    {
        static_assert(std::endian::native == std::endian::little || std::endian::native == std::endian::big, "Mixed-endian platforms are not supported");

        if constexpr (std::endian::native == std::endian::big)
            return in;
        else
            return ByteSwap(in);
    }

    template<std::integral T> constexpr T FromBigEndian(const T in)
    {
        return ToBigEndian(in);
    }

    /**
     * Customization point for endian-swapping aggregate game types in place.
     *
     * Arithmetic values, enums, pointers, and arrays are handled by
     * FromBigEndianInPlace directly. Games can specialize this type for the
     * structs that are shared with a big-endian platform.
     */
    template<typename T> struct InPlaceBigEndianConverter
    {
        static void Convert(T&)
        {
            throw std::logic_error("No big-endian converter is registered for this type");
        }
    };

    template<typename T> void FromBigEndianInPlace(T& value)
    {
        using Value = std::remove_cv_t<T>;
        auto& mutableValue = const_cast<Value&>(value);

        if constexpr (sizeof(Value) == 1u)
        {
            // Single-byte values are endian-independent.
        }
        else if constexpr (std::integral<Value>)
        {
            mutableValue = FromBigEndian(mutableValue);
        }
        else if constexpr (std::is_enum_v<Value>)
        {
            using Underlying = std::underlying_type_t<Value>;
            auto underlying = static_cast<Underlying>(mutableValue);
            underlying = FromBigEndian(underlying);
            mutableValue = static_cast<Value>(underlying);
        }
        else if constexpr (std::is_floating_point_v<Value> && sizeof(Value) == sizeof(uint32_t))
        {
            auto bits = FromBigEndian(std::bit_cast<uint32_t>(mutableValue));
            mutableValue = std::bit_cast<Value>(bits);
        }
        else if constexpr (std::is_floating_point_v<Value> && sizeof(Value) == sizeof(uint64_t))
        {
            auto bits = FromBigEndian(std::bit_cast<uint64_t>(mutableValue));
            mutableValue = std::bit_cast<Value>(bits);
        }
        else if constexpr (std::is_pointer_v<Value>)
        {
            auto bits = FromBigEndian(reinterpret_cast<uintptr_t>(mutableValue));
            mutableValue = reinterpret_cast<Value>(bits);
        }
        else if constexpr (std::is_array_v<Value>)
        {
            for (auto& entry : mutableValue)
                FromBigEndianInPlace(entry);
        }
        else
        {
            InPlaceBigEndianConverter<Value>::Convert(mutableValue);
        }
    }
} // namespace endianness
