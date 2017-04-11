#include "Grid.h"



Grid::Grid()
{
}


Grid::~Grid()
{
}

void Grid::setup(std::list<Obj*> objects)
{	//Compute grid's Bounding Box
	BBox * bbox;
	float min_point_X = HUGE_VALUE;
	float min_point_Y = HUGE_VALUE;
	float min_point_Z = HUGE_VALUE;
	float max_point_X = -HUGE_VALUE;
	float max_point_Y = -HUGE_VALUE;
	float max_point_Z = -HUGE_VALUE;

	int num_objects = objects.size();

	for (std::list<Obj*>::iterator it = objects.begin(); it != objects.end(); it++) {
		bbox = ((Obj*)*it)->get_bounding_box();
		/// Bounding box minimum coordinate
		if (bbox->getMinX() < min_point_X)
			min_point_X = bbox->getMinX();
		if (bbox->getMinY() < min_point_Y)
			min_point_Y = bbox->getMinY();
		if (bbox->getMinZ() < min_point_Z)
			min_point_Z = bbox->getMinZ();

		/// Bounding box miximum coordinate
		if (bbox->getMaxX() < max_point_X)
			max_point_X = bbox->getMaxX();
		if (bbox->getMaxY() < max_point_Y)
			max_point_Y = bbox->getMaxY();
		if (bbox->getMaxZ() < max_point_Z)
			max_point_Z = bbox->getMaxZ();
	}

	Vect * min_point = new Vect(min_point_X - EPSILON, min_point_Y - EPSILON, min_point_Z - EPSILON);
	Vect * max_point = new Vect(max_point_X + EPSILON, max_point_Y + EPSILON, max_point_Z + EPSILON);

	_bbox = new BBox(min_point, max_point);
	
	//Compute Nx,Ny,Nz
	float wx = max_point->getX() - min_point->getX();
	float wy = max_point->getY() - min_point->getY();
	float wz = max_point->getZ() - min_point->getZ();

	float s = cbrt((wx*wy*wz) / num_objects);
	
	_Nx = trunc(M*wx / s) + 1;
	_Ny = trunc(M*wy / s) + 1;
	_Nz = trunc(M*wz / s) + 1;

	float cells_size = _Nx*_Ny*_Nz;

	for (int i = 0; i < cells_size;i++) {
		_cells->push_back(dynamic_cast<Cell*>(new Cell()));
	}

	for (std::list<Obj*>::iterator it = objects.begin(); it != objects.end(); it++) {
		bbox = ((Obj*)*it)->get_bounding_box();
		int ixmin = clamp((bbox->getMinX()-min_point->getX())*_Nx/(max_point->getX()-min_point->getX()),0,_Nx-1);
		int iymin = clamp((bbox->getMinY() - min_point->getY())*_Ny / (max_point->getY() - min_point->getY()), 0, _Ny - 1);
		int izmin = clamp((bbox->getMinZ() - min_point->getZ())*_Nz / (max_point->getZ() - min_point->getZ()), 0, _Nz - 1);
		int ixmax = clamp((bbox->getMaxX() - min_point->getX())*_Nx / (max_point->getX() - min_point->getX()), 0, _Nx - 1);
		int iymax = clamp((bbox->getMaxY() - min_point->getY())*_Ny / (max_point->getY() - min_point->getY()), 0, _Ny - 1);
		int izmax = clamp((bbox->getMaxZ() - min_point->getZ())*_Nz / (max_point->getZ() - min_point->getZ()), 0, _Nz - 1);

		for (int iz = izmin; iz <= izmax; iz++) {
			for (int iy = iymin; iy <= iymax; iy++) {
				for (int ix = ixmin; ix <= ixmax; ix++) {
					float index = ix + _Nx*iy + _Nx*_Ny*iz;
					_cells->at(index)->push((Obj*)*it);
				}
			}
		}

	}
	delete min_point;
	delete max_point;
	delete bbox;

	
}

Intersection Grid::traverse(Ray* ray) {
	Intersection a;
	return a;
}
