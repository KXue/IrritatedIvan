OBJS = main.o game.o vec2i.o gamemap.o inmanager.o mapspec.o entity.o character.o pig.o useful.o decider.o healthpotion.o poisonpotion.o item.o
CC = clang++
SDL = -lSDL2 -lSDL2_ttf -lSDL2_image
CFLAGS = -Wall -c -std=c++11 $(DEBUG)
LFLAGS = -Wall -std=c++11 $(DEBUG)
EXE = run_this

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $@

main.o: main.cc
	$(CC) $(CFLAGS) $< -o $@
mapspec.o: mapspec.cc
	$(CC) $(CFLAGS) $< -o $@
inmanager.o: inmanager.cc
	$(CC) $(CFLAGS) $< -o $@
vec2i.o: vec2i.cc
	$(CC) $(CFLAGS) $< -o $@
game.o: game.cc
	$(CC) $(CFLAGS) $< -o $@
gamemap.o: gamemap.cc
	$(CC) $(CFLAGS) $< -o $@
entity.o: entity.cc
	$(CC) $(CFLAGS) $< -o $@
character.o: character.cc
	$(CC) $(CFLAGS) $< -o $@
pig.o: pig.cc
	$(CC) $(CFLAGS) $< -o $@
useful.o: useful.cc
	$(CC) $(CFLAGS) $< -o $@
decider.o: decider.cc
	$(CC) $(CFLAGS) $< -o $@
item.o: item.cc
	$(CC) $(CFLAGS) $< -o $@
healthpotion.o: healthpotion.cc
	$(CC) $(CFLAGS) $< -o $@
poisonpotion.o: poisonpotion.cc
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o && rm $(EXE)
