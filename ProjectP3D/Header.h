#pragma once
#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <stdio.h>
#include <stdlib.h>

// OpenGL
#include <GL/glut.h>

// Project Classes
#include "scene.h"

// CONSTANTS
#include "Constants.h"

#define ERAND (float)rand()/RAND_MAX
// Control CONSTANTS
#define USE_DOF false
#define USE_SOFTSHADOWS false
#define USE_OPEN_GL true /// SET THIS TO FALSE IF YOU WANT TO USE THE SAVE_BMP function

// VARIABLES
Scene * scene = NULL;
int RES_X, RES_Y;
Vect * rayTracing(Ray * ray, int depth, float ior);
void multiSample(Vect * color, int x, int y);
void multiSampleDOF(Vect * color, int x, int y);
void monteCarlo();
Vect* monteCarlo2(float x, float y, std::vector<Vect*> &monte, int a, int b, int depth);
bool checkThreshold(Vect*, Vect*, Vect*, Vect*);
bool inShadow(Ray* ray);
void drawScene_withoutOPENGL(Scene);
// Declarations
/// STRUCTURE for color
struct RGBType;
/// function to save an image to BMP
void savebmp(const char * filename, int w, int h, int dpi, RGBType *data);

// TIME
/// This server to count the time the program takes to render the image
#define TIMING
#ifdef TIMING
#define INIT_TIMER auto start = std::chrono::high_resolution_clock::now();
#define START_TIMER  start = std::chrono::high_resolution_clock::now();
#define STOP_TIMER(name)  std::cout << "\n RUNTIME of " << name << ": " << \
    std::chrono::duration_cast<std::chrono::milliseconds>( \
            std::chrono::high_resolution_clock::now()-start \
    ).count() << " ms " << std::endl; 
#else
#define INIT_TIMER
#define START_TIMER
#define STOP_TIMER(name)
#endif

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

// Definitions
struct RGBType {
	double r; /// RED
	double g; /// GREEN
	double b; /// BLUE
};
/**
*** @function saveBMP
*** function to save an image to a BMP file
**/
void savebmp(const char * filename, int w, int h, int dpi, RGBType *data) {
	FILE * f;
	int k = w*h;
	int s = 4 * k;
	int filesize = 54 + s;

	double factor = 39.375;
	int m = static_cast<int>(factor);

	int ppm = dpi*m;

	unsigned char bmpfileheader[14] = { 'B', 'M', 0,0,0,0, 0,0,0,0, 54,0,0,0 };
	unsigned char bmpinfoheader[40] = { 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0 };

	bmpfileheader[2] = (unsigned char)(filesize);
	bmpfileheader[3] = (unsigned char)(filesize >> 8);
	bmpfileheader[4] = (unsigned char)(filesize >> 16);
	bmpfileheader[5] = (unsigned char)(filesize >> 24);

	bmpinfoheader[4] = (unsigned char)(w);
	bmpinfoheader[5] = (unsigned char)(w >> 8);
	bmpinfoheader[6] = (unsigned char)(w >> 16);
	bmpinfoheader[7] = (unsigned char)(w >> 24);

	bmpinfoheader[8] = (unsigned char)(h);
	bmpinfoheader[9] = (unsigned char)(h >> 8);
	bmpinfoheader[10] = (unsigned char)(h >> 16);
	bmpinfoheader[11] = (unsigned char)(h >> 24);

	bmpinfoheader[21] = (unsigned char)(s);
	bmpinfoheader[22] = (unsigned char)(s >> 8);
	bmpinfoheader[23] = (unsigned char)(s >> 16);
	bmpinfoheader[24] = (unsigned char)(s >> 24);

	bmpinfoheader[25] = (unsigned char)(ppm);
	bmpinfoheader[26] = (unsigned char)(ppm >> 8);
	bmpinfoheader[27] = (unsigned char)(ppm >> 16);
	bmpinfoheader[28] = (unsigned char)(ppm >> 24);

	bmpinfoheader[29] = (unsigned char)(ppm);
	bmpinfoheader[30] = (unsigned char)(ppm >> 8);
	bmpinfoheader[31] = (unsigned char)(ppm >> 16);
	bmpinfoheader[32] = (unsigned char)(ppm >> 24);

	f = fopen(filename, "wb");

	fwrite(bmpfileheader, 1, 14, f);
	fwrite(bmpinfoheader, 1, 40, f);

	for (int i = 0; i < k; i++) {
		RGBType rgb = data[i];
		double red = (data[i].r) * 255;
		double green = (data[i].g) * 255;
		double blue = (data[i].b) * 255;

		unsigned char color[3] = { (int)floor(blue),(int)floor(green),(int)floor(red) };

		fwrite(color, 1, 3, f);
	}

	fclose(f);
}



void drawScene_withoutOPENGL(Scene * scene) {
	// Start Timer
	INIT_TIMER;
	START_TIMER;

	std::cout << "Initialization...";
	
	int n = RES_Y*RES_X;
	RGBType *pixels = new RGBType[n];
	int pixel_index = 0;

	std::cout << "DONE!" << std::endl;
	std::cout << "Rendering the image...";
	for (int y = 0; y < RES_Y; y++) {
		for (int x = 0; x < RES_X; x++) {

			Vect * color = new Vect();
			Ray * ray;

			ray = scene->getCamera()->PrimaryRay(x + 0.5f, y + 0.5f);
			color->add(rayTracing(ray, 1, IOR)); 

			pixel_index = y*RES_X + x;
			// RED
			if (color->getX() > 1.0) pixels[pixel_index].r = 1.0;
			else if (color->getX() < 0.0) pixels[pixel_index].r = 0.0;
			else pixels[pixel_index].r = color->getX();
			// GREEN
			if (color->getY() > 1.0) pixels[pixel_index].g = 1.0;
			else if (color->getY() < 0.0) pixels[pixel_index].g = 0.0;
			else pixels[pixel_index].g = color->getY();
			// BLUE
			if (color->getZ() > 1.0) pixels[pixel_index].b = 1.0;
			else if (color->getZ() < 0.0) pixels[pixel_index].b = 0.0;
			else pixels[pixel_index].b = color->getZ();
			/// Delete Pointers
			delete ray;
			delete color;

		}
	}
	std::cout << "DONE!" << std::endl;
	std::cout << "Saving image to BMP file..." << std::endl;
	savebmp("scene.bmp", RES_X, RES_Y, 72, pixels);
	STOP_TIMER("draw")
	printf("Terminou!\n");
}