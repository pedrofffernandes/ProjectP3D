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
	_n->normalize();
	// Computing Bounding Box
	// NOTE: The plane doesnt really have a bounding box since its infinite
	// This is just to give some values that wont interfere
	/// Check the minimum coordinates for the BBox
	float x_min = HUGE_VALUE;
	float y_min = HUGE_VALUE;
	float z_min = HUGE_VALUE;
	Vect * min_point = new Vect(x_min - EPSILON, y_min - EPSILON, z_min - EPSILON);
	/// Check the maximum coordinates for the BBox
	float x_max = -HUGE_VALUE;
	float y_max = -HUGE_VALUE;
	float z_max = -HUGE_VALUE;
	Vect * max_point = new Vect(x_max + EPSILON, y_max + EPSILON, z_max + EPSILON);
	this->setBBox(new BBox(min_point, max_point));

	delete min_point;
	delete max_point;
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
	return new Vect(_n);
}
