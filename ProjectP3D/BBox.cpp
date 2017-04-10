#include "BBox.h"



BBox::BBox(Vect* min, Vect* max)
{
	_min_point = new Vect(min);
	_max_point = new Vect(max);
}


BBox::~BBox()
{
}
