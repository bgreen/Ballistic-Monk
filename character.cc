#include <SDL/SDL.h>
#include <queue>
#include "character.h"

Character::Character() {
	x = 0;
	y = 0;
	xVel = 0, yVel = 0;
	speed = 5;
	sprite = SDL_CreateRGBSurface(SDL_SWSURFACE, 32, 32, 16, 0,0,0,0);
	SDL_FillRect(sprite, NULL, 0x0F00);
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
	x += speed * xVel;
	y += speed * yVel;
}

void Player::handle_show(SDL_Surface* screen) {
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;
	SDL_BlitSurface(sprite, NULL, screen, &offset);
}

