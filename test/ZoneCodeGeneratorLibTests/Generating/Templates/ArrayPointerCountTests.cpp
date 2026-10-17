#include "Domain/Definition/PointerDeclarationModifier.h"
#include "Domain/Evaluation/OperandDynamic.h"
#include "Generating/Templates/ZoneMarkTemplate.h"
#include "Generating/Templates/ZoneWriteTemplate.h"
#include "Parsing/PostProcessing/CreateMemberInformationPostProcessor.h"
#include "Parsing/PostProcessing/CreateStructureInformationPostProcessor.h"
#include "Persistence/InMemory/InMemoryRepository.h"

#include <catch2/catch_test_macros.hpp>
#include <cctype>
#include <sstream>

TEST_CASE("Recursive array counts use the parent structure", "[generating][array]")
{
    InMemoryRepository repository;
    repository.SetGame("Test");
    repository.Add(std::make_unique<FastFileBlock>("TEMP", 0, FastFileBlockType::TEMP, true));
    repository.Add(std::make_unique<FastFileBlock>("NORMAL", 1, FastFileBlockType::NORMAL, true));

    auto node = std::make_unique<StructDefinition>("", "Node", 4);
    node->m_members.emplace_back(std::make_shared<Variable>("childCount", std::make_unique<TypeDeclaration>(BaseTypeDefinition::UNSIGNED_INT)));
    node->m_members.emplace_back(std::make_shared<Variable>("name", std::make_unique<TypeDeclaration>(BaseTypeDefinition::UNSIGNED_SHORT)));
    auto children = std::make_unique<TypeDeclaration>(node.get());
    auto pointer = std::make_unique<PointerDeclarationModifier>();
    auto* childPointer = pointer.get();
    children->m_declaration_modifiers.emplace_back(std::move(pointer));
    node->m_members.emplace_back(std::make_shared<Variable>("children", std::move(children)));
    const auto* nodeDefinition = node.get();
    repository.Add(std::move(node));

    auto entry = std::make_unique<StructDefinition>("", "Entry", 4);
    entry->m_members.emplace_back(std::make_shared<Variable>("root", std::make_unique<TypeDeclaration>(nodeDefinition)));
    const auto* entryDefinition = entry.get();
    repository.Add(std::move(entry));

    REQUIRE(CreateStructureInformationPostProcessor().PostProcess(&repository));
    REQUIRE(CreateMemberInformationPostProcessor().PostProcess(&repository));
    auto* entryInfo = repository.GetInformationFor(entryDefinition);
    auto* nodeInfo = repository.GetInformationFor(nodeDefinition);
    entryInfo->m_asset_name = "ASSET_TYPE_ENTRY";
    entryInfo->m_requires_marking = true;
    nodeInfo->m_requires_marking = true;
    nodeInfo->m_ordered_members[1]->m_is_script_string = true;
    std::vector<MemberAccessor> members;
    members.emplace_back(nodeInfo->m_ordered_members[0].get());
    childPointer->m_count_evaluation = std::make_unique<OperandDynamic>(nodeInfo, std::move(members));

    std::unique_ptr<ICodeTemplate> codeTemplate;
    std::string call;
    SECTION("Asset marking")
    {
        codeTemplate = std::make_unique<ZoneMarkTemplate>();
        call = "MarkArray_Node(arrayPointerCount);";
    }
    SECTION("Zone writing")
    {
        codeTemplate = std::make_unique<ZoneWriteTemplate>();
        call = "WriteArray_Node(true,arrayPointerCount);";
    }

    const auto context = OncePerAssetRenderingContext::BuildContext(&repository, entryInfo);
    std::ostringstream output;
    for (const auto& file : codeTemplate->GetFilesToRenderOncePerAsset(*context))
    {
        if (file.m_file_name.ends_with(".cpp"))
            codeTemplate->RenderOncePerAssetFile(output, file.m_tag, *context);
    }

    auto source = output.str();
    std::erase_if(source,
                  [](const unsigned char c)
                  {
                      return std::isspace(c);
                  });
    INFO(source);
    REQUIRE(source.find("constautoarrayPointerCount=static_cast<size_t>(varNode->childCount);varNode=varNode->children;" + call) != std::string::npos);
}
