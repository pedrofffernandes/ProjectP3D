#include "Cell.h"

Cell::Cell()
{
}

Cell::~Cell()
{
}

// Returns the closest intersection with the Ray
// Returns nullptr otherwise
intersection* Cell::hit(Ray * ray)
{
	/// Initialize the intersection Struct
	intersection * i = new intersection();
	i->distance = HUGE_VALUE;
	i->object = nullptr;

	/// Iterates over all objects
	for (std::list<Obj*>::iterator itO = _objects.begin(); itO != _objects.end(); itO++) { 
		/// Intersect returns distance from hitpoint to camera
		float current_distance = ((Obj*)*itO)->intersect(ray);
		/// If distance is smaller than all previous distances
		if (current_distance > EPSILON && current_distance < i->distance) {
			/// Then save distance and object
			i->distance = current_distance;
			i->object = (Obj*)*itO;
		}
	}

	return i;
}


intersection* Cell::hitInShadow(Ray * ray)
{
	/// Initialize the intersection Struct
	intersection * i = new intersection();
	i->distance = HUGE_VALUE;
	i->object = nullptr;

	/// Iterates over all objects
	for (std::list<Obj*>::iterator itO = _objects.begin(); itO != _objects.end(); itO++) {
		/// Intersect returns distance from hitpoint to camera
		float current_distance = ((Obj*)*itO)->intersect(ray);
		/// If distance is smaller than all previous distances
		if (current_distance > EPSILON && current_distance < i->distance) {
			/// Then save distance and object
			i->distance = current_distance;
			i->object = (Obj*)*itO;
			return i;
		}
	}

	return i;
}