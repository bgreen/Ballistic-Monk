#include <SDL/SDL.h>
#include <queue>

class Character {
	protected:
	int x, y;
	int xVel, yVel;
	int speed;
	SDL_Surface *sprite;
	
	public:
	
	Character() {
		x = 0;
		y = 0;
		xVel = 0, yVel = 0;
		speed = 5;
		sprite = SDL_CreateRGBSurface(SDL_SWSURFACE, 32, 32, 16, 0,0,0,0);
		SDL_FillRect(sprite, NULL, 0x0F00);
	}
	
	virtual void handle_move() = 0;
	virtual void handle_show(SDL_Surface* screen) = 0;
};


class Player: Character {
	
	public:
	
	std::queue<SDL_Event> input;
	
	void handle_input() {
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
	
	void handle_move() {
		x += speed * xVel;
		y += speed * yVel;
	}
	void handle_show(SDL_Surface* screen) {
		SDL_Rect offset;
		offset.x = x;
		offset.y = y;
		SDL_BlitSurface(sprite, NULL, screen, &offset);
	}
};
