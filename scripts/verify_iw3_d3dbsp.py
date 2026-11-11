"""Validate recovered IBSP v22 disk records against the SDK loader contracts.

Usage: python scripts/verify_iw3_d3dbsp.py <file.d3dbsp> [...]
No game install or third-party Python modules are required.
"""

import argparse
import collections
import json
import math
import re
import struct
from pathlib import Path


def validate(path, representation='layered'):
    assert representation in ('layered', 'simple'), 'Unknown geometry representation'
    simple = representation == 'simple'
    data = path.read_bytes()
    magic, version, count = struct.unpack_from('<4sII', data)
    assert magic == b'IBSP' and version == 22, 'Expected IBSP version 22'
    offset = 12 + 8 * count
    assert offset <= len(data), 'Truncated directory'
    lumps = {}
    for i in range(count):
        kind, size = struct.unpack_from('<II', data, 12 + i * 8)
        assert kind not in lumps and kind <= 54, f'Invalid/duplicate lump {kind}'
        assert offset + size <= len(data), f'Truncated lump {kind}'
        lumps[kind] = data[offset:offset + size]
        end = offset + ((size + 3) & ~3)
        assert not any(data[offset + size:end]), f'Nonzero padding after lump {kind}'
        offset = end
    assert offset == len(data), 'Trailing bytes'

    def records(kind, fmt):
        payload = lumps.get(kind, b'')
        size = struct.calcsize('<' + fmt)
        assert len(payload) % size == 0, f'Lump {kind} is not divisible by {size}'
        return list(struct.iter_unpack('<' + fmt, payload))

    materials = records(0, '64sii')
    names = [entry[0].split(b'\0', 1)[0].decode('ascii') for entry in materials]
    if simple:
        assert all(k in lumps for k in range(47, 52)), 'Missing simple geometry lumps'
    surfaces = records(47 if simple else 9, 'H4BHiIHHI')
    vertices = records(48 if simple else 10, '6fI10f')
    indices = [x[0] for x in records(49 if simple else 11, 'H')]
    lightmap_count, remaining = divmod(len(lumps.get(1, b'')), 0x300000)
    assert remaining == 0 and lightmap_count <= 31, 'Invalid raw lightmap pages'
    probe_count, remaining = divmod(len(lumps.get(41, b'')), 0x20044)
    assert remaining == 0, 'Invalid reflection probe record size'
    primary_lights = records(43, '4B15f64s')
    # Com_LoadPrimaryLights in both linker_pc and IW3xRadiant rejects <= 1.
    assert len(primary_lights) > 1, 'Radiant requires at least two primary lights (reserved null and sun slots)'
    assert primary_lights[0][0] == 0, 'Reserved primary-light slot zero must be null'
    if 52 in lumps:
        assert len(lumps[52]) == len(primary_lights), 'Light-region count must match ComWorld lights'
    region_hulls = records(53, '18fI')
    region_axes = records(54, '5f')
    assert sum(lumps.get(52, b'')) == len(region_hulls), 'Light-region hull count'
    assert sum(h[-1] for h in region_hulls) == len(region_axes), 'Light-region axis count'
    assert all(math.isfinite(x) for h in region_hulls for x in h[:-1]), 'Non-finite light-region hull'
    assert all(math.isfinite(x) for a in region_axes for x in a), 'Non-finite light-region axis'
    layered = 0
    uv_min, uv_max = math.inf, -math.inf
    checked_layer_batches = set()
    for i, surf in enumerate(surfaces):
        material, lightmap, probe, primary, shadow, _, layer_offset, first_vertex, vertex_count, index_count, first_index = surf
        assert material < len(materials), f'Surface {i} material'
        assert lightmap == 31 or lightmap < lightmap_count, f'Surface {i} lightmap'
        assert probe <= probe_count, f'Surface {i} reflection probe'
        assert primary < len(primary_lights) or primary == 0, f'Surface {i} light'
        assert shadow <= 1 and index_count % 3 == 0, f'Surface {i} flags/indices'
        assert first_vertex + vertex_count <= len(vertices), f'Surface {i} vertex range'
        assert first_index + index_count <= len(indices), f'Surface {i} index range'
        assert all(x < vertex_count for x in indices[first_index:first_index + index_count]), f'Surface {i} local indices'
        name = names[material]
        if simple:
            assert not name.startswith('*') and layer_offset == -1, 'Simple surface still uses layered material/data'
        if name.startswith('*'):
            assert re.fullmatch(r'\*\d+n?(?:_\d+n?)+', name), name
            layers = name[1:].split('_')
            assert 2 <= len(layers) <= 5, name
            assert name.count('n') <= 3, name
            for layer in layers:
                index = int(layer.rstrip('n'))
                assert index < len(names) and not names[index].startswith('*'), (name, index)
            uv_size = (len(layers) - 1) * 8
            stride = uv_size + max(0, name.count('n') - 1) * 4
            payload = lumps.get(42, b'')
            assert 0 <= layer_offset and layer_offset + vertex_count * stride <= len(payload), f'Layer range {name}'
            # Many surfaces reference the same large compiled vertex batch.
            # Check its bytes once and keep extrema without duplicating UVs.
            batch = (layer_offset, vertex_count, uv_size, stride)
            if batch not in checked_layer_batches:
                checked_layer_batches.add(batch)
                for vertex in range(vertex_count):
                    uv = struct.unpack_from('<' + 'f' * (uv_size // 4), payload, layer_offset + vertex * stride)
                    assert all(math.isfinite(x) and abs(x) < 1e7 for x in uv), (name, uv)
                    uv_min, uv_max = min(uv_min, *uv), max(uv_max, *uv)
            layered += 1
    for vertex in vertices:
        assert all(math.isfinite(x) for i, x in enumerate(vertex) if i != 6), 'Non-finite vertex'

    planes = records(4, '4f')
    nodes = records(27, '9i')
    leaves = records(28, '6i')
    leafbrushes = records(29, 'I')
    brushes = records(8, 'HH')
    sides = records(5, 'II')
    assert sum(brush[0] for brush in brushes) == len(sides), 'Brush side count'
    assert len(lumps.get(6, b'')) == len(sides), 'Brush edge count records'
    side = 0
    for count, material in brushes:
        assert count >= 6 and material < len(materials), 'Brush header'
        for i in range(count):
            assert sides[side + i][1] < len(materials), 'Brush side material'
            if i >= 6:
                assert sides[side + i][0] < len(planes), 'Brush side plane'
        side += count
    assert all(x[0] < len(brushes) for x in leafbrushes), 'Leaf brush reference'
    collision_verts = records(31, '3f')
    collision_tris = records(32, '3H')
    assert all(max(tri) < len(collision_verts) for tri in collision_tris), 'Collision vertex reference'
    assert len(lumps.get(33, b'')) == ((len(collision_tris) * 3 + 31) // 32) * 4, 'Walkability bit buffer'
    borders = records(34, '7f')
    partitions = records(35, '4BII')
    for _, _, tris, border_count, first_tri, first_border in partitions:
        assert first_tri + tris <= len(collision_tris), 'Collision partition triangle range'
        assert not border_count or first_border + border_count <= len(borders), 'Collision partition border range'
    aabbs = records(36, '6fHHI')
    for *_, material, children, index in aabbs:
        assert material < len(materials), 'Collision AABB material'
        assert index + children <= len(aabbs) if children else index < len(partitions), 'Collision AABB child/partition'
    cells = records(25, '6fHH4iB67s')
    for cluster, first_aabb, aabb_count, first_brush, brush_count, cell in leaves:
        assert -1 <= cell < len(cells), 'Leaf cell'
        assert first_aabb + aabb_count <= len(aabbs), 'Leaf collision AABB range'
        assert first_brush + brush_count <= len(leafbrushes), 'Leaf brush range'
    for plane, front, back, *bounds in nodes:
        assert 0 <= plane < len(planes), 'Collision node plane'
        for child in (front, back):
            assert child < len(nodes) if child >= 0 else -child - 1 < len(leaves), 'Collision node child'
        assert all(bounds[i] <= bounds[i + 3] for i in range(3)), 'Inverted node bounds'
    trees = records(51 if simple else 24, 'III')
    portals = records(26, '4I')
    portal_vertices = records(19, '3f')
    culls = records(50 if simple else 12, '6fII')
    cull_indices = records(13, 'I')
    for first, count, _ in trees:
        assert first + count <= len(surfaces), 'Render AABB surface range'

    def check_tree(root, next_free):
        children = trees[root][2]
        if not children:
            return next_free
        first_child = next_free
        assert first_child + children <= len(trees), 'Render AABB children'
        assert trees[first_child][0] == trees[root][0], 'Render AABB first-child surface'
        next_free += children
        for child in range(first_child, first_child + children):
            next_free = check_tree(child, next_free)
        return next_free

    for cell in cells:
        _, _, _, _, _, _, layered_tree, simple_tree, portal, portal_count, cull, cull_count, probes, probe_indices = cell
        tree = simple_tree if simple else layered_tree
        assert tree < len(trees), 'Cell AABB root'
        check_tree(tree, tree + 1)
        assert portal + portal_count <= len(portals), 'Cell portal range'
        assert cull + cull_count <= len(cull_indices), 'Cell cull group range'
        assert probes <= 64 and all(0 <= p <= probe_count for p in probe_indices[:probes]), 'Cell probe list'
    for plane, cell, vertex, count in portals:
        assert plane < len(planes) and cell < len(cells), 'Portal plane/cell'
        assert vertex + count <= len(portal_vertices), 'Portal vertex range'
    assert all(x[0] < len(culls) for x in cull_indices), 'Cull group reference'
    for *_, first, count in culls:
        # R_LoadCullGroups ignores firstSurface when surfaceCount is zero.
        # Empty Alpha groups can retain the unsigned -1 sentinel.
        assert not count or first + count <= len(surfaces), 'Cull group surface range'
    models = records(37, '6f4H4I')
    for model in models:
        assert model[6 + simple] == 0xffff or model[6 + simple] + model[8 + simple] <= len(surfaces), 'Model render range'
        assert model[10] + model[11] <= len(aabbs), 'Model collision range'
        assert model[12] + model[13] <= len(brushes), 'Model brush range'
    if models and models[0][8 + simple]:
        coverage = collections.Counter()
        for first, count, children in trees:
            if not children:
                coverage.update(range(first, first + count))
        world_first, world_count = models[0][6 + simple], models[0][8 + simple]
        assert set(coverage) == set(range(world_first, world_first + world_count)), 'Incomplete cell coverage of world surfaces'
        assert all(count == 1 for count in coverage.values()), 'Duplicate world surfaces in cell leaves'

    entries = records(2, 'HBB')
    colors = records(3, '168s')
    assert all(x[0] < len(colors) for x in entries), 'Light-grid color reference'
    if 44 in lumps:
        header = lumps[44]
        mins_maxs = struct.unpack_from('<6H', header)
        row_axis, col_axis = struct.unpack_from('<II', header, 12)
        assert row_axis < 3 and col_axis < 3, 'Light-grid axes'
        count = mins_maxs[row_axis + 3] - mins_maxs[row_axis] + 1
        assert len(header) == 20 + count * 2, 'Light-grid row count'
        rows = lumps.get(45, b'')
        for offset, in struct.iter_unpack('<H', header[20:]):
            if offset == 0xffff:
                continue
            offset *= 4
            assert offset + 12 <= len(rows), 'Light-grid row header range'
            start, columns, z_start, z_count, entry = struct.unpack_from('<4HI', rows, offset)
            assert columns and z_count and entry < len(entries), 'Light-grid row header byte order'
    return {'file': str(path), 'representation': representation, 'bytes': len(data), 'chunks': len(lumps), 'materials': len(names),
            'surfaces': len(surfaces), 'layered_surfaces': layered, 'vertices': len(vertices),
            'triangles': len(indices) // 3, 'brushes': len(brushes), 'cells': len(cells),
            'leaf_cells': dict(collections.Counter(leaf[5] for leaf in leaves)),
            'portals': len(portals), 'lightmap_pages': lightmap_count, 'reflection_probes': probe_count,
            'primary_lights': len(primary_lights),
            'layer_uv_range': [uv_min, uv_max] if math.isfinite(uv_min) else None}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('files', nargs='+', type=Path)
    parser.add_argument('--representation', choices=('layered', 'simple', 'both'), default='layered')
    args = parser.parse_args()
    representations = ('layered', 'simple') if args.representation == 'both' else (args.representation,)
    print(json.dumps([validate(path, rep) for path in args.files for rep in representations], indent=2))
