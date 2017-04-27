#pragma once
// Project Classes
#include "Vect.h"
#include "Ray.h"

class BBox
{
	Vect * _min_point;
	Vect * _max_point;

public:
	BBox(Vect*,Vect*);
	~BBox();
	Vect * getMinPoint() { return _min_point; }
	Vect * getMaxPoint() { return _max_point; }
	float getMinX() { return _min_point->getX(); }
	float getMinY() { return _min_point->getY(); }
	float getMinZ() { return _min_point->getZ(); }
	float getMaxX() { return _max_point->getX(); }
	float getMaxY() { return _max_point->getY(); }
	float getMaxZ() { return _max_point->getZ(); }
	bool inside(Vect*); /// Returns true if the point is inside the BBox
};

