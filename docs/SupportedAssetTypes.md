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
| Material             | ✅              | ✅              | IW3 PC-compatible material JSON, including water.                            |
| MaterialPixelShader  | ✅              | ✅              | Native `.xpu` microcode.                                                     |
| MaterialTechniqueSet | ❌              | ❌              |                                                                              |
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
| Font_s               | ✅              | ✅              | IW3 PC-compatible compiled font JSON; TrueType compilation is unsupported.    |
| MenuList             | ✅              | ✅              | IW3 PC-compatible `.menu` files and menu lists.                              |
| menuDef_t            | ✅              | ✅              | See menulist; expressions and scripts are decompiled.                        |
| LocalizeEntry        | ✅              | ✅              |                                                                              |
| WeaponDef            | ✅              | ✅              | Raw weapon files and GDT entries, including accuracy graphs.                  |
| SndDriverGlobals     | ❌              | ❌              |                                                                              |
| FxEffectDef          | ❌              | ❌              |                                                                              |
| FxImpactTable        | ❌              | ❌              |                                                                              |
| RawFile              | ✅              | ✅              | Plain files, including binary data.                                          |
| StringTable          | ✅              | ✅              |                                                                              |

## IW3 Xbox 360 build 253 (Call of Duty 4: Modern Warfare pre-alpha)

| Asset Type           | Dumping Support | Loading Support | Notes |
|----------------------|-----------------|-----------------|-------|
| PhysPreset           | ❌               | ❌               |       |
| XAnimParts           | ✅               | ✅               | IW3 Mod Tools v17. |
| XModel               | ❌               | ❌               |       |
| Material             | ❌               | ❌               |       |
| MaterialPixelShader  | ❌               | ❌               |       |
| MaterialTechniqueSet | ❌               | ❌               |       |
| GfxImage             | ✅               | ✅               | 2D and cube textures. |
| snd_alias_list_t     | ❌               | ❌               |       |
| SndCurve             | ✅               | ✅               |       |
| clipMap_t            | ❌               | ❌               |       |
| ComWorld             | ❌               | ❌               |       |
| GameWorldSp          | ❌               | ❌               |       |
| GameWorldMp          | ❌               | ❌               |       |
| MapEnts              | ✅               | ✅               |       |
| GfxWorld             | ❌               | ❌               |       |
| GfxLightDef          | ❌               | ❌               |       |
| Font_s               | ❌               | ❌               |       |
| MenuList             | ❌               | ❌               |       |
| menuDef_t            | ❌               | ❌               |       |
| LocalizeEntry        | ✅               | ✅               |       |
| WeaponDef            | ❌               | ❌               |       |
| SndDriverGlobals     | ❌               | ❌               |       |
| FxEffectDef          | ❌               | ❌               |       |
| FxImpactTable        | ❌               | ❌               |       |
| RawFile              | ✅               | ✅               |       |

## IW3 Xbox 360 build 270 (Call of Duty 4: Modern Warfare pre-alpha)

| Asset Type           | Dumping Support | Loading Support | Notes |
|----------------------|-----------------|-----------------|-------|
| PhysPreset           | ❌               | ❌               |       |
| XAnimParts           | ✅               | ✅               | IW3 Mod Tools v17. |
| XModel               | ❌               | ❌               |       |
| Material             | ❌               | ❌               |       |
| MaterialPixelShader  | ❌               | ❌               |       |
| MaterialTechniqueSet | ❌               | ❌               |       |
| GfxImage             | ✅               | ✅               | 2D and cube textures. |
| snd_alias_list_t     | ❌               | ❌               |       |
| SndCurve             | ✅               | ✅               |       |
| clipMap_t            | ❌               | ❌               |       |
| ComWorld             | ❌               | ❌               |       |
| GameWorldSp          | ❌               | ❌               |       |
| GameWorldMp          | ❌               | ❌               |       |
| MapEnts              | ✅               | ✅               |       |
| GfxWorld             | ❌               | ❌               |       |
| GfxLightDef          | ❌               | ❌               |       |
| Font_s               | ❌               | ❌               |       |
| MenuList             | ❌               | ❌               |       |
| menuDef_t            | ❌               | ❌               |       |
| LocalizeEntry        | ✅               | ✅               |       |
| WeaponDef            | ❌               | ❌               |       |
| SndDriverGlobals     | ❌               | ❌               |       |
| FxEffectDef          | ❌               | ❌               |       |
| FxImpactTable        | ❌               | ❌               |       |
| RawFile              | ✅               | ✅               |       |
| StringTable          | ✅               | ✅               |       |

