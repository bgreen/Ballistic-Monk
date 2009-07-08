#include <stdlib.h>
#include <queue>
#include <SDL/SDL.h>
#include <vector>

class Character {
	protected:
	int x, y;
	int xVel, yVel;
	int speed;
	SDL_Surface* sprite;
	std::vector<SDL_Rect> hitboxes;
	public:
	
	Character();
	
	virtual void handle_move() =0;
	virtual void handle_show(SDL_Surface*) =0;
};


class Player: Character {
	
	public:
	
	std::queue<SDL_Event> input;
	
	void handle_input();
	void handle_move();
	void handle_show(SDL_Surface*);
};
