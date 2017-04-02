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
#define MAX_DEPTH 4
#define EPSILON 1e-4
#define IOR 1.0
#define NUMEROAMOSTRAS 4
#define ERAND (float)rand()/RAND_MAX
// VARIABLES
Scene * scene = NULL;
int RES_X, RES_Y;
Vect * rayTracing(Ray * ray, int depth, float ior);
bool inShadow(Ray* ray);


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