## IW3 Xbox 360 build 290 (Call of Duty 4: Modern Warfare pre-alpha)

| Asset Type           | Dumping Support | Loading Support | Notes |
|----------------------|-----------------|-----------------|-------|
| PhysPreset           | ❌               | ❌               |       |
| XAnimParts           | ✅               | ✅               | IW3 Mod Tools v17. Compact quaternions used when required by 16-bit pool limits. |
| XModel               | ❌               | ❌               |       |
| Material             | ❌               | ❌               |       |
| MaterialPixelShader  | ❌               | ❌               |       |
| MaterialTechniqueSet | ❌               | ❌               |       |
| GfxImage             | ✅               | ✅               | 2D and cube textures. |
| snd_alias_list_t     | ❌               | ❌               |       |
| SndCurve             | ✅               | ✅               |       |
| clipMap_t            | ❌               | ❌               |       |
| ComWorld             | ❌               | ❌               |       |
| GameWorldSp          | ❌               | ❌               |       |
| GameWorldMp          | ❌               | ❌               |       |
| MapEnts              | ✅               | ✅               |       |
| GfxWorld             | ❌               | ❌               |       |
| GfxLightDef          | ❌               | ❌               |       |
| Font_s               | ❌               | ❌               |       |
| MenuList             | ❌               | ❌               |       |
| menuDef_t            | ❌               | ❌               |       |
| LocalizeEntry        | ✅               | ✅               |       |
| WeaponDef            | ❌               | ❌               |       |
| SndDriverGlobals     | ❌               | ❌               |       |
| FxEffectDef          | ❌               | ❌               |       |
| FxImpactTable        | ❌               | ❌               |       |
| RawFile              | ✅               | ✅               |       |
| StringTable          | ✅               | ✅               |       |

## IW3 Xbox 360 build 328 (Call of Duty 4: Modern Warfare pre-alpha)

| Asset Type           | Dumping Support | Loading Support | Notes |
|----------------------|-----------------|-----------------|-------|
| PhysPreset           | ❌               | ❌               |       |
| XAnimParts           | ✅               | ✅               | IW3 Mod Tools v17. |
| XModel               | ❌               | ❌               |       |
| Material             | ❌               | ❌               |       |
| MaterialPixelShader  | ❌               | ❌               |       |
| MaterialTechniqueSet | ❌               | ❌               |       |
| GfxImage             | ✅               | ✅               | 2D and cube textures. |
| snd_alias_list_t     | ❌               | ❌               |       |
| SndCurve             | ✅               | ✅               |       |
| clipMap_t            | ❌               | ❌               |       |
| ComWorld             | ❌               | ❌               |       |
| GameWorldSp          | ❌               | ❌               |       |
| GameWorldMp          | ❌               | ❌               |       |
| MapEnts              | ✅               | ✅               |       |
| GfxWorld             | ❌               | ❌               |       |
| GfxLightDef          | ❌               | ❌               |       |
| Font_s               | ❌               | ❌               |       |
| MenuList             | ❌               | ❌               |       |
| menuDef_t            | ❌               | ❌               |       |
| LocalizeEntry        | ✅               | ✅               |       |
| WeaponDef            | ❌               | ❌               |       |
| SndDriverGlobals     | ❌               | ❌               |       |
| FxEffectDef          | ❌               | ❌               |       |
| FxImpactTable        | ❌               | ❌               |       |
| RawFile              | ✅               | ✅               |       |
| StringTable          | ✅               | ✅               |       |

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
| MapEnts                   | ⁉️              | ⁉️              | Entity text only; `--load` preserves existing triggers and stages.           |
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

## IW4 Xbox 360 (Call of Duty: Modern Warfare 2)

