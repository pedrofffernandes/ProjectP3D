#include "Plane.h"


Plane::Plane(Vect* point1, Vect* point2, Vect* point3, Material * mat) : Obj(mat)
{
	_point1 = point1;
	_point2 = point2;
	_point3 = point3;
	Vect * p12 = _point2->minus(_point1);
	Vect * p13 = _point3->minus(_point1);
	//Set normal
	_n = p12->crossP(p13);
	_n = _n->normalize();	
	delete p12;
	delete p13;
}


Plane::~Plane()
{
}

float Plane::intersect(Ray * ray)
{	
	if (_n->dotP(ray->getD()) > -1e-4)
		return 0.0f;
	return -(((ray->getO())->minus(_point1))->dotP(_n)) / (_n->dotP(ray->getD())); //TODO otimizaçaos
}

Vect * Plane::getNormal(Vect * point)
{
	return _n;
}
