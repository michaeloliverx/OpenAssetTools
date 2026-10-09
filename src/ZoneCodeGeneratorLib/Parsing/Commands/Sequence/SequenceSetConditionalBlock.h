#pragma once

#include "Parsing/Commands/Impl/CommandsParser.h"

class SequenceSetConditionalBlock final : public CommandsParser::sequence_t
{
public:
    SequenceSetConditionalBlock();

protected:
    void ProcessMatch(CommandsParserState* state, SequenceResult<CommandsParserValue>& result) const override;
};
