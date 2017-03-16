#include "Triangle.h"



Triangle::Triangle(Vect * point1, Vect * point2, Vect * point3, Material * mat) : Polygon(mat)
{
	_point1 = point1;
	_point2 = point2;
	_point3 = point3;
}

Triangle::~Triangle()
{
}
