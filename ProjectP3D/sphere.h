#pragma once
#include "Obj.h"

class Sphere : public Obj
{
	Vect* _position;
	float _radius;

public:
	Sphere(Vect* position, float r, Material * mat);
	~Sphere();
	//Returns Ray distance to "this"
	//Returns 0.0f if doesn't intersect
	//Returns 0.0f if origin is in surface
	float intersect(Ray * ray);
	Vect* getNormal(Vect* point);

};

