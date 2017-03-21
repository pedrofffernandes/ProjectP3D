#include "Ray.h"
#include <iostream>


Ray::Ray(Vect* origin, Vect* direction) {
	_origin = origin;
	Vect * dir = new Vect(direction);
	_direction = dir->normalize();
}

Ray::~Ray() {
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
	Vect* hit = _direction->multiply(dist);
	hit = hit->add(_origin);
	return hit;
}

