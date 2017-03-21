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

// Length (Magnitude)
/// Returns the vector magnitude (length)
float Vect::length() {
	return sqrt(_x*_x + _y*_y + _z*_z);
}

// Scalar Multiplication
/// CHANGES THIS VECTOR
/// returns this vector multiplyed by a scalar
Vect * Vect::multiply(float f) {
	_x = _x * f;
	_y = _y * f;
	_z = _z * f;
	return this;
}


// Vector Addition
/// CHANGES THIS VECTOR
/// returns this vector added by another vector
Vect * Vect::add(Vect * vect) {
	_x = _x + vect->getX();
	_y = _y + vect->getY();
	_z = _z + vect->getZ();
	return this;
}

// Color multiplication
/// ONLY USE when the applying the Vect class to <COLOR>
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


// Vector Determinant
/// ONLY APPLIES TO <2D_VECTORS>
/// computes and returns the determinant of the vector
float Vect::det2D(Vect * vect)
{
	return _x*vect->getY() - _y*vect->getX();
}


// Projection of a 3D Vector into 2D
/// Part of the Badouel's Ray-Triangle Intersection algorithmn
/// used in the Triangle::Intersect(Ray*) method
/// Motivation: when using 2D Vectors only
/// the first 2 coordinates (x,y) are used
/// the z coordinate value is ignored
/// ex: when projecting in the x plane,
/// the x coordinate value must be cut off
/// and the y and z coordinates must be preserved
/// so it returns this 3D Vector but with the
/// y value in the x coordinate and the z value
/// in the y coordinate
Vect * Vect::r2D2(int i)
{
	switch (i) {
		case 0:
			/// Projecting in the X plane
			_x = _y;
			_y = _z;
			break;
		case 1:
			/// Projecting in the Y plane
			_y = _z;
			break;
		case 2:
			/// Projecting in the Z plane
			break;
	}
	return this;
}
