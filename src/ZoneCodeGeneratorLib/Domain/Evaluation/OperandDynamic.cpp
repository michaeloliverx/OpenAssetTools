#include "OperandDynamic.h"

#include <cassert>

OperandDynamic::OperandDynamic(StructureInformation* structure)
    : m_structure(structure)
{
}

OperandDynamic::OperandDynamic(StructureInformation* structure, std::vector<MemberAccessor> memberChain)
    : m_structure(structure),
      m_member_chain(std::move(memberChain))
{
}

OperandDynamic::OperandDynamic(StructureInformation* structure,
                               std::vector<MemberInformation*> referencedMemberChain,
                               std::vector<std::unique_ptr<IEvaluation>> arrayIndices)
    : m_structure(structure)
{
    // Legacy constructor: convert old format to new format
    // Put all array indices on the last member in the chain
    for (size_t i = 0; i < referencedMemberChain.size(); ++i)
    {
        if (i == referencedMemberChain.size() - 1 && !arrayIndices.empty())
        {
            m_member_chain.emplace_back(referencedMemberChain[i], std::move(arrayIndices));
        }
        else
        {
            m_member_chain.emplace_back(referencedMemberChain[i]);
        }
    }
}

EvaluationType OperandDynamic::GetType() const
{
    return EvaluationType::OPERAND_DYNAMIC;
}

bool OperandDynamic::IsStatic() const
{
    return false;
}

int OperandDynamic::EvaluateNumeric() const
{
    assert(false);
    return 0;
}
