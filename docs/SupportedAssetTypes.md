# Supported Asset Types

All asset types are supported to be loaded from other fastfiles in memory. The following section specify which assets
are supported to be dumped to disk (using `Unlinker`) and loaded from disk (using `Linker`):

- ❌ = Currently unsupported
- ✅ = Supported
- ⁉️ = Partial support

## IW3 (Call of Duty 4: Modern Warfare)

| Asset Type           | Dumping Support | Loading Support | Notes                                                                        |
|----------------------|-----------------|-----------------|------------------------------------------------------------------------------|
| PhysPreset           | ✅              | ✅              |                                                                              |
| XAnimParts           | ✅              | ✅              |                                                                              |
| XModel               | ✅              | ✅              | Model data can be exported to `XMODEL_EXPORT/XMODEL_BIN`, `OBJ`, `GLB/GLTF`. |
| Material             | ✅              | ✅              |                                                                              |
| MaterialTechniqueSet | ✅              | ✅              | For shaders: only dumps/loads shader bytecode.                               |
| GfxImage             | ✅              | ✅              |                                                                              |
| snd_alias_list_t     | ❌              | ❌              |                                                                              |
| SndCurve             | ✅              | ✅              |                                                                              |
| LoadedSound          | ✅              | ❌              |                                                                              |
| clipMap_t            | ❌              | ❌              |                                                                              |
| ComWorld             | ❌              | ❌              |                                                                              |
| GameWorldSp          | ❌              | ❌              |                                                                              |
| GameWorldMp          | ❌              | ❌              |                                                                              |
| MapEnts              | ✅              | ❌              |                                                                              |
| GfxWorld             | ❌              | ❌              |                                                                              |
| GfxLightDef          | ✅              | ✅              |                                                                              |
| Font_s               | ✅              | ✅              |                                                                              |
| MenuList             | ✅              | ✅              | The output is decompiled. The result will not be the same as the input.      |
| menuDef_t            | ✅              | ✅              | See menulist.                                                                |
| LocalizeEntry        | ✅              | ✅              |                                                                              |
| WeaponDef            | ✅              | ✅              |                                                                              |
| FxEffectDef          | ❌              | ❌              |                                                                              |
| FxImpactTable        | ❌              | ❌              |                                                                              |
| RawFile              | ✅              | ✅              |                                                                              |
| StringTable          | ✅              | ✅              |                                                                              |

## IW3 Xbox 360 (Call of Duty 4: Modern Warfare)

| Asset Type           | Dumping Support | Loading Support | Notes                                                                        |
|----------------------|-----------------|-----------------|------------------------------------------------------------------------------|
| PhysPreset           | ✅              | ✅              | IW3 PC-compatible raw files and GDT entries.                                  |
| XAnimParts           | ✅              | ✅              | IW3 PC-compatible version-17 files in `xanim/`.                               |
| XModel               | ✅              | ✅              | Model data can be exported to `XMODEL_EXPORT/XMODEL_BIN`, `OBJ`, `GLB/GLTF`. |
| Material             | ✅              | ✅              | IW3 material JSON, including water.                                          |
| MaterialPixelShader  | ✅              | ✅              | Native `.xpu` microcode.                                                     |
| MaterialTechniqueSet | ✅              | ✅              | Text techniques with Xbox extensions and all `.xvu` shader variants.        |
| GfxImage             | ✅              | ✅              | DDS/IWI, 2D and cube textures.                                               |
| snd_alias_list_t     | ⁉️              | ❌              | Extracts streamed `.xma` audio; alias definitions are not dumped.             |
| SndCurve             | ✅              | ✅              | IW3 PC-compatible `.vfcurve` files in `soundaliases/`.                         |
| LoadedSound          | ✅              | ❌              | Embedded XMA1 audio as RIFF `.xma` files.                                     |
| clipMap_t            | ❌              | ❌              |                                                                              |
| ComWorld             | ❌              | ❌              |                                                                              |
| GameWorldSp          | ❌              | ❌              |                                                                              |
| GameWorldMp          | ❌              | ❌              |                                                                              |
| MapEnts              | ✅              | ✅              | IW3 PC-compatible `.d3dbsp.ents` entity strings.                               |
| GfxWorld             | ❌              | ❌              |                                                                              |
| GfxLightDef          | ✅              | ✅              | IW3 PC-compatible attenuation files in `lights/`.                             |
| Font_s               | ❌              | ❌              |                                                                              |
| MenuList             | ❌              | ❌              |                                                                              |
| menuDef_t            | ❌              | ❌              |                                                                              |
| LocalizeEntry        | ✅              | ✅              |                                                                              |
| WeaponDef            | ✅              | ✅              | Raw weapon files and GDT entries, including accuracy graphs.                  |
| SndDriverGlobals     | ❌              | ❌              |                                                                              |
| FxEffectDef          | ❌              | ❌              |                                                                              |
| FxImpactTable        | ❌              | ❌              |                                                                              |
| RawFile              | ✅              | ✅              | Plain files, including binary data.                                          |
| StringTable          | ✅              | ✅              |                                                                              |

