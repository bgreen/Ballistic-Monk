#include <SDL/SDL.h>
#include <queue>
#include "character.h"

Character::Character() {
	xVel = 0, yVel = 0;
	speed = 5;
	layer = 0;
}

Player::Player() {
	layer = 1;
	sprite = SDL_CreateRGBSurface(SDL_SWSURFACE, 32, 32, 16, 0,0,0,0);
	SDL_FillRect(sprite, NULL, SDL_MapRGB(sprite->format, 0x00, 0xFF, 0x00));
}

void Player::handle_input() {
	SDL_Event event;
	while(!input.empty()) {
		event = input.front();
		if(event.type == SDL_KEYDOWN) {
			switch(event.key.keysym.sym) {
				case SDLK_UP:		yVel -= 1; break;
				case SDLK_DOWN:		yVel += 1; break;
				case SDLK_LEFT:		xVel -= 1; break;
				case SDLK_RIGHT:	xVel += 1; break;
			}
		} else if(event.type == SDL_KEYUP) {
			switch(event.key.keysym.sym) {
				case SDLK_UP:		yVel += 1; break;
				case SDLK_DOWN:		yVel -= 1; break;
				case SDLK_LEFT:		xVel += 1; break;
				case SDLK_RIGHT:	xVel -= 1; break;
			}
		}
		input.pop();
	}
}
	
void Player::handle_move() {
	position.x += speed * xVel;
	position.y += speed * yVel;
}

void Player::collide(Object* other) {
	printf("Player says ouch\n");
	fflush(stdout);
	return;
}

