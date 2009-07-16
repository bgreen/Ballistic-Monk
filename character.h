#include <stdlib.h>
#include <queue>
#include <SDL/SDL.h>

#ifndef INC_OBJECT
#define INC_OBJECT
#include "object.h"
#endif

class Character: public Object {
	public:
	int speed;
	float xVel, yVel;
	struct coord before;

	Character();
	
	int hit_where(Object* other);
	
	virtual void handle_move() {
		return;
	}
	
	char* pos_str(char* buffer) {
		sprintf(buffer, "%3f, %3f @ %2f, %2f\n", position.x, position.y, xVel, yVel);
		return buffer;
	}
};


class Player: public Character {
	
	public:
	Player();
	
	std::queue<SDL_Event> input;
	
	void collide(Object* other);
	void handle_input();
	void handle_move();
};
