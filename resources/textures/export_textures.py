import imageio.v3 as imageio
import glob
import os

output_file = open("textures.h", "w")
output_file.truncate(0)

files = glob.glob("*.png")
for file in files:
    image = imageio.imread(file)
    name = os.path.splitext(file)[0]
    (height, width, channels) = image.shape
    output_file.write("#ifdef GINT\nstatic\n#endif\nunsigned short " + name + "_pixels[] = {\n")
    for row in image:
        for pixel in row:
          color = ((pixel[0] >> 3) << (6+5)) | ((pixel[1] >> 2) << 5) | (pixel[2] >> 3)
          output_file.write("0x{:04X},".format(color))
        output_file.write("\n")
    output_file.write("};\n")
    output_file.write("#ifdef GINT\nstatic\n#endif\nRasterizer::Texture {0}_texture = {{{0}_pixels, {1}, {2}}};\n".format(name, width, height))

output_file.close()
