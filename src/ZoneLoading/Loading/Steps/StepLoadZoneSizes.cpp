#include "StepLoadZoneSizes.h"

#include "Utils/Endianness.h"

namespace
{
    class StepLoadZoneSizes final : public step::IStepLoadZoneSizes
    {
    public:
        explicit StepLoadZoneSizes(const GameEndianness endianness)
            : m_size(0),
              m_external_size(0),
              m_endianness(endianness)
        {
        }

        void PerformStep(ZoneLoader& zoneLoader, ILoadingStream& stream) override
        {
            stream.Load(&m_size, sizeof(m_size));
            stream.Load(&m_external_size, sizeof(m_external_size));

            if (m_endianness == GameEndianness::BE)
            {
                m_size = endianness::FromBigEndian(m_size);
                m_external_size = endianness::FromBigEndian(m_external_size);
            }
        }

        [[nodiscard]] uint32_t GetSize() const override
        {
            return m_size;
        }

        [[nodiscard]] uint32_t GetExternalSize() const override
        {
            return m_external_size;
        }

    private:
        uint32_t m_size;
        uint32_t m_external_size;
        GameEndianness m_endianness;
    };
} // namespace

namespace step
{
    std::unique_ptr<IStepLoadZoneSizes> CreateStepLoadZoneSizes(const GameEndianness endianness)
    {
        return std::make_unique<StepLoadZoneSizes>(endianness);
    }
} // namespace step
