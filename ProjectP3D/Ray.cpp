#include "Ray.h"



Ray::Ray(Vect* origin, Vect* direction)
{
	_origin = origin;
	_direction = direction;
	_direction = direction->normalize();
}


Ray::~Ray()
{
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
