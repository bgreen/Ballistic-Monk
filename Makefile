CC = g++
LINKS = -lSDL -lSDL_image -lSDL_ttf

SRC = BM.cc character.cc fps_counter.cc
HEADERS = character.h fps_counter.h
all: BM

#BM: BM.o character.o fps_counter.o
#	$(CC) BM.o character.o -o BM $(LINKS)

BM: $(SRC) $(HEADERS)
	$(CC) $(SRC) -o BM $(LINKS)

#BM.o: BM.cc
#	$(CC) -c BM.cc

#character.o: character.cc character.h
#	$(CC) -c character.cc

clean:
	rm *.o BM
