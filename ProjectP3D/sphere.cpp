#include "Sphere.h"


Sphere::Sphere(Vect* position, float r, Material * mat) : Obj(mat)
{
	_position = position;
	_radius = r;
	_radiusSquare = r * r;
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

	//Compute the square distance sphere to ray origin
	float d = (cx - ox)*(cx - ox) + (cy - oy)*(cy - oy) + (cz - oz)*(cz - oz);
	//Compare distance squared to radius squared
	if (d == _radiusSquare)
		return 0.0f;
	//Compute B
	float b = dx*(cx - ox) + dy*(cy - oy) + dz*(cz - oz);

	if (d > _radiusSquare)
		if (b < 0)
			return 0.0f;
	//Compute R
	float R = b*b - d + _radiusSquare;
	//If R < 0 then Sphere not intersected
	if (R < 0)
		return 0.0f;
	//Choose the root that is in front
	if (d > _radiusSquare)
		return b - sqrt(R);
	if (d < _radiusSquare)
		return b + sqrt(R);
}

Vect * Sphere::getNormal(Vect * point)
{
	return (point->minus(_position))->normalize();
}
