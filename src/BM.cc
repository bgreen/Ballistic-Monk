#include <stdlib.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "character.h"
#include "object.h"
#include "fps_counter.h"
#include "static.h"

#define MAX_FPS 60
#define COLORKEY 0xFF00FF

int main(int argc, char *argv[])
{
    printf("Let's start ...\n");
    SDL_Surface *screen;
    
    /************************/
    /* SDL Initialization	*/
    /************************/
    
    printf("initializing video ...\n");
    
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
    	fprintf(stderr, "Unable to init ttf engine\n");
    	exit(1);
    }
    
    
    printf("Loading objects ...\n");
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
    
    // A test wall or two
    SDL_Rect dim = {0, 10, 640, 10};
    Wall w (dim);
    objects.push_back(&w);
    
    SDL_Rect dim2 = {630, 480, 10, 480};
    Wall w2 (dim2);
    objects.push_back(&w2);
    
    SDL_Rect dim3 = {440,100,200,10};
    Wall w3 (dim3);
    objects.push_back(&w3);
    
    FPS_counter fps;
    Timer time;
    
    printf("Entering game loop ...\n");
    
    // Game loop
    bool quit = false;
    while(!quit) {
    	
    	char pos[20];
    	// Start timer for fps monitoring
    	fps.mark();
    	
    	printf("Events ...\n");
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
    	
    	printf("Logic ...\n");
    	/************/
    	/*	Logic	*/
    	/************/
    	
    	// move all characters
    	std::vector<Character*>::iterator character_iter;
    	character_iter = characters.begin();
    	time.stop();
    	double dt = time.dt_s();
    	while(character_iter != characters.end()) {
    		(*character_iter)->handle_move(dt);
    		character_iter++;
    	}
    	time.start();
    	
    	// check for hits between all character-object pairs
    	std::queue<collision_pair> collisions;
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
    		c.a->collide(c.b, dt);
    		c.b->collide(c.a, dt);
    		collisions.pop();
    	}
    	
    	
    	printf("Rendering ...\n");
    	/****************/
    	/*	Rendering	*/
    	/****************/
    	
    	SDL_BlitSurface(background, NULL, screen, NULL);
    	
    	printf("clear scene ...\n");
    	// clear scene
    	for(int i=0; i<3; i++) {
    		SDL_FillRect(scene[i], NULL, color_key);
    	}
    	
    	printf("blit objects ...\n");
    	// Blit all objects to appropriate layers
    	object_iter = objects.begin();
    	while(object_iter != objects.end()) {
    		(*object_iter)->handle_show(scene);
    		object_iter++;
    	}
    	
    	printf("fps counter ...\n");
    	// render the fps counter
    	fps.show(scene[2]);
    	
    	for(int i=0; i<3; i++) {
    		SDL_BlitSurface(scene[i], NULL, screen, NULL);
    	}
    	
    	printf("blit to screen ...\n");
    	// show that shit
    	SDL_Flip(screen);
    	
    	// FPS delay
    	fps.delay_until(1000/MAX_FPS);
    	printf("End loop ...\n");
    	
    }
    
    return 0;
}
