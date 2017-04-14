#pragma once
#include "obj.h"
#include "Constants.h"


class Triangle :
	public Obj
{
	Vect* _point1;
	Vect* _point2;
	Vect* _point3;
	Vect* _n;
	float _d;
	//Index of normal's biggest component
	int _i;

public:
	Triangle(Vect* point1, Vect* point2, Vect* point3, Material* mat);
	~Triangle();
	float intersect(Ray * ray);
	Vect* getNormal(Vect* hit);
	void bboxSetup(void);
};

