../../src/models.h: models.h
	cp models.h ../../src/models.h

models.h: models.blend export_models.py
	blender $(CURDIR)/models.blend -b -P $(CURDIR)/export_models.py

clean:
	rm -f ../../src/models.h
