#include "Game/IW2Xenon/ContentLoaderIW2Xenon.h"
#include "Game/IW2Xenon/ContentWriterIW2Xenon.h"
#include "Loading/Exception/InvalidXStringIndexException.h"
#include "Loading/LoadingFileStream.h"
#include "Zone/Stream/ZoneInputStream.h"
#include "Zone/Stream/ZoneOutputStream.h"

#include <catch2/catch_test_macros.hpp>
#include <cstring>
#include <sstream>

#ifdef ARCH_x86

namespace
{
    void Word(std::ostream& stream, const uint32_t value)
    {
        for (int shift = 24; shift >= 0; shift -= 8)
            stream.put(static_cast<char>(value >> shift));
    }

    std::string Fixture(const uint32_t localizeNameIndex = 3)
    {
        std::ostringstream stream(std::ios::binary);
        for (const auto value : {3u, UINT32_MAX, 2u, UINT32_MAX, 3u, UINT32_MAX})
            Word(stream, value);
        for (int i = 0; i < 3; ++i)
            Word(stream, UINT32_MAX);
        stream.write("sample.txt\0Hello\0KEY\0", 21);
        Word(stream, 0);
        Word(stream, UINT32_MAX);
        stream.write("tag\0", 4);
        Word(stream, IW2Xenon::ASSET_TYPE_RAWFILE);
        Word(stream, UINT32_MAX);
        Word(stream, IW2Xenon::ASSET_TYPE_LOCALIZE_ENTRY);
        Word(stream, UINT32_MAX);
        Word(stream, IW2Xenon::ASSET_TYPE_RAWFILE);
        // Alias the first header's pointer at virtual offset 40.
        Word(stream, 0x40000029);
        Word(stream, 1);
        Word(stream, 3);
        Word(stream, UINT32_MAX);
        stream.write("abc\0", 4);
        Word(stream, 2);
        Word(stream, localizeNameIndex);
        return stream.str();
    }

    struct Blocks
    {
        XBlock temp{"temp", 0, XBlockType::BLOCK_TYPE_NORMAL};
        XBlock virt{"virtual", 1, XBlockType::BLOCK_TYPE_NORMAL};
        XBlock phys{"physical", 2, XBlockType::BLOCK_TYPE_NORMAL};
        std::vector<XBlock*> all{&temp, &virt, &phys};
    };

    void Read(Zone& zone, const std::string& payload)
    {
        std::vector<XBlock*> blocks;
        for (unsigned i = 0; i < 3; ++i)
        {
            auto block = std::make_unique<XBlock>("fixture", i, XBlockType::BLOCK_TYPE_NORMAL);
            block->Alloc(4096);
            blocks.push_back(block.get());
            zone.Memory().AddBlock(std::move(block));
        }
        std::istringstream bytes(payload, std::ios::binary);
        LoadingFileStream input(bytes);
        auto stream = ZoneInputStream::Create(32, 2, blocks, 1, input, zone.Memory(), std::nullopt);
        IW2Xenon::ContentLoader(zone, *stream).Load();
        REQUIRE(input.Pos() == payload.size());
    }
} // namespace

TEST_CASE("IW2 indexed strings distinguish raw strings from script strings and asset aliases", "[iw2xenon][system]")
{
    Zone source("indexed", 0, GameId::IW2Xenon, GamePlatform::XBOX);
    Read(source, Fixture());
    REQUIRE(source.m_pools.GetTotalAssetCount() == 2);
    REQUIRE(source.m_script_strings.Count() == 2);
    REQUIRE(source.m_script_strings.CValue(0) == nullptr);
    REQUIRE(std::string(source.m_script_strings.CValue(1)) == "tag");
    auto* raw = source.m_pools.GetAsset<IW2Xenon::AssetRawFile>("sample.txt")->Asset();
    REQUIRE(raw->len == 3);
    REQUIRE(std::string(raw->buffer, raw->len) == "abc");
    auto* localize = source.m_pools.GetAsset<IW2Xenon::AssetLocalize>("KEY")->Asset();
    REQUIRE(std::string(localize->value) == "Hello");

    Blocks blocks;
    InMemoryZoneData data, delay;
    auto output = ZoneOutputStream::Create(32, 2, blocks.all, 1, data, delay, GameEndianness::BE);
    IW2Xenon::ContentWriter(source).WriteContent(*output);
    std::string payload;
    for (const auto& buffer : data.m_buffers)
        payload.append(buffer.m_data.get(), buffer.m_size);
    Zone target("rewritten", 0, GameId::IW2Xenon, GamePlatform::XBOX);
    Read(target, payload);
    REQUIRE(target.m_pools.GetTotalAssetCount() == 2);
    REQUIRE(std::string(target.m_pools.GetAsset<IW2Xenon::AssetLocalize>("KEY")->Asset()->value) == "Hello");
    REQUIRE(std::string(target.m_pools.GetAsset<IW2Xenon::AssetRawFile>("sample.txt")->Asset()->buffer, 3) == "abc");
}

