#pragma once


#include "Material.h"
#include "Ray.h"
#include "Constants.h"
#include "BBox.h"

#define EPSILON 1e-4
#define HUGE_VALUE 1000000;


class Obj
{
	BBox * _bbox;
	Material * _material;

public:
	Obj(Material * m);
	~Obj();

	//Intersects the Obj with a Ray
	virtual float intersect(Ray * ray);
	virtual Vect* getNormal(Vect* point);
	void setBBox(BBox* bb) { _bbox = bb; }
	BBox * get_bounding_box() { return _bbox; }
	Material* getMat();
};

