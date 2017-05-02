#pragma once
#ifndef __GRID_H__
#define __GRID_H__

// CPP files
#include <list>
#include <vector>
// Project Files
#include "BBox.h"
#include "obj.h"
#include "Cell.h"
#include "Constants.h"
#include "Structs.h"
#include "AuxFunctions.h"

class Grid
{
	// This is a bounding box made from the bounding boxes
	// of all the scene objects
	BBox * _bbox;
	unsigned int _Nx; /// Number of cells in the x axis
	unsigned int _Ny; /// Number of cells in the y axis
	unsigned int _Nz; /// Number of cells in the z axis
	std::vector<Cell*> _cells; /// structure containning the cells

public:
	Grid();
	~Grid();
	void setup(std::list<Obj*>);
	//Intersection traverse(Ray* ray);
	limit * kkAlgorithmn(Ray*);
	intersection * traverse(Ray*);
	intersection * traverseInShadow(Ray*);
	unsigned int indexOfCell(int, int, int); /// Returns the index 
	int clamp(float n, float min, float max) {
		return n <= min ? min : n >= max ? max : n;
	}
	unsigned int numberOfObjects() { return _Nx*_Ny*_Nz / pow(M, 3); }
	unsigned int numberOfCells() { return _Nx*_Ny*_Nz; }
};

#endif // !__GRID_H__
