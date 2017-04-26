#pragma once
// CPP files
#include <list>
#include<vector>
// Project Files
#include "BBox.h"
#include "obj.h"
#include "Cell.h"
#include "Constants.h"
#include "Structs.h"



class Grid
{
	// This is a bounding box made from the bounding boxes
	// of all the scene objects
	BBox * _bbox;
	unsigned int _Nx; /// Number of cells in the x axis
	unsigned int _Ny; /// Number of cells in the y axis
	unsigned int _Nz; /// Number of cells in the z axis
	std::vector<Cell*>* _cells; /// structure containning the cells

public:
	Grid();
	~Grid();
	void setup(std::list<Obj*>);
	//Intersection traverse(Ray* ray);
	float clamp(float n, float min, float max) { return n <= min ? min : n >= max ? max : n; }
	limit * kkAlgorithmn(Ray*);
	intersection * traverse(Ray*);

};
