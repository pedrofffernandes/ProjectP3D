#include "Triangle.h"
#define EPSILON 1e-4



Triangle::Triangle(Vect * point1, Vect * point2, Vect * point3, Material * mat) : Obj(mat)
{
	_point1 = point1;
	_point2 = point2;
	_point3 = point3;
	Vect * p12 = _point2->minus(_point1);
	Vect * p13 = _point3->minus(_point1);
	//Set normal
	_n = p12->crossP(p13);
	_n = _n->normalize();
	_d = _n->dotP(_point1->multiply(-1));

	_i = _n->max();
}

Triangle::~Triangle()
{
}

float Triangle::intersect(Ray * ray)
{	
	if (ray->getD()->dotP(_n) > -EPSILON) {
		return 0.0;
	}
	float t = -(_d + _n->dotP(ray->getO())) / (_n->dotP(ray->getD()));
	if (t < EPSILON) {
		return 0.0;
	}

	Vect* point = ray->getO()->add(ray->getD()->multiply(t));
	Vect* a = point->minus(_point1)->r2D2(_i);
	Vect* b = _point2->minus(_point1)->r2D2(_i);
	Vect* c = _point3->minus(_point1)->r2D2(_i);

	float deno = b->det2D(c);
	float alfa = a->det2D(c) / deno;
	float beta = b->det2D(a) / deno;

	if (alfa >= 0 && beta >= 0 && alfa + beta <= 1)
		
		return t;
	return 0.0f;
}

Vect * Triangle::getNormal(Vect * hit)
{
	return _n;
}
