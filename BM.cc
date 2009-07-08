#include <stdlib.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "character.h"
#include "fps_counter.h"

#define MAX_FPS 20

SDL_Surface *screen;

int main(int argc, char *argv[])
{
    // initialize stuff
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
    
    SDL_Event event;
    Player you;
    FPS_counter fps;
    
    // Game loop
    bool quit = false;
    while(!quit) {
    	
    	// Start timer for fps monitoring
    	fps.mark();

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
    	SDL_FillRect(screen, NULL, 0x0000);
    	you.handle_show(screen);
    	fps.show(screen);
    	SDL_Flip(screen);
    	
    	// FPS delay
    	fps.delay_until(1000/MAX_FPS);
    }
    
    return 0;
}
