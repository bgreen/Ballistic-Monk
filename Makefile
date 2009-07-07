CC = g++
LINKS = -lSDL -lSDL_image -lSDL_ttf

all: BM

BM: BM.o character.o
	$(CC) BM.o character.o -o BM $(LINKS)

.c.o:
	$(CC) -c $<
	
