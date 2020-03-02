P=miditool
OBJECTS=main.o bytebuff.o track.o typehelp.o
CC=gcc
CFLAGS=-g -O0 -Wall -Wextra -pedantic

$(P): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(P) $(OBJECTS)

.PHONY: run
run:
	./miditool

.PHONY: play
play:
	timidity out.mid

.PHONY: clean
clean:
	rm $(OBJECTS) $(P)

all: clean $(P) run play
