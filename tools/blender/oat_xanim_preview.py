bl_info = {
    "name": "OpenAssetTools XAnim Preview",
    "author": "OpenAssetTools",
    "version": (0, 3, 0),
    "blender": (4, 2, 0),
    "location": "3D View > Sidebar > OAT",
    "description": "Bake an imported OpenAssetTools XAnim GLB onto matching XModel armatures",
    "category": "Animation",
}

import math
import re

import bpy
from bpy.props import BoolProperty, PointerProperty
from bpy.types import Object, Operator, Panel, PropertyGroup
from mathutils import Matrix, Quaternion, Vector


XANIM_ROOT_NAME = "_oat_xanim"
BLENDER_NAME_SUFFIX = re.compile(r"\.\d{3}$")
ATTACHMENT_CONSTRAINT_NAME = "OAT XAnim Attachment"
ATTACHMENT_RULES = (("j_gun", "tag_weapon"),)
REST_RELATIVE_TRANSLATION_BONES = {"tag_clip"}


def _is_xanim_root(obj):
    return obj is not None and obj.type == "EMPTY" and obj.name.startswith(XANIM_ROOT_NAME)


def _poll_xanim_root(_self, obj):
    return _is_xanim_root(obj)


def _find_xanim_root(scene):
    roots = [obj for obj in scene.objects if _is_xanim_root(obj)]
    return roots[0] if len(roots) == 1 else None


def _descendants(root):
    pending = list(root.children)
    while pending:
        obj = pending.pop()
        yield obj
        pending.extend(obj.children)


def _action_fcurves(animation_data):
    if animation_data is None or animation_data.action is None:
        return []

    action = animation_data.action
    if hasattr(action, "fcurves"):
        return list(action.fcurves)

    slot = getattr(animation_data, "action_slot", None)
    if slot is None:
        return []

    result = []
    for layer in action.layers:
        for strip in layer.strips:
            if strip.type != "KEYFRAME":
                continue
            try:
                channel_bag = strip.channelbag(slot)
            except RuntimeError:
                continue
            if channel_bag is not None:
                result.extend(channel_bag.fcurves)
    return result


def _source_channels(obj):
    curves = _action_fcurves(obj.animation_data)
    has_location = any(curve.data_path == "location" for curve in curves)
    has_rotation = any(
        curve.data_path in {"rotation_quaternion", "rotation_euler", "rotation_axis_angle"}
        for curve in curves
    )
    frames = {
        float(point.co.x)
        for curve in curves
        if curve.data_path in {"location", "rotation_quaternion", "rotation_euler", "rotation_axis_angle"}
        for point in curve.keyframe_points
    }
    return has_location, has_rotation, frames


def _game_location(location):
    return Vector((location.x, location.z, -location.y))


def _game_rotation(rotation):
    return Quaternion((rotation.w, rotation.x, rotation.z, -rotation.y)).normalized()


def _rest_local_matrix(pose_bone):
    bone = pose_bone.bone
    if bone.parent is None:
        return bone.matrix_local.copy()
    return bone.parent.matrix_local.inverted_safe() @ bone.matrix_local


def _target_bone_name(source_name, target_armatures):
    if any(source_name in armature.pose.bones for armature in target_armatures):
        return source_name

    without_suffix = BLENDER_NAME_SUFFIX.sub("", source_name)
    if without_suffix != source_name and any(without_suffix in armature.pose.bones for armature in target_armatures):
        return without_suffix
    return source_name


def _animation_name(source_objects, source_root):
    for obj in source_objects:
        if obj.animation_data and obj.animation_data.action:
            return obj.animation_data.action.name
    return BLENDER_NAME_SUFFIX.sub("", source_root.name).removeprefix(f"{XANIM_ROOT_NAME}_") or "XAnim"


def _new_action(armature, name):
    action = bpy.data.actions.new(name=name)
    animation_data = armature.animation_data_create()

    if hasattr(action, "slots"):
        slot = action.slots.new(armature.id_type, armature.name)
        layer = action.layers.new("XAnim")
        strip = layer.strips.new(type="KEYFRAME")
        channel_bag = strip.channelbag(slot, ensure=True)
        animation_data.action = action
        animation_data.action_slot = slot
        return channel_bag.fcurves

    animation_data.action = action
    return action.fcurves


def _add_fcurve(fcurves, data_path, index, samples):
    curve = fcurves.new(data_path=data_path, index=index)
    curve.keyframe_points.add(len(samples))
    for point, (frame, value) in zip(curve.keyframe_points, samples):
        point.co = frame, value
        point.interpolation = "LINEAR"
    curve.update()


