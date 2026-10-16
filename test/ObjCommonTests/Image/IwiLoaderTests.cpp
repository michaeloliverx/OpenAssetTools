#include "Image/IwiLoader.h"
#include "Image/IwiTypes.h"

#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

using namespace image;

namespace
{
    class BitWriter
    {
    public:
        void WriteBits(const unsigned value, const unsigned bitCount)
        {
            for (auto bitIndex = 0u; bitIndex < bitCount; bitIndex++)
            {
                if (m_bit_count % 8u == 0)
                    m_data.emplace_back(0);

                m_data.back() |= ((value >> bitIndex) & 1u) << (m_bit_count % 8u);
                m_bit_count++;
            }
        }

        [[nodiscard]] const std::vector<std::uint8_t>& Data() const
        {
            return m_data;
        }

    private:
        std::vector<std::uint8_t> m_data;
        unsigned m_bit_count = 0;
    };

    void AppendUint16(std::string& data, const std::uint16_t value)
    {
        data.push_back(static_cast<char>(value));
        data.push_back(static_cast<char>(value >> 8u));
    }

    void AppendUint32(std::string& data, const std::uint32_t value)
    {
        data.push_back(static_cast<char>(value));
        data.push_back(static_cast<char>(value >> 8u));
        data.push_back(static_cast<char>(value >> 16u));
        data.push_back(static_cast<char>(value >> 24u));
    }

    std::string MakeIwi6(const iwi6::IwiFormat format, const std::vector<std::uint8_t>& payload)
    {
        std::string data{'I', 'W', 'i', static_cast<char>(IwiVersion::IWI_6)};
        data.push_back(static_cast<char>(format));
        data.push_back(0);
        AppendUint16(data, 2);
        AppendUint16(data, 2);
        AppendUint16(data, 1);

        const auto fileSize = static_cast<std::uint32_t>(sizeof(IwiVersionHeader) + sizeof(iwi6::IwiHeader) + payload.size());
        for (auto picmip = 0u; picmip < 4u; picmip++)
            AppendUint32(data, fileSize);

        data.append(reinterpret_cast<const char*>(payload.data()), payload.size());
        return data;
    }

    std::optional<IwiLoaderResult> Load(const std::string& data)
    {
        std::istringstream stream(data);
        return LoadIwi(stream);
    }

    std::string MakeIwi_8_9(const IwiVersion version, const bool fullSizePicmips)
    {
        std::string data{'I', 'W', 'i', static_cast<char>(version)};
        AppendUint32(data, iwi8::IMG_FLAG_NOPICMIP);
        data.push_back(static_cast<char>(iwi8::IwiFormat::IMG_FORMAT_DXT1));
        data.push_back(0);
        AppendUint16(data, 8);
        AppendUint16(data, 8);
        AppendUint16(data, 1);
        for (const auto size : {88u, 56u, 48u, 40u})
            AppendUint32(data, fullSizePicmips ? 88u : size);
        for (auto mip = 3; mip >= 0; --mip)
            data.append(mip ? 8u : 32u, static_cast<char>(mip + 1));
        return data;
    }

    TEST_CASE("IwiLoader: IWI9 supports CoD Online no-picmip size tables", "[image][iwi]")
    {
        for (const auto fullSizePicmips : {false, true})
        {
            const auto result = Load(MakeIwi_8_9(IwiVersion::IWI_9, fullSizePicmips));
            REQUIRE(result);
            CHECK(result->m_version == IwiVersion::IWI_9);
            REQUIRE(result->m_texture->GetMipMapCount() == 4);
            for (auto mip = 0; mip < 4; ++mip)
                for (auto i = 0uz; i < result->m_texture->GetSizeOfMipLevel(mip); ++i)
                    CHECK(result->m_texture->GetBufferForMipLevel(mip)[i] == mip + 1);
        }
        CHECK(Load(MakeIwi_8_9(IwiVersion::IWI_8, false)));
        CHECK_FALSE(Load(MakeIwi_8_9(IwiVersion::IWI_8, true)));
    }

    TEST_CASE("IwiLoader: IWI9 ignores unused no-picmip size entries", "[image][iwi]")
    {
        auto data = MakeIwi_8_9(IwiVersion::IWI_9, true);
        for (auto offset = 20u; offset < 32u; ++offset)
            data[offset] = 0;
        CHECK(Load(data));
    }

    TEST_CASE("IwiLoader: IWI9 rejects incorrect payload sizes", "[image][iwi]")
    {
        auto data = MakeIwi_8_9(IwiVersion::IWI_9, true);
        SECTION("truncated")
        {
            data.pop_back();
        }
        SECTION("wrong full size")
        {
            ++data[16];
        }
        SECTION("wrong picmip size")
        {
            data = MakeIwi_8_9(IwiVersion::IWI_9, false);
            data[4] = 0;
            ++data[20];
        }
        SECTION("missing no-picmip flag")
        {
            data[4] = 0;
        }
        CHECK_FALSE(Load(data));
    }

