#pragma once
#include "Vect.h"
#include "Constants.h"

class Ray
{
	//Ray�s origin
	Vect* _origin;
	//Ray's direction
	Vect* _direction;

	float _t;

public:
	Ray(Vect* origin, Vect* direction);
	~Ray();
	//Returns _direction
	Vect * getD();
	//Returns _origin
	Vect * getO();
	//Returns the point the ray hitsS
	Vect* getHitPoint(float dist);
};

