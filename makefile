OBJS = main.o gamemap.o vec2i.o mapspec.o
CC = clang++
CFLAGS = -Wall -c -std=c++11 $(DEBUG)
LFLAGS = -Wall -std=c++11 $(DEBUG)
EXE = run_this

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $@

main.o: main.cc
	$(CC) $(CFLAGS) $< -o $@
gamemap.o: gamemap.cc
	$(CC) $(CFLAGS) $< -o $@
mapspec.o: mapspec.cc
	$(CC) $(CFLAGS) $< -o $@
vec2i.o: vec2i.cc
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o && rm $(EXE)
