#include "Vect.h"

// Vector class to define points and arrays

Vect::Vect()
{
	_x = 0;
	_y = 0;
	_z = 0;
}

Vect::Vect(float x, float y, float z)
{
	_x = x;
	_y = y;
	_z = z;
}

Vect::~Vect()
{
}

Vect * Vect::normalize() {
	float length = this->length();
	float x, y, z;
	x = _x / length;
	y = _y / length;
	z = _z / length;
	return new Vect(x, y, z);
}

Vect * Vect::minus(Vect * vect) {
	return new Vect(_x - vect->getX(), _y - vect->getY(), _z - vect->getZ());
}

Vect * Vect::crossP(Vect * vect) {
	float x, y, z;
	x = (_y * vect->getZ()) - (_z * vect->getY());
	y = - ((_x * vect->getZ()) - (_z * vect->getX()));
	z = (_x * vect->getY()) - (_y * vect->getX());
	return new Vect(x, y, z);
}

float Vect::dotP(Vect * vect) {
	return _x * vect->getX() + _y * vect->getY() + _z * vect->getZ();
}

float Vect::length() {
	return sqrt(_x*_x + _y*_y + _z*_z);
}

Vect * Vect::multiply(float f) {
	float x, y, z;
	x = _x * f;
	y = _y * f;
	z = _z * f;
	return new Vect(x, y, z);
}

Vect * Vect::add(Vect * vect) {
	float x, y, z;
	x = _x + vect->getX();
	y = _y + vect->getY();
	z = _z + vect->getZ();
	return new Vect(x, y, z);
}