    TEST_CASE("IwiLoader: Can decode IWI6 wavelet luminance", "[image][iwi][wavelet]")
    {
        constexpr std::uint8_t PIXEL_VALUE = 0x40;

        BitWriter bits;
        bits.WriteBits(0, 1);     // No predictor delta.
        bits.WriteBits(0, 1);     // Even reconstruction parity.
        bits.WriteBits(0x001, 1); // Three zero alpha/luminance coefficients.
        bits.WriteBits(0x001, 1);
        bits.WriteBits(0x001, 1);

        std::vector<std::uint8_t> payload{PIXEL_VALUE};
        payload.insert(payload.end(), bits.Data().begin(), bits.Data().end());

        auto result = Load(MakeIwi6(iwi6::IwiFormat::IMG_FORMAT_WAVELET_LUMINANCE, payload));

        REQUIRE(result.has_value());
        REQUIRE(result->m_version == IwiVersion::IWI_6);
        REQUIRE(result->m_texture->GetFormat()->GetId() == ImageFormatId::R8);
        REQUIRE(result->m_texture->GetWidth() == 2);
        REQUIRE(result->m_texture->GetHeight() == 2);
        REQUIRE(result->m_texture->GetMipMapCount() == 2);

        const auto* largestMip = result->m_texture->GetBufferForMipLevel(0);
        for (auto pixel = 0u; pixel < 4u; pixel++)
            REQUIRE(largestMip[pixel] == PIXEL_VALUE);
        REQUIRE(result->m_texture->GetBufferForMipLevel(1)[0] == PIXEL_VALUE);
    }

    TEST_CASE("IwiLoader: Can decode IWI6 wavelet RGBA", "[image][iwi][wavelet]")
    {
        constexpr std::uint8_t BLUE = 0x10;
        constexpr std::uint8_t GREEN = 0x20;
        constexpr std::uint8_t RED = 0x30;
        constexpr std::uint8_t ALPHA = 0x40;

        BitWriter bits;
        bits.WriteBits(0, 1); // No predictor delta.

        bits.WriteBits(0, 1); // Blue parity and zero coefficients.
        bits.WriteBits(0x001, 3);
        bits.WriteBits(0x001, 3);
        bits.WriteBits(0x001, 3);

        for (auto channel = 0u; channel < 2u; channel++)
        {
            bits.WriteBits(0, 1); // Red/green parity and zero residual coefficients.
            bits.WriteBits(0x003, 2);
            bits.WriteBits(0x003, 2);
            bits.WriteBits(0x003, 2);
        }

        bits.WriteBits(0, 1); // Alpha parity and zero coefficients.
        bits.WriteBits(0x001, 1);
        bits.WriteBits(0x001, 1);
        bits.WriteBits(0x001, 1);

        std::vector<std::uint8_t> payload{BLUE, GREEN, RED, ALPHA};
        payload.insert(payload.end(), bits.Data().begin(), bits.Data().end());

        auto result = Load(MakeIwi6(iwi6::IwiFormat::IMG_FORMAT_WAVELET_RGBA, payload));

        REQUIRE(result.has_value());
        REQUIRE(result->m_texture->GetFormat()->GetId() == ImageFormatId::B8_G8_R8_A8);

        const auto* largestMip = result->m_texture->GetBufferForMipLevel(0);
        for (auto pixel = 0u; pixel < 4u; pixel++)
        {
            REQUIRE(largestMip[pixel * 4u] == BLUE);
            REQUIRE(largestMip[pixel * 4u + 1u] == GREEN);
            REQUIRE(largestMip[pixel * 4u + 2u] == RED);
            REQUIRE(largestMip[pixel * 4u + 3u] == ALPHA);
        }
    }

    TEST_CASE("IwiLoader: Can decode IWI6 wavelet RGB", "[image][iwi][wavelet]")
    {
        constexpr std::uint8_t BLUE = 0x10;
        constexpr std::uint8_t GREEN = 0x20;
        constexpr std::uint8_t RED = 0x30;

        BitWriter bits;
        bits.WriteBits(0, 1); // No predictor delta.

        bits.WriteBits(0, 1); // Blue parity and zero coefficients.
        bits.WriteBits(0x001, 3);
        bits.WriteBits(0x001, 3);
        bits.WriteBits(0x001, 3);

        for (auto channel = 0u; channel < 2u; channel++)
        {
            bits.WriteBits(0, 1); // Red/green parity and zero residual coefficients.
            bits.WriteBits(0x003, 2);
            bits.WriteBits(0x003, 2);
            bits.WriteBits(0x003, 2);
        }

        std::vector<std::uint8_t> payload{BLUE, GREEN, RED};
        payload.insert(payload.end(), bits.Data().begin(), bits.Data().end());

        auto result = Load(MakeIwi6(iwi6::IwiFormat::IMG_FORMAT_WAVELET_RGB, payload));

        REQUIRE(result.has_value());
        REQUIRE(result->m_texture->GetFormat()->GetId() == ImageFormatId::B8_G8_R8_X8);

        const auto* largestMip = result->m_texture->GetBufferForMipLevel(0);
        for (auto pixel = 0u; pixel < 4u; pixel++)
        {
            REQUIRE(largestMip[pixel * 4u] == BLUE);
            REQUIRE(largestMip[pixel * 4u + 1u] == GREEN);
            REQUIRE(largestMip[pixel * 4u + 2u] == RED);
            REQUIRE(largestMip[pixel * 4u + 3u] == 0xFF);
        }
    }
} // namespace
