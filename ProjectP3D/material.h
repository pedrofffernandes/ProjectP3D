#pragma once
#include "vect.h"

class Material
{
	Vect* _rgb;
	float _kd;
	float _ks;
	float _shine;
	float _T; // transmitance
	float _refraction; // index of refraction
public:
	Material(Vect* rgb, float kd, float ks, float shine, float T, float refraction);
	~Material();

	Vect* getRGB();
	float getKd();
	float getKs();
};

