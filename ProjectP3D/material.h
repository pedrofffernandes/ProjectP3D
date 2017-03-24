#pragma once
#include "vect.h"

class Material
{
	Vect* _rgb;		/// Color of the Object
	float _kd;		/// Index of Reflection (diffuse)
	float _ks;		/// Index of Reflection (specular)
	float _shine;	/// Shininess (for the specular reflection)
	float _T;		/// transmitance (Emission) (how much colour will be emitted from the object
	float _ior;		/// index of refraction
public:
	Material(Vect* rgb, float kd, float ks, float shine, float T, float refraction);
	~Material();

	Vect* getRGB();
	float getKd();
	float getKs();
	float getShine();
	float getT();
	float getIOR();
};

