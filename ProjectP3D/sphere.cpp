#include "Sphere.h"


Sphere::Sphere(Vect* position, float r, Material * mat) : Obj(mat)
{
	_position = position;
	_radius = r;
	_radiusSquare = r * r;
	// Setup Bounding Box
	bboxSetup();
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

Vect * Sphere::getNormal(Vect * point) {
	Vect * result = new Vect(point);
	result->minus(_position);
	return result->normalize();
}


void Sphere::bboxSetup(void) {
	// Computing Bounding Box
	/// Check the minimum coordinates for the BBox
	float x_min = _position->getX() - _radius; x_min -= EPSILON;
	float y_min = _position->getY() - _radius; y_min -= EPSILON;
	float z_min = _position->getZ() - _radius; z_min -= EPSILON;
	Vect * min_point = new Vect(x_min, y_min, z_min);
	/// Check the minimum coordinates for the BBox
	float x_max = _position->getX() + _radius; x_max += EPSILON;
	float y_max = _position->getY() + _radius; y_max += EPSILON;
	float z_max = _position->getZ() + _radius; z_max += EPSILON;
	Vect * max_point = new Vect(x_max, y_max, z_max);
	this->setBBox(new BBox(min_point, max_point));
	/// Deleting Pointers
	delete min_point;
	delete max_point;
}