import bpy

def mat_texture(material):
    v = material.get("texture")
    if v == None:
        return "nullptr"
    else:
        return "&" + v + "_texture"

def mat_color(material):
    v = material.get("color")
    if v == None:
        return "{1,1,1}"
    else:
        return "{%.6f, %.6f, %.6f}" % (v[0], v[1], v[2])

def mat_isShaded(material):
    v = material.get("isShaded")
    if v == None:
        return "true"
    else:
        return v

file = open("models.h", "w")
file.truncate(0)
for object in bpy.data.objects:
    if object.type == 'MESH':
        file.write("Renderer::Material " + object.name + "_materials[" + str(len(object.data.materials)) + "] = {\n")
        for material in object.data.materials:
            file.write("{%s, %s, %s},\n" % (mat_texture(material), mat_color(material), mat_isShaded(material)))
        file.write("};\n")

        file.write("Renderer::Triangle " + object.name + "_triangles[" + str(len(object.data.polygons)) + "] = {\n")
        for polygon in object.data.polygons:
            positions = []
            texCoords = []
            normal = polygon.normal
            material = polygon.material_index
            for vertex in polygon.vertices:
                   positions.append(object.data.vertices[vertex].co)

            for vert_idx, loop_idx in zip(polygon.vertices, polygon.loop_indices):
                texCoords.append(object.data.uv_layers.active.data[loop_idx].uv)

            file.write("{")

            file.write("{")
            for i in range(3):
                file.write("{{%.6f,%.6f,%.6f},{%.6f,%.6f}}," % (positions[i].x, -positions[i].z, positions[i].y, texCoords[i].x, 1-texCoords[i].y))
            file.write("},")

            file.write("{%.6f, %.6f, %.6f}," % (normal.x, -normal.z, normal.y))

            file.write("%d," % material)

            file.write("},\n")
            # color = object.data.materials[polygon.material_index].node_tree.nodes["Principled BSDF"].inputs["Base Color"].default_value
        file.write("};\n")

        file.write("Renderer::Mesh %s_mesh = {%d, %s_triangles};\n" % (object.name, len(object.data.polygons), object.name))
file.close()
