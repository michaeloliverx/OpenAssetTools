#include "Parsing/Commands/Impl/CommandsLexer.h"
#include "Parsing/Mock/MockParserLineStream.h"

#include <catch2/catch_test_macros.hpp>

namespace test::parsing::commands::impl::commands_lexer
{
    void ExpectIntegerToken(CommandsLexer& lexer, int number)
    {
        REQUIRE(lexer.GetToken(0).m_type == CommandsParserValueType::INTEGER);
        REQUIRE(lexer.GetToken(0).IntegerValue() == number);
        lexer.PopTokens(1);
    }

    void ExpectIdentifierToken(CommandsLexer& lexer, const std::string& identifier)
    {
        REQUIRE(lexer.GetToken(0).m_type == CommandsParserValueType::IDENTIFIER);
        REQUIRE(lexer.GetToken(0).IdentifierValue() == identifier);
        lexer.PopTokens(1);
    }

    void ExpectTokenWithType(CommandsLexer& lexer, CommandsParserValueType type)
    {
        REQUIRE(lexer.GetToken(0).m_type == type);
        lexer.PopTokens(1);
    }

    void ExpectCharacterToken(CommandsLexer& lexer, char c)
    {
        REQUIRE(lexer.GetToken(0).m_type == CommandsParserValueType::CHARACTER);
        REQUIRE(lexer.GetToken(0).CharacterValue() == c);
        lexer.PopTokens(1);
    }

    TEST_CASE("CommandsLexer: Ensure can parse signed integer literals", "[parsing][commands]")
    {
        const std::vector<std::string> lines{"+12 -34 56"};

        MockParserLineStream mockStream(lines);
        CommandsLexer lexer(&mockStream);

        ExpectIntegerToken(lexer, 12);
        ExpectIntegerToken(lexer, -34);
        ExpectIntegerToken(lexer, 56);

        REQUIRE(lexer.GetToken(0).m_type == CommandsParserValueType::END_OF_FILE);
    }

    TEST_CASE("CommandsLexer: Ensure can parse logical or expression with negative integer across lines", "[parsing][commands]")
    {
        const std::vector<std::string> lines{"StreamedSound::filename::fileIndex == 0", "    || StreamedSound::filename::fileIndex == -1;"};

        MockParserLineStream mockStream(lines);
        CommandsLexer lexer(&mockStream);

        ExpectIdentifierToken(lexer, "StreamedSound");
        ExpectCharacterToken(lexer, ':');
        ExpectCharacterToken(lexer, ':');
        ExpectIdentifierToken(lexer, "filename");
        ExpectCharacterToken(lexer, ':');
        ExpectCharacterToken(lexer, ':');
        ExpectIdentifierToken(lexer, "fileIndex");
        ExpectTokenWithType(lexer, CommandsParserValueType::EQUALS);
        ExpectIntegerToken(lexer, 0);
        ExpectTokenWithType(lexer, CommandsParserValueType::LOGICAL_OR);
        ExpectIdentifierToken(lexer, "StreamedSound");
        ExpectCharacterToken(lexer, ':');
        ExpectCharacterToken(lexer, ':');
        ExpectIdentifierToken(lexer, "filename");
        ExpectCharacterToken(lexer, ':');
        ExpectCharacterToken(lexer, ':');
        ExpectIdentifierToken(lexer, "fileIndex");
        ExpectTokenWithType(lexer, CommandsParserValueType::EQUALS);
        ExpectIntegerToken(lexer, -1);
        ExpectCharacterToken(lexer, ';');

        REQUIRE(lexer.GetToken(0).m_type == CommandsParserValueType::END_OF_FILE);
    }
} // namespace test::parsing::commands::impl::commands_lexer