def _attach_secondary_armatures(armatures):
    attachment_count = 0
    for child_root_name, host_bone_name in ATTACHMENT_RULES:
        hosts = [
            armature
            for armature in armatures
            if armature.pose.bones.get(host_bone_name) is not None
        ]
        if len(hosts) != 1:
            continue

        host = hosts[0]
        for child in armatures:
            root_bone = child.data.bones.get(child_root_name)
            if child == host or root_bone is None or root_bone.parent is not None:
                continue

            constraint = child.constraints.get(ATTACHMENT_CONSTRAINT_NAME)
            if constraint is not None and constraint.type != "COPY_TRANSFORMS":
                child.constraints.remove(constraint)
                constraint = None
            if constraint is None:
                constraint = child.constraints.new(type="COPY_TRANSFORMS")
                constraint.name = ATTACHMENT_CONSTRAINT_NAME

            constraint.target = host
            constraint.subtarget = host_bone_name
            constraint.target_space = "WORLD"
            constraint.owner_space = "WORLD"
            attachment_count += 1

    return attachment_count


def _clear_secondary_attachments(armatures):
    for armature in armatures:
        constraint = armature.constraints.get(ATTACHMENT_CONSTRAINT_NAME)
        if constraint is not None:
            armature.constraints.remove(constraint)


class OATXAnimPreviewSettings(PropertyGroup):
    source_root: PointerProperty(
        name="XAnim root",
        description="The _oat_xanim root created by importing an XAnim GLB",
        type=Object,
        poll=_poll_xanim_root,
    )
    hide_source_nodes: BoolProperty(
        name="Hide source nodes",
        description="Hide the imported animation-only empties after baking",
        default=True,
    )
    attach_weapon_models: BoolProperty(
        name="Attach weapon models",
        description="Attach selected j_gun-rooted armatures to the selected hands' tag_weapon bone",
        default=True,
    )


