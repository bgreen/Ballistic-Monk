
#include "SDL/SDL.h"

#include "object.h"
	
void Object::handle_show(SDL_Surface* scene[]) {
	SDL_Rect tmp;
	tmp.x = position.x;
	tmp.y = 479 - position.y;
	SDL_BlitSurface(sprite, NULL, scene[layer], &tmp);
}

Object::Object() {
	position.x = 0;
	position.y = 480;
	friction = 1.3;
	sprite = SDL_CreateRGBSurface(SDL_SWSURFACE, 32, 32, 16, 0,0,0,0);
	SDL_FillRect(sprite, NULL, 0xF000);
}

Object::Object(int x, int y, int l) {
	position.x = x;
	position.y = y;
	friction = 1;
	layer = l;
	sprite = SDL_CreateRGBSurface(SDL_SWSURFACE, 32, 32, 16, 0,0,0,0);
	SDL_FillRect(sprite, NULL, 0xF000);
}

Object::~Object() {
	SDL_FreeSurface(sprite);
}

void Object::add_hitbox(SDL_Rect b) {
	hitboxes.push_back(b);
}

bool Object::is_hit(Object* other) {
	if(this == other) return false;
	 
	std::vector<SDL_Rect>::iterator this_iter = hitboxes.begin();
	while(this_iter != hitboxes.end()) {
		// calculate sides for this hitbox
		int a_left, a_right, a_top, a_bottom;
		a_left = position.x + (*this_iter).x;
		a_right = a_left + ((*this_iter).w);
		a_top = position.y + (*this_iter).y;
		a_bottom = a_top - ((*this_iter).h);
		
		std::vector<SDL_Rect>::iterator other_iter = other->hitboxes.begin();
		while(other_iter != other->hitboxes.end()) {
			// calculate sides for that hitbox
			int b_left, b_right, b_top, b_bottom;
			b_left = other->position.x + (*other_iter).x;
			b_right = b_left + ((*other_iter).w);
			b_top = other->position.y + (*other_iter).y;
			b_bottom = b_top - ((*other_iter).h);
			// hit????
			if(a_bottom >= b_top) return false;
			if(a_top <= b_bottom) return false;
			if(a_right <= b_left) return false;
			if(a_left >= b_right) return false;
			other_iter++;
		}
		this_iter++;
	}
	return true;
}

void Object::collide(Object* other, double dt) {
	return;
}
