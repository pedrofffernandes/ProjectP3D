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

Vect::Vect(Vect * vect) {
	_x = vect->getX();
	_y = vect->getY();
	_z = vect->getZ();
}

Vect::~Vect()
{
}

// Normalization
/// Changes this vector to be normalized
/// A normalized vector, is the same vector
/// but with length = 1
Vect * Vect::normalize() {
	float length = this->length();
	_x = _x / length;
	_y = _y / length;
	_z = _z / length;
	return this;
}

// Vector Subtraction
/// Subtracts this vector with the given vector
Vect * Vect::minus(Vect * vect) {
	_x = _x - vect->getX();
	_y = _y - vect->getY();
	_z = _z - vect->getZ();
	return this;
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
	_x = _x * f;
	_y = _y * f;
	_z = _z * f;
	return this;
}

Vect * Vect::add(Vect * vect) {
	_x = _x + vect->getX();
	_y = _y + vect->getY();
	_z = _z + vect->getZ();
	return this;
}

Vect * Vect::lineP(Vect * vect)
{
	_x = _x * vect->getX();
	_y = _y * vect->getY();
	_z = _z * vect->getZ();
	return this;
}

Vect * Vect::positive()
{
	float x, y, z;
	x = (_x < 0) ? _x * -1 : _x;
	y = (_y < 0) ? _y * -1 : _y;
	z = (_z < 0) ? _z * -1 : _z;
	return new Vect(x, y, z);
}

int Vect::max()
{
	Vect* n = this->positive();
	if(n->getX() < n->getY()) {
		if (n->getY() < n->getZ()) {
			delete n;
			return 2;
		} else {
			delete n;
			return 1;
		}
	} else {
		if (n->getX() < n->getZ()) {
			delete n;
			return 2;
		} else {
			delete n;
			return 0;
		}
	}
}

float Vect::det2D(Vect * vect)
{
	return _x*vect->getY() - _y*vect->getX();
}

Vect * Vect::r2D2(int i)
{
	switch (i) {
	case 0:
		_x = _y;
		_y = _z;
		return this;
	case 1:
		_y = _z;
		return this;
	case 2:
		return this;
	}
	return nullptr;
}