TEST_CASE("IW2 rejects an indexed string outside its raw string table", "[iw2xenon][system]")
{
    Zone zone("bad_index", 0, GameId::IW2Xenon, GamePlatform::XBOX);
    REQUIRE_THROWS_AS(Read(zone, Fixture(4)), InvalidXStringIndexException);
}

TEST_CASE("IW2 material arguments retain scalar unions and shared vertex declarations", "[iw2xenon][system][material]")
{
    using namespace IW2Xenon;
    Zone source("material", 0, GameId::IW2Xenon, GamePlatform::XBOX);
    auto& memory = source.Memory();
    auto* material = memory.Alloc<Material>();
    material->info.name = "material";
    material->techniqueSet = memory.Alloc<MaterialTechniqueSet>();
    material->techniqueSet->name = "techset";
    auto* technique = static_cast<MaterialTechnique*>(memory.AllocRaw(offsetof(MaterialTechnique, passArray) + 2 * sizeof(MaterialPassDx9)));
    material->techniqueSet->techniques[0] = technique;
    technique->name = "technique";
    technique->passCount = 2;
    auto* decl = memory.Alloc<MaterialVertexDeclaration>();
    auto* args = memory.Alloc<MaterialShaderArgument>(4);
    for (unsigned short i = 0; i < 4; ++i)
        args[i].type = i;
    args[0].u.literalConst = memory.Alloc<float>(4);
    args[0].u.literalConst[0] = 0.5f;
    args[1].u.codeConst = {210, 4, 2};
    args[2].u.name = "CONST_COLOR";
    args[3].u.codeSampler = 4;
    for (int i = 0; i < 2; ++i)
    {
        technique->passArray[i].vertexDecl = decl;
        technique->passArray[i].vertexArgCount = 4;
        technique->passArray[i].vertexArgs = args;
    }
    source.m_pools.AddAsset(ASSET_TYPE_MATERIAL, "material", material, {}, {}, {});

    Blocks blocks;
    InMemoryZoneData data, delay;
    auto output = ZoneOutputStream::Create(32, 2, blocks.all, 1, data, delay, GameEndianness::BE);
    ContentWriter(source).WriteContent(*output);
    std::string payload;
    for (const auto& buffer : data.m_buffers)
        payload.append(buffer.m_data.get(), buffer.m_size);
    Zone target("material", 0, GameId::IW2Xenon, GamePlatform::XBOX);
    Read(target, payload);
    const auto* loaded = target.m_pools.GetAsset<AssetMaterial>("material")->Asset()->techniqueSet->techniques[0];
    REQUIRE(loaded->passCount == 2);
    REQUIRE(loaded->passArray[0].vertexDecl == loaded->passArray[1].vertexDecl);
    REQUIRE(loaded->passArray[0].vertexArgs == loaded->passArray[1].vertexArgs);
    const auto* loadedArgs = loaded->passArray[0].vertexArgs;
    REQUIRE(loadedArgs[0].u.literalConst[0] == 0.5f);
    REQUIRE(loadedArgs[1].u.codeConst.index == 210);
    REQUIRE(loadedArgs[1].u.codeConst.firstRow == 4);
    REQUIRE(loadedArgs[1].u.codeConst.rowCount == 2);
    REQUIRE(std::string(loadedArgs[2].u.name) == "CONST_COLOR");
    REQUIRE(loadedArgs[3].u.codeSampler == 4);
}

TEST_CASE("IW2 fonts can alias material pointer slots in earlier temp data", "[iw2xenon][system][material]")
{
    using namespace IW2Xenon;
    std::ostringstream bytes(std::ios::binary);
    for (const auto value : {3u, UINT32_MAX, 0u, 0u, 2u, UINT32_MAX})
        Word(bytes, value);
    for (int i = 0; i < 3; ++i)
        Word(bytes, UINT32_MAX);
    bytes.write("font_a\0font_b\0material\0", 23);
    for (int i = 0; i < 2; ++i)
    {
        Word(bytes, ASSET_TYPE_FONT);
        Word(bytes, UINT32_MAX);
    }
    for (const auto value : {1u, 16u, 0u, UINT32_MAX, 0u})
        Word(bytes, value);
    Word(bytes, 3);
    const char materialRemainder[40]{};
    bytes.write(materialRemainder, sizeof(materialRemainder));
    // The first font's material pointer is at temp offset 24 + 12.
    for (const auto value : {2u, 16u, 0u, 37u, 0u})
        Word(bytes, value);

    Zone target("fonts", 0, GameId::IW2Xenon, GamePlatform::XBOX);
    Read(target, bytes.str());
    const auto* a = target.m_pools.GetAsset<AssetFont>("font_a")->Asset();
    const auto* b = target.m_pools.GetAsset<AssetFont>("font_b")->Asset();
    REQUIRE(a->material == b->material);
    REQUIRE(std::string(b->material->info.name) == "material");
}