| Asset Type                | Dumping Support | Loading Support | Notes                                                                        |
|---------------------------|-----------------|-----------------|------------------------------------------------------------------------------|
| PhysPreset                | ✅               | ✅               |                                                                              |
| PhysCollmap               | ❌               | ❌               |                                                                              |
| XAnimParts                | ✅               | ✅               |                                                                              |
| XModel                    | ✅               | ✅               |                                                                              |
| Material                  | ✅               | ✅               |                                                                              |
| MaterialPixelShader       | ❌               | ❌               |                                                                              |
| MaterialTechniqueSet      | ❌               | ❌               |                                                                              |
| GfxImage                  | ✅              | ✅              | Loading requires `>imagefile,0` for appended fastfile streams or `>imagefile,1` through `>imagefile,20` for external imagefiles.                 |
| snd_alias_list_t          | ❌               | ❌               |                                                                              |
| SndCurve                  | ✅               | ✅               |                                                                              |
| LoadedSound               | ❌               | ❌               |                                                                              |
| clipMap_t                 | ❌               | ❌               |                                                                              |
| ComWorld                  | ❌               | ❌               |                                                                              |
| GameWorldSp               | ❌               | ❌               |                                                                              |
| GameWorldMp               | ❌               | ❌               |                                                                              |
| MapEnts                   | ⁉️              | ⁉️              | Entity text only; `--load` preserves existing triggers and stages.           |
| FxWorld                   | ❌               | ❌               |                                                                              |
| GfxWorld                  | ❌               | ❌               |                                                                              |
| GfxLightDef               | ✅               | ✅               |                                                                              |
| Font_s                    | ❌               | ❌               |                                                                              |
| MenuList                  | ❌               | ❌               |                                                                              |
| menuDef_t                 | ❌               | ❌               |                                                                              |
| LocalizeEntry             | ✅               | ✅               |                                                                              |
| WeaponCompleteDef         | ❌               | ❌               |                                                                              |
| SndDriverGlobals          | ❌               | ❌               |                                                                              |
| FxEffectDef               | ❌               | ❌               |                                                                              |
| FxImpactTable             | ❌               | ❌               |                                                                              |
| RawFile                   | ✅               | ✅               |                                                                              |
| StringTable               | ✅               | ✅               |                                                                              |
| LeaderboardDef            | ❌               | ❌               |                                                                              |
| StructuredDataDefSet      | ❌               | ❌               |                                                                              |
| TracerDef                 | ❌               | ❌               |                                                                              |
| VehicleDef                | ❌               | ❌               |                                                                              |
| AddonMapEnts              | ⁉️              | ❌               | Entity string can be exported. Binary data currently not.                    |

## IW4 Xbox 360 alpha build 482 (Call of Duty: Modern Warfare 2)

| Asset Type                | Dumping Support | Loading Support | Notes |
|---------------------------|-----------------|-----------------|-------|
| PhysPreset                | ❌               | ❌               |       |
| PhysCollmap               | ❌               | ❌               |       |
| XAnimParts                | ❌               | ❌               |       |
| XModel                    | ❌               | ❌               |       |
| Material                  | ❌               | ❌               |       |
| MaterialPixelShader       | ❌               | ❌               |       |
| MaterialTechniqueSet      | ❌               | ❌               |       |
| GfxImage                  | ❌               | ❌               |       |
| snd_alias_list_t          | ❌               | ❌               |       |
| SndCurve                  | ❌               | ❌               |       |
| LoadedSound               | ❌               | ❌               |       |
| clipMap_t                 | ❌               | ❌               |       |
| ComWorld                  | ❌               | ❌               |       |
| GameWorldSp               | ❌               | ❌               |       |
| GameWorldMp               | ❌               | ❌               |       |
| MapEnts                   | ❌               | ❌               |       |
| FxWorld                   | ❌               | ❌               |       |
| GfxWorld                  | ❌               | ❌               |       |
| GfxLightDef               | ❌               | ❌               |       |
| Font_s                    | ❌               | ❌               |       |
| MenuList                  | ❌               | ❌               |       |
| menuDef_t                 | ❌               | ❌               |       |
| LocalizeEntry             | ❌               | ❌               |       |
| WeaponCompleteDef         | ❌               | ❌               |       |
| SndDriverGlobals          | ❌               | ❌               |       |
| FxEffectDef               | ❌               | ❌               |       |
| FxImpactTable             | ❌               | ❌               |       |
| RawFile                   | ❌               | ❌               |       |
| StringTable               | ❌               | ❌               |       |
| LeaderboardDef            | ❌               | ❌               |       |
| StructuredDataDefSet      | ❌               | ❌               |       |
| TracerDef                 | ❌               | ❌               |       |
| VehicleDef                | ❌               | ❌               |       |
| AddonMapEnts              | ❌               | ❌               |       |

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

## NX1 (Call of Duty: Future Warfare, Xbox 360)

