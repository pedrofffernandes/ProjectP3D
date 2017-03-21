#include "Ray.h"
#include <iostream>


Ray::Ray(Vect* origin, Vect* direction) {
	_origin = origin;
	_direction = direction;
	_direction = direction->normalize();
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
	Vect* hit = new Vect(_direction->multiply(dist));
	return hit->add(_origin);
}

