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


// Shadow Feeler
/// Receives an intersection point with an object
/// and returns a vector that goes from the
/// intersection to the light source (Shadow Feeler)
Vect * Light::getLVect(Vect * hit)
{	
	Vect * shadow_feeler = _position->minus(hit)->normalize();
	return shadow_feeler;
}

Vect * Light::getDiffuse(Vect * normal, Vect* L, Material * mat){

	float kd = mat->getKd();
	float nl = normal->dotP(L);
	//std::cout << nl << '\n';
	
	Vect* result = mat->getRGB()->multiply(kd)->multiply(nl);
	result = (_rgb->lineP(result));
	return result;
}

Vect * Light::getSpecular(Vect * normal, Vect * L, Material * mat, Vect* v)
{
	float ks = mat->getKs();
	float shine = mat->getShine();

	Vect * I = L->multiply(-1);
	Vect * V = normal->multiply(-2 * I->dotP(normal));
	Vect * R = I->add(V);

	float rv = pow((R->dotP(v) < EPSILON) ? EPSILON : R->dotP(v), shine);

	Vect* result = mat->getRGB()->multiply(ks)->multiply(rv);
	result = (_rgb->lineP(result));

	delete I;
	delete V;
	delete R;

	return result;
}
