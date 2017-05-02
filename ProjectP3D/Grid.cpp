#include "Grid.h"



Grid::Grid()
{
	_bbox = nullptr;
}


Grid::~Grid()
{
	delete _bbox;
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

		/// Bounding box maximum coordinate
		if (bbox->getMaxX() > max_point_X)
			max_point_X = bbox->getMaxX();
		if (bbox->getMaxY() > max_point_Y)
			max_point_Y = bbox->getMaxY();
		if (bbox->getMaxZ() > max_point_Z)
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

	/// cbrt is the cubic root
	float s = cbrt((wx*wy*wz) / num_objects);
	
	_Nx = static_cast<int> (ceil(M*wx / s));
	_Ny = static_cast<int> (ceil(M*wy / s));
	_Nz = static_cast<int> (ceil(M*wz / s));

	int cells_size = _Nx*_Ny*_Nz;
	
	/// inicializacao das celulas da grip a empty
	/// isto para depois se poder aceder a elas
	for (int i = 0; i < cells_size;i++) {
		_cells.push_back(dynamic_cast<Cell*>(new Cell()));
	}
	
	// Store the objects in the cells
	for (std::list<Obj*>::iterator it = objects.begin(); it != objects.end(); it++) {
		/// Compute indices of both cells that contain min and max coord of obj bbox
		bbox = ((Obj*)*it)->get_bounding_box();
		int ixmin = static_cast<int> (clamp((bbox->getMinX() - min_point->getX())* (float) _Nx / (max_point->getX() - min_point->getX()), 0, (float) _Nx - 1));
		int iymin = static_cast<int> (clamp((bbox->getMinY() - min_point->getY())* (float) _Ny / (max_point->getY() - min_point->getY()), 0, (float) _Ny - 1));
		int izmin = static_cast<int> (clamp((bbox->getMinZ() - min_point->getZ())* (float) _Nz / (max_point->getZ() - min_point->getZ()), 0, (float) _Nz - 1));
		int ixmax = static_cast<int> (clamp((bbox->getMaxX() - min_point->getX())* (float) _Nx / (max_point->getX() - min_point->getX()), 0, (float) _Nx - 1));
		int iymax = static_cast<int> (clamp((bbox->getMaxY() - min_point->getY())* (float) _Ny / (max_point->getY() - min_point->getY()), 0, (float) _Ny - 1));
		int izmax = static_cast<int> (clamp((bbox->getMaxZ() - min_point->getZ())* (float) _Nz / (max_point->getZ() - min_point->getZ()), 0, (float) _Nz - 1));
		
		/// Insert the object in the overlaped cells
		for (int iz = izmin; iz <= izmax; iz++) {
			for (int iy = iymin; iy <= iymax; iy++) {
				for (int ix = ixmin; ix <= ixmax; ix++) {
					unsigned int index = ix + _Nx*iy + _Nx*_Ny*iz;
					_cells.at(index)->push((Obj*)*it);
				}
			}
		}
		
		
	}
	/// Delete pointers
	delete min_point;
	delete max_point;
	delete bbox;
	
	
}
limit * Grid::kkAlgorithmn(Ray * r)
{
	// 1) Initialize t_min and t_max
	float tnear = -HUGE_VALUE;
	float tfar = HUGE_VALUE;

	/// Outher auxiliar values
	float Vo = 0;
	float Vd = 0;
	float Vmin = 0;
	float Vmax = 0;
	float tmin = 0;
	float tmax = 0;

	Vect * min = new Vect(- HUGE_VALUE, -HUGE_VALUE, -HUGE_VALUE);
	Vect * max = new Vect(HUGE_VALUE, HUGE_VALUE, HUGE_VALUE);

	for (int i = 0; i < 3; i++) {
		// 2) Initializing the Value
		switch (i) {
		case 0: /// X coordinate
			Vo = r->getO()->getX();
			Vd = r->getD()->getX();
			Vmin = _bbox->getMinX();
			Vmax = _bbox->getMaxX();
			break;
		case 1: /// Y coordinate
			Vo = r->getO()->getY();
			Vd = r->getD()->getY();
			Vmin = _bbox->getMinY();
			Vmax = _bbox->getMaxY();
			break;
		case 2: /// Z coordinate
			Vo = r->getO()->getZ();
			Vd = r->getD()->getZ();
			Vmin = _bbox->getMinZ();
			Vmax = _bbox->getMaxZ();
			break;
		}
		// 3) Check if ray doesnt intersect
		if (Vd == 0) {
			/// Ray is parallel
			if (Vo < Vmin || Vo > Vmax)
				/// Ray doesnt intersect
				return nullptr;
			else {
				switch (i) {
				case 0: /// X coordinate
					min->setX(Vmin - Vo);
					max->setX(Vmax - Vo);
					break;
				case 1: /// Y coordinate
					min->setY(Vmin - Vo);
					max->setY(Vmax - Vo);
					break;
				case 2: /// Z coordinate
					min->setZ(Vmin - Vo);
					max->setZ(Vmax - Vo);
					break;
				}
				/// go to next coordinate
				break;
			}
		}
		// 4) Compute Intersections
		tmin = (Vmin - Vo) / Vd;
		tmax = (Vmax - Vo) / Vd;
		/// switch values if tmin < tmax
		if (tmin > tmax) {
			float aux = tmin;
			tmin = tmax;
			tmax = aux;
		}
		/// update the min and max vectors
		switch (i) {
		case 0: /// X coordinate
			min->setX(tmin);
			max->setX(tmax);
			break;
		case 1: /// Y coordinate
			min->setY(tmin);
			max->setY(tmax);
			break;
		case 2: /// Z coordinate
			min->setZ(tmin);
			max->setZ(tmax);
			break;
		}
		/// update tmin and tmax
		if (tmin > tnear) tnear = tmin;
		if (tmax < tfar) tfar = tmax;
		// 5)
		if (tnear > tfar) return nullptr;
		// 6)
		/// ray points in the opposite direction
		if (tfar < 0) return nullptr;
	} // END of FOR

	// Compute intersection points
	limit * l = new limit();
	l->near = tnear;
	l->far = tfar;
	l->min = min;
	l->max = max;

	return l;
}

