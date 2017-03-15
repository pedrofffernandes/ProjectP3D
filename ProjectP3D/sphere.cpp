#include "Sphere.h"
#include <iostream>



Sphere::Sphere(Vect* position, float r, Material * mat) : Obj(mat)
{
	_position = position;
	_radius = r;
}


Sphere::~Sphere()
{
}

float Sphere::intersect(Ray * ray)
{
	float dx = ray->getD()->getX();
	float dy = ray->getD()->getY();
	float dz = ray->getD()->getZ();

	float ox = ray->getO()->getX();
	float oy = ray->getO()->getY();
	float oz = ray->getO()->getZ();

	float cx = _position->getX();
	float cy = _position->getY();
	float cz = _position->getZ();

	float r2;
	//Compute the square distance sphere to ray origin
	float d = (cx - ox)*(cx - ox) + (cy - oy)*(cy - oy) + (cz - oz)*(cz - oz);
	//Compare distance squared to radius squared
	if (d == (r2 = _radius*_radius))
		return 0.0f;
	//Compute B
	float b = dx*(cx - ox) + dy*(cy - oy) + dz*(cz - oz);

	if (d > r2)	
		if (b < 0)
			return 0.0f;
	//Compute R
	float R = b*b - d + r2;
	//If R < 0 then Sphere not intersected
	if (R < 0)
		return 0.0f;
	//Choose the root that is in front
	if (d > r2)
		return b - sqrt(R);
	if (d < r2)
		return b + sqrt(R);
}

Vect * Sphere::getNormal(Vect * point)
{
	return point->minus(_position)->normalize();
}
