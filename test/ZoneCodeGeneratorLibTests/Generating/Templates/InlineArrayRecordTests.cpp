#include "Domain/Definition/ArrayDeclarationModifier.h"
#include "Domain/Definition/PointerDeclarationModifier.h"
#include "Domain/Evaluation/OperandDynamic.h"
#include "Generating/Templates/ZoneLoadTemplate.h"
#include "Generating/Templates/ZoneMarkTemplate.h"
#include "Generating/Templates/ZoneWriteTemplate.h"
#include "Parsing/PostProcessing/CreateMemberInformationPostProcessor.h"
#include "Parsing/PostProcessing/CreateStructureInformationPostProcessor.h"
#include "Persistence/InMemory/InMemoryRepository.h"

#include <bit>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <cctype>
#include <sstream>

TEST_CASE("Record arrays respect inline array sizes", "[generating][array]")
{
    const auto dynamicSize = GENERATE(false, true);
    const auto endianness = GENERATE(std::endian::little, std::endian::big);
    InMemoryRepository repository;
    repository.SetGame("Test");
    repository.SetEndianness(endianness);
    repository.Add(std::make_unique<FastFileBlock>("TEMP", 0, FastFileBlockType::TEMP, true));
    repository.Add(std::make_unique<FastFileBlock>("NORMAL", 1, FastFileBlockType::NORMAL, true));

    auto record = std::make_unique<StructDefinition>("", "Record", 4);
    record->m_members.emplace_back(std::make_shared<Variable>("valueCount", std::make_unique<TypeDeclaration>(BaseTypeDefinition::UNSIGNED_INT)));
    auto values = std::make_unique<TypeDeclaration>(BaseTypeDefinition::UNSIGNED_INT);
    auto array = std::make_unique<ArrayDeclarationModifier>(dynamicSize ? 1 : 4);
    auto* valueArray = array.get();
    values->m_declaration_modifiers.emplace_back(std::move(array));
    record->m_members.emplace_back(std::make_shared<Variable>("values", std::move(values)));
    const auto* recordDefinition = record.get();
    repository.Add(std::move(record));

    auto entry = std::make_unique<StructDefinition>("", "Entry", 4);
    entry->m_members.emplace_back(std::make_shared<Variable>("recordCount", std::make_unique<TypeDeclaration>(BaseTypeDefinition::UNSIGNED_INT)));
    auto records = std::make_unique<TypeDeclaration>(recordDefinition);
    auto pointer = std::make_unique<PointerDeclarationModifier>();
    auto* recordPointer = pointer.get();
    records->m_declaration_modifiers.emplace_back(std::move(pointer));
    entry->m_members.emplace_back(std::make_shared<Variable>("records", std::move(records)));
    const auto* entryDefinition = entry.get();
    repository.Add(std::move(entry));

    REQUIRE(CreateStructureInformationPostProcessor().PostProcess(&repository));
    REQUIRE(CreateMemberInformationPostProcessor().PostProcess(&repository));
    auto* entryInfo = repository.GetInformationFor(entryDefinition);
    auto* recordInfo = repository.GetInformationFor(recordDefinition);
    entryInfo->m_asset_name = "ASSET_TYPE_ENTRY";
    entryInfo->m_requires_marking = true;
    entryInfo->m_has_matching_cross_platform_structure = true;
    recordInfo->m_requires_marking = true;
    recordInfo->m_has_matching_cross_platform_structure = true;
    recordInfo->m_non_embedded_reference_exists = true;
    recordInfo->m_ordered_members[1]->m_is_script_string = true;

    std::vector<MemberAccessor> valueCount;
    valueCount.emplace_back(recordInfo->m_ordered_members[0].get());
    auto size = std::make_unique<OperandDynamic>(recordInfo, std::move(valueCount));
    if (dynamicSize)
        valueArray->m_dynamic_size_evaluation = std::move(size);
    else
        valueArray->m_dynamic_count_evaluation = std::move(size);
    std::vector<MemberAccessor> recordCount;
    recordCount.emplace_back(entryInfo->m_ordered_members[0].get());
    recordPointer->m_count_evaluation = std::make_unique<OperandDynamic>(entryInfo, std::move(recordCount));

    std::unique_ptr<ICodeTemplate> codeTemplate;
    std::string method;
    std::string expected;
    std::string excluded;
    SECTION("Zone loading")
    {
        codeTemplate = std::make_unique<ZoneLoadTemplate>();
        method = "LoadArray_Record";
        expected = dynamicSize ? "Load_Record(true);varRecord=m_stream.Alloc<Record>(1);" : "m_stream.Load<Record>(varRecord,count);";
        excluded = dynamicSize ? "Load_Record(false);" : "Load_Record(true);";
    }
    SECTION("Zone writing")
    {
        codeTemplate = std::make_unique<ZoneWriteTemplate>();
        method = "WriteArray_Record";
        expected = dynamicSize ? "constautostride=offsetof(Record,values)+sizeof(varRecord->values[0])*(varRecord->valueCount);"
                                 "auto*next=reinterpret_cast<Record*>(reinterpret_cast<char*>(varRecord)+stride);Write_Record(true);varRecord=next;"
                               : "m_stream->Write<Record>(varRecord,count);";
        excluded = dynamicSize ? "Write_Record(false);" : "Write_Record(true);";
    }
    SECTION("Asset marking")
    {
        codeTemplate = std::make_unique<ZoneMarkTemplate>();
        method = "MarkArray_Record";
        expected = dynamicSize ? "constautostride=offsetof(Record,values)+sizeof(var->values[0])*(varRecord->valueCount);"
                                 "Mark_Record();var=reinterpret_cast<Record*>(reinterpret_cast<char*>(var)+stride);"
                               : "Mark_Record();var++;";
        excluded = dynamicSize ? "var++;" : "constautostride=";
    }

    const auto context = OncePerAssetRenderingContext::BuildContext(&repository, entryInfo);
    std::ostringstream output;
    for (const auto& file : codeTemplate->GetFilesToRenderOncePerAsset(*context))
    {
        if (file.m_file_name.ends_with(".cpp"))
            codeTemplate->RenderOncePerAssetFile(output, file.m_tag, *context);
    }

    const auto source = output.str();
    const auto start = source.find("::" + method + "(");
    REQUIRE(start != std::string::npos);
    const auto end = source.find("\n}\n", start);
    REQUIRE(end != std::string::npos);
    auto body = source.substr(start, end - start);
    std::erase_if(body,
                  [](const unsigned char c)
                  {
                      return std::isspace(c);
                  });
    INFO(body);
    REQUIRE(body.find(expected) != std::string::npos);
    REQUIRE(body.find(excluded) == std::string::npos);
}
