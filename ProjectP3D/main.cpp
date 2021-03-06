#include "Header.h"
#include <stdlib.h>

void reshape(int w, int h) {
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

void drawScene() {	
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
			Ray * ray = scene->getCamera()->PrimaryRay(x + ((n + (ERAND / NUMEROAMOSTRAS)) / NUMEROAMOSTRAS), y + ((m + (ERAND / NUMEROAMOSTRAS)) / NUMEROAMOSTRAS));
			int index = LIndex(x, y, n, m);
			Vect * sample = rayTracing(ray, 1, IOR, index);
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
			Vect * focalp = scene->getCamera()->GetFocalPoint(x + ((n + (ERAND / NUMEROAMOSTRAS)) / NUMEROAMOSTRAS), y + ((m + (ERAND / NUMEROAMOSTRAS)) / NUMEROAMOSTRAS));
			for (int o = 0; o < NUMEROAMOSTRAS_DOF; o++) {
				for (int q = 0; q < NUMEROAMOSTRAS_DOF; q++) {
					Ray * ray = scene->getCamera()->PrimaryRayDOF(focalp);
					int index = LIndex(x, y, n, m, o, q);
					Vect * sample = rayTracing(ray, 1, IOR, index);
					color->add(sample); //depth=1, ior=1.0
					delete ray;
					delete sample;
				}
			}
			delete focalp;
		}
	}
	color->multiply((float)1 / (NUMEROAMOSTRAS_DOF * NUMEROAMOSTRAS_DOF * NUMEROAMOSTRAS * NUMEROAMOSTRAS));
}

void castDOF(Vect * color, int x, int y) {
	Vect * focalp = scene->getCamera()->GetFocalPoint(x, y);
	for (int o = 0; o < NUMEROAMOSTRAS_DOF; o++) {
		for (int q = 0; q < NUMEROAMOSTRAS_DOF; q++) {
			Ray * ray = scene->getCamera()->PrimaryRayDOF(focalp);
			Vect * sample = rayTracing(ray, 1, IOR, 0);
			color->add(sample); //depth=1, ior=1.0
			delete ray;
			delete sample;
		}
	}
	delete focalp;
	color->multiply((float)1 / (NUMEROAMOSTRAS_DOF * NUMEROAMOSTRAS_DOF));
}

int LIndex(int x, int y, int m, int n) {
	return n + m * NUMEROAMOSTRAS + x * (NUMEROAMOSTRAS * NUMEROAMOSTRAS) + y * (NUMEROAMOSTRAS * NUMEROAMOSTRAS * RES_X);
}

int LIndex(int x, int y, int m, int n, int o, int q) {
	return q + o * NUMEROAMOSTRAS_DOF + n * (NUMEROAMOSTRAS_DOF * NUMEROAMOSTRAS_DOF) + 
		m * (NUMEROAMOSTRAS_DOF * NUMEROAMOSTRAS_DOF * NUMEROAMOSTRAS) + x * (NUMEROAMOSTRAS_DOF * NUMEROAMOSTRAS_DOF * NUMEROAMOSTRAS * NUMEROAMOSTRAS) + 
		y * (NUMEROAMOSTRAS_DOF * NUMEROAMOSTRAS_DOF * NUMEROAMOSTRAS * NUMEROAMOSTRAS * RES_X);
}

void monteCarlo() {

	std::vector<Vect*> newColors(RES_X + 1);
	std::vector<Vect*> oldColors(RES_X + 1);
	Ray * ray;

	int y = 0;
	for (int x = 0; x < RES_X + 1; x++) {					
		if (USE_DOF) {
			Vect* color = new Vect();
			castDOF(color, x, y);
			oldColors[x] = color;
		} else {
			ray = scene->getCamera()->PrimaryRay(x, y);
			oldColors[x] = rayTracing(ray, 1, IOR, 0);
		}				
	}

	for (y = 1; y < RES_Y + 1; y++) {
		int x = 0;

		if (USE_DOF) {
			Vect* color = new Vect();
			castDOF(color, x, y);
			newColors[x] = color;
		} else {
			ray = scene->getCamera()->PrimaryRay(x, y);
			newColors[x] = rayTracing(ray, 1, IOR, 0);
		}

		glBegin(GL_POINTS);
		for (int x = 1; x < RES_X + 1; x++) {
			if (USE_DOF) {
				Vect* color = new Vect();
				castDOF(color, x, y);
				newColors[x] = color;
			} else {
				ray = scene->getCamera()->PrimaryRay(x, y);
				newColors[x] = rayTracing(ray, 1, IOR, 0);
			}

			std::vector<Vect*> monte(SIZE_MONTECARLO * SIZE_MONTECARLO);
			//for (int i = 0; i < SIZE_MONTECARLO * SIZE_MONTECARLO; i++) monte[i] = nullptr;
			monte[0]										 = newColors[x];
			monte[SIZE_MONTECARLO - 1]						 = newColors[x - 1];
			monte[SIZE_MONTECARLO * (SIZE_MONTECARLO - 1)] = oldColors[x];
			monte[(SIZE_MONTECARLO * SIZE_MONTECARLO) - 1] = oldColors[x - 1];

			Vect* color = monteCarlo2(x, y, monte, 0, 0, 1);
			
			glColor3f(color->getX(), color->getY(), color->getZ());
			glVertex2f(x, y);
		}
		glEnd();
		glFlush();
		if(!USE_DOF) delete ray;
		oldColors.swap(newColors);
	}
} 

bool checkThreshold(Vect* sw, Vect* se, Vect* ne, Vect* nw) {
	if (!sw->checkDiff(se, DIFF_MONTECARLO)) return false;
	if (!sw->checkDiff(ne, DIFF_MONTECARLO*1.4)) return false;
	if (!sw->checkDiff(nw, DIFF_MONTECARLO)) return false;
	if (!nw->checkDiff(ne, DIFF_MONTECARLO)) return false;
	if (!nw->checkDiff(se, DIFF_MONTECARLO*1.4)) return false;
	if (!ne->checkDiff(se, DIFF_MONTECARLO)) return false;
	return true;
}

Vect* monteCarlo2(float x, float y, std::vector<Vect*> &monte, int a, int b, int depth) {
	Vect* c1, *c2, *c3, *c4;	//Colors
	Ray * ray;

	int step = (DEPTH_MONTECARLO / depth);

	//ne corner
	int index = (a * SIZE_MONTECARLO) + b;
	if (monte[index] == nullptr) {
		if (USE_DOF) {
			Vect* color = new Vect();
			castDOF(color, x, y);
			c1 = color;
		} else {
			ray = scene->getCamera()->PrimaryRay(x, y);
			c1 = rayTracing(ray, 1, IOR, 0);
			delete ray;
		}
		monte[index] = c1;
	} else { c1 = monte[index]; }
	
	//nw corner
	index = (a * SIZE_MONTECARLO) + b + step;
	if (monte[index] == nullptr) {
		if (USE_DOF) {
			Vect* color = new Vect();
			castDOF(color, x, y);
			c2 = color;
		} else {
			ray = scene->getCamera()->PrimaryRay(x, y);
			c2 = rayTracing(ray, 1, IOR, 0);
			delete ray;
		}
		monte[index] = c2;
	} else { c2 = monte[index]; }

	//se corner
	index = ((a + step) * SIZE_MONTECARLO) + b;
	if (monte[index] == nullptr) {
		if (USE_DOF) {
			Vect* color = new Vect();
			castDOF(color, x, y);
			c3 = color;
		} else {
			ray = scene->getCamera()->PrimaryRay(x, y);
			c3 = rayTracing(ray, 1, IOR, 0);
			delete ray;
		}
		monte[index] = c3;
	} else { c3 = monte[index]; }

	//sw corner
	index = ((a + step) * SIZE_MONTECARLO) + b + step;
	if (monte[index] == nullptr) {
		if (USE_DOF) {
			Vect* color = new Vect();
			castDOF(color, x, y);
			c4 = color;
		} else {
			ray = scene->getCamera()->PrimaryRay(x, y);
			c4 = rayTracing(ray, 1, IOR, 0);
			delete ray;
		}
		monte[index] = c4;
	} else { c4 = monte[index]; }
	


	if (!checkThreshold(c1, c2, c3, c4) && depth < DEPTH_MONTECARLO) {
		depth *= 2;
		c1 = monteCarlo2(x, y, monte, a, b, depth);
		c2 = monteCarlo2((x - (1 / depth)), y, monte, a, b + (DEPTH_MONTECARLO / depth), depth);
		c3 = monteCarlo2(x, (y - (1 / depth)), monte, a + (DEPTH_MONTECARLO / depth), b, depth);
		c4 = monteCarlo2((x - (1 / depth)), (y - (1 / depth)), monte, a + (DEPTH_MONTECARLO / depth), b + (DEPTH_MONTECARLO / depth), depth);
	}
	c1->monteAdd(c2, c3, c4);
	return c1;
}



Vect * rayTracing(Ray * ray, int depth, float ior, int index) {
	std::list<Obj*> objs = scene->getObjects();
	std::list<Obj*>::iterator itO;

	Obj* closest = nullptr;									//the closest object to the camera that the ray hits
	float dist = HUGE_VALUE;
	Vect * hit = nullptr;

	if (USE_GRID) {
		intersection * i = scene->getGrid()->traverse(ray);
		
		/// Check if there is an intersection with the objects in the grid
		if (i == nullptr) {
			/// Check if the plane is intersected
			if (scene->getPlane() != nullptr) {
				float distance_aux = scene->getPlane()->intersect(ray);
				if (distance_aux > EPSILON && distance_aux < dist) {
					dist = distance_aux;
					closest = (Obj*)scene->getPlane();
					hit = ray->getHitPoint(dist);
				}
				else
					return new Vect(scene->getBackground());
			}
			else
				return new Vect(scene->getBackground());
		}
		else {
			closest = i->object;
			dist = i->distance;
			hit = ray->getHitPoint(dist);
			delete i;
		}
	}
	else {
		float dist = 9999, distNew = 0;
		for (itO = objs.begin(); itO != objs.end(); itO++) {	//Iterates over all objects
			distNew = ((Obj*)*itO)->intersect(ray);				//Intersect returns distance from hitpoint to camera
			if (distNew > EPSILON && distNew < dist) {			//If distance is smaller than all previous distances
				dist = distNew;									//Then save distance and object
				closest = (Obj*)*itO;
			}
		}
		hit = ray->getHitPoint(dist);
	}
	
	
	if (closest == nullptr)									//If the ray doesn't intersect any object
		return new Vect(scene->getBackground());						
	

	std::list<Light*> lights = scene->getLights();
	std::list<Light*>::iterator itL;
	Vect* color = new Vect();
	Vect* normal = closest->getNormal(hit);
	Vect * lightD;
	//Local ilumination
	for (itL = lights.begin(); itL != lights.end(); itL++) {			//Iterates over all the lights
		if (USE_SOFTSHADOWS) {
			lightD = ((Light*)*itL)->getLVectSoft(hit);
		} else if (USE_ARRAY_SOFTSHADOWS) {
			lightD = ((Light*)*itL)->getLVectArrays(hit, index);
		} else { 
			lightD = ((Light*)*itL)->getLVect(hit); 
		}

		

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
		Vect * reflectColor = rayTracing(reflectRay, depth + 1, ior, index);		//Compute reflection color
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
			Vect * refractColor = rayTracing(refractRay, depth + 1, ior, index);		//Compute refraction color
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
	srand((unsigned)time(NULL));
	scene = new Scene();
	if (!(scene->load_nff("test_scenes/balls_low.nff"))) return 0;
	
	RES_X = scene->getCamera()->getResX();
	RES_Y = scene->getCamera()->getResY();
	printf("resx = %d resy= %d.\n", RES_X, RES_Y);
	if (USE_GRID) {
		printf("initializing grid... ");
		INIT_TIMER
		START_TIMER
		scene->initGrid();
		STOP_TIMER("DONE\n")
	}
	
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