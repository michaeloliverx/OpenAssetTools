# IW3 Xbox 360 technique files

Xbox 360 techsets use the same slot and binding syntax as IW3 PC, with extensions for compiled Xbox data.
They reference existing `.xpu` pixel shaders and `.xvu` vertex shaders; OAT does not compile HLSL for Xbox 360.
PC-only instanced technique slots are ignored when loading shared source for Xbox 360.

`techsets/<name>.techset` maps named slots to techniques:

```text
"unlit":
  vertcol_shaded_w;
```

`techniques/<name>.tech` contains passes. Shader parameter names come from the constant tables in the shader objects.
For example:

```text
{
  stateMap "passthrough";
  xenonVertexShaders
  {
    packed;
  }
  vertexShader 3.0 "vertcol_simple.hlsl"
  {
  }
  pixelShader 3.0 "vertcol_shaded.hlsl"
  {
    colorMapSampler = material.colorMap;
  }
}
```

Named bindings use the common IW3 argument compiler with Xbox shader reflection. It derives destination registers,
matrix transposition and row counts, argument order and update groups, and custom sampler flags.
Recognized engine constants and samplers are supplied automatically when omitted from a shader block.
Shader arrays use `parameter[index]`; matrix source rows use `constant.worldMatrix[row]`.
The same binding text can target different registers on PC and Xbox when the corresponding platform shaders are available.

The short vertex declaration form selects shaders named `<source>*<declaration>` from the following vertex shader block.
For example, `packed;` above loads `vertcol_simple.hlsl*packed`. Only the listed array slots are populated;
the pass's direct vertex shader pointer remains null. The selected shaders must have matching constant tables.
This block must precede the vertex shader block. To retain a direct shader pointer, irregular variant names,
or variants with different interfaces, use the explicit `packed "shaderName";` form instead.
Short declarations and explicit shader names cannot be mixed in one block.

| Extension | Meaning |
|-----------|---------|
| `xenonWorldVertexFormat value;` | Techset world vertex format byte. Omitted when zero. |
| `xenonUnused first, second;` | Two retained techset bytes. Omitted when zero. |
| `xenonTechniqueFlags value;` | Overrides inferred technique flags. Omitted when they match. |
| `xenonArgumentCounts perPrim, perObj, stable;` | Selects explicit layout for a pass and retains the exact argument groups. Appears before shader blocks. |
| `xenonCustomSamplerFlags value;` | Overrides inferred custom sampler flags. Omitted when they match. |
| `xenonPrecompiledIndex value;` | Compiled pass index. Omitted when zero. |
| `xenonOptionalSource value;` | Vertex declaration is present; `0` or `1` preserves its optional-source flag, including empty declarations. |
| `xenonVertexShaders { declaration; ... }` | Selects variants of the source named in the following vertex shader block. Missing entries and the direct shader pointer are null. |
| `xenonVertexShaders { declaration "shader"; ... }` | Exact shader variant array and original direct shader name. Missing entries are null, including an explicitly empty block. |
| `xenonArg index, destination, firstRow, rowCount;` | Explicit layout of the following binding in a pass with `xenonArgumentCounts`. Index spans both shader blocks. Row fields apply to code constants. |

Dumps omit argument-layout extensions when reflection and the normal compiler reproduce the compiled layout.
When reflection is missing or the native layout differs, the whole pass uses explicit counts and bindings instead;
automatic argument creation is disabled for that pass so no extra bindings are introduced.

Bindings use `constant.<name>`, `sampler.<name>`, `material.<name>` and `float4(...)` as on PC.
Unknown source values remain explicit as `constant.#0x...`, `sampler.#0x...` or `material.#0x...`.
When reflection is absent, destinations use `register[index]` or `samplerRegister[index]`.
`null` retains a null literal pointer. An empty shader name retains a null shader pointer in the explicit variant form.
All compiled bindings are written explicitly, including bindings that the PC compiler would infer from matching names.

Fastfiles contain final material state bits, not the original state-map rules. Dumps use `stateMap "passthrough"`,
as on PC, and retain the state bits in the material files. Other state maps are rejected by this native loader.
These files preserve compiled Xbox techniques; they are not complete recovery of the original Mod Tools source.

Filenames escape Windows-invalid characters as `~XX`. Shader and technique references retain their original asset names.
Shader runtime headers are recreated as zero-filled allocations, matching stock IW3 Xbox fastfiles, without JSON sidecars or an SDK dependency.

See [the PC/Xbox research notes](IW3XenonTechsetResearch.md) for the verified naming, binding and vertex-layout rules,
stock validation, and the boundaries of shader conversion through the Xbox SDK.
