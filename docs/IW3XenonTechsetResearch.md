# IW3 PC and Xbox 360 techset research

These notes distinguish rules visible in the official tools and debug runtime from observations in shipped assets.
They describe the supplied public CoD4 Mod Tools, the PC linker decompilation, the IW3 472 Xbox debug decompilation,
and the installed Xbox SDK. They do not establish the exact implementation of the missing Xbox linker or recover HLSL.

## Authoring and compilation boundaries

The material GDF chooses `material.dll` on PC and `material_xenon.dll` on Xbox. The light GDF similarly selects
`light_xenon.dll`. The material factory selects the same `textfactory` DLL for PC, Xbox and PS3.

`material_xenon.dll` obtains the platform name through the converter interface and defines `TARGET_PLATFORM` before
expanding the selected material template. `textfactory.dll` also defines numeric `PC`, `XENON` and `PS3` predicates
from the converter's platform enum. Shared source can therefore generate target-specific output without maintaining
an unrelated material description for each platform. The later IW-Core converter sources confirm separate platform
roots and a shared root; those later sources are corroboration of the interface, not the original CoD4 implementation.

The supplied phong templates generate their technique-set names through common branches on both platforms.
Their automatic normal-map format is `DXT5` for PC and `DXN` for Xbox. Platform-specific shader behavior is therefore
still required even where logical material and shader names agree. SDK `D3DFMT_DXN` is a real named GPU format.

The material converter expands and parses material source, serializes material tables and references technique-set
names. This path is separate from final technique/shader loading. The PC linker reads `.techset` and `.tech` source,
loads precompiled shader objects from `shader_bin`, obtains their D3D constant tables and constructs native passes.
Finding the Xbox material DLL does not by itself recover the missing Xbox shader build pipeline.

Evidence:

- Mod Tools `deffiles/material.gdf`, `light.gdf`, `materialfactory.gdf` and `materials/mtl_phong*.template`.
- `material_xenon.dll.c`: `Convert` at `0x10020A50`; template selection `0x100209B0`, expansion `0x100208C0`,
  platform macro `0x10020610`, material parser `0x1000F130`, serializer `0x1000CD50`.
- `textfactory.dll.c`: platform predicates `0x10004190`, CSV factory `Convert` at `0x1000C850`.
- `iw3_linker_pc.exe.c`: vertex shader load `0x0045D090`, pixel shader load `0x0045D200`, reflection `0x0045D6E0`.

## Names and technique slots

PC has 34 technique slots and Xbox has 26. Xbox omits the seven instanced lighting slots and the instanced debug
bump-map slot. Map slots by their named role, rather than copying ordinal indexes across the missing range.
The current Xbox text loader ignores recognized PC-only slot labels when reading shared text.

Material categories use these existing PC conventions:

| Category | Material prefix | Technique-set prefix |
|----------|-----------------|----------------------|
| Default | empty | empty |
| Model | `m/` | `m_` |
| Model with vertex color | `mc/` | `mc_` |
| World | `w/` | `w_` |
| World with vertex color | `wc/` | `wc_` |

The supplied raw source and shipped Xbox techniques show `_m` and `_w` specialisations, including before `_sm3`.
Removing that observed specialisation for the purpose of comparing source files finds 2,262 matching techniques
among 2,686 unique shipped technique names. Their logical vertex/pixel shader names match in 2,261 cases.
The exception is `build_shadowmap_model00`: the PC source uses `vertcol_simple_atest_dtex.hlsl` for its vertex shader,
while Xbox uses `vertcol_simple_atest.hlsl`.

Techset slot mappings also show PC `_dtex` effect variants, `l_spot_shad_sm_*` versus Xbox `l_spot_shad_*` names,
and layered `zprepass` versus Xbox `zprepass_nc_w`. These are observed correspondences, not proof that a substring
may be removed from arbitrary authored names. Dumps preserve the native technique and shader asset names.
The missing source cases include stock variants and layered combinations absent from the public Mod Tools.

The runtime's technique-set remapper is another naming mechanism: it tokenizes a name and substitutes enabled
material features, including the `twk` environment-map override. It is a runtime feature remapper, not a PC/Xbox
conversion table.

