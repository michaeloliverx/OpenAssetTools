#include "Parsing/Commands/Sequence/SequenceEndianness.h"
#include "Parsing/Mock/MockLexer.h"
#include "Persistence/InMemory/InMemoryRepository.h"

#include <catch2/catch_test_macros.hpp>

namespace test::parsing::commands::sequence::sequence_endianness
{
    class CommandsSequenceTestsHelper
    {
    public:
        std::unique_ptr<IDataRepository> m_repository;
        std::unique_ptr<CommandsParserState> m_state;
        std::unique_ptr<ILexer<CommandsParserValue>> m_lexer;

        unsigned m_consumed_token_count;

        CommandsSequenceTestsHelper()
            : m_repository(std::make_unique<InMemoryRepository>()),
              m_state(std::make_unique<CommandsParserState>(m_repository.get())),
              m_consumed_token_count(0u)
        {
        }

        void Tokens(std::initializer_list<Movable<CommandsParserValue>> tokens)
        {
            m_lexer = std::make_unique<MockLexer<CommandsParserValue>>(tokens, CommandsParserValue::EndOfFile(TokenPos()));
        }

        bool PerformTest()
        {
            REQUIRE(m_lexer);
            const auto sequence = std::make_unique<SequenceEndianness>();
            return sequence->MatchSequence(m_lexer.get(), m_state.get(), m_consumed_token_count);
        }
    };

    TEST_CASE("SequenceEndianness: Ensure little-endian is the default", "[parsing][sequence]")
    {
        CommandsSequenceTestsHelper helper;

        REQUIRE(helper.m_repository->GetEndianness() == std::endian::little);
    }

    TEST_CASE("SequenceEndianness: Ensure can set little-endian", "[parsing][sequence]")
    {
        CommandsSequenceTestsHelper helper;
        const TokenPos pos;
        helper.Tokens({
            CommandsParserValue::Identifier(pos, new std::string("endianness")),
            CommandsParserValue::Identifier(pos, new std::string("little")),
            CommandsParserValue::Character(pos, ';'),
            CommandsParserValue::EndOfFile(pos),
        });

        const auto result = helper.PerformTest();

        REQUIRE(result);
        REQUIRE(helper.m_consumed_token_count == 3);
        REQUIRE(helper.m_repository->GetEndianness() == std::endian::little);
    }

    TEST_CASE("SequenceEndianness: Ensure can set big-endian", "[parsing][sequence]")
    {
        CommandsSequenceTestsHelper helper;
        const TokenPos pos;
        helper.Tokens({
            CommandsParserValue::Identifier(pos, new std::string("endianness")),
            CommandsParserValue::Identifier(pos, new std::string("big")),
            CommandsParserValue::Character(pos, ';'),
            CommandsParserValue::EndOfFile(pos),
        });

        const auto result = helper.PerformTest();

        REQUIRE(result);
        REQUIRE(helper.m_consumed_token_count == 3);
        REQUIRE(helper.m_repository->GetEndianness() == std::endian::big);
    }

    TEST_CASE("SequenceEndianness: Ensure cannot match unknown value", "[parsing][sequence]")
    {
        CommandsSequenceTestsHelper helper;
        const TokenPos pos;
        helper.Tokens({
            CommandsParserValue::Identifier(pos, new std::string("endianness")),
            CommandsParserValue::Identifier(pos, new std::string("middle")),
            CommandsParserValue::Character(pos, ';'),
            CommandsParserValue::EndOfFile(pos),
        });

        REQUIRE_THROWS_AS(helper.PerformTest(), ParsingException);
        REQUIRE(helper.m_repository->GetEndianness() == std::endian::little);
    }
} // namespace test::parsing::commands::sequence::sequence_endianness
