#include "BBox.h"



BBox::BBox(Vect* min, Vect* max)
{
	_min_point = new Vect(min);
	_max_point = new Vect(max);
}


BBox::~BBox() {
	delete _min_point;
	delete _max_point;
}

bool BBox::inside(Vect * point)
{
	if (_min_point->getX() <= point->getX() &&
		point->getX() <= _max_point->getX() &&
		_min_point->getY() <= point->getY() &&
		point->getY() <= _max_point->getY() &&
		_min_point->getZ() <= point->getZ() &&
		point->getZ() <= _max_point->getZ())
		return true;
	return false;
}
