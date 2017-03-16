#include "Polygon.h"



Polygon::Polygon(Material* mat) : Obj(mat)
{
}

Polygon::~Polygon()
{
}

float Polygon::intersect(Ray * ray)
{
	return 0.0f;
}
