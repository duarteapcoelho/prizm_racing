CC = sh-elf-g++
CFLAGS += -Wall -Wextra -Ofast -funroll-loops -DGINT
CFLAGS += -DFXCG50 -DTARGET_FXCG50 -m4-nofpu -mb -ffreestanding -nostdlib -fstrict-volatile-bitfields
LDFLAGS = -m4-nofpu -mb -nostdlib -Wl,--no-warn-rwx-segments -T fxcg50.ld -lgint-cg -lc -lgcc -lgint-cg -lprof-cg

INCLUDES =

SRCDIR = ../src

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%,%,$(SOURCES:.cpp=.o))
DEPS = $(patsubst $(SRCDIR)/%,%,$(SOURCES:.cpp=.d))

racing_singleplayer.g3a: racing.bin
	mkg3a racing.bin racing_singleplayer.g3a -n basic:racing_singleplayer -i uns:../resources/icons/unselected.bmp -i sel:../resources/icons/selected.bmp

racing.bin: racing
	sh-elf-objcopy -O binary -R .bss -R .gint_bss racing racing.bin

racing: $(OBJECTS)
	@echo "Linking..."
	$(CC) $^ -o racing $(LDFLAGS)

%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ -MMD $< -MF "$(patsubst $(SRCDIR)/%,%,$(<:.cpp=.d))"

clean:
	rm racing_singleplayer.g3a racing.bin racing $(OBJECTS) $(DEPS) -f

.PHONY: clean

-include $(DEPS)
