
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

#include "fps_counter.h"

FPS_counter::FPS_counter() {
	fps = 0;
	fps_disp = NULL;
	font = TTF_OpenFont("../font/FreeMono.ttf", 12);
	text_color.r = 0xFF;
	text_color.g = 0xFF;
	text_color.b = 0xFF;
}
	
void FPS_counter::mark() {
	Uint32 now = SDL_GetTicks();
	fps = 1000/(now - start);
	start = now;
}
	
void FPS_counter::show(SDL_Surface* screen) {
	char fps_string[10];
   	sprintf(fps_string, "fps: %d", fps);
   	fps_disp = TTF_RenderText_Solid(font, fps_string, text_color);
   	SDL_Rect offset;
   	offset.x = 5;
   	offset.y = 5;
   	SDL_BlitSurface(fps_disp, NULL, screen, &offset);
}
	
void FPS_counter::delay_until(int t) {
	Uint32 end = SDL_GetTicks();
	int delay = t - (end - start);
	if(delay > 0) SDL_Delay(delay);
}
