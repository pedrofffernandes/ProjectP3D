#pragma once
#ifndef _VECT_H
#define _VECT_H

#include "math.h"
#include <stdlib.h>

class Vect
{
	float _x, _y, _z;
public:
	// Basic Constructor
	Vect();
	// General Construction Function
	Vect(float x, float y, float z);
	Vect(Vect* vect);
	// destructor
	~Vect();
	
	//Returns the vector length
	float length();
	//Normalizes the vector
	Vect * normalize();
	//Subtracts two vectors
	Vect * minus(Vect * vect);
	//Does the cross product
	Vect * crossP(Vect * vect);
	//Does the dot product
	float dotP(Vect * vect);
	//Multiplies the vector by a float f
	Vect * multiply(float f);
	//Adds two vectors
	Vect * add(Vect * vect);
	//Multiplies the elements of the vectors
	Vect * lineP(Vect * vect);
	//absf to all vector elements
	Vect * positive();

	int max();
	//Compute det of x and y
	float det2D(Vect * vect);
	//Rearange coordinates for Badouel's algorithm
	Vect* r2D2(int i);
	// METHOD functions
	// get individual coordinates
	float getX() { return _x; }
	float getY() { return _y; }
	float getZ() { return _z; }
};
#endif // !_VECT_H