Evidence:

- PC linker slot table `0x0045E380`; Xbox debug header `MaterialTechniqueType`.
- Existing IW3 `g_materialTypeInfo`; public raw `.techset`/`.tech` files compared with shipped Xbox assets.
- Xbox runtime `Material_NextTechniqueSetNameToken`, `Material_RemapTechniqueSetName`,
  `Material_GetRemappedFeatures_RunTime`, and `s_materialFeatures`.

## Vertex shader families and declarations

Xbox has 15 declaration slots. A variant is named `<source>*<declaration>`, for example
`particle_cloud_add.hlsl*pos_tex`. `Material_GetVertexShaderName` explicitly removes the first `*` suffix when
recovering the logical source from an array variant. If the pass has a direct shader pointer, it uses that name instead.

| Slot | Declaration | Use / source layout |
|------|-------------|---------------------|
| 0 | `generic` | Generic draw vertices |
| 1 | `packed` | Packed model vertices |
| 2 | `world` | Base world vertices |
| 3–4 | `world_t1n0`, `world_t1n1` | World with one additional texture-coordinate stream |
| 5–7 | `world_t2n0` through `world_t2n2` | World with two additional texture-coordinate streams |
| 8–10 | `world_t3n0` through `world_t3n2` | World with three additional texture-coordinate streams |
| 11–13 | `world_t4n0` through `world_t4n2` | World with four additional texture-coordinate streams |
| 14 | `pos_tex` | Position/texture-coordinate vertices, including particle clouds |

The declaration is constructed from `s_streamSourceInfo[layout][source]`, which specifies streams, offsets and
SDK declaration types. `Load_BuildVertexDecl` builds the per-layout declarations; `Load_BindVertexShader` binds each
populated shader to its declaration using the stream and layer-data strides. Shader variants therefore represent
compiled/bound input layouts; their names do not indicate different material constant meanings.

All 33,658 populated variant instances in the stock audit obey the name/slot rule. Every selected family has matching
constant-table names, register sets, registers, counts, classes, types and dimensions. Repeated shader names across
fastfiles also have matching constant tables. This supports a shorter, reversible text representation:

```text
xenonVertexShaders
{
  packed;
  world;
}
vertexShader 3.0 "vertcol_simple.hlsl"
{
  worldMatrix = constant.worldMatrix;
}
```

This selects exactly the named slots, loads `vertcol_simple.hlsl*packed` and `vertcol_simple.hlsl*world`, and retains
a null direct shader pointer. Explicit quoted variant names remain available for direct pointers, irregular names,
different interfaces and missing reflection. The loader rejects ambiguous families and mismatched constant tables.

Do not infer retained array membership from available streams alone. A route-availability prediction overpopulates
every audited stock pass. Some same-named techniques also retain different arrays in different fastfiles. The exact
offline pruning/specialisation policy is not recovered here; dumps retain membership explicitly.

Evidence: Xbox debug `MaterialVertexDeclType`, `s_streamSourceInfo`, `g_streamStride`, `g_layerDataStride`,
`Load_BuildVertexDecl`, `Load_BindVertexShader`, and `Material_GetVertexShaderName` (`0x82341768`).

## Layered world materials

The PC linker constructs a layered technique-set name from a fixed supported-family table. It accepts `w_`/`wc_`
input sets in this path, takes the first layer's lighting prefix, joins layer descriptors with `_`, and changes each
descriptor's `0` to that layer's index. Layer prefixes are not arbitrary names that can always be concatenated.

Its world-format rule is:

```text
baseForLayerCount = [0, 1, 3, 6, 9]       // 1 through 5 material layers
worldVertFormat = baseForLayerCount[layerCount - 1] + max(normalMappedLayers - 1, 0)
```

All 254 unambiguous multi-layer stock Xbox techset names checked match this formula. The Xbox world draw path
selects declaration index `worldVertFormat + 2`. The field has values 0 through 11 corresponding to the base `world`
and the 11 layered world declarations. These values are supported by the PC computation and Xbox consumers;
the exact world-format enum names are not present as a standalone enum in the supplied Xbox debug header.

Preserve the native byte in dumps. Applying the formula to arbitrary author-supplied names requires validating their
supported layer descriptors and intended world-material usage first.