class OAT_OT_bake_xanim_preview(Operator):
    bl_idname = "oat.bake_xanim_preview"
    bl_label = "Bake XAnim to Selected Armatures"
    bl_description = "Map XAnim nodes to bones with the same names and create preview actions"
    bl_options = {"REGISTER", "UNDO"}

    def execute(self, context):
        settings = context.scene.oat_xanim_preview
        source_root = settings.source_root or _find_xanim_root(context.scene)
        if source_root is None:
            self.report({"ERROR"}, "Choose an XAnim root; automatic selection requires exactly one _oat_xanim root")
            return {"CANCELLED"}

        target_armatures = [obj for obj in context.selected_objects if obj.type == "ARMATURE"]
        if not target_armatures:
            self.report({"ERROR"}, "Select at least one imported XModel armature")
            return {"CANCELLED"}

        source_objects = list(_descendants(source_root))
        animation_name = _animation_name(source_objects, source_root)
        bindings = []
        mapped_sources = set()

        for source in source_objects:
            has_location, has_rotation, frames = _source_channels(source)
            if not frames or not (has_location or has_rotation):
                continue

            bone_name = _target_bone_name(source.name, target_armatures)
            for armature in target_armatures:
                pose_bone = armature.pose.bones.get(bone_name)
                if pose_bone is None:
                    continue

                rest_local = _rest_local_matrix(pose_bone)
                rest_location, rest_rotation, rest_scale = rest_local.decompose()
                bindings.append(
                    {
                        "armature": armature,
                        "pose_bone": pose_bone,
                        "source": source,
                        "has_location": has_location,
                        "has_rotation": has_rotation,
                        "relative_location": bone_name in REST_RELATIVE_TRANSLATION_BONES,
                        "frames": frames,
                        "rest_local_inverse": rest_local.inverted_safe(),
                        "rest_location": rest_location,
                        "rest_rotation": rest_rotation,
                        "rest_scale": rest_scale,
                        "samples": [],
                    }
                )
                mapped_sources.add(source)

        if not bindings:
            self.report({"ERROR"}, "No animated XAnim nodes match bones on the selected armatures")
            return {"CANCELLED"}

        source_root.hide_set(False)
        context.view_layer.update()
        current_frame = context.scene.frame_current_final
        all_frames = sorted({frame for binding in bindings for frame in binding["frames"]})
        bindings_by_frame = {
            frame: [binding for binding in bindings if frame in binding["frames"]]
            for frame in all_frames
        }

        for frame in all_frames:
            integer_frame = math.floor(frame)
            context.scene.frame_set(integer_frame, subframe=frame - integer_frame)
            for binding in bindings_by_frame[frame]:
                source = binding["source"]
                if binding["has_location"]:
                    location = _game_location(source.location)
                    if binding["relative_location"]:
                        location += binding["rest_location"]
                else:
                    location = binding["rest_location"]
                rotation = _game_rotation(source.rotation_quaternion) if binding["has_rotation"] else binding["rest_rotation"]
                animated_local = Matrix.LocRotScale(location, rotation, binding["rest_scale"])
                basis_location, basis_rotation, basis_scale = (binding["rest_local_inverse"] @ animated_local).decompose()

                samples = binding["samples"]
                if samples and samples[-1][2].dot(basis_rotation) < 0.0:
                    basis_rotation.negate()
                samples.append((frame, basis_location, basis_rotation, basis_scale))

        bound_armatures = sorted(
            {binding["armature"] for binding in bindings},
            key=lambda armature: armature.name,
        )
        fcurves_by_armature = {
            armature: _new_action(armature, f"{animation_name} [{armature.name}]")
            for armature in bound_armatures
        }
        for binding in bindings:
            pose_bone = binding["pose_bone"]
            pose_bone.rotation_mode = "QUATERNION"
            samples = binding["samples"]
            fcurves = fcurves_by_armature[binding["armature"]]
            location_path = pose_bone.path_from_id("location")
            rotation_path = pose_bone.path_from_id("rotation_quaternion")
            scale_path = pose_bone.path_from_id("scale")

            for component in range(3):
                component_samples = [
                    (frame, location[component])
                    for frame, location, _rotation, _scale in samples
                ]
                _add_fcurve(fcurves, location_path, component, component_samples)
            for component in range(4):
                component_samples = [
                    (frame, rotation[component])
                    for frame, _location, rotation, _scale in samples
                ]
                _add_fcurve(fcurves, rotation_path, component, component_samples)
            for component in range(3):
                component_samples = [
                    (frame, scale[component])
                    for frame, _location, _rotation, scale in samples
                ]
                _add_fcurve(fcurves, scale_path, component, component_samples)

        _clear_secondary_attachments(target_armatures)
        attachment_count = 0
        if settings.attach_weapon_models:
            attachment_count = _attach_secondary_armatures(target_armatures)

        context.scene.frame_start = math.floor(all_frames[0])
        context.scene.frame_end = math.ceil(all_frames[-1])
        restored_frame = max(context.scene.frame_start, min(context.scene.frame_end, current_frame))
        context.scene.frame_set(math.floor(restored_frame), subframe=restored_frame - math.floor(restored_frame))

        if settings.hide_source_nodes:
            source_root.hide_set(True)
            source_root.hide_render = True

        animated_sources = {
            obj
            for obj in source_objects
            if obj.animation_data is not None and obj.animation_data.action is not None
        }
        unmatched_count = len(animated_sources - mapped_sources)
        self.report(
            {"INFO"},
            f"Baked {len(mapped_sources)} XAnim nodes to {len(bound_armatures)} armature(s); "
            f"{unmatched_count} animated nodes unmatched; "
            f"{attachment_count} weapon model(s) attached",
        )
        return {"FINISHED"}


class OAT_PT_xanim_preview(Panel):
    bl_label = "XAnim Preview"
    bl_idname = "OAT_PT_xanim_preview"
    bl_space_type = "VIEW_3D"
    bl_region_type = "UI"
    bl_category = "OAT"

    def draw(self, context):
        layout = self.layout
        settings = context.scene.oat_xanim_preview
        layout.prop(settings, "source_root")
        layout.prop(settings, "hide_source_nodes")
        layout.prop(settings, "attach_weapon_models")
        layout.label(text="Select the target XModel armature(s).")
        layout.operator(OAT_OT_bake_xanim_preview.bl_idname, icon="ACTION")


CLASSES = (
    OATXAnimPreviewSettings,
    OAT_OT_bake_xanim_preview,
    OAT_PT_xanim_preview,
)


def register():
    for cls in CLASSES:
        bpy.utils.register_class(cls)
    bpy.types.Scene.oat_xanim_preview = PointerProperty(type=OATXAnimPreviewSettings)


def unregister():
    del bpy.types.Scene.oat_xanim_preview
    for cls in reversed(CLASSES):
        bpy.utils.unregister_class(cls)


if __name__ == "__main__":
    register()
