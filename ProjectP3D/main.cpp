#pragma once
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <GL/glut.h>

#include "scene.h"


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

	Obj* closest = nullptr;									//the closest object to the camera that the ray hits

	float dist = 9999, distNew = 0;
	for (itO = objs.begin(); itO != objs.end(); itO++) {	//Iterates over all objects
		distNew = ((Obj*)*itO)->intersect(ray);				//Intersect returns distance from hitpoint to camera
		if (distNew > EPSILON && distNew < dist) {			//If distance is smaller than all previous distances
			dist = distNew;									//Then save distance and object
			closest = (Obj*)*itO;
		}
	}
	if (closest == nullptr)									//If the ray doesn't intersect any object
		return scene->getBackground();						

	std::list<Light*> lights = scene->getLights();
	std::list<Light*>::iterator itL;
	Vect* hit = ray->getHitPoint(dist);
	Vect* color = new Vect();
	Vect* normal = closest->getNormal(hit);

	//Local ilumination
	for (itL = lights.begin(); itL != lights.end(); itL++) {			//Iterates over all the lights
		Vect * L = ((Light*)*itL)->getLVect(hit);
		
		if(L->dotP(normal) > 0) {										//If surface faces light
			Ray * newRay = new Ray(hit->add(L->multiply(EPSILON)), L);	//Create shadowfeeler
			if (!inShadow(newRay)) {
				Vect* difuse = ((Light*)*itL)->getDiffuse(normal, L, closest->getMat());								//Compute Diffuse		
				Vect* specular = ((Light*)*itL)->getSpecular(normal, L, closest->getMat(), ray->getD()->multiply(-1));	//Compute Specular

				color = color->add(difuse);					
				color = color->add(specular);
			}

		}

	}
	
	if (depth >= MAX_DEPTH)												
		return color;
	
	//Reflection
	if (closest->getMat()->getKs() > 0) {					//If material is reflective
		Vect * I = ray->getD();								//Compute reflection direction
		Vect * V = normal->multiply(-2 * I->dotP(normal));	//
		Vect * R = I->add(V);								//

		Ray * reflectRay = new Ray(hit->add(R->multiply(EPSILON)), R);			//Create reflection ray
		Vect * reflectColor = rayTracing(reflectRay, depth + 1, ior);			//Compute reflection color
		color = color->add(reflectColor->multiply(closest->getMat()->getKs())); //Add color to pixel color
	}

	//Refraction
	if (closest->getMat()->getT() > 0) {
		
		float cosi = ray->getD()->dotP(normal);				//Get cos of Viewer and normal
		float aux;
		float iorM = ior;
		float iorO = closest->getMat()->getIOR();
		Vect * n = normal;
		if (cosi < 0) {										//If outside
			cosi = -cosi;									//Turn cos positive
		} else {											//If inside
			aux = iorM;										//Swap ior's
			iorM = iorO;									//
			iorO = aux;										//
			n = n->multiply(-1);							//Get inside normal
		}

		float eta = iorM / iorO;
		float k = 1 - eta*eta * (1 - cosi*cosi);
		if (k < 0) {
			return new Vect();
		} else {
			Vect * I = ray->getD()->multiply(eta);			//Compute refraction ray equation
			n = n->multiply(eta * cosi - sqrtf(k));			//Using Ray Tracing: Texto Apoio
			Vect* T = I->add(n);							//
			Ray * refractRay = new Ray(hit->add(T->multiply(EPSILON)), T);			//Create refraction ray
			Vect * refractColor = rayTracing(refractRay, depth + 1, ior);			//Compute refraction color
			color = color->add(refractColor->multiply(closest->getMat()->getT()));	//Add refraction color to pixel
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
}

//Checks if a ray intersects any object
//Used to check if objects are in shadow
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
	if (!(scene->load_nff("test_scenes/mount_low.nff"))) return 0;
	

	
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