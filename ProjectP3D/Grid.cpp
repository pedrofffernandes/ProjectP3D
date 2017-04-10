#include "Grid.h"



Grid::Grid()
{
}


Grid::~Grid()
{
}

void Grid::computeBBox(std::list<Obj*> objects)
{
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

	this->_bbox = new BBox(min_point, max_point);
	
	delete min_point;
	delete max_point;
}
