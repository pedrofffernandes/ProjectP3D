#pragma once

//CONSTANTS
#define MAX_DEPTH 4
#define EPSILON 1e-4
#define IOR 1.0
#define NUMEROAMOSTRAS 1				//Actually sqrt(numero de amostras)
#define USE_SOFTSHADOWS false
#define USE_MONTECARLO true
#define DIFF_MONTECARLO 0.3
#define USE_ARRAY_SOFTSHADOWS true		//Must disable soft shadows
#define DEPTH_MONTECARLO 2				//Use powers of 2
#define SIZE_MONTECARLO (DEPTH_MONTECARLO + 1)
#define USE_DOF false
#define NUMEROAMOSTRAS_DOF 1			//Actually sqrt(numero de amostras dof)
#define APERTURE (float) 0.32
#define FDRATIO 0.85