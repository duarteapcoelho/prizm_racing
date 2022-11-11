all: sdl prizm

sdl: sdl/racing

prizm: prizm/racing.g3a

clean:
	make $(MFLAGS) -C sdl/ clean
	make $(MFLAGS) -C prizm/ clean

sdl/racing:
	make $(MFLAGS) -C sdl/

prizm/racing.g3a:
	make $(MFLAGS) -C prizm/
