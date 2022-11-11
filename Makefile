CC = g++
CFLAGS += -Wall -Wextra
CFLAGS += -DSDL
LIB += -lSDL2 -lSDL2_ttf
LDFLAGS = $(LIB)
INCLUDES =

SRCDIR = src

SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%,build_sdl/%,$(SOURCES:.cpp=.o))
DEPS = $(patsubst $(SRCDIR)/%,build_sdl/%,$(SOURCES:.cpp=.d))

TARGET = racing

all: debug

debug: CFLAGS += -Og -g
debug: $(TARGET)

release: CFLAGS += -Ofast
release: $(TARGET)

profile-generate: CFLAGS += -fprofile-generate
profile-generate: LDFLAGS += -fprofile-generate
profile-generate: release

profile-use: CFLAGS += -fprofile-use=profile
profile-use: LDFLAGS += -fprofile-use=profile -lgcov
profile-use: release

$(TARGET): $(OBJECTS)
	@echo "Linking..."
	$(CC) $^ -o $(TARGET) $(LIB)

build_sdl/%.o: $(SRCDIR)/%.cpp
	mkdir -p build_sdl
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ -MMD $< -MF "build_sdl/$(patsubst $(SRCDIR)/%,%,$(<:.cpp=.d))"

clean:
	rm $(TARGET)
	rm build_sdl -r

.PHONY: clean

-include $(DEPS)
