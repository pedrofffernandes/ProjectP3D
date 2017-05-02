#pragma once

//CONSTANTS
#define MAX_DEPTH 4
#define EPSILON 1e-4
#define IOR 1.0
#define NUMEROAMOSTRAS 1				//Actually sqrt(numero de amostras) //Set to 1 if disabled
#define USE_SOFTSHADOWS false			//Must disable array soft shadows
#define USE_ARRAY_SOFTSHADOWS true		//Must disable soft shadows and montecarlo
#define LIGHT_SIZE 1
#define USE_MONTECARLO false			//Must disable array soft shadows
#define DIFF_MONTECARLO 0.3
#define DEPTH_MONTECARLO 2				//Use powers of 2
#define SIZE_MONTECARLO (DEPTH_MONTECARLO + 1)
#define USE_DOF false
#define NUMEROAMOSTRAS_DOF 1			//Actually sqrt(numero de amostras dof) //Set to 1 if disabled
#define APERTURE (float) 0.32
#define FDRATIO 0.85