#include "Header.h"
#include <vector>
#include <random>

#define MAX_DEPTH 4
#define EPSILON 1e-4
#define IOR 1.0
#define NUMEROAMOSTRAS 4
#define ERAND (float)rand()/RAND_MAX

#include <chrono>

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
	INIT_TIMER
	START_TIMER
	for (int y = 0; y < RES_Y; y++)
	{
		for (int x = 0; x < RES_X; x++)
		{	
			Vect * color = new Vect();
			Ray * ray;
			for (int n = 0; n < NUMEROAMOSTRAS; n++) {
				for (int m = 0; m < NUMEROAMOSTRAS; m++) {
					ray = scene->getCamera()->PrimaryRay(x + ((n + ERAND) / NUMEROAMOSTRAS), y + ((m + ERAND) / NUMEROAMOSTRAS));
					color->add(rayTracing(ray, 1, IOR)); //depth=1, ior=1.0
				}
			}

			color->multiply((float)1 / (NUMEROAMOSTRAS*NUMEROAMOSTRAS));
			glBegin(GL_POINTS);
			glColor3f(color->getX(), color->getY(), color->getZ());
			glVertex2f(x, y);
			glEnd();
			glFlush();
			delete ray;
			delete color;
		}
	} 
	STOP_TIMER("draw")
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
		return new Vect(scene->getBackground());						

	std::list<Light*> lights = scene->getLights();
	std::list<Light*>::iterator itL;
	Vect* hit = ray->getHitPoint(dist);
	Vect* color = new Vect();
	Vect* normal = closest->getNormal(hit);

	//Local ilumination
	for (itL = lights.begin(); itL != lights.end(); itL++) {			//Iterates over all the lights
		Vect * lightD = ((Light*)*itL)->getLVect(hit);
		
		if(lightD->dotP(normal) > 0) {										//If surface faces light
			Vect * newO = new Vect(hit);
			Vect * newD = new Vect(lightD);
			Ray * shadowFeeler = new Ray(newO->add(newD->multiply(EPSILON)), lightD);	//Create shadowfeeler
			if (!inShadow(shadowFeeler)) {
				Vect * difuse = ((Light*)*itL)->getDiffuse(normal, lightD, closest->getMat());								//Compute Diffuse
				Vect * rayD = new Vect(ray->getD());
				Vect * specular = ((Light*)*itL)->getSpecular(normal, lightD, closest->getMat(), rayD->multiply(-1));	//Compute Specular

				color->add(difuse);					
				color->add(specular);
				delete specular;
				delete rayD;
				delete difuse;
			}
			delete newO;
			delete newD;
			delete shadowFeeler;
		}
		delete lightD;
	}

	if (depth >= MAX_DEPTH) {
		delete hit;
		delete normal;
		return color;
	}

	//Reflection
	if (closest->getMat()->getKs() > 0) {					//If material is reflective
		Vect * I = new Vect(ray->getD());					//Compute reflection direction
		Vect * V = new Vect(normal);						//
		V->multiply(-2 * I->dotP(normal));					//
		I->add(V);											//

		Vect * newO = new Vect(hit);
		Vect * newD = new Vect(I);
		Ray * reflectRay = new Ray(newO->add(newD->multiply(EPSILON)), I);	//Create reflection ray
		Vect * reflectColor = rayTracing(reflectRay, depth + 1, ior);		//Compute reflection color
		reflectColor->multiply(closest->getMat()->getKs());					//Multiply by intensity
		color->add(reflectColor);											//Add color to pixel color
		delete V;
		delete I;
		delete newO;
		delete newD;
		delete reflectRay;
		delete reflectColor;
	}
	

	//Refraction
	if (closest->getMat()->getT() > 0) {
		
		float cosi = ray->getD()->dotP(normal);				//Get cos of Viewer and normal
		float aux;
		float iorM = ior;									//IOR medium
		float iorO = closest->getMat()->getIOR();			//IOR object
		Vect * n = new Vect(normal);
		if (cosi < 0) {										//If outside
			cosi = -cosi;									//Turn cos positive
		} else {											//If inside
			aux = iorM;										//Swap ior's
			iorM = iorO;									//
			iorO = aux;										//
			n->multiply(-1);								//Get inside normal
		}

		float eta = iorM / iorO;
		float k = 1 - eta*eta * (1 - cosi*cosi);
		if (k >= 0) {										//k<0 Total internal refraction
			Vect * I = new Vect(ray->getD());
			I->multiply(eta);								//Compute refraction ray equation
			n->multiply(eta * cosi - sqrtf(k));				//Using Ray Tracing: Texto Apoio
			I->add(n);										//
			Vect * newO = new Vect(hit);
			Vect * newD = new Vect(I);
			Ray * refractRay = new Ray(newO->add(newD->multiply(EPSILON)), I);	//Create refraction ray
			Vect * refractColor = rayTracing(refractRay, depth + 1, ior);		//Compute refraction color
			refractColor->multiply(closest->getMat()->getT());					//Multiply by intensity
			color->add(refractColor);											//Add refraction color to pixel
			delete I;
			delete newO;
			delete newD;
			delete refractRay;
			delete refractColor;
		}
		delete n;
	}
	delete normal;
	delete hit;
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
	if (!(scene->load_nff("test_scenes/balls_low.nff"))) return 0;
	
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