/**
* Previewer for Benchmark for Animated Ray Tracings: BART.
*
* Based on readnff.c (part of SPD by Eric Haines) by Eduard Schwan.
*
* Simple parser for data files, including argument parsing.
*/

#pragma warning(disable:4996)

#include "Scene.h"



#ifndef  M_PI
#define  M_PI (float) 3.1415926535
#endif

/* Current material. */
Material * mat;

/* Scene */

Scene::Scene() {}

Camera* Scene::getCamera()
{
	return _camera;
}

bool Scene::load_nff(std::string fileName)
{
	// Opening FILE
	const char * c = fileName.c_str();
	FILE * file;
	if (fopen_s(&file, c, "r")) {
		std::cout << "No NFF file." << '\n';
		return false;
	}
		
	// parsing file
	std::cout << "parsing\n";
	parseFile(file);
	// Closing FILE
	if (file) {
		if (fclose(file))
			std::cout << "failed to close file\n";
		else std::cout << "File was closed with success!\n";
	}
	return true;
}

std::list<Light*> Scene::getLights()
{
	return _lights;
}

std::list<Obj*> Scene::getObjects()
{
	return _objects;
}

void Scene::addObject(Obj * object)
{
	_objects.push_back(object);
}

void Scene::addLight(Light * light)
{
	_lights.push_back(light);
}

Vect * Scene::getBackground()
{
	return _background;
}


bool Scene::parseFile(FILE *file) {
	char ch;
	

	while ((ch = getc(file)) != EOF)
	{
		switch (ch)
		{
			/* White space. */
		case ' ':
		case '\t':
		case '\n':
		case '\f':
		case '\r':
			continue;

		case 'v':
			parseViewpoint(file);
			break;
			/* Light sources. */
		case 'l':
			parseLight(file);
			break;
			/* Background colour. */
		case 'b':
			parseBackground(file);			
			break;
			/* Fill material. */
		case 'f':
			parseFill(file);					
			break;
			/* Cylinder or cone. */
		case 'c':
			//parseCone(file);					//TODO
			break;
			/* Sphere. */
		case 's':
			parseSphere(file);				
			break;
			/* Polygon or patch. */
		case 'p':
			{
			char cha = getc(file);
			if (cha == 'p') {
				parsePolygon(file);		//TODO
			} else if (cha == 'l') {
				parsePlane(file);
			} else {
				printf("unknown NFF primitive code: p%c\n", cha);
				exit(1);
			}
			break;
			}
		default:
			printf("unknown NFF primitive code: %c\n", ch);
			exit(1);
		}
	}
	return true;
}

void Scene::parseViewpoint(FILE *file)
{
	/* Initialize variables here to avoid crossing them with gotos. */
	float eyex, eyey, eyez;
	float atx, aty, atz;
	float upx, upy, upz;
	float fovAngle;
	float hither;


	if (fscanf(file, " from %g %g %g", &eyex, &eyey, &eyez) != 3)
	{
		printf("from");
		goto fmterr;
	}
	;
	if (fscanf(file, " at %g %g %g", &atx, &aty, &atz) != 3)
	{
		printf("at");
		goto fmterr;
	}

	if (fscanf(file, " up %g %g %g", &upx, &upy, &upz) != 3)
	{
		printf("up");
		goto fmterr;
	}

	if (fscanf(file, " angle %g", &fovAngle) != 1)
	{
		goto fmterr;
	}

	if (fscanf(file, " hither %g", &hither) != 1)
	{
		goto fmterr;
	}

	if (hither < 0.0001)
	{
		hither = 1.0f;
	}

	int resX;
	int resY;
	if (fscanf(file, " resolution %d %d", &resX, &resY) != 2)
	{
		goto fmterr;
	}

	/* Convert the fovAngle, which is in degrees, to radians. */
	fovAngle = fovAngle / 180.0f * M_PI;

	
	Vect * eye = new Vect(eyex, eyey, eyez);
	Vect * at = new Vect(atx, aty, atz);
	Vect * up = new Vect(upx, upy, upz);

	this->_camera = new Camera(eye, at, up, resX, resY, fovAngle, hither);

	return;

fmterr: printf("Parser view syntax error");
	exit(1);
}

void Scene::parseLight(FILE *file)
{
	float lx, ly, lz;
	float lr, lg, lb;

	if (fscanf(file, " %g %g %g ", &lx, &ly, &lz) != 3)
	{
		printf("Light source position syntax error");
		exit(1);
	}

	/* Read optional color of light. */
	int num = fscanf(file, " %g %g %g ", &lr, &lg, &lb);
	if (num == 0)
	{
		Vect * position = new Vect(lx, ly, lz);
		Vect * color = new Vect(1.0, 1.0, 1.0);
		Light * light = new Light(position, color);
		this->_lights.push_back(light);
		return;

	}
	else if (num != 3)
	{
		printf("Light source color syntax error");
		exit(1);
	}

	Vect * position = new Vect(lx, ly, lz);
	Vect * color = new Vect(lr, lg, lb);
	Light * light = new Light(position, color);
	this->_lights.push_back(light);
}