intersection * Grid::traverse(Ray * ray)
{
	// Check if ray hits the grid
	limit * l = kkAlgorithmn(ray);
	if (l == nullptr)
		return nullptr;
	// Compute the starting Cell indexes
	int ix, iy, iz;
	/// Ray Origin Coordinates
	float ox = ray->getO()->getX();
	float oy = ray->getO()->getY();
	float oz = ray->getO()->getZ();
	/// Grid BBox min point vertices
	float min_x = _bbox->getMinX();
	float min_y = _bbox->getMinY();
	float min_z = _bbox->getMinZ();
	/// Grid BBox max point vertices
	float max_x = _bbox->getMaxX();
	float max_y = _bbox->getMaxY();
	float max_z = _bbox->getMaxZ();
	/// First intersection point coordinates
	float tmin_x = l->min->getX(); // ERRO
	float tmin_y = l->min->getY(); // ERRO
	float tmin_z = l->min->getZ(); // ERRO
	/// Second intersection point coordinates
	float tmax_x = l->max->getX(); // ERRO
	float tmax_y = l->max->getY(); // ERRO
	float tmax_z = l->max->getZ(); // ERRO
	/// Check if the ray comes from inside the GRID
	if (_bbox->inside(ray->getO())) {
		ix = clamp((ox - min_x) * _Nx / (max_x - min_x), 0, _Nx - 1);
		iy = clamp((oy - min_y) * _Ny / (max_y - min_y), 0, _Ny - 1);
		iz = clamp((oz - min_z) * _Nz / (max_z - min_z), 0, _Nz - 1);
	} else {
		Vect * p = new Vect(ray->getD());
		p->multiply(l->near);
		p->add(ray->getO());
		ix = clamp((p->getX() - min_x) * _Nx / (max_x - min_x), 0, _Nx - 1);
		iy = clamp((p->getY() - min_y) * _Ny / (max_y - min_y), 0, _Ny - 1);
		iz = clamp((p->getZ() - min_z) * _Nz / (max_z - min_z), 0, _Nz - 1);
	}
	// traverse the grid
	/// 1) Set Up Grid Traversal
	float dtx = (tmax_x - tmin_x) / _Nx;
	float dty = (tmax_y - tmin_y) / _Ny;
	float dtz = (tmax_z - tmin_z) / _Nz;

	float tnext_x, tnext_y, tnext_z;
	int istep_x, istep_y, istep_z;
	int istop_x, istop_y, istop_z;
	
	// Tnext_X
	if (ray->getD()->getX() > 0) {
		tnext_x = tmin_x + (ix + 1) * dtx;
		istep_x = +1;
		istop_x = _Nx;
	}
	else if (ray->getD()->getX() < 0) {
		tnext_x = tmin_x + (_Nx - ix) * dtx;
		istep_x = -1;
		istop_x = -1;
	}
	else {
		tnext_x = HUGE_VALUE; /// This way it won't advance in this direction
		istep_x = -1;
		istop_x = -1;
	}
	// Tnext_Y
	if (ray->getD()->getY() > 0) {
		tnext_y = tmin_y + (iy + 1) * dty;
		istep_y = +1;
		istop_y = _Ny;
	}
	else if (ray->getD()->getY() < 0) {
		tnext_y = tmin_y + (_Ny - iy) * dty;
		istep_y = -1;
		istop_y = -1;
	}
	else {
		tnext_y = HUGE_VALUE; /// This way it won't advance in this direction
		istep_y = -1;
		istop_y = -1;
	}
	// Tnext_Z
	if (ray->getD()->getZ() > 0) {
		tnext_z = tmin_z + (iz + 1) * dtz;
		istep_z = +1;
		istop_z = _Nz;
	}
	else if (ray->getD()->getZ() < 0) {
		tnext_z = tmin_z + (_Nz - iz) * dtz;
		istep_z = -1;
		istop_z = -1;
	}
	else {
		tnext_z = HUGE_VALUE; /// This way it won't advance in this direction
		istep_z = -1;
		istop_z = -1;
	}

	// Grid Traversal Loop
	while (true) {
		/// Get Cell at current index
		Cell * cell_ptr = _cells.at(indexOfCell(ix, iy, iz));

		/// Check which cell will the ray traverse next (in x, y or z)
		if (tnext_x < tnext_y && tnext_x < tnext_z) {
			// ray will traverse in X
			/// IF the cell is NOT empty
			if (!cell_ptr->isEmpty()) {
				/// Compute the intersection
				intersection * i = cell_ptr->hit(ray);
				/// check if the ray intersected the object 
				if (i->object != nullptr) {
					/// compute the distance in X to the intersection point
					float t = i->distance;  // ERRO
					/// check if the intersection its on the current cell
					if (t < tnext_x) {  // ERRO
						/// if it is, then its the object we wanted
						return i;
					}
				}
				delete i;
			}
			/// ELSE it will advance to the next cell
			tnext_x += dtx;
			ix += istep_x;
			/// IF it reached the last cell
			if (ix == istop_x)
				/// return NO INTERSECTION
				return nullptr;

		}
		else {
			if (tnext_y < tnext_z) {
				// ray will traverse in Y
				/// IF the cell is NOT empty
				if (!cell_ptr->isEmpty()) {
					/// Compute the intersection
					intersection * i = cell_ptr->hit(ray);
					/// check if the ray intersected the object 
					if (i->object != nullptr) {
						/// compute the distance in X to the intersection point
						float t = i->distance;  // ERRO
						/// check if the intersection its on the current cell
						if (t < tnext_y) {  // ERRO
							/// if it is, then its the object we wanted
							return i;
						}
					}
					delete i;
				}
				/// ELSE it will advance to the next cell
				tnext_y += dty;
				iy += istep_y;
				/// IF it reached the last cell
				if (iy == istop_y)
					/// return NO INTERSECTION
					return nullptr;
			}
			else {
				// ray will traverse in Z
				/// IF the cell is NOT empty
				if (!cell_ptr->isEmpty()) {
					/// Compute the intersection
					intersection * i = cell_ptr->hit(ray);
					/// check if the ray intersected the object 
					if (i->object != nullptr) {
						/// compute the distance in X to the intersection point
						float t = i->distance;  // ERRO
						/// check if the intersection its on the current cell
						if (t < tnext_z) {  // ERRO
							/// if it is, then its the object we wanted
							return i;
						}
					}
					delete i;
				}
				/// ELSE it will advance to the next cell
				tnext_z += dtz;
				iz += istep_z;
				/// IF it reached the last cell
				if (iz == istop_z)
					/// return NO INTERSECTION
					return nullptr;
			}
		}
	}
	
	/// It wont reach this state
	return nullptr;
}

