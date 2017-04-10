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
	vy->multiply(_h * ((y / _resY) - 0.5f));
	Vect * vx = new Vect(_xe);
	vx->multiply(_w * ((x / _resX) - 0.5f));
	
	Vect * d = (vz->add(vy))->add(vx);
	
	Ray * result = new Ray(_vrp, d);

	delete vz;
	delete vy;
	delete vx;
	return result;
}

Ray * Camera::PrimaryRayDOF(Vect * focalp, float aperture) {
	Vect * origin = originDOF(aperture);
	Vect * direction = new Vect(focalp);
	direction->minus(origin);

	return new Ray(origin, direction);
}

Vect * Camera::originDOF(float aperture) {
	float r = sqrtf(RAND);					//sqrt spreads the points more evenly on the circle
	float teta = 2 * PIRAND;
	float x = aperture * r * cosf(teta);	
	float y = aperture * r * sinf(teta);
	Vect * u = new Vect(_xe);
	Vect * v = new Vect(_ye);
	u->multiply(x);
	v->multiply(y);
	u->add(v);
	delete v;
	return u;
}

Vect * Camera::GetFocalPoint(float x, float y)
{
	float px = x;
	float py = y;
	Vect* u = new Vect(_xe);
	Vect* v = new Vect(_ye);
	Vect* w = new Vect(_ze);

	u->multiply(px);
	v->multiply(py);
	w->multiply(-_d);
	u->add(v->add(w));
	delete v;
	delete w;

	return u;
}

Vect * Camera::getZe()
{
	return _ze;
}

Ray * Camera::getPrimaryRayDOF(Vect *p)
{
	Vect* u = new Vect(_xe);
	Vect* v = new Vect(_ye);
	u->multiply(ERAND*0.00001);
	v->multiply(ERAND*0.00001);
	u->add(v);
	Vect * direction = new Vect(u);
	direction->minus(p);
	delete v;
	return new Ray(u,direction);
}
