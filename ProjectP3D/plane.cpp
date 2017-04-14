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
	// Setup Bounding Box
	bboxSetup();
	
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


void Plane::bboxSetup(void) {
	// Computing Bounding Box
	// NOTE: The plane doesnt really have a bounding box since its infinite
	// This is just to give some values that wont interfere with the calculations
	/// Check the minimum coordinates for the BBox
	float x_min = HUGE_VALUE - EPSILON;
	float y_min = HUGE_VALUE - EPSILON;
	float z_min = HUGE_VALUE - EPSILON;
	Vect * min_point = new Vect(x_min, y_min, z_min);
	/// Check the maximum coordinates for the BBox
	float x_max = -HUGE_VALUE + EPSILON;
	float y_max = -HUGE_VALUE + EPSILON;
	float z_max = -HUGE_VALUE + EPSILON;
	Vect * max_point = new Vect(x_max, y_max, z_max);
	this->setBBox(new BBox(min_point, max_point));
	/// Delete Pointers
	delete min_point;
	delete max_point;
}