#include "SequenceEndianness.h"

#include "Parsing/Commands/Matcher/CommandsMatcherFactory.h"

namespace
{
    static constexpr auto CAPTURE_ENDIANNESS = 1;
}

SequenceEndianness::SequenceEndianness()
{
    const CommandsMatcherFactory create(this);

    AddMatchers({
        create.Keyword("endianness"),
        create.Identifier().Capture(CAPTURE_ENDIANNESS),
        create.Char(';'),
    });
}

void SequenceEndianness::ProcessMatch(CommandsParserState* state, SequenceResult<CommandsParserValue>& result) const
{
    const auto& endiannessToken = result.NextCapture(CAPTURE_ENDIANNESS);
    const auto& endianness = endiannessToken.IdentifierValue();

    if (endianness == "little")
        state->SetEndianness(std::endian::little);
    else if (endianness == "big")
        state->SetEndianness(std::endian::big);
    else
        throw ParsingException(endiannessToken.GetPos(), "Unknown endianness");
}
