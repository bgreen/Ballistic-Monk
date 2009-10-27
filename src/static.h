#ifndef _INC_STATIC
#define _INC_STATIC

#include "object.h"

class Wall: public Object {
	public:
	Wall(SDL_Rect dim);
	void collide(Object* other, double dt);
};

#endif
