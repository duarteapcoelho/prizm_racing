CC = g++
CFLAGS += -Wall -Wextra
CFLAGS += -DSDL
LIB += -lSDL2 -lSDL2_ttf
LDFLAGS = $(LIB)
INCLUDES =

SRCDIR = ../src

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%,%,$(SOURCES:.cpp=.o))
DEPS = $(patsubst $(SRCDIR)/%,%,$(SOURCES:.cpp=.d))

TARGET = racing

all: debug

debug: CFLAGS += -Og -g
debug: $(TARGET)

release: CFLAGS += -Ofast
release: $(TARGET)

$(TARGET): $(OBJECTS)
	@echo "Linking..."
	$(CC) $^ -o $(TARGET) $(LIB)

%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ -MMD $< -MF "$(patsubst $(SRCDIR)/%,%,$(<:.cpp=.d))"

clean:
	rm $(TARGET) $(OBJECTS) $(DEPS) -f

.PHONY: clean

-include $(DEPS)
