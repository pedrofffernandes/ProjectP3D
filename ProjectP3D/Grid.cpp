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

	/// Taking the error margin into account
	min_point_X -= (float) EPSILON;	min_point_Y -= (float) EPSILON;	min_point_Z -= (float) EPSILON;
	max_point_X += (float) EPSILON;	max_point_Y += (float) EPSILON;	max_point_Z += (float) EPSILON;
	/// P0 := min_point; P1 := max_point;
	Vect * min_point = new Vect(min_point_X, min_point_Y, min_point_Z);
	Vect * max_point = new Vect(max_point_X, max_point_Y, max_point_Z);

	_bbox = new BBox(min_point, max_point);
	
	//Compute Nx,Ny,Nz
	float wx = max_point->getX() - min_point->getX();
	float wy = max_point->getY() - min_point->getY();
	float wz = max_point->getZ() - min_point->getZ();

	float s = cbrt((wx*wy*wz) / num_objects);
	
	_Nx = static_cast<int> (trunc(M*wx / s) + 1);
	_Ny = static_cast<int> (trunc(M*wy / s) + 1);
	_Nz = static_cast<int> (trunc(M*wz / s) + 1);

	int cells_size = _Nx*_Ny*_Nz;

	for (int i = 0; i < cells_size;i++) {
		_cells->push_back(dynamic_cast<Cell*>(new Cell()));
	}

	for (std::list<Obj*>::iterator it = objects.begin(); it != objects.end(); it++) {
		bbox = ((Obj*)*it)->get_bounding_box();
		int ixmin = static_cast<int> (clamp((bbox->getMinX() - min_point->getX())* (float) _Nx / (max_point->getX() - min_point->getX()), 0, (float) _Nx - 1));
		int iymin = static_cast<int> (clamp((bbox->getMinY() - min_point->getY())* (float) _Ny / (max_point->getY() - min_point->getY()), 0, (float) _Ny - 1));
		int izmin = static_cast<int> (clamp((bbox->getMinZ() - min_point->getZ())* (float) _Nz / (max_point->getZ() - min_point->getZ()), 0, (float) _Nz - 1));
		int ixmax = static_cast<int> (clamp((bbox->getMaxX() - min_point->getX())* (float) _Nx / (max_point->getX() - min_point->getX()), 0, (float) _Nx - 1));
		int iymax = static_cast<int> (clamp((bbox->getMaxY() - min_point->getY())* (float) _Ny / (max_point->getY() - min_point->getY()), 0, (float) _Ny - 1));
		int izmax = static_cast<int> (clamp((bbox->getMaxZ() - min_point->getZ())* (float) _Nz / (max_point->getZ() - min_point->getZ()), 0, (float) _Nz - 1));

		for (int iz = izmin; iz <= izmax; iz++) {
			for (int iy = iymin; iy <= iymax; iy++) {
				for (int ix = ixmin; ix <= ixmax; ix++) {
					unsigned int index = ix + _Nx*iy + _Nx*_Ny*iz;
					_cells->at(index)->push((Obj*)*it);
				}
			}
		}

	}
	delete min_point;
	delete max_point;
	delete bbox;

	
}
/*
Intersection Grid::traverse(Ray* ray) {
	Intersection a;
	return a;
}
*/