#include "physics.h"
#include "math.h"
#include <algorithm>
#include "SDL/SDL.h"

/************/
/*	PVector	*/
/************/

PVector::PVector() {
	x = 0;
	y = 0;
}

PVector::PVector(double px,double py) {
	x = px;
	y = py;
}
	
PVector PVector::operator + (PVector n1) {
	return PVector(x + n1.x, y + n1.y);
}

PVector PVector::operator - (PVector n1) {
	return PVector(x - n1.x, y - n1.y);
}

bool PVector::operator += (PVector n1) {
	x += n1.x;
	y += n1.y;
	return true;
}

bool PVector::operator -= (PVector n1) {
	x -= n1.x;
	y -= n1.y;
	return true;
}

PVector PVector::normalize() {
	double div;
	if(fabs(x) >= fabs(y)) {
		div = fabs(x);
	} else {
		div = fabs(y);
	}
	
	return PVector(x/div, y/div);
}

PVector PVector::xcomp() {
	return PVector(x, 0);
}

PVector PVector::ycomp() {
	return PVector(0, y);
}

float PVector::mag() {
	return sqrt(pow(x,2) + pow(y,2));
}

	
void PVector::reset() {
	x = 0;
	y = 0;
}

/************/
/*	Timer	*/
/************/


Timer::Timer() {
	start_t = 0;
	stop_t = 0;
}
	
void Timer::start() {
	start_t = SDL_GetTicks();
}

void Timer::stop() {
	stop_t = SDL_GetTicks();
}

unsigned int Timer::dt_ms() {
	return stop_t - start_t;
}

double Timer::dt_s() {
	return (stop_t - start_t)/1000.0;
}


