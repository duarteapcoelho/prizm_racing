../../src/textures.h: textures.h
	cp textures.h ../../src/textures.h

textures.h: car.png export_textures.py
	python export_textures.py

clean:
	rm -f ../../src/textures.h
