# IW3 Xenon FF269 Embassy d3dbsp export

The target now registers a `GfxWorld` dumper which reconstructs CoD4 PC IBSP v22
using the checked IW3 Xenon 253 writer. It uses Embassy's render world and clipmap;
this file has no separate common-world asset. This is a compiled-map recovery
for viewing, not an editable source `.map`.

## Installed Radiant test

The viewer-ready file is installed at:

```text
C:\Program Files (x86)\Steam\steamapps\common\Call of Duty 4\raw\maps\embassy.d3dbsp
```

* Size: **38,024,664 bytes**.
* SHA-256: `9ea3fb1adaf39f0cf8f738c7d3f8eb57245c9ffd2cb6b01e6d18ba4de8e4baaf`.
* Both the original and simple geometry representations pass
  `scripts/verify_iw3_d3dbsp.py` after installation.
* 1,979 surfaces, 94,217 triangles, 5,815 collision brushes, 217 submodels,
  three cells, four lightmap pages, one reflection capture, and 188 render model
  instances are retained.

All active surface materials have matching files in the installed CoD4 raw
material directory. The map contains no generated multilayer material recipes.
The existing optional unlayering helper adds the simple geometry lumps used by
Radiant, preserving the original geometry and other lumps. PC material appearance
still comes from the installed assets. Of 33 unique model names referenced by
entities, `character_sas_urban_hoodmask` is absent from the installed raw xmodels.

Recovery metadata, original decoded reflections, and the packed source light-grid
records are installed beside the BSP. The three `oat_iw3_269_*` reflection
correction entries were merged into `raw/reflections/reflections.csv`, preserving
its existing default and custom entries. Its previous contents are backed up under
`build/iw3-269-research/install-backup/20261027-151622`.

The original dump, converted output, install manifest, installed-file hashes and
validation reports are under `build/iw3-269-research`. The initial Radiant test
reported `Com_LoadPrimaryLights: no primary lights in bsp`. The corrected export
adds the required reserved null and sun records; every existing lump is byte
identical to the initial installed BSP. The previous installed files and the
new validation/install reports are under `primary-light-fix` in that research
directory. The user subsequently confirmed the corrected Embassy dump was fine
in Radiant before requesting the Descent experiment.

## Version-specific recovery

* There is no root plane array. The adapter gathers planes referenced by collision
  sides, collision nodes and render nodes, emits inverse pairs, and maps each
  pointer to its exported index.
* The 24-byte render nodes form a pointer graph. The adapter converts the world
  tree to the checked writer's compact partition representation, validating
  references, depth and cycles. The collision/render plane and leaf assignments
  are then cross-checked by the shared writer.
* The 32-byte brush-model records contain bounds and surface ranges without the
  later writable-bounds prefix.
* `GfxSurface` stores reflection-probe index at byte **44** and lightmap index at
  **45**. Sky surfaces establish the latter with the reserved value 31.
* The reflection array excludes the implicit black probe. Source zero-based
  references become PC one-based references; the source count sentinel becomes
  black. The 64x64 cubemap and mip tail are retained. The reconstructed correction
  matches the decoded source pixels exactly before texture compression.
* Source lightmaps store RGBA indirect lighting first and A8 sun visibility
  second. Reversing their roles for the checked view recovers four PC pages.
  Original compiler page order is unavailable; the established writer restores
  a consistent page order and matching UVs.
* The packed eight-byte sparse light-grid records remain unresolved. The PC BSP
  contains an empty grid. The `.source-lightgrid.bin` sidecar contains the eight
  bytes `LG269LE` plus a NUL, two little-endian uint32 counts, host-decoded uint32
  pairs for each entry, then the unchanged 168-byte color records. The opaque
  56-byte sun record is retained in recovery JSON. Dynamic model/grid lighting
  therefore remains provisional; baked lightmaps are exported.
* PC primary-light lump 43 contains two 128-byte records: the reserved null light
  at index zero and the directional sun at index one. The local Radiant and stock
  linker `Com_LoadPrimaryLights` implementations reject counts of zero or one.
  The sun direction and direct RGB are decoded from big-endian floats at offsets
  4 and 20 of the retained 56-byte source record. Embassy's direction is
  `(-0.81379765, 0.29619819, 0.5)`, matching the retained authored sun angles;
  direct RGB is approximately `(0.216, 0.216, 0.24)`. Separate ambient RGB is not
  mapped into the direct-light color. Unresolved flags and light limits remain
  zero; no light regions are emitted. A missing source sun produces a zero-energy
  compatibility sun slot. All source surface and static-model primary-light
  indices are zero; other values are rejected.

## Reproduce

Use the x86 Unlinker:

```powershell
$ff = 'C:\Users\Michael\games\xbox360\Call of Duty 4 - Modern Warfare (Mar 23, 2007 prototype)\Call of Duty 4 Modern Warfare - Pre-Alpha 253 (March 23, 2007)\embassy.ff'
.\build\bin\Debug_x86\Unlinker.exe --include-assets gfxworld --output-folder 'build/iw3-269-research/bsp-dumps/?zone?' $ff
```

The existing local experimental helper accepts this dumper's material audit:

```powershell
python tools/experimental/iw3-bsp-unlayer/t4_302_batch.py build/iw3-269-research/bsp-dumps build/iw3-269-research/radiant-unlayered --pc-raw 'C:\Program Files (x86)\Steam\steamapps\common\Call of Duty 4\raw' --derive-missing-normal-frames
python scripts/verify_iw3_d3dbsp.py build/iw3-269-research/radiant-unlayered/maps/embassy.d3dbsp --representation both
```

Use a fresh output directory or the helper's `--force` option. Retain the dumper's
binary recovery sidecars when copying the converted result. The production
exporter and eight focused tests build successfully; the tests pass **435
assertions**, covering packed vertices, index counts, nested trees, malformed
trees, model/path placement, plane gathering, partition recovery, surface layout,
and the reserved null/sun records with retained big-endian sun fields. Three Python
validator regression tests also pass, including rejection of missing lights and
an invalid reserved slot. The validator now checks the Radiant light-count contract.
