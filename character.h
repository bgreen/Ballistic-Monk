#ifndef _INC_CHARACTER
#define _INC_CHARACTER

#include <stdlib.h>
#include <queue>
#include <SDL/SDL.h>

#include "object.h"
#include "physics.h"

class Character: public Object {
	public:
	float mass;
	PVector weight;
	PVector force;
	PVector vel;
	bool mod_flag;
	int ID;

	Character();
	Character(float x, float y);
	
	int hit_where(Object* other, double dt);
	void collide(Object* other, double dt);
	
	void handle_move(double dt);
	
};


class Player: public Character {
	
	public:
	Player();
	
	std::queue<SDL_Event> input;
	
	//void collide(Object* other, double dt);
	void handle_input();
};

#endif
