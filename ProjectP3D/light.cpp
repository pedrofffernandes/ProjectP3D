#include "Light.h"
#include <iostream>

#define EPSILON 1e-3
#define RAND rand() % (NUMEROAMOSTRAS * NUMEROAMOSTRAS)

Light::Light(Vect* position, Vect* rgb, int resx, int resy)
{
	_position = position;
	_rgb = rgb;
	_a = new Vect(1, 0, 0);
	_b = new Vect(0, 1, 0);
	if(USE_ARRAY_SOFTSHADOWS) buildArray(resx, resy);
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

Vect * Light::getLVectSoft(Vect * hit) {
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

Vect * Light::getLVectArrays(Vect * hit, int index) {
	Vect * result = new Vect(_lightArray[index]);
	result->minus(hit);
	result->normalize();
	return result;
}

void Light::buildArray(int resx, int resy) {
	_lightArray.reserve(resx * resy * NUMEROAMOSTRAS * NUMEROAMOSTRAS);

	for (int y = 0; y < resy; y++) {
		for (int x = 0; x < resx; x++) {

			for (int n = 0; n < NUMEROAMOSTRAS; n++) {
				for (int m = 0; m < NUMEROAMOSTRAS; m++) {
					_lightArray.push_back(positionSoft(n, m));
				}
			}
		}
	}
	shuffleArray(resx, resy);
}

void Light::buildArrayDOF(int resx, int resy) {
	_lightArray.reserve(resx * resy * NUMEROAMOSTRAS * NUMEROAMOSTRAS * NUMEROAMOSTRAS_DOF * NUMEROAMOSTRAS_DOF);
	std::vector<Vect*>::iterator itb = _lightArray.begin();
	std::vector<Vect*>::iterator ite = _lightArray.end();
	for (int y = 0; y < resy; y++) {
		for (int x = 0; x < resx; x++) {

			for (int n = 0; n < NUMEROAMOSTRAS; n++) {
				for (int m = 0; m < NUMEROAMOSTRAS; m++) {

					for (int o = 0; o < NUMEROAMOSTRAS_DOF; o++) {
						for (int q = 0; q < NUMEROAMOSTRAS_DOF; q++) {
							//_lightArray.push_back(positionSoft(int p, int q));			//FIXME
							ite++;
						}
					}
					std::random_shuffle(itb, ite);
					itb = ite;
				}
			}
		}
	}
}

Vect * Light::positionSoft(int n, int m) {
	Vect * result = new Vect(_position);
	Vect * a = new Vect(_a);
	Vect * b = new Vect(_b);
	result->add(a->multiply((n + ERAND) / NUMEROAMOSTRAS));
	result->add(b->multiply((m + ERAND) / NUMEROAMOSTRAS));
	delete a;
	delete b;
	return result;
}
}