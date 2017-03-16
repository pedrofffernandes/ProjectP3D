#pragma once
#include "Polygon.h"
class Triangle :
	public Polygon
{
	Vect* _point1;
	Vect* _point2;
	Vect* _point3;

public:
	Triangle(Vect* point1, Vect* point2, Vect* point3, Material* mat);
	~Triangle();
};

