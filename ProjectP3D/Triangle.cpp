#include "Triangle.h"
#define EPSILON 1e-4



Triangle::Triangle(Vect * point1, Vect * point2, Vect * point3, Material * mat) : Obj(mat)
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
	Vect * p1 = new Vect(point1);
	_d = _n->dotP(p1->multiply(-1));	

	_i = _n->max();
	delete p2;
	delete p3;
}

Triangle::~Triangle()
{
}

float Triangle::intersect(Ray * ray)
{	

	if (ray->getD()->dotP(_n) > -EPSILON) {
		return 0.0; /// The Ray doesnt intersect the triangle
	}

	float t = -(_d + _n->dotP(ray->getO())) / (_n->dotP(ray->getD()));
	if (t < EPSILON) {
		return 0.0; /// The Ray doesnt intersect the triangle
	}

	Vect* point = new Vect(ray->getO());
	Vect* epsilon = new Vect(ray->getD());
	epsilon->multiply(t);
	point->add(epsilon);
	Vect * a = point->minus(_point1)->r2D2(_i);
	Vect * p2 = new Vect(_point2);
	Vect * b = p2->minus(_point1)->r2D2(_i);
	Vect * p3 = new Vect(_point3);
	Vect * c = p3->minus(_point1)->r2D2(_i);

	float deno = b->det2D(c);
	float alfa = a->det2D(c) / deno;
	float beta = b->det2D(a) / deno;

	delete p2;
	delete p3;
	delete point;
	delete epsilon;

	if (alfa >= 0 && beta >= 0 && alfa + beta <= 1)
		return t;
	return 0.0f;
}

Vect * Triangle::getNormal(Vect * hit)
{
	return new Vect(_n);
}
