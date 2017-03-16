#include <stdlib.h>
#include <GL/glut.h>
#include <iostream>
#include <stdio.h>
#include "scene.h"
#include <vector>

#define MAX_DEPTH 6
#define EPSILON 1e-4
#define IOR 1.0

Scene * scene = NULL;
int RES_X, RES_Y;
Vect * rayTracing(Ray * ray, int depth, float ior);
bool inShadow(Ray* ray);


void reshape(int w, int h)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, RES_X - 1, 0, RES_Y - 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// Draw function by primary ray casting from the eye towards the scene's objects

void drawScene()
{	
	for (int y = 0; y < RES_Y; y++)
	{
		for (int x = 0; x < RES_X; x++)
		{	
			Ray * ray = scene->getCamera()->PrimaryRay(x, y);
			Vect * color = rayTracing(ray, 1, IOR); //depth=1, ior=1.0
			glBegin(GL_POINTS);
			//std::cout << 'x' << color->getX() << 'y' << color->getY() << 'z' << color->getZ() << '\n';
			glColor3f(color->getX(), color->getY(), color->getZ());
			glVertex2f(x, y);
			glEnd();
			glFlush();
			
		}
	}
	
	printf("Terminou!\n");
}

Vect * rayTracing(Ray * ray, int depth, float ior) {
	std::list<Obj*> objs = scene->getObjects();
	std::list<Obj*>::iterator itO;

	Obj* closest = nullptr;
	float dist = 9999, distNew;
	for (itO = objs.begin(); itO != objs.end(); itO++) {
		distNew = ((Obj*)*itO)->intersect(ray);				//Intersect

		if (distNew > EPSILON && distNew < dist) {
			dist = distNew;
			closest = (Obj*)*itO;
		}		
	}
	if (closest == nullptr)
		return scene->getBackground();

	std::list<Light*> lights = scene->getLights();
	std::list<Light*>::iterator itL;
	Vect* hit = ray->getHitPoint(dist);
	Vect* color = new Vect();
	Vect* normal = closest->getNormal(hit);

	for (itL = lights.begin(); itL != lights.end(); itL++) {
		Vect * L = ((Light*)*itL)->getLVect(hit);
		
		if(L->dotP(normal) > 0) {
			Ray * newRay = new Ray(hit->add(L->multiply(EPSILON)), L);
			if (!inShadow(newRay)) {
				Vect* difuse = ((Light*)*itL)->getDiffuse(normal, L, closest->getMat()); //+diffuse
				Vect* specular = ((Light*)*itL)->getSpecular(normal, L, closest->getMat(), ray->getD()->multiply(-1));

				color = color->add(difuse);
				color = color->add(specular);
			}

		}

	}

	if (depth >= MAX_DEPTH)
		return color;
	//Reflection
	if (closest->getMat()->getKs() > 0) {
		Vect * I = ray->getD();
		Vect * V = normal->multiply(-2 * I->dotP(normal));
		Vect * R = I->add(V);
		Ray * reflectRay = new Ray(hit->add(R->multiply(EPSILON)), R);
		Vect * reflectColor = rayTracing(reflectRay, depth + 1, ior);
		color = color->add(reflectColor->multiply(closest->getMat()->getKs()));
	}
	//Refraction
	if (closest->getMat()->getT() > 0) {
		
		float cosi = ray->getD()->dotP(normal);
		float aux;
		float iorM = ior;
		float iorO = closest->getMat()->getIOR();
		Vect * n = normal;
		if (cosi < 0) {
			cosi = -cosi;
		} else {
			aux = iorM;
			iorM = iorO;
			iorO = aux;
			n = n->multiply(-1);
		}

		float eta = iorM / iorO;
		float k = 1 - eta*eta * (1 - cosi*cosi);
		if (k < 0) {
			return new Vect();
		} else {
			Vect * I = ray->getD()->multiply(eta);
			n = n->multiply(eta * cosi - sqrtf(k));
			Vect* T = I->add(n);
			Ray * refractRay = new Ray(hit->add(T->multiply(EPSILON)), T);
			Vect * refractColor = rayTracing(refractRay, depth + 1, ior);
			color = color->add(refractColor->multiply(closest->getMat()->getT()));
		}



		/*
		Vect * I = ray->getD();
		Vect * IT = (normal->multiply(2 * I->dotP(normal)))->minus(I);
		Vect * t = IT->normalize();
		float sin = IT->length();
		Vect * sint = t->multiply(sin);
		Vect * cosN = normal->multiply(-1*sqrt(1 - sin*sin));
		Vect * R = sint->add(cosN);
		Ray * refractedRay = new Ray(hit->add(R->multiply(EPSILON)), R);
		Vect * refractedColor = rayTracing(refractedRay, depth + 1, ior);
		//R = sint+ cos-n
		*/
	}
	

	return color;
	
	


	//return nullptr;
}

bool inShadow(Ray* ray) {
	std::list<Obj*> objs = scene->getObjects();
	std::list<Obj*>::iterator itO;
	float hit = 0;
	for (itO = objs.begin(); itO != objs.end(); itO++) {
		hit = ((Obj*)*itO)->intersect(ray);				//Intersect
		if (hit > 0) {
			return true;
		}
	}
	return false;
}


int main(int argc, char**argv)
{
	scene = new Scene();
	if (!(scene->load_nff("test_scenes/balls_low.nff"))) return 0;
	

	
	RES_X = scene->getCamera()->getResX();
	RES_Y = scene->getCamera()->getResY();
	
	printf("resx = %d resy= %d.\n", RES_X, RES_Y);

	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);

	glutInitWindowSize(RES_X, RES_Y);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("JAP Ray Tracing");
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glutReshapeFunc(reshape);
	glutDisplayFunc(drawScene);
	glDisable(GL_DEPTH_TEST);
	glutMainLoop();
	return 0;
}