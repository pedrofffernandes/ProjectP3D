#include "Header.h"
#include <stdlib.h>

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
	if (USE_MONTECARLO) {
		monteCarlo();
	}
	else {
		for (int y = 0; y < RES_Y; y++)
		{
			glBegin(GL_POINTS);
			for (int x = 0; x < RES_X; x++)
			{
				Vect * color = new Vect();
				(USE_DOF) ? multiSampleDOF(color, x, y) : multiSample(color, x, y);

				glColor3f(color->getX(), color->getY(), color->getZ());
				glVertex2f(x, y);

				delete color;
			}
			glEnd();
			glFlush();
		}
	}
	STOP_TIMER("draw")
	printf("Terminou!\n");
}

void multiSample(Vect * color, int x, int y) {

	for (int n = 0; n < NUMEROAMOSTRAS; n++) {
		for (int m = 0; m < NUMEROAMOSTRAS; m++) {
			Ray * ray = scene->getCamera()->PrimaryRay(x + ((n + ERAND) / NUMEROAMOSTRAS), y + ((m + ERAND) / NUMEROAMOSTRAS));
			Vect * sample = rayTracing(ray, 1, IOR);
			color->add(sample); //depth=1, ior=1.0
			delete ray;
			delete sample;
		}
	}
	color->multiply((float)1 / (NUMEROAMOSTRAS*NUMEROAMOSTRAS));
}

void multiSampleDOF(Vect * color, int x, int y) {

	for (int n = 0; n < NUMEROAMOSTRAS; n++) {
		for (int m = 0; m < NUMEROAMOSTRAS; m++) {
			Vect * focalp = scene->getCamera()->GetFocalPoint(x + ((n + ERAND) / NUMEROAMOSTRAS), y + ((m + ERAND) / NUMEROAMOSTRAS));
			for (int o = 0; o < NUMEROAMOSTRAS; o++) {
				for (int q = 0; q < NUMEROAMOSTRAS; q++) {
					Ray * ray = scene->getCamera()->PrimaryRayDOF(focalp);
					Vect * sample = rayTracing(ray, 1, IOR);
					color->add(sample); //depth=1, ior=1.0
					delete ray;
					delete sample;
				}
			}
			color->multiply((float)1 / (NUMEROAMOSTRAS));
			delete focalp;
		}
	}
	color->multiply((float)1 / (NUMEROAMOSTRAS*NUMEROAMOSTRAS));
}

void monteCarlo() {
	/*
	std::list<Vect*> newColors;
	std::list<Vect*> oldColors;
	std::list<Vect*>::iterator newColor;
	std::list<Vect*>::iterator oldColor;
	*/

	/*vect * newColors = (Vect*) malloc(sizeof(Vect*) * RES_X);
	vect * oldColors = (Vect*) malloc(sizeof(Vect*) * RES_X);*/

	std::vector<Vect*> newColors(RES_X + 1);
	std::vector<Vect*> oldColors(RES_X + 1);

	int y = 0;
	for (int x = 0; x < RES_X + 1; x++) {					
		Ray * ray = scene->getCamera()->PrimaryRay(x / NUMEROAMOSTRAS, y / NUMEROAMOSTRAS);
		oldColors[x] = rayTracing(ray, 1, IOR);
		//oldColors.push_back(rayTracing(ray, 1, IOR));							
	}

	for (y = 1; y < RES_Y + 1; y++) {
		int x = 0;
		Ray * ray = scene->getCamera()->PrimaryRay(x / NUMEROAMOSTRAS, y / NUMEROAMOSTRAS);
		newColors[x] = rayTracing(ray, 1, IOR);

		glBegin(GL_POINTS);
		for (int x = 1; x < RES_X + 1; x++) {
			Ray * ray = scene->getCamera()->PrimaryRay(x / NUMEROAMOSTRAS, y / NUMEROAMOSTRAS);
			newColors[x] = rayTracing(ray, 1, IOR);

			std::vector<Vect*> monte(DEPTH_MONTECARLO * DEPTH_MONTECARLO);
			for (int i = 0; i < DEPTH_MONTECARLO * DEPTH_MONTECARLO; i++) monte[i] = nullptr;
			monte[0]										 = newColors[x];
			monte[DEPTH_MONTECARLO - 1]						 = newColors[x - 1];
			monte[DEPTH_MONTECARLO * (DEPTH_MONTECARLO - 1)] = oldColors[x];
			monte[(DEPTH_MONTECARLO * DEPTH_MONTECARLO) - 1] = oldColors[x - 1];

			Vect* color = monteCarlo2(x, y, monte, 0, 0, 1);
			
			glColor3f(color->getX(), color->getY(), color->getZ());
			glVertex2f(x, y);
			
			delete ray;
		}
		glEnd();
		glFlush();
		delete ray;
		oldColors.swap(newColors);
	}
} 

