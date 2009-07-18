#include <SDL/SDL.h>
#include <queue>
#include "character.h"

Character::Character() {
	layer = 0;
	
	// in kg
	mass = 80;
	vel = PVector();
	weight = PVector(0, mass*(-9.81*16.67));
	force = PVector();
	force += weight;
}

void Character::handle_move(double dt) {
	vel.x += dt * force.x/mass;
	vel.y += dt * force.y/mass;
	
	// 30 pixels = 1.8 meters
	// 16.67 pixels/meter
	
	position.x += vel.x * dt;
	position.y += vel.y * dt;
	
	force.reset();
	force += weight;
}

Player::Player() {
	layer = 1;
	sprite = SDL_CreateRGBSurface(SDL_SWSURFACE, 32, 32, 16, 0,0,0,0);
	SDL_FillRect(sprite, NULL, SDL_MapRGB(sprite->format, 0x00, 0xFF, 0x00));
}

void Player::handle_input() {
	//SDL_Event event;
	PVector one_x (5000.0,0);
	PVector one_y (0,30000.0);
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

void Player::collide(Object* other, double dt) {
	//printf("Player says ouch\n");
	//fflush(stdout);
	return;
}

