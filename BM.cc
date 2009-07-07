#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "character.h"

#define MAX_FPS 20



SDL_Surface *screen;



void clear_screen() {
	SDL_FillRect(screen, NULL, 0x0000);
}

int sys_init();

int main(int argc, char *argv[])
{
    sys_init();
    
    SDL_Event event;
    Player you;
    
    Uint16 fps = 0;
    SDL_Surface *fps_counter = NULL;
    TTF_Font *font = NULL;
    font = TTF_OpenFont("FreeMono.ttf", 12);
    SDL_Color text_color = {0xFF, 0xFF, 0xFF};
    
    if(font == NULL) {
    	fprintf(stderr, "Error loading font");
    	return 1;
    }
    
    bool quit = false;
    while(!quit) {
    	
    	// Start timer for fps monitoring
    	Uint32 start = SDL_GetTicks(), end = 0;

    	// Events
    	while(SDL_PollEvent(&event)) {
    		if(event.type == SDL_QUIT) {
    			quit = true;
    			break;
    		}
    		you.input.push(event);
    	}
    	you.handle_input();
    	
    	// Logic
    	you.handle_move();
    	
    	// Rendering
    	clear_screen();
    	char fps_string[10];
    	sprintf(fps_string, "fps: %d", fps);
    	fps_counter = TTF_RenderText_Solid(font, fps_string, text_color);
    	SDL_Rect offset;
    	offset.x = 5;
    	offset.y = 5;
    	you.handle_show(screen);
    	
    	SDL_BlitSurface(fps_counter, NULL, screen, &offset);
    	SDL_Flip(screen);
    	
    	// FPS delay
    	end = SDL_GetTicks();
    	int delay = (1000/MAX_FPS) - (end - start);
    	fps = 1000/(end - start);
    	if(delay > 0) SDL_Delay(delay);
    }
    
    return 0;
}

int sys_init() {
	
	if ( SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO|SDL_INIT_TIMER) < 0 ) {
        fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
        exit(1);
    }
    atexit(SDL_Quit);

    screen = SDL_SetVideoMode(640, 480, 16, SDL_SWSURFACE);
    if ( screen == NULL ) {
        fprintf(stderr, "Unable to set 640x480 video: %s\n", SDL_GetError());
        exit(1);
    }
    
    if (TTF_Init() == -1) {
    	fprintf(stderr, "Unable to init ttf engine");
    	exit(1);
    }
    
    return 0;
}
