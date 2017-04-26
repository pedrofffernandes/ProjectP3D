#pragma once
#ifndef __STRUCTS_H__
#define __STRUCTS_H__
#include "Vect.h"
#include "obj.h"

// Declarations
/// <limit> Used in Kay and Kajyia Algorithmn
/// to specify a min point and a max point
struct limit;
struct intersection;

// Definitions
struct limit {
	Vect * min;
	Vect * max;
};
struct intersection {
	Vect * hit;
	Obj * object;
};

#endif // __STRUCTS_H__