bool checkThreshold(Vect* sw, Vect* se, Vect* ne, Vect* nw) {
	if (!sw->checkDiff(se, DIFF_MONTECARLO)) return false;
	if (!sw->checkDiff(ne, DIFF_MONTECARLO)) return false;
	if (!sw->checkDiff(nw, DIFF_MONTECARLO)) return false;
	if (!nw->checkDiff(ne, DIFF_MONTECARLO)) return false;
	if (!nw->checkDiff(se, DIFF_MONTECARLO)) return false;
	if (!ne->checkDiff(se, DIFF_MONTECARLO)) return false;
	return true;
}

Vect* monteCarlo2(float x, float y, std::vector<Vect*> &monte, int a, int b, int depth) {
	Vect* c1, *c2, *c3, *c4;	//Colors

	int step = (DEPTH_MONTECARLO / depth) -1;

	//ne corner
	int index = (a * DEPTH_MONTECARLO) + b;
	if (monte[index] == nullptr) {
		Ray * ray = scene->getCamera()->PrimaryRay(x, y);
		c1 = rayTracing(ray, 1, IOR);
		monte[index] = c1;
		delete ray;
	} else { c1 = monte[index]; }
	
	//nw corner
	index = (a * DEPTH_MONTECARLO) + b + step;
	if (monte[index] == nullptr) {
		Ray * ray = scene->getCamera()->PrimaryRay((x - (1 / depth)), y);
		c2 = rayTracing(ray, 1, IOR);
		monte[index] = c2;
		delete ray;
	} else { c2 = monte[index]; }

	//se corner
	index = ((a + step) * DEPTH_MONTECARLO) + b;
	if (monte[index] == nullptr) {
		Ray * ray = scene->getCamera()->PrimaryRay(x, (y - (1 / depth)));
		c3 = rayTracing(ray, 1, IOR);
		monte[index] = c3;
		delete ray;
	} else { c3 = monte[index]; }

	//sw corner
	index = ((a + step) * DEPTH_MONTECARLO) + b + step;
	if (monte[index] == nullptr) {
		Ray * ray = scene->getCamera()->PrimaryRay((x - (1 / depth)), (y - (1 / depth)));
		c4 = rayTracing(ray, 1, IOR);
		monte[index] = c4;
		delete ray;
	} else { c4 = monte[index]; }
	


	if (!checkThreshold(c1, c2, c3, c4) && depth < DEPTH_MONTECARLO) {
		depth *= 2;
		c1 = monteCarlo2(x, y, monte, a, b, depth);
		c2 = monteCarlo2((x - (1 / depth)), y, monte, a, b + (DEPTH_MONTECARLO / depth), depth);
		c3 = monteCarlo2(x, (y - (1 / depth)), monte, a + (DEPTH_MONTECARLO / depth), b, depth);
		c4 = monteCarlo2((x - (1 / depth)), (y - (1 / depth)), monte, a + (DEPTH_MONTECARLO / depth), b + (DEPTH_MONTECARLO / depth), depth);
	}
	c1->monteAdd(c2, c3, c4);
	//delete c2;
	//delete c3;
	//delete c4;
	return c1;
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
		Vect * lightD = (USE_SOFTSHADOWS) ? ((Light*)*itL)->getLVectSoft(hit) : ((Light*)*itL)->getLVect(hit);
		

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


int main(int argc, char**argv)
{
	scene = new Scene();
	if (!(scene->load_nff("test_scenes/balls_low.nff"))) return 0;
	
	RES_X = scene->getCamera()->getResX();
	RES_Y = scene->getCamera()->getResY();
	printf("resx = %d resy= %d.\n", RES_X, RES_Y);


	if (USE_OPEN_GL) {
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
	}
	else {
		drawScene_withoutOPENGL(scene);
	}
	
	
	return 0;
}