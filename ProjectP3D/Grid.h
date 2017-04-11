#pragma once
// CPP files
#include <list>
// Project Files
#include "BBox.h"
#include "obj.h"

// Constants
#define EPSILON 1e-4
#define HUGE_VALUE 1000000

class Grid
{
	// This is a bounding box made from the bounding boxes
	// of all the scene objects
	BBox * _bbox;
public:
	Grid();
	~Grid();
	void computeBBox(std::list<Obj*>);
};

