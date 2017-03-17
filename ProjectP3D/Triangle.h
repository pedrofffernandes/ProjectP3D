#pragma once
#include "obj.h"
class Triangle :
	public Obj
{
	Vect* _point1;
	Vect* _point2;
	Vect* _point3;
	Vect* _n;
	float _d;

public:
	Triangle(Vect* point1, Vect* point2, Vect* point3, Material* mat);
	~Triangle();
	float intersect(Ray * ray);
	Vect* getNormal(Vect* hit);
};

