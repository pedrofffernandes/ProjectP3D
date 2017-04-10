#include "Ray.h"
#include <iostream>


Ray::Ray(Vect* origin, Vect* direction) {
	_origin = new Vect(origin);
	_direction = new Vect(direction);
	_direction->normalize();
}

Ray::~Ray() {
	delete _origin;
	delete _direction;
}

Vect * Ray::getD()
{
	return _direction;
}

Vect * Ray::getO()
{
	return _origin;
}

Vect * Ray::getHitPoint(float dist)
{
	Vect* hit = new Vect(_direction);
	hit->multiply(dist);
	return hit->add(_origin);
}

