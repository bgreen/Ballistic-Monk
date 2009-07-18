
#include <stdio.h>

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

/*
 * This defines what effects the wall will apply to something with which it collides,
 * specifically moving it back to its outside edge. 
 * It starts by detecting if the collision was perpendicular to the surface, which
 * should be the most common, and simply moving it backwards.
 * If that is not the case, it must decide which side the object hit and move it back
 * appropriately. A line (path) is drawn from the old position to the new position, connecting
 * the vertex of the hitbox that is in the direction the pox moved (If the box moved down and to
 * the left, the bottom left vertex of the before and after positions would be connected).
 * This line is checked to see which side of the wall it intersects first, and consequently
 * how far back to move the object.
 */

void Wall::collide(Object* other, double dt) {

	struct mv_vector {
		// current position
		double x, y;
		// distance to get here
		double dx, dy;
		bool top, bottom, left, right;
	} path;
	
	// Make sure the object is a movable character (it really should be)
	Character* o = dynamic_cast<Character*>(other);
	if(o != NULL) {
		// correct the simplest and most common perpendicular collisions
		// vertical:
		if(o->vel.x == 0) {
			// from the top:
			if(o->vel.y < 0) {
				o->position.y = position.y + o->hitboxes.front().h;
			// from the bottom:
			} else {
				o->position.y = position.y - hitboxes.front().h;
			}
			o->vel.y = 0;
			return;
		} 
		// horizontal:
		if (o->vel.y == 0) {
			// from the left:
			if(o->vel.x > 0) {
				o->position.x = position.x - o->hitboxes.front().w;
			// from the right:
			} else {
				o->position.x = position.x + hitboxes.front().w;
			}
			o->vel.x = 0;
			return;
		}
		
		// calculate the line representing the movement of the object
		path.x = o->position.x;
		path.y = o->position.y;
		path.dx = o->vel.x * dt;
		path.dy = o->vel.y * dt;
		path.top = true;
		path.left = true;
		// the upper and lower bounds for the movement vector
		double upper_bound, lower_bound;
		if(o->vel.y < 0) {
			path.y -= (o->hitboxes.front().h - 1);
			path.top = false;
			upper_bound = path.y - path.dy;
			lower_bound = path.y;
		} else {
			upper_bound = path.y;
			lower_bound = path.y - path.dy;
		}
		if(o->vel.x > 0) {
			path.x += (o->hitboxes.front().w - 1);
			path.left = false;
		}
		
		// check which movement vectors intersect with the hitbox and where
		mv_vector hit_vector;
		float max_dist = 0;
		int hit_where = NULL;
		
		// derive b (y = mx + b) using a line and a point
		double m = path.dy/path.dx;
		double b = path.y - m * path.x;
		
		// determine where this line intersects
		enum side_enum {top = 0, bottom = 1, left = 2, right = 3};
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
					y = (position.y - (hitboxes.front().h - 1));
					x = (y - b) / m;
					break;
				case left:
					x = position.x;
					y = m * x + b;
					break;
				case right:
					x = (position.x + (hitboxes.front().w - 1));
					y = m * x + b;
					break;
			}
			if((lower_bound <= y) && (y <= upper_bound)) {
				float dist_squared = pow(x - path.x, 2) + pow(y - path.y, 2);
				if(dist_squared > max_dist) {
					max_dist = dist_squared;
					hit_vector = path;
					hit_where = side;
				}
			}
		}
		
		// compute how far to move the object back
		if(hit_where == top) {
			o->position.y = position.y + o->hitboxes.front().h;
			o->vel.y = 0;
		} else if(hit_where == bottom) {
			o->position.y = position.y - hitboxes.front().h;
			o->vel.y = 0;
		} else if(hit_where == left) {
			o->position.x = position.x - o->hitboxes.front().w;
			o->vel.x = 0;
		} else if(hit_where == right) {
			o->position.x = position.x + hitboxes.front().w;
			o->vel.x = 0;
		}
		
	} else {
		fprintf(stderr, "Error: somehow two immovable objects collided");
	}
	
	return;
}
