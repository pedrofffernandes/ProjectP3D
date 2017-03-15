#pragma once
#include "Material.h"
#include "Ray.h"

class Obj
{

	Material * _material;

public:
	Obj(Material * m);
	~Obj();

	//Intersects the Obj with a Ray
	virtual float intersect(Ray * ray);
	virtual Vect* getNormal(Vect* point);
	Material* getMat();
};