| Asset Type               | Dumping Support  | Loading Support  | Notes                                                                                                                        |
|--------------------------|------------------|------------------|------------------------------------------------------------------------------------------------------------------------------|
| PhysPreset               | ❌                | ❌                |                                                                                                                              |
| PhysCollmap              | ❌                | ❌                |                                                                                                                              |
| XAnimParts               | ❌                | ❌                |                                                                                                                              |
| XModel                   | ❌                | ❌                |                                                                                                                              |
| Material                 | ❌                | ❌                |                                                                                                                              |
| MaterialPixelShader      | ❌                | ❌                |                                                                                                                              |
| MaterialTechniqueSet     | ❌                | ❌                |                                                                                                                              |
| GfxImage                 | ✅                | ✅                | Loading requires `>imagefile,0` for appended fastfile streams or `>imagefile,1` through `>imagefile,20` for external imagefiles. |
| snd_alias_list_t         | ❌                | ❌                |                                                                                                                              |
| SndVfCurve               | ✅                | ✅                |                                                                                                                              |
| LoadedSound              | ❌                | ❌                |                                                                                                                              |
| clipMap_t                | ❌                | ❌                |                                                                                                                              |
| ComWorld                 | ❌                | ❌                |                                                                                                                              |
| GameWorldSp              | ❌                | ❌                |                                                                                                                              |
| GameWorldMp              | ❌                | ❌                |                                                                                                                              |
| MapEnts                  | ⁉️               | ⁉️               | Entity text only; loading preserves compiled data from a preloaded zone.                                                     |
| FxWorld                  | ❌                | ❌                |                                                                                                                              |
| GfxWorld                 | ❌                | ❌                |                                                                                                                              |
| GfxLightDef              | ❌                | ❌                |                                                                                                                              |
| Font_s                   | ❌                | ❌                |                                                                                                                              |
| MenuList                 | ❌                | ❌                |                                                                                                                              |
| menuDef_t                | ❌                | ❌                |                                                                                                                              |
| LocalizeEntry            | ✅                | ✅                |                                                                                                                              |
| WeaponCompleteDef        | ❌                | ❌                |                                                                                                                              |
| SndDriverGlobals         | ❌                | ❌                |                                                                                                                              |
| FxEffectDef              | ❌                | ❌                |                                                                                                                              |
| FxImpactTable            | ❌                | ❌                |                                                                                                                              |
| SurfaceFxTable           | ❌                | ❌                |                                                                                                                              |
| RawFile                  | ✅                | ✅                |                                                                                                                              |
| StringTable              | ✅                | ✅                |                                                                                                                              |
| LeaderboardDef           | ❌                | ❌                |                                                                                                                              |
| StructuredDataDefSet     | ❌                | ❌                |                                                                                                                              |
| TracerDef                | ❌                | ❌                |                                                                                                                              |
| LaserDef                 | ❌                | ❌                |                                                                                                                              |
| VehicleDef               | ❌                | ❌                |                                                                                                                              |
| AddonMapEnts             | ⁉️               | ❌                | Entity text only.                                                                                                            |
| ShellShockParams         | ❌                | ❌                |                                                                                                                              |
| AudioVolumeSetting       | ❌                | ❌                |                                                                                                                              |
| RadVerbPreset            | ❌                | ❌                |                                                                                                                              |
| FogDef                   | ❌                | ❌                |                                                                                                                              |

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

## QoS Xbox 360 (007: Quantum of Solace)

| Asset Type           | Dumping Support | Loading Support | Notes |
|----------------------|-----------------|-----------------|-------|
| PhysPreset           | ❌               | ❌               |  |
| PhysConstraints      | ❌               | ❌               |  |
| DestructibleDef      | ❌               | ❌               |  |
| XAnimParts           | ❌               | ❌               |  |
| XModel               | ❌               | ❌               |  |
| Material             | ❌               | ❌               |  |
| MaterialPixelShader  | ❌               | ❌               |  |
| MaterialTechniqueSet | ❌               | ❌               |  |
| GfxImage             | ✅               | ✅               | 2D and cube images. Streamed high mips are read from `highmip`. |
| snd_alias_list_t     | ❌               | ❌               |  |
| SndCurve             | ✅               | ✅               |  |
| clipMap_t            | ❌               | ❌               |  |
| ComWorld             | ❌               | ❌               |  |
| GameWorldSp          | ❌               | ❌               |  |
| GameWorldMp          | ❌               | ❌               |  |
| MapEnts              | ✅               | ✅               |  |
| GfxWorld             | ❌               | ❌               |  |
| GfxLightDef          | ❌               | ❌               |  |
| Font_s               | ❌               | ❌               |  |
| MenuList             | ❌               | ❌               |  |
| menuDef_t            | ❌               | ❌               |  |
| LocalizeEntry        | ✅               | ✅               |  |
| WeaponDef            | ❌               | ❌               |  |
| SndDriverGlobals     | ❌               | ❌               |  |
| FxEffectDef          | ❌               | ❌               |  |
| FxImpactTable        | ❌               | ❌               |  |
| RawFile              | ✅               | ✅               |  |
| StringTable          | ✅               | ✅               |  |
| XmlTree              | ❌               | ❌               |  |
| SceneAnimation       | ❌               | ❌               |  |
| Cutscene             | ❌               | ❌               |  |
| CustomCamera         | ❌               | ❌               |  |

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