void Scene::parseBackground(FILE * file)
{
	float r, g, b;
	if (fscanf(file, "%g %g %g", &r, &g, &b) != 3)
	{
		printf("background color syntax error");
		exit(1);
	}

	Vect * color = new Vect(r, g, b);

	this->_background = color;
}

void Scene::parseFill(FILE * file) {
	float r, g, b;
	if (fscanf(file, "%g %g %g", &r, &g, &b) != 3)
	{
		printf("fill color syntax error");
		exit(1);
	}
	Vect * rgb = new Vect(r, g, b);

	float kD, kS, kT, shine, ior;
	if (fscanf(file, "%g %g %g %g %g", &kD, &kS, &shine, &kT, &ior) != 5)
	{
		printf("fill material syntax error");
		exit(1);
	}
	mat = new Material(rgb, kD, kS, shine, kT, ior);
}

void Scene::parseSphere(FILE *file)
{
	float radius;
	float x, y, z;

	if (fscanf(file, "%g %g %g %g", &x, &y, &z, &radius)
		!= 4)
	{
		printf("sphere syntax error");
		exit(1);
	}
	Vect * position = new Vect(x, y, z);
	Sphere * sphere = new Sphere(position, radius, mat);

	this->addObject(sphere);
}

void Scene::parsePolygon(FILE * file)
{
	int d;
	if (fscanf(file, " %d ", &d) != 1)
	{
		printf("Polygon syntax error");
		exit(1);
	}
	if (d == 3) {
		parseTriangle(file);
	} else {
		parseBigPoly(file, d);
	}
}

void Scene::parseTriangle(FILE * file)
{
	float x, y, z;
	if (fscanf(file, " %g %g %g ", &x, &y, &z) != 3)
	{
		printf("Triangle syntax error");
		exit(1);
	}
	Vect* point1 = new Vect(x, y, z);

	if (fscanf(file, " %g %g %g ", &x, &y, &z) != 3)
	{
		printf("Triangle syntax error");
		exit(1);
	}
	Vect* point2 = new Vect(x, y, z);

	if (fscanf(file, " %g %g %g ", &x, &y, &z) != 3)
	{
		printf("Triangle syntax error");
		exit(1);
	}
	Vect* point3 = new Vect(x, y, z);
	Triangle* triangle = new Triangle(point1, point2, point3, mat);
	this->addObject(triangle);
}

void Scene::parseBigPoly(FILE * file, int d)
{
	float x, y, z;
	BigPoly* bigpoly = new BigPoly(mat);
	Vect* point;
	for (d; d > 0; d--) {
		if (fscanf(file, " %g %g %g ", &x, &y, &z) != 3)
		{
			printf("Triangle syntax error");
			exit(1);
		}
		point = new Vect(x, y, z);
		bigpoly->addPoint(point);
	}
}

/*
void Scene::parseCone(FILE * file) {
	vec3 base;
	vec3 apex;
	float baseRadius, apexRadius;

	if (fscanf(fp, " %f %f %f %f %f %f %f %f",
		&base.x, &base.y, &base.z, &baseRadius,
		&apex.x, &apex.y, &apex.z, &apexRadius) != 8)
	{
		printf("cylinder or cone syntax error\n");
		exit(1);
	}

	if ((baseRadius < 0) || (apexRadius < 0))
	{
		printf("negative radii are not supported\n");
		exit(1);
	}

	if (apexRadius > baseRadius)
	{
		printf("apex radii cannot be bigger than base radii (see NFF spec)\n");
		exit(1);
	}

	SceneObject* o;

	if (baseRadius == apexRadius)
	{
		o = new Cylinder(base, apex, baseRadius);
	}
	else
	{
		o = new Cone(base, baseRadius, apex, apexRadius);
	}
	o->mat = mat;

	scene.addObject(o);

}
*/

void Scene::parsePlane(FILE * file) {
	float x, y, z;

	if (fscanf(file, " %g %g %g ", &x, &y, &z) != 3)
	{
		printf("sphere syntax error");
		exit(1);
	}
	Vect * p1 = new Vect(x, y, z);

	if (fscanf(file, " %g %g %g ", &x, &y, &z) != 3)
	{
		printf("sphere syntax error");
		exit(1);
	}
	Vect * p2 = new Vect(x, y, z);

	if (fscanf(file, " %g %g %g ", &x, &y, &z) != 3)
	{
		printf("sphere syntax error");
		exit(1);
	}
	Vect * p3 = new Vect(x, y, z);
	Plane * plane = new Plane(p1, p2, p3, mat);
	this->addObject(plane);
}