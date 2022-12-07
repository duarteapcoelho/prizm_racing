SOURCES = $(wildcard src/*)

all: sdl prizm gint

sdl: sdl/racing

prizm: prizm/racing.g3a

gint: gint/racing_singleplayer.g3a

clean:
	make $(MFLAGS) -C sdl/ clean
	make $(MFLAGS) -C prizm/ clean
	make $(MFLAGS) -C gint/ clean

sdl/racing: $(SOURCES)
	make $(MFLAGS) -C sdl/

prizm/racing.g3a: $(SOURCES)
	make $(MFLAGS) -C prizm/

gint/racing_singleplayer.g3a: $(SOURCES)
	make $(MFLAGS) -C gint/

package: release/racing.zip release/racing.tar.gz release/racing_singleplayer.zip release/racing_singleplayer.tar.gz

release/racing.zip: prizm/racing.g3a
	mkdir -p release
	zip -j release/racing.zip LICENSE prizm/racing.g3a

release/racing.tar.gz: prizm/racing.g3a
	mkdir -p release
	tar czvf release/racing.tar.gz LICENSE -C prizm racing.g3a

release/racing_singleplayer.zip: gint/racing_singleplayer.g3a
	mkdir -p release
	zip -j release/racing_singleplayer.zip LICENSE gint/racing_singleplayer.g3a

release/racing_singleplayer.tar.gz: gint/racing_singleplayer.g3a
	mkdir -p release
	tar czvf release/racing_singleplayer.tar.gz LICENSE -C gint racing_singleplayer.g3a

package-clean:
	rm release -rf
