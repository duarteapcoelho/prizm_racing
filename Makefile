SOURCES = $(wildcard src/*)

all: sdl prizm

sdl: sdl/racing

prizm: prizm/racing.g3a

clean:
	make $(MFLAGS) -C sdl/ clean
	make $(MFLAGS) -C prizm/ clean

sdl/racing: $(SOURCES)
	make $(MFLAGS) -C sdl/

prizm/racing.g3a: $(SOURCES)
	make $(MFLAGS) -C prizm/
