#include "Domain/Computations/MemberComputations.h"
#include "Domain/Definition/PointerDeclarationModifier.h"
#include "Parsing/Commands/Sequence/SequenceSetConditionalBlock.h"
#include "Parsing/Mock/MockLexer.h"
#include "Parsing/PostProcessing/CreateMemberInformationPostProcessor.h"
#include "Parsing/PostProcessing/CreateStructureInformationPostProcessor.h"
#include "Persistence/InMemory/InMemoryRepository.h"

#include <catch2/catch_test_macros.hpp>

namespace test::parsing::commands::sequence::sequence_set_conditional_block
{
    class CommandsSequenceTestsHelper
    {
    public:
        std::unique_ptr<IDataRepository> m_repository;
        std::unique_ptr<CommandsParserState> m_state;
        std::unique_ptr<ILexer<CommandsParserValue>> m_lexer;

        StructDefinition* m_test_struct_t;
        StructureInformation* m_test_struct;
        MemberInformation* m_should_delay_member;
        MemberInformation* m_pixels_member;

        unsigned m_consumed_token_count;

    private:
        [[nodiscard]] bool CreateInformation() const
        {
            auto createStructureInformation = std::make_unique<CreateStructureInformationPostProcessor>();
            auto createMemberInformation = std::make_unique<CreateMemberInformationPostProcessor>();

            return createStructureInformation->PostProcess(m_repository.get()) && createMemberInformation->PostProcess(m_repository.get());
        }

        void AddSampleData()
        {
            auto def = std::make_unique<StructDefinition>("", "test_struct_t", 8);
            def->m_members.emplace_back(std::make_shared<Variable>("m_should_delay", std::make_unique<TypeDeclaration>(BaseTypeDefinition::BOOL)));
            auto pixelsType = std::make_unique<TypeDeclaration>(BaseTypeDefinition::UNSIGNED_CHAR);
            pixelsType->m_declaration_modifiers.emplace_back(std::make_unique<PointerDeclarationModifier>());
            def->m_members.emplace_back(std::make_shared<Variable>("m_pixels", std::move(pixelsType)));
            m_test_struct_t = def.get();
            m_repository->Add(std::move(def));

            m_repository->Add(std::make_unique<FastFileBlock>("XFILE_BLOCK_LARGE_RUNTIME", 0, FastFileBlockType::DELAY, false));
            m_repository->Add(std::make_unique<FastFileBlock>("XFILE_BLOCK_LARGE", 1, FastFileBlockType::NORMAL, false));
        }

        void RetrieveInformationPointers()
        {
            m_test_struct = m_repository->GetInformationFor(m_test_struct_t);
            REQUIRE(m_test_struct != nullptr);
            REQUIRE(m_test_struct->m_ordered_members.size() == 2);

            m_should_delay_member = m_test_struct->m_ordered_members[0].get();
            m_pixels_member = m_test_struct->m_ordered_members[1].get();
            REQUIRE(m_should_delay_member != nullptr);
            REQUIRE(m_pixels_member != nullptr);
        }

    public:
        CommandsSequenceTestsHelper()
            : m_repository(std::make_unique<InMemoryRepository>()),
              m_state(std::make_unique<CommandsParserState>(m_repository.get())),
              m_test_struct_t(nullptr),
              m_test_struct(nullptr),
              m_should_delay_member(nullptr),
              m_pixels_member(nullptr),
              m_consumed_token_count(0u)
        {
            AddSampleData();
            REQUIRE(CreateInformation());
            RetrieveInformationPointers();
        }

        void Tokens(std::initializer_list<Movable<CommandsParserValue>> tokens)
        {
            m_lexer = std::make_unique<MockLexer<CommandsParserValue>>(tokens, CommandsParserValue::EndOfFile(TokenPos()));
        }

        bool PerformTest()
        {
            REQUIRE(m_lexer);
            const auto sequence = std::make_unique<SequenceSetConditionalBlock>();
            return sequence->MatchSequence(m_lexer.get(), m_state.get(), m_consumed_token_count);
        }
    };

    TEST_CASE("SequenceSetConditionalBlock: Ensure can parse conditional block directive", "[parsing][sequence]")
    {
        CommandsSequenceTestsHelper helper;
        const TokenPos pos;
        helper.Tokens({
            CommandsParserValue::Identifier(pos, new std::string("set")),
            CommandsParserValue::Identifier(pos, new std::string("conditional")),
            CommandsParserValue::Identifier(pos, new std::string("block")),
            CommandsParserValue::Identifier(pos, new std::string("test_struct_t")),
            CommandsParserValue::Character(pos, ':'),
            CommandsParserValue::Character(pos, ':'),
            CommandsParserValue::Identifier(pos, new std::string("m_pixels")),
            CommandsParserValue::Identifier(pos, new std::string("m_should_delay")),
            CommandsParserValue::Identifier(pos, new std::string("XFILE_BLOCK_LARGE_RUNTIME")),
            CommandsParserValue::Identifier(pos, new std::string("XFILE_BLOCK_LARGE")),
            CommandsParserValue::Character(pos, ';'),
            CommandsParserValue::EndOfFile(pos),
        });

        const auto result = helper.PerformTest();

        REQUIRE(result);
        REQUIRE(helper.m_consumed_token_count == 11);
        REQUIRE(helper.m_pixels_member->m_conditional_block_condition != nullptr);
        REQUIRE(helper.m_pixels_member->m_conditional_block_true != nullptr);
        REQUIRE(helper.m_pixels_member->m_conditional_block_true->m_name == "XFILE_BLOCK_LARGE_RUNTIME");
        REQUIRE(helper.m_pixels_member->m_conditional_block_false != nullptr);
        REQUIRE(helper.m_pixels_member->m_conditional_block_false->m_name == "XFILE_BLOCK_LARGE");
        REQUIRE(MemberComputations(helper.m_pixels_member).CanBeInRuntimeOrDelayBlock());
    }

    TEST_CASE("SequenceSetConditionalBlock: Fails for unknown block", "[parsing][sequence]")
    {
        CommandsSequenceTestsHelper helper;
        const TokenPos pos;
        helper.Tokens({
            CommandsParserValue::Identifier(pos, new std::string("set")),
            CommandsParserValue::Identifier(pos, new std::string("conditional")),
            CommandsParserValue::Identifier(pos, new std::string("block")),
            CommandsParserValue::Identifier(pos, new std::string("test_struct_t")),
            CommandsParserValue::Character(pos, ':'),
            CommandsParserValue::Character(pos, ':'),
            CommandsParserValue::Identifier(pos, new std::string("m_pixels")),
            CommandsParserValue::Integer(pos, 1),
            CommandsParserValue::Identifier(pos, new std::string("XFILE_BLOCK_UNKNOWN")),
            CommandsParserValue::Identifier(pos, new std::string("XFILE_BLOCK_LARGE")),
            CommandsParserValue::Character(pos, ';'),
            CommandsParserValue::EndOfFile(pos),
        });

        REQUIRE_THROWS_AS(helper.PerformTest(), ParsingException);
        REQUIRE(helper.m_pixels_member->m_conditional_block_condition == nullptr);
        REQUIRE(helper.m_pixels_member->m_conditional_block_true == nullptr);
        REQUIRE(helper.m_pixels_member->m_conditional_block_false == nullptr);
    }
} // namespace test::parsing::commands::sequence::sequence_set_conditional_block
