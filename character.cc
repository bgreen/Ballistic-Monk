
#include <SDL/SDL.h>
#include <queue>

#include "character.h"
#include "physics.h"

Character::Character() {
	layer = 0;
	
	// in kg
	mass = 80;
	vel = PVector();
	weight = PVector(0, mass*(-9.81 * PIXELS_PER_METER));
	force = PVector();
	force += weight;
	mod_flag = false;
}

Character::Character(float x, float y) {
	layer = 0;
	
	// in kg
	mass = 80;
	vel = PVector();
	weight = PVector(0, mass*(-9.81 * PIXELS_PER_METER));
	force = PVector();
	force += weight;
	
	position.x = x;
	position.y = y;
}

void Character::handle_move(double dt) {
	// apply acceleration
	vel.x += dt * force.x/mass;
	vel.y += dt * force.y/mass;
	
	// 30 pixels = 1.8 meters
	// 16.67 pixels/meter
	
	position.x += vel.x * dt;
	position.y += vel.y * dt;
	
	force.reset();
	force += weight;
}

void Character::collide(Object* other, double dt) {
	
	Character* o = dynamic_cast<Character*>(other);
	if(o != NULL) {
		if(mod_flag == false) {
			// a perfectly elastic collision
			double c_r = 1;
			double vx = vel.x;
			double vy = vel.y;
			double inv = 1/(mass + o->mass);
			//printf("inv=%f\n", inv);
			//printf("%d:a:%f, b:%f\n", ID, vel.x, o->vel.x);
			// delayed modification??
			vel.x = ((c_r + 1)*o->vel.x*o->mass + vel.x*(mass - c_r*o->mass)) * inv;
			vel.y = ((c_r + 1)*o->vel.y*o->mass + vel.y*(mass - c_r*o->mass)) * inv;
			
			o->vel.x = ((c_r + 1)*vx*mass + o->vel.x*(o->mass - c_r*mass)) * inv;
			o->vel.y = ((c_r + 1)*vy*mass + o->vel.y*(o->mass - c_r*mass)) * inv;
			
			mod_flag = true;
			o->mod_flag = true;
			
		} else {
			mod_flag = false;
			o->mod_flag = false;
			//printf("%d:a:%f, b:%f, dt:%f\n", ID, o->vel.x, vel.x, dt);
		}
	}
	return;
}

Player::Player() {
	layer = 1;
	sprite = SDL_CreateRGBSurface(SDL_SWSURFACE, 32, 32, 16, 0,0,0,0);
	SDL_FillRect(sprite, NULL, SDL_MapRGB(sprite->format, 0x00, 0xFF, 0x00));
}

void Player::handle_input() {
	//SDL_Event event;
	PVector one_x (mass*30*PIXELS_PER_METER,0);
	PVector one_y (0,mass*2*9.81*PIXELS_PER_METER);
	while(!input.empty()) {
	//	event = input.front();
	//	if(event.type == SDL_KEYDOWN) {
	//		switch(event.key.keysym.sym) {
	//			case SDLK_UP:		force += one_y; break;
	//			case SDLK_DOWN:		force -= one_y; break;
	//			case SDLK_LEFT:		force -= one_x; break;
	//			case SDLK_RIGHT:	force += one_x; break;
	//		}
	//	} else if(event.type == SDL_KEYUP) {
	//		switch(event.key.keysym.sym) {
	//			case SDLK_UP:		force -= one_y; break;
	//			case SDLK_DOWN:		force += one_y; break;
	//			case SDLK_LEFT:		force += one_x; break;
	//			case SDLK_RIGHT:	force -= one_x; break;
	//		}
	//	}
		input.pop();
	}
	
	Uint8 *keystates = SDL_GetKeyState(NULL);
	if(keystates[SDLK_UP]) {
		force += one_y;
	}
	if(keystates[SDLK_DOWN]) {
		force -= one_y;
	}
	if(keystates[SDLK_LEFT]) {
		force -= one_x;
	}
	if(keystates[SDLK_RIGHT]) {
		force += one_x;
	}
	
}

/*void Player::collide(Object* other, double dt) {
	// a perfectly elastic collision
	
	return;
}*/

