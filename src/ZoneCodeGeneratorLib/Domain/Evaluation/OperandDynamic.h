#pragma once
#include "Domain/Information/MemberInformation.h"
#include "Domain/Information/StructureInformation.h"
#include "IEvaluation.h"

#include <memory>
#include <vector>

struct MemberAccessor
{
    MemberInformation* m_member;
    std::vector<std::unique_ptr<IEvaluation>> m_array_indices;

    MemberAccessor(MemberInformation* member, std::vector<std::unique_ptr<IEvaluation>> arrayIndices)
        : m_member(member),
          m_array_indices(std::move(arrayIndices))
    {
    }

    explicit MemberAccessor(MemberInformation* member)
        : m_member(member)
    {
    }
};

class OperandDynamic final : public IEvaluation
{
public:
    explicit OperandDynamic(StructureInformation* structure);
    OperandDynamic(StructureInformation* structure, std::vector<MemberAccessor> memberChain);

    // Legacy constructor for backward compatibility
    OperandDynamic(StructureInformation* structure,
                   std::vector<MemberInformation*> referencedMemberChain,
                   std::vector<std::unique_ptr<IEvaluation>> arrayIndices);

    [[nodiscard]] EvaluationType GetType() const override;
    [[nodiscard]] bool IsStatic() const override;
    [[nodiscard]] int EvaluateNumeric() const override;

    StructureInformation* const m_structure;
    std::vector<MemberAccessor> m_member_chain;
};