Evidence: PC linker supported-family table `off_4EB5B8`, prefix `off_4EB270` (`wc_`), layer name append
`0x0045F820`, lookup `0x0045F870`, construction `0x004603D0`, base-format table `dword_4ECDE4`;
Xbox world draw consumers of `worldVertFormat + 2`.

## Constants, samplers and compiled arguments

The Xbox debug header agrees with the existing IW3/OAT declarations for all 98 explicitly named `CONST_SRC_CODE_*`
entries, including range aliases, all texture source IDs, shader argument kinds, stream sources and destinations.
These are engine meanings, distinct from destination registers chosen by a particular shader compiler.

In the live PC/Xbox Killhouse comparison, 185 shared logical shader names provide 874 matched constant destinations:
all agree. Of 635 matched sampler destinations, 612 agree and 23 differ. For example,
`lp_sun_t0c0n0s0_sm3.hlsl` places `normalMapSampler` at PC sampler 5 versus Xbox sampler 6, and `specularMapSampler`
at 6 versus 7. Reusing names and code-source IDs is supported; hardcoding PC destination registers is not.

The SDK exposes `D3DXSHADER_CONSTANTTABLE`, `D3DXSHADER_CONSTANTINFO` and `D3DXSHADER_TYPEINFO`, along with
`XGMicrocodeGetConstantTable` and `XGCopyUCodeToNativeConstantTable`. The cached shader object retains this table
in big-endian form. OAT reads it without an SDK dependency and resolves bindings against the target object.

The PC linker verifies destination coverage, infers matching engine parameters, handles matrix rows/transposition,
orders arguments by update frequency then argument kind, and sorts material bindings by hash and other bindings
by destination. It also extracts fixed custom samplers into pass flags and can merge contiguous code-matrix rows.
OAT's common IW3 argument compiler supplies the shared binding behavior; unusual compiled layouts retain explicit
counts/order/row metadata rather than being silently normalized.

Evidence: Xbox debug constant-source entries, `MaterialTextureSource`, argument and stream enums;
SDK `d3dx9shader.h` constant-table definitions and `xgraphics.h` constant-table APIs; PC linker auto binding
`0x0045B9B0`, row merge `0x0045BED0`/`0x0045BF50`, frequency `0x0045DB60`, ordering `0x0045DC20`,
pass assembly `0x0045DCE0`.

## Other retained pass fields

`precompiledIndex` selects draw routines, rather than selecting a shader file. In the Xbox static-model draw path,
index 1 dispatches to `R_DrawStaticModelLitNoPrepass`, index 2 to `R_DrawStaticModelUnlitNoPrepass`, and index 0 uses
the general path. The stock audit observes only 0, 1 and 2. These are verified consumer meanings; no original enum
declaration or complete offline selection rule has been recovered. OAT preserves the byte and accepts other values.

`hasOptionalSource` corresponds to optional source streams such as additional texture coordinates and normal
transforms. A retained empty declaration and a null declaration are different states. The text extensions preserve
that distinction, along with direct versus array shader pointers.

Evidence: Xbox `R_DrawStaticModels` (`0x8212DAA8`), its specialized draw routines, `MaterialStreamSource` and native passes.

## SDK shader conversion experiment

The installed SDK exposes `XGConvertDXTokensToMicrocode`, taking D3D shader tokens and an optional constant table.
A local experiment passes actual dumped PC shader objects through this compiler, retaining their reflected table.
The resulting objects have the same `0x102a1100` pixel / `0x102a1101` vertex signatures and cached/physical structure
that OAT reads. Converted particle-cloud vertex/pixel objects successfully load, reflect and link into an Xbox fastfile.

The catalog experiment converted 738 of 740 dumped PC Killhouse shader objects with SDK validation enabled.
OAT reads all 738 outputs, and their reflected constant tables match the PC originals. The two failures are
`ps_shadowmap_build_atest_sm2.hlsl` and `ps_shadowmap_build_atest_sm3.hlsl`: Xbox validation rejects a `kill`
instruction that is not followed by a write to an output register. Repeating those two conversions with
`XGCUCODE_SKIPOPTIMIZATION` still fails validation. Disabling validation was not used to treat either result as valid.

