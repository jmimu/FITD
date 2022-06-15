# ##### BEGIN GPL LICENSE BLOCK #####
#
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU General Public License
#  as published by the Free Software Foundation; either version 2
#  of the License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software Foundation,
#  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#
# ##### END GPL LICENSE BLOCK #####

# <pep8-80 compliant>

bl_info = {
    "name": "Alone in the Dark body format",
    "author": "jmimu",
    "version": (1, 0),
    "blender": (2, 80, 0),
    "location": "File > Import/Export > Alone Body",
    "description": "Import-Export: Alone in the Dark body",
    "doc_url": "jmimu.free.fr",
    "category": "Import-Export",
}

# Copyright (C) 2021: jmimu@free.fr

if "bpy" in locals():
    import importlib
    if "export_body" in locals():
        importlib.reload(export_body)
    if "import_body" in locals():
        importlib.reload(import_body)


import bpy
from bpy.props import (
    CollectionProperty,
    StringProperty,
    BoolProperty,
    FloatProperty,
)
from bpy_extras.io_utils import (
    ImportHelper,
    ExportHelper,
    axis_conversion,
    orientation_helper,
)

# Todo Import Alone palette : 256 materials with different base color 
class ImportBody(bpy.types.Operator, ImportHelper):
    """Load a Body geometry file"""
    bl_idname = "import_mesh.dat"
    bl_label = "Import Alone Body"
    bl_options = {'UNDO'}

    files: CollectionProperty(
        name="File Path",
        description="File path used for importing the Alone Body file",
        type=bpy.types.OperatorFileListElement,
    )

    # Hide opertator properties, rest of this is managed in C. See WM_operator_properties_filesel().
    hide_props_region: BoolProperty(
        name="Hide Operator Properties",
        description="Collapse the region displaying the operator settings",
        default=True,
    )

    directory: StringProperty()

    filename_ext = ".dat"
    filter_glob: StringProperty(default="*.dat", options={'HIDDEN'})

    def execute(self, context):
        import os
        from . import import_body

        context.window.cursor_set('WAIT')

        paths = [
            os.path.join(self.directory, name.name)
            for name in self.files
        ]

        if not paths:
            paths.append(self.filepath)

        for path in paths:
            import_body.load(self, context, path)

        context.window.cursor_set('DEFAULT')

        return {'FINISHED'}


class ExportBody(bpy.types.Operator, ExportHelper):
    bl_idname = "export_mesh.dat"
    bl_label = "Export Alone Body"
    bl_description = "Export mesh as a Alone Body"

    filename_ext = ".dat"
    filter_glob: StringProperty(default="*.dat", options={'HIDDEN'})

    # ~ use_selection: BoolProperty(
        # ~ name="Selection Only",
        # ~ description="Export selected objects only",
        # ~ default=False,
    # ~ )
    
    global_scale: FloatProperty(
        name="Scale",
        min=0.01,
        max=1000.0,
        default=1.0,
    )

    def execute(self, context):
        from mathutils import Matrix
        from . import export_body

        context.window.cursor_set('WAIT')

        keywords = self.as_keywords(
            ignore=(
                "global_scale",
                "check_existing",
                "filter_glob",
            )
        )
        export_body.save(context, **keywords)

        context.window.cursor_set('DEFAULT')

        return {'FINISHED'}

    def draw(self, context):
        layout = self.layout
        layout.use_property_split = True
        layout.use_property_decorate = False

        sfile = context.space_data
        operator = sfile.active_operator


class BODY_PT_export_include(bpy.types.Panel):
    bl_space_type = 'FILE_BROWSER'
    bl_region_type = 'TOOL_PROPS'
    bl_label = "Include"
    bl_parent_id = "FILE_PT_operator"

    @classmethod
    def poll(cls, context):
        sfile = context.space_data
        operator = sfile.active_operator

        return operator.bl_idname == "EXPORT_MESH_OT_body"

    def draw(self, context):
        layout = self.layout
        layout.use_property_split = True
        layout.use_property_decorate = False

        sfile = context.space_data
        operator = sfile.active_operator

        # ~ layout.prop(operator, "use_selection")


class BODY_PT_export_transform(bpy.types.Panel):
    bl_space_type = 'FILE_BROWSER'
    bl_region_type = 'TOOL_PROPS'
    bl_label = "Transform"
    bl_parent_id = "FILE_PT_operator"

    @classmethod
    def poll(cls, context):
        sfile = context.space_data
        operator = sfile.active_operator

        return operator.bl_idname == "EXPORT_MESH_OT_body"

    def draw(self, context):
        layout = self.layout
        layout.use_property_split = True
        layout.use_property_decorate = False

        sfile = context.space_data
        operator = sfile.active_operator

        layout.prop(operator, "global_scale")


class BODY_PT_export_geometry(bpy.types.Panel):
    bl_space_type = 'FILE_BROWSER'
    bl_region_type = 'TOOL_PROPS'
    bl_label = "Geometry"
    bl_parent_id = "FILE_PT_operator"

    @classmethod
    def poll(cls, context):
        sfile = context.space_data
        operator = sfile.active_operator

        return operator.bl_idname == "EXPORT_MESH_OT_body"

    def draw(self, context):
        layout = self.layout
        layout.use_property_split = True
        layout.use_property_decorate = False

        sfile = context.space_data
        operator = sfile.active_operator


def menu_func_import(self, context):
    self.layout.operator(ImportBody.bl_idname, text="Alone Body (.dat)")


def menu_func_export(self, context):
    self.layout.operator(ExportBody.bl_idname, text="Alone Body (.dat)")


classes = (
    ImportBody,
    ExportBody,
    BODY_PT_export_include,
    BODY_PT_export_transform,
    BODY_PT_export_geometry,
)


def register():
    for cls in classes:
        bpy.utils.register_class(cls)

    #bpy.types.TOPBAR_MT_file_import.append(menu_func_import) #import does not work for now
    bpy.types.TOPBAR_MT_file_export.append(menu_func_export)


def unregister():
    for cls in classes:
        bpy.utils.unregister_class(cls)

    #bpy.types.TOPBAR_MT_file_import.remove(menu_func_import) #import does not work for now
    bpy.types.TOPBAR_MT_file_export.remove(menu_func_export)


if __name__ == "__main__":
    register()
