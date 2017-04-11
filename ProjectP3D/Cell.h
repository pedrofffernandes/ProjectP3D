#pragma once
#include <list>
#include "Obj.h"
#include "Vect.h"

class Cell
{
	std::list<Obj*>* _objects;
	

public:
	Cell();
	~Cell();
	void push(Obj* obj) { _objects->push_back(obj);}
};
