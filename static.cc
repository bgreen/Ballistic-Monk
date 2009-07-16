#include "static.h"
#include "character.h"
#include "math.h"

#define HIT_TOP (0b0001)
#define HIT_BOTTOM (0b0010)
#define HIT_LEFT (0b0100)
#define HIT_RIGHT (0b1000)

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
	} path[4];
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
		
		// calculate the lines represented by the movement
		// of each vertex of the hitbox
		//path[0] = {o->position.x, o->position.y, o->speed * o->xVel, o->speed * o->yVel};
		path[0].x = o->position.x;
		path[0].y = o->position.y;
		path[0].dx = o->speed * o->xVel;
		path[0].dy = o->speed * o->yVel;
		path[0].top = true;
		path[0].left = true;
		path[1] = path[0];
		path[1].x += o->hitboxes.front().w;
		path[1].left = false;
		path[2] = path[1];
		path[2].y += o->hitboxes.front().h;
		path[2].top = false;
		path[3] = path[2];
		path[3].x -= o->hitboxes.front().w;
		path[3].left = true;
		
		// check which movement vectors intersect with the hitbox and where
		mv_vector hit_vector;
		float max_dist = 0;
		int hit_where = NULL;
		for(int i=0; i<4; i++) {
			//printf("%d:\nx=%03f;y=%03f;dx=%03f;dy=%03f\n", i, path[i].x, path[i].y, path[i].dx, path[i].dy);
			// we need to define the upper and lower bound for the
			// intersection point, so we know whether it is on the
			// mv_vector or not
			float upper_bound, lower_bound;
			if(path[i].dy > 0) {
				upper_bound = path[i].y;
				lower_bound = path[i].y - path[i].dy;
			} else {
				upper_bound = path[i].y - path[i].dy;
				lower_bound = path[i].y;
			}
			
			// general case:
			// derive b (y = mx + b) using a line and a point
			float m = path[i].dy/path[i].dx;
			float b = path[i].y - m * path[i].x;
			//printf("m=%3f;b=%3f\n", m, b);
			// determine where this line intersects
			// on the left?
			float x = position.x;
			float y = m * x + b;
			if((lower_bound <= y) && (y <= upper_bound) && !path[i].left) {
				// hit on the left
				//printf("hit left: x=%3.1f;y=%3.1f\n", x, y); fflush(stdout);
				float dist = sqrt(pow(x - path[i].x, 2) + pow(y - path[i].y, 2));
				if(dist > max_dist) {
					max_dist = dist;
					hit_vector = path[i];
					hit_where = HIT_LEFT;
				}
			}
			// on the right?
			x = (position.x + hitboxes.front().w);
			y = m * x + b;
			if((lower_bound <= y) && (y <= upper_bound) && path[i].left) {
				// hit on the right
				//printf("hit right: x=%3.1f;y=%3.1f\n", x, y); fflush(stdout);
				float dist = sqrt(pow(x - path[i].x, 2) + pow(y - path[i].y, 2));
				if(dist > max_dist) {
					max_dist = dist;
					hit_vector = path[i];
					hit_where = HIT_RIGHT;
				}
			}
			// on the top?
			y = position.y;
			x = (y - b) / m;
			if((lower_bound <= y) && (y <= upper_bound) && !path[i].top) {
				// hit on the top
				//printf("hit top: x=%3.1f;y=%3.1f\n", x, y); fflush(stdout);
				float dist = sqrt(pow(x - path[i].x, 2) + pow(y - path[i].y, 2));
				if(dist > max_dist) {
					max_dist = dist;
					hit_vector = path[i];
					hit_where = HIT_TOP;
				}
			}
			// on the bottom?
			y = (position.y + hitboxes.front().h);
			x = (y - b) / m;
			if((lower_bound <= y) && (y <= upper_bound) && path[i].top) {
				// hit on the bottom
				//printf("hit bottom: x=%3.1f;y=%3.1f\n", x, y); fflush(stdout);
				float dist = sqrt(pow(x - path[i].x, 2) + pow(y - path[i].y, 2));
				if(dist > max_dist) {
					max_dist = dist;
					hit_vector = path[i];
					hit_where = HIT_BOTTOM;
				}
			}
		}
		//printf("diag: 0x%X\n", hit_where); fflush(stdout);
		float mag = max_dist / sqrt(pow(hit_vector.dx, 2) + pow(hit_vector.dy, 2));
		if(hit_where == HIT_TOP || hit_where == HIT_BOTTOM) {
			o->position.y -= mag * hit_vector.dy;
		} else if(hit_where == HIT_LEFT || hit_where == HIT_RIGHT) {
			o->position.x -= mag * hit_vector.dx;
		}
		
	} else {
		fprintf(stderr, "Error: somehow two immovable objects collided");
	}
	
	return;
}
