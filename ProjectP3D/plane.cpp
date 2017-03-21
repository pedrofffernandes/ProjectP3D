#include "Plane.h"


Plane::Plane(Vect* point1, Vect* point2, Vect* point3, Material * mat) : Obj(mat)
{
	_point1 = point1;
	_point2 = point2;
	_point3 = point3;
	Vect * p2 = new Vect(point2);
	Vect * p12 = p2->minus(_point1);
	Vect * p3 = new Vect(point3);
	Vect * p13 = p3->minus(_point1);
	//Set normal
	_n = p12->crossP(p13);
	_n = _n->normalize();	
	delete p2;
	delete p3;
}


Plane::~Plane()
{
}

float Plane::intersect(Ray * ray)
{	
	if (_n->dotP(ray->getD()) > -1e-4)
		return 0.0f;
	Vect * rayO = new Vect(ray->getO());
	rayO->minus(_point1);
	float result = -((rayO)->dotP(_n)) / (_n->dotP(ray->getD())); 
	delete rayO;
	return result;
}

Vect * Plane::getNormal(Vect * point)
{
	return _n;
}
