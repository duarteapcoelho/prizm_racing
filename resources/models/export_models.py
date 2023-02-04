import bpy
file = open("models.h", "w")
file.truncate(0)
for object in bpy.data.objects:
    if object.type == 'MESH':
        file.write("Triangle " + object.name + "_triangles[" + str(len(object.data.polygons)) + "] = {\n")
        for polygon in object.data.polygons:
            file.write("\t{\n")

            file.write("\t\t{\n")
            for vertex in polygon.vertices:
                v = object.data.vertices[vertex].co
                file.write("\t\t\t{{{:.3f}, {:.3f}, {:.3f}}},\n".format(v.x, -v.z, v.y))
            file.write("\t\t},\n")

            n = polygon.normal
            file.write("\t\t{{{:.3f}, {:.3f}, {:.3f}}},\n".format(n.x, -n.z, n.y))

            color = object.data.materials[polygon.material_index].node_tree.nodes["Principled BSDF"].inputs["Base Color"].default_value
            file.write("\t\tnewColor({:d}, {:d}, {:d}),\n".format(int(color[0]*255), int(color[1]*255), int(color[2]*255)))

            file.write("\t\t{\n")
            for vert_idx, loop_idx in zip(polygon.vertices, polygon.loop_indices):
                uv_coords = object.data.uv_layers.active.data[loop_idx].uv
                file.write("\t\t\t{{{:.6f}, {:.6f}}},\n".format(uv_coords.x, 1-uv_coords.y))
            file.write("\t\t}\n")

            file.write("\t},\n")
        file.write("};\n")
file.close()