TEST_CASE("Recursive IW2 world arrays use the parent child count", "[iw2xenon][system][gfxworld]")
{
    using namespace IW2Xenon;
    Zone source("world", 0, GameId::IW2Xenon, GamePlatform::XBOX);
    auto& memory = source.Memory();
    auto* world = memory.Alloc<GfxWorld>();
    world->name = "world";
    world->baseName = "base";
    world->cellCount = 1;
    world->cells = memory.Alloc<GfxCell>();
    auto* tree = memory.Alloc<GfxAabbTree>();
    world->cells->aabbTree = tree;
    tree->childrenCount = 2;
    tree->children = memory.Alloc<GfxAabbTree>(2);
    tree->children[1].staticModelCount = 1;
    tree->children[1].staticModelIndices = memory.Alloc<int>();
    tree->children[1].staticModelIndices[0] = 7;
    source.m_pools.AddAsset(ASSET_TYPE_GFXWORLD, "world", world, {}, {}, {});

    Blocks blocks;
    InMemoryZoneData data, delay;
    auto output = ZoneOutputStream::Create(32, 2, blocks.all, 1, data, delay, GameEndianness::BE);
    ContentWriter(source).WriteContent(*output);
    std::string payload;
    for (const auto& buffer : data.m_buffers)
        payload.append(buffer.m_data.get(), buffer.m_size);
    Zone target("world", 0, GameId::IW2Xenon, GamePlatform::XBOX);
    Read(target, payload);
    const auto* actual = target.m_pools.GetAsset<AssetGfxWorld>("world")->Asset()->cells->aabbTree;
    REQUIRE(actual->childrenCount == 2);
    REQUIRE(actual->children[0].childrenCount == 0);
    REQUIRE(actual->children[1].staticModelCount == 1);
    REQUIRE(actual->children[1].staticModelIndices[0] == 7);
}

TEST_CASE("IW2 vertex arrays retain each record's variable blend data", "[iw2xenon][system][xmodel]")
{
    using namespace IW2Xenon;
    Zone source("model", 0, GameId::IW2Xenon, GamePlatform::XBOX);
    auto& memory = source.Memory();
    auto* model = memory.Alloc<XModel>();
    model->name = "model";
    model->numLods = 1;
    auto& lod = model->lodInfo[0];
    lod.filename = "model_lod0";
    lod.numsurfs = 1;
    lod.surfs = memory.Alloc<XModelSurfs>();
    lod.surfs->surfs = memory.Alloc<XSurface*>();
    auto* surface = memory.Alloc<XSurface>();
    lod.surfs->surfs[0] = surface;
    surface->vertCount = 3;
    constexpr size_t dataSize = 3 * offsetof(XVertexBuffer, w) + 4 * sizeof(XBlendInfo);
    auto* vertices = static_cast<unsigned char*>(memory.AllocRaw(dataSize));
    surface->verts = reinterpret_cast<XVertexBuffer*>(vertices);
    size_t offset = 0;
    for (const unsigned char blendCount : {0, 3, 1})
    {
        const auto stride = offsetof(XVertexBuffer, w) + blendCount * sizeof(XBlendInfo);
        std::memset(vertices + offset, 0x20 + blendCount, stride);
        reinterpret_cast<XVertexBuffer*>(vertices + offset)->blendCount = blendCount;
        offset += stride;
    }
    source.m_pools.AddAsset(ASSET_TYPE_XMODEL, model->name, model, {}, {}, {});

    Blocks blocks;
    InMemoryZoneData data, delay;
    auto output = ZoneOutputStream::Create(32, 2, blocks.all, 1, data, delay, GameEndianness::BE);
    ContentWriter(source).WriteContent(*output);
    std::string payload;
    for (const auto& buffer : data.m_buffers)
        payload.append(buffer.m_data.get(), buffer.m_size);
    Zone target("model", 0, GameId::IW2Xenon, GamePlatform::XBOX);
    Read(target, payload);
    const auto* actual = target.m_pools.GetAsset<AssetXModel>("model")->Asset()->lodInfo[0].surfs->surfs[0];
    REQUIRE(actual->vertCount == surface->vertCount);
    REQUIRE(std::memcmp(actual->verts, vertices, dataSize) == 0);
}

#endif