unsigned int Grid::indexOfCell(int ix, int iy, int iz)
{
	return ix + (_Nx*iy) + (_Nx*_Ny*iz);
}



intersection * Grid::traverseInShadow(Ray * ray)
{
	// Check if ray hits the grid
	limit * l = kkAlgorithmn(ray);
	if (l == nullptr)
		return nullptr;
	// Compute the starting Cell indexes
	int ix, iy, iz;
	/// Ray Origin Coordinates
	float ox = ray->getO()->getX();
	float oy = ray->getO()->getY();
	float oz = ray->getO()->getZ();
	/// Grid BBox min point vertices
	float min_x = _bbox->getMinX();
	float min_y = _bbox->getMinY();
	float min_z = _bbox->getMinZ();
	/// Grid BBox max point vertices
	float max_x = _bbox->getMaxX();
	float max_y = _bbox->getMaxY();
	float max_z = _bbox->getMaxZ();
	/// First intersection point coordinates
	float tmin_x = l->min->getX(); // ERRO
	float tmin_y = l->min->getY(); // ERRO
	float tmin_z = l->min->getZ(); // ERRO
								   /// Second intersection point coordinates
	float tmax_x = l->max->getX(); // ERRO
	float tmax_y = l->max->getY(); // ERRO
	float tmax_z = l->max->getZ(); // ERRO
								   /// Check if the ray comes from inside the GRID
	if (_bbox->inside(ray->getO())) {
		ix = clamp((ox - min_x) * _Nx / (max_x - min_x), 0, _Nx - 1);
		iy = clamp((oy - min_y) * _Ny / (max_y - min_y), 0, _Ny - 1);
		iz = clamp((oz - min_z) * _Nz / (max_z - min_z), 0, _Nz - 1);
	}
	else {
		Vect * p = new Vect(ray->getD());
		p->multiply(l->near);
		p->add(ray->getO());
		ix = clamp((p->getX() - min_x) * _Nx / (max_x - min_x), 0, _Nx - 1);
		iy = clamp((p->getY() - min_y) * _Ny / (max_y - min_y), 0, _Ny - 1);
		iz = clamp((p->getZ() - min_z) * _Nz / (max_z - min_z), 0, _Nz - 1);
	}
	// traverse the grid
	/// 1) Set Up Grid Traversal
	float dtx = (tmax_x - tmin_x) / _Nx;
	float dty = (tmax_y - tmin_y) / _Ny;
	float dtz = (tmax_z - tmin_z) / _Nz;

	float tnext_x, tnext_y, tnext_z;
	int istep_x, istep_y, istep_z;
	int istop_x, istop_y, istop_z;

	// Tnext_X
	if (ray->getD()->getX() > 0) {
		tnext_x = tmin_x + (ix + 1) * dtx;
		istep_x = +1;
		istop_x = _Nx;
	}
	else if (ray->getD()->getX() < 0) {
		tnext_x = tmin_x + (_Nx - ix) * dtx;
		istep_x = -1;
		istop_x = -1;
	}
	else {
		tnext_x = HUGE_VALUE; /// This way it won't advance in this direction
		istep_x = -1;
		istop_x = -1;
	}
	// Tnext_Y
	if (ray->getD()->getY() > 0) {
		tnext_y = tmin_y + (iy + 1) * dty;
		istep_y = +1;
		istop_y = _Ny;
	}
	else if (ray->getD()->getY() < 0) {
		tnext_y = tmin_y + (_Ny - iy) * dty;
		istep_y = -1;
		istop_y = -1;
	}
	else {
		tnext_y = HUGE_VALUE; /// This way it won't advance in this direction
		istep_y = -1;
		istop_y = -1;
	}
	// Tnext_Z
	if (ray->getD()->getZ() > 0) {
		tnext_z = tmin_z + (iz + 1) * dtz;
		istep_z = +1;
		istop_z = _Nz;
	}
	else if (ray->getD()->getZ() < 0) {
		tnext_z = tmin_z + (_Nz - iz) * dtz;
		istep_z = -1;
		istop_z = -1;
	}
	else {
		tnext_z = HUGE_VALUE; /// This way it won't advance in this direction
		istep_z = -1;
		istop_z = -1;
	}

	// Grid Traversal Loop
	while (true) {
		/// Get Cell at current index
		Cell * cell_ptr = _cells.at(indexOfCell(ix, iy, iz));

		/// Check which cell will the ray traverse next (in x, y or z)
		if (tnext_x < tnext_y && tnext_x < tnext_z) {
			// ray will traverse in X
			/// IF the cell is NOT empty
			if (!cell_ptr->isEmpty()) {
				/// Compute the intersection
				intersection * i = cell_ptr->hit(ray);
				/// check if the ray intersected the object 
				if (i->object != nullptr) {
					return i;
				}
				delete i;
			}
			/// ELSE it will advance to the next cell
			tnext_x += dtx;
			ix += istep_x;
			/// IF it reached the last cell
			if (ix == istop_x)
				/// return NO INTERSECTION
				return nullptr;

		}
		else {
			if (tnext_y < tnext_z) {
				// ray will traverse in Y
				/// IF the cell is NOT empty
				if (!cell_ptr->isEmpty()) {
					/// Compute the intersection
					intersection * i = cell_ptr->hit(ray);
					/// check if the ray intersected the object 
					if (i->object != nullptr) {
						return i;
					}
					delete i;
				}
				/// ELSE it will advance to the next cell
				tnext_y += dty;
				iy += istep_y;
				/// IF it reached the last cell
				if (iy == istop_y)
					/// return NO INTERSECTION
					return nullptr;
			}
			else {
				// ray will traverse in Z
				/// IF the cell is NOT empty
				if (!cell_ptr->isEmpty()) {
					/// Compute the intersection
					intersection * i = cell_ptr->hit(ray);
					/// check if the ray intersected the object 
					if (i->object != nullptr) {
						return i;
					}
					delete i;
				}
				/// ELSE it will advance to the next cell
				tnext_z += dtz;
				iz += istep_z;
				/// IF it reached the last cell
				if (iz == istop_z)
					/// return NO INTERSECTION
					return nullptr;
			}
		}
	}

	/// It wont reach this state
	return nullptr;
}