# T4 Xenon fastfile version 204: Descent

OAT identifies this older file bundled with the March 27, 2007 World at War
prototype as `T4Xenon204`. The suffix is the fastfile version; the containing
build date does not establish when Descent itself was compiled. No matching
executable was used to establish this layout.

Use the **x86** tools. This target parses the complete supplied `descent.ff`
and exports its render world and collision map as CoD4 PC IBSP v22. Individual
asset disk formats and fastfile writing remain unsupported. Asset families absent
from this one file retain reference layouts and are not established version-204
support. Unresolved regions use `unknown` fields and explicit padding.

## Input and format

```text
C:\Users\Michael\games\xbox360\Call of Duty WaW (2007-03-27)\Call of Duty WaW (2007-03-27)\descent.ff
```

* Compressed file: 45,042,478 bytes; SHA-256
  `5365a6e92bff939fd34063745f52c2e4eea16ff447ef164c81ccaaf6048b3ea1`.
* Seven big-endian header words: version 204, decompressed byte count,
  then TEMP, RUNTIME, VIRTUAL, LARGE and PHYSICAL allocation sizes.
* Zlib starts at byte 28 and expands to **98,881,156 bytes** with no trailing
  compressed bytes. The parser consumes the entire decompressed payload.
* Two-bit reused-pointer block IDs and four-byte VIRTUAL insertions match the
  neighboring alpha formats. Asset IDs differ: animation 0, model 1, material 2,
  technique set 3, image 4, sound 5, curve 6, clipmap 7, render world 9,
  localization 15, weapon 16, effect 18 and rawfile 24. There are no physics-preset
  or model-pieces root entries in this registry.
* There are **629 script strings** and **7,369 root asset records**.

The diagnostic parser's final persistent allocation positions match every header
size exactly:

| Block | Declared bytes | Consumed allocation bytes |
| --- | ---: | ---: |
| RUNTIME | 15,972 | 15,972 |
| VIRTUAL | 37,976,788 | 37,976,788 |
| LARGE | 60,620,800 | 60,620,800 |
| PHYSICAL | 155,016 | 155,016 |

TEMP declares 1,652 bytes, enough for one weapon root. Nested roots require a
measured high-water mark of 2,060 bytes. This target rounds scratch capacity to
a 4 KiB boundary, as the Embassy target does. This is a compatibility choice;
persistent allocation sizes and aliases remain exact.

## Recovered layouts and coverage

| Structure | Bytes | Distinguishing layout |
| --- | ---: | --- |
| `XModel` | 256 | No later physics-preset/geometry tail |
| `XSurface` | 104 | Vertex/triangle counts at offsets 4 and 6 |
| `Material` | 96 | Larger info prefix; texture count at 70 |
| `MaterialTechniqueSet` | 148 | 35 pointers starting at 8; no remap pointer |
| `MaterialPass` | 20 | One vertex shader; no later shader-variant array |
| `MaterialVertexDeclaration` | 32 | Six declaration slots |
| `snd_alias_t` | 76 | Curve/speaker pointers at 68/72 |
| `clipMap_t` | 292 | No root plane array or later runtime client-list tail |
| `GfxWorld` | 560 | Earlier GPU buffers and lighting pointers |
| `GfxSurface` | 68 | Index-array pointer and count; vertex extent recovered from indices |
| `GfxStaticModelDrawInst` | 84 | Three retained lighting-lookup values after bounds |
| `GfxLightmapArray` | 12 | Three image pointers |
| `GfxLightGridColors` | 24 | Channel-major R[8], G[8], B[8] corner samples |
| `FxElemDef` | 232 | Earlier effect fields and visual IDs |
| `WeaponDef` | 1,652 | 28 animation names and earlier pointer/graph offsets |

| Asset family | Root records | Pooled including inline dependencies |
| --- | ---: | ---: |
| Animations | 54 | 54 |
| Models | 35 | 96 |
| Materials | 3 | 113 |
| Technique sets | 59 | 59 |
| Images | 0 | 193 |
| Sound aliases | 7,164 | 7,164 |
| Sound curves | 0 | 6 |
| Collision map | 1 | 1 |
| Render world | 1 | 1 |
| Localization | 18 | 18 |
| Weapons | 2 | 2 |
| Effects | 17 | 17 |
| Raw files | 15 | 15 |
| **Total** | **7,369** | **7,739** |

## BSP reconstruction

The checked IW3 Xenon 253 writer receives an adapted view of the recovered
geometry. The conversion gathers referenced collision/render planes, retains
inverse pairs, resolves triangle pointers into index ranges, derives each
surface's used vertex extent, reconstructs the pointer-based partition and
cell trees, and retains the native surface ordering. Partition planes and
collision/render leaf assignments are cross-checked.

The output contains **5,940 surfaces, 186,778 triangles, 17,587 collision
brushes, 984 submodels, 63 cells, 200 portals and 1,751 render model instances**.
The collision map has 1,745 static-model entries; the render-only instances are
retained through the existing entity recovery path.

The two 1024x1024 RGBA lightmap images are joined vertically to match the later
writer's atlas layout. The third 2048x2048 A8 image supplies sun visibility.
This recovers four PC lightmap pages with matching UV remaps. Reserved null/sun
primary-light records are emitted, including recovered source direction and
direct RGB, satisfying Radiant's requirement for at least two records.
There are no source reflection captures or generated multilayer materials.

