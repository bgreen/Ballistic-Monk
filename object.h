#include <SDL/SDL.h>
#include <vector>
#include "physics.h"

struct coord {
	double x;
	double y;
};

class Object {
	public:
	int layer;
	struct coord position;
	std::vector<SDL_Rect> hitboxes;
	
	Object();
	Object(int x, int y, int layer);
	~Object();
	
	virtual void handle_show(SDL_Surface**);
	
	void add_hitbox(SDL_Rect);
	
	bool is_hit(Object* other);
	
	virtual void collide(Object* other, double dt);
	
	protected:
	SDL_Surface* sprite;
};

struct collision_pair {
	Object* a;
	Object* b;
};
