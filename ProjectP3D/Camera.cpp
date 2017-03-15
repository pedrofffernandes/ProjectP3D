#include "Camera.h"
#include <iostream>


Camera::Camera()
{
	_vrp = new Vect(1, 1, 1);
	_vpn = new Vect(1, 0, 0);
	_vuv = new Vect(1, 1, 0);
}
Camera::Camera(Vect* vrp, Vect* vpn, Vect* vuv)
{
	_vrp = vrp;
	_vpn = vpn;
	_vuv = vuv;
}
Camera::Camera(Vect* vrp, Vect* vpn, Vect* vuv, int resX, int resY, float fov, float hither)
{
	_vrp = vrp;
	_vpn = vpn;
	_vuv = vuv;
	_fov = fov;
	_resX = resX;
	_resY = resY;
	_hither = hither;
	
	_d = (vrp->minus(vpn))->length();
	_h = 2 * _d * tan(fov / 2);
	_w = (resX / resY) * _h;


	_ze = (_vrp->minus(_vpn))->normalize();
	
	_xe = (_vuv->crossP(_ze))->normalize();
	_ye = _ze->crossP(_xe);
	
}

Camera::~Camera()
{
}

int Camera::getResX() {
	return _resX;
}

int Camera::getResY() {
	return _resY;
}

Ray * Camera::PrimaryRay(int x, int y) {
	Vect * vz = _ze->multiply(-_d);
	Vect * vy = _ye->multiply(_h * (((y + 0.5f) / _resY) - 0.5f));
	Vect * vx = _xe->multiply(_w * (((x + 0.5f) / _resX) - 0.5f));
	
	Vect * d = (vz->add(vy))->add(vx);
	
	return new Ray(_vrp, d);
}

Vect * Camera::getZe()
{
	return _ze;
}
