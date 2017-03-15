#pragma once
#include "Obj.h"

class Plane : public Obj
{
	Vect* _point1;
	Vect* _point2;
	Vect* _point3;
	Vect* _n;

public:
	Plane(Vect* point1, Vect* point2, Vect* point3, Material * mat);
	~Plane();
	Vect * getPoint1() { return _point1;}
	//Returns Ray distance to "this"
	//Returns 0.0f if doesn't intersect
	//Returns 0.0f if origin is in surface
	float intersect(Ray * ray);
	Vect* getNormal(Vect* point);
};

