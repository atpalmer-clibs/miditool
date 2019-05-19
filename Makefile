P=miditool
OBJECTS=miditool.o track.o typehelp.o

$(P): $(OBJECTS)

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
