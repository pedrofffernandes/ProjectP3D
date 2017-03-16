#pragma once
#include "Polygon.h"
#include "material.h"
#include "Vect.h"
#include <list>

class BigPoly :
	public Polygon
{
	std::list<Vect*> _points;

public:
	BigPoly(Material* mat);
	~BigPoly();

	void addPoint(Vect* point);
	std::list<Vect*> getPoints();
	float intersect(Ray * ray);
};

