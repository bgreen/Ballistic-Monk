#include "static.h"
#include "character.h"
#include "math.h"

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
	// TODO: this should be more based on hitboxes than item dimensions
	// TODO: this is all ugly as fuck
	struct mv_vector {
		// current position
		float x, y;
		// distance to get here
		float dx, dy;
		bool top, bottom, left, right;
	} path;
	// Make sure the object is a movable character (it really should be)
	Character* o = dynamic_cast<Character*>(other);
	if(o != NULL) {
		// correct the simplest and most common perpendicular collisions
		// vertical:
		if(o->xVel == 0) {
			// from the top:
			if(o->yVel > 0) {
				o->position.y = position.y - o->hitboxes.front().h;
			// from the bottom:
			} else {
				o->position.y = position.y + hitboxes.front().h;
			}
			return;
		} 
		// horizontal:
		if (o->yVel == 0) {
			// from the left:
			if(o->xVel > 0) {
				o->position.x = position.x - o->hitboxes.front().w;
			// from the right:
			} else {
				o->position.x = position.x + hitboxes.front().w;
			}
			return;
		}
		
		// calculate the line representing the movement of the object
		path.x = o->position.x;
		path.y = o->position.y;
		path.dx = o->speed * o->xVel;
		path.dy = o->speed * o->yVel;
		path.top = true;
		path.left = true;
		// the upper and lower bounds for the movement vector
		float upper_bound, lower_bound;
		if(o->yVel > 0) {
			path.y += o->hitboxes.front().h;
			path.top = false;
			upper_bound = path.y;
			lower_bound = path.y - path.dy;
		} else {
			upper_bound = path.y - path.dy;
			lower_bound = path.y;
		}
		if(o->xVel > 0) {
			path.x += o->hitboxes.front().w;
			path.left = false;
		}
		enum side_enum {top = 0, bottom = 1, left = 2, right = 3};
		// check which movement vectors intersect with the hitbox and where
		mv_vector hit_vector;
		float max_dist = 0;
		int hit_where = NULL;
		// general case:
		// derive b (y = mx + b) using a line and a point
		float m = path.dy/path.dx;
		float b = path.y - m * path.x;
		// determine where this line intersects
		for(int side=0; side<4; side++) {
			if(side == left && path.left) continue;
			if(side == right && !path.left) continue;
			if(side == top && path.top) continue;
			if(side == bottom && !path.top) continue;
			
			float x, y;
			switch(side) {
				case top:
					y = position.y;
					x = (y - b) / m;
					break;
				case bottom:
					y = (position.y + hitboxes.front().h);
					x = (y - b) / m;
					break;
				case left:
					x = position.x;
					y = m * x + b;
					break;
				case right:
					x = (position.x + hitboxes.front().w);
					y = m * x + b;
					break;
			}
			if((lower_bound <= y) && (y <= upper_bound)) {
				float dist = sqrt(pow(x - path.x, 2) + pow(y - path.y, 2));
				if(dist > max_dist) {
					max_dist = dist;
					hit_vector = path;
					hit_where = side;
				}
			}
		}
		
		
		//printf("diag: 0x%X\n", hit_where); fflush(stdout);
		float mag = max_dist / sqrt(pow(hit_vector.dx, 2) + pow(hit_vector.dy, 2));
		if(hit_where == top || hit_where == bottom) {
			o->position.y -= mag * hit_vector.dy;
		} else if(hit_where == left || hit_where == right) {
			o->position.x -= mag * hit_vector.dx;
		}
		
	} else {
		fprintf(stderr, "Error: somehow two immovable objects collided");
	}
	
	return;
}