The packed sparse light grid matches the stock linker's legacy version-15 point
format. The exporter unpacks XYZ, sunlight visibility, trace flags and color
references, then emits PC row headers, RLE rows and entries. The stock legacy
color conversion expands the eight channel-major corners to 56 directional RGB
samples. Descent contains 57,694 packed records and 45,504 distinct positions;
duplicate positions are retained once. Vertical holes use the stock default
entry and the most common sunlit palette color is moved to slot zero.

The entity string already contains all 1,751 authored `misc_model` placements.
Those entities, including existing `gndLt` values and custom keys, are retained
verbatim rather than appending reconstructed duplicates. If an authored model
list is present but its count differs from the render-world instances, export
fails explicitly. Files without an authored list retain model reconstruction.

The static-model volume-texture lookup remains unresolved for PC export. Native
data and lookup values are retained in sidecars:

* `.source-lightgrid.bin`: `LG204LE` plus NUL, two LE uint32 counts, decoded uint32
  pairs, then unchanged 24-byte RGB color records.
* `.source-model-lighting.bin`: `ML204LE` plus NUL, six LE uint32 values for
  width/height/depth/map type/pixel bytes/header bytes, the native 52-byte texture
  header, then native pixels. Per-instance lookup values and the opaque grid
  header remain in `.recovery.json`.
* `.debug.json`, `.recovery.json` and `.unlayered.json` describe the input assets,
  recovered fields, geometry conversion and limitations.

All active surface materials and all 55 unique render model names are present
in the installed CoD4 raw directory. The existing experimental helper adds the
simple geometry representation Radiant uses, preserving the original lumps.
The user confirmed the corrected grid/model export improves the model lighting,
with some purple tint still visible. A subsequent native-image audit confirms
all 12,582,912 lightmap bytes survive the export unchanged, with the texture
headers specifying unsigned BGRA, 8-in-32 endian conversion and no exponent
adjustment. Lilac values already occur in native lightmap data referenced by
dirt surfaces (for example RGB 197/161/214 near 1472/90/-1138). These are encoded
lightmap samples, rather than a prediction of the final material shading.
The native and converted light-grid colors have no strong magenta samples
(both red and blue more than eight byte values above green). Worldspawn also
retains slightly blue ambient `_color` 0.90/0.90/1. This supports investigating
baked lighting and PC material/shader behavior before changing the grid again;
it does not establish an exact visual match to the original executable.

## Installed Radiant test

```text
C:\Program Files (x86)\Steam\steamapps\common\Call of Duty 4\raw\maps\descent.d3dbsp
```

* Corrected viewer BSP: **67,866,140 bytes**.
* SHA-256: `0aca0c61dac3efd325eec229b452270d1913c1be9803498834bb0acd38686cae`.
* Original and simple geometry representations pass `scripts/verify_iw3_d3dbsp.py`.
* Focused parser tests including the actual fastfile: **610,809 assertions in five
  tests passed**, requiring exact EOF and checking geometry references and counts.
* Descent, Embassy, T4 FF302 and IW3 FF358 exporter regressions: **2,859 assertions
  in 41 tests passed**, including authored model retention, packed grid conversion,
  malformed triangle pointers/ranges and null/sun slots.
* Independent comparison validates all 45,504 native grid positions and their
  color/visibility/trace references, plus all 26,480 stock default padding entries.
  All 4,996,488 converted color bytes match an independent double-precision legacy
  interpolation within one byte of rounding. Every model origin has four to
  eight native grid corners nearby. Only the grid and entity lumps differ from
  the first export; geometry and lightmaps are byte-identical.
* Broader system suite excluding opt-in stock and material tests: **568,193
  assertions in 207 tests passed**. The existing unrelated empty-input Base64
  assertion in material tests is excluded, as in the neighboring target work.
* Native Unlinker listing and BSP dumping: zero warnings and zero errors.

The install manifest, installed hashes and previous installed files are retained
under `build/t4-204-research/lighting-fixed`. Recovery files are installed beside
the BSP. The user reports improved lighting; full visual fidelity remains unverified.

## Reproduce and validate

```powershell
$ff = 'C:\Users\Michael\games\xbox360\Call of Duty WaW (2007-03-27)\Call of Duty WaW (2007-03-27)\descent.ff'
.\build\bin\Debug_x86\Unlinker.exe --list $ff
.\build\bin\Debug_x86\Unlinker.exe --include-assets gfxworld --output-folder 'build/t4-204-research/bsp-dumps/?zone?' $ff
python tools/experimental/iw3-bsp-unlayer/t4_302_batch.py build/t4-204-research/bsp-dumps build/t4-204-research/radiant-unlayered --pc-raw 'C:\Program Files (x86)\Steam\steamapps\common\Call of Duty 4\raw' --derive-missing-normal-frames --force
python scripts/verify_iw3_d3dbsp.py build/t4-204-research/radiant-unlayered/maps/descent.d3dbsp --representation both
$env:OAT_T4_XENON_204_FF = $ff
.\build\lib\Debug_x86\tests\SystemTests.exe '[t4xenon204]'
.\build\lib\Debug_x86\tests\ObjWritingTests.exe '[t4xenon204]'
```

Copy the recovery sidecars alongside the converted BSP. Research logs, allocation
totals, validation reports and installation backups are under
`build/t4-204-research`. No original game files are added to the repository.
