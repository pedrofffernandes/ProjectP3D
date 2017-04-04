#include "Camera.h"



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
	
	Vect * vrpa = new Vect(vrp);
	_d = (vrpa->minus(vpn))->length();
	_h = 2 * _d * tan(fov / 2);
	_w = (resX / resY) * _h;

	Vect * vrpb = new Vect(vrp);
	_ze = (vrpb->minus(vpn))->normalize();
	_xe = (vuv->crossP(_ze))->normalize();
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

Ray * Camera::PrimaryRay(float x, float y) {
	Vect * vz = new Vect(_ze);
	vz->multiply(-_d);
	Vect * vy = new Vect(_ye);
	vy->multiply(_h * (((y + 0.5f) / _resY) - 0.5f));
	Vect * vx = new Vect(_xe);
	vx->multiply(_w * (((x + 0.5f) / _resX) - 0.5f));
	
	Vect * d = (vz->add(vy))->add(vx);
	
	Ray * result = new Ray(_vrp, d);

	delete vz;
	delete vy;
	delete vx;
	return result;
}

Vect * Camera::GetFocalPoint(float x, float y)
{
	float px = x * 2;
	float py = y * 2;
	Vect* u = new Vect(_xe);
	Vect* v = new Vect(_ye);
	Vect* w = new Vect(_ze);

	u->multiply(px);
	v->multiply(py);
	w->multiply(-_d * 2);
	u->add(v->add(w));
	delete v;
	delete w;

	return u;
}

Vect * Camera::getZe()
{
	return _ze;
}
