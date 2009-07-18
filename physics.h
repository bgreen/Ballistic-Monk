#ifndef _INC_PHYSICS
#define _INC_PHYSICS

#include "stdint.h"

#define AIR_VISCOSITY 0.2
#define PIXELS_PER_METER 16.67

class PVector {
	public:
	
	double x, y;
	
	PVector();
	PVector(double px,double py);
	
	PVector operator + (PVector n1);
	PVector operator - (PVector n1);
	bool operator += (PVector n1);
	bool operator -= (PVector n1);
	
	PVector normalize();
	PVector xcomp();
	PVector ycomp();
	float mag();
	
	void reset();
};

class Timer {
	public:
	
	unsigned int start_t, stop_t;
	
	Timer();
	
	void start();
	void stop();
	unsigned int dt_ms();
	double dt_s();
};

#endif
