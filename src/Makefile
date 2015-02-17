IDIR=../include
CC=g++
CFLAGS=-std=c++11 -c -Wall -I$(IDIR)
LFLAGS=-lSDL2 -lSDL2_image -lSDL2_ttf
SOURCES=main.cpp display.cpp board.cpp input.cpp move.cpp movegen.cpp legal.cpp sdl.cpp ltexture.cpp square.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=moosey 

all: $(SOURCES) $(EXECUTABLE) 

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *o $(EXECUTABLE)
