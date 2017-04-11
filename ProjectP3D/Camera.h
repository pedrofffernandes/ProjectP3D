#pragma once

#ifndef _CAMERA_H
#define _CAMERA_H

#include "Vect.h"
#include "Ray.h"
#include "Constants.h"

#define M_PI (float) 3.1415926535
#define RAND (float)rand()/RAND_MAX
#define PIRAND (float)rand()/RAND_MAX*M_PI


class Camera
{
	// Camera Parameters
	// Camera Position (eyepoint)
	// VRP (View Reference Point)
	Vect* _vrp;
	// Camera Direction
	// VPN (View Plane Normal)
	Vect* _vpn;
	// Camera Rotation
	// VUV (View Up Vector)
	Vect* _vuv;

	int _resX;
	int _resY;
	// Camera Angle
	// (aka.: How wide you can see?)
	// FOV (Field of View)
	float _fov;
	// Camera Near
	float _hither;
	//Camera far
	float _far;
	//Distance Camera to Plane
	float _d;

	float _w, _h;
	// Camera reference vector x
	Vect * _xe;
	// Camera reference vector y
	Vect * _ye;
	// Camera reference vector z
	Vect * _ze;



	// Window settings
	// not to be used in here
	/*
		float _fovy;
		float _near, _far;
	*/

public:
	// Default Camera
	Camera();
	// Camera without the FOV (angle)
	Camera(Vect* vrp, Vect* vpn, Vect* vuv);
	// Camera with the FOV (angle)
	Camera(Vect* vrp, Vect* vpn, Vect* vuv, int resX, int resY, float fov, float hither);
	// destructor
	~Camera();

	int Camera::getResX();
	int Camera::getResY();
	//Computes the primaryRay
	Ray * Camera::PrimaryRay(float x, float y);
	Ray * Camera::PrimaryRayDOF(Vect * focalp);
	Vect * Camera::originDOF();
	Vect* Camera::GetFocalPoint(float x, float y);
	Vect * getZe();

};

#endif // !_CAMERA_H