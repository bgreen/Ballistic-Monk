#include <stdlib.h>
#include <queue>
#include <SDL/SDL.h>

#ifndef INC_OBJECT
#define INC_OBJECT
#include "object.h"
#endif

class Character: public Object {
	public:
	float mass;
	PVector weight;
	PVector force;
	PVector vel;

	Character();
	
	int hit_where(Object* other, double dt);
	
	void handle_move(double dt);
	
};


class Player: public Character {
	
	public:
	Player();
	
	std::queue<SDL_Event> input;
	
	void collide(Object* other, double dt);
	void handle_input();
};
