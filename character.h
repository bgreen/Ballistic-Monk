#include <stdlib.h>
#include <queue>
#include <SDL/SDL.h>

class Character {
	protected:
	int x, y;
	int xVel, yVel;
	int speed;
	SDL_Surface *sprite;
	
	public:
	
	Character();
	
	void handle_move();
	void handle_show(SDL_Surface* screen);
};


class Player: Character {
	
	public:
	
	std::queue<SDL_Event> input;
	
	void handle_input();
	void handle_move();
	void handle_show(SDL_Surface* screen);
};
