#include "BigPoly.h"




BigPoly::BigPoly(Material * mat) : Polygon(mat)
{
}

BigPoly::~BigPoly()
{
}

void BigPoly::addPoint(Vect * point) {
	_points.push_back(point);
}

std::list<Vect*> BigPoly::getPoints()
{
	return _points;
}
