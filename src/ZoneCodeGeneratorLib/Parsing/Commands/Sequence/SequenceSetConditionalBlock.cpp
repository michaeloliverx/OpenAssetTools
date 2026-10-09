#include "SequenceSetConditionalBlock.h"

#include "Parsing/Commands/Matcher/CommandsCommonMatchers.h"
#include "Parsing/Commands/Matcher/CommandsMatcherFactory.h"

namespace
{
    static constexpr auto CAPTURE_START = 1;
    static constexpr auto CAPTURE_TYPE = 2;
    static constexpr auto CAPTURE_EVALUATION = 3;
    static constexpr auto CAPTURE_BLOCK_TRUE = 4;
    static constexpr auto CAPTURE_BLOCK_FALSE = 5;
} // namespace

SequenceSetConditionalBlock::SequenceSetConditionalBlock()
{
    const CommandsMatcherFactory create(this);

    AddLabeledMatchers(CommandsCommonMatchers::Typename(this), CommandsCommonMatchers::LABEL_TYPENAME);
    AddLabeledMatchers(CommandsCommonMatchers::Evaluation(this), CommandsCommonMatchers::LABEL_EVALUATION);
    AddMatchers({
        create.Keyword("set").Capture(CAPTURE_START),
        create.Keyword("conditional"),
        create.Keyword("block"),
        create.Label(CommandsCommonMatchers::LABEL_TYPENAME).Capture(CAPTURE_TYPE),
        create.Label(CommandsCommonMatchers::LABEL_EVALUATION).Capture(CAPTURE_EVALUATION),
        create.Identifier().Capture(CAPTURE_BLOCK_TRUE),
        create.Identifier().Capture(CAPTURE_BLOCK_FALSE),
        create.Char(';'),
    });
}

void SequenceSetConditionalBlock::ProcessMatch(CommandsParserState* state, SequenceResult<CommandsParserValue>& result) const
{
    const auto& typeNameToken = result.NextCapture(CAPTURE_TYPE);

    StructureInformation* type;
    std::vector<MemberInformation*> memberChain;
    if (!state->GetTypenameAndMembersFromTypename(typeNameToken.TypeNameValue(), type, memberChain))
        throw ParsingException(typeNameToken.GetPos(), "Unknown type");

    if (memberChain.empty())
        throw ParsingException(typeNameToken.GetPos(), "Conditional blocks can only be set on members and not for types");

    const auto& blockTrueToken = result.NextCapture(CAPTURE_BLOCK_TRUE);
    auto* blockTrue = state->GetRepository()->GetFastFileBlockByName(blockTrueToken.IdentifierValue());
    if (blockTrue == nullptr)
        throw ParsingException(blockTrueToken.GetPos(), "Unknown block");

    const auto& blockFalseToken = result.NextCapture(CAPTURE_BLOCK_FALSE);
    auto* blockFalse = state->GetRepository()->GetFastFileBlockByName(blockFalseToken.IdentifierValue());
    if (blockFalse == nullptr)
        throw ParsingException(blockFalseToken.GetPos(), "Unknown block");

    auto conditionEvaluation = CommandsCommonMatchers::ProcessEvaluation(state, result, type);

    auto* member = memberChain.back();
    member->m_conditional_block_condition = std::move(conditionEvaluation);
    member->m_conditional_block_true = blockTrue;
    member->m_conditional_block_false = blockFalse;
}
