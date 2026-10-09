#include "StepAllocXBlocks.h"

#include "Loading/Exception/InvalidXBlockSizeException.h"
#include "Utils/Endianness.h"

namespace
{
    constexpr uint64_t MAX_XBLOCK_SIZE = 0x3C000000; // ~1GB

    class StepAllocXBlocks final : public ILoadingStep
    {
    public:
        explicit StepAllocXBlocks(const GameEndianness endianness)
            : m_endianness(endianness)
        {
        }

        void PerformStep(ZoneLoader& zoneLoader, ILoadingStream& stream) override
        {
            const auto blockCount = static_cast<unsigned>(zoneLoader.m_blocks.size());

            std::vector<xblock_size_t> blockSizes(blockCount);
            stream.Load(blockSizes.data(), sizeof(xblock_size_t) * blockCount);

            if (m_endianness == GameEndianness::BE)
            {
                for (auto& blockSize : blockSizes)
                    blockSize = endianness::FromBigEndian(blockSize);
            }

            uint64_t totalMemory = 0;
            for (unsigned int block = 0; block < blockCount; block++)
            {
                totalMemory += blockSizes[block];
            }

            if (totalMemory > MAX_XBLOCK_SIZE)
            {
                throw InvalidXBlockSizeException(totalMemory, MAX_XBLOCK_SIZE);
            }

            for (unsigned int block = 0; block < blockCount; block++)
            {
                zoneLoader.m_blocks[block]->Alloc(blockSizes[block]);
            }
        }

    private:
        GameEndianness m_endianness;
    };
} // namespace

namespace step
{
    std::unique_ptr<ILoadingStep> CreateStepAllocXBlocks(const GameEndianness endianness)
    {
        return std::make_unique<StepAllocXBlocks>(endianness);
    }
} // namespace step
