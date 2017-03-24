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
	// Initialization
	/// This is just for the sake of legibility
	/// Ray Direction and respective Coordinates
	float ray_direction_x = ray->getD()->getX();
	float ray_direction_y = ray->getD()->getY();
	float ray_direction_z = ray->getD()->getZ();
	/// Ray Origin Point and respective Coordinates
	float ray_origin_x = ray->getO()->getX();
	float ray_origin_y = ray->getO()->getY();
	float ray_origin_z = ray->getO()->getZ();
	/// Sphere Center and respective Coordinates
	float sphere_center_x = _position->getX();
	float sphere_center_y = _position->getY();
	float sphere_center_z = _position->getZ();
	
	// 1) Normalize ray_direction
	/// Already done
	// 2) Compute the square distance from the ray origin to the sphere center
	float d =
		pow((sphere_center_x - ray_origin_x), 2) +
		pow((sphere_center_y - ray_origin_y), 2) +
		pow((sphere_center_z - ray_origin_z), 2);
	// 3) Compare distance squared with the radius squared
	if (d == _radiusSquare)
		/// It means the ray origin is in the sphere surface
		/// so we will consider that it doesn't intersect
		return 0.0f;
	// 4) Compute B
	float b = ray_direction_x*(sphere_center_x - ray_origin_x) + ray_direction_y*(sphere_center_y - ray_origin_y) + ray_direction_z*(sphere_center_z - ray_origin_z);
	// 5) Check if the ray came from outside the sphere
	// and its pointing in the opposite direction to the sphere center
	if (d > _radiusSquare && b < 0)
		return 0.0f;
	// 6) Compute R
	float R = b*b - d + _radiusSquare;
	/// If R < 0 then Sphere not intersected
	if (R < 0)
		return 0.0f;
	// 7) Choose the root that is in closest
	if (d > _radiusSquare)
		return b - sqrt(R);
	else 
		return b + sqrt(R);
}

Vect * Sphere::getNormal(Vect * point) {
	Vect * result = new Vect(point);
	result->minus(_position);
	return result->normalize();
}
