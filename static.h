#ifndef INC_OBJECT
#define INC_OBJECT
#include "object.h"
#endif

class Wall: public Object {
	public:
	Wall(SDL_Rect dim);
	void collide(Object* other, double dt);
};
