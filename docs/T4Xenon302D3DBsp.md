# T4 Xenon version-302 d3dbsp export

The March 2007 target registers a `GfxWorld` dumper which reconstructs CoD4 PC
IBSP v22 through the checked IW3 Xenon 253 writer. The corresponding clipmap
and common world must be loaded. It exports render geometry and generated-layer
streams, collision, brush models, cells/portals/trees, entities, static/dynamic
model placement, embedded path connections, lightmaps, light grid, primary
lights and reflection captures. This is a viewer/recovery format; it does not
recreate editable source brushes or patches.

## T4 layouts

* `srfTriangles_t` retains an **index count**, as in build 253. Do not multiply
  it by three as the newer IW3 alpha adapters do.
* Entity text and `PathData` reside inside the 332-byte clipmap. There are no
  separate MapEnts or GameWorld assets in version 302.
* `GfxStaticModelDrawInst` is 76 bytes. `R_StaticModelLoddedOut` (`0x8241DE38`)
  identifies cull distance at 0 and origin at 4. Placement drawing
  (`0x82449108`) decodes the packed axis at 16 and scale at 28. Model is at 32;
  `R_AABBTreeStaticModels_r` (`0x8242E5E8`) uses mins/maxs at 36/48.
  `R_SetStaticModelLighting` (`0x8243B2D8`) reads ground color at 60 and primary
  light at 74. Cache handles start at 64; reflection-probe index is a **ushort**
  at 72 (`0x824492E8`). Byte 75 remains `unknown_0x4b` and is retained in recovery.
* `GfxLight` is 60 bytes: word type, color, direction, origin, radius, cone
  cosines, exponent and light-definition pointer. `R_SetLightProperties`
  (`0x824432B0`) and sun interpretation (`0x824413F8`) establish those fields.
  The 68-byte `ComPrimaryLight` also starts with a word type, rather than the
  later byte type/shadow flags. Its endian conversion must include that word.
* Four supplied maps have zero common-world lights but retain two graphics-world
  lights. Those graphics records supply the export's light list, keeping surface
  and grid indices valid. Shadow-map flags and unavailable motion limits become
  zero and this fallback is recorded. Authored common-world records take priority.
* Native packed axes/vertex frames use the shared Xenon third-based decoding.
  Vertex-layer floats and raw light-grid row words remain big endian until export.

## Reproduce the export

Use an x86 Unlinker built with this target. The executable used for this delivery
is `build/bin/Debug_x86/UnlinkerT4Xenon302.exe`: it uses the normal CLI entry point
and current libraries. Another running Afghan job held `Unlinker.exe` open, so
that process was left untouched and the final CLI was linked under a separate name.

```powershell
$game = 'C:\Users\Michael\games\xbox360\Call of Duty WaW (2007-03-27)\Call of Duty WaW (2007-03-27)'
& .\build\bin\Debug_x86\UnlinkerT4Xenon302.exe --include-assets gfxworld --search-path $game -l "$game\common.ff" -l "$game\common_mp.ff" -o 'build/t4-302-research/bsp-dumps/?zone?' "$game\mp_argun.ff"
```

Repeat for `mp_bjd_test1`, `mp_canal`, `mp_rooftops` and `proto` (omit common_mp
for proto). Version-204 `descent.ff` is excluded. The dumper writes recovery JSON,
original decoded reflection records and material/geometry metadata. Material
metadata is available in release builds too, including loaded common-zone
references when available.

## Optional Radiant material unwrapping

The local experimental helper uses the same separate geometry conversion as the
other alpha deliveries. Original layered lumps are preserved; simple lumps
47-51 expose generated layers as ordinary named base-material surfaces. Each
layer retains its own UVs, weights and reconstructed tangent frame. Trees,
cull ranges and brush-model ranges are remapped and both representations are
independently validated. The helper and its dependencies reside under the
repository's ignored `tools/experimental/iw3-bsp-unlayer` directory.

```powershell
python tools/experimental/iw3-bsp-unlayer/t4_302_batch.py build/t4-302-research/bsp-dumps build/t4-302-research/radiant-unlayered --pc-raw 'C:\Program Files (x86)\Steam\steamapps\common\Call of Duty 4\raw' --derive-missing-normal-frames --allow-missing-material-inputs
```

Installed PC materials supply tint/usage inputs, with retained native base
material metadata available as fallback. Combined generated-material definitions
in proto are unresolved references; the conversion does not claim to recover
their original shader constants. `--allow-missing-material-inputs` explicitly
allows neutral tint and ordinary alpha blending for two absent inputs:
`ca_quarry_wall02` and `us_dirtroad_blend`. Their recipe normal flags, source UVs,
weights and frames remain intact. Every approximation is recorded in the
`.unlayered.json` sidecar; the default rejects missing inputs. Five-layer
non-opaque recipes without base opacity remain unsupported.

The output does not convert native materials, images, models or custom shaders.
Missing PC material files in this installation are `brown_128`, `green_1024`,
`grey_1024`, `mtl_glass_frosted_panel`, `ca_quarry_wall02`, `glasstest` and
`us_dirtroad_blend`. Their surfaces remain present with their original names.
Reflection corrections are merged into the existing raw/reflections CSV without
replacing its default or custom entries. Original decoded runtime probes remain
in recovery sidecars; reconstructed probes differ by at most one byte in one
pixel each for argun and proto.

## Corpus validation

| Map | Simple surfaces | Simple triangles | Render model instances |
| --- | ---: | ---: | ---: |
| mp_argun | 2,607 | 104,407 | 482 |
| mp_bjd_test1 | 19 | 572 | 0 |
| mp_canal | 154 | 7,222 | 26 |
| mp_rooftops | 821 | 32,861 | 292 |
| proto | 3,377 | 111,325 | 1,763 |

All five fastfiles export with zero warnings/errors. Both layered and simple
representations pass `scripts/verify_iw3_d3dbsp.py`; the batch also checks original
lump preservation and hashes. The focused adapter tests pass 297 assertions in
seven cases, the unwrapping suite passes 22 tests, and the version-302 parser and
complete 12-file stock corpus pass 68 and 50 assertions respectively.

The installed BSPs use `raw/maps/mp/<name>.d3dbsp` for multiplayer and
`raw/maps/proto.d3dbsp` for proto. Installation backups, file hashes, raw exports
and recovery sidecars are under `build/t4-302-research`. Visual confirmation in
Radiant remains the user's test.
