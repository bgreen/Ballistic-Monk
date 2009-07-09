#include "static.h"

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
	printf("Wall says ouch\n");
	fflush(stdout);
	return;
}
