CC = g++
LINKS = -lSDL -lSDL_image -lSDL_ttf

OBS = BM.o character.o fps_counter.o object.o static.o
HEADERS = character.h fps_counter.h object.h static.h

all: BM

BM: $(OBS)
	$(CC) $(OBS) -o BM $(LINKS)

.cc.o:
	$(CC) -c $< -o $@

BM.o: BM.cc character.h fps_counter.h
character.o: character.cc character.h object.h
fps_counter.o: fps_counter.cc fps_counter.h
object.o: object.cc object.h
static.o: static.cc static.h

clean:
	rm *.o BM
