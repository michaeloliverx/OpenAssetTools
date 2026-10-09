#pragma once

#include "Parsing/Commands/Impl/CommandsParser.h"

class SequenceEndianness final : public CommandsParser::sequence_t
{
public:
    SequenceEndianness();

protected:
    void ProcessMatch(CommandsParserState* state, SequenceResult<CommandsParserValue>& result) const override;
};
