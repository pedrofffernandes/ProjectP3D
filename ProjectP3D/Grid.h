#pragma once
// CPP files
#include <list>
#include<vector>
// Project Files
#include "BBox.h"
#include "obj.h"
#include "Cell.h"

// Constants
#define EPSILON 1e-4
#define HUGE_VALUE 1000000
#define M 2

class Grid
{
	// This is a bounding box made from the bounding boxes
	// of all the scene objects
	BBox * _bbox;
	float _Nx;
	float _Ny;
	float _Nz;
	std::vector<Cell*>* _cells;

public:
	Grid();
	~Grid();
	void setup(std::list<Obj*>);
	float clamp(float n, float min, float max) { return n <= min ? min : n >= max ? max : n; }
};

