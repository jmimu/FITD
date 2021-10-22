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

import random 
from struct import pack

float2intScale = 1000

def float2int(v):
    return int(float2intScale*v)

def float2int_array(array):
    return [float2int(v) for v in array]

def save_mesh(filepath, mesh, bbox):
    import bpy

    mesh_verts = mesh.vertices
    # vdict = {} # (index, normal, uv) -> new index
    vdict = [{} for i in range(len(mesh_verts))]
    body_verts = []
    body_faces = [[] for f in range(len(mesh.polygons))]
    vert_count = 0

    for i, f in enumerate(mesh.polygons):

        col = [
            [0.5, 0.5, 0.5, 0.5]
            for l in range(f.loop_start, f.loop_start + f.loop_total)
        ]

        pf = body_faces[i]
        for j, vidx in enumerate(f.vertices):
            v = mesh_verts[vidx]

            color = col[j]
            color = (
                int(color[0] * 255.0),
                int(color[1] * 255.0),
                int(color[2] * 255.0),
                int(color[3] * 255.0),
            )
            key = color

            vdict_local = vdict[vidx]
            pf_vidx = vdict_local.get(key)  # Will be None initially

            if pf_vidx is None:  # Same as vdict_local.has_key(key)
                pf_vidx = vdict_local[key] = vert_count
                body_verts.append((vidx, color))
                vert_count += 1

            pf.append(pf_vidx)

    with open(filepath, "wb") as file:
        fw = file.write
        # flags
        fw(pack("<h", 0))
        
        # bounding box
        fw(pack("<6h", *bbox))
        
        # offset
        fw(pack("<h", 0))
        
        # Vertex data (max 400?)
        fw(pack("<h", len(body_verts)))
        for index, color in body_verts:
            fw(pack("<h", float2int(mesh_verts[index].co[0])))
            fw(pack("<h", float2int(-mesh_verts[index].co[2])))
            fw(pack("<h", float2int(-mesh_verts[index].co[1])))
            print("vert ", float2int_array(mesh_verts[index].co))

        # No bones
        
        # Face data
        fw(pack("<h", len(mesh.polygons)))
        for pf in body_faces:
            fw(pack("<B", 1)) # polygon
            length = len(pf)
            fw(pack("<B", length))
            fw(pack("<B", 0)) # poly type
            fw(pack("<B", 16+random.randrange(8))) # color
            fw(pack("<%dH" % length, *[6*v for v in pf]))


def save(
    context,
    filepath=""
):
    import time
    import bpy
    import bmesh

    t = time.time()

    if bpy.ops.object.mode_set.poll():
        bpy.ops.object.mode_set(mode='OBJECT')

    obs = context.selected_objects

    depsgraph = context.evaluated_depsgraph_get()
    bm = bmesh.new()

    bbox = [0,0,0,0,0,0]
# TODO: apply transform to bbox, compute min max after
    for ob in obs:
        try:
            me = ob.to_mesh()
        except RuntimeError:
            continue

        me.transform(ob.matrix_world)
        bm.from_mesh(me)
        ob.to_mesh_clear()
        
        print("object bbox: ", ob.bound_box)
        for v in ob.bound_box:
            if v[0]<bbox[0]: bbox[0] = v[0]
            if v[0]>bbox[1]: bbox[1] = v[0]
            if -v[2]<bbox[2]: bbox[2] = -v[2]
            if -v[2]>bbox[3]: bbox[3] = -v[2]
            if -v[1]<bbox[4]: bbox[4] = -v[1]
            if -v[1]>bbox[5]: bbox[5] = -v[1]

    bbox_int = float2int_array(bbox)
    print("final bbox: ", bbox)
    mesh = bpy.data.meshes.new("TMP BODY EXPORT")
    bm.to_mesh(mesh)
    bm.free()

    save_mesh(filepath, mesh, bbox_int)

    bpy.data.meshes.remove(mesh)

    t_delta = time.time() - t
    print(f"Export completed {filepath!r} in {t_delta:.3f}")