IW3 Xbox 360 image dumping looks for `highmip/<image name>.hi` beside the fastfile or under a supplied search path.
For streamed images, a valid file adds the highest mip level; otherwise, the resident image is dumped.

IW3 Xbox 360 sound dumping preserves native XMA audio in `sound/`. Embedded sounds use their asset name with an `.xma`
extension and retain stream formats, loops, duration and seek data in a reconstructed RIFF file.
Streamed sounds use `<alias>.<index>.xma`; packed audio is copied from the recorded offset and length in `packfileN.pak`.
The packfiles are found beside stock fastfiles or through `--search-path`. Loose XMA streams use the same search paths.
No Xbox 360 SDK is required for dumping. These files contain compressed XMA audio, rather than decoded PCM WAV.
For PCM playback, [vgmstream](https://github.com/vgmstream/vgmstream) supports the legacy XMA1 format:
`vgmstream-cli -i -o output.wav input.xma` decodes a single pass without repeating loops.
The SDK's `xma2encode /DecodeToPCM` can produce empty or corrupt audio from these legacy files.

IW3 Xbox 360 technique sets use `techsets/<name>.techset` and reference `techniques/<name>.tech`,
following the PC text syntax for technique slots, named shader bindings and vertex routing.
Xbox extensions preserve flags, all 15 vertex-shader variants, the precompiled index and the exact shader-argument layout.
See [IW3 Xbox 360 technique files](IW3XenonTechsets.md) for the format and its source-recovery limitations.
Shaders use `shader_bin/ps_<name>.xpu` and `shader_bin/vs_<name>.xvu`, the SDK microcode file formats.
The loader recreates the zero-filled runtime headers used by stock fastfiles; no shader sidecars or Xbox 360 SDK are required.
Use `--load` for base fastfiles when a map references shaders stored in those fastfiles.
Characters that cannot appear in Windows filenames are escaped as `~XX`; asset names inside text files retain their original spelling.

## IW4 (Call of Duty: Modern Warfare 2)

| Asset Type                | Dumping Support | Loading Support | Notes                                                                        |
|---------------------------|-----------------|-----------------|------------------------------------------------------------------------------|
| PhysPreset                | ✅              | ✅              |                                                                              |
| PhysCollmap               | ❌              | ❌              |                                                                              |
| XAnimParts                | ✅              | ✅              |                                                                              |
| XModel                    | ✅              | ✅              | Model data can be exported to `XMODEL_EXPORT/XMODEL_BIN`, `OBJ`, `GLB/GLTF`. |
| Material                  | ✅              | ✅              |                                                                              |
| MaterialPixelShader       | ✅              | ✅              | Only dumps/loads shader bytecode.                                            |
| MaterialVertexShader      | ✅              | ✅              | Only dumps/loads shader bytecode.                                            |
| MaterialVertexDeclaration | ✅              | ✅              |                                                                              |
| MaterialTechniqueSet      | ✅              | ✅              |                                                                              |
| GfxImage                  | ✅              | ❌              |                                                                              |
| snd_alias_list_t          | ❌              | ❌              |                                                                              |
| SndCurve                  | ✅              | ✅              |                                                                              |
| LoadedSound               | ✅              | ❌              |                                                                              |
| clipMap_t                 | ❌              | ❌              |                                                                              |
| ComWorld                  | ❌              | ❌              |                                                                              |
| GameWorldSp               | ❌              | ❌              |                                                                              |
| GameWorldMp               | ❌              | ❌              |                                                                              |
| MapEnts                   | ⁉️               | ❌              | Entity string can be exported. Binary data currently not.                    |
| FxWorld                   | ❌              | ❌              |                                                                              |
| GfxWorld                  | ❌              | ❌              |                                                                              |
| GfxLightDef               | ✅              | ✅              |                                                                              |
| Font_s                    | ✅              | ✅              |                                                                              |
| MenuList                  | ✅              | ✅              | The output is decompiled. The result will not be the same as the input.      |
| menuDef_t                 | ✅              | ✅              | See menulist.                                                                |
| LocalizeEntry             | ✅              | ✅              |                                                                              |
| WeaponCompleteDef         | ✅              | ✅              |                                                                              |
| FxEffectDef               | ❌              | ❌              |                                                                              |
| FxImpactTable             | ❌              | ❌              |                                                                              |
| RawFile                   | ✅              | ✅              |                                                                              |
| StringTable               | ✅              | ✅              |                                                                              |
| LeaderboardDef            | ✅              | ✅              |                                                                              |
| StructuredDataDefSet      | ✅              | ✅              | The format is custom due to lacking information about original format.       |
| TracerDef                 | ✅              | ✅              |                                                                              |
| VehicleDef                | ✅              | ✅              |                                                                              |
| AddonMapEnts              | ⁉️               | ❌              | MapEnts String can be exported. Binary data currently not.                   |

## IW5 (Call of Duty: Modern Warfare 3)

| Asset Type                | Dumping Support | Loading Support | Notes                                                                                                         |
|---------------------------|-----------------|-----------------|---------------------------------------------------------------------------------------------------------------|
| PhysPreset                | ✅              | ✅              |                                                                                                               |
| PhysCollmap               | ❌              | ❌              |                                                                                                               |
| XAnimParts                | ✅              | ✅              |                                                                                                               |
| XModelSurfs               | ❌              | ❌              |                                                                                                               |
| XModel                    | ✅              | ✅              | Model data can be exported to `XMODEL_EXPORT/XMODEL_BIN`, `OBJ`, `GLB/GLTF`.                                  |
| Material                  | ✅              | ✅              |                                                                                                               |
| MaterialPixelShader       | ❌              | ❌              |                                                                                                               |
| MaterialVertexShader      | ❌              | ❌              |                                                                                                               |
| MaterialVertexDeclaration | ❌              | ❌              |                                                                                                               |
| MaterialTechniqueSet      | ❌              | ❌              |                                                                                                               |
| GfxImage                  | ✅              | ✅              |                                                                                                               |
| snd_alias_list_t          | ❌              | ❌              |                                                                                                               |
| SndCurve                  | ✅              | ✅              |                                                                                                               |
| LoadedSound               | ✅              | ❌              |                                                                                                               |
| clipMap_t                 | ❌              | ❌              |                                                                                                               |
| ComWorld                  | ❌              | ❌              |                                                                                                               |
| GlassWorld                | ❌              | ❌              |                                                                                                               |
| PathData                  | ❌              | ❌              |                                                                                                               |
| VehicleTrack              | ❌              | ❌              |                                                                                                               |
| MapEnts                   | ⁉️               | ❌              | Entity string can be exported. Binary data currently not.                                                     |
| FxWorld                   | ❌              | ❌              |                                                                                                               |
| GfxWorld                  | ❌              | ❌              |                                                                                                               |
| GfxLightDef               | ✅              | ✅              |                                                                                                               |
| Font_s                    | ✅              | ✅              |                                                                                                               |
| MenuList                  | ✅              | ✅              | The output is decompiled. The result will not be the same as the input.                                       |
| menuDef_t                 | ✅              | ✅              | See menulist.                                                                                                 |
| LocalizeEntry             | ✅              | ✅              |                                                                                                               |
| WeaponAttachment          | ✅              | ✅              |                                                                                                               |
| WeaponCompleteDef         | ✅              | ✅              |                                                                                                               |
| FxEffectDef               | ❌              | ❌              |                                                                                                               |
| FxImpactTable             | ❌              | ❌              |                                                                                                               |
| SurfaceFxTable            | ❌              | ❌              |                                                                                                               |
| RawFile                   | ✅              | ✅              |                                                                                                               |
| ScriptFile                | ⁉️               | ⁉️               | Can only be dumped/loaded as binary. Editing is possible with [GSC-Tool](https://github.com/xensik/gsc-tool). |
| StringTable               | ✅              | ✅              |                                                                                                               |
| LeaderboardDef            | ✅              | ✅              |                                                                                                               |
| StructuredDataDefSet      | ❌              | ❌              |                                                                                                               |
| TracerDef                 | ✅              | ✅              |                                                                                                               |
| VehicleDef                | ✅              | ✅              |                                                                                                               |
| AddonMapEnts              | ⁉️               | ❌              | MapEnts String can be exported. Binary data currently not.                                                    |

## QOS (007: Quantum of Solace)

| Asset Type           | Dumping Support | Loading Support | Notes                                                                        |
|----------------------|-----------------|-----------------|------------------------------------------------------------------------------|
| PhysPreset           | ❌              | ❌              |                                                                              |
| PhysConstraints      | ❌              | ❌              |                                                                              |
| DestructibleDef      | ❌              | ❌              |                                                                              |
| XAnimParts           | ❌              | ❌              |                                                                              |
| XModel               | ✅              | ❌              | Model data can be exported to `XMODEL_EXPORT/XMODEL_BIN`, `OBJ`, `GLB/GLTF`. |
| Material             | ❌              | ❌              |                                                                              |
| MaterialTechniqueSet | ❌              | ❌              |                                                                              |
| GfxImage             | ✅              | ❌              |                                                                              |
| snd_alias_list_t     | ❌              | ❌              |                                                                              |
| SndCurve             | ❌              | ❌              |                                                                              |
| clipMap_t            | ❌              | ❌              |                                                                              |
| ComWorld             | ❌              | ❌              |                                                                              |
| GameWorldSp          | ❌              | ❌              |                                                                              |
| GameWorldMp          | ❌              | ❌              |                                                                              |
| MapEnts              | ✅              | ❌              |                                                                              |
| GfxWorld             | ❌              | ❌              |                                                                              |
| GfxLightDef          | ❌              | ❌              |                                                                              |
| Font_s               | ✅              | ❌              |                                                                              |
| MenuList             | ❌              | ❌              |                                                                              |
| menuDef_t            | ❌              | ❌              |                                                                              |
| LocalizeEntry        | ✅              | ✅              |                                                                              |
| WeaponDef            | ❌              | ❌              |                                                                              |
| SndDriverGlobals     | ❌              | ❌              |                                                                              |
| FxEffectDef          | ❌              | ❌              |                                                                              |
| FxImpactTable        | ❌              | ❌              |                                                                              |
| RawFile              | ✅              | ✅              |                                                                              |
| StringTable          | ✅              | ✅              |                                                                              |
| XmlTree              | ❌              | ❌              |                                                                              |
| SceneAnimation       | ❌              | ❌              |                                                                              |
| Cutscene             | ❌              | ❌              |                                                                              |
| CustomCamera         | ❌              | ❌              |                                                                              |

## T4 (Call of Duty: World at War)

| Asset Type           | Dumping Support | Loading Support | Notes                                                                        |
|----------------------|-----------------|-----------------|------------------------------------------------------------------------------|
| PhysPreset           | ✅              | ✅              |                                                                              |
| PhysConstraints      | ✅              | ❌              |                                                                              |
| DestructibleDef      | ❌              | ❌              |                                                                              |
| XAnimParts           | ✅              | ✅              |                                                                              |
| XModel               | ✅              | ✅              | Model data can be exported to `XMODEL_EXPORT/XMODEL_BIN`, `OBJ`, `GLB/GLTF`. |
| Material             | ✅              | ✅              |                                                                              |
| MaterialTechniqueSet | ❌              | ❌              |                                                                              |
| GfxImage             | ✅              | ✅              |                                                                              |
| snd_alias_list_t     | ❌              | ❌              |                                                                              |
| SndDriverGlobals     | ❌              | ❌              |                                                                              |
| LoadedSound          | ✅              | ❌              |                                                                              |
| clipMap_t            | ❌              | ❌              |                                                                              |
| ComWorld             | ❌              | ❌              |                                                                              |
| GameWorldSp          | ❌              | ❌              |                                                                              |
| GameWorldMp          | ❌              | ❌              |                                                                              |
| MapEnts              | ✅              | ✅              |                                                                              |
| GfxWorld             | ❌              | ❌              |                                                                              |
| GfxLightDef          | ❌              | ❌              |                                                                              |
| Font_s               | ✅              | ✅              |                                                                              |
| MenuList             | ✅              | ✅              | The output is decompiled. The result will not be the same as the input.      |
| menuDef_t            | ✅              | ✅              | See menulist.                                                                |
| LocalizeEntry        | ✅              | ✅              |                                                                              |
| WeaponDef            | ✅              | ✅              |                                                                              |
| FxEffectDef          | ❌              | ❌              |                                                                              |
| FxImpactTable        | ❌              | ❌              |                                                                              |
| RawFile              | ✅              | ✅              |                                                                              |
| StringTable          | ✅              | ✅              |                                                                              |
| PackIndex            | ❌              | ❌              |                                                                              |

## T5 (Call of Duty: Black Ops)

| Asset Type           | Dumping Support | Loading Support | Notes                                                                        |
|----------------------|-----------------|-----------------|------------------------------------------------------------------------------|
| PhysPreset           | ✅              | ✅              |                                                                              |
| PhysConstraints      | ✅              | ❌              |                                                                              |
| DestructibleDef      | ❌              | ❌              |                                                                              |
| XAnimParts           | ✅              | ✅              |                                                                              |
| XModel               | ✅              | ✅              | Model data can be exported to `XMODEL_EXPORT/XMODEL_BIN`, `OBJ`, `GLB/GLTF`. |
| Material             | ✅              | ✅              |                                                                              |
| MaterialTechniqueSet | ✅              | ❌              | For shaders: only dumps/loads shader bytecode.                               |
| GfxImage             | ✅              | ❌              | A few special image encodings are not yet supported.                         |
| SndBank              | ❌              | ❌              |                                                                              |
| SndPatch             | ❌              | ❌              |                                                                              |
| clipMap_t            | ❌              | ❌              |                                                                              |
| ComWorld             | ❌              | ❌              |                                                                              |
| GameWorldSp          | ❌              | ❌              |                                                                              |
| GameWorldMp          | ❌              | ❌              |                                                                              |
| MapEnts              | ✅              | ❌              |                                                                              |
| GfxWorld             | ❌              | ❌              |                                                                              |
| GfxLightDef          | ✅              | ✅              |                                                                              |
| Font_s               | ✅              | ✅              |                                                                              |
| MenuList             | ❌              | ❌              |                                                                              |
| menuDef_t            | ❌              | ❌              |                                                                              |
| LocalizeEntry        | ✅              | ✅              |                                                                              |
| WeaponVariantDef     | ✅              | ✅              |                                                                              |
| SndDriverGlobals     | ❌              | ❌              |                                                                              |
| FxEffectDef          | ❌              | ❌              |                                                                              |
| FxImpactTable        | ❌              | ❌              |                                                                              |
| RawFile              | ✅              | ✅              |                                                                              |
| StringTable          | ✅              | ✅              |                                                                              |
| PackIndex            | ❌              | ❌              |                                                                              |
| XGlobals             | ❌              | ❌              |                                                                              |
| ddlRoot_t            | ❌              | ❌              |                                                                              |
| Glasses              | ❌              | ❌              |                                                                              |
| EmblemSet            | ❌              | ❌              |                                                                              |

## T6 (Call of Duty: Black Ops II)

| Asset Type             | Dumping Support | Loading Support | Notes                                                                                                                                                          |
|------------------------|-----------------|-----------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------|
| PhysPreset             | ✅              | ✅              |                                                                                                                                                                |
| PhysConstraints        | ✅              | ✅              |                                                                                                                                                                |
| DestructibleDef        | ❌              | ❌              |                                                                                                                                                                |
| XAnimParts             | ✅              | ✅              |                                                                                                                                                                |
| XModel                 | ✅              | ✅              | Model data can be exported to `XMODEL_EXPORT/XMODEL_BIN`, `OBJ`, `GLB/GLTF`.                                                                                   |
| Material               | ✅              | ✅              | Dumping/Loading is currently possible for materials in their compiled form. There is currently no material pipeline.                                           |
| MaterialTechniqueSet   | ✅              | ✅              | For shaders: only dumps/loads shader bytecode.                                                                                                                 |
| GfxImage               | ✅              | ✅              | A few special image encodings are not yet supported.                                                                                                           |
| SndBank                | ✅              | ✅              | The current implementation is subject to change.                                                                                                               |
| SndPatch               | ❌              | ❌              |                                                                                                                                                                |
| clipMap_t              | ❌              | ❌              |                                                                                                                                                                |
| ComWorld               | ❌              | ❌              |                                                                                                                                                                |
| GameWorldSp            | ❌              | ❌              |                                                                                                                                                                |
| GameWorldMp            | ❌              | ❌              |                                                                                                                                                                |
| MapEnts                | ⁉️               | ❌              | Entity string can be exported. Binary data currently not.                                                                                                      |
| GfxWorld               | ❌              | ❌              |                                                                                                                                                                |
| GfxLightDef            | ✅              | ✅              |                                                                                                                                                                |
| Font_s                 | ✅              | ✅              |                                                                                                                                                                |
| FontIcon               | ✅              | ✅              |                                                                                                                                                                |
| MenuList               | ❌              | ❌              |                                                                                                                                                                |
| menuDef_t              | ❌              | ❌              |                                                                                                                                                                |
| LocalizeEntry          | ✅              | ✅              |                                                                                                                                                                |
| WeaponVariantDef       | ✅              | ✅              |                                                                                                                                                                |
| WeaponAttachment       | ✅              | ✅              |                                                                                                                                                                |
| WeaponAttachmentUnique | ✅              | ✅              |                                                                                                                                                                |
| WeaponCamo             | ✅              | ✅              |                                                                                                                                                                |
| SndDriverGlobals       | ✅              | ❌              |                                                                                                                                                                |
| FxEffectDef            | ❌              | ❌              |                                                                                                                                                                |
| FxImpactTable          | ❌              | ❌              |                                                                                                                                                                |
| RawFile                | ✅              | ✅              |                                                                                                                                                                |
| StringTable            | ✅              | ✅              |                                                                                                                                                                |
| LeaderboardDef         | ✅              | ✅              |                                                                                                                                                                |
| XGlobals               | ❌              | ❌              |                                                                                                                                                                |
| ddlRoot_t              | ❌              | ❌              |                                                                                                                                                                |
| Glasses                | ❌              | ❌              |                                                                                                                                                                |
| EmblemSet              | ❌              | ❌              |                                                                                                                                                                |
| ScriptParseTree        | ⁉️               | ⁉️               | Can only be dumped/loaded as binary. Editing is possible with [GSC-Tool](https://github.com/xensik/gsc-tool). Plutonium supports loading source files however. |
| KeyValuePairs          | ✅              | ✅              | Is dumped/loaded as part of the `.zone` file.                                                                                                                  |
| VehicleDef             | ✅              | ✅              |                                                                                                                                                                |
| MemoryBlock            | ❌              | ❌              |                                                                                                                                                                |
| AddonMapEnts           | ⁉️               | ❌              | MapEnts String can be exported. Binary data currently not.                                                                                                     |
| TracerDef              | ✅              | ✅              |                                                                                                                                                                |
| SkinnedVertsDef        | ❌              | ❌              |                                                                                                                                                                |
| Qdb                    | ⁉️               | ⁉️               | Dumping/Loading is implemented as rawfiles. Their use is currently unknown.                                                                                    |
| Slug                   | ⁉️               | ⁉️               | Dumping/Loading is implemented as rawfiles. Their use is currently unknown.                                                                                    |
| FootstepTableDef       | ❌              | ❌              |                                                                                                                                                                |
| FootstepFXTableDef     | ❌              | ❌              |                                                                                                                                                                |
| ZBarrierDef            | ✅              | ✅              |                                                                                                                                                                |
