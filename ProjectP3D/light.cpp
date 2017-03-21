#include "Light.h"

#define EPSILON 1e-3


Light::Light(Vect* position, Vect* rgb)
{
	_position = position;
	_rgb = rgb;
}


Light::~Light()
{
}

void Light::setColor(Vect * rgb) {
	_rgb = rgb;
}

Vect * Light::getLVect(Vect * hit)
{	
	Vect * lVect = _position->minus(hit);
	lVect = lVect->normalize();
	return lVect;
}

Vect * Light::getDiffuse(Vect * normal, Vect* L, Material * mat){

	float kd = mat->getKd();
	float nl = normal->dotP(L);
	
	Vect* result = new Vect(mat->getRGB());
	result->multiply(kd);
	result->multiply(nl);
	result->lineP(_rgb);
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
	result->lineP(_rgb);

	delete I;
	delete V;
	return result;
}
