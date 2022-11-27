SOURCES = $(wildcard src/*)

all: sdl prizm gint

sdl: sdl/racing

prizm: prizm/racing.g3a

gint: gint/racing.g3a

clean:
	make $(MFLAGS) -C sdl/ clean
	make $(MFLAGS) -C prizm/ clean
	make $(MFLAGS) -C gint/ clean

sdl/racing: $(SOURCES)
	make $(MFLAGS) -C sdl/

prizm/racing.g3a: $(SOURCES)
	make $(MFLAGS) -C prizm/

gint/racing.g3a: $(SOURCES)
	make $(MFLAGS) -C gint/