## T4 Xbox 360 (Call of Duty: World at War)

| Asset Type           | Dumping Support | Loading Support | Notes                                                                        |
|----------------------|-----------------|-----------------|------------------------------------------------------------------------------|
| PhysPreset           | ✅              | ✅              |                                                                              |
| PhysConstraints      | ❌              | ❌              |                                                                              |
| DestructibleDef      | ❌              | ❌              |                                                                              |
| XAnimParts           | ✅              | ✅              | T4 PC-compatible version-17 files in `xanim/`.                                |
| XModel               | ✅              | ✅              | Model data can be exported to `XMODEL_EXPORT/XMODEL_BIN`, `OBJ`, `GLB/GLTF`. |
| Material             | ✅              | ✅              | T4 PC-compatible material JSON, including water.                             |
| MaterialPixelShader  | ✅              | ✅              | Native `.xpu` microcode in `shader_bin/`.                                    |
| MaterialTechniqueSet | ❌              | ❌              |                                                                              |
| GfxImage             | ✅              | ✅              | DDS/IWI, 2D and cube textures. External highmips used when available.         |
| snd_alias_list_t     | ⁉️              | ❌              | Extracts loose/packed native `.xma`; alias definitions are not dumped.       |
| SndDriverGlobals     | ❌              | ❌              |                                                                              |
| LoadedSound          | ✅              | ❌              | Embedded XMA audio as RIFF `.xma` files.                                     |
| clipMap_t            | ❌              | ❌              |                                                                              |
| ComWorld             | ❌              | ❌              |                                                                              |
| GameWorldSp          | ❌              | ❌              |                                                                              |
| GameWorldMp          | ❌              | ❌              |                                                                              |
| MapEnts              | ✅              | ✅              |                                                                              |
| GfxWorld             | ❌              | ❌              |                                                                              |
| GfxLightDef          | ❌              | ❌              |                                                                              |
| Font_s               | ✅              | ✅              | T4 PC-compatible compiled font JSON; TrueType compilation is unsupported.     |
| MenuList             | ✅              | ✅              | The output is decompiled. The result will not be the same as the input.      |
| menuDef_t            | ✅              | ✅              | See menulist.                                                                |
| LocalizeEntry        | ✅              | ✅              |                                                                              |
| WeaponDef            | ✅              | ✅              | T4 PC-compatible weapon files and GDTs, accuracy graphs, and flame tables.     |
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

## T5 Xbox 360 (Call of Duty: Black Ops)

| Asset Type           | Dumping Support | Loading Support | Notes |
|----------------------|-----------------|-----------------|-------|
| PhysPreset           | ❌              | ❌              |       |
| PhysConstraints      | ❌              | ❌              |       |
| DestructibleDef      | ❌              | ❌              |       |
| XAnimParts           | ❌              | ❌              |       |
| XModel               | ❌              | ❌              |       |
| Material             | ❌              | ❌              |       |
| MaterialPixelShader  | ❌              | ❌              |       |
| MaterialTechniqueSet | ❌              | ❌              |       |
| GfxImage             | ⁉️               | ✅              | 2D and cube images. Some lightmap formats are unsupported. |
| SndBank              | ❌              | ❌              |       |
| SndPatch             | ❌              | ❌              |       |
| clipMap_t            | ❌              | ❌              |       |
| ComWorld             | ❌              | ❌              |       |
| GameWorldSp          | ❌              | ❌              |       |
| GameWorldMp          | ❌              | ❌              |       |
| MapEnts              | ✅              | ✅              |       |
| GfxWorld             | ❌              | ❌              |       |
| GfxLightDef          | ❌              | ❌              |       |
| Font_s               | ❌              | ❌              |       |
| MenuList             | ❌              | ❌              |       |
| menuDef_t            | ❌              | ❌              |       |
| LocalizeEntry        | ✅              | ✅              |       |
| WeaponVariantDef     | ❌              | ❌              |       |
| SndDriverGlobals     | ❌              | ❌              |       |
| FxEffectDef          | ❌              | ❌              |       |
| FxImpactTable        | ❌              | ❌              |       |
| RawFile              | ✅              | ✅              |       |
| StringTable          | ✅              | ✅              |       |
| PackIndex            | ❌              | ❌              |       |
| XGlobals             | ❌              | ❌              |       |
| ddlRoot_t            | ❌              | ❌              |       |
| Glasses              | ❌              | ❌              |       |
| TextureList          | ❌              | ❌              |       |
| EmblemSet            | ❌              | ❌              |       |

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
