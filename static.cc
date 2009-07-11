#include "static.h"
#include "character.h"

Wall::Wall(SDL_Rect dim) {
	position.x = dim.x;
	position.y = dim.y;
	layer = 0;
	sprite = SDL_CreateRGBSurface(SDL_SWSURFACE, dim.w, dim.h, 16, 0,0,0,0);
	SDL_FillRect(sprite, NULL, SDL_MapRGB(sprite->format, 0x80, 0x80, 0x80));
	SDL_Rect h = {0, 0, dim.w, dim.h};
	add_hitbox(h);
}

void Wall::collide(Object* other) {
	// TODO: each movement vector is assigned the same values, fix that
	// TODO: this should be more based on hitboxes than item dimensions
	// TODO: this is all ugly as fuck
	struct mv_vector {
		float x1, y1;
		float x2, y2;
	} path[4];
	// Make sure the object is a movable character (it really should be)
	Character* o = dynamic_cast<Character*>(other);
	if(o != NULL) {
		// detect the simple and most common perpendicular collisions
		// vertical:
		if(o->xVel == 0) {
			// from the top:
			if(o->yVel > 0) {
				o->position.y -= (o->position.y + o->hitboxes.front().h) - position.y;
			// from the bottom:
			} else {
				o->position.y += (position.y + hitboxes.front().h) - other->position.y;
			}
			return;
		} 
		// horizontal:
		if (o->yVel == 0) {
			// from the left:
			if(o->xVel > 0) {
				o->position.x -= (o->position.x + o->hitboxes.front().w) - position.x;
			// from the right:
			} else {
				o->position.x += (position.x + hitboxes.front().w) - other->position.x;
			}
			return;
		}
		
		// calculate the lines represented by the movement
		// of each vertex of the hitbox
		for(int i=0; i<3; i++) {
			path[i].x2 = o->position.x;
			path[i].y2 = o->position.y;
			path[i].x1 = o->position.x - o->speed * o->xVel;
			path[i].y1 = o->position.y - o->speed * o->yVel;
		}
		// check which movement vectors intersect with the hitbox and where
		for(int i=0; i<3; i++) {
			// general case:
		}
	} else {
		fprintf(stderr, "Error: somehow two immovable objects collided");
	}
	
	fflush(stdout);
	return;
}
