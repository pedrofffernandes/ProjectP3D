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
/// Changes this vector
/// Subtracts this vector with the given vector
Vect * Vect::minus(Vect * vect) {
	_x = _x - vect->getX();
	_y = _y - vect->getY();
	_z = _z - vect->getZ();
	return this;
}


// Cross Product
/// Receives a vector and returns a new
/// vector which results in the cross product
/// of this vector with the given vector
Vect * Vect::crossP(Vect * vect) {
	float x, y, z;
	x = (_y * vect->getZ()) - (_z * vect->getY());
	y = - ((_x * vect->getZ()) - (_z * vect->getX()));
	z = (_x * vect->getY()) - (_y * vect->getX());
	return new Vect(x, y, z);
}

// Dot Product
/// Receives a vector and returns a the 
/// the result of the dot product between
/// this vector and the given one
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

// Color multiplication
/// ONLY USE when the applying the Vect class to colors
/// returns a color multiplied by the vector coordinates
Vect * Vect::colorMultiplication(Vect * vect)
{
	/// Multiplies the value of the RED value by X
	_x = _x * vect->getX();
	/// Multiplies the value of the GREEN value by Y
	_y = _y * vect->getY();
	/// Multiplies the value of the BLUE value by Z
	_z = _z * vect->getZ();
	return this;
}

// Positive
/// Converts all the negative coordinates
/// of the vector to their symmetric
Vect * Vect::positive()
{
	float x, y, z;
	x = (_x < 0) ? _x * -1 : _x;
	y = (_y < 0) ? _y * -1 : _y;
	z = (_z < 0) ? _z * -1 : _z;
	return new Vect(x, y, z);
}

// Max
/// Returns the an integer that serves
/// to identify, which coordinate has the
/// highest value
/// IF x has the highest value, 0 is returned
/// IF y has the highest value, 1 is returned
/// IF z has the highest value, 2 is returned
int Vect::max()
{
	Vect* n = this->positive();
	if (n->getX() >= n->getY() && n->getX() >= n->getZ())
		return 0; /// X has the highest value
	if (n->getY() >= n->getZ())
		return 1; /// Y has the highest value
	return 2;  /// Z has the highest value
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
