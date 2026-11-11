# IW3 Xenon fastfile version 269

`IW3Xenon269` is an experimental, Xbox 360-only parser for the supplied
`embassy.ff`. The suffix is its fastfile version, not an executable build number.
There is no matching executable; these layouts are inferred from the file,
using the existing IW3 Xenon and T4 Xenon 302 parsers as references.

Use the **x86** tools:

```powershell
.\build\bin\Debug_x86\Unlinker.exe --list "C:\Users\Michael\games\xbox360\Call of Duty 4 - Modern Warfare (Mar 23, 2007 prototype)\Call of Duty 4 Modern Warfare - Pre-Alpha 253 (March 23, 2007)\embassy.ff"
```

This target loads every asset in this file into OAT's asset pools and exports
[CoD4 PC d3dbsp files for Radiant](IW3Xenon269D3DBsp.md). Other object dumpers,
disk asset loaders, and writing version-269 fastfiles remain unsupported.

## Input and format evidence

* Input: 45,196,886 bytes; SHA-256
  `3ad5f43b6de2d7587192afe86028b28c94efd388fe569356a04a4f6b50f471eb`.
* Seven big-endian header words: version 269, decompressed size 96,044,746,
  then five block sizes. Zlib data starts at byte 28.
* Block order: TEMP (1,856), RUNTIME (21,708), VIRTUAL (27,137,532),
  LARGE (68,683,776), PHYSICAL (89,004).
* Reused pointers use two block bits and one-based offsets. Inserted pointers
  reserve four aligned bytes in VIRTUAL. The asset-list header is read raw,
  outside block allocation; its strings and asset table occupy VIRTUAL.
* The file declares 628 script strings and 5,219 top-level asset records.
  Its clipmap ID is 9, render-world ID 11, weapon ID 18, and rawfile ID 26.
  Pixel shaders are nested technique data.

The reported TEMP size holds one weapon root but is smaller than the 2,264
bytes needed by nested weapon/model/material/image loads. This target rounds
TEMP capacity to a 4 KiB page boundary. This is an experimental compatibility
choice, not a claim about the missing executable's allocator. Persistent block
sizes and alias offsets remain exact. Diagnostic parsing reaches the declared
end of each persistent block and consumes all 96,044,746 payload bytes.

## Recovered layouts

| Structure | Bytes | Distinguishing layout |
| --- | ---: | --- |
| `PhysPreset` | 40 | No later cylinder-default field |
| `XModel` | 264 | Separate variable-length table of per-LOD material arrays |
| `XModelLodInfo` | 40 | Material-name script strings at 12, surfaces at 16 |
| `XSurface` | 104 | 32-byte packed vertices and 16-bit indices |
| `Material` | 88 | Texture/constant/state counts at 62/63/64 |
| `MaterialTechniqueSet` | 152 | 35 technique pointers |
| `clipMap_t` | 300 | No root plane array; planes referenced through sides and nodes |
| `GfxWorld` | 620 | Explicit BSP node graph; older lighting and submodel records |
| `GfxBspNode` | 24 | Parent, plane and child references; opaque leaf payload |
| `GfxCell` | 48 | Four trailing unknown bytes |
| `GfxBrushModel` | 32 | Bounds and surface range |
| `GfxLightmapArray` | 12 | Two images and an unknown word |
| `GfxLightGridEntry` | 8 | Two packed words with unresolved semantics |
| `GfxLight` | 56 | Raw bytes retained; BSP adapter decodes sun direction and direct RGB |
| `WeaponDef` | 1,856 | Older pointers and four accuracy-graph counts |

Unresolved fields use `unknown` names and explicit padding. Opaque byte regions
are retained without inventing scalar meanings. Families absent from this
single input retain reference layouts and have **not** been validated for
version 269; they should not be considered established support.

## Coverage and validation

| Top-level asset family | Records |
| --- | ---: |
| Technique sets | 32 |
| Materials | 3 |
| Sound aliases | 4,796 |
| Animations | 208 |
| Models | 18 |
| Collision map | 1 |
| Render world | 1 |
| Localization | 46 |
| Weapons | 15 |
| Effects | 59 |
| Raw files | 40 |

Including inline dependencies, the resulting pools contain 5,607 assets:
73 models, 122 materials, 207 images, one physics preset, six sound curves,
and the other families above. Images retain their load definitions and texture
headers after TEMP reuse.

Recovered map data includes 1,979 render surfaces, 163,175 render vertices,
282,651 render indices, 5,815 collision brushes, 22,036 collision triangles,
217 submodels, 188 static-model instances, three cells, two lightmap images,
and 40,723 light-grid entries. The BSP exporter retains the unresolved packed
light grid in a recovery sidecar and emits an empty PC grid. The output passes
structural checks; the user subsequently confirmed the corrected dump was fine
in Radiant.

The initial viewer test exposed an omitted primary-light lump. The corrected
export supplies the PC reserved null/sun slots, recovering direction and direct
RGB from the retained source sun. Both BSP representations pass the validator's
new light-count check; the user confirmed the corrected installed dump was fine.

The focused system tests exercise header detection, target resolution, literal
layout sizes, inserted aliases, unknown-byte retention, nested TEMP scratch,
and a two-LOD material table. The opt-in stock test requires exact payload EOF,
checks all pool counts, and validates geometry ranges and BSP node references:

```powershell
$env:OAT_IW3_XENON_269_FF = "C:\path\to\embassy.ff"
.\build\lib\Debug_x86\tests\SystemTests.exe "[iw3xenon269]"
```

Verified with the x86 Debug build:

* Regular Unlinker auto-detection and listing: zero warnings and zero errors.
* Focused tests, including Embassy: **315,601 assertions in five tests passed**.
* Broader suite excluding material tests: **568,124 assertions in 203 tests passed**.
  The unfiltered suite encounters an existing empty-input assertion in
  `src/Cryptography/Base64.cpp` during a material-loader test. The broader run
  required normal access to the tests' `.oat` cache directories.

No stock fastfile or extracted game data is added to the repository.
