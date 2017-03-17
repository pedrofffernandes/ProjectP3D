#pragma once
#include "obj.h"
class Polygon :
	public Obj
{
public:
	Polygon(Material* mat);
	~Polygon();
	virtual float intersect(Ray * ray);
	virtual Vect* getNormal();

};

