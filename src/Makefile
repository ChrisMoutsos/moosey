IDIR=../include
CC=g++
CFLAGS=-std=c++11 -c -Wall -I$(IDIR)
SOURCES=main.cpp display.cpp board.cpp input.cpp move.cpp movegen.cpp legal.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=moosey 

all: $(SOURCES) $(EXECUTABLE) 

$(EXECUTABLE): $(OBJECTS)
	$(CC)  $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *o $(EXECUTABLE)
