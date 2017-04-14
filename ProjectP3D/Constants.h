#pragma once
#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

//CONSTANTS
#define MAX_DEPTH 4
#define EPSILON 1e-4f
#define HUGE_VALUE 1e6f
#define IOR 1.0
#define NUMEROAMOSTRAS 2				//Actually sqrt(NUMEROAMOSTRAS)
#define APERTURE (float) 0.32
#define FDRATIO 0.85
#define M 2			/// M is a multiplying factor to vary the number of cells in the GRID
// Control CONSTANTS
#define USE_DOF true
#define USE_SOFTSHADOWS false
#define USE_OPEN_GL true /// SET THIS TO FALSE IF YOU WANT TO USE THE SAVE_BMP function

#endif // !__CONSTANTS_H__