#include "Plane.h"
#include <stdio.h>
#include <iostream>


Plane::Plane(Vect* point1, Vect* point2, Vect* point3, Material * mat) : Obj(mat)
{
	_point1 = point1;
	_point2 = point2;
	_point3 = point3;
	Vect * p12 = _point2->minus(_point1);
	Vect * p13 = _point3->minus(_point1);
	//Get normal
	_n = p12->crossP(p13);
	_n = _n->normalize();
	
}


Plane::~Plane()
{
}

float Plane::intersect(Ray * ray)
{	
	if (_n->dotP(ray->getD()) < 1e-6 && _n->dotP(ray->getD()) > -1e-6)
		return 0.0f;
	float a = -(((ray->getO())->minus(_point1))->dotP(_n)) / (_n->dotP(ray->getD()));
	return a;
}

Vect * Plane::getNormal(Vect * point)
{
	return _n;
}
