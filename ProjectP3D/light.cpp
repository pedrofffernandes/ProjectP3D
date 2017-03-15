#include "Light.h"



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
	lVect->normalize();
	return lVect;
}

Vect * Light::getDiffuse(Vect * normal, Material * mat){

	//Vect* result = mat->getRGB
	return nullptr;
}
