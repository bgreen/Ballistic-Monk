CC = g++
LINKS = -lSDL -lSDL_image -lSDL_ttf
DEBUG = 

OBS = BM.o character.o fps_counter.o object.o static.o physics.o
HEADERS = character.h fps_counter.h object.h static.h physics.h

all: BM

BM: $(OBS)
	$(CC) $(OBS) -o BM $(LINKS) $(DEBUG)

.cc.o:
	$(CC) -c $< -o $@ $(DEBUG)

BM.o: BM.cc character.h fps_counter.h
character.o: character.cc character.h object.h
fps_counter.o: fps_counter.cc fps_counter.h
object.o: object.cc object.h
static.o: static.cc static.h
physics.o: physics.cc physics.h

clean:
	rm *.o BM
