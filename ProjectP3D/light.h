#pragma once
#include "Vect.h"
#include "Material.h"
#include <random>
#define ERAND (float)rand()/RAND_MAX


class Light
{
	Vect* _position;
	Vect* _rgb;
	//Soft Shadows light edge a
	Vect* _a;
	//Soft Shadows light edge b
	Vect* _b;


public:
	Light(Vect* position, Vect* rgb);
	~Light();
	void setColor(Vect * rgb);
	Vect * getLVect(Vect * hit);
	Vect * getLVectSoft(Vect * hit);
	Vect * getDiffuse(Vect* normal, Vect* L, Material* mat);
	Vect * getSpecular(Vect* normal, Vect* L, Material* mat, Vect* v);
};

