#pragma once
#include "Vect.h"
#include "Material.h"
#include "Constants.h"
#include <random>
#include <algorithm>
#define ERAND (float)rand()/RAND_MAX


class Light
{
	Vect* _position;
	Vect* _rgb;
	//Soft Shadows light edge a
	Vect* _a;
	//Soft Shadows light edge b
	Vect* _b;

	std::vector<Vect*> _lightArray;


public:
	Light(Vect* position, Vect* rgb, int resx, int resy);
	~Light();
	void setColor(Vect * rgb);
	Vect * getLVect(Vect * hit);
	Vect * getLVectSoft(Vect * hit);
	Vect * getDiffuse(Vect* normal, Vect* L, Material* mat);
	Vect * getSpecular(Vect* normal, Vect* L, Material* mat, Vect* v);
	Vect * getLVectArrays(Vect * hit, int index);
	void buildArray(int resx, int resy);
	void buildArrayDOF(int resx, int resy);
	Vect * positionSoft(int n, int m);
};

