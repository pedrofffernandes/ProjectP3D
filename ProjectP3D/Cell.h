#pragma once
#ifndef __CELL_H__
#define __CELL_H__
// C++ Headers
#include <list>
// Project Files Headers
#include "Obj.h"
#include "Vect.h"
#include "Ray.h"
#include "Structs.h"
#include "Constants.h"

class Cell
{
	std::list<Obj*> _objects;

public:
	Cell();
	~Cell();
	void push(Obj* obj) { _objects.push_back(obj); }
	std::list<Obj*> getObjects() { return _objects; }
	bool isEmpty() { return _objects.empty(); }
	intersection* hit(Ray*);
};

#endif // !__CELL_H__