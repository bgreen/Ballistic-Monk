#include <stdlib.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "character.h"
#include "fps_counter.h"
#include "static.h"

#define MAX_FPS 30
#define COLORKEY 0xFF00FF

int main(int argc, char *argv[])
{
    SDL_Surface *screen;
    
    /************************/
    /* SDL Initialization	*/
    /************************/
    
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
    
    // Create Rendering Surfaces
    Uint32 color_key = SDL_MapRGB(screen->format, 0xff, 0x00, 0xff);
    SDL_Surface* background = SDL_CreateRGBSurface(SDL_SWSURFACE, 640, 480, 16, 0,0,0,0);
    SDL_FillRect(background, NULL, 0x0000);
    SDL_Surface* scene[3];
    for(int i=0; i<3; i++) {
    	scene[i] = SDL_CreateRGBSurface(SDL_SWSURFACE, 640, 480, 16, 0,0,0,0);
    	SDL_SetColorKey(scene[i], SDL_SRCCOLORKEY, color_key);
    }
    
    // Keep Track of Objects
    std::vector<Object*> objects;
    std::vector<Character*> characters;
    std::vector<Player*> players;
    
    // Create the Player
    Player you;
    SDL_Rect hitbox = {0, 0, 32, 32};
    you.add_hitbox(hitbox);
    objects.push_back(&you);
    characters.push_back(&you);
    players.push_back(&you);
    
    // A test wall
    SDL_Rect dim = {0, 480-10, 640, 10};
    Wall w (dim);
    objects.push_back(&w);
    
    FPS_counter fps;
    
    // Game loop
    bool quit = false;
    while(!quit) {
    	char pos[20];
    	// Start timer for fps monitoring
    	fps.mark();
    	
		/************/
    	/*	Events	*/
    	/************/
    	
    	SDL_Event event;
    	while(SDL_PollEvent(&event)) {
    		if(event.type == SDL_QUIT) {
    			quit = true;
    			break;
    		}
    		you.input.push(event);
    	}
    	you.handle_input();
    	
    	/************/
    	/*	Logic	*/
    	/************/
    	
    	// check for hits between all character-object pairs
    	std::queue<collision_pair> collisions;
    	std::vector<Character*>::iterator character_iter;
    	std::vector<Object*>::iterator object_iter;
    	character_iter = characters.begin();
    	while(character_iter != characters.end()) {
    		object_iter = objects.begin();
    		while(object_iter != objects.end()) {
    			bool collided = (*character_iter)->is_hit(*object_iter);
    			if(collided) {
    				collision_pair c;
  					c.a = *character_iter;
  					c.b = *object_iter;
  					collisions.push(c);
    			}
    			object_iter++;
    		}
    		character_iter++;
    	}
    	
    	// determine and apply effects of each object in a hit pair
    	while(!collisions.empty()) {
    		collision_pair c = collisions.front();
    		c.a->collide(c.b);
    		c.b->collide(c.a);
    		collisions.pop();
    	}
    	
    	// move all characters
    	character_iter = characters.begin();
    	while(character_iter != characters.end()) {
    		(*character_iter)->handle_move();
    		character_iter++;
    	}
    	
    	/****************/
    	/*	Rendering	*/
    	/****************/
    	
    	SDL_BlitSurface(background, NULL, screen, NULL);
    	
    	// clear scene
    	for(int i=0; i<3; i++) {
    		SDL_FillRect(scene[i], NULL, color_key);
    	}
    	
    	// Blit all objects to appropriate layers
    	object_iter = objects.begin();
    	while(object_iter != objects.end()) {
    		(*object_iter)->handle_show(scene);
    		object_iter++;
    	}
    	// render the fps counter
    	fps.show(scene[2]);
    	for(int i=0; i<3; i++) {
    		SDL_BlitSurface(scene[i], NULL, screen, NULL);
    	}
    	
    	// show that shit
    	SDL_Flip(screen);
    	
    	// FPS delay
    	fps.delay_until(1000/MAX_FPS);
    }
    
    return 0;
}
