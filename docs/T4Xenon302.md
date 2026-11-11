# T4 Xenon fastfile version 302

OAT identifies the March 27, 2007 World at War prototype as `T4Xenon302`.
The suffix denotes the **fastfile version**, rather than an executable build
number. The supplied MP build reports `1.0.7278`; the SP executable reports
`1.0.7` from March 20.

Use the **x86** tools. This implementation covers fastfile parsing and asset
pool registration, plus [CoD4 PC d3dbsp export](T4Xenon302D3DBsp.md) from render,
collision and common-world assets. Other asset export formats and disk asset
loaders are separate work.

```powershell
.\build\bin\Debug_x86\Unlinker.exe --list "C:\Users\Michael\games\xbox360\Call of Duty WaW (2007-03-27)\Call of Duty WaW (2007-03-27)\proto.ff"
```

## Format evidence

The primary reference is the supplied `default_mp.xex` decompile,
`t4_2007-03-27-default_mp.xex.c`. IW3 Xenon Alpha 253 provided a structural
starting point; its version-358 layouts are not interchangeable with this build.

* `DB_LoadXFileInternal` (`0x821535D0`) requires version **302**. The outer
  header contains seven big-endian words: version, decompressed payload size,
  and five allocation-block sizes. A zlib stream follows the 28-byte header.
* The allocation blocks are TEMP, RUNTIME, VIRTUAL, LARGE and PHYSICAL, numbered
  0 through 4. TEMP is reused for asset roots; runtime arrays allocate memory
  without consuming payload bytes.
* `DB_ConvertOffsetToPointer` and `DB_ConvertOffsetToAlias` use
  `(encoded - 1) & 0x3fffffff` for the offset and the upper **two** bits for the
  block. Having five allocation blocks does not imply three encoded block bits.
* `DB_InsertPointer` reserves four aligned bytes in VIRTUAL. Asset handles
  support following pointers (`-1`), inserted pointers (`-2`) and aliases.
* Material handles allocate on four-byte boundaries, independently of the
  native C++ structure alignment.
* `Load_XAssetHeader` (`0x8215E778`) supplies the asset IDs below. Pixel shaders
  are nested technique data, not a separate root asset in this version.

| IDs | Parsed data |
| --- | --- |
| 1 | Physics presets |
| 2, 3 | Animations and models, including LOD surface/material data |
| 4, 5, 6 | Materials, technique sets, shaders, declarations and images |
| 7, 8 | Sound aliases, XMA data, seek tables, speaker maps and sound curves |
| 9, 10 | SP/MP collision maps, entities, path nodes and dynamic entities |
| 11, 12, 13 | Common worlds, render worlds and light definitions |
| 15, 16, 17, 18 | Fonts, menu lists, menus and localization |
| 19, 20 | Weapons and sound-driver globals |
| 21, 22, 27 | Effects, impact-effect tables and raw files |

The executable has no fastfile load case for IDs 0, 14 or 23–26. These are not
invented as serialized assets. Both collision-map IDs use OAT's clipmap pool.

Important build-specific sizes include:

| Structure | Bytes | Executable reference |
| --- | ---: | --- |
| `XModel` / `XModelLodInfo` | 260 / 40 | `Load_XModel`, `0x82159210` |
| `XSurface` / `PhysGeomInfo` | 104 / 104 | Their respective load routines |
| `Material` / `MaterialPass` | 88 / 88 | `0x82158BF0` / `0x82155518` |
| `MaterialTechniqueSet` | 164 | 38 technique pointers |
| `MaterialVertexDeclaration` | 72 | 16 declaration pointers |
| `snd_alias_t` | 80 | Curve and speaker-map pointers at 72 and 76 |
| `clipMap_t` | 332 | `Load_clipMap_t`, `0x8215CE10` |
| `DynEntityClient` | 36 | Runtime allocation in `Load_clipMap_t` |
| `GfxWorld` | 752 | `Load_GfxWorld`, `0x8215B910` |
| `listBoxDef_s` | 256 | `Load_listBoxDef_ptr`, `0x82157138` |
| `FxTrailVertex` | 12 | `Load_FxTrailDef`, `0x82156708` |
| `WeaponDef` | 1888 | `Load_WeaponDef`, `0x8215DB68` |

Unidentified regions and uncertain field meanings use `unknown` names. Opaque
byte regions retain their original bytes; inferred pointer fields and counts
are decoded separately. These definitions establish serialization layouts, not
complete gameplay semantics.

## Validation

The supplied directory contains twelve version-302 files with **24,507
top-level asset records** and **399,315,945 decompressed bytes**:

| File | Top-level records |
| --- | ---: |
| `code_post_gfx.ff` | 282 |
| `code_post_gfx_mp.ff` | 294 |
| `common.ff` | 5,999 |
| `common_ignore.ff` | 22 |
| `common_mp.ff` | 3,256 |
| `mp_argun.ff` | 1,637 |
| `mp_bjd_test1.ff` | 1,539 |
| `mp_canal.ff` | 1,638 |
| `mp_rooftops.ff` | 1,678 |
| `proto.ff` | 8,144 |
| `ui.ff` | 6 |
| `ui_mp.ff` | 12 |

The stock test parses every version-302 file and requires end-of-payload after
the final asset. Together these files exercise every serialized asset family.
`descent.ff` is version **204** and uses the separate experimental
[T4Xenon204 target](T4Xenon204.md). It is excluded from this version-302 corpus. No game files or
decompiled executable sources are included in the repository.

```powershell
.\build\lib\Debug_x86\tests\SystemTests.exe "[t4xenon302][system]"
$env:OAT_T4_XENON_302_DIR = 'C:\Users\Michael\games\xbox360\Call of Duty WaW (2007-03-27)\Call of Duty WaW (2007-03-27)'
.\build\lib\Debug_x86\tests\SystemTests.exe "[t4xenon302][stock]"
```

The normal tests also cover version discrimination, executable-derived sizes,
two-bit offsets, inserted pointers, script strings, and preservation of unknown
bytes using a small independently constructed compressed fastfile.

The Debug x86 build passes 68 assertions in four target tests, 50 assertions in
the stock-corpus test, and all 646 CommonTests assertions. The CLI also lists
`proto.ff` with zero warnings and errors. The wider SystemTests run stops at the
existing IW4 Xenon invalid-water test's empty Base64 input assertion in
`src/Cryptography/Base64.cpp:51`; that code and test case were not changed here.
