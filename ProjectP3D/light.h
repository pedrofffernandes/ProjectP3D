#pragma once
#include "Vect.h"
#include "Material.h"

class Light
{
	Vect* _position;
	Vect* _rgb;

public:
	Light(Vect* position, Vect* rgb);
	~Light();
	void setColor(Vect * rgb);
	Vect * getLVect(Vect * hit);
	Vect * getDiffuse(Vect* normal, Vect* L, Material* mat);
	Vect * getSpecular(Vect* normal, Vect* L, Material* mat, Vect* v);
};