The installed SDK has an interface mismatch worth recording for reproducing this experiment: its header declares a
10-argument function (`@40` on x86), while `xgraphics.lib` exports `@44`. Disassembly shows an additional debugger
parameter pointer before the final register-limit argument. The isolated helper uses the verified library interface
and the installed VS2010 compiler with the corresponding SDK libraries. This workaround is not part of OAT.

Successful token conversion is not proof of game rendering equivalence. It retains the PC shader's calculations:
normal-map channel decoding, platform shader defines, input conventions, sampler assignments and game draw-path
assumptions still need validation. Shader objects must be bound to the intended vertex declarations. The newer SDK
also need not reproduce the game's original compiler output byte for byte. No converted shader has been tested on
an Xbox console in this research pass.

The useful next boundary is an optional external shader compilation step, followed by the existing target-reflected
technique compiler. Reconstructing original HLSL or making arbitrary PC shaders portable is a separate problem.

## Related material and light observations

The Xenon material parser accepts `streamable` before an image declaration, sets a converter-local flag of 8,
and explicitly rejects the combination with `watermap`. This is a source/converter flag, not the serialized image
flag value (`IMG_FLAG_STREAMING` is 0x10); those encodings must not be interchanged.
The image helpers in both Xenon DLLs construct `highmip\\<image>.hi` alongside normal image outputs, corroborating
the external high-mip naming used by the image dumper.

`light_xenon` converts the `attenuationMap`, strips its source filename extension for the image reference, and writes
a sampler byte of 0x62 followed by that image name and its terminating zero. Against the verified engine sampler
flags, 0x62 is `SAMPLER_FILTER_LINEAR | SAMPLER_CLAMP_U | SAMPLER_CLAMP_V`. This is a useful default for later light
asset work; it is not a D3D texture-filter enum value to cast directly into a runtime object.

Evidence: `material_xenon.dll.c` image declaration parser `0x1000ED70`; `light_xenon.dll.c` conversion `0x10002150`,
attenuation reference `0x10002060`, high-mip path `0x100187D0`; PC light reference reader `0x004590D0`;
IW3 engine sampler flags and image flags.

## State-map boundary

Fastfiles retain final material state bits, not the original authored base bits and state-map expressions. Multiple
authored descriptions can produce the same compiled bits, so a dump cannot generally invert this transformation.
OAT uses `passthrough` for native dumps and retains the compiled state bits in material files.

The PC linker applies `(oldBits & ~entryMask) | ruleValue` after selecting each rule from base-state variables.
Supporting authored Xbox state maps also requires connecting the IW3 layout, base bits and per-pass results to material
compilation. The existing shared `StateMapHandler::BuildVars` additionally matches encoded values with a subset-bit
test; zero-valued enum choices need an exact masked-field comparison before that path can be relied on generically.
This research does not enable authored state maps or replace retained state bits with guessed source.

Evidence: PC linker state application `0x0045E800`; current OAT state-map handler and native material/technique loaders.

## Validation and remaining work

The stock scan loaded all 74 supplied Xbox fastfiles: 4,279 technique-set instances, 23,703 technique instances,
23,750 passes and 33,658 populated vertex variants, covering 593 unique variant shader names. It checked actual
reflection on every variant instance, including repeated names across files.

Killhouse's resolved techset/shader dump was linked and redumped with the family shorthand. All compared native
fields agree across 69 techsets, 448 techniques, 449 passes and 6,994 arguments. All 1,179 `.tech`, `.techset`, `.xpu`
and `.xvu` files match after the round trip; 429 techniques use abbreviated families. This is fastfile and object-file
validation, not a console rendering test or a relink of every stock map.

The final system test run passed 43,869 assertions in 46 cases, including sparse families, reordered reflection,
different interfaces, irregular names, exact explicit fallback, and invalid family metadata.

Remaining work is to recover the Xbox linker's family-pruning and fast-path selection policy, confirm the observed
technique aliases against its actual build code, connect authored state-map/material compilation, and test SDK-converted
shaders against each platform's texture and vertex conventions. Existing native dumps preserve those unresolved fields.
