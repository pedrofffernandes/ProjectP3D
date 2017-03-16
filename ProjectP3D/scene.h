#pragma once
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <list>

#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "Ray.h"
#include "Plane.h"
#include "Sphere.h"
#include "Vect.h"
#include "Triangle.h"
#include "BigPoly.h"

class Scene {
	std::list<Light*> _lights;
	std::list<Obj*> _objects;

	Camera * _camera;
	Vect * _background;

public:
	Scene();
	Camera * getCamera();
	bool load_nff(std::string fileName);
	std::list<Light*> getLights();
	std::list<Obj*> getObjects();
	void addObject(Obj * object);
	void addLight(Light * light);
	Vect * getBackground();


	bool Scene::parseFile(FILE *file);
	void Scene::parseViewpoint(FILE *file);
	void Scene::parseLight(FILE *file);
	void Scene::parseBackground(FILE * file);
	void Scene::parseFill(FILE * file);
	//void Scene::parseCone(FILE * file);
	void Scene::parseSphere(FILE * file);
	void Scene::parsePolygon(FILE * file);
	void Scene::parseTriangle(FILE * file);
	void Scene::parseBigPoly(FILE * file, int d);
	void Scene::parsePlane(FILE * file);



};