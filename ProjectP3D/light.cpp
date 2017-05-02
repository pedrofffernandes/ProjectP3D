#include "Light.h"

#define EPSILON 1e-3
#define RAND4 (int)rand() % (NUMEROAMOSTRAS * NUMEROAMOSTRAS * NUMEROAMOSTRAS_DOF * NUMEROAMOSTRAS_DOF)

Light::Light(Vect* position, Vect* rgb, int resx, int resy)
{
	_position = position;
	_rgb = rgb;
	_a = new Vect(LIGHT_SIZE, 0, 0);
	_b = new Vect(0, LIGHT_SIZE, 0);
	if (USE_ARRAY_SOFTSHADOWS) {
		buildArray(resx, resy);
	}
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
	_lightArray.reserve(resx * resy * NUMEROAMOSTRAS * NUMEROAMOSTRAS * NUMEROAMOSTRAS_DOF * NUMEROAMOSTRAS_DOF);
	
	for (int y = 0; y < resy; y++) {
		for (int x = 0; x < resx; x++) {

			for (int n = 0; n < NUMEROAMOSTRAS * NUMEROAMOSTRAS_DOF; n++) {
				for (int m = 0; m < NUMEROAMOSTRAS * NUMEROAMOSTRAS_DOF; m++) {
					_lightArray.push_back(positionSoft(n, m));
				}
			}
		}
	}
	int step = NUMEROAMOSTRAS * NUMEROAMOSTRAS * NUMEROAMOSTRAS_DOF * NUMEROAMOSTRAS_DOF;
	shuffleArray(resx, resy, step);
}

Vect * Light::positionSoft(int n, int m) {
	Vect * result = new Vect(_position);
	Vect * a = new Vect(_a);
	Vect * b = new Vect(_b);
	result->add(a->multiply(((n + (ERAND / NUMEROAMOSTRAS * NUMEROAMOSTRAS_DOF)) / (NUMEROAMOSTRAS * NUMEROAMOSTRAS_DOF))));
	result->add(b->multiply(((m + (ERAND / NUMEROAMOSTRAS * NUMEROAMOSTRAS_DOF)) / (NUMEROAMOSTRAS * NUMEROAMOSTRAS_DOF))));
	delete a;
	delete b;
	return result;
}

void Light::shuffleArray(int resx, int resy, int step) {
	int indexa = 0;
	for (int i = 0; i < resy; i++) {
		srand((unsigned) time(NULL));
		for (int ii = 0; ii < resx; ii++) {
			for (int j = 0; j < step; j++) {
				int r = RAND4;
				std::swap(_lightArray[indexa + j], _lightArray[indexa + r]);
			}
			indexa += step;
		}
	}
}