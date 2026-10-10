#pragma once

#include "XAnim/XAnimCommon.h"

#include <cstdint>
#include <exception>
#include <expected>
#include <string>
#include <vector>

namespace xanim
{
    class XAnimBoneCounts
    {
    public:
        XAnimBoneCounts(size_t noQuatCount,
                        size_t halfQuatCount,
                        size_t fullQuatCount,
                        size_t halfQuatNoSizeCount,
                        size_t fullQuatNoSizeCount,
                        size_t smallTransCount,
                        size_t fullTransCount,
                        size_t transNoSizeCount,
                        size_t noTransCount,
                        size_t precisionQuatCount = 0,
                        size_t precisionQuatNoSizeCount = 0);

        [[nodiscard]] size_t GetCountForQuatType(QuatType quatType) const;
        [[nodiscard]] size_t GetCountForTransType(TransType transType) const;
        [[nodiscard]] size_t GetPrecisionQuatCount() const;
        [[nodiscard]] size_t GetPrecisionQuatNoSizeCount() const;

    private:
        std::array<size_t, 9> m_counts;
        size_t m_precision_quat_count;
        size_t m_precision_quat_no_size_count;
    };

    class FlatDataReadException : public std::exception
    {
    public:
        explicit FlatDataReadException(std::string message);

        [[nodiscard]] const char* what() const noexcept override;
        [[nodiscard]] const std::string& message() const;

    private:
        std::string m_message;
    };

    class FlatXAnimReadCursor
    {
    public:
        FlatXAnimReadCursor(uint8_t* dataByte,
                            size_t dataByteCount,
                            int16_t* dataShort,
                            size_t dataShortCount,
                            int32_t* dataInt,
                            size_t dataIntCount,
                            uint8_t* randomDataByte,
                            size_t randomDataByteCount,
                            int16_t* randomDataShort,
                            size_t randomDataShortCount,
                            uint16_t* indices,
                            size_t indicesCount,
                            int32_t* randomDataInt = nullptr,
                            size_t randomDataIntCount = 0);

        uint8_t PopDataByte();
        int16_t PopDataShort();
        void ReadDataShort(void* dst, size_t count);
        void SkipDataShort(size_t count);
        int32_t PopDataInt();
        int32_t PopRandomDataInt();
        void ReadRandomDataByte(void* dst, size_t count);
        void ReadRandomDataShort(void* dst, size_t count);
        void ReadIndices(void* dst, size_t count);

        [[nodiscard]] std::expected<void, std::string> ExpectEndOfData() const;

    private:
        uint8_t* m_data_byte;
        size_t m_data_byte_count;

        int16_t* m_data_short;
        size_t m_data_short_count;

        int32_t* m_data_int;
        size_t m_data_int_count;

        uint8_t* m_random_data_byte;
        size_t m_random_data_byte_count;

        int16_t* m_random_data_short;
        size_t m_random_data_short_count;

        int32_t* m_random_data_int;
        size_t m_random_data_int_count;

        uint16_t* m_indices;
        size_t m_indices_count;
    };

    std::expected<std::vector<BoneTrack>, std::string> CreateBoneTracksFromFlatData(std::vector<std::string> boneNames,
                                                                                    const XAnimBoneCounts& boneCounts,
                                                                                    FlatXAnimReadCursor& cursor,
                                                                                    bool useByteIndices,
                                                                                    QuatEncoding quatEncoding = QuatEncoding::SIGNED_16);
} // namespace xanim
