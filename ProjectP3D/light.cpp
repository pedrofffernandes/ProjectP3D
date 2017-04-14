#include "Light.h"
#include <iostream>


Light::Light(Vect* position, Vect* rgb)
{
	_position = position;
	_rgb = rgb;
	_a = new Vect(1, 0, 0);
	_b = new Vect(0, 1, 0);
}


Light::~Light()
{
}

void Light::setColor(Vect * rgb) {
	_rgb = rgb;
}

// Shadow Feeler
/// Receives an intersection point with an object
/// and returns a vector that goes from the
/// intersection to the light source (Shadow Feeler)
Vect * Light::getLVect(Vect * hit) {	
	Vect * result = new Vect(_position);
	result->minus(hit);
	result->normalize();
	return result;
}

Vect * Light::getLVectSoft(Vect * hit)
{
	Vect * result = new Vect(_position);
	Vect * a = new Vect(_a);
	Vect * b = new Vect(_b);
	result->add(a->multiply(ERAND));
	result->add(b->multiply(ERAND));
	result->minus(hit);
	result->normalize();
	delete a;
	delete b;
	return result;
}

Vect * Light::getDiffuse(Vect * normal, Vect* L, Material * mat){

	float kd = mat->getKd();
	float nl = normal->dotP(L);
	
	Vect* result = new Vect(mat->getRGB());
	result->multiply(kd);
	result->multiply(nl);
	result->colorMultiplication(_rgb);
	return result;
}

Vect * Light::getSpecular(Vect * normal, Vect * L, Material * mat, Vect* v)
{
	float ks = mat->getKs();
	float shine = mat->getShine();

	Vect * I = new Vect(L);
	I->multiply(-1);
	Vect * V = new Vect(normal);
	V->multiply(-2 * I->dotP(normal));
	I->add(V);

	float rv = pow((I->dotP(v) < EPSILON) ? EPSILON : I->dotP(v), shine);

	Vect* result = new Vect(mat->getRGB());
	result->multiply(ks);
	result->multiply(rv);
	result->colorMultiplication(_rgb);

	delete I;
	delete V;
	return result;
}
