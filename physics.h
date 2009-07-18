#include "stdint.h"

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
