#include <stdlib.h>

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

class FPS_counter {
	Uint16 fps;
	SDL_Surface *fps_disp;
	TTF_Font *font;
	SDL_Color text_color;
	Uint32 start;
	
	public:
	
	FPS_counter();
	
	void mark();
	
	void show(SDL_Surface*);
	
	void delay_until(int);
};
