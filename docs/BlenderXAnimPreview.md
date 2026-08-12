# Previewing IW4 XAnims in Blender

IW4 XAnim GLB files contain animation channels but no mesh, bind pose, or bone hierarchy. The OpenAssetTools Blender helper maps those channels by bone name onto one or more matching XModel armatures and bakes Blender actions for previewing.

## Export the assets

Dump XModels and XAnims as GLB files:

```bash
./build/bin/Release_arm64/Unlinker \
  --verbose \
  --model-format GLB \
  --xanim-format GLB \
  --output-folder "zone_dump/?game?/?zone?" \
  "/path/to/iw4/zone.ff"
```

XModels are written below `model_export/`, and XAnims are written below `xanim/`.

## Install the Blender helper

Install `tools/blender/oat_xanim_preview.py` through Blender's **Edit > Preferences > Add-ons > Install from Disk** command, then enable **OpenAssetTools XAnim Preview**. During development, the file can instead be opened in Blender's Scripting workspace and run directly.

## Create a preview

1. Import the matching XModel GLB with **File > Import > glTF 2.0**.
2. Import the XAnim GLB in the same Blender scene.
3. Select the target XModel armature in the Outliner rather than its mesh. Multiple armatures can be selected when an animation spans multiple models, such as view hands and a weapon.
4. Press **N** over the 3D View and open the **OAT** tab in the sidebar.
5. Choose the imported `_oat_xanim` object as **XAnim root**. It is selected automatically when exactly one exists.
6. Click **Bake XAnim to Selected Armatures**.
7. Scrub the timeline or use the Action Editor to preview the generated action.

Before baking, separately imported view hands and weapons remain in their own model spaces, so the hands appear around 60 units above the weapon. With **Attach weapon models** enabled, the helper attaches a selected weapon armature whose root is `j_gun` to the selected hands' animated `tag_weapon` bone.

The helper preserves the XModel rest transform for components that are not present in an XAnim track. IW4's `tag_clip` translation is applied relative to its XModel bind position so reload animations keep the magazine aligned when it is inserted. Animated scale is not supported. The synthetic `_oat_delta` root-motion node is currently left unmatched rather than moving the armature object.

The chosen XModel must use the same skeleton and bone names as the animation. An unmatched-node count is shown after baking to help identify incomplete model